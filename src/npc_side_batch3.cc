#include "entity_actor.hh"

#define NPC_EXTRA_CODE(address) SECTION(".text.npc_extra." #address)

EC void SetMovingNpcFacingVelocity(AActorEntity * self, i32 speed) NPC_EXTRA_CODE(08037098);
EC void SetMovingNpcFacingVelocity(AActorEntity * self, i32 speed)
{
    i32 x_speed = 0;
    i32 y_speed = 0;
    u8 facing = self->facing;
    switch (facing)
    {
    case 0: y_speed = speed; break;
    case 1: y_speed = -speed; break;
    case 2: x_speed = -speed; break;
    case 3: x_speed = speed; break;
    }
    self->x_speed_q16 = x_speed;
    self->y_speed_q16 = y_speed;
}

EC UnknownEntityThing * func_080324BC(UnknownEntityThing * self, AActorEntity * actor, u32 layer, u32 animation, u32 priority, u32 palette, u32 flags, bool visible);

EC UnknownEntityThing * CreateScheduledNpcSprite01(AActorEntity * self) NPC_EXTRA_CODE(08035B38);
EC UnknownEntityThing * CreateScheduledNpcSprite01(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite02(AActorEntity * self) NPC_EXTRA_CODE(08035B9C);
EC UnknownEntityThing * CreateScheduledNpcSprite02(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite03(AActorEntity * self) NPC_EXTRA_CODE(08035D40);
EC UnknownEntityThing * CreateScheduledNpcSprite03(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 3, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite04(AActorEntity * self) NPC_EXTRA_CODE(08035DAC);
EC UnknownEntityThing * CreateScheduledNpcSprite04(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 1, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite05(AActorEntity * self) NPC_EXTRA_CODE(08035E14);
EC UnknownEntityThing * CreateScheduledNpcSprite05(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite06(AActorEntity * self) NPC_EXTRA_CODE(08035E7C);
EC UnknownEntityThing * CreateScheduledNpcSprite06(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 2, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite07(AActorEntity * self) NPC_EXTRA_CODE(08035EE4);
EC UnknownEntityThing * CreateScheduledNpcSprite07(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite08(AActorEntity * self) NPC_EXTRA_CODE(08035F48);
EC UnknownEntityThing * CreateScheduledNpcSprite08(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite09(AActorEntity * self) NPC_EXTRA_CODE(08035FB0);
EC UnknownEntityThing * CreateScheduledNpcSprite09(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 2, 0, false);
}
