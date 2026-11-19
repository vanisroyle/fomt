#include "prelude.h"

#include "entity.hh"
#include "farm.hh"

struct GameStateAudioList;
struct GameStateMapEffect;
struct GameStatePlayerData;
struct GameStateScript;
struct Unk_0800E324;

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
    STRUCT_PAD(0x0, 0x10);
    u8 clock[4];
    Farm farm;
    u8 reserved[0x34C4 - 0x14 - sizeof(Farm)];
    bool player_only_when_disabled;
    STRUCT_PAD(0x34C5, 0x34D8);
    u32 tile_variant_flags;
};

struct GameStateMapOwner;
struct GameStateMapOverlay;

struct GameStateMapInteraction
{
    struct Vtable
    {
        STRUCT_PAD(0x0, 0x8);
        void (*destroy)(GameStateMapInteraction * interaction, u32 flags);
    };

    STRUCT_PAD(0x0, 0x24);
    Vtable const * vtable;
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

struct GameStateEntitySlotVtable
{
    STRUCT_PAD(0x0, 0x40);
    AEntity * (*get_entity)(GameStateMapOwner & game, u32 slot);
    AEntity const * (*get_const_entity)(GameStateMapOwner const & game, u32 slot);
    STRUCT_PAD(0x48, 0x140);
    void (*queue_event)(GameStateMapOwner & game, GameStateScriptEvent const & event, u32 queue);
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
    GameStateMapEffect * map_effect;
    STRUCT_PAD(0x1028, 0x1038);
    GameStateMapData * map_data;
    bool entities_enabled;
    STRUCT_PAD(0x103D, 0x1054);
    GameStateScript * script;
    GameStateMapInteraction * interaction;
    STRUCT_PAD(0x105C, 0x107C);
    GameStateMapOverlay * map_overlay;
    STRUCT_PAD(0x1080, 0x10C8);
    bool day_update_requested;
    bool skip_day_update_events;
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
EC void SetAlternateMapPatch(u32 & flags, u32 index);
EC void ClearAlternateMapPatch(u32 & flags, u32 index);
EC void func_080AA850(GameStateMapView & view, u32 index);
EC MapData const * GetMapData(u32 map_variant);
EC u32 func_0803A8A4(u32 map, u32 season, u32 house_level, u32 coop_level, u32 barn_level);
EC u32 func_0800E324(Unk_0800E324 const & clock);
EC bool func_080A5EB8(GameStateMapView const & view, Location const & location);
EC u8 * GameScript_GetSoundPlayers(GameStateScript & script);
EC void func_08012B24(GameStateScript & script, GameStateScriptEvent const & event, u32 queue);
EC void func_0801F3E4(GameStateMapOwner & game);
EC bool SaveData_CollectShippingValue(GameStateMapData & state);
EC bool func_0801C0F8(GameStateMapOwner & game, Location const & location, GameStateFarmPlot & result);

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
    return GetMapData(func_0803A8A4(map, season, house_level, coop_level, barn_level))->width * 8;
}

EC SECTION(".text.game_state_c2.08019FE8") u32 GameStateGetMapHeightPixels(GameStateMapOwner const & game, u32 map)
{
    GameStateMapData * const & state = game.map_data;
    u32 season = func_0800E324(reinterpret_cast<Unk_0800E324 const &>(state->clock));
    u32 house_level = state->farm.farm_house.GetUpgradeLevel();
    u32 coop_level = state->farm.coop.GetUpgradeLevel();
    u32 barn_level = state->farm.barn.GetUpgradeLevel();
    return GetMapData(func_0803A8A4(map, season, house_level, coop_level, barn_level))->height * 8;
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

EC SECTION(".text.game_state_c2.0801C0E0") void GameStateRequestDayUpdate(GameStateMapOwner & game, bool skip_events)
{
    game.day_update_requested = true;
    game.skip_day_update_events = skip_events;
}

EC SECTION(".text.game_state_c2.0801C3DC") void GameStateNoOpMapAction(GameStateMapOwner &)
{
}

EC SECTION(".text.game_state_c2.0801D88C") GameStatePlotPlacement GameStateCheckFarmPlotPlacement(
    GameStateMapOwner & game, Location const & location, Article const & article)
{
    GameStateFarmPlot result;
    if (func_0801C0F8(game, location, result))
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
    func_08012B24(*game.script, event, queue);
}

EC SECTION(".text.game_state_c2.0801DB44") GameStateAudioList * GameStateGetAudioList(GameStateMapOwner & game)
{
    return reinterpret_cast<GameStateAudioList *>(GameScript_GetSoundPlayers(*game.script));
}

EC SECTION(".text.game_state_c2.0801DCA8") void GameStateClearMapInteraction(GameStateMapOwner & game)
{
    GameStateMapInteraction * & interaction = game.interaction;
    GameStateMapInteraction * replacement = nullptr;
    if (replacement != interaction && interaction != nullptr)
        interaction->vtable->destroy(interaction, 3);
    interaction = replacement;
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

EC SECTION(".text.game_state_c2.0801F5F0") void GameStateSetVaseArticle(GameStateMapOwner & game, u32 article_id)
{
    game.map_data->farm.farm_house.SetVaseArticleId(article_id);
    func_0801F3E4(game);
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
