#include "pet.hh"
#include "sprite_animation.hh"
#include "sprite_rendering.hh"

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
    u16 entrant_id;
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

struct HardwareContext;
struct MusicPlayer;
struct AudioPlayer
{
    MusicPlayer * player;
};

struct RaceCourseMarker
{
    i16 position;
    STRUCT_PAD(2, 4);
};

struct HorseRaceState
{
    STRUCT_PAD(0, 0x1C);
    AudioPlayer sprint_audio_player;
    STRUCT_PAD(0x20, 0x170);
    u8 entry_animation[0x30] ALIGN(4);
    STRUCT_PAD(0x1A0, 0x208);
    struct RaceLaneAnimator
    {
        SpriteAnimator animator;
        STRUCT_PAD(0x14, 0x38);
    };
    RaceLaneAnimator race_horse_animators[5];
    STRUCT_PAD(0x320, 0x358);
    SpriteAnimator betting_graphics;
    STRUCT_PAD(0x36C, 0x390);
    RaceLaneAnimator race_sprint_animators[4];
    RaceLaneAnimator race_effect_animators[4];
    SpriteAnimator betting_cursor;
    SpriteFrameDrawOptions cursor_options;
    STRUCT_PAD(0x580, 0x600);
    RaceObjAllocation betting_tiles, betting_palette;
    RaceObjAllocation betting_glyph_tiles, betting_glyph_palette;
    RaceObjAllocation prompt_palette;
    STRUCT_PAD(0x628, 0x640);
    RaceObjAllocation betting_cursor_tiles, betting_cursor_palette;
    STRUCT_PAD(0x650, 0x6B8);
    RaceCourseMarker course_markers[4][8];
    RaceGlyph glyphs[148];
    i16 glyph_count;
    STRUCT_PAD(0xBDA, 0xBEA);
    u16 tilemap_work[0x800 / sizeof(u16)];
    u16 bg1_scroll_x, bg2_scroll_x, bg3_scroll_x;
    u16 bg1_last_tile_scroll, bg2_last_tile_scroll, bg3_last_tile_scroll;
    i16 bg1_pattern_phase, bg2_pattern_phase, bg3_pattern_phase;
    i16 finish_count;
    i16 finish_order[4];
    STRUCT_PAD(0x1406, 0x140A);
    i16 betting_menu_selection;
    STRUCT_PAD(0x140C, 0x140E);
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

typedef char HorseRaceProgressOffsetsMatch[
    offsetof(HorseRaceState, sprint_audio_player) == 0x1C &&
    offsetof(HorseRaceState, race_horse_animators) == 0x208 &&
    offsetof(HorseRaceState, race_sprint_animators) == 0x390 &&
    offsetof(HorseRaceState, race_effect_animators) == 0x470 &&
    offsetof(HorseRaceState, course_markers) == 0x6B8 &&
    offsetof(HorseRaceState, finish_count) == 0x13FC &&
    offsetof(HorseRaceState, finish_order) == 0x13FE &&
    offsetof(HorseRaceState, racers) == 0x142C &&
    sizeof(RaceEntrant) == 0x2C ? 1 : -1];

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
        PlayerEntrantField<u16>(self, offsetof(RaceEntrant, entrant_id)) = 128;
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
                racer->sprint_chances[j] = gHorseRaceSprintChances[racer->entrant_id][j];
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
        AppendRaceDisplayGlyph(self, 0, self->finish_order[0] * 16 + 48, 24, 2);
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

EC u32 ResolveRaceObjPalette(RaceObjAllocation *, u32) asm("ResourceHandle_GetIndex");
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

// Use the frame getter's hidden return pointer to write directly into the work buffer.
EC void ReadRaceDataFrame(RaceFrameView *, SpriteAnimationData const *, u32) asm("GetSpriteAnimationDataFrame");
EC u8 const gHorseRaceEntryModeObjTiles[] asm("gUnk_08752F8C");
EC u16 const gHorseRaceEntryModeObjPalette[] asm("gUnk_0875300C");
EC u8 const gHorseRaceBettingModeObjTiles[] asm("gUnk_0875196C");
EC u16 const gHorseRaceBettingModeObjPalette[] asm("gUnk_087519EC");
EC char const gHorseRaceOpponentNames[][13];
EC char const gHorseRaceBettingTableHeader[];
EC char const gHorseRaceEnterLabel[];
EC char const gHorseRaceMedalsToWinLabel[];

// Keep the stack slots, argument registers, and literal order used by the original Thumb code.
#define RACE_TABLE_STACK(reg, field) ({ \
    register uintptr_t stack_offset asm(reg) = offsetof(__typeof__(work), field); \
    asm ("" : "+l" (stack_offset)); \
    asm ("add %0, sp" : "+l" (stack_offset)); \
    reinterpret_cast<__typeof__(&work.field)>(stack_offset); \
})

#define RACE_TABLE_ALLOCATION(field) ({ \
    register u32 field_offset asm("r0") = offsetof(HorseRaceState, field); \
    asm ("" : "+l" (field_offset)); \
    register uintptr_t address asm("r0") = field_offset; \
    asm ("add %0, %1" : "+l" (address) : "r" (race)); \
    reinterpret_cast<RaceObjAllocation *>(address); \
})

#define RACE_TABLE_FIELD(reg, field) ({ \
    register uintptr_t address asm(reg) = offsetof(HorseRaceState, field); \
    asm ("" : "+l" (address)); \
    asm ("add %0, %1" : "+l" (address) : "r" (race)); \
    reinterpret_cast<__typeof__(&race->field)>(address); \
})

#define RACE_TABLE_STORE_POINTER(from_reg, to_reg, source_field, dest_field) do { \
    register __typeof__(&work.source_field) source_pointer asm(from_reg) = RACE_TABLE_STACK(from_reg, source_field); \
    register __typeof__(&work.dest_field) dest_pointer asm(to_reg) = RACE_TABLE_STACK(to_reg, dest_field); \
    asm volatile ("str %0, [%1]" : : "l" (source_pointer), "l" (dest_pointer) : "memory"); \
} while (0)

#define RACE_TABLE_BUFFER_ADDRESS(reg, field) ({ \
    register u32 * pixels asm(reg); \
    asm ("add %0, sp, %1" : "=l" (pixels) : "i" (offsetof(__typeof__(work), field))); \
    pixels; \
})

#define RACE_TABLE_CONSTANT(reg, value) ({ \
    register u32 constant_value asm(reg) = value; \
    asm ("" : "+l" (constant_value)); \
    constant_value; \
})

#define RACE_TABLE_OBJ_COPY(asset, resolve, allocation, base, bytes) do { \
    register __typeof__(&(asset)[0]) data asm("r4") = asset; \
    asm ("" : "+l" (data)); \
    register RaceObjAllocation * resource asm("r0") = RACE_TABLE_ALLOCATION(allocation); \
    asm ("" : "+l" (resource)); \
    register u32 index asm("r1") = resolve(resource, resource->handle); \
    asm ("" : "+l" (index)); \
    register u32 offset asm("r1") = index * TILE_BYTES; \
    asm ("" : "+l" (offset)); \
    register void * dest asm("r1") = reinterpret_cast<void *>(offset + base); \
    asm ("" : "+l" (dest)); \
    Dma3_Copy(data, dest, bytes); \
} while (0)

EC SECTION(".text.f3.08059FE0") void LoadHorseRaceBettingTableGraphics(HorseRaceState * self, u32 betting)
{
    enum
    {
        TILE_BYTES = 32,
        NAME_COLUMNS = 12,
        HEADING_COLUMNS = 32,
        LABEL_ROWS = 2,
        TILEMAP_COLUMNS = 32,
        RACER_COUNT = 4,
        BUILT_IN_RACER_COUNT = 40,
        NAME_TOP_ROW = 6,
        NAME_FIRST_COLUMN = 4,
        FIRST_HEADING_ROW = 4,
        SECOND_HEADING_ROW = 15,
        TABLE_PALETTE = 2 << 12,
        NAME_TILES_BASE = TABLE_PALETTE | 0x140,
        FIRST_HEADING_TILE = TABLE_PALETTE | 0x200,
        SECOND_HEADING_TILE = TABLE_PALETTE | 0x240,
        NAME_TILES_VRAM = 0x06002800,
        HEADING_TILES_VRAM = 0x06004000,
        PROMPT_TILES_VRAM = 0x06004800,
        TABLE_MAP_VRAM = 0x0600F000,
        OBJ_TILES_VRAM = 0x06010000,
        PROMPT_OBJ_TILES_VRAM = 0x06010600,
        OBJ_PALETTE_RAM = 0x05000200,
        PROMPT_OBJ_TILE_BYTES = 4 * TILE_BYTES
    };
    // The renderer takes three stack arguments before the local buffers.
    struct
    {
        u32 outgoing_text_args[3];
        RaceFrameView frame;
        u32 frame_padding;
        u32 name_pixels[NAME_COLUMNS * LABEL_ROWS * TILE_BYTES / sizeof(u32)];
        u32 name_padding;
        u32 heading_pixels[HEADING_COLUMNS * LABEL_ROWS * TILE_BYTES / sizeof(u32)];
        RaceHelpTileSize name_fill, opponent_name, player_name, heading_fill, heading_draw, prompt_fill,
            entry_prompt, betting_prompt;
        u32 betting;
        u32 * heading_buffer;
        RaceHelpTileSize * heading_fill_size, * heading_draw_size, * prompt_fill_size;
        u32 * name_buffer;
        RaceHelpTileSize * name_fill_size;
    } work;
    register HorseRaceState * race asm("r8") = self;
    asm ("" : "+r" (race));
    *RACE_TABLE_STACK("r0", betting) = betting;

    if (!betting)
    {
        register uintptr_t entry asm("r1") = offsetof(HorseRaceState, entry_animation);
        asm ("" : "+l" (entry));
        asm ("add %0, %1" : "+l" (entry) : "r" (race));
        ReadRaceDataFrame(&work.frame, reinterpret_cast<SpriteAnimationData const *>(entry), 0);

        register RaceFrameSlice<u16> const * palette_slice asm("r5") = &work.frame.palette;
        asm ("" : "+l" (palette_slice));
        register u16 const * palette asm("r4") = work.frame.palette.data;
        asm ("" : "+l" (palette));
        register RaceObjAllocation * resource asm("r0") = RACE_TABLE_ALLOCATION(betting_cursor_palette);
        asm ("" : "+l" (resource));
        register u32 palette_offset asm("r0") = ResolveRaceObjPalette(resource, resource->handle) * TILE_BYTES;
        asm ("" : "+l" (palette_offset));
        register u32 palette_ram asm("r6") = OBJ_PALETTE_RAM;
        asm ("" : "+l" (palette_ram));
        register void * dest asm("r1") = reinterpret_cast<void *>(palette_offset + palette_ram);
        asm ("" : "+l" (dest));
        register u16 const * palette_data asm("r0") = work.frame.palette.data;
        register u32 palette_length asm("r2") = 0;
        asm ("" : "+l" (palette_data), "+l" (palette_length));
        if (palette_data)
            palette_length = palette_slice->length;
        Dma3_Copy(palette, dest, palette_length);

        register RaceFrameSlice<u8> const * tile_slice asm("r4") = &work.frame.tiles;
        asm ("" : "+l" (tile_slice));
        register u8 const * tiles asm("r5") = work.frame.tiles.data;
        asm ("" : "+l" (tiles));
        resource = RACE_TABLE_ALLOCATION(betting_cursor_tiles);
        asm ("" : "+l" (resource));
        register u32 tile_offset asm("r0") = ResolveRaceObjTile(resource, resource->handle) * TILE_BYTES;
        asm ("" : "+l" (tile_offset));
        register u32 tile_vram asm("r2") = OBJ_TILES_VRAM;
        asm ("" : "+l" (tile_vram));
        dest = reinterpret_cast<void *>(tile_offset + tile_vram);
        asm ("" : "+l" (dest));
        register u8 const * tile_data asm("r0") = work.frame.tiles.data;
        register u32 tile_length asm("r2") = 0;
        asm ("" : "+l" (tile_data), "+l" (tile_length));
        if (tile_data)
            tile_length = tile_slice->length;
        Dma3_Copy(tiles, dest, tile_length);

        RACE_TABLE_OBJ_COPY(gHorseRaceEntryModeObjTiles, ResolveRaceObjTile, betting_glyph_tiles,
            RACE_TABLE_CONSTANT("r3", PROMPT_OBJ_TILES_VRAM), PROMPT_OBJ_TILE_BYTES);
        RACE_TABLE_OBJ_COPY(gHorseRaceEntryModeObjPalette, ResolveRaceObjPalette, prompt_palette,
            palette_ram, TILE_BYTES);
    }
    else
    {
        RACE_TABLE_OBJ_COPY(gHorseRaceBettingModeObjTiles, ResolveRaceObjTile, betting_glyph_tiles,
            RACE_TABLE_CONSTANT("r7", PROMPT_OBJ_TILES_VRAM), PROMPT_OBJ_TILE_BYTES);
        RACE_TABLE_OBJ_COPY(gHorseRaceBettingModeObjPalette, ResolveRaceObjPalette, prompt_palette,
            RACE_TABLE_CONSTANT("r0", OBJ_PALETTE_RAM), TILE_BYTES);
    }

    register RaceEntrant * racer asm("r4") = *RACE_TABLE_FIELD("r4", racers);
    asm ("" : "+l" (racer));
    RACE_TABLE_STORE_POINTER("r1", "r2", name_fill, name_fill_size);
    {
        register u32 * pixels asm("r3");
        asm ("mov %0, sp\n\tadd %0, %1" : "=l" (pixels)
            : "i" (offsetof(__typeof__(work), name_pixels)) : "cc");
        register u32 ** name_buffer asm("r7") = RACE_TABLE_STACK("r7", name_buffer);
        asm volatile ("str %0, [%1]" : : "l" (pixels), "l" (name_buffer) : "memory");
    }
    RACE_TABLE_STORE_POINTER("r0", "r1", heading_fill, heading_fill_size);
    RACE_TABLE_STORE_POINTER("r2", "r3", heading_draw, heading_draw_size);
    RACE_TABLE_STORE_POINTER("r7", "r0", heading_pixels, heading_buffer);
    RACE_TABLE_STORE_POINTER("r1", "r2", prompt_fill, prompt_fill_size);
    register u32 name_rows_low asm("r3") = LABEL_ROWS;
    asm ("" : "+l" (name_rows_low));
    register u32 name_rows asm("r10") = name_rows_low;
    asm ("" : "+r" (name_rows));
    register u32 * name_vram_low asm("r7") = reinterpret_cast<u32 *>(NAME_TILES_VRAM);
    asm ("" : "+l" (name_vram_low));
    register u32 * name_vram asm("r9") = name_vram_low;
    asm ("" : "+r" (name_vram));
    register RaceHelpTileSize * opponent_name asm("r6") = RACE_TABLE_STACK("r6", opponent_name);
    register RaceHelpTileSize * player_name asm("r5") = RACE_TABLE_STACK("r5", player_name);
    register int racers_left = RACER_COUNT - 1;
    asm ("" : "+l" (racers_left));
    do
    {
        register RaceHelpTileSize * name_fill asm("r0") = RACE_TABLE_STACK("r0", name_fill);
        register u32 name_width asm("r1") = NAME_COLUMNS;
        asm volatile ("strh %0, [%1]" : : "l" (name_width), "l" (name_fill) : "memory");
        register u32 name_height asm("r3") = name_rows;
        asm ("" : "+l" (name_height));
        register RaceHelpTileSize * fill_size asm("r2") = *RACE_TABLE_STACK("r2", name_fill_size);
        asm volatile ("strh %0, [%1, #2]" : : "l" (name_height), "l" (fill_size) : "memory");
        FillRaceHelpTiles(*name_fill, RACE_TABLE_BUFFER_ADDRESS("r1", name_pixels), 0);

        register u32 entrant_id asm("r0") = racer->entrant_id;
        asm ("" : "+l" (entrant_id));
        if (entrant_id < BUILT_IN_RACER_COUNT)
        {
            register u32 name_index asm("r1") = entrant_id;
            asm ("" : "+l" (name_index));
            register u32 stride asm("r0") = sizeof(gHorseRaceOpponentNames[0]);
            asm ("mul %0, %1, %0" : "+l" (name_index) : "l" (stride) : "cc");
            register uintptr_t names asm("r0") = reinterpret_cast<uintptr_t>(gHorseRaceOpponentNames);
            asm ("" : "+l" (names));
            register char const * name asm("r1") = reinterpret_cast<char const *>(name_index + names);
            asm ("" : "+l" (name));
            register u32 width asm("r3") = NAME_COLUMNS;
            asm volatile ("strh %0, [%1]" : : "l" (width), "l" (opponent_name) : "memory");
            register u32 height asm("r0") = name_rows;
            asm volatile ("strh %0, [%1, #2]" : : "l" (height), "l" (opponent_name) : "memory");
            asm volatile (
                "ldr r0, [%1]\n\tstr %0, [sp]\n\tmov r1, #0\n\tstr r1, [sp, #4]\n\t"
                "mov r2, #1\n\tstr r2, [sp, #8]\n\tadd r1, sp, %2\n\tmov r2, #0\n\t"
                "mov r3, #0\n\tbl DrawMessageTileStringWithColors"
                : "+l" (name) : "l" (opponent_name),
                "i" (offsetof(__typeof__(work), name_pixels))
                : "r0", "r2", "r3", "r12", "lr", "cc", "memory");
        }
        else
        {
            register char const * name asm("r0") = (*RACE_TABLE_FIELD("r0", player_horse))->GetName();
            asm ("" : "+l" (name));
            register u32 width asm("r3") = NAME_COLUMNS;
            asm volatile ("strh %0, [%1]" : : "l" (width), "l" (player_name) : "memory");
            register u32 height asm("r1") = name_rows;
            asm volatile ("strh %0, [%1, #2]" : : "l" (height), "l" (player_name) : "memory");
            asm volatile (
                "ldr r1, [%1]\n\tstr %0, [sp]\n\tmov r2, #0\n\tstr r2, [sp, #4]\n\t"
                "mov r3, #1\n\tstr r3, [sp, #8]\n\tadd r0, r1, #0\n\tadd r1, sp, %2\n\t"
                "mov r3, #0\n\tbl DrawMessageTileStringWithColors"
                : "+l" (name) : "l" (player_name),
                "i" (offsetof(__typeof__(work), name_pixels))
                : "r1", "r2", "r3", "r12", "lr", "cc", "memory");
        }

        register u32 * pixels asm("r0") = *RACE_TABLE_STACK("r1", name_buffer);
        asm ("" : "+l" (pixels));
        register u32 * destination asm("r1") = name_vram;
        asm ("" : "+l" (destination));
        Dma3_Copy(pixels, destination, RACE_TABLE_CONSTANT("r2", sizeof(work.name_pixels)));
        register u32 name_bytes asm("r2") = sizeof(work.name_pixels);
        asm ("add %0, %1" : "+r" (name_vram) : "l" (name_bytes));
        --racers_left;
        asm ("" : "+l" (racers_left));
        ++racer;
        asm ("" : "+l" (racer));
    } while (racers_left >= 0);

    register u32 heading_columns asm("r4") = HEADING_COLUMNS;
    asm ("" : "+l" (heading_columns));
    register u32 heading_rows asm("r5") = LABEL_ROWS;
    asm ("" : "+l" (heading_rows));
    register u32 text_color asm("r7") = 0;
    asm ("" : "+l" (text_color));
    {
        register RaceHelpTileSize * heading_fill asm("r0") = RACE_TABLE_STACK("r0", heading_fill);
        asm volatile ("strh %0, [%1]" : : "l" (heading_columns), "l" (heading_fill) : "memory");
        register RaceHelpTileSize * heading_fill_size asm("r3") = *RACE_TABLE_STACK("r3", heading_fill_size);
        asm volatile ("strh %0, [%1, #2]" : : "l" (heading_rows), "l" (heading_fill_size) : "memory");
        FillRaceHelpTiles(*heading_fill, RACE_TABLE_BUFFER_ADDRESS("r1", heading_pixels), 0);
    }
    {
        register char const * header_text asm("r1") = gHorseRaceBettingTableHeader;
        asm ("" : "+l" (header_text));
        register u32 text_background asm("r6") = 1;
        asm ("" : "+l" (text_background));
        register RaceHelpTileSize * heading_draw asm("r0") = RACE_TABLE_STACK("r0", heading_draw);
        asm volatile ("strh %0, [%1]" : : "l" (heading_columns), "l" (heading_draw) : "memory");
        register RaceHelpTileSize * heading_draw_size asm("r2") = *RACE_TABLE_STACK("r2", heading_draw_size);
        asm volatile ("strh %0, [%1, #2]" : : "l" (heading_rows), "l" (heading_draw_size) : "memory");
        register RaceHelpTileSize size asm("r0") = *heading_draw;
        asm volatile (
            "str %1, [sp]\n\tstr %2, [sp, #4]\n\tstr %3, [sp, #8]\n\t"
            "add r1, sp, %4\n\tmov r2, #0\n\tmov r3, #0\n\tbl DrawMessageTileStringWithColors"
            : "+l" (size), "+l" (header_text) : "l" (text_color), "l" (text_background),
            "i" (offsetof(__typeof__(work), heading_pixels))
            : "r2", "r3", "r12", "lr", "cc", "memory");
        register void * pixels_vram asm("r1") = reinterpret_cast<void *>(HEADING_TILES_VRAM);
        asm ("" : "+l" (pixels_vram));
        register u32 heading_bytes asm("r2") = sizeof(work.heading_pixels);
        asm ("" : "+l" (heading_bytes));
        register u32 * pixels asm("r0") = *RACE_TABLE_STACK("r3", heading_buffer);
        asm ("" : "+l" (pixels));
        Dma3_Copy(pixels, pixels_vram, heading_bytes);
        {
            register RaceHelpTileSize * prompt_fill asm("r0") = RACE_TABLE_STACK("r0", prompt_fill);
            asm volatile ("strh %0, [%1]" : : "l" (heading_columns), "l" (prompt_fill) : "memory");
            register RaceHelpTileSize * prompt_fill_size asm("r1") = *RACE_TABLE_STACK("r1", prompt_fill_size);
            asm volatile ("strh %0, [%1, #2]" : : "l" (heading_rows), "l" (prompt_fill_size) : "memory");
            register RaceHelpTileSize prompt_size asm("r0") = *prompt_fill;
            asm ("" : "+l" (prompt_size));
            register u32 * prompt_buffer asm("r1") = *RACE_TABLE_STACK("r2", heading_buffer);
            FillRaceHelpTiles(prompt_size, prompt_buffer, 0);
        }
        register u32 betting_mode asm("r3") = *RACE_TABLE_STACK("r3", betting);
        asm ("" : "+l" (betting_mode));
        if (!betting_mode)
        {
            register char const * prompt_text asm("r2") = gHorseRaceEnterLabel;
            asm ("" : "+l" (prompt_text));
            register RaceHelpTileSize * prompt_height asm("r1") = RACE_TABLE_STACK("r1", entry_prompt);
            register RaceHelpTileSize * prompt_width asm("r0") = RACE_TABLE_STACK("r0", entry_prompt);
            asm volatile ("strh %0, [%1]\n\tstrh %2, [%3, #2]" : : "l" (heading_columns),
                "l" (prompt_width), "l" (heading_rows), "l" (prompt_height) : "memory");
            register RaceHelpTileSize size asm("r0") = *prompt_width;
            asm ("" : "+l" (size));
            // Preserve the text arguments before loading the heading buffer.
            asm volatile ("str %0, [sp]\n\tstr %1, [sp, #4]\n\tstr %2, [sp, #8]"
                : : "l" (prompt_text), "l" (text_color), "l" (text_background) : "memory");
            register u32 * pixels asm("r1") = *RACE_TABLE_STACK("r7", heading_buffer);
            asm volatile (
                "mov r2, #0\n\tmov r3, #0\n\tbl DrawMessageTileStringWithColors"
                : "+l" (size), "+l" (pixels) : : "r2", "r3", "r12", "lr", "cc", "memory");
        }
        else
        {
            register char const * prompt_text asm("r2") = gHorseRaceMedalsToWinLabel;
            asm ("" : "+l" (prompt_text));
            register RaceHelpTileSize * prompt_height asm("r1") = RACE_TABLE_STACK("r1", betting_prompt);
            register RaceHelpTileSize * prompt_width asm("r0") = RACE_TABLE_STACK("r0", betting_prompt);
            asm volatile ("strh %0, [%1]\n\tstrh %2, [%3, #2]" : : "l" (heading_columns),
                "l" (prompt_width), "l" (heading_rows), "l" (prompt_height) : "memory");
            register RaceHelpTileSize size asm("r0") = *prompt_width;
            asm ("" : "+l" (size));
            asm volatile ("str %0, [sp]\n\tstr %1, [sp, #4]\n\tstr %2, [sp, #8]"
                : : "l" (prompt_text), "l" (text_color), "l" (text_background) : "memory");
            register u32 * pixels asm("r1") = *RACE_TABLE_STACK("r2", heading_buffer);
            asm volatile (
                "mov r2, #0\n\tmov r3, #0\n\tbl DrawMessageTileStringWithColors"
                : "+l" (size), "+l" (pixels) : : "r2", "r3", "r12", "lr", "cc", "memory");
        }
    }
    register void * prompt_tiles_vram asm("r1") = reinterpret_cast<void *>(PROMPT_TILES_VRAM);
    asm ("" : "+l" (prompt_tiles_vram));
    register u32 map_bytes asm("r4") = sizeof(work.heading_pixels);
    asm ("" : "+l" (map_bytes));
    register u32 * pixels asm("r0") = *RACE_TABLE_STACK("r3", heading_buffer);
    asm ("" : "+l" (pixels));
    Dma3_Copy(pixels, prompt_tiles_vram, map_bytes);

    register u32 blank_map asm("r0") = ~0u;
    asm ("" : "+l" (blank_map));
    Dma3_Fill(blank_map, *RACE_TABLE_FIELD("r1", tilemap_work), map_bytes);
    register u32 name_tile asm("r5") = RACE_TABLE_CONSTANT("r5", NAME_TILES_BASE);
    asm ("" : "+l" (name_tile));
    register int row asm("r1") = 0;
    asm ("" : "+l" (row));
    register uintptr_t name_base_low asm("r7") =
        offsetof(HorseRaceState, tilemap_work) + NAME_FIRST_COLUMN * sizeof(u16);
    asm ("" : "+l" (name_base_low));
    asm ("add %0, %1" : "+l" (name_base_low) : "r" (race));
    register uintptr_t name_base asm("ip") = name_base_low;
    asm ("" : "+r" (name_base));
    do
    {
        register int column asm("r4") = 0;
        asm ("" : "+l" (column));
        register u32 tile_row asm("r0") = row * LABEL_ROWS;
        asm ("" : "+l" (tile_row));
        register u32 next_name_tile asm("r6") = name_tile;
        asm ("" : "+l" (next_name_tile));
        next_name_tile += NAME_COLUMNS * LABEL_ROWS;
        asm ("" : "+l" (next_name_tile));
        ++row;
        asm ("" : "+l" (row));
        register int next_row asm("r9") = row;
        asm ("" : "+r" (next_row));
        register uintptr_t top_offset asm("r1") = (tile_row + NAME_TOP_ROW) * TILEMAP_COLUMNS * sizeof(u16);
        asm ("" : "+l" (top_offset));
        register uintptr_t bottom_offset asm("r0") = tile_row;
        asm ("add %0, %1" : "+l" (bottom_offset) : "i" (NAME_TOP_ROW + 1) : "cc");
        bottom_offset *= TILEMAP_COLUMNS * sizeof(u16);
        asm ("" : "+l" (bottom_offset));
        register u32 bottom_tile asm("r3") = name_tile;
        asm ("" : "+l" (bottom_tile));
        bottom_tile += NAME_COLUMNS;
        asm ("" : "+l" (bottom_tile));
        register uintptr_t name_base_copy asm("r7") = name_base;
        asm ("" : "+l" (name_base_copy));
        register u16 * bottom asm("r2") = reinterpret_cast<u16 *>(bottom_offset + name_base_copy);
        asm ("" : "+l" (bottom));
        asm ("add %0, %1" : "+l" (top_offset) : "r" (name_base));
        register u16 * top asm("r1") = reinterpret_cast<u16 *>(top_offset);
        do
        {
            register u32 top_tile asm("r0") = name_tile + column;
            asm volatile ("strh %0, [%1]\n\tstrh %2, [%3]" : : "l" (top_tile), "l" (top), "l" (bottom_tile),
                "l" (bottom) : "memory");
            ++bottom_tile;
            asm ("" : "+l" (bottom_tile));
            ++bottom;
            asm ("" : "+l" (bottom));
            ++top;
            asm ("" : "+l" (top));
        } while (++column < NAME_COLUMNS);
        register u32 wrapped_tile asm("r0");
        asm ("lsl %0, %1, #16" : "=l" (wrapped_tile) : "l" (next_name_tile) : "cc");
        asm ("lsr %0, %1, #16" : "=l" (name_tile) : "l" (wrapped_tile) : "cc");
        row = next_row;
        asm ("" : "+l" (row));
    } while (row < RACER_COUNT);

    register int tile asm("r4") = 0;
    asm ("" : "+l" (tile));
    register u16 * tilemap asm("r5") = *RACE_TABLE_FIELD("r5", tilemap_work);
    register u32 first_heading_base asm("r6") = RACE_TABLE_CONSTANT("r0", FIRST_HEADING_TILE);
    asm ("" : "+l" (first_heading_base));
    do
    {
        register int row asm("r3") = tile;
        asm ("cmp %1, #0\n\tbge 1f\n\tadd %0, %2\n1:\n\tasr %0, %0, #5"
            : "+l" (row) : "l" (tile), "i" (TILEMAP_COLUMNS - 1) : "cc");
        register int column asm("r2") = row * TILEMAP_COLUMNS;
        asm ("" : "+l" (column));
        column = tile - column;
        asm ("" : "+l" (column));
        register int column_bytes asm("r2") = column * sizeof(u16);
        asm ("" : "+l" (column_bytes));
        register uintptr_t top_offset asm("r0") = (FIRST_HEADING_ROW + row) * TILEMAP_COLUMNS * sizeof(u16);
        asm ("" : "+l" (top_offset));
        asm ("add %0, %1, %0" : "+l" (top_offset) : "l" (column_bytes) : "cc");
        register u16 * top asm("r0") = reinterpret_cast<u16 *>(reinterpret_cast<uintptr_t>(tilemap) + top_offset);
        asm ("" : "+l" (top));
        register u32 top_tile asm("r1") = tile + first_heading_base;
        asm volatile ("strh %0, [%1]" : : "l" (top_tile), "l" (top) : "memory");
        asm ("add %0, %1" : "+l" (row) : "i" (SECOND_HEADING_ROW) : "cc");
        register uintptr_t bottom_offset asm("r3") = row * TILEMAP_COLUMNS * sizeof(u16);
        asm ("" : "+l" (bottom_offset));
        register uintptr_t offset asm("r2") = column_bytes + bottom_offset;
        asm ("" : "+l" (offset));
        register u16 * bottom asm("r2") = reinterpret_cast<u16 *>(reinterpret_cast<uintptr_t>(tilemap) + offset);
        asm ("" : "+l" (bottom));
        register u32 bottom_tile asm("r0") = tile + RACE_TABLE_CONSTANT("r1", SECOND_HEADING_TILE);
        asm volatile ("strh %0, [%1]" : : "l" (bottom_tile), "l" (bottom) : "memory");
    } while (++tile < HEADING_COLUMNS * LABEL_ROWS);
    register u16 * table_map asm("r0") = *RACE_TABLE_FIELD("r0", tilemap_work);
    asm ("" : "+l" (table_map));
    register void * map_vram asm("r1") = reinterpret_cast<void *>(TABLE_MAP_VRAM);
    asm ("" : "+l" (map_vram));
    Dma3_Copy(table_map, map_vram, sizeof(race->tilemap_work));
}

#undef RACE_TABLE_OBJ_COPY
#undef RACE_TABLE_BUFFER_ADDRESS
#undef RACE_TABLE_STORE_POINTER
#undef RACE_TABLE_FIELD
#undef RACE_TABLE_CONSTANT
#undef RACE_TABLE_ALLOCATION
#undef RACE_TABLE_STACK

EC SpriteOamBuffer * GetRaceBettingOam(HorseRaceState *) asm("HardwareContext_GetOamBuffer");

#define RACE_BETTING_FIELD(reg, field) ({ \
    register uintptr_t offset asm(reg) = offsetof(HorseRaceState, field); \
    asm ("" : "+l" (offset)); \
    reinterpret_cast<__typeof__(&self->field)>(reinterpret_cast<u8 *>(self) + offset); \
})

#define RACE_BETTING_LITERAL(reg, field) ({ \
    register u32 constant asm(reg); \
    asm ("ldr %0, .Lrace_betting_objects_" #field : "=l" (constant)); \
    constant; \
})

#define RACE_BETTING_DRAW_FRAME(result, block_reg) do { \
    CopyRaceFrame(result, result, sizeof(work.frame)); \
    register RaceFrameView * target asm("r1"); \
    asm ("add %0, sp, %1" : "=l" (target) : "i" (offsetof(__typeof__(work), frame))); \
    register RaceFrameView * output asm("r0") = result; \
    asm volatile ("ldmia %1!, {r2, r3, " block_reg "}\n\tstmia %0!, {r2, r3, " block_reg "}\n\t" \
        "ldmia %1!, {r2, r3, " block_reg "}\n\tstmia %0!, {r2, r3, " block_reg "}\n\t" \
        "ldmia %1!, {r2, r3}\n\tstmia %0!, {r2, r3}" \
        : "+l" (target), "+l" (output) : : "r2", "r3", block_reg, "memory"); \
} while (0)

#define RACE_BETTING_COPY_DRAW_FRAME() do { \
    register RaceFrameView * frame_result asm("r0") = work.fetched_pointer; \
    asm ("" : "+l" (frame_result)); \
    CopyRaceFrame(frame_result, frame_result, sizeof(work.frame)); \
    register RaceFrameView * target asm("r1") = &work.frame; \
    asm ("" : "+l" (target)); \
    register RaceFrameView * box asm("r0") = work.fetched_pointer; \
    asm ("" : "+l" (box)); \
    asm volatile ("ldmia %1!, {r2, r3, r7}\n\tstmia %0!, {r2, r3, r7}\n\t" \
        "ldmia %1!, {r2, r3, r7}\n\tstmia %0!, {r2, r3, r7}\n\t" \
        "ldmia %1!, {r2, r3}\n\tstmia %0!, {r2, r3}" \
        : "+l" (target), "+l" (box) : : "r2", "r3", "r7", "memory"); \
} while (0)

#define RACE_BETTING_SUBMIT_FRAME(temp_reg) do { \
    register SpriteOamBuffer * oam asm("r0") = GetRaceBettingOam(self); \
    asm ("" : "+l" (oam)); \
    register SpriteOamBuffer * sprites asm("r3") = oam; \
    asm ("" : "+l" (sprites)); \
    register SpriteFrameObject const * objects asm("r0") = work.frame.objects.data; \
    asm ("" : "+l" (objects)); \
    register SpriteFrameObject const * list asm("r1") = objects; \
    asm ("" : "+l" (list)); \
    register u32 count asm("r2") = 0; \
    asm ("" : "+l" (count)); \
    if (list != nullptr) { \
        register SpriteFrameDrawOptions * stack asm(temp_reg); \
        asm ("mov %0, sp" : "=l" (stack)); \
        asm ("ldrh %0, [%1, %2]" : "=l" (count) : "l" (stack), \
            "i" (offsetof(__typeof__(work), frame.objects.length)) : "memory"); \
    } \
    DrawSpriteFrameObjects(sprites, reinterpret_cast<SpriteAttributes const *>(list), count, &work.options); \
} while (0)

EC SECTION(".text.f3.0805A744") void DrawHorseRaceBettingScreenObjects(HorseRaceState * race)
{
    enum
    {
        RACER_SLOTS = 4,
        ROW_SPACING = 16,
        CURSOR_FIRST_ROW_X = 12,
        CURSOR_FIRST_ROW_Y = 56,
        CURSOR_CONFIRM_X = 28,
        CURSOR_CONFIRM_Y = 128,
        RACER_ICON_X = 16,
        RACER_ICON_FIRST_Y = 48,
        RACER_ICON_TILE_STRIDE = 4,
        PALETTE_MASK = 0xF,
        TILE_MASK = 0x3FF,
        X_MASK = 0x1FF,
        OBJECT_PRIORITY = 2,
        PROMPT_GLYPH = 24,
        SHAPE_TALL = 2 << 14,
        SIZE_MASK = 3u << 30,
    };
    // Keep the frame copies and OAM scratch space in the original stack slots.
    struct
    {
        SpriteFrameDrawOptions options;
        RaceFrameView frame;
        RaceFrameView fetched;
        SpriteFrameObject glyph;
        RaceFrameView * fetched_pointer;
    } work;

    register HorseRaceState * self asm("r5") = race;
    asm ("" : "+l" (self));
    register SpriteFrameDrawOptions * initial asm("r1") = &work.options;
    asm ("" : "+l" (initial));
    register u32 zero_byte asm("r2") = 0;
    asm ("" : "+l" (zero_byte));
    register u32 zero asm("r0") = 0;
    asm ("" : "+l" (zero));
    asm volatile (
        "strh %0, [%2]\n\tstrh %0, [%2, #2]\n\tstrh %0, [%2, #4]\n\tstrh %0, [%2, #6]\n\tstrh %0, [%2, #8]\n\t"
        "str %0, [sp, #12]\n\tstrb %1, [%2, #16]\n\tstrb %1, [%2, #17]\n\tstr %0, [sp, #20]\n\tstrb %1, [%2, #24]"
        : : "l" (zero), "l" (zero_byte), "l" (initial) : "memory");

#define CLEAR_SLICE(field, reg) do { \
    register void * slice asm(reg); \
    asm ("add %0, sp, %1" : "=l" (slice) : "i" (offsetof(__typeof__(work), frame.field))); \
    asm volatile ("str %0, [sp, %2]\n\tstrh %0, [%1, #4]" : : \
        "l" (zero), "l" (slice), "i" (offsetof(__typeof__(work), frame.field)) : "memory"); \
} while (0)
    CLEAR_SLICE(objects, "r2");
    CLEAR_SLICE(tiles, "r1");
    CLEAR_SLICE(palette, "r1");
    CLEAR_SLICE(extras, "r1");
#undef CLEAR_SLICE

    register uintptr_t selection_field asm("r0") = RACE_BETTING_LITERAL("r0", selection);
    register i16 * selection asm("r6") = reinterpret_cast<i16 *>(reinterpret_cast<uintptr_t>(self) + selection_field);
    asm ("" : "+l" (selection));
    register u32 selection_start asm("r1") = 0;
    asm ("" : "+l" (selection_start));
    register int slot asm("r0");
    asm ("ldrsh %0, [%1, %2]" : "=l" (slot) : "l" (selection), "l" (selection_start) : "memory");
    register RaceFrameView * fetched = &work.fetched;
    asm ("" : "+l" (fetched));
    if (slot >= 0)
    {
        register SpriteAnimator * cursor asm("r4") = RACE_BETTING_FIELD("r2", betting_cursor);
        asm ("" : "+l" (cursor));
        AdvanceSpriteAnimator(cursor);
        register RaceFrameSource const * source asm("r1") = reinterpret_cast<RaceFrameSource const *>(cursor->source);
        register RaceFrameSourceVTable const * vtable asm("r3") = source->vtable;
        register u32 keyframe asm("r2") = cursor->keyframe;
        asm ("" : "+l" (source), "+l" (vtable), "+l" (keyframe));
        register uintptr_t clip_field asm("r4") = RACE_BETTING_LITERAL("r4", cursor_clip);
        register SpriteAnimationClip const * cursor_clip asm("r0") =
            reinterpret_cast<SpriteAnimationClip const *>(reinterpret_cast<uintptr_t>(self) + clip_field);
        asm ("" : "+l" (cursor_clip));
        register u32 frame_index asm("r2") = keyframe * sizeof(SpriteKeyframe);
        asm ("" : "+l" (frame_index));
        register SpriteKeyframe const * keyframes asm("r0") = cursor_clip->keyframes;
        asm ("" : "+l" (keyframes));
        register u32 frame_id asm("r2") = reinterpret_cast<SpriteKeyframe const *>(
            reinterpret_cast<uintptr_t>(keyframes) + frame_index)->frame;
        asm ("" : "+l" (frame_id));
        register RaceFrameView * target asm("r0") = fetched;
        asm ("" : "+l" (target));
        vtable->get_frame(target, source, frame_id);
        RACE_BETTING_DRAW_FRAME(fetched, "r4");

        register SpriteFrameDrawOptions * cursor_draw asm("r1") = &work.options;
        asm ("" : "+l" (cursor_draw));
        register uintptr_t style_field asm("r4") = RACE_BETTING_LITERAL("r4", cursor_style);
        register SpriteFrameDrawOptions const * cursor_style asm("r0") =
            reinterpret_cast<SpriteFrameDrawOptions const *>(reinterpret_cast<uintptr_t>(self) + style_field);
        asm volatile ("ldmia %1!, {r2, r3, r4}\n\tstmia %0!, {r2, r3, r4}\n\t"
            "ldmia %1!, {r2, r3, r4}\n\tstmia %0!, {r2, r3, r4}\n\t"
            "ldr %1, [%1]\n\tstr %1, [%0]"
            : "+l" (cursor_draw), "+l" (cursor_style) : : "r2", "r3", "r4", "memory");
        register SpriteFrameDrawOptions * cursor_offset_x asm("r2") = &work.options;
        asm ("" : "+l" (cursor_offset_x));
        register u32 selection_offset asm("r1") = 0;
        asm ("" : "+l" (selection_offset));
        register int current_slot asm("r0");
        asm ("ldrsh %0, [%1, %2]" : "=l" (current_slot) : "l" (selection), "l" (selection_offset) : "memory");
        register u32 cursor_x asm("r1") = CURSOR_CONFIRM_X;
        asm ("" : "+l" (cursor_x));
        asm ("cmp %1, %2\n\tbgt 1f\n\tmov %0, %3\n1:" : "+l" (cursor_x)
            : "l" (current_slot), "i" (RACER_SLOTS - 1), "i" (CURSOR_FIRST_ROW_X) : "cc");
        asm volatile ("strh %0, [%1]" : : "l" (cursor_x), "l" (cursor_offset_x) : "memory");
        register SpriteFrameDrawOptions * cursor_offset_y asm("r1") = &work.options;
        asm ("" : "+l" (cursor_offset_y));
        register int cursor_y asm("r0");
        // The original literal pool occupies the gap between the two cursor positions.
        typedef char CursorPoolOffsetsMatch[
            offsetof(HorseRaceState, betting_menu_selection) == 0x140A &&
            offsetof(HorseRaceState, betting_cursor.clip) == 0x554 &&
            offsetof(HorseRaceState, cursor_options) == 0x564 ? 1 : -1];
        asm ("cmp %1, %3\n\tbgt 1f\n\tmov r2, #0\n\tldrsh %0, [%2, r2]\n\tlsl %0, %0, %4\n\tadd %0, %5\n\tb 2f\n"
            ".balign 4, 0\n"
            ".Lrace_betting_objects_selection: .word 0x140A\n"
            ".Lrace_betting_objects_cursor_clip: .word 0x554\n"
            ".Lrace_betting_objects_cursor_style: .word 0x564\n"
            "1:\n\tmov %0, %6\n2:" : "=l" (cursor_y)
            : "l" (current_slot), "l" (selection), "i" (RACER_SLOTS - 1), "i" (4), "i" (CURSOR_FIRST_ROW_Y),
              "i" (CURSOR_CONFIRM_Y) : "r2", "cc", "memory");
        asm volatile ("strh %0, [%1, #2]" : : "l" (cursor_y), "l" (cursor_offset_y) : "memory");
        register RaceObjAllocation * tiles asm("r0") = RACE_BETTING_FIELD("r3", betting_cursor_tiles);
        asm ("" : "+l" (tiles));
        work.options.tile_base = ResolveRaceObjTile(tiles, tiles->handle);
        register RaceObjAllocation * palette asm("r0") = RACE_BETTING_FIELD("r4", betting_cursor_palette);
        asm ("" : "+l" (palette));
        work.options.palette_base = ResolveRaceObjPalette(palette, palette->handle);
        work.options.priority = OBJECT_PRIORITY;
        work.options.mosaic = true;
        RACE_BETTING_SUBMIT_FRAME("r0");
    }

    register int racer asm("r6") = 0;
    asm ("" : "+l" (racer));
    register SpriteAnimationClip const * betting_clip asm("r10") = RACE_BETTING_FIELD("r1", betting_graphics.clip);
    asm ("" : "+r" (betting_clip));
    register SpriteFrameObject * glyph_stack asm("r2") = &work.glyph;
    asm ("" : "+l" (glyph_stack));
    register SpriteFrameObject * glyph asm("r9") = glyph_stack;
    asm ("" : "+r" (glyph));
    register SpriteAnimator * icon asm("r8") = RACE_BETTING_FIELD("r3", betting_graphics);
    asm ("" : "+r" (icon));
    register SpriteFrameDrawOptions * icon_draw asm("r4") = &work.options;
    asm ("" : "+l" (icon_draw));
    work.fetched_pointer = fetched;
    do
    {
        SetSpriteAnimatorClip(icon, racer);
        register SpriteAnimator * current_icon asm("r7") = icon;
        asm ("" : "+l" (current_icon));
        register RaceFrameSource const * source asm("r1");
        asm ("ldr %0, [%1]" : "=l" (source) : "l" (current_icon) : "memory");
        register RaceFrameSourceVTable const * vtable asm("r3") = source->vtable;
        asm ("" : "+l" (vtable));
        register u32 keyframe asm("r2");
        asm ("ldrh %0, [%1, %2]" : "=l" (keyframe)
            : "l" (current_icon), "i" (offsetof(SpriteAnimator, keyframe)) : "memory");
        asm ("" : "+l" (source), "+l" (vtable), "+l" (keyframe));
        register u32 frame_index asm("r2") = keyframe * sizeof(SpriteKeyframe);
        asm ("" : "+l" (frame_index));
        register SpriteAnimationClip const * clip asm("r7") = betting_clip;
        asm ("" : "+l" (clip));
        register SpriteKeyframe const * keyframes asm("r0") = clip->keyframes;
        asm ("" : "+l" (keyframes));
        register u32 frame_id asm("r2") = reinterpret_cast<SpriteKeyframe const *>(
            reinterpret_cast<uintptr_t>(keyframes) + frame_index)->frame;
        asm ("" : "+l" (frame_id));
        register RaceFrameView * target asm("r0") = &work.fetched;
        asm ("" : "+l" (target));
        vtable->get_frame(target, source, frame_id);
        RACE_BETTING_COPY_DRAW_FRAME();
        icon_draw->offset_x = RACER_ICON_X;
        icon_draw->offset_y = racer * ROW_SPACING + RACER_ICON_FIRST_Y;
        RaceObjAllocation * palette = RACE_BETTING_FIELD("r7", betting_palette);
        icon_draw->palette_base = ResolveRaceObjPalette(palette, palette->handle);
        RaceObjAllocation * tiles = RACE_BETTING_FIELD("r1", betting_tiles);
        icon_draw->tile_base = ResolveRaceObjTile(tiles, tiles->handle) + racer * RACER_ICON_TILE_STRIDE;
        icon_draw->priority = OBJECT_PRIORITY;
        icon_draw->mosaic = true;
        RACE_BETTING_SUBMIT_FRAME("r7");
        ++racer;
        asm ("" : "+l" (racer));
    } while (racer < static_cast<int>(RACER_SLOTS));

    enum { AFFINE_MASK = 3 << 8, OBJECT_MODE_MASK = 3 << 10, MOSAIC = 1 << 12,
        COLOR_MODE_MASK = 1 << 13, SHAPE_MASK = 3 << 14, AFFINE_PARAM_MASK = 7 << 25,
        HFLIP_MASK = 1 << 28, VFLIP_MASK = 1 << 29 };

#define CLEAR_GLYPH_BITS(field_mask) do { \
    register u32 mask asm("r1") = ~(field_mask); \
    asm ("" : "+l" (mask)); \
    asm ("and %0, %1" : "+l" (flags) : "l" (mask) : "cc"); \
} while (0)
    register u32 mask asm("r1") = ~AFFINE_MASK;
    asm ("" : "+l" (mask));
    register u32 flags asm("r0");
    asm ("ldr %0, [sp, %2]\n\tand %0, %1" : "=l" (flags) : "l" (mask),
        "i" (offsetof(__typeof__(work), glyph.attributes[0])) : "cc", "memory");
    CLEAR_GLYPH_BITS(OBJECT_MODE_MASK);
    register u32 mosaic asm("r1") = MOSAIC;
    asm ("" : "+l" (mosaic));
    asm ("orr %0, %1" : "+l" (flags) : "l" (mosaic) : "cc");
    CLEAR_GLYPH_BITS(COLOR_MODE_MASK);
    CLEAR_GLYPH_BITS(SHAPE_MASK);
    CLEAR_GLYPH_BITS(AFFINE_PARAM_MASK);
    CLEAR_GLYPH_BITS(HFLIP_MASK);
    CLEAR_GLYPH_BITS(VFLIP_MASK);
    register u32 clear_size asm("r1") = ~SIZE_MASK;
    asm ("" : "+l" (clear_size));
    asm ("and %0, %1\n\tadd %1, #1\n\torr %0, %1" : "+l" (flags), "+l" (clear_size) : : "cc");
    work.glyph.attributes[0] = flags;
#undef CLEAR_GLYPH_BITS

#define RACE_GLYPH_PALETTE(reg, field, glyph_reg) do { \
    register RaceObjAllocation * palette asm("r0") = RACE_BETTING_FIELD(reg, field); \
    asm ("" : "+l" (palette)); \
    register u32 palette_index asm("r0") = ResolveRaceObjPalette(palette, palette->handle); \
    register u32 palette_bits asm("r0"); \
    asm ("lsl %0, %1, #16\n\tlsr %0, %0, #16" : "=l" (palette_bits) : "l" (palette_index) : "cc"); \
    register u32 palette_mask asm("r1") = PALETTE_MASK; \
    asm ("" : "+l" (palette_mask)); \
    asm ("and %0, %1\n\tlsl %0, %0, #12" : "+l" (palette_bits) : "l" (palette_mask) : "cc"); \
    register u32 clear_palette asm("r2") = ~(PALETTE_MASK << 12); \
    asm ("" : "+l" (clear_palette)); \
    register SpriteFrameObject * palette_out asm(glyph_reg) = glyph; \
    asm ("" : "+l" (palette_out)); \
    register u32 attrs asm("r1"); \
    asm ("ldr %0, [%1, #4]\n\tand %0, %2\n\torr %0, %3" : "=&l" (attrs) : \
        "l" (palette_out), "l" (clear_palette), "l" (palette_bits) : "cc", "memory"); \
    FINISH_RACE_PALETTE(attrs, palette_out); \
} while (0)

#define FINISH_RACE_PALETTE(attrs, palette_out) do { \
    register u32 mask asm("r0") = 0xFFFF; \
    asm ("" : "+l" (mask)); \
    asm volatile ("and %0, %2\n\tstr %0, [%1, #4]" : "+l" (attrs) : "l" (palette_out), "l" (mask) : "cc", "memory"); \
} while (0)
    RACE_GLYPH_PALETTE("r1", betting_glyph_palette, "r3");
#undef FINISH_RACE_PALETTE

    enum { GLYPH_STRIDE_SHIFT = 3 };
    register uintptr_t count_offset asm("r2");
#define POP_RACE_GLYPH() ({ \
    count_offset = offsetof(HorseRaceState, glyph_count); \
    asm ("" : "+l" (count_offset)); \
    register i16 * glyph_count asm("r1") = reinterpret_cast<i16 *>(reinterpret_cast<uintptr_t>(self) + count_offset); \
    asm ("" : "+l" (glyph_count)); \
    register i32 glyph_index asm("r0"); \
    asm volatile ("ldrh %0, [%1]\n\tsub %0, #1\n\tstrh %0, [%1]\n\tlsl %0, %0, #16" \
        : "=l" (glyph_index) : "l" (glyph_count) : "cc", "memory"); \
    asm ("" : "+l" (count_offset)); \
    glyph_index; \
})

#define RACE_GLYPH_READ(name, field_offset_reg, zero_reg, is_signed, field) \
    register u32 zero_idx asm(zero_reg) = 0; \
    asm ("" : "+l" (zero_idx)); \
    register int glyph_byte_offset asm("r1"); \
    asm ("ldrsh %0, [%1, %2]\n\tlsl %0, %0, %3" : "=l" (glyph_byte_offset) \
        : "l" (active_index), "l" (zero_idx), "i" (GLYPH_STRIDE_SHIFT) : "cc", "memory"); \
    register uintptr_t member_offset asm(field_offset_reg) = \
        offsetof(HorseRaceState, glyphs) + offsetof(RaceGlyph, field); \
    asm ("" : "+l" (member_offset)); \
    register uintptr_t field_base asm("r0") = reinterpret_cast<uintptr_t>(self) + member_offset; \
    asm ("" : "+l" (field_base)); \
    register void const * field_pointer asm("r0") = reinterpret_cast<void const *>(field_base + glyph_byte_offset); \
    asm ("" : "+l" (field_pointer)); \
    register u32 name asm("r1"); \
    is_signed

#define RACE_GLYPH_LDRH asm ("ldrh %0, [%1]" : "=l" (value) : "l" (field_pointer) : "memory");
#define RACE_GLYPH_LDRB asm ("ldrb %0, [%1]" : "=l" (value) : "l" (field_pointer) : "memory");
#define RACE_GLYPH_LDRSH \
    register u32 zero_shift asm("r3") = 0; \
    asm ("" : "+l" (zero_shift)); \
    asm ("ldrsh %0, [%1, %2]" : "=l" (value) : "l" (field_pointer), "l" (zero_shift) : "memory");

    register i32 remaining asm("r0") = POP_RACE_GLYPH();
    if (remaining < 0)
        return;
    register SpriteFrameObject * glyph_output asm("r6") = glyph;
    asm ("" : "+l" (glyph_output));
    register uintptr_t tile_field asm("r4") = offsetof(HorseRaceState, glyphs) + offsetof(RaceGlyph, tile);
    asm ("" : "+l" (tile_field));
    register uintptr_t glyph_tile_base asm("r7") = reinterpret_cast<uintptr_t>(self) + tile_field;
    asm ("" : "+l" (glyph_tile_base));
    do
    {
        register i16 const * active_index asm("r4") =
            reinterpret_cast<i16 const *>(reinterpret_cast<uintptr_t>(self) + count_offset);
        asm ("" : "+l" (active_index));
        {
            RACE_GLYPH_READ(value, "r2", "r0", RACE_GLYPH_LDRH, oam_priority)
            register u32 keep_priority asm("r0") = 3;
            asm ("" : "+l" (keep_priority));
            asm ("and %0, %1\n\tlsl %0, %0, #10" : "+l" (value) : "l" (keep_priority) : "cc");
            register u32 clear_priority asm("r2") = ~OBJECT_MODE_MASK;
            asm ("" : "+l" (clear_priority));
            asm volatile ("ldr r0, [%0, #4]\n\tand r0, %2\n\torr r0, %1\n\tstr r0, [%0, #4]" :
                : "l" (glyph_output), "l" (value), "l" (clear_priority) : "r0", "cc", "memory");
        }
        register u32 position asm("r2");
        {
            RACE_GLYPH_READ(value, "r2", "r3", RACE_GLYPH_LDRSH, x)
            register u32 x_mask asm("r0") = X_MASK;
            asm ("" : "+l" (x_mask));
            asm ("and %0, %1\n\tlsl %0, %0, #16" : "+l" (value) : "l" (x_mask) : "cc");
            register u32 clear_x asm("r0") = ~(X_MASK << 16);
            asm ("" : "+l" (clear_x));
            asm ("ldr %0, [sp, %3]\n\tand %0, %2\n\torr %0, %1"
                : "=l" (position) : "l" (value), "l" (clear_x),
                    "i" (offsetof(__typeof__(work), glyph.attributes[0])) : "cc", "memory");
        }
        {
            RACE_GLYPH_READ(value, "r3", "r0", RACE_GLYPH_LDRB, y)
            register u32 clear_y asm("r0") = ~0xFF;
            asm ("" : "+l" (clear_y));
            asm volatile ("and %0, %2\n\torr %0, %1\n\tstr %0, [sp, %3]" :
                "+l" (position) : "l" (value), "l" (clear_y),
                "i" (offsetof(__typeof__(work), glyph.attributes[0])) : "cc", "memory");
        }
        register RaceObjAllocation * tiles asm("r0") = RACE_BETTING_FIELD("r1", betting_glyph_tiles);
        asm ("" : "+l" (tiles));
        register u32 tile_base asm("r0") = ResolveRaceObjTile(tiles, tiles->handle);

#define READ_RACE_GLYPH_TILE(out, zero_index_reg, zero_tile_reg) do { \
    register u32 zero_index asm(zero_index_reg) = 0; \
    asm ("" : "+l" (zero_index)); \
    asm ("ldrsh %0, [%1, %2]\n\tlsl %0, %0, %3\n\tadd %0, %4, %0" : "=l" (out) \
        : "l" (active_index), "l" (zero_index), "i" (GLYPH_STRIDE_SHIFT), "l" (glyph_tile_base) : "cc", "memory"); \
    register u32 zero_tile asm(zero_tile_reg) = 0; \
    asm ("" : "+l" (zero_tile)); \
    asm ("ldrsh %0, [%0, %1]" : "+l" (out) : "l" (zero_tile) : "memory"); \
} while (0)
        register i32 tile asm("r1");
        READ_RACE_GLYPH_TILE(tile, "r2", "r3");
        asm ("lsl %1, %1, #1\n\tadd %0, %0, %1\n\tlsl %0, %0, #22\n\tlsr %0, %0, #22"
            : "+l" (tile_base), "+l" (tile) : : "cc");
        register u32 clear_tile asm("r2") = ~TILE_MASK;
        asm ("" : "+l" (clear_tile));
        asm volatile ("ldr r1, [%0, #4]\n\tand r1, %1\n\torr r1, %2\n\tstr r1, [%0, #4]" :
            : "l" (glyph_output), "l" (clear_tile), "l" (tile_base) : "r1", "cc", "memory");
        register i32 current_tile asm("r0");
        READ_RACE_GLYPH_TILE(current_tile, "r1", "r2");
#define FINISH_RACE_PALETTE(attrs, palette_out) \
    asm volatile ("str %0, [%1, #4]" : : "l" (attrs), "l" (palette_out) : "memory")
        register u32 shape_bits asm("r0");
        if (current_tile != static_cast<int>(PROMPT_GLYPH))
        {
            RACE_GLYPH_PALETTE("r3", betting_glyph_palette, "r4");
            register u32 clear_shape asm("r1") = ~SHAPE_MASK;
            asm ("" : "+l" (clear_shape));
            asm ("ldr %0, [sp, %2]\n\tand %0, %1" : "=l" (shape_bits) : "l" (clear_shape),
                "i" (offsetof(__typeof__(work), glyph.attributes[0])) : "cc", "memory");
            register u32 tall asm("r1") = SHAPE_TALL;
            asm ("" : "+l" (tall));
            asm ("orr %0, %1" : "+l" (shape_bits) : "l" (tall) : "cc");
            register u32 clear_size asm("r1") = ~SIZE_MASK;
            asm ("" : "+l" (clear_size));
            asm ("and %0, %1" : "+l" (shape_bits) : "l" (clear_size) : "cc");
        }
        else
        {
            RACE_GLYPH_PALETTE("r1", prompt_palette, "r3");
            register u32 clear_shape asm("r1") = ~SHAPE_MASK;
            asm ("" : "+l" (clear_shape));
            asm ("ldr %0, [sp, %2]\n\tand %0, %1" : "=l" (shape_bits) : "l" (clear_shape),
                "i" (offsetof(__typeof__(work), glyph.attributes[0])) : "cc", "memory");
            register u32 clear_size asm("r1") = ~SIZE_MASK;
            asm ("" : "+l" (clear_size));
            asm ("and %0, %1\n\tadd %1, #1\n\torr %0, %1" : "+l" (shape_bits), "+l" (clear_size) : : "cc");
        }
        work.glyph.attributes[0] = shape_bits;
        SpriteOamBuffer * oam = GetRaceBettingOam(self);
        u32 count = oam->sprite_count;
        if (count < 128)
        {
            SpriteAttributes * output = &oam->sprites[count];
            output->words[0] = work.glyph.attributes[0];
            register SpriteFrameObject * source asm("r4") = glyph;
            asm ("" : "+l" (source));
            register u32 tile_and_palette asm("r0");
            asm ("ldrh %0, [%1, #4]" : "=l" (tile_and_palette) : "l" (source) : "memory");
            reinterpret_cast<u16 *>(output)[2] = tile_and_palette;
            oam->sprite_count = count + 1;
        }
        remaining = POP_RACE_GLYPH();
    } while (remaining >= 0);

#undef FINISH_RACE_PALETTE
#undef READ_RACE_GLYPH_TILE
#undef RACE_GLYPH_LDRSH
#undef RACE_GLYPH_LDRB
#undef RACE_GLYPH_LDRH
#undef RACE_GLYPH_READ
#undef POP_RACE_GLYPH
#undef RACE_GLYPH_PALETTE
}

#undef RACE_BETTING_SUBMIT_FRAME
#undef RACE_BETTING_COPY_DRAW_FRAME
#undef RACE_BETTING_DRAW_FRAME
#undef RACE_BETTING_LITERAL
#undef RACE_BETTING_FIELD

EC u16 HardwareContext_GetPressedKeys(HardwareContext *);
EC void AudioPlayer_StartSong(AudioPlayer *, u16);

enum RaceSprintPhase
{
    SPRINT_READY,
    SPRINT_START,
    SPRINT_ACCELERATING,
    SPRINT_AT_SPEED,
    SPRINT_DECELERATING,
};

EC SECTION(".text.f3.0805C96C") int UpdateHorseRaceProgress(HorseRaceState * race)
{
    enum
    {
        ENTRANT_COUNT = 4,
        PLAYER_HORSE_ANIMATOR = 4,
        STAMINA_REGEN_INTERVAL = 10,
        STAMINA_REGEN = 3,
        STAMINA_LIMIT = 255,
        SPRINT_COST = 40,
        SPRINT_PHASE_FRAMES = 60,
        SPRINT_RETRY_FRAMES = 12,
        FINISH_DISTANCE = 184 << 16,
        RACETRACK_LENGTH = 1720 << 16,
        SPRINT_SOUND = 0xA8,
        BUTTON_A = 1,
        CAMERA_LEAD = 120,
        CAMERA_END = 48,
        CAMERA_FAR_ORIGIN = 1536,
        NORMAL_ANIMATION_RATE_Q8 = 256,
        SPRINT_ANIMATION_RATE_Q8 = 384,
        NO_FINISH_RANK = 255,
        AI_SPRINT_MIN_GAP = 32 << 16,
        AI_SPRINT_HOME_STRETCH = 768 << 16,
        VICTORY_CLIP_LANE_ONE = 0x76A,
        VICTORY_CLIP_LANE_TWO = 0x746,
        VICTORY_CLIP_LANE_THREE = 0x752,
        VICTORY_CLIP_LANE_FOUR = 0x75E,
        VICTORY_CLIP_PLAYER_HORSE = 0x776,
    };

    // The original Thumb branches enter shared blocks with live values in fixed registers.
    register HorseRaceState * self asm("r6") = race;
    asm ("" : "+l" (self));
    register RaceEntrant * racer asm("r5") = self->racers;
    asm ("" : "+l" (racer));
    int order[ENTRANT_COUNT], distances_left[ENTRANT_COUNT];
    register int track_length asm("r1") = RACETRACK_LENGTH;
    asm ("" : "+l" (track_length));
    volatile int camera_distance = track_length;
    register int first_slot asm("r2") = 0;
    asm ("" : "+l" (first_slot));
    register int slot asm("r8") = first_slot;
    asm ("" : "+h" (slot));
    register int * distance_base asm("r3") = distances_left;
    asm ("" : "+l" (distance_base));
    register int * distance asm("r10") = distance_base;
    asm ("" : "+h" (distance));
    #define PIN_ANIMATOR(result, reg, field) \
        register uintptr_t result##_offset asm(reg) = offsetof(HorseRaceState, field); \
        asm ("" : "+l" (result##_offset)); \
        SpriteAnimator * result = reinterpret_cast<SpriteAnimator *>(reinterpret_cast<u8 *>(self) + result##_offset)
    #define PIN_RACE_VALUE(reg, value) ({ \
        register int animation_speed asm(reg) = value; \
        asm ("" : "+l" (animation_speed)); \
        animation_speed; \
    })
    #define PIN_ANIMATOR_RESULT(result, reg, out_reg, field) \
        PIN_ANIMATOR(typed_##result, reg, field); \
        register SpriteAnimator * result asm(out_reg) = typed_##result; \
        asm ("" : "+l" (result))
    #define SET_PAIR_ANIMATORS(reg, field, other_reg, other_field, speed_reg, speed) { \
        PIN_ANIMATOR_RESULT(first, reg, "r0", field); \
        register int rate asm(speed_reg) = (speed); \
        asm ("" : "+l" (rate)); \
        first->speed_q8 = rate; \
        PIN_ANIMATOR_RESULT(second, other_reg, "r0", other_field); \
        second->speed_q8 = rate; \
    }
    #define PLAYER_RACING(offset_reg) ({ \
        register uintptr_t player_offset asm(offset_reg) = offsetof(HorseRaceState, player_is_racing); \
        asm ("" : "+l" (player_offset)); \
        register u8 * player_flag asm("r0") = reinterpret_cast<u8 *>(self) + player_offset; \
        asm ("" : "+l" (player_flag)); \
        *player_flag; \
    })
    #define READ_RACE_SHORT(ptr, field, reg_idx, reg_out) ({ \
        register uintptr_t field_offset asm(reg_idx) = offsetof(__typeof__(*ptr), field); \
        asm ("" : "+l" (field_offset)); \
        register int field_value asm(reg_out); \
        asm ("ldrsh %0, [%1, %2]" : "=l" (field_value) : "l" (ptr), "l" (field_offset) : "memory"); \
        field_value; \
    })
    #define RACE_LOW_SLOT(reg) ({ \
        register int current_slot asm(reg) = slot; \
        asm ("" : "+l" (current_slot)); \
        current_slot; \
    })
    // Advance every lane before computing the order used for finish and sprint decisions.
    do
    {
        if (++racer->stamina_timer >= STAMINA_REGEN_INTERVAL)
        {
            racer->stamina_timer = 0;
            register int stamina asm("r0");
            asm volatile ("ldrh %0, [%1, %2]\n\tadd %0, %3\n\tstrh %0, [%1, %2]\n\t"
                "lsl %0, %0, #16\n\tasr %0, %0, #16"
                : "=l" (stamina) : "l" (racer), "i" (offsetof(RaceEntrant, stamina)),
                  "i" (STAMINA_REGEN) : "cc", "memory");
            if (stamina >= STAMINA_LIMIT)
                racer->stamina = STAMINA_LIMIT;
        }

        switch (racer->sprint_phase)
        {
        case SPRINT_READY:
            goto sprint_ready;
        case SPRINT_START:
        {
            register int lower asm("r7") = slot;
            asm ("" : "+l" (lower));
            if (lower == 1) goto start_slot_one;
            if (lower > 1) goto start_upper;
            if (lower == 0) goto start_slot_zero;
            goto start_sprint;
start_upper:
            {
                register int upper asm("r0") = slot;
                asm ("" : "+l" (upper));
                if (upper == 2) goto start_slot_two;
                if (upper == 3) goto start_slot_three;
                goto start_sprint;
            }
start_slot_zero:
            {
                register int rate asm("r1") = offsetof(HorseRaceState, race_horse_animators[0]);
                asm ("" : "+l" (rate));
                register SpriteAnimator * first asm("r0") = reinterpret_cast<SpriteAnimator *>(
                    reinterpret_cast<u8 *>(self) + rate);
                asm ("" : "+l" (first) : "l" (rate));
                asm ("sub %0, %1" : "+l" (rate) : "i" (offsetof(HorseRaceState, race_horse_animators[0]) - SPRINT_ANIMATION_RATE_Q8) : "cc");
                first->speed_q8 = rate;
                PIN_ANIMATOR_RESULT(second, "r2", "r0", race_sprint_animators[0]);
                second->speed_q8 = rate;
                PIN_ANIMATOR(effect, "r3", race_effect_animators[0]);
                SetSpriteAnimatorClip(effect, 0);
                goto start_sprint;
            }
start_slot_one:
            {
                SET_PAIR_ANIMATORS("r7", race_horse_animators[1], "r2", race_sprint_animators[1], "r1", SPRINT_ANIMATION_RATE_Q8);
                PIN_ANIMATOR(effect, "r3", race_effect_animators[1]);
                SetSpriteAnimatorClip(effect, 0);
                goto start_sprint;
            }
start_slot_two:
            {
                SpriteAnimator * horse;
                if (PLAYER_RACING("r7") == 0)
                {
                    PIN_ANIMATOR(npc_horse, "r0", race_horse_animators[2]);
                    horse = npc_horse;
                }
                else
                {
                    PIN_ANIMATOR(player_horse, "r2", race_horse_animators[PLAYER_HORSE_ANIMATOR]);
                    horse = player_horse;
                }
                horse->speed_q8 = PIN_RACE_VALUE("r0", SPRINT_ANIMATION_RATE_Q8);
                PIN_ANIMATOR_RESULT(sprint, "r3", "r0", race_sprint_animators[2]);
                sprint->speed_q8 = PIN_RACE_VALUE("r1", SPRINT_ANIMATION_RATE_Q8);
                PIN_ANIMATOR(effect, "r7", race_effect_animators[2]);
                SetSpriteAnimatorClip(effect, 0);
                goto start_sprint;
            }
start_slot_three:
            {
                SET_PAIR_ANIMATORS("r1", race_horse_animators[3], "r2", race_sprint_animators[3], "r1", SPRINT_ANIMATION_RATE_Q8);
                PIN_ANIMATOR(effect, "r3", race_effect_animators[3]);
                SetSpriteAnimatorClip(effect, 0);
            }
start_sprint:
            ;
        }
            racer->phase_timer = SPRINT_PHASE_FRAMES;
            racer->sprint_phase = SPRINT_ACCELERATING;
            // Fall through on the first animation frame.
        case SPRINT_ACCELERATING:
        {
            register int speed asm("r1") = racer->speed;
            register int acceleration asm("r0") = racer->acceleration;
            asm ("add %0, %0, %1" : "+l" (speed) : "l" (acceleration) : "cc");
            racer->speed = speed;
            register int timer asm("r0");
            register int zero asm("r3");
            asm volatile ("ldrh %0, [%2, %3]\n\tsub %0, #1\n\tmov %1, #0\n\tstrh %0, [%2, %3]"
                : "=&l" (timer), "=l" (zero) : "l" (racer), "i" (offsetof(RaceEntrant, phase_timer))
                : "cc", "memory");
            register int limit asm("r0") = racer->sprint_speed;
            asm ("" : "+l" (limit));
            register int sprint_speed asm("r2") = limit;
            asm ("" : "+l" (sprint_speed));
            if (speed >= sprint_speed)
                racer->phase_timer = zero;
            register int phase_timer asm("r0") = READ_RACE_SHORT(racer, phase_timer, "r7", "r0");
            asm volatile ("cmp %0, #0\n\tbgt .Lrace_update_distance"
                : : "l" (phase_timer) : "cc", "memory");
            racer->sprint_phase = SPRINT_AT_SPEED;
            racer->speed = sprint_speed;
            racer->phase_timer = SPRINT_PHASE_FRAMES;
            goto update_distance;
        }
        case SPRINT_AT_SPEED:
            {
                register int timer asm("r0");
                asm volatile ("ldrh %0, [%1, %2]\n\tsub %0, #1\n\tstrh %0, [%1, %2]\n\t"
                    "lsl %0, %0, #16\n\tcmp %0, #0\n\tbgt .Lrace_update_distance"
                    : "=&l" (timer) : "l" (racer), "i" (offsetof(RaceEntrant, phase_timer))
                    : "cc", "memory");
            }
            racer->sprint_phase = SPRINT_DECELERATING;
            {
                racer->phase_timer = SPRINT_PHASE_FRAMES /
                    READ_RACE_SHORT(racer, stamina, "r0", "r1") + 30;
                racer->deceleration = (racer->sprint_speed - racer->base_speed) / racer->phase_timer;
            }
            goto update_distance;
        case SPRINT_DECELERATING:
        {
            register int speed asm("r1") = racer->speed;
            register int deceleration asm("r0") = racer->deceleration;
            asm ("sub %0, %0, %1" : "+l" (speed) : "l" (deceleration) : "cc");
            racer->speed = speed;
            register int timer asm("r0");
            register int zero asm("r3");
            asm volatile ("ldrh %0, [%2, %3]\n\tsub %0, #1\n\tmov %1, #0\n\tstrh %0, [%2, %3]"
                : "=&l" (timer), "=l" (zero) : "l" (racer), "i" (offsetof(RaceEntrant, phase_timer))
                : "cc", "memory");
            register int limit asm("r0") = racer->base_speed;
            asm ("" : "+l" (limit));
            register int base_speed asm("r2") = limit;
            asm ("" : "+l" (base_speed));
            if (speed <= base_speed)
                racer->phase_timer = zero;
            if (READ_RACE_SHORT(racer, phase_timer, "r7", "r0") <= 0)
            {
                racer->sprint_phase = zero;
                racer->speed = base_speed;
                racer->phase_timer = SPRINT_RETRY_FRAMES;
                SpriteAnimator * sprint_animator;
                register int restored_rate asm("r1");
                register int lower asm("r0") = slot;
                asm ("" : "+l" (lower));
                if (lower == 1) goto restore_slot_one;
                if (lower > 1) goto restore_upper;
                if (lower == 0) goto restore_slot_zero;
                goto update_distance;
restore_upper:
                {
                    register int upper asm("r1") = slot;
                    asm ("" : "+l" (upper));
                    if (upper == 2) goto restore_slot_two;
                    if (upper == 3) goto restore_slot_three;
                    goto update_distance;
                }
restore_slot_zero:
                {
                    PIN_ANIMATOR_RESULT(horse, "r2", "r0", race_horse_animators[0]);
                    restored_rate = NORMAL_ANIMATION_RATE_Q8;
                    asm ("" : "+l" (restored_rate));
                    horse->speed_q8 = restored_rate;
                    PIN_ANIMATOR_RESULT(sprint, "r3", "r0", race_sprint_animators[0]);
                    sprint_animator = sprint;
                    goto restore_sprint_animation;
                }
restore_slot_one:
                {
                    PIN_ANIMATOR_RESULT(horse, "r7", "r0", race_horse_animators[1]);
                    restored_rate = NORMAL_ANIMATION_RATE_Q8;
                    asm ("" : "+l" (restored_rate));
                    horse->speed_q8 = restored_rate;
                    PIN_ANIMATOR_RESULT(sprint, "r2", "r0", race_sprint_animators[1]);
                    sprint_animator = sprint;
                    goto restore_sprint_animation;
                }
restore_slot_two:
                {
                    SpriteAnimator * horse;
                    if (PLAYER_RACING("r3") == 0)
                    {
                        PIN_ANIMATOR(npc_horse, "r7", race_horse_animators[2]);
                        horse = npc_horse;
                    }
                    else
                    {
                        PIN_ANIMATOR(player_horse, "r0", race_horse_animators[PLAYER_HORSE_ANIMATOR]);
                        horse = player_horse;
                    }
                    horse->speed_q8 = PIN_RACE_VALUE("r0", NORMAL_ANIMATION_RATE_Q8);
                    PIN_ANIMATOR_RESULT(sprint, "r2", "r1", race_sprint_animators[2]);
                    sprint->speed_q8 = PIN_RACE_VALUE("r0", NORMAL_ANIMATION_RATE_Q8);
                    goto update_distance;
                }
restore_slot_three:
                {
                    PIN_ANIMATOR_RESULT(horse, "r3", "r0", race_horse_animators[3]);
                    restored_rate = NORMAL_ANIMATION_RATE_Q8;
                    asm ("" : "+l" (restored_rate));
                    horse->speed_q8 = restored_rate;
                    PIN_ANIMATOR_RESULT(sprint, "r7", "r0", race_sprint_animators[3]);
                    sprint_animator = sprint;
                }
restore_sprint_animation:
                sprint_animator->speed_q8 = restored_rate;
            }
            break;
        }
        default:
            goto update_distance;
        }
sprint_ready:
        asm volatile ("");
update_distance:
        asm volatile (".Lrace_update_distance:" : : : "memory");
        int remaining = racer->distance_left -= racer->speed;
        register int * distance_entry asm("r0");
        register int entrant_slot asm("r2") = RACE_LOW_SLOT("r2");
        asm ("lsl %0, %1, #2\n\tadd %0, %2" : "=&l" (distance_entry)
            : "l" (entrant_slot), "h" (distance) : "cc");
        *distance_entry = remaining;
        register int previous_camera asm("r3") = camera_distance;
        asm ("" : "+l" (previous_camera));
        if (previous_camera > remaining)
            camera_distance = remaining;

        register u32 position_u16 asm("r7");
        register int position_shifted asm("r0");
        register int position asm("r4");
        asm ("lsr %0, %3, #16\n\tlsl %1, %0, #16\n\tasr %2, %1, #16"
            : "=l" (position_u16), "=l" (position_shifted), "=l" (position) : "l" (remaining) : "cc");
        if (position >= 32)
        {
            register uintptr_t marker_offset asm("r2");
            register u32 index asm("r1");
            register i32 segment asm("r0") = position_shifted;
            register u32 slots_mask asm("r2");
            register u32 slot_index asm("r3");
            asm ("asr %0, %0, #21\n\tmov %2, #7\n\tmov %3, %4\n\tlsl %1, %3, #3\n\t"
                "and %0, %2\n\tadd %1, %1, %0\n\tlsl %1, %1, #2"
                : "+l" (segment), "=&l" (index), "=&l" (slots_mask), "=l" (slot_index)
                : "h" (slot) : "cc");
            asm ("mov %0, %1\n\tlsl %0, %0, #3"
                : "=l" (marker_offset) : "i" (offsetof(HorseRaceState, course_markers) >> 3) : "cc");
            register u8 * markers asm("r0") = reinterpret_cast<u8 *>(self) + marker_offset;
            asm ("" : "+l" (markers));
            register RaceCourseMarker * marker asm("r3") = reinterpret_cast<RaceCourseMarker *>(markers + index);
            asm ("" : "+l" (marker));
            register int zero asm("r0") = 0;
            asm ("" : "+l" (zero));
            register int stored_position asm("r1");
            asm ("ldrsh %0, [%1, %2]" : "=l" (stored_position) : "l" (marker), "l" (zero) : "memory");
            register u32 tilemask asm("r2") = 0xFFE0;
            asm ("" : "+l" (tilemask));
            asm ("and %0, %1" : "+l" (stored_position) : "l" (tilemask) : "cc");
            register u32 current_position asm("r0") = position;
            asm ("and %0, %1" : "+l" (current_position) : "l" (tilemask) : "cc");
            if (stored_position != static_cast<int>(current_position))
                marker->position = position_u16;
        }
        slot += PIN_RACE_VALUE("r1", 1);
        ++racer;
    } while (RACE_LOW_SLOT("r2") < ENTRANT_COUNT);

    // Rank by remaining distance, using the original strict comparison.
    slot = PIN_RACE_VALUE("r3", 0);
    register int * begin_distance = distance;
    asm ("" : "+l" (begin_distance));
    register int * next_distance asm("r5") = distance;
    asm ("" : "+l" (next_distance));
    do
    {
        register int rank asm("r9") = PIN_RACE_VALUE("r0", 0);
        asm ("" : "+h" (rank));
        register int other asm("r3") = 0;
        asm ("" : "+l" (other));
        register int * current asm("r4") = next_distance;
        asm ("" : "+l" (current));
        register int * compare asm("r2") = begin_distance;
        asm ("" : "+l" (compare));
        do
        {
            if (slot != other && *current > *compare)
                rank += PIN_RACE_VALUE("r1", 1);
            ++compare;
        } while (++other < ENTRANT_COUNT);
        register int low_rank asm("r2") = rank;
        asm ("" : "+l" (low_rank));
        register int * order_entry asm("r0") = &order[low_rank];
        asm ("" : "+l" (order_entry));
        *order_entry = RACE_LOW_SLOT("r3");
        ++next_distance;
        slot += PIN_RACE_VALUE("r0", 1);
    } while (RACE_LOW_SLOT("r1") < ENTRANT_COUNT);

    // Process finish crossings and new sprint requests in race order.
    register int stop_init asm("r2") = PIN_RACE_VALUE("r2", 0);
    register int stopped asm("r9") = stop_init;
    slot = stop_init;
    do
    {
        register int low_slot asm("r3") = RACE_LOW_SLOT("r3");
        register int order_index asm("r1");
        register int * order_base asm("r7");
        asm ("lsl %0, %2, #2; mov %1, sp" : "=l" (order_index), "=l" (order_base)
            : "l" (low_slot) : "cc");
        register int * order_entry asm("r4") = reinterpret_cast<int *>(
            reinterpret_cast<u8 *>(order_base) + order_index);
        asm ("" : "+l" (order_entry));
        register int racer_id asm("r3") = *order_entry;
        register uintptr_t stride asm("r0") = sizeof(RaceEntrant);
        asm ("mul %0, %1" : "+l" (stride) : "l" (racer_id) : "cc");
        register uintptr_t racer_offset asm("r2") = offsetof(HorseRaceState, racers);
        asm ("" : "+l" (racer_offset));
        register uintptr_t entry_offset asm("r0") = stride + racer_offset;
        asm ("" : "+l" (entry_offset));
        racer = reinterpret_cast<RaceEntrant *>(reinterpret_cast<u8 *>(self) + entry_offset);
        asm ("" : "+l" (racer));
        if (racer->finish_rank != NO_FINISH_RANK)
            goto finished_entrant;

        {
            register int finish_line asm("r0") = FINISH_DISTANCE;
            asm ("" : "+l" (finish_line));
            register int remaining asm("r7") = racer->distance_left;
            asm ("" : "+l" (remaining));
            if (remaining <= finish_line)
            {
                register uintptr_t finish_offset asm("r7") = offsetof(HorseRaceState, finish_count);
                asm ("" : "+l" (finish_offset));
                register i16 * finish_count asm("r2") = reinterpret_cast<i16 *>(
                    reinterpret_cast<u8 *>(self) + finish_offset);
                asm ("" : "+l" (finish_count));
                register int zero asm("r0") = 0;
                asm ("" : "+l" (zero));
                register uintptr_t place_offset asm("r1");
                asm ("ldrsh %0, [%1, %2]\n\tlsl %0, %0, #1" : "=l" (place_offset)
                    : "l" (finish_count), "l" (zero) : "cc", "memory");
                finish_offset += sizeof(i16);
                asm ("" : "+l" (finish_offset));
                register i16 * finish_order asm("r0") = reinterpret_cast<i16 *>(
                    reinterpret_cast<u8 *>(self) + finish_offset);
                asm ("" : "+l" (finish_order));
                *reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(finish_order) + place_offset) = racer_id;
                register int finish_rank asm("r1");
                register int next_rank asm("r0");
                asm volatile ("ldrh %0, [%2]\n\tadd %1, %0, #1\n\tstrh %1, [%2]\n\t"
                    "strb %0, [%3, %4]\n\tlsl %0, %0, #24"
                    : "=&l" (finish_rank), "=&l" (next_rank) : "l" (finish_count), "l" (racer),
                      "i" (offsetof(RaceEntrant, finish_rank)) : "cc", "memory");
                if (finish_rank != 0)
                    goto next_entrant;
                SpriteAnimator * winner;
                u32 clip;
                int winner_slot;
                asm ("ldr %0, [%1]" : "=l" (winner_slot) : "l" (order_entry) : "memory");
                switch (winner_slot)
                {
            case 0:
                    winner = &self->race_horse_animators[0].animator;
                    clip = VICTORY_CLIP_LANE_ONE;
                    break;
            case 1:
                    winner = &self->race_horse_animators[1].animator;
                    clip = VICTORY_CLIP_LANE_TWO;
                    break;
            case 2:
                    if (!self->player_is_racing)
                    {
                        winner = ({
                            PIN_ANIMATOR(winner_animator, "r3", race_horse_animators[2]);
                            winner_animator;
                        });
                        clip = VICTORY_CLIP_LANE_THREE;
                    }
                    else
                    {
                        winner = ({
                            PIN_ANIMATOR(winner_animator, "r7", race_horse_animators[PLAYER_HORSE_ANIMATOR]);
                            winner_animator;
                        });
                        clip = VICTORY_CLIP_PLAYER_HORSE;
                    }
                    break;
            case 3:
                    winner = ({
                            PIN_ANIMATOR(winner_animator, "r0", race_horse_animators[3]);
                            winner_animator;
                        });
                    clip = VICTORY_CLIP_LANE_FOUR;
                    break;
            default:
                    goto next_entrant;
                }
                SetSpriteAnimatorClip(winner, clip);
                winner->speed_q8 = NORMAL_ANIMATION_RATE_Q8;
                goto next_entrant;
            }

            if (PLAYER_RACING("r2") == 1 && racer_id == PLAYER_RACER_SLOT)
            {
                register u16 keys asm("r0") = HardwareContext_GetPressedKeys(reinterpret_cast<HardwareContext *>(self));
                register u32 pressed asm("r0");
                asm ("" : "=l" (pressed) : "l" (keys));
                register u32 a_button asm("r1") = BUTTON_A;
                asm ("" : "+l" (a_button));
                if ((pressed & a_button) == 0)
                    goto next_entrant;
                AudioPlayer_StartSong(&self->sprint_audio_player, SPRINT_SOUND);
                register int player_stamina asm("r0") = READ_RACE_SHORT(racer, stamina, "r3", "r0");
                asm volatile ("cmp %0, %1\n\tble .Lrace_next_entrant" : : "l" (player_stamina),
                    "i" (SPRINT_COST) : "cc", "memory");
                switch (racer->sprint_phase)
                {
            case SPRINT_READY:
            case SPRINT_DECELERATING:
                    racer->sprint_phase = SPRINT_START;
                    goto charge_stamina;
            case SPRINT_AT_SPEED:
                    racer->phase_timer += SPRINT_PHASE_FRAMES;
charge_stamina:
                    racer->stamina -= SPRINT_COST;
                    goto next_entrant;
            case SPRINT_START:
            case SPRINT_ACCELERATING:
                    goto unchanged_player_sprint;
            default:
                    goto next_entrant;
                }
            }
            else if (racer->sprint_phase == SPRINT_READY)
            {
                register int saved_timer asm("r2");
                register int phase_timer asm("r0");
                register int timer_offset asm("r3");
                asm ("ldrh %0, [%3, %4]; mov %2, %4; ldrsh %1, [%3, %2]"
                    : "=l" (saved_timer), "=l" (phase_timer), "=l" (timer_offset) : "l" (racer),
                      "i" (offsetof(RaceEntrant, phase_timer)) : "memory");
                if (phase_timer > 0)
                {
                    register int decremented asm("r0");
                    asm ("sub %0, %1, #1" : "=l" (decremented) : "l" (saved_timer) : "cc");
                    racer->phase_timer = decremented;
                }
                else
                {
                    register int chance asm("r2") = 0;
                    asm ("" : "+l" (chance));
                    int rank = RACE_LOW_SLOT("r0");
                    if (rank > 0)
                    {
                        register int gap asm("r1") = order_index;
                        register int cursor asm("r0");
                        register int * base asm("r3");
                        asm ("mov %2, sp; add %1, %2, %0; ldr %0, [%1]; lsl %0, %0, #2; add %0, %4; "
                            "mov %1, %3; sub %1, #1; lsl %1, %1, #2; add %1, sp; ldr %1, [%1]; "
                            "lsl %1, %1, #2; add %1, %4; ldr %0, [%0]; ldr %1, [%1]; sub %0, %0, %1"
                            : "+l" (gap), "=&l" (cursor), "=&l" (base)
                            : "h" (slot), "h" (distance) : "cc", "memory");
                        if (gap >= AI_SPRINT_MIN_GAP)
                            chance = 1;
                    }
                    if (remaining < AI_SPRINT_HOME_STRETCH)
                        chance += 2;
                    if (racer->sprint_chances[chance] > rand() % 100)
                    {
                        register int saved_stamina asm("r1");
                        register int stamina asm("r0");
                        register int stamina_offset asm("r7");
                        asm ("ldrh %0, [%3, %4]; mov %2, %4; ldrsh %1, [%3, %2]"
                            : "=l" (saved_stamina), "=l" (stamina), "=l" (stamina_offset) : "l" (racer),
                              "i" (offsetof(RaceEntrant, stamina)) : "memory");
                        if (stamina >= SPRINT_COST)
                        {
                            register int new_stamina asm("r0");
                            asm ("add %0, %1, #0; sub %0, %2" : "=l" (new_stamina)
                                : "l" (saved_stamina), "i" (SPRINT_COST) : "cc");
                            racer->stamina = new_stamina;
                            racer->sprint_phase = SPRINT_START;
                            if (!PLAYER_RACING("r1"))
                                AudioPlayer_StartSong(&self->sprint_audio_player, SPRINT_SOUND);
                        }
                    }
                    else
                        racer->phase_timer = SPRINT_RETRY_FRAMES;
                }
            }
            goto next_entrant;
        }

finished_entrant:
        if (racer->distance_left < 0)
        {
            racer->speed = 0;
            stopped += PIN_RACE_VALUE("r2", 1);
        }
unchanged_player_sprint:
        asm volatile ("");
next_entrant:
        asm volatile (".Lrace_next_entrant:" : : : "memory");
        slot += PIN_RACE_VALUE("r3", 1);
    } while (RACE_LOW_SLOT("r7") < ENTRANT_COUNT);

    if (PLAYER_RACING("r1") == 1)
    {
        register int distance_left asm("r2");
        asm ("mov %0, %1; ldr %0, [%0, %2]" : "=l" (distance_left)
            : "h" (distance), "i" (PLAYER_RACER_SLOT * sizeof(int)) : "memory");
        camera_distance = distance_left;
    }
    register uintptr_t camera_offset asm("r3") = offsetof(HorseRaceState, bg2_scroll_x);
    asm ("" : "+l" (camera_offset));
    register u16 * scroll asm("r2") = reinterpret_cast<u16 *>(reinterpret_cast<u8 *>(self) + camera_offset);
    asm ("" : "+l" (scroll));
    if (*scroll > CAMERA_END)
    {
        register int camera asm("r7") = camera_distance;
        asm ("asr %0, %0, #16" : "+l" (camera) : : "cc");
        camera_distance = camera;
        if (camera < *scroll + CAMERA_LEAD)
        {
            register u32 new_scroll asm("r0");
            asm ("add %0, %3, #0; sub %0, %2; strh %0, [%1]; lsl %0, %0, #16; lsr %0, %0, #16"
                : "=&l" (new_scroll) : "l" (scroll), "i" (CAMERA_LEAD), "l" (camera) : "cc", "memory");
            if (new_scroll < CAMERA_END)
                *scroll = CAMERA_END;
            register int current_scroll asm("r0") = *scroll;
            register int far_scroll asm("r1") = CAMERA_FAR_ORIGIN;
            asm ("sub %0, %1, %0; lsl %0, %0, #1; sub %1, %1, %0"
                : "+l" (current_scroll), "+l" (far_scroll) : : "cc");
            register uintptr_t far_offset asm("r3") = offsetof(HorseRaceState, bg1_scroll_x);
            asm ("" : "+l" (far_offset));
            register u16 * far_scroll_ptr asm("r0") = reinterpret_cast<u16 *>(reinterpret_cast<u8 *>(self) + far_offset);
            asm ("" : "+l" (far_scroll_ptr));
            *far_scroll_ptr = far_scroll;
            register int near_scroll asm("r1") = *scroll;
            asm ("" : "+l" (near_scroll));
            register uintptr_t near_offset asm("r7") = offsetof(HorseRaceState, bg3_scroll_x);
            asm ("" : "+l" (near_offset));
            register u16 * near_scroll_ptr asm("r0") = reinterpret_cast<u16 *>(reinterpret_cast<u8 *>(self) + near_offset);
            asm ("" : "+l" (near_scroll_ptr));
            *near_scroll_ptr = near_scroll;
        }
    }
#undef RACE_LOW_SLOT
#undef READ_RACE_SHORT
#undef PLAYER_RACING
#undef SET_PAIR_ANIMATORS
#undef PIN_ANIMATOR_RESULT
#undef PIN_RACE_VALUE
#undef PIN_ANIMATOR

    return stopped;
}
