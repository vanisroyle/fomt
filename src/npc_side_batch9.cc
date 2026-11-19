#include "entity_actor.hh"
#include "g1_map_resources.hh"

#define NPC_SIDE_CODE(address) SECTION(".text.npc_side." #address)
#define NPC_EXTRA_CODE(address) SECTION(".text.npc_extra." #address)

struct MovingNpcClock
{
    STRUCT_PAD(0x00, 0x0A);
    GameTime time;
};

struct MovingNpcServices;
struct MovingNpcServicesVTable
{
    STRUCT_PAD(0x00, 0x144);
    MovingNpcClock * (*get_clock)(MovingNpcServices *);
};

struct MovingNpcServices
{
    MovingNpcServicesVTable * vtable;
};

struct WanderChoice
{
    u8 weight;
    u8 minimum_seconds;
    u8 random_seconds;
    u8 padding;
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

struct SpriteWorkerEntity : AActorEntity
{
    SpriteWorkerEntity();
    u8 sprite_id;
    STRUCT_PAD(0x31, 0x34);
    void * record;
    void * work_targets;
    u32 work_context;
    u8 work_state;
    u8 work_pause;
    u8 work_active;
};

EC WanderChoice const gUnk_080F1538[2][5];
EC WanderChoice const gUnk_080F156C[2][7];
EC WanderChoice const gUnk_080F15B4[2][13];
EC u32 ChooseWeightedEntityMovement(WanderChoice const * choices, u32 count);
EC u32 RollEntityMovementRandom(u32 limit);
EC int RotateEntityMovementFacing(int facing, int turn);
EC void SetMovingNpcFacingVelocity(AActorEntity * self, i32 speed);
EC u32 GetChildNpcSpeed(AActorEntity * self, u32 movement);
EC u32 GetChildNpcAnimation(AActorEntity * self, u32 movement);
EC u32 GetNpcVariantTwoSpeed(AActorEntity * self, u32 movement);
EC u32 GetNpcVariantTwoAnimation(AActorEntity * self, u32 movement);
EC u32 GetNpcVariantThreeSpeed(AActorEntity * self, u32 movement);
EC u32 GetNpcVariantThreeAnimation(AActorEntity * self, u32 movement);
EC void func_0805E860(SpriteAnimator * animator, u32 animation);
EC u32 GetSpriteTaskAnimation(AActorEntity * self, u32 state);
EC void func_08032384(AActorEntity * self, u32 animation, bool visible);
EC void func_080323C8(AActorEntity * self);

inline i32 SpriteWorkerSpeedMagnitude(i32 speed)
{
    return __builtin_abs(speed);
}

EC void SetSpriteWorkerMovementState(SpriteWorkerEntity * self, i32 state, i32 facing, i32 speed) NPC_SIDE_CODE(08034180);
EC void SetSpriteWorkerMovementState(SpriteWorkerEntity * self, i32 state, i32 facing, i32 speed)
{
    u32 animation = GetSpriteTaskAnimation(self, state);
    if (self->anim_id != animation)
        self->SetAnim(animation);
    if (self->facing != facing)
        self->SetAnimFacing(facing);
    self->work_active = 0;
    switch (state)
    {
    case 0: case 2: case 3: case 4:
        self->x_speed_q16 = 0;
        self->y_speed_q16 = 0;
        break;
    case 1:
        speed = SpriteWorkerSpeedMagnitude(speed);
        switch (facing)
        {
        case 0: self->y_speed_q16 = speed; self->x_speed_q16 = 0; break;
        case 1: self->y_speed_q16 = -speed; self->x_speed_q16 = 0; break;
        case 2: self->x_speed_q16 = -speed; self->y_speed_q16 = 0; break;
        case 3: self->x_speed_q16 = speed; self->y_speed_q16 = 0; break;
        }
        break;
    }
    if (state != self->work_state)
    {
        i32 overlay = -1;
        switch (state)
        {
        case 2: overlay = 8; break;
        case 3: overlay = 5; break;
        }
        if (overlay >= 0)
            func_08032384(self, overlay, true);
        else
            func_080323C8(self);
    }
    self->work_state = state;
}

EC u32 ChooseChildNpcMovement(AActorEntity * self) NPC_EXTRA_CODE(08037494);
EC u32 ChooseChildNpcMovement(AActorEntity * self)
{
    MovingNpcServices * world = reinterpret_cast<MovingNpcServices *>(self->game_object);
    u32 hour = world->vtable->get_clock(world)->time.clock_hour;
    u32 row = 0;
    if (hour - 6 > 14)
        row = 1;
    WanderChoice const * choices = gUnk_080F1538[row];
    WanderChoice const * choice;
    u32 movement;
    {
        u32 selected = ChooseWeightedEntityMovement(choices, 5);
        choice = choices + selected;
        movement = selected;
    }
    u32 duration = 60;
    u32 random_duration = RollEntityMovementRandom(choice->random_seconds * duration);
    duration = choice->minimum_seconds * duration + random_duration;
    u32 facing = RollEntityMovementRandom(4);
    u32 duration_ticks = duration;
    if (facing != 0 && facing != 3)
    {
        facing = RotateEntityMovementFacing(self->facing, facing);
        if (self->facing != facing)
            self->SetAnimFacing(facing);
    }
    SetMovingNpcFacingVelocity(self, GetChildNpcSpeed(self, movement));
    u32 animation = GetChildNpcAnimation(self, movement);
    if (self->anim_id != animation)
        self->SetAnim(animation);
    return duration_ticks;
}

EC u32 ChooseNpcVariantTwoMovement(AActorEntity * self) NPC_EXTRA_CODE(0803763C);
EC u32 ChooseNpcVariantTwoMovement(AActorEntity * self)
{
    MovingNpcServices * world = reinterpret_cast<MovingNpcServices *>(self->game_object);
    u32 hour = world->vtable->get_clock(world)->time.clock_hour;
    u32 row = 0;
    if (hour - 6 > 14)
        row = 1;
    WanderChoice const * choices = gUnk_080F156C[row];
    WanderChoice const * choice;
    u32 movement;
    {
        u32 selected = ChooseWeightedEntityMovement(choices, 7);
        choice = choices + selected;
        movement = selected;
    }
    u32 duration = 60;
    u32 random_duration = RollEntityMovementRandom(choice->random_seconds * duration);
    duration = choice->minimum_seconds * duration + random_duration;
    u32 facing = RollEntityMovementRandom(4);
    u32 duration_ticks = duration;
    if (facing != 0 && facing != 3)
    {
        facing = RotateEntityMovementFacing(self->facing, facing);
        if (self->facing != facing)
            self->SetAnimFacing(facing);
    }
    SetMovingNpcFacingVelocity(self, GetNpcVariantTwoSpeed(self, movement));
    u32 animation = GetNpcVariantTwoAnimation(self, movement);
    if (self->anim_id != animation)
        self->SetAnim(animation);
    return duration_ticks;
}

EC u32 ChooseNpcVariantThreeMovement(AActorEntity * self) NPC_EXTRA_CODE(080377E8);
EC u32 ChooseNpcVariantThreeMovement(AActorEntity * self)
{
    MovingNpcServices * world = reinterpret_cast<MovingNpcServices *>(self->game_object);
    u32 hour = world->vtable->get_clock(world)->time.clock_hour;
    u32 row = 0;
    if (hour - 6 > 14)
        row = 1;
    WanderChoice const * choices = gUnk_080F15B4[row];
    WanderChoice const * choice;
    u32 movement;
    {
        u32 selected = ChooseWeightedEntityMovement(choices, 13);
        choice = choices + selected;
        movement = selected;
    }
    u32 duration = 60;
    u32 random_duration = RollEntityMovementRandom(choice->random_seconds * duration);
    duration = choice->minimum_seconds * duration + random_duration;
    u32 facing = RollEntityMovementRandom(4);
    u32 duration_ticks = duration;
    if (facing != 0 && facing != 3)
    {
        facing = RotateEntityMovementFacing(self->facing, facing);
        if (self->facing != facing)
            self->SetAnimFacing(facing);
    }
    SetMovingNpcFacingVelocity(self, GetNpcVariantThreeSpeed(self, movement));
    u32 animation = GetNpcVariantThreeAnimation(self, movement);
    if (self->anim_id != animation)
        self->SetAnim(animation);
    return duration_ticks;
}

inline i32 AddLaunchedSpriteAnimationVariant(u32 animation, u32 variant)
{
    return animation + variant;
}

EC void SelectLaunchedSpriteAnimation(LaunchedSideSprite * self, u32 base_animation, u32 variant) NPC_EXTRA_CODE(08038398);
EC void SelectLaunchedSpriteAnimation(LaunchedSideSprite * self, u32 base_animation, u32 variant)
{
    u32 animation = 21;
    switch (base_animation)
    {
    case 824: break;
    case 828: animation = 25; break;
    case 832: animation = 29; break;
    case 836: animation = 33; break;
    case 840: animation = 37; break;
    case 844: animation = 41; break;
    case 885: animation = 45; break;
    }
    u32 selected = AddLaunchedSpriteAnimationVariant(animation, variant);
    if (selected != self->animation)
    {
        UnknownEntityThing * sprite = self->unk_10.Get();
        if (sprite != nullptr)
        {
            func_0805E860(&sprite->sprite_animator, selected);
            sprite->unk_44 = 1;
            sprite->unk_46 = 0;
            sprite->unk_47 = 1;
        }
        self->animation = selected;
    }
}
