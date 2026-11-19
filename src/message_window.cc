#include "prelude.h"
#include <new>

struct PACKED MessageWindowFlags
{
    u8 visible : 1;
    u8 dirty : 1;
    u8 unused : 6;
};

// The public dialogue handle owns a separately allocated text-window state.
struct MessageWindowState
{
    u32 background;
    u32 style;
    i32 phase;
    u8 animationFrame;
    u8 instantText;
    u8 unknown_0e[0x158 - 0xe];
    u32 selection;
    u8 unknown_15c[0x1dc - 0x15c];
    MessageWindowFlags portraitFlags;
    u8 unknown_1dd[3];
    u32 portraitId;
    u8 portraitResource[0xc];
    MessageWindowFlags speakerNameFlags;
    u8 unknown_1f1[0x500 - 0x1f1];
    u8 indicatorVisible;
};
struct MessageWindow { MessageWindowState * state; };

EC void __builtin_delete(void *);
EC void func_0804F0E0(void *);
EC void TaskList_Append(void *, void *);
struct MessageScript;
EC void StartMessageScript(MessageScript &, const char *, const void *);
EC void SetMessageScriptRate(MessageScript &, u16);
struct MessagePortraitResources
{
    u32 const * volatile vtable;
    STRUCT_PAD(0x04, 0x20);
    u16 clipCount;
    STRUCT_PAD(0x22, 0x30);
};

struct MessagePortraitClip
{
    void const * keyframes;
    u16 length;
};

struct MessagePortraitClips
{
    MessagePortraitClip * entries;
};

EC void func_0805E6CC(MessagePortraitResources *, void const *);
EC u8 const gUnk_0852D984[];
EC u32 const vtable_unk_080E5A28[];
EC void func_080ADCA4(void *, u32);
EC void func_0805E860(void *, u32);
EC const u8 gUnk_080F9F3C[];
EC void DestroyMessageWindowState(MessageWindowState *, u32);
EC u32 UpdateMessageWindowState(MessageWindowState *, u32, u32, u32, u32, u32, u32);
EC void BeginMessageWindowOpening(MessageWindowState *);
EC void CompleteMessageWindowOpening(MessageWindowState *);
EC void BeginMessageWindowClosing(MessageWindowState *);
EC void HideMessageWindowState(MessageWindowState *);
EC void ClearMessageWindowStatePortrait(MessageWindowState *);
EC void ClearMessageWindowStateSpeakerName(MessageWindowState *);
EC void ClearMessageWindowStateIndicator(MessageWindowState *);
EC void SetMessageWindowStateText(MessageWindowState *, const char *, u16, const void *);
EC void SetMessageWindowStateChoices(MessageWindowState *, const char *, const char *, const char *, const char *, const char *, const char *, const void *);
EC void SetMessageWindowStatePromptChoices(MessageWindowState *, const char *, const char *, const char *, const char *, const char *, const void *);
EC void SetMessageWindowStatePortrait(MessageWindowState *, u32);
EC void SetMessageWindowStateSpeakerName(MessageWindowState *, const char *);
EC void SetMessageWindowStateIndicator(MessageWindowState *, u32);

EC SECTION(".text.080ADC84") void DestroyMessagePortraitClips(MessagePortraitClips * clips, unsigned flags)
{
    __builtin_delete(clips->entries);
    if (flags & 1)
        __builtin_delete(clips);
}

EC SECTION(".text.080ADD20") u32 GetMessagePortraitCount()
{
    MessagePortraitResources resources;
    func_0805E6CC(&resources, gUnk_0852D984);
    u32 count = resources.clipCount;
    resources.vtable = vtable_unk_080E5A28;
    return count;
}

EC SECTION(".text.message_window.08050D34") i32 GetMessageWindowPhase(MessageWindow * handle)
{
    return handle->state->phase;
}

EC SECTION(".text.message_window.08050D3C") u32 UpdateMessageWindow(MessageWindow * handle, u32 input, u32 a, u32 b, u32 c, u32 d, u32 e)
{
    return UpdateMessageWindowState(handle->state, input, a, b, c, d, e);
}

EC SECTION(".text.message_window.08050D5C") void OpenMessageWindow(MessageWindow * handle)
{
    BeginMessageWindowOpening(handle->state);
}

EC SECTION(".text.message_window.08050D68") void FinishOpeningMessageWindow(MessageWindow * handle)
{
    CompleteMessageWindowOpening(handle->state);
}

EC SECTION(".text.message_window.08050D74") void CloseMessageWindow(MessageWindow * handle)
{
    BeginMessageWindowClosing(handle->state);
}

EC SECTION(".text.message_window.08050D80") void HideMessageWindow(MessageWindow * handle)
{
    HideMessageWindowState(handle->state);
}

EC SECTION(".text.message_window.08050D8C") void SetMessageWindowText(MessageWindow * handle, const char * text)
{
    SetMessageWindowStateText(handle->state, text, 256, 0);
}

EC SECTION(".text.message_window.08050DA0") void SetMessageWindowTextSubstitutions(MessageWindow * handle, const char * text, const void * substitutions)
{
    SetMessageWindowStateText(handle->state, text, 256, substitutions);
}

EC SECTION(".text.message_window.08050DB4") void SetMessageWindowTextSpeed(MessageWindow * handle, const char * text, u16 speed)
{
    SetMessageWindowStateText(handle->state, text, speed, 0);
}

EC SECTION(".text.message_window.08050DC8") void SetMessageWindowTextOptions(MessageWindow * handle, const char * text, u16 speed, const void * substitutions)
{
    SetMessageWindowStateText(handle->state, text, speed, substitutions);
}

EC SECTION(".text.message_window.08050DD8") u8 DisableMessageWindowInstantText(MessageWindow * handle)
{
    MessageWindowState * state = handle->state;
    u8 previous = state->instantText;
    state->instantText = 0;
    return previous;
}

EC SECTION(".text.message_window.08050DE4") u8 EnableMessageWindowInstantText(MessageWindow * handle)
{
    MessageWindowState * state = handle->state;
    u8 previous = state->instantText;
    state->instantText = 1;
    return previous;
}

EC SECTION(".text.message_window.08050DF0") u32 GetMessageWindowSelection(MessageWindow * handle)
{
    MessageWindowState * state = handle->state;
    return state->phase != 6 ? state->selection : 0;
}

EC SECTION(".text.message_window.08050E0C") void ConfigureMessageWindowChoices(MessageWindow * handle, const char * first, const char * second, const char * third, const char * fourth, const char * fifth, const char * sixth, const void * substitutions)
{
    SetMessageWindowStateChoices(handle->state, first, second, third, fourth, fifth, sixth, substitutions);
}

EC SECTION(".text.message_window.08050E30") void ConfigureMessageWindowPromptChoices(MessageWindow * handle, const char * prompt, const char * first, const char * second, const char * third, const char * fourth, const void * substitutions)
{
    SetMessageWindowStatePromptChoices(handle->state, prompt, first, second, third, fourth, substitutions);
}

EC SECTION(".text.message_window.08050E50") void SetMessageWindowPortrait(MessageWindow * handle, u32 portrait)
{
    SetMessageWindowStatePortrait(handle->state, portrait);
}

EC SECTION(".text.message_window.08050E5C") void ClearMessageWindowPortrait(MessageWindow * handle)
{
    ClearMessageWindowStatePortrait(handle->state);
}

EC SECTION(".text.message_window.08050E68") void SetMessageWindowSpeakerName(MessageWindow * handle, const char * name)
{
    SetMessageWindowStateSpeakerName(handle->state, name);
}

EC SECTION(".text.message_window.08050E74") void ClearMessageWindowSpeakerName(MessageWindow * handle)
{
    ClearMessageWindowStateSpeakerName(handle->state);
}

EC SECTION(".text.message_window.08050E80") void SetMessageWindowIndicator(MessageWindow * handle, u32 indicator)
{
    SetMessageWindowStateIndicator(handle->state, indicator);
}

EC SECTION(".text.message_window.08050E8C") void ClearMessageWindowIndicator(MessageWindow * handle)
{
    ClearMessageWindowStateIndicator(handle->state);
}

EC SECTION(".text.message_window.0805039C") void BeginMessageWindowOpening(MessageWindowState * window) // 0x0805039C
{
    if (window->phase == 0)
    {
        func_0804F0E0((u8 *) window + 0xd0);
        u32 style = window->style;
        void * commands = (u8 *) window + 0x20;
        if (style != 1)
            TaskList_Append(commands, (u8 *) window + 0x130);
        TaskList_Append(commands, (u8 *) window + 0x148);
        window->animationFrame = 0;
    }
    if (window->phase < 0) return;
    if (window->phase > 2) return;
    window->phase = 1;
}

EC SECTION(".text.message_window.080503E4") void CompleteMessageWindowOpening(MessageWindowState * window) // 0x080503E4
{
    BeginMessageWindowOpening(window);
    window->phase = 3;
    window->animationFrame = 9;
}

EC SECTION(".text.message_window.080503FC") void BeginMessageWindowClosing(MessageWindowState * window) // 0x080503FC
{
    if (window->phase != 0)
        window->phase = 2;
}

EC SECTION(".text.message_window.08050410") void HideMessageWindowState(MessageWindowState * window) // 0x08050410
{
    if (window->phase != 0)
        window->phase = 0;
}

EC SECTION(".text.message_window.08050424") void SetMessageWindowStateText(MessageWindowState * window, const char * text, u16 speed, const void * substitutions) // 0x08050424
{
    if (window->phase >= 0)
    {
        if (window->phase <= 2)
            CompleteMessageWindowOpening(window);
        else if (window->phase <= 6)
            func_0804F0E0((u8 *) window + 0xd0);
    }
    MessageScript * parser = (MessageScript *) ((u8 *) window + 0xec);
    StartMessageScript(*parser, text, substitutions);
    SetMessageScriptRate(*parser, speed);
    window->phase = 4;
}

EC SECTION(".text.message_window.080507D0") u32 GetMessageChoicePosition(u32 layout, u32 choice) // 0x080507D0
{
    if (layout > 7) layout = 0;
    if (choice - 1 > 5) choice = 1;
    return *(const u8 *) (layout * 6 + choice + (u32) gUnk_080F9F3C);
}

EC SECTION(".text.message_window.08050B3C") void ClearMessageWindowStatePortrait(MessageWindowState * window) // 0x08050B3C
{
    window->portraitFlags.visible = 0;
}

EC SECTION(".text.message_window.08050C18") void ClearMessageWindowStateSpeakerName(MessageWindowState * window) // 0x08050C18
{
    window->speakerNameFlags.visible = 0;
}

EC SECTION(".text.message_window.08050C2C") void SetMessageWindowStateIndicator(MessageWindowState * window, u32 indicator) // 0x08050C2C
{
    void * resource = (u8 *) window + 0x504;
    if (indicator >= *(u16 *) ((u8 *) resource + 0x20))
        ClearMessageWindowStateIndicator(window);
    else
    {
        window->indicatorVisible = 1;
        func_0805E860((u8 *) window + 0x534, indicator);
    }
}

EC SECTION(".text.message_window.08050C64") void ClearMessageWindowStateIndicator(MessageWindowState * window) // 0x08050C64
{
    window->indicatorVisible = 0;
}

EC SECTION(".text.message_window.08050D0C") void DestroyMessageWindow(MessageWindow * window, u32 flags) // 0x08050D0C
{
    register MessageWindow * self asm("r4") = window;
    register u32 deleteFlags asm("r5") = flags;
    if (self->state != nullptr)
        DestroyMessageWindowState(self->state, 3);
    if (deleteFlags & 1)
        operator delete(self);
}
