#include "entity_actor.hh"
#include "g1_npc.hh"
#include <stdlib.h>

#define NPC_EXTRA_CODE(address) SECTION(".text.npc_extra." #address)
#define NPC_SIDE_CODE(address) SECTION(".text.npc_side." #address)

struct SideActorRecord
{
    ActorLocation location;
    u16 animation;
    u16 sprite_value;
};

struct RecordedSideActor : AActorEntity
{
    RecordedSideActor();
    SideActorRecord * record;
};

struct MovingNpcEntity : AActorEntity
{
    MovingNpcEntity();
    virtual void ChooseMovement();
    ActorLocation * saved_location;
    Box movement_bounds;
    u16 animation;
    u16 timer;
    u8 hidden;
};

struct RecordedEffectEntity : AEntity
{
    RecordedEffectEntity();
    void * record;
};

struct LaunchedSideSprite : AEntity
{
    LaunchedSideSprite();
    Location * destination;
    i32 height_q16;
    i32 rise_speed;
    u8 variant;
    u8 launched;
    u8 moving;
    u8 padding;
    u16 animation;
};

struct SpriteWorkerNpc : AActorEntity
{
    SpriteWorkerNpc();
    u8 sprite_id;
    STRUCT_PAD(0x31, 0x34);
    Npc * npc;
};

struct SideSpriteFrame
{
    u8 bytes[0x20];
};

inline void * operator new(size_t, SideSpriteFrame * frame) { return frame; }

struct SideSpriteBank
{
    virtual ~SideSpriteBank();
    virtual SpriteAnimationClip GetClip(u32 animation);
    virtual SideSpriteFrame GetFrame(u32 frame);
};
struct SideSpriteServices;
struct SideSpriteServicesVTable
{
    STRUCT_PAD(0x00, 0x40);
    AActorEntity * (*find_entity)(SideSpriteServices *, u32);
    STRUCT_PAD(0x44, 0x64);
    SideSpriteBank * (*get_bank)(SideSpriteServices *);
    SideSpriteBank * (*get_actor_bank)(SideSpriteServices *);
    SideSpriteBank * (*get_overlay_bank)(SideSpriteServices *);
    SideSpriteBank * (*get_transition_bank)(SideSpriteServices *);
};

struct SideSpriteServices
{
    SideSpriteServicesVTable * vtable;
};

struct LaunchedSpriteRenderer
{
    LaunchedSideSprite * actor;
    void const * vtable;
    u8 renderer[0x40];
};

struct ReactionSpriteRenderer
{
    AEntity * actor;
    void const * vtable;
    u8 renderer[0x28];
    SideSpriteFrame frame;
    u8 refresh_frame;
    STRUCT_PAD(0x51, 0x54);
    u8 reaction;
};

struct SideActorSprite
{
    AActorEntity * actor;
    void const * vtable;
    u8 renderer[0x40];
    u8 overlay_renderer[0x40];
    u8 overlay_offset;
    u8 overlay_padding;
    u8 overlay_flags;
    u8 overlay_visible;
};

struct SideTransitionSprite
{
    SideTransitionSprite(GameObject * game);
    u8 renderer[0x28];
    SideSpriteFrame frame;
    u8 refresh_frame;
    STRUCT_PAD(0x49, 0x4C);
    u32 progress;
    u8 complete;
    u8 hidden;
    ~SideTransitionSprite();
};

struct SideTransitionEffects
{
    AEntity * actor;
    STRUCT_PAD(0x04, 0x0C);
    SmartPtr<SideTransitionSprite> sprite;
};

struct ActivityNpcRecord : Actor
{
    ActivityNpcRecord();
    u32 padding;
    u32 activity;
};

struct ActivityNpcEntity : AActorEntity
{
    ActivityNpcEntity();
    STRUCT_PAD(0x30, 0x34);
    ActivityNpcRecord * npc;
};

struct NpcPauseState
{
    u32 timer : 16;
    u32 chance : 7;
    u32 alternate : 1;
    u32 target : 8;
};

struct ActivitySpriteOverlay
{
    ActivitySpriteOverlay(GameObject * game);
    u8 renderer[0x28];
    SideSpriteFrame frame;
    u8 refresh_frame;
};

struct ActivityRenderSprite
{
    GameObject * game;
    u16 animation;
    STRUCT_PAD(0x06, 0x08);
    u8 renderer[0x40];
    ActivitySpriteOverlay * overlay;
    void const * vtable;
};

struct NpcWorldState
{
    STRUCT_PAD(0x00, 0x1CD4);
    u8 roster[4];
};

struct ArrivalReactionNpc : AActorEntity
{
    ArrivalReactionNpc();
    Npc * npc;
    NpcWorldState * world;
    STRUCT_PAD(0x38, 0x48);
    u16 arrival_timer;
};

struct ArrivalAnimationChoice
{
    u8 weight;
    u8 padding;
    u16 animation;
};

struct ArrivalAnimationChoices
{
    ArrivalAnimationChoice entries[4];
};

EC u8 const vtable_unk_080E6828[];
EC u8 const vtable_unk_080E7328[];
EC u8 const vtable_unk_080E73FC[];
EC u8 const vtable_unk_080E73B4[];
EC u8 const vtable_unk_080E68C8[];
EC u8 const vtable_unk_080E73E8[];
EC u8 const vtable_unk_080E7478[];
EC u8 const vtable_unk_080E7430[];
EC u8 const vtable_unk_080E602C[];
EC u8 const vtable_unk_080E68B4[];
EC u8 const vtable_unk_080E76BC[];
EC u8 const gUnk_080F14F0[];
EC void ItemCollection_SetExtraItems(void * record, Location const & location);
EC void SetNpcWalkingAnimation(AActorEntity * self, u32 animation);
EC u8 * GetActiveRosterNpc(u8 * roster);
EC u32 GetNamedNpcAgeDays(u8 * npc);
EC bool HasNamedNpcFlag(G1NamedNpcState * npc);
EC bool IsNpcAtNorthernExit(AEntity * self);
EC void RefreshArrivalReactionAnimations(ArrivalReactionNpc * self);
EC void func_08034EB4(AActorEntity * self, u32 reason);
EC SideActorSprite * InitializeSideActorSpriteLayers(SideActorSprite * self, AActorEntity * actor, u32 layer, u32 palette, u32 overlay_offset, u32 overlay_flags, u32 overlay_visible, bool hidden);
EC void InitializeEntityBase(AEntity * self, GameObject * game, Location const & location) asm("__7AEntityP10GameObjectRC8Location");
EC bool DogEntity_IsCarryingTarget(AActorEntity * self);
EC void StartLaunchedSpriteMovement(LaunchedSideSprite * self, i32 x, i32 y, u32 animation, u32 facing);
EC void func_080A4A00(void * renderer, SideSpriteBank * bank, u32 animation, GameObject * game, u32 layer, u32 palette, bool flags);
EC void InitializeMapSpriteSinglePalette(void * renderer, GameObject * game, u32 layer, u32 palette);
EC void func_080A49A0(void * renderer, SideSpriteBank * bank, u32 animation, GameObject * game, u32 layer, u32 palette, u32 priority, bool flags);
EC void DestroyMapSpriteResource(void * self, u32 flags);
EC void func_0809C0C8(ActivityNpcRecord * self, NpcPauseState const & state);
EC void func_0809C068(ActivityNpcRecord * self, u32 activity);
EC void func_08032384(AActorEntity * self, u32 activity, u32 flags);
EC void AEntity_PlayAnimationSound(AEntity * self, u32 sound);
EC u32 RollEntityMovementRandom(u32 limit);

inline SideTransitionSprite::~SideTransitionSprite() { DestroyMapSpriteResource(this, 2); }

inline ActivitySpriteOverlay::ActivitySpriteOverlay(GameObject * game)
{
    SideSpriteServices * services = reinterpret_cast<SideSpriteServices *>(game);
    SideSpriteBank * bank = services->vtable->get_actor_bank(services);
    InitializeMapSpriteSinglePalette(this, game, 2, 15);
    reinterpret_cast<void const **>(this)[9] = vtable_unk_080E602C;
    new (&frame) SideSpriteFrame(bank->GetFrame(bank->GetClip(2330).keyframes[0].frame));
    refresh_frame = 1;
}

inline SideTransitionSprite::SideTransitionSprite(GameObject * game)
{
    SideSpriteServices * services = reinterpret_cast<SideSpriteServices *>(game);
    SideSpriteBank * bank = services->vtable->get_transition_bank(services);
    InitializeMapSpriteSinglePalette(this, game, 6, 27);
    reinterpret_cast<void const **>(this)[9] = vtable_unk_080E602C;
    new (&frame) SideSpriteFrame(bank->GetFrame(bank->GetClip(0).keyframes[0].frame));
    refresh_frame = 1;
    progress = 0x1200000;
    complete = 0;
    hidden = 0;
}

EC void DestroyRecordedSideActor(RecordedSideActor * self, u32 flags) NPC_SIDE_CODE(0803295C);
EC void DestroyRecordedSideActor(RecordedSideActor * self, u32 flags)
{
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6828;
    self->record->location = self->GetLocation();
    u32 animation = self->anim_id;
    self->record->animation = animation;
    self->AEntity::~AEntity();
    if (flags & 1)
        delete reinterpret_cast<u8 *>(self);
}

EC void DestroyMovingNpc(MovingNpcEntity * self, u32 flags) NPC_EXTRA_CODE(08037048);
EC void DestroyMovingNpc(MovingNpcEntity * self, u32 flags)
{
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E7328;
    *self->saved_location = self->GetLocation();
    self->AEntity::~AEntity();
    if (flags & 1)
        delete reinterpret_cast<u8 *>(self);
}

EC void DestroyTimedReactionEntity(RecordedEffectEntity * self, u32 flags) NPC_EXTRA_CODE(08037E48);
EC void DestroyTimedReactionEntity(RecordedEffectEntity * self, u32 flags)
{
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E73FC;
    ItemCollection_SetExtraItems(self->record, self->GetLocation());
    self->AEntity::~AEntity();
    if (flags & 1)
        delete reinterpret_cast<u8 *>(self);
}

EC void DestroyLaunchedSideSprite(LaunchedSideSprite * self, u32 flags) NPC_EXTRA_CODE(08038098);
EC void DestroyLaunchedSideSprite(LaunchedSideSprite * self, u32 flags)
{
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E73B4;
    *self->destination = self->GetLocation();
    self->AEntity::~AEntity();
    if (flags & 1)
        delete reinterpret_cast<u8 *>(self);
}

EC UnknownEntityThing * CreateArrivalReactionSprite(ArrivalReactionNpc * self) NPC_EXTRA_CODE(08036F0C);
EC UnknownEntityThing * CreateArrivalReactionSprite(ArrivalReactionNpc * self)
{
    u8 * npc = GetActiveRosterNpc(self->world->roster);
    if (npc != nullptr)
    {
        RefreshArrivalReactionAnimations(self);
        u32 palette = HasNamedNpcFlag(reinterpret_cast<G1NamedNpcState *>(npc)) ? 4 : 1;
        struct Storage { u8 bytes[0x8C]; };
        return reinterpret_cast<UnknownEntityThing *>(InitializeSideActorSpriteLayers(reinterpret_cast<SideActorSprite *>(new Storage), self, 4, 27, 0, palette, 0, false));
    }
    return nullptr;
}

EC void UpdateArrivalReactionAnimation(ArrivalReactionNpc * self, u32 reason) NPC_EXTRA_CODE(08036F68);
EC void UpdateArrivalReactionAnimation(ArrivalReactionNpc * self, u32 reason)
{
    if (self->unk_10.Get() != nullptr)
    {
        u8 * npc = GetActiveRosterNpc(self->world->roster);
        if (npc != nullptr && GetNamedNpcAgeDays(npc) <= 59 && IsNpcAtNorthernExit(self))
        {
            if (self->arrival_timer != 0)
                self->arrival_timer--;
            else
            {
                ArrivalAnimationChoices choices = *reinterpret_cast<ArrivalAnimationChoices const *>(gUnk_080F14F0 + 0x24);
                u32 selection = rand() % 100;
                for (u32 i = 0; i < 4; i++)
                {
                    ArrivalAnimationChoice * choice = choices.entries + i;
                    if (selection < choice->weight)
                    {
                        SetNpcWalkingAnimation(self, choice->animation);
                        break;
                    }
                    selection -= choice->weight;
                }
                self->arrival_timer = rand() % 120 + 120;
            }
        }
    }
    func_08034EB4(self, reason);
}

EC bool IsMovingNpcAnimationComplete(AEntity * self) NPC_EXTRA_CODE(080370D8);
EC bool IsMovingNpcAnimationComplete(AEntity * self)
{
    UnknownEntityThing * sprite = self->unk_10.Get();
    if (sprite != nullptr)
    {
        bool complete = false;
        SpriteAnimator * animator = &sprite->sprite_animator;
        bool playing = false;
        if (animator->remaining_q8 != 0)
            playing = animator->speed_q8 != 0;
        if (!playing || WillSpriteAnimationLoop(animator))
            complete = true;
        return complete;
    }
    return true;
}

EC void DestroySpriteWorker(SpriteWorkerNpc * self, u32 flags) NPC_SIDE_CODE(08033A2C);
EC void DestroySpriteWorker(SpriteWorkerNpc * self, u32 flags)
{
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E68C8;
    self->npc->SetLocation(self->GetLocation());
    self->AEntity::~AEntity();
    if (flags & 1)
        delete reinterpret_cast<u8 *>(self);
}

EC LaunchedSideSprite * InitializeLaunchedSideSprite(LaunchedSideSprite * self, GameObject * game, Location * destination) NPC_EXTRA_CODE(08038028);
EC LaunchedSideSprite * InitializeLaunchedSideSprite(LaunchedSideSprite * self, GameObject * game, Location * destination)
{
    InitializeEntityBase(self, game, *destination);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E73B4;
    self->destination = destination;
    self->height_q16 = 0;
    self->launched = 0;
    self->moving = 0;
    self->animation = 49;
    SideSpriteServices * services = reinterpret_cast<SideSpriteServices *>(game);
    AActorEntity * dog = services->vtable->find_entity(services, 43);
    if (dog != nullptr && dog->location_map == self->location_map && DogEntity_IsCarryingTarget(dog))
        StartLaunchedSpriteMovement(self, dog->x_q16, dog->y_q16, dog->anim_id, dog->facing);
    return self;
}

EC LaunchedSpriteRenderer * InitializeLaunchedSpriteRenderer(LaunchedSpriteRenderer * self, LaunchedSideSprite * actor) NPC_EXTRA_CODE(0803853C);
EC LaunchedSpriteRenderer * InitializeLaunchedSpriteRenderer(LaunchedSpriteRenderer * self, LaunchedSideSprite * actor)
{
    self->actor = actor;
    self->vtable = vtable_unk_080E73E8;
    SideSpriteServices * services = reinterpret_cast<SideSpriteServices *>(actor->game_object);
    SideSpriteBank * bank = services->vtable->get_bank(services);
    GameObject * game = actor->game_object;
    void * renderer = self->renderer;
    u32 animation = actor->animation;
    func_080A4A00(renderer, bank, animation, game, 2, 7, false);
    return self;
}

EC ReactionSpriteRenderer * InitializeReactionSpriteRenderer(ReactionSpriteRenderer * self, AEntity * actor) NPC_EXTRA_CODE(08037D08);
EC ReactionSpriteRenderer * InitializeReactionSpriteRenderer(ReactionSpriteRenderer * self, AEntity * actor)
{
    self->actor = actor;
    self->vtable = vtable_unk_080E7478;
    SideSpriteServices * services = reinterpret_cast<SideSpriteServices *>(actor->game_object);
    SideSpriteBank * bank = services->vtable->get_bank(services);
    GameObject * game = actor->game_object;
    void * renderer = self->renderer;
    InitializeMapSpriteSinglePalette(renderer, game, 2, 10);
    reinterpret_cast<void const **>(renderer)[9] = vtable_unk_080E602C;
    new (&self->frame) SideSpriteFrame(bank->GetFrame(bank->GetClip(125).keyframes[0].frame));
    self->refresh_frame = 1;
    return self;
}

EC ReactionSpriteRenderer * InitializeTimedReactionSpriteRenderer(ReactionSpriteRenderer * self, AEntity * actor) NPC_EXTRA_CODE(08037EF0);
EC ReactionSpriteRenderer * InitializeTimedReactionSpriteRenderer(ReactionSpriteRenderer * self, AEntity * actor)
{
    self->actor = actor;
    self->vtable = vtable_unk_080E7430;
    SideSpriteServices * services = reinterpret_cast<SideSpriteServices *>(actor->game_object);
    SideSpriteBank * bank = services->vtable->get_bank(services);
    GameObject * game = actor->game_object;
    void * renderer = self->renderer;
    InitializeMapSpriteSinglePalette(renderer, game, 2, 8);
    reinterpret_cast<void const **>(renderer)[9] = vtable_unk_080E602C;
    new (&self->frame) SideSpriteFrame(bank->GetFrame(bank->GetClip(53).keyframes[0].frame));
    self->refresh_frame = 1;
    self->reaction = 0;
    return self;
}

EC void CreateSideTransitionSprite(SideTransitionEffects * self) NPC_EXTRA_CODE(08038DF0);
EC void CreateSideTransitionSprite(SideTransitionEffects * self)
{
    self->sprite = new SideTransitionSprite(self->actor->game_object);
}

EC SideActorSprite * InitializeSideActorSpriteLayers(SideActorSprite * self, AActorEntity * actor, u32 layer, u32 palette, u32 overlay_offset, u32 overlay_flags, u32 overlay_visible, bool hidden) NPC_SIDE_CODE(080324BC);
EC SideActorSprite * InitializeSideActorSpriteLayers(SideActorSprite * self, AActorEntity * actor, u32 layer, u32 palette, u32 overlay_offset, u32 overlay_flags, u32 overlay_visible, bool hidden)
{
    self->actor = actor;
    self->vtable = vtable_unk_080E68B4;
    {
        SideSpriteServices * services = reinterpret_cast<SideSpriteServices *>(actor->game_object);
        SideSpriteBank * bank = services->vtable->get_actor_bank(services);
        u32 animation = actor->anim_id + actor->facing;
        GameObject * game = actor->game_object;
        void * renderer = self->renderer;
        func_080A4A00(renderer, bank, animation, game, layer, palette, hidden);
    }
    {
        SideSpriteServices * services = reinterpret_cast<SideSpriteServices *>(actor->game_object);
        SideSpriteBank * bank = services->vtable->get_overlay_bank(services);
        GameObject * game = actor->game_object;
        void * renderer = self->overlay_renderer;
        func_080A4A00(renderer, bank, 0, game, 2, 14, false);
    }
    self->overlay_offset = overlay_offset;
    self->overlay_padding = 0;
    u8 * flags = &self->overlay_flags;
    *flags = overlay_flags << 2;
    self->overlay_visible = overlay_visible;
    return self;
}

EC SideActorSprite * InitializePrioritizedSideActorSprite(SideActorSprite * self, AActorEntity * actor, u32 layer, u32 palette, u32 priority, u32 overlay_offset, u32 overlay_flags, u32 overlay_visible, bool hidden) NPC_SIDE_CODE(08032560);
EC SideActorSprite * InitializePrioritizedSideActorSprite(SideActorSprite * self, AActorEntity * actor, u32 layer, u32 palette, u32 priority, u32 overlay_offset, u32 overlay_flags, u32 overlay_visible, bool hidden)
{
    self->actor = actor;
    self->vtable = vtable_unk_080E68B4;
    {
        SideSpriteServices * services = reinterpret_cast<SideSpriteServices *>(actor->game_object);
        SideSpriteBank * bank = services->vtable->get_actor_bank(services);
        u32 animation = actor->anim_id + actor->facing;
        GameObject * game = actor->game_object;
        void * renderer = self->renderer;
        func_080A49A0(renderer, bank, animation, game, layer, palette, priority, hidden);
    }
    {
        SideSpriteServices * services = reinterpret_cast<SideSpriteServices *>(actor->game_object);
        SideSpriteBank * bank = services->vtable->get_overlay_bank(services);
        GameObject * game = actor->game_object;
        void * renderer = self->overlay_renderer;
        func_080A4A00(renderer, bank, 0, game, 2, 14, false);
    }
    self->overlay_offset = overlay_offset;
    self->overlay_padding = 0;
    u8 * flags = &self->overlay_flags;
    *flags = overlay_flags << 2;
    self->overlay_visible = overlay_visible;
    return self;
}

EC void StartNpcPauseActivity(ActivityNpcEntity * self) NPC_EXTRA_CODE(08039DA8);
EC void StartNpcPauseActivity(ActivityNpcEntity * self)
{
    if (self->npc->activity != 1)
    {
        NpcPauseState state;
        state.timer = RollEntityMovementRandom(120) + 240;
        state.chance = 60;
        state.alternate = 0;
        state.target = 0;
        func_0809C0C8(self->npc, state);
        func_0809C068(self->npc, 15);
        func_08032384(self, 2, 0);
        AEntity_PlayAnimationSound(self, 170);
    }
}

EC void StartNpcAlternatePauseActivity(ActivityNpcEntity * self) NPC_EXTRA_CODE(08039E18);
EC void StartNpcAlternatePauseActivity(ActivityNpcEntity * self)
{
    if (self->npc->activity != 1)
    {
        NpcPauseState state;
        state.timer = RollEntityMovementRandom(120) + 240;
        state.chance = 60;
        state.alternate = 1;
        state.target = 0;
        func_0809C0C8(self->npc, state);
        func_0809C068(self->npc, 4);
        func_08032384(self, 2, 0);
        AEntity_PlayAnimationSound(self, 170);
    }
}

EC ActivityRenderSprite * InitializeActivityRenderSprite(ActivityRenderSprite * self, GameObject * game, u32 animation, u32 variant) NPC_EXTRA_CODE(08039E98);
EC ActivityRenderSprite * InitializeActivityRenderSprite(ActivityRenderSprite * self, GameObject * game, u32 animation, u32 variant)
{
    self->vtable = vtable_unk_080E76BC;
    self->game = game;
    self->animation = animation;
    SideSpriteServices * services = reinterpret_cast<SideSpriteServices *>(game);
    SideSpriteBank * bank = services->vtable->get_actor_bank(services);
    func_080A4A00(self->renderer, bank, animation, game, 3, 27, false);
    ActivitySpriteOverlay * overlay = variant == 1 ? new ActivitySpriteOverlay(game) : nullptr;
    self->overlay = overlay;
    return self;
}
