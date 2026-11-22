"""Independent model of the ROM glyph lookup and four-cell dialogue chunks."""

import struct


def tagged_lookup(rom, index):
    if rom[0xD0E50:0xD0E54] != bytes.fromhex("00 4a 10 47"):
        return None
    target = struct.unpack_from("<I", rom, 0xD0E54)[0]
    if not target & 1:
        raise ValueError("tag decoder trampoline is not Thumb")
    pc = target & ~1
    regs = [0] * 16
    regs[0], regs[1] = index * 2, 0x08523290
    zero = negative = False
    for _ in range(32):
        op = struct.unpack_from("<H", rom, pc - 0x08000000)[0]
        next_pc = pc + 2
        rd, rs = op & 7, (op >> 3) & 7
        if op & 0xFE00 == 0x1800:
            regs[rd] = (regs[rs] + regs[(op >> 6) & 7]) & 0xFFFFFFFF
        elif op & 0xF800 == 0x2000:
            regs[(op >> 8) & 7] = op & 255
        elif op & 0xFE00 == 0x5E00:
            address = regs[rs] + regs[(op >> 6) & 7] - 0x08000000
            regs[rd] = struct.unpack_from("<h", rom, address)[0] & 0xFFFFFFFF
        elif op & 0xF800 == 0x2800:
            value = (regs[(op >> 8) & 7] - (op & 255)) & 0xFFFFFFFF
            zero, negative = value == 0, bool(value & 0x80000000)
        elif op & 0xF800 == 0:
            regs[rd] = (regs[rs] << ((op >> 6) & 31)) & 0xFFFFFFFF
        elif op & 0xFFC0 == 0x4200:
            zero = (regs[rd] & regs[rs]) == 0
        elif op & 0xFFC0 == 0x4380:
            regs[rd] &= ~regs[rs]
        elif op & 0xF000 == 0xD000:
            condition = (op >> 8) & 15
            if condition not in (0, 11):
                raise ValueError("unsupported decoder condition")
            take = zero if condition == 0 else negative
            if take:
                delta = (op & 255) - (256 if op & 128 else 0)
                next_pc = pc + 4 + delta * 2
        elif op & 0xF800 == 0x4800:
            address = ((pc + 4) & ~3) + (op & 255) * 4
            regs[(op >> 8) & 7] = struct.unpack_from("<I", rom, address - 0x08000000)[0]
        elif op & 0xFF87 == 0x4700:
            if regs[(op >> 3) & 15] != 0x080D0E59:
                raise ValueError("tag decoder does not resume original renderer")
            slot = regs[5] - (0x100000000 if regs[5] & 0x80000000 else 0)
            return slot, regs[6]
        else:
            raise ValueError(f"unsupported tag decoder opcode {op:04X}")
        pc = next_pc
    raise ValueError("tag decoder did not terminate")


def shift_jis_index(code):
    if not 0 <= code <= 0xFFFF:
        return -1
    lead, trail = code >> 8, code & 0xFF
    if not 0x40 <= trail <= 0xFC or not (0x81 <= lead <= 0x9F or 0xE0 <= lead <= 0xEA):
        return -1
    lead -= 0x81 if lead <= 0x9F else 0xC1
    return lead * 189 + trail - 0x40


def lookup(rom, code):
    if code in (0xB1, 0xB2, 0xB3, 0xB4, 0xB6, 0xB7, *range(0xBB, 0xC4)):
        raise ValueError("hardcoded glyph, not a map assignment")
    if 0 < code <= 255:
        slot = struct.unpack_from("<h", rom, 0x4FA7A0 + code * 2)[0]
        width, base, stride = 1, 0x4F90CC, 12
    else:
        index = shift_jis_index(code)
        if index < 0:
            return 0, None, b""
        slot = struct.unpack_from("<h", rom, 0x523290 + index * 2)[0]
        width, base, stride = 2, 0x4FA9A0, 24
        patched = tagged_lookup(rom, index)
        if patched is not None:
            slot, width = patched
            if width == 1:
                base, stride = 0x4F90CC, 12
    if slot < 0:
        return 0, None, b""
    offset = base + slot * stride
    return width, offset, rom[offset:offset + stride]


def assignment(rom, raw):
    pending = 0
    drawn = []
    for byte in raw:
        if byte <= 31:
            raise ValueError("control in glyph assignment")
        pending |= byte
        width, offset, pixels = lookup(rom, pending)
        if width:
            drawn.append((width, offset, pixels))
            pending = 0
        else:
            pending = (pending << 8) & 0xFFFFFFFF
    if pending or len(drawn) != 1:
        raise ValueError("assignment does not render exactly one glyph")
    return drawn[0]


def dialogue_pixels(pixels, width, column):
    if width == 2 and column % 4 == 3:
        return b"".join(pixels[y:y + 1] + b"\0" for y in range(0, 24, 2))
    return pixels


def validate(rom, mapping, expected):
    for char, pixels in expected.items():
        width, _, actual = assignment(rom, mapping[char])
        if actual != pixels:
            raise ValueError(f"{char!r}: renderer selects different glyph pixels")
        for column in range(4):
            if dialogue_pixels(actual, width, column) != pixels:
                raise ValueError(f"{char!r}: right tile clipped at dialogue chunk boundary")
    print(f"Renderer validation: {len(expected)}/{len(expected)} assignments select written pixels; all four dialogue alignments safe", flush=True)
