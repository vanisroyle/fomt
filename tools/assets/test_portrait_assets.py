"""Portrait frame views, shared source ownership and fixed-slot rebuilding."""

from contextlib import ExitStack, contextmanager
from pathlib import Path
import struct
import tempfile
import unittest
from unittest.mock import patch

import graphics_assets as g
import indexed_png
import portrait_assets as portraits


@contextmanager
def fixture():
    record = struct.pack("<8H", 1, 0, 1, 0, 1, 0, 0, 0)
    sections = ((1, bytes(4)), (2, record * 2), (1, bytes(8)),
                (1, bytes(32)), (1, bytes(32)), (0, b""), (1, bytes(4)))
    raw = b"".join(struct.pack("<I", count) + data for count, data in sections)
    rom = raw + b"unchanged suffix"
    with ExitStack() as stack, tempfile.TemporaryDirectory() as directory:
        for name, value in (("START", 0), ("END", len(raw)), ("SLOT_END", len(rom))):
            stack.enter_context(patch.object(portraits, name, value))
        yield rom, Path(directory) / "assets", Path(directory) / "custom output", raw


class PortraitTests(unittest.TestCase):
    def test_roundtrip_build_and_preserve_edits(self):
        with fixture() as (rom, assets, output, raw):
            portraits.roundtrip(rom, assets, g)
            portraits.build(rom, assets, output, g)
            self.assertEqual((output / "graphics-portraits.bin").read_bytes(), rom)
            path = assets / "graphics/portraits/000.png"
            rows, palette = indexed_png.read(path.read_bytes())
            rows[0][0] = 1
            path.write_bytes(indexed_png.write(rows, palette))
            portraits.extract(rom, assets, g)
            rebuilt = portraits.edited(rom, assets, g)
            start = next(portraits.frames(rom))[1][0]
            self.assertEqual(rebuilt, raw[:start] + b"\x01" + raw[start + 1:])
            with self.assertRaisesRegex(ValueError, "portraits are edited"):
                portraits.roundtrip(rom, assets, g)
            portraits.extract(rom, assets, g, force=True)
            self.assertEqual(portraits.edited(rom, assets, g), raw)

    def test_shared_pixels_and_palettes_must_agree(self):
        for palette_edit in (False, True):
            with self.subTest(palette=palette_edit), fixture() as (rom, assets, output, raw):
                portraits.extract(rom, assets, g)
                for index in (0, 1):
                    path = assets / f"graphics/portraits/{index:03d}.png"
                    rows, colors = indexed_png.read(path.read_bytes())
                    if palette_edit:
                        colors[1] = (255, 0, 0) if index == 0 else (0, 0, 255)
                    else:
                        rows[0][0] = index + 1
                    path.write_bytes(indexed_png.write(rows, colors))
                with self.assertRaisesRegex(ValueError, "conflicting edits to shared portrait"):
                    portraits.build(rom, assets, output, g)

    def test_padding_empty_and_malformed(self):
        with fixture() as (rom, assets, output, raw):
            portraits.extract(rom, assets, g)
            path = assets / "graphics/portraits/000.png"
            rows, colors = indexed_png.read(path.read_bytes())
            rows[0][8] = 1
            path.write_bytes(indexed_png.write(rows, colors))
            with self.assertRaisesRegex(ValueError, "unused sheet"):
                portraits.build(rom, assets, output, g)
        with patch.object(portraits, "START", 0), patch.object(portraits, "END", 28):
            self.assertEqual(list(portraits.frames(bytes(28))), [])
            with self.assertRaisesRegex(ValueError, "truncated"):
                list(portraits.frames(b""))


if __name__ == "__main__":
    unittest.main()
