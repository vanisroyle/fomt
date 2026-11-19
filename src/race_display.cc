#include "prelude.h"

struct RaceGlyph
{
    i16 x;
    i16 y;
    u16 tile;
    u16 oam_priority;
} PACKED ALIGN(2);

struct RaceEntrant
{
    STRUCT_PAD(0, 0x28);
    i16 odds;
    i16 medals_wagered;
};

struct RaceDisplay
{
    STRUCT_PAD(0, 0x738);
    RaceGlyph glyphs[148];
    i16 glyph_count;
    STRUCT_PAD(0xBDA, 0x13FE);
    i16 selected_racer;
    STRUCT_PAD(0x1400, 0x1414);
    int entry_funds;
    int medal_balance;
    i16 race_index;
    i16 medal_payout;
    STRUCT_PAD(0x1420, 0x142C);
    RaceEntrant racers[4];
};

EC SECTION(".text.f3.0805A6E4") void AppendRaceDisplayGlyph(RaceDisplay * self, int x, int y,
    int tile, int oam_priority)
{
    self->glyphs[self->glyph_count].x = x;
    self->glyphs[self->glyph_count].y = y;
    self->glyphs[self->glyph_count].tile = tile;
    self->glyphs[self->glyph_count].oam_priority = oam_priority;
    ++self->glyph_count;
}

EC SECTION(".text.f3.0805A63C") void AppendRaceDisplayNumber(RaceDisplay * self, int x, int y,
    int oam_priority, int number, int width, int suffix)
{
    int place = width;
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
            AppendRaceDisplayGlyph(self, x, y, digit, oam_priority);
            draw_digit = true;
        }
        x += 8;
        number -= digit * place;
        place /= 10;
    }
    if (suffix != 0)
        AppendRaceDisplayGlyph(self, x, y, suffix, oam_priority);
}

EC SECTION(".text.f3.0805A4DC") void UpdateRaceDisplayGlyphs(RaceDisplay * self, u32 display_bets)
{
    self->glyph_count = 0;
    if (!display_bets)
    {
        AppendRaceDisplayGlyph(self, 16, 4, 24, 1);
        AppendRaceDisplayNumber(self, 40, 4, 1, self->entry_funds, 10, 12);
    }
    else
    {
        int i = 0;
        int label_priority = 1;
        for (int x = 8; i < 11; x += 8, ++i)
            AppendRaceDisplayGlyph(self, x, 4, i + 13, label_priority);
        AppendRaceDisplayNumber(self, 136, 4, 1, self->medal_balance, 5, 10);
        AppendRaceDisplayNumber(self, 160, 120, 2, self->medal_payout, 4, 10);
        AppendRaceDisplayGlyph(self, 0, self->selected_racer * 16 + 48, 24, 2);
    }

    AppendRaceDisplayNumber(self, 32, 32, 2, self->race_index + 1, 1, 0);
    RaceEntrant * racer = self->racers;
    int priority = 2;
    int suffix = 10;
    int y = 48;
    int rows_left = 3;
    do
    {
        AppendRaceDisplayGlyph(self, 136, y, 11, priority);
        AppendRaceDisplayNumber(self, 144, y, 2, racer->odds, priority, suffix);
        AppendRaceDisplayNumber(self, 192, y, 2, racer->medals_wagered, priority, suffix);
        y += 16;
        --rows_left;
        ++racer;
    } while (rows_left >= 0);
}
