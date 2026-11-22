#include "prelude.h"

#include <new>

#include "entity.hh"
#include "farm.hh"
#include "farmer.hh"
#include "rucksack_item.hh"
#include "utility/vec2.hh"
#include "map_overlay.hh"

#include <string.h>

struct GameStateAudioList;
struct GameStateMapEffect;
struct GameStatePlayerData;
struct GameStateScript;
struct MineFloor;
struct Unk_0800E324;

struct GameStateCameraCoordinate
{
    u16 fraction;
    i16 pixel;
};

struct GameStateMapView;

struct GameStateMapDisplayVtable
{
    STRUCT_PAD(0x0, 0x14);
    void (*apply_daylight)(GameStateMapView & view, u32 band, int amount, int remaining);
};

struct GameStateMapView
{
    u32 id;
    u32 map;
    GameStateCameraCoordinate camera_x;
    GameStateCameraCoordinate camera_y;
    u16 * tilemaps[3];
    STRUCT_PAD(0x1C, 0x29);
    u8 tilemapsDirty;
    STRUCT_PAD(0x2A, 0x2C);
    TerrainInfo * expanded_terrain;
    STRUCT_PAD(0x30, 0x90);
    GameStateMapDisplayVtable const * display_vtable;
};

struct GameStateMapTerrain
{
    GameStateMapTerrain(TerrainInfo const * info, u8 const * map, u16 width, u16 height)
        : info(info), map(map), width(width), height(height) {}

    GameStateMapTerrain(TerrainInfo const * info, u16 width, u16 height)
        : info(info), map(nullptr), width(width), height(height) {}

    TerrainInfo const * info;
    u8 const * map;
    u16 width;
    u16 height;
};

struct GameStateMapCalendar
{
    u8 year;
    GameDate date;
    u8 hour : 5;
};

struct GameStateMapData
{
    STRUCT_PAD(0x0, 0x3);
    u8 : 7;
    u8 map_12_scenery_variant : 1;
    u8 map_27_scenery_variant : 1;
    STRUCT_PAD(0x5, 0x8);
    u32 farm_layout;
    STRUCT_PAD(0xC, 0x10);
    GameStateMapCalendar clock;
    Farm farm;
    u8 reserved[0x1BD8 - 0x14 - sizeof(Farm)];
    Farmer farmer;
    u8 reserved_after_farmer[0x2192 - 0x1BD8 - sizeof(Farmer)];
    u8 shipping_collection_flags;
    STRUCT_PAD(0x2193, 0x34C4);
    bool player_only_when_disabled;
    STRUCT_PAD(0x34C5, 0x34D8);
    u32 tile_variant_flags;
};

struct GameStateMapOwner;
struct GameStateVaseDisplay;
struct GameStatePackedSpriteArchive;
struct MapTileEffectQueue;

struct GameStateMapInteractionBase
{
    virtual ~GameStateMapInteractionBase();

    STRUCT_PAD(0x0, 0x24);
};

struct GameStateMapInteraction : GameStateMapInteractionBase
{
    SpriteAnimator animator;
    bool playing;
    bool start;
    bool ended;
    bool restart;
};

struct GameStateFieldObject
{
    u8 unused;
    u8 kind : 2;
    u8 action : 2;
    u16 reserved;
    u16 tile_x;
    u16 tile_y;
    GameStateMapInteraction * interaction;
};

struct GameStateFarmPlot
{
    GameStateFarmPlot() : plot(nullptr), tile_x(0), tile_y(0), plot_x(0), plot_y(0) {}

    FieldPlot * plot;
    u16 tile_x;
    u16 tile_y;
    u16 plot_x;
    u16 plot_y;
};

struct GameStateMinePlot
{
    u16 tile_x;
    u16 tile_y;
    u16 plot_x;
    u16 plot_y;
};

EC u32 GetMineFloorKind(MineFloor const & floor);
EC int GetMineFloorWidth(MineFloor const & floor);
EC int GetMineFloorHeight(MineFloor const & floor);

enum GameStatePlotPlacement
{
    GAME_STATE_PLOT_CAN_PLACE,
    GAME_STATE_PLOT_CANNOT_PLACE,
    GAME_STATE_PLOT_OUTSIDE_FIELD,
};

struct GameStateScriptEvent
{
    GameStateScriptEvent(u32 id, u32 argument) : id(id), argument(argument) {}

    u32 id;
    u32 argument;
};

struct GameStateClock
{
    u32 : 16;
    u32 hour : 5;
    u32 minute : 6;
};

struct GameStateMusicParams
{
    GameStateMusicParams() : tempo(256), volume(256), pitch(0), pan(0), modulation(0), lfo_speed(0) {}

    u16 tempo;
    u16 volume;
    i16 pitch;
    i8 pan;
    u8 modulation;
    u8 lfo_speed;
};

struct GameStateEntitySlotVtable
{
    STRUCT_PAD(0x0, 0x14);
    u32 (*get_map_id)(GameStateMapOwner const & game);
    STRUCT_PAD(0x18, 0x3C);
    void (*remove_entity)(GameStateMapOwner & game, u32 slot);
    AEntity * (*get_entity)(GameStateMapOwner & game, u32 slot);
    AEntity const * (*get_const_entity)(GameStateMapOwner const & game, u32 slot);
    STRUCT_PAD(0x48, 0x5C);
    GameStatePackedSpriteArchive const * (*get_interaction_archive)(GameStateMapOwner const & game);
    GameStatePackedSpriteArchive const * (*get_scenery_archive)(GameStateMapOwner const & game);
    GameStatePackedSpriteArchive const * (*get_vase_archive)(GameStateMapOwner const & game);
    STRUCT_PAD(0x68, 0x140);
    void (*queue_event)(GameStateMapOwner & game, GameStateScriptEvent const & event, u32 queue);
    STRUCT_PAD(0x144, 0x148);
    u32 (*get_music_track)(GameStateMapOwner const & game);
};

struct GameStateSpriteCacheEntry
{
    STRUCT_PAD(0x0, 0x80);
    i8 resourceIndex;
    bool active;
    STRUCT_PAD(0x82, 0x84);
};

struct GameStateSpriteCacheResource
{
    STRUCT_PAD(0, 8);
    u8 spriteSlot;
    i8 paletteIndex;
    u8 flags;
    STRUCT_PAD(0xB, 0xC);
};

struct GameStateSpriteCache
{
    u8 slot_ids[32];
    GameStateSpriteCacheEntry entries[22];
    GameStateSpriteCacheResource resources[13];
    u8 assignedResources;
    STRUCT_PAD(0xC15, 0xC18);
};

struct GameStatePackedSpriteArchive
{
    void const * vtable;
    void const * tables[7];
    u16 counts[7];
};


struct GameStateMapOwner
{
    GameStateEntitySlotVtable const * vtable;
    GameStateMapView * map_view;
    AEntity * entities[100];
    STRUCT_PAD(0x198, 0x19C);
    GameStateSpriteCache sprite_cache;
    GameStatePackedSpriteArchive sprite_archives[13];
    GameStateMapEffect * map_effect;
    MapTileEffectQueue * tile_effects;
    STRUCT_PAD(0x102C, 0x1038);
    GameStateMapData * map_data;
    bool entities_enabled;
    STRUCT_PAD(0x103D, 0x1040);
    GameStateClock const * clock;
    STRUCT_PAD(0x1044, 0x1048);
    u32 daylight_band;
    u8 daylight_amount;
    STRUCT_PAD(0x104D, 0x1050);
    TerrainInfo const * farm_terrain;
    GameStateScript * script;
    SmartPtr<GameStateMapInteraction> interaction;
    SmartPtr<GameStateMapInteraction> scenery;
    bool scenery_active;
    STRUCT_PAD(0x1061, 0x1064);
    SmartPtr<GameStateMapInteraction> timed_interaction;
    GameStateVaseDisplay * vase_display;
    SmartPtr<GameStateMapInteraction> paired_scenery_first;
    SmartPtr<GameStateMapInteraction> paired_scenery_second;
    SmartPtr<GameStateMapInteraction> seasonal_scenery;
    SmartPtr<GameStateMapInteraction> alternate_seasonal_scenery;
    GameStateMapOverlay * map_overlay;
    GameStateFieldObject field_objects[4];
    GameStateVaseDisplay * transition_icons[6];
    bool day_update_requested;
    bool skip_day_update_events;
};

EC SECTION(".text.080AC96C") i32 AssignMapSceneSpriteCacheSlot(GameStateSpriteCache & inputCache, u32 slot)
{
    register GameStateSpriteCache * receivedCache asm("r0") = &inputCache;
    register u32 receivedPaletteIndex asm("r1") = slot;
    register GameStateSpriteCache * cache asm("r4");
    register u32 paletteIndex asm("r5");
    asm ("add %0, %2, #0\n\tadd %1, %3, #0" : "=l" (cache), "=l" (paletteIndex) : "l" (receivedCache), "l" (receivedPaletteIndex));
    register u32 entryOffset asm("r0") = paletteIndex * sizeof(GameStateSpriteCacheEntry);
    asm ("" : "+l" (entryOffset));
    register u8 * entryBase asm("r0") = reinterpret_cast<u8 *>(cache) + entryOffset;
    asm ("" : "+l" (entryBase));
    register i8 * resourceIndex asm("r6") = reinterpret_cast<i8 *>(entryBase);
    asm ("" : "+l" (resourceIndex));
    resourceIndex += offsetof(GameStateSpriteCache, entries) + offsetof(GameStateSpriteCacheEntry, resourceIndex);
    asm ("" : "+l" (resourceIndex));
    register unsigned invertedIndex asm("r0") = ~static_cast<u8>(*resourceIndex) << 24;
    asm ("" : "+l" (invertedIndex));
    u32 alreadyAssigned = invertedIndex >> 31;
    asm ("" : "+l" (alreadyAssigned));
    register GameStateSpriteCacheResource * resource asm("r1");
    asm volatile ("" : : "X" (&&getSlot), "X" (&&capacityReached));
    if (!alreadyAssigned)
        goto assignResource;
    {
        register i32 index asm("r2") = *resourceIndex;
        asm ("" : "+l" (index));
        register u32 resourcesOffset asm("r0") = offsetof(GameStateSpriteCache, resources);
        asm ("" : "+l" (resourcesOffset));
        register u8 * resources asm("r1") = reinterpret_cast<u8 *>(cache) + resourcesOffset;
        asm ("" : "+l" (resources));
        register i32 offset asm("r0") = index * sizeof(GameStateSpriteCacheResource);
        asm ("" : "+l" (offset));
        resource = reinterpret_cast<GameStateSpriteCacheResource *>(resources + offset);
        asm ("" : "+l" (resource));
    }
getSlot:
    return resource->spriteSlot;
assignResource:
    {
        register unsigned counterOffset asm("r0") = offsetof(GameStateSpriteCache, assignedResources);
        asm ("" : "+l" (counterOffset));
        register u8 * counter asm("r2") = reinterpret_cast<u8 *>(cache) + counterOffset;
        asm ("" : "+l" (counter));
        register u32 nextIndex asm("r3") = *counter;
        asm ("" : "+l" (nextIndex));
        u32 capacity = ARRAY_COUNT(cache->resources);
        if (nextIndex < capacity)
            goto slotAvailable;
        goto capacityReached;
    slotAvailable:
        register u32 newIndex asm("r1") = nextIndex;
        asm ("" : "+l" (newIndex));
        *counter = newIndex + 1;
        register unsigned resourcesOffset asm("r0") = offsetof(GameStateSpriteCache, resources);
        asm ("" : "+l" (resourcesOffset));
        register u8 * resources asm("r2") = reinterpret_cast<u8 *>(cache) + resourcesOffset;
        asm ("" : "+l" (resources));
        register unsigned offset asm("r0") = newIndex * sizeof(GameStateSpriteCacheResource);
        asm ("" : "+l" (offset));
        resource = reinterpret_cast<GameStateSpriteCacheResource *>(resources + offset);
        asm ("" : "+l" (resource));
        register i32 previousPalette asm("r0") = resource->paletteIndex;
        asm ("" : "+l" (previousPalette));
        if (previousPalette != static_cast<i32>(paletteIndex))
        {
            resource->paletteIndex = paletteIndex;
            resource->flags = alreadyAssigned;
        }
        register i32 previousResource asm("r0") = *resourceIndex;
        asm ("" : "+l" (previousResource));
        if (previousResource == static_cast<i32>(nextIndex))
            goto getSlot;
        *resourceIndex = nextIndex;
        goto getSlot;
    }
capacityReached:
    return -1;
}

EC void ApplyMapRow15PatchA(GameStateMapView & view);
EC void ApplyMapRow15PatchB(GameStateMapView & view);
EC void ApplyMapRow3PatchA(GameStateMapView & view);
EC void ApplyMapRow3PatchB(GameStateMapView & view);
EC void ApplyMapRow2PatchA(GameStateMapView & view);
EC void ApplyMapRow2PatchB(GameStateMapView & view);
EC void ApplyMapRow17PatchA(GameStateMapView & view);
EC void ApplyMapRow17PatchB(GameStateMapView & view);
EC void ApplyMapIndexedOverlayPatch(GameStateMapView & view, u32 index);
EC void ApplyMapIndexedFixturePatch(GameStateMapView & view, u32 index);
EC void StartMapTransitionDelay(GameStateMapView & view);
EC void ApplyMapBuildingDoorPatch(GameStateMapView & view, u32 alternate);
struct MapUpdateState;
EC void ClearSupermarketRucksackDisplay(MapUpdateState & map);
EC void ClearSupermarketBlueFeatherDisplay(MapUpdateState & map);
EC void RequestMap29Refresh(MapUpdateState & map);
EC bool IsMapTransitionIdle(GameStateMapView & view);
EC void StartMapCameraPan(GameStateMapView & view, i32 x, i32 y, u32 speed);
EC void SetMapCameraPosition(GameStateMapView & view, i32 x, i32 y);
EC void SetAlternateMapPatch(u32 & flags, u32 index);
EC void ClearAlternateMapPatch(u32 & flags, u32 index);
EC void ApplySavedMapTilePatch(MapUpdateState & map, u32 index);
EC MapData const * GetMapData(u32 map_variant);
EC u32 func_0800E324(Unk_0800E324 const & clock);
EC bool func_080A5EB8(GameStateMapView const & view, Location const & location);
EC u8 * GameScript_GetSoundPlayers(GameStateScript & script);
EC void GameScript_QueueSceneEvent(GameStateScript & script, GameStateScriptEvent const & event, i32 queue);
EC bool SaveData_CollectShippingValue(GameStateMapData & state);
EC void func_0801A13C(GameStateMapOwner & game, u32 hour, u32 minute, bool force);
EC void GameScript_ConfigureMusic(GameStateScript & script, u32 player, u32 track, GameStateMusicParams const & params);
EC SECTION(".text.080AA6D0") void ApplyFieldPlotToMap(GameStateMapView & view, u32 x, u32 y, FieldPlot const & plot, Unk_Something const * volatile adjacent)
{
    GameStateMapView * map = &view;
    asm ("" : "+l" (map));
    volatile unsigned column = x;
    register unsigned row asm("r6") = y;
    asm ("" : "+l" (row));
    FieldPlot const * volatile currentPlot = &plot;
    register unsigned width asm("sl") = GetMapData(map->map)->width;
    asm ("" : "+h" (width));
    register u8 const * groundArt asm("r5") = static_cast<u8 const *>(currentPlot->method_0800AF20());
    asm ("" : "+l" (groundArt));
    if (groundArt)
    {
        register unsigned rowOffset asm("r0") = width;
        asm ("" : "+l" (rowOffset));
        rowOffset = rowOffset * row;
        asm ("" : "+l" (rowOffset));
        register u16 * tiles asm("r1") = map->tilemaps[0];
        asm ("" : "+l" (tiles));
        rowOffset = rowOffset * sizeof(u16);
        asm ("" : "+l" (rowOffset));
        register unsigned tileX asm("r2") = column;
        asm ("" : "+l" (tileX));
        register unsigned columnOffset asm("r4") = tileX * sizeof(u16);
        asm ("" : "+l" (columnOffset));
        asm ("add %0, %1, %2" : "=l" (tiles) : "l" (columnOffset), "l" (tiles) : "cc");
        asm ("add %0, %1, %2" : "=l" (rowOffset) : "l" (rowOffset), "l" (tiles) : "cc");
        memcpy(reinterpret_cast<void *>(rowOffset), groundArt, 4);

        register unsigned followingRow asm("r0") = row + 1;
        asm ("" : "+l" (followingRow));
        register unsigned index asm("r1") = width;
        asm ("" : "+l" (index));
        index = index * followingRow;
        asm ("" : "+l" (index));
        rowOffset = index;
        asm ("" : "+l" (rowOffset));
        tiles = map->tilemaps[0];
        asm ("" : "+l" (tiles));
        rowOffset = rowOffset * sizeof(u16);
        asm ("" : "+l" (rowOffset));
        asm ("add %0, %1, %2" : "=l" (columnOffset) : "l" (columnOffset), "l" (tiles) : "cc");
        asm ("add %0, %1, %2" : "=l" (rowOffset) : "l" (rowOffset), "l" (columnOffset) : "cc");
        memcpy(reinterpret_cast<void *>(rowOffset), groundArt + 4, 4);
        register GameStateMapView * dirtyMap asm("r1") = map;
        asm ("" : "+l" (dirtyMap));
        register u8 * dirty asm("r1") = &dirtyMap->tilemapsDirty;
        asm ("" : "+l" (dirty));
        *dirty = 1;
    }

    register unsigned rowIndex asm("r5") = width;
    asm ("" : "+l" (rowIndex));
    rowIndex = rowIndex * row;
    asm ("" : "+l" (rowIndex));
    register unsigned followingRow asm("r2") = row + 1;
    asm ("" : "+l" (followingRow));
    volatile unsigned nextRow = followingRow;
    if (adjacent)
    {
        register unsigned previousRow asm("r0") = row - 1;
        asm ("" : "+l" (previousRow));
        register unsigned index asm("r1") = width;
        asm ("" : "+l" (index));
        index = index * previousRow;
        asm ("" : "+l" (index));
        register unsigned rowOffset asm("r0") = index;
        asm ("" : "+l" (rowOffset));
        register u16 * tiles asm("r1") = map->tilemaps[2];
        asm ("" : "+l" (tiles));
        rowOffset = rowOffset * sizeof(u16);
        asm ("" : "+l" (rowOffset));
        register unsigned tileX asm("r2") = column;
        asm ("" : "+l" (tileX));
        register unsigned columnOffset asm("r4") = tileX * sizeof(u16);
        asm ("" : "+l" (columnOffset));
        asm ("add %0, %1, %2" : "=l" (tiles) : "l" (columnOffset), "l" (tiles) : "cc");
        asm ("add %0, %1, %2" : "=l" (rowOffset) : "l" (rowOffset), "l" (tiles) : "cc");
        memcpy(reinterpret_cast<void *>(rowOffset), adjacent, 4);

        tiles = map->tilemaps[1];
        asm ("" : "+l" (tiles));
        rowOffset = rowIndex * sizeof(u16);
        asm ("" : "+l" (rowOffset));
        asm ("add %0, %1, %2" : "=l" (tiles) : "l" (columnOffset), "l" (tiles) : "cc");
        asm ("add %0, %1, %2" : "=l" (rowOffset) : "l" (rowOffset), "l" (tiles) : "cc");
        memcpy(reinterpret_cast<void *>(rowOffset), reinterpret_cast<u8 const *>(adjacent) + 4, 4);

        register unsigned lowerRow asm("r1") = nextRow;
        asm ("" : "+l" (lowerRow));
        rowOffset = width;
        asm ("" : "+l" (rowOffset));
        rowOffset = rowOffset * lowerRow;
        asm ("" : "+l" (rowOffset));
        tiles = map->tilemaps[1];
        asm ("" : "+l" (tiles));
        rowOffset = rowOffset * sizeof(u16);
        asm ("" : "+l" (rowOffset));
        asm ("add %0, %1, %2" : "=l" (columnOffset) : "l" (columnOffset), "l" (tiles) : "cc");
        asm ("add %0, %1, %2" : "=l" (rowOffset) : "l" (rowOffset), "l" (columnOffset) : "cc");
        memcpy(reinterpret_cast<void *>(rowOffset), reinterpret_cast<u8 const *>(adjacent) + 8, 4);
        register GameStateMapView * dirtyMap asm("r1") = map;
        asm ("" : "+l" (dirtyMap));
        register u8 * dirty asm("r1") = &dirtyMap->tilemapsDirty;
        asm ("" : "+l" (dirty));
        *dirty = 1;
    }

    register bool isSolid asm("r0") = currentPlot->method_0800A07C();
    register unsigned plotSolid asm("r0");
    asm ("" : "=l" (plotSolid) : "0" (isSolid));
    register unsigned upperIndex asm("r2") = column;
    asm ("" : "+l" (upperIndex));
    asm ("add %0, %1, %2" : "=l" (upperIndex) : "l" (upperIndex), "l" (rowIndex) : "cc");
    register unsigned upperOffset asm("r8") = upperIndex;
    asm ("" : "+h" (upperOffset));
    register u8 * terrain asm("r2") = reinterpret_cast<u8 *>(map->expanded_terrain);
    asm ("" : "+l" (terrain));
    register unsigned offset asm("r1") = upperOffset;
    asm ("" : "+l" (offset));
    offset = offset * sizeof(TerrainInfo);
    asm ("" : "+l" (offset));
    upperOffset = offset;
    asm ("" : "+h" (upperOffset));
    asm ("add %0, %1, %2" : "=l" (offset) : "l" (offset), "l" (terrain) : "cc");
    register u8 * upper asm("sb") = reinterpret_cast<u8 *>(offset);
    asm ("" : "+h" (upper));
    register unsigned lowerRow asm("r1") = nextRow;
    asm ("" : "+l" (lowerRow));
    register unsigned lowerOffset asm("r4") = width;
    asm ("" : "+l" (lowerOffset));
    lowerOffset = lowerOffset * lowerRow;
    asm ("" : "+l" (lowerOffset));
    register unsigned tileX asm("r1") = column;
    asm ("" : "+l" (tileX));
    asm ("add %0, %1, %2" : "=l" (lowerOffset) : "l" (tileX), "l" (lowerOffset) : "cc");
    lowerOffset = lowerOffset * sizeof(TerrainInfo);
    asm ("" : "+l" (lowerOffset));
    register u8 * lower asm("r2") = terrain;
    asm ("" : "+l" (lower));
    asm ("add %0, %1, %2" : "=l" (lower) : "l" (lowerOffset), "l" (lower) : "cc");
    register unsigned solidBit asm("r6") = 1;
    asm ("" : "+l" (solidBit));
    register unsigned solid asm("r5") = solidBit;
    asm ("" : "+l" (solid));
    solid = solid & plotSolid;
    asm ("" : "+l" (solid));
    register unsigned oldFlags asm("r3") = lower[4];
    asm ("" : "+l" (oldFlags));
    register unsigned clearSolid asm("r1") = ~1u;
    asm ("" : "+l" (clearSolid));
    register unsigned merged asm("r0") = clearSolid;
    asm ("" : "+l" (merged));
    merged = merged & oldFlags;
    asm ("" : "+l" (merged));
    lower[4] = merged | solid;
    oldFlags = lower[0];
    asm ("" : "+l" (oldFlags));
    merged = clearSolid;
    asm ("" : "+l" (merged));
    merged = merged & oldFlags;
    asm ("" : "+l" (merged));
    lower[0] = merged | solid;
    register u8 * upperBase asm("r0") = upper;
    asm ("" : "+l" (upperBase));
    register unsigned upperFlags asm("r2") = upperBase[4];
    asm ("" : "+l" (upperFlags));
    merged = clearSolid;
    asm ("" : "+l" (merged));
    merged = merged & upperFlags;
    asm ("" : "+l" (merged));
    merged = merged | solid;
    asm ("" : "+l" (merged));
    lower = upper;
    asm ("" : "+l" (lower));
    lower[4] = merged;
    merged = lower[0];
    asm ("" : "+l" (merged));
    clearSolid = clearSolid & merged;
    asm ("" : "+l" (clearSolid));
    lower[0] = clearSolid | solid;

    register unsigned terrainTag asm("r0") = currentPlot->method_0800A0A4();
    asm ("" : "+l" (terrainTag));
    register u8 * updatedTerrain asm("r1") = reinterpret_cast<u8 *>(map->expanded_terrain);
    asm ("" : "+l" (updatedTerrain));
    asm ("add %0, %1" : "+h" (upperOffset) : "l" (updatedTerrain));
    register unsigned rightTile asm("r2") = sizeof(TerrainInfo);
    asm ("" : "+l" (rightTile));
    asm ("add %0, %1" : "+l" (rightTile) : "h" (upperOffset));
    upper = reinterpret_cast<u8 *>(rightTile);
    asm ("" : "+h" (upper));
    asm ("add %0, %1, %2" : "=l" (lowerOffset) : "l" (lowerOffset), "l" (updatedTerrain) : "cc");
    register u8 * lowerRight asm("r3") = reinterpret_cast<u8 *>(lowerOffset) + sizeof(TerrainInfo);
    asm ("" : "+l" (lowerRight));
    register unsigned packedTag asm("r5") = terrainTag << 1;
    asm ("" : "+l" (packedTag));
    register unsigned existingTag asm("r2") = *reinterpret_cast<u16 *>(lowerRight + 2);
    asm ("" : "+l" (existingTag));
    register unsigned tagAndSolid asm("r1") = solidBit;
    asm ("" : "+l" (tagAndSolid));
    tagAndSolid = tagAndSolid & existingTag;
    asm ("" : "+l" (tagAndSolid));
    *reinterpret_cast<u16 *>(lowerRight + 2) = tagAndSolid | packedTag;
    register unsigned tagMask asm("r1") = 0x7fff;
    asm ("" : "+l" (tagMask));
    packedTag = tagMask;
    asm ("" : "+l" (packedTag));
    terrainTag = terrainTag & packedTag;
    asm ("" : "+l" (terrainTag));
    register unsigned shiftedTag asm("r3") = terrainTag << 1;
    asm ("" : "+l" (shiftedTag));
    existingTag = *reinterpret_cast<u16 *>(lowerOffset + 2);
    asm ("" : "+l" (existingTag));
    tagAndSolid = solidBit;
    asm ("" : "+l" (tagAndSolid));
    tagAndSolid = tagAndSolid & existingTag;
    asm ("" : "+l" (tagAndSolid));
    *reinterpret_cast<u16 *>(lowerOffset + 2) = tagAndSolid | shiftedTag;
    terrainTag = terrainTag & packedTag;
    asm ("" : "+l" (terrainTag));
    shiftedTag = terrainTag << 1;
    asm ("" : "+l" (shiftedTag));
    updatedTerrain = upper;
    asm ("" : "+l" (updatedTerrain));
    existingTag = *reinterpret_cast<u16 *>(updatedTerrain + 2);
    asm ("" : "+l" (existingTag));
    tagAndSolid = solidBit;
    asm ("" : "+l" (tagAndSolid));
    tagAndSolid = tagAndSolid & existingTag;
    asm ("" : "+l" (tagAndSolid));
    tagAndSolid = tagAndSolid | shiftedTag;
    asm ("" : "+l" (tagAndSolid));
    lower = upper;
    asm ("" : "+l" (lower));
    *reinterpret_cast<u16 *>(lower + 2) = tagAndSolid;
    terrainTag = terrainTag & packedTag;
    asm ("" : "+l" (terrainTag));
    terrainTag = terrainTag << 1;
    asm ("" : "+l" (terrainTag));
    lower = reinterpret_cast<u8 *>(upperOffset);
    asm ("" : "+l" (lower));
    tagAndSolid = *reinterpret_cast<u16 *>(lower + 2);
    asm ("" : "+l" (tagAndSolid));
    solidBit = solidBit & tagAndSolid;
    asm ("" : "+l" (solidBit));
    *reinterpret_cast<u16 *>(lower + 2) = solidBit | terrainTag;
}

EC void func_0805E860(SpriteAnimator & animator, u32 animation);
EC GameStateMapInteraction * InitializeAnimatedMapSpriteSinglePalette(GameStateMapInteraction * interaction,
    GameStatePackedSpriteArchive const * archive, u32 animation, GameStateMapOwner & game,
    u32 layer, u32 kind, bool start);

EC SECTION(".text.game_state_c2.08019B90") void GameStateRefreshMapDaylight(GameStateMapOwner & game)
{
    GameStateMapView & view = *game.map_view;
    void (* const * apply_daylight)(GameStateMapView &, u32, int, int) = &view.display_vtable->apply_daylight;
    (*apply_daylight)(view, game.daylight_band, game.daylight_amount, 30 - game.daylight_amount);
    if (!GetMapData(game.map_view->map)->is_interior)
        func_0801A13C(game, game.clock->hour, game.clock->minute, true);
}

EC SECTION(".text.game_state_c2.08019F38") bool GameStateIsWinterOrSpecialOutdoorTile(GameStateMapOwner const & game, Location const & location)
{
    if (!GetMapData(game.map_view->map)->is_interior &&
        (func_0800E324(reinterpret_cast<Unk_0800E324 const &>(game.map_data->clock)) == SEASON_WINTER ||
         func_080A5EB8(*game.map_view, location)))
        goto winter_or_special;
    return false;

winter_or_special:
    return true;
}

EC SECTION(".text.game_state_c2.08019F7C") u32 GameStateGetMapWidthPixels(GameStateMapOwner const & game, u32 map)
{
    GameStateMapData * const & state = game.map_data;
    u32 season = func_0800E324(reinterpret_cast<Unk_0800E324 const &>(state->clock));
    u32 house_level = state->farm.farm_house.GetUpgradeLevel();
    u32 coop_level = state->farm.coop.GetUpgradeLevel();
    u32 barn_level = state->farm.barn.GetUpgradeLevel();
    return GetMapData(GetMapGraphicsVariantId(map, season, house_level, coop_level, barn_level))->width * 8;
}

EC SECTION(".text.game_state_c2.08019FE8") u32 GameStateGetMapHeightPixels(GameStateMapOwner const & game, u32 map)
{
    GameStateMapData * const & state = game.map_data;
    u32 season = func_0800E324(reinterpret_cast<Unk_0800E324 const &>(state->clock));
    u32 house_level = state->farm.farm_house.GetUpgradeLevel();
    u32 coop_level = state->farm.coop.GetUpgradeLevel();
    u32 barn_level = state->farm.barn.GetUpgradeLevel();
    return GetMapData(GetMapGraphicsVariantId(map, season, house_level, coop_level, barn_level))->height * 8;
}

EC SECTION(".text.game_state_c2.0801A054") GameStateMapTerrain GameStateGetMapTerrain(GameStateMapOwner const & game, u32 map)
{
    if (map >= MAP_NONE)
        return GameStateMapTerrain(nullptr, nullptr, 0, 0);

    u32 current_map = game.map_view->id;
    if (map == 2 && current_map != 2)
        return GameStateMapTerrain(game.farm_terrain, 128, 88);

    u32 variant;
    if (current_map == map)
    {
        variant = game.map_view->map;
    }
    else
    {
        GameStateMapData * const & state = game.map_data;
        u32 season = func_0800E324(reinterpret_cast<Unk_0800E324 const &>(state->clock));
        u32 house_level = state->farm.farm_house.GetUpgradeLevel();
        u32 coop_level = state->farm.coop.GetUpgradeLevel();
        u32 barn_level = state->farm.barn.GetUpgradeLevel();
        variant = GetMapGraphicsVariantId(map, season, house_level, coop_level, barn_level);
    }

    MapData const * data = GetMapData(variant);
    u16 width = data->width;
    u16 height = data->height;
    TerrainInfo const * info;
    u8 const * terrain_map;
    if (current_map == map)
    {
        GameStateMapView const & view = *game.map_view;
        GetMapData(view.map);
        terrain_map = nullptr;
        info = view.expanded_terrain;
    }
    else
    {
        terrain_map = data->terrain_map;
        info = data->terrain_info;
    }
    return GameStateMapTerrain(info, terrain_map, width, height);
}

EC SECTION(".text.game_state_c2.0801A8C0") u32 GameStateGetTimeOfDayBand(GameStateMapOwner const &, u32 hour)
{
    if (hour < 6)
        return 3;
    if (hour < 12)
        return 0;
    if (hour > 17)
        return 2;
    return 1;
}

EC AEntity * InitializeScheduledNpc01(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc02(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc03(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc04(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc05(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc06(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc07(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc08(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc09(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc10(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc11(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc12(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc13(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc14(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc15(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc16(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc17(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc18(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc19(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc20(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc21(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc22(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc23(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc24(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc26(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc27(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeCarpenterNpc(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc29(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc30(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeUnscheduledNpcOne(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeUnscheduledNpcTwo(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeUnscheduledNpcThree(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc34(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);
EC AEntity * InitializeScheduledNpc25(void * storage, GameStateMapOwner & game, void * npc, u32 schedule_context);

EC AEntity * SpawnFarmer(GameStateMapOwner &, GameStateMapData *) asm("NewFarmerMapEntity");
EC AEntity * SpawnSoldier(void *, GameStateMapOwner &, u32, void *, Farm &) asm("InitializeSpriteWorkerEntity");
EC bool ShouldSpawnDog(void const *) asm("func_0800F190");
EC u32 DogPlacementState(void const *) asm("func_0800F204");
EC AEntity * InitDog(void *, GameStateMapOwner &, void *, void *) asm("DogEntity_Init");
EC AEntity * InitHorse(void *, GameStateMapOwner &, Horse *) asm("HorseEntity_Init");

EC AEntity * MakeArrivalNpc(void *, GameStateMapOwner &, void *, u32) asm("InitializeArrivalReactionNpc");
EC AEntity * MakeChicken(void *, GameStateMapOwner &, Chicken *) asm("ChickenEntity_Init");
EC AEntity * MakeCow(void *, GameStateMapOwner &, Cow *) asm("CowEntity_Init");
EC AEntity * MakeSheep(void *, GameStateMapOwner &, Sheep *) asm("SheepEntity_Init");
EC AEntity * MakeEgg(void *, GameStateMapOwner &, Location const &) asm("InitializeReactionEntity");
EC AEntity * MakeTimedReaction(void *, GameStateMapOwner &, void *) asm("InitializeTimedReactionEntity");
EC AEntity * MakePickupSprite(void *, GameStateMapOwner &, void *) asm("InitializeLaunchedSideSprite");
EC AEntity * MakeSpecialFieldActor(void *, GameStateMapOwner &, void *) asm("InitializeActivityNpcEntity");
EC AEntity * MakeActiveSceneEffect(GameStateMapOwner &, void *) asm("CreateActiveSideEffectEntity");
EC AEntity * MakeUpdateEffect(void *, GameStateMapOwner &, void *) asm("InitializeSideEffectEntity");
EC AEntity * MakeRecordedActor(void *, GameStateMapOwner &, void *) asm("InitializeRecordedSideActor");
EC void * FindArrivalNpc(void *) asm("GetActiveScheduleNpc");
EC void * FindDayNpc(void *, u32) asm("GetNpcScheduleDaySlot");
EC void * FindNightNpc(void *, u32) asm("GetNpcScheduleNightSlot");
EC void * FindCurrentNpc(void *) asm("GetNpcScheduleCurrentSlot");
EC void * FindPreviousNpc(void *) asm("GetNpcSchedulePreviousSlot");
EC AEntity * MakeDayNpc(GameStateMapOwner &, void *, u32) asm("CreateDirectedMovingNpcTwo");
EC AEntity * MakeNightNpc(GameStateMapOwner &, void *, u32) asm("CreateDirectedMovingNpcOne");
EC AEntity * MakeCurrentNpc(GameStateMapOwner &, void *) asm("CreateMovingNpcVariantSeven");
EC AEntity * MakePreviousNpc(GameStateMapOwner &, void *) asm("CreateMovingNpcVariantSix");
EC u32 ChickenPlacementIndex(void const *) asm("func_0800F344");
EC u8 HeldArticleByte(void const *) asm("func_0800F258");
EC u32 GetHeldArticleId(u8 const *) asm("GetId__C7Article");
EC bool CanSpawnTimedReaction(void const *) asm("func_0800E934");
EC bool CanSpawnPickupSprite(void const *) asm("func_0800E940");
EC void * FindHouseShelf(FarmHouse &) asm("GetShelf__9FarmHouse");
EC u32 FindShelfArticle(void const *, u32) asm("GetFirstSlotWith__C5ShelfUi");
EC u32 FindRucksackArticle(void const *, u32) asm("GetFirstSlotWithArticle__C8RucksackUi");
struct GameStateGlobal;
extern GameStateGlobal * gActiveGameScene;

EC SECTION(".text.game_state_c2.0801A8E0") void GameStateRespawnActor(GameStateMapOwner & owner, u32 entity_slot)
{
    register GameStateMapOwner & game = owner;
    register u32 slot = entity_slot;
    AEntity ** entries = owner.entities;
    AEntity * & entry = entries[entity_slot];
    if (entry)
    {
        delete entry;
        entry = nullptr;
    }
    register AEntity * actor asm("r5") = nullptr;
    switch (slot)
    {
        case 0:
            actor = SpawnFarmer(game, game.map_data);
            break;
        case 43:
        {
            register GameStateMapData ** map = &game.map_data;
            register u8 * state = reinterpret_cast<u8 *>(*map);
            if (state[0x219C] & 0x10)
                break;
            register u8 * placement = state + 0x1C2C;
            if (!ShouldSpawnDog(placement) && DogPlacementState(placement) == 2)
                break;
            void * storage = ::operator new(0x44);
            u8 * current_state = reinterpret_cast<u8 *>(*map);
            actor = InitDog(storage, game, current_state + 0x1C70, current_state + 0x1CCC);
            break;
        }
        case 44:
            if (!(reinterpret_cast<u8 *>(game.map_data)[0x219A] & 2) && !(reinterpret_cast<u8 *>(game.map_data)[0x21A1] & 4))
                if (Horse * horse = game.map_data->farm.GetHorse())
                    actor = InitHorse(::operator new(0x40), game, horse);
            break;
        case 46: case 47: case 48: case 49: case 50: case 51: case 52: case 53:
        {
            u32 index = slot - 46;
            void * placement = reinterpret_cast<u8 *>(game.map_data) + 0x1C2C;
            if (!ShouldSpawnDog(placement) && DogPlacementState(placement) == 3 && ChickenPlacementIndex(placement) == index)
                break;
            if (index == game.map_data->farm.coop.GetUnkEnt() || index >= game.map_data->farm.coop.GetCapacity())
                break;
            if (Chicken * chicken = game.map_data->farm.coop.GetChicken(index))
                actor = MakeChicken(::operator new(0x3C), game, chicken);
            break;
        }
        case 54: case 55: case 56: case 57: case 58: case 59: case 60: case 61:
        case 62: case 63: case 64: case 65: case 66: case 67: case 68: case 69:
        {
            register u32 index = slot - 54;
            register u32 sheep_index asm("r9") = index;
            asm("" : : "r"(sheep_index));
            register GameStateMapData ** map = &game.map_data;
            if (index == (*map)->farm.barn.method_0800D058() || index >= (*map)->farm.barn.GetCapacity())
                break;
            register Cow * cow = (*map)->farm.barn.GetCow(index);
            if (cow)
                actor = MakeCow(::operator new(0x3C), game, cow);
            else if (Sheep * sheep = (*map)->farm.barn.GetSheep(sheep_index))
                actor = MakeSheep(::operator new(0x3C), game, sheep);
            break;
        }
        case 76: case 77: case 78: case 79: case 80: case 81: case 82: case 83:
        {
            u32 index = slot - 76;
            if (index < game.map_data->farm.coop.GetCapacity())
                if (Coop::Egg const * egg = game.map_data->farm.coop.GetEgg(index))
                    actor = MakeEgg(::operator new(0x18), game, egg->location);
            break;
        }
        case 75:
        {
            if (!CanSpawnPickupSprite(reinterpret_cast<u8 *>(game.map_data) + 0x1BD8))
                break;
            void * placement = reinterpret_cast<u8 *>(game.map_data) + 0x1C2C;
            if (!ShouldSpawnDog(placement) && DogPlacementState(placement) == 1)
            {
                u8 held_article = HeldArticleByte(placement);
                if (GetHeldArticleId(&held_article) == 0x35)
                    break;
            }
            GameStateMapData ** state = &game.map_data;
            register u32 found = FindRucksackArticle(reinterpret_cast<u8 *>(*state) + 0x1C38, 0x35);
            if (found != static_cast<u32>(-1))
                break;
            void * shelf = FindHouseShelf((*state)->farm.farm_house);
            if (shelf && FindShelfArticle(shelf, 0x35) != found)
                break;
            void * storage = ::operator new(0x2C);
            actor = MakePickupSprite(storage, game, reinterpret_cast<u8 *>(*state) + 0x1CCC);
            break;
        }
        case 74:
        {
            if (!CanSpawnTimedReaction(reinterpret_cast<u8 *>(game.map_data) + 0x1BD8))
                break;
            void * placement = reinterpret_cast<u8 *>(game.map_data) + 0x1C2C;
            if (!ShouldSpawnDog(placement) && DogPlacementState(placement) == 4)
                break;
            actor = MakeTimedReaction(::operator new(0x1C), game, reinterpret_cast<u8 *>(game.map_data) + 0x1CA0);
            break;
        }
        case 36:
            actor = SpawnSoldier(::operator new(0x44), game, 0, reinterpret_cast<u8 *>(game.map_data) + 0x1CD4, game.map_data->farm);
            break;
        case 37:
            actor = SpawnSoldier(::operator new(0x44), game, 1, reinterpret_cast<u8 *>(game.map_data) + 0x1CD4, game.map_data->farm);
            break;
        case 38:
            actor = SpawnSoldier(::operator new(0x44), game, 2, reinterpret_cast<u8 *>(game.map_data) + 0x1CD4, game.map_data->farm);
            break;
        case 39:
            actor = SpawnSoldier(::operator new(0x44), game, 3, reinterpret_cast<u8 *>(game.map_data) + 0x1CD4, game.map_data->farm);
            break;
        case 40:
            actor = SpawnSoldier(::operator new(0x44), game, 4, reinterpret_cast<u8 *>(game.map_data) + 0x1CD4, game.map_data->farm);
            break;
        case 41:
            actor = SpawnSoldier(::operator new(0x44), game, 5, reinterpret_cast<u8 *>(game.map_data) + 0x1CD4, game.map_data->farm);
            break;
        case 42:
            actor = SpawnSoldier(::operator new(0x44), game, 6, reinterpret_cast<u8 *>(game.map_data) + 0x1CD4, game.map_data->farm);
            break;
        case 1:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc01(storage, game, state + 0x1D44, reinterpret_cast<u32>(state));
            break;
        }
        case 2:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc02(storage, game, state + 0x1D58, reinterpret_cast<u32>(state));
            break;
        }
        case 3:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc03(storage, game, state + 0x1D6C, reinterpret_cast<u32>(state));
            break;
        }
        case 4:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc04(storage, game, state + 0x1D84, reinterpret_cast<u32>(state));
            break;
        }
        case 5:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc05(storage, game, state + 0x1D98, reinterpret_cast<u32>(state));
            break;
        }
        case 6:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc06(storage, game, state + 0x1DAC, reinterpret_cast<u32>(state));
            break;
        }
        case 7:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc07(storage, game, state + 0x1DC4, reinterpret_cast<u32>(state));
            break;
        }
        case 8:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc08(storage, game, state + 0x1DD8, reinterpret_cast<u32>(state));
            break;
        }
        case 9:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc09(storage, game, state + 0x1DEC, reinterpret_cast<u32>(state));
            break;
        }
        case 10:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc10(storage, game, state + 0x1E00, reinterpret_cast<u32>(state));
            break;
        }
        case 11:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc11(storage, game, state + 0x1E14, reinterpret_cast<u32>(state));
            break;
        }
        case 12:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc12(storage, game, state + 0x1E28, reinterpret_cast<u32>(state));
            break;
        }
        case 13:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc13(storage, game, state + 0x1E40, reinterpret_cast<u32>(state));
            break;
        }
        case 14:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc14(storage, game, state + 0x1E54, reinterpret_cast<u32>(state));
            break;
        }
        case 15:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc15(storage, game, state + 0x1E68, reinterpret_cast<u32>(state));
            break;
        }
        case 16:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc16(storage, game, state + 0x1E7C, reinterpret_cast<u32>(state));
            break;
        }
        case 17:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc17(storage, game, state + 0x1E90, reinterpret_cast<u32>(state));
            break;
        }
        case 18:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc18(storage, game, state + 0x1EA4, reinterpret_cast<u32>(state));
            break;
        }
        case 19:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc19(storage, game, state + 0x1EB8, reinterpret_cast<u32>(state));
            break;
        }
        case 20:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc20(storage, game, state + 0x1ED0, reinterpret_cast<u32>(state));
            break;
        }
        case 21:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc21(storage, game, state + 0x1EE4, reinterpret_cast<u32>(state));
            break;
        }
        case 22:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc22(storage, game, state + 0x1EFC, reinterpret_cast<u32>(state));
            break;
        }
        case 23:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc23(storage, game, state + 0x1F10, reinterpret_cast<u32>(state));
            break;
        }
        case 24:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc24(storage, game, state + 0x1F24, reinterpret_cast<u32>(state));
            break;
        }
        case 25:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc26(storage, game, state + 0x1F38, reinterpret_cast<u32>(state));
            break;
        }
        case 26:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc27(storage, game, state + 0x1F50, reinterpret_cast<u32>(state));
            break;
        }
        case 27:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeCarpenterNpc(storage, game, state + 0x1F64, reinterpret_cast<u32>(state));
            break;
        }
        case 28:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc29(storage, game, state + 0x1F7C, reinterpret_cast<u32>(state));
            break;
        }
        case 29:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc30(storage, game, state + 0x1F90, reinterpret_cast<u32>(state));
            break;
        }
        case 30:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeUnscheduledNpcOne(storage, game, state + 0x1FA4, reinterpret_cast<u32>(state));
            break;
        }
        case 31:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeUnscheduledNpcTwo(storage, game, state + 0x1FB8, reinterpret_cast<u32>(state));
            break;
        }
        case 32:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeUnscheduledNpcThree(storage, game, state + 0x1FD0, reinterpret_cast<u32>(state));
            break;
        }
        case 33:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc34(storage, game, state + 0x1FE4, reinterpret_cast<u32>(state));
            break;
        }
        case 34:
        {
            void * storage = ::operator new(72);
            register u8 * state asm("r3") = reinterpret_cast<u8 *>(game.map_data);
            actor = InitializeScheduledNpc25(storage, game, state + 0x1FFC, reinterpret_cast<u32>(state));
            break;
        }

        case 35:
        {
            void * npc = FindArrivalNpc(reinterpret_cast<u8 *>(game.map_data) + 0x1CD4);
            if (npc)
                actor = MakeArrivalNpc(::operator new(0x4C), game, npc, reinterpret_cast<u32>(game.map_data));
            break;
        }
        case 84:
            actor = MakeActiveSceneEffect(game, reinterpret_cast<u8 *>(game.map_data) + 0x3494);
            break;
        case 45:
            actor = MakeSpecialFieldActor(::operator new(0x54), game, reinterpret_cast<u8 *>(game.map_data) + 0x34DC);
            break;
        case 70: case 71: case 72: case 73:
        {
            register u32 index asm("r5") = slot - 70;
            asm("" : : "l"(index));
            u8 * records = reinterpret_cast<u8 *>(gActiveGameScene) + 0x524;
            actor = MakeRecordedActor(::operator new(0x34), game, records + index * 12);
            break;
        }
        case 85: case 86: case 87:
        {
            u32 index = slot - 85;
            actor = MakeDayNpc(game, FindDayNpc(reinterpret_cast<u8 *>(game.map_data) + 0x1CD4, index), index);
            break;
        }
        case 88: case 89: case 90:
        {
            u32 index = slot - 88;
            actor = MakeNightNpc(game, FindNightNpc(reinterpret_cast<u8 *>(game.map_data) + 0x1CD4, index), index);
            break;
        }
        case 91:
            actor = MakeCurrentNpc(game, FindCurrentNpc(reinterpret_cast<u8 *>(game.map_data) + 0x1CD4));
            break;
        case 92:
            actor = MakePreviousNpc(game, FindPreviousNpc(reinterpret_cast<u8 *>(game.map_data) + 0x1CD4));
            break;
        case 93:
            actor = MakeUpdateEffect(::operator new(0x1C), game, reinterpret_cast<u8 *>(game.map_data) + 0x214C);
            break;
    }
    asm("" : : "l"(actor));
    if (actor)
    {
        register AEntity ** entries asm("r0") = game.entities;
        register u32 offset asm("r1") = slot * sizeof(*entries);
        asm("" : "=l"(entries) : "0"(entries));
        *reinterpret_cast<AEntity **>(reinterpret_cast<u8 *>(entries) + offset) = actor;
        actor->vfunc_10();
    }
}

EC SECTION(".text.game_state_c2.0801C0E0") void GameStateRequestDayUpdate(GameStateMapOwner & game, bool skip_events)
{
    game.day_update_requested = true;
    game.skip_day_update_events = skip_events;
}

EC SECTION(".text.game_state_c2.0801C3DC") void GameStateNoOpMapAction(GameStateMapOwner &)
{
}

typedef Field<Farm::FIELD_WIDTH, Farm::FIELD_HEIGHT> GameStateFarmField;

static inline FieldPlot * GetFieldNeighbor(GameStateFarmField & field, int x, int y)
{
    bool in_bounds = x >= 0 && y >= 0 && x < Farm::FIELD_WIDTH && y < Farm::FIELD_HEIGHT;
    return in_bounds ? &field.PlotAt(x, y) : nullptr;
}

EC void GameScript_PlaySound(GameStateScript & script, i32 mode, u32 song_id);

static inline u32 TryHitFieldObject(GameStateMapOwner & game, int tile_x, int tile_y)
{
    for (u32 i = 0; i < 4; ++i)
    {
        GameStateFieldObject & object = game.field_objects[i];
        u32 kind = object.kind;
        if (kind == 1 && tile_x == object.tile_x && tile_y == object.tile_y)
        {
            u32 inactive_action = 2;
            if (object.action != inactive_action)
            {
                GameScript_PlaySound(*game.script, 0, 0xC0);
                object.action = inactive_action;
                GameStateMapInteraction * interaction = object.interaction;
                func_0805E860(interaction->animator, 0x873);
                interaction->playing = kind;
                interaction->ended = false;
                interaction->restart = kind;
                func_0800EEF8(game.map_data->farmer);
            }
            return true;
        }
    }
    register u32 missed asm("r0") = 0;
    asm("" : "=l"(missed) : "0"(missed));
    return missed;
}

EC SECTION(".text.game_state_c2.0801C6EC") u32 GameStateHitFieldPlot(GameStateMapOwner & game, int x, int y, int strength)
{
    u32 hit = 0;
    if (game.vtable->get_map_id(game) == 2)
    {
        int tile_x = x * 2 + 34;
        int tile_y = y * 2 + 22;
        if (!TryHitFieldObject(game, tile_x, tile_y))
        {
            FieldPlot & plot = game.map_data->farm.field.PlotAt(x, y);
            hit = plot.method_0800A238(strength);
            GameStateMapView * view = game.map_view;
            GameStateFarmField & field = game.map_data->farm.field;
            FieldPlot & current = field.PlotAt(x, y);
            FieldPlot * south = GetFieldNeighbor(field, x, y + 1);
            FieldPlot * north = GetFieldNeighbor(field, x, y - 1);
            Unk_Something const * adjacent = current.method_0800AF5C(south, north);
            ApplyFieldPlotToMap(*view, tile_x, tile_y, plot, adjacent);
        }
    }
    return hit;
}

EC SECTION(".text.game_state_c2.0801CCE4") u32 GameStateDamageFieldDebris(GameStateMapOwner & game, int x, int y, int strength)
{
    GameStateMapData * & state = game.map_data;
    FieldPlot & plot = state->farm.field.PlotAt(x, y);
    u32 result = plot.method_0800A33C(strength);
    if (game.vtable->get_map_id(game) == 2)
    {
        GameStateMapView * view = game.map_view;
        GameStateFarmField & field = state->farm.field;
        FieldPlot & current = field.PlotAt(x, y);
        FieldPlot * south = GetFieldNeighbor(field, x, y + 1);
        FieldPlot * north = GetFieldNeighbor(field, x, y - 1);
        Unk_Something const * adjacent = current.method_0800AF5C(south, north);
        ApplyFieldPlotToMap(*view, x * 2 + 34, y * 2 + 22, plot, adjacent);
    }
    return result;
}

EC void QueueDebrisEffect(MapTileEffectQueue *, int, int, u32) asm("ScheduleMapTileEffect");
EC int GetFieldDebrisQuadrant(FieldPlot const &) asm("GameStateVariant_GetColumn");

EC SECTION(".text.game_state_c2.0801C874") u32 GameStateStrikeFieldPlots(
    GameStateMapOwner & game, int column, int row, int tool_strength)
{
    register int strength asm("sl") = tool_strength;
    register u32 data_offset asm("r3") = 0x1038;
    asm("" : "+l"(data_offset) : "h"(strength));
    register GameStateMapData ** data_slot asm("r0") = reinterpret_cast<GameStateMapData **>(reinterpret_cast<u8 *>(&game) + data_offset);
    register GameStateMapData * state asm("r3") = *data_slot;
    register u32 field_offset asm("r0") = 0x9DC;
    asm("" : "+l"(field_offset) : "l"(state));
    register u8 * field asm("r3") = reinterpret_cast<u8 *>(state) + field_offset;
    register int index asm("r0") = row * Farm::FIELD_WIDTH;
    index = column + index;
    register FieldPlot & plot asm("r4") = *reinterpret_cast<FieldPlot *>(field + index * sizeof(FieldPlot));
    register int x = column;
    register int y asm("r8") = row;
    register int result asm("r9");
    if (plot.method_0800A78C())
    {
        register int quadrant asm("r0") = GetFieldDebrisQuadrant(plot);
        if (quadrant == 1)
            goto shift_left;
        asm("" : "+l"(quadrant));
        if (quadrant <= 1)
            goto top_left;
        if (quadrant == 2)
            goto shift_up;
        if (quadrant == 3)
            goto shift_up_and_left;
        goto top_left;
    shift_left:
        --x;
        goto top_left;
    shift_up:
        {
            register int back asm("r1") = -1;
            asm("" : "+l"(back));
            y += back;
        }
        goto top_left;
    shift_up_and_left:
        --x;
        {
            register int back asm("r2") = -1;
            asm("" : "+l"(back));
            y += back;
        }
    top_left:
        result = GameStateHitFieldPlot(game, x, y, strength);
        register int east asm("r5") = x + 1;
        GameStateHitFieldPlot(game, east, y, strength);
        register int south asm("r4") = y + 1;
        GameStateHitFieldPlot(game, x, south, strength);
        GameStateHitFieldPlot(game, east, south, strength);
    }
    else
        result = GameStateHitFieldPlot(game, x, y, strength);
    if (game.vtable->get_map_id(game) == 2)
    {
        register u32 effect asm("r3") = result;
        asm("" : : "l"(effect));
        switch (effect)
        {
        case 0: case 1: case 2: case 6: break;
        case 3:
            QueueDebrisEffect(game.tile_effects, x * 16 + 280, y * 16 + 192, 3);
            break;
        case 4:
            QueueDebrisEffect(game.tile_effects, x * 16 + 280, y * 16 + 192, 1);
            break;
        case 5:
            QueueDebrisEffect(game.tile_effects, x * 16 + 280, y * 16 + 192, 2);
            break;
        case 7:
            QueueDebrisEffect(game.tile_effects, x * 16 + 288, y * 16 + 208, 8);
            break;
        case 8:
            QueueDebrisEffect(game.tile_effects, x * 16 + 288, y * 16 + 208, 6);
            break;
        }
        asm("" : : "l"(effect));
    }
    return 0;
}

EC SECTION(".text.game_state_c2.0801CDBC") u32 GameStateStrikeFieldDebris(GameStateMapOwner & game, int column, int row, int tool_strength)
{
    register int strength asm("sl") = tool_strength;
    register u32 data_offset asm("r3") = 0x1038;
    asm("" : "+l"(data_offset) : "h"(strength));
    register GameStateMapData ** data_slot asm("r0") = reinterpret_cast<GameStateMapData **>(reinterpret_cast<u8 *>(&game) + data_offset);
    register GameStateMapData * state asm("r3") = *data_slot;
    register u32 field_offset asm("r0") = 0x9DC;
    asm("" : "+l"(field_offset) : "l"(state));
    register u8 * field asm("r3") = reinterpret_cast<u8 *>(state) + field_offset;
    register int index asm("r0") = row * Farm::FIELD_WIDTH;
    index = column + index;
    register FieldPlot & plot asm("r4") = *reinterpret_cast<FieldPlot *>(field + index * sizeof(FieldPlot));
    register int x = column;
    register int y asm("r8") = row;
    register int result asm("r9");
    if (plot.method_0800A78C())
    {
        register int quadrant asm("r0") = GetFieldDebrisQuadrant(plot);
        if (quadrant == 1)
            goto shift_left;
        asm("" : "+l"(quadrant));
        if (quadrant <= 1)
            goto top_left;
        if (quadrant == 2)
            goto shift_up;
        if (quadrant == 3)
            goto shift_up_and_left;
        goto top_left;
    shift_left:
        --x;
        goto top_left;
    shift_up:
        {
            register int back asm("r1") = -1;
            asm("" : "+l"(back));
            y += back;
        }
        goto top_left;
    shift_up_and_left:
        --x;
        {
            register int back asm("r2") = -1;
            asm("" : "+l"(back));
            y += back;
        }
    top_left:
        result = GameStateDamageFieldDebris(game, x, y, strength);
        register int east asm("r5") = x + 1;
        GameStateDamageFieldDebris(game, east, y, strength);
        register int south asm("r4") = y + 1;
        GameStateDamageFieldDebris(game, x, south, strength);
        GameStateDamageFieldDebris(game, east, south, strength);
    }
    else
        result = GameStateDamageFieldDebris(game, x, y, strength);
    if (game.vtable->get_map_id(game) == 2)
    {
        register int effect asm("r3") = result;
        asm("" : "+l"(effect));
        if (effect == 1)
            goto small_debris;
        asm("" : "+l"(effect));
        if (effect <= 1)
            goto done;
        if (effect == 2)
            goto stump;
        goto done;
    small_debris:
        game.map_data->farm.method_08009B68(1);
        QueueDebrisEffect(game.tile_effects, x * 16 + 280, y * 16 + 187, 4);
        goto done;
    stump:
        game.map_data->farm.method_08009B68(6);
        QueueDebrisEffect(game.tile_effects, x * 16 + 288, y * 16 + 208, 7);
    }
done:
    return 0;
}

extern Vec2 const gUnk_080F0704[], gUnk_080F0718[], gUnk_080F0730[];
EC bool DoesEntityCollisionRectangleHitSolidTile(GameStateMapTerrain const &, Box const &);

EC SECTION(".text.game_state_c2.0801E67C") void GameStateRelocateObstructedFarmAnimals(GameStateMapOwner & owner)
{
    register GameStateMapOwner * game = &owner;
    const u32 animal_map = 29;
    register u32 position_count asm("sl");
    struct Scratch
    {
        struct Terrain
        {
            TerrainInfo const * info;
            u8 const * map;
            u16 width, height;
        } terrain;
        u32 rectangle[sizeof(Box) / sizeof(u32)];
        Vec2 const * positions;
    } scratch;
    asm("" : "+l"(game) : : "r4", "r5", "r6");
    register int map asm("r0") = game->map_view->map;
    switch (map)
    {
        case 41: scratch.positions = gUnk_080F0704; position_count = 5; break;
        case 42: scratch.positions = gUnk_080F0718; position_count = 6; break;
        case 43: scratch.positions = gUnk_080F0730; position_count = 6; break;
        default: return;
    }
    MapData const * data = GetMapData(map);
    register u16 width asm("r5") = data->width;
    register u16 height asm("r6") = data->height;
    register GameStateMapView & view asm("r4") = *game->map_view;
    GetMapData(view.map);
    register u32 empty asm("r2") = 0;
    asm("" : "+l"(empty));
    register TerrainInfo const * info asm("r0") = view.expanded_terrain;
    register Scratch::Terrain * area asm("r1") = &scratch.terrain;
    asm("" : "+l"(area), "+l"(info) : "l"(empty));
    scratch.terrain.info = info;
    scratch.terrain.map = reinterpret_cast<u8 *>(empty);
    area->width = width;
    area->height = height;
    register int slot asm("r8") = empty;
    do
    {
        register int current_slot asm("r6") = slot;
        register AEntity * entity asm("r5") = game->vtable->get_entity(*game, slot);
        if (!entity)
            continue;
        register u32 obstructed asm("r9") = 0;
        switch (slot)
        {
        case 43:
        case 46: case 47: case 48: case 49: case 50: case 51: case 52: case 53:
        case 74: case 75: case 76: case 77: case 78: case 79: case 80: case 81: case 82: case 83:
            if (entity->GetMap() == animal_map)
            {
                struct CollisionSlots
                {
                    STRUCT_PAD(0, 0xC);
                    void (*get_box)(Box &, AEntity const &);
                };
                register CollisionSlots const * slots asm("r1") = *reinterpret_cast<CollisionSlots const *const *>(reinterpret_cast<u8 *>(entity) + 0x14);
                asm("" : : "l"(slots));
                register Box * box_storage asm("r4") = reinterpret_cast<Box *>(scratch.rectangle);
                register Box * box_out asm("r0") = box_storage;
                slots->get_box(*box_out, *entity);
                Box const & box = *reinterpret_cast<Box *>(scratch.rectangle);
                if (DoesEntityCollisionRectangleHitSolidTile(reinterpret_cast<GameStateMapTerrain const &>(scratch.terrain), box))
                    obstructed = 1;
            }
            break;
        }
        if (!obstructed)
            continue;
        switch (current_slot)
        {
        case 43: case 46: case 47: case 48: case 49: case 50: case 51: case 52: case 53: case 74: case 75:
            goto relocate;
        default:
        relocate:
            if (entity)
            {
                register int offset asm("r0") = (static_cast<u32>(rand()) % position_count) * sizeof(Vec2);
                register u8 const * table asm("r1") = reinterpret_cast<u8 const *>(scratch.positions);
                asm("" : "+l"(table), "+l"(offset));
                register Vec2 const * pos_ptr asm("r0") = reinterpret_cast<Vec2 const *>(table + offset);
                asm("" : "+l"(pos_ptr));
                Vec2 const & pos = *pos_ptr;
                entity->SetLocation(animal_map, pos.x, pos.y);
            }
            break;
        case 76: case 77: case 78: case 79: case 80: case 81: case 82: case 83:
            if (entity)
            {
                game->vtable->remove_entity(*game, current_slot);
                register u32 map_data_offset asm("r1") = offsetof(GameStateMapOwner, map_data);
                asm("" : "+l"(map_data_offset) : "l"(game));
                register GameStateMapData ** state_pointer asm("r0") = reinterpret_cast<GameStateMapData **>(reinterpret_cast<u8 *>(game) + map_data_offset);
                asm("" : "+l"(state_pointer));
                register GameStateMapData * data asm("r0") = *state_pointer;
                data->farm.coop.method_0800C9B8(current_slot - 76);
            }
            break;
        }
    } while (++slot < 100);
}

EC void RefreshOutsideFarmTerrain(TerrainInfo *, Farm &, bool) asm("GameStateMap_BuildFarmTerrain");

EC SECTION(".text.game_state_c2.0801E90C") void GameStateClearFieldPlotsUnderActors(GameStateMapOwner & owner)
{
    register GameStateMapOwner & game asm("r8") = owner;
    u32 changed = false;
    for (int i = 0; i < 100; ++i)
    {
        register GameStateMapOwner * map_owner asm("r1") = &game;
        register GameStateEntitySlotVtable const * slots asm("r0") = map_owner->vtable;
        AEntity * actor = slots->get_entity(game, i);
        if (!actor || actor->GetMap() != 2)
            continue;
        Box box = actor->GetBox();
        Box limits = box;
        register Box * bounds asm("r4") = &limits;
        asm("" : "+l"(bounds));
        register Box * lower_bounds asm("r6") = bounds;
        int left = std::max(0, ((bounds->x1 >> 3) - 34) >> 1);
        int right = std::min<int>(Farm::FIELD_WIDTH - 1, (((bounds->x2 - 1) >> 3) - 34) >> 1);
        int top = std::max(0, ((bounds->y1 >> 3) - 22) >> 1);
        int bottom = std::min<int>(Farm::FIELD_HEIGHT - 1, (((lower_bounds->y2 - 1) >> 3) - 22) >> 1);
        for (int y = top; y <= bottom; ++y)
        {
            for (int x = left; x <= right; ++x)
            {
                FieldPlot & plot = game.map_data->farm.field.PlotAt(x, y);
                if (plot.GetUnk2() == 21)
                {
                    plot.method_0800A4A4();
                    changed = true;
                }
            }
        }
    }
    if (changed)
    {
        register GameStateMapOwner * map_owner asm("r2") = &game;
        asm("" : "+l"(map_owner));
        register GameStateMapView & view asm("r5") = *map_owner->map_view;
        if (view.id == 2)
        {
            GameStateMapDisplayVtable const * display = view.display_vtable;
            register u32 band asm("r1") = game.daylight_band;
            register int amount asm("r2") = game.daylight_amount;
            display->apply_daylight(view, band, amount, 30 - amount);
        }
        else
        {
            TerrainInfo * terrain = const_cast<TerrainInfo *>(game.farm_terrain);
            GameStateMapData * state = game.map_data;
            Farm & farm = state->farm;
            u32 season = func_0800E324(reinterpret_cast<Unk_0800E324 const &>(state->clock));
            RefreshOutsideFarmTerrain(terrain, farm, season == SEASON_WINTER);
        }
    }
}

EC SECTION(".text.game_state_c2.0801C0F8") bool GameStateFindFarmPlot(
    GameStateMapOwner & game, Location const & location, GameStateFarmPlot & result)
{
    bool found = false;
    if (location.GetMap() == 2)
    {
        u32 origin_x = 34;
        u32 origin_y = 22;
        u32 tile_x = location.GetX() / 8;
        u32 tile_y = location.GetY() / 8;
        if (tile_x < origin_x || tile_y < origin_y || tile_x > 119 || tile_y > 71)
            goto done;

        u32 plot_x = (tile_x - origin_x) / 2;
        u32 plot_y = (tile_y - origin_y) / 2;
        result.plot = &game.map_data->farm.field.PlotAt(plot_x, plot_y);
        result.tile_x = plot_x * 2 + origin_x;
        result.tile_y = plot_y * 2 + origin_y;
        result.plot_x = plot_x;
        result.plot_y = plot_y;
        found = true;
    }
done:
    return found;
}

static inline FieldPlot * GetHoeFieldPlot(FieldPlot * field, int x, int row)
{
    register int index asm("r0") = x + row * Farm::FIELD_WIDTH;
    asm("" : "=l"(index) : "0"(index));
    return field + index;
}

EC SECTION(".text.game_state_c2.0801C29C") u32 GameStateHoeFarmPlot(GameStateMapOwner & game, Location const & location)
{
    u32 result = 0;
    GameStateFarmPlot plot;
    if (GameStateFindFarmPlot(game, location, plot))
    {
        FieldPlot * farm_plot = plot.plot;
        bool default_layout = game.map_data->farm_layout == 0;
        result = farm_plot->method_0800A190(default_layout);
        if (game.vtable->get_map_id(game) == ((u32(*reinterpret_cast<u16 const *>(&location) << 22)) >> 22))
        {
            GameStateMapView * view = game.map_view;
            register GameStateFarmPlot * coords = &plot;
            register GameStateFarmPlot * row_coords = coords;
            FieldPlot * field = game.map_data->farm.field.plots;
            register int x asm("r3") = coords->plot_x;
            register int y asm("r2") = row_coords->plot_y;
            asm("" : : "l"(x), "l"(y));
            FieldPlot * current = GetHoeFieldPlot(field, x, y);
            int south_y = y + 1;
            bool south_valid = x >= 0 && south_y >= 0 && x < Farm::FIELD_WIDTH && south_y < Farm::FIELD_HEIGHT;
            FieldPlot * south = south_valid ? GetHoeFieldPlot(field, x, south_y) : nullptr;
            int north_y = y - 1;
            bool north_valid = x >= 0 && north_y >= 0 && x < Farm::FIELD_WIDTH && north_y < Farm::FIELD_HEIGHT;
            FieldPlot * north = north_valid ? GetHoeFieldPlot(field, x, north_y) : nullptr;
            Unk_Something const * adjacent = current->method_0800AF5C(south, north);
            ApplyFieldPlotToMap(*view, plot.tile_x, plot.tile_y, *plot.plot, adjacent);
        }
        if (result == 3)
        {
            for (u32 i = 0; i < 4; ++i)
            {
                if (game.field_objects[i].kind == 0)
                {
                    game.field_objects[i].kind = 2;
                    game.field_objects[i].tile_x = plot.tile_x;
                    game.field_objects[i].tile_y = plot.tile_y;
                    break;
                }
            }
        }
    }
    return result;
}

EC SECTION(".text.game_state_c2.0801C198") bool GameStateFindMinePlot(
    GameStateMapOwner & game, Location const & location, GameStateMinePlot & result)
{
    bool found = false;
    if (location.GetMap() - 0x34 <= 0x1FF)
    {
        MineFloor const & floor = *reinterpret_cast<MineFloor const *>(reinterpret_cast<u8 const *>(game.map_data) + 0x2E58);
        u32 tile_x = location.GetX() / 8;
        u32 tile_y = location.GetY() / 8;
        register u32 left asm("r8");
        switch (GetMineFloorKind(floor))
        {
            case 1:
            {
                register u32 origin_x asm("r1") = 2;
                asm("" : "=l"(origin_x) : "0"(origin_x), "i"(1));
                left = origin_x;
                break;
            }
            case 3:
            {
                register u32 origin_x asm("r1") = 2;
                asm("" : "=l"(origin_x) : "0"(origin_x), "i"(3));
                left = origin_x;
                break;
            }
            case 0:
            case 2:
            case 4:
            default:
                left = 2;
                asm("" : "=h"(left) : "0"(left), "i"(0));
                break;
        }
        register u32 top asm("r3") = 7;
        asm("" : "=l"(top) : "0"(top));
        u32 min_x = 2;
        if (tile_x < min_x || tile_y < top)
            goto done;

        register MineFloor const * width_floor asm("r0") = &floor;
        asm("" : "=l"(width_floor) : "0"(width_floor));
        u32 saved_top = top;
        asm("" : : "m"(saved_top));
        u32 width = GetMineFloorWidth(*width_floor);
        if (width * 2 + left <= tile_x)
            goto done;

        u32 height = GetMineFloorHeight(floor) * 2;
        asm("" : "=m"(saved_top) : "m"(saved_top));
        top = saved_top;
        if (top + height <= tile_y)
            goto done;

        u32 plot_x = (tile_x - left) / 2;
        u32 plot_y = (tile_y - top) / 2;
        result.tile_x = plot_x * 2 + left;
        result.tile_y = plot_y * 2 + top;
        result.plot_x = plot_x;
        result.plot_y = plot_y;
        asm("" : : "l"(top));
        found = true;
    }
done:
    return found;
}

struct GameStateLooseItemRect
{
    i16 left, top, right, bottom;
};

struct GameStateLooseItemTile
{
    i16 x, y;
};

struct GameStateLooseItemInfo
{
    u32 kind, map;
    i16 x, y;
    u32 variant;
};

struct GameStateLooseItemStore
{
    RucksackItem PickUp(u32, GameStateLooseItemRect const &, u32) asm("func_080A1B38");
};

EC int FindLooseItem(GameStateLooseItemStore const &, u32, GameStateLooseItemRect const &, u32)
    asm("FindMapForageLocation");
EC void ReadLooseItemInfo(GameStateLooseItemInfo &, GameStateLooseItemStore const &, int)
    asm("GetMapForageObstacle");
EC void RefreshLooseItemTile(GameStateMapView &, u32, GameStateLooseItemTile const &, u32, u32)
    asm("ClearMapTileRectangle");

struct GameStateLooseItemInteraction
{
    bool removed;
    i32 effect;
    i32 x, y;
    bool changed;
};

EC bool ApplyLooseItemInteraction(Farm &, GameStateLooseItemStore &, int, u32, u32, bool,
    GameStateLooseItemInteraction &) asm("GameState_TryBreakMapObstacle");

EC SECTION(".text.game_state_c2.0801D104") bool GameStateBreakLooseItem(GameStateMapOwner & game, Location const & location,
    u32 strength, bool heavyTool, GameStateLooseItemInteraction & result)
{
    GameStateMapData ** data_slot = &game.map_data;
    GameStateMapData * state = *data_slot;
    GameStateLooseItemStore & items = *reinterpret_cast<GameStateLooseItemStore *>(reinterpret_cast<u8 *>(state) + 0x34C8);
    register u32 season asm("r8") = func_0800E324(reinterpret_cast<Unk_0800E324 const &>(state->clock));
    GameStateLooseItemRect bounds;
    u8 const * bytes = reinterpret_cast<u8 const *>(&location);
    u16 const * words = reinterpret_cast<u16 const *>(&location);
    register u32 low_x asm("r0") = bytes[1] >> 2;
    asm("" : "=l"(low_x) : "0"(low_x));
    register u32 high_x asm("r1") = words[1];
    asm("" : "=l"(high_x) : "0"(high_x), "l"(low_x));
    register u32 mask asm("r3") = 0x3FF;
    asm("" : "=l"(mask) : "0"(mask), "l"(high_x));
    high_x = mask & high_x;
    asm("" : "=l"(high_x) : "0"(high_x), "l"(mask));
    register int x asm("r1") = static_cast<i16>((high_x << 6) | low_x);
    register u32 low_y asm("r2") = bytes[3] >> 2;
    register u32 high_y asm("r0") = words[2];
    asm("" : "=l"(high_y) : "0"(high_y), "l"(low_y));
    high_y = mask & high_y;
    asm("" : "=l"(high_y) : "0"(high_y), "l"(mask));
    register int y asm("r0") = static_cast<i16>((high_y << 6) | low_y);
    register GameStateLooseItemRect * rect asm("r2") = &bounds;
    asm("" : "=l"(rect) : "0"(rect), "l"(x), "l"(y));
    rect->left = x;
    rect->top = y;
    rect->right = x + 1;
    rect->bottom = y + 1;

    register int index asm("r4") = FindLooseItem(items, location.GetMap(), *rect, season);
    if (index != -1)
    {
        GameStateLooseItemInfo record;
        ReadLooseItemInfo(record, items, index);
        register GameStateLooseItemInfo * info asm("r6") = &record;
        asm("" : : "l"(info));
        register GameStateMapData * current = *data_slot;
        register Farm * farm = &current->farm;
        register u32 store_offset = 0x34C8;
        register GameStateLooseItemStore * current_items = reinterpret_cast<GameStateLooseItemStore *>(reinterpret_cast<u8 *>(current) + store_offset);
        ApplyLooseItemInteraction(*farm, *current_items, index, season, strength, heavyTool, result);
        if (result.removed)
        {
            u32 map = location.GetMap();
            register GameStateMapView * active_view asm("r1") = game.map_view;
            register u32 map_id asm("r2") = active_view->id;
            register GameStateMapView * view asm("r5") = active_view;
            if (map == map_id)
            {
                register int width asm("r3");
                register int height asm("r4");
                switch (result.effect)
                {
                case 4: width = 2; height = 2; break;
                case 7: width = 4; asm("" : "+l"(width)); goto tall;
                case 6: width = 4; asm("" : "+l"(width)); goto tall;
                case 9: width = 6; tall: height = 4; break;
                default: goto miss;
                }
                register int pixel_x asm("r0") = info->x;
                asm("" : "=l"(pixel_x) : "0"(pixel_x));
                register int tile_x asm("r1") = pixel_x / 8;
                register int tile_y asm("r0") = info->y / 8;
                asm("" : : "l"(tile_x), "l"(tile_y), "l"(width), "l"(height));
                GameStateLooseItemTile origin;
                register GameStateLooseItemTile * tile = &origin;
                tile->x = tile_x;
                tile->y = tile_y;
                RefreshLooseItemTile(*view, 1, *tile, width, height);
            }
        }
        return true;
    }
miss:
    return false;
}

EC SECTION(".text.game_state_c2.0801D6AC") RucksackItem GameStateCollectLooseItem(
    GameStateMapOwner & game, Location const & location)
{
    GameStateMapData * state = game.map_data;
    GameStateLooseItemStore & items = *reinterpret_cast<GameStateLooseItemStore *>(reinterpret_cast<u8 *>(state) + 0x34C8);
    register u32 season asm("r9") = func_0800E324(reinterpret_cast<Unk_0800E324 const &>(state->clock));
    GameStateLooseItemRect bounds;

    u8 const * bytes = reinterpret_cast<u8 const *>(&location);
    u16 const * words = reinterpret_cast<u16 const *>(&location);
    register u32 low_x asm("r0") = bytes[1] >> 2;
    asm("" : "=l"(low_x) : "0"(low_x));
    register u32 high_x asm("r1") = words[1];
    asm("" : "=l"(high_x) : "0"(high_x), "l"(low_x));
    register u32 mask asm("r3") = 0x3FF;
    asm("" : "=l"(mask) : "0"(mask), "l"(high_x));
    high_x = mask & high_x;
    asm("" : "=l"(high_x) : "0"(high_x), "l"(mask));
    register int x asm("r1") = static_cast<i16>((high_x << 6) | low_x);
    register u32 low_y asm("r2") = bytes[3] >> 2;
    register u32 high_y asm("r0") = words[2];
    asm("" : "=l"(high_y) : "0"(high_y), "l"(low_y));
    high_y = mask & high_y;
    asm("" : "=l"(high_y) : "0"(high_y), "l"(mask));
    register int y asm("r0") = static_cast<i16>((high_y << 6) | low_y);
    register GameStateLooseItemRect * rect asm("r4") = &bounds;
    asm("" : "=l"(rect) : "0"(rect), "l"(x), "l"(y));
    rect->left = x;
    rect->top = y;
    rect->right = x + 1;
    rect->bottom = y + 1;

    u8 item_storage[sizeof(RucksackItem)] ALIGN(4);
    register int index asm("r8") = FindLooseItem(items, location.GetMap(), *rect, season);
    new (item_storage) RucksackItem;
    if (index != -1)
    {
        RucksackItem & item = *reinterpret_cast<RucksackItem *>(item_storage);
        item = items.PickUp(location.GetMap(), *rect, season);
        GameStateLooseItemInfo record;
        if (!item.IsEmpty() && location.GetMap() == game.map_view->id)
        {
            GameStateLooseItemTile redraw_origin;
            ReadLooseItemInfo(record, items, index);
            register GameStateMapView * view = game.map_view;
            register int pixel_x asm("r0") = record.x;
            asm("" : "=l"(pixel_x) : "0"(pixel_x));
            register int tile_x asm("r3") = pixel_x / 8;
            register int tile_y asm("r0") = record.y / 8;
            asm("" : : "l"(tile_x), "l"(tile_y));
            register GameStateLooseItemTile * tile = &redraw_origin;
            tile->x = tile_x;
            tile->y = tile_y;
            RefreshLooseItemTile(*view, 1, *tile, 2, 2);
        }
    }
    return *reinterpret_cast<RucksackItem const *>(item_storage);
}

EC SECTION(".text.game_state_c2.0801D88C") GameStatePlotPlacement GameStateCheckFarmPlotPlacement(
    GameStateMapOwner & game, Location const & location, Article const & article)
{
    GameStateFarmPlot result;
    if (GameStateFindFarmPlot(game, location, result))
    {
        if (result.plot->method_0800A6C8(article))
            return GAME_STATE_PLOT_CAN_PLACE;
        return GAME_STATE_PLOT_CANNOT_PLACE;
    }
    return GAME_STATE_PLOT_OUTSIDE_FIELD;
}

EC SECTION(".text.game_state_c2.0801D8CC") void GameStateEnableEntities(GameStateMapOwner & game)
{
    game.entities_enabled = true;
    if (game.map_data->player_only_when_disabled)
    {
        AEntity ** slot = game.entities;
        AEntity ** end = game.entities + 100;
        for (; slot != end; ++slot)
            if (*slot != nullptr)
                (*slot)->vfunc_20();
    }
    else
    {
        AEntity ** slot = game.entities;
        AEntity ** end = game.entities + 100;
        for (; slot != end; ++slot)
            if (*slot != nullptr)
                (*slot)->vfunc_20();
    }
}

EC SECTION(".text.game_state_c2.0801D948") void GameStateDisableEntities(GameStateMapOwner & game)
{
    game.entities_enabled = false;
    if (game.map_data->player_only_when_disabled)
    {
        game.entities[0]->vfunc_24();
    }
    else
    {
        AEntity ** slot = game.entities;
        AEntity ** end = game.entities + 100;
        for (; slot != end; ++slot)
            if (*slot != nullptr)
                (*slot)->vfunc_24();
    }
}

EC SECTION(".text.game_state_c2.0801D9A8") void GameStateQueueScriptEvent(GameStateMapOwner & game, GameStateScriptEvent const & event, u32 queue)
{
    GameScript_QueueSceneEvent(*game.script, event, queue);
}

EC SECTION(".text.game_state_c2.0801DB44") GameStateAudioList * GameStateGetAudioList(GameStateMapOwner & game)
{
    return reinterpret_cast<GameStateAudioList *>(GameScript_GetSoundPlayers(*game.script));
}

EC SECTION(".text.game_state_c2.0801DB58") void GameStateRefreshMapMusic(GameStateMapOwner & game)
{
    GameStateMusicParams params;
    int map = game.vtable->get_map_id(game);
    u32 mine_floor = map - 0x34;
    if (mine_floor <= 511 || map == 0x2B)
    {
        if (map == 0x2B)
            goto special_floor;
        else if (map > 0x133)
            map -= 0x134;
        else
            map = mine_floor;
        goto adjust_pitch;
    special_floor:
        map = 9;
    adjust_pitch:
        i32 pitch = params.pitch;
        params.pitch = pitch + ((-2560 - pitch) * (map + 1)) / 256;
    }
    int fatigue = func_0800E4FC(game.map_data->farmer);
    if (fatigue > 79)
    {
        params.modulation = fatigue - 79;
        params.lfo_speed = 3;
    }
    GameStateScript * script = game.script;
    u32 track = game.vtable->get_music_track(game);
    GameScript_ConfigureMusic(*script, 1, track, params);
}

EC SECTION(".text.game_state_c2.0801DCA8") void GameStateClearMapInteraction(GameStateMapOwner & game)
{
    game.interaction = nullptr;
}

EC SECTION(".text.game_state_c2.0801DC24") void GameStateStartMapInteraction(GameStateMapOwner & game, u32 animation)
{
    SmartPtr<GameStateMapInteraction> & active = game.interaction;
    GameStateMapInteraction * interaction = active.Get();
    if (interaction != nullptr)
    {
        func_0805E860(interaction->animator, animation);
        interaction->playing = true;
        interaction->ended = false;
        interaction->restart = true;
    }
    else
    {
        GameStateMapInteraction * storage = static_cast<GameStateMapInteraction *>(::operator new(sizeof(GameStateMapInteraction)));
        GameStatePackedSpriteArchive const * archive = game.vtable->get_interaction_archive(game);
        game.interaction = InitializeAnimatedMapSpriteSinglePalette(storage, archive, animation, game, 3, 0x1B, true);
    }
}

EC SECTION(".text.game_state_c2.0801DCD4") void GameStateSettleShippingBin(GameStateMapOwner & game)
{
    GameStateMapData * & state = game.map_data;
    u32 proceeds = state->farm.shipping_bin.GetValueShipped();
    SaveData_CollectShippingValue(*state);
    void (* const * queue_event)(GameStateMapOwner &, GameStateScriptEvent const &, u32) = &game.vtable->queue_event;
    GameStateScriptEvent event(0x3DB, proceeds);
    (*queue_event)(game, event, 1);
}

EC SECTION(".text.game_state_c2.0801F14C") void GameStateRefreshMapScenery(GameStateMapOwner & game)
{
    game.scenery_active = false;
    game.scenery = nullptr;

    u32 variant;
    switch (game.vtable->get_map_id(game))
    {
        case 0x1D:
        {
            GameStateMapData * & state = game.map_data;
            if (state->farm.farm_house.GetUpgradeLevel() != 2)
                return;
            variant = state->farm.farm_house.IsFireplaceLighted() != false;
            break;
        }
        case 0x12:
            variant = game.map_data->map_12_scenery_variant;
            break;
        case 0x27:
            variant = game.map_data->map_27_scenery_variant;
            break;
        case 0x0A:
        case 0x1B:
            variant = func_0800E324(reinterpret_cast<Unk_0800E324 const &>(game.map_data->clock)) == SEASON_WINTER;
            break;
        default:
            return;
    }

    GameStateMapInteraction * storage = static_cast<GameStateMapInteraction *>(::operator new(sizeof(GameStateMapInteraction)));
    GameStatePackedSpriteArchive const * archive = game.vtable->get_scenery_archive(game);
    game.scenery = InitializeAnimatedMapSpriteSinglePalette(storage, archive, variant, game, 2, 0x1B, false);
}

static inline GameStateMapInteraction * MakeTimedMapInteraction(GameStateMapOwner & game, u32 animation)
{
    GameStateMapInteraction * storage = static_cast<GameStateMapInteraction *>(::operator new(sizeof(GameStateMapInteraction)));
    return InitializeAnimatedMapSpriteSinglePalette(storage, &game.sprite_archives[12], animation, game, 5, 0x1B, false);
}

EC SECTION(".text.game_state_c2.0801F2DC") void GameStateRefreshTimedMapInteraction(GameStateMapOwner & game)
{
    if (game.vtable->get_map_id(game) == 0x23)
    {
        u32 hour = game.map_data->clock.hour;
        if (hour <= 3)
            goto night;
        if (hour <= 8)
            game.timed_interaction = MakeTimedMapInteraction(game, 0);
        else if (hour <= 15)
            game.timed_interaction = MakeTimedMapInteraction(game, 1);
        else if (hour <= 18)
            game.timed_interaction = MakeTimedMapInteraction(game, 2);
        else
        {
        night:
            game.timed_interaction = MakeTimedMapInteraction(game, 3);
        }
    }
    else
        game.timed_interaction = nullptr;
}

struct GameStateVaseFrame
{
    u32 words[8];
};

struct GameStateVaseClip
{
    SpriteKeyframe const * keyframes;
    u16 length;
};

struct GameStateVaseDisplay;
struct GameStateVaseDisplayVtable
{
    STRUCT_PAD(0, 8);
    void (*destroy)(GameStateVaseDisplay *, u32);
};

struct GameStateVaseDisplay
{
    STRUCT_PAD(0, 0x24);
    GameStateVaseDisplayVtable const * vtable;
    GameStateVaseFrame frame;
    bool active;
};

struct GameStateVaseArchiveVtable
{
    STRUCT_PAD(0, 0xC);
    void (*get_clip)(GameStateVaseClip *, GameStatePackedSpriteArchive const *, u32);
    void (*decode)(GameStateVaseFrame *, GameStatePackedSpriteArchive const *, u32);
};

struct GameStateVaseAnimator
{
    GameStatePackedSpriteArchive const * source;
    SpriteKeyframe const * keyframes;
    STRUCT_PAD(8, 12);
    u16 keyframe;
    STRUCT_PAD(14, 20);
};

struct GameStateVaseScratch
{
    GameStateVaseFrame frame;
    GameStateVaseAnimator animator;
};

EC void InitGameStateVaseAnimator(GameStateVaseAnimator *, GameStatePackedSpriteArchive const *, u32, int) asm("InitSpriteAnimator");
EC void InitGameStateVaseDisplay(GameStateVaseDisplay *, GameStateMapOwner &, u32, u32) asm("InitializeMapSpriteSinglePalette");
EC GameStateVaseDisplayVtable const vtable_unk_080E602C;
EC void * CopyGameStateVaseFrame(void *, void const *, size_t) asm("memcpy");

static inline GameStateVaseArchiveVtable const * GetVaseArchiveVtable(GameStatePackedSpriteArchive const * archive)
{
    return static_cast<GameStateVaseArchiveVtable const *>(archive->vtable);
}

static inline SpriteKeyframe const * GetVaseKeyframes(GameStateVaseClip clip)
{
    return clip.keyframes;
}

EC SECTION(".text.game_state_c2.0801F3E4") void GameStateRefreshVase(GameStateMapOwner & game)
{
    GameStateVaseScratch scratch;
    register GameStateVaseDisplay * replacement asm("r5");
    GameStateVaseDisplay ** slot;
    int speed_q8;
    if (game.vtable->get_map_id(game) == 0x1D)
    {
        u32 article_id = game.map_data->farm.farm_house.GetVaseArticleId();
        if (article_id == 0x5F)
            return;

        Article article(article_id);
        register u16 icon = article.GetIconId();
        register GameStateVaseDisplay ** base = &game.vase_display;
        register GameStateVaseDisplay ** vase_slot = base;
        register GameStateVaseDisplay * display asm("r4") = *base;
        speed_q8 = 0x100;
        if (display)
        {
            GameStatePackedSpriteArchive const * archive = game.vtable->get_vase_archive(game);
            register GameStateVaseAnimator * init = &scratch.animator;
            asm("" : "=l"(init) : "0"(init));
            InitGameStateVaseAnimator(init, archive, icon, speed_q8);
            register GameStateVaseAnimator * animator asm("r0") = &scratch.animator;
            GameStatePackedSpriteArchive const * source = scratch.animator.source;
            GameStateVaseArchiveVtable const * methods = GetVaseArchiveVtable(source);
            register u32 offset = animator->keyframe * sizeof(SpriteKeyframe);
            SpriteKeyframe const * keyframes = scratch.animator.keyframes;
            register u32 frame_id = reinterpret_cast<SpriteKeyframe const *>(reinterpret_cast<u8 const *>(keyframes) + offset)->frame;
            register GameStateVaseFrame * frame asm("r0") = &scratch.frame;
            methods->decode(frame, source, frame_id);
            CopyGameStateVaseFrame(&scratch.frame, &scratch.frame, sizeof(scratch.frame));
            display->frame = scratch.frame;
            display->active = true;
            asm("" : : "l"(display));
            return;
        }

        replacement = static_cast<GameStateVaseDisplay *>(::operator new(sizeof(GameStateVaseDisplay)));
        GameStatePackedSpriteArchive const * archive = game.vtable->get_vase_archive(game);
        InitGameStateVaseDisplay(replacement, game, 2, 0x1B);
        replacement->vtable = &vtable_unk_080E602C;
        GameStateVaseFrame * dest = &replacement->frame;
        GameStateVaseClip clip;
        GameStateVaseArchiveVtable const * methods = GetVaseArchiveVtable(archive);
        methods->get_clip(&clip, archive, icon);
        register u32 frame_id asm("r2") = GetVaseKeyframes(clip)->frame;
        register GameStateVaseFrame * frame asm("r0") = dest;
        asm("" : "=l"(frame_id), "=l"(frame) : "0"(frame_id), "1"(frame));
        methods->decode(frame, archive, frame_id);
        replacement->active = true;
        slot = vase_slot;
    }
    else
    {
        slot = &game.vase_display;
        replacement = nullptr;
    }

    GameStateVaseDisplay * previous = *slot;
    if (replacement != previous && previous)
        previous->vtable->destroy(previous, 3);
    *slot = replacement;
}

EC SECTION(".text.game_state_c2.0801F5F0") void GameStateSetVaseArticle(GameStateMapOwner & game, u32 article_id)
{
    game.map_data->farm.farm_house.SetVaseArticleId(article_id);
    GameStateRefreshVase(game);
}

EC SECTION(".text.game_state_c2.0801FCA4") GameStatePlayerData const * GameStateGetPlayerData(GameStateMapOwner const & game)
{
    return reinterpret_cast<GameStatePlayerData const *>(reinterpret_cast<u8 const *>(game.map_data) + 8);
}

EC SECTION(".text.game_state_c2.0801FD30") GameStateMapEffect * GameStateGetMapEffect(GameStateMapOwner & game)
{
    return game.map_effect;
}

EC SECTION(".text.game_state_c2.0801DD18") void GameStateApplyRow15TilePatchVariant1(GameStateMapOwner & game)
{
    ApplyMapRow15PatchA(*game.map_view);
}

EC SECTION(".text.game_state_c2.0801DD24") void GameStateApplyRow15TilePatchVariant2(GameStateMapOwner & game)
{
    ApplyMapRow15PatchB(*game.map_view);
}

EC SECTION(".text.game_state_c2.0801DD30") void GameStateApplyRow3TilePatchVariant1(GameStateMapOwner & game)
{
    ApplyMapRow3PatchA(*game.map_view);
}

EC SECTION(".text.game_state_c2.0801DD3C") void GameStateApplyRow3TilePatchVariant2(GameStateMapOwner & game)
{
    ApplyMapRow3PatchB(*game.map_view);
}

EC SECTION(".text.game_state_c2.0801DD48") void GameStateApplyRow2TilePatchVariant1(GameStateMapOwner & game)
{
    ApplyMapRow2PatchA(*game.map_view);
}

EC SECTION(".text.game_state_c2.0801DD54") void GameStateApplyRow2TilePatchVariant2(GameStateMapOwner & game)
{
    ApplyMapRow2PatchB(*game.map_view);
}

EC SECTION(".text.game_state_c2.0801DD60") void GameStateApplyMapTilePatchAtIndex(GameStateMapOwner & game, u32 index)
{
    ApplyMapIndexedOverlayPatch(*game.map_view, index);
}

EC SECTION(".text.game_state_c2.0801DD6C") void GameStateApplyMapTilePatchInColumn(GameStateMapOwner & game, u32 index)
{
    ApplyMapIndexedFixturePatch(*game.map_view, index);
}

EC SECTION(".text.game_state_c2.0801DD78") void GameStateStartMapViewTimer(GameStateMapOwner & game)
{
    StartMapTransitionDelay(*game.map_view);
}

EC SECTION(".text.game_state_c2.0801DD84") int GameStateRemoveRecordPlayerAlbum(GameStateMapOwner & game)
{
    RecordPlayer * record_player = game.map_data->farm.farm_house.GetRecordPlayer();
    ApplyMapRow17PatchA(*game.map_view);
    ArticleStack previous_album = record_player->RemoveAlbum();
    GameStateRefreshMapMusic(game);
    return previous_album.GetArticle().GetId();
}

EC SECTION(".text.game_state_c2.0801DDCC") void GameStateSetMapTileVariant(GameStateMapOwner & game, u32 index)
{
    SetAlternateMapPatch(game.map_data->tile_variant_flags, index);
    ApplySavedMapTilePatch(reinterpret_cast<MapUpdateState &>(*game.map_view), index);
}

EC SECTION(".text.game_state_c2.0801DDF8") void GameStateClearMapTileVariant(GameStateMapOwner & game, u32 index)
{
    ClearAlternateMapPatch(game.map_data->tile_variant_flags, index);
    ApplySavedMapTilePatch(reinterpret_cast<MapUpdateState &>(*game.map_view), index);
}

EC SECTION(".text.game_state_c2.0801DE24") void GameStateApplyMapTilePatchWithAlternateRow(GameStateMapOwner & game, u32 alternate)
{
    ApplyMapBuildingDoorPatch(*game.map_view, alternate);
}

EC SECTION(".text.game_state_c2.0801DE30") void GameStateApplyMapTilePatchAtRow20(GameStateMapOwner & game)
{
    ClearSupermarketRucksackDisplay(reinterpret_cast<MapUpdateState &>(*game.map_view));
}

EC SECTION(".text.game_state_c2.0801DE3C") void GameStateApplyMapTilePatchAtRow23(GameStateMapOwner & game)
{
    ClearSupermarketBlueFeatherDisplay(reinterpret_cast<MapUpdateState &>(*game.map_view));
}

EC SECTION(".text.game_state_c2.0801DE48") void GameStateQueueMapEvent1D(GameStateMapOwner & game)
{
    RequestMap29Refresh(reinterpret_cast<MapUpdateState &>(*game.map_view));
}

EC SECTION(".text.game_state_c2.0801DEB4") bool GameStateClearNearestEligibleFarmPlot(GameStateMapOwner & owner, Vec2 const & center)
{
    register GameStateMapOwner & game asm("sl") = owner;
    Vec2 const & point = center;
    struct SearchState
    {
        Unk_Something const * outgoing_neighbor_tiles;
        int best_distance;
        int best_y;
        int tile_x;
        GameStateMapView * view;
        int remaining;
        GameStateFarmField * field;
        int center_y;
    } scratch;
    const u32 field_offset_in_map_data = offsetof(GameStateMapData, farm) + offsetof(Farm, field);
    scratch.best_distance = 0;
    register int best_x asm("r8") = 0;
    register u32 no_candidate asm("r1") = 0;
    asm("" : "+l"(no_candidate) : "h"(best_x));
    scratch.best_y = no_candidate;
    register u32 found asm("ip") = no_candidate;
    asm("" : : "h"(found));
    register unsigned y asm("r4") = 0;

    register GameStateMapData * data asm("r0") = game.map_data;
    register u32 field_offset asm("r2") = field_offset_in_map_data;
    register GameStateFarmField * farm_field asm("r2");
    asm("add %0, %2, %0" : "=l"(farm_field) : "0"(field_offset), "l"(data) : "cc");
    scratch.field = farm_field;
    register int width asm("r1");
    do
    {
        register unsigned x asm("r3") = 0;
        register unsigned following asm("r6") = y + 1;
        asm("" : "+l"(following) : "l"(x));
        register unsigned next_y asm("r9") = following;
        register int center_x asm("r5") = 280;
        register unsigned offset asm("r0") = y * (Farm::FIELD_WIDTH * sizeof(FieldPlot));
        register u32 field_row asm("r1") = reinterpret_cast<u32>(scratch.field);
        asm("" : "+l"(field_row) : "l"(offset));
        register FieldPlot * plot asm("r2") = reinterpret_cast<FieldPlot *>(offset + field_row);
        scratch.center_y = y * 16 + 184;

        do
        {
            FieldPlot const sample = *plot;
            if (sample.GetUnk2() == 20 && sample.GetUnk8() == 5)
            {
                register int coordinate_offset asm("r6") = offsetof(Vec2, x);
                register int origin_x asm("r1");
                asm("ldrsh %0, [%1, %2]" : "=l"(origin_x) : "l"(&point), "l"(coordinate_offset), "m"(point.x));
                register int dx asm("r1") = center_x - origin_x;
                register int origin_y asm("r0") = point.y;
                register int dy asm("r0") = scratch.center_y - origin_y;
                register int dx_squared asm("r6") = dx * dx;
                dx = dx_squared;
                register int dy_squared asm("r6") = dy * dy;
                asm("" : "+l"(dy_squared) : "l"(dx), "l"(dy));
                dy = dy_squared;
                asm("" : "+l"(dy) : "l"(dx));
                register int distance asm("r1") = dx + dy;
                register int candidate asm("r0") = distance;

                register u32 already_found asm("r6") = found;
                asm("" : : "l"(already_found), "l"(candidate));
                if (!already_found || distance < scratch.best_distance)
                {
                    register u32 hit asm("r1") = 1;
                    asm volatile ("" : : "l"(hit));
                    found = hit;
                    best_x = x;
                    scratch.best_y = y;
                    scratch.best_distance = candidate;
                }
            }
            center_x += 16;
            ++plot;
            ++x;
        } while (x < Farm::FIELD_WIDTH);
        width = Farm::FIELD_WIDTH;
        asm("" : "+l"(width));
        y = next_y;
    } while (y < Farm::FIELD_HEIGHT);
    register u32 have_result asm("r2") = found;
    asm("" : "+l"(have_result));
    if (!have_result)
        goto miss;
    {
        register GameStateMapData * current_data asm("r0") = game.map_data;
        register u32 field_offset asm("r3") = field_offset_in_map_data;
        asm("" : "+l"(field_offset) : "l"(current_data));
        register FieldPlot * field asm("r0") = reinterpret_cast<FieldPlot *>(reinterpret_cast<u8 *>(current_data) + field_offset);
        asm("" : "+l"(field));
        register int selected_y asm("r6") = scratch.best_y;
        register int selected_row asm("r1") = selected_y * width;
        asm("" : "+l"(selected_row) : "l"(selected_y), "l"(field));
        int x = best_x;
        asm("" : "+l"(x) : "l"(selected_row) : "r5");
        register int index asm("r4") = x + selected_row;
        field[index].method_0800A3C8();
        register GameStateMapOwner * map_owner asm("r1") = &game;
        asm("" : "+l"(map_owner));
        if (map_owner->vtable->get_map_id(game) == 2)
        {
            scratch.tile_x = x * 2 + 34;
            register int position_y asm("r0") = selected_y * 2;
            register GameStateMapOwner * owner_view asm("r2") = &game;
            register GameStateMapView * view asm("r2") = owner_view->map_view;
            scratch.view = view;
            register int tile_y asm("r9") = position_y + 20;
            register int row_index asm("r5") = index - Farm::FIELD_WIDTH;
            register int row asm("r4") = selected_y;
            asm("" : "+l"(row));
            --row;
            register int remaining asm("r3") = 2;
            scratch.remaining = remaining;
            register int rows_left asm("r7");
            do
            {
                register GameStateMapData * data asm("r0") = game.map_data;
                register u32 field_offset asm("r6") = field_offset_in_map_data;
                asm("" : "+l"(field_offset) : "l"(data));
                register FieldPlot * field asm("r1") = reinterpret_cast<FieldPlot *>(reinterpret_cast<u8 *>(data) + field_offset);
                register u32 in_bounds asm("r0") = 0;
                register int column asm("r7") = best_x;
                if (column >= 0 && row >= 0 && column < Farm::FIELD_WIDTH && row < Farm::FIELD_HEIGHT)
                    in_bounds = 1;
                register FieldPlot * plot asm("r3");
                if (in_bounds)
                {
                    register int offset asm("r0") = row_index * sizeof(FieldPlot);
                    plot = reinterpret_cast<FieldPlot *>(reinterpret_cast<u8 *>(field) + offset);
                }
                else
                    plot = nullptr;
                register FieldPlot * selected asm("r6") = plot;
                if (selected)
                {
                    register GameStateMapData * data asm("r0") = game.map_data;
                    register u32 field_offset asm("r1") = field_offset_in_map_data;
                    register FieldPlot * field asm("r2") = reinterpret_cast<FieldPlot *>(reinterpret_cast<u8 *>(data) + field_offset);
                    register int this_row asm("r3") = row;
                    register FieldPlot * current asm("ip") = &field[row_index];

                    register int south_row asm("r0") = row + 1;
                    register u32 south_in_bounds asm("r1") = 0;
                    register int column asm("r7") = best_x;
                    if (column >= 0 && south_row >= 0 && column < Farm::FIELD_WIDTH && south_row < Farm::FIELD_HEIGHT)
                        south_in_bounds = 1;
                    register FieldPlot * south asm("r1");
                    if (south_in_bounds)
                    {
                        register int width asm("r1") = Farm::FIELD_WIDTH;
                        south_row *= width;
                        south_row += best_x;
                        south = &field[south_row];
                    }
                    else
                        south = nullptr;

                    register int north_row asm("r0") = this_row - 1;
                    register u32 north_in_bounds asm("r3") = 0;
                    column = best_x;
                    if (column >= 0 && north_row >= 0 && column < Farm::FIELD_WIDTH && north_row < Farm::FIELD_HEIGHT)
                        north_in_bounds = 1;
                    register FieldPlot * north asm("r2");
                    if (north_in_bounds)
                    {
                        register int width asm("r3") = Farm::FIELD_WIDTH;
                        north_row *= width;
                        north_row += best_x;
                        north = &field[north_row];
                    }
                    else
                        north = nullptr;
                    register Unk_Something const * adjacent asm("r0") = current->method_0800AF5C(south, north);
                    asm volatile (
                        ".ifnc \"%1\", \"[sp]\"\n\t"
                        ".error \"Field redraw argument must be at the outgoing argument slot\"\n\t"
                        ".endif\n\t"
                        "str %0, %1\n\t"
                        "ldr r0, %2\n\t"
                        "ldr r1, %3\n\t"
                        "mov r2, %4\n\t"
                        "add r3, %5, #0\n\t"
                        "bl ApplyFieldPlotToMap"
                        : "+l"(adjacent), "=m"(scratch.outgoing_neighbor_tiles)
                        : "m"(scratch.view), "m"(scratch.tile_x), "h"(tile_y), "l"(selected)
                        : "r1", "r2", "r3", "ip", "lr", "cc", "memory");
                }
                register int step asm("r6") = 2;
                tile_y += step;
                row_index += Farm::FIELD_WIDTH;
                ++row;
                rows_left = scratch.remaining;
                rows_left--;
                scratch.remaining = rows_left;
            } while (rows_left >= 0);
        }
    }
    return true;
miss:
    return false;
}

EC SECTION(".text.game_state_c2.0801DE54") int GameStateSetRecordPlayerAlbum(GameStateMapOwner & game, u32 album_id)
{
    RecordPlayer * record_player = game.map_data->farm.farm_house.GetRecordPlayer();
    ApplyMapRow17PatchB(*game.map_view);
    ArticleStack previous_album = record_player->SetAlbum(Article(album_id));
    GameStateRefreshMapMusic(game);
    return previous_album.GetArticle().GetId();
}

EC SECTION(".text.game_state_c2.0801FD3C") bool GameStateIsCameraIdle(GameStateMapOwner & game)
{
    return IsMapTransitionIdle(*game.map_view);
}

EC SECTION(".text.game_state_c2.0801FD48") i32 GameStateGetCameraY(GameStateMapOwner const & game)
{
    return game.map_view->camera_y.pixel;
}

EC SECTION(".text.game_state_c2.0801FD50") i32 GameStateGetCameraX(GameStateMapOwner const & game)
{
    return game.map_view->camera_x.pixel;
}

EC SECTION(".text.game_state_c2.0801FD58") u32 GameStateGetMapViewId(GameStateMapOwner const & game)
{
    return game.map_view->id;
}

EC SECTION(".text.game_state_c2.0801FD60") void GameStatePanCamera(GameStateMapOwner & game, i32 x, i32 y, u32 speed)
{
    StartMapCameraPan(*game.map_view, x, y, speed);
}

EC SECTION(".text.game_state_c2.0801FD6C") void GameStateSetCameraCenter(GameStateMapOwner & game, i32 x, i32 y)
{
    SetMapCameraPosition(*game.map_view, x, y);
}

EC SECTION(".text.game_state_c2.0801FAA8") AEntity * GameStateLookupEntitySlot(GameStateMapOwner & game, u32 slot)
{
    return game.vtable->get_entity(game, slot);
}

EC SECTION(".text.game_state_c2.0801FAB8") AEntity const * GameStateLookupConstEntitySlot(GameStateMapOwner const & game, u32 slot)
{
    return game.vtable->get_const_entity(game, slot);
}

EC SECTION(".text.game_state_c2.0801FBDC") GameStatePackedSpriteArchive const * GameStateGetSpriteArchive12(GameStateMapOwner const & game)
{
    return &game.sprite_archives[12];
}

EC SECTION(".text.game_state_c2.0801FBE8") GameStatePackedSpriteArchive const * GameStateGetSpriteArchive11(GameStateMapOwner const & game)
{
    return &game.sprite_archives[11];
}

EC SECTION(".text.game_state_c2.0801FBF4") GameStatePackedSpriteArchive const * GameStateGetSpriteArchive10(GameStateMapOwner const & game)
{
    return &game.sprite_archives[10];
}

EC SECTION(".text.game_state_c2.0801FC00") GameStatePackedSpriteArchive const * GameStateGetSpriteArchive9(GameStateMapOwner const & game)
{
    return &game.sprite_archives[9];
}

EC SECTION(".text.game_state_c2.0801FC0C") GameStatePackedSpriteArchive const * GameStateGetSpriteArchive8(GameStateMapOwner const & game)
{
    return &game.sprite_archives[8];
}

EC SECTION(".text.game_state_c2.0801FC18") GameStatePackedSpriteArchive const * GameStateGetSpriteArchive7(GameStateMapOwner const & game)
{
    return &game.sprite_archives[7];
}

template<u32 EventId>
static inline void QueueShippingMilestone(GameStateMapOwner & game, u32 argument)
{
    void (* const * queue_event)(GameStateMapOwner &, GameStateScriptEvent const &, u32) = &game.vtable->queue_event;
    GameStateScriptEvent event(EventId, argument);
    (*queue_event)(game, event, 1);
}

EC SECTION(".text.game_state_c2.0801F924") void GameStateShipProduct(GameStateMapOwner & game, Product const & product)
{
    ShippingBin & bin = game.map_data->farm.shipping_bin;
    int id = product.GetId();
    u32 previous = bin.IsDisplayEnabled(id) ? bin.GetAmountShipped(id) : 0;
    bin.Ship(product);
    u32 amount = bin.GetAmountShipped(id);
    if (previous >= amount)
        goto collection;

    switch (amount)
    {
        case 10000: QueueShippingMilestone<0x33C>(game, id); break;
        case 100000: QueueShippingMilestone<0x33D>(game, id); break;
        case 1000000: QueueShippingMilestone<0x33E>(game, id); break;
        case 10000000: QueueShippingMilestone<0x33F>(game, id); break;
        case 100000000: QueueShippingMilestone<0x340>(game, id); break;
        case 1000000000: QueueShippingMilestone<0x341>(game, id); break;
    }

collection:
    u32 status = game.map_data->shipping_collection_flags & 3;
    if (status == 0 && bin.HasShippedOneOfEachProduct())
        QueueShippingMilestone<0x342>(game, status);
}

static inline GameStateMapInteraction * AllocateMapInteraction()
{
    return static_cast<GameStateMapInteraction *>(::operator new(sizeof(GameStateMapInteraction)));
}

EC SECTION(".text.game_state_c2.0801F614") void GameStateRefreshPairedScenery(GameStateMapOwner & owner)
{
    register GameStateMapOwner & game asm("r6") = owner;
    game.paired_scenery_first = nullptr;
    game.paired_scenery_second = nullptr;

    u32 map = game.vtable->get_map_id(game);
    if (map == 0)
    {
        u32 season = func_0800E324(reinterpret_cast<Unk_0800E324 const &>(game.map_data->clock));
        if (season != SEASON_WINTER)
        {
            GameStateMapInteraction * first = AllocateMapInteraction();
            register GameStatePackedSpriteArchive const * archive asm("r5") = &game.sprite_archives[7];
            game.paired_scenery_first = InitializeAnimatedMapSpriteSinglePalette(first, archive, 0, game, 3, 0x1E, map);
            GameStateMapInteraction * second = AllocateMapInteraction();
            game.paired_scenery_second = InitializeAnimatedMapSpriteSinglePalette(second, archive, 0, game, 3, 0x1E, false);
        }
        else
        {
            GameStateMapInteraction * first = AllocateMapInteraction();
            register GameStatePackedSpriteArchive const * archive asm("r5") = &game.sprite_archives[7];
            game.paired_scenery_first = InitializeAnimatedMapSpriteSinglePalette(first, archive, 1, game, season, 0x1E, map);
            GameStateMapInteraction * second = AllocateMapInteraction();
            game.paired_scenery_second = InitializeAnimatedMapSpriteSinglePalette(second, archive, 1, game, 3, 0x1E, false);
        }
    }
    asm("" : : "l"(&game));
}

EC SECTION(".text.game_state_c2.0801F77C") void GameStateRefreshSeasonalScenery(GameStateMapOwner & owner)
{
    register GameStateMapOwner & game asm("r5") = owner;
    game.seasonal_scenery = nullptr;
    game.alternate_seasonal_scenery = nullptr;

    int map = game.vtable->get_map_id(game);
    switch (map)
    {
        case 7:
            if ((static_cast<u32>(reinterpret_cast<u8 const *>(game.map_data)[4]) << 31) != 0)
                game.alternate_seasonal_scenery = InitializeAnimatedMapSpriteSinglePalette(AllocateMapInteraction(), &game.sprite_archives[7], 2, game, 2, 0x1F, false);
            if (func_0800E324(reinterpret_cast<Unk_0800E324 const &>(game.map_data->clock)) == SEASON_WINTER)
                game.seasonal_scenery = InitializeAnimatedMapSpriteSinglePalette(AllocateMapInteraction(), &game.sprite_archives[7], 2, game, 2, 0x1F, false);
            break;
        case 3:
            if (func_0800E324(reinterpret_cast<Unk_0800E324 const &>(game.map_data->clock)) == SEASON_WINTER)
                game.seasonal_scenery = InitializeAnimatedMapSpriteSinglePalette(AllocateMapInteraction(), &game.sprite_archives[7], 2, game, 2, 0x1F, false);
            break;
        case 0:
            if (reinterpret_cast<u8 const *>(game.map_data)[3] >> 7)
                game.seasonal_scenery = InitializeAnimatedMapSpriteSinglePalette(AllocateMapInteraction(), &game.sprite_archives[7], 2, game, 2, 0x1F, map);
            break;
        case 2:
            if (game.map_data->farm.farm_house.IsFireplaceLighted())
                game.seasonal_scenery = InitializeAnimatedMapSpriteSinglePalette(AllocateMapInteraction(), &game.sprite_archives[7], 2, game, map, 0x1F, false);
            break;
    }
}

static inline void InstallMapOverlay(GameStateMapOverlay * & overlay, GameStateMapOverlay * replacement)
{
    if (replacement != overlay && overlay != nullptr)
        DestroyGameStateMapOverlay(overlay, 3);
    overlay = replacement;
}

EC SECTION(".text.game_state_c2.0801FAC8") void GameStateCreateMapOverlay(GameStateMapOwner & game, u32 kind)
{
    GameStatePackedSpriteArchive const * archive = nullptr;
    if (kind == 0)
        goto first;
    if (kind == 1)
        archive = &game.sprite_archives[6];
    goto selected;
first:
    archive = &game.sprite_archives[5];
selected:
    if (archive != nullptr)
    {
        int capacity = 32;
        u32 map = game.map_view->id;
        if (map == 2)
        {
            int count = 0;
            AEntity ** slot = game.entities;
            AEntity ** end = game.entities + 100;
            for (; slot != end; ++slot)
                if (*slot != nullptr && (*slot)->GetMap() == map)
                    ++count;
            if (static_cast<u32>(count + 10) > 10u)
            {
                if (static_cast<u32>(count) <= 21u)
                    capacity = 32 - count;
                else
                    capacity = 10;
            }
        }
        GameStateMapOverlay * storage = static_cast<GameStateMapOverlay *>(::operator new(sizeof(GameStateMapOverlay)));
        InstallMapOverlay(game.map_overlay, InitializeGameStateMapOverlay(storage,
            reinterpret_cast<MapOverlayScene *>(&game), reinterpret_cast<SpriteAnimationSource const *>(archive), kind, capacity));
    }
}

EC SECTION(".text.game_state_c2.0801FC24") GameStatePackedSpriteArchive const * GameStateGetSpriteArchive6(GameStateMapOwner const & game)
{
    return &game.sprite_archives[6];
}

EC SECTION(".text.game_state_c2.0801FC30") GameStatePackedSpriteArchive const * GameStateGetSpriteArchive5(GameStateMapOwner const & game)
{
    return &game.sprite_archives[5];
}

EC SECTION(".text.game_state_c2.0801FC3C") GameStatePackedSpriteArchive const * GameStateGetSpriteArchive4(GameStateMapOwner const & game)
{
    return &game.sprite_archives[4];
}

EC SECTION(".text.game_state_c2.0801FC48") GameStatePackedSpriteArchive const * GameStateGetSpriteArchive3(GameStateMapOwner const & game)
{
    return &game.sprite_archives[3];
}

struct GameStateSavedMapView : GameStateMapView
{
    STRUCT_PAD(0x94, 0xBC);
};

struct GameStateMapObject
{
    virtual ~GameStateMapObject();
};

struct GameStateMapHandle
{
    struct Transfer
    {
        Transfer(GameStateMapObject ** source, GameStateMapObject * pointer)
            : source(source), pointer(pointer) {}

        GameStateMapObject * Release() const
        {
            GameStateMapObject * owned = pointer;
            *source = nullptr;
            return owned;
        }

        GameStateMapObject ** source;
        GameStateMapObject * pointer;
    };

    explicit GameStateMapHandle(GameStateMapObject * pointer) : pointer(pointer) {}
    GameStateMapHandle(Transfer const & transfer) : pointer(transfer.Release()) {}
    ~GameStateMapHandle() { delete pointer; }

    Transfer Move() { return Transfer(&pointer, pointer); }

private:
    GameStateMapObject * pointer;
};

inline void * operator new(size_t, GameStateMapHandle * storage) { return storage; }

EC GameStateSavedMapView * InitializeWorldMapBuffers(GameStateSavedMapView * view, GameStateMapData & state);
// 0x080175B4
EC GameStateMapObject * GameStateMapScene_Initialize(GameStateMapObject * map, GameStateMapData & state, GameStateScript * script,
    GameStateSavedMapView * & view, bool restore_events);

EC SECTION(".text.game_state_c2.0801FB7C") GameStateMapHandle * GameStateCreateMapObject(
    GameStateMapHandle * result, GameStateMapData & state, GameStateScript * script, bool restore_events)
{
    GameStateSavedMapView * view = InitializeWorldMapBuffers(
        static_cast<GameStateSavedMapView *>(::operator new(sizeof(GameStateSavedMapView))), state);
    GameStateMapHandle map(GameStateMapScene_Initialize(
        static_cast<GameStateMapObject *>(::operator new(sizeof(GameStateMapOwner))), state, script, view, restore_events));
    return new (result) GameStateMapHandle(map.Move());
}

EC SECTION(".text.game_state_c2.0801FC54") GameStatePackedSpriteArchive const * GameStateGetSpriteArchive2(GameStateMapOwner const & game)
{
    return &game.sprite_archives[2];
}

EC SECTION(".text.game_state_c2.0801FC60") GameStatePackedSpriteArchive const * GameStateGetSpriteArchive1(GameStateMapOwner const & game)
{
    return &game.sprite_archives[1];
}

EC SECTION(".text.game_state_c2.0801FC6C") GameStatePackedSpriteArchive const * GameStateGetSpriteArchive0(GameStateMapOwner const & game)
{
    return &game.sprite_archives[0];
}

EC SECTION(".text.game_state_c2.0801FC78") void GameStateClearMapOverlay(GameStateMapOwner & game)
{
    InstallMapOverlay(game.map_overlay, nullptr);
}

EC SECTION(".text.game_state_c2.0801FC9C") GameStateMapView * GameStateGetMapView(GameStateMapOwner & game)
{
    return game.map_view;
}

EC SECTION(".text.game_state_c2.0801FCA0") GameStateMapView const * GameStateGetConstMapView(GameStateMapOwner const & game)
{
    return game.map_view;
}

EC SECTION(".text.game_state_c2.0801FCB4") i32 GameStateGetCachedSpriteSlot(GameStateMapOwner & game, u32 slot)
{
    GameStateSpriteCache & cache = game.sprite_cache;
    u8 & cached = cache.slot_ids[slot];
    i32 sprite_slot = cached - 0x10;
    if (sprite_slot < 0)
    {
        sprite_slot = AssignMapSceneSpriteCacheSlot(cache, slot);
        cached = sprite_slot + 0x10;
    }
    return sprite_slot;
}

EC SECTION(".text.game_state_c2.0801FCDC") void GameStateClearSpriteCacheEntry(GameStateMapOwner & game, u32 slot)
{
    game.sprite_cache.entries[slot].active = false;
}

EC SECTION(".text.game_state_c2.0801FCF4") AEntity const * GameStateGetConstEntitySlot(GameStateMapOwner const & game, u32 slot)
{
    AEntity * const * entities = game.entities;
    entities += slot;
    return *entities;
}

EC SECTION(".text.game_state_c2.0801FD00") AEntity * GameStateGetEntitySlot(GameStateMapOwner & game, u32 slot)
{
    AEntity ** entities = game.entities;
    entities += slot;
    return *entities;
}

EC SECTION(".text.game_state_c2.0801FD0C") void GameStateRemoveEntitySlot(GameStateMapOwner & game, u32 slot)
{
    AEntity ** entities = game.entities;
    AEntity * & entity = *(entities + slot);
    if (entity != nullptr)
    {
        delete entity;
        entity = nullptr;
    }
}

struct GameStateTransitionSpriteRecord
{
    u8 kind : 2;
    STRUCT_PAD(1, 2);
    u16 map : 10;
    STRUCT_PAD(4, 8);
    u16 icon;
    STRUCT_PAD(0xA, 0xC);
};

struct GameStateTransitionWork
{
    GameStateVaseClip clip;
    u32 refresh_music;
    u32 slot;
    u32 kind;
    AEntity ** actor_begin;
    AEntity ** actor_end;
    GameStateTransitionSpriteRecord const * records;
};

EC void LoadMapAtLocation(GameStateMapView &, Location const &, u32, u32, u32);
EC void Dma3_Copy(void const *, void *, u32);
EC void DestroyTransitionOverlay(GameStateMapEffect *, u32) asm("DestroyFixedSpriteOverlay");
EC GameStateMapEffect * CreateFixedSpriteOverlay(void *, GameStateMapOwner &) asm("InitializeFixedSpriteOverlay");
EC void DestroyMapTileEffectQueue(MapTileEffectQueue *, u32);
EC MapTileEffectQueue * CreateMapTileEffectQueue(void *, GameStateMapOwner &, u32) asm("func_080AAFC8");
EC bool HasPendingScriptEvents(void const *);
EC u32 GameScript_IsMusicActive(GameStateScript &);

EC SECTION(".text.game_state_c2.08019BEC") void GameStateTransitionMap(GameStateMapOwner & owner, Location const & target, u32 refresh_music)
{
    GameStateMapOwner & game = owner;
    register Location const & location asm("r8") = target;
    GameStateTransitionWork work;
    work.refresh_music = static_cast<u8>(refresh_music);
    register AEntity ** first asm("r4") = game.entities;
    register AEntity ** last asm("r5") = game.entities + 100;
    asm("" : "+l"(first), "+l"(last));
    for (AEntity ** actor = first; actor != last; ++actor)
        if (*actor)
            (*actor)->vfunc_14();

    register GameStateTransitionSpriteRecord const * source asm("sl") =
        reinterpret_cast<GameStateTransitionSpriteRecord const *>(reinterpret_cast<u8 *>(gActiveGameScene) + 0x4DC);
    register u32 index asm("r5") = 0;
    asm("" : "+l"(index) : "h"(source));
    register AEntity ** actor_begin asm("r2") = game.entities;
    asm("" : "+l"(actor_begin));
    work.actor_begin = actor_begin;
    register u32 end_offset asm("r3") = offsetof(GameStateMapOwner, entities) + sizeof(game.entities);
    asm volatile("" : "+l"(end_offset), "=m"(work.actor_begin) : "m"(work.actor_begin));
    register AEntity ** actor_tail asm("r3") = reinterpret_cast<AEntity **>(reinterpret_cast<u8 *>(&game) + end_offset);
    asm("" : "+l"(actor_tail));
    work.actor_end = actor_tail;
    register u32 overlay_offset asm("r0") = offsetof(GameStateMapOwner, map_overlay);
    asm("" : "+l"(overlay_offset));
    register GameStateMapOverlay ** overlay_addr asm("r0") = reinterpret_cast<GameStateMapOverlay **>(overlay_offset + reinterpret_cast<u32>(&game));
    asm("" : "+l"(overlay_addr));
    register GameStateMapOverlay ** overlay_slot asm("r9") = overlay_addr;
    register GameStateVaseDisplay * empty asm("r6") = nullptr;
    register u32 icons_offset asm("r1") = offsetof(GameStateMapOwner, transition_icons);
    asm("" : "+l"(icons_offset) : "l"(empty), "h"(overlay_slot));
    register GameStateVaseDisplay ** icons asm("r4") = reinterpret_cast<GameStateVaseDisplay **>(reinterpret_cast<u32>(&game) + icons_offset);
    {
    clear_icons:
        GameStateVaseDisplay * previous = *icons;
        if (empty != previous && previous)
            previous->vtable->destroy(previous, 3);
        *icons++ = empty;
        switch (++index)
        {
        case 6 ... 0xFFFFFFFFU: break;
        default: goto clear_icons;
        }
    }

    register Location const * pos asm("r2") = &location;
    asm("" : "+l"(pos));
    if (pos->GetMap() != 2)
    {
        GameStateMapView * view = game.map_view;
        if (view->id == 2 && game.farm_terrain)
            Dma3_Copy(view->expanded_terrain, const_cast<TerrainInfo *>(game.farm_terrain), 0xB000);
    }
    LoadMapAtLocation(*game.map_view, location, game.daylight_band, game.daylight_amount, 30 - game.daylight_amount);

    {
        register unsigned offset asm("r0") = offsetof(GameStateMapOwner, map_effect);
        asm("" : "+l"(offset) : "l"(&game));
        register GameStateMapEffect ** slot asm("r4") = reinterpret_cast<GameStateMapEffect **>(reinterpret_cast<u8 *>(&game) + offset);
        register GameStateMapEffect * empty asm("r5") = nullptr;
        register GameStateMapEffect * previous asm("r0") = *slot;
        if (empty != previous && previous)
            DestroyTransitionOverlay(previous, 3);
        *slot = empty;
    }
    {
        register GameStateMapOverlay ** slot asm("r4") = overlay_slot;
        register GameStateMapOverlay * empty asm("r5") = nullptr;
        register GameStateMapOverlay * previous asm("r0") = *slot;
        if (empty != previous && previous)
            DestroyGameStateMapOverlay(previous, 3);
        *slot = empty;
    }
    {
        register unsigned offset asm("r1") = offsetof(GameStateMapOwner, tile_effects);
        asm("" : "+l"(offset) : "l"(&game));
        register MapTileEffectQueue ** slot asm("r4") = reinterpret_cast<MapTileEffectQueue **>(reinterpret_cast<u8 *>(&game) + offset);
        register MapTileEffectQueue * empty asm("r5") = nullptr;
        register MapTileEffectQueue * previous asm("r0") = *slot;
        if (empty != previous && previous)
            DestroyMapTileEffectQueue(previous, 3);
        *slot = empty;
    }

    register u32 first_slot asm("r2") = 0;
    asm("" : "+l"(first_slot));
    work.slot = first_slot;
    register GameStateTransitionSpriteRecord const * initial asm("r3") = source;
    asm("" : "+l"(initial));
    work.records = initial;
    {
    restore_icons:
        GameStateTransitionSpriteRecord const * record = work.records;
        work.kind = record->kind;
        if (work.kind == 1 && record->map == game.vtable->get_map_id(game))
        {
            register u32 offset asm("r2") = offsetof(GameStateMapOwner, transition_icons);
            asm("" : "+l"(offset));
            register GameStateVaseDisplay ** base asm("r2") = reinterpret_cast<GameStateVaseDisplay **>(offset + reinterpret_cast<u32>(&game));
            asm("" : "+l"(base));
            register GameStateVaseDisplay ** slot asm("r9") = base;
            asm("" : "+h"(slot));
            register u32 index asm("r3") = work.slot;
            asm("" : "+l"(index));
            register u32 step asm("r0") = index * sizeof(*slot);
            asm("" : "+l"(step));
            slot = reinterpret_cast<GameStateVaseDisplay **>(reinterpret_cast<u32>(slot) + step);
            register GameStateVaseDisplay * replacement asm("r6") =
                static_cast<GameStateVaseDisplay *>(::operator new(sizeof(GameStateVaseDisplay)));
            register GameStatePackedSpriteArchive const * archive asm("r5") = game.vtable->get_vase_archive(game);
            register u32 icon asm("sl") = work.records->icon;
            InitGameStateVaseDisplay(replacement, game, 2, 0x1B);
            replacement->vtable = &vtable_unk_080E602C;
            register u32 frame_offset asm("r1") = offsetof(GameStateVaseDisplay, frame);
            asm("" : "+l"(frame_offset));
            register GameStateVaseFrame * dest asm("r1") = reinterpret_cast<GameStateVaseFrame *>(frame_offset + reinterpret_cast<u32>(replacement));
            asm("" : "+l"(dest));
            register GameStateVaseFrame * frame asm("r8") = dest;
            register GameStateVaseArchiveVtable const * methods asm("r4") = GetVaseArchiveVtable(archive);
            register GameStateVaseClip * clip asm("r0") = &work.clip;
            asm("" : "+l"(clip) : "l"(methods));
            methods->get_clip(clip, archive, icon);
            register u32 frame_index asm("r2") = work.clip.keyframes->frame;
            asm("" : "+l"(frame_index));
            register GameStateVaseFrame * output asm("r0") = frame;
            asm("" : "+l"(output) : "l"(frame_index));
            methods->decode(output, archive, frame_index);
            register u8 * active asm("r0") = reinterpret_cast<u8 *>(&replacement->active);
            asm("" : "+l"(active));
            register u8 kind asm("r2");
            asm(
                ".ifnes \"%1\", \"[sp, #0x14]\"\n\t"
                ".error \"Transition sprite kind must be at sp + 20\"\n\t"
                ".endif\n\t"
                "mov %0, sp\n\t"
                "ldrb %0, [%0, #20]"
                : "=l"(kind) : "m"(work.kind), "l"(active));
            asm("" : "+l"(kind) : "l"(active));
            *active = kind;
            register GameStateVaseDisplay ** dest_slot asm("r4") = slot;
            asm("" : "+l"(dest_slot));
            register GameStateVaseDisplay * previous asm("r1") = *dest_slot;
            if (replacement != previous && previous)
                previous->vtable->destroy(previous, 3);
            *dest_slot = replacement;
        }
        register GameStateTransitionSpriteRecord const * next asm("r3") = work.records;
        asm("" : "+l"(next));
        work.records = next + 1;
        switch (++work.slot)
        {
        case 6 ... 0xFFFFFFFFU: break;
        default: goto restore_icons;
        }
    }

    {
        register GameStateMapEffect * result asm("r0") = CreateFixedSpriteOverlay(::operator new(0xA0), game);
        register u32 offset asm("r1") = offsetof(GameStateMapOwner, map_effect);
        asm("" : "+l"(offset) : "l"(result));
        register GameStateMapEffect ** slot asm("r5") = reinterpret_cast<GameStateMapEffect **>(reinterpret_cast<u8 *>(&game) + offset);
        asm("" : "+l"(slot) : "l"(result));
        register GameStateMapEffect * camera asm("r4") = result;
        register GameStateMapEffect * previous asm("r0") = *slot;
        if (camera != previous && previous)
            DestroyTransitionOverlay(previous, 3);
        *slot = camera;
    }
    if (!GetMapData(game.map_view->map)->is_interior)
    {
        register i32 map asm("r1") = game.map_view->id;
        register u32 offset asm("r2") = offsetof(GameStateMapOwner, map_data);
        asm("" : "+l"(offset), "+l"(map));
        register GameStateMapData ** address asm("r0") = reinterpret_cast<GameStateMapData **>(reinterpret_cast<u8 *>(&game) + offset);
        asm("" : "+l"(address) : "l"(map));
        register GameStateMapData * state asm("r0") = *address;
        asm("" : "+l"(state) : "l"(map));
        register u32 * weather_record asm("r0") = reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(state) + 8);
        asm("" : "+l"(weather_record) : "l"(map));
        register u32 weather asm("r0");
        if (map == 46) goto rain;
        if (map <= 46) goto earlier;
        if (map == 47) goto snow;
        goto current_weather;
    earlier:
        switch (map)
        {
        case 44 ... 0x7FFFFFFF: break;
        default: goto current_weather;
        }
        weather = 0;
        goto selected;
    rain:
        weather = 1;
        goto selected;
    snow:
        weather = 2;
        goto selected;
    current_weather:
        weather = *weather_record;
    selected:
        switch (weather)
        {
            case 1: GameStateCreateMapOverlay(game, 1); break;
            case 2: GameStateCreateMapOverlay(game, 0); break;
        }
    }
    void * storage = ::operator new(4);
    {
        register MapTileEffectQueue * result asm("r0") = CreateMapTileEffectQueue(storage, game, game.map_view->map);
        register u32 offset asm("r3") = offsetof(GameStateMapOwner, tile_effects);
        asm("" : "+l"(offset) : "l"(result));
        register MapTileEffectQueue ** slot asm("r5") = reinterpret_cast<MapTileEffectQueue **>(reinterpret_cast<u8 *>(&game) + offset);
        asm("" : "+l"(slot) : "l"(result));
        register MapTileEffectQueue * effects asm("r4") = result;
        register MapTileEffectQueue * previous asm("r0") = *slot;
        if (effects != previous && previous)
            DestroyMapTileEffectQueue(previous, 3);
        *slot = effects;
    }
    GameStateRefreshMapScenery(game);
    GameStateRefreshVase(game);
    GameStateRefreshTimedMapInteraction(game);
    GameStateRefreshPairedScenery(game);
    GameStateRefreshSeasonalScenery(game);
    GameStateRelocateObstructedFarmAnimals(game);

    register AEntity ** actor asm("r4") = work.actor_begin;
    register AEntity ** end asm("r5") = work.actor_end;
    asm("" : "+l"(actor), "+l"(end));
    for (; actor != end; ++actor)
    {
        register AEntity * entity asm("r2") = *actor;
        asm("" : "+l"(entity));
        if (entity)
            entity->vfunc_10();
    }
    if (work.refresh_music && (!HasPendingScriptEvents(reinterpret_cast<u8 *>(game.map_data) + 0x214C) ||
        !static_cast<u8>(GameScript_IsMusicActive(*game.script))))
        GameStateRefreshMapMusic(game);
    if (!GetMapData(game.map_view->map)->is_interior)
    {
        register u32 offset asm("r1") = offsetof(GameStateMapOwner, clock);
        asm("" : : "l"(offset));
        register GameStateClock const ** address asm("r0") = reinterpret_cast<GameStateClock const **>(reinterpret_cast<u8 *>(&game) + offset);
        asm("" : "+l"(address));
        GameStateClock const * clock = *address;
        func_0801A13C(game, clock->hour, clock->minute, true);
    }
}

struct ClockCalendarTime
{
    u8 year_in_cycle : 3;
    u8 year_cycle : 5;
    u8 season : 2;
    u8 day : 5;
    u32 : 1;
    u32 hour : 5;
    u32 minute : 6;
};

struct ClockCounters
{
    ClockCalendarTime * time;
    u8 frames_since_minute;
};

struct ClockTickContext
{
    u32 value;
    u8 minute_changed;
};

struct ClockActorList
{
    u32 size;
    AEntity * actors[100];
};

struct ClockOwnerMethods
{
    STRUCT_PAD(0, 0x124);
    void (*enable)(GameStateMapOwner &);
    void (*disable)(GameStateMapOwner &);
};

struct ClockActorMethods
{
    STRUCT_PAD(0, 0x18);
    void (*tick)(AEntity *, ClockTickContext &);
    STRUCT_PAD(0x1C, 0xA4);
    void (*finish)(AEntity *);
};

struct ClockAdvanceWork
{
    ClockActorList fast, slow;
    ClockTickContext tick;
    u16 const * target;
    u16 const * current;
    u32 was_enabled;
    AEntity ** actor_begin;
    AEntity ** actor_end;
    AEntity ** slow_end;
    u32 season;
    u32 date;
};

extern u32 const sClockFrameActorSlots[] asm("gUnk_080F0748");
extern u32 const sClockMinuteActorSlots[] asm("gUnk_080F0770");

static inline ClockActorMethods const & ClockMethods(AEntity * actor)
{
    return **reinterpret_cast<ClockActorMethods const **>(reinterpret_cast<u8 *>(actor) + 0x14);
}

static inline bool AdvanceClockFrame(ClockCounters & state, ClockAdvanceWork &, u32)
{
    register ClockCounters * counter asm("r6") = &state;
    asm("" : "+l"(counter));
    register u32 frames asm("r0") = counter->frames_since_minute + 1;
    asm("" : "+l"(frames));
    u32 frames_per_minute = 25;
    if (frames < frames_per_minute)
    {
        counter->frames_since_minute = frames;
        return false;
    }
    counter->frames_since_minute = 0;
    register u32 word asm("r1");
    {
        register ClockCalendarTime * time asm("r4") = counter->time;
        word = *reinterpret_cast<u32 *>(time);
        word = (word << 5 >> 26) + 1;
        register u32 mask asm("r2") = 63;
        register u32 bits asm("r2") = (mask & word) << 5;
        register u32 previous asm("r3") = reinterpret_cast<u16 *>(time)[1];
        register u32 preserve asm("r5") = ~0x7E0u;
        asm("" : "+l"(preserve) : "l"(previous));
        register u32 base asm("r0") = preserve;
        reinterpret_cast<u16 *>(time)[1] = (base & previous) | bits;
        register u32 low_mask asm("r0") = 63;
        word &= low_mask;
        if (word <= 59)
            return true;
        register ClockCalendarTime * reset asm("r1") = counter->time;
        asm("" : "+l"(reset));
        register u32 raw asm("r2") = reinterpret_cast<u16 *>(reset)[1];
        asm("" : "+l"(raw));
        register u32 minute_reset asm("r0") = preserve;
        asm("" : "+l"(minute_reset));
        reinterpret_cast<u16 *>(reset)[1] = minute_reset & raw;
    }
    {
        register ClockCalendarTime * time asm("r3") = counter->time;
        word = *reinterpret_cast<u32 *>(time);
        word = (word << 11 >> 27) + 1;
        register u32 hour_mask asm("r4") = 31;
        asm("" : "+l"(hour_mask) : "l"(word));
        register u32 field_mask asm("ip") = hour_mask;
        register u32 mask asm("r5") = field_mask;
        asm("" : "+l"(mask));
        word &= mask;
        register u32 previous asm("r2") = reinterpret_cast<u8 *>(time)[2];
        asm("" : "+l"(previous));
        register u32 preserve asm("r4") = ~31u;
        register u32 base asm("r0") = preserve;
        reinterpret_cast<u8 *>(time)[2] = (base & previous) | word;
        if (word <= 23)
            return true;
        register ClockCalendarTime * reset asm("r1") = counter->time;
        register u32 raw asm("r2") = reinterpret_cast<u8 *>(reset)[2];
        asm("" : "+l"(raw));
        register u32 hour_reset asm("r0") = preserve;
        asm("" : "+l"(hour_reset));
        reinterpret_cast<u8 *>(reset)[2] = hour_reset & raw;

        register ClockCalendarTime * date asm("r3") = counter->time;
        register u32 previous_date asm("r2") = reinterpret_cast<u8 *>(date)[1];
        register u32 day asm("r0") = ((previous_date << 25) >> 27) + 1;
        if (day == 30)
        {
            register u32 season asm("r0") = (previous_date << 30) >> 30;
            register u32 low_mask asm("r1");
            asm("mov %0, sl" : "=l"(low_mask));
            season &= low_mask;
            register u32 preserve asm("r1") = ~3u;
            register u8 * season_ptr asm("r4");
            asm("add %0, sp, #0x348" : "=l"(season_ptr));
            register u32 scratch asm("r4") = *season_ptr;
            preserve = (preserve & scratch) | season;
            ++season;
            register u32 low_mask_again asm("r5");
            asm("mov %0, sl" : "=l"(low_mask_again));
            season &= low_mask_again;
            register u32 clear asm("r4") = ~3u;
            preserve = (preserve & clear) | season;
            register u8 * season_out asm("r5");
            asm volatile("add %0, sp, #0x348" : "=l"(season_out) : "l"(preserve));
            asm volatile("strb %1, [%0]" : : "l"(season_out), "l"(preserve) : "memory");
            register u32 stored_season asm("r1") = (preserve << 30) >> 30;
            register u32 low_mask_final asm("r0");
            asm("mov %0, sl" : "=l"(low_mask_final));
            stored_season &= low_mask_final;
            register u32 update asm("r0");
            asm("add %0, %1, #0" : "=l"(update) : "l"(clear), "l"(stored_season));

            reinterpret_cast<u8 *>(date)[1] = (update & previous_date) | stored_season;
            day = 0;
        }
        register u32 day_mask asm("r1") = field_mask;
        register u32 day_bits asm("r2") = (day & day_mask) << 2;
        register u32 raw_date asm("r1") = reinterpret_cast<u8 *>(date)[1];
        asm("" : "+l"(raw_date));
        register u32 update asm("r0") = ~0x7Cu;
        reinterpret_cast<u8 *>(date)[1] = (update & raw_date) | day_bits;
    }
    {
        register u8 * scratch asm("r3");
        asm("add %0, sp, #0x34c" : "=l"(scratch));
        register u32 date asm("r0") = *scratch;
        register u32 season_clear asm("r3") = ~3u;
        date &= season_clear;
        register u32 day_clear asm("r4") = ~0x7Cu;
        register u32 masked asm("r0") = date & day_clear;
        register u8 * storage asm("r5");
        asm("add %0, sp, #0x34c" : "=l"(storage) : "l"(masked));
        *storage = masked;
    }
    register ClockCalendarTime * time asm("r2") = counter->time;
    register u32 new_year asm("r3") = 0;
    register u32 date asm("r1") = reinterpret_cast<u8 *>(time)[1];
    register u32 mask asm("r0") = 127;
    if ((mask & date) == 0)
        new_year = 1;
    if (new_year != 0)
    {
        register ClockCalendarTime * period asm("r4") = time;
        register u32 year_byte asm("r3") = reinterpret_cast<u8 *>(period)[0];
        register u32 low asm("r0") = (year_byte << 29) >> 29;
        register u32 year asm("r2") = low + 1;
        if (year > 6)
        {
            year = 0;
            register u32 cycle asm("r1") = year_byte >> 3;
            if (cycle <= 28)
            {
                cycle = (cycle + 1) << 3;
                register u32 clear asm("r0") = 7;
                reinterpret_cast<u8 *>(period)[0] = (clear & year_byte) | cycle;
            }
        }
        register u32 year_mask asm("r0") = 7;
        year &= year_mask;
        register u32 byte asm("r0") = reinterpret_cast<u8 *>(period)[0];
        register u32 preserve asm("r3") = ~7u;
        register u32 copy asm("r1");
        asm("add %0, %1, #0" : "=l"(copy) : "l"(preserve), "l"(byte));
        reinterpret_cast<u8 *>(period)[0] = (byte & copy) | year;
    }
    return true;
}

EC SECTION(".text.game_state_c2.0801EAA0") void GameStateAdvanceClockToTime(GameStateMapOwner & owner, u16 const & target)
{
    ClockAdvanceWork work;
    GameStateMapOwner & game = owner;
    register u16 const * requested asm("r1") = &target;
    asm("" : "+l"(requested) : "l"(&game));
    asm volatile("str %0, [sp, #0x330]" : : "l"(requested) : "memory");
    register u32 map_data_offset asm("r1") = offsetof(GameStateMapOwner, map_data);
    asm("" : "+l"(map_data_offset));
    register GameStateMapData ** map_data asm("r0") =
        reinterpret_cast<GameStateMapData **>(reinterpret_cast<u8 *>(&game) + map_data_offset);
    asm("" : "+l"(map_data));
    register u16 const * current_time asm("r0") = reinterpret_cast<u16 const *>(reinterpret_cast<u8 *>(*map_data) + 0x12);
    asm("" : "+l"(current_time));
    asm volatile("str %0, [sp, #0x334]" : : "l"(current_time) : "memory");
    register u32 enabled_offset asm("r3") = offsetof(GameStateMapOwner, entities_enabled);
    asm("" : "+l"(enabled_offset));
    register bool * enabled_ptr asm("r0") = reinterpret_cast<bool *>(reinterpret_cast<u8 *>(&game) + enabled_offset);
    asm("" : "+l"(enabled_ptr));
    register u32 enabled asm("r0") = *enabled_ptr;
    asm("" : "+l"(enabled));
    asm volatile("str %0, [sp, #0x338]" : : "l"(enabled) : "memory");
    if (enabled)
    {
        register ClockOwnerMethods const * methods asm("r0");
        asm("ldr %0, [%1]" : "=l"(methods) : "l"(&game) : "memory");
        asm("" : "+l"(methods));
        register u32 offset asm("r4") = offsetof(ClockOwnerMethods, disable);
        asm("" : "+l"(offset) : "l"(methods));
        register void (**entry)(GameStateMapOwner &) asm("r0") =
            reinterpret_cast<void (**)(GameStateMapOwner &)>(reinterpret_cast<u32>(methods) + offset);
        asm("" : "+l"(entry));
        (*entry)(game);
    }

    register AEntity ** begin asm("r5") = game.entities;
    asm("" : "+l"(begin));
    asm volatile("str %0, [sp, #0x33c]" : : "l"(begin) : "memory");
    register u32 entity_end_offset asm("r0") = offsetof(GameStateMapOwner, entities) + sizeof(game.entities);
    asm("" : "+l"(entity_end_offset));
    register AEntity ** end asm("r0") = reinterpret_cast<AEntity **>(reinterpret_cast<u8 *>(&game) + entity_end_offset);
    asm("" : "+l"(end));
    asm volatile("str %0, [sp, #0x340]" : : "l"(end) : "memory");
    register AEntity ** actor asm("r4") = begin;
    asm("" : "+l"(actor));
    register AEntity ** actors asm("r6") = actor;
    asm("" : "+l"(actors));
    register u32 tick_offset asm("r3") = offsetof(ClockAdvanceWork, tick);
    asm("" : "+l"(tick_offset) : "l"(actors));
    register ClockTickContext * tick_ptr asm("r3");
    asm("add %0, sp" : "=l"(tick_ptr) : "0"(tick_offset));
    register ClockTickContext * tick asm("r8") = tick_ptr;
    asm("" : "+h"(tick));
    if (actors != end)
    {
        do
        {
            register AEntity * entity asm("r2") = *actor;
            asm("" : "+l"(entity));
            if (entity)
                entity->vfunc_14();
            ++actor;
            register AEntity ** last asm("r0");
            asm volatile("ldr %0, [sp, #0x340]" : "=l"(last) : "l"(actor) : "memory");
            if (actor == last)
                break;
        } while (true);
    }
    register u32 zero asm("r0") = 0;
    asm("" : "+l"(zero) : "l"(actors));
    work.fast.size = zero;
    register u32 index asm("r3") = 0;
    asm("" : "+l"(index) : "m"(work.fast.size));
    register AEntity ** first_actors asm("r4") = actors;
    asm("" : "+l"(first_actors) : "l"(index));
    register u32 const * fast_slots asm("r2") = sClockFrameActorSlots;
    asm("" : "+l"(fast_slots) : "l"(first_actors));
    do
    {
        register u32 slot asm("r0") = *fast_slots;
        asm("" : "+l"(slot));
        register AEntity ** actor_ptr asm("r0");
        asm("add %0, %1, %0" : "=l"(actor_ptr) : "l"(first_actors), "0"(slot * 4));
        register AEntity * entity asm("r1") = *actor_ptr;
        asm("" : "+l"(entity));
        if (entity)
        {
            register u32 size asm("r0") = work.fast.size;
            asm("" : "+l"(size));
            if (size < 100)
            {
                register u32 offset asm("r0") = (size * sizeof(AEntity *)) + offsetof(ClockActorList, actors);
                asm("" : "+l"(offset));
                register AEntity ** dest asm("r0");
                asm("add %0, sp" : "=l"(dest) : "0"(offset));
                asm("" : "+l"(dest));
                if (dest)
                    *dest = entity;
                asm volatile("" : "+m"(work.fast.size));
                ++work.fast.size;
            }
        }
        ++fast_slots;
        asm("" : "+l"(fast_slots));
        ++index;
    } while (index <= 9);
    register u32 size asm("r0") = work.fast.size;
    asm("" : "+l"(size));
    register u32 fast_offset asm("r0") = size * sizeof(AEntity *) + offsetof(ClockActorList, actors);
    asm("" : "+l"(fast_offset));
    register AEntity ** fast_ptr asm("r0");
    asm("add %0, sp" : "=l"(fast_ptr) : "0"(fast_offset));
    register AEntity ** fast_end asm("r9") = fast_ptr;
    asm("" : "+h"(fast_end));
    register u32 slow_offset asm("r2") = offsetof(ClockAdvanceWork, slow);
    asm("" : "+l"(slow_offset));
    register u8 * stack asm("r3");
    asm("mov %0, sp" : "=l"(stack));
    register ClockActorList * slow_list asm("r1") = reinterpret_cast<ClockActorList *>(stack + slow_offset);
    asm("" : "+l"(slow_list));
    register u32 clear asm("r0") = 0;
    asm("" : "+l"(clear));
    slow_list->size = clear;
    register u32 slow_index asm("r5") = 0;
    asm("" : "+l"(slow_index) : "m"(slow_list->size));
    register u32 const * slow_slots asm("r4") = sClockMinuteActorSlots;
    asm("" : "+l"(slow_slots) : "l"(slow_index));
    do
    {
        register u32 slot asm("r0") = *slow_slots;
        asm("" : "+l"(slot));
        register AEntity ** actor_ptr asm("r0");
        asm("add %0, %1, %0" : "=l"(actor_ptr) : "l"(actors), "0"(slot * 4));
        register AEntity * entity asm("r3") = *actor_ptr;
        asm("" : "+l"(entity));
        if (entity)
        {
            register u8 * stack asm("r0");
            asm("mov %0, sp" : "=l"(stack) : "l"(entity));
            register ClockActorList * list asm("r1") = reinterpret_cast<ClockActorList *>(stack + slow_offset);
            asm("" : "+l"(list));
            register u32 size asm("r0") = list->size;
            asm("" : "+l"(size));
            if (size < 100)
            {
                register u32 offset asm("r0") = (size * sizeof(AEntity *)) + offsetof(ClockActorList, actors);
                asm("" : "+l"(offset));
                register AEntity ** dest asm("r0") = reinterpret_cast<AEntity **>(reinterpret_cast<u8 *>(list) + offset);
                asm("" : "+l"(dest));
                if (dest)
                    *dest = entity;
                asm volatile("" : "+m"(list->size));
                ++list->size;
            }
        }
        ++slow_slots;
        asm("" : "+l"(slow_slots));
        ++slow_index;
    } while (slow_index <= 35);
    register ClockActorList * slow asm("r1") = &work.slow;
    asm("" : "+l"(slow));
    register u32 used asm("r0") = slow->size;
    asm("" : "+l"(used));
    register u32 end_offset asm("r0") = used * sizeof(AEntity *) + offsetof(ClockActorList, actors);
    asm("" : "+l"(end_offset));
    register AEntity ** slow_end asm("r1") = reinterpret_cast<AEntity **>(reinterpret_cast<u8 *>(slow) + end_offset);
    asm("" : "+l"(slow_end));
    asm volatile("str %0, [sp, #0x344]" : : "l"(slow_end) : "memory");
    register u32 flags asm("r1") = 0;
    asm("" : "+l"(flags));
    register ClockTickContext * context asm("r0");
    asm("add %0, sp, #0x328" : "=l"(context) : "l"(flags));
    context->value = flags;
    register ClockTickContext * initial_tick asm("r3");
    asm("mov %0, r8" : "=l"(initial_tick));
    initial_tick->minute_changed = flags;
    register u32 clock_offset asm("r4") = offsetof(GameStateMapOwner, clock);
    asm("" : "+l"(clock_offset) : "m"(initial_tick->minute_changed));
    register ClockCounters * clock_ptr asm("r6") = reinterpret_cast<ClockCounters *>(reinterpret_cast<u8 *>(&game) + clock_offset);
    asm("" : "+l"(clock_ptr));
    register u32 seasons asm("r5") = 3;
    asm("" : "+l"(seasons) : "l"(clock_ptr));
    register u32 season_mask asm("sl") = seasons;
    asm volatile("" : "+h"(season_mask));
    goto advance_frame;
clear_minute_change:
    {
        register u32 state asm("r0") = 0;
        asm("" : "+l"(state));
        register ClockTickContext * context asm("r1");
        asm("mov %0, r8" : "=l"(context) : "l"(state));
        context->minute_changed = state;
    }
    goto advance_frame;
tick_frame_actors:
    {
        register AEntity ** actor asm("r4") = work.fast.actors;
        asm("" : "+l"(actor));
        if (actor != fast_end)
        {
            do
            {
                AEntity * next = *actor++;
                ClockMethods(next).tick(next, *tick);
            } while (actor != fast_end);
        }
    }
advance_frame:
    if (!AdvanceClockFrame(*clock_ptr, work, season_mask))
        goto tick_frame_actors;
    {
        register u32 state asm("r0") = 1;
        asm("" : "+l"(state));
        register ClockTickContext * context asm("r4");
        asm("mov %0, r8" : "=l"(context) : "l"(state));
        context->minute_changed = state;
        register AEntity ** actor asm("r4") = work.slow.actors;
        asm("" : "+l"(actor) : "m"(context->minute_changed));
        register AEntity ** end asm("r5");
        asm volatile("ldr %0, [sp, #0x344]" : "=l"(end) : "l"(actor) : "memory");
        if (actor != end)
        {
            do
            {
                AEntity * next = *actor++;
                ClockMethods(next).tick(next, *tick);
                register AEntity ** end asm("r0");
                asm volatile("ldr %0, [sp, #0x344]" : "=l"(end) : "l"(actor) : "memory");
                if (actor == end)
                    break;
            } while (true);
        }
    }
    {
        register u16 const * target asm("r1");
        asm volatile("ldr %0, [sp, #0x330]" : "=l"(target) : : "memory");
        register u32 requested asm("r2") = *target;
        asm("" : "+l"(requested));
        register u32 mask asm("r0") = 0x7FF;
        asm("" : "+l"(mask) : "l"(requested));
        register u16 const * current asm("r4");
        asm volatile("ldr %0, [sp, #0x334]" : "=l"(current) : "l"(mask) : "memory");
        register u32 actual asm("r3") = *current;
        asm("" : "+l"(actual));
        register u32 mask_copy asm("r1") = mask;
        asm("" : "+l"(mask_copy) : "l"(actual));
        mask_copy &= requested;
        asm("" : "+l"(mask_copy));
        mask &= actual;
        asm("" : "+l"(mask));
        if (mask_copy != mask)
            goto clear_minute_change;
    }
    register AEntity ** finish_actor asm("r4");
    asm volatile("ldr %0, [sp, #0x33c]" : "=l"(finish_actor) : : "memory");
    register AEntity ** last_actor asm("r5");
    asm volatile("ldr %0, [sp, #0x340]" : "=l"(last_actor) : : "memory");
    if (finish_actor != last_actor)
    {
        do
        {
            register AEntity * entity asm("r2") = *finish_actor;
            asm("" : "+l"(entity));
            if (entity)
            {
                register void (**vtable)(AEntity *) asm("r0") = *reinterpret_cast<void (***)(AEntity *)>(reinterpret_cast<u8 *>(entity) + 0x14);
                asm("" : "+l"(vtable));
                vtable[4](entity);
            }
            ++finish_actor;
            register AEntity ** last asm("r0");
            asm volatile("ldr %0, [sp, #0x340]" : "=l"(last) : "l"(finish_actor) : "memory");
            if (finish_actor == last)
                break;
        } while (true);
    }
    register u32 enable asm("r1");
    asm volatile("ldr %0, [sp, #0x338]" : "=l"(enable) : : "memory", "cc");
    u32 entity_state = enable;
    asm volatile("" : "+l"(entity_state));
    if (entity_state)
    {
        register ClockOwnerMethods const * methods asm("r0");
        asm("ldr %0, [%1]" : "=l"(methods) : "l"(&game) : "memory");
        register u32 offset asm("r3") = offsetof(ClockOwnerMethods, enable);
        asm("" : "+l"(offset) : "l"(methods));
        register void (**entry)(GameStateMapOwner &) asm("r0") =
            reinterpret_cast<void (**)(GameStateMapOwner &)>(reinterpret_cast<u32>(methods) + offset);
        asm("" : "+l"(entry));
        (*entry)(game);
    }
    register GameStateMapView * map_view asm("r0");
    asm("ldr %0, [%1, #4]" : "=l"(map_view) : "l"(&game) : "memory");
    if (!GetMapData(map_view->map)->is_interior)
    {
        register u32 offset asm("r4") = offsetof(GameStateMapOwner, clock);
        asm("" : "+l"(offset));
        register ClockCalendarTime ** time_ptr asm("r0") =
            reinterpret_cast<ClockCalendarTime **>(reinterpret_cast<u8 *>(&game) + offset);
        asm("" : "+l"(time_ptr));
        register u32 time asm("r2") = *reinterpret_cast<u32 *>(*time_ptr);
        asm("" : "+l"(time));
        func_0801A13C(game, (time << 11) >> 27, (time << 5) >> 26, false);
    }
    register AEntity * player asm("r0");
    asm("ldr %0, [%1, #8]" : "=l"(player) : "l"(&game) : "memory");
    ClockMethods(player).finish(player);
    register u32 script_offset asm("r5") = offsetof(GameStateMapOwner, script);
    asm("" : "+l"(script_offset));
    register GameStateScript ** script asm("r0") =
        reinterpret_cast<GameStateScript **>(reinterpret_cast<u8 *>(&game) + script_offset);
    asm("" : "+l"(script));
    if (static_cast<u8>(GameScript_IsMusicActive(**script)))
    {
        register u32 map_offset asm("r1") = offsetof(GameStateMapOwner, map_data);
        asm("" : "+l"(map_offset));
        register GameStateMapData ** map asm("r0") = reinterpret_cast<GameStateMapData **>(reinterpret_cast<u8 *>(&game) + map_offset);
        asm("" : "+l"(map));
        register u8 * map_data asm("r0") = reinterpret_cast<u8 *>(*map);
        asm("" : "+l"(map_data));
        register u32 events_offset asm("r3") = 0x214C;
        asm("" : "+l"(events_offset) : "l"(map_data));
        if (!HasPendingScriptEvents(map_data + events_offset))
            GameStateRefreshMapMusic(game);
    }
}

struct AudioPlayer;
struct GameStateAudioList
{
    AudioPlayer ** begin;
    AudioPlayer ** end;
};

struct GameStateDropDisplay
{
    GameStateMapInteraction * sprite;
    Vec2 position;
    u8 terrain_blocked;
    u8 height;
    u8 frame;
};

struct GameStateDropMethods
{
    STRUCT_PAD(0, 0x34);
    void (*describe_terrain)(UnkMap *, GameStateMapOwner &, u32);
    STRUCT_PAD(0x38, 0x140);
    void (*queue)(GameStateMapOwner &, GameStateScriptEvent const &, u32);
    STRUCT_PAD(0x144, 0x14C);
    GameStateAudioList * (*sounds)(GameStateMapOwner &);
};

struct GameStateDropPlayerMethods
{
    STRUCT_PAD(0, 0x5C);
    void (*receive)(AEntity *, RucksackItem const &, u32);
};

struct GameStateDropWork
{
    u32 outgoing_layer;
    u32 outgoing_palette;
    bool outgoing_start;
    STRUCT_PAD(9, 12);
    UnkMap terrain;
    u32 food;
    u32 event[2];
    u8 article;
    STRUCT_PAD(0x1D, 0x20);
    i16 position[2];
    u32 blocked;
    u32 queued_event;
    u8 * start_ptr;
    i16 * position_ptr;
};

EC u32 IsDropTile(UnkMap const &, int, int) asm("AreEntityFootTilesMarked");
EC i32 GetSpecialDropType(Location const &) asm("func_080A45A8");
EC bool AudioPlayer_IsPlaying(AudioPlayer *);
EC void AudioPlayer_StartSong(AudioPlayer *, u32);
EC void SaveProgress_SetFlagE(GameStateMapData &);
EC void TrackDroppedItem(void *, Location const &, void *) asm("ApplyRosterLitteringPenalty");
EC void ReadDropFood(Food *, RucksackItem const &) asm("GetFood__C12RucksackItem");
EC void InitDropFood(Food *, u32) asm("__4FoodUi");
EC void InitDropItem(RucksackItem *, u32) asm("__12RucksackItemG4Food");
EC u8 ReadDropArticle(RucksackItem const &) asm("GetArticle__C12RucksackItem");

// The layer, palette and start arguments are already in the caller's stack slots.
EC GameStateMapInteraction * InitializePreparedDropSprite(GameStateMapInteraction *, GameStatePackedSpriteArchive const *,
    u32, GameStateMapOwner &) asm("InitializeAnimatedMapSpriteSinglePalette");

static inline void * DropTemporary(GameStateDropWork & work)
{
    register void * frame asm("r0");
    asm volatile("add %0, sp, #12" : "=l"(frame));
    return frame;
}

static inline void SetDropSprite(GameStateDropDisplay & display, GameStateMapInteraction * sprite)
{
    GameStateMapInteraction * previous = display.sprite;
    if (sprite != previous && previous)
        delete previous;
    display.sprite = sprite;
}

EC SECTION(".text.game_state_c2.0801EE00") u32 GameStateDisplayDroppedItem(GameStateMapOwner & owner, Location const & location, RucksackItem const & item)
{
    register GameStateMapOwner & game asm("sl") = owner;
    register Location const & target asm("r9") = location;
    register RucksackItem const & dropped_item asm("r7") = item;
    GameStateDropWork work;
    register GameStateDropMethods const * methods asm("r1") = reinterpret_cast<GameStateDropMethods const *>(owner.vtable);
    asm("" : "+l"(methods));
    register Location const * pos asm("r0") = &target;
    asm("" : "+l"(pos) : "l"(methods));
    register u32 map asm("r2") = pos->GetMap();
    asm("" : "+l"(map) : "l"(methods));
    register UnkMap * terrain asm("r0");
    asm("add %0, sp, #12" : "=l"(terrain));
    asm("" : "+l"(terrain) : "l"(map));
    methods->describe_terrain(terrain, game, map);
    register i32 x_coord asm("r1") = target.GetX();
    asm("" : "+l"(x_coord));
    register i32 y_coord asm("r2") = target.GetY();
    asm("" : "+l"(y_coord) : "l"(x_coord));
    UnkMap const * drop_terrain = static_cast<UnkMap *>(DropTemporary(work));
    work.blocked = static_cast<u8>(IsDropTile(*drop_terrain, x_coord, y_coord) ^ 1);
    register u32 no_event asm("r3") = 0;
    asm("" : "+l"(no_event) : "m"(work.blocked));
    work.queued_event = no_event;
    register u32 track asm("r6") = 1;
    asm("" : "+l"(track));
    register u8 * position_base asm("r4");
    asm("mov %0, sp" : "=l"(position_base) : "m"(work.blocked));
    register i16 * position asm("r4") = reinterpret_cast<i16 *>(position_base + 0x28);
    asm("" : "+l"(position));
    work.position_ptr = position;
    register u8 * args_base asm("r0");
    asm("mov %0, sp" : "=l"(args_base) : "m"(work.position_ptr));
    register u8 * start asm("r0") = args_base + 8;
    asm("" : "+l"(start));
    work.start_ptr = start;
    asm volatile("" : : "m"(work.start_ptr));
    asm volatile("" : "+m"(work.blocked));
    register u32 blocked asm("r1") = work.blocked;
    asm("" : "+l"(blocked));
    if (!blocked)
    {
        register GameStateMapOwner * sound_owner asm("r2") = &game;
        asm("" : "+l"(sound_owner));
        register GameStateDropMethods const * methods asm("r0") = reinterpret_cast<GameStateDropMethods const *>(sound_owner->vtable);
        asm("" : "+l"(methods));
        register u32 offset asm("r3") = offsetof(GameStateDropMethods, sounds);
        asm("" : "+l"(offset) : "l"(methods));
        register GameStateAudioList * (**entry)(GameStateMapOwner &) asm("r0") =
            reinterpret_cast<GameStateAudioList * (**)(GameStateMapOwner &)>(reinterpret_cast<u32>(methods) + offset);
        asm("" : "+l"(entry));
        GameStateAudioList * sounds = (*entry)(game);
        register AudioPlayer ** current asm("r4") = sounds->begin;
        register AudioPlayer ** end asm("r5") = sounds->end;
        AudioPlayer * selected;
        for (; current != end; ++current)
        {
            if (!AudioPlayer_IsPlaying(*current))
            {
                selected = *current;
                goto play_sound;
            }
        }
        register AudioPlayer * last asm("r0");
        asm("sub %0, %1, #4\n\tldr %0, [%0]" : "=l"(last) : "l"(end) : "memory");
        selected = last;
    play_sound:
        AudioPlayer_StartSong(selected, 0x6D);
        switch (GetSpecialDropType(target))
        {
        case 4:
            if (dropped_item.GetKind() == RucksackItem::KIND_FOOD)
            {
                Food * food = reinterpret_cast<Food *>(DropTemporary(work));
                ReadDropFood(food, dropped_item);
                if (static_cast<Food *>(DropTemporary(work))->GetId() == 0xA0)
                    SaveProgress_SetFlagE(*game.map_data);
            }
            break;
        case 6:
        {
            register u32 eligible asm("r5") = 0;
            if (dropped_item.GetKind() == RucksackItem::KIND_FOOD)
            {
                Food * food = reinterpret_cast<Food *>(DropTemporary(work));
                ReadDropFood(food, dropped_item);
                if (static_cast<Food *>(DropTemporary(work))->GetId() > 0xE)
                {
                    register Food * copy asm("r4") = reinterpret_cast<Food *>(&work.terrain.unk_04);
                    ReadDropFood(copy, dropped_item);
                    if (copy->GetId() <= 0x14)
                        eligible = 1;
                }
            }
            if (eligible)
            {
                register GameStateMapOwner * owner asm("r4") = &game;
                asm("" : "+l"(owner));
                register AEntity * player asm("r6") = owner->entities[0];
                register GameStateDropPlayerMethods const * methods asm("r8") = *reinterpret_cast<GameStateDropPlayerMethods const **>(
                    reinterpret_cast<u8 *>(player) + 0x14);
                register RucksackItem * accepted asm("r5") = reinterpret_cast<RucksackItem *>(&work.terrain.unk_08);
                register Food * gift asm("r4") = reinterpret_cast<Food *>(&work.food);
                InitDropFood(gift, 0x15);
                register u32 low asm("r1") = *reinterpret_cast<u8 const *>(gift);
                asm("" : "+l"(low));
                register u8 const * middle_byte asm("r0") = reinterpret_cast<u8 const *>(&work.food) + 1;
                asm("" : "+l"(middle_byte));
                register u32 middle asm("r0") = *middle_byte;
                asm("" : "+l"(middle));
                register u32 two_bytes asm("r0") = (middle << 8) | low;
                asm("" : "+l"(two_bytes));
                register u8 const * high_byte asm("r1") = reinterpret_cast<u8 const *>(&work.food) + 2;
                asm("" : "+l"(high_byte));
                register u32 high asm("r1") = *high_byte;
                asm("" : "+l"(high));
                InitDropItem(accepted, (high << 16) | two_bytes);
                register GameStateDropPlayerMethods const * player_methods asm("r1") = methods;
                asm("" : "+l"(player_methods));
                player_methods->receive(player, *accepted, 0);
                track = 0;
            }
            break;
        }
        case 2:
        {
            register u32 event_id asm("r2") = 0x23B;
            asm("" : "+l"(event_id));
            work.queued_event = event_id;
            track = 0;
            break;
        }
        case 0:
            if (dropped_item.GetKind() == RucksackItem::KIND_FOOD)
            {
                Food * food = reinterpret_cast<Food *>(DropTemporary(work));
                ReadDropFood(food, dropped_item);
                if (static_cast<Food *>(DropTemporary(work))->GetId() == 2)
                {
                    register u32 event_id asm("r3") = 0x23C;
                    asm("" : "+l"(event_id));
                    work.queued_event = event_id;
                    track = 0;
                }
            }
            break;
        }
    }
    register u32 queued_event asm("r4") = work.queued_event;
    asm("" : "+l"(queued_event));
    if (queued_event)
    {
        register GameStateMapOwner * owner asm("r1") = &game;
        asm("" : "+l"(owner));
        register GameStateDropMethods const * methods asm("r0") = reinterpret_cast<GameStateDropMethods const *>(owner->vtable);
        register u32 offset asm("r2") = offsetof(GameStateDropMethods, queue);
        asm("" : "+l"(offset) : "l"(methods));
        register void (**entry)(GameStateMapOwner &, GameStateScriptEvent const &, u32) asm("r0") =
            reinterpret_cast<void (**)(GameStateMapOwner &, GameStateScriptEvent const &, u32)>(reinterpret_cast<u32>(methods) + offset);
        asm("" : "+l"(entry));
        register GameStateScriptEvent * event asm("r1") = reinterpret_cast<GameStateScriptEvent *>(work.event);
        asm("" : "+l"(event) : "l"(entry));
        register u32 zero asm("r2") = 0;
        asm("" : "+l"(zero) : "l"(event));
        work.event[0] = queued_event;
        event->argument = zero;
        (*entry)(game, *event, zero);
    }
    if (track)
    {
        register u8 * state asm("r2") = reinterpret_cast<u8 *>(game.map_data);
        register u32 item_offset asm("r3") = 0x1CD4;
        asm("" : "+l"(item_offset) : "l"(state));
        register void * items asm("r0") = state + item_offset;
        asm("" : "+l"(items));
        register u32 events_offset asm("r4") = 0x214C;
        asm("" : "+l"(events_offset) : "l"(items), "l"(state));
        TrackDroppedItem(items, target, state + events_offset);
    }
    register u32 icon asm("r5");
    if (dropped_item.IsWrapped())
        goto wrapped;
    else
    {
        switch (dropped_item.GetKind())
        {
        default:
        case RucksackItem::KIND_FOOD:
        {
            Food * food = reinterpret_cast<Food *>(DropTemporary(work));
            ReadDropFood(food, dropped_item);
            register Food * icon_food asm("r0") = static_cast<Food *>(DropTemporary(work));
            asm("" : "+l"(icon_food));
            icon = static_cast<u16>(icon_food->GetIconId());
            break;
        }
        case RucksackItem::KIND_ARTICLE:
        {
            register u8 raw_article asm("r0") = ReadDropArticle(dropped_item);
            register Article * article asm("r4") = reinterpret_cast<Article *>(&work.article);
            asm("" : "+l"(article) : "l"(raw_article));
            *reinterpret_cast<u8 *>(article) = raw_article;
            if (article->GetId() == 0x1B)
                goto special_article;
            icon = static_cast<u16>(article->GetIconId());
            break;
        }
        }
    }
    goto display_drop;
special_article:
    {
        register Location const * island asm("r1") = &target;
        asm("" : "+l"(island));
        i32 map = island->GetMap();
        asm("" : "+l"(map));
        icon = 3;
        asm("" : "+l"(icon));
        if (map <= 8) icon = 4;
        goto display_drop;
    }
wrapped:
    icon = 0x160;
display_drop:
    register Location const * xs asm("r2") = &target;
    asm("" : "+l"(xs));
    register u32 low_x asm("r0") = reinterpret_cast<u8 const *>(xs)[1] >> 2;
    asm("" : "+l"(low_x) : "l"(xs));
    register u32 high_x asm("r2") = reinterpret_cast<u16 const *>(xs)[1];
    asm("" : "+l"(high_x) : "l"(low_x));
    register u32 mask asm("r3") = 0x3FF;
    asm("" : "+l"(mask) : "l"(high_x));
    register u32 x asm("r2") = ((high_x & mask) << 6) | low_x;
    asm("" : "+l"(x));
    register Location const * ys asm("r4") = &target;
    asm("" : "+l"(ys));
    register u32 low_y asm("r1") = reinterpret_cast<u8 const *>(ys)[3] >> 2;
    asm("" : "+l"(low_y) : "l"(ys));
    register u32 high_y asm("r0") = reinterpret_cast<u16 const *>(ys)[2];
    asm("" : "+l"(high_y) : "l"(low_y), "l"(mask));
    register i32 y asm("r0") = static_cast<i16>(((high_y & mask) << 6) | low_y);
    asm("" : "+l"(y));
    register i16 * destination asm("r1") = work.position;
    asm("" : "+l"(destination) : "l"(y), "l"(x));
    destination[0] = x;
    register i16 * position_ptr asm("r1") = work.position_ptr;
    asm("" : "+l"(position_ptr) : "l"(y));
    position_ptr[1] = y;
    register GameStateMapOwner * archive_owner asm("r2") = &game;
    asm("" : "+l"(archive_owner));
    register GameStatePackedSpriteArchive const * result asm("r0") = archive_owner->vtable->get_vase_archive(game);
    asm("" : "+l"(result));

    register GameStateDropDisplay * display asm("r4") =
        reinterpret_cast<GameStateDropDisplay *>(reinterpret_cast<u8 *>(&game) + 0x102C);
    asm("" : "+l"(display) : "l"(result));
    register GameStatePackedSpriteArchive const * archive asm("r6") = result;
    u32 clip = icon;
    register u32 position_bits asm("r0");
    asm("ldr %0, %1" : "=l"(position_bits) : "m"(*reinterpret_cast<u32 *>(work.position)), "l"(display), "l"(archive), "l"(clip));
    *reinterpret_cast<u32 *>(&display->position) = position_bits;
    register GameStateMapInteraction * empty asm("r5") = nullptr;
    asm("" : "+l"(empty) : "l"(display), "l"(clip), "l"(archive));
    register GameStateMapInteraction * previous asm("r1") = display->sprite;
    asm("" : "+l"(previous));
    if (empty != previous)
    {
        asm volatile("cmp %0, #0\n\tbeq 0f" : : "l"(previous) : "cc");
        delete previous;
        asm volatile("0:" : : : "memory");
    }
    display->sprite = empty;
    register u8 const * blocked_ptr asm("r3") = reinterpret_cast<u8 const *>(&work.blocked);
    asm("" : "+l"(blocked_ptr));
    register u8 blocked_byte asm("r3") = *blocked_ptr;
    asm("" : "+l"(blocked_byte));
    display->terrain_blocked = blocked_byte;
    register u32 sprite_state asm("r5") = work.blocked;
    asm("" : "+l"(sprite_state) : "l"(display), "l"(clip), "l"(archive));
    if (sprite_state)
    {
        register GameStateMapInteraction * sprite asm("r0") = AllocateMapInteraction();
        asm volatile(
            ".ifnes \"%1\", \"[sp, #0x34]\"\n\t"
            ".error \"Drop sprite arguments must start at sp\"\n\t"
            ".endif\n\t"
            "movs r1, #2\n\tstr r1, [sp]\n\t"
            "movs r1, #3\n\tstr r1, [sp, #4]\n\t"
            "movs r1, #0\n\tldr r2, %1\n\tstrb r1, [r2]"
            : "+l"(sprite) : "m"(work.start_ptr) : "r1", "r2", "memory");
        register GameStatePackedSpriteArchive const * archive_arg asm("r1") = archive;
        asm("" : "+l"(archive_arg) : "l"(sprite));
        register u32 clip_arg asm("r2") = clip;
        asm("" : "+l"(clip_arg) : "l"(archive_arg), "l"(sprite));
        register GameStateMapOwner * owner_arg asm("r3") = &game;
        asm("" : "+l"(owner_arg) : "l"(clip_arg), "l"(archive_arg), "l"(sprite));
        register GameStateMapInteraction * next asm("r5") = InitializePreparedDropSprite(sprite, archive_arg, clip_arg, *owner_arg);
        asm("" : "+l"(next), "+l"(display));
        register GameStateMapInteraction * old asm("r1");
        asm("ldr %0, [%1]" : "=l"(old) : "l"(display), "l"(next) : "memory");
        if (next != old)
        {
            asm volatile("cmp %0, #0\n\tbeq 0f" : : "l"(old) : "cc");
            delete old;
            asm volatile("0:" : : : "memory");
        }
        asm("str %1, [%0]" : : "l"(display), "l"(next) : "memory");
        register u32 frame asm("r1") = 0;
        asm("" : "+l"(frame));
        register u32 height asm("r0") = 16;
        asm("" : "+l"(height) : "l"(frame));
        display->height = height;
        display->frame = frame;
        return height;
    }
    register GameStateMapInteraction * sprite asm("r0") = AllocateMapInteraction();
    asm volatile(
        ".ifnes \"%1\", \"[sp, #0x34]\"\n\t"
        ".error \"Drop sprite arguments must start at sp\"\n\t"
        ".endif\n\t"
        "movs r1, #2\n\tstr r1, [sp]\n\t"
        "movs r1, #3\n\tstr r1, [sp, #4]\n\t"
        "ldr r3, %1\n\tstrb %2, [r3]"
        : "+l"(sprite) : "m"(work.start_ptr), "l"(sprite_state) : "r1", "r3", "memory");
    register GameStatePackedSpriteArchive const * archive_arg asm("r1") = archive;
    asm("" : "+l"(archive_arg) : "l"(sprite));
    register u32 clip_arg asm("r2") = 0x1A9;
    asm("" : "+l"(clip_arg) : "l"(archive_arg), "l"(sprite));
    register GameStateMapOwner * owner_arg asm("r3") = &game;
    asm("" : "+l"(owner_arg) : "l"(clip_arg), "l"(archive_arg), "l"(sprite));
    SetDropSprite(*display, InitializePreparedDropSprite(sprite, archive_arg, clip_arg, *owner_arg));
    return 0;
}
