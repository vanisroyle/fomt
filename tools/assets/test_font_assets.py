"""Font tests use synthetic pixels/maps only, never extracted assets."""

from pathlib import Path
import struct
import tempfile
import unittest
from unittest.mock import patch
import zlib

import font_assets as font


class FontTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)

    def test_png_roundtrip(self):
        rows = [bytearray((x + y) % 2 for x in range(17)) for y in range(13)]
        self.assertEqual(font.read_png(font.write_png(rows)), rows)
        self.assertEqual(font.read_png(font.write_png([bytearray(1)])), [bytearray(1)])

    def test_png_filter_types_and_palette_order(self):
        rows = [bytes([0b10101010, 0b11000000]), bytes([0b00110011, 0b01000000])]
        for filter_type in range(5):
            previous = bytes(2)
            raw = bytearray()
            for row in rows:
                raw.append(filter_type)
                for i, value in enumerate(row):
                    a, b, c = (row[i - 1] if i else 0), previous[i], (previous[i - 1] if i else 0)
                    prediction = (0, a, b, (a + b) // 2, font.paeth(a, b, c))[filter_type]
                    raw.append((value - prediction) & 255)
                previous = row
            png = (font.SIGNATURE + font.png_chunk(b"IHDR", struct.pack(">IIBBBBB", 10, 2, 1, 0, 0, 0, 0))
                   + font.png_chunk(b"IDAT", zlib.compress(raw)) + font.png_chunk(b"IEND", b""))
            expected = [bytearray(1 - ((row[x // 8] >> (7 - x % 8)) & 1) for x in range(10)) for row in rows]
            with self.subTest(filter=filter_type):
                self.assertEqual(font.read_png(png), expected)
        png = font.write_png([bytearray([0, 1])])
        self.assertEqual(font.read_png(png), [bytearray([0, 1])])

    def test_indexed_depths(self):
        for depth in (1, 2, 4, 8):
            raw = bytes([0, 1 << (8 - 2 * depth)]) if depth < 8 else b"\0\0\1"
            png = (font.SIGNATURE + font.png_chunk(b"IHDR", struct.pack(">IIBBBBB", 2, 1, depth, 3, 0, 0, 0))
                   + font.png_chunk(b"PLTE", b"\xff\xff\xff\0\0\0")
                   + font.png_chunk(b"IDAT", zlib.compress(raw)) + font.png_chunk(b"IEND", b""))
            self.assertEqual(font.read_png(png), [bytearray([0, 1])])

    def test_bad_png(self):
        valid = font.write_png([bytearray([0, 1])])
        for data in (b"", valid[:-1], valid + b"extra", valid[:40] + bytes([valid[40] ^ 1]) + valid[41:]):
            with self.subTest(data=data), self.assertRaises(ValueError):
                font.read_png(data)
        for rows in ([], [bytearray()], [bytearray([2])], [bytearray(2), bytearray(1)]):
            with self.assertRaises(ValueError):
                font.write_png(rows)

    def test_font_byte_order_and_edit(self):
        for width in (8, 16):
            data = bytes((i * 37) % 256 for i in range(33 * 12 * (width // 8)))
            rows = font.to_sheet(data, 33, width)
            self.assertEqual(font.from_sheet(rows, 33, width), data)
            rows[12][width + 3] ^= 1  # glyph 33 is padding: must reject
            with self.assertRaisesRegex(ValueError, "unused sheet cells"):
                font.from_sheet(rows, 33, width)
            rows[12][width + 3] ^= 1
            rows[4][2] ^= 1
            edited = font.from_sheet(rows, 33, width)
            expected = bytearray(data)
            expected[4 * (width // 8)] ^= 0x20
            self.assertEqual(edited, expected)
            with self.assertRaisesRegex(ValueError, "exactly"):
                font.from_sheet(rows[:-1], 33, width)

    def test_map_roundtrip_and_errors(self):
        path = self.root / "single.map"
        values = [-1] * 256
        path.write_text(font.map_text("single", values))
        self.assertEqual(list(font.read_map(path, "single").values()), values)
        path.write_text("\ufeff" + font.map_text("single", values))
        self.assertEqual(list(font.read_map(path, "single").values()), values)
        for source in ("00 = -1\n", font.map_text("single", values) + "00 = 1\n",
                       font.map_text("single", values).replace("00 = -1", "00 = 487"),
                       font.map_text("single", values).replace("00 = -1", "00 = -2")):
            path.write_text(source)
            with self.assertRaises(ValueError):
                font.read_map(path, "single")
        self.assertEqual(len(font.codes("double")), 7938)

    def test_safe_codes(self):
        single = dict.fromkeys(range(256), -1)
        safe = font.safe_codes(single)
        self.assertNotIn(0xFF, safe)
        self.assertNotIn(0, safe)
        self.assertNotIn(0x81, safe)
        self.assertNotIn(0xEA, safe)
        self.assertIn(0xEB, safe)
        self.assertEqual(len(font.vietnamese_letters()), 134)

    def test_safe_codes_excludes_hardcoded_glyphs_even_when_unmapped(self):
        single = dict.fromkeys(range(256), -1)
        safe = set(font.safe_codes(single))
        hardcoded = {0xB1, 0xB2, 0xB3, 0xB4, 0xB6, 0xB7,
                     0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xC0, 0xC1, 0xC2, 0xC3}
        self.assertEqual(safe & hardcoded, set())
        self.assertTrue({0xB5, 0xB8, 0xB9, 0xBA, 0xEB} <= safe)

    def sample_fixture(self):
        root = self.root / "fonts"
        root.mkdir()
        single = [-1] * 256
        single[ord("a")] = 0
        (root / "single.map").write_text(font.map_text("single", single))
        (root / "double.map").write_text(font.map_text("double", [-1] * 7938))
        pixels = bytearray(487 * 12)
        pixels[0] = 0x18
        pixels[12] = 1  # author has drawn an unmapped glyph: reserve this slot
        (root / "small.png").write_bytes(font.write_png(font.to_sheet(pixels, 487, 8)))
        (self.root / "charmap.txt").write_text("# preserve this comment\n")

    def test_sample_preserves_edits_and_is_idempotent(self):
        self.sample_fixture()
        with patch.object(font, "original_rom", return_value=bytes(0x4FA7A0)):
            font.sample(Path("unused"), self.root, "ạ", placeholders=True)
            mapping = font.fa.charmap(self.root / "charmap.txt")
            code = mapping["ạ"][0]
            single = font.read_map(self.root / "fonts/single.map", "single")
            self.assertEqual(single[code], 2)
            rows = font.read_png((self.root / "fonts/small.png").read_bytes())
            data = font.from_sheet(rows, 487, 8)
            self.assertEqual(data[12], 1)
            self.assertEqual(data[24], 0x18)
            before = {p: p.read_bytes() for p in self.root.rglob("*") if p.is_file()}
            font.sample(Path("unused"), self.root, "ạ", placeholders=True)
            self.assertTrue(all(p.read_bytes() == data for p, data in before.items()))

    def test_sample_exhaustion_does_not_overwrite(self):
        self.sample_fixture()
        before = {p: p.read_bytes() for p in self.root.rglob("*") if p.is_file()}
        with patch.object(font, "original_rom", return_value=bytes(0x4FA7A0)), patch.object(font, "safe_codes", return_value=[]):
            with self.assertRaisesRegex(ValueError, "no edits made"):
                font.sample(Path("unused"), self.root, "ạ")
        self.assertTrue(all(p.read_bytes() == data for p, data in before.items()))

    def test_streaming_codes(self):
        single = dict.fromkeys(range(256), -1)
        single.update({ord("A"): 0, 0x7F: 1})
        double = {0x889F: 0, 0x88A0: 1, 0x887F: 2}
        data = b"A\x88\x9f\r\n\xff\x21\x88\xa0\x05\x88\x7f\x7f"
        self.assertEqual(font.stream_codes(data, single, double), set(double))
        self.assertEqual(font.stream_codes(b"", single, double), set())
        # Mapped leads are rendered alone, not combined with the next byte.
        single[0x88] = 2
        self.assertEqual(font.stream_codes(b"\x88\x9f", single, double), set())

    def test_reclaim_checks_every_alias_and_original_mapping(self):
        double = {0x889F: 0, 0x88A0: 0, 0x88A1: 1, 0x8140: 2, 0x88A2: 3}
        unused, kanji = font.reclaimable_slots(double, {0x88A0})
        self.assertNotIn(0, unused)
        self.assertIn(2, unused)
        self.assertEqual(kanji, [1, 3])
        _, kanji = font.reclaimable_slots(double, {0x88A3}, {0x88A3: 1})
        self.assertNotIn(1, kanji)
        single = dict.fromkeys(range(256), -1)
        single[0x88] = 1
        self.assertEqual(font.accepted_free_codes(single, {0x889F: -1, 0x8940: -1}), [0x8940])

    def test_usage_includes_original_and_current_scripts(self):
        maps = {"single": dict.fromkeys(range(256), -1), "double": {0x889F: 0, 0x88A0: 1}}
        payload = font.fa.pack(1) + font.fa.pack(0) + b"\x88\x9f\0"
        with (patch.object(font, "rom_maps", return_value=maps),
              patch.object(font.fa, "COUNT", 1),
              patch.object(font.fa, "rom_scripts", return_value=[(1, 0, b"script")]),
              patch.object(font.fa, "chunks", return_value=[(b"STR ", payload)]),
              patch.object(font.fa, "charmap", return_value={}),
              patch.object(font.fa, "read_text", return_value=[b"\x88\xa0"])):
            used, count = font.script_usage(b"rom", self.root, maps["single"], maps["double"])
        self.assertEqual(used, {0x889F, 0x88A0})
        self.assertEqual(count, 1)

    def large_fixture(self):
        self.sample_fixture()
        path = self.root / "fonts/double.map"
        source = path.read_text().replace("889F = -1", "889F = 0").replace("88A0 = -1", "88A0 = 1")
        path.write_text(source)
        pixels = bytearray(6922 * 24)
        pixels[0] = 1  # edited, unused kanji: do not overwrite
        (self.root / "fonts/large.png").write_bytes(font.write_png(font.to_sheet(pixels, 6922, 16)))

    def test_large_sample_preserves_edits_and_reservations(self):
        self.large_fixture()
        with (patch.object(font, "original_rom", return_value=bytes(0x527094)),
              patch.object(font, "rom_maps", return_value={"double": {0x889F: 0, 0x88A0: 1}}),
              patch.object(font, "script_usage", return_value=(set(), 0))):
            font.sample(Path("unused"), self.root, "ạ", placeholders=True, reclaim_large=True)
            mapping = font.fa.charmap(self.root / "charmap.txt")
            self.assertEqual(mapping["ạ"], b"\x88\xa1")
            double = font.read_map(self.root / "fonts/double.map", "double")
            self.assertEqual(double[0x88A1], 1)
            self.assertEqual(double[0x88A0], 1)  # original alias retained
            data = font.from_sheet(font.read_png((self.root / "fonts/large.png").read_bytes()), 6922, 16)
            self.assertEqual(data[0], 1)
            self.assertEqual(data[24:26], b"\x01\x80")
            before = {p: p.read_bytes() for p in self.root.rglob("*") if p.is_file()}
            font.sample(Path("unused"), self.root, "ạ", placeholders=True, reclaim_large=True)
            self.assertTrue(all(p.read_bytes() == raw for p, raw in before.items()))

    def test_large_assignment_reserves_slot_without_placeholder(self):
        self.large_fixture()
        path = self.root / "fonts/large.png"
        saved = path.read_bytes()
        with (patch.object(font, "original_rom", return_value=bytes(0x527094)),
              patch.object(font, "rom_maps", return_value={"double": {0x889F: 0, 0x88A0: 1}}),
              patch.object(font, "script_usage", return_value=(set(), 0))):
            font.sample(Path("unused"), self.root, "☃", reclaim_large=True)
            self.assertEqual(path.read_bytes(), saved)
            before = {p: p.read_bytes() for p in self.root.rglob("*") if p.is_file()}
            with self.assertRaisesRegex(ValueError, "no edits made"):
                font.sample(Path("unused"), self.root, "☃ầ", reclaim_large=True)
            self.assertTrue(all(p.read_bytes() == raw for p, raw in before.items()))

    def test_large_assignment_preserves_multi_glyph_charmap(self):
        self.large_fixture()
        (self.root / "charmap.txt").write_text("U+2603 = 88 9F 88 A0\n")
        before = {p: p.read_bytes() for p in self.root.rglob("*") if p.is_file()}
        with (patch.object(font, "original_rom", return_value=bytes(0x527094)),
              patch.object(font, "rom_maps", return_value={"double": {0x889F: 0, 0x88A0: 1}}),
              patch.object(font, "script_usage", return_value=(set(), 0))):
            with self.assertRaisesRegex(ValueError, "no edits made"):
                font.sample(Path("unused"), self.root, "ạ", placeholders=True, reclaim_large=True)
        self.assertTrue(all(p.read_bytes() == raw for p, raw in before.items()))

    def test_sample_rewrites_all_accepted_single_map_row_spellings(self):
        self.sample_fixture()
        path = self.root / "fonts/single.map"
        original = path.read_text()
        for row in ("  A0 = -1", "\t0a0\t= -01 # keep", "  00a0 = -001  # keep",
                    "00A0 = -٠١ # keep"):
            with self.subTest(row=row):
                source = "\ufeff" + original.replace("A0 = -1", row)
                path.write_text(source)
                (self.root / "charmap.txt").write_text("# preserve\n")
                self.assertEqual(font.read_map(path, "single")[0xA0], -1)
                with (patch.object(font, "original_rom", return_value=bytes(0x4FA7A0)),
                      patch.object(font, "safe_codes", return_value=[0xA0])):
                    font.sample(Path("unused"), self.root, "ạ")
                self.assertEqual(font.read_map(path, "single")[0xA0], 2)
                expected = source.replace(row, row.replace(row.split("=", 1)[1].split("#", 1)[0].strip(), "2"))
                self.assertEqual(path.read_text(), expected)

    def test_vietnamese_composition_all_letters_and_tone_families(self):
        single = {ord(c): i for i, c in enumerate("aeiouyAEIOUYDd")}
        small = b"".join(bytes([0x10, 0x28] + [0x44] * 6 + [0x38, 0, 0, 0]) for _ in single)
        for char in font.vietnamese_letters():
            with self.subTest(char=char):
                glyph = font.vietnamese_glyph(char, small, single)
                self.assertEqual(len(glyph), 24)
                self.assertTrue(any(glyph))
                if "\u0323" in font.unicodedata.normalize("NFD", char):
                    self.assertEqual(int.from_bytes(glyph[22:24], "big"), 1 << 8)
        for family in ("ăằắẳẵặ", "âầấẩẫậ", "êềếểễệ", "ôồốổỗộ", "ơờớởỡợ", "ưừứửữự"):
            for letters in (family, family.upper()):
                self.assertEqual(len({font.vietnamese_glyph(c, small, single) for c in letters}), 6)
        for char in ("", "x", "ệế"):
            with self.assertRaises(ValueError):
                font.vietnamese_glyph(char, small, single)
        with self.assertRaisesRegex(ValueError, "base glyph"):
            font.vietnamese_glyph("ạ", b"", {})

    def test_reclaimed_vietnamese_is_drawn_without_placeholder_flag(self):
        self.large_fixture()
        with (patch.object(font, "original_rom", return_value=bytes(0x527094)),
              patch.object(font, "rom_maps", return_value={"double": {0x889F: 0, 0x88A0: 1}}),
              patch.object(font, "script_usage", return_value=(set(), 0))):
            font.sample(Path("unused"), self.root, "ạ", reclaim_large=True)
        small = font.from_sheet(font.read_png((self.root / "fonts/small.png").read_bytes()), 487, 8)
        large = font.from_sheet(font.read_png((self.root / "fonts/large.png").read_bytes()), 6922, 16)
        single = font.read_map(self.root / "fonts/single.map", "single")
        self.assertEqual(large[24:48], font.vietnamese_glyph("ạ", small, single))


if __name__ == "__main__":
    unittest.main()
