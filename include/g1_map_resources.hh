#ifndef G1_MAP_RESOURCES_HH
#define G1_MAP_RESOURCES_HH

#include "prelude.h"

struct G1MapGraphics;

struct G1MapGraphicsVTable
{
    void (*reserved[18])();
    u32 (*allocate_palette)(G1MapGraphics *, u32);
    void (*release_palette)(G1MapGraphics *, u32);
    void (*reserved_50)();
    void (*load_palette)(G1MapGraphics *, void const *, u32, u8);
};

struct G1MapGraphics
{
    G1MapGraphicsVTable const * vtable;
};

struct G1MapTileOwner
{
    void const * vtable;
    u32 handle;
};

struct G1MapSpriteResource
{
    G1MapGraphics * graphics;
    G1MapTileOwner tiles;
    u16 tile_index;
    u16 reserved;
    u32 palette_count;
    u8 palettes[16];
    void const * vtable;
};

struct G1MapResourceData
{
    void const * data;
    u16 size;
    u16 reserved;
};

#endif
