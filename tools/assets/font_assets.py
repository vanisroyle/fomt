"""Editable fixed-slot FOMT fonts and a minimal monochrome PNG codec."""

import argparse
import binascii
from collections import defaultdict
import hashlib
import math
from pathlib import Path
import re
import struct
import unicodedata
import zlib

import fomt_assets as fa


COLUMNS = 32
FONTS = {"small": (0x4F90CC, 487, 8), "large": (0x4FA9A0, 6922, 16)}
MAPS = {"single": (0x4FA7A0, 256, 487), "double": (0x523290, 7938, 6922)}
SIGNATURE = b"\x89PNG\r\n\x1a\n"
SPECIAL_CODES = frozenset((0xB1, 0xB2, 0xB3, 0xB4, 0xB6, 0xB7, *range(0xBB, 0xC4)))
MAP_ROW = re.compile(r"([0-9a-fA-F]{2,4})\s*=\s*(-?\d+)")
ACCENTS = {
    "\u0301": (".#", "#."),
    "\u0300": ("#.", ".#"),
    "\u0309": (".##", "..#", ".#."),
    "\u0303": (".#.#", "#.#."),
    "\u0323": ("#",),
    "\u0302": (".#.", "#.#"),
    "\u0306": ("#.#", ".#."),
    "\u031b": ("..#", "..#", "##."),
}


def png_chunk(tag, data):
    return struct.pack(">I", len(data)) + tag + data + struct.pack(">I", binascii.crc32(tag + data) & 0xFFFFFFFF)


def write_png(rows):
    if not rows or not rows[0] or any(len(row) != len(rows[0]) for row in rows):
        raise ValueError("PNG must have nonempty equal-width rows")
    width = len(rows[0])
    raw = bytearray()
    for row in rows:
        packed = bytearray((width + 7) // 8)
        for x, pixel in enumerate(row):
            if pixel not in (0, 1):
                raise ValueError("PNG pixels must be 0 (white) or 1 (black)")
            packed[x // 8] |= pixel << (7 - x % 8)
        raw.extend(b"\0" + packed)
    header = struct.pack(">IIBBBBB", width, len(rows), 1, 3, 0, 0, 0)
    return (SIGNATURE + png_chunk(b"IHDR", header) + png_chunk(b"PLTE", b"\xff\xff\xff\0\0\0")
            + png_chunk(b"IDAT", zlib.compress(raw)) + png_chunk(b"IEND", b""))


def paeth(a, b, c):
    p = a + b - c
    distances = (abs(p - a), abs(p - b), abs(p - c))
    return (a, b, c)[distances.index(min(distances))]


def read_png(data):
    if not data.startswith(SIGNATURE):
        raise ValueError("not a PNG")
    pos = len(SIGNATURE)
    header = None
    palette = None
    compressed = bytearray()
    ended = False
    while pos < len(data):
        if pos + 12 > len(data):
            raise ValueError("truncated PNG chunk")
        size = struct.unpack_from(">I", data, pos)[0]
        tag = data[pos + 4:pos + 8]
        payload = data[pos + 8:pos + 8 + size]
        if pos + size + 12 > len(data):
            raise ValueError("truncated PNG payload")
        crc = struct.unpack_from(">I", data, pos + 8 + size)[0]
        if binascii.crc32(tag + payload) & 0xFFFFFFFF != crc:
            raise ValueError("PNG CRC mismatch")
        if tag == b"IHDR":
            if header is not None or size != 13 or pos != 8:
                raise ValueError("invalid PNG IHDR")
            header = struct.unpack(">IIBBBBB", payload)
        elif tag == b"PLTE":
            if not size or size % 3:
                raise ValueError("invalid PNG palette")
            palette = [tuple(payload[i:i + 3]) for i in range(0, size, 3)]
        elif tag == b"IDAT":
            compressed.extend(payload)
        elif tag == b"IEND":
            ended = True
            pos += 12 + size
            break
        elif tag == b"tRNS":
            raise ValueError("transparent PNGs are unsupported; flatten to black/white")
        elif not tag[0] & 32:
            raise ValueError(f"unsupported critical PNG chunk {tag!r}")
        pos += 12 + size
    if header is None or not ended or pos != len(data):
        raise ValueError("incomplete PNG or trailing data")
    width, height, depth, color, compression, filtering, interlace = header
    if (not 0 < width <= 4096 or not 0 < height <= 8192 or color not in (0, 3)
            or depth not in (1, 2, 4, 8) or compression or filtering or interlace):
        raise ValueError("use noninterlaced grayscale/indexed 1/2/4/8-bit PNG (max 4096x8192)")
    if color == 3 and palette is None:
        raise ValueError("indexed PNG needs a palette")
    stride = (width * depth + 7) // 8
    expected = height * (stride + 1)
    inflater = zlib.decompressobj()
    try:
        raw = inflater.decompress(compressed, expected + 1)
    except zlib.error as error:
        raise ValueError(f"invalid PNG compression: {error}") from error
    if len(raw) != expected or not inflater.eof or inflater.unused_data:
        raise ValueError("PNG decompressed size does not match dimensions")
    rows = []
    previous = bytearray(stride)
    mask = (1 << depth) - 1
    for y in range(height):
        start = y * (stride + 1)
        filter_type = raw[start]
        if filter_type > 4:
            raise ValueError("invalid PNG filter")
        scan = bytearray(raw[start + 1:start + 1 + stride])
        for i in range(stride):
            a = scan[i - 1] if i else 0
            b = previous[i]
            c = previous[i - 1] if i else 0
            predictor = (0, a, b, (a + b) // 2, paeth(a, b, c))[filter_type]
            scan[i] = (scan[i] + predictor) & 255
        row = bytearray()
        for x in range(width):
            sample = (scan[x * depth // 8] >> (8 - depth - x * depth % 8)) & mask
            if color == 0:
                rgb = (sample * 255 // mask,) * 3
            else:
                if sample >= len(palette):
                    raise ValueError("PNG palette index out of range")
                rgb = palette[sample]
            if rgb not in ((0, 0, 0), (255, 255, 255)):
                raise ValueError("PNG must use only opaque black/white pixels (no antialiasing)")
            row.append(int(rgb == (0, 0, 0)))
        rows.append(row)
        previous = scan
    return rows


def to_sheet(data, count, width):
    stride = width // 8
    if len(data) != count * 12 * stride:
        raise ValueError("font binary size does not match fixed slot")
    rows = [bytearray(COLUMNS * width) for _ in range(math.ceil(count / COLUMNS) * 12)]
    for glyph in range(count):
        for y in range(12):
            for x in range(width):
                byte = data[glyph * 12 * stride + y * stride + x // 8]
                rows[glyph // COLUMNS * 12 + y][glyph % COLUMNS * width + x] = (byte >> (7 - x % 8)) & 1
    return rows


def from_sheet(rows, count, width):
    height = math.ceil(count / COLUMNS) * 12
    if len(rows) != height or any(len(row) != COLUMNS * width for row in rows):
        raise ValueError(f"font sheet must be exactly {COLUMNS * width}x{height}; slot cannot grow")
    output = bytearray(count * 12 * (width // 8))
    for glyph in range(COLUMNS * math.ceil(count / COLUMNS)):
        for y in range(12):
            for x in range(width):
                pixel = rows[glyph // COLUMNS * 12 + y][glyph % COLUMNS * width + x]
                if pixel not in (0, 1):
                    raise ValueError("nonbinary font pixel")
                if glyph >= count:
                    if pixel:
                        raise ValueError("unused sheet cells must stay white; no additional glyph slots")
                else:
                    output[glyph * 12 * (width // 8) + y * (width // 8) + x // 8] |= pixel << (7 - x % 8)
    return bytes(output)


def codes(kind):
    if kind == "single":
        return list(range(256))
    return [(lead << 8) | trail for lead in [*range(0x81, 0xA0), *range(0xE0, 0xEB)]
            for trail in range(0x40, 0xFD)]


def map_text(kind, values):
    digits = 2 if kind == "single" else 4
    return "# Game code (hex) = glyph index (decimal), -1 means unmapped\n" + "".join(
        f"{code:0{digits}X} = {value}\n" for code, value in zip(codes(kind), values))


def read_map(path, kind):
    values = {}
    allowed = set(codes(kind))
    for line, text in enumerate(path.read_text(encoding="utf-8-sig").splitlines(), 1):
        text = text.split("#", 1)[0].strip()
        if not text:
            continue
        match = MAP_ROW.fullmatch(text)
        if not match:
            fa.fail(path, line, "expected HEX_CODE = DECIMAL_GLYPH_INDEX")
        code, index = int(match[1], 16), int(match[2])
        if code not in allowed or code in values:
            fa.fail(path, line, "invalid or duplicate game code")
        tagged = kind == "double" and 0x4000 <= index < 0x4000 + 487
        if not tagged and not -1 <= index < MAPS[kind][2]:
            fa.fail(path, line, f"glyph index must be -1..{MAPS[kind][2] - 1}; fixed slot cannot grow")
        values[code] = index
    if set(values) != allowed:
        raise ValueError(f"{path}: preserve all {len(allowed)} code rows, including -1 entries")
    return values


def assign_map_row(source, code, slot):
    rows = source.splitlines(keepends=True)
    matches = 0
    for i, row in enumerate(rows):
        text = (row[1:] if i == 0 and row.startswith("\ufeff") else row).split("#", 1)[0].strip()
        match = MAP_ROW.fullmatch(text)
        if match and int(match[1], 16) == code and int(match[2]) == -1:
            start = row.index(text) + match.start(2)
            rows[i] = row[:start] + str(slot) + row[start + len(match[2]):]
            matches += 1
    if matches != 1:
        raise ValueError("cannot update map row safely")
    return "".join(rows)


def original_rom(path):
    rom = path.read_bytes()
    if hashlib.sha1(rom).hexdigest() != fa.SHA1:
        raise ValueError(f"{path}: expected original US ROM (SHA-1 {fa.SHA1})")
    return rom


def extract(rom_path, assets, force=False):
    rom = original_rom(rom_path)
    root = assets / "fonts"
    for name, (offset, count, width) in FONTS.items():
        path = root / (name + ".png")
        if force or not path.exists():
            data = rom[offset:offset + count * 12 * (width // 8)]
            fa.write_missing(path, write_png(to_sheet(data, count, width)), force)
    for kind, (offset, count, _) in MAPS.items():
        values = struct.unpack_from(f"<{count}h", rom, offset)
        fa.write_missing(root / (kind + ".map"), map_text(kind, values).encode(), force)


def build_data(assets):
    root = assets / "fonts"
    result = {}
    for name, (_, count, width) in FONTS.items():
        path = root / (name + ".png")
        try:
            result[name] = from_sheet(read_png(path.read_bytes()), count, width)
        except ValueError as error:
            raise ValueError(f"{path}: {error}") from error
    for kind in MAPS:
        values = read_map(root / (kind + ".map"), kind)
        result[kind] = struct.pack(
            f"<{len(values)}h", *(values[code] for code in codes(kind)))
    return result


def build(assets, output, halfwidth=False):
    data = build_data(assets)
    if not halfwidth and any(value >= 0x4000 for value in struct.unpack("<7938h", data["double"])):
        raise ValueError("tagged small glyphs require make VIETNAMESE_FONT=1")
    fa.write_changed(output / "vietnamese-mode.inc", f".equ VIETNAMESE_FONT, {int(halfwidth)}\n".encode())
    for name, raw in data.items():
        fa.write_changed(output / ("font-" + name + ".bin"), raw)
    digest = hashlib.sha256(b"".join(data.values()) + bytes([halfwidth])).hexdigest()
    fa.write_changed(output / "fonts.stamp", (digest + "\n").encode())
    print("Built fonts: 487 small / 6922 large glyphs; fixed slots unchanged")


def safe_codes(single):
    return [code for code, index in single.items() if index == -1 and code > 0x1F
            and code != 0xFF and code not in SPECIAL_CODES
            and not 0x81 <= code <= 0x9F and not 0xE0 <= code <= 0xEA]


def vietnamese_letters():
    letters = set("ĂÂĐÊÔƠƯăâđêôơư")
    for base in "AaĂăÂâEeÊêIiOoÔôƠơUuƯưYy":
        for tone in "\u0300\u0301\u0303\u0309\u0323":
            letters.add(unicodedata.normalize("NFC", base + tone))
    return sorted(letters)


def vietnamese_glyph(char, small, single, width=16):
    if width not in (8, 16):
        raise ValueError("Vietnamese glyph width must be 8 or 16")
    if char not in vietnamese_letters():
        raise ValueError(f"not a Vietnamese precomposed letter: {char!r}")
    parts = unicodedata.normalize("NFD", char)
    base = parts[0]
    if base in "Đđ":
        base = "D" if base == "Đ" else "d"
    index = single.get(ord(base), -1)
    if index < 0:
        raise ValueError(f"missing base glyph for {base!r}")
    source = list(small[index * 12:index * 12 + 12])
    if len(source) != 12 or not any(source):
        raise ValueError(f"empty or truncated base glyph for {base!r}")
    shape = next((mark for mark in parts[1:] if mark in "\u0302\u0306\u031b"), None)
    tone = next((mark for mark in parts[1:] if mark in "\u0300\u0301\u0309\u0303\u0323"), None)
    above = shape is not None or tone not in (None, "\u0323")
    if base == "i" and above:
        for y in range(3):
            source[y] = 0
    if char in "Đđ":
        source[4 if char == "Đ" else 2] |= 0xE0 if char == "Đ" else 0x1E
    body = [(y, row) for y, row in enumerate(source) if row]
    top, bottom = body[0][0], body[-1][0]
    target_top = (3 if base.isupper() else 4) if tone == "\u0309" else 3 if above else top
    if base.isupper() and above and tone == "\u0323":
        target_top = 2
    height = min(bottom - top + 1, 8 if base.isupper() and above else 12)
    height = min(height, (10 if tone == "\u0323" else 12) - target_top)
    rows = [0] * 12
    for y in range(top, bottom + 1):
        if y == top:
            target = target_top
        elif y == bottom:
            target = target_top + height - 1
        else:
            target = target_top + 1 + (y - top - 1) * (height - 2) // max(1, bottom - top - 1)
        rows[target] |= source[y] << 8

    def stamp(mark, x, y):
        for dy, pattern in enumerate(ACCENTS[mark]):
            for dx, pixel in enumerate(pattern):
                if pixel == "#":
                    rows[y + dy] |= 1 << (15 - x - dx)

    stacked = shape in ("\u0302", "\u0306") and tone not in (None, "\u0323")
    if shape == "\u031b":
        stamp(shape, 5, target_top - 2)
    elif shape:
        stamp(shape, 0 if stacked else 2, 0)
    if tone == "\u0323":
        stamp(tone, 3, 11)
    elif tone:
        stamp(tone, 4 if stacked else 5 if tone == "\u0309" and base.isupper() else 2, 0)
    glyph = b"".join(row.to_bytes(2, "big") for row in rows)
    return glyph[::2] if width == 8 else glyph


def vietnamese_sheet(rom_path, output):
    rom = original_rom(rom_path)
    single = rom_maps(rom)["single"]
    small = rom[0x4F90CC:0x4FA7A0]
    letters = vietnamese_letters()
    rows = [bytearray(8 * 40) for _ in range(math.ceil(len(letters) / 8) * 28)]
    for i, char in enumerate(letters):
        x, y = i % 8 * 40, i // 8 * 28
        glyph = vietnamese_glyph(char, small, single)
        for dy in range(12):
            value = int.from_bytes(glyph[dy * 2:dy * 2 + 2], "big")
            for dx in range(16):
                rows[y + dy][x + 12 + dx] = (value >> (15 - dx)) & 1
        for j, digit in enumerate(f"{ord(char):04X}"):
            start = single[ord(digit)] * 12
            for dy, value in enumerate(small[start:start + 12]):
                for dx in range(8):
                    rows[y + 14 + dy][x + 4 + j * 8 + dx] = (value >> (7 - dx)) & 1
    zoomed = [bytearray(pixel for pixel in row for _ in range(4)) for row in rows for _ in range(4)]
    fa.write_changed(output, write_png(zoomed))
    print(f"Vietnamese contact sheet: {len(letters)} glyphs, 4x, Unicode hex labels -> {output}")


def glyph_for(raw, single, double):
    code = int.from_bytes(raw, "big")
    return single.get(code, -1) if len(raw) == 1 else double.get(code, -1) if len(raw) == 2 else -1


def rom_maps(rom):
    return {kind: dict(zip(codes(kind), struct.unpack_from(f"<{count}h", rom, offset)))
            for kind, (offset, count, _) in MAPS.items()}


def stream_codes(data, single, double):
    """Track the renderer's accumulated code, excluding FF substitutions."""
    used = set()
    pending = 0
    for byte in data:
        if byte <= 0x1F:
            continue
        pending |= byte
        if pending <= 0xFF:
            # These special single-byte codes bypass the ordinary font table.
            if single.get(pending, -1) >= 0 or pending in SPECIAL_CODES:
                pending = 0
                continue
        elif pending in double:
            # Record attempts even if unmapped: do not allocate an in-use code.
            used.add(pending)
            if double[pending] >= 0:
                pending = 0
                continue
        elif 0xFF00 <= pending <= 0xFFFF:
            pending = 0
            continue
        pending = (pending << 8) & 0xFFFFFFFF
    return used


def script_usage(rom, assets=None, single=None, double=None):
    original = rom_maps(rom)
    used = set()
    count = 0
    for _, _, data in fa.rom_scripts(rom):
        for tag, payload in fa.chunks(data):
            if tag == b"STR ":
                for entry in fa.strings(payload):
                    used.update(stream_codes(entry, original["single"], original["double"]))
                    count += 1
    if assets is not None:
        mapping = fa.charmap(assets / "charmap.txt")
        for script_id in range(1, fa.COUNT + 1):
            path = assets / "text" / f"{script_id:04d}.txt"
            for entry in fa.read_text(path, mapping):
                used.update(stream_codes(entry, single, double))
    return used, count


def kanji_code(code):
    return 0x889F <= code <= 0x9FFC or 0xE040 <= code <= 0xEAFC


def reclaimable_slots(double, used, original=None):
    aliases = defaultdict(set)
    for table in (double, original or {}):
        for code, slot in table.items():
            if slot >= 0:
                aliases[slot].add(code)
    unused = [slot for slot in range(6922) if not aliases[slot] & used]
    kanji = [slot for slot in unused if aliases[slot] and all(kanji_code(code) for code in aliases[slot])]
    return unused, kanji


def accepted_free_codes(single, double):
    return sorted((code for code, slot in double.items() if slot == -1 and single[code >> 8] == -1),
                  key=lambda code: (not kanji_code(code), (code & 0xFF) == 0x7F, code))


def capacity(assets, rom_path):
    single = read_map(assets / "fonts/single.map", "single")
    double = read_map(assets / "fonts/double.map", "double")
    missing = []
    for char in vietnamese_letters():
        try:
            index = glyph_for(char.encode("cp932"), single, double)
        except UnicodeEncodeError:
            index = -1
        if index < 0:
            missing.append(char)
    unused = sorted(set(range(487)) - set(single.values()))
    print(f"Unmapped single-byte table entries: {list(single.values()).count(-1)}")
    print(f"Safe unused single-byte text codes: {len(safe_codes(single))} "
          + " ".join(f"{c:02X}" for c in safe_codes(single)))
    print(f"Unreferenced small-font slots: {len(unused)}")
    print(f"Vietnamese non-ASCII precomposed letters (both cases): {len(vietnamese_letters())}; "
          f"missing from CP932/current maps: {len(missing)}")
    print("Full Vietnamese set fits single-byte allocation: " + ("yes" if len(missing) <= min(len(unused), len(safe_codes(single))) else "no"))
    rom = original_rom(rom_path)
    used, count = script_usage(rom)
    original = rom_maps(rom)
    candidates, kanji = reclaimable_slots(original["double"], used)
    print(f"Original scripts: {fa.COUNT} scripts / {count} strings; {len(used)} distinct double-byte codes")
    print(f"Original large slots with every alias unused in scripts: {len(candidates)}; kanji-only: {len(kanji)}")
    print(f"Original accepted unmapped double-byte codes: {len(accepted_free_codes(original['single'], original['double']))}")
    print("WARNING: script-only reclamation does not audit non-script or dynamically substituted text")


def sample(rom_path, assets, text, placeholders=False, reclaim_large=False, halfwidth=False):
    if halfwidth:
        reclaim_large = True
    text = unicodedata.normalize("NFC", text)
    root = assets / "fonts"
    single = read_map(root / "single.map", "single")
    double = read_map(root / "double.map", "double")
    tagged_slots = {v & 0x3FFF for v in double.values() if v >= 0x4000}
    fa.write_missing(assets / "charmap.txt", b"# Unicode overrides\n")
    mapping = fa.charmap(assets / "charmap.txt")
    missing = []
    for char in dict.fromkeys(text):
        if char.isspace():
            continue
        try:
            raw = mapping.get(char) or char.encode("cp932")
        except UnicodeEncodeError:
            raw = b""
        if glyph_for(raw, single, double) < 0:
            if char in mapping:
                raise ValueError(f"existing charmap assignment for {char!r} is unmapped; fix it first")
            missing.append(char)
        elif halfwidth and char in mapping and double.get(int.from_bytes(raw, "big"), -1) < 0x4000:
            raise ValueError(f"existing assignment for {char!r} is not tagged; restore or migrate it before --halfwidth")
    reserved = {int.from_bytes(raw, "big") for raw in mapping.values()}
    for raw in mapping.values():
        reserved.update(stream_codes(raw, single, double))
    name, kind = ("large", "double") if reclaim_large else ("small", "single")
    if halfwidth:
        name = "small"
    offset, count, width = FONTS[name]
    size = 12 * (width // 8)
    data = from_sheet(read_png((root / (name + ".png")).read_bytes()), count, width)
    rom = original_rom(rom_path)
    original = rom[offset:offset + len(data)]
    if reclaim_large:
        used, _ = script_usage(rom, assets, single, double)
        used.update(reserved)
        if halfwidth:
            occupied = set(single.values()) | set(rom_maps(rom)["single"].values()) | tagged_slots
            candidates = [i for i in range(487) if i not in occupied]
        else:
            _, candidates = reclaimable_slots(double, used, rom_maps(rom)["double"])
        available = [code for code in accepted_free_codes(single, double) if code not in used]
        print("Allocating tagged small-font slots" if halfwidth else "WARNING: reclaiming script-unused kanji; non-script/substituted text is NOT audited")
    else:
        occupied = set(single.values()) | tagged_slots
        candidates = [i for i in range(count) if i not in occupied]
        available = [code for code in safe_codes(single) if code not in reserved]
    # Unmapped cells with hand-drawn edits are reserved for their author.
    slots = [i for i in candidates if data[i * size:i * size + size] == original[i * size:i * size + size]]
    if len(missing) > min(len(available), len(slots)):
        raise ValueError(f"need {len(missing)} codes/slots; only {len(available)} safe codes and {len(slots)} untouched free slots; no edits made")
    assignments = list(zip(missing, available, slots))
    map_path = root / (kind + ".map")
    map_source = map_path.read_bytes().decode("utf-8")
    additions = []
    pixels = bytearray(data)
    real_letters = set(vietnamese_letters()) if reclaim_large else set()
    draw = placeholders or any(char in real_letters for char in missing)
    small = from_sheet(read_png((root / "small.png").read_bytes()), 487, 8) if reclaim_large and draw else data
    for char, code, slot in assignments:
        map_source = assign_map_row(map_source, code, slot | 0x4000 if halfwidth else slot)
        raw = code.to_bytes(2 if reclaim_large else 1, "big")
        additions.append(f"U+{ord(char):04X} = {raw.hex(' ').upper()}\n")
        if char in real_letters:
            pixels[slot * size:slot * size + size] = vietnamese_glyph(char, small, single, width)
        elif placeholders:
            base = unicodedata.normalize("NFD", char)[0]
            if base in "Đđ":
                base = "D" if base == "Đ" else "d"
            index = single.get(ord(base), -1) if ord(base) < 128 else -1
            glyph = bytearray(small[index * 12:index * 12 + 12] if index >= 0 else bytes(12))
            glyph[0] |= 0x18
            glyph[1] |= 0x08
            if reclaim_large and not halfwidth:
                glyph = b"".join((row << 4).to_bytes(2, "big") for row in glyph)
            pixels[slot * size:slot * size + size] = glyph
        print(f"Assign {char} U+{ord(char):04X} -> {raw.hex(' ').upper()}, {name} glyph {slot}")
    if not assignments:
        print("All characters already have mapped glyphs; no files changed")
        return
    old_charmap = (assets / "charmap.txt").read_text(encoding="utf-8")
    fa.write_changed(map_path, map_source.encode("utf-8"))
    separator = "" if old_charmap.endswith("\n") else "\n"
    fa.write_changed(assets / "charmap.txt", (old_charmap + separator + "".join(additions)).encode("utf-8"))
    if draw:
        fa.write_changed(root / (name + ".png"), write_png(to_sheet(bytes(pixels), count, width)))
    print(f"Added {len(assignments)} {kind}-byte assignments. "
          + ("Vietnamese glyphs composed from base letters and accent patterns." if real_letters and draw
             else "Placeholder accents need artist review." if placeholders else f"Draw the assigned cells in {name}.png."))


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("extract", "build", "roundtrip", "capacity", "sample", "vietnamese-sheet"))
    parser.add_argument("--rom", type=Path, default=Path("baserom.gba"))
    parser.add_argument("--assets", type=Path, default=Path("assets"))
    parser.add_argument("--output", type=Path, default=Path("build/assets"))
    parser.add_argument("--force", action="store_true")
    parser.add_argument("--text", default="Xin chào Việt Nam!")
    parser.add_argument("--placeholders", action="store_true")
    parser.add_argument("--reclaim-large", action="store_true", help="opt in to script-only kanji reclamation; full-width output")
    parser.add_argument("--halfwidth", action="store_true", help="allocate tagged 8px Vietnamese glyphs; build with VIETNAMESE_FONT=1")
    args = parser.parse_args()
    try:
        if args.force and args.command != "extract":
            raise ValueError("--force is only valid with extract")
        if args.command in ("extract", "build", "sample"):
            extract(args.rom, args.assets, args.force)
        if args.command == "extract":
            print("Extracted fonts (existing edits preserved)" if not args.force else "Re-extracted fonts (overwritten)")
        elif args.command == "build":
            build(args.assets, args.output, args.halfwidth)
        elif args.command == "capacity":
            capacity(args.assets, args.rom)
        elif args.command == "sample":
            sample(args.rom, args.assets, args.text, args.placeholders, args.reclaim_large, args.halfwidth)
        elif args.command == "vietnamese-sheet":
            vietnamese_sheet(args.rom, args.output)
        else:
            rom = original_rom(args.rom)
            data = build_data(args.assets)
            for name, raw in data.items():
                offset = (FONTS[name] if name in FONTS else MAPS[name])[0]
                if raw != rom[offset:offset + len(raw)]:
                    raise ValueError(f"{name}: font round-trip mismatch")
            print("Font roundtrip: 487/487 small + 6922/6922 large glyphs and both maps byte-identical (188360 bytes)")
    except (OSError, ValueError) as error:
        parser.exit(1, f"error: {error}\n")


if __name__ == "__main__":
    main()
