"""Dialogue portrait frames owned by the sprite-animation loader."""

import hashlib
import fomt_assets as fa
import indexed_png
import title_graphics as sprite


START, END, SLOT_END = 0x52D984, 0x58BA08, 0x58BA28


def frames(rom):
    raw = rom[START:END]
    sections = sprite.sections(raw)
    for index in range(sections[1][1]):
        item = sprite.item(raw, index)
        if item[4] != 4:
            raise ValueError("portrait objects require 4bpp tiles")
        yield index, item


def extract(rom, assets, g, force=False):
    root = assets / "graphics/portraits"
    raw = rom[START:END]
    for index, (start, size, pal, pal_size, bpp, objects) in frames(rom):
        image = indexed_png.write(g.tile_sheet(raw[start:start + size], bpp),
                                  g.palette_rgb(raw[pal:pal + pal_size]))
        fa.write_missing(root / f"{index:03d}.png", image, force)


def edited(rom, assets, g):
    extract(rom, assets, g)
    original = rom[START:END]
    rebuilt, changed = bytearray(original), {}
    for index, (start, size, pal, pal_size, bpp, objects) in frames(rom):
        path = assets / f"graphics/portraits/{index:03d}.png"
        rows, colors = indexed_png.read(path.read_bytes())
        if len(rows) != len(g.tile_sheet(original[start:start + size], bpp)) or any(len(row) != 256 for row in rows):
            raise ValueError(f"{path}: preserve portrait tile-sheet dimensions")
        tiles = g.sheet_tiles(rows, size, bpp)
        palette = g.palette_bytes(colors, original[pal:pal + pal_size])
        for offset, value, width in ((start, tiles, 1), (pal, palette, 2)):
            for relative in range(0, len(value), width):
                i, unit = offset + relative, value[relative:relative + width]
                if unit == original[i:i + width]:
                    continue
                if i in changed and changed[i] != unit:
                    raise ValueError(f"{path}: conflicting edits to shared portrait bytes at {START + i:#x}")
                rebuilt[i:i + width] = unit
                changed[i] = unit
    return bytes(rebuilt)


def build(rom, assets, output, g):
    raw = edited(rom, assets, g)
    fa.write_changed(output / "graphics-portraits.bin", raw + rom[END:SLOT_END])
    return hashlib.sha256(raw).digest()


def previews(rom, assets, destination, g):
    raw = edited(rom, assets, g)
    count = 0
    for index, (start, size, pal, pal_size, bpp, objects) in frames(rom):
        image = sprite.sprite_preview(raw[start:start + size], g.palette_rgb(raw[pal:pal + pal_size]), bpp, objects)
        fa.write_changed(destination / f"gfx_rest_portrait-{index:03d}.png", image)
        count += 1
    print(f"Portrait previews: {count} OAM-assembled frames -> {destination}")


def roundtrip(rom, assets, g):
    if edited(rom, assets, g) != rom[START:END]:
        raise ValueError("portraits are edited; roundtrip requires unmodified assets")
    count = sum(1 for _ in frames(rom))
    print(f"Portrait roundtrip: {count}/{count} frames, shared tiles/palettes and animation metadata byte-identical")
