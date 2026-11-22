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
    STRUCT_PAD(0x20, 0x48);
    unsigned (*reservePalette)(MapOverlayScene *, unsigned);
    void (*releasePalette)(MapOverlayScene *, unsigned);
    STRUCT_PAD(0x50, 0x54);
    void (*setObjectPalette)(MapOverlayScene *, u16 const * colors, int slot, u8 overrideParameter);
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
        FixedOverlayFrameView view;
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
    u8 uploadPending;
    u8 drawHalf;
    STRUCT_PAD(0xE7, 0xE8);
    struct ParticleList
    {
        unsigned count;
        MapOverlayParticle entries[32];
    } particles;
};

EC GameStateMapOverlay * InitializeGameStateMapOverlay(GameStateMapOverlay *, MapOverlayScene *,
    SpriteAnimationSource const *, u32 kind, unsigned particleCount);
EC void DestroyGameStateMapOverlay(GameStateMapOverlay *, unsigned destructorFlags);
EC void QueueGameStateMapOverlayGraphics(GameStateMapOverlay &, DmaTransferQueue &);

typedef char MapOverlayFrameSizeCheck[sizeof(GameStateMapOverlay::Frame) == 0x28 ? 1 : -1];
typedef char MapOverlaySizeCheck[sizeof(GameStateMapOverlay) == 0x46C ? 1 : -1];

#endif // MAP_OVERLAY_HH
