"""Bounded reader for the game's Unpack format and a deterministic raw-atom encoder."""

from collections import defaultdict, deque
import importlib.util
from pathlib import Path
import struct


_spec = importlib.util.spec_from_file_location("fomt_unpack_decoder", Path(__file__).parents[1] / "scripts/decompress.py")
_decoder = importlib.util.module_from_spec(_spec)
_spec.loader.exec_module(_decoder)


class ReadBits(_decoder.ReadBits):
    def read_bits(self, num_bits):
        if not 1 <= num_bits <= 32:
            raise ValueError("invalid bit count")
        if num_bits > self.current_bits and self.current_offset + 4 > len(self.source_data):
            raise ValueError("truncated Unpack bitstream")
        return super().read_bits(num_bits)


_decoder.ReadBits = ReadBits


def decode_stream(data):
    if len(data) < 8 or data[0] != 0x70 or int.from_bytes(data[1:4], "little") > 0x40000:
        raise ValueError("invalid Unpack header")
    try:
        reader = ReadBits(data, 4)
        spec = reader.read_bits(8)
        lzss, atom, diff = spec & 7, (spec >> 3) & 3, spec >> 5
        if atom > 2 or lzss > 4 or diff > 4:
            raise ValueError("invalid Unpack format")
        read = (_decoder.ReadRaw, _decoder.ReadHuff4, _decoder.ReadHuff8)[atom](reader)
        if atom and read.huff_tree is None:
            raise ValueError("invalid Unpack Huffman tree")
        stream = _decoder.LZSSDecoder(int.from_bytes(data[1:4], "little"))
        if not _decoder.LZSS_FUNCS[lzss](read, stream) or stream.overflew():
            raise ValueError("invalid Unpack LZSS reference or output size")
        _decoder.DIFF_FUNCS[diff](stream.data)
    except (IndexError, _decoder.UnpackException) as error:
        raise ValueError(f"invalid Unpack stream: {error}") from error
    return bytes(stream.data), f"{atom}{lzss}{diff}", stream.ladder_spec, reader.current_offset


def decode(data):
    return decode_stream(data)[:3]


class WriteBits:
    def __init__(self):
        self.data = bytearray()
        self.word = self.bits = 0

    def put(self, value, count):
        if not 0 <= value < 1 << count:
            raise ValueError("value exceeds bit field")
        for bit in range(count - 1, -1, -1):
            self.word = (self.word << 1) | ((value >> bit) & 1)
            self.bits += 1
            if self.bits == 32:
                self.data.extend(struct.pack("<I", self.word))
                self.word = self.bits = 0

    def finish(self):
        if self.bits:
            self.data.extend(struct.pack("<I", self.word << (32 - self.bits)))
            self.word = self.bits = 0
        return bytes(self.data)


def encode(data):
    if len(data) > 0x40000:
        raise ValueError("Unpack output exceeds 256 KiB")
    bits = WriteBits()
    bits.put(0, 8)  # raw atoms, LZSS 0, no difference filter
    bits.put(7, 4)
    bits.put(11, 4)
    positions = defaultdict(lambda: deque(maxlen=32))
    literals = bytearray()

    def flush():
        if literals:
            bits.put(2, 2)
            bits.put(len(literals) - 1, 6)
            for value in literals:
                bits.put(value, 8)
            literals.clear()

    pos = 0
    while pos < len(data):
        run = 1
        while run < 65 and pos + run < len(data) and data[pos + run] == data[pos]:
            run += 1
        length = distance = 0
        key = data[pos:pos + 3]
        for previous in reversed(positions.get(key, ())):
            dist = pos - previous
            if dist > 4352:
                break
            size = 0
            while size < 66 and pos + size < len(data) and data[pos + size] == data[pos + size - dist]:
                size += 1
            if size > length:
                length, distance = size, dist
                if size == 66:
                    break
        if run >= 3 and run >= length:
            flush()
            bits.put(3, 2)
            bits.put(run - 2, 6)
            bits.put(data[pos], 8)
            consumed = run
        elif length >= 3:
            flush()
            second = distance > 256
            bits.put(int(second), 2)
            bits.put(distance - (257 if second else 1), 12 if second else 8)
            bits.put(length - 3, 6)
            consumed = length
        else:
            literals.append(data[pos])
            consumed = 1
            if len(literals) == 64:
                flush()
        for p in range(pos, pos + consumed):
            positions[data[p:p + 3]].append(p)
            expired = p - 4352
            if expired >= 0:
                old_key = data[expired:expired + 3]
                old = positions.get(old_key)
                if old and old[0] == expired:
                    old.popleft()
                    if not old:
                        del positions[old_key]
        pos += consumed
    flush()
    return struct.pack("<I", len(data) << 8 | 0x70) + bits.finish()
