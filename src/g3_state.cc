#include "prelude.h"

EC u16 gUnk_08107882[];
EC u16 gUnk_0810788A[];
EC u16 gUnk_08107890[];
EC u16 gUnk_0810789A[];
EC u16 gUnk_081078A0[];
EC u32 func_080AE4D8(void *);
EC void func_08008724(void *);

struct G3SelectionState
{
    u8 unknown_000[0x848];
    u32 slots848[5];
    u32 slots85C[5];
    u32 slots870[5];
};

EC SECTION(".text.g3_080B3990") u16 G3GetBankedSelectionValue(u8 * state) // 0x080B3990
{
    return gUnk_08107882[*(u32 *) (state + 0x844)] + 0x54 * *(u32 *) (state + 0x930);
}

EC SECTION(".text.g3_080B39BC") u16 G3GetSlotValue870(G3SelectionState * state, u32 slot) // 0x080B39BC
{
    return gUnk_0810788A[state->slots870[slot]];
}

EC SECTION(".text.g3_080B39D8") u16 G3GetSlotValue848(G3SelectionState * state, u32 slot) // 0x080B39D8
{
    return gUnk_08107890[state->slots848[slot]];
}

EC SECTION(".text.g3_080B39F4") u16 G3GetSlotValue85C(G3SelectionState * state, u32 slot) // 0x080B39F4
{
    return gUnk_0810789A[state->slots85C[slot]];
}

EC SECTION(".text.g3_080B3A10") u16 G3GetSelectionValue934(u8 * state) // 0x080B3A10
{
    return gUnk_081078A0[*(u32 *) (state + 0x934)];
}

EC SECTION(".text.g3_080B3C78") u32 G3GetWrappedFlag92C(u8 ** wrapper) // 0x080B3C78
{
    return func_080AE4D8(wrapper[1]);
}

EC SECTION(".text.g3_080B3C84") u32 G3GetWrappedValue910(u8 ** wrapper) // 0x080B3C84
{
    return *(u32 *) (wrapper[1] + 0x910);
}

EC SECTION(".text.g3_080B44E0") u32 G3GetFlagB30(u8 * state) // 0x080B44E0
{
    return state[0xb30];
}

EC SECTION(".text.g3_080B45E4") void G3RequestPrimaryDisplayUpdate(void * display) // 0x080B45E4
{
    func_08008724(display);
}

EC SECTION(".text.g3_080BA1F4") void G3IgnoreActorCallback0() // 0x080BA1F4
{
}

EC SECTION(".text.g3_080BA1F8") void G3IgnoreActorCallback1() // 0x080BA1F8
{
}

EC SECTION(".text.g3_080BA1FC") void G3IgnoreActorCallback2() // 0x080BA1FC
{
}

EC SECTION(".text.g3_080BA200") void G3IgnoreActorCallback3() // 0x080BA200
{
}

EC SECTION(".text.g3_080BC8F0") u32 G3GetWrappedFlagB30(u8 ** wrapper) // 0x080BC8F0
{
    return G3GetFlagB30(wrapper[1]);
}

EC SECTION(".text.g3_080BCCD4") u32 G3GetFlag610(u8 * state) // 0x080BCCD4
{
    return state[0x610];
}

EC SECTION(".text.g3_080BCEBC") void G3RequestSecondaryDisplayUpdate(void * display) // 0x080BCEBC
{
    func_08008724(display);
}
