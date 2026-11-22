"""Childhood-memory illustrations traced through event_scenes.cc."""

import hashlib
import fomt_assets as fa
import indexed_png
import more_graphics as maps
import unpack_codec as codec


TABLE, COUNT = 0xFB1E4, 20
MAP_START, MAP_END = 0x7419F4, 0x743058
TILES, PALETTE = 0x743058, 0x747A14


def symbol(address):
    return f"gRestGraphics_{address:06X}"


def catalogue(rom):
    streams = {}
    addresses = [fa.word(rom, TABLE + i * 4) - fa.BASE for i in range(COUNT)]
    for address in [*addresses, TILES]:
        raw, _, _, size = codec.decode_stream(rom[address:address + 0x40000])
        refs = [TABLE + i * 4 for i, value in enumerate(addresses) if value == address]
        streams[address] = {"raw": raw, "size": size, "references": refs if address != TILES else [0x5FCB4]}
    streams[PALETTE] = {"raw": rom[PALETTE:PALETTE + 64], "size": 64, "references": [0x5FCB8]}
    scene = {"tiles": TILES, "maps": addresses, "width": 32, "height": 20, "reserved_bank": False}
    return scene, streams


def colors(raw, g):
    result = g.palette_rgb(raw)
    result[0] = (255, 255, 255)
    return result


def extract(rom, assets, g, force=False):
    scene, streams = catalogue(rom)
    root = assets / "graphics/rest"
    hints = maps.hints_for(scene, streams)
    image = indexed_png.write(maps.banked_sheet(streams[TILES]["raw"], hints, g), colors(streams[PALETTE]["raw"], g))
    fa.write_missing(root / "childhood.tiles.png", image, force)
    palette = indexed_png.write(maps.swatches(32), g.palette_rgb(streams[PALETTE]["raw"]))
    fa.write_missing(root / "childhood.palette.png", palette, force)
    for index, address in enumerate(scene["maps"]):
        fa.write_missing(root / f"childhood-{index:02d}.map.bin", streams[address]["raw"], force)
    return scene, streams


def edited(rom, assets, g):
    scene, streams = extract(rom, assets, g)
    raw = {address: stream["raw"] for address, stream in streams.items()}
    root = assets / "graphics/rest"
    rows, palette = indexed_png.read((root / "childhood.palette.png").read_bytes())
    if rows != maps.swatches(32):
        raise ValueError("childhood palette swatch pixels are read-only")
    raw[PALETTE] = g.palette_bytes(palette, raw[PALETTE])
    if raw[PALETTE][:2] != streams[PALETTE]["raw"][:2]:
        raise ValueError("childhood palette color 0 is overwritten white at runtime")
    rows, palette = indexed_png.read((root / "childhood.tiles.png").read_bytes())
    if palette != colors(streams[PALETTE]["raw"], g):
        raise ValueError("childhood tile palette is read-only; edit childhood.palette.png")
    raw[TILES] = maps.unbank_sheet(rows, raw[TILES], maps.hints_for(scene, streams), g)
    for index, address in enumerate(scene["maps"]):
        value = (root / f"childhood-{index:02d}.map.bin").read_bytes()
        for entry in maps.map_values(value, 32, 20, raw[TILES], False):
            if entry >> 12 >= 2:
                raise ValueError("childhood maps can use only the two palettes loaded at runtime")
        if address in raw and address in scene["maps"][:index] and raw[address] != value:
            raise ValueError("shared childhood tilemap edits must agree")
        raw[address] = value
    return scene, streams, raw


def build(rom, assets, output, g):
    scene, streams, raw = edited(rom, assets, g)
    digest = hashlib.sha256()
    audit = maps.relocation_audit(streams_as_maps(streams), rom)
    for address, stream in streams.items():
        part = address, stream["size"], tuple(stream["references"])
        slot, overflow = g.pack_part(rom, part, raw[address], stream["raw"], address != PALETTE, *audit)
        prefix = output / f"graphics-rest-{address:06X}"
        fa.write_changed(prefix.with_suffix(".bin"), slot)
        lines = [f".global {symbol(address)}"]
        if overflow:
            fa.write_changed(output / f"graphics-rest-{address:06X}-overflow.bin", overflow)
            lines += ['.pushsection .graphics_overflow,"a",%progbits', '.balign 4', symbol(address) + ':',
                      f'.incbin "{output.as_posix()}/graphics-rest-{address:06X}-overflow.bin"', '.popsection']
        else:
            lines.append(symbol(address) + ':')
        lines.append(f'.incbin "{prefix.as_posix()}.bin"')
        data = ('\n'.join(lines) + '\n').encode()
        fa.write_changed(prefix.with_suffix(".s"), data)
        digest.update(slot + (overflow or b''))
    lines, cursor = [], MAP_START
    for address in sorted(set(scene["maps"])):
        if cursor < address:
            lines.append(f'.incbin "baserom.gba", {cursor}, {address - cursor}')
        lines.append(f'.include "{output.as_posix()}/graphics-rest-{address:06X}.s"')
        cursor = address + streams[address]["size"]
    if cursor < MAP_END:
        lines.append(f'.incbin "baserom.gba", {cursor}, {MAP_END - cursor}')
    fa.write_changed(output / "graphics-rest-maps.s", ('\n'.join(lines) + '\n').encode())
    table = ''.join(f'.4byte {symbol(address)}\n' for address in scene["maps"])
    fa.write_changed(output / "graphics-rest-table.s", table.encode())
    return digest.digest()


def streams_as_maps(streams):
    return {address: dict(stream, compressed=address != PALETTE) for address, stream in streams.items()}


def previews(rom, assets, destination, g):
    scene, streams, raw = edited(rom, assets, g)
    palette = colors(raw[PALETTE], g)
    for index, address in enumerate(scene["maps"]):
        image = maps.preview(raw[TILES], raw[address], palette, 32, 20, False, g)
        fa.write_changed(destination / f"gfx_rest_childhood-{index:02d}.png", image)
    print(f"Remaining graphics previews: {COUNT} childhood-memory layers -> {destination}")


def roundtrip(rom, assets, g):
    _, streams, raw = edited(rom, assets, g)
    if any(raw[a] != stream["raw"] for a, stream in streams.items()):
        raise ValueError("childhood graphics are edited; roundtrip requires unmodified assets")
    print(f"Remaining graphics roundtrip: {len(streams)}/{len(streams)} payloads byte-identical; {len(streams) - 1} Unpack streams")
    return set(streams) - {PALETTE}
