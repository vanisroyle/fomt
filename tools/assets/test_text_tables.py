"""Synthetic non-script text and pointer records; no ROM data."""

from contextlib import redirect_stdout
from copy import deepcopy
import io
from pathlib import Path
import tempfile
import unittest
from unittest.mock import patch

import fomt_assets as fa
import text_tables as text


class TextTableTests(unittest.TestCase):
    def setUp(self):
        self.temporary = tempfile.TemporaryDirectory()
        self.addCleanup(self.temporary.cleanup)
        self.assets = Path(self.temporary.name)
        (self.assets / "charmap.txt").write_text("", encoding="utf-8")
        self.single = {code: code for code in range(32, 128)}
        self.double = {0x82A0: 10, 0x8540: 0x4001}
        self.single[0x82] = self.single[0x85] = -1
        self.rom = b"AB\0padding\0"
        self.records = [{"address": 0, "length": 3, "groups": ["example"], "fixed": 0}]

    def prepare(self, records=None, pointers=None):
        records = self.records if records is None else records
        self.start_patch("entries", side_effect=lambda rom: (deepcopy(records), pointers or {}))
        self.start_patch("font.read_map", side_effect=lambda path, kind: getattr(self, kind))
        return text.extract(self.rom, self.assets)

    def start_patch(self, name, **kwargs):
        mocked = patch("text_tables." + name, **kwargs)
        self.addCleanup(mocked.stop)
        return mocked.start()

    def replace_source(self, source):
        (self.assets / "text_tables/strings.txt").write_text(source, encoding="utf-8")

    def test_extract_roundtrip_and_preserve_edits(self):
        self.prepare()
        self.assertEqual(bytes.fromhex(text.edited(self.rom, self.assets)[0]["value"]), b"AB")
        self.replace_source("=== 0\n|Z\n")
        text.extract(self.rom, self.assets)
        result = text.edited(self.rom, self.assets)[0]
        self.assertEqual(result["value"], "5a")
        self.assertFalse(result["moved"])
        text.extract(self.rom, self.assets, force=True)
        self.assertEqual(text.edited(self.rom, self.assets)[0]["value"], "4142")

    def test_empty_catalog(self):
        self.prepare([])
        self.assertEqual(text.edited(self.rom, self.assets), [])

    def test_cp932_controls_and_charmap(self):
        raw = b"A\r\n\x05\xff\x01" + "あ".encode("cp932")
        self.rom = raw + b"\0"
        self.records[0]["length"] = len(self.rom)
        self.prepare()
        self.assertEqual(bytes.fromhex(text.edited(self.rom, self.assets)[0]["value"]), raw)
        (self.assets / "charmap.txt").write_text("U+1EA1 = 85 40\n", encoding="utf-8")
        self.replace_source("=== 0\n|ạ\n")
        self.assertEqual(text.edited(self.rom, self.assets)[0]["value"], "8540")

    def test_bad_ids_encoding_and_nul(self):
        self.prepare()
        for source, error in (("=== 1\n|A\n", "sequential"), ("=== 0\n|{00}\n", "NUL"),
                              ("=== 0\n|😀\n", "cannot encode"), ("# deleted\n", "keep all")):
            with self.subTest(source=source):
                self.replace_source(source)
                with self.assertRaisesRegex(ValueError, error):
                    text.edited(self.rom, self.assets)

    def test_growing_string_needs_audited_pointer(self):
        self.prepare()
        self.replace_source("=== 0\n|Larger name\n")
        with self.assertRaisesRegex(ValueError, "cannot grow text.*no audited symbolic pointer"):
            text.edited(self.rom, self.assets)

    def test_symbolic_growth_preserves_slot_and_emits_pointer(self):
        self.rom += fa.pack(fa.BASE)
        self.prepare(pointers={len(self.rom) - 4: 0})
        self.replace_source("=== 0\n|Larger name\n")
        record = text.edited(self.rom, self.assets)[0]
        self.assertTrue(record["moved"])
        self.assertEqual(text.slot_bytes(self.rom, 0, 3, [record]), self.rom[:3])
        output = self.assets / "generated"
        fragment = text.fragment(self.rom, 0, len(self.rom), [record], {len(self.rom) - 4: 0}, output).decode()
        self.assertIn('.pushsection .text_table_overflow,"a",%progbits', fragment)
        self.assertIn(".4byte gNonScriptText_000000", fragment)
        self.assertIn(str(output / "text-tables-000000.bin"), fragment)

    def test_shorter_string_does_not_clear_unrelated_bytes(self):
        record = dict(self.records[0], moved=False, value="5a")
        self.assertEqual(text.slot_bytes(self.rom, 0, len(self.rom), [record]), b"Z\0\0padding\0")

    def test_fixed_byte_rows_keep_width_and_single_byte_cells(self):
        self.records[0]["fixed"] = 3
        self.prepare()
        self.replace_source("=== 0\n|A\n")
        with self.assertRaisesRegex(ValueError, "retain their byte width"):
            text.edited(self.rom, self.assets)
        self.replace_source("=== 0\n|あ\n")
        with self.assertRaisesRegex(ValueError, "byte-indexed"):
            text.edited(self.rom, self.assets)

    def test_fullwidth_name_grid_keeps_cell_boundaries(self):
        address = 0xE89B6 + 6 * 31
        self.rom = bytes(address) + b"\x82\xa0\0"
        self.records[0].update(address=address, fixed=31)
        self.prepare()
        self.assertFalse(text.edited(self.rom, self.assets)[0]["moved"])
        for source in ("AB", "{85}{40}"):
            self.replace_source(f"=== 0\n|{source}\n")
            with self.assertRaisesRegex(ValueError, "full-width name-entry cells"):
                text.edited(self.rom, self.assets)

    def test_halfwidth_name_grid_keeps_separators(self):
        address = 0xE89B6
        self.rom = bytes(address) + b"A B \0"
        self.records[0].update(address=address, length=5, fixed=31)
        self.prepare()
        self.replace_source("=== 0\n|C D \n")
        self.assertEqual(text.edited(self.rom, self.assets)[0]["value"], "43204420")
        self.replace_source("=== 0\n|C_D \n")
        with self.assertRaisesRegex(ValueError, "separators"):
            text.edited(self.rom, self.assets)

    def test_glyph16_tables_are_little_endian_and_fixed_cell_count(self):
        self.rom = b"A\0B\0"
        self.records[0].update(length=4, fixed=4, kind="glyph16")
        self.prepare()
        self.assertEqual(text.source_value(self.rom, self.records[0]), b"AB")
        self.assertEqual(text.edited(self.rom, self.assets)[0]["value"], "41004200")
        self.replace_source("=== 0\n|{85}{40}B\n")
        self.assertEqual(text.edited(self.rom, self.assets)[0]["value"], "40854200")
        self.replace_source("=== 0\n|あB\n")
        with self.assertRaisesRegex(ValueError, "8px glyph"):
            text.edited(self.rom, self.assets)
        self.replace_source("=== 0\n|A\n")
        with self.assertRaisesRegex(ValueError, "glyph count"):
            text.edited(self.rom, self.assets)
        for raw, error in ((b"\x82", "incomplete"), (b"\x82\x40", "unmapped")):
            with self.assertRaisesRegex(ValueError, error):
                text.glyph_codes(raw, self.single, self.double)

    def test_entries_deduplicate_pointer_targets_and_reject_bad_roots(self):
        for name in ("ITEMS", "ARRAYS", "POINTERS", "FIXED", "BYTE_ROWS", "GLYPHS", "SHOPS", "ROOTS"):
            self.start_patch("catalog." + name, new=())
        self.start_patch("catalog.LIBRARY", new=(0, 0))
        with patch.object(text.catalog, "ARRAYS", (("test", 8, 2),)):
            rom = b"AB\0xxxxx" + fa.pack(fa.BASE) * 2
            records, pointers = text.entries(rom)
            self.assertEqual(len(records), 1)
            self.assertEqual(records[0]["groups"], ["test/000", "test/001"])
            self.assertEqual(pointers, {8: 0, 12: 0})
        with patch.object(text.catalog, "ROOTS", (99,)):
            with self.assertRaisesRegex(ValueError, "outside ROM"):
                text.entries(b"A\0")
        with patch.object(text.catalog, "ROOTS", (0, 1)):
            with self.assertRaisesRegex(ValueError, "overlapping"):
                text.entries(b"AB\0")
        with patch.object(text.catalog, "ROOTS", (0,)):
            with self.assertRaisesRegex(ValueError, "unterminated"):
                text.entries(b"AB")

    def test_verify_checks_symbolic_and_interior_references(self):
        rom = self.rom + fa.pack(fa.BASE) + fa.pack(fa.BASE + 1)
        target = len(rom)
        record = dict(self.records[0], moved=True, value=b"Larger name".hex())
        linked = bytearray(rom + b"Larger name\0")
        self.start_patch("elf_symbols", return_value={text.symbol(0): (".rom_tail", fa.BASE + target)})
        with self.assertRaisesRegex(ValueError, "non-symbolic reference"):
            text.verify(rom, linked, b"", [record])
        linked[len(self.rom):target] = fa.pack(fa.BASE + target) + fa.pack(fa.BASE + target + 1)
        with redirect_stdout(io.StringIO()):
            text.verify(rom, linked, b"", [record])
        linked[target] = 0
        with self.assertRaisesRegex(ValueError, "linked bytes differ"):
            text.verify(rom, linked, b"", [record])


if __name__ == "__main__":
    unittest.main()
