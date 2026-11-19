"""Lossless FOMT script assets. Python 3 standard library only."""

import argparse
import hashlib
import json
from pathlib import Path
import re
import struct
import sys

import event_scripts as events


COUNT = 1328
BASE = 0x08000000
TABLE = 0xF89D4
START = 0x2AC0B0
END = 0x4F90CC
SHA1 = "a2fc3574f0a65a4fcf7682fb274b9d7eebdef963"


def word(data, offset=0):
    if offset < 0 or offset + 4 > len(data):
        raise ValueError("truncated word")
    return struct.unpack_from("<I", data, offset)[0]


def pack(value):
    return struct.pack("<I", value)


def chunks(data):
    if len(data) < 12 or data[:4] != b"RIFF" or data[8:12] != b"SCR ":
        raise ValueError("invalid RIFF/SCR header")
    if word(data, 4) != len(data):
        raise ValueError("script length does not match container")
    result = []
    pos = 12
    while pos < len(data):
        if pos + 8 > len(data):
            raise ValueError("truncated chunk header")
        size = word(data, pos + 4)
        if pos + 8 + size > len(data):
            raise ValueError("truncated chunk payload")
        result.append((data[pos:pos + 4], data[pos + 8:pos + 8 + size]))
        pos += 8 + size
    for tag in (b"CODE", b"STR "):
        if sum(t == tag for t, _ in result) != 1:
            raise ValueError("expected exactly one " + repr(tag))
    return result


def strings(payload):
    count = word(payload)
    pool = 4 + count * 4
    if pool > len(payload):
        raise ValueError("truncated STR offset table")
    result = []
    for i in range(count):
        start = pool + word(payload, 4 + i * 4)
        end = payload.find(b"\0", start)
        if start >= len(payload) or end < 0:
            raise ValueError("unterminated STR string")
        result.append(payload[start:end])
    return result


def decode(data):
    result = []
    pos = 0
    while pos < len(data):
        byte = data[pos]
        if data[pos:pos + 2] == b"\r\n":
            result.append("\n")
            pos += 2
            continue
        if byte == 0xFF and pos + 1 < len(data):
            result.append("{FF%02X}" % data[pos + 1])
            pos += 2
            continue
        if byte in (5, 12):
            result.append("{W}" if byte == 5 else "{CLEAR}")
        elif byte < 32 or byte in (0x7F, 0x7B, 0x7D):
            result.append("{%02X}" % byte)
        else:
            found = False
            for size in (1, 2):
                raw = data[pos:pos + size]
                try:
                    char = raw.decode("cp932")
                    if len(raw) == size and char.encode("cp932") == raw:
                        result.append(char)
                        pos += size
                        found = True
                        break
                except UnicodeError:
                    pass
            if found:
                continue
            result.append("{%02X}" % byte)
        pos += 1
    return "".join(result)


def fail(path, line, message):
    raise ValueError(f"{path}:{line}: {message}")


def charmap(path):
    mapping = {}
    for line, text in enumerate(path.read_text(encoding="utf-8-sig").splitlines(), 1):
        text = text.split("#", 1)[0].strip()
        if not text:
            continue
        match = re.fullmatch(r"U\+([0-9A-Fa-f]{4,6})\s*=\s*((?:[0-9A-Fa-f]{2}\s*)+)", text)
        if not match:
            fail(path, line, "expected U+XXXX = XX [XX ...]")
        try:
            char = chr(int(match[1], 16))
        except ValueError:
            fail(path, line, "invalid Unicode code point")
        raw = bytes.fromhex(match[2])
        if char in mapping or "\0" == char or b"\0" in raw:
            fail(path, line, "duplicate mapping or forbidden NUL")
        mapping[char] = raw
    return mapping


def encode(text, mapping, path, line):
    result = bytearray()
    pos = 0
    while pos < len(text):
        char = text[pos]
        if char == "{":
            end = text.find("}", pos)
            token = text[pos + 1:end] if end >= 0 else ""
            if token in ("W", "CLEAR"):
                raw = b"\x05" if token == "W" else b"\x0c"
            elif re.fullmatch(r"(?:[Ff]{2})?[0-9A-Fa-f]{2}", token):
                raw = bytes.fromhex(token)
            else:
                fail(path, line, "invalid token; use {7B}/{7D} for literal braces")
            pos = end + 1
        else:
            if char == "}" or ord(char) < 32:
                fail(path, line, "use a hex token for braces/control characters")
            try:
                raw = mapping[char] if char in mapping else char.encode("cp932")
            except UnicodeEncodeError:
                fail(path, line, f"cannot encode {char!r} (U+{ord(char):04X}); add a charmap mapping")
            pos += 1
        if b"\0" in raw:
            fail(path, line, "NUL is a string terminator, not editable content")
        result.extend(raw)
    return bytes(result)


def text_file(script_id, address, entries):
    lines = [f"# Script {script_id}; original ROM address 0x{address:08X}"]
    for i, entry in enumerate(entries):
        lines.append(f"=== {i}")
        lines.extend("|" + line for line in decode(entry).split("\n"))
    return "\n".join(lines) + "\n"


def read_text(path, mapping):
    entries = []
    current = None
    # split on LF only: do not treat Unicode separators as game newlines.
    text = path.read_text(encoding="utf-8-sig")
    lines = text.split("\n")
    if lines[-1] == "":
        lines.pop()
    for line, value in enumerate(lines, 1):
        if value.startswith("#") and current is None:
            continue
        if value == f"=== {len(entries)}":
            if current == []:
                fail(path, line, "entry needs at least one | line (| alone is empty)")
            current = []
            entries.append(current)
        elif value.startswith("|") and current is not None:
            current.append(encode(value[1:], mapping, path, line))
        else:
            fail(path, line, "expected sequential === ID or |content line")
    if current == []:
        fail(path, len(lines), "entry needs at least one | line")
    return [b"\r\n".join(entry) for entry in entries]


def write_missing(path, data, force=False):
    path.parent.mkdir(parents=True, exist_ok=True)
    if path.exists() and not force:
        return
    temporary = path.with_suffix(path.suffix + ".tmp")
    temporary.write_bytes(data)
    temporary.replace(path)


def write_changed(path, data):
    path.parent.mkdir(parents=True, exist_ok=True)
    if not path.exists() or path.read_bytes() != data:
        temporary = path.with_suffix(path.suffix + ".tmp")
        temporary.write_bytes(data)
        temporary.replace(path)


def rom_scripts(rom):
    if word(rom, TABLE) != 0:
        raise ValueError("script ID 0 must be NULL")
    for i in range(1, COUNT + 1):
        start = word(rom, TABLE + i * 4) - BASE
        size = word(rom, start + 4)
        if size < 12 or start < 0 or start + size > len(rom):
            raise ValueError(f"invalid script {i} pointer/size")
        data = rom[start:start + size]
        chunks(data)
        yield i, start, data


def event_string_count(metadata):
    payloads = [c["payload"] for c in metadata["chunks"] if bytes.fromhex(c["tag"]) == b"STR "]
    if len(payloads) != 1:
        raise ValueError("event metadata requires exactly one STR chunk")
    return len(strings(bytes.fromhex(payloads[0])))


def extract(rom_path, assets, force=False):
    rom = rom_path.read_bytes()
    if hashlib.sha1(rom).hexdigest() != SHA1:
        raise ValueError(f"{rom_path}: expected the original US ROM (SHA-1 {SHA1})")
    write_missing(assets / "charmap.txt", b"# Optional Unicode overrides, e.g. U+1EA1 = 81 40\n", force)
    for i, start, data in rom_scripts(rom):
        name = f"{i:04d}"
        metadata = {"version": 1, "id": i, "address": BASE + start, "chunks": []}
        for tag, payload in chunks(data):
            item = {"tag": tag.hex()}
            if tag == b"CODE":
                size = word(payload)
                if size > len(payload) - 4:
                    raise ValueError(f"script {i}: truncated CODE")
                write_missing(assets / "scripts" / (name + ".code.bin"), payload[4:4 + size], force)
                item["tail"] = payload[4 + size:].hex()
            elif tag == b"JUMP":
                write_missing(assets / "scripts" / (name + ".jump.bin"), payload, force)
            else:
                item["payload"] = payload.hex()
                if tag == b"STR ":
                    output = text_file(i, BASE + start, strings(payload))
                    write_missing(assets / "text" / (name + ".txt"), output.encode("utf-8"), force)
            metadata["chunks"].append(item)
        write_missing(assets / "scripts" / (name + ".json"),
                      (json.dumps(metadata, indent=2) + "\n").encode(), force)
        event_path = assets / "scripts" / (name + ".evt")
        if force or not event_path.exists():
            current = json.loads((assets / "scripts" / (name + ".json")).read_text(encoding="utf-8"))
            string_count = event_string_count(current)
            code = (assets / "scripts" / (name + ".code.bin")).read_bytes()
            jump = (assets / "scripts" / (name + ".jump.bin")).read_bytes() if any(bytes.fromhex(c["tag"]) == b"JUMP" for c in current["chunks"]) else None
            write_missing(event_path, events.disassemble(code, jump, string_count).encode("utf-8"), force)


def build_script(assets, script_id, mapping):
    name = f"{script_id:04d}"
    path = assets / "scripts" / (name + ".json")
    metadata = json.loads(path.read_text(encoding="utf-8"))
    if metadata["version"] != 1 or metadata["id"] != script_id:
        raise ValueError(f"{path}: unsupported metadata version or script ID")
    event_path = assets / "scripts" / (name + ".evt")
    event = None
    if event_path.exists():
        string_count = event_string_count(metadata)
        event = events.assemble(event_path.read_text(encoding="utf-8-sig"), string_count, str(event_path))
        jump_count = sum(bytes.fromhex(c["tag"]) == b"JUMP" for c in metadata["chunks"])
        if jump_count != int(event[1] is not None):
            raise ValueError(f"{event_path}: .jump presence must match metadata's JUMP chunk")
    result = bytearray()
    for item in metadata["chunks"]:
        tag = bytes.fromhex(item["tag"])
        if len(tag) != 4:
            raise ValueError(f"{path}: chunk tag must be four bytes")
        if tag == b"CODE":
            code = event[0] if event is not None else (assets / "scripts" / (name + ".code.bin")).read_bytes()
            payload = pack(len(code)) + code + bytes.fromhex(item["tail"])
        elif tag == b"JUMP":
            payload = event[1] if event is not None else (assets / "scripts" / (name + ".jump.bin")).read_bytes()
        else:
            payload = bytes.fromhex(item["payload"])
            if tag == b"STR ":
                entries = read_text(assets / "text" / (name + ".txt"), mapping)
                originals = strings(payload)
                if len(entries) != len(originals):
                    raise ValueError(f"script {script_id}: preserve all {len(originals)} string IDs")
                if entries != originals:
                    pool = bytearray()
                    offsets = bytearray()
                    for entry in entries:
                        offsets.extend(pack(len(pool)))
                        pool.extend(entry + b"\0")
                    payload = pack(len(entries)) + offsets + pool
        result.extend(tag + pack(len(payload)) + payload)
    data = b"RIFF" + pack(12 + len(result)) + b"SCR " + result
    chunks(data)
    return data


def build(assets, output, overflow_from):
    mapping = charmap(assets / "charmap.txt")
    bank = []
    overflow = []
    used = 0
    spilling = False
    # Validate/encode everything before replacing any generated files.
    scripts = [(i, build_script(assets, i, mapping)) for i in range(1, COUNT + 1)]
    for i, data in scripts:
        size = (len(data) + 3) & ~3
        spilling |= i >= overflow_from or used + size > END - START
        section = overflow if spilling else bank
        section.extend([".balign 4, 0", f".global gScript_{i:04d}", f"gScript_{i:04d}:",
                        f'.incbin "{(output / (str(i).zfill(4) + ".bin")).as_posix()}"'])
        if not spilling:
            used += size
    bank.extend([".balign 4, 0", f".space {END - START - used}, 0"])
    bank.extend(['.pushsection .script_overflow, "a", %progbits', *overflow, ".balign 4, 0", ".popsection"])
    table = [".4byte 0"] + [f".4byte gScript_{i:04d}" for i in range(1, COUNT + 1)]
    for i, data in scripts:
        write_changed(output / f"{i:04d}.bin", data)
    # A changed binary must also invalidate the parent .include/.incbin object.
    digest = hashlib.sha256(b"".join(data for _, data in scripts)).hexdigest()
    bank.insert(0, f"@ Script content SHA-256: {digest}")
    write_changed(output / "table.inc", ("\n".join(table) + "\n").encode())
    write_changed(output / "scripts.s", ("\n".join(bank) + "\n").encode())
    print(f"Built {COUNT} scripts; fixed slot {used:#x}/{END - START:#x} bytes; "
          f"{len(overflow) // 4} overflow scripts")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("extract", "build", "roundtrip", "verify-rom"))
    parser.add_argument("--rom", type=Path, default=Path("baserom.gba"))
    parser.add_argument("--assets", type=Path, default=Path("assets"))
    parser.add_argument("--output", type=Path, default=Path("build/assets"))
    parser.add_argument("--force", action="store_true", help="explicitly overwrite extracted edits")
    parser.add_argument("--overflow-from", type=int, default=COUNT + 1)
    args = parser.parse_args()
    try:
        if not 1 <= args.overflow_from <= COUNT + 1:
            raise ValueError(f"overflow-from must be 1..{COUNT + 1}")
        if args.force and args.command != "extract":
            raise ValueError("--force is only valid with extract")
        if args.command in ("extract", "build"):
            extract(args.rom, args.assets, args.force)
        if args.command == "extract":
            print("Extracted missing assets" if not args.force else "Re-extracted assets (overwritten)")
        elif args.command == "build":
            build(args.assets, args.output, args.overflow_from)
        else:
            mapping = charmap(args.assets / "charmap.txt")
            count = 0
            moved = 0
            for i, start, data in rom_scripts(args.rom.read_bytes()):
                if data != build_script(args.assets, i, mapping):
                    raise ValueError(f"script {i}: ROM/asset mismatch at {start:#x}")
                moved += not START <= start < END
                count += 1
            print(f"{args.command}: {count}/{COUNT} scripts byte-identical; {moved} outside original slot")
    except (OSError, ValueError, KeyError, TypeError) as error:
        parser.exit(1, f"error: {error}\n")


if __name__ == "__main__":
    main()
