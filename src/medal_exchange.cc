#include "prelude.h"

struct MedalPrize
{
    char name[16];
    i16 price;
    STRUCT_PAD(0x12, 0x14);
};

struct MedalExchangeGlyph
{
    i16 x;
    i16 y;
    u16 tile;
    u16 style;
} PACKED ALIGN(2);

struct MedalExchange
{
    STRUCT_PAD(0x00, 0x14C);
    u16 scroll_y;
    MedalExchangeGlyph glyphs[48];
    i16 glyph_count;
    STRUCT_PAD(0x2D0, 0xAD0);
    int medals;
    STRUCT_PAD(0xAD4, 0xAE0);
    u8 prize_indices[10];
};

EC MedalPrize const gMedalExchangePrizes[] asm("gUnk_080FB000");

EC SECTION(".text.f3.0805DF68") void AppendMedalExchangeGlyph(MedalExchange * self, int x, int y,
    int tile, int style)
{
    self->glyphs[self->glyph_count].x = x;
    self->glyphs[self->glyph_count].y = y;
    self->glyphs[self->glyph_count].tile = tile;
    self->glyphs[self->glyph_count].style = style;
    ++self->glyph_count;
}

EC SECTION(".text.f3.0805DEC0") void AppendMedalExchangeNumber(MedalExchange * self, int x, int y,
    int style, int number, int place, int suffix)
{
    // Convert the requested width to its leading decimal place.
    int digit;
    if (place > 1)
    {
        --place;
        digit = 10;
        while (place > 1)
        {
            digit *= 10;
            --place;
        }
        place = digit;
    }
    else
        place = 1;

    bool draw_digit = false;
    while (place > 0)
    {
        if (place == 1)
            draw_digit = true;
        digit = number / place;
        if (digit != 0 || draw_digit)
        {
            AppendMedalExchangeGlyph(self, x, y, digit, style);
            draw_digit = true;
        }
        x += 8;
        number -= digit * place;
        place /= 10;
    }
    if (suffix != 0)
        AppendMedalExchangeGlyph(self, x, y, suffix, style);
}

EC SECTION(".text.f3.0805DE24") void UpdateMedalExchangeGlyphs(MedalExchange * self)
{
    self->glyph_count = 0;
    for (int i = 0; i < 10; ++i)
    {
        int y = i << 4;
        int scroll = self->scroll_y - 32;
        y -= scroll;
        AppendMedalExchangeNumber(self, 144, y, 3,
            gMedalExchangePrizes[self->prize_indices[i]].price, 4, 10);
    }
    int i = 0;
    int label_style = 1;
    for (int x = 8; i < 11; x += 8, ++i)
        AppendMedalExchangeGlyph(self, x, 4, i + 13, label_style);
    AppendMedalExchangeNumber(self, 136, 4, 1, self->medals, 5, 10);
}
