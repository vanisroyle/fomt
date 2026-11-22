#include "coop.hh"
#include "dma_queue.hh"
#include "dog.hh"
#include "farmer_entity_layout.hh"
#include "entity_sound_channel.hh"
#include "farm.hh"
#include "farm_house.hh"
#include "fixed_sprite_overlay.hh"
#include "g1_map_resources.hh"
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

EC u32 AudioPlayerIsPlayingRaw(void *) asm("AudioPlayer_IsPlaying");

EC void BeginFarmerHeldItemThrow(FarmerEntity *) ENTITY_CODE(080299EC);
EC void BeginFarmerHeldItemThrow(FarmerEntity * farmer)
{
    register FarmerEntity * self asm("r6") = farmer;
    register bool * item_override asm("r1") = &self->item_animation_override;
    asm volatile("" : : "r"(self), "r"(item_override));
    *item_override = true;
    RefreshFarmerHeldItemDisplay(self);
    u32 empty = FarmerHeldItemIsEmptyRaw(&self->farmer->unk_54) << 24;
    asm volatile("" : "+r"(empty));
    register u8 * state asm("r8");
    u16 * icon_field;
    register bool * animation_active asm("sl");
    if (!empty)
    {
        GameObject * game = self->game_object;
        EntitySoundGameVTable const * table = *reinterpret_cast<EntitySoundGameVTable const **>(game);
        register EntitySoundChannels const * (*get_channels)(GameObject *) asm("r1") = table->get_channels;
        asm volatile("" : : "r"(get_channels));
        EntitySoundChannels const * channels = get_channels(game);
        register void ** channel asm("r4") = channels->begin;
        register void ** last asm("r5") = channels->end;
        asm volatile("" : : "r"(channel), "r"(last));
        state = &self->state;
        asm volatile("" : "+r"(state));
        register u8 const * facing_field asm("sb") = &self->facing;
        asm volatile("" : "+r"(facing_field));
        icon_field = &self->icon;
        asm volatile("" : "+r"(icon_field));
        {
            register u32 offset asm("r1") = 0xC0;
            asm volatile("" : "+r"(offset));
            bool * active_ptr = reinterpret_cast<bool *>(offset + reinterpret_cast<u8 *>(self));

            animation_active = active_ptr;
            asm volatile("" : "+r"(animation_active));
        }
        register u32 zero asm("r4");
        void * selected;
        if (channel != last)
        {
            do
            {
                u32 playing = AudioPlayerIsPlayingRaw(*channel) << 24;
                asm volatile("" : "+r"(playing));
                if (!playing)
                    goto have_channel;
                ++channel;
            } while (channel != last);
        }
        {
            register void ** previous asm("r0") = last - 1;
            asm volatile("" : "+r"(previous));
            selected = *previous;
        }
    start_sound:
        AudioPlayer_StartSong(selected, 106);
        zero = 0;
        asm volatile("" : "+r"(zero));
        {
            register u32 throwing asm("r0") = 17;
            asm volatile("" : "+r"(throwing));
            register u8 * state_dest asm("r2") = state;
            asm volatile("" : "+r"(state_dest));
            *state_dest = throwing;
        }
        {
            register u8 const * direction_ptr asm("r0");
            u32 direction;
            direction_ptr = facing_field;
            asm volatile("" : : "r"(direction_ptr));
            direction = *direction_ptr;
            asm volatile("" : "+r"(direction));
            register ThrownItemMotion * thrown asm("r0");
            register i32 speed asm("r1");
            register i32 rise asm("r2");
            if (static_cast<i32>(direction) == FACING_UP)
                goto up;
            asm volatile("" : "+r"(direction));
            if (static_cast<i32>(direction) > FACING_UP)
                goto horizontal_direction;
            asm volatile("" : "+r"(direction));
            if (direction == FACING_DOWN)
                goto down;
            goto select_animation;
        have_channel:
            selected = *channel;
            goto start_sound;
        horizontal_direction:
            if (direction == FACING_LEFT)
                goto left;
            if (direction == FACING_RIGHT)
                goto right;
            goto select_animation;
        left:
                thrown = &self->thrown_item;
                speed = -10 * 65536;
                asm volatile("" : : "r"(thrown), "r"(speed));
                goto horizontal;
        right:
                thrown = &self->thrown_item;
                speed = 10 * 65536;
                asm volatile("" : : "r"(thrown), "r"(speed));
            horizontal:
                rise = 12 * 65536;
                asm volatile("" : : "r"(rise));
                thrown->x_speed = speed;
                thrown->y_speed = zero;
                thrown->z_speed = rise;
                goto select_animation;
        up:
                thrown = &self->thrown_item;
                speed = -10 * 65536;
                rise = 12 * 65536;
                asm volatile("" : : "r"(thrown), "r"(speed), "r"(rise));
                thrown->x_speed = zero;
                goto vertical;
        down:
                thrown = &self->thrown_item;
                speed = 10 * 65536;
                rise = 12 * 65536;
                asm volatile("" : : "r"(thrown), "r"(speed), "r"(rise));
                thrown->x_speed = direction;
            vertical:
                thrown->y_speed = speed;
                thrown->z_speed = rise;
                goto select_animation;
        }
        goto select_animation;
    }
    {
        register u8 * state_ptr asm("r1") = &self->state;
        asm volatile("" : : "r"(state_ptr));
        *state_ptr = 0;
        state = state_ptr;
        asm volatile("" : "+r"(state));
        icon_field = &self->icon;
        asm volatile("" : "+r"(icon_field));
        {
            register u32 offset asm("r1") = 0xC0;
            asm volatile("" : "+r"(offset));
            bool * active_ptr = reinterpret_cast<bool *>(offset + reinterpret_cast<u8 *>(self));

            animation_active = active_ptr;
            asm volatile("" : "+r"(animation_active));
        }
    }
select_animation:
    register u32 animation asm("r5") = FARMER_IDLE_ANIMATION;
    asm volatile("" : : "r"(animation), "r"(icon_field));
    register u32 hidden asm("r0") = 0xFFFF;
    asm volatile("" : : "r"(hidden));
    *static_cast<volatile u16 *>(icon_field) = hidden;
    register u8 const * state_address asm("r2") = state;
    asm volatile("" : : "r"(state_address));
    u32 state_code = *state_address;
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
            { asm volatile(""); goto hold_still; }
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
                { animation -= 0x40; asm volatile("" : "+r"(animation)); }
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
                { animation -= 0x40; asm volatile("" : "+r"(animation)); }
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
    register u32 active asm("r0") = 1;
    asm volatile("" : : "r"(active));
    register bool * active_flag asm("r1") = animation_active;
    asm volatile("" : "+r"(active_flag));
    *active_flag = active;
}

EC void InitFarmerToolRaw(Tool *, u32) asm("__4ToolUi");

EC void BeginFarmerToolDisplayAnimation(FarmerEntity *, u32) ENTITY_CODE(08029F30);
EC void BeginFarmerToolDisplayAnimation(FarmerEntity * farmer, u32 display_tool_id)
{
    register FarmerEntity * self asm("r6") = farmer;
    register u32 tool_kind asm("r3") = display_tool_id;
    asm volatile("" : : "r"(self), "r"(tool_kind));
    register bool * override_animation asm("r0") = &farmer->item_animation_override;
    register u32 zero asm("r8") = 0;
    asm volatile("" : "+r"(zero) : "r"(override_animation));
    typedef u8 EncodedTool;
    u32 tool_values_storage[2];
    register EncodedTool * tool_values asm("sp");
    asm volatile("" : "=r"(tool_values), "+m"(tool_values_storage));
    register u32 active asm("r7") = 1;
    asm volatile("" : "+r"(active));
    *reinterpret_cast<u8 *>(override_animation) = active;
    register ThrownItemMotion * thrown asm("r0") = reinterpret_cast<ThrownItemMotion *>(reinterpret_cast<u8 *>(override_animation) - 0x1C);
    asm volatile("" : "+r"(thrown));
    *reinterpret_cast<u8 *>(thrown) = active;
    register u32 display_mode asm("r1") = zero;
    asm volatile("" : : "r"(display_mode));
    thrown->display_mode = display_mode;
    register i32 x_speed asm("r1") = -8 * 65536;
    register i32 z_speed asm("r2") = 17 * 65536;
    asm volatile("" : : "r"(x_speed), "r"(z_speed));
    thrown->x_speed = x_speed;
    display_mode = zero;
    asm volatile("" : : "r"(display_mode));
    thrown->y_speed = display_mode;
    thrown->z_speed = z_speed;
    register FarmerIconGraphics * graphics asm("r5") = static_cast<FarmerIconGraphics *>(self->unk_10.Get());
    asm volatile("" : "+r"(graphics));
    if (graphics)
    {
        register Tool * icon asm("r0");
        icon = reinterpret_cast<Tool *>(tool_values);
        InitFarmerToolRaw(icon, tool_kind);
        icon = reinterpret_cast<Tool *>(tool_values);
        asm volatile("" : "+r"(icon));
        register u32 frame asm("r4") = ReadFarmerToolIconRaw(icon);
        asm volatile("" : "+r"(frame));
        frame = static_cast<u16>(frame);
        asm volatile("" : : "r"(frame));
        void * sprites = FarmerItemSprites(graphics);
        func_0805E850(&graphics->icon_animator, sprites, frame);
        register u8 * flags asm("r0") = reinterpret_cast<u8 *>(&graphics->icon_state);
        asm volatile("" : "+r"(flags));
        *flags = active;
        flags += 2;
        register u32 stopped asm("r3") = zero;
        asm volatile("" : : "r"(stopped));
        *flags = stopped;
        flags += 1;
        asm volatile("" : "+r"(flags));
        *flags = active;
    }
    register u8 * state asm("r3") = &self->state;
    asm volatile("" : "+r"(state));
    register u32 displaying_tool asm("r0") = 0x34;
    asm volatile("" : "+r"(displaying_tool));
    *state = displaying_tool;
    register u8 * delay asm("r2") = &self->unk_3D;
    asm volatile("" : "+r"(delay));
    register u32 display_delay asm("r0") = 120;
    asm volatile("" : "+r"(display_delay));
    *delay = display_delay;
    register u32 animation asm("r5") = FARMER_IDLE_ANIMATION;
    asm volatile("" : "+r"(animation));
    register u16 * icon_address asm("r1") = &self->icon;
    asm volatile("" : "+r"(icon_address));
    register u32 hidden asm("r0") = 0xFFFF;
    asm volatile("" : "+r"(hidden));
    *icon_address = hidden;
    register u32 state_code asm("r0") = *state;
    asm volatile("" : "+r"(state_code));
    u16 * icon_field = icon_address;
    asm volatile("" : "+r"(icon_field) : "r"(state_code));
    register AActorEntity * horse asm("r2");
    register u32 horse_animation asm("r1");
    register EncodedTool * stored_tool_id asm("r0");
    register u32 tool_id asm("r1");
    Tool * icon_tool;
    register u8 const * previous_tool_step asm("r2");
    register u8 const * current_tool_step asm("r1");

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
            tool_id = ReadResolvedToolIdRaw(*inventory, inventory->unk_5C.GetTool());
            stored_tool_id = &tool_values[1];
            goto store_tool_id;
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
            { asm volatile(""); goto hold_still; }
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
                { animation -= 0x40; asm volatile("" : "+r"(animation)); }
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
            stored_tool_id = &tool_values[2];
            goto store_tool_id;
        }
    case 0x1B:
        animation = 0x1EA;
        {
            Farmer * inventory = self->farmer;
            tool_id = ReadResolvedToolIdRaw(*inventory, inventory->unk_5C.GetTool());
            stored_tool_id = &tool_values[3];
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
            stored_tool_id = &tool_values[4];
            goto store_tool_id;
        }
    case 0x1E:
        animation = 0x76;
        {
            Farmer * inventory = self->farmer;
            tool_id = ReadResolvedToolIdRaw(*inventory, inventory->unk_5C.GetTool());
            stored_tool_id = &tool_values[5];
            goto store_tool_id;
        }
    case 0x1F:
        animation = 0x6E;
        {
            Farmer * inventory = self->farmer;
            tool_id = ReadResolvedToolIdRaw(*inventory, inventory->unk_5C.GetTool());
            stored_tool_id = &tool_values[6];
            goto store_tool_id;
        }
    case 0x20:
        animation = 0x72;
        {
            Farmer * inventory = self->farmer;
            tool_id = ReadResolvedToolIdRaw(*inventory, inventory->unk_5C.GetTool());
            stored_tool_id = &tool_values[7];
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
                { animation -= 0x40; asm volatile("" : "+r"(animation)); }
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
            u32 variant = *delay;
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
    register bool * active_flag asm("r1") = &self->consume_animation_active;
    register u32 enabled asm("r0") = 1;
    asm volatile("" : : "r"(active_flag), "r"(enabled));
    *active_flag = enabled;
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

struct OamBuffer;

struct ItemDisplayRenderContext
{
    DmaTransferQueue * tile_uploads;
    STRUCT_PAD(4, 8);
    OamBuffer * sprite_queue;
    i16 scroll_x;
    i16 scroll_y;
};
typedef FixedOverlayFrameView ItemDisplayFrame;
struct ItemDisplayResources
{
    DmaTransferQueue * tile_uploads;
    OamBuffer * sprite_queue;
};
struct ItemDisplayFrameData
{
    u32 stack_arguments[5];
    ItemDisplayFrame frame;
    ItemDisplayResources resources;
    u32 clip[2];
    i32 map;
    u32 base_sprite_visible;
    i32 y_q16;
    u32 region;
    i32 screen_x, sort_y, screen_y;
    ItemDisplayResources const * saved_resources;
    u8 * frame_upload_pending;
    i32 x_q16;
};
struct ItemDisplaySpriteSource;
struct ItemDisplaySpriteVTable
{
    STRUCT_PAD(0, 0xC);
    void (*get_clip)(u32 *, ItemDisplaySpriteSource const *, u32);
    void (*get_frame)(ItemDisplayFrame *, ItemDisplaySpriteSource const *, u32);
};
struct ItemDisplaySpriteSource
{
    ItemDisplaySpriteVTable const * table;
};
struct ItemDisplayRenderVTable
{
    STRUCT_PAD(0, 0x48);
    void * (*get_icon_target)(GameObject *, u32);
    STRUCT_PAD(0x4C, 0x54);
    void (*upload_icon_palette)(GameObject *, void const *, void *, u32);
    FixedSpriteOverlay const * (*get_shadow)(GameObject *);
    STRUCT_PAD(0x5C, 0x64);
    ItemDisplaySpriteSource const * (*get_sprites)(GameObject *);
};
struct ItemDisplaySprite
{
    void * sprite_resources;
    STRUCT_PAD(4, 8);
    i32 tile_handle;
    u16 base_tile;
    STRUCT_PAD(0xE, 0x3C);
    u8 frame_upload_pending;
    u8 force_palette_upload;
    u8 palette_uploaded;
};

EC u32 ClassifySideEntityFarmRegion(u32, i32, i32);
EC void DrawSideActorOverlayLayers(FarmerIconGraphics *, ItemDisplayRenderContext const *) asm("func_08032690");
EC void QueueItemDisplayTileUpload(G1MapSpriteResource *, DmaTransferQueue *, G1MapResourceData const *) asm("func_080A480C");
EC void LoadMapSpriteResourcePalettes(G1MapSpriteResource *, G1MapResourceData const *, u8);

typedef u32 (*ItemDisplayQueueCall)(OamBuffer *, i32, i32, u32);
EC u32 QueueItemDisplayFrame(OamBuffer *, i32, i32, u32) asm("func_030004DC");
EC void DrawItemDisplayShadow(FixedSpriteOverlay const *, OamBuffer *, i32, i32) asm("DrawFixedSpriteOverlay");

#define ITEM_DISPLAY_GAME(game) (*reinterpret_cast<ItemDisplayRenderVTable const **>(game))

EC void DrawItemDisplayGraphics(ItemDisplayGraphics *, ItemDisplayRenderContext const *) ENTITY_CODE(0802BFE4);
EC void DrawItemDisplayGraphics(ItemDisplayGraphics * graphics, ItemDisplayRenderContext const * render)
{
    register ItemDisplayGraphics * self asm("r6") = graphics;
    register ItemDisplayRenderContext const * context asm("r8") = render;
    ItemDisplayFrameData local;
    register ItemDisplayFrameData * stack asm("sp");
    register ItemDisplayEntity * display asm("r4") = *reinterpret_cast<ItemDisplayEntity **>(self);
    local.map = display->location_map;
    asm volatile("" : "+m"(local.map));
    register GameObject * game_arg asm("r1") = display->game_object;
    asm volatile("" : "+r"(game_arg));
    register GameObject * game asm("sb") = game_arg;
    local.base_sprite_visible = self->unk_44;
    register i32 x_q16 asm("r2") = display->x_q16;
    local.x_q16 = x_q16;
    register i32 x asm("r1") = x_q16 >> 16;
    register i32 y_q16 asm("r0") = display->y_q16;
    local.y_q16 = y_q16;
    register i32 y asm("r2") = y_q16 >> 16;
    register ItemDisplayResources * resources asm("r5") = &local.resources;
    register ItemDisplayRenderContext const * resource_context asm("r3") = context;
    register DmaTransferQueue * tile_uploads asm("r0") = resource_context->tile_uploads;
    register OamBuffer * sprite_queue asm("r3") = resource_context->sprite_queue;
    asm volatile("" : "+r"(sprite_queue));
    local.resources.tile_uploads = tile_uploads;
    resources->sprite_queue = sprite_queue;
    local.region = ClassifySideEntityFarmRegion(local.map, x, y);
    register u8 * region_dest asm("r0") = reinterpret_cast<u8 *>(self) + 0x8B;
    register u32 keep_byte_r1 asm("r1");
    register u32 keep_byte_r2 asm("r2");
    register u32 keep_byte_r3 asm("r3");
    register u32 keep_byte_r5 asm("r5");
    asm volatile("" : "=l"(keep_byte_r1), "=l"(keep_byte_r2), "=l"(keep_byte_r3), "=l"(keep_byte_r5));
    register u8 const * region_slot = reinterpret_cast<u8 const *>(&local.region);
    asm volatile("" : "+l"(region_slot));
    u32 region_byte = *region_slot;
    asm volatile("" : "+l"(region_byte) : "l"(keep_byte_r1), "l"(keep_byte_r2), "l"(keep_byte_r3), "l"(keep_byte_r5) : "memory");
    *region_dest = region_byte;
    register ThrownItemMotion * display_motion asm("r4") = &display->thrown_item;
    register ThrownItemMotion * motion asm("sl") = display_motion;
    register u32 displaying_throw asm("r1") = 0;
    if (display_motion->active == 1)
        displaying_throw = 1;
    if (displaying_throw)
    {
        register ThrownItemMotion * motion_position asm("r1") = motion;
        register i32 item_x_q16 asm("r0") = motion_position->x_speed;
        register i32 world_x asm("r2") = local.x_q16;
        item_x_q16 = world_x + item_x_q16;
        item_x_q16 >>= 16;
        register ItemDisplayRenderContext const * scroll asm("r3") = context;
        register u32 keep_camera_x_r2 asm("r2");
        register u32 keep_camera_x_r4 asm("r4");
        register u32 keep_camera_x_r5 asm("r5");
        asm volatile("" : "=l"(keep_camera_x_r2), "=l"(keep_camera_x_r4), "=l"(keep_camera_x_r5));
        register i32 camera_x asm("r1") = *reinterpret_cast<i16 const *>(reinterpret_cast<u8 const *>(scroll) + 0xC);
        asm volatile("" : "+l"(camera_x) : "l"(keep_camera_x_r2), "l"(keep_camera_x_r4), "l"(keep_camera_x_r5) : "memory");
        local.screen_x = item_x_q16 - camera_x;
        motion_position = motion;
        register i32 y_offset_q16 asm("r0") = motion_position->y_speed;
        register i32 entity_y_q16 asm("r2") = local.y_q16;
        register i32 world_y asm("r4") = entity_y_q16 + y_offset_q16;
        register i32 ground_y asm("r1") = world_y >> 16;
        register ItemDisplayRenderContext const * camera_base = scroll;
        asm volatile("" : "+l"(camera_base));
        register u32 keep_camera_y_r2 asm("r2");
        register u32 keep_camera_y_r4 asm("r4");
        register u32 keep_camera_y_r5 asm("r5");
        asm volatile("" : "=l"(keep_camera_y_r2), "=l"(keep_camera_y_r4), "=l"(keep_camera_y_r5));
        register i32 camera_y asm("r0") = camera_base->scroll_y;
        asm volatile("" : "+l"(camera_y) : "l"(keep_camera_y_r2), "l"(keep_camera_y_r4), "l"(keep_camera_y_r5) : "memory");

        register i32 shadow_y asm("r7") = ground_y - camera_y;
        local.sort_y = 0x8000 - ground_y;
        register u32 show_shadow asm("r1") = 0;
        register ThrownItemMotion * display_mode asm("r2") = motion;
        if (display_mode->display_mode == 1)
            show_shadow = 1;
        if (show_shadow)
        {
            register GameObject * shadow_game asm("r3") = game;
            register FixedSpriteOverlay const * (*get_shadow)(GameObject *) asm("r1") = ITEM_DISPLAY_GAME(shadow_game)->get_shadow;
            FixedSpriteOverlay const * geometry = get_shadow(game);
            register ItemDisplayRenderContext const * queue_context asm("r2") = context;
            register OamBuffer * sprite_queue asm("r1") = queue_context->sprite_queue;
            register u32 shadow_frame asm("r2") = 0;
            stack->stack_arguments[0] = shadow_frame;
            asm volatile("" : : "m"(stack->stack_arguments[0]) : "memory");
            register i32 shadow_x asm("r2") = local.screen_x;
            DrawItemDisplayShadow(geometry, sprite_queue, shadow_x, shadow_y);
        }
        register ThrownItemMotion * motion_for_height asm("r3") = motion;
        register i32 elevation asm("r0") = motion_for_height->z_speed;
        elevation = world_y - elevation;
        elevation >>= 16;
        register ItemDisplayRenderContext const * camera = context;
        asm volatile("" : "+r"(camera) : : "r1", "r2", "r3", "r4", "r5");
        register i32 camera_y_again asm("r1") = camera->scroll_y;
        asm volatile("" : "+l"(camera_y_again) : "l"(camera), "l"(elevation) : "memory");
        local.screen_y = elevation - camera_y_again;
        register u32 priority asm("r0");
        register i32 map asm("r3") = local.map;
        if (map <= 8)
        {
            priority = 1;
            register u32 region asm("r7") = local.region;
            if (region != 1)
                priority = 2;
        }
        else
            priority = 1;
        register ItemDisplaySprite * sprite asm("r5") = reinterpret_cast<ItemDisplaySprite *>(reinterpret_cast<u8 *>(self) + 0x8C);
        register ItemDisplayFrameData * call_frame asm("sp");
        register u8 * resources_address asm("r1") = reinterpret_cast<u8 *>(call_frame);
        asm volatile("" : "+l"(resources_address));
        resources_address += sizeof(ItemDisplayFrame) + sizeof(local.stack_arguments);
        asm volatile("" : "+l"(resources_address));
        local.saved_resources = reinterpret_cast<ItemDisplayResources const *>(resources_address);
        register u32 priorities asm("r4") = (priority << 2) | priority;
        priorities |= priority << 4;
        priorities |= priority << 6;
        register SpriteAnimator * animator asm("r0") = &self->icon_animator;
        register ItemDisplaySpriteSource const * source asm("r1") = reinterpret_cast<ItemDisplaySpriteSource const *>(animator->source);
        ItemDisplaySpriteVTable const * table = source->table;
        register u32 frame_index asm("r2") = animator->keyframe;
        register SpriteKeyframe const ** keys_slot asm("r0") = &animator->clip.keyframes;
        frame_index <<= 2;
        asm volatile("" : "+r"(frame_index));
        register u8 const * keys asm("r0") = reinterpret_cast<u8 const *>(*keys_slot);
        keys += frame_index;
        register u32 frame_id asm("r2") = *reinterpret_cast<u16 const *>(keys);
        register ItemDisplayFrame * frame_dest asm("r0") = &local.frame;
        register void (*get_frame)(ItemDisplayFrame *, ItemDisplaySpriteSource const *, u32) asm("r3") = table->get_frame;
        get_frame(frame_dest, source, frame_id);
        register ItemDisplayFrameData * current_frame asm("sp");
        ItemDisplayFrame * frame = &current_frame->frame;
        register ItemDisplayResources const * saved_resources asm("r2") = local.saved_resources;
        register OamBuffer * queue_arg asm("r2") = saved_resources->sprite_queue;
        register OamBuffer * queue asm("ip") = queue_arg;
        register void * sprite_resources asm("r2") = sprite->sprite_resources;
        asm volatile("" : "+l"(sprite_resources));
        register G1MapTileOwner const * tiles asm("r0") = &reinterpret_cast<G1MapSpriteResource *>(reinterpret_cast<u8 *>(self) + 0x8C)->tiles;
        register i32 handle asm("r1") = tiles->handle;
        register i32 handle_check asm("r0") = -handle | handle;
        u32 queued;
        if (handle_check >= 0)
            goto no_sprite;
        {
            register u32 tile asm("r0") = sprite->base_tile;
            register OverlayPaletteTable const * palettes asm("r1") = reinterpret_cast<OverlayPaletteTable *>(reinterpret_cast<u8 *>(self) + 0x9C);
            register u32 sort_y asm("r3") = local.sort_y;
            stack->stack_arguments[0] = sort_y;
            asm volatile("" : : "m"(stack->stack_arguments[0]) : "memory");
            register ItemDisplayFrame const * frame_arg asm("r3") = frame;
            asm volatile("" : "+l"(frame_arg));
            stack->stack_arguments[1] = reinterpret_cast<u32>(frame_arg);
            asm volatile("" : : "m"(stack->stack_arguments[1]) : "memory");
            stack->stack_arguments[2] = reinterpret_cast<u32>(sprite_resources);
            asm volatile("" : : "m"(stack->stack_arguments[2]) : "memory");
            stack->stack_arguments[3] = tile;
            asm volatile("" : : "m"(stack->stack_arguments[3]) : "memory");
            stack->stack_arguments[4] = reinterpret_cast<u32>(palettes);
            asm volatile("" : : "m"(stack->stack_arguments[4]) : "memory");
            register OamBuffer * queue_arg asm("r0") = queue;
            register i32 screen_x asm("r1") = local.screen_x;
            register i32 screen_y asm("r2") = local.screen_y;
            register u32 layer asm("r3") = priorities;
            register ItemDisplayQueueCall enqueue asm("r4") = QueueItemDisplayFrame;
            queued = enqueue(queue_arg, screen_x, screen_y, layer);
            goto check_enqueued;
        }
    no_sprite:
        queued = 0;
    check_enqueued:
        if (queued)
        {
            register u8 * pending asm("r0") = &sprite->frame_upload_pending;
            register u32 upload_pending asm("r1") = *pending;
            local.frame_upload_pending = pending;
            if (upload_pending)
            {
                QueueItemDisplayTileUpload(reinterpret_cast<G1MapSpriteResource *>(sprite), local.saved_resources->tile_uploads,
                    reinterpret_cast<G1MapResourceData const *>(&frame->tileBytes));
                if (sprite->force_palette_upload)
                    LoadMapSpriteResourcePalettes(reinterpret_cast<G1MapSpriteResource *>(sprite),
                        reinterpret_cast<G1MapResourceData const *>(&frame->paletteBytes), 1);
                else
                {
                    register u8 * loaded asm("r4") = &sprite->palette_uploaded;
                    if (!*loaded)
                    {
                        LoadMapSpriteResourcePalettes(reinterpret_cast<G1MapSpriteResource *>(sprite),
                            reinterpret_cast<G1MapResourceData const *>(&frame->paletteBytes), 1);
                        *loaded = 1;
                    }
                }
                register u32 zero asm("r0") = 0;
                *local.frame_upload_pending = zero;
            }
        }
    }
    DrawSideActorOverlayLayers(self, context);
    register u32 base_sprite_visible asm("r2") = local.base_sprite_visible;
    if (!base_sprite_visible)
        return;
    register u32 thrown asm("r1") = 0;
    register ThrownItemMotion * thrown_item asm("r3") = motion;
    register u32 is_active asm("r0") = thrown_item->active;
    if (is_active == 1)
        thrown = 1;
    if (thrown)
        return;
    register u16 * icon asm("r5") = &self->icon;
    if (*icon == 0xFFFF)
        return;
    register GameObject * icon_game asm("r7") = game;
    register ItemDisplaySpriteSource const * (*get_sprites)(GameObject *) asm("r1") = ITEM_DISPLAY_GAME(icon_game)->get_sprites;
    register ItemDisplaySpriteSource const * sprites asm("r4") = get_sprites(game);
    register u32 clip asm("r2") = *icon;
    register ItemDisplaySpriteVTable const * table asm("r5") = sprites->table;
    register u32 * clip_destination asm("r0") = local.clip;
    register void (*get_clip)(u32 *, ItemDisplaySpriteSource const *, u32) asm("r3") = table->get_clip;
    get_clip(clip_destination, sprites, clip);
    register u32 frame_id asm("r2") = *reinterpret_cast<u16 *>(local.clip[0]);
    register ItemDisplayFrame * frame_destination asm("r0") = &local.frame;
    register void (*get_frame)(ItemDisplayFrame *, ItemDisplaySpriteSource const *, u32) asm("r3") = table->get_frame;
    get_frame(frame_destination, sprites, frame_id);
    register void * (*get_target)(GameObject *, u32) asm("r2") = ITEM_DISPLAY_GAME(icon_game)->get_icon_target;
    register void * target asm("r2") = get_target(game, 2);
    register ItemDisplayRenderVTable const * icon_table asm("r0") = ITEM_DISPLAY_GAME(icon_game);
    register void const * palette asm("r1") = local.frame.paletteBytes.data;
    register void (*upload_icon_palette)(GameObject *, void const *, void *, u32) asm("r4") = icon_table->upload_icon_palette;
    upload_icon_palette(game, palette, target, 1);
}

#undef ITEM_DISPLAY_GAME

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

struct FarmerToolSwing
{
    u16 animation;
    u16 song;
    i8 stamina;
    i8 cursed_stamina;
    u16 reserved;
};
struct FarmerToolTarget
{
    u32 predicate;
    u32 context;
};
struct FarmerToolTargetActions
{
    STRUCT_PAD(0, 0x88);
    void (*apply_tool)(AEntity *, u32);
};
EC FarmerToolSwing const tool_swings[6][7] asm("gUnk_080F1020");
EC FarmerToolTarget const milker_target asm("gUnk_080F1184");
EC FarmerToolTarget const brush_target asm("gUnk_080F118C");
EC FarmerToolTarget const clippers_target asm("gUnk_080F1194");
EC FarmerToolTarget const medicine_target asm("gUnk_080F119C");
EC FarmerToolTarget const cow_potion_target asm("gUnk_080F11A4");
EC FarmerToolTarget const sheep_potion_target asm("gUnk_080F11AC");
EC u32 IsUsingCursedToolRaw(FarmerEntity *) asm("FarmerEntity_IsUsingCursedTool");

#define FIND_FARMER_TOOL_SOUND_CHANNEL(channel, selected, channel_found, owner_register, prepare_target) \
    register FarmerEntity * owner asm(owner_register) = self; \
    asm volatile("" : "+r"(owner)); \
    GameObject * game = owner->game_object; \
    EntitySoundGameVTable const * table = *reinterpret_cast<EntitySoundGameVTable const **>(game); \
    register EntitySoundChannels const * (*get_channels)(GameObject *) asm("r1") = table->get_channels; \
    asm volatile("" : : "r"(get_channels)); \
    EntitySoundChannels const * channels = get_channels(game); \
    channel = channels->begin; \
    register void ** end asm("r5") = channels->end; \
    asm volatile("" : : "r"(channel), "r"(end)); \
    prepare_target \
    if (channel != end) \
    { \
        do \
        { \
            u32 playing = AudioPlayerIsPlayingRaw(*channel) << 24; \
            asm volatile("" : "+r"(playing)); \
            if (!playing) \
                goto channel_found; \
            ++channel; \
        } while (channel != end); \
    } \
    { \
        register void ** previous asm("r0") = end - 1; \
        asm volatile("" : "+r"(previous)); \
        selected = *previous; \
    }

#define PREPARE_FARMER_TOOL_TARGET_SLOT() \
    target_slot_address = &self->interaction_target_slot; \
    asm volatile("" : "+r"(target_slot_address));

#define FIND_FARMER_TOOL_ANIMAL_TARGET() \
    asm volatile("" : "+r"(target_test)); \
    register u32 predicate asm("r1") = target_test->predicate; \
    asm volatile("" : "+r"(predicate)); \
    register u32 context asm("r2") = target_test->context; \
    asm volatile("" : : "r"(context)); \
    SelectFarmerInteractionTarget(self, predicate, context); \
    target_slot = *target_slot_address; \
    asm volatile("" : "+r"(target_slot)); \
    u32 slot_valid = 0; \
    asm volatile("" : "+r"(slot_valid)); \
    if (static_cast<u32>(target_slot - 0x2B) <= 0x1A) \
        slot_valid = 1; \
    asm volatile("" : "+r"(slot_valid));

EC void BeginFarmerToolSwing(FarmerEntity *) ENTITY_CODE(0802C2E0);
EC void BeginFarmerToolSwing(FarmerEntity * farmer)
{
    register FarmerEntity * self asm("r8") = farmer;
    asm volatile("" : "+r"(self));
    register u8 * state_address asm("r1") = &self->state;
    asm volatile("" : "+r"(state_address));
    register u32 swing_state asm("r0") = 9;
    asm volatile("" : : "r"(swing_state));
    *state_address = swing_state;
    register i32 stamina asm("r5") = 0;
    asm volatile("" : "+r"(stamina));
    register u32 tool_state asm("r0") = self->tool_state;
    asm volatile("" : "+r"(tool_state));
    register u8 const * tool_state_address asm("r4");
    register u8 const * tool_level_address asm("r5");
    register FarmerToolSwing const (* swings)[7] asm("r6");
    register FarmerToolTarget const * target_test asm("r0");
    register u8 const * target_slot_address asm("r6");
    register u32 target_slot asm("r3");
    AEntity * target;
    register void ** channel_normal asm("r6");
    register void * selected_normal asm("r2");
    register void ** channel_other asm("r6");
    register void * selected_other asm("r2");
    register void ** channel_milker asm("r4");
    register void * selected_milker asm("r0");
    register void ** channel_brush asm("r4");
    register void * selected_brush asm("r0");
    register void ** channel_tool_8 asm("r4");
    register void * selected_tool_8 asm("r0");
    register void ** channel_clippers asm("r4");
    register void * selected_clippers asm("r0");
    register void ** channel_medicine asm("r4");
    register void * selected_medicine asm("r0");
    register void ** channel_cow_potion asm("r4");
    register void * selected_cow_potion asm("r0");
    register void ** channel_tool_12 asm("r4");
    register void * selected_tool_12 asm("r0");
    register void ** channel_sheep_potion asm("r4");
    register void * selected_sheep_potion asm("r0");
    switch (tool_state)
    {
    case 5:
        SetFarmerThrownItemIcon(self, 0xA2, 1);
        {
            register ThrownItemMotion * motion asm("r2") = &self->thrown_item;
            asm volatile("" : "+r"(motion));
            register i32 zero asm("r4") = 0;
            register i32 lift asm("r0") = 16 * 65536;
            asm volatile("" : : "r"(zero), "r"(lift));
            motion->x_speed = zero;
            motion->y_speed = zero;
            motion->z_speed = lift;
            register u32 facing asm("r3") = self->facing;
            asm volatile("" : "+r"(facing));
            register i32 velocity asm("r0");
            register i32 vertical_velocity asm("r1");
            switch (static_cast<i32>(facing))
            {
            case FACING_LEFT:
                velocity = -0x1B6DB;
                vertical_velocity = 0x28000;
                asm volatile("" : : "r"(velocity), "r"(vertical_velocity));
                motion->x_velocity = velocity;
                motion->y_velocity = zero;
                asm volatile("" : : "m"(*motion));
                goto set_vertical_velocity;
            case FACING_RIGHT:
                velocity = 0x1B6DB;
                vertical_velocity = 0x28000;
                asm volatile("" : : "r"(velocity), "r"(vertical_velocity));
                motion->x_velocity = velocity;
                motion->y_velocity = zero;
                goto set_vertical_velocity;
            case FACING_UP:
                velocity = -0x1B6DB;
                vertical_velocity = 0x28000;
                asm volatile("" : : "r"(velocity), "r"(vertical_velocity));
                motion->x_velocity = zero;
                goto set_y_velocity;
            have_channel_normal:
                selected_normal = *channel_normal;
                goto start_sound_normal;
            case FACING_DOWN:
                velocity = 0x1B6DB;
                vertical_velocity = 0x28000;
                asm volatile("" : : "r"(velocity), "r"(vertical_velocity));
                motion->x_velocity = facing;
            set_y_velocity:
                motion->y_velocity = velocity;
            set_vertical_velocity:
                motion->z_velocity = vertical_velocity;
                break;
            }
        }
    case 0:
    case 1:
    case 3:
    case 4:
        {
            register FarmerToolSwing const (* swing_table)[7] asm("r2") = tool_swings;
            asm volatile("" : "+r"(swing_table));
            tool_level_address = &self->tool_level;
            asm volatile("" : "+r"(tool_level_address));
            tool_state_address = &self->tool_state;
            asm volatile("" : "+r"(tool_state_address));
            register u32 kind_index asm("r1") = *tool_state_address;
            asm volatile("" : "+r"(kind_index));
            register u32 entry_index asm("r0") = kind_index * 7;
            asm volatile("" : "+r"(entry_index));
            register u32 level_index asm("r1") = *tool_level_address;
            asm volatile("" : "+r"(level_index));
            entry_index += level_index;
            asm volatile("" : "+r"(entry_index));
            entry_index *= sizeof(FarmerToolSwing);
            asm volatile("" : "+r"(entry_index));
            register FarmerToolSwing const * entry asm("r0") = reinterpret_cast<FarmerToolSwing const *>(entry_index + reinterpret_cast<u32>(swing_table));
            asm volatile("" : "+r"(entry));
            register u32 animation asm("r1") = entry->animation;
            asm volatile("" : "+r"(animation));
            register u16 * animation_address asm("r0") = &self->unk_44;
            asm volatile("" : : "r"(animation_address));
            *animation_address = animation;
            register FarmerEntity * owner asm("r2") = self;
            asm volatile("" : "+r"(owner));
            GameObject * game = owner->game_object;
            EntitySoundGameVTable const * table = *reinterpret_cast<EntitySoundGameVTable const **>(game);
            register EntitySoundChannels const * (*get_channels)(GameObject *) asm("r1") = table->get_channels;
            asm volatile("" : : "r"(get_channels));
            EntitySoundChannels const * channels = get_channels(game);
            channel_normal = channels->begin;
            void ** end = channels->end;
            asm volatile("" : : "r"(channel_normal), "r"(end));
            if (channel_normal != end)
            {
                do
                {
                    u32 playing = AudioPlayerIsPlayingRaw(*channel_normal) << 24;
                    asm volatile("" : "+r"(playing));
                    if (!playing)
                        goto have_channel_normal;
                    ++channel_normal;
                } while (channel_normal != end);
            }
            {
                register void ** previous asm("r0") = end - 1;
                asm volatile("" : "+r"(previous));
                selected_normal = *previous;
            }
        }
    start_sound_normal:
        {
            asm volatile("" : : "r"(selected_normal));
            {
                swings = tool_swings;
                asm volatile("" : "+r"(swings));
                register u32 kind_index asm("r1") = *tool_state_address;
                asm volatile("" : "+r"(kind_index));
                register u32 entry_index asm("r0") = kind_index * 7;
                asm volatile("" : "+r"(entry_index));
                register u32 level_index asm("r1") = *tool_level_address;
                asm volatile("" : "+r"(level_index));
                entry_index += level_index;
                asm volatile("" : "+r"(entry_index));
                entry_index *= sizeof(FarmerToolSwing);
                asm volatile("" : "+r"(entry_index));
                register FarmerToolSwing const * entry asm("r0") = reinterpret_cast<FarmerToolSwing const *>(entry_index + reinterpret_cast<u32>(swings));
                asm volatile("" : "+r"(entry));
                register u32 song asm("r1") = entry->song;
                asm volatile("" : : "r"(song));
                AudioPlayer_StartSong(selected_normal, song);
                u32 cursed = IsUsingCursedToolRaw(self) << 24;
                asm volatile("" : "+r"(cursed));
                if (!cursed)
                    goto regular_stamina;
                goto cursed_stamina;
            }
        }
    have_channel_other:
        selected_other = *channel_other;
        goto start_sound_other;
    case 2:
        {
            register FarmerToolSwing const (* swing_table)[7] asm("r2") = tool_swings;
            asm volatile("" : "+r"(swing_table));
            tool_level_address = &self->tool_level;
            asm volatile("" : "+r"(tool_level_address));
            tool_state_address = &self->tool_state;
            asm volatile("" : "+r"(tool_state_address));
            register u32 kind_index asm("r1") = *tool_state_address;
            asm volatile("" : "+r"(kind_index));
            register u32 entry_index asm("r0") = kind_index * 7;
            asm volatile("" : "+r"(entry_index));
            register u32 level_index asm("r1") = *tool_level_address;
            asm volatile("" : "+r"(level_index));
            entry_index += level_index;
            asm volatile("" : "+r"(entry_index));
            entry_index *= sizeof(FarmerToolSwing);
            asm volatile("" : "+r"(entry_index));
            register FarmerToolSwing const * entry asm("r0") = reinterpret_cast<FarmerToolSwing const *>(entry_index + reinterpret_cast<u32>(swing_table));
            asm volatile("" : "+r"(entry));
            register u32 animation asm("r1") = entry->animation;
            asm volatile("" : "+r"(animation));
            register u16 * animation_address asm("r0") = &self->unk_44;
            asm volatile("" : : "r"(animation_address));
            *animation_address = animation;
            register FarmerEntity * owner asm("r2") = self;
            asm volatile("" : "+r"(owner));
            GameObject * game = owner->game_object;
            EntitySoundGameVTable const * table = *reinterpret_cast<EntitySoundGameVTable const **>(game);
            register EntitySoundChannels const * (*get_channels)(GameObject *) asm("r1") = table->get_channels;
            asm volatile("" : : "r"(get_channels));
            EntitySoundChannels const * channels = get_channels(game);
            channel_other = channels->begin;
            void ** end = channels->end;
            asm volatile("" : : "r"(channel_other), "r"(end));
            if (channel_other != end)
            {
                do
                {
                    u32 playing = AudioPlayerIsPlayingRaw(*channel_other) << 24;
                    asm volatile("" : "+r"(playing));
                    if (!playing)
                        goto have_channel_other;
                    ++channel_other;
                } while (channel_other != end);
            }
            {
                register void ** previous asm("r0") = end - 1;
                asm volatile("" : "+r"(previous));
                selected_other = *previous;
            }
        }
    start_sound_other:
        {
            asm volatile("" : : "r"(selected_other));
            {
                swings = tool_swings;
                asm volatile("" : "+r"(swings));
                register u32 kind_index asm("r1") = *tool_state_address;
                asm volatile("" : "+r"(kind_index));
                register u32 entry_index asm("r0") = kind_index * 7;
                asm volatile("" : "+r"(entry_index));
                register u32 level_index asm("r1") = *tool_level_address;
                asm volatile("" : "+r"(level_index));
                entry_index += level_index;
                asm volatile("" : "+r"(entry_index));
                entry_index *= sizeof(FarmerToolSwing);
                asm volatile("" : "+r"(entry_index));
                register FarmerToolSwing const * entry asm("r0") = reinterpret_cast<FarmerToolSwing const *>(entry_index + reinterpret_cast<u32>(swings));
                asm volatile("" : "+r"(entry));
                register u32 song asm("r1") = entry->song;
                asm volatile("" : : "r"(song));
                AudioPlayer_StartSong(selected_other, song);
                u32 cursed = IsUsingCursedToolRaw(self) << 24;
                asm volatile("" : "+r"(cursed));
                if (cursed)
                    goto cursed_stamina;
                goto regular_stamina;
            }
        }
    regular_stamina:
        {
            register u32 kind_index asm("r1") = *tool_state_address;
            asm volatile("" : "+r"(kind_index));
            register u32 entry_index asm("r0") = kind_index * 7;
            asm volatile("" : "+r"(entry_index));
            register u32 level_index asm("r5") = *tool_level_address;
            asm volatile("" : "+r"(level_index));
            entry_index += level_index;
            asm volatile("" : "+r"(entry_index));
            entry_index *= sizeof(FarmerToolSwing);
            asm volatile("" : "+r"(entry_index));
            register FarmerToolSwing const * entry asm("r0") = reinterpret_cast<FarmerToolSwing const *>(entry_index + reinterpret_cast<u32>(swings));
            asm volatile("" : "+r"(entry));
            stamina = entry->stamina;
            break;
        }
    cursed_stamina:
        {
            register u32 kind_index asm("r1") = *tool_state_address;
            asm volatile("" : "+r"(kind_index));
            register u32 entry_index asm("r0") = kind_index * 7;
            asm volatile("" : "+r"(entry_index));
            register u32 level_index asm("r5") = *tool_level_address;
            asm volatile("" : "+r"(level_index));
            entry_index += level_index;
            asm volatile("" : "+r"(entry_index));
            entry_index *= sizeof(FarmerToolSwing);
            asm volatile("" : "+r"(entry_index));
            register FarmerToolSwing const * entry asm("r0") = reinterpret_cast<FarmerToolSwing const *>(entry_index + reinterpret_cast<u32>(swings));
            asm volatile("" : "+r"(entry));
            stamina = entry->cursed_stamina;
            break;
        }
    have_channel_milker:
        selected_milker = *channel_milker;
        goto start_sound_milker;
    case 6:
        {
            register u16 * animation_address asm("r1") = &self->unk_44;
            asm volatile("" : "+r"(animation_address));
            register u32 animation asm("r0") = 0x1ae;
            asm volatile("" : : "r"(animation));
            *animation_address = animation;
            FIND_FARMER_TOOL_SOUND_CHANNEL(channel_milker, selected_milker, have_channel_milker, "r2",
                PREPARE_FARMER_TOOL_TARGET_SLOT())
        }
    start_sound_milker:
        {
            asm volatile("" : : "r"(selected_milker));
            {
                AudioPlayer_StartSong(selected_milker, 0x9d);
                target_test = &milker_target;
            find_animal_target:
                {
                    FIND_FARMER_TOOL_ANIMAL_TARGET()
                    if (slot_valid)
                        goto get_target;
                    goto no_target;
                }
            }
        }
    have_channel_brush:
        selected_brush = *channel_brush;
        goto start_sound_brush;
    case 7:
        {
            register u16 * animation_address asm("r1") = &self->unk_44;
            asm volatile("" : "+r"(animation_address));
            register u32 animation asm("r0") = 0x1be;
            asm volatile("" : : "r"(animation));
            *animation_address = animation;
            FIND_FARMER_TOOL_SOUND_CHANNEL(channel_brush, selected_brush, have_channel_brush, "r2",
                PREPARE_FARMER_TOOL_TARGET_SLOT())
        }
    start_sound_brush:
        {
            asm volatile("" : : "r"(selected_brush));
            {
                AudioPlayer_StartSong(selected_brush, 0x9d);
                target_test = &brush_target;
                goto find_animal_target;
            }
        }
    have_channel_tool_8:
        selected_tool_8 = *channel_tool_8;
        goto start_sound_tool_8;
    case 8:
        {
            register u16 * animation_address asm("r1") = &self->unk_44;
            asm volatile("" : "+r"(animation_address));
            register u32 animation asm("r0") = 0x1ba;
            asm volatile("" : : "r"(animation));
            *animation_address = animation;
            FIND_FARMER_TOOL_SOUND_CHANNEL(channel_tool_8, selected_tool_8, have_channel_tool_8, "r2",
                )
        }
    start_sound_tool_8:
        {
            asm volatile("" : : "r"(selected_tool_8));
            {
                AudioPlayer_StartSong(selected_tool_8, 0x94);
            lose_two_stamina:
                stamina = -2;
                break;
            }
        }
    have_channel_clippers:
        selected_clippers = *channel_clippers;
        goto start_sound_clippers;
    case 9:
        {
            register u16 * animation_address asm("r1") = &self->unk_44;
            asm volatile("" : "+r"(animation_address));
            register u32 animation asm("r0") = 0x1aa;
            asm volatile("" : : "r"(animation));
            *animation_address = animation;
            FIND_FARMER_TOOL_SOUND_CHANNEL(channel_clippers, selected_clippers, have_channel_clippers, "r1",
                PREPARE_FARMER_TOOL_TARGET_SLOT())
        }
    start_sound_clippers:
        {
            asm volatile("" : : "r"(selected_clippers));
            {
                AudioPlayer_StartSong(selected_clippers, 0x93);
                target_test = &clippers_target;
                FIND_FARMER_TOOL_ANIMAL_TARGET()
                if (!slot_valid)
                    goto no_clippers_target;
                {
                    register FarmerEntity * owner asm("r1") = self;
                    asm volatile("" : "+r"(owner));
                    GameObject * game = owner->game_object;
                    FarmerInteractionGameVTable const * table = *reinterpret_cast<FarmerInteractionGameVTable const **>(game);
                    register AEntity * (*get_entity)(GameObject *, u32) asm("r2") = table->get_entity;
                    asm volatile("" : : "r"(get_entity));
                    target = get_entity(game, target_slot);
                }
                goto apply_clippers;
            no_clippers_target:
                target = nullptr;
            apply_clippers:
                asm volatile("" : "+r"(target));
                if (target)
                {
                    FarmerToolTargetActions const * actions = reinterpret_cast<FarmerToolTargetActions const **>(target)[5];
                    register void (*apply_tool)(AEntity *, u32) asm("r2") = actions->apply_tool;
                    asm volatile("" : : "r"(apply_tool));
                    apply_tool(target, 0x3C);
                }
                goto lose_two_stamina;
            }
        }
    have_channel_medicine:
        selected_medicine = *channel_medicine;
        goto start_sound_medicine;
    case 10:
        {
            register u16 * animation_address asm("r1") = &self->unk_44;
            asm volatile("" : "+r"(animation_address));
            register u32 animation asm("r0") = 0x1b6;
            asm volatile("" : : "r"(animation));
            *animation_address = animation;
            FIND_FARMER_TOOL_SOUND_CHANNEL(channel_medicine, selected_medicine, have_channel_medicine, "r2",
                PREPARE_FARMER_TOOL_TARGET_SLOT())
        }
    start_sound_medicine:
        {
            asm volatile("" : : "r"(selected_medicine));
            {
                AudioPlayer_StartSong(selected_medicine, 0x9f);
                target_test = &medicine_target;
                goto find_animal_target;
            }
        }
    have_channel_cow_potion:
        selected_cow_potion = *channel_cow_potion;
        goto start_sound_cow_potion;
    case 11:
        {
            register u16 * animation_address asm("r1") = &self->unk_44;
            asm volatile("" : "+r"(animation_address));
            register u32 animation asm("r0") = 0x1b2;
            asm volatile("" : : "r"(animation));
            *animation_address = animation;
            FIND_FARMER_TOOL_SOUND_CHANNEL(channel_cow_potion, selected_cow_potion, have_channel_cow_potion, "r2",
                PREPARE_FARMER_TOOL_TARGET_SLOT())
        }
    start_sound_cow_potion:
        {
            asm volatile("" : : "r"(selected_cow_potion));
            {
                AudioPlayer_StartSong(selected_cow_potion, 0xa0);
                target_test = &cow_potion_target;
                FIND_FARMER_TOOL_ANIMAL_TARGET()
                if (!slot_valid)
                    goto no_cow_target;
                {
                    register FarmerEntity * owner asm("r1") = self;
                    asm volatile("" : "+r"(owner));
                    GameObject * game = owner->game_object;
                    FarmerInteractionGameVTable const * table = *reinterpret_cast<FarmerInteractionGameVTable const **>(game);
                    register AEntity * (*get_entity)(GameObject *, u32) asm("r2") = table->get_entity;
                    asm volatile("" : : "r"(get_entity));
                    target = get_entity(game, target_slot);
                }
                goto apply_cow_potion;
            no_cow_target:
                target = nullptr;
            apply_cow_potion:
                asm volatile("" : "+r"(target));
                if (target)
                {
                    FarmerToolTargetActions const * actions = reinterpret_cast<FarmerToolTargetActions const **>(target)[5];
                    register void (*apply_tool)(AEntity *, u32) asm("r2") = actions->apply_tool;
                    asm volatile("" : : "r"(apply_tool));
                    apply_tool(target, 0x3C);
                }
                stamina = -8;
                break;
            }
        }
    have_channel_tool_12:
        selected_tool_12 = *channel_tool_12;
        goto start_sound_tool_12;
    case 12:
        {
            register u16 * animation_address asm("r1") = &self->unk_44;
            asm volatile("" : "+r"(animation_address));
            register u32 animation asm("r0") = 0x1a6;
            asm volatile("" : : "r"(animation));
            *animation_address = animation;
            FIND_FARMER_TOOL_SOUND_CHANNEL(channel_tool_12, selected_tool_12, have_channel_tool_12, "r2",
                )
        }
    start_sound_tool_12:
        {
            asm volatile("" : : "r"(selected_tool_12));
            {
                AudioPlayer_StartSong(selected_tool_12, 0x9e);
                goto lose_four_stamina;
            }
        }
    have_channel_sheep_potion:
        selected_sheep_potion = *channel_sheep_potion;
        goto start_sound_sheep_potion;
    case 13:
        {
            register u16 * animation_address asm("r1") = &self->unk_44;
            asm volatile("" : "+r"(animation_address));
            register u32 animation asm("r0") = 0x1a2;
            asm volatile("" : : "r"(animation));
            *animation_address = animation;
            FIND_FARMER_TOOL_SOUND_CHANNEL(channel_sheep_potion, selected_sheep_potion, have_channel_sheep_potion, "r1",
                PREPARE_FARMER_TOOL_TARGET_SLOT())
        }
    start_sound_sheep_potion:
        {
            asm volatile("" : : "r"(selected_sheep_potion));
            {
                AudioPlayer_StartSong(selected_sheep_potion, 0x9d);
                target_test = &sheep_potion_target;
                FIND_FARMER_TOOL_ANIMAL_TARGET()
                if (!slot_valid)
                    goto no_target;
            }
        }
    get_target:
        {
            register FarmerEntity * owner asm("r1") = self;
            asm volatile("" : "+r"(owner));
            GameObject * game = owner->game_object;
            FarmerInteractionGameVTable const * table = *reinterpret_cast<FarmerInteractionGameVTable const **>(game);
            register AEntity * (*get_entity)(GameObject *, u32) asm("r2") = table->get_entity;
            asm volatile("" : : "r"(get_entity));
            target = get_entity(game, target_slot);
        }
        goto apply_tool;
    no_target:
        target = nullptr;
    apply_tool:
        asm volatile("" : "+r"(target));
        if (target)
        {
            FarmerToolTargetActions const * actions = reinterpret_cast<FarmerToolTargetActions const **>(target)[5];
            register void (*apply_tool)(AEntity *, u32) asm("r2") = actions->apply_tool;
            asm volatile("" : : "r"(apply_tool));
            apply_tool(target, 0x3C);
        }
    lose_four_stamina:
        stamina = -4;
        break;
    }
    register FarmerEntity * owner asm("r2") = self;
    asm volatile("" : "+r"(owner));
    register FarmerActionVTable const * actions asm("r4") = reinterpret_cast<FarmerActionVTable const **>(owner)[5];
    asm volatile("" : "+r"(actions));
    register u32 fatigue asm("r2") = FarmerEntity_GetToolFatiguePenalty(self);
    asm volatile("" : : "r"(fatigue));
    register bool (*adjust)(FarmerEntity *, i32, i32) asm("r3") = actions->change_stamina_and_fatigue;
    asm volatile("" : : "r"(adjust));
    adjust(self, stamina, fatigue);
}
#undef FIND_FARMER_TOOL_ANIMAL_TARGET
#undef PREPARE_FARMER_TOOL_TARGET_SLOT
#undef FIND_FARMER_TOOL_SOUND_CHANNEL

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

EC u32 IsFarmerToolStackEmptyRaw(ToolStack const *) asm("IsEmpty__C9ToolStack");
EC u32 GetFarmerToolIdRaw(Tool const *) asm("GetId__C4Tool");
struct FarmerToolTerrainGameVTable
{
    STRUCT_PAD(0, 0x34);
    void (*get_grid)(FarmerTerrainGrid *, GameObject *, u32);
};
struct FarmerToolSetupFrame
{
    FarmerTerrainGrid grid;
    u8 tool_for_use;
    u8 tool_for_charge_limit;
};
#define SET_FARMER_TOOL_VALUE(field, value, destination) \
    output = &self->field; \
    asm volatile("" : "+r"(output)); \
    setting = value; \
    asm volatile("" : : "r"(setting)); \
    goto destination

EC void PrepareFarmerToolUse(FarmerEntity *) ENTITY_CODE(0802C984);
EC void PrepareFarmerToolUse(FarmerEntity * farmer)
{
    register FarmerEntity * self asm("r6") = farmer;
    FarmerToolSetupFrame frame;
    register u8 * charge_counter asm("r0") = &farmer->tool_level;
    asm volatile("" : "+r"(charge_counter));
    register u32 zero asm("r1") = 0;
    asm volatile("" : : "r"(zero));
    *charge_counter = zero;
    charge_counter += 2;
    asm volatile("" : "+r"(charge_counter));
    *charge_counter = zero;
    register u8 * output asm("r1");
    register u32 setting asm("r0");
    register void ** channel asm("r4");
    register void * player asm("r0");
    if (IsFarmerToolStackEmptyRaw(&self->farmer->unk_5C) << 24)
    {
        SET_FARMER_TOOL_VALUE(tool_state, 15, write_value);
    }
    {
        Farmer const * model = self->farmer;
        register u32 resolved asm("r1") = ReadResolvedToolIdRaw(*model, model->unk_5C.GetTool());
        asm volatile("" : "+r"(resolved));
        register u8 * id_address asm("r0") = &frame.tool_for_use;
        asm volatile("" : "+r"(id_address));
        *id_address = resolved;
        register u32 id asm("r0") = GetFarmerToolIdRaw(reinterpret_cast<Tool const *>(id_address));
        asm volatile("" : "+r"(id));
        switch (id)
        {
        case TOOL_IRON_SICKLE:
        case TOOL_COPPER_SICKLE:
        case TOOL_SILVER_SICKLE:
        case TOOL_GOLD_SICKLE:
        case TOOL_MYSTRILE_SICKLE:
        case TOOL_CURSED_SICKLE:
        case TOOL_BLESSED_SICKLE:
        case TOOL_MYTHIC_SICKLE:
            SET_FARMER_TOOL_VALUE(tool_state, 0, set_tool_state);
        case TOOL_IRON_HOE:
        case TOOL_COPPER_HOE:
        case TOOL_SILVER_HOE:
        case TOOL_GOLD_HOE:
        case TOOL_MYSTRILE_HOE:
        case TOOL_CURSED_HOE:
        case TOOL_BLESSED_HOE:
        case TOOL_MYTHIC_HOE:
            SET_FARMER_TOOL_VALUE(tool_state, 3, set_tool_state);
        case TOOL_IRON_AXE:
        case TOOL_COPPER_AXE:
        case TOOL_SILVER_AXE:
        case TOOL_GOLD_AXE:
        case TOOL_MYSTRILE_AXE:
        case TOOL_CURSED_AXE:
        case TOOL_BLESSED_AXE:
        case TOOL_MYTHIC_AXE:
            SET_FARMER_TOOL_VALUE(tool_state, 1, set_tool_state);
        case TOOL_IRON_HAMMER:
        case TOOL_COPPER_HAMMER:
        case TOOL_SILVER_HAMMER:
        case TOOL_GOLD_HAMMER:
        case TOOL_MYSTRILE_HAMMER:
        case TOOL_CURSED_HAMMER:
        case TOOL_BLESSED_HAMMER:
        case TOOL_MYTHIC_HAMMER:
            SET_FARMER_TOOL_VALUE(tool_state, 4, set_tool_state);
        case TOOL_WATERING_CAN:
        case TOOL_COPPER_WATERING_CAN:
        case TOOL_SILVER_WATERING_CAN:
        case TOOL_GOLD_WATERING_CAN:
        case TOOL_MYSTRILE_WATERING_CAN:
        case TOOL_CURSED_WATERING_CAN:
        case TOOL_BLESSED_WATERING_CAN:
        case TOOL_MYTHIC_WATERING_CAN:
            {
                register i32 x asm("r5") = reinterpret_cast<i16 const *>(&self->x_q16)[1];
                asm volatile("" : "+r"(x));
                register i32 y asm("r4") = reinterpret_cast<i16 const *>(&self->y_q16)[1];
                asm volatile("" : "+r"(y));
                u32 facing = self->facing;
                asm volatile("" : "+r"(facing));
                register u32 facing_copy asm("r1") = facing;
                asm volatile("" : "+r"(facing));
                if (facing == FACING_UP)
                    goto face_up;
                if (static_cast<i32>(facing) > FACING_UP)
                    goto face_horizontal;
                if (facing == FACING_DOWN)
                    goto face_down;
                goto tile_ahead;
            face_horizontal:
                if (facing_copy == FACING_LEFT)
                    goto face_left;
                if (facing_copy == FACING_RIGHT)
                    goto face_right;
                goto tile_ahead;
            face_up:
                y -= 16;
                goto tile_ahead;
            face_down:
                y += 16;
                goto tile_ahead;
            face_left:
                x -= 16;
                goto tile_ahead;
            face_right:
                x += 16;
            tile_ahead:
                register u32 map asm("r2") = self->location_map;
                asm volatile("" : "+r"(map));
                GameObject * game = self->game_object;
                asm volatile("" : "+r"(game));
                register FarmerToolTerrainGameVTable const * vtable asm("r1") = *reinterpret_cast<FarmerToolTerrainGameVTable const **>(game);
                asm volatile("" : "+r"(vtable));
                register FarmerTerrainGrid * result asm("r0") = &frame.grid;
                asm volatile("" : "+r"(result));
                register void (*get_grid)(FarmerTerrainGrid *, GameObject *, u32) asm("r3") = vtable->get_grid;
                asm volatile("" : : "r"(get_grid));
                get_grid(result, game, map);
                register FarmerTerrainGrid const * grid asm("r1") = &frame.grid;
                asm volatile("" : "+r"(grid));
                x >>= 3;
                asm volatile("" : "+r"(x));
                register i32 row asm("r3") = y >> 3;
                asm volatile("" : "+r"(row));
                register u32 const * properties asm("r2") = frame.grid.properties;
                asm volatile("" : "+r"(properties));
                register u32 width asm("r0") = grid->width;
                asm volatile("" : "+r"(width));
                u32 const * tile;
                if (!properties || static_cast<u32>(x) > width || static_cast<u32>(row) > grid->height)
                    goto no_tile;
                {
                    register u8 const * indices asm("r1") = frame.grid.indices;
                    asm volatile("" : "+r"(indices));
                    register u32 index asm("r0") = width * row + x;
                    asm volatile("" : "+r"(index));
                    if (indices)
                        index = indices[index];
                    tile = properties + index;
                    goto check_tile;
                }
            unused_channel:
                player = *channel;
                goto play_sound;
            no_tile:
                tile = nullptr;
            check_tile:
                asm volatile("" : "+r"(tile));
                if (tile && static_cast<i32>(*tile << 30) < 0)
                {
                    {
                        register u8 * state asm("r1") = &self->state;
                        asm volatile("" : "+r"(state));
                        register u32 state_value asm("r0") = 27;
                        asm volatile("" : : "r"(state_value));
                        *state = state_value;
                        state += 3;
                        asm volatile("" : "+r"(state));
                        register u32 none asm("r0") = 15;
                        asm volatile("" : : "r"(none));
                        *state = none;
                        register FarmerActionVTable const * actions asm("r4") = reinterpret_cast<FarmerActionVTable const **>(self)[5];
                        asm volatile("" : "+r"(actions));
                        register i32 stamina asm("r5") = -2;
                        asm volatile("" : "+r"(stamina));
                        register u32 fatigue asm("r2") = FarmerEntity_GetToolFatiguePenalty(self);
                        asm volatile("" : : "r"(fatigue));
                        register bool (*adjust)(FarmerEntity *, i32, i32) asm("r3") = actions->change_stamina_and_fatigue;
                        asm volatile("" : : "r"(adjust));
                        adjust(self, stamina, fatigue);

                        EntitySoundGameVTable const * sound_table = *reinterpret_cast<EntitySoundGameVTable const **>(game);
                        register EntitySoundChannels const * (*get_channels)(GameObject *) asm("r1") = sound_table->get_channels;
                        asm volatile("" : : "r"(get_channels));
                        EntitySoundChannels const * channels = get_channels(game);
                        channel = channels->begin;
                        register void ** end asm("r5") = channels->end;
                        asm volatile("" : : "r"(channel), "r"(end));
                        if (channel != end)
                        {
                            do
                            {
                                u32 playing = AudioPlayerIsPlayingRaw(*channel) << 24;
                                asm volatile("" : "+r"(playing));
                                if (!playing)
                                    goto unused_channel;
                                ++channel;
                            } while (channel != end);
                        }
                        {
                            register void ** previous asm("r0") = end - 1;
                            asm volatile("" : "+r"(previous));
                            player = *previous;
                        }
                    }
                play_sound:
                    asm volatile("" : : "r"(player));
                    AudioPlayer_StartSong(player, 0x92);
                    return;
                }
                if (func_0800E94C(*self->farmer))
                {
                    SET_FARMER_TOOL_VALUE(tool_state, 2, set_tool_state);
                }
                SET_FARMER_TOOL_VALUE(tool_state, 15, write_value);
            }
        case TOOL_FISHING_ROD:
        case TOOL_COPPER_FISHING_ROD:
        case TOOL_SILVER_FISHING_ROD:
        case TOOL_GOLD_FISHING_ROD:
        case TOOL_MYSTRILE_FISHING_ROD:
        case TOOL_CURSED_FISHING_ROD:
        case TOOL_BLESSED_FISHING_ROD:
        case TOOL_MYTHIC_FISHING_ROD:
            SET_FARMER_TOOL_VALUE(tool_state, 5, set_tool_state);
        case TOOL_COW_MIRACLE_POTION:
            SET_FARMER_TOOL_VALUE(tool_state, 6, set_tool_state);
        case TOOL_SHEEP_MIRACLE_POTION:
            SET_FARMER_TOOL_VALUE(tool_state, 7, set_tool_state);
        case TOOL_TURNIP_SEEDS:
        case TOOL_POTATO_SEEDS:
        case TOOL_CUCUMBER_SEEDS:
        case TOOL_STRAWBERRY_SEEDS:
        case TOOL_CABBAGE_SEEDS:
        case TOOL_TOMATO_SEEDS:
        case TOOL_CORN_SEEDS:
        case TOOL_ONION_SEEDS:
        case TOOL_PUMPKIN_SEEDS:
        case TOOL_PINEAPPLE_SEEDS:
        case TOOL_EGGPLANT_SEEDS:
        case TOOL_CARROT_SEEDS:
        case TOOL_SWEET_POTATO_SEEDS:
        case TOOL_SPINACH_SEEDS:
        case TOOL_GREEN_PEPPER_SEEDS:
        case TOOL_GRASS_SEEDS:
        case TOOL_MOON_DROP_SEEDS:
        case TOOL_PINK_CAT_SEEDS:
        case TOOL_MAGIC_SEEDS:
        case TOOL_TOY_FLOWER_SEEDS:
            SET_FARMER_TOOL_VALUE(tool_state, 8, set_tool_state);
        case TOOL_BRUSH:
            SET_FARMER_TOOL_VALUE(tool_state, 9, set_tool_state);
        case TOOL_MILKER:
            SET_FARMER_TOOL_VALUE(tool_state, 10, set_tool_state);
        case TOOL_CLIPPER:
            SET_FARMER_TOOL_VALUE(tool_state, 11, set_tool_state);
        case TOOL_BELL:
            SET_FARMER_TOOL_VALUE(tool_state, 12, set_tool_state);
        case TOOL_ANIMAL_MEDICINE:
            SET_FARMER_TOOL_VALUE(tool_state, 13, set_tool_state);
        case TOOL_BLUE_FEATHER:
        case TOOL_PEDOMETER:
        case TOOL_TELEPORT_STONE:
        case TOOL_GEM_OF_THE_GODDESS:
        case TOOL_GEM_OF_THE_KAPPA:
        case TOOL_GEM_OF_TRUTH:
            SET_FARMER_TOOL_VALUE(tool_state, 14, set_tool_state);
        default:
            SET_FARMER_TOOL_VALUE(tool_state, 15, set_tool_state);
        }
    }
set_tool_state:
    *output = setting;
    {
        Farmer const * model = self->farmer;
        register u32 resolved asm("r1") = ReadResolvedToolIdRaw(*model, model->unk_5C.GetTool());
        asm volatile("" : "+r"(resolved));
        register u8 * id_address asm("r0") = &frame.tool_for_charge_limit;
        asm volatile("" : "+r"(id_address));
        *id_address = resolved;
        register u32 id asm("r0") = GetFarmerToolIdRaw(reinterpret_cast<Tool const *>(id_address));
        asm volatile("" : "+r"(id));
        switch (id)
        {
        case TOOL_IRON_SICKLE:
        case TOOL_IRON_HOE:
        case TOOL_IRON_AXE:
        case TOOL_IRON_HAMMER:
        case TOOL_WATERING_CAN:
        case TOOL_FISHING_ROD:
            SET_FARMER_TOOL_VALUE(tool_charge_limit, 1, write_value);
        case TOOL_COPPER_SICKLE:
        case TOOL_COPPER_HOE:
        case TOOL_COPPER_AXE:
        case TOOL_COPPER_HAMMER:
        case TOOL_COPPER_WATERING_CAN:
        case TOOL_COPPER_FISHING_ROD:
            SET_FARMER_TOOL_VALUE(tool_charge_limit, 2, write_value);
        case TOOL_SILVER_SICKLE:
        case TOOL_SILVER_HOE:
        case TOOL_SILVER_AXE:
        case TOOL_SILVER_HAMMER:
        case TOOL_SILVER_WATERING_CAN:
        case TOOL_SILVER_FISHING_ROD:
            SET_FARMER_TOOL_VALUE(tool_charge_limit, 3, write_value);
        case TOOL_GOLD_SICKLE:
        case TOOL_GOLD_HOE:
        case TOOL_GOLD_AXE:
        case TOOL_GOLD_HAMMER:
        case TOOL_GOLD_WATERING_CAN:
        case TOOL_GOLD_FISHING_ROD:
            SET_FARMER_TOOL_VALUE(tool_charge_limit, 4, write_value);
        case TOOL_MYSTRILE_SICKLE:
        case TOOL_MYSTRILE_HOE:
        case TOOL_MYSTRILE_AXE:
        case TOOL_MYSTRILE_HAMMER:
        case TOOL_MYSTRILE_WATERING_CAN:
        case TOOL_MYSTRILE_FISHING_ROD:
            SET_FARMER_TOOL_VALUE(tool_charge_limit, 5, write_value);
        case TOOL_CURSED_SICKLE:
        case TOOL_BLESSED_SICKLE:
        case TOOL_CURSED_HOE:
        case TOOL_BLESSED_HOE:
        case TOOL_CURSED_AXE:
        case TOOL_BLESSED_AXE:
        case TOOL_CURSED_HAMMER:
        case TOOL_BLESSED_HAMMER:
        case TOOL_CURSED_WATERING_CAN:
        case TOOL_BLESSED_WATERING_CAN:
        case TOOL_CURSED_FISHING_ROD:
        case TOOL_BLESSED_FISHING_ROD:
            SET_FARMER_TOOL_VALUE(tool_charge_limit, 6, write_value);
        case TOOL_MYTHIC_SICKLE:
        case TOOL_MYTHIC_HOE:
        case TOOL_MYTHIC_AXE:
        case TOOL_MYTHIC_HAMMER:
        case TOOL_MYTHIC_WATERING_CAN:
        case TOOL_MYTHIC_FISHING_ROD:
            SET_FARMER_TOOL_VALUE(tool_charge_limit, 7, write_value);
        default:
            SET_FARMER_TOOL_VALUE(tool_charge_limit, 0, write_value);
        }
    }
write_value:
    *output = setting;
}

#undef SET_FARMER_TOOL_VALUE

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

union FarmerHeldItemKeyWord
{
    u32 word;
    u16 halfwords[2];
    u8 bytes[4];
};

struct FarmerMovementFrame
{
    Location where;
    u16 where_padding;
    FarmerTerrainGrid terrain;
    FarmerEventRequest request;
    FarmerHeldItemKeyWord held;
};

struct FarmerTerrainReturnVTable
{
    STRUCT_PAD(0, 0x34);
    void (*get_grid)(FarmerTerrainGrid *, GameObject *, u32);
};

EC void UpdateFarmerMovementInput(FarmerEntity *, FarmerInputState const *) ENTITY_CODE(0802D59C);
EC void UpdateFarmerMovementInput(FarmerEntity * farmer, FarmerInputState const * input)
{
    register FarmerEntity * self asm("r6") = farmer;
    asm volatile("" : : "r"(self));
    FarmerHeldItemKeyWord held_storage;
    register FarmerMovementFrame * stack asm("sp");
    asm volatile("" : "=r"(stack));
    register u32 pressed asm("r8");
    register u32 clear asm("r2") = 0;
    asm volatile("" : "+r"(clear));
    stack->held.word = clear;
    pressed = clear;
    if (input)
    {
        stack->held.word = input->held;
        pressed = input->pressed;
    }
    asm volatile("" : "=m"(held_storage));
    register u8 * motion_field asm("r0") = &self->motion;
    asm volatile("" : : "r"(motion_field));
    *motion_field = clear;
    register u8 * exhausted_field asm("r1") = &self->exhaustion_state;
    register u32 exhausted_now asm("r2") = *exhausted_field;
    asm volatile("" : : "r"(exhausted_now));
    u8 * motion = motion_field;
    asm volatile("" : : "r"(motion), "r"(exhausted_now));
    asm volatile("" : : "r"(motion));
    register u8 * exhausted asm("sl") = exhausted_field;
    asm volatile("" : : "r"(exhausted));
    register u8 * facing asm("sb") = &self->facing;
    if (exhausted_now)
        goto skip_tile;
    {
        register u32 key_a asm("r0") = INPUT_A;
        register u32 pressed_now asm("r1") = pressed;
        asm volatile("" : : "r"(key_a), "r"(pressed_now));
        u32 pressed_a = key_a & pressed_now;
        asm volatile("" : : "r"(pressed_a));
        asm volatile("" : "+r"(pressed_a) : : "cc");
        if (!pressed_a)
            goto skip_tile;
    }
    {
        Location where = self->AEntity::GetLocation();
        register u8 const * face asm("r2") = facing;
        asm volatile("" : : "r"(face));
        i32 direction = *face;
        asm volatile("" : "+r"(direction) : "r"(face));
        register u8 * packed_y_destination asm("r3");
        register u32 low asm("r4");
        register u32 mask asm("r5");
        register u32 y_shifted asm("r1");
        switch (direction)
        {
        case FACING_UP:
            {
                packed_y_destination = reinterpret_cast<u8 *>(&where);
                asm volatile("" : : "r"(packed_y_destination));
                register u8 const * source asm("r0") = reinterpret_cast<u8 const *>(&where);
                asm volatile("" : "+r"(source));
                low = source[3];
                register u32 y_low asm("r2") = low >> 2;
                asm volatile("" : : "r"(y_low));
                register u32 y_high asm("r1") = *reinterpret_cast<u16 const *>(source + 4);
                mask = 0x3FF;
                asm volatile("" : "+r"(mask) : "r"(y_high));
                y_shifted = (((y_high & mask) << 6) | y_low) << 16;
                asm volatile("" : "+r"(y_shifted));
                register u32 decrease asm("r0") = 0xFFF00000;
                asm volatile("" : "+r"(decrease));
                y_shifted += decrease;
                goto set_y;
            }
        case FACING_DOWN:
            {
                packed_y_destination = reinterpret_cast<u8 *>(&where);
                asm volatile("" : : "r"(packed_y_destination));
                register u8 const * source asm("r0") = reinterpret_cast<u8 const *>(&where);
                asm volatile("" : "+r"(source));
                low = source[3];
                register u32 y_low asm("r2") = low >> 2;
                asm volatile("" : : "r"(y_low));
                register u32 y_high asm("r1") = *reinterpret_cast<u16 const *>(source + 4);
                mask = 0x3FF;
                asm volatile("" : "+r"(mask) : "r"(y_high));
                y_shifted = (((y_high & mask) << 6) | y_low) << 16;
                asm volatile("" : "+r"(y_shifted));
                register u32 increase asm("r2") = 16 << 16;
                asm volatile("" : "+r"(increase));
                y_shifted += increase;
                asm volatile("" : "+r"(y_shifted));
            }
        set_y:
            {
                register u32 low_y asm("r2") = ((y_shifted >> 16) & 0x3F) << 2;
                asm volatile("" : "+r"(low_y));
                register u32 low_x asm("r0") = low & 3;
                asm volatile("" : "+r"(low_x));
                low_x |= low_y;
                asm volatile("" : "+r"(low_x));
                packed_y_destination[3] = low_x;
                register u32 high_y asm("r1") = (y_shifted >> 22) & mask;
                asm volatile("" : "+r"(high_y));
                register u16 * high asm("r3") = reinterpret_cast<u16 *>(packed_y_destination);
                register u32 high_bits asm("r2") = high[2];
                asm volatile("" : "+r"(high_bits));
                register u32 high_mask asm("r0") = 0xFFFFFC00;
                asm volatile("" : "+r"(high_mask));
                high_mask = (high_bits & high_mask) | high_y;
                asm volatile("" : "+r"(high_mask));
                high[2] = high_mask;
            }
            break;
        case FACING_LEFT:
            where.x -= 16;
            break;
        case FACING_RIGHT:
            where.x += 16;
            break;
        }
        register u32 map asm("r2") = self->location_map;
        asm volatile("" : : "r"(map));
        register GameObject * game asm("r1") = self->game_object;
        asm volatile("" : : "r"(game));
        FarmerTerrainGrid terrain;
        register FarmerTerrainGrid * grid asm("r4") = &terrain;
        asm volatile("" : : "r"(grid), "r"(game), "r"(map));
        register FarmerTerrainReturnVTable const * table asm("r3") = *reinterpret_cast<FarmerTerrainReturnVTable const **>(game);
        asm volatile("" : "+r"(table));
        register FarmerTerrainGrid * grid_argument asm("r0") = grid;
        asm volatile("" : "+r"(grid_argument) : "r"(table));
        register void (*reader)(FarmerTerrainGrid *, GameObject *, u32) asm("r3") = table->get_grid;
        asm volatile("" : : "r"(reader));
        reader(grid_argument, game, map);
        register i32 packed_x asm("r0") = *reinterpret_cast<u32 const *>(&where) << 6;
        asm volatile("" : "+r"(packed_x));
        register u32 x asm("r5") = packed_x >> 19;
        asm volatile("" : "+r"(x));
        u32 y = where.y >> 3;
        asm volatile("" : "+r"(y));
        register u32 const * property asm("r2") = terrain.properties;
        register u32 width asm("r0") = grid->width;
        asm volatile("" : "+r"(property), "+r"(width) : "r"(y));
        if (property && x <= width)
        {
            register u32 height asm("r1") = grid->height;
            asm volatile("" : : "r"(height), "r"(y));
            if (y <= height)
            {
                register u8 const * indices asm("r1") = grid->indices;
                asm volatile("" : : "r"(indices));
                u32 index = y * width + x;
                if (indices)
                {
                    register u8 const * tile_index asm("r0") = indices + index;
                    asm volatile("" : "+r"(tile_index));
                    index = *tile_index;
                }
                register u32 const * selected asm("r0") = property + index;
                asm volatile("" : "+r"(selected));
                property = selected;
                goto have_property;
            }
        }
        property = nullptr;
    have_property:
        asm volatile("" : "+r"(property));
        if (property && (reinterpret_cast<u16 const *>(property)[1] & 0xFFFE))
        {
            GameObject * current_game = self->game_object;
            FarmerEventDispatch const * dispatch = &(*reinterpret_cast<FarmerEventGameVTable const **>(current_game))->dispatch_event;
            register u32 event_id asm("r2") = *property >> 17;
            FarmerEventRequest request_storage;
            asm volatile("" : "=m"(request_storage));
            asm volatile("" : "+r"(event_id));
            register FarmerEventRequest * request_ptr asm("r1") = &stack->request;
            asm volatile("" : "+r"(request_ptr) : "r"(event_id));
            register u32 zero asm("r4") = 0;
            asm volatile("" : : "r"(zero));
            stack->request.id = event_id;
            request_ptr->argument = zero;
            FarmerEventDispatch function = *dispatch;
            function(current_game, request_ptr, 0);
            return;
        }
    }
skip_tile:
    register u32 speed_x asm("r3") = 0;
    register u32 speed_y asm("r5") = 0;
    register u8 const * facing_address asm("r2") = facing;
    asm volatile("" : : "r"(facing_address), "r"(speed_x), "r"(speed_y));
    register u32 new_facing asm("r4") = *facing_address;
    register u8 const * exhaustion_address asm("r1") = exhausted;
    asm volatile("" : : "r"(new_facing), "r"(exhaustion_address));
    if (*exhaustion_address)
        goto stop;
    {
        register u32 mask asm("r0") = INPUT_A | INPUT_B;
        register u32 action asm("r2") = pressed;
        asm volatile("" : : "r"(mask), "r"(action));
        action &= mask;
        asm volatile("" : "+r"(action));
        if (!action)
            goto read_direction;
    }
stop:
    self->state = 0x23;
    goto set_speed;
read_direction:
    {
        register u32 keys asm("r1") = stack->held.word;
        asm volatile("" : "+r"(keys));
        register u32 up asm("r2") = (keys << 25) >> 31;
        register u32 down asm("r0") = (keys << 24) >> 31;
        asm volatile("" : "+r"(up), "+r"(down));
        u32 move_keys = up | down | ((keys << 26) >> 31) | ((keys << 27) >> 31) | ((keys << 23) >> 31);
        if (move_keys)
        {
            register volatile u8 * held_bytes asm("r1") = stack->held.bytes;
            register u32 directions asm("r2") = held_bytes[0];
            asm volatile("" : : "r"(directions), "r"(held_bytes));
            if (directions & INPUT_LEFT)
                new_facing = FACING_LEFT;
            else if (directions & INPUT_RIGHT)
                new_facing = FACING_RIGHT;
            else if (directions & INPUT_UP)
                new_facing = FACING_UP;
            else if (directions & INPUT_DOWN)
                new_facing = FACING_DOWN;
            asm volatile("" : "+r"(new_facing));
            switch (static_cast<i32>(new_facing))
            {
            case FACING_LEFT:
                speed_x = 0xFE;
                break;
            case FACING_RIGHT:
                speed_x = 2;
                break;
            case FACING_UP:
                speed_y = 0xFE;
                break;
            case FACING_DOWN:
                speed_y = 2;
                break;
            }
            register u8 const * settings asm("r0") = reinterpret_cast<u8 const *>(self->game_state);
            asm volatile("" : : "r"(settings));
            register u32 settings_offset asm("r2") = 0x2210;
            asm volatile("" : "+r"(settings_offset) : "r"(settings));
            u32 default_run = *(settings + settings_offset);
            default_run <<= 31;
            asm volatile("" : "+r"(default_run));
            register u32 modifier_mask asm("r2");
            if (!default_run)
            {
                register u32 modifier asm("r1") = held_bytes[1];
                modifier_mask = 1;
                asm volatile("" : "+r"(modifier) : "r"(modifier_mask));
                u32 shifted = modifier_mask;
                asm volatile("" : "+r"(shifted));
                shifted &= modifier;
                if (!shifted)
                    goto walk;
                goto run;
            }
            {
                register u32 modifier asm("r1") = held_bytes[1];
                modifier_mask = 1;
                asm volatile("" : "+r"(modifier) : "r"(modifier_mask));
                u32 shifted = modifier_mask;
                asm volatile("" : "+r"(shifted));
                shifted &= modifier;
                if (shifted)
                    goto walk;
            }
        run:
            {
                register u32 run_motion asm("r0") = 2;
                asm volatile("" : : "r"(run_motion));
                *motion = run_motion;
                register u32 shifted_x asm("r0") = speed_x << 25;
                asm volatile("" : "+r"(shifted_x));
                speed_x = shifted_x >> 24;
                asm volatile("" : "+r"(speed_x));
                register u32 shifted_y asm("r0") = speed_y << 25;
                asm volatile("" : "+r"(shifted_y));
                speed_y = shifted_y >> 24;
                asm volatile("" : "+r"(speed_y));
            }
            goto set_speed;
        walk:
            register u32 walk_motion asm("r2") = modifier_mask;
            asm volatile("" : : "r"(walk_motion));
            *motion = walk_motion;
        }
    }
set_speed:
    {
        register i32 shifted_x asm("r0") = static_cast<i32>(speed_x << 24);
        asm volatile("" : "+r"(shifted_x));
        self->x_speed_q16 = shifted_x >> 8;
        asm volatile("" : : "m"(self->x_speed_q16));
        register i32 shifted_y asm("r0") = static_cast<i32>(speed_y << 24);
        asm volatile("" : "+r"(shifted_y));
        self->y_speed_q16 = shifted_y >> 8;
    }
    {
        register u8 * facing_address asm("r1") = facing;
        asm volatile("" : : "r"(facing_address));
        if (*facing_address != new_facing)
            self->SetAnimFacing(new_facing);
    }
    GameObject * game = self->game_object;
    FarmerInteractionGameVTable const * methods = *reinterpret_cast<FarmerInteractionGameVTable const **>(game);
    register AEntity * (*lookup)(GameObject *, u32) asm("r2") = methods->get_entity;
    asm volatile("" : : "r"(lookup));
    register AActorEntity * horse asm("r1") = static_cast<AActorEntity *>(lookup(game, 0x2C));
    asm volatile("" : : "r"(horse));
    register u8 const * horse_facing asm("r0") = reinterpret_cast<u8 const *>(horse);
    asm volatile("" : "=r"(horse_facing) : "r"(horse));
    horse_facing += 0x20;
    asm volatile("" : : "r"(horse_facing), "r"(horse));
    if (*horse_facing != new_facing)
        horse->SetAnimFacing(new_facing);
}

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
        BeginFarmerToolSwing(farmer);
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

struct FarmerCollisionGameVTable
{
    STRUCT_PAD(0, 0x2C);
    i32 (*get_width)(GameObject *, u32);
    i32 (*get_height)(GameObject *, u32);
    void (*get_grid)(FarmerTerrainGrid *, GameObject *, u32);
    STRUCT_PAD(0x38, 0x40);
    AEntity * (*get_entity)(GameObject *, u32);
};

struct FarmerCollisionActorVTable
{
    STRUCT_PAD(0, 0xC);
    void (*get_bounds)(FarmerTargetBounds *, AEntity *);
    STRUCT_PAD(0x10, 0x40);
    u32 (*get_interaction_state)(AEntity *);
    STRUCT_PAD(0x44, 0x84);
    void (*show_collision_reaction)(AEntity *, u32, u32);
};

struct FarmerCollisionFrame
{
    FarmerTerrainGrid grid;
    FarmerTargetBounds current_return, current, next_return, next, target_return, target;
    FarmerTargetBounds tile_return, tile, next_tile;
    GameObject * game;
    u32 map, can_react, facing, pressure, count;
    u32 const * slots;
    u32 index, slot;
    AEntity * entity;
    i32 target_y;
};

EC u32 const farmer_collision_slots[60] asm("gUnk_080F0E88");
EC u32 const farmer_reduced_collision_slots[21] asm("gUnk_080F0F78");
EC void FarmerEntity_MoveWithSliding(FarmerEntity *, u32, i32);
EC u32 CheckFarmerCollisionTile(FarmerTerrainGrid const *, FarmerTargetBounds const *) asm("DoesEntityCollisionRectangleHitSolidTile");

#define COLLISION_GAME(game) (*reinterpret_cast<FarmerCollisionGameVTable const **>(game))
#define COLLISION_ACTOR(entity) (reinterpret_cast<FarmerCollisionActorVTable const **>(entity)[5])

EC void UpdateFarmerMovementCollision(FarmerEntity *) ENTITY_CODE(0802CDCC);
EC void UpdateFarmerMovementCollision(FarmerEntity * self)
{
    FarmerCollisionFrame frame;
    {
        register u8 * reaction_timer asm("r1") = reinterpret_cast<u8 *>(&self->unk_B3);
        register u32 remaining asm("r0") = *reaction_timer;
        register u8 * pressure asm("r1");
        register u32 scale asm("r0");
        if (remaining)
        {
            --remaining;
            *reaction_timer = remaining;
            remaining <<= 24;
            if (!remaining)
            {
                register u32 slot asm("r4") = 0x2B;
                do
                {
                    GameObject * game = self->game_object;
                    register AEntity * (*get_entity)(GameObject *, u32) asm("r2") = COLLISION_GAME(game)->get_entity;
                    register AEntity * actor asm("r2") = get_entity(game, slot);
                    if (actor)
                    {
                        register u32 (*refresh)(AEntity *) asm("r1") = COLLISION_ACTOR(actor)->get_interaction_state;
                        refresh(actor);
                    }
                    ++slot;
                } while (slot <= 0x45);
                goto check_position;
            }
            pressure = reinterpret_cast<u8 *>(&self->unk_B4);
            scale = *pressure;
            if (scale > 89)
                goto check_position;
            ++scale;
        }
        else
        {
            pressure = reinterpret_cast<u8 *>(&self->unk_B4);
            scale = *pressure;
            if (!scale)
                goto check_position;
            --scale;
        }
        *pressure = scale;
    }
check_position:
    frame.game = self->game_object;
    asm volatile("" : "+m"(frame.game));
    register u32 map asm("r1") = self->location_map;
    frame.map = map;
    asm volatile("" : "+m"(frame.map));
    {
        register u32 x_speed asm("r0") = self->x_speed_q16;
        if (x_speed)
            goto check_actor_collisions;
        register u32 y_speed asm("r1") = self->y_speed_q16;
        if (!y_speed)
            goto move_farmer;
    }
check_actor_collisions:
    {
        register u32 state asm("r0") = self->state;
        if (state == 0x2C)
            goto move_farmer;
        if (state == 0x35)
            goto move_farmer;
        register FarmerCollisionActorVTable const * methods asm("r1") = COLLISION_ACTOR(self);
        register FarmerTargetBounds * output asm("r0") = &frame.current_return;
        register void (*bounds)(FarmerTargetBounds *, AEntity *) asm("r2") = methods->get_bounds;
        bounds(output, self);
        frame.current = frame.current_return;
        register FarmerTargetBounds const * copied_box asm("r2") = &frame.current;
        asm volatile("" : "+r"(copied_box) : "m"(frame.current));
        register FarmerTargetBounds const * current asm("sb") = copied_box;
        register u32 (*get_state)(AEntity *) asm("r1") = COLLISION_ACTOR(self)->get_interaction_state;
        frame.can_react = static_cast<u8>(get_state(self) ^ 1);
        asm volatile("" : "+m"(frame.can_react));
        frame.facing = self->facing;
        asm volatile("" : "+m"(frame.facing));
        frame.pressure = *reinterpret_cast<u8 *>(&self->unk_B4);
        asm volatile("" : "+m"(frame.pressure));
        register u32 keep_dx_r0 asm("r0");
        register u32 keep_dx_r1 asm("r1");
        register u32 keep_dx_r2 asm("r2");
        asm volatile("" : "=l"(keep_dx_r0), "=l"(keep_dx_r1), "=l"(keep_dx_r2));
        register i32 dx asm("r3") = *reinterpret_cast<i16 const *>(reinterpret_cast<u8 const *>(self) + 0x1A);
        asm volatile("" : "+l"(dx) : "l"(keep_dx_r0), "l"(keep_dx_r1), "l"(keep_dx_r2) : "memory");
        register u32 keep_dy_r0 asm("r0");
        register u32 keep_dy_r1 asm("r1");
        register u32 keep_dy_r2 asm("r2");
        asm volatile("" : "=l"(keep_dy_r0), "=l"(keep_dy_r1), "=l"(keep_dy_r2));
        register i32 dy asm("r4") = *reinterpret_cast<i16 const *>(reinterpret_cast<u8 const *>(self) + 0x1E);
        asm volatile("" : "+l"(dy) : "l"(keep_dy_r0), "l"(keep_dy_r1), "l"(keep_dy_r2) : "memory");
        register FarmerTargetBounds const * bounds_ptr asm("r6") = current;
        register i32 left asm("r0") = *reinterpret_cast<i16 const *>(reinterpret_cast<u8 const *>(bounds_ptr) + 0);
        asm volatile("" : "+l"(left) : : "memory");
        register u32 keep_top_r1 asm("r1");
        asm volatile("" : "=l"(keep_top_r1));
        register i32 top asm("r2") = *reinterpret_cast<i16 const *>(reinterpret_cast<u8 const *>(bounds_ptr) + 2);
        asm volatile("" : "+l"(top) : "l"(keep_top_r1) : "memory");
        register i32 right asm("r5") = *reinterpret_cast<i16 const *>(reinterpret_cast<u8 const *>(bounds_ptr) + 4);
        asm volatile("" : "+l"(right) : : "memory");
        register i32 bottom asm("r6") = *reinterpret_cast<i16 const *>(reinterpret_cast<u8 const *>(bounds_ptr) + 6);
        asm volatile("" : "+l"(bottom) : : "memory");
        register i32 bottom_copy asm("r8") = bottom;
        register FarmerTargetBounds * output_box asm("r1") = &frame.next_return;
        left = dx + left;
        output_box->x1 = left;
        asm volatile("" : : "m"(output_box->x1));
        top = dy + top;
        output_box->y1 = top;
        dx += right;
        output_box->x2 = dx;
        dy += bottom_copy;
        output_box->y2 = dy;
        frame.next = frame.next_return;
        register FarmerTargetBounds const * copied_next asm("r2") = &frame.next;
        asm volatile("" : "+r"(copied_next) : "m"(frame.next));
        register FarmerTargetBounds const * next asm("sl") = copied_next;
        if (self->motion != 2)
        {
            register u32 walking_count asm("r3") = 60;
            frame.count = walking_count;
            asm volatile("" : "+m"(frame.count));
            register u32 const * walking_slots asm("r4") = farmer_collision_slots;
            frame.slots = walking_slots;
            asm volatile("" : "+m"(frame.slots));
        }
        else
        {
            register u32 running_count asm("r5") = 21;
            frame.count = running_count;
            asm volatile("" : "+m"(frame.count));
            register u32 const * running_slots asm("r6") = farmer_reduced_collision_slots;
            frame.slots = running_slots;
            asm volatile("" : "+m"(frame.slots));
        }
        asm volatile("" : "+m"(frame.count), "+m"(frame.slots));
        register u32 first_index asm("r0") = 0;
        frame.index = first_index;
        register u32 count asm("r1") = frame.count;
        if (first_index >= count)
            goto finished_actor_scan;

        register FarmerTargetBounds const * target_box asm("r2") = &frame.target;
        register FarmerTargetBounds const * target asm("r8") = target_box;
        do
        {
            register u32 i asm("r3") = frame.index;
            register u32 offset asm("r0") = i << 2;
            register u32 const * slots asm("r4") = frame.slots;
            offset += reinterpret_cast<u32>(slots);
            register u32 slot asm("r0") = *reinterpret_cast<u32 const *>(offset);
            *reinterpret_cast<u32 volatile *>(&frame.slot) = slot;
            register GameObject * game asm("r5") = frame.game;
            register AEntity * (*get_entity)(GameObject *, u32) asm("r2") = COLLISION_GAME(game)->get_entity;
            register GameObject * game_arg asm("r0") = game;
            register AEntity * entity asm("r3") = get_entity(game_arg, frame.slot);
            frame.entity = entity;
            if (!entity)
                goto next_slot;
            {
                register u32 entity_map asm("r0") = entity->location_map;
                register u32 current_map asm("r6") = frame.map;
                if (entity_map != current_map)
                    goto next_slot;
            }
            {
                register FarmerCollisionActorVTable const * actor_methods asm("r1") = COLLISION_ACTOR(entity);
                register FarmerTargetBounds * output asm("r0") = &frame.target_return;
                register void (*get_target_bounds)(FarmerTargetBounds *, AEntity *) asm("r2") = actor_methods->get_bounds;
                get_target_bounds(output, entity);
                frame.target = frame.target_return;
                register FarmerTargetBounds const * target_ptr = target;
                register u32 keep_target_left_r2 asm("r2");
                register u32 keep_target_left_r3 asm("r3");
                register u32 keep_target_left_r4 asm("r4");
                register u32 keep_target_left_r5 asm("r5");
                register u32 keep_target_left_r6 asm("r6");
                asm volatile("" : "=l"(keep_target_left_r2), "=l"(keep_target_left_r3), "=l"(keep_target_left_r4), "=l"(keep_target_left_r5), "=l"(keep_target_left_r6));
                register i32 left = target_ptr->x1;
                asm volatile("" : "+l"(left) : "l"(keep_target_left_r2), "l"(keep_target_left_r3), "l"(keep_target_left_r4), "l"(keep_target_left_r5), "l"(keep_target_left_r6) : "memory");
                register i32 target_left asm("ip") = left;

                register FarmerTargetBounds const * current_ptr = current;
                register i32 old_gap asm("r1") = current_ptr->x2;
                old_gap = left - old_gap;
                register i32 diff asm("r0") = reinterpret_cast<i16 const *>(current_ptr)[0];

                register FarmerTargetBounds const * target_read asm("r5") = target;
                register i32 target_right asm("r4") = target_read->x2;
                diff -= target_right;
                old_gap &= diff;
                register i32 target_top asm("r0") = target_read->y1;
                *reinterpret_cast<i32 volatile *>(&frame.target_y) = target_top;
                register FarmerTargetBounds const * old_box asm("r3") = current;
                register i32 old_bottom asm("r0") = old_box->y2;
                register i32 top asm("r6") = frame.target_y;
                old_gap &= top - old_bottom;
                register u32 keep_old_top_r5 asm("r5");
                register u32 keep_old_top_r6 asm("r6");
                register i32 old_top asm("r0") = old_box->y1;
                register FarmerTargetBounds const * target_bottom_ptr asm("r5") = target;
                register u32 keep_bottom_r2 asm("r2");
                asm volatile("" : "=l"(keep_bottom_r2) : "l"(old_top), "l"(target_bottom_ptr), "l"(keep_old_top_r5), "l"(keep_old_top_r6));
                register i32 target_bottom asm("r3") = target_bottom_ptr->y2;
                asm volatile("" : "+l"(target_bottom) : "l"(keep_bottom_r2), "l"(target_bottom_ptr) : "memory");
                old_gap &= old_top - target_bottom;
                register u32 clear asm("r2") = static_cast<u32>(old_gap) >> 31;
                if (clear)
                    goto next_slot;

                register FarmerTargetBounds const * new_ptr = next;
                register i32 new_gap asm("r1") = new_ptr->x2;
                register i32 left_copy asm("r6") = target_left;
                new_gap = left_copy - new_gap;
                register i32 new_left asm("r0") = new_ptr->x1;
                new_gap &= new_left - target_right;
                register FarmerTargetBounds const * new_box asm("r6") = next;
                register i32 new_bottom asm("r0") = new_box->y2;
                register i32 top_copy asm("r5") = frame.target_y;
                new_gap &= top_copy - new_bottom;
                register i32 new_top asm("r0") = static_cast<i16>(reinterpret_cast<u16 const *>(new_box)[1]);
                asm volatile("" : : "l"(new_top), "l"(top_copy), "l"(new_box));
                new_gap &= new_top - target_bottom;
                if (new_gap >= 0)
                    goto next_slot;

                self->x_speed_q16 = clear;
                self->y_speed_q16 = clear;
                register u32 can_react asm("r5") = frame.can_react;
                if (!can_react)
                    goto next_slot;
                u32 reacts = 0;
                register u32 reaction_slot asm("r0") = frame.slot;
                if (reaction_slot - 0x2B <= 0x1A)
                    reacts = 1;
                if (!reacts)
                    goto next_slot;
                register AEntity * collided asm("r6") = frame.entity;
                register void (*const * reaction_method)(AEntity *, u32, u32) asm("r0") = &COLLISION_ACTOR(collided)->show_collision_reaction;
                register void (*react)(AEntity *, u32, u32) asm("r3") = *reaction_method;
                register AEntity * actor_arg asm("r0") = collided;
                register u32 facing_arg asm("r1") = frame.facing;
                register u32 pressure_arg asm("r2") = frame.pressure;
                react(actor_arg, facing_arg, pressure_arg);
                *reinterpret_cast<u8 *>(&self->unk_B3) = 8;
            }
        next_slot:
            ;
            register u32 next_index asm("r0") = frame.index;
            ++next_index;
            *reinterpret_cast<u32 volatile *>(&frame.index) = next_index;
            register u32 count asm("r1") = frame.count;
            if (next_index < count)
                continue;
            goto move_farmer;
        } while (true);
    }
finished_actor_scan:
move_farmer:
    register i32 x_move asm("r0") = self->x_speed_q16;
    register i32 y_move asm("r1") = self->y_speed_q16;
    if (!x_move && !y_move)
        return;
    register i32 old_x asm("r5") = self->x_q16;
    register i32 old_y asm("r6") = self->y_q16;
    x_move += old_x;
    register i32 x asm("r8") = x_move;
    y_move += old_y;
    register i32 y asm("sb") = y_move;
    if (x_move < 0)
    {
        register i32 left_edge asm("r2") = 0;
        asm volatile("" : "+r"(left_edge));
        x = left_edge;
    }
    else
    {
        register GameObject * game asm("r3") = frame.game;
        register i32 (*get_width)(GameObject *, u32) asm("r2") = COLLISION_GAME(game)->get_width;
        register GameObject * game_arg asm("r0") = game;
        i32 right = get_width(game_arg, frame.map) << 16;
        if (x > right)
            x = right;
    }
    register i32 proposed_y asm("r4") = y;
    asm volatile("" : "+r"(proposed_y));
    register i32 bottom asm("r0");
    if (proposed_y < 0)
    {
        bottom = 0;
        goto clamp_bottom;
    }
    else
    {
        register GameObject * game asm("r1") = frame.game;
        register FarmerCollisionGameVTable const * methods asm("r0") = COLLISION_GAME(game);
        register i32 (*get_height)(GameObject *, u32) asm("r2") = methods->get_height;
        register GameObject * game_arg asm("r0") = game;
        register u32 map asm("r1") = frame.map;
        bottom = get_height(game_arg, map) << 16;
        if (y <= bottom)
            goto vertical_clamped;
    }
    {
    clamp_bottom:
        y = bottom;
    vertical_clamped: ;
    }
    if (static_cast<u8>(self->state - 0x2C) <= 1)
        goto apply_position;
    {
        register GameObject * game asm("r4") = frame.game;
        register FarmerCollisionGameVTable const * methods asm("r1") = COLLISION_GAME(game);
        register FarmerTerrainGrid * grid asm("r0") = &frame.grid;
        register void (*get_grid)(FarmerTerrainGrid *, GameObject *, u32) asm("r3") = methods->get_grid;
        register GameObject * game_arg asm("r1") = game;
        get_grid(grid, game_arg, frame.map);
        register FarmerCollisionActorVTable const * actor_methods asm("r1") = COLLISION_ACTOR(self);
        register FarmerTargetBounds * box_output asm("r0") = &frame.tile_return;
        register void (*get_bounds)(FarmerTargetBounds *, AEntity *) asm("r2") = actor_methods->get_bounds;
        get_bounds(box_output, self);
        frame.tile = frame.tile_return;
        register FarmerTargetBounds const * bounds asm("r4") = &frame.tile;
        if (CheckFarmerCollisionTile(&frame.grid, bounds) << 24)
        {
            old_x = x;
            asm volatile("" : "+r"(old_x));
            self->x_q16 = old_x;
            old_y = y;
            asm volatile("" : "+r"(old_y));
            self->y_q16 = old_y;
            goto update_mount;
        }
        register i32 new_x asm("r0") = x;
        register i32 new_x_tile asm("r1") = new_x >> 16;
        new_x = old_x >> 16;
        old_x = new_x_tile - new_x;
        register i32 new_y asm("r2") = y;
        asm volatile("" : "+r"(new_y));
        register i32 new_y_tile asm("r1") = new_y >> 16;
        register i32 old_y_tile asm("r0") = old_y >> 16;
        old_y = new_y_tile - old_y_tile;
        asm volatile("" : "+l"(bounds));
        register u32 keep_left_r1 asm("r1");
        register u32 keep_left_r2 asm("r2");
        asm volatile("" : "=l"(keep_left_r1), "=l"(keep_left_r2));
        register i32 left asm("r0") = *reinterpret_cast<i16 const *>(reinterpret_cast<u8 const *>(bounds) + 0);
        asm volatile("" : "+l"(left) : "l"(keep_left_r1), "l"(keep_left_r2) : "memory");
        register u32 keep_top_r3 asm("r3");
        asm volatile("" : "=l"(keep_top_r3));
        register i32 top asm("r2") = *reinterpret_cast<i16 const *>(reinterpret_cast<u8 const *>(bounds) + 2);
        asm volatile("" : "+l"(top) : "l"(keep_top_r3) : "memory");
        register i32 right asm("r3") = *reinterpret_cast<i16 const *>(reinterpret_cast<u8 const *>(bounds) + 4);
        asm volatile("" : "+l"(right) : : "memory");
        register i32 bottom asm("r4") = *reinterpret_cast<i16 const *>(reinterpret_cast<u8 const *>(bounds) + 6);
        asm volatile("" : "+l"(bottom) : : "memory");
        register FarmerTargetBounds * next_box asm("r1") = &frame.next_tile;
        left = old_x + left;
        next_box->x1 = left;
        top = old_y + top;
        next_box->y1 = top;
        right = old_x + right;
        next_box->x2 = right;
        bottom = old_y + bottom;
        next_box->y2 = bottom;
        register u32 solid_tile asm("r0") = CheckFarmerCollisionTile(&frame.grid, next_box);
        register u32 mask asm("r1") = 1;
        solid_tile ^= mask;
        if (!(solid_tile << 24))
            goto slide_farmer;
    }
apply_position:
    {
        register i32 applied_x asm("r2") = x;
        asm volatile("" : "+r"(applied_x));
        self->x_q16 = applied_x;
        register i32 applied_y asm("r3") = y;
        asm volatile("" : "+r"(applied_y));
        self->y_q16 = applied_y;
    }
    goto update_mount;
slide_farmer:
    if (old_x > 0)
        FarmerEntity_MoveWithSliding(self, FACING_RIGHT, old_x);
    else if (old_x < 0)
        FarmerEntity_MoveWithSliding(self, FACING_LEFT, -old_x);
    else if (old_y > 0)
        FarmerEntity_MoveWithSliding(self, FACING_DOWN, old_y);
    else if (old_y < 0)
        FarmerEntity_MoveWithSliding(self, FACING_UP, -old_y);
update_mount:
    register u32 (*get_state)(AEntity *) asm("r1") = COLLISION_ACTOR(self)->get_interaction_state;
    if (get_state(self) << 24)
    {
        register GameObject * game asm("r4") = frame.game;
        register FarmerCollisionGameVTable const * methods asm("r0") = COLLISION_GAME(game);
        register AEntity * (*get_entity)(GameObject *, u32) asm("r2") = methods->get_entity;
        AEntity * mount = get_entity(game, 0x2C);
        mount->x_q16 = self->x_q16;
        mount->y_q16 = self->y_q16;
    }
}

#undef COLLISION_ACTOR
#undef COLLISION_GAME

struct FarmerInteractionEscapeOffset
{
    i16 x, y;
};

struct FarmerActionInputGameVTable
{
    STRUCT_PAD(0x00, 0x34);
    void (*get_grid)(FarmerTerrainGrid *, GameObject *, u32);
    void (*refresh_entity)(GameObject *, u32);
    void (*remove_entity)(GameObject *, u32);
    AEntity * (*get_entity)(GameObject *, u32);
    STRUCT_PAD(0x44, 0xE0);
    void (*pick_up_tile_item)(GameObject *, Location const *, RucksackItem *);
    STRUCT_PAD(0xE4, 0x12C);
    AEntity * (*get_interaction_entity)(GameObject *, u32);
    STRUCT_PAD(0x130, 0x134);
    u32 (*select_interaction_slot)(GameObject *, Location const *, FarmerTargetBounds const *);
    STRUCT_PAD(0x138, 0x140);
    FarmerEventDispatch dispatch_event;
    STRUCT_PAD(0x144, 0x14C);
    EntitySoundChannels const * (*get_channels)(GameObject *);
    STRUCT_PAD(0x150, 0x15C);
    void (*give_horse_food)(GameObject *, Product const *);
};

struct FarmerActionInputActorVTable
{
    STRUCT_PAD(0, 0x34);
    u16 (*get_interaction_event)(AEntity *);
    STRUCT_PAD(0x38, 0x88);
    void (*set_reaction_time)(AEntity *, u32);
};

struct FarmerInteractionInputFrame
{
    u16 origin[3];
    u16 origin_pad;
    u32 scratch[6];
    FarmerEventRequest input_event;
    FarmerHeldItemKeyWord pressed;
    FarmerTargetBounds find_box, find_box_copy, horse_box, horse_box_copy;
    FarmerInteractionEscapeOffset escape, escape_copy;
    FarmerTargetBounds moved_horse_box;
    u8 horse_food, collectable_food, special_item, item_pad;
    FarmerEventRequest world_event;
    Location * origin_address;
    u32 event_id;
    u32 handled;
    void const * tile_item_vtable_slot;
    u8 const * facing;
    u8 * state;
};

EC void ReadFarmerInteractionLocation(Location *, AEntity const *) asm("GetLocation__C7AEntity");
EC u32 FarmerReadHorseGrowth(Horse const *) asm("GetGrowthStage__C5Horse");
EC void StartHorseInteraction(AActorEntity *) asm("HorseEntity_BeginPlay");
EC u32 ReadHorseTileCollision(FarmerTerrainGrid const *, Box const *) asm("DoesEntityCollisionRectangleHitSolidTile");
EC void ReadFarmerCollisionEscape(FarmerInteractionEscapeOffset *, FarmerTerrainGrid const *, Box const *) asm("Map_FindCollisionEscapeOffset");
EC u32 ReadFarmerInteractionProduct(FarmerEntity const *) asm("FarmerEntity_GetHeldProduct");
EC u32 ReadFarmerInteractionProductPrice(void const *) asm("GetPrice__C7Product");
EC void func_08032384(AActorEntity &, u32, bool);
EC void FarmerEntity_LiftDog(FarmerEntity *);
EC void FarmerEntity_LiftChicken(FarmerEntity *, u8);
EC void FarmerEntity_LiftRucksackItem(FarmerEntity *, RucksackItem const *, bool);
EC void SetFarmerNpcReaction(AEntity *, void const *) asm("SetNpcReaction");
EC u32 ItemCollection_GetByteCount(void const *);
EC void PopFarmerEgg(Coop *, u32) asm("method_0800C9B8__4CoopUi");
EC void ConstructFarmerInteractionFood(void *, u32) asm("__4FoodUi");
EC void ConstructFarmerInteractionArticle(void *, u32) asm("__7ArticleUi");
EC void ConstructFarmerFoodItem(RucksackItem *, u32) asm("__12RucksackItemG4Food");
EC void ConstructFarmerArticleItem(RucksackItem *, u32) asm("__12RucksackItemG7Article");
EC u32 IsFarmerInteractionItemEmpty(RucksackItem const *) asm("IsEmpty__C12RucksackItem");
EC u32 IsFarmerSideSpriteLaunched(AEntity *) asm("IsSideSpriteLaunched");
EC void LaunchFarmerSideSprite(AEntity *, u32) asm("LaunchSideSprite");

#define FARMER_ACTION_GAME(game) (*reinterpret_cast<FarmerActionInputGameVTable const **>(game))
#define FARMER_ACTION_ACTOR(actor) (reinterpret_cast<FarmerActionInputActorVTable const **>(actor)[5])
#define FARMER_ACTION_STATE(type, offset) reinterpret_cast<type *>(static_cast<u8 *>(self->game_state) + offset)
#define INTERACTION_AUDIO_METHOD(game, table_reg, offset_reg) \
    register FarmerActionInputGameVTable const * methods asm(table_reg) = FARMER_ACTION_GAME(game); \
    asm volatile("" : "+r"(methods)); \
    register u32 sound_offset asm(offset_reg) = 0x14C; \
    asm volatile("" : "+r"(sound_offset)); \
    typedef EntitySoundChannels const * (*GetChannels)(GameObject *); \
    register GetChannels const * method_ptr asm(table_reg) = reinterpret_cast<GetChannels const *>(reinterpret_cast<u8 const *>(methods)+sound_offset); \
    asm volatile("" : "+r"(method_ptr)); \
    register GetChannels method asm("r1") = *method_ptr; \
    asm volatile("" : : "r"(method)); \
    EntitySoundChannels const * channels = method(game);

#define SCAN_INTERACTION_AUDIO(channel, channel_end, found, fallback) \
    channel = channels->begin; \
    channel_end = channels->end; \
    asm volatile("" : : "r"(channel), "r"(channel_end)); \
    if (channel == channel_end) goto fallback; \
    do \
    { \
        u32 playing = AudioPlayerIsPlayingRaw(*channel) << 24; \
        asm volatile("" : "+r"(playing)); \
        if (playing == 0) goto found; \
        ++channel; \
    } while (channel != channel_end); \
    goto fallback;

EC void HandleFarmerInteractionInput(FarmerEntity *, FarmerInputState const *) ENTITY_CODE(0802E0FC);
EC void HandleFarmerInteractionInput(FarmerEntity * farmer, FarmerInputState const * input)
{
    FarmerEntity * self = farmer;
    FarmerInteractionInputFrame frame;
    register u32 held asm("r2") = 0;
    frame.pressed.word = held;
    asm volatile("" : "+r"(held), "+m"(frame.pressed.word));
    if (input)
    {
        held = input->held;
        asm volatile("" : "+r"(held));
        frame.pressed.word = input->pressed;
        asm volatile("" : "+m"(frame.pressed.word));
    }
    register u32 exhaustion asm("r4") = self->exhaustion_state;
    asm volatile("" : "+r"(exhaustion));
    if (exhaustion)
    {
        self->state = 0;
        return;
    }
    register u32 a_button asm("r5") = INPUT_A;
    asm volatile("" : "+r"(a_button));
    register u32 a_held asm("r0") = a_button & held;
    asm volatile("" : "+r"(a_held));
    if (a_held)
    {
        register FarmerHeldItemKeyWord * pressed asm("r2") = &frame.pressed;
        asm volatile("" : "+r"(pressed));
        register u32 buttons asm("r1") = pressed->bytes[0];
        asm volatile("" : "+r"(buttons));
        register u32 b_button asm("r3") = INPUT_B;
        if (b_button & buttons)
        {
            GameObject * game = self->game_object;
            register FarmerActionInputGameVTable const * methods asm("r3") = FARMER_ACTION_GAME(game);
            asm volatile("" : "+r"(methods));
            register FarmerEventDispatch const * method asm("r3") = &methods->dispatch_event;
            register FarmerEventRequest * request asm("r1") = &frame.input_event;
            asm volatile("" : "+r"(request));
            register u32 event_id asm("r2") = 0x3CE;
            asm volatile("" : "+r"(event_id));
            frame.input_event.id = event_id;
            asm volatile("" : : "m"(frame.input_event.id));
            request->argument = exhaustion;
            register FarmerEventDispatch dispatch asm("r3") = *method;
            dispatch(game, request, 0);
            self->state = exhaustion;
            return;
        }
        buttons = pressed->bytes[1];
        register u32 b_pressed asm("r0") = b_button & buttons;
        asm volatile("" : "+r"(b_pressed));
        b_pressed <<= 24;
        asm volatile("" : "+r"(b_pressed));
        register u32 change_item asm("r2") = b_pressed >> 24;
        asm volatile("" : "+r"(change_item));
        if (change_item)
        {
            if (!(FarmerHeldItemIsEmptyRaw(&self->farmer->unk_54) << 24))
                FarmerEntity_CycleHeldItem(self, true);
            else
                FarmerEntity_CycleHeldItemWithFeedback(self, false);
            return;
        }
        register u32 a_pressed asm("r0") = a_button & buttons;
        asm volatile("" : "+r"(a_pressed));
        if (a_pressed)
            self->state = change_item;
        return;
    }
    register u8 * state asm("r5") = &self->state;
    asm volatile("" : "+r"(state));
    *state = a_held;
    register GameObject * interaction_game asm("r2") = self->game_object;
    asm volatile("" : "+r"(interaction_game));
    register GameObject * base_game asm("sb") = interaction_game;
    asm volatile("" : "+r"(base_game));
    Location * origin = reinterpret_cast<Location *>(frame.origin);
    ReadFarmerInteractionLocation(origin, self);
    register Location * saved_origin asm("r3") = reinterpret_cast<Location *>(frame.origin);
    asm volatile("" : "+r"(saved_origin));
    frame.origin_address = saved_origin;
    register u8 * facing asm("r4") = &self->facing;
    asm volatile("" : "+r"(facing), "+m"(frame.origin_address));
    register u32 direction asm("r2") = *facing;
    asm volatile("" : "+r"(direction));
    ReadFarmerInteractionBox(&frame.find_box, self, direction);
    frame.find_box_copy = frame.find_box;
    register FarmerTargetBounds const * find_box asm("r2") = &frame.find_box_copy;
    asm volatile("" : "+r"(find_box));
    register GameObject * game_copy asm("r1") = base_game;
    asm volatile("" : "+r"(game_copy));
    register FarmerActionInputGameVTable const * methods asm("r0") = FARMER_ACTION_GAME(game_copy);
    typedef u32 (*SelectSlot)(GameObject *, Location const *, FarmerTargetBounds const *);
    register unsigned index asm("r3") = 0x134;
    asm volatile("" : "+r"(index));
    register SelectSlot const * slot_method asm("r0") = reinterpret_cast<SelectSlot const *>(reinterpret_cast<u8 const *>(methods) + index);
    asm volatile("" : "+r"(slot_method));
    register u32 (*select_slot)(GameObject *, Location const *, FarmerTargetBounds const *) asm("r3") = *slot_method;
    asm volatile("" : : "r"(select_slot));
    register u32 slot asm("r6") = select_slot(base_game, origin, find_box);
    register u32 no_event asm("r0") = 0;
    asm volatile("" : "+r"(no_event));
    frame.event_id = no_event;
    register u32 not_handled asm("r1") = 0;
    asm volatile("" : "+r"(not_handled));
    frame.handled = not_handled;
    asm volatile("" : "+m"(frame.event_id), "+m"(frame.handled));
    register u32 slot_type asm("r0") = slot - 0x2B;
    asm volatile("" : "+r"(slot_type));
    frame.state = state;
    frame.facing = facing;
    asm volatile("" : "+m"(frame.state), "+m"(frame.facing));

    register RucksackItem * item asm("r5");
    register void ** gift_channel asm("r4");
    register void ** gift_end asm("r5");
    register void * gift_player asm("r0");
    register void ** discard_channel asm("r4");
    register void ** discard_end asm("r5");
    register void * discard_player asm("r0");
    switch (slot_type)
    {
    case 1:
        {
            register u32 growth asm("r0") = FarmerReadHorseGrowth(FARMER_ACTION_STATE(Farm, 0x14)->GetHorse());
            asm volatile("" : "+r"(growth));
            if (growth != 1)
                goto horse_too_young;
        }
        if (FarmerHeldItemIsEmptyRaw(&self->farmer->unk_54) << 24)
        {
            register u32 map asm("r2") = self->location_map;
            asm volatile("" : "+r"(map));
            register FarmerTerrainGrid * scratch asm("r3") = reinterpret_cast<FarmerTerrainGrid *>(frame.scratch);
            asm volatile("" : "+r"(scratch));
            register FarmerTerrainGrid * grid asm("r8") = scratch;
            register GameObject * method_game asm("r5") = base_game;
            asm volatile("" : "+r"(method_game));
            register FarmerActionInputGameVTable const * methods asm("r1") = FARMER_ACTION_GAME(method_game);
            asm volatile("" : "+r"(methods));
            register FarmerTerrainGrid * grid_arg asm("r0") = grid;
            register void (*get_grid)(FarmerTerrainGrid *, GameObject *, u32) asm("r3") = methods->get_grid;
            asm volatile("" : : "r"(get_grid));
            get_grid(grid_arg, base_game, map);

            register i32 x asm("r5");
            register u32 keep_0_r2 asm("r2");
            register u32 keep_0_r3 asm("r3");
            asm volatile("" : "=l"(keep_0_r2), "=l"(keep_0_r3));
            x = self->x_q16 >> 16;
            asm volatile("" : "+r"(x) : "l"(keep_0_r2), "l"(keep_0_r3));

            register i32 y asm("r4");
            register u32 keep_1_r0 asm("r0");
            register u32 keep_1_r2 asm("r2");
            register u32 keep_1_r3 asm("r3");
            asm volatile("" : "=l"(keep_1_r0), "=l"(keep_1_r2), "=l"(keep_1_r3));
            y = self->y_q16 >> 16;
            asm volatile("" : "+r"(y) : "l"(keep_1_r0), "l"(keep_1_r2), "l"(keep_1_r3));
            register FarmerTargetBounds * horse_box_ptr asm("r1") = &frame.horse_box;
            asm volatile("" : "+r"(horse_box_ptr));
            register i32 horse_box_coord asm("r0") = x - 7;
            asm volatile("" : "+r"(horse_box_coord));
            horse_box_ptr->x1 = horse_box_coord;
            asm volatile("" : : "m"(horse_box_ptr->x1));
            horse_box_coord = y;
            asm volatile("" : "+r"(horse_box_coord));
            horse_box_coord -= 9;
            horse_box_ptr->y1 = horse_box_coord;
            asm volatile("" : : "m"(horse_box_ptr->y1));
            horse_box_coord = x + 7;
            horse_box_ptr->x2 = horse_box_coord;
            horse_box_coord = y + 5;
            horse_box_ptr->y2 = horse_box_coord;
            frame.horse_box_copy = frame.horse_box;
            register Box * collision_box asm("r6") = reinterpret_cast<Box *>(&frame.horse_box_copy);
            asm volatile("" : "+r"(collision_box));
            register u32 blocked asm("r0") = ReadHorseTileCollision(grid, collision_box);
            asm volatile("" : "+r"(blocked));
            register u32 one asm("r2") = 1;
            asm volatile("" : "+r"(one));
            register u32 clear_bit asm("sl") = one;
            register u32 mask asm("r3") = clear_bit;
            asm volatile("" : "+r"(mask));
            blocked ^= mask;
            asm volatile("" : "+r"(blocked));
            if (!(blocked << 24))
            {
                ReadFarmerCollisionEscape(&frame.escape, grid, collision_box);
                frame.escape_copy = frame.escape;
                register FarmerInteractionEscapeOffset const * escape asm("r1") = &frame.escape_copy;
                asm volatile("" : "+r"(escape));

            register i32 escape_delta asm("r0");
            escape_delta = escape->x;
            asm volatile("" : "+r"(escape_delta));
                x += escape_delta;
                asm volatile("" : "+r"(x));


            escape_delta = escape->y;
            asm volatile("" : "+r"(escape_delta));
                y += escape_delta;
                asm volatile("" : "+r"(y));
                register FarmerTargetBounds * moved_horse_box_ptr asm("r1") = &frame.moved_horse_box;
                asm volatile("" : "+r"(moved_horse_box_ptr));
                register i32 moved_horse_box_coord asm("r0") = x - 7;
                asm volatile("" : "+r"(moved_horse_box_coord));
                moved_horse_box_ptr->x1 = moved_horse_box_coord;
                asm volatile("" : : "m"(moved_horse_box_ptr->x1));
                moved_horse_box_coord = y;
                asm volatile("" : "+r"(moved_horse_box_coord));
                moved_horse_box_coord -= 9;
                moved_horse_box_ptr->y1 = moved_horse_box_coord;
                moved_horse_box_coord = x + 7;
                moved_horse_box_ptr->x2 = moved_horse_box_coord;
                asm volatile("" : : "m"(moved_horse_box_ptr->x2));
                moved_horse_box_coord = y + 5;
                moved_horse_box_ptr->y2 = moved_horse_box_coord;
                asm volatile("" : : "r"(moved_horse_box_ptr), "m"(frame.moved_horse_box));
                register u32 moved_blocked asm("r0") = ReadHorseTileCollision(grid, reinterpret_cast<Box *>(moved_horse_box_ptr));
                asm volatile("" : "+r"(moved_blocked));
                register u32 mask asm("r1") = clear_bit;
                asm volatile("" : "+r"(mask));
                moved_blocked ^= mask;
                asm volatile("" : "+r"(moved_blocked));
                if (!(moved_blocked << 24))
                    goto after_interaction;
            }
            register i32 horse_x asm("r6") = x << 16;
            register i32 horse_y_q16 asm("r4") = y << 16;
            asm volatile("" : "+r"(horse_x), "+r"(horse_y_q16));
            register i32 horse_y asm("r8") = horse_y_q16;
            register GameObject * game_for_lookup asm("r2") = base_game;
            register FarmerActionInputGameVTable const * lookup_methods asm("r0") = FARMER_ACTION_GAME(game_for_lookup);
            asm volatile("" : "+r"(lookup_methods));
            register AEntity * (*lookup)(GameObject *, u32) asm("r2") = lookup_methods->get_entity;
            asm volatile("" : : "r"(lookup));
            register AActorEntity * horse asm("r4") = static_cast<AActorEntity *>(lookup(base_game, 0x2C));
            StartHorseInteraction(horse);
            horse->x_q16 = horse_x;
            register i32 placed_y asm("r3") = horse_y;
            asm volatile("" : "+r"(placed_y));
            horse->y_q16 = placed_y;
            register u32 stopped asm("r5") = 0;
            asm volatile("" : "+r"(stopped));
            horse->x_speed_q16 = stopped;
            horse->y_speed_q16 = stopped;
            register u8 const * facing_ptr asm("r1") = frame.facing;
            asm volatile("" : "+r"(facing_ptr));
            register i32 facing asm("r0") = *facing_ptr;
            asm volatile("" : "+r"(facing));
            switch (facing)
            {
            case FACING_UP: if (horse->facing != FACING_RIGHT) horse->SetAnimFacing(FACING_RIGHT); break;
            case FACING_DOWN: if (horse->facing != FACING_LEFT) horse->SetAnimFacing(FACING_LEFT); break;
            case FACING_LEFT: if (horse->facing != FACING_UP) horse->SetAnimFacing(FACING_UP); break;
            case FACING_RIGHT: if (horse->facing != FACING_DOWN) horse->SetAnimFacing(FACING_DOWN); break;
            }
            register i32 position_x asm("r0") = self->x_q16;
            asm volatile("" : "+r"(position_x));
            register i32 * x_speed asm("r5") = reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(self) + 0xB8);
            *x_speed = (horse_x - position_x) / 24;
            asm volatile("" : "+m"(*x_speed));
            register i32 position_y asm("r0") = self->y_q16;
            asm volatile("" : "+r"(position_y));
            register i32 * y_speed asm("r4") = reinterpret_cast<i32 *>(reinterpret_cast<u8 *>(self) + 0xBC);
            asm volatile("" : "+r"(y_speed));
            register i32 target_y asm("r2") = horse_y;
            asm volatile("" : "+r"(target_y));
            *y_speed = (target_y - position_y) / 24;
            asm volatile("" : "+m"(*y_speed));
            self->x_speed_q16 = *x_speed;
            self->y_speed_q16 = *y_speed;
            asm volatile("" : "+m"(self->y_speed_q16));
            register u32 handled asm("r3") = 1;
            asm volatile("" : "+r"(handled));
            frame.handled = handled;
            asm volatile("" : "+m"(frame.handled));
            register u32 next_state asm("r0") = 0x22;
            register u8 * farmer_state asm("r5") = frame.state;
            asm volatile("" : "+r"(farmer_state));
            *farmer_state = next_state;
            goto after_interaction;
        }
        else
        {
            frame.horse_food = ReadFarmerInteractionProduct(self);
            if (!ReadFarmerInteractionProductPrice(&frame.horse_food))
                goto after_interaction;
            GameObject * game = self->game_object;
            typedef void (*FeedHorse)(GameObject *, Product const *);
            register FarmerActionInputGameVTable const * table asm("r0") = FARMER_ACTION_GAME(game);
            asm volatile("" : "+r"(table));
            register u32 index asm("r1") = 0x15C;
            asm volatile("" : "+r"(index));
            register FeedHorse const * feed_slot asm("r0") = reinterpret_cast<FeedHorse const *>(reinterpret_cast<u8 const *>(table) + index);
            asm volatile("" : "+r"(feed_slot));
            register FeedHorse feed_horse asm("r2") = *feed_slot;
            asm volatile("" : : "r"(feed_horse));
            feed_horse(game, reinterpret_cast<Product const *>(&frame.horse_food));
            register u32 next_state asm("r0") = 0xE;
            register u8 * farmer_state asm("r2") = frame.state;
            asm volatile("" : "+r"(farmer_state));
            *farmer_state = next_state;
            asm volatile("" : "+m"(*farmer_state));
            register u32 handled asm("r3") = 1;
            asm volatile("" : "+r"(handled));
            frame.handled = handled;
            AEntity * horse = FARMER_ACTION_GAME(game)->get_entity(game, 0x2C);
            if (horse)
            {
                FARMER_ACTION_ACTOR(horse)->set_reaction_time(horse, 0x3C);
                func_08032384(*static_cast<AActorEntity *>(horse), 5, false);
            }
            INTERACTION_AUDIO_METHOD(game, "r0", "r1")
            gift_channel = channels->begin;
            gift_end = channels->end;
            if (gift_channel == gift_end) goto gift_last_channel;
            do {
                u32 playing = AudioPlayerIsPlayingRaw(*gift_channel) << 24;
                if (playing == 0) goto gift_channel_found;
                ++gift_channel;
            } while (gift_channel != gift_end);
            goto gift_last_channel;
        }
    horse_too_young:
        {
            register u32 event_id asm("r2") = 9;
            frame.event_id = event_id;
            goto after_interaction;
        }
    case 0:
        if (!(FarmerHeldItemIsEmptyRaw(&self->farmer->unk_54) << 24))
            goto after_interaction;
        {
            GameObject * game = self->game_object;
            AEntity * dog = FARMER_ACTION_GAME(game)->get_entity(game, slot);
            if (dog)
                FARMER_ACTION_ACTOR(dog)->set_reaction_time(dog, 0x3C);
            FARMER_ACTION_GAME(game)->remove_entity(game, slot);
            FarmerEntity_LiftDog(self);
            return;
        }
    case 31:
        if (FarmerHeldItemIsEmptyRaw(&self->farmer->unk_54) << 24)
        {
            GameObject * game = self->game_object;
            FARMER_ACTION_GAME(game)->remove_entity(game, slot);
            FarmerEntity_LiftObject(self);
            return;
        }
        {
            goto collectable_food;
        }
    gift_channel_found:
        gift_player = *gift_channel;
        goto gift_play;
    collectable_food:
        {
            register u32 product asm("r0") = ReadFarmerInteractionProduct(self);
            register u8 * collectable asm("r4") = &frame.collectable_food;
            *collectable = product;
            if (!ReadFarmerInteractionProductPrice(collectable))
                goto after_interaction;
            GameObject * game = self->game_object;
            register AEntity * npc asm("r5") = FARMER_ACTION_GAME(game)->get_entity(game, 0x4A);
            register u32 items asm("r0") = ItemCollection_GetByteCount(reinterpret_cast<void **>(npc)[6]);
            register u32 full asm("r1") = 0;
            if (items == 30)
                full = 1;
            if (full)
                goto collection_full;
            SetFarmerNpcReaction(npc, collectable);
            register u32 next_state asm("r0") = 14;
            register u8 * farmer_state asm("r3") = frame.state;
            *farmer_state = next_state;
            register u32 handled asm("r5") = 1;
            frame.handled = handled;
            GameObject * sound_game = self->game_object;
            register FarmerActionInputGameVTable const * methods asm("r1") = FARMER_ACTION_GAME(sound_game);
            asm volatile("" : "+r"(methods));
            register u32 sound_offset asm("r2") = 0x14C;
            typedef EntitySoundChannels const * (*GetChannels)(GameObject *);
            register GetChannels const * method_ptr asm("r1") = reinterpret_cast<GetChannels const *>(reinterpret_cast<u8 const *>(methods)+sound_offset);
            asm volatile("" : "+r"(method_ptr));
            register GetChannels method asm("r1") = *method_ptr;
            EntitySoundChannels const * channels = method(sound_game);
            {
                gift_channel = channels->begin;
                gift_end = channels->end;
                {
                    while (gift_channel != gift_end)
                    {
                        u32 playing = AudioPlayerIsPlayingRaw(*gift_channel) << 24;
                        if (!playing) goto gift_channel_found;
                        ++gift_channel;
                    }
                }
            }
        }
    gift_last_channel:
        {
            register void ** final_channel asm("r0") = gift_end - 1;
            gift_player = *final_channel;
        }
    gift_play:
        AudioPlayer_StartSong(gift_player, 0x6E);
        goto after_interaction;
    collection_full:
        {
            register u32 event_id asm("r3") = 0x3CD;
            frame.event_id = event_id;
            goto after_interaction;
        }
    case 3: case 4: case 5: case 6: case 7: case 8: case 9: case 10:
        if (!(FarmerHeldItemIsEmptyRaw(&self->farmer->unk_54) << 24))
            goto after_interaction;
        {
            GameObject * game = self->game_object;
            AEntity * chicken = FARMER_ACTION_GAME(game)->get_entity(game, slot);
            if (chicken)
                FARMER_ACTION_ACTOR(chicken)->set_reaction_time(chicken, 0x3C);
            FARMER_ACTION_GAME(game)->remove_entity(game, slot);
            FarmerEntity_LiftChicken(self, static_cast<u8>(slot - 0x2E));
            return;
        }
    case 11: case 12: case 13: case 14: case 15: case 16: case 17: case 18: case 19: case 20: case 21: case 22: case 23: case 24: case 25: case 26:
        {
            register u32 cow_idx asm("r1") = slot - 0x36;
            register u8 * target_slot asm("r0") = &self->target_cow_slot;
            *target_slot = cow_idx;
            register u8 * barn_data asm("r0") = static_cast<u8 *>(self->game_state);
            register u32 barn_offset asm("r5") = 0x5F0;
            asm volatile("" : "+r"(barn_offset));
            Cow * cow = reinterpret_cast<Barn *>(barn_data + barn_offset)->GetCow(cow_idx);
            register u32 event_id asm("r1") = 13;
            frame.event_id = event_id;
            if (cow)
            {
                register u32 pet_cow_event asm("r2") = 8;
                frame.event_id = pet_cow_event;
            }
        }
        goto after_interaction;
    case 33: case 34: case 35: case 36: case 37: case 38: case 39: case 40:
        if (!(FarmerHeldItemIsEmptyRaw(&self->farmer->unk_54) << 24))
            goto after_interaction;
        {
            register u32 egg_index asm("r4") = slot - 0x4C;
            register u8 * data asm("r0") = static_cast<u8 *>(self->game_state);
            register u32 coop_offset asm("r3") = 0x410;
            asm volatile("" : "+r"(coop_offset));
            Coop::Egg const * egg = reinterpret_cast<Coop *>(data + coop_offset)->GetEgg(egg_index);
            if (!egg)
                goto after_interaction;
            register u32 quality asm("r0") = egg->quality;
            register u32 default_food asm("r5") = 15;
            asm volatile("" : "+r"(default_food));
            register u32 food asm("r8") = default_food;
            register u32 low_food asm("r0");
            switch (quality)
            {
            case 0: low_food = 15; asm volatile("" : "+r"(low_food)); goto end_food;
            case 1: { register u32 id asm("r1") = 16; asm volatile("" : "+r"(id)); food = id; asm volatile("" : "+r"(food)); break; }
            case 2: { register u32 id asm("r2") = 17; asm volatile("" : "+r"(id)); food = id; asm volatile("" : "+r"(food)); break; }
            case 3: { register u32 id asm("r3") = 18; asm volatile("" : "+r"(id)); food = id; asm volatile("" : "+r"(food)); break; }
            case 4: { register u32 id asm("r5") = 19; asm volatile("" : "+r"(id)); food = id; asm volatile("" : "+r"(food)); break; }
            case 5: low_food = 20; asm volatile("" : "+r"(low_food));
            end_food:
                food = low_food;
                break;
            }
            register GameObject * game asm("r0") = self->game_object;
            register FarmerActionInputGameVTable const * game_methods asm("r1") = FARMER_ACTION_GAME(game);
            register void (*remove_entity)(GameObject *,u32) asm("r2") = game_methods->remove_entity;
            remove_entity(game, slot);
            data = static_cast<u8 *>(self->game_state);
            register u32 coop_offset_again asm("r1") = 0x410;
            asm volatile("" : "+r"(coop_offset_again));
            PopFarmerEgg(reinterpret_cast<Coop *>(data + coop_offset_again), egg_index);
            register u8 * item_data asm("r5") = reinterpret_cast<u8 *>(frame.scratch);
            register u8 * food_data asm("r4") = reinterpret_cast<u8 *>(frame.scratch)+4;
            ConstructFarmerInteractionFood(food_data, food);
            register u32 low asm("r1") = *food_data;
            asm volatile("" : "+r"(low));
            register u8 * mid_ptr asm("r0") = reinterpret_cast<u8 *>(&frame)+0xD;
            register u32 mid asm("r0") = *mid_ptr;
            mid <<= 8;
            mid |= low;
            register u8 * high_ptr asm("r1") = reinterpret_cast<u8 *>(&frame)+0xE;
            register u32 food_arg asm("r1") = *high_ptr;
            food_arg <<= 16;
            food_arg |= mid;
            ConstructFarmerFoodItem(reinterpret_cast<RucksackItem *>(item_data), food_arg);
            item = reinterpret_cast<RucksackItem *>(item_data);
            goto lift_constructed_item;
        }
    case 32:
        if (FarmerHeldItemIsEmptyRaw(&self->farmer->unk_54) << 24)
        {
            register GameObject * game asm("r0") = self->game_object;
            register FarmerActionInputGameVTable const * methods asm("r1") = FARMER_ACTION_GAME(game);
            register AEntity * (*get_entity)(GameObject *,u32) asm("r2") = methods->get_entity;
            AEntity * sprite = get_entity(game, 0x4B);
            if (!(IsFarmerSideSpriteLaunched(sprite) << 24))
            {
                game = self->game_object;
                methods = FARMER_ACTION_GAME(game);
                register void (*remove_entity)(GameObject *,u32) asm("r2") = methods->remove_entity;
                remove_entity(game, slot);
                item = reinterpret_cast<RucksackItem *>(frame.scratch);
                register u8 * article asm("r4") = &frame.special_item;
                ConstructFarmerInteractionArticle(article, 0x35);
                ConstructFarmerArticleItem(item, *article);
                goto lift_constructed_item;
            }
        }
        goto after_interaction;
            lift_constructed_item:
                {
                    asm volatile("" : : "r"(item));
                    FarmerEntity_LiftRucksackItem(self, item, false);
                    return;
                }
    default:
        {
            GameObject * game = self->game_object;
            typedef AEntity * (*FindInteractionEntity)(GameObject *, u32);
            register FarmerActionInputGameVTable const * methods asm("r1") = FARMER_ACTION_GAME(game);
            asm volatile("" : "+r"(methods));
            register u32 method_offset asm("r2") = 0x12C;
            asm volatile("" : "+r"(method_offset));
            register FindInteractionEntity const * lookup_entry asm("r1") =
                reinterpret_cast<FindInteractionEntity const *>(reinterpret_cast<u8 const *>(methods) + method_offset);
            asm volatile("" : "+r"(lookup_entry));
            AEntity * actor = (*lookup_entry)(game, slot);
            if (actor)
                frame.event_id = FARMER_ACTION_ACTOR(actor)->get_interaction_event(actor);
        }
    case 57:
        break;
    }
    goto after_interaction;
lift_item:
    FarmerEntity_LiftRucksackItem(self, reinterpret_cast<RucksackItem *>(frame.scratch), false);
    return;
after_interaction:
    {
        register u32 handled asm("r3") = frame.handled;
        if (!handled)
            goto check_event;
        return;
    }
check_event:
    register u32 pending_0 asm("r0");
    register u32 pending_1 asm("r1");
    register u32 pending_2 asm("r2");
    register u32 pending_3 asm("r3");
    register u32 pending_4 asm("r4");
    asm volatile("" : "=l"(pending_0), "=l"(pending_1), "=l"(pending_2), "=l"(pending_3), "=l"(pending_4));
    if (frame.event_id)
        goto dispatch_world_event;
    {
        asm volatile("" : : "l"(pending_0), "l"(pending_1), "l"(pending_2), "l"(pending_3), "l"(pending_4));
        {
            register Location * ahead asm("r4") = reinterpret_cast<Location *>(frame.scratch);

            register Location * destination asm("r0") = ahead;
            memcpy(destination, frame.origin_address, sizeof(Location));
            register u8 const * facing_ptr asm("r1") = frame.facing;
            register i32 facing asm("r0") = *facing_ptr;
            register i32 horizontal asm("r1") = facing;
            register u32 coordinate_word asm("r0");
            register i32 coordinate_x asm("r1");
            if (facing == FACING_UP) goto tile_up;
            if (facing > FACING_UP) goto tile_horizontal;
            if (facing == FACING_DOWN) goto tile_down;
            goto tile_adjusted;
        tile_horizontal:
            if (horizontal == FACING_LEFT) goto tile_left;
            if (horizontal == FACING_RIGHT) goto tile_right;
            goto tile_adjusted;
        tile_up:
                {
                    register u8 * bytes asm("r2") = reinterpret_cast<u8 *>(&frame);
                    asm volatile("" : "+r"(bytes));
                    register u32 low asm("r3") = bytes[0xB];
                    register u32 y_low asm("r0") = low >> 2;
                    register u32 y_high asm("r1") = reinterpret_cast<u16 *>(bytes)[6];
                    register u32 mask asm("r4") = 0x3FF;
                    y_high = (((y_high & mask) << 6) | y_low) << 16;
                    register u32 delta asm("r5") = 0xFFF00000;
                    asm volatile("" : "+r"(delta));
                    y_high += delta;
                    register u32 new_low asm("r2") = ((y_high >> 16) & 0x3F) << 2;
                    register u32 low_mask asm("r0") = 3;
                    low_mask &= low;
                    low_mask |= new_low;
                    bytes = reinterpret_cast<u8 *>(&frame);
                    bytes[0xB] = low_mask;
                    register u32 new_high asm("r1") = (y_high >> 22) & mask;
                    register u32 preserved asm("r2") = reinterpret_cast<u16 *>(bytes)[6];
                    register u32 high_mask asm("r0") = 0xFFFFFC00;
                    high_mask &= preserved;
                    high_mask |= new_high;
                    register u8 * destination asm("r3") = reinterpret_cast<u8 *>(&frame);
                    reinterpret_cast<u16 *>(destination)[6] = high_mask;
                    goto tile_adjusted;
                }
        tile_down:
                {
                    register u8 * bytes asm("r5") = reinterpret_cast<u8 *>(&frame);
                    register u32 low asm("r3") = bytes[0xB];
                    register u32 y_low asm("r0") = low >> 2;
                    register u32 y_high asm("r1") = reinterpret_cast<u16 *>(bytes)[6];
                    asm volatile("" : "+r"(y_high));
                    register u32 mask asm("r4") = 0x3FF;
                    y_high = (((y_high & mask) << 6) | y_low) << 16;
                    asm volatile("" : "+r"(y_high));
                    register u32 delta asm("r0") = 0x100000;
                    y_high += delta;
                    register u32 new_low asm("r2") = ((y_high >> 16) & 0x3F) << 2;
                    register u32 low_mask asm("r0") = 3;
                    low_mask &= low;
                    low_mask |= new_low;
                    bytes[0xB] = low_mask;
                    register u32 new_high asm("r1") = (y_high >> 22) & mask;
                    register u32 preserved asm("r2") = reinterpret_cast<u16 *>(bytes)[6];
                    register u32 high_mask asm("r0") = 0xFFFFFC00;
                    high_mask &= preserved;
                    high_mask |= new_high;
                    reinterpret_cast<u16 *>(bytes)[6] = high_mask;
                    goto tile_adjusted;
                }
        tile_left:
            coordinate_word = frame.scratch[0];
            coordinate_x = static_cast<i32>(coordinate_word << 6) >> 16;
            coordinate_x -= 16;
            goto tile_store_x;
        tile_right:
            coordinate_word = frame.scratch[0];
            coordinate_x = static_cast<i32>(coordinate_word << 6) >> 16;
            coordinate_x += 16;
            asm volatile("" : "+r"(coordinate_x));
        tile_store_x:
            {
                register u32 shifted asm("r1") = static_cast<u32>(coordinate_x) << 16;
                shifted >>= 6;
                register u32 preserved asm("r2") = 0xFC0003FF;
                preserved &= coordinate_word;
                preserved |= shifted;
                frame.scratch[0] = preserved;
            }
        tile_adjusted:
            register u32 map asm("r2") = self->location_map;
            register GameObject * game asm("r1") = self->game_object;
            register FarmerTerrainGrid * grid asm("r4") = reinterpret_cast<FarmerTerrainGrid *>(reinterpret_cast<u8 *>(frame.scratch) + 12);
            register FarmerActionInputGameVTable const * methods asm("r3") = FARMER_ACTION_GAME(game);
            register FarmerTerrainGrid * grid_arg asm("r0") = grid;
            register void (*read_grid)(FarmerTerrainGrid *, GameObject *, u32) asm("r3") = methods->get_grid;
            read_grid(grid_arg, game, map);
            register u32 x_data asm("r0") = frame.scratch[0];
            x_data <<= 6;
            register i32 x asm("r5") = static_cast<i32>(x_data) >> 19;
            register u8 * bytes asm("r1") = reinterpret_cast<u8 *>(&frame);
            register u32 low asm("r2") = bytes[0xB] >> 2;
            register u32 high asm("r0") = reinterpret_cast<u16 *>(bytes)[6];
            register u32 mask asm("r1") = 0x3FF;
            high &= mask;
            high <<= 6;
            high |= low;
            high <<= 16;
            register i32 y asm("r3") = static_cast<i32>(high) >> 19;
            register u32 const * properties asm("r2") = *reinterpret_cast<u32 const * volatile *>(frame.scratch + 3);
            register u32 width asm("r0") = grid->width;
            u32 const * tile;
            if (!properties || static_cast<u32>(x) > width || static_cast<u32>(y) > grid->height)
                goto missing_tile;
            {
                register u8 const * indices asm("r1") = grid->indices;
                register u32 index asm("r0") = width * y + x;
                if (indices)
                    index = indices[index];
                tile = properties + index;
                goto check_tile;
            }
        missing_tile:
            tile = 0;
        check_tile:
            if (tile)
                frame.event_id = *tile >> 17;
        }
    }
    {
        register u32 tile_event asm("r2");
        tile_event = frame.event_id;
        asm volatile("" : : "l"(tile_event));
        if (!tile_event) goto no_interaction_tile_event;
    }
dispatch_world_event:
    {
        GameObject * game = self->game_object;
        register FarmerActionInputGameVTable const * table asm("r2") = FARMER_ACTION_GAME(game);
        register u32 index asm("r3") = 0x140;
        asm volatile("" : "+r"(index));
        register FarmerEventDispatch const * method asm("r2") = reinterpret_cast<FarmerEventDispatch const *>(reinterpret_cast<u8 const *>(table) + index);
        register FarmerEventRequest * request asm("r1") = &frame.world_event;
        register u32 no_argument asm("r4") = 0;
        register u32 event_id asm("r5") = frame.event_id;
        frame.world_event.id = event_id;
        request->argument = no_argument;
        register FarmerEventDispatch dispatch asm("r3") = *method;
        dispatch(game, request, 0);
        *frame.state = no_argument;
        return;
    }
no_interaction_tile_event:
    {
        register u32 flag asm("r0") = FarmerHeldItemIsEmptyRaw(&self->farmer->unk_54) << 24;
        register u32 empty asm("r5") = flag >> 24;
        u32 held_slot_empty = empty;
        if (held_slot_empty != 0)
            goto pick_up_item;
        register i32 discard_kind asm("r0") = CheckFarmerHeldItemDiscard(self);
        switch (discard_kind)
        {
        case 2: return;
        case 0:
            {
                register u32 drop_state asm("r0") = 11;
                register u8 * farmer_state asm("r1") = frame.state;
                *farmer_state = drop_state;
                register ThrownItemMotion * motion asm("r1") = &self->thrown_item;
                register u32 vertical_speed asm("r0") = 0x150000;
                motion->x_speed = held_slot_empty;
                motion->y_speed = held_slot_empty;
                motion->z_speed = vertical_speed;
                register u8 const * facing_field asm("r2") = frame.facing;
                register i32 direction asm("r4") = *facing_field;
                register i32 speed asm("r0");
                switch (direction)
                {
                case FACING_LEFT: motion->x_velocity = -0x1AAAA; motion->y_velocity = held_slot_empty; motion->z_velocity = held_slot_empty; break;
                case FACING_RIGHT: motion->x_velocity = 0x1AAAA; motion->y_velocity = held_slot_empty; motion->z_velocity = held_slot_empty; break;
                case FACING_UP:
                    {
                        speed = -0x1AAAA;
                        goto drop_vertical;
                    }
                case FACING_DOWN:
                    {
                        speed = 0x1AAAA;
                    drop_vertical:
                        motion->x_velocity = held_slot_empty;
                        motion->y_velocity = speed;
                        motion->z_velocity = held_slot_empty;
                    }
                    break;
                }
                register GameObject * game asm("r0") = self->game_object;
                INTERACTION_AUDIO_METHOD(game, "r1", "r3")
                SCAN_INTERACTION_AUDIO(discard_channel, discard_end, discard_found, discard_last)

            }
        case 1:
            {
                register u32 next_state asm("r0") = 13;
                asm volatile("" : "+r"(next_state));
                register u8 * farmer_state asm("r1") = frame.state;
                *farmer_state = next_state;
                register u8 const * facing_ptr asm("r2") = frame.facing;
                asm volatile("" : "+r"(facing_ptr));
                register i32 direction asm("r4") = *facing_ptr;
                register ThrownItemMotion * motion asm("r0");
                register i32 lateral asm("r1");
                register i32 height asm("r2");
                if (direction == FACING_UP) goto launch_up;
                if (direction > FACING_UP) goto launch_horizontal;
                if (direction == FACING_DOWN) goto launch_down;
                goto launch_sound;
            launch_horizontal:
                if (direction == FACING_LEFT) goto launch_left;
                if (direction == FACING_RIGHT) goto launch_right;
                goto launch_sound;
            launch_left:
                motion = &self->thrown_item;
                lateral = -0xA0000;
                height = 0xC0000;
                motion->x_speed = lateral;
                asm volatile("" : "+m"(motion->x_speed));
                motion->y_speed = held_slot_empty;
                goto launch_height;
            launch_right:
                motion = &self->thrown_item;
                lateral = 0xA0000;
                height = 0xC0000;
                motion->x_speed = lateral;
                motion->y_speed = held_slot_empty;
                goto launch_height;
            launch_up:
                motion = &self->thrown_item;
                lateral = -0xA0000;
                height = 0xC0000;
                motion->x_speed = held_slot_empty;
                goto launch_store_y;
            launch_down:
                motion = &self->thrown_item;
                lateral = 0xA0000;
                height = 0xC0000;
                motion->x_speed = direction;
            launch_store_y:
                motion->y_speed = lateral;
            launch_height:
                motion->z_speed = height;
            launch_sound:
                register GameObject * game asm("r0") = self->game_object;
                INTERACTION_AUDIO_METHOD(game, "r1", "r3")
                SCAN_INTERACTION_AUDIO(discard_channel, discard_end, discard_found, discard_last)

            }
        discard_found:
            discard_player = *discard_channel;
            goto discard_play;
        case 3:
            {
                register u32 next_state asm("r0") = 12;
                register u8 * farmer_state asm("r5") = frame.state;
                *farmer_state = next_state;
                CancelFarmerItemDisplay(self);
                func_0800F390(&self->farmer->unk_54);
                register u8 * game_state asm("r4") = static_cast<u8 *>(self->game_state);
                register u32 offset asm("r0") = 0x1CCC;
                asm volatile("" : "+r"(offset));
                game_state += offset;
                register Location * current asm("r0") = reinterpret_cast<Location *>(frame.scratch);

                ReadFarmerInteractionLocation(current, self);
                register u8 * destination_state asm("r0") = game_state;
                asm volatile("" : : "r"(destination_state));
                register u8 * location_bytes asm("r1") = reinterpret_cast<u8 *>(frame.scratch);

                memcpy(destination_state, location_bytes, sizeof(Location));
                register GameObject * game asm("r0") = self->game_object;
                asm volatile("" : "+r"(game));
                {
                    register FarmerActionInputGameVTable const * methods asm("r1") = FARMER_ACTION_GAME(game);
                    register void (*refresh)(GameObject *,u32) asm("r2") = methods->refresh_entity;
                    refresh(game, 0x4B);
                }
                game = self->game_object;
                {
                    register FarmerActionInputGameVTable const * methods asm("r1") = FARMER_ACTION_GAME(game);
                    register AEntity * (*lookup)(GameObject *,u32) asm("r2") = methods->get_entity;
                    register AEntity * sprite asm("r0") = lookup(game, 0x4B);
                    if (!sprite)
                        return;
                    register u8 const * facing_ptr asm("r2") = frame.facing;
                    register u32 facing asm("r1") = *facing_ptr;
                    LaunchFarmerSideSprite(sprite, facing);
                }
                game = self->game_object;
                INTERACTION_AUDIO_METHOD(game, "r1", "r3")
                SCAN_INTERACTION_AUDIO(discard_channel, discard_end, discard_found, discard_last)
            }
        discard_last:
            {
                register void ** channel asm("r0") = discard_end - 1;
                discard_player = *channel;
            }
        discard_play:
            AudioPlayer_StartSong(discard_player, 0x6C);
            return;
        default:
            return;
        }
    }
pick_up_item:
    {

            register i32 x asm("r5");
            register u32 keep_4_r2 asm("r2");
            register u32 keep_4_r3 asm("r3");
            asm volatile("" : "=l"(keep_4_r2), "=l"(keep_4_r3));
            x = self->x_q16 >> 16;
            asm volatile("" : "+r"(x) : "l"(keep_4_r2), "l"(keep_4_r3));

            register i32 initial_y asm("r1");
            register u32 keep_5_r0 asm("r0");
            register u32 keep_5_r3 asm("r3");
            asm volatile("" : "=l"(keep_5_r0), "=l"(keep_5_r3));
            initial_y = self->y_q16 >> 16;
            asm volatile("" : "+r"(initial_y) : "l"(keep_5_r0), "l"(keep_5_r3));
        register i32 y asm("sl") = initial_y;
        register u8 const * facing_ptr asm("r3") = frame.facing;
        register i32 facing_pick asm("r4") = *facing_ptr;
        if (facing_pick == FACING_UP) goto pick_up_up;
        if (facing_pick > FACING_UP) goto pick_up_horizontal;
        if (facing_pick == FACING_DOWN) goto pick_up_down;
        goto pick_up_position;
    pick_up_horizontal:
        if (facing_pick == FACING_LEFT) goto pick_up_left;
        if (facing_pick == FACING_RIGHT) goto pick_up_right;
        goto pick_up_position;
    pick_up_up:
        {
            register i32 delta asm("r0") = 16;
            delta = -delta;
            asm volatile("" : "+r"(delta));
            y += delta;
            goto pick_up_position;
        }
    pick_up_down:
        {
            register i32 delta asm("r1") = 16;
            asm volatile("" : "+r"(delta));
            y += delta;
            goto pick_up_position;
        }
    pick_up_left:
        x -= 16;
        goto pick_up_position;
    pick_up_right:
        x += 16;
    pick_up_position:
        register RucksackItem * item asm("r0") = reinterpret_cast<RucksackItem *>(frame.scratch);

        __12RucksackItem(item);
        GameObject * game = self->game_object;
        typedef void (*TileItemPickup)(GameObject *, Location const *, RucksackItem *);
        register TileItemPickup const * pick_up_slot asm("r2") = &FARMER_ACTION_GAME(game)->pick_up_tile_item;
        asm volatile("" : "+r"(pick_up_slot));
        frame.tile_item_vtable_slot = pick_up_slot;
        asm volatile("" : "+m"(frame.tile_item_vtable_slot));
        register u32 map asm("r3") = self->location_map;
        asm volatile("" : "+r"(map));
        register Location * where asm("r1") = reinterpret_cast<Location *>(reinterpret_cast<u8 *>(frame.scratch) + 4);
        asm volatile("" : "+r"(where));
        register u32 low_mask asm("r2") = 0x3FF;
        register u32 map_mask asm("r9") = low_mask;
        asm volatile("" : "+r"(map_mask));
        low_mask = map_mask;
        asm volatile("" : "+r"(low_mask));
        map &= low_mask;
        asm volatile("" : "+r"(map));
        register u16 * pieces asm("r1") = reinterpret_cast<u16 *>(where);
        register u32 old_map asm("r4") = pieces[0];
        asm volatile("" : "+r"(old_map));
        register u32 preserved_bits asm("r6") = 0xFFFFFC00;
        asm volatile("" : "+r"(preserved_bits));
        register u32 written_map asm("r2") = preserved_bits;
        asm volatile("" : "+r"(written_map));
        written_map &= old_map;
        written_map |= map;
        pieces[0] = written_map;
        asm volatile("" : "+m"(pieces[0]));
        register u32 x_shifted asm("r5") = static_cast<u32>(x) << 16;
        asm volatile("" : "+r"(x_shifted));
        register u32 x_unsigned asm("r3") = x_shifted >> 16;
        asm volatile("" : "+r"(x_unsigned) : "r"(x_shifted));
        register u32 x_mask asm("r2") = 63;
        register u32 coordinate_low_mask asm("r12") = x_mask;
        asm volatile("" : "+r"(x_mask), "+r"(coordinate_low_mask));
        register u8 * bytes asm("r1") = reinterpret_cast<u8 *>(where);
        register u32 low_x asm("r3") = (x_unsigned & x_mask) << 2;
        u32 saved_pickup_x = low_x;
        asm volatile("" : : "g"(saved_pickup_x));
        register u32 x_prior asm("r8") = bytes[1];
        asm volatile("" : "+r"(x_prior));
        register u32 preserved_byte_bits asm("r4") = 3;
        asm volatile("" : "+r"(preserved_byte_bits));
        register u32 merged_x asm("r2") = preserved_byte_bits;
        asm volatile("" : "+r"(merged_x));
        register u32 prior_low asm("r3") = x_prior;
        asm volatile("" : "+r"(prior_low));
        merged_x &= prior_low;
        merged_x |= saved_pickup_x;
        bytes[1] = merged_x;
        asm volatile("" : "+m"(bytes[1]));
        x_shifted >>= 22;
        x_shifted &= map_mask;
        asm volatile("" : "+r"(x_shifted));
        register u32 old_x_high asm("r3") = pieces[1];
        asm volatile("" : "+r"(old_x_high));
        register u32 written_x_high asm("r2") = preserved_bits;
        asm volatile("" : "+r"(written_x_high));
        written_x_high &= old_x_high;
        written_x_high |= x_shifted;
        pieces[1] = written_x_high;
        asm volatile("" : "+m"(pieces[1]));
        register i32 input_y asm("r5") = y;
        asm volatile("" : "+r"(input_y));
        register u32 y_shifted asm("r3") = static_cast<u32>(input_y) << 16;
        asm volatile("" : "+r"(y_shifted));
        register u32 y_low asm("r2") = y_shifted >> 16;
        asm volatile("" : "+r"(y_low) : "r"(y_shifted));
        register u32 y_mask asm("r5") = coordinate_low_mask;
        asm volatile("" : "+r"(y_mask));
        y_low &= y_mask;
        y_low <<= 2;
        asm volatile("" : "+r"(y_low));
        register u32 y_prior asm("r5") = bytes[3];
        asm volatile("" : "+r"(y_prior));
        preserved_byte_bits &= y_prior;
        preserved_byte_bits |= y_low;
        bytes[3] = preserved_byte_bits;
        asm volatile("" : "+m"(bytes[3]));
        y_shifted >>= 22;
        register u32 y_high_mask asm("r2") = map_mask;
        asm volatile("" : "+r"(y_high_mask) : "r"(y_shifted));
        y_shifted &= y_high_mask;
        asm volatile("" : "+r"(y_shifted));
        register u32 y_previous_high asm("r2") = pieces[2];
        asm volatile("" : "+r"(y_previous_high));
        preserved_bits &= y_previous_high;
        preserved_bits |= y_shifted;
        pieces[2] = preserved_bits;
        register TileItemPickup const * pickup_method asm("r5") = static_cast<TileItemPickup const *>(frame.tile_item_vtable_slot);
        register TileItemPickup pick_up asm("r3") = *pickup_method;
        asm volatile("" : : "r"(pick_up));
        register RucksackItem * item_destination asm("r2") = reinterpret_cast<RucksackItem *>(frame.scratch);

        pick_up(game, where, item_destination);
        register RucksackItem * item_check asm("r0") = reinterpret_cast<RucksackItem *>(frame.scratch);

        if (!(IsFarmerInteractionItemEmpty(item_check) << 24))
        {
            register FarmerEntity * pickup_farmer asm("r0") = self;
            asm volatile("" : : "r"(pickup_farmer));
            register RucksackItem * held_item asm("r1") = reinterpret_cast<RucksackItem *>(frame.scratch);

            FarmerEntity_LiftRucksackItem(pickup_farmer, held_item, false);
        }
    }
}

#undef SCAN_INTERACTION_AUDIO
#undef INTERACTION_AUDIO_METHOD
#undef FARMER_ACTION_STATE
#undef FARMER_ACTION_ACTOR
#undef FARMER_ACTION_GAME

struct FarmerIdleGameVTable
{
    STRUCT_PAD(0, 0x40);
    AEntity * (*get_entity)(GameObject *, u32);
    STRUCT_PAD(0x44, 0x120);
    u32 (*can_spawn_step_effect)(GameObject *, Location const *);
    STRUCT_PAD(0x124, 0x14C);
    EntitySoundChannels const * (*get_channels)(GameObject *);
};

struct FarmerIdleActorVTable
{
    STRUCT_PAD(0, 0x40);
    void (*cancel_interaction)(AEntity *);
    STRUCT_PAD(0x44, 0x6C);
    void (*refresh_after_exhaustion)(FarmerEntity *);
};

struct FarmerIdleInputFrame
{
    u8 step_interval[4];
    u16 location[3];
    u16 padding;
    FarmerHeldItemKeyWord pressed;
    FarmerHeldItemKeyWord held;
};

EC void ReadFarmerIdleLocation(Location *, AEntity const *) asm("GetLocation__C7AEntity");
EC u32 FarmerCheckSpecialTerrain(FarmerEntity *) asm("IsFarmerOnSpecialTerrain");
EC u32 FarmerGetTimeOfDay(void const *) asm("func_0800E324");
EC u8 const farmer_step_sound_intervals[3] asm("gUnk_080F117F");

#define FARMER_IDLE_GAME(game) (*reinterpret_cast<FarmerIdleGameVTable const **>(game))
#define FARMER_IDLE_ACTOR(actor) (reinterpret_cast<FarmerIdleActorVTable const **>(actor)[5])

EC void HandleFarmerIdleInput(FarmerEntity *, FarmerInputState const *) ENTITY_CODE(0802EC00);
EC void HandleFarmerIdleInput(FarmerEntity * farmer, FarmerInputState const * keys)
{
    FarmerEntity * self = farmer;
    register FarmerInputState const * input asm("sb") = keys;
    FarmerIdleInputFrame frame;
    register u8 * flags asm("r0") = reinterpret_cast<u8 *>(farmer);
    flags += 0xC0;
    register u32 zero asm("r2") = 0;
    *flags = zero;
    flags -= 0x46;
    *flags = zero;

    register u8 * exhaustion asm("r3") = &self->exhaustion_state;
    register u32 exhausted_state asm("r1") = *exhaustion;
    register u32 exhausted_copy asm("r4") = exhausted_state;
    if (exhausted_copy)
    {
        flags -= 0x3E;
        *flags = exhausted_state;
        register u16 * counter asm("r1") = &self->unk_A6;
        register u32 clear asm("r0") = 0;
        *counter = zero;
        *exhaustion = clear;
        register void (*refresh)(FarmerEntity *) asm("r1") = FARMER_IDLE_ACTOR(self)->refresh_after_exhaustion;
        refresh(self);
        return;
    }
    if (FarmerCheckSpecialTerrain(self) << 24)
    {
        register u8 * state asm("r0") = &self->state;
        *state = 0x35;
        state += 0x6A;
        *reinterpret_cast<u16 *>(state) = exhausted_copy;
        return;
    }

    register Farmer * inventory asm("r0") = self->farmer;
    register HeldItem * held_item asm("r4") = &inventory->unk_54;
    if (FarmerHeldItemIsEmptyRaw(held_item) << 24)
        goto read_movement;
    if (func_0800F204(held_item) != 5)
        goto read_movement;
    {
        register int item_id asm("r1") = func_0800F360(held_item);
        register u8 * state asm("r1");
        register u32 timer_reset asm("r2");
        register u32 next_state asm("r0");
        switch (item_id)
        {
        case 0x15F:
            {
                register u8 * state asm("sb");
                register u16 * timer asm("r6");
                register ThrownItemMotion * motion asm("r8");
                register u8 * state_timer asm("sl");
                {
                    GameObject * game;
                    register EntitySoundChannels const * (*get_channels)(GameObject *) asm("r1");
                    EntitySoundChannels const * channels;
                    register void ** channel asm("r4");
                    register void ** end asm("r5");
                    register void * player asm("r0");
                    goto find_channels_use;
                found_channel_use:
                    player = *channel;
                    goto play_song_use;
                find_channels_use:
                    game = (self)->game_object;
                    get_channels = FARMER_IDLE_GAME(game)->get_channels;
                    channels = get_channels(game);
                    channel = channels->begin;
                    end = channels->end;
                    {
                        register u32 state_offset asm("r3") = 0x3C;
                        asm volatile("" : "+r"(state_offset));
                        state_offset += reinterpret_cast<u32>(self);
                        state = reinterpret_cast<u8 *>(state_offset);
                        timer = &self->unk_A6;
                        register u32 throw_offset asm("r0") = 0x88;
                        throw_offset += reinterpret_cast<u32>(self);
                        motion = reinterpret_cast<ThrownItemMotion *>(throw_offset);
                        register u32 timer_offset asm("r1") = 0x3D;
                        timer_offset += reinterpret_cast<u32>(self);
                        state_timer = reinterpret_cast<u8 *>(timer_offset);
                    }
                    if (channel != end)
                    {
                        do
                        {
                            u32 playing = AudioPlayerIsPlayingRaw(*channel) << 24;
                            if (playing == 0)
                                goto found_channel_use;
                            ++channel;
                        } while (channel != end);
                    }
                    {
                        register void ** final_channel asm("r0") = end - 1;
                        player = *final_channel;
                    }
                play_song_use:
                    AudioPlayer_StartSong(player, 0xB6);
                }
                func_0800F390(&self->farmer->unk_54);
                register i32 speed asm("r0") = -0x80000;
                register u32 zero asm("r2") = 0;
                register u32 height asm("r1") = 0x110000;
                register ThrownItemMotion * move asm("r3") = motion;
                move->x_speed = speed;
                move->y_speed = zero;
                move->z_speed = height;
                register u32 use_state asm("r0") = 0x2F;
                register u8 * state_address asm("r1") = state;
                *state_address = use_state;
                *timer = zero;
                register u32 duration asm("r0") = 120;
                register u8 * timer_address asm("r2") = state_timer;
                *timer_address = duration;
                return;
            }
        case 0xB: case 0x62: case 0x6B: case 0xF7: case 0x155: case 0x180: case 0x18C: case 0x191: case 0x1A6:
            {
                state = &self->state;
                timer_reset = 0;
                next_state = 0x38;
                goto use_item;
            case 0xD6: case 0x122: case 0x167: case 0x1CD:
                state = &self->state;
                timer_reset = 0;
                next_state = 0x39;
            use_item:
                *state = next_state;
                self->unk_A6 = timer_reset;
                ++state;
                asm volatile("" : "+r"(state));
                *state = 30;
                func_0800F390(&self->farmer->unk_54);
                return;
            }
        }
    }
read_movement:
    register u8 * motion_addr asm("r0") = reinterpret_cast<u8 *>(self);
    motion_addr += 0x3e;
    register u32 initial_motion asm("r5") = *motion_addr;
    register u8 * motion_state asm("r8") = motion_addr;
    register u16 * idle_ticks asm("r6");
    if (initial_motion == 0 && (FarmerHeldItemIsEmptyRaw(held_item) << 24))
    {
        register u16 * idle_ticks_address asm("r0") = &self->unk_A6;
        register u32 elapsed asm("r2") = *idle_ticks_address;
        register u32 threshold asm("r1") = 600;
        idle_ticks = idle_ticks_address;
        if (elapsed > threshold)
        {
            self->state = 0x17;
            *idle_ticks = initial_motion;
            return;
        }
        register u32 next_ticks asm("r0") = elapsed + 1;
        *idle_ticks = next_ticks;
    }
    else
    {
        register u16 * idle_ticks_address asm("r1") = &self->unk_A6;
        *idle_ticks_address = 0;
        idle_ticks = idle_ticks_address;
    }

    register u8 * motion_address asm("r3") = motion_state;
    register i32 last_motion asm("r1") = *motion_address;
    if (last_motion == 0)
    {
        register u8 * step_count asm("r0") = reinterpret_cast<u8 *>(self) + 0xC4;
        *step_count = last_motion;
    }
    else
    {
        if (last_motion < 0)
            goto read_buttons;
        if (last_motion > 2)
            goto read_buttons;
        register u8 * step_count asm("r1") = reinterpret_cast<u8 *>(self) + 0xC4;
        ++*step_count;
    }

read_buttons:
    register u32 x asm("r2");
    register u32 y asm("r4");
    register u32 run_key asm("r3");
    register void ** day_channel asm("r4");
    register void * day_player asm("r0");
    register u32 empty_keys asm("r2") = 0;
    frame.held.word = empty_keys;
    frame.pressed.word = empty_keys;
    if (input)
    {
        frame.held.word = input->held;
        register FarmerInputState const * pressed_input asm("r1") = input;
        register u32 pressed_bits asm("r0");
        pressed_bits = pressed_input->pressed;
        asm volatile("" : "+l"(pressed_bits));
        frame.pressed.word = pressed_bits;
    }
    {
        register u8 * motion_output asm("r3") = motion_state;
        *motion_output = empty_keys;
        asm volatile("" : "+m"(*motion_output) : : "memory");
    }

    register FarmerHeldItemKeyWord * pressed asm("r4") = &frame.pressed;
    register u32 buttons asm("r3") = pressed->bytes[0];
    register u32 a_initial asm("r5") = INPUT_A;
    register u32 a_pressed asm("r1") = a_initial;
    a_pressed &= buttons;
    if (a_pressed)
    {
        self->state = a_initial;
        *idle_ticks = empty_keys;
    }
    else
    {
        register u32 b_initial asm("r2") = INPUT_B;
        register u32 b_mask asm("r0") = b_initial;
        b_mask &= buttons;
        register u32 b_pressed asm("r3") = static_cast<u8>(b_mask);
        if (static_cast<u8>(b_pressed))
        {
            self->state = b_initial;
            *idle_ticks = a_pressed;
            if (FarmerHeldItemIsEmptyRaw(&self->farmer->unk_54) << 24)
                PrepareFarmerToolUse(self);
        }
        else
        {
            register u32 high_pressed asm("r1") = pressed->bytes[1];
            asm volatile("" : "+r"(high_pressed));
            register u32 l_mask asm("r0") = b_initial;
            asm volatile("" : "+r"(l_mask) : "r"(b_initial));
            u32 l_pressed = l_mask & high_pressed;
            if (l_pressed)
            {
                self->state = 3;
                *idle_ticks = b_pressed;
            }
            else
            {
                register u32 held asm("r1") = frame.held.word;
                register u32 directions asm("r2") = held << 25;
                directions >>= 31;
                register u32 bit asm("r0") = held << 24;
                directions |= bit >> 31;
                bit = held << 26;
                bit >>= 31;
                bit |= directions;
                directions = held << 27;
                directions >>= 31;
                directions |= bit;
                held <<= 23;
                held >>= 31;
                held |= directions;
                if (held != 0)
                {
                    register u8 * facing_address asm("r2") = &self->facing;
                    register u32 direction asm("r5") = *facing_address;
                    register u32 old_direction asm("r4") = direction;
                    register FarmerHeldItemKeyWord * directional_keys_address asm("r1") = &frame.held;
                    register u32 directional_keys asm("r3") = directional_keys_address->bytes[0];
                    register u32 left_mask asm("r0") = INPUT_LEFT;
                    register u8 * facing asm("r6");
                    register FarmerHeldItemKeyWord * held_keys asm("sb");
                    left_mask &= directional_keys;
                    asm volatile("" : "+l"(left_mask));
                    facing = facing_address;
                    asm volatile("" : "+l"(facing));
                    held_keys = directional_keys_address;
                    asm volatile("" : "+r"(held_keys));
                    u32 left_test = left_mask;
                    asm volatile("" : "+r"(left_test) : "r"(facing), "r"(held_keys));
                    if (left_test)
                        direction = FACING_LEFT;
                    else if (directional_keys & INPUT_RIGHT)
                        direction = FACING_RIGHT;
                    else if (directional_keys & INPUT_UP)
                        direction = FACING_UP;
                    else if (directional_keys & INPUT_DOWN)
                        direction = FACING_DOWN;
                    if (direction != old_direction)
                    {
                        self->unk_B3 = false;
                        register u32 slot asm("r4") = 0x2B;
                        do
                        {
                            GameObject * game = self->game_object;
                            register AEntity * (*get_entity)(GameObject *, u32) asm("r2") = FARMER_IDLE_GAME(game)->get_entity;
                            register AEntity * entity asm("r2") = get_entity(game, slot);
                            if (entity)
                            {
                                register void (*cancel_interaction)(AEntity *) asm("r1") = FARMER_IDLE_ACTOR(entity)->cancel_interaction;
                                cancel_interaction(entity);
                            }
                            ++slot;
                        } while (slot <= 0x45);
                    }
                    if (*facing != direction)
                        self->SetAnimFacing(direction);
                    x = *facing;
                    if (x == FACING_UP)
                        goto face_up;
                    if (static_cast<i32>(x) > FACING_UP)
                        goto horizontal;
                    if (x == FACING_DOWN)
                        goto face_down;
                    goto face_left;
                horizontal:
                    if (x == FACING_LEFT) goto face_left;
                    if (x == FACING_RIGHT) goto face_right;
                face_left:
                    x = 0xFF;
                    y = 0;
                    goto have_speed;
                face_right:
                    x = 1;
                    y = 0;
                    goto have_speed;
                face_up:
                    x = 0;
                    y = 0xFF;
                    goto have_speed;
                face_down:
                    x = 0;
                    y = 1;
                have_speed:
                    asm volatile("" : "+r"(x), "=r"(y) : "r"(y));
                    u32 run_setting = *reinterpret_cast<u8 const *>(reinterpret_cast<u8 const *>(self->game_state) + 0x2210);
                    run_setting <<= 31;
                    asm volatile("" : "+r"(run_setting));
                    if (run_setting)
                        goto default_fast;
                    {
                        register FarmerHeldItemKeyWord const volatile * held_snapshot asm("r3") = held_keys;
                        asm volatile("" : "+l"(held_snapshot));
                        register u32 held_high_byte asm("r1") = held_snapshot->bytes[1];
                        asm volatile("" : "+l"(held_high_byte));
                        run_key = 1;
                        asm volatile("" : "+l"(run_key));
                        register u32 run_key_pressed asm("r0") = run_key;
                        run_key_pressed &= held_high_byte;
                        asm volatile("" : "+l"(run_key_pressed));
                        u32 keys = run_key_pressed;
                        asm volatile("" : "+r"(keys));
                        if (!keys)
                            goto walk;
                        goto run;
                    }
                default_fast:
                    {
                        register FarmerHeldItemKeyWord const volatile * held_snapshot asm("r3") = held_keys;
                        asm volatile("" : "+l"(held_snapshot));
                        register u32 held_high_byte asm("r1") = held_snapshot->bytes[1];
                        asm volatile("" : "+l"(held_high_byte));
                        run_key = 1;
                        asm volatile("" : "+l"(run_key));
                        register u32 run_key_pressed asm("r0") = run_key;
                        run_key_pressed &= held_high_byte;
                        asm volatile("" : "+l"(run_key_pressed));
                        u32 keys = run_key_pressed;
                        asm volatile("" : "+r"(keys));
                        if (keys)
                            goto walk;
                    }
                run:
                    {
                        register u32 run_motion asm("r0") = 2;
                        register u8 * motion_addr asm("r1") = motion_state;
                        *motion_addr = run_motion;
                    }
                    {
                        register u32 shifted_x asm("r0") = x << 25;
                        x = shifted_x >> 24;
                        register u32 shifted_y asm("r0") = y << 25;
                        y = shifted_y >> 24;
                    }
                    goto speed_output;
                }
            }
        }
    }
    goto step_effects;
found_channel_day:
    day_player = *day_channel;
    goto play_song_day;
walk:
    {
        register u8 * motion_out asm("r0") = motion_state;
        *motion_out = run_key;
    }
speed_output:
    {
        register i32 x_speed asm("r0") = x << 24;
        self->x_speed_q16 = x_speed >> 8;
        register i32 y_speed asm("r0") = y << 24;
        self->y_speed_q16 = y_speed >> 8;
    }
step_effects:
    {
        memcpy(frame.step_interval, farmer_step_sound_intervals, 3);
        register u8 * step_motion asm("r1") = motion_state;
        register u32 step_type asm("r0") = *step_motion;
        register u32 interval asm("r1") = frame.step_interval[step_type];
        if (!interval)
            goto after_step_sound;
        {
            register u8 * step_count asm("r2") = reinterpret_cast<u8 *>(self) + 0xC4;
            if (*step_count < interval)
                goto after_step_sound;
            *step_count = 0;
            register GameObject * game asm("r6") = self->game_object;
            register u32 (*const * check)(GameObject *, Location const *) asm("r4") = &FARMER_IDLE_GAME(game)->can_spawn_step_effect;
            register Location * location asm("r5") = reinterpret_cast<Location *>(frame.location);
            ReadFarmerIdleLocation(location, self);
            register u32 (*can_spawn)(GameObject *, Location const *) asm("r2") = *check;
            if (!(can_spawn(game, location) << 24))
                goto other_step_sound;
            FarmerEffectGraphics * graphics = reinterpret_cast<FarmerEffectGraphics *>(reinterpret_cast<void **>(self)[4]);
            if (graphics)
                SpawnFarmerStepEffect(graphics);
            if (FarmerGetTimeOfDay(static_cast<u8 *>(self->game_state) + 0x10) == 3)
                goto night_step_sound;
            {
                register u32 method_offset asm("r3");
                register FarmerIdleGameVTable const * methods asm("r1");
                GameObject * game;
                register EntitySoundChannels const * (*get_channels)(GameObject *) asm("r1");
                EntitySoundChannels const * channels;
                register void ** end asm("r5");
                game = (self)->game_object;
                methods = FARMER_IDLE_GAME(game);
                method_offset = 0xA6;
                asm volatile("" : "+l"(method_offset) : "r"(methods));
                method_offset <<= 1;
                asm volatile("" : "+l"(method_offset));
                get_channels = *reinterpret_cast<EntitySoundChannels const * (**)(GameObject *)>(reinterpret_cast<u32>(methods) + method_offset);
                channels = get_channels(game);
                day_channel = channels->begin;
                end = channels->end;

                if (day_channel != end)
                {
                    do
                    {
                        u32 playing = AudioPlayerIsPlayingRaw(*day_channel) << 24;
                        if (playing == 0)
                            goto found_channel_day;
                        ++day_channel;
                    } while (day_channel != end);
                }
                {
                    register void ** final_channel asm("r0") = end - 1;
                    day_player = *final_channel;
                }
            }
        }
    }
play_song_day:
    AudioPlayer_StartSong(day_player, 0xD0);
    goto after_step_sound;
night_step_sound:
    {
        GameObject * game;
        register EntitySoundChannels const * (*get_channels)(GameObject *) asm("r1");
        EntitySoundChannels const * channels;
        register void ** channel asm("r4");
        register void ** end asm("r5");
        register void * player asm("r0");
        goto find_channels_night;
    found_channel_night:
        player = *channel;
        goto play_song_night;
    find_channels_night:
        game = (self)->game_object;
        get_channels = FARMER_IDLE_GAME(game)->get_channels;
        channels = get_channels(game);
        channel = channels->begin;
        end = channels->end;

        if (channel != end)
        {
            do
            {
                u32 playing = AudioPlayerIsPlayingRaw(*channel) << 24;
                if (playing == 0)
                    goto found_channel_night;
                ++channel;
            } while (channel != end);
        }
        {
            register void ** final_channel asm("r0") = end - 1;
            player = *final_channel;
        }
    play_song_night:
        AudioPlayer_StartSong(player, 0xD1);
    }
    goto after_step_sound;
other_step_sound:
    {
        register u32 method_offset asm("r3");
        register FarmerIdleGameVTable const * methods asm("r1");
        GameObject * game;
        register EntitySoundChannels const * (*get_channels)(GameObject *) asm("r1");
        EntitySoundChannels const * channels;
        register void ** channel asm("r4");
        register void ** end asm("r5");
        register void * player asm("r0");
        goto find_channels_other;
    found_channel_other:
        player = *channel;
        goto play_song_other;
    find_channels_other:
        game = (self)->game_object;
        methods = FARMER_IDLE_GAME(game);
        method_offset = 0xA6;
        asm volatile("" : "+l"(method_offset) : "r"(methods));
        method_offset <<= 1;
        get_channels = *reinterpret_cast<EntitySoundChannels const * (**)(GameObject *)>(reinterpret_cast<u32>(methods) + method_offset);
        channels = get_channels(game);
        channel = channels->begin;
        end = channels->end;

        if (channel != end)
        {
            do
            {
                u32 playing = AudioPlayerIsPlayingRaw(*channel) << 24;
                if (playing == 0)
                    goto found_channel_other;
                ++channel;
            } while (channel != end);
        }
        {
            register void ** final_channel asm("r0") = end - 1;
            player = *final_channel;
        }
    play_song_other:
        AudioPlayer_StartSong(player, 0xD0);
    }
after_step_sound:
    register u32 count_steps asm("r1") = 0;
    if (self->unk_24 == 0)
        count_steps = 1;
    if (count_steps)
    {
        register u8 * step_motion asm("r1") = motion_state;
        register u32 speed asm("r0") = *step_motion;
        if (speed == 1)
        {
            TickFarmerPedometer(self);
            return;
        }
        if (speed != 2)
            return;
        TickFarmerPedometer(self);
        TickFarmerPedometer(self);
    }
}

#undef FARMER_IDLE_ACTOR
#undef FARMER_IDLE_GAME
