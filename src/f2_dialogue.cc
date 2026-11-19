#include "prelude.h"

// The public dialogue handle owns a separately allocated text-window state.
struct F2DialogueState
{
    u32 unknown_00[2];
    i32 phase;
    u8 animationFrame;
    u8 instantText;
    u8 unknown_0e[0x158 - 0xe];
    u32 selection;
};
struct F2DialogueHandle { F2DialogueState * state; };

EC void __builtin_delete(void *);
EC void func_0804F69C(F2DialogueState *, u32);
EC u32 func_0804F7A4(F2DialogueState *, u32, u32, u32, u32, u32, u32);
EC void func_0805039C(F2DialogueState *);
EC void func_080503E4(F2DialogueState *);
EC void func_080503FC(F2DialogueState *);
EC void func_08050410(F2DialogueState *);
EC void func_08050B3C(F2DialogueState *);
EC void func_08050C18(F2DialogueState *);
EC void func_08050C64(F2DialogueState *);
EC void func_08050424(F2DialogueState *, u32, u16, u32);
EC void func_08050478(F2DialogueState *, u32, u32, u32, u32, u32, u32, u32);
EC void func_08050664(F2DialogueState *, u32, u32, u32, u32, u32, u32);
EC void func_08050AD8(F2DialogueState *, u32);
EC void func_08050B50(F2DialogueState *, const char *);
EC void func_08050C2C(F2DialogueState *, u32);


EC i32 GetF2DialoguePhase(F2DialogueHandle * handle) asm("func_08050D34");
EC SECTION(".text.f2_dialogue") i32 GetF2DialoguePhase(F2DialogueHandle * handle)
{
    return handle->state->phase;
}

EC u32 UpdateF2Dialogue(F2DialogueHandle * handle, u32 input, u32 a, u32 b, u32 c, u32 d, u32 e) asm("func_08050D3C");
EC SECTION(".text.f2_dialogue") u32 UpdateF2Dialogue(F2DialogueHandle * handle, u32 input, u32 a, u32 b, u32 c, u32 d, u32 e)
{
    return func_0804F7A4(handle->state, input, a, b, c, d, e);
}

EC void OpenF2Dialogue(F2DialogueHandle * handle) asm("func_08050D5C");
EC SECTION(".text.f2_dialogue") void OpenF2Dialogue(F2DialogueHandle * handle)
{
    func_0805039C(handle->state);
}

EC void FinishOpeningF2Dialogue(F2DialogueHandle * handle) asm("func_08050D68");
EC SECTION(".text.f2_dialogue") void FinishOpeningF2Dialogue(F2DialogueHandle * handle)
{
    func_080503E4(handle->state);
}

EC void CloseF2Dialogue(F2DialogueHandle * handle) asm("func_08050D74");
EC SECTION(".text.f2_dialogue") void CloseF2Dialogue(F2DialogueHandle * handle)
{
    func_080503FC(handle->state);
}

EC void HideF2Dialogue(F2DialogueHandle * handle) asm("func_08050D80");
EC SECTION(".text.f2_dialogue") void HideF2Dialogue(F2DialogueHandle * handle)
{
    func_08050410(handle->state);
}

EC void SetF2DialogueText(F2DialogueHandle * handle, u32 text) asm("func_08050D8C");
EC SECTION(".text.f2_dialogue") void SetF2DialogueText(F2DialogueHandle * handle, u32 text)
{
    func_08050424(handle->state, text, 256, 0);
}

EC void SetF2DialogueTextFlags(F2DialogueHandle * handle, u32 text, u32 flags) asm("func_08050DA0");
EC SECTION(".text.f2_dialogue") void SetF2DialogueTextFlags(F2DialogueHandle * handle, u32 text, u32 flags)
{
    func_08050424(handle->state, text, 256, flags);
}

EC void SetF2DialogueTextSpeed(F2DialogueHandle * handle, u32 text, u16 speed) asm("func_08050DB4");
EC SECTION(".text.f2_dialogue") void SetF2DialogueTextSpeed(F2DialogueHandle * handle, u32 text, u16 speed)
{
    func_08050424(handle->state, text, speed, 0);
}

EC void SetF2DialogueTextOptions(F2DialogueHandle * handle, u32 text, u16 speed, u32 flags) asm("func_08050DC8");
EC SECTION(".text.f2_dialogue") void SetF2DialogueTextOptions(F2DialogueHandle * handle, u32 text, u16 speed, u32 flags)
{
    func_08050424(handle->state, text, speed, flags);
}

EC u8 DisableF2DialogueInstantText(F2DialogueHandle * handle) asm("func_08050DD8");
EC SECTION(".text.f2_dialogue") u8 DisableF2DialogueInstantText(F2DialogueHandle * handle)
{
    F2DialogueState * state = handle->state;
    u8 previous = state->instantText;
    state->instantText = 0;
    return previous;
}

EC u8 EnableF2DialogueInstantText(F2DialogueHandle * handle) asm("func_08050DE4");
EC SECTION(".text.f2_dialogue") u8 EnableF2DialogueInstantText(F2DialogueHandle * handle)
{
    F2DialogueState * state = handle->state;
    u8 previous = state->instantText;
    state->instantText = 1;
    return previous;
}

EC u32 GetF2DialogueSelection(F2DialogueHandle * handle) asm("func_08050DF0");
EC SECTION(".text.f2_dialogue") u32 GetF2DialogueSelection(F2DialogueHandle * handle)
{
    F2DialogueState * state = handle->state;
    return state->phase != 6 ? state->selection : 0;
}

EC void ConfigureF2DialogueChoices(F2DialogueHandle * handle, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g) asm("func_08050E0C");
EC SECTION(".text.f2_dialogue") void ConfigureF2DialogueChoices(F2DialogueHandle * handle, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f, u32 g)
{
    func_08050478(handle->state, a, b, c, d, e, f, g);
}

EC void ConfigureF2DialogueLayout(F2DialogueHandle * handle, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f) asm("func_08050E30");
EC SECTION(".text.f2_dialogue") void ConfigureF2DialogueLayout(F2DialogueHandle * handle, u32 a, u32 b, u32 c, u32 d, u32 e, u32 f)
{
    func_08050664(handle->state, a, b, c, d, e, f);
}

EC void SetF2DialoguePortrait(F2DialogueHandle * handle, u32 portrait) asm("func_08050E50");
EC SECTION(".text.f2_dialogue") void SetF2DialoguePortrait(F2DialogueHandle * handle, u32 portrait)
{
    func_08050AD8(handle->state, portrait);
}

EC void ClearF2DialoguePortrait(F2DialogueHandle * handle) asm("func_08050E5C");
EC SECTION(".text.f2_dialogue") void ClearF2DialoguePortrait(F2DialogueHandle * handle)
{
    func_08050B3C(handle->state);
}

EC void SetF2DialogueSpeakerName(F2DialogueHandle * handle, const char * name) asm("func_08050E68");
EC SECTION(".text.f2_dialogue") void SetF2DialogueSpeakerName(F2DialogueHandle * handle, const char * name)
{
    func_08050B50(handle->state, name);
}

EC void ClearF2DialogueSpeakerName(F2DialogueHandle * handle) asm("func_08050E74");
EC SECTION(".text.f2_dialogue") void ClearF2DialogueSpeakerName(F2DialogueHandle * handle)
{
    func_08050C18(handle->state);
}

EC void SetF2DialogueIndicator(F2DialogueHandle * handle, u32 indicator) asm("func_08050E80");
EC SECTION(".text.f2_dialogue") void SetF2DialogueIndicator(F2DialogueHandle * handle, u32 indicator)
{
    func_08050C2C(handle->state, indicator);
}

EC void ClearF2DialogueIndicator(F2DialogueHandle * handle) asm("func_08050E8C");
EC SECTION(".text.f2_dialogue") void ClearF2DialogueIndicator(F2DialogueHandle * handle)
{
    func_08050C64(handle->state);
}
