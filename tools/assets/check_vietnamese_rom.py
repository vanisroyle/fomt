"""Build a local Thomas dialogue ROM with all composed Vietnamese glyphs."""

from pathlib import Path
import shutil

from check_font_rom import make
import fomt_assets as fa
import font_assets as font
import font_renderer


def main():
    assets = Path("assets")
    original = font.original_rom(Path("baserom.gba"))
    if Path("fomt.gba").read_bytes() != original:
        raise ValueError("start with an unmodified compare-OK ROM")
    paths = [assets / name for name in ("fonts/large.png", "fonts/double.map", "charmap.txt", "text/0867.txt")]
    saved = {path: path.read_bytes() for path in paths}
    phrase = "Người ở đây thật dễ chịu. Ữ Ộ ặ ẫ!"
    lines = ["Người ở đây thật", "dễ chịu. Ữ Ộ ặ ẫ!"]
    try:
        alphabet = "".join(font.vietnamese_letters())
        font.sample(Path("baserom.gba"), assets, alphabet, reclaim_large=True)
        mapping = fa.charmap(assets / "charmap.txt")
        path = paths[-1]
        source = path.read_text(encoding="utf-8-sig")
        start = source.index("=== 0\n")
        end = source.index("=== 1\n", start)
        path.write_text(source[:start] + "=== 0\n|" + "\n|".join(lines) + "{W}\n" + source[end:], encoding="utf-8")
        make("fomt.gba")
        rom = Path("fomt.gba").read_bytes()
        script = next(data for script_id, _, data in fa.rom_scripts(rom) if script_id == 867)
        payload = next(p for tag, p in fa.chunks(script) if tag == b"STR ")
        expected = b"\r\n".join(fa.encode(line, mapping, path, 3) for line in lines) + b"\x05"
        assert fa.strings(payload)[0] == expected
        assert " ".join(lines) == phrase
        single = font.rom_maps(original)["single"]
        small = original[0x4F90CC:0x4FA7A0]
        double = font.rom_maps(rom)["double"]
        slots = set()
        for char in alphabet:
            assert len(mapping[char]) == 2
            slot = double[int.from_bytes(mapping[char], "big")]
            offset = 0x4FA9A0 + slot * 24
            assert rom[offset:offset + 24] == font.vietnamese_glyph(char, small, single)
            slots.add(slot)
        assert len(slots) == 134
        font_renderer.validate(rom, mapping, {char: font.vietnamese_glyph(char, small, single) for char in alphabet})
        output = Path("build/assets/vietnamese-thomas.gba")
        shutil.copyfile("fomt.gba", output)
        print("Composed Vietnamese ROM check: 134/134 glyphs match; 134 distinct large slots", flush=True)
        print(f"Thomas 0867 string 0: {phrase} (wrapped over two rows)", flush=True)
        print(f"Sample ROM: {output.resolve()}", flush=True)
    finally:
        for path, data in saved.items():
            path.write_bytes(data)
        make("compare")


if __name__ == "__main__":
    main()
