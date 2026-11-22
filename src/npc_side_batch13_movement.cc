#include "entity_actor.hh"

#define NPC_EXTRA_CODE(address) SECTION(".text.npc_extra." #address)

struct ActivityCollisionProbe;
EC void ProbeActivityDown(ActivityCollisionProbe &) asm("ProbeEntityCollisionDown");
EC void ProbeActivityUp(ActivityCollisionProbe &) asm("ProbeEntityCollisionUp");
EC void ProbeActivityLeft(ActivityCollisionProbe &) asm("ProbeEntityCollisionLeft");
EC void ProbeActivityRight(ActivityCollisionProbe &) asm("ProbeEntityCollisionRight");

struct ActivityCollisionProbe
{
    i32 down, up, left, right;
    u8 down_clear, up_clear, left_clear, right_clear;
    UnkMap map;
    Box bounds;
    i32 step;
    i32 horizontal_lookahead;
    i32 vertical_lookahead;

    ActivityCollisionProbe(UnkMap const & map, Box const & bounds, i32 step)
        : down(step + 1), up(-step - 1), left(-step - 1), right(step + 1), map(map), bounds(bounds), step(step), horizontal_lookahead(0), vertical_lookahead(0)
    {
    }

    ActivityCollisionProbe(UnkMap const & map, Box const & bounds, i32 step, i32 x, i32 y)
        : down(step + 1), up(-step - 1), left(-step - 1), right(step + 1), map(map), bounds(bounds), step(step), horizontal_lookahead(x), vertical_lookahead(y)
    {
    }

    i32 GetDown()
    {
        i32 distance = down;
        if (distance > step)
        {
            ProbeActivityDown(*this);
            return down;
        }
        return distance;
    }
    i32 GetUp()
    {
        i32 distance = up;
        if (distance < -step)
        {
            ProbeActivityUp(*this);
            return up;
        }
        return distance;
    }
    i32 GetLeft()
    {
        i32 distance = left;
        if (distance < -step)
        {
            ProbeActivityLeft(*this);
            return left;
        }
        return distance;
    }
    i32 GetRight()
    {
        i32 distance = right;
        if (distance > step)
        {
            ProbeActivityRight(*this);
            return right;
        }
        return distance;
    }
};

struct ActivityWanderController
{
    GameObject * game;
    STRUCT_PAD(0x04, 0x30);
    u8 animator[0x14];
    u8 refresh_frame;
    STRUCT_PAD(0x45, 0x47);
    u8 frame_latch;
};

struct ActivityWanderState
{
    i32 direction;
    i16 x, y;
    i32 x_q16, y_q16;
    u16 timer;
    i8 moving;
};

EC u32 GetActivityAnimationFlags(void *) asm("func_0805E8F0");
EC i16 RollActivityWanderRandom(u32) asm("RollEntityMovementRandom");
EC u32 RotateEntityMovementFacing(u32);

struct MovingNpcCollisionEntity : AActorEntity
{
    MovingNpcCollisionEntity();
    virtual void ReservedMovementMethod();
    virtual u32 ChooseMovementTimer();
    ActorLocation * saved_location;
    Box movement_bounds;
    u16 animation;
    u16 timer;
    u8 hidden;
};

struct MovingNpcActorLookup
{
    struct Methods
    {
        STRUCT_PAD(0x00, 0x40);
        AEntity * (*get_actor)(GameObject *, u32);
    } * methods;
};

inline AEntity * FindMovingNpcCollisionActor(GameObject * game, u32 actor)
{
    return reinterpret_cast<MovingNpcActorLookup *>(game)->methods->get_actor(game, actor);
}

EC void LimitMovingNpcAgainstBox(ActivityCollisionProbe &, Box const &, u32) asm("ConstrainEntityMovementAgainstRectangle");
EC void ApplyMovingNpcProbe(AActorEntity *, ActivityCollisionProbe *) asm("ApplyMovingNpcCollisionLimits");

EC void UpdateMovingNpcCollisionState(MovingNpcCollisionEntity *) NPC_EXTRA_CODE(0803726C);
EC void UpdateMovingNpcCollisionState(MovingNpcCollisionEntity * input_self)
{
    MovingNpcCollisionEntity * self = input_self;
    asm("" : "+r"(self) : : "r6");
    GameObject * game = self->game_object;
    register u32 map_id asm("r8") = self->location_map;
    asm volatile("" : : "h"(map_id));
    if (game->vfunc_14() == map_id)
    {
        UnkMap map = game->vfunc_34(map_id);
        UnkMapBox bounds(self->GetBox());
        ActivityCollisionProbe probe(map, bounds, 32, 0, 0);
        AEntity * actor = FindMovingNpcCollisionActor(self->game_object, 0);
        if (actor != nullptr && actor->location_map == map_id)
            LimitMovingNpcAgainstBox(probe, actor->GetBox(), 0);
        actor = FindMovingNpcCollisionActor(self->game_object, 74);
        if (actor != nullptr && actor->location_map == map_id)
            LimitMovingNpcAgainstBox(probe, actor->GetBox(), 0);
        Box * limits = &self->movement_bounds;
        if (limits->x2 - limits->x1 > 0 && limits->y2 - limits->y1 > 0)
            LimitMovingNpcAgainstBox(probe, *limits, 0);
        u32 timer = self->timer;
        if (timer != 0)
        {
            i32 x_speed = self->x_speed_q16;
            i32 y_speed = self->y_speed_q16;
            if ((x_speed < 0 && probe.GetLeft() >= 0)
                || (x_speed > 0 && probe.GetRight() <= 0)
                || (y_speed < 0 && probe.GetUp() >= 0)
                || (y_speed > 0 && probe.GetDown() <= 0))
                timer = 0;
            else
                --timer;
        }
        else
            timer = self->ChooseMovementTimer();
        self->timer = timer;
        ApplyMovingNpcProbe(self, &probe);
    }
    if (self->unk_24 != 0)
        --self->unk_24;
    else
        self->unk_24 = self->unk_26;
    if (self->unk_10.Get() != nullptr)
        self->unk_10->vfunc_0C();
}

EC u32 UpdateActivityNpcWandering(ActivityWanderController *, ActivityWanderState *, i32) NPC_EXTRA_CODE(0803A180);
EC u32 UpdateActivityNpcWandering(ActivityWanderController * controller, ActivityWanderState * state, i32 speed)
{
    u32 changed = 0;
    UnkMap map = controller->game->vfunc_34(0);
    Box bounds(state->x, state->y, 16, 16);
    ActivityCollisionProbe probe(map, bounds, 32);
    if (state->moving != 0)
    {
        if (state->moving == 1)
        {
            switch (state->direction)
            {
            case 0:
                if (probe.GetDown() > 1)
                    state->y_q16 += speed;
                else
                    goto stop;
                break;
            case 1:
                if (probe.GetUp() < -1)
                    state->y_q16 -= speed;
                else
                    goto stop;
                break;
            case 2:
                if (probe.GetLeft() < -1)
                    state->x_q16 -= speed;
                else
                    goto stop;
                break;
            case 3:
                if (probe.GetRight() > 1)
                {
                    state->x_q16 += speed;
                    if ((state->x_q16 >> 16) > 580)
                    {
                        state->x_q16 = 580 << 16;
                        goto stop;
                    }
                }
                else
                    goto stop;
                break;
            default:
            stop:
                state->timer = 0;
                break;
            }
        }
    }
    state->x = state->x_q16 >> 16;
    state->y = state->y_q16 >> 16;
    u8 * latch = &controller->frame_latch;
    u8 value;
    if (*latch == 0)
    {
        if (i32(GetActivityAnimationFlags(controller->animator) << 30) < 0)
        {
            latch = &controller->refresh_frame;
            value = 1;
            goto store_latch;
        }
    }
    else
    {
        value = 0;
    store_latch:
        *latch = value;
    }
    --state->timer;
    if (i16(state->timer) < 0)
    {
        register i32 movement_roll asm("r1") = RollActivityWanderRandom(3);
        asm volatile("" : : "l"(movement_roll));
        if (movement_roll == 3)
            state->moving = 0;
        else
        {
            state->moving = 1;
            state->direction = RotateEntityMovementFacing(state->direction);
        }
        state->timer = RollActivityWanderRandom(31) + 16;
        changed = 1;
    }
    return changed;
}
