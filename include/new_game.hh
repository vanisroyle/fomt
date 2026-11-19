#ifndef NEW_GAME_HH
#define NEW_GAME_HH

#include "prelude.h"

struct NewGameState
{
    u32 unk_00;
    virtual ~NewGameState();
};

struct NewGameScene
{
    void * vtable;
    NewGameState * state;
};

struct NewGameNameState
{
    STRUCT_PAD(0x0000, 0x4600);
    u32 page;
    i8 column;
    i8 row;
    STRUCT_PAD(0x4606, 0x4610);
    u8 length;
    STRUCT_PAD(0x4611, 0x461C);
    char name[12];
    STRUCT_PAD(0x4628, 0x4636);
    u8 unk_4636;
    u8 unk_4637;
    u8 unk_4638;
    u8 unk_4639;
    STRUCT_PAD(0x463A, 0x463C);
    u32 unk_463C;
};

struct NewGameChoiceState
{
    STRUCT_PAD(0x0000, 0x1A74);
    u8 unk_1A74;
    u8 unk_1A75;
    u8 choice;
};

struct NewGameChoiceScene
{
    void * vtable;
    NewGameChoiceState * state;
};

struct NewGameNameScene
{
    void * vtable;
    NewGameNameState * state;
};

#endif // NEW_GAME_HH
