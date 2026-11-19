#include "fixed_sprite_overlay.hh"

struct OamBuffer;

EC unsigned QueueOverlayFrameSprites(OamBuffer & sprites, int x, int y, unsigned priorityMap, unsigned sortKey,
    FixedSpriteOverlay::Frame const & frame, OverlaySpriteResources * resources, unsigned baseTile,
    OverlayPaletteTable const & palettes) asm("func_030004DC");

EC SECTION(".text.0803AE58") void DrawFixedSpriteOverlay(FixedSpriteOverlay const & overlay, OamBuffer & sprites,
    int x, int y, unsigned frameId)
{
    // Preserve the argument setup for the original long call into IWRAM.
    register FixedSpriteOverlay::Frame const * frame asm("r4") = &overlay.frames[frameId];
    asm("" : "+r"(frame));
    register unsigned baseTile asm("r8") = frame->baseTile;
    asm("" : "+r"(baseTile));
    register unsigned sortKey asm("r5") = 0x8000;
    asm("" : "+r"(sortKey) : "r"(baseTile), "r"(frame));
    OverlaySpriteResources * resources = overlay.resources;
    OverlayPaletteTable const & palettes = overlay.palettes;
    unsigned const priorityMap = 2 | (2 << 2) | (2 << 4) | (2 << 6);
    QueueOverlayFrameSprites(sprites, x, y, priorityMap, sortKey, *frame, resources, baseTile, palettes);
}
