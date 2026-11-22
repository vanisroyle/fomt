#include "entity_actor.hh"
#include "harvest_sprite.hh"
#include "smart_ptr.hh"
#include "utility/vec2.hh"

#define NPC_EXTRA_CODE(address) SECTION(".text.npc_extra." #address)
#define NPC_SIDE_CODE(address) SECTION(".text.npc_side." #address)

struct ScheduledNpcCalendar
{
    u32 weather;
    u32 reserved;
    u8 year : 3;
    u8 flags : 5;
    GameDate date;
    GameTime time;

    u32 GetWeekday() const
    {
        register u32 years asm("r2") = year;
        register u32 season asm("r0") = date.season;
        register u32 day asm("r3") = date.day;
        asm("" : "+l"(years), "+l"(season), "+l"(day));
        years += 6;
        asm("" : "+l"(years));
        day += season * 30 + years * 120;
        if (day > 6)
            day %= 7;
        day &= 7;
        asm("" : "+l"(day));
        return day;
    }
};

struct ScheduledNpcServices;
struct ScheduledNpcServicesVTable
{
    STRUCT_PAD(0x00, 0x144);
    ScheduledNpcCalendar * (*get_calendar)(ScheduledNpcServices *);
};

struct ScheduledNpcServices
{
    ScheduledNpcServicesVTable * vtable;
};

struct DirectedScheduledNpc : AActorEntity
{
    DirectedScheduledNpc();
    STRUCT_PAD(0x30, 0x44);
    u8 direction : 2;
    u8 flags : 6;
};

EC ActorLocation GetNpcScheduleMap7Arrival(u32 index);
EC ActorLocation GetNpcScheduleMap7Departure(u32 index);
EC u32 GetChildNpcAnimation(AActorEntity * self, u32 movement);
EC u32 GetNpcVariantTwoAnimation(AActorEntity * self, u32 movement);

inline void HideDirectedScheduledNpc(AEntity * self)
{
    self->SetMap(564);
}

EC void RefreshChildNpcSchedule(DirectedScheduledNpc * self) NPC_EXTRA_CODE(08037568);
EC void RefreshChildNpcSchedule(DirectedScheduledNpc * self)
{
    ScheduledNpcServices * world = reinterpret_cast<ScheduledNpcServices *>(self->game_object);
    asm("" : : : "r8");
    register ScheduledNpcCalendar * calendar asm("r1") = world->vtable->get_calendar(world);
    asm("" : "+l"(calendar));
    register ScheduledNpcCalendar * weather_calendar asm("r5") = calendar;
    asm("" : "+l"(weather_calendar));
    if (u32(calendar->time.clock_hour - 6) <= 9 && calendar->GetWeekday() != 0 && weather_calendar->weather == 0)
    {
        self->SetLocation(GetNpcScheduleMap7Arrival(self->direction));
        u32 animation = GetChildNpcAnimation(self, 0);
        if (self->anim_id != animation)
            self->SetAnim(animation);
    }
    else
        HideDirectedScheduledNpc(self);
}

EC void RefreshVariantTwoNpcSchedule(DirectedScheduledNpc * self) NPC_EXTRA_CODE(08037714);
EC void RefreshVariantTwoNpcSchedule(DirectedScheduledNpc * self)
{
    ScheduledNpcServices * world = reinterpret_cast<ScheduledNpcServices *>(self->game_object);
    asm("" : : : "r8");
    register ScheduledNpcCalendar * calendar asm("r1") = world->vtable->get_calendar(world);
    asm("" : "+l"(calendar));
    register ScheduledNpcCalendar * weather_calendar asm("r5") = calendar;
    asm("" : "+l"(weather_calendar));
    if (u32(calendar->time.clock_hour - 6) <= 9 && calendar->GetWeekday() != 1 && weather_calendar->weather == 0)
    {
        self->SetLocation(GetNpcScheduleMap7Departure(self->direction));
        u32 animation = GetNpcVariantTwoAnimation(self, 0);
        if (self->anim_id != animation)
            self->SetAnim(animation);
    }
    else
        HideDirectedScheduledNpc(self);
}

struct SpriteWorkerEntity : AActorEntity
{
    SpriteWorkerEntity();
    u8 sprite_id;
    STRUCT_PAD(0x31, 0x34);
    HarvestSprite * record;
    void * work_targets;
    void * work_context;
    u8 work_state;
    u8 work_pause;
    u8 work_active;
};

EC u32 const gUnk_080F149C[];
EC u16 const gUnk_080F14E2[];
EC u8 const vtable_unk_080E68C8[];
EC ActorLocation GetRosterNpcLocation(void * roster, u32 id);
EC HarvestSprite * GetRosterHarvestSpriteByIndexConst(void * roster, u32 index);
EC ActorLocation GetHarvestSpriteHomeLocation(u32 index);
EC u32 ComputeSpriteWorkSpeed(u32 experience);
EC void InitializeSpriteWorkerActorBase(AActorEntity * self, GameObject * game, ActorLocation const & location, u32 layer, u32 animation) asm("__12AActorEntityP10GameObjectRC13ActorLocationUiUi");
EC void SetSpriteWorkerMovementState(SpriteWorkerEntity * self, i32 state, i32 facing, i32 speed);

EC SpriteWorkerEntity * InitializeSpriteWorkerEntity(SpriteWorkerEntity * self, GameObject * game, u32 index, void * roster, void * work_context) NPC_SIDE_CODE(08033928);
EC SpriteWorkerEntity * InitializeSpriteWorkerEntity(SpriteWorkerEntity * self, GameObject * game, u32 index, void * roster, void * work_context)
{
    ActorLocation location = GetRosterNpcLocation(roster, gUnk_080F149C[index]);
    InitializeSpriteWorkerActorBase(self, game, location, 2, gUnk_080F14E2[index]);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E68C8;
    self->sprite_id = index;
    self->record = GetRosterHarvestSpriteByIndexConst(roster, index);
    self->work_targets = reinterpret_cast<u8 *>(roster) + 0x438;
    self->work_context = work_context;
    self->work_pause = 0;
    self->work_active = 0;
    u32 speed = 0x8000;
    u32 state = 0;
    switch (self->record->unk_1C)
    {
    case 4: state = 4; break;
    case 2: state = self->record->unk_20.unk_02; break;
    case 0:
        self->SetLocation(GetHarvestSpriteHomeLocation(index));
        break;
    case 3:
        state = 1;
        speed = ComputeSpriteWorkSpeed(self->record->GetTaskExp(self->record->GetCurrentTask()));
        break;
    case 1: case 5: state = 1; break;
    }
    self->work_state = ~state;
    SetSpriteWorkerMovementState(self, state, self->facing, speed);
    return self;
}

struct SpriteWorkerGraphics
{
    AActorEntity * actor;
    void const * vtable;
    u8 layers[0x84];
    u8 effect_renderer[0x40];
    STRUCT_PAD(0xCC, 0xD0);
    u16 effect_timer;
};

struct SpriteWorkerBank;
struct SpriteWorkerGraphicsServices;
struct SpriteWorkerGraphicsVTable
{
    STRUCT_PAD(0x00, 0x68);
    SpriteWorkerBank * (*get_bank)(SpriteWorkerGraphicsServices *);
};

struct SpriteWorkerGraphicsServices
{
    SpriteWorkerGraphicsVTable * vtable;
};

EC u32 const gUnk_080F14B8[];
EC u16 const gUnk_080F14F0[];
EC u8 const vtable_unk_080E6904[];
EC void InitializeSideActorSpriteLayers(SpriteWorkerGraphics * self, AActorEntity * actor, u32 layer, u32 palette, u32 overlay_offset, u32 overlay_flags, u32 overlay_visible, bool hidden);
EC void InitializeAnimatedMapSpriteSinglePalette(void * renderer, SpriteWorkerBank * bank, u32 animation, GameObject * game, u32 layer, u32 palette, bool hidden);

EC SpriteWorkerGraphics * InitializeSpriteWorkerGraphics(SpriteWorkerGraphics * self, SpriteWorkerEntity * actor) NPC_SIDE_CODE(08034A14);
EC SpriteWorkerGraphics * InitializeSpriteWorkerGraphics(SpriteWorkerGraphics * input, SpriteWorkerEntity * input_actor)
{
    register SpriteWorkerGraphics * self asm("r8") = input;
    asm("" : "+h"(self));
    register SpriteWorkerEntity * actor asm("r4") = input_actor;
    asm("" : : "l"(actor));
    register u32 const * palettes asm("r9") = gUnk_080F14B8;
    asm("" : "+h"(palettes));
    register u8 * sprite_id asm("r5") = &actor->sprite_id;
    asm("" : "+l"(sprite_id));
    InitializeSideActorSpriteLayers(self, input_actor, 3, palettes[*sprite_id], 0, 4, 0, false);
    register void const * vtable asm("r0") = vtable_unk_080E6904;
    asm("" : "+l"(vtable));
    self->vtable = vtable;
    SpriteWorkerGraphicsServices * services = reinterpret_cast<SpriteWorkerGraphicsServices *>(actor->game_object);
    SpriteWorkerBank * bank = services->vtable->get_bank(services);
    register u16 const * animations asm("r2") = gUnk_080F14F0;
    asm("" : "+l"(animations));
    u32 id = *sprite_id;
    u32 animation = animations[id];
    GameObject * game = actor->game_object;
    u32 const * palette = &palettes[id];
    asm("" : "+r"(palette));
    InitializeAnimatedMapSpriteSinglePalette(self->effect_renderer, bank, animation, game, 2, *palette, false);
    self->effect_timer = 0;
    return self;
}

struct ReturningNpcRecord
{
    STRUCT_PAD(0x00, 0x10);
    u16 turn_timer;
};

struct ReturningNpcEntity : AActorEntity
{
    ReturningNpcEntity();
    STRUCT_PAD(0x30, 0x34);
    ReturningNpcRecord * record;
};

struct ActorActionReset {};
EC bool IsWithinSideEntityExit(i32 x, i32 y);
EC u32 ChooseEntityFacingTowardPoint(i32 x, i32 y, i32 target_x, i32 target_y, u32 argument, u32 facing);
EC u32 RollEntityMovementRandom(u32 limit);
EC void FinishNpcReturn(ReturningNpcRecord * record, ActorActionReset const & state) asm("func_0809C098");

inline i32 GetReturnTargetX(Vec2 const & point)
{
    register Vec2 const * source asm("r0") = &point;
    asm("" : "+l"(source));
    register i32 x asm("r3") = source->x;
    asm("" : "+l"(x));
    return x;
}

inline void StoreNpcReturnTimer(u16 * timer, u32 value)
{
    register u16 * destination asm("r0") = timer;
    asm("" : "+l"(destination));
    *destination = value;
}

inline i32 GetNpcReturnDistance(i32 target, i32 coordinate)
{
    register i32 left asm("r1") = target;
    register i32 right asm("r2") = coordinate;
    asm("" : "+l"(left), "+l"(right));
    register i32 distance asm("r0") = left - right;
    asm("" : "+l"(distance));
    return distance;
}

struct NpcReturnPoint : Vec2
{
    NpcReturnPoint() {}
    NpcReturnPoint(u32 point_x, u32 point_y)
    {
        i16 * origin = &x;
        *origin = point_x;
        y = point_y;
    }
};

struct NpcReturnScratch
{
    NpcReturnPoint destination;
    ActorActionReset next_state;
    volatile u32 argument;
    u16 * volatile turn_timer;

    NpcReturnScratch(u32 update_argument) : argument(update_argument) {}
};

inline void * operator new(size_t, NpcReturnPoint * point) { return point; }

EC void UpdateReturningNpcFacing(void * state, ReturningNpcEntity * self, u32 argument) NPC_EXTRA_CODE(08039204);
EC void UpdateReturningNpcFacing(void * state, ReturningNpcEntity * self, u32 argument)
{
    NpcReturnScratch scratch(argument);
    register ReturningNpcRecord * record asm("r8") = self->record;
    asm("" : "+h"(record));
    register u16 * turn_timer asm("r1") = &record->turn_timer;
    asm("" : "+l"(turn_timer));
    scratch.turn_timer = turn_timer;
    register ReturningNpcRecord * timer_record asm("r2") = record;
    asm("" : "+l"(timer_record));
    u32 timer = timer_record->turn_timer;
    if (timer != 0)
        timer--;
    register u32 loaded_map asm("r3") = self->location_map;
    asm("" : "+l"(loaded_map));
    register u32 map asm("r12") = loaded_map;
    asm("" : "+h"(map));
    register u32 x_offset asm("r1") = 10;
    asm("" : "+l"(x_offset));
    register i32 pixel_x asm("r0");
    asm("ldrsh %0, [%1, %2]" : "=l"(pixel_x) : "l"(self), "l"(x_offset), "m"(self->x_q16));
    register i32 x asm("r9") = pixel_x;
    asm("" : "+h"(x));
    register u32 actor_y_offset asm("r2") = 14;
    asm("" : "+l"(actor_y_offset));
    i32 y;
    asm("ldrsh %0, [%1, %2]" : "=l"(y) : "l"(self), "l"(actor_y_offset), "m"(self->y_q16));
    y > 352 ? new (&scratch.destination) NpcReturnPoint(264, 720) : new (&scratch.destination) NpcReturnPoint(340, -16);
    register Vec2 const * point asm("r1") = &scratch.destination;
    asm("" : "+l"(point));
    register i32 target_x asm("r10") = GetReturnTargetX(*point);
    asm("" : "+h"(target_x));
    register u32 y_offset asm("r3") = 2;
    asm("" : "+l"(y_offset));
    i32 target_y;
    asm("ldrsh %0, [%1, %2]" : "=l"(target_y) : "l"(point), "l"(y_offset), "m"(point->y));
    register u32 current_map asm("r0") = map;
    asm("" : "+l"(current_map));
    if (current_map == 2 && (__builtin_abs(GetNpcReturnDistance(target_x, x)) > 15 || __builtin_abs(target_y - y) > 15))
    {
        if (timer == 0)
        {
            if (IsWithinSideEntityExit(x, y))
            {
                u32 facing = 0;
                if (y >= target_y)
                    facing = 1;
                if (self->facing != facing)
                    self->SetAnimFacing(facing);
            }
            else
            {
                register u32 old_facing asm("r0") = self->facing;
                asm("" : "+l"(old_facing));
                register u32 update_argument asm("r3") = scratch.argument;
                asm("" : "+l"(update_argument));
                u32 facing = ChooseEntityFacingTowardPoint(x, y, target_x, target_y, update_argument, old_facing);
                if (self->facing != facing)
                    self->SetAnimFacing(facing);
            }
            timer = RollEntityMovementRandom(30) + 1;
        }
        StoreNpcReturnTimer(scratch.turn_timer, timer);
    }
    else
    {
        HideDirectedScheduledNpc(self);
        FinishNpcReturn(record, scratch.next_state);
    }
}

struct PolicyDrivenNpcEntity;
struct NpcActivityPolicy;
struct NpcActivityPolicyVTable
{
    STRUCT_PAD(0x00, 0x0C);
    u32 (*choose_movement)(NpcActivityPolicy *, PolicyDrivenNpcEntity *);
};

struct NpcActivityPolicy
{
    NpcActivityPolicy(NpcActivityPolicyVTable const * table) : vtable(table) {}
    NpcActivityPolicyVTable const * vtable;
};

struct NpcActivityPolicies
{
    SmartPtr<NpcActivityPolicy> entries[5];
};

struct PolicyDrivenNpcRecord : Actor
{
    PolicyDrivenNpcRecord();
    u32 reserved;
    u32 activity;
};

struct PolicyDrivenNpcEntity : AActorEntity
{
    PolicyDrivenNpcEntity();
    STRUCT_PAD(0x30, 0x34);
    PolicyDrivenNpcRecord * record;
    NpcActivityPolicies policies;
    u32 movement;
    u32 previous_facing;
};

inline void * operator new(size_t, NpcActivityPolicies * policies) { return policies; }

EC u8 const vtable_unk_080E74DC[];
EC NpcActivityPolicyVTable const vtable_unk_080E74CC;
EC NpcActivityPolicyVTable const vtable_unk_080E74BC;
EC NpcActivityPolicyVTable const vtable_unk_080E74AC;
EC NpcActivityPolicyVTable const vtable_unk_080E749C;
EC NpcActivityPolicyVTable const vtable_unk_080E748C;
EC void InitializeActiveNpcActor(AActorEntity * self, GameObject * game, Actor const & actor, u32 animation) asm("func_08020038");
EC u32 GetActiveNpcMovementSpeed(AEntity * self, u32 movement);
EC void AActorEntity_SetFacingSpeed(AActorEntity * self, i32 speed);
EC u32 GetNpcVariantFiveAnimation(AEntity * self, u32 movement);

EC PolicyDrivenNpcEntity * InitializeActivityNpcEntity(PolicyDrivenNpcEntity * self, GameObject * game, PolicyDrivenNpcRecord * record) NPC_EXTRA_CODE(080398A4);
EC PolicyDrivenNpcEntity * InitializeActivityNpcEntity(PolicyDrivenNpcEntity * self, GameObject * game, PolicyDrivenNpcRecord * record)
{
    Actor actor(*record);
    InitializeActiveNpcActor(self, game, actor, 2503);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E74DC;
    self->record = record;
    new (&self->policies) NpcActivityPolicies;
    self->policies.entries[0] = new NpcActivityPolicy(&vtable_unk_080E74CC);
    self->policies.entries[1] = new NpcActivityPolicy(&vtable_unk_080E74BC);
    self->policies.entries[2] = new NpcActivityPolicy(&vtable_unk_080E74AC);
    self->policies.entries[3] = new NpcActivityPolicy(&vtable_unk_080E749C);
    self->policies.entries[4] = new NpcActivityPolicy(&vtable_unk_080E748C);
    NpcActivityPolicy * policy = self->policies.entries[self->record->activity].Get();
    u32 movement = policy->vtable->choose_movement(policy, self);
    AActorEntity_SetFacingSpeed(self, GetActiveNpcMovementSpeed(self, movement));
    u32 animation = GetNpcVariantFiveAnimation(self, movement);
    if (self->anim_id != animation)
        self->SetAnim(animation);
    self->movement = movement;
    self->previous_facing = self->facing;
    return self;
}
