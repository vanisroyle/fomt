#include "entity_actor.hh"
#include "g1_map_resources.hh"
#include "smart_ptr.hh"

#define NPC_EXTRA_CODE(address) SECTION(".text.npc_extra." #address)

struct TransitionFrame
{
    u8 bytes[0x20];
};

struct TransitionSpriteBank
{
    virtual ~TransitionSpriteBank();
    virtual SpriteAnimationClip GetClip(u32 animation);
    virtual TransitionFrame GetFrame(u32 frame);
};

struct TransitionSpriteServices;
struct TransitionSpriteVTable
{
    STRUCT_PAD(0x00, 0x74);
    TransitionSpriteBank * (*get_bank)(TransitionSpriteServices *);
};

struct TransitionSpriteServices
{
    TransitionSpriteVTable * vtable;
};

EC u8 const vtable_unk_080E602C[];
EC void InitializeMapSpriteSinglePalette(void * renderer, GameObject * game, u32 layer, u32 palette);
EC void DestroyMapSpriteResource(void * renderer, u32 flags);

inline void * operator new(size_t, TransitionFrame * frame) { return frame; }

struct NpcDepartureResources
{
    u8 bytes[0x24];
    void const * vtable;
};

struct NpcDepartureSprite
{
    NpcDepartureSprite(GameObject * game, u32 refresh_value)
    {
        register u32 saved_refresh asm("r10") = refresh_value;
        asm("" : "+h"(saved_refresh));
        TransitionSpriteServices * services = reinterpret_cast<TransitionSpriteServices *>(game);
        TransitionSpriteBank * bank = services->vtable->get_bank(services);
        InitializeMapSpriteSinglePalette(this, game, 6, 27);
        renderer.vtable = vtable_unk_080E602C;
        register u32 frame_offset asm("r1") = sizeof(renderer);
        asm("add %0, %0, %1" : "+l"(frame_offset) : "l"(this));
        register TransitionFrame * destination asm("r1") = reinterpret_cast<TransitionFrame *>(frame_offset);
        asm("" : "+l"(destination));
        new (destination) TransitionFrame(bank->GetFrame(bank->GetClip(0).keyframes[0].frame));
        register u8 * refresh_flag asm("r0") = &refresh_frame;
        asm("" : "+l"(refresh_flag));
        register u32 refresh asm("r5") = saved_refresh;
        asm("" : "+l"(refresh));
        *refresh_flag = refresh;
    }

    NpcDepartureResources renderer;
    TransitionFrame frame;
    u8 refresh_frame;
};

struct NpcFloatingParticle
{
    u8 renderer[0x28];
    u8 animation[0x14];
    struct Motion
    {
        u8 refresh_frame;
        u8 reserved[2];
        u8 paused;
        i32 y_q16;
        i32 x_q16;
        i32 speed_q16;
        u8 height;
        STRUCT_PAD(0x11, 0x14);
    } motion;
    ~NpcFloatingParticle() { DestroyMapSpriteResource(this, 2); }
};

struct NpcFloatingParticles
{
    u32 count;
    u8 storage[5 * sizeof(NpcFloatingParticle)];

    ~NpcFloatingParticles()
    {
        register u32 size asm("r0") = count * sizeof(NpcFloatingParticle) + 4;
        asm("" : "+l"(size));
        register NpcFloatingParticle * end asm("r5") = reinterpret_cast<NpcFloatingParticle *>(reinterpret_cast<u8 *>(this) + size);
        asm("" : "+l"(end));
        NpcFloatingParticle * begin = reinterpret_cast<NpcFloatingParticle *>(storage);
        for (NpcFloatingParticle * particle = begin; particle != end; ++particle)
            particle->~NpcFloatingParticle();
    }
};

struct NpcEffectRecord
{
    STRUCT_PAD(0x00, 0x54);
    u8 reserved_departure : 5;
    u8 departure_mode : 2;
    u8 departure_flags : 1;
    STRUCT_PAD(0x55, 0x5D);
    u8 reserved_particles : 2;
    u8 particle_mode : 2;
    u8 particle_flags : 4;
};

struct NpcEffectOwner
{
    GameObject * game;
    STRUCT_PAD(0x04, 0x18);
    NpcEffectRecord * record;
};

struct NpcFloatingParticlePtr
{
    NpcFloatingParticlePtr() : inner(nullptr) {}

    NpcFloatingParticles * inner;
};

struct NpcTransitionEffects
{
    NpcEffectOwner * owner;
    void const * vtable;
    NpcDepartureSprite * departure_sprite;
    void * transition_sprite;
    void * effect_particles;
    NpcFloatingParticlePtr floating_particles;
};

inline void * operator new(size_t, NpcFloatingParticlePtr * particles) { return particles; }

EC u8 const vtable_unk_080E736C[];
EC NpcFloatingParticles * InitializeNpcFloatingParticles(NpcFloatingParticles * particles, GameObject * game, u32 variant) asm("func_080DCB7C");
EC u32 AdvanceNpcEffectAnimation(void * animation) asm("func_0805E8F0");

EC NpcTransitionEffects * InitializeNpcTransitionEffects(NpcTransitionEffects * self, NpcEffectOwner * owner) NPC_EXTRA_CODE(08038820);
EC NpcTransitionEffects * InitializeNpcTransitionEffects(NpcTransitionEffects * self, NpcEffectOwner * input_owner)
{
    register NpcEffectOwner * owner asm("r9") = input_owner;
    asm("" : : "h"(owner));
    self->owner = input_owner;
    self->vtable = vtable_unk_080E736C;
    u32 departure = input_owner->record->departure_mode;
    register u32 refresh_value asm("r10") = departure;
    asm("" : "+h"(refresh_value));
    NpcDepartureSprite * sprite;
    if (departure == 1)
        sprite = new NpcDepartureSprite(owner->game, refresh_value);
    else
        sprite = nullptr;
    self->departure_sprite = sprite;
    self->transition_sprite = nullptr;
    self->effect_particles = nullptr;
    register NpcFloatingParticlePtr * destination asm("r4") = &self->floating_particles;
    asm("" : : "l"(destination));
    new (&self->floating_particles) NpcFloatingParticlePtr;
    register NpcFloatingParticles * particles asm("r3") = static_cast<NpcFloatingParticles *>(::operator new(sizeof(NpcFloatingParticles)));
    asm("" : "+l"(particles));
    register GameObject * game asm("r1") = self->owner->game;
    asm("" : "+l"(game));
    register u32 flag asm("r2") = 0;
    asm("" : : "l"(flag));
    register NpcEffectOwner * variant_owner asm("r5") = owner;
    asm("" : "+l"(variant_owner));
    register u32 variant asm("r0") = variant_owner->record->particle_mode;
    asm("" : "+l"(variant));
    if (variant == 1)
        flag = 1;
    NpcFloatingParticles * pending = InitializeNpcFloatingParticles(particles, game, flag);
    register NpcFloatingParticlePtr * slot asm("r9") = destination;
    asm("" : "+h"(slot));
    register NpcFloatingParticles * initialized asm("r8") = pending;
    asm("" : "+h"(initialized));
    register NpcFloatingParticles * previous asm("r6") = self->floating_particles.inner;
    asm("" : "+l"(previous));
    if (initialized != previous)
        delete previous;
    register NpcFloatingParticles * value asm("r0") = initialized;
    asm("" : "+l"(value));
    register NpcFloatingParticlePtr * target asm("r1") = slot;
    asm("" : "+l"(target));
    target->inner = value;
    return self;
}

struct NpcAscendingSprite
{
    u8 sprite[0x4C];
    i32 y_q16;
    u8 ascending;
};

struct NpcBurstParticle;
struct NpcBurstParticleVTable
{
    STRUCT_PAD(0x00, 0x08);
    void (*destroy)(NpcBurstParticle *, u32);
};

struct NpcBurstAnimation
{
    u8 state[0x14];
    u8 refresh_frame;
    u8 reserved[2];
    u8 paused;
};

struct NpcBurstParticle
{
    u8 renderer[0x24];
    NpcBurstParticleVTable * vtable;
    NpcBurstAnimation animation;

    ~NpcBurstParticle() { vtable->destroy(this, 2); }
};

struct NpcBurstParticles
{
    u32 count;
    u8 storage[5 * sizeof(NpcBurstParticle)];
    u8 completed[5];

    ~NpcBurstParticles()
    {
        u32 size = count * sizeof(NpcBurstParticle) + 4;
        asm("" : "+r"(size));
        NpcBurstParticle * end = reinterpret_cast<NpcBurstParticle *>(reinterpret_cast<u8 *>(this) + size);
        for (NpcBurstParticle * particle = reinterpret_cast<NpcBurstParticle *>(storage); particle != end; ++particle)
            particle->~NpcBurstParticle();
    }
};

EC void UpdateNpcTransitionEffects(NpcTransitionEffects * self) NPC_EXTRA_CODE(08038928);
EC void UpdateNpcTransitionEffects(NpcTransitionEffects * self)
{
    register NpcFloatingParticles * initial asm("r0") = self->floating_particles.inner;
    asm("" : "+l"(initial));
    if (initial != nullptr)
    {
        NpcFloatingParticles * floating = initial;
        u32 index = 0;
        if (index < floating->count)
        {
            NpcFloatingParticle * first = reinterpret_cast<NpcFloatingParticle *>(floating->storage);
            NpcFloatingParticle::Motion * motion = &first->motion;
            u8 * animation = first->animation;
            do
            {
                if (motion->paused == 0)
                {
                    if (i32(AdvanceNpcEffectAnimation(animation) << 30) < 0)
                        motion->refresh_frame = 1;
                }
                else
                    motion->paused = 0;
                motion->y_q16 -= motion->speed_q16;
                if (motion->y_q16 < -(motion->height << 16))
                    motion->y_q16 = 0x1E00000;
                motion = reinterpret_cast<NpcFloatingParticle::Motion *>(reinterpret_cast<u8 *>(motion) + sizeof(NpcFloatingParticle));
                animation += sizeof(NpcFloatingParticle);
                ++index;
            } while (index < floating->count);
        }
    }
    NpcAscendingSprite * ascending = static_cast<NpcAscendingSprite *>(self->transition_sprite);
    if (ascending != nullptr && ascending->ascending != 0)
    {
        ascending->y_q16 -= 0x8000;
        if (ascending->y_q16 <= 0xA80000)
        {
            ascending->y_q16 = 0xA80000;
            ascending->ascending = 0;
        }
    }
    NpcBurstParticles * burst = static_cast<NpcBurstParticles *>(self->effect_particles);
    if (burst != nullptr)
    {
        u32 completed = 0;
        u32 index = 0;
        u8 * completion = burst->completed;
        NpcBurstAnimation * animation = &reinterpret_cast<NpcBurstParticle *>(burst->storage)->animation;
        u8 * paused = &animation->paused;
        do
        {
            register u32 finished asm("r1") = *completion;
            asm("" : "+l"(finished));
            if (finished == 0)
            {
                register u32 events asm("r0");
                if (animation->paused == 0)
                {
                    u32 flags = AdvanceNpcEffectAnimation(animation);
                    if (i32(flags << 30) < 0)
                    {
                        register u32 refresh asm("r0") = 1;
                        asm("" : "+l"(refresh));
                        animation->refresh_frame = refresh;
                    }
                    events = flags;
                    asm("" : "+l"(events));
                }
                else
                {
                    *paused = finished;
                    events = 2;
                    asm("" : "+l"(events));
                }
                if (i32(events << 29) < 0)
                {
                    register u32 finished_flag asm("r0") = 1;
                    asm("" : "+l"(finished_flag));
                    *completion = finished_flag;
                }
            }
            else
                ++completed;
            ++completion;
            animation = reinterpret_cast<NpcBurstAnimation *>(reinterpret_cast<u8 *>(animation) + sizeof(NpcBurstParticle));
            paused += sizeof(NpcBurstParticle);
            ++index;
        } while (index <= 4);
        u32 finished = 0;
        if (completed == 5)
            finished = 1;
        asm("" : "+r"(finished));
        if (finished != 0)
        {
            register NpcBurstParticles ** slot asm("r7") = reinterpret_cast<NpcBurstParticles **>(&self->effect_particles);
            asm("" : "+l"(slot));
            register u32 zero asm("r2") = 0;
            asm("" : "+l"(zero));
            register NpcBurstParticles * empty asm("r9") = reinterpret_cast<NpcBurstParticles *>(zero);
            asm("" : "+h"(empty));
            register NpcTransitionEffects * effects asm("r0") = self;
            asm("" : "+l"(effects));
            register NpcBurstParticles * previous asm("r6") = static_cast<NpcBurstParticles *>(effects->effect_particles);
            asm("" : "+l"(previous));
            if (empty != previous)
                delete previous;
            register NpcBurstParticles * value asm("r1") = empty;
            asm("" : "+l"(value));
            *slot = value;
        }
    }
}
