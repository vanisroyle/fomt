#include "pet.hh"
#include "sprite_animation.hh"

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

struct RaceObjAllocation
{
    u32 context;
    u32 handle;
};

struct HorseRaceState
{
    STRUCT_PAD(0, 0x358);
    SpriteAnimator betting_graphics;
    STRUCT_PAD(0x36C, 0x600);
    RaceObjAllocation betting_tiles, betting_palette;
    RaceObjAllocation betting_glyph_tiles, betting_glyph_palette;
    STRUCT_PAD(0x620, 0x738);
    RaceGlyph glyphs[148];
    i16 glyph_count;
    STRUCT_PAD(0xBDA, 0xBEA);
    u16 tilemap_work[0x800 / sizeof(u16)];
    u16 bg1_scroll_x, bg2_scroll_x, bg3_scroll_x;
    u16 bg1_last_tile_scroll, bg2_last_tile_scroll, bg3_last_tile_scroll;
    i16 bg1_pattern_phase, bg2_pattern_phase, bg3_pattern_phase;
    STRUCT_PAD(0x13FC, 0x13FE);
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

enum
{
    COURSE_REPEAT_COLUMNS = 6,
    COURSE_ROWS = 32,
    TILEMAP_COLUMNS = 32,
    BG_TILE_BYTES = 32,
    BG_SCREEN_BLOCK_BYTES = 0x800,
    VRAM = 0x06000000,
    BG_PALETTE_RAM = 0x05000000,
    COURSE_START_X = 0x600,
    BG3_COURSE_SCREEN_BLOCK = 25,
    BG2_COURSE_SCREEN_BLOCK = 27,
    BG1_COURSE_SCREEN_BLOCK = 29,
    BG_MOSAIC = 1 << 6,
    BG_SIZE_64X32 = 1 << 14,
};

struct CourseDisplayShadow
{
    u16 control;
    STRUCT_PAD(2, 8);
    u16 bg_control[4];
    u16 scroll[4][2];
};

EC CourseDisplayShadow * HardwareContext_GetDisplayRegisters(HorseRaceState *);
EC void Unpack(void const *, void *);

EC u8 const gHorseRaceCourseBg3Pattern[] asm("gUnk_0872F11C");
EC u8 const gHorseRaceCourseBg2Pattern[] asm("gUnk_0872F1BC");
EC u8 const gHorseRaceCourseBg1Pattern[] asm("gUnk_0872F1EC");
EC u8 const gHorseRaceCourseBg2AlternatePattern[] asm("gUnk_0872FBFC");
EC u8 const gHorseRaceCourseBackgroundTiles[] asm("gUnk_0872F21C");
EC u16 const gHorseRaceCourseBackgroundPalette[] asm("gUnk_0872FA9C");

// The register constraints and stack scratch preserve the original Thumb codegen.
#define WRITE_COURSE_DISPLAY(field, value) { \
    CourseDisplayShadow * display = HardwareContext_GetDisplayRegisters(self); \
    volatile u16 * scratch = &work.display_value; \
    register u32 setting asm("r3") = (value); \
    asm ("" : : "r" (setting)); \
    register u32 output asm("r1") = setting; \
    *scratch = output; \
    display->field = output; \
}

#define CLEAR_COURSE_Y(bg) { \
    CourseDisplayShadow * display = HardwareContext_GetDisplayRegisters(self); \
    volatile u16 * scratch = &work.display_value; \
    *scratch = zero; \
    display->scroll[bg][1] = zero; \
}

#define UNPACK_COURSE_PATTERN(asset, offset, reg) { \
    u8 const * compressed = (asset); \
    register uintptr_t destination_offset asm(reg) = offsetof(HorseRaceState, tilemap_work) + offset * sizeof(u16); \
    asm ("" : : "r" (destination_offset)); \
    Unpack(compressed, reinterpret_cast<u8 *>(self) + destination_offset); \
}

EC SECTION(".text.f3.0805AB08") void LoadHorseRaceCourseGraphics(HorseRaceState * course)
{
    HorseRaceState * self = course;
    const uintptr_t bg1_map = VRAM + BG1_COURSE_SCREEN_BLOCK * BG_SCREEN_BLOCK_BYTES;
    const uintptr_t bg2_map = VRAM + BG2_COURSE_SCREEN_BLOCK * BG_SCREEN_BLOCK_BYTES;
    const uintptr_t bg3_map = VRAM + BG3_COURSE_SCREEN_BLOCK * BG_SCREEN_BLOCK_BYTES;
    const unsigned pattern_bytes = COURSE_REPEAT_COLUMNS * sizeof(u16);
    const unsigned map_bytes = 2 * BG_SCREEN_BLOCK_BYTES;
    const unsigned bg2_alternate_column = 18;
    const unsigned bg2_pattern_offset = 0x100 / sizeof(u16);
    const unsigned bg1_pattern_offset = 0x200 / sizeof(u16);
    const unsigned bg2_alternate_offset = 0x300 / sizeof(u16);
    struct
    {
        u16 display_value;
        STRUCT_PAD(2, 4);
        uintptr_t bg3_source;
        uintptr_t bg2_alternate_source;
        uintptr_t bg1_source;
        int next_row;
    } volatile work;

    // Blank tile 1023 fills the unused parts of the course maps.
    Dma3_Fill(0, reinterpret_cast<void *>(VRAM + 1023 * BG_TILE_BYTES), BG_TILE_BYTES);
    Dma3_Fill(~0u, reinterpret_cast<void *>(bg3_map), map_bytes);
    Dma3_Fill(~0u, reinterpret_cast<void *>(bg2_map), map_bytes);
    Dma3_Fill(~0u, reinterpret_cast<void *>(bg1_map), map_bytes);
    UNPACK_COURSE_PATTERN(gHorseRaceCourseBg3Pattern, 0, "r2");
    UNPACK_COURSE_PATTERN(gHorseRaceCourseBg2Pattern, bg2_pattern_offset, "r3");
    UNPACK_COURSE_PATTERN(gHorseRaceCourseBg1Pattern, bg1_pattern_offset, "r2");
    UNPACK_COURSE_PATTERN(gHorseRaceCourseBg2AlternatePattern, bg2_alternate_offset, "r3");
    Unpack(gHorseRaceCourseBackgroundTiles, reinterpret_cast<void *>(VRAM));
    Dma3_Copy(gHorseRaceCourseBackgroundPalette, reinterpret_cast<void *>(BG_PALETTE_RAM), 256 * sizeof(u16));
    WRITE_COURSE_DISPLAY(control, (1 << 6) | (1 << 9) | (1 << 10) | (1 << 11) | (1 << 12));
    WRITE_COURSE_DISPLAY(bg_control[3], BG_SIZE_64X32 | BG_MOSAIC | (BG3_COURSE_SCREEN_BLOCK << 8) | 3);
    WRITE_COURSE_DISPLAY(bg_control[2], BG_SIZE_64X32 | BG_MOSAIC | (BG2_COURSE_SCREEN_BLOCK << 8) | 2);
    WRITE_COURSE_DISPLAY(bg_control[1], BG_SIZE_64X32 | BG_MOSAIC | (BG1_COURSE_SCREEN_BLOCK << 8) | 1);

    // Repeat six-tile patterns across the first screen block of each layer.
    int row = 0;
    do
    {
        int column = 0;
        unsigned pattern_row = row * 2;
        unsigned screen_row = row * TILEMAP_COLUMNS * sizeof(u16);
        register int next_row asm("r3") = row + 1;
        work.next_row = next_row;
        pattern_row += row;
        pattern_row *= 4;
        register uintptr_t bg2_alternate_source asm("r1") =
            offsetof(HorseRaceState, tilemap_work) + bg2_alternate_offset * sizeof(u16);
        asm ("" : "+r" (bg2_alternate_source));
        bg2_alternate_source = pattern_row + bg2_alternate_source;
        work.bg2_alternate_source = bg2_alternate_source;
        register uintptr_t bg1_pattern asm("r2") = offsetof(HorseRaceState, tilemap_work) + bg1_pattern_offset * sizeof(u16);
        asm ("" : "+r" (bg1_pattern));
        bg1_pattern = pattern_row + bg1_pattern;
        work.bg1_source = bg1_pattern;
        register uintptr_t bg1_entry asm("r6") = bg1_map + screen_row;
        uintptr_t bg2_entry = bg2_map + screen_row;
        uintptr_t bg3_entry = bg3_map + screen_row;
        register uintptr_t bg3_pattern asm("r3") = offsetof(HorseRaceState, tilemap_work);
        asm ("" : "+r" (bg3_pattern));
        bg3_pattern = pattern_row + bg3_pattern;
        work.bg3_source = bg3_pattern;
        uintptr_t bg2_pattern = offsetof(HorseRaceState, tilemap_work) + bg2_pattern_offset * sizeof(u16);
        bg2_pattern += pattern_row;
        uintptr_t bg2_source = bg2_pattern;
        do
        {
            register uintptr_t bg3_source asm("r2") = work.bg3_source;
            Dma3_Copy(reinterpret_cast<u8 *>(self) + bg3_source,
                reinterpret_cast<void *>(bg3_entry), pattern_bytes);
            register int copy_column asm("r3") = column;
            asm ("" : "+r" (copy_column));
            if (copy_column != bg2_alternate_column)
            {
                register uintptr_t default_source asm("r1") = bg2_source;
                asm ("" : : "r" (default_source), "r" (self));
                Dma3_Copy(reinterpret_cast<u8 *>(self) + default_source,
                    reinterpret_cast<void *>(bg2_entry), pattern_bytes);
            }
            else
            {
                register uintptr_t source asm("r2") = work.bg2_alternate_source;
                Dma3_Copy(reinterpret_cast<u8 *>(self) + source,
                    reinterpret_cast<void *>(bg2_map + bg2_alternate_column * sizeof(u16) + screen_row), pattern_bytes);
            }
            register uintptr_t bg1_source asm("r3") = work.bg1_source;
            Dma3_Copy(reinterpret_cast<u8 *>(self) + bg1_source,
                reinterpret_cast<void *>(bg1_entry), pattern_bytes);
            bg1_entry += pattern_bytes;
            bg2_entry += pattern_bytes;
            bg3_entry += pattern_bytes;
            register int column_step asm("r0") = COURSE_REPEAT_COLUMNS;
            asm ("" : : "r" (column_step));
            column += column_step;
            register int next_column asm("r1") = column;
            asm ("" : "+r" (next_column));
            if (next_column >= TILEMAP_COLUMNS - 2)
                break;
        } while (true);
        row = work.next_row;
    } while (row < COURSE_ROWS);

    u16 zero = 0;
    uintptr_t bg1_scroll = offsetof(HorseRaceState, bg1_scroll_x);
    u16 * scroll_destination =
        reinterpret_cast<u16 *>(reinterpret_cast<u8 *>(self) + bg1_scroll);
    u32 starting_x = COURSE_START_X >> 3;
    starting_x <<= 3;
    *scroll_destination = starting_x;
    register uintptr_t bg2_scroll asm("r3") = offsetof(HorseRaceState, bg2_scroll_x);
    *reinterpret_cast<u16 *>(reinterpret_cast<u8 *>(self) + bg2_scroll) = starting_x;
    bg1_scroll += offsetof(HorseRaceState, bg3_scroll_x) - offsetof(HorseRaceState, bg1_scroll_x);
    *reinterpret_cast<u16 *>(reinterpret_cast<u8 *>(self) + bg1_scroll) = starting_x;
    bg2_scroll += offsetof(HorseRaceState, bg1_last_tile_scroll) - offsetof(HorseRaceState, bg2_scroll_x);
    *reinterpret_cast<u16 *>(reinterpret_cast<u8 *>(self) + bg2_scroll) = zero;
    register uintptr_t bg2_last asm("r1") = offsetof(HorseRaceState, bg2_last_tile_scroll);
    *reinterpret_cast<u16 *>(reinterpret_cast<u8 *>(self) + bg2_last) = zero;
    bg1_scroll += offsetof(HorseRaceState, bg3_last_tile_scroll) - offsetof(HorseRaceState, bg3_scroll_x);
    *reinterpret_cast<u16 *>(reinterpret_cast<u8 *>(self) + bg1_scroll) = zero;
    bg2_scroll += offsetof(HorseRaceState, bg1_pattern_phase) - offsetof(HorseRaceState, bg1_last_tile_scroll);
    scroll_destination = reinterpret_cast<u16 *>(reinterpret_cast<u8 *>(self) + bg2_scroll);
    *scroll_destination = zero;
    bg2_last += offsetof(HorseRaceState, bg2_pattern_phase) - offsetof(HorseRaceState, bg2_last_tile_scroll);
    scroll_destination = reinterpret_cast<u16 *>(reinterpret_cast<u8 *>(self) + bg2_last);
    *scroll_destination = zero;
    bg1_scroll += offsetof(HorseRaceState, bg3_pattern_phase) - offsetof(HorseRaceState, bg3_last_tile_scroll);
    scroll_destination = reinterpret_cast<u16 *>(reinterpret_cast<u8 *>(self) + bg1_scroll);
    *scroll_destination = zero;
    CLEAR_COURSE_Y(1);
    CLEAR_COURSE_Y(2);
    CLEAR_COURSE_Y(3);
}

#undef UNPACK_COURSE_PATTERN
#undef CLEAR_COURSE_Y
#undef WRITE_COURSE_DISPLAY

struct RaceFrameSource;

template <typename T>
struct RaceFrameSlice
{
    T const * data;
    u16 length;
};

struct RaceFrameView
{
    RaceFrameSlice<SpriteFrameObject> objects;
    RaceFrameSlice<u8> tiles;
    RaceFrameSlice<u16> palette;
    RaceFrameSlice<SpriteFrameExtra> extras;
};

struct RaceFrameSourceVTable
{
    STRUCT_PAD(0, 0x10);
    void (*get_frame)(RaceFrameView *, RaceFrameSource const *, u32);
};

struct RaceFrameSource
{
    RaceFrameSourceVTable const * vtable;
};

EC u32 ResolveRaceObjPalette(RaceObjAllocation *, u32) asm("func_080074C0");
EC u32 ResolveRaceObjTile(RaceObjAllocation *, u32) asm("func_08007D4C");
EC void * CopyRaceFrame(void *, void const *, u32) asm("memcpy");
EC void DrawMessageFrame(u16 *, u32, u32, u32, u32, u32, u32);

#define CLEAR_RACE_FRAME_SLICE(slice, reg) do { \
    register __typeof__(&work.frame.slice) frame_slice asm(reg) = &work.frame.slice; \
    asm ("" : "+l" (frame_slice)); \
    work.frame.slice.data = reinterpret_cast<__typeof__(work.frame.slice.data)>(zero); \
    asm ("strh %1, [%2, #4]" : "=m" (frame_slice->length) : "l" (zero), "l" (frame_slice)); \
} while (0)

#define RACE_FIELD_OFFSET(reg, type, field) ({ \
    register u32 field_offset asm(reg) = offsetof(type, field); \
    asm ("" : "+l" (field_offset)); \
    field_offset; \
})

#define PREPARE_RACE_FRAME() \
    register RaceFrameSource const * source asm("r1") = reinterpret_cast<RaceFrameSource const *>(animator->source); \
    register RaceFrameSourceVTable const * vtable asm("r3") = source->vtable; \
    register u32 keyframe asm("r2") = animator->keyframe; \
    asm ("" : "+l" (source), "+l" (vtable), "+l" (keyframe));

#define READ_RACE_FRAME(result_pointer) do { \
    register RaceFrameView * frame_output asm("r0") = result_pointer; \
    asm ("" : "+l" (frame_output)); \
    vtable->get_frame(frame_output, source, frame_id); \
} while (0)

// A fresh stack address is used by the first and the looped frame reads.
#define READ_RACE_STACK_FRAME() do { \
    register RaceFrameView * stack_result asm("r0"); \
    asm ("add %0, sp, %1" : "=l" (stack_result) \
        : "i" (PARAM_BYTES + offsetof(__typeof__(work), result))); \
    vtable->get_frame(stack_result, source, frame_id); \
} while (0)

#define COPY_RACE_PALETTE_FRAME_CLOBBERS "r5", "r6"
#define COPY_RACE_FRAME(result_pointer, regs1, regs2, regs3, extra) do { \
    CopyRaceFrame(result_pointer, result_pointer, sizeof(work.result)); \
    register RaceFrameView * destination asm("r1") = &work.frame; \
    asm ("" : "+l" (destination)); \
    register RaceFrameView * temporary asm("r0") = result_pointer; \
    asm volatile ("ldmia %1!, {" regs1 "}\n\tstmia %0!, {" regs1 "}\n\t" \
        "ldmia %1!, {" regs2 "}\n\tstmia %0!, {" regs2 "}\n\t" \
        "ldmia %1!, {" regs3 "}\n\tstmia %0!, {" regs3 "}" \
        : "+l" (destination), "+l" (temporary) : : "r2", "r3", extra, "memory"); \
} while (0)

// Keep PC-relative loads pointed at the original trailing literal pool.
#define RACE_GRAPHICS_LITERAL(reg, name) ({ \
    register u32 literal_value asm(reg); \
    asm ("ldr %0, .Lrace_graphics_" #name : "=l" (literal_value)); \
    literal_value; \
})

#define SET_RACE_BETTING_REGISTER(field, value, scratch_def) do { \
    register CourseDisplayShadow * display asm("r0") = HardwareContext_GetDisplayRegisters(self); \
    register u32 setting asm("r1") = value; \
    asm ("" : "+l" (setting)); \
    scratch_def; \
    asm volatile ("strh %0, [%1]" : : "l" (setting), "l" (scratch) : "memory"); \
    display->field = setting; \
} while (0)

#define RACE_DISPLAY_SCRATCH(reg) \
    register u16 * scratch asm(reg) = display_value; \
    asm ("" : "+l" (scratch))

#define CLEAR_RACE_BETTING_SCROLL(field) do { \
    register CourseDisplayShadow * display asm("r0") = HardwareContext_GetDisplayRegisters(self); \
    asm volatile ("strh %0, [%1]\n\tstrh %0, [%2, %3]" : : "l" (scroll_zero), "l" (scratch), \
        "l" (display), "i" (offsetof(CourseDisplayShadow, field)) : "memory"); \
} while (0)

EC SECTION(".text.f3.08059C94") void LoadHorseRaceBettingScreenGraphics(HorseRaceState * self)
{
    enum
    {
        PARAM_BYTES = 3 * sizeof(u32),
        TILE_BYTES = 32,
        GLYPH_TILE_WIDTH = 1,
        GLYPH_TILE_HEIGHT = 2,
        GLYPH_COUNT = 24,
        BG_MAP_BYTES = 0x800,
        BG_PALETTE_RAM = 0x05000000,
        BG_VRAM = 0x06000000,
        DISPLAY_ENABLE = 0x1E40,
        FRAME_COLUMNS = 32,
        GLYPH_TRANSPARENT = 0x03E0
    };
    struct
    {
        RaceFrameView frame, result;
        STRUCT_PAD(0x40, 0x44);
        u32 glyph_pixels[GLYPH_TILE_WIDTH * GLYPH_TILE_HEIGHT * TILE_BYTES / sizeof(u32)];
        RaceHelpTileSize glyph_size;
        RaceHelpTileSize * glyph_dimensions;
        SpriteAnimationClip const * clip_copy;
        u32 * glyph_pixels_copy;
    } work;
    // The agbcc frame return uses a stack buffer, then copies into the frame view.
    register RaceFrameSlice<SpriteFrameObject> * objects asm("r1") = &work.frame.objects;
    asm ("" : "+l" (objects));
    register u32 zero asm("r0") = 0;
    asm ("" : "+l" (zero));
    work.frame.objects.data = reinterpret_cast<SpriteFrameObject const *>(zero);
    asm ("strh %1, [%2, #4]" : "=m" (objects->length) : "l" (zero), "l" (objects));
    CLEAR_RACE_FRAME_SLICE(tiles, "r1");
    register RaceFrameSlice<u16> * palette_slice asm("r2") = &work.frame.palette;
    asm ("" : "+l" (palette_slice));
    register RaceFrameSlice<u16> * palette_stack asm("r9") = palette_slice;
    asm ("" : "+r" (palette_stack));
    work.frame.palette.data = reinterpret_cast<u16 const *>(zero);
    asm ("strh %1, [%2, #4]" : "=m" (palette_slice->length) : "l" (zero), "l" (palette_slice));
    CLEAR_RACE_FRAME_SLICE(extras, "r1");
    register SpriteAnimator * animator asm("r5") = reinterpret_cast<SpriteAnimator *>(reinterpret_cast<u8 *>(self) +
        RACE_FIELD_OFFSET("r3", HorseRaceState, betting_graphics));
    asm ("" : "+l" (animator));
    register SpriteAnimationClip const * clip_high asm("r8");
    {
        PREPARE_RACE_FRAME()
        register uintptr_t clip_low asm("r4") = offsetof(HorseRaceState, betting_graphics) + offsetof(SpriteAnimator, clip);
        asm ("" : "+l" (clip_low));
        clip_low += reinterpret_cast<uintptr_t>(self);
        asm ("" : "+l" (clip_low));
        clip_high = reinterpret_cast<SpriteAnimationClip const *>(clip_low);
        asm ("" : "+r" (clip_high));
        register u32 offset asm("r2") = keyframe * sizeof(SpriteKeyframe);
        asm ("" : "+l" (offset));
        register uintptr_t frames asm("r0") = reinterpret_cast<uintptr_t>(reinterpret_cast<SpriteAnimationClip const *>(clip_low)->keyframes);
        asm ("" : "+l" (frames));
        frames += offset;
        asm ("" : "+l" (frames));
        register u32 frame_id asm("r2") = reinterpret_cast<SpriteKeyframe const *>(frames)->frame;
        asm ("" : "+l" (frame_id));
        READ_RACE_STACK_FRAME();
    }
    register RaceFrameView * result asm("r4");
    asm ("add %0, sp, %1" : "=l" (result) : "i" (PARAM_BYTES + offsetof(__typeof__(work), result)));
    COPY_RACE_FRAME(result, "r2, r3, r6", "r2, r3, r6", "r2, r6", "r6");
    {
        PREPARE_RACE_FRAME()
        register u32 offset asm("r2") = keyframe * sizeof(SpriteKeyframe);
        asm ("" : "+l" (offset));
        register SpriteAnimationClip const * clip asm("r5") = clip_high;
        asm ("" : "+l" (clip));
        register uintptr_t frames asm("r0") = reinterpret_cast<uintptr_t>(clip->keyframes);
        asm ("" : "+l" (frames));
        frames += offset;
        asm ("" : "+l" (frames));
        register u32 frame_id asm("r2") = reinterpret_cast<SpriteKeyframe const *>(frames)->frame;
        asm ("" : "+l" (frame_id));
        READ_RACE_FRAME(result);
    }
    asm ("" : "+l" (result));
    COPY_RACE_FRAME(result, "r2, r3, r6", "r2, r5, r6", "r3, r5", COPY_RACE_PALETTE_FRAME_CLOBBERS);
    register u16 const * palette asm("r5") = work.frame.palette.data;
    asm ("" : "+l" (palette));
    register RaceObjAllocation * betting_palette asm("r0") = reinterpret_cast<RaceObjAllocation *>(reinterpret_cast<u8 *>(self) +
        RACE_FIELD_OFFSET("r6", HorseRaceState, betting_palette));
    asm ("" : "+l" (betting_palette));
    register unsigned palette_offset asm("r0") = ResolveRaceObjPalette(betting_palette, betting_palette->handle) * TILE_BYTES;
    asm ("" : "+l" (palette_offset));
    register uintptr_t palette_vram asm("r2") = RACE_GRAPHICS_LITERAL("r2", OBJ_PALETTE_RAM);
    asm ("" : "+l" (palette_vram));
    register void * palette_dest asm("r1") = reinterpret_cast<void *>(palette_offset + palette_vram);
    asm ("" : "+l" (palette_dest));
    register u16 const * palette_data asm("r0") = work.frame.palette.data;
    register u32 palette_length asm("r2") = 0;
    asm ("" : "+l" (palette_data), "+l" (palette_length));
    register RaceFrameView * result_high asm("r10") = result;
    asm ("" : "+r" (result_high));
    if (palette_data)
    {
        register RaceFrameSlice<u16> * palette_info asm("r3") = palette_stack;
        asm ("" : "+l" (palette_info));
        palette_length = palette_info->length;
    }
    Dma3_Copy(palette, palette_dest, palette_length);

    register u32 tile_offset_low asm("r4") = 0;
    asm ("" : "+l" (tile_offset_low));
    register u32 tile_offset asm("r9") = tile_offset_low;
    asm ("" : "+r" (tile_offset));
    register int clip_id asm("r6") = 0;
    asm ("" : "+l" (clip_id));
    // Keep the frame and glyph scratch pointers in the original stack slots.
    register SpriteAnimationClip const * clip_stack asm("r5") = clip_high;
    asm volatile ("str %0, [sp, %1]" : : "l" (clip_stack),
        "i" (PARAM_BYTES + offsetof(__typeof__(work), clip_copy)) : "memory");
    asm volatile ("mov r0, sp\n\tadd r0, %0\n\tstr r0, [sp, %1]\n\t"
        "mov r1, sp\n\tadd r1, %2\n\tstr r1, [sp, %3]"
        : : "i" (PARAM_BYTES + offsetof(__typeof__(work), glyph_size)),
        "i" (PARAM_BYTES + offsetof(__typeof__(work), glyph_dimensions)),
        "i" (PARAM_BYTES + offsetof(__typeof__(work), glyph_pixels)),
        "i" (PARAM_BYTES + offsetof(__typeof__(work), glyph_pixels_copy)) : "r0", "r1", "memory");
    animator = reinterpret_cast<SpriteAnimator *>(reinterpret_cast<u8 *>(self) +
        RACE_FIELD_OFFSET("r2", HorseRaceState, betting_graphics));
    asm ("" : "+l" (animator));
    register RaceFrameView * frame_result asm("r8") = result_high;
    asm ("" : "+r" (frame_result));
    do
    {
        SetSpriteAnimatorClip(animator, clip_id);
        PREPARE_RACE_FRAME()
        register u32 offset asm("r2") = keyframe * sizeof(SpriteKeyframe);
        asm ("" : "+l" (offset));
        register SpriteAnimationClip const * clip asm("r4");
        asm ("ldr %0, [sp, %1]" : "=l" (clip) : "i" (PARAM_BYTES + offsetof(__typeof__(work), clip_copy)) : "memory");
        register uintptr_t frames asm("r0") = reinterpret_cast<uintptr_t>(clip->keyframes);
        asm ("" : "+l" (frames));
        frames += offset;
        asm ("" : "+l" (frames));
        register u32 frame_id asm("r2") = reinterpret_cast<SpriteKeyframe const *>(frames)->frame;
        asm ("" : "+l" (frame_id));
        READ_RACE_STACK_FRAME();
        COPY_RACE_FRAME(frame_result, "r2, r3, r4", "r2, r3, r4", "r2, r3", "r4");
        register u8 const * tiles asm("r4") = work.frame.tiles.data;
        asm ("" : "+l" (tiles));
        register RaceObjAllocation * allocation asm("r0") =
            reinterpret_cast<RaceObjAllocation *>(reinterpret_cast<u8 *>(self) +
                RACE_FIELD_OFFSET("r1", HorseRaceState, betting_tiles));
        asm ("" : "+l" (allocation));
        register u32 offset_bytes asm("r0") =
            (ResolveRaceObjTile(allocation, allocation->handle) + tile_offset) * TILE_BYTES;
        asm ("" : "+l" (offset_bytes));
        register u32 obj_vram asm("r2") = RACE_GRAPHICS_LITERAL("r2", OBJ_VRAM);
        register void * tile_dest asm("r1") = reinterpret_cast<void *>(offset_bytes + obj_vram);
        asm ("" : "+l" (tile_dest));
        register u8 const * tile_data asm("r0") = work.frame.tiles.data;
        register u32 tile_bytes asm("r2") = 0;
        asm ("" : "+l" (tile_data), "+l" (tile_bytes));
        if (tile_data)
        {
            asm ("mov r3, sp\n\tldrh %0, [r3, %1]" : "=l" (tile_bytes)
                : "i" (PARAM_BYTES + offsetof(__typeof__(work), frame.tiles.length)) : "r3", "memory");
        }
        Dma3_Copy(tiles, tile_dest, tile_bytes);
        tile_data = work.frame.tiles.data;
        register u32 copied_bytes asm("r1") = 0;
        asm ("" : "+l" (tile_data), "+l" (copied_bytes));
        if (tile_data)
        {
            asm ("mov r4, sp\n\tldrh %0, [r4, %1]" : "=l" (copied_bytes)
                : "i" (PARAM_BYTES + offsetof(__typeof__(work), frame.tiles.length)) : "r4", "memory");
        }
        register u32 copied_tiles asm("r0") = copied_bytes / TILE_BYTES;
        asm ("" : "+l" (copied_tiles));
        asm ("add %0, %1" : "+r" (tile_offset) : "l" (copied_tiles));
    } while (++clip_id < 4);

    // Set up the blank background, frame, and betting table maps.
    Dma3_Fill(0, reinterpret_cast<void *>(RACE_GRAPHICS_LITERAL("r1", BLANK_TILE)), TILE_BYTES);
    register u32 empty_map asm("r4") = ~0u;
    asm ("" : "+l" (empty_map));
    register u32 base_map_low asm("r5") = RACE_GRAPHICS_LITERAL("r5", BASE_MAP);
    register void * base_map asm("r8") = reinterpret_cast<void *>(base_map_low);
    asm ("" : "+r" (base_map));
    register u32 map_bytes asm("r6") = BG_MAP_BYTES;
    asm ("" : "+l" (map_bytes));
    Dma3_Fill(empty_map, base_map, map_bytes);
    register u32 frame_map_low asm("r0") = RACE_GRAPHICS_LITERAL("r0", FRAME_MAP);
    register u16 * frame_map asm("r9") = reinterpret_cast<u16 *>(frame_map_low);
    asm ("" : "+r" (frame_map));
    Dma3_Fill(empty_map, frame_map, map_bytes);
    Dma3_Fill(empty_map, reinterpret_cast<void *>(RACE_GRAPHICS_LITERAL("r1", BETTING_MAP)), map_bytes);
    register u16 * display_value asm("r10") = reinterpret_cast<u16 *>(result_high);
    register u32 scroll_zero asm("r5");
    {
        CourseDisplayShadow * display = HardwareContext_GetDisplayRegisters(self);
        register u32 enable asm("r2") = DISPLAY_ENABLE;
        asm ("" : "+l" (enable));
        register u32 setting asm("r1") = enable;
        asm ("" : "+l" (setting));
        RACE_DISPLAY_SCRATCH("r3");
        asm volatile ("strh %0, [%1]" : : "l" (setting), "l" (scratch) : "memory");
        scroll_zero = 0;
        asm ("" : "+l" (scroll_zero));
        display->control = setting;
    }
    SET_RACE_BETTING_REGISTER(bg_control[3], RACE_GRAPHICS_LITERAL("r4", BASE_BG_CONTROL),
        RACE_DISPLAY_SCRATCH("r2"));
    register u16 * scratch asm("r4");
    SET_RACE_BETTING_REGISTER(bg_control[2], RACE_GRAPHICS_LITERAL("r3", FRAME_BG_CONTROL),
        scratch = display_value; asm ("" : "+l" (scratch)));
    SET_RACE_BETTING_REGISTER(bg_control[1], RACE_GRAPHICS_LITERAL("r2", BETTING_BG_CONTROL), (void)0);
    register u8 const * tiles = *reinterpret_cast<u8 const * const *>(RACE_GRAPHICS_LITERAL("r0", FRAME_TILES));
    asm ("" : "+l" (tiles));
    register void * bg_vram asm("r1") = reinterpret_cast<void *>(BG_VRAM);
    asm ("" : "+l" (bg_vram));
    Dma3_Copy(tiles, bg_vram, *reinterpret_cast<u32 const *>(RACE_GRAPHICS_LITERAL("r2", FRAME_TILE_BYTES)));
    Dma3_Fill(RACE_GRAPHICS_LITERAL("r0", BLANK_MAP_WORD), base_map, map_bytes);
    CLEAR_RACE_BETTING_SCROLL(scroll[3][0]);
    CLEAR_RACE_BETTING_SCROLL(scroll[3][1]);
    DrawMessageFrame(frame_map, 0, 0, FRAME_COLUMNS, 3, scroll_zero, scroll_zero);
    CLEAR_RACE_BETTING_SCROLL(scroll[2][0]);
    CLEAR_RACE_BETTING_SCROLL(scroll[2][1]);
    CLEAR_RACE_BETTING_SCROLL(scroll[1][0]);
    CLEAR_RACE_BETTING_SCROLL(scroll[1][1]);
    register u16 const * frame_palette asm("r0") =
        *reinterpret_cast<u16 const * const *>(RACE_GRAPHICS_LITERAL("r0", FRAME_PALETTE));
    asm ("" : "+l" (frame_palette));
    Dma3_Copy(frame_palette, reinterpret_cast<void *>(BG_PALETTE_RAM), TILE_BYTES);

    register volatile u16 * glyph_palette asm("r0") =
        reinterpret_cast<volatile u16 *>(RACE_GRAPHICS_LITERAL("r0", GLYPH_BG_PALETTE_RAM));
    register u32 transparent_low asm("r3") = GLYPH_TRANSPARENT;
    asm ("" : "+l" (transparent_low));
    register u32 transparent asm("r6") = transparent_low;
    asm ("" : "+l" (transparent));
    asm volatile ("strh %1, [%0]\n\tadd %0, #2\n\tstrh %2, [%0]\n\tadd %0, #2"
        : "+l" (glyph_palette) : "l" (transparent), "l" (scroll_zero) : "cc", "memory");
    register u32 shadow_low asm("r4") = RACE_GRAPHICS_LITERAL("r4", GLYPH_SHADOW);
    register u32 shadow asm("r8") = shadow_low;
    asm ("" : "+r" (shadow));
    *glyph_palette = shadow_low;
    register RaceObjAllocation * palette_allocation asm("r4") =
        reinterpret_cast<RaceObjAllocation *>(reinterpret_cast<u8 *>(self) +
            RACE_FIELD_OFFSET("r0", HorseRaceState, betting_glyph_palette));
    asm ("" : "+l" (palette_allocation));

#define SET_RACE_GLYPH_COLOR(reg, address, color, color_reg) do { \
    register u32 offset asm("r0") = ResolveRaceObjPalette(palette_allocation, palette_allocation->handle) * TILE_BYTES; \
    asm ("" : "+l" (offset)); \
    register volatile u16 * entry asm("r0") = reinterpret_cast<volatile u16 *>(offset + RACE_GRAPHICS_LITERAL(reg, address)); \
    asm ("" : "+l" (entry)); \
    register u32 color_value asm(color_reg) = color; \
    asm volatile ("strh %0, [%1]" : : "l" (color_value), "l" (entry) : "memory"); \
} while (0)
    SET_RACE_GLYPH_COLOR("r1", OBJ_PALETTE_RAM, transparent, "r6");
    SET_RACE_GLYPH_COLOR("r2", OBJ_PALETTE_COLOR_1, scroll_zero, "r5");
    SET_RACE_GLYPH_COLOR("r3", OBJ_PALETTE_COLOR_2, shadow, "r4");
#undef SET_RACE_GLYPH_COLOR

    // Each betting glyph occupies two consecutive OBJ tiles.
    register u32 background asm("r10") = scroll_zero;
    asm ("" : "+r" (background));
    register u32 width_low asm("r6") = GLYPH_TILE_WIDTH;
    asm ("" : "+l" (width_low));
    register u32 width asm("r8") = width_low;
    asm ("" : "+r" (width));
    register u32 height_low asm("r0") = GLYPH_TILE_HEIGHT;
    asm ("" : "+l" (height_low));
    register u32 height asm("r9") = height_low;
    asm ("" : "+r" (height));
    register RaceHelpTileSize * glyph_size asm("r6");
    asm ("add %0, sp, %1" : "=l" (glyph_size) : "i" (PARAM_BYTES + offsetof(__typeof__(work), glyph_size)));
    register int glyph asm("r5") = scroll_zero;
    do
    {
        register u32 code_table asm("r0") = RACE_GRAPHICS_LITERAL("r0", GLYPH_CODES);
        register u32 code_offset asm("r4") = glyph * sizeof(u16);
        asm ("" : "+l" (code_offset));
        register u32 code asm("r1") = *reinterpret_cast<u16 const *>(code_offset + code_table);
        asm ("" : "+l" (code));
        register u32 width_value asm("r2") = width;
        asm volatile ("strh %0, [%1]" : : "l" (width_value), "l" (glyph_size) : "memory");
        register u32 height_value asm("r0") = height;
        asm ("" : "+l" (height_value));
        register RaceHelpTileSize * dimensions asm("r3");
        asm ("ldr %0, [sp, %1]" : "=l" (dimensions) : "i" (PARAM_BYTES + offsetof(__typeof__(work), glyph_dimensions)) : "memory");
        dimensions->height = height_value;
        asm volatile (
            "ldr r0, [%4]\n\tstr %0, [sp]\n\tmov r1, %1\n\tstr r1, [sp, #4]\n\t"
            "mov r2, %2\n\tstr r2, [sp, #8]\n\tadd r1, sp, %3\n\tmov r2, #0\n\t"
            "mov r3, #0\n\tbl DrawMessageGlyphWithColors"
            : "+l" (code) : "r" (background), "r" (width),
            "i" (PARAM_BYTES + offsetof(__typeof__(work), glyph_pixels)), "l" (glyph_size)
            : "r0", "r2", "r3", "r12", "lr", "cc", "memory");
        register RaceObjAllocation * allocation asm("r0") =
            reinterpret_cast<RaceObjAllocation *>(reinterpret_cast<u8 *>(self) +
                RACE_FIELD_OFFSET("r3", HorseRaceState, betting_glyph_tiles));
        asm ("" : "+l" (allocation));
        register u32 tile asm("r1") = ResolveRaceObjTile(allocation, allocation->handle);
        asm ("" : "+l" (tile));
        tile = (tile + code_offset) * TILE_BYTES;
        asm ("" : "+l" (tile));
        register void * tile_dest asm("r1") =
            reinterpret_cast<void *>(tile + RACE_GRAPHICS_LITERAL("r4", OBJ_VRAM));
        asm ("" : "+l" (tile_dest));
        register u32 * pixels asm("r0");
        asm ("ldr %0, [sp, %1]" : "=l" (pixels) : "i" (PARAM_BYTES + offsetof(__typeof__(work), glyph_pixels_copy)) : "memory");
        Dma3_Copy(pixels, tile_dest, sizeof(work.glyph_pixels));
    } while (++glyph < GLYPH_COUNT);
}

asm (
    ".section .text.f3.08059C94,\"ax\",%progbits\n\t.align 2, 0\n"
    ".Lrace_graphics_OBJ_PALETTE_RAM: .word 0x05000200\n"
    ".Lrace_graphics_OBJ_VRAM: .word 0x06010000\n"
    ".Lrace_graphics_BLANK_TILE: .word 0x06007FE0\n"
    ".Lrace_graphics_BASE_MAP: .word 0x0600E000\n"
    ".Lrace_graphics_FRAME_MAP: .word 0x0600E800\n"
    ".Lrace_graphics_BETTING_MAP: .word 0x0600F000\n"
    ".Lrace_graphics_BASE_BG_CONTROL: .word 0x00005C43\n"
    ".Lrace_graphics_FRAME_BG_CONTROL: .word 0x00001D41\n"
    ".Lrace_graphics_BETTING_BG_CONTROL: .word 0x00005E42\n"
    ".Lrace_graphics_FRAME_TILES: .word gUnk_080F9F70\n"
    ".Lrace_graphics_FRAME_TILE_BYTES: .word gUnk_080F9F74\n"
    ".Lrace_graphics_BLANK_MAP_WORD: .word 0x00030003\n"
    ".Lrace_graphics_FRAME_PALETTE: .word gUnk_080F9F78\n"
    ".Lrace_graphics_GLYPH_BG_PALETTE_RAM: .word 0x05000040\n"
    ".Lrace_graphics_GLYPH_SHADOW: .word 0x00004218\n"
    ".Lrace_graphics_OBJ_PALETTE_COLOR_1: .word 0x05000202\n"
    ".Lrace_graphics_OBJ_PALETTE_COLOR_2: .word 0x05000204\n"
    ".Lrace_graphics_GLYPH_CODES: .word gHorseRaceBettingGlyphCodes\n"
    ".previous");

#undef CLEAR_RACE_BETTING_SCROLL
#undef RACE_DISPLAY_SCRATCH
#undef RACE_GRAPHICS_LITERAL
#undef SET_RACE_BETTING_REGISTER
#undef COPY_RACE_FRAME
#undef COPY_RACE_PALETTE_FRAME_CLOBBERS
#undef READ_RACE_FRAME
#undef READ_RACE_STACK_FRAME
#undef PREPARE_RACE_FRAME
#undef RACE_FIELD_OFFSET
#undef CLEAR_RACE_FRAME_SLICE
