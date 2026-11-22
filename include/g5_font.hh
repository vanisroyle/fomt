#ifndef G5_FONT_HH
#define G5_FONT_HH

#include "prelude.h"

// ROM font indexing uses Shift-JIS lead ranges 81-9F/E0-EA, with 189
// trail-byte slots (40-FC) per lead, including the otherwise unused 7F slot.
// Returns -1 outside those ranges;
// this index is not a Unicode code point or an ASCII glyph index.
EC int GetShiftJisGlyphIndex(int encoded_character); // 0x080D0CD4

// Decodes into a work buffer of at least 0x80 bytes; returns width in 8px cells.
// Missing glyphs clear the buffer and return zero. General mappings permit a
// null buffer to query width; the special single-byte overrides require one.
// Tagged Vietnamese-font builds retain the assembly decoder and its hook.
EC int Font_DecodeGlyph(void * output, int encoded_character); // 0x080D0D28

#endif
