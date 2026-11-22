#include "prelude.h"

#include "bachelorette.hh"
#include "clock_display.hh"
#include "dma_queue.hh"
#include "dog.hh"
#include "farm.hh"
#include "farm_house.hh"
#include "farmer.hh"
#include "fixed_sprite_overlay.hh"
#include "gbasvc.h"
#include "map_overlay.hh"
#include "message_script.hh"
#include "npc.hh"
#include "schedule_info.hh"
#include "sprite_animation.hh"
#include "unknown_types.hh"

#include <algorithm>
#include <stdlib.h>
#include <string.h>

#define ROM_TEXT(addr) EC SECTION(".text." #addr)

ROM_TEXT(0803A8A4) u32 GetMapGraphicsVariantId(u32 mapId, u32 season, u32 houseLevel, u32 coopLevel, u32 barnLevel)
{
    enum
    {
        FARM_MAP = 2,
        COOP_MAP = 17,
        FARMHOUSE_MAP = 29,
        BARN_MAP = 37,
        SPRING_MINE_FIRST_MAP = 52,
        MAPS_PER_MINE = 256,
        LAKE_MINE_FIRST_MAP = SPRING_MINE_FIRST_MAP + MAPS_PER_MINE,
    };
    enum Mine { SPRING_MINE, LAKE_MINE };
    enum
    {
        MINE_FLOOR_ZERO_GRAPHICS = 56,
        MINE_MULTIPLE_OF_FIVE_GRAPHICS = 57,
        MINE_MULTIPLE_OF_THREE_GRAPHICS = 58,
        MINE_DEFAULT_GRAPHICS = 59,
        LAKE_MINE_FLOOR_NINE_GRAPHICS = 61,
    };

    u32 mineMapOffset = mapId - SPRING_MINE_FIRST_MAP;
    if (mineMapOffset < 2 * MAPS_PER_MINE)
    {
        unsigned floor;
        Mine mine;
        if (static_cast<i32>(mapId) >= LAKE_MINE_FIRST_MAP)
        {
            floor = mapId - LAKE_MINE_FIRST_MAP;
            mine = LAKE_MINE;
        }
        else
        {
            floor = mineMapOffset;
            mine = SPRING_MINE;
        }
        if (floor == 0)
            return MINE_FLOOR_ZERO_GRAPHICS;
        if (floor == 9 && mine == LAKE_MINE)
            return LAKE_MINE_FLOOR_NINE_GRAPHICS;
        if (floor % 5 == 0)
            return MINE_MULTIPLE_OF_FIVE_GRAPHICS;
        if (floor % 3 == 0)
            return MINE_MULTIPLE_OF_THREE_GRAPHICS;
        return MINE_DEFAULT_GRAPHICS;
    }

    // Keep the original case order for the Thumb jump table. Return values index MapData.
    switch (mapId)
    {
    case FARM_MAP: return season == SEASON_WINTER ? 1 : 0;
    case 6: return season != SEASON_WINTER ? 2 : 3;
    case 5: return season != SEASON_WINTER ? 4 : 5;
    case 7: return season != SEASON_WINTER ? 6 : 7;
    case 1: return season != SEASON_WINTER ? 8 : 9;
    case 3: return season != SEASON_WINTER ? 12 : 13;
    case 4: return season != SEASON_WINTER ? 10 : 11;
    case 0: return season != SEASON_WINTER ? 14 : 15;
    case 8: return season != SEASON_WINTER ? 16 : 17;
    case COOP_MAP:
        switch (coopLevel)
        {
        case 0: return 36;
        case 1: return 37;
        }
        break;
    case BARN_MAP:
        switch (barnLevel)
        {
        case 0: return 38;
        case 1: return 39;
        }
        break;
    case FARMHOUSE_MAP:
        switch (houseLevel)
        {
        case 0: return 41;
        case 1: return 42;
        case 2: return 43;
        }
        break;
    case 31: return 18;
    case 32: return 19;
    case 33: return 20;
    case 34: return 21;
    case 21: return 22;
    case 22: return 23;
    case 23: return 24;
    case 15: return 25;
    case 16: return 26;
    case 36: return 27;
    case 13: return 28;
    case 19: return 29;
    case 20: return 30;
    case 12: return 31;
    case 24: return 32;
    case 25: return 33;
    case 26: return 34;
    case 27: return 35;
    case 9: return 40;
    case 10: return 44;
    case 11: return 45;
    case 41: return 46;
    case 42: return 47;
    case 38: return 48;
    case 40: return 49;
    case 30: return 50;
    case 14: return 51;
    case 28: return 52;
    case 18: return 53;
    case 35: return 54;
    case 39: return 55;
    case 43: return 60;
    case 48: return 62;
    case 49: return 63;
    case 50: return 64;
    case 51: return 65;
    case 44:
    case 45:
    case 46: return 16;
    case 47: return 17;
    }
    return 0;
}

struct MapOverlayParticleMotion
{
    i32 dxQ16;
    i32 dyQ16;
};

EC MapOverlayParticleMotion const gMapOverlayParticleMotion[][2];
EC void ResetMapOverlayParticle(MapOverlayParticle &, int cameraX, int cameraY);

ROM_TEXT(0803B2A8) void UpdateGameStateMapOverlay(GameStateMapOverlay & overlay)
{
    unsigned kind = overlay.kind;
    MapOverlayParticleMotion const * motion = gMapOverlayParticleMotion[kind];
    unsigned motionIndex = 0;
    unsigned particlesInGroup = 0;
    unsigned groupSize;
    switch (kind)
    {
    case 0:
    default: groupSize = 2; break;
    case 1: groupSize = 3; break;
    }

    int cameraX = overlay.scene->vtable->getCameraX(overlay.scene);
    int cameraY = overlay.scene->vtable->getCameraY(overlay.scene);
    unsigned count = overlay.particles.count;
    unsigned index = 0;
    if (index >= count)
        return;

    MapOverlayParticle * particle = overlay.particles.entries;
    // Preserve the scaled index and address addition in the original Thumb code.
    MapOverlayParticleMotion const * step = reinterpret_cast<MapOverlayParticleMotion const *>(
        motionIndex * sizeof(MapOverlayParticleMotion) + reinterpret_cast<uintptr_t>(motion));
    do
    {
        if (static_cast<i32>(AdvanceSpriteAnimator(&particle->animation) << (31 - SpriteAnimator::LOOP_FLAG_BIT)) < 0)
            ResetMapOverlayParticle(*particle, cameraX, cameraY);

        if (++particlesInGroup >= groupSize)
        {
            ++step;
            if (++motionIndex >= ARRAY_COUNT(gMapOverlayParticleMotion[0]))
            {
                step = motion;
                motionIndex = 0;
            }
            particlesInGroup = 0;
        }
        particle->xQ16 += step->dxQ16;
        particle->yQ16 += step->dyQ16;
        ++particle;
        ++index;
    } while (index < count);
}

EC void TileHeapOwner_Destroy(OverlayTileAllocation *, unsigned);
EC void __builtin_delete(void *);

struct FixedOverlayClipView
{
    SpriteKeyframe const * keyframes;
    u16 length;
};

struct FixedOverlayAnimationMethods
{
    STRUCT_PAD(0, 0xC);
    void (*getClip)(FixedOverlayClipView *, FixedOverlayAnimation *, unsigned);
    void (*getFrame)(FixedOverlayFrameView *, FixedOverlayAnimation *, unsigned);
};

struct FixedOverlayAnimation
{
    FixedOverlayAnimationMethods const * methods;
};

struct FixedOverlayBuildScratch
{
    FixedOverlayFrameView view;
    u8 palette;
    FixedOverlayClipView clip;
};

EC void InitOverlayTileHeap(OverlayTileAllocation *) asm("func_08007874");
EC void ReleaseOverlayTiles(OverlayTileAllocation *, unsigned) asm("TileHandle_Release");
EC u32 AllocateOverlayTiles(OverlayTileAllocation *, unsigned) asm("func_08007B54");
EC unsigned GetOverlayBaseTile(OverlayTileAllocation *, unsigned) asm("func_08007D4C");
EC void * CopyOverlayFrame(void *, void const *, size_t) asm("memcpy");

ROM_TEXT(0803AB30) FixedSpriteOverlay * InitializeFixedSpriteOverlay(void * storage, OverlaySpriteResources * inputResources)
{
    // Register and stack constraints preserve the original Thumb layout.
    enum { FIXED_OVERLAY_PALETTE_KIND = 15, FIRST_FIXED_OVERLAY_CLIP = 0x91A, TILE_ALLOCATION_ORDER = 2 };
    register FixedSpriteOverlay * overlay asm("r9") = static_cast<FixedSpriteOverlay *>(storage);
    register OverlaySpriteResources * resources asm("r10");
    register OverlaySpriteResources * resourceArgument asm("r1") = inputResources;
    register FixedSpriteOverlay::Frame * tileFrame asm("r6");
    asm("mov %1, %3\n\tmov %0, %2\n\tstmia %0!, {%3}"
        : "=l"(tileFrame), "=h"(resources) : "h"(overlay), "l"(resourceArgument) : "memory");
    int remaining = ARRAY_COUNT(overlay->frames) - 1;
    asm("" : "+l"(remaining));
    FixedOverlayBuildScratch local;
    register u8 * paletteLow asm("r0");
    register u8 * palette asm("r8");
    asm("add %1, sp, %2\n\tmov %0, %1"
        : "=h"(palette), "=l"(paletteLow) : "i"(offsetof(FixedOverlayBuildScratch, palette)));
    register FixedOverlayClipView * clipPointer asm("r1");
    asm(".syntax unified\n\tmov %0, sp\n\tadds %0, %1\n\t.syntax divided"
        : "=l"(clipPointer) : "i"(offsetof(FixedOverlayBuildScratch, clip)));
    FixedOverlayClipView * volatile clipView = clipPointer;
    register unsigned zero asm("r5") = 0;
    asm("" : "+l"(zero));
    register FixedSpriteOverlay::Frame * frame asm("r4") = tileFrame;
    asm("" : "+l"(frame));
    do
    {
        frame->view.objects.data = reinterpret_cast<SpriteFrameObject const *>(zero);
        asm("strh %1, [%2, %3]" : "=m"(frame->view.objects.length)
            : "l"(zero), "l"(frame), "i"(offsetof(FixedSpriteOverlay::Frame, view.objects.length)));
        frame->view.tileBytes.data = reinterpret_cast<u8 const *>(zero);
        asm("strh %1, [%2, %3]" : "=m"(frame->view.tileBytes.length)
            : "l"(zero), "l"(frame), "i"(offsetof(FixedSpriteOverlay::Frame, view.tileBytes.length)));
        frame->view.paletteBytes.data = reinterpret_cast<u16 const *>(zero);
        asm("strh %1, [%2, %3]" : "=m"(frame->view.paletteBytes.length)
            : "l"(zero), "l"(frame), "i"(offsetof(FixedSpriteOverlay::Frame, view.paletteBytes.length)));
        frame->view.extras.data = reinterpret_cast<SpriteFrameExtra const *>(zero);
        asm("strh %1, [%2, %3]" : "=m"(frame->view.extras.length)
            : "l"(zero), "l"(frame), "i"(offsetof(FixedSpriteOverlay::Frame, view.extras.length)));
        InitOverlayTileHeap(&tileFrame->tiles);
        frame->tiles.handle = zero;
        ++frame;
        ++tileFrame;
        --remaining;
        register int exhausted asm("r0");
        asm(".syntax unified\n\tmovs %0, #1\n\trsbs %0, %0, #0\n\t.syntax divided" : "=l"(exhausted));
        if (remaining == exhausted)
            break;
    }
    while (true);
    register OverlayPaletteTable * paletteTable asm("r6");
    asm(".syntax unified\n\tmov %0, %1\n\tadds %0, %2\n\t.syntax divided"
        : "=l"(paletteTable) : "h"(overlay), "i"(offsetof(FixedSpriteOverlay, palettes)));
    paletteTable->count = 0;
    register u8 * uploadPending asm("r0");
    asm(".syntax unified\n\tmov %0, %1\n\tadds %0, %2\n\t.syntax divided"
        : "=l"(uploadPending) : "h"(overlay), "i"(offsetof(FixedSpriteOverlay, uploadPending)) : "memory");
    register unsigned paletteCount asm("r4") = 1;
    asm("" : "+l"(paletteCount));
    *uploadPending = paletteCount;
    register OverlaySpriteResources * paletteResources asm("r2") = resources;
    asm("" : "+l"(paletteResources));
    register unsigned paletteSlot asm("r0") = paletteResources->vtable->reservePalette(resources, FIXED_OVERLAY_PALETTE_KIND);
    register u8 * paletteOut asm("r3") = palette;
    asm("" : "+l"(paletteOut) : "l"(paletteSlot));
    *paletteOut = paletteSlot;
    register OverlayPaletteTable * palettes asm("r5") = paletteTable;
    asm("" : "+l"(palettes) : "l"(paletteOut));
    register unsigned size asm("r2") = palettes->count;
    asm("" : "+l"(size));
    if (paletteCount > size)
    {
        register u8 * begin asm("r0");
        asm(".syntax unified\n\tmov %0, %1\n\tadds %0, %2\n\t.syntax divided"
            : "=l"(begin) : "h"(overlay), "i"(offsetof(FixedSpriteOverlay, palettes.ids)));
        register u8 * end asm("r2");
        asm(".syntax unified\n\tadds %0, %2\n\tadds %0, %1, %0\n\t.syntax divided"
            : "=l"(end) : "l"(palettes), "i"(offsetof(OverlayPaletteTable, ids)), "0"(size));
        register unsigned value asm("r1") = *paletteOut;
        asm("" : "+l"(value) : "l"(end), "l"(begin));
        memset(begin, value, end - begin);
        size = palettes->count;
        asm("" : "+l"(size));
        asm(".syntax unified\n\tadds %0, %2, %3\n\tadds %0, %1, %0\n\t.syntax divided"
            : "=l"(begin) : "l"(palettes), "l"(size), "I"(offsetof(OverlayPaletteTable, ids)));
        register u8 * source asm("r1") = &local.palette;
        asm("" : "+l"(source) : "l"(begin));
        value = *source;
        asm("" : "+l"(value));
        memset(begin, value, paletteCount - size);
        palettes->count = paletteCount;
    }
    else
    {
        register u8 * begin asm("r0");
        asm(".syntax unified\n\tmov %0, %1\n\tadds %0, %2\n\t.syntax divided"
            : "=l"(begin) : "h"(overlay), "i"(offsetof(FixedSpriteOverlay, palettes.ids)));
        register u8 * newEnd asm("r4");
        asm(".syntax unified\n\tmov %0, %1\n\tadds %0, %2\n\t.syntax divided"
            : "=l"(newEnd) : "h"(overlay), "i"(offsetof(FixedSpriteOverlay, palettes.ids) + 1));
        register u8 * source asm("r2") = palette;
        asm("" : "+l"(source) : "l"(newEnd), "l"(begin));
        register unsigned value asm("r1") = *source;
        asm("" : "+l"(value));
        memset(begin, value, newEnd - begin);
        register u8 * oldEnd asm("r0") = palettes->ids + palettes->count;
        asm("" : "+l"(oldEnd));
        register size_t removed asm("r0") = oldEnd - newEnd;
        asm("" : "+l"(removed));
        paletteTable->count -= removed;
    }
    register OverlaySpriteResources * animationResources asm("r3") = resources;
    asm("" : "+l"(animationResources));
    register FixedOverlayAnimation * animation asm("r8") = animationResources->vtable->getAnimationSlot0(resources);
    asm("" : "+r"(animation));
    register unsigned index asm("r7") = 0;
    asm("" : "+l"(index));
    do
    {
        register unsigned stride asm("r0") = sizeof(FixedSpriteOverlay::Frame);
        asm("" : "+l"(stride) : "l"(index));
        register uintptr_t frameAddress asm("r6") = index * stride;
        register FixedSpriteOverlay::Frame * start asm("r4");
        asm(".syntax unified\n\tadd %1, %3\n\tadds %0, %1, %2\n\t.syntax divided"
            : "=&l"(start), "+l"(frameAddress) : "I"(offsetof(FixedSpriteOverlay, frames)), "h"(overlay));
        register unsigned firstClip asm("r0") = FIRST_FIXED_OVERLAY_CLIP;
        asm("" : "+l"(firstClip) : "l"(start));
        register unsigned clip asm("r2") = index + firstClip;
        asm("" : "+l"(clip));
        register FixedOverlayAnimation * lookup asm("r1") = animation;
        asm("" : "+l"(lookup) : "l"(clip));
        register FixedOverlayAnimationMethods const * methods asm("r5") = lookup->methods;
        asm("" : "+l"(methods));
        methods->getClip(clipView, lookup, clip);
        register SpriteKeyframe const * keyframes asm("r0") = local.clip.keyframes;
        asm("" : "+l"(keyframes));
        register unsigned firstFrame asm("r2") = keyframes[0].frame;
        asm("" : "+l"(firstFrame));
        register FixedOverlayFrameView * frameView asm("r0") = &local.view;
        asm("" : "+l"(frameView));
        methods->getFrame(frameView, animation, firstFrame);
        // Preserve the compiler's original temporary self-copy.
        CopyOverlayFrame(&local.view, &local.view, sizeof(local.view));
        register FixedOverlayFrameView * output asm("r4") = &start->view;
        asm volatile(".syntax unified\n\tmov r0, sp\n"
            "\tldmia r0!, {r1, r2, r3}\n\tstmia %0!, {r1, r2, r3}\n"
            "\tldmia r0!, {r1, r2, r3}\n\tstmia %0!, {r1, r2, r3}\n"
            "\tldmia r0!, {r2, r3}\n\tstmia %0!, {r2, r3}\n\t.syntax divided"
            : "+l"(output) :
            : "r0", "r1", "r2", "r3", "memory");
        register OverlayTileAllocation * tiles asm("r4") =
            reinterpret_cast<OverlayTileAllocation *>(frameAddress + offsetof(FixedSpriteOverlay, frames) + offsetof(FixedSpriteOverlay::Frame, tiles));
        asm("" : "+l"(tiles));
        ReleaseOverlayTiles(tiles, tiles->handle);
        register unsigned handle asm("r1") = AllocateOverlayTiles(tiles, TILE_ALLOCATION_ORDER);
        asm("" : "+l"(handle));
        tiles->handle = handle;
        reinterpret_cast<FixedSpriteOverlay::Frame *>(frameAddress + offsetof(FixedSpriteOverlay, frames))->baseTile =
            GetOverlayBaseTile(tiles, handle);
        ++index;
        asm("" : "+l"(index));
    }
    while (index < ARRAY_COUNT(overlay->frames));
    return overlay;
}

ROM_TEXT(0803AC78) void DestroyFixedSpriteOverlay(FixedSpriteOverlay * inputOverlay, unsigned inputFlags)
{
    // Match the original Thumb register allocation for the destructor.
    register FixedSpriteOverlay * overlay asm("r6") = inputOverlay;
    asm("" : "+r"(overlay));
    register unsigned destructorFlags asm("r8") = inputFlags;
    asm("" : "+r"(destructorFlags) : "r"(overlay));
    OverlaySpriteResources * resources = overlay->resources;
    OverlaySpriteResourceVtable const * vtable = resources->vtable;
    register FixedSpriteOverlay::Frame * end asm("r4") = overlay->frames + 3;
    asm("" : "+r"(end) : "r"(resources), "r"(vtable));
    vtable->releasePalette(resources, overlay->palettes.ids[0]);

    register FixedSpriteOverlay::Frame * firstFrame asm("r0") = overlay->frames;
    asm("" : "+r"(firstFrame));
    if (firstFrame)
    {
        FixedSpriteOverlay::Frame * frame = end;
        if (firstFrame != frame)
        {
            FixedSpriteOverlay::Frame * first = firstFrame;
            do
            {
                --frame;
                OverlayTileAllocation * tiles = &frame->tiles;
                ReleaseOverlayTiles(tiles, tiles->handle);
                TileHeapOwner_Destroy(tiles, 2);
            }
            while (first != frame);
        }
    }
    if (destructorFlags & 1)
        __builtin_delete(overlay);
}

EC void * RetryDmaAllocation(u32) asm("func_080D3BC0");

#define COPY_DMA_COMMAND(destination, source, a, b, c) \
    do { \
        register DmaCommand * out asm("r0") = destination; \
        register DmaCommand const * in asm("r1") = source; \
        asm volatile(".syntax unified\n\tldmia %1!, {" #a ", " #b ", " #c "}\n" \
            "\tstmia %0!, {" #a ", " #b ", " #c "}\n\tldr %1, [%1]\n\tstr %1, [%0]\n\t.syntax divided" \
            : "+l"(out), "+l"(in) : : #a, #b, #c, "memory"); \
    } while (false)

ROM_TEXT(0803ACD8) void QueueFixedSpriteOverlayGraphics(FixedSpriteOverlay & inputOverlay, DmaTransferQueue & outputQueue)
{
    // Keep the original Thumb register assignments around each DMA command copy.
    enum { OBJ_VRAM_BASE = 0x06010000, OBJ_TILE_BYTES = 32 };
    register FixedSpriteOverlay * overlay asm("r9") = &inputOverlay;
    asm("" : "+h"(overlay));
    register DmaTransferQueue * queue asm("r5") = &outputQueue;
    asm("" : "+l"(queue));
    register unsigned initialIndex asm("r0") = 0;
    asm("" : "+l"(initialIndex));
    register unsigned index asm("r10") = initialIndex;
    asm("" : "+h"(index));
    struct
    {
        DmaCommand command;
        unsigned count;
        unsigned inserted;
        DmaCommand const * volatile commandSource;
        u8 const * volatile paletteIds;
        DmaCommand * volatile insertionPoint;
        DmaCommand * volatile buffer;
    } local;
    do
    {
        register unsigned stride asm("r0") = sizeof(FixedSpriteOverlay::Frame);
        asm("" : "+l"(stride));
        register unsigned lowIndex asm("r1") = index;
        asm("" : "+l"(lowIndex));
        register uintptr_t frameOffset asm("r0");
        asm(".syntax unified\n\tmuls %1, %0, %1\n\tadds %0, %1, #0\n\t.syntax divided"
            : "=l"(frameOffset), "+l"(lowIndex) : "0"(stride) : "cc");
        register FixedSpriteOverlay::Frame * frame asm("r4");
        asm(".syntax unified\n\tadds %0, %1, %2\n\tadd %0, %3\n\t.syntax divided"
            : "=&l"(frame) : "l"(frameOffset), "I"(offsetof(FixedSpriteOverlay, frames)), "h"(overlay));
        register FixedOverlayFrameSlice<u8> const * tileData asm("r6") = &frame->view.tileBytes;
        asm("" : "+l"(tileData));
        u8 const * source = frame->view.tileBytes.data;
        asm("" : "+l"(source));
        register OverlayTileAllocation * allocation asm("r0");
        asm(".syntax unified\n\tadd %0, %1\n\tadds %0, %2\n\t.syntax divided"
            : "=l"(allocation) : "h"(overlay), "i"(offsetof(FixedSpriteOverlay, frames[0].tiles)), "0"(frameOffset));
        register unsigned tile asm("r2") = GetOverlayBaseTile(allocation, allocation->handle);
        asm("" : "+l"(tile));
        unsigned bytes = frame->view.tileBytes.data ? tileData->length : 0;
        register unsigned tileOffset asm("r2") = tile * OBJ_TILE_BYTES;
        asm("" : "+l"(tileOffset) : "l"(bytes));
        register unsigned vramBase asm("r4") = OBJ_VRAM_BASE;
        asm("" : "+l"(vramBase));
        DmaCommand_InitializeCopy(&local.command, source, reinterpret_cast<void *>(tileOffset + vramBase), bytes);
        register DmaCommand * command asm("r6") = &local.command;
        asm("" : "+l"(command));
        local.commandSource = command;
        register DmaCommand * position asm("r1") = queue->end;
        asm("" : "+l"(position));
        if (position != queue->capacity_end)
        {
            if (position)
                COPY_DMA_COMMAND(position, &local.command, r2, r3, r7);
            ++queue->end;
            register unsigned step asm("r4") = 1;
            asm("" : "+l"(step));
            index += step;
            register u8 const * firstPalette asm("r6");
            asm(".syntax unified\n\tmov %0, %1\n\tadds %0, %2\n\t.syntax divided"
                : "=l"(firstPalette) : "h"(overlay), "i"(offsetof(FixedSpriteOverlay, palettes.ids)));
            local.paletteIds = firstPalette;
        }
        else
        {
            // Grow by the larger of the queue's size and the insertion count.
            local.insertionPoint = position;
            local.inserted = 1;
            register DmaCommand * begin asm("r1") = queue->begin;
            asm("" : "+l"(begin));
            register DmaCommand * end asm("r7") = local.insertionPoint;
            asm("" : "+l"(end));
            register intptr_t occupiedBytes asm("r0") = reinterpret_cast<intptr_t>(end) - reinterpret_cast<intptr_t>(begin);
            asm("" : "+l"(occupiedBytes));
            register unsigned count asm("r4") = occupiedBytes >> 4;
            asm("" : "+l"(count));
            local.count = count;
            register unsigned * insertCount asm("r2") = &local.inserted;
            asm("" : "+l"(insertCount));
            register unsigned * growthCount asm("r0") = &local.count;
            asm("" : "+l"(growthCount));
            asm(".syntax unified\n\tcmp %1, #1\n\tbhs 1f\n\tadds %0, %2, #0\n1:\n\t.syntax divided"
                : "+l"(growthCount) : "l"(count), "l"(insertCount) : "cc");
            unsigned growth = *growthCount;
            register unsigned newCapacity asm("r0") = count + growth;
            asm("" : "+l"(newCapacity));
            register unsigned bufferBytes asm("r8");
            register DmaCommand * buffer asm("r0");
            if (newCapacity)
            {
                register unsigned bytes asm("r4") = newCapacity * sizeof(DmaCommand);
                asm("" : "+l"(bytes));
                buffer = static_cast<DmaCommand *>(malloc(bytes));
                bufferBytes = bytes;
                asm("" : "+h"(bufferBytes) : "l"(buffer));
                if (!buffer)
                    buffer = static_cast<DmaCommand *>(RetryDmaAllocation(bufferBytes));
                begin = queue->begin;
                asm("" : "+l"(begin) : "l"(buffer));
            }
            else
            {
                buffer = 0;
                asm("" : "+l"(buffer));
                bufferBytes = reinterpret_cast<unsigned>(buffer);
                asm("" : "+h"(bufferBytes));
            }
            register DmaCommand * newBuffer asm("r4") = buffer;
            asm("" : "+l"(newBuffer));
            local.buffer = newBuffer;
            register DmaCommand const * from asm("r2") = begin;
            asm("" : "+l"(from));
            register DmaCommand * to asm("r3") = newBuffer;
            asm("" : "+l"(to));
            register unsigned step asm("r0") = 1;
            asm("" : "+l"(step));
            index += step;
            register u8 const * firstPalette asm("r1");
            asm(".syntax unified\n\tmov %0, %1\n\tadds %0, %2\n\t.syntax divided"
                : "=l"(firstPalette) : "h"(overlay), "i"(offsetof(FixedSpriteOverlay, palettes.ids)));
            local.paletteIds = firstPalette;
            register DmaCommand const * oldEnd asm("r4") = local.insertionPoint;
            asm("" : "+l"(oldEnd));
            if (from != oldEnd)
                do
                {
                    if (to)
                        COPY_DMA_COMMAND(to, from, r4, r6, r7);
                    ++from;
                    ++to;
                    register DmaCommand const * positionReload asm("r6") = local.insertionPoint;
                    asm("" : "+l"(positionReload));
                    if (from == positionReload)
                        break;
                }
                while (true);
            register DmaCommand * after asm("r4") = to;
            asm("" : "+l"(after));
            register unsigned insertedCount asm("r0") = local.inserted;
            asm("" : "+l"(insertedCount));
            if (insertedCount == 1)
            {
                if (after)
                    COPY_DMA_COMMAND(after, local.commandSource, r2, r3, r7);
                ++after;
            }
            else
            {
                register unsigned remaining asm("r3") = insertedCount;
                asm("" : "+l"(remaining));
                register DmaCommand * target asm("r2") = after;
                asm("" : "+l"(target));
                while (remaining)
                {
                    if (target)
                        COPY_DMA_COMMAND(target, local.commandSource, r4, r6, r7);
                    --remaining;
                    ++target;
                }
                after = target;
                asm("" : "+l"(after));
            }
            register DmaCommand * limit asm("r2") = queue->end;
            asm("" : "+l"(limit));
            register DmaCommand * old asm("r1") = queue->begin;
            asm("" : "+l"(old));
            register DmaCommand * item asm("r0") = old;
            asm("" : "+l"(item));
            // The original queue walks the old commands before freeing their storage.
            if (old != limit)
                do { ++item; } while (item != limit);
            if (old)
                free(old);
            register uintptr_t capacityEnd asm("r0") = reinterpret_cast<uintptr_t>(local.buffer);
            asm("add %0, %1" : "+l"(capacityEnd) : "h"(bufferBytes));
            register DmaCommand * replacement asm("r1") = local.buffer;
            asm("" : "+l"(replacement));
            queue->begin = replacement;
            queue->end = after;
            queue->capacity_end = reinterpret_cast<DmaCommand *>(capacityEnd);
        }
        register unsigned next asm("r2") = index;
        asm("" : "+l"(next));
        if (next >= ARRAY_COUNT(overlay->frames))
            break;
    }
    while (true);
    register FixedSpriteOverlay * owner asm("r3") = overlay;
    asm("" : "+l"(owner));
    register OverlaySpriteResources * resources asm("r0") = owner->resources;
    asm("" : "+l"(resources));
    register OverlaySpriteResourceVtable const * methods asm("r3") = resources->vtable;
    asm("" : "+l"(methods));
    register FixedSpriteOverlay * data asm("r4") = overlay;
    asm("" : "+l"(data));
    register u16 const * colors asm("r1") = data->frames[0].view.paletteBytes.data;
    asm("" : "+l"(colors));
    register u8 const * ids asm("r6") = local.paletteIds;
    asm("" : "+l"(ids));
    register unsigned palette asm("r2") = *ids;
    asm("" : "+l"(palette));
    methods->setObjectPalette(resources, colors, palette, 1);
}

struct MapOverlayAnimationMethods
{
    STRUCT_PAD(0x00, 0x0C);
    void (*getClip)(FixedOverlayClipView *, SpriteAnimationSource const *, unsigned);
    void (*getFrame)(FixedOverlayFrameView *, SpriteAnimationSource const *, unsigned);
};

EC u32 RetainOverlayTiles(OverlayTileAllocation *, unsigned) asm("func_08007CD8");
EC MapOverlayParticle & InitMapOverlayParticle(MapOverlayParticle &, SpriteAnimator const &, int, int);

ROM_TEXT(0803AEA0) GameStateMapOverlay * InitializeGameStateMapOverlay(GameStateMapOverlay * input,
    MapOverlayScene * scene, SpriteAnimationSource const * archive, u32 kind, unsigned particleCount)
{
    // Keep the scratch buffers and copies in the original Thumb stack slots.
    enum { PRIMARY_PALETTE_KIND = 16, ALTERNATE_PALETTE_KIND = 17, OVERLAY_CLIP_ID = 0 };
    register GameStateMapOverlay * overlay asm("r6") = input;
    asm("" : "+l"(overlay));
    register MapOverlayScene * inputResources asm("r1") = scene;
    asm("" : "+l"(inputResources));
    register MapOverlayScene * resources asm("r9") = inputResources;
    asm("" : "+h"(resources));
    register SpriteAnimationSource const * animation asm("r10") = archive;
    asm("" : "+h"(animation));
    struct BuildScratch
    {
        u32 workspace[8];
        OverlayTileAllocation tiles;
        FixedOverlayFrameView frame;
        FixedOverlayClipView clip;
        volatile u32 kind;
        volatile unsigned frameCount;
        GameStateMapOverlay::FrameList * volatile frames;
        volatile int cameraX;
        OverlayPaletteTable * volatile palettes;
        volatile unsigned index;
    } scratch;
    scratch.kind = kind;
    overlay->scene = inputResources;
    register unsigned zero asm("r1") = 0;
    asm("" : "+l"(zero));
    overlay->frames.count = zero;
    register OverlayPaletteTable * palettes asm("r5") = &overlay->palettes;
    asm("" : "+l"(palettes));
    palettes->count = zero;
    register GameStateMapOverlay * storage asm("r0") = input;
    register u8 * overlayKind asm("r0");
    asm(".syntax unified\n\tadds %0, %2\n\t.syntax divided"
        : "=l"(overlayKind) : "0"(storage), "i"(offsetof(GameStateMapOverlay, kind)));
    register volatile u32 * kindAddress asm("r2") = &scratch.kind;
    asm("" : "+l"(kindAddress));
    *overlayKind = *reinterpret_cast<u8 volatile *>(kindAddress);
    register u8 * uploadPending asm("r2") = &overlay->uploadPending;
    asm("" : "+l"(uploadPending));
    register unsigned one asm("r0") = 1;
    asm("" : "+l"(one));
    *uploadPending = one;
    register u8 * drawHalf asm("r0") = &overlay->drawHalf;
    asm("" : "+l"(drawHalf));
    *drawHalf = zero;
    register unsigned * count asm("r0") = reinterpret_cast<unsigned *>(drawHalf
        + offsetof(GameStateMapOverlay, particles) - offsetof(GameStateMapOverlay, drawHalf));
    asm("" : "+l"(count));
    *count = zero;
    register SpriteAnimationSource const * methodOwner asm("r3") = animation;
    asm("" : "+l"(methodOwner));
    register MapOverlayAnimationMethods const * methods asm("r1") = *reinterpret_cast<MapOverlayAnimationMethods const * const *>(methodOwner);
    asm("" : "+l"(methods));
    register FixedOverlayClipView * clip asm("r4") = &scratch.clip;
    asm("" : "+l"(clip));
    register FixedOverlayClipView * clipOut asm("r0") = clip;
    asm("" : "+l"(clipOut));
    methods->getClip(clipOut, animation, OVERLAY_CLIP_ID);
    register unsigned frameCount asm("r1");
    asm(".syntax unified\n\tldr r0, [sp, %2]\n\tmovs %0, #0\n\tcmp r0, #0\n\tbeq 1f\n"
        "\tldrh %0, [%1, %3]\n1:\n\t.syntax divided"
        : "=&l"(frameCount)
        : "l"(clip), "i"(offsetof(BuildScratch, clip) + offsetof(FixedOverlayClipView, keyframes)),
          "i"(offsetof(FixedOverlayClipView, length))
        : "r0", "cc", "memory");
    scratch.frameCount = frameCount;
    register unsigned firstKey asm("r4") = 0;
    asm("" : "+l"(firstKey));
    scratch.index = firstKey;
    scratch.palettes = palettes;
    if (firstKey < frameCount)
    {
        FixedOverlayFrameView * frameOutLow;
        asm(".syntax unified\n\tadd %0, sp, %2\n\t.syntax divided"
            : "=&l"(frameOutLow) : "l"(overlay), "i"(offsetof(BuildScratch, frame)) : "r0", "r1", "r2", "r3", "r4", "r5");
        register FixedOverlayFrameView * frameOut asm("r8") = frameOutLow;
        asm("" : "+h"(frameOut));
        register OverlayTileAllocation * tiles asm("r5") = &scratch.tiles;
        asm("" : "+l"(tiles));
        scratch.frames = &overlay->frames;
        do
        {
            register SpriteAnimationSource const * methodOwner asm("r1") = animation;
            asm("" : "+l"(methodOwner));
            register MapOverlayAnimationMethods const * methods asm("r3") = *reinterpret_cast<MapOverlayAnimationMethods const * const *>(methodOwner);
            asm("" : "+l"(methods));
            register unsigned key asm("r2") = scratch.index;
            asm("" : "+l"(key));
            register unsigned offset asm("r1") = key * sizeof(SpriteKeyframe);
            asm("" : "+l"(offset));
            register SpriteKeyframe const * base asm("r0") = scratch.clip.keyframes;
            asm("" : "+l"(base));
            unsigned frameId = reinterpret_cast<SpriteKeyframe const *>(reinterpret_cast<uintptr_t>(base) + offset)->frame;
            register FixedOverlayFrameView * frameResult asm("r0") = frameOut;
            asm("" : "+l"(frameResult) : "r"(frameId));
            methods->getFrame(frameResult, animation, frameId);
            register void * copyDest asm("r1");
            register FixedOverlayFrameView * copySource asm("r0");
            asm volatile(".syntax unified\n\tmov %0, sp\n\tmov %1, %2\n"
                "\tldmia %1!, {r3, r4, r7}\n\tstmia %0!, {r3, r4, r7}\n"
                "\tldmia %1!, {r2, r3, r4}\n\tstmia %0!, {r2, r3, r4}\n"
                "\tldmia %1!, {r2, r7}\n\tstmia %0!, {r2, r7}\n\t.syntax divided"
                : "=&l"(copyDest), "=&l"(copySource) : "h"(frameOut) : "r2", "r3", "r4", "r7", "memory");
            InitOverlayTileHeap(tiles);
            tiles->handle = AllocateOverlayTiles(tiles, 0);
            unsigned frameIndex = overlay->frames.count;
            if (frameIndex < ARRAY_COUNT(overlay->frames.entries))
            {
                register unsigned frameOffset asm("r0") = frameIndex * sizeof(GameStateMapOverlay::Frame)
                    + offsetof(GameStateMapOverlay::FrameList, entries);
                asm("" : "+l"(frameOffset));
                register GameStateMapOverlay::FrameList * frameList asm("r3") = scratch.frames;
                asm("" : "+l"(frameList));
                register GameStateMapOverlay::Frame * frame asm("r4") =
                    reinterpret_cast<GameStateMapOverlay::Frame *>(reinterpret_cast<uintptr_t>(frameList) + frameOffset);
                asm("" : "+l"(frame));
                if (frame)
                {
                    register void * to asm("r1") = frame;
                    register void const * from asm("r0") = scratch.workspace;
                    asm volatile(".syntax unified\n"
                        "\tldmia %1!, {r2, r3, r7}\n\tstmia %0!, {r2, r3, r7}\n"
                        "\tldmia %1!, {r2, r3, r7}\n\tstmia %0!, {r2, r3, r7}\n"
                        "\tldmia %1!, {r2, r7}\n\tstmia %0!, {r2, r7}\n\t.syntax divided"
                        : "+l"(to), "+l"(from) : : "r2", "r3", "r7", "memory");
                    register OverlayTileAllocation * frameTiles asm("r4") = &frame->tiles;
                    asm("" : "+l"(frameTiles));
                    InitOverlayTileHeap(frameTiles);
                    frameTiles->handle = RetainOverlayTiles(frameTiles, tiles->handle);
                }
                ++overlay->frames.count;
            }
            ReleaseOverlayTiles(tiles, tiles->handle);
            TileHeapOwner_Destroy(tiles, 2);
            register unsigned next asm("r3") = scratch.index + 1;
            asm("" : "+l"(next));
            scratch.index = next;
            register unsigned limit asm("r4") = scratch.frameCount;
            asm("" : "+l"(limit));
            if (next >= limit)
                break;
        }
        while (true);
    }
    register unsigned overlayType asm("r7") = scratch.kind;
    asm("" : "+l"(overlayType));
    register unsigned paletteKind asm("r1");
    asm(".syntax unified\n\tcmp %1, #0\n\tbeq 1f\n\tcmp %1, #1\n\tbeq 2f\n"
        "1:\n\tmovs %0, %2\n\tb 3f\n2:\n\tmovs %0, %3\n3:\n\t.syntax divided"
        : "=l"(paletteKind) : "l"(overlayType), "i"(PRIMARY_PALETTE_KIND), "i"(ALTERNATE_PALETTE_KIND) : "cc");
    register MapOverlayScene * paletteOwner asm("r2") = resources;
    asm("" : "+l"(paletteOwner) : "l"(paletteKind));
    unsigned palette = paletteOwner->vtable->reservePalette(resources, paletteKind);
    OverlayPaletteTable * slots = scratch.palettes;
    unsigned slotIndex = slots->count;
    if (slotIndex < ARRAY_COUNT(slots->ids))
    {
        u8 * slot = &slots->ids[slotIndex];
        if (slot)
            *slot = palette;
        register OverlayPaletteTable * storedSlots asm("r3") = scratch.palettes;
        asm("" : "+l"(storedSlots));
        ++storedSlots->count;
    }
    register unsigned initialDuration asm("r4") = 0;
    asm("" : "+l"(initialDuration));
    register unsigned totalDuration asm("r8") = initialDuration;
    asm("" : "+h"(totalDuration));
    register unsigned key asm("r2") = 0;
    asm("" : "+l"(key));
    register MapOverlayScene * cameraOwner asm("r7") = resources;
    asm("" : "+l"(cameraOwner));
    register MapOverlaySceneVtable const * cameraMethods asm("r3") = cameraOwner->vtable;
    asm("" : "+l"(cameraMethods));
    register unsigned frameLimit asm("r0") = scratch.frameCount;
    asm("" : "+l"(frameLimit));
    if (totalDuration < frameLimit)
    {
        register SpriteKeyframe const * frame asm("r1") = scratch.clip.keyframes;
        asm("" : "+l"(frame));
        do
        {
            totalDuration += frame->duration;
            ++frame;
            ++key;
            register unsigned length asm("r4") = scratch.frameCount;
            asm("" : "+l"(length) : "l"(frame), "l"(key));
            if (key >= length)
                break;
        }
        while (true);
    }
    scratch.cameraX = cameraMethods->getCameraX(resources);
    cameraOwner = resources;
    asm("" : "+l"(cameraOwner));
    register int cameraY asm("r9") = cameraOwner->vtable->getCameraY(resources);
    asm("" : "+h"(cameraY));
    register unsigned firstParticle asm("r0") = 0;
    asm("" : "+l"(firstParticle));
    scratch.index = firstParticle;
    register unsigned particleLimit asm("r1");
    asm volatile("ldr %0, %1" : "=l"(particleLimit) : "m"(particleCount));
    if (firstParticle < particleLimit)
    {
        register SpriteAnimator * animator asm("r5");
        asm("mov %0, sp" : "=l"(animator) : : "memory");
        do
        {
            register SpriteAnimator * animatorArg asm("r0");
            asm("mov %0, sp" : "=l"(animatorArg) : : "memory");
            InitSpriteAnimator(animatorArg, animation, OVERLAY_CLIP_ID, 0x100);
            unsigned duration;
            asm volatile("mov %0, %1" : "=l"(duration) : "h"(totalDuration) : "r0", "r1", "r3");
            unsigned phase;
            if (duration)
                phase = rand() % totalDuration;
            else
                phase = 0;
            asm("" : "+l"(phase));
            if (phase)
            {
                register int speed asm("r4");
                asm(".syntax unified\n\tmovs r3, %2\n\tldrsh %0, [%1, r3]\n\t.syntax divided"
                    : "=l"(speed) : "l"(animator), "i"(offsetof(SpriteAnimator, speed_q8)) : "r3", "cc", "memory");
                animator->speed_q8 = phase << 8;
                asm("mov %0, sp" : "=l"(animatorArg) : : "memory");
                AdvanceSpriteAnimator(animatorArg);
                animator->speed_q8 = speed;
            }
            register MapOverlayParticle * particle asm("r4") = reinterpret_cast<MapOverlayParticle *>(scratch.workspace + 5);
            asm("" : "+l"(particle));
            register MapOverlayParticle * particleArg asm("r0") = particle;
            asm("" : "+l"(particleArg));
            register SpriteAnimator * source asm("r1");
            asm("mov %0, sp" : "=l"(source) : : "memory");
            InitMapOverlayParticle(*particleArg, *source, scratch.cameraX, cameraY);
            register GameStateMapOverlay::ParticleList * particles asm("r2");
            asm volatile(".syntax unified\n\tadds %0, %1, #0\n\tadds %0, %2\n\t.syntax divided"
                : "=&l"(particles) : "l"(overlay), "i"(offsetof(GameStateMapOverlay, particles)) : "cc", "memory");
            register unsigned particleIndex asm("r1") = particles->count;
            asm("" : "+l"(particleIndex));
            if (particleIndex < ARRAY_COUNT(particles->entries))
            {
                register unsigned particleOffset asm("r0") = particleIndex * sizeof(MapOverlayParticle)
                    + offsetof(GameStateMapOverlay::ParticleList, entries);
                asm("" : "+l"(particleOffset));
                MapOverlayParticle * entry;
                asm(".syntax unified\n\tadds %0, %1, %2\n\t.syntax divided"
                    : "=l"(entry) : "l"(particles), "0"(particleOffset) : "cc");
                if (entry)
                {
                    register MapOverlayParticle const * from asm("r1") = particle;
                    asm volatile(".syntax unified\n"
                        "\tldmia %1!, {r3, r4, r7}\n\tstmia %0!, {r3, r4, r7}\n"
                        "\tldmia %1!, {r3, r4, r7}\n\tstmia %0!, {r3, r4, r7}\n"
                        "\tldr %1, [sp, %2]\n\tstr %1, [%0]\n\t.syntax divided"
                        : "+l"(entry), "+l"(from)
                        : "i"(5 * sizeof(u32) + offsetof(MapOverlayParticle, yQ16))
                        : "r3", "r4", "r7", "memory");
                }
                ++particles->count;
            }
            register unsigned next asm("r4") = scratch.index + 1;
            asm("" : "+l"(next));
            scratch.index = next;
            register unsigned limit asm("r7");
            asm volatile("ldr %0, %1" : "=l"(limit) : "m"(particleCount));
            if (next >= limit)
                break;
        }
        while (true);
    }
    return overlay;
}

ROM_TEXT(0803B0A8) void DestroyGameStateMapOverlay(GameStateMapOverlay * overlay, unsigned destructorFlags)
{
    overlay->scene->vtable->releasePalette(overlay->scene, overlay->palettes.ids[0]);

    // The original compiler emitted this traversal even though particles own no tiles.
    GameStateMapOverlay::ParticleList & particles = overlay->particles;
    MapOverlayParticle * particlesEnd = particles.entries + particles.count;
    for (MapOverlayParticle * particle = particles.entries; particle != particlesEnd; ++particle)
    {
    }

    GameStateMapOverlay::FrameList & frames = overlay->frames;
    GameStateMapOverlay::Frame * framesEnd = frames.entries + frames.count;
    for (GameStateMapOverlay::Frame * frame = frames.entries; frame != framesEnd; ++frame)
    {
        OverlayTileAllocation * tiles = &frame->tiles;
        ReleaseOverlayTiles(tiles, tiles->handle);
        TileHeapOwner_Destroy(tiles, 2);
    }
    if (destructorFlags & 1)
        __builtin_delete(overlay);
}

typedef GameStateMapOverlay::Frame MapOverlayFrame;

ROM_TEXT(0803B128) void QueueGameStateMapOverlayGraphics(GameStateMapOverlay & inputOverlay, DmaTransferQueue & outputQueue)
{
    enum { OBJ_VRAM_BASE = 0x06010000, OBJ_TILE_BYTES = 32 };
    struct
    {
        DmaCommand command;
        unsigned count;
        unsigned inserted;
        GameStateMapOverlay * volatile overlay;
        MapOverlayFrame * volatile frameEnd;
        MapOverlayFrame * volatile nextFrame;
        DmaCommand * volatile insertionPoint;
    } local;
    local.overlay = &inputOverlay;
    DmaTransferQueue * queue;
    asm volatile(".syntax unified\n\tadds %0, %1, #0\n\t.syntax divided"
        : "=l"(queue) : "l"(&outputQueue) : "cc", "memory");
    register GameStateMapOverlay::FrameList * frames asm("r2");
    asm(".syntax unified\n\tadds %0, %1, #0\n\tadds %0, #4\n\t.syntax divided"
        : "=l"(frames) : "l"(&inputOverlay) : "cc");
    register MapOverlayFrame * frame asm("r5") = frames->entries;
    asm("" : "+l"(frame));
    register MapOverlayFrame * endFrame asm("r2");
    register unsigned frameCount asm("r1") = inputOverlay.frames.count;
    asm("" : "+l"(frameCount));
    asm(".syntax unified\n\tlsls r0, %2, #2\n\tadds r0, r0, %2\n\tlsls r0, r0, #3\n\tadds r0, #4\n\tadds %0, %1, r0\n\t.syntax divided"
        : "=l"(endFrame) : "0"(frames), "l"(frameCount) : "r0", "cc");
    local.frameEnd = endFrame;
    if (frame != endFrame)
        do
        {
            register FixedOverlayFrameSlice<u8> const * tileData asm("r0");
            register u8 const * source asm("r4");
            asm(".syntax unified\n\tadds %0, %2, #0\n\tadds %0, %3\n\tldr %1, [%2, %3]\n\t.syntax divided"
                : "=&l"(tileData), "=l"(source)
                : "l"(frame), "i"(offsetof(FixedOverlayFrameView, tileBytes)) : "cc", "memory");
            register unsigned bytes asm("r6") = 0;
            asm("" : "+l"(bytes));
            if (source)
                bytes = tileData->length;
            asm("" : "+l"(bytes));
            register MapOverlayFrame * next asm("r0") = frame + 1;
            asm("" : "+l"(next));
            local.nextFrame = next;
            if (bytes)
            {
                register OverlayTileAllocation * allocation asm("r0") =
                    reinterpret_cast<OverlayTileAllocation *>(reinterpret_cast<u8 *>(next) - sizeof(OverlayTileAllocation));
                asm("" : "+l"(allocation));
                register unsigned tile asm("r2") = GetOverlayBaseTile(allocation, allocation->handle);
                asm("" : "+l"(tile));
                register unsigned tileOffset asm("r2") = tile * OBJ_TILE_BYTES;
                asm("" : "+l"(tileOffset));
                register unsigned vramBase asm("r1") = OBJ_VRAM_BASE;
                asm("" : "+l"(vramBase));
                DmaCommand_InitializeCopy(&local.command, source, reinterpret_cast<void *>(tileOffset + vramBase), bytes);
                register DmaCommand * command asm("r10");
                asm("mov %0, sp" : "=h"(command) : : "memory");
                register DmaCommand * position asm("r1") = queue->end;
                asm("" : "+l"(position));
                if (position != queue->capacity_end)
                {
                    if (position)
                        COPY_DMA_COMMAND(position, &local.command, r2, r3, r4);
                    ++queue->end;
                }
                else
                {
                    // Grow by the larger of the queue's size and the insertion count.
                    local.insertionPoint = position;
                    local.inserted = 1;
                    register DmaCommand * begin asm("r1") = queue->begin;
                    asm("" : "+l"(begin));
                    register DmaCommand * end asm("r6") = local.insertionPoint;
                    asm("" : "+l"(end));
                    register intptr_t occupiedBytes asm("r0") = reinterpret_cast<intptr_t>(end) - reinterpret_cast<intptr_t>(begin);
                    asm("" : "+l"(occupiedBytes));
                    register unsigned count asm("r4") = occupiedBytes >> 4;
                    asm("" : "+l"(count));
                    local.count = count;
                    register unsigned * insertCount asm("r2") = &local.inserted;
                    asm("" : "+l"(insertCount));
                    register unsigned * growthCount asm("r0") = &local.count;
                    asm("" : "+l"(growthCount));
                    asm(".syntax unified\n\tcmp %1, #1\n\tbhs 1f\n\tadds %0, %2, #0\n1:\n\t.syntax divided"
                        : "+l"(growthCount) : "l"(count), "l"(insertCount) : "cc");
                    unsigned growth = *growthCount;
                    register unsigned newCapacity asm("r0") = count + growth;
                    asm("" : "+l"(newCapacity));
                    register unsigned bufferBytes asm("r9");
                    register DmaCommand * buffer asm("r0");
                    if (newCapacity)
                    {
                        register unsigned bytes asm("r4") = newCapacity * sizeof(DmaCommand);
                        asm("" : "+l"(bytes));
                        buffer = static_cast<DmaCommand *>(malloc(bytes));
                        bufferBytes = bytes;
                        asm("" : "+h"(bufferBytes) : "l"(buffer));
                        if (!buffer)
                            buffer = static_cast<DmaCommand *>(RetryDmaAllocation(bufferBytes));
                        begin = queue->begin;
                        asm("" : "+l"(begin) : "l"(buffer));
                    }
                    else
                    {
                        buffer = 0;
                        asm("" : "+l"(buffer));
                        bufferBytes = reinterpret_cast<unsigned>(buffer);
                        asm("" : "+h"(bufferBytes));
                    }
                    register DmaCommand * allocated asm("r4") = buffer;
                    asm("" : "+l"(allocated));
                    register DmaCommand * newBuffer asm("r8") = allocated;
                    asm("" : "+h"(newBuffer));
                    register DmaCommand const * from asm("r2") = begin;
                    asm("" : "+l"(from));
                    register DmaCommand * to asm("r3") = newBuffer;
                    asm("" : "+l"(to));
                    register MapOverlayFrame * following asm("r5") = frame;
                    asm(".syntax unified\n\tadds %0, %2\n\t.syntax divided"
                        : "+l"(following) : "l"(to), "i"(sizeof(MapOverlayFrame)) : "cc");
                    local.nextFrame = following;
                    while (true)
                    {
                        register DmaCommand const * insertionPoint asm("r0") = local.insertionPoint;
                        asm("" : "+l"(insertionPoint));
                        if (from == insertionPoint)
                            break;
                        if (to)
                            COPY_DMA_COMMAND(to, from, r4, r5, r6);
                        ++from;
                        ++to;
                    }
                    register DmaCommand * after asm("r4") = to;
                    asm("" : "+l"(after));
                    register unsigned insertedCount asm("r0") = local.inserted;
                    asm("" : "+l"(insertedCount));
                    if (insertedCount == 1)
                    {
                        if (after)
                            COPY_DMA_COMMAND(after, command, r2, r3, r5);
                        ++after;
                    }
                    else
                    {
                        register unsigned remaining asm("r3") = insertedCount;
                        asm("" : "+l"(remaining));
                        register DmaCommand * target asm("r2") = after;
                        asm("" : "+l"(target));
                        while (remaining)
                        {
                            if (target)
                                COPY_DMA_COMMAND(target, command, r4, r5, r6);
                            --remaining;
                            ++target;
                        }
                        after = target;
                        asm("" : "+l"(after));
                    }
                    register DmaCommand * limit asm("r2") = queue->end;
                    asm("" : "+l"(limit));
                    register DmaCommand * old asm("r1") = queue->begin;
                    asm("" : "+l"(old));
                    register DmaCommand * item asm("r0") = old;
                    asm("" : "+l"(item));
                    // The original queue walks the old commands before freeing their storage.
                    if (old != limit)
                        do { ++item; } while (item != limit);
                    if (old)
                        free(old);
                    register uintptr_t capacityEnd asm("r0") = bufferBytes;
                    asm("add %0, %1" : "+l"(capacityEnd) : "h"(newBuffer));
                    register DmaCommand * replacement asm("r6") = newBuffer;
                    asm("" : "+l"(replacement));
                    queue->begin = replacement;
                    queue->end = after;
                    queue->capacity_end = reinterpret_cast<DmaCommand *>(capacityEnd);
                }
            }
            frame = local.nextFrame;
            asm("" : "+l"(frame));
            register MapOverlayFrame * last asm("r0") = local.frameEnd;
            asm("" : "+l"(last));
            if (frame == last)
                break;
        }
        while (true);
    register GameStateMapOverlay * owner asm("r1") = local.overlay;
    asm("" : "+l"(owner));
    register GameStateMapOverlay::FrameList * frameList asm("r1");
    register MapOverlayScene * resources asm("r0");
    asm(".syntax unified\n\tldmia %1!, {%0}\n\t.syntax divided"
        : "=l"(resources), "=l"(frameList) : "1"(owner) : "memory");
    register MapOverlaySceneVtable const * methods asm("r3") = resources->vtable;
    asm("" : "+l"(methods));
    register u16 const * colors asm("r1") =
        frameList->entries[0].view.paletteBytes.data;
    asm("" : "+l"(colors));
    register GameStateMapOverlay * paletteOwner asm("r2") = local.overlay;
    asm("" : "+l"(paletteOwner));
    register u8 const * ids asm("r2") = paletteOwner->palettes.ids;
    asm("" : "+l"(ids));
    register unsigned palette asm("r2") = *ids;
    asm("" : "+l"(palette));
    methods->setObjectPalette(resources, colors, palette, 1);
}

#undef COPY_DMA_COMMAND

ROM_TEXT(0803B454) MapOverlayParticle & InitMapOverlayParticle(MapOverlayParticle & particle,
    SpriteAnimator const & animation, int cameraX, int cameraY)
{
    particle.animation = animation;
    ResetMapOverlayParticle(particle, cameraX, cameraY);
    return particle;
}

ROM_TEXT(0803B454) void ResetMapOverlayParticle(MapOverlayParticle & particle, int cameraX, int cameraY)
{
    particle.xQ16 = (static_cast<u32>(cameraX) + rand() % 240) << 16;
    particle.yQ16 = (static_cast<u32>(cameraY) + rand() % 160) << 16;
    int sample = rand();
    int variation = 0x90;
    variation &= sample;
    particle.animation.speed_q8 = variation + 0x60;
}

struct GameScriptTextVariables
{
    void const * vtable;
    u8 * saveData;
    char labels[4][24];
};

EC char const gGameScriptTextVariablesVTable[];
EC char const gGoldenLumberNickname[];
EC char const gMissingHorseName[];
EC char const gMissingChildName[];
EC u8 * GetActiveRosterNpc(u8 *);
EC char * GetNamedNpcName(u8 *);

ROM_TEXT(0803B454) GameScriptTextVariables & InitializeGameScriptTextVariables(GameScriptTextVariables & self, u8 * saveData)
{
    self.vtable = gGameScriptTextVariablesVTable;
    self.saveData = saveData;
    char (* label)[24] = self.labels;
    for (int remaining = ARRAY_COUNT(self.labels) - 1; remaining != -1; ++label, --remaining)
        (*label)[0] = '\0';
    return self;
}

ROM_TEXT(0803B4DC) MessageScriptReplacement & ResolveGameScriptTextVariable(
    MessageScriptReplacement & output, GameScriptTextVariables const & lookup, u32 placeholder)
{
    enum
    {
        PLAYER_NAME = 0xFF21,
        HORSE_NAME = 0xFF22,
        DOG_NAME = 0xFF23,
        FARM_NAME = 0xFF24,
        FIRST_SCRIPT_LABEL = 0xFF25,
        SECOND_SCRIPT_LABEL = 0xFF26,
        THIRD_SCRIPT_LABEL = 0xFF27,
        FOURTH_SCRIPT_LABEL = 0xFF28,
        PLAYER_DIALOGUE_ALIAS = 0xFF29,
        CHILD_NAME = 0xFF2A,
        FIRST_SAVED_LABEL = 0xFF2B,
        SECOND_SAVED_LABEL = 0xFF2C,
        THIRD_SAVED_LABEL = 0xFF2D,
    };
    enum
    {
        FARM_OFFSET = 0x14,
        FARMER_OFFSET = 0x1BD8,
        DOG_OFFSET = 0x1C70,
        NPC_ROSTER_OFFSET = 0x1CD4,
        DIALOGUE_FLAGS_OFFSET = 0x21C8,
        GOLDEN_LUMBER_NICKNAME_BIT = 2,
        FIRST_SAVED_LABEL_OFFSET = 0x21E0,
        SECOND_SAVED_LABEL_OFFSET = 0x21F0,
        THIRD_SAVED_LABEL_OFFSET = 0x2200,
    };

    // Keep the shared copy paths in the original Thumb register layout.
    register MessageScriptReplacement * result asm("r6") = &output;
    asm("" : "+l"(result));
    register GameScriptTextVariables const * variables asm("r5") = &lookup;
    asm("" : "+l"(variables));
    char text[sizeof(output.text)];
    register char const * source asm("r5");
    register size_t length asm("r4");
    char * terminator;
    register u8 const * saveData asm("r0");
    register unsigned offset asm("r1");
    register char const * name asm("r0");
    switch (placeholder)
    {
    case PLAYER_NAME:
    {
        Farm const & farm = *reinterpret_cast<Farm const *>(variables->saveData + FARM_OFFSET);
        if (farm.HasGoldenLumberOnField()
            && static_cast<i32>(static_cast<u32>(variables->saveData[DIALOGUE_FLAGS_OFFSET])
                << (31 - GOLDEN_LUMBER_NICKNAME_BIT)) < 0)
        {
            source = gGoldenLumberNickname;
            goto copyName;
        }
        name = GetFarmerName(*reinterpret_cast<Farmer const *>(variables->saveData + FARMER_OFFSET));
        source = name;
        asm("" : "+l"(source), "+l"(name));
        goto returnedName;
    }
    case HORSE_NAME:
    {
        Horse const * horse = reinterpret_cast<Farm const *>(variables->saveData + FARM_OFFSET)->GetHorse();
        char * horseNameBuffer = text;
        source = horse ? horse->GetName() : gMissingHorseName;
        length = strlen(source);
        if (length > sizeof(text) - 1)
            length = sizeof(text) - 1;
        memcpy(horseNameBuffer, source, length);
        terminator = horseNameBuffer + length;
        goto terminate;
    }
    case DOG_NAME:
        name = reinterpret_cast<Dog const *>(variables->saveData + DOG_OFFSET)->GetName();
        source = name;
        asm("" : "+l"(source), "+l"(name));
        goto returnedName;
    case PLAYER_DIALOGUE_ALIAS:
        name = GetFarmerDialogueAlias(*reinterpret_cast<Farmer const *>(variables->saveData + FARMER_OFFSET));
        source = name;
        asm("" : "+l"(source), "+l"(name));
        goto returnedName;
    case CHILD_NAME:
    {
        u8 * child = GetActiveRosterNpc(variables->saveData + NPC_ROSTER_OFFSET);
        asm("" : "+l"(child));
        if (child)
        {
            name = GetNamedNpcName(GetActiveRosterNpc(variables->saveData + NPC_ROSTER_OFFSET));
            source = name;
            asm("" : "+l"(source), "+l"(name));
            goto returnedName;
        }
        source = gMissingChildName;
        length = strlen(source);
        if (length > sizeof(text) - 1)
            length = sizeof(text) - 1;
        memcpy(text, source, length);
        register char * buffer asm("r1") = text;
        asm volatile("" : : "l"(buffer) : "memory");
        register char * end asm("r0") = buffer + length;
        asm("" : "+l"(end));
        *end = static_cast<char>(reinterpret_cast<uintptr_t>(child)); // Reuse the null child pointer for the terminator.
        result->found = 1;
        strcpy(result->text, buffer);
        break;
    }
    case FARM_NAME:
        name = reinterpret_cast<Farm const *>(variables->saveData + FARM_OFFSET)->GetName();
        source = name;
        asm("" : "+l"(source), "+l"(name));
        goto returnedName;
    case FIRST_SCRIPT_LABEL:
    case SECOND_SCRIPT_LABEL:
    case THIRD_SCRIPT_LABEL:
    case FOURTH_SCRIPT_LABEL:
    {
        register unsigned found asm("r4") = 1;
        asm("" : "+l"(found));
        register unsigned relativeOffset asm("r1") = placeholder * sizeof(variables->labels[0]);
        asm("" : "+l"(relativeOffset));
        unsigned labelBias = offsetof(GameScriptTextVariables, labels) - FIRST_SCRIPT_LABEL * sizeof(variables->labels[0]);
        strcpy(text, reinterpret_cast<char const *>(relativeOffset + reinterpret_cast<uintptr_t>(variables) + labelBias));
        result->found = found;
        goto copyToOutput;
    }
    case FIRST_SAVED_LABEL:
        saveData = variables->saveData;
        offset = FIRST_SAVED_LABEL_OFFSET;
        asm("" : "+l"(saveData), "+l"(offset));
        goto savedLabel;
    case SECOND_SAVED_LABEL:
        saveData = variables->saveData;
        offset = SECOND_SAVED_LABEL_OFFSET;
        asm("" : "+l"(saveData), "+l"(offset));
        goto savedLabel;
    case THIRD_SAVED_LABEL:
        saveData = variables->saveData;
        offset = THIRD_SAVED_LABEL_OFFSET;
        asm("" : "+l"(saveData), "+l"(offset));
    savedLabel:
        source = reinterpret_cast<char const *>(saveData + offset);
        asm("" : "+l"(source));
    copyName:
        name = source;
        asm("" : "+l"(name) : "l"(source));
    returnedName:
        length = strlen(name);
        if (length > sizeof(text) - 1)
            length = sizeof(text) - 1;
        memcpy(text, source, length);
        terminator = text + length;
    terminate:
        *terminator = '\0';
        result->found = 1;
    copyToOutput:
        strcpy(result->text, text);
        break;
    default:
        text[0] = '\0';
        result->found = 0;
        strcpy(result->text, text);
        break;
    }
    return *result;
}

ROM_TEXT(0803B6B8) void SetGameScriptTextVariable(GameScriptTextVariables & self, unsigned index, char const * label)
{
    char buf[24];
    size_t length = strlen(label);
    if (length > 22)
        length = 22;
    memcpy(buf, label, length);
    buf[length] = '\0';
    unsigned offset = index * sizeof(self.labels[0]);
    char * record = reinterpret_cast<char *>(offset + reinterpret_cast<uintptr_t>(&self));
    strcpy(record + offsetof(GameScriptTextVariables, labels), buf);
}

struct DialogWindowTilemapTask
{
    void * previous;
    void * next;
    void const * vtable;
    int left;
    int top;
    unsigned width;
    unsigned height;
    enum
    {
        ScreenBlockBits = 5,
        FirstTileBits = 10,
        PaletteBankBits = 4,
    };
    unsigned screenBlock : ScreenBlockBits;
    unsigned firstTile : FirstTileBits;
    unsigned paletteBank : PaletteBankBits;
};

typedef char DialogWindowTilemapTaskSizeCheck[sizeof(DialogWindowTilemapTask) == 0x20 ? 1 : -1];

EC char const gDialogWindowTilemapTaskVtable[];
EC void DrawMessageFrame(u16 * tilemap, unsigned x, unsigned y, unsigned width, unsigned height,
    unsigned firstTile, unsigned paletteBank);
EC u16 const * const gUnk_080F9F78;

ROM_TEXT(0803B840) bool RunDialogWindowTilemapTask(DialogWindowTilemapTask & task)
{
    enum
    {
        VideoRam = 0x06000000,
        BackgroundPaletteRam = 0x05000000,
        ScreenBlockBytes = 0x800,
        PaletteBankBytes = 32,
        FillWords = 32 * 10 / 2,
        FillMode = 1 << 24,
        TileIndexMask = 0x3FF,
        CenterTileOffset = 3,
    };

    // Keep the original Thumb register allocation for the VRAM fill and palette copy.
    register unsigned tile asm("r2") = task.firstTile + CenterTileOffset;
    asm("" : "+r"(tile));
    register unsigned mask asm("r1") = TileIndexMask;
    asm("" : "+r"(mask) : "r"(tile));
    register unsigned tileMask asm("r0") = mask;
    asm("" : "+r"(tileMask));
    tile = (tile & tileMask) | (task.paletteBank << 12);
    register unsigned highHalf asm("r0") = tile << 16;
    asm("" : "+r"(highHalf) : "r"(tile));
    tile |= highHalf;

    register unsigned screenOffset asm("r1") = task.screenBlock * ScreenBlockBytes;
    asm("" : "+r"(screenOffset) : "r"(tile));
    register u16 * screen asm("r1") = reinterpret_cast<u16 *>(VideoRam + screenOffset);
    asm("" : "+r"(screen) : "r"(tile));
    u32 repeatedTile = tile;
    register u32 * source asm("r0") = &repeatedTile;
    asm("" : "+r"(source) : "m"(repeatedTile));
    CpuFastSet(source, screen, FillMode | FillWords);

    DrawMessageFrame(reinterpret_cast<u16 *>(VideoRam + task.screenBlock * ScreenBlockBytes),
        task.left, 0, task.width, task.height, task.firstTile, task.paletteBank);

    register unsigned paletteBank asm("r1") = task.paletteBank;
    asm("" : "+r"(paletteBank));
    register u16 const * palette asm("r0") = gUnk_080F9F78;
    asm("" : "+r"(palette) : "r"(paletteBank));
    CpuFastSet(palette, reinterpret_cast<u16 *>(BackgroundPaletteRam + paletteBank * PaletteBankBytes),
        PaletteBankBytes / sizeof(u32));
    return false;
}

ROM_TEXT(0803BF14) DialogWindowTilemapTask & InitializeDialogWindowTilemapTask(DialogWindowTilemapTask & task,
    int left, int top, unsigned width, unsigned height, unsigned screenBlock, unsigned firstTile, unsigned paletteBank)
{
    // Preserve the original order of the height store and stack argument loads.
    asm volatile("ldr r4, %0\n\tstr r4, [%1, %2]"
        : : "m"(height), "l"(&task), "i"(offsetof(DialogWindowTilemapTask, height)) : "r4", "memory");
    // Use a register pair so agbcc saves both r6 and r7.
    register u64 blockPalette asm("r6");
    register unsigned tile asm("r5");
    asm volatile("ldr %0, %2\n\tldr %1, %3\n\tldr %H0, %4"
        : "=l"(blockPalette), "=l"(tile)
        : "m"(screenBlock), "m"(firstTile), "m"(paletteBank));
    register unsigned block asm("r6");
    register unsigned palette asm("r7");
    // The low and high words of the pair are already in these registers.
    asm("" : "=l"(block), "=l"(palette) : "l"(blockPalette));
    task.previous = 0;
    task.next = 0;
    task.vtable = gDialogWindowTilemapTaskVtable;
    task.left = left;
    task.top = top;
    task.width = width;
    // Preserve the original byte, halfword, and word writes to the packed fields.
    enum
    {
        PackedFields = offsetof(DialogWindowTilemapTask, height) + sizeof(unsigned),
        ScreenBlockMask = (1 << DialogWindowTilemapTask::ScreenBlockBits) - 1,
        FirstTileShift = DialogWindowTilemapTask::ScreenBlockBits,
        FirstTileMask = (1 << DialogWindowTilemapTask::FirstTileBits) - 1,
        PaletteBankShift = FirstTileShift + DialogWindowTilemapTask::FirstTileBits,
        PaletteBankMask = (1 << DialogWindowTilemapTask::PaletteBankBits) - 1,
    };
    u8 * packed = reinterpret_cast<u8 *>(&task) + PackedFields;
    {
        register unsigned mask asm("r1") = ScreenBlockMask;
        asm("" : "+l"(mask));
        block &= mask;
        asm("" : "+l"(block));
        register unsigned previous asm("r2") = *packed;
        asm("" : "+l"(previous) : "l"(block));
        mask = ~ScreenBlockMask;
        asm("" : "+l"(mask) : "l"(previous));
        mask &= previous;
        mask |= block;
        *packed = mask;
    }
    {
        register unsigned maskValue asm("r2") = FirstTileMask;
        asm("" : "+l"(maskValue));
        register unsigned mask asm("r1") = maskValue;
        asm("" : "+l"(mask));
        tile = (tile & mask) << FirstTileShift;
        asm("" : "+l"(tile));
        register unsigned previous asm("r2") = *reinterpret_cast<u16 *>(packed);
        asm("" : "+l"(previous) : "l"(tile));
        mask = ~(FirstTileMask << FirstTileShift);
        asm("" : "+l"(mask) : "l"(previous));
        mask &= previous;
        mask |= tile;
        *reinterpret_cast<u16 *>(packed) = mask;
    }
    {
        register unsigned mask asm("r1") = PaletteBankMask;
        asm("" : "+l"(mask));
        palette &= mask;
        asm("" : "+l"(palette));
        register unsigned paletteBits asm("r3") = palette << PaletteBankShift;
        asm("" : "+l"(paletteBits));
        register unsigned previous asm("r1") = *reinterpret_cast<u32 *>(packed);
        asm("" : "+l"(previous) : "l"(paletteBits));
        register unsigned preservedMask asm("r2") = ~(PaletteBankMask << PaletteBankShift);
        asm("" : "+l"(preservedMask) : "l"(previous));
        previous &= preservedMask;
        previous |= paletteBits;
        *reinterpret_cast<u32 *>(packed) = previous;
    }
    return task;
}

struct PACKED ALIGN(4) DialogWindowLayers
{
    i16 x;
    i16 y;
    STRUCT_PAD(0x04, 0x0A);
    u16 : 7;
    u16 layerMode : 8;
    u16 state : 8;
    u8 : 1;
    u8 animationFrames;
    STRUCT_PAD(0x0E, 0x10);
    u8 layerList[0x1C];
    u8 backgroundLayer[0x18];
    DialogWindowTilemapTask tilemapTask;
};

struct DialogWindow
{
    DialogWindowLayers * inner;
    void const * vtable;
};

enum DialogWindowState
{
    DIALOG_WINDOW_CLOSED,
    DIALOG_WINDOW_OPENING,
    DIALOG_WINDOW_CLOSING,
    DIALOG_WINDOW_OPEN,
};

struct BackgroundVideoState;
struct DialogLayerRenderer;
struct TileTransferQueue;
struct OamBuffer;

EC int UpdateDialogWindowLayers(DialogWindowLayers &, BackgroundVideoState *, DialogLayerRenderer *, TileTransferQueue *);
EC void TaskList_Append(void *, void *);

ROM_TEXT(0803BCF8) void OpenDialogWindowLayers(DialogWindowLayers & self)
{
    int state = self.state;
    if (state == DIALOG_WINDOW_CLOSED)
    {
        u32 layerMode = self.layerMode;
        if (layerMode != 1)
            TaskList_Append(self.layerList, self.backgroundLayer);
        TaskList_Append(self.layerList, &self.tilemapTask);
        self.animationFrames = state;
    }
    int nextState = self.state;
    if (nextState < 0)
        return;
    if (nextState > DIALOG_WINDOW_CLOSING)
        return;
    self.state = DIALOG_WINDOW_OPENING;
}

ROM_TEXT(0803BD60) void func_0803BD60(DialogWindowLayers & self)
{
    OpenDialogWindowLayers(self);
    self.state = DIALOG_WINDOW_OPEN;
    self.animationFrames = 9;
}

ROM_TEXT(0803BD60) void func_0803BD88(DialogWindowLayers & self)
{
    if (self.state)
        self.state = DIALOG_WINDOW_CLOSING;
}

ROM_TEXT(0803BD60) void func_0803BDB8(DialogWindowLayers & self)
{
    if (self.state)
        self.state = DIALOG_WINDOW_CLOSED;
}

ROM_TEXT(0803BD60) void func_0803BDE4(DialogWindowLayers & self, int x, int y)
{
    self.x = x;
    self.y = y;
}

ROM_TEXT(0803BD60) void func_0803BDEC(DialogWindowLayers & self, int x, int y)
{
    self.x += x;
    self.y += y;
}

EC char const vtable_unk_080E7778[];
EC DialogWindowLayers * InitializeDialogWindowLayers(void *, int, int, int, int, int, int, int, int);
EC void TaskNode_Destroy(void *, int);
EC void TaskList_Destroy(void *, int);

ROM_TEXT(0803BD60) DialogWindow & func_0803BDFC(DialogWindow & self, int x, int y, int width, int height, int arg5, int arg6, int arg7, int arg8)
{
    self.vtable = vtable_unk_080E7778;
    self.inner = InitializeDialogWindowLayers(::operator new(0x64), x, y, width, height, arg5, arg6, arg7, arg8);
    return self;
}

ROM_TEXT(0803BD60) void func_0803BE50(DialogWindow & self, int flags)
{
    self.vtable = vtable_unk_080E7778;
    if (DialogWindowLayers * inner = self.inner)
    {
        u8 * data = reinterpret_cast<u8 *>(inner);
        TaskNode_Destroy(data + 0x44, 2);
        TaskNode_Destroy(data + 0x2C, 2);
        TaskList_Destroy(data + 0x10, 2);
        ::operator delete(inner);
    }
    if (flags & 1)
        ::operator delete(&self);
}

ROM_TEXT(0803BD60) unsigned func_0803BE9C(DialogWindow const & self)
{
    return self.inner->state;
}

ROM_TEXT(0803BD60) int UpdateDialogWindow(DialogWindow & self, BackgroundVideoState * video,
    DialogLayerRenderer * layerRenderer, TileTransferQueue * tileTransfers)
{
    return UpdateDialogWindowLayers(*self.inner, video, layerRenderer, tileTransfers);
}

ROM_TEXT(0803BD60) void func_0803BEBC(DialogWindow & self)
{
    OpenDialogWindowLayers(*self.inner);
}

ROM_TEXT(0803BD60) void func_0803BEC8(DialogWindow & self)
{
    func_0803BD60(*self.inner);
}

ROM_TEXT(0803BD60) void func_0803BED4(DialogWindow & self)
{
    func_0803BD88(*self.inner);
}

ROM_TEXT(0803BD60) void func_0803BEE0(DialogWindow & self)
{
    func_0803BDB8(*self.inner);
}

ROM_TEXT(0803BD60) int func_0803BEEC(DialogWindow const & self)
{
    return self.inner->x;
}

ROM_TEXT(0803BD60) int func_0803BEF4(DialogWindow const & self)
{
    return self.inner->y;
}

ROM_TEXT(0803BD60) void func_0803BEFC(DialogWindow & self, int x, int y)
{
    func_0803BDE4(*self.inner, x, y);
}

ROM_TEXT(0803BD60) void func_0803BF08(DialogWindow & self, int x, int y)
{
    func_0803BDEC(*self.inner, x, y);
}

struct PauseMenuGraphics;

struct PauseMenuWindow : DialogWindow
{
    PauseMenuGraphics * graphics;
    u8 selectedTab;
    bool needsInitialRefresh;
    u8 cursorBlinkFrames;
    bool selectionEnabled;
};

ROM_TEXT(0803C518) void OpenPauseMenuWindow(PauseMenuWindow & self)
{
    func_0803BEBC(self);
    self.needsInitialRefresh = true;
    self.selectionEnabled = true;
}

ROM_TEXT(0803C518) void OpenPauseMenuWindowImmediately(PauseMenuWindow & self)
{
    func_0803BEC8(self);
    self.needsInitialRefresh = true;
    self.selectionEnabled = true;
}

ROM_TEXT(0803C518) u8 GetPauseMenuSelection(PauseMenuWindow const & self)
{
    return self.selectedTab;
}

ROM_TEXT(0803C518) void DisablePauseMenuSelection(PauseMenuWindow & self)
{
    self.selectionEnabled = false;
}


ROM_TEXT(0803D080) void SetClockSeasonTiles(ClockDisplay & self, Season season)
{
    ClockDisplayData & inner = *self.inner;
    u32 seasonTileIds[2];
    switch (season)
    {
    case SEASON_SPRING:
    default: seasonTileIds[0] = 11; seasonTileIds[1] = 12; break;
    case SEASON_SUMMER: seasonTileIds[0] = 13; seasonTileIds[1] = 14; break;
    case SEASON_AUTUMN: seasonTileIds[0] = 15; seasonTileIds[1] = 16; break;
    case SEASON_WINTER: seasonTileIds[0] = 17; seasonTileIds[1] = 18; break;
    }
    memcpy(inner.seasonFirstTiles,
           gClockFont[seasonTileIds[0]],
           sizeof(inner.seasonFirstTiles));
    inner.dateTilesDirty = 1;
    u8 const * secondTile = gClockFont[seasonTileIds[1]];
    memcpy(inner.seasonSecondTiles, secondTile, sizeof(inner.seasonSecondTiles));
    inner.dateTilesDirty = 1;
}

ROM_TEXT(0803D0F0) void SetClockDayAndWeekdayTiles(ClockDisplay & self, unsigned day, unsigned weekday)
{
    ClockDisplayData & inner = *self.inner;
    int tens = day / 10;
    unsigned ones = day % 10;
    u32 weekdayTileIds[8]; // Also used as the blank tens tile when the day is one digit.
    u8 * dateDirty;
    if (tens > 0)
    {
        u8 const * tile = gClockFont[tens];
        memcpy(inner.dayTensTiles, tile, sizeof(inner.dayTensTiles));
        dateDirty = &inner.dateTilesDirty;
    }
    else
    {
        memset(weekdayTileIds, 0, sizeof(weekdayTileIds));
        u8 * blankDest = inner.dayTensTiles;
        asm("" : "+r"(blankDest));
        memcpy(blankDest, weekdayTileIds, sizeof(inner.dayTensTiles));
        register unsigned dirtyOffset asm("r2") = offsetof(ClockDisplayData, dateTilesDirty);
        asm("" : "+r"(dirtyOffset));
        dateDirty = reinterpret_cast<u8 *>(&inner) + dirtyOffset;
    }
    *dateDirty = 1;
    u8 const * tile = gClockFont[ones];
    memcpy(inner.dayOnesTiles, tile, sizeof(inner.dayOnesTiles));
    inner.dateTilesDirty = 1;
    switch (weekday)
    {
    case 0:
    default: weekdayTileIds[0] = 0x60; weekdayTileIds[1] = 0x61; weekdayTileIds[2] = 0x62; break;
    case 1: weekdayTileIds[0] = 0x63; weekdayTileIds[1] = 0x64; weekdayTileIds[2] = 0x65; break;
    case 2: weekdayTileIds[0] = 0x66; weekdayTileIds[1] = 0x67; weekdayTileIds[2] = 0x68; break;
    case 3: weekdayTileIds[0] = 0x69; weekdayTileIds[1] = 0x6A; weekdayTileIds[2] = 0x6B; break;
    case 4: weekdayTileIds[0] = 0x6C; weekdayTileIds[1] = 0x6D; weekdayTileIds[2] = 0x6E; break;
    case 5: weekdayTileIds[0] = 0x6F; weekdayTileIds[1] = 0x70; weekdayTileIds[2] = 0x71; break;
    case 6: weekdayTileIds[0] = 0x72; weekdayTileIds[1] = 0x73; weekdayTileIds[2] = 0x74; break;
    }
    // Preserve the original tile copy order and register allocation.
    u8 * firstDest = inner.weekdayFirstTiles;
    register unsigned firstTileOffset asm("r1") = weekdayTileIds[0] * sizeof(inner.weekdayFirstTiles);
    asm("" : "+r"(firstTileOffset) : "r"(firstDest));
    u8 const * font = &gClockFont[0][0];
    memcpy(firstDest, reinterpret_cast<u8 const *>(firstTileOffset + reinterpret_cast<uintptr_t>(font)), sizeof(inner.weekdayFirstTiles));
    unsigned refreshOffset = offsetof(ClockDisplayData, tilesDirty);
    asm("" : "+r"(refreshOffset));
    u8 * dirty = reinterpret_cast<u8 *>(&inner) + refreshOffset;
    *dirty = 1;
    unsigned secondTileOffset = weekdayTileIds[1] * sizeof(inner.weekdaySecondTiles);
    register u8 const * secondTile asm("r1") = reinterpret_cast<u8 const *>(secondTileOffset + reinterpret_cast<uintptr_t>(font));
    asm("" : "+r"(secondTile));
    register unsigned secondDestOffset asm("r2") = refreshOffset
        - (offsetof(ClockDisplayData, tilesDirty) - offsetof(ClockDisplayData, weekdaySecondTiles));
    memcpy(reinterpret_cast<u8 *>(&inner) + secondDestOffset, secondTile, sizeof(inner.weekdaySecondTiles));
    *dirty = 1;
    unsigned thirdTileOffset = weekdayTileIds[2] * sizeof(inner.weekdayThirdTiles);
    register u8 const * thirdTile asm("r1") = reinterpret_cast<u8 const *>(thirdTileOffset + reinterpret_cast<uintptr_t>(font));
    register unsigned thirdDestOffset asm("r2") = offsetof(ClockDisplayData, weekdayThirdTiles);
    asm("" : "+r"(thirdDestOffset) : "r"(thirdTile));
    memcpy(reinterpret_cast<u8 *>(&inner) + thirdDestOffset, thirdTile, sizeof(inner.weekdayThirdTiles));
    *dirty = 1;
}

ROM_TEXT(0803D240) void SetClockHourTiles(ClockDisplay & self, unsigned hour)
{
    ClockDisplayData & inner = *self.inner;
    const unsigned pmTileId = 30;
    const unsigned amTileId = 28;
    unsigned meridiemTileId = pmTileId;
    if (hour <= 11)
        meridiemTileId = amTileId;
    u8 const * meridiemTile = gClockFont[meridiemTileId];
    memcpy(inner.meridiemTiles, meridiemTile, sizeof(inner.meridiemTiles));
    u8 * dirty = &inner.dateTilesDirty;
    *dirty = 1;
    // The clock keeps 12 at noon and subtracts 12 in the afternoon.
    register unsigned displayedHour asm("r4") = 12;
    u8 * dirtyForDigits = dirty;
    if (hour != 12)
    {
        displayedHour = hour;
        if (displayedHour > 11)
            displayedHour -= 12;
    }
    int tens = displayedHour / 10;
    displayedHour %= 10;
    u8 blank[0x20];
    u8 const * tensTile;
    u8 * tensDest;
    if (tens > 0)
    {
        tensTile = gClockFont[tens];
        tensDest = inner.hourTensTiles;
    }
    else
    {
        memset(blank, 0, sizeof(blank));
        tensDest = inner.hourTensTiles;
        tensTile = blank;
    }
    memcpy(tensDest, tensTile, sizeof(inner.hourTensTiles));
    {
        register u8 dirtyValue asm("r0") = 1;
        asm("" : "+r"(dirtyValue));
        *dirtyForDigits = dirtyValue;
    }
    u8 const * onesTile = gClockFont[displayedHour];
    memcpy(inner.hourOnesTiles, onesTile, sizeof(inner.hourOnesTiles));
    {
        register u8 dirtyValue asm("r0") = 1;
        asm("" : "+r"(dirtyValue));
        *dirtyForDigits = dirtyValue;
    }
}

ROM_TEXT(0803D2E8) void SetClockColonVisible(ClockDisplay & self, bool visible)
{
    ClockDisplayData & inner = *self.inner;
    if (visible)
    {
        uintptr_t tileBase = reinterpret_cast<uintptr_t>(gClockFont);
        u8 const * tile = reinterpret_cast<u8 const *>(tileBase + 10 * sizeof(inner.colonTiles));
        memcpy(inner.colonTiles, tile, sizeof(inner.colonTiles));
        inner.tilesDirty = 1;
    }
    else
    {
        u8 blank[0x20];
        memset(blank, 0, sizeof(blank));
        u8 * dest = reinterpret_cast<u8 *>(reinterpret_cast<uintptr_t>(&inner) + offsetof(ClockDisplayData, colonTiles));
        memcpy(dest, blank, sizeof(inner.colonTiles));
        inner.tilesDirty = 1;
    }
}

ROM_TEXT(0803D2E8) void SetClockMinuteTiles(ClockDisplay & self, unsigned minute)
{
    ClockDisplayData & inner = *self.inner;
    unsigned tens = minute / 10;
    unsigned ones = minute % 10;
    u8 const * tensTile = gClockFont[tens];
    memcpy(inner.minuteTensTiles, tensTile, sizeof(inner.minuteTensTiles));
    inner.tilesDirty = 1;
    u8 const * onesTile = gClockFont[ones];
    memcpy(inner.minuteOnesTiles, onesTile, sizeof(inner.minuteOnesTiles));
    inner.tilesDirty = 1;
}

ROM_TEXT(0803D2E8) void SetClockWeatherIcon(ClockDisplay & self, unsigned weather)
{
    ClockDisplayData & inner = *self.inner;
    unsigned current = inner.weatherIcon;
    switch (weather)
    {
        case 0: current = 0; break;
        case 1: current = 1; break;
        case 2: current = 2; break;
        case 3: current = 3; break;
        case 4: current = 4; break;
    }
    if (current != inner.weatherIcon)
    {
        inner.weatherIcon = current;
        inner.weatherIconDirty = 1;
    }
}

ROM_TEXT(0803D2E8) void SetClockToolIcon(ClockDisplay & self, int iconId)
{
    ClockDisplayData * inner = self.inner;
    if (!inner->toolIconVisible || iconId != inner->toolIconId)
    {
        inner->toolIconVisible = 1;
        inner->toolIconId = iconId;
        inner->toolIconDirty = 1;
    }
}

ROM_TEXT(0803D2E8) void HideClockToolIcon(ClockDisplay & self)
{
    ClockDisplayData & inner = *self.inner;
    if (inner.toolIconVisible)
    {
        inner.toolIconVisible = 0;
        inner.toolIconDirty = 1;
    }
}

struct SavedCalendarDate;
EC void SetClockDateAndTimeTiles(ClockDisplay &, SavedCalendarDate const &, unsigned colonVisible);

struct ClockHud : DialogWindow
{
    ClockDisplay display;
};

ROM_TEXT(0803D464) void ClockHud_HideToolIcon(ClockHud & hud)
{
    HideClockToolIcon(hud.display);
}

ROM_TEXT(0803D464) void ClockHud_SetToolIcon(ClockHud & hud, int iconId)
{
    SetClockToolIcon(hud.display, iconId);
}

ROM_TEXT(0803D464) void ClockHud_SetWeatherIcon(ClockHud & hud, unsigned weather)
{
    SetClockWeatherIcon(hud.display, weather);
}

ROM_TEXT(0803D464) void ClockHud_SetMinute(ClockHud & hud, unsigned minute)
{
    SetClockMinuteTiles(hud.display, minute);
}

ROM_TEXT(0803D464) void ClockHud_SetColonVisible(ClockHud & hud, bool visible)
{
    SetClockColonVisible(hud.display, visible);
}

ROM_TEXT(0803D464) void ClockHud_SetHour(ClockHud & hud, unsigned hour)
{
    SetClockHourTiles(hud.display, hour);
}

ROM_TEXT(0803D464) void ClockHud_SetDayAndWeekday(ClockHud & hud, unsigned day, unsigned weekday)
{
    SetClockDayAndWeekdayTiles(hud.display, day, weekday);
}

ROM_TEXT(0803D464) void ClockHud_SetSeason(ClockHud & hud, Season season)
{
    SetClockSeasonTiles(hud.display, season);
}

ROM_TEXT(0803D464) void ClockHud_SetDateAndTime(ClockHud & hud, SavedCalendarDate const & calendar, bool colonVisible)
{
    SetClockDateAndTimeTiles(hud.display, calendar, colonVisible);
}

EC u8 const gNpcScheduleFestivalDays[14];
EC u8 const * func_080E0E80(u8 const *, u8 const *, unsigned const &, int, int);

ROM_TEXT(0803D644) bool IsNpcScheduleFestivalDay(unsigned season, unsigned day)
{
    unsigned dayOfYear = season * 30;
    dayOfYear--;
    dayOfYear += day;
    u8 const * begin = gNpcScheduleFestivalDays;
    u8 const * end = begin + ARRAY_COUNT(gNpcScheduleFestivalDays);
    u8 const * found = func_080E0E80(begin, end, dayOfYear, 0, 0);
    return found != end && dayOfYear >= *found;
}

struct NpcScheduleRoster
{
    STRUCT_PAD(0x000, 0x070);
    Npc npc_070;
    Npc npc_084;
    Npc popuri;
    STRUCT_PAD(0x0AC, 0x0B0);
    Npc npc_0B0;
    Npc npc_0C4;
    Npc npc_0D8;
    STRUCT_PAD(0x0EC, 0x0F0);
    Npc npc_0F0;
    Npc npc_104;
    Npc npc_118;
    Npc npc_12C;
    Npc npc_140;
    Npc mary;
    STRUCT_PAD(0x168, 0x16C);
    Npc npc_16C;
    Npc npc_180;
    Npc npc_194;
    Npc npc_1A8;
    Npc npc_1BC;
    Npc npc_1D0;
    Npc karen;
    STRUCT_PAD(0x1F8, 0x1FC);
    Npc npc_1FC;
    Npc elli;
    STRUCT_PAD(0x224, 0x228);
    Npc npc_228;
    Npc npc_23C;
    Npc npc_250;
    Npc ann;
    STRUCT_PAD(0x278, 0x27C);
    Npc npc_27C;
    Npc npc_290;
    STRUCT_PAD(0x2A4, 0x2A8);
    Npc npc_2A8;
    Npc npc_2BC;
    STRUCT_PAD(0x2D0, 0x310);
    Npc npc_310;
    STRUCT_PAD(0x324, 0x328);
    Npc npc_328;
};

EC ScheduleInfo const gUnk_080F280C;
extern ScheduleInfo const ScheduleInfo_Unk_080F1A80;
EC ScheduleInfo const gPopuriScheduleInfo;
EC ScheduleInfo const gUnk_080F8678;
EC ScheduleInfo const gUnk_080F81BC;
EC ScheduleInfo const gUnk_080F77FC;
EC ScheduleInfo const gUnk_080F7294;
EC ScheduleInfo const gUnk_080F6370;
EC ScheduleInfo const gUnk_080F66C4;
EC ScheduleInfo const gUnk_080F49C0;
EC ScheduleInfo const gUnk_080F5540;
EC ScheduleInfo const gMaryScheduleInfo;
EC ScheduleInfo const gUnk_080F59CC;
EC ScheduleInfo const gUnk_080F6B4C;
EC ScheduleInfo const gUnk_080F33B8;
EC ScheduleInfo const gUnk_080F61FC;
EC ScheduleInfo const gUnk_080F3408;
EC ScheduleInfo const gUnk_080F3FD8;
EC ScheduleInfo const gKarenScheduleInfo;
EC ScheduleInfo const gUnk_080F3010;
EC ScheduleInfo const gElliScheduleInfo;
EC ScheduleInfo const gUnk_080F6DE8;
EC ScheduleInfo const gCliffScheduleInfo;
EC ScheduleInfo const gUnk_080F42F0;
EC ScheduleInfo const gAnnScheduleInfo;
EC ScheduleInfo const gUnk_080F6FF8;
EC ScheduleInfo const gCarpenterScheduleInfo;
EC ScheduleInfo const gUnk_080F2DC0;
EC ScheduleInfo const gUnk_080F597C;
EC ScheduleInfo const gUnk_080F6B10;
EC ScheduleInfo const gUnk_080F4974;
EC ScheduleInfo const gChildScheduleInfo;
EC Npc * GetActiveScheduleNpc(NpcScheduleRoster &);
ROM_TEXT(0803D688) void ResetNpcDailySchedule(Npc & npc, ScheduleInfo const & info, void const * context)
{
    unsigned scheduleIndex = info.select_schedule(context);
    PathInfo const * firstPath = nullptr;
    if (info.schedules)
    {
        Schedule const * schedule = info.schedules[scheduleIndex];
        if (schedule && schedule->num_time_entries && schedule->time_entries)
            firstPath = schedule->time_entries[0].path;
    }

    ActorLocation start = firstPath
        ? ActorLocation(Location(firstPath->location_start, firstPath->x_start, firstPath->y_start),
            firstPath->facing_start)
        : ActorLocation(Location(MAP_NONE, 0, 0), 0);
    npc.SetLocation(start);
    npc.schedule_index = scheduleIndex;
    npc.schedule_path_index = 0;
    npc.path_point_index = 0;
    npc.path_point_elapsed_ticks = 0;
}

ROM_TEXT(0803D7E4) void ResetAllNpcDailySchedules(NpcScheduleRoster & self, void const * context)
{
    ResetNpcDailySchedule(self.npc_070, gUnk_080F280C, context);
    ResetNpcDailySchedule(self.npc_084, ScheduleInfo_Unk_080F1A80, context);
    ResetNpcDailySchedule(self.popuri, gPopuriScheduleInfo, context);
    ResetNpcDailySchedule(self.npc_0B0, gUnk_080F8678, context);
    ResetNpcDailySchedule(self.npc_0C4, gUnk_080F81BC, context);
    ResetNpcDailySchedule(self.npc_0D8, gUnk_080F77FC, context);
    ResetNpcDailySchedule(self.npc_0F0, gUnk_080F7294, context);
    ResetNpcDailySchedule(self.npc_104, gUnk_080F6370, context);
    ResetNpcDailySchedule(self.npc_118, gUnk_080F66C4, context);
    ResetNpcDailySchedule(self.npc_12C, gUnk_080F49C0, context);
    ResetNpcDailySchedule(self.npc_140, gUnk_080F5540, context);
    ResetNpcDailySchedule(self.mary, gMaryScheduleInfo, context);
    ResetNpcDailySchedule(self.npc_16C, gUnk_080F59CC, context);
    ResetNpcDailySchedule(self.npc_180, gUnk_080F6B4C, context);
    ResetNpcDailySchedule(self.npc_194, gUnk_080F33B8, context);
    ResetNpcDailySchedule(self.npc_1A8, gUnk_080F61FC, context);
    ResetNpcDailySchedule(self.npc_1BC, gUnk_080F3408, context);
    ResetNpcDailySchedule(self.npc_1D0, gUnk_080F3FD8, context);
    ResetNpcDailySchedule(self.karen, gKarenScheduleInfo, context);
    ResetNpcDailySchedule(self.npc_1FC, gUnk_080F3010, context);
    ResetNpcDailySchedule(self.elli, gElliScheduleInfo, context);
    ResetNpcDailySchedule(self.npc_228, gUnk_080F6DE8, context);
    ResetNpcDailySchedule(self.npc_23C, gCliffScheduleInfo, context);
    ResetNpcDailySchedule(self.npc_250, gUnk_080F42F0, context);
    ResetNpcDailySchedule(self.ann, gAnnScheduleInfo, context);
    ResetNpcDailySchedule(self.npc_27C, gUnk_080F6FF8, context);
    ResetNpcDailySchedule(self.npc_290, gCarpenterScheduleInfo, context);
    ResetNpcDailySchedule(self.npc_2A8, gUnk_080F2DC0, context);
    ResetNpcDailySchedule(self.npc_2BC, gUnk_080F597C, context);
    ResetNpcDailySchedule(self.npc_310, gUnk_080F6B10, context);
    ResetNpcDailySchedule(self.npc_328, gUnk_080F4974, context);

    if (Npc * npc = GetActiveScheduleNpc(self))
        ResetNpcDailySchedule(*npc, gChildScheduleInfo, context);
}

struct PACKED SavedCalendarDate
{
    u8 yearInCycle : 3;
    u8 yearCycle : 5;
    u8 packedDate;
    u8 packedTime[2];
};

enum NpcScheduleWeekday
{
    SCHEDULE_SUNDAY,
    SCHEDULE_MONDAY,
    SCHEDULE_TUESDAY,
    SCHEDULE_WEDNESDAY,
    SCHEDULE_THURSDAY,
    SCHEDULE_FRIDAY,
    SCHEDULE_SATURDAY,
};

struct CalendarDate
{
    u8 year;
    u8 season;
    u8 day;
    u8 weekday;
};

struct PACKED EventScheduleFlags
{
    u32 : 19;
    u32 unk_217A_3 : 2;
};

union PACKED BacheloretteScheduleEvents
{
    u8 bytes[2];
    struct PACKED
    {
        u16 : 7;
        u16 ann : 2;
        u16 : 7;
    } states;
};

struct NpcScheduleContext
{
    STRUCT_PAD(0x0000, 0x0008);
    u32 weather;
    STRUCT_PAD(0x000C, 0x0010);
    SavedCalendarDate calendar;
    STRUCT_PAD(0x0014, 0x1CD4);
    u8 unk_1CD4[4];
    STRUCT_PAD(0x1CD8, 0x1D6C);
    Bachelorette popuri;
    STRUCT_PAD(0x1D84, 0x1DAC);
    u32 unk_1DAC[2];
    STRUCT_PAD(0x1DB4, 0x1E28);
    Bachelorette mary;
    STRUCT_PAD(0x1E40, 0x1EB8);
    Bachelorette karen;
    STRUCT_PAD(0x1ED0, 0x1EE4);
    Bachelorette elli;
    STRUCT_PAD(0x1EFC, 0x1F38);
    Bachelorette ann;
    STRUCT_PAD(0x1F50, 0x2148);
    u8 marriedScheduleVariant;
    STRUCT_PAD(0x2149, 0x215A);
    u8 unk_215A;
    STRUCT_PAD(0x215B, 0x216B);
    BacheloretteScheduleEvents marriageScheduleEvents;
    STRUCT_PAD(0x216D, 0x2174);
    u32 unk_2174;
    union
    {
        u32 unk_2178;
        EventScheduleFlags scheduleFlags;
    };
    STRUCT_PAD(0x217C, 0x2181);
    u8 cliffEventStates;
    STRUCT_PAD(0x2182, 0x2186);
    u8 : 5;
    u8 unk_2186_5 : 2;
    u8 : 1;
    u8 : 3;
    u8 unk_2187_3 : 2;
    u8 : 3;
    STRUCT_PAD(0x2188, 0x21A2);
    u8 unk_21A2;
    BacheloretteScheduleEvents seasonalScheduleEvents;
    u8 maryWinterScheduleEvent;
    STRUCT_PAD(0x21A6, 0x21CA);
    u8 eventFlags;
};

#define EXTRACT_CALENDAR_FIELD(output, input, offset, width) \
    asm(".syntax unified\n\tlsls %0, %1, %2\n\tlsrs %0, %0, %3\n\t.syntax divided" \
        : "=l"(output) : "l"(input), "I"(32 - (offset) - (width)), "I"(32 - (width)))

ROM_TEXT(0803D52C) CalendarDate & DecodeNpcScheduleDate(CalendarDate & result, SavedCalendarDate const & calendar)
{
    enum
    {
        DAYS_PER_SEASON = 30,
        DAYS_PER_YEAR = 4 * DAYS_PER_SEASON,
        DAYS_PER_WEEK = 7,
        YEARS_PER_CALENDAR_CYCLE = 7,
        WEEKDAY_YEAR_OFFSET = 6,
        MAX_YEAR = 200,
        SCHEDULE_DAY_START_HOUR = 6,
        SEASON_BIT = 0,
        SEASON_WIDTH = 2,
        SEASON_MASK = (1u << SEASON_WIDTH) - 1,
        DAY_BIT = 2,
        DAY_WIDTH = 5,
        DAY_MASK = (1u << DAY_WIDTH) - 1,
        WEEKDAY_WIDTH = 3,
        WEEKDAY_MASK = (1u << WEEKDAY_WIDTH) - 1,
        HOUR_BIT = 16,
        HOUR_WIDTH = 5,
    };
    struct LocalDate
    {
        unsigned maxYear;
        unsigned year;
        u8 date;
    } local;
    // Register and stack constraints below preserve agbcc's original Thumb layout.
    asm("" : : : "r9", "r10");

    unsigned dateByte = calendar.packedDate;
    u8 * dateBuffer = &local.date;
    *dateBuffer = dateByte;

    unsigned yearInCycle = calendar.yearInCycle;
    unsigned season;
    EXTRACT_CALENDAR_FIELD(season, dateByte, SEASON_BIT, SEASON_WIDTH);
    register unsigned dayCount asm("r3");
    EXTRACT_CALENDAR_FIELD(dayCount, dateByte, DAY_BIT, DAY_WIDTH);
    yearInCycle += WEEKDAY_YEAR_OFFSET;
    dayCount += season * DAYS_PER_SEASON + yearInCycle * DAYS_PER_YEAR;
    register u8 * date asm("r8");
    date = dateBuffer;
    asm("" : "+r"(date));
    if (dayCount > DAYS_PER_WEEK - 1)
        dayCount %= DAYS_PER_WEEK;
    // The original keeps the weekday in the low three bits of a register.
    unsigned weekdayStorage;
    asm("" : "=l"(weekdayStorage));
    unsigned weekdayMask = ~unsigned(WEEKDAY_MASK);
    asm("" : "+l"(weekdayMask));
    unsigned weekdayLow = dayCount & WEEKDAY_MASK;
    asm("and %0, %1" : "+l"(weekdayMask) : "l"(weekdayStorage));
    weekdayStorage = weekdayMask | weekdayLow;
    asm("" : "+l"(weekdayStorage) : : "memory");

    unsigned maxYear = MAX_YEAR;
    local.maxYear = maxYear;
    unsigned actualYear = calendar.yearCycle * unsigned(YEARS_PER_CALENDAR_CYCLE)
        + calendar.yearInCycle;
    local.year = actualYear;
    unsigned * actualYearAddress = &local.year;
    register unsigned * yearAddress asm("r2") = &local.maxYear;
    if (maxYear > actualYear)
        yearAddress = actualYearAddress;
    register unsigned year asm("r4") = *yearAddress;
    unsigned packedCalendar;
    asm("ldr %0, [%1]" : "=l"(packedCalendar) : "l"(&calendar) : "memory");
    unsigned hour;
    EXTRACT_CALENDAR_FIELD(hour, packedCalendar, HOUR_BIT, HOUR_WIDTH);
    // NPC schedules still use the previous date before 6 AM.
    if (hour < SCHEDULE_DAY_START_HOUR)
    {
        u8 * rolledDate;
        asm("mov %0, %1" : "=l"(rolledDate) : "r"(date));
        unsigned currentDateByte = *rolledDate;
        unsigned dayShift;
        register unsigned previousDay asm("r2");
        asm("lsl %0, %2, %3; lsr %1, %0, %4"
            : "=&l"(dayShift), "=l"(previousDay)
            : "l"(currentDateByte), "I"(32 - DAY_BIT - DAY_WIDTH), "I"(32 - DAY_WIDTH));
        if (previousDay == 0)
        {
            unsigned previousSeason;
            EXTRACT_CALENDAR_FIELD(previousSeason, currentDateByte, SEASON_BIT, SEASON_WIDTH);
            unsigned seasonMask = ~unsigned(SEASON_MASK);
            --previousSeason;
            unsigned seasonBits = SEASON_MASK;
            previousSeason &= seasonBits;
            seasonMask &= currentDateByte;
            seasonMask |= previousSeason;
            *rolledDate = seasonMask;
            previousDay = DAYS_PER_SEASON - 1;
        }
        else
        {
            --previousDay;
        }
        register unsigned dayMask asm("r0") = DAY_MASK;
        asm("" : "+l"(dayMask));
        previousDay = (previousDay & dayMask) << DAY_BIT;
        unsigned unchangedDate = *rolledDate;
        unsigned preserveOtherDateBits = ~(unsigned(DAY_MASK) << DAY_BIT);
        preserveOtherDateBits &= unchangedDate;
        *rolledDate = preserveOtherDateBits | previousDay;

        u8 const * normalizedDate;
        asm("mov %0, %1" : "=l"(normalizedDate) : "r"(date));
        unsigned normalizedDateByte = *normalizedDate;
        unsigned normalizedSeason;
        EXTRACT_CALENDAR_FIELD(normalizedSeason, normalizedDateByte, SEASON_BIT, SEASON_WIDTH);
        if (normalizedSeason == SEASON_WINTER)
        {
            unsigned normalizedDay;
            EXTRACT_CALENDAR_FIELD(normalizedDay, normalizedDateByte, DAY_BIT, DAY_WIDTH);
            if (normalizedDay == DAYS_PER_SEASON - 1)
                --year;
        }

        int weekday;
        EXTRACT_CALENDAR_FIELD(weekday, weekdayStorage, 0, WEEKDAY_WIDTH);
        int previousWeekday = DAYS_PER_WEEK - 1;
        if (weekday > SCHEDULE_MONDAY)
            previousWeekday = weekday - 1;
        unsigned weekdayBits = WEEKDAY_MASK;
        asm("" : "+l"(weekdayBits));
        previousWeekday &= weekdayBits;
        register unsigned preserveOtherWeekdayBits asm("r0") = ~unsigned(WEEKDAY_MASK);
        preserveOtherWeekdayBits &= weekdayStorage;
        weekdayStorage = preserveOtherWeekdayBits | previousWeekday;
    }

    result.year = year;
    register u8 const * scheduleDate asm("r1");
    asm("mov %0, %1" : "=l"(scheduleDate) : "r"(date));
    unsigned scheduleSeasonByte = *scheduleDate;
    asm("" : "+l"(scheduleSeasonByte));
    unsigned scheduleSeason;
    EXTRACT_CALENDAR_FIELD(scheduleSeason, scheduleSeasonByte, SEASON_BIT, SEASON_WIDTH);
    result.season = scheduleSeason;
    u8 const * dayAddress;
    asm("add %0, sp, %1"
        : "=l"(dayAddress) : "I"(offsetof(LocalDate, date)) : "memory");
    unsigned scheduleDayByte = *dayAddress;
    unsigned scheduleDay;
    EXTRACT_CALENDAR_FIELD(scheduleDay, scheduleDayByte, DAY_BIT, DAY_WIDTH);
    result.day = scheduleDay + 1;
    unsigned scheduleWeekday;
    EXTRACT_CALENDAR_FIELD(scheduleWeekday, weekdayStorage, 0, WEEKDAY_WIDTH);
    result.weekday = scheduleWeekday;
    return result;
}

ROM_TEXT(0803CD4C) void SetClockDateAndTimeTiles(ClockDisplay & display, SavedCalendarDate const & calendar, unsigned colonVisible)
{
    enum
    {
        SEASON_BIT = 0,
        SEASON_WIDTH = 2,
        DAY_BIT = 2,
        DAY_WIDTH = 5,
        HOUR_BIT = 16,
        HOUR_WIDTH = 5,
        MINUTE_BIT = 21,
        MINUTE_WIDTH = 6,
        WEEKDAY_YEAR_OFFSET = 6,
        DAYS_PER_SEASON = 30,
        DAYS_PER_YEAR = 4 * DAYS_PER_SEASON,
        DAYS_PER_WEEK = 7,
        WEEKDAY_MASK = 7,
        HOURS_PER_HALF_DAY = 12,
        MINUTES_PER_DISPLAY_STEP = 10,
        AM_TILE = 28,
        PM_TILE = 30,
        COLON_TILE = 10,
    };
    struct ClockTileLocals
    {
        unsigned weekdayTiles[CLOCK_TILE_BYTES / sizeof(unsigned)];
        SavedCalendarDate const * calendar;
        unsigned showColon;
        STRUCT_PAD(0x28, 0x2C);
        u8 * timeDirty;
    } local;
    // The scratch tile and register constraints preserve the original Thumb layout.
    local.calendar = &calendar;
    asm("" : : "m"(local.calendar) : "memory");
    register unsigned colonFlag asm("r2");
    EXTRACT_CALENDAR_FIELD(colonFlag, colonVisible, 0, 8);
    local.showColon = colonFlag;
    asm("" : : "m"(local.calendar), "m"(local.showColon) : "memory");
    ClockDisplayData * clock = display.inner;
    unsigned packedSeason = calendar.packedDate;
    asm("" : "+l"(packedSeason));
    unsigned season;
    EXTRACT_CALENDAR_FIELD(season, packedSeason, SEASON_BIT, SEASON_WIDTH);
    register unsigned firstSeasonTile asm("r5");
    unsigned secondSeasonTile;
    switch (int(season))
    {
    case SEASON_SPRING:
    default: firstSeasonTile = 11; secondSeasonTile = 12; break;
    case SEASON_SUMMER: firstSeasonTile = 13; secondSeasonTile = 14; break;
    case SEASON_AUTUMN: firstSeasonTile = 15; secondSeasonTile = 16; break;
    case SEASON_WINTER: firstSeasonTile = 17; secondSeasonTile = 18; break;
    }
    asm("" : "+l"(firstSeasonTile), "+l"(secondSeasonTile));
    u8 * firstSeasonDest = clock->seasonFirstTiles;
    unsigned firstSeasonOffset = firstSeasonTile * sizeof(ClockTile);
    ClockTile const * font = gClockFont;
    asm("" : "+r"(font));
    memcpy(firstSeasonDest, reinterpret_cast<u8 const *>(font) + firstSeasonOffset, sizeof(ClockTile));
    u8 * dateDirty = &clock->dateTilesDirty;
    register unsigned firstDirty asm("r1") = 1;
    asm("" : "+l"(firstDirty));
    unsigned initialDirty = firstDirty;
    asm("" : "+r"(initialDirty));
    register unsigned markSeasonFirst asm("r2");
    asm("mov %0, %1" : "=l"(markSeasonFirst) : "r"(initialDirty));
    *dateDirty = markSeasonFirst;
    u8 const * secondSeasonTileData = font[secondSeasonTile];
    memcpy(clock->seasonSecondTiles, secondSeasonTileData, sizeof(ClockTile));
    unsigned markSeasonSecond;
    asm("mov %0, %1" : "=l"(markSeasonSecond) : "r"(initialDirty));
    *dateDirty = markSeasonSecond;

    SavedCalendarDate const * saved = local.calendar;
    unsigned packedDate = saved->packedDate;
    register unsigned dayExtracted asm("r0");
    EXTRACT_CALENDAR_FIELD(dayExtracted, packedDate, DAY_BIT, DAY_WIDTH);
    dayExtracted++;
    asm("" : "+l"(dayExtracted));
    unsigned day = dayExtracted;
    unsigned yearInCycle = saved->yearInCycle;
    unsigned daySeason;
    EXTRACT_CALENDAR_FIELD(daySeason, packedDate, SEASON_BIT, SEASON_WIDTH);
    yearInCycle += WEEKDAY_YEAR_OFFSET;
    unsigned weekday = daySeason * DAYS_PER_SEASON + yearInCycle * DAYS_PER_YEAR + day;
    weekday--;
    register u8 * dateUploadDirty asm("r9") = dateDirty;
    asm("" : "+r"(dateUploadDirty));
    if (weekday >= DAYS_PER_WEEK)
        weekday %= DAYS_PER_WEEK;
    register unsigned weekdayMask asm("r0") = WEEKDAY_MASK;
    asm("" : "+l"(weekdayMask));
    unsigned weekdayIndex = weekdayMask & weekday;
    int dayTens = day / 10;
    day %= 10;
    if (dayTens > 0)
    {
        u8 const * dayTensTile = font[dayTens];
        memcpy(clock->dayTensTiles, dayTensTile, sizeof(ClockTile));
        unsigned markDayTens;
        register u8 * dayTensDirty asm("r2");
        asm("mov %0, %2; mov %1, %3" : "=l"(markDayTens), "=l"(dayTensDirty)
            : "r"(initialDirty), "r"(dateUploadDirty));
        *dayTensDirty = markDayTens;
    }
    else
    {
        memset(local.weekdayTiles, 0, sizeof(ClockTile));
        u8 * blankDayTensDest = clock->dayTensTiles;
        u8 const * blankDayTens = reinterpret_cast<u8 const *>(local.weekdayTiles);
        memcpy(blankDayTensDest, blankDayTens, sizeof(ClockTile));
        register unsigned markBlankDayTens asm("r2");
        register u8 * blankDayTensDirty asm("r1");
        asm("mov %0, %2; mov %1, %3" : "=l"(markBlankDayTens), "=l"(blankDayTensDirty)
            : "r"(initialDirty), "r"(dateUploadDirty));
        *blankDayTensDirty = markBlankDayTens;
    }
    unsigned dayOnesOffset = day * sizeof(ClockTile);
    register ClockTile const * dayOnesFont asm("r0") = gClockFont;
    u8 const * dayOnesTile;
    asm(".syntax unified; adds %0, %1, %2; .syntax divided"
        : "=l"(dayOnesTile) : "l"(dayOnesOffset), "l"(dayOnesFont));
    memcpy(clock->dayOnesTiles, dayOnesTile, sizeof(ClockTile));
    unsigned markDayOnes = 1;
    register u8 * dayOnesDirty asm("r1");
    asm("mov %0, %1" : "=l"(dayOnesDirty) : "r"(dateUploadDirty));
    *dayOnesDirty = markDayOnes;
    switch (weekdayIndex)
    {
    case 0:
    default: local.weekdayTiles[0] = 0x60; local.weekdayTiles[1] = 0x61; local.weekdayTiles[2] = 0x62; break;
    case 1: local.weekdayTiles[0] = 0x63; local.weekdayTiles[1] = 0x64; local.weekdayTiles[2] = 0x65; break;
    case 2: local.weekdayTiles[0] = 0x66; local.weekdayTiles[1] = 0x67; local.weekdayTiles[2] = 0x68; break;
    case 3: local.weekdayTiles[0] = 0x69; local.weekdayTiles[1] = 0x6A; local.weekdayTiles[2] = 0x6B; break;
    case 4: local.weekdayTiles[0] = 0x6C; local.weekdayTiles[1] = 0x6D; local.weekdayTiles[2] = 0x6E; break;
    case 5: local.weekdayTiles[0] = 0x6F; local.weekdayTiles[1] = 0x70; local.weekdayTiles[2] = 0x71; break;
    case 6: local.weekdayTiles[0] = 0x72; local.weekdayTiles[1] = 0x73; local.weekdayTiles[2] = 0x74; break;
    }
    asm("" : : "l"(weekdayIndex));
    register unsigned weekdayFirstDestOffset asm("r2") = offsetof(ClockDisplayData, weekdayFirstTiles);
    asm("" : "+l"(weekdayFirstDestOffset));
    u8 * weekdayFirstDest = reinterpret_cast<u8 *>(clock) + weekdayFirstDestOffset;
    unsigned weekdayFirstTileOffset = local.weekdayTiles[0] * sizeof(ClockTile);
    ClockTile const * timeFont = gClockFont;
    asm("" : "+r"(timeFont));
    memcpy(weekdayFirstDest, reinterpret_cast<u8 const *>(timeFont) + weekdayFirstTileOffset, sizeof(ClockTile));
    u8 * timeDirty = &clock->tilesDirty;
    register unsigned markTimeDirty asm("r6") = 1;
    *timeDirty = markTimeDirty;
    u8 const * secondWeekdayTile = timeFont[local.weekdayTiles[1]];
    unsigned weekdaySecondDestOffset = offsetof(ClockDisplayData, weekdaySecondTiles);
    memcpy(reinterpret_cast<u8 *>(clock) + weekdaySecondDestOffset, secondWeekdayTile, sizeof(ClockTile));
    *timeDirty = markTimeDirty;
    u8 const * thirdWeekdayTile = timeFont[local.weekdayTiles[2]];
    unsigned weekdayThirdDestOffset = offsetof(ClockDisplayData, weekdayThirdTiles);
    memcpy(reinterpret_cast<u8 *>(clock) + weekdayThirdDestOffset, thirdWeekdayTile, sizeof(ClockTile));
    *timeDirty = markTimeDirty;
    register SavedCalendarDate const * savedTime asm("r1") = local.calendar;
    unsigned packedCalendar;
    asm("ldr %0, [%1]" : "=l"(packedCalendar) : "l"(savedTime) : "memory");
    register unsigned decodedHour asm("r0");
    EXTRACT_CALENDAR_FIELD(decodedHour, packedCalendar, HOUR_BIT, HOUR_WIDTH);
    unsigned hour = decodedHour;
    asm("" : "+l"(hour));
    register unsigned meridiemTile asm("r1") = PM_TILE;
    local.timeDirty = timeDirty;
    asm("" : : "m"(local.timeDirty), "l"(meridiemTile) : "memory");
    if (hour < HOURS_PER_HALF_DAY)
        meridiemTile = AM_TILE;
    u8 const * meridiemTileData = timeFont[meridiemTile];
    unsigned meridiemDestOffset = offsetof(ClockDisplayData, meridiemTiles);
    memcpy(reinterpret_cast<u8 *>(clock) + meridiemDestOffset, meridiemTileData, sizeof(ClockTile));
    *dateUploadDirty = markTimeDirty;
    unsigned displayedHour = HOURS_PER_HALF_DAY;
    if (hour != HOURS_PER_HALF_DAY)
    {
        displayedHour = hour;
        if (displayedHour >= HOURS_PER_HALF_DAY)
            displayedHour -= HOURS_PER_HALF_DAY;
    }
    int hourTens = displayedHour / 10;
    unsigned hourOnes = displayedHour % 10;
    u8 const * hourTensTile;
    u8 * hourTensDest;
    if (hourTens > 0)
    {
        unsigned hourTensOffset = hourTens * sizeof(ClockTile);
        ClockTile const * hourTensFont = gClockFont;
        register u8 const * tensTile asm("r1");
        asm(".syntax unified; adds %0, %1, %2; .syntax divided"
            : "=l"(tensTile) : "l"(hourTensOffset), "l"(hourTensFont));
        unsigned hourTensDestOffset = offsetof(ClockDisplayData, hourTensTiles);
        hourTensDest = reinterpret_cast<u8 *>(clock) + hourTensDestOffset;
        hourTensTile = tensTile;
    }
    else
    {
        memset(local.weekdayTiles, 0, sizeof(ClockTile));
        register unsigned hourTensDestOffset asm("r2") = offsetof(ClockDisplayData, hourTensTiles);
        asm("" : "+l"(hourTensDestOffset));
        hourTensDest = reinterpret_cast<u8 *>(clock) + hourTensDestOffset;
        hourTensTile = reinterpret_cast<u8 *>(local.weekdayTiles);
    }
    memcpy(hourTensDest, hourTensTile, sizeof(ClockTile));
    unsigned markHourTens = 1;
    register u8 * hourTensDirty asm("r1");
    asm("mov %0, %1" : "=l"(hourTensDirty) : "r"(dateUploadDirty));
    *hourTensDirty = markHourTens;
    unsigned hourOnesOffset = hourOnes * sizeof(ClockTile);
    register ClockTile const * timeRowFontAddress asm("r2") = gClockFont;
    asm("" : "+l"(timeRowFontAddress));
    ClockTile const * timeRowFont = timeRowFontAddress;
    asm("" : "+r"(timeRowFont));
    u8 const * hourOnesTile = reinterpret_cast<u8 const *>(timeRowFont) + hourOnesOffset;
    unsigned hourOnesDestOffset = offsetof(ClockDisplayData, hourOnesTiles);
    memcpy(reinterpret_cast<u8 *>(clock) + hourOnesDestOffset, hourOnesTile, sizeof(ClockTile));
    unsigned markTimeRowDirty = 1;
    asm("" : "+r"(markTimeRowDirty));
    register unsigned markHourOnes asm("r2");
    register u8 * hourOnesDirty asm("r1");
    asm("mov %0, %2; mov %1, %3" : "=l"(markHourOnes), "=l"(hourOnesDirty)
        : "r"(markTimeRowDirty), "r"(dateUploadDirty));
    *hourOnesDirty = markHourOnes;

    register SavedCalendarDate const * savedMinute asm("r1") = local.calendar;
    asm("ldr %0, [%1]" : "=l"(packedCalendar) : "l"(savedMinute) : "memory");
    unsigned minute;
    EXTRACT_CALENDAR_FIELD(minute, packedCalendar, MINUTE_BIT, MINUTE_WIDTH);
    // The bulk HUD refresh displays minutes in ten-minute steps.
    unsigned displayedMinute = u8(minute / MINUTES_PER_DISPLAY_STEP) * MINUTES_PER_DISPLAY_STEP;
    unsigned minuteTens = displayedMinute / 10;
    unsigned minuteOnes = displayedMinute % 10;
    u8 const * minuteTensTile = timeRowFont[minuteTens];
    u8 * colonDest = clock->colonTiles;
    asm("" : "+l"(colonDest));
    register unsigned minuteTensDestOffset asm("r1") = offsetof(ClockDisplayData, minuteTensTiles);
    asm("" : "+l"(minuteTensDestOffset));
    u8 * minuteTensDest = reinterpret_cast<u8 *>(clock) + minuteTensDestOffset;
    memcpy(minuteTensDest, minuteTensTile, sizeof(ClockTile));
    unsigned markMinuteTens;
    asm("mov %0, %1" : "=l"(markMinuteTens) : "r"(markTimeRowDirty));
    register u8 * minuteTensDirty asm("r2") = local.timeDirty;
    *minuteTensDirty = markMinuteTens;
    u8 const * minuteOnesTile = timeRowFont[minuteOnes];
    register unsigned minuteOnesDestOffset asm("r1") = offsetof(ClockDisplayData, minuteOnesTiles);
    asm("" : "+l"(minuteOnesDestOffset));
    u8 * minuteOnesDest = reinterpret_cast<u8 *>(clock) + minuteOnesDestOffset;
    memcpy(minuteOnesDest, minuteOnesTile, sizeof(ClockTile));
    unsigned markMinuteOnes;
    asm("mov %0, %1" : "=l"(markMinuteOnes) : "r"(markTimeRowDirty));
    register u8 * minuteOnesDirty asm("r2") = local.timeDirty;
    *minuteOnesDirty = markMinuteOnes;
    unsigned showColon = local.showColon;
    asm("" : "+l"(showColon) : : "r0");
    if (showColon)
    {
        memcpy(colonDest, timeRowFont[COLON_TILE], sizeof(ClockTile));
        unsigned markColon;
        asm("mov %0, %1" : "=l"(markColon) : "r"(markTimeRowDirty));
        register u8 * colonDirty asm("r2") = local.timeDirty;
        *colonDirty = markColon;
    }
    else
    {
        memset(local.weekdayTiles, 0, sizeof(ClockTile));
        u8 * blankColonDest = colonDest;
        u8 const * blankColon = reinterpret_cast<u8 const *>(local.weekdayTiles);
        memcpy(blankColonDest, blankColon, sizeof(ClockTile));
        register unsigned markBlankColon asm("r2");
        asm("mov %0, %1" : "=l"(markBlankColon) : "r"(markTimeRowDirty));
        register u8 * blankColonDirty asm("r1") = local.timeDirty;
        *blankColonDirty = markBlankColon;
    }
}

#undef EXTRACT_CALENDAR_FIELD

EC unsigned GetRosterMarriageCandidate(void const *);

ROM_TEXT(0803D7E4) unsigned SelectNpcSchedule_080F1A80(void const * context)
{
    NpcScheduleContext const & self = *static_cast<NpcScheduleContext const *>(context);
    if (self.eventFlags & 0x0C)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (IsNpcScheduleFestivalDay(season, day))
        return 4;
    if (self.weather == 0)
    {
        if (weekday == SCHEDULE_TUESDAY)
            return 3;
        return 1;
    }
    return 2;
}

enum CliffSchedule
{
    CLIFF_SCHEDULE_ABSENT,
    CLIFF_SCHEDULE_REGULAR,
    CLIFF_SCHEDULE_WORKING,
    CLIFF_SCHEDULE_EMPLOYED_SATURDAY,
    CLIFF_SCHEDULE_FESTIVAL,
    CLIFF_SCHEDULE_STORY_OVERRIDE,
};

ROM_TEXT(0803DDE8) unsigned SelectCliffSchedule(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 6)) < 0)
        return CLIFF_SCHEDULE_ABSENT;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned year = date.year;
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;

    // Retain the register used by both offsets and the original flag extraction.
    register unsigned flagOffset asm("r1") = offsetof(NpcScheduleContext, cliffEventStates);
    asm("" : "+r"(flagOffset));
    unsigned departureState = static_cast<u32>(reinterpret_cast<u8 const *>(&self)[flagOffset]) << 26;
    asm("" : "+r"(departureState) : "r"(flagOffset));
    if ((departureState >> 30) == 2)
        return CLIFF_SCHEDULE_ABSENT;
    flagOffset -= offsetof(NpcScheduleContext, cliffEventStates) - offsetof(NpcScheduleContext, unk_2174) - 1;
    asm("" : "+r"(flagOffset));
    register u8 const * wineryEventByte asm("r0") = reinterpret_cast<u8 const *>(&self) + flagOffset;
    asm("" : "+r"(wineryEventByte));
    if (static_cast<i32>(static_cast<u32>(*wineryEventByte) << (31 - 4)) >= 0)
    {
        if (year > 1)
            return CLIFF_SCHEDULE_ABSENT;
        if (year == 1 && season == SEASON_WINTER)
        {
            register unsigned winterDay asm("r0") = day;
            asm("" : "+r"(winterDay));
            if (winterDay > 28)
                return CLIFF_SCHEDULE_ABSENT;
        }
    }
    unsigned storyState = static_cast<u32>(self.cliffEventStates) << 30;
    asm("" : "+r"(storyState));
    if ((storyState >> 30) == 1)
        return CLIFF_SCHEDULE_STORY_OVERRIDE;
    if (IsNpcScheduleFestivalDay(season, day))
        return CLIFF_SCHEDULE_FESTIVAL;
    if (year != 1 || season != SEASON_AUTUMN || day != 15)
        goto regularSchedule;
    if (static_cast<i32>(static_cast<u32>(reinterpret_cast<u8 const *>(&self.unk_2174)[1]) << (31 - 4)) >= 0)
        goto regularSchedule;
workingSchedule:
    return CLIFF_SCHEDULE_WORKING;
regularSchedule:
    unsigned schedule;
    if ((self.unk_2174 & 0x61000) == 0x41000)
    {
        if (weekday != SCHEDULE_SATURDAY)
            goto workingSchedule;
        schedule = CLIFF_SCHEDULE_EMPLOYED_SATURDAY;
    }
    else
        schedule = CLIFF_SCHEDULE_REGULAR;
    return schedule;
}

ROM_TEXT(0803DECC) unsigned SelectNpcSchedule_080F2DC0(NpcScheduleContext const & self)
{
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    if (IsNpcScheduleFestivalDay(date.season, date.day))
        return 2;
    return 1;
}

ROM_TEXT(0803DECC) unsigned SelectNpcSchedule_080F3010(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 5)) < 0)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (IsNpcScheduleFestivalDay(season, day))
        return 4;
    if (weekday == SCHEDULE_WEDNESDAY)
    {
        if (self.weather == 0)
            return 2;
        return 3;
    }
    return 1;
}

ROM_TEXT(0803DC64) unsigned SelectNpcSchedule_080F280C(NpcScheduleContext const & self)
{
    if (self.eventFlags & 0xC)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (IsNpcScheduleFestivalDay(season, day))
        return 3;
    if (weekday == SCHEDULE_SUNDAY)
        return 2;
    return 1;
}

ROM_TEXT(0803DECC) unsigned SelectNpcSchedule_080F33B8(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 5)) < 0)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    if (IsNpcScheduleFestivalDay(date.season, date.day))
        return 2;
    return 1;
}

ROM_TEXT(0803DECC) unsigned SelectNpcSchedule_080F3408(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 2)) < 0)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (IsNpcScheduleFestivalDay(season, day))
        return 4;
    if (weekday == SCHEDULE_TUESDAY)
        return 2;
    if (weekday == SCHEDULE_SUNDAY)
        return 3;
    return 1;
}

ROM_TEXT(0803E1A4) unsigned SelectNpcSchedule_080F3FD8(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 2)) < 0)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (IsNpcScheduleFestivalDay(season, day))
        return 4;
    if (self.weather == 0)
    {
        if (weekday == SCHEDULE_TUESDAY)
            return 3;
        return 1;
    }
    return 2;
}

enum KarenSchedule
{
    KAREN_SCHEDULE_ABSENT,
    KAREN_SCHEDULE_CLEAR_WEATHER,
    KAREN_SCHEDULE_OTHER_WEATHER,
    KAREN_SCHEDULE_SUNDAY,
    KAREN_SCHEDULE_TUESDAY_CLEAR,
    KAREN_SCHEDULE_TUESDAY_OTHER_WEATHER,
    KAREN_SCHEDULE_AFTER_RIVAL_EVENTS,
    KAREN_SCHEDULE_AFTER_RIVAL_EVENTS_SUNDAY,
    KAREN_SCHEDULE_FESTIVAL,
    KAREN_SCHEDULE_WINTER_14,
    KAREN_SCHEDULE_AFTER_RIVAL_EVENTS_FESTIVAL,
    KAREN_SCHEDULE_MARRIED_ACTIVE_NPC,
    KAREN_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT,
    KAREN_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT_SUNDAY_TUESDAY,
    KAREN_SCHEDULE_MARRIED,
    KAREN_SCHEDULE_MARRIED_VARIANT,
    KAREN_SCHEDULE_MARRIED_VARIANT_SUNDAY_TUESDAY,
    KAREN_SCHEDULE_MARRIED_ACTIVE_NPC_FESTIVAL_OR_STORM,
    KAREN_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT_FESTIVAL_OR_STORM,
    KAREN_SCHEDULE_MARRIED_FESTIVAL_OR_STORM,
    KAREN_SCHEDULE_MARRIED_VARIANT_FESTIVAL_OR_STORM,
    KAREN_SCHEDULE_MARRIED_AUTUMN_9,
    KAREN_SCHEDULE_MARRIED_AUTUMN_13,
    KAREN_SCHEDULE_MARRIED_EVENT_OVERRIDE,
};

enum { KAREN_NPC_ID = 19 };

struct G1NpcRoster;

EC Bachelorette * GetRosterBacheloretteById(G1NpcRoster *, u32);

ROM_TEXT(0803DFE8) unsigned SelectKarenSchedule(NpcScheduleContext const & self)
{
    if (self.eventFlags & 0xC)
        return KAREN_SCHEDULE_ABSENT;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    u8 * roster = const_cast<u8 *>(self.unk_1CD4);
    unsigned spouseNpcId = GetRosterMarriageCandidate(roster);
    if (spouseNpcId == KAREN_NPC_ID)
    {
        if (season == SEASON_AUTUMN)
        {
            if (day == 9)
                return KAREN_SCHEDULE_MARRIED_AUTUMN_9;
            if (day == 13)
                return KAREN_SCHEDULE_MARRIED_AUTUMN_13;
        }
        if ((static_cast<u32>(self.marriageScheduleEvents.bytes[1]) << 27 >> 30) == 1)
            return KAREN_SCHEDULE_MARRIED_EVENT_OVERRIDE;
        unsigned marriedVariant = self.marriedScheduleVariant;
        uintptr_t activeNpc = reinterpret_cast<uintptr_t>(GetActiveRosterNpc(const_cast<u8 *>(self.unk_1CD4)));
        // Keep the original branchless non-null test.
        unsigned hasActiveNpc = ((0u - activeNpc) | activeNpc) >> 31;
        if (IsNpcScheduleFestivalDay(season, day) || self.weather == 3 || self.weather == 4)
        {
            if (hasActiveNpc)
            {
                if (marriedVariant)
                    return KAREN_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT_FESTIVAL_OR_STORM;
                return KAREN_SCHEDULE_MARRIED_ACTIVE_NPC_FESTIVAL_OR_STORM;
            }
            if (marriedVariant)
                return KAREN_SCHEDULE_MARRIED_VARIANT_FESTIVAL_OR_STORM;
            return KAREN_SCHEDULE_MARRIED_FESTIVAL_OR_STORM;
        }
        if (marriedVariant && (weekday == SCHEDULE_SUNDAY || weekday == SCHEDULE_TUESDAY))
        {
            if (hasActiveNpc)
                return KAREN_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT_SUNDAY_TUESDAY;
            return KAREN_SCHEDULE_MARRIED_VARIANT_SUNDAY_TUESDAY;
        }
        if (hasActiveNpc)
        {
            if (marriedVariant)
                return KAREN_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT;
            return KAREN_SCHEDULE_MARRIED_ACTIVE_NPC;
        }
        if (marriedVariant)
            return KAREN_SCHEDULE_MARRIED_VARIANT;
        return KAREN_SCHEDULE_MARRIED;
    }
    if (self.karen.GetRivalEventCount() > 4)
    {
        if (IsNpcScheduleFestivalDay(season, day))
            return KAREN_SCHEDULE_AFTER_RIVAL_EVENTS_FESTIVAL;
        if (weekday == SCHEDULE_SUNDAY)
            return KAREN_SCHEDULE_AFTER_RIVAL_EVENTS_SUNDAY;
        return KAREN_SCHEDULE_AFTER_RIVAL_EVENTS;
    }
    Bachelorette const * karen = GetRosterBacheloretteById(reinterpret_cast<G1NpcRoster *>(roster), KAREN_NPC_ID);
    if (season == SEASON_WINTER && day == 14 && spouseNpcId == 0 && karen && karen->GetLove() > 9999
        && (static_cast<u32>(self.seasonalScheduleEvents.bytes[1]) << 25 >> 30) != 2)
        return KAREN_SCHEDULE_WINTER_14;
    if (IsNpcScheduleFestivalDay(season, day))
        return KAREN_SCHEDULE_FESTIVAL;
    if (weekday == SCHEDULE_SUNDAY)
        return KAREN_SCHEDULE_SUNDAY;
    if (weekday == SCHEDULE_TUESDAY)
    {
        if (self.weather == 0)
            return KAREN_SCHEDULE_TUESDAY_CLEAR;
        return KAREN_SCHEDULE_TUESDAY_OTHER_WEATHER;
    }
    if (self.weather == 0)
        return KAREN_SCHEDULE_CLEAR_WEATHER;
    return KAREN_SCHEDULE_OTHER_WEATHER;
}

ROM_TEXT(0803E1A4) unsigned SelectNpcSchedule_080F42F0(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 6)) < 0)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    if (IsNpcScheduleFestivalDay(season, day))
        return 2;
    unsigned event = GetRosterMarriageCandidate(self.unk_1CD4);
    if (season == SEASON_AUTUMN && day == 5 && event != 25)
        return 3;
    return 1;
}

enum AnnSchedule
{
    ANN_SCHEDULE_ABSENT,
    ANN_SCHEDULE_CLEAR_WEATHER,
    ANN_SCHEDULE_OTHER_WEATHER,
    ANN_SCHEDULE_AFTER_RIVAL_EVENTS_CLEAR,
    ANN_SCHEDULE_AFTER_RIVAL_EVENTS_OTHER_WEATHER,
    ANN_SCHEDULE_AUTUMN_5,
    ANN_SCHEDULE_FESTIVAL,
    ANN_SCHEDULE_WINTER_14,
    ANN_SCHEDULE_MARRIED_ACTIVE_NPC,
    ANN_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT,
    ANN_SCHEDULE_MARRIED,
    ANN_SCHEDULE_MARRIED_VARIANT,
    ANN_SCHEDULE_MARRIED_ACTIVE_NPC_FESTIVAL_OR_STORM,
    ANN_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT_FESTIVAL_OR_STORM,
    ANN_SCHEDULE_MARRIED_FESTIVAL_OR_STORM,
    ANN_SCHEDULE_MARRIED_VARIANT_FESTIVAL_OR_STORM,
    ANN_SCHEDULE_MARRIED_AUTUMN_9,
    ANN_SCHEDULE_MARRIED_AUTUMN_13,
    ANN_SCHEDULE_MARRIED_EVENT_OVERRIDE,
};

enum { ANN_NPC_ID = 25 };

ROM_TEXT(0803E264) unsigned SelectAnnSchedule(NpcScheduleContext const & self)
{
    // Preserve the original register allocation around the schedule helper calls.
    register NpcScheduleContext const * context asm("r5") = &self;
    asm("" : "+r"(context));
    if (static_cast<i32>(static_cast<u32>(context->eventFlags) << 25) < 0)
        return ANN_SCHEDULE_ABSENT;
    CalendarDate date;
    DecodeNpcScheduleDate(date, context->calendar);
    register unsigned season asm("r8") = date.season;
    register unsigned day asm("r6") = date.day;
    asm("" : "+r"(season), "+r"(day));
    unsigned spouseNpcId = GetRosterMarriageCandidate(context->unk_1CD4);
    if (season == SEASON_AUTUMN && day == 5)
    {
        if (spouseNpcId != ANN_NPC_ID)
            return ANN_SCHEDULE_AUTUMN_5;
        goto married;
    }
    if (spouseNpcId == ANN_NPC_ID)
    {
married:
        if (season == SEASON_AUTUMN)
        {
            if (day == 9)
                return ANN_SCHEDULE_MARRIED_AUTUMN_9;
            if (day == 13)
                return ANN_SCHEDULE_MARRIED_AUTUMN_13;
        }
        if (context->marriageScheduleEvents.states.ann == 1)
            return ANN_SCHEDULE_MARRIED_EVENT_OVERRIDE;
        register unsigned marriedVariant asm("r4") = context->marriedScheduleVariant;
        asm("" : "+r"(marriedVariant));
        uintptr_t activeNpc = reinterpret_cast<uintptr_t>(GetActiveRosterNpc(const_cast<u8 *>(context->unk_1CD4)));
        unsigned hasActiveNpc = ((0u - activeNpc) | activeNpc) >> 31;
        register unsigned hasActiveNpcOnFestival asm("r9") = hasActiveNpc;
        asm("" : "+r"(hasActiveNpcOnFestival) : "r"(hasActiveNpc));
        if (IsNpcScheduleFestivalDay(season, day))
            goto festivalOrStorm;
        {
            register unsigned weather asm("r0") = context->weather;
            asm("" : "+r"(weather));
            if (weather == 3)
                goto festivalOrStorm;
            asm("" : "+r"(weather));
            if (weather != 4)
                goto normalMarriedDay;
        }
festivalOrStorm:
        {
            if (hasActiveNpcOnFestival)
            {
                if (marriedVariant)
                    return ANN_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT_FESTIVAL_OR_STORM;
                return ANN_SCHEDULE_MARRIED_ACTIVE_NPC_FESTIVAL_OR_STORM;
            }
            if (marriedVariant)
                return ANN_SCHEDULE_MARRIED_VARIANT_FESTIVAL_OR_STORM;
            return ANN_SCHEDULE_MARRIED_FESTIVAL_OR_STORM;
        }
normalMarriedDay:
        if (hasActiveNpc)
        {
            if (marriedVariant)
                return ANN_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT;
            return ANN_SCHEDULE_MARRIED_ACTIVE_NPC;
        }
        if (marriedVariant)
            return ANN_SCHEDULE_MARRIED_VARIANT;
        return ANN_SCHEDULE_MARRIED;
    }
    if (context->ann.GetRivalEventCount() > 4)
    {
        if (IsNpcScheduleFestivalDay(season, day))
            goto festival;
        if (context->weather == 0)
            return ANN_SCHEDULE_AFTER_RIVAL_EVENTS_CLEAR;
        return ANN_SCHEDULE_AFTER_RIVAL_EVENTS_OTHER_WEATHER;
    }
    {
        Bachelorette const * ann = GetRosterBacheloretteById(
            reinterpret_cast<G1NpcRoster *>(const_cast<u8 *>(context->unk_1CD4)), ANN_NPC_ID);
        if (season == SEASON_WINTER && day == 14 && spouseNpcId == 0 && ann && ann->GetLove() > 9999
            && context->seasonalScheduleEvents.states.ann != 2)
            return ANN_SCHEDULE_WINTER_14;
    }
    if (IsNpcScheduleFestivalDay(season, day))
    {
festival:
        return ANN_SCHEDULE_FESTIVAL;
    }
    if (context->weather == 0)
        return ANN_SCHEDULE_CLEAR_WEATHER;
    return ANN_SCHEDULE_OTHER_WEATHER;
}

enum PopuriSchedule
{
    POPURI_SCHEDULE_ABSENT,
    POPURI_SCHEDULE_CLEAR_WEATHER,
    POPURI_SCHEDULE_OTHER_WEATHER,
    POPURI_SCHEDULE_SUNDAY_CLEAR,
    POPURI_SCHEDULE_SUNDAY_OTHER_WEATHER,
    POPURI_SCHEDULE_SUMMER_SUNDAY_CLEAR,
    POPURI_SCHEDULE_MARRIED_ACTIVE_NPC,
    POPURI_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT,
    POPURI_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT_SUNDAY,
    POPURI_SCHEDULE_MARRIED,
    POPURI_SCHEDULE_MARRIED_VARIANT,
    POPURI_SCHEDULE_MARRIED_VARIANT_SUNDAY,
    POPURI_SCHEDULE_MARRIED_ACTIVE_NPC_FESTIVAL_OR_STORM,
    POPURI_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT_FESTIVAL_OR_STORM,
    POPURI_SCHEDULE_MARRIED_FESTIVAL_OR_STORM,
    POPURI_SCHEDULE_MARRIED_VARIANT_FESTIVAL_OR_STORM,
    POPURI_SCHEDULE_FESTIVAL,
    POPURI_SCHEDULE_AUTUMN_30,
    POPURI_SCHEDULE_WINTER_14,
    POPURI_SCHEDULE_AFTER_RIVAL_EVENTS_SUMMER_FESTIVAL,
    POPURI_SCHEDULE_AFTER_RIVAL_EVENTS_SUMMER_CLEAR,
    POPURI_SCHEDULE_AFTER_RIVAL_EVENTS_SUMMER_OTHER_WEATHER,
    POPURI_SCHEDULE_MARRIED_AUTUMN_13,
    POPURI_SCHEDULE_MARRIED_EVENT_OVERRIDE,
};

enum { POPURI_NPC_ID = 3 };

ROM_TEXT(0803DA7C) unsigned SelectPopuriSchedule(NpcScheduleContext const & self)
{
    CalendarDate date;
    unsigned season, day, weekday, spouseNpcId;
    Bachelorette const * popuri;
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << 28) < 0)
        goto absent;
    DecodeNpcScheduleDate(date, self.calendar);
    season = date.season;
    day = date.day;
    weekday = date.weekday;
    spouseNpcId = GetRosterMarriageCandidate(self.unk_1CD4);
    if (spouseNpcId == POPURI_NPC_ID)
    {
        if (season == SEASON_AUTUMN && day == 13)
            return POPURI_SCHEDULE_MARRIED_AUTUMN_13;
        if ((static_cast<u32>(self.marriageScheduleEvents.bytes[0]) << 25 >> 30) == 1)
            return POPURI_SCHEDULE_MARRIED_EVENT_OVERRIDE;
        unsigned marriedVariant = self.marriedScheduleVariant;
        uintptr_t activeNpc = reinterpret_cast<uintptr_t>(GetActiveRosterNpc(const_cast<u8 *>(self.unk_1CD4)));
        unsigned hasActiveNpc = ((0u - activeNpc) | activeNpc) >> 31;
        if (IsNpcScheduleFestivalDay(season, day) || self.weather == 3 || self.weather == 4)
        {
            if (hasActiveNpc)
            {
                if (marriedVariant)
                    return POPURI_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT_FESTIVAL_OR_STORM;
                return POPURI_SCHEDULE_MARRIED_ACTIVE_NPC_FESTIVAL_OR_STORM;
            }
            if (marriedVariant)
                return POPURI_SCHEDULE_MARRIED_VARIANT_FESTIVAL_OR_STORM;
            return POPURI_SCHEDULE_MARRIED_FESTIVAL_OR_STORM;
        }
        if (marriedVariant && weekday == SCHEDULE_SUNDAY)
        {
            if (hasActiveNpc)
                return POPURI_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT_SUNDAY;
            return POPURI_SCHEDULE_MARRIED_VARIANT_SUNDAY;
        }
        if (hasActiveNpc)
        {
            if (marriedVariant)
                return POPURI_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT;
            return POPURI_SCHEDULE_MARRIED_ACTIVE_NPC;
        }
        if (marriedVariant)
            return POPURI_SCHEDULE_MARRIED_VARIANT;
        return POPURI_SCHEDULE_MARRIED;
    }
    if (self.popuri.GetRivalEventCount() > 4)
    {
        if (season != SEASON_SUMMER)
            goto absent;
        if (IsNpcScheduleFestivalDay(SEASON_SUMMER, day))
            return POPURI_SCHEDULE_AFTER_RIVAL_EVENTS_SUMMER_FESTIVAL;
        if (self.weather == 0)
            return POPURI_SCHEDULE_AFTER_RIVAL_EVENTS_SUMMER_CLEAR;
        return POPURI_SCHEDULE_AFTER_RIVAL_EVENTS_SUMMER_OTHER_WEATHER;
    }
    if (season == SEASON_AUTUMN && day == 30 && (static_cast<u32>(self.seasonalScheduleEvents.bytes[0]) << 30 >> 30) != 2)
        return POPURI_SCHEDULE_AUTUMN_30;
    popuri = GetRosterBacheloretteById(
        reinterpret_cast<G1NpcRoster *>(const_cast<u8 *>(self.unk_1CD4)), POPURI_NPC_ID);
    if (season == SEASON_WINTER && day == 14 && spouseNpcId == 0 && popuri && popuri->GetLove() > 9999
        && (static_cast<u32>(self.seasonalScheduleEvents.bytes[0]) << 26 >> 30) != 2)
        return POPURI_SCHEDULE_WINTER_14;
    if (IsNpcScheduleFestivalDay(season, day))
        return POPURI_SCHEDULE_FESTIVAL;
    if (static_cast<i32>(static_cast<u32>(self.unk_215A) << 28) < 0)
    {
absent:
        return POPURI_SCHEDULE_ABSENT;
    }
    if (weekday == SCHEDULE_SUNDAY)
    {
        if (self.weather == 0)
        {
            if (season == SEASON_SUMMER)
                return POPURI_SCHEDULE_SUMMER_SUNDAY_CLEAR;
            return POPURI_SCHEDULE_SUNDAY_CLEAR;
        }
        return POPURI_SCHEDULE_SUNDAY_OTHER_WEATHER;
    }
    if (self.weather == 0)
        return POPURI_SCHEDULE_CLEAR_WEATHER;
    return POPURI_SCHEDULE_OTHER_WEATHER;
}

enum MarySchedule
{
    MARY_SCHEDULE_ABSENT,
    MARY_SCHEDULE_REGULAR,
    MARY_SCHEDULE_MONDAY_CLEAR,
    MARY_SCHEDULE_MONDAY_OTHER_WEATHER,
    MARY_SCHEDULE_AFTER_RIVAL_EVENTS,
    MARY_SCHEDULE_AFTER_RIVAL_EVENTS_THURSDAY_CLEAR,
    MARY_SCHEDULE_AFTER_RIVAL_EVENTS_THURSDAY_OTHER_WEATHER,
    MARY_SCHEDULE_WINTER_14,
    MARY_SCHEDULE_FESTIVAL,
    MARY_SCHEDULE_AFTER_RIVAL_EVENTS_FESTIVAL,
    MARY_SCHEDULE_MARRIED_ACTIVE_NPC,
    MARY_SCHEDULE_MARRIED_ACTIVE_NPC_MONDAY,
    MARY_SCHEDULE_MARRIED,
    MARY_SCHEDULE_MARRIED_MONDAY,
    MARY_SCHEDULE_MARRIED_ACTIVE_NPC_FESTIVAL_OR_STORM,
    MARY_SCHEDULE_MARRIED_FESTIVAL_OR_STORM,
    MARY_SCHEDULE_MARRIED_AUTUMN_9,
    MARY_SCHEDULE_MARRIED_AUTUMN_13,
    MARY_SCHEDULE_MARRIED_EVENT_OVERRIDE,
};

enum { MARY_NPC_ID = 12 };

ROM_TEXT(0803E4A0) unsigned SelectMarySchedule(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << 27) < 0)
        return MARY_SCHEDULE_ABSENT;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    u8 * roster = const_cast<u8 *>(self.unk_1CD4);
    unsigned spouseNpcId = GetRosterMarriageCandidate(roster);
    if (spouseNpcId == MARY_NPC_ID)
    {
        if (season == SEASON_AUTUMN)
        {
            if (day == 9)
                return MARY_SCHEDULE_MARRIED_AUTUMN_9;
            if (day == 13)
                return MARY_SCHEDULE_MARRIED_AUTUMN_13;
        }
        if ((static_cast<u32>(self.marriageScheduleEvents.bytes[1]) << 25 >> 30) == 1)
            return MARY_SCHEDULE_MARRIED_EVENT_OVERRIDE;
        uintptr_t activeNpc = reinterpret_cast<uintptr_t>(GetActiveRosterNpc(const_cast<u8 *>(self.unk_1CD4)));
        unsigned hasActiveNpc = ((0u - activeNpc) | activeNpc) >> 31;
        register unsigned hasActiveNpcOnFestival asm("r8") = hasActiveNpc;
        asm("" : "+r"(hasActiveNpcOnFestival) : "r"(hasActiveNpc));
        if (IsNpcScheduleFestivalDay(season, day) || self.weather == 3 || self.weather == 4)
        {
            if (hasActiveNpcOnFestival)
                return MARY_SCHEDULE_MARRIED_ACTIVE_NPC_FESTIVAL_OR_STORM;
            return MARY_SCHEDULE_MARRIED_FESTIVAL_OR_STORM;
        }
        if (weekday == SCHEDULE_MONDAY)
        {
            if (hasActiveNpc)
                return MARY_SCHEDULE_MARRIED_ACTIVE_NPC_MONDAY;
            return MARY_SCHEDULE_MARRIED_MONDAY;
        }
        if (hasActiveNpc)
            return MARY_SCHEDULE_MARRIED_ACTIVE_NPC;
        return MARY_SCHEDULE_MARRIED;
    }
    if (self.mary.GetRivalEventCount() > 4)
    {
        if (IsNpcScheduleFestivalDay(season, day))
            return MARY_SCHEDULE_AFTER_RIVAL_EVENTS_FESTIVAL;
        if (weekday == SCHEDULE_THURSDAY)
        {
            if (self.weather == 0)
                return MARY_SCHEDULE_AFTER_RIVAL_EVENTS_THURSDAY_CLEAR;
            return MARY_SCHEDULE_AFTER_RIVAL_EVENTS_THURSDAY_OTHER_WEATHER;
        }
        return MARY_SCHEDULE_AFTER_RIVAL_EVENTS;
    }
    Bachelorette const * mary = GetRosterBacheloretteById(reinterpret_cast<G1NpcRoster *>(roster), MARY_NPC_ID);
    if (season == SEASON_WINTER && day == 14 && spouseNpcId == 0 && mary && mary->GetLove() > 9999
        && (static_cast<u32>(self.maryWinterScheduleEvent) << 30 >> 30) != 2)
        return MARY_SCHEDULE_WINTER_14;
    if (IsNpcScheduleFestivalDay(season, day))
        return MARY_SCHEDULE_FESTIVAL;
    if (weekday == SCHEDULE_MONDAY)
    {
        if (self.weather == 0)
            return MARY_SCHEDULE_MONDAY_CLEAR;
        return MARY_SCHEDULE_MONDAY_OTHER_WEATHER;
    }
    return MARY_SCHEDULE_REGULAR;
}

enum ElliSchedule
{
    ELLI_SCHEDULE_ABSENT,
    ELLI_SCHEDULE_REGULAR,
    ELLI_SCHEDULE_WEDNESDAY,
    ELLI_SCHEDULE_AFTER_RIVAL_EVENTS,
    ELLI_SCHEDULE_AFTER_RIVAL_EVENTS_WEDNESDAY,
    ELLI_SCHEDULE_FESTIVAL,
    ELLI_SCHEDULE_WINTER_14,
    ELLI_SCHEDULE_AFTER_RIVAL_EVENTS_FESTIVAL,
    ELLI_SCHEDULE_MARRIED_ACTIVE_NPC,
    ELLI_SCHEDULE_MARRIED_ACTIVE_NPC_WEDNESDAY,
    ELLI_SCHEDULE_MARRIED,
    ELLI_SCHEDULE_MARRIED_WEDNESDAY,
    ELLI_SCHEDULE_MARRIED_ACTIVE_NPC_FESTIVAL_OR_STORM,
    ELLI_SCHEDULE_MARRIED_FESTIVAL_OR_STORM,
    ELLI_SCHEDULE_MARRIED_AUTUMN_9,
    ELLI_SCHEDULE_MARRIED_AUTUMN_13,
    ELLI_SCHEDULE_MARRIED_EVENT_OVERRIDE,
};

enum { ELLI_NPC_ID = 21 };

ROM_TEXT(0803E780) unsigned SelectElliSchedule(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << 26) < 0)
        return ELLI_SCHEDULE_ABSENT;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    u8 * roster = const_cast<u8 *>(self.unk_1CD4);
    unsigned spouseNpcId = GetRosterMarriageCandidate(roster);
    if (spouseNpcId == ELLI_NPC_ID)
    {
        if (season == SEASON_AUTUMN)
        {
            if (day == 9)
                return ELLI_SCHEDULE_MARRIED_AUTUMN_9;
            if (day == 13)
                return ELLI_SCHEDULE_MARRIED_AUTUMN_13;
        }
        if ((static_cast<u32>(self.marriageScheduleEvents.bytes[1]) << 29 >> 30) == 1)
            return ELLI_SCHEDULE_MARRIED_EVENT_OVERRIDE;
        uintptr_t activeNpc = reinterpret_cast<uintptr_t>(GetActiveRosterNpc(const_cast<u8 *>(self.unk_1CD4)));
        unsigned hasActiveNpc = ((0u - activeNpc) | activeNpc) >> 31;
        register unsigned hasActiveNpcOnFestival asm("r8") = hasActiveNpc;
        asm("" : "+r"(hasActiveNpcOnFestival) : "r"(hasActiveNpc));
        if (IsNpcScheduleFestivalDay(season, day) || self.weather == 3 || self.weather == 4)
        {
            if (hasActiveNpcOnFestival)
                return ELLI_SCHEDULE_MARRIED_ACTIVE_NPC_FESTIVAL_OR_STORM;
            return ELLI_SCHEDULE_MARRIED_FESTIVAL_OR_STORM;
        }
        if (weekday == SCHEDULE_WEDNESDAY)
        {
            if (hasActiveNpc)
                return ELLI_SCHEDULE_MARRIED_ACTIVE_NPC_WEDNESDAY;
            return ELLI_SCHEDULE_MARRIED_WEDNESDAY;
        }
        if (hasActiveNpc)
            return ELLI_SCHEDULE_MARRIED_ACTIVE_NPC;
        return ELLI_SCHEDULE_MARRIED;
    }
    if (self.elli.GetRivalEventCount() > 4)
    {
        if (IsNpcScheduleFestivalDay(season, day))
            return ELLI_SCHEDULE_AFTER_RIVAL_EVENTS_FESTIVAL;
        if (weekday == SCHEDULE_WEDNESDAY)
            return ELLI_SCHEDULE_AFTER_RIVAL_EVENTS_WEDNESDAY;
        return ELLI_SCHEDULE_AFTER_RIVAL_EVENTS;
    }
    Bachelorette const * elli = GetRosterBacheloretteById(reinterpret_cast<G1NpcRoster *>(roster), ELLI_NPC_ID);
    if (season == SEASON_WINTER && day == 14 && spouseNpcId == 0 && elli && elli->GetLove() > 9999
        && (static_cast<u32>(self.seasonalScheduleEvents.bytes[1]) << 28 >> 30) != 2)
        return ELLI_SCHEDULE_WINTER_14;
    if (IsNpcScheduleFestivalDay(season, day))
        return ELLI_SCHEDULE_FESTIVAL;
    if (weekday == SCHEDULE_WEDNESDAY)
        return ELLI_SCHEDULE_WEDNESDAY;
    return ELLI_SCHEDULE_REGULAR;
}

enum ChildSchedule
{
    CHILD_SCHEDULE_ABSENT,
    CHILD_SCHEDULE_REGULAR,
    CHILD_SCHEDULE_LATE_MORNING,
    CHILD_SCHEDULE_HOME_ALL_DAY,
    CHILD_SCHEDULE_WALKING_AGE,
    CHILD_SCHEDULE_AUTUMN_13,
};

enum { HARVEST_GODDESS_NPC_ID = 31 };

EC u32 GetNamedNpcAgeDays(u8 *);

ROM_TEXT(0803DCB4) unsigned SelectChildSchedule(NpcScheduleContext const & self)
{
    unsigned spouseNpcId, season, day, weekday, marriedVariant;
    CalendarDate date;
    u8 * roster = const_cast<u8 *>(self.unk_1CD4);
    u8 * child = GetActiveRosterNpc(roster);
    if (!child)
        goto absent;
    if (GetNamedNpcAgeDays(child) > 59)
        goto walkingAge;
    spouseNpcId = GetRosterMarriageCandidate(roster);
    if (spouseNpcId == HARVEST_GODDESS_NPC_ID)
        return CHILD_SCHEDULE_HOME_ALL_DAY;
    DecodeNpcScheduleDate(date, self.calendar);
    season = date.season;
    day = date.day;
    weekday = date.weekday;
    if (season == SEASON_AUTUMN && day == 13)
        return CHILD_SCHEDULE_AUTUMN_13;
    if (IsNpcScheduleFestivalDay(season, day) || self.weather == 3 || self.weather == 4)
        goto lateMorning;
    marriedVariant = self.marriedScheduleVariant;
    switch (spouseNpcId)
    {
    case MARY_NPC_ID:
        if (weekday != SCHEDULE_MONDAY)
            break;
    lateMorning:
        return CHILD_SCHEDULE_LATE_MORNING;
    case POPURI_NPC_ID:
        if (!marriedVariant)
            goto lateMorning;
        if (weekday != SCHEDULE_SUNDAY)
            break;
        // Keep the two Sunday checks separate for the original branch layout.
        asm("" : : "r"(weekday));
        goto lateMorning;
    case KAREN_NPC_ID:
        if (!marriedVariant || weekday == SCHEDULE_TUESDAY || weekday == SCHEDULE_SUNDAY)
            goto lateMorning;
        break;
    case ELLI_NPC_ID:
        if (weekday != SCHEDULE_WEDNESDAY)
            break;
        goto lateMorning;
    case ANN_NPC_ID:
        if (!marriedVariant)
            goto lateMorning;
        break;
    case HARVEST_GODDESS_NPC_ID:
        break;
    }
    return CHILD_SCHEDULE_REGULAR;
walkingAge:
    return CHILD_SCHEDULE_WALKING_AGE;
absent:
    return CHILD_SCHEDULE_ABSENT;
}

ROM_TEXT(0803E40C) unsigned SelectNpcSchedule_080F4974(NpcScheduleContext const & self)
{
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned weekday = date.weekday;
    switch (self.unk_2187_3)
    {
    case 2:
        if (weekday == SCHEDULE_SUNDAY)
            return 1;
        break;
    }
    return 0;
}

ROM_TEXT(0803E40C) unsigned SelectNpcSchedule_080F49C0(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 4)) < 0)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (IsNpcScheduleFestivalDay(season, day))
        return 4;
    if (weekday == SCHEDULE_MONDAY)
    {
        if (self.weather == 0)
            return 2;
        return 3;
    }
    return 1;
}

ROM_TEXT(0803E620) unsigned SelectNpcSchedule_080F5540(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 4)) < 0)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (IsNpcScheduleFestivalDay(season, day))
        return 5;
    if (weekday == SCHEDULE_MONDAY)
    {
        if (self.weather == 0)
            return 3;
        return 4;
    }
    if (self.weather == 0)
        return 1;
    return 2;
}

ROM_TEXT(0803E620) unsigned SelectNpcSchedule_080F597C(NpcScheduleContext const & self)
{
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    u8 season = date.season;
    u8 day = date.day;
    if (IsNpcScheduleFestivalDay(season, day))
        return 2;
    unsigned maxYear = 200;
    unsigned calendarYear = self.calendar.yearCycle * 7 + self.calendar.yearInCycle;
    if (std::min(maxYear, calendarYear) <= 1 && season == SEASON_SPRING && day <= 2)
        return 0;
    return 1;
}

ROM_TEXT(0803E620) unsigned SelectNpcSchedule_080F59CC(NpcScheduleContext const & self)
{
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (season == SEASON_WINTER && day == 2 && (self.unk_2178 & 0x43000) == 0x2000)
    {
        switch (self.scheduleFlags.unk_217A_3)
        {
        default: return 7;
        case 2: break;
        }
        return 0;
    }
    if (IsNpcScheduleFestivalDay(season, day))
        return 6;
    if (weekday == SCHEDULE_SATURDAY)
    {
        if (self.weather == 0)
            return 2;
        return 3;
    }
    if (weekday == SCHEDULE_SUNDAY)
    {
        if (self.weather == 0)
            return 4;
        return 5;
    }
    return 1;
}

ROM_TEXT(0803E8E8) unsigned SelectNpcSchedule_080F61FC(NpcScheduleContext const & self)
{
    unsigned weekday;
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 5)) >= 0)
    {
        CalendarDate date;
        DecodeNpcScheduleDate(date, self.calendar);
        unsigned season = date.season;
        unsigned day = date.day;
        weekday = date.weekday;
        if (season == SEASON_AUTUMN && day == 30 && (self.unk_21A2 >> 6) != 2)
            return 5;
        if (IsNpcScheduleFestivalDay(season, day))
            return 4;
        if (static_cast<i32>(static_cast<u32>(self.unk_215A) << (31 - 3)) >= 0)
            goto active;
    }
    return 0;
active:
    if (weekday == SCHEDULE_MONDAY || weekday == SCHEDULE_WEDNESDAY)
        return 3;
    if (self.weather == 0)
        return 1;
    return 2;
}

ROM_TEXT(0803E8E8) unsigned SelectNpcSchedule_080F6370(NpcScheduleContext const & self)
{
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    u32 weekday = date.weekday;
    // Keep the copy for the original register allocation.
    u32 weekdayForRegularSchedule = weekday;
    if (IsNpcScheduleFestivalDay(season, day))
        return 5;
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 6)) < 0)
        return 0;
    if ((self.unk_2174 & 0x61000) == 0x41000)
    {
        if (weekday == SCHEDULE_SATURDAY)
            return 4;
        return 3;
    }
    if (weekdayForRegularSchedule == SCHEDULE_SATURDAY)
        return 2;
    return 1;
}

ROM_TEXT(0803E8E8) unsigned SelectNpcSchedule_080F66C4(NpcScheduleContext const & self)
{
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    u8 weekday = date.weekday;
    if (IsNpcScheduleFestivalDay(season, day))
        return 5;
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 6)) < 0)
        return 0;
    if (self.weather == 0)
    {
        if (weekday == SCHEDULE_SATURDAY)
        {
            if ((self.unk_2174 & 0x61000) == 0x41000)
                return 4;
            return 3;
        }
        return 1;
    }
    return 2;
}

ROM_TEXT(0803E8E8) unsigned SelectNpcSchedule_080F6B10(NpcScheduleContext const & self)
{
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned weekday = date.weekday;
    switch (self.unk_2186_5)
    {
    case 2:
        if (weekday == SCHEDULE_WEDNESDAY)
            return 1;
        break;
    }
    return 0;
}

ROM_TEXT(0803E8E8) unsigned SelectNpcSchedule_080F6B4C(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 5)) < 0)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    if (IsNpcScheduleFestivalDay(date.season, date.day))
        return 2;
    return 1;
}

ROM_TEXT(0803E8E8) unsigned SelectNpcSchedule_080F6DE8(NpcScheduleContext const & self)
{
    if (self.eventFlags & 0x7C)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (season == SEASON_AUTUMN && day == 3)
        return 9;
    if (IsNpcScheduleFestivalDay(season, day))
        return 8;
    if (weekday == SCHEDULE_MONDAY || weekday == SCHEDULE_WEDNESDAY || self.weather != 0)
        return 1;
    return 2;
}

ROM_TEXT(0803E8E8) unsigned SelectNpcSchedule_080F6FF8(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 3)) >= 0)
    {
        CalendarDate date;
        DecodeNpcScheduleDate(date, self.calendar);
        unsigned season = date.season;
        unsigned day = date.day;
        unsigned weekday = date.weekday;
        if (season == SEASON_SUMMER)
        {
            if (IsNpcScheduleFestivalDay(1, day))
                return 4;
            if (weekday == SCHEDULE_SUNDAY)
            {
                if (self.weather == 0)
                    return 2;
                return 3;
            }
            return 1;
        }
    }
    return 0;
}

ROM_TEXT(0803E8E8) unsigned SelectNpcSchedule_080F7294(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 4)) < 0)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    unsigned rivalEvents = self.mary.GetRivalEventCount();
    if (rivalEvents > 4)
    {
        if (IsNpcScheduleFestivalDay(season, day))
            return 9;
        if (weekday == SCHEDULE_THURSDAY)
        {
            if (self.weather == 0)
                return 6;
            return 7;
        }
        return 5;
    }
    if (IsNpcScheduleFestivalDay(season, day))
        return 8;
    if (rivalEvents != 0)
    {
        if (weekday == SCHEDULE_MONDAY)
            return 3;
        if (weekday == SCHEDULE_THURSDAY)
            return 4;
        return 2;
    }
    return 1;
}

EC unsigned GetNpcMilestoneEventId(void const *);
EC bool IsNpcMilestoneEventReady(void const *);

ROM_TEXT(0803E8E8) unsigned SelectNpcSchedule_080F77FC(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 4)) < 0)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (IsNpcScheduleFestivalDay(season, day))
        return 4;
    void const * npc = self.unk_1DAC;
    if (GetNpcMilestoneEventId(npc) != 0 && !IsNpcMilestoneEventReady(npc))
        return 5;
    if (weekday == SCHEDULE_THURSDAY)
    {
        if (self.weather == 0)
            return 2;
        return 3;
    }
    return 1;
}

struct CarpenterConstructionState
{
    STRUCT_PAD(0, 0x14);
    u8 jobAndProgress;
    u8 dailyFlags;
};

enum ConstructionJob
{
    CONSTRUCTION_NONE = 0,
    CONSTRUCTION_HOUSE_UPGRADE = 1,
    CONSTRUCTION_COOP_UPGRADE = 2,
    CONSTRUCTION_BARN_UPGRADE = 3,
    CONSTRUCTION_WINDOW_FIRST = 5,
    CONSTRUCTION_WINDOW_SECOND,
    CONSTRUCTION_WINDOW_THIRD,
    CONSTRUCTION_MAILBOX_FIRST,
    CONSTRUCTION_MAILBOX_SECOND,
    CONSTRUCTION_MAILBOX_THIRD,
    CONSTRUCTION_DOGHOUSE_FIRST,
    CONSTRUCTION_DOGHOUSE_SECOND,
    CONSTRUCTION_DOGHOUSE_THIRD,
};

struct CarpenterScheduleContext
{
    STRUCT_PAD(0, 8);
    unsigned weather;
    STRUCT_PAD(0xC, 0x10);
    SavedCalendarDate date;
    STRUCT_PAD(0x14, 0x1F4);
    FarmHouse farmHouse;
    u8 padAfterFarmHouse[0x1F64 - 0x1F4 - sizeof(FarmHouse)];
    CarpenterConstructionState construction;
};

EC unsigned GetConstructionJob(CarpenterConstructionState const &);
EC bool IsConstructionComplete(CarpenterConstructionState const &);
EC bool IsFirstConstructionDay(CarpenterConstructionState const &);

ROM_TEXT(0803ECC0) unsigned SelectCarpenterSchedule(CarpenterScheduleContext const & self)
{
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.date);
    unsigned season = date.season;
    unsigned day = date.day;
    int weekday = date.weekday;
    if (IsNpcScheduleFestivalDay(season, day))
        return 5;
    unsigned weather = self.weather;
    if (GetConstructionJob(self.construction) != CONSTRUCTION_NONE && !IsConstructionComplete(self.construction) &&
        !IsFirstConstructionDay(self.construction) && weather - 3 > 1)
    {
        switch (GetConstructionJob(self.construction))
        {
        case CONSTRUCTION_NONE: goto regular;
        case CONSTRUCTION_HOUSE_UPGRADE:
            switch (self.farmHouse.GetUpgradeLevel())
            {
            case 0: return 6;
            case 1: return 7;
            case 2: return 13;
            }
            break;
        case CONSTRUCTION_COOP_UPGRADE: return 8;
        case CONSTRUCTION_BARN_UPGRADE: return 9;
        case 4: return 14;
        case CONSTRUCTION_WINDOW_FIRST:
        case CONSTRUCTION_WINDOW_SECOND:
        case CONSTRUCTION_WINDOW_THIRD: return 10;
        case CONSTRUCTION_MAILBOX_FIRST:
        case CONSTRUCTION_MAILBOX_SECOND:
        case CONSTRUCTION_MAILBOX_THIRD: return 11;
        case CONSTRUCTION_DOGHOUSE_FIRST:
        case CONSTRUCTION_DOGHOUSE_SECOND:
        case CONSTRUCTION_DOGHOUSE_THIRD: return 12;
        }
    }
regular:
    switch (weekday)
    {
    default: return 1;
    case SCHEDULE_THURSDAY:
    case SCHEDULE_FRIDAY: return 2;
    case SCHEDULE_SATURDAY: return 3;
    case SCHEDULE_SUNDAY: return 4;
    }
}

ROM_TEXT(0803EDCC) unsigned SelectNpcSchedule_080F81BC(NpcScheduleContext const & self)
{
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    u8 day = date.day;
    unsigned weekday = date.weekday;
    u8 festival = IsNpcScheduleFestivalDay(season, day);
    if (festival != 0)
        return 5;
    if (season == SEASON_AUTUMN && day == 30)
    {
        switch ((static_cast<u32>(self.unk_21A2) << 26) >> 30)
        {
        case 2: goto regularDay;
        }
        return 6;
    }
regularDay:
    if (static_cast<i32>(static_cast<u32>(self.unk_215A) << (31 - 3)) < 0)
        return 0;
    if (self.weather == 0)
    {
        if (weekday == SCHEDULE_MONDAY)
            return 1;
        if (weekday == SCHEDULE_WEDNESDAY)
            return 2;
        return 3;
    }
    return 4;
}

ROM_TEXT(0803EDCC) unsigned SelectNpcSchedule_080F8678(NpcScheduleContext const & self)
{
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (IsNpcScheduleFestivalDay(season, day))
        return 4;
    if (weekday == SCHEDULE_MONDAY)
    {
        if (self.weather == 0)
            return 2;
        return 3;
    }
    return 1;
}

struct ControllerInput;
struct AudioPlayer;

EC int UpdatePauseMenuWindow(PauseMenuWindow &, ControllerInput const &, OamBuffer *,
    BackgroundVideoState *, DialogLayerRenderer *, TileTransferQueue *, AudioPlayer *);
EC void func_0803BE50(DialogWindow &, int);
EC void func_080E0C38(PauseMenuGraphics *, int);
EC char const gPauseMenuWindowVtable[];
EC char const gClockHudVTable[];

struct ClockHudVTable
{
    STRUCT_PAD(0, 0x1C);
    int (* getX)(ClockHud &);
    int (* getY)(ClockHud &);
};

ROM_TEXT(0803D4D8) int UpdateClockHud(ClockHud & self, OamBuffer * oam,
    BackgroundVideoState * video, DialogLayerRenderer * layerRenderer, TileTransferQueue * tileTransfers)
{
    // Preserve the original register allocation for the window state.
    register int state asm("r6") = UpdateDialogWindow(self, video, layerRenderer, tileTransfers);
    if (state == DIALOG_WINDOW_OPEN)
    {
        int x = static_cast<ClockHudVTable const *>(self.vtable)->getX(self);
        int y = static_cast<ClockHudVTable const *>(self.vtable)->getY(self);
        DrawClockDisplay(self.display, x, y, oam, layerRenderer);
    }
    return state;
}

EC DialogWindow & func_0803BDFC(DialogWindow &, int, int, int, int, int, int, int, int);

EC PauseMenuGraphics * func_080E0A94(void *);

ROM_TEXT(0803BF78) PauseMenuWindow & InitializePauseMenuWindow(PauseMenuWindow & self, int background,
    int screenBlock, int paletteBank, int layerMode, int selectedTab)
{
    func_0803BDFC(self, 0, 136, 240, 24, background, screenBlock, paletteBank, layerMode);
    self.vtable = gPauseMenuWindowVtable;
    self.graphics = func_080E0A94(::operator new(0x41C));
    self.selectedTab = selectedTab;
    self.needsInitialRefresh = false;
    self.cursorBlinkFrames = 1;
    self.selectionEnabled = true;
    return self;
}

ROM_TEXT(0803BF78) void DestroyPauseMenuWindow(PauseMenuWindow & self, int flags)
{
    self.vtable = gPauseMenuWindowVtable;
    if (self.graphics)
        func_080E0C38(self.graphics, 3);
    func_0803BE50(self, flags);
}

ROM_TEXT(0803BFF0) int UpdatePauseMenuWindowWithoutAudio(PauseMenuWindow & self, ControllerInput const & input,
    OamBuffer * sprites, BackgroundVideoState * video, DialogLayerRenderer * renderer, TileTransferQueue * transfers)
{
    return UpdatePauseMenuWindow(self, input, sprites, video, renderer, transfers, nullptr);
}

ROM_TEXT(0803C00C) int UpdatePauseMenuWindowWithAudio(PauseMenuWindow & self, ControllerInput const & input,
    OamBuffer * sprites, BackgroundVideoState * video, DialogLayerRenderer * renderer, TileTransferQueue * transfers,
    AudioPlayer * audio)
{
    return UpdatePauseMenuWindow(self, input, sprites, video, renderer, transfers, audio);
}

ROM_TEXT(0803C518) ClockHud & InitializeClockHud(ClockHud & self, int background, int screenBlock, int paletteBank, int layerMode)
{
    func_0803BDFC(self, 128, 136, 112, 24, background, screenBlock, paletteBank, layerMode);
    self.vtable = gClockHudVTable;
    InitializeClockDisplay(self.display);
    return self;
}

ROM_TEXT(0803C518) void DestroyClockHud(ClockHud & self, int flags)
{
    self.vtable = gClockHudVTable;
    DestroyClockDisplay(self.display, 2);
    func_0803BE50(self, flags);
}
