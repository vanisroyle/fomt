"""Local-ROM one-pixel edit, symbolic relocation, and restoration check."""

from pathlib import Path
import argparse
import shutil
import struct

from check_font_rom import make
import graphics_assets as graphics
import indexed_png
import unpack_codec as codec
from font_assets import original_rom


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--title", action="store_true", help="edit the 8bpp title logo instead of the boot logo")
    args = parser.parse_args()
    original = original_rom(Path("baserom.gba"))
    if Path("fomt.gba").read_bytes() != original:
        raise ValueError("start with an unmodified compare-OK ROM")
    path = Path("assets/graphics/title-logo.png" if args.title else "assets/graphics/boot-logo.png")
    saved = path.read_bytes()
    try:
        rows, palette = indexed_png.read(saved)
        rows[0][0] ^= 1
        path.write_bytes(indexed_png.write(rows, palette))
        make("fomt.gba")
        rom = Path("fomt.gba").read_bytes()
        offset, size, refs = graphics.title.RESOURCE if args.title else graphics.TILES
        pointers = [struct.unpack_from("<I", rom, ref)[0] - 0x08000000 for ref in refs]
        assert len(set(pointers)) == 1
        target = pointers[0]
        raw = codec.decode(rom[target:target + 0x40000])[0]
        before = graphics.title.originals(original)[0] if args.title else graphics.logo_data(original)["tiles"]
        pixel = graphics.title.item(before, 0)[0] if args.title else 0
        assert raw == before[:pixel] + bytes([before[pixel] ^ 1]) + before[pixel + 1:]
        if target != offset:
            assert target >= 0x75C244
            assert rom[offset:offset + size] == original[offset:offset + size]
        for pos, (a, b) in enumerate(zip(original, rom)):
            if a != b:
                assert (offset <= pos < offset + size or pos >= 0x75C244
                        or any(ref <= pos < ref + 4 for ref in refs)), hex(pos)
        output = Path("build/assets/title-pixel-edit.gba" if args.title else "build/assets/graphics-pixel-edit.gba")
        shutil.copyfile("fomt.gba", output)
        print(f"{'Title' if args.title else 'Graphics'} pixel edit: exactly 1 decoded tile byte changed; all other decoded bytes identical", flush=True)
        print(f"Graphics tiles -> ROM 0x{target:X}; {len(refs)}/{len(refs)} symbolic pointers verified; original suffix placement preserved", flush=True)
        print(f"Sample ROM: {output.resolve()}", flush=True)
    finally:
        path.write_bytes(saved)
        make("compare")


if __name__ == "__main__":
    main()
