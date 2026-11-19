"""Build a local Thomas dialogue ROM with all composed Vietnamese glyphs."""

from pathlib import Path
import argparse
import shutil

from check_font_rom import make
import fomt_assets as fa
import font_assets as font
import font_renderer


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("--halfwidth", action="store_true")
    halfwidth = parser.parse_args().halfwidth
    width = 8 if halfwidth else 16
    assets = Path("assets")
    original = font.original_rom(Path("baserom.gba"))
    if Path("fomt.gba").read_bytes() != original:
        raise ValueError("start with an unmodified compare-OK ROM")
    paths = [assets / name for name in ("fonts/small.png" if halfwidth else "fonts/large.png", "fonts/double.map", "charmap.txt", "text/0867.txt")]
    saved = {path: path.read_bytes() for path in paths}
    phrase = "Người ở đây thật dễ chịu. Ữ Ộ ặ ẫ!"
    lines = ["Người ở đây thật", "dễ chịu. Ữ Ộ ặ ẫ!"]
    try:
        alphabet = "".join(font.vietnamese_letters())
        font.sample(Path("baserom.gba"), assets, alphabet, reclaim_large=True, halfwidth=halfwidth)
        mapping = fa.charmap(assets / "charmap.txt")
        path = paths[-1]
        source = path.read_text(encoding="utf-8-sig")
        start = source.index("=== 0\n")
        end = source.index("=== 1\n", start)
        path.write_text(source[:start] + "=== 0\n|" + "\n|".join(lines) + "{W}\n" + source[end:], encoding="utf-8")
        make("VIETNAMESE_FONT=1" if halfwidth else "VIETNAMESE_FONT=0", "fomt.gba")
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
            if halfwidth:
                assert slot & 0x4000
                slot &= 0x3FFF
            size = 12 if halfwidth else 24
            offset = (0x4F90CC if halfwidth else 0x4FA9A0) + slot * size
            assert rom[offset:offset + size] == font.vietnamese_glyph(char, small, single, width)
            assert font_renderer.assignment(rom, mapping[char])[0] == width // 8
            slots.add(slot)
        assert len(slots) == 134
        font_renderer.validate(rom, mapping, {char: font.vietnamese_glyph(char, small, single, width) for char in alphabet})
        if halfwidth:
            assert rom[0x4FA9A0:0x523290] == original[0x4FA9A0:0x523290]
            for code, slot in font.rom_maps(original)["double"].items():
                if slot >= 0:
                    result = font_renderer.lookup(rom, code)
                    assert result[0:2] == (2, 0x4FA9A0 + slot * 24)
            print("Tagged decoder machine-code emulation: all 134 assignments advance 8px; original full-width mappings preserved", flush=True)
        output = Path("build/assets/vietnamese-thomas-8px.gba" if halfwidth else "build/assets/vietnamese-thomas.gba")
        shutil.copyfile("fomt.gba", output)
        print(f"Composed Vietnamese ROM check: 134/134 glyphs match; 134 distinct {'small' if halfwidth else 'large'} slots; {width}px advance", flush=True)
        print(f"Thomas 0867 string 0: {phrase} (wrapped over two rows)", flush=True)
        print(f"Sample ROM: {output.resolve()}", flush=True)
    finally:
        for path, data in saved.items():
            path.write_bytes(data)
        make("VIETNAMESE_FONT=0", "compare")


if __name__ == "__main__":
    main()
