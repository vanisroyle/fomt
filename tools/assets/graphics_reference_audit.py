"""Non-pointer regions proven by instruction mappings and resource schemas."""

import hashlib
import json
from pathlib import Path

import title_graphics as sprites


def regions(rom):
    if len(rom) != 0x800000:
        return ()
    result = []
    audit = json.loads(Path(__file__).with_name("graphics_instruction_audit.json").read_text())
    for record in audit["regions"]:
        start, end = record["start"], record["end"]
        data = rom[start:end]
        if hashlib.sha256(data).hexdigest() != record["sha256"]:
            raise ValueError(f"audited Thumb instructions changed at {start:#x}")
        result.append((start, data))
    for record in audit.get("data_regions", ()):
        start, end = record["start"], record["end"]
        data = rom[start:end]
        if hashlib.sha256(data).hexdigest() != record["sha256"]:
            raise ValueError(f"audited non-pointer data changed at {start:#x}")
        result.append((start, data))
    for start, end in ((0x52D984, 0x58BA08), (0x58BA28, 0x667060)):
        data = rom[start:end]
        sprites.sections(data)
        result.append((start, data))
    for start, end in ((0x6678A0, 0x67A0E8), (0x74F34C, 0x7506E0)):
        data = rom[start:end]
        sections = sprites.sections(data)
        for index in (3, 4):
            offset, count = sections[index]
            result.append((start + offset, data[offset:offset + count * 32]))
    return tuple(result)
