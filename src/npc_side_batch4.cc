#include "entity_actor.hh"

#define NPC_EXTRA_CODE(address) SECTION(".text.npc_extra." #address)
#define NPC_SIDE_CODE(address) SECTION(".text.npc_side." #address)

struct Npc;
EC void InitializeScheduledNpcBase(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context, void const * schedule, u32 walking_animation, u32 idle_animation, u32 interaction)
    asm("__10ANpcEntityP10GameObjectP3NpcUiPCvUiUiUi");

struct ArrivalReactionNpc : AActorEntity
{
    ArrivalReactionNpc();
    STRUCT_PAD(0x30, 0x48);
    u16 arrival_timer;
};

EC u8 const gUnk_080F6B10[];
EC u8 const gUnk_080F29C0[];

EC u8 const vtable_unk_080E6A18[];
EC AActorEntity * InitializeUnscheduledNpcOne(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08036CAC);
EC AActorEntity * InitializeUnscheduledNpcOne(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, nullptr, 1657, 1661, 0);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6A18;
    return self;
}

EC u8 const vtable_unk_080E69D8[];
EC AActorEntity * InitializeUnscheduledNpcTwo(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08036D0C);
EC AActorEntity * InitializeUnscheduledNpcTwo(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, nullptr, 1641, 1645, 0);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E69D8;
    return self;
}

EC u8 const vtable_unk_080E6998[];
EC AActorEntity * InitializeUnscheduledNpcThree(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08036D68);
EC AActorEntity * InitializeUnscheduledNpcThree(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, nullptr, 2046, 2046, 0);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6998;
    return self;
}

EC u8 const vtable_unk_080E6958[];
EC AActorEntity * InitializeScheduledNpc34(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08036DC4);
EC AActorEntity * InitializeScheduledNpc34(AActorEntity * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F6B10, 2264, 2268, 1022);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6958;
    return self;
}

EC u8 const vtable_unk_080E6918[];
EC ArrivalReactionNpc * InitializeArrivalReactionNpc(ArrivalReactionNpc * self, GameObject * game, Npc * npc, u32 schedule_context) NPC_EXTRA_CODE(08036E2C);
EC ArrivalReactionNpc * InitializeArrivalReactionNpc(ArrivalReactionNpc * self, GameObject * game, Npc * npc, u32 schedule_context)
{
    InitializeScheduledNpcBase(self, game, npc, schedule_context, gUnk_080F29C0, 615, 623, 995);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6918;
    self->arrival_timer = 0;
    return self;
}

EC UnknownEntityThing * func_080324BC(UnknownEntityThing * self, AActorEntity * actor, u32 layer, u32 animation, u32 priority, u32 palette, u32 flags, bool visible);

EC UnknownEntityThing * CreateChildNpcSprite(AActorEntity * self) NPC_EXTRA_CODE(0803753C);
EC UnknownEntityThing * CreateChildNpcSprite(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 2, 9, 0, 7, 0, false);
}

EC UnknownEntityThing * CreateNpcVariantTwoSprite(AActorEntity * self) NPC_EXTRA_CODE(080376E4);
EC UnknownEntityThing * CreateNpcVariantTwoSprite(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 12, 2, 12, 0, false);
}

EC UnknownEntityThing * CreateNpcVariantThreeSprite(AActorEntity * self) NPC_EXTRA_CODE(0803788C);
EC UnknownEntityThing * CreateNpcVariantThreeSprite(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 2, 27, 0, 8, 0, false);
}

EC UnknownEntityThing * CreateNpcVariantFourSprite(AActorEntity * self) NPC_EXTRA_CODE(080379D8);
EC UnknownEntityThing * CreateNpcVariantFourSprite(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 2, 27, 0, 8, 0, false);
}

EC UnknownEntityThing * CreateNpcVariantFiveSprite(AActorEntity * self) NPC_EXTRA_CODE(08039A30);
EC UnknownEntityThing * CreateNpcVariantFiveSprite(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 2, 27, 0, 8, 0, false);
}
