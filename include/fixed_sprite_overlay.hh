#ifndef FIXED_SPRITE_OVERLAY_HH
#define FIXED_SPRITE_OVERLAY_HH

#include "prelude.h"
#include "sprite_animation.hh"

struct OverlaySpriteResources;
struct FixedOverlayAnimation;

struct OverlaySpriteResourceVtable
{
    STRUCT_PAD(0x00, 0x48);
    unsigned (*reservePalette)(OverlaySpriteResources *, unsigned);
    void (*releasePalette)(OverlaySpriteResources *, unsigned);
    STRUCT_PAD(0x50, 0x68);
    FixedOverlayAnimation * (*getAnimationSlot0)(OverlaySpriteResources *);
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

template <typename T>
struct FixedOverlayFrameSlice
{
    T const * data;
    u16 length;
};

// The archive writes this borrowed frame description into uninitialized storage.
struct FixedOverlayFrameView
{
    FixedOverlayFrameSlice<SpriteFrameObject> objects;
    FixedOverlayFrameSlice<u8> tileBytes;
    FixedOverlayFrameSlice<u16> paletteBytes;
    FixedOverlayFrameSlice<SpriteFrameExtra> extras;
};

struct FixedSpriteOverlay
{
    struct Frame
    {
        FixedOverlayFrameView view;
        OverlayTileAllocation tiles;
        u16 baseTile;
    };

    OverlaySpriteResources * resources;
    Frame frames[3];
    OverlayPaletteTable palettes;
    u8 uploadPending;
};

EC FixedSpriteOverlay * InitializeFixedSpriteOverlay(void *, OverlaySpriteResources *);
EC void DestroyFixedSpriteOverlay(FixedSpriteOverlay *, unsigned);

typedef char FixedOverlayFrameSizeCheck[sizeof(FixedSpriteOverlay::Frame) == 0x2C ? 1 : -1];
typedef char FixedOverlaySizeCheck[sizeof(FixedSpriteOverlay) == 0xA0 ? 1 : -1];

#endif // FIXED_SPRITE_OVERLAY_HH
