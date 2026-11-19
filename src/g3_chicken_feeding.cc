// Chicken-feeding minigame timers, approach/return movement, and player feeding actions.
#include "prelude.h"
#include <stdlib.h>

enum FeedingChickenPhase
{
    FEEDING_WAITING = 1,
    FEEDING_READY,
    FEEDING_RETURNING,
    FEEDING_APPROACHING,
    FEEDING_AT_TROUGH,
    FEEDING_EATING,
};

enum FeedingEffectPhase { FEEDING_EFFECT_1 = 1, FEEDING_EFFECT_2 };
enum FeedingTroughPhase { FEEDING_TROUGH_0, FEEDING_TROUGH_1, FEEDING_TROUGH_2 };

struct ChickenFeedingState
{
    u8 unknown_000[0x844];
    u32 spriteAction;
    FeedingChickenPhase chickenStates[5];
    FeedingEffectPhase effectStates[5];
    FeedingTroughPhase troughStates[5];
    u32 selectedTrough;
    u8 unknown_888[0x59];
    u8 chickenY[5];
    u16 waitDuration[5];
    u16 waitFrames[5];
    u8 unknown_8FA[2];
    u16 eatingFrames[5];
    u8 effectFrames[5];
    u8 unknown_90B;
    u8 eventCount90C;
    u8 eventCount90D;
    u8 unknown_90E[7];
    u8 resetWaitDuration[5];
    u8 chickenAnimationDirty[5];
    u8 effectAnimationDirty[5];
    u8 troughAnimationDirty[5];
    u8 spriteAnimationDirty;
};

EC void AudioPlayer_StartSong(void *, u32);
EC void func_080B34DC(void *);


EC SECTION(".text.g3_080B3A28") void HandleChickenFeedingAction(ChickenFeedingState * state)
{
    func_080B34DC(state);
    state->spriteAction = 3;
    state->spriteAnimationDirty = 1;
    if (state->troughStates[state->selectedTrough] == 0)
    {
        if (state->chickenStates[state->selectedTrough] == 1)
        {
            state->waitFrames[state->selectedTrough] = 0;
            state->effectStates[state->selectedTrough] = FEEDING_EFFECT_1;
            ++state->eventCount90D;
            state->effectAnimationDirty[state->selectedTrough] = 1;
            state->effectFrames[state->selectedTrough] = 0;
            AudioPlayer_StartSong((u8 *) state + 0x814, 0xc7);
        }
    }
    else if (state->troughStates[state->selectedTrough] == 1)
    {
        state->troughStates[state->selectedTrough] = FEEDING_TROUGH_0;
        state->waitFrames[state->selectedTrough] = 0;
        state->resetWaitDuration[state->selectedTrough] = 1;
        state->chickenStates[state->selectedTrough] = FEEDING_WAITING;
        state->effectStates[state->selectedTrough] = FEEDING_EFFECT_2;
        state->chickenAnimationDirty[state->selectedTrough] = 1;
        state->troughAnimationDirty[state->selectedTrough] = 1;
        state->effectAnimationDirty[state->selectedTrough] = 1;
        AudioPlayer_StartSong((u8 *) state + 0x814, 0x6c);
    }
    else
    {
        AudioPlayer_StartSong((u8 *) state + 0x814, 0x6c);
        state->troughStates[state->selectedTrough] = FEEDING_TROUGH_0;
        state->troughAnimationDirty[state->selectedTrough] = 1;
        if (state->chickenStates[state->selectedTrough] == 4)
        {
            state->chickenStates[state->selectedTrough] = FEEDING_RETURNING;
            state->chickenAnimationDirty[state->selectedTrough] = 1;
        }
        else if (state->chickenStates[state->selectedTrough] == 5)
        {
            state->chickenStates[state->selectedTrough] = FEEDING_EATING;
            state->chickenAnimationDirty[state->selectedTrough] = 1;
        }
    }
}
