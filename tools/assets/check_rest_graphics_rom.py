"""Rebuild a visible portrait pixel and preserve every other ROM byte."""

from pathlib import Path
import shutil

from check_font_rom import make
from font_assets import original_rom
import graphics_assets as g
import indexed_png
import portrait_assets as portraits


def main():
    original = original_rom(Path("baserom.gba"))
    if Path("fomt.gba").read_bytes() != original:
        raise ValueError("start with an unmodified compare-OK ROM")
    path = Path("assets/graphics/portraits/000.png")
    saved = path.read_bytes()
    try:
        item = next(portraits.frames(original))[1]
        start, size, pal, pal_size, bpp, objects = item
        rows, colors = indexed_png.read(saved)
        attr, word = objects[0]
        pixel = (word & 1023) * 64
        x, y = pixel % (32 * 64) // 64 * 8, pixel // (32 * 64) * 8
        rows[y][x] ^= 1
        byte = portraits.START + start + pixel // 2
        path.write_bytes(indexed_png.write(rows, colors))
        make("fomt.gba")
        rebuilt = Path("fomt.gba").read_bytes()
        assert rebuilt == original[:byte] + bytes([original[byte] ^ 1]) + original[byte + 1:]
        raw = rebuilt[portraits.START:portraits.END]
        preview = g.title.sprite_preview(raw[start:start + size], g.palette_rgb(raw[pal:pal + pal_size]), bpp, objects)
        Path("/home/ubuntu/screenshots/gfx_rest_portrait-pixel-edit.png").write_bytes(preview)
        output = Path("/home/ubuntu/rest-graphics-edit.gba")
        shutil.copyfile("fomt.gba", output)
        print("Portrait pixel edit: exactly 1 pixel / 1 ROM byte changed; all other ROM bytes identical", flush=True)
        print("Portrait source: frame 000; OAM-selected tile; shared animation container and suffix preserved", flush=True)
        print(f"Sample ROM: {output}", flush=True)
    finally:
        path.write_bytes(saved)
        make("compare")


if __name__ == "__main__":
    main()
