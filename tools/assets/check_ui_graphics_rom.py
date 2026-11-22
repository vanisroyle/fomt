"""Paint a visible minigame UI marker; verify graphics payloads and restore."""

import argparse
from collections import Counter
from pathlib import Path
import shutil
import struct
import tempfile

from check_font_rom import make
from font_assets import original_rom
import graphics_assets as g
import indexed_png
import ui_graphics as ui


def check(scene_name, output, solid_tiles=None):
    original = original_rom(Path("baserom.gba"))
    if Path("fomt.gba").read_bytes() != original:
        raise ValueError("start with an unmodified compare-OK ROM")
    assets = Path("assets")
    streams = ui.catalogue(original)
    entry = next(item for item in ui.SCENES if item["name"] == scene_name)
    address = entry["tiles"]
    before = streams[address]["raw"]
    values = [v[0] for v in struct.iter_unpack("<H", streams[entry["maps"][1][0]]["raw"])]
    counts = Counter(v & 1023 for v in values)
    tile = None if solid_tiles is not None else next(v & 1023 for v in values if counts[v & 1023] == 1 and (v & 1023) < len(before) // 32 and
                any(before[(v & 1023) * 32:(v & 1023) * 32 + 32]))
    if solid_tiles is not None:
        part = (address, streams[address]["size"], tuple(streams[address]["references"]))
        audit = g.more.relocation_audit({**g.more.catalogue(original)[1], **streams}, original)
        assert g.growth_safe(original, part, *audit)
        unknown = bytearray(original)
        struct.pack_into("<I", unknown, 0x7FFFF0, g.fa.BASE + address + 4)
        assert not g.growth_safe(bytes(unknown), part, *audit)
        print(f"UI relocation guard: {scene_name} audited; injected unknown interior reference rejected", flush=True)
    png = assets / f"graphics/ui/{address:06X}.tiles.png"
    saved = png.read_bytes()
    try:
        rows, colors = indexed_png.read(saved)
        selected = solid_tiles if solid_tiles is not None else (tile,)
        for tile in selected:
            if not 0 <= tile < len(before) // 32:
                raise ValueError("solid tile index outside resource")
            x, y = tile % 32 * 8, tile // 32 * 8
            bank = rows[y][x] >> 4
            choices = range(bank * 16 + 1, bank * 16 + 16)
            dark = min(choices, key=lambda i: sum(colors[i]))
            light = max(choices, key=lambda i: sum(colors[i]))
            for dy in range(8):
                for dx in range(8):
                    rows[y + dy][x + dx] = light if solid_tiles is not None or dx == dy or dx == 7 - dy else dark
        png.write_bytes(indexed_png.write(rows, colors))
        expected = ui.edited(original, assets, g)[1][address]
        for i in range(len(before) // 32):
            if i not in selected:
                assert expected[i * 32:(i + 1) * 32] == before[i * 32:(i + 1) * 32]
        assert all(expected[i * 32:(i + 1) * 32] != before[i * 32:(i + 1) * 32] for i in selected)
        assert expected != before
        make("fomt.gba")
        rebuilt = Path("fomt.gba").read_bytes()
        rest = {a: dict(item, compressed=a != g.rest.PALETTE) for a, item in g.rest.catalogue(original)[1].items()}
        combined = {**g.more.catalogue(original)[1], **rest, **streams}
        target = None
        for start, stream in combined.items():
            pointers = {g.fa.word(rebuilt, ref) - g.fa.BASE for ref in stream["references"]}
            assert len(pointers) == 1, hex(start)
            pointer = pointers.pop()
            raw = g.codec.decode(rebuilt[pointer:pointer + 0x40000])[0] if stream["compressed"] else rebuilt[pointer:pointer + len(stream["raw"])]
            assert raw == (expected if start == address else stream["raw"]), hex(start)
            if start == address:
                target = pointer
        size, refs = streams[address]["size"], set(streams[address]["references"])
        if target != address:
            assert rebuilt[address:address + size] == original[address:address + size]
        _, _, _, packed_size = g.codec.decode_stream(rebuilt[target:target + 0x40000])
        for pos, (old, new) in enumerate(zip(original, rebuilt)):
            if old != new:
                assert target <= pos < target + packed_size or any(ref <= pos < ref + 4 for ref in refs), hex(pos)
        if solid_tiles is None:
            with tempfile.TemporaryDirectory() as directory:
                ui.previews(original, assets, Path(directory), g)
                shutil.copyfile(Path(directory) / "gfx_rest_minigame-b7164-layer-1.png", "/home/ubuntu/screenshots/gfx_rest_ui-panel-visible-edit.png")
        shutil.copyfile("fomt.gba", output)
        print(f"UI visible edit: {scene_name}, tiles {tuple(selected)}, {'solid' if solid_tiles is not None else 'contrasting X'}; all other decoded tiles unchanged", flush=True)
        print(f"UI verification: {len(combined) - 1}/{len(combined) - 1} other payloads unchanged; all symbolic roots verified", flush=True)
        print(f"UI compression: {packed_size} bytes / {size} original slot", flush=True)
        print(f"UI tiles -> ROM 0x{target:X}; {len(refs)}/{len(refs)} symbolic roots; original suffix preserved", flush=True)
        print(f"Sample ROM: {output}", flush=True)
    finally:
        png.write_bytes(saved)
        make("compare")


if __name__ == "__main__":
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--scene", default="minigame-b7164")
    parser.add_argument("--output", type=Path, default=Path("/home/ubuntu/ui-graphics-edit.gba"))
    parser.add_argument("--solid-tiles", type=int, nargs=2)
    args = parser.parse_args()
    check(args.scene, args.output, args.solid_tiles)
