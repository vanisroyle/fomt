#ifndef FARMER_ENTITY_LAYOUT_HH
#define FARMER_ENTITY_LAYOUT_HH

#include "entity_actor.hh"
#include "farmer.hh"

struct ThrownItemMotion
{
    bool active;
    u8 display_mode;
    STRUCT_PAD(0x02, 0x04);
    i32 x_speed;
    i32 y_speed;
    i32 unk_0C;
    i32 unk_10;
    i32 z_speed;
    i32 unk_18;

    void Init()
    {
        active = false;
        display_mode = 0;
        x_speed = 0;
        y_speed = 0;
        unk_0C = 0;
        unk_10 = 0;
        z_speed = 0;
        unk_18 = 0;
    }

    void Launch(i32 x, i32 y, i32 z)
    {
        x_speed = x;
        y_speed = y;
        z_speed = z;
    }
};

EC char const gUnk_080F0E80[];
EC void __12RucksackItem(RucksackItem * self);

struct FarmerHeldItemAnimation
{
    void const * data;
    u16 frame;
    STRUCT_PAD(0x06, 0x08);
    RucksackItem item;
    bool unk_0C;
    bool unk_0D;
    STRUCT_PAD(0x0E, 0x10);
    i32 remaining;
    bool unk_14;
    STRUCT_PAD(0x15, 0x18);

    void Init()
    {
        data = gUnk_080F0E80;
        frame = 0;
        __12RucksackItem(&item);
        unk_0C = false;
        unk_0D = false;
        remaining = 59;
        unk_14 = false;
    }
};

struct FarmerEntity : AActorEntity
{
    FarmerEntity();
    u32 sound_flags;
    void * game_state;
    Farmer * farmer;
    u8 state;
    u8 unk_3D;
    u8 motion;
    u8 tool_state;
    u8 tool_level;
    bool unk_41;
    bool unk_42;
    u8 unk_43;
    u16 unk_44;
    STRUCT_PAD(0x46, 0x60);
    FarmerHeldItemAnimation held_anim;
    bool unk_78;
    bool unk_79;
    bool unk_7A;
    u8 unk_7B;
    u16 icon;
    u16 unk_7E;
    i32 unk_80;
    STRUCT_PAD(0x84, 0x88);
    ThrownItemMotion thrown_item;
    bool item_animation_override;
    u8 unk_A5;
    u16 unk_A6;
    i32 unk_A8;
    i32 unk_AC;
    bool unk_B0;
    bool unk_B1;
    u8 unk_B2;
    bool unk_B3;
    bool unk_B4;
    STRUCT_PAD(0xB5, 0xC0);
    bool unk_C0;
    bool unk_C1;
    bool unk_C2;
    u8 exhaustion_state;
    bool unk_C4;
    bool unk_C5;
};

#endif
