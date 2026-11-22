# Editing M4A audio

Run `make extract` (or `python3 tools/assets/audio_assets.py extract`).
Existing files are never overwritten, unless explicitly extracting with
`--force` / `make extract FORCE=1`. Back up your edits before forcing.
ROM-derived sources live only in ignored `assets/audio/`; build products and
sample ROMs live in ignored `build/assets/`. Python uses only the standard library.

## Inventory and evidence

`gSongTable`, ROM `0x13ABF0`, has 211 records, each a song pointer and two u16
player fields. There are 145 distinct headers: one four-byte empty header at
`0x13B288`, and 144 nonempty songs with 277 distinct track entry points.
Aliased IDs share one source, named for their lowest ID. The source comment
lists every alias. The table's metadata and aliases are preserved; its pointers
are generated as `.4byte gAudioSong_...`, never patched numeric addresses.

Reachability starts at this table: `src/m4a.c`'s `m4aSongNumStart` calls
`MPlayStart`, which reads the header's track count, tone pointer and track
pointers. `src/m4a_1.s:MPlayMain` supplies the running-status rule (only commands
>= BD update it); `ply_goto`, `ply_patt`, `ply_pend`, `ply_rept`, `ply_note`,
and `src/m4a.c:ply_memacc` define control flow and argument lengths. Extraction
walks those branches, up to three pattern return levels, carrying voice/key and
running status. Loops are bounded by visited states; MEMACC/repeat branches are
conservatively traversed both ways. Unsupported/ambiguous control flow fails.

The two root groups are `0x139D60` and `0x13A360`, each 128 twelve-byte tones.
The rhythm group at `0x13A7B0` overlaps the second group. Its table ends at
`0x13A9B4`, the first PSG wave address referenced by the groups, not at a
speculatively assumed 128th drum. This yields 263 physical tone records and
eight PSG waves. The actual note walk checks every played program/key against
these bounds (drum keys 39..42 in the original songs). The driver indexes the
rhythm group by the note key in `ply_note`; it rejects another nested group.

Walking all root/rhythm tone sample fields yields **106 unique DirectSound
waves**. Their bounded 16-byte `WaveData` headers plus signed PCM cover
`0x13B28C..0x29CD3C`, including one endpoint byte per wave and four-byte
alignment. Header kind is zero; status, fractional frequency and loop start are
preserved. This is not a scan for plausible WAV signatures. PSG waves are
preserved as editable bytes in `voices.s`, not incorrectly exported as PCM WAVs.

## Replace or change a song

Edit `assets/audio/songs/0035.s`, for example:

```asm
    .byte KEYSH, 2          @ transpose this track up two semitones
    .byte TEMPO, 60         @ driver tempo is twice this byte
    .byte VOICE, gAudioInstrument_00139D60_031
    .byte N06, Cn3, v100
    .byte W06
    .byte GOTO
    .word gAudioTrack_002A9948
```

Keep the labels from your own file: the example track label must not be copied
into unrelated songs. A song header references its root group symbolically;
VOICE uses symbolic, zero-based selectors generated in `instruments.inc`.
Selectors identify **slots**, not General MIDI instrument names. Edit the
corresponding record of `voices.s` to change its sample/ADSR. PCM and PSG wave
references there are symbolic too. Keep voice labels in their fixed positions.

The format is M4A GAS data assembly, not MIDI: `.byte`, `.word`/`.4byte`,
`.hword`/`.2byte`, `.equ`/`.equiv`/`.set`, `.align` (power of two), `.balign`,
`.global`, labels, `@` comments, integer arithmetic and the supplied command,
note and velocity constants. Alignment is at most four bytes. Source can be
assembled with GNU ARM `as`, but the builder uses a bounded stdlib assembler,
not executable macros or arbitrary includes. It only accepts the provided
command/instrument includes. `instruments.inc` is generated selector metadata;
do not edit it. Arbitrary mid2agb output may need its macros/includes expanded
into these directives; there is no MIDI converter or MIDI roundtrip claim.

To replace an entire song, retain its `gAudioSong_...` header label and put
1..8 track entry pointers at the end of the file, with `.balign 4` before the
header. Place track/pattern data before the header, and use symbolic `.word`
operands for **all** branch, pattern, group and track pointers. Preserve or
replace the file's internal labels consistently. Numeric note arguments and
running-status rows are intentional and lossless; do not insert a command
that changes running status without updating subsequent implicit rows.
The empty song cannot be moved or changed. Adding new song IDs, voice slots,
key-split groups or new sample files is not supported by this manifest.

Run `make -j8`. All 145 song sources are assembled on rebuild. A song that fits
stays in its original slot; growth relocates that song into `.audio_overflow`.
For explicit relocation testing, `make AUDIO_OVERFLOW_SONG=35 -j8 fomt.gba`.
`build/assets/audio-layout.json` records every original/target span.

## Replace a sample

Replace a file such as `assets/audio/samples/0013B28C.wav` with **mono,
uncompressed, 8-bit PCM WAV**. WAV stores unsigned bytes; the tool converts to
the driver's signed bytes. Stereo, 16-bit, empty and truncated audio is rejected;
convert externally first. The matching JSON contains `kind` (must stay zero),
`status` (0, 16384, 32768 or 49152), `frequency`, and `loop_start` (frame index).
For looping status (`status & 49152`), the loop start must precede the last
frame. The frame count is rebuilt from the WAV, never from stale metadata.

Driver frequency is in 1/1024-Hz units. An unchanged integer WAV rate preserves
the original fractional frequency exactly. Changing the WAV rate changes the
driver frequency to rate*1024; alternatively edit JSON `frequency`, not both.
The JSON loop/status fields are authoritative; WAV editor loop chunks are not
imported. Changing sample rate changes playback tuning. The tool does not
resample or normalize. Unchanged PCM retains its original endpoint byte.
Changing PCM or loop metadata regenerates this byte from the loop start for
looping waves, or zero for nonlooping waves. Alignment outside the payload is
preserved in fixed slots; overflow gets four-byte alignment.

Smaller PCM stays in its fixed slot. A larger WAV relocates the whole header
and PCM to audio overflow, and all symbolic tone pointers follow it.

## Relocation safety and checks

The original bank, script start and ROM suffix remain pinned. Audio overflow
is first after the fixed suffix marker at `0x75C244`, followed by existing
script/font/graphics overflow; the shared 32 MiB ROM limit still applies.
Generated assembler emits labels and `.4byte` relocations for pointer fields,
not binary-search-and-replace patches.

Before moving a song/sample, a **byte-step** original-ROM audit rejects unknown
pointer-shaped references into its entire old slot, including interior and
unaligned references. Known song-table/header/branch/voice fields are symbolic;
proven signed PCM payload bytes are excluded as non-pointer data. Other raw
matches are **not** silently rebased, even if probably coincidental machine
code or graphics. Thus some edits can fail conservatively with
`cannot relocate audio: non-symbolic reference at ... -> ...`; keep them in
place or audit and symbolize the actual consumer before proceeding. This is
not a proof against computed pointers or arbitrary runtime-generated sequences.

```sh
make assets-test
python3 tools/assets/audio_assets.py roundtrip
make clean && make -j8 compare
python3 tools/assets/check_audio_rom.py
```

Roundtrip verifies all song/voice/sample bytes, including exact fractional
frequencies; unmodified `make compare` must say `fomt.gba: OK`. The integration
check transposes all six tracks of song ID 35, forces relocation, verifies
every branch/header reference and unchanged original bank, saves
`build/assets/audio-song-35-transposed.gba`, then restores the source and vanilla
ROM. It does not assert an audible/emulator playback check.
