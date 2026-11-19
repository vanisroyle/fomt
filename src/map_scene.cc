#include "prelude.h"

#include "unknown_types.hh"

struct MapUpdateState;
struct G1MapState;
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

struct SpriteAnimationClipHeader;

struct MapSceneAnimationSlot
{
    STRUCT_PAD(0x00, 0x04);
    SpriteAnimationClipHeader const * clips;
    STRUCT_PAD(0x08, 0x30);
};

struct MapSceneOverlay;

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
};

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
