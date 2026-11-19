#include "sprite_animation.hh"

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

struct MedalExchange
{
    enum { PRIZE_ROWS = 10, MAX_GLYPHS = 48 };

    STRUCT_PAD(0x00, 0x50);
    u8 prize_graphics_data[0x30];
    SpriteAnimator cursor;
    STRUCT_PAD(0x94, 0x11C);
    MedalExchangeObjAllocation glyph_tiles;
    MedalExchangeObjAllocation glyph_palette;
    STRUCT_PAD(0x12C, 0x134);
    MedalExchangeObjAllocation cursor_palette;
    MedalExchangeObjAllocation prize_tiles;
    MedalExchangeObjAllocation prize_palette;
    u16 scroll_y;
    MedalExchangeGlyph glyphs[MAX_GLYPHS];
    i16 glyph_count;
    u16 prize_name_tilemap[32][32];
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
EC u32 ResolveMedalObjPalette(MedalExchangeObjAllocation *, u32) asm("func_080074C0");
EC u32 ResolveMedalObjTile(MedalExchangeObjAllocation *, u32) asm("func_08007D4C");
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
