#include "prelude.h"

#include "entity.hh"

struct GameStateCameraCoordinate
{
    u16 fraction;
    i16 pixel;
};

struct GameStateMapView
{
    u32 id;
    u32 map;
    GameStateCameraCoordinate camera_x;
    GameStateCameraCoordinate camera_y;
};

struct GameStateMapData
{
    STRUCT_PAD(0x0, 0x34D8);
    u32 tile_variant_flags;
};

struct GameStateMapOwner;
struct GameStateMapOverlay;

struct GameStateEntitySlotVtable
{
    STRUCT_PAD(0x0, 0x40);
    AEntity * (*get_entity)(GameStateMapOwner & game, u32 slot);
    AEntity const * (*get_const_entity)(GameStateMapOwner const & game, u32 slot);
};

struct GameStateSpriteCacheEntry
{
    STRUCT_PAD(0x0, 0x81);
    bool active;
    STRUCT_PAD(0x82, 0x84);
};

struct GameStateSpriteCache
{
    u8 slot_ids[32];
    GameStateSpriteCacheEntry entries[22];
    STRUCT_PAD(0xB78, 0xC18);
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
    STRUCT_PAD(0x1024, 0x1038);
    GameStateMapData * map_data;
    STRUCT_PAD(0x103C, 0x107C);
    GameStateMapOverlay * map_overlay;
};

EC void func_0803B0A8(GameStateMapOverlay * overlay, u32 destructor_flags);
EC i32 func_080AC96C(GameStateSpriteCache & cache, u32 slot);

EC void func_080A6138(GameStateMapView & view);
EC void func_080A6194(GameStateMapView & view);
EC void func_080A61F0(GameStateMapView & view);
EC void func_080A6234(GameStateMapView & view);
EC void func_080A6278(GameStateMapView & view);
EC void func_080A62BC(GameStateMapView & view);
EC void func_080A607C(GameStateMapView & view, u32 index);
EC void func_080A6024(GameStateMapView & view, u32 index);
EC void func_080A601C(GameStateMapView & view);
EC void func_080A63B8(GameStateMapView & view, u32 alternate);
EC void func_080AA89C(GameStateMapView & view);
EC void func_080AA8BC(GameStateMapView & view);
EC void func_080AA8DC(GameStateMapView & view);
EC bool func_080A5A9C(GameStateMapView & view);
EC void func_080A59BC(GameStateMapView & view, i32 x, i32 y, u32 speed);
EC void func_080A5960(GameStateMapView & view, i32 x, i32 y);
EC void func_0809C5B4(u32 & flags, u32 index);
EC void func_0809C5D0(u32 & flags, u32 index);
EC void func_080AA850(GameStateMapView & view, u32 index);

EC SECTION(".text.game_state_c2.0801DD18") void GameStateApplyRow15TilePatchVariant1(GameStateMapOwner & game)
{
    func_080A6138(*game.map_view);
}

EC SECTION(".text.game_state_c2.0801DD24") void GameStateApplyRow15TilePatchVariant2(GameStateMapOwner & game)
{
    func_080A6194(*game.map_view);
}

EC SECTION(".text.game_state_c2.0801DD30") void GameStateApplyRow3TilePatchVariant1(GameStateMapOwner & game)
{
    func_080A61F0(*game.map_view);
}

EC SECTION(".text.game_state_c2.0801DD3C") void GameStateApplyRow3TilePatchVariant2(GameStateMapOwner & game)
{
    func_080A6234(*game.map_view);
}

EC SECTION(".text.game_state_c2.0801DD48") void GameStateApplyRow2TilePatchVariant1(GameStateMapOwner & game)
{
    func_080A6278(*game.map_view);
}

EC SECTION(".text.game_state_c2.0801DD54") void GameStateApplyRow2TilePatchVariant2(GameStateMapOwner & game)
{
    func_080A62BC(*game.map_view);
}

EC SECTION(".text.game_state_c2.0801DD60") void GameStateApplyMapTilePatchAtIndex(GameStateMapOwner & game, u32 index)
{
    func_080A607C(*game.map_view, index);
}

EC SECTION(".text.game_state_c2.0801DD6C") void GameStateApplyMapTilePatchInColumn(GameStateMapOwner & game, u32 index)
{
    func_080A6024(*game.map_view, index);
}

EC SECTION(".text.game_state_c2.0801DD78") void GameStateStartMapViewTimer(GameStateMapOwner & game)
{
    func_080A601C(*game.map_view);
}

EC SECTION(".text.game_state_c2.0801DDCC") void GameStateSetMapTileVariant(GameStateMapOwner & game, u32 index)
{
    func_0809C5B4(game.map_data->tile_variant_flags, index);
    func_080AA850(*game.map_view, index);
}

EC SECTION(".text.game_state_c2.0801DDF8") void GameStateClearMapTileVariant(GameStateMapOwner & game, u32 index)
{
    func_0809C5D0(game.map_data->tile_variant_flags, index);
    func_080AA850(*game.map_view, index);
}

EC SECTION(".text.game_state_c2.0801DE24") void GameStateApplyMapTilePatchWithAlternateRow(GameStateMapOwner & game, u32 alternate)
{
    func_080A63B8(*game.map_view, alternate);
}

EC SECTION(".text.game_state_c2.0801DE30") void GameStateApplyMapTilePatchAtRow20(GameStateMapOwner & game)
{
    func_080AA89C(*game.map_view);
}

EC SECTION(".text.game_state_c2.0801DE3C") void GameStateApplyMapTilePatchAtRow23(GameStateMapOwner & game)
{
    func_080AA8BC(*game.map_view);
}

EC SECTION(".text.game_state_c2.0801DE48") void GameStateQueueMapEvent1D(GameStateMapOwner & game)
{
    func_080AA8DC(*game.map_view);
}

EC SECTION(".text.game_state_c2.0801FD3C") bool GameStateIsCameraIdle(GameStateMapOwner & game)
{
    return func_080A5A9C(*game.map_view);
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
    func_080A59BC(*game.map_view, x, y, speed);
}

EC SECTION(".text.game_state_c2.0801FD6C") void GameStateSetCameraCenter(GameStateMapOwner & game, i32 x, i32 y)
{
    func_080A5960(*game.map_view, x, y);
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
    GameStateMapOverlay * & overlay = game.map_overlay;
    GameStateMapOverlay * replacement = nullptr;
    if (replacement != overlay && overlay != nullptr)
        func_0803B0A8(overlay, 3);
    overlay = replacement;
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
        sprite_slot = func_080AC96C(cache, slot);
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
