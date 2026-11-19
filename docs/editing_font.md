# Editing the dialogue fonts

Use the existing Python 3 / ARM build tools; no Python packages are needed.
`make extract` creates missing script **and font** assets. Normal extraction and
builds never overwrite existing assets. `make extract FORCE=1` is the explicit,
destructive restore of all extracted assets, including text and charmap.
Font-only restore: `python3 tools/assets/font_assets.py extract --force`.
Keep all extracted PNGs/maps under ignored `assets/`; never commit them or ROMs.

## Sheets and code maps

| Editable file under assets/fonts/ | Layout | ROM slot |
|---|---|---|
| `small.png` | 487 8x12 glyphs; 32 columns, 256x192 sheet | 0x4F90CC, 5844 bytes |
| `single.map` | 256 signed-16 code-to-small-glyph entries | 0x4FA7A0, 512 bytes |
| `large.png` | 6922 16x12 glyphs; 32 columns, 512x2604 sheet | 0x4FA9A0, 166128 bytes |
| `double.map` | 7938 signed-16 code-to-large-glyph entries | 0x523290, 15876 bytes |

Glyph N is at column `N % 32`, row `N // 32` (zero-based). There are no gutters
or grid pixels. In an image editor enable a grid at 8x12 / 16x12 and nearest
neighbor zoom. Black is ink, white is transparent background in-game. Retain the
exact dimensions, and leave unused last-row cells white. Export a **noninterlaced
indexed or grayscale PNG, 1/2/4/8-bit, opaque pure black and white**. RGB/RGBA,
transparency, antialiasing, resizing and extra glyph cells are rejected clearly.
All five PNG row filters are supported. The writer exports a 1-bit indexed PNG.

Stored glyphs are 1bpp, MSB-left within each byte, twelve rows per glyph. Each
large row is two bytes: left eight pixels first, then right eight (not a big-
endian 16-bit word). ARM IWRAM routines 0x0300085C / 0x03000714 expand these
into 4bpp padded/shadowed tiles. This step does not change their renderer or the
separate calendar/clock fonts.

Map lines are `HEX_CODE = DECIMAL_GLYPH_INDEX`, with optional `#` comments:

```text
7F = -1
```

Change `-1` to a small index 0..486 to map a single byte; double-byte entries
take -1 or large indices 0..6921. Keep **all** rows, including unmapped entries.
Double-byte keys enumerate the game's 81..9F/E0..EA lead bytes and 40..FC trail
bytes, including 7F trail slots; this is the game's indexing, not strict CP932.
Negative values other than -1, duplicate/missing keys and oversized indices are
errors. New glyph counts and tagged half-width mappings are not implemented.

## Adding a letter manually

1. Find a safe unused code in `single.map` (see capacity limits below).
2. Find an unused glyph index: it must not appear as any nonnegative value in
   `single.map`. Do not reuse a cell another translator has drawn/reserved.
3. Draw the 8x12 glyph in `small.png`, and set that code's map entry to its index.
4. Add the Unicode assignment to `assets/charmap.txt`, e.g. `U+1EA1 = 7F` for ạ.
   This example assumes you have mapped 7F to your drawn glyph; assigning bytes
   in charmap alone does not install graphics.
5. Type NFC/precomposed UTF-8 text into `assets/text/NNNN.txt` using the format in
   [editing_text.md](editing_text.md). Run `make -j8 fomt.gba`.

Overrides are encoded before CP932. Decomposed sequences should be normalized
to NFC before editing: the script encoder does not silently normalize text.
Inspect your output in-game: accents, line spacing and text-box widths are not
automatically checked. Very tall accents must fit inside twelve source rows.
Unmodified assets must still pass `make -j8 compare`; edited ones intentionally
have a different ROM hash. Generated font binaries live in `build/assets/` and
are included at the original labels, so neither scripts nor the suffix move.

## Capacity of the original US fonts

Run `python3 tools/assets/font_assets.py capacity` against your current maps.
For the original maps:

* **98** single-byte entries are -1, but only **23** are safe ordinary text codes:
  **7F, 80, A0, EB..FE**. The other unmapped entries are 00..1F controls/NUL,
  **81..9F/E0..EA Shift-JIS lead bytes**, and FF (substitution prefix). Do not
  steal those leads: the streaming parser first tries the single-byte glyph and
  only accumulates another byte when it fails. Mapping a lead breaks existing
  Japanese/full-width sequences that begin with it.
* 158 mapped single-byte entries reference **157 distinct small glyphs**;
  **330 of 487 small glyph slots are unreferenced by the single-byte table**.
  These are unreferenced, not necessarily blank artwork. Double-byte entries
  currently reference the separate large font, not these small slots. The
  hardcoded B1..C3 alternate glyphs live elsewhere and are not allocation codes.
* Vietnamese has **134 non-ASCII precomposed letters across both cases**:
  12 vowel bases x 6 tone states x 2 cases, minus the 12 ASCII vowel forms,
  plus Đ/đ. **All 134 are absent from CP932**; the existing CP932-to-glyph route
  therefore cannot address them as those Unicode letters. This is an encoding
  coverage count, not an assertion that no existing bitmap resembles a letter.
* Thus the full set **does not fit in safe single-byte codes** (111 short),
  although glyph storage has ample room. Even all 98 -1 entries would not fit.
  The ASCII consonants/base vowels already work. A short selected sample does
  fit in the original single-byte allocator. The opt-in full-width alternative
  below can encode all 134, but trades unused-in-scripts kanji artwork for them.

## Opt-in script-only kanji reclamation (no decoder change)

The original double map has **606 accepted unmapped codes**, but **zero truly
unreferenced large glyph slots**: all 6922 slots have at least one mapped code.
That is different from whether a code occurs in dialogue. A renderer-style
scan of **all 1328 original scripts / 8368 STR strings** finds **146 distinct
double-byte codes**. **6776 large glyph slots have no alias occurring in those
strings**; **6310** of them have aliases exclusively in the kanji ranges
`889F..9FFC` / `E040..EAFC`. This includes checking every alias, not just the
first code mapping to each glyph. Thus there is room for all 134 letters with
the existing **16px full-width** renderer.

```sh
python3 tools/assets/font_assets.py capacity
python3 tools/assets/font_assets.py sample --reclaim-large --placeholders \
    --text 'Xin chào Việt Nam!'
# Or assign the complete precomposed alphabet at once (Python standard library):
python3 -c "import sys; sys.path.insert(0, 'tools/assets'); import font_assets as f; print(''.join(f.vietnamese_letters()))" > build/vietnamese.txt
python3 tools/assets/font_assets.py sample --reclaim-large --placeholders \
    --text "$(cat build/vietnamese.txt)"
```

Then type the same precomposed UTF-8 characters into the text assets and build
with `make -j8 fomt.gba`. Two-byte charmap rows look like `U+1EA1 = 98 73`
(illustrative; use the code actually printed by the allocator). Draw assigned
cells in `assets/fonts/large.png`, not small.png. These glyphs consume **two
text cells**, even if placeholder ink is narrow; dialogue wraps sooner.

`--reclaim-large` is an explicit acceptance of **script-only** risk. It:

* scans original ROM scripts and currently edited text, using the renderer's
  accumulated lead/trail logic (not a byte-pair grep), skipping CRLF/control
  bytes and FF substitution tokens;
* allocates only unmapped accepted double codes whose lead remains unmapped
  in the single table, preferring kanji-range gaps and ordinary trails over
  accepted 7F trails;
* reclaims only slots whose **every current and original alias is unused** in
  those scripts, and whose aliases are all kanji-range codes (never kana or
  punctuation slots, even if unused);
* protects existing charmap assignments, original script uses even if you have
  deleted them from edited text, and glyph cells hand-edited since extraction;
* preserves old aliases rather than renumbering/removing codes, adds the new
  code mapping, and leaves other art/map comments/charmap entries intact.

**This is not a proof that a slot is unused throughout the game.** Non-script
item/menu/name tables, hardcoded strings, save names, and dynamically expanded
FF replacement text are deliberately **not audited**. A non-script consumer
of an old alias will display the new Vietnamese glyph instead of the kanji.
Adding that kanji to script text later will also show the replacement art;
old aliases are retained. Do not ship a translation without auditing the
non-script consumers and testing relevant gameplay. No runtime decoder or
global pointer changes are made, and normal builds never reclaim automatically.

Vietnamese letters in `--reclaim-large` mode are drawn automatically, even
without `--placeholders`. The tool composites the extracted small-font base
letters with original, hand-drawn acute/grave/hook/tilde/dot/circumflex/breve/horn
patterns. Circumflex or breve plus tone are placed side by side; horn attaches
to the upper right of o/u. Accented i loses its original dot, dot-below letters
reserve row 11, and marked capitals compress their base vertically while
preserving strokes. Đ/đ add a crossbar to D/d. All ink fits a 16x12 cell.
Only accent patterns and algorithms are committed, never ROM-derived letters.

The unmodified renderer has no per-glyph width table: double-byte map entries
always return width class 2 and use the 16px drawing/advance path, regardless
of ink width. There is no existing double-byte-to-8px switch; no decoder patch
is made here. The original single-byte path remains 8px.

```sh
python3 tools/assets/font_assets.py vietnamese-sheet \
    --output /home/ubuntu/screenshots/vi_glyphs.png
```

This generates all 134 letters from the supplied ROM at 4x zoom, with Unicode
hex labels (alphabetical by code point). The PNG is a local review artifact,
not source to commit. Existing assigned/hand-edited glyphs are not redrawn by
sample; repeated assignments remain no-ops. For non-Vietnamese characters,
`--placeholders` retains its old generic-marker behavior; otherwise those
cells keep their original artwork for manual drawing. Existing single-byte
assignments are kept, not migrated. Review the generated Vietnamese sheet
and in-game typography before release, especially the compact capitals.

### Proposed next decoder change — NOT implemented

`func_080D0D28` in `asm/code_080D0C68.s` loads a double-byte table entry and
unconditionally sets width class `r6 = 2`. Positive indices then use the large
24-byte glyph path; the half-width path uses class 1 and 12-byte small glyphs.
The result class also controls text advance. `GetShiftJisGlyphIndex` at
0x080D0CD4 (`src/g5_encoding.cc`, formerly func_080D0CD4) is only a code-to-table
index calculation; it has no width convention. Therefore **no current double-
byte map value selects an 8px small glyph**. Negative entries return failure;
putting a small index in the double map simply selects the wrong large glyph.

Smallest semantic extension: reserve positive bit 14 (`0x4000`) in double-map
values to mean `0x4000 | small_index`. Immediately after the double table load,
detect this bit, clear it from r5, and set r6=1; otherwise retain r6=2. Keep -1
unmapped by testing positivity before interpreting flags. Existing large
indices max at 6921, so bit 14 is free; tagged values remain positive s16. Both
draw calls and width-only queries must see the same class. Update the map
validator/sample allocator to support these tags, then allocate unused valid
double-byte codes rather than stealing lead bytes. No change to the Shift-JIS
index formula is required.

This semantic patch must **not expand the existing assembly in place**: doing
so shifts code/data with raw pointer dependencies. Use a fixed-size branch to
an appended Thumb helper/trampoline and return to the original instruction
stream, or a size-matched replacement after an instruction budget audit. That
placement detail remains to be designed/tested; no runtime patch is in this PR.

## Optional sample assignment

```sh
python3 tools/assets/font_assets.py sample --text 'Xin chào Việt Nam!' --placeholders
# Put that same UTF-8 text in an assets/text/NNNN.txt entry, then:
make -j8 fomt.gba
```

The command normalizes its input to NFC, checks available capacity before
editing, allocates safe codes and unreferenced small glyph slots, updates only
the selected -1 map entries, and appends charmap assignments. Existing charmap
assignments, map comments and other pixels are retained. Hand-edited unmapped
cells (different from original ROM) are reserved and skipped. Repeating the
same command is a no-op. Capacity exhaustion leaves existing assets unchanged.

Without `--placeholders`, it does not alter the sheet; draw the reported cells
yourself. With the flag it copies each base ASCII letter and adds a simple
top accent marker. These are **readable placeholders, not correct Vietnamese
typography** (all accents intentionally share a marker). Review/redraw them.
The command does not edit script text automatically or implement the proposed
half-width double-byte extension. The default single-byte mode still fails
safely for the full set; `--reclaim-large` enables the riskier full-width mode.

## Verification

```sh
make assets-test
python3 tools/assets/font_assets.py roundtrip
make clean && make -j8 compare
python3 tools/assets/check_font_rom.py
python3 tools/assets/check_vietnamese_rom.py
```

Roundtrip compares all font and map bytes to the original ROM. The opt-in ROM
check requires pristine extracted assets and compare-OK `fomt.gba`, toggles one
PNG pixel and verifies exactly one expected ROM byte changes, then builds a
Unicode/charmap/map/glyph sample and parses it through the built script table.
It additionally assigns all 134 letters via reclaimed large slots, builds the
phrase plus the alphabet, and verifies every new two-byte code, glyph and
script string in the ROM while protecting every original script-used alias.
It restores its edits in `finally` and finishes with compare; local sample
ROMs are retained at `build/assets/font-sample.gba` and
`build/assets/font-reclaimed-sample.gba`. This pipeline check is not an emulator
rendering test.

The Vietnamese-specific check assigns all 134 composed letters and replaces
Thomas's script 0867 string 0 with a two-row Vietnamese sample. It validates
the built script and all 134 bitmaps, saves `build/assets/vietnamese-thomas.gba`,
then restores the edited files and compares the original ROM again. Start
with pristine assets and a compare-OK build; do not run these edit checks in
parallel with another build or asset editor.
