#include "prelude.h"
#include <new>
#include <string.h>

struct PACKED MessageWindowFlags
{
    u8 visible : 1;
    u8 dirty : 1;
    u8 unused : 6;
};

struct MessageTileSize
{
    u16 width, height;
    MessageTileSize(u16 w, u16 h) : width(w), height(h) {}
};

struct MessageGlyphTile
{
    void * pixels;
    u32 allocation[2];
    u8 dirty;
    u8 padding[3];
};

// Three rotating rows of seven 32x16-pixel tiles hold 28 half-width columns.
struct MessageTextCursor
{
    void * vtable;
    void * fontPalette;
    MessageGlyphTile * rows[3];
    u16 column; // Half-width character cells, modulo 28.
    u16 row;    // Visible line, modulo 3.
    u8 firstRow;
    u8 needsClear;
    u8 changed;
    u8 printedNonSpace;
};

// The public dialogue handle owns a separately allocated text-window state.
struct MessageWindowState
{
    u32 background;
    u32 style;
    i32 phase;
    u8 animationFrame;
    u8 instantText;
    u8 unknown_0e[0xd0 - 0xe];
    MessageTextCursor textCursor;
    u8 textParser[0x158 - 0xec];
    u32 selection;
    u32 choiceLayout;
    u8 unknown_160[0x1dc - 0x160];
    MessageWindowFlags portraitFlags;
    u8 unknown_1dd[3];
    u32 portraitId;
    u8 portraitResource[0xc];
    MessageWindowFlags speakerNameFlags;
    u8 unknown_1f1[3];
    struct { u32 fontPalette; u8 pixels[0x100]; } speakerNameTiles[3];
    u8 indicatorVisible;
};
struct MessageWindow { MessageWindowState * state; };

EC void __builtin_delete(void *);
EC void func_0804E7A0(MessageTileSize, void *, u32);
EC void func_0804E8F0(MessageTileSize, void *, u32, u32, const char *);
EC void DrawMessageChoiceText(MessageWindowState *, const u8 &, const char *, const void *);
EC u32 GetMessageChoicePosition(u32, u32);
EC void func_0805E850(void *, void *, u32);
EC void func_080073E0(void *, u32);
EC void func_08007184(void *, u32);
EC void func_080ADC84(void *, u32);
EC void func_08007C28(void *, u32);
EC void func_080079E8(void *, u32);
EC void func_080098AC(void *, u32);
EC void func_080E105C(void *, u32);
EC void func_080098DC(void *, u32);
EC u8 vtable_unk_080E5A28[];
EC void func_0804F0E0(void *);
EC void func_08009940(void *, void *);
EC void func_080ADD5C(void *, const char *, const void *);
EC void func_080ADD74(void *, u16);
EC u32 func_080ADD20();
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
            func_08009940(commands, (u8 *) window + 0x130);
        func_08009940(commands, (u8 *) window + 0x148);
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
    void * parser = (u8 *) window + 0xec;
    func_080ADD5C(parser, text, substitutions);
    func_080ADD74(parser, speed);
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

EC SECTION(".text.message_window.08050478") void SetMessageWindowStateChoices(MessageWindowState * window, const char * first, const char * second, const char * third, const char * fourth, const char * fifth, const char * sixth, const void * substitutions) // 0x08050478
{
    if (window->phase >= 0)
    {
        if (window->phase <= 2) CompleteMessageWindowOpening(window);
        else if (window->phase <= 6) func_0804F0E0(&window->textCursor);
    }
    u32 layout = 4;
    u32 lineWidth = 12;
    if (sixth == nullptr)
    {
        layout = 3;
        if (fifth == nullptr)
        {
            layout = 2;
            if (fourth == nullptr)
            {
                lineWidth = 26;
                layout = ((u32) third | -(u32) third) >> 31;
            }
        }
    }
    switch (layout)
    {
    default: layout = 0;
    case 4: if (sixth == nullptr || strlen(sixth) > lineWidth) sixth = (const char *) gUnk_080F9F3C;
    case 3: if (fifth == nullptr || strlen(fifth) > lineWidth) fifth = (const char *) gUnk_080F9F3C;
    case 2: if (fourth == nullptr || strlen(fourth) > lineWidth) fourth = (const char *) gUnk_080F9F3C;
    case 1: if (third == nullptr || strlen(third) > lineWidth) third = (const char *) gUnk_080F9F3C;
    case 0:
        if (second == nullptr || strlen(second) > lineWidth) second = (const char *) gUnk_080F9F3C;
        if (first == nullptr || strlen(first) > lineWidth) first = (const char *) gUnk_080F9F3C;
    }
    switch (layout)
    {
    case 4:
    { u8 sixthPosition = GetMessageChoicePosition(layout, 6);
        DrawMessageChoiceText(window, sixthPosition, sixth, substitutions); }
    case 3:
    { u8 fifthPosition = GetMessageChoicePosition(layout, 5);
        DrawMessageChoiceText(window, fifthPosition, fifth, substitutions); }
    case 2:
    { u8 fourthPosition = GetMessageChoicePosition(layout, 4);
        DrawMessageChoiceText(window, fourthPosition, fourth, substitutions); }
    case 1:
    { u8 thirdPosition = GetMessageChoicePosition(layout, 3);
        DrawMessageChoiceText(window, thirdPosition, third, substitutions); }
    case 0:
    { u8 secondPosition = GetMessageChoicePosition(layout, 2);
        DrawMessageChoiceText(window, secondPosition, second, substitutions);
        u8 firstPosition = GetMessageChoicePosition(layout, 1);
        DrawMessageChoiceText(window, firstPosition, first, substitutions);
        break; }
    default: layout = 0;
    }
    window->choiceLayout = layout;
    window->selection = 1;
    window->phase = 6;
    func_0805E850((u8 *) window + 0x1c0, (u8 *) window + 0x160, 0);
}

EC SECTION(".text.message_window.0804F69C") void DestroyMessageWindowState(MessageWindowState * window, u32 flags) // 0x0804F69C
{
    register void * resource asm("r4") = (u8 *) window + 0x548;
    func_080073E0(resource, *((u32 *) resource + 1));
    func_08007184(resource, 2);
    *(void **) ((u8 *) window + 0x504) = vtable_unk_080E5A28;
    resource = (u8 *) window + 0x1e8;
    func_080073E0(resource, *((u32 *) resource + 1));
    func_08007184(resource, 2);
    func_080ADC84((u8 *) window + 0x1e4, 2);
    resource = (u8 *) window + 0x1d4;
    func_08007C28(resource, *((u32 *) resource + 1));
    func_080079E8(resource, 2);
    *(void **) ((u8 *) window + 0x190) = vtable_unk_080E5A28;
    *(void **) ((u8 *) window + 0x160) = vtable_unk_080E5A28;
    func_080098AC((u8 *) window + 0x148, 2);
    func_080098AC((u8 *) window + 0x130, 2);
    func_080098AC((u8 *) window + 0x120, 2);
    func_080E105C(&window->textCursor, 2);
    func_080098AC((u8 *) window + 0xc0, 2);
    u8 * end = (u8 *) window + 0x3c;
    end += *(u32 *) end * 16 + 4;
    for (u8 * transfer = (u8 *) window + 0x40; transfer != end; transfer += 16) {}
    func_080098DC((u8 *) window + 0x20, 2);
    resource = (u8 *) window + 0x18;
    func_08007C28(resource, *((u32 *) resource + 1));
    func_080079E8(resource, 2);
    resource = (u8 *) window + 0x10;
    func_080073E0(resource, *((u32 *) resource + 1));
    func_08007184(resource, 2);
    if (flags & 1) operator delete(window);
}
