"""Lossless M4A sequence assembly and signed PCM/WAV editing."""

import argparse
import ast
import bisect
import hashlib
from collections import defaultdict
import io
import json
from pathlib import Path
import re
import struct
import wave

import fomt_assets as fa
from font_assets import original_rom


ROM_BASE = 0x08000000
TABLE, SONG_COUNT = 0x13ABF0, 211
VOICE_START, VOICE_END = 0x139D60, 0x13ABB4
BANK_START, TRACK_START, BANK_END = 0x13B288, 0x29CD3C, 0x2AC0B0
OVERFLOW = 0x75C244
DURATIONS = list(range(25)) + [28, 30, 32, 36, 40, 42, 44, 48, 52, 54, 56, 60, 64, 66, 68, 72, 76, 78, 80, 84, 88, 90, 92, 96]
COMMANDS = {0xB1: "FINE", 0xB2: "GOTO", 0xB3: "PATT", 0xB4: "PEND", 0xB5: "REPT",
            0xB9: "MEMACC", 0xBA: "PRIO", 0xBB: "TEMPO", 0xBC: "KEYSH", 0xBD: "VOICE",
            0xBE: "VOL", 0xBF: "PAN", 0xC0: "BEND", 0xC1: "BENDR", 0xC2: "LFOS",
            0xC3: "LFODL", 0xC4: "MOD", 0xC5: "MODT", 0xC8: "TUNE", 0xCC: "PORT",
            0xCD: "XCMD", 0xCE: "EOT", 0xCF: "TIE"}
COMMANDS.update({0x80 + i: f"W{n:02}" for i, n in enumerate(DURATIONS)})
COMMANDS.update({0xCF + i: f"N{n:02}" for i, n in enumerate(DURATIONS) if i})
CONSTANTS = {name: value for value, name in COMMANDS.items()}
CONSTANTS["C_V"] = 64
CONSTANTS.update({f"v{i:03}": i for i in range(128)})
CONSTANTS.update({f"{name}{'M' + str(-octave) if octave < 0 else octave}": octave * 12 + i + 24 for octave in range(-2, 9)
                  for i, name in enumerate(("Cn", "Cs", "Dn", "Ds", "En", "Fn", "Fs", "Gn", "Gs", "An", "As", "Bn"))
                  if octave * 12 + i + 24 < 128})


def symbol(kind, offset):
    return f"gAudio{kind}_{offset:08X}"


def u32(raw, offset):
    if offset < 0 or offset + 4 > len(raw):
        raise ValueError(f"truncated audio word at 0x{offset:X}")
    return struct.unpack_from("<I", raw, offset)[0]


def pointer(raw, offset):
    target = u32(raw, offset) - ROM_BASE
    if not 0 <= target < len(raw):
        raise ValueError(f"invalid audio pointer at 0x{offset:X}")
    return target


def sequences(raw, starts, lower, upper):
    """Walk driver control flow, preserving running status and pattern returns."""
    pending = [(start, 0, None, 0, ()) for start in starts]
    seen, events, pointers, notes = set(), {}, {}, set()
    while pending:
        pos, running, voice, key, stack = pending.pop()
        state = pos, running, voice, key, stack
        if state in seen:
            continue
        seen.add(state)
        if len(seen) > 500000:
            raise ValueError("M4A control flow exceeds bounded state budget")
        if not lower <= pos < upper:
            raise ValueError(f"M4A control flow leaves song at 0x{pos:X}")
        start = pos
        explicit = raw[pos] >= 0x80
        command = raw[pos] if explicit else running
        if not command:
            raise ValueError(f"M4A running status is unset at 0x{pos:X}")
        pos += int(explicit)
        if explicit and command >= 0xBD:
            running = command
        edges = []

        def take(count):
            nonlocal pos
            if pos + count > upper:
                raise ValueError(f"truncated M4A command at 0x{start:X}")
            data = raw[pos:pos + count]
            pos += count
            return data

        def target():
            at = pos
            take(4)
            dest = pointer(raw, at)
            if not lower <= dest < upper:
                raise ValueError(f"M4A branch leaves song at 0x{at:X}")
            pointers[at] = dest
            return dest

        if command >= 0xCF:
            args = []
            while len(args) < 3 and pos < upper and raw[pos] < 0x80:
                args.extend(take(1))
            if args:
                key = args[0]
            if voice is not None:
                notes.add((voice, key))
        elif command == 0xCE:
            if pos < upper and raw[pos] < 0x80:
                take(1)
        elif 0x80 <= command <= 0xB0:
            pass
        elif command in (0xB2, 0xB3):
            dest = target()
            if command == 0xB3:
                if len(stack) == 3:
                    raise ValueError("M4A pattern stack exceeds three levels")
                stack = stack + (pos,)
            edges.append((dest, stack))
        elif command == 0xB4:
            if stack:
                edges.append((stack[-1], stack[:-1]))
            else:
                edges.append((pos, stack))
        elif command == 0xB5:
            count = take(1)[0]
            edges.append((target(), stack))
            if count:
                edges.append((pos, stack))
        elif command == 0xB9:
            op = take(3)[0]
            if op > 17:
                raise ValueError("unsupported M4A MEMACC operation")
            if op >= 6:
                edges.extend(((target(), stack), (pos, stack)))
        elif command == 0xCD:
            op = take(1)[0]
            if op == 1:
                at = pos
                take(4)
                pointers[at] = pointer(raw, at)
            elif op in (2, 4, 5, 6, 7, 8, 9, 10, 11):
                take(1)
            else:
                raise ValueError(f"unsupported M4A XCMD {op}")
        elif command in (0xBA, 0xBB, 0xBC, 0xBD, 0xBE, 0xBF, 0xC0, 0xC1, 0xC2, 0xC3, 0xC4, 0xC5, 0xC8):
            value = take(1)[0]
            if command == 0xBD:
                if value >= 128:
                    raise ValueError("M4A instrument number exceeds 127")
                voice = value
        elif command == 0xCC:
            take(2)
        elif command != 0xB1:
            raise ValueError(f"unsupported M4A command 0x{command:02X} at 0x{start:X}")
        event = (pos, command, explicit)
        if start in events and events[start] != event:
            raise ValueError(f"ambiguous M4A running-status parse at 0x{start:X}")
        events[start] = event
        if not edges and command not in (0xB1, 0xB2, 0xB3, 0xB4, 0xB5):
            edges.append((pos, stack))
        for dest, returns in edges:
            pending.append((dest, running, voice, key, returns))
    return {"events": events, "pointers": pointers, "notes": notes}


def inventory(rom):
    table = [struct.unpack_from("<IHH", rom, TABLE + i * 8) for i in range(SONG_COUNT)]
    aliases = defaultdict(list)
    for i, (address, _, _) in enumerate(table):
        aliases[address - ROM_BASE].append(i)
    songs, refs, roots = [], {}, set()
    previous = TRACK_START
    for header in sorted(aliases):
        count = rom[header]
        if not count:
            if header != BANK_START:
                raise ValueError("unexpected empty M4A header")
            song = {"header": header, "start": header, "end": header + 4, "tracks": [], "voice": None,
                    "parse": {"events": {}, "pointers": {}, "notes": set()}}
        else:
            if count > 8 or header < previous:
                raise ValueError("invalid original M4A header ordering")
            voice = pointer(rom, header + 4)
            tracks = [pointer(rom, header + 8 + i * 4) for i in range(count)]
            if min(tracks) != previous:
                raise ValueError("M4A song data are not a disjoint contiguous bank")
            end = (header + 8 + count * 4 + 3) & ~3
            parsed = sequences(rom, tracks, previous, header)
            song = {"header": header, "start": previous, "end": end, "tracks": tracks, "voice": voice, "parse": parsed}
            roots.add(voice)
            refs[header + 4] = voice
            refs.update({header + 8 + i * 4: track for i, track in enumerate(tracks)})
            refs.update(parsed["pointers"])
            previous = end
        song["ids"] = aliases[header]
        songs.append(song)
    if previous != BANK_END or len(songs) != 145:
        raise ValueError("unexpected M4A song bank layout")
    for i, (address, _, _) in enumerate(table):
        refs[TABLE + i * 8] = address - ROM_BASE
    cgb = {pointer(rom, root + i * 12 + 4) for root in roots for i in range(128) if rom[root + i * 12] & 7 == 3}
    tone_end = min(cgb)
    if min(roots) != VOICE_START or tone_end > VOICE_END or (tone_end - VOICE_START) % 12:
        raise ValueError("unexpected M4A voice/PSG bank boundary")
    tones = list(range(VOICE_START, tone_end, 12))
    samples = set()
    for tone in tones:
        kind = rom[tone]
        if kind & 0x40:
            raise ValueError("unmapped key-split voicegroup")
        if kind & 0x80 or kind & 7 in (0, 3):
            dest = pointer(rom, tone + 4)
            refs[tone + 4] = dest
            if kind & 0x80:
                if dest not in tones:
                    raise ValueError("rhythm voicegroup is not in the tone bank")
            elif kind & 7 == 0:
                samples.add(dest)
    for song in songs:
        if song["voice"] is None:
            continue
        for program, key in song["parse"]["notes"]:
            tone = song["voice"] + program * 12
            if rom[tone] & 0x80:
                tone = pointer(rom, tone + 4) + key * 12
            if tone not in tones or rom[tone] & 0xC0:
                raise ValueError(f"played instrument outside bounded tone bank: 0x{tone:X}")
    waves = []
    for offset in sorted(samples):
        kind, status, freq, loop, count = struct.unpack_from("<HHIII", rom, offset)
        if kind or not freq or not count or loop > count or not BANK_START + 4 <= offset < TRACK_START:
            raise ValueError(f"unsupported DirectSound sample at 0x{offset:X}")
        end = (offset + 16 + count + 4) & ~3
        if end > TRACK_START:
            raise ValueError("DirectSound sample exceeds bank")
        waves.append({"start": offset, "end": end, "kind": kind, "status": status, "frequency": freq,
                      "loop_start": loop, "count": count, "rate": (freq + 512) // 1024})
    for left, right in zip(waves, waves[1:]):
        if left["end"] != right["start"]:
            raise ValueError("unmapped gap/overlap in DirectSound sample bank")
    if waves[0]["start"] != BANK_START + 4 or waves[-1]["end"] != TRACK_START:
        raise ValueError("DirectSound samples do not cover bank")
    return {"songs": songs, "table": table, "roots": roots, "tones": tones, "cgb": cgb,
            "samples": waves, "refs": refs}


def instrument_constants(model):
    return {f"gAudioInstrument_{root:08X}_{i:03}": i for root in model["roots"] for i in range(128)}


def expression(source, names):
    def visit(node):
        if isinstance(node, ast.Constant) and type(node.value) is int:
            return node.value
        if isinstance(node, ast.Name) and node.id in names:
            return names[node.id]
        if isinstance(node, ast.UnaryOp) and isinstance(node.op, (ast.USub, ast.UAdd)):
            return -visit(node.operand) if isinstance(node.op, ast.USub) else visit(node.operand)
        if isinstance(node, ast.BinOp) and isinstance(node.op, (ast.Add, ast.Sub, ast.Mult, ast.FloorDiv)):
            a, b = visit(node.left), visit(node.right)
            if isinstance(node.op, ast.Add):
                return a + b
            if isinstance(node.op, ast.Sub):
                return a - b
            if isinstance(node.op, ast.Mult):
                return a * b
            return a // b
        raise ValueError(f"unknown symbol or unsupported expression: {source}")
    try:
        return visit(ast.parse(source, mode="eval").body)
    except (SyntaxError, ZeroDivisionError) as error:
        raise ValueError(f"invalid assembly expression: {source}") from error


def assemble(source, base, external=None, filename="<assembly>", measure=False):
    """A bounded GAS data-directive subset; never executes extracted source."""
    names = dict(CONSTANTS, **(external or {}))
    labels, rows, pos = {}, [], 0
    for line_number, line in enumerate(source.splitlines(), 1):
        text = line.split("@", 1)[0].strip()
        try:
            if not text or text.startswith((".global ", ".globl ")):
                continue
            if text.startswith(".include "):
                name = text[len(".include "):].strip('"')
                if Path(name).name not in ("m4a_commands.inc", "instruments.inc"):
                    raise ValueError("only M4A command/instrument includes are supported")
                continue
            if text.endswith(":"):
                name = text[:-1]
                if not re.fullmatch(r"[A-Za-z_][A-Za-z_0-9]*", name) or name in labels or name in CONSTANTS:
                    raise ValueError(f"invalid or duplicate label {name}")
                labels[name] = pos
                continue
            directive, _, values = text.partition(" ")
            args = [value.strip() for value in values.split(",")]
            if directive in (".equ", ".equiv", ".set"):
                if len(args) != 2 or not re.fullmatch(r"[A-Za-z_][A-Za-z_0-9]*", args[0]):
                    raise ValueError("invalid constant definition")
                names[args[0]] = expression(args[1], names)
                continue
            if directive in (".balign", ".align"):
                alignment = expression(args[0], names)
                if directive == ".align":
                    alignment = 1 << alignment if 0 <= alignment <= 2 else 0
                if alignment not in (1, 2, 4) or len(args) > 2:
                    raise ValueError("audio alignment must be a power of two, at most 4")
                fill = args[1] if len(args) == 2 else "0"
                padding = (-(base + pos)) % alignment
                rows.append((pos, 1, [fill] * padding, line_number))
                pos += padding
                continue
            width = {".byte": 1, ".hword": 2, ".2byte": 2, ".word": 4, ".4byte": 4}.get(directive)
            if not width or not values:
                raise ValueError(f"unsupported directive: {directive}")
            rows.append((pos, width, args, line_number))
            pos += width * len(args)
            if pos > 0x200000:
                raise ValueError("audio source exceeds 2 MiB limit")
        except ValueError as error:
            raise ValueError(f"{filename}:{line_number}: {error}") from error
    if measure:
        return pos, labels
    names.update({name: base + offset for name, offset in labels.items()})
    runtime_names = (set(labels) | set(external or {})) - set(CONSTANTS)
    runtime_names = {name for name in runtime_names if not name.startswith("gAudioInstrument_")}
    data, relocations = bytearray(pos), {}
    for pos, width, args, line_number in rows:
        for arg in args:
            try:
                value = expression(arg, names)
                if not -(1 << (width * 8 - 1)) <= value < 1 << (width * 8):
                    raise ValueError(f"value does not fit {width}-byte field: {arg}")
                data[pos:pos + width] = (value & ((1 << (width * 8)) - 1)).to_bytes(width, "little")
                identifiers = {node.id for node in ast.walk(ast.parse(arg, mode="eval")) if isinstance(node, ast.Name)}
                if width == 4 and identifiers & runtime_names:
                    relocations[pos] = re.sub(r"\b[A-Za-z_][A-Za-z_0-9]*\b",
                                             lambda match: match[0] if match[0] in runtime_names else str(names[match[0]]), arg)
                pos += width
            except ValueError as error:
                raise ValueError(f"{filename}:{line_number}: {error}") from error
    return bytes(data), labels, relocations


def source_image(raw, start, end, labels, refs, events=None, voice=None):
    lines, pos = [], start
    events = events or {}
    while pos < end:
        if pos in labels:
            lines.extend((f".global {labels[pos]}", labels[pos] + ":"))
        if pos in refs:
            lines.append(f"    .word {refs[pos]}")
            pos += 4
        elif pos in events:
            stop, command, explicit = events[pos]
            values = [COMMANDS[command]] if explicit else []
            argument_start = pos + int(explicit)
            for i in range(argument_start, stop):
                if i in refs:
                    break
                value = raw[i]
                if command == 0xBD and voice is not None:
                    values.append(f"gAudioInstrument_{voice:08X}_{value:03}")
                elif command == 0xBC:
                    values.append(str(value if value < 128 else value - 256))
                else:
                    values.append(str(value))
            lines.append("    .byte " + ", ".join(values) + (f" @ running {COMMANDS[command]}" if not explicit else ""))
            pos = argument_start + len(values) - int(explicit)
        else:
            stop = min(pos + 16, end)
            for at in range(pos + 1, stop):
                if at in labels or at in refs or at in events:
                    stop = at
                    break
            lines.append("    .byte " + ", ".join(f"0x{value:02X}" for value in raw[pos:stop]))
            pos = stop
    return "\n".join(lines) + "\n"


def wav_bytes(pcm, rate):
    output = io.BytesIO()
    with wave.open(output, "wb") as wav:
        wav.setparams((1, 1, rate, len(pcm), "NONE", "not compressed"))
        wav.writeframes(bytes(value ^ 0x80 for value in pcm))
    return output.getvalue()


def rebuild_sample(wav_data, metadata, original):
    try:
        with wave.open(io.BytesIO(wav_data), "rb") as wav:
            if wav.getnchannels() != 1 or wav.getsampwidth() != 1 or wav.getcomptype() != "NONE":
                raise ValueError("DirectSound WAV must be uncompressed mono 8-bit PCM")
            rate, count = wav.getframerate(), wav.getnframes()
            frames = wav.readframes(count)
        if not frames or len(frames) != count:
            raise ValueError("DirectSound WAV is empty or truncated")
    except (wave.Error, EOFError) as error:
        raise ValueError(f"invalid DirectSound WAV: {error}") from error
    if not isinstance(metadata, dict) or set(metadata) != {"kind", "status", "frequency", "loop_start"} or any(type(value) is not int for value in metadata.values()):
        raise ValueError("sample metadata requires integer kind/status/frequency/loop_start")
    kind, status, freq, loop = (metadata[key] for key in ("kind", "status", "frequency", "loop_start"))
    if rate != original["rate"]:
        if freq != original["frequency"]:
            raise ValueError("edit WAV rate or metadata frequency, not both")
        freq = rate * 1024
    if kind or status not in (0, 0x4000, 0x8000, 0xC000) or not 0 < freq <= 0xFFFFFFFF or not 0 <= loop <= count:
        raise ValueError("unsupported sample kind/status/frequency or loop outside PCM frames")
    if status & 0xC000 and loop == count:
        raise ValueError("loop must contain at least one PCM frame")
    return struct.pack("<HHIII", kind, status, freq, loop, count) + bytes(value ^ 0x80 for value in frames)


def extract(rom, assets, force=False, model=None):
    model = model or inventory(rom)
    root = assets / "audio"
    definitions = "".join(f".equiv {name}, {value}\n" for name, value in sorted(instrument_constants(model).items()))
    fa.write_missing(root / "instruments.inc", definitions.encode(), force)
    for song in model["songs"]:
        labels = {song["header"]: symbol("Song", song["header"])}
        labels.update({at: symbol("Track", at) for at in set(song["tracks"]) | set(song["parse"]["pointers"].values())
                       if song["start"] <= at < song["header"]})
        refs = {at: symbol("Track", dest) for at, dest in song["parse"]["pointers"].items()}
        if song["tracks"]:
            refs[song["header"] + 4] = symbol("Voice", song["voice"])
            refs.update({song["header"] + 8 + i * 4: symbol("Track", track) for i, track in enumerate(song["tracks"])})
        text = ('    .include "tools/assets/m4a_commands.inc"\n'
                f'    .include "{(root / "instruments.inc").as_posix()}"\n'
                f'@ Song IDs {song["ids"]}; original ROM 0x{song["start"]:X}..0x{song["end"]:X}\n')
        text += source_image(rom, song["start"], song["end"], labels, refs, song["parse"]["events"], song["voice"])
        if song["tracks"]:
            text = text.replace(f'.global {symbol("Song", song["header"])}', f'.balign 4\n.global {symbol("Song", song["header"])}')
        fa.write_missing(root / "songs" / f'{song["ids"][0]:04}.s', text.encode(), force)
    labels = {at: symbol("Voice", at) for at in model["tones"]}
    labels.update({at: symbol("CgbWave", at) for at in model["cgb"]})
    sample_offsets = {sample["start"] for sample in model["samples"]}
    refs = {at: symbol("Sample" if dest in sample_offsets else "CgbWave" if dest in model["cgb"] else "Voice", dest)
            for at, dest in model["refs"].items() if VOICE_START <= at < VOICE_END}
    fa.write_missing(root / "voices.s", source_image(rom, VOICE_START, VOICE_END, labels, refs).encode(), force)
    for sample in model["samples"]:
        offset, count = sample["start"], sample["count"]
        fa.write_missing(root / "samples" / f"{offset:08X}.wav", wav_bytes(rom[offset + 16:offset + 16 + count], sample["rate"]), force)
        metadata = {key: sample[key] for key in ("kind", "status", "frequency", "loop_start")}
        fa.write_missing(root / "samples" / f"{offset:08X}.json", (json.dumps(metadata, indent=2) + "\n").encode(), force)
    return model


def audit_relocations(rom, spans, model):
    """Reject unknown raw references; signed PCM bytes are not pointer fields."""
    if not spans:
        return
    ordered = sorted(spans)
    starts = [start for start, end in ordered]
    pcm = [(sample["start"] + 16, sample["start"] + 17 + sample["count"]) for sample in model["samples"]]
    pcm_starts = [start for start, end in pcm]
    for at in range(len(rom) - 3):
        if rom[at + 3] != 8:
            continue
        value = u32(rom, at) - ROM_BASE
        index = bisect.bisect_right(starts, value) - 1
        if index < 0 or value >= ordered[index][1]:
            continue
        if model["refs"].get(at) == value:
            continue
        index = bisect.bisect_right(pcm_starts, at) - 1
        if index >= 0 and at + 4 <= pcm[index][1]:
            continue
        raise ValueError(f"cannot relocate audio: non-symbolic reference at 0x{at:X} -> 0x{value:X}")


def image_assembly(path, data, labels, relocations):
    by_offset = defaultdict(list)
    for name, offset in labels.items():
        by_offset[offset].append(name)
    boundaries = sorted(set(by_offset) | set(relocations) | {0, len(data)})
    lines, pos = [], 0
    while pos <= len(data):
        for name in sorted(by_offset[pos]):
            lines.extend((f".global {name}", name + ":"))
        if pos == len(data):
            break
        if pos in relocations:
            if any(pos < at < pos + 4 for at in boundaries):
                raise ValueError("audio label overlaps a pointer operand")
            lines.append(f".4byte {relocations[pos]}")
            pos += 4
        else:
            stop = boundaries[bisect.bisect_right(boundaries, pos)]
            lines.append(f'.incbin "{path.as_posix()}", {pos}, {stop - pos}')
            pos = stop
    return lines


def prepare(rom, assets, output, overflow_songs=()):
    model = extract(rom, assets)
    root = assets / "audio"
    constants = instrument_constants(model)
    units, names, cursor = [], {}, OVERFLOW
    for song in model["songs"]:
        path = root / "songs" / f'{song["ids"][0]:04}.s'
        source = path.read_text(encoding="utf-8-sig")
        size, labels = assemble(source, 0, constants, str(path), True)
        unit = dict(song, name=f'song-{song["ids"][0]:04}', source=source, path=path, size=size, labels=labels, kind="song")
        unit["overflow"] = size > song["end"] - song["start"] or bool(set(song["ids"]) & set(overflow_songs))
        if not song["tracks"] and (size != 4 or unit["overflow"]):
            raise ValueError("the four-byte empty song must remain in its original slot")
        units.append(unit)
    unknown = set(overflow_songs) - {i for song in model["songs"] for i in song["ids"]}
    if unknown:
        raise ValueError(f"unknown overflow song IDs: {sorted(unknown)}")
    for sample in model["samples"]:
        stem = root / "samples" / f'{sample["start"]:08X}'
        try:
            meta = json.loads(stem.with_suffix(".json").read_text(encoding="utf-8-sig"))
            data = rebuild_sample(stem.with_suffix(".wav").read_bytes(), meta, sample)
            before = rom[sample["start"] + 16:sample["start"] + 16 + sample["count"]]
            guard = rom[sample["start"] + 16 + sample["count"]]
            if data[16:] != before or meta["status"] != sample["status"] or meta["loop_start"] != sample["loop_start"]:
                guard = data[16 + meta["loop_start"]] if meta["status"] & 0xC000 else 0
            data += bytes([guard])
        except (OSError, ValueError) as error:
            raise ValueError(f"{stem}: {error}") from error
        unit = dict(sample, name=f'sample-{sample["start"]:08X}', data=data, size=len(data), kind="sample",
                    labels={symbol("Sample", sample["start"]): 0})
        unit["overflow"] = len(data) > sample["end"] - sample["start"]
        units.append(unit)
    units.sort(key=lambda unit: unit["start"])
    audit_relocations(rom, [(unit["start"], unit["end"]) for unit in units if unit["overflow"]], model)
    for unit in units:
        unit["target"] = cursor if unit["overflow"] else unit["start"]
        if unit["overflow"]:
            cursor = (cursor + unit["size"] + 3) & ~3
        for name, offset in unit["labels"].items():
            if name in names:
                raise ValueError(f"duplicate global audio label {name}")
            names[name] = ROM_BASE + unit["target"] + offset
    if cursor > 0x2000000:
        raise ValueError("audio overflow exceeds 32 MiB ROM limit")
    voice_path = root / "voices.s"
    voice_source = voice_path.read_text(encoding="utf-8-sig")
    voice_size, voice_labels = assemble(voice_source, 0, constants, str(voice_path), True)
    if voice_size != VOICE_END - VOICE_START:
        raise ValueError("preserve the fixed-size voice/PSG bank")
    required = {symbol("Voice", at): at - VOICE_START for at in model["tones"]}
    required.update({symbol("CgbWave", at): at - VOICE_START for at in model["cgb"]})
    if any(voice_labels.get(name) != offset for name, offset in required.items()):
        raise ValueError("preserve all voice/PSG label positions")
    names.update({name: ROM_BASE + VOICE_START + offset for name, offset in voice_labels.items()})
    names.update(constants)
    voices, _, voice_refs = assemble(voice_source, ROM_BASE + VOICE_START, names, str(voice_path))
    for at in model["tones"]:
        pos = at - VOICE_START
        kind, dest = voices[pos], u32(voices, pos + 4)
        if kind not in (0, 1, 2, 3, 4, 8, 9, 11, 12, 128):
            raise ValueError(f"unsupported edited instrument kind at 0x{at:X}")
        if kind & 0x80 or kind & 7 in (0, 3):
            valid = (model["tones"] if kind & 0x80 else model["cgb"] if kind & 7 == 3 else
                     [unit["target"] for unit in units if unit["kind"] == "sample"])
            if pos + 4 not in voice_refs or dest - ROM_BASE not in valid:
                raise ValueError(f"instrument at 0x{at:X} requires a symbolic compatible pointer")
    memory = bytearray(rom)
    memory.extend(b"\xff" * max(0, cursor - len(memory)))
    memory[VOICE_START:VOICE_END] = voices
    for unit in units:
        if unit["kind"] == "song":
            unit["data"], _, unit["relocations"] = assemble(unit["source"], ROM_BASE + unit["target"], names, str(unit["path"]))
        else:
            unit["relocations"] = {}
        at = unit["target"]
        memory[at:at + unit["size"]] = unit["data"]
    for unit in units:
        if unit["kind"] != "song":
            continue
        header_name = symbol("Song", unit["header"])
        if header_name not in unit["labels"]:
            raise ValueError(f'{unit["path"]}: missing song header label {header_name}')
        header = names[header_name] - ROM_BASE
        count = memory[header]
        if not unit["tracks"]:
            if unit["data"] != rom[unit["start"]:unit["end"]]:
                raise ValueError("keep the empty song unchanged")
            continue
        if header % 4 or not 1 <= count <= 8 or header + 8 + count * 4 != unit["target"] + unit["size"]:
            raise ValueError(f'{unit["path"]}: put the 1..8-track header last, without trailing padding')
        if pointer(memory, header + 4) not in model["roots"]:
            raise ValueError("song voicegroup must use an existing root voicegroup")
        tracks = [pointer(memory, header + 8 + i * 4) for i in range(count)]
        parsed = sequences(memory, tracks, unit["target"], header)
        sample_targets = {sample["target"] for sample in units if sample["kind"] == "sample"}
        if any(not unit["target"] <= dest < header and dest not in sample_targets for dest in parsed["pointers"].values()):
            raise ValueError("XCMD wave requires a symbolic DirectSound sample pointer")
        fields = [header + 4] + [header + 8 + i * 4 for i in range(count)] + list(parsed["pointers"])
        if any(pos - unit["target"] not in unit["relocations"] for pos in fields):
            raise ValueError(f'{unit["path"]}: use symbolic .word operands for every M4A pointer')
        voice = pointer(memory, header + 4)
        for program, key in parsed["notes"]:
            tone = voice + program * 12
            if memory[tone] & 0x80:
                tone = pointer(memory, tone + 4) + key * 12
            if tone not in model["tones"] or memory[tone] & 0xC0:
                raise ValueError("edited song plays an invalid rhythm instrument")
    return model, units, names, voices, voice_labels, voice_refs


def build(rom, assets, output, overflow_songs=()):
    model, units, names, voices, voice_labels, voice_refs = prepare(rom, assets, output, overflow_songs)
    products = {"audio-voices.bin": voices}
    lines = image_assembly(output / "audio-voices.bin", voices, voice_labels, voice_refs)
    products["audio-voices.s"] = ("\n".join(lines) + "\n").encode()
    bank, cursor, overflow_lines, layout = [], BANK_START, ['.section .audio_overflow, "a"'], []
    for unit in units:
        start, end, target = unit["start"], unit["end"], unit["target"]
        if start > cursor:
            bank.append(f'.incbin "baserom.gba", {cursor}, {start - cursor}')
        path = output / f'audio-{unit["name"]}.bin'
        products[path.name] = unit["data"]
        image = image_assembly(path, unit["data"], unit["labels"], unit["relocations"])
        if unit["overflow"]:
            bank.append(f'.incbin "baserom.gba", {start}, {end - start}')
            overflow_lines += [".balign 4"] + image
        else:
            bank += image
            if unit["size"] < end - start:
                bank.append(f'.incbin "baserom.gba", {start + unit["size"]}, {end - start - unit["size"]}')
        cursor = end
        layout.append({"name": unit["name"], "original": start, "target": target, "bytes": unit["size"], "overflow": unit["overflow"]})
    if cursor != BANK_END:
        raise ValueError("generated audio bank does not end at the script bank")
    products["audio-bank.s"] = ("\n".join(bank + overflow_lines + ['.section .rodata']) + "\n").encode()
    table = []
    for address, player, unknown in model["table"]:
        table += [f".4byte {symbol('Song', address - ROM_BASE)}", f".2byte {player}, {unknown}"]
    products["audio-table.s"] = ("\n".join(table) + "\n").encode()
    products["audio-layout.json"] = (json.dumps(layout, indent=2) + "\n").encode()
    for name, data in products.items():
        fa.write_changed(output / name, data)
    fa.write_changed(output / "audio.stamp", (hashlib.sha256(b"".join(products.values())).hexdigest() + "\n").encode())
    return model, units


def roundtrip(rom, assets, output):
    model, units, *_ = prepare(rom, assets, output)
    for unit in units:
        if unit["overflow"] or unit["data"] != rom[unit["start"]:unit["start"] + unit["size"]]:
            raise ValueError(f'roundtrip differs: {unit["name"]}')
    voices, _, _ = assemble((assets / "audio/voices.s").read_text(encoding="utf-8-sig"), ROM_BASE + VOICE_START,
                            {**instrument_constants(model), **{symbol("Sample", s["start"]): ROM_BASE + s["start"] for s in model["samples"]},
                             **{symbol("Voice", p): ROM_BASE + p for p in model["tones"]}, **{symbol("CgbWave", p): ROM_BASE + p for p in model["cgb"]}})
    if voices != rom[VOICE_START:VOICE_END]:
        raise ValueError("voice bank roundtrip differs")
    print(f"Audio roundtrip: {len(model['songs'])}/145 song headers and sequences byte-identical; 211 symbolic song-table entries")
    print(f"Audio roundtrip: {len(model['samples'])}/106 DirectSound WAV samples byte-identical; 263 tones / 2 root voicegroups / 8 PSG waves preserved")


def main():
    parser = argparse.ArgumentParser(description=__doc__)
    parser.add_argument("command", choices=("extract", "build", "roundtrip"))
    parser.add_argument("--rom", type=Path, default=Path("baserom.gba"))
    parser.add_argument("--assets", type=Path, default=Path("assets"))
    parser.add_argument("--output", type=Path, default=Path("build/assets"))
    parser.add_argument("--force", action="store_true")
    parser.add_argument("--overflow-song", type=int, action="append", default=[])
    args = parser.parse_args()
    try:
        if args.force and args.command != "extract":
            raise ValueError("--force is only valid with extract")
        rom = original_rom(args.rom)
        if args.command == "extract":
            extract(rom, args.assets, args.force)
        elif args.command == "build":
            build(rom, args.assets, args.output, args.overflow_song)
        else:
            roundtrip(rom, args.assets, args.output)
    except (OSError, ValueError, KeyError, struct.error) as error:
        parser.exit(1, f"error: {error}\n")


if __name__ == "__main__":
    main()
