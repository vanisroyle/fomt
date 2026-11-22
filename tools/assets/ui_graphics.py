"""Code-owned 4bpp menu backgrounds and their runtime palette allocations."""

import hashlib
import math
import struct

import fomt_assets as fa
import indexed_png
import more_graphics as world
import unpack_codec as codec


def scene(name, tiles, palette, maps, tile_ref, palette_ref, width, height, base=0, compressed=True, shapes=None, animation=None, tile_base=0, external=False, palette_parent=None, palette_parent_base=0):
    return {"name": name, "tiles": tiles, "palette": palette[0], "palette_size": palette[1],
            "palette_base": base, "maps": maps, "tile_refs": (tile_ref,), "palette_refs": (palette_ref,),
            "width": width, "height": height, "maps_compressed": compressed,
            "shapes": shapes or {}, "animation": animation, "tile_base": tile_base, "external": external,
            "palette_parent": palette_parent, "palette_parent_base": palette_parent_base}


SCENES = (
    scene("farm-overview", 0x5275D0, (0x529794, 512),
          ((0x527238, 0x6ED7C), (0x527348, 0x6ED84), (0x527520, 0x6ED8C)),
          0x6ED6C, 0x6ED74, 64, 44),
    scene("farm-overview-winter", 0x52AA6C, (0x52CA40, 512),
          ((0x52A6D8, 0x6ED80), (0x52A7EC, 0x6ED88), (0x52A9BC, 0x6ED90)),
          0x6ED70, 0x6ED78, 64, 44),
    scene("town-map", 0x755848, (0x757AE0, 192),
          ((0x755230, 0x77F4C), (0x75553C, 0x77F54)),
          0x77F38, 0x77F40, 30, 13, base=1, compressed=False),
    scene("town-map-winter", 0x75822C, (0x75A358, 192),
          ((0x757C14, 0x77F50), (0x757F20, 0x77F58)),
          0x77F3C, 0x77F44, 30, 13, base=1, compressed=False),
    scene("minigame-b7164", 0x72D6D8, (0x72DDE4, 96),
          ((0x72D5CC, 0xB72E8), (0x72D630, 0xB72F0)),
          0xB72F8, 0xB7300, 32, 32),
    scene("minigame-bd0", 0x73BFF0, (0x73CBF4, 192),
          ((0x73BE24, 0xBD04C), (0x73BF48, 0xBD050)),
          0xBD054, 0xBD058, 32, 32),
    scene("minigame-c16", 0x73F6AC, (0x7400E4, 192),
          ((0x73F3C8, 0xC16B8), (0x73F5D4, 0xC16BC)),
          0xC16C0, 0xC16C4, 32, 32, base=1),
    scene("minigame-a2f", 0x74F050, (0x74F2EC, 96),
          ((0x74EF14, 0xA2FD0), (0x74EF3C, 0xA2FD4), (0x74EFEC, 0xA2FD8)),
          0xA2FDC, 0xA2FE0, 32, 20),
    scene("horse-race", 0x72F21C, (0x72FA9C, 352),
          ((0x72F11C, 0x5AC30), (0x72F1BC, 0x5AC38), (0x72F1EC, 0x5AC40), (0x72FBFC, 0x5AC48)),
          0x5AC50, 0x5AC54, 6, 32),
    scene("frisbee", 0x738D1C, (0x7399C4, 160),
          ((0x738AD8, 0x55084), (0x738CC8, 0x5508C), (0x738CF0, 0x55090)),
          0x55094, 0x55098, 32, 20, shapes={0x738CF0: (32, 5)}, animation=(0x739A64, 0x5509C, 48)),
    scene("minigame-b55", 0x72C84C, (0x72D5AC, 32),
          ((0x72C5D0, 0xB5754), (0x72C73C, 0xB575C)),
          0xB5764, 0xB576C, 32, 32, base=7, palette_parent=0x6FD19C),
)

PATCHES = (
    (0x107204, 0x6ECAAC, 0x6ECE70, None, (0x6ECE90, 0x6ECEC0), 6, 4, 384, 10, 1),
    (0x107230, 0x6ECEF0, 0x6ED314, 0x6ED51C, (0x6ED374, 0x6ED448), 21, 5, 448, 9, 3),
    (0x10725C, 0x6F292C, 0x6F2D44, 0x6F2F4C, (0x6F2DA4, 0x6F2E78), 21, 5, 448, 9, 3),
    (0x107288, 0x6EF994, 0x6F0530, None, (0x6F0550, 0x6F0688), 13, 12, 512, 7, 1),
    (0x1072B4, 0x6F07C0, 0x6F1EAC, None, (0x6F1F0C, 0x6F241C), 24, 27, 512, 7, 3),
    (0x1072E0, 0x6ED57C, 0x6ED8E0, 0x6ED990, (0x6ED940, 0x6ED968), 4, 5, 544, 7, 1),
    (0x10730C, 0x6ED9F0, 0x6EEACC, None, (0x6EEB4C, 0x6EEC6C), 12, 12, 512, 7, 4),
)
BLOCKS = ((0x6EAD8C, 0x8220),)
SHARED_MAP = (0x74ECCC, 0x7799C)


def patch_scenes():
    for table, tiles, pal, winter, maps, width, height, tile_base, bank, count in PATCHES:
        for palette_address, field in ((pal, 32), (winter, 36)):
            if palette_address is not None:
                suffix = "-winter" if field == 36 else ""
                yield scene(f"world-patch-{table:06X}{suffix}", tiles, (palette_address, count * 32),
                            tuple((address, table + 4 + i * 4) for i, address in enumerate(maps)),
                            table + 24, table + field, width, height, base=bank, compressed=False,
                            tile_base=tile_base, external=True, palette_parent=0x6FFA84 if field == 36 else 0x6FD19C,
                            palette_parent_base=1)


SCENES += tuple(patch_scenes())

PALETTE_TAILS = ((0x72D5CC, 0x72D7AC), *world.PALETTE_READ_PAST, (0x739A64, 0x739BC4))


def map_shape(entry, address):
    width, height = entry["shapes"].get(address, (entry["width"], entry["height"]))
    return dict(entry, width=width, height=height)


def symbol(address):
    return f"gUiGraphics_{address:06X}"


def catalogue(rom):
    streams = {}

    def add(address, kind, references, compressed, size=None):
        if address not in streams:
            if compressed:
                raw, _, _, size = codec.decode_stream(rom[address:address + 0x40000])
            else:
                raw = rom[address:address + size]
            streams[address] = {"kind": kind, "raw": raw, "size": size,
                                "references": [], "compressed": compressed}
        item = streams[address]
        if item["kind"] != kind or item["compressed"] != compressed:
            raise ValueError(f"conflicting UI resource roles at {address:#x}")
        item["references"].extend(references)

    for entry in SCENES:
        add(entry["tiles"], "tiles", entry["tile_refs"], True)
        add(entry["palette"], "palette", entry["palette_refs"], False, entry["palette_size"])
        for address, reference in entry["maps"]:
            add(address, "map", (reference,), entry["maps_compressed"], entry["width"] * entry["height"] * 2)
        if entry["animation"]:
            address, reference, _ = entry["animation"]
            add(address, "animation", (reference,), True)
        if entry["palette_parent"] and entry["palette_parent"] not in streams:
            address = entry["palette_parent"]
            streams[address] = dict(world.catalogue(rom)[1][address], kind="external")
    if SHARED_MAP:
        add(SHARED_MAP[0], "map", (SHARED_MAP[1],), True)
    return streams


def map_values(raw, entry, tiles, original=None):
    if len(raw) != entry["width"] * entry["height"] * 2:
        raise ValueError(f"{entry['name']}: preserve the {entry['width']}x{entry['height']} tilemap")
    values = [v for (v,) in struct.iter_unpack("<H", raw)]
    for i, value in enumerate(values):
        tile, bank = (value & 1023) - entry["tile_base"], value >> 12
        if not 0 <= tile < len(tiles) // 32:
            if entry["external"] and (original is None or raw[i * 2:i * 2 + 2] == original[i * 2:i * 2 + 2]):
                continue
            if original is None or raw[i * 2:i * 2 + 2] == original[i * 2:i * 2 + 2]:
                if value & 1023 == 1023:
                    continue
            raise ValueError("UI map references a partially loaded or unavailable tile")
        first = entry["palette_base"]
        count = entry["palette_size"] // 32
        if entry["palette_parent"]:
            first, count = entry["palette_parent_base"], 15
        if not first <= bank < first + count and any(tiles[tile * 32:tile * 32 + 32]):
            raise ValueError("UI map references a palette bank not owned by its loader")
    return values


def hints(entry, streams):
    tiles = streams[entry["tiles"]]["raw"]
    result = [None] * math.ceil(len(tiles) / 32)
    for address, _ in entry["maps"]:
        for value in map_values(streams[address]["raw"], map_shape(entry, address), tiles):
            tile, bank = (value & 1023) - entry["tile_base"], value >> 12
            if 0 <= tile < len(result) and result[tile] is None:
                result[tile] = bank if entry["palette_parent"] or bank >= entry["palette_base"] else entry["palette_base"]
    return [value if value is not None else entry["palette_base"] for value in result]


def tile_view(address, streams):
    for entry in SCENES:
        banks = None
        if entry["tiles"] == address:
            banks = hints(entry, streams)
        elif entry["animation"] and entry["animation"][0] == address:
            frame_tiles = entry["animation"][2]
            if len(streams[address]["raw"]) % (frame_tiles * 32):
                raise ValueError("preserve complete UI animation frames")
            banks = hints(entry, streams)[:frame_tiles] * (len(streams[address]["raw"]) // (frame_tiles * 32))
        if banks is not None:
            return entry, banks
    raise ValueError(f"unknown UI tile stream {address:#x}")


def palette(entry, raw, g):
    if entry["palette_parent"]:
        colors = [(0, 0, 0)] * (entry["palette_parent_base"] * 16) + g.palette_rgb(raw[entry["palette_parent"]])
    else:
        colors = [(0, 0, 0)] * (entry["palette_base"] * 16)
    start = entry["palette_base"] * 16
    colors[start:start + entry["palette_size"] // 2] = g.palette_rgb(raw[entry["palette"]])
    return colors


def shared_map_tiles(rom, assets, g):
    rows, _ = indexed_png.read((assets / "graphics/boot-logo.png").read_bytes())
    return g.sheet_tiles(rows, len(g.logo_data(rom)["tiles"]))


def validate_shared_map(raw, tiles):
    if len(raw) != 4096:
        raise ValueError("preserve the 32x64 shared-logo tilemap")
    for (value,) in struct.iter_unpack("<H", raw):
        if value >> 12 or value & 1023 != 1023 and value & 1023 >= len(tiles) // 32:
            raise ValueError("shared-logo map references an unavailable tile or palette bank")


def extract(rom, assets, g, force=False):
    streams = catalogue(rom)
    root = assets / "graphics/ui"
    raw = {address: item["raw"] for address, item in streams.items()}
    if any(item["kind"] == "external" for item in streams.values()):
        world.extract(rom, assets, g, force)
    if SHARED_MAP:
        boot = g.logo_data(rom)
        fa.write_missing(assets / "graphics/boot-logo.png", indexed_png.write(g.tile_sheet(boot["tiles"]), g.palette_rgb(boot["palette"])), force)
    for address, item in streams.items():
        if item["kind"] == "external":
            continue
        elif item["kind"] == "map":
            data, suffix = item["raw"], "map.bin"
        elif item["kind"] == "palette":
            colors = g.palette_rgb(item["raw"])
            data, suffix = indexed_png.write(world.swatches(len(colors)), colors), "palette.png"
        else:
            entry, banks = tile_view(address, streams)
            rows = world.banked_sheet(item["raw"], banks, g)
            data, suffix = indexed_png.write(rows, palette(entry, raw, g)), "tiles.png"
        fa.write_missing(root / f"{address:06X}.{suffix}", data, force)
    return streams


def edited(rom, assets, g):
    streams = extract(rom, assets, g)
    root = assets / "graphics/ui"
    raw = {address: item["raw"] for address, item in streams.items()}
    for address, item in streams.items():
        if item["kind"] == "external":
            _, colors = indexed_png.read((assets / f"graphics/more/{address:06X}.palette.png").read_bytes())
            raw[address] = g.palette_bytes(colors, item["raw"])
        elif item["kind"] == "map":
            raw[address] = (root / f"{address:06X}.map.bin").read_bytes()
        elif item["kind"] == "palette":
            rows, colors = indexed_png.read((root / f"{address:06X}.palette.png").read_bytes())
            if rows != world.swatches(len(item["raw"]) // 2):
                raise ValueError("UI palette swatch indices are read-only")
            raw[address] = g.palette_bytes(colors, item["raw"])
        else:
            entry, banks = tile_view(address, streams)
            rows, colors = indexed_png.read((root / f"{address:06X}.tiles.png").read_bytes())
            before = {a: value["raw"] for a, value in streams.items()}
            if colors != palette(entry, before, g):
                raise ValueError(f"tile palette is read-only; edit {entry['palette']:06X}.palette.png")
            raw[address] = world.unbank_sheet(rows, item["raw"], banks, g)
    for entry in SCENES:
        for address, _ in entry["maps"]:
            shape = map_shape(entry, address)
            map_values(raw[address], shape, raw[entry["tiles"]], streams[address]["raw"])
            if entry["animation"]:
                animation, _, frame_tiles = entry["animation"]
                for start in range(0, len(raw[animation]), frame_tiles * 32):
                    tiles = raw[animation][start:start + frame_tiles * 32] + raw[entry["tiles"]][frame_tiles * 32:]
                    map_values(raw[address], shape, tiles, streams[address]["raw"])
    if SHARED_MAP:
        validate_shared_map(raw[SHARED_MAP[0]], shared_map_tiles(rom, assets, g))
    return streams, raw


def build(rom, assets, output, g):
    streams, raw = edited(rom, assets, g)
    all_streams = {**world.catalogue(rom)[1], **streams}
    audit = world.relocation_audit(all_streams, rom)
    digest = hashlib.sha256()
    for address, item in streams.items():
        if item["kind"] == "external":
            continue
        roots = tuple(sorted(set(item["references"])))
        part = (address, item["size"], roots)
        protected = any(address < end and address + item["size"] > start for start, end in PALETTE_TAILS)
        if protected and raw[address] != item["raw"]:
            if not item["compressed"] or not g.growth_safe(rom, part, *audit):
                raise ValueError("UI palette read-past bytes require audited relocation; original slot must remain unchanged")
            slot, overflow = rom[address:address + item["size"]], codec.encode(raw[address])
        else:
            slot, overflow = g.pack_part(rom, part, raw[address], item["raw"], item["compressed"], *audit)
        prefix = output / f"graphics-ui-{address:06X}"
        fa.write_changed(prefix.with_suffix(".bin"), slot)
        fa.write_changed(output / f"graphics-ui-{address:06X}-overflow.bin", overflow or b"")
        lines = [f".global {symbol(address)}"]
        if overflow:
            lines += ['.pushsection .graphics_overflow,"a",%progbits', '.balign 4', symbol(address) + ':',
                      f'.incbin "{output.as_posix()}/graphics-ui-{address:06X}-overflow.bin"', '.popsection']
        else:
            lines.append(symbol(address) + ':')
        lines.append(f'.incbin "{prefix.as_posix()}.bin"')
        fa.write_changed(prefix.with_suffix(".s"), ('\n'.join(lines) + '\n').encode())
        digest.update(slot + (overflow or b''))
    for start, size in BLOCKS:
        lines, cursor = [], start
        for address in sorted(a for a in streams if start <= a < start + size):
            if cursor > address or address + streams[address]["size"] > start + size:
                raise ValueError("overlapping UI graphics assembly slots")
            if cursor < address:
                lines.append(f'.incbin "baserom.gba", {cursor}, {address - cursor}')
            lines.append(f'.include "{output.as_posix()}/graphics-ui-{address:06X}.s"')
            cursor = address + streams[address]["size"]
        if cursor < start + size:
            lines.append(f'.incbin "baserom.gba", {cursor}, {start + size - cursor}')
        data = ('\n'.join(lines) + '\n').encode()
        fa.write_changed(output / f"graphics-ui-block-{start:06X}.s", data)
        digest.update(data)
    for table, *_ in PATCHES:
        lines = []
        for offset in range(0, 44, 4):
            value = fa.word(rom, table + offset)
            target = value - fa.BASE
            lines.append(f'.4byte {symbol(target)}' if offset in (4, 8, 12, 24, 32, 36) and target in streams else f'.4byte {value}')
        data = ('\n'.join(lines) + '\n').encode()
        fa.write_changed(output / f"graphics-ui-patch-{table:06X}.s", data)
        digest.update(data)
    return digest.digest()


def previews(rom, assets, destination, g):
    streams, raw = edited(rom, assets, g)
    count = 0
    for entry in SCENES:
        tiles, colors = raw[entry["tiles"]], palette(entry, raw, g)
        for layer, (address, _) in enumerate(entry["maps"]):
            shape = map_shape(entry, address)
            width, height = shape["width"], shape["height"]
            rows = [bytearray(width * 8) for _ in range(height * 8)]
            sheet = g.tile_sheet(tiles)
            for i, value in enumerate(map_values(raw[address], shape, tiles, streams[address]["raw"])):
                tile, bank = (value & 1023) - entry["tile_base"], value >> 12
                if not 0 <= tile < len(tiles) // 32 or bank * 16 >= len(colors):
                    continue
                for y in range(8):
                    for x in range(8):
                        sx, sy = (7 - x if value & 0x400 else x), (7 - y if value & 0x800 else y)
                        pixel = sheet[tile // 32 * 8 + sy][tile % 32 * 8 + sx]
                        rows[i // width * 8 + y][i % width * 8 + x] = bank * 16 + pixel if pixel else 0
            fa.write_changed(destination / f"gfx_rest_{entry['name']}-layer-{layer}.png", indexed_png.write(rows, colors))
            count += 1
    if SHARED_MAP:
        tiles = shared_map_tiles(rom, assets, g)
        _, colors = indexed_png.read((assets / "graphics/boot-logo.png").read_bytes())
        colors[0] = (255, 255, 255)
        tilemap = raw[SHARED_MAP[0]]
        sheet = g.tile_sheet(tiles)
        rows = [bytearray(256) for _ in range(512)]
        for i, (value,) in enumerate(struct.iter_unpack("<H", tilemap)):
            tile = value & 1023
            if tile == 1023:
                continue
            for y in range(8):
                for x in range(8):
                    sx, sy = (7 - x if value & 0x400 else x), (7 - y if value & 0x800 else y)
                    rows[i // 32 * 8 + y][i % 32 * 8 + x] = sheet[tile // 32 * 8 + sy][tile % 32 * 8 + sx]
        fa.write_changed(destination / "gfx_rest_shared-logo-loop.png", indexed_png.write(rows, colors))
        count += 1
    print(f"UI previews: {count} verified static background layers -> {destination}")


def roundtrip(rom, assets, g):
    streams, raw = edited(rom, assets, g)
    if any(raw[a] != item["raw"] for a, item in streams.items()):
        raise ValueError("UI graphics are edited; roundtrip requires unmodified assets")
    compressed = {a for a, item in streams.items() if item["compressed"] and item["kind"] != "external"}
    count = sum(item["kind"] != "external" for item in streams.values())
    print(f"UI graphics roundtrip: {count}/{count} payloads byte-identical; {len(compressed)} Unpack streams")
    return compressed
