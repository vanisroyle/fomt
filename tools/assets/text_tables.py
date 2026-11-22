"""Editable, code-referenced non-script text; Python standard library only."""

import argparse
import hashlib
import json
from pathlib import Path
import re
import struct

import fomt_assets as fa
import font_assets as font
import text_table_catalog as catalog


ITEM_RODATA = 0xE9614
ITEM_END = 0xF04E4
SLOT = re.compile(r'\.(?:incbin "baserom.gba",|include "build/assets/text-tables-[^"\n]+" @ slot)\s*(0x[0-9A-Fa-f]+),\s*(0x[0-9A-Fa-f]+)')


def symbol(address):
    return f"gNonScriptText_{address:06X}"


def entries(rom):
    found, pointers = {}, {}

    def add(address, group, fixed=0):
        if not 0 <= address < len(rom):
            raise ValueError(f"{group}: string address outside ROM")
        end = rom.find(b"\0", address, address + (fixed or 8192))
        if end < 0:
            raise ValueError(f"{group}: unterminated string at {address:#x}")
        if address not in found:
            found[address] = {"address": address, "length": end - address + 1,
                              "groups": [], "fixed": fixed}
        found[address]["groups"].append(group)

    for group, table, count, stride, desc in catalog.ITEMS:
        for i in range(count):
            for field, kind in ((0, "name"), (desc, "description")):
                pointer = table + i * stride + field
                address = fa.word(rom, pointer)
                if address:
                    address -= fa.BASE
                    add(address, f"{group}/{i:03d}/{kind}")
                    pointers[pointer] = address
    for group, table, count in catalog.ARRAYS:
        for i in range(count):
            pointer = table + i * 4
            address = fa.word(rom, pointer) - fa.BASE
            add(address, f"{group}/{i:03d}")
            pointers[pointer] = address
    for group, table, count, stride in catalog.POINTERS:
        for i in range(count):
            pointer = table + i * stride
            address = fa.word(rom, pointer) - fa.BASE
            add(address, f"{group}/{i:03d}")
            pointers[pointer] = address
    for table, count in catalog.SHOPS:
        for i in range(count):
            for field in (4, 12):
                pointer = table + i * 20 + field
                address = fa.word(rom, pointer) - fa.BASE
                add(address, f"shops/{table:06X}/{i:03d}/{field}")
                pointers[pointer] = address
    for book in range(catalog.LIBRARY[1]):
        table = fa.word(rom, catalog.LIBRARY[0] + book * 4) - fa.BASE
        for line in range(2048):
            pointer = table + line * 4
            target = fa.word(rom, pointer)
            if not target:
                break
            address = target - fa.BASE
            add(address, f"library/{book:03d}/{line:03d}")
            pointers[pointer] = address
        else:
            raise ValueError(f"library {book}: missing line-table terminator")
    for address in catalog.ROOTS:
        add(address, "ui")
    for group, table, count, stride in catalog.FIXED + catalog.BYTE_ROWS:
        for i in range(count):
            add(table + i * stride, f"{group}/{i:03d}", stride)
    for group, table, count in catalog.GLYPHS:
        found[table] = {"address": table, "length": count * 2, "kind": "glyph16",
                        "groups": [f"{group}/glyphs"], "fixed": count * 2}
    result = sorted(found.values(), key=lambda entry: entry["address"])
    for a, b in zip(result, result[1:]):
        if a["address"] + a["length"] > b["address"]:
            raise ValueError(f"overlapping text roots {a['address']:#x}/{b['address']:#x}")
    return result, pointers


def source_value(rom, entry):
    address = entry["address"]
    if entry.get("kind") == "glyph16":
        result = bytearray()
        for pos in range(address, address + entry["length"], 2):
            code = int.from_bytes(rom[pos:pos + 2], "little")
            result.extend(code.to_bytes(1 if code < 256 else 2, "big"))
        return bytes(result)
    return rom[address:address + entry["length"] - 1]


def glyph_codes(value, single, double):
    result = []
    pos = 0
    while pos < len(value):
        code = value[pos]
        width = 1
        if single.get(code, -1) < 0 and code not in font.SPECIAL_CODES:
            width = 2
            if pos + 1 == len(value):
                raise ValueError("incomplete glyph code")
            code = (code << 8) | value[pos + 1]
            if double.get(code, -1) < 0:
                raise ValueError(f"unmapped glyph code {code:04X}")
        result.append(code)
        pos += width
    return result


def extract(rom, assets, force=False):
    records, _ = entries(rom)
    lines = ["# Non-script text; IDs/addresses and consumers are in catalog.json"]
    for i, entry in enumerate(records):
        address = entry["address"]
        lines.extend([f"=== {i}", *["|" + line for line in fa.decode(source_value(rom, entry)).split("\n")]])
    fa.write_missing(assets / "text_tables/strings.txt", ("\n".join(lines) + "\n").encode(), force)
    fa.write_missing(assets / "text_tables/catalog.json", (json.dumps(records, indent=2) + "\n").encode(), force)
    return records


def edited(rom, assets):
    records = extract(rom, assets)
    relocatable = set(entries(rom)[1].values())
    path = assets / "text_tables/strings.txt"
    values = fa.read_text(path, fa.charmap(assets / "charmap.txt"))
    if len(values) != len(records):
        raise ValueError(f"{path}: keep all {len(records)} IDs")
    single = font.read_map(assets / "fonts/single.map", "single")
    double = font.read_map(assets / "fonts/double.map", "double")
    for record, value in zip(records, values):
        if record.get("kind") == "glyph16":
            codes = glyph_codes(value, single, double)
            if any(code > 255 and not double[code] & 0x4000 for code in codes):
                raise ValueError(f"{path}: {record['address']:#x}: glyph cells require an 8px glyph")
            value = b"".join(code.to_bytes(2, "little") for code in codes)
            if len(value) != record["length"]:
                raise ValueError(f"{path}: glyph table {record['address']:#x}: preserve the glyph count")
        record["value"] = value.hex()
        if record["fixed"] and record.get("kind") != "glyph16":
            original = rom[record["address"]:record["address"] + record["length"] - 1]
            if len(value) != len(original):
                raise ValueError(f"{path}: {record['address']:#x}: name-entry rows must retain their byte width")
            if 0xE89B6 <= record["address"] < 0xE89B6 + 6 * 31 and value[1::2] != original[1::2]:
                raise ValueError(f"{path}: name-entry cell separators cannot change")
            full_width_grid = 0xE89B6 + 6 * 31 <= record["address"] < 0xE89B6 + 18 * 31
            if full_width_grid:
                for pos in range(0, len(value), 2):
                    codes = glyph_codes(value[pos:pos + 2], single, double)
                    if len(codes) != 1 or codes[0] <= 255 or double[codes[0]] & 0x4000:
                        raise ValueError(f"{path}: full-width name-entry cells need untagged double-byte glyphs")
            elif any(code > 255 for code in glyph_codes(value, single, double)):
                raise ValueError(f"{path}: {record['address']:#x}: byte-indexed text cannot contain double-byte glyphs")
        record["moved"] = len(value) + (record.get("kind") != "glyph16") > record["length"]
        if record["moved"] and record["address"] not in relocatable:
            raise ValueError(f"{path}: cannot grow text {record['address']:#x}: no audited symbolic pointer table; keep its original byte capacity")
    return records


def definition(record):
    name = symbol(record["address"])
    lines = [f".global {name}"]
    if record["moved"]:
        lines.extend(['.pushsection .text_table_overflow,"a",%progbits', '.balign 4'])
    lines.append(name + ":")
    if record["moved"]:
        lines.extend([".byte " + ",".join(str(b) for b in bytes.fromhex(record["value"]) + b"\0"), ".popsection"])
    return lines


def slot_bytes(rom, offset, size, records):
    result = bytearray(rom[offset:offset + size])
    for entry in records:
        if entry["moved"]:
            continue
        start = entry["address"]
        value = bytes.fromhex(entry["value"]) + (b"" if entry.get("kind") == "glyph16" else b"\0")
        lo, hi = max(offset, start), min(offset + size, start + len(value))
        if lo < hi:
            result[lo - offset:hi - offset] = value[lo - start:hi - start]
    return bytes(result)


def fragment(rom, offset, size, records, pointers, output):
    binary = output / f"text-tables-{offset:06X}.bin"
    fa.write_changed(binary, slot_bytes(rom, offset, size, records))
    roots = {r["address"]: r for r in records if offset <= r["address"] < offset + size}
    references = {p: address for p, address in pointers.items() if offset <= p < offset + size}
    events = sorted(set(roots) | set(references) | {offset, offset + size})
    lines, cursor = [], offset
    for pos in events:
        if pos < cursor:
            raise ValueError("text label overlaps a pointer word")
        if cursor < pos:
            lines.append(f'.incbin "{binary.as_posix()}", {cursor - offset}, {pos - cursor}')
        if pos in roots:
            lines.extend(definition(roots[pos]))
        if pos in references:
            lines.append(f".4byte {symbol(references[pos])}")
            cursor = pos + 4
        else:
            cursor = pos
    return ("\n".join(lines) + "\n").encode()


def blocks(repo):
    for path in sorted((repo / "asm/data").glob("*.s")):
        source = path.read_text()
        for match in SLOT.finditer(source):
            yield path, match, int(match[1], 16), int(match[2], 16)


def build(rom, assets, output, repo):
    records = edited(rom, assets)
    _, pointers = entries(rom)
    products = []
    for path, match, offset, size in blocks(repo):
        selected = any(offset < r["address"] + r["length"] and r["address"] < offset + size for r in records)
        selected |= any(offset <= p < offset + size for p in pointers if not ITEM_RODATA <= p < ITEM_END)
        if selected:
            if match[0].startswith('.incbin'):
                raise ValueError(f"{path}: text slot {offset:#x} needs a symbolic generated include")
            data = fragment(rom, offset, size, records, pointers, output)
            target = output / f"text-tables-{offset:06X}.s"
            fa.write_changed(target, data)
            products.append(data + (output / f"text-tables-{offset:06X}.bin").read_bytes())
    manifest = (json.dumps(records, sort_keys=True) + "\n").encode()
    fa.write_changed(output / "text-tables.json", manifest)
    fa.write_changed(output / "text-tables.stamp", hashlib.sha256(manifest + b"".join(products)).hexdigest().encode())
    return records


def elf_symbols(data):
    if data[:6] != b"\x7fELF\x01\x01":
        raise ValueError("expected 32-bit little-endian ELF")
    start = fa.word(data, 32)
    stride, count, names_id = struct.unpack_from("<HHH", data, 46)
    sections = [struct.unpack_from("<10I", data, start + i * stride) for i in range(count)]
    names = sections[names_id]
    strings = data[names[4]:names[4] + names[5]]
    section_names = [strings[s[0]:strings.index(b"\0", s[0])].decode() for s in sections]
    result = {}
    for section in sections:
        if section[1] != 2:
            continue
        strings_section = sections[section[6]]
        strings = data[strings_section[4]:strings_section[4] + strings_section[5]]
        for pos in range(section[4], section[4] + section[5], section[9]):
            name, value, _, _, _, index = struct.unpack_from("<IIIBBH", data, pos)
            if name and index < count:
                name = strings[name:strings.index(b"\0", name)].decode()
                result[name] = (section_names[index], value)
    return result


def patch_item(assembly, obj, manifest, rom):
    records = {r["address"]: r for r in json.loads(manifest.read_text()) if ITEM_RODATA <= r["address"] < ITEM_END}
    symbols = elf_symbols(obj.read_bytes())
    data = obj.read_bytes()
    start = fa.word(data, 32)
    stride, count, names_id = struct.unpack_from("<HHH", data, 46)
    sections = [struct.unpack_from("<10I", data, start + i * stride) for i in range(count)]
    names = sections[names_id]
    names = data[names[4]:names[4] + names[5]]
    rodata = next(s for s in sections if names[s[0]:names.index(b"\0", s[0])] == b".rodata")
    rodata = data[rodata[4]:rodata[4] + rodata[5]]
    source = assembly.read_text(encoding="latin1")
    seen = set()

    def replace(match):
        label = match[1]
        section, offset = symbols[label]
        address = ITEM_RODATA + offset
        if section != ".rodata" or address not in records:
            return match[0]
        record = records[address]
        if rodata[offset:offset + record["length"]] != rom[address:address + record["length"]]:
            raise ValueError(f"compiler item literal at {address:#x} no longer matches baserom")
        seen.add(address)
        binary = manifest.parent / f"text-item-{address:06X}.bin"
        fa.write_changed(binary, slot_bytes(rom, address, record["length"], [record]))
        lines = definition(record)
        lines.extend([f".set {label}, {symbol(address)}", f'.incbin "{binary.as_posix()}"'])
        return "\n".join(lines) + "\n"

    source = re.sub(r"(?m)^(\.LC\d+):\n((?:\s*\.ascii[^\n]*\n)+)", replace, source)
    if seen != set(records):
        raise ValueError(f"compiler literal layout changed; missing item strings: {sorted(set(records) - seen)}")
    assembly.write_text(source, encoding="latin1")


def verify(rom, built, elf, records):
    symbols = elf_symbols(elf)
    moved = 0
    for entry in records:
        address = entry["address"]
        target = symbols[symbol(address)][1] - fa.BASE
        expected = bytes.fromhex(entry["value"]) + (b"" if entry.get("kind") == "glyph16" else b"\0")
        if built[target:target + len(expected)] != expected:
            raise ValueError(f"text {address:#x}: linked bytes differ from editable source")
        if target == address:
            continue
        moved += 1
        for delta in range(entry["length"]):
            pointer = fa.pack(fa.BASE + address + delta)
            pos = rom.find(pointer)
            while pos >= 0:
                if built[pos:pos + 4] != fa.pack(fa.BASE + target + delta):
                    raise ValueError(f"cannot relocate text {address:#x}: non-symbolic reference at {pos:#x} -> {address + delta:#x}")
                pos = rom.find(pointer, pos + 1)
    print(f"Non-script text: {len(records)}/{len(records)} linked strings verified; {moved} relocated through symbolic references")


def audit(rom, records):
    maps = font.rom_maps(rom)
    script_codes, _ = font.script_usage(rom)
    _, candidates = font.reclaimable_slots(maps["double"], script_codes)
    reclaimed = set(candidates[:134])
    codes = set()
    for r in records:
        address = r["address"]
        codes.update(font.stream_codes(source_value(rom, r), maps["single"], maps["double"]))
    risky = sorted(code for code in codes if maps["double"].get(code, -1) in candidates)
    assigned = sorted(code for code in risky if maps["double"][code] in reclaimed)
    print(f"Non-script Vietnamese audit: {len(codes)} double-byte codes; {len(risky)} aliases of script-unused kanji slots; {len(assigned)} in the first 134 reclaimed slots")
    print("Kanji aliases: " + ", ".join(f"{code:04X}->glyph {maps['double'][code]}" for code in risky))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("extract", "build", "roundtrip", "patch-item", "verify"))
    parser.add_argument("--assets", type=Path, default=Path("assets"))
    parser.add_argument("--rom", type=Path, default=Path("baserom.gba"))
    parser.add_argument("--output", type=Path, default=Path("build/assets"))
    parser.add_argument("--assembly", type=Path)
    parser.add_argument("--object", type=Path)
    parser.add_argument("--built", type=Path, default=Path("fomt.gba"))
    parser.add_argument("--elf", type=Path, default=Path("fomt.elf"))
    parser.add_argument("--force", action="store_true")
    args = parser.parse_args()
    try:
        rom = font.original_rom(args.rom)
        if args.force and args.command != "extract":
            raise ValueError("--force is only valid for extract")
        if args.command == "extract":
            records = extract(rom, args.assets, args.force)
            print(f"Extracted {len(records)} non-script strings (existing edits preserved unless --force)")
        elif args.command == "build":
            build(rom, args.assets, args.output, Path.cwd())
        elif args.command == "patch-item":
            patch_item(args.assembly, args.object, args.output / "text-tables.json", rom)
        elif args.command == "verify":
            verify(rom, args.built.read_bytes(), args.elf.read_bytes(), edited(rom, args.assets))
        else:
            records = edited(rom, args.assets)
            for entry in records:
                address = entry["address"]
                length = entry["length"] - (entry.get("kind") != "glyph16")
                if bytes.fromhex(entry["value"]) != rom[address:address + length]:
                    raise ValueError(f"text {address:#x} is edited; roundtrip requires unmodified assets")
            print(f"Non-script text roundtrip: {len(records)}/{len(records)} strings byte-identical")
            audit(rom, records)
    except (ValueError, OSError, KeyError) as error:
        parser.exit(1, f"error: {error}\n")


if __name__ == "__main__":
    main()
