#include "clock_display.hh"
#include "sprite_animation.hh"
#include "sprite_rendering.hh"

struct ClockUploadQueueVtable;

struct ClockUploadQueue
{
    void * previous;
    void * next;
    const ClockUploadQueueVtable * vtable;
};
typedef void (*UploadAppender)(ClockUploadQueue *, ClockUploadTask *);

struct ClockUploadQueueVtable
{
    STRUCT_PAD(0, 0x10);
    void (*queueUpload)(ClockUploadQueue *, ClockUploadTask *);
};
// The animation methods return these views through an output pointer.
struct ClockClipView
{
    SpriteKeyframe const * keyframes;
    u16 length;
};
struct ClockFrameView
{
    SpriteFrameObject const * objects;
    u16 objectCount;
    u8 const * tiles;
    u16 tileBytes;
    u16 const * palette;
    u16 paletteBytes;
    SpriteFrameExtra const * extras;
    u16 extraCount;
};
struct ClockAnimationVtable;

struct ClockAnimation
{
    const ClockAnimationVtable * vtable;
};
typedef void (*ReadClip)(ClockClipView *, const ClockAnimation *, u32);
typedef void (*ReadFrame)(ClockFrameView *, const ClockAnimation *, u32);

struct ClockAnimationVtable
{
    STRUCT_PAD(0, 0xC);
    void (*getClip)(ClockClipView *, const ClockAnimation *, u32);
    void (*getFrame)(ClockFrameView *, const ClockAnimation *, u32);
};
struct ClockDrawWork
{
    ClockFrameView frame;
    SpriteAttributes date;
    SpriteAttributes time;
    ClockClipView weatherClip;
    SpriteAttributes weather;
    ClockClipView toolClip;
    SpriteAttributes tool;
    ClockClipView fixedClip;
    unsigned palette;
    unsigned paddedX;
    unsigned paddedY;
    SpriteAttributes icon;
};

typedef char ClockDrawToolOffsetsCheck[
    offsetof(ClockDisplayData, toolIconVisible) == 0x2F1 &&
    offsetof(ClockDisplayData, toolIconId) == 0x2F2 ? 1 : -1];

EC unsigned ClockPaletteIndex(ClockPaletteAllocation *, u32) asm("ResourceHandle_GetIndex");
EC unsigned ClockBaseTile(ClockTileAllocation *, u32) asm("func_08007D4C");
EC void ReadClockAnimationFrame(ClockFrameView *, const ClockAnimation *, unsigned) asm("GetSpriteAnimationDataFrame");

enum
{
    PaletteBytes = 32,
    RowTileBytes = 8 * CLOCK_TILE_BYTES,
    IconTileBytes = 4 * CLOCK_TILE_BYTES,
    FixedIconClip = 5,
    HudInset = 4,
    TimeOffset = 32,
    WeatherOffset = 64,
    ToolOffset = 88,
    OamMosaic = 1 << 12,
    OamHorizontal = 1 << 14,
    OamSize32x16 = 2u << 30,
    OamSize16x16 = 1u << 30
};

// The old compiler needs explicit register and pool placement for this routine.
#define NONZERO_SIGN(value) ((0u - (value)) | (value))
#define PIN_REG(Type, name, reg, value) register Type name asm(reg) = (value); asm("" : "+r"(name))
#define PIN_LOW(Type, name, reg, value) register Type name asm(reg) = (value); asm("" : "+l"(name))
#define ADDRESS(type, name, pointerReg, offsetReg, member) \
    PIN_LOW(u32, name##Offset, offsetReg, offsetof(ClockDisplayData, member)); \
    register type name asm(pointerReg); \
    asm("add %0, %1, %2" : "=l"(name) : "l"(clock), "l"(name##Offset))
#define ADDRESS_SWAP(type, name, pointerReg, offsetReg, member) \
    PIN_LOW(u32, name##Offset, offsetReg, offsetof(ClockDisplayData, member)); \
    register type name asm(pointerReg); \
    asm("add %0, %1, %2" : "=l"(name) : "l"(name##Offset), "l"(clock))
#define LOAD_BYTE(name, pointer) \
    register u32 name asm("r0"); \
    asm volatile("ldrb %0, [%1]" : "=l"(name) : "l"(pointer) : "memory")
#define STORE_BYTE(pointer, value) \
    asm volatile("strb %0, [%1]" : : "l"(value), "l"(pointer) : "memory")
#define BARRIER(m) asm("" : "+m"(m))

#define LITERAL(type, name, reg, label) \
    register type name asm(reg); \
    asm("ldr %0, .Lclock_" #label : "=l"(name))

#define BUILD_ROW_POSITION(row) \
    u32 clearPosition = 0; \
    register u32 clearTile asm("r1") = 0; \
    row.words[0] = clearPosition; \
    row.words[1] = clearTile; \
    BARRIER(row); \
    u32 mosaic = OamMosaic; \
    u32 position = row.words[0] | mosaic; \
    LITERAL(u32, shapeMask, "r1", shape_mask); \
    position &= shapeMask; \
    u32 shape = OamHorizontal; \
    position |= shape; \
    LITERAL(u32, sizeMask, "r1", size_mask); \
    position &= sizeMask; \
    u32 size = OamSize32x16; \
    position |= size; \
    register u32 yCoord asm("r2") = work.paddedY; \
    u32 yPixels = (u8)yCoord; \
    LITERAL(u32, clearY, "r2", clear_y); \
    position = (position & clearY) | yPixels; \
    asm("" : "+l"(position));

#define SET_HUD_TILE_AND_PALETTE(sprite, tile, palette) \
    u32 tileIndex = tile; \
    u32 tileBits = (tileIndex << 22) >> 22; \
    LITERAL(u32, tileMask, "r2", clear_tile); \
    SpriteAttributes * attributes = &sprite; \
    u32 attrs = attributes->words[1]; \
    attrs = (attrs & tileMask) | tileBits; \
    register u32 paletteInput asm("r2") = palette; \
    u32 paletteIndex = (u16)paletteInput; \
    u32 lowNibble = 0xF; \
    register u32 paletteBits asm("r0") = (paletteIndex & lowNibble) << 12; \
    LITERAL(u32, paletteMask, "r2", clear_palette); \
    attributes->words[1] = (attrs & paletteMask) | paletteBits;

#define APPEND_HUD_SPRITE(record, attributes) \
    SpriteOamBuffer * countBuffer = sprites; \
    unsigned count = countBuffer->sprite_count; \
    if (count < ARRAY_COUNT(countBuffer->sprites)) { \
        u32 offset = count * sizeof(SpriteAttributes) + offsetof(SpriteOamBuffer, sprites); \
        asm("add %0, %1" : "+l"(offset) : "h"(sprites)); \
        SpriteAttributes * sprite = (SpriteAttributes *)offset; \
        u32 pos = record.words[0]; \
        sprite->words[0] = pos; \
        u32 tilePalette = ((u16 *)attributes)[2]; \
        ((u16 *)sprite)[2] = tilePalette; \
        unsigned nextCount = count + 1; \
        register SpriteOamBuffer * countToUpdate asm("r1") = sprites; \
        countToUpdate->sprite_count = nextCount; \
    }

EC SECTION(".text.0803C7C8") void DrawClockDisplay(ClockDisplay & display, int x, int y,
    OamBuffer * oam, DialogLayerRenderer * volatile renderer)
{
    ClockDrawWork work;
    register int xOrigin asm("r9") = x;
    register int yOrigin asm("r10") = y;
    register SpriteOamBuffer * sprites asm("r8") = reinterpret_cast<SpriteOamBuffer *>(oam);
    asm("" : "+h"(xOrigin), "+h"(yOrigin), "+h"(sprites));
    PIN_LOW(ClockDisplayData *, clock, "r6", display.inner);
    PIN_LOW(ClockPaletteAllocation *, clockPalette, "r2", &clock->clockPalette);
    u32 paletteHandle = clockPalette->handle;
    if ((i32)NONZERO_SIGN(paletteHandle) >= 0)
        return;
    work.palette = ClockPaletteIndex(clockPalette, paletteHandle);
    PIN_LOW(u8 *, paletteDirty, "r7", &clock->clockPaletteDirty);
    LOAD_BYTE(needPalette, paletteDirty);
    if (needPalette)
    {
        PIN_LOW(ClockUploadTask *, upload, "r4", &clock->clockPaletteUpload);
        u32 previous = (u32)clock->clockPaletteUpload.previous;
        u32 pending = NONZERO_SIGN(previous) >> 31;
        if (!pending)
        {
            ReadClockAnimationFrame(&work.frame, (const ClockAnimation *)clock->weatherAnimation, 0);
            PIN_LOW(const void *, source, "r2", work.frame.palette);
            PIN_LOW(unsigned, destinationIndex, "r1", work.palette);
            PIN_LOW(unsigned, destination, "r0", destinationIndex * CLOCK_TILE_BYTES);
            LITERAL(unsigned, memoryBase, "r1", obj_palette);
            destination += memoryBase;
            asm("" : "+l"(destination));
            PIN_LOW(unsigned, length, "r1", PaletteBytes);
            upload->source = source;
            upload->destination = (void *)destination;
            upload->length = length;
            PIN_LOW(ClockUploadQueue *, queue, "r2", reinterpret_cast<ClockUploadQueue *>(renderer));
            PIN_LOW(UploadAppender, append, "r2", queue->vtable->queueUpload);
            append(reinterpret_cast<ClockUploadQueue *>(renderer), upload);
            STORE_BYTE(paletteDirty, pending);
        }
    }

    // Both rows and the icons share an inset from the window origin.
    work.paddedX = xOrigin + HudInset;
    PIN_LOW(unsigned, nextY, "r1", yOrigin);
    work.paddedY = nextY + HudInset;

    // Date row.
    {
        u8 * firstDateTile = clock->seasonFirstTiles;
        ADDRESS(ClockTileAllocation *, allocation, "r2", "r0", dateTileAllocation);
        u32 handle = allocation->handle;
        if ((i32)NONZERO_SIGN(handle) < 0)
        {
            PIN_REG(unsigned, tile, "r9", ClockBaseTile(allocation, handle));
            ADDRESS(u8 *, dirty, "r5", "r1", dateTilesDirty);
            LOAD_BYTE(updateTiles, dirty);
            if (updateTiles)
            {
                ADDRESS(ClockUploadTask *, upload, "r3", "r2", dateTileUpload);
                u32 previous = (u32)upload->previous;
                u32 pending = NONZERO_SIGN(previous) >> 31;
                if (pending == 0)
                {
                    unsigned destinationIndex = tile;
                    unsigned destination = destinationIndex * CLOCK_TILE_BYTES;
                    LITERAL(unsigned, memoryBase, "r2", obj_tiles);
                    destination += memoryBase;
                    unsigned length = RowTileBytes;
                    upload->source = firstDateTile;
                    upload->destination = (void *)destination;
                    upload->length = length;
                    register ClockUploadQueue * queue asm("r1") = reinterpret_cast<ClockUploadQueue *>(renderer);
                    queue->vtable->queueUpload(queue, upload);
                    STORE_BYTE(dirty, pending);
                }
            }
            BUILD_ROW_POSITION(work.date);
            LITERAL(u32, xMask, "r1", x_mask);
            u32 xCoord = work.paddedX;
            xMask &= xCoord;
            register u32 xBits asm("r1") = xMask << 16;
            LITERAL(u32, clearX, "r2", clear_x);
            position = (position & clearX) | xBits;
            work.date.words[0] = position;
            BARRIER(work.date);
            SET_HUD_TILE_AND_PALETTE(work.date, tile, work.palette);
            APPEND_HUD_SPRITE(work.date, attributes);
        }
    }

    // Time row.
    {
        register u32 firstTimeOffset asm("r2") = offsetof(ClockDisplayData, weekdayFirstTiles);
        asm("add %0, %1" : "+l"(firstTimeOffset) : "l"(clock));
        PIN_REG(u8 *, firstTimeTile, "r9", (u8 *)firstTimeOffset);
        register u32 timeX asm("r5") = work.paddedX + TimeOffset;
        ADDRESS(ClockTileAllocation *, allocation, "r2", "r0", timeTileAllocation);
        u32 handle = allocation->handle;
        if ((i32)NONZERO_SIGN(handle) < 0)
        {
            PIN_REG(unsigned, tile, "r10", ClockBaseTile(allocation, handle));
            ADDRESS(u8 *, dirty, "r7", "r1", tilesDirty);
            LOAD_BYTE(updateTiles, dirty);
            if (updateTiles)
            {
                ADDRESS(ClockUploadTask *, upload, "r3", "r2", timeTileUpload);
                u32 previous = (u32)upload->previous;
                u32 pending = NONZERO_SIGN(previous) >> 31;
                if (pending == 0)
                {
                    unsigned destinationIndex = tile;
                    unsigned destination = destinationIndex * CLOCK_TILE_BYTES;
                    LITERAL(unsigned, memoryBase, "r2", obj_tiles);
                    destination += memoryBase;
                    unsigned length = RowTileBytes;
                    PIN_LOW(const void *, source, "r2", firstTimeTile);
                    upload->source = source;
                    upload->destination = (void *)destination;
                    upload->length = length;
                    register ClockUploadQueue * queue asm("r1") = reinterpret_cast<ClockUploadQueue *>(renderer);
                    queue->vtable->queueUpload(queue, upload);
                    STORE_BYTE(dirty, pending);
                }
            }
            BUILD_ROW_POSITION(work.time);
            LITERAL(u32, xMask, "r1", x_mask);
            timeX &= xMask;
            register u32 xBits asm("r2") = timeX << 16;
            LITERAL(u32, clearX, "r1", clear_x);
            position = (position & clearX) | xBits;
            work.time.words[0] = position;
            BARRIER(work.time);
            SET_HUD_TILE_AND_PALETTE(work.time, tile, work.palette);
            APPEND_HUD_SPRITE(work.time, attributes);
        }
    }

    {
        u32 clearPosition = 0;
        register u32 clearTile asm("r1") = 0;
        work.icon.words[0] = clearPosition;
        work.icon.words[1] = clearTile;
        PIN_LOW(u32, position, "r1", OamMosaic);
        work.icon.words[0] = position;
        BARRIER(work.icon);
        PIN_LOW(u32, size, "r0", OamSize16x16);
        position |= size;
        work.icon.words[0] = position;
        BARRIER(work.icon);
    }
    // Weather icon.
    {
        ADDRESS(ClockTileAllocation *, allocation, "r2", "r1", weatherIconTiles);
        u32 handle = allocation->handle;
        if ((i32)NONZERO_SIGN(handle) < 0)
        {
            PIN_REG(unsigned, tile, "r10", ClockBaseTile(allocation, handle));
            ADDRESS_SWAP(u8 *, dirty, "r2", "r2", weatherIconDirty);
            PIN_REG(u8 *, dirtyHigh, "r9", dirty);
            LOAD_BYTE(updateTiles, dirty);
            if (updateTiles)
            {
                ADDRESS(ClockUploadTask *, upload, "r5", "r0", weatherIconUpload);
                u32 previous = (u32)upload->previous;
                u32 pending = NONZERO_SIGN(previous) >> 31;
                if (pending == 0)
                {
                    ADDRESS(u8 *, clipIdAddress, "r0", "r1", weatherIcon);
                    u32 clipId = *clipIdAddress;
                    const ClockAnimationVtable * sourceVtable = ((const ClockAnimation *)clock)->vtable;
                    register ClockClipView * clipOut asm("r0") = &work.weatherClip;
                    ReadClip getClip = sourceVtable->getClip;
                    const ClockAnimation * clipSource = (const ClockAnimation *)clock;
                    getClip(clipOut, clipSource, clipId);
                    SpriteKeyframe const * keyframes = work.weatherClip.keyframes;
                    u32 frameId = keyframes->frame;
                    register ClockFrameView * frameOut asm("r0") = &work.frame;
                    ReadFrame getFrame = sourceVtable->getFrame;
                    const ClockAnimation * frameSource = (const ClockAnimation *)clock;
                    getFrame(frameOut, frameSource, frameId);
                    const void * source = work.frame.tiles;
                    unsigned destinationIndex = tile;
                    unsigned destination = destinationIndex * CLOCK_TILE_BYTES;
                    LITERAL(unsigned, memoryBase, "r1", obj_tiles);
                    destination += memoryBase;
                    unsigned length = IconTileBytes;
                    upload->source = source;
                    upload->destination = (void *)destination;
                    upload->length = length;
                    register ClockUploadQueue * queue asm("r2") = reinterpret_cast<ClockUploadQueue *>(renderer);
                    UploadAppender append = queue->vtable->queueUpload;
                    append(reinterpret_cast<ClockUploadQueue *>(renderer), upload);
                    u8 * dirtyAddress = dirtyHigh;
                    STORE_BYTE(dirtyAddress, pending);
                }
            }
            register u32 basePosition asm("r1") = work.icon.words[0];
            register u32 baseTile asm("r2") = work.icon.words[1];
            work.weather.words[0] = basePosition;
            work.weather.words[1] = baseTile;
            BARRIER(work.weather);
            register u32 yCoord asm("r2") = work.paddedY;
            u32 yPixels = (u8)yCoord;
            LITERAL(u32, clearY, "r2", clear_y);
            u32 position = work.weather.words[0];
            position = (position & clearY) | yPixels;
            u32 xCoord = work.paddedX + WeatherOffset;
            LITERAL(u32, xMask, "r2", x_mask);
            xCoord &= xMask;
            register u32 xBits asm("r1") = xCoord << 16;
            LITERAL(u32, clearX, "r2", clear_x);
            position = (position & clearX) | xBits;
            work.weather.words[0] = position;
            BARRIER(work.weather);
            SET_HUD_TILE_AND_PALETTE(work.weather, tile, work.palette);
            APPEND_HUD_SPRITE(work.weather, attributes);
        }
    }

    // Held tool icon.
    {
        ADDRESS(ClockTileAllocation *, tiles, "r3", "r2", toolIconTiles);
        u32 handle = tiles->handle;
        if ((i32)NONZERO_SIGN(handle) >= 0)
            return;
        ADDRESS(ClockPaletteAllocation *, palettes, "r4", "r0", toolIconPalette);
        u32 paletteHandle = palettes->handle;
        if ((i32)NONZERO_SIGN(paletteHandle) >= 0)
            return;
        PIN_LOW(u32, basePosition, "r1", work.icon.words[0]);
        PIN_LOW(u32, baseTile, "r2", work.icon.words[1]);
        work.tool.words[0] = basePosition;
        work.tool.words[1] = baseTile;
        BARRIER(work.tool);
        PIN_LOW(u32, yCoord, "r2", work.paddedY);
        PIN_LOW(u32, yPixels, "r1", (u8)yCoord);
        LITERAL(u32, clearY, "r2", clear_y);
        PIN_LOW(u32, position, "r0", work.tool.words[0]);
        position = (position & clearY) | yPixels;
        asm("" : "+l"(position));
        PIN_LOW(u32, xCoord, "r1", work.paddedX + ToolOffset);
        LITERAL(u32, xMask, "r2", x_mask);
        xCoord &= xMask;
        asm("" : "+l"(xCoord));
        PIN_LOW(u32, xBits, "r1", xCoord << 16);
        LITERAL(u32, clearX, "r2", clear_x);
        position = (position & clearX) | xBits;
        asm("" : "+l"(position));
        work.tool.words[0] = position;
        BARRIER(work.tool);
        LITERAL(u32, visibleOffset, "r1", tool_visible);
        register u8 * visibility asm("r0");
        asm("add %0, %1, %2" : "=l"(visibility) : "l"(clock), "l"(visibleOffset));
        LOAD_BYTE(toolVisible, visibility);
        if (toolVisible)
        {
            PIN_REG(unsigned, tile, "r9", ClockBaseTile(tiles, tiles->handle));
            PIN_REG(unsigned, palette, "r10", ClockPaletteIndex(palettes, palettes->handle));
            ADDRESS(u8 *, toolDirty, "r7", "r2", toolIconDirty);
            LOAD_BYTE(updateTool, toolDirty);
            if (updateTool)
            {
                LITERAL(u32, idOffset, "r1", tool_id);
                register u16 * clipIdAddress asm("r0");
                asm("add %0, %1, %2" : "=l"(clipIdAddress) : "l"(clock), "l"(idOffset));
                PIN_LOW(u32, clipId, "r2", *clipIdAddress);
                PIN_LOW(const ClockAnimation *, toolAnimation, "r5", (const ClockAnimation *)clock->toolAnimation);
                PIN_LOW(const ClockAnimationVtable *, sourceVtable, "r4", ((const ClockAnimation *)clock->toolAnimation)->vtable);
                register ClockClipView * clipOut asm("r0") = &work.toolClip;
                PIN_LOW(ReadClip, getClip, "r3", sourceVtable->getClip);
                PIN_LOW(const ClockAnimation *, clipSource, "r1", toolAnimation);
                getClip(clipOut, clipSource, clipId);
                PIN_LOW(SpriteKeyframe const *, keyframes, "r0", work.toolClip.keyframes);
                PIN_LOW(u32, frameId, "r2", keyframes->frame);
                PIN_LOW(ClockFrameView *, frameOut, "r0", &work.frame);
                PIN_LOW(ReadFrame, getFrame, "r3", sourceVtable->getFrame);
                PIN_LOW(const ClockAnimation *, frameSource, "r1", toolAnimation);
                getFrame(frameOut, frameSource, frameId);
                {
                    ADDRESS(ClockUploadTask *, upload, "r3", "r2", toolIconUpload);
                    u32 previous = (u32) upload->previous;
                    if ((i32)NONZERO_SIGN(previous) >= 0)
                    {
                        PIN_LOW(const void *, source, "r2", work.frame.tiles);
                        PIN_LOW(unsigned, destinationIndex, "r1", tile);
                        PIN_LOW(unsigned, destination, "r0", destinationIndex * CLOCK_TILE_BYTES);
                        LITERAL(unsigned, memoryBase, "r1", obj_tiles);
                        destination += memoryBase;
                        asm("" : "+l"(destination));
                        PIN_LOW(unsigned, length, "r1", IconTileBytes);
                        upload->source = source;
                        upload->destination = (void *)destination;
                        upload->length = length;
                        PIN_LOW(ClockUploadQueue *, queue, "r2", reinterpret_cast<ClockUploadQueue *>(renderer));
                        register const ClockUploadQueueVtable * uploadVtable asm("r0") = queue->vtable;
                        // The early Thumb loads need a pool within PC-relative range.
                        asm volatile("b .Lclock_after_literals\n\t.align 2, 0\n"
                            ".Lclock_obj_palette: .4byte 0x05000200\n"
                            ".Lclock_obj_tiles: .4byte 0x06010000\n"
                            ".Lclock_shape_mask: .4byte 0xFFFF3FFF\n"
                            ".Lclock_size_mask: .4byte 0x3FFFFFFF\n"
                            ".Lclock_clear_y: .4byte 0xFFFFFF00\n"
                            ".Lclock_x_mask: .4byte 0x000001FF\n"
                            ".Lclock_clear_x: .4byte 0xFE00FFFF\n"
                            ".Lclock_clear_tile: .4byte 0xFFFFFC00\n"
                            ".Lclock_clear_palette: .4byte 0xFFFF0FFF\n"
                            ".Lclock_tool_visible: .4byte 0x000002F1\n"
                            ".Lclock_tool_id: .4byte 0x000002F2\n"
                            ".Lclock_after_literals:" : "+l"(uploadVtable));
                        PIN_LOW(UploadAppender, append, "r2", uploadVtable->queueUpload);
                        append(reinterpret_cast<ClockUploadQueue *>(renderer), upload);
                    }
                }
                {
                    ADDRESS(ClockUploadTask *, upload, "r3", "r0", toolIconPaletteUpload);
                    u32 previous = (u32) upload->previous;
                    if ((i32)NONZERO_SIGN(previous) >= 0)
                    {
                        PIN_LOW(const void *, source, "r2", work.frame.palette);
                        PIN_LOW(unsigned, destinationIndex, "r1", palette);
                        PIN_LOW(unsigned, destination, "r0", destinationIndex * CLOCK_TILE_BYTES);
                        LITERAL(unsigned, memoryBase, "r1", obj_palette_tail);
                        destination += memoryBase;
                        asm("" : "+l"(destination));
                        PIN_LOW(unsigned, length, "r1", PaletteBytes);
                        upload->source = source;
                        upload->destination = (void *)destination;
                        upload->length = length;
                        PIN_LOW(ClockUploadQueue *, queue, "r2", reinterpret_cast<ClockUploadQueue *>(renderer));
                        PIN_LOW(UploadAppender, append, "r2", queue->vtable->queueUpload);
                        append(reinterpret_cast<ClockUploadQueue *>(renderer), upload);
                    }
                }
                PIN_LOW(u32, cleanTool, "r0", 0);
                STORE_BYTE(toolDirty, cleanTool);
            }
            PIN_LOW(u32, tileIndex, "r1", tile);
            PIN_LOW(u32, tileBits, "r0", (tileIndex << 22) >> 22);
            LITERAL(u32, tileMask, "r2", clear_tile_tail);
            PIN_LOW(SpriteAttributes *, attributes, "r3", &work.tool);
            PIN_LOW(u32, attrs, "r1", attributes->words[1]);
            attrs = (attrs & tileMask) | tileBits;
            asm("" : "+l"(attrs));
            PIN_LOW(u32, paletteInput, "r2", palette);
            PIN_LOW(u32, paletteIndex, "r0", (u16)paletteInput);
            PIN_LOW(u32, lowNibble, "r2", 0xF);
            PIN_LOW(u32, paletteBits, "r0", (paletteIndex & lowNibble) << 12);
            LITERAL(u32, paletteMask, "r2", clear_palette_tail);
            attributes->words[1] = (attrs & paletteMask) | paletteBits;
            asm("" : : "l"(attributes), "m"(*attributes));
            PIN_LOW(SpriteOamBuffer *, countBuffer, "r0", sprites);
            PIN_LOW(unsigned, count, "r2", countBuffer->sprite_count);
            if (count < ARRAY_COUNT(countBuffer->sprites))
            {
                PIN_LOW(u32, offset, "r0", count * sizeof(SpriteAttributes) + offsetof(SpriteOamBuffer, sprites));
                asm("add %0, %1" : "+l"(offset) : "h"(sprites));
                SpriteAttributes * sprite = (SpriteAttributes *)offset;
                PIN_LOW(u32, pos, "r1", work.tool.words[0]);
                sprite->words[0] = pos;
                PIN_LOW(u32, tilePalette, "r1", ((u16 *)attributes)[2]);
                ((u16 *)sprite)[2] = tilePalette;
                PIN_LOW(unsigned, nextCount, "r0", count + 1);
                PIN_LOW(SpriteOamBuffer *, countToUpdate, "r1", sprites);
                countToUpdate->sprite_count = nextCount;
            }
        }
    }
    {
        ADDRESS(ClockTileAllocation *, allocation, "r2", "r0", fixedIconTiles);
        u32 handle = allocation->handle;
        if ((i32)NONZERO_SIGN(handle) < 0)
        {
            PIN_REG(unsigned, tile, "r7", ClockBaseTile(allocation, handle));
            ADDRESS_SWAP(u8 *, dirty, "r1", "r1", fixedIconDirty);
            PIN_REG(u8 *, dirtyHigh, "r10", dirty);
            LOAD_BYTE(updateTiles, dirty);
            if (updateTiles)
            {
                ADDRESS(ClockUploadTask *, upload, "r5", "r2", fixedIconUpload);
                u32 previous = (u32)upload->previous;
                PIN_LOW(u32, pending, "r0", NONZERO_SIGN(previous) >> 31);
                PIN_REG(u32, pendingSaved, "r9", pending);
                if (pending == 0)
                {
                    PIN_LOW(const ClockAnimationVtable *, sourceVtable, "r4", ((const ClockAnimation *)clock)->vtable);
                    register ClockClipView * clipOut asm("r0") = &work.fixedClip;
                    PIN_LOW(ReadClip, getClip, "r3", sourceVtable->getClip);
                    PIN_LOW(const ClockAnimation *, clipSource, "r1", (const ClockAnimation *)clock);
                    PIN_LOW(u32, clipId, "r2", FixedIconClip);
                    getClip(clipOut, clipSource, clipId);
                    PIN_LOW(SpriteKeyframe const *, keyframes, "r0", work.fixedClip.keyframes);
                    PIN_LOW(u32, frameId, "r2", keyframes->frame);
                    PIN_LOW(ClockFrameView *, frameOut, "r0", &work.frame);
                    PIN_LOW(ReadFrame, getFrame, "r3", sourceVtable->getFrame);
                    PIN_LOW(const ClockAnimation *, frameSource, "r1", (const ClockAnimation *)clock);
                    getFrame(frameOut, frameSource, frameId);
                    PIN_LOW(const void *, source, "r2", work.frame.tiles);
                    register unsigned destination asm("r0");
                    asm("lsl %0, %1, #5" : "=l"(destination) : "l"(tile));
                    LITERAL(unsigned, memoryBase, "r1", obj_tiles_tail);
                    destination += memoryBase;
                    asm("" : "+l"(destination));
                    PIN_LOW(unsigned, length, "r1", IconTileBytes);
                    upload->source = source;
                    upload->destination = (void *)destination;
                    upload->length = length;
                    PIN_LOW(ClockUploadQueue *, queue, "r2", reinterpret_cast<ClockUploadQueue *>(renderer));
                    PIN_LOW(UploadAppender, append, "r2", queue->vtable->queueUpload);
                    append(reinterpret_cast<ClockUploadQueue *>(renderer), upload);
                    PIN_LOW(u32, pendingToStore, "r1", pendingSaved);
                    PIN_LOW(u8 *, dirtyAddress, "r0", dirtyHigh);
                    STORE_BYTE(dirtyAddress, pendingToStore);
                }
            }

            // The fixed icon shares the tool position and uses the clock palette.
            PIN_LOW(SpriteAttributes *, attributes, "r3", &work.tool);
            LITERAL(u32, tileMask, "r2", tile_mask_tail);
            PIN_LOW(u32, tileMaskCopy, "r0", tileMask);
            tile &= tileMaskCopy;
            asm("" : "+l"(tile));
            PIN_LOW(u32, oldTile, "r1", ((u16 *)attributes)[2]);
            LITERAL(u32, otherBits, "r0", clear_tile_tail);
            PIN_LOW(u32, replacementTile, "r0", (otherBits & oldTile) | tile);
            ((u16 *)attributes)[2] = replacementTile;
            asm("" : : "l"(attributes), "m"(*attributes));
            PIN_LOW(u32, paletteValue, "r0", work.palette);
            PIN_LOW(u32, newPaletteByte, "r2", paletteValue << 4);
            PIN_LOW(u32, oldPaletteByte, "r1", ((u8 *)attributes)[5]);
            PIN_LOW(u32, lowNibble, "r0", 0xF);
            PIN_LOW(u32, replacementPaletteByte, "r0", (lowNibble & oldPaletteByte) | newPaletteByte);
            ((u8 *)attributes)[5] = replacementPaletteByte;
            asm("" : : "l"(attributes), "m"(*attributes));
            PIN_LOW(SpriteOamBuffer *, countBuffer, "r1", sprites);
            PIN_LOW(unsigned, count, "r2", countBuffer->sprite_count);
            if (count < ARRAY_COUNT(countBuffer->sprites))
            {
                PIN_LOW(u32, offset, "r0", count * sizeof(SpriteAttributes) + offsetof(SpriteOamBuffer, sprites));
                asm("add %0, %1" : "+l"(offset) : "h"(sprites));
                SpriteAttributes * sprite = (SpriteAttributes *)offset;
                PIN_LOW(u32, pos, "r1", work.tool.words[0]);
                sprite->words[0] = pos;
                PIN_LOW(u32, tilePalette, "r1", ((u16 *)attributes)[2]);
                ((u16 *)sprite)[2] = tilePalette;
                PIN_LOW(unsigned, nextCount, "r0", count + 1);
                PIN_LOW(SpriteOamBuffer *, countToUpdate, "r2", sprites);
                countToUpdate->sprite_count = nextCount;
            }
        }
    }
}

// Keep the tail pool after the return, outside the generated function body.
asm(".pushsection .text.0803C7C8, \"ax\", %progbits\n"
    ".align 2, 0\n"
    ".Lclock_obj_palette_tail: .4byte 0x05000200\n"
    ".Lclock_clear_tile_tail: .4byte 0xFFFFFC00\n"
    ".Lclock_clear_palette_tail: .4byte 0xFFFF0FFF\n"
    ".Lclock_obj_tiles_tail: .4byte 0x06010000\n"
    ".Lclock_tile_mask_tail: .4byte 0x000003FF\n"
    ".popsection");
