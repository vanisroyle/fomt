"""Opt-in integration check against a locally supplied ROM; restores its text edit."""

from pathlib import Path
import shutil
import subprocess
import sys

import fomt_assets as fa


def make(*args):
    result = subprocess.run(["make", "-j8", *args], capture_output=True, text=True)
    if result.returncode:
        raise RuntimeError(result.stdout + result.stderr)
    for line in result.stdout.splitlines():
        if line.startswith("Built ") or line.endswith(": OK"):
            print(line, flush=True)


def verify(assets, overflow_from=None):
    rom = Path("fomt.gba").read_bytes()
    mapping = fa.charmap(assets / "charmap.txt")
    count = 0
    moved = 0
    for script_id, start, data in fa.rom_scripts(rom):
        assert data == fa.build_script(assets, script_id, mapping), script_id
        outside = not fa.START <= start < fa.END
        if overflow_from is not None:
            assert outside == (script_id >= overflow_from), (script_id, start)
        moved += outside
        count += 1
    original = Path("baserom.gba").read_bytes()
    assert rom[fa.END:0x75C244] == original[fa.END:0x75C244], "suffix moved/changed"
    print(f"ROM table check: {count}/{fa.COUNT} scripts match assets; {moved} overflow; suffix unchanged", flush=True)
    return rom


def main():
    assets = Path("assets")
    mapping = fa.charmap(assets / "charmap.txt")
    originals = list(fa.rom_scripts(Path("baserom.gba").read_bytes()))
    for script_id, _, data in originals:
        if fa.build_script(assets, script_id, mapping) != data:
            raise ValueError("Integration test requires unmodified extracted assets")
    # Select a late script with text, so extending it spills that script itself.
    for script_id, start, data in reversed(originals):
        payload = next(p for tag, p in fa.chunks(data) if tag == b"STR ")
        entries = fa.strings(payload)
        if entries:
            break
    path = assets / "text" / f"{script_id:04d}.txt"
    saved = path.read_bytes()
    try:
        minimum = max(fa.END - start, 0x800000 - 0x75C244) + 4096
        expected = b"Overflow translation edit test. " * (minimum // 30 + 1)
        entries[0] = expected
        path.write_text(fa.text_file(script_id, fa.BASE + start, entries), encoding="utf-8")
        make("fomt.gba", "SCRIPTS_OVERFLOW_FROM=1329")
        rom = verify(assets)
        assert len(rom) == 0x1000000, "growth past 8 MiB must pad to 16 MiB"
        print(f"Growth check: ROM padded to {len(rom):#x} bytes (16 MiB)", flush=True)
        address = fa.word(rom, fa.TABLE + script_id * 4)
        assert address >= fa.BASE + 0x75C244
        built = dict((i, data) for i, _, data in fa.rom_scripts(rom))[script_id]
        payload = next(p for tag, p in fa.chunks(built) if tag == b"STR ")
        actual = fa.strings(payload)[0]
        assert actual == expected
        print(f"Edited script {script_id}: table -> {address:#010x}; "
              f"parsed {len(actual)} text bytes; starts {actual[:31].decode('ascii')!r}", flush=True)
        path.write_bytes(saved)
        make("fomt.gba", "SCRIPTS_OVERFLOW_FROM=1000")
        verify(assets, overflow_from=1000)
        shutil.copyfile("fomt.gba", "build/assets/overflow-test.gba")
    finally:
        path.write_bytes(saved)
        make("compare", "SCRIPTS_OVERFLOW_FROM=1329")


if __name__ == "__main__":
    try:
        main()
    except (AssertionError, OSError, ValueError, RuntimeError) as error:
        sys.exit(f"Integration check failed: {error}")
