"""Independent model of the ROM glyph lookup and four-cell dialogue chunks."""

import struct


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
