"""Title OBJ resources and shared intro/title background layers."""

import struct

import fomt_assets as fa
import indexed_png
import unpack_codec as codec


RESOURCE = (0x747A74, 0x2764, (0xCB4,))
BACKGROUND = (0x74A9C0, 0x3970, (0x1DC4,))
PALETTE = (0x74E330, 0x200, (0x1DC8,))
MAPS = ((0x74A1D8, 0x3DC, ()), (0x74A5B4, 0x21C, ()),
        (0x74A7D0, 0xF4, ()), (0x74A8C4, 0xFC, ()))
ITEMS = {"title-logo": 0, "title-start": 1, "title-continue": 5, "title-copyright": 9}
RESOURCE_NON_POINTER_STREAMS = ((0x5275D0, 0x21C4), (0x7180E0, 0x29F8))


def sections(raw):
    result, pos = [], 0
    for size in (4, 16, 8, 32, 32, 8, 4):
        if pos + 4 > len(raw):
            raise ValueError("truncated sprite resource header")
        count = struct.unpack_from("<H", raw, pos)[0]
        pos += 4
        result.append((pos, count))
        pos += count * size
        if pos > len(raw):
            raise ValueError("sprite resource section exceeds container")
    if pos != len(raw):
        raise ValueError("unexpected sprite resource trailing bytes")
    return result


def item(raw, index):
    sec = sections(raw)
    if not 0 <= index < sec[1][1]:
        raise ValueError("sprite item index out of range")
    count, first, tiles, tile, palettes, palette, _, _ = struct.unpack_from("<8H", raw, sec[1][0] + index * 16)
    if not count or first + count > sec[2][1] or tile + tiles > sec[3][1] or palette + palettes > sec[4][1]:
        raise ValueError("sprite item references unavailable data")
    objects = [struct.unpack_from("<II", raw, sec[2][0] + (first + n) * 8) for n in range(count)]
    depths = {8 if a & 0x2000 else 4 for a, _ in objects}
    if len(depths) != 1:
        raise ValueError("mixed-depth sprite item")
    bpp = depths.pop()
    if palettes != (16 if bpp == 8 else 1):
        raise ValueError("unsupported sprite palette size")
    return sec[3][0] + tile * 32, tiles * 32, sec[4][0] + palette * 32, palettes * 32, bpp, objects


def sprite_preview(tiles, palette, bpp, objects):
    dimensions = (((8, 8), (16, 16), (32, 32), (64, 64)),
                  ((16, 8), (32, 8), (32, 16), (64, 32)),
                  ((8, 16), (8, 32), (16, 32), (32, 64)))
    cells = []
    for attr, word in objects:
        shape, size = (attr >> 14) & 3, attr >> 30
        if shape == 3 or attr & 0x100:
            raise ValueError("unsupported affine/prohibited sprite")
        width, height = dimensions[shape][size]
        x, y = (attr >> 16) & 511, attr & 255
        x = x - 512 if x >= 256 else x
        y = y - 256 if y >= 128 else y
        start = (word & 1023) * 32
        if start + width * height * bpp // 8 > len(tiles):
            raise ValueError("sprite references unavailable pixels")
        cells.append((x, y, width, height, start, attr))
    left, top = min(c[0] for c in cells), min(c[1] for c in cells)
    width = max(x + w for x, y, w, h, start, attr in cells) - left
    height = max(y + h for x, y, w, h, start, attr in cells) - top
    rows = [bytearray(width) for _ in range(height)]
    for x, y, w, h, start, attr in reversed(cells):
        for dy in range(h):
            for dx in range(w):
                sx = w - 1 - dx if attr & 0x10000000 else dx
                sy = h - 1 - dy if attr & 0x20000000 else dy
                pixel = ((sy // 8 * (w // 8) + sx // 8) * 64 + sy % 8 * 8 + sx % 8)
                bit = pixel * bpp
                value = (tiles[start + bit // 8] >> (bit % 8)) & ((1 << bpp) - 1)
                if value:
                    rows[y - top + dy][x - left + dx] = value
    return indexed_png.write(rows, palette)


def map_entries(raw, tiles, colors):
    if len(raw) != 4096:
        raise ValueError("preserve the 64x32 background map (4096 bytes)")
    entries = [value for (value,) in struct.iter_unpack("<H", raw)]
    if any((value & 1023) >= len(tiles) // 32 or value >> 12 >= colors // 16 for value in entries):
        raise ValueError("background map references an unavailable tile or palette bank")
    return entries


def bank_hints(maps, tiles, colors):
    hints = [None] * (len(tiles) // 32)
    for raw in maps:
        for value in map_entries(raw, tiles, colors):
            tile = value & 1023
            if hints[tile] is None:
                hints[tile] = value >> 12
    return [bank or 0 for bank in hints]


def background_sheet(tiles, hints, g):
    rows = g.tile_sheet(tiles)
    for y, row in enumerate(rows):
        for x in range(len(row)):
            row[x] += hints[y // 8 * 32 + x // 8] * 16
    return rows


def background_tiles(rows, original, hints, g):
    if len(rows) != len(g.tile_sheet(original)) or any(len(row) != 256 for row in rows):
        raise ValueError("preserve background sheet dimensions")
    unbanked = []
    for y, row in enumerate(rows):
        output = bytearray()
        for x, value in enumerate(row):
            bank = hints[y // 8 * 32 + x // 8]
            if value >> 4 != bank:
                raise ValueError(f"background tile must retain palette bank {bank}")
            output.append(value & 15)
        unbanked.append(output)
    return g.sheet_tiles(unbanked, len(original))


def background_preview(tiles, palette, raw, g):
    entries = map_entries(raw, tiles, len(palette))
    sheet = g.tile_sheet(tiles)
    rows = [bytearray(512) for _ in range(256)]
    for i, value in enumerate(entries):
        tile, bank = value & 1023, value >> 12
        for y in range(8):
            for x in range(8):
                sx = 7 - x if value & 0x400 else x
                sy = 7 - y if value & 0x800 else y
                pixel = sheet[tile // 32 * 8 + sy][tile % 32 * 8 + sx]
                rows[i // 64 * 8 + y][i % 64 * 8 + x] = bank * 16 + pixel if pixel else 0
    return indexed_png.write(rows, palette)


def originals(rom):
    resource = codec.decode(rom[RESOURCE[0]:RESOURCE[0] + RESOURCE[1]])[0]
    tiles = codec.decode(rom[BACKGROUND[0]:BACKGROUND[0] + BACKGROUND[1]])[0]
    maps = [codec.decode(rom[offset:offset + size])[0] for offset, size, _ in MAPS]
    palette = rom[PALETTE[0]:PALETTE[0] + PALETTE[1]]
    return resource, tiles, maps, palette


def extract(rom, assets, g, force=False):
    resource, tiles, maps, palette = originals(rom)
    root = assets / "graphics"
    for name, index in ITEMS.items():
        start, size, pal, pal_size, bpp, _ = item(resource, index)
        fa.write_missing(root / f"{name}.png", indexed_png.write(g.tile_sheet(resource[start:start + size], bpp),
                         g.palette_rgb(resource[pal:pal + pal_size])), force)
    hints = bank_hints(maps, tiles, len(palette) // 2)
    fa.write_missing(root / "title-background.png", indexed_png.write(background_sheet(tiles, hints, g), g.palette_rgb(palette)), force)
    for i, raw in enumerate(maps):
        fa.write_missing(root / f"title-background-{i}.map.bin", raw, force)


def build(rom, assets, output, g):
    extract(rom, assets, g)
    resource, tiles, maps, palette = originals(rom)
    rebuilt = bytearray(resource)
    root = assets / "graphics"
    previews = {}
    for name, index in ITEMS.items():
        start, size, pal, pal_size, bpp, objects = item(resource, index)
        rows, colors = indexed_png.read((root / f"{name}.png").read_bytes())
        raw = g.sheet_tiles(rows, size, bpp)
        color_bytes = g.palette_bytes(colors, resource[pal:pal + pal_size])
        rebuilt[start:start + size] = raw
        rebuilt[pal:pal + pal_size] = color_bytes
        previews[f"{name}-preview.png"] = sprite_preview(raw, g.palette_rgb(color_bytes), bpp, objects)
    rows, colors = indexed_png.read((root / "title-background.png").read_bytes())
    hints = bank_hints(maps, tiles, len(palette) // 2)
    new_tiles = background_tiles(rows, tiles, hints, g)
    new_palette = g.palette_bytes(colors, palette)
    new_maps = [(root / f"title-background-{i}.map.bin").read_bytes() for i in range(4)]
    for i, raw in enumerate(new_maps):
        previews[f"title-background-{i}-preview.png"] = background_preview(new_tiles, g.palette_rgb(new_palette), raw, g)
    parts = [("title-resource", RESOURCE, bytes(rebuilt), resource, True),
             ("title-background", BACKGROUND, new_tiles, tiles, True),
             ("title-palette", PALETTE, new_palette, palette, False)]
    parts += [(f"title-map-{i}", part, new_maps[i], maps[i], True) for i, part in enumerate(MAPS)]
    products = {}
    for name, part, raw, original, compressed in parts:
        exclusions = RESOURCE_NON_POINTER_STREAMS if part == RESOURCE else ()
        slot, overflow = g.pack_part(rom, part, raw, original, compressed, exclusions)
        label = f"gUnk_{0x08000000 + part[0]:08X}"
        lines = [f".global {label}"]
        if overflow is not None:
            lines += ['.section .graphics_overflow, "a"', ".balign 4", label + ":",
                      f'.incbin "{(output / f"graphics-{name}-overflow.bin").as_posix()}"', '.section .rodata']
        else:
            lines += [label + ":"]
        lines += [f'.incbin "{(output / f"graphics-{name}-slot.bin").as_posix()}"']
        products[f"graphics-{name}.s"] = ("\n".join(lines) + "\n").encode()
        products[f"graphics-{name}-slot.bin"] = slot
        products[f"graphics-{name}-overflow.bin"] = overflow or b""
    for name, raw in {**products, **previews}.items():
        fa.write_changed(output / name, raw)
    return b"".join(products.values())


def review_previews(output, destination):
    if not destination.is_dir():
        raise ValueError("preview destination must be an existing directory")
    for source in output.glob("title-*-preview.png"):
        fa.write_changed(destination / ("gfx_" + source.name), source.read_bytes())
