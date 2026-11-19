#include "entity_actor.hh"
#include "dog.hh"
#include "horse.hh"
#include "chicken.hh"

struct DogEntityPart1 : AActorEntity
{
    DogEntityPart1();
    u32 unk_30;
    Dog * dog;
};

struct HorseEntityPart1 : AActorEntity
{
    HorseEntityPart1();
    u32 unk_30;
    Horse * horse;
};

struct ChickenEntityPart1 : AActorEntity
{
    ChickenEntityPart1();
    u32 unk_30;
    Chicken * chicken;
};

EC u16 const gUnk_080F0908[];
EC u16 const gUnk_080F09AC[];
EC u16 const gUnk_080F0A68[];
EC void func_080323C8(AEntity * self);
EC void func_0802A588(AEntity * self);

#define ENTITY_CODE(address) SECTION(".text.entities_part1." #address)

EC bool func_08020060(AEntity * self) ENTITY_CODE(08020060);
EC bool func_08020060(AEntity * self)
{
    return self->game_object->vfunc_14() == self->location_map;
}

EC Box func_080202A0(AEntity const * self) ENTITY_CODE(080202A0);
EC Box func_080202A0(AEntity const * self)
{
    return Box(-7, -9, 7, 5, self->x_q16 >> 16, self->y_q16 >> 16);
}

EC void func_080202C4(AEntity * self) ENTITY_CODE(080202C4);
EC void func_080202C4(AEntity * self)
{
}

EC void func_080202C8(AEntity * self) ENTITY_CODE(080202C8);
EC void func_080202C8(AEntity * self)
{
}

EC bool func_08020460(DogEntityPart1 * self) ENTITY_CODE(08020460);
EC bool func_08020460(DogEntityPart1 * self)
{
    return self->dog->unk_20 == 5;
}

EC u32 func_0802161C(u32 value) ENTITY_CODE(0802161C);
EC u32 func_0802161C(u32 value)
{
    return 0;
}

EC u32 func_08021620(u32 value) ENTITY_CODE(08021620);
EC u32 func_08021620(u32 value)
{
    switch (value)
    {
    default:
    case 0: return 6;
    case 1: return 8;
    }
}

EC u32 func_08021F88(HorseEntityPart1 * self, u32 anim) ENTITY_CODE(08021F88);
EC u32 func_08021F88(HorseEntityPart1 * self, u32 anim)
{
    u16 const * table = gUnk_080F09AC;
    return table[self->horse->GetGrowthStage() * 7 + anim];
}

EC u32 func_08022320(u32 value) ENTITY_CODE(08022320);
EC u32 func_08022320(u32 value)
{
    switch (value)
    {
    default:
    case 0: return 1;
    case 1: return 2;
    }
}

EC u32 func_08022334(u32 value) ENTITY_CODE(08022334);
EC u32 func_08022334(u32 value)
{
    switch (value)
    {
    default:
    case 0: return 14;
    case 1: return 15;
    }
}

EC u32 func_08022980(ChickenEntityPart1 * self, u32 anim) ENTITY_CODE(08022980);
EC u32 func_08022980(ChickenEntityPart1 * self, u32 anim)
{
    u16 const * table = gUnk_080F0A68;
    return table[self->chicken->GetGrowthStage() * 5 + anim];
}

EC u32 func_08022B58(u32 value) ENTITY_CODE(08022B58);
EC u32 func_08022B58(u32 value)
{
    return 0;
}

EC u32 func_08022B5C(u32 value) ENTITY_CODE(08022B5C);
EC u32 func_08022B5C(u32 value)
{
    switch (value)
    {
    default:
    case 0: return 6;
    case 1: return 7;
    }
}

EC u32 func_080237F0(u32 value) ENTITY_CODE(080237F0);
EC u32 func_080237F0(u32 value)
{
    switch (value)
    {
    default:
    case 0:
    case 1: return 1;
    case 2: return 2;
    }
}

EC u32 func_08023804(u32 value) ENTITY_CODE(08023804);
EC u32 func_08023804(u32 value)
{
    switch (value)
    {
    default:
    case 0: return 9;
    case 1: return 11;
    case 2: return 12;
    }
}

EC u32 func_080244C4(u32 value) ENTITY_CODE(080244C4);
EC u32 func_080244C4(u32 value)
{
    switch (value)
    {
    default:
    case 0: return 1;
    case 1: return 2;
    }
}

EC u32 func_080244D8(u32 value) ENTITY_CODE(080244D8);
EC u32 func_080244D8(u32 value)
{
    switch (value)
    {
    default:
    case 0: return 10;
    case 1: return 13;
    }
}

EC void func_08024BE8(AEntity * self) ENTITY_CODE(08024BE8);
EC void func_08024BE8(AEntity * self)
{
    self->AEntity::vfunc_10();
    func_0802A588(self);
}

EC void func_08024C54(AEntity * self) ENTITY_CODE(08024C54);
EC void func_08024C54(AEntity * self)
{
    self->unk_06 = true;
    func_080323C8(self);
}

EC void func_08024C64(AEntity * self) ENTITY_CODE(08024C64);
EC void func_08024C64(AEntity * self)
{
    self->unk_06 = false;
    func_080323C8(self);
}

EC void func_08024C88(u8 * self) ENTITY_CODE(08024C88);
EC void func_08024C88(u8 * self)
{
    self[0x3C] = 8;
    self[0x3D] = 255;
}

EC void func_08024C98(u8 * self) ENTITY_CODE(08024C98);
EC void func_08024C98(u8 * self)
{
    self[0x3C] = 0;
}
