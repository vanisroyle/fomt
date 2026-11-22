# Editable graphics

Requires the original US `baserom.gba`. No extracted art is committed. Everything
in `assets/` is private editable source; `build/` contains generated output.

```sh
make extract                       # existing assets are never overwritten
make extract FORCE=1               # explicitly discard ALL asset edits
make -j8                           # editing a PNG or map rebuilds the ROM
make compare                       # unmodified assets must be byte-identical
```

## First supported image: Natsume boot-logo background

`assets/graphics/boot-logo.png` is an **indexed 4bpp tile sheet**, 32 tiles per
row, using the image's actual 16-color palette. Edit pixels or palette colors
without merging/reordering palette entries. Keep the image dimensions, indexed
color mode and 16 palette entries. Noninterlaced indexed PNGs of depths 1/2/4/8
are supported by the reader; this asset requires 16 entries and indices 0..15.
RGB/RGBA and PNG transparency are rejected. GBA transparency uses index zero.
Colors are quantized to BGR555; palette bit 15 is preserved from the original.

The last tile is partial. Extra sheet cells/pixels beyond the original tile
payload must remain index zero. This prevents silently writing beyond the
loader's original VRAM allocation. The tiles retain the original decoded size.

`assets/graphics/boot-logo.map.bin` is the decoded 32x20 little-endian u16 GBA
tilemap. Its entries have a 10-bit tile number and horizontal/vertical flip
bits. Only palette bank zero is valid for this 16-color background. Preserve
the map size; tiles may be shared by many visible cells. Only fully loaded tiles
(indices 0..350) may be referenced. Tile 351 has only two bytes loaded; its
remaining bytes retain prior VRAM contents and cannot safely be displayed.
A read-only assembled
preview is generated at `build/assets/boot-logo-preview.png` (256x160, including
the rightmost two off-screen columns). Edit the tile sheet, **not** the preview.

### Identification evidence

`func_08000F5C` in `asm/intro_scene.s` loads:

| Part | ROM offset | Original slot | Decoded bytes | Reference literals |
|---|---:|---:|---:|---|
| tiles | `0x74E648` | `0x4F8` | 11234 | `0xFE8`, `0x77990` |
| palette | `0x74EB40` | `0x20` | 32 | `0xFEC`, `0x77998` |
| tilemap | `0x74EB60` | `0x16C` | 1280 | `0xFF0` |

Tiles are unpacked to `0x06000000`, the palette is copied to `0x05000000`,
and the map is unpacked to `0x0600F800`. BG control `0x1F43` selects char base
zero, screen base 31 and **4bpp**. The second tile/palette consumer is in
`asm/code_0803EE94.s` near `0x08077990`. All five references are symbolic.

## Title logo, menu words and copyright

The following indexed tile sheets are now editable under `assets/graphics/`:

| PNG | Sprite item | Depth / palette | Decoded tile bytes |
|---|---:|---|---:|
| `title-logo.png` | 0 | 8bpp / 256 colors | 15168 |
| `title-start.png` | 1 (also shared by items 2/3) | 4bpp / 16 colors | 448 |
| `title-continue.png` | 5 (also shared by items 6/7) | 4bpp / Start's shared 16 colors | 672 |
| `title-copyright.png` | 9 | 4bpp / 16 colors | 1920 |

These four disjoint pixel ranges cover all tiles in the
19468-byte resource unpacked from `0x747A74`. Keep the original indexed palette
length, sheet dimensions and zero padding. Do not add/reorder tiles. All
animation states sharing those tiles see the edit. Their OAM layouts and
animation metadata are retained unchanged, not editable BG maps.

**Shared palette:** edit **`title-start.png`** to change the colors used by
both Start and Continue. Continue has no independent runtime palette. Fresh
extraction gives both PNGs Start's palette; Continue's PNG palette is a
read-only reference. Editing it is rejected with an instruction to edit Start
instead. Do not copy a newly edited Start palette into Continue's source PNG;
use the generated `title-continue-preview.png` to see its effective colors.
Legacy Continue PNGs with their original, unused resource palette are also
accepted unchanged (their transparent color 0 differs in the original ROM).
No extraction overwrites existing files unless forced. The unused Continue
palette bytes in the resource always remain byte-identical, even when Start's
palette changes. Copyright has its own independent, editable 16-color palette.

**Load-site verification:** `func_08000914` in `asm/intro_scene.s`, literal
`0xCB4`, calls `Unpack` on `gUnk_08747A74`, then constructs its resource object
at scene offset `0x6A10` with `func_0805E6CC`. That constructor parses seven
counted sections (animations, 16-byte items, 8-byte OBJ records, 32-byte tile
units, 32-byte palette units, extra data, animation frames). In
`asm/code_0803EE94.s`, `func_0805E790` resolves each item's OBJ layout, tile
range and palette range. Its vtable entry at `0xE79D8` is called by
`func_080019D8` while setting up the title/intro objects; tiles are copied by
`func_08008E64` into allocated `0x06010000 + tile_base*32`, palettes into
`0x05000200 + palette_base*32`. OBJ attribute bit 13 proves item 0 is **8bpp**
and the word/copyright items **4bpp**; their resource palette counts are
respectively 16 banks and one bank. These resource descriptors alone do **not**
establish which palette the game actually copies or selects:

| Item | `func_080019D8` palette copy | Draw selection in `func_080024E0` |
|---|---|---|
| Logo (0) | Copy at `0x08001C4A`: source palette + `0x120`, `0xE0` bytes to `0x05000320` | 8bpp OBJ uses global OBJ colors; only 144..255 come from this source |
| Start (1) | Copy at `0x0800224A`: 32 bytes to palette allocation at scene `+0x6D00` | `0x0800268C`: allocation `+0x6D00` |
| Continue (5) | **No palette copy** between its item lookup at `0x08002340` and tile copy at `0x0800237E` | `0x0800271E`: reuses the same allocation in `r6`, `+0x6D00` |
| Copyright (9) | Copy at `0x080023E2`: 32 bytes to its own allocation at scene `+0x6D10` | `0x080025F6`: allocation `+0x6D10` |

`func_08000914` binds animations 1/4/7 to scene objects at `+0x6BE0`,
`+0x6C00`, `+0x6C20`; their first frames resolve to resource items 1/5/9.
Continue is drawn on the existing-save menu (`scene +0x6C6D == 2`), using
Start's palette even though its descriptor names a separate source palette.
`func_0805E99C` adds the selected allocation to the OBJ palette bank and emits
OAM entries. This explains why changing Start recolors an unedited Continue,
while changing Continue's resource palette has no in-game effect.
There is no BG tilemap load for these objects: their map is the resource's
OBJ layout section, used by that draw path. Display control `0x1F40` enables
1D OBJ tile addressing, as used by the preview renderer.

The actual packed resource ends at `0x74A1D8`, **not** at the next old asm
label `0x74A9C0`. The old incbin also contained four unrelated compressed
background maps. It is split into five fixed slots so resource growth cannot
overwrite them. Only the root resource's existing symbolic label moves when
its audited reference list allows overflow.

The conservative interior-address scan initially blocked even a one-pixel
title edit: pointer-shaped words at `0x52824F` (`0x08749059`) and `0x719643`
(`0x08747F21`) occur in **encoded data**, not pointer fields. Only for title
resource relocation, two audited Unpack payloads are excluded from that
interior scan: `0x5275D0` (packed `0x21C4`, decoded 18432, format 230) and
`0x7180E0` (packed `0x29F8`, decoded 32768, format 231). The former is loaded
by the Unpack call near `0x0806ECBC`; the latter has source-table references
at `0x10665C` and `0x106684`. Each stream is decoded/validated before its
consumed payload range can be excluded; the header, unused slot suffix and
all other ROM bytes remain checked. Root-reference matching is never waived.
These exclusions identify compressed byte structure, not those screens' bpp
or palette, and do not increase the identified graphics count.

## Shared opening/title scenery

`title-background.png` is a 1024-tile **4bpp** sheet carrying a 256-entry
palette. Each tile's displayed bank is chosen from its first original map
reference (bank zero for unreferenced tiles). Keep pixel indices in that
tile's 16-entry bank; the builder strips the bank nibble back to 4bpp. A tile
used under several banks still has one underlying pixel pattern. Edit palette
colors without reordering indices. The four `title-background-N.map.bin`
files are row-major **64x32** little-endian GBA entries (tile 0..1023, flips,
palette bank 0..15). Preserve their 4096-byte sizes.

`func_080019D8` verifies this ownership chain in `asm/intro_scene.s`:

| Part | ROM offset / slot | Load site / destination |
|---|---|---|
| tiles | `0x74A9C0 / 0x3970` | literal `0x1DC4`, Unpack to `0x06000000` (32768 bytes) |
| palette | `0x74E330 / 0x200` | literal `0x1DC8`, copy to `0x05000000` (512 bytes) |
| map 0 | `0x74A1D8 / 0x3DC` | `gUnk_080E8684[0]` |
| map 1 | `0x74A5B4 / 0x21C` | `gUnk_080E8684[1]` |
| map 2 | `0x74A7D0 / 0xF4` | `gUnk_080E8684[2]` |
| map 3 | `0x74A8C4 / 0xFC` | `gUnk_080E8684[3]` |

The loop at `0x08001AC0` unpacks each map to the stack, then `0x08001AF2`
splits its row-major left/right 32-column halves into hardware screen blocks
at `0x0600C000 + layer*0x1000` and `+0x800`. BG controls
`0x5843/0x5A42/0x5C41/0x5E40` select char base zero, 64x32 maps and **4bpp**
(bit 7 clear). These are full scenery/character layers, not English raster
text. Their pointers remain raw words in the table, so map recompressions
must fit the fixed original slots; unsafe growth is rejected. Tile/palette
roots have audited symbolic references and may relocate safely.

### Previews and scope

Every build assembles the four OBJ layouts and four full scenery maps into
`build/assets/title-*-preview.png`. These are review images, not editable
sources; the scenery images do not represent the scene's final composite or
animated scrolling. To copy them to an existing review directory:

```sh
python3 tools/assets/graphics_assets.py build --previews /home/ubuntu/screenshots
```

This produces `/home/ubuntu/screenshots/gfx_title-*-preview.png`.

Name entry was traced through `func_08007078` → `func_08005A58` and its
`func_08005CDC`/`func_080064E8` drawing paths in `asm/new_game.s`. Its labels
and typed name use `func_0804E8F0` text rendering (for example the
`gUnk_080E8BF4` label), not an independently identified Unpack English bitmap.
Its BG chrome/common character tiles are copied via `gUnk_080F9F70/74/78`,
with generated tilemap entries; those raw assets and the non-script strings
are not newly editable here. The existing font tools cover the font-rendered
letters, not those raw UI tiles. Other menus/banners have **not been
exhaustively audited**. Unverified loader families are not exported under
guessed screen names or palettes.

## World maps, signs and chicken-feeding background

The initial additional catalogue made **264 more Unpack streams** editable, for
**272/356 total**. This is **261 world-map streams** (30 tile sheets,
45 compressed palettes, 186 tilemaps) plus the chicken-feeding background
(one tile sheet and two tilemaps, with one raw palette). There are 63 supported
world-map graphics definitions and one chicken-feeding definition. A stream
shared by several definitions is extracted only once.

```sh
make extract
python3 tools/assets/graphics_assets.py build --previews /home/ubuntu/screenshots
python3 tools/assets/graphics_assets.py roundtrip
make -j8 compare
python3 tools/assets/check_more_graphics_rom.py --output /home/ubuntu/more-graphics-edit.gba
```

Sources live in `assets/graphics/more/`, which is ignored by Git:

* `catalog.json` maps each scene to its tile/palette/tilemap ROM offsets,
  dimensions and audited root references. This is informational, not editable
  configuration. Map numbers are **graphics definition indices**, not logical
  locations. `GetMapGraphicsVariantId` in `src/code_0803A8A4.cc` maps logical
  locations, seasons and building upgrades to these indices.
* `XXXXXX.tiles.png` is an editable indexed **4bpp** tile sheet, using one
  verified consumer's primary palette and original tilemap palette-bank hints.
  Keep dimensions and each tile's high four palette-index bits. Unused tiles
  use bank 1 for world maps (bank 0 for chicken-feeding). A tile used with
  multiple banks has one sheet preview bank; its tilemap previews show the
  other usages. Padding pixels must remain zero.
* `XXXXXX.palette.png` is the **single palette owner** for that stream. Edit
  its indexed colors, not the swatch pixels. Palette edits in tile sheets fail
  explicitly instead of being ignored or overriding another file. The
  secondary/time-of-day palettes are separate owner files when distinct.
* `XXXXXX.map.bin` contains little-endian u16, **row-major**, exactly
  `width * height` entries (not GBA screen-block ordering). Each entry holds
  tile index 0..1023, X/Y flip bits 10/11, and palette bank bits 12..15.
  Preserve dimensions; invalid tile references are rejected. Tile 1023 must
  remain empty because missing layers and runtime patches use it as blank.

This includes the farm and its signs (definitions 0/1), farmhouse upgrades
(41..43), coop upgrades (36/37), barn upgrades (38/39), mine variants
(56..59/61), other outdoor areas and interiors. The files are keyed by source
offset, not guessed English screen names. Follow `catalog.json` for the full
list and shared ownership. Previews are
`/home/ubuntu/screenshots/gfx_more_map-NN-layer-L.png` and
`gfx_more_chicken-feeding-layer-L.png` (188 layers). These show primary-palette
layers independently, not final composited gameplay, lighting interpolation,
door/crop patches, animated water, actors or text windows. Transparent pixels
use palette index zero in the review image. Tile sheets are source art; do not
edit the previews.

### Runtime verification

`gMapDefinitions` at `0x105EDC` contains 66 `MapData` records of 40 bytes
(`include/unknown_types.hh`, `GetMapData` in `src/g1_map.cc`). For every supported
record the first six pointers are followed, not discovered through a stream
signature scan:

| Record field | Loader and destination |
|---|---|
| +00, tile graphics | `LoadMapBackgroundTiles`, `src/g1_npc.cc`, `0x080A5EA0`: Unpack to `0x06000000`; 32768 bytes = 1024 complete 4bpp tiles |
| +04/+08, primary/alternate palettes | `ReloadMapPalette`, `src/g1_map_reload.cc`, `0x080A5DB8`; modes 0/1/2 use primary, mode 3 uses alternate, 480-byte buffers |
| +0C/+10/+14, three layers | `ReloadMapTilemaps`, `asm/g1_npc.s`, `0x080A5CC0`; Unpack to three layer buffers, or fill null layers with `0x03FF03FF` |
| +20/+22, width/height | used as row stride by `func_080A4F50` and its layer copy path, and `ApplyMapPatch` |

The display loop at `0x080A4FCC` builds BG control words from screen-block
indices, layer priorities, and `0x40`: char base zero, bit 7 clear (**4bpp**).
The scrolling map copy at `0x080A5670` transfers entries unchanged in 31-column
rows; tile indices, bank bits and flip bits therefore retain their meanings.
The palette frame-task path copies/interpolates the 480 bytes into BG palette
RAM **`0x05000020`**, reserving bank 0 for other scene/UI ownership. Preview
palettes explicitly pad this bank; it is not an editable guessed palette.
Map edits referencing nonblank world tiles through bank zero fail clearly.

At that checkpoint three definitions were excluded (see the continuation
below for current guarded support): **11 and 15** use nonblank
bank-zero tiles whose palette ownership was not established; **62** uses
special palette overrides in `MapGraphics_LoadPaletteBuffer` at `0x080CFB5C`
(`src/g5_map_graphics.cc`). That path unpacks the primary palette, then copies
`gUnk_086D6368` to bytes `0x100..0x15F` and `gUnk_08699FF4` to
`0x160..0x1DF`. The primary palette is shared with supported definitions;
edits to those overwritten bytes are conservatively rejected. Eleven unique
streams belonging only to the excluded definitions remain unexported. These
are limitations, not inferred palettes or a claim that every map is covered.

`LoadChickenFeedingBackground` in `src/chicken_feeding.cc` (`0x080AE7D0`)
first clears tile 1023 at `0x06007FE0`, then unpacks `0x72FC34` to BG2's
`0x0600F800` map and `0x72FE6C` to BG1's `0x0600F000` map. Both maps are 32x32.
It unpacks the 29696-byte tile stream `0x72FEF8` to char base zero and copies
the complete 512-byte palette at `0x731940` to `0x05000000`. BG controls
`0x1E41/0x1F42` prove **4bpp**, char base zero and 32x32 maps. Literal roots
are at `0xAE87C/0xAE880/0xAE884/0xAE888` respectively. The sprites/portraits
used by the minigame are not part of this background family.

The map table fields and these four chicken-feeding code references are now
symbolic. Unchanged streams preserve their complete original packed slots;
larger edits use the existing graphics overflow section **only** if the
whole-ROM root/interior-reference audit passes. Pointer-shaped words in other
data are not silently repointed or blanket-excluded. Consequently some edits
still fail conservatively, even though their map-table roots are symbolic.
`check_more_graphics_rom.py` demonstrates a safe one-pixel relocation and
checks all 265 payloads, every audited pointer, and unrelated original bytes.

### Farm edits and relocation audit

The farm tile stream `0x69B730` already has three symbolic map-table roots:
`0x105EDC`, `0x10688C`, and `0x1068B4`. The earlier growth rejection was not
caused by a remaining raw map-table field. Six **non-pointer** byte sequences
looked like pointers into its packed slot:

* `0x1846F` and `0xACD99` cross the Thumb instruction sequence
  `ldr r0,[r1,#20]; ldr r2,[r0,#28]; adds r0,r1,#0`. Original-ELF disassembly
  establishes that these are instructions, not address literals. Their exact
  six instruction bytes are checked before excluding either range.
* `0x68665B`, `0x6A1202`, `0x71D159`, and `0x72D374` are encoded bits inside
  Unpack streams at `0x681F2C`, `0x6A018C`, `0x71B474`, and `0x72C84C`.
  The first three are code-owned map streams (including the tile data for
  excluded definition 15, whose palette ownership is unresolved). The last
  has a direct Unpack call in `asm/code_809E804.s` through literal `0xB5764`
  to `0x06004000`.

The relocation audit now excludes only decoded-and-validated **encoded
payloads** of this family, excluded definition 15's tile stream and that
additional loader-owned stream, plus the two exact instruction ranges.
Headers and trailing bytes stay audited. Both
root-shaped and interior-shaped words are treated consistently; an unknown
actual pointer still rejects growth. The original ROM pointer index and
validated exclusions are cached, not rescanned/redecoded per stream.

Three additional genuine references were found at `0xB573C`, `0xB574C`, and
`0xB5750`, to streams `0x6FD240`, `0x6FB004`, and `0x6FD19C` respectively.
These loads are already symbolic in `asm/code_809E804.s`; the corresponding
data labels now alias the relocating `gMoreGraphics_` symbols instead of
remaining fixed at their original slots. They are included in the root audit.

On the original US ROM, **193/264** exported compressed streams pass the
**complete** root and interior audit and can relocate, including the farm.
The other 71 remain conservatively fixed because their other pointer-shaped
words have not been classified. Run `roundtrip` to reproduce the count. This
is not a blanket waiver of interior references.

The four-solid-tile integration sample is reproducible with:

```sh
python3 tools/assets/check_more_graphics_rom.py --farm-block --output /home/ubuntu/farm-block-edit.gba
```

It paints a 2x2 source-sheet block of farm tiles, rebuilds normally, follows
all three farm roots, checks the complete decoded result and all 264 other
payloads, and restores the source and vanilla ROM. Its layer preview is
`/home/ubuntu/screenshots/gfx_more_farm-block-edit-layer-0.png`.
The sample paints tiles **0, 1, 32, 33**; its optimized stream is **17,672
bytes** versus **18,100 greedy** and the **15,280-byte** original slot. It
therefore exercises real overflow relocation to `0x75C244`, not just an
in-place build benefiting from better compression.

No additional independent English menu/name-entry/shop/calendar/diary bitmap
or compressed portrait family was verified in this pass. Font-rendered labels
belong to the existing non-script text/font tools (see `editing_text.md`), not
guessed graphics resources. The common map loader was chosen to cover signs
and many shared backgrounds without inventing palette ownership.

## Compression, unchanged builds and growth

`tools/assets/unpack_codec.py` writes the game's custom `0x70` Unpack format:
little-endian 32-bit words carrying MSB-first bits, raw atoms, LZSS method 0,
two distance classes (8/12 bits), literal groups, repeated-byte runs and
overlapping back-references. Matching searches the complete 4352-byte window.
Dynamic programming minimizes the **bit cost** of literals (all lengths
1..64), runs (2..65), and both lookup classes (3..66), rather than committing
to the longest local match. For each distance class, its longest match covers
all shorter choices at the same token cost. All five inverse difference
filters are tried (word/interleaved only for even-sized input). The smallest
word-padded candidate wins; the old deterministic greedy stream is retained
as a fallback, so optimized output is never larger than it. Huffman encoding
and other LZSS grammars are not implemented by this encoder, so the original
compressor can still produce smaller results. The bounded decoder reuses
the existing format implementation from `tools/scripts/decompress.py`, including
Huffman atom coding, the other LZSS methods and difference filters.

This encoder is **not** a reconstruction of the original compressor. Therefore
unchanged decoded assets reuse their original compressed slots verbatim. This
keeps `make compare` exact even when a greedy recompression differs. Edited
streams fitting the original slot are rebuilt there without moving its end.

Larger edited streams move to a word-aligned `.graphics_overflow` section after
the original IWRAM image (alongside script/font overflow). The original slot
and every later byte's placement stay fixed. Only the existing symbolic labels
are moved. Before allowing growth, the tool checks the original ROM for every
occurrence of the asset's address, including unaligned occurrences, against the
audited symbolic-reference list. Any additional/interior pointer makes growth
fail with a clear error. Unmapped assets cannot be relocated by this tool.
The existing linker padding/32MiB limit applies. Do not slide the opaque suffix.

Generated bins, assembly, a layout report and the assembled preview live under
`build/assets/`. Unchanged generator results are not rewritten. Palette and
tilemap validation happens before build products are replaced.

## Inventory and checks

```sh
make assets-test
python3 tools/assets/graphics_assets.py roundtrip
make clean && make -j8 compare
python3 tools/assets/check_graphics_rom.py
python3 tools/assets/check_graphics_rom.py --title
```

`roundtrip` scans word-aligned headers in `0x527094..0x75B9F8`, validates complete
streams (max 256KiB output/source window), recompresses each valid candidate and
checks decoded equality. It reports optimized/original/greedy byte totals and
smaller/equal/larger counts, and writes per-stream optimized and greedy sizes
to `build/assets/graphics-inventory.json`, including formats, sizes and
aligned ROM pointers. **A valid stream is a candidate, not proof of a separately
owned graphics asset.** Embedded/overlapping candidates and non-graphics data
are not promoted to editable assets automatically. No BIOS-LZ77 inventory is
claimed by this custom-format scan.

The original US ROM yields **356** valid aligned candidates in **21** format
combinations; **353** have aligned ROM pointers. All 356 round-trip through the
greedy encoder, but **0/356** reproduce the original packed bytes.
The optimized encoder round-trips **356/356** and writes **818,632 bytes**
versus **721,840 original** (113.4%) and **871,164 greedy** (94.0%). Against
the originals, **17 are smaller, 4 equal-sized, 335 larger**; 21 fit their
original slots, versus 6 with greedy. This improves recompression, not full
parity with the original Huffman/LZSS compressor. Per-stream sizes are in the
inventory JSON; unchanged builds still keep the original packed bytes.
**272/356 streams** are now identified and editable: the original eight
Natsume/title streams, 261 world-map streams, and three chicken-feeding
background streams. Raw palettes and the four exported title OBJ images do
not add separate compressed streams to that count.

The local-ROM integration checker changes one indexed pixel, rebuilds, follows
both symbolic tile pointers, checks the exact decoded-byte change and verifies
that changes elsewhere are confined to the reserved slot/pointer words/overflow
space. It saves `build/assets/graphics-pixel-edit.gba`, restores the PNG and runs
compare again. These are binary/layout checks, not emulator visual verification.
With `--title` it edits one 8bpp title-logo pixel inside the container, follows
its root pointer, checks all other decoded metadata/pixels and every byte
outside the owned slot/reference/overflow, and saves
`build/assets/title-pixel-edit.gba` before restoration.

## Remaining-graphics increment: portraits and childhood illustrations

The additional editable set is **21 Unpack streams** (20 illustration maps and
one tile sheet), bringing compressed coverage to **293/356 inventory candidates**.
The **184 dialogue portrait frames** are also editable, but are raw animation
data and therefore do not increase that denominator. The other 63 candidates,
including special map definitions 11/15/62, are not made editable by this increment.

`make extract` creates `assets/graphics/portraits/000.png` through `183.png`.
These are indexed 4bpp tile sheets, named by animation **frame**, not NPC ID.
`graphics_assets.py build --previews /home/ubuntu/screenshots` assembles their
actual OAM layouts as `gfx_rest_portrait-NNN.png`.

Runtime ownership is established by `SetMessageWindowStatePortrait` at
`0x08050AD8`, which calls `LoadMessagePortraitAnimationFrame` at `0x080ADCA4`.
The latter initializes the raw resource `gUnk_0852D984`, resolves a clip's first
keyframe, and obtains its frame through `GetSpriteAnimationDataFrame`.
The seven section layouts come from `InitSpriteAnimationData` in
`src/sprite_animation.cc`. All 184 frames select 4bpp OAM objects and exactly
one 16-color palette. The message-window update path at `0x0804FF12..0x0804FFDC`
copies the selected frame's tiles to allocated OBJ VRAM and its palette to the
allocated OBJ palette bank; `DrawSpriteFrameObjects` consumes its OAM objects.
The resource ends at `0x58BA08`; the following 32 bytes are preserved verbatim.

Frames can share tiles or palettes. Only changes relative to the original
exports are applied, so an untouched frame does not undo another frame's edit.
Conflicting changes to shared tile bytes or whole BGR555 colors fail rather than
depending on file order. Shared edits affect every frame that uses those bytes;
regenerate assembled previews to review them. Preserve PNG dimensions and zero
padding. Animation records and tile counts remain fixed; these edits cannot
grow or relocate the raw container.

Childhood-memory sources live in `assets/graphics/rest/`:

* `childhood.tiles.png`: banked 4bpp tile sheet, read-only embedded palette.
* `childhood.palette.png`: sole owner of the 32 runtime-loaded colors.
* `childhood-00.map.bin` through `childhood-19.map.bin`: 32x20 row-major maps.

The background initializer in `src/event_scenes.cc` unpacks `gUnk_08743058`
to VRAM and the 20 resources in `gUnk_080FB1E4` into map buffers, subsequently
queued for BG3/BG1 DMA. Its BG controls establish 4bpp. The palette DMA copies
only the first 64 bytes of `gUnk_08747A14`; color zero is then overwritten white.
The tool rejects changes to that overwritten color and references to unloaded
palette banks. The other 32 source palette bytes remain untouched and are not
exported. Pointer-table entries and the tile/palette roots are symbolic; growth
still requires the non-symbolic/interior-reference audit to pass.

`gfx_rest_childhood-NN.png` previews show each of the 20 layers separately.
Run `python3 tools/assets/check_rest_graphics_rom.py` after a clean comparison
to paint one OAM-selected portrait pixel, verify exactly one changed ROM byte,
save `/home/ubuntu/rest-graphics-edit.gba`, restore the source, and compare again.
This does not verify gameplay in an emulator.

### Additional world relocation classifications

`graphics_instruction_audit.json` records 437 previously unresolved reference
positions proved to fall wholly inside Thumb instruction regions, not literal
pools, using the vanilla ELF's `$t`/`$d` mappings. Each minimal halfword-aligned
region has a SHA-256 guard against the original ROM; clean builds do not depend
on an existing ELF. A mismatch fails closed. Two raw sprite-animation resources
at `0x52D984..0x58BA08` and `0x58BA28..0x667060` account for another 89 matches;
their seven count-delimited sections are validated before being excluded.
Their tables contain relative indices/OAM fields/pixels/colors, not ROM pointers.

These classifications raised relocation eligibility from **193 to 216 of 264**
world/chicken-feeding streams, leaving **48** conservatively blocked. Unknown
literal/data references are not repointed or ignored. This is not a complete
classification of the original 71 failures.

## UI, minigame and special-map continuation

Current coverage is **353 editable rooted Unpack streams**: boot/title 8,
world/chicken-feeding 280, childhood-memory 21, UI/minigames/world patches 44.
This increment adds **60** editable streams and classifies the other **three**
of the previous 63 candidates. There are **zero unclassified candidates**,
but this does not make raw sprite containers generally editable or prove every
possible image in the ROM has been found.

The inventory's original denominator of 356 includes three incidental valid
decodes, not independently loaded compressed resources:

* `0x52C2D4` lies inside the validated compressed winter farm-overview tiles
  at `0x52AA6C`; changing the parent owns those bytes.
* `0x65CCFC` lies inside the validated raw tile section of the sprite animation
  container `0x58BA28..0x667060`, not at a resource or compressed-frame boundary.
* `0x699570` lies inside raw tiles `0x698E14..0x699FF4`; the callers at
  `0x080A81D2` and `0x080CFD96` DMA-copy this source, not Unpack it.

All three lack aligned ROM roots. `incidental_candidates` checks their owners
and root evidence; roundtrip annotates `graphics-inventory.json`, still tests
all 356 decoder candidates, and reports the three separately from editable
streams. No palette is invented for those incidental decodes.

### Sources and verified ownership

New sources live in `assets/graphics/ui/`:

* `ADDRESS.tiles.png`: bank-tagged indexed 4bpp tiles. Edit pixel indices only.
* `ADDRESS.palette.png`: the single owner of that resource's loaded colors.
  Do not edit swatch indices; high palette bits are preserved.
* `ADDRESS.map.bin`: little-endian 16-bit GBA map cells. Preserve dimensions.

Shared resources are exported once. Palettes already owned by the world
catalogue stay in `assets/graphics/more/`; the looping logo uses the existing
`assets/graphics/boot-logo.png`. Atlas colors are read-only snapshots of the
original owning palette, so separate palette edits do not require recoloring
the atlas file. All dimensions, complete-tile bounds and loaded palette banks
are validated before building. Padding and partially loaded tiles are not new
editable image space.

| Family | Tile stream(s) | Load-site and layout evidence |
| --- | --- | --- |
| Farm overview, normal/winter | `5275D0`, `52AA6C` | `func_0806EC94`: tiles to `06000000`, 512 palette bytes to `05000000`, three maps passed to `func_0806EB70`; BG controls `DC40/D840/D440` establish 4bpp. Maps contain 64x44 cells and are copied into runtime BG layout. |
| Town map, normal/winter | `755848`, `75822C` | `func_08077EC0`: tiles to `06000000`, 192 palette bytes to `05000020`, two raw 30x13 maps through `func_08077E34`. The caller at `08078B6C` and BG setup establish the 4bpp map UI. |
| Minigame panel `b7164` | `72D6D8` | Loader literals `080B72E8..080B7300` select two 32x32 maps, tiles, and 96 palette bytes; BG controls `1D40/1F42` establish 4bpp. |
| Minigame panel `bd0` | `73BFF0` | `080BD04C..080BD058` select two maps, tiles and 192 palette bytes; controls `1E41/1F42`, 4bpp. The final partial tile is unavailable to map edits. |
| Minigame panel `c16` | `73F6AC` | `080C16B8..080C16C4` select maps, tiles and 192 palette bytes loaded at `05000020`; controls `1E41/1F42`, 4bpp. |
| Minigame panel `a2f` | `74F050` | `080A2FD0..080A2FE0` select three 32x20 maps, tiles and 96 palette bytes; controls `1C43/1D41/1E42`, 4bpp. |
| Mixed-palette panel `b55` | `72C84C` | `080B5754..080B576C` select two 32x32 maps and tiles at `06004000`. Unpack loads existing `6FD19C` colors at `05000000`, not the world's `05000020`; 32 bytes from `72D5AC` override bank 7 at `050000E0`. Controls `1D46/1F42` establish 4bpp. |
| Horse race | `72F21C` | `LoadHorseRaceCourseGraphics` in `src/race_display.cc` selects four **6x32 repeating strips**, tiles and the palette at `72FA9C`. BG configuration and 32-byte tile addressing establish 4bpp. |
| Frisbee | `738D1C`, `739A64` | `LoadFrisbeeBackground` selects three maps, tiles and palette `7399C4`; BG controls `1C43/1D42/1E41` establish 4bpp. Timer code at `080534A4` copies one of four **1536-byte / 48-tile** frames from the unpacked `739A64` buffer to VRAM zero. Each frame is validated against the maps. |
| Looping shared logo map | map `74ECCC` | Loader around `08077970` shares the boot tiles and palette, unpacks this 32x64 map, explicitly clears tile 1023, and sets backdrop color zero white. Only complete loaded tiles or that cleared blank can be referenced. |

The address-based minigame names deliberately avoid guessing scene semantics.
Their previews expose the actual artwork. Town-map dynamic overlay resources
are not newly exported. Frisbee map `738CF0` overwrites five rows of another
map at runtime: the preview shows that patch separately, not a claimed final
composite. Horse-race previews show the source strips before repetition.

### Seven additional world patch tile streams

`ui_graphics.PATCHES` records seven extended patch structures at
`107204/107230/10725C/107288/1072B4/1072E0/10730C`. Their tile sources are
`6ECAAC/6ECEF0/6F292C/6EF994/6F07C0/6ED57C/6ED9F0`.
The type-6 branches at `080A924C..080A93DC` Unpack structure field `+24`
to VRAM at tile index `field(+28)`. `ApplyMapPatch` consumes the two raw maps
at `+4/+8`. The palette branches at `080A9E42..080AA1A6` select `+32` or
winter `+36`, copy `byte(+41)*32` bytes to palette bank `byte(+40)` through
the common `05000020` transfer. The common map BG setup establishes 4bpp.

`GetMapGraphicsVariantId` maps location type 6 to definition 2 or winter 3;
their base palettes `6FD19C/6FFA84` own the ground/shadow colors used alongside
the patch-specific colors. Previews and atlases combine these verified owners,
without duplicating their editable files. Patch previews isolate downloaded
tiles and omit cells belonging to the unchanged base-world tile sheet.
Existing external cells are preserved; new references must target loaded patch
tiles. Generated 44-byte table records replace only resource pointer fields
with symbols; unrelated terrain pointers, dimensions and destination indices
retain their original bytes. Growth still requires the reference audit.

### Definitions 11/15/62 and conditional palettes

The world catalogue now includes all 66 definitions, plus palette/context
variants. Five additional conditional palettes (`714A30/714B60/714BEC/716F84/
7170B8`) follow the map-16/17 selection branches at `080A960C..080A96B4`.
They share the existing tile/map files; only their owning palette PNGs are new.

**No bank-zero palette was guessed.** Definition 11's tile 399 and definition
15's tile 647 still contain nonblank pixels with unproven bank-zero ownership.
Those two tile patterns are **read-only**, including shared contexts; their
existing bank-zero map cells can remain or be removed but cannot be introduced
elsewhere. Previews mask those cells rather than assigning a false color.
All other verified tiles, palettes and maps in those definitions are editable.
Existing atlas-owner ordering is retained so new scenes do not reinterpret
previously extracted PNG bank tags.

Definition 62 has both a normal G1 preview and a `map-62-g5` preview with the
two `MapGraphics_LoadPaletteBuffer` overrides applied. The shared overwritten
palette ranges remain read-only; the raw overriding resources are not given
competing palette owners. This distinguishes contexts rather than pretending
one palette represents both.

### Palette read-past protection and checks

Some minigame loaders copy 512 bytes although their owned palettes are shorter.
Only 32 bytes at `72D5AC`, 352 at `72FA9C` and 160 at `7399C4` are exported.
Neighboring streams inside the copied tails must leave their original bytes
in place: **any edit forces audited relocation even if it would fit**.
This protects the B55-overlapping next panel, horse-race alternate strip and
the already editable chicken-feeding map, and the Frisbee animation prefix.
An unknown reference fails clearly rather than silently changing another
scene's loaded colors. Raw palette sizes remain fixed.

The B55 panel loader first unpacks the parent palette at `080B561C`, then
DMA-copies 512 bytes from `72D5AC` to `050000E0` at `080B5640`.
Only the first 32 source bytes belong to its panel palette (bank 7); the
following compressed bytes overwrite BG banks 8–15 (and part of OBJ palette
RAM). Parent-owned banks 0–6 and panel-owned bank 7 remain usable. B55 maps
reject banks 8–15, including blank-tile references, rather than previewing
parent colors the loader overwrites. The original two maps use only banks
0/2 and 0/7 respectively, so unmodified sources remain byte-identical.

```sh
make assets-test
python3 tools/assets/graphics_assets.py roundtrip --output build/assets
make clean && make -j8 compare
python3 tools/assets/check_ui_graphics_rom.py
```

The sample checker paints an 8x8 contrasting X into one newly editable minigame-panel
tile, verifies every world/UI/childhood payload and symbolic root, checks every
changed ROM byte belongs to that tile stream or its roots, writes
`/home/ubuntu/ui-graphics-edit.gba`, restores the PNG and runs compare again.
Preview: `/home/ubuntu/screenshots/gfx_rest_ui-panel-visible-edit.png`.
Unmodified family previews use `gfx_rest_*.png`. These are asset-layout previews,
not emulator verification of runtime scene transitions or dynamic overlays.

### Town-map and farm-overview overflow

Both normal-season tile sheets can relocate through their existing symbolic
loader roots: town map `755848` via `77F38`, farm overview `5275D0` via `6ED6C`.
No additional loader roots were found. The previously blocking words are data,
not interior references:

- `6718DC -> 757A99`: raw OBJ tiles in the seven-section `6678A0..67A0E8`
  animation resource, initialized by `InitSpriteAnimationData` in the map scene.
- `1A5C6D -> 527C7A`: signed PCM inside DirectSound sample `19B6B0`.
- `9B524 -> 527FC2`: Thumb `ldrb`/`lsrs`, not a literal pool.
- `5B57C/5B60E/5B6AE/5B75C/5B7BC -> 528A9A` and
  `5B548/5B5DA/5B67A/5B728/5B7F0 -> 528ABA`: Thumb `ldrh`/`lsrs`
  in `LoadHorseRaceObjectGraphics`, verified against ELF `$t` mappings.
- `6EAC18 -> 528286`: raw 4bpp window-style tiles. Record `107104` points
  to `6EA5B8`; its header specifies 19 tiles per frame and three frames.
  `func_080A5AB0` selects a frame and DMA-copies 32-byte tiles.
- `750633 -> 52944A`: palette colors in the seven-section `74F34C..7506E0`
  animation resource passed to `func_0805E6CC` in `func_080A220C`.

Only raw tile/palette sections of the two additional animation resources are
excluded, never their metadata. PCM, window tiles and instructions are guarded
by SHA-256; changed guarded bytes fail closed. Unknown words elsewhere still
block relocation. No compressed-payload exclusions were needed for these two
screens.

Reproduce two solid-tile edits (each restores its source PNG and compares):

```sh
python3 tools/assets/check_ui_graphics_rom.py --scene town-map --solid-tiles 0 1 --output /home/ubuntu/townmap-edit.gba
python3 tools/assets/check_ui_graphics_rom.py --scene farm-overview --solid-tiles 0 1 --output /home/ubuntu/farmoverview-edit.gba
```
