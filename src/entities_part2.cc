#include "coop.hh"
#include "dog.hh"
#include "farmer_entity_layout.hh"
#include "entity_sound_channel.hh"
#include "farm_house.hh"
#include "shipping_bin.hh"
#include "sprite_animation.hh"

#define ENTITY_CODE(addr) SECTION(".text.entities_part2." #addr)

enum
{
    FARMER_IDLE_ANIMATION = 0x192,
    FARMER_STATE_7 = 7,
    FARMER_STATE_45 = 45,
};

enum
{
    FACING_DOWN = 0,
    FACING_UP = 1,
    FACING_LEFT = 2,
    FACING_RIGHT = 3,
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

struct FarmerStepEffectSlot
{
    u8 active;
    STRUCT_PAD(0x01, 0x2C);
    SpriteAnimator animator;
    FarmerIconState icon_state;
    u32 packed_position;
};

struct FarmerEffectGraphics : FarmerIconGraphics
{
    STRUCT_PAD(0xCC, 0xD0);
    FarmerStepEffectSlot step_effects[6];
    u8 next_step_effect;
};

EC void UpdateSideActorAnimationLayers(FarmerIconGraphics * graphics);
EC void TickFarmerEffectGraphics(FarmerEffectGraphics * graphics) ENTITY_CODE(0802B3B4);
EC void TickFarmerEffectGraphics(FarmerEffectGraphics * graphics)
{
    UpdateSideActorAnimationLayers(graphics);
    FarmerEntity * farmer = *reinterpret_cast<FarmerEntity **>(graphics);
    register u8 base_frame_changed asm("r3") = graphics->unk_44;
    register u16 * icon asm("r2") = &farmer->icon;
    register u16 current_icon asm("r1") = *icon;
    register u32 no_icon asm("r0") = 0xFFFF;
    if (current_icon != no_icon && base_frame_changed == 0)
    {
        no_icon |= current_icon;
        *icon = no_icon;
    }

    ThrownItemMotion const * item = &farmer->thrown_item;
    bool show_icon = false;
    if (item->active == 1)
        show_icon = true;
    if (show_icon)
    {
        if (graphics->icon_state.restart == 0)
        {
            if (i32(AdvanceSpriteAnimator(&graphics->icon_animator) << 30) < 0)
                graphics->icon_state.enabled = 1;
        }
        else
            graphics->icon_state.restart = 0;
    }

    u32 index = 0;
    u32 clear = 0;
    u8 * base = reinterpret_cast<u8 *>(graphics);
    u8 * animator = base + 0xFC;
    u8 * restart = base + 0x113;
    u8 * active = base + 0xD0;
    do
    {
        if (*active)
        {
            u32 status;
            FarmerIconState * icon_state = reinterpret_cast<FarmerIconState *>(animator + sizeof(SpriteAnimator));
            if (icon_state->restart == 0)
            {
                u32 flags = AdvanceSpriteAnimator(reinterpret_cast<SpriteAnimator *>(animator));
                if (i32(flags << 30) < 0)
                    icon_state->enabled = 1;
                status = flags;
            }
            else
            {
                *restart = clear;
                status = SpriteAnimator::FRAME_CHANGED;
                goto test_completion;
            }
        test_completion:
            if (i32(status << (31 - SpriteAnimator::LOOP_FLAG_BIT)) < 0)
                *active = clear;
        }
        animator += sizeof(FarmerStepEffectSlot);
        restart += sizeof(FarmerStepEffectSlot);
        active += sizeof(FarmerStepEffectSlot);
        ++index;
    } while (index <= 5);
}

struct FarmerIconGameVTable
{
    STRUCT_PAD(0x00, 0x64);
    void * (*get_item_sprites)(GameObject * game);
    void * (*get_actor_sprites)(GameObject * game);
};

EC void func_0805E850(SpriteAnimator * animator, void * sprites, u32 animation);
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

EC bool func_0800F388(HeldItem const *);
EC bool AEntity_IsOnOutdoorMap(AEntity const *);
EC u32 func_0800F344(HeldItem const *);
EC int func_0800F360(HeldItem const *);
EC void CopyHeldFood(Food *, HeldItem const *) asm("func_0800F20C");
EC u32 FarmerHeldItemIsEmptyRaw(HeldItem const *) asm("func_0800F190");
EC u32 ReadHeldArticleId(HeldItem const *) asm("func_0800F258");
EC u32 ReadDogGrowthStage(Dog const *) asm("GetGrowthStage__C3Dog");
EC u32 ReadChickenGrowthStage(Chicken const *) asm("GetGrowthStage__C7Chicken");
EC u32 ReadFoodIconId(Food const *) asm("GetIconId__C4Food");
EC u32 ReadArticleIconId(Article const *) asm("GetIconId__C7Article");

EC void RefreshFarmerHeldItemDisplay(FarmerEntity * farmer) ENTITY_CODE(0802A588);
EC void RefreshFarmerHeldItemDisplay(FarmerEntity * farmer)
{
    register FarmerEntity * self asm("r6") = farmer;
    asm volatile("" : "+r"(self));
    register Farmer * model asm("r0") = self->farmer;
    asm volatile("" : : "r"(model));
    register HeldItem * held asm("r4") = &model->unk_54;
    asm volatile("" : : "r"(held));
    register u32 hand_shift asm("r0") = FarmerHeldItemIsEmptyRaw(held) << 24;
    asm volatile("" : "+r"(hand_shift));
    u32 empty = hand_shift >> 24;
    asm volatile("" : "+r"(empty));
    if (empty)
        goto clear_item;
    {
        register ThrownItemMotion * motion asm("r0") = &self->thrown_item;
        asm volatile("" : : "r"(motion));
        register u32 enabled asm("r8") = 1;
        asm volatile("" : : "r"(enabled));
        register u32 active asm("r2") = enabled;
        asm volatile("" : : "r"(active));
        motion->active = active;
        motion->display_mode = empty;
        register i32 z_speed asm("r1") = 21 << 16;
        asm volatile("" : : "r"(z_speed));
        motion->x_speed = empty;
        motion->y_speed = empty;
        motion->z_speed = z_speed;
        register FarmerIconGraphics * icon_graphics asm("r0") = static_cast<FarmerIconGraphics *>(self->unk_10.Get());
        uintptr_t graphics_address = reinterpret_cast<uintptr_t>(icon_graphics);
        if (!graphics_address)
            return;
        register FarmerIconGraphics * graphics asm("r5") = icon_graphics;

        struct
        {
            u32 food;
            u8 article;
            u8 displayed_article;
        } item_values;
        register u8 * icon_flags asm("r0");
        register u32 icon_on asm("r1");
        register u32 item_icon asm("r4");
        void * sprites;
        SpriteAnimator * animator;
        u32 animation;
        asm volatile("" : : "r"(empty));
        if (func_0800F388(held))
        {
            void * bank = FarmerItemSprites(graphics);
            asm volatile("" : : "r"(bank));
            func_0805E850(&graphics->icon_animator, bank, 0x160);
            icon_flags = reinterpret_cast<u8 *>(&graphics->icon_state);
            icon_on = enabled;
            *icon_flags = icon_on;
            icon_flags += 2;
            asm volatile("" : : "r"(empty));
            *icon_flags = empty;
            asm volatile("" : : "m"(*icon_flags));
            goto restart_icon;
        }

        switch (func_0800F204(held))
        {
        case HeldItem::KIND_FOOD:
        {
            Food * food = reinterpret_cast<Food *>(&item_values.food);
            CopyHeldFood(food, held);
            item_icon = ReadFoodIconId(food);
            asm volatile("" : "+r"(item_icon));
            item_icon <<= 16;
            asm volatile("" : : "r"(item_icon));
            item_icon >>= 16;
            goto item_sprite;
        }
        case HeldItem::KIND_ARTICLE:
        {
            register u32 article_id asm("r1") = ReadHeldArticleId(held);
            asm volatile("" : : "r"(article_id));
            register u8 * article_ptr asm("r0") = &item_values.article;
            *article_ptr = article_id;
            Article & article = *reinterpret_cast<Article *>(article_ptr);
            if (article.GetId() == ARTICLE_ALEXANDRITE)
            {
                u8 outdoors = AEntity_IsOnOutdoorMap(self);
                if (outdoors)
                {
                    sprites = FarmerItemSprites(graphics);
                    animator = &graphics->icon_animator;
                    animation = 4;
                    goto set_animation;
                }
                void * bank = FarmerItemSprites(graphics);
                asm volatile("" : : "r"(bank));
                func_0805E850(&graphics->icon_animator, bank, 3);
                icon_flags = reinterpret_cast<u8 *>(&graphics->icon_state);
                icon_on = 1;
                *icon_flags = icon_on;
                icon_flags += 2;
                *icon_flags = outdoors;
                asm volatile("" : : "m"(*icon_flags));
                goto restart_icon;
            }
            register u32 displayed_id asm("r1") = ReadHeldArticleId(held);
            asm volatile("" : : "r"(displayed_id));
            register u8 * displayed_article asm("r0") = &item_values.displayed_article;
            *displayed_article = displayed_id;
            item_icon = ReadArticleIconId(reinterpret_cast<Article *>(displayed_article));
            asm volatile("" : "+r"(item_icon));
            item_icon <<= 16;
            asm volatile("" : : "r"(item_icon));
            item_icon >>= 16;
            goto item_sprite;
        }
        case HeldItem::KIND_2:
        {
            Dog * dog = reinterpret_cast<Dog *>(reinterpret_cast<u8 *>(self->game_state) + 0x1C70);
            register u32 stage asm("r4") = ReadDogGrowthStage(dog);
            asm volatile("" : "+r"(stage));
            if (stage == 0)
            {
                void * bank = FarmerActorSprites(graphics);
                asm volatile("" : : "r"(bank));
                func_0805E850(&graphics->icon_animator, bank, 0x3DA);
                icon_flags = reinterpret_cast<u8 *>(&graphics->icon_state);
                icon_on = 1;
                *icon_flags = icon_on;
                icon_flags += 2;
                *icon_flags = stage;
                asm volatile("" : : "m"(*icon_flags));
                goto restart_icon;
            }
            sprites = FarmerActorSprites(graphics);
            animator = &graphics->icon_animator;
            animation = 0x374;
            goto set_animation;
        }
        case HeldItem::KIND_3:
        {
            register u32 chicken_index asm("r1") = func_0800F344(held);
            asm volatile("" : : "r"(chicken_index));
            Coop * coop = reinterpret_cast<Coop *>(reinterpret_cast<u8 *>(self->game_state) + 0x410);
            Chicken * chicken = coop->GetChicken(chicken_index);
            register u32 stage asm("r4") = ReadChickenGrowthStage(chicken);
            asm volatile("" : "+r"(stage));
            if (stage == 0)
            {
                void * bank = FarmerActorSprites(graphics);
                asm volatile("" : : "r"(bank));
                func_0805E850(&graphics->icon_animator, bank, 0x73D);
                icon_flags = reinterpret_cast<u8 *>(&graphics->icon_state);
                icon_on = 1;
                *icon_flags = icon_on;
                icon_flags += 2;
                *icon_flags = stage;
                asm volatile("" : : "m"(*icon_flags));
                goto restart_icon;
            }
            sprites = FarmerActorSprites(graphics);
            animator = &graphics->icon_animator;
            animation = 0x734;
            goto set_animation;
        }
        case HeldItem::KIND_4:
            sprites = FarmerItemSprites(graphics);
            animator = &graphics->icon_animator;
            animation = 0x35;
            goto set_animation;
        case HeldItem::KIND_5:
            item_icon = func_0800F360(held);
            goto item_sprite;
        default:
            return;
        }

item_sprite:
        sprites = FarmerItemSprites(graphics);
        animator = &graphics->icon_animator;
        animation = item_icon;
set_animation:
        func_0805E850(animator, sprites, animation);
        icon_flags = reinterpret_cast<u8 *>(&graphics->icon_state);
        {
            register u32 frame_zero asm("r2") = 0;
            asm volatile("" : : "r"(frame_zero));
            icon_on = 1;
            *icon_flags = icon_on;
            icon_flags += 2;
            *icon_flags = frame_zero;
        }
restart_icon:
        icon_flags += 1;
        asm volatile("" : "+r"(icon_flags));
        icon_flags[0] = icon_on;
        return;
    }
clear_item:
    {
        register ThrownItemMotion * clear_to asm("r1") = &self->thrown_item;
        asm volatile("" : : "r"(clear_to));
        clear_to->active = false;
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

struct FarmerInteractionGameVTable
{
    STRUCT_PAD(0x00, 0x40);
    AEntity * (*get_entity)(GameObject * game, u32 slot);
};

struct FarmerTargetBounds
{
    i16 x1, y1, x2, y2;
};
struct FarmerTargetSearchFrame
{
    FarmerTargetBounds interaction_box;
    FarmerTargetBounds bounds;
    FarmerTargetBounds actor_bounds;
    GameObject * game;
    u32 map;
    i32 x, y;
    u32 best_slot;
    i32 best_distance;
    u32 current_slot;
    u8 accepted;
};
EC void ReadFarmerInteractionBox(FarmerTargetBounds *, FarmerEntity const *, i32) asm("GetFarmerInteractionBox");
struct FarmerTargetActorVTable
{
    STRUCT_PAD(0x00, 0x0C);
    void (*get_bounds)(FarmerTargetBounds *, AEntity const *);
};

EC u16 const FarmerToolLevelAnimations[6][7] asm("gUnk_080F0FCC");
EC u32 ReadResolvedToolIdRaw(Farmer const &, Tool) asm("func_0800E550");
EC u32 ReadFarmerToolIconRaw(Tool const *) asm("GetIconId__C4Tool");
EC void FarmerEntity_LiftObject(FarmerEntity *);

EC u32 TryStartFarmerObjectLift(FarmerEntity *) ENTITY_CODE(08029558);
EC u32 TryStartFarmerObjectLift(FarmerEntity * farmer)
{
    register FarmerEntity * self asm("r6") = farmer;
    register bool * item_override asm("r0") = &farmer->item_animation_override;
    asm volatile("" : : "r"(item_override), "r"(self));
    register u32 lift_flag asm("r4") = 1;
    asm volatile("" : : "r"(lift_flag), "r"(item_override));
    *item_override = lift_flag;
    register HeldItem const * game_held_item asm("r0") =
        reinterpret_cast<HeldItem const *>(reinterpret_cast<u8 const *>(self->game_state) + 0x1C2C);
    u32 is_game_held_item_empty = FarmerHeldItemIsEmptyRaw(game_held_item) << 24;
    if (!is_game_held_item_empty)
        return 0;
    {
        register bool * consume_animation asm("r0") = &self->consume_animation_active;
        asm volatile("" : : "r"(consume_animation), "r"(lift_flag));
        *consume_animation = lift_flag;
    }
    FarmerEntity_LiftObject(self);
    register u32 animation asm("r5") = FARMER_IDLE_ANIMATION;
    u16 * icon_field;
    u32 state_code;
    {
        register u16 * icon_address asm("r1") = &self->icon;
        asm volatile("" : : "r"(icon_address), "r"(animation));
        register u32 hidden asm("r0") = 0xFFFF;
        asm volatile("" : : "r"(hidden), "r"(icon_address));
        *icon_address = hidden;
        register u8 const * state_address asm("r0") = &self->state;
        asm volatile("" : : "r"(state_address));
        register u32 state asm("r0") = *state_address;
        asm volatile("" : : "r"(state));
        icon_field = icon_address;
        asm volatile("" : "+r"(icon_field) : "r"(state));
        state_code = state;
    }
    register AActorEntity * horse asm("r2");
    register u32 horse_animation asm("r1");
    typedef u8 EncodedTool;
    register EncodedTool * stored_tool_id asm("r0");
    register u32 tool_id asm("r1");
    Tool * icon_tool;
    register u8 const * previous_tool_step asm("r2");
    register u8 const * current_tool_step asm("r1");
    EncodedTool tool_values[8];
    switch (state_code)
    {
    case 0:
        if (!func_0800F190(&self->farmer->unk_54))
        {
            i32 motion = self->motion;
            asm volatile("" : "+r"(motion) : "r"(self));
            switch (motion)
            {
            case 1: goto hold_walk;
            case 0: goto hold_still;
            case 2: animation = 0x14E; goto done;
            default: goto done;
            }
        }
        else
        {
            i32 motion = self->motion;
            asm volatile("" : "+r"(motion) : "r"(self));
            register u32 motion_animation asm("r1");
            switch (motion)
            {
            case 0: goto standing;
            case 1:
                {
                    u32 footstep = self->unk_B3;
                    asm volatile("" : : "r"(footstep), "r"(self));
                    motion_animation = 0x132;
                    asm volatile("" : "+r"(motion_animation));
                    asm volatile("" : "+r"(footstep) : "r"(motion_animation));
                    if (!footstep)
                        motion_animation += 0x94;
                    goto set_motion_animation;
                }
            case 2:
                {
                    u32 footstep = self->unk_B3;
                    asm volatile("" : : "r"(footstep), "r"(self));
                    motion_animation = 0x132;
                    asm volatile("" : "+r"(motion_animation));
                    asm volatile("" : "+r"(footstep) : "r"(motion_animation));
                    if (!footstep)
                        motion_animation += 0x2C;
                }
            set_motion_animation:
                animation = motion_animation;
                goto done;
            default: goto done;
            }
        }
    case 2:
        if (!func_0800F190(&self->farmer->unk_54))
            goto hold_still;
        {
            register i32 tool_state asm("r3") = self->tool_state;
            asm volatile("" : "+r"(tool_state));
            if (tool_state > 5)
                goto standing;
            asm volatile("" : "+r"(tool_state));
            if (tool_state < 0)
                goto standing;
            register u16 const * animations asm("r2") = &FarmerToolLevelAnimations[0][0];
            asm volatile("" : : "r"(animations));
            register u8 const * level_address asm("r1") = &self->tool_level;
            asm volatile("" : : "r"(level_address));
            register i32 row asm("r0") = tool_state * 7;
            asm volatile("" : : "r"(row));
            animation = animations[row + *level_address];
        }
        {
            Farmer * inventory = self->farmer;
            register u32 id asm("r0") = ReadResolvedToolIdRaw(*inventory, inventory->unk_5C.GetTool());
            register u8 * stored_tool asm("r1") = &tool_values[0];
            asm volatile("" : : "r"(stored_tool), "r"(id));
            *stored_tool = id;
            stored_tool_id = &tool_values[0];
            goto update_tool_icon;
        }
    case 4:
        animation = FARMER_IDLE_ANIMATION;
        {
            u32 empty = FarmerHeldItemIsEmptyRaw(&self->farmer->unk_54) << 24;
            asm volatile("" : "+r"(empty));
            animation = FARMER_IDLE_ANIMATION;
            asm volatile("" : "+r"(animation));
            asm volatile("" : "+r"(empty) : "r"(animation));
            if (!empty)
            {
                asm volatile("");
                goto hold_still;
            }
            goto done;
        }
    case 1: case 3: case 5:
        {
            u32 empty = FarmerHeldItemIsEmptyRaw(&self->farmer->unk_54) << 24;
            asm volatile("" : "+r"(empty));
            animation = FARMER_IDLE_ANIMATION;
            asm volatile("" : "+r"(animation));
            asm volatile("" : "+r"(empty) : "r"(animation));
            if (!empty)
            {
                animation -= 0x40;
                asm volatile("" : "+r"(animation));
            }
            break;
        }
    case 0xD: animation = 0x136; break;
    case 0xB: case 0xC: case 0xE: animation = 0x19E; break;
    case 0xF: animation = 0x4E; break;
    case 0x10: animation = 0x46; break;
    case 0x13: animation = 0x13E; break;
    case 0x14: animation = 0x142; break;
    case 9:
        animation = self->unk_44;
        {
            Farmer * inventory = self->farmer;
            tool_id = ReadResolvedToolIdRaw(*inventory, inventory->unk_5C.GetTool());
            stored_tool_id = &tool_values[1];
            goto store_tool_id;
        }
    case 0x1B:
        animation = 0x1EA;
        {
            Farmer * inventory = self->farmer;
            tool_id = ReadResolvedToolIdRaw(*inventory, inventory->unk_5C.GetTool());
            stored_tool_id = &tool_values[2];
            goto store_tool_id;
        }
    case 0x1C: case 0x1D:
        {
            previous_tool_step = reinterpret_cast<u8 const *>(self) + 0x4E;
            asm volatile("" : "+r"(previous_tool_step));
            current_tool_step = reinterpret_cast<u8 const *>(self) + 0x4F;
            asm volatile("" : "+r"(current_tool_step));
            if (*previous_tool_step == *current_tool_step)
            {
            standing:
                animation = FARMER_IDLE_ANIMATION;
                goto done;
            }
            {
                register u32 new_frame asm("r0") = *current_tool_step;
                asm volatile("" : : "r"(new_frame));
                register u32 last_frame asm("r1") = *previous_tool_step;
                asm volatile("" : : "r"(last_frame), "r"(new_frame));
                register i32 delta asm("r0") = new_frame - last_frame;
                asm volatile("" : : "r"(delta));
                animation = 0x182;
                asm volatile("" : "+r"(animation));
                if (delta == 1) animation += 4;
            }
            Farmer * inventory = self->farmer;
            tool_id = ReadResolvedToolIdRaw(*inventory, inventory->unk_5C.GetTool());
            stored_tool_id = &tool_values[3];
            goto store_tool_id;
        }
    case 0x1E:
        animation = 0x76;
        {
            Farmer * inventory = self->farmer;
            tool_id = ReadResolvedToolIdRaw(*inventory, inventory->unk_5C.GetTool());
            stored_tool_id = &tool_values[4];
            goto store_tool_id;
        }
    case 0x1F:
        animation = 0x6E;
        {
            Farmer * inventory = self->farmer;
            tool_id = ReadResolvedToolIdRaw(*inventory, inventory->unk_5C.GetTool());
            stored_tool_id = &tool_values[5];
            goto store_tool_id;
        }
    case 0x20:
        animation = 0x72;
        {
            Farmer * inventory = self->farmer;
            tool_id = ReadResolvedToolIdRaw(*inventory, inventory->unk_5C.GetTool());
            stored_tool_id = &tool_values[6];
            goto store_tool_id;
        }
    store_tool_id:
        *stored_tool_id = tool_id;
    update_tool_icon:
        icon_tool = reinterpret_cast<Tool *>(stored_tool_id);
        {
            register u32 icon_id asm("r0") = ReadFarmerToolIconRaw(icon_tool);
            asm volatile("" : : "r"(icon_id));
            *icon_field = icon_id;
        }
        goto done;
    case 0x16: animation = 0x1FA; break;
    case 0x17: animation = 0x196; break;
    case 6: animation = 0x112; break;
    case 0x18: animation = 0x116; break;
    case 7:
        {
            GameObject * game = self->game_object;
            FarmerInteractionGameVTable const * methods = *reinterpret_cast<FarmerInteractionGameVTable const **>(game);
            register AEntity * (*get_entity)(GameObject *, u32) asm("r2") = methods->get_entity;
            asm volatile("" : : "r"(get_entity));
            horse = static_cast<AActorEntity *>(get_entity(game, 0x2C));
            register u8 const * motion_address asm("r0") = &self->motion;
            asm volatile("" : : "r"(motion_address), "r"(horse));
            i32 motion = *motion_address;
            asm volatile("" : "+r"(motion) : "r"(horse));
            switch (motion)
            {
            case 0: animation = 0xEA; horse_animation = 0x77A; asm volatile("" : : "r"(horse), "r"(horse_animation), "r"(animation)); goto update_horse_animation;
            case 1: animation = 0xEE; horse_animation = 0x77E; asm volatile("" : : "r"(horse), "r"(horse_animation), "r"(animation)); goto update_horse_animation;
            case 2: animation = 0xE6; horse_animation = 0x782; asm volatile("" : : "r"(horse), "r"(horse_animation), "r"(animation)); goto update_horse_animation;
            }
            break;
        }
    case 0x22:
        animation = 0x15A;
        {
            GameObject * game = self->game_object;
            FarmerInteractionGameVTable const * methods = *reinterpret_cast<FarmerInteractionGameVTable const **>(game);
            register AEntity * (*get_entity)(GameObject *, u32) asm("r2") = methods->get_entity;
            asm volatile("" : : "r"(get_entity));
            horse = static_cast<AActorEntity *>(get_entity(game, 0x2C));
            horse_animation = 0x77A;
            asm volatile("" : : "r"(horse), "r"(horse_animation), "r"(animation));
            goto update_horse_animation;
        }
    case 0x23:
        animation = 0x9A;
        {
            GameObject * game = self->game_object;
            FarmerInteractionGameVTable const * methods = *reinterpret_cast<FarmerInteractionGameVTable const **>(game);
            register AEntity * (*get_entity)(GameObject *, u32) asm("r2") = methods->get_entity;
            asm volatile("" : : "r"(get_entity));
            horse = static_cast<AActorEntity *>(get_entity(game, 0x2C));
            horse_animation = 0x77A;
            asm volatile("" : : "r"(horse), "r"(horse_animation), "r"(animation));
            goto update_horse_animation;
        }
    case 0x24: animation = 0x106; break;
    case 0x25: animation = 0x102; break;
    case 0x26: animation = 0xFE; break;
    case 0x27: animation = 0xFA; break;
    case 0x28: animation = 0x122; break;
    case 0x29: animation = 0x11E; break;
    case 0x2A: case 0x2B: animation = 0x11A; break;
    case 0x2C:
        {
            u32 empty = FarmerHeldItemIsEmptyRaw(&self->farmer->unk_54) << 24;
            asm volatile("" : "+r"(empty));
            animation = 0x14A;
            asm volatile("" : "+r"(animation));
            asm volatile("" : "+r"(empty) : "r"(animation));
            if (empty)
            {
                animation -= 0x40;
                asm volatile("" : "+r"(animation));
            }
            break;
        }
    case 0x2D:
        animation = 0xE2;
        {
            GameObject * game = self->game_object;
            FarmerInteractionGameVTable const * methods = *reinterpret_cast<FarmerInteractionGameVTable const **>(game);
            register AEntity * (*get_entity)(GameObject *, u32) asm("r2") = methods->get_entity;
            asm volatile("" : : "r"(get_entity));
            horse = static_cast<AActorEntity *>(get_entity(game, 0x2C));
            horse_animation = 0x792;
            asm volatile("" : : "r"(horse), "r"(horse_animation), "r"(animation));
            goto update_horse_animation;
        }
    update_horse_animation:
        if (horse->anim_id != horse_animation)
            horse->SetAnim(horse_animation);
        goto done;
    case 0x2E: animation = 0x3E; break;
    case 0x11: case 0x21: case 0x31: case 0x36: case 0x37: animation = 0x146; break;
    case 0x19: case 0x1A: goto standing;
    case 0x30: case 0x32: case 0x38: case 0x39:
    hold_still:
        animation = 0x152;
        goto done;
    case 0x12: case 0x33: animation = 0x13A; break;
    case 0xA: case 0x15: case 0x2F: case 0x34: animation = 0x42; break;
    case 0x35:
        {
            u32 empty = FarmerHeldItemIsEmptyRaw(&self->farmer->unk_54) << 24;
            asm volatile("" : "+r"(empty));
            animation = 0x1C6;
            asm volatile("" : "+r"(animation));
            asm volatile("" : "+r"(empty) : "r"(animation));
            if (!empty)
                goto hold_walk;
            goto done;
        }
    hold_walk:
        animation = 0x156;
        goto done;
    case 8:
        {
            u32 variant = self->unk_3D;
            asm volatile("" : : "r"(variant), "r"(self));
            animation = 0xF6;
            asm volatile("" : "+r"(animation), "+r"(variant));
            if (variant)
                animation = 0xF2;
            goto done;
        }
    default:
        break;
    }
    goto done;
done:
    asm volatile("" : : "r"(animation));
    self->SetAnim(static_cast<u16>(animation));
    return 1;
}

EC u32 SelectFarmerAnimation(FarmerEntity *) ENTITY_CODE(0802D158);
EC u32 SelectFarmerAnimation(FarmerEntity * farmer)
{
    register FarmerEntity * self asm("r5") = farmer;
    register u32 animation asm("r6") = FARMER_IDLE_ANIMATION;
    u32 state_code;
    {
        u8 * field = reinterpret_cast<u8 *>(farmer);
        asm volatile("" : "+r"(field) : "r"(animation), "r"(self));
        field += 0x7C;
        asm volatile("" : "+r"(field));
        register u32 no_icon asm("r1") = 0xFFFF;
        *reinterpret_cast<u16 *>(field) = no_icon;
        state_code = static_cast<volatile u8 *>(field)[-0x40];
    }
    register AActorEntity * horse asm("r2");
    register u32 horse_animation asm("r1");
    typedef u8 EncodedTool;
    register EncodedTool * stored_tool_id asm("r0");
    register u32 tool_id asm("r1");
    Tool * icon_tool;
    register u8 const * previous_tool_step asm("r2");
    register u8 const * current_tool_step asm("r1");
    EncodedTool tool_values[8];
    switch (state_code)
    {
    case 0:
        if (!func_0800F190(&self->farmer->unk_54))
        {
            register i32 motion asm("r0") = self->motion;
            asm volatile("" : "+r"(motion) : "r"(self));
            switch (motion)
            {
            case 1: goto hold_walk;
            case 0: goto hold_still;
            case 2: animation = 0x14E; goto done;
            default: goto done;
            }
        }
        else
        {
            register i32 motion asm("r0") = self->motion;
            asm volatile("" : "+r"(motion) : "r"(self));
            register u32 motion_animation asm("r1");
            switch (motion)
            {
            case 0: goto standing;
            case 1:
                {
                    register u32 footstep asm("r0") = self->unk_B3;
                    asm volatile("" : : "r"(footstep), "r"(self));
                    motion_animation = 0x132;
                    asm volatile("" : "+r"(motion_animation));
                    if (!footstep)
                        motion_animation += 0x94;
                    goto set_motion_animation;
                }
            case 2:
                {
                    register u32 footstep asm("r0") = self->unk_B3;
                    asm volatile("" : : "r"(footstep), "r"(self));
                    motion_animation = 0x132;
                    asm volatile("" : "+r"(motion_animation));
                    if (!footstep)
                        motion_animation += 0x2C;
                }
            set_motion_animation:
                animation = motion_animation;
                goto done;
            default: goto done;
            }
        }
    case 2:
        if (!func_0800F190(&self->farmer->unk_54))
            goto hold_still;
        {
            register i32 tool_state asm("r3") = self->tool_state;
            asm volatile("" : "+r"(tool_state));
            if (tool_state > 5)
                goto standing;
            asm volatile("" : "+r"(tool_state));
            if (tool_state < 0)
                goto standing;
            register u16 const * animations asm("r2") = &FarmerToolLevelAnimations[0][0];
            asm volatile("" : : "r"(animations));
            register u8 const * level_address asm("r1") = &self->tool_level;
            asm volatile("" : : "r"(level_address));
            register i32 row asm("r0") = tool_state * 7;
            asm volatile("" : : "r"(row));
            animation = animations[row + *level_address];
        }
        {
            Farmer * inventory = self->farmer;
            register u32 id asm("r0") = ReadResolvedToolIdRaw(*inventory, inventory->unk_5C.GetTool());
            register u8 * stored_tool asm("r1") = &tool_values[0];
            asm volatile("" : : "r"(stored_tool), "r"(id));
            *stored_tool = id;
            stored_tool_id = &tool_values[0];
            goto update_tool_icon;
        }
    case 4:
        animation = FARMER_IDLE_ANIMATION;
        {
            register u32 empty asm("r0") = FarmerHeldItemIsEmptyRaw(&self->farmer->unk_54) << 24;
            asm volatile("" : "+r"(empty));
            animation = FARMER_IDLE_ANIMATION;
            asm volatile("" : "+r"(animation));
            if (empty)
                goto done;
            goto hold_still;
        }
    case 1: case 3: case 5:
        {
            register u32 empty asm("r0") = FarmerHeldItemIsEmptyRaw(&self->farmer->unk_54) << 24;
            asm volatile("" : "+r"(empty));
            animation = FARMER_IDLE_ANIMATION;
            asm volatile("" : "+r"(animation));
            if (!empty)
                animation -= 0x40;
            break;
        }
    case 0xD: animation = 0x136; break;
    case 0xB: case 0xC: case 0xE: animation = 0x19E; break;
    case 0xF: animation = 0x4E; break;
    case 0x10: animation = 0x46; break;
    case 0x13: animation = 0x13E; break;
    case 0x14: animation = 0x142; break;
    case 9:
        animation = self->unk_44;
        {
            Farmer * inventory = self->farmer;
            tool_id = ReadResolvedToolIdRaw(*inventory, inventory->unk_5C.GetTool());
            stored_tool_id = &tool_values[1];
            goto store_tool_id;
        }
    case 0x1B:
        animation = 0x1EA;
        {
            Farmer * inventory = self->farmer;
            tool_id = ReadResolvedToolIdRaw(*inventory, inventory->unk_5C.GetTool());
            stored_tool_id = &tool_values[2];
            goto store_tool_id;
        }
    case 0x1C: case 0x1D:
        {
            previous_tool_step = reinterpret_cast<u8 const *>(self) + 0x4E;
            asm volatile("" : "+r"(previous_tool_step));
            current_tool_step = reinterpret_cast<u8 const *>(self) + 0x4F;
            asm volatile("" : "+r"(current_tool_step));
            if (*previous_tool_step == *current_tool_step)
            {
            standing:
                animation = FARMER_IDLE_ANIMATION;
                goto done;
            }
            {
                register u32 new_frame asm("r0") = *current_tool_step;
                asm volatile("" : : "r"(new_frame));
                register u32 last_frame asm("r1") = *previous_tool_step;
                asm volatile("" : : "r"(last_frame), "r"(new_frame));
                register i32 delta asm("r0") = new_frame - last_frame;
                asm volatile("" : : "r"(delta));
                animation = 0x182;
                asm volatile("" : "+r"(animation));
                if (delta == 1) animation += 4;
            }
            Farmer * inventory = self->farmer;
            tool_id = ReadResolvedToolIdRaw(*inventory, inventory->unk_5C.GetTool());
            stored_tool_id = &tool_values[3];
            goto store_tool_id;
        }
    case 0x1E:
        animation = 0x76;
        {
            Farmer * inventory = self->farmer;
            tool_id = ReadResolvedToolIdRaw(*inventory, inventory->unk_5C.GetTool());
            stored_tool_id = &tool_values[4];
            goto store_tool_id;
        }
    case 0x1F:
        animation = 0x6E;
        {
            Farmer * inventory = self->farmer;
            tool_id = ReadResolvedToolIdRaw(*inventory, inventory->unk_5C.GetTool());
            stored_tool_id = &tool_values[5];
            goto store_tool_id;
        }
    case 0x20:
        animation = 0x72;
        {
            Farmer * inventory = self->farmer;
            tool_id = ReadResolvedToolIdRaw(*inventory, inventory->unk_5C.GetTool());
            stored_tool_id = &tool_values[6];
            goto store_tool_id;
        }
    store_tool_id:
        *stored_tool_id = tool_id;
    update_tool_icon:
        icon_tool = reinterpret_cast<Tool *>(stored_tool_id);
        {
            register u32 icon_id asm("r0") = ReadFarmerToolIconRaw(icon_tool);
            asm volatile("" : : "r"(icon_id));
            register u16 * icon_field asm("r1") = &self->icon;
            asm volatile("" : : "r"(icon_field));
            *icon_field = icon_id;
        }
        goto done;
    case 0x16: animation = 0x1FA; break;
    case 0x17: animation = 0x196; break;
    case 6: animation = 0x112; break;
    case 0x18: animation = 0x116; break;
    case 7:
        {
            GameObject * game = self->game_object;
            FarmerInteractionGameVTable const * methods = *reinterpret_cast<FarmerInteractionGameVTable const **>(game);
            register AEntity * (*get_entity)(GameObject *, u32) asm("r2") = methods->get_entity;
            asm volatile("" : : "r"(get_entity));
            horse = static_cast<AActorEntity *>(get_entity(game, 0x2C));
            register u8 const * motion_address asm("r0") = &self->motion;
            asm volatile("" : : "r"(motion_address), "r"(horse));
            register i32 motion asm("r0") = *motion_address;
            asm volatile("" : "+r"(motion) : "r"(horse));
            switch (motion)
            {
            case 0: animation = 0xEA; horse_animation = 0x77A; asm volatile("" : : "r"(horse), "r"(horse_animation), "r"(animation)); goto update_horse_animation;
            case 1: animation = 0xEE; horse_animation = 0x77E; asm volatile("" : : "r"(horse), "r"(horse_animation), "r"(animation)); goto update_horse_animation;
            case 2: animation = 0xE6; horse_animation = 0x782; asm volatile("" : : "r"(horse), "r"(horse_animation), "r"(animation)); goto update_horse_animation;
            }
            break;
        }
    case 0x22:
        animation = 0x15A;
        {
            GameObject * game = self->game_object;
            FarmerInteractionGameVTable const * methods = *reinterpret_cast<FarmerInteractionGameVTable const **>(game);
            register AEntity * (*get_entity)(GameObject *, u32) asm("r2") = methods->get_entity;
            asm volatile("" : : "r"(get_entity));
            horse = static_cast<AActorEntity *>(get_entity(game, 0x2C));
            horse_animation = 0x77A;
            asm volatile("" : : "r"(horse), "r"(horse_animation), "r"(animation));
            goto update_horse_animation;
        }
    case 0x23:
        animation = 0x9A;
        {
            GameObject * game = self->game_object;
            FarmerInteractionGameVTable const * methods = *reinterpret_cast<FarmerInteractionGameVTable const **>(game);
            register AEntity * (*get_entity)(GameObject *, u32) asm("r2") = methods->get_entity;
            asm volatile("" : : "r"(get_entity));
            horse = static_cast<AActorEntity *>(get_entity(game, 0x2C));
            horse_animation = 0x77A;
            asm volatile("" : : "r"(horse), "r"(horse_animation), "r"(animation));
            goto update_horse_animation;
        }
    case 0x24: animation = 0x106; break;
    case 0x25: animation = 0x102; break;
    case 0x26: animation = 0xFE; break;
    case 0x27: animation = 0xFA; break;
    case 0x28: animation = 0x122; break;
    case 0x29: animation = 0x11E; break;
    case 0x2A: case 0x2B: animation = 0x11A; break;
    case 0x2C:
        {
            register u32 empty asm("r0") = FarmerHeldItemIsEmptyRaw(&self->farmer->unk_54) << 24;
            asm volatile("" : "+r"(empty));
            animation = 0x14A;
            asm volatile("" : "+r"(animation));
            if (empty)
                animation -= 0x40;
            break;
        }
    case 0x2D:
        animation = 0xE2;
        {
            GameObject * game = self->game_object;
            FarmerInteractionGameVTable const * methods = *reinterpret_cast<FarmerInteractionGameVTable const **>(game);
            register AEntity * (*get_entity)(GameObject *, u32) asm("r2") = methods->get_entity;
            asm volatile("" : : "r"(get_entity));
            horse = static_cast<AActorEntity *>(get_entity(game, 0x2C));
            horse_animation = 0x792;
            asm volatile("" : : "r"(horse), "r"(horse_animation), "r"(animation));
            goto update_horse_animation;
        }
    update_horse_animation:
        if (horse->anim_id != horse_animation)
            horse->SetAnim(horse_animation);
        goto done;
    case 0x2E: animation = 0x3E; break;
    case 0x11: case 0x21: case 0x31: case 0x36: case 0x37: animation = 0x146; break;
    case 0x19: case 0x1A: goto standing;
    case 0x30: case 0x32: case 0x38: case 0x39:
    hold_still:
        animation = 0x152;
        goto done;
    case 0x12: case 0x33: animation = 0x13A; break;
    case 0xA: case 0x15: case 0x2F: case 0x34: animation = 0x42; break;
    case 0x35:
        {
            register u32 empty asm("r0") = FarmerHeldItemIsEmptyRaw(&self->farmer->unk_54) << 24;
            asm volatile("" : "+r"(empty));
            animation = 0x1C6;
            asm volatile("" : "+r"(animation));
            if (empty)
                goto done;
        }
    hold_walk:
        animation = 0x156;
        goto done;
    case 8:
        {
            register u32 variant asm("r0") = self->unk_3D;
            asm volatile("" : : "r"(variant), "r"(self));
            animation = 0xF6;
            if (variant)
                animation = 0xF2;
            goto done;
        }
    default:
        break;
    }
    goto done;
done:
    asm volatile("" : : "r"(animation));
    return animation;
}

EC void SelectFarmerInteractionTarget(FarmerEntity * farmer, u32 predicate_info, u32 predicate_data) ENTITY_CODE(0802AC2C);
EC void SelectFarmerInteractionTarget(FarmerEntity * farmer, u32 predicate_info, u32 predicate_data)
{
    FarmerTargetSearchFrame frame;
    register FarmerEntity * input_farmer asm("r0") = farmer;
    register FarmerEntity * self asm("sb") = input_farmer;
    register u32 predicate_method asm("r6") = predicate_data;
    register u32 predicate asm("r5") = predicate_info;
    asm volatile("" : "+r"(predicate_method), "+r"(predicate));
    register GameObject * init_game asm("r0") = input_farmer->game_object;
    frame.game = init_game;
    register FarmerEntity const * map_source asm("r1") = self;
    register u32 map asm("r1") = map_source->location_map;
    frame.map = map;
    register FarmerEntity const * x_source asm("r2") = self;
    register u32 reserved_r0 asm("r0");
    register u32 reserved_r1 asm("r1");
    asm volatile("" : "=r"(reserved_r0), "=r"(reserved_r1));
    register i32 x asm("r2") = static_cast<i16>(x_source->x_q16 >> 16);
    asm volatile("" : : "r"(x), "r"(reserved_r0), "r"(reserved_r1));
    frame.x = x;
    register FarmerEntity const * y_source asm("r0") = self;
    register i32 y asm("r0") = static_cast<i16>(y_source->y_q16 >> 16);
    frame.y = y;
    register FarmerEntity const * face_source asm("r0") = self;
    register u32 facing asm("r2") = face_source->facing;
    ReadFarmerInteractionBox(&frame.interaction_box, self, facing);
    frame.bounds = frame.interaction_box;
    register FarmerTargetBounds const * bounds_low asm("r2") = &frame.bounds;
    register FarmerTargetBounds const * bounds_ptr asm("r8") = bounds_low;
    register u32 no_target asm("r3") = 100;
    frame.best_slot = no_target;
    register i32 zero asm("r0") = 0;
    frame.best_distance = zero;
    register u32 slot_init asm("r1") = 43;
    asm volatile("" : : "r"(slot_init));
    register u32 slot asm("sl") = slot_init;
    FarmerTargetBounds * actor_bounds = &frame.actor_bounds;
    do
    {
    scan_slot:
        register u32 current_slot asm("r2") = slot;
        asm volatile("" : : "r"(current_slot));
        frame.current_slot = current_slot;
        register GameObject * scene asm("r3") = frame.game;
        register FarmerInteractionGameVTable const * table asm("r0") = *reinterpret_cast<FarmerInteractionGameVTable const **>(scene);
        register AEntity * (*get_entity)(GameObject *, u32) asm("r2") = table->get_entity;
        AEntity * target = get_entity(scene, slot);
        register u32 invalid asm("r0") = 0;
        register FarmerTargetSearchFrame * stack0 asm("sp");
        asm volatile("" : "=r"(stack0));
        register u8 * invalid_dest asm("r1") = &stack0->accepted;
        *invalid_dest = invalid;
        if (target != nullptr)
        {
            register u32 target_map asm("r0") = target->location_map;
            register u32 stored_map asm("r2") = frame.map;
            if (target_map == stored_map)
            {
                register FarmerTargetActorVTable const * actor asm("r1") = reinterpret_cast<FarmerTargetActorVTable const **>(target)[5];
                register FarmerTargetBounds * out asm("r0") = actor_bounds;
                register void (*bounds)(FarmerTargetBounds *, AEntity const *) asm("r2") = actor->get_bounds;
                bounds(out, target);
                register FarmerTargetBounds const * overlap_box asm("r3") = bounds_ptr;
                register i32 left1 asm("r2") = overlap_box->x1;
                register i32 right2 asm("r0") = actor_bounds->x2;
                register i32 overlap asm("r2") = left1 - right2;
                register i32 left2 asm("r0") = actor_bounds->x1;
                register FarmerTargetBounds const * other_box asm("r1") = bounds_ptr;
                register i32 right1 asm("r1") = other_box->x2;
                register i32 horizontal asm("r0") = left2 - right1;
                overlap &= horizontal;
                other_box = bounds_ptr;
                register i32 top1 asm("r0") = other_box->y1;
                register i32 bottom2 asm("r1") = actor_bounds->y2;
                register i32 vertical1 asm("r0") = top1 - bottom2;
                overlap &= vertical1;
                register i32 top2 asm("r0") = actor_bounds->y1;
                overlap_box = bounds_ptr;
                register i32 bottom1 asm("r3") = overlap_box->y2;
                register i32 vertical2 asm("r0") = top2 - bottom1;
                overlap &= vertical2;
                if (overlap >= 0)
                    goto target_checked;
                {
                    i32 virtual_slot = static_cast<i32>(predicate) >> 16;
                    if (virtual_slot != 0)
                    {
                        typedef bool (*Method)(void *);
                        Method method;
                        if (virtual_slot > 0)
                        {
                            Method const * methods = *reinterpret_cast<Method const **>(reinterpret_cast<u8 *>(target) + static_cast<i16>(predicate_method));
                            method = methods[virtual_slot - 1];
                        }
                        else
                            method = reinterpret_cast<Method>(predicate_method);
                        if (!method(reinterpret_cast<u8 *>(target) + static_cast<i16>(predicate)))
                            goto target_checked;
                    }
                }
                {
                    register u32 accepted asm("r2") = 1;
                    register FarmerTargetSearchFrame * stack1 asm("sp") = &frame;
                    asm volatile("" : : "r"(stack1));
                    register u8 * valid_dest asm("r3") = &stack1->accepted;
                    *valid_dest = accepted;
                }
            }
        }
    target_checked:
        register FarmerTargetSearchFrame * stack2 asm("sp") = &frame;
        asm volatile("" : : "r"(stack2));
        register u8 * valid_source asm("r0") = &stack2->accepted;
        if (*valid_source)
        {
            register i32 target_x asm("r1") = static_cast<i16>(target->x_q16 >> 16);
            register i32 from_x asm("r3") = frame.x;
            register i32 dx asm("r1") = target_x - from_x;
            register i32 target_y asm("r0") = static_cast<i16>(target->y_q16 >> 16);
            asm volatile("" : : "r"(target_y), "r"(from_x));
            register i32 from_y asm("r3") = frame.y;
            register i32 dy asm("r0") = target_y - from_y;
            register i32 dx_squared asm("r2") = dx * dx;
            asm volatile("" : "+r"(dx_squared));
            register i32 x_squared asm("r1") = dx_squared;
            register i32 dy_squared asm("r3") = dy * dy;
            register i32 y_squared asm("r0") = dy_squared;
            asm volatile("" : : "r"(y_squared));
            register i32 distance asm("r0") = x_squared + y_squared;
            register i32 store_distance asm("r1") = distance;
            register u32 best_slot asm("r2") = frame.best_slot;
            if (best_slot != 100)
            {
                register i32 best_distance asm("r3") = frame.best_distance;
                if (best_distance <= distance)
                    goto next_target;
            }
            {
                register u32 new_slot asm("r0") = frame.current_slot;
                frame.best_slot = new_slot;
                frame.best_distance = store_distance;
            }
        }
    next_target:
        register u32 increment asm("r1") = 1;
        asm volatile("" : : "r"(increment));
        slot += increment;
        register u32 next asm("r2") = slot;
        asm volatile("" : : "r"(next), "r"(slot));
        if (next <= 69) goto scan_slot;
    } while (false);
    register u8 * target_slot asm("r0") = &self->interaction_target_slot;
    register FarmerTargetSearchFrame * final_frame asm("sp") = &frame;
    asm volatile("" : : "r"(final_frame));
    register u8 const * best_slot asm("r3") = reinterpret_cast<u8 const *>(&final_frame->best_slot);
    register u32 selected_slot asm("r3") = *best_slot;
    *target_slot = selected_slot;
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

struct FarmerTerrainGrid
{
    u32 const * properties;
    u8 const * indices;
    u16 width;
    u16 height;
};

struct FarmerTerrainGameVTable
{
    STRUCT_PAD(0x00, 0x34);
    FarmerTerrainGrid (*get_terrain)(GameObject *, u32);
};

struct FarmerTerrainActorVTable
{
    STRUCT_PAD(0x00, 0x0C);
    Box (*get_bounds)(AEntity const *);
};

EC bool DoesEntityCollisionRectangleHitSolidTile(FarmerTerrainGrid const * grid, Box const * bounds);

struct FarmerDiscardGameVTable
{
    STRUCT_PAD(0x00, 0x34);
    void (*get_grid)(FarmerTerrainGrid *, GameObject *, u32);
    STRUCT_PAD(0x38, 0xE8);
    u32 (*check_article_drop)(GameObject *, Location const &, Article const &);
};
EC bool AreEntityFootTilesMarked(FarmerTerrainGrid const &, i32, i32);

struct FarmerDiscardBounds
{
    i16 x1, y1, x2, y2;
};

struct FarmerDiscardFrame
{
    FarmerTerrainGrid terrain;
    Location position;
    u16 position_padding;
    u8 article;
    u8 article_padding[3];
    FarmerDiscardBounds dog_bounds, chicken_bounds, object_bounds;
    GameObject * game;
};

EC u32 CheckFarmerHeldItemDiscard(FarmerEntity *) ENTITY_CODE(0802A7E0);
EC u32 CheckFarmerHeldItemDiscard(FarmerEntity * self)
{
    {
        FarmerDiscardFrame frame;
        register FarmerEntity * farmer asm("r5") = self;
        asm volatile("" : "+r"(farmer));
        register HeldItem * entry_item asm("r0") = &farmer->farmer->unk_54;
        asm volatile("" : : "r"(entry_item));
        register HeldItem * held asm("r8") = entry_item;
        asm volatile("" : : "r"(held));
        if (FarmerHeldItemIsEmptyRaw(entry_item) << 24)
            return 2;
        if (func_0800F388(held))
            return 0;
        register i32 initial_x asm("r0") = i16(farmer->x_q16 >> 16);
        asm volatile("" : : "r"(initial_x));
        register i32 sample_x asm("sb") = initial_x;
        asm volatile("" : "+r"(sample_x));
        register i32 initial_y asm("r4") = i16(farmer->y_q16 >> 16);
        asm volatile("" : : "r"(initial_y));
        register i32 sample_y asm("sl") = initial_y;
        asm volatile("" : "+r"(sample_y));
        register i32 x asm("r4") = sample_x;
        register i32 y = sample_y;
        asm volatile("" : "+r"(x), "+r"(y));
        switch (farmer->facing)
        {
        case FACING_UP: sample_y = y - 20; y -= 16; break;
        case FACING_DOWN: sample_y = y + 20; y += 16; break;
        case FACING_LEFT: sample_x = x - 20; x -= 16; break;
        case FACING_RIGHT: sample_x = x + 20; x += 16; break;
        }
        register GameObject * scene asm("r1") = farmer->game_object;
        asm volatile("" : : "r"(scene));
        frame.game = scene;
        asm volatile("" : : "m"(frame.game));
        register u32 map asm("r6") = farmer->location_map;
        asm volatile("" : : "r"(map));
        register FarmerDiscardGameVTable const * table asm("r1") = *reinterpret_cast<FarmerDiscardGameVTable const **>(scene);
        asm volatile("" : : "r"(table));
        register FarmerTerrainGrid * grid_address asm("r0") = &frame.terrain;
        asm volatile("" : : "r"(grid_address));
        register void (*get_grid)(FarmerTerrainGrid *, GameObject *, u32) asm("r3") = table->get_grid;
        asm volatile("" : : "r"(get_grid));
        asm volatile("" : "+m"(frame.game));
        get_grid(grid_address, frame.game, map);
        register FarmerTerrainGrid * terrain asm("r5") = &frame.terrain;
        asm volatile("" : "+r"(terrain));
        FarmerDiscardBounds * bounds;
        switch (func_0800F204(held))
        {
        case HeldItem::KIND_ARTICLE:
            {
                register u32 article_id asm("r0") = ReadHeldArticleId(held);
                asm volatile("" : : "r"(article_id));
                register FarmerDiscardFrame * article_frame asm("sp");
                asm volatile("" : "=r"(article_frame));
                register u8 * encoded_article asm("r1") = &article_frame->article;
                *encoded_article = article_id;
                register Article * initial_article asm("r0") = reinterpret_cast<Article *>(encoded_article);
                asm volatile("" : : "r"(initial_article));
                asm volatile("" : "+r"(y));
                if (!initial_article->CanBeDiscarded())
                    return 2;
                register FarmerDiscardFrame * index_frame asm("sp");
                asm volatile("" : "=r"(index_frame));
                register u32 case_index asm("r0") = reinterpret_cast<Article *>(&index_frame->article)->GetId() - 49;
                asm volatile("" : "+r"(case_index), "+r"(y));
                switch (case_index)
                {
                case 0:
                    if (AreEntityFootTilesMarked(*terrain, sample_x, sample_y))
                    {
                        asm volatile("");
                        goto allowed;
                    }
                    goto inspect_tile;
                case 4:
                    return 3;
                case 1: case 40: case 41:
                inspect_tile:
                    {
                        register FarmerDiscardGameVTable const * current asm("r0") = *reinterpret_cast<FarmerDiscardGameVTable const **>(frame.game);
                        asm volatile("" : : "r"(current));
                        typedef u32 (*CheckDrop)(GameObject *, Location const &, Article const &);
                        register CheckDrop const * check asm("sb") = reinterpret_cast<CheckDrop const *>(current);
                        asm volatile("" : : "r"(check));
                        register u32 action_offset asm("r1") = 0xE8;
                        asm volatile("" : : "r"(action_offset));
                        check = reinterpret_cast<CheckDrop const *>(reinterpret_cast<u8 const *>(check) + action_offset);
                        Location * position = &frame.position;
                        register u32 coordinate_mask asm("r8") = 0x3FF;
                        asm volatile("" : : "r"(coordinate_mask));
                        register u32 map_mask asm("r0") = coordinate_mask;
                        asm volatile("" : : "r"(map_mask));
                        map &= map_mask;
                        asm volatile("" : "+r"(map));
                        register u32 old_storage asm("r2") = *reinterpret_cast<u16 *>(position);
                        asm volatile("" : : "r"(old_storage));
                        register u32 high_mask asm("r5") = ~0x3FFu;
                        asm volatile("" : : "r"(high_mask));
                        register u32 stored_map asm("r0") = high_mask;
                        asm volatile("" : "+r"(stored_map));
                        stored_map &= old_storage;
                        *reinterpret_cast<u16 *>(position) = stored_map | map;
                        u8 * bytes = reinterpret_cast<u8 *>(position);
                        u16 * halves = reinterpret_cast<u16 *>(position);
                        register u32 x_bits asm("r4") = static_cast<u32>(x) << 16;
                        asm volatile("" : : "r"(x_bits));
                        register u32 x_lo asm("r2") = x_bits >> 16;
                        asm volatile("" : "+r"(x_lo));
                        register u32 lo_mask asm("r0") = 63;
                        asm volatile("" : : "r"(lo_mask));
                        register u32 saved_lo_mask asm("sl") = lo_mask;
                        asm volatile("" : : "r"(saved_lo_mask));
                        x_lo &= lo_mask;
                        x_lo <<= 2;
                        asm volatile("" : : "r"(x_lo));
                        register u32 old_xlo asm("r6") = bytes[1];
                        asm volatile("" : : "r"(old_xlo));
                        register u32 tail_mask asm("r3") = 3;
                        asm volatile("" : : "r"(tail_mask));
                        register u32 new_xlo asm("r0") = tail_mask;
                        asm volatile("" : "+r"(new_xlo));
                        new_xlo &= old_xlo;
                        bytes[1] = new_xlo | x_lo;
                        register u32 x_hi asm("r4") = x_bits >> 22;
                        asm volatile("" : "+r"(x_hi));
                        register u32 ten_bits asm("r0") = coordinate_mask;
                        asm volatile("" : : "r"(ten_bits));
                        x_hi &= ten_bits;
                        register u32 old_xhi asm("r2") = halves[1];
                        asm volatile("" : : "r"(old_xhi), "r"(x_hi));
                        register u32 new_xhi asm("r0") = high_mask;
                        asm volatile("" : "+r"(new_xhi));
                        new_xhi &= old_xhi;
                        halves[1] = new_xhi | x_hi;
                        register u32 y_bits asm("r2") = static_cast<u32>(y) << 16;
                        asm volatile("" : : "r"(y_bits));
                        register u32 y_lo asm("r0") = y_bits >> 16;
                        asm volatile("" : "+r"(y_lo));
                        register u32 y_lo_mask asm("r4") = saved_lo_mask;
                        asm volatile("" : : "r"(y_lo_mask));
                        y_lo &= y_lo_mask;
                        y_lo <<= 2;
                        asm volatile("" : : "r"(y_lo));
                        register u32 old_ylo asm("r4") = bytes[3];
                        asm volatile("" : : "r"(old_ylo), "r"(tail_mask));
                        tail_mask &= old_ylo;
                        tail_mask |= y_lo;
                        bytes[3] = tail_mask;
                        register u32 y_hi asm("r2") = y_bits >> 22;
                        asm volatile("" : "+r"(y_hi));
                        register u32 y_hi_mask asm("r0") = coordinate_mask;
                        asm volatile("" : : "r"(y_hi_mask));
                        y_hi &= y_hi_mask;
                        register u32 old_yhi asm("r0") = halves[2];
                        asm volatile("" : : "r"(old_yhi), "r"(y_hi), "r"(high_mask));
                        high_mask &= old_yhi;
                        high_mask |= y_hi;
                        halves[2] = high_mask;
                        register CheckDrop const * callee_slot asm("r4") = check;
                        asm volatile("" : : "r"(callee_slot));
                        register CheckDrop verify_drop asm("r3") = *callee_slot;
                        asm volatile("" : "+r"(verify_drop));
                        register GameObject * drop_game asm("r0") = frame.game;
                        asm volatile("" : : "r"(drop_game), "r"(verify_drop));
                        register FarmerDiscardFrame * drop_frame asm("sp");
                        asm volatile("" : "=r"(drop_frame));
                        register Article * drop_article asm("r2") = reinterpret_cast<Article *>(&drop_frame->article);
                        asm volatile("" : : "r"(drop_article));
                        i32 outcome = verify_drop(drop_game, *position, *drop_article);
                        if (outcome == 1)
                            goto blocked;
                        if (outcome > 1)
                            goto check_special_result;
                        if (outcome == 0)
                            goto accepted;
                        goto blocked;
                    check_special_result:
                        if (outcome == 2)
                            goto check_discarded_article;
                        goto blocked;
                    accepted:
                        return 1;
                    check_discarded_article:
                        if (reinterpret_cast<Article *>(&frame.article)->GetId() == 49)
                            goto blocked;
                        goto allowed;
                    }
                case 2:
                case 3:
                case 5:
                case 6:
                case 7:
                case 8:
                case 9:
                case 10:
                case 11:
                case 12:
                case 13:
                case 14:
                case 15:
                case 16:
                case 17:
                case 18:
                case 19:
                case 20:
                case 21:
                case 22:
                case 23:
                case 24:
                case 25:
                case 26:
                case 27:
                case 28:
                case 29:
                case 30:
                case 31:
                case 32:
                case 33:
                case 34:
                case 35:
                case 36:
                case 37:
                case 38:
                case 39:
                default:
                    goto allowed;
                }
            }
        case HeldItem::KIND_FOOD:
        allowed:
            return 0;
        case HeldItem::KIND_2: bounds = &frame.dog_bounds; goto inspect_bounds;
        case HeldItem::KIND_3: bounds = &frame.chicken_bounds; goto inspect_bounds;
        case HeldItem::KIND_4: bounds = &frame.object_bounds; goto inspect_bounds;
        case HeldItem::KIND_5: return 2;
        default: return 2;
        }
    inspect_bounds:
        bounds->x1 = x - 7;
        bounds->y1 = y - 4;
        register i32 max_x asm("r0") = x + 7;
        asm volatile("" : : "r"(max_x));
        bounds->x2 = max_x;
        bounds->y2 = y + 5;
        bool solid = DoesEntityCollisionRectangleHitSolidTile(terrain, reinterpret_cast<Box *>(bounds));
        i32 result = 1;
        if (solid)
            result = 2;
        return result;
    }
blocked:
    return 2;
}

EC bool IsFarmerOnSpecialTerrain(FarmerEntity * farmer) ENTITY_CODE(0802AEA4);
EC bool IsFarmerOnSpecialTerrain(FarmerEntity * farmer)
{
    GameObject * game = farmer->game_object;
    register u32 map asm("r6") = farmer->location_map;
    FarmerTerrainGameVTable const * table = *reinterpret_cast<FarmerTerrainGameVTable const **>(game);
    FarmerTerrainGrid terrain = table->get_terrain(game, map);
    FarmerTerrainGrid * view = &terrain;
    FarmerTerrainActorVTable const * actor = reinterpret_cast<FarmerTerrainActorVTable const **>(farmer)[5];
    Box original = actor->get_bounds(farmer);
    Box bounds = original;
    Box const * box = &bounds;

    {
        if (DoesEntityCollisionRectangleHitSolidTile(&terrain, box))
            goto found;
        register u32 range asm("r1") = map - 0x34;
        asm volatile("" : : "r"(range));
        if (range > 0x1ff)
            goto missing;

        u32 columns;
        register u32 const * tile asm("r0");
        register u32 const * properties asm("r2");
        {
            i32 contact_x = box->x1;
            asm volatile("" : : "r"(contact_x));
            register u32 x asm("r6") = contact_x / 8;
            register u32 y asm("r0") = box->y1 / 8;
            properties = view->properties;
            columns = view->width;
            if (properties != nullptr && x <= columns && y <= view->height)
            {
                u8 const * indirect = view->indices;
                u32 index = y * columns + x;
                if (indirect)
                    index = indirect[index];
                tile = properties + index;
            }
            else
                tile = nullptr;
        }
        if (tile != nullptr && (*tile & 0x1fffc))
            goto found;
        {
            i32 contact_x = box->x1;
            asm volatile("" : : "r"(contact_x));
            register u32 x asm("r6") = contact_x / 8;
            register u32 y asm("r0") = (box->y2 - 1) / 8;
            properties = view->properties;
            if (properties != nullptr && x <= columns && y <= view->height)
            {
                u8 const * indirect = view->indices;
                u32 index = y * columns + x;
                if (indirect)
                    index = indirect[index];
                tile = properties + index;
            }
            else
                tile = nullptr;
        }
        if (tile != nullptr && (*tile & 0x1fffc))
            goto found;
        {
            i32 contact_x = (box->x2 - 1);
            asm volatile("" : : "r"(contact_x));
            register u32 x asm("r6") = contact_x / 8;
            register u32 y asm("r0") = box->y1 / 8;
            asm volatile("" : : "r"(properties));
            if (properties != nullptr && x <= columns && y <= view->height)
            {
                u8 const * indirect = view->indices;
                u32 index = y * columns + x;
                if (indirect)
                    index = indirect[index];
                tile = properties + index;
            }
            else
                tile = nullptr;
        }
        if (tile != nullptr && (*tile & 0x1fffc))
            goto found;
        {
            i32 contact_x = (box->x2 - 1);
            asm volatile("" : : "r"(contact_x));
            register u32 x asm("r6") = contact_x / 8;
            register u32 y asm("r0") = (box->y2 - 1) / 8;
            register u32 const * last_properties asm("r4") = properties;
            if (last_properties != nullptr && x <= columns && y <= view->height)
            {
                u8 const * indirect = view->indices;
                u32 index = y * columns + x;
                if (indirect)
                    tile = last_properties + indirect[index];
                else
                    tile = properties + index;
            }
            else
                tile = nullptr;
        }
        if (tile != nullptr && (*tile & 0x1fffc))
            goto found;
        goto missing;
    }
found:
    return true;
missing:
    return false;
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

EC void InitializePrioritizedSideActorSprite(UnknownEntityThing * graphics, AEntity * entity, u32 sprite_mode, void const * config,
    u32 priority, u32 layer, u32 x, u32 y, bool enabled);
EC void InitializeAnimatedMapSpriteSinglePalette(void * sprite, void * sprites, u32 animation, GameObject * game,
    u32 priority, u32 layer, bool mirrored);
EC char const gUnk_080F1224[];
EC char const vtable_unk_080E6644[];

EC ItemDisplayGraphics * InitItemDisplayGraphics(ItemDisplayGraphics * graphics, ItemDisplayEntity * display) ENTITY_CODE(0802BF2C);
EC ItemDisplayGraphics * InitItemDisplayGraphics(ItemDisplayGraphics * graphics, ItemDisplayEntity * display)
{
    InitializePrioritizedSideActorSprite(graphics, display, 6, gUnk_080F1224, 3, 1, 0, 0, true);
    reinterpret_cast<void const **>(graphics)[1] = vtable_unk_080E6644;
    GameObject * game = display->game_object;
    FarmerIconGameVTable const * table = *reinterpret_cast<FarmerIconGameVTable const **>(game);
    void * sprites = table->get_item_sprites(game);
    GameObject * sprite_game = display->game_object;
    InitializeAnimatedMapSpriteSinglePalette(reinterpret_cast<u8 *>(graphics) + 0x8C, sprites, 0, sprite_game, 2, 1, false);
    graphics->icon = 0xFFFF;
    return graphics;
}

EC ItemDisplayGraphics * NewItemDisplayGraphics(ItemDisplayEntity * display) ENTITY_CODE(0802BA10);
EC ItemDisplayGraphics * NewItemDisplayGraphics(ItemDisplayEntity * display)
{
    return InitItemDisplayGraphics(static_cast<ItemDisplayGraphics *>(::operator new(sizeof(ItemDisplayGraphics))), display);
}

EC void AdvanceItemDisplayState(ItemDisplayEntity * display) ENTITY_CODE(0802BA74);
EC void AdvanceSideEntitySlide(AActorEntity * display, u32 argument);
EC u32 GetItemDisplayStateAnimation(ItemDisplayEntity const * display) ENTITY_CODE(0802BC48);

EC void TickItemDisplayEntity(ItemDisplayEntity * display, u32 argument) ENTITY_CODE(0802BA28);
EC void TickItemDisplayEntity(ItemDisplayEntity * display, u32 argument)
{
    if (!display->restart)
    {
        if (display->active)
        {
            AdvanceItemDisplayState(display);
            u32 animation = static_cast<u16>(GetItemDisplayStateAnimation(display));
            if (display->anim_id != animation)
                display->SetAnim(animation);
        }
    }
    else
        display->restart = false;
    AdvanceSideEntitySlide(display, argument);
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

struct ItemDisplayDropGameVTable
{
    STRUCT_PAD(0x00, 0xBC);
    void (*drop_item)(GameObject *, Location const &, u32);
};

inline bool HasItemDisplayAnimationEnded(ItemDisplayEntity const * display)
{
    bool ended = false;
    if (display->unk_24 == 0)
        ended = true;
    return ended;
}

EC void AdvanceItemDisplayState(ItemDisplayEntity * display)
{
    switch (display->state)
    {
    case 0:
        display->active = false;
        break;
    case 1:
    {
        register ThrownItemMotion * motion asm("r1") = &display->thrown_item;
        motion->x_speed += motion->x_velocity;
        motion->y_speed += motion->y_velocity;
        register i32 falling_speed asm("r0") = motion->z_velocity - 19114;
        motion->z_velocity = falling_speed;
        register i32 height asm("r2") = motion->z_speed + falling_speed;
        register ThrownItemMotion * retained asm("sl") = motion;
        asm volatile("" : : "r"(retained));
        if (height > 0)
            motion->z_speed = height;
        else
        {
            i32 zero = 0;
            register ThrownItemMotion * stopped asm("r3") = retained;
            stopped->z_speed = zero;
            stopped->z_velocity = zero;
        }
        if (!HasItemDisplayAnimationEnded(display))
            break;
        display->state = 0;
        i32 x = display->x_q16 >> 16;
        i32 y = display->y_q16 >> 16;
        switch (static_cast<i32>(display->facing))
        {
        case FACING_DOWN: y += 20; break;
        case FACING_UP: y -= 20; break;
        case FACING_LEFT: x -= 20; break;
        case FACING_RIGHT: x += 20; break;
        }
        Location where(display->location_map, x, y);
        GameObject * game = display->game_object;
        ItemDisplayDropGameVTable const * table = *reinterpret_cast<ItemDisplayDropGameVTable const **>(game);
        table->drop_item(game, where, display->display_icon);
        register i32 inactive asm("r1") = 0;
        register ThrownItemMotion * idle asm("r2") = retained;
        asm volatile("" : "+r"(idle));
        *reinterpret_cast<u8 *>(&idle->active) = inactive;
        *reinterpret_cast<u8 *>(&display->alternate_idle) = inactive;
        break;
    }
    case 2:
        if (HasItemDisplayAnimationEnded(display))
        {
            display->state = 0;
            display->thrown_item.active = false;
            display->alternate_idle = false;
        }
        break;
    case 3:
        if (HasItemDisplayAnimationEnded(display))
        {
            u8 * motion_bytes = reinterpret_cast<u8 *>(display) + 0x38;
            ThrownItemMotion * motion = reinterpret_cast<ThrownItemMotion *>(motion_bytes);
            register i32 zero asm("r1") = 0;
            register i32 carried_height asm("r2") = 21 << 16;
            motion->x_speed = zero;
            motion->y_speed = zero;
            motion->z_speed = carried_height;
            *(motion_bytes - 4) = zero;
            display->alternate_idle = true;
        }
        break;
    case 4:
    {
        u32 timer = display->transition_timer - 1;
        display->transition_timer = timer;
        if (static_cast<u8>(timer) == 0)
        {
            display->state = 0;
            display->thrown_item.active = false;
        }
        break;
    }
    }
}

struct FarmerEventRequest
{
    u32 id;
    u32 argument;
};

typedef void (*FarmerEventDispatch)(GameObject *, FarmerEventRequest const *, u32);

struct FarmerEventGameVTable
{
    STRUCT_PAD(0, 0x140);
    FarmerEventDispatch dispatch_event;
};

EC u32 CountHarvestGoddessJewels(void const *);
EC u32 CountKappaJewels(void const *);
EC bool HaveAllCursedToolsStarted(void const *);
EC bool func_0809E174(void const *);
EC bool func_0809E184(void const *);
EC bool func_0809E194(void const *);
EC bool func_0809E1A4(void const *);
EC i32 const gHarvestGoddessCollectionProducts[] asm("gUnk_080F11BC");
EC i32 const gKappaCollectionProducts[] asm("gUnk_080F11E0");
struct FarmerCollectionEventLocals
{
    u8 goddess_jewel;
    FarmerEventRequest goddess_jewel_event;
    u8 kappa_jewel;
    FarmerEventRequest kappa_jewel_event;
    FarmerEventRequest goddess_collection_event;
    FarmerEventRequest kappa_collection_event;
};

EC void CheckFarmerCollectionEvents(FarmerEntity *) ENTITY_CODE(0802B054);
EC void CheckFarmerCollectionEvents(FarmerEntity * self)
{
    FarmerCollectionEventLocals locals;
    register FarmerEntity * farmer asm("r5") = self;
    asm volatile("" : : "r"(farmer));
    {
        register u32 jewels asm("r0") = CountHarvestGoddessJewels(reinterpret_cast<u8 const *>(farmer->game_state) + 0x2E58) << 24;
        asm volatile("" : "+r"(jewels));
        if ((jewels >> 24) != 9)
            goto check_kappa_jewels;
    }
    {
        register u32 flags asm("r0") = reinterpret_cast<u8 const *>(farmer->game_state)[0x2189];
        asm volatile("" : : "r"(flags));
        register u32 exchanged asm("r4") = 0x30;
        exchanged &= flags;
        asm volatile("" : : "r"(exchanged));
        if (exchanged != 0)
            goto check_kappa_jewels;
        register u32 held_kind asm("r0") = func_0800F204(&farmer->farmer->unk_54);
        asm volatile("" : : "r"(held_kind));
        if (held_kind != HeldItem::KIND_ARTICLE)
            goto check_kappa_jewels;
        register u32 article_id asm("r0") = ReadHeldArticleId(&farmer->farmer->unk_54);
        asm volatile("" : : "r"(article_id));
        register u8 * value asm("r1") = &locals.goddess_jewel;
        *value = article_id;
        Article const * article = reinterpret_cast<Article const *>(&locals.goddess_jewel);
        if (article->GetId() != ARTICLE_HARVEST_GODDESS_JEWEL)
            goto check_kappa_jewels;
        GameObject * game = farmer->game_object;
        FarmerEventGameVTable const * table = *reinterpret_cast<FarmerEventGameVTable const **>(game);
        FarmerEventDispatch const * handler = &table->dispatch_event;
        FarmerEventRequest * args = &locals.goddess_jewel_event;
        args->id = 0x330;
        args->argument = exchanged;
        FarmerEventDispatch dispatch = *handler;
        dispatch(game, args, 0);
    }
check_kappa_jewels:
    {
        register u32 jewels asm("r0") = CountKappaJewels(reinterpret_cast<u8 const *>(farmer->game_state) + 0x2E58) << 24;
        asm volatile("" : "+r"(jewels));
        if ((jewels >> 24) != 9)
            goto check_goddess_shipping;
    }
    {
        register u32 flags asm("r0") = reinterpret_cast<u8 const *>(farmer->game_state)[0x218b];
        asm volatile("" : : "r"(flags));
        register u32 exchanged asm("r4") = 0x18;
        exchanged &= flags;
        asm volatile("" : : "r"(exchanged));
        if (exchanged != 0)
            goto check_goddess_shipping;
        register u32 held_kind asm("r0") = func_0800F204(&farmer->farmer->unk_54);
        asm volatile("" : : "r"(held_kind));
        if (held_kind != HeldItem::KIND_ARTICLE)
            goto check_goddess_shipping;
        register u32 article_id asm("r0") = ReadHeldArticleId(&farmer->farmer->unk_54);
        asm volatile("" : : "r"(article_id));
        register u32 encoded asm("r1") = article_id;
        asm volatile("" : : "r"(encoded));
        register u8 * value asm("r0") = &locals.kappa_jewel;
        *value = encoded;
        Article const * article = reinterpret_cast<Article const *>(&locals.kappa_jewel);
        if (article->GetId() != ARTICLE_KAPPA_JEWEL)
            goto check_goddess_shipping;
        GameObject * game = farmer->game_object;
        FarmerEventGameVTable const * table = *reinterpret_cast<FarmerEventGameVTable const **>(game);
        FarmerEventDispatch const * handler = &table->dispatch_event;
        FarmerEventRequest * args = &locals.kappa_jewel_event;
        args->id = 0x332;
        args->argument = exchanged;
        FarmerEventDispatch dispatch = *handler;
        dispatch(game, args, 0);
    }
check_goddess_shipping:
    if (reinterpret_cast<u8 const *>(farmer->game_state)[0x2194] & 0xc0)
        goto check_kappa_shipping;
    {
        u32 have_shipped = 1;
        u32 index = 0;
        register i32 const * item asm("r4") = gHarvestGoddessCollectionProducts;
        asm volatile("" : "+r"(have_shipped), "+r"(index), "+r"(item));
        for (; index < 9; ++item, ++index)
        {
            if (!reinterpret_cast<ShippingBin const *>(reinterpret_cast<u8 const *>(farmer->game_state) + 0x54)->IsDisplayEnabled(*item))
            {
                have_shipped = 0;
                break;
            }
        }
        asm volatile("" : "+r"(have_shipped));
        asm volatile("" : : "r"(index));
        if (!have_shipped)
            goto check_kappa_shipping;
        register u8 const * state asm("r0") = reinterpret_cast<u8 const *>(farmer->game_state);
        asm volatile("" : : "r"(state));
        register u32 mine_record_offset asm("r4") = 0x2E58;
        asm volatile("" : : "r"(mine_record_offset));
        if (!func_0809E184(state + mine_record_offset))
            goto check_kappa_shipping;
        if (!func_0809E194(reinterpret_cast<u8 const *>(farmer->game_state) + mine_record_offset))
            goto check_kappa_shipping;
        if (!u8(CountHarvestGoddessJewels(reinterpret_cast<u8 const *>(farmer->game_state) + mine_record_offset)))
            goto check_kappa_shipping;
        if (!func_0809E1A4(reinterpret_cast<u8 const *>(farmer->game_state) + mine_record_offset))
            goto check_kappa_shipping;
        GameObject * game = farmer->game_object;
        register FarmerEventGameVTable const * table asm("r2") = *reinterpret_cast<FarmerEventGameVTable const **>(game);
        asm volatile("" : : "r"(table));
        register u32 dispatch_slot asm("r1") = 0x140;
        asm volatile("" : : "r"(dispatch_slot));
        register FarmerEventDispatch const * handler asm("r2") = reinterpret_cast<FarmerEventDispatch const *>(reinterpret_cast<u8 const *>(table) + dispatch_slot);
        asm volatile("" : "+r"(handler));
        FarmerEventRequest * args = &locals.goddess_collection_event;
        register u32 event_id asm("r3") = 0x34d;
        register u32 argument asm("r4") = 0;
        asm volatile("" : : "r"(event_id), "r"(argument));
        args->id = event_id;
        args->argument = argument;
        FarmerEventDispatch dispatch = *handler;
        dispatch(game, args, 0);
    }
check_kappa_shipping:
    if (reinterpret_cast<u8 const *>(farmer->game_state)[0x2195] & 0x3)
        goto done;
    {
        u32 have_shipped = 1;
        u32 index = 0;
        register i32 const * item asm("r4") = gKappaCollectionProducts;
        asm volatile("" : "+r"(have_shipped), "+r"(index), "+r"(item));
        for (; index < 14; ++item, ++index)
        {
            if (!reinterpret_cast<ShippingBin const *>(reinterpret_cast<u8 const *>(farmer->game_state) + 0x54)->IsDisplayEnabled(*item))
            {
                have_shipped = 0;
                break;
            }
        }
        asm volatile("" : "+r"(have_shipped));
        asm volatile("" : : "r"(index));
        if (!have_shipped)
            goto done;
        if (!HaveAllCursedToolsStarted(reinterpret_cast<u8 const *>(farmer->game_state) + 0x3480))
            goto done;
        if (!func_0809E174(reinterpret_cast<u8 const *>(farmer->game_state) + 0x2e58))
            goto done;
        if (!u8(CountKappaJewels(reinterpret_cast<u8 const *>(farmer->game_state) + 0x2e58)))
            goto done;
        GameObject * game = farmer->game_object;
        register FarmerEventGameVTable const * table asm("r2") = *reinterpret_cast<FarmerEventGameVTable const **>(game);
        asm volatile("" : : "r"(table));
        register u32 dispatch_slot asm("r1") = 0x140;
        asm volatile("" : : "r"(dispatch_slot));
        register FarmerEventDispatch const * handler asm("r2") = reinterpret_cast<FarmerEventDispatch const *>(reinterpret_cast<u8 const *>(table) + dispatch_slot);
        asm volatile("" : "+r"(handler));
        FarmerEventRequest * args = &locals.kappa_collection_event;
        register u32 event_id asm("r3") = 0x34e;
        register u32 argument asm("r4") = 0;
        asm volatile("" : : "r"(event_id), "r"(argument));
        args->id = event_id;
        args->argument = argument;
        FarmerEventDispatch dispatch = *handler;
        dispatch(game, args, 0);
    }
done:
    return;
}

#define SEND_PEDOMETER_MILESTONE(id_value) \
    { \
        GameObject * game = farmer->game_object; \
        FarmerEventDispatch const * method = &(*reinterpret_cast<FarmerEventGameVTable const **>(game))->dispatch_event; \
        FarmerEventRequest event; \
        FarmerEventRequest * args = &event; \
        u32 event_id = id_value; \
        u32 argument = 0; \
        args->id = event_id; \
        args->argument = argument; \
        FarmerEventDispatch dispatch = *method; \
        dispatch(game, args, 1); \
        return; \
    }

EC void TickFarmerPedometer(FarmerEntity * farmer) ENTITY_CODE(0802AD80);
EC void TickFarmerPedometer(FarmerEntity * farmer)
{
    u32 previous_step_count = func_0800E958(*farmer->farmer);
    func_0800ED8C(*farmer->farmer);
    if (previous_step_count == func_0800E958(*farmer->farmer))
        return;

    switch (func_0800E958(*farmer->farmer))
    {
    case 10000: SEND_PEDOMETER_MILESTONE(0x336)
    case 100000: SEND_PEDOMETER_MILESTONE(0x337)
    case 1000000: SEND_PEDOMETER_MILESTONE(0x338)
    case 10000000: SEND_PEDOMETER_MILESTONE(0x339)
    case 100000000: SEND_PEDOMETER_MILESTONE(0x33A)
    case 1000000000: SEND_PEDOMETER_MILESTONE(0x33B)
    default: return;
    }
}

#undef SEND_PEDOMETER_MILESTONE

struct FarmerStepPosition
{
    i16 x;
    i16 y;
};

union PackedFarmerStepPosition
{
    FarmerStepPosition coordinates;
    u32 packed;
};

struct FarmerStepCoordinateQ16
{
    i16 fraction;
    i16 pixel;
};

struct FarmerStepSprite
{
    STRUCT_PAD(0x00, 0x24);
    void const * vtable;
};

struct FarmerStepSpriteAnimatorStorage
{
    u32 words[sizeof(SpriteAnimator) / sizeof(u32)];
};

struct FarmerStepSpriteSlot
{
    u8 active;
    STRUCT_PAD(0x01, 0x04);
    FarmerStepSprite sprite;
    FarmerStepSpriteAnimatorStorage animator;
    FarmerIconState icon_state;
    FarmerStepPosition position;
};

struct FarmerEffectSlots
{
    u32 count;
    FarmerStepSpriteSlot slots[6];
};

struct FarmerEffectGraphicsInit : FarmerIconGraphics
{
    FarmerEffectSlots effects;
    u8 next_step_effect;
    bool refresh_icon;
};

struct FarmerStepSpriteCallFrame
{
    u32 priority;
    u32 layer;
    bool mirrored;
    STRUCT_PAD(0x09, 0x14);
    FarmerStepSpriteSlot slot;
};

EC char const gUnk_080F1218[];
EC char const vtable_unk_080E6708[];
EC char const vtable_unk_080E681C[];
EC void CopyFarmerStepSprite(FarmerStepSprite *, FarmerStepSprite const *) asm("func_080DC840");
EC void DestroyFarmerStepSprite(FarmerStepSprite *, u32) asm("DestroyMapSpriteResource");
EC void InitFarmerStepSpriteWithStackArgs(FarmerStepSprite *, void *, u32, GameObject *) asm("InitializeAnimatedMapSpriteSinglePalette");

EC UnknownEntityThing * InitFarmerEffectGraphics(FarmerEffectGraphicsInit * graphics, FarmerEntity * farmer) ENTITY_CODE(0802B260);
EC UnknownEntityThing * InitFarmerEffectGraphics(FarmerEffectGraphicsInit * graphics, FarmerEntity * farmer)
{
    GameObject * sprite_game[2];
    FarmerStepSpriteSlot * copied_effect;
    void * sprite_bank[2];
    FarmerEffectSlots * effects;
    FarmerStepSpriteSlot * destination;
    FarmerStepSpriteSlot effect;
    u32 effect_index;
    FarmerStepSprite * effect_sprite;
    FarmerStepPosition * position;
    register FarmerStepSpriteCallFrame * call_frame asm("sp");
    bool * mirrored_argument;

    InitializePrioritizedSideActorSprite(graphics, farmer, 6, gUnk_080F1218, 3, 1, 0, 1, true);
    reinterpret_cast<void const **>(graphics)[1] = vtable_unk_080E6708;
    sprite_game[0] = farmer->game_object;
    sprite_bank[1] = (*reinterpret_cast<FarmerIconGameVTable const **>(sprite_game[0]))->get_item_sprites(sprite_game[0]);
    sprite_game[0] = farmer->game_object;
    InitializeAnimatedMapSpriteSinglePalette(reinterpret_cast<u8 *>(graphics) + 0x8C, sprite_bank[1], 0, sprite_game[0], 2, 1, false);
    register FarmerEffectSlots * effect_slots asm("r1") = &graphics->effects;
    effect_slots->count = 0;
    graphics->refresh_icon = false;
    effect_index = 0;
    mirrored_argument = &call_frame->mirrored;
    effect_sprite = &effect.sprite;
    position = &effect.position;
    register FarmerStepSpriteSlot * template_effect asm("sl") = &effect;
    register u32 inactive asm("r7") = 0;
    asm volatile("" : : "r"(template_effect), "r"(inactive) : "r6");
    effects = effect_slots;
    do
    {
        u32 count;
        sprite_game[0] = farmer->game_object;
        sprite_bank[0] = (*reinterpret_cast<FarmerIconGameVTable const **>(sprite_game[0]))->get_actor_sprites(sprite_game[0]);
        sprite_bank[1] = sprite_bank[0];
        sprite_game[0] = farmer->game_object;
        sprite_game[1] = sprite_game[0];
        register FarmerStepSpriteSlot * current_effect asm("r4") = template_effect;
        current_effect->active = 0;

        // The stack arguments are shared with the icon initialization above.
        call_frame->priority = 0;
        call_frame->layer = 18;
        *mirrored_argument = false;
        register FarmerStepSpriteCallFrame * effect_frame asm("sp");
        asm volatile("" : "=r"(effect_frame) : "0"(call_frame));
        InitFarmerStepSpriteWithStackArgs(&effect_frame->slot.sprite, sprite_bank[1], 0, sprite_game[1]);

        position->y = (position->x = 0);
        count = effects->count;
        if (count < 6)
        {
            FarmerStepSpriteSlot * slot = &effects->slots[count];
            destination = slot;
            copied_effect = destination;
            if (copied_effect)
            {
                FarmerStepSprite * sprite;
                destination->active = current_effect->active;
                sprite = &destination->sprite;
                CopyFarmerStepSprite(sprite, &effect.sprite);
                sprite->vtable = vtable_unk_080E681C;
                destination->animator = effect.animator;
                destination->icon_state.enabled = effect.icon_state.enabled;
                copied_effect->icon_state.reserved = effect.icon_state.reserved;
                copied_effect->icon_state.frame_state = effect.icon_state.frame_state;
                destination->icon_state.restart = effect.icon_state.restart;
                destination->position = effect.position;
            }
            ++effects->count;
        }
        DestroyFarmerStepSprite(effect_sprite, 2);
        ++effect_index;
    } while (effect_index < 6);
    graphics->next_step_effect = 0;
    return graphics;
}

EC void SpawnFarmerStepEffect(FarmerEffectGraphics * graphics) ENTITY_CODE(0802B46C);
EC void SpawnFarmerStepEffect(FarmerEffectGraphics * graphics)
{
    FarmerEntity * farmer = *reinterpret_cast<FarmerEntity **>(graphics);
    u32 index = graphics->next_step_effect;
    u8 * slots_base = reinterpret_cast<u8 *>(graphics) + 0xCC;
    u32 stride = (index * sizeof(FarmerStepEffectSlot)) + 4;
    register i32 x asm("r2") = reinterpret_cast<FarmerStepCoordinateQ16 *>(&farmer->x_q16)->pixel;
    register i32 y asm("r3") = reinterpret_cast<FarmerStepCoordinateQ16 *>(&farmer->y_q16)->pixel;
    asm volatile("" : : "r"(x), "r"(y));
    i32 facing = farmer->facing;
    register u32 parity asm("r5") = index & 1;
    FarmerStepEffectSlot * effect = reinterpret_cast<FarmerStepEffectSlot *>(slots_base + stride);
    effect->active = 1;
    PackedFarmerStepPosition position;
    volatile i16 * x_ptr = &position.coordinates.x;
    volatile i16 * y_ptr = &position.coordinates.y;
    *x_ptr = x;
    *y_ptr = y;
    effect->packed_position = position.packed;
    u32 animation;
    register u32 directional_base asm("r3");
    if (facing == FACING_UP)
        goto north;
    if (facing <= FACING_UP)
        goto south;
    if (facing == FACING_LEFT)
        goto west;
    if (facing == FACING_RIGHT)
        goto east;
south:
    animation = parity + 0x650;
    goto apply_animation;
north:
    directional_base = 0x652;
    goto select_directional_animation;
west:
    animation = parity + 0x654;
    goto apply_animation;
east:
    directional_base = 0x656;
select_directional_animation:
    animation = parity + directional_base;
apply_animation:
    SetSpriteAnimatorClip(&effect->animator, animation);
    u8 * flags = reinterpret_cast<u8 *>(&effect->icon_state);
    register u32 zero asm("r2") = 0;
    register u32 enabled asm("r1") = 1;
    *flags = enabled;
    flags += 2;
    *flags = zero;
    ++flags;
    *flags = enabled;
    u32 advanced = index + 1;
    u32 next = 0;
    if (advanced <= 5)
        next = advanced;
    graphics->next_step_effect = next;
}

EC void BeginItemDisplayThrow(ItemDisplayEntity * display) ENTITY_CODE(0802BE68);
EC void BeginItemDisplayThrow(ItemDisplayEntity * display)
{
    const i32 throw_speed = 109226;
    register i32 facing asm("r2") = display->facing;
    ThrownItemMotion * motion;
    register i32 velocity asm("r1");
    switch (facing)
    {
    case FACING_LEFT:
        motion = &display->thrown_item;
        velocity = -throw_speed;
        facing = 0;
        motion->x_velocity = velocity;
        asm volatile("" : : : "memory");
        motion->y_velocity = facing;
        goto reset_vertical_velocity;
    case FACING_RIGHT:
        motion = &display->thrown_item;
        velocity = throw_speed;
        facing = 0;
        motion->x_velocity = velocity;
        motion->y_velocity = facing;
        goto reset_vertical_velocity;
    case FACING_UP:
        motion = &display->thrown_item;
        facing = 0;
        velocity = -throw_speed;
        goto set_y_velocity;
    case FACING_DOWN:
        motion = &display->thrown_item;
        velocity = throw_speed;
        goto set_y_velocity;
    default:
        goto start_animation;
    }
set_y_velocity:
    motion->x_velocity = facing;
    motion->y_velocity = velocity;
reset_vertical_velocity:
    motion->z_velocity = facing;
start_animation:
    u8 * state = reinterpret_cast<u8 *>(display) + 0x34;
    int throwing = 1;
    *state = throwing;
    bool * restart = reinterpret_cast<bool *>(state + 0x24);
    *restart = throwing;
    display->SetAnim(static_cast<u16>(GetItemDisplayStateAnimation(display)));
    display->active = throwing;
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
    UpdateSideActorAnimationLayers(graphics);
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
    AudioPlayer_StartSong(FindEntitySoundChannel(farmer->game_object), 188);
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
                AudioPlayer_StartSong(FindEntitySoundChannel(farmer->game_object), 184);
        }
    }
}

struct FarmerInputState
{
    u16 held;
    STRUCT_PAD(0x02, 0x04);
    u16 pressed;
    STRUCT_PAD(0x06, 0x08);
    u16 repeated;
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
    FARMER_TURN_STATE = 24,
};

EC void func_080323C8(AEntity * farmer);

EC void UpdateFarmerTurnInput(FarmerEntity * farmer, FarmerInputState const * input) ENTITY_CODE(0802D844);
EC void UpdateFarmerTurnInput(FarmerEntity * farmer, FarmerInputState const * input)
{
    u32 keys = input != nullptr ? input->pressed : 0;
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

union FarmerToolCyclePressedKeys
{
    u32 word;
    u8 bytes[4];
};

EC void FarmerEntity_CycleTool(FarmerEntity * farmer, bool next);

EC void HandleFarmerToolCycleInput(FarmerEntity * farmer, FarmerInputState const * input) ENTITY_CODE(0802D8E8);
EC void HandleFarmerToolCycleInput(FarmerEntity * farmer, FarmerInputState const * input)
{
    u32 held = 0;
    struct
    {
        FarmerEventRequest event;
        volatile FarmerToolCyclePressedKeys pressed;
    } locals;
    locals.pressed.word = 0;
    u32 repeated = 0;
    if (input)
    {
        held = input->held;
        locals.pressed.word = input->pressed;
        repeated = input->repeated;
    }
    if (farmer->exhaustion_state)
    {
        farmer->state = 0;
        return;
    }
    u32 cycling = ((held << 30) >> 31) | ((held << 22) >> 31);
    if (!cycling)
        goto cancel_cycle;
    if (((repeated << 25) >> 31) | ((repeated << 26) >> 31))
    {
        FarmerEntity_CycleTool(farmer, true);
        return;
    }
    {
        u32 previous_direction = ((repeated << 24) >> 31) | ((repeated << 27) >> 31);
        u32 pressed = locals.pressed.word;
        u32 previous_or_b = ((pressed << 30) >> 31) | previous_direction;
        asm volatile("" : : "r"(previous_or_b));
        u32 previous = previous_or_b | ((pressed << 22) >> 31);
        if (previous)
        {
            FarmerEntity_CycleTool(farmer, false);
            return;
        }
        volatile u8 * pressed_bytes = locals.pressed.bytes;
        u32 low_byte = pressed_bytes[0];
        u32 button_bit = 1;
        cycling = button_bit & low_byte;
        if (cycling)
        {
            GameObject * game = farmer->game_object;
            FarmerEventDispatch const * method = &(*reinterpret_cast<FarmerEventGameVTable const **>(game))->dispatch_event;
            u32 event_id = 0x3CE;
            locals.event.id = event_id;
            locals.event.argument = previous;
            (*method)(game, &locals.event, 0);
            farmer->state = previous;
            return;
        }
        if (!(pressed_bytes[1] & button_bit))
            return;
    }
cancel_cycle:
    farmer->state = cycling;
}

union FarmerHeldItemKeyWord
{
    u32 word;
    u16 halfwords[2];
    u8 bytes[4];
};

EC void FarmerEntity_CycleHeldItem(FarmerEntity *, bool);
EC void FarmerEntity_CycleHeldItemWithFeedback(FarmerEntity *, bool);

EC u32 HeldFoodIsDrink(Food const *) asm("IsDrink__C4Food");

EC void HandleFarmerHeldItemInput(FarmerEntity * farmer, FarmerInputState const * input) ENTITY_CODE(0802D9AC);
EC void HandleFarmerHeldItemInput(FarmerEntity * farmer, FarmerInputState const * input)
{
    struct
    {
        u32 food_storage;
        FarmerEventRequest combined_event;
        FarmerEventRequest single_event;
        FarmerHeldItemKeyWord held;
        FarmerHeldItemKeyWord pressed;
    } locals;
    register u32 clear asm("r2") = 0;
    asm volatile("" : "+r"(clear));
    locals.held.word = clear;
    locals.pressed.word = clear;
    u32 repeated = 0;
    if (input)
    {
        locals.held.word = input->held;
        locals.pressed.word = input->pressed;
        repeated = input->repeated;
    }
    asm volatile("" : : "m"(locals.held), "m"(locals.pressed));
    if (farmer->exhaustion_state)
    {
        farmer->state = clear;
        return;
    }
    u32 held = locals.held.word;
    u32 cycling = ((held << 31) >> 31) | ((held << 22) >> 31);
    if (!cycling)
        goto cancel_cycle;
    if (((repeated << 25) >> 31) | ((repeated << 26) >> 31))
    {
        if (!func_0800F190(&farmer->farmer->unk_54))
            FarmerEntity_CycleHeldItem(farmer, true);
        else
            FarmerEntity_CycleHeldItemWithFeedback(farmer, true);
        return;
    }
    {
        u32 previous_direction = ((repeated << 24) >> 31) | ((repeated << 27) >> 31);
        u32 pressed = locals.pressed.word;
        u32 previous_or_a = ((pressed << 31) >> 31) | previous_direction;
        asm volatile("" : : "r"(previous_or_a));
        u32 previous = ((pressed << 22) >> 31) | previous_or_a;
        if (previous)
        {
            if (!func_0800F190(&farmer->farmer->unk_54))
                FarmerEntity_CycleHeldItem(farmer, false);
            else
                FarmerEntity_CycleHeldItemWithFeedback(farmer, false);
            return;
        }

        volatile u8 * pressed_bytes = locals.pressed.bytes;
        register u32 pressed_low asm("r1") = pressed_bytes[0];
        asm volatile("" : "+r"(pressed_low));
        register u32 button_b asm("r5") = 2;
        asm volatile("" : "+r"(button_b));
        register u32 use_mask asm("r0") = button_b & pressed_low;
        asm volatile("" : "+r"(use_mask));
        u32 upper = use_mask << 24;
        register u8 use asm("r3") = static_cast<u32>(upper) >> 24;
        if (!use)
            goto no_use;
        {
            volatile FarmerHeldItemKeyWord * held_bits = &locals.held;
            u32 held_half = held_bits->halfwords[0];
            u32 both = 0x201;
            register GameObject * game asm("r0");
            register FarmerEventDispatch const * dispatch asm("r3");
            FarmerEventRequest * event;
            register FarmerEventGameVTable const * methods asm("r3");
            if ((both & held_half) == both)
            {
                game = farmer->game_object;
                asm volatile("" : "+r"(game));
                methods = *reinterpret_cast<FarmerEventGameVTable const **>(game);
                dispatch = &methods->dispatch_event;
                event = &locals.combined_event;
                register u32 event_id asm("r2") = 0x3CE;
                event->id = event_id;
                goto dispatch_event;
            }
            if (held_bits->bytes[0] & 1)
            {
                game = farmer->game_object;
                asm volatile("" : "+r"(game));
                methods = *reinterpret_cast<FarmerEventGameVTable const **>(game);
                dispatch = &methods->dispatch_event;
                event = &locals.single_event;
                {
                    register u32 event_id asm("r2") = 0x3CE;
                    event->id = event_id;
                }
dispatch_event:
                event->argument = previous;
                (*dispatch)(game, event, 0);
                farmer->state = previous;
                return;
            }
            register u32 held_high asm("r1") = held_bits->bytes[1];
            asm volatile("" : "+r"(held_high));
            register u32 held_button asm("r0") = button_b & held_high;
            if (held_button)
            {
                if (func_0800F190(&farmer->farmer->unk_54))
                    goto cycle_tool;
                if (!func_0800F388(&farmer->farmer->unk_54) &&
                    func_0800F204(&farmer->farmer->unk_54) == HeldItem::KIND_FOOD)
                {
                    Food * food = reinterpret_cast<Food *>(&locals.food_storage);
                    CopyHeldFood(food, &farmer->farmer->unk_54);
                    FarmerActionVTable const * actions = reinterpret_cast<FarmerActionVTable const **>(farmer)[5];
                    i32 stamina = food->GetStaminaGain();
                    i32 fatigue = food->GetFatigueGain();
                    actions->change_stamina_and_fatigue(farmer, stamina, fatigue);
                    if (!HeldFoodIsDrink(food))
                    {
                        farmer->state = 0xF;
                        AudioPlayer_StartSong(FindEntitySoundChannel(farmer->game_object), 0x65);
                    }
                    else
                    {
                        farmer->state = 0x10;
                        AudioPlayer_StartSong(FindEntitySoundChannel(farmer->game_object), 0x66);
                    }
                    CancelFarmerItemDisplay(farmer);
                    func_0800F390(&farmer->farmer->unk_54);
                    return;
                }
                farmer->state = 0;
                return;
cycle_tool:
                FarmerEntity_CycleTool(farmer, false);
                return;
            }
        }
        return;
no_use:
        if (pressed_bytes[1] & 1)
            farmer->state = use;
    }
    return;
cancel_cycle:
    farmer->state = cycling;
}

EC void HandleFarmerPetInteractionInput(FarmerEntity * farmer, FarmerInputState const * input) ENTITY_CODE(0802DBF0);
EC void HandleFarmerPetInteractionInput(FarmerEntity * farmer, FarmerInputState const * input)
{
    enum { FARMER_STATE_PET_INTERACTION = 0x16 };
    struct
    {
        u32 food_storage;
        FarmerHeldItemKeyWord held;
        FarmerHeldItemKeyWord pressed;
    } locals;
    register u32 clear asm("r2") = 0;
    asm volatile("" : "+r"(clear));
    locals.held.word = clear;
    locals.pressed.word = clear;
    if (input)
    {
        locals.held.word = input->held;
        locals.pressed.word = input->pressed;
    }
    asm volatile("" : : "m"(locals.held), "m"(locals.pressed));
    register void ** channel asm("r4");
    register void ** end asm("r5");
    register u32 hand_empty asm("r1");
    void * player;
    if (farmer->exhaustion_state)
        goto reset;
    {
        register volatile u8 * held_bytes asm("r0") = locals.held.bytes;
        register u32 held_high asm("r1") = held_bytes[1];
        asm volatile("" : "+r"(held_high));
        register u32 button_l asm("r5") = INPUT_L >> 8;
        asm volatile("" : "+r"(button_l));
        register u32 held_l asm("r0") = button_l & held_high;
        if (!held_l)
            goto interact_with_both_pets;

        register u32 pressed asm("r2") = locals.pressed.word;
        asm volatile("" : "+r"(pressed));
        register u32 vertical asm("r1") = ((pressed << 25) >> 31) | ((pressed << 26) >> 31);
        if (vertical)
        {
            {
                register u32 empty_status asm("r0") = FarmerHeldItemIsEmptyRaw(&farmer->farmer->unk_54) << 24;
                asm volatile("" : "+r"(empty_status));
                hand_empty = empty_status >> 24;
            }
            if (!hand_empty)
                goto holding_item;
            farmer->state = FARMER_STATE_PET_INTERACTION;
            farmer->pet_interaction_flags = 1;
            GameObject * game = farmer->game_object;
            EntitySoundGameVTable const * methods = *reinterpret_cast<EntitySoundGameVTable const **>(game);
            EntitySoundChannels const * channels = methods->get_channels(game);
            channel = channels->begin;
            end = channels->end;
            for (; channel != end; ++channel)
            {
                if (!AudioPlayer_IsPlaying(*channel))
                    goto play_available;
            }
            goto play_last;
        }
        register u32 down asm("r1") = (pressed << 24) >> 31;
        register u32 right asm("r0") = (pressed << 27) >> 31;
        register u32 horizontal asm("r1") = down | right;
        if (horizontal)
        {
            {
                register u32 empty_status asm("r0") = FarmerHeldItemIsEmptyRaw(&farmer->farmer->unk_54) << 24;
                asm volatile("" : "+r"(empty_status));
                hand_empty = empty_status >> 24;
            }
            if (!hand_empty)
                goto holding_item;
            farmer->state = FARMER_STATE_PET_INTERACTION;
            farmer->pet_interaction_flags = button_l;
            GameObject * game = farmer->game_object;
            EntitySoundGameVTable const * methods = *reinterpret_cast<EntitySoundGameVTable const **>(game);
            EntitySoundChannels const * channels = methods->get_channels(game);
            channel = channels->begin;
            end = channels->end;
            for (; channel != end; ++channel)
            {
                if (!AudioPlayer_IsPlaying(*channel))
                    goto play_available;
            }
            goto play_last;
        }
        register volatile u8 * pressed_bytes asm("r3") = locals.pressed.bytes;
        register u32 pressed_low asm("r1") = pressed_bytes[0];
        asm volatile("" : "+r"(pressed_low));
        register u32 button_a asm("r4") = INPUT_A;
        asm volatile("" : "+r"(button_a));
        register u32 pressed_a asm("r0") = button_a & pressed_low;
        if (pressed_a)
        {
            if (func_0800F190(&farmer->farmer->unk_54))
                FarmerEntity_CycleHeldItemWithFeedback(farmer, true);
            else
                FarmerEntity_CycleHeldItem(farmer, true);
            return;
        }
        register u32 pressed_b asm("r0") = button_l & pressed_low;
        asm volatile("" : "+r"(pressed_b));
        register u32 pressed_b_shifted asm("r0") = pressed_b << 24;
        asm volatile("" : : "r"(pressed_b_shifted));
        clear = pressed_b_shifted >> 24;
        if (!clear)
            goto press_r;
        if (func_0800F190(&farmer->farmer->unk_54))
        {
            FarmerEntity_CycleTool(farmer, false);
            return;
        }
        if (!func_0800F388(&farmer->farmer->unk_54) && func_0800F204(&farmer->farmer->unk_54) == HeldItem::KIND_FOOD)
        {
            Food * food = reinterpret_cast<Food *>(&locals.food_storage);
            CopyHeldFood(food, &farmer->farmer->unk_54);
            FarmerActionVTable const * actions = reinterpret_cast<FarmerActionVTable const **>(farmer)[5];
            i32 stamina = food->GetStaminaGain();
            i32 fatigue = food->GetFatigueGain();
            actions->change_stamina_and_fatigue(farmer, stamina, fatigue);
            if (!HeldFoodIsDrink(food))
            {
                farmer->state = 0xF;
                AudioPlayer_StartSong(FindEntitySoundChannel(farmer->game_object), 0x65);
            }
            else
            {
                farmer->state = 0x10;
                AudioPlayer_StartSong(FindEntitySoundChannel(farmer->game_object), 0x66);
            }
            CancelFarmerItemDisplay(farmer);
            func_0800F390(&farmer->farmer->unk_54);
            return;
        }
        farmer->state = 0;
        return;
press_r:
        {
            register u32 pressed_high asm("r1") = pressed_bytes[1];
            asm volatile("" : "+r"(pressed_high));
            register u32 pressed_r asm("r0") = button_a & pressed_high;
            if (pressed_r)
                goto reset;
            return;
        }
    }
reset:
    farmer->state = clear;
    return;
play_available:
    player = *channel;
    goto play;
interact_with_both_pets:
    {
        register u32 empty_status asm("r0") = FarmerHeldItemIsEmptyRaw(&farmer->farmer->unk_54) << 24;
        asm volatile("" : "+r"(empty_status));
        hand_empty = empty_status >> 24;
    }
    if (!hand_empty)
        goto holding_item;
    farmer->state = FARMER_STATE_PET_INTERACTION;
    farmer->pet_interaction_flags = 3;
    {
        GameObject * game = farmer->game_object;
        EntitySoundGameVTable const * methods = *reinterpret_cast<EntitySoundGameVTable const **>(game);
        EntitySoundChannels const * channels = methods->get_channels(game);
        channel = channels->begin;
        end = channels->end;
        for (; channel != end; ++channel)
        {
            if (!AudioPlayer_IsPlaying(*channel))
                goto play_available;
        }
    }
play_last:
    {
        register void ** last_channel asm("r0") = end - 1;
        player = *last_channel;
    }
    goto play;
play:
    AudioPlayer_StartSong(player, 0x67);
    return;
holding_item:
    farmer->state = hand_empty;
}

EC void FarmerEntity_StoreHeldItem(FarmerEntity *);
EC void func_0802C2E0(FarmerEntity *);



EC void HandleFarmerToolChargeInput(FarmerEntity * farmer, FarmerInputState const * input) ENTITY_CODE(0802DE94);
EC void HandleFarmerToolChargeInput(FarmerEntity * farmer, FarmerInputState const * input)
{
    struct
    {
        u32 food_storage;
        FarmerHeldItemKeyWord pressed;
        FarmerEventRequest event;
    } locals;
    register u32 held asm("r2") = 0;
    asm volatile("" : "+r"(held));
    locals.pressed.word = held;
    if (input)
    {
        held = input->held;
        locals.pressed.word = input->pressed;
    }
    asm volatile("" : : "m"(locals.pressed));
    if (farmer->exhaustion_state)
    {
        farmer->state = 0;
        return;
    }
    {
        register u32 button_b asm("r5") = INPUT_B;
        asm volatile("" : "+r"(button_b));
        register u32 held_b asm("r2") = held & button_b;
        asm volatile("" : "+r"(held_b));
        register u32 shifted asm("r0") = held_b << 24;
        asm volatile("" : : "r"(shifted));
        register u32 clear_state_value asm("r4") = shifted >> 24;
        if (!clear_state_value)
            goto release;
        if (func_0800F190(&farmer->farmer->unk_54))
            TickFarmerToolCharge(farmer);

        {
            register u32 pressed asm("r2") = locals.pressed.word;
            asm volatile("" : "+r"(pressed));
            register u32 vertical_input asm("r1") = (pressed << 25) >> 31;
            asm volatile("" : "+r"(vertical_input));
            vertical_input |= (pressed << 24) >> 31;
            asm volatile("" : "+r"(vertical_input));
            register u32 vertical_or_left asm("r0") = ((pressed << 26) >> 31) | vertical_input;
            asm volatile("" : "+r"(vertical_or_left));
            register u32 has_direction asm("r2") = ((pressed << 27) >> 31) | vertical_or_left;
            if (has_direction)
            {
                register volatile u8 * direction_bytes asm("r0") = locals.pressed.bytes;
                register u32 pressed_low asm("r1") = direction_bytes[0];
                if (INPUT_LEFT & pressed_low)
                {
                    if (farmer->facing != FACING_LEFT)
                        farmer->SetAnimFacing(FACING_LEFT);
                    return;
                }
                if (INPUT_RIGHT & pressed_low)
                {
                    if (farmer->facing != FACING_RIGHT)
                        farmer->SetAnimFacing(FACING_RIGHT);
                    return;
                }
                if (INPUT_UP & pressed_low)
                {
                    if (farmer->facing != FACING_UP)
                        farmer->SetAnimFacing(FACING_UP);
                    return;
                }
                if (INPUT_DOWN & pressed_low)
                {
                    if (farmer->facing != FACING_DOWN)
                        farmer->SetAnimFacing(FACING_DOWN);
                }
                return;
            }
            register volatile u8 * pressed_bytes asm("r1") = locals.pressed.bytes;
            register u32 pressed_high asm("r3") = pressed_bytes[1];
            register u32 button_a asm("r0") = 1;
            asm volatile("" : "+r"(pressed_high), "+r"(button_a));
            clear_state_value = button_a & pressed_high;
            if (clear_state_value)
            {
                farmer->state = has_direction;
                return;
            }
            register u32 pressed_low asm("r1") = pressed_bytes[0];
            register u32 pressed_a asm("r0") = button_a & pressed_low;
            if (pressed_a)
            {
                register GameObject * game asm("r0") = farmer->game_object;
                asm volatile("" : "+r"(game));
                register FarmerEventGameVTable const * methods asm("r3") = *reinterpret_cast<FarmerEventGameVTable const **>(game);
                register FarmerEventDispatch const * dispatch asm("r3") = &methods->dispatch_event;
                register FarmerEventRequest * request asm("r1") = &locals.event;
                asm volatile("" : : "r"(request));
                register u32 event_id asm("r2") = 0x3CE;
                locals.event.id = event_id;
                asm volatile("" : : "m"(locals.event.id));
                request->argument = clear_state_value;
                (*dispatch)(game, request, 0);
                goto clear_state;
            }
            register u32 pressed_l asm("r0") = button_b;
            pressed_l &= pressed_high;
            if (!pressed_l)
                return;

            if (func_0800F190(&farmer->farmer->unk_54))
            {
                FarmerEntity_CycleTool(farmer, false);
                return;
            }
            if (!func_0800F388(&farmer->farmer->unk_54) && func_0800F204(&farmer->farmer->unk_54) == HeldItem::KIND_FOOD)
            {
                Food * food = reinterpret_cast<Food *>(&locals.food_storage);
                CopyHeldFood(food, &farmer->farmer->unk_54);
                FarmerActionVTable const * actions = reinterpret_cast<FarmerActionVTable const **>(farmer)[5];
                i32 stamina = food->GetStaminaGain();
                i32 fatigue = food->GetFatigueGain();
                actions->change_stamina_and_fatigue(farmer, stamina, fatigue);
                if (!HeldFoodIsDrink(food))
                {
                    farmer->state = 0xF;
                    AudioPlayer_StartSong(FindEntitySoundChannel(farmer->game_object), 0x65);
                }
                else
                {
                    farmer->state = 0x10;
                    AudioPlayer_StartSong(FindEntitySoundChannel(farmer->game_object), 0x66);
                }
                CancelFarmerItemDisplay(farmer);
                func_0800F390(&farmer->farmer->unk_54);
                return;
            }
        }
        goto reset;
reset:
        farmer->state = 0;
        return;
release:
        if (!func_0800F190(&farmer->farmer->unk_54))
        {
            FarmerEntity_StoreHeldItem(farmer);
            return;
        }
        if (farmer->tool_state == 0xF)
        {
clear_state:
            farmer->state = clear_state_value;
            return;
        }
        if (farmer->tool_state == 0xE)
        {
            BeginFarmerToolDisplay(farmer);
            return;
        }
        func_0802C2E0(farmer);
        return;
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

EC u32 SumSpriteAnimationFrameDelays(SpriteAnimationSource const * source, u32 animation) ENTITY_CODE(08032090);
EC u32 SumSpriteAnimationFrameDelays(SpriteAnimationSource const * source, u32 animation)
{
    SpriteAnimationClip loaded = source->GetClip(animation);
    SpriteAnimationClip clip = loaded;
    SpriteAnimationClip * sequence = &clip;
    u32 total_delay = 0;
    u32 frame = 0;
    u32 count = static_cast<u16>(sequence->GetLength());
    if (frame < count)
    {
        SpriteKeyframe const * next = sequence->keyframes;
        do
        {
            total_delay += (next++)->duration;
            ++frame;
        } while (frame < count);
    }
    return total_delay;
}
