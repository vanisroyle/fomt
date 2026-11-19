#include "entity_actor.hh"
#include "dog.hh"
#include "horse.hh"
#include "chicken.hh"
#include "barn_animal.hh"
#include "cow.hh"
#include "sheep.hh"
#include "item.hh"
#include "farmer.hh"
#include "farmer_entity_layout.hh"
#include "entity_sound_channel.hh"
#include "shipping_bin.hh"
#include "coop.hh"
#include <stdlib.h>

struct DogEntity : AActorEntity
{
    DogEntity();
    u32 sound_flags;
    Dog * dog;
    void * unk_38;
    i16 saved_x;
    i16 saved_y;
    bool override_state;
    u8 state;
};

struct HorseEntity : AActorEntity
{
    HorseEntity();
    u32 sound_flags;
    Horse * horse;
    STRUCT_PAD(0x38, 0x3C);
    u8 state;
    bool override_state;
    bool suppress_effect;
};

struct ChickenEntity : AActorEntity
{
    ChickenEntity();
    u32 sound_flags;
    Chicken * chicken;
    u8 state;
};

EC u16 const gUnk_080F09AC[];
EC u16 const gUnk_080F0908[];
EC u16 const gUnk_080F0A68[];
EC u16 const gUnk_080F0C3C[];
EC u16 const gUnk_080F0E34[];
EC void func_080323C8(AEntity * self);
EC void func_0802A588(AEntity * self);

EC bool func_080AC070(void * map, Box const * box);

struct BarnAnimalEntity : AActorEntity
{
    BarnAnimalEntity();
    u32 sound_flags;
    BarnAnimal * animal;
    u8 state;
};

EC void func_08020038(AActorEntity * self, GameObject * game, ActorLocation const * location, u32 speed);
EC u32 ChickenEntity_GetAnimationState(ChickenEntity * self);
EC u32 CowEntity_GetAnimationState(BarnAnimalEntity * self);
EC u32 SheepEntity_GetAnimationState(BarnAnimalEntity * self);
EC void ChickenEntity_SetAnimationState(ChickenEntity * self, u32 state, u32 facing);
EC void CowEntity_SetAnimationState(BarnAnimalEntity * self, u32 state, u32 facing);
EC void SheepEntity_SetAnimationState(BarnAnimalEntity * self, u32 state, u32 facing);
EC char const vtable_unk_080E639C[];
EC char const vtable_unk_080E6310[];
EC char const vtable_unk_080E6284[];
EC void AActorEntity_SetFacingSpeed(AActorEntity * self, i32 speed);
EC u32 DogEntity_GetStateAnimation(DogEntity * self, u32 state);
EC i32 DogEntity_GetStateSpeed(DogEntity * self, u32 state);
EC i32 HorseEntity_GetStateSpeed(HorseEntity * self, u32 state);
EC u32 HorseEntity_GetStateAnimation(HorseEntity * self, u32 state);
EC u32 CowEntity_GetStateAnimation(BarnAnimalEntity * self, u32 state);
EC u32 SheepEntity_GetStateAnimation(BarnAnimalEntity * self, u32 state);
EC u32 CowEntity_GetStateSpeed(AEntity * self, u32 state);
EC u32 SheepEntity_GetStateSpeed(AEntity * self, u32 state);
EC void func_08032384(AEntity * self, u32 effect, u32 argument);
EC void DogEntity_PlaceCarriedTarget(DogEntity * self);
EC u32 DogEntity_GetAnimationState(DogEntity * self);
EC u32 HorseEntity_GetAnimationState(HorseEntity * self);
EC void DogEntity_SetAnimationState(DogEntity * self, u32 state, u32 facing);
EC void HorseEntity_SetAnimationState(HorseEntity * self, u32 state, u32 facing);

struct AnimalBehaviorTimer
{
    u32 time : 16;
    u32 active : 8;
    u32 unused : 8;
};

EC bool FarmerEntity_IsUsingCursedTool(FarmerEntity * self);

EC void CancelFarmerItemDisplay(FarmerEntity * self);
EC void ShowFarmerToolDisplay(FarmerEntity * self);
EC void func_0800F4C0(HeldItem * self);
EC void func_0800F4F0(HeldItem * self, u8 value);
EC void func_0800F4D8(HeldItem * self);
EC u32 func_08020018(u32 value);
EC void AEntity_PlayAnimationSound(AActorEntity * self, u32 sound);
EC void func_0802720C(FarmerEntity * self, Tool const & tool);
EC void func_0802771C(FarmerEntity * self, u32 argument);

struct AnimalBehaviorTimerPair
{
    u32 time;
    u32 unknown;
};

struct CarryableActorEntity : AActorEntity
{
    CarryableActorEntity();
    virtual void vfunc_3C();
    virtual bool vfunc_40();
};

struct EntityLookupGameVTable
{
    STRUCT_PAD(0x00, 0x40);
    CarryableActorEntity * (*get_entity)(GameObject * game, u32 id);
};

inline CarryableActorEntity * GetCarryableEntityById(GameObject * game, u32 id)
{
    EntityLookupGameVTable const * table = *reinterpret_cast<EntityLookupGameVTable const **>(game);
    return table->get_entity(game, id);
}

EC char const vtable_unk_080E6428[];
EC char const vtable_unk_080E64C8[];
EC void StartLaunchedSpriteMovement(CarryableActorEntity * self, i32 x, i32 y, u32 anim, u32 facing);
EC void DogEntity_ResetBehavior(DogEntity * self, u32 time);
EC Article func_0800F258(HeldItem const * self);
EC u32 ChickenEntity_GetStateSpeed(AEntity * self, u32 value);
EC Box func_0803240C(AActorEntity const * self);
EC u32 func_0800F344(HeldItem const * self);
EC void ItemCollection_SetExtraItems(void * self, Location const & location);
EC u32 ChickenEntity_GetStateAnimation(ChickenEntity * self, u32 state);
EC void __builtin_delete(void * ptr);
EC void * __builtin_new(size_t size);
EC UnknownEntityThing * func_0802B260(void * memory, FarmerEntity * self);
struct ScriptEventSelection;
EC bool IsSelectedScriptEvent(ScriptEventSelection const & events, u16 eventId);
EC u32 func_08022320(u32 growth_stage);
EC u32 func_08022334(u32 growth_stage);
EC void func_080324BC(UnknownEntityThing * self, AActorEntity * entity, u32 sprite_kind, u32 palette, u32 size, u32 variant, u32 flags, bool secondary);
EC char const vtable_unk_080E64B4[];

struct FarmerEventStateView
{
    STRUCT_PAD(0x00, 0x2171);
    u8 unk_2171_0 : 3;
    u8 unk_2171_3 : 2;
};

struct EntityGameCalendar
{
    u32 weather;
    STRUCT_PAD(0x04, 0x0A);
    u8 hour : 5;
};

struct EntityCalendarGameVTable
{
    STRUCT_PAD(0x00, 0x144);
    EntityGameCalendar const * (*get_calendar)(GameObject * game);
};

inline u32 GetEntityGameHour(GameObject * game)
{
    EntityCalendarGameVTable const * table = *reinterpret_cast<EntityCalendarGameVTable const **>(game);
    return table->get_calendar(game)->hour;
}
EC Food func_0800F20C(HeldItem const * self);
EC bool func_0800F388(HeldItem const * self);
EC void func_0800F528(HeldItem * self);
EC bool AEntity_IsOnOutdoorMap(AEntity const * self);

struct FarmerGameStateView
{
    STRUCT_PAD(0x00, 0x08);
    u32 weather;
    STRUCT_PAD(0x0C, 0x12);
    GameTime time;
    STRUCT_PAD(0x14, 0x54);
    ShippingBin shipping_bin;
};
EC void StopLaunchedSpriteMovement(CarryableActorEntity * self);
EC FarmerEntity * InitFarmerMapActor(FarmerEntity * self, GameObject * game, void * state);
EC void FarmerEntityInitNoOp(FarmerEntity * self);
EC char const vtable_unk_080E6658[];
EC void func_0802F0EC(FarmerEntity * self, u32 first, u32 second);
EC u16 func_0802D158(FarmerEntity * self);
EC void func_0803242C(AActorEntity * self, u32 argument);

inline i32 GetFarmerAnimationState(FarmerEntity const * self)
{
    return self->state;
}

struct EntityCollisionProbe;
EC void ProbeEntityCollisionDown(EntityCollisionProbe & probe);
EC void ProbeEntityCollisionUp(EntityCollisionProbe & probe);
EC void ProbeEntityCollisionLeft(EntityCollisionProbe & probe);
EC void ProbeEntityCollisionRight(EntityCollisionProbe & probe);

struct EntityMovementBounds
{
    i32 positive_y;
    i32 negative_y;
    i32 negative_x;
    i32 positive_x;
    STRUCT_PAD(0x10, 0x14);
    UnkMap map;
    Box collision_box;
    i32 limit;
    i32 horizontal_clearance;
    i32 vertical_clearance;

    EntityMovementBounds(UnkMap const & a_map, Box const & box, i32 radius, i32 extra_x = 0, i32 extra_y = 0)
        : positive_y(radius + 1), negative_y(-radius - 1), negative_x(-radius - 1), positive_x(radius + 1),
          map(a_map), collision_box(box), limit(radius), horizontal_clearance(extra_x), vertical_clearance(extra_y)
    {
    }

    i32 GetPositiveY()
    {
        if (positive_y > limit)
        {
            ProbeEntityCollisionDown(*reinterpret_cast<EntityCollisionProbe *>(this));
            return positive_y;
        }
        return positive_y;
    }

    i32 GetNegativeY()
    {
        i32 value = negative_y;
        if (value < -limit)
        {
            ProbeEntityCollisionUp(*reinterpret_cast<EntityCollisionProbe *>(this));
            return negative_y;
        }
        return value;
    }

    i32 GetNegativeX()
    {
        i32 value = negative_x;
        if (value < -limit)
        {
            ProbeEntityCollisionLeft(*reinterpret_cast<EntityCollisionProbe *>(this));
            return negative_x;
        }
        return value;
    }

    i32 GetPositiveX()
    {
        if (positive_x > limit)
        {
            ProbeEntityCollisionRight(*reinterpret_cast<EntityCollisionProbe *>(this));
            return positive_x;
        }
        return positive_x;
    }
};

inline i32 GetEntityFacing(AActorEntity const * self)
{
    return self->facing;
}

inline void LaunchFarmerHeldItem(FarmerEntity * self)
{
    switch (GetEntityFacing(self))
    {
    case 2: self->thrown_item.Launch(-0xA0000, 0, 0xC0000); break;
    case 3: self->thrown_item.Launch(0xA0000, 0, 0xC0000); break;
    case 1: self->thrown_item.Launch(0, -0xA0000, 0xC0000); break;
    case 0: self->thrown_item.Launch(0, 0xA0000, 0xC0000); break;
    }
}


inline bool IsEntityCollisionBoxBlocked(void * map, Box const & box)
{
    return func_080AC070(map, &box);
}

#define ENTITY_CODE(address) SECTION(".text.entities_part1." #address)

struct ChickenEntityUpdateContext
{
    u32 unk_00;
    bool minute_tick;
};

struct ChickenBehaviorChoice
{
    u8 weight;
    u8 minimum_seconds;
    u8 additional_seconds;
    u8 unk_03;
};

struct ChickenBehaviorTiming
{
    u32 remaining_frames : 16;
    u32 running_frames : 7;
    u32 follow_dog : 1;
    u32 direction_frames : 8;
};

EC ChickenBehaviorChoice const gUnk_080F09C8[2][2][2][5];
EC u32 RollEntityMovementRandom(u32 bound);
EC u32 ChooseWeightedEntityMovement(ChickenBehaviorChoice const * choices, u32 count);
EC i32 RotateEntityMovementFacing(i32 facing, i32 turn);
EC u32 func_080ABE58(i32 x, i32 y, i32 target_x, i32 target_y, EntityMovementBounds * bounds, u32 facing);
EC void func_080ABA90(EntityMovementBounds * bounds, Box const & box, u32 argument);
EC bool AEntity_IsAnimationFinished(AEntity * self);
EC void AActorEntity_ApplyBoundedMovement(AActorEntity * self, EntityMovementBounds * bounds);

inline u32 ChickenBehaviorDuration(ChickenBehaviorChoice const & choice, u32 frames_per_second)
{
    return choice.minimum_seconds * frames_per_second + RollEntityMovementRandom(choice.additional_seconds * frames_per_second);
}

// 0x080224E4
EC void ChickenEntity_Update(ChickenEntity * self, ChickenEntityUpdateContext const * context) ENTITY_CODE(080224E4);
EC void ChickenEntity_Update(ChickenEntity * self, ChickenEntityUpdateContext const * context)
{
    Chicken * const chicken = self->chicken;
    GameObject * const game = self->game_object;
    u32 const map_id = self->location_map;
    if (map_id == 2 && context->minute_tick)
    {
        u32 before = chicken->GetCurrentOutdoorMinutes();
        chicken->AddOutdoorMinutes(1);
        u32 after = chicken->GetCurrentOutdoorMinutes();
        if (before <= 179 && after > 179)
        {
            EntityCalendarGameVTable const * table = *reinterpret_cast<EntityCalendarGameVTable const **>(game);
            if (table->get_calendar(game)->weather != 0 && !chicken->IsUnhappy())
                chicken->SetUnhappy();
        }
        if (before <= 239 && after > 239)
            chicken->SetFed();
        if (before <= 299 && after > 299)
        {
            EntityCalendarGameVTable const * table = *reinterpret_cast<EntityCalendarGameVTable const **>(game);
            if (table->get_calendar(game)->weather == 0)
            {
                if (chicken->IsUnhappy() && RollEntityMovementRandom(100) <= 29)
                    chicken->ResetUnhappy();
                chicken->AddAffection(1);
            }
        }
    }
    UnkMap map = game->vfunc_34(map_id);
    Box const & loaded_box = self->GetBox();
    Box box = loaded_box;
    EntityMovementBounds bounds(map, box, 16);
    AEntity * farmer = GetCarryableEntityById(game, 0);
    AEntity * obstacle = GetCarryableEntityById(game, 74);
    if (farmer != nullptr && farmer->location_map == map_id)
        func_080ABA90(&bounds, farmer->GetBox(), 0);
    if (obstacle != nullptr && obstacle->location_map == map_id)
        func_080ABA90(&bounds, obstacle->GetBox(), 0);
    switch (chicken->unk_24)
    {
    case 0:
    {
        AnimalBehaviorTimer * timer = reinterpret_cast<AnimalBehaviorTimer *>(&chicken->unk_28);
        u32 remaining = timer->time;
        if (remaining != 0)
        {
            i32 x_speed = self->x_speed_q16;
            i32 y_speed = self->y_speed_q16;
            bool blocked = false;
            if ((x_speed < 0 && bounds.GetNegativeX() > -16) ||
                (x_speed > 0 && bounds.GetPositiveX() < 16) ||
                (y_speed < 0 && bounds.GetNegativeY() > -16) ||
                (y_speed > 0 && bounds.GetPositiveY() < 16))
                blocked = true;
            if (blocked)
                remaining = 0;
            else
                --remaining;
        }
        else if (AEntity_IsAnimationFinished(self))
        {
            u32 hour = GetEntityGameHour(game);
            bool night_time = false;
            if (hour < 6 || hour > 20)
                night_time = true;
            ChickenBehaviorChoice const * choices = gUnk_080F09C8[night_time][chicken->GetGrowthStage()][chicken->IsSick()];
            u32 state = ChooseWeightedEntityMovement(choices, 5);
            choices += state;
            remaining = ChickenBehaviorDuration(*choices, 60);
            u32 facing = self->facing;
            u32 turn = RollEntityMovementRandom(4);
            if (turn != 0 && turn != 3)
                facing = RotateEntityMovementFacing(facing, turn);
            ChickenEntity_SetAnimationState(self, state, facing);
            timer->active = state;
        }
        timer->time = remaining;
        break;
    }
    case 1:
    {
        ChickenBehaviorTiming * timer = reinterpret_cast<ChickenBehaviorTiming *>(&chicken->unk_28);
        u32 remaining = timer->remaining_frames;
        if (remaining != 0)
            --remaining;
        ChickenBehaviorTiming const status = *timer;
        u32 running = status.running_frames;
        if (running != 0)
            --running;
        u32 direction_time = timer->direction_frames;
        if (direction_time != 0)
            --direction_time;
        if (remaining != 0)
        {
            AEntity * target;
            if (status.follow_dog)
                target = GetCarryableEntityById(game, 45);
            else
                target = farmer;
            if (target != nullptr && target->location_map == map_id)
            {
                if (direction_time == 0)
                {
                    i32 x = self->x_q16 >> 16;
                    i32 y = self->y_q16 >> 16;
                    i32 target_x = target->x_q16 >> 16;
                    i32 target_y = target->y_q16 >> 16;
                    u32 state = 1;
                    if (running != 0)
                        state = 4;
                    u32 const old_facing = self->facing;
                    u32 facing = func_080ABE58(x, y, target_x, target_y, &bounds, old_facing);
                    ChickenEntity_SetAnimationState(self, state, facing);
                    direction_time = RollEntityMovementRandom(30) + 1;
                }
                timer->remaining_frames = remaining;
                timer->running_frames = running;
                timer->direction_frames = direction_time;
                break;
            }
        }
        AnimalBehaviorTimer idle;
        idle.time = 180;
        idle.active = 0;
        chicken->method_0809BD20(reinterpret_cast<UnkBarnAnimal2C const *>(&idle));
        u32 state = ChickenEntity_GetAnimationState(self);
        ChickenEntity_SetAnimationState(self, state, self->facing);
        break;
    }
    }
    AActorEntity_ApplyBoundedMovement(self, &bounds);
}

struct FarmerActionDispatchVTable
{
    STRUCT_PAD(0x00, 0x5C);
    void (*lift_item)(FarmerEntity * self, RucksackItem const & item, bool wrapped);
    STRUCT_PAD(0x60, 0x80);
    void (*vfunc_80)(FarmerEntity * self);
};

inline FarmerActionDispatchVTable const * GetFarmerActionDispatchVTable(FarmerEntity * self)
{
    return reinterpret_cast<FarmerActionDispatchVTable const **>(self)[5];
}

// 0x08025B64
EC void FarmerEntity_PerformAction(FarmerEntity * self, u32 action) ENTITY_CODE(08025B64);
EC void FarmerEntity_PerformAction(FarmerEntity * self, u32 action)
{
    switch (action)
    {
    default:
    case 0: case 1: self->state = 0; break;
    case 3: GetFarmerActionDispatchVTable(self)->vfunc_80(self); break;
    case 4: func_0802720C(self, Tool(TOOL_CURSED_SICKLE)); break;
    case 5: func_0802720C(self, Tool(TOOL_CURSED_HOE)); break;
    case 6: func_0802720C(self, Tool(TOOL_CURSED_AXE)); break;
    case 7: func_0802720C(self, Tool(TOOL_CURSED_HAMMER)); break;
    case 8: func_0802720C(self, Tool(TOOL_CURSED_WATERING_CAN)); break;
    case 9: func_0802720C(self, Tool(TOOL_CURSED_FISHING_ROD)); break;
    case 10:
        func_0802720C(self, Tool(TOOL_TELEPORT_STONE));
        AudioPlayer_StartSong(FindEntitySoundChannel(self->game_object), 182);
        break;
    case 2: func_0802771C(self, rand() % 16 + 5); break;
    case 11: GetFarmerActionDispatchVTable(self)->lift_item(self, RucksackItem(Food(FOOD_BLACK_GRASS)), false); break;
    case 12: GetFarmerActionDispatchVTable(self)->lift_item(self, RucksackItem(Article(ARTICLE_JUNK_ORE)), false); break;
    case 13: GetFarmerActionDispatchVTable(self)->lift_item(self, RucksackItem(Article(ARTICLE_COPPER)), false); break;
    case 14: GetFarmerActionDispatchVTable(self)->lift_item(self, RucksackItem(Article(ARTICLE_SILVER)), false); break;
    case 15: GetFarmerActionDispatchVTable(self)->lift_item(self, RucksackItem(Article(ARTICLE_GOLD)), false); break;
    case 16: GetFarmerActionDispatchVTable(self)->lift_item(self, RucksackItem(Article(ARTICLE_MYSTRILE)), false); break;
    case 17: GetFarmerActionDispatchVTable(self)->lift_item(self, RucksackItem(Article(ARTICLE_ORICHALC)), false); break;
    case 18: GetFarmerActionDispatchVTable(self)->lift_item(self, RucksackItem(Article(ARTICLE_ADAMANTITE)), false); break;
    case 19: GetFarmerActionDispatchVTable(self)->lift_item(self, RucksackItem(Article(ARTICLE_MOON_STONE)), false); break;
    case 20: GetFarmerActionDispatchVTable(self)->lift_item(self, RucksackItem(Article(ARTICLE_SAND_ROSE)), false); break;
    case 21: GetFarmerActionDispatchVTable(self)->lift_item(self, RucksackItem(Article(ARTICLE_PINK_DIAMOND)), false); break;
    case 22: GetFarmerActionDispatchVTable(self)->lift_item(self, RucksackItem(Article(ARTICLE_ALEXANDRITE)), false); break;
    case 23: GetFarmerActionDispatchVTable(self)->lift_item(self, RucksackItem(Article(ARTICLE_MYTHIC_STONE)), false); break;
    case 24: GetFarmerActionDispatchVTable(self)->lift_item(self, RucksackItem(Article(ARTICLE_DIAMOND)), false); break;
    case 25: GetFarmerActionDispatchVTable(self)->lift_item(self, RucksackItem(Article(ARTICLE_EMERALD)), false); break;
    case 26: GetFarmerActionDispatchVTable(self)->lift_item(self, RucksackItem(Article(ARTICLE_RUBY)), false); break;
    case 27: GetFarmerActionDispatchVTable(self)->lift_item(self, RucksackItem(Article(ARTICLE_TOPAZ)), false); break;
    case 28: GetFarmerActionDispatchVTable(self)->lift_item(self, RucksackItem(Article(ARTICLE_PERIDOT)), false); break;
    case 29: GetFarmerActionDispatchVTable(self)->lift_item(self, RucksackItem(Article(ARTICLE_FLUORITE)), false); break;
    case 30: GetFarmerActionDispatchVTable(self)->lift_item(self, RucksackItem(Article(ARTICLE_AGATE)), false); break;
    case 31: GetFarmerActionDispatchVTable(self)->lift_item(self, RucksackItem(Article(ARTICLE_AMETHYST)), false); break;
    case 32: GetFarmerActionDispatchVTable(self)->lift_item(self, RucksackItem(Article(ARTICLE_HARVEST_GODDESS_JEWEL)), false); break;
    case 33: GetFarmerActionDispatchVTable(self)->lift_item(self, RucksackItem(Article(ARTICLE_KAPPA_JEWEL)), false); break;
    case 34: GetFarmerActionDispatchVTable(self)->lift_item(self, RucksackItem(Article(ARTICLE_LITHOGRAPH)), false); break;
    }
}

// 0x0802405C
EC u32 SheepEntity_GetStateAnimation(BarnAnimalEntity * self, u32 state) ENTITY_CODE(0802405C);
EC u32 SheepEntity_GetStateAnimation(BarnAnimalEntity * self, u32 state)
{
    u32 const animation_state = state;
    bool pregnant = false;
    if (self->animal->IsPregnant())
        pregnant = self->animal->GetDaysPregnant() != 0;
    u16 const * table = gUnk_080F0E34;
    u32 offset = animation_state * 2;
    u32 index;
    if (pregnant)
        index = offset + 42;
    else if (static_cast<Sheep *>(self->animal)->IsSheared())
        index = offset + 28;
    else
        index = offset + static_cast<Sheep *>(self->animal)->GetGrowthStage() * 14;
    return *reinterpret_cast<u16 const *>(reinterpret_cast<u8 const *>(table) + index);
}

// 0x08021FAC
EC i32 HorseEntity_GetStateSpeed(HorseEntity * self, u32 state) ENTITY_CODE(08021FAC);
EC i32 HorseEntity_GetStateSpeed(HorseEntity * self, u32 state)
{
    switch (state)
    {
    case 0: case 3: case 4: case 5: default: return 0;
    case 1: return 0x8000;
    case 2: return 0x10000;
    case 6:
    {
        Box const & collision = self->GetBox();
        Box box = collision;
        Box const * bounds = &box;
        i32 distance = 16;
        switch (i32(self->facing))
        {
        default: break;
        case 0: case 1: distance = bounds->y2 - bounds->y1 + 16; break;
        case 2: case 3: distance = bounds->x2 - bounds->x1 + 16; break;
        }
        return (distance << 16) / 12;
    }
    }
}

// 0x08021130
EC i32 DogEntity_GetStateSpeed(DogEntity * self, u32 state) ENTITY_CODE(08021130);
EC i32 DogEntity_GetStateSpeed(DogEntity * self, u32 state)
{
    switch (state)
    {
    case 0: case 3: case 4: case 5: case 6: case 7:
    case 8: case 9: case 10: case 11: default: return 0;
    case 1: return 0x8000;
    case 2: return 0x10000;
    case 12:
    {
        Box const & collision = self->GetBox();
        Box box = collision;
        Box const * bounds = &box;
        i32 distance = 16;
        switch (i32(self->facing))
        {
        default: break;
        case 0: case 1: distance = bounds->y2 - bounds->y1 + 16; break;
        case 2: case 3: distance = bounds->x2 - bounds->x1 + 16; break;
        }
        return (distance << 16) / 30;
    }
    }
}

// 0x08025248
EC u32 FarmerEntity_FindVerticalClearStep(FarmerEntity const * self, void * map, Box const * box, i32 delta, u32 limit) ENTITY_CODE(08025248);
EC u32 FarmerEntity_FindVerticalClearStep(FarmerEntity const * self, void * map, Box const * box, i32 delta, u32 limit)
{
    u32 const max_steps = limit;
    Box shifted = *box;
    u32 step = 1;
    if (step <= max_steps)
    {
        do
        {
            i16 * top = &shifted.y1;
            i16 * bottom = &shifted.y2;
            *top += delta;
            *bottom += delta;
            if (!func_080AC070(map, &shifted))
                break;
            ++step;
        } while (step <= max_steps);
    }
    return step;
}

// 0x080251FC
EC u32 FarmerEntity_FindHorizontalClearStep(FarmerEntity const * self, void * map, Box const * box, i32 delta, u32 limit) ENTITY_CODE(080251FC);
EC u32 FarmerEntity_FindHorizontalClearStep(FarmerEntity const * self, void * map, Box const * box, i32 delta, u32 limit)
{
    u32 const max_steps = limit;
    Box shifted = *box;
    u32 step = 1;
    if (step <= max_steps)
    {
        do
        {
            i16 * left = &shifted.x1;
            i16 * right = &shifted.x2;
            *left += delta;
            *right += delta;
            if (!func_080AC070(map, &shifted))
                break;
            ++step;
        } while (step <= max_steps);
    }
    return step;
}

// 0x080222A8
EC UnknownEntityThing * ChickenEntitySprite_Init(UnknownEntityThing * self, AActorEntity * entity, u32 growth_stage) ENTITY_CODE(080222A8);
EC UnknownEntityThing * ChickenEntitySprite_Init(UnknownEntityThing * self, AActorEntity * entity, u32 growth_stage)
{
    u32 size = func_08022320(growth_stage);
    u32 variant = func_08022334(growth_stage);
    func_080324BC(self, entity, 5, 13, size, variant, 0, false);
    reinterpret_cast<void const **>(self)[1] = vtable_unk_080E64B4;
    return self;
}

// 0x08025294
EC bool FarmerEntity_IsAdjacentBoxClear(FarmerEntity const * self, void * map, i32 facing, Box const * box) ENTITY_CODE(08025294);
EC bool FarmerEntity_IsAdjacentBoxClear(FarmerEntity const * self, void * map, i32 facing, Box const * box)
{
    Box bounds(-1, -1, 689, 401, 272, 176);
    if (((bounds.x1 - box->x2) & (box->x1 - bounds.x2) & (bounds.y1 - box->y2) & (box->y1 - bounds.y2)) < 0)
    {
        switch (facing)
        {
        case 0: if (!IsEntityCollisionBoxBlocked(map, box->Moved(0, box->y2 - box->y1 + 16))) return true; break;
        case 1: if (!IsEntityCollisionBoxBlocked(map, box->Moved(0, -(box->y2 - box->y1 + 16)))) return true; break;
        case 2: if (!IsEntityCollisionBoxBlocked(map, box->Moved(-(box->x2 - box->x1 + 16), 0))) return true; break;
        case 3: if (!IsEntityCollisionBoxBlocked(map, box->Moved(box->x2 - box->x1 + 16, 0))) return true; break;
        }
    }
    return false;
}

// 0x08024B9C
EC UnknownEntityThing * FarmerEntity_CreateSprite(FarmerEntity * self) ENTITY_CODE(08024B9C);
EC UnknownEntityThing * FarmerEntity_CreateSprite(FarmerEntity * self)
{
    u32 priority = 2;
    if (self->location_map == 19)
    {
        FarmerEventStateView * events = reinterpret_cast<FarmerEventStateView *>(self->game_state);
        u32 event_state = events->unk_2171_3;
        if (event_state == 1 || IsSelectedScriptEvent(*reinterpret_cast<ScriptEventSelection const *>(
            reinterpret_cast<u8 *>(events) + 0x214C), 74))
            priority = 1;
    }
    self->unk_21 = priority;
    return func_0802B260(__builtin_new(0x284), self);
}

// 0x080233B4
EC u32 CowEntity_GetStateAnimation(BarnAnimalEntity * self, u32 state) ENTITY_CODE(080233B4);
EC u32 CowEntity_GetStateAnimation(BarnAnimalEntity * self, u32 state)
{
    u32 offset = state;
    bool pregnant = false;
    if (self->animal->IsPregnant())
        pregnant = self->animal->GetDaysPregnant() != 0;
    u16 const * table = gUnk_080F0C3C;
    offset *= 2;
    u32 index;
    if (pregnant)
        index = offset + 42;
    else
        index = offset + static_cast<Cow *>(self->animal)->GetGrowthStage() * 14;
    return *reinterpret_cast<u16 const *>(reinterpret_cast<u8 const *>(table) + index);
}

// 0x08025AFC
EC bool AEntity_IsOnOutdoorMap(AEntity const * self) ENTITY_CODE(08025AFC);
EC bool AEntity_IsOnOutdoorMap(AEntity const * self)
{
    i32 map = self->location_map;
    if (map <= 8)
        return true;
    return false;
}

// 0x0802110C
EC u32 DogEntity_GetStateAnimation(DogEntity * self, u32 state) ENTITY_CODE(0802110C);
EC u32 DogEntity_GetStateAnimation(DogEntity * self, u32 state)
{
    u32 offset = state;
    u16 const * table = gUnk_080F0908;
    offset *= 2;
    if (self->dog->GetGrowthStage() == 1)
        offset += 26;
    return *reinterpret_cast<u16 const *>(reinterpret_cast<u8 const *>(table) + offset);
}

// 0x08024A80
EC void FarmerEntity_Destroy(FarmerEntity * self, u32 flags) ENTITY_CODE(08024A80);
EC void FarmerEntity_Destroy(FarmerEntity * self, u32 flags)
{
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6658;
    func_0800EB34(*self->farmer, self->AActorEntity::GetLocation());
    HeldItem * held = &self->farmer->unk_54;
    if (!func_0800F190(held))
    {
        switch (func_0800F204(held))
        {
        default:
        case HeldItem::KIND_FOOD:
        case HeldItem::KIND_5: break;
        case HeldItem::KIND_ARTICLE:
        {
            Article article = func_0800F258(held);
            if (article.GetId() == 53)
                *reinterpret_cast<Location *>(reinterpret_cast<u8 *>(self->game_state) + 0x1CCC) = self->AEntity::GetLocation();
            break;
        }
        case HeldItem::KIND_2:
            reinterpret_cast<Actor *>(reinterpret_cast<u8 *>(self->game_state) + 0x1C70)->SetLocation(self->AActorEntity::GetLocation());
            break;
        case HeldItem::KIND_3:
        {
            Coop * coop = reinterpret_cast<Coop *>(reinterpret_cast<u8 *>(self->game_state) + 0x410);
            Chicken * chicken = coop->GetChicken(func_0800F344(held));
            if (chicken != nullptr)
                chicken->SetLocation(self->AActorEntity::GetLocation());
            break;
        }
        case HeldItem::KIND_4:
            ItemCollection_SetExtraItems(reinterpret_cast<u8 *>(self->game_state) + 0x1CA0, self->AEntity::GetLocation());
            break;
        }
    }
    self->AEntity::~AEntity();
    if (flags & 1)
        __builtin_delete(self);
}

// 0x08024BFC
EC Box FarmerEntity_GetCollisionBox(FarmerEntity const * self) ENTITY_CODE(08024BFC);
EC Box FarmerEntity_GetCollisionBox(FarmerEntity const * self)
{
    if (reinterpret_cast<CarryableActorEntity *>(const_cast<FarmerEntity *>(self))->vfunc_40())
    {
        Box const & box = Box(-7, -9, 7, 5, self->GetQ16X() >> 16, self->GetQ16Y() >> 16);
        return box;
    }
    Box const & box = func_0803240C(self);
    return box;
}

// 0x080228B4
EC void ChickenEntity_SetAnimationState(ChickenEntity * self, u32 state, u32 facing) ENTITY_CODE(080228B4);
EC void ChickenEntity_SetAnimationState(ChickenEntity * self, u32 state, u32 facing)
{
    u32 anim = ChickenEntity_GetStateAnimation(self, state);
    if (self->anim_id != anim)
        self->SetAnim(anim);
    if (self->facing != facing)
        self->SetAnimFacing(facing);
    AActorEntity_SetFacingSpeed(self, ChickenEntity_GetStateSpeed(self, state));
    if (state != self->state)
    {
        i32 effect = -1;
        u32 sleep = self->chicken->GetGrowthStage() == 0 ? 0 : 3;
        if (state == sleep && u32(GetEntityGameHour(self->game_object) - 6) > 14)
            effect = 8;
        if (effect >= 0)
            func_08032384(self, effect, 1);
        else
            func_080323C8(self);
    }
    self->state = state;
}

// 0x08024C74
EC void FarmerEntity_ResetAction(FarmerEntity * self) ENTITY_CODE(08024C74);
EC void FarmerEntity_ResetAction(FarmerEntity * self)
{
    self->state = 0;
    self->motion = 0;
    self->unk_A6 = 0;
}

// 0x08025068
EC bool FarmerEntity_ChangeStaminaAndFatigue(FarmerEntity * self, i32 stamina, i32 fatigue) ENTITY_CODE(08025068);
EC bool FarmerEntity_ChangeStaminaAndFatigue(FarmerEntity * self, i32 stamina, i32 fatigue)
{
    if (reinterpret_cast<u8 const *>(self->game_state)[0x34C4])
        return false;
    if (self->unk_10.Get() != nullptr)
        reinterpret_cast<u8 *>(self->unk_10.Get())[0x281] = true;
    u32 old_stamina = func_0800E4F0(*self->farmer);
    u32 max_stamina = func_0800E51C(*self->farmer);
    func_0800E9E4(*self->farmer, stamina);
    u32 new_stamina = func_0800E4F0(*self->farmer);
    u32 old_fatigue = func_0800E4FC(*self->farmer);
    func_0800EA68(*self->farmer, fatigue);
    u32 new_fatigue = func_0800E4FC(*self->farmer);
    if (stamina > 0 || fatigue < 0)
    {
        switch (self->exhaustion_state)
        {
        case 36: if (new_stamina > max_stamina / 2) self->exhaustion_state = 0; break;
        case 37: if (new_stamina > max_stamina / 5) self->exhaustion_state = 0; break;
        case 38: if (new_stamina > max_stamina / 20) self->exhaustion_state = 0; break;
        case 39: if (new_stamina != 0) self->exhaustion_state = 0; break;
        case 40: if (new_fatigue <= 49) self->exhaustion_state = 0; break;
        case 41: if (new_fatigue <= 79) self->exhaustion_state = 0; break;
        case 42: if (new_fatigue != 100) self->exhaustion_state = 0; break;
        }
    }
    if (old_fatigue != 100 && new_fatigue == 100)
    {
        self->exhaustion_state = 42;
        return true;
    }
    if (old_fatigue <= 79 && new_fatigue > 79)
        self->exhaustion_state = 41;
    else if (old_fatigue <= 49 && new_fatigue > 49)
        self->exhaustion_state = 40;
    else if (old_stamina != 0 && new_stamina == 0)
        self->exhaustion_state = 39;
    else if (old_stamina > max_stamina / 20 && new_stamina <= max_stamina / 20)
        self->exhaustion_state = 38;
    else if (old_stamina > max_stamina / 5 && new_stamina <= max_stamina / 5)
        self->exhaustion_state = 37;
    else if (old_stamina > max_stamina / 2 && new_stamina <= max_stamina / 2)
        self->exhaustion_state = 36;
    return false;
}

// 0x08024974
EC FarmerEntity * FarmerEntity_Init(FarmerEntity * self, GameObject * game, void * state) ENTITY_CODE(08024974);
EC FarmerEntity * FarmerEntity_Init(FarmerEntity * self, GameObject * game, void * state)
{
    InitFarmerMapActor(self, game, state);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6658;
    self->game_state = state;
    self->farmer = reinterpret_cast<Farmer *>(reinterpret_cast<u8 *>(state) + 0x1BD8);
    self->state = 0;
    self->unk_3D = 0;
    self->motion = 0;
    self->tool_state = 15;
    self->tool_level = 0;
    self->tool_charge_limit = 0;
    self->tool_charge_ticks = 0;
    self->unk_44 = 0;
    self->held_anim.Init();
    self->unk_78 = false;
    self->unk_79 = false;
    self->unk_7A = false;
    self->icon = 0xFFFF;
    self->unk_80 = 0;
    self->thrown_item.Init();
    self->item_animation_override = true;
    self->unk_A6 = 0;
    self->last_discarded_item_kind = -1;
    self->last_discarded_item_id = -1;
    self->last_discard_special = false;
    self->target_cow_slot = 0;
    self->interaction_target_slot = 100;
    self->unk_B3 = false;
    self->unk_B4 = false;
    self->consume_animation_active = false;
    self->unk_C1 = false;
    self->update_paused = false;
    self->exhaustion_state = 0;
    self->unk_C4 = false;
    self->unk_C5 = false;
    FarmerEntityInitNoOp(self);
    u16 anim = func_0802D158(self);
    if (self->anim_id != anim)
        self->SetAnim(anim);
    return self;
}

// 0x08025B10
EC u32 FarmerEntity_GetToolFatiguePenalty(FarmerEntity * self) ENTITY_CODE(08025B10);
EC u32 FarmerEntity_GetToolFatiguePenalty(FarmerEntity * self)
{
    u32 result = 0;
    if (AEntity_IsOnOutdoorMap(self) && u32(reinterpret_cast<FarmerGameStateView const *>(self->game_state)->weather - 1) <= 1)
        result = 1;
    FarmerGameStateView const * game = reinterpret_cast<FarmerGameStateView const *>(self->game_state);
    if (game->time.GetHour() > 21)
        ++result;
    if (game->time.GetHour() <= 5)
        result += 2;
    if (func_0800E4F0(*self->farmer) == 0)
        result += 2;
    if (FarmerEntity_IsUsingCursedTool(self))
        result += 10;
    return result;
}

// 0x08026150
EC void FarmerEntity_LiftRucksackItem(FarmerEntity * self, RucksackItem const * item, bool wrapped) ENTITY_CODE(08026150);
EC void FarmerEntity_LiftRucksackItem(FarmerEntity * self, RucksackItem const * item, bool wrapped)
{
    Product product;
    switch (item->GetKind())
    {
    case RucksackItem::KIND_FOOD: product = Product(item->GetFood()); break;
    case RucksackItem::KIND_ARTICLE: product = Product(item->GetArticle()); break;
    }
    ShippingBin * shipping = &reinterpret_cast<FarmerGameStateView *>(self->game_state)->shipping_bin;
    if (product.GetPrice() != 0 && !shipping->IsDisplayEnabled(product.GetId()))
        shipping->ForceEnableDisplay(product.GetId());
    func_0800F418(&self->farmer->unk_54, *item);
    if (wrapped)
        func_0800F528(&self->farmer->unk_54);
    func_0802A588(self);
    self->state = 17;
    LaunchFarmerHeldItem(self);
    AudioPlayer_StartSong(FindEntitySoundChannel(self->game_object), 106);
}

// 0x080262C4
EC Product FarmerEntity_GetHeldProduct(FarmerEntity const * self) ENTITY_CODE(080262C4);
EC Product FarmerEntity_GetHeldProduct(FarmerEntity const * self)
{
    HeldItem const * item = reinterpret_cast<HeldItem const *>(reinterpret_cast<u8 const *>(self->game_state) + 0x1C2C);
    if (!func_0800F190(item) && !func_0800F388(item))
    {
        switch (func_0800F204(item))
        {
        case HeldItem::KIND_FOOD: return Product(func_0800F20C(item));
        case HeldItem::KIND_ARTICLE: return Product(func_0800F258(item));
        }
    }
    return Product();
}

// 0x08024CA0
EC bool FarmerEntity_IsExhaustionAnimationPending(FarmerEntity const * self) ENTITY_CODE(08024CA0);
EC bool FarmerEntity_IsExhaustionAnimationPending(FarmerEntity const * self)
{
    if (!self->vfunc_28())
    {
        switch (GetFarmerAnimationState(self))
        {
        case 36: case 37: case 38: case 39: case 40: case 41: case 42: return true;
        }
    }
    return false;
}

// 0x08025018
EC void func_08025018(FarmerEntity * self, u32 argument) ENTITY_CODE(08025018);
EC void func_08025018(FarmerEntity * self, u32 argument)
{
    self->unk_C1 = false;
    if (!self->update_paused)
    {
        if (self->consume_animation_active)
        {
            func_0802F0EC(self, 0, 0);
            u16 anim = func_0802D158(self);
            if (self->anim_id != anim)
                self->SetAnim(anim);
        }
        func_0803242C(self, argument);
    }
}

// 0x08020170
EC void AActorEntity_ApplyBoundedMovement(AActorEntity * self, EntityMovementBounds * bounds) ENTITY_CODE(08020170);
EC void AActorEntity_ApplyBoundedMovement(AActorEntity * self, EntityMovementBounds * bounds)
{
    i32 old_x = self->x_q16;
    i32 old_y = self->y_q16;
    i32 new_x = old_x + self->x_speed_q16;
    i32 new_y = old_y + self->y_speed_q16;
    if (bounds != nullptr)
    {
        EntityMovementBounds * movement = bounds;
        i32 x = old_x >> 16;
        i32 y = old_y >> 16;
        i32 dy = (new_y >> 16) - y;
        i32 dx = (new_x >> 16) - x;
        if (dy > 0 && dy > movement->GetPositiveY())
            new_y = (y + movement->GetPositiveY()) << 16;
        if (dy < 0 && dy < movement->GetNegativeY())
            new_y = (y + movement->GetNegativeY()) << 16;
        if (dx < 0 && dx < movement->GetNegativeX())
            new_x = (x + movement->GetNegativeX()) << 16;
        if (dx > 0 && dx > movement->GetPositiveX())
            new_x = (x + movement->GetPositiveX()) << 16;
    }
    self->x_q16 = new_x;
    self->y_q16 = new_y;
}

// 0x080212C0
EC void DogEntity_PlaceCarriedTarget(DogEntity * self) ENTITY_CODE(080212C0);
EC void DogEntity_PlaceCarriedTarget(DogEntity * self)
{
    CarryableActorEntity * target = GetCarryableEntityById(self->game_object, 75);
    if (target != nullptr && reinterpret_cast<u8 *>(target)[0x26] != 0)
    {
        i32 x = self->x_q16;
        i32 y = self->y_q16;
        i32 offset = 8;
        u32 map_id = self->location_map;
        GameObject * game = self->game_object;
        if (map_id != game->vfunc_14() && map_id != 2)
            offset = 0;
        UnkMap const & map = game->vfunc_34(map_id);
        for (;;)
        {
            i32 delta = offset << 16;
            switch (GetEntityFacing(self))
            {
            case 0: target->x_q16 = x; target->y_q16 = y + delta; break;
            case 1: target->x_q16 = x; target->y_q16 = y - delta; break;
            case 2: target->x_q16 = x - delta; target->y_q16 = y; break;
            case 3: target->x_q16 = x + delta; target->y_q16 = y; break;
            }
            --offset;
            if (offset < 0)
                break;
            Box box = target->GetBox();
            if (!func_080AC070(const_cast<UnkMap *>(&map), &box))
                break;
        }
        StopLaunchedSpriteMovement(target);
    }
}

// 0x0802907C
EC void FarmerEntity_ClearHeldItem(FarmerEntity * self) ENTITY_CODE(0802907C);
EC void FarmerEntity_ClearHeldItem(FarmerEntity * self)
{
    HeldItem * item = reinterpret_cast<HeldItem *>(reinterpret_cast<u8 *>(self->game_state) + 0x1C2C);
    if (!func_0800F190(item))
    {
        if (func_0800F204(item) == HeldItem::KIND_ARTICLE)
        {
            Article article = func_0800F258(item);
            if (article.GetId() == 53)
                func_0800ED2C(*self->farmer);
        }
        func_0800F390(&self->farmer->unk_54);
        CancelFarmerItemDisplay(self);
    }
}

// 0x08020310
EC DogEntity * DogEntity_Init(DogEntity * self, GameObject * game, Dog * dog, void * object) ENTITY_CODE(08020310);
EC DogEntity * DogEntity_Init(DogEntity * self, GameObject * game, Dog * dog, void * object)
{
    Actor location(*dog);
    func_08020038(self, game, &location.location, 0x340);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E64C8;
    self->dog = dog;
    self->unk_38 = object;
    self->override_state = false;
    if (dog->unk_20 == 5)
    {
        CarryableActorEntity * target = GetCarryableEntityById(game, 75);
        if (target != nullptr)
        {
            if (target->location_map == self->location_map)
                StartLaunchedSpriteMovement(target, self->x_q16, self->y_q16, self->anim_id, self->facing);
            else
                DogEntity_ResetBehavior(self, 0);
        }
    }
    DogEntity_SetAnimationState(self, DogEntity_GetAnimationState(self), self->facing);
    return self;
}

// 0x080216BC
EC HorseEntity * HorseEntity_Init(HorseEntity * self, GameObject * game, Horse * horse) ENTITY_CODE(080216BC);
EC HorseEntity * HorseEntity_Init(HorseEntity * self, GameObject * game, Horse * horse)
{
    Actor location(*horse);
    func_08020038(self, game, &location.location, 0x782);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6428;
    self->horse = horse;
    self->override_state = false;
    CarryableActorEntity * target = GetCarryableEntityById(game, 0);
    self->suppress_effect = target && target->vfunc_40();
    HorseEntity_SetAnimationState(self, HorseEntity_GetAnimationState(self), self->facing);
    return self;
}

// 0x080221F0
EC void HorseEntity_TalkIfNearby(HorseEntity * self, Location const * target) ENTITY_CODE(080221F0);
EC void HorseEntity_TalkIfNearby(HorseEntity * self, Location const * target)
{
    if (target->GetMap() == self->GetMap())
    {
        u32 affection = self->horse->GetAffection();
        i32 radius = func_08020018(affection);
        i32 x = self->GetQ16X() >> 16;
        i32 dx = target->GetX() - x;
        i32 y = self->GetQ16Y() >> 16;
        i32 dy = target->GetY() - y;
        if (dx * dx + dy * dy <= radius * radius)
        {
            AnimalBehaviorTimerPair timer;
            timer.time = u16(480 + self->horse->GetAffection() * 960 / 256);
            self->horse->method_0809BC30(reinterpret_cast<UnkBarnAnimal2C_x2 const *>(&timer));
            if (!self->override_state)
                HorseEntity_SetAnimationState(self, HorseEntity_GetAnimationState(self), self->facing);
            self->horse->SetHasTalkedToday();
        }
    }
}

// 0x0802151C
EC void DogEntity_PlayIfNearby(DogEntity * self, Location const * target) ENTITY_CODE(0802151C);
EC void DogEntity_PlayIfNearby(DogEntity * self, Location const * target)
{
    if (self->dog->GetGrowthStage() == 1 && target->GetMap() == self->GetMap())
    {
        u32 affection = self->dog->GetAdequacy();
        i32 radius = func_08020018(affection);
        i32 x = self->GetQ16X() >> 16;
        i32 dx = target->GetX() - x;
        i32 y = self->GetQ16Y() >> 16;
        i32 dy = target->GetY() - y;
        if (dx * dx + dy * dy <= radius * radius)
        {
            AEntity_PlayAnimationSound(self, 169);
            if (self->dog->unk_20 == 5)
                DogEntity_PlaceCarriedTarget(self);
            AnimalBehaviorTimerPair timer;
            timer.time = u16(480 + affection * 960 / 251);
            self->dog->method_0809BB70(reinterpret_cast<UnkBarnAnimal2C_x2 const *>(&timer));
            if (!self->override_state)
                DogEntity_SetAnimationState(self, DogEntity_GetAnimationState(self), self->facing);
            self->dog->SetHasPlayedToday();
        }
    }
}

// 0x08021444
EC void DogEntity_TalkIfNearby(DogEntity * self, Location const * target) ENTITY_CODE(08021444);
EC void DogEntity_TalkIfNearby(DogEntity * self, Location const * target)
{
    if (target->GetMap() == self->GetMap() && self->dog->unk_20 != 5)
    {
        u32 affection = self->dog->GetAffection();
        i32 radius = func_08020018(affection);
        i32 x = self->GetQ16X() >> 16;
        i32 dx = target->GetX() - x;
        i32 y = self->GetQ16Y() >> 16;
        i32 dy = target->GetY() - y;
        if (dx * dx + dy * dy <= radius * radius)
        {
            if (self->dog->unk_20 == 5)
                DogEntity_PlaceCarriedTarget(self);
            AnimalBehaviorTimerPair timer;
            timer.time = u16(480 + affection * 960 / 251);
            self->dog->method_0809BB54(reinterpret_cast<UnkBarnAnimal2C_x2 const *>(&timer));
            if (!self->override_state)
                DogEntity_SetAnimationState(self, DogEntity_GetAnimationState(self), self->facing);
            self->dog->SetHasTalkedToday();
        }
    }
}

// 0x08025944
EC bool FarmerEntity_IsUsingCursedTool(FarmerEntity * self) ENTITY_CODE(08025944);
EC bool FarmerEntity_IsUsingCursedTool(FarmerEntity * self)
{
    ToolStack * tools = &self->farmer->unk_5C;
    if (!tools->IsEmpty())
    {
        Tool tool = func_0800E550(*self->farmer, tools->GetTool());
        switch (tool.GetId())
        {
        case 5: case 13: case 21: case 29: case 37: case 45: return true;
        }
    }
    return false;
}

// 0x08025FC4
EC void FarmerEntity_LiftObject(FarmerEntity * self) ENTITY_CODE(08025FC4);
EC void FarmerEntity_LiftObject(FarmerEntity * self)
{
    func_0800F4D8(&self->farmer->unk_54);
    func_0802A588(self);
    self->state = 17;
    LaunchFarmerHeldItem(self);
    AudioPlayer_StartSong(FindEntitySoundChannel(self->game_object), 106);
}

// 0x08025EFC
EC void FarmerEntity_LiftChicken(FarmerEntity * self, u8 value) ENTITY_CODE(08025EFC);
EC void FarmerEntity_LiftChicken(FarmerEntity * self, u8 value)
{
    func_0800F4F0(&self->farmer->unk_54, value);
    func_0802A588(self);
    self->state = 17;
    LaunchFarmerHeldItem(self);
    AudioPlayer_StartSong(FindEntitySoundChannel(self->game_object), 106);
}

// 0x08025E38
EC void FarmerEntity_LiftDog(FarmerEntity * self) ENTITY_CODE(08025E38);
EC void FarmerEntity_LiftDog(FarmerEntity * self)
{
    func_0800F4C0(&self->farmer->unk_54);
    func_0802A588(self);
    self->state = 17;
    LaunchFarmerHeldItem(self);
    AudioPlayer_StartSong(FindEntitySoundChannel(self->game_object), 106);
}

// 0x08025A40
EC void FarmerEntity_CycleTool(FarmerEntity * self, bool next) ENTITY_CODE(08025A40);
EC void FarmerEntity_CycleTool(FarmerEntity * self, bool next)
{
    if (FarmerEntity_IsUsingCursedTool(self))
    {
        AudioPlayer_StartSong(FindEntitySoundChannel(self->game_object), 199);
        self->state = 0;
    }
    else
    {
        u32 result;
        if (!next)
            result = func_0800EC4C(*self->farmer);
        else
            result = func_0800ECB0(*self->farmer);
        if (result != 0)
        {
            self->state = 21;
            self->unk_3D = 20;
            ShowFarmerToolDisplay(self);
            AudioPlayer_StartSong(FindEntitySoundChannel(self->game_object), 188);
        }
    }
}

// 0x080258A0
EC void FarmerEntity_StoreHeldItem(FarmerEntity * self) ENTITY_CODE(080258A0);
EC void FarmerEntity_StoreHeldItem(FarmerEntity * self)
{
    RucksackItem item = func_0800F294(&self->farmer->unk_54);
    if (!item.IsEmpty())
    {
        u32 slot = self->farmer->rucksack.GetFirstFreeItemSlot();
        if (slot != u32(-1))
        {
            self->farmer->rucksack.GetItemAt(slot) = item;
            func_0800F390(&self->farmer->unk_54);
            self->state = 18;
            CancelFarmerItemDisplay(self);
            AudioPlayer_StartSong(FindEntitySoundChannel(self->game_object), 107);
        }
        else
            self->state = 4;
    }
    else
        self->state = 0;
}

// 0x08025838
EC void FarmerEntity_CycleHeldItemWithFeedback(FarmerEntity * self, bool next) ENTITY_CODE(08025838);
EC void FarmerEntity_CycleHeldItemWithFeedback(FarmerEntity * self, bool next)
{
    u32 result;
    if (!next)
        result = func_0800EB3C(*self->farmer);
    else
        result = func_0800EBC4(*self->farmer);
    if (result != 0)
    {
        func_0802A588(self);
        self->state = 20;
        AudioPlayer_StartSong(FindEntitySoundChannel(self->game_object), 107);
    }
}

// 0x080257D0
EC void FarmerEntity_CycleHeldItem(FarmerEntity * self, bool next) ENTITY_CODE(080257D0);
EC void FarmerEntity_CycleHeldItem(FarmerEntity * self, bool next)
{
    u32 result;
    if (!next)
        result = func_0800EB3C(*self->farmer);
    else
        result = func_0800EBC4(*self->farmer);
    if (result != 0)
    {
        self->state = 19;
        CancelFarmerItemDisplay(self);
        AudioPlayer_StartSong(FindEntitySoundChannel(self->game_object), 107);
    }
}

// 0x08021F3C
EC void HorseEntity_EndPlay(HorseEntity * self) ENTITY_CODE(08021F3C);
EC void HorseEntity_EndPlay(HorseEntity * self)
{
    self->suppress_effect = false;
    AnimalBehaviorTimer timer;
    timer.time = 120;
    timer.active = 0;
    self->horse->method_0809BC24(reinterpret_cast<UnkBarnAnimal2C const *>(&timer));
    HorseEntity_SetAnimationState(self, HorseEntity_GetAnimationState(self), self->facing);
}

// 0x08021EE0
EC void HorseEntity_BeginPlay(HorseEntity * self) ENTITY_CODE(08021EE0);
EC void HorseEntity_BeginPlay(HorseEntity * self)
{
    if (self->horse->GetGrowthStage() == 1)
    {
        AnimalBehaviorTimer timer;
        timer.time = 120;
        timer.active = 0;
        self->horse->method_0809BC24(reinterpret_cast<UnkBarnAnimal2C const *>(&timer));
        HorseEntity_SetAnimationState(self, HorseEntity_GetAnimationState(self), self->facing);
        self->suppress_effect = true;
        self->horse->SetHasPlayedToday();
    }
}

// 0x08020410
EC void DogEntity_ResetBehavior(DogEntity * self, u32 time) ENTITY_CODE(08020410);
EC void DogEntity_ResetBehavior(DogEntity * self, u32 time)
{
    if (self->dog->unk_20 == 5)
        DogEntity_PlaceCarriedTarget(self);
    AnimalBehaviorTimer timer;
    timer.time = time;
    timer.active = 0;
    self->dog->method_0809BB48(reinterpret_cast<UnkBarnAnimal2C const *>(&timer));
    DogEntity_SetAnimationState(self, 0, self->facing);
}

// 0x08020080
EC void AActorEntity_SetFacingSpeed(AActorEntity * self, i32 speed) ENTITY_CODE(08020080);
EC void AActorEntity_SetFacingSpeed(AActorEntity * self, i32 speed)
{
    i32 x = 0;
    i32 y = 0;
    if (speed != 0)
    {
        switch (GetEntityFacing(self))
        {
        case 0: y = speed; break;
        case 1: y = -speed; break;
        case 2: x = -speed; break;
        case 3: x = speed; break;
        }
    }
    self->x_speed_q16 = x;
    self->y_speed_q16 = y;
}

// 0x08022048
EC void HorseEntity_SetAnimationState(HorseEntity * self, u32 state, u32 facing) ENTITY_CODE(08022048);
EC void HorseEntity_SetAnimationState(HorseEntity * self, u32 state, u32 facing)
{
    if (state != 6 && self->override_state)
        state = 6;
    u32 anim = HorseEntity_GetStateAnimation(self, state);
    if (self->anim_id != anim)
        self->SetAnim(anim);
    if (self->facing != facing)
        self->SetAnimFacing(facing);
    AActorEntity_SetFacingSpeed(self, HorseEntity_GetStateSpeed(self, state));
    if (state != self->state && !self->suppress_effect)
    {
        i32 effect = -1;
        if (state == 4)
            effect = 8;
        if (effect >= 0)
            func_08032384(self, effect, 1);
        else
            func_080323C8(self);
    }
    self->state = state;
}

// 0x080211E4
EC void DogEntity_SetAnimationState(DogEntity * self, u32 state, u32 facing) ENTITY_CODE(080211E4);
EC void DogEntity_SetAnimationState(DogEntity * self, u32 state, u32 facing)
{
    if (state != 12 && self->override_state)
        state = 12;
    u32 anim = DogEntity_GetStateAnimation(self, state);
    if (self->anim_id != anim)
        self->SetAnim(anim);
    if (self->facing != facing)
        self->SetAnimFacing(facing);
    AActorEntity_SetFacingSpeed(self, DogEntity_GetStateSpeed(self, state));
    if (state != self->state)
    {
        i32 effect = -1;
        if (state == 8)
            effect = 8;
        if (effect >= 0)
            func_08032384(self, effect, 1);
        else
            func_080323C8(self);
    }
    self->state = state;
}

// 0x08023FB0
EC void SheepEntity_SetAnimationState(BarnAnimalEntity * self, u32 state, u32 facing) ENTITY_CODE(08023FB0);
EC void SheepEntity_SetAnimationState(BarnAnimalEntity * self, u32 state, u32 facing)
{
    u32 anim = SheepEntity_GetStateAnimation(self, state);
    if (self->anim_id != anim)
        self->SetAnim(anim);
    if (self->facing != facing)
        self->SetAnimFacing(facing);
    AActorEntity_SetFacingSpeed(self, SheepEntity_GetStateSpeed(self, state));
    if (state != self->state)
    {
        i32 effect = -1;
        if (state == 4)
            effect = 8;
        if (effect >= 0)
            func_08032384(self, effect, 1);
        else
            func_080323C8(self);
    }
    self->state = state;
}

// 0x08023308
EC void CowEntity_SetAnimationState(BarnAnimalEntity * self, u32 state, u32 facing) ENTITY_CODE(08023308);
EC void CowEntity_SetAnimationState(BarnAnimalEntity * self, u32 state, u32 facing)
{
    u32 anim = CowEntity_GetStateAnimation(self, state);
    if (self->anim_id != anim)
        self->SetAnim(anim);
    if (self->facing != facing)
        self->SetAnimFacing(facing);
    AActorEntity_SetFacingSpeed(self, CowEntity_GetStateSpeed(self, state));
    if (state != self->state)
    {
        i32 effect = -1;
        if (state == 4)
            effect = 8;
        if (effect >= 0)
            func_08032384(self, effect, 1);
        else
            func_080323C8(self);
    }
    self->state = state;
}

// 0x080238A8
EC BarnAnimalEntity * SheepEntity_Init(BarnAnimalEntity * self, GameObject * game, BarnAnimal * animal) ENTITY_CODE(080238A8);
EC BarnAnimalEntity * SheepEntity_Init(BarnAnimalEntity * self, GameObject * game, BarnAnimal * animal)
{
    Actor location(*animal);
    func_08020038(self, game, &location.location, 0x921);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6284;
    self->animal = animal;
    SheepEntity_SetAnimationState(self, SheepEntity_GetAnimationState(self), self->facing);
    return self;
}

// 0x08022C18
EC BarnAnimalEntity * CowEntity_Init(BarnAnimalEntity * self, GameObject * game, BarnAnimal * animal) ENTITY_CODE(08022C18);
EC BarnAnimalEntity * CowEntity_Init(BarnAnimalEntity * self, GameObject * game, BarnAnimal * animal)
{
    Actor location(*animal);
    func_08020038(self, game, &location.location, 0x2B0);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6310;
    self->animal = animal;
    CowEntity_SetAnimationState(self, CowEntity_GetAnimationState(self), self->facing);
    return self;
}

// 0x080223A8
EC ChickenEntity * ChickenEntity_Init(ChickenEntity * self, GameObject * game, Chicken * animal) ENTITY_CODE(080223A8);
EC ChickenEntity * ChickenEntity_Init(ChickenEntity * self, GameObject * game, Chicken * animal)
{
    Actor location(*animal);
    func_08020038(self, game, &location.location, 0x720);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E639C;
    self->chicken = animal;
    ChickenEntity_SetAnimationState(self, ChickenEntity_GetAnimationState(self), self->facing);
    return self;
}

// 0x080246BC
EC u32 WateringCan_GetEffectiveLevel(u32 minimum, Tool const * tool) ENTITY_CODE(080246BC);
EC u32 WateringCan_GetEffectiveLevel(u32 minimum, Tool const * tool)
{
    u32 level;
    switch (tool->GetId())
    {
    case 32: level = 0; break;
    case 33: level = 1; break;
    case 34: level = 2; break;
    case 35: level = 3; break;
    case 36: level = 4; break;
    case 37:
    case 38: level = 5; break;
    case 39: level = 6; break;
    default: return minimum;
    }
    if (level > minimum)
        minimum = level;
    return minimum;
}

// 0x080245E0
EC u32 SeedTool_GetCropId(u32 value) ENTITY_CODE(080245E0);
EC u32 SeedTool_GetCropId(u32 value)
{
    switch (value)
    {
    case 50: return 0;
    case 51: return 1;
    case 52: return 2;
    case 53: return 3;
    case 54: return 4;
    case 55: return 5;
    case 56: return 6;
    case 57: return 7;
    case 58: return 8;
    case 59: return 9;
    case 60: return 10;
    case 61: return 11;
    case 62: return 12;
    case 63: return 13;
    case 64: return 14;
    case 65: return 20;
    case 66: return 15;
    case 67: return 16;
    case 68:
    {
        u8 chance = rand() % 1000 / 10;
        if (chance <= 4)
            return 18;
        return 17;
    }
    case 69: return 19;
    default: return 21;
    }
}

// 0x08021264
EC u32 DogEntity_GetAnimationState(DogEntity * self) ENTITY_CODE(08021264);
EC u32 DogEntity_GetAnimationState(DogEntity * self)
{
    switch (self->dog->unk_20)
    {
    default: return 3;
    case 0: return self->dog->unk_24.a.unk_02;
    case 1:
    case 2:
    case 3:
    case 5: return 2;
    case 6: return self->dog->unk_24.a.unk_02 ? 11 : 2;
    }
}


// 0x08020060
EC bool AEntity_IsOnCurrentMap(AEntity * self) ENTITY_CODE(08020060);
EC bool AEntity_IsOnCurrentMap(AEntity * self)
{
    return self->game_object->vfunc_14() == self->location_map;
}

// 0x08020130
EC bool AEntity_IsAnimationFinished(AEntity * self) ENTITY_CODE(08020130);
EC bool AEntity_IsAnimationFinished(AEntity * self)
{
    if (self->unk_10.Get() != nullptr)
    {
        bool result = false;
        SpriteAnimator * state = &self->unk_10.Get()->sprite_animator;
        bool active = state->remaining_q8 != 0 && state->speed_q8 != 0;
        if (!active || WillSpriteAnimationLoop(state))
            result = true;
        return result;
    }
    return true;
}

// 0x080202A0
EC Box AEntity_GetStandardCollisionBox(AEntity const * self) ENTITY_CODE(080202A0);
EC Box AEntity_GetStandardCollisionBox(AEntity const * self)
{
    return Box(-7, -9, 7, 5, self->x_q16 >> 16, self->y_q16 >> 16);
}

// 0x080202C4
EC void func_080202C4(AEntity * self) ENTITY_CODE(080202C4);
EC void func_080202C4(AEntity * self)
{
}

// 0x080202C8
EC void func_080202C8(AEntity * self) ENTITY_CODE(080202C8);
EC void func_080202C8(AEntity * self)
{
}

// 0x08020460
EC bool DogEntity_IsCarryingTarget(DogEntity * self) ENTITY_CODE(08020460);
EC bool DogEntity_IsCarryingTarget(DogEntity * self)
{
    return self->dog->unk_20 == 5;
}

// 0x0802161C
EC u32 func_0802161C(u32 value) ENTITY_CODE(0802161C);
EC u32 func_0802161C(u32 value)
{
    return 0;
}

// 0x08021620
EC u32 func_08021620(u32 value) ENTITY_CODE(08021620);
EC u32 func_08021620(u32 value)
{
    switch (value)
    {
    default:
    case 0: return 6;
    case 1: return 8;
    }
}

// 0x08021F88
EC u32 HorseEntity_GetStateAnimation(HorseEntity * self, u32 anim) ENTITY_CODE(08021F88);
EC u32 HorseEntity_GetStateAnimation(HorseEntity * self, u32 anim)
{
    u16 const * table = gUnk_080F09AC;
    return table[self->horse->GetGrowthStage() * 7 + anim];
}

// 0x080220D4
EC u32 HorseEntity_GetAnimationState(HorseEntity * self) ENTITY_CODE(080220D4);
EC u32 HorseEntity_GetAnimationState(HorseEntity * self)
{
    Horse * horse = self->horse;
    switch (i32(horse->unk_20))
    {
    default: return 0;
    case 0: return horse->unk_24.a.unk_02;
    case 1: return 2;
    case 2: return 1;
    }
}

// 0x08022320
EC u32 func_08022320(u32 value) ENTITY_CODE(08022320);
EC u32 func_08022320(u32 value)
{
    switch (value)
    {
    default:
    case 0: return 1;
    case 1: return 2;
    }
}

// 0x08022334
EC u32 func_08022334(u32 value) ENTITY_CODE(08022334);
EC u32 func_08022334(u32 value)
{
    switch (value)
    {
    default:
    case 0: return 14;
    case 1: return 15;
    }
}

// 0x08022950
EC u32 ChickenEntity_GetAnimationState(ChickenEntity * self) ENTITY_CODE(08022950);
EC u32 ChickenEntity_GetAnimationState(ChickenEntity * self)
{
    Chicken * chicken = self->chicken;
    switch (i32(chicken->unk_24))
    {
    case 0: return chicken->unk_28.unk_02;
    case 1: return (chicken->unk_28.unk_02 & 0x7F) ? 4 : 1;
    default: return 0;
    }
}

// 0x08022980
EC u32 ChickenEntity_GetStateAnimation(ChickenEntity * self, u32 anim) ENTITY_CODE(08022980);
EC u32 ChickenEntity_GetStateAnimation(ChickenEntity * self, u32 anim)
{
    u16 const * table = gUnk_080F0A68;
    return table[self->chicken->GetGrowthStage() * 5 + anim];
}

// 0x080229A4
EC u32 ChickenEntity_GetStateSpeed(AEntity * self, u32 value) ENTITY_CODE(080229A4);
EC u32 ChickenEntity_GetStateSpeed(AEntity * self, u32 value)
{
    switch (value)
    {
    case 1: return 0x8000;
    default: return 0;
    }
}

// 0x08022B58
EC u32 func_08022B58(u32 value) ENTITY_CODE(08022B58);
EC u32 func_08022B58(u32 value)
{
    return 0;
}

// 0x08022B5C
EC u32 func_08022B5C(u32 value) ENTITY_CODE(08022B5C);
EC u32 func_08022B5C(u32 value)
{
    switch (value)
    {
    default:
    case 0: return 6;
    case 1: return 7;
    }
}

// 0x08023378
EC u32 CowEntity_GetAnimationState(BarnAnimalEntity * self) ENTITY_CODE(08023378);
EC u32 CowEntity_GetAnimationState(BarnAnimalEntity * self)
{
    BarnAnimal * animal = self->animal;
    switch (i32(animal->unk_28))
    {
    default: return 0;
    case 0: return animal->unk_2C.a.unk_02;
    case 1: return (animal->unk_2C.a.unk_02 & 0x7F) ? 6 : 1;
    case 2:
    case 3: return 1;
    }
}

// 0x08023400
EC u32 CowEntity_GetStateSpeed(AEntity * self, u32 value) ENTITY_CODE(08023400);
EC u32 CowEntity_GetStateSpeed(AEntity * self, u32 value)
{
    switch (value)
    {
    case 1: return 0x8000;
    default: return 0;
    }
}

// 0x080237F0
EC u32 func_080237F0(u32 value) ENTITY_CODE(080237F0);
EC u32 func_080237F0(u32 value)
{
    switch (value)
    {
    default:
    case 0:
    case 1: return 1;
    case 2: return 2;
    }
}

// 0x08023804
EC u32 func_08023804(u32 value) ENTITY_CODE(08023804);
EC u32 func_08023804(u32 value)
{
    switch (value)
    {
    default:
    case 0: return 9;
    case 1: return 11;
    case 2: return 12;
    }
}

// 0x08024020
EC u32 SheepEntity_GetAnimationState(BarnAnimalEntity * self) ENTITY_CODE(08024020);
EC u32 SheepEntity_GetAnimationState(BarnAnimalEntity * self)
{
    BarnAnimal * animal = self->animal;
    switch (i32(animal->unk_28))
    {
    default: return 0;
    case 0: return animal->unk_2C.a.unk_02;
    case 1: return (animal->unk_2C.a.unk_02 & 0x7F) ? 6 : 1;
    case 2:
    case 3: return 1;
    }
}

// 0x080240BC
EC u32 SheepEntity_GetStateSpeed(AEntity * self, u32 value) ENTITY_CODE(080240BC);
EC u32 SheepEntity_GetStateSpeed(AEntity * self, u32 value)
{
    switch (value)
    {
    case 1: return 0x8000;
    default: return 0;
    }
}

// 0x080244C4
EC u32 func_080244C4(u32 value) ENTITY_CODE(080244C4);
EC u32 func_080244C4(u32 value)
{
    switch (value)
    {
    default:
    case 0: return 1;
    case 1: return 2;
    }
}

// 0x080244D8
EC u32 func_080244D8(u32 value) ENTITY_CODE(080244D8);
EC u32 func_080244D8(u32 value)
{
    switch (value)
    {
    default:
    case 0: return 10;
    case 1: return 13;
    }
}

// 0x08024BE8
EC void func_08024BE8(AEntity * self) ENTITY_CODE(08024BE8);
EC void func_08024BE8(AEntity * self)
{
    self->AEntity::vfunc_10();
    func_0802A588(self);
}

// 0x08024C54
EC void FarmerEntity_EnableAndClearEffect(AEntity * self) ENTITY_CODE(08024C54);
EC void FarmerEntity_EnableAndClearEffect(AEntity * self)
{
    self->unk_06 = true;
    func_080323C8(self);
}

// 0x08024C64
EC void FarmerEntity_DisableAndClearEffect(AEntity * self) ENTITY_CODE(08024C64);
EC void FarmerEntity_DisableAndClearEffect(AEntity * self)
{
    self->unk_06 = false;
    func_080323C8(self);
}

// 0x08024C88
EC void func_08024C88(u8 * self) ENTITY_CODE(08024C88);
EC void func_08024C88(u8 * self)
{
    self[0x3C] = 8;
    self[0x3D] = 255;
}

// 0x08024C98
EC void FarmerEntity_ClearAction(u8 * self) ENTITY_CODE(08024C98);
EC void FarmerEntity_ClearAction(u8 * self)
{
    self[0x3C] = 0;
}
