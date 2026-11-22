#include "prelude.h"
#include <new>
#include <stddef.h>
#include <string.h>
#include "gbasvc.h"
#include "sprite_animation.hh"
#include "g5_font.hh"

struct PACKED MessageWindowFlags
{
    u8 visible : 1;
    u8 dirty : 1;
    u8 unused : 6;
};

struct PACKED MessageWindowInputFlags
{
    u32 buttons : 6;
    u32 unused : 2;
};

struct MessageTileSize
{
    u16 width, height;
    MessageTileSize(u16 w, u16 h) : width(w), height(h) {}
};

struct MessageGlyphTile
{
    void * tileBuffer; // Four-byte header followed by 4bpp glyph pixels.
    u32 allocation[2];
    u8 dirty;
    u8 padding[3];
};

// Three rotating rows of seven 32x16-pixel tiles hold 28 half-width columns.
struct MessageTextCursor
{
    void * vtable;
    void * fontPalette; // Shared resource handle for the glyph tiles.
    MessageGlyphTile * rows[3];
    u16 column; // Half-width character cells, modulo 28.
    u16 row;    // Visible line, modulo 3.
    u8 firstRow;
    u8 needsClear;
    u8 changed;
    u8 printedNonSpace;
};

struct MessageNumberCommand
{
    u32 state;
    u32 unknown_04;
    void * vtable;
    u32 value;
    u16 * tilemap;
    u16 firstTile;
    u16 palette;
    u32 stride;
};

struct MessageTileCopyCommand
{
    u32 state;
    u32 unknown_04;
    void * vtable;
    u32 * source;
    u32 * destination;
    u16 tileCount : 14;
    u16 alternate : 1;
    u16 masked : 1;
};

struct MessageTilemapRectCommand
{
    u32 state;
    u32 unknown_04;
    void * vtable;
    u16 * tilemap;
    u16 palette;
    u16 firstTile;
    u32 width, height, stride;
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
    u8 unknown_501[0x550 - 0x501];
    MessageWindowInputFlags inputFlags;
};
struct MessageWindow { MessageWindowState * state; };
struct MessageTextAdvanceGate
{
    void * vtable;
    u8 enabled;
    u8 unknown_05[3];
    bool ready;
    u8 GetEnabled() const { return enabled; }
    bool IsEnabled() const { return GetEnabled(); }
};

EC void __builtin_delete(void *);
EC void FillMessageTiles(MessageTileSize, void *, u32);
EC u32 DrawMessageGlyph(MessageTileSize, void *, u32, u32, u32);
EC u32 DrawMessageGlyphWithColors(MessageTileSize, void *, u32, u32, u32, u32, u32);
EC void DrawMessageTileString(MessageTileSize, void *, u32, u32, const char *);
EC void DrawMessageChoiceText(MessageWindowState *, const u8 &, const char *, const void *);
EC u32 GetMessageChoicePosition(u32, u32);
EC void func_0805E850(void *, void *, u32);
EC void ResourceHandle_Release(void *, u32);
EC void ResourceHandles_Destroy(void *, u32);
EC void TileHandle_Release(void *, u32);
EC void TileHeapOwner_Destroy(void *, u32);
EC void TaskNode_Destroy(void *, u32);
EC void func_080E105C(void *, u32);
EC void TaskList_Destroy(void *, u32);
EC u8 vtable_unk_080E5A28[];
EC u8 vtable_unk_080E7868[], vtable_unk_080E7858[], vtable_unk_080E7848[], vtable_unk_080E7838[];
EC MessageWindowState * InitializeMessageWindowState(MessageWindowState *, u32, u32, u32, u32, void *, void *);
EC void ClearMessageTextCursor(MessageTextCursor *);
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

struct PACKED MessageGraphicsFlags
{
    u32 screenBase : 5;
    u32 firstTile : 10;
    u32 palette : 4;
    u32 spritePalette : 4;
    u32 unused : 9;
};
struct MessageWindowGraphics
{
    u32 unknown[3];
    MessageGraphicsFlags flags;
};
struct MessageGraphicsFrame
{
    u32 unknown[4];
    const void * palette;
    u32 tail[3];
};
struct MessageWindowGraphicsScratch
{
    u32 fill;
    MessageGraphicsFrame frame;
    MessagePortraitResources resources;
};

EC void DrawMessageFrame(u16 *, u32, u32, u32, u32, u32, u32);
EC const void * gUnk_080F9F78;
EC const u8 gUnk_08728208[];
EC void func_0805E790(MessageGraphicsFrame *, MessagePortraitResources *, u32);

EC void func_0805E6CC(MessagePortraitResources *, void const *);
EC u8 const gUnk_0852D984[];
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

struct MessagePortraitClipSet
{
    MessagePortraitClip clips[4];
};

struct MessagePortraitAnimationMethods
{
    STRUCT_PAD(0, 0xC);
    void (*getClip)(SpriteAnimationClip *, MessagePortraitResources const *, u32);
    void (*getFrame)(SpriteFrameView *, MessagePortraitResources const *, u32);
};

EC void * CopyMessagePortraitFrameBytes(void *, void const *, unsigned) asm("memcpy");

EC SECTION(".text.080ADCA4") void LoadMessagePortraitAnimationFrame(MessagePortraitClips & owner, u32 portraitId)
{
    struct PortraitFrameLookup
    {
        MessagePortraitClipSet frame;
        MessagePortraitResources resources;
        MessagePortraitClip clip;
    } lookup;
    register u32 clipId asm("r8") = portraitId;
    asm ("" : "+h" (clipId));
    register MessagePortraitClipSet * destination asm("r4") = reinterpret_cast<MessagePortraitClipSet *>(owner.entries);
    asm ("" : "+l" (destination));
    register void const * data asm("r1") = gUnk_0852D984;
    asm ("" : "+l" (data));
    register MessagePortraitResources * initialize asm("r0") = &lookup.resources;
    asm ("" : "+l" (initialize));
    func_0805E6CC(initialize, data);
    register MessagePortraitResources * resources asm("r6");
    asm ("add %0, sp, %2" : "=l" (resources) : "m" (lookup.resources.vtable), "I" (offsetof(PortraitFrameLookup, resources)));
    register MessagePortraitAnimationMethods const * methods asm("r5") =
        reinterpret_cast<MessagePortraitAnimationMethods const *>(lookup.resources.vtable);
    asm ("" : "+l" (methods));
    register SpriteAnimationClip * clip asm("r0") = reinterpret_cast<SpriteAnimationClip *>(&lookup.clip);
    asm ("" : "+l" (clip));
    methods->getClip(clip, resources, clipId);
    register unsigned frameId asm("r2") = static_cast<SpriteKeyframe const *>(lookup.clip.keyframes)->frame;
    asm ("" : "+l" (frameId));
    register SpriteFrameView * frame asm("r0") = reinterpret_cast<SpriteFrameView *>(&lookup.frame);
    asm ("" : "+l" (frame));
    methods->getFrame(frame, resources, frameId);
    lookup.resources.vtable = reinterpret_cast<u32 const *>(vtable_unk_080E5A28);
    frame = reinterpret_cast<SpriteFrameView *>(&lookup.frame);
    asm ("" : "+l" (frame));
    register MessagePortraitClipSet * frameSource asm("r1") = &lookup.frame;
    asm ("" : "+l" (frameSource));
    CopyMessagePortraitFrameBytes(frame, frameSource, sizeof(*frame));
    *destination = lookup.frame;
}

static inline void ClearMessagePortraitClipLength(MessagePortraitClip * clip, unsigned zero)
{
    asm ("strh %1, [%2, #4]" : "=m" (clip->length) : "l" (zero), "l" (clip));
}

EC SECTION(".text.080ADC00") MessagePortraitClips * CreateMessagePortraitClips(MessagePortraitClips * owner)
{
    register MessagePortraitClipSet * set asm("r2") =
        static_cast<MessagePortraitClipSet *>(operator new(sizeof(MessagePortraitClipSet)));
    asm ("" : "+l" (set));
    register unsigned zero asm("r0") = 0;
    asm ("" : "+l" (zero));
    set->clips[0].keyframes = reinterpret_cast<void *>(zero);
    ClearMessagePortraitClipLength(set->clips, zero);
    register MessagePortraitClip * clip asm("r1") = set->clips;
    asm ("" : "+l" (clip));
    ++clip;
    asm ("" : "+l" (clip));
    set->clips[1].keyframes = reinterpret_cast<void *>(zero);
    ClearMessagePortraitClipLength(clip, zero);
    ++clip;
    asm ("" : "+l" (clip));
    set->clips[2].keyframes = reinterpret_cast<void *>(zero);
    ClearMessagePortraitClipLength(clip, zero);
    ++clip;
    asm ("" : "+l" (clip));
    set->clips[3].keyframes = reinterpret_cast<void *>(zero);
    ClearMessagePortraitClipLength(clip, zero);
    owner->entries = set->clips;
    return owner;
}

EC SECTION(".text.080ADD08") MessagePortraitClipSet & CopyMessagePortraitClips(MessagePortraitClipSet & output, MessagePortraitClips const & owner)
{
    register MessagePortraitClipSet * original asm("r0") = &output;
    register MessagePortraitClipSet * destination asm("r2") = original;
    asm ("" : "+l" (destination) : "l" (original));
    register MessagePortraitClipSet const * source asm("r1") = reinterpret_cast<MessagePortraitClipSet const *>(owner.entries);
    asm ("" : "+l" (source));
    *destination = *source;
    return *original;
}

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
    resources.vtable = reinterpret_cast<u32 const *>(vtable_unk_080E5A28);
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
        ClearMessageTextCursor(&window->textCursor);
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
            ClearMessageTextCursor(&window->textCursor);
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

inline u32 GetMessagePortraitMinimum()
{
    union
    {
        u32 value;
        u16 halves[2];
    } bound;
    // Preserve the original unsigned lower-bound check.
    bound.halves[0] = 0;
    bound.halves[1] = 0;
    return bound.value;
}

EC SECTION(".text.message_window.08050AD8") void SetMessageWindowStatePortrait(MessageWindowState * window, u32 portrait)
{
    if (portrait < GetMessagePortraitMinimum() || portrait >= GetMessagePortraitCount())
    {
        ClearMessageWindowStatePortrait(window);
        return;
    }
    switch (window->portraitFlags.visible)
    {
    case 1:
        if (window->portraitId == portrait) return;
        break;
    default:
        window->portraitFlags.visible = 1;
    }
    LoadMessagePortraitAnimationFrame(*(MessagePortraitClips *) window->portraitResource, portrait);
    window->portraitId = portrait;
    window->portraitFlags.dirty = 1;
}

EC SECTION(".text.message_window.08050B3C") void ClearMessageWindowStatePortrait(MessageWindowState * window) // 0x08050B3C
{
    window->portraitFlags.visible = 0;
}

EC SECTION(".text.message_window.08050B50") void SetMessageWindowStateSpeakerName(MessageWindowState * inputWindow, const char * inputName)
{
    register MessageWindowState * window = inputWindow;
    register const char * name = inputName;
    register u32 length asm("r6") = name ? strlen(name) : 0;
    if (length - 1 > 11)
    {
        ClearMessageWindowStateSpeakerName(window);
        return;
    }
    register const char * source = name;
    register u32 tile asm("r5") = 0;
    register u32 tileWidth = 4;
    register u32 tileHeight = 2;
    do
    {
        register u8 * tileBase asm("r1") = (u8 *) window + tile * sizeof(window->speakerNameTiles[0]);
        MessageTileSize size(tileWidth, tileHeight);
        asm ("" : : "r" (size));
        register u8 * pixels asm("r4") = tileBase + offsetof(MessageWindowState, speakerNameTiles[0].pixels);
        asm ("" : : "r" (size));
        FillMessageTiles(size, pixels, 0);
        ++tile;
        if (length)
        {
            char chunk[5];
            register char * destination asm("r2") = chunk;
            register u8 * target asm("r1") = pixels;
            asm ("" : "+r" (target), "+r" (destination));
            do
            {
                *destination++ = *source++;
                --length;
            } while (length && destination != chunk + 4);
            *destination = 0;
            DrawMessageTileString(MessageTileSize(tileWidth, tileHeight), target, 0, 0, chunk);
        }
    } while (tile <= 2);
    window->speakerNameFlags.visible = 1;
    window->speakerNameFlags.dirty = 1;
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
        else if (window->phase <= 6) ClearMessageTextCursor(&window->textCursor);
    }
    u32 layout = 4;
    u32 lineWidth = 12; // Choice widths are limits in encoded bytes.
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

EC SECTION(".text.message_window.08050664") void SetMessageWindowStatePromptChoices(MessageWindowState * window, const char * prompt, const char * first, const char * inputSecond, const char * inputThird, const char * inputFourth, const void * substitutions)
{
    register const char * second = inputSecond;
    register const char * third = inputThird;
    register const char * fourth asm("r6") = inputFourth;
    if (prompt == nullptr || strlen(prompt) > 28) prompt = (const char *) gUnk_080F9F3C;
    register i32 layout = 7;
    register u32 lineWidth = 12;
    if (fourth == nullptr)
    {
        layout = 6;
        if (third == nullptr)
        {
            lineWidth = 26;
            layout = 5;
        }
    }
    switch (layout)
    {
    default: layout = 5;
    case 7: if (fourth == nullptr || strlen(fourth) > lineWidth) fourth = (const char *) gUnk_080F9F3C;
    case 6: if (third == nullptr || strlen(third) > lineWidth) third = (const char *) gUnk_080F9F3C;
    case 5:
        if (second == nullptr || strlen(second) > lineWidth) second = (const char *) gUnk_080F9F3C;
        if (first == nullptr || strlen(first) > lineWidth) first = (const char *) gUnk_080F9F3C;
    }
    ClearMessageTextCursor(&window->textCursor);
    u8 promptPosition = 0;
    DrawMessageChoiceText(window, promptPosition, prompt, substitutions);
    switch (layout)
    {
    case 7:
    { u8 fourthPosition = GetMessageChoicePosition(layout, 4);
        DrawMessageChoiceText(window, fourthPosition, fourth, substitutions); }
    case 6:
    { u8 thirdPosition = GetMessageChoicePosition(layout, 3);
        DrawMessageChoiceText(window, thirdPosition, third, substitutions); }
    case 5:
    { u8 secondPosition = GetMessageChoicePosition(layout, 2);
        DrawMessageChoiceText(window, secondPosition, second, substitutions);
        u8 firstPosition = GetMessageChoicePosition(layout, 1);
        DrawMessageChoiceText(window, firstPosition, first, substitutions);
        break; }
    default: layout = 5;
    }
    window->choiceLayout = layout;
    window->selection = 1;
    window->phase = 6;
    func_0805E850((u8 *) window + 0x1c0, (u8 *) window + 0x160, 0);
}

EC SECTION(".text.message_window.0804F69C") void DestroyMessageWindowState(MessageWindowState * window, u32 flags) // 0x0804F69C
{
    register void * resource asm("r4") = (u8 *) window + 0x548;
    ResourceHandle_Release(resource, *((u32 *) resource + 1));
    ResourceHandles_Destroy(resource, 2);
    *(void **) ((u8 *) window + 0x504) = vtable_unk_080E5A28;
    resource = (u8 *) window + 0x1e8;
    ResourceHandle_Release(resource, *((u32 *) resource + 1));
    ResourceHandles_Destroy(resource, 2);
    DestroyMessagePortraitClips(reinterpret_cast<MessagePortraitClips *>((u8 *) window + 0x1e4), 2);
    resource = (u8 *) window + 0x1d4;
    TileHandle_Release(resource, *((u32 *) resource + 1));
    TileHeapOwner_Destroy(resource, 2);
    *(void **) ((u8 *) window + 0x190) = vtable_unk_080E5A28;
    *(void **) ((u8 *) window + 0x160) = vtable_unk_080E5A28;
    TaskNode_Destroy((u8 *) window + 0x148, 2);
    TaskNode_Destroy((u8 *) window + 0x130, 2);
    TaskNode_Destroy((u8 *) window + 0x120, 2);
    func_080E105C(&window->textCursor, 2);
    TaskNode_Destroy((u8 *) window + 0xc0, 2);
    u8 * end = (u8 *) window + 0x3c;
    end += *(u32 *) end * 16 + 4;
    for (u8 * transfer = (u8 *) window + 0x40; transfer != end; transfer += 16) {}
    TaskList_Destroy((u8 *) window + 0x20, 2);
    resource = (u8 *) window + 0x18;
    TileHandle_Release(resource, *((u32 *) resource + 1));
    TileHeapOwner_Destroy(resource, 2);
    resource = (u8 *) window + 0x10;
    ResourceHandle_Release(resource, *((u32 *) resource + 1));
    ResourceHandles_Destroy(resource, 2);
    if (flags & 1) operator delete(window);
}

inline bool IsMessageGlyphPixelAligned(u32 position) { return (position & 7) == 0; }
inline u8 * GetMessageGlyphTilePixels(void * pixels, u32 columns, u32 column, u32 row)
{
    return (u8 *) ((columns * row + column) * 32 + (u32) pixels);
}
EC u32 DrawUnalignedMessageGlyph(MessageTileSize, void *, u32, u32, const void *);

EC SECTION(".text.message_rendering.0804E4AC") u32 DrawMessageGlyph(MessageTileSize size, void * pixels, u32 x, u32 y, u32 glyph)
{
    i32 cachedColumn;
    u32 tiles[4][8];
    u32 rowStep;
    u32 * columnAddress;
    u8 * rightPixels;
    u8 * bottomPixels;
    u32 boundsRows;
    u32 width = Font_DecodeGlyph(tiles, glyph);
    i32 bottomRightTile;
    if (width - 1 <= 1)
    {
        u32 column = x >> 3;
        u32 row = y >> 3;
        register u32 columns = size.width;
        u16 rows = size.height;
        bottomRightTile = 3;
        rowStep = 1;
        boundsRows = rows;
        columnAddress = &column;
        cachedColumn = *columnAddress;
        if (*columnAddress < columns && row < boundsRows)
        {
            if (IsMessageGlyphPixelAligned(x) && IsMessageGlyphPixelAligned(y))
            {
                i32 right = 0;
                if (*columnAddress + 1 < columns)
                {
                    register u32 rightValue asm("r1") = 1;
                    asm ("" : "+r" (rightValue));
                    right = rightValue;
                }
                bool bottom = row + rowStep < rows;
                register u32 firstOffset asm("r0") = (columns * row + *columnAddress) * 32;
                register u32 firstBase asm("r2") = (u32) pixels;
                asm ("" : "+r" (firstOffset), "+r" (firstBase));
                u8 * destination = (u8 *) (firstOffset + firstBase);
                CpuFastSet(tiles[0], destination, 8);
                if (bottom)
                {
                    bottomPixels = GetMessageGlyphTilePixels(pixels, size.width, *columnAddress, row + rowStep);
                    CpuFastSet(tiles[2], bottomPixels, 8);
                }
                if (right)
                {
                    register u32 glyphWidth asm("r1") = width;
                    asm ("" : "+r" (glyphWidth));
                    if (glyphWidth > 1)
                    {
                        rightPixels = destination + 32;
                        CpuFastSet(tiles[1], rightPixels, 8);
                        if (bottom)
                        {
                            register u32 product asm("r0") = columns * (row + rowStep);
                            register u32 copyColumn asm("r2") = cachedColumn;
                            asm ("" : "+r" (product), "+r" (copyColumn));
                            register u32 tileIndex asm("r1") = copyColumn + product;
                            asm ("" : "+r" (tileIndex));
                            register u32 byteOffset asm("r0") = tileIndex * 32;
                            register u32 pixelBase asm("r2") = (u32) pixels;
                            asm ("" : "+r" (byteOffset), "+r" (pixelBase));
                            u8 * copyDestination = (u8 *) (byteOffset + pixelBase + 32);
                            column = 8;
                            CpuFastSet(tiles[bottomRightTile], copyDestination, column);
                        }
                    }
                }
            }
            else
            {
                DrawUnalignedMessageGlyph(size, pixels, x, y, tiles);
            }
        }
        return width;
    }
    return 0;
}

EC SECTION(".text.message_rendering.0804E7A0") void FillMessageTiles(MessageTileSize size, void * pixels, u32 color) // 0x0804E7A0
{
    color &= 15;
    color |= color << 4;
    color |= color << 8;
    color |= color << 16;
    u32 width = size.width;
    u32 bytes = size.height * 32;
    bytes *= width;
    u32 fill = color;
    CpuFastSet(&fill, pixels, ((bytes >> 2) & 0x1fffff) | 0x1000000);
}

EC SECTION(".text.message_rendering.0804E8F0") void DrawMessageTileString(MessageTileSize size, void * pixels, u32 x, u32 y, const char * text) // 0x0804E8F0
{
    u32 lineWidth = size.width * 8;
    u32 code = 0;
    u32 byte = *(const u8 *) text;
    while (byte != 0 && x < lineWidth)
    {
        code |= byte;
        switch (DrawMessageGlyph(size, pixels, x, y, code))
        {
        case 1: code = 0; x += 8; break;
        case 2: code = 0; x += 16; break;
        case 0: break;
        default: return;
        }
        text++;
        byte = *(const u8 *) text;
        code <<= 8;
    }
}

EC SECTION(".text.message_rendering.0804E958") void DrawMessageTileStringWithColors(MessageTileSize size, void * pixels, u32 x, u32 y, const char * text, u32 foreground, u32 background) // 0x0804E958
{
    u32 lineWidth = size.width * 8;
    u32 code = 0;
    u32 byte = *(const u8 *) text;
    while (byte != 0 && x < lineWidth)
    {
        code |= byte;
        switch (DrawMessageGlyphWithColors(size, pixels, x, y, code, foreground, background))
        {
        case 1: code = 0; x += 8; break;
        case 2: code = 0; x += 16; break;
        case 0: break;
        default: return;
        }
        text++;
        byte = *(const u8 *) text;
        code <<= 8;
    }
}

EC SECTION(".text.message_rendering.0804E9F4") void FillMessageTilemapRect(u16 * tilemap, u16 firstTile, u32 width, u32 height, u16 palette, u32 stride) // 0x0804E9F4
{
    u16 * row = tilemap;
    for (u32 y = 0; y < height; y++)
    {
        for (u32 x = 0; x < width; x++)
            *tilemap++ = firstTile++ | (palette << 12);
        row += stride;
        tilemap = row;
    }
}

EC SECTION(".text.message_rendering.0804EA58") MessageTilemapRectCommand * InitializeMessageTilemapRectCommand(MessageTilemapRectCommand * command, u16 * tilemap, u16 firstTile, u32 width, u32 height, u16 palette, u32 stride) // 0x0804EA58
{
    command->state = 0;
    command->unknown_04 = 0;
    command->vtable = vtable_unk_080E7868;
    command->tilemap = tilemap;
    command->firstTile = firstTile;
    command->width = width;
    command->height = height;
    command->palette = palette;
    command->stride = stride;
    return command;
}

EC SECTION(".text.message_rendering.0804EA80") void DestroyMessageTilemapRectCommand(MessageTilemapRectCommand * command, u32 flags) // 0x0804EA80
{
    command->vtable = vtable_unk_080E7868;
    TaskNode_Destroy(command, flags);
}

EC SECTION(".text.message_rendering.0804EDA0") void DestroyLargeMessageNumberCommand(MessageNumberCommand * command, u32 flags) // 0x0804EDA0
{
    command->vtable = vtable_unk_080E7858;
    TaskNode_Destroy(command, flags);
}

EC SECTION(".text.message_rendering.0804EE1C") void DestroyTallMessageNumberCommand(MessageNumberCommand * command, u32 flags) // 0x0804EE1C
{
    command->vtable = vtable_unk_080E7848;
    TaskNode_Destroy(command, flags);
}

EC SECTION(".text.message_rendering.0804EE88") void DestroySmallMessageNumberCommand(MessageNumberCommand * command, u32 flags) // 0x0804EE88
{
    command->vtable = vtable_unk_080E7838;
    TaskNode_Destroy(command, flags);
}

EC SECTION(".text.message_rendering.0804ED28") void DrawLargeMessageNumber(u32 value, u16 * tilemap, u32 firstTileArg, u32 paletteArg, u32 stride) // 0x0804ED28
{
    u16 firstTile = firstTileArg;
    u16 palette = paletteArg;
    u16 * destination = tilemap;
    u32 paletteBits = palette << 12;
    do
    {
        u32 quotient = value / 10;
        u32 tile = firstTile + (value - quotient * 10) * 4;
        destination[0] = (tile + 1) | paletteBits;
        destination[-1] = paletteBits | tile;
        destination[stride] = (tile + 3) | paletteBits;
        destination[stride - 1] = (tile + 2) | paletteBits;
        value = quotient;
        destination -= 2;
    } while (value != 0);
}

EC SECTION(".text.message_rendering.0804EDB4") void DrawTallMessageNumber(u32 value, u16 * tilemap, u32 firstTileArg, u32 paletteArg, u32 stride) // 0x0804EDB4
{
    u16 firstTile = firstTileArg;
    u16 palette = paletteArg;
    u16 * destination = tilemap;
    u32 paletteBits = palette << 12;
    do
    {
        u32 quotient = value / 10;
        u32 tile = firstTile + (value - quotient * 10) * 2;
        destination[0] = paletteBits | tile;
        destination[stride] = (tile + 1) | paletteBits;
        value = quotient;
        destination -= 1;
    } while (value != 0);
}

EC SECTION(".text.message_rendering.0804EE30") void DrawSmallMessageNumber(u32 value, u16 * tilemap, u32 firstTileArg, u32 paletteArg, u32 stride) // 0x0804EE30
{
    u16 firstTile = firstTileArg;
    u16 palette = paletteArg;
    u16 * destination = tilemap;
    u32 paletteBits = palette << 12;
    do
    {
        u32 quotient = value / 10;
        u32 tile = firstTile + (value - quotient * 10) * 1;
        destination[0] = paletteBits | tile;
        value = quotient;
        destination -= 1;
    } while (value != 0);
}

EC SECTION(".text.message_rendering.0804EE64") MessageNumberCommand * InitializeSmallMessageNumberCommand(MessageNumberCommand * command, u32 value, u16 * tilemap, u16 firstTile, u16 palette, u32 stride) // 0x0804EE64
{
    command->state = 0;
    command->unknown_04 = 0;
    command->vtable = vtable_unk_080E7838;
    command->value = value;
    command->tilemap = tilemap;
    command->firstTile = firstTile;
    command->palette = palette;
    command->stride = stride;
    return command;
}

EC SECTION(".text.message_rendering.0804EE9C") u32 RunSmallMessageNumberCommand(MessageNumberCommand * command) // 0x0804EE9C
{
    DrawSmallMessageNumber(command->value, command->tilemap, command->firstTile, command->palette, command->stride);
    return 0;
}

EC SECTION(".text.message_rendering.0804EEBC") u32 RunTallMessageNumberCommand(MessageNumberCommand * command) // 0x0804EEBC
{
    DrawTallMessageNumber(command->value, command->tilemap, command->firstTile, command->palette, command->stride);
    return 0;
}

EC SECTION(".text.message_rendering.0804EEDC") u32 RunLargeMessageNumberCommand(MessageNumberCommand * command) // 0x0804EEDC
{
    DrawLargeMessageNumber(command->value, command->tilemap, command->firstTile, command->palette, command->stride);
    return 0;
}

EC SECTION(".text.message_rendering.0804EEFC") u32 RunMessageTilemapRectCommand(MessageTilemapRectCommand * command) // 0x0804EEFC
{
    FillMessageTilemapRect(command->tilemap, command->firstTile, command->width, command->height, command->palette, command->stride);
    return 0;
}

EC SECTION(".text.message_rendering.0804F15C") u8 IsMessageTextCursorCleared(MessageTextCursor * cursor) // 0x0804F15C
{
    return cursor->needsClear;
}

EC SECTION(".text.message_rendering.0804F174") bool AllowMessageTextAdvance(MessageTextAdvanceGate * gate) // 0x0804F174
{
    return true;
}

EC SECTION(".text.message_window.08050CC0") MessageWindow * CreateMessageWindow(MessageWindow * window, u32 background, u32 tiles, u32 palette, u32 style, void * graphics, void * font) // 0x08050CC0
{
    MessageWindowState * state = (MessageWindowState *) operator new(0x554);
    window->state = InitializeMessageWindowState(state, background, tiles, palette, style, graphics, font);
    return window;
}

EC SECTION(".text.message_rendering.0804E9C8") u32 DrawUnalignedMessageGlyph(MessageTileSize, void *, u32, u32, const void *)
{
    return 0;
}

EC SECTION(".text.message_rendering.0804E9CC") u32 DrawUnalignedMessageGlyphWithColors(MessageTileSize, void *, u32, u32, const void *, u32, u32)
{
    return 0;
}

EC SECTION(".text.message_rendering.0804F160") bool IsMessageTextAdvanceEnabled(MessageTextAdvanceGate * gate)
{
    return gate->IsEnabled();
}

EC SECTION(".text.message_rendering.0804F178") bool ShouldMessageTextAdvance(MessageTextAdvanceGate * gate)
{
    return gate->ready && gate->IsEnabled();
}

EC SECTION(".text.message_rendering.0804E9D0") void CopyMessageTiles(MessageTileSize size, void * destination, const void * source)
{
    CpuFastSet(source, destination, ((size.width * size.height * 32u) >> 2) & 0x1fffff);
}

EC SECTION(".text.message_rendering.0804ED7C") MessageNumberCommand * InitializeLargeMessageNumberCommand(MessageNumberCommand * command, u32 value, u16 * tilemap, u16 firstTile, u16 palette, u32 stride)
{
    command->state = 0;
    command->unknown_04 = 0;
    command->vtable = vtable_unk_080E7858;
    command->value = value;
    command->tilemap = tilemap;
    command->firstTile = firstTile;
    command->palette = palette;
    command->stride = stride;
    return command;
}

EC SECTION(".text.message_rendering.0804EDF8") MessageNumberCommand * InitializeTallMessageNumberCommand(MessageNumberCommand * command, u32 value, u16 * tilemap, u16 firstTile, u16 palette, u32 stride)
{
    command->state = 0;
    command->unknown_04 = 0;
    command->vtable = vtable_unk_080E7848;
    command->value = value;
    command->tilemap = tilemap;
    command->firstTile = firstTile;
    command->palette = palette;
    command->stride = stride;
    return command;
}

inline u32 PrintMessageCursorTile(MessageGlyphTile * tile, u32 inputColumn, u32 glyph)
{
    register u32 column = inputColumn;
    switch (column)
    {
    case 0: case 1: case 2: case 3:
    {
        void * buffer = tile->tileBuffer;
        u32 x = (column & 3) * 8;
        u32 width = DrawMessageGlyph(MessageTileSize(4, 2), (u8 *) buffer + 4, x, 0, glyph);
        if (width - 1 <= 1) tile->dirty = tile->allocation[1] != 0;
        return width;
    }
    }
    return 0;
}

EC SECTION(".text.message_rendering.0804EFAC") u32 PrintMessageCursorGlyph(MessageTextCursor * cursor, u32 glyph)
{
    u32 column = cursor->column;
    i32 row = cursor->row + cursor->firstRow;
    if (row > 2) row -= 3;
    MessageGlyphTile * line = *((MessageGlyphTile **) ((u8 *) cursor + row * 4) + 2);
    MessageGlyphTile * tile = line + (column >> 2);
    register u32 tileColumn asm("r2") = column & 3;
    u32 width = PrintMessageCursorTile(tile, tileColumn, glyph);
    if (width == 0) return 0;
    if (width > 2) return 0;
    u32 nextColumn = column + width;
    if (nextColumn > 27) nextColumn %= 28;
    cursor->column = nextColumn;
    if (glyph != 0x20 && glyph != 0x8140) cursor->printedNonSpace = 1;
    cursor->changed = 1;
    return 1;
}

EC SECTION(".text.message_rendering.0804F058") void ResetMessageTextColumn(MessageTextCursor * cursor)
{
    cursor->column = 0;
}

EC SECTION(".text.message_rendering.0804EF20") bool RunMessageTileCopyCommand(MessageTileCopyCommand * command)
{
    u32 * source = command->source;
    u32 * destination = command->destination;
    if (command->masked)
    {
        u32 * end = destination + command->tileCount * 8;
        u32 alternate = command->alternate ^ 1;
        command->alternate = alternate;
        u32 mask = 0x0f0f0f0f << (alternate * 4);
        u32 inverse = ~mask;
        do
        {
            *destination++ = *source++ & mask;
            *destination++ = *source++ & inverse;
            *destination++ = *source++ & mask;
            *destination++ = *source++ & inverse;
            *destination++ = *source++ & mask;
            *destination++ = *source++ & inverse;
            *destination++ = *source++ & mask;
            *destination++ = *source++ & inverse;
        } while (destination != end);
        return true;
    }
    CpuFastSet(source, destination, command->tileCount * 8);
    return false;
}

inline void FillMessageGraphicsMap(void * destination, u32 count, u32 pattern, u32 & fill)
{
    fill = pattern;
    CpuFastSet(&fill, destination, count | 0x1000000);
}

inline void LoadMessageFramePalette(u32 bank)
{
    CpuFastSet(gUnk_080F9F78, (void *) (0x5000000 + bank * 32), 8);
}

EC SECTION(".text.message_rendering.0804F19C") u32 LoadMessageWindowGraphics(MessageWindowGraphics * graphics)
{
    MessageWindowGraphicsScratch scratch;
    u16 tile = ((graphics->flags.firstTile + 3) & 0x3ff) | (graphics->flags.palette << 12);
    u32 pattern = tile | (tile << 16);
    FillMessageGraphicsMap((void *) (0x6000000 + graphics->flags.screenBase * 0x800), 0xa0, pattern, scratch.fill);
    DrawMessageFrame((u16 *) (0x6000000 + graphics->flags.screenBase * 0x800), 16, 0, 14, 3, graphics->flags.firstTile, graphics->flags.palette);
    DrawMessageFrame((u16 *) (0x6000000 + graphics->flags.screenBase * 0x800), 0, 3, 30, 7, graphics->flags.firstTile, graphics->flags.palette);
    LoadMessageFramePalette(graphics->flags.palette);
    register MessagePortraitResources * resources asm("r4") = &scratch.resources;
    func_0805E6CC(resources, gUnk_08728208);
    func_0805E790(&scratch.frame, resources, 0);
    CpuFastSet(scratch.frame.palette, (void *) (0x5000200 + graphics->flags.spritePalette * 32), 8);
    scratch.resources.vtable = (const u32 *) vtable_unk_080E5A28;
    return 0;
}

EC SECTION(".text.message_window.08050E98") u32 ClearMessageWindowButtonMask(MessageWindow * window, u32 mask)
{
    MessageWindowState * state = window->state;
    u32 previous = state->inputFlags.buttons;
    state->inputFlags.buttons = previous & ~mask;
    return previous;
}

inline void ClearMessageGlyphTile(MessageGlyphTile * tile)
{
    void * buffer = tile->tileBuffer;
    FillMessageTiles(MessageTileSize(4, 2), (u8 *) buffer + 4, 0);
    tile->dirty = tile->allocation[1] != 0;
}

inline void ClearMessageGlyphRow(MessageGlyphTile * tiles)
{
    for (i32 column = 0; column <= 6; ++column)
        ClearMessageGlyphTile(tiles + column);
}

EC SECTION(".text.message_rendering.0804F060") void AdvanceMessageTextLine(MessageTextCursor * cursor)
{
    if (cursor->row <= 1)
    {
        ++cursor->row;
        return;
    }
    ++cursor->firstRow;
    if (cursor->firstRow > 2) cursor->firstRow = 0;
    i32 physicalRow = cursor->firstRow + 2;
    if (physicalRow > 2) physicalRow = cursor->firstRow - 1;
    ClearMessageGlyphRow(*((MessageGlyphTile **) ((u8 *) cursor + physicalRow * 4) + 2));
    cursor->needsClear = 1;
    cursor->changed = 1;
}

inline bool IsMessagePixelAligned(u32 value) { return (value & 7) == 0; }

inline u32 ExpandMessagePixelColor(u32 color)
{
    color &= 15;
    color |= color << 4;
    color |= color << 8;
    color |= color << 16;
    return color;
}

EC SECTION(".text.message_rendering.0804E7DC") void FillMessagePixelRect(MessageTileSize size, void * pixels, u32 x, u32 y, u32 width, u32 height, u32 color)
{
    register u32 pattern = ExpandMessagePixelColor(color);
    u32 fill;
    u32 columns = size.width;
    u32 rows = size.height;
    u32 pixelWidth = columns * 8;
    u32 pixelHeight = rows * 8;
    i32 columnBytes;
    if (x == 0 && y == 0 && width >= pixelWidth && height >= pixelHeight)
    {
        columnBytes = rows * 32;
        u32 bufferBytes = columns * columnBytes;
        fill = pattern;
        CpuFastSet(&fill, pixels, ((bufferBytes >> 2) & 0x1fffff) | 0x1000000);
        return;
    }
    if (x < columns && y < rows)
    {
        if (x + width >= pixelWidth)
        {
            // matches original codegen
            do { width = pixelWidth - x; } while (0);
        }
        if (y + height >= pixelHeight) height = pixelHeight - y;
        if (IsMessagePixelAligned(x) && IsMessagePixelAligned(y) && IsMessagePixelAligned(width) && IsMessagePixelAligned(height))
        {
            u32 count = height >> 3;
            u8 * destination = (u8 *) pixels + (((y >> 3) * columns + (x >> 3)) * 32);
            u32 stride = columns * 32;
            u32 words = (((width >> 3) * 32) >> 2) & 0x1fffff;
            do
            {
                fill = pattern;
                CpuFastSet(&fill, destination, words | 0x1000000);
                destination += stride;
            } while (--count);
        }
    }
}
