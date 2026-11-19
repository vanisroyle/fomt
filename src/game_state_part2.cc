#include "prelude.h"

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

struct GameStateMapOwner
{
    void * vtable;
    GameStateMapView * map_view;
    STRUCT_PAD(0x8, 0x1038);
    GameStateMapData * map_data;
};

EC void func_080A6138(GameStateMapView & view);
EC void func_080A6194(GameStateMapView & view);
EC void func_080A61F0(GameStateMapView & view);
EC void func_080A6234(GameStateMapView & view);
EC void func_080A6278(GameStateMapView & view);
EC void func_080A62BC(GameStateMapView & view);
EC void func_080A607C(GameStateMapView & view, u32 index);
EC void func_080A6024(GameStateMapView & view, u32 index);
EC void StartMapTransitionDelay(GameStateMapView & view);
EC void func_080A63B8(GameStateMapView & view, u32 alternate);
EC void func_080AA89C(GameStateMapView & view);
EC void func_080AA8BC(GameStateMapView & view);
EC void func_080AA8DC(GameStateMapView & view);
EC bool IsMapTransitionIdle(GameStateMapView & view);
EC void func_080A59BC(GameStateMapView & view, i32 x, i32 y, u32 speed);
EC void func_080A5960(GameStateMapView & view, i32 x, i32 y);
EC void SetAlternateMapPatch(u32 & flags, u32 index);
EC void ClearAlternateMapPatch(u32 & flags, u32 index);
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
    StartMapTransitionDelay(*game.map_view);
}

EC SECTION(".text.game_state_c2.0801DDCC") void GameStateSetMapTileVariant(GameStateMapOwner & game, u32 index)
{
    SetAlternateMapPatch(game.map_data->tile_variant_flags, index);
    func_080AA850(*game.map_view, index);
}

EC SECTION(".text.game_state_c2.0801DDF8") void GameStateClearMapTileVariant(GameStateMapOwner & game, u32 index)
{
    ClearAlternateMapPatch(game.map_data->tile_variant_flags, index);
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
    func_080A59BC(*game.map_view, x, y, speed);
}

EC SECTION(".text.game_state_c2.0801FD6C") void GameStateSetCameraCenter(GameStateMapOwner & game, i32 x, i32 y)
{
    func_080A5960(*game.map_view, x, y);
}
