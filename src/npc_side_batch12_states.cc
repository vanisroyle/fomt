#include "entity_actor.hh"

#define NPC_EXTRA_CODE(address) SECTION(".text.npc_extra." #address)

struct NpcInteractionAction
{
    u32 timer : 16;
    u32 reserved : 16;
    u32 reserved_second : 16;
    u32 steps : 8;
    u32 target : 8;
};

struct NpcInteractionWait
{
    u32 timer : 16;
    u32 phase : 8;
    u32 target : 8;
};

struct NpcInteractionRecord
{
    STRUCT_PAD(0x00, 0x10);
    struct State
    {
        u16 timer;
        u8 phase;
        u8 target;
    } state;
};

struct NpcInteractionActor;
struct NpcInteractionActorVTable
{
    STRUCT_PAD(0x00, 0x60);
    bool (*available)(NpcInteractionActor *);
    STRUCT_PAD(0x64, 0x80);
    void (*interact)(NpcInteractionActor *);
};

struct NpcInteractionActor
{
    STRUCT_PAD(0x00, 0x04);
    u16 map;
    u16 reserved;
    u16 x_fraction;
    i16 x;
    u16 y_fraction;
    i16 y;
    u32 sprite;
    NpcInteractionActorVTable * vtable;
};

struct NpcInteractionServices;
struct NpcInteractionServicesVTable
{
    STRUCT_PAD(0x00, 0x40);
    NpcInteractionActor * (*get_actor)(NpcInteractionServices *, u32);
};

struct NpcInteractionServices
{
    NpcInteractionServicesVTable * vtable;
};

struct NpcInteractionEntity
{
    NpcInteractionServices * services;
    u16 map;
    u16 reserved;
    u16 x_fraction;
    i16 x;
    u16 y_fraction;
    i16 y;
    STRUCT_PAD(0x10, 0x34);
    NpcInteractionRecord * record;
};

EC u32 FindNearestSideEffectActor(NpcInteractionServices *, u32 map, i32 x, i32 y);
EC void PlayNpcInteractionSound(NpcInteractionEntity *, u32) asm("AEntity_PlayAnimationSound");
EC void ReduceNpcInteractionEnergy(NpcInteractionRecord *, u32) asm("func_0809C068");
EC u32 GetNpcInteractionEnergy(NpcInteractionRecord *) asm("func_0809C060");
EC void SetNpcInteractionWalk(NpcInteractionRecord *, NpcInteractionAction const *) asm("func_0809C0AC");
EC void SetNpcInteractionWait(NpcInteractionRecord *, NpcInteractionWait const *) asm("func_0809C0BC");
EC void SetNpcInteractionIdle(NpcInteractionRecord *, NpcInteractionWait const *) asm("func_0809C0A0");
EC u32 RollEntityMovementRandom(u32 maximum);

EC void UpdateNpcInteractionPause(void * policy, NpcInteractionEntity * self) NPC_EXTRA_CODE(0803955C);
EC void UpdateNpcInteractionPause(void * policy, NpcInteractionEntity * input_self)
{
    register NpcInteractionEntity * self asm("r5") = input_self;
    asm("" : "+l"(self));
    register NpcInteractionRecord * record asm("r8") = self->record;
    asm("" : "+h"(record));
    register NpcInteractionRecord::State * state asm("r6") = &record->state;
    asm("" : "+l"(state));
    register NpcInteractionRecord * timer_record asm("r1") = record;
    asm("" : "+l"(timer_record));
    u32 timer = timer_record->state.timer;
    if (timer != 0)
    {
        --timer;
        if (timer != 0)
            goto store_timer;
    }
    if (state->phase != 0)
    {
        NpcInteractionServices * services = self->services;
        u32 map = self->map;
        asm("" : "+r"(map));
        register u32 x_offset asm("r3") = 10;
        asm("" : "+l"(x_offset));
        register i32 pixel_x asm("r2");
        asm("ldrsh %0, [%1, %2]" : "=l"(pixel_x) : "l"(self), "l"(x_offset), "m"(self->x));
        register i32 x asm("r9") = pixel_x;
        asm("" : : "h"(x));
        register u32 pixel_y_offset asm("r1") = 14;
        asm("" : "+l"(pixel_y_offset));
        register i32 pixel_y asm("r0");
        asm("ldrsh %0, [%1, %2]" : "=l"(pixel_y) : "l"(self), "l"(pixel_y_offset), "m"(self->y));
        register i32 y asm("r10") = pixel_y;
        asm("" : "+h"(y));
        register u32 nearest asm("r1") = FindNearestSideEffectActor(services, map, pixel_x, y);
        asm("" : "+l"(nearest));
        if (nearest == 100)
            nearest = state->target;
        NpcInteractionActor * actor = services->vtable->get_actor(services, nearest);
        if (actor != nullptr && actor->map == map && actor->vtable->available(actor))
        {
            register u32 actor_x_offset asm("r2") = 10;
            asm("" : "+l"(actor_x_offset));
            register i32 actor_x asm("r0");
            asm("ldrsh %0, [%1, %2]" : "=l"(actor_x) : "l"(actor), "l"(actor_x_offset), "m"(actor->x));
            register u32 y_offset asm("r3") = 14;
            asm("" : "+l"(y_offset));
            register i32 actor_y asm("r1");
            asm("ldrsh %0, [%1, %2]" : "=l"(actor_y) : "l"(actor), "l"(y_offset), "m"(actor->y));
            register i32 source_x asm("r6") = x;
            asm("" : "+l"(source_x));
            if (__builtin_abs(actor_x - source_x) <= 15)
            {
                register i32 delta_y asm("r3") = actor_y - y;
                asm("" : "+l"(delta_y));
                if (__builtin_abs(delta_y) <= 15)
                    actor->vtable->interact(actor);
            }
        }
        ReduceNpcInteractionEnergy(record, 4);
        NpcInteractionRecord * next_record = self->record;
        u32 target = 100;
        if (GetNpcInteractionEnergy(next_record) != 0)
        {
            register NpcInteractionServices * search_services asm("r0") = self->services;
            asm("" : "+l"(search_services));
            register u32 search_map asm("r1") = self->map;
            asm("" : "+l"(search_map));
            register u32 search_x_offset asm("r3") = 10;
            asm("" : "+l"(search_x_offset));
            register i32 search_x asm("r2");
            asm("ldrsh %0, [%1, %2]" : "=l"(search_x) : "l"(self), "l"(search_x_offset), "m"(self->x));
            register u32 search_y_offset asm("r6") = 14;
            asm("" : "+l"(search_y_offset));
            register i32 search_y asm("r3");
            asm("ldrsh %0, [%1, %2]" : "=l"(search_y) : "l"(self), "l"(search_y_offset), "m"(self->y));
            target = FindNearestSideEffectActor(search_services, search_map, search_x, search_y);
        }
        else
            PlayNpcInteractionSound(self, 0xAA);
        if (target != 100)
        {
            if (RollEntityMovementRandom(100) <= 69)
            {
                NpcInteractionAction walk;
                walk.timer = 0;
                walk.steps = RollEntityMovementRandom(8) + 3;
                walk.target = target;
                SetNpcInteractionWalk(next_record, &walk);
            }
            else
            {
                NpcInteractionWait wait;
                wait.timer = RollEntityMovementRandom(60) + 120;
                wait.phase = 0;
                wait.target = target;
                SetNpcInteractionWait(next_record, &wait);
            }
        }
        else
        {
            NpcInteractionWait idle;
            idle.timer = 180;
            SetNpcInteractionIdle(next_record, &idle);
        }
        return;
    }
    timer = 36;
    state->phase = 1;
    PlayNpcInteractionSound(self, 0xAB);
store_timer:
    state->timer = timer;
}
