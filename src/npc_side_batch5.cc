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
