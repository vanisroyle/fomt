"""Small indexed-color PNG codec; no palette merging or pixel-index remapping."""

import binascii
import struct
import zlib

from font_assets import SIGNATURE, paeth, png_chunk


def write(rows, palette):
    if not rows or not rows[0] or any(len(row) != len(rows[0]) for row in rows):
        raise ValueError("PNG needs nonempty equal-width rows")
    if not 1 <= len(palette) <= 256 or any(len(rgb) != 3 or any(not 0 <= v <= 255 for v in rgb) for rgb in palette):
        raise ValueError("invalid PNG palette")
    depth = 4 if len(palette) <= 16 else 8
    raw = bytearray()
    for row in rows:
        scan = bytearray((len(row) * depth + 7) // 8)
        for x, pixel in enumerate(row):
            if not 0 <= pixel < len(palette):
                raise ValueError("PNG palette index out of range")
            scan[x * depth // 8] |= pixel << (8 - depth - x * depth % 8)
        raw.extend(b"\0" + scan)
    return (SIGNATURE + png_chunk(b"IHDR", struct.pack(">IIBBBBB", len(rows[0]), len(rows), depth, 3, 0, 0, 0))
            + png_chunk(b"PLTE", bytes(v for rgb in palette for v in rgb))
            + png_chunk(b"IDAT", zlib.compress(raw)) + png_chunk(b"IEND", b""))


def read(data):
    if not data.startswith(SIGNATURE):
        raise ValueError("not a PNG")
    pos, header, palette, ended = 8, None, None, False
    compressed = bytearray()
    while pos < len(data):
        if pos + 12 > len(data):
            raise ValueError("truncated PNG chunk")
        size = struct.unpack_from(">I", data, pos)[0]
        tag, payload = data[pos + 4:pos + 8], data[pos + 8:pos + 8 + size]
        if pos + 12 + size > len(data):
            raise ValueError("truncated PNG payload")
        if binascii.crc32(tag + payload) & 0xFFFFFFFF != struct.unpack_from(">I", data, pos + size + 8)[0]:
            raise ValueError("PNG CRC mismatch")
        if tag == b"IHDR":
            if header is not None or pos != 8 or size != 13:
                raise ValueError("invalid PNG header")
            header = struct.unpack(">IIBBBBB", payload)
        elif tag == b"PLTE":
            if palette is not None or not size or size % 3 or size > 768:
                raise ValueError("invalid PNG palette")
            palette = [tuple(payload[i:i + 3]) for i in range(0, size, 3)]
        elif tag == b"IDAT":
            compressed.extend(payload)
        elif tag == b"IEND":
            if size:
                raise ValueError("invalid PNG end")
            ended = True
            pos += size + 12
            break
        elif tag == b"tRNS":
            raise ValueError("use an opaque indexed PNG; game transparency is determined by index 0")
        elif not tag[0] & 32:
            raise ValueError(f"unsupported critical PNG chunk {tag!r}")
        pos += size + 12
    if not ended or pos != len(data) or header is None or palette is None:
        raise ValueError("incomplete PNG or trailing data")
    width, height, depth, color, compression, filtering, interlace = header
    if not (0 < width <= 4096 and 0 < height <= 4096 and depth in (1, 2, 4, 8)
            and color == 3 and not compression and not filtering and not interlace):
        raise ValueError("use a noninterlaced indexed 1/2/4/8-bit PNG, max 4096x4096")
    if len(palette) > 1 << depth:
        raise ValueError("palette exceeds PNG bit depth")
    stride = (width * depth + 7) // 8
    expected = (stride + 1) * height
    inflater = zlib.decompressobj()
    try:
        raw = inflater.decompress(compressed, expected + 1)
    except zlib.error as error:
        raise ValueError(f"invalid PNG compression: {error}") from error
    if len(raw) != expected or not inflater.eof or inflater.unused_data:
        raise ValueError("PNG decompressed size does not match dimensions")
    rows, previous = [], bytearray(stride)
    for y in range(height):
        start = y * (stride + 1)
        method = raw[start]
        if method > 4:
            raise ValueError("invalid PNG filter")
        scan = bytearray(raw[start + 1:start + stride + 1])
        for i in range(stride):
            a, b, c = scan[i - 1] if i else 0, previous[i], previous[i - 1] if i else 0
            prediction = (0, a, b, (a + b) // 2, paeth(a, b, c))[method]
            scan[i] = (scan[i] + prediction) & 255
        row = bytearray((scan[x * depth // 8] >> (8 - depth - x * depth % 8)) & ((1 << depth) - 1) for x in range(width))
        if any(pixel >= len(palette) for pixel in row):
            raise ValueError("PNG palette index out of range")
        rows.append(row)
        previous = scan
    return rows, palette
