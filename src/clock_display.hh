#ifndef CLOCK_DISPLAY_HH
#define CLOCK_DISPLAY_HH

#include "prelude.h"

enum { CLOCK_TILE_BYTES = 32 };
typedef u8 ClockTile[CLOCK_TILE_BYTES];

struct ClockPaletteAllocation
{
    STRUCT_PAD(0, 4);
    u32 handle;
};

struct ClockTileAllocation
{
    STRUCT_PAD(0, 4);
    u32 handle;
};

struct ClockUploadTask
{
    ClockUploadTask * previous;
    ClockUploadTask * next;
    void const * vtable;
    void const * source;
    void * destination;
    unsigned length;
};

struct ClockDisplayData
{
    u8 weatherAnimation[0x30];
    u8 toolAnimation[0x30];

    ClockPaletteAllocation clockPalette;
    ClockUploadTask clockPaletteUpload;
    u8 clockPaletteDirty;
    STRUCT_PAD(0x81, 0x84);

    ClockTile seasonFirstTiles;
    ClockTile seasonSecondTiles;
    ClockTile dayTensTiles;
    ClockTile dayOnesTiles;
    ClockTile meridiemTiles;
    ClockTile meridiemSuffixTiles;
    ClockTile hourTensTiles;
    ClockTile hourOnesTiles;
    ClockTileAllocation dateTileAllocation;
    u8 dateTilesDirty;
    ClockUploadTask dateTileUpload;

    ClockTile weekdayFirstTiles;
    ClockTile weekdaySecondTiles;
    ClockTile weekdayThirdTiles;
    ClockTile weekdayGapTiles;
    ClockTile colonTiles;
    ClockTile minuteTensTiles;
    ClockTile minuteOnesTiles;
    ClockTile timeRowEndTiles;
    ClockTileAllocation timeTileAllocation;
    u8 tilesDirty;
    ClockUploadTask timeTileUpload;

    u8 weatherIcon;
    ClockTileAllocation weatherIconTiles;
    ClockUploadTask weatherIconUpload;
    u8 weatherIconDirty;
    u8 toolIconVisible;
    u16 toolIconId;

    ClockPaletteAllocation toolIconPalette;
    ClockUploadTask toolIconPaletteUpload;
    ClockTileAllocation toolIconTiles;
    ClockUploadTask toolIconUpload;
    u8 toolIconDirty;

    ClockTileAllocation fixedIconTiles;
    ClockUploadTask fixedIconUpload;
    u8 fixedIconDirty;
};

struct ClockDisplay
{
    ClockDisplayData * inner;
};

struct OamBuffer;
struct DialogLayerRenderer;

EC ClockTile const gClockFont[];
EC ClockDisplay & InitializeClockDisplay(ClockDisplay & display);
EC void DestroyClockDisplay(ClockDisplay & display, int destructorFlags);
EC void DrawClockDisplay(ClockDisplay & display, int x, int y, OamBuffer *, DialogLayerRenderer *);

#endif // CLOCK_DISPLAY_HH
