"""Runtime-loaded illustration palettes and guarded non-pointer ranges."""

from contextlib import ExitStack, contextmanager
import hashlib
import json
from pathlib import Path
import struct
import tempfile
import unittest
from unittest.mock import patch

import graphics_assets as g
import graphics_reference_audit as audit
import indexed_png
import rest_graphics as rest


@contextmanager
def fixture():
    rom = bytearray(512)
    scene = {"tiles": 128, "maps": [256], "width": 32, "height": 20, "reserved_bank": False}
    data = {128: bytes(32), 192: bytes(64), 256: bytes(1280)}
    streams = {a: {"raw": value, "size": len(value), "references": []} for a, value in data.items()}
    with ExitStack() as stack, tempfile.TemporaryDirectory() as directory:
        stack.enter_context(patch.object(rest, "catalogue", return_value=(scene, streams)))
        stack.enter_context(patch.object(rest, "TILES", 128))
        stack.enter_context(patch.object(rest, "PALETTE", 192))
        yield rom, Path(directory), streams


class RemainingGraphicsTests(unittest.TestCase):
    def test_roundtrip_pixel_and_palette_edits(self):
        with fixture() as (rom, assets, streams):
            self.assertEqual(rest.roundtrip(rom, assets, g), {128, 256})
            image = assets / "graphics/rest/childhood.tiles.png"
            rows, colors = indexed_png.read(image.read_bytes())
            self.assertEqual(colors[0], (255, 255, 255))
            rows[0][0] = 1
            image.write_bytes(indexed_png.write(rows, colors))
            self.assertEqual(rest.edited(rom, assets, g)[2][128], b"\x01" + bytes(31))
            with self.assertRaisesRegex(ValueError, "graphics are edited"):
                rest.roundtrip(rom, assets, g)
            palette = assets / "graphics/rest/childhood.palette.png"
            rows, colors = indexed_png.read(palette.read_bytes())
            colors[1] = (255, 0, 0)
            palette.write_bytes(indexed_png.write(rows, colors))
            self.assertEqual(rest.edited(rom, assets, g)[2][192][2:4], b"\x1f\x00")

    def test_overwritten_palette_color_and_unloaded_banks(self):
        with fixture() as (rom, assets, streams):
            rest.extract(rom, assets, g)
            palette = assets / "graphics/rest/childhood.palette.png"
            saved = palette.read_bytes()
            rows, colors = indexed_png.read(saved)
            colors[0] = (255, 0, 0)
            palette.write_bytes(indexed_png.write(rows, colors))
            with self.assertRaisesRegex(ValueError, "overwritten white"):
                rest.edited(rom, assets, g)
            palette.write_bytes(saved)
            tilemap = assets / "graphics/rest/childhood-00.map.bin"
            tilemap.write_bytes(struct.pack("<H", 0x2000) * 640)
            with self.assertRaisesRegex(ValueError, "only the two palettes"):
                rest.edited(rom, assets, g)
            tilemap.write_bytes(b"")
            with self.assertRaisesRegex(ValueError, "row-major"):
                rest.edited(rom, assets, g)

    def test_guarded_instruction_ranges_reject_changed_bytes(self):
        rom = bytearray(0x800000)
        record = {"start": 4, "end": 10, "sha256": hashlib.sha256(bytes(6)).hexdigest()}
        with patch.object(Path, "read_text", return_value=json.dumps({"regions": [record]})), patch.object(audit.sprites, "sections"):
            self.assertEqual(audit.regions(bytes(rom))[0], (4, bytes(6)))
            rom[5] = 1
            with self.assertRaisesRegex(ValueError, "audited Thumb instructions changed"):
                audit.regions(bytes(rom))
        self.assertEqual(audit.regions(b""), ())


if __name__ == "__main__":
    unittest.main()
