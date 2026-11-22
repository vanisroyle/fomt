#include "g1_map_resources.hh"

#define NPC_SIDE_CODE(address) SECTION(".text.npc_side." #address)

struct NpcLayerOwner;

struct NpcLayerFrame { u8 bytes[0x20]; };
struct NpcLayerFrameSource
{
    virtual ~NpcLayerFrameSource();
    virtual SpriteAnimationClip GetClip(u32);
    virtual NpcLayerFrame GetFrame(u32);
};

struct NpcWorkerLayerSprite
{
    NpcLayerOwner * actor;
    void const * vtable;
    G1AnimatedMapSpriteResource renderer;
    G1AnimatedMapSpriteResource held;
    u8 overlay;
    u8 overlay_offset;
    struct PACKED { u8 kind : 2; u8 frame : 6; } attachment;
    u8 priority;
    G1AnimatedMapSpriteResource effect;
    i16 effect_x;
    i16 effect_y;
    u16 effect_timer;
};

struct NpcLayerDrawContext
{
    void * tiles;
    u32 reserved;
    void * sprites;
    i16 camera_x;
    i16 camera_y;
};

struct NpcLayerQueues
{
    void * tiles;
    void * sprites;

    NpcLayerQueues(void * tiles, void * sprites) : tiles(tiles), sprites(sprites) {}
};

EC u32 QueueNpcLayerSprites(void *, i32, i32, u32, u32, NpcLayerFrame const &, G1MapGraphics *, u32, u32 const &) asm("func_030004DC");

inline u32 GetNpcLayerFrameIndex(SpriteAnimator const * input_animator)
{
    register SpriteAnimator const * animator asm("r0") = input_animator;
    register u32 index asm("r2") = animator->keyframe;
    asm volatile("" : "+l"(index));
    register SpriteAnimationClip const * clip asm("r0") = &animator->clip;
    asm volatile("" : "+l"(clip));
    index *= sizeof(SpriteKeyframe);
    asm volatile("" : "+l"(index));
    register u8 const * keyframes asm("r0") = reinterpret_cast<u8 const *>(clip->keyframes) + index;
    asm volatile("" : "+l"(keyframes));
    return reinterpret_cast<SpriteKeyframe const *>(keyframes)->frame;
}

inline NpcLayerFrame GetNpcLayerFrame(SpriteAnimator const * input_animator)
{
    register SpriteAnimator const * animator asm("r0") = input_animator;
    asm volatile("" : "+l"(animator));
    NpcLayerFrameSource * source = reinterpret_cast<NpcLayerFrameSource *>(const_cast<SpriteAnimationSource *>(animator->source));
    return source->GetFrame(GetNpcLayerFrameIndex(animator));
}

inline void UploadNpcLayerTiles(G1MapSpriteResource * renderer, void * queue, void const * tiles)
{
    register void * tile_queue asm("r1") = queue;
    register void const * source asm("r2") = tiles;
    register G1MapSpriteResource * target asm("r0") = renderer;
    asm volatile("bl func_080A480C" : "+r"(target), "+r"(tile_queue), "+r"(source) : : "r3", "r12", "lr", "cc", "memory");
}

inline void LoadNpcLayerPalettes(G1MapSpriteResource * renderer, void const * palettes)
{
    register void const * source asm("r1") = palettes;
    register G1MapSpriteResource * target asm("r0") = renderer;
    register u32 flag asm("r2") = 1;
    asm volatile("bl LoadMapSpriteResourcePalettes" : "+r"(target), "+r"(source), "+r"(flag) : : "r3", "r12", "lr", "cc", "memory");
}

EC void DrawSpriteWorkerEffectLayer(NpcWorkerLayerSprite * self, NpcLayerDrawContext * context) NPC_SIDE_CODE(08034AE8);
EC void DrawSpriteWorkerEffectLayer(NpcWorkerLayerSprite * input_self, NpcLayerDrawContext * input_context)
{
    register NpcWorkerLayerSprite * self asm("r5") = input_self;
    register NpcLayerDrawContext * context asm("r4") = input_context;
    register NpcWorkerLayerSprite * target asm("r0") = input_self;
    register NpcLayerDrawContext * draw_context asm("r1") = input_context;
    asm volatile("bl DrawSpriteWorkerLayers" : "+r"(target), "+r"(draw_context) : : "r2", "r3", "r12", "lr", "cc", "memory");
    if (self->effect_timer != 0)
    {
        NpcLayerQueues queues(context->tiles, context->sprites);
        asm("" : "+m"(queues));
        register i32 effect_x asm("r1") = self->effect_x;
        register i32 effect_y asm("r2") = self->effect_y;
        asm("" : "+l"(effect_x), "+l"(effect_y));
        i32 x = effect_x - context->camera_x;
        i32 y = effect_y - context->camera_y;
        register u32 base_depth asm("r0") = 0x8000;
        asm volatile("" : "+l"(base_depth));
        register u32 depth asm("r4") = base_depth - effect_y;
        register G1AnimatedMapSpriteResource * renderer asm("r6") = &self->effect;
        register NpcLayerQueues * queue_ptr asm("r10") = &queues;
        asm volatile("" : "+l"(renderer), "+h"(queue_ptr));
        NpcLayerFrame frame = GetNpcLayerFrame(&self->effect.animator);
        register NpcLayerFrame * frame_register asm("r7") = &frame;
        NpcLayerFrame * frame_ptr = &frame;
        register NpcLayerQueues * read_queues asm("r2") = queue_ptr;
        asm volatile("" : "+l"(read_queues) : "l"(frame_register));
        register void * sprites asm("r3") = read_queues->sprites;
        register G1MapGraphics * graphics asm("r2") = renderer->graphics;
        asm volatile("" : "+l"(sprites), "+l"(graphics));
        register G1MapTileOwner * tile_owner asm("r0") = &self->effect.tiles;
        asm volatile("" : "+l"(tile_owner));
        i32 handle = tile_owner->handle;
        u32 queued;
        if ((-handle | handle) < 0)
        {
            u32 tile = renderer->tile_index;
            register u32 const * palettes asm("r1") = &self->effect.palette_count;
            asm volatile("" : "+l"(palettes));
            queued = QueueNpcLayerSprites(sprites, x, y, 0x55, depth, *frame_ptr, graphics, tile, *palettes);
        }
        else
            queued = 0;
        if (queued != 0)
        {
            register u8 * refresh_register asm("r0") = &renderer->refresh_frame;
            register u32 refresh_flag asm("r1") = *refresh_register;
            asm volatile("" : "+l"(refresh_register), "+l"(refresh_flag));
            u8 * refresh = refresh_register;
            asm volatile("" : "+r"(refresh));
            if (refresh_flag != 0)
            {
                UploadNpcLayerTiles(renderer, queue_ptr->tiles, frame_ptr->bytes + 8);
                if (renderer->mode)
                    LoadNpcLayerPalettes(renderer, frame_ptr->bytes + 16);
                else
                {
                    bool * loaded = &renderer->dirty;
                    if (!*loaded)
                    {
                        LoadNpcLayerPalettes(renderer, frame_ptr->bytes + 16);
                        *loaded = true;
                    }
                }
                *refresh = 0;
            }
        }
    }
}
