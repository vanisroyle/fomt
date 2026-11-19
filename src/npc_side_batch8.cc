#include "entity_actor.hh"
#include "g1_npc.hh"
#include "harvest_sprite.hh"
#include "schedule_info.hh"

#define NPC_SIDE_CODE(address) SECTION(".text.npc_side." #address)
#define NPC_EXTRA_CODE(address) SECTION(".text.npc_extra." #address)

struct VariantFiveWorld
{
    STRUCT_PAD(0x00, 0x2181);
    u8 flags;
};

struct VariantFiveScheduledNpc : AActorEntity
{
    VariantFiveScheduledNpc();
    Npc * record;
    VariantFiveWorld * world;
};

struct MarriageNpcWorld
{
    STRUCT_PAD(0x00, 0x1CD4);
    u8 roster[0x497];
    u8 first_flags;
    u8 second_flags;
};

struct MarriageScheduledNpc : AActorEntity
{
    MarriageScheduledNpc();
    Npc * record;
    MarriageNpcWorld * world;
};

struct MarriageNpcClock
{
    STRUCT_PAD(0x00, 0x0A);
    GameTime time;
};

struct MarriageNpcServices;
struct MarriageNpcVTable
{
    STRUCT_PAD(0x00, 0x144);
    MarriageNpcClock * (*get_clock)(MarriageNpcServices *);
};

struct MarriageNpcServices
{
    MarriageNpcVTable * vtable;
};

struct NorthernArrivalServices;
struct NorthernArrivalVTable
{
    STRUCT_PAD(0x00, 0x114);
    void (*arrive_at_exit)(NorthernArrivalServices *);
};

struct NorthernArrivalServices
{
    NorthernArrivalVTable * vtable;
};

struct SpriteWorkTarget
{
    u8 kind_flags;
    u8 index;
    u8 row;
    u8 padding;
    u32 extra;
};

struct SpriteWorkerTaskStorage
{
    u8 kind_flags;
    u8 payload[7];
};

struct SpriteWorkerLocationStorage
{
    u16 packed_map_x;
    i16 packed_x_y;
    i16 packed_y;
};

struct ActivitySpriteAnimationPair
{
    STRUCT_PAD(0x00, 0x04);
    u16 animation_id;
    u16 overlay_animation_id;
};

struct SpriteWorkerEntity : AActorEntity
{
    SpriteWorkerEntity();
    u8 sprite_id;
    STRUCT_PAD(0x31, 0x34);
    HarvestSprite * record;
    SpriteWorkTarget * work_targets;
    void * work_context;
    u8 work_state;
    u8 work_pause;
    u8 work_active;
};

struct SpriteWorkMapServices;
struct SpriteWorkMapVTable
{
    STRUCT_PAD(0x00, 0x44);
    AEntity * (*get_actor)(SpriteWorkMapServices *, u32);
};

struct SpriteWorkMapServices
{
    SpriteWorkMapVTable * vtable;
};

struct SideEffectActorView;
struct SideEffectActorVTable
{
    STRUCT_PAD(0x00, 0x60);
    u8 (*is_available)(SideEffectActorView *);
};

struct SideEffectActorView
{
    GameObject * game_object;
    u16 location_map;
    u8 enabled;
    u8 padding;
    i32 x_q16;
    i32 y_q16;
    UnknownEntityThing * sprite;
    SideEffectActorVTable * vtable;
};

struct SideEffectSearchServices;
struct SideEffectSearchVTable
{
    STRUCT_PAD(0x00, 0x40);
    SideEffectActorView * (*get_actor)(SideEffectSearchServices *, u32);
};

struct SideEffectSearchServices
{
    SideEffectSearchVTable * vtable;
};

struct ActivityNpcEntityView
{
    STRUCT_PAD(0x00, 0x10);
    UnknownEntityThing * sprite;
    void * vtable;
    STRUCT_PAD(0x18, 0x34);
    Actor * record;
    void * particles[5];
};

struct ActivityNpcLocationStorage
{
    u8 bytes[sizeof(ActorLocation)];
};

struct NearbyReactiveNpc : AActorEntity
{
    NearbyReactiveNpc();
    virtual void vfunc_3C(u32 active);
    STRUCT_PAD(0x30, 0x3F);
    u8 reaction_timer;
};

struct ScheduledNpcPathView : AActorEntity
{
    ScheduledNpcPathView();
    Npc * record;
    void * world;
    ScheduleInfo const * schedule_info;
    u8 schedule_index;
    u8 entry_index;
    u8 point_index;
    u8 reaction_timer;
    u16 path_elapsed;
};

EC void SetNpcWalkingAnimation(AActorEntity * self, u32 animation);
EC void SetNpcIdleAnimation(AActorEntity * self, u32 animation);
EC void func_08034EB4(AActorEntity * self, u32 reason);
EC void func_08034F00(AActorEntity * self, i32 reason);
EC void ReadActivityNpcLocation(ActivityNpcLocationStorage *, ActivityNpcEntityView const *) asm("GetLocation__C12AActorEntity");
EC void WriteActivityNpcLocation(Actor *, ActivityNpcLocationStorage const *) asm("SetLocation__5ActorRC13ActorLocation");
EC void __builtin_delete(void *);
EC char vtable_unk_080E74DC[];
EC char ActivityNpcBaseVTable[] asm("__vt_7AEntity");
EC u32 GetRosterMarriageCandidate(void const * roster);
EC bool IsNpcAtNorthernExit(AEntity * self);
EC u8 * GetActiveRosterNpc(u8 * roster);
EC u32 GetNamedNpcAgeDays(u8 * npc);
EC bool IsFamilyScheduleAwayFromHome(void const * world, GameTime const & time);
EC bool CanAdvanceScheduledNpcPath(AActorEntity * self);
EC void func_08032384(AActorEntity * self, u32 state, u32 argument);
EC ScheduleInfo const gUnk_080F29C0;
EC ScheduleEntry const * func_080D7ECC(ScheduleEntry const *, ScheduleEntry const *, i32 const *, u32, u32);
EC void ReadSpriteWorkerTaskLocation(SpriteWorkerLocationStorage *, AEntity const *) asm("GetLocation__C7AEntity");
EC void FindSpriteWorkerTask(SpriteWorkerTaskStorage *, void *, GameObject *, SpriteWorkerLocationStorage const *, HarvestSprite::Task, SpriteWorkTarget *) asm("func_080330F4");
EC void SetNpcEventSlot(SpriteWorkTarget *, u8, SpriteWorkerTaskStorage const *);
EC u32 RollEntityMovementRandom(u32 maximum);
EC u8 const gUnk_080F16C2[];
EC u32 func_0805E8F0(void * animation);

EC bool CanAdvanceScheduledNpcPath(AActorEntity * actor) NPC_EXTRA_CODE(08035978);
EC bool CanAdvanceScheduledNpcPath(AActorEntity * actor)
{
    ScheduledNpcPathView * self = reinterpret_cast<ScheduledNpcPathView *>(actor);
    i32 ready;
    if (!self->vfunc_28())
    {
        ScheduleInfo const * info = self->schedule_info;
        if (info != nullptr)
        {
            Schedule const * schedule = info->schedules[self->schedule_index];
            if (schedule != nullptr)
            {
                u8 entry_index = self->entry_index;
                if (u32(entry_index) < u32(schedule->num_time_entries))
                {
                    ScheduleEntry const * entry = &schedule->time_entries[entry_index];
                    PathInfo const * path = entry->path;
                    if (path != nullptr)
                    {
                        MarriageNpcServices * game = reinterpret_cast<MarriageNpcServices *>(self->game_object);
                        MarriageNpcClock * clock = game->vtable->get_clock(game);
                        GameTime const & time = clock->time;
                        i32 hour = u32(time.clock_hour) - 6;
                        if (hour < 0)
                            hour += 24;
                        u32 minutes = hour * 60 + u32(time.clock_minute);
                        if (path->points != nullptr)
                        {
                            u16 count = path->num_points;
                            if (count != 0)
                            {
                                if (u32(self->point_index) >= u32(count))
                                    goto not_ready;
                                if (i32(u32(path->point_type_select) << 31) >= 0)
                                {
                                    ready = 0;
                                    i32 elapsed = self->path_elapsed;
                                    i32 timeout = 750;
                                    if (elapsed < timeout)
                                        goto done;
                                    goto path_ready;
                                }
                            }
                        }
                        ready = 0;
                        if (i32(self->entry_index + 1) < i32(schedule->num_time_entries) && minutes >= u32(entry[1].time))
                            path_ready: ready = 1;
                        done: return ready;
                    }
                }
            }
        }
    }
    not_ready:
    return false;
}

static inline u32 AdvanceSideActorOverlay(UnknownEntityThing * self)
{
    i16 zero = 0;
    if (self->unk_87 == zero)
    {
        u32 result = func_0805E8F0(&self->sprite_animator_70);
        if (i32(result << 30) < zero)
            self->unk_84 = 1;
        return result;
    }
    self->unk_87 = zero;
    return 2;
}

EC void UpdateSideActorAnimationLayers(UnknownEntityThing * self) NPC_SIDE_CODE(0803260C);
EC void UpdateSideActorAnimationLayers(UnknownEntityThing * self)
{
    u8 * changed = &self->unk_47;
    u8 state;
    if (self->unk_47 == 0)
    {
        if (i32(func_0805E8F0(&self->sprite_animator) << 30) < 0)
        {
            changed = &self->unk_44;
            state = 1;
            goto store_state;
        }
    }
    else
    {
        state = 0;
        store_state:
        *changed = state;
    }
    u32 mode = self->unk_8A_0;
    if (mode != 0)
    {
        i32 hide = 0;
        u32 result = AdvanceSideActorOverlay(self);
        if (i32(result << 29) < 0 && mode != 2)
            hide = 1;
        if (hide)
            self->unk_8A_0 = 0;
    }
}

EC i32 SelectActivitySpriteAnimationPair(void * self, ActivitySpriteAnimationPair * sprite, u8 variant) NPC_EXTRA_CODE(0803A144);
EC i32 SelectActivitySpriteAnimationPair(void * self, ActivitySpriteAnimationPair * sprite, u8 variant)
{
    u8 selection = variant;
    u8 const * overlay;
    i32 index;
    do
    {
        if (i32(u32(variant) << 24) < 0)
            selection = RollEntityMovementRandom(3);
        u8 const * table = gUnk_080F16C2;
        do
        {
            index = i8(selection);
            sprite->animation_id = *reinterpret_cast<u16 const *>(table + index * 4);
            overlay = table + (index * 2 + 1) * 2;
            sprite->overlay_animation_id = *reinterpret_cast<u16 const *>(overlay);
        } while (0);
    } while (0);
    return index;
}

EC i32 IsSpriteWorkerOutOfTasks(SpriteWorkerEntity * self) NPC_SIDE_CODE(08033B24);
EC i32 IsSpriteWorkerOutOfTasks(SpriteWorkerEntity * self)
{
    SpriteWorkerTaskStorage target;
    SpriteWorkerLocationStorage location;
    GameObject * game;
    u32 sprite_id = self->sprite_id;
    SpriteWorkTarget const * current = self->work_targets;
    current += sprite_id;
    if (u32(current->kind_flags) << 28)
        return 0;
    game = self->game_object;
    ReadSpriteWorkerTaskLocation(&location, self);
    FindSpriteWorkerTask(&target, self->work_context, game, &location, self->record->GetCurrentTask(), self->work_targets);
    SpriteWorkerTaskStorage const * result = &target;
    i32 empty = 0;
    if ((u32(result->kind_flags) << 28) == 0)
        empty = 1;
    return empty;
}

EC bool TryAssignSpriteWorkerTask(SpriteWorkerEntity * self) NPC_SIDE_CODE(08034260);
EC bool TryAssignSpriteWorkerTask(SpriteWorkerEntity * self)
{
    SpriteWorkerTaskStorage target;
    SpriteWorkerLocationStorage location;
    struct Timer { u32 duration : 16; u32 : 16; } timer;
    GameObject * game;
    bool found;
    if (self->work_pause)
        return false;
    game = self->game_object;
    ReadSpriteWorkerTaskLocation(&location, self);
    FindSpriteWorkerTask(&target, self->work_context, game, &location, self->record->GetCurrentTask(), self->work_targets);
    if ((u32(target.kind_flags) << 28) == 0)
    {
        self->work_pause = 1;
        found = false;
    }
    else
    {
        SetNpcEventSlot(self->work_targets, self->sprite_id, &target);
        timer.duration = 0;
        self->record->method_0809E7E4(reinterpret_cast<UnkBarnAnimal2C const *>(&timer));
        found = true;
    }
    return found;
}

EC bool IsFamilyScheduleAwayFromHome(void const * world, GameTime const & time) NPC_SIDE_CODE(08034C64);
EC bool IsFamilyScheduleAwayFromHome(void const * world, GameTime const & time)
{
    Schedule const * schedule = gUnk_080F29C0.schedules[gUnk_080F29C0.select_schedule(world)];
    if (schedule != nullptr)
    {
        i32 hour = u32(time.clock_hour) - 6;
        if (hour < 0)
            hour += 24;
        i32 minutes = hour * 60 + u32(time.clock_minute);
        ScheduleEntry const * entries = schedule->time_entries;
        ScheduleEntry const * entry = func_080D7ECC(entries, entries + schedule->num_time_entries, &minutes, 0, 0);
        u32 index = (entry - entries) - 1;
        if (index < u32(schedule->num_time_entries))
        {
            PathInfo const * path = schedule->time_entries[index].path;
            if (path != nullptr)
            {
                u32 different_map = path->location_start ^ 29;
                return (u32(-different_map) | different_map) >> 31;
            }
        }
    }
    return false;
}

static inline i16 GetNearbyNpcCoordinate(i32 coordinate_q16)
{
    return i16(coordinate_q16 >> 16);
}

EC void ReactToNearbyNpcLocation(NearbyReactiveNpc * self, Location const & location) NPC_EXTRA_CODE(08035A48);
EC void ReactToNearbyNpcLocation(NearbyReactiveNpc * self, Location const & location)
{
    u32 map = self->location_map;
    if (map == location.GetMap())
    {
        i32 dx = location.x - GetNearbyNpcCoordinate(self->x_q16);
        if (dx < 0)
            dx = -dx;
        if (dx <= 151)
        {
            i32 dy = location.y - GetNearbyNpcCoordinate(self->y_q16);
            if (dy < 0)
                dy = -dy;
            if (dy <= 111 && !CanAdvanceScheduledNpcPath(self) && !self->vfunc_28())
            {
                func_08032384(self, 2, 0);
                self->reaction_timer = 120;
                self->vfunc_3C(1);
            }
        }
    }
}

static inline u32 GetNpcMarriageAnimationState(MarriageNpcWorld const * world)
{
    u32 low = world->first_flags >> 7;
    u32 high = world->second_flags;
    high &= 1;
    high *= 2;
    return high | low;
}

EC void RefreshNpcVariantSixAnimations(MarriageScheduledNpc * self) NPC_EXTRA_CODE(08036900);
EC void RefreshNpcVariantSixAnimations(MarriageScheduledNpc * self)
{
    u32 walking_animation = 1982;
    u32 idle_animation = 1986;
    GameTime const * time;
    MarriageNpcWorld * world = self->world;
    u8 * roster = world->roster;
    do
    {
        if (GetRosterMarriageCandidate(roster) == 25 && self->location_map == 29)
        {
            if (IsNpcAtNorthernExit(self) || 1 == GetNpcMarriageAnimationState(world))
                walking_animation = 2002;
            else
            {
                u8 * npc = GetActiveRosterNpc(roster);
                if (npc != nullptr && GetNamedNpcAgeDays(npc) <= 59)
                {
                    MarriageNpcServices * game = reinterpret_cast<MarriageNpcServices *>(self->game_object);
                    MarriageNpcClock * clock = game->vtable->get_clock(game);
                    time = &clock->time;
                    if (IsFamilyScheduleAwayFromHome(world, *time))
                    {
                        walking_animation = 1994;
                        idle_animation = 1998;
                    }
                }
            }
        }
        SetNpcWalkingAnimation(self, walking_animation);
    } while (0);
    SetNpcIdleAnimation(self, idle_animation);
}

EC void DestroyActivityNpc(ActivityNpcEntityView * self, u32 flags) NPC_EXTRA_CODE(080399C0);
EC void DestroyActivityNpc(ActivityNpcEntityView * self, u32 flags)
{
    UnknownEntityThing * owned_sprite;
    ActivityNpcLocationStorage location;
    self->vtable = vtable_unk_080E74DC;
    Actor * record = self->record;
    ReadActivityNpcLocation(&location, self);
    WriteActivityNpcLocation(record, &location);
    void ** first = self->particles;
    if (self->particles != nullptr)
    {
        void ** last = self->particles + 5;
        if (first != last)
        {
            do
            {
                --last;
                __builtin_delete(*last);
            } while (self->particles != last);
        }
    }
    self->vtable = ActivityNpcBaseVTable;
    UnknownEntityThing * sprite = self->sprite;
    owned_sprite = sprite;
    if (owned_sprite != nullptr)
        delete sprite;
    if (flags & 1)
        __builtin_delete(self);
}

EC i32 FindNearestSideEffectActor(SideEffectSearchServices * game, i32 map, i32 x, i32 y) NPC_EXTRA_CODE(08039134);
EC i32 FindNearestSideEffectActor(SideEffectSearchServices * game, i32 map, i32 x, i32 y)
{
    i32 nearest = 100;
    i32 candidate_distance;
    i32 best_distance = 0;
    i32 index = 46;
    do
    {
        SideEffectActorView * actor = game->vtable->get_actor(game, index);
        if (actor != nullptr && actor->location_map == map && actor->vtable->is_available(actor))
        {
            i32 dx = i16(actor->x_q16 >> 16) - x;
            i32 dy = i16(actor->y_q16 >> 16) - y;
            i32 distance = dx * dx + dy * dy;
            candidate_distance = distance;
            if (nearest == 100 || best_distance > distance)
            {
                nearest = index;
                best_distance = candidate_distance;
            }
        }
        index++;
    } while (index <= 69);
    return nearest;
}

EC u16 GetSpriteWorkTargetMap(SpriteWorkTarget const * target, void * context, SpriteWorkMapServices * game) NPC_SIDE_CODE(08033784);
EC u16 GetSpriteWorkTargetMap(SpriteWorkTarget const * target, void * context, SpriteWorkMapServices * game)
{
    i32 index;
    AEntity * actor;
    u32 kind = u32(target->kind_flags) << 28;
    kind >>= 28;
    switch (kind)
    {
    case 0: default: goto not_found;
    case 1: case 2: return 2;
    case 3: return 17;
    case 4: case 5: return 37;
    case 6: index = target->index; index += 54; break;
    case 7: index = target->index; index += 46; break;
    case 8: index = target->index; index += 76; break;
    }
    actor = game->vtable->get_actor(game, index);
    if (actor != 0)
        goto found;
not_found:
    return MAP_NONE;
found:
    return actor->location_map;
}

EC Box GetRecordedActorBounds(AEntity * self) NPC_SIDE_CODE(080329DC);
EC Box GetRecordedActorBounds(AEntity * self)
{
    i32 x = i16(self->x_q16 >> 16);
    i32 y = i16(self->y_q16 >> 16);
    return Box(x, y - 3, 14, 14);
}

EC void RefreshNpcVariantFiveAnimations(VariantFiveScheduledNpc * self) NPC_EXTRA_CODE(08036768);
EC void RefreshNpcVariantFiveAnimations(VariantFiveScheduledNpc * self)
{
    i32 idle_animation;
    u32 walking_animation = 635;
    u32 mode = 2;
    u32 state = u32(self->world->flags) << 30;
    state >>= 30;
    idle_animation = 639;
    if (state == 1)
    {
        walking_animation += 24;
        mode = 1;
    }
    SetNpcWalkingAnimation(self, walking_animation);
    SetNpcIdleAnimation(self, idle_animation);
    self->unk_21 = mode;
}

EC void UpdateNpcNorthernArrival(AActorEntity * self, u32 reason) NPC_EXTRA_CODE(08036BB0);
EC void UpdateNpcNorthernArrival(AActorEntity * self, u32 reason)
{
    i32 previous_x = i16(self->x_q16 >> 16);
    i32 previous_y = self->y_q16 >> 16;
    func_08034EB4(self, reason);
    if (self->location_map == 2 && (previous_x != 244 || previous_y != 171))
    {
        i32 x = i16(self->x_q16 >> 16);
        i32 y = i16(self->y_q16 >> 16);
        if (x == 244 && y == 171)
        {
            NorthernArrivalServices * game = reinterpret_cast<NorthernArrivalServices *>(self->game_object);
            game->vtable->arrive_at_exit(game);
        }
    }
}

EC void RefreshNpcNorthernArrival(AActorEntity * self, i32 reason) NPC_EXTRA_CODE(08036BF8);
EC void RefreshNpcNorthernArrival(AActorEntity * self, i32 reason)
{
    i32 previous_x = i16(self->x_q16 >> 16);
    i32 previous_y = self->y_q16 >> 16;
    func_08034F00(self, reason);
    if (reason > 1 && self->location_map == 2 && (previous_x != 244 || previous_y != 171))
    {
        i32 x = i16(self->x_q16 >> 16);
        i32 y = i16(self->y_q16 >> 16);
        if (x == 244 && y == 171)
        {
            NorthernArrivalServices * game = reinterpret_cast<NorthernArrivalServices *>(self->game_object);
            game->vtable->arrive_at_exit(game);
        }
    }
}
