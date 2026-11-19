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

struct NpcMovementChoice
{
    u8 weight;
    u8 minimum_seconds;
    u8 random_seconds;
    u8 padding;
};

EC NpcMovementChoice const gUnk_080F1638[];
EC u32 func_080AB82C(NpcMovementChoice const * choices, u32 count);
EC u32 func_080AB788(u32 limit);
EC u32 func_080AB7A4(u32 excluded);
EC u32 GetNpcVariantFourSpeed(AActorEntity * self, u32 movement);
EC u32 GetNpcVariantFourAnimation(AActorEntity * self, u32 movement);

EC u32 ChooseNpcVariantFourMovement(AActorEntity * self) NPC_EXTRA_CODE(08037958);
EC u32 ChooseNpcVariantFourMovement(AActorEntity * self)
{
    NpcMovementChoice const * choices = gUnk_080F1638;
    NpcMovementChoice const * choice;
    u32 movement;
    {
        u32 selected = func_080AB82C(choices, 3);
        choice = choices + selected;
        movement = selected;
    }
    u32 duration = 60;
    u32 random_duration = func_080AB788(choice->random_seconds * duration);
    duration = choice->minimum_seconds * duration + random_duration;
    u32 facing = func_080AB788(4);
    if (facing != 0 && facing != 3)
    {
        facing = func_080AB7A4(self->facing);
        if (self->facing != facing)
            self->SetAnimFacing(facing);
    }
    SetMovingNpcFacingVelocity(self, GetNpcVariantFourSpeed(self, movement));
    u32 animation = GetNpcVariantFourAnimation(self, movement);
    if (self->anim_id != animation)
        self->SetAnim(animation);
    return duration;
}

EC ActorLocation func_080A198C();
EC ActorLocation func_080A19EC();
EC u32 GetNpcVariantThreeAnimation(AActorEntity * self, u32 movement);

EC void ResetNpcVariantThreeLocation(AActorEntity * self) NPC_EXTRA_CODE(080378B8);
EC void ResetNpcVariantThreeLocation(AActorEntity * self)
{
    self->SetLocation(func_080A198C());
    u32 animation = GetNpcVariantThreeAnimation(self, 0);
    if (self->anim_id != animation)
        self->SetAnim(animation);
}

EC void ResetNpcVariantFourLocation(AActorEntity * self) NPC_EXTRA_CODE(08037A04);
EC void ResetNpcVariantFourLocation(AActorEntity * self)
{
    self->SetLocation(func_080A19EC());
    u32 animation = GetNpcVariantFourAnimation(self, 0);
    if (self->anim_id != animation)
        self->SetAnim(animation);
}

EC UnknownEntityThing * CreateScheduledNpcSprite10(AActorEntity * self) NPC_EXTRA_CODE(0803601C);
EC UnknownEntityThing * CreateScheduledNpcSprite10(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite11(AActorEntity * self) NPC_EXTRA_CODE(080361C0);
EC UnknownEntityThing * CreateScheduledNpcSprite11(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite12(AActorEntity * self) NPC_EXTRA_CODE(08036228);
EC UnknownEntityThing * CreateScheduledNpcSprite12(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 2, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite13(AActorEntity * self) NPC_EXTRA_CODE(08036290);
EC UnknownEntityThing * CreateScheduledNpcSprite13(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 2, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite14(AActorEntity * self) NPC_EXTRA_CODE(080362FC);
EC UnknownEntityThing * CreateScheduledNpcSprite14(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 1, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite15(AActorEntity * self) NPC_EXTRA_CODE(08036364);
EC UnknownEntityThing * CreateScheduledNpcSprite15(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite16(AActorEntity * self) NPC_EXTRA_CODE(080363CC);
EC UnknownEntityThing * CreateScheduledNpcSprite16(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}
