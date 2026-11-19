#include "prelude.h"

#include "entity.hh"
#include "farm.hh"
#include "farmer.hh"

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
    STRUCT_PAD(0x10, 0x2C);
    TerrainInfo const * expanded_terrain;
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

struct GameStateMapData
{
    STRUCT_PAD(0x0, 0x10);
    u8 clock[4];
    Farm farm;
    u8 reserved[0x1BD8 - 0x14 - sizeof(Farm)];
    Farmer farmer;
    u8 reserved_after_farmer[0x34C4 - 0x1BD8 - sizeof(Farmer)];
    bool player_only_when_disabled;
    STRUCT_PAD(0x34C5, 0x34D8);
    u32 tile_variant_flags;
};

struct GameStateMapOwner;
struct GameStatePackedSpriteArchive;

struct GameStateMapOverlay
{
    STRUCT_PAD(0x0, 0x46C);
};

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
    STRUCT_PAD(0x18, 0x40);
    AEntity * (*get_entity)(GameStateMapOwner & game, u32 slot);
    AEntity const * (*get_const_entity)(GameStateMapOwner const & game, u32 slot);
    STRUCT_PAD(0x48, 0x5C);
    GameStatePackedSpriteArchive const * (*get_interaction_archive)(GameStateMapOwner const & game);
    STRUCT_PAD(0x60, 0x140);
    void (*queue_event)(GameStateMapOwner & game, GameStateScriptEvent const & event, u32 queue);
    STRUCT_PAD(0x144, 0x148);
    u32 (*get_music_track)(GameStateMapOwner const & game);
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
    STRUCT_PAD(0x103D, 0x1040);
    GameStateClock const * clock;
    STRUCT_PAD(0x1044, 0x1048);
    u32 daylight_band;
    u8 daylight_amount;
    STRUCT_PAD(0x104D, 0x1050);
    TerrainInfo const * farm_terrain;
    GameStateScript * script;
    SmartPtr<GameStateMapInteraction> interaction;
    STRUCT_PAD(0x105C, 0x107C);
    GameStateMapOverlay * map_overlay;
    STRUCT_PAD(0x1080, 0x10C8);
    bool day_update_requested;
    bool skip_day_update_events;
};

EC void func_0803B0A8(GameStateMapOverlay * overlay, u32 destructor_flags);
EC GameStateMapOverlay * func_0803AEA0(GameStateMapOverlay * overlay, GameStateMapOwner & game,
    GameStatePackedSpriteArchive const & archive, u32 kind, int capacity);
EC i32 func_080AC96C(GameStateSpriteCache & cache, u32 slot);

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
EC void func_080A59BC(GameStateMapView & view, i32 x, i32 y, u32 speed);
EC void func_080A5960(GameStateMapView & view, i32 x, i32 y);
EC void SetAlternateMapPatch(u32 & flags, u32 index);
EC void ClearAlternateMapPatch(u32 & flags, u32 index);
EC void ApplySavedMapTilePatch(MapUpdateState & map, u32 index);
EC MapData const * GetMapData(u32 map_variant);
EC u32 func_0803A8A4(u32 map, u32 season, u32 house_level, u32 coop_level, u32 barn_level);
EC u32 func_0800E324(Unk_0800E324 const & clock);
EC bool func_080A5EB8(GameStateMapView const & view, Location const & location);
EC u8 * func_0801468C(GameStateScript & script);
EC void func_08012B24(GameStateScript & script, GameStateScriptEvent const & event, u32 queue);
EC void func_0801F3E4(GameStateMapOwner & game);
EC bool func_0801140C(GameStateMapData & state);
EC bool func_0801C0F8(GameStateMapOwner & game, Location const & location, GameStateFarmPlot & result);
EC void func_0801A13C(GameStateMapOwner & game, u32 hour, u32 minute, bool force);
EC void func_080123CC(GameStateScript & script, u32 player, u32 track, GameStateMusicParams const & params);
EC void func_080AA6D0(GameStateMapView & view, u32 x, u32 y, FieldPlot const & plot, Unk_Something const * adjacent);
EC void func_0805E860(SpriteAnimator & animator, u32 animation);
EC GameStateMapInteraction * func_080A4A00(GameStateMapInteraction * interaction,
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
        variant = func_0803A8A4(map, season, house_level, coop_level, barn_level);
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
        func_080AA6D0(*view, x * 2 + 34, y * 2 + 22, plot, adjacent);
    }
    return result;
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
    return reinterpret_cast<GameStateAudioList *>(func_0801468C(*game.script));
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
    func_080123CC(*script, 1, track, params);
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
        game.interaction = func_080A4A00(storage, archive, animation, game, 3, 0x1B, true);
    }
}

EC SECTION(".text.game_state_c2.0801DCD4") void GameStateSettleShippingBin(GameStateMapOwner & game)
{
    GameStateMapData * & state = game.map_data;
    u32 proceeds = state->farm.shipping_bin.GetValueShipped();
    func_0801140C(*state);
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

static inline void InstallMapOverlay(GameStateMapOverlay * & overlay, GameStateMapOverlay * replacement)
{
    if (replacement != overlay && overlay != nullptr)
        func_0803B0A8(overlay, 3);
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
        InstallMapOverlay(game.map_overlay, func_0803AEA0(storage, game, *archive, kind, capacity));
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
