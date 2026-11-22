#include "entity_actor.hh"

#define NPC_EXTRA_CODE(address) SECTION(".text.npc_extra." #address)
#define NPC_SIDE_CODE(address) SECTION(".text.npc_side." #address)

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

struct SpriteWorkerEntity : AActorEntity
{
    SpriteWorkerEntity();
    u8 sprite_id;
    STRUCT_PAD(0x31, 0x34);
    void * record;
    void * work_targets;
    u32 work_context;
    u8 work_state;
    u8 work_pause;
    u8 work_active;
};

struct ActivityAnimationRecord
{
    STRUCT_PAD(0x00, 0x0C);
    u32 activity;
};

struct ActivityAnimationNpc : AActorEntity
{
    ActivityAnimationNpc();
    STRUCT_PAD(0x30, 0x34);
    ActivityAnimationRecord * record;
    u32 animations[5];
};

EC UnknownEntityThing * InitializeSideActorSpriteLayers(UnknownEntityThing * self, AActorEntity * actor, u32 layer, u32 animation, u32 priority, u32 palette, u32 flags, bool visible);
EC void func_08034F00(AActorEntity * self, i32 reason);
EC void RefreshArrivalReactionAnimations(AActorEntity * self);
EC void ClearSpriteWorkPause(SpriteWorkerEntity * self);
EC u16 const gUnk_080F14B8[];

EC UnknownEntityThing * CreateScheduledNpc34Sprite(AActorEntity * self) NPC_EXTRA_CODE(08036E00);
EC UnknownEntityThing * CreateScheduledNpc34Sprite(AActorEntity * self)
{
    struct Storage { u8 bytes[0x8C]; };
    return InitializeSideActorSpriteLayers(reinterpret_cast<UnknownEntityThing *>(new Storage), self, 4, 27, 1, 0, 0, false);
}

EC void RefreshArrivalReactionNpc(AActorEntity * self, i32 reason) NPC_EXTRA_CODE(08036EF0);
EC void RefreshArrivalReactionNpc(AActorEntity * self, i32 reason)
{
    func_08034F00(self, reason);
    if (reason > 1)
        RefreshArrivalReactionAnimations(self);
}

EC void * GetArrivalReactionNpcRecord(ActivityAnimationNpc * self) NPC_EXTRA_CODE(08037004);
EC void * GetArrivalReactionNpcRecord(ActivityAnimationNpc * self)
{
    return self->record;
}

EC void ClearMovingNpcBounds(MovingNpcEntity * self) NPC_EXTRA_CODE(08037250);
EC void ClearMovingNpcBounds(MovingNpcEntity * self)
{
    self->movement_bounds = Box(0, 0, 0, 0, 0, 0);
}

EC void ReactionSpriteIdleNoOp() NPC_EXTRA_CODE(08037D70);
EC void ReactionSpriteIdleNoOp()
{
}

EC u32 GetBusyNpcVariantAnimation() NPC_EXTRA_CODE(080398A0);
EC u32 GetBusyNpcVariantAnimation()
{
    return 2;
}

EC u32 GetActivityNpcAnimation() NPC_EXTRA_CODE(08039E88);
EC u32 GetActivityNpcAnimation()
{
    return 2;
}

EC u32 GetActivityNpcAnimationValue(ActivityAnimationNpc * self) NPC_EXTRA_CODE(08039E8C);
EC u32 GetActivityNpcAnimationValue(ActivityAnimationNpc * self)
{
    ActivityAnimationNpc * selected = reinterpret_cast<ActivityAnimationNpc *>(self->record->activity * sizeof(u32) + reinterpret_cast<u32>(self));
    return selected->animations[0];
}

EC void ResumeSpriteWorker(SpriteWorkerEntity * self) NPC_SIDE_CODE(08033AE4);
EC void ResumeSpriteWorker(SpriteWorkerEntity * self)
{
    self->unk_06 = false;
    self->work_active = 1;
    ClearSpriteWorkPause(self);
}

EC u32 GetSpriteWorkerBaseAnimation(SpriteWorkerEntity * self) NPC_SIDE_CODE(08033AFC);
EC u32 GetSpriteWorkerBaseAnimation(SpriteWorkerEntity * self)
{
    u16 const * animations = gUnk_080F14B8 + 14;
    return animations[self->sprite_id];
}

struct DirectedMovingNpc : MovingNpcEntity
{
    DirectedMovingNpc();
    u8 direction : 2;
    u8 flags : 6;
};

struct RecordedEffectEntity : AEntity
{
    RecordedEffectEntity();
    void * record;
    u8 active;
};

EC void InitializeActorBase(AActorEntity * self, GameObject * game, ActorLocation const & location, u32 layer, u32 animation)
    asm("__12AActorEntityP10GameObjectRC13ActorLocationUiUi");
EC void InitializeEntityBase(AEntity * self, GameObject * game, Location const & location)
    asm("__7AEntityP10GameObjectRC8Location");
EC Location ItemCollection_GetExtraItems(void * record);
EC void SetNpcMovementBounds(MovingNpcEntity * self, Box const & bounds);
EC u8 const vtable_unk_080E7328[];
EC u8 const vtable_unk_080E7218[];
EC u8 const vtable_unk_080E725C[];
EC u8 const vtable_unk_080E72A0[];
EC u8 const vtable_unk_080E72E4[];
EC u8 const vtable_unk_080E7380[];
EC u8 const vtable_unk_080E7568[];
EC u8 const vtable_unk_080E73FC[];

EC MovingNpcEntity * InitializeMovingNpcBase(MovingNpcEntity * self, GameObject * game, ActorLocation * location, u32 animation, u32 idle_animation) NPC_EXTRA_CODE(08037008);
EC MovingNpcEntity * InitializeMovingNpcBase(MovingNpcEntity * self, GameObject * game, ActorLocation * location, u32 animation, u32 idle_animation)
{
    InitializeActorBase(self, game, *location, 2, animation);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E7328;
    self->saved_location = location;
    Box * bounds = &self->movement_bounds;
    bounds->x1 = 0;
    bounds->y1 = 0;
    bounds->x2 = 0;
    bounds->y2 = 0;
    self->animation = idle_animation;
    self->timer = 0;
    self->hidden = 1;
    return self;
}

EC MovingNpcEntity * InitializeMovingNpcVariantSix(MovingNpcEntity * self, GameObject * game, ActorLocation * location) NPC_EXTRA_CODE(08037BB8);
EC MovingNpcEntity * InitializeMovingNpcVariantSix(MovingNpcEntity * self, GameObject * game, ActorLocation * location)
{
    InitializeMovingNpcBase(self, game, location, 519, 10);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E7218;
    return self;
}

EC MovingNpcEntity * InitializeMovingNpcVariantSeven(MovingNpcEntity * self, GameObject * game, ActorLocation * location) NPC_EXTRA_CODE(08037BE0);
EC MovingNpcEntity * InitializeMovingNpcVariantSeven(MovingNpcEntity * self, GameObject * game, ActorLocation * location)
{
    InitializeMovingNpcBase(self, game, location, 889, 12);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E725C;
    return self;
}

EC MovingNpcEntity * CreateMovingNpcVariantSeven(GameObject * game, ActorLocation * location) NPC_EXTRA_CODE(08037B48);
EC MovingNpcEntity * CreateMovingNpcVariantSeven(GameObject * game, ActorLocation * location)
{
    struct Storage
    {
        u8 bytes[0x44];
        Storage(GameObject * game, ActorLocation * location)
        {
            MovingNpcEntity * self = reinterpret_cast<MovingNpcEntity *>(this);
            InitializeMovingNpcBase(self, game, location, 889, 12);
            reinterpret_cast<void const **>(self)[5] = vtable_unk_080E725C;
        }
    };
    return reinterpret_cast<MovingNpcEntity *>(new Storage(game, location));
}

EC MovingNpcEntity * CreateMovingNpcVariantSix(GameObject * game, ActorLocation * location) NPC_EXTRA_CODE(08037B80);
EC MovingNpcEntity * CreateMovingNpcVariantSix(GameObject * game, ActorLocation * location)
{
    struct Storage
    {
        u8 bytes[0x44];
        Storage(GameObject * game, ActorLocation * location)
        {
            MovingNpcEntity * self = reinterpret_cast<MovingNpcEntity *>(this);
            InitializeMovingNpcBase(self, game, location, 519, 10);
            reinterpret_cast<void const **>(self)[5] = vtable_unk_080E7218;
        }
    };
    return reinterpret_cast<MovingNpcEntity *>(new Storage(game, location));
}

EC DirectedMovingNpc * InitializeDirectedMovingNpcOne(DirectedMovingNpc * self, GameObject * game, ActorLocation * location, u32 direction) NPC_EXTRA_CODE(08037C08);
EC DirectedMovingNpc * InitializeDirectedMovingNpcOne(DirectedMovingNpc * self, GameObject * game, ActorLocation * location, u32 direction)
{
    InitializeMovingNpcBase(self, game, location, 684, 11);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E72A0;
    self->direction = direction;
    Box bounds(920, 288);
    bounds.x2 += 328;
    bounds.y2 += 8;
    SetNpcMovementBounds(self, bounds);
    return self;
}

EC DirectedMovingNpc * InitializeDirectedMovingNpcTwo(DirectedMovingNpc * self, GameObject * game, ActorLocation * location, u32 direction) NPC_EXTRA_CODE(08037C68);
EC DirectedMovingNpc * InitializeDirectedMovingNpcTwo(DirectedMovingNpc * self, GameObject * game, ActorLocation * location, u32 direction)
{
    InitializeMovingNpcBase(self, game, location, 1820, 0);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E72E4;
    self->direction = direction;
    Box bounds(352, 216);
    bounds.x2 += 56;
    bounds.y2 += 16;
    SetNpcMovementBounds(self, bounds);
    return self;
}

EC RecordedEffectEntity * InitializeSideEffectEntity(RecordedEffectEntity * self, GameObject * game, void * record) NPC_EXTRA_CODE(08038740);
EC RecordedEffectEntity * InitializeSideEffectEntity(RecordedEffectEntity * self, GameObject * game, void * record)
{
    Location location(8, 0, 0);
    InitializeEntityBase(self, game, location);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E7380;
    self->record = record;
    return self;
}

EC RecordedEffectEntity * CreateActiveSideEffectEntity(GameObject * game, void * record) NPC_EXTRA_CODE(0803A798);
EC RecordedEffectEntity * CreateActiveSideEffectEntity(GameObject * game, void * record)
{
    struct Storage { u8 bytes[0x20]; };
    RecordedEffectEntity * self = reinterpret_cast<RecordedEffectEntity *>(new Storage);
    Location location(0, 0, 0);
    InitializeEntityBase(self, game, location);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E7568;
    self->record = record;
    self->active = 1;
    return self;
}

EC RecordedEffectEntity * InitializeTimedReactionEntity(RecordedEffectEntity * self, GameObject * game, void * record) NPC_EXTRA_CODE(08037E18);
EC RecordedEffectEntity * InitializeTimedReactionEntity(RecordedEffectEntity * self, GameObject * game, void * record)
{
    Location location = ItemCollection_GetExtraItems(record);
    InitializeEntityBase(self, game, location);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E73FC;
    self->record = record;
    return self;
}

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

struct SpriteWorkerEffect : UnknownEntityThing
{
    STRUCT_PAD(0x8C, 0xB4);
    SpriteAnimator effect_animator;
    u8 effect_visible;
    u8 effect_offset;
    u8 effect_flip;
    u8 effect_restart;
    i16 effect_x;
    i16 effect_y;
    u16 effect_timer;
};

struct AudioPlayer;
struct SideEntityAudioPlayers
{
    AudioPlayer ** begin;
    AudioPlayer ** end;
};

struct SideEntityAudioProvider;
struct SideEntityAudioProviderVTable
{
    STRUCT_PAD(0x00, 0x14C);
    SideEntityAudioPlayers * (*get_players)(SideEntityAudioProvider *);
};

struct SideEntityAudioProvider
{
    SideEntityAudioProviderVTable * vtable;
};

EC void DestroyMapSpriteResource(void * self, u32 flags);

struct OwnedTransitionSprite
{
    STRUCT_PAD(0x00, 0x54);
    ~OwnedTransitionSprite() { DestroyMapSpriteResource(this, 2); }
};

struct SideTransitionOwner
{
    STRUCT_PAD(0x00, 0x0C);
    SmartPtr<OwnedTransitionSprite> sprite;
};

struct OwnedActivityOverlay
{
    STRUCT_PAD(0x00, 0x24);
    virtual ~OwnedActivityOverlay();
};

struct ActivityRenderSprite
{
    STRUCT_PAD(0x00, 0x08);
    u8 renderer[0x40];
    OwnedActivityOverlay * overlay;
    void const * vtable;
};

EC u8 const vtable_unk_080E6828[];
EC u8 const vtable_unk_080E76BC[];
EC void vfunc_14__7AEntity(AEntity * self);
EC void SetChangedLocation__3Npc(void * record);
EC SpriteWorkerEffect * InitializeSpriteWorkerGraphics(SpriteWorkerEffect * self, SpriteWorkerEntity * actor);
EC void UpdateSideActorAnimationLayers(UnknownEntityThing * self);
EC bool AudioPlayer_IsPlaying(AudioPlayer * self);
EC void AudioPlayer_StartSong(AudioPlayer * self, u16 song);

EC RecordedSideActor * InitializeRecordedSideActor(RecordedSideActor * self, GameObject * game, SideActorRecord * record) NPC_SIDE_CODE(08032934);
EC RecordedSideActor * InitializeRecordedSideActor(RecordedSideActor * self, GameObject * game, SideActorRecord * record)
{
    InitializeActorBase(self, game, record->location, 2, record->animation);
    reinterpret_cast<void const **>(self)[5] = vtable_unk_080E6828;
    self->record = record;
    return self;
}

EC SpriteWorkerEffect * CreateSpriteWorkerSprite(SpriteWorkerEntity * actor) NPC_SIDE_CODE(08033A80);
EC SpriteWorkerEffect * CreateSpriteWorkerSprite(SpriteWorkerEntity * actor)
{
    struct Storage { u8 bytes[0xD4]; };
    SpriteWorkerEffect * sprite = InitializeSpriteWorkerGraphics(reinterpret_cast<SpriteWorkerEffect *>(new Storage), actor);
    i32 overlay = -1;
    switch (actor->work_state)
    {
    case 2: overlay = 8; break;
    case 3: overlay = 5; break;
    }
    if (overlay >= 0)
    {
        SetSpriteAnimatorClip(&sprite->sprite_animator_70, overlay);
        sprite->unk_84 = 1;
        sprite->unk_86 = 0;
        sprite->unk_87 = 1;
        u32 mode = 2;
        sprite->unk_8A_0 = mode;
    }
    return sprite;
}

EC void RefreshSpriteWorkerRecord(SpriteWorkerEntity * self) NPC_SIDE_CODE(08033B10);
EC void RefreshSpriteWorkerRecord(SpriteWorkerEntity * self)
{
    vfunc_14__7AEntity(self);
    SetChangedLocation__3Npc(self->record);
}

EC void UpdateSpriteWorkerEffect(SpriteWorkerEffect * self) NPC_SIDE_CODE(08034AA0);
EC void UpdateSpriteWorkerEffect(SpriteWorkerEffect * self)
{
    UpdateSideActorAnimationLayers(self);
    if (self->effect_timer != 0)
    {
        if (self->effect_restart == 0)
        {
            if (i32(AdvanceSpriteAnimator(&self->effect_animator) << 30) < 0)
                self->effect_visible = 1;
        }
        else
            self->effect_restart = 0;
        self->effect_timer--;
    }
}




EC void PlaySideEntitySong(AEntity * self, u32 song) NPC_EXTRA_CODE(0803A350);
EC void PlaySideEntitySong(AEntity * self, u32 song)
{
    SideEntityAudioProvider * game = reinterpret_cast<SideEntityAudioProvider *>(self->game_object);
    SideEntityAudioPlayers * players = game->vtable->get_players(game);
    AudioPlayer ** current = players->begin;
    AudioPlayer ** end = players->end;
    AudioPlayer * player;
    while (current != end)
    {
        if (!AudioPlayer_IsPlaying(*current))
        {
            player = *current;
            goto play;
        }
        current++;
    }
    player = end[-1];
play:
    AudioPlayer_StartSong(player, song);
}

EC void ClearSideTransitionSprite(SideTransitionOwner * self) NPC_EXTRA_CODE(08038EB8);
EC void ClearSideTransitionSprite(SideTransitionOwner * self)
{
    SmartPtr<OwnedTransitionSprite> * sprite = &self->sprite;
    *sprite = nullptr;
}

EC void DestroyActivityRenderSprite(ActivityRenderSprite * self, u32 flags) NPC_EXTRA_CODE(08039F50);
EC void DestroyActivityRenderSprite(ActivityRenderSprite * self, u32 flags)
{
    self->vtable = vtable_unk_080E76BC;
    delete self->overlay;
    DestroyMapSpriteResource(self->renderer, 2);
    if (flags & 1)
        delete reinterpret_cast<u8 *>(self);
}
