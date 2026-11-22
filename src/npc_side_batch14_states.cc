#include "entity_actor.hh"

#define NPC_EXTRA_CODE(address) SECTION(".text.npc_extra." #address)

struct NpcFollowingAction
{
    u32 timer_flags;
    u32 target_flags;
};

struct NpcFollowingWait
{
    u32 flags;
};

struct NpcFollowingRecord
{
    STRUCT_PAD(0x00, 0x10);
    struct State
    {
        u16 timer;
        i16 target_x;
        i16 target_y;
        u8 steps;
        u8 target;
    } state;
};

struct NpcFollowingActor;
struct NpcFollowingActorMethods
{
    STRUCT_PAD(0x00, 0x60);
    bool (*can_interact)(NpcFollowingActor *);
};
struct NpcFollowingActor
{
    STRUCT_PAD(0x00, 0x04);
    u16 map;
    u16 reserved;
    u16 x_fraction;
    i16 x;
    u16 y_fraction;
    i16 y;
    STRUCT_PAD(0x10, 0x14);
    NpcFollowingActorMethods * methods;
};

struct NpcFollowingServices;
struct NpcFollowingServicesMethods
{
    STRUCT_PAD(0x00, 0x40);
    NpcFollowingActor * (*get_actor)(NpcFollowingServices *, u32);
};
struct NpcFollowingServices { NpcFollowingServicesMethods * methods; };
struct NpcFollowingEntity
{
    NpcFollowingServices * services;
    u16 map;
    u16 reserved;
    u16 x_fraction;
    i16 x;
    u16 y_fraction;
    i16 y;
    STRUCT_PAD(0x10, 0x20);
    u8 facing;
    STRUCT_PAD(0x21, 0x34);
    NpcFollowingRecord * record;
};

struct EntityCollisionProbe;

struct NpcFollowingScratch
{
    NpcFollowingWait arrived;
    NpcFollowingAction walk;
    NpcFollowingWait wait;
    NpcFollowingWait idle;
    EntityCollisionProbe * volatile probe;
    NpcFollowingRecord * volatile record;
    volatile u32 steps;
    volatile i32 saved_x;
};

EC u32 FindNearestSideEffectActor(NpcFollowingServices *, u32, i32, i32);
EC u32 ChooseEntityFacingTowardPoint(i32, i32, i32, i32, EntityCollisionProbe *, u32);
EC bool IsWithinSideEntityExit(i32, i32);
EC void SetNpcFollowingFacing(NpcFollowingEntity *, u32) asm("SetAnimFacing__12AActorEntityUi");
EC void PlayNpcFollowingSound(NpcFollowingEntity *, u32) asm("AEntity_PlayAnimationSound");
EC u32 GetNpcFollowingEnergy(NpcFollowingRecord *) asm("func_0809C060");
EC void SetNpcFollowingWalk(NpcFollowingRecord *, NpcFollowingAction const *) asm("func_0809C0AC");
EC void SetNpcFollowingWait(NpcFollowingRecord *, NpcFollowingWait const *) asm("func_0809C0BC");
EC void SetNpcFollowingIdle(NpcFollowingRecord *, NpcFollowingWait const *) asm("func_0809C0A0");
EC u32 RollEntityMovementRandom(u32 maximum);

inline i32 NpcFollowingAbs(i32 value) { return value < 0 ? -value : value; }
inline i32 ReadNpcFollowingTargetCoordinate(NpcFollowingRecord::State * state, u32 offset)
{
    register NpcFollowingRecord::State * position asm("r1") = state;
    asm("" : "+l"(position));
    return offset == offsetof(NpcFollowingRecord::State, target_x) ? position->target_x : position->target_y;
}

inline i32 NpcFollowingVerticalDistance(i32 value, i32 y)
{
    register i32 coordinate asm("r3") = value;
    asm("" : "+l"(coordinate));
    register i32 difference asm("r0") = coordinate - y;
    asm("" : "+l"(difference));
    return NpcFollowingAbs(difference);
}

EC void UpdateNpcInteractionFollowing(void *, NpcFollowingEntity *, EntityCollisionProbe *) NPC_EXTRA_CODE(08039310);
EC void UpdateNpcInteractionFollowing(void *, NpcFollowingEntity * input_self, EntityCollisionProbe * probe)
{
    register NpcFollowingEntity * self asm("r5") = input_self;
    asm("" : "+l"(self));
    NpcFollowingScratch scratch;
    asm("" : "=m"(scratch.arrived), "=m"(scratch.walk), "=m"(scratch.wait), "=m"(scratch.idle));
    scratch.probe = probe;
    NpcFollowingRecord * record = self->record;
    scratch.record = record;
    NpcFollowingRecord::State * initial_state = &record->state;
    asm("" : "+r"(initial_state));
    register NpcFollowingRecord::State * state asm("r8") = initial_state;
    asm("" : : "h"(state));
    register u32 timer asm("r6");
    register NpcFollowingActor * actor asm("r4");
    {
        register NpcFollowingServices * services asm("r3") = self->services;
        asm("" : : "l"(services));
        register u32 initial_steps asm("r1") = initial_state->steps;
        asm("" : "+l"(initial_steps));
        scratch.steps = initial_steps;
        register NpcFollowingRecord * timer_record asm("r2") = scratch.record;
        asm("" : "+l"(timer_record));
        timer = timer_record->state.timer;
        asm("" : "+l"(timer));
        if (timer != 0)
        {
            --timer;
            if (timer == 0 && initial_steps != 0)
                scratch.steps = initial_steps - 1;
        }
        register NpcFollowingServicesMethods * methods asm("r0") = services->methods;
        asm("" : "+l"(methods));
        register NpcFollowingRecord::State * read_state asm("r2") = state;
        asm("" : "+l"(read_state));
        actor = methods->get_actor(services, read_state->target);
    }
    asm("" : "+l"(actor));
    if (actor == nullptr)
        goto find_target;
    {
        register u32 actor_map asm("r0") = actor->map;
        asm("" : "+l"(actor_map));
        register u32 map asm("r3") = self->map;
        asm("" : "+l"(map));
        if (actor_map != map || !actor->methods->can_interact(actor))
            goto find_target;
    }
    {
        register i32 x asm("r2") = self->x;
        asm("" : "+l"(x));
        i32 y = self->y;
        asm("" : "+r"(y));
        register i32 actor_x asm("r3") = actor->x;
        asm("" : "+l"(actor_x));
        register i32 target_x asm("r10") = actor_x;
        asm("" : : "h"(target_x));
        register i32 actor_y asm("r1") = actor->y;
        asm("" : "+l"(actor_y));
        register i32 target_y asm("r9") = actor_y;
        asm("" : : "h"(target_y));
        if (scratch.steps != 0 && (NpcFollowingAbs(target_x - x) > 15 || NpcFollowingVerticalDistance(target_y, y) > 15))
        {
            if (timer == 0 || (NpcFollowingAbs(ReadNpcFollowingTargetCoordinate(state, offsetof(NpcFollowingRecord::State, target_x)) - x) <= 15 && NpcFollowingAbs(ReadNpcFollowingTargetCoordinate(state, offsetof(NpcFollowingRecord::State, target_y)) - y) <= 15))
            {
                register i32 exit_x asm("r0") = x;
                register i32 exit_y asm("r1") = y;
                asm("" : "+l"(exit_x), "+l"(exit_y));
                scratch.saved_x = x;
                bool at_exit = IsWithinSideEntityExit(exit_x, exit_y);
                x = scratch.saved_x;
                asm("" : "+l"(x));
                if (at_exit)
                {
                    u32 facing = y >= target_y;
                    if (self->facing != facing)
                        SetNpcFollowingFacing(self, facing);
                }
                else
                {
                    register u8 * facing_ptr asm("r4") = &self->facing;
                    asm("" : "+l"(facing_ptr));
                    register u32 current_facing asm("r0") = *facing_ptr;
                    asm("" : "+l"(current_facing));
                    register EntityCollisionProbe * facing_probe asm("r6") = scratch.probe;
                    asm("" : "+l"(facing_probe) : "l"(current_facing));
                    u32 facing = ChooseEntityFacingTowardPoint(x, y, target_x, target_y, facing_probe, current_facing);
                    if (*facing_ptr != facing)
                        SetNpcFollowingFacing(self, facing);
                }
                timer = RollEntityMovementRandom(30) + 1;
                register i32 saved_x asm("r1") = target_x;
                asm("" : "+l"(saved_x));
                register NpcFollowingRecord::State * write_state asm("r0") = state;
                asm("" : "+l"(write_state));
                write_state->target_x = saved_x;
                register i32 saved_y asm("r2") = target_y;
                asm("" : "+l"(saved_y));
                write_state->target_y = saved_y;
            }
            register u8 const volatile * steps_address asm("r3") = reinterpret_cast<u8 const volatile *>(&scratch.steps);
            asm("" : "+l"(steps_address));
            register u32 steps asm("r0") = *steps_address;
            asm("" : "+l"(steps));
            register NpcFollowingRecord::State * store_state asm("r3") = state;
            asm("" : "+l"(store_state));
            store_state->steps = steps;
            store_state->timer = timer;
            return;
        }
        register u32 mask asm("r1") = 0xFFFF0000;
        asm("" : "+l"(mask));
        register u32 flags asm("r0") = scratch.arrived.flags & mask;
        asm("" : "+l"(flags));
        mask = 36;
        asm("" : "+l"(mask));
        flags |= mask;
        mask = 0xFF00FFFF;
        asm("" : "+l"(mask));
        flags &= mask;
        mask = 1 << 16;
        asm("" : "+l"(mask));
        flags |= mask;
        scratch.arrived.flags = flags;
        register NpcFollowingRecord::State * arrived_state asm("r6") = state;
        asm("" : "+l"(arrived_state));
        register u32 arrived_target asm("r1") = arrived_state->target;
        arrived_target <<= 24;
        asm("" : "+l"(arrived_target));
        register u32 target_mask asm("r2") = 0x00FFFFFF;
        asm("" : "+l"(target_mask));
        flags &= target_mask;
        flags |= arrived_target;
        scratch.arrived.flags = flags;
        SetNpcFollowingWait(scratch.record, &scratch.arrived);
        PlayNpcFollowingSound(self, 0xAB);
        return;
    }
find_target:
    register NpcFollowingRecord * next_record asm("r4") = self->record;
    register u32 target asm("r6") = 100;
    if (GetNpcFollowingEnergy(next_record) != 0)
    {
        register NpcFollowingServices * search_services asm("r0") = self->services;
        asm("" : "+l"(search_services));
        register u32 map asm("r1") = self->map;
        asm("" : "+l"(map));
        register i32 x asm("r2") = self->x;
        asm("" : "+l"(next_record), "+l"(target));
        asm("" : "+l"(x));
        i32 y = self->y;
        asm("" : "+r"(y));
        target = FindNearestSideEffectActor(search_services, map, x, y);
    }
    else
        PlayNpcFollowingSound(self, 0xAA);
    if (target != 100)
    {
        if (RollEntityMovementRandom(100) <= 69)
        {
            register u32 timer_mask asm("r1") = 0xFFFF0000;
            asm("" : "+l"(timer_mask));
            scratch.walk.timer_flags &= timer_mask;
            register u32 step_bits asm("r0") = RollEntityMovementRandom(8) + 3;
            step_bits = (step_bits << 24) >> 8;
            asm("" : "+l"(step_bits));
            register u32 steps_mask asm("r3") = 0xFF00FFFF;
            asm("" : "+l"(steps_mask));
            register NpcFollowingAction * action asm("r1") = &scratch.walk;
            asm("" : "+l"(action));
            register u32 flags asm("r2") = action->target_flags;
            flags &= steps_mask;
            flags |= step_bits;
            register u32 target_bits asm("r3") = target << 24;
            asm("" : "+l"(target_bits));
            register u32 target_mask asm("r0") = 0x00FFFFFF;
            asm("" : "+l"(target_mask));
            flags &= target_mask;
            flags |= target_bits;
            action->target_flags = flags;
            SetNpcFollowingWalk(next_record, action);
        }
        else
        {
            register u32 wait_timer asm("r0") = u16(RollEntityMovementRandom(60) + 120);
            asm("" : "+l"(wait_timer));
            register u32 timer_mask asm("r2") = 0xFFFF0000;
            asm("" : "+l"(timer_mask));
            register u32 flags asm("r1") = scratch.wait.flags;
            flags &= timer_mask;
            flags |= wait_timer;
            register u32 mask asm("r0") = 0xFF00FFFF;
            asm("" : "+l"(mask));
            flags &= mask;
            register u32 target_bits asm("r2") = target << 24;
            asm("" : "+l"(target_bits));
            mask = 0x00FFFFFF;
            asm("" : "+l"(mask));
            flags &= mask;
            flags |= target_bits;
            scratch.wait.flags = flags;
            SetNpcFollowingWait(next_record, &scratch.wait);
        }
    }
    else
    {
        register u32 mask asm("r1") = 0xFFFF0000;
        asm("" : "+l"(mask));
        register u32 flags asm("r0") = scratch.idle.flags;
        flags &= mask;
        mask = 180;
        asm("" : "+l"(mask));
        flags |= mask;
        scratch.idle.flags = flags;
        SetNpcFollowingIdle(next_record, &scratch.idle);
    }
}
