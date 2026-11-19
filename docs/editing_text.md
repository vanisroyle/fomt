# Editing event text

Requires Python 3 (standard library only), the existing ARM toolchain/agbcp,
and the original US `baserom.gba` matching `fomt.sha1`. Never commit the ROM,
extracted files, screenshots containing extracted text, or generated binaries.
`assets/` and `build/` are ignored. Run commands from the repository root.

## Extract, edit, build

```sh
make extract                # only creates missing files; keeps existing edits
# edit assets/text/NNNN.txt in UTF-8
make -j8                    # builds fomt.gba (compare is the default target)
```

The default target compares against the original hash, so **an edited ROM will
build successfully but fail the vanilla comparison**. To build edited text
without that expected comparison failure, use `make -j8 fomt.gba`.
`make -j8 compare` must print `fomt.gba: OK` for unmodified extraction.
From a clean checkout, simply running make extracts missing assets automatically.
Every build checks the editable files, including deletions and charmap changes;
unchanged generated outputs keep their timestamps.

```sh
make extract FORCE=1        # DESTRUCTIVE: restore all extracted files from ROM
```

This explicit command overwrites text, code, metadata, JUMP files **and charmap**.
Ordinary builds never overwrite existing files under assets. Deleting an
individual file makes the next build restore that file only. Back up your edits
outside the repository before re-extracting; a partial restore may intentionally
leave a mixture of edited and original files.

## Text format

Each file begins with a comment giving the script ID/original address. Each
string starts with `=== N`, with sequential zero-based IDs. Every following
content line starts with `|` (the prefix is not part of the game string):

```text
# Script ID and original address are written here by extraction
=== 0
|Example dialogue.{W}
=== 1
|
=== 2
|First line
|Second line
|
```

String 1 is empty. String 2 ends in a game newline: the final `|` line is
significant. The physical final newline after a content line is **not** part of
the string. Multiple `|` lines join with game CRLF. A line must retain its `|`
even when empty; this also prevents content resembling `=== N` or comments from
being parsed as structure. Keep every ID, including duplicates and empty entries.
Files with no strings contain only the initial comment. Do not add comments
inside entries.

Tokens (case-sensitive names; hex digits may be lowercase):

| Token | Bytes / meaning |
|---|---|
| `{W}` | 05, wait/acknowledge |
| `{CLEAR}` | 0C, clear text surface |
| `{FFxx}` | FF followed by any nonzero hex byte; generic substitution/control tokens, semantic names not assigned |
| `{XX}` | one raw hex byte; e.g. `{0D}`, `{0A}` for bare CR/LF |
| `{7B}`, `{7D}` | literal opening/closing braces |

NUL (`{00}` or `{FF00}`) is forbidden inside strings; the builder appends the
terminator. Preserve unknown tokens rather than guessing their meanings. Bytes
that cannot be decoded/re-encoded losslessly as CP932 are exported as hex tokens.

Normal Unicode text is encoded to CP932. Unsupported characters produce an
error naming the file, line, and Unicode code point. Optional
`assets/charmap.txt` overrides are consulted **before** CP932, one character per
mapping, for example:

```text
# Unicode code point = space-separated game bytes
U+1EA1 = 81 40
```

This is only an encoding assignment, **not a font installation**. Until a later
font step replaces the assigned glyph, the ROM displays the original glyph for
those bytes. Literal braces/control characters use tokens even if a charmap
entry exists. No UTF-8 bytes are sent directly to the game.

## Binary preservation and placement

Scripts have IDs 1..1328; ID 0 stays NULL. `assets/scripts/NNNN.code.bin` is
unchanged bytecode (without its length word); optional `NNNN.jump.bin` preserves
the entire JUMP payload. JSON sidecars preserve chunk order, original STR
offsets/pool bytes, CODE trailers and unknown chunks. Do not edit sidecars for
ordinary translation. If text re-encodes to the original bytes, the original
STR layout is reused exactly, including sharing, duplicates and unused bytes.
Otherwise STR offsets and all enclosing lengths are rebuilt.

Bytecode/JUMP assembly is not implemented: changing bytecode size or ordering
requires manually fixing its internal branch/JUMP targets. String IDs cannot be
added/deleted by this step. The renderer, line widths, substitution buffers and
save-name limits are unchanged. Very long text can build yet be unusable in the
game; building is not a dialogue-layout or gameplay validation.

Generated containers/assembly live under `build/assets/`. The original bank
`0x082AC0B0..0x084F90CC` is always exactly its original size. Scripts are placed
in ID order until the next script cannot fit; that script and all later scripts
go into an aligned overflow section after the original IWRAM image and zero
marker (starting at `0x0875C244`). The old suffix is never shifted. Table entries
are symbolic addresses, including entries for moved scripts. Freed bank space
is zero-filled; scripts and inter-script alignment use the original format.
The ROM is FF-padded to an 8 MiB multiple, up to the GBA's 32 MiB limit.

```sh
make -j8 fomt.gba SCRIPTS_OVERFLOW_FROM=1000  # diagnostic: IDs >=1000 spill
make -j8 compare                            # remove knob to restore placement
```

Growth can also spill earlier than the forced threshold. This supports the
observed table-based script consumers; a complete audit of hypothetical direct
script-interior references is not claimed. Raw pointer-looking pixel/text bytes
are never blindly rebased.

## Checks

```sh
make assets-test
python3 tools/assets/fomt_assets.py roundtrip
python3 tools/assets/fomt_assets.py verify-rom --rom fomt.gba
python3 tools/assets/check_rom.py
```

`roundtrip` requires extracted, unedited assets and the original ROM;
`verify-rom` compares every script reached through the built ROM's table with
the current assets. Unit fixtures contain no ROM-derived data. The opt-in
integration check requires unmodified assets, temporarily lengthens one string,
tests natural and forced overflow, verifies all table entries and the fixed
suffix, restores the text in a `finally` block, then runs compare. It saves a
forced-overflow ROM at `build/assets/overflow-test.gba` for local emulator checks.
It does not test text rendering or gameplay. Emulator boot is only a smoke test;
exercise translated dialogue in-game separately.
