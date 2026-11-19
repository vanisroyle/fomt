"""Synthetic title-resource, banked background and rebuild regressions."""

from contextlib import ExitStack
from pathlib import Path
import struct
import tempfile
import unittest
from unittest.mock import patch

import graphics_assets as g
import indexed_png
import title_graphics as title


def resource():
    records = bytearray(160)
    objects = b"".join(struct.pack("<II", attr, 0) for attr in (0x2000, 0, 0, 0))
    for i, obj, count, tile, palettes, palette in ((0, 0, 2, 0, 16, 0), (1, 1, 1, 2, 1, 16),
                                                 (5, 2, 1, 3, 1, 17), (9, 3, 1, 4, 1, 18)):
        struct.pack_into("<8H", records, i * 16, 1, obj, count, tile, palettes, palette, 0, 0)
    sections = ((0, b""), (10, records), (4, objects), (5, bytes(160)), (19, bytes(608)), (0, b""), (0, b""))
    return b"".join(struct.pack("<I", count) + data for count, data in sections)


class TitleGraphicsTests(unittest.TestCase):
    def test_growth_excludes_only_decoded_audited_payload_ranges(self):
        encoded = g.codec.encode(b"\x74\x90\x08" * 100)
        rom = bytearray(0xC000)
        rom[0x100:0x100 + len(encoded)] = encoded
        target = struct.unpack_from("<I", encoded, 7)[0] - 0x08000000 - 4
        part = (target, 8, (0x10,))
        struct.pack_into("<I", rom, 0x10, 0x08000000 + target)
        self.assertFalse(g.growth_safe(rom, part))
        audited = ((0x100, len(encoded) + 8),)
        self.assertTrue(g.growth_safe(rom, part, audited))
        struct.pack_into("<I", rom, 0x100 + len(encoded), 0x08000000 + target + 4)
        self.assertFalse(g.growth_safe(rom, part, audited))
        rom[0x100 + len(encoded):0x104 + len(encoded)] = bytes(4)
        struct.pack_into("<I", rom, 0x80, 0x08000000 + target + 4)
        self.assertFalse(g.growth_safe(rom, part, audited))
        rom[0x80:0x84] = bytes(4)
        rom[0x100] = 0
        with self.assertRaises(ValueError):
            g.growth_safe(rom, part, audited)

    def test_sprite_sections_depth_and_transparent_preview(self):
        raw = resource()
        for index in title.ITEMS.values():
            offset, size, pal, colors, bpp, objects = title.item(raw, index)
            self.assertEqual(bpp, 8 if index == 0 else 4)
            image = title.sprite_preview(raw[offset:offset + size], g.palette_rgb(raw[pal:pal + colors]), bpp, objects)
            rows, palette = indexed_png.read(image)
            self.assertEqual(rows, [bytearray(8) for _ in range(8)])
            self.assertEqual(len(palette), 256 if index == 0 else 16)
        for invalid in (b"", raw[:-1], raw + b"\0"):
            with self.subTest(length=len(invalid)), self.assertRaises(ValueError):
                title.sections(invalid)
        with self.assertRaises(ValueError):
            title.item(raw, 10)
        with self.assertRaisesRegex(ValueError, "unavailable pixels"):
            title.sprite_preview(b"", [(0, 0, 0)] * 16, 4, [(0, 0)])

    def test_sprite_flips_and_8bpp_pixel_indices(self):
        palette = [(i, i, i) for i in range(256)]
        tiles = bytes([200]) + bytes(63)
        for flag, x, y in ((0, 0, 0), (0x10000000, 7, 0), (0x20000000, 0, 7)):
            rows, _ = indexed_png.read(title.sprite_preview(tiles, palette, 8, [(0x2000 | flag, 0)]))
            self.assertEqual(rows[y][x], 200)
            self.assertEqual(sum(value != 0 for row in rows for value in row), 1)

    def test_banked_background_roundtrip_flips_and_errors(self):
        tiles = bytes([1]) + bytes(32767)
        raw = struct.pack("<H", 0x3C00) + bytes(4094)
        hints = title.bank_hints([raw], tiles, 256)
        rows = title.background_sheet(tiles, hints, g)
        self.assertEqual(rows[0][0], 49)
        self.assertEqual(title.background_tiles(rows, tiles, hints, g), tiles)
        preview, _ = indexed_png.read(title.background_preview(tiles, [(0, 0, 0)] * 256, raw, g))
        self.assertEqual(preview[7][7], 49)
        rows[0][0] = 1
        with self.assertRaisesRegex(ValueError, "retain palette bank"):
            title.background_tiles(rows, tiles, hints, g)
        for invalid in (b"", bytes(4094), bytes(4098)):
            with self.assertRaisesRegex(ValueError, "64x32"):
                title.map_entries(invalid, tiles, 256)
        with self.assertRaisesRegex(ValueError, "unavailable tile"):
            title.map_entries(bytes(4096), b"", 256)
        with self.assertRaisesRegex(ValueError, "palette bank"):
            title.map_entries(raw, tiles, 16)

    def test_extraction_preservation_rebuild_and_custom_output(self):
        raw = resource()
        originals = [raw, bytes(32768)] + [bytes(4096)] * 4
        rom = bytearray(0x1000)
        parts = []
        for i, data in enumerate(originals):
            encoded = g.codec.encode(data)
            offset = len(rom)
            parts.append((offset, len(encoded), (i * 4,)))
            struct.pack_into("<I", rom, i * 4, 0x08000000 + offset)
            rom += encoded
        palette = (len(rom), 512, (24,))
        struct.pack_into("<I", rom, 24, 0x08000000 + palette[0])
        rom += bytes(512)
        with ExitStack() as stack, tempfile.TemporaryDirectory() as directory:
            for key, value in (("RESOURCE", parts[0]), ("BACKGROUND", parts[1]), ("MAPS", tuple(parts[2:])),
                               ("PALETTE", palette), ("RESOURCE_NON_POINTER_STREAMS", ())):
                stack.enter_context(patch.object(title, key, value))
            assets, output = Path(directory) / "assets", Path(directory) / "custom output"
            title.extract(rom, assets, g)
            title.build(rom, assets, output, g)
            for name, part in zip(("title-resource", "title-background", "title-map-0", "title-map-1", "title-map-2", "title-map-3"), parts):
                self.assertEqual((output / f"graphics-{name}-slot.bin").read_bytes(), rom[part[0]:part[0] + part[1]])
                self.assertIn(str(output), (output / f"graphics-{name}.s").read_text())
            path = assets / "graphics/title-logo.png"
            saved = path.read_bytes()
            rows, colors = indexed_png.read(saved)
            rows[0][0] = 200
            edited = indexed_png.write(rows, colors)
            path.write_bytes(edited)
            title.extract(rom, assets, g)
            self.assertEqual(path.read_bytes(), edited)
            title.build(rom, assets, output, g)
            target = output / "graphics-title-resource-overflow.bin"
            packed = target.read_bytes() or (output / "graphics-title-resource-slot.bin").read_bytes()
            rebuilt = g.codec.decode(packed)[0]
            offset = title.item(raw, 0)[0]
            self.assertEqual(rebuilt, raw[:offset] + bytes([200]) + raw[offset + 1:])
            title.extract(rom, assets, g, True)
            self.assertEqual(path.read_bytes(), saved)
            path.write_bytes(indexed_png.write([bytearray(8)], colors))
            with self.assertRaisesRegex(ValueError, "dimensions"):
                title.build(rom, assets, output, g)


if __name__ == "__main__":
    unittest.main()
