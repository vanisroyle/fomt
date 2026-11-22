#include "entity_actor.hh"

#include <new>

#define NPC_EXTRA_CODE(address) SECTION(".text.npc_extra." #address)

struct ActivityNpcBox { i16 x1, y1, x2, y2; };

inline void * operator new(size_t, ActivityNpcBox * destination) { return destination; }

struct ActivityNpcCollisionProbe
{
    i32 down, up, left, right;
    u8 down_clear, up_clear, left_clear, right_clear;
    UnkMap map;
    ActivityNpcBox bounds;
    i32 step, horizontal_lookahead, vertical_lookahead;

    ActivityNpcCollisionProbe * Initialize(UnkMap const & input_map, ActivityNpcBox const & input_bounds, i32 distance)
    {
        register ActivityNpcCollisionProbe * destination asm("r4") = this;
        asm("" : : "l"(destination));
        register i32 distance_low asm("r1") = distance;
        asm("" : "+l"(distance_low));
        register i32 saved_distance asm("r8") = distance_low;
        asm("" : "+h"(saved_distance));
        register i32 zero_low asm("r2") = 0;
        asm("" : "+l"(zero_low));
        register i32 zero asm("r9") = zero_low;
        asm("" : "+h"(zero));
        down = distance + 1;
        asm("" : "+l"(destination));
        destination->up = -distance - 1;
        destination->left = -distance - 1;
        destination->right = distance + 1;
        map = input_map;
        bounds = input_bounds;
        register i32 stored_distance asm("r3") = saved_distance;
        asm("" : "+l"(stored_distance));
        destination->step = stored_distance;
        register i32 stored_zero asm("r6") = zero;
        asm("" : "+l"(stored_zero));
        destination->horizontal_lookahead = stored_zero;
        destination->vertical_lookahead = stored_zero;
        return destination;
    }
};

struct ActivityNpcCalendar
{
    u32 weather;
    STRUCT_PAD(0x04, 0x0A);
    u16 packed_time;
};

struct ActivityNpcEntity;
struct ActivityNpcServices;
struct ActivityNpcServiceMethods
{
    STRUCT_PAD(0x00, 0x34);
    UnkMap (*get_map)(ActivityNpcServices *, u32);
    STRUCT_PAD(0x38, 0x40);
    ActivityNpcEntity * (*get_actor)(ActivityNpcServices *, u32);
    STRUCT_PAD(0x44, 0x144);
    ActivityNpcCalendar * (*get_calendar)(ActivityNpcServices *);
};
struct ActivityNpcServices { ActivityNpcServiceMethods * methods; };

struct ActivityNpcEntityMethods
{
    STRUCT_PAD(0x00, 0x0C);
    ActivityNpcBox (*get_box)(ActivityNpcEntity *);
};

struct ActivityNpcPolicy;
struct ActivityNpcPolicyMethods
{
    STRUCT_PAD(0x00, 0x08);
    void (*update)(ActivityNpcPolicy *, ActivityNpcEntity *, ActivityNpcCollisionProbe *);
    u32 (*choose_movement)(ActivityNpcPolicy *, ActivityNpcEntity *);
};
struct ActivityNpcPolicy { ActivityNpcPolicyMethods * methods; };

struct ActivityNpcRecord
{
    STRUCT_PAD(0x00, 0x0C);
    u32 activity;
};

struct ActivityNpcSprite;
struct ActivityNpcSpriteMethods
{
    STRUCT_PAD(0x00, 0x0C);
    void (*update)(ActivityNpcSprite *);
};
struct ActivityNpcSprite
{
    void * owner;
    ActivityNpcSpriteMethods * methods;
};

struct ActivityNpcEntity
{
    ActivityNpcServices * services;
    u16 map;
    u16 flags;
    u16 x_fraction;
    i16 x;
    u16 y_fraction;
    i16 y;
    ActivityNpcSprite * sprite;
    ActivityNpcEntityMethods * methods;
    i32 x_speed, y_speed;
    u8 facing, animation_flags;
    u16 animation;
    u16 frame_timer, frame_duration;
    STRUCT_PAD(0x28, 0x30);
    u8 frame_latch;
    STRUCT_PAD(0x31, 0x34);
    ActivityNpcRecord * record;
    ActivityNpcPolicy * policies[5];
    u32 movement;
    u32 previous_facing;
};

inline ActivityNpcCalendar * GetActivityNpcCalendar(ActivityNpcServices * input_services)
{
    register ActivityNpcServices * services asm("r10") = input_services;
    asm("" : "+h"(services));
    register ActivityNpcServices * receiver asm("r1") = services;
    asm("" : "+l"(receiver));
    register u8 const * method_address asm("r0") = reinterpret_cast<u8 const *>(receiver->methods);
    asm("" : "+l"(method_address));
    register u32 offset asm("r2") = offsetof(ActivityNpcServiceMethods, get_calendar);
    asm("" : "+l"(offset));
    method_address += offset;
    asm("" : "+l"(method_address));
    typedef ActivityNpcCalendar * (*Getter)(ActivityNpcServices *);
    register Getter getter asm("r1") = *reinterpret_cast<Getter const *>(method_address);
    asm("" : "+l"(getter));
    return getter(services);
}

inline ActivityNpcEntity * FindActivityNpcActor(ActivityNpcEntity * self, u32 index)
{
    register ActivityNpcServices * services asm("r0") = self->services;
    asm("" : "+l"(services));
    register ActivityNpcServiceMethods * methods asm("r1") = services->methods;
    asm("" : "+l"(methods));
    typedef ActivityNpcEntity * (*Getter)(ActivityNpcServices *, u32);
    register Getter getter asm("r2") = methods->get_actor;
    asm("" : "+l"(getter));
    return getter(services, index);
}

inline ActivityNpcPolicy * GetActivityNpcPolicy(ActivityNpcEntity * self)
{
    register ActivityNpcRecord * record asm("r0") = self->record;
    asm("" : "+l"(record));
    register u32 offset asm("r0") = record->activity;
    asm("" : "+l"(offset));
    offset *= sizeof(ActivityNpcPolicy *);
    asm("" : "+l"(offset));
    register u8 const * policy_address asm("r0") = reinterpret_cast<u8 const *>(offset + reinterpret_cast<u32>(self));
    asm("" : "+l"(policy_address));
    return *reinterpret_cast<ActivityNpcPolicy * const *>(policy_address + offsetof(ActivityNpcEntity, policies));
}

struct ActivityNpcUpdateContext
{
    u32 flags;
    u8 allow_spawn;
};

struct ActivityNpcAction
{
    u32 timer_flags;
    u32 target_flags;
};

struct ActivityNpcPoint
{
    i16 x, y;
    ActivityNpcPoint() {}
    ActivityNpcPoint(i16 x, i16 y) : x(x), y(y) {}
};

struct ActivityNpcUpdateScratch
{
    ActivityNpcPoint entrance, position;
    ActivityNpcAction action;
    ActivityNpcBox bounds, player_bounds, companion_bounds;
    u32 target;
};

enum { ACTIVITY_NPC_LOCATION_STRIDE = (sizeof(Location) + sizeof(u32) - 1) & ~(sizeof(u32) - 1) };
union ActivityNpcLocationScratch
{
    u8 teleport[ACTIVITY_NPC_LOCATION_STRIDE + sizeof(ActorLocation)];
    UnkMap map;
};

EC u32 RollEntityMovementRandom(u32);
EC u32 FindNearestSideEffectActor(ActivityNpcServices *, u32, i32, i32);
EC void SetActivityNpcLocation(ActivityNpcEntity *, ActorLocation const &) asm("SetLocation__12AActorEntityRC13ActorLocation");
EC void SetActivityNpcWalk(ActivityNpcRecord *, ActivityNpcAction const &) asm("func_0809C0AC");
EC void PlayActivityNpcSound(ActivityNpcEntity *, u32) asm("AEntity_PlayAnimationSound");
EC void LimitActivityNpcProbe(ActivityNpcCollisionProbe *, ActivityNpcBox const &, u32) asm("ConstrainEntityMovementAgainstRectangle");
EC u32 GetActiveNpcMovementSpeed(ActivityNpcEntity *, u32);
EC void SetActivityNpcSpeed(ActivityNpcEntity *, i32) asm("AActorEntity_SetFacingSpeed");
EC u32 GetNpcVariantFiveAnimation(ActivityNpcEntity *, u32);
EC void SetActivityNpcAnimation(ActivityNpcEntity *, u32) asm("SetAnim__12AActorEntityUi");
EC u8 IsWithinSideEntityExit(i32, i32);
EC void ApplyActivityNpcMovement(ActivityNpcEntity *, ActivityNpcCollisionProbe *) asm("AActorEntity_ApplyBoundedMovement");

EC void UpdateActivityNpcEntity(ActivityNpcEntity *, ActivityNpcUpdateContext *) NPC_EXTRA_CODE(08039A60);
EC void UpdateActivityNpcEntity(ActivityNpcEntity * input_self, ActivityNpcUpdateContext * context)
{
    ActivityNpcEntity * self = input_self;
    asm("" : "+&l"(self) : "r"(context) : "r0", "r2", "r3", "r4", "r5", "r6");
    register ActivityNpcServices * services asm("r10") = self->services;
    asm("" : "+h"(services));
    ActivityNpcLocationScratch location;
    ActivityNpcCollisionProbe probe;
    ActivityNpcUpdateScratch scratch;
    if (context->allow_spawn != 0 && self->map != 2)
    {
        ActivityNpcCalendar * calendar = GetActivityNpcCalendar(services);
        if ((calendar->packed_time & 0x7FF) == 20 && calendar->weather == 0)
        {
            if (RollEntityMovementRandom(2) != 0)
                new (&scratch.entrance) ActivityNpcPoint(264, 720);
            else
                new (&scratch.entrance) ActivityNpcPoint(340, -16);
            scratch.position = scratch.entrance;
            register ActivityNpcPoint * position asm("r4") = &scratch.position;
            asm("" : "+l"(position));
            register u32 x_offset asm("r3") = 0;
            asm("" : : "l"(x_offset));
            register i32 nearby_x asm("r2");
            asm("ldrsh %0, [%1, %2]" : "=l"(nearby_x) : "l"(position), "l"(x_offset), "m"(*position));
            register u32 y_offset asm("r6") = offsetof(ActivityNpcPoint, y);
            register i32 nearby_y asm("r3");
            asm("ldrsh %0, [%1, %2]" : "=l"(nearby_y) : "l"(position), "l"(y_offset), "m"(*position));
            u32 nearest = FindNearestSideEffectActor(services, 2, nearby_x, nearby_y);
            scratch.target = nearest;
            asm("" : "+m"(scratch.target));
            if (nearest != 100 && RollEntityMovementRandom(100) <= 14)
            {
                register u32 spawn_x_offset asm("r0") = offsetof(ActivityNpcPoint, x);
                register i32 spawn_x asm("r6");
                asm("ldrsh %0, [%1, %2]" : "=l"(spawn_x) : "l"(position), "l"(spawn_x_offset), "m"(*position));
                Location * destination = new (location.teleport) Location(2, spawn_x, position->y);
                ActorLocation * actor_location = new (location.teleport + ACTIVITY_NPC_LOCATION_STRIDE) ActorLocation(*destination, 1);
                SetActivityNpcLocation(self, *actor_location);
                asm("" : "=m"(scratch.action));
                register u32 timer_mask asm("r1") = 0xFFFF0000;
                asm("" : "+l"(timer_mask));
                scratch.action.timer_flags &= timer_mask;
                register u32 steps asm("r0") = (RollEntityMovementRandom(8) + 3) << 24;
                asm("" : "+l"(steps));
                steps >>= 8;
                asm("" : "+l"(steps));
                register u32 steps_mask asm("r3") = 0xFF00FFFF;
                asm("" : "+l"(steps_mask));
                register ActivityNpcAction * action asm("r1") = &scratch.action;
                asm("" : "+l"(action));
                register u32 flags asm("r2") = action->target_flags & steps_mask;
                asm("" : "+l"(flags));
                flags |= steps;
                asm("" : "+l"(flags));
                register u32 target asm("r6") = scratch.target;
                asm("" : "+l"(target));
                register u32 target_bits asm("r3") = target << 24;
                asm("" : "+l"(target_bits));
                register u32 target_mask asm("r0") = 0x00FFFFFF;
                asm("" : "+l"(target_mask));
                flags = (flags & target_mask) | target_bits;
                action->target_flags = flags;
                SetActivityNpcWalk(self->record, *action);
                PlayActivityNpcSound(self, 171);
            }
        }
    }
    register u32 map asm("r5") = self->map;
    asm("" : "+l"(map));
    if (map != MAP_NONE)
    {
        new (&location.map) UnkMap(services->methods->get_map(services, map));
        register ActivityNpcEntityMethods * bounds_methods asm("r1") = self->methods;
        asm("" : "+l"(bounds_methods));
        new (&scratch.bounds) ActivityNpcBox(bounds_methods->get_box(self));
        ActivityNpcCollisionProbe * initialized_probe = probe.Initialize(location.map, scratch.bounds, 32);
        ActivityNpcEntity * actor = FindActivityNpcActor(self, 0);
        register ActivityNpcCollisionProbe * active_probe asm("r8") = initialized_probe;
        asm("" : "+h"(active_probe));
        if (actor != nullptr && actor->map == map)
        {
            register ActivityNpcEntityMethods * methods asm("r1") = actor->methods;
            asm("" : "+l"(methods));
            register ActivityNpcBox * destination asm("r4") = &scratch.player_bounds;
            asm("" : : "l"(destination));
            new (destination) ActivityNpcBox(methods->get_box(actor));
            LimitActivityNpcProbe(active_probe, *destination, 0);
        }
        actor = FindActivityNpcActor(self, 74);
        if (actor != nullptr && actor->map == map)
        {
            register ActivityNpcEntityMethods * methods asm("r1") = actor->methods;
            asm("" : "+l"(methods));
            register ActivityNpcBox * destination asm("r4") = &scratch.companion_bounds;
            asm("" : : "l"(destination));
            new (destination) ActivityNpcBox(methods->get_box(actor));
            LimitActivityNpcProbe(active_probe, *destination, 0);
        }
        register ActivityNpcPolicy * policy asm("r0") = GetActivityNpcPolicy(self);
        asm("" : "+l"(policy));
        register ActivityNpcPolicyMethods * methods asm("r1") = policy->methods;
        asm("" : "+l"(methods));
        typedef void (*Update)(ActivityNpcPolicy *, ActivityNpcEntity *, ActivityNpcCollisionProbe *);
        register Update update asm("r3") = methods->update;
        asm("" : "+l"(update));
        update(policy, self, active_probe);
        policy = GetActivityNpcPolicy(self);
        asm("" : "+l"(policy));
        methods = policy->methods;
        asm("" : "+l"(methods));
        typedef u32 (*Choose)(ActivityNpcPolicy *, ActivityNpcEntity *);
        register Choose choose asm("r2") = methods->choose_movement;
        asm("" : "+l"(choose));
        u32 movement = choose(policy, self);
        u32 facing = self->facing;
        if (movement != self->movement || facing != self->previous_facing)
            SetActivityNpcSpeed(self, GetActiveNpcMovementSpeed(self, movement));
        if (movement != self->movement)
        {
            u32 animation = GetNpcVariantFiveAnimation(self, movement);
            if (self->animation != animation)
                SetActivityNpcAnimation(self, animation);
        }
        self->movement = movement;
        self->previous_facing = facing;
        ActivityNpcCollisionProbe * limits = active_probe;
        register u32 activity asm("r0") = self->record->activity;
        asm("" : "+l"(activity));
        register u32 current_facing asm("r1") = self->facing;
        asm("" : "+l"(current_facing));
        if (activity - 1 <= 1 && current_facing <= 1 && IsWithinSideEntityExit(self->x, self->y) != 0)
            limits = nullptr;
        ApplyActivityNpcMovement(self, limits);
    }
    self->frame_latch = 0;
    if (self->frame_timer != 0)
        --self->frame_timer;
    else
        self->frame_timer = self->frame_duration;
    if (self->sprite != nullptr)
        self->sprite->methods->update(self->sprite);
}
