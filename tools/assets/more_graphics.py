"""Code-owned world-map and chicken-feeding background graphics."""

import hashlib
import json
import struct

import fomt_assets as fa
import indexed_png
import unpack_codec as codec
import graphics_reference_audit


TABLE, COUNT, STRIDE = 0x105EDC, 66, 40
EXCLUDED = ()
PALETTE_READ_PAST = ((0x72FBFC, 0x72FC9C),)
BLOCKS = ((0x6678A0, 0x30080), (0x69B284, 0x3A284), (0x6D74F8, 0x4ECC),
          (0x6DC3F4, 0x158), (0x6FB004, 0x2198), (0x6FD19C, 0xA4),
          (0x6FD240, 0x177F0), (0x714A30, 0x130), (0x714B60, 0x8C), (0x714BEC, 0x2398),
          (0x716F84, 0x134), (0x7170B8, 0x6464),
          (0x72FC34, 0x238), (0x72FE6C, 0x8C), (0x72FEF8, 0x1A48), (0x731940, 0x200))
FEEDING = ((0x72FEF8, "tiles", (0xAE884,)), (0x731940, "palette", (0xAE888,)),
           (0x72FC34, "map", (0xAE87C,)), (0x72FE6C, "map", (0xAE880,)))
EXTRA_ROOTS = {0x6FD240: (0xB573C,), 0x6FB004: (0xB574C,), 0x6FD19C: (0xB5750,)}
PALETTE_VARIANTS = ((16, 0x714A30, 0xA9648), (16, 0x714B60, 0xA9660),
                    (17, 0x714BEC, 0xA9678), (17, 0x716F84, 0xA969C), (17, 0x7170B8, 0xA96B4))
NON_POINTER_STREAMS = ((0x681F2C, 0x5000), (0x72C84C, 0xD60))
INSTRUCTION_BYTES = ((0x1846E, bytes.fromhex("4869c269081c")),
                     (0xACD98, bytes.fromhex("4869c269081c")))


def symbol(address):
    return f"gMoreGraphics_{address:06X}"


def catalogue(rom):
    scenes, streams = [], {}

    def add(address, kind, refs=(), compressed=True):
        if address not in streams:
            if compressed:
                raw, _, _, size = codec.decode_stream(rom[address:address + 0x40000])
            else:
                raw, size = rom[address:address + 512], 512
            streams[address] = {"kind": kind, "raw": raw, "size": size,
                                "references": [], "compressed": compressed}
        stream = streams[address]
        if stream["kind"] != kind:
            raise ValueError(f"conflicting graphics roles at {address:#x}")
        stream["references"].extend(refs)

    rows = [struct.unpack_from("<10I", rom, TABLE + i * STRIDE) for i in range(COUNT)]
    for i, row in enumerate(rows):
        if i in EXCLUDED:
            continue
        addresses = [value - fa.BASE if value else None for value in row[:6]]
        for field, address in enumerate(addresses):
            if address is not None:
                add(address, ("tiles", "palette", "palette", "map", "map", "map")[field])
        scenes.append({"name": f"map-{i:02d}", "id": i, "tiles": addresses[0],
                       "palettes": addresses[1:3], "maps": addresses[3:6],
                       "width": row[8] & 65535, "height": row[8] >> 16, "reserved_bank": True,
                       "unowned_tiles": {11: [399], 15: [647]}.get(i, [])})
        if i == 62:
            overrides = [(0x100, rom[0x6D6368:0x6D63C8].hex()), (0x160, rom[0x699FF4:0x69A074].hex())]
            scenes.append(dict(scenes[-1], name="map-62-g5", palette_overrides=overrides))
    for i, row in enumerate(rows):
        for field, value in enumerate(row[:6]):
            if value and value - fa.BASE in streams:
                streams[value - fa.BASE]["references"].append(TABLE + i * STRIDE + field * 4)
    for address, kind, refs in FEEDING:
        add(address, kind, refs, kind != "palette")
    for address, refs in EXTRA_ROOTS.items():
        if address in streams:
            streams[address]["references"].extend(refs)
    for map_id, address, reference in PALETTE_VARIANTS:
        owner = next((scene for scene in scenes if scene.get("id") == map_id), None)
        if owner is not None:
            add(address, "palette", (reference,))
            scenes.append(dict(owner, name=f"map-{map_id:02d}-palette-{address:06X}", palettes=[address]))
    if FEEDING:
        scenes.append({"name": "chicken-feeding", "tiles": FEEDING[0][0],
                       "palettes": [FEEDING[1][0]], "maps": [FEEDING[2][0], FEEDING[3][0]],
                       "width": 32, "height": 32, "reserved_bank": False})
    spans = sorted((start, start + stream["size"]) for start, stream in streams.items())
    if any(stop > following for (_, stop), (following, _) in zip(spans, spans[1:])):
        raise ValueError("overlapping compressed graphics slots")
    return scenes, streams


def map_values(raw, width, height, tiles, reserved_bank, unowned_tiles=()):
    if width < 1 or height < 1 or len(raw) != width * height * 2:
        raise ValueError(f"preserve the {width}x{height} row-major tilemap")
    if not tiles or len(tiles) % 32:
        raise ValueError("background requires complete 4bpp tiles")
    values = [value for (value,) in struct.iter_unpack("<H", raw)]
    for value in values:
        index = value & 1023
        if index != 1023 and index >= len(tiles) // 32:
            raise ValueError("tilemap references an unavailable tile")
        if reserved_bank and value >> 12 == 0 and index not in unowned_tiles and any(tiles[index * 32:index * 32 + 32]):
            raise ValueError("world-map palette bank zero is reserved; only blank tiles may use it")
    if len(tiles) > 1023 * 32 and any(tiles[1023 * 32:]):
        raise ValueError("tile 1023 must remain blank for the loader's empty-map cells")
    return values


def hints_for(scene, streams):
    tiles = streams[scene["tiles"]]["raw"]
    hints = [None] * (len(tiles) // 32)
    for address in scene["maps"]:
        if address is None:
            continue
        for value in map_values(streams[address]["raw"], scene["width"], scene["height"], tiles, scene["reserved_bank"], scene.get("unowned_tiles", ())):
            tile, bank = value & 1023, value >> 12
            if tile < len(hints) and hints[tile] is None and (bank or not scene["reserved_bank"]):
                hints[tile] = bank
    return [bank if bank is not None else int(scene["reserved_bank"]) for bank in hints]


def banked_sheet(tiles, hints, g):
    rows = g.tile_sheet(tiles)
    for y, row in enumerate(rows):
        for x in range(len(row)):
            tile = y // 8 * 32 + x // 8
            if tile < len(hints):
                row[x] += hints[tile] * 16
    return rows


def unbank_sheet(rows, original, hints, g):
    shape = g.tile_sheet(original)
    if len(rows) != len(shape) or any(len(row) != len(shape[0]) for row in rows):
        raise ValueError("preserve tile sheet dimensions")
    output = []
    for y, row in enumerate(rows):
        unbanked = bytearray()
        for x, pixel in enumerate(row):
            tile = y // 8 * 32 + x // 8
            if tile >= len(hints):
                if pixel:
                    raise ValueError("unused sheet padding must remain zero")
            elif pixel >> 4 != hints[tile]:
                raise ValueError(f"tile {tile} must retain palette bank {hints[tile]}")
            unbanked.append(pixel & 15)
        output.append(unbanked)
    return g.sheet_tiles(output, len(original))


def swatches(count):
    return [bytearray(range(i, min(i + 16, count))) for i in range(0, count, 16)]


def palette_for(scene, address, streams):
    raw = streams[address]["raw"]
    expected = 480 if scene["reserved_bank"] else 512
    if len(raw) != expected:
        raise ValueError(f"{scene['name']}: palette loader requires {expected} bytes")
    if scene.get("palette_overrides"):
        raw = bytearray(raw)
        for offset, value in scene["palette_overrides"]:
            value = bytes.fromhex(value)
            raw[offset:offset + len(value)] = value
        raw = bytes(raw)
    return (bytes(32) if scene["reserved_bank"] else b"") + raw


def owners(scenes):
    result = {}
    for scene in sorted(scenes, key=lambda item: item.get("id") in (11, 15, 62)):
        result.setdefault(scene["tiles"], scene)
    return result


def extract(rom, assets, g, force=False):
    scenes, streams = catalogue(rom)
    root = assets / "graphics/more"
    manifest = {"scenes": scenes, "streams": {f"{a:06X}": {k: v for k, v in s.items() if k != "raw"}
                                               for a, s in sorted(streams.items())}}
    fa.write_changed(root / "catalog.json", (json.dumps(manifest, indent=2) + "\n").encode())
    tile_owners = owners(scenes)
    for address, stream in sorted(streams.items()):
        kind, raw = stream["kind"], stream["raw"]
        if kind == "map":
            data, suffix = raw, "map.bin"
        elif kind == "palette":
            colors = g.palette_rgb(raw)
            data, suffix = indexed_png.write(swatches(len(colors)), colors), "palette.png"
        else:
            scene = tile_owners[address]
            colors = g.palette_rgb(palette_for(scene, scene["palettes"][0], streams))
            data = indexed_png.write(banked_sheet(raw, hints_for(scene, streams), g), colors)
            suffix = "tiles.png"
        fa.write_missing(root / f"{address:06X}.{suffix}", data, force)
    return scenes, streams


def edited(rom, assets, g):
    scenes, streams = extract(rom, assets, g)
    root = assets / "graphics/more"
    raw = {address: stream["raw"] for address, stream in streams.items()}
    for address, stream in streams.items():
        if stream["kind"] == "map":
            raw[address] = (root / f"{address:06X}.map.bin").read_bytes()
        elif stream["kind"] == "palette":
            rows, colors = indexed_png.read((root / f"{address:06X}.palette.png").read_bytes())
            if rows != swatches(len(stream["raw"]) // 2):
                raise ValueError("palette swatch pixels are read-only; edit their indexed colors")
            raw[address] = g.palette_bytes(colors, stream["raw"])
    protected = fa.word(rom, TABLE + 62 * STRIDE + 4) - fa.BASE if COUNT > 62 else None
    if protected in raw and raw[protected][0x100:0x1E0] != streams[protected]["raw"][0x100:0x1E0]:
        raise ValueError("map 62 overwrites these shared palette colors at runtime; preserve bytes 0x100..0x1DF")
    for address, scene in owners(scenes).items():
        rows, colors = indexed_png.read((root / f"{address:06X}.tiles.png").read_bytes())
        reference = palette_for(scene, scene["palettes"][0], streams)
        if g.palette_bytes(colors, reference) != reference:
            raise ValueError(f"{address:06X}.tiles.png: palette is read-only; edit {scene['palettes'][0]:06X}.palette.png")
        raw[address] = unbank_sheet(rows, streams[address]["raw"], hints_for(scene, streams), g)
    for scene in scenes:
        for tile in scene.get("unowned_tiles", ()):
            address = scene["tiles"]
            if raw[address][tile * 32:tile * 32 + 32] != streams[address]["raw"][tile * 32:tile * 32 + 32]:
                raise ValueError(f"{scene['name']}: tile {tile} also uses unowned palette bank zero; its pixels are read-only")
        for address in scene["maps"]:
            if address is not None:
                values = map_values(raw[address], scene["width"], scene["height"], raw[scene["tiles"]], scene["reserved_bank"], scene.get("unowned_tiles", ()))
                for i, value in enumerate(values):
                    if value >> 12 == 0 and value & 1023 in scene.get("unowned_tiles", ()):
                        if raw[address][i * 2:i * 2 + 2] != streams[address]["raw"][i * 2:i * 2 + 2]:
                            raise ValueError("unowned palette-bank-zero map cells cannot be introduced or changed")
    return scenes, streams, raw


def relocation_audit(streams, rom=None):
    compressed = tuple((address, stream["size"]) for address, stream in sorted(streams.items()) if stream["compressed"])
    additional = graphics_reference_audit.regions(rom) if rom is not None else ()
    return compressed + NON_POINTER_STREAMS, INSTRUCTION_BYTES + additional


def emit_part(rom, address, stream, value, output, g, audit=((), ())):
    part = (address, stream["size"], tuple(sorted(set(stream["references"]))))
    if value != stream["raw"] and any(address < end and address + stream["size"] > start for start, end in PALETTE_READ_PAST):
        if not stream["compressed"] or not g.growth_safe(rom, part, *audit):
            raise ValueError("palette read-past bytes require audited relocation; original slot must remain unchanged")
        slot, overflow = rom[address:address + stream["size"]], codec.encode(value)
    else:
        slot, overflow = g.pack_part(rom, part, value, stream["raw"], stream["compressed"], *audit)
    prefix = f"graphics-more-{address:06X}"
    fa.write_changed(output / f"{prefix}-slot.bin", slot)
    fa.write_changed(output / f"{prefix}-overflow.bin", overflow or b"")
    lines = [f".global {symbol(address)}"]
    if overflow is not None:
        lines += ['.pushsection .graphics_overflow,"a",%progbits', '.balign 4', symbol(address) + ":",
                  f'.incbin "{(output / (prefix + "-overflow.bin")).as_posix()}"', '.popsection']
    else:
        lines.append(symbol(address) + ":")
    lines.append(f'.incbin "{(output / (prefix + "-slot.bin")).as_posix()}"')
    return lines, {"overflow": overflow is not None, "size": stream["size"],
                   "kind": stream["kind"], "compressed": stream["compressed"],
                   "references": part[2], "decoded_bytes": len(value),
                   "packed_sha256": hashlib.sha256(slot + (overflow or b"")).hexdigest(),
                   "sha256": hashlib.sha256(value).hexdigest()}


def build(rom, assets, output, g):
    scenes, streams, raw = edited(rom, assets, g)
    fragments, layout, products = {}, {}, []
    audit = relocation_audit(streams, rom)
    for address, stream in sorted(streams.items()):
        fragments[address], layout[f"{address:06X}"] = emit_part(rom, address, stream, raw[address], output, g, audit)
    covered = set()
    for start, size in BLOCKS:
        lines, cursor = [], start
        for address in sorted(a for a in streams if start <= a < start + size):
            if cursor < address:
                lines.append(f'.incbin "baserom.gba", {cursor}, {address - cursor}')
            if address + streams[address]["size"] > start + size:
                raise ValueError("graphics stream crosses an assembly slot boundary")
            lines.extend(fragments[address])
            cursor = address + streams[address]["size"]
            covered.add(address)
        if cursor < start + size:
            lines.append(f'.incbin "baserom.gba", {cursor}, {start + size - cursor}')
        data = ("\n".join(lines) + "\n").encode()
        fa.write_changed(output / f"graphics-more-{start:06X}.s", data)
        products.append(data)
    if covered != set(streams):
        raise ValueError("a graphics stream is missing its assembly slot")
    lines = []
    for pos in range(TABLE, TABLE + COUNT * STRIDE, 4):
        value = fa.word(rom, pos)
        field = (pos - TABLE) % STRIDE
        target = value - fa.BASE
        lines.append(f".4byte {symbol(target)}" if field < 24 and target in streams else f".4byte {value}")
    table = ("\n".join(lines) + "\n").encode()
    fa.write_changed(output / "graphics-more-table.s", table)
    metadata = (json.dumps(layout, indent=2) + "\n").encode()
    fa.write_changed(output / "graphics-more-layout.json", metadata)
    return b"".join(products) + table + metadata


def preview(tiles, raw_map, palette, width, height, reserved_bank, g, unowned_tiles=()):
    values = map_values(raw_map, width, height, tiles, reserved_bank, unowned_tiles)
    sheet = g.tile_sheet(tiles)
    rows = [bytearray(width * 8) for _ in range(height * 8)]
    for i, value in enumerate(values):
        tile, bank = value & 1023, value >> 12
        if tile == 1023 or bank == 0 and tile in unowned_tiles:
            continue
        for y in range(8):
            sy = 7 - y if value & 0x800 else y
            for x in range(8):
                sx = 7 - x if value & 0x400 else x
                pixel = sheet[tile // 32 * 8 + sy][tile % 32 * 8 + sx]
                rows[i // width * 8 + y][i % width * 8 + x] = bank * 16 + pixel if pixel else 0
    return indexed_png.write(rows, palette)


def review_previews(rom, assets, destination, g):
    if not destination.is_dir():
        raise ValueError("preview destination must be an existing directory")
    scenes, streams, raw = edited(rom, assets, g)
    count = 0
    for scene in scenes:
        address = scene["palettes"][0]
        palette = palette_for(scene, address, {a: dict(streams[a], raw=value) for a, value in raw.items()})
        for layer, address in enumerate(scene["maps"]):
            if address is None:
                continue
            image = preview(raw[scene["tiles"]], raw[address], g.palette_rgb(palette),
                            scene["width"], scene["height"], scene["reserved_bank"], g, scene.get("unowned_tiles", ()))
            name = f"gfx_more_{scene['name']}-layer-{layer}.png"
            fa.write_changed(destination / name, image)
            count += 1
    print(f"More graphics previews: {count} verified background layers -> {destination}")


def roundtrip(rom, assets, g):
    scenes, streams, raw = edited(rom, assets, g)
    for address, value in raw.items():
        if value != streams[address]["raw"]:
            raise ValueError(f"more graphics {address:#x} is edited; roundtrip requires unmodified assets")
    compressed = sum(stream["compressed"] for stream in streams.values())
    print(f"More graphics source roundtrip: {len(streams)}/{len(streams)} payloads byte-identical; {compressed} Unpack streams; {len(scenes)} background definitions")
    audit = relocation_audit(streams, rom)
    movable = sum(g.growth_safe(rom, (address, stream["size"], tuple(sorted(set(stream["references"])))), *audit)
                  for address, stream in streams.items() if stream["compressed"])
    print(f"More graphics relocation audit: {movable}/{compressed} compressed streams can relocate through audited symbolic roots")
    return {a for a, stream in streams.items() if stream["compressed"]}
