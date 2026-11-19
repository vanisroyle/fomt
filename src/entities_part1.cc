#include "entity_actor.hh"
#include "dog.hh"
#include "horse.hh"
#include "chicken.hh"
#include "barn_animal.hh"
#include "cow.hh"
#include "sheep.hh"
#include "item.hh"
#include <stdlib.h>

struct DogEntityPart1 : AActorEntity
{
    DogEntityPart1();
    u32 unk_30;
    Dog * dog;
    void * unk_38;
    i16 saved_x;
    i16 saved_y;
    bool override_state;
    u8 state;
};

struct HorseEntityPart1 : AActorEntity
{
    HorseEntityPart1();
    u32 unk_30;
    Horse * horse;
    STRUCT_PAD(0x38, 0x3C);
    u8 state;
    bool override_state;
    bool suppress_effect;
};

struct ChickenEntityPart1 : AActorEntity
{
    ChickenEntityPart1();
    u32 unk_30;
    Chicken * chicken;
};

EC u16 const gUnk_080F09AC[];
EC u16 const gUnk_080F0A68[];
EC u16 const gUnk_080F0C3C[];
EC u16 const gUnk_080F0E34[];
EC void func_080323C8(AEntity * self);
EC void func_0802A588(AEntity * self);

struct AnimStatePart1
{
    STRUCT_PAD(0x00, 0x0E);
    u16 frames;
    i16 count;
};

EC bool func_0805E894(AnimStatePart1 * state);
EC bool func_080AC070(void * map, Box const * box);

struct BarnEntityPart1 : AActorEntity
{
    BarnEntityPart1();
    u32 unk_30;
    BarnAnimal * animal;
    u8 state;
};

EC void func_08020038(AActorEntity * self, GameObject * game, ActorLocation const * location, u32 speed);
EC u32 func_08022950(ChickenEntityPart1 * self);
EC u32 func_08023378(BarnEntityPart1 * self);
EC u32 func_08024020(BarnEntityPart1 * self);
EC void func_080228B4(ChickenEntityPart1 * self, u32 state, u32 facing);
EC void func_08023308(BarnEntityPart1 * self, u32 state, u32 facing);
EC void func_08023FB0(BarnEntityPart1 * self, u32 state, u32 facing);
EC char const vtable_unk_080E639C[];
EC char const vtable_unk_080E6310[];
EC char const vtable_unk_080E6284[];
EC void func_08020080(AActorEntity * self, i32 speed);
EC u32 func_0802110C(DogEntityPart1 * self, u32 state);
EC i32 func_08021130(DogEntityPart1 * self, u32 state);
EC i32 func_08021FAC(HorseEntityPart1 * self, u32 state);
EC u32 func_08021F88(HorseEntityPart1 * self, u32 state);
EC u32 func_080233B4(BarnEntityPart1 * self, u32 state);
EC u32 func_0802405C(BarnEntityPart1 * self, u32 state);
EC u32 func_08023400(AEntity * self, u32 state);
EC u32 func_080240BC(AEntity * self, u32 state);
EC void func_08032384(AEntity * self, u32 effect, u32 argument);

#define ENTITY_CODE(address) SECTION(".text.entities_part1." #address)

EC void func_08022048(HorseEntityPart1 * self, u32 state, u32 facing) ENTITY_CODE(08022048);
EC void func_08022048(HorseEntityPart1 * self, u32 state, u32 facing)
{
    if (state != 6 && self->override_state)
        state = 6;
    u32 anim = func_08021F88(self, state);
    if (self->anim_id != anim)
        self->SetAnim(anim);
    if (self->facing != facing)
        self->SetAnimFacing(facing);
    func_08020080(self, func_08021FAC(self, state));
    if (state != self->state && !self->suppress_effect)
    {
        i32 effect = -1;
        if (state == 4)
            effect = 8;
        if (effect >= 0)
            func_08032384(self, effect, 1);
        else
            func_080323C8(self);
    }
    self->state = state;
}

EC void func_080211E4(DogEntityPart1 * self, u32 state, u32 facing) ENTITY_CODE(080211E4);
EC void func_080211E4(DogEntityPart1 * self, u32 state, u32 facing)
{
    if (state != 12 && self->override_state)
        state = 12;
    u32 anim = func_0802110C(self, state);
    if (self->anim_id != anim)
        self->SetAnim(anim);
    if (self->facing != facing)
        self->SetAnimFacing(facing);
    func_08020080(self, func_08021130(self, state));
    if (state != self->state)
    {
        i32 effect = -1;
        if (state == 8)
            effect = 8;
        if (effect >= 0)
            func_08032384(self, effect, 1);
        else
            func_080323C8(self);
    }
    self->state = state;
}

EC void func_08023FB0(BarnEntityPart1 * self, u32 state, u32 facing) ENTITY_CODE(08023FB0);
EC void func_08023FB0(BarnEntityPart1 * self, u32 state, u32 facing)
{
    u32 anim = func_0802405C(self, state);
    if (self->anim_id != anim)
        self->SetAnim(anim);
    if (self->facing != facing)
        self->SetAnimFacing(facing);
    func_08020080(self, func_080240BC(self, state));
    if (state != self->state)
    {
        i32 effect = -1;
        if (state == 4)
            effect = 8;
        if (effect >= 0)
            func_08032384(self, effect, 1);
        else
            func_080323C8(self);
    }
    self->state = state;
}

EC void func_08023308(BarnEntityPart1 * self, u32 state, u32 facing) ENTITY_CODE(08023308);
EC void func_08023308(BarnEntityPart1 * self, u32 state, u32 facing)
{
    u32 anim = func_080233B4(self, state);
    if (self->anim_id != anim)
        self->SetAnim(anim);
    if (self->facing != facing)
        self->SetAnimFacing(facing);
    func_08020080(self, func_08023400(self, state));
    if (state != self->state)
    {
        i32 effect = -1;
        if (state == 4)
            effect = 8;
        if (effect >= 0)
            func_08032384(self, effect, 1);
        else
            func_080323C8(self);
    }
    self->state = state;
}

EC BarnEntityPart1 * func_080238A8(BarnEntityPart1 * self, GameObject * game, BarnAnimal * animal) ENTITY_CODE(080238A8);
EC BarnEntityPart1 * func_080238A8(BarnEntityPart1 * self, GameObject * game, BarnAnimal * animal)
{
    Actor location(*animal);
    func_08020038(self, game, &location.location, 0x921);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6284;
    self->animal = animal;
    func_08023FB0(self, func_08024020(self), self->facing);
    return self;
}

EC BarnEntityPart1 * func_08022C18(BarnEntityPart1 * self, GameObject * game, BarnAnimal * animal) ENTITY_CODE(08022C18);
EC BarnEntityPart1 * func_08022C18(BarnEntityPart1 * self, GameObject * game, BarnAnimal * animal)
{
    Actor location(*animal);
    func_08020038(self, game, &location.location, 0x2B0);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6310;
    self->animal = animal;
    func_08023308(self, func_08023378(self), self->facing);
    return self;
}

EC ChickenEntityPart1 * func_080223A8(ChickenEntityPart1 * self, GameObject * game, Chicken * animal) ENTITY_CODE(080223A8);
EC ChickenEntityPart1 * func_080223A8(ChickenEntityPart1 * self, GameObject * game, Chicken * animal)
{
    Actor location(*animal);
    func_08020038(self, game, &location.location, 0x720);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E639C;
    self->chicken = animal;
    func_080228B4(self, func_08022950(self), self->facing);
    return self;
}

EC u32 func_080246BC(u32 minimum, Tool const * tool) ENTITY_CODE(080246BC);
EC u32 func_080246BC(u32 minimum, Tool const * tool)
{
    u32 level;
    switch (tool->GetId())
    {
    case 32: level = 0; break;
    case 33: level = 1; break;
    case 34: level = 2; break;
    case 35: level = 3; break;
    case 36: level = 4; break;
    case 37:
    case 38: level = 5; break;
    case 39: level = 6; break;
    default: return minimum;
    }
    if (level > minimum)
        minimum = level;
    return minimum;
}

EC u32 func_080245E0(u32 value) ENTITY_CODE(080245E0);
EC u32 func_080245E0(u32 value)
{
    switch (value)
    {
    case 50: return 0;
    case 51: return 1;
    case 52: return 2;
    case 53: return 3;
    case 54: return 4;
    case 55: return 5;
    case 56: return 6;
    case 57: return 7;
    case 58: return 8;
    case 59: return 9;
    case 60: return 10;
    case 61: return 11;
    case 62: return 12;
    case 63: return 13;
    case 64: return 14;
    case 65: return 20;
    case 66: return 15;
    case 67: return 16;
    case 68:
    {
        u8 chance = rand() % 1000 / 10;
        if (chance <= 4)
            return 18;
        return 17;
    }
    case 69: return 19;
    default: return 21;
    }
}

EC u32 func_08021264(DogEntityPart1 * self) ENTITY_CODE(08021264);
EC u32 func_08021264(DogEntityPart1 * self)
{
    switch (self->dog->unk_20)
    {
    default: return 3;
    case 0: return self->dog->unk_24.a.unk_02;
    case 1:
    case 2:
    case 3:
    case 5: return 2;
    case 6: return self->dog->unk_24.a.unk_02 ? 11 : 2;
    }
}


EC bool func_08020060(AEntity * self) ENTITY_CODE(08020060);
EC bool func_08020060(AEntity * self)
{
    return self->game_object->vfunc_14() == self->location_map;
}

EC bool func_08020130(AEntity * self) ENTITY_CODE(08020130);
EC bool func_08020130(AEntity * self)
{
    if (self->unk_10.Get() != nullptr)
    {
        bool result = false;
        AnimStatePart1 * state = reinterpret_cast<AnimStatePart1 *>(&self->unk_10.Get()->sprite_animator);
        bool active = state->frames != 0 && state->count != 0;
        if (!active || func_0805E894(state))
            result = true;
        return result;
    }
    return true;
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

EC u32 func_080220D4(HorseEntityPart1 * self) ENTITY_CODE(080220D4);
EC u32 func_080220D4(HorseEntityPart1 * self)
{
    Horse * horse = self->horse;
    switch (i32(horse->unk_20))
    {
    default: return 0;
    case 0: return horse->unk_24.a.unk_02;
    case 1: return 2;
    case 2: return 1;
    }
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

EC u32 func_08022950(ChickenEntityPart1 * self) ENTITY_CODE(08022950);
EC u32 func_08022950(ChickenEntityPart1 * self)
{
    Chicken * chicken = self->chicken;
    switch (i32(chicken->unk_24))
    {
    case 0: return chicken->unk_28.unk_02;
    case 1: return (chicken->unk_28.unk_02 & 0x7F) ? 4 : 1;
    default: return 0;
    }
}

EC u32 func_08022980(ChickenEntityPart1 * self, u32 anim) ENTITY_CODE(08022980);
EC u32 func_08022980(ChickenEntityPart1 * self, u32 anim)
{
    u16 const * table = gUnk_080F0A68;
    return table[self->chicken->GetGrowthStage() * 5 + anim];
}

EC u32 func_080229A4(AEntity * self, u32 value) ENTITY_CODE(080229A4);
EC u32 func_080229A4(AEntity * self, u32 value)
{
    switch (value)
    {
    case 1: return 0x8000;
    default: return 0;
    }
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

EC u32 func_08023378(BarnEntityPart1 * self) ENTITY_CODE(08023378);
EC u32 func_08023378(BarnEntityPart1 * self)
{
    BarnAnimal * animal = self->animal;
    switch (i32(animal->unk_28))
    {
    default: return 0;
    case 0: return animal->unk_2C.a.unk_02;
    case 1: return (animal->unk_2C.a.unk_02 & 0x7F) ? 6 : 1;
    case 2:
    case 3: return 1;
    }
}

EC u32 func_08023400(AEntity * self, u32 value) ENTITY_CODE(08023400);
EC u32 func_08023400(AEntity * self, u32 value)
{
    switch (value)
    {
    case 1: return 0x8000;
    default: return 0;
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

EC u32 func_08024020(BarnEntityPart1 * self) ENTITY_CODE(08024020);
EC u32 func_08024020(BarnEntityPart1 * self)
{
    BarnAnimal * animal = self->animal;
    switch (i32(animal->unk_28))
    {
    default: return 0;
    case 0: return animal->unk_2C.a.unk_02;
    case 1: return (animal->unk_2C.a.unk_02 & 0x7F) ? 6 : 1;
    case 2:
    case 3: return 1;
    }
}

EC u32 func_080240BC(AEntity * self, u32 value) ENTITY_CODE(080240BC);
EC u32 func_080240BC(AEntity * self, u32 value)
{
    switch (value)
    {
    case 1: return 0x8000;
    default: return 0;
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
