"""Editable boot-logo graphics and a conservative custom-Unpack inventory."""

import argparse
from collections import Counter, defaultdict
import hashlib
import json
import math
from pathlib import Path
import struct

import fomt_assets as fa
from font_assets import original_rom
import indexed_png
import unpack_codec as codec


START, END = 0x527094, 0x75B9F8
TILES = (0x74E648, 0x4F8, (0xFE8, 0x77990))
PALETTE = (0x74EB40, 0x20, (0xFEC, 0x77998))
TILEMAP = (0x74EB60, 0x16C, (0xFF0,))
PARTS = {"tiles": TILES, "palette": PALETTE, "map": TILEMAP}


def palette_rgb(raw):
    return [tuple(((v >> shift) & 31) * 255 // 31 for shift in (0, 5, 10)) for (v,) in struct.iter_unpack("<H", raw)]


def palette_bytes(palette, original):
    if len(palette) * 2 != len(original):
        raise ValueError("preserve exactly 16 palette entries and their indices")
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
        if entry >> 12 or entry & 1023 >= math.ceil(len(tiles) / 32):
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
    data = logo_data(rom)
    root = assets / "graphics"
    fa.write_missing(root / "boot-logo.png", indexed_png.write(tile_sheet(data["tiles"]), palette_rgb(data["palette"])), force)
    fa.write_missing(root / "boot-logo.map.bin", data["map"], force)


def growth_safe(rom, part):
    offset, size, known = part
    address = struct.pack("<I", 0x08000000 + offset)
    found, pos = [], 0
    while True:
        pos = rom.find(address, pos)
        if pos < 0:
            break
        found.append(pos)
        pos += 1
    if tuple(found) != known:
        return False
    for end in range(3, len(rom)):
        if rom[end] == 8:
            value = struct.unpack_from("<I", rom, end - 3)[0]
            if 0x08000000 + offset < value < 0x08000000 + offset + size:
                return False
    return True


def pack_part(rom, part, raw, original, compressed=True):
    offset, size, _ = part
    slot = rom[offset:offset + size]
    if raw == original:
        return slot, None
    packed = codec.encode(raw) if compressed else raw
    if len(packed) <= size:
        return packed + slot[len(packed):], None
    if not growth_safe(rom, part):
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
                          f'.incbin "build/assets/graphics-{name}-overflow.bin"', ".section .rodata"])
            products[f"graphics-{name}-overflow.bin"] = overflow
        else:
            lines.append(label + ":")
        lines.append(f'.incbin "build/assets/graphics-{name}-slot.bin"')
        products[f"graphics-{name}-slot.bin"] = slot
        products[f"graphics-{name}-overflow.bin"] = overflow or b""
        layout[name] = {"overflow": overflow is not None, "decoded_bytes": len(data[name]),
                        "sha256": hashlib.sha256(data[name]).hexdigest()}
    products["graphics.s"] = ("\n".join(lines) + "\n").encode()
    for name, raw in products.items():
        fa.write_changed(output / name, raw)
    fa.write_changed(output / "graphics-layout.json", (json.dumps(layout, indent=2) + "\n").encode())
    fa.write_changed(output / "boot-logo-preview.png", indexed_png.write(preview(data["tiles"], data["map"]), palette))
    digest = hashlib.sha256(b"".join(products.values())).hexdigest()
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


def roundtrip(rom, output):
    streams = inventory(rom)
    exact = 0
    for item in streams:
        offset, size = item["offset"], item["packed_bytes"]
        raw = codec.decode(rom[offset:offset + size])[0]
        packed = codec.encode(raw)
        if codec.decode(packed)[0] != raw:
            raise ValueError(f"encoder roundtrip failed at 0x{offset:X}")
        exact += packed == rom[offset:offset + size]
    fa.write_changed(output / "graphics-inventory.json", (json.dumps(streams, indent=2) + "\n").encode())
    print(f"Unpack inventory: {len(streams)} valid aligned candidates; {sum(bool(s['references']) for s in streams)} have aligned ROM pointers")
    print(f"Unpack formats (atom/LZSS/diff): {dict(sorted(Counter(s['format'] for s in streams).items()))}")
    print(f"Unpack encoder roundtrip: {len(streams)}/{len(streams)} streams; greedy byte-identical: {exact}/{len(streams)}")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("extract", "build", "roundtrip"))
    parser.add_argument("--rom", type=Path, default=Path("baserom.gba"))
    parser.add_argument("--assets", type=Path, default=Path("assets"))
    parser.add_argument("--output", type=Path, default=Path("build/assets"))
    parser.add_argument("--force", action="store_true")
    args = parser.parse_args()
    try:
        if args.force and args.command != "extract":
            raise ValueError("--force is only valid with extract")
        rom = original_rom(args.rom)
        if args.command == "extract":
            extract(rom, args.assets, args.force)
        elif args.command == "build":
            build(rom, args.assets, args.output)
        else:
            roundtrip(rom, args.output)
    except (OSError, ValueError) as error:
        parser.exit(1, f"error: {error}\n")


if __name__ == "__main__":
    main()
