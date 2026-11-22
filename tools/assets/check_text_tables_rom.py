"""Grow one item name, verify its linked pointers and restore the source."""

from pathlib import Path
import shutil

from check_font_rom import make
import fomt_assets as fa
import text_tables as text


def main():
    rom = text.font.original_rom(Path("baserom.gba"))
    if Path("fomt.gba").read_bytes() != rom:
        raise ValueError("start with an unmodified compare-OK ROM")
    assets = Path("assets")
    records = text.edited(rom, assets)
    index = next(i for i, record in enumerate(records) if "tools/000/name" in record["groups"])
    record = records[index]
    path = assets / "text_tables/strings.txt"
    saved = path.read_bytes()
    source = saved.decode("utf-8")
    old = f"=== {index}\n|{fa.decode(text.source_value(rom, record))}\n"
    new = f"=== {index}\n|Copper Sickle (edited)\n"
    if source.count(old) != 1:
        raise ValueError("item name no longer has the expected source layout")
    try:
        path.write_text(source.replace(old, new), encoding="utf-8")
        make("fomt.gba")
        built = Path("fomt.gba").read_bytes()
        symbols = text.elf_symbols(Path("fomt.elf").read_bytes())
        target = symbols[text.symbol(record["address"])][1] - fa.BASE
        assert target >= 0x75C240, "edited name did not relocate to overflow"
        assert built[target:target + 23] == b"Copper Sickle (edited)\0"
        assert built[record["address"]:record["address"] + record["length"]] == text.source_value(rom, record) + b"\0"
        pointers = {pointer: address for pointer, address in text.entries(rom)[1].items() if address == record["address"]}
        assert pointers
        for pointer in pointers:
            assert fa.word(built, pointer) == fa.BASE + target
        other = [item for i, item in enumerate(records) if i != index]
        for item in other:
            start = item["address"]
            assert built[start:start + item["length"]] == rom[start:start + item["length"]]
        assert built[0x13B288:0x75C240] == rom[0x13B288:0x75C240]
        output = Path("/home/ubuntu/text-table-edit.gba")
        shutil.copyfile("fomt.gba", output)
        print("Text item edit: tools/000/name -> Copper Sickle (edited); original slot preserved", flush=True)
        print(f"Text overflow: ROM {target:#x}; {len(pointers)}/{len(pointers)} symbolic item pointers verified; {len(other)}/{len(other)} other text entries unchanged", flush=True)
        print(f"Sample ROM: {output}", flush=True)
    finally:
        path.write_bytes(saved)
        make("compare")


if __name__ == "__main__":
    main()
