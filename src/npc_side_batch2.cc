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
    virtual void ChooseMovement();
    ActorLocation * saved_location;
    Box movement_bounds;
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

struct NpcReactionSprite
{
    STRUCT_PAD(0x00, 0x54);
    u8 reaction_timer;
};

struct LaunchedSideSprite : AEntity
{
    LaunchedSideSprite();
    Location * destination;
    i32 height_q16;
    i32 rise_speed;
    u8 variant;
    u8 launched;
    u8 moving;
    u8 padding;
    u16 animation;
};

EC u16 const gUnk_080F15A4[];
EC u16 const gUnk_080F161C[];
EC u16 const gUnk_080F1644[];
EC u16 const gUnk_080F16AE[];

EC u32 GetNpcVariantTwoAnimation(AEntity * self, u32 state) NPC_EXTRA_CODE(080377C4);
EC u32 GetNpcVariantTwoAnimation(AEntity * self, u32 state)
{
    return gUnk_080F15A4[state];
}

EC u32 GetNpcVariantTwoSpeed(AEntity * self, u32 state) NPC_EXTRA_CODE(080377D4);
EC u32 GetNpcVariantTwoSpeed(AEntity * self, u32 state)
{
    if (state == 1)
        return 0x8000;
    return 0;
}

EC u32 GetNpcVariantThreeAnimation(AEntity * self, u32 state) NPC_EXTRA_CODE(080378EC);
EC u32 GetNpcVariantThreeAnimation(AEntity * self, u32 state)
{
    return gUnk_080F161C[state];
}

EC u32 GetNpcVariantFourAnimation(AEntity * self, u32 state) NPC_EXTRA_CODE(08037A38);
EC u32 GetNpcVariantFourAnimation(AEntity * self, u32 state)
{
    return gUnk_080F1644[state];
}

EC u32 GetNpcVariantFourSpeed(AEntity * self, u32 state) NPC_EXTRA_CODE(08037A48);
EC u32 GetNpcVariantFourSpeed(AEntity * self, u32 state)
{
    if (state == 1)
        return 0x8000;
    return 0;
}

EC void TickNpcReactionTimer(NpcReactionSprite * self) NPC_EXTRA_CODE(08037F60);
EC void TickNpcReactionTimer(NpcReactionSprite * self)
{
    if (self->reaction_timer != 0)
        self->reaction_timer--;
}

EC void StartNpcReactionTimer(NpcReactionSprite * self) NPC_EXTRA_CODE(08037F74);
EC void StartNpcReactionTimer(NpcReactionSprite * self)
{
    self->reaction_timer = 10;
}

EC u8 IsSideSpriteLaunched(LaunchedSideSprite * self) NPC_EXTRA_CODE(08038108);
EC u8 IsSideSpriteLaunched(LaunchedSideSprite * self)
{
    return self->launched;
}

EC i32 GetSideSpriteHeight(LaunchedSideSprite * self) NPC_EXTRA_CODE(0803836C);
EC i32 GetSideSpriteHeight(LaunchedSideSprite * self)
{
    return i16(self->height_q16 >> 16);
}

EC u32 GetNpcVariantFiveAnimation(AEntity * self, u32 state) NPC_EXTRA_CODE(08039D4C);
EC u32 GetNpcVariantFiveAnimation(AEntity * self, u32 state)
{
    return gUnk_080F16AE[state];
}

struct TransitionSideSprite
{
    STRUCT_PAD(0x00, 0x4C);
    i32 transition_q16;
    u8 transitioning;
};

struct SideEntityEffects
{
    STRUCT_PAD(0x00, 0x0C);
    TransitionSideSprite * transition_sprite;
    void * particles;
};

EC i16 const gUnk_080F16D2[];
EC void func_08038DF0(SideEntityEffects * self);
EC void func_08038EB8(SideEntityEffects * self);
EC void func_08038EE0(SideEntityEffects * self);

EC void LaunchSideSprite(LaunchedSideSprite * self, u8 variant) NPC_EXTRA_CODE(080380EC);
EC void LaunchSideSprite(LaunchedSideSprite * self, u8 variant)
{
    self->variant = variant;
    self->height_q16 = 0x150000;
    self->rise_speed = 0x30000;
    self->launched = 1;
}

EC i32 GetSideEntitySine(AEntity * self, u32 phase) NPC_EXTRA_CODE(0803A320);
EC i32 GetSideEntitySine(AEntity * self, u32 phase)
{
    return gUnk_080F16D2[i16(phase)];
}

EC i32 GetSideEntityCosine(AEntity * self, u32 phase) NPC_EXTRA_CODE(0803A334);
EC i32 GetSideEntityCosine(AEntity * self, u32 phase)
{
    return gUnk_080F16D2[(i16(phase) + 64) & 255];
}

EC void BeginSideSpriteTransition(SideEntityEffects * self) NPC_EXTRA_CODE(08038E90);
EC void BeginSideSpriteTransition(SideEntityEffects * self)
{
    self->transition_sprite->transition_q16 = 0x1200000;
    self->transition_sprite->transitioning = 1;
}

EC u32 IsSideSpriteTransitionComplete(SideEntityEffects * self) NPC_EXTRA_CODE(08038EA0);
EC u32 IsSideSpriteTransitionComplete(SideEntityEffects * self)
{
    TransitionSideSprite * sprite = self->transition_sprite;
    bool complete = false;
    if (sprite->transitioning == 0)
        complete = true;
    return complete;
}

EC void CreateSideEntityEffectSprite(AEntity * self) NPC_EXTRA_CODE(080387B8);
EC void CreateSideEntityEffectSprite(AEntity * self)
{
    if (self->unk_10.Get() != nullptr)
        func_08038DF0(reinterpret_cast<SideEntityEffects *>(self->unk_10.Get()));
}

EC void BeginSideEntityTransition(AEntity * self) NPC_EXTRA_CODE(080387C8);
EC void BeginSideEntityTransition(AEntity * self)
{
    if (self->unk_10.Get() != nullptr)
        BeginSideSpriteTransition(reinterpret_cast<SideEntityEffects *>(self->unk_10.Get()));
}

EC u32 IsSideEntityTransitionComplete(AEntity * self) NPC_EXTRA_CODE(080387D8);
EC u32 IsSideEntityTransitionComplete(AEntity * self)
{
    u32 complete;
    if (self->unk_10.Get() == nullptr)
        complete = true;
    else
        complete = IsSideSpriteTransitionComplete(reinterpret_cast<SideEntityEffects *>(self->unk_10.Get()));
    return complete;
}

EC void ClearSideEntityEffectSprite(AEntity * self) NPC_EXTRA_CODE(080387EC);
EC void ClearSideEntityEffectSprite(AEntity * self)
{
    if (self->unk_10.Get() != nullptr)
        func_08038EB8(reinterpret_cast<SideEntityEffects *>(self->unk_10.Get()));
}

EC void CreateSideEntityParticleSprites(AEntity * self) NPC_EXTRA_CODE(080387FC);
EC void CreateSideEntityParticleSprites(AEntity * self)
{
    if (self->unk_10.Get() != nullptr)
        func_08038EE0(reinterpret_cast<SideEntityEffects *>(self->unk_10.Get()));
}

EC void ActivateMovingNpc(MovingNpcEntity * self) NPC_EXTRA_CODE(08037434);
EC void ActivateMovingNpc(MovingNpcEntity * self)
{
    if (!self->hidden)
        self->ChooseMovement();
    self->timer = 0;
    self->AEntity::vfunc_10();
}

EC Box GetMovingNpcBounds(AEntity * self) NPC_EXTRA_CODE(08037470);
EC Box GetMovingNpcBounds(AEntity * self)
{
    i32 x = i16(self->x_q16 >> 16);
    i32 y = i16(self->y_q16 >> 16);
    return Box(x, y - 2, 14, 14);
}

EC Box GetReactionNpcBounds(AEntity * self) NPC_EXTRA_CODE(08037ED0);
EC Box GetReactionNpcBounds(AEntity * self)
{
    i32 x = i16(self->x_q16 >> 16);
    i32 y = i16(self->y_q16 >> 16);
    return Box(x, y, 14, 9);
}

EC Box GetLaunchedSpriteBounds(AEntity * self) NPC_EXTRA_CODE(0803834C);
EC Box GetLaunchedSpriteBounds(AEntity * self)
{
    i32 x = i16(self->x_q16 >> 16);
    i32 y = i16(self->y_q16 >> 16);
    return Box(x, y, 8, 8);
}

EC void func_08038110(AEntity * self);
EC void UpdateLaunchedSideSprite(AEntity * self) NPC_EXTRA_CODE(08038300);
EC void UpdateLaunchedSideSprite(AEntity * self)
{
    if (self->unk_10.Get() != nullptr)
        self->unk_10.Get()->vfunc_0C();
    func_08038110(self);
}

EC void RefreshLaunchedSideSprite(AEntity * self, u32 value) NPC_EXTRA_CODE(08038320);
EC void RefreshLaunchedSideSprite(AEntity * self, u32 value)
{
    self->AEntity::vfunc_2C(value);
    func_08038110(self);
}

EC void * func_0803853C(void * sprite, AEntity * self);
EC void * CreateLaunchedSideSprite(AEntity * self) NPC_EXTRA_CODE(08038334);
EC void * CreateLaunchedSideSprite(AEntity * self)
{
    struct Storage { u8 bytes[0x48]; };
    return func_0803853C(new Storage, self);
}

EC void * func_08037D08(void * sprite, AEntity * self);
EC void * CreateReactionNpcSprite(AEntity * self) NPC_EXTRA_CODE(08037CF0);
EC void * CreateReactionNpcSprite(AEntity * self)
{
    struct Storage { u8 bytes[0x54]; };
    return func_08037D08(new Storage, self);
}

EC u32 func_080390D0(SideEntityEffects * self);
EC u32 HasSideEntityParticles(AEntity * self) NPC_EXTRA_CODE(0803880C);
EC u32 HasSideEntityParticles(AEntity * self)
{
    u32 present;
    if (self->unk_10.Get() == nullptr)
        present = 0;
    else
        present = func_080390D0(reinterpret_cast<SideEntityEffects *>(self->unk_10.Get()));
    return present;
}

EC u32 GetNpcVariantThreeSpeed(AEntity * self, u32 state) NPC_EXTRA_CODE(080378FC);
EC u32 GetNpcVariantThreeSpeed(AEntity * self, u32 state)
{
    switch (state)
    {
    case 0: case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10: case 12: default: return 0;
    case 1: return 0x8000;
    case 2: case 11: return 0x10000;
    }
}

EC void StopLaunchedSpriteMovement(LaunchedSideSprite * self) NPC_EXTRA_CODE(080384FC);
EC void StopLaunchedSpriteMovement(LaunchedSideSprite * self)
{
    if (self->animation != 49)
    {
        UnknownEntityThing * sprite = self->unk_10.Get();
        if (sprite != nullptr)
        {
            func_0805E860(&sprite->sprite_animator, 49);
            sprite->unk_44 = 1;
            sprite->unk_46 = 0;
            sprite->unk_47 = 1;
        }
        self->animation = 49;
    }
    self->moving = 0;
}

EC void UpdateLaunchedSpriteAnimation(UnknownEntityThing * self) NPC_EXTRA_CODE(08038580);
EC void UpdateLaunchedSpriteAnimation(UnknownEntityThing * self)
{
    if (self->unk_47 == 0)
    {
        if (i32(func_0805E8F0(&self->sprite_animator) << 30) < 0)
            self->unk_44 = 1;
    }
    else
        self->unk_47 = 0;
}

EC void * func_08037EF0(void * sprite, AEntity * self);
EC void * CreateTimedReactionSprite(AEntity * self) NPC_EXTRA_CODE(08037E9C);
EC void * CreateTimedReactionSprite(AEntity * self)
{
    struct Storage { u8 bytes[0x58]; };
    return func_08037EF0(new Storage, self);
}

struct ReactiveNpcEntity : AEntity
{
    ReactiveNpcEntity();
    void * reaction_state;
};

EC void func_0800FFF4(void * state, u32 reaction);
EC void SetNpcReaction(ReactiveNpcEntity * self, u8 const * reaction) NPC_EXTRA_CODE(08037EB4);
EC void SetNpcReaction(ReactiveNpcEntity * self, u8 const * reaction)
{
    func_0800FFF4(self->reaction_state, *reaction);
    if (self->unk_10.Get() != nullptr)
        StartNpcReactionTimer(reinterpret_cast<NpcReactionSprite *>(self->unk_10.Get()));
}

EC void SetNpcMovementBounds(MovingNpcEntity * self, Box const * bounds) NPC_EXTRA_CODE(08037244);
EC void SetNpcMovementBounds(MovingNpcEntity * self, Box const * bounds)
{
    self->movement_bounds = *bounds;
}

EC void func_08038398(LaunchedSideSprite * self, i32 x, i32 y);
EC void StartLaunchedSpriteMovement(LaunchedSideSprite * self, i32 x_q16, i32 y_q16, i32 target_x, i32 target_y) NPC_EXTRA_CODE(08038374);
EC void StartLaunchedSpriteMovement(LaunchedSideSprite * self, i32 x_q16, i32 y_q16, i32 target_x, i32 target_y)
{
    self->moving = 1;
    self->x_q16 = x_q16;
    self->y_q16 = y_q16;
    func_08038398(self, target_x, target_y);
}

EC void func_08034F00(AActorEntity * self, i32 reason);

EC void func_08035C04(AActorEntity * self);
EC void RefreshScheduledNpcVariantOne(AActorEntity * self, i32 reason) NPC_EXTRA_CODE(08035CB8);
EC void RefreshScheduledNpcVariantOne(AActorEntity * self, i32 reason)
{
    func_08034F00(self, reason);
    if (reason > 1)
        func_08035C04(self);
}

EC UnknownEntityThing * CreateScheduledNpcVariantOneSprite(AActorEntity * self) NPC_EXTRA_CODE(08035CD4);
EC UnknownEntityThing * CreateScheduledNpcVariantOneSprite(AActorEntity * self)
{
    func_08035C04(self);
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 5, 27, 1, 0, 0, false);
}

EC void func_08036084(AActorEntity * self);
EC void RefreshScheduledNpcVariantTwo(AActorEntity * self, i32 reason) NPC_EXTRA_CODE(08036138);
EC void RefreshScheduledNpcVariantTwo(AActorEntity * self, i32 reason)
{
    func_08034F00(self, reason);
    if (reason > 1)
        func_08036084(self);
}

EC UnknownEntityThing * CreateScheduledNpcVariantTwoSprite(AActorEntity * self) NPC_EXTRA_CODE(08036154);
EC UnknownEntityThing * CreateScheduledNpcVariantTwoSprite(AActorEntity * self)
{
    func_08036084(self);
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 5, 27, 1, 0, 0, false);
}

EC void func_08036430(AActorEntity * self);
EC void RefreshScheduledNpcVariantThree(AActorEntity * self, i32 reason) NPC_EXTRA_CODE(080364E4);
EC void RefreshScheduledNpcVariantThree(AActorEntity * self, i32 reason)
{
    func_08034F00(self, reason);
    if (reason > 1)
        func_08036430(self);
}

EC UnknownEntityThing * CreateScheduledNpcVariantThreeSprite(AActorEntity * self) NPC_EXTRA_CODE(08036500);
EC UnknownEntityThing * CreateScheduledNpcVariantThreeSprite(AActorEntity * self)
{
    func_08036430(self);
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 5, 27, 1, 0, 0, false);
}

EC void func_080365CC(AActorEntity * self);
EC void RefreshScheduledNpcVariantFour(AActorEntity * self, i32 reason) NPC_EXTRA_CODE(08036680);
EC void RefreshScheduledNpcVariantFour(AActorEntity * self, i32 reason)
{
    func_08034F00(self, reason);
    if (reason > 1)
        func_080365CC(self);
}

EC UnknownEntityThing * CreateScheduledNpcVariantFourSprite(AActorEntity * self) NPC_EXTRA_CODE(0803669C);
EC UnknownEntityThing * CreateScheduledNpcVariantFourSprite(AActorEntity * self)
{
    func_080365CC(self);
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 5, 27, 1, 0, 0, false);
}


EC void func_08036768(AActorEntity * self);
EC void RefreshScheduledNpcVariantFive(AActorEntity * self, i32 reason) NPC_EXTRA_CODE(080367AC);
EC void RefreshScheduledNpcVariantFive(AActorEntity * self, i32 reason)
{
    func_08034F00(self, reason);
    if (reason > 1)
        func_08036768(self);
}

EC UnknownEntityThing * CreateScheduledNpcVariantFiveSprite(AActorEntity * self) NPC_EXTRA_CODE(080367C8);
EC UnknownEntityThing * CreateScheduledNpcVariantFiveSprite(AActorEntity * self)
{
    func_08036768(self);
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 5, 27, 1, 0, 0, false);
}

EC void func_08036900(AActorEntity * self);
EC void RefreshScheduledNpcVariantSix(AActorEntity * self, i32 reason) NPC_EXTRA_CODE(080369C4);
EC void RefreshScheduledNpcVariantSix(AActorEntity * self, i32 reason)
{
    func_08034F00(self, reason);
    if (reason > 1)
        func_08036900(self);
}

EC UnknownEntityThing * CreateScheduledNpcVariantSixSprite(AActorEntity * self) NPC_EXTRA_CODE(080369E0);
EC UnknownEntityThing * CreateScheduledNpcVariantSixSprite(AActorEntity * self)
{
    func_08036900(self);
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 5, 27, 1, 0, 0, false);
}

EC void func_08036AB4(AActorEntity * self);
EC void RefreshScheduledNpcVariantSeven(AActorEntity * self, i32 reason) NPC_EXTRA_CODE(08036B00);
EC void RefreshScheduledNpcVariantSeven(AActorEntity * self, i32 reason)
{
    func_08034F00(self, reason);
    if (reason > 1)
        func_08036AB4(self);
}

EC UnknownEntityThing * CreateScheduledNpcVariantSevenSprite(AActorEntity * self) NPC_EXTRA_CODE(08036B1C);
EC UnknownEntityThing * CreateScheduledNpcVariantSevenSprite(AActorEntity * self)
{
    func_08036AB4(self);
    struct Storage { u8 bytes[0x8C]; };
    return func_080324BC(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 5, 27, 1, 0, 0, false);
}
