#include "entity_actor.hh"
#include "g1_npc.hh"
#include "harvest_sprite.hh"

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

EC void SetNpcWalkingAnimation(AActorEntity * self, u32 animation);
EC void SetNpcIdleAnimation(AActorEntity * self, u32 animation);
EC void func_08034EB4(AActorEntity * self, u32 reason);
EC void func_08034F00(AActorEntity * self, i32 reason);

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
