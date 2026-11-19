// Chicken Festival contestants, combat reactions, movement, and instruction dialogue.
#include "prelude.h"
#include "utility/fixed_str.hh"
#include <new>
#include <stdlib.h>

struct FestivalChicken
{
    u32 action;
    u32 previousAction;
    u32 facing;
    u32 unknown_0C;
    u32 movementDecision;
    u32 reaction;
    i32 ringDistance;
    u8 actionFrames;
    u8 unknown_1D;
    u16 x;
    u16 y;
    u8 affection;
    u8 speed;
    u8 unknown_24[2];
    u8 cheerCooldown;
    u8 unknown_27[5];
    u8 cheerRequested;
    u8 animationDirty;
    u8 unknown_2E[2];
    u8 canCheer;
    u8 unknown_31[15];
};

struct ChickenFestivalCombat
{
    u8 unknown_000[0x6c];
    FestivalChicken chickens[2];
};

struct ChickenFestivalEntrant
{
    FixedStr<12> name;
    u8 unknown_10;
    u8 affection;
    SECTION(".text.g3_080B3C90") ChickenFestivalEntrant();
};

EC char gUnk_081078BC[];

ChickenFestivalEntrant::ChickenFestivalEntrant()
    : name(gUnk_081078BC), unknown_10(0), affection(0)
{
}

struct MinigameWrapper
{
    void * vtable;
    void * state;
};

struct MinigameStateVtable
{
    u32 unknown_00[2];
    void (*destroy)(void *, u32);
};

struct MinigameState
{
    u32 unknown_00;
    MinigameStateVtable * vtable;
};

EC u8 vtable_unk_080E850C[];
EC u8 vtable_unk_080E8528[];
EC void * func_080ADF6C(void *, void *);
EC void * func_080B3CC8(void *, void *);
EC void func_080007EC(void *, u32);
EC void func_08008B6C(void *, u32);
EC void func_080B0934(void *);
EC void func_080B0970(void *);
EC void func_08050D8C(void *, char const *);
EC u16 gUnk_08107ACC[4][6];
EC u8 gUnk_08107AFC[];
EC char gUnk_08107B28[], gUnk_08107B5C[], gUnk_08107C18[];
EC char gUnk_08107CB0[], gUnk_08107CE0[];

struct FeedingEffectTimers
{
    u8 unknown_000[0x85c];
    i32 effects[5];
    u8 unknown_870[0x96];
    u8 timers[5];
    u8 unknown_90B[0x14];
    u8 dirty[5];
};

EC SECTION(".text.g3_080B3948") void AdvanceFeedingEffectTimers(FeedingEffectTimers * state)
{
    for (int i = 0; i < 5; ++i)
    {
        switch (state->effects[i])
        {
        case 1:
        case 2:
            ++state->timers[i];
            if (state->timers[i] == 60)
            {
                state->timers[i] = 0;
                state->effects[i] = 0;
                state->dirty[i] = 1;
            }
            break;
        }
    }
}

EC SECTION(".text.g3_080B3BA8") void ToggleFeedingPause(u8 * state)
{
    func_08008B6C(state + 0x818, 0xb8);
    u32 * paused = (u32 *) (state + 0x83c);
    if (*paused == 0)
    {
        *paused = 1;
        func_080B0934(state);
    }
    else
    {
        *paused = 0;
        func_080B0970(state);
    }
}

EC SECTION(".text.g3_080B3BE4") MinigameWrapper * CreateChickenFeedingWrapper(MinigameWrapper * self, void * settings)
{
    self->vtable = vtable_unk_080E850C;
    self->state = func_080ADF6C(operator new(0x93c), settings);
    return self;
}

EC SECTION(".text.g3_080B3C0C") void DestroyChickenFeedingWrapper(MinigameWrapper * self, u32 flags)
{
    self->vtable = vtable_unk_080E850C;
    MinigameState * state = (MinigameState *) self->state;
    if (state != nullptr)
        state->vtable->destroy(state, 3);
    func_080007EC(self, flags);
}

EC SECTION(".text.g3_080B8DAC") u16 GetFestivalChickenAnimation(u8 * state, u32 animation, u32 direction, u8 round)
{
    if (round == 1)
        round = *(u32 *) (state + 0xb18) + 1;
    return gUnk_08107ACC[round][animation] + gUnk_08107AFC[direction];
}

EC SECTION(".text.g3_080B9428") void TriggerFestivalRivalCheer(ChickenFestivalCombat * state)
{
    state->chickens[1].cheerRequested = 1;
    if (state->chickens[1].action <= 1)
    {
        state->chickens[1].canCheer = 0;
        state->chickens[1].actionFrames = 0;
        state->chickens[1].animationDirty = 1;
        state->chickens[1].action = 5;
        func_08008B6C((u8 *) state + 0x70c, 0xcc);
    }
}

EC SECTION(".text.g3_080B9464") i32 EvaluateChickenArenaPosition(void *, u16 x, u16 y)
{
    i32 dx = (x >> 8) - 120;
    i32 dy = (y >> 8) - 76;
    i32 distance = 0x384000 - (1600 * (dx * dx) + 2304 * (dy * dy));
    if ((u32) (distance - 1) <= 0x2c6fe)
        distance = 0;
    return distance;
}

EC SECTION(".text.g3_080B97FC") void SetFestivalCheerReactions(ChickenFestivalCombat * state, u8 cheeringChicken)
{
    if (cheeringChicken)
    {
        state->chickens[0].reaction = 4;
        state->chickens[1].reaction = 2;
    }
    else
    {
        state->chickens[0].reaction = 2;
        state->chickens[1].reaction = 4;
    }
}

EC SECTION(".text.g3_080B9F0C") void BeginFestivalChickenAction2(ChickenFestivalCombat * state, u8 chicken)
{
    state->chickens[chicken].action = 2;
    u32 facing = ((u32) rand() & 0xffff) >> 14;
    state->chickens[chicken].facing = facing;
}

EC SECTION(".text.g3_080B9F38") void BeginFestivalChickenAction4(ChickenFestivalCombat * state, u8 chicken)
{
    state->chickens[chicken].action = 4;
    u32 facing = ((u32) rand() & 0xffff) >> 14;
    state->chickens[chicken].facing = facing;
}

EC SECTION(".text.g3_080BA1A4") void MoveFestivalChicken(FestivalChicken * chicken)
{
    if (chicken->facing == 0)
        chicken->y -= chicken->speed;
    else if (chicken->facing == 1)
        chicken->x += chicken->speed;
    else if (chicken->facing == 2)
        chicken->y += chicken->speed;
    else if (chicken->facing == 3)
        chicken->x -= chicken->speed;
}

struct FestivalCooldownState
{
    u8 unknown_000[0x6c];
    FestivalChicken chickens[2];
    u8 unknown_0EC[0xa54];
    u32 cheerAnimations[2];
};

EC SECTION(".text.g3_080BA284") void UpdateFestivalCheerCooldowns(FestivalCooldownState * state)
{
    for (int i = 0; i < 2; ++i)
    {
        FestivalChicken & chicken = state->chickens[i];
        if (chicken.cheerRequested)
        {
            state->cheerAnimations[i] = 2;
            ++chicken.cheerCooldown;
            if (chicken.cheerCooldown > 60)
            {
                state->cheerAnimations[i] = 1;
                chicken.cheerRequested = 0;
                chicken.cheerCooldown = 0;
            }
        }
    }
}

EC SECTION(".text.g3_080BA574") void ShowFestivalInstructionPage(u8 * state, u32 page)
{
    switch (page)
    {
    case 1: func_08050D8C(*(void **) (state + 0x718), gUnk_08107B28); break;
    case 2: func_08050D8C(*(void **) (state + 0x718), gUnk_08107B5C); break;
    case 3: func_08050D8C(*(void **) (state + 0x718), gUnk_08107C18); break;
    case 4: func_08050D8C(*(void **) (state + 0x718), gUnk_08107CB0); break;
    case 5: func_08050D8C(*(void **) (state + 0x718), gUnk_08107CE0); break;
    }
}

EC SECTION(".text.g3_080BC898") MinigameWrapper * CreateChickenFestivalWrapper(MinigameWrapper * self, void * entrant)
{
    self->vtable = vtable_unk_080E8528;
    self->state = func_080B3CC8(operator new(0xb78), entrant);
    return self;
}

EC SECTION(".text.g3_080BC8C0") void DestroyChickenFestivalWrapper(MinigameWrapper * self, u32 flags)
{
    self->vtable = vtable_unk_080E8528;
    MinigameState * state = (MinigameState *) self->state;
    if (state != nullptr)
        state->vtable->destroy(state, 3);
    func_080007EC(self, flags);
}

struct FestivalMatchState
{
    u8 unknown_000[0x6c];
    FestivalChicken chickens[2];
    u8 unknown_0EC[0xa28];
    u32 phase;
    u8 unknown_B18[0x22];
    u8 rivalCheerTimer;
    u8 unknown_B3B[5];
    u32 reactions[2];
    u8 unknown_B48[8];
    u32 resultPending;
    u8 unknown_B54[6];
    u8 rivalCheerInterval;
};

EC u32 func_080088D4(void *);
EC void func_080B92B4(void *);

EC SECTION(".text.g3_080BA6B8") void HandleFestivalCheerInput(FestivalMatchState * state)
{
    u32 keys = func_080088D4(state);
    if (state->chickens[0].canCheer == 1 && (keys & 1))
    {
        state->chickens[0].cheerRequested = 1;
        if (state->chickens[0].action <= 1)
        {
            state->chickens[0].canCheer = 0;
            state->chickens[0].actionFrames = 0;
            state->chickens[0].animationDirty = 1;
            state->chickens[0].action = 5;
            func_08008B6C((u8 *) state + 0x70c, 0xcc);
        }
    }
    if (state->chickens[1].canCheer == 1)
    {
        ++state->rivalCheerTimer;
        if (state->rivalCheerTimer > state->rivalCheerInterval - 1)
        {
            state->rivalCheerTimer = 0;
            func_080B92B4(state);
        }
    }
}

EC SECTION(".text.g3_080BA734") void CheckFestivalRingOut(FestivalMatchState * state)
{
    if (state->chickens[0].ringDistance < 0)
    {
        if ((state->chickens[0].y >> 8) <= 71)
            ((u8 *) &state->chickens[0])[0x29] = 2;
        state->chickens[0].y += 0x300;
        state->phase = 3;
        state->reactions[0] = 0;
        state->reactions[1] = 3;
        state->resultPending = 1;
    }
    else if (state->chickens[1].ringDistance < 0)
    {
        if ((state->chickens[0].y >> 8) <= 71)
            ((u8 *) &state->chickens[1])[0x29] = 2;
        state->chickens[1].y += 0x300;
        state->phase = 3;
        state->reactions[1] = 1;
        state->reactions[0] = 3;
        state->resultPending = 1;
    }
}

struct FestivalSprite
{
    u8 unknown_00[0x10];
    i16 playbackRate;
};

struct FestivalPauseState
{
    u8 unknown_000[0x7a4];
    FestivalSprite * chickens[2];
    u8 unknown_7AC[0xd0];
    FestivalSprite * shadows[2];
    u8 unknown_884[0x98];
    FestivalSprite * reactions[2];
    u8 unknown_924[0x20d];
    u8 paused;
};

EC u32 func_080088CC(void *);

EC SECTION(".text.g3_080BA608") void ToggleFestivalPause(FestivalPauseState * state)
{
    if (func_080088CC(state) & 8)
    {
        func_08008B6C((u8 *) state + 0x714, 0xb8);
        if (state->paused)
        {
            state->paused = 0;
            state->chickens[0]->playbackRate = 0x100;
            state->chickens[1]->playbackRate = 0x100;
            state->shadows[0]->playbackRate = 0x100;
            state->shadows[1]->playbackRate = 0x100;
            state->reactions[0]->playbackRate = 0x100;
            state->reactions[1]->playbackRate = 0x100;
        }
        else
        {
            state->chickens[0]->playbackRate = 0;
            state->chickens[1]->playbackRate = 0;
            state->shadows[0]->playbackRate = 0;
            state->shadows[1]->playbackRate = 0;
            state->reactions[0]->playbackRate = 0;
            state->reactions[1]->playbackRate = 0;
            state->paused = 1;
        }
    }
}
