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
