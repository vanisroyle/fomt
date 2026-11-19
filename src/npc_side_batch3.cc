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

EC UnknownEntityThing * InitializeSideActorSpriteLayers(UnknownEntityThing * self, AActorEntity * actor, u32 layer, u32 animation, u32 priority, u32 palette, u32 flags, bool visible);

EC UnknownEntityThing * CreateScheduledNpcSprite01(AActorEntity * self) NPC_EXTRA_CODE(08035B38);
EC UnknownEntityThing * CreateScheduledNpcSprite01(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite02(AActorEntity * self) NPC_EXTRA_CODE(08035B9C);
EC UnknownEntityThing * CreateScheduledNpcSprite02(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite03(AActorEntity * self) NPC_EXTRA_CODE(08035D40);
EC UnknownEntityThing * CreateScheduledNpcSprite03(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 3, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite04(AActorEntity * self) NPC_EXTRA_CODE(08035DAC);
EC UnknownEntityThing * CreateScheduledNpcSprite04(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 1, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite05(AActorEntity * self) NPC_EXTRA_CODE(08035E14);
EC UnknownEntityThing * CreateScheduledNpcSprite05(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite06(AActorEntity * self) NPC_EXTRA_CODE(08035E7C);
EC UnknownEntityThing * CreateScheduledNpcSprite06(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 2, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite07(AActorEntity * self) NPC_EXTRA_CODE(08035EE4);
EC UnknownEntityThing * CreateScheduledNpcSprite07(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite08(AActorEntity * self) NPC_EXTRA_CODE(08035F48);
EC UnknownEntityThing * CreateScheduledNpcSprite08(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite09(AActorEntity * self) NPC_EXTRA_CODE(08035FB0);
EC UnknownEntityThing * CreateScheduledNpcSprite09(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 2, 0, false);
}

struct WanderChoice
{
    u8 weight;
    u8 minimum_seconds;
    u8 random_seconds;
    u8 padding;
};

EC WanderChoice const gUnk_080F1638[];
EC u32 ChooseWeightedEntityMovement(WanderChoice const * choices, u32 count);
EC u32 RollEntityMovementRandom(u32 limit);
EC int RotateEntityMovementFacing(int facing, int turn);
EC u32 GetNpcVariantFourSpeed(AActorEntity * self, u32 movement);
EC u32 GetNpcVariantFourAnimation(AActorEntity * self, u32 movement);

EC u32 ChooseNpcVariantFourMovement(AActorEntity * self) NPC_EXTRA_CODE(08037958);
EC u32 ChooseNpcVariantFourMovement(AActorEntity * self)
{
    WanderChoice const * choices = gUnk_080F1638;
    WanderChoice const * choice;
    u32 movement;
    {
        u32 selected = ChooseWeightedEntityMovement(choices, 3);
        choice = choices + selected;
        movement = selected;
    }
    u32 duration = 60;
    u32 random_duration = RollEntityMovementRandom(choice->random_seconds * duration);
    duration = choice->minimum_seconds * duration + random_duration;
    u32 facing = RollEntityMovementRandom(4);
    if (facing != 0 && facing != 3)
    {
        facing = RotateEntityMovementFacing(self->facing, facing);
        if (self->facing != facing)
            self->SetAnimFacing(facing);
    }
    SetMovingNpcFacingVelocity(self, GetNpcVariantFourSpeed(self, movement));
    u32 animation = GetNpcVariantFourAnimation(self, movement);
    if (self->anim_id != animation)
        self->SetAnim(animation);
    return duration;
}

EC ActorLocation GetNpcScheduleLocation41();
EC ActorLocation GetNpcScheduleLocation23();
EC u32 GetNpcVariantThreeAnimation(AActorEntity * self, u32 movement);

EC void ResetNpcVariantThreeLocation(AActorEntity * self) NPC_EXTRA_CODE(080378B8);
EC void ResetNpcVariantThreeLocation(AActorEntity * self)
{
    self->SetLocation(GetNpcScheduleLocation41());
    u32 animation = GetNpcVariantThreeAnimation(self, 0);
    if (self->anim_id != animation)
        self->SetAnim(animation);
}

EC void ResetNpcVariantFourLocation(AActorEntity * self) NPC_EXTRA_CODE(08037A04);
EC void ResetNpcVariantFourLocation(AActorEntity * self)
{
    self->SetLocation(GetNpcScheduleLocation23());
    u32 animation = GetNpcVariantFourAnimation(self, 0);
    if (self->anim_id != animation)
        self->SetAnim(animation);
}

EC UnknownEntityThing * CreateScheduledNpcSprite10(AActorEntity * self) NPC_EXTRA_CODE(0803601C);
EC UnknownEntityThing * CreateScheduledNpcSprite10(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite11(AActorEntity * self) NPC_EXTRA_CODE(080361C0);
EC UnknownEntityThing * CreateScheduledNpcSprite11(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite12(AActorEntity * self) NPC_EXTRA_CODE(08036228);
EC UnknownEntityThing * CreateScheduledNpcSprite12(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 2, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite13(AActorEntity * self) NPC_EXTRA_CODE(08036290);
EC UnknownEntityThing * CreateScheduledNpcSprite13(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 2, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite14(AActorEntity * self) NPC_EXTRA_CODE(080362FC);
EC UnknownEntityThing * CreateScheduledNpcSprite14(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 1, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite15(AActorEntity * self) NPC_EXTRA_CODE(08036364);
EC UnknownEntityThing * CreateScheduledNpcSprite15(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite16(AActorEntity * self) NPC_EXTRA_CODE(080363CC);
EC UnknownEntityThing * CreateScheduledNpcSprite16(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite17(AActorEntity * self) NPC_EXTRA_CODE(08036564);
EC UnknownEntityThing * CreateScheduledNpcSprite17(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite18(AActorEntity * self) NPC_EXTRA_CODE(08036704);
EC UnknownEntityThing * CreateScheduledNpcSprite18(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite19(AActorEntity * self) NPC_EXTRA_CODE(08036834);
EC UnknownEntityThing * CreateScheduledNpcSprite19(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite20(AActorEntity * self) NPC_EXTRA_CODE(08036898);
EC UnknownEntityThing * CreateScheduledNpcSprite20(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite21(AActorEntity * self) NPC_EXTRA_CODE(08036A4C);
EC UnknownEntityThing * CreateScheduledNpcSprite21(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite22(AActorEntity * self) NPC_EXTRA_CODE(08036B80);
EC UnknownEntityThing * CreateScheduledNpcSprite22(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 2, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite23(AActorEntity * self) NPC_EXTRA_CODE(08036C80);
EC UnknownEntityThing * CreateScheduledNpcSprite23(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite24(AActorEntity * self) NPC_EXTRA_CODE(08036CDC);
EC UnknownEntityThing * CreateScheduledNpcSprite24(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 2, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite25(AActorEntity * self) NPC_EXTRA_CODE(08036D3C);
EC UnknownEntityThing * CreateScheduledNpcSprite25(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC UnknownEntityThing * CreateScheduledNpcSprite26(AActorEntity * self) NPC_EXTRA_CODE(08036D98);
EC UnknownEntityThing * CreateScheduledNpcSprite26(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

struct Npc;
EC void InitializeScheduledNpcBase(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context, void const * schedule, u32 walking_animation, u32 idle_animation, u32 interaction)
    asm("__10ANpcEntityP10GameObjectP3NpcUiPCvUiUiUi");

EC u8 const gUnk_080F280C[];
EC u8 const vtable_unk_080E7198[];
EC AActorEntity * InitializeScheduledNpc01(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08035AFC);
EC AActorEntity * InitializeScheduledNpc01(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F280C, 607, 611, 994);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E7198;
    return self;
}

EC u8 const ScheduleInfo_Unk_080F1A80[];
EC u8 const vtable_unk_080E7158[];
EC AActorEntity * InitializeScheduledNpc02(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08035B64);
EC AActorEntity * InitializeScheduledNpc02(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, ScheduleInfo_Unk_080F1A80, 531, 535, 992);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E7158;
    return self;
}

EC u8 const gUnk_080F1FC0[];
EC u8 const vtable_unk_080E7118[];
EC AActorEntity * InitializeScheduledNpc03(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08035BC8);
EC AActorEntity * InitializeScheduledNpc03(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F1FC0, 559, 563, 993);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E7118;
    return self;
}

EC u8 const gUnk_080F8678[];
EC u8 const vtable_unk_080E70D8[];
EC AActorEntity * InitializeScheduledNpc04(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08035D04);
EC AActorEntity * InitializeScheduledNpc04(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F8678, 2543, 2547, 1030);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E70D8;
    return self;
}

EC u8 const gUnk_080F81BC[];
EC u8 const vtable_unk_080E7098[];
EC AActorEntity * InitializeScheduledNpc05(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08035D70);
EC AActorEntity * InitializeScheduledNpc05(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F81BC, 2535, 2539, 1029);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E7098;
    return self;
}

EC u8 const gUnk_080F77FC[];
EC u8 const vtable_unk_080E7058[];
EC AActorEntity * InitializeScheduledNpc06(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08035DD8);
EC AActorEntity * InitializeScheduledNpc06(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F77FC, 2465, 2469, 1027);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E7058;
    return self;
}

EC u8 const gUnk_080F7294[];
EC u8 const vtable_unk_080E7018[];
EC AActorEntity * InitializeScheduledNpc07(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08035E40);
EC AActorEntity * InitializeScheduledNpc07(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F7294, 2441, 2445, 1026);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E7018;
    return self;
}

EC u8 const gUnk_080F6370[];
EC u8 const vtable_unk_080E6FD8[];
EC AActorEntity * InitializeScheduledNpc08(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08035EAC);
EC AActorEntity * InitializeScheduledNpc08(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F6370, 2240, 2244, 1020);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6FD8;
    return self;
}

EC u8 const gUnk_080F66C4[];
EC u8 const vtable_unk_080E6F98[];
EC AActorEntity * InitializeScheduledNpc09(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08035F10);
EC AActorEntity * InitializeScheduledNpc09(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F66C4, 2256, 2260, 1021);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6F98;
    return self;
}

EC u8 const gUnk_080F49C0[];
EC u8 const vtable_unk_080E6F58[];
EC AActorEntity * InitializeScheduledNpc10(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08035F74);
EC AActorEntity * InitializeScheduledNpc10(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F49C0, 2059, 2063, 1013);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6F58;
    return self;
}

EC u8 const gUnk_080F5540[];
EC u8 const vtable_unk_080E6F18[];
EC AActorEntity * InitializeScheduledNpc11(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08035FE0);
EC AActorEntity * InitializeScheduledNpc11(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F5540, 2123, 2127, 1015);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6F18;
    return self;
}

EC u8 const gUnk_080F4D74[];
EC u8 const vtable_unk_080E6ED8[];
EC AActorEntity * InitializeScheduledNpc12(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08036048);
EC AActorEntity * InitializeScheduledNpc12(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F4D74, 2067, 2071, 1014);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6ED8;
    return self;
}

EC u8 const gUnk_080F59CC[];
EC u8 const vtable_unk_080E6E98[];
EC AActorEntity * InitializeScheduledNpc13(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08036184);
EC AActorEntity * InitializeScheduledNpc13(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F59CC, 2143, 2147, 1017);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6E98;
    return self;
}

EC u8 const gUnk_080F6B4C[];
EC u8 const vtable_unk_080E6E58[];
EC AActorEntity * InitializeScheduledNpc14(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(080361EC);
EC AActorEntity * InitializeScheduledNpc14(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F6B4C, 2276, 2280, 1023);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6E58;
    return self;
}

EC u8 const gUnk_080F33B8[];
EC u8 const vtable_unk_080E6E18[];
EC AActorEntity * InitializeScheduledNpc15(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08036258);
EC AActorEntity * InitializeScheduledNpc15(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F33B8, 1669, 1669, 1006);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6E18;
    return self;
}

EC u8 const gUnk_080F61FC[];
EC u8 const vtable_unk_080E6DD8[];
EC AActorEntity * InitializeScheduledNpc16(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(080362C0);
EC AActorEntity * InitializeScheduledNpc16(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F61FC, 2232, 2236, 1019);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6DD8;
    return self;
}

EC u8 const gUnk_080F3408[];
EC u8 const vtable_unk_080E6D98[];
EC AActorEntity * InitializeScheduledNpc17(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08036328);
EC AActorEntity * InitializeScheduledNpc17(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F3408, 1673, 1677, 1007);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6D98;
    return self;
}

EC u8 const gUnk_080F3FD8[];
EC u8 const vtable_unk_080E6D58[];
EC AActorEntity * InitializeScheduledNpc18(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08036390);
EC AActorEntity * InitializeScheduledNpc18(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F3FD8, 1733, 1737, 1009);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6D58;
    return self;
}

EC u8 const gUnk_080F35E4[];
EC u8 const vtable_unk_080E6D18[];
EC AActorEntity * InitializeScheduledNpc19(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(080363F8);
EC AActorEntity * InitializeScheduledNpc19(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F35E4, 1681, 1685, 1008);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6D18;
    return self;
}

EC u8 const gUnk_080F3010[];
EC u8 const vtable_unk_080E6CD8[];
EC AActorEntity * InitializeScheduledNpc20(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08036530);
EC AActorEntity * InitializeScheduledNpc20(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F3010, 800, 804, 998);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6CD8;
    return self;
}

EC u8 const gUnk_080F5D94[];
EC u8 const vtable_unk_080E6C98[];
EC AActorEntity * InitializeScheduledNpc21(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08036590);
EC AActorEntity * InitializeScheduledNpc21(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F5D94, 2180, 2184, 1018);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6C98;
    return self;
}

EC u8 const gUnk_080F6DE8[];
EC u8 const vtable_unk_080E6C58[];
EC AActorEntity * InitializeScheduledNpc22(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(080366CC);
EC AActorEntity * InitializeScheduledNpc22(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F6DE8, 2284, 2288, 1024);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6C58;
    return self;
}

EC u8 const gUnk_080F2AF8[];
EC u8 const vtable_unk_080E6C18[];
EC AActorEntity * InitializeScheduledNpc23(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08036730);
EC AActorEntity * InitializeScheduledNpc23(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F2AF8, 635, 639, 996);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6C18;
    return self;
}

EC u8 const gUnk_080F42F0[];
EC u8 const vtable_unk_080E6BD8[];
EC AActorEntity * InitializeScheduledNpc24(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(080367F8);
EC AActorEntity * InitializeScheduledNpc24(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F42F0, 1970, 1974, 1010);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6BD8;
    return self;
}

EC u8 const gUnk_080F4974[];
EC u8 const vtable_unk_080E6B98[];
EC AActorEntity * InitializeScheduledNpc25(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08036860);
EC AActorEntity * InitializeScheduledNpc25(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F4974, 2034, 2038, 1012);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6B98;
    return self;
}

EC u8 const gUnk_080F43DC[];
EC u8 const vtable_unk_080E6B58[];
EC AActorEntity * InitializeScheduledNpc26(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(080368C4);
EC AActorEntity * InitializeScheduledNpc26(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F43DC, 1982, 1986, 1011);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6B58;
    return self;
}

EC u8 const gUnk_080F6FF8[];
EC u8 const vtable_unk_080E6B18[];
EC AActorEntity * InitializeScheduledNpc27(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08036A10);
EC AActorEntity * InitializeScheduledNpc27(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F6FF8, 2306, 2310, 1025);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6B18;
    return self;
}

EC u8 const gCarpenterScheduleInfo[];
EC u8 const vtable_unk_080E6AD8[];
EC AActorEntity * InitializeCarpenterNpc(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08036A78);
EC AActorEntity * InitializeCarpenterNpc(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gCarpenterScheduleInfo, 2515, 2519, 1028);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6AD8;
    return self;
}

EC u8 const gUnk_080F2DC0[];
EC u8 const vtable_unk_080E6A98[];
EC AActorEntity * InitializeScheduledNpc29(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08036B4C);
EC AActorEntity * InitializeScheduledNpc29(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F2DC0, 792, 796, 997);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6A98;
    return self;
}

EC u8 const gUnk_080F597C[];
EC u8 const vtable_unk_080E6A58[];
EC AActorEntity * InitializeScheduledNpc30(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08036C48);
EC AActorEntity * InitializeScheduledNpc30(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F597C, 2135, 2139, 1016);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6A58;
    return self;
}
