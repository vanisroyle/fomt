#include "farmer_entity_layout.hh"

#define ENTITY_CODE(addr) SECTION(".text.entities_part2." #addr)

enum
{
    FARMER_IDLE_ANIMATION = 0x192,
    FARMER_STATE_7 = 7,
    FARMER_STATE_45 = 45,
};

EC void ResetFarmerItemDisplayAnimation(FarmerEntityPart1 * farmer) ENTITY_CODE(080299D0);
EC void ResetFarmerItemDisplayAnimation(FarmerEntityPart1 * farmer)
{
    farmer->item_animation_override = false;
    farmer->thrown_item.active = false;
    farmer->SetAnim(FARMER_IDLE_ANIMATION);
}

struct FarmerIconState
{
    u8 enabled;
    u8 reserved;
    u8 frame_state;
    u8 restart;
};

struct FarmerIconGraphics : UnknownEntityThing
{
    STRUCT_PAD(0x8C, 0xB4);
    SpriteAnimator icon_animator;
    FarmerIconState icon_state;
};

struct FarmerIconGameVTable
{
    STRUCT_PAD(0x00, 0x64);
    void * (*get_item_sprites)(GameObject * game);
};

EC void func_0805E850(SpriteAnimator * animator, void * sprites, u32 animation);

inline void * FarmerItemSprites(FarmerIconGraphics * graphics)
{
    AEntity * entity = *reinterpret_cast<AEntity **>(graphics);
    GameObject * game = entity->game_object;
    FarmerIconGameVTable const * table = *reinterpret_cast<FarmerIconGameVTable const **>(game);
    return table->get_item_sprites(game);
}

inline void RestartFarmerIcon(FarmerIconState * state)
{
    u8 * flags = reinterpret_cast<u8 *>(state);
    *flags = 1;
    flags += 2;
    *flags = 0;
    ++flags;
    *flags = 1;
}

EC void ShowFarmerToolDisplay(FarmerEntityPart1 * farmer) ENTITY_CODE(0802A510);
EC void ShowFarmerToolDisplay(FarmerEntityPart1 * farmer)
{
    ThrownItemPart1 * item = &farmer->thrown_item;
    item->active = true;
    item->display_mode = 0;
    item->Launch(-8 * 65536, 0, 17 * 65536);
    FarmerIconGraphics * graphics = static_cast<FarmerIconGraphics *>(farmer->unk_10.Get());
    if (graphics != nullptr)
    {
        Farmer const * inventory_owner = farmer->farmer;
        Tool tool = func_0800E550(*inventory_owner, inventory_owner->unk_5C.GetTool());
        u32 animation = tool.GetIconId();
        void * sprites = FarmerItemSprites(graphics);
        func_0805E850(&graphics->icon_animator, sprites, animation);
        RestartFarmerIcon(&graphics->icon_state);
    }
}

EC void CancelFarmerItemDisplay(FarmerEntityPart1 * farmer) ENTITY_CODE(0802A7D8);
EC void CancelFarmerItemDisplay(FarmerEntityPart1 * farmer)
{
    farmer->thrown_item.active = false;
}

EC FarmerEntityPart1 * func_08024974(FarmerEntityPart1 * farmer, GameObject * game, void * state);
EC FarmerEntityPart1 * NewFarmerMapEntity(GameObject * game, void * state) ENTITY_CODE(0802B8EC);
EC FarmerEntityPart1 * NewFarmerMapEntity(GameObject * game, void * state)
{
    return func_08024974(static_cast<FarmerEntityPart1 *>(::operator new(sizeof(FarmerEntityPart1))), game, state);
}

EC AActorEntity * __12AActorEntityP10GameObjectRC13ActorLocationUiUi(AActorEntity * entity, GameObject * game,
    ActorLocation const & location, u32 sprite_mode, u32 animation);
EC char const vtable_unk_080E676C[];

EC FarmerEntityPart1 * InitFarmerMapActor(FarmerEntityPart1 * farmer, GameObject * game, void * state) ENTITY_CODE(0802B908);
EC FarmerEntityPart1 * InitFarmerMapActor(FarmerEntityPart1 * farmer, GameObject * game, void * state)
{
    Farmer const * model = reinterpret_cast<Farmer const *>(static_cast<u8 *>(state) + 0x1BD8);
    ActorLocation location = func_0800E924(*model);
    __12AActorEntityP10GameObjectRC13ActorLocationUiUi(farmer, game, location, 2, FARMER_IDLE_ANIMATION);
    reinterpret_cast<void const **>(farmer)[5] = vtable_unk_080E676C;
    return farmer;
}

struct ItemDisplayEntity : AActorEntity
{
    ItemDisplayEntity();

    STRUCT_PAD(0x30, 0x34);
    u8 state;
    STRUCT_PAD(0x35, 0x54);
    bool active;
    STRUCT_PAD(0x55, 0x58);
    bool restart;
    bool alternate_idle;
    STRUCT_PAD(0x5A, 0x5C);
};

EC u32 GetItemDisplayStateAnimation(ItemDisplayEntity const * display) ENTITY_CODE(0802BC48);
EC u32 GetItemDisplayStateAnimation(ItemDisplayEntity const * display)
{
    u32 animation = FARMER_IDLE_ANIMATION;
    switch (display->state)
    {
    case 0:
    {
        bool alternate = display->alternate_idle;
        animation = 0x152;
        if (!alternate)
            animation += 0x40;
        break;
    }
    case 1:
    case 2:
        animation = 0x19E;
        break;
    case 3:
        animation = 0x146;
        break;
    case 4:
        animation = 0x42;
        break;
    }
    return animation;
}

EC void BeginItemDisplayStateTwo(ItemDisplayEntity * display) ENTITY_CODE(0802BEFC);
EC void BeginItemDisplayStateTwo(ItemDisplayEntity * display)
{
    display->state = 2;
    display->restart = true;
    display->SetAnim(static_cast<u16>(GetItemDisplayStateAnimation(display)));
    display->active = true;
}

EC ItemDisplayEntity * func_0802B9C0(ItemDisplayEntity * display, GameObject * game);
EC ItemDisplayEntity * NewItemDisplayEntity(GameObject * game) ENTITY_CODE(0802C210);
EC ItemDisplayEntity * NewItemDisplayEntity(GameObject * game)
{
    return func_0802B9C0(static_cast<ItemDisplayEntity *>(::operator new(sizeof(ItemDisplayEntity))), game);
}

EC void FarmerEntityInitNoOp(FarmerEntityPart1 * farmer) ENTITY_CODE(0802C2DC);
EC void FarmerEntityInitNoOp(FarmerEntityPart1 * farmer)
{
}

EC bool IsFarmerState7Or45(FarmerEntityPart1 const * farmer) ENTITY_CODE(0803205C);
EC bool IsFarmerState7Or45(FarmerEntityPart1 const * farmer)
{
    bool matches = false;
    if (farmer->state == FARMER_STATE_7 || farmer->state == FARMER_STATE_45)
        matches = true;
    return matches;
}

struct SpriteAnimationFrame
{
    u16 sprite_index;
    u16 delay;
};

struct SpriteAnimationFrames
{
    SpriteAnimationFrame const * frames;
    u16 count;

    u16 Count() const { return frames != nullptr ? count : 0; }
};

struct ActorSpriteAnimations
{
    virtual ~ActorSpriteAnimations();
    virtual SpriteAnimationFrames GetFrames(u32 animation);
};

EC u32 SumSpriteAnimationFrameDelays(ActorSpriteAnimations * sprite, u32 animation) ENTITY_CODE(08032090);
EC u32 SumSpriteAnimationFrameDelays(ActorSpriteAnimations * sprite, u32 animation)
{
    SpriteAnimationFrames loaded = sprite->GetFrames(animation);
    SpriteAnimationFrames frames = loaded;
    SpriteAnimationFrames * sequence = &frames;
    u32 total_delay = 0;
    u32 frame = 0;
    u32 count = sequence->Count();
    if (frame < count)
    {
        SpriteAnimationFrame const * next = sequence->frames;
        do
        {
            total_delay += (next++)->delay;
            ++frame;
        } while (frame < count);
    }
    return total_delay;
}
