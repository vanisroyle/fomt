# Editing text

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

## Non-script text tables

`make extract` also creates `assets/text_tables/strings.txt` and
`assets/text_tables/catalog.json`. The UTF-8 text uses the same `=== N` / `|`
format, CP932 encoder, control tokens and `assets/charmap.txt` as dialogue.
The catalog maps each ID to its original ROM address, byte capacity, table
membership and record index. Find an item using, for example,
`tools/000/name`, `foods/000/description`, or `articles/000/name` in the catalog,
then edit that ID in `strings.txt`. Multiple memberships mean shared storage:
editing that ID changes every consumer. Keep the IDs, including empty entries.
The JSON is a generated index, not an editable pointer/layout specification.
Deleting either file restores only that missing file; `make extract FORCE=1`
overwrites both, along with the other extracted assets.

The code-derived catalog currently contains **3,359 unique entries**:

| Data | Ownership / renderer path |
| --- | --- |
| 81 tools, 171 foods/crops/products, 95 articles: names and descriptions, plus invalid-item fallbacks | `src/item.cc`: `gToolInfo`, `gFoodInfo`, `gArticleInfo`, `GetName`/`GetDesc`; inventory/message consumers in `src/g5_item_text.cc` and `asm/code_080C8360.s` |
| 18 settings pointers; setup/menu/confirmation labels; 18 name-entry rows | `src/new_game.cc`, `asm/new_game.s`: string draws, byte and double-byte character selection/drawing |
| Horse-race help (10 pointers), tickets, medal exchange, livestock reports and status labels | `src/race_display.cc`, `src/medal_exchange.cc`, `src/livestock_report.cc`; `DrawMessageTileStringWithColors` and message-window choices |
| Shop prompts, choices, descriptions, recipe headings and utensil labels | `asm/code_0803EE94.s`: renderer calls, `strlen`/copy into menu labels, and message-window substitution/choice setup; the 20-byte shop records at `0xFD988` (5) and `0xFFB90` (11) contain name/description pointers at +4/+12 |
| Calendar/season/birthday, contest/report, inventory and recipe byte/glyph labels | Direct `DrawMessageGlyphWithColors` callers in `asm/code_0803EE94.s` and `asm/code_080C8360.s`, in addition to string callers |
| 43 NPC name pointers, 59 fishing-name pointers, default animal name, wrapped-item suffix | `src/g1_schedule_extra.cc`, `src/code_actor_0809BFE8.cc`, `src/animal.cc`, `src/g5_item_text.cc` |
| 137 book/library line tables, 3,279 line references (shared strings deduplicated) | `gUnk_0810828C` indexed by `func_080790E4` and library UI routines; NULL-terminated line-pointer arrays passed through `func_0807A708` and into `DrawMessageTileStringWithColors` |

`tools/assets/text_table_catalog.py` records the table layouts and explicit
roots. Its `roots()` helper follows literal arguments to string draws, choices,
substitutions and length/copy operations in the disassembly; running it prints
discovery sites. The inventory was expanded by following the indirect table
loads and direct-glyph consumers listed above, not by treating arbitrary ROM
bytes or pointer-shaped words as strings. Generated numeric/date/player-name
text is still produced by game code; bitmap artwork belongs to the graphics
pipeline. This is not an automated proof that every possible dynamically
constructed renderer input has been identified.

### Layout and growth constraints

* Ordinary strings are NUL-terminated. Shorter edits preserve the remaining
  original slot bytes; equal-sized edits stay in place. Never insert `{00}`.
* Name-entry rows preserve their byte width and cell layout. The first six rows
  have single-byte characters separated by spaces; do not alter separators.
  The remaining twelve rows require one untagged double-byte glyph per cell.
  Tagged 8px Vietnamese assignments cannot replace a full-width grid cell.
* Seven direct-glyph arrays are represented as UTF-8 text but rebuilt as
  **little-endian 16-bit glyph codes**, not NUL-terminated strings. Preserve
  their glyph count. Their 8px cells accept single-byte or tagged 8px glyphs,
  not ordinary 16px double-byte glyphs. Other fixed byte-indexed labels retain
  byte width and single-byte cells. Pad fixed labels with spaces if necessary.
* Growth is allowed only for roots belonging to an audited symbolic pointer
  table (items, settings, race help, shop records, NPC/fishing names, wrapped
  suffix and library lines). Larger strings move to `.text_table_overflow`;
  the original slot and all unrelated data retain their placement. Direct UI
  literals with no such table fail with `no audited symbolic pointer table`.
  This conservative restriction also protects labels addressed by base+index.
* A post-link audit searches original references to every byte of a moved
  string and requires their linked values to follow the symbol, including
  interior references. A raw/non-symbolic reference causes an error and removes
  the rejected ROM. It is never silently patched. Pointer-shaped bytes in
  unrelated binary data may conservatively reject an otherwise valid edit.
* Item literals are emitted by the original compiler. A checked assembly pass
  adds symbolic aliases and replaces only the verified literal bytes; it fails
  if compiler literal placement no longer matches the original ROM. No source
  item IDs, numeric fields, descriptions' pointers or structure sizes change.
* Relocation does **not** resize menu tiles or runtime buffers. Preserve book
  markup (`<...>`), dialogue control tokens and substitution syntax; check
  line wrapping and clipping in-game. A successful build is not a layout check.

### Vietnamese reclaimed-kanji audit

`python3 tools/assets/text_tables.py roundtrip` reports code usage against the
original font maps and original RIFF scripts, using the font tool's renderer
lead/trail parser rather than Unicode categories. The original catalog uses
97 distinct double-byte codes. Two are aliases of script-unused kanji slots:
`96B3` → large glyph **3220**, and `96BC` → large glyph **3229**, in the default
unnamed-animal text. Neither belongs to the first 134 slots selected by the
unmodified `sample --reclaim-large` allocation in PR #29. Thus that default
sample does not replace these two glyphs, but a different/larger reclamation
can; script-only reclamation is not universally safe for non-script text.
The opt-in tagged small-font mode does not reclaim these large slots. Newly
edited text and dynamic/player-entered names still require their own review.

### Checks and sample

```sh
make assets-test
python3 tools/assets/text_tables.py roundtrip
make clean && make -j8 compare
python3 tools/assets/check_text_tables_rom.py
```

The last command requires an unmodified compare-OK ROM. It grows the first tool
name, rebuilds, verifies the item pointer and original slot, checks every other
catalog entry plus the original ROM suffix, and copies the sample to
`/home/ubuntu/text-table-edit.gba`. It restores the text source in `finally`
and rebuilds vanilla with `compare`. No emulator or visual check is performed.

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
