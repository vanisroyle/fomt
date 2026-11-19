#include "entity_actor.hh"
#include "dog.hh"
#include "horse.hh"
#include "chicken.hh"
#include "barn_animal.hh"
#include "cow.hh"
#include "sheep.hh"
#include "item.hh"
#include "farmer.hh"
#include "shipping_bin.hh"
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
EC void func_080212C0(DogEntityPart1 * self);
EC u32 func_08021264(DogEntityPart1 * self);
EC u32 func_080220D4(HorseEntityPart1 * self);
EC void func_080211E4(DogEntityPart1 * self, u32 state, u32 facing);
EC void func_08022048(HorseEntityPart1 * self, u32 state, u32 facing);

struct AnimalTimerPart1
{
    u32 time : 16;
    u32 active : 8;
    u32 unused : 8;
};

struct ThrownItemPart1
{
    bool active;
    bool unk_01;
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
        unk_01 = false;
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

struct FarmerHeldAnimationPart1
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

struct FarmerEntityPart1 : AActorEntity
{
    FarmerEntityPart1();
    u32 unk_30;
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
    FarmerHeldAnimationPart1 held_anim;
    bool unk_78;
    bool unk_79;
    bool unk_7A;
    u8 unk_7B;
    u16 icon;
    u16 unk_7E;
    i32 unk_80;
    STRUCT_PAD(0x84, 0x88);
    ThrownItemPart1 thrown_item;
    bool unk_A4;
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

struct SoundChannelsPart1
{
    void ** begin;
    void ** end;
};

EC bool func_08025944(FarmerEntityPart1 * self);

struct SoundGameVTablePart1
{
    STRUCT_PAD(0x00, 0x14C);
    SoundChannelsPart1 const * (*get_channels)(GameObject * game);
};

EC bool func_08008CD0(void * channel);
EC void func_08008B6C(void * channel, u16 sound);
EC void func_0802A7D8(FarmerEntityPart1 * self);
EC void func_0802A510(FarmerEntityPart1 * self);
EC void func_0800F4C0(HeldItem * self);
EC void func_0800F4F0(HeldItem * self, u8 value);
EC void func_0800F4D8(HeldItem * self);
EC u32 func_08020018(u32 value);
EC void func_080200C4(AActorEntity * self, u32 sound);

struct AnimalTimerPairPart1
{
    u32 time;
    u32 unknown;
};

struct CarryEntityPart1 : AActorEntity
{
    CarryEntityPart1();
    virtual void vfunc_3C();
    virtual bool vfunc_40();
};

struct EntityGameVTablePart1
{
    STRUCT_PAD(0x00, 0x40);
    CarryEntityPart1 * (*get_entity)(GameObject * game, u32 id);
};

inline CarryEntityPart1 * EntityByIdPart1(GameObject * game, u32 id)
{
    EntityGameVTablePart1 const * table = *reinterpret_cast<EntityGameVTablePart1 const **>(game);
    return table->get_entity(game, id);
}

EC char const vtable_unk_080E6428[];
EC char const vtable_unk_080E64C8[];
EC void func_08038374(CarryEntityPart1 * self, i32 x, i32 y, u32 anim, u32 facing);
EC void func_08020410(DogEntityPart1 * self, u32 time);
EC Article func_0800F258(HeldItem const * self);
EC Food func_0800F20C(HeldItem const * self);
EC bool func_0800F388(HeldItem const * self);
EC void func_0800F528(HeldItem * self);
EC bool func_08025AFC(AEntity const * self);

struct FarmerGameStatePart1
{
    STRUCT_PAD(0x00, 0x08);
    u32 weather;
    STRUCT_PAD(0x0C, 0x12);
    GameTime time;
    STRUCT_PAD(0x14, 0x54);
    ShippingBin shipping_bin;
};
EC void func_080384FC(CarryEntityPart1 * self);
EC void func_0802B908(FarmerEntityPart1 * self, GameObject * game, void * state);
EC void func_0802C2DC(FarmerEntityPart1 * self);
EC char const vtable_unk_080E6658[];
EC void func_0802F0EC(FarmerEntityPart1 * self, u32 first, u32 second);
EC u16 func_0802D158(FarmerEntityPart1 * self);
EC void func_0803242C(AActorEntity * self, u32 argument);

inline i32 FarmerStatePart1(FarmerEntityPart1 const * self)
{
    return self->state;
}

struct MovementBoundsPart1;
EC void func_080AB85C(MovementBoundsPart1 * self);
EC void func_080AB8D0(MovementBoundsPart1 * self);
EC void func_080AB948(MovementBoundsPart1 * self);
EC void func_080AB9C4(MovementBoundsPart1 * self);

struct MovementBoundsPart1
{
    i32 positive_y;
    i32 negative_y;
    i32 negative_x;
    i32 positive_x;
    STRUCT_PAD(0x10, 0x28);
    i32 limit;

    i32 GetPositiveY()
    {
        if (positive_y > limit)
        {
            func_080AB85C(this);
            return positive_y;
        }
        return positive_y;
    }

    i32 GetNegativeY()
    {
        i32 value = negative_y;
        if (value < -limit)
        {
            func_080AB8D0(this);
            return negative_y;
        }
        return value;
    }

    i32 GetNegativeX()
    {
        i32 value = negative_x;
        if (value < -limit)
        {
            func_080AB948(this);
            return negative_x;
        }
        return value;
    }

    i32 GetPositiveX()
    {
        if (positive_x > limit)
        {
            func_080AB9C4(this);
            return positive_x;
        }
        return positive_x;
    }
};

inline void * EntitySoundChannelPart1(GameObject * game)
{
    SoundGameVTablePart1 const * table = *reinterpret_cast<SoundGameVTablePart1 const **>(game);
    SoundChannelsPart1 const * channels = table->get_channels(game);
    void ** it = channels->begin;
    void ** end = channels->end;
    for (; it != end; ++it)
    {
        if (!func_08008CD0(*it))
            return *it;
    }
    return end[-1];
}

inline i32 EntityFacingPart1(AActorEntity const * self)
{
    return self->facing;
}

inline void LaunchHeldItemPart1(FarmerEntityPart1 * self)
{
    switch (EntityFacingPart1(self))
    {
    case 2: self->thrown_item.Launch(-0xA0000, 0, 0xC0000); break;
    case 3: self->thrown_item.Launch(0xA0000, 0, 0xC0000); break;
    case 1: self->thrown_item.Launch(0, -0xA0000, 0xC0000); break;
    case 0: self->thrown_item.Launch(0, 0xA0000, 0xC0000); break;
    }
}

#define ENTITY_CODE(address) SECTION(".text.entities_part1." #address)

EC void func_08024C74(FarmerEntityPart1 * self) ENTITY_CODE(08024C74);
EC void func_08024C74(FarmerEntityPart1 * self)
{
    self->state = 0;
    self->motion = 0;
    self->unk_A6 = 0;
}

EC bool func_08025068(FarmerEntityPart1 * self, i32 stamina, i32 fatigue) ENTITY_CODE(08025068);
EC bool func_08025068(FarmerEntityPart1 * self, i32 stamina, i32 fatigue)
{
    if (reinterpret_cast<u8 const *>(self->game_state)[0x34C4])
        return false;
    if (self->unk_10.Get() != nullptr)
        reinterpret_cast<u8 *>(self->unk_10.Get())[0x281] = true;
    u32 old_stamina = func_0800E4F0(*self->farmer);
    u32 max_stamina = func_0800E51C(*self->farmer);
    func_0800E9E4(*self->farmer, stamina);
    u32 new_stamina = func_0800E4F0(*self->farmer);
    u32 old_fatigue = func_0800E4FC(*self->farmer);
    func_0800EA68(*self->farmer, fatigue);
    u32 new_fatigue = func_0800E4FC(*self->farmer);
    if (stamina > 0 || fatigue < 0)
    {
        switch (self->exhaustion_state)
        {
        case 36: if (new_stamina > max_stamina / 2) self->exhaustion_state = 0; break;
        case 37: if (new_stamina > max_stamina / 5) self->exhaustion_state = 0; break;
        case 38: if (new_stamina > max_stamina / 20) self->exhaustion_state = 0; break;
        case 39: if (new_stamina != 0) self->exhaustion_state = 0; break;
        case 40: if (new_fatigue <= 49) self->exhaustion_state = 0; break;
        case 41: if (new_fatigue <= 79) self->exhaustion_state = 0; break;
        case 42: if (new_fatigue != 100) self->exhaustion_state = 0; break;
        }
    }
    if (old_fatigue != 100 && new_fatigue == 100)
    {
        self->exhaustion_state = 42;
        return true;
    }
    if (old_fatigue <= 79 && new_fatigue > 79)
        self->exhaustion_state = 41;
    else if (old_fatigue <= 49 && new_fatigue > 49)
        self->exhaustion_state = 40;
    else if (old_stamina != 0 && new_stamina == 0)
        self->exhaustion_state = 39;
    else if (old_stamina > max_stamina / 20 && new_stamina <= max_stamina / 20)
        self->exhaustion_state = 38;
    else if (old_stamina > max_stamina / 5 && new_stamina <= max_stamina / 5)
        self->exhaustion_state = 37;
    else if (old_stamina > max_stamina / 2 && new_stamina <= max_stamina / 2)
        self->exhaustion_state = 36;
    return false;
}

EC FarmerEntityPart1 * func_08024974(FarmerEntityPart1 * self, GameObject * game, void * state) ENTITY_CODE(08024974);
EC FarmerEntityPart1 * func_08024974(FarmerEntityPart1 * self, GameObject * game, void * state)
{
    func_0802B908(self, game, state);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6658;
    self->game_state = state;
    self->farmer = reinterpret_cast<Farmer *>(reinterpret_cast<u8 *>(state) + 0x1BD8);
    self->state = 0;
    self->unk_3D = 0;
    self->motion = 0;
    self->tool_state = 15;
    self->tool_level = 0;
    self->unk_41 = false;
    self->unk_42 = false;
    self->unk_44 = 0;
    self->held_anim.Init();
    self->unk_78 = false;
    self->unk_79 = false;
    self->unk_7A = false;
    self->icon = 0xFFFF;
    self->unk_80 = 0;
    self->thrown_item.Init();
    self->unk_A4 = true;
    self->unk_A6 = 0;
    self->unk_A8 = -1;
    self->unk_AC = -1;
    self->unk_B0 = false;
    self->unk_B1 = false;
    self->unk_B2 = 100;
    self->unk_B3 = false;
    self->unk_B4 = false;
    self->unk_C0 = false;
    self->unk_C1 = false;
    self->unk_C2 = false;
    self->exhaustion_state = 0;
    self->unk_C4 = false;
    self->unk_C5 = false;
    func_0802C2DC(self);
    u16 anim = func_0802D158(self);
    if (self->anim_id != anim)
        self->SetAnim(anim);
    return self;
}

EC u32 func_08025B10(FarmerEntityPart1 * self) ENTITY_CODE(08025B10);
EC u32 func_08025B10(FarmerEntityPart1 * self)
{
    u32 result = 0;
    if (func_08025AFC(self) && u32(reinterpret_cast<FarmerGameStatePart1 const *>(self->game_state)->weather - 1) <= 1)
        result = 1;
    FarmerGameStatePart1 const * game = reinterpret_cast<FarmerGameStatePart1 const *>(self->game_state);
    if (game->time.GetHour() > 21)
        ++result;
    if (game->time.GetHour() <= 5)
        result += 2;
    if (func_0800E4F0(*self->farmer) == 0)
        result += 2;
    if (func_08025944(self))
        result += 10;
    return result;
}

EC void func_08026150(FarmerEntityPart1 * self, RucksackItem const * item, bool wrapped) ENTITY_CODE(08026150);
EC void func_08026150(FarmerEntityPart1 * self, RucksackItem const * item, bool wrapped)
{
    Product product;
    switch (item->GetKind())
    {
    case RucksackItem::KIND_FOOD: product = Product(item->GetFood()); break;
    case RucksackItem::KIND_ARTICLE: product = Product(item->GetArticle()); break;
    }
    ShippingBin * shipping = &reinterpret_cast<FarmerGameStatePart1 *>(self->game_state)->shipping_bin;
    if (product.GetPrice() != 0 && !shipping->IsDisplayEnabled(product.GetId()))
        shipping->ForceEnableDisplay(product.GetId());
    func_0800F418(&self->farmer->unk_54, *item);
    if (wrapped)
        func_0800F528(&self->farmer->unk_54);
    func_0802A588(self);
    self->state = 17;
    LaunchHeldItemPart1(self);
    func_08008B6C(EntitySoundChannelPart1(self->game_object), 106);
}

EC Product func_080262C4(FarmerEntityPart1 const * self) ENTITY_CODE(080262C4);
EC Product func_080262C4(FarmerEntityPart1 const * self)
{
    HeldItem const * item = reinterpret_cast<HeldItem const *>(reinterpret_cast<u8 const *>(self->game_state) + 0x1C2C);
    if (!func_0800F190(item) && !func_0800F388(item))
    {
        switch (func_0800F204(item))
        {
        case HeldItem::KIND_FOOD: return Product(func_0800F20C(item));
        case HeldItem::KIND_ARTICLE: return Product(func_0800F258(item));
        }
    }
    return Product();
}

EC bool func_08024CA0(FarmerEntityPart1 const * self) ENTITY_CODE(08024CA0);
EC bool func_08024CA0(FarmerEntityPart1 const * self)
{
    if (!self->vfunc_28())
    {
        switch (FarmerStatePart1(self))
        {
        case 36: case 37: case 38: case 39: case 40: case 41: case 42: return true;
        }
    }
    return false;
}

EC void func_08025018(FarmerEntityPart1 * self, u32 argument) ENTITY_CODE(08025018);
EC void func_08025018(FarmerEntityPart1 * self, u32 argument)
{
    self->unk_C1 = false;
    if (!self->unk_C2)
    {
        if (self->unk_C0)
        {
            func_0802F0EC(self, 0, 0);
            u16 anim = func_0802D158(self);
            if (self->anim_id != anim)
                self->SetAnim(anim);
        }
        func_0803242C(self, argument);
    }
}

EC void func_08020170(AActorEntity * self, MovementBoundsPart1 * bounds) ENTITY_CODE(08020170);
EC void func_08020170(AActorEntity * self, MovementBoundsPart1 * bounds)
{
    i32 old_x = self->x_q16;
    i32 old_y = self->y_q16;
    i32 new_x = old_x + self->x_speed_q16;
    i32 new_y = old_y + self->y_speed_q16;
    if (bounds != nullptr)
    {
        MovementBoundsPart1 * movement = bounds;
        i32 x = old_x >> 16;
        i32 y = old_y >> 16;
        i32 dy = (new_y >> 16) - y;
        i32 dx = (new_x >> 16) - x;
        if (dy > 0 && dy > movement->GetPositiveY())
            new_y = (y + movement->GetPositiveY()) << 16;
        if (dy < 0 && dy < movement->GetNegativeY())
            new_y = (y + movement->GetNegativeY()) << 16;
        if (dx < 0 && dx < movement->GetNegativeX())
            new_x = (x + movement->GetNegativeX()) << 16;
        if (dx > 0 && dx > movement->GetPositiveX())
            new_x = (x + movement->GetPositiveX()) << 16;
    }
    self->x_q16 = new_x;
    self->y_q16 = new_y;
}

EC void func_080212C0(DogEntityPart1 * self) ENTITY_CODE(080212C0);
EC void func_080212C0(DogEntityPart1 * self)
{
    CarryEntityPart1 * target = EntityByIdPart1(self->game_object, 75);
    if (target != nullptr && reinterpret_cast<u8 *>(target)[0x26] != 0)
    {
        i32 x = self->x_q16;
        i32 y = self->y_q16;
        i32 offset = 8;
        u32 map_id = self->location_map;
        GameObject * game = self->game_object;
        if (map_id != game->vfunc_14() && map_id != 2)
            offset = 0;
        UnkMap const & map = game->vfunc_34(map_id);
        for (;;)
        {
            i32 delta = offset << 16;
            switch (EntityFacingPart1(self))
            {
            case 0: target->x_q16 = x; target->y_q16 = y + delta; break;
            case 1: target->x_q16 = x; target->y_q16 = y - delta; break;
            case 2: target->x_q16 = x - delta; target->y_q16 = y; break;
            case 3: target->x_q16 = x + delta; target->y_q16 = y; break;
            }
            --offset;
            if (offset < 0)
                break;
            Box box = target->GetBox();
            if (!func_080AC070(const_cast<UnkMap *>(&map), &box))
                break;
        }
        func_080384FC(target);
    }
}

EC void func_0802907C(FarmerEntityPart1 * self) ENTITY_CODE(0802907C);
EC void func_0802907C(FarmerEntityPart1 * self)
{
    HeldItem * item = reinterpret_cast<HeldItem *>(reinterpret_cast<u8 *>(self->game_state) + 0x1C2C);
    if (!func_0800F190(item))
    {
        if (func_0800F204(item) == HeldItem::KIND_ARTICLE)
        {
            Article article = func_0800F258(item);
            if (article.GetId() == 53)
                func_0800ED2C(*self->farmer);
        }
        func_0800F390(&self->farmer->unk_54);
        func_0802A7D8(self);
    }
}

EC DogEntityPart1 * func_08020310(DogEntityPart1 * self, GameObject * game, Dog * dog, void * object) ENTITY_CODE(08020310);
EC DogEntityPart1 * func_08020310(DogEntityPart1 * self, GameObject * game, Dog * dog, void * object)
{
    Actor location(*dog);
    func_08020038(self, game, &location.location, 0x340);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E64C8;
    self->dog = dog;
    self->unk_38 = object;
    self->override_state = false;
    if (dog->unk_20 == 5)
    {
        CarryEntityPart1 * target = EntityByIdPart1(game, 75);
        if (target != nullptr)
        {
            if (target->location_map == self->location_map)
                func_08038374(target, self->x_q16, self->y_q16, self->anim_id, self->facing);
            else
                func_08020410(self, 0);
        }
    }
    func_080211E4(self, func_08021264(self), self->facing);
    return self;
}

EC HorseEntityPart1 * func_080216BC(HorseEntityPart1 * self, GameObject * game, Horse * horse) ENTITY_CODE(080216BC);
EC HorseEntityPart1 * func_080216BC(HorseEntityPart1 * self, GameObject * game, Horse * horse)
{
    Actor location(*horse);
    func_08020038(self, game, &location.location, 0x782);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6428;
    self->horse = horse;
    self->override_state = false;
    CarryEntityPart1 * target = EntityByIdPart1(game, 0);
    self->suppress_effect = target && target->vfunc_40();
    func_08022048(self, func_080220D4(self), self->facing);
    return self;
}

EC void func_080221F0(HorseEntityPart1 * self, Location const * target) ENTITY_CODE(080221F0);
EC void func_080221F0(HorseEntityPart1 * self, Location const * target)
{
    if (target->GetMap() == self->GetMap())
    {
        u32 affection = self->horse->GetAffection();
        i32 radius = func_08020018(affection);
        i32 x = self->GetQ16X() >> 16;
        i32 dx = target->GetX() - x;
        i32 y = self->GetQ16Y() >> 16;
        i32 dy = target->GetY() - y;
        if (dx * dx + dy * dy <= radius * radius)
        {
            AnimalTimerPairPart1 timer;
            timer.time = u16(480 + self->horse->GetAffection() * 960 / 256);
            self->horse->method_0809BC30(reinterpret_cast<UnkBarnAnimal2C_x2 const *>(&timer));
            if (!self->override_state)
                func_08022048(self, func_080220D4(self), self->facing);
            self->horse->SetHasTalkedToday();
        }
    }
}

EC void func_0802151C(DogEntityPart1 * self, Location const * target) ENTITY_CODE(0802151C);
EC void func_0802151C(DogEntityPart1 * self, Location const * target)
{
    if (self->dog->GetGrowthStage() == 1 && target->GetMap() == self->GetMap())
    {
        u32 affection = self->dog->GetAdequacy();
        i32 radius = func_08020018(affection);
        i32 x = self->GetQ16X() >> 16;
        i32 dx = target->GetX() - x;
        i32 y = self->GetQ16Y() >> 16;
        i32 dy = target->GetY() - y;
        if (dx * dx + dy * dy <= radius * radius)
        {
            func_080200C4(self, 169);
            if (self->dog->unk_20 == 5)
                func_080212C0(self);
            AnimalTimerPairPart1 timer;
            timer.time = u16(480 + affection * 960 / 251);
            self->dog->method_0809BB70(reinterpret_cast<UnkBarnAnimal2C_x2 const *>(&timer));
            if (!self->override_state)
                func_080211E4(self, func_08021264(self), self->facing);
            self->dog->SetHasPlayedToday();
        }
    }
}

EC void func_08021444(DogEntityPart1 * self, Location const * target) ENTITY_CODE(08021444);
EC void func_08021444(DogEntityPart1 * self, Location const * target)
{
    if (target->GetMap() == self->GetMap() && self->dog->unk_20 != 5)
    {
        u32 affection = self->dog->GetAffection();
        i32 radius = func_08020018(affection);
        i32 x = self->GetQ16X() >> 16;
        i32 dx = target->GetX() - x;
        i32 y = self->GetQ16Y() >> 16;
        i32 dy = target->GetY() - y;
        if (dx * dx + dy * dy <= radius * radius)
        {
            if (self->dog->unk_20 == 5)
                func_080212C0(self);
            AnimalTimerPairPart1 timer;
            timer.time = u16(480 + affection * 960 / 251);
            self->dog->method_0809BB54(reinterpret_cast<UnkBarnAnimal2C_x2 const *>(&timer));
            if (!self->override_state)
                func_080211E4(self, func_08021264(self), self->facing);
            self->dog->SetHasTalkedToday();
        }
    }
}

EC bool func_08025944(FarmerEntityPart1 * self) ENTITY_CODE(08025944);
EC bool func_08025944(FarmerEntityPart1 * self)
{
    ToolStack * tools = &self->farmer->unk_5C;
    if (!tools->IsEmpty())
    {
        Tool tool = func_0800E550(*self->farmer, tools->GetTool());
        switch (tool.GetId())
        {
        case 5: case 13: case 21: case 29: case 37: case 45: return true;
        }
    }
    return false;
}

EC void func_08025FC4(FarmerEntityPart1 * self) ENTITY_CODE(08025FC4);
EC void func_08025FC4(FarmerEntityPart1 * self)
{
    func_0800F4D8(&self->farmer->unk_54);
    func_0802A588(self);
    self->state = 17;
    LaunchHeldItemPart1(self);
    func_08008B6C(EntitySoundChannelPart1(self->game_object), 106);
}

EC void func_08025EFC(FarmerEntityPart1 * self, u8 value) ENTITY_CODE(08025EFC);
EC void func_08025EFC(FarmerEntityPart1 * self, u8 value)
{
    func_0800F4F0(&self->farmer->unk_54, value);
    func_0802A588(self);
    self->state = 17;
    LaunchHeldItemPart1(self);
    func_08008B6C(EntitySoundChannelPart1(self->game_object), 106);
}

EC void func_08025E38(FarmerEntityPart1 * self) ENTITY_CODE(08025E38);
EC void func_08025E38(FarmerEntityPart1 * self)
{
    func_0800F4C0(&self->farmer->unk_54);
    func_0802A588(self);
    self->state = 17;
    LaunchHeldItemPart1(self);
    func_08008B6C(EntitySoundChannelPart1(self->game_object), 106);
}

EC void func_08025A40(FarmerEntityPart1 * self, bool next) ENTITY_CODE(08025A40);
EC void func_08025A40(FarmerEntityPart1 * self, bool next)
{
    if (func_08025944(self))
    {
        func_08008B6C(EntitySoundChannelPart1(self->game_object), 199);
        self->state = 0;
    }
    else
    {
        u32 result;
        if (!next)
            result = func_0800EC4C(*self->farmer);
        else
            result = func_0800ECB0(*self->farmer);
        if (result != 0)
        {
            self->state = 21;
            self->unk_3D = 20;
            func_0802A510(self);
            func_08008B6C(EntitySoundChannelPart1(self->game_object), 188);
        }
    }
}

EC void func_080258A0(FarmerEntityPart1 * self) ENTITY_CODE(080258A0);
EC void func_080258A0(FarmerEntityPart1 * self)
{
    RucksackItem item = func_0800F294(&self->farmer->unk_54);
    if (!item.IsEmpty())
    {
        u32 slot = self->farmer->rucksack.GetFirstFreeItemSlot();
        if (slot != u32(-1))
        {
            self->farmer->rucksack.GetItemAt(slot) = item;
            func_0800F390(&self->farmer->unk_54);
            self->state = 18;
            func_0802A7D8(self);
            func_08008B6C(EntitySoundChannelPart1(self->game_object), 107);
        }
        else
            self->state = 4;
    }
    else
        self->state = 0;
}

EC void func_08025838(FarmerEntityPart1 * self, bool next) ENTITY_CODE(08025838);
EC void func_08025838(FarmerEntityPart1 * self, bool next)
{
    u32 result;
    if (!next)
        result = func_0800EB3C(*self->farmer);
    else
        result = func_0800EBC4(*self->farmer);
    if (result != 0)
    {
        func_0802A588(self);
        self->state = 20;
        func_08008B6C(EntitySoundChannelPart1(self->game_object), 107);
    }
}

EC void func_080257D0(FarmerEntityPart1 * self, bool next) ENTITY_CODE(080257D0);
EC void func_080257D0(FarmerEntityPart1 * self, bool next)
{
    u32 result;
    if (!next)
        result = func_0800EB3C(*self->farmer);
    else
        result = func_0800EBC4(*self->farmer);
    if (result != 0)
    {
        self->state = 19;
        func_0802A7D8(self);
        func_08008B6C(EntitySoundChannelPart1(self->game_object), 107);
    }
}

EC void func_08021F3C(HorseEntityPart1 * self) ENTITY_CODE(08021F3C);
EC void func_08021F3C(HorseEntityPart1 * self)
{
    self->suppress_effect = false;
    AnimalTimerPart1 timer;
    timer.time = 120;
    timer.active = 0;
    self->horse->method_0809BC24(reinterpret_cast<UnkBarnAnimal2C const *>(&timer));
    func_08022048(self, func_080220D4(self), self->facing);
}

EC void func_08021EE0(HorseEntityPart1 * self) ENTITY_CODE(08021EE0);
EC void func_08021EE0(HorseEntityPart1 * self)
{
    if (self->horse->GetGrowthStage() == 1)
    {
        AnimalTimerPart1 timer;
        timer.time = 120;
        timer.active = 0;
        self->horse->method_0809BC24(reinterpret_cast<UnkBarnAnimal2C const *>(&timer));
        func_08022048(self, func_080220D4(self), self->facing);
        self->suppress_effect = true;
        self->horse->SetHasPlayedToday();
    }
}

EC void func_08020410(DogEntityPart1 * self, u32 time) ENTITY_CODE(08020410);
EC void func_08020410(DogEntityPart1 * self, u32 time)
{
    if (self->dog->unk_20 == 5)
        func_080212C0(self);
    AnimalTimerPart1 timer;
    timer.time = time;
    timer.active = 0;
    self->dog->method_0809BB48(reinterpret_cast<UnkBarnAnimal2C const *>(&timer));
    func_080211E4(self, 0, self->facing);
}

EC void func_08020080(AActorEntity * self, i32 speed) ENTITY_CODE(08020080);
EC void func_08020080(AActorEntity * self, i32 speed)
{
    i32 x = 0;
    i32 y = 0;
    if (speed != 0)
    {
        switch (EntityFacingPart1(self))
        {
        case 0: y = speed; break;
        case 1: y = -speed; break;
        case 2: x = -speed; break;
        case 3: x = speed; break;
        }
    }
    self->x_speed_q16 = x;
    self->y_speed_q16 = y;
}

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
