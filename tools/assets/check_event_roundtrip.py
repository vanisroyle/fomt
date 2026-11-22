"""Verify every original event's CODE/JUMP and complete asset container."""

import argparse
from collections import Counter
from pathlib import Path

import event_scripts as events
import fomt_assets as fa


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--rom", type=Path, default=Path("baserom.gba"))
    parser.add_argument("--assets", type=Path, default=Path("assets"))
    args = parser.parse_args()
    try:
        counts, scripts, jump_scripts = Counter(), 0, 0
        mapping = fa.charmap(args.assets / "charmap.txt")
        for i, _, data in fa.rom_scripts(args.rom.read_bytes()):
            parts = dict(fa.chunks(data))
            code = parts[b"CODE"][4:4 + fa.word(parts[b"CODE"])]
            jump = parts.get(b"JUMP")
            string_count = len(fa.strings(parts[b"STR "]))
            source = events.disassemble(code, jump, string_count)
            if events.assemble(source, string_count) != (code, jump):
                raise ValueError(f"script {i}: event roundtrip mismatch")
            if fa.build_script(args.assets, i, mapping) != data:
                raise ValueError(f"script {i}: asset/container roundtrip mismatch")
            counts.update(op for _, op, _, _ in events.instructions(code))
            scripts += 1
            jump_scripts += jump is not None
        print(f"Event roundtrip: {scripts}/{fa.COUNT} CODE and JUMP chunks and complete scripts byte-identical")
        print(f"Event coverage: {len(events.NAMES)}/37 VM handlers implemented; {len(counts)} observed opcodes / {sum(counts.values())} instructions; {jump_scripts} JUMP chunks")
    except (OSError, ValueError, KeyError) as error:
        parser.exit(1, f"error: {error}\n")


if __name__ == "__main__":
    main()
