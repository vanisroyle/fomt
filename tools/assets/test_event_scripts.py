"""Synthetic event bytecode only."""

import hashlib
import json
from pathlib import Path
import re
import tempfile
import unittest
from unittest.mock import patch

import event_scripts as evt
import fomt_assets as fa


class EventTests(unittest.TestCase):
    def test_all_opcodes_and_flag_bits(self):
        jump = fa.pack(1) + fa.pack(4) + fa.pack(0) + fa.pack(0xFFFFFFFF)
        code = bytearray()
        for flag in (0, 0x80):
            for op in range(37):
                code.append(op | flag)
                code.extend(bytes(evt.WIDTHS.get(op, 0)))
        code.extend(b"\x7f\xff")
        source = evt.disassemble(code, jump)
        self.assertIn("OP_7F.BASE", source)
        self.assertEqual(evt.assemble(source), (bytes(code), jump))

    def test_handler_widths_match_cpp(self):
        source = Path(__file__).resolve().parents[2] / "src/script_engine.cc"
        body = source.read_text().split("int AScriptEngine::NextInstruction()", 1)[1].split("char const * AScriptEngine::GetString", 1)[0]
        handlers = re.split(r"case (0x[0-9A-F]+):", body)[1:]
        widths = {}
        for key, block in zip(handlers[::2], handlers[1::2]):
            operand = re.search(r"Operand(8|16|32)\(pc\)", block)
            widths[int(key, 16)] = int(operand[1]) // 8 if operand else 0
        self.assertEqual(set(widths), set(range(37)))
        self.assertEqual({op: width for op, width in widths.items() if width}, evt.WIDTHS)

    def test_labels_repoint_code_and_jump_when_inserting_instructions(self):
        source = ".code\nEVT_BRANCH done\nstart:\nEVT_NOP\ndone:\nEVT_END\n.jump\n.word 1, table\ntable:\n.word 1, done\n.case -7, start\n"
        before, jump = evt.assemble(source)
        after, moved = evt.assemble(source.replace("done:\n", "EVT_NOP\ndone:\n"))
        self.assertEqual(fa.word(before, 1), 6)
        self.assertEqual(fa.word(after, 1), 7)
        self.assertEqual(fa.word(jump, 12), 6)
        self.assertEqual(fa.word(moved, 12), 7)
        self.assertEqual(fa.word(moved, 20), 5)
        self.assertEqual(evt.assemble(evt.disassemble(after, moved)), (after, moved))

    def test_empty_code_absent_and_empty_jump(self):
        self.assertEqual(evt.assemble(".code\n"), (b"", None))
        self.assertEqual(evt.assemble(".code\n.jump\n.word 0\n"), (b"", fa.pack(0)))

    def test_noncanonical_jump_aliases_padding_and_defaults(self):
        jump = b"".join(fa.pack(v) for v in (2, 12, 12, 0xAABBCCDD, 1, 1, 0xFFFFFFFF, 0, 0x11223344))
        self.assertEqual(evt.assemble(evt.disassemble(b"\x00\x20", jump)), (b"\x00\x20", jump))

    def test_string_ids_are_only_marked_for_known_consumers(self):
        for call, count in evt.STRING_CALLS.items():
            code = b"\x23\x00" * (count + 1) + b"\x21" + fa.pack(call) + b"\x20"
            source = evt.disassemble(code, string_count=1)
            self.assertEqual(source.count("STR_0"), count)
            self.assertEqual(evt.assemble(source, 1), (code, None))
        self.assertNotIn("STR_0", evt.disassemble(b"\x23\x00\x21" + fa.pack(1), string_count=1))
        self.assertEqual(evt.assemble(".code\nEVT_PUSH8 STR_1\n", 2)[0], b"\x23\x01")

    def test_errors_have_source_locations(self):
        for source, error in (("EVT_END", "expected"), (".code\nBAD", "unknown opcode"),
                              (".code\nEVT_PUSH8 256", "8 bits"), (".code\nEVT_PUSH16 -32769", "16 bits"),
                              (".code\nEVT_NOP 1", "operand"), (".code\nEVT_BRANCH missing", "undefined label"),
                              (".code\na:\na:", "duplicate label"), (".code\nEVT_PUSH8 STR_2", "string reference"),
                              (".code\nEVT_PUSH8.BASE STR_0", "dynamic"), (".code\n.code", "duplicate section"),
                              (".code\nEVT_PUSH32 table\n.jump\ntable:\n.word 0", "JUMP offset label")):
            with self.subTest(source=source), self.assertRaisesRegex(ValueError, "fixture.evt:[0-9]+:.*" + error):
                evt.assemble(source, 1, "fixture.evt")

    def test_invalid_binary_is_rejected(self):
        for code, jump in ((b"\x23", None), (b"\x18\x01\0\0\0", None),
                           (b"\x24\0\0\0\0", None), (b"", b""), (b"", fa.pack(1)),
                           (b"", fa.pack(0) + b"x"), (b"", fa.pack(1) + fa.pack(0)),
                           (b"", fa.pack(1) + fa.pack(5) + bytes(20)),
                           (b"\x20", b"".join(fa.pack(v) for v in (1, 4, 2, 0xFFFFFFFF, 1, 0, 0, 0)))):
            with self.subTest(code=code, jump=jump), self.assertRaises(ValueError):
                evt.disassemble(code, jump)

    def test_container_evt_precedence_raw_fallback_and_chunk_mismatch(self):
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            (root / "scripts").mkdir()
            (root / "text").mkdir()
            metadata = {"version": 1, "id": 1, "chunks": [
                {"tag": b"CODE".hex(), "tail": "abcd"},
                {"tag": b"STR ".hex(), "payload": "00000000"}]}
            (root / "scripts/0001.json").write_text(json.dumps(metadata))
            (root / "scripts/0001.code.bin").write_bytes(b"\x20")
            (root / "text/0001.txt").write_bytes(b"")
            original = fa.build_script(root, 1, {})
            path = root / "scripts/0001.evt"
            path.write_text(".code\nEVT_NOP\nEVT_END\n")
            rebuilt = dict(fa.chunks(fa.build_script(root, 1, {})))
            self.assertEqual(rebuilt[b"CODE"], fa.pack(2) + b"\x00\x20\xab")
            str_offset = lambda data: data.index(b"STR ")
            self.assertEqual(str_offset(fa.build_script(root, 1, {})) % 4, str_offset(original) % 4)
            path.write_text(".code\nEVT_NOP\nEVT_NOP\nEVT_NOP\nEVT_NOP\nEVT_END\n")
            self.assertEqual(str_offset(fa.build_script(root, 1, {})) % 4, str_offset(original) % 4)
            path.write_text(".code\nEVT_NOP\nEVT_END\n")
            path.write_text(".code\n.jump\n.word 0\n")
            with self.assertRaisesRegex(ValueError, "presence must match"):
                fa.build_script(root, 1, {})
            path.unlink()
            self.assertEqual(fa.build_script(root, 1, {}), original)
            metadata["chunks"].append({"tag": b"JUMP".hex()})
            (root / "scripts/0001.json").write_text(json.dumps(metadata))
            (root / "scripts/0001.jump.bin").write_bytes(fa.pack(0))
            path.write_text(".code\ndone:\nEVT_END\n.jump\n.word 1, table\ntable:\n.word 1, -1\n.case 0, done\n")
            rebuilt = dict(fa.chunks(fa.build_script(root, 1, {})))
            self.assertEqual(fa.word(rebuilt[b"JUMP"]), 1)
            path.unlink()
            self.assertEqual(dict(fa.chunks(fa.build_script(root, 1, {})))[b"JUMP"], fa.pack(0))

    def test_extraction_preserves_raw_edits_and_existing_evt(self):
        code = fa.pack(1) + b"\x20"
        body = b"CODE" + fa.pack(len(code)) + code + b"STR " + fa.pack(4) + fa.pack(0)
        data = b"RIFF" + fa.pack(12 + len(body)) + b"SCR " + body
        with tempfile.TemporaryDirectory() as directory:
            root = Path(directory)
            rom = root / "rom"
            rom.write_bytes(data)
            assets = root / "assets"
            (assets / "scripts").mkdir(parents=True)
            (assets / "scripts/0001.code.bin").write_bytes(b"\x00\x20")
            with patch.object(fa, "SHA1", hashlib.sha1(data).hexdigest()), patch.object(fa, "rom_scripts", return_value=[(1, 0, data)]):
                fa.extract(rom, assets)
                path = assets / "scripts/0001.evt"
                self.assertEqual(evt.assemble(path.read_text())[0], b"\x00\x20")
                path.write_text(".code\nEVT_END\n")
                fa.extract(rom, assets)
                self.assertEqual(path.read_text(), ".code\nEVT_END\n")


if __name__ == "__main__":
    unittest.main()
