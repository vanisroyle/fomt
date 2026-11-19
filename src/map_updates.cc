#include "prelude.h"

#include "barn.hh"
#include "bachelorette.hh"
#include "coop.hh"
#include "farm_house.hh"
#include "item.hh"
#include "unknown_types.hh"

struct MapPatchTiles;
struct BacheloretteRoster;
struct Unk_0800E324;
struct MapPatch
{
    MapPatchTiles const * tiles;
    unsigned tileX;
    unsigned tileY;
};

struct MapUpdateSave
{
    STRUCT_PAD(0x0000, 0x0010);
    Unk_0800E324 const & GetCalendar() const
    {
        return *reinterpret_cast<Unk_0800E324 const *>(reinterpret_cast<u8 const *>(this) + 0x10);
    }

    STRUCT_PAD(0x0010, 0x01F4);
    FarmHouse house;
    u8 padAfterHouse[0x0410 - 0x01F4 - sizeof(FarmHouse)];
    Coop coop;
    u8 padAfterCoop[0x05F0 - 0x0410 - sizeof(Coop)];
    Barn barn;
    u8 padAfterBarn[0x1C34 - 0x05F0 - sizeof(Barn)];
    ToolStack equippedTool;
    STRUCT_PAD(0x1C36, 0x1C38);
    u8 rucksack[0x1CD4 - 0x1C38];
    u8 bachelorettes[0x34D8 - 0x1CD4];
    u32 tilePatchFlags;
};

struct MapUpdateState
{
    u32 mapId;
    STRUCT_PAD(0x04, 0x29);
    bool tilesDirty;
    STRUCT_PAD(0x2A, 0x94);
    MapUpdateSave * save;
    bool refreshRequested;
};

EC u32 func_0800E324(Unk_0800E324 const & calendar);
EC MapPatch & GetMapPatch(MapPatch & result, u32 const & flags, unsigned index, bool isWinter);
EC void ApplyMapPatch(MapUpdateState & map, MapPatchTiles const * tiles, u32 x, u32 y);
EC u32 func_0803A8A4(u32 mapId, u32 season, u32 houseLevel, u32 coopLevel, u32 barnLevel);
EC MapPatchTiles const gUnk_08106DAC;
EC u32 const gUnk_08107438[6];
EC bool HasRosterEntry(void * self);
EC u32 GetRosterProposalCandidate(BacheloretteRoster const & roster);
EC Bachelorette const * GetRosterBacheloretteById(BacheloretteRoster const & roster, u32 npcId);
EC u32 GetFirstSlotWithTool__C8RucksackUi(void const * rucksack, u32 toolId);
EC u32 GetFirstSlotWith__C9ToolChestUi(void const * chest, u32 toolId);

EC SECTION(".text.080AA850") void ApplySavedMapTilePatch(MapUpdateState & map, u32 patchId)
{
    u32 season = func_0800E324(map.save->GetCalendar());
    MapPatch patch;
    GetMapPatch(patch, map.save->tilePatchFlags, patchId, season == SEASON_WINTER);
    ApplyMapPatch(map, patch.tiles, patch.tileX, patch.tileY);
    map.tilesDirty = true;
}

EC SECTION(".text.080AA89C") void ClearSupermarketRucksackDisplay(MapUpdateState & map)
{
    ApplyMapPatch(map, &gUnk_08106DAC, 33, 20);
    map.tilesDirty = true;
}

EC SECTION(".text.080AA8BC") void ClearSupermarketBlueFeatherDisplay(MapUpdateState & map)
{
    ApplyMapPatch(map, &gUnk_08106DAC, 33, 23);
    map.tilesDirty = true;
}

EC SECTION(".text.080AA8DC") void RequestMap29Refresh(MapUpdateState & map)
{
    if (map.mapId == 29)
        map.refreshRequested = true;
}

EC SECTION(".text.080AA8F0") bool CanStockBlueFeather(MapUpdateState const & map)
{
    MapUpdateSave * const & save = map.save;
    BacheloretteRoster const & roster = *reinterpret_cast<BacheloretteRoster const *>(save->bachelorettes);
    if (HasRosterEntry(save->bachelorettes) || GetRosterProposalCandidate(roster) != 0)
        return false;

    ToolStack const & equippedTool = save->equippedTool;
    bool hasFeather = false;
    if (!equippedTool.IsEmpty() && equippedTool.GetTool().GetId() == TOOL_BLUE_FEATHER)
        goto foundFeather;
    {
        u32 slot = GetFirstSlotWithTool__C8RucksackUi(save->rucksack, TOOL_BLUE_FEATHER);
        if (slot == static_cast<u32>(-1)
            && GetFirstSlotWith__C9ToolChestUi(reinterpret_cast<u8 const *>(save) + 0x380, TOOL_BLUE_FEATHER) == slot)
            goto checkFeather;
    }
foundFeather:
    hasFeather = true;
checkFeather:
    asm("" : "+r"(hasFeather));
    if (hasFeather)
        return false;

    struct CandidateIds
    {
        u32 ids[6];
    };
    CandidateIds candidates = *reinterpret_cast<CandidateIds const *>(gUnk_08107438);
    for (u32 i = 0; i < ARRAY_COUNT(candidates.ids); ++i)
    {
        Bachelorette const * candidate = GetRosterBacheloretteById(roster, candidates.ids[i]);
        if (candidate != 0 && candidate->GetLove() >= 50000 && candidate->GetRivalEventCount() <= 3)
            return true;
    }
    return false;
}

struct MapTileCacheNode
{
    u32 color;
    MapTileCacheNode * parent;
    MapTileCacheNode * left;
    MapTileCacheNode * right;
    int key;
    u32 value;
};

struct MapTileCacheTree
{
    u32 bookkeeping;
    MapTileCacheNode * header;
    u32 size;
};

struct MapTileCache
{
    u32 nextTile;
    u32 nextPalette;
    MapTileCacheTree index;
};

EC void func_080DC57C(MapTileCacheTree & tree, MapTileCacheNode * root);
EC u16 const gUnk_08107458[][2];

EC SECTION(".text.080AAE14") void ResetMapTileCache(MapTileCache & cache, u32 firstTile, u32 firstPalette)
{
    cache.nextTile = firstTile;
    cache.nextPalette = firstPalette;
    MapTileCacheTree & tree = cache.index;
    if (tree.size != 0)
    {
        func_080DC57C(tree, tree.header->parent);
        tree.header->left = tree.header;
        tree.header->parent = 0;
        tree.header->right = tree.header;
        tree.size = 0;
    }
}

EC SECTION(".text.080AAF10") MapTileCacheNode * GetFirstMapTileCacheNode(MapTileCache const & cache)
{
    MapTileCacheNode * iterator = cache.index.header->left;
    asm("" : : "m"(iterator));
    return iterator;
}

struct SavedMapPatchRecord
{
    u32 words[4];
};

EC u32 GetMapStateBit(u8 *, u32, u32);
EC void func_080A1CBC(SavedMapPatchRecord &, u8 *, unsigned);
EC void func_080AA9C8(MapUpdateState &, SavedMapPatchRecord const &);
EC u8 const gUnk_08107450[][2];

static inline MapTileCache & GetMapTileCacheForRefresh(MapUpdateState * map)
{
    register MapUpdateState * base asm("r0") = map;
    asm ("" : "+l" (base));
    register MapTileCache * cache asm("r0") = reinterpret_cast<MapTileCache *>(reinterpret_cast<u8 *>(base) + 0x9C);
    asm ("" : "+l" (cache));
    return *cache;
}

EC SECTION(".text.080AACB8") void RefreshMapTileCacheForState(MapUpdateState & state)
{
    register MapUpdateState * map asm("r5") = &state;
    asm ("" : "+l" (map));
    register unsigned season asm("r8") = func_0800E324(state.save->GetCalendar());
    asm ("" : "+h" (season));
    unsigned mapId = reinterpret_cast<u32 *>(map)[1];
    register u32 firstTile asm("r1");
    register MapTileCache * cache asm("r0");
    switch (mapId)
    {
    case 14:
        cache = &GetMapTileCacheForRefresh(map);
        firstTile = 968;
        goto resetCache;
    case 15:
        cache = &GetMapTileCacheForRefresh(map);
        firstTile = 996;
        goto resetCache;
    case 8:
        cache = &GetMapTileCacheForRefresh(map);
        firstTile = 704;
        goto resetCache;
    case 4: ResetMapTileCache(GetMapTileCacheForRefresh(map), 998, 12); break;
    case 10:
    case 12:
        cache = &GetMapTileCacheForRefresh(map);
        firstTile = 992;
        goto resetCache;
    case 11:
    case 13:
        cache = &GetMapTileCacheForRefresh(map);
        firstTile = 692;
    resetCache:
        ResetMapTileCache(*cache, firstTile, 9);
        break;
    case 0:
        ResetMapTileCache(GetMapTileCacheForRefresh(map), 0, 0);
        if (season == 2)
            break;
        return;
    default:
        ResetMapTileCache(GetMapTileCacheForRefresh(map), 0, 0);
        return;
    }

    u8 * patches = reinterpret_cast<u8 *>(map->save) + 0x34C8;
    register u8 const * ranges asm("r0") = &gUnk_08107450[0][0];
    asm ("" : "+l" (ranges));
    register unsigned seasonIndex asm("r2") = season;
    asm ("" : "+l" (seasonIndex));
    register unsigned rangeOffset asm("r1") = seasonIndex * sizeof(gUnk_08107450[0]);
    asm ("" : "+l" (rangeOffset));
    register u8 const * startIndex asm("r2");
    asm ("add %0, %1, %2" : "=l" (startIndex) : "l" (rangeOffset), "l" (ranges) : "cc");
    ++ranges;
    asm ("" : "+l" (ranges));
    register u8 const * endIndex asm("r1");
    asm ("add %0, %1, %2" : "=l" (endIndex) : "l" (rangeOffset), "l" (ranges) : "cc");
    unsigned end = *endIndex;
    unsigned index = *startIndex;
    SavedMapPatchRecord patch;
    while (index < end)
    {
        if (static_cast<u8>(GetMapStateBit(patches, index, season)))
        {
            func_080A1CBC(patch, patches, index);
            func_080AA9C8(*map, patch);
        }
        ++index;
    }
    for (index = 96; index < 116; ++index)
    {
        if (static_cast<u8>(GetMapStateBit(patches, index, season)))
        {
            func_080A1CBC(patch, patches, index);
            func_080AA9C8(*map, patch);
        }
    }
}

EC SECTION(".text.080AAF1C") MapTileCacheNode * GetEndMapTileCacheNode(MapTileCache const & cache)
{
    MapTileCacheNode * iterator = cache.index.header;
    asm("" : : "m"(iterator));
    return iterator;
}

EC SECTION(".text.080AAF28") u16 GetMapTileVariantId(u32 index, u8 variant)
{
    return gUnk_08107458[index][variant];
}

EC SECTION(".text.080AAF3C") u32 SelectMapGraphicsId(MapUpdateState const & map)
{
    u32 season = func_0800E324(map.save->GetCalendar());
    u32 houseLevel = map.save->house.GetUpgradeLevel();
    u32 coopLevel = map.save->coop.GetUpgradeLevel();
    u32 barnLevel = map.save->barn.GetUpgradeLevel();
    return func_0803A8A4(map.mapId, season, houseLevel, coopLevel, barnLevel);
}
