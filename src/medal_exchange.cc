#include "prelude.h"

struct MedalPrize
{
    char name[16];
    i16 price_in_medals;
    STRUCT_PAD(0x12, 0x14);
};

struct MedalExchangeGlyph
{
    i16 x;
    i16 y;
    u16 tile;
    u16 oam_priority;
} PACKED ALIGN(2);

struct MedalExchange
{
    enum { PRIZE_ROWS = 10, MAX_GLYPHS = 48 };

    STRUCT_PAD(0x00, 0x14C);
    u16 scroll_y;
    MedalExchangeGlyph glyphs[MAX_GLYPHS];
    i16 glyph_count;
    STRUCT_PAD(0x2D0, 0xAD0);
    int medal_balance;
    STRUCT_PAD(0xAD4, 0xAE0);
    u8 prize_indices[PRIZE_ROWS];
};

EC MedalPrize const gMedalExchangePrizes[];

EC SECTION(".text.f3.0805DF68") void AppendMedalExchangeGlyph(MedalExchange * self, int x, int y,
    int tile, int oam_priority)
{
    self->glyphs[self->glyph_count].x = x;
    self->glyphs[self->glyph_count].y = y;
    self->glyphs[self->glyph_count].tile = tile;
    self->glyphs[self->glyph_count].oam_priority = oam_priority;
    ++self->glyph_count;
}

EC SECTION(".text.f3.0805DEC0") void AppendMedalExchangeNumber(MedalExchange * self, int x, int y,
    int oam_priority, int number, int place, int suffix)
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
            AppendMedalExchangeGlyph(self, x, y, digit, oam_priority);
            draw_digit = true;
        }
        x += 8;
        number -= digit * place;
        place /= 10;
    }
    if (suffix != 0)
        AppendMedalExchangeGlyph(self, x, y, suffix, oam_priority);
}

EC SECTION(".text.f3.0805DE24") void UpdateMedalExchangeGlyphs(MedalExchange * self)
{
    enum { SUFFIX_TILE = 10, BALANCE_LABEL_START_TILE = 13, BALANCE_LABEL_TILES = 11 };

    self->glyph_count = 0;
    for (int i = 0; i < MedalExchange::PRIZE_ROWS; ++i)
    {
        int y = i << 4;
        int scroll = self->scroll_y - 32;
        y -= scroll;
        AppendMedalExchangeNumber(self, 144, y, 3,
            gMedalExchangePrizes[self->prize_indices[i]].price_in_medals, 4, SUFFIX_TILE);
    }
    int i = 0;
    int label_priority = 1;
    for (int x = 8; i < BALANCE_LABEL_TILES; x += 8, ++i)
        AppendMedalExchangeGlyph(self, x, 4, i + BALANCE_LABEL_START_TILE, label_priority);
    AppendMedalExchangeNumber(self, 136, 4, 1, self->medal_balance, 5, SUFFIX_TILE);
}
