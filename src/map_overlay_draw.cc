#include "map_overlay.hh"

struct OamBuffer;

EC unsigned QueueOverlayParticleSprites(OamBuffer & sprites, int x, int y, unsigned priorityMap, unsigned sortKey,
    GameStateMapOverlay::Frame const & frame, MapOverlayScene * scene, unsigned baseTile,
    OverlayPaletteTable const & palettes) asm("func_030004DC");

EC SECTION(".text.0803B368") void DrawMapOverlayParticles(GameStateMapOverlay & input, OamBuffer & sprites)
{
    // Register constraints preserve agbcc's original Thumb code.
    register GameStateMapOverlay * overlay asm("r9") = &input;
    OamBuffer * target = &sprites;
    asm("" : "+l"(target) : "r"(overlay));

    unsigned count = input.particles.count;
    register unsigned half asm("r3") = count / 2;
    asm("" : "+r"(half));
    unsigned start;
    register unsigned end asm("r10");
    unsigned nextHalf;
    register u8 * drawHalf asm("r2") = &overlay->drawHalf;
    asm("" : "+r"(drawHalf) : "r"(half));
    switch (*drawHalf)
    {
    case 0:
    default:
        start = 0;
        end = half;
        nextHalf = 1;
        break;
    case 1:
        start = half;
        end = count;
        nextHalf = 0;
        break;
    }
    *drawHalf = nextHalf;

    register GameStateMapOverlay * cameraXOwner asm("r1") = overlay;
    asm("" : "+l"(cameraXOwner));
    register MapOverlayScene * scene asm("r0") = cameraXOwner->scene;
    int cameraX = scene->vtable->getCameraX(scene);
    register GameStateMapOverlay * cameraYOwner asm("r2") = overlay;
    asm("" : "+l"(cameraYOwner));
    scene = cameraYOwner->scene;
    int cameraY = scene->vtable->getCameraY(scene);

    register unsigned index asm("r8") = start;
    asm("" : "+r"(index) : "r"(end));
    if (index >= end)
        return;

    GameStateMapOverlay::FrameList & frames = overlay->frames;
    // The loop keeps the address of the animation clip, not the start of the particle.
    enum
    {
        CLIP_OFFSET = offsetof(MapOverlayParticle, animation) + offsetof(SpriteAnimator, clip),
        KEYFRAME_OFFSET = offsetof(SpriteAnimator, keyframe) - offsetof(SpriteAnimator, clip),
        X_PIXEL_OFFSET = offsetof(MapOverlayParticle, xQ16) + sizeof(i16) - CLIP_OFFSET,
        Y_PIXEL_OFFSET = offsetof(MapOverlayParticle, yQ16) + sizeof(i16) - CLIP_OFFSET,
        WRAP_MARGIN = 8,
        WRAP_MASK = 255,
    };
    uintptr_t particleOffset = start * sizeof(MapOverlayParticle) +
        offsetof(GameStateMapOverlay, particles.entries) + CLIP_OFFSET;
    asm("" : "+r"(particleOffset));
    uintptr_t clipAddress = particleOffset + reinterpret_cast<uintptr_t>(overlay);
    asm("" : "+r"(clipAddress));
    OverlayPaletteTable const & palettes = overlay->palettes;
    unsigned const priorityMap = 1 | (1 << 2) | (1 << 4) | (1 << 6);

    do
    {
        unsigned keyframeOffset = *reinterpret_cast<u16 const *>(clipAddress + KEYFRAME_OFFSET) * sizeof(SpriteKeyframe);
        asm("" : "+r"(keyframeOffset));
        SpriteAnimationClip const & clip = *reinterpret_cast<SpriteAnimationClip const *>(clipAddress);
        unsigned frameIndex = reinterpret_cast<SpriteKeyframe const *>(
            reinterpret_cast<uintptr_t>(clip.keyframes) + keyframeOffset)->frame;

        // Subtract the camera origin and wrap into [-8, 247] for sprites straddling the seam.
        register unsigned xOffset asm("r2") = X_PIXEL_OFFSET;
        asm volatile("" : : "l"(xOffset), "l"(frameIndex));
        register int xPosition asm("r6");
        asm("ldrsh %0, [%1, %2]" : "=l"(xPosition)
            : "l"(clipAddress), "l"(xOffset),
              "m"(*reinterpret_cast<u8 const *>(clipAddress + X_PIXEL_OFFSET)),
              "m"(*reinterpret_cast<u8 const *>(clipAddress + X_PIXEL_OFFSET + 1)));
        register int xCamera asm("r1") = cameraX;
        register int xBiased asm("r6") = xPosition;
        asm(".syntax unified\n\tsubs %0, %0, %1\n\t.syntax divided" : "+l"(xBiased) : "l"(xCamera) : "cc");
        xBiased += WRAP_MARGIN;
        register unsigned xMask asm("r2") = WRAP_MASK;
        asm("" : "+l"(xMask) : "l"(xBiased));
        int x = (xBiased & xMask) - WRAP_MARGIN;

        register unsigned yOffset asm("r1") = Y_PIXEL_OFFSET;
        asm volatile("" : : "l"(yOffset), "l"(x));
        register int yPosition asm("r5");
        asm("ldrsh %0, [%1, %2]" : "=l"(yPosition)
            : "l"(clipAddress), "l"(yOffset),
              "m"(*reinterpret_cast<u8 const *>(clipAddress + Y_PIXEL_OFFSET)),
              "m"(*reinterpret_cast<u8 const *>(clipAddress + Y_PIXEL_OFFSET + 1)));
        register int yCamera asm("r2") = cameraY;
        register int yBiased asm("r5") = yPosition;
        asm(".syntax unified\n\tsubs %0, %0, %1\n\tadds %0, %2\n\t.syntax divided"
            : "+l"(yBiased) : "l"(yCamera), "i"(WRAP_MARGIN) : "cc");
        asm(".syntax unified\n\tmovs r1, %1\n\tands %0, r1\n\t.syntax divided"
            : "+l"(yBiased) : "i"(WRAP_MASK) : "r1", "cc");
        int y = yBiased - WRAP_MARGIN;

        unsigned frameOffset = frameIndex * sizeof(GameStateMapOverlay::Frame) + offsetof(GameStateMapOverlay::FrameList, entries);
        asm("" : "+l"(frameOffset) : "l"(y));
        register uintptr_t framesAddress asm("r2") = reinterpret_cast<uintptr_t>(&frames);
        asm("" : "+l"(framesAddress));
        register GameStateMapOverlay::Frame * frame asm("r4") = reinterpret_cast<GameStateMapOverlay::Frame *>(
            framesAddress + frameOffset);
        asm("" : "+r"(frame));

        // The tile lookup is a short call even though sprite queuing uses a long call into IWRAM.
        register OverlayTileAllocation * tileAllocation asm("r0") = &frame->tiles;
        register unsigned baseTile asm("r0");
        asm volatile("ldr r1, [%0, %2]\n\tbl TileHandle_GetTileIndex" : "=l"(baseTile)
            : "0"(tileAllocation), "i"(offsetof(OverlayTileAllocation, handle))
            : "r1", "r2", "r3", "r12", "lr", "cc", "memory");
        register GameStateMapOverlay * sceneOwner asm("r2") = overlay;
        asm("" : "+l"(sceneOwner));
        register MapOverlayScene * drawScene asm("r1") = sceneOwner->scene;
        asm("" : "+l"(drawScene));
        QueueOverlayParticleSprites(*target, x, y, priorityMap, 0, *frame, drawScene, baseTile, palettes);

        clipAddress += sizeof(MapOverlayParticle);
        register unsigned step asm("r1") = 1;
        asm("" : "+l"(step));
        index += step;
    }
    while (index < end);
}
