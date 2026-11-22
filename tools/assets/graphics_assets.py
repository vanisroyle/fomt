"""Editable logo/title graphics and a conservative custom-Unpack inventory."""

import argparse
from collections import Counter, defaultdict
import hashlib
import json
import math
from pathlib import Path
import struct
import sys
from bisect import bisect_left, bisect_right
from functools import lru_cache

import fomt_assets as fa
from font_assets import original_rom
import indexed_png
import more_graphics as more
import unpack_codec as codec
import title_graphics as title


START, END = 0x527094, 0x75B9F8
TILES = (0x74E648, 0x4F8, (0xFE8, 0x77990))
PALETTE = (0x74EB40, 0x20, (0xFEC, 0x77998))
TILEMAP = (0x74EB60, 0x16C, (0xFF0,))
PARTS = {"tiles": TILES, "palette": PALETTE, "map": TILEMAP}


def palette_rgb(raw):
    return [tuple(((v >> shift) & 31) * 255 // 31 for shift in (0, 5, 10)) for (v,) in struct.iter_unpack("<H", raw)]


def palette_bytes(palette, original):
    if len(palette) * 2 != len(original):
        raise ValueError(f"preserve exactly {len(original) // 2} palette entries and their indices")
    values = []
    for rgb, (old,) in zip(palette, struct.iter_unpack("<H", original)):
        values.append((old & 0x8000) | sum(((v * 31 + 127) // 255) << shift for v, shift in zip(rgb, (0, 5, 10))))
    return struct.pack(f"<{len(values)}H", *values)


def tile_sheet(raw, bpp=4, columns=32):
    if bpp not in (4, 8) or not raw or columns < 1:
        raise ValueError("nonempty 4/8bpp tile data required")
    size = 8 * bpp
    tiles = math.ceil(len(raw) / size)
    rows = [bytearray(columns * 8) for _ in range(math.ceil(tiles / columns) * 8)]
    for tile in range(tiles):
        for y in range(8):
            for x in range(8):
                bit = (tile * 64 + y * 8 + x) * bpp
                value = raw[bit // 8] if bit // 8 < len(raw) else 0
                rows[tile // columns * 8 + y][tile % columns * 8 + x] = (value >> (bit % 8)) & ((1 << bpp) - 1)
    return rows


def sheet_tiles(rows, length, bpp=4, columns=32):
    shape = tile_sheet(bytes(length), bpp, columns)
    if len(rows) != len(shape) or any(len(row) != len(shape[0]) for row in rows):
        raise ValueError(f"preserve sheet dimensions {len(shape[0])}x{len(shape)}")
    raw = bytearray(len(rows) // 8 * columns * 8 * bpp)
    for y, row in enumerate(rows):
        for x, pixel in enumerate(row):
            if not 0 <= pixel < 1 << bpp:
                raise ValueError("pixel exceeds tile bit depth")
            bit = ((y // 8 * columns + x // 8) * 64 + y % 8 * 8 + x % 8) * bpp
            raw[bit // 8] |= pixel << (bit % 8)
    if any(raw[length:]):
        raise ValueError("unused sheet cells and partial-tile padding must remain index 0")
    return bytes(raw[:length])


def logo_data(rom):
    return {name: rom[offset:offset + size] if name == "palette" else codec.decode(rom[offset:offset + size])[0]
            for name, (offset, size, _) in PARTS.items()}


def validate_map(raw, tiles):
    if len(raw) != 32 * 20 * 2:
        raise ValueError("preserve the 32x20 tilemap (1280 bytes)")
    for (entry,) in struct.iter_unpack("<H", raw):
        if entry >> 12 or entry & 1023 >= len(tiles) // 32:
            raise ValueError("tilemap references an unavailable tile or palette bank")


def preview(tiles, tilemap):
    validate_map(tilemap, tiles)
    sheet = tile_sheet(tiles)
    rows = [bytearray(256) for _ in range(160)]
    for i, (entry,) in enumerate(struct.iter_unpack("<H", tilemap)):
        tile = entry & 1023
        for y in range(8):
            for x in range(8):
                sx = 7 - x if entry & 0x400 else x
                sy = 7 - y if entry & 0x800 else y
                rows[i // 32 * 8 + y][i % 32 * 8 + x] = sheet[tile // 32 * 8 + sy][tile % 32 * 8 + sx]
    return rows


def extract(rom, assets, force=False):
    title.extract(rom, assets, sys.modules[__name__], force)
    more.extract(rom, assets, sys.modules[__name__], force)
    data = logo_data(rom)
    root = assets / "graphics"
    fa.write_missing(root / "boot-logo.png", indexed_png.write(tile_sheet(data["tiles"]), palette_rgb(data["palette"])), force)
    fa.write_missing(root / "boot-logo.map.bin", data["map"], force)


@lru_cache(maxsize=2)
def pointer_words(rom):
    return sorted((struct.unpack_from("<I", rom, end - 3)[0], end - 3)
                  for end in range(3, len(rom)) if rom[end] == 8)


@lru_cache(maxsize=2)
def audited_ranges(rom, non_pointer_streams, non_pointer_bytes):
    encoded_ranges = []
    for start, span in non_pointer_streams:
        _, _, _, consumed = codec.decode_stream(rom[start:start + span])
        encoded_ranges.append((start + 4, start + consumed))
    for start, expected in non_pointer_bytes:
        if rom[start:start + len(expected)] != expected:
            raise ValueError(f"audited instruction bytes changed at {start:#x}")
        encoded_ranges.append((start, start + len(expected)))
    return tuple(sorted(encoded_ranges))


def growth_safe(rom, part, non_pointer_streams=(), non_pointer_bytes=()):
    offset, size, known = part
    rom = bytes(rom)
    words = pointer_words(rom)
    ranges = audited_ranges(rom, tuple(non_pointer_streams), tuple(non_pointer_bytes))
    first = bisect_left(words, (0x08000000 + offset, -1))
    last = bisect_left(words, (0x08000000 + offset + size, -1))
    roots = []
    for value, pos in words[first:last]:
        index = bisect_right(ranges, (pos, len(rom))) - 1
        if index >= 0 and ranges[index][0] <= pos and pos + 4 <= ranges[index][1]:
            continue
        if value != 0x08000000 + offset:
            return False
        roots.append(pos)
    return tuple(sorted(roots)) == known


def pack_part(rom, part, raw, original, compressed=True, non_pointer_streams=(), non_pointer_bytes=()):
    offset, size, _ = part
    slot = rom[offset:offset + size]
    if raw == original:
        return slot, None
    packed = codec.encode(raw) if compressed else raw
    if len(packed) <= size:
        return packed + slot[len(packed):], None
    if not growth_safe(rom, part, non_pointer_streams, non_pointer_bytes):
        raise ValueError(f"graphics at 0x{offset:X} grew beyond {size} bytes; non-symbolic/interior references prevent safe relocation")
    return slot, packed


def build(rom, assets, output):
    extract(rom, assets)
    original = logo_data(rom)
    root = assets / "graphics"
    rows, palette = indexed_png.read((root / "boot-logo.png").read_bytes())
    data = {"tiles": sheet_tiles(rows, len(original["tiles"])),
            "palette": palette_bytes(palette, original["palette"]), "map": (root / "boot-logo.map.bin").read_bytes()}
    validate_map(data["map"], data["tiles"])
    products, lines, layout = {}, [], {}
    for name, part in PARTS.items():
        slot, overflow = pack_part(rom, part, data[name], original[name], name != "palette")
        label = f"gUnk_{0x08000000 + part[0]:08X}"
        lines.append(f".global {label}")
        if overflow is not None:
            lines.extend(['.section .graphics_overflow, "a"', ".balign 4", label + ":",
                          f'.incbin "{(output / f"graphics-{name}-overflow.bin").as_posix()}"', ".section .rodata"])
            products[f"graphics-{name}-overflow.bin"] = overflow
        else:
            lines.append(label + ":")
        lines.append(f'.incbin "{(output / f"graphics-{name}-slot.bin").as_posix()}"')
        products[f"graphics-{name}-slot.bin"] = slot
        products[f"graphics-{name}-overflow.bin"] = overflow or b""
        layout[name] = {"overflow": overflow is not None, "decoded_bytes": len(data[name]),
                        "sha256": hashlib.sha256(data[name]).hexdigest()}
    products["graphics.s"] = ("\n".join(lines) + "\n").encode()
    for name, raw in products.items():
        fa.write_changed(output / name, raw)
    fa.write_changed(output / "graphics-layout.json", (json.dumps(layout, indent=2) + "\n").encode())
    fa.write_changed(output / "boot-logo-preview.png", indexed_png.write(preview(data["tiles"], data["map"]), palette))
    title_products = title.build(rom, assets, output, sys.modules[__name__])
    more_products = more.build(rom, assets, output, sys.modules[__name__])
    digest = hashlib.sha256(b"".join(products.values()) + title_products + more_products).hexdigest()
    fa.write_changed(output / "graphics.stamp", (digest + "\n").encode())


def inventory(rom):
    pointers = defaultdict(list)
    for pos, (value,) in enumerate(struct.iter_unpack("<I", rom)):
        if 0x08000000 + START <= value < 0x08000000 + END:
            pointers[value - 0x08000000].append(pos * 4)
    result = []
    for offset in range(START, END - 8, 4):
        if rom[offset] != 0x70 or not 0 < int.from_bytes(rom[offset + 1:offset + 4], "little") <= 0x40000:
            continue
        try:
            raw, fmt, ladder, consumed = codec.decode_stream(rom[offset:min(END, offset + 0x40000)])
        except ValueError:
            continue
        result.append({"offset": offset, "decoded_bytes": len(raw), "packed_bytes": consumed,
                       "format": fmt, "ladder": ladder, "references": pointers[offset]})
    return result


def roundtrip(rom, output, assets=Path("assets")):
    streams = inventory(rom)
    identified = {TILES[0], TILEMAP[0], title.RESOURCE[0], title.BACKGROUND[0], *(part[0] for part in title.MAPS)}
    identified.update(more.roundtrip(rom, assets, sys.modules[__name__]))
    located = {item["offset"] for item in streams}
    if not identified <= located:
        raise ValueError("an identified graphics stream is missing from the inventory")
    exact = 0
    original_bytes = optimized_bytes = greedy_bytes = 0
    sizes = Counter()
    for item in streams:
        offset, size = item["offset"], item["packed_bytes"]
        raw = codec.decode(rom[offset:offset + size])[0]
        packed = codec.encode(raw)
        if codec.decode(packed)[0] != raw:
            raise ValueError(f"encoder roundtrip failed at 0x{offset:X}")
        greedy = codec.encode_greedy(raw)
        exact += greedy == rom[offset:offset + size]
        original_bytes += size
        optimized_bytes += len(packed)
        greedy_bytes += len(greedy)
        sizes["smaller" if len(packed) < size else "equal" if len(packed) == size else "larger"] += 1
        item["optimized_bytes"], item["greedy_bytes"] = len(packed), len(greedy)
    fa.write_changed(output / "graphics-inventory.json", (json.dumps(streams, indent=2) + "\n").encode())
    print(f"Unpack inventory: {len(streams)} valid aligned candidates; {sum(bool(s['references']) for s in streams)} have aligned ROM pointers")
    print(f"Unpack formats (atom/LZSS/diff): {dict(sorted(Counter(s['format'] for s in streams).items()))}")
    print(f"Unpack encoder roundtrip: {len(streams)}/{len(streams)} streams; greedy byte-identical: {exact}/{len(streams)}")
    print(f"Unpack optimized size: {optimized_bytes} bytes / {original_bytes} original ({optimized_bytes / original_bytes:.1%}); "
          f"{sizes['smaller']} smaller / {sizes['equal']} equal / {sizes['larger']} larger")
    print(f"Unpack optimized versus greedy: {optimized_bytes} / {greedy_bytes} bytes ({optimized_bytes / greedy_bytes:.1%})")
    print(f"Identified editable Unpack streams: {len(identified)}/{len(streams)} (boot/title 8; world maps and chicken-feeding backgrounds {len(identified) - 8})")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("extract", "build", "roundtrip"))
    parser.add_argument("--rom", type=Path, default=Path("baserom.gba"))
    parser.add_argument("--assets", type=Path, default=Path("assets"))
    parser.add_argument("--output", type=Path, default=Path("build/assets"))
    parser.add_argument("--force", action="store_true")
    parser.add_argument("--previews", type=Path, help="write title and additional graphics previews to an existing directory")
    args = parser.parse_args()
    try:
        if args.force and args.command != "extract":
            raise ValueError("--force is only valid with extract")
        rom = original_rom(args.rom)
        if args.command == "extract":
            extract(rom, args.assets, args.force)
        elif args.command == "build":
            build(rom, args.assets, args.output)
            if args.previews:
                title.review_previews(args.output, args.previews)
                more.review_previews(rom, args.assets, args.previews, sys.modules[__name__])
        else:
            roundtrip(rom, args.output, args.assets)
    except (OSError, ValueError) as error:
        parser.exit(1, f"error: {error}\n")


if __name__ == "__main__":
    main()
