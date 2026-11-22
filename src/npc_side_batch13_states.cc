#include "entity_actor.hh"

#define NPC_EXTRA_CODE(address) SECTION(".text.npc_extra." #address)

struct NpcAvoidanceAction
{
    u32 timer : 16;
    u32 reserved : 16;
    u32 reserved_second : 16;
    u32 steps : 8;
    u32 target : 8;
};

struct NpcAvoidanceWait
{
    u32 timer : 16;
    u32 phase : 8;
    u32 target : 8;
};

struct NpcAvoidanceRecord
{
    STRUCT_PAD(0x00, 0x10);
    struct State
    {
        u32 timer : 16;
        u32 steps : 7;
        u32 alternate_target : 1;
        u32 facing_timer : 8;
    } state;
};

struct NpcAvoidanceActor
{
    STRUCT_PAD(0x00, 0x04);
    u16 map;
    u16 reserved;
    u16 x_fraction;
    i16 x;
    u16 y_fraction;
    i16 y;
};

struct NpcAvoidanceServices;
struct NpcAvoidanceServicesVTable
{
    STRUCT_PAD(0x00, 0x40);
    NpcAvoidanceActor * (*get_actor)(NpcAvoidanceServices *, u32);
};
struct NpcAvoidanceServices { NpcAvoidanceServicesVTable * vtable; };

struct NpcAvoidanceEntity
{
    NpcAvoidanceServices * services;
    u16 map;
    u16 reserved;
    u16 x_fraction;
    i16 x;
    u16 y_fraction;
    i16 y;
    STRUCT_PAD(0x10, 0x20);
    u8 facing;
    STRUCT_PAD(0x21, 0x34);
    NpcAvoidanceRecord * record;
};

EC u32 FindNearestSideEffectActor(NpcAvoidanceServices *, u32, i32, i32);
EC u32 ChooseEntityFacingAwayFromPoint(i32, i32, i32, i32, i32, u32);
EC void SetNpcAvoidanceFacing(NpcAvoidanceEntity *, u32) asm("SetAnimFacing__12AActorEntityUi");
EC void ResetNpcAvoidanceMovement(NpcAvoidanceEntity *) asm("func_080323C8");
EC void PlayNpcAvoidanceSound(NpcAvoidanceEntity *, u32) asm("AEntity_PlayAnimationSound");
EC u32 GetNpcAvoidanceEnergy(NpcAvoidanceRecord *) asm("func_0809C060");
EC void SetNpcAvoidanceWalk(NpcAvoidanceRecord *, NpcAvoidanceAction const *) asm("func_0809C0AC");
EC void SetNpcAvoidanceWait(NpcAvoidanceRecord *, NpcAvoidanceWait const *) asm("func_0809C0BC");
EC void SetNpcAvoidanceIdle(NpcAvoidanceRecord *, NpcAvoidanceWait const *) asm("func_0809C0A0");
EC u32 RollEntityMovementRandom(u32 maximum);

EC void UpdateNpcInteractionAvoidance(void *, NpcAvoidanceEntity *, i32) NPC_EXTRA_CODE(08039708);
EC void UpdateNpcInteractionAvoidance(void *, NpcAvoidanceEntity * input_self, i32 input_distance)
{
    register NpcAvoidanceEntity * self asm("r6") = input_self;
    asm("" : "+l"(self));
    register i32 distance asm("r10") = input_distance;
    asm("" : "+h"(distance));
    NpcAvoidanceRecord * record = self->record;
    register NpcAvoidanceRecord::State * state asm("r9") = &record->state;
    asm volatile("" : : "h"(state));
    register NpcAvoidanceServices * services asm("r3") = self->services;
    asm volatile("" : : "l"(services));
    register u32 initial_timer asm("r2") = record->state.timer;
    asm volatile("" : : "l"(initial_timer));
    register u32 timer asm("r8") = initial_timer;
    asm volatile("" : : "h"(timer));
    if (initial_timer != 0)
    {
        register i32 decrement asm("r4") = -1;
        asm("" : "+l"(decrement));
        timer += decrement;
    }
    register u32 word asm("r1") = *reinterpret_cast<u32 const *>(&record->state);
    asm("" : "+l"(word));
    u32 steps = (word << 9) >> 25;
    if (steps != 0)
        --steps;
    register NpcAvoidanceRecord::State * read_state asm("r5") = state;
    asm volatile("" : : "l"(read_state));
    u32 facing_timer = read_state->facing_timer;
    if (facing_timer != 0)
        --facing_timer;
    if (timer != 0)
    {
        register NpcAvoidanceServicesVTable * methods asm("r2") = services->vtable;
        asm volatile("" : : "l"(methods));
        register i32 alternate_target asm("r0") = i32(word << 8);
        asm("" : "+l"(alternate_target));
        register u32 target_id asm("r1") = 0;
        asm("" : "+l"(target_id));
        if (alternate_target < 0)
            target_id = 43;
        register NpcAvoidanceActor * actor asm("r3") = methods->get_actor(services, target_id);
        asm("" : "+l"(actor));
        if (actor != nullptr && actor->map == self->map)
        {
            if (facing_timer == 0)
            {
                register u32 x_offset asm("r2") = 10;
                asm("" : "+l"(x_offset));
                register i32 x asm("r0");
                asm("ldrsh %0, [%1, %2]" : "=l"(x) : "l"(self), "l"(x_offset), "m"(self->x));
                register u32 y_offset asm("r4") = 14;
                asm("" : "+l"(y_offset));
                register i32 y asm("r1");
                asm("ldrsh %0, [%1, %2]" : "=l"(y) : "l"(self), "l"(y_offset), "m"(self->y));
                register u32 actor_x_offset asm("r5") = 10;
                asm("" : "+l"(actor_x_offset));
                register i32 actor_x asm("r2");
                asm("ldrsh %0, [%1, %2]" : "=l"(actor_x) : "l"(actor), "l"(actor_x_offset), "m"(actor->x));
                y_offset = 14;
                asm("" : "+l"(y_offset));
                register i32 actor_y asm("r3");
                asm("ldrsh %0, [%1, %2]" : "=l"(actor_y) : "l"(actor), "l"(y_offset), "m"(actor->y));
                register u8 * facing_ptr asm("r5") = &self->facing;
                asm volatile("" : : "l"(facing_ptr));
                register u32 current_facing asm("r12") = *facing_ptr;
                asm("" : "+h"(current_facing));
                u32 facing = ChooseEntityFacingAwayFromPoint(x, y, actor_x, actor_y, distance, current_facing);
                if (*facing_ptr != facing)
                    SetNpcAvoidanceFacing(self, facing);
                facing_timer = RollEntityMovementRandom(30) + 1;
            }
            register u32 saved_timer asm("r0") = timer;
            asm volatile("" : : "l"(saved_timer));
            register NpcAvoidanceRecord::State * store_state asm("r5") = state;
            asm("" : "+l"(store_state));
            store_state->timer = saved_timer;
            store_state->steps = steps;
            store_state->facing_timer = facing_timer;
            return;
        }
    }
    ResetNpcAvoidanceMovement(self);
    NpcAvoidanceRecord * next_record = self->record;
    u32 target = 100;
    if (GetNpcAvoidanceEnergy(next_record) != 0)
    {
        register NpcAvoidanceServices * search_services asm("r0") = self->services;
        asm("" : "+l"(search_services));
        register u32 search_map asm("r1") = self->map;
        asm("" : "+l"(search_map));
        register u32 search_x_offset asm("r3") = 10;
        asm("" : "+l"(search_x_offset));
        register i32 search_x asm("r2");
        asm("ldrsh %0, [%1, %2]" : "=l"(search_x) : "l"(self), "l"(search_x_offset), "m"(self->x));
        register u32 search_y_offset asm("r5") = 14;
        asm("" : "+l"(search_y_offset));
        register i32 search_y asm("r3");
        asm("ldrsh %0, [%1, %2]" : "=l"(search_y) : "l"(self), "l"(search_y_offset), "m"(self->y));
        target = FindNearestSideEffectActor(search_services, search_map, search_x, search_y);
    }
    else
        PlayNpcAvoidanceSound(self, 0xAA);
    if (target != 100)
    {
        if (RollEntityMovementRandom(100) <= 69)
        {
            NpcAvoidanceAction walk;
            walk.timer = 0;
            walk.steps = RollEntityMovementRandom(8) + 3;
            walk.target = target;
            SetNpcAvoidanceWalk(next_record, &walk);
        }
        else
        {
            NpcAvoidanceWait wait;
            wait.timer = RollEntityMovementRandom(60) + 120;
            wait.phase = 0;
            wait.target = target;
            SetNpcAvoidanceWait(next_record, &wait);
        }
    }
    else
    {
        NpcAvoidanceWait idle;
        idle.timer = 180;
        SetNpcAvoidanceIdle(next_record, &idle);
    }
}
