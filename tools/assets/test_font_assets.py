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


if __name__ == "__main__":
    unittest.main()
