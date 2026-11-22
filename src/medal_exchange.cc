#include "dma_queue.hh"
#include "farmer.hh"
#include "message_script.hh"
#include "sprite_animation.hh"
#include "sprite_rendering.hh"
#include "utility/vec2.hh"

#include <stdlib.h>
#include <string.h>

struct MedalExchangeGameState
{
    STRUCT_PAD(0, 0x1BD8);
    Farmer farmer;
    u8 reserved_after_farmer[0x218C - 0x1BD8 - sizeof(Farmer)];
    u32 jewel_count_word;
    STRUCT_PAD(0x2190, 0x21C7);
    u8 festival_flags;
    STRUCT_PAD(0x21C8, 0x2C48);
    union
    {
        u32 medal_save_word;
        struct
        {
            u8 preceding_bits[2];
            u8 prize_flags;
        } medal_save_bytes;
    };
};

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

struct MedalExchangeObjAllocation
{
    u32 context;
    u32 handle;
};

struct MessageWindow;
struct MusicPlayer;
struct AudioPlayer { MusicPlayer * player; };

struct MedalExchangeTextSubstitutions
{
    void const * vtable;
    int prize_id;
};

struct MedalExchange
{
    enum { PRIZE_ROWS = 10, MAX_GLYPHS = 48 };

    void * hardware_state;
    void const * vtable;
    MedalExchangeGameState * game_state;
    u32 dialogue_style;
    MessageWindow * message_window;
    AudioPlayer text_audio;
    AudioPlayer music;
    AudioPlayer sound_effects;
    u32 menu_graphics_data[12];
    u8 prize_graphics_data[0x30];
    SpriteAnimator cursor;
    SpriteFrameDrawOptions cursor_draw;
    u8 cursor_draw_flag;
    SpriteAnimator menu_icon;
    SpriteFrameDrawOptions menu_icon_draw;
    u8 menu_icon_draw_flag;
    SpriteAnimator prize_indicator;
    SpriteFrameDrawOptions prize_indicator_draw;
    u8 prize_indicator_draw_flag;
    MedalExchangeObjAllocation glyph_tiles;
    MedalExchangeObjAllocation glyph_palette;
    MedalExchangeObjAllocation cursor_tiles;
    MedalExchangeObjAllocation cursor_palette;
    MedalExchangeObjAllocation prize_tiles;
    MedalExchangeObjAllocation prize_palette;
    u16 scroll_y;
    MedalExchangeGlyph glyphs[MAX_GLYPHS];
    i16 glyph_count;
    u16 prize_name_tilemap[32][32];
    int medal_balance;
    i16 selected_row;
    i16 first_visible_row;
    i16 selected_prize_id;
    i16 exchange_phase;
    i16 dialogue_phase;
    i16 declined_choice;
    u8 prize_indices[PRIZE_ROWS];
    STRUCT_PAD(0xAEA, 0xAEC);
    MedalExchangeTextSubstitutions text_substitutions;
};

typedef char MedalExchangeSelectionOffsetsMatch[
    offsetof(MedalExchange, selected_row) == 0xAD4 &&
    offsetof(MedalExchange, first_visible_row) == 0xAD6 &&
    offsetof(MedalExchange, selected_prize_id) == 0xAD8 &&
    offsetof(MedalExchange, prize_indices) == 0xAE0 &&
    offsetof(MedalExchange, text_substitutions) +
        offsetof(MedalExchangeTextSubstitutions, prize_id) == 0xAF0 ? 1 : -1];


struct TileResources { void * handles; };
struct ResourcePool { void * storage; };

EC void * __builtin_new(size_t);
EC void InitMedalExchangeHardware(MedalExchange *, Vec2 const *) asm("HardwareContext_InitializeWithKeyRepeat");
EC void InitTileResources(TileResources *) asm("TilePool_Initialize");
EC void DestroyTileResources(TileResources *, int) asm("TilePool_Destroy");
EC ResourcePool * ResourcePool_Initialize(ResourcePool *);
EC void DestroyResourcePool(ResourcePool *, int) asm("ResourcePool_Destroy");
EC MessageWindow * CreateMessageWindow(MessageWindow *, u32, u32, u32, u32, TileResources *, ResourcePool *);
EC AudioPlayer * AudioPlayer_Initialize(AudioPlayer *, u8);
EC SpriteAnimationData * InitSpriteAnimationData(SpriteAnimationData *, void const *);
EC void InitMedalExchangeTiles(MedalExchangeObjAllocation *) asm("TileHeapOwner_Initialize");
EC u32 AllocateMedalExchangeTiles(MedalExchangeObjAllocation *, u32) asm("TileHandle_Allocate");
EC void ResourceHandles_Initialize(MedalExchangeObjAllocation *);
EC u32 AllocateMedalExchangePalette(MedalExchangeObjAllocation *) asm("ResourceHandle_Allocate");
EC char const gMedalExchangeScreenVTable[] asm("vtable_unk_080E799C");
EC char const gMedalExchangeTextSubstitutionsVTable[];
EC u8 const gMedalExchangeMenuAnimation[] asm("gUnk_08527094");
EC u8 const gMedalExchangeSelectionAnimation[] asm("gUnk_08728208");

#define INIT_MEDAL_DRAW_OPTIONS(field, byte_reg) \
    register SpriteFrameDrawOptions * options asm("r0") = &self->field; \
    asm ("" : "+l" (options)); \
    register u16 halfword_zero asm("r5"); \
    asm ("" : "=l" (halfword_zero) : "0" (zero)); \
    options->offset_x = halfword_zero; \
    options->offset_y = halfword_zero; \
    options->palette_base = halfword_zero; \
    options->tile_base = halfword_zero; \
    options->priority = halfword_zero; \
    options->object_mode = zero; \
    register u8 flag_zero asm(byte_reg); \
    asm ("mov %0, %1" : "=l" (flag_zero) : "r" (byte_zero)); \
    options->flip = flag_zero; \
    options->affine_index = flag_zero; \
    options->affine_mode = zero; \
    options->mosaic = flag_zero;

#define MEDAL_INIT_FIELD(reg, type, offset, base) ({ \
    register u32 field_offset asm(reg) = offset; \
    asm ("" : "+l" (field_offset)); \
    reinterpret_cast<type *>(reinterpret_cast<u8 *>(base) + field_offset); \
})

#define INIT_MEDAL_ANIMATOR(animator, graphics, clip_id) do { \
    register SpriteAnimator * cursor_slot asm("r0") = animator; \
    register SpriteAnimationSource const * source asm("r1") = reinterpret_cast<SpriteAnimationSource *>(graphics); \
    register u32 clip asm("r2") = clip_id; \
    asm ("" : "+l" (cursor_slot), "+l" (source), "+l" (clip)); \
    register int speed asm("r3") = 256; \
    asm ("" : "+l" (speed)); \
    InitSpriteAnimator(cursor_slot, source, clip, speed); \
} while (0)

#define ALLOCATE_MEDAL_TILES(field, count, reg) do { \
    MedalExchangeObjAllocation * tiles = MEDAL_INIT_FIELD(reg, MedalExchangeObjAllocation, offsetof(MedalExchange, field), self); \
    InitMedalExchangeTiles(tiles); \
    tiles->handle = AllocateMedalExchangeTiles(tiles, count); \
} while (0)

#define ALLOCATE_MEDAL_PALETTE(field, reg) do { \
    MedalExchangeObjAllocation * palette = MEDAL_INIT_FIELD(reg, MedalExchangeObjAllocation, offsetof(MedalExchange, field), self); \
    ResourceHandles_Initialize(palette); \
    palette->handle = AllocateMedalExchangePalette(palette); \
} while (0)

EC SECTION(".text.f3.0805CF70") MedalExchange * InitMedalExchangeScreen(
    MedalExchange * self, MedalExchangeGameState * game_state)
{
    enum { KEY_REPEAT_DELAY = 30, KEY_REPEAT_PERIOD = 1, MAX_MEDAL_BALANCE = 99999 };
    struct
    {
        TileResources graphics;
        ResourcePool font;
        Vec2 key_repeat;
    } scratch;
    register Vec2 * key_repeat asm("r1") = &scratch.key_repeat;
    register u32 key_delay asm("r0") = KEY_REPEAT_DELAY;
    register u32 key_period asm("r2") = KEY_REPEAT_PERIOD;
    asm ("" : "+l" (key_repeat), "+l" (key_delay), "+l" (key_period));
    register u32 low_zero asm("r3") = 0;
    asm ("" : "+l" (low_zero));
    register u32 byte_zero asm("r8") = low_zero;
    register u32 zero asm("r5") = 0;
    asm ("" : "+r" (byte_zero), "+l" (zero));
    key_repeat->x = key_delay;
    key_repeat->y = key_period;
    InitMedalExchangeHardware(self, key_repeat);
    self->vtable = gMedalExchangeScreenVTable;
    self->game_state = game_state;
    self->dialogue_style = zero;
    register MessageWindow * window_storage asm("r6") = static_cast<MessageWindow *>(__builtin_new(4));
    asm ("" : "+l" (window_storage));
    InitTileResources(&scratch.graphics);
    ResourcePool_Initialize(&scratch.font);
    self->message_window = CreateMessageWindow(window_storage, 0, 31, 0, self->dialogue_style,
        &scratch.graphics, &scratch.font);
    DestroyResourcePool(&scratch.font, 2);
    DestroyTileResources(&scratch.graphics, 2);
    AudioPlayer_Initialize(&self->text_audio, 1);
    AudioPlayer_Initialize(&self->music, 0);
    AudioPlayer_Initialize(&self->sound_effects, 2);

    register SpriteAnimationData * menu_graphics asm("r4") = reinterpret_cast<SpriteAnimationData *>(self->menu_graphics_data);
    asm ("" : "+l" (menu_graphics));
    InitSpriteAnimationData(menu_graphics, gMedalExchangeMenuAnimation);
    register SpriteAnimationData * prize_graphics asm("r6") = reinterpret_cast<SpriteAnimationData *>(self->prize_graphics_data);
    asm ("" : "+l" (prize_graphics));
    InitSpriteAnimationData(prize_graphics, gMedalExchangeSelectionAnimation);
    INIT_MEDAL_ANIMATOR(&self->cursor, menu_graphics, 0);
    {
        INIT_MEDAL_DRAW_OPTIONS(cursor_draw, "r1")
        register u8 * draw_flag asm("r0") = reinterpret_cast<u8 *>(options) +
            (offsetof(MedalExchange, cursor_draw_flag) - offsetof(MedalExchange, cursor_draw));
        asm ("" : "+l" (draw_flag));
        *draw_flag = flag_zero;
        INIT_MEDAL_ANIMATOR(reinterpret_cast<SpriteAnimator *>(draw_flag +
            (offsetof(MedalExchange, menu_icon) - offsetof(MedalExchange, cursor_draw_flag))), menu_graphics, 1);
    }
    {
        INIT_MEDAL_DRAW_OPTIONS(menu_icon_draw, "r3")
        register u8 * draw_flag asm("r0") = reinterpret_cast<u8 *>(options) +
            (offsetof(MedalExchange, menu_icon_draw_flag) - offsetof(MedalExchange, menu_icon_draw));
        asm ("" : "+l" (draw_flag));
        *draw_flag = flag_zero;
        INIT_MEDAL_ANIMATOR(reinterpret_cast<SpriteAnimator *>(draw_flag +
            (offsetof(MedalExchange, prize_indicator) - offsetof(MedalExchange, menu_icon_draw_flag))), prize_graphics, 0);
    }
    {
        INIT_MEDAL_DRAW_OPTIONS(prize_indicator_draw, "r1")
        register u8 * draw_flag asm("r0") = MEDAL_INIT_FIELD("r3", u8, offsetof(MedalExchange, prize_indicator_draw_flag), self);
        asm ("" : "+l" (draw_flag));
        *draw_flag = flag_zero;
    }
    ALLOCATE_MEDAL_TILES(glyph_tiles, 7, "r0");
    ALLOCATE_MEDAL_PALETTE(glyph_palette, "r1");
    ALLOCATE_MEDAL_TILES(cursor_tiles, 4, "r3");
    ALLOCATE_MEDAL_PALETTE(cursor_palette, "r0");
    ALLOCATE_MEDAL_TILES(prize_tiles, 2, "r1");
    ALLOCATE_MEDAL_PALETTE(prize_palette, "r3");
    register MedalExchangeTextSubstitutions * substitutions asm("r1") = &self->text_substitutions;
    asm ("" : "+l" (substitutions));
    substitutions->vtable = gMedalExchangeTextSubstitutionsVTable;
    substitutions->prize_id = zero;
    register int * balance asm("r2") = MEDAL_INIT_FIELD("r1", int, offsetof(MedalExchange, medal_balance), self);
    asm ("" : "+l" (balance));
    register MedalExchangeGameState * game asm("r0") = self->game_state;
    asm ("" : "+l" (game));
    u32 const * saved_medals = MEDAL_INIT_FIELD("r3", u32, offsetof(MedalExchangeGameState, medal_save_word), game);
    *balance = (*saved_medals >> 2) & 0x1FFFF;
    if (*balance > MAX_MEDAL_BALANCE)
        *balance = MAX_MEDAL_BALANCE;
    register u16 * scroll asm("r0") = MEDAL_INIT_FIELD("r1", u16, offsetof(MedalExchange, scroll_y), self);
    asm ("" : "+l" (scroll));
    *scroll = zero;
    return self;
}

#undef ALLOCATE_MEDAL_PALETTE
#undef ALLOCATE_MEDAL_TILES
#undef INIT_MEDAL_DRAW_OPTIONS
#undef INIT_MEDAL_ANIMATOR
#undef MEDAL_INIT_FIELD

EC MedalPrize const gMedalExchangePrizes[];

EC SECTION(".text.medal_exchange.080E1844") MessageScriptReplacement * ResolveMedalExchangePrizeName(
    MessageScriptReplacement * output, MedalExchangeTextSubstitutions const * substitutions, u32 placeholder)
{
    enum { PRIZE_NAME_PLACEHOLDER = 0xFF };
    register MessageScriptReplacement * result asm("r6") = output;
    asm ("" : "+l" (result));
    char text[sizeof(output->text)];
    if (placeholder == PRIZE_NAME_PLACEHOLDER)
    {
        char const * name = gMedalExchangePrizes[substitutions->prize_id].name;
        size_t length = strlen(name);
        if (length > sizeof(text) - 1)
            length = sizeof(text) - 1;
        memcpy(text, name, length);
        text[length] = '\0';
        result->found = true;
        strcpy(result->text, text);
    }
    else
    {
        text[0] = '\0';
        result->found = false;
        strcpy(result->text, text);
    }
    return result;
}

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

struct MedalExchangeFrameSource;
template <typename T> struct MedalExchangeFrameSlice
{
    T const * data;
    u16 length;
};
struct MedalExchangeFrameView
{
    MedalExchangeFrameSlice<SpriteFrameObject> objects;
    MedalExchangeFrameSlice<u8> tiles;
    MedalExchangeFrameSlice<u16> palette;
    MedalExchangeFrameSlice<SpriteFrameExtra> extras;
};
struct MedalExchangeFrameSourceVTable
{
    STRUCT_PAD(0, 0x10);
    void (*get_frame)(MedalExchangeFrameView *, MedalExchangeFrameSource const *, u32);
};
struct MedalExchangeFrameSource
{
    MedalExchangeFrameSourceVTable const * vtable;
};

struct PrizeNameTileSize
{
    u16 width, height;
};

EC void FillPrizeNameTiles(PrizeNameTileSize, void *, u32) asm("FillMessageTiles");
EC void DrawPrizeNameTiles(PrizeNameTileSize, void *, u32, u32, char const *, u32, u32) asm("DrawMessageTileStringWithColors");
EC void Dma3_Copy(void const *, void *, u32);
EC void Dma3_Fill(u32, void *, u32);
EC u32 ResolveMedalObjPalette(MedalExchangeObjAllocation *, u32) asm("ResourceHandle_GetIndex");
EC u32 ResolveMedalObjTile(MedalExchangeObjAllocation *, u32) asm("TileHandle_GetTileIndex");
EC void ReadPrizeFrame(MedalExchangeFrameView *, SpriteAnimationData const *, u32) asm("GetSpriteAnimationDataFrame");
EC void * CopyPrizeFrame(void *, void const *, size_t) asm("memcpy");

struct MedalExchangeDisplayRegisters
{
    u16 control;
    STRUCT_PAD(0x02, 0x0A);
    u16 bg1_control, bg2_control, bg3_control;
    STRUCT_PAD(0x10, 0x14);
    u16 bg1_scroll_x, bg1_scroll_y;
    u16 bg2_scroll_x, bg2_scroll_y;
    u16 bg3_scroll_x, bg3_scroll_y;
    STRUCT_PAD(0x20, 0x50);
    u16 blend_control;
};

EC MedalExchangeDisplayRegisters * GetMedalExchangeDisplayRegisters(MedalExchange *) asm("HardwareContext_GetDisplayRegisters");
EC void DrawMessageFrame(u16 *, u32, u32, u32, u32, u32, u32);
EC u8 const * const gMedalExchangeMessageFrameTiles asm("gUnk_080F9F70");
EC u32 const gMedalExchangeMessageFrameTileBytes asm("gUnk_080F9F74");
EC u16 const * const gMedalExchangeMessageFramePalette asm("gUnk_080F9F78");
EC u16 const gMedalExchangeGlyphCodes[] asm("gUnk_080FB1A6");

// The original register setters copy through a halfword scratch slot.
#define SET_MEDAL_SCROLL(field, zero, zero_register) \
    do { \
        register MedalExchangeDisplayRegisters * display asm("r0") = GetMedalExchangeDisplayRegisters(exchange); \
        register u16 * scratch asm("r1") = &work.display_value; \
        asm ("" : "+r" (scratch), "+r" (display)); \
        register u16 value asm(zero_register) = zero; \
        asm volatile ("strh %0, [%1]" : : "l" (value), "l" (scratch) : "memory"); \
        display->field = value; \
    } while (0)

#define SET_MEDAL_BG_CONTROL(field, flags) \
    do { \
        register MedalExchangeDisplayRegisters * display asm("r0") = GetMedalExchangeDisplayRegisters(exchange); \
        register u16 * scratch asm("r2") = &work.display_value; \
        asm ("" : "+r" (scratch), "+r" (display)); \
        register u32 bg_flags asm("r3") = flags; \
        asm ("" : "+r" (bg_flags)); \
        register u16 value asm("r1") = bg_flags; \
        asm ("" : "+r" (value)); \
        asm volatile ("strh %0, [%1]" : : "l" (value), "l" (scratch) : "memory"); \
        display->field = value; \
    } while (0)

EC SECTION(".text.f3.0805D9C0") void LoadMedalExchangeScreenGraphics(MedalExchange * self)
{
    enum
    {
        BG_VRAM = 0x06000000,
        OBJ_VRAM = 0x06010000,
        BG_PALETTE = 0x05000000,
        OBJ_PALETTE = 0x05000200,
        PRIZE_NAME_PALETTE = BG_PALETTE + 0x40,
        BLANK_TILE = 0x06007FE0,
        BASE_MAP = 0x0600E000,
        FRAME_MAP = 0x0600E800,
        PRIZE_NAME_MAP = 0x0600F000,
        MAP_BYTES = 0x800,
        TILE_BYTES = 32,
        FRAME_WIDTH = 32,
        GLYPH_COUNT = 24,
        GLYPH_WIDTH = 1,
        GLYPH_HEIGHT = 2,
        GLYPH_TRANSPARENT = 0x03E0,
        GLYPH_SHADOW = 0x4218,
        DISPLAY_LAYERS = 0x1E40,
        BASE_BG_CONTROL = 0x5C43,
        FRAME_BG_CONTROL = 0x1D41,
        PRIZE_NAMES_BG_CONTROL = 0x5E42,
        BASE_MAP_TILE_PAIR = 0x00030003,
    };
    // Keep the scratch block after the outgoing arguments of DrawMessageFrame.
    enum { STACKED_TEXT_ARGS = 3 * sizeof(u32) };
    struct
    {
        u16 display_value;
        STRUCT_PAD(0x02, 0x08);
        u32 glyph_tiles[GLYPH_WIDTH * GLYPH_HEIGHT * TILE_BYTES / sizeof(u32)];
        PrizeNameTileSize size;
    } work;

    register MedalExchange * exchange asm("r6") = self;
    register u16 scroll_zero asm("r4");
    register int glyph asm("r5");
    {
        register MedalExchangeDisplayRegisters * display asm("r0") = GetMedalExchangeDisplayRegisters(exchange);
        register u16 * scratch asm("r1") = &work.display_value;
        asm ("" : "+r" (scratch), "+r" (display));
        asm ("mov %0, #0" : "=l" (scroll_zero) : : "cc");
        register u16 value asm("r4") = scroll_zero;
        asm volatile ("strh %0, [%1]" : : "l" (value), "l" (scratch) : "memory");
        display->bg1_scroll_x = value;
    }
    {
        register MedalExchangeDisplayRegisters * display asm("r0") = GetMedalExchangeDisplayRegisters(exchange);
        register u16 * scratch asm("r2") = &work.display_value;
        asm ("" : "+r" (scratch), "+r" (display));
        register unsigned scroll_offset asm("r3") = offsetof(MedalExchange, scroll_y);
        asm ("" : "+r" (scroll_offset));
        register uintptr_t scroll_address asm("r1");
        asm ("add %0, %1, %2" : "=l" (scroll_address) : "l" (exchange), "l" (scroll_offset) : "cc");
        register u16 value asm("r1") = *reinterpret_cast<u16 const *>(scroll_address);
        asm volatile ("strh %0, [%1]" : : "l" (value), "l" (scratch) : "memory");
        display->bg1_scroll_y = value;
    }
    SET_MEDAL_SCROLL(bg2_scroll_x, scroll_zero, "r4");
    SET_MEDAL_SCROLL(bg2_scroll_y, scroll_zero, "r4");
    SET_MEDAL_SCROLL(bg3_scroll_x, scroll_zero, "r4");
    SET_MEDAL_SCROLL(bg3_scroll_y, scroll_zero, "r4");

    Dma3_Fill(0, reinterpret_cast<void *>(BLANK_TILE), TILE_BYTES);
    register u32 empty_map asm("r4") = ~0u;
    asm ("" : "+r" (empty_map));
    register void * base_map_low asm("r0") = reinterpret_cast<void *>(BASE_MAP);
    asm ("" : "+r" (base_map_low));
    register void * base_map asm("r9") = base_map_low;
    asm ("" : "+r" (base_map));
    register u32 map_bytes_low asm("r1") = MAP_BYTES;
    asm ("" : "+r" (map_bytes_low));
    register u32 map_bytes asm("r8") = map_bytes_low;
    asm ("" : "+r" (map_bytes));
    Dma3_Fill(empty_map, base_map, map_bytes);
    register u16 * frame_map_low asm("r2") = reinterpret_cast<u16 *>(FRAME_MAP);
    asm ("" : "+r" (frame_map_low));
    register u16 * frame_map asm("r10") = frame_map_low;
    asm ("" : "+r" (frame_map));
    Dma3_Fill(empty_map, frame_map, map_bytes);
    Dma3_Fill(empty_map, reinterpret_cast<void *>(PRIZE_NAME_MAP), map_bytes);
    {
        register MedalExchangeDisplayRegisters * display asm("r0") = GetMedalExchangeDisplayRegisters(exchange);
        register u16 * scratch asm("r1") = &work.display_value;
        asm ("" : "+r" (scratch), "+r" (display));
        register u32 register_value asm("r3") = DISPLAY_LAYERS;
        asm ("" : "+r" (register_value));
        register u16 value asm("r2") = register_value;
        asm ("" : "+r" (value));
        asm volatile ("strh %0, [%1]" : : "l" (value), "l" (scratch) : "memory");
        asm ("mov %0, #0" : "=l" (glyph) : : "cc");
        display->control = value;
    }
    SET_MEDAL_BG_CONTROL(bg3_control, BASE_BG_CONTROL);
    SET_MEDAL_BG_CONTROL(bg2_control, FRAME_BG_CONTROL);
    SET_MEDAL_BG_CONTROL(bg1_control, PRIZE_NAMES_BG_CONTROL);
    Dma3_Copy(gMedalExchangeMessageFrameTiles, reinterpret_cast<void *>(BG_VRAM), gMedalExchangeMessageFrameTileBytes);
    Dma3_Fill(BASE_MAP_TILE_PAIR, base_map, map_bytes);

    SET_MEDAL_SCROLL(bg3_scroll_x, glyph, "r5");
    SET_MEDAL_SCROLL(bg3_scroll_y, glyph, "r5");
    DrawMessageFrame(frame_map, 0, 0, FRAME_WIDTH, 3, glyph, glyph);
    SET_MEDAL_SCROLL(bg2_scroll_x, glyph, "r5");
    SET_MEDAL_SCROLL(bg2_scroll_y, glyph, "r5");
    SET_MEDAL_SCROLL(bg1_scroll_x, glyph, "r5");
    SET_MEDAL_SCROLL(bg1_scroll_y, glyph, "r5");
    Dma3_Copy(gMedalExchangeMessageFramePalette, reinterpret_cast<void *>(BG_PALETTE), TILE_BYTES);

    volatile u16 * prize_palette = reinterpret_cast<volatile u16 *>(PRIZE_NAME_PALETTE);
    prize_palette[0] = GLYPH_TRANSPARENT;
    asm ("" : "+r" (prize_palette));
    ++prize_palette;
    *prize_palette = glyph;
    asm ("" : "+r" (prize_palette));
    ++prize_palette;
    asm ("" : "+r" (prize_palette));
    register u32 shadow_low asm("r2") = GLYPH_SHADOW;
    asm ("" : "+r" (shadow_low));
    register u16 glyph_shadow asm("r9") = shadow_low;
    asm ("" : "+r" (glyph_shadow));
    *prize_palette = shadow_low;
    register u32 glyph_palette_offset asm("r3") = offsetof(MedalExchange, glyph_palette);
    asm ("" : "+r" (glyph_palette_offset));
    MedalExchangeObjAllocation * glyph_palette = reinterpret_cast<MedalExchangeObjAllocation *>(reinterpret_cast<u8 *>(exchange) + glyph_palette_offset);
    u32 palette_slot = ResolveMedalObjPalette(glyph_palette, glyph_palette->handle);
    register uintptr_t transparent_address asm("r0") = OBJ_PALETTE + palette_slot * TILE_BYTES;
    asm ("" : "+r" (transparent_address));
    register u16 transparent asm("r2") = GLYPH_TRANSPARENT;
    asm ("" : "+r" (transparent));
    *reinterpret_cast<volatile u16 *>(transparent_address) = transparent;
    palette_slot = ResolveMedalObjPalette(glyph_palette, glyph_palette->handle);
    register uintptr_t palette_bytes asm("r0") = palette_slot * TILE_BYTES;
    asm ("" : "+r" (palette_bytes));
    register uintptr_t ink_entry asm("r3") = OBJ_PALETTE + sizeof(u16);
    asm ("" : "+r" (ink_entry));
    asm ("add %0, %0, %1" : "+l" (palette_bytes) : "l" (ink_entry) : "cc");
    *reinterpret_cast<volatile u16 *>(palette_bytes) = glyph;
    palette_slot = ResolveMedalObjPalette(glyph_palette, glyph_palette->handle);
    palette_bytes = palette_slot * TILE_BYTES;
    asm ("" : "+r" (palette_bytes));
    register uintptr_t shadow_entry asm("r1") = OBJ_PALETTE + 2 * sizeof(u16);
    asm ("" : "+r" (shadow_entry));
    asm ("add %0, %0, %1" : "+l" (palette_bytes) : "l" (shadow_entry) : "cc");
    register u16 shadow asm("r2") = glyph_shadow;
    asm ("" : "+r" (shadow));
    *reinterpret_cast<volatile u16 *>(palette_bytes) = shadow;

    register PrizeNameTileSize * size_low asm("r3");
    asm ("add %0, sp, %1" : "=l" (size_low) : "i" (STACKED_TEXT_ARGS + offsetof(__typeof__(work), size)));
    register PrizeNameTileSize * size asm("r9") = size_low;
    asm ("" : "+r" (size));
    register u32 * pixels_low asm("r0");
    asm ("add %0, sp, %1" : "=l" (pixels_low) : "i" (STACKED_TEXT_ARGS + offsetof(__typeof__(work), glyph_tiles)));
    register u32 * pixels asm("r8") = pixels_low;
    asm ("" : "+r" (pixels));
    u32 width = GLYPH_WIDTH;
    asm ("" : "+r" (width));
    register u32 height_low asm("r1") = GLYPH_HEIGHT;
    asm ("" : "+r" (height_low));
    register u32 height asm("r10") = height_low;
    asm ("" : "+r" (height));
    register u32 glyph_tiles_offset asm("r2") = offsetof(MedalExchange, glyph_tiles);
    asm ("" : "+r" (glyph_tiles_offset));
    register MedalExchangeObjAllocation * glyph_tiles asm("r6") =
        reinterpret_cast<MedalExchangeObjAllocation *>(reinterpret_cast<u8 *>(exchange) + glyph_tiles_offset);
    asm ("" : "+r" (glyph_tiles));
    do
    {
        register u16 const * codes asm("r0") = gMedalExchangeGlyphCodes;
        asm volatile ("" : "+r" (codes));
        register u32 code_offset asm("r4") = glyph * sizeof(u16);
        asm ("" : "+r" (code_offset));
        asm ("add %0, %1, %0" : "+l" (codes) : "l" (code_offset) : "cc");
        register u32 code asm("r1") = *codes;
        register PrizeNameTileSize * size_width asm("r0");
        asm volatile ("add %0, sp, %2\n\tstrh %1, [%0]"
            : "=&l" (size_width)
            : "l" (width), "i" (STACKED_TEXT_ARGS + offsetof(__typeof__(work), size)) : "memory");
        asm ("" : "+r" (code));
        register u32 height_value asm("r0") = height;
        asm ("" : "+r" (height_value));
        register PrizeNameTileSize * size_height asm("r3") = size;
        asm volatile ("strh %0, [%1, #2]" : : "l" (height_value), "l" (size_height) : "memory");

        // Match the agbcc stack argument order for this glyph draw.
        asm volatile (
            "ldr r0, [sp, %2]\n\t"
            "str %0, [sp]\n\t"
            "mov %0, #0\n\t"
            "str %0, [sp, #4]\n\t"
            "str %1, [sp, #8]\n\t"
            "add r1, sp, %3\n\t"
            "mov r2, #0\n\t"
            "mov r3, #0\n\t"
            "bl DrawMessageGlyphWithColors"
            : "+l" (code)
            : "l" (width),
              "i" (STACKED_TEXT_ARGS + offsetof(__typeof__(work), size)),
              "i" (STACKED_TEXT_ARGS + offsetof(__typeof__(work), glyph_tiles))
            : "r0", "r2", "r3", "r12", "lr", "cc", "memory");
        register u32 tile asm("r1") = ResolveMedalObjTile(glyph_tiles, glyph_tiles->handle);
        asm ("" : "+r" (tile));
        tile += code_offset;
        register uintptr_t tile_bytes asm("r1") = tile * TILE_BYTES;
        asm ("" : "+r" (tile_bytes));
        register uintptr_t obj_vram asm("r2") = OBJ_VRAM;
        asm ("" : "+r" (obj_vram));
        Dma3_Copy(pixels, reinterpret_cast<void *>(tile_bytes + obj_vram), sizeof(work.glyph_tiles));
        ++glyph;
    } while (glyph < GLYPH_COUNT);
}

#undef SET_MEDAL_SCROLL
#undef SET_MEDAL_BG_CONTROL

struct HardwareContextState;
struct OamBuffer;
struct FrameTaskList;
struct VBlankTaskList;

EC HardwareContextState * HardwareContext_GetState(MedalExchange *);
EC OamBuffer * HardwareContext_GetOamBuffer(MedalExchange *);
EC FrameTaskList * HardwareContext_GetFrameTasks(MedalExchange *);
EC VBlankTaskList * HardwareContext_GetVBlankTasks(MedalExchange *);
EC u32 UpdateMessageWindow(MessageWindow *, HardwareContextState *, OamBuffer *, MedalExchangeDisplayRegisters *,
    FrameTaskList *, VBlankTaskList *, AudioPlayer *);
EC void OpenMessageWindow(MessageWindow *);
EC void CloseMessageWindow(MessageWindow *);
EC u32 GetMessageWindowSelection(MessageWindow *);
EC void SetMessageWindowTextSubstitutions(MessageWindow *, char const *, void const *);
EC void ConfigureMessageWindowPromptChoices(MessageWindow *, char const *, char const *, char const *,
    char const *, char const *, void const *);

EC char const gAskToExchangeMedals[];
EC char const gMedalDialogueYes[];
EC char const gMedalDialogueNo[];
EC char const gMedalExchangeFarewell[];
EC char const gMedalExchangeKeepBrowsing[];
EC char const gConfirmMedalExchangePrize[];
EC char const gMedalExchangeLeaving[];
EC char const gMedalExchangeNotEnoughMedals[];

#define MEDAL_FIELD_PTR(name, field, offset_register, pointer_register) \
    register u32 name##_offset asm(offset_register) = offsetof(MedalExchange, field); \
    asm ("" : "+r" (name##_offset)); \
    register __typeof__(&self->field) name asm(pointer_register) = \
        reinterpret_cast<__typeof__(&self->field)>(reinterpret_cast<u8 *>(self) + name##_offset); \
    asm ("" : "+r" (name));

#define MEDAL_PROMPT(message, offset_register) \
    do { \
        register MessageWindow * prompt_window asm("r0") = self->message_window; \
        register char const * prompt asm("r1") = message; \
        register char const * yes asm("r2") = gMedalDialogueYes; \
        register char const * no asm("r3") = gMedalDialogueNo; \
        asm ("" : "+r" (prompt_window), "+r" (prompt), "+r" (yes), "+r" (no)); \
        MEDAL_FIELD_PTR(substitutions, text_substitutions, offset_register, "r5"); \
        register char const * none asm("r4") = 0; \
        asm ("" : "+r" (none)); \
        ConfigureMessageWindowPromptChoices(prompt_window, prompt, yes, no, none, none, substitutions); \
    } while (0)

#define MEDAL_TEXT(message, offset_register) \
    do { \
        register MessageWindow * text_window asm("r0") = self->message_window; \
        register char const * text asm("r1") = message; \
        asm ("" : "+r" (text_window), "+r" (text)); \
        MEDAL_FIELD_PTR(substitutions, text_substitutions, offset_register, "r2"); \
        SetMessageWindowTextSubstitutions(text_window, text, substitutions); \
    } while (0)

EC SECTION(".text.f3.0805D760") void UpdateMedalExchangeDialogue(MedalExchange * self)
{
    enum
    {
        WINDOW_CLOSED = 0, WINDOW_TEXT_COMPLETE = 3,
        OPEN_REQUESTED = 0, CLOSING = 1, IDLE = 2, PREPARE_TEXT = 3, WAIT_CHOICE = 4, CLOSE_AFTER_TEXT = 5,
        EXCHANGE_PROMPT = 2, EXCHANGE_ACCEPTED = 3, EXCHANGE_DECLINED = 4,
        PRIZE_PROMPT = 8, PRIZE_ACCEPTED = 9, PRIZE_DECLINED = 10,
        CHOICE_YES = 1, CHOICE_NO = 2,
    };
    MessageWindow * window = self->message_window;
    HardwareContextState * hardware_state = HardwareContext_GetState(self);
    OamBuffer * sprites = HardwareContext_GetOamBuffer(self);
    MedalExchangeDisplayRegisters * display = GetMedalExchangeDisplayRegisters(self);
    FrameTaskList * frame_tasks = HardwareContext_GetFrameTasks(self);
    VBlankTaskList * vblank_tasks = HardwareContext_GetVBlankTasks(self);
    AudioPlayer * text_audio = &self->text_audio;

    switch (UpdateMessageWindow(window, hardware_state, sprites, display, frame_tasks, vblank_tasks, text_audio))
    {
    case WINDOW_CLOSED:
        if (self->dialogue_phase == OPEN_REQUESTED)
        {
            OpenMessageWindow(self->message_window);
            self->dialogue_phase = PREPARE_TEXT;
        }
        else
            self->dialogue_phase = IDLE;
        break;
    case WINDOW_TEXT_COMPLETE:
    {
        MEDAL_FIELD_PTR(phase, dialogue_phase, "r4", "r5");
        register int phase_value asm("r4");
        asm ("mov r6, #0\n\tldrsh %0, [%1, r6]"
            : "=l" (phase_value) : "l" (phase) : "r6", "cc", "memory");
        if (phase_value == CLOSE_AFTER_TEXT)
        {
            CloseMessageWindow(self->message_window);
            *phase = CLOSING;
        }
        else if (phase_value == WAIT_CHOICE)
        {
            register u32 selection asm("r0") = GetMessageWindowSelection(self->message_window);
            asm ("" : "+r" (selection));
            register int zero_low asm("r1") = 0;
            asm ("" : "+r" (zero_low));
            register int accepted asm("r8") = zero_low;
            asm ("" : "+r" (accepted));
            register int prepare asm("r3") = PREPARE_TEXT;
            asm ("" : "+r" (prepare));
            *phase = prepare;
            MEDAL_FIELD_PTR(exchange, exchange_phase, "r6", "r2");
            register int exchange_value asm("r1");
            asm ("mov r6, #0\n\tldrsh %0, [%1, r6]"
                : "=l" (exchange_value) : "l" (exchange) : "r6", "cc", "memory");
            switch (exchange_value)
            {
            case EXCHANGE_PROMPT: goto exchange_choice;
            case PRIZE_PROMPT: goto prize_choice;
            default: return;
            }
        exchange_choice:
            if (selection == CHOICE_YES)
            {
                MEDAL_FIELD_PTR(choice, declined_choice, "r1", "r0");
                register int restore_accepted asm("r4") = accepted;
                asm ("" : "+r" (restore_accepted));
                *choice = restore_accepted;
                *exchange = prepare;
                return;
            }
            if (selection == CHOICE_NO)
            {
                MEDAL_FIELD_PTR(choice, declined_choice, "r6", "r1");
                register int no asm("r0") = 1;
                asm ("" : "+r" (no));
                *choice = no;
                *exchange = phase_value; // WAIT_CHOICE and EXCHANGE_DECLINED are both 4.
            }
            return;
        prize_choice:
            if (selection == CHOICE_YES)
            {
                MEDAL_FIELD_PTR(choice, declined_choice, "r1", "r0");
                register int restore_accepted asm("r3") = accepted;
                asm ("" : "+r" (restore_accepted));
                *choice = restore_accepted;
                *phase = CLOSE_AFTER_TEXT;
                *exchange = PRIZE_ACCEPTED;
                return;
            }
            if (selection == CHOICE_NO)
            {
                MEDAL_FIELD_PTR(choice, declined_choice, "r4", "r0");
                *choice = 1;
                *exchange = PRIZE_DECLINED;
            }
            return;
        }
        else if (phase_value == PREPARE_TEXT)
        {
            *phase = CLOSE_AFTER_TEXT;
            // agbcp lowers this switch to a long branch; keep the original short branch and inline table.
            typedef char VerifyExchangePhaseOffset[offsetof(MedalExchange, exchange_phase) == 0xADA ? 1 : -1];
            register void * target asm("r0");
            asm (
                "ldr r6, .Lmedal_exchange_phase_offset\n\t"
                "add r0, %1, r6\n\t"
                "ldrh r0, [r0]\n\t"
                "sub r0, %2\n\t"
                "lsl r0, #16\n\t"
                "asr r0, #16\n\t"
                "cmp r0, %3\n\t"
                "bhi .Lmedal_dialogue_done\n\t"
                "lsl r0, #2\n\t"
                "ldr r1, .Lmedal_dispatch_pointer\n\t"
                "add r0, r0, r1\n\t"
                "ldr r0, [r0]"
                : "=l" (target)
                : "l" (self), "i" (EXCHANGE_PROMPT), "i" (PRIZE_DECLINED - EXCHANGE_PROMPT),
                  "X" (&&start_exchange_prompt), "X" (&&start_farewell),
                  "X" (&&start_browsing_text), "X" (&&start_not_enough_text),
                  "X" (&&start_prize_prompt), "X" (&&start_leaving_text)
                : "r1", "r6", "cc", "memory");
            goto *target;
        start_exchange_prompt:
            asm volatile (
                ".balign 4, 0\n"
                ".Lmedal_exchange_phase_offset: .word 0xADA\n"
                ".Lmedal_dispatch_pointer: .word .Lmedal_dispatch\n"
                ".Lmedal_dispatch:\n\t"
                ".word .Lmedal_start_exchange_prompt, .Lmedal_start_farewell, .Lmedal_start_browsing_text\n\t"
                ".word .Lmedal_dialogue_done, .Lmedal_dialogue_done, .Lmedal_start_not_enough_text\n\t"
                ".word .Lmedal_start_prize_prompt, .Lmedal_dialogue_done, .Lmedal_start_leaving_text\n"
                ".Lmedal_start_exchange_prompt:"
                : : "X" (&&dialogue_done) : "memory");
            MEDAL_PROMPT(gAskToExchangeMedals, "r4");
            {
                MEDAL_FIELD_PTR(choice_phase, dialogue_phase, "r6", "r1");
                *choice_phase = WAIT_CHOICE;
            }
            return;
        start_farewell:
            asm volatile (".Lmedal_start_farewell:" : : : "memory");
            SetMessageWindowTextSubstitutions(self->message_window, gMedalExchangeFarewell, &self->text_substitutions);
            return;
        start_browsing_text:
            asm volatile (".Lmedal_start_browsing_text:" : : : "memory");
            MEDAL_TEXT(gMedalExchangeKeepBrowsing, "r4");
            return;
        start_prize_prompt:
            asm volatile (".Lmedal_start_prize_prompt:" : : : "memory");
            MEDAL_PROMPT(gConfirmMedalExchangePrize, "r6");
            {
                MEDAL_FIELD_PTR(choice_phase, dialogue_phase, "r0", "r1");
                *choice_phase = WAIT_CHOICE;
            }
            return;
        start_leaving_text:
            asm volatile (".Lmedal_start_leaving_text:" : : : "memory");
            SetMessageWindowTextSubstitutions(self->message_window, gMedalExchangeLeaving, &self->text_substitutions);
            return;
        start_not_enough_text:
            asm volatile (".Lmedal_start_not_enough_text:" : : : "memory");
            MEDAL_TEXT(gMedalExchangeNotEnoughMedals, "r4");
        }
        break;
    }
    case 1: case 2: case 4: case 5: case 6:
        break;
    }
dialogue_done:
    asm volatile (".Lmedal_dialogue_done:" : : : "memory");
}

#undef MEDAL_FIELD_PTR
#undef MEDAL_PROMPT
#undef MEDAL_TEXT

EC SECTION(".text.f3.0805DC24") void LoadMedalExchangePrizeGraphics(MedalExchange * self)
{
    enum
    {
        PALETTE_RAM = 0x05000200,
        OBJ_TILE_VRAM = 0x06010000,
        PRIZE_NAME_TILE_VRAM = 0x06002800,
        PRIZE_NAME_MAP_VRAM = 0x0600F000,
        PRIZE_COUNT = MedalExchange::PRIZE_ROWS,
        NAME_WIDTH = 14,
        NAME_HEIGHT = 2,
        TILE_BYTES = 32,
        FIRST_NAME_TILE = (2 << 12) | 0x140,
        NAME_START_ROW = 4,
    };
    struct
    {
        MedalExchangeFrameView frame, result;
        STRUCT_PAD(0x40, 0x44);
        u32 pixels[NAME_WIDTH * NAME_HEIGHT * TILE_BYTES / sizeof(u32)];
        PrizeNameTileSize fill_size, draw_size;
        PrizeNameTileSize * size_ptr;
        u32 * pixels_ptr;
    } work;
    // agbcc places this scratch block just above the three stacked arguments to the text renderer.
    enum { DRAW_CALL_STACK_BYTES = 3 * sizeof(u32) };

    MedalExchange * display = self;
    MedalExchangeFrameSlice<SpriteFrameObject> * objects = &work.frame.objects;
    u32 zero = 0;
    work.frame.objects.data = nullptr;
    asm ("strh %1, [%2, #4]" : "=m" (objects->length) : "l" (zero), "l" (objects));
    MedalExchangeFrameSlice<u8> * tile_slice = &work.frame.tiles;
    MedalExchangeFrameSlice<u8> * frame_tiles = tile_slice;
    work.frame.tiles.data = nullptr;
    asm ("strh %1, [%2, #4]" : "=m" (tile_slice->length) : "l" (zero), "l" (tile_slice));
    MedalExchangeFrameSlice<u16> * frame_palette = &work.frame.palette;
    work.frame.palette.data = nullptr;
    asm ("strh %1, [%2, #4]" : "=m" (frame_palette->length) : "l" (zero), "l" (frame_palette));
    MedalExchangeFrameSlice<SpriteFrameExtra> * extras = &work.frame.extras;
    work.frame.extras.data = nullptr;
    asm ("strh %1, [%2, #4]" : "=m" (extras->length) : "l" (zero), "l" (extras));
    SpriteAnimator * animator = &display->cursor;
    MedalExchangeFrameSource const * source =
        reinterpret_cast<MedalExchangeFrameSource const *>(animator->source);
    MedalExchangeFrameSourceVTable const * vtable = source->vtable;
    u32 keyframe = animator->keyframe;
    SpriteAnimationClip const * clip = &animator->clip;
    keyframe *= sizeof(SpriteKeyframe);
    uintptr_t entry = reinterpret_cast<uintptr_t>(clip->keyframes);
    entry += keyframe;
    u32 frame_id = reinterpret_cast<SpriteKeyframe const *>(entry)->frame;
    MedalExchangeFrameView * cursor_result;
    asm ("add %0, sp, %1" : "=l" (cursor_result)
        : "i" (DRAW_CALL_STACK_BYTES + offsetof(__typeof__(work), result)), "r" (vtable) : "memory");
    vtable->get_frame(cursor_result, source, frame_id);
    MedalExchangeFrameView * frame_result = &work.result;
    // Preserve agbcc's redundant copy of the virtual call's return value.
    CopyPrizeFrame(frame_result, frame_result, sizeof(*frame_result));
    work.frame = *frame_result;

    u16 const * palette = work.frame.palette.data;
    MedalExchangeObjAllocation * cursor_palette = &display->cursor_palette;
    unsigned palette_slot = ResolveMedalObjPalette(cursor_palette, cursor_palette->handle) * TILE_BYTES;
    uintptr_t palette_ram = PALETTE_RAM;
    void * palette_dest = reinterpret_cast<void *>(palette_slot + palette_ram);
    u16 const * palette_data = work.frame.palette.data;
    unsigned bytes = 0;
    if (palette_data)
        bytes = frame_palette->length;
    Dma3_Copy(palette, palette_dest, bytes);

    ReadPrizeFrame(frame_result, reinterpret_cast<SpriteAnimationData const *>(display->prize_graphics_data), 0);
    {
        register MedalExchangeFrameView * copy_destination asm("r1") = &work.frame;
        register MedalExchangeFrameView * copy_source asm("r0") = frame_result;
        asm volatile ("ldmia %1!, {r2, r3, r4}\n\t"
             "stmia %0!, {r2, r3, r4}\n\t"
             "ldmia %1!, {r2, r3, r5}\n\t"
             "stmia %0!, {r2, r3, r5}\n\t"
             "ldmia %1!, {r4, r5}\n\t"
             "stmia %0!, {r4, r5}"
            : "+l" (copy_destination), "+l" (copy_source)
            : : "r2", "r3", "r4", "r5", "memory");
    }
    u16 const * prize_palette = work.frame.palette.data;
    MedalExchangeObjAllocation * prize_palette_allocation = &display->prize_palette;
    palette_slot = ResolveMedalObjPalette(prize_palette_allocation, prize_palette_allocation->handle) * TILE_BYTES;
    palette_dest = reinterpret_cast<void *>(palette_slot + palette_ram);
    bytes = work.frame.palette.data ? frame_palette->length : 0;
    Dma3_Copy(prize_palette, palette_dest, bytes);

    u8 const * tiles = work.frame.tiles.data;
    MedalExchangeObjAllocation * tile_allocation = &display->prize_tiles;
    unsigned tile_slot = ResolveMedalObjTile(tile_allocation, tile_allocation->handle) * TILE_BYTES;
    register uintptr_t tile_vram asm("r7") = OBJ_TILE_VRAM;
    void * tile_dest = reinterpret_cast<void *>(tile_slot + tile_vram);
    bytes = work.frame.tiles.data ? frame_tiles->length : 0;
    Dma3_Copy(tiles, tile_dest, bytes);

    // Render each visible prize name to its own two-row tile strip.
    int prize = 0;
    PrizeNameTileSize * fill_dims = &work.fill_size;
    register uintptr_t draw_dims asm("r1") = DRAW_CALL_STACK_BYTES + offsetof(__typeof__(work), draw_size);
    asm volatile ("add %0, sp\n\tstr %0, [sp, %1]"
        : "+l" (draw_dims) : "i" (DRAW_CALL_STACK_BYTES + offsetof(__typeof__(work), size_ptr)) : "memory");
    register u32 * pixels asm("r2");
    asm volatile ("mov %0, sp\n\tadd %0, %1\n\tstr %0, [sp, %2]"
        : "=l" (pixels)
        : "i" (DRAW_CALL_STACK_BYTES + offsetof(__typeof__(work), pixels)),
          "i" (DRAW_CALL_STACK_BYTES + offsetof(__typeof__(work), pixels_ptr)) : "cc", "memory");
    register u32 width_low asm("r3") = NAME_WIDTH;
    asm ("" : "+r" (width_low));
    register u32 width asm("r10") = width_low;
    register u32 height_low asm("r5") = NAME_HEIGHT;
    asm ("" : "+r" (height_low));
    register u32 height asm("r9") = height_low;
    u8 * name_tiles = reinterpret_cast<u8 *>(PRIZE_NAME_TILE_VRAM);
    register PrizeNameTileSize * fill_width asm("r7") = fill_dims;
    do
    {
        register u32 fill_columns asm("r0") = width;
        asm volatile ("strh %0, [%1]" : : "l" (fill_columns), "l" (fill_width) : "memory");
        u32 fill_rows = height;
        fill_dims->height = fill_rows;
        union { u32 word; PrizeNameTileSize dimensions; } fill;
        u32 fill_word;
        u32 * fill_pixels;
        asm volatile ("ldr %0, [%2]\n\tadd %1, sp, %3"
            : "=l" (fill_word), "=l" (fill_pixels)
            : "l" (fill_width), "i" (DRAW_CALL_STACK_BYTES + offsetof(__typeof__(work), pixels)) : "memory");
        fill.word = fill_word;
        FillPrizeNameTiles(fill.dimensions, fill_pixels, 0);
        unsigned prize_id = display->prize_indices[prize];
        char const * draw_name = gMedalExchangePrizes[prize_id].name;
        register PrizeNameTileSize * draw_width asm("r0") = &work.draw_size;
        u32 draw_columns = width;
        draw_width->width = draw_columns;
        u32 draw_rows = height;
        register PrizeNameTileSize * draw_height asm("r3");
        asm volatile ("ldr %0, [sp, %1]" : "=l" (draw_height)
            : "i" (DRAW_CALL_STACK_BYTES + offsetof(__typeof__(work), size_ptr)) : "memory");
        draw_height->height = draw_rows;
        DrawPrizeNameTiles(*draw_width, work.pixels, 0, 0, draw_name, ({ register unsigned ink asm("r3") = 0; asm ("" : : "r" (ink)); ink; }), 1);
        u32 * name_src;
        asm volatile ("ldr %0, [sp, %1]" : "=l" (name_src)
            : "i" (DRAW_CALL_STACK_BYTES + offsetof(__typeof__(work), pixels_ptr)) : "memory");
        register u8 * target_name_tiles asm("r1") = name_tiles;
        register unsigned name_tile_bytes asm("r2") = sizeof(work.pixels);
        Dma3_Copy(name_src, target_name_tiles, name_tile_bytes);
        register unsigned name_tile_step asm("r2") = sizeof(work.pixels);
        asm ("" : "+r" (name_tile_step));
        name_tiles += name_tile_step;
        ++prize;
    } while (prize < PRIZE_COUNT);

    Dma3_Fill(~0u, display->prize_name_tilemap, sizeof(display->prize_name_tilemap));
    u32 name_tile = FIRST_NAME_TILE;
    int row_prize = 0;
    register u16 * name_map_start asm("r3") = &display->prize_name_tilemap[0][2];
    asm ("" : "+r" (name_map_start));
    register u16 * name_map asm("r12") = name_map_start;
    do
    {
        int column = 0;
        unsigned tile_row = row_prize * NAME_HEIGHT;
        unsigned next_name_tile = name_tile + NAME_WIDTH * NAME_HEIGHT;
        ++row_prize;
        int next_prize = row_prize;
        unsigned top_offset = (tile_row + NAME_START_ROW) * 32 * sizeof(u16);
        unsigned bottom_offset = (tile_row + NAME_START_ROW + 1) * 32 * sizeof(u16);
        unsigned bottom_tile = name_tile + NAME_WIDTH;
        u16 * map_low = name_map;
        u16 * bottom_row;
        asm ("add %0, %1, %2" : "=l" (bottom_row) : "l" (bottom_offset), "l" (map_low) : "cc");
        u16 * top_row = reinterpret_cast<u16 *>(reinterpret_cast<u8 *>(name_map) + top_offset);
        do
        {
            *top_row = name_tile + column;
            *bottom_row = bottom_tile;
            ++bottom_tile;
            ++bottom_row;
            ++top_row;
            ++column;
        } while (column < NAME_WIDTH);
        name_tile = static_cast<u16>(next_name_tile);
        row_prize = next_prize;
    } while (row_prize < PRIZE_COUNT);
    Dma3_Copy(display->prize_name_tilemap, reinterpret_cast<void *>(PRIZE_NAME_MAP_VRAM), sizeof(display->prize_name_tilemap));
}

// Keep the literal pools at their retail addresses. Assert the layout wherever a pool loads a field offset.
#define MEDAL_SELECTED_ROW_OFFSET 0xAD4
#define MEDAL_FIRST_VISIBLE_ROW_OFFSET 0xAD6
#define MEDAL_SELECTED_PRIZE_OFFSET 0xAD8
#define MEDAL_PHASE_OFFSET 0xADA
#define MEDAL_DIALOGUE_PHASE_OFFSET 0xADC
#define MEDAL_DECLINED_CHOICE_OFFSET 0xADE
#define MEDAL_SECOND_PRIZE_OFFSET 0xAE1
#define MEDAL_LAST_PRIZE_OFFSET 0xAE9
#define MEDAL_FADE_STEP 0x889
#define MEDAL_SAVE_FARMER_OFFSET 0x1BD8
#define MEDAL_SAVE_HELD_ITEM_OFFSET 0x1C2C
#define MEDAL_SAVE_PRIZE_FLAGS_OFFSET 0x2C4A
#define MEDAL_SAVE_WORD_OFFSET 0x2C48
#define MEDAL_SAVE_JEWEL_COUNT_OFFSET 0x218C
#define MEDAL_SAVE_FESTIVAL_FLAGS_OFFSET 0x21C7
#define MEDAL_POWER_BERRY_HELD_ID 0x15F
#define MEDAL_SAVE_BALANCE_MASK 0x1FFFF
#define MEDAL_SAVE_OTHER_BITS_MASK 0xFFF80003
#define MEDAL_SAVE_NON_JEWEL_COUNT_MASK 0xFFFE1FFF
#define STRINGIFY_MEDAL_VALUE(value) #value
#define MEDAL_SCREEN_WORD(label, value) label ": .word " STRINGIFY_MEDAL_VALUE(value) "\n"

typedef char MedalExchangeScreenOffsetsMatch[
    offsetof(MedalExchange, selected_row) == MEDAL_SELECTED_ROW_OFFSET &&
    offsetof(MedalExchange, first_visible_row) == MEDAL_FIRST_VISIBLE_ROW_OFFSET &&
    offsetof(MedalExchange, selected_prize_id) == MEDAL_SELECTED_PRIZE_OFFSET &&
    offsetof(MedalExchange, exchange_phase) == MEDAL_PHASE_OFFSET &&
    offsetof(MedalExchange, dialogue_phase) == MEDAL_DIALOGUE_PHASE_OFFSET &&
    offsetof(MedalExchange, declined_choice) == MEDAL_DECLINED_CHOICE_OFFSET &&
    offsetof(MedalExchange, prize_indices) + 1 == MEDAL_SECOND_PRIZE_OFFSET &&
    offsetof(MedalExchange, prize_indices) + MedalExchange::PRIZE_ROWS - 1 == MEDAL_LAST_PRIZE_OFFSET &&
    offsetof(MedalExchangeGameState, farmer) == MEDAL_SAVE_FARMER_OFFSET &&
    offsetof(MedalExchangeGameState, farmer) + offsetof(Farmer, unk_54) == MEDAL_SAVE_HELD_ITEM_OFFSET &&
    offsetof(MedalExchangeGameState, jewel_count_word) == MEDAL_SAVE_JEWEL_COUNT_OFFSET &&
    offsetof(MedalExchangeGameState, festival_flags) == MEDAL_SAVE_FESTIVAL_FLAGS_OFFSET &&
    offsetof(MedalExchangeGameState, medal_save_word) == MEDAL_SAVE_WORD_OFFSET &&
    offsetof(MedalExchangeGameState, medal_save_bytes) + 2 == MEDAL_SAVE_PRIZE_FLAGS_OFFSET ? 1 : -1];

struct MedalRunFade;
struct MedalRunFadeTask;
struct MedalRunTaskVtable
{
    void * abi[2];
    void (*run)();
    void (*destroy)(MedalRunFadeTask *, u32);
    void (*append)(FrameTaskList *, MedalRunFadeTask *);
};
struct MedalRunFade { void * state; };
struct MedalRunFadeTask
{
    void * previous_link;
    void * next;
    MedalRunTaskVtable const * vtable;
    MedalRunFade fade;
};
struct MedalRunTaskList
{
    void * previous_link;
    void * next;
    MedalRunTaskVtable const * vtable;
};
struct MedalRunSceneVtable
{
    void * abi[2];
    void (*destroy)(void *, u32);
};
struct MedalRunReturnedScene
{
    MedalRunSceneVtable const * vtable;
};
EC MedalRunTaskVtable const gDisplayFadeTaskVtable;
EC MedalRunFade * DisplayFade_Initialize(MedalRunFade *, MedalExchangeDisplayRegisters *);
// Pass the fixed fade step in the full argument register, without a redundant sign extension.
EC void StartMedalFadeIn(MedalRunFade *, i32, u32, u32) asm("DisplayFade_StartIn");
EC void StartMedalFadeOut(MedalRunFade *, i32, u32, u32) asm("DisplayFade_StartOut");
EC void BlankMedalExchangeDisplay(MedalExchange *) asm("HardwareContext_ForceBlank");
EC void RenderMedalExchangeFrame(MedalExchange *) asm("HardwareContext_CommitFrame");
EC void SetMessageWindowPortrait(MessageWindow *, u32);
EC void HardwareContext_UpdateKeys(MedalExchange *);
EC u32 HardwareContext_GetRepeatedKeys(MedalExchange *);
EC u32 HardwareContext_GetPressedKeys(MedalExchange *);
EC void AudioPlayer_StartSong(AudioPlayer *, u32);
EC void DrawMedalExchangeObjects(MedalExchange *);
EC void AwardMedalExchangePowerBerry(Farmer &) asm("func_0800EAFC");
EC void SetMedalExchangeHeldSpecialItem(HeldItem *, u32) asm("func_0800F510");
EC void SetMedalExchangeHeldFood(HeldItem *, u32) asm("func_0800F3B0");
EC void SetMedalExchangeHeldArticle(HeldItem *, u32) asm("func_0800F3E8");
EC void ConstructMedalRunFood(Food *, u32) asm("__4FoodUi");
EC void ConstructMedalRunArticle(Article *, u32) asm("__7ArticleUi");
EC u16 const gMedalExchangePrizeItemIds[];

#define RUN_FIELD_OFFSET(reg, name, label) \
    register uintptr_t name asm(reg); \
    asm ("ldr %0, " label : "=l" (name))

#define RUN_FIELD_PTR(reg, name, offset_reg, label, type) \
    RUN_FIELD_OFFSET(offset_reg, name##_offset, label); \
    register type * name asm(reg) = reinterpret_cast<type *>(reinterpret_cast<u8 *>(self) + name##_offset); \
    asm ("" : "+l" (name))

#define RUN_SIGNED_HALF(out, reg, ptr, index_reg) \
    register i32 out asm(reg); \
    asm ("mov " index_reg ", #0\n\tldrsh %0, [%1, " index_reg "]" \
        : "=l" (out) : "l" (ptr) : index_reg, "cc", "memory")

#define QUEUE_MEDAL_FADE(screen, fade_task) do { \
    register FrameTaskList * tasks asm("r0") = HardwareContext_GetFrameTasks(screen); \
    register MedalRunTaskVtable const * vtable asm("r2") = reinterpret_cast<MedalRunTaskList *>(tasks)->vtable; \
    asm ("" : "+l" (tasks), "+l" (vtable)); \
    register MedalRunFadeTask * queued_fade asm("r1") = fade_task; \
    asm ("" : "+l" (queued_fade)); \
    register void (*append)(FrameTaskList *, MedalRunFadeTask *) asm("r2") = vtable->append; \
    asm ("" : "+l" (append)); \
    append(tasks, queued_fade); \
} while (0)

// A fade remains pending while its task is linked into the frame list.
static inline i32 MedalRunFadePendingSign(MedalRunFadeTask * fade)
{
    register MedalRunFadeTask * f asm("r0") = fade;
    asm ("" : "+l" (f));
    register uintptr_t previous asm("r1") = reinterpret_cast<uintptr_t>(f->previous_link);
    asm ("" : "+l" (previous));
    register i32 busy asm("r0");
    asm ("neg %0, %1\n\torr %0, %1" : "=&l" (busy) : "l" (previous) : "cc");
    return busy;
}

// Preserve the retail jump table and branch/literal-pool layout. Transitions that
// share a short instruction sequence pass the phase field in r1 to that label.
EC SECTION(".text.f3.0805D170") void ** RunMedalExchangeScreen(void ** output, MedalExchange * exchange)
{
    enum { FADE_IN, BROWSE, EXIT_PROMPT, EXIT_ACCEPTED, EXIT_DECLINED,
        SCROLL_UP, SCROLL_DOWN, INSUFFICIENT_MEDALS, PRIZE_PROMPT, PRIZE_ACCEPTED, PRIZE_DECLINED,
        START_FADE_OUT, FADE_OUT };
    enum { JEWEL_OF_TRUTH = 0, POWER_BERRY = 1, TRUFFLE = 8, DRESS = 10, PERFUME = 11 };
    enum { KEY_UP = 0x40, KEY_DOWN = 0x80, KEY_A = 1, KEY_B = 2, DIALOGUE_IDLE = 2 };
    enum { BLEND_ALL_FIRST_TARGETS = 0x3F, FADE_MODE = 1, FADE_MOSAIC = 1 };
    enum { MEDAL_VENDOR_PORTRAIT = 0x75, SFX_CONFIRM = 0xC5, SFX_CANCEL = 0xC6, SFX_CURSOR = 0xCB };
    enum { VISIBLE_PRIZE_ROWS = 8, PRIZE_ROW_HEIGHT = 16, SCROLL_PIXELS_PER_FRAME = 4 };
    enum { POWER_BERRY_FLAG = 1 << 3, JEWEL_FLAG = 1 << 4 };
    struct
    {
        u16 display_value;
        MedalRunFadeTask * fade;
        union { u32 returned_scene; u8 food[sizeof(Food)]; } temporary;
        u8 food_copy[4];
        u8 article[4];
        struct { void * value; void * end; } scene_slot;
    } scratch;
    register void ** result asm("r9") = output;
    register MedalExchange * self asm("r6") = exchange;
    asm ("" : "+r" (result), "+l" (self));

    BlankMedalExchangeDisplay(self);
    register MedalExchangeDisplayRegisters * regs asm("r0") = GetMedalExchangeDisplayRegisters(self);
    register u16 * value_address asm("r1");
    asm ("mov %0, sp" : "=l" (value_address));
    register u32 value asm("r2") = BLEND_ALL_FIRST_TARGETS;
    asm ("" : "+l" (value), "+l" (regs));
    *value_address = value;
    regs->blend_control = value;
    register MedalRunFadeTask * fade asm("r4") = static_cast<MedalRunFadeTask *>(__builtin_new(sizeof(MedalRunFadeTask)));
    asm ("" : "+l" (fade));
    register MedalExchangeDisplayRegisters * fade_registers asm("r1") = GetMedalExchangeDisplayRegisters(self);
    asm ("" : "+l" (fade_registers));
    register uintptr_t zero asm("r0") = 0;
    asm ("" : "+l" (zero));
    fade->previous_link = reinterpret_cast<void *>(zero);
    fade->next = reinterpret_cast<void *>(zero);
    register MedalRunTaskVtable const * fade_vtable asm("r0");
    asm ("ldr %0, .Lmedal_exchange_fade_vtable" : "=l" (fade_vtable) : : "memory");
    fade->vtable = fade_vtable;
    DisplayFade_Initialize(&fade->fade, fade_registers);
    scratch.fade = fade;
    SetMessageWindowPortrait(self->message_window, MEDAL_VENDOR_PORTRAIT);

    // Use a register pair so agbcp saves both r6 (screen) and r7 (prize scratch).
    register u64 self_and_food asm("r6");
    asm ("" : "=l" (self_and_food) : "l" (self));
    asm ("add %H0, sp, %1" : "+l" (self_and_food)
        : "I" (offsetof(__typeof__(scratch), temporary)));
    register Food * food asm("r7");
    asm ("" : "=l" (food), "=l" (self) : "l" (self_and_food));
    register void * scene_slot_low asm("r0") = &scratch.scene_slot;
    asm ("" : "+l" (food), "+l" (scene_slot_low));
    register void * scene_slot asm("r10") = scene_slot_low;
    asm ("" : "+r" (scene_slot));
    register int row asm("r1") = MedalExchange::PRIZE_ROWS - 1;
    asm ("" : "+l" (row));
    register uintptr_t last_row asm("r2");
    asm ("ldr %0, .Lmedal_exchange_last_row" : "=l" (last_row));
    register u8 * prize asm("r0") = reinterpret_cast<u8 *>(self) + last_row;
    asm ("" : "+l" (prize));
    do {
        *prize-- = row;
        asm ("" : "+l" (prize));
        --row;
    } while (row >= 0);
    register MedalExchangeGameState * saved_state asm("r0") = self->game_state;
    asm ("" : "+l" (saved_state));
    RUN_FIELD_OFFSET("r2", flag_offset, ".Lmedal_exchange_save_flags");
    register int redeemed asm("r0");
    asm ("add %0, %1, %2\n\tldrb %0, [%0]\n\tlsl %0, #27"
        : "=l" (redeemed) : "l" (saved_state), "l" (flag_offset) : "cc", "memory");
    if (redeemed < 0)
    {
        register uintptr_t dress_index asm("r3") = offsetof(MedalExchange, prize_indices);
        asm ("" : "+l" (dress_index));
        register u8 * first_row asm("r1") = reinterpret_cast<u8 *>(self) + dress_index;
        asm ("" : "+l" (first_row));
        *first_row = DRESS;
    }
    saved_state = self->game_state;
    asm ("" : "+l" (saved_state));
    asm ("add %0, %1, %2\n\tldrb %0, [%0]\n\tlsl %0, #28"
        : "=l" (redeemed) : "l" (saved_state), "l" (flag_offset) : "cc", "memory");
    if (redeemed < 0)
    {
        RUN_FIELD_OFFSET("r4", perfume_index, ".Lmedal_exchange_second_row");
        register u8 * second_row asm("r1") = reinterpret_cast<u8 *>(self) + perfume_index;
        asm ("" : "+l" (second_row));
        *second_row = PERFUME;
    }

    LoadMedalExchangeScreenGraphics(self);
    LoadMedalExchangePrizeGraphics(self);
    {
        RUN_FIELD_OFFSET("r1", row_offset, ".Lmedal_exchange_selected_row");
        register i16 * row_slot asm("r0") = reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(self) + row_offset);
        asm ("" : "+l" (row_slot));
        register u32 row_zero asm("r1") = 0;
        asm ("" : "+l" (row_zero));
        *row_slot = row_zero;
        RUN_FIELD_OFFSET("r2", top_offset, ".Lmedal_exchange_top_row");
        register i16 * top_slot asm("r0") = reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(self) + top_offset);
        asm ("" : "+l" (top_slot));
        *top_slot = row_zero;
        RUN_FIELD_OFFSET("r3", prize_offset, ".Lmedal_exchange_prize_id");
        register i16 * prize_slot asm("r0") = reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(self) + prize_offset);
        asm ("" : "+l" (prize_slot));
        *prize_slot = row_zero;
        RUN_FIELD_OFFSET("r4", dialogue_offset, ".Lmedal_exchange_dialogue_phase");
        register i16 * dialogue_slot asm("r2") = reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(self) + dialogue_offset);
        asm ("" : "+l" (dialogue_slot));
        *dialogue_slot = DIALOGUE_IDLE;
        RUN_FIELD_OFFSET("r2", phase_offset, ".Lmedal_exchange_phase");
        register i16 * phase_slot asm("r0") = reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(self) + phase_offset);
        asm ("" : "+l" (phase_slot));
        *phase_slot = row_zero;
    }
    {
        register MedalRunFadeTask * fade_task asm("r0") = scratch.fade;
        asm ("" : "+l" (fade_task));
        RUN_FIELD_OFFSET("r1", fade_step, ".Lmedal_exchange_fade_step");
        StartMedalFadeIn(&fade_task->fade, fade_step, FADE_MODE, FADE_MOSAIC);
        QUEUE_MEDAL_FADE(self, scratch.fade);
    }

    register u32 low_false asm("r3") = 0;
    asm ("" : "+l" (low_false));
    register u32 finished asm("r8") = low_false;
    asm ("" : "+r" (finished));
    do
    {
        RenderMedalExchangeFrame(self);
        HardwareContext_UpdateKeys(self);
        register void * target asm("r0");
        asm volatile ("" : : "X" (&&wait_prize_dialogue), "X" (&&start_fade_out),
            "X" (&&wait_fade_out), "X" (&&browse_down), "X" (&&browse_select),
            "X" (&&browse_exit), "X" (&&insufficient_medals), "X" (&&cancel_exit),
            "X" (&&resume_browsing));
        asm volatile ("" : : "X" (&&check_food_prize), "X" (&&award_power_berry),
            "X" (&&award_truffle), "X" (&&record_jewel), "X" (&&award_article));
        asm (
            "ldr r4, .Lmedal_exchange_phase\n\t"
            "add r0, %1, r4\n\t"
            "mov r1, #0\n\t"
            "ldrsh r0, [r0, r1]\n\t"
            "cmp r0, %8\n\t"
            "bls .Lmedal_exchange_dispatch\n\t"
            "b .Lmedal_exchange_render\n"
            ".Lmedal_exchange_dispatch:\n\t"
            "lsl r0, #2\n\t"
            "ldr r1, .Lmedal_exchange_table_pointer\n\t"
            "add r0, r0, r1\n\t"
            "ldr r0, [r0]"
            : "=l" (target)
            : "l" (self), "X" (&&wait_fade_in), "X" (&&browse), "X" (&&wait_exit_dialogue),
              "X" (&&scroll_up), "X" (&&scroll_down), "X" (&&wait_insufficient), "I" (FADE_OUT)
            : "r1", "r4", "cc", "memory");
        goto *target;
        wait_fade_in:
            asm volatile (
                ".balign 4, 0\n"
                ".Lmedal_exchange_fade_vtable: .word gDisplayFadeTaskVtable\n"
                MEDAL_SCREEN_WORD(".Lmedal_exchange_last_row", MEDAL_LAST_PRIZE_OFFSET)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_save_flags", MEDAL_SAVE_PRIZE_FLAGS_OFFSET)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_second_row", MEDAL_SECOND_PRIZE_OFFSET)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_selected_row", MEDAL_SELECTED_ROW_OFFSET)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_top_row", MEDAL_FIRST_VISIBLE_ROW_OFFSET)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_prize_id", MEDAL_SELECTED_PRIZE_OFFSET)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_dialogue_phase", MEDAL_DIALOGUE_PHASE_OFFSET)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_phase", MEDAL_PHASE_OFFSET)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_fade_step", MEDAL_FADE_STEP)
                ".Lmedal_exchange_table_pointer: .word .Lmedal_exchange_table\n"
                ".Lmedal_exchange_table:\n"
                ".word .Lmedal_exchange_wait_fade_in, .Lmedal_exchange_browse\n"
                ".word .Lmedal_exchange_wait_exit, .Lmedal_exchange_wait_exit, .Lmedal_exchange_wait_exit\n"
                ".word .Lmedal_exchange_scroll_up, .Lmedal_exchange_scroll_down, .Lmedal_exchange_wait_insufficient\n"
                ".word .Lmedal_exchange_wait_prize, .Lmedal_exchange_wait_prize, .Lmedal_exchange_wait_prize\n"
                ".word .Lmedal_exchange_start_fade_out, .Lmedal_exchange_wait_fade_out\n"
                ".Lmedal_exchange_wait_fade_in:" : : : "memory");
            if (MedalRunFadePendingSign(scratch.fade) < 0) goto render;
            {
                RUN_FIELD_PTR("r1", phase, "r2", ".Lmedal_exchange_fade_in_phase", i16);
                asm ("" : : "l" (phase));
                goto advance_phase;
            }
        browse:
            asm volatile (
                ".balign 4, 0\n"
                MEDAL_SCREEN_WORD(".Lmedal_exchange_fade_in_phase", MEDAL_PHASE_OFFSET)
                ".Lmedal_exchange_browse:" : : : "memory");
            {
                register u32 repeated asm("r0") = HardwareContext_GetRepeatedKeys(self);
                register u32 key_mask asm("r1") = KEY_UP;
                asm volatile ("and %0, %1\n\tcmp %0, #0\n\tbeq .Lmedal_exchange_browse_down"
                    : "+l" (repeated) : "l" (key_mask), "X" (&&browse_down) : "cc");
            }
            {
                AudioPlayer_StartSong(&self->sound_effects, SFX_CURSOR);
                register u32 new_row asm("r1");
                {
                    RUN_FIELD_PTR("r1", selected, "r3", ".Lmedal_exchange_up_read_row", u16);
                    register u32 raw asm("r2") = *selected;
                    asm ("" : "+l" (raw));
                    register i32 signed_row asm("r0");
                    asm ("mov r4, #0\n\tldrsh %0, [%1, r4]" : "=l" (signed_row) : "l" (selected) : "r4", "cc", "memory");
                    asm volatile ("cmp %0, #0\n\tble .Lmedal_exchange_clamp_up"
                        : : "l" (signed_row), "X" (&&clamp_up_row) : "cc");
                    register u32 previous asm("r0") = raw - 1;
                    asm ("" : "+l" (previous));
                    *selected = previous;
                    asm ("add %0, %1, #0" : "=l" (new_row) : "l" (previous) : "cc", "memory");
                    goto set_up_row;
                }
            clamp_up_row:
                asm volatile (
                    ".balign 4, 0\n"
                    MEDAL_SCREEN_WORD(".Lmedal_exchange_up_read_row", MEDAL_SELECTED_ROW_OFFSET)
                    ".Lmedal_exchange_clamp_up:" : : : "memory");
                new_row = 0;
            set_up_row:
                {
                    RUN_FIELD_PTR("r0", selected, "r2", ".Lmedal_exchange_up_write_row", u16);
                    *selected = new_row;
                    RUN_FIELD_PTR("r2", first, "r3", ".Lmedal_exchange_up_top_row", u16);
                    asm ("lsl %0, #16\n\tasr %0, #16" : "+l" (new_row) : : "cc");
                    register u32 raw_first asm("r3") = *first;
                    asm ("" : "+l" (raw_first));
                    register int first_signed asm("r0");
                    asm ("mov r4, #0\n\tldrsh %0, [%1, r4]" : "=l" (first_signed) : "l" (first) : "r4", "cc", "memory");
                    if (static_cast<int>(new_row) >= first_signed) goto render;
                    register u32 previous asm("r0") = raw_first - 1;
                    asm ("" : "+l" (previous));
                    *first = previous;
                    RUN_FIELD_PTR("r1", phase, "r0", ".Lmedal_exchange_up_phase", i16);
                    *phase = SCROLL_UP;
                    goto render;
                }
            }
        browse_down:
            asm volatile (
                ".balign 4, 0\n"
                MEDAL_SCREEN_WORD(".Lmedal_exchange_up_write_row", MEDAL_SELECTED_ROW_OFFSET)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_up_top_row", MEDAL_FIRST_VISIBLE_ROW_OFFSET)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_up_phase", MEDAL_PHASE_OFFSET)
                ".Lmedal_exchange_browse_down:" : : : "memory");
            {
                register u32 repeated asm("r0") = HardwareContext_GetRepeatedKeys(self);
                register u32 key_mask asm("r1") = KEY_DOWN;
                register u32 down asm("r5");
                asm volatile ("and %1, %2\n\tlsl %1, #24\n\tlsr %0, %1, #24\n\t"
                    "cmp %0, #0\n\tbeq .Lmedal_exchange_browse_select"
                    : "=l" (down), "+l" (repeated)
                    : "l" (key_mask), "X" (&&browse_select) : "cc");
                AudioPlayer_StartSong(&self->sound_effects, SFX_CURSOR);
                register u32 new_row asm("r1");
                {
                    RUN_FIELD_PTR("r1", selected, "r2", ".Lmedal_exchange_down_read_row", u16);
                    register u32 raw asm("r2") = *selected;
                    asm ("" : "+l" (raw));
                    register i32 signed_row asm("r0");
                    asm ("mov r3, #0\n\tldrsh %0, [%1, r3]" : "=l" (signed_row) : "l" (selected) : "r3", "cc", "memory");
                    asm volatile ("cmp %0, #8\n\tbgt .Lmedal_exchange_clamp_down"
                        : : "l" (signed_row), "X" (&&clamp_down_row) : "cc");
                    register u32 next asm("r0") = raw + 1;
                    asm ("" : "+l" (next));
                    *selected = next;
                    asm ("add %0, %1, #0" : "=l" (new_row) : "l" (next) : "cc", "memory");
                    goto set_down_row;
                }
            clamp_down_row:
                asm volatile (
                    ".balign 4, 0\n"
                    MEDAL_SCREEN_WORD(".Lmedal_exchange_down_read_row", MEDAL_SELECTED_ROW_OFFSET)
                    ".Lmedal_exchange_clamp_down:" : : : "memory");
                new_row = MedalExchange::PRIZE_ROWS - 1;
            set_down_row:
                {
                    RUN_FIELD_PTR("r0", selected, "r4", ".Lmedal_exchange_down_write_row", u16);
                    *selected = new_row;
                    register int selected_signed asm("r1");
                    asm ("mov r2, #0\n\tldrsh %0, [%1, r2]" : "=l" (selected_signed) : "l" (selected) : "r2", "cc", "memory");
                    RUN_FIELD_PTR("r2", first, "r3", ".Lmedal_exchange_down_top_row", u16);
                    register int first_signed asm("r0");
                    asm ("mov r4, #0\n\tldrsh %0, [%1, r4]" : "=l" (first_signed) : "l" (first) : "r4", "cc", "memory");
                    register int visible_row asm("r1") = selected_signed - first_signed;
                    asm ("" : "+l" (visible_row));
                    if (visible_row < VISIBLE_PRIZE_ROWS) goto render;
                    register u32 next asm("r0") = *first + 1;
                    asm ("" : "+l" (next));
                    *first = next;
                    RUN_FIELD_PTR("r1", phase, "r0", ".Lmedal_exchange_down_phase", i16);
                    *phase = SCROLL_DOWN;
                    goto render;
                }
            }
        browse_select:
            asm volatile (
                ".balign 4, 0\n"
                MEDAL_SCREEN_WORD(".Lmedal_exchange_down_write_row", MEDAL_SELECTED_ROW_OFFSET)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_down_top_row", MEDAL_FIRST_VISIBLE_ROW_OFFSET)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_down_phase", MEDAL_PHASE_OFFSET)
                ".Lmedal_exchange_browse_select:" : : : "memory");
            {
                register u32 pressed asm("r0") = HardwareContext_GetPressedKeys(self);
                register u32 selected_key asm("r4");
                asm volatile ("add %0, %1, #0\n\tmov %1, %2\n\tand %0, %1\n\t"
                    "cmp %0, #0\n\tbeq .Lmedal_exchange_browse_exit"
                    : "=l" (selected_key), "+l" (pressed)
                    : "I" (KEY_A), "X" (&&browse_exit) : "cc");
                AudioPlayer_StartSong(&self->sound_effects, SFX_CONFIRM);
                {
                    RUN_FIELD_PTR("r0", dialogue, "r1", ".Lmedal_exchange_select_dialogue", i16);
                    register u32 inactive_key asm("r5");
                    asm ("" : "=l" (inactive_key));
                    *dialogue = inactive_key;
                }
                RUN_FIELD_PTR("r0", selected, "r2", ".Lmedal_exchange_select_row", i16);
                register i32 row asm("r1");
                asm ("mov r3, #0\n\tldrsh %0, [%1, r3]"
                    : "=l" (row) : "l" (selected) : "r3", "cc", "memory");
                register uintptr_t indices_offset asm("r4") = offsetof(MedalExchange, prize_indices);
                asm ("" : "+l" (indices_offset));
                register u8 * indices asm("r0") = reinterpret_cast<u8 *>(self) + indices_offset;
                asm ("" : "+l" (indices));
                register u8 * prize_slot asm("r0") = indices + row;
                asm ("" : "+l" (prize_slot));
                register u32 prize asm("r0") = *prize_slot;
                asm ("" : "+l" (prize));
                selected_offset += offsetof(MedalExchange, selected_prize_id) - offsetof(MedalExchange, selected_row);
                asm ("" : "+l" (selected_offset));
                register i16 * prize_id asm("r1") =
                    reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(self) + selected_offset);
                asm ("" : "+l" (prize_id));
                *prize_id = prize;
                register MedalPrize const * table asm("r2");
                asm ("ldr %0, .Lmedal_exchange_select_prizes" : "=l" (table));
                register int prize_index asm("r3");
                asm ("mov r4, #0\n\tldrsh %0, [%1, r4]"
                    : "=l" (prize_index) : "l" (prize_id), "l" (table) : "r4", "cc", "memory");
                register int price asm("r1");
                asm (
                    "lsl r0, %1, #2\n\tadd r0, r0, %1\n\tlsl r0, #2\n\tadd r0, r0, %2\n\t"
                    "mov r2, %3\n\tldrsh %0, [r0, r2]"
                    : "=l" (price) : "l" (prize_index), "l" (table), "I" (offsetof(MedalPrize, price_in_medals))
                    : "r0", "cc", "memory");
                register uintptr_t balance_offset asm("r4") = offsetof(MedalExchange, medal_balance);
                asm ("" : "+l" (balance_offset));
                register i32 * balance_ptr asm("r0") =
                    reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(self) + balance_offset);
                asm ("" : "+l" (balance_ptr));
                register i32 balance asm("r0") = *balance_ptr;
                asm volatile ("cmp %0, %1\n\tbgt .Lmedal_exchange_insufficient_medals"
                    : : "l" (price), "l" (balance), "X" (&&insufficient_medals) : "cc");
                {
                    register uintptr_t substitution_offset asm("r1") =
                        offsetof(MedalExchange, text_substitutions) + offsetof(MedalExchangeTextSubstitutions, prize_id);
                    asm ("" : "+l" (substitution_offset));
                    register u32 * substitution asm("r0") =
                        reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(self) + substitution_offset);
                    asm ("" : "+l" (substitution));
                    *substitution = prize_index;
                    RUN_FIELD_PTR("r1", phase, "r2", ".Lmedal_exchange_select_phase", i16);
                    *phase = PRIZE_PROMPT;
                    goto render;
                }
            }
        insufficient_medals:
            asm volatile (
                ".balign 4, 0\n"
                MEDAL_SCREEN_WORD(".Lmedal_exchange_select_dialogue", MEDAL_DIALOGUE_PHASE_OFFSET)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_select_row", MEDAL_SELECTED_ROW_OFFSET)
                ".Lmedal_exchange_select_prizes: .word gMedalExchangePrizes\n"
                MEDAL_SCREEN_WORD(".Lmedal_exchange_select_phase", MEDAL_PHASE_OFFSET)
                ".Lmedal_exchange_insufficient_medals:" : : : "memory");
            {
                RUN_FIELD_PTR("r1", phase, "r3", ".Lmedal_exchange_insufficient_phase", i16);
                *phase = INSUFFICIENT_MEDALS;
                goto render;
            }
        browse_exit:
            asm volatile (
                ".balign 4, 0\n"
                MEDAL_SCREEN_WORD(".Lmedal_exchange_insufficient_phase", MEDAL_PHASE_OFFSET)
                ".Lmedal_exchange_browse_exit:" : : : "memory");
            {
                register u32 pressed asm("r0") = HardwareContext_GetPressedKeys(self);
                register u32 key_mask asm("r1") = KEY_B;
                asm volatile ("and %0, %1\n\tcmp %0, #0\n\tbne 1f\n\tb .Lmedal_exchange_render\n1:"
                    : "+l" (pressed) : "l" (key_mask) : "cc");
                AudioPlayer_StartSong(&self->sound_effects, SFX_CANCEL);
                RUN_FIELD_PTR("r0", dialogue, "r1", ".Lmedal_exchange_exit_dialogue", i16);
                register u32 inactive_key asm("r4");
                asm ("" : "=l" (inactive_key));
                *dialogue = inactive_key;
                RUN_FIELD_PTR("r1", phase, "r2", ".Lmedal_exchange_exit_phase", i16);
                *phase = EXIT_PROMPT;
                goto render;
            }
        wait_exit_dialogue:
            asm volatile (
                ".balign 4, 0\n"
                MEDAL_SCREEN_WORD(".Lmedal_exchange_exit_dialogue", MEDAL_DIALOGUE_PHASE_OFFSET)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_exit_phase", MEDAL_PHASE_OFFSET)
                ".Lmedal_exchange_wait_exit:" : : : "memory");
            {
                RUN_FIELD_PTR("r0", dialogue, "r3", ".Lmedal_exchange_wait_exit_dialogue", i16);
                RUN_SIGNED_HALF(dialogue_phase, "r0", dialogue, "r4");
                if (dialogue_phase != DIALOGUE_IDLE) goto render;
                RUN_FIELD_PTR("r0", choice, "r1", ".Lmedal_exchange_exit_choice", i16);
                RUN_SIGNED_HALF(declined, "r0", choice, "r2");
                asm volatile ("cmp %0, #0\n\tbne .Lmedal_exchange_cancel_exit"
                    : : "l" (declined), "X" (&&cancel_exit) : "cc");
                dialogue_offset -= offsetof(MedalExchange, dialogue_phase) - offsetof(MedalExchange, exchange_phase);
                asm volatile ("" : "+l" (dialogue_offset));
                register u8 * phase asm("r1") = reinterpret_cast<u8 *>(self) + dialogue_offset;
                asm volatile ("" : "+l" (phase));
                goto queue_fade_out;
            }
        cancel_exit:
            asm volatile (
                ".balign 4, 0\n"
                MEDAL_SCREEN_WORD(".Lmedal_exchange_wait_exit_dialogue", MEDAL_DIALOGUE_PHASE_OFFSET)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_exit_choice", MEDAL_DECLINED_CHOICE_OFFSET)
                ".Lmedal_exchange_cancel_exit:" : : : "memory");
            {
                RUN_FIELD_PTR("r1", phase, "r4", ".Lmedal_exchange_cancel_exit_phase", i16);
                asm ("" : : "l" (phase));
                goto set_browsing_phase;
            }
        scroll_up:
            asm volatile (
                ".balign 4, 0\n"
                MEDAL_SCREEN_WORD(".Lmedal_exchange_cancel_exit_phase", MEDAL_PHASE_OFFSET)
                ".Lmedal_exchange_scroll_up:" : : : "memory");
            {
                register uintptr_t scroll_offset asm("r1") = offsetof(MedalExchange, scroll_y);
                asm ("" : "+l" (scroll_offset));
                register u16 * scroll asm("r0") =
                    reinterpret_cast<u16 *>(reinterpret_cast<u8 *>(self) + scroll_offset);
                asm ("" : "+l" (scroll));
                register u32 new_scroll asm("r1") = *scroll;
                asm ("" : "+l" (new_scroll));
                new_scroll -= SCROLL_PIXELS_PER_FRAME;
                asm ("" : "+l" (new_scroll));
                *scroll = new_scroll;
                register u32 row_mask asm("r0") = PRIZE_ROW_HEIGHT - 1;
                asm volatile ("and %0, %1\n\tcmp %0, #0\n\tbeq 1f\n\tb .Lmedal_exchange_render\n1:"
                    : "+l" (new_scroll) : "l" (row_mask) : "cc", "memory");
                asm volatile ("b .Lmedal_exchange_resume_browsing" : : : "cc");
                goto scroll_down;
            }
        scroll_down:
            asm volatile (".Lmedal_exchange_scroll_down:" : : : "memory");
            {
                register uintptr_t scroll_offset asm("r3") = offsetof(MedalExchange, scroll_y);
                asm ("" : "+l" (scroll_offset));
                register u16 * scroll asm("r0") =
                    reinterpret_cast<u16 *>(reinterpret_cast<u8 *>(self) + scroll_offset);
                asm ("" : "+l" (scroll));
                register u32 new_scroll asm("r1") = *scroll;
                asm ("" : "+l" (new_scroll));
                new_scroll += SCROLL_PIXELS_PER_FRAME;
                asm ("" : "+l" (new_scroll));
                *scroll = new_scroll;
                register u32 row_mask asm("r0") = PRIZE_ROW_HEIGHT - 1;
                asm volatile ("and %0, %1\n\tcmp %0, #0\n\tbeq 1f\n\tb .Lmedal_exchange_render\n1:"
                    : "+l" (new_scroll) : "l" (row_mask) : "cc", "memory");
                RUN_FIELD_PTR("r1", phase, "r4", ".Lmedal_exchange_scroll_down_phase", i16);
                asm ("" : : "l" (phase));
                goto set_browsing_phase;
            }
        wait_insufficient:
            asm volatile (
                ".balign 4, 0\n"
                MEDAL_SCREEN_WORD(".Lmedal_exchange_scroll_down_phase", MEDAL_PHASE_OFFSET)
                ".Lmedal_exchange_wait_insufficient:" : : : "memory");
            {
                RUN_FIELD_PTR("r0", dialogue, "r1", ".Lmedal_exchange_insufficient_dialogue", i16);
                RUN_SIGNED_HALF(dialogue_phase, "r0", dialogue, "r2");
                if (dialogue_phase != DIALOGUE_IDLE) goto render;
                RUN_FIELD_PTR("r1", phase, "r3", ".Lmedal_exchange_dismiss_insufficient_phase", i16);
                asm ("" : : "l" (phase));
                goto set_browsing_phase;
            }
        wait_prize_dialogue:
            asm volatile (
                ".balign 4, 0\n"
                MEDAL_SCREEN_WORD(".Lmedal_exchange_insufficient_dialogue", MEDAL_DIALOGUE_PHASE_OFFSET)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_dismiss_insufficient_phase", MEDAL_PHASE_OFFSET)
                ".Lmedal_exchange_wait_prize:" : : : "memory");
            {
                RUN_FIELD_PTR("r0", dialogue, "r4", ".Lmedal_exchange_prize_dialogue", i16);
                RUN_SIGNED_HALF(dialogue_phase, "r0", dialogue, "r1");
                if (dialogue_phase != DIALOGUE_IDLE) goto render;
                RUN_FIELD_PTR("r0", choice, "r2", ".Lmedal_exchange_prize_choice", i16);
                RUN_SIGNED_HALF(declined, "r0", choice, "r3");
                asm volatile ("cmp %0, #0\n\tbeq 1f\n\tb .Lmedal_exchange_resume_browsing\n1:"
                    : : "l" (declined) : "cc");
                dialogue_offset -= offsetof(MedalExchange, dialogue_phase) - offsetof(MedalExchange, selected_prize_id);
                asm volatile ("" : "+l" (dialogue_offset));
                register i16 * chosen asm("r0") =
                    reinterpret_cast<i16 *>(reinterpret_cast<u8 *>(self) + dialogue_offset);
                asm ("" : "+l" (chosen));
                RUN_SIGNED_HALF(prize, "r0", chosen, "r1");
                asm volatile (
                    "cmp %0, %1\n\tbeq .Lmedal_exchange_power_berry\n\t"
                    "cmp %0, %1\n\tbgt .Lmedal_exchange_check_food_prize\n\t"
                    "cmp %0, %2\n\tbeq .Lmedal_exchange_record_jewel\n\t"
                    "b .Lmedal_exchange_award_article"
                    : : "l" (prize), "I" (POWER_BERRY), "I" (JEWEL_OF_TRUTH) : "cc");
                goto check_food_prize;
            }
        check_food_prize:
            asm volatile (
                ".balign 4, 0\n"
                MEDAL_SCREEN_WORD(".Lmedal_exchange_prize_dialogue", MEDAL_DIALOGUE_PHASE_OFFSET)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_prize_choice", MEDAL_DECLINED_CHOICE_OFFSET)
                ".Lmedal_exchange_check_food_prize:" : : : "memory");
            asm volatile (
                "cmp r0, %0\n\tbeq .Lmedal_exchange_award_truffle\n\t"
                "b .Lmedal_exchange_award_article"
                : : "I" (TRUFFLE) : "cc");
            goto award_power_berry;
        award_power_berry:
            asm volatile (".Lmedal_exchange_power_berry:" : : : "memory");
            {
                register MedalExchangeGameState * game asm("r0") = self->game_state;
                asm ("" : "+l" (game));
                RUN_FIELD_OFFSET("r2", farmer_offset, ".Lmedal_exchange_save_farmer");
                AwardMedalExchangePowerBerry(*reinterpret_cast<Farmer *>(reinterpret_cast<u8 *>(game) + farmer_offset));
                register MedalExchangeGameState * saved asm("r2") = self->game_state;
                asm ("" : "+l" (saved));
                RUN_FIELD_OFFSET("r3", flags_offset, ".Lmedal_exchange_power_berry_flags");
                register u8 * flags asm("r2") = reinterpret_cast<u8 *>(saved) + flags_offset;
                asm ("" : "+l" (flags));
                register u32 flags_value asm("r0") = *flags;
                asm ("" : "+l" (flags_value));
                register u32 flag asm("r1") = POWER_BERRY_FLAG;
                asm ("orr %0, %1" : "+l" (flags_value) : "l" (flag) : "cc");
                *flags = flags_value;
                register MedalExchangeGameState * item_state asm("r0") = self->game_state;
                asm ("" : "+l" (item_state));
                RUN_FIELD_OFFSET("r4", item_offset, ".Lmedal_exchange_power_berry_held_item");
                register HeldItem * held_item asm("r0") =
                    reinterpret_cast<HeldItem *>(reinterpret_cast<u8 *>(item_state) + item_offset);
                asm ("" : "+l" (held_item));
                RUN_FIELD_OFFSET("r1", special_item, ".Lmedal_exchange_power_berry_item_id");
                SetMedalExchangeHeldSpecialItem(held_item, special_item);
                goto deduct_medals;
            }
        award_truffle:
            asm volatile (
                ".balign 4, 0\n"
                MEDAL_SCREEN_WORD(".Lmedal_exchange_save_farmer", MEDAL_SAVE_FARMER_OFFSET)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_power_berry_flags", MEDAL_SAVE_PRIZE_FLAGS_OFFSET)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_power_berry_held_item", MEDAL_SAVE_HELD_ITEM_OFFSET)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_power_berry_item_id", MEDAL_POWER_BERRY_HELD_ID)
                ".Lmedal_exchange_award_truffle:" : : : "memory");
            {
                register u16 const * ids asm("r0");
                asm ("ldr %0, .Lmedal_exchange_truffle_item_ids" : "=l" (ids));
                register u32 item asm("r1") = ids[TRUFFLE];
                asm ("" : "+l" (item));
                ConstructMedalRunFood(food, item);
                register MedalExchangeGameState * game asm("r5") = self->game_state;
                asm ("" : "+l" (game));
                RUN_FIELD_OFFSET("r0", held_item_offset, ".Lmedal_exchange_truffle_held_item");
                register HeldItem * held_item asm("r5") =
                    reinterpret_cast<HeldItem *>(reinterpret_cast<u8 *>(game) + held_item_offset);
                asm ("" : "+l" (held_item));
                register u8 * copy asm("r4") = scratch.food_copy;
                asm ("" : "+l" (copy), "+l" (food));
                register u8 * food_destination asm("r0");
                register Food const * food_source asm("r1") = food;
                asm ("add %0, %2, #0\n\tadd %1, %3, #0"
                    : "=l" (food_destination), "=l" (food_source) : "l" (copy), "l" (food) : "cc");
                memcpy(food_destination, food_source, sizeof(Food));
                register u32 first_byte asm("r1") = copy[0];
                asm ("" : "+l" (first_byte));
                register u32 packed asm("r1");
                asm ("mov r0, sp\n\tadd r0, %2\n\tldrb r2, [r0]\n\tlsl r2, #8\n\torr r2, %1\n\t"
                    "add r0, #1\n\tldrb %0, [r0]\n\tlsl %0, #16\n\torr %0, r2"
                    : "=l" (packed) : "l" (first_byte), "I" (offsetof(__typeof__(scratch), food_copy) + 1)
                    : "r0", "r2", "cc", "memory");
                SetMedalExchangeHeldFood(held_item, packed);
                goto deduct_medals;
            }
        record_jewel:
            asm volatile (
                ".balign 4, 0\n"
                ".Lmedal_exchange_truffle_item_ids: .word gMedalExchangePrizeItemIds\n"
                MEDAL_SCREEN_WORD(".Lmedal_exchange_truffle_held_item", MEDAL_SAVE_HELD_ITEM_OFFSET)
                ".Lmedal_exchange_record_jewel:" : : : "memory");
            {
                register MedalExchangeGameState * game asm("r2") = self->game_state;
                asm ("" : "+l" (game));
                RUN_FIELD_OFFSET("r1", flags_offset, ".Lmedal_exchange_jewel_prize_flags");
                register u8 * flags asm("r2") = reinterpret_cast<u8 *>(game) + flags_offset;
                asm ("" : "+l" (flags));
                register u32 flags_value asm("r0") = *flags;
                asm ("" : "+l" (flags_value));
                register u32 flag asm("r1") = JEWEL_FLAG;
                asm ("orr %0, %1" : "+l" (flags_value) : "l" (flag) : "cc");
                *flags = flags_value;
            }
            {
                register MedalExchangeGameState * game asm("r2") = self->game_state;
                asm ("" : "+l" (game));
                RUN_FIELD_OFFSET("r3", jewel_count_offset, ".Lmedal_exchange_jewel_count_word");
                register u32 * count_word asm("r2") =
                    reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(game) + jewel_count_offset);
                asm ("" : "+l" (count_word));
                register u32 previous asm("r3") = *count_word;
                asm ("" : "+l" (previous));
                register u32 count asm("r1");
                asm ("lsl %0, %1, #15\n\tlsr %0, #28" : "=l" (count) : "l" (previous) : "cc");
                ++count;
                asm ("" : "+l" (count));
                register u32 count_mask asm("r0") = 15;
                asm ("and %0, %1\n\tlsl %0, #13" : "+l" (count) : "l" (count_mask) : "cc");
                register uintptr_t keep_bits asm("r0");
                asm ("ldr %0, .Lmedal_exchange_jewel_keep_bits" : "=l" (keep_bits));
                asm ("and %0, %1\n\torr %0, %2" : "+l" (keep_bits) : "l" (previous), "l" (count) : "cc");
                *count_word = keep_bits;
            }
            {
                register MedalExchangeGameState * game asm("r2") = self->game_state;
                asm ("" : "+l" (game));
                RUN_FIELD_OFFSET("r4", festival_offset, ".Lmedal_exchange_festival_flags");
                register u8 * flags asm("r2") = reinterpret_cast<u8 *>(game) + festival_offset;
                asm ("" : "+l" (flags));
                register u32 flags_value asm("r0") = *flags;
                asm ("" : "+l" (flags_value));
                register u32 flag asm("r1") = 4;
                asm ("orr %0, %1" : "+l" (flags_value) : "l" (flag) : "cc");
                *flags = flags_value;
            }
        award_article:
            asm volatile (".Lmedal_exchange_award_article:" : : : "memory");
            {
                register Article * article asm("r4") = reinterpret_cast<Article *>(scratch.article);
                asm ("" : "+l" (article));
                register u16 const * ids asm("r1");
                asm ("ldr %0, .Lmedal_exchange_award_item_ids" : "=l" (ids));
                RUN_FIELD_PTR("r0", selected_prize, "r2", ".Lmedal_exchange_award_prize_id", i16);
                RUN_SIGNED_HALF(prize_id, "r0", selected_prize, "r3");
                register u16 const * slot asm("r0");
                asm ("lsl %0, #1\n\tadd %0, %0, %2" : "=l" (slot) : "0" (prize_id), "l" (ids) : "cc");
                register u32 item asm("r1") = *slot;
                asm ("" : "+l" (item));
                ConstructMedalRunArticle(article, item);
                register MedalExchangeGameState * game asm("r0") = self->game_state;
                asm ("" : "+l" (game));
                RUN_FIELD_OFFSET("r1", held_item_offset, ".Lmedal_exchange_award_held_item");
                register HeldItem * held_item asm("r0") =
                    reinterpret_cast<HeldItem *>(reinterpret_cast<u8 *>(game) + held_item_offset);
                asm ("" : "+l" (held_item));
                SetMedalExchangeHeldArticle(held_item, *reinterpret_cast<u8 *>(article));
            }
        deduct_medals:
            {
                register uintptr_t balance_offset asm("r2") = offsetof(MedalExchange, medal_balance);
                asm ("" : "+l" (balance_offset));
                register int * balance asm("r3") =
                    reinterpret_cast<int *>(reinterpret_cast<u8 *>(self) + balance_offset);
                asm ("" : "+l" (balance));
                register MedalPrize const * table asm("r2");
                asm ("ldr %0, .Lmedal_exchange_award_prizes" : "=l" (table));
                RUN_FIELD_PTR("r0", selected_prize, "r4", ".Lmedal_exchange_award_prize_id", i16);
                RUN_SIGNED_HALF(prize_index, "r1", selected_prize, "r4");
                register int price asm("r0");
                asm (
                    "lsl %0, %1, #2\n\tadd %0, %0, %1\n\tlsl %0, #2\n\tadd %0, %0, %2\n\t"
                    "mov %1, %3\n\tldrsh %0, [%0, %1]"
                    : "=&l" (price) : "l" (prize_index), "l" (table), "I" (offsetof(MedalPrize, price_in_medals))
                    : "cc", "memory");
                register i32 new_balance asm("r2") = *balance;
                asm ("" : "+l" (new_balance));
                new_balance -= price;
                asm ("" : "+l" (new_balance));
                *balance = new_balance;
                register MedalExchangeGameState * game asm("r3") = self->game_state;
                asm ("" : "+l" (game));
                RUN_FIELD_OFFSET("r4", word_offset, ".Lmedal_exchange_save_medal_word");
                register u32 * medal_word asm("r3") =
                    reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(game) + word_offset);
                asm ("" : "+l" (medal_word));
                register uintptr_t medal_mask asm("r0");
                asm ("ldr %0, .Lmedal_exchange_save_medal_mask" : "=l" (medal_mask));
                asm ("and %0, %1\n\tlsl %0, #2" : "+l" (new_balance) : "l" (medal_mask) : "cc");
                register u32 old_word asm("r0") = *medal_word;
                asm ("" : "+l" (old_word));
                register uintptr_t keep_bits asm("r1");
                asm ("ldr %0, .Lmedal_exchange_save_medal_keep_bits" : "=l" (keep_bits));
                asm ("and %0, %1\n\torr %0, %2" : "+l" (old_word) : "l" (keep_bits), "l" (new_balance) : "cc");
                *medal_word = old_word;
                RUN_FIELD_PTR("r1", phase, "r0", ".Lmedal_exchange_award_phase", i16);
                asm ("" : : "l" (phase));
            }
        queue_fade_out:
            {
                register i16 * phase asm("r1");
                asm ("" : "=l" (phase));
                *phase = START_FADE_OUT;
                goto render;
            }
        resume_browsing:
            asm volatile (
                ".balign 4, 0\n"
                MEDAL_SCREEN_WORD(".Lmedal_exchange_jewel_prize_flags", MEDAL_SAVE_PRIZE_FLAGS_OFFSET)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_jewel_count_word", MEDAL_SAVE_JEWEL_COUNT_OFFSET)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_jewel_keep_bits", MEDAL_SAVE_NON_JEWEL_COUNT_MASK)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_festival_flags", MEDAL_SAVE_FESTIVAL_FLAGS_OFFSET)
                ".Lmedal_exchange_award_item_ids: .word gMedalExchangePrizeItemIds\n"
                MEDAL_SCREEN_WORD(".Lmedal_exchange_award_prize_id", MEDAL_SELECTED_PRIZE_OFFSET)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_award_held_item", MEDAL_SAVE_HELD_ITEM_OFFSET)
                ".Lmedal_exchange_award_prizes: .word gMedalExchangePrizes\n"
                MEDAL_SCREEN_WORD(".Lmedal_exchange_save_medal_word", MEDAL_SAVE_WORD_OFFSET)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_save_medal_mask", MEDAL_SAVE_BALANCE_MASK)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_save_medal_keep_bits", MEDAL_SAVE_OTHER_BITS_MASK)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_award_phase", MEDAL_PHASE_OFFSET)
                ".Lmedal_exchange_resume_browsing:");
            {
                RUN_FIELD_PTR("r1", phase, "r2", ".Lmedal_exchange_resume_browsing_phase", i16);
                asm volatile ("" : : "l" (phase));
            }
        set_browsing_phase:
            {
                register i16 * phase asm("r1");
                asm ("" : "=l" (phase));
                *phase = BROWSE;
                goto render;
            }
        start_fade_out:
            asm volatile (
                ".balign 4, 0\n"
                MEDAL_SCREEN_WORD(".Lmedal_exchange_resume_browsing_phase", MEDAL_PHASE_OFFSET)
                ".Lmedal_exchange_start_fade_out:" : : : "memory");
            {
                register MedalRunFadeTask * fade_task asm("r0") = scratch.fade;
                asm ("" : "+l" (fade_task));
                RUN_FIELD_OFFSET("r1", fade_step, ".Lmedal_exchange_fade_out_step");
                StartMedalFadeOut(&fade_task->fade, fade_step, FADE_MODE, FADE_MOSAIC);
                QUEUE_MEDAL_FADE(self, scratch.fade);
                RUN_FIELD_PTR("r1", phase, "r3", ".Lmedal_exchange_fade_out_phase", i16);
                asm ("" : : "l" (phase));
            }
        advance_phase:
            {
                register i16 * phase asm("r1");
                asm ("" : "=l" (phase));
                ++*phase;
                goto render;
            }
        wait_fade_out:
            asm volatile (
                ".balign 4, 0\n"
                MEDAL_SCREEN_WORD(".Lmedal_exchange_fade_out_step", MEDAL_FADE_STEP)
                MEDAL_SCREEN_WORD(".Lmedal_exchange_fade_out_phase", MEDAL_PHASE_OFFSET)
                ".Lmedal_exchange_wait_fade_out:" : : : "memory");
            if (MedalRunFadePendingSign(scratch.fade) >= 0)
            {
                register u32 done asm("r4") = 1;
                asm ("" : "+l" (done));
                finished = done;
            }
        render:
        asm volatile (".Lmedal_exchange_render:" : : : "memory");
        UpdateMedalExchangeDialogue(self);
        register MedalExchangeDisplayRegisters * frame_regs asm("r0") = GetMedalExchangeDisplayRegisters(self);
        register u16 * scroll_copy asm("r2");
        asm volatile ("mov %0, sp" : "=l" (scroll_copy) : "l" (frame_regs));
        register uintptr_t scroll asm("r3") = offsetof(MedalExchange, scroll_y);
        asm ("" : "+l" (scroll) : "l" (scroll_copy));
        register u16 * scroll_ptr asm("r1") =
            reinterpret_cast<u16 *>(reinterpret_cast<u8 *>(self) + scroll);
        asm ("" : "+l" (scroll_ptr));
        register u16 scroll_y asm("r1") = *scroll_ptr;
        asm volatile ("strh %0, [%1]" : : "l" (scroll_y), "l" (scroll_copy) : "memory");
        frame_regs->bg1_scroll_y = scroll_y;
        UpdateMedalExchangeGlyphs(self);
        DrawMedalExchangeObjects(self);
        register u32 loop_done asm("r4");
        asm ("mov %0, %1" : "=l" (loop_done) : "r" (finished));
        if (loop_done == 1)
            break;
    } while (true);
    register u32 empty asm("r0") = 0;
    asm ("" : "+l" (empty));
    scratch.temporary.returned_scene = empty;
    asm volatile ("str %0, [sp, #20]" : : "l" (food) : "memory");
    register void ** slot asm("r1");
    asm ("mov %0, %1" : "=l" (slot) : "r" (scene_slot));
    asm volatile ("str %0, [%1, #4]\n\tstr %0, [%2]"
        : : "l" (empty), "l" (slot), "l" (food) : "memory");
    register void ** destination asm("r2");
    asm ("mov %0, %1" : "=l" (destination) : "r" (result));
    *destination = reinterpret_cast<void *>(empty);
    register MedalRunReturnedScene * discarded asm("r1");
    asm volatile ("ldr %0, [sp, #8]" : "=l" (discarded) : : "memory");
    {
        asm volatile ("cmp %0, #0\n\tbeq .Lmedal_exchange_destroy_fade"
            : : "l" (discarded) : "cc");
        register void * scene_argument asm("r0");
        register void (*destroy)(void *, u32) asm("r2") = discarded->vtable->destroy;
        asm ("" : "+l" (destroy));
        asm ("add %0, %1, #0" : "=l" (scene_argument) : "l" (discarded) : "cc");
        destroy(scene_argument, 3);
    }
    asm volatile (".Lmedal_exchange_destroy_fade:" : : : "memory");
    if (scratch.fade) scratch.fade->vtable->destroy(scratch.fade, 3);
    return result;
}

#undef QUEUE_MEDAL_FADE
#undef RUN_SIGNED_HALF
#undef RUN_FIELD_PTR
#undef RUN_FIELD_OFFSET

#undef MEDAL_SCREEN_WORD
#undef MEDAL_SAVE_NON_JEWEL_COUNT_MASK
#undef MEDAL_SAVE_OTHER_BITS_MASK
#undef MEDAL_SAVE_BALANCE_MASK
#undef MEDAL_POWER_BERRY_HELD_ID
#undef MEDAL_SAVE_FESTIVAL_FLAGS_OFFSET
#undef MEDAL_SAVE_JEWEL_COUNT_OFFSET
#undef MEDAL_SAVE_WORD_OFFSET
#undef MEDAL_SAVE_PRIZE_FLAGS_OFFSET
#undef MEDAL_SAVE_HELD_ITEM_OFFSET
#undef MEDAL_SAVE_FARMER_OFFSET
#undef MEDAL_FADE_STEP
#undef MEDAL_LAST_PRIZE_OFFSET
#undef MEDAL_SECOND_PRIZE_OFFSET
#undef MEDAL_DECLINED_CHOICE_OFFSET
#undef MEDAL_DIALOGUE_PHASE_OFFSET
#undef MEDAL_SELECTED_PRIZE_OFFSET

EC SpriteOamBuffer * GetMedalObjectBuffer(MedalExchange *) asm("HardwareContext_GetOamBuffer");
EC DmaTransferQueue * GetMedalObjectDmaQueue(MedalExchange *) asm("HardwareContext_GetDmaQueue");
EC void * RetryMedalObjectQueueAllocation(u32) asm("func_080D3BC0");

// Grow by max(occupied, inserted), then preserve the order of the pending uploads.
#define MEDAL_GROW_ARROW_QUEUE(growth, restore_attributes, initial_end_reg, loop_end_reg, single_copy_temp, multi_copy_set, subtract_reg, remaining_reg, free_queue_reg, base_reg, publish_queue_reg) do { \
    MEDAL_WORK_WRITE(tail, old_end); \
    register u32 inserted_count asm("r4") = requested; \
    MEDAL_LOW(inserted_count); \
    MEDAL_WORK_WRITE(inserted_count, growth.inserted); \
    register DmaTransferQueue * old_queue asm("r5"); \
    MEDAL_WORK_READ(old_queue, queue); \
    register DmaCommand * old_begin asm("r0") = old_queue->begin; \
    MEDAL_LOW(old_begin); \
    register DmaCommand * old_tail asm("r7") = tail; \
    MEDAL_LOW(old_tail); \
    register u32 occupied asm("r4"); \
    asm ("sub %1, %2, %1\n\tasr %0, %1, #4" : "=l" (occupied), "+l" (old_begin) \
        : "l" (old_tail) : "cc"); \
    MEDAL_WORK_WRITE(occupied, growth.occupied); \
    register u32 * inserted_ptr asm("r1"); \
    MEDAL_IN_STACK(inserted_ptr, growth.inserted); \
    register u32 * growth_count asm("r0"); \
    MEDAL_IN_STACK(growth_count, growth.occupied); \
    asm ("cmp %1, #1\n\tbhs 1f\n\tadd %0, %2, #0\n1:" : "+l" (growth_count) \
        : "l" (occupied), "l" (inserted_ptr) : "cc"); \
    register u32 increase asm("r0") = *growth_count; \
    MEDAL_LOW(increase); \
    register u32 new_capacity asm("r0") = occupied + increase; \
    MEDAL_LOW(new_capacity); \
    register DmaCommand * allocation asm("r0"); \
    register u32 capacity_bytes asm("r9"); \
    if (new_capacity) { \
        register u32 bytes asm("r4") = new_capacity * sizeof(DmaCommand); \
        MEDAL_LOW(bytes); \
        allocation = static_cast<DmaCommand *>(malloc(bytes)); \
        capacity_bytes = bytes; \
        asm ("" : "+h" (capacity_bytes) : "l" (allocation)); \
        if (!allocation) { \
            allocation = static_cast<DmaCommand *>(RetryMedalObjectQueueAllocation(capacity_bytes)); \
            MEDAL_LOW(allocation); \
        } \
    } else { \
        allocation = nullptr; \
        MEDAL_LOW(allocation); \
        capacity_bytes = reinterpret_cast<uintptr_t>(allocation); \
        MEDAL_HIGH(capacity_bytes); \
    } \
    register DmaCommand * buffer asm("r4") = allocation; \
    MEDAL_LOW(buffer); \
    register DmaCommand * begin asm("r8") = buffer; \
    MEDAL_HIGH(begin); \
    register DmaTransferQueue * queue_to_copy asm("r0"); \
    MEDAL_WORK_READ(queue_to_copy, queue); \
    register DmaCommand * input asm("r2") = queue_to_copy->begin; \
    MEDAL_LOW(input); \
    register DmaCommand * output asm("r3") = begin; \
    MEDAL_LOW(output); \
    if (restore_attributes) { \
        register SpriteAttributes * attributes asm("r1"); \
        asm ("mov %0, sp\n\tadd %0, %1" : "=l" (attributes) \
            : "I" (offsetof(Stack, object.attributes)) : "cc"); \
        MEDAL_WORK_WRITE(attributes, attributes_ptr); \
    } \
    register DmaCommand * first_end asm(initial_end_reg); \
    MEDAL_WORK_READ(first_end, old_end); \
    if (input != first_end) { \
        register DmaCommand * old_end asm(loop_end_reg); \
        do { \
            asm volatile ("cmp %0, #0\n\tbeq 1f\n\tadd r0, %0, #0\n\tadd r1, %1, #0\n\t" \
                "ldmia r1!, {r4,r5,r7}\n\tstmia r0!, {r4,r5,r7}\n\tldr r1, [r1]\n\tstr r1, [r0]\n1:" \
                : : "l" (output), "l" (input) : "r0", "r1", "r4", "r5", "r7", "cc", "memory"); \
            ++input; \
            MEDAL_LOW(input); \
            ++output; \
            MEDAL_LOW(output); \
            MEDAL_WORK_READ(old_end, old_end); \
        } while (input != old_end); \
    } \
    register DmaCommand * new_end asm("r4") = output; \
    MEDAL_LOW(new_end); \
    register u32 append_count asm("r0"); \
    MEDAL_WORK_READ(append_count, growth.inserted); \
    if (append_count == 1) { \
        asm volatile ("cmp %0, #0\n\tbeq 1f\n\tadd r0, %0, #0\n\tmov r1, %1\n\t" \
            "ldmia r1!, {r2,r3," single_copy_temp "}\n\tstmia r0!, {r2,r3," single_copy_temp "}\n\t" \
            "ldr r1, [r1]\n\tstr r1, [r0]\n1:" : : "l" (new_end), "h" (command) \
            : "r0", "r1", "r2", "r3", single_copy_temp, "cc", "memory"); \
        ++new_end; \
        MEDAL_LOW(new_end); \
    } else { \
        register u32 pending asm("r12") = append_count; \
        MEDAL_HIGH(pending); \
        register DmaCommand * output asm("r2") = new_end; \
        MEDAL_LOW(output); \
        if (append_count) { \
            register u32 remaining asm(remaining_reg); \
            do { \
                asm volatile ("cmp %0, #0\n\tbeq 1f\n\tadd r0, %0, #0\n\tmov r1, %1\n\t" \
                    "ldmia r1!, {" multi_copy_set "}\n\tstmia r0!, {" multi_copy_set "}\n\tldr r1, [r1]\n\tstr r1, [r0]\n1:" \
                    : : "l" (output), "h" (command) : "r0", "r1", "r3", "r4", "r5", "r7", "cc", "memory"); \
                register u32 decrement asm(subtract_reg) = MEDAL_CONST(subtract_reg, 1); \
                asm ("neg %1, %1\n\tadd %0, %1" : "+h" (pending), "+l" (decrement) : : "cc"); \
                ++output; \
                MEDAL_LOW(output); \
                remaining = pending; \
                MEDAL_LOW(remaining); \
            } while (remaining != 0); \
        } \
        new_end = output; \
        MEDAL_LOW(new_end); \
    } \
    register DmaTransferQueue * discard asm(free_queue_reg); \
    MEDAL_WORK_READ(discard, queue); \
    register DmaCommand * discard_end asm("r2") = discard->end; \
    MEDAL_LOW(discard_end); \
    register DmaCommand * discard_begin asm("r0") = discard->begin; \
    MEDAL_LOW(discard_begin); \
    register DmaCommand * cursor asm("r1") = discard_begin; \
    MEDAL_LOW(cursor); \
    if (discard_begin != discard_end) { \
        do { \
            ++cursor; \
            MEDAL_LOW(cursor); \
        } while (cursor != discard_end); \
    } \
    if (discard_begin) free(discard_begin); \
    register uintptr_t limit asm("r0") = capacity_bytes; \
    MEDAL_LOW(limit); \
    asm ("add %0, %1" : "+l" (limit) : "h" (begin)); \
    register DmaCommand * new_begin asm(base_reg) = begin; \
    MEDAL_LOW(new_begin); \
    register DmaTransferQueue * publish asm(publish_queue_reg); \
    MEDAL_WORK_READ(publish, queue); \
    asm volatile ("str %1, [%0, %4]\n\tstr %2, [%0, %5]\n\tstr %3, [%0, %6]" \
        : : "l" (publish), "l" (new_begin), "l" (new_end), "l" (limit), \
          "I" (offsetof(DmaTransferQueue, begin)), "I" (offsetof(DmaTransferQueue, end)), \
          "I" (offsetof(DmaTransferQueue, capacity_end)) : "memory"); \
} while (0)

// The legacy DMA append path uses a separate growth record for each arrow.
struct MedalObjectQueueGrowth
{
    unsigned occupied;
    unsigned inserted;
};

// Inline Thumb fragments address this scratch layout directly.
struct MedalObjectDrawWork
{
    SpriteFrameDrawOptions options;
    MedalExchangeFrameView frame;
    union
    {
        MedalExchangeFrameView result;
        DmaCommand arrow_upload;
    };
    MedalObjectQueueGrowth upper_growth;
    MedalObjectQueueGrowth lower_growth;
    union
    {
        u32 words[2];
        SpriteAttributes attributes;
    } object;
    SpriteAttributes * attributes_ptr;
    DmaTransferQueue * queue;
    DmaCommand * old_end;
};

#define MEDAL_LOW(value) asm ("" : "+l" (value))
#define MEDAL_HIGH(value) asm ("" : "+h" (value))
#define MEDAL_WORK_READ(value, field) \
    asm volatile ("ldr %0, [sp, %1]" : "=l" (value) : "I" (offsetof(MedalObjectDrawWork, field)) : "memory")
#define MEDAL_WORK_WRITE(value, field) \
    asm volatile ("str %0, [sp, %1]" : : "l" (value), "I" (offsetof(MedalObjectDrawWork, field)) : "memory")
#define MEDAL_SET_OPTION(store, options, field, value) \
    asm volatile (store " %0, [%1, %2]" : : "l" (value), "l" (options), "I" (offsetof(SpriteFrameDrawOptions, field)) : "memory")
#define MEDAL_IN_STACK(value, field) \
    asm ("add %0, sp, %1" : "=l" (value) : "I" (offsetof(MedalObjectDrawWork, field)))
#define MEDAL_CONST(reg, value) ({ \
    register u32 constant asm(reg) = (value); MEDAL_LOW(constant); constant; \
})
#define MEDAL_FIELD(type, reg, member) ({ \
    register uintptr_t address asm(reg) = reinterpret_cast<uintptr_t>(self); \
    MEDAL_LOW(address); \
    address += offsetof(MedalExchange, member); \
    MEDAL_LOW(address); \
    reinterpret_cast<type *>(address); \
})
#define MEDAL_LOAD_LITERAL(reg, name) ({ \
    register u32 constant asm(reg); \
    asm ("ldr %0, .Lmedal_cpp_" #name : "=l" (constant)); \
    constant; \
})
#define MEDAL_DRAW_WORD(name, value) ".Lmedal_cpp_" #name ": .word " STRINGIFY_MEDAL_VALUE(value) "\n"
#define MEDAL_UPPER_ARROW_VRAM 0x06010080
#define MEDAL_LOWER_ARROW_VRAM 0x06010100
#define MEDAL_LOAD_FIELD(type, reg, field_offset) ({ \
    register type * p asm(reg) = reinterpret_cast<type *>(reinterpret_cast<uintptr_t>(self) + field_offset); \
    MEDAL_LOW(p); \
    p; \
})

// The frame source's virtual call returns a descriptor through a scratch buffer.
#define READ_MEDAL_FRAME(animator_member, result_reg) \
    register SpriteAnimator * animator asm("r4") = MEDAL_FIELD(SpriteAnimator, "r4", animator_member); \
    AdvanceSpriteAnimator(animator); \
    register MedalExchangeFrameSource const * animation_data asm("r1") = \
        reinterpret_cast<MedalExchangeFrameSource const *>(animator->source); \
    MEDAL_LOW(animation_data); \
    register MedalExchangeFrameSourceVTable const * frame_vtable asm("r3") = animation_data->vtable; \
    MEDAL_LOW(frame_vtable); \
    register unsigned keyframe asm("r2") = animator->keyframe; \
    MEDAL_LOW(keyframe); \
    register SpriteKeyframe const * const * keyframes asm("r0") = \
        MEDAL_FIELD(SpriteKeyframe const * const, "r0", animator_member.clip.keyframes); \
    keyframe *= sizeof(SpriteKeyframe); \
    MEDAL_LOW(keyframe); \
    register SpriteKeyframe const * keys asm("r0") = *keyframes; \
    MEDAL_LOW(keys); \
    register SpriteKeyframe const * current asm("r0"); \
    asm ("add %0, %1, %2" : "=l" (current) : "l" (keys), "l" (keyframe) : "cc"); \
    register unsigned frame_index asm("r2") = current->frame; \
    MEDAL_LOW(frame_index); \
    register MedalExchangeFrameView * result asm("r0"); \
    MEDAL_IN_STACK(result, result); \
    register void (*read_frame)(MedalExchangeFrameView *, MedalExchangeFrameSource const *, u32) asm("r3") = frame_vtable->get_frame; \
    MEDAL_LOW(read_frame); \
    read_frame(result, animation_data, frame_index); \
    register MedalExchangeFrameView * fetched asm(result_reg); \
    MEDAL_IN_STACK(fetched, result); \
    CopyPrizeFrame(fetched, fetched, sizeof(*fetched))

#define MEDAL_COPY_OPTIONS(member, copy_set, extra_reg) \
    register SpriteFrameDrawOptions * preset_target asm("r1"); \
    asm ("mov %0, sp" : "=l" (preset_target)); \
    register SpriteFrameDrawOptions const * preset asm("r0") = MEDAL_FIELD(SpriteFrameDrawOptions, "r0", member); \
    asm volatile ("ldmia %1!, {r2,r3,r4}\n\tstmia %0!, {r2,r3,r4}\n\t" \
        "ldmia %1!, {" copy_set "}\n\tstmia %0!, {" copy_set "}\n\t" \
        "ldr %1, [%1]\n\tstr %1, [%0]" \
        : "+l" (preset_target), "+l" (preset) : : "r2", "r3", "r4", "r5", extra_reg, "memory")

#define MEDAL_DRAW_FRAME(count_reg) do { \
    register SpriteOamBuffer * oam asm("r0") = GetMedalObjectBuffer(self); \
    MEDAL_LOW(oam); \
    register SpriteOamBuffer * sprites asm("r3") = oam; \
    MEDAL_LOW(sprites); \
    register SpriteAttributes const * objects asm("r0"); \
    asm ("ldr %0, [sp, %1]" : "=l" (objects) : "I" (offsetof(MedalObjectDrawWork, frame.objects.data)) : "memory"); \
    register SpriteAttributes const * list asm("r1") = objects; \
    MEDAL_LOW(list); \
    register u32 count asm("r2") = 0; \
    MEDAL_LOW(count); \
    if (list != nullptr) { \
        register SpriteFrameDrawOptions * stack asm(count_reg); \
        asm ("mov %0, sp" : "=l" (stack)); \
        asm ("ldrh %0, [%1, %2]" : "=l" (count) : "l" (stack), \
            "I" (offsetof(MedalObjectDrawWork, frame.objects.length)) : "memory"); \
    } \
    register SpriteOamBuffer * target asm("r0") = sprites; \
    MEDAL_LOW(target); \
    register SpriteFrameDrawOptions * draw_options asm("r3"); \
    asm ("mov %0, sp" : "=l" (draw_options)); \
    DrawSpriteFrameObjects(target, list, count, draw_options); \
} while (0)

#define MEDAL_ALLOC_FIELD(member, address_reg) ({ \
    register uintptr_t offset asm(address_reg) = offsetof(MedalExchange, member); \
    MEDAL_LOW(offset); \
    MEDAL_LOAD_FIELD(MedalExchangeObjAllocation, "r0", offset); \
})

EC SECTION(".text.f3.0805DFC4") void DrawMedalExchangeObjects(MedalExchange * input)
{
    typedef MedalObjectDrawWork Stack;
    Stack scratch;
    register MedalExchange * self asm("r6") = input;
    MEDAL_LOW(self);
    typedef char DrawWorkHasOriginalSize[sizeof(Stack) == 0x80 ? 1 : -1];
    // Reserve stack storage for accesses addressed relative to sp in the matching Thumb blocks.
    register u32 stack_barrier asm("r0");
    asm ("" : "=l" (stack_barrier) : "m" (scratch));
    register SpriteFrameDrawOptions * initial_options asm("r1");
    asm ("mov %0, sp" : "=l" (initial_options));
    register u32 byte_zero asm("r2") = 0;
    MEDAL_LOW(byte_zero);
    register u32 zero asm("r0") = 0;
    MEDAL_LOW(zero);
    asm volatile (
        "strh %0, [%2]\n\tstrh %0, [%2, #2]\n\tstrh %0, [%2, #4]\n\tstrh %0, [%2, #6]\n\tstrh %0, [%2, #8]\n\t"
        "str %0, [sp, #12]\n\tstrb %1, [%2, #16]\n\tstrb %1, [%2, #17]\n\tstr %0, [sp, #20]\n\tstrb %1, [%2, #24]"
        : : "l" (zero), "l" (byte_zero), "l" (initial_options) : "memory");
#define RESET_MEDAL_FRAME_SPAN(field, base) do { \
    register uintptr_t span asm(base); \
    MEDAL_IN_STACK(span, frame.field); \
    asm volatile ("str %1, [sp, %2]\n\tstrh %1, [%0, #4]" : : "l" (span), "l" (zero), \
        "I" (offsetof(Stack, frame.field)) : "memory"); \
} while (0)
    RESET_MEDAL_FRAME_SPAN(objects, "r3");
    RESET_MEDAL_FRAME_SPAN(tiles, "r2");
    RESET_MEDAL_FRAME_SPAN(palette, "r1");
    RESET_MEDAL_FRAME_SPAN(extras, "r1");
#undef RESET_MEDAL_FRAME_SPAN
    register unsigned phase_offset asm("r1") = MEDAL_LOAD_LITERAL("r1", phase);
    register i16 * phase_ptr asm("r0") = MEDAL_LOAD_FIELD(i16, "r0", phase_offset);
    register u32 phase_zero asm("r4") = 0;
    MEDAL_LOW(phase_zero);
    register int phase asm("r0");
    asm ("ldrsh %0, [%1, %2]" : "=l" (phase) : "l" (phase_ptr), "l" (phase_zero) : "memory");
    // The prize marker follows the selected row relative to the top visible row.
    if (phase > 0)
    {
        READ_MEDAL_FRAME(prize_indicator, "r4");
        register MedalExchangeFrameView * frame_storage asm("r0");
        MEDAL_IN_STACK(frame_storage, frame);
        asm volatile ("ldmia %1!, {r1,r5,r7}\n\tstmia %0!, {r1,r5,r7}\n\t"
            "ldmia %1!, {r2,r3,r5}\n\tstmia %0!, {r2,r3,r5}\n\t"
            "ldmia %1!, {r1,r7}\n\tstmia %0!, {r1,r7}"
            : "+l" (frame_storage), "+l" (fetched) : : "r1", "r2", "r3", "r5", "r7", "memory");
        MEDAL_COPY_OPTIONS(prize_indicator_draw, "r2,r5,r7", "r7");
        register SpriteFrameDrawOptions * options_x asm("r1");
        asm ("mov %0, sp" : "=l" (options_x));
        register u32 x asm("r0") = MEDAL_CONST("r0", 12);
        MEDAL_SET_OPTION("strh", options_x, offset_x, x);
        register SpriteFrameDrawOptions * options_y asm("r2");
        asm ("mov %0, sp" : "=l" (options_y));
        register uintptr_t row_offset asm("r3") = MEDAL_LOAD_LITERAL("r3", selected_row);
        register i16 * row asm("r0") = MEDAL_LOAD_FIELD(i16, "r0", row_offset);
        register u32 zero_row asm("r4") = MEDAL_CONST("r4", 0);
        register i32 selected_y asm("r1");
        asm ("ldrsh %0, [%1, %2]\n\tlsl %0, %0, #4" : "=l" (selected_y)
            : "l" (row), "l" (zero_row) : "cc", "memory");
        register uintptr_t first_row_offset asm("r5") = MEDAL_LOAD_LITERAL("r5", first_visible_row);
        register i16 * first_row asm("r0") = MEDAL_LOAD_FIELD(i16, "r0", first_row_offset);
        register u32 zero_first_row asm("r7") = MEDAL_CONST("r7", 0);
        register i32 first_row_y asm("r0");
        asm ("ldrsh %0, [%1, %2]\n\tlsl %0, %0, #4\n\tsub %0, #40" : "=l" (first_row_y)
            : "l" (first_row), "l" (zero_first_row) : "cc", "memory");
        asm ("sub %0, %0, %1" : "+l" (selected_y) : "l" (first_row_y) : "cc");
        MEDAL_SET_OPTION("strh", options_y, offset_y, selected_y);

        register MedalExchangeObjAllocation * tiles asm("r0") = MEDAL_ALLOC_FIELD(prize_tiles, "r1");
        register u32 handle asm("r1") = tiles->handle;
        MEDAL_LOW(handle);
        register u32 tile_index asm("r0") = ResolveMedalObjTile(tiles, handle);
        register SpriteFrameDrawOptions * options_tile asm("r1");
        asm ("mov %0, sp" : "=l" (options_tile));
        MEDAL_SET_OPTION("strh", options_tile, tile_base, tile_index);

        register MedalExchangeObjAllocation * palette asm("r0") = MEDAL_ALLOC_FIELD(prize_palette, "r2");
        handle = palette->handle;
        MEDAL_LOW(handle);
        register u32 palette_index asm("r0") = ResolveMedalObjPalette(palette, handle);
        register SpriteFrameDrawOptions * options_palette asm("r1");
        asm ("mov %0, sp" : "=l" (options_palette));
        MEDAL_SET_OPTION("strh", options_palette, palette_base, palette_index);
        register u32 priority asm("r0") = MEDAL_CONST("r0", 2);
        MEDAL_SET_OPTION("strh", options_palette, priority, priority);
        register u32 mosaic asm("r0") = MEDAL_CONST("r0", 1);
        MEDAL_SET_OPTION("strb", options_palette, mosaic, mosaic);
        MEDAL_DRAW_FRAME("r4");
    }



    enum { ARROW_X = 232, TOP_ARROW_Y = 26, BOTTOM_ARROW_Y = 152,
        MAX_SCROLL_Y = 32, UPPER_ARROW_TILE = 4, LOWER_ARROW_TILE = 8,
        OBJ_TILE_VRAM = 0x06010000, OBJ_TILE_BYTES = 32 };

#define MEDAL_SCROLL_POS(offset_reg) ({ \
    register uintptr_t offset asm(offset_reg) = offsetof(MedalExchange, scroll_y); \
    MEDAL_LOW(offset); \
    register u16 * scroll asm("r0") = MEDAL_LOAD_FIELD(u16, "r0", offset); \
    register u32 position asm("r0") = *scroll; \
    MEDAL_LOW(position); \
    position; \
})
#define MEDAL_PREPARE_ARROW(animation, preset, arrow_y, arrow_tile, tile_address) \
    READ_MEDAL_FRAME(animation, "r7"); \
    { \
        register MedalExchangeFrameView * frame_storage asm("r1"); \
        MEDAL_IN_STACK(frame_storage, frame); \
        register MedalExchangeFrameView * result_copy asm("r0") = fetched; \
        MEDAL_LOW(result_copy); \
        asm volatile ("ldmia %1!, {r2,r3,r4}\n\tstmia %0!, {r2,r3,r4}\n\t" \
            "ldmia %1!, {r2,r3,r5}\n\tstmia %0!, {r2,r3,r5}\n\t" \
            "ldmia %1!, {r4,r5}\n\tstmia %0!, {r4,r5}" \
            : "+l" (frame_storage), "+l" (result_copy) : : "r2", "r3", "r4", "r5", "memory"); \
    } \
    MEDAL_COPY_OPTIONS(preset, "r2,r3,r5", "r5"); \
    register SpriteFrameDrawOptions * position_options asm("r1"); \
    asm ("mov %0, sp" : "=l" (position_options)); \
    register u32 x asm("r0") = MEDAL_CONST("r0", ARROW_X); \
    MEDAL_SET_OPTION("strh", position_options, offset_x, x); \
    register u32 y asm("r0") = MEDAL_CONST("r0", arrow_y); \
    MEDAL_SET_OPTION("strh", position_options, offset_y, y); \
    register uintptr_t tile_field asm("r5") = offsetof(MedalExchange, cursor_tiles); \
    MEDAL_LOW(tile_field); \
    register MedalExchangeObjAllocation * tiles asm("r4") = MEDAL_LOAD_FIELD(MedalExchangeObjAllocation, "r4", tile_field); \
    register u32 handle asm("r1") = tiles->handle; \
    MEDAL_LOW(handle); \
    register u32 tile_index asm("r0") = ResolveMedalObjTile(tiles, handle); \
    register SpriteFrameDrawOptions * tile_options asm("r1"); \
    asm ("mov %0, sp" : "=l" (tile_options)); \
    asm volatile ("add %0, %2\n\tstrh %0, [%1, %3]" : "+l" (tile_index) \
        : "l" (tile_options), "I" (arrow_tile), "I" (offsetof(SpriteFrameDrawOptions, tile_base)) : "cc", "memory"); \
    register MedalExchangeObjAllocation * palette asm("r0") = MEDAL_ALLOC_FIELD(cursor_palette, "r1"); \
    handle = palette->handle; \
    MEDAL_LOW(handle); \
    register u32 palette_index asm("r0") = ResolveMedalObjPalette(palette, handle); \
    register SpriteFrameDrawOptions * palette_options asm("r1"); \
    asm ("mov %0, sp" : "=l" (palette_options)); \
    MEDAL_SET_OPTION("strh", palette_options, palette_base, palette_index); \
    register u32 priority asm("r0") = MEDAL_CONST("r0", 2); \
    MEDAL_SET_OPTION("strh", palette_options, priority, priority); \
    register SpriteFrameDrawOptions * mosaic_options asm("r0"); \
    asm volatile ("mov %0, sp" : "=l" (mosaic_options)); \
    register u32 one asm("r2") = MEDAL_CONST("r2", 1); \
    register u32 requested asm("r9") = one; \
    MEDAL_HIGH(requested); \
    register u32 mosaic asm("r3") = requested; \
    MEDAL_LOW(mosaic); \
    MEDAL_SET_OPTION("strb", mosaic_options, mosaic, mosaic); \
    register DmaTransferQueue * queue asm("r5") = GetMedalObjectDmaQueue(self); \
    MEDAL_LOW(queue); \
    register u8 const * tile_data asm("r0"); \
    MEDAL_WORK_READ(tile_data, frame.tiles.data); \
    register u8 const * tile_source asm("r8") = tile_data; \
    MEDAL_HIGH(tile_source); \
    handle = tiles->handle; \
    MEDAL_LOW(handle); \
    register u32 upload_tile_index asm("r0") = ResolveMedalObjTile(tiles, handle); \
    MEDAL_LOW(upload_tile_index); \
    register u32 tile_offset asm("r2") = upload_tile_index; \
    MEDAL_LOW(tile_offset); \
    register u8 const * tiles_present asm("r0"); \
    MEDAL_WORK_READ(tiles_present, frame.tiles.data); \
    register u32 tile_bytes asm("r3") = MEDAL_CONST("r3", 0); \
    if (tiles_present) { \
        register void * stack asm("r1"); \
        asm ("mov %0, sp" : "=l" (stack)); \
        asm ("ldrh %0, [%1, %2]" : "=l" (tile_bytes) : "l" (stack), \
            "I" (offsetof(Stack, frame.tiles.length)) : "memory"); \
    } \
    MEDAL_WORK_WRITE(queue, queue); \
    tile_offset *= OBJ_TILE_BYTES; \
    MEDAL_LOW(tile_offset); \
    register u32 destination_base asm("r4") = MEDAL_LOAD_LITERAL("r4", tile_address); \
    register void * destination asm("r2") = reinterpret_cast<void *>(tile_offset + destination_base); \
    MEDAL_LOW(destination); \
    register DmaCommand * command_ptr asm("r0") = reinterpret_cast<DmaCommand *>(fetched); \
    MEDAL_LOW(command_ptr); \
    register u8 const * data asm("r1") = tile_source; \
    MEDAL_LOW(data); \
    DmaCommand_InitializeCopy(command_ptr, data, destination, tile_bytes); \
    register DmaCommand * command asm("r10") = reinterpret_cast<DmaCommand *>(fetched); \
    MEDAL_HIGH(command); \
    register DmaCommand * tail asm("r1") = queue->end; \
    MEDAL_LOW(tail); \
    register DmaCommand * capacity asm("r0") = queue->capacity_end; \
    MEDAL_LOW(capacity);

#define MEDAL_APPEND_ARROW() do { \
    register DmaCommand * copy_source asm("r1") = tail; \
    register DmaCommand * copy_destination asm("r0"); \
    asm volatile ("cmp %0, #0\n\tbeq 1f\n\tadd %1, %0, #0\n\tmov %0, %2\n\t" \
        "ldmia %0!, {r2,r5,r7}\n\tstmia %1!, {r2,r5,r7}\n\t" \
        "ldr %0, [%0]\n\tstr %0, [%1]\n1:" \
        : "+l" (copy_source), "=&l" (copy_destination) : "h" (command) : "r2", "r5", "r7", "cc", "memory"); \
    register DmaTransferQueue * stored_queue asm("r3"); \
    MEDAL_WORK_READ(stored_queue, queue); \
    register DmaCommand * end asm("r0") = stored_queue->end; \
    MEDAL_LOW(end); \
    ++end; \
    MEDAL_LOW(end); \
    stored_queue->end = end; \
} while (0)

    register u32 top_scroll asm("r0") = MEDAL_SCROLL_POS("r5");
    // Make agbcp allocate r7 and save it in the prologue.
    register SpriteAttributes * attributes;
    register unsigned r1 asm("r1"), r2 asm("r2"), r3 asm("r3"), r4 asm("r4"), r5 asm("r5");
    asm ("" : "=l" (r1), "=l" (r2), "=l" (r3), "=l" (r4), "=l" (r5));
    asm volatile ("mov %0, sp\n\tadd %0, %1\n\tstr %0, [sp, %2]" : "=&l" (attributes)
        : "I" (offsetof(Stack, object.attributes)), "I" (offsetof(Stack, attributes_ptr)),
          "l" (top_scroll), "l" (r1), "l" (r2), "l" (r3), "l" (r4), "l" (r5), "l" (self) : "cc", "memory");
    if (top_scroll != 0)
    {
        MEDAL_PREPARE_ARROW(cursor, cursor_draw, TOP_ARROW_Y, UPPER_ARROW_TILE, upper_tile_destination);
        // Keep the literal pool between the fast append and the full-queue path.
        // The full-queue branch carries the original tail in r1 across the pool.
        asm volatile ("cmp %0, %1\n\tbeq .Lmedal_cpp_grow_upper_arrow"
            : : "l" (tail), "l" (capacity) : "cc");
        MEDAL_APPEND_ARROW();
        typedef char UpperArrowLiteralPoolMatches[
            offsetof(MedalExchange, exchange_phase) == MEDAL_PHASE_OFFSET &&
            offsetof(MedalExchange, selected_row) == MEDAL_SELECTED_ROW_OFFSET &&
            offsetof(MedalExchange, first_visible_row) == MEDAL_FIRST_VISIBLE_ROW_OFFSET &&
            OBJ_TILE_VRAM + UPPER_ARROW_TILE * OBJ_TILE_BYTES == MEDAL_UPPER_ARROW_VRAM ? 1 : -1];
        asm volatile ("b .Lmedal_cpp_draw_upper_arrow\n\t.balign 4, 0\n"
            MEDAL_DRAW_WORD(phase, MEDAL_PHASE_OFFSET)
            MEDAL_DRAW_WORD(selected_row, MEDAL_SELECTED_ROW_OFFSET)
            MEDAL_DRAW_WORD(first_visible_row, MEDAL_FIRST_VISIBLE_ROW_OFFSET)
            MEDAL_DRAW_WORD(upper_tile_destination, MEDAL_UPPER_ARROW_VRAM)
            ".Lmedal_cpp_grow_upper_arrow:" : "=l" (tail) : : "memory");
        MEDAL_GROW_ARROW_QUEUE(upper_growth, true, "r4", "r5", "r7", "r4,r5,r7", "r0", "r1", "r3", "r5", "r7");
        asm volatile (".Lmedal_cpp_draw_upper_arrow:" : : : "memory");
        MEDAL_DRAW_FRAME("r0");
    }

    register u32 bottom_scroll asm("r0") = MEDAL_SCROLL_POS("r1");
    if (bottom_scroll < MAX_SCROLL_Y)
    {
        MEDAL_PREPARE_ARROW(menu_icon, menu_icon_draw, BOTTOM_ARROW_Y, LOWER_ARROW_TILE, lower_tile_destination);
        asm volatile ("cmp %0, %1\n\tbeq .Lmedal_cpp_grow_lower_arrow"
            : : "l" (tail), "l" (capacity) : "cc");
        MEDAL_APPEND_ARROW();
        typedef char LowerArrowLiteralPoolMatches[
            OBJ_TILE_VRAM + LOWER_ARROW_TILE * OBJ_TILE_BYTES == MEDAL_LOWER_ARROW_VRAM ? 1 : -1];
        asm volatile ("b .Lmedal_cpp_draw_lower_arrow\n\t.balign 4, 0\n"
            MEDAL_DRAW_WORD(lower_tile_destination, MEDAL_LOWER_ARROW_VRAM)
            ".Lmedal_cpp_grow_lower_arrow:" : "=l" (tail) : : "memory");
        MEDAL_GROW_ARROW_QUEUE(lower_growth, false, "r1", "r0", "r5", "r3,r4,r7", "r5", "r7", "r0", "r1", "r2");
        asm volatile (".Lmedal_cpp_draw_lower_arrow:" : : : "memory");
        MEDAL_DRAW_FRAME("r4");
    }
#undef MEDAL_SCROLL_POS
#undef MEDAL_PREPARE_ARROW
#undef MEDAL_APPEND_ARROW

    // Reuse the OAM entry for each glyph, consuming the glyph list back to front.
    enum
    {
        AFFINE_MASK = 3u << 8,
        OBJECT_MODE_MASK = 3u << 10,
        MOSAIC = 1u << 12,
        COLOR_MODE_MASK = 1u << 13,
        SHAPE_MASK = 3u << 14,
        TALL_SHAPE = 2u << 14,
        AFFINE_INDEX_MASK = 7u << 25,
        HFLIP_MASK = 1u << 28,
        VFLIP_MASK = 1u << 29,
        SIZE_MASK = 3u << 30,
        PALETTE_MASK = 0xF,
        X_MASK = 0x1FF,
        Y_MASK = 0xFF,
        TILE_MASK = 0x3FF,
        PRIORITY_MASK = 3u << 10,
    };

#define MEDAL_PIN(value) asm ("" : "+l" (value))
#define MEDAL_MASK(name, reg, value) register u32 name asm(reg) = (value); MEDAL_PIN(name)
#define MEDAL_CLEAR(field) do { \
    MEDAL_MASK(mask, "r1", ~(field)); \
    asm ("and %0, %1" : "+l" (flags) : "l" (mask) : "cc"); \
} while (0)
#define MEDAL_LOOKUP(result, field, offset_reg) \
    register uintptr_t offset asm(offset_reg) = offsetof(MedalExchange, field); \
    MEDAL_PIN(offset); \
    register MedalExchangeObjAllocation * allocation asm("r0") = \
        reinterpret_cast<MedalExchangeObjAllocation *>(reinterpret_cast<uintptr_t>(self) + offset); \
    MEDAL_PIN(allocation); \
    register u32 handle asm("r1") = allocation->handle; \
    MEDAL_PIN(handle); \
    register u32 index asm("r0") = (result)(allocation, handle)

    register u32 flags asm("r0");
    {
        MEDAL_MASK(mask, "r1", ~AFFINE_MASK);
        asm volatile ("ldr %0, [sp, %2]\n\tand %0, %1" : "=l" (flags)
            : "l" (mask), "I" (offsetof(Stack, object.words[0])) : "cc", "memory");
    }
    MEDAL_CLEAR(OBJECT_MODE_MASK);
    MEDAL_MASK(mosaic, "r4", MOSAIC);
    asm ("orr %0, %1" : "+l" (flags) : "l" (mosaic) : "cc");
    MEDAL_CLEAR(COLOR_MODE_MASK);
    MEDAL_CLEAR(SHAPE_MASK);
    {
        MEDAL_MASK(shape, "r1", TALL_SHAPE);
        asm ("orr %0, %1" : "+l" (flags) : "l" (shape) : "cc");
    }
    MEDAL_CLEAR(AFFINE_INDEX_MASK);
    MEDAL_CLEAR(HFLIP_MASK);
    MEDAL_CLEAR(VFLIP_MASK);
    MEDAL_CLEAR(SIZE_MASK);
    MEDAL_WORK_WRITE(flags, object.words[0]);
#undef MEDAL_CLEAR
    {
        MEDAL_LOOKUP(ResolveMedalObjPalette, glyph_palette, "r5");
        asm ("lsl %0, %0, #16\n\tlsr %0, %0, #16" : "+l" (index) : : "cc");
        MEDAL_MASK(mask, "r1", PALETTE_MASK);
        asm ("and %0, %1\n\tlsl %0, %0, #12" : "+l" (index) : "l" (mask) : "cc");
        MEDAL_MASK(clear_palette, "r2", ~(PALETTE_MASK << 12));
        register SpriteAttributes * attributes asm("r7");
        MEDAL_WORK_READ(attributes, attributes_ptr);
        register u32 palette_word asm("r1");
        asm ("ldr %0, [%1, #4]\n\tand %0, %2\n\torr %0, %3" : "=&l" (palette_word)
            : "l" (attributes), "l" (clear_palette), "l" (index) : "cc", "memory");
        MEDAL_MASK(low_halfword, "r0", 0xFFFF);
        asm volatile ("and %0, %2\n\tstr %0, [%1, #4]" : "+l" (palette_word)
            : "l" (attributes), "l" (low_halfword) : "cc", "memory");
    }
    MEDAL_WORK_READ(flags, object.words[0]);
    asm ("orr %0, %1" : "+l" (flags) : "l" (mosaic) : "cc");
    MEDAL_WORK_WRITE(flags, object.words[0]);

#define MEDAL_POP_GLYPH(count_ptr) ({ \
    register i32 remaining asm("r0"); \
    asm volatile ("ldrh %0, [%1]\n\tsub %0, #1\n\tstrh %0, [%1]\n\tlsl %0, %0, #16" \
        : "=l" (remaining) : "l" (count_ptr) : "cc", "memory"); \
    remaining; \
})
#define MEDAL_GLYPH_FIELD(address, address_reg, field, index_reg, field_reg, zero_reg) \
    MEDAL_MASK(zero_index, zero_reg, 0); \
    register int glyph_offset asm(index_reg); \
    asm ("ldrsh %0, [%1, %2]\n\tlsl %0, %0, #3" : "=l" (glyph_offset) \
        : "l" (glyph_count), "l" (zero_index) : "cc", "memory"); \
    MEDAL_MASK(field_offset, field_reg, offsetof(MedalExchange, glyphs) + offsetof(MedalExchangeGlyph, field)); \
    register uintptr_t field_base asm(address_reg) = reinterpret_cast<uintptr_t>(self) + field_offset; \
    MEDAL_PIN(field_base); \
    register __typeof__(&self->glyphs[0].field) address asm(address_reg) = \
        reinterpret_cast<__typeof__(address)>(field_base + glyph_offset); \
    MEDAL_PIN(address)

    MEDAL_MASK(glyph_count_offset, "r0", offsetof(MedalExchange, glyph_count));
    register i16 * count_ptr asm("r1") =
        reinterpret_cast<i16 *>(reinterpret_cast<uintptr_t>(self) + glyph_count_offset);
    MEDAL_PIN(count_ptr);
    register i32 first_glyph asm("r0") = MEDAL_POP_GLYPH(count_ptr);
    asm volatile ("cmp %0, #0\n\tblt .Lmedal_exchange_glyphs_done" : : "l" (first_glyph) : "cc");
    register i16 * glyph_count asm("r4") = count_ptr;
    MEDAL_PIN(glyph_count);
    register SpriteAttributes * glyph asm("r5");
    MEDAL_WORK_READ(glyph, attributes_ptr);
    do
    {
        {
            MEDAL_GLYPH_FIELD(priority, "r0", oam_priority, "r1", "r3", "r2");
            register u32 value asm("r1") = *priority;
            MEDAL_PIN(value);
            MEDAL_MASK(mask, "r0", 3);
            asm ("and %0, %1\n\tlsl %0, %0, #10" : "+l" (value) : "l" (mask) : "cc");
            MEDAL_MASK(clear_priority, "r2", ~PRIORITY_MASK);
            asm volatile ("ldr r0, [%0, #4]\n\tand r0, %2\n\torr r0, %1\n\tstr r0, [%0, #4]"
                : : "l" (glyph), "l" (value), "l" (clear_priority) : "r0", "cc", "memory");
        }
        register u32 position asm("r2");
        {
            MEDAL_GLYPH_FIELD(x, "r0", x, "r1", "r2", "r7");
            MEDAL_MASK(x_zero, "r3", 0);
            register u32 value asm("r1");
            asm ("ldrsh %0, [%1, %2]" : "=l" (value) : "l" (x), "l" (x_zero) : "memory");
            MEDAL_MASK(mask, "r0", X_MASK);
            asm ("and %0, %1\n\tlsl %0, %0, #16" : "+l" (value) : "l" (mask) : "cc");
            MEDAL_MASK(clear_x, "r0", ~(X_MASK << 16));
            asm volatile ("ldr %0, [sp, %3]\n\tand %0, %2\n\torr %0, %1" : "=l" (position)
                : "l" (value), "l" (clear_x), "I" (offsetof(Stack, object.words[0])) : "cc", "memory");
        }
        {
            MEDAL_GLYPH_FIELD(y, "r0", y, "r1", "r3", "r7");
            register u32 value asm("r1") = *reinterpret_cast<u8 const *>(y);
            MEDAL_PIN(value);
            MEDAL_MASK(clear_y, "r0", ~Y_MASK);
            asm ("and %0, %2\n\torr %0, %1" : "+l" (position)
                : "l" (value), "l" (clear_y) : "cc");
            MEDAL_WORK_WRITE(position, object.words[0]);
        }
        {
            MEDAL_LOOKUP(ResolveMedalObjTile, glyph_tiles, "r7");
            MEDAL_GLYPH_FIELD(tile, "r1", tile, "r2", "r3", "r1");
            MEDAL_MASK(tile_zero, "r7", 0);
            asm ("ldrsh %1, [%1, %2]\n\tlsl %1, %1, #1\n\tadd %0, %0, %1\n\t"
                "lsl %0, %0, #22\n\tlsr %0, %0, #22"
                : "+l" (index), "+l" (tile) : "l" (tile_zero) : "cc", "memory");
            MEDAL_MASK(clear_tile, "r2", ~TILE_MASK);
            asm volatile ("ldr r1, [%0, #4]\n\tand r1, %1\n\torr r1, %2\n\tstr r1, [%0, #4]"
                : : "l" (glyph), "l" (clear_tile), "l" (index) : "r1", "cc", "memory");
        }
        register SpriteOamBuffer * oam asm("r3") = GetMedalObjectBuffer(self);
        MEDAL_PIN(oam);
        register unsigned count asm("r2") = oam->sprite_count;
        MEDAL_PIN(count);
        if (count < sizeof(oam->sprites) / sizeof(oam->sprites[0]))
        {
            register uintptr_t offset asm("r1") = count * sizeof(SpriteAttributes);
            MEDAL_PIN(offset);
            offset += offsetof(SpriteOamBuffer, sprites);
            MEDAL_PIN(offset);
            register SpriteAttributes * out asm("r1") =
                reinterpret_cast<SpriteAttributes *>(reinterpret_cast<uintptr_t>(oam) + offset);
            MEDAL_PIN(out);
            register u32 pos asm("r0");
            MEDAL_WORK_READ(pos, object.words[0]);
            out->words[0] = pos;
            register SpriteAttributes const * attrs asm("r7");
            MEDAL_WORK_READ(attrs, attributes_ptr);
            register u32 tile_palette asm("r0");
            asm ("ldrh %0, [%1, #4]" : "=l" (tile_palette) : "l" (attrs) : "memory");
            reinterpret_cast<u16 *>(out)[2] = tile_palette;
            register u32 next asm("r0") = count + 1;
            MEDAL_PIN(next);
            oam->sprite_count = next;
        }
    } while (MEDAL_POP_GLYPH(glyph_count) >= 0);
    asm volatile (".Lmedal_exchange_glyphs_done:");

#undef MEDAL_GLYPH_FIELD
#undef MEDAL_POP_GLYPH
#undef MEDAL_LOOKUP
#undef MEDAL_MASK
#undef MEDAL_PIN

}

#undef MEDAL_GROW_ARROW_QUEUE
#undef MEDAL_ALLOC_FIELD
#undef MEDAL_DRAW_FRAME
#undef MEDAL_COPY_OPTIONS
#undef READ_MEDAL_FRAME
#undef MEDAL_LOAD_FIELD
#undef MEDAL_LOAD_LITERAL
#undef MEDAL_LOWER_ARROW_VRAM
#undef MEDAL_UPPER_ARROW_VRAM
#undef MEDAL_DRAW_WORD
#undef MEDAL_FIELD
#undef MEDAL_CONST
#undef MEDAL_SET_OPTION
#undef MEDAL_WORK_WRITE
#undef MEDAL_WORK_READ
#undef MEDAL_IN_STACK
#undef MEDAL_HIGH
#undef MEDAL_LOW
#undef MEDAL_PHASE_OFFSET
#undef MEDAL_FIRST_VISIBLE_ROW_OFFSET
#undef MEDAL_SELECTED_ROW_OFFSET
#undef STRINGIFY_MEDAL_VALUE
