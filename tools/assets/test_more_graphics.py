"""Synthetic shared world graphics, palette ownership and slot regressions."""

from contextlib import ExitStack, contextmanager
from pathlib import Path
import struct
import random
import tempfile
import unittest
from unittest.mock import patch

import graphics_assets as g
import indexed_png
import more_graphics as m


@contextmanager
def fixture():
    rom = bytearray(128)
    addresses = []
    for raw in (bytes(32) + b"\x01" + bytes(31), struct.pack("<H", 0x801F) * 240,
                struct.pack("<2H", 0x1401, 0x03FF)):
        addresses.append(len(rom))
        rom += g.codec.encode(raw)
    image, palette, tilemap = addresses
    struct.pack_into("<10I", rom, 0, *(a + g.fa.BASE for a in (image, palette, palette, tilemap, tilemap, tilemap)),
                     0, 0, 2 | 1 << 16, 1)
    with ExitStack() as stack, tempfile.TemporaryDirectory() as directory:
        for name, value in (("TABLE", 0), ("COUNT", 1), ("EXCLUDED", ()), ("FEEDING", ()),
                            ("INSTRUCTION_BYTES", ()), ("NON_POINTER_STREAMS", ()),
                            ("BLOCKS", ((128, len(rom) - 128),))):
            stack.enter_context(patch.object(m, name, value))
        assets, output = Path(directory) / "assets", Path(directory) / "custom output"
        yield rom, addresses, assets, output


class MoreGraphicsTests(unittest.TestCase):
    def test_palette_read_past_preserves_slot_even_when_new_data_fits(self):
        with fixture() as (rom, addresses, _, output):
            address = addresses[0]
            stream = m.catalogue(rom)[1][address]
            with patch.object(m, "PALETTE_READ_PAST", ((address, address + 4),)):
                m.emit_part(rom, address, stream, bytes(64), output, g)
                self.assertEqual((output / f"graphics-more-{address:06X}-slot.bin").read_bytes(), rom[address:address + stream["size"]])
                self.assertEqual(g.codec.decode((output / f"graphics-more-{address:06X}-overflow.bin").read_bytes())[0], bytes(64))
                with patch.object(g, "growth_safe", return_value=False), self.assertRaisesRegex(ValueError, "original slot must remain unchanged"):
                    m.emit_part(rom, address, stream, bytes(64), output, g)

    def test_unknown_bank_zero_cells_are_frozen_not_assigned_a_palette(self):
        with fixture() as (rom, addresses, assets, _):
            scenes, streams = m.catalogue(rom)
            scenes[0].update(id=11, unowned_tiles=[1])
            stream = streams[addresses[2]]
            stream["raw"] = struct.pack("<2H", 1, 1023)
            with patch.object(m, "catalogue", return_value=(scenes, streams)):
                m.extract(rom, assets, g)
                m.edited(rom, assets, g)
                png = assets / f"graphics/more/{addresses[0]:06X}.tiles.png"
                saved = png.read_bytes()
                rows, colors = indexed_png.read(saved)
                rows[0][8] = 18
                png.write_bytes(indexed_png.write(rows, colors))
                with self.assertRaisesRegex(ValueError, "unowned palette bank zero"):
                    m.edited(rom, assets, g)
                png.write_bytes(saved)
                tilemap = assets / f"graphics/more/{addresses[2]:06X}.map.bin"
                tilemap.write_bytes(struct.pack("<2H", 1, 1))
                with self.assertRaisesRegex(ValueError, "cannot be introduced"):
                    m.edited(rom, assets, g)
            owner = dict(scenes[0], id=13)
            self.assertIs(m.owners([scenes[0], owner])[addresses[0]], owner)

    def test_palette_overrides_are_context_specific(self):
        streams = {1: {"raw": bytes(480)}}
        ordinary = {"name": "ordinary", "reserved_bank": True}
        overridden = dict(ordinary, palette_overrides=[(0x100, "e003")])
        self.assertEqual(m.palette_for(ordinary, 1, streams), bytes(512))
        self.assertEqual(m.palette_for(overridden, 1, streams)[0x120:0x122], b"\xe0\x03")
        self.assertEqual(streams[1]["raw"], bytes(480))

    def test_additional_palette_variants_share_tiles_and_keep_code_roots(self):
        with fixture() as (rom, addresses, _, _):
            address = len(rom)
            rom += g.codec.encode(bytes(480))
            struct.pack_into("<I", rom, 28, g.fa.BASE + address)
            with patch.object(m, "PALETTE_VARIANTS", ((0, address, 28),)):
                scenes, streams = m.catalogue(rom)
            self.assertEqual(len(scenes), 2)
            self.assertEqual(scenes[1]["tiles"], addresses[0])
            self.assertEqual(scenes[1]["palettes"], [address])
            self.assertEqual(streams[address]["references"], [28])

    def test_shared_catalogue_and_byte_identical_rebuild(self):
        with fixture() as (rom, addresses, assets, output):
            scenes, streams = m.extract(rom, assets, g)
            self.assertEqual(len(scenes), 1)
            self.assertEqual(len(streams), 3)
            self.assertEqual(streams[addresses[1]]["references"], [4, 8])
            self.assertEqual(streams[addresses[2]]["references"], [12, 16, 20])
            first = m.build(rom, assets, output, g)
            for address, stream in streams.items():
                self.assertEqual((output / f"graphics-more-{address:06X}-slot.bin").read_bytes(),
                                 rom[address:address + stream["size"]])
            self.assertEqual(m.build(rom, assets, output, g), first)
            self.assertEqual(m.roundtrip(rom, assets, g), set(addresses))
            assembly = (output / "graphics-more-000080.s").read_text()
            self.assertIn(output.as_posix(), assembly)
            self.assertNotIn('"build/assets/', assembly)
            self.assertEqual((output / "graphics-more-table.s").read_text().count(m.symbol(addresses[2])), 3)
            tilemap = assets / f"graphics/more/{addresses[2]:06X}.map.bin"
            tilemap.write_bytes(b"user edit")
            m.extract(rom, assets, g)
            self.assertEqual(tilemap.read_bytes(), b"user edit")
            with self.assertRaisesRegex(ValueError, "row-major"):
                m.build(rom, assets, output, g)
            m.extract(rom, assets, g, force=True)
            self.assertNotEqual(tilemap.read_bytes(), b"user edit")

    def test_indexed_pixel_edit_and_palette_single_owner(self):
        with fixture() as (rom, addresses, assets, output):
            scenes, streams = m.extract(rom, assets, g)
            image, palette, _ = addresses
            path = assets / f"graphics/more/{image:06X}.tiles.png"
            rows, colors = indexed_png.read(path.read_bytes())
            rows[0][8] = 18
            path.write_bytes(indexed_png.write(rows, colors))
            _, _, edited = m.edited(rom, assets, g)
            self.assertEqual(edited[image], bytes(32) + b"\x02" + bytes(31))
            with self.assertRaisesRegex(ValueError, "is edited"):
                m.roundtrip(rom, assets, g)
            colors[16] = (0, 255, 0)
            path.write_bytes(indexed_png.write(rows, colors))
            with self.assertRaisesRegex(ValueError, "palette is read-only; edit"):
                m.build(rom, assets, output, g)
            m.extract(rom, assets, g, force=True)
            swatch = assets / f"graphics/more/{palette:06X}.palette.png"
            rows, colors = indexed_png.read(swatch.read_bytes())
            colors[0] = (0, 255, 0)
            swatch.write_bytes(indexed_png.write(rows, colors))
            _, _, edited = m.edited(rom, assets, g)
            self.assertEqual(edited[palette][:2], struct.pack("<H", 0x83E0))
            self.assertEqual(edited[image], streams[image]["raw"])
            rows[0][0] = 1
            swatch.write_bytes(indexed_png.write(rows, colors))
            with self.assertRaisesRegex(ValueError, "swatch pixels are read-only"):
                m.build(rom, assets, output, g)

    def test_blank_tiles_reserved_bank_and_map_dimensions(self):
        tiles = b"\x01" + bytes(31)
        for invalid in (b"", bytes(2), bytes(6)):
            with self.assertRaisesRegex(ValueError, "row-major"):
                m.map_values(invalid, 2, 1, tiles, False)
        for raw, message in ((struct.pack("<H", 1), "unavailable tile"),
                             (bytes(2), "bank zero is reserved")):
            with self.assertRaisesRegex(ValueError, message):
                m.map_values(raw, 1, 1, tiles, True)
        m.map_values(struct.pack("<H", 1023), 1, 1, tiles, True)
        with self.assertRaisesRegex(ValueError, "complete 4bpp"):
            m.map_values(bytes(2), 1, 1, b"", False)
        with self.assertRaisesRegex(ValueError, "remain blank"):
            m.map_values(bytes(2), 1, 1, bytes(32767) + b"\x01", False)

    def test_shared_runtime_overwritten_palette_range_is_guarded(self):
        with fixture() as (rom, addresses, assets, output):
            scenes, streams = m.extract(rom, assets, g)
            palette = addresses[1]
            path = assets / f"graphics/more/{palette:06X}.palette.png"
            rows, colors = indexed_png.read(path.read_bytes())
            colors[128] = (0, 255, 0)
            path.write_bytes(indexed_png.write(rows, colors))
            rom.extend(bytes(63 * 40 - len(rom)))
            struct.pack_into("<I", rom, 62 * 40 + 4, g.fa.BASE + palette)
            with patch.object(m, "COUNT", 63), patch.object(m, "extract", return_value=(scenes, streams)):
                with self.assertRaisesRegex(ValueError, "overwrites these shared palette colors"):
                    m.edited(rom, assets, g)

    def test_sheet_banks_padding_flips_and_blank_preview(self):
        tiles = b"\x01" + bytes(31)
        rows = m.banked_sheet(tiles, [3], g)
        self.assertEqual(rows[0][0], 49)
        self.assertEqual(m.unbank_sheet(rows, tiles, [3], g), tiles)
        rows[0][0] = 1
        with self.assertRaisesRegex(ValueError, "retain palette bank"):
            m.unbank_sheet(rows, tiles, [3], g)
        rows[0][0], rows[0][8] = 49, 1
        with self.assertRaisesRegex(ValueError, "padding"):
            m.unbank_sheet(rows, tiles, [3], g)
        with self.assertRaisesRegex(ValueError, "dimensions"):
            m.unbank_sheet([], tiles, [3], g)
        palette = [(0, 0, 0)] * 256
        raw = struct.pack("<2H", 0x3C00, 1023)
        image, _ = indexed_png.read(m.preview(tiles, raw, palette, 2, 1, True, g))
        self.assertEqual(image[7][7], 49)
        self.assertEqual(sum(bool(pixel) for row in image for pixel in row), 1)

    def test_overflow_audits_roots_and_retains_custom_output(self):
        with fixture() as (rom, addresses, assets, output):
            _, streams = m.catalogue(rom)
            address = addresses[0]
            raw = random.Random(7).randbytes(64)
            lines, layout = m.emit_part(rom, address, streams[address], raw, output, g)
            self.assertTrue(layout["overflow"])
            self.assertIn(".pushsection", "\n".join(lines))
            self.assertIn(output.as_posix(), "\n".join(lines))
            self.assertEqual((output / f"graphics-more-{address:06X}-slot.bin").read_bytes(),
                             rom[address:address + streams[address]["size"]])
            rom += struct.pack("<I", g.fa.BASE + address + 1)
            with self.assertRaisesRegex(ValueError, "non-symbolic/interior"):
                m.emit_part(rom, address, streams[address], raw, output, g)

    def test_empty_catalogue(self):
        with patch.object(m, "COUNT", 0), patch.object(m, "FEEDING", ()):
            self.assertEqual(m.catalogue(b""), ([], {}))


if __name__ == "__main__":
    unittest.main()
