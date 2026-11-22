"""Graphics tests use synthetic pixels, palettes, pointers and compressed streams."""

from pathlib import Path
from functools import lru_cache
from itertools import product
import random
import struct
import tempfile
import unittest
from unittest.mock import patch
import zlib

import graphics_assets as graphics
import indexed_png
from font_assets import SIGNATURE, png_chunk
import unpack_codec as codec


class GraphicsTests(unittest.TestCase):
    def setUp(self):
        extract = patch.object(graphics.title, "extract")
        build = patch.object(graphics.title, "build", return_value=b"")
        extract.start()
        build.start()
        self.addCleanup(extract.stop)
        self.addCleanup(build.stop)
        for name, value in (("extract", None), ("build", b"")):
            for module in (graphics.more, graphics.portraits, graphics.rest):
                mocked = patch.object(module, name, return_value=value)
                mocked.start()
                self.addCleanup(mocked.stop)

    def test_encoder_empty_literals_runs_and_overlapping_matches(self):
        rng = random.Random(29)
        cases = [b"", b"a", b"ab", b"a" * 4096, b"ab" * 500, bytes(range(256)) * 30,
                 bytes(rng.randrange(256) for _ in range(8192))]
        for raw in cases:
            with self.subTest(size=len(raw)):
                packed = codec.encode(raw)
                self.assertEqual(codec.decode(packed)[0], raw)
                self.assertEqual(bytes(codec._decoder.unpack(packed)[0]), raw)
                self.assertEqual(packed, codec.encode(raw))
                self.assertEqual(len(packed) % 4, 0)
        with self.assertRaisesRegex(ValueError, "256 KiB"):
            codec.encode(bytes(0x40001))

    def test_decoder_rejects_truncated_headers_formats_and_references(self):
        packed = codec.encode(bytes(range(100)))
        for invalid in (b"", b"\0" * 8, packed[:-1], packed[:4], b"\x70\x01\0\0\0\0\0\xff"):
            with self.subTest(invalid=invalid[:8]), self.assertRaises(ValueError):
                codec.decode(invalid)
        bits = codec.WriteBits()
        for value, count in ((0, 8), (0, 4), (0, 4), (0, 2), (0, 1), (0, 6)):
            bits.put(value, count)
        with self.assertRaisesRegex(ValueError, "reference"):
            codec.decode(struct.pack("<I", 3 << 8 | 0x70) + bits.finish())

    def test_optimal_parser_matches_exhaustive_bit_cost(self):
        def exhaustive(data):
            @lru_cache(None)
            def solve(pos):
                if pos == len(data):
                    return 0
                costs = [8 + length * 8 + solve(pos + length)
                         for length in range(1, min(64, len(data) - pos) + 1)]
                for length in range(2, min(65, len(data) - pos) + 1):
                    if data[pos:pos + length] == data[pos:pos + 1] * length:
                        costs.append(16 + solve(pos + length))
                for distance in range(1, min(pos, 4352) + 1):
                    for length in range(3, min(66, len(data) - pos) + 1):
                        if data[pos:pos + length] == data[pos - distance:pos - distance + length]:
                            costs.append((16 if distance <= 256 else 20) + solve(pos + length))
                return min(costs)
            return solve(0)
        for size in range(9):
            for values in product((0, 1), repeat=size):
                raw = bytes(values)
                tokens, cost = codec.optimal_tokens(raw)
                self.assertEqual(cost, exhaustive(raw), raw)
                self.assertEqual(sum(count for _, count, _ in tokens), len(raw))
        raw = b"aabbcc"
        self.assertLess(len(codec.encode_optimal(raw)), len(codec.encode_greedy(raw)))

    def test_difference_candidates_and_greedy_size_bound(self):
        rng = random.Random(71)
        cases = (b"", b"a", b"aabbcc", bytes(range(256)) * 4,
                 bytes(rng.randrange(256) for _ in range(1024)), bytes(5000),
                 b"ab" * 200 + bytes(range(256)) + b"ab" * 200)
        for raw in cases:
            for method in range(5 if len(raw) % 2 == 0 else 3):
                encoded = codec.encode_optimal(codec.difference(raw, method), method)
                self.assertEqual(bytes(codec._decoder.unpack(encoded)[0]), raw)
            self.assertLessEqual(len(codec.encode(raw)), len(codec.encode_greedy(raw)))
        for method in (3, 4):
            with self.assertRaisesRegex(ValueError, "even-sized"):
                codec.difference(b"a", method)

    def test_png_preserves_duplicate_colors_and_indices(self):
        for count in (16, 256):
            palette = [(0, 0, 0)] * count
            rows = [bytearray((x + y) % count for x in range(19)) for y in range(9)]
            self.assertEqual(indexed_png.read(indexed_png.write(rows, palette)), (rows, palette))
        for rows, palette in (([], [(0, 0, 0)]), ([bytearray([16])], [(0, 0, 0)] * 16)):
            with self.assertRaises(ValueError):
                indexed_png.write(rows, palette)

    def test_png_depths_and_filters(self):
        for depth in (1, 2, 4, 8):
            palette = [(i, i, i) for i in range(1 << depth)]
            scan = bytes([0x55, 0xAA])
            width = 16 // depth
            expected = bytearray((scan[x * depth // 8] >> (8 - depth - x * depth % 8)) & ((1 << depth) - 1) for x in range(width))
            for method in range(5):
                raw, previous = bytearray(), bytes(2)
                for _ in range(2):
                    raw.append(method)
                    for i, value in enumerate(scan):
                        a, b, c = scan[i - 1] if i else 0, previous[i], previous[i - 1] if i else 0
                        raw.append((value - (0, a, b, (a + b) // 2, indexed_png.paeth(a, b, c))[method]) & 255)
                    previous = scan
                png = (SIGNATURE + png_chunk(b"IHDR", struct.pack(">IIBBBBB", width, 2, depth, 3, 0, 0, 0))
                       + png_chunk(b"PLTE", bytes(v for rgb in palette for v in rgb))
                       + png_chunk(b"IDAT", zlib.compress(raw)) + png_chunk(b"IEND", b""))
                self.assertEqual(indexed_png.read(png), ([expected, expected], palette))

    def test_png_rejects_corruption_transparency_and_wrong_size(self):
        png = indexed_png.write([bytearray([0, 1])], [(0, 0, 0), (255, 255, 255)])
        transparent = png[:-12] + png_chunk(b"tRNS", b"\0") + png[-12:]
        for invalid in (png[:-1], png + b"x", png[:40] + bytes([png[40] ^ 1]) + png[41:], transparent):
            with self.assertRaises(ValueError):
                indexed_png.read(invalid)
        with self.assertRaises(ValueError):
            indexed_png.read(png.replace(png[8:33], png_chunk(b"IHDR", struct.pack(">IIBBBBB", 99, 2, 4, 3, 0, 0, 0))))

    def test_palette_roundtrip_preserves_high_bits(self):
        raw = struct.pack("<16H", *((i * 2017) | (0x8000 if i % 2 else 0) for i in range(16)))
        self.assertEqual(graphics.palette_bytes(graphics.palette_rgb(raw), raw), raw)
        with self.assertRaises(ValueError):
            graphics.palette_bytes([(0, 0, 0)], raw)

    def test_tile_roundtrip_partial_padding_and_pixel_edit(self):
        for bpp in (4, 8):
            raw = bytes((i * 13) & 255 for i in range(93))
            rows = graphics.tile_sheet(raw, bpp)
            self.assertEqual(graphics.sheet_tiles(rows, len(raw), bpp), raw)
            rows[0][0] ^= 1
            edited = graphics.sheet_tiles(rows, len(raw), bpp)
            self.assertEqual(edited, bytes([raw[0] ^ 1]) + raw[1:])
            rows[-1][-1] = 1
            with self.assertRaisesRegex(ValueError, "padding"):
                graphics.sheet_tiles(rows, len(raw), bpp)
        with self.assertRaises(ValueError):
            graphics.tile_sheet(b"")
        with self.assertRaisesRegex(ValueError, "dimensions"):
            graphics.sheet_tiles([bytearray(8)], 32)

    def test_map_validation_and_flips(self):
        tiles = b"\x21" + bytes(31)
        for entry, x, y in ((0, 0, 0), (0x400, 7, 0), (0x800, 0, 7), (0xC00, 7, 7)):
            tilemap = struct.pack("<H", entry) + bytes(1278)
            rows = graphics.preview(tiles, tilemap)
            self.assertEqual(rows[y][x], 1)
        for tilemap in (b"", struct.pack("<H", 1) + bytes(1278), struct.pack("<H", 0x1000) + bytes(1278)):
            with self.assertRaises(ValueError):
                graphics.validate_map(tilemap, tiles)

    def test_overflow_audits_symbolic_and_interior_references(self):
        rom = bytearray(128)
        struct.pack_into("<I", rom, 0, 0x08000040)
        part = (64, 8, (0,))
        self.assertTrue(graphics.growth_safe(rom, part))
        slot, overflow = graphics.pack_part(rom, part, bytes(range(64)), b"original")
        self.assertEqual(slot, bytes(8))
        self.assertEqual(codec.decode(overflow)[0], bytes(range(64)))
        struct.pack_into("<I", rom, 13, 0x08000040)
        self.assertFalse(graphics.growth_safe(rom, part))
        with self.assertRaisesRegex(ValueError, "non-symbolic/interior"):
            graphics.pack_part(rom, part, bytes(range(64)), b"original")
        struct.pack_into("<I", rom, 13, 0x08000041)
        self.assertFalse(graphics.growth_safe(rom, part))
        self.assertEqual(graphics.pack_part(rom, part, b"same", b"same"), (bytes(8), None))

    def test_map_rejects_partial_and_unloaded_tiles(self):
        tiles = bytes(351 * 32 + 2)
        graphics.validate_map(struct.pack("<H", 350) + bytes(1278), tiles)
        for tile in (351, 352):
            with self.subTest(tile=tile), self.assertRaisesRegex(ValueError, "unavailable tile"):
                graphics.validate_map(struct.pack("<H", tile) + bytes(1278), tiles)
        with self.assertRaisesRegex(ValueError, "unavailable tile"):
            graphics.validate_map(bytes(1280), b"")

    def test_reference_audit_excludes_only_validated_payloads_and_exact_instructions(self):
        payload = codec.encode_greedy(b"\x74\x90\x08" * 100)
        rom = bytearray(0x200)
        rom[0x20:0x20 + len(payload)] = payload
        # Use a word actually present in encoded bits, not the decoded data.
        value = struct.unpack_from("<I", payload, 7)[0]
        part = (value - 0x08000000, 8, (0,))
        struct.pack_into("<I", rom, 0, value)
        self.assertFalse(graphics.growth_safe(rom, part))
        streams = ((0x20, len(payload)),)
        self.assertTrue(graphics.growth_safe(rom, part, streams))
        instruction = struct.pack("<I", value + 1)
        rom[0x80:0x84] = instruction
        self.assertFalse(graphics.growth_safe(rom, part, streams))
        expected = ((0x80, instruction),)
        self.assertTrue(graphics.growth_safe(rom, part, streams, expected))
        struct.pack_into("<I", rom, 0x90, value + 2)
        self.assertFalse(graphics.growth_safe(rom, part, streams, expected))
        rom[0x80] ^= 1
        with self.assertRaisesRegex(ValueError, "instruction bytes changed"):
            graphics.growth_safe(rom, part, streams, expected)
        rom[0x20] = 0
        with self.assertRaises(ValueError):
            graphics.growth_safe(rom, part, streams)

    def test_extraction_preserves_edits_and_build_is_deterministic(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            rom = bytearray(0x1000)
            parts = {"tiles": (64, 128, ()), "palette": (192, 32, ()), "map": (224, 128, ())}
            for name, raw in (("tiles", bytes(32)), ("map", bytes(1280))):
                offset = parts[name][0]
                packed = codec.encode(raw)
                rom[offset:offset + len(packed)] = packed
            with patch.object(graphics, "PARTS", parts):
                graphics.extract(rom, root)
                graphics.build(rom, root, root / "output")
                before = {p: p.read_bytes() for p in (root / "output").iterdir()}
                graphics.build(rom, root, root / "output")
                self.assertTrue(all(p.read_bytes() == raw for p, raw in before.items()))
                png = root / "graphics/boot-logo.png"
                png.write_bytes(b"user edit")
                graphics.extract(rom, root)
                self.assertEqual(png.read_bytes(), b"user edit")
                with self.assertRaises(ValueError):
                    graphics.build(rom, root, root / "output")
                graphics.extract(rom, root, force=True)
                self.assertNotEqual(png.read_bytes(), b"user edit")

    def test_assembly_uses_custom_output_for_slots_and_overflow(self):
        with tempfile.TemporaryDirectory() as temporary:
            root = Path(temporary)
            output = root / "custom output"
            data = {"tiles": bytes(32), "palette": bytes(32), "map": bytes(1280)}
            parts = {"tiles": (0, 8, ()), "palette": (8, 32, ()), "map": (40, 8, ())}
            with patch.object(graphics, "PARTS", parts), patch.object(graphics, "logo_data", return_value=data):
                graphics.extract(bytes(48), root)
                for overflow in (None, b"overflow stream"):
                    with self.subTest(overflow=overflow), patch.object(graphics, "pack_part", return_value=(bytes(8), overflow)):
                        graphics.build(bytes(48), root, output)
                        assembly = (output / "graphics.s").read_text()
                        for name in parts:
                            self.assertIn(f'.incbin "{(output / f"graphics-{name}-slot.bin").as_posix()}"', assembly)
                            if overflow is not None:
                                self.assertIn(f'.incbin "{(output / f"graphics-{name}-overflow.bin").as_posix()}"', assembly)
                        self.assertNotIn('"build/assets/', assembly)


if __name__ == "__main__":
    unittest.main()
