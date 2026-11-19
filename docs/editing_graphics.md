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
| `title-continue.png` | 5 (also shared by items 6/7) | 4bpp / 16 colors | 672 |
| `title-copyright.png` | 9 | 4bpp / 16 colors | 1920 |

These four disjoint pixel/palette ranges cover all tiles and palettes in the
19468-byte resource unpacked from `0x747A74`. Keep the original indexed palette
length, sheet dimensions and zero padding. Do not add/reorder tiles. All
animation states sharing those tiles see the edit. Their OAM layouts and
animation metadata are retained unchanged, not editable BG maps.

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
and the word/copyright items **4bpp**; their palette counts are respectively
16 banks and one bank. `func_0805E99C` applies the layout and emits OAM entries.
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

## Compression, unchanged builds and growth

`tools/assets/unpack_codec.py` writes the game's custom `0x70` Unpack format:
little-endian 32-bit words carrying MSB-first bits, raw atoms, LZSS method 0,
two distance classes (8/12 bits), literal groups, repeated-byte runs and
overlapping back-references. Matching is deterministic and greedy with a
4352-byte window. Output is padded to whole words. The bounded decoder reuses
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
checks decoded equality. It reports how often greedy output is byte-identical
and writes `build/assets/graphics-inventory.json`, including formats, sizes and
aligned ROM pointers. **A valid stream is a candidate, not proof of a separately
owned graphics asset.** Embedded/overlapping candidates and non-graphics data
are not promoted to editable assets automatically. No BIOS-LZ77 inventory is
claimed by this custom-format scan.

The original US ROM yields **356** valid aligned candidates in **21** format
combinations; **353** have aligned ROM pointers. All 356 round-trip through the
greedy encoder, but **0/356** reproduce the original packed bytes.
**8/356 streams** are now identified and editable: two Natsume streams, one
title OBJ container, one shared scenery tile stream and four scenery map
streams. Palettes and the four exported title OBJ images do not add separate
compressed streams to that count.

The local-ROM integration checker changes one indexed pixel, rebuilds, follows
both symbolic tile pointers, checks the exact decoded-byte change and verifies
that changes elsewhere are confined to the reserved slot/pointer words/overflow
space. It saves `build/assets/graphics-pixel-edit.gba`, restores the PNG and runs
compare again. These are binary/layout checks, not emulator visual verification.
With `--title` it edits one 8bpp title-logo pixel inside the container, follows
its root pointer, checks all other decoded metadata/pixels and every byte
outside the owned slot/reference/overflow, and saves
`build/assets/title-pixel-edit.gba` before restoration.
