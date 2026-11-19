#include "entity_actor.hh"
#include "farm.hh"

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

struct NpcActivityState
{
    STRUCT_PAD(0x00, 0x0C);
    u32 state;
    STRUCT_PAD(0x10, 0x12);
    u8 busy;
};

struct ActiveNpcEntity : AActorEntity
{
    ActiveNpcEntity();
    void * npc;
    NpcActivityState * activity;
};

EC u32 IsWithinSideEntityExit(i32 x, i32 y) NPC_EXTRA_CODE(080391C0);
EC u32 IsWithinSideEntityExit(i32 x, i32 y)
{
    if (y <= 56 && x > 323 && x <= 356)
        return 1;
    if (y > 639 && x > 247 && x <= 280)
        return 1;
    return 0;
}

EC void SideEntityIdleNoOp() NPC_EXTRA_CODE(080391FC);
EC void SideEntityIdleNoOp()
{
}

EC u32 SideEntityCannotInteract() NPC_EXTRA_CODE(08039200);
EC u32 SideEntityCannotInteract()
{
    return 0;
}

EC u32 SideEntityCanInteract() NPC_EXTRA_CODE(0803930C);
EC u32 SideEntityCanInteract()
{
    return 1;
}

EC void ActiveNpcIdleNoOp() NPC_EXTRA_CODE(08039A5C);
EC void ActiveNpcIdleNoOp()
{
}

EC u32 GetBusyNpcFacing(void * context, ActiveNpcEntity * self) NPC_EXTRA_CODE(080396F4);
EC u32 GetBusyNpcFacing(void * context, ActiveNpcEntity * self)
{
    u32 busy = self->activity->busy;
    u32 facing = 0;
    if (busy != 0)
        facing = 3;
    return facing;
}

EC u32 IsNpcActivityNotFour(ActiveNpcEntity * self) NPC_EXTRA_CODE(08039D98);
EC u32 IsNpcActivityNotFour(ActiveNpcEntity * self)
{
    return self->activity->state != 4;
}

EC u32 GetActiveNpcMovementSpeed(AEntity * self, u32 movement) NPC_EXTRA_CODE(08039D5C);
EC u32 GetActiveNpcMovementSpeed(AEntity * self, u32 movement)
{
    switch (movement)
    {
    case 1: return 0x8000;
    case 2: return 0x10000;
    case 0: case 3: case 4: default: return 0;
    }
}

EC UnknownEntityThing * func_08038820(UnknownEntityThing * self, AEntity * entity);
EC UnknownEntityThing * CreateSideEntityEffectController(AEntity * self) NPC_EXTRA_CODE(080387A0);
EC UnknownEntityThing * CreateSideEntityEffectController(AEntity * self)
{
    struct Storage { u8 bytes[0x18]; };
    return func_08038820(reinterpret_cast<UnknownEntityThing *>(new Storage), self);
}

EC void InitializeEntityBase(AEntity * self, GameObject * game, Location const & location) asm("__7AEntityP10GameObjectRC8Location");
EC u8 const vtable_unk_080E7444[];
EC AEntity * InitializeReactionEntity(AEntity * self, GameObject * game, Location const & location) NPC_EXTRA_CODE(08037CC4);
EC AEntity * InitializeReactionEntity(AEntity * self, GameObject * game, Location const & location)
{
    InitializeEntityBase(self, game, location);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E7444;
    return self;
}

struct PACKED SpriteWorkTarget
{
    u8 kind : 4;
    u8 flags : 4;
    u8 index;
    u8 row;
};

struct SpriteTaskActor
{
    STRUCT_PAD(0x00, 0x04);
    u16 map;
    STRUCT_PAD(0x06, 0x0A);
    i16 x;
    STRUCT_PAD(0x0C, 0x0E);
    i16 y;
};

struct SpriteTaskActorProvider;
struct SpriteTaskActorProviderVTable
{
    STRUCT_PAD(0x00, 0x44);
    SpriteTaskActor * (*get_actor)(SpriteTaskActorProvider *, u32);
};

struct SpriteTaskActorProvider
{
    SpriteTaskActorProviderVTable * vtable;
};

struct IndexedSpriteWorkerEntity : AActorEntity
{
    IndexedSpriteWorkerEntity();
    u8 sprite_id;
};

struct SideEntityEffectFlags
{
    STRUCT_PAD(0x00, 0x26);
    u8 effect_state;
};

struct SideEntityItemCounter : AEntity
{
    SideEntityItemCounter();
    void * item_collection;
};

EC Vec2 GetSpriteTaskActorCoordinates(SpriteTaskActorProvider * actors, u32 actor_index) NPC_SIDE_CODE(08033800);
EC Vec2 GetSpriteTaskActorCoordinates(SpriteTaskActorProvider * actors, u32 actor_index)
{
    SpriteTaskActor * actor = actors->vtable->get_actor(actors, actor_index);
    if (actor != nullptr)
        return Vec2(actor->x, actor->y);
    return Vec2(0, 0);
}

EC Vec2 GetSpriteWorkTargetCoordinates(SpriteWorkTarget const * target, Farm const * farm, SpriteTaskActorProvider * actors) NPC_SIDE_CODE(08033830);
EC Vec2 GetSpriteWorkTargetCoordinates(SpriteWorkTarget const * target, Farm const * farm, SpriteTaskActorProvider * actors)
{
    switch (target->kind)
    {
    case 0: default: return Vec2(0, 0);
    case 1: case 2: return Vec2(target->index * 16 + 280, target->row * 16 + 184);
    case 3: return farm->coop.method_0800C6F0(target->index);
    case 4: return farm->barn.method_0800D074(target->index);
    case 5: return farm->barn.method_0800D0C0(target->index);
    case 6: return GetSpriteTaskActorCoordinates(actors, target->index + 54);
    case 7: return GetSpriteTaskActorCoordinates(actors, target->index + 46);
    case 8: return GetSpriteTaskActorCoordinates(actors, target->index + 76);
    }
}

EC u8 GetSpriteWorkerIndex(IndexedSpriteWorkerEntity * self) NPC_SIDE_CODE(08034C40);
EC u8 GetSpriteWorkerIndex(IndexedSpriteWorkerEntity * self)
{
    return self->sprite_id;
}

EC UnknownEntityThing * GetSideEntitySprite(AEntity * self) NPC_SIDE_CODE(08034C48);
EC UnknownEntityThing * GetSideEntitySprite(AEntity * self)
{
    return self->unk_10.Get();
}

EC void ShowSpriteWorkerEffect(AEntity * self, u32 sprite, i32 x, i32 y);
EC void ShowIndexedSpriteWorkerEffect(IndexedSpriteWorkerEntity * self, i32 x, i32 y) NPC_SIDE_CODE(08034C4C);
EC void ShowIndexedSpriteWorkerEffect(IndexedSpriteWorkerEntity * self, i32 x, i32 y)
{
    ShowSpriteWorkerEffect(self, self->sprite_id, x, y);
}

EC void GetReactionSpriteBounds(Box * result, SpriteTaskActor const * actor) NPC_EXTRA_CODE(08037CDC);
EC void GetReactionSpriteBounds(Box * result, SpriteTaskActor const * actor)
{
    i32 x = actor->x;
    i32 y = actor->y;
    result->x1 = x;
    result->y1 = y + 1;
    result->x2 = x;
    result->y2 = y + 1;
}

EC void RefreshSideEntityEffectSprite(UnknownEntityThing * self, u32 clip_id) NPC_EXTRA_CODE(080390DC);
EC void RefreshSideEntityEffectSprite(UnknownEntityThing * self, u32 clip_id)
{
    SetSpriteAnimatorClip(&self->sprite_animator, clip_id);
    self->unk_44 = 1;
    self->unk_46 = 0;
    self->unk_47 = 1;
}

EC u8 GetSideEntityEffectState(SideEntityEffectFlags * self) NPC_EXTRA_CODE(08039100);
EC u8 GetSideEntityEffectState(SideEntityEffectFlags * self)
{
    return self->effect_state;
}

EC void GetSideEntityEffectBounds(Box * result, i32 x, i32 y) NPC_EXTRA_CODE(08039108);
EC void GetSideEntityEffectBounds(Box * result, i32 x, i32 y)
{
    result->x1 = x - 7;
    result->y1 = y - 4;
    result->x2 = x + 7;
    result->y2 = y + 5;
}

EC u32 ItemCollection_GetByteCount(void * self);
EC u32 HasThirtySideEntityItems(SideEntityItemCounter * self) NPC_EXTRA_CODE(0803911C);
EC u32 HasThirtySideEntityItems(SideEntityItemCounter * self)
{
    u32 count = ItemCollection_GetByteCount(self->item_collection);
    u32 active = 0;
    if (count == 30)
        active = 1;
    return active;
}
