#include "pet.hh"

#include <stdlib.h>

struct RaceHelpTileSize
{
    u16 width, height;
};

EC void FillRaceHelpTiles(RaceHelpTileSize size, void * pixels, u32 color) asm("FillMessageTiles");
EC void DrawRaceHelpText(RaceHelpTileSize size, void * pixels, u32 x, u32 y,
    char const * text, u32 foreground, u32 background) asm("DrawMessageTileStringWithColors");
EC void Dma3_Fill(u32 value, void * destination, u32 bytes);
EC void Dma3_Copy(void const * source, void * destination, u32 bytes);

struct RaceHelpLine
{
    char const * text;
};

EC RaceHelpLine const gHorseRaceHelpLines[] asm("gUnk_080FAFC8");

EC SECTION(".text.f3.0805A3F0") void LoadHorseRaceHelpTextTiles()
{
    enum
    {
        TILE_COLUMNS = 32,
        TILEMAP_ROWS = 22,
        TILEMAP_SIDE = 32,
        LABEL_TILE_ROWS = 2,
        TILE_BYTES = 32,
        HELP_TILE_VRAM = 0x06008000,
        HELP_TILEMAP_VRAM = 0x0600F800,
        PALETTE_BANK_2 = 2 << 12
    };
    struct
    {
        u32 padding;
        u32 pixels[TILE_COLUMNS * LABEL_TILE_ROWS * TILE_BYTES / sizeof(u32)];
        RaceHelpTileSize fill_size, draw_size;
    } work;
    Dma3_Fill(0, reinterpret_cast<void *>(HELP_TILE_VRAM), 0x2C00);
    u16 * tilemap = reinterpret_cast<u16 *>(HELP_TILEMAP_VRAM);
    Dma3_Fill(0, tilemap, TILEMAP_SIDE * TILEMAP_SIDE * sizeof(u16));

    // Register constraints preserve the original stack slots and Thumb instruction order.
    register u32 y asm("r2") = 0;
    asm ("" : : "r" (y));
    register RaceHelpLine const * line asm("r8") = gHorseRaceHelpLines;
    register u32 palette_bits asm("r1") = PALETTE_BANK_2;
    asm ("" : "+r" (palette_bits));
    register u32 tile_base asm("r5") = palette_bits;
    asm ("" : "+r" (tile_base));
    do
    {
        u32 x = 0;
        register u32 row_offset asm("r1") = y * TILE_COLUMNS * sizeof(u16);
        asm ("" : "+r" (row_offset));
        u32 tile = y * TILE_COLUMNS;
        ++y;
        tile += tile_base;
        u16 * row = reinterpret_cast<u16 *>(row_offset + reinterpret_cast<uintptr_t>(tilemap));
        do
        {
            *row = tile;
            ++tile;
            ++row;
        } while (++x < TILE_COLUMNS);
    } while (y < TILEMAP_ROWS);

    register RaceHelpLine const * next_line asm("r1") = line;
    if (next_line->text != nullptr)
    {
        RaceHelpTileSize * fill_size = &work.fill_size;
        register u32 tile_columns asm("r0") = TILE_COLUMNS;
        asm ("" : "+r" (tile_columns));
        register u32 columns asm("r10") = tile_columns;
        asm ("" : "+r" (columns));
        u32 tile_rows = LABEL_TILE_ROWS;
        register u32 rows asm("r9") = tile_rows;
        asm ("" : "+r" (rows));
        u32 * pixels = work.pixels;
        u16 * vram_tiles = reinterpret_cast<u16 *>(HELP_TILE_VRAM);
        register RaceHelpTileSize * draw_size asm("r4") = &work.draw_size;
        asm ("" : : "r" (draw_size));
        do
        {
            u32 width = columns;
            fill_size->width = width;
            register u32 height asm("r1") = rows;
            asm ("" : : "r" (height));
            fill_size->height = height;
            FillRaceHelpTiles(*fill_size, pixels, 0);
            register RaceHelpLine const * cursor asm("r0") = line;
            cursor++;
            line = cursor;
            asm ("" : : "r" (line));
            // Keep the original post-increment load.
            --cursor;
            RaceHelpLine entry = *cursor++;
            asm ("" : : "r" (cursor));
            draw_size->width = columns;
            draw_size->height = rows;
            DrawRaceHelpText(*draw_size, pixels, 0, 0, entry.text, 0, 1);
            Dma3_Copy(pixels, vram_tiles, sizeof(work.pixels));
            u32 stride = sizeof(work.pixels);
            vram_tiles = reinterpret_cast<u16 *>(reinterpret_cast<uintptr_t>(vram_tiles) + stride);
            next_line = line;
            asm ("" : "+r" (next_line));
            if (!next_line->text)
                break;
        } while (true);
    }
}

struct RaceGlyph
{
    i16 x;
    i16 y;
    u16 tile;
    u16 oam_priority;
} PACKED ALIGN(2);

struct RaceEntrant
{
    int base_speed;
    int sprint_speed;
    int acceleration;
    int deceleration;
    int speed;
    int distance_left;
    i16 phase_timer;
    i16 stamina;
    u8 stamina_timer;
    u8 pace_rating;
    u8 sprint_rating;
    u8 finish_rank;
    u16 sprint_profile;
    u16 sprint_phase;
    u8 sprint_chances[4];
    i16 odds;
    i16 medals_wagered;
};

struct HorseRaceState
{
    STRUCT_PAD(0, 0x738);
    RaceGlyph glyphs[148];
    i16 glyph_count;
    STRUCT_PAD(0xBDA, 0x13FE);
    i16 selected_racer;
    STRUCT_PAD(0x1400, 0x140E);
    u8 player_is_racing;
    STRUCT_PAD(0x140F, 0x1414);
    int entry_funds;
    int medal_balance;
    i16 race_index;
    i16 medal_payout;
    STRUCT_PAD(0x1420, 0x1424);
    i16 scene_phase;
    STRUCT_PAD(0x1426, 0x142C);
    RaceEntrant racers[4];
    Pet const * player_horse;
};

EC u8 const gHorseRaceSprintChances[][4] asm("gUnk_080FA99E");

enum { PLAYER_RACER_SLOT = 2 };

template <typename T>
static inline T & PlayerEntrantField(HorseRaceState * self, size_t field_offset)
{
    return *reinterpret_cast<T *>(reinterpret_cast<u8 *>(self) +
        offsetof(HorseRaceState, racers) + PLAYER_RACER_SLOT * sizeof(RaceEntrant) + field_offset);
}

EC SECTION(".text.f3.0805C80C") void InitHorseRaceEntrants(HorseRaceState * self)
{
    RaceEntrant * racer = self->racers;
    if (self->player_is_racing == 1)
    {
        PlayerEntrantField<u16>(self, offsetof(RaceEntrant, sprint_profile)) = 128;
        u32 adequacy = self->player_horse->GetAdequacy();
        u8 & pace_rating = PlayerEntrantField<u8>(self, offsetof(RaceEntrant, pace_rating));
        pace_rating = adequacy;
        u32 affection = self->player_horse->GetAffection();
        PlayerEntrantField<u8>(self, offsetof(RaceEntrant, sprint_rating)) = affection;
        int untrained = 250 - pace_rating;
        int odds = untrained * untrained / 1000;
        int random_bonus = rand() & 3;
        ++random_bonus;
        odds += random_bonus;
        PlayerEntrantField<i16>(self, offsetof(RaceEntrant, odds)) = odds;
    }

    for (int slot = 0; slot < 4; ++slot, ++racer)
    {
        int speed = (racer->pace_rating * 3145 + (88 << 16)) / 60;
        racer->base_speed = speed;
        racer->sprint_speed = (racer->sprint_rating * 3145 + (12 << 16)) / 60 + speed;
        racer->acceleration = (racer->sprint_speed - speed) / 60;
        racer->deceleration = 0;
        racer->phase_timer = 12;
        racer->stamina = 255;
        racer->stamina_timer = 0;
        racer->speed = speed;
        racer->distance_left = 1720 << 16;
        racer->finish_rank = 255;
        racer->sprint_phase = 0;
        racer->medals_wagered = 0;

        if (self->scene_phase != 0)
            continue;
        if (self->player_is_racing == 1 && slot == PLAYER_RACER_SLOT)
        {
            iptr first = reinterpret_cast<iptr>(racer->sprint_chances);
            u8 zero = 0;
            iptr chance = reinterpret_cast<iptr>(&racer->sprint_chances[3]);
            do
            {
                *reinterpret_cast<u8 *>(chance) = zero;
                --chance;
            } while (chance >= first);
        }
        else
        {
            for (int j = 0; j < 4; ++j)
                racer->sprint_chances[j] = gHorseRaceSprintChances[racer->sprint_profile][j];
        }
    }
}

EC SECTION(".text.f3.0805A6E4") void AppendRaceDisplayGlyph(HorseRaceState * self, int x, int y,
    int tile, int oam_priority)
{
    self->glyphs[self->glyph_count].x = x;
    self->glyphs[self->glyph_count].y = y;
    self->glyphs[self->glyph_count].tile = tile;
    self->glyphs[self->glyph_count].oam_priority = oam_priority;
    ++self->glyph_count;
}

EC SECTION(".text.f3.0805A63C") void AppendRaceDisplayNumber(HorseRaceState * self, int x, int y,
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

EC SECTION(".text.f3.0805A4DC") void UpdateRaceDisplayGlyphs(HorseRaceState * self, u32 display_bets)
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
