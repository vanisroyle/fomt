#include "prelude.h"

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

struct GameStateRuntime
{
    STRUCT_PAD(0x00, 0x8C);
    u8 * state;
    STRUCT_PAD(0x90, 0x9C);
    u32 command;
    STRUCT_PAD(0xA0, 0xA8);
    void * object;
    void * actor;
    void * text;
    STRUCT_PAD(0xB4, 0xBC);
    u8 music[0x14];
    u8 unk_D0[0x10];
    u16 unk_E0;
};

struct GameStateScript
{
    u32 unk_00;
    GameStateRuntime * runtime;
};

struct GameStateGlobalStatus
{
    u8 unk_00;
    u32 unk_04;
};

struct GameStateGlobal
{
    STRUCT_PAD(0x000, 0x36C);
    GameStateGlobalStatus status;
};

extern GameStateGlobal * gUnk_0300040C;

EC void func_08008DE8();
EC void func_08008BE0(void * self);
EC u32 func_08008CD0(void * self);
EC void func_0803B6B8(void * self, u32 index, char const * text);
EC void func_08050E74(void * self);
EC void func_08050E5C(void * self);
EC void func_08050E8C(void * self);

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
        func_08050E74(actor);
}

EC void SECTION(".text.game_state_0801278C") func_0801278C(GameStateScript & self)
{
    void * actor = self.runtime->actor;
    if (actor != nullptr)
        func_08050E5C(actor);
}

EC void SECTION(".text.game_state_0801284C") func_0801284C(GameStateScript & self)
{
    void * actor = self.runtime->actor;
    if (actor != nullptr)
        func_08050E8C(actor);
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
    return self.runtime->unk_D0;
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
