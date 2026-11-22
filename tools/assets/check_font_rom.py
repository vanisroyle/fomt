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
    paths = [assets / name for name in ("fonts/small.png", "fonts/single.map", "fonts/large.png",
                                       "fonts/double.map", "charmap.txt", "text/0001.txt")]
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
        for path, data in saved.items():
            path.write_bytes(data)
        alphabet = "".join(font.vietnamese_letters())
        font.sample(Path("baserom.gba"), assets, alphabet, placeholders=True, reclaim_large=True)
        mapping = fa.charmap(assets / "charmap.txt")
        double = font.read_map(assets / "fonts/double.map", "double")
        assert len({mapping[c] for c in alphabet}) == 134
        assert all(len(mapping[c]) == 2 for c in alphabet)
        paths[-1].write_text("# Full Vietnamese integration test\n=== 0\n|" + message + " " + alphabet + "{W}\n", encoding="utf-8")
        make("fomt.gba")
        rom = Path("fomt.gba").read_bytes()
        _, _, script = next(fa.rom_scripts(rom))
        payload = next(p for tag, p in fa.chunks(script) if tag == b"STR ")
        expected = fa.encode(message + " " + alphabet + "{W}", mapping, paths[-1], 3)
        assert fa.strings(payload)[0] == expected
        single = font.read_map(assets / "fonts/single.map", "single")
        assert font.stream_codes(expected, single, double) == {int.from_bytes(mapping[c], "big") for c in alphabet}
        large = font.from_sheet(font.read_png((assets / "fonts/large.png").read_bytes()), 6922, 16)
        used, _ = font.script_usage(original)
        original_double = font.rom_maps(original)["double"]
        for code in used:
            offset = 0x4FA9A0 + original_double[code] * 24
            assert rom[offset:offset + 24] == original[offset:offset + 24]
        slots = set()
        for char in alphabet:
            code = int.from_bytes(mapping[char], "big")
            slot = double[code]
            assert original_double[code] == -1
            assert all(old_code not in used for old_code, old_slot in original_double.items() if old_slot == slot)
            map_offset = 0x523290 + font.codes("double").index(code) * 2
            assert rom[map_offset:map_offset + 2] == slot.to_bytes(2, "little")
            offset = 0x4FA9A0 + slot * 24
            assert rom[offset:offset + 24] == large[slot * 24:slot * 24 + 24]
            slots.add(slot)
        assert len(slots) == 134
        print("Reclaimed sample: 134/134 Vietnamese letters use distinct double-byte codes and large glyphs; all original script-used aliases protected", flush=True)
        print(f"Full-width phrase: {message!r} -> {fa.encode(message, mapping, paths[-1], 3).hex(' ')}", flush=True)
        shutil.copyfile("fomt.gba", "build/assets/font-reclaimed-sample.gba")
    finally:
        for path, data in saved.items():
            path.write_bytes(data)
        make("compare")


if __name__ == "__main__":
    main()
