// Chicken-feeding sprite minigame, Chicken Festival combat, and sprite-harvesting results.
#include "prelude.h"

EC u16 gUnk_08107882[];
EC u16 gUnk_0810788A[];
EC u16 gUnk_08107890[];
EC u16 gUnk_0810789A[];
EC u16 gUnk_081078A0[];
struct ChickenFeedingScene;
EC u32 GetChickenFeedingResult(ChickenFeedingScene const & scene);
EC void HardwareContext_ForceBlank(void *);

struct ChickenFeedingAnimations
{
    u8 unknown_000[0x848];
    u32 chickenStates[5];
    u32 feedEffectStates[5];
    u32 troughStates[5];
};

EC SECTION(".text.g3_080B3990") u16 GetFeedingSpriteAnimation(u8 * state) // 0x080B3990
{
    return gUnk_08107882[*(u32 *) (state + 0x844)] + 0x54 * *(u32 *) (state + 0x930);
}

EC SECTION(".text.g3_080B39BC") u16 GetFeedingTroughAnimation(ChickenFeedingAnimations * state, u32 slot) // 0x080B39BC
{
    return gUnk_0810788A[state->troughStates[slot]];
}

EC SECTION(".text.g3_080B39D8") u16 GetFeedingChickenAnimation(ChickenFeedingAnimations * state, u32 slot) // 0x080B39D8
{
    return gUnk_08107890[state->chickenStates[slot]];
}

EC SECTION(".text.g3_080B39F4") u16 GetFeedingEffectAnimation(ChickenFeedingAnimations * state, u32 slot) // 0x080B39F4
{
    return gUnk_0810789A[state->feedEffectStates[slot]];
}

EC SECTION(".text.g3_080B3A10") u16 GetFeedingDialogueFrame(u8 * state) // 0x080B3A10
{
    return gUnk_081078A0[*(u32 *) (state + 0x934)];
}

EC SECTION(".text.g3_080B3C78") u32 DidChickenFeedingSucceed(u8 ** wrapper) // 0x080B3C78
{
    return GetChickenFeedingResult(*reinterpret_cast<ChickenFeedingScene const *>(wrapper[1]));
}

EC SECTION(".text.g3_080B3C84") u32 GetChickenFeedingScore(u8 ** wrapper) // 0x080B3C84
{
    return *(u32 *) (wrapper[1] + 0x910);
}

EC SECTION(".text.g3_080B44E0") u32 DidChickenFestivalPlayerWin(u8 * state) // 0x080B44E0
{
    return state[0xb30];
}

EC SECTION(".text.g3_080B45E4") void BlankChickenFestivalDisplay(void * display) // 0x080B45E4
{
    HardwareContext_ForceBlank(display);
}

EC SECTION(".text.g3_080BA1F4") void UpdateFestivalChickenAction2() // 0x080BA1F4
{
}

EC SECTION(".text.g3_080BA1F8") void UpdateFestivalChickenAction4() // 0x080BA1F8
{
}

EC SECTION(".text.g3_080BA1FC") void UpdateFestivalChickenIdle() // 0x080BA1FC
{
}

EC SECTION(".text.g3_080BA200") void UpdateFestivalChickenAction3() // 0x080BA200
{
}

EC SECTION(".text.g3_080BC8F0") u32 DidWrappedChickenFestivalPlayerWin(u8 ** wrapper) // 0x080BC8F0
{
    return DidChickenFestivalPlayerWin(wrapper[1]);
}

EC SECTION(".text.g3_080BCCD4") u32 DidSpriteHarvestingSucceed(u8 * state) // 0x080BCCD4
{
    return state[0x610];
}

EC SECTION(".text.g3_080BCEBC") void BlankSpriteHarvestingDisplay(void * display) // 0x080BCEBC
{
    HardwareContext_ForceBlank(display);
}
