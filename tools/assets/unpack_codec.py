"""Bounded reader for the game's Unpack format and a deterministic raw-atom encoder."""

from collections import defaultdict, deque
from array import array
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


def encode_greedy(data):
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


def difference(data, method):
    result = bytearray()
    previous = [0, 0]
    if method == 0:
        return bytes(data)
    if method in (3, 4) and len(data) % 2:
        raise ValueError("word difference filters require even-sized data")
    if method == 3:
        value = 0
        for (word,) in struct.iter_unpack("<H", data):
            result.extend(struct.pack("<H", (word - value) & 65535))
            value = word
    else:
        for i, value in enumerate(data):
            if method == 1:
                hi, lo = value >> 4, value & 15
                result.append(((hi - previous[0]) & 15) << 4 | (lo - hi) & 15)
                previous[0] = lo
            else:
                index = i % 2 if method == 4 else 0
                result.append((value - previous[index]) & 255)
                previous[index] = value
    return bytes(result)


def optimal_tokens(data):
    size = len(data)
    lengths = [bytearray(size), bytearray(size)]
    distances = [array("H", [0]) * size, array("H", [0]) * size]

    def longest(pos, start, stop, limit):
        if start > stop or limit < 3:
            return 0, 0
        previous = data.rfind(data[pos:pos + 3], start, stop + 3)
        if previous < 0:
            return 0, 0
        best, best_previous = 3, previous
        if limit > 3:
            previous = data.rfind(data[pos:pos + limit], start, stop + limit)
            if previous >= 0:
                return limit, pos - previous
        low, high = 4, limit - 1
        while low <= high:
            middle = (low + high) // 2
            previous = data.rfind(data[pos:pos + middle], start, stop + middle)
            if previous >= 0:
                best, best_previous, low = middle, previous, middle + 1
            else:
                high = middle - 1
        return best, pos - best_previous

    for pos in range(size):
        limit = min(66, size - pos)
        lengths[0][pos], distances[0][pos] = longest(pos, max(0, pos - 256), pos - 1, limit)
        if lengths[0][pos] < limit:
            lengths[1][pos], distances[1][pos] = longest(pos, max(0, pos - 4352), pos - 257, limit)
    costs = [0] * (size + 1)
    kinds, counts = bytearray(size), bytearray(size)
    literal = deque()
    run = 0
    for pos in range(size - 1, -1, -1):
        end = pos + 1
        price = costs[end] + 8 * end
        while literal and costs[literal[-1]] + 8 * literal[-1] >= price:
            literal.pop()
        literal.append(end)
        while literal[0] > pos + 64:
            literal.popleft()
        end = literal[0]
        cost, kind, count = 8 + 8 * (end - pos) + costs[end], 2, end - pos
        run = min(65, run + 1) if pos + 1 < size and data[pos] == data[pos + 1] else 1
        for candidate, minimum, maximum, token_cost in ((3, 2, run, 16),
                                                       (0, 3, lengths[0][pos], 16),
                                                       (1, 3, lengths[1][pos], 20)):
            if maximum < minimum:
                continue
            best = min(costs[pos + minimum:pos + maximum + 1])
            if best + token_cost < cost:
                end = costs.index(best, pos + minimum, pos + maximum + 1)
                cost, kind, count = best + token_cost, candidate, end - pos
        costs[pos], kinds[pos], counts[pos] = cost, kind, count
    tokens, pos = [], 0
    while pos < size:
        kind, count = kinds[pos], counts[pos]
        tokens.append((kind, count, distances[kind][pos] if kind < 2 else 0))
        pos += count
    return tokens, costs[0]


def encode_optimal(data, method=0):
    tokens, _ = optimal_tokens(data)
    bits = WriteBits()
    bits.put(method << 5, 8)
    bits.put(7, 4)
    bits.put(11, 4)
    pos = 0
    for kind, count, distance in tokens:
        bits.put(kind, 2)
        if kind == 2:
            bits.put(count - 1, 6)
            for value in data[pos:pos + count]:
                bits.put(value, 8)
        elif kind == 3:
            bits.put(count - 2, 6)
            bits.put(data[pos], 8)
        else:
            bits.put(distance - (257 if kind else 1), 12 if kind else 8)
            bits.put(count - 3, 6)
        pos += count
    return struct.pack("<I", len(data) << 8 | 0x70) + bits.finish()


def encode(data):
    if len(data) > 0x40000:
        raise ValueError("Unpack output exceeds 256 KiB")
    best = encode_greedy(data)
    for method in range(5 if len(data) % 2 == 0 else 3):
        candidate = encode_optimal(difference(data, method), method)
        if len(candidate) < len(best):
            best = candidate
    return best
