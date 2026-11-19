"""Opt-in local-ROM font edit and Unicode assignment integration checks."""

from pathlib import Path
import shutil
import subprocess

import fomt_assets as fa
import font_assets as font


def make(*goals):
    result = subprocess.run(["make", "-j8", *goals], capture_output=True, text=True)
    if result.returncode:
        raise RuntimeError(result.stdout + result.stderr)
    for line in result.stdout.splitlines():
        if line.endswith(": OK"):
            print(line, flush=True)


def main():
    assets = Path("assets")
    original = font.original_rom(Path("baserom.gba"))
    if Path("fomt.gba").read_bytes() != original:
        raise ValueError("start with an unmodified compare-OK ROM")
    paths = [assets / name for name in ("fonts/small.png", "fonts/single.map", "charmap.txt", "text/0001.txt")]
    saved = {path: path.read_bytes() for path in paths}
    try:
        png = paths[0]
        rows = font.read_png(saved[png])
        rows[0][0] ^= 1
        png.write_bytes(font.write_png(rows))
        make("fomt.gba")
        edited = Path("fomt.gba").read_bytes()
        changed = [i for i, (a, b) in enumerate(zip(original, edited)) if a != b]
        assert len(edited) == len(original)
        assert changed == [0x4F90CC], changed
        assert edited[0x4F90CC] == original[0x4F90CC] ^ 0x80
        print("PNG edit: exactly ROM byte 0x4F90CC changed (XOR 0x80); all other bytes identical", flush=True)
        png.write_bytes(saved[png])
        message = "Xin chào Việt Nam!"
        font.sample(Path("baserom.gba"), assets, message, placeholders=True)
        mapping = fa.charmap(assets / "charmap.txt")
        text = "# Local integration test\n=== 0\n|" + message + "{W}\n"
        paths[-1].write_text(text, encoding="utf-8")
        make("fomt.gba")
        rom = Path("fomt.gba").read_bytes()
        _, _, script = next(fa.rom_scripts(rom))
        payload = next(p for tag, p in fa.chunks(script) if tag == b"STR ")
        expected = fa.encode(message + "{W}", mapping, paths[-1], 3)
        assert fa.strings(payload)[0] == expected
        single = font.read_map(assets / "fonts/single.map", "single")
        small = font.from_sheet(font.read_png((assets / "fonts/small.png").read_bytes()), 487, 8)
        for char in ("à", "ệ"):
            raw = mapping[char]
            slot = single[raw[0]]
            assert slot >= 0
            assert fa.word(rom, fa.TABLE + 4) >= fa.BASE + fa.START
            assert rom[0x4FA7A0 + raw[0] * 2:0x4FA7A0 + raw[0] * 2 + 2] == slot.to_bytes(2, "little")
            offset = 0x4F90CC + slot * 12
            assert rom[offset:offset + 12] == small[slot * 12:slot * 12 + 12]
        print(f"Unicode sample: {message!r} -> {expected.hex(' ')}; script, map and placeholder glyphs rebuilt", flush=True)
        shutil.copyfile("fomt.gba", "build/assets/font-sample.gba")
    finally:
        for path, data in saved.items():
            path.write_bytes(data)
        make("compare")


if __name__ == "__main__":
    main()
