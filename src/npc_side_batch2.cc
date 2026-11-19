#include "entity_actor.hh"

#define NPC_SIDE_CODE(address) SECTION(".text.npc_side." #address)
#define NPC_EXTRA_CODE(address) SECTION(".text.npc_extra." #address)

struct NpcAnimationSlots : AActorEntity
{
    NpcAnimationSlots();
    STRUCT_PAD(0x30, 0x44);
    u16 walking_animation;
    u16 idle_animation;
};

struct MovingNpcEntity : AActorEntity
{
    MovingNpcEntity();
    STRUCT_PAD(0x30, 0x3C);
    u16 animation;
    u16 timer;
    u8 hidden;
};

EC u32 func_0805E8F0(SpriteAnimator * animator);
EC u16 const gUnk_080F1560[];

struct SpriteWorkerEffect : UnknownEntityThing
{
    STRUCT_PAD(0x8C, 0xB4);
    SpriteAnimator effect_animator;
    u8 effect_visible;
    u8 effect_offset;
    u8 effect_flip;
    u8 effect_restart;
    i16 effect_x;
    i16 effect_y;
    u16 effect_timer;
};

EC u16 const gUnk_080F14F0[];
EC char const vtable_unk_080E6864[];
EC void func_0805E860(SpriteAnimator * animator, u32 animation);
EC UnknownEntityThing * func_080324BC(UnknownEntityThing * self, AActorEntity * actor, u32 layer, u32 animation, u32 priority, u32 palette, u32 flags, bool visible);

EC UnknownEntityThing * InitializeSideActorSprite(UnknownEntityThing * self, AActorEntity * actor) NPC_SIDE_CODE(08032A00);
EC UnknownEntityThing * InitializeSideActorSprite(UnknownEntityThing * self, AActorEntity * actor)
{
    func_080324BC(self, actor, 6, 32, 1, 0, 0, false);
    reinterpret_cast<void const **>(self)[1] = vtable_unk_080E6864;
    return self;
}

EC void StartSpriteWorkerEffect(SpriteWorkerEffect * self, u32 sprite, i32 x, i32 y) NPC_SIDE_CODE(08034BFC);
EC void StartSpriteWorkerEffect(SpriteWorkerEffect * self, u32 sprite, i32 x, i32 y)
{
    u32 animation = gUnk_080F14F0[sprite];
    func_0805E860(&self->effect_animator, animation);
    self->effect_visible = 1;
    self->effect_flip = 0;
    self->effect_restart = 1;
    self->effect_x = x;
    self->effect_y = y;
    self->effect_timer = 60;
}

EC void ShowSpriteWorkerEffect(AEntity * self, u32 sprite, i32 x, i32 y) NPC_SIDE_CODE(08033B84);
EC void ShowSpriteWorkerEffect(AEntity * self, u32 sprite, i32 x, i32 y)
{
    if (self->unk_10.Get() != nullptr)
        StartSpriteWorkerEffect(static_cast<SpriteWorkerEffect *>(self->unk_10.Get()), sprite, x, y);
}

EC void SetNpcWalkingAnimation(NpcAnimationSlots * self, u32 animation) NPC_EXTRA_CODE(08035908);
EC void SetNpcWalkingAnimation(NpcAnimationSlots * self, u32 animation)
{
    u32 previous = self->walking_animation;
    if (animation != previous)
    {
        self->walking_animation = animation;
        if (!self->vfunc_28() && self->anim_id == previous && self->anim_id != animation)
            self->SetAnim(animation);
    }
}

EC void SetNpcIdleAnimation(NpcAnimationSlots * self, u32 animation) NPC_EXTRA_CODE(08035940);
EC void SetNpcIdleAnimation(NpcAnimationSlots * self, u32 animation)
{
    u32 previous = self->idle_animation;
    if (animation != previous)
    {
        self->idle_animation = animation;
        if (!self->vfunc_28() && self->anim_id == previous && self->anim_id != animation)
            self->SetAnim(animation);
    }
}

EC bool IsNpcAtNorthernExit(AEntity * self) NPC_EXTRA_CODE(08035AE0);
EC bool IsNpcAtNorthernExit(AEntity * self)
{
    bool at_exit = false;
    if (self->location_map == 29 && i16(self->y_q16 >> 16) <= 55)
        at_exit = true;
    return at_exit;
}

EC u32 GetMovingNpcAnimation(MovingNpcEntity * self) NPC_EXTRA_CODE(08037430);
EC u32 GetMovingNpcAnimation(MovingNpcEntity * self)
{
    return self->animation;
}

EC void ResetMovingNpcVisibility(MovingNpcEntity * self) NPC_EXTRA_CODE(0803745C);
EC void ResetMovingNpcVisibility(MovingNpcEntity * self)
{
    self->AEntity::vfunc_14();
    self->hidden = 0;
}

EC u32 GetChildNpcAnimation(AEntity * self, u32 state) NPC_EXTRA_CODE(08037618);
EC u32 GetChildNpcAnimation(AEntity * self, u32 state)
{
    return gUnk_080F1560[state];
}

EC u32 GetChildNpcSpeed(AEntity * self, u32 state) NPC_EXTRA_CODE(08037628);
EC u32 GetChildNpcSpeed(AEntity * self, u32 state)
{
    if (state == 1)
        return 0x8000;
    return 0;
}
