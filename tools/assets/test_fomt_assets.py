"""Synthetic fixtures only: no ROM-derived data."""

import json
from pathlib import Path
import tempfile
import unittest

import fomt_assets as fa


class AssetTests(unittest.TestCase):
    def setUp(self):
        self.temp = tempfile.TemporaryDirectory()
        self.addCleanup(self.temp.cleanup)
        self.root = Path(self.temp.name)

    def text(self, entries):
        path = self.root / "text.txt"
        path.write_text(fa.text_file(1, fa.BASE, entries), encoding="utf-8")
        return path

    def test_text_roundtrip(self):
        entries = [b"", b"same", b"same", b"\r\n", b"x\r\n\r\n",
                   b"{braces}|=== 2\r\n#comment\x05\x0c\xff\x21\r\n",
                   "あいうえお".encode("cp932"), b"\r\n\r\nend"]
        self.assertEqual(fa.read_text(self.text(entries), {}), entries)

    def test_every_nonzero_byte(self):
        entries = [bytes([i]) for i in range(1, 256)]
        self.assertEqual(fa.read_text(self.text(entries), {}), entries)
        data = bytes(range(1, 256))
        self.assertEqual(fa.read_text(self.text([data]), {}), [data])

    def test_empty_table(self):
        self.assertEqual(fa.read_text(self.text([]), {}), [])

    def test_lowercase_hex_tokens(self):
        self.assertEqual(fa.encode("{ff2a}{7b}{0d}", {}, "test", 1), b"\xff\x2a{\r")

    def test_file_newline_is_not_game_newline(self):
        path = self.text([b"a"])
        self.assertEqual(fa.read_text(path, {}), [b"a"])
        path.write_text(path.read_text().rstrip("\n"), encoding="utf-8")
        self.assertEqual(fa.read_text(path, {}), [b"a"])

    def test_encoding_error_has_location(self):
        path = self.text([b"ok"])
        path.write_text("# comment\n=== 0\n|ạ\n", encoding="utf-8")
        with self.assertRaisesRegex(ValueError, r"text.txt:3: cannot encode.*U\+1EA1"):
            fa.read_text(path, {})
        self.assertEqual(fa.read_text(path, {"ạ": b"\x81\x40"}), [b"\x81\x40"])

    def test_bad_text(self):
        for text in ("=== 1\n|a\n", "=== 0\n", "=== 0\n|{NO}\n",
                     "=== 0\n|{00}\n", "=== 0\n|a}\n", "=== 0\nplain\n"):
            path = self.root / "bad.txt"
            path.write_text(text, encoding="utf-8")
            with self.subTest(text=text), self.assertRaisesRegex(ValueError, r"bad.txt:\d+:"):
                fa.read_text(path, {})

    def test_charmap(self):
        path = self.root / "charmap.txt"
        path.write_text("# map\nU+1EA1 = 81 40 # example\n", encoding="utf-8")
        self.assertEqual(fa.charmap(path), {"ạ": b"\x81\x40"})
        for text in ("U+1EA1 = 00", "bad", "U+FFFFFF = 81", "U+0041 = 41\nU+0041 = 42"):
            path.write_text(text, encoding="utf-8")
            with self.subTest(text=text), self.assertRaises(ValueError):
                fa.charmap(path)

    def fixture(self):
        scripts = self.root / "scripts"
        scripts.mkdir()
        (self.root / "text").mkdir()
        # Noncanonical string offsets, duplicate IDs, unreachable pool bytes,
        # an unknown chunk, odd chunk lengths, and a CODE trailer.
        payload = fa.pack(3) + fa.pack(2) + fa.pack(0) + fa.pack(2) + b"b\0a\0unused"
        meta = {"version": 1, "id": 1, "chunks": [
            {"tag": b"STR ".hex(), "payload": payload.hex()},
            {"tag": b"JUMP".hex()},
            {"tag": b"ODD!".hex(), "payload": "01"},
            {"tag": b"CODE".hex(), "tail": "ff"}]}
        (scripts / "0001.json").write_text(json.dumps(meta))
        (scripts / "0001.code.bin").write_bytes(b"\x01\x02")
        (scripts / "0001.jump.bin").write_bytes(fa.pack(0))
        (self.root / "text/0001.txt").write_text(fa.text_file(1, fa.BASE, fa.strings(payload)))
        return payload

    def test_container_quirks_and_edit(self):
        payload = self.fixture()
        original = fa.build_script(self.root, 1, {})
        parts = fa.chunks(original)
        self.assertEqual(parts[0], (b"STR ", payload))
        self.assertEqual(parts[2], (b"ODD!", b"\x01"))
        self.assertEqual(parts[3], (b"CODE", fa.pack(2) + b"\x01\x02\xff"))
        text = self.root / "text/0001.txt"
        text.write_text(fa.text_file(1, fa.BASE, [b"longer", b"", b"longer"]))
        edited = fa.build_script(self.root, 1, {})
        self.assertEqual(fa.strings(fa.chunks(edited)[0][1]), [b"longer", b"", b"longer"])
        self.assertEqual(fa.chunks(edited)[1:], parts[1:])
        text.write_text(fa.text_file(1, fa.BASE, [b"missing IDs"]))
        with self.assertRaisesRegex(ValueError, "preserve all 3 string IDs"):
            fa.build_script(self.root, 1, {})

    def test_malformed_binary(self):
        for data in (b"", b"RIFF" + fa.pack(12) + b"SCR ",
                     b"RIFF" + fa.pack(20) + b"SCR CODE" + fa.pack(10)):
            with self.subTest(data=data), self.assertRaises(ValueError):
                fa.chunks(data)
        for data in (b"", fa.pack(2), fa.pack(1) + fa.pack(0) + b"no NUL"):
            with self.subTest(data=data), self.assertRaises(ValueError):
                fa.strings(data)

    def test_extraction_write_protection(self):
        path = self.root / "asset"
        fa.write_missing(path, b"first")
        fa.write_missing(path, b"second")
        self.assertEqual(path.read_bytes(), b"first")
        fa.write_missing(path, b"second", force=True)
        self.assertEqual(path.read_bytes(), b"second")


if __name__ == "__main__":
    unittest.main()
