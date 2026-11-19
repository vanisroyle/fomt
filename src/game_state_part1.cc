#include "prelude.h"

#include "entity_actor.hh"
#include "farm.hh"
#include "coop.hh"
#include "script_engine.hh"
#include "scene.hh"

#include <stdlib.h>

struct PACKED GameStateHeader
{
    u8 flag_0 : 1;
    u8 flag_1 : 1;
    u8 flag_2 : 1;
    u8 flag_3 : 1;
    u8 flag_4 : 1;
    u8 unused_0 : 3;
    u8 unused_1[2];
    u8 unused_3 : 1;
    u8 value_3 : 6;
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
    u32 value : 5;
};

EC u32 SECTION(".text.game_state_08010F0C") func_08010F0C(GameStateWordHeader const & self)
{
    return self.value;
}

EC u32 SECTION(".text.game_state_08011588") func_08011588(void const * data, u32 length)
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

EC int SECTION(".text.game_state_08010348") func_08010348()
{
    int value;
    do
    {
        value = rand();
    } while (value == 0);
    return value;
}

EC u32 SECTION(".text.game_state_08010E48") func_08010E48(GameStateHeader const & self)
{
    return self.flag_0;
}

EC u32 SECTION(".text.game_state_08010E48") func_08010E50(GameStateHeader const & self)
{
    return self.flag_1;
}

EC u32 SECTION(".text.game_state_08010E48") func_08010E58(GameStateHeader const & self)
{
    return self.flag_2;
}

EC u32 SECTION(".text.game_state_08010E48") func_08010E60(GameStateHeader const & self)
{
    return self.flag_3;
}

EC u32 SECTION(".text.game_state_08010F04") func_08010F04(GameStateHeader const & self)
{
    return self.flag_4;
}

EC u32 SECTION(".text.game_state_08010F1C") func_08010F1C(GameStateHeader const & self)
{
    return self.value_3;
}

EC void SECTION(".text.game_state_08010F1C") func_08010F24(GameStateHeader & self)
{
    self.flag_0 = 1;
}

EC void SECTION(".text.game_state_08010F1C") func_08010F30(GameStateHeader & self)
{
    self.flag_1 = 1;
}

EC void SECTION(".text.game_state_08010F1C") func_08010F3C(GameStateHeader & self)
{
    self.flag_2 = 1;
}

EC void SECTION(".text.game_state_08010F1C") func_08010F48(GameStateHeader & self)
{
    self.flag_3 = 1;
}

EC void SECTION(".text.game_state_08011458") func_08011458(GameStateHeader & self)
{
    self.flag_4 = 1;
}

EC void SECTION(".text.game_state_08011510") func_08011510(GameStateSelection & self)
{
    self.current = 0x10;
    self.previous = 0x10;
    self.delay = 0;
}

EC u32 SECTION(".text.game_state_08011510") func_0801151C(GameStateSelection const & self)
{
    return self.current;
}

EC u32 SECTION(".text.game_state_08011510") func_08011520(GameStateSelection const & self)
{
    return self.previous;
}

EC void SECTION(".text.game_state_08011540") func_08011540(GameStateSelection & self, u32 value)
{
    self.current = value;
}

EC void SECTION(".text.game_state_08011540") func_08011544(GameStateSelection & self)
{
    self.previous = self.current;
    self.delay = 2;
}

EC void SECTION(".text.game_state_08012218") func_08012218()
{
}

struct GameStateActorEntity : public AActorEntity
{
    GameStateActorEntity();

    virtual void vfunc_3C();
    virtual u32 vfunc_40();
    virtual u32 vfunc_44();
    virtual void vfunc_48();
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
    virtual GameStateActorEntity * vfunc_40(u32 id);
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
    u32 unk_0C;
};

struct GameStateRuntime
{
    STRUCT_PAD(0x00, 0x8C);
    u8 * state;
    STRUCT_PAD(0x90, 0x9C);
    u32 command;
    STRUCT_PAD(0xA0, 0xA8);
    GameStateObject * object;
    void * actor;
    void * text;
    STRUCT_PAD(0xB4, 0xBC);
    u8 music[0x14];
    GameStateAudioList audio;
    u16 unk_E0;
    STRUCT_PAD(0xE2, 0xE4);
    GameStateActorEntity * target;
    STRUCT_PAD(0xE8, 0xF4);
    virtual ~GameStateRuntime();
};

struct GameStateScript
{
    u32 unk_00;
    GameStateRuntime * runtime;
};


struct GameStateGlobalStatus
{
    u32 GetValue() const { return unk_04; }

    u8 unk_00;
    u32 unk_04;
    u16 unk_08;
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

extern GameStateGlobal * gUnk_0300040C;

EC u32 SECTION(".text.game_state_08016D9C") func_08016D9C()
{
    return gUnk_0300040C->status.GetValue();
}

EC void func_08008DE8();
EC void func_08008BE0(void * self);
EC u32 func_08008CD0(void * self);
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
EC void func_0804EC84(i32 value, char * text, u32 digits);
EC void SetMessageWindowTextOptions(void * self, const char * text, u16 speed, const void * substitutions);
EC void func_08032384(AActorEntity & self, u32 value, bool enabled);
EC u32 func_08033B24(AActorEntity & self);
EC void CloseMessageWindow(void * self);
EC void func_080D7F88(GameStateRuntime & self);
EC void func_080D7F74(GameStateRuntime & self);
EC void func_080D7F60(GameStateRuntime & self);
EC void SetMessageWindowSpeakerName(void * self, const char * name);
EC void SetMessageWindowPortrait(void * self, u32 value);
EC u32 func_0809FE3C(void * self, u32 value);
EC void ShowSpriteWorkerEffect(AActorEntity & self, u32 map, i32 x, i32 y);
extern u8 const vtable_unk_080E5E64[];
EC void func_080007EC(GameStateScript & self, u32 flags);
EC void func_080D7F9C(GameStateRuntime & self, i32 id, u32 value_1, u32 value_2, u32 value_3, u32 value_4);
EC void func_080A05E8(void * self, void * value);
EC void func_08008B6C(void * self, u16 amount);
EC void func_08008B88(void * self, u16 amount);
EC void func_08008BB0(void * self, u16 amount);

EC void SECTION(".text.game_state_08012454") func_08012454(GameStateScript & self)
{
    func_08008BE0(self.runtime->music);
}

EC void SECTION(".text.game_state_080124C4") func_080124C4()
{
    func_08008DE8();
}

EC void SECTION(".text.game_state_0801274C") func_0801274C(GameStateScript & self)
{
    void * actor = self.runtime->actor;
    if (actor != nullptr)
        ClearMessageWindowSpeakerName(actor);
}

EC void SECTION(".text.game_state_0801278C") func_0801278C(GameStateScript & self)
{
    void * actor = self.runtime->actor;
    if (actor != nullptr)
        ClearMessageWindowPortrait(actor);
}

EC void SECTION(".text.game_state_0801284C") func_0801284C(GameStateScript & self)
{
    void * actor = self.runtime->actor;
    if (actor != nullptr)
        ClearMessageWindowIndicator(actor);
}

EC void SECTION(".text.game_state_08012ACC") func_08012ACC(GameStateScript & self, u32 index, char const * text)
{
    func_0803B6B8(self.runtime->text, index, text);
}

EC void SECTION(".text.game_state_08012BAC") func_08012BAC(GameStateScript & self, u32 value)
{
    GameStateRuntime & runtime = *self.runtime;
    runtime.unk_E0 = value;
    runtime.command = 0x15;
}

EC void SECTION(".text.game_state_080142F0") func_080142F0(GameStateScript & self)
{
    self.runtime->state[0x34C4] = 1;
}

EC void SECTION(".text.game_state_08014304") func_08014304(GameStateScript & self)
{
    self.runtime->state[0x34C4] = 0;
}

EC SECTION(".text.game_state_0801468C") u8 * func_0801468C(GameStateScript & self)
{
    return reinterpret_cast<u8 *>(&self.runtime->audio);
}

EC u32 SECTION(".text.game_state_080167CC") func_080167CC(GameStateScript & self)
{
    return func_08008CD0(self.runtime->music);
}

EC void SECTION(".text.game_state_08016D80") func_08016D80()
{
    GameStateGlobalStatus & status = gUnk_0300040C->status;
    status.unk_00 = 0;
    status.unk_04 = 0x234;
}

EC void SECTION(".text.game_state_08014034") func_08014034(GameStateScript & self)
{
    self.runtime->object->vfunc_84();
}

EC void SECTION(".text.game_state_08014034") func_0801404C(GameStateScript & self)
{
    self.runtime->object->vfunc_88();
}

EC void SECTION(".text.game_state_08014034") func_08014064(GameStateScript & self)
{
    self.runtime->object->vfunc_8C();
}

EC void SECTION(".text.game_state_08014034") func_0801407C(GameStateScript & self)
{
    self.runtime->object->vfunc_90();
}

EC void SECTION(".text.game_state_08014034") func_08014094(GameStateScript & self)
{
    self.runtime->object->vfunc_94();
}

EC void SECTION(".text.game_state_08014034") func_080140AC(GameStateScript & self)
{
    self.runtime->object->vfunc_98();
}

EC void SECTION(".text.game_state_08014034") func_080140C4(GameStateScript & self, u32 value)
{
    self.runtime->object->vfunc_9C(value);
}

EC void SECTION(".text.game_state_08014034") func_080140DC(GameStateScript & self, u32 value)
{
    self.runtime->object->vfunc_A0(value);
}

EC void SECTION(".text.game_state_08014034") func_080140F4(GameStateScript & self)
{
    self.runtime->object->vfunc_80();
}

EC u32 SECTION(".text.game_state_08014234") func_08014234(GameStateScript & self)
{
    return self.runtime->object->vfunc_A4();
}

EC u32 SECTION(".text.game_state_08014234") func_0801424C(GameStateScript & self, u32 value)
{
    return self.runtime->object->vfunc_A8(value);
}

EC void SECTION(".text.game_state_080142B8") func_080142B8(GameStateScript & self, u32 value)
{
    self.runtime->object->vfunc_118(value);
}

EC void SECTION(".text.game_state_08016BDC") func_08016BDC(GameStateScript & self)
{
    self.runtime->object->vfunc_FC();
}

EC void SECTION(".text.game_state_08016BDC") func_08016BF4(GameStateScript & self)
{
    self.runtime->object->vfunc_100();
}

EC void SECTION(".text.game_state_080114F8") func_080114F8(GameStateHeader & self)
{
    self.flag_0 = 0;
    self.flag_1 = 1;
    self.flag_2 = 1;
    self.flag_3 = 1;
}

EC bool SECTION(".text.game_state_08011524") func_08011524(GameStateSelection const & self)
{
    return self.previous != 0x10 && self.delay == 0;
}

EC void SECTION(".text.game_state_08011550") func_08011550(GameStateSelection & self)
{
    if (self.previous == self.current)
        self.current = 0x10;
    self.previous = 0x10;
}

EC void SECTION(".text.game_state_08011568") func_08011568(GameStateSelection & self)
{
    if (self.current != 0x10 && self.previous != 0x10 && self.delay != 0)
        self.delay--;
}

EC void SECTION(".text.game_state_080122A0") func_080122A0(GameStateScript & self, u32 id)
{
    AActorEntity * entity = self.runtime->object->vfunc_40(id);
    if (entity != nullptr)
        func_080323C8(*entity);
}

EC void SECTION(".text.game_state_080142D4") func_080142D4(GameStateScript & self)
{
    self.runtime->object->vfunc_11C();
}

EC void SECTION(".text.game_state_08014694") func_08014694(GameStateScript & self, u32 value)
{
    self.runtime->object->vfunc_150(value);
}

EC void SECTION(".text.game_state_080146B0") func_080146B0(GameStateScript & self, u32 value)
{
    self.runtime->object->vfunc_154(value);
}

EC void SECTION(".text.game_state_08016BA4") func_08016BA4(GameStateScript & self, u32 value)
{
    self.runtime->object->vfunc_10C(value);
}

EC void SECTION(".text.game_state_08016BC0") func_08016BC0(GameStateScript & self, u32 value)
{
    self.runtime->object->vfunc_110(value);
}

EC void SECTION(".text.game_state_08016C10") func_08016C10(GameStateScript & self)
{
    self.runtime->object->vfunc_104();
}

EC void SECTION(".text.game_state_08016C2C") func_08016C2C(GameStateScript & self)
{
    CreateSideEntityEffectSprite(self.runtime->object->vfunc_40(0x5D));
}

EC void SECTION(".text.game_state_08016C6C") func_08016C6C(GameStateScript & self)
{
    ClearSideEntityEffectSprite(self.runtime->object->vfunc_40(0x5D));
}

EC void SECTION(".text.game_state_08016EF0") func_08016EF0(GameStateScript & self, u32 value)
{
    self.runtime->object->vfunc_164(value);
}

EC u32 SECTION(".text.game_state_0801410C") func_0801410C(GameStateScript & self)
{
    return self.runtime->object->vfunc_40(0)->vfunc_40();
}

EC void SECTION(".text.game_state_08014214") func_08014214(GameStateScript & self)
{
    self.runtime->object->vfunc_40(0)->vfunc_54();
}

EC u32 SECTION(".text.game_state_08014D5C") func_08014D5C(GameStateScript & self)
{
    return self.runtime->object->vfunc_40(0)->vfunc_78();
}

EC u32 SECTION(".text.game_state_08014D7C") func_08014D7C(GameStateScript & self)
{
    return self.runtime->object->vfunc_40(0)->vfunc_44();
}

EC u32 SECTION(".text.game_state_08015950") func_08015950(GameStateScript & self)
{
    return self.runtime->object->vfunc_40(0)->vfunc_84();
}

EC u32 SECTION(".text.game_state_08015970") func_08015970(GameStateScript & self)
{
    return self.runtime->object->vfunc_40(0)->vfunc_88();
}

EC u32 SECTION(".text.game_state_08015990") func_08015990(GameStateScript & self)
{
    return self.runtime->object->vfunc_40(0)->vfunc_8C();
}

EC u32 SECTION(".text.game_state_080167AC") func_080167AC(GameStateScript & self)
{
    return self.runtime->object->vfunc_40(0)->vfunc_90();
}

EC void SECTION(".text.game_state_08016CAC") func_08016CAC(GameStateScript & self)
{
    self.runtime->object->vfunc_40(0)->vfunc_A8();
}

EC void SECTION(".text.game_state_08016CCC") func_08016CCC(GameStateScript & self)
{
    self.runtime->object->vfunc_40(0)->vfunc_AC();
}

EC void SECTION(".text.game_state_08016E7C") func_08016E7C(GameStateScript & self)
{
    self.runtime->object->vfunc_40(0)->vfunc_6C();
}

EC void SECTION(".text.game_state_08016C48") func_08016C48(GameStateScript & self)
{
    GameStateRuntime & runtime = *self.runtime;
    BeginSideEntityTransition(runtime.object->vfunc_40(0x5D));
    runtime.command = 0x1C;
}

EC void SECTION(".text.game_state_08016C88") func_08016C88(GameStateScript & self)
{
    GameStateRuntime & runtime = *self.runtime;
    CreateSideEntityParticleSprites(runtime.object->vfunc_40(0x5D));
    runtime.command = 0x1B;
}

EC void SECTION(".text.game_state_0801221C") func_0801221C(GameStateScript & self, u32 id)
{
    AActorEntity * entity = self.runtime->object->vfunc_40(id);
    if (entity != nullptr)
        entity->SetMap(0x234);
}

EC i32 SECTION(".text.game_state_080120A4") func_080120A4(GameStateScript & self, u32 id)
{
    AActorEntity * entity = self.runtime->object->vfunc_40(id);
    i32 x = 0;
    if (entity != nullptr)
        x = entity->x_q16;
    return x >> 16;
}

EC i32 SECTION(".text.game_state_080120C4") func_080120C4(GameStateScript & self, u32 id)
{
    AActorEntity * entity = self.runtime->object->vfunc_40(id);
    i32 y = 0;
    if (entity != nullptr)
        y = entity->y_q16;
    return y >> 16;
}

EC u32 SECTION(".text.game_state_08012114") func_08012114(GameStateScript & self, u32 id)
{
    AActorEntity * entity = self.runtime->object->vfunc_40(id);
    u32 facing;
    if (entity != nullptr)
        facing = entity->facing;
    else
        facing = 0;
    return facing;
}

EC u32 SECTION(".text.game_state_08012134") func_08012134(GameStateScript & self, u32 id)
{
    AActorEntity * entity = self.runtime->object->vfunc_40(id);
    u32 map = 0x234;
    if (entity != nullptr)
        map = entity->location_map;
    return map;
}

EC void SECTION(".text.game_state_08012154") func_08012154(GameStateScript & self, u32 id, u32 value)
{
    AActorEntity * entity = self.runtime->object->vfunc_40(id);
    if (entity != nullptr)
        entity->unk_21 = value;
}

EC void SECTION(".text.game_state_08012174") func_08012174(GameStateScript & self, u32 id, u32 target, i32 speed)
{
    AActorEntity * entity = self.runtime->object->vfunc_40(id);
    if (entity != nullptr)
        func_08032308(*entity, target, speed);
}

EC void SECTION(".text.game_state_0801219C") func_0801219C(GameStateScript & self, u32 id, u32 target, i32 speed)
{
    AActorEntity * entity = self.runtime->object->vfunc_40(id);
    if (entity != nullptr)
        func_0803233C(*entity, target, speed);
}

EC void SECTION(".text.game_state_080122BC") func_080122BC(GameStateScript & self, u32 id, u32 value)
{
    AActorEntity * entity = self.runtime->object->vfunc_40(id);
    if (entity != nullptr)
        func_080323E0(*entity, value);
}

EC void SECTION(".text.game_state_080141C4") func_080141C4(GameStateScript & self)
{
    GameStateRuntime & runtime = *self.runtime;
    runtime.object->vfunc_40(0)->vfunc_4C();
    runtime.command = 0x19;
}

EC void SECTION(".text.game_state_080141EC") func_080141EC(GameStateScript & self)
{
    GameStateRuntime & runtime = *self.runtime;
    runtime.object->vfunc_40(0)->vfunc_50();
    runtime.command = 0x19;
}

EC void SECTION(".text.game_state_08014C0C") func_08014C0C(GameStateScript & self, u32 value_1, u32 value_2)
{
    self.runtime->object->vfunc_40(0)->vfunc_64(value_1, value_2);
}

EC void SECTION(".text.game_state_080158F8") func_080158F8(GameStateScript & self)
{
    GameStateRuntime & runtime = *self.runtime;
    runtime.object->vfunc_40(0)->vfunc_7C();
    runtime.command = 0x19;
}

EC void SECTION(".text.game_state_08016E9C") func_08016E9C(GameStateScript & self)
{
    GameStateRuntime & runtime = *self.runtime;
    runtime.object->vfunc_40(0)->vfunc_70();
    runtime.command = 0x19;
}

EC void SECTION(".text.game_state_0801238C") func_0801238C(GameStateScript & self, u32 value_1, u32 value_2, u32 value_3)
{
    self.runtime->object->vfunc_24(value_1, value_2, value_3);
}

EC void SECTION(".text.game_state_08012B04") func_08012B04(GameStateScript & self, int id, int value)
{
    GameStateRuntime & runtime = *self.runtime;
    gUnk_0300040C->engine.LoadById(id, value);
    func_080D7F3C(runtime);
}

EC void SECTION(".text.game_state_08012AA4") func_08012AA4(GameStateScript & self, u32 index, i32 value)
{
    GameStateRuntime & runtime = *self.runtime;
    char text[50];
    func_0804EC84(value, text, 0);
    func_0803B6B8(runtime.text, index, text);
}

EC void SECTION(".text.game_state_08012ADC") func_08012ADC(GameStateScript & self, u32 index, i32 value, u32 digits)
{
    GameStateRuntime & runtime = *self.runtime;
    char text[50];
    func_0804EC84(value, text, digits);
    func_0803B6B8(runtime.text, index, text);
}

EC void SECTION(".text.game_state_08016F0C") func_08016F0C(GameStateScript & self, u8 value)
{
    self.runtime->object->vfunc_40(0)->vfunc_9C(value);
}

EC void SECTION(".text.game_state_08014290") func_08014290(GameStateScript & self, u32 id)
{
    AActorEntity * entity = self.runtime->object->vfunc_40(id);
    if (entity != nullptr)
        entity->vfunc_38(0);
}

EC void SECTION(".text.game_state_080121C4") func_080121C4(GameStateScript & self, u32 id)
{
    GameStateRuntime & runtime = *self.runtime;
    runtime.target = runtime.object->vfunc_40(id);
    if (runtime.target != nullptr)
        runtime.command = 0x16;
}

EC void SECTION(".text.game_state_080123A4") func_080123A4(GameStateScript & self)
{
    GameStateRuntime & runtime = *self.runtime;
    if (!runtime.object->vfunc_28())
        runtime.command = 0x17;
}

EC void SECTION(".text.game_state_080126BC") func_080126BC(GameStateScript & self, u32 value)
{
    GameStateRuntime & runtime = *self.runtime;
    if (runtime.actor != nullptr)
    {
        SetMessageWindowTextOptions(runtime.actor, (const char *) value, 0x40, runtime.text);
        runtime.command = 0x11;
    }
}

EC bool SECTION(".text.game_state_08014198") func_08014198(GameStateScript & self)
{
    GameStateRuntime & runtime = *self.runtime;
    runtime.object->vfunc_40(0)->vfunc_60();
    runtime.command = 0x19;
    return true;
}

EC void SECTION(".text.game_state_080158CC") func_080158CC(GameStateScript & self)
{
    GameStateRuntime & runtime = *self.runtime;
    runtime.object->vfunc_40(0)->vfunc_80();
    runtime.command = 0x19;
}

EC void SECTION(".text.game_state_08016F34") func_08016F34(GameStateScript & self, u32 value)
{
    GameStateRuntime & runtime = *self.runtime;
    runtime.object->vfunc_40(0)->vfunc_74(value);
    runtime.command = 0x19;
}

EC void SECTION(".text.game_state_080121EC") func_080121EC(GameStateScript & self, u32 id, u32 anim)
{
    AActorEntity * entity = self.runtime->object->vfunc_40(id);
    if (entity != nullptr && entity->anim_id != anim)
        entity->SetAnim(anim);
}

EC void SECTION(".text.game_state_08012274") func_08012274(GameStateScript & self, u32 id, u32 value, u32 enabled)
{
    AActorEntity * entity = self.runtime->object->vfunc_40(id);
    if (entity != nullptr)
        func_08032384(*entity, value, enabled != 0);
}

EC void SECTION(".text.game_state_08014264") func_08014264(GameStateScript & self, u32 id, u32 value)
{
    AActorEntity * entity = self.runtime->object->vfunc_40(id);
    if (entity != nullptr)
        entity->vfunc_38(value);
}

EC void SECTION(".text.game_state_080120E4") func_080120E4(GameStateScript & self, u32 id, u32 facing)
{
    AActorEntity * entity = self.runtime->object->vfunc_40(id);
    if (entity != nullptr && entity->facing != facing)
        entity->SetAnimFacing(facing);
}

EC void SECTION(".text.game_state_08012658") func_08012658(GameStateScript & self)
{
    GameStateRuntime & runtime = *self.runtime;
    if (runtime.actor != nullptr)
    {
        CloseMessageWindow(runtime.actor);
        func_080D7F88(runtime);
        func_080D7F74(runtime);
        func_080D7F60(runtime);
        runtime.command = 0xF;
    }
}

EC void SECTION(".text.game_state_0801268C") func_0801268C(GameStateScript & self, u32 value, u32 flags)
{
    GameStateRuntime & runtime = *self.runtime;
    if (runtime.actor != nullptr)
    {
        SetMessageWindowTextOptions(runtime.actor, (const char *) value, (u16) flags, runtime.text);
        runtime.command = 0x10;
    }
}

EC void SECTION(".text.game_state_0801589C") func_0801589C(GameStateScript & self, u16 value)
{
    GameStateRuntime & runtime = *self.runtime;
    runtime.object->vfunc_40(0)->vfunc_68(value);
    runtime.command = 0x19;
}

EC bool SECTION(".text.game_state_08014164") func_08014164(GameStateScript & self, u32 value, u8 enabled)
{
    GameStateRuntime & runtime = *self.runtime;
    runtime.object->vfunc_40(0)->vfunc_5C(value, enabled);
    runtime.command = 0x19;
    return true;
}

EC u32 SECTION(".text.game_state_08015920") func_08015920(GameStateScript & self, u32 id)
{
    GameStateRuntime & runtime = *self.runtime;
    if (id - 0x24 <= 6)
    {
        AActorEntity * entity = runtime.object->vfunc_40(id);
        if (entity != nullptr)
            return func_08033B24(*entity);
    }
    return 0;
}

EC void SECTION(".text.game_state_08016EC4") func_08016EC4(GameStateScript & self, u32 value_1, u32 value_2, u32 value_3, u32 value_4, u16 value_5)
{
    self.runtime->object->vfunc_160(value_1, value_2, value_3, value_4, value_5);
}

EC void SECTION(".text.game_state_08014D30") func_08014D30(GameStateScript & self, u32 value)
{
    GameStateRuntime & runtime = *self.runtime;
    if (value == 0)
    {
        runtime.object->vfunc_3C(0x2C);
        reinterpret_cast<Farm *>(runtime.state + 0x14)->RemoveHorse();
    }
}

EC void SECTION(".text.game_state_080146CC") func_080146CC(GameStateScript & self, u32 index)
{
    GameStateRuntime & runtime = *self.runtime;
    reinterpret_cast<Coop *>(runtime.state + 0x410)->BeginIncubation(index);
    runtime.object->vfunc_AC(index);
}

EC u32 SECTION(".text.game_state_080115A8") func_080115A8()
{
    return 0x34FC;
}

EC u32 SECTION(".text.game_state_08011464") func_08011464(GameStateWordHeader & self, u32 value)
{
    if (value > 99)
        value = 99;
    u32 result;
    if (self.value >= value)
        result = 0;
    else
    {
        self.value = value;
        result = 1;
    }
    return result;
}

EC u32 SECTION(".text.game_state_080114C8") func_080114C8(GameStateHeader & self, u32 value)
{
    if (value > 99)
        value = 99;
    u32 result;
    if (self.value_3 >= value)
        result = 0;
    else
    {
        self.value_3 = value;
        result = 1;
    }
    return result;
}

EC void SECTION(".text.game_state_08012720") func_08012720(GameStateScript & self, u32 value)
{
    GameStateRuntime & runtime = *self.runtime;
    if (runtime.actor != nullptr && (i32(u32(runtime.state[0x2210]) << 28) < 0))
        SetMessageWindowSpeakerName(runtime.actor, (const char *) value);
}

EC void SECTION(".text.game_state_08012760") func_08012760(GameStateScript & self, u32 value)
{
    GameStateRuntime & runtime = *self.runtime;
    if (runtime.actor != nullptr && (i32(u32(runtime.state[0x2210]) << 29) < 0))
        SetMessageWindowPortrait(runtime.actor, value);
}

EC void SECTION(".text.game_state_080126E4") func_080126E4(GameStateScript & self, u32 value)
{
    GameStateRuntime & runtime = *self.runtime;
    void * actor = runtime.actor;
    if (actor != nullptr && (i32(u32(runtime.state[0x2210]) << 28) < 0))
        SetMessageWindowSpeakerName(actor, (const char *) func_0809FE3C(runtime.state + 0x1CD4, value));
}

EC void SECTION(".text.game_state_0801223C") func_0801223C(GameStateScript & self, u32 id, i32 x, i32 y)
{
    AActorEntity * entity = self.runtime->object->vfunc_40(id);
    if (entity != nullptr)
    {
        i32 current_x = reinterpret_cast<i16 const *>(&entity->x_q16)[1];
        i32 current_y = reinterpret_cast<i16 const *>(&entity->y_q16)[1];
        current_x += x;
        current_y += y;
        ShowSpriteWorkerEffect(*entity, reinterpret_cast<u8 *>(entity)[0x30], current_x, current_y);
    }
}

EC void SECTION(".text.game_state_08016D48") func_08016D48(GameStateScript & self)
{
    GameStateRuntime & runtime = *self.runtime;
    AActorEntity * entity = runtime.object->vfunc_40(0);
    GameStateGlobalStatus & status = gUnk_0300040C->status;
    u16 map = entity->location_map;
    u8 * state = runtime.state;
    status.unk_00 = 1;
    status.unk_04 = map;
    status.unk_08 = *reinterpret_cast<u16 *>(state + 0x12);
}

EC void SECTION(".text.game_state_08016F60") func_08016F60(GameStateScript & self)
{
    GameStateRuntime & runtime = *self.runtime;
    u32 id = 0x2E;
    GameStateObject * & object = runtime.object;
    for (; id <= 0x45; id++)
    {
        GameStateActiveEntity * entity = reinterpret_cast<GameStateActiveEntity *>(object->vfunc_40(id));
        if (entity != nullptr && entity->vfunc_5C())
            entity->vfunc_7C();
    }
}

EC SECTION(".text.game_state_08011FE8") void func_08011FE8(GameStateScript & self, u32 flags)
{
    self.unk_00 = reinterpret_cast<u32>(vtable_unk_080E5E64);
    gUnk_0300040C->engine.ClearUnk();
    delete self.runtime;
    func_080007EC(self, flags);
}

EC SECTION(".text.game_state_08012064") void func_08012064(GameStateScript & self, i32 id, u32 value_2, u32 value_3, u32 value_4)
{
    GameStateRuntime & runtime = *self.runtime;
    u32 value_1 = runtime.object->vfunc_14();
    func_080D7F9C(runtime, id, value_1, value_2, value_3, value_4);
}

EC SECTION(".text.game_state_08016CEC") void func_08016CEC(GameStateScript & self, u32 id)
{
    GameStateObject * & object = self.runtime->object;
    object->vfunc_38(id);
    AActorEntity * entity = object->vfunc_40(id);
    if (entity != nullptr && !entity->vfunc_28())
        entity->vfunc_20();
}

EC SECTION(".text.game_state_080167DC") void func_080167DC(GameStateScript & self)
{
    GameStateRuntime & runtime = *self.runtime;
    func_080A05E8(runtime.state + 0x1CD4, runtime.state + 0x11);
    GameStateObject * & object = runtime.object;
    object->vfunc_38(0x23);
    AActorEntity * entity = object->vfunc_40(0x23);
    if (entity != nullptr && !entity->vfunc_28())
        entity->vfunc_20();
}

EC SECTION(".text.game_state_08012464") void func_08012464(GameStateScript & self, i32 mode, u32 value)
{
    GameStateAudioList & audio = self.runtime->audio;
    void ** current = audio.begin;
    void ** end = audio.end;
    void * music;
    if (current != end)
    {
        do
        {
            if (!(u8) func_08008CD0(*current))
            {
                music = *current;
                goto play;
            }
            current++;
        } while (current != end);
    }
    music = end[-1];
play:
    u16 amount = value;
    switch (mode)
    {
    case 0:
        func_08008B6C(music, amount);
        break;
    case 1:
        func_08008B88(music, amount);
        break;
    case 2:
        func_08008BB0(music, amount);
        break;
    }
}
