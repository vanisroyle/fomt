#ifndef MAP_OVERLAY_HH
#define MAP_OVERLAY_HH

#include "fixed_sprite_overlay.hh"
#include "sprite_animation.hh"

struct MapOverlayScene;

struct MapOverlaySceneVtable
{
    STRUCT_PAD(0x00, 0x18);
    int (*getCameraX)(MapOverlayScene *);
    int (*getCameraY)(MapOverlayScene *);
    STRUCT_PAD(0x20, 0x4C);
    void (*releasePalette)(MapOverlayScene *, unsigned);
};

struct MapOverlayScene
{
    MapOverlaySceneVtable const * vtable;
};

struct MapOverlayParticle
{
    SpriteAnimator animation;
    u32 xQ16;
    u32 yQ16;
};

struct GameStateMapOverlay
{
    struct Frame
    {
        STRUCT_PAD(0x00, 0x20);
        OverlayTileAllocation tiles;
    };

    MapOverlayScene * scene;
    struct FrameList
    {
        unsigned count;
        Frame entries[5];
    } frames;
    OverlayPaletteTable palettes;
    u8 kind;
    u8 unk_E5;
    u8 drawHalf;
    STRUCT_PAD(0xE7, 0xE8);
    struct ParticleList
    {
        unsigned count;
        MapOverlayParticle entries[32];
    } particles;
};

#endif // MAP_OVERLAY_HH
