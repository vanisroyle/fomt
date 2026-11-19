#include "prelude.h"

#include "unknown_types.hh"

struct MapUpdateState;
struct MapSceneEntity;

struct MapSceneEntityMethods
{
    STRUCT_PAD(0x00, 0x08);
    void (* destroy)(MapSceneEntity &, int);
};

struct MapSceneEntity
{
    STRUCT_PAD(0x00, 0x14);
    MapSceneEntityMethods const * methods;
};

struct MapScene
{
    STRUCT_PAD(0x00, 0x04);
    MapUpdateState * map;
    MapSceneEntity * entities[100];
};

EC u32 func_0803A8A4(u32 mapId, u32 season, u32 houseLevel, u32 coopLevel, u32 barnLevel);
EC MapData const * GetMapData(u32 graphicsId);
EC void func_080CFFC0(MapUpdateState & map, int column, int row, u32 const & value);
EC void ApplyMapBuildingDoorPatch(MapUpdateState & map, u32 useUpperPosition);
EC void ApplyMapRow17PatchB(MapUpdateState & map);
EC void ApplyMapRow17PatchA(MapUpdateState & map);
EC void ApplyMapIndexedFixturePatch(MapUpdateState & map, u32 columnIndex);
EC void func_080A607C(MapUpdateState & map, u32 tileIndex);
EC void ApplyMapRow2PatchB(MapUpdateState & map);
EC void ApplyMapRow2PatchA(MapUpdateState & map);
EC void ApplyMapRow3PatchB(MapUpdateState & map);
EC void ApplyMapRow3PatchA(MapUpdateState & map);
EC void ApplyMapRow15PatchB(MapUpdateState & map);
EC void ApplyMapRow15PatchA(MapUpdateState & map);
EC void StartMapTransitionDelay(u8 * self);
EC bool IsMapTransitionIdle(u8 * self);
EC void func_080A59BC(MapUpdateState & map, int x, int y, u32 pixelsPerStep);
EC void func_080A5960(MapUpdateState & map, int x, int y);

EC SECTION(".text.080AD938") void SetMapGridCell(MapScene & scene, int column, int row, u32 const & value)
{
    func_080CFFC0(*scene.map, column, row, value);
}

EC SECTION(".text.080AD944") void ApplyMapPatchAtX1(MapScene & scene, u32 useUpperPosition)
{
    ApplyMapBuildingDoorPatch(*scene.map, useUpperPosition);
}

EC SECTION(".text.080AD950") int ApplyMapPatchAtY17VariantA(MapScene & scene)
{
    ApplyMapRow17PatchB(*scene.map);
    return 0;
}

EC SECTION(".text.080AD960") int ApplyMapPatchAtY17VariantB(MapScene & scene)
{
    ApplyMapRow17PatchA(*scene.map);
    return 0;
}

EC SECTION(".text.080AD970") void ApplyMapPatchFromColumnIndex(MapScene & scene, u32 columnIndex)
{
    ApplyMapIndexedFixturePatch(*scene.map, columnIndex);
}

EC SECTION(".text.080AD97C") void ApplyMapPatchFromTileIndex(MapScene & scene, u32 tileIndex)
{
    func_080A607C(*scene.map, tileIndex);
}

EC SECTION(".text.080AD988") void ApplyMapPatchAtY2VariantB(MapScene & scene)
{
    ApplyMapRow2PatchB(*scene.map);
}

EC SECTION(".text.080AD994") void ApplyMapPatchAtY2VariantA(MapScene & scene)
{
    ApplyMapRow2PatchA(*scene.map);
}

EC SECTION(".text.080AD9A0") void ApplyMapPatchAtY3VariantB(MapScene & scene)
{
    ApplyMapRow3PatchB(*scene.map);
}

EC SECTION(".text.080AD9AC") void ApplyMapPatchAtY3VariantA(MapScene & scene)
{
    ApplyMapRow3PatchA(*scene.map);
}

EC SECTION(".text.080AD9B8") void ApplyMapPatchAtY15VariantB(MapScene & scene)
{
    ApplyMapRow15PatchB(*scene.map);
}

EC SECTION(".text.080AD9C4") void ApplyMapPatchAtY15VariantA(MapScene & scene)
{
    ApplyMapRow15PatchA(*scene.map);
}

EC SECTION(".text.080AD9D0") void StartMapTileAnimationDelay(MapScene & scene)
{
    StartMapTransitionDelay(reinterpret_cast<u8 *>(scene.map));
}

EC SECTION(".text.080ADB78") bool IsMapScrollIdle(MapScene & scene)
{
    return IsMapTransitionIdle(reinterpret_cast<u8 *>(scene.map));
}

EC SECTION(".text.080ADB9C") void ScrollMapTowardPixel(MapScene & scene, int x, int y, u32 pixelsPerStep)
{
    func_080A59BC(*scene.map, x, y, pixelsPerStep);
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
