// Chicken Festival contestants, combat reactions, movement, and instruction dialogue.
#include "prelude.h"
#include "scene.hh"
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
    u8 unknown_27[3];
    u16 displayY;
    u8 cheerRequested;
    u8 animationDirty;
    u8 unknown_2E;
    u8 movementBias;
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
EC void Scene_DestroyBase(void *, u32);
EC void AudioPlayer_StartSong(void *, u32);
EC void func_080B0934(void *);
EC void func_080B0970(void *);
EC void SetMessageWindowText(void *, char const *);
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

struct FestivalInstructionChickens
{
    u8 unknown_000[0x6c];
    FestivalChicken chickens[2];
    u8 unknown_0EC[0xa72];
    u8 speed;
};

EC void func_080B8DF4(void *, u8);

EC SECTION(".text.g3_080B5788") void ResetFestivalInstructionChickens(FestivalInstructionChickens * state)
{
    for (i32 i = 0; i < 2; ++i)
    {
        state->chickens[i].x = 0x7800;
        state->chickens[i].y = (i * 24 + 58) << 8;
        state->chickens[i].speed = state->speed;
        state->chickens[i].unknown_31[0] = 1;
        state->chickens[i].unknown_27[2] = 1;
        state->chickens[i].action = 2;
        state->chickens[i].previousAction = 2;
        state->chickens[i].unknown_31[14] = 0;
    }
    state->chickens[0].facing = 3;
    state->chickens[0].unknown_0C = 3;
    state->chickens[1].facing = 1;
    state->chickens[1].unknown_0C = 1;
    func_080B8DF4(state, 0);
    func_080B8DF4(state, 1);
}

EC SECTION(".text.g3_080B3BA8") void ToggleFeedingPause(u8 * state)
{
    AudioPlayer_StartSong(state + 0x818, 0xb8);
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
    Scene_DestroyBase(self, flags);
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
        AudioPlayer_StartSong((u8 *) state + 0x70c, 0xcc);
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
    case 1: SetMessageWindowText(*(void **) (state + 0x718), gUnk_08107B28); break;
    case 2: SetMessageWindowText(*(void **) (state + 0x718), gUnk_08107B5C); break;
    case 3: SetMessageWindowText(*(void **) (state + 0x718), gUnk_08107C18); break;
    case 4: SetMessageWindowText(*(void **) (state + 0x718), gUnk_08107CB0); break;
    case 5: SetMessageWindowText(*(void **) (state + 0x718), gUnk_08107CE0); break;
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
    Scene_DestroyBase(self, flags);
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

EC u32 HardwareContext_GetRepeatedKeys(void *);
struct FestivalCheerAI;
EC void TryFestivalRivalCheer(FestivalCheerAI *);

EC SECTION(".text.g3_080BA6B8") void HandleFestivalCheerInput(FestivalMatchState * state)
{
    u32 keys = HardwareContext_GetRepeatedKeys(state);
    if (state->chickens[0].canCheer == 1 && (keys & 1))
    {
        state->chickens[0].cheerRequested = 1;
        if (state->chickens[0].action <= 1)
        {
            state->chickens[0].canCheer = 0;
            state->chickens[0].actionFrames = 0;
            state->chickens[0].animationDirty = 1;
            state->chickens[0].action = 5;
            AudioPlayer_StartSong((u8 *) state + 0x70c, 0xcc);
        }
    }
    if (state->chickens[1].canCheer == 1)
    {
        ++state->rivalCheerTimer;
        if (state->rivalCheerTimer > state->rivalCheerInterval - 1)
        {
            state->rivalCheerTimer = 0;
            TryFestivalRivalCheer((FestivalCheerAI *) state);
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

EC u32 HardwareContext_GetPressedKeys(void *);

EC SECTION(".text.g3_080BA608") void ToggleFestivalPause(FestivalPauseState * state)
{
    if (HardwareContext_GetPressedKeys(state) & 8)
    {
        AudioPlayer_StartSong((u8 *) state + 0x714, 0xb8);
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

struct FestivalSceneResult
{
    struct Transfer
    {
        FestivalSceneResult * source;
        AUnk_0800080C * value;
        Transfer(FestivalSceneResult * owner, AUnk_0800080C * ptr) : source(owner), value(ptr) {}
    };

    mutable AUnk_0800080C * inner;

    explicit FestivalSceneResult(AUnk_0800080C * value = nullptr) : inner(value) {}
    FestivalSceneResult(FestivalSceneResult const & other) : inner(other.inner) { other.inner = nullptr; }
    FestivalSceneResult(Transfer const & transfer)
    {
        AUnk_0800080C * value = transfer.value;
        transfer.source->inner = nullptr;
        inner = value;
    }
    ~FestivalSceneResult() { delete inner; }

    Transfer Move()
    {
        return Transfer(this, inner);
    }
};

EC FestivalSceneResult func_080AE4E4(void *);
EC FestivalSceneResult func_080B44EC(void *);

EC SECTION(".text.g3_080B3C3C") FestivalSceneResult RunChickenFeedingWrapper(MinigameWrapper * self)
{
    FestivalSceneResult result = func_080AE4E4(self->state);
    return result.Move();
}

EC SECTION(".text.g3_080BC8FC") FestivalSceneResult RunChickenFestivalWrapper(MinigameWrapper * self)
{
    FestivalSceneResult result = func_080B44EC(self->state);
    return result.Move();
}

EC SECTION(".text.g3_080B9828") void ResetFestivalCheerReaction(u8 * state, u8 chicken)
{
    if (chicken)
        *(u32 *) (state + 0xc0) = 1;
    else
        *(u32 *) (state + 0x80) = 1;
}

EC u16 ArcTan2(i32, i32);

EC SECTION(".text.g3_080B96AC") u8 IsFestivalRivalInCheerCone(void *, u16 x, u16 y, i32 facing, u16 rivalX, u16 rivalY, u8 radius, u16 halfAngle)
{
    i32 dx = (rivalX >> 8) - (x >> 8);
    i32 dy = (y >> 8) - (rivalY >> 8);
    if (dx * dx + dy * dy > radius * radius)
        return 0;
    u16 angle = ArcTan2(dx, dy);
    if (facing == 1)
    {
        if (angle < halfAngle || angle > 0x10000 - halfAngle)
            return 1;
    }
    else
    {
        i32 direction = facing;
        if (direction == 3)
            direction = 1;
        ++direction;
        direction <<= 14;
        if (angle < halfAngle + direction && angle > direction - halfAngle)
            return 1;
    }
    return 0;
}

EC SECTION(".text.g3_080BC254") u8 * CopyFestivalDialogueSpan(u8 * state, i32 characters, i32 start, u8 const * text)
{
    int i = 0;
    int bytes = characters * 2;
    if (i < bytes)
    {
        u8 * destination = state + 0xb35;
        u8 const * source = (u8 const *) (start * 2 + (u32) text);
        do
        {
            destination[i] = *source++;
            ++i;
        }
        while (i < bytes);
    }
    return state + 0xb35;
}

EC void UpdateFestivalChickenAction2(FestivalChicken *);
EC void UpdateFestivalChickenAction4(FestivalChicken *);
EC void UpdateFestivalChickenIdle(FestivalChicken *);
EC void UpdateFestivalChickenAction3(FestivalChicken *);
EC void func_080BA204(FestivalChicken *);

EC SECTION(".text.g3_080B9AC0") void UpdateFestivalChickenMovement(ChickenFestivalCombat * state)
{
    for (u32 i = 0; i < 2; ++i)
    {
        switch (state->chickens[i].action)
        {
        case 1: MoveFestivalChicken(&state->chickens[i]); break;
        case 2: UpdateFestivalChickenAction2(&state->chickens[i]); break;
        case 4: UpdateFestivalChickenAction4(&state->chickens[i]); break;
        case 0: UpdateFestivalChickenIdle(&state->chickens[i]); break;
        case 3: UpdateFestivalChickenAction3(&state->chickens[i]); break;
        case 5: func_080BA204(&state->chickens[i]); break;
        }
    }
}


EC u16 gUnk_08107D3C[], gUnk_08107D46[4][4], gUnk_08107D66[];


EC void * HardwareContext_GetDisplayRegisters(void *);
EC void * HardwareContext_GetState(void *);
EC void * HardwareContext_GetOamBuffer(void *);
EC void * HardwareContext_GetFrameTasks(void *);
EC void * HardwareContext_GetVBlankTasks(void *);
EC i32 UpdateMessageWindow(void *, void *, void *, void *, void *, void *, void *);

struct FestivalSceneFlow
{
    u8 unknown_000[0xb14];
    i32 phase;
    u32 round;
    u8 unknown_B1C[0x15];
    u8 introComplete;
    u8 finished;
    u8 unknown_B33[0xa];
    u8 firstParticipant;
    u8 secondParticipant;
    u8 thirdParticipant;
    u8 unknown_B40[0x18];
    u8 initialRound;
};

EC SECTION(".text.g3_080B45F0") void ResetChickenFestivalScene(FestivalSceneFlow * flow)
{
    flow->introComplete = 0;
    flow->finished = 0;
    flow->round = flow->initialRound;
    flow->phase = 0;
    flow->firstParticipant = 22;
    flow->secondParticipant = 22;
    flow->thirdParticipant = 22;
    u8 * state = (u8 *) flow;
    void * window = *(void **) (state + 0x718);
    void * input = HardwareContext_GetState(state);
    void * sprites = HardwareContext_GetOamBuffer(state);
    void * registers = HardwareContext_GetDisplayRegisters(state);
    void * graphics = HardwareContext_GetFrameTasks(state);
    void * palettes = HardwareContext_GetVBlankTasks(state);
    void * dialogueState = state + 0x708;
    UpdateMessageWindow(window, input, sprites, registers, graphics, palettes, dialogueState);
}


struct SpriteHarvestingResource
{
    u32 unknown_00;
    void * allocation;
};

struct SpriteHarvestingResources
{
    u8 unknown_000[0x5b0];
    SpriteHarvestingResource groups[9];
};

EC void func_08007C28(SpriteHarvestingResource *, void *);
EC void * func_08007B54(SpriteHarvestingResource *, u32);

static inline void ResizeHarvestingResource(SpriteHarvestingResource & resource, u32 count)
{
    func_08007C28(&resource, resource.allocation);
    resource.allocation = func_08007B54(&resource, count);
}

EC SECTION(".text.g3_080BCEC8") void AllocateSpriteHarvestingResources(SpriteHarvestingResources * state)
{
    ResizeHarvestingResource(state->groups[0], 3);
    ResizeHarvestingResource(state->groups[1], 6);
    ResizeHarvestingResource(state->groups[2], 2);
    ResizeHarvestingResource(state->groups[3], 5);
    ResizeHarvestingResource(state->groups[4], 5);
    ResizeHarvestingResource(state->groups[5], 2);
    ResizeHarvestingResource(state->groups[8], 4);
    ResizeHarvestingResource(state->groups[7], 2);
    ResizeHarvestingResource(state->groups[6], 6);
}

EC char gUnk_081078BD[20][13];
EC char * strcpy(char *, char const *);

struct FestivalNames
{
    u8 unknown_000[0x9e];
    char player[14];
    u8 unknown_0AC[0x32];
    char rival[14];
    u8 unknown_0EC[0xa2c];
    i32 round;
    char entrant[13];
    u8 unknown_B29[0x14];
    u8 opponents[3];
};

EC SECTION(".text.g3_080B757C") void ChooseFestivalOpponentName(FestivalNames * state)
{
    i32 opponent = 0;
    for (int i = 0; i < 14; ++i)
    {
        state->player[i] = 0;
        state->rival[i] = 0;
    }
    strcpy(state->player, state->entrant);
    switch (state->round)
    {
    case 0:
    {
        u32 range = 6;
        opponent = (range * (rand() & 0xffff)) >> 16;
        break;
    }
    case 1:
    {
        u32 range = 7;
        opponent = (range * (rand() & 0xffff)) >> 16;
        opponent += 6;
        break;
    }
    case 2:
    {
        u32 range = 7;
        opponent = (range * (rand() & 0xffff)) >> 16;
        opponent += 13;
        break;
    }
    }
    state->opponents[state->round] = opponent;
    strcpy(state->rival, gUnk_081078BD[opponent]);
}

EC SECTION(".text.g3_080B9F64") u8 GetFestivalRivalRelativeDirection(ChickenFestivalCombat * state, u8 chicken, u8 rival)
{
    u8 direction = 0;
    i32 dx = state->chickens[rival].x - state->chickens[chicken].x;
    i32 dy = state->chickens[chicken].y - state->chickens[rival].y;
    if ((dx > 0 && dy > 0 && dy > dx)
        || (dx < 0 && dy > 0 && dy > -dx)
        || (dy > 0 && dx == 0))
        direction = 4 - state->chickens[chicken].facing;
    else if ((dx > 0 && dy > 0 && dx > dy)
        || (dx > 0 && dy < 0 && dx > -dy)
        || (dx > 0 && dy == 0))
        direction = 5 - state->chickens[chicken].facing;
    else if ((dx > 0 && dy < 0 && dx < -dy)
        || (dx < 0 && dy < 0 && dx > dy)
        || (dy < 0 && dx == 0))
        direction = 6 - state->chickens[chicken].facing;
    else if ((dx < 0 && dy < 0 && dx < dy)
        || (dx < 0 && dy > 0 && dx < -dy)
        || (dx < 0 && dy == 0))
        direction = 7 - state->chickens[chicken].facing;
    else if (dx > 0 && dy > 0 && dx == dy)
        direction = 12 - state->chickens[chicken].facing;
    else if (dx > 0 && dy < 0 && dx == -dy)
        direction = 13 - state->chickens[chicken].facing;
    else if (dx < 0 && dy < 0 && dx == dy)
        direction = 14 - state->chickens[chicken].facing;
    else if (dx < 0 && dy > 0 && dx == -dy)
        direction = 15 - state->chickens[chicken].facing;
    if ((u8) (direction - 4) <= 7)
        direction -= 4;
    else if (direction > 11)
        direction -= 8;
    return direction;
}

struct FestivalCheerContest
{
    u8 unknown_000[0x6c];
    FestivalChicken chickens[2];
    u8 unknown_0EC[0x830];
    void * reactions[2];
};

EC void SetSpriteAnimatorClip(void *, u16);

EC SECTION(".text.g3_080B9734") void ResolveFestivalCheerClash(FestivalCheerContest * state)
{
    u32 range = 100;
    u32 roll = (range * (rand() & 0xffff)) >> 16;
    if (state->chickens[0].affection > state->chickens[1].affection)
    {
        if (roll <= 89)
        {
            state->chickens[0].reaction = 3;
            SetSpriteAnimatorClip(state->reactions[1], 2);
            state->chickens[1].unknown_31[14] = 1;
            state->chickens[1].reaction = 5;
        }
        else
        {
            state->chickens[0].reaction = 5;
            SetSpriteAnimatorClip(state->reactions[0], 2);
            state->chickens[0].unknown_31[14] = 1;
            state->chickens[1].reaction = 3;
        }
    }
    else
    {
        if (roll > 89)
        {
            state->chickens[0].reaction = 3;
            SetSpriteAnimatorClip(state->reactions[1], 2);
            state->chickens[1].unknown_31[14] = 1;
            state->chickens[1].reaction = 5;
        }
        else
        {
            state->chickens[0].reaction = 5;
            SetSpriteAnimatorClip(state->reactions[0], 2);
            state->chickens[0].unknown_31[14] = 1;
            state->chickens[1].reaction = 3;
        }
    }
    for (int i = 0; i < 2; ++i)
    {
        if (state->chickens[i].reaction == 5)
            state->chickens[i].movementBias = 0;
        else
            state->chickens[i].movementBias = 1;
    }
}

struct FestivalCheerAI
{
    u8 unknown_000[0x6c];
    FestivalChicken chickens[2];
    u8 unknown_0EC[0xa71];
    u8 radius;
    u8 unknown_B5E[0xc];
    u8 cheerThresholds[8];
    u16 halfAngle;
};

EC SECTION(".text.g3_080B92B4") void TryFestivalRivalCheer(FestivalCheerAI * state)
{
    if (state->chickens[1].action <= 1)
    {
        if (IsFestivalRivalInCheerCone(state, state->chickens[1].x, state->chickens[1].y,
                state->chickens[1].facing, state->chickens[0].x, state->chickens[0].y, state->radius, state->halfAngle))
        {
            if (state->chickens[1].affection > 80)
            {
                if (((u32) (rand() & 0xffff) >> 9) < state->cheerThresholds[0])
                    TriggerFestivalRivalCheer((ChickenFestivalCombat *) state);
            }
            else
            {
                if (((u32) (rand() & 0xffff) >> 9) > state->cheerThresholds[1])
                    TriggerFestivalRivalCheer((ChickenFestivalCombat *) state);
            }
        }
        else if (GetFestivalRivalRelativeDirection((ChickenFestivalCombat *) state, 1, 0) == 0)
        {
            if (state->chickens[1].affection > 80)
            {
                if (((u32) (rand() & 0xffff) >> 9) < state->cheerThresholds[2])
                    TriggerFestivalRivalCheer((ChickenFestivalCombat *) state);
            }
            else
            {
                if (((u32) (rand() & 0xffff) >> 9) > state->cheerThresholds[3])
                    TriggerFestivalRivalCheer((ChickenFestivalCombat *) state);
            }
        }
        else
        {
            if (state->chickens[1].affection > 80)
            {
                if (((u32) (rand() & 0xffff) >> 9) > state->cheerThresholds[4])
                    TriggerFestivalRivalCheer((ChickenFestivalCombat *) state);
            }
            else
            {
                if (((u32) (rand() & 0xffff) >> 9) < state->cheerThresholds[5])
                    TriggerFestivalRivalCheer((ChickenFestivalCombat *) state);
            }
        }
    }
    else
    {
        if (state->chickens[1].affection > 80)
        {
            if (((u32) (rand() & 0xffff) >> 9) < state->cheerThresholds[6])
                TriggerFestivalRivalCheer((ChickenFestivalCombat *) state);
        }
        else
        {
            if (((u32) (rand() & 0xffff) >> 9) > state->cheerThresholds[7])
                TriggerFestivalRivalCheer((ChickenFestivalCombat *) state);
        }
    }
}

struct FestivalCheerResolution
{
    u8 unknown_000[0x6c];
    FestivalChicken chickens[2];
    u8 unknown_0EC[0x830];
    void * reactions[2];
    u8 unknown_924[0x238];
    u8 windupFrames;
    u8 radius;
    u8 unknown_B5E[0x14];
    u16 halfAngle;
};

EC SECTION(".text.g3_080B94A8") void UpdateFestivalCheerWindup(FestivalCheerResolution * state, u8 chicken, u8 rival)
{
    ++state->chickens[chicken].actionFrames;
    ++state->chickens[chicken].unknown_1D;
    if (state->chickens[chicken].unknown_1D < state->windupFrames)
    {
        if (state->chickens[rival].action == 5)
        {
            if (IsFestivalRivalInCheerCone(state, state->chickens[rival].x, state->chickens[rival].y,
                    state->chickens[rival].facing, state->chickens[chicken].x, state->chickens[chicken].y,
                    state->radius, state->halfAngle))
            {
                if (IsFestivalRivalInCheerCone(state, state->chickens[chicken].x, state->chickens[chicken].y,
                        state->chickens[chicken].facing, state->chickens[rival].x, state->chickens[rival].y,
                        state->radius, state->halfAngle))
                {
                    state->chickens[chicken].animationDirty = 0;
                    state->chickens[chicken].unknown_2E = 1;
                    state->chickens[rival].animationDirty = 0;
                    state->chickens[rival].unknown_2E = 1;
                    state->chickens[rival].canCheer = 0;
                    state->chickens[chicken].unknown_24[0] = 0;
                    state->chickens[rival].unknown_24[0] = 0;
                    ResolveFestivalCheerClash((FestivalCheerContest *) state);
                    return;
                }
            }
        }
    }
    else
    {
        if (IsFestivalRivalInCheerCone(state, state->chickens[chicken].x, state->chickens[chicken].y,
                state->chickens[chicken].facing, state->chickens[rival].x, state->chickens[rival].y,
                state->radius, state->halfAngle))
        {
            SetFestivalCheerReactions((ChickenFestivalCombat *) state, chicken);
            SetSpriteAnimatorClip(*(void **) ((u8 *) state + rival * 4 + 0x91c), 2);
            state->chickens[rival].unknown_31[14] = 1;
            state->chickens[chicken].movementBias = 1;
            state->chickens[chicken].animationDirty = 0;
            state->chickens[chicken].unknown_2E = 1;
            state->chickens[rival].animationDirty = 0;
            state->chickens[rival].unknown_2E = 1;
            state->chickens[rival].canCheer = 0;
            state->chickens[rival].unknown_24[0] = 0;
            return;
        }
    }
    if (state->chickens[chicken].unknown_1D > 1)
        state->chickens[chicken].unknown_1D = 0;
    if (state->chickens[chicken].actionFrames > 59)
    {
        state->chickens[chicken].actionFrames = 0;
        ResetFestivalCheerReaction((u8 *) state, chicken);
        state->chickens[chicken].movementBias = 0;
        state->chickens[chicken].animationDirty = 0;
        state->chickens[chicken].unknown_2E = 1;
        AudioPlayer_StartSong((u8 *) state + 0x710, 0xb5);
    }
}

struct FestivalReactionSettings
{
    u8 unknown_000[0x6c];
    FestivalChicken chickens[2];
    u8 unknown_0EC[0xa6d];
    u8 cheerCooldown;
    u8 unknown_B5A[4];
    u8 defaultSpeed;
    u8 unknown_B5F;
    u16 reactionSpeeds[5];
    u8 unknown_B6A[0xa];
    u8 reactionDurations[4];
};

static inline void FinishFestivalChickenReaction(FestivalReactionSettings * state, u8 chicken)
{
    state->chickens[chicken].unknown_24[0] = 0;
    state->chickens[chicken].reaction = 0;
    state->chickens[chicken].canCheer = 1;
    state->chickens[chicken].unknown_2E = 0;
    state->chickens[chicken].speed = state->defaultSpeed;
    state->chickens[chicken].unknown_24[1] = state->cheerCooldown + 1;
}

EC SECTION(".text.g3_080B9844") void UpdateFestivalCheerReaction(FestivalReactionSettings * state, u8 chicken)
{
    u8 rival = chicken + 1;
    if (rival > 1)
        rival = 0;
    if ((u8) (state->chickens[chicken].actionFrames - 1) <= 58
        && state->chickens[chicken].action == 5)
    {
        if (++state->chickens[chicken].actionFrames > 59)
        {
            state->chickens[chicken].actionFrames = 0;
            AudioPlayer_StartSong((u8 *) state + 0x710, 0xb5);
        }
        return;
    }
    switch (state->chickens[chicken].reaction)
    {
    case 0:
        state->chickens[chicken].canCheer = 1;
        state->chickens[chicken].unknown_2E = 0;
        break;
    case 1:
        if (state->chickens[chicken].unknown_24[0] < state->reactionDurations[1])
        {
            state->chickens[chicken].speed = state->reactionSpeeds[0];
            state->chickens[chicken].action = 1;
        }
        else
            FinishFestivalChickenReaction(state, chicken);
        break;
    case 2:
        if (state->chickens[chicken].unknown_24[0] < state->reactionDurations[0] - 60)
        {
            state->chickens[chicken].speed = state->reactionSpeeds[1];
            state->chickens[chicken].action = 1;
        }
        else
            FinishFestivalChickenReaction(state, chicken);
        break;
    case 3:
        if (state->chickens[chicken].unknown_24[0] <= 59)
        {
            state->chickens[chicken].action = 5;
            state->chickens[chicken].actionFrames = 1;
            state->chickens[chicken].unknown_24[0] = 60;
        }
        else if (state->chickens[chicken].unknown_24[0] < state->reactionDurations[2])
        {
            state->chickens[chicken].speed = state->reactionSpeeds[2];
            state->chickens[chicken].action = 1;
        }
        else
            FinishFestivalChickenReaction(state, chicken);
        break;
    case 4:
        if (state->chickens[chicken].unknown_24[0] <= 59)
        {
            if (state->chickens[chicken].unknown_24[0] == 0)
            {
                if (state->chickens[chicken].action <= 1)
                    state->chickens[chicken].speed = state->reactionSpeeds[3];
                if (state->chickens[chicken].action == 2 || state->chickens[chicken].action == 4)
                    state->chickens[chicken].speed = state->reactionSpeeds[2];
            }
            state->chickens[chicken].action = 3;
        }
        else if (state->chickens[chicken].unknown_24[0] < state->reactionDurations[0])
        {
            if (state->chickens[chicken].unknown_24[0] == 60)
                state->chickens[chicken].facing = state->chickens[rival].facing;
            state->chickens[chicken].action = 1;
        }
        else
        {
            state->chickens[chicken].unknown_31[14] = 0;
            FinishFestivalChickenReaction(state, chicken);
        }
        break;
    case 5:
        if (state->chickens[chicken].unknown_24[0] <= 29)
            state->chickens[chicken].action = 3;
        else if (state->chickens[chicken].unknown_24[0] < state->reactionDurations[3] - 60)
        {
            if (state->chickens[chicken].unknown_24[0] == 30)
                state->chickens[chicken].facing = state->chickens[rival].facing;
            state->chickens[chicken].speed = state->reactionSpeeds[4];
            state->chickens[chicken].action = 1;
        }
        else
        {
            state->chickens[chicken].unknown_31[14] = 0;
            FinishFestivalChickenReaction(state, chicken);
        }
        break;
    }
    ++state->chickens[chicken].unknown_24[0];
}


struct FestivalCheerDemonstration
{
    u8 unknown_000[0x6c];
    FestivalChicken chickens[2];
    u8 unknown_0EC[0x62c];
    void * message;
    u8 unknown_71C[0x200];
    void * reactions[2];
    u8 unknown_924[0x217];
    u8 frames;
    u8 step;
    u8 unknown_B3D[3];
    u32 playerAnimation;
    u32 rivalAnimation;
    u8 unknown_B48[0x1c];
    u16 reactionSpeed;
};

EC void AudioPlayer_StartSongIfStoppedOrChanged(void *, u32);
EC void EnableMessageWindowInstantText(void *);

EC SECTION(".text.g3_080B5318") void UpdateFestivalCheerDemonstration(FestivalCheerDemonstration * state)
{
    if (state->step > 6)
        return;
    switch (state->step)
    {
    case 1:
        state->playerAnimation = 0;
        state->rivalAnimation = 1;
        break;
    case 2:
        state->chickens[0].action = 0;
        state->chickens[0].facing = 2;
        state->frames = 0;
        state->chickens[0].actionFrames = 0;
        state->playerAnimation = 1;
        state->rivalAnimation = 1;
        AudioPlayer_StartSongIfStoppedOrChanged((u8 *) state + 0x70c, 0xcc);
        break;
    case 3:
        if (state->frames == 1)
        {
            AudioPlayer_StartSong((u8 *) state + 0x70c, 0xa6);
            SetSpriteAnimatorClip(state->reactions[1], 2);
            state->chickens[1].unknown_31[14] = 1;
        }
        if (state->frames <= 59)
        {
            state->playerAnimation = 2;
            ++state->chickens[0].actionFrames;
            state->chickens[0].action = 5;
            state->chickens[1].action = 3;
        }
        else if (state->frames <= 179)
        {
            state->playerAnimation = 1;
            state->chickens[0].action = 1;
            state->chickens[1].action = 1;
            state->chickens[1].speed = state->reactionSpeed;
            state->chickens[1].facing = 2;
        }
        else
        {
            state->chickens[1].unknown_31[14] = 0;
            state->chickens[0].action = 0;
            state->chickens[1].action = 0;
            state->frames = 180;
        }
        break;
    case 4:
        AudioPlayer_StartSongIfStoppedOrChanged((u8 *) state + 0x70c, 0xcc);
        state->frames = 0;
        state->chickens[0].actionFrames = 0;
        state->chickens[1].action = 2;
        state->chickens[1].facing = 1;
        state->chickens[0].facing = 3;
        state->chickens[0].ringDistance = EvaluateChickenArenaPosition(state,
            state->chickens[0].x, state->chickens[0].y);
        if (state->chickens[0].ringDistance > 0)
            state->chickens[0].action = 1;
        else if (state->chickens[0].ringDistance == 0)
        {
            state->chickens[0].action = 0;
            EnableMessageWindowInstantText(state->message);
        }
        break;
    case 5:
        state->chickens[1].action = 2;
        state->chickens[1].facing = 1;
        if (state->frames == 1)
            AudioPlayer_StartSong((u8 *) state + 0x70c, 0xa6);
        if (state->frames <= 59)
        {
            state->playerAnimation = 2;
            ++state->chickens[0].actionFrames;
            state->chickens[0].action = 5;
        }
        else if (state->frames <= 179)
        {
            state->chickens[0].action = 1;
            state->playerAnimation = 1;
        }
        else
            state->frames = 180;
        if (state->chickens[0].ringDistance >= 0)
        {
            state->chickens[0].ringDistance = EvaluateChickenArenaPosition(state,
                state->chickens[0].x, state->chickens[0].y);
            if (state->chickens[0].ringDistance < 0)
            {
                EnableMessageWindowInstantText(state->message);
                state->frames = 180;
                state->chickens[0].action = 0;
                if ((state->chickens[0].y >> 11) <= 9)
                    state->chickens[0].unknown_27[2] = 2;
                state->chickens[0].y += 0x300;
            }
        }
        break;
    case 6:
        state->chickens[0].actionFrames = 0;
        state->frames = 0;
        state->chickens[0].unknown_27[2] = 1;
        for (u32 i = 0; i < 2; ++i)
        {
            state->chickens[i].action = 2;
            state->chickens[i].x = 0x7800;
            state->chickens[i].y = (i * 24 + 58) << 8;
        }
        state->chickens[0].facing = 2;
        state->chickens[1].facing = 0;
        state->playerAnimation = 0;
        break;
    }
    ++state->frames;
}

struct FestivalInstructionFlow
{
    u8 unknown_000[0x6c];
    FestivalChicken chickens[2];
    u8 unknown_0EC[0x61c];
    u8 dialogueState[0x10];
    void * message;
    u8 unknown_71C[0x3f8];
    i32 phase;
    i32 round;
    u8 unknown_B1C[0x17];
    u8 waitingForChoice;
    u8 demonstrationComplete;
    u8 unknown_B35[6];
    u8 frames;
    u8 step;
    u8 unknown_B3D[0x13];
    u32 showPrompt;
};

EC i32 GetMessageWindowPhase(void *);
EC void OpenMessageWindow(void *);
EC void CloseMessageWindow(void *);
EC void DisableMessageWindowInstantText(void *);
EC i32 GetMessageWindowSelection(void *);
EC void ConfigureMessageWindowPromptChoices(void *, char const *, char const *, char const *, u32, u32, u32);
EC char gUnk_08107B00[], gUnk_08107B14[], gUnk_08107B1C[];
EC char gUnk_08107A68[], gUnk_08107A80[], gUnk_08107A9C[], gUnk_08107AB4[];

EC SECTION(".text.g3_080BA2C4") i32 RunFestivalInstructionDialogue(FestivalInstructionFlow * state, i32 timer)
{
    i32 mode = GetMessageWindowPhase(state->message);
    void * window = state->message;
    void * input = HardwareContext_GetState(state);
    void * sprites = HardwareContext_GetOamBuffer(state);
    void * registers = HardwareContext_GetDisplayRegisters(state);
    void * graphics = HardwareContext_GetFrameTasks(state);
    void * palettes = HardwareContext_GetVBlankTasks(state);
    void * dialogueState = state->dialogueState;
    switch (UpdateMessageWindow(window, input, sprites, registers, graphics, palettes, dialogueState))
    {
    case 0:
        if (state->demonstrationComplete)
            state->phase = 2;
        else if (state->step != 3 || state->frames > 179)
            OpenMessageWindow(state->message);
        break;
    case 3:
        switch (mode)
        {
        default:
            CloseMessageWindow(state->message);
            break;
        case 1:
            if (state->step == 4 && state->chickens[0].ringDistance > 0)
                DisableMessageWindowInstantText(state->message);
            if (state->step == 5 && state->chickens[0].ringDistance == 0)
                DisableMessageWindowInstantText(state->message);
            ShowFestivalInstructionPage((u8 *) state, state->step);
            break;
        case 4:
        case 5:
            if (++state->step == 6)
                ConfigureMessageWindowPromptChoices(state->message, gUnk_08107B00, gUnk_08107B14, gUnk_08107B1C, 0, 0, 0);
            break;
        case 6:
            switch (GetMessageWindowSelection(state->message))
            {
            case 1:
                switch (state->round)
                {
                case 0: SetMessageWindowText(state->message, gUnk_08107A68); break;
                case 1: SetMessageWindowText(state->message, gUnk_08107A80); break;
                case 2: SetMessageWindowText(state->message, gUnk_08107A9C); break;
                }
                DisableMessageWindowInstantText(state->message);
                timer = 0;
                break;
            case 2:
                state->step = 1;
                state->waitingForChoice = 1;
                break;
            }
            break;
        }
    case 1:
    case 2:
    case 4:
    case 5:
    case 6:
        if (timer <= 119)
            ++timer;
        if (timer == 120)
        {
            state->showPrompt = 1;
            timer = 122;
            EnableMessageWindowInstantText(state->message);
            SetMessageWindowText(state->message, gUnk_08107AB4);
            DisableMessageWindowInstantText(state->message);
        }
        if ((u32) (timer - 122) <= 89)
            ++timer;
        if (timer == 212)
        {
            state->showPrompt = 0;
            timer = 121;
            EnableMessageWindowInstantText(state->message);
            CloseMessageWindow(state->message);
            state->waitingForChoice = 0;
            state->demonstrationComplete = 1;
        }
        break;
    }
    UpdateFestivalCheerDemonstration((FestivalCheerDemonstration *) state);
    return timer;
}


struct FestivalCrowdAnimation
{
    u8 unknown_000[0xb18];
    u32 round;
};

EC SECTION(".text.g3_080BB890") u16 GetFestivalCrowdAnimation(FestivalCrowdAnimation * state, i32 group, i32 participant, i32 pose)
{
    u16 animation = 0;
    switch (group)
    {
    case 0:
        animation = gUnk_08107D3C[participant];
        break;
    case 1:
        if (participant != 0) participant += state->round;
        animation = gUnk_08107D46[participant][pose];
        break;
    case 2:
        animation = gUnk_08107D66[pose];
        break;
    }
    return animation;
}
