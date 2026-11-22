#include "entity_actor.hh"
#include "farm.hh"
#include "harvest_sprite.hh"

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

struct DirectedMovingNpc : MovingNpcEntity
{
    DirectedMovingNpc();
    u8 direction : 2;
    u8 flags : 6;
};

EC MovingNpcEntity * InitializeMovingNpcBase(MovingNpcEntity * self, GameObject * game, ActorLocation * location, u32 animation, u32 idle_animation);
EC void SetNpcMovementBounds(MovingNpcEntity * self, Box const & bounds);
EC u8 const vtable_unk_080E72A0[];
EC u8 const vtable_unk_080E72E4[];

struct NpcWanderBounds
{
    i16 x1, y1, x2, y2;

    NpcWanderBounds(u32 x, u32 y, u32 width, u32 height)
    {
        i16 * origin = &x1;
        *origin = x;
        y1 = y;
        x2 = x + width;
        y2 = y + height;
    }
};

EC DirectedMovingNpc * CreateDirectedMovingNpcTwo(GameObject * game, ActorLocation * location, u32 direction) NPC_EXTRA_CODE(08037A5C);
EC DirectedMovingNpc * CreateDirectedMovingNpcTwo(GameObject * game, ActorLocation * location, u32 direction)
{
    struct Storage
    {
        u8 bytes[0x48];
        Storage(GameObject * game, ActorLocation * location, u32 direction)
        {
            DirectedMovingNpc * self = reinterpret_cast<DirectedMovingNpc *>(this);
            InitializeMovingNpcBase(self, game, location, 1820, 0);
            reinterpret_cast<void const **>(self)[5] = vtable_unk_080E72E4;
            self->direction = direction;
            NpcWanderBounds bounds(352, 216, 56, 16);
            SetNpcMovementBounds(self, reinterpret_cast<Box const &>(bounds));
        }
    };
    return reinterpret_cast<DirectedMovingNpc *>(new Storage(game, location, direction));
}

struct MovingNpcCalendar
{
    u32 weather;
    u32 reserved;
    u8 year : 3;
    u8 flags : 5;
    GameDate date;
    GameTime time;
};

struct MovingNpcServices;
struct MovingNpcServicesVTable
{
    STRUCT_PAD(0x00, 0x144);
    MovingNpcCalendar * (*get_calendar)(MovingNpcServices *);
};

struct MovingNpcServices
{
    MovingNpcServicesVTable * vtable;
};

struct EntityCollisionProbe;
EC void ProbeEntityCollisionDown(EntityCollisionProbe & probe);
EC void ProbeEntityCollisionUp(EntityCollisionProbe & probe);
EC void ProbeEntityCollisionLeft(EntityCollisionProbe & probe);
EC void ProbeEntityCollisionRight(EntityCollisionProbe & probe);

struct EntityCollisionProbe
{
    i32 down, up, left, right;
    u8 down_clear, up_clear, left_clear, right_clear;
    STRUCT_PAD(0x14, 0x28);
    i32 step;
    i32 horizontal_lookahead;
    i32 vertical_lookahead;

    i32 GetDown()
    {
        i32 distance = down;
        if (distance > step)
        {
            ProbeEntityCollisionDown(*this);
            return down;
        }
        return distance;
    }
    i32 GetUp()
    {
        i32 distance = up;
        if (distance < -step)
        {
            ProbeEntityCollisionUp(*this);
            return up;
        }
        return distance;
    }
    i32 GetLeft()
    {
        i32 distance = left;
        if (distance < -step)
        {
            ProbeEntityCollisionLeft(*this);
            return left;
        }
        return distance;
    }
    i32 GetRight()
    {
        i32 distance = right;
        if (distance > step)
        {
            ProbeEntityCollisionRight(*this);
            return right;
        }
        return distance;
    }
};

EC void ApplyMovingNpcCollisionLimits(AActorEntity * self, EntityCollisionProbe * probe) NPC_EXTRA_CODE(08037118);
EC void ApplyMovingNpcCollisionLimits(AActorEntity * self, EntityCollisionProbe * probe)
{
    i32 old_x = self->x_q16;
    i32 old_y = self->y_q16;
    i32 next_x = old_x + self->x_speed_q16;
    i32 next_y = old_y + self->y_speed_q16;
    i32 x = old_x >> 16;
    i32 y = old_y >> 16;
    i32 dy = (next_y >> 16) - y;
    i32 dx = (next_x >> 16) - x;
    if (dy > 0 && dy > probe->GetDown())
        next_y = (y + probe->GetDown()) << 16;
    if (dy < 0 && dy < probe->GetUp())
        next_y = (y + probe->GetUp()) << 16;
    if (dx < 0 && dx < probe->GetLeft())
        next_x = (x + probe->GetLeft()) << 16;
    if (dx > 0 && dx > probe->GetRight())
        next_x = (x + probe->GetRight()) << 16;
    self->x_q16 = next_x;
    self->y_q16 = next_y;
}

struct SpriteWorkerEntity : AActorEntity
{
    SpriteWorkerEntity();
    u8 sprite_id;
    STRUCT_PAD(0x31, 0x34);
    HarvestSprite * record;
    void * work_targets;
    u32 work_context;
    u8 work_state;
    u8 work_pause;
    u8 work_active;
};

EC void ClearNpcEventSlot(void * slots, u32 index);
EC void BeginSpriteWorkerReturn(HarvestSprite * record, UnkBarnAnimal2C const & state) asm("method_0809E7FC__13HarvestSprite");
EC void SetSpriteWorkerMovementState(SpriteWorkerEntity * self, i32 state, i32 facing, i32 speed);

inline i32 GetSpriteWorkerReturnFacing(i32 x)
{
    if (x == 360)
        return 1;
    register i32 boundary asm("r0") = 359;
    asm("" : "+l"(boundary));
    i32 facing = 2;
    if (x <= boundary)
        facing = 3;
    return facing;
}

EC bool TryReturnSpriteWorkerHome(SpriteWorkerEntity * self) NPC_SIDE_CODE(08034940);
EC bool TryReturnSpriteWorkerHome(SpriteWorkerEntity * self)
{
    MovingNpcServices * world = reinterpret_cast<MovingNpcServices *>(self->game_object);
    GameTime time = world->vtable->get_calendar(world)->time;
    u32 hour = time.clock_hour;
    if (hour <= 5 || (hour - 6) * 60 + time.clock_minute >= self->sprite_id + 901u)
    {
        switch (self->location_map)
        {
        case 2:
            break;
        case 37:
        {
            Vec2 point = Farm::smethod_08009ADC();
            self->AEntity::SetLocation(2, point.x, point.y);
            break;
        }
        case 17:
        {
            Vec2 point = Farm::smethod_08009AE8();
            self->AEntity::SetLocation(2, point.x, point.y);
            break;
        }
        }
        ClearNpcEventSlot(self->work_targets, self->sprite_id);
        UnkBarnAnimal2C state;
        BeginSpriteWorkerReturn(self->record, state);
        i32 x = self->x_q16 >> 16;
        i32 facing = GetSpriteWorkerReturnFacing(x);
        SetSpriteWorkerMovementState(self, 1, facing, 0x8000);
        return true;
    }
    return false;
}

EC DirectedMovingNpc * CreateDirectedMovingNpcOne(GameObject * game, ActorLocation * location, u32 direction) NPC_EXTRA_CODE(08037AD0);
EC DirectedMovingNpc * CreateDirectedMovingNpcOne(GameObject * game, ActorLocation * location, u32 direction)
{
    struct Storage
    {
        u8 bytes[0x48];
        Storage(GameObject * game, ActorLocation * location, u32 direction)
        {
            DirectedMovingNpc * self = reinterpret_cast<DirectedMovingNpc *>(this);
            InitializeMovingNpcBase(self, game, location, 684, 11);
            reinterpret_cast<void const **>(self)[5] = vtable_unk_080E72A0;
            self->direction = direction;
            NpcWanderBounds bounds(920, 288, 328, 8);
            SetNpcMovementBounds(self, reinterpret_cast<Box const &>(bounds));
        }
    };
    return reinterpret_cast<DirectedMovingNpc *>(new Storage(game, location, direction));
}
