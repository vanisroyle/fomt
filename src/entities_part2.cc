#include "farmer_entity_layout.hh"
#include "entity_sound_channel.hh"
#include "farm_house.hh"

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
    void * (*get_actor_sprites)(GameObject * game);
};

EC void func_0805E850(SpriteAnimator * animator, void * sprites, u32 animation);
EC void func_0803260C(FarmerIconGraphics * graphics);
EC u32 func_0805E8F0(SpriteAnimator * animator);

inline void * FarmerItemSprites(FarmerIconGraphics * graphics)
{
    AEntity * entity = *reinterpret_cast<AEntity **>(graphics);
    GameObject * game = entity->game_object;
    FarmerIconGameVTable const * table = *reinterpret_cast<FarmerIconGameVTable const **>(game);
    return table->get_item_sprites(game);
}

inline void * FarmerActorSprites(FarmerIconGraphics * graphics)
{
    AEntity * entity = *reinterpret_cast<AEntity **>(graphics);
    GameObject * game = entity->game_object;
    FarmerIconGameVTable const * table = *reinterpret_cast<FarmerIconGameVTable const **>(game);
    return table->get_actor_sprites(game);
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

template<bool remember_icon>
inline void SetItemDisplayGraphicsIconFlags(FarmerIconState * state, u32 animation)
{
    u8 * flags = reinterpret_cast<u8 *>(state);
    int zero = 0;
    int enabled = 1;
    *flags = enabled;
    flags += 2;
    *flags = zero;
    ++flags;
    *flags = enabled;
    ++flags;
    if (remember_icon)
        *reinterpret_cast<u16 *>(flags) = animation;
}

EC void SetFarmerThrownItemIcon(FarmerEntity * farmer, u32 animation, i32 mode) ENTITY_CODE(0802A4C0);
EC void SetFarmerThrownItemIcon(FarmerEntity * farmer, u32 animation, i32 mode)
{
    ThrownItemMotion * item = &farmer->thrown_item;
    item->active = true;
    if (mode != 1)
    {
        if (mode != 0)
            goto show_icon;
    }
    item->display_mode = mode;
show_icon:
    FarmerIconGraphics * graphics = static_cast<FarmerIconGraphics *>(farmer->unk_10.Get());
    if (graphics != nullptr)
    {
        void * sprites = FarmerItemSprites(graphics);
        func_0805E850(&graphics->icon_animator, sprites, animation);
        SetItemDisplayGraphicsIconFlags<false>(&graphics->icon_state, animation);
    }
}

struct FarmerActionVTable
{
    STRUCT_PAD(0x00, 0x64);
    bool (*change_stamina_and_fatigue)(FarmerEntity * farmer, i32 stamina, i32 fatigue);
};

EC i32 IsDrink__C4Food(Food const * food);

EC void BeginFarmerEatingAnimation(FarmerEntity * farmer, Food const * food) ENTITY_CODE(0802A400);
EC void BeginFarmerEatingAnimation(FarmerEntity * farmer, Food const * food)
{
    farmer->item_animation_override = true;
    ThrownItemMotion * item = &farmer->thrown_item;
    item->active = true;
    item->display_mode = 0;
    item->Launch(0, 0, 21 * 65536);
    FarmerIconGraphics * graphics = static_cast<FarmerIconGraphics *>(farmer->unk_10.Get());
    if (graphics != nullptr)
    {
        u16 animation = food->GetIconId();
        void * sprites = FarmerItemSprites(graphics);
        func_0805E850(&graphics->icon_animator, sprites, animation);
        RestartFarmerIcon(&graphics->icon_state);
    }
    FarmerActionVTable const * actions = reinterpret_cast<FarmerActionVTable const **>(farmer)[5];
    i32 stamina = food->GetStaminaGain();
    i32 fatigue = food->GetFatigueGain();
    actions->change_stamina_and_fatigue(farmer, stamina, fatigue);
    if (!IsDrink__C4Food(food))
        farmer->state = 54;
    else
        farmer->state = 55;
    farmer->consume_animation_active = true;
    farmer->SetAnim(0x146);
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

struct FarmerToolInventory
{
    STRUCT_PAD(0x00, 0x380);
    ToolChest tool_chest;
    STRUCT_PAD(0x400, 0x1BD8);
    Farmer farmer;
};

EC u32 WateringCan_GetEffectiveLevel(u32 minimum, Tool const * tool);
EC u8 const gUnk_080F11B4[7];

EC u32 GetFarmerWateringCanCapacity(FarmerEntity const * farmer) ENTITY_CODE(0802AA84);
EC u32 GetFarmerWateringCanCapacity(FarmerEntity const * farmer)
{
    u32 level = 0;
    FarmerToolInventory const * inventory = reinterpret_cast<FarmerToolInventory const *>(farmer->game_state);
    ToolStack const * held_tool = &inventory->farmer.unk_5C;
    if (!held_tool->IsEmpty())
    {
        Tool tool = func_0800E550(reinterpret_cast<FarmerToolInventory const *>(farmer->game_state)->farmer, held_tool->GetTool());
        level = WateringCan_GetEffectiveLevel(0, &tool);
    }
    Rucksack const * rucksack = &reinterpret_cast<FarmerToolInventory const *>(farmer->game_state)->farmer.rucksack;
    for (u32 slot = 0; slot < rucksack->GetSize(); ++slot)
    {
        ToolStack const * stack = &rucksack->GetToolAt(slot);
        if (!stack->IsEmpty())
        {
            Tool tool = func_0800E550(reinterpret_cast<FarmerToolInventory const *>(farmer->game_state)->farmer, stack->GetTool());
            level = WateringCan_GetEffectiveLevel(level, &tool);
        }
    }
    ToolChest const * chest = &reinterpret_cast<FarmerToolInventory const *>(farmer->game_state)->tool_chest;
    for (u32 slot = 0; slot < ToolChest::CAPACITY; ++slot)
    {
        ToolStack const * stack = chest->GetToolStackAt(slot);
        if (!stack->IsEmpty())
        {
            Tool tool = func_0800E550(reinterpret_cast<FarmerToolInventory const *>(farmer->game_state)->farmer, stack->GetTool());
            level = WateringCan_GetEffectiveLevel(level, &tool);
        }
    }
    u8 capacities[7];
    memcpy(capacities, gUnk_080F11B4, sizeof(capacities));
    return capacities[level];
}

EC Box GetFarmerInteractionBox(FarmerEntity const * farmer, i32 facing) ENTITY_CODE(0802AB98);
EC Box GetFarmerInteractionBox(FarmerEntity const * farmer, i32 facing)
{
    switch (facing)
    {
    case 1: return Box(-4, -20, 4, 2, farmer->x_q16 >> 16, farmer->y_q16 >> 16);
    case 0: return Box(-4, -6, 4, 18, farmer->x_q16 >> 16, farmer->y_q16 >> 16);
    case 2: return Box(-20, -14, 4, 2, farmer->x_q16 >> 16, farmer->y_q16 >> 16);
    case 3: return Box(-4, -14, 20, 2, farmer->x_q16 >> 16, farmer->y_q16 >> 16);
    default: return Box(0, 0);
    }
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

struct ItemDisplayGraphics : FarmerIconGraphics
{
    u16 icon;
};

EC void func_08032560(UnknownEntityThing * graphics, AEntity * entity, u32 sprite_mode, void const * config,
    u32 priority, u32 layer, u32 x, u32 y, bool enabled);
EC void func_080A4A00(void * sprite, void * sprites, u32 animation, GameObject * game,
    u32 priority, u32 layer, bool mirrored);
EC char const gUnk_080F1224[];
EC char const vtable_unk_080E6644[];

EC ItemDisplayGraphics * InitItemDisplayGraphics(ItemDisplayGraphics * graphics, ItemDisplayEntity * display) ENTITY_CODE(0802BF2C);
EC ItemDisplayGraphics * InitItemDisplayGraphics(ItemDisplayGraphics * graphics, ItemDisplayEntity * display)
{
    func_08032560(graphics, display, 6, gUnk_080F1224, 3, 1, 0, 0, true);
    reinterpret_cast<void const **>(graphics)[1] = vtable_unk_080E6644;
    GameObject * game = display->game_object;
    FarmerIconGameVTable const * table = *reinterpret_cast<FarmerIconGameVTable const **>(game);
    void * sprites = table->get_item_sprites(game);
    GameObject * sprite_game = display->game_object;
    func_080A4A00(reinterpret_cast<u8 *>(graphics) + 0x8C, sprites, 0, sprite_game, 2, 1, false);
    graphics->icon = 0xFFFF;
    return graphics;
}

EC ItemDisplayGraphics * NewItemDisplayGraphics(ItemDisplayEntity * display) ENTITY_CODE(0802BA10);
EC ItemDisplayGraphics * NewItemDisplayGraphics(ItemDisplayEntity * display)
{
    return InitItemDisplayGraphics(static_cast<ItemDisplayGraphics *>(::operator new(sizeof(ItemDisplayGraphics))), display);
}

EC void func_0802BA74(ItemDisplayEntity * display);
EC void func_0803242C(AActorEntity * display, u32 argument);
EC u32 GetItemDisplayStateAnimation(ItemDisplayEntity const * display) ENTITY_CODE(0802BC48);

EC void TickItemDisplayEntity(ItemDisplayEntity * display, u32 argument) ENTITY_CODE(0802BA28);
EC void TickItemDisplayEntity(ItemDisplayEntity * display, u32 argument)
{
    if (!display->restart)
    {
        if (display->active)
        {
            func_0802BA74(display);
            u32 animation = static_cast<u16>(GetItemDisplayStateAnimation(display));
            if (display->anim_id != animation)
                display->SetAnim(animation);
        }
    }
    else
        display->restart = false;
    func_0803242C(display, argument);
}

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

EC void SetItemDisplayGraphicsIcon(ItemDisplayGraphics * graphics, u32 animation) ENTITY_CODE(0802C228);
EC void SetItemDisplayGraphicsIcon(ItemDisplayGraphics * graphics, u32 animation)
{
    void * sprites = FarmerItemSprites(graphics);
    func_0805E850(&graphics->icon_animator, sprites, animation);
    SetItemDisplayGraphicsIconFlags<true>(&graphics->icon_state, animation);
}

EC void SetFarmerEffectIcon(FarmerIconGraphics * graphics, u32 animation) ENTITY_CODE(0802C264);
EC void SetFarmerEffectIcon(FarmerIconGraphics * graphics, u32 animation)
{
    void * sprites = FarmerActorSprites(graphics);
    func_0805E850(&graphics->icon_animator, sprites, animation);
    SetItemDisplayGraphicsIconFlags<false>(&graphics->icon_state, animation);
}

EC void SetFarmerHeldItemIcon(FarmerIconGraphics * graphics, u32 animation) ENTITY_CODE(0802C29C);
EC void SetFarmerHeldItemIcon(FarmerIconGraphics * graphics, u32 animation)
{
    void * sprites = FarmerItemSprites(graphics);
    func_0805E850(&graphics->icon_animator, sprites, animation);
    SetItemDisplayGraphicsIconFlags<false>(&graphics->icon_state, animation);
}

EC bool IsItemDisplayActive(ItemDisplayEntity const * display) ENTITY_CODE(0802C2D4);
EC bool IsItemDisplayActive(ItemDisplayEntity const * display)
{
    return display->active;
}

EC void FarmerEntityInitNoOp(FarmerEntity * farmer) ENTITY_CODE(0802C2DC);
EC void FarmerEntityInitNoOp(FarmerEntity * farmer)
{
}

EC u32 FarmerEntity_GetToolFatiguePenalty(FarmerEntity * farmer);

EC void BeginFarmerToolDisplay(FarmerEntity * farmer) ENTITY_CODE(0802C860);
EC void BeginFarmerToolDisplay(FarmerEntity * farmer)
{
    u8 * state = &farmer->state;
    *state++ = 10;
    *state = 20;
    ShowFarmerToolDisplay(farmer);
    func_08008B6C(FindEntitySoundChannel(farmer->game_object), 188);
    Farmer const * model = farmer->farmer;
    Tool tool = func_0800E550(*model, model->unk_5C.GetTool());
    if (tool.GetId() == TOOL_TELEPORT_STONE)
    {
        FarmerActionVTable const * actions = reinterpret_cast<FarmerActionVTable const **>(farmer)[5];
        i32 stamina = -2;
        u32 fatigue = FarmerEntity_GetToolFatiguePenalty(farmer);
        actions->change_stamina_and_fatigue(farmer, stamina, fatigue);
    }
    else
    {
        FarmerActionVTable const * actions = reinterpret_cast<FarmerActionVTable const **>(farmer)[5];
        u32 fatigue = FarmerEntity_GetToolFatiguePenalty(farmer);
        actions->change_stamina_and_fatigue(farmer, 0, fatigue);
    }
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

struct FarmerGraphicsRefresh : FarmerIconGraphics
{
    STRUCT_PAD(0xCC, 0x281);
    bool refresh_icon;
};

EC void MarkFarmerGraphicsForRefresh(FarmerGraphicsRefresh * graphics) ENTITY_CODE(08031F20);
EC void MarkFarmerGraphicsForRefresh(FarmerGraphicsRefresh * graphics)
{
    graphics->refresh_icon = true;
}

EC void SetFarmerActorEffectIcon(FarmerIconGraphics * graphics, u32 animation) ENTITY_CODE(08031F30);
EC void SetFarmerActorEffectIcon(FarmerIconGraphics * graphics, u32 animation)
{
    void * sprites = FarmerActorSprites(graphics);
    func_0805E850(&graphics->icon_animator, sprites, animation);
    SetItemDisplayGraphicsIconFlags<false>(&graphics->icon_state, animation);
}

EC void SetFarmerItemEffectIcon(FarmerIconGraphics * graphics, u32 animation) ENTITY_CODE(08031F68);
EC void SetFarmerItemEffectIcon(FarmerIconGraphics * graphics, u32 animation)
{
    void * sprites = FarmerItemSprites(graphics);
    func_0805E850(&graphics->icon_animator, sprites, animation);
    SetItemDisplayGraphicsIconFlags<false>(&graphics->icon_state, animation);
}

struct FarmerInteractionGameVTable
{
    STRUCT_PAD(0x00, 0x40);
    AEntity * (*get_entity)(GameObject * game, u32 slot);
};

inline bool IsFarmerTargetSlot(u32 slot)
{
    bool is_target = false;
    if (slot - 43 <= 26)
        is_target = true;
    return is_target;
}

EC AEntity * GetFarmerInteractionTarget(FarmerEntity * farmer) ENTITY_CODE(08031FA0);
EC AEntity * GetFarmerInteractionTarget(FarmerEntity * farmer)
{
    u32 slot = farmer->interaction_target_slot;
    AEntity * target;
    if (!IsFarmerTargetSlot(slot))
        target = nullptr;
    else
    {
        GameObject * game = farmer->game_object;
        FarmerInteractionGameVTable const * table = *reinterpret_cast<FarmerInteractionGameVTable const **>(game);
        target = table->get_entity(game, slot);
    }
    return target;
}

EC bool IsFarmerInteractionTargetSlot(FarmerEntity const * farmer, u32 slot) ENTITY_CODE(08031FCC);
EC bool IsFarmerInteractionTargetSlot(FarmerEntity const *, u32 slot)
{
    return IsFarmerTargetSlot(slot);
}

struct FarmerToolOffset
{
    i32 x;
    i32 y;
};

EC void TransformFarmerToolOffset(FarmerEntity const * farmer, FarmerToolOffset * offset, i32 facing) ENTITY_CODE(08031FDC);
EC void TransformFarmerToolOffset(FarmerEntity const *, FarmerToolOffset * offset, i32 facing)
{
    switch (facing)
    {
    case FACING_DOWN:
    {
        facing = offset->x;
        i32 y = offset->y;
        offset->x = y;
        offset->y = facing;
        break;
    }
    case FACING_UP:
    {
        facing = offset->x;
        i32 y = offset->y;
        offset->x = -y;
        offset->y = -facing;
        break;
    }
    case FACING_LEFT:
        offset->x = -offset->x;
        break;
    }
}

EC ThrownItemMotion * GetFarmerThrownItemMotion(FarmerEntity * farmer) ENTITY_CODE(08032014);
EC ThrownItemMotion * GetFarmerThrownItemMotion(FarmerEntity * farmer)
{
    return &farmer->thrown_item;
}

EC bool IsFarmerUpdatePaused(FarmerEntity const * farmer) ENTITY_CODE(08032018);
EC bool IsFarmerUpdatePaused(FarmerEntity const * farmer)
{
    return farmer->update_paused;
}

EC void SetFarmerUpdatePaused(FarmerEntity * farmer, bool paused) ENTITY_CODE(08032020);
EC void SetFarmerUpdatePaused(FarmerEntity * farmer, bool paused)
{
    farmer->update_paused = paused;
}

EC FarmerHeldItemAnimation * GetFarmerHeldItemAnimation(FarmerEntity * farmer) ENTITY_CODE(08032028);
EC FarmerHeldItemAnimation * GetFarmerHeldItemAnimation(FarmerEntity * farmer)
{
    return &farmer->held_anim;
}

EC bool WasFarmerDiscardedItemSpecial(FarmerEntity const * farmer) ENTITY_CODE(08032034);
EC bool WasFarmerDiscardedItemSpecial(FarmerEntity const * farmer)
{
    return farmer->last_discard_special;
}

EC i32 GetFarmerLastDiscardedItemId(FarmerEntity const * farmer) ENTITY_CODE(0803203C);
EC i32 GetFarmerLastDiscardedItemId(FarmerEntity const * farmer)
{
    return farmer->last_discarded_item_id;
}

EC i32 GetFarmerLastDiscardedItemKind(FarmerEntity const * farmer) ENTITY_CODE(08032044);
EC i32 GetFarmerLastDiscardedItemKind(FarmerEntity const * farmer)
{
    return farmer->last_discarded_item_kind;
}

EC u8 GetFarmerTargetCowSlot(FarmerEntity const * farmer) ENTITY_CODE(0803204C);
EC u8 GetFarmerTargetCowSlot(FarmerEntity const * farmer)
{
    return farmer->target_cow_slot;
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

EC bool HasFarmerConsumeAnimation(FarmerEntity const * farmer) ENTITY_CODE(08032074);
EC bool HasFarmerConsumeAnimation(FarmerEntity const * farmer)
{
    return farmer->consume_animation_active;
}

EC void MakeFarmerContactBox(Box * box, i32 x, i32 y) ENTITY_CODE(0803207C);
EC void MakeFarmerContactBox(Box * box, i32 x, i32 y)
{
    box->x1 = x - 7;
    box->y1 = y - 9;
    box->x2 = x + 7;
    box->y2 = y + 5;
}

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
