"""Build an altered, relocated song; verify references and restore editable files."""

import json
from pathlib import Path
import re
import shutil

import audio_assets as audio
from check_font_rom import make
from font_assets import original_rom


def main():
    original = original_rom(Path("baserom.gba"))
    if Path("fomt.gba").read_bytes() != original:
        raise ValueError("start with an unmodified compare-OK ROM")
    path = Path("assets/audio/songs/0035.s")
    saved = path.read_bytes()
    model = audio.inventory(original)
    song = next(song for song in model["songs"] if 35 in song["ids"])
    try:
        source, changed = re.subn(r"(\.byte KEYSH, )0\b", r"\g<1>2", saved.decode())
        assert changed == len(song["tracks"])
        path.write_text(source, encoding="utf-8")
        make("AUDIO_OVERFLOW_SONG=35", "fomt.gba")
        edited = Path("fomt.gba").read_bytes()
        layout = json.loads(Path("build/assets/audio-layout.json").read_text())
        moved = next(unit for unit in layout if unit["name"] == "song-0035")
        assert moved["overflow"] and moved["target"] >= audio.OVERFLOW
        header = audio.pointer(edited, audio.TABLE + 35 * 8)
        assert header == moved["target"] + song["header"] - song["start"]
        assert edited[song["start"]:song["end"]] == original[song["start"]:song["end"]]
        assert edited[audio.VOICE_START:audio.VOICE_END] == original[audio.VOICE_START:audio.VOICE_END]
        tracks = [audio.pointer(edited, header + 8 + i * 4) for i in range(edited[header])]
        parsed = audio.sequences(edited, tracks, moved["target"], header)
        for at, end_command in parsed["events"].items():
            if end_command[1] == audio.CONSTANTS["KEYSH"]:
                assert edited[at + 1] == 2
        expected = bytearray(original[song["start"]:song["end"]])
        delta = moved["target"] - song["start"]
        for at, dest in model["refs"].items():
            if song["start"] <= at < song["end"] and song["start"] <= dest < song["end"]:
                expected[at - song["start"]:at - song["start"] + 4] = (audio.ROM_BASE + dest + delta).to_bytes(4, "little")
        for at, event in song["parse"]["events"].items():
            if event[1] == audio.CONSTANTS["KEYSH"]:
                expected[at + 1 - song["start"]] = 2
        assert edited[moved["target"]:moved["target"] + moved["bytes"]] == expected
        for i, (before, after) in enumerate(zip(original, edited)):
            if before != after:
                assert i >= audio.OVERFLOW or audio.TABLE + 35 * 8 <= i < audio.TABLE + 35 * 8 + 4, hex(i)
        output = Path("build/assets/audio-song-35-transposed.gba")
        shutil.copyfile("fomt.gba", output)
        print(f"Audio song edit: ID 35; {changed}/{changed} tracks transposed +2 semitones; symbolic header and all pattern/branch pointers verified", flush=True)
        print(f"Audio overflow: song -> ROM 0x{moved['target']:X}; original song slot, voice/sample bank and suffix preserved", flush=True)
        print(f"Sample ROM: {output.resolve()}", flush=True)
    finally:
        path.write_bytes(saved)
        make("compare")


if __name__ == "__main__":
    main()
