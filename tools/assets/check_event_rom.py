"""Swap Thomas's first two dialogue references, verify the ROM and restore edits."""

from pathlib import Path
import shutil

from check_font_rom import make
import event_scripts as events
import fomt_assets as fa
from font_assets import original_rom


def main():
    original = original_rom(Path("baserom.gba"))
    if Path("fomt.gba").read_bytes() != original:
        raise ValueError("start with an unmodified compare-OK ROM")
    path = Path("assets/scripts/0867.evt")
    saved = path.read_bytes()
    mapping = fa.charmap(Path("assets/charmap.txt"))
    baseline = fa.build_script(Path("assets"), 867, mapping)
    parts = dict(fa.chunks(baseline))
    string_count = len(fa.strings(parts[b"STR "]))
    code, jump = events.assemble(saved.decode("utf-8-sig"), string_count)
    positions = []
    decoded = events.instructions(code)
    for index, (pos, op, flag, value) in enumerate(decoded[:-1]):
        if op in events.PUSHES and not flag and value == len(positions) and decoded[index + 1][1:] == (0x21, 0, 33):
            positions.append(pos)
            if len(positions) == 2:
                break
    if len(positions) != 2:
        raise ValueError("Thomas no longer has the expected first two immediate dialogue calls")
    expected = bytearray(code)
    expected[positions[0] + 1] = 1
    expected[positions[1] + 1] = 0
    source = events.disassemble(bytes(expected), jump, string_count)
    try:
        path.write_text(source, encoding="utf-8")
        make("fomt.gba")
        edited = Path("fomt.gba").read_bytes()
        linked = {i: data for i, _, data in fa.rom_scripts(edited)}
        linked_parts = dict(fa.chunks(linked[867]))
        assert linked_parts[b"CODE"] == fa.pack(len(expected)) + expected + parts[b"CODE"][4 + len(code):]
        assert [(tag, payload) for tag, payload in fa.chunks(linked[867]) if tag != b"CODE"] == [(tag, payload) for tag, payload in fa.chunks(baseline) if tag != b"CODE"]
        events.validate(bytes(expected), jump)
        for i, _, data in fa.rom_scripts(original):
            if i != 867:
                assert linked[i] == data, i
        output = Path("build/assets/event-thomas-swapped.gba")
        shutil.copyfile("fomt.gba", output)
        print("Event edit: Thomas 0867 first two dialogue STR references swapped; CODE reparsed; STR/JUMP/other chunks unchanged", flush=True)
        print("Event ROM check: 1327/1327 other scripts unchanged; symbolic table resolves edited script", flush=True)
        print(f"Sample ROM: {output.resolve()}", flush=True)
    finally:
        path.write_bytes(saved)
        make("compare")


if __name__ == "__main__":
    main()
