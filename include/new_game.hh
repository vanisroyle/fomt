#ifndef NEW_GAME_HH
#define NEW_GAME_HH

#include "prelude.h"
#include "scene.hh"

struct NewGameTransitionOwner
{
    AUnk_0800080C * inner;
    ~NewGameTransitionOwner() { delete inner; }
    AUnk_0800080C * Release()
    {
        AUnk_0800080C * result = inner;
        inner = nullptr;
        return result;
    }
};

struct NewGameChoiceLifetime
{
    u32 unk_00;
    void * vtable;
    STRUCT_PAD(0x0008, 0x1A2C);
    void * resource;
};

struct NewGameNameLifetime
{
    u32 unk_00;
    void * vtable;
    STRUCT_PAD(0x0008, 0x45F8);
    void * resource;
};

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
    STRUCT_PAD(0x0000, 0x45F0);
    u32 unk_45F0;
    u32 unk_45F4;
    void * resource;
    u32 unk_45FC;
    u32 page;
    i8 column;
    i8 row;
    STRUCT_PAD(0x4606, 0x4608);
    u32 cursor_attributes;
    u16 cursor_attr2;
    STRUCT_PAD(0x460E, 0x4610);
    u8 length;
    STRUCT_PAD(0x4611, 0x4614);
    u32 marker_attributes;
    u16 marker_attr2;
    STRUCT_PAD(0x461A, 0x461C);
    char name[12];
    u8 unk_4628;
    char trimmed_name[13];
    u8 unk_4636;
    u8 unk_4637;
    u8 unk_4638;
    u8 unk_4639;
    STRUCT_PAD(0x463A, 0x463C);
    u32 unk_463C;
};

struct NewGameChoiceState
{
    STRUCT_PAD(0x0000, 0x1A2C);
    void * resource;
    u32 unk_1A30;
    STRUCT_PAD(0x1A34, 0x1A74);
    u8 unk_1A74;
    u8 unk_1A75;
    u8 choice;
    void MarkReady() { unk_1A75 = 1; }
    void Select(bool value) { choice = value; }
};

struct NewGameMenuState
{
    struct String
    {
        char text[128];
        bool Empty() const { return text[0] == 0; }
        char const * Get() const { return text; }
    };

    STRUCT_PAD(0x00, 0x80);
    u32 selection;
    u8 values[6];
    STRUCT_PAD(0x8A, 0x98);
    String names[2];
};

struct NewGameSprite
{
    u32 attributes;
    u16 attr2;
};

struct NewGameSpriteList
{
    u8 count;
    STRUCT_PAD(0x01, 0x04);
    NewGameSprite entries[128];

    NewGameSprite & At(unsigned int index)
    {
        return *reinterpret_cast<NewGameSprite *>(reinterpret_cast<char *>(this) + (index * 8 + 4));
    }
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
