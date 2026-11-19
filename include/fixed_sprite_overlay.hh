#ifndef FIXED_SPRITE_OVERLAY_HH
#define FIXED_SPRITE_OVERLAY_HH

#include "prelude.h"

struct OverlaySpriteResources;

struct OverlaySpriteResourceVtable
{
    STRUCT_PAD(0x00, 0x4C);
    void (*releasePalette)(OverlaySpriteResources *, unsigned);
};

struct OverlaySpriteResources
{
    OverlaySpriteResourceVtable const * vtable;
};

struct OverlayTileAllocation
{
    STRUCT_PAD(0x00, 0x04);
    u32 handle;
};

struct OverlayPaletteTable
{
    unsigned count;
    u8 ids[16];
};

struct FixedSpriteOverlay
{
    struct Frame
    {
        STRUCT_PAD(0x00, 0x20);
        OverlayTileAllocation tiles;
        u16 baseTile;
    };

    OverlaySpriteResources * resources;
    Frame frames[3];
    OverlayPaletteTable palettes;
    STRUCT_PAD(0x9C, 0xA0);
};

#endif // FIXED_SPRITE_OVERLAY_HH
