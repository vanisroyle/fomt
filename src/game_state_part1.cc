#include "prelude.h"

#include "entity_actor.hh"
#include "farm.hh"
#include "coop.hh"
#include "script_engine.hh"
#include "scene.hh"
#include "shipping_bin.hh"
#include "bachelorette.hh"

#include <stdlib.h>

struct PACKED GameStateHeader
{
    u8 flagA : 1;
    u8 flagB : 1;
    u8 flagC : 1;
    u8 flagD : 1;
    u8 flagE : 1;
    u8 reservedBits : 3;
    u8 reservedBytes[2];
    u8 reservedBit : 1;
    u8 counter : 6;
};

struct GameStateSelection
{
    u32 current;
    u32 previous;
    u8 delay;
};

struct GameStateWordHeader
{
    u32 unused : 13;
    u32 counter : 5;
};

// 0x08010F0C
EC u32 SECTION(".text.game_state_08010F0C") SaveProgress_GetWordCounter(GameStateWordHeader const & header)
{
    return header.counter;
}

// 0x08011588
EC u32 SECTION(".text.game_state_08011588") SaveData_SumBytes(void const * data, u32 length)
{
    u32 sum = 0;
    if (data != nullptr && length != 0)
    {
        u8 const * cursor = static_cast<u8 const *>(data);
        do
        {
            sum += *cursor++;
        } while (--length != 0);
    }
    return sum;
}

// 0x08010348
EC int SECTION(".text.game_state_08010348") Random_NonZero()
{
    int value;
    do
    {
        value = rand();
    } while (value == 0);
    return value;
}

// 0x08010E48
EC u32 SECTION(".text.game_state_08010E48") SaveProgress_GetFlagA(GameStateHeader const & header)
{
    return header.flagA;
}

// 0x08010E50
EC u32 SECTION(".text.game_state_08010E48") SaveProgress_GetFlagB(GameStateHeader const & header)
{
    return header.flagB;
}

// 0x08010E58
EC u32 SECTION(".text.game_state_08010E48") SaveProgress_GetFlagC(GameStateHeader const & header)
{
    return header.flagC;
}

// 0x08010E60
EC u32 SECTION(".text.game_state_08010E48") SaveProgress_GetFlagD(GameStateHeader const & header)
{
    return header.flagD;
}

// 0x08010F04
EC u32 SECTION(".text.game_state_08010F04") SaveProgress_GetFlagE(GameStateHeader const & header)
{
    return header.flagE;
}

// 0x08010F1C
EC u32 SECTION(".text.game_state_08010F1C") SaveProgress_GetByteCounter(GameStateHeader const & header)
{
    return header.counter;
}

// 0x08010F24
EC void SECTION(".text.game_state_08010F1C") SaveProgress_SetFlagA(GameStateHeader & header)
{
    header.flagA = 1;
}

// 0x08010F30
EC void SECTION(".text.game_state_08010F1C") SaveProgress_SetFlagB(GameStateHeader & header)
{
    header.flagB = 1;
}

// 0x08010F3C
EC void SECTION(".text.game_state_08010F1C") SaveProgress_SetFlagC(GameStateHeader & header)
{
    header.flagC = 1;
}

// 0x08010F48
EC void SECTION(".text.game_state_08010F1C") SaveProgress_SetFlagD(GameStateHeader & header)
{
    header.flagD = 1;
}

// 0x08011458
EC void SECTION(".text.game_state_08011458") SaveProgress_SetFlagE(GameStateHeader & header)
{
    header.flagE = 1;
}

// 0x08011510
EC void SECTION(".text.game_state_08011510") Selection_Reset(GameStateSelection & selection)
{
    selection.current = 0x10;
    selection.previous = 0x10;
    selection.delay = 0;
}

// 0x0801151C
EC u32 SECTION(".text.game_state_08011510") Selection_GetCurrent(GameStateSelection const & selection)
{
    return selection.current;
}

// 0x08011520
EC u32 SECTION(".text.game_state_08011510") Selection_GetPrevious(GameStateSelection const & selection)
{
    return selection.previous;
}

// 0x08011540
EC void SECTION(".text.game_state_08011540") Selection_SetCurrent(GameStateSelection & selection, u32 value)
{
    selection.current = value;
}

// 0x08011544
EC void SECTION(".text.game_state_08011540") Selection_Commit(GameStateSelection & selection)
{
    selection.previous = selection.current;
    selection.delay = 2;
}

// 0x08012218
EC void SECTION(".text.game_state_08012218") GameScript_NoOp()
{
}

struct GameStateActorEntity : public AActorEntity
{
    GameStateActorEntity();

    virtual void vfunc_3C(u32 mode);
    virtual u32 vfunc_40();
    virtual u32 vfunc_44();
    virtual bool vfunc_48();
    virtual void vfunc_4C();
    virtual void vfunc_50();
    virtual void vfunc_54();
    virtual void vfunc_58();
    virtual void vfunc_5C(u32 value, u32 enabled);
    virtual void vfunc_60();
    virtual void vfunc_64(u32 value_1, u32 value_2);
    virtual void vfunc_68(u32 value);
    virtual void vfunc_6C();
    virtual void vfunc_70();
    virtual void vfunc_74(u32 value);
    virtual u32 vfunc_78();
    virtual void vfunc_7C();
    virtual void vfunc_80();
    virtual u32 vfunc_84();
    virtual u32 vfunc_88();
    virtual u32 vfunc_8C();
    virtual u32 vfunc_90();
    virtual void vfunc_94();
    virtual void vfunc_98();
    virtual void vfunc_9C(u32 value);
    virtual void vfunc_A0();
    virtual void vfunc_A4();
    virtual void vfunc_A8();
    virtual void vfunc_AC();
};

struct GameStateActiveEntity : public AActorEntity
{
    GameStateActiveEntity();

    virtual void vfunc_3C();
    virtual void vfunc_40();
    virtual void vfunc_44();
    virtual void vfunc_48();
    virtual void vfunc_4C();
    virtual void vfunc_50();
    virtual void vfunc_54();
    virtual void vfunc_58();
    virtual bool vfunc_5C();
    virtual void vfunc_60();
    virtual void vfunc_64();
    virtual void vfunc_68();
    virtual void vfunc_6C();
    virtual void vfunc_70();
    virtual void vfunc_74();
    virtual void vfunc_78();
    virtual void vfunc_7C();
};


struct GameStateObject
{
    virtual ~GameStateObject();
    virtual void vfunc_C();
    virtual void vfunc_10();
    virtual u32 vfunc_14();
    virtual void vfunc_18();
    virtual void vfunc_1C();
    virtual void vfunc_20();
    virtual void vfunc_24(u32 value_1, u32 value_2, u32 value_3);
    virtual bool vfunc_28();
    virtual void vfunc_2C();
    virtual void vfunc_30();
    virtual void vfunc_34();
    virtual void vfunc_38(u32 id);
    virtual void vfunc_3C(u32 value);
    virtual GameStateActorEntity * GetActor(u32 actorId);
    virtual void vfunc_44();
    virtual void vfunc_48();
    virtual void vfunc_4C();
    virtual void vfunc_50();
    virtual void vfunc_54();
    virtual void vfunc_58();
    virtual void vfunc_5C();
    virtual void vfunc_60();
    virtual void vfunc_64();
    virtual void vfunc_68();
    virtual void vfunc_6C();
    virtual void vfunc_70();
    virtual void vfunc_74();
    virtual void vfunc_78();
    virtual void vfunc_7C();
    virtual void vfunc_80();
    virtual void vfunc_84();
    virtual void vfunc_88();
    virtual void vfunc_8C();
    virtual void vfunc_90();
    virtual void vfunc_94();
    virtual void vfunc_98();
    virtual void vfunc_9C(u32 value);
    virtual void vfunc_A0(u32 value);
    virtual u32 vfunc_A4();
    virtual u32 vfunc_A8(u32 value);
    virtual void vfunc_AC(u32 index);
    virtual void vfunc_B0();
    virtual void vfunc_B4();
    virtual void vfunc_B8();
    virtual void vfunc_BC();
    virtual void vfunc_C0();
    virtual void vfunc_C4();
    virtual void vfunc_C8();
    virtual void vfunc_CC();
    virtual void vfunc_D0();
    virtual void vfunc_D4();
    virtual void vfunc_D8();
    virtual void vfunc_DC();
    virtual void vfunc_E0();
    virtual void vfunc_E4();
    virtual void vfunc_E8();
    virtual void vfunc_EC();
    virtual void vfunc_F0();
    virtual void vfunc_F4();
    virtual void vfunc_F8();
    virtual void vfunc_FC();
    virtual void vfunc_100();
    virtual void vfunc_104();
    virtual void vfunc_108();
    virtual void vfunc_10C(u32 value);
    virtual void vfunc_110(u32 value);
    virtual void vfunc_114();
    virtual void vfunc_118(u32 value);
    virtual void vfunc_11C();
    virtual void vfunc_120();
    virtual void vfunc_124();
    virtual void vfunc_128();
    virtual void vfunc_12C();
    virtual void vfunc_130();
    virtual void vfunc_134();
    virtual void vfunc_138();
    virtual void vfunc_13C();
    virtual void vfunc_140();
    virtual void vfunc_144();
    virtual void vfunc_148();
    virtual void vfunc_14C();
    virtual void vfunc_150(u32 value);
    virtual void vfunc_154(u32 value);
    virtual void vfunc_158();
    virtual void vfunc_15C();
    virtual void vfunc_160(u32 value_1, u32 value_2, u32 value_3, u32 value_4, u16 value_5);
    virtual void vfunc_164(u32 value);
};

struct GameStateAudioList
{
    void ** begin;
    void ** end;
    void ** capacity;
    u32 reserved;
};

struct GameStateTextPanel
{
    u32 reserved;
    virtual ~GameStateTextPanel();
};

EC void DestroyMessageWindow(void * dialogue, u32 mode);

struct GameStateDialogueOwner
{
    ~GameStateDialogueOwner()
    {
        if (inner != nullptr)
            DestroyMessageWindow(inner, 3);
    }

    void operator=(void * dialogue)
    {
        if (dialogue != inner)
            if (inner != nullptr)
                DestroyMessageWindow(inner, 3);
        inner = dialogue;
    }

    void * Get() const { return inner; }
    void * inner;
};

struct GameStateRuntime
{
    STRUCT_PAD(0x00, 0x10);
    u16 flags;
    STRUCT_PAD(0x12, 0x8C);
    u8 * saveData;
    STRUCT_PAD(0x90, 0x9C);
    u32 pendingCommand;
    STRUCT_PAD(0xA0, 0xA8);
    GameStateObject * scene;
    GameStateDialogueOwner dialogue;
    void * textVariables;
    SmartPtr<GameStateTextPanel> textPanel;
    SmartPtr<GameStateTextPanel> secondaryTextPanel;
    u8 musicPlayer[0x14];
    GameStateAudioList soundPlayers;
    u16 pendingArgument;
    STRUCT_PAD(0xE2, 0xE4);
    GameStateActorEntity * waitActor;
    STRUCT_PAD(0xE8, 0xF4);
    virtual ~GameStateRuntime();

    void * GetTextVariables() const { return textVariables; }
};

struct GameStateScript
{
    u32 vtable;
    GameStateRuntime * runtime;
};

// 0x080125EC
EC void SECTION(".text.game_state_080125EC") GameScript_CloseDialoguePanels(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.textPanel = nullptr;
    runtime.secondaryTextPanel = nullptr;
    runtime.dialogue = nullptr;
    runtime.flags &= ~0x100;
}



struct GameStateShippingView
{
    ShippingBin & GetShippingBin() { return shippingBin; }

    STRUCT_PAD(0x00, 0x54);
    ShippingBin shippingBin;
    u8 reserved[0x1AA8 - 0x54 - sizeof(ShippingBin)];
    u8 earnings[0x34C5 - 0x1AA8];
    u8 doubleShippingValue;
};

struct GameStateMapQuery
{
    struct MapContext
    {
        u32 reserved;
        u32 mapId;
    };

    u32 vtable;
    MapContext * context;
    STRUCT_PAD(0x08, 0x103C);
    u8 forceInterior;
};

EC MapData const * GetMapData(u32 mapId);
EC void func_0809ABD8(void * earnings, u32 amount);

// 0x0801140C
EC bool SECTION(".text.game_state_0801140C") SaveData_CollectShippingValue(GameStateShippingView & save)
{
    ShippingBin & shipping = save.GetShippingBin();
    u32 amount = shipping.GetValueShipped();
    void * earnings = save.earnings;
    func_0809ABD8(earnings, amount);
    u8 & doubled = save.doubleShippingValue;
    if (doubled != 0)
    {
        func_0809ABD8(earnings, amount);
        doubled = 0;
    }
    shipping.ResetValueShipped();
    return amount != 0;
}


// 0x08017C00
EC u32 SECTION(".text.game_state_08017C00") GameMap_IsInteriorOrForced(GameStateMapQuery & query)
{
    if (query.forceInterior != 0)
        goto interior;
    if (!GetMapData(query.context->mapId)->is_interior)
        return 0;
interior:
    return 1;
}


struct GameStateGlobalStatus
{
    u32 GetMapId() const { return mapId; }

    u8 hasReturnLocation;
    u32 mapId;
    u16 mapFlags;
};

struct GameStateScriptEngine : public ScriptEngine
{
    GameStateScriptEngine();

    virtual int OnCall(int id);
};

struct GameStateGlobal
{
    GameStateGlobal();

    STRUCT_PAD(0x000, 0x008);
    GameStateScriptEngine engine;
    STRUCT_PAD(0x364, 0x36C);
    GameStateGlobalStatus status;
};

extern GameStateGlobal * gActiveGameScene;

// 0x08016D9C
EC u32 SECTION(".text.game_state_08016D9C") GameScript_GetReturnMap()
{
    return gActiveGameScene->status.GetMapId();
}

EC void Audio_StopAllPlayers();
EC void AudioPlayer_Stop(void * self);
EC u32 AudioPlayer_IsPlaying(void * self);
EC void func_0803B6B8(void * self, u32 index, char const * text);
EC void ClearMessageWindowSpeakerName(void * self);
EC void ClearMessageWindowPortrait(void * self);
EC void ClearMessageWindowIndicator(void * self);
EC void func_080323C8(AActorEntity & self);
EC void func_08032308(AActorEntity & self, u32 target, i32 speed);
EC void func_0803233C(AActorEntity & self, u32 target, i32 speed);
EC void func_080323E0(AActorEntity & self, u32 value);
EC void CreateSideEntityEffectSprite(void * self);
EC void ClearSideEntityEffectSprite(void * self);
EC void BeginSideEntityTransition(void * self);
EC void CreateSideEntityParticleSprites(void * self);
EC void func_080D7F3C(GameStateRuntime & self);
EC void FormatMessageInteger(i32 value, char * text, u32 digits);
EC void SetMessageWindowTextOptions(void * self, const char * text, u16 speed, const void * substitutions);
EC void func_08032384(AActorEntity & self, u32 value, bool enabled);
EC u32 IsSpriteWorkerOutOfTasks(AActorEntity & self);
EC void CloseMessageWindow(void * self);
EC void func_080D7F88(GameStateRuntime & self);
EC void func_080D7F74(GameStateRuntime & self);
EC void func_080D7F60(GameStateRuntime & self);
EC void SetMessageWindowSpeakerName(void * self, const char * name);
EC void SetMessageWindowPortrait(void * self, u32 value);
EC u32 GetRosterNpcName(void * self, u32 value);
EC void ShowSpriteWorkerEffect(AActorEntity & self, u32 map, i32 x, i32 y);
extern u8 const vtable_unk_080E5E64[];
EC void Scene_DestroyBase(GameStateScript & self, u32 flags);
EC void func_080D7F9C(GameStateRuntime & self, i32 id, u32 value_1, u32 value_2, u32 value_3, u32 value_4);
EC void func_080A05E8(void * self, void * value);
EC void AudioPlayer_StartSong(void * self, u16 amount);
EC void AudioPlayer_StartSongIfStoppedOrChanged(void * self, u16 amount);
EC void AudioPlayer_StartOrResumeSong(void * self, u16 amount);
EC void AudioPlayer_FadeOut(void * self, u32 speed);

EC Bachelorette const * GetRosterBacheloretteByIdConst(void * data, u32 characterId);
EC void SetMessageWindowIndicator(void * dialogue, u32 level);
EC void ConfigureMessageWindowChoices(void * dialogue, char const * first, char const * second, char const * third, char const * fourth, char const * fifth, char const * sixth, void const * substitutions);

// 0x080128D0
EC void SECTION(".text.game_state_080128D0") GameScript_StartChoices(GameStateScript & script, char const * first, char const * second, char const * third, char const * fourth, char const * fifth, char const * sixth)
{
    GameStateRuntime & runtime = *script.runtime;
    if (runtime.dialogue.Get() != nullptr && first != nullptr && second != nullptr)
    {
        if (third == nullptr)
            ConfigureMessageWindowChoices(runtime.dialogue.Get(), first, second, nullptr, nullptr, nullptr, nullptr, runtime.GetTextVariables());
        else if (fourth == nullptr)
            ConfigureMessageWindowChoices(runtime.dialogue.Get(), first, second, third, nullptr, nullptr, nullptr, runtime.GetTextVariables());
        else if (fifth == nullptr)
            ConfigureMessageWindowChoices(runtime.dialogue.Get(), first, second, third, fourth, nullptr, nullptr, runtime.GetTextVariables());
        else if (sixth == nullptr)
            ConfigureMessageWindowChoices(runtime.dialogue.Get(), first, second, third, fourth, fifth, nullptr, runtime.GetTextVariables());
        else
            ConfigureMessageWindowChoices(runtime.dialogue.Get(), first, second, third, fourth, fifth, sixth, runtime.GetTextVariables());
        runtime.pendingCommand = 0x14;
    }
}

// 0x08016AFC
EC i32 SECTION(".text.game_state_08016AFC") GameScript_GetFarmAnimalEventIndex(GameStateScript & script, u32 kind)
{
    GameStateRuntime & runtime = *script.runtime;
    i32 index;
    u32 eventFlag;
    switch (kind)
    {
    case 0:
        if ((runtime.saveData[0x219A] & 2) != 0 || (runtime.saveData[0x21A1] & 4) != 0)
            goto firstAnimal;
    default:
        index = -1;
        break;
    case 4:
    {
        eventFlag = runtime.saveData[0x219C] & 0x10;
        index = -1;
        if (eventFlag != 0)
        {
        firstAnimal:
            index = 0;
        }
        break;
    }
    case 1:
    case 2:
        index = reinterpret_cast<Barn const *>(runtime.saveData + 0x5F0)->method_0800D058();
        break;
    case 3:
        index = reinterpret_cast<Coop const *>(runtime.saveData + 0x410)->GetUnkEnt();
        break;
    }
    return index;
}


// 0x08016834
EC void SECTION(".text.game_state_08016834") GameScript_ClearFarmAnimalActor(GameStateScript & script, u32 kind, u32 index)
{
    GameStateRuntime & runtime = *script.runtime;
    switch (kind)
    {
    case 0:
        runtime.scene->vfunc_3C(0x2C);
        break;
    case 4:
        runtime.scene->vfunc_3C(0x2B);
        break;
    case 1:
    case 2:
        reinterpret_cast<Barn *>(runtime.saveData + 0x5F0)->method_0800D80C(index);
        runtime.scene->vfunc_3C(index + 0x36);
        break;
    case 3:
        reinterpret_cast<Coop *>(runtime.saveData + 0x410)->method_0800CB7C(index);
        runtime.scene->vfunc_3C(index + 0x2E);
        break;
    }
}

// 0x080127A0
EC void SECTION(".text.game_state_080127A0") GameScript_SetDialogueLoveIndicator(GameStateScript & script, u32 characterId)
{
    GameStateRuntime & runtime = *script.runtime;
    if (runtime.dialogue.Get() != nullptr && (i32(u32(runtime.saveData[0x2210]) << 29) < 0) && characterId != 0x1F)
    {
        Bachelorette const * character = GetRosterBacheloretteByIdConst(runtime.saveData + 0x1CD4, characterId);
        if (character != nullptr)
        {
            u32 love = character->GetLove();
            u32 indicator;
            if (love >= 60000)
                indicator = 6;
            else if (love >= 50000)
                indicator = 5;
            else if (love >= 40000)
                indicator = 4;
            else if (love >= 30000)
                indicator = 3;
            else if (love >= 20000)
                indicator = 2;
            else if (love >= 10000)
                indicator = 1;
            else
                indicator = 0;
            SetMessageWindowIndicator(runtime.dialogue.Get(), indicator);
        }
    }
}
struct GameStateVariant
{
    u32 reserved : 2;
    u32 variant : 6;
};

// 0x0801740C
EC u32 SECTION(".text.game_state_0801740C") GameStateVariant_GetColumn(GameStateVariant const & header)
{
    switch (header.variant)
    {
    default:
    case 27: case 31: case 35:
        return 0;
    case 28: case 32: case 36:
        return 1;
    case 29: case 33: case 37:
        return 2;
    case 30: case 34: case 38:
        return 3;
    }
}

// 0x0801746C
EC u32 SECTION(".text.game_state_0801746C") GameStateVariant_GetRow(GameStateVariant const & header)
{
    switch (header.variant)
    {
    default:
    case 27: case 28: case 29: case 30:
        return 0;
    case 31: case 32: case 33: case 34:
        return 1;
    case 35: case 36: case 37: case 38:
        return 2;
    }
}


struct __attribute__((packed, aligned(2))) GameStateHalfCounter
{
    u16 reserved;
    struct __attribute__((packed, aligned(2)))
    {
        u16 reserved : 2;
        u16 counter : 7;
    } progress;
};

// 0x08010F14
EC u32 SECTION(".text.game_state_08010F14") GameProgress_GetHalfCounter(GameStateHalfCounter const & header)
{
    return header.progress.counter;
}

// 0x08011498
EC u32 SECTION(".text.game_state_08011498") GameProgress_RaiseHalfCounter(GameStateHalfCounter & header, u32 minimum)
{
    if (minimum > 99)
        minimum = 99;
    u32 changed;
    if (header.progress.counter >= minimum)
        changed = 0;
    else
    {
        header.progress.counter = minimum;
        changed = 1;
    }
    return changed;
}

// 0x0801412C
EC u32 SECTION(".text.game_state_0801412C") GameScript_BeginPlayerAction48(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    u32 started;
    if (runtime.scene->GetActor(0)->vfunc_48())
    {
        runtime.pendingCommand = 0x19;
        started = 1;
    }
    else
        started = 0;
    return started;
}

// 0x08014BD8
EC void SECTION(".text.game_state_08014BD8") GameScript_SetNonPlayerActorMode(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    u32 actorId = 1;
    GameStateObject * & scene = runtime.scene;
    for (; actorId <= 0x23; actorId++)
    {
        GameStateActorEntity * actor = scene->GetActor(actorId);
        if (actor != nullptr)
            actor->vfunc_3C(3);
    }
}

// 0x08016784
EC u32 SECTION(".text.game_state_08016784") GameScript_FadeMusicIfPlaying(GameStateScript & script)
{
    u8 * music = script.runtime->musicPlayer;
    u32 started;
    if ((u8) AudioPlayer_IsPlaying(music))
    {
        AudioPlayer_FadeOut(music, 5);
        started = 1;
    }
    else
        started = 0;
    return started;
}

// 0x08016D34
EC void SECTION(".text.game_state_08016D34") GameScript_InvokeSceneActorAction(GameStateScript & script, u32 actorId)
{
    script.runtime->scene->vfunc_3C(actorId);
}

// 0x08012454
EC void SECTION(".text.game_state_08012454") GameScript_StopMusic(GameStateScript & script)
{
    AudioPlayer_Stop(script.runtime->musicPlayer);
}

// 0x080124C4
EC void SECTION(".text.game_state_080124C4") GameScript_ResetAudio()
{
    Audio_StopAllPlayers();
}

// 0x0801274C
EC void SECTION(".text.game_state_0801274C") GameScript_ClearDialogueSpeakerName(GameStateScript & script)
{
    void * window = script.runtime->dialogue.Get();
    if (window != nullptr)
        ClearMessageWindowSpeakerName(window);
}

// 0x0801278C
EC void SECTION(".text.game_state_0801278C") GameScript_ClearDialoguePortrait(GameStateScript & script)
{
    void * window = script.runtime->dialogue.Get();
    if (window != nullptr)
        ClearMessageWindowPortrait(window);
}

// 0x0801284C
EC void SECTION(".text.game_state_0801284C") GameScript_ClearDialogueIndicator(GameStateScript & script)
{
    void * window = script.runtime->dialogue.Get();
    if (window != nullptr)
        ClearMessageWindowIndicator(window);
}

// 0x08012ACC
EC void SECTION(".text.game_state_08012ACC") GameScript_SetTextVariable(GameStateScript & script, u32 variableIndex, char const * text)
{
    func_0803B6B8(script.runtime->textVariables, variableIndex, text);
}

// 0x08012BAC
EC void SECTION(".text.game_state_08012BAC") GameScript_SetPendingArgument(GameStateScript & script, u32 value)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.pendingArgument = value;
    runtime.pendingCommand = 0x15;
}

// 0x080142F0
EC void SECTION(".text.game_state_080142F0") GameScript_EnableSaveFlag(GameStateScript & script)
{
    script.runtime->saveData[0x34C4] = 1;
}

// 0x08014304
EC void SECTION(".text.game_state_08014304") GameScript_DisableSaveFlag(GameStateScript & script)
{
    script.runtime->saveData[0x34C4] = 0;
}

// 0x0801468C
EC SECTION(".text.game_state_0801468C") u8 * GameScript_GetSoundPlayers(GameStateScript & script)
{
    return reinterpret_cast<u8 *>(&script.runtime->soundPlayers);
}

// 0x080167CC
EC u32 SECTION(".text.game_state_080167CC") GameScript_IsMusicActive(GameStateScript & script)
{
    return AudioPlayer_IsPlaying(script.runtime->musicPlayer);
}

// 0x08016D80
EC void SECTION(".text.game_state_08016D80") GameScript_ClearReturnLocation()
{
    GameStateGlobalStatus & status = gActiveGameScene->status;
    status.hasReturnLocation = 0;
    status.mapId = 0x234;
}

// 0x08014034
EC void SECTION(".text.game_state_08014034") GameScript_InvokeSceneAction84(GameStateScript & script)
{
    script.runtime->scene->vfunc_84();
}

// 0x0801404C
EC void SECTION(".text.game_state_08014034") GameScript_InvokeSceneAction88(GameStateScript & script)
{
    script.runtime->scene->vfunc_88();
}

// 0x08014064
EC void SECTION(".text.game_state_08014034") GameScript_InvokeSceneAction8C(GameStateScript & script)
{
    script.runtime->scene->vfunc_8C();
}

// 0x0801407C
EC void SECTION(".text.game_state_08014034") GameScript_InvokeSceneAction90(GameStateScript & script)
{
    script.runtime->scene->vfunc_90();
}

// 0x08014094
EC void SECTION(".text.game_state_08014034") GameScript_InvokeSceneAction94(GameStateScript & script)
{
    script.runtime->scene->vfunc_94();
}

// 0x080140AC
EC void SECTION(".text.game_state_08014034") GameScript_InvokeSceneAction98(GameStateScript & script)
{
    script.runtime->scene->vfunc_98();
}

// 0x080140C4
EC void SECTION(".text.game_state_08014034") GameScript_InvokeSceneAction9C(GameStateScript & script, u32 value)
{
    script.runtime->scene->vfunc_9C(value);
}

// 0x080140DC
EC void SECTION(".text.game_state_08014034") GameScript_InvokeSceneActionA0(GameStateScript & script, u32 value)
{
    script.runtime->scene->vfunc_A0(value);
}

// 0x080140F4
EC void SECTION(".text.game_state_08014034") GameScript_InvokeSceneAction80(GameStateScript & script)
{
    script.runtime->scene->vfunc_80();
}

// 0x08014234
EC u32 SECTION(".text.game_state_08014234") GameScript_GetScenePropertyA4(GameStateScript & script)
{
    return script.runtime->scene->vfunc_A4();
}

// 0x0801424C
EC u32 SECTION(".text.game_state_08014234") GameScript_QueryScenePropertyA8(GameStateScript & script, u32 value)
{
    return script.runtime->scene->vfunc_A8(value);
}

// 0x080142B8
EC void SECTION(".text.game_state_080142B8") GameScript_InvokeSceneAction118(GameStateScript & script, u32 value)
{
    script.runtime->scene->vfunc_118(value);
}

// 0x08016BDC
EC void SECTION(".text.game_state_08016BDC") GameScript_InvokeSceneActionFC(GameStateScript & script)
{
    script.runtime->scene->vfunc_FC();
}

// 0x08016BF4
EC void SECTION(".text.game_state_08016BDC") GameScript_InvokeSceneAction100(GameStateScript & script)
{
    script.runtime->scene->vfunc_100();
}

// 0x080114F8
EC void SECTION(".text.game_state_080114F8") SaveProgress_ResetPrimaryFlags(GameStateHeader & header)
{
    header.flagA = 0;
    header.flagB = 1;
    header.flagC = 1;
    header.flagD = 1;
}

// 0x08011524
EC bool SECTION(".text.game_state_08011524") Selection_IsPreviousReady(GameStateSelection const & selection)
{
    return selection.previous != 0x10 && selection.delay == 0;
}

// 0x08011550
EC void SECTION(".text.game_state_08011550") Selection_ClearPrevious(GameStateSelection & selection)
{
    if (selection.previous == selection.current)
        selection.current = 0x10;
    selection.previous = 0x10;
}

// 0x08011568
EC void SECTION(".text.game_state_08011568") Selection_TickDelay(GameStateSelection & selection)
{
    if (selection.current != 0x10 && selection.previous != 0x10 && selection.delay != 0)
        selection.delay--;
}

// 0x080122A0
EC void SECTION(".text.game_state_080122A0") GameScript_StopActorMovement(GameStateScript & script, u32 actorId)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    if (entity != nullptr)
        func_080323C8(*entity);
}

// 0x080142D4
EC void SECTION(".text.game_state_080142D4") GameScript_InvokeSceneAction11C(GameStateScript & script)
{
    script.runtime->scene->vfunc_11C();
}

// 0x08014694
EC void SECTION(".text.game_state_08014694") GameScript_InvokeSceneAction150(GameStateScript & script, u32 value)
{
    script.runtime->scene->vfunc_150(value);
}

// 0x080146B0
EC void SECTION(".text.game_state_080146B0") GameScript_InvokeSceneAction154(GameStateScript & script, u32 value)
{
    script.runtime->scene->vfunc_154(value);
}

// 0x08016BA4
EC void SECTION(".text.game_state_08016BA4") GameScript_InvokeSceneAction10C(GameStateScript & script, u32 value)
{
    script.runtime->scene->vfunc_10C(value);
}

// 0x08016BC0
EC void SECTION(".text.game_state_08016BC0") GameScript_InvokeSceneAction110(GameStateScript & script, u32 value)
{
    script.runtime->scene->vfunc_110(value);
}

// 0x08016C10
EC void SECTION(".text.game_state_08016C10") GameScript_InvokeSceneAction104(GameStateScript & script)
{
    script.runtime->scene->vfunc_104();
}

// 0x08016C2C
EC void SECTION(".text.game_state_08016C2C") GameScript_InvokeSpecialActorActionA(GameStateScript & script)
{
    CreateSideEntityEffectSprite(script.runtime->scene->GetActor(0x5D));
}

// 0x08016C6C
EC void SECTION(".text.game_state_08016C6C") GameScript_InvokeSpecialActorActionB(GameStateScript & script)
{
    ClearSideEntityEffectSprite(script.runtime->scene->GetActor(0x5D));
}

// 0x08016EF0
EC void SECTION(".text.game_state_08016EF0") GameScript_InvokeSceneAction164(GameStateScript & script, u32 value)
{
    script.runtime->scene->vfunc_164(value);
}

// 0x0801410C
EC u32 SECTION(".text.game_state_0801410C") GameScript_GetPlayerProperty40(GameStateScript & script)
{
    return script.runtime->scene->GetActor(0)->vfunc_40();
}

// 0x08014214
EC void SECTION(".text.game_state_08014214") GameScript_InvokePlayerAction54(GameStateScript & script)
{
    script.runtime->scene->GetActor(0)->vfunc_54();
}

// 0x08014D5C
EC u32 SECTION(".text.game_state_08014D5C") GameScript_GetPlayerProperty78(GameStateScript & script)
{
    return script.runtime->scene->GetActor(0)->vfunc_78();
}

// 0x08014D7C
EC u32 SECTION(".text.game_state_08014D7C") GameScript_GetPlayerProperty44(GameStateScript & script)
{
    return script.runtime->scene->GetActor(0)->vfunc_44();
}

// 0x08015950
EC u32 SECTION(".text.game_state_08015950") GameScript_GetPlayerProperty84(GameStateScript & script)
{
    return script.runtime->scene->GetActor(0)->vfunc_84();
}

// 0x08015970
EC u32 SECTION(".text.game_state_08015970") GameScript_GetPlayerProperty88(GameStateScript & script)
{
    return script.runtime->scene->GetActor(0)->vfunc_88();
}

// 0x08015990
EC u32 SECTION(".text.game_state_08015990") GameScript_GetPlayerProperty8C(GameStateScript & script)
{
    return script.runtime->scene->GetActor(0)->vfunc_8C();
}

// 0x080167AC
EC u32 SECTION(".text.game_state_080167AC") GameScript_GetPlayerProperty90(GameStateScript & script)
{
    return script.runtime->scene->GetActor(0)->vfunc_90();
}

// 0x08016CAC
EC void SECTION(".text.game_state_08016CAC") GameScript_InvokePlayerActionA8(GameStateScript & script)
{
    script.runtime->scene->GetActor(0)->vfunc_A8();
}

// 0x08016CCC
EC void SECTION(".text.game_state_08016CCC") GameScript_InvokePlayerActionAC(GameStateScript & script)
{
    script.runtime->scene->GetActor(0)->vfunc_AC();
}

// 0x08016E7C
EC void SECTION(".text.game_state_08016E7C") GameScript_InvokePlayerAction6C(GameStateScript & script)
{
    script.runtime->scene->GetActor(0)->vfunc_6C();
}

// 0x08016C48
EC void SECTION(".text.game_state_08016C48") GameScript_BeginSpecialActorActionA(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    BeginSideEntityTransition(runtime.scene->GetActor(0x5D));
    runtime.pendingCommand = 0x1C;
}

// 0x08016C88
EC void SECTION(".text.game_state_08016C88") GameScript_BeginSpecialActorActionB(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    CreateSideEntityParticleSprites(runtime.scene->GetActor(0x5D));
    runtime.pendingCommand = 0x1B;
}

// 0x0801221C
EC void SECTION(".text.game_state_0801221C") GameScript_ClearActorMap(GameStateScript & script, u32 actorId)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    if (entity != nullptr)
        entity->SetMap(0x234);
}

// 0x080120A4
EC i32 SECTION(".text.game_state_080120A4") GameScript_GetActorX(GameStateScript & script, u32 actorId)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    i32 x = 0;
    if (entity != nullptr)
        x = entity->x_q16;
    return x >> 16;
}

// 0x080120C4
EC i32 SECTION(".text.game_state_080120C4") GameScript_GetActorY(GameStateScript & script, u32 actorId)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    i32 y = 0;
    if (entity != nullptr)
        y = entity->y_q16;
    return y >> 16;
}

// 0x08012114
EC u32 SECTION(".text.game_state_08012114") GameScript_GetActorFacing(GameStateScript & script, u32 actorId)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    u32 facing;
    if (entity != nullptr)
        facing = entity->facing;
    else
        facing = 0;
    return facing;
}

// 0x08012134
EC u32 SECTION(".text.game_state_08012134") GameScript_GetActorMap(GameStateScript & script, u32 actorId)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    u32 map = 0x234;
    if (entity != nullptr)
        map = entity->location_map;
    return map;
}

// 0x08012154
EC void SECTION(".text.game_state_08012154") GameScript_SetActorStateByte(GameStateScript & script, u32 actorId, u32 value)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    if (entity != nullptr)
        entity->unk_21 = value;
}

// 0x08012174
EC void SECTION(".text.game_state_08012174") GameScript_MoveActorX(GameStateScript & script, u32 actorId, u32 target, i32 speed)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    if (entity != nullptr)
        func_08032308(*entity, target, speed);
}

// 0x0801219C
EC void SECTION(".text.game_state_0801219C") GameScript_MoveActorY(GameStateScript & script, u32 actorId, u32 target, i32 speed)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    if (entity != nullptr)
        func_0803233C(*entity, target, speed);
}

// 0x080122BC
EC void SECTION(".text.game_state_080122BC") GameScript_SetActorMovementValue(GameStateScript & script, u32 actorId, u32 value)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    if (entity != nullptr)
        func_080323E0(*entity, value);
}

// 0x080141C4
EC void SECTION(".text.game_state_080141C4") GameScript_BeginPlayerAction4C(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.scene->GetActor(0)->vfunc_4C();
    runtime.pendingCommand = 0x19;
}

// 0x080141EC
EC void SECTION(".text.game_state_080141EC") GameScript_BeginPlayerAction50(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.scene->GetActor(0)->vfunc_50();
    runtime.pendingCommand = 0x19;
}

// 0x08014C0C
EC void SECTION(".text.game_state_08014C0C") GameScript_InvokePlayerAction64(GameStateScript & script, u32 value_1, u32 value_2)
{
    script.runtime->scene->GetActor(0)->vfunc_64(value_1, value_2);
}

// 0x080158F8
EC void SECTION(".text.game_state_080158F8") GameScript_BeginPlayerAction7C(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.scene->GetActor(0)->vfunc_7C();
    runtime.pendingCommand = 0x19;
}

// 0x08016E9C
EC void SECTION(".text.game_state_08016E9C") GameScript_BeginPlayerAction70(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.scene->GetActor(0)->vfunc_70();
    runtime.pendingCommand = 0x19;
}

// 0x0801238C
EC void SECTION(".text.game_state_0801238C") GameScript_InvokeSceneAction24(GameStateScript & script, u32 value_1, u32 value_2, u32 value_3)
{
    script.runtime->scene->vfunc_24(value_1, value_2, value_3);
}

// 0x08012B04
EC void SECTION(".text.game_state_08012B04") GameScript_LoadScript(GameStateScript & script, int scriptId, int argument)
{
    GameStateRuntime & runtime = *script.runtime;
    gActiveGameScene->engine.LoadById(scriptId, argument);
    func_080D7F3C(runtime);
}

// 0x08012AA4
EC void SECTION(".text.game_state_08012AA4") GameScript_SetIntegerTextVariable(GameStateScript & script, u32 variableIndex, i32 value)
{
    GameStateRuntime & runtime = *script.runtime;
    char formattedNumber[50];
    FormatMessageInteger(value, formattedNumber, 0);
    func_0803B6B8(runtime.textVariables, variableIndex, formattedNumber);
}

// 0x08012ADC
EC void SECTION(".text.game_state_08012ADC") GameScript_SetPaddedIntegerTextVariable(GameStateScript & script, u32 variableIndex, i32 value, u32 minimumDigits)
{
    GameStateRuntime & runtime = *script.runtime;
    char formattedNumber[50];
    FormatMessageInteger(value, formattedNumber, minimumDigits);
    func_0803B6B8(runtime.textVariables, variableIndex, formattedNumber);
}

// 0x08016F0C
EC void SECTION(".text.game_state_08016F0C") GameScript_InvokePlayerAction9C(GameStateScript & script, u8 value)
{
    script.runtime->scene->GetActor(0)->vfunc_9C(value);
}

// 0x08014290
EC void SECTION(".text.game_state_08014290") GameScript_ResetActorAction38(GameStateScript & script, u32 actorId)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    if (entity != nullptr)
        entity->vfunc_38(0);
}

// 0x080121C4
EC void SECTION(".text.game_state_080121C4") GameScript_WaitForActor(GameStateScript & script, u32 actorId)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.waitActor = runtime.scene->GetActor(actorId);
    if (runtime.waitActor != nullptr)
        runtime.pendingCommand = 0x16;
}

// 0x080123A4
EC void SECTION(".text.game_state_080123A4") GameScript_WaitForScene(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    if (!runtime.scene->vfunc_28())
        runtime.pendingCommand = 0x17;
}

// 0x080126BC
EC void SECTION(".text.game_state_080126BC") GameScript_StartDialogueWithDefaultSpeed(GameStateScript & script, char const * text)
{
    GameStateRuntime & runtime = *script.runtime;
    if (runtime.dialogue.Get() != nullptr)
    {
        SetMessageWindowTextOptions(runtime.dialogue.Get(), text, 0x40, runtime.textVariables);
        runtime.pendingCommand = 0x11;
    }
}

// 0x08014198
EC bool SECTION(".text.game_state_08014198") GameScript_BeginPlayerAction60(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.scene->GetActor(0)->vfunc_60();
    runtime.pendingCommand = 0x19;
    return true;
}

// 0x080158CC
EC void SECTION(".text.game_state_080158CC") GameScript_BeginPlayerAction80(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.scene->GetActor(0)->vfunc_80();
    runtime.pendingCommand = 0x19;
}

// 0x08016F34
EC void SECTION(".text.game_state_08016F34") GameScript_BeginPlayerAction74(GameStateScript & script, u32 value)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.scene->GetActor(0)->vfunc_74(value);
    runtime.pendingCommand = 0x19;
}

// 0x080121EC
EC void SECTION(".text.game_state_080121EC") GameScript_SetActorAnimation(GameStateScript & script, u32 actorId, u32 animationId)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    if (entity != nullptr && entity->anim_id != animationId)
        entity->SetAnim(animationId);
}

// 0x08012274
EC void SECTION(".text.game_state_08012274") GameScript_SetActorMovementOption(GameStateScript & script, u32 actorId, u32 value, u32 enabled)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    if (entity != nullptr)
        func_08032384(*entity, value, enabled != 0);
}

// 0x08014264
EC void SECTION(".text.game_state_08014264") GameScript_InvokeActorAction38(GameStateScript & script, u32 actorId, u32 value)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    if (entity != nullptr)
        entity->vfunc_38(value);
}

// 0x080120E4
EC void SECTION(".text.game_state_080120E4") GameScript_SetActorFacing(GameStateScript & script, u32 actorId, u32 facing)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    if (entity != nullptr && entity->facing != facing)
        entity->SetAnimFacing(facing);
}

// 0x08012658
EC void SECTION(".text.game_state_08012658") GameScript_ResetDialogue(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    if (runtime.dialogue.Get() != nullptr)
    {
        CloseMessageWindow(runtime.dialogue.Get());
        func_080D7F88(runtime);
        func_080D7F74(runtime);
        func_080D7F60(runtime);
        runtime.pendingCommand = 0xF;
    }
}

// 0x0801268C
EC void SECTION(".text.game_state_0801268C") GameScript_StartDialogue(GameStateScript & script, char const * text, u32 speed)
{
    GameStateRuntime & runtime = *script.runtime;
    if (runtime.dialogue.Get() != nullptr)
    {
        SetMessageWindowTextOptions(runtime.dialogue.Get(), text, (u16) speed, runtime.textVariables);
        runtime.pendingCommand = 0x10;
    }
}

// 0x0801589C
EC void SECTION(".text.game_state_0801589C") GameScript_BeginPlayerAction68(GameStateScript & script, u16 value)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.scene->GetActor(0)->vfunc_68(value);
    runtime.pendingCommand = 0x19;
}

// 0x08014164
EC bool SECTION(".text.game_state_08014164") GameScript_BeginPlayerAction5C(GameStateScript & script, u32 value, u8 enabled)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.scene->GetActor(0)->vfunc_5C(value, enabled);
    runtime.pendingCommand = 0x19;
    return true;
}

// 0x08015920
EC u32 SECTION(".text.game_state_08015920") GameScript_GetFarmActorProperty(GameStateScript & script, u32 actorId)
{
    GameStateRuntime & runtime = *script.runtime;
    if (actorId - 0x24 <= 6)
    {
        AActorEntity * entity = runtime.scene->GetActor(actorId);
        if (entity != nullptr)
            return IsSpriteWorkerOutOfTasks(*entity);
    }
    return 0;
}

// 0x08016EC4
EC void SECTION(".text.game_state_08016EC4") GameScript_InvokeSceneAction160(GameStateScript & script, u32 value_1, u32 value_2, u32 value_3, u32 value_4, u16 value_5)
{
    script.runtime->scene->vfunc_160(value_1, value_2, value_3, value_4, value_5);
}

// 0x08014D30
EC void SECTION(".text.game_state_08014D30") GameScript_RemoveHorse(GameStateScript & script, u32 value)
{
    GameStateRuntime & runtime = *script.runtime;
    if (value == 0)
    {
        runtime.scene->vfunc_3C(0x2C);
        reinterpret_cast<Farm *>(runtime.saveData + 0x14)->RemoveHorse();
    }
}

// 0x080146CC
EC void SECTION(".text.game_state_080146CC") GameScript_BeginEggIncubation(GameStateScript & script, u32 index)
{
    GameStateRuntime & runtime = *script.runtime;
    reinterpret_cast<Coop *>(runtime.saveData + 0x410)->BeginIncubation(index);
    runtime.scene->vfunc_AC(index);
}

// 0x080115A8
EC u32 SECTION(".text.game_state_080115A8") SaveData_GetSize()
{
    return 0x34FC;
}

// 0x08011464
EC u32 SECTION(".text.game_state_08011464") SaveProgress_RaiseWordCounter(GameStateWordHeader & header, u32 value)
{
    if (value > 99)
        value = 99;
    u32 result;
    if (header.counter >= value)
        result = 0;
    else
    {
        header.counter = value;
        result = 1;
    }
    return result;
}

// 0x080114C8
EC u32 SECTION(".text.game_state_080114C8") SaveProgress_RaiseByteCounter(GameStateHeader & header, u32 value)
{
    if (value > 99)
        value = 99;
    u32 result;
    if (header.counter >= value)
        result = 0;
    else
    {
        header.counter = value;
        result = 1;
    }
    return result;
}

// 0x08012720
EC void SECTION(".text.game_state_08012720") GameScript_SetDialogueSpeakerName(GameStateScript & script, char const * speakerName)
{
    GameStateRuntime & runtime = *script.runtime;
    if (runtime.dialogue.Get() != nullptr && (i32(u32(runtime.saveData[0x2210]) << 28) < 0))
        SetMessageWindowSpeakerName(runtime.dialogue.Get(), speakerName);
}

// 0x08012760
EC void SECTION(".text.game_state_08012760") GameScript_SetDialoguePortrait(GameStateScript & script, u32 portraitId)
{
    GameStateRuntime & runtime = *script.runtime;
    if (runtime.dialogue.Get() != nullptr && (i32(u32(runtime.saveData[0x2210]) << 29) < 0))
        SetMessageWindowPortrait(runtime.dialogue.Get(), portraitId);
}

// 0x080126E4
EC void SECTION(".text.game_state_080126E4") GameScript_SetDialogueCharacterName(GameStateScript & script, u32 characterId)
{
    GameStateRuntime & runtime = *script.runtime;
    void * window = runtime.dialogue.Get();
    if (window != nullptr && (i32(u32(runtime.saveData[0x2210]) << 28) < 0))
        SetMessageWindowSpeakerName(window, (char const *) GetRosterNpcName(runtime.saveData + 0x1CD4, characterId));
}

// 0x0801223C
EC void SECTION(".text.game_state_0801223C") GameScript_OffsetActorPosition(GameStateScript & script, u32 actorId, i32 deltaX, i32 deltaY)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    if (entity != nullptr)
    {
        i32 currentX = reinterpret_cast<i16 const *>(&entity->x_q16)[1];
        i32 currentY = reinterpret_cast<i16 const *>(&entity->y_q16)[1];
        currentX += deltaX;
        currentY += deltaY;
        ShowSpriteWorkerEffect(*entity, reinterpret_cast<u8 *>(entity)[0x30], currentX, currentY);
    }
}

// 0x08016D48
EC void SECTION(".text.game_state_08016D48") GameScript_SaveReturnLocation(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    AActorEntity * entity = runtime.scene->GetActor(0);
    GameStateGlobalStatus & status = gActiveGameScene->status;
    u16 map = entity->location_map;
    u8 * state = runtime.saveData;
    status.hasReturnLocation = 1;
    status.mapId = map;
    status.mapFlags = *reinterpret_cast<u16 *>(state + 0x12);
}

// 0x08016F60
EC void SECTION(".text.game_state_08016F60") GameScript_UpdateActiveActors(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    u32 actorId = 0x2E;
    GameStateObject * & object = runtime.scene;
    for (; actorId <= 0x45; actorId++)
    {
        GameStateActiveEntity * entity = reinterpret_cast<GameStateActiveEntity *>(object->GetActor(actorId));
        if (entity != nullptr && entity->vfunc_5C())
            entity->vfunc_7C();
    }
}

// 0x08011FE8
EC SECTION(".text.game_state_08011FE8") void GameScript_Destroy(GameStateScript & script, u32 flags)
{
    script.vtable = reinterpret_cast<u32>(vtable_unk_080E5E64);
    gActiveGameScene->engine.ClearUnk();
    delete script.runtime;
    Scene_DestroyBase(script, flags);
}

// 0x08012064
EC SECTION(".text.game_state_08012064") void GameScript_StartRuntimeOperation(GameStateScript & script, i32 id, u32 value_2, u32 value_3, u32 value_4)
{
    GameStateRuntime & runtime = *script.runtime;
    u32 value_1 = runtime.scene->vfunc_14();
    func_080D7F9C(runtime, id, value_1, value_2, value_3, value_4);
}

// 0x08016CEC
EC SECTION(".text.game_state_08016CEC") void GameScript_ActivateActor(GameStateScript & script, u32 actorId)
{
    GameStateObject * & object = script.runtime->scene;
    object->vfunc_38(actorId);
    AActorEntity * entity = object->GetActor(actorId);
    if (entity != nullptr && !entity->vfunc_28())
        entity->vfunc_20();
}

// 0x080167DC
EC SECTION(".text.game_state_080167DC") void GameScript_RefreshActor23(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    func_080A05E8(runtime.saveData + 0x1CD4, runtime.saveData + 0x11);
    GameStateObject * & object = runtime.scene;
    object->vfunc_38(0x23);
    AActorEntity * entity = object->GetActor(0x23);
    if (entity != nullptr && !entity->vfunc_28())
        entity->vfunc_20();
}

// 0x08012464
EC SECTION(".text.game_state_08012464") void GameScript_PlaySound(GameStateScript & script, i32 mode, u32 songId)
{
    GameStateAudioList & players = script.runtime->soundPlayers;
    void ** cursor = players.begin;
    void ** end = players.end;
    void * player;
    if (cursor != end)
    {
        do
        {
            if (!(u8) AudioPlayer_IsPlaying(*cursor))
            {
                player = *cursor;
                goto play;
            }
            cursor++;
        } while (cursor != end);
    }
    player = end[-1];
play:
    u16 song = songId;
    switch (mode)
    {
    case 0:
        AudioPlayer_StartSong(player, song);
        break;
    case 1:
        AudioPlayer_StartSongIfStoppedOrChanged(player, song);
        break;
    case 2:
        AudioPlayer_StartOrResumeSong(player, song);
        break;
    }
}
