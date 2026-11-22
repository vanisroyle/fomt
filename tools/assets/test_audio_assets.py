"""Synthetic M4A control-flow, assembly, PCM and relocation tests (no ROM)."""

import json
from pathlib import Path
import struct
import tempfile
import unittest
from unittest.mock import patch

import audio_assets as audio


class AudioTests(unittest.TestCase):
    def test_assembly_forward_symbols_running_bytes_and_alignment(self):
        source = "start:\n.byte KEYSH, -2\n.byte N06, Cn3, v100\n.byte 62, 90\n.word target\n.balign 4\ntarget:\n.byte FINE\n"
        raw, labels, refs = audio.assemble(source, 0x08001000)
        self.assertEqual(raw[:7], bytes((0xBC, 254, 0xD5, 60, 100, 62, 90)))
        self.assertEqual(audio.u32(raw, 7), 0x0800100C)
        self.assertEqual(labels["target"], 12)
        self.assertEqual(refs, {7: "target"})
        self.assertEqual(audio.assemble(source, 0, measure=True), (13, labels))
        raw, _, refs = audio.assemble(".word gAudioSample_00000000\n", 0, {"gAudioSample_00000000": 0x08000000})
        self.assertEqual(refs, {0: "gAudioSample_00000000"})

    def test_reassigned_constant_applies_only_to_later_operands(self):
        raw, _, _ = audio.assemble(".set vel, 100\n.byte vel\n.set vel, 40\n.byte vel\n", 0)
        self.assertEqual(raw, bytes((100, 40)))

    def test_assembly_errors_and_empty(self):
        self.assertEqual(audio.assemble("@ empty", 0), (b"", {}, {}))
        for text in (".byte 256", ".byte missing", ".incbin \"secret\"", ".include \"other.inc\"", "x:\nx:\n", ".balign 3", ".byte __import__('os')"):
            with self.subTest(text=text), self.assertRaises(ValueError):
                audio.assemble(text, 0)

    def test_constants_match_gas_include(self):
        constants = {}
        for line in Path("tools/assets/m4a_commands.inc").read_text().splitlines():
            if line.startswith(".equiv "):
                name, number = line[7:].split(",")
                constants[name] = int(number)
        self.assertEqual(constants, audio.CONSTANTS)

    def test_pattern_return_running_status_and_loop(self):
        # VOICE 5; PATT pattern; running N06 key 62; GOTO start;
        raw = bytearray((0xBD, 5, 0xB3)) + struct.pack("<I", audio.ROM_BASE + 14)
        raw += bytes((62, 90, 0xB2)) + struct.pack("<I", audio.ROM_BASE)
        raw += bytes((0xD5, 60, 100, 0xB4))
        parsed = audio.sequences(raw, [0], 0, len(raw))
        self.assertEqual(parsed["notes"], {(5, 60), (5, 62)})
        self.assertEqual(parsed["pointers"], {3: 14, 10: 0})

    def test_memacc_conditional_and_repeat(self):
        raw = bytes((0xB9, 6, 0, 0)) + struct.pack("<I", audio.ROM_BASE + 9) + bytes((0xB1, 0xB5, 2)) + struct.pack("<I", audio.ROM_BASE + 8) + bytes((0xB1,))
        parsed = audio.sequences(raw, [0], 0, len(raw))
        self.assertEqual(parsed["pointers"], {4: 9, 11: 8})
        self.assertIn(15, parsed["events"])

    def test_sequence_errors(self):
        for raw in (b"", b"\x01", b"\xb3\0", b"\xbb", b"\xb2\0\0\0\0", b"\xcd\xff", b"\xb6"):
            with self.subTest(raw=raw), self.assertRaises(ValueError):
                audio.sequences(raw, [0], 0, len(raw))
        raw = bytes((0xB3,)) + struct.pack("<I", audio.ROM_BASE)
        with self.assertRaisesRegex(ValueError, "three levels"):
            audio.sequences(raw, [0], 0, len(raw))

    def test_wav_identity_signed_samples_fractional_frequency_and_rate_edit(self):
        original = {"rate": 8000, "frequency": 8192007}
        meta = dict(kind=0, status=0x4000, frequency=8192007, loop_start=1)
        pcm = bytes((0, 127, 128, 255))
        rebuilt = audio.rebuild_sample(audio.wav_bytes(pcm, 8000), meta, original)
        self.assertEqual(rebuilt, struct.pack("<HHIII", 0, 0x4000, 8192007, 1, 4) + pcm)
        edited = audio.rebuild_sample(audio.wav_bytes(pcm, 16000), meta, original)
        self.assertEqual(audio.u32(edited, 4), 16000 * 1024)
        self.assertEqual(audio.u32(edited, 8), 1)

    def test_wav_empty_truncated_invalid_metadata_and_loops(self):
        original = {"rate": 8000, "frequency": 8192000}
        meta = dict(kind=0, status=0x4000, frequency=8192000, loop_start=0)
        wav = audio.wav_bytes(b"abcd", 8000)
        for data in (b"", b"not a WAV", audio.wav_bytes(b"", 8000), wav[:-1]):
            with self.subTest(data=data[:8]), self.assertRaises(ValueError):
                audio.rebuild_sample(data, meta, original)
        for changes in ({"loop_start": 4}, {"loop_start": -1}, {"kind": 1}, {"status": 1}, {"frequency": 0}, {"extra": 0}):
            with self.subTest(changes=changes), self.assertRaises(ValueError):
                audio.rebuild_sample(wav, dict(meta, **changes), original)

    def test_relocation_audit_handles_unaligned_references_and_pcm(self):
        rom = bytearray(128)
        struct.pack_into("<I", rom, 3, audio.ROM_BASE + 64)
        model = {"refs": {}, "samples": []}
        with self.assertRaisesRegex(ValueError, "non-symbolic reference at 0x3"):
            audio.audit_relocations(rom, [(64, 70)], model)
        model["refs"][3] = 64
        audio.audit_relocations(rom, [(64, 70)], model)
        model = {"refs": {}, "samples": [{"start": 0, "count": 16}]}
        rom[3:7] = b"\0" * 4
        struct.pack_into("<I", rom, 19, audio.ROM_BASE + 65)
        audio.audit_relocations(rom, [(64, 70)], model)

    def test_synthetic_pipeline_preserves_edits_and_relocates_song_and_sample(self):
        with tempfile.TemporaryDirectory() as directory, patch.multiple(audio, VOICE_START=0, VOICE_END=12, BANK_START=32, TRACK_START=128, BANK_END=160, OVERFLOW=256):
            root = Path(directory)
            rom = bytearray(256)
            struct.pack_into("<I", rom, 4, audio.ROM_BASE + 36)
            pcm = bytes((0, 127, 128, 255))
            struct.pack_into("<HHIII", rom, 36, 0, 0, 8192000, 0, 4)
            rom[52:56] = pcm
            rom[128:138] = bytes((0xBC, 0, 0xBD, 0, 0xD0, 60, 100, 0x81, 0xB1, 0))
            rom[148:160] = struct.pack("<4BII", 1, 0, 0, 0, audio.ROM_BASE, audio.ROM_BASE + 128)
            sample = dict(start=36, end=60, kind=0, status=0, frequency=8192000, loop_start=0, count=4, rate=8000)
            dummy = dict(header=32, start=32, end=36, tracks=[], voice=None, ids=[0], parse=dict(events={}, pointers={}, notes=set()))
            song = dict(header=148, start=128, end=160, tracks=[128], voice=0, ids=[1, 2], parse=audio.sequences(rom, [128], 128, 148))
            model = dict(songs=[dummy, song], table=[(audio.ROM_BASE + p, 0, 0) for p in (32, 148, 148)],
                         roots={0}, tones=[0], cgb=set(), samples=[sample], refs={4: 36, 152: 0, 156: 128})
            with patch.object(audio, "inventory", return_value=model):
                assets, output = root / "assets", root / "custom output"
                audio.build(bytes(rom), assets, output)
                source = assets / "audio/songs/0001.s"
                text = source.read_text().replace(".byte KEYSH, 0", ".byte KEYSH, 2\n    .byte W01")
                source.write_text(text)
                audio.extract(bytes(rom), assets)
                self.assertEqual(source.read_text(), text)
                wav = assets / "audio/samples/00000024.wav"
                wav.write_bytes(audio.wav_bytes(pcm * 4, 8000))
                _, units = audio.build(bytes(rom), assets, output)
                moved = [unit for unit in units if unit["overflow"]]
                self.assertEqual([unit["kind"] for unit in moved], ["sample", "song"])
                self.assertEqual(moved[0]["target"], 256)
                self.assertGreater(moved[1]["target"], 256)
                self.assertIn("custom output", (output / "audio-bank.s").read_text())
                self.assertIn("gAudioSample_00000024", (output / "audio-voices.s").read_text())
                self.assertEqual(len(json.loads((output / "audio-layout.json").read_text())), 3)
                source.write_text(text.replace(".word gAudioVoice_00000000", f".word {audio.ROM_BASE}"))
                with self.assertRaisesRegex(ValueError, "symbolic"):
                    audio.build(bytes(rom), assets, output)


if __name__ == "__main__":
    unittest.main()
