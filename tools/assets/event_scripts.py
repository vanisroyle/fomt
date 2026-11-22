"""Lossless AScriptEngine CODE/JUMP assembly; standard library only."""

import re


NAMES = (
    "NOP STORE_IND ADD_STORE SUB_STORE MUL_STORE DIV_STORE MOD_STORE "
    "ADD SUB MUL DIV MOD AND OR INC DEC NEG NOT CMP LOAD STORE DUP POP "
    "PUSH32 BRANCH BRANCH_LT BRANCH_LE BRANCH_EQ BRANCH_NE BRANCH_GE "
    "BRANCH_GT BRANCH_STACK END CALL PUSH16 PUSH8 SWITCH"
).split()
OPCODES = {"EVT_" + name: i for i, name in enumerate(NAMES)}
WIDTHS = {0x13: 4, 0x14: 4, 0x17: 4, **{i: 4 for i in range(0x18, 0x1F)},
          0x21: 4, 0x22: 2, 0x23: 1, 0x24: 4}
BRANCHES = range(0x18, 0x1F)
PUSHES = (0x17, 0x22, 0x23)
STRING_CALLS = {33: 1, 34: 1, 35: 1, 36: 3, 37: 4, 38: 5,
                39: 2, 40: 3, 41: 4, 42: 5, 43: 6, 45: 1,
                58: 1, 251: 1, 293: 1}
IDENT = re.compile(r"[A-Za-z_][A-Za-z_0-9]*\Z")


def word(data, pos=0):
    if pos < 0 or pos + 4 > len(data):
        raise ValueError("truncated JUMP word")
    return int.from_bytes(data[pos:pos + 4], "little")


def signed(value):
    return value if value < 0x80000000 else value - 0x100000000


def instructions(code):
    result = []
    pos = 0
    while pos < len(code):
        byte = code[pos]
        op, flag = byte & 0x7F, byte >> 7
        width = WIDTHS.get(op, 0)
        if pos + 1 + width > len(code):
            raise ValueError(f"CODE {pos:#x}: truncated operand")
        value = int.from_bytes(code[pos + 1:pos + 1 + width], "little") if width else None
        result.append((pos, op, flag, value))
        pos += 1 + width
    return result


def jump_tables(jump):
    if jump is None:
        return []
    if len(jump) % 4:
        raise ValueError("JUMP must contain whole words")
    count = word(jump)
    if count > (len(jump) - 4) // 4:
        raise ValueError("truncated JUMP offset table")
    tables = []
    for i in range(count):
        offset = word(jump, 4 + i * 4)
        if offset % 4 or offset < count * 4:
            raise ValueError("JUMP table offset is unaligned or inside offset array")
        start = 4 + offset
        size, default = word(jump, start), word(jump, start + 4)
        if size > (len(jump) - start - 8) // 8:
            raise ValueError("truncated JUMP cases")
        cases = [(signed(word(jump, start + 8 + j * 8)), word(jump, start + 12 + j * 8)) for j in range(size)]
        if any(a[0] > b[0] for a, b in zip(cases, cases[1:])):
            raise ValueError("JUMP case values must be sorted (signed)")
        tables.append((start, default, cases))
    return tables


def validate(code, jump):
    decoded = instructions(code)
    starts = {pos for pos, _, _, _ in decoded}
    tables = jump_tables(jump)
    targets = set()
    for pos, op, flag, value in decoded:
        if not flag and op in BRANCHES:
            targets.add(value)
        if not flag and op == 0x24 and value >= len(tables):
            raise ValueError(f"CODE {pos:#x}: switch table ID {value} is unavailable")
    for _, default, cases in tables:
        if signed(default) > 0:
            targets.add(default)
        targets.update(target for _, target in cases)
    if not targets <= starts:
        raise ValueError(f"branch targets are not CODE instruction boundaries: {sorted(targets - starts)}")
    return decoded, tables, targets


def disassemble(code, jump=None, string_count=0):
    decoded, tables, targets = validate(code, jump)
    label = lambda value: f"L_{value:06X}"
    string_positions = set()
    for index, (_, op, flag, call) in enumerate(decoded):
        if op != 0x21 or flag or call not in STRING_CALLS:
            continue
        for pos, push, push_flag, value in reversed(decoded[max(0, index - STRING_CALLS[call]):index]):
            if push not in PUSHES or push_flag:
                break
            if value < string_count:
                string_positions.add(pos)
    lines = ["# AScriptEngine event source; strings are in assets/text/NNNN.txt", ".code"]
    for pos, op, flag, value in decoded:
        if pos in targets:
            lines.append(label(pos) + ":")
        name = "EVT_" + NAMES[op] if op < len(NAMES) else f"OP_{op:02X}"
        if flag:
            name += ".BASE"
        if value is not None:
            operand = str(value)
            if op in BRANCHES and not flag:
                operand = label(value)
            elif pos in string_positions:
                operand = f"STR_{value}"
            name += " " + operand
        lines.append("    " + name)
    if jump is not None:
        lines.extend(["", ".jump", f"    .word {len(tables)} # table count"])
        labels, fields, cases_at = {}, {}, {}
        for i, (start, _, _) in enumerate(tables):
            labels.setdefault(start, f"JT_{i:04d}")
        for i, (start, default, cases) in enumerate(tables):
            fields[4 + i * 4] = labels[start] + " # offset from JUMP payload + 4"
            fields[start] = f"{len(cases)} # case count"
            fields[start + 4] = (label(default) if signed(default) > 0 else str(signed(default))) + " # default; <= 0 falls through"
            for j, (value, target) in enumerate(cases):
                cases_at[start + 8 + j * 8] = (value, target)
                fields[start + 8 + j * 8] = str(value) + " # case value"
                fields[start + 12 + j * 8] = label(target) + " # case target"
        pos = 4
        while pos < len(jump):
            if pos in labels:
                lines.append(labels[pos] + ":")
            if pos in cases_at and pos + 4 not in labels:
                value, target = cases_at[pos]
                lines.append(f"    .case {value}, {label(target)}")
                pos += 8
            else:
                lines.append("    .word " + fields.get(pos, f"0x{word(jump, pos):08X}"))
                pos += 4
    return "\n".join(lines) + "\n"


def assemble(source, string_count=0, filename="<event>"):
    records, labels, sizes, seen = [], {}, {"code": 0, "jump": 0}, set()
    section = None
    for number, original in enumerate(source.lstrip("\ufeff").splitlines(), 1):
        line = original.split("#", 1)[0].strip()
        if not line:
            continue
        location = f"{filename}:{number}"
        if line in (".code", ".jump"):
            section = line[1:]
            if section in seen:
                raise ValueError(f"{location}: duplicate section")
            seen.add(section)
            continue
        if section is None:
            raise ValueError(f"{location}: expected .code or .jump")
        if line.endswith(":"):
            name = line[:-1]
            if not IDENT.fullmatch(name) or name in labels or re.fullmatch(r"STR_\d+", name):
                raise ValueError(f"{location}: invalid or duplicate label {name}")
            labels[name] = (section, sizes[section] - (4 if section == "jump" else 0))
            continue
        parts = line.split(None, 1)
        name = parts[0]
        args = [arg.strip() for arg in parts[1].split(",")] if len(parts) == 2 else []
        op, flag = None, name.endswith(".BASE")
        if section == "code":
            mnemonic = name[:-5] if flag else name
            op = OPCODES.get(mnemonic)
            if op is None and re.fullmatch(r"OP_[0-7][0-9A-F]", mnemonic):
                op = int(mnemonic[3:], 16)
            if op is None:
                raise ValueError(f"{location}: unknown opcode {name}")
            width = WIDTHS.get(op, 0)
            if len(args) != bool(width):
                raise ValueError(f"{location}: {name} needs {int(bool(width))} operand(s)")
            length = 1 + width
        else:
            if name not in (".word", ".case") or not args or (name == ".case" and len(args) != 2):
                raise ValueError(f"{location}: expected .word value(s) or .case value, target")
            length = 4 * len(args)
        records.append((section, op, flag, args, location))
        sizes[section] += length
    if "code" not in seen:
        raise ValueError(f"{filename}: missing .code section")

    def resolve(token, section, op, location):
        if re.fullmatch(r"STR_\d+", token):
            value = int(token[4:])
            if section != "code" or op not in PUSHES or not 0 <= value < string_count:
                raise ValueError(f"{location}: unavailable string reference {token}")
            return value
        if token in labels:
            kind, value = labels[token]
            if section == "code" and kind != "code":
                raise ValueError(f"{location}: instruction operand cannot use a JUMP offset label")
            return value
        try:
            return int(token, 0) if not token.isdecimal() else int(token)
        except ValueError:
            raise ValueError(f"{location}: undefined label or invalid integer {token}") from None

    output = {"code": bytearray(), "jump": bytearray()}
    for section, op, flag, args, location in records:
        target = output[section]
        width = WIDTHS.get(op, 0) if section == "code" else 4
        if section == "code":
            target.append(op | (0x80 if flag else 0))
        for arg in args:
            if flag and arg.startswith("STR_"):
                raise ValueError(f"{location}: .BASE string references are dynamic; use a numeric operand")
            value = resolve(arg, section, op, location)
            if not -(1 << (width * 8 - 1)) <= value < (1 << (width * 8)):
                raise ValueError(f"{location}: operand does not fit {width * 8} bits")
            target.extend((value & ((1 << (width * 8)) - 1)).to_bytes(width, "little"))
    code = bytes(output["code"])
    jump = bytes(output["jump"]) if "jump" in seen else None
    validate(code, jump)
    return code, jump
