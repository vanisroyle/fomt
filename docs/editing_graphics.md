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
the map size; tiles may be shared by many visible cells. A read-only assembled
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

The intro also directly unpacks `0x747A74` (19468 bytes, animation container,
format 220) and `0x74A9C0` (32768 bytes of VRAM graphics, format 230), with a
512-byte palette at `0x74E330`. Their container/arrangement and color-depth
ownership are not sufficiently mapped for editing here. No claim is made that
all other scanned streams are graphics, or that their palettes are known.

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
greedy encoder, but **0/356** reproduce the original packed bytes. Only the
two compressed Natsume streams and their shared palette are editable so far.

The local-ROM integration checker changes one indexed pixel, rebuilds, follows
both symbolic tile pointers, checks the exact decoded-byte change and verifies
that changes elsewhere are confined to the reserved slot/pointer words/overflow
space. It saves `build/assets/graphics-pixel-edit.gba`, restores the PNG and runs
compare again. These are binary/layout checks, not emulator visual verification.
