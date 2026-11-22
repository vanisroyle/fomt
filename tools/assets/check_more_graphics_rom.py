"""Local-ROM world-map pixel edit, linked references, and restoration check."""

import argparse
from pathlib import Path
import shutil
import struct

from check_font_rom import make
from font_assets import original_rom
import graphics_assets as g
import indexed_png
import more_graphics as more


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--output", type=Path, default=Path("build/assets/more-graphics-pixel-edit.gba"))
    args = parser.parse_args()
    original = original_rom(Path("baserom.gba"))
    if Path("fomt.gba").read_bytes() != original:
        raise ValueError("start with an unmodified compare-OK ROM")
    scenes, streams = more.catalogue(original)
    scene = next(scene for scene in scenes if scene["name"] == "map-03")
    address = scene["tiles"]
    before = streams[address]["raw"]
    tile = next(value & 1023 for (value,) in struct.iter_unpack("<H", streams[scene["maps"][0]]["raw"])
                if value >> 12 and value & 1023 != 1023 and any(before[(value & 1023) * 32:(value & 1023) * 32 + 32]))
    byte = next(i for i in range(tile * 32, tile * 32 + 32) if before[i])
    nibble = 0 if before[byte] & 15 else 1
    x, y = tile % 32 * 8 + byte % 4 * 2 + nibble, tile // 32 * 8 + byte % 32 // 4
    path = Path(f"assets/graphics/more/{address:06X}.tiles.png")
    saved = path.read_bytes()
    try:
        rows, palette = indexed_png.read(saved)
        rows[y][x] ^= 1
        path.write_bytes(indexed_png.write(rows, palette))
        make("fomt.gba")
        rom = Path("fomt.gba").read_bytes()
        expected = before[:byte] + bytes([before[byte] ^ 1 << (nibble * 4)]) + before[byte + 1:]
        target = None
        for start, stream in streams.items():
            refs = stream["references"]
            pointers = {struct.unpack_from("<I", rom, ref)[0] - g.fa.BASE for ref in refs}
            assert len(pointers) == 1, hex(start)
            pointer = pointers.pop()
            raw = g.codec.decode(rom[pointer:pointer + 0x40000])[0] if stream["compressed"] else rom[pointer:pointer + len(stream["raw"])]
            assert raw == (expected if start == address else stream["raw"]), hex(start)
            if start == address:
                target = pointer
        size = streams[address]["size"]
        refs = streams[address]["references"]
        if target != address:
            assert target >= 0x75C244
            assert rom[address:address + size] == original[address:address + size]
        for pos, (a, b) in enumerate(zip(original, rom)):
            if a != b:
                assert address <= pos < address + size or pos >= 0x75C244 or any(ref <= pos < ref + 4 for ref in refs), hex(pos)
        shutil.copyfile("fomt.gba", args.output)
        print("More graphics pixel edit: map-03, exactly 1 pixel / 1 decoded tile byte changed", flush=True)
        print(f"More graphics verification: {len(streams) - 1}/{len(streams) - 1} other payloads unchanged; all symbolic pointers verified", flush=True)
        print(f"More graphics tiles -> ROM 0x{target:X}; {len(refs)}/{len(refs)} roots verified; original slot and suffix preserved", flush=True)
        print(f"Sample ROM: {args.output.resolve()}", flush=True)
    finally:
        path.write_bytes(saved)
        make("compare")


if __name__ == "__main__":
    main()
