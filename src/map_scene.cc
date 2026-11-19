#include "prelude.h"

#include "sprite_animation.hh"
#include "unknown_types.hh"

struct MapUpdateState;
struct G1MapState;
struct Location;
struct MapSceneEntity;

struct MapSceneFrame
{
    u32 frameState;
    u8 flags;
};

struct MapSceneEntityMethods
{
    STRUCT_PAD(0x00, 0x08);
    void (* destroy)(MapSceneEntity &, int);
    STRUCT_PAD(0x0C, 0x10);
    void (* finishMapChange)(MapSceneEntity &);
    void (* prepareMapChange)(MapSceneEntity &);
    STRUCT_PAD(0x18, 0x2C);
    void (* update)(MapSceneEntity &, MapSceneFrame const &);
};

struct MapSceneEntity
{
    STRUCT_PAD(0x00, 0x14);
    MapSceneEntityMethods const * methods;
};

struct SpriteAnimationClipHeader;

struct MapSceneAnimationSlot
{
    STRUCT_PAD(0x00, 0x04);
    SpriteAnimationClipHeader const * clips;
    STRUCT_PAD(0x08, 0x30);
};

struct MapSceneOverlay;
struct GameStateSpriteCache;
struct MapTileEffectQueue;

struct MapSceneEffect
{
    STRUCT_PAD(0x00, 0x24);
    void (* const * methods)(MapSceneEffect &, unsigned);
    SpriteAnimator animator;
    u8 redraw;
    STRUCT_PAD(0x3D, 0x3F);
    u8 skipAnimationFrame;
};

struct MapSceneEffectSlot
{
    MapSceneEffect * effect;
    u32 position;
    bool timed;
    u8 remainingFrames;
    u8 phase;
};

struct MapSceneObjectPalette
{
    u16 colors[16];
    STRUCT_PAD(0x20, 0x81);
    bool flag;
    bool overridden;
    u8 overrideParameter;
};

struct MapScene
{
    STRUCT_PAD(0x00, 0x04);
    MapUpdateState * map;
    MapSceneEntity * entities[100];
    STRUCT_PAD(0x198, 0x1BC);
    MapSceneObjectPalette objectPalettes[16];
    STRUCT_PAD(0x9FC, 0xDB0);
    MapSceneAnimationSlot animations[13];
    STRUCT_PAD(0x1020, 0x1024);
    MapSceneOverlay * overlay;
    MapTileEffectQueue * tileEffects;
    MapSceneEffectSlot effect;
};

EC void func_080AB68C(MapTileEffectQueue *, int, int, u32);
EC i32 func_080AC96C(GameStateSpriteCache &, u32);

struct MapSceneTerrainSource
{
    u32 mapId;
    u32 graphicsId;
    STRUCT_PAD(0x08, 0x2C);
    TerrainInfo const * terrain;
};

struct MapSceneTerrainGrid
{
    TerrainInfo const * terrain;
    u8 const * indices;
    u16 width;
    u16 height;
};

struct MapSceneOwner
{
    MapScene * scene;
};

EC void * func_080CFB28(u8 *);
EC u32 func_0803A8A4(u32 mapId, u32 season, u32 houseLevel, u32 coopLevel, u32 barnLevel);
EC MapData const * GetMapData(u32 graphicsId);
EC void func_080CFFC0(MapUpdateState & map, int column, int row, u32 const & value);
EC void ApplyMapBuildingDoorPatch(G1MapState * map, u32 useUpperPosition);
EC void ApplyMapRow17PatchB(G1MapState * map);
EC void ApplyMapRow17PatchA(G1MapState * map);
EC void ApplyMapIndexedFixturePatch(G1MapState * map, u32 columnIndex);
EC void ApplyMapIndexedOverlayPatch(G1MapState * map, u32 tileIndex);
EC void ApplyMapRow2PatchB(G1MapState * map);
EC void ApplyMapRow2PatchA(G1MapState * map);
EC void ApplyMapRow3PatchB(G1MapState * map);
EC void ApplyMapRow3PatchA(G1MapState * map);
EC void ApplyMapRow15PatchB(G1MapState * map);
EC void ApplyMapRow15PatchA(G1MapState * map);
EC void StartMapTransitionDelay(u8 * self);
EC bool IsMapTransitionIdle(u8 * self);
EC void DestroyFixedSpriteOverlay(MapSceneOverlay *, unsigned);
EC void func_080AB2E4(MapTileEffectQueue *, unsigned);
EC void LoadMapAtLocation(G1MapState *, Location const *, void *, void *, void *);
EC MapSceneOverlay * func_0803AB30(void *, MapScene *);
EC MapTileEffectQueue * func_080AAFC8(void *, MapScene *, unsigned);
EC void UpdateMapTileEffectQueue(MapTileEffectQueue &);

EC SECTION(".text.080ACAF0") void UpdateMapSceneEntitiesAndEffects(MapScene & inputScene, u32 const & inputFrameState)
{
    register MapScene * scene asm("r6") = &inputScene;
    register u32 const * frameSource asm("r1") = &inputFrameState;
    asm ("" : "+l" (scene), "+l" (frameSource));
    u32 const * frameState = frameSource;
    typedef void (* UpdateMap)(MapUpdateState *);
    MapUpdateState * map = scene->map;
    (*reinterpret_cast<UpdateMap const **>(reinterpret_cast<u8 *>(map) + 0x90))[3](map);
    MapTileEffectQueue * queue = scene->tileEffects;
    if (queue)
        UpdateMapTileEffectQueue(*queue);

    register unsigned effectOffset asm("r0") = offsetof(MapScene, effect);
    asm ("" : "+l" (effectOffset));
    register MapSceneEffectSlot * effectSlot asm("r1") = reinterpret_cast<MapSceneEffectSlot *>(reinterpret_cast<u8 *>(scene) + effectOffset);
    asm ("" : "+l" (effectSlot));
    register MapSceneEffect * effect asm("r4") = effectSlot->effect;
    asm ("" : "+l" (effect));
    register unsigned address asm("r4") = reinterpret_cast<unsigned>(effect);
    register unsigned negativeAddress asm("r0") = -address;
    asm ("" : "+l" (negativeAddress));
    asm ("orr %0, %1" : "+l" (negativeAddress) : "l" (address) : "cc");
    if (i32(negativeAddress) < 0)
    {
        register MapSceneEffectSlot * activeSlot asm("r5") = effectSlot;
        asm ("" : "+l" (activeSlot));
        MapSceneEffectSlot & slot = *activeSlot;
        if (slot.timed)
        {
            register unsigned remaining asm("r0") = slot.remainingFrames - 1;
            asm ("" : "+l" (remaining));
            register unsigned zero asm("r2") = 0;
            asm ("" : "+l" (zero), "+l" (remaining));
            slot.remainingFrames = remaining;
            if (static_cast<u8>(remaining) == 0)
            {
                register MapSceneEffect * previous asm("r1") = slot.effect;
                asm ("" : "+l" (previous));
                if (zero != reinterpret_cast<unsigned>(previous) && previous)
                    previous->methods[2](*previous, 3);
                slot.effect = 0;
            }
            else
            {
                register MapSceneEffect * active asm("r4") = slot.effect;
                asm ("" : "+l" (active));
                register u8 * skip asm("r1") = &active->skipAnimationFrame;
                asm ("" : "+l" (skip));
                if (!*skip)
                {
                    if (i32(AdvanceSpriteAnimator(&active->animator) << 30) < 0)
                    {
                        register u8 * redraw asm("r1") = &active->redraw;
                        asm ("" : "+l" (redraw));
                        *redraw = 1;
                    }
                }
                else
                    *skip = zero;
            }
            slot.phase = (slot.phase + 1) & 7;
        }
        else
        {
            register u32 changes asm("r0");
            register u8 * skip asm("r1") = &effect->skipAnimationFrame;
            asm ("" : "+l" (skip));
            if (!*skip)
            {
                register u32 animationChanges asm("r2") = AdvanceSpriteAnimator(&effect->animator);
                asm ("" : "+l" (animationChanges));
                if (i32(animationChanges << 30) < 0)
                {
                    register u8 * redraw asm("r1") = &effect->redraw;
                    asm ("" : "+l" (redraw));
                    *redraw = 1;
                }
                changes = animationChanges;
            }
            else
            {
                *skip = slot.timed;
                changes = SpriteAnimator::FRAME_CHANGED;
            }
            asm ("" : "+l" (changes));
            if (i32(changes << 29) < 0)
            {
                MapSceneEffect * cleared = 0;
                asm ("" : "+l" (cleared));
                MapSceneEffect * previous = slot.effect;
                if (cleared != previous && previous)
                    previous->methods[2](*previous, 3);
                slot.effect = cleared;
            }
        }
    }

    MapSceneEntity ** entry = scene->entities;
    register unsigned entitiesEndOffset asm("r1") = offsetof(MapScene, entities) + sizeof(scene->entities);
    asm ("" : "+l" (entitiesEndOffset));
    register MapSceneEntity ** end asm("r5") = reinterpret_cast<MapSceneEntity **>(reinterpret_cast<u8 *>(scene) + entitiesEndOffset);
    asm ("" : "+l" (end));
    MapSceneFrame frame;
    register MapSceneFrame * frameOutput asm("r0") = &frame;
    asm ("" : "+l" (frameOutput));
    frame.frameState = *frameState;
    frameOutput->flags = 0;
    if (entry != end)
    {
        do
        {
            MapSceneEntity * entity = *entry;
            if (entity)
                entity->methods->update(*entity, frame);
            ++entry;
        }
        while (entry != end);
    }
}

struct MapSceneStartPosition
{
    u16 mapIdAndXFraction;
    u16 xAndYFraction;
    u16 yAndPadding;
};

EC void func_080AC674(void *, void **);
EC void func_080AD49C(MapScene &, unsigned);
EC u8 vtable_unk_080E8440[];
EC u8 vtable_unk_080E8344[];

EC SECTION(".text.080AC9EC") MapScene * ConstructMapScene(void * memory, void ** inputResource)
{
    register MapScene * scene asm("r6") = static_cast<MapScene *>(memory);
    register void ** incomingResource asm("r1") = inputResource;
    asm ("" : "+l" (scene), "+l" (incomingResource));
    void * volatile * resource = incomingResource;
    *reinterpret_cast<void * volatile *>(scene) = vtable_unk_080E8440;
    register void * sceneMethods asm("r1") = vtable_unk_080E8344;
    asm ("" : "+l" (sceneMethods));
    register void ** base asm("r0") = reinterpret_cast<void **>(scene);
    asm ("" : "+l" (base));
    *base++ = sceneMethods;
    asm ("" : "+l" (base));
    register void * transferred asm("r2") = *resource;
    asm ("" : "+l" (transferred));
    *resource = 0;
    void * owned = transferred;
    func_080AC674(base, &owned);

    MapSceneStartPosition start;
    asm ("" : "=m" (start));
    register MapUpdateState * map asm("r0") = scene->map;
    asm ("" : "+l" (map));
    register MapSceneStartPosition * position asm("r1") = &start;
    asm ("" : "+l" (position));
    register unsigned startMapId asm("r5") = 48;
    asm ("" : "+l" (startMapId));
    register unsigned mapWord asm("r3") = position->mapIdAndXFraction;
    asm ("" : "+l" (mapWord));
    register unsigned coordinatePadding asm("r4") = ~0x3FFu;
    asm ("" : "+l" (coordinatePadding));
    position->mapIdAndXFraction = (coordinatePadding & mapWord) | startMapId;
    register u8 * bytes asm("r1") = reinterpret_cast<u8 *>(position);
    register unsigned xFractionByte asm("r5") = bytes[1];
    asm ("" : "+l" (xFractionByte));
    register unsigned fractionMask asm("r3") = 3;
    asm ("" : "+l" (fractionMask));
    bytes[1] = fractionMask & xFractionByte;
    register unsigned xWord asm("r5") = position->xAndYFraction;
    asm ("" : "+l" (xWord));
    position->xAndYFraction = coordinatePadding & xWord;
    register unsigned yFractionByte asm("r2") = bytes[3];
    asm ("" : "+l" (yFractionByte));
    register unsigned clearedFraction asm("r3") = fractionMask & yFractionByte;
    asm ("" : "+l" (clearedFraction));
    bytes[3] = clearedFraction;
    register unsigned yWord asm("r2") = position->yAndPadding;
    asm ("" : "+l" (yWord));
    register unsigned clearedY asm("r4") = coordinatePadding & yWord;
    asm ("" : "+l" (clearedY));
    position->yAndPadding = clearedY;
    asm ("" : "+l" (map), "+l" (position));
    LoadMapAtLocation(reinterpret_cast<G1MapState *>(map), reinterpret_cast<Location const *>(position), 0, 0, reinterpret_cast<void *>(30));

    register MapSceneOverlay * overlay asm("r0") = func_0803AB30(operator new(0xA0), scene);
    asm ("" : "+l" (overlay));
    register MapSceneOverlay ** overlaySlot asm("r5") = &scene->overlay;
    asm ("" : "+l" (overlaySlot));
    MapSceneOverlay * nextOverlay = overlay;
    asm ("" : "+l" (nextOverlay));
    if (nextOverlay != *overlaySlot && *overlaySlot)
        DestroyFixedSpriteOverlay(*overlaySlot, 3);
    *overlaySlot = nextOverlay;

    register unsigned queueOffset asm("r0") = offsetof(MapScene, tileEffects);
    asm ("" : "+l" (queueOffset));
    register MapTileEffectQueue ** queueSlot asm("r4") = reinterpret_cast<MapTileEffectQueue **>(reinterpret_cast<u8 *>(scene) + queueOffset);
    asm ("" : "+l" (queueSlot));
    register MapTileEffectQueue * emptyQueue asm("r5") = 0;
    asm ("" : "+l" (emptyQueue));
    if (emptyQueue != *queueSlot && *queueSlot)
        func_080AB2E4(*queueSlot, 3);
    *queueSlot = emptyQueue;

    void * queueMemory = operator new(4);
    unsigned graphicsId = reinterpret_cast<MapSceneTerrainSource const *>(scene->map)->graphicsId;
    register MapTileEffectQueue * queue asm("r0") = func_080AAFC8(queueMemory, scene, graphicsId);
    asm ("" : "+l" (queue));
    register MapTileEffectQueue ** targetQueue asm("r5") = &scene->tileEffects;
    asm ("" : "+l" (targetQueue));
    MapTileEffectQueue * nextQueue = queue;
    asm ("" : "+l" (nextQueue));
    if (nextQueue != *targetQueue && *targetQueue)
        func_080AB2E4(*targetQueue, 3);
    *targetQueue = nextQueue;

    for (unsigned slot = 0; slot < 100; ++slot)
        func_080AD49C(*scene, slot);

    void * pending = *resource;
    if (pending)
    {
        typedef void (* Destroy)(void *, unsigned);
        (*reinterpret_cast<Destroy const **>(static_cast<u8 *>(pending) + 0x90))[2](pending, 3);
    }
    return scene;
}

EC SECTION(".text.080AD3B0") void ReloadMapSceneResources(MapScene & scene, Location const * location)
{
    MapSceneEntity ** entry = scene.entities;
    MapSceneEntity ** end = entry + 100;
    if (entry != end)
    {
        do
        {
            register MapSceneEntity * entity asm("r2") = *entry;
            asm ("" : "+l" (entity));
            if (entity)
                entity->methods->prepareMapChange(*entity);
            ++entry;
        }
        while (entry != end);
    }
    register unsigned overlayOffset asm("r1") = offsetof(MapScene, overlay);
    asm ("" : "+l" (overlayOffset));
    register MapSceneOverlay ** overlaySlot asm("r4") = reinterpret_cast<MapSceneOverlay **>(reinterpret_cast<u8 *>(&scene) + overlayOffset);
    asm ("" : "+l" (overlaySlot));
    register MapSceneOverlay * emptyOverlay asm("r5") = 0;
    asm ("" : "+l" (emptyOverlay));
    if (emptyOverlay != *overlaySlot && *overlaySlot)
        DestroyFixedSpriteOverlay(*overlaySlot, 3);
    *overlaySlot = emptyOverlay;
    register unsigned queueOffset asm("r0") = offsetof(MapScene, tileEffects);
    asm ("" : "+l" (queueOffset));
    register MapTileEffectQueue ** queueSlot asm("r4") = reinterpret_cast<MapTileEffectQueue **>(reinterpret_cast<u8 *>(&scene) + queueOffset);
    asm ("" : "+l" (queueSlot));
    register MapTileEffectQueue * emptyQueue asm("r5") = 0;
    asm ("" : "+l" (emptyQueue));
    if (emptyQueue != *queueSlot && *queueSlot)
        func_080AB2E4(*queueSlot, 3);
    *queueSlot = emptyQueue;
    LoadMapAtLocation(reinterpret_cast<G1MapState *>(scene.map), location, 0, 0, reinterpret_cast<void *>(30));
    register MapSceneOverlay * overlay asm("r0") = func_0803AB30(operator new(0xA0), &scene);
    asm ("" : "+l" (overlay));
    register MapSceneOverlay ** targetOverlay asm("r5") = &scene.overlay;
    asm ("" : "+l" (targetOverlay));
    MapSceneOverlay * nextOverlay = overlay;
    asm ("" : "+l" (nextOverlay));
    if (nextOverlay != *targetOverlay && *targetOverlay)
        DestroyFixedSpriteOverlay(*targetOverlay, 3);
    *targetOverlay = nextOverlay;
    void * queueMemory = operator new(4);
    unsigned graphicsId = reinterpret_cast<MapSceneTerrainSource const *>(scene.map)->graphicsId;
    register MapTileEffectQueue * queue asm("r0") = func_080AAFC8(queueMemory, &scene, graphicsId);
    asm ("" : "+l" (queue));
    register MapTileEffectQueue ** targetQueue asm("r5") = &scene.tileEffects;
    asm ("" : "+l" (targetQueue));
    MapTileEffectQueue * nextQueue = queue;
    asm ("" : "+l" (nextQueue));
    if (nextQueue != *targetQueue && *targetQueue)
        func_080AB2E4(*targetQueue, 3);
    *targetQueue = nextQueue;
    entry = scene.entities;
    end = entry + 100;
    if (entry != end)
    {
        do
        {
            register MapSceneEntity * entity asm("r2") = *entry;
            asm ("" : "+l" (entity));
            if (entity)
                entity->methods->finishMapChange(*entity);
            ++entry;
        }
        while (entry != end);
    }
}

EC SECTION(".text.080AD69C") MapSceneTerrainGrid & GetMapSceneTerrainGrid(MapSceneTerrainGrid & result, MapScene const & scene, u32 requestedMap)
{
    register MapSceneTerrainGrid * output asm("r5") = &result;
    register MapScene const * state asm("sb") = &scene;
    register u32 mapId asm("r4") = requestedMap;
    asm ("" : "+l" (output), "+h" (state), "+l" (mapId));
    if (mapId > 563)
    {
        output->terrain = 0;
        output->indices = 0;
        output->width = 0;
        output->height = 0;
    }
    else
    {
        register MapScene const * source asm("r1") = state;
        asm ("" : "+l" (source));
        register MapSceneTerrainSource const * current asm("r0") = reinterpret_cast<MapSceneTerrainSource const *>(source->map);
        asm ("" : "+l" (current));
        u32 currentMap = current->mapId;
        asm ("" : "+l" (currentMap));
        u32 graphicsId = currentMap == mapId ? current->graphicsId : func_0803A8A4(mapId, 1, 0, 0, 0);
        register MapData const * data asm("r0") = GetMapData(graphicsId);
        asm ("" : "+l" (data));
        register unsigned width asm("r6") = data->width;
        asm ("" : "+l" (width));
        register unsigned heightValue asm("r1") = data->height;
        asm ("" : "+l" (heightValue));
        register unsigned height asm("r8") = heightValue;
        asm ("" : "+h" (height));
        if (currentMap == mapId)
        {
            MapSceneTerrainSource const * active = reinterpret_cast<MapSceneTerrainSource const *>(state->map);
            GetMapData(active->graphicsId);
            register unsigned noIndices asm("r1") = 0;
            asm ("" : "+l" (noIndices));
            output->terrain = active->terrain;
            output->indices = reinterpret_cast<u8 const *>(noIndices);
            output->width = width;
            register unsigned liveHeight asm("r1") = height;
            asm ("" : "+l" (liveHeight));
            output->height = liveHeight;
        }
        else
        {
            register u8 const * indices asm("r1") = data->terrain_map;
            asm ("" : "+l" (indices));
            output->terrain = data->terrain_info;
            output->indices = indices;
            output->width = width;
            output->height = height;
        }
    }
    return *output;
}

EC SECTION(".text.080AD724") MapSceneOwner * InitializeMapSceneOwner(MapSceneOwner * owner)
{
    struct SceneTemporary
    {
        MapScene * volatile owned;
        void * resource;
        MapScene * volatile * volatile source;
        MapScene * volatile transferred;
    } temporary;
    temporary.resource = func_080CFB28(static_cast<u8 *>(operator new(0x1E4)));
    MapScene * scene = ConstructMapScene(operator new(0x1038), &temporary.resource);
    temporary.owned = scene;
    register MapScene * volatile * source asm("r1") = &temporary.owned;
    asm ("" : "+l" (source));
    temporary.source = source;
    temporary.transferred = scene;
    register MapScene * volatile * released asm("r2") = source;
    asm ("" : "+l" (released));
    *released = 0;
    owner->scene = scene;
    MapScene * pending = temporary.owned;
    if (pending)
    {
        typedef void (* DestroyScene)(MapScene *, unsigned);
        reinterpret_cast<DestroyScene const * const *>(pending)[0][2](pending, 3);
    }
    return owner;
}

EC SECTION(".text.080AD8C4") void QueueMapSceneTileEffect(MapScene & scene, int tileX, int tileY, u32 effectId)
{
    register MapScene * state asm("r5") = &scene;
    register int column asm("r6") = tileX;
    int row = tileY;
    register u32 effect asm("r4") = effectId;
    asm ("" : "+l" (state), "+l" (column), "+l" (row), "+l" (effect));
    register int offsetX asm("r2") = 0;
    register int offsetY asm("r3") = 0;
    switch (effect)
    {
    case 0:
    case 1:
    case 2:
    case 3:
        offsetX = 8;
        offsetY = 16;
        break;
    case 4:
        offsetX = 8;
        offsetY = 11;
        break;
    case 6:
    case 7:
    case 8:
        offsetX = 16;
        offsetY = 32;
        break;
    }
    register u32 queueOffset asm("r1") = offsetof(MapScene, tileEffects);
    asm ("" : "+l" (queueOffset));
    register MapTileEffectQueue ** queueSlot asm("r0") = reinterpret_cast<MapTileEffectQueue **>(reinterpret_cast<u8 *>(state) + queueOffset);
    asm ("" : "+l" (queueSlot));
    register MapTileEffectQueue * queue asm("r0") = *queueSlot;
    asm ("" : "+l" (queue), "+l" (offsetX), "+l" (offsetY));
    register int pixelX asm("r1") = column * 16 + offsetX;
    asm ("" : "+l" (pixelX));
    register int pixelY asm("r2") = row * 16;
    asm ("" : "+l" (pixelY));
    offsetY += 56;
    asm ("" : "+l" (offsetY));
    func_080AB68C(queue, pixelX, pixelY + offsetY, effect);
}

EC SECTION(".text.080ADAFC") i32 GetMapSceneCachedSpriteSlot(MapScene & scene, u32 slot)
{
    u8 * cache = reinterpret_cast<u8 *>(&scene) + 0x19c;
    u8 & cached = cache[slot];
    i32 spriteSlot = cached - 0x10;
    if (spriteSlot < 0)
    {
        spriteSlot = func_080AC96C(*reinterpret_cast<GameStateSpriteCache *>(cache), slot);
        cached = spriteSlot + 0x10;
    }
    return spriteSlot;
}

struct MapTileEffectResource
{
    u32 metadata;
    u32 handle;
};

struct MapTileEffectAssets
{
    u8 active;
    u8 loaded;
    STRUCT_PAD(2, 0x44);
    MapTileEffectResource resources[2];
};

struct MapTileEffect
{
    i16 x;
    i16 y;
    u8 active;
    u8 frames;
    u8 type;
    u8 padding;
};

struct MapTileEffectState
{
    STRUCT_PAD(0, 8);
    MapTileEffectAssets assets[10];
    STRUCT_PAD(0x350, 0x368);
    MapTileEffect effects[16];
};

struct MapTileEffectQueue
{
    MapTileEffectState * state;
};

EC void func_08007C28(MapTileEffectResource &, u32);

EC SECTION(".text.080AB380") void UpdateMapTileEffectQueue(MapTileEffectQueue & queue)
{
    register MapTileEffectState * state asm("r8") = queue.state;
    asm ("" : "+h" (state));
    unsigned active = 0;
    unsigned expired = 0;
    unsigned index = 0;
    register unsigned inactive asm("r5") = 0;
    asm ("" : "+l" (inactive));
    MapTileEffect * effect = state->effects;
    do
    {
        if (effect->active)
        {
            ++active;
            if (effect->frames > 1)
                --effect->frames;
            else
            {
                effect->active = inactive;
                ++expired;
            }
        }
        ++effect;
    }
    while (++index < 16);

    if (active != 0 && expired == active)
    {
        unsigned type = 0;
        do
        {
            register unsigned offset asm("r0") = sizeof(MapTileEffectAssets);
            asm ("" : "+l" (offset));
            offset *= type;
            asm ("" : "+l" (offset));
            offset += offsetof(MapTileEffectState, assets);
            asm ("" : "+l" (offset));
            register MapTileEffectState * base asm("r2") = state;
            asm ("" : "=l" (base) : "0" (base));
            register MapTileEffectAssets * assets asm("r5") = reinterpret_cast<MapTileEffectAssets *>(reinterpret_cast<u8 *>(base) + offset);
            asm ("" : "+l" (assets));
            register unsigned loaded asm("r0") = assets->active;
            asm ("" : "+l" (loaded));
            unsigned nextType = type + 1;
            asm ("" : "+l" (nextType));
            if (loaded)
            {
                register unsigned resource asm("r6") = 0;
                asm ("" : "+l" (resource));
                u32 zero = 0;
                asm ("" : "+l" (zero) : "l" (resource));
                u32 * handle = &assets->resources[0].handle;
                do
                {
                    asm ("" : : "l" (resource));
                    register unsigned resourceOffset asm("r0") = resource * sizeof(MapTileEffectResource);
                    asm ("" : "+l" (resourceOffset));
                    register u8 * record asm("r0");
                    asm ("add %0, %1, %2" : "=l" (record) : "l" (resourceOffset), "l" (assets) : "cc");
                    func_08007C28(*reinterpret_cast<MapTileEffectResource *>(record + offsetof(MapTileEffectAssets, resources)), *handle);
                    *handle = zero;
                    handle += sizeof(MapTileEffectResource) / sizeof(u32);
                    ++resource;
                    asm ("" : "+l" (resource));
                }
                while (resource < 2);
                register unsigned reset asm("r0") = 0;
                asm ("" : "+l" (reset));
                assets->active = reset;
                assets->loaded = reset;
            }
            type = nextType;
        }
        while (type < 10);
    }
}

EC u8 const gUnk_081074F6[27];
EC u8 const * func_080E0E80(u8 const *, u8 const *, unsigned const *, unsigned, unsigned);

EC SECTION(".text.080ADBB4") bool IsMapEventCalendarDate(u8 const * packedDate)
{
    register u32 date asm("r2") = *packedDate;
    asm ("" : "+l" (date));
    register u32 season asm("r1") = (date << 30) >> 30;
    asm ("" : "+l" (season));
    u32 day = (date << 25) >> 27;
    unsigned yearDay = season * 30 + day;
    register u8 const * dates asm("r0") = gUnk_081074F6;
    asm ("" : "+l" (dates));
    u8 const * end = dates + 27;
    u8 const * event = func_080E0E80(dates, end, &yearDay, 0, 0);
    return event != end && yearDay >= *event;
}

EC SECTION(".text.080ADA78") MapSceneOverlay * GetMapSceneOverlay(MapScene const & scene)
{
    return scene.overlay;
}

EC SECTION(".text.080ADACC") void SetMapSceneObjectPalette(MapScene & scene, u16 const * colors, int slot, u8 overrideParameter)
{
    MapSceneObjectPalette * palettes = scene.objectPalettes;
    asm ("" : "+l" (palettes));
    MapSceneObjectPalette & palette = palettes[slot];
    memcpy(palette.colors, colors, sizeof palette.colors);
    palette.overridden = true;
    palette.overrideParameter = overrideParameter;
}

EC SECTION(".text.080ADB24") void ClearMapSceneObjectPaletteFlag(MapScene & scene, int slot)
{
    scene.objectPalettes[slot].flag = false;
}
EC void StartMapCameraPan(MapUpdateState & map, int x, int y, u32 pixelsPerStep);
EC void func_080A5960(MapUpdateState & map, int x, int y);

EC SECTION(".text.080AD938") void SetMapGridCell(MapScene & scene, int column, int row, u32 const & value)
{
    func_080CFFC0(*scene.map, column, row, value);
}

EC SECTION(".text.080AD944") void ApplyMapPatchAtX1(MapScene & scene, u32 useUpperPosition)
{
    ApplyMapBuildingDoorPatch(reinterpret_cast<G1MapState *>(scene.map), useUpperPosition);
}

EC SECTION(".text.080AD950") int ApplyMapPatchAtY17VariantA(MapScene & scene)
{
    ApplyMapRow17PatchB(reinterpret_cast<G1MapState *>(scene.map));
    return 0;
}

EC SECTION(".text.080AD960") int ApplyMapPatchAtY17VariantB(MapScene & scene)
{
    ApplyMapRow17PatchA(reinterpret_cast<G1MapState *>(scene.map));
    return 0;
}

EC SECTION(".text.080AD970") void ApplyMapPatchFromColumnIndex(MapScene & scene, u32 columnIndex)
{
    ApplyMapIndexedFixturePatch(reinterpret_cast<G1MapState *>(scene.map), columnIndex);
}

EC SECTION(".text.080AD97C") void ApplyMapPatchFromTileIndex(MapScene & scene, u32 tileIndex)
{
    ApplyMapIndexedOverlayPatch(reinterpret_cast<G1MapState *>(scene.map), tileIndex);
}

EC SECTION(".text.080AD988") void ApplyMapPatchAtY2VariantB(MapScene & scene)
{
    ApplyMapRow2PatchB(reinterpret_cast<G1MapState *>(scene.map));
}

EC SECTION(".text.080AD994") void ApplyMapPatchAtY2VariantA(MapScene & scene)
{
    ApplyMapRow2PatchA(reinterpret_cast<G1MapState *>(scene.map));
}

EC SECTION(".text.080AD9A0") void ApplyMapPatchAtY3VariantB(MapScene & scene)
{
    ApplyMapRow3PatchB(reinterpret_cast<G1MapState *>(scene.map));
}

EC SECTION(".text.080AD9AC") void ApplyMapPatchAtY3VariantA(MapScene & scene)
{
    ApplyMapRow3PatchA(reinterpret_cast<G1MapState *>(scene.map));
}

EC SECTION(".text.080AD9B8") void ApplyMapPatchAtY15VariantB(MapScene & scene)
{
    ApplyMapRow15PatchB(reinterpret_cast<G1MapState *>(scene.map));
}

EC SECTION(".text.080AD9C4") void ApplyMapPatchAtY15VariantA(MapScene & scene)
{
    ApplyMapRow15PatchA(reinterpret_cast<G1MapState *>(scene.map));
}

EC SECTION(".text.080AD9D0") void StartMapTileAnimationDelay(MapScene & scene)
{
    StartMapTransitionDelay(reinterpret_cast<u8 *>(scene.map));
}


EC SECTION(".text.080AD9DC") SpriteAnimationClipHeader const * const & GetMapSceneAnimationSlot12(MapScene const & scene)
{
    return scene.animations[12].clips;
}

EC SECTION(".text.080AD9E8") SpriteAnimationClipHeader const * const & GetMapSceneAnimationSlot11(MapScene const & scene)
{
    return scene.animations[11].clips;
}

EC SECTION(".text.080AD9F4") SpriteAnimationClipHeader const * const & GetMapSceneAnimationSlot10(MapScene const & scene)
{
    return scene.animations[10].clips;
}

EC SECTION(".text.080ADA00") SpriteAnimationClipHeader const * const & GetMapSceneAnimationSlot9(MapScene const & scene)
{
    return scene.animations[9].clips;
}

EC SECTION(".text.080ADA0C") SpriteAnimationClipHeader const * const & GetMapSceneAnimationSlot8(MapScene const & scene)
{
    return scene.animations[8].clips;
}

EC SECTION(".text.080ADA18") SpriteAnimationClipHeader const * const & GetMapSceneAnimationSlot7(MapScene const & scene)
{
    return scene.animations[7].clips;
}

EC SECTION(".text.080ADA24") SpriteAnimationClipHeader const * const & GetMapSceneAnimationSlot6(MapScene const & scene)
{
    return scene.animations[6].clips;
}

EC SECTION(".text.080ADA30") SpriteAnimationClipHeader const * const & GetMapSceneAnimationSlot5(MapScene const & scene)
{
    return scene.animations[5].clips;
}

EC SECTION(".text.080ADA3C") SpriteAnimationClipHeader const * const & GetMapSceneAnimationSlot4(MapScene const & scene)
{
    return scene.animations[4].clips;
}

EC SECTION(".text.080ADA48") SpriteAnimationClipHeader const * const & GetMapSceneAnimationSlot3(MapScene const & scene)
{
    return scene.animations[3].clips;
}

EC SECTION(".text.080ADA54") SpriteAnimationClipHeader const * const & GetMapSceneAnimationSlot2(MapScene const & scene)
{
    return scene.animations[2].clips;
}

EC SECTION(".text.080ADA60") SpriteAnimationClipHeader const * const & GetMapSceneAnimationSlot1(MapScene const & scene)
{
    return scene.animations[1].clips;
}

EC SECTION(".text.080ADA6C") SpriteAnimationClipHeader const * const & GetMapSceneAnimationSlot0(MapScene const & scene)
{
    return scene.animations[0].clips;
}

EC SECTION(".text.080ADB78") bool IsMapScrollIdle(MapScene & scene)
{
    return IsMapTransitionIdle(reinterpret_cast<u8 *>(scene.map));
}

EC SECTION(".text.080ADB9C") void ScrollMapTowardPixel(MapScene & scene, int x, int y, u32 pixelsPerStep)
{
    StartMapCameraPan(*scene.map, x, y, pixelsPerStep);
}

EC SECTION(".text.080ADBA8") void CenterMapScrollOnPixel(MapScene & scene, int x, int y)
{
    func_080A5960(*scene.map, x, y);
}

EC SECTION(".text.080ADA84") u32 GetDefaultMapPixelHeight(MapScene const &, u32 mapId)
{
    return GetMapData(func_0803A8A4(mapId, 1, 0, 0, 0))->height * 8;
}

EC SECTION(".text.080ADAA8") u32 GetDefaultMapPixelWidth(MapScene const &, u32 mapId)
{
    return GetMapData(func_0803A8A4(mapId, 1, 0, 0, 0))->width * 8;
}

EC SECTION(".text.080ADB54") void ClearMapSceneEntitySlot(MapScene & scene, u32 index)
{
    MapSceneEntity ** entities = scene.entities;
    asm("" : "+l"(entities));
    MapSceneEntity * & slot = entities[index];
    if (slot != nullptr)
    {
        slot->methods->destroy(*slot, 3);
        slot = nullptr;
    }
}

struct MapScrollFields
{
    u32 mapId;
    STRUCT_PAD(0x04, 0x0A);
    i16 scrollPixelX;
    STRUCT_PAD(0x0C, 0x0E);
    i16 scrollPixelY;
};

EC SECTION(".text.080ADB84") int GetSceneMapScrollY(MapScene const & scene)
{
    return reinterpret_cast<MapScrollFields const *>(scene.map)->scrollPixelY;
}

EC SECTION(".text.080ADB8C") int GetSceneMapScrollX(MapScene const & scene)
{
    return reinterpret_cast<MapScrollFields const *>(scene.map)->scrollPixelX;
}

EC SECTION(".text.080ADB94") u32 GetSceneMapId(MapScene const & scene)
{
    return reinterpret_cast<MapScrollFields const *>(scene.map)->mapId;
}
