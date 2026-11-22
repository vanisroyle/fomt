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
    for start, end in ((0x52D984, 0x58BA08), (0x58BA28, 0x667060)):
        data = rom[start:end]
        sprites.sections(data)
        result.append((start, data))
    return tuple(result)
