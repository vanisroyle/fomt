#include "g5_font.hh"
#include <cstring>

#if !VIETNAMESE_FONT

typedef void (*GlyphDecoder)(void const *, void *);
EC void func_0300085C(void const *, void *);
static inline void DecodeNarrowGlyph(void const * data, void * out)
{
    register void * output asm("r1") = out;
    asm ("" : "+l" (output));
    register GlyphDecoder decoder asm("r2") = func_0300085C;
    asm ("" : "+l" (decoder));
    decoder(data, output);
}

EC void func_03000714(void const *, void *);
static inline void DecodeWideGlyph(void const * data, void * out)
{
    register void * output asm("r1") = out;
    asm ("" : "+l" (output));
    register GlyphDecoder decoder asm("r2") = func_03000714;
    asm ("" : "+l" (decoder));
    decoder(data, output);
}

EC i16 const gUnk_084FA7A0[];
EC i16 const gUnk_08523290[];
EC u8 const gUnk_084F90CC[][12];
EC u8 const gUnk_084FA9A0[][24];
EC u8 const gUnk_08117B20[];
EC u8 const gUnk_08117B2C[];
EC u8 const gUnk_08117B38[];
EC u8 const gUnk_08117B44[];
EC u8 const gUnk_08117B50[];
EC u8 const gUnk_08117B5C[];
EC u8 const gUnk_08117B68[];
EC u8 const gUnk_08117B74[];
EC u8 const gUnk_08117B80[];
EC u8 const gUnk_08117B8C[];
EC u8 const gUnk_08117B98[];
EC u8 const gUnk_08117BA4[];
EC u8 const gUnk_08117BB0[];
EC u8 const gUnk_08117BBC[];
EC u8 const gUnk_08117BC8[];

EC SECTION(".text.g5_080D0D28") int Font_DecodeGlyph(void * output, int encoded_character) // 0x080D0D28
{
    int advance_cells = 0;
    int glyph_index = -1;
    switch (encoded_character)
    {
    case 0xb4: DecodeNarrowGlyph(gUnk_08117B20, output); return 1;
    case 0xb6: DecodeNarrowGlyph(gUnk_08117B2C, output); return 1;
    case 0xb7: DecodeNarrowGlyph(gUnk_08117B38, output); return 1;
    case 0xb1: DecodeNarrowGlyph(gUnk_08117B44, output); return 1;
    case 0xb2: DecodeNarrowGlyph(gUnk_08117B50, output); return 1;
    case 0xb3: DecodeNarrowGlyph(gUnk_08117B5C, output); return 1;
    case 0xbb: DecodeNarrowGlyph(gUnk_08117B68, output); return 1;
    case 0xbc: DecodeNarrowGlyph(gUnk_08117B74, output); return 1;
    case 0xbd: DecodeNarrowGlyph(gUnk_08117B80, output); return 1;
    case 0xbe: DecodeNarrowGlyph(gUnk_08117B8C, output); return 1;
    case 0xbf: DecodeNarrowGlyph(gUnk_08117B98, output); return 1;
    case 0xc0: DecodeNarrowGlyph(gUnk_08117BA4, output); return 1;
    case 0xc1: DecodeNarrowGlyph(gUnk_08117BB0, output); return 1;
    case 0xc2: DecodeNarrowGlyph(gUnk_08117BBC, output); return 1;
    case 0xc3: DecodeNarrowGlyph(gUnk_08117BC8, output); return 1;
    }
    if (encoded_character > 0)
    {
        if (encoded_character <= 0xff)
        {
            glyph_index = gUnk_084FA7A0[encoded_character];
            advance_cells = 1;
        }
        else if (encoded_character <= 0xffff)
        {
            int shift_jis_index = GetShiftJisGlyphIndex(encoded_character);
            if (shift_jis_index >= 0)
            {
                glyph_index = gUnk_08523290[shift_jis_index];
                advance_cells = 2;
            }
        }
    }
    if (output != nullptr)
    {
        if (glyph_index >= 0)
        {
            if (advance_cells != 1)
                DecodeWideGlyph(gUnk_084FA9A0[glyph_index], output);
            else
                DecodeNarrowGlyph(gUnk_084F90CC[glyph_index], output);
        }
        else
            memset(output, 0, 0x80);
    }
    int return_width = 0;
    if (glyph_index >= 0)
        return_width = advance_cells;
    return return_width;
}

#endif
