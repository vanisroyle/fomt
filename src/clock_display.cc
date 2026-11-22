#include "clock_display.hh"
#include "sprite_animation.hh"

#include <string.h>

#define MATCH(addr) EC SECTION(".text." #addr)
EC void * __builtin_new(size_t);
EC char const gClockWeatherSprites[] asm("gUnk_0875B444");
EC char const gClockToolSprites[] asm("gUnk_086678A0");
EC char const gClockUploadTaskVTable[] asm("vtable_unk_080E77D0");
enum { MERIDIEM_SUFFIX_FONT_TILE = 29 };

EC void * CopyClockTileBytes(void *, void const *, size_t) asm("memcpy");

EC SpriteAnimationData * InitSpriteAnimationData(SpriteAnimationData *, void const *);

struct ResourceHandleOwner;
struct TileHeapOwner;

EC ResourceHandleOwner * ResourceHandles_Initialize(ResourceHandleOwner *);
EC u32 AllocateClockPalette(ResourceHandleOwner *) asm("func_0800736C");
EC TileHeapOwner * InitializeClockTileHeap(TileHeapOwner *) asm("TileHeapOwner_Initialize");
EC u32 AllocateClockTiles(TileHeapOwner *, u32) asm("TileHandle_Allocate");

EC void DestroyClockDisplayData(ClockDisplayData *, int);

typedef char ClockDisplayDataSizeCheck[sizeof(ClockDisplayData) == 0x35C ? 1 : -1];
typedef char ClockDateTilesOffsetCheck[offsetof(ClockDisplayData, seasonFirstTiles) == 0x84 ? 1 : -1];
typedef char ClockTimeTilesOffsetCheck[offsetof(ClockDisplayData, weekdayFirstTiles) == 0x1A8 ? 1 : -1];
typedef char ClockToolVisibleOffsetCheck[offsetof(ClockDisplayData, toolIconVisible) == 0x2F1 ? 1 : -1];

// Keep address formation and register allocation identical to the original Thumb code.
#define OFFSET_ADDRESS(Type, address, addrReg, fieldOffset, offReg, member, first, second) \
    register unsigned fieldOffset asm(#offReg) = offsetof(ClockDisplayData, member); \
    asm("" : "+l"(fieldOffset)); \
    register Type * address asm(#addrReg); \
    asm(".syntax unified\n\tadds %0, %1, %2\n\t.syntax divided" \
        : "=l"(address) : "l"(first), "l"(second))

#define ALLOCATE_CLOCK_TILES(member, offReg, order) \
    do { \
        OFFSET_ADDRESS(ClockTileAllocation, resource, r5, offset, offReg, member, graphics, offset); \
        InitializeClockTileHeap(reinterpret_cast<TileHeapOwner *>(resource)); \
        resource->handle = AllocateClockTiles(reinterpret_cast<TileHeapOwner *>(resource), order); \
    } while (false)

#define TASK_CONTENTS(task, table, tableReg) \
    do { \
        (task)->previous = nullptr; \
        (task)->next = nullptr; \
        register void const * tableValue asm(#tableReg); \
        asm("mov %0, %1" : "=l"(tableValue) : "r"(table)); \
        (task)->vtable = tableValue; \
        (task)->source = nullptr; \
        (task)->destination = nullptr; \
        (task)->length = 0; \
    } while (false)

#define CLOCK_UPLOAD_TASK(member, offReg, tableReg) \
    do { \
        OFFSET_ADDRESS(ClockUploadTask, task, r0, offset, offReg, member, graphics, offset); \
        TASK_CONTENTS(task, vtable, tableReg); \
    } while (false)

#define FLAGGED_CLOCK_UPLOAD_TASK(member, flag, offReg, tableReg, flagValue) \
    do { \
        OFFSET_ADDRESS(ClockUploadTask, task, r0, offset, offReg, member, graphics, offset); \
        TASK_CONTENTS(task, vtable, tableReg); \
        offset += offsetof(ClockDisplayData, flag) - offsetof(ClockDisplayData, member); \
        asm("" : "+l"(offset)); \
        register u8 * dirty asm("r0"); \
        asm(".syntax unified\n\tadds %0, %1, %2\n\t.syntax divided" \
            : "=l"(dirty) : "l"(graphics), "l"(offset)); \
        *dirty = flagValue; \
    } while (false)

MATCH(0803C5B0) ClockDisplay & InitializeClockDisplay(ClockDisplay & display)
{
    ClockDisplayData * graphics = static_cast<ClockDisplayData *>(__builtin_new(sizeof(ClockDisplayData)));
    InitSpriteAnimationData(reinterpret_cast<SpriteAnimationData *>(graphics->weatherAnimation), gClockWeatherSprites);
    InitSpriteAnimationData(reinterpret_cast<SpriteAnimationData *>(graphics->toolAnimation), gClockToolSprites);
    register ClockPaletteAllocation * clockPalette asm("r4") = &graphics->clockPalette;
    asm("" : "+l"(clockPalette));
    ResourceHandles_Initialize(reinterpret_cast<ResourceHandleOwner *>(clockPalette));
    clockPalette->handle = AllocateClockPalette(reinterpret_cast<ResourceHandleOwner *>(clockPalette));
    register ClockUploadTask * paletteTask asm("r0") = &graphics->clockPaletteUpload;
    asm("" : "+l"(paletteTask));
    graphics->clockPaletteUpload.previous = nullptr;
    paletteTask->next = nullptr;
    register void const * vtable asm("r8");
    register void const * uploadVTable asm("r1") = gClockUploadTaskVTable;
    asm("mov %0, %1" : "=r"(vtable), "+l"(uploadVTable));
    paletteTask->vtable = uploadVTable;
    paletteTask->source = nullptr;
    paletteTask->destination = nullptr;
    paletteTask->length = 0;
    register u8 * paletteDirty asm("r0") = reinterpret_cast<u8 *>(paletteTask) + sizeof(ClockUploadTask);
    asm("" : "+l"(paletteDirty));
    register u32 needsUpload asm("r6") = 1;
    asm("" : "+l"(needsUpload));
    *paletteDirty = needsUpload;

    ALLOCATE_CLOCK_TILES(dateTileAllocation, r2, 3);
    register u8 * dateDirty asm("r10");
    OFFSET_ADDRESS(u8, initialDateDirty, r0, dateDirtyOffset, r0, dateTilesDirty, dateDirtyOffset, graphics);
    asm("mov %0, %1" : "=r"(dateDirty), "+l"(initialDateDirty));
    *initialDateDirty = needsUpload;
    CLOCK_UPLOAD_TASK(dateTileUpload, r1, r2);
    ALLOCATE_CLOCK_TILES(timeTileAllocation, r0, 3);
    register u8 * timeDirty asm("r9");
    OFFSET_ADDRESS(u8, initialTimeDirty, r1, timeDirtyOffset, r1, tilesDirty, timeDirtyOffset, graphics);
    asm("mov %0, %1" : "=r"(timeDirty), "+l"(initialTimeDirty));
    *initialTimeDirty = needsUpload;
    FLAGGED_CLOCK_UPLOAD_TASK(timeTileUpload, weatherIcon, r2, r1, 0);

    ALLOCATE_CLOCK_TILES(weatherIconTiles, r0, 2);
    FLAGGED_CLOCK_UPLOAD_TASK(weatherIconUpload, weatherIconDirty, r1, r2, needsUpload);
    register unsigned toolVisibilityOffset asm("r2") = offsetof(ClockDisplayData, toolIconVisible);
    asm("" : "+l"(toolVisibilityOffset));
    register u8 * toolVisibility asm("r0") = reinterpret_cast<u8 *>(graphics) + toolVisibilityOffset;
    asm("" : "+l"(toolVisibility));
    *toolVisibility = 0;
    register ClockPaletteAllocation * toolIconPalette asm("r5") = &graphics->toolIconPalette;
    asm("" : "+l"(toolIconPalette));
    ResourceHandles_Initialize(reinterpret_cast<ResourceHandleOwner *>(toolIconPalette));
    toolIconPalette->handle = AllocateClockPalette(reinterpret_cast<ResourceHandleOwner *>(toolIconPalette));
    CLOCK_UPLOAD_TASK(toolIconPaletteUpload, r1, r2);
    ALLOCATE_CLOCK_TILES(toolIconTiles, r0, 2);
    FLAGGED_CLOCK_UPLOAD_TASK(toolIconUpload, toolIconDirty, r1, r2, needsUpload);
    ALLOCATE_CLOCK_TILES(fixedIconTiles, r2, 2);
    FLAGGED_CLOCK_UPLOAD_TASK(fixedIconUpload, fixedIconDirty, r1, r2, needsUpload);

    register u8 const * font asm("r1") = gClockFont[0];
    asm("" : "+l"(font));
    register unsigned fontTileOffset asm("r2") = MERIDIEM_SUFFIX_FONT_TILE * CLOCK_TILE_BYTES;
    asm("" : "+l"(fontTileOffset));
    register u8 const * meridiemSuffixTile asm("r1") = font + fontTileOffset;
    asm("" : "+l"(meridiemSuffixTile));
    register unsigned meridiemSuffixOffset asm("r2") = offsetof(ClockDisplayData, meridiemSuffixTiles);
    asm("" : "+l"(meridiemSuffixOffset));
    CopyClockTileBytes(reinterpret_cast<u8 *>(graphics) + meridiemSuffixOffset, meridiemSuffixTile, CLOCK_TILE_BYTES);
    *dateDirty = needsUpload;
    u8 blankTile[CLOCK_TILE_BYTES];
    memset(blankTile, 0, sizeof(blankTile));
    register unsigned timeRowEndOffset asm("r1") = offsetof(ClockDisplayData, timeRowEndTiles);
    asm("" : "+l"(timeRowEndOffset));
    CopyClockTileBytes(reinterpret_cast<u8 *>(graphics) + timeRowEndOffset, blankTile, sizeof(blankTile));
    register u8 * timeDirtyState asm("r2") = timeDirty;
    asm("" : "+l"(timeDirtyState));
    *timeDirtyState = needsUpload;
    CopyClockTileBytes(graphics->weekdayGapTiles, blankTile, sizeof(blankTile));
    timeDirtyState = timeDirty;
    asm("" : "+l"(timeDirtyState));
    *timeDirtyState = needsUpload;
    register ClockDisplay * result asm("r0") = &display;
    asm("" : "+l"(result));
    result->inner = graphics;
    return *result;
}

MATCH(0803C7A0) void DestroyClockDisplay(ClockDisplay & display, int destructorFlags)
{
    ClockDisplayData * graphics = display.inner;
    if (graphics != nullptr)
        DestroyClockDisplayData(graphics, 3);
    destructorFlags &= 1;
    if (destructorFlags != 0)
        ::operator delete(&display);
}
