#include "prelude.h"

#include "entity.hh"
#include "farm.hh"
#include "farmer.hh"

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
    STRUCT_PAD(0x18, 0x40);
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
    STRUCT_PAD(0x10B0, 0x10C8);
    bool day_update_requested;
    bool skip_day_update_events;
};

EC void DestroyGameStateMapOverlay(GameStateMapOverlay * overlay, u32 destructor_flags);
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
EC u8 * GameScript_GetSoundPlayers(GameStateScript & script);
EC void func_08012B24(GameStateScript & script, GameStateScriptEvent const & event, u32 queue);
EC bool SaveData_CollectShippingValue(GameStateMapData & state);
EC void func_0801A13C(GameStateMapOwner & game, u32 hour, u32 minute, bool force);
EC void GameScript_ConfigureMusic(GameStateScript & script, u32 player, u32 track, GameStateMusicParams const & params);
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
EC AEntity * SpawnSoldier(void *, GameStateMapOwner &, u32, void *, Farm &) asm("func_08033928");
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
EC AEntity * MakeSpecialFieldActor(void *, GameStateMapOwner &, void *) asm("func_080398A4");
EC AEntity * MakeActiveSceneEffect(GameStateMapOwner &, void *) asm("CreateActiveSideEffectEntity");
EC AEntity * MakeUpdateEffect(void *, GameStateMapOwner &, void *) asm("InitializeSideEffectEntity");
EC AEntity * MakeRecordedActor(void *, GameStateMapOwner &, void *) asm("InitializeRecordedSideActor");
EC void * FindArrivalNpc(void *) asm("GetActiveScheduleNpc");
EC void * FindDayNpc(void *, u32) asm("GetNpcScheduleDaySlot");
EC void * FindNightNpc(void *, u32) asm("GetNpcScheduleNightSlot");
EC void * FindCurrentNpc(void *) asm("GetNpcScheduleCurrentSlot");
EC void * FindPreviousNpc(void *) asm("GetNpcSchedulePreviousSlot");
EC AEntity * MakeDayNpc(GameStateMapOwner &, void *, u32) asm("func_08037A5C");
EC AEntity * MakeNightNpc(GameStateMapOwner &, void *, u32) asm("func_08037AD0");
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
            func_080AA6D0(*view, tile_x, tile_y, plot, adjacent);
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
        func_080AA6D0(*view, x * 2 + 34, y * 2 + 22, plot, adjacent);
    }
    return result;
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
            func_080AA6D0(*view, plot.tile_x, plot.tile_y, *plot.plot, adjacent);
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
    func_08012B24(*game.script, event, queue);
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
        game.interaction = func_080A4A00(storage, archive, animation, game, 3, 0x1B, true);
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
    game.scenery = func_080A4A00(storage, archive, variant, game, 2, 0x1B, false);
}

static inline GameStateMapInteraction * MakeTimedMapInteraction(GameStateMapOwner & game, u32 animation)
{
    GameStateMapInteraction * storage = static_cast<GameStateMapInteraction *>(::operator new(sizeof(GameStateMapInteraction)));
    return func_080A4A00(storage, &game.sprite_archives[12], animation, game, 5, 0x1B, false);
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
EC void InitGameStateVaseDisplay(GameStateVaseDisplay *, GameStateMapOwner &, u32, u32) asm("func_080A4740");
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
            game.paired_scenery_first = func_080A4A00(first, archive, 0, game, 3, 0x1E, map);
            GameStateMapInteraction * second = AllocateMapInteraction();
            game.paired_scenery_second = func_080A4A00(second, archive, 0, game, 3, 0x1E, false);
        }
        else
        {
            GameStateMapInteraction * first = AllocateMapInteraction();
            register GameStatePackedSpriteArchive const * archive asm("r5") = &game.sprite_archives[7];
            game.paired_scenery_first = func_080A4A00(first, archive, 1, game, season, 0x1E, map);
            GameStateMapInteraction * second = AllocateMapInteraction();
            game.paired_scenery_second = func_080A4A00(second, archive, 1, game, 3, 0x1E, false);
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
                game.alternate_seasonal_scenery = func_080A4A00(AllocateMapInteraction(), &game.sprite_archives[7], 2, game, 2, 0x1F, false);
            if (func_0800E324(reinterpret_cast<Unk_0800E324 const &>(game.map_data->clock)) == SEASON_WINTER)
                game.seasonal_scenery = func_080A4A00(AllocateMapInteraction(), &game.sprite_archives[7], 2, game, 2, 0x1F, false);
            break;
        case 3:
            if (func_0800E324(reinterpret_cast<Unk_0800E324 const &>(game.map_data->clock)) == SEASON_WINTER)
                game.seasonal_scenery = func_080A4A00(AllocateMapInteraction(), &game.sprite_archives[7], 2, game, 2, 0x1F, false);
            break;
        case 0:
            if (reinterpret_cast<u8 const *>(game.map_data)[3] >> 7)
                game.seasonal_scenery = func_080A4A00(AllocateMapInteraction(), &game.sprite_archives[7], 2, game, 2, 0x1F, map);
            break;
        case 2:
            if (game.map_data->farm.farm_house.IsFireplaceLighted())
                game.seasonal_scenery = func_080A4A00(AllocateMapInteraction(), &game.sprite_archives[7], 2, game, map, 0x1F, false);
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
EC GameStateMapObject * func_080175B4(GameStateMapObject * map, GameStateMapData & state, GameStateScript * script,
    GameStateSavedMapView * & view, bool restore_events);

EC SECTION(".text.game_state_c2.0801FB7C") GameStateMapHandle * GameStateCreateMapObject(
    GameStateMapHandle * result, GameStateMapData & state, GameStateScript * script, bool restore_events)
{
    GameStateSavedMapView * view = InitializeWorldMapBuffers(
        static_cast<GameStateSavedMapView *>(::operator new(sizeof(GameStateSavedMapView))), state);
    GameStateMapHandle map(func_080175B4(
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
