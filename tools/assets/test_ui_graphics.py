"""Synthetic UI ownership, partial tiles, patch roots and read-past guards."""

from contextlib import ExitStack, contextmanager
from pathlib import Path
import struct
import tempfile
import unittest
from unittest.mock import patch

import graphics_assets as g
import indexed_png
import ui_graphics as ui


@contextmanager
def fixture(compressed=True):
    rom = bytearray(512)
    tiles = len(rom)
    rom += g.codec.encode(bytes(32) + b"\x01" + bytes(31))
    palette = len(rom)
    rom += struct.pack("<16H", *range(16))
    tilemap = len(rom)
    raw_map = struct.pack("<2H", 0, 1)
    rom += g.codec.encode(raw_map) if compressed else raw_map
    struct.pack_into("<3I", rom, 0, *(g.fa.BASE + a for a in (tiles, palette, tilemap)))
    entry = ui.scene("fixture", tiles, (palette, 32), ((tilemap, 8),), 0, 4, 2, 1, compressed=compressed)
    with ExitStack() as stack, tempfile.TemporaryDirectory() as directory:
        for name, value in (("SCENES", (entry,)), ("SHARED_MAP", None), ("PATCHES", ()), ("BLOCKS", ()), ("PALETTE_TAILS", ())):
            stack.enter_context(patch.object(ui, name, value))
        stack.enter_context(patch.object(ui.world, "catalogue", return_value=([], {})))
        stack.enter_context(patch.object(ui.world, "NON_POINTER_STREAMS", ()))
        stack.enter_context(patch.object(ui.world, "INSTRUCTION_BYTES", ()))
        yield rom, Path(directory) / "assets", Path(directory) / "custom output", entry


class UiGraphicsTests(unittest.TestCase):
    def test_shared_parent_palette_keeps_actual_bank_tags(self):
        entry = ui.scene("parent", 1, (2, 32), ((3, 0),), 0, 0, 1, 1, base=7, palette_parent=4)
        streams = {1: {"raw": b"\x11" * 32}, 3: {"raw": b"\x00\x20"}}
        self.assertEqual(ui.hints(entry, streams), [2])

    def test_roundtrip_slots_custom_paths_and_extraction_preserves_edits(self):
        with fixture() as (rom, assets, output, entry):
            streams = ui.extract(rom, assets, g)
            self.assertEqual(ui.roundtrip(rom, assets, g), {entry["tiles"], entry["maps"][0][0]})
            first = ui.build(rom, assets, output, g)
            for address, item in streams.items():
                self.assertEqual((output / f"graphics-ui-{address:06X}.bin").read_bytes(), rom[address:address + item["size"]])
                assembly = (output / f"graphics-ui-{address:06X}.s").read_text()
                self.assertIn(output.as_posix(), assembly)
                self.assertNotIn('"build/assets/', assembly)
            self.assertEqual(ui.build(rom, assets, output, g), first)
            tilemap = assets / f"graphics/ui/{entry['maps'][0][0]:06X}.map.bin"
            tilemap.write_bytes(b"edited")
            ui.extract(rom, assets, g)
            self.assertEqual(tilemap.read_bytes(), b"edited")
            with self.assertRaisesRegex(ValueError, "preserve the 2x1"):
                ui.edited(rom, assets, g)
            ui.extract(rom, assets, g, force=True)
            self.assertEqual(ui.build(rom, assets, output, g), first)

    def test_pixels_and_single_owner_palette(self):
        with fixture() as (rom, assets, output, entry):
            ui.extract(rom, assets, g)
            address = entry["tiles"]
            png = assets / f"graphics/ui/{address:06X}.tiles.png"
            rows, colors = indexed_png.read(png.read_bytes())
            rows[0][8] = 2
            png.write_bytes(indexed_png.write(rows, colors))
            self.assertEqual(ui.edited(rom, assets, g)[1][address], bytes(32) + b"\x02" + bytes(31))
            with self.assertRaisesRegex(ValueError, "graphics are edited"):
                ui.roundtrip(rom, assets, g)
            colors[1] = (255, 0, 0)
            png.write_bytes(indexed_png.write(rows, colors))
            with self.assertRaisesRegex(ValueError, "tile palette is read-only"):
                ui.build(rom, assets, output, g)
            ui.extract(rom, assets, g, force=True)
            pal = assets / f"graphics/ui/{entry['palette']:06X}.palette.png"
            rows, colors = indexed_png.read(pal.read_bytes())
            colors[1] = (0, 255, 0)
            pal.write_bytes(indexed_png.write(rows, colors))
            self.assertEqual(ui.edited(rom, assets, g)[1][entry["palette"]][2:4], b"\xe0\x03")
            rows[0][0] = 1
            pal.write_bytes(indexed_png.write(rows, colors))
            with self.assertRaisesRegex(ValueError, "swatch indices"):
                ui.edited(rom, assets, g)

    def test_empty_maps_unloaded_tiles_and_palette_banks(self):
        entry = ui.scene("test", 0, (0, 32), (), 0, 0, 1, 1)
        with self.assertRaisesRegex(ValueError, "tilemap"):
            ui.map_values(b"", entry, bytes(32))
        for size in (32, 33, 63):
            with self.assertRaisesRegex(ValueError, "partially loaded or unavailable"):
                ui.map_values(b"\x01\0", entry, bytes(size))
        with self.assertRaisesRegex(ValueError, "not owned"):
            ui.map_values(b"\x00\x10", entry, b"\x01" + bytes(31))
        ui.map_values(b"\xff\x03", entry, bytes(32), b"\xff\x03")
        with self.assertRaisesRegex(ValueError, "unavailable"):
            ui.map_values(b"\xff\x03", entry, bytes(32), bytes(2))

    def test_patch_external_cells_preserved_and_tile_base(self):
        entry = ui.scene("patch", 0, (0, 32), (), 0, 0, 1, 1, base=7, tile_base=512, external=True)
        ui.map_values(b"\x00\x72", entry, b"\x01" + bytes(31))
        ui.map_values(b"\x03\x20", entry, bytes(32), b"\x03\x20")
        with self.assertRaisesRegex(ValueError, "unavailable"):
            ui.map_values(b"\x03\x20", entry, bytes(32), b"\x04\x20")
        with self.assertRaisesRegex(ValueError, "not owned"):
            ui.map_values(b"\x00\x22", entry, b"\x01" + bytes(31))

    def test_palette_copy_tail_forces_relocation_even_when_encoded_data_fits(self):
        with fixture() as (rom, assets, output, entry):
            streams = ui.extract(rom, assets, g)
            address = entry["tiles"]
            png = assets / f"graphics/ui/{address:06X}.tiles.png"
            rows, colors = indexed_png.read(png.read_bytes())
            rows[0][8] = 0
            png.write_bytes(indexed_png.write(rows, colors))
            self.assertLessEqual(len(g.codec.encode(bytes(64))), streams[address]["size"])
            with patch.object(ui, "PALETTE_TAILS", ((address, address + 4),)):
                ui.build(rom, assets, output, g)
                self.assertEqual((output / f"graphics-ui-{address:06X}.bin").read_bytes(), rom[address:address + streams[address]["size"]])
                self.assertEqual(g.codec.decode((output / f"graphics-ui-{address:06X}-overflow.bin").read_bytes())[0], bytes(64))
                with patch.object(g, "growth_safe", return_value=False), self.assertRaisesRegex(ValueError, "original slot must remain unchanged"):
                    ui.build(rom, assets, output, g)

    def test_shared_logo_map_rejects_partial_tiles_and_accepts_cleared_blank(self):
        ui.validate_shared_map(bytes(4096), bytes(32))
        ui.validate_shared_map(b"\xff\x03" * 2048, bytes(32))
        for raw in (b"", b"\x01\0" * 2048, b"\0\x10" * 2048):
            with self.assertRaises(ValueError):
                ui.validate_shared_map(raw, bytes(33))

    def test_raw_maps_and_symbolic_patch_tables(self):
        with fixture(compressed=False) as (rom, assets, output, entry):
            struct.pack_into("<I", rom, 64 + 24, g.fa.BASE + entry["tiles"])
            struct.pack_into("<I", rom, 64 + 16, g.fa.BASE + entry["tiles"])
            with patch.object(ui, "PATCHES", ((64,),)), patch.object(ui, "BLOCKS", ((512, len(rom) - 512),)):
                ui.build(rom, assets, output, g)
            lines = (output / "graphics-ui-patch-000040.s").read_text().splitlines()
            self.assertEqual(lines[6], ".4byte " + ui.symbol(entry["tiles"]))
            self.assertEqual(lines[4], f".4byte {g.fa.BASE + entry['tiles']}")
            self.assertIn(".include", (output / "graphics-ui-block-000200.s").read_text())


if __name__ == "__main__":
    unittest.main()
