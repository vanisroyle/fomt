"""Code-derived roots for non-script text, not a ROM string scan."""

from pathlib import Path
import re


ITEMS = (("tools", 0xEAB0C, 81, 12, 8),
         ("foods", 0xEDCD8, 171, 16, 12),
         ("articles", 0xEFED4, 95, 12, 8))
ARRAYS = (("settings", 0xE8704, 18), ("race_help", 0xFAFC8, 10))
FIXED = (("name_entry", 0xE89B6, 18, 31),)
POINTERS = (("npc_names", 0x104258, 43, 8),
            ("fishing_names", 0x103A18, 59, 4),
            ("wrapped_item_suffix", 0x117AA4, 1, 4))
LIBRARY = (0x10828C, 137)
SHOPS = ((0xFD988, 5), (0xFFB90, 11))
GLYPHS = (("frisbee", 0xFA12E, 13), ("race", 0xFAE6A, 24),
          ("medals", 0xFB1A6, 24), ("animals", 0xFB966, 10),
          ("calendar", 0xFBD30, 43), ("report", 0xFC082, 10),
          ("inventory", 0x1177F6, 10))
BYTE_ROWS = (("book_tabs", 0xFD940, 2, 12), ("recipe_heading", 0x101008, 1, 23),
             ("recipe_fridge", 0x101048, 1, 17), ("utensils", 0x1010B1, 5, 11))
ROOTS = tuple(int(value, 16) for value in """
0E86C0 0E8770 0E877C 0E878C 0E87A4 0E87FC 0E8828 0E8878
0E8958 0E8968 0E8978 0E8988 0E8997 0E8998 0E89A4 0E89A8
0E8BE4 0E8BF4 0E8BFC 0E8C14 0E8C2C 0E8C44 0E8C48
0E9620 0E962C 0E963C 0E9648 0E9658
0FA158 0FAE9C 0FAEB8 0FAEC4
0FB108 0FB120 0FB124 0FB128 0FB140 0FB158 0FB164 0FB17C
0FB830 0FB834 0FB838 0FB844 0FB850 0FB85C 0FB864 0FB86C
0FB874 0FB87C 0FB880 0FB894 0FB8A4 0FB8C8 0FB8CC 0FB8D4
0FB8E0 0FB904 0FB908 0FB90C 0FB914 0FB91C 0FB93C 0FB944
0FB950 0FB97C 0FB988 0FB994 0FB9A0 0FB9A8 0FB9B0 0FB9B8
0FB9C0 0FB9C4 0FBE8C 0FBE9C 0FBEA4 0FBEB4 0FBEC0 0FBEC8
0FBECC 0FBEE0 0FBEFC 0FBF08 0FBF1C 0FBF28 0FBF34 0FBF3C
0FBF58 0FBF68 0FBF74 0FBF80 0FBF84 0FBFA0 0FBFAC 0FBFB4
0FBFBC 0FBFC4 0FBFC8 0FBFCC 0FBFE8 0FBFF0 0FC008
103658 11780C
0E890C 0E8914 0F05B8 0F05C0 0F05C8 0F19A0
0FA3B8 0FA3BC 0FA660 0FA6A0 0FA6E0 0FAAFC 0FAB94 0FABE0
0FAC24 0FAC8C 0FAD08 0FAD20 0FAD28 0FAD94 0FADDC 0FAE00
0FB234 0FB23C 0FB27C 0FB284 0FB2D0 0FB300 0FB398 0FB3CC
0FB420 0FB46C 0FB484 0FB51C 0FB520 0FB53C 0FB544 0FB578
0FB634 0FB6C0 0FB738 0FB754 0FB7A0 0FB7F0 0FB9DC 0FB9E0
0FB9E4 0FB9F4 0FBC9C 0FBD18 0FBD28 0FBD2C 0FDBF8 0FDBFC
0FF228 0FF230 0FFACC 0FFAD4 0FFAE0 0FFAE8 0FFEAC 0FFEB0
100084 100088 1000A8 1000AC 100120 100124 1003DC 1003E0
100420 100430 100434 100588 10058C 100590 10073C
100E34 100E48 100E5C 100E74 100E90 100EA4 100EBC 100ED8
100F30 100F44 100F78 100F90 100FA4 100FBC 100FCC
1077E4 1077F8 107808 107A30 107A4C 107A58 107DD8 107DF4
107DF8 107F18 107F2C 107F3C 1081D8 1081E4 1081F4
0E874C 0E8754 0E8758 0E8760 0E8764 0E8768
0F0654 0F0658 0F065C 0F0660 0F067C 0F0684 0F068C 0F069C
0F06B4 0F06D0 0FB824 0FBED4 0FC6CC 0FC6D4 0FC6E8
0FD964 0FD9EC 0FD9F0 0FD9FC 0FDA10 0FDA20 0FDA24 0FDA40
0FDA48 0FDA50 0FDA58 0FDA60 0FDA78 0FDAD4 0FDBE8 0FDE70
0FE0BC 0FE540 0FE7A0 0FE9CC 0FF838 0FF83C 0FFB60 0FFC6C
0FFC70 0FFC80 0FFC90 0FFCA0 0FFCA4 0FFCC0 0FFCC8 0FFCD0
0FFCD8 0FFCE0 0FFCF4 0FFCF8 0FFD08 0FFD24 0FFD90 0FFE9C
0FFEA8 0FFFE8 0FFFFC 100350 100360 100368 10037C 100388
100398 10039C 100E28 104938 104940 104948 104950 107AC4
1179AC 1179BC 1179C4 1179C8 1179CC 1179D8 1179E0 1179F8
1179FC 117A14 117A28 117A38 117A44 117A50 117A5C 117A78
117A84 101020 10102C 101038
""".split())


def symbol_address(name, symbols):
    if re.fullmatch(r"gUnk_08[0-9A-Fa-f]{6}", name):
        return int(name[5:], 16) - 0x08000000
    return symbols.get(name)


def roots(repo):
    symbols, result = {}, {}
    for path in (repo / "asm/data").glob("*.s"):
        for name, offset in re.findall(r'(\w+):\s*\n\s*\.incbin "baserom.gba", (0x[0-9A-Fa-f]+)', path.read_text()):
            symbols[name] = int(offset, 16)
    for path in sorted((repo / "asm").glob("*.s")):
        lines = path.read_text().splitlines()
        literals = dict(re.findall(r'(\.L\w+):\s*\.(?:4byte|word)\s+(\w+)', "\n".join(lines)))

        def value(register, before, budget=100):
            for j in range(before - 1, max(-1, before - budget - 1), -1):
                line = lines[j].split("@")[0].strip()
                if line.startswith("bl ") and register in ("r0", "r1", "r2", "r3"):
                    return None
                match = re.match(r"(\w+)\s+" + register + r",\s*(.*)", line)
                if not match:
                    continue
                op, rhs = match.groups()
                if op in ("str", "strh", "strb", "cmp", "tst"):
                    continue
                if op == "ldr" and rhs in literals:
                    return literals[rhs]
                if op in ("mov", "movs") and re.fullmatch(r"(?:r\d+|sb|sl|fp|ip)", rhs):
                    return value(rhs, j, budget - before + j)
                if op == "adds" and rhs.endswith(", #0"):
                    return value(rhs[:-4], j, budget - before + j)
                return None
            return None

        for i, line in enumerate(lines):
            if re.search(r"\bbl strlen\s*$", line):
                name = value("r0", i)
                address = symbol_address(name or "", symbols)
                if address is not None:
                    result.setdefault(address, []).append(f"{path.relative_to(repo)}:{i + 1}")
            if re.search(r"\bbl (?:SetMessageWindowTextSubstitutions|SetMessageWindowSpeakerName|ConfigureMessageWindowPromptChoices|ConfigureMessageWindowChoices)\s*$", line):
                registers = ("r1", "r2", "r3") if "Choices" in line else ("r1",)
                for register in registers:
                    name = value(register, i)
                    address = symbol_address(name or "", symbols)
                    if address is not None:
                        result.setdefault(address, []).append(f"{path.relative_to(repo)}:{i + 1}")
            if not re.search(r"\bbl DrawMessageTileString(?:WithColors)?\s*$", line):
                continue
            for j in range(i - 1, max(-1, i - 40), -1):
                if re.search(r"\bbl ", lines[j]):
                    break
                match = re.search(r"\bstr\s+(\w+), \[sp(?:, #0)?\]", lines[j])
                if match:
                    name = value(match[1], j)
                    address = symbol_address(name or "", symbols)
                    if address is not None:
                        result.setdefault(address, []).append(f"{path.relative_to(repo)}:{i + 1}")
                    break
    for module in ("new_game", "livestock_report", "medal_exchange", "animal"):
        path = repo / "src" / (module + ".cc")
        source = path.read_text()
        names = re.findall(r'(?:extern|EC) char(?: const)?\s+(\w+)\[\](?:\s+__asm__\("(\w+)"\))?', source)
        for name, alias in names:
            address = symbol_address(alias or name, symbols)
            if address is not None:
                result.setdefault(address, []).append(str(path.relative_to(repo)))
    return result


if __name__ == "__main__":
    for address, sites in sorted(roots(Path.cwd()).items()):
        print(f"{address:06X}", ", ".join(sites))
