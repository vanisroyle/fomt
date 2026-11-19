#include "farmer_entity_layout.hh"
#include "entity_sound_channel.hh"

#define ENTITY_CODE(addr) SECTION(".text.entities_part2." #addr)

enum
{
    FARMER_IDLE_ANIMATION = 0x192,
    FARMER_STATE_7 = 7,
    FARMER_STATE_45 = 45,
};

EC void ResetFarmerItemDisplayAnimation(FarmerEntity * farmer) ENTITY_CODE(080299D0);
EC void ResetFarmerItemDisplayAnimation(FarmerEntity * farmer)
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

EC void ShowFarmerToolDisplay(FarmerEntity * farmer) ENTITY_CODE(0802A510);
EC void ShowFarmerToolDisplay(FarmerEntity * farmer)
{
    ThrownItemMotion * item = &farmer->thrown_item;
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

EC void CancelFarmerItemDisplay(FarmerEntity * farmer) ENTITY_CODE(0802A7D8);
EC void CancelFarmerItemDisplay(FarmerEntity * farmer)
{
    farmer->thrown_item.active = false;
}

EC FarmerEntity * FarmerEntity_Init(FarmerEntity * farmer, GameObject * game, void * state);
EC FarmerEntity * NewFarmerMapEntity(GameObject * game, void * state) ENTITY_CODE(0802B8EC);
EC FarmerEntity * NewFarmerMapEntity(GameObject * game, void * state)
{
    return FarmerEntity_Init(static_cast<FarmerEntity *>(::operator new(sizeof(FarmerEntity))), game, state);
}

EC AActorEntity * __12AActorEntityP10GameObjectRC13ActorLocationUiUi(AActorEntity * entity, GameObject * game,
    ActorLocation const & location, u32 sprite_mode, u32 animation);
EC char const vtable_unk_080E676C[];

EC FarmerEntity * InitFarmerMapActor(FarmerEntity * farmer, GameObject * game, void * state) ENTITY_CODE(0802B908);
EC FarmerEntity * InitFarmerMapActor(FarmerEntity * farmer, GameObject * game, void * state)
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
    u8 transition_timer;
    STRUCT_PAD(0x36, 0x38);
    ThrownItemMotion thrown_item;
    bool active;
    STRUCT_PAD(0x55, 0x56);
    u16 display_icon;
    bool restart;
    bool alternate_idle;
    STRUCT_PAD(0x5A, 0x5C);

    bool HasThrownItem() const
    {
        ThrownItemMotion const * item = &thrown_item;
        return item->active == 1;
    }
};

EC char const vtable_unk_080E671C[];
EC char const vtable_unk_080E65F4[];

EC ItemDisplayEntity * InitItemDisplayMapActor(ItemDisplayEntity * display, GameObject * game) ENTITY_CODE(0802B944);
EC ItemDisplayEntity * InitItemDisplayMapActor(ItemDisplayEntity * display, GameObject * game)
{
    ActorLocation location(Location(MAP_NONE, 0, 0), 0);
    __12AActorEntityP10GameObjectRC13ActorLocationUiUi(display, game, location, 2, 0);
    reinterpret_cast<void const **>(display)[5] = vtable_unk_080E671C;
    return display;
}

EC ItemDisplayEntity * InitItemDisplayEntity(ItemDisplayEntity * display, GameObject * game) ENTITY_CODE(0802B9C0);
EC ItemDisplayEntity * InitItemDisplayEntity(ItemDisplayEntity * display, GameObject * game)
{
    InitItemDisplayMapActor(display, game);
    reinterpret_cast<void const **>(display)[5] = vtable_unk_080E65F4;
    display->state = 0;
    display->transition_timer = 0;
    display->thrown_item.Init();
    display->active = false;
    display->display_icon = 0;
    display->restart = false;
    display->alternate_idle = false;
    display->AEntity::vfunc_10();
    return display;
}

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

EC void func_0803260C(FarmerIconGraphics * graphics);
EC u32 func_0805E8F0(SpriteAnimator * animator);

EC void TickItemDisplayIcon(FarmerIconGraphics * graphics) ENTITY_CODE(0802BFA0);
EC void TickItemDisplayIcon(FarmerIconGraphics * graphics)
{
    func_0803260C(graphics);
    ItemDisplayEntity * display = *reinterpret_cast<ItemDisplayEntity **>(graphics);
    if (display->HasThrownItem())
    {
        if (graphics->icon_state.restart == 0)
        {
            if (i32(func_0805E8F0(&graphics->icon_animator) << 30) < 0)
                graphics->icon_state.enabled = 1;
        }
        else
            graphics->icon_state.restart = 0;
    }
}

EC ItemDisplayEntity * NewItemDisplayEntity(GameObject * game) ENTITY_CODE(0802C210);
EC ItemDisplayEntity * NewItemDisplayEntity(GameObject * game)
{
    return InitItemDisplayEntity(static_cast<ItemDisplayEntity *>(::operator new(sizeof(ItemDisplayEntity))), game);
}

EC void FarmerEntityInitNoOp(FarmerEntity * farmer) ENTITY_CODE(0802C2DC);
EC void FarmerEntityInitNoOp(FarmerEntity * farmer)
{
}

EC void TickFarmerToolCharge(FarmerEntity * farmer) ENTITY_CODE(0802C908);
EC void TickFarmerToolCharge(FarmerEntity * farmer)
{
    if (farmer->tool_charge_limit != 0 && farmer->tool_level < farmer->tool_charge_limit - 1)
    {
        if (++farmer->tool_charge_ticks == 20)
        {
            farmer->tool_charge_ticks = 0;
            u32 level = farmer->tool_level + 1;
            farmer->tool_level = level;
            if (static_cast<u8>(level) == 5)
                func_08008B6C(FindEntitySoundChannel(farmer->game_object), 184);
        }
    }
}

struct FarmerInputState
{
    STRUCT_PAD(0x00, 0x04);
    u16 direction_keys;
};

enum
{
    INPUT_A = 1 << 0,
    INPUT_B = 1 << 1,
    INPUT_RIGHT = 1 << 4,
    INPUT_LEFT = 1 << 5,
    INPUT_UP = 1 << 6,
    INPUT_DOWN = 1 << 7,
    INPUT_R = 1 << 8,
    INPUT_L = 1 << 9,
    TURN_INPUT_MASK = INPUT_A | INPUT_B | INPUT_RIGHT | INPUT_LEFT | INPUT_UP | INPUT_DOWN | INPUT_R | INPUT_L,
    FACING_DOWN = 0,
    FACING_UP = 1,
    FACING_LEFT = 2,
    FACING_RIGHT = 3,
    FARMER_TURN_STATE = 24,
};

EC void func_080323C8(AEntity * farmer);

EC void UpdateFarmerTurnInput(FarmerEntity * farmer, FarmerInputState const * input) ENTITY_CODE(0802D844);
EC void UpdateFarmerTurnInput(FarmerEntity * farmer, FarmerInputState const * input)
{
    u32 keys = input != nullptr ? input->direction_keys : 0;
    if (farmer->exhaustion_state != 0 || (keys & TURN_INPUT_MASK))
    {
        func_080323C8(farmer);
        farmer->state = FARMER_TURN_STATE;
        if (keys & INPUT_LEFT)
        {
            if (farmer->facing != FACING_LEFT)
                farmer->SetAnimFacing(FACING_LEFT);
        }
        else if (keys & INPUT_RIGHT)
        {
            if (farmer->facing != FACING_RIGHT)
                farmer->SetAnimFacing(FACING_RIGHT);
        }
        else if (keys & INPUT_UP)
        {
            if (farmer->facing != FACING_UP)
                farmer->SetAnimFacing(FACING_UP);
        }
        else if (keys & INPUT_DOWN)
        {
            if (farmer->facing != FACING_DOWN)
                farmer->SetAnimFacing(FACING_DOWN);
        }
    }
}

EC bool IsFarmerState7Or45(FarmerEntity const * farmer) ENTITY_CODE(0803205C);
EC bool IsFarmerState7Or45(FarmerEntity const * farmer)
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
