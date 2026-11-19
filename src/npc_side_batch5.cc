#include "entity_actor.hh"

#define NPC_EXTRA_CODE(address) SECTION(".text.npc_extra." #address)
#define NPC_SIDE_CODE(address) SECTION(".text.npc_side." #address)

struct MovingNpcEntity : AActorEntity
{
    MovingNpcEntity();
    virtual void ChooseMovement();
    ActorLocation * saved_location;
    Box movement_bounds;
    u16 animation;
    u16 timer;
    u8 hidden;
};

struct SpriteWorkerEntity : AActorEntity
{
    SpriteWorkerEntity();
    u8 sprite_id;
    STRUCT_PAD(0x31, 0x41);
    u8 work_pause;
    u8 work_active;
};

struct ActivityAnimationRecord
{
    STRUCT_PAD(0x00, 0x0C);
    u32 activity;
};

struct ActivityAnimationNpc : AActorEntity
{
    ActivityAnimationNpc();
    STRUCT_PAD(0x30, 0x34);
    ActivityAnimationRecord * record;
    u32 animations[5];
};

EC UnknownEntityThing * func_080324BC(UnknownEntityThing * self, AActorEntity * actor, u32 layer, u32 animation, u32 priority, u32 palette, u32 flags, bool visible);
EC void func_08034F00(AActorEntity * self, i32 reason);
EC void func_08036E70(AActorEntity * self);
EC void ClearSpriteWorkPause(SpriteWorkerEntity * self);
EC u16 const gUnk_080F14B8[];

EC UnknownEntityThing * CreateScheduledNpc34Sprite(AActorEntity * self) NPC_EXTRA_CODE(08036E00);
EC UnknownEntityThing * CreateScheduledNpc34Sprite(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC void RefreshArrivalReactionNpc(AActorEntity * self, i32 reason) NPC_EXTRA_CODE(08036EF0);
EC void RefreshArrivalReactionNpc(AActorEntity * self, i32 reason)
{
    func_08034F00(self, reason);
    if (reason > 1)
        func_08036E70(self);
}

EC void * GetArrivalReactionNpcRecord(ActivityAnimationNpc * self) NPC_EXTRA_CODE(08037004);
EC void * GetArrivalReactionNpcRecord(ActivityAnimationNpc * self)
{
    return self->record;
}

EC void ClearMovingNpcBounds(MovingNpcEntity * self) NPC_EXTRA_CODE(08037250);
EC void ClearMovingNpcBounds(MovingNpcEntity * self)
{
    self->movement_bounds = Box(0, 0, 0, 0, 0, 0);
}

EC void ReactionSpriteIdleNoOp() NPC_EXTRA_CODE(08037D70);
EC void ReactionSpriteIdleNoOp()
{
}

EC u32 GetBusyNpcVariantAnimation() NPC_EXTRA_CODE(080398A0);
EC u32 GetBusyNpcVariantAnimation()
{
    return 2;
}

EC u32 GetActivityNpcAnimation() NPC_EXTRA_CODE(08039E88);
EC u32 GetActivityNpcAnimation()
{
    return 2;
}

EC u32 GetActivityNpcAnimationValue(ActivityAnimationNpc * self) NPC_EXTRA_CODE(08039E8C);
EC u32 GetActivityNpcAnimationValue(ActivityAnimationNpc * self)
{
    ActivityAnimationNpc * selected = reinterpret_cast<ActivityAnimationNpc *>(self->record->activity * sizeof(u32) + reinterpret_cast<u32>(self));
    return selected->animations[0];
}

EC void ResumeSpriteWorker(SpriteWorkerEntity * self) NPC_SIDE_CODE(08033AE4);
EC void ResumeSpriteWorker(SpriteWorkerEntity * self)
{
    self->unk_06 = false;
    self->work_active = 1;
    ClearSpriteWorkPause(self);
}

EC u32 GetSpriteWorkerBaseAnimation(SpriteWorkerEntity * self) NPC_SIDE_CODE(08033AFC);
EC u32 GetSpriteWorkerBaseAnimation(SpriteWorkerEntity * self)
{
    u16 const * animations = gUnk_080F14B8 + 14;
    return animations[self->sprite_id];
}

struct DirectedMovingNpc : MovingNpcEntity
{
    DirectedMovingNpc();
    u8 direction : 2;
    u8 flags : 6;
};

struct RecordedEffectEntity : AEntity
{
    RecordedEffectEntity();
    void * record;
    u8 active;
};

EC void InitializeActorBase(AActorEntity * self, GameObject * game, ActorLocation const & location, u32 layer, u32 animation)
    asm("__12AActorEntityP10GameObjectRC13ActorLocationUiUi");
EC void InitializeEntityBase(AEntity * self, GameObject * game, Location const & location)
    asm("__7AEntityP10GameObjectRC8Location");
EC Location ItemCollection_GetExtraItems(void * record);
EC void SetNpcMovementBounds(MovingNpcEntity * self, Box const & bounds);
EC u8 const vtable_unk_080E7328[];
EC u8 const vtable_unk_080E7218[];
EC u8 const vtable_unk_080E725C[];
EC u8 const vtable_unk_080E72A0[];
EC u8 const vtable_unk_080E72E4[];
EC u8 const vtable_unk_080E7380[];
EC u8 const vtable_unk_080E7568[];
EC u8 const vtable_unk_080E73FC[];

EC MovingNpcEntity * InitializeMovingNpcBase(MovingNpcEntity * self, GameObject * game, ActorLocation * location, u32 animation, u32 idle_animation) NPC_EXTRA_CODE(08037008);
EC MovingNpcEntity * InitializeMovingNpcBase(MovingNpcEntity * self, GameObject * game, ActorLocation * location, u32 animation, u32 idle_animation)
{
    InitializeActorBase(self, game, *location, 2, animation);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E7328;
    self->saved_location = location;
    Box * bounds = &self->movement_bounds;
    bounds->x1 = 0;
    bounds->y1 = 0;
    bounds->x2 = 0;
    bounds->y2 = 0;
    self->animation = idle_animation;
    self->timer = 0;
    self->hidden = 1;
    return self;
}

EC MovingNpcEntity * InitializeMovingNpcVariantSix(MovingNpcEntity * self, GameObject * game, ActorLocation * location) NPC_EXTRA_CODE(08037BB8);
EC MovingNpcEntity * InitializeMovingNpcVariantSix(MovingNpcEntity * self, GameObject * game, ActorLocation * location)
{
    InitializeMovingNpcBase(self, game, location, 519, 10);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E7218;
    return self;
}

EC MovingNpcEntity * InitializeMovingNpcVariantSeven(MovingNpcEntity * self, GameObject * game, ActorLocation * location) NPC_EXTRA_CODE(08037BE0);
EC MovingNpcEntity * InitializeMovingNpcVariantSeven(MovingNpcEntity * self, GameObject * game, ActorLocation * location)
{
    InitializeMovingNpcBase(self, game, location, 889, 12);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E725C;
    return self;
}

EC MovingNpcEntity * CreateMovingNpcVariantSeven(GameObject * game, ActorLocation * location) NPC_EXTRA_CODE(08037B48);
EC MovingNpcEntity * CreateMovingNpcVariantSeven(GameObject * game, ActorLocation * location)
{
    struct Storage
    {
        u8 bytes[0x44];
        Storage(GameObject * game, ActorLocation * location)
        {
            MovingNpcEntity * self = reinterpret_cast<MovingNpcEntity *>(this);
            InitializeMovingNpcBase(self, game, location, 889, 12);
            reinterpret_cast<void const **>(self)[5] = vtable_unk_080E725C;
        }
    };
    return reinterpret_cast<MovingNpcEntity *>(new Storage(game, location));
}

EC MovingNpcEntity * CreateMovingNpcVariantSix(GameObject * game, ActorLocation * location) NPC_EXTRA_CODE(08037B80);
EC MovingNpcEntity * CreateMovingNpcVariantSix(GameObject * game, ActorLocation * location)
{
    struct Storage
    {
        u8 bytes[0x44];
        Storage(GameObject * game, ActorLocation * location)
        {
            MovingNpcEntity * self = reinterpret_cast<MovingNpcEntity *>(this);
            InitializeMovingNpcBase(self, game, location, 519, 10);
            reinterpret_cast<void const **>(self)[5] = vtable_unk_080E7218;
        }
    };
    return reinterpret_cast<MovingNpcEntity *>(new Storage(game, location));
}

EC DirectedMovingNpc * InitializeDirectedMovingNpcOne(DirectedMovingNpc * self, GameObject * game, ActorLocation * location, u32 direction) NPC_EXTRA_CODE(08037C08);
EC DirectedMovingNpc * InitializeDirectedMovingNpcOne(DirectedMovingNpc * self, GameObject * game, ActorLocation * location, u32 direction)
{
    InitializeMovingNpcBase(self, game, location, 684, 11);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E72A0;
    self->direction = direction;
    Box bounds(920, 288);
    bounds.x2 += 328;
    bounds.y2 += 8;
    SetNpcMovementBounds(self, bounds);
    return self;
}

EC DirectedMovingNpc * InitializeDirectedMovingNpcTwo(DirectedMovingNpc * self, GameObject * game, ActorLocation * location, u32 direction) NPC_EXTRA_CODE(08037C68);
EC DirectedMovingNpc * InitializeDirectedMovingNpcTwo(DirectedMovingNpc * self, GameObject * game, ActorLocation * location, u32 direction)
{
    InitializeMovingNpcBase(self, game, location, 1820, 0);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E72E4;
    self->direction = direction;
    Box bounds(352, 216);
    bounds.x2 += 56;
    bounds.y2 += 16;
    SetNpcMovementBounds(self, bounds);
    return self;
}

EC RecordedEffectEntity * InitializeSideEffectEntity(RecordedEffectEntity * self, GameObject * game, void * record) NPC_EXTRA_CODE(08038740);
EC RecordedEffectEntity * InitializeSideEffectEntity(RecordedEffectEntity * self, GameObject * game, void * record)
{
    Location location(8, 0, 0);
    InitializeEntityBase(self, game, location);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E7380;
    self->record = record;
    return self;
}

EC RecordedEffectEntity * CreateActiveSideEffectEntity(GameObject * game, void * record) NPC_EXTRA_CODE(0803A798);
EC RecordedEffectEntity * CreateActiveSideEffectEntity(GameObject * game, void * record)
{
    struct Storage { u8 bytes[0x20]; };
    RecordedEffectEntity * self = reinterpret_cast<RecordedEffectEntity *>(new Storage);
    Location location(0, 0, 0);
    InitializeEntityBase(self, game, location);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E7568;
    self->record = record;
    self->active = 1;
    return self;
}

EC RecordedEffectEntity * InitializeTimedReactionEntity(RecordedEffectEntity * self, GameObject * game, void * record) NPC_EXTRA_CODE(08037E18);
EC RecordedEffectEntity * InitializeTimedReactionEntity(RecordedEffectEntity * self, GameObject * game, void * record)
{
    Location location = ItemCollection_GetExtraItems(record);
    InitializeEntityBase(self, game, location);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E73FC;
    self->record = record;
    return self;
}
