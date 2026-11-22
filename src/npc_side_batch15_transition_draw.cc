#include "g1_map_resources.hh"
#define NPC_EXTRA_CODE(address) SECTION(".text.npc_extra." #address)

struct TransitionDrawFrame
{
    u8 bytes[0x20];
    TransitionDrawFrame() {}
};
struct TransitionDrawFrameSource;
struct TransitionDrawFrameMethods
{
    STRUCT_PAD(0x00, 0x10);
    void (*get_frame)(TransitionDrawFrame *, TransitionDrawFrameSource *, u32);
};
struct TransitionDrawFrameSource { TransitionDrawFrameMethods * methods; };

struct TransitionFloatingFlags
{
    u8 refresh_frame, mode, dirty, paused;
    u16 reserved;
    i16 x, y;
    STRUCT_PAD(0x0A, 0x11);
    u8 visible;
    STRUCT_PAD(0x12, 0x14);
};

struct TransitionFloatingSprite
{
    G1MapSpriteResource renderer;
    SpriteAnimator animator;
    TransitionFloatingFlags flags;
};
struct TransitionFloatingCollection
{
    u32 count;
    TransitionFloatingSprite sprites[5];
};

struct TransitionDepartureSprite
{
    G1MapSpriteResource renderer;
    TransitionDrawFrame frame;
    u8 refresh_frame;
};
struct TransitionAscendingSprite : TransitionDepartureSprite
{
    i32 y_q16;
    u8 ascending, column;
};

struct TransitionBurstCollection
{
    u32 count;
    G1AnimatedMapSpriteResource sprites[5];
    u8 completed[5];
};
struct TransitionBurstPosition { u16 x, y; };
EC TransitionBurstPosition const gUnk_080F165E[5];

struct TransitionDrawEffects
{
    void * owner;
    void const * methods;
    TransitionDepartureSprite * departure;
    TransitionAscendingSprite * ascending;
    TransitionBurstCollection * burst;
    TransitionFloatingCollection * floating;
};

struct TransitionDrawContext
{
    void * tiles;
    void * reserved;
    void * sprites;
    i16 camera_x, camera_y;
};
struct TransitionDrawQueues
{
    void * tiles;
    void * sprites;
    TransitionDrawQueues() {}
};

template <typename T> struct TransitionDrawLocal
{
    T volatile value;
    TransitionDrawLocal() {}
};

EC u32 QueueTransitionFrame(void *, i32, i32, u32, u32, TransitionDrawFrame const &, G1MapGraphics *, u32, u32 const *) asm("func_030004DC");

inline void UploadTransitionTiles(G1MapSpriteResource * renderer, void * queue, void const * tiles)
{
    register void * tile_queue asm("r1") = queue;
    register void const * source asm("r2") = tiles;
    register G1MapSpriteResource * target asm("r0") = renderer;
    asm volatile("bl func_080A480C" : "+r"(target), "+r"(tile_queue), "+r"(source) : : "r3", "r12", "lr", "cc", "memory");
}

inline void UploadTransitionPalette(G1MapSpriteResource * renderer, void const * palettes, u32 flag)
{
    register void const * source asm("r1") = palettes;
    register G1MapSpriteResource * target asm("r0") = renderer;
    register u32 mode asm("r2") = flag;
    asm volatile("bl LoadMapSpriteResourcePalettes" : "+r"(target), "+r"(source), "+r"(mode) : : "r3", "r12", "lr", "cc", "memory");
}

inline void ReadFloatingTransitionFrame(G1AnimatedMapSpriteResource * input_renderer, TransitionDrawFrame * frame)
{
    register G1AnimatedMapSpriteResource * renderer asm("r6") = input_renderer;
    asm("" : "+l"(renderer));
    register TransitionDrawFrameSource * source asm("r1") = reinterpret_cast<TransitionDrawFrameSource *>(const_cast<SpriteAnimationSource *>(renderer->animator.source));
    register TransitionDrawFrameMethods * methods asm("r3") = source->methods;
    register u32 keyframe asm("r2") = renderer->animator.keyframe;
    keyframe *= sizeof(SpriteKeyframe);
    register SpriteKeyframe const * keys asm("r0") = renderer->animator.clip.keyframes;
    keys = reinterpret_cast<SpriteKeyframe const *>(reinterpret_cast<u8 const *>(keys) + keyframe);
    keyframe = keys->frame;
    register TransitionDrawFrame * output asm("r0") = frame;
    register void (*callback)(TransitionDrawFrame *, TransitionDrawFrameSource *, u32) asm("r3") = methods->get_frame;
    callback(output, source, keyframe);
}

inline void ReadBurstTransitionFrame(G1AnimatedMapSpriteResource * input_renderer, TransitionDrawFrame * frame)
{
    register G1AnimatedMapSpriteResource * renderer asm("r5") = input_renderer;
    register TransitionDrawFrameSource * source asm("r1") = reinterpret_cast<TransitionDrawFrameSource *>(const_cast<SpriteAnimationSource *>(renderer->animator.source));
    register TransitionDrawFrameMethods * methods asm("r3") = source->methods;
    register u32 keyframe asm("r2") = renderer->animator.keyframe;
    keyframe *= sizeof(SpriteKeyframe);
    register SpriteKeyframe const * keys asm("r0") = renderer->animator.clip.keyframes;
    keys = reinterpret_cast<SpriteKeyframe const *>(reinterpret_cast<u8 const *>(keys) + keyframe);
    keyframe = keys->frame;
    register TransitionDrawFrame * output asm("r0") = frame;
    register void (*callback)(TransitionDrawFrame *, TransitionDrawFrameSource *, u32) asm("r3") = methods->get_frame;
    callback(output, source, keyframe);
}

EC void DrawNpcTransitionEffects(TransitionDrawEffects *, TransitionDrawContext *) NPC_EXTRA_CODE(08038A70);
EC void DrawNpcTransitionEffects(TransitionDrawEffects * self, TransitionDrawContext * context)
{
    TransitionDrawFrame frame_data;
    TransitionDrawQueues queues_data, floating_queues, departure_queues, ascending_queues, burst_queues;
    TransitionDrawLocal<TransitionDrawEffects *> saved_self;
    TransitionDrawLocal<i32> camera_x, camera_y;
    TransitionDrawLocal<TransitionFloatingCollection *> floating_collection;
    TransitionDrawQueues floating_base;
    TransitionDrawLocal<u32> floating_index;
    TransitionDrawLocal<TransitionBurstCollection *> burst_collection;
    TransitionDrawLocal<u32> burst_index;
    u32 const * burst_palette;
    TransitionBurstPosition const * burst_position;
    u32 const * floating_palette;
    asm("" : "=m"(queues_data), "=m"(floating_queues), "=m"(departure_queues), "=m"(ascending_queues), "=m"(burst_queues));
    asm("" : "=m"(saved_self), "=m"(camera_x), "=m"(camera_y), "=m"(floating_collection), "=m"(floating_base), "=m"(floating_index));
    asm("" : "=m"(burst_collection), "=m"(burst_index));
    saved_self.value = self;
    register u32 camera_offset asm("r2") = offsetof(TransitionDrawContext, camera_x);
    register i32 camera_value asm("r0");
    asm("ldrsh %0, [%1, %2]" : "=l"(camera_value) : "l"(context), "l"(camera_offset), "m"(*context));
    camera_x.value = camera_value;
    camera_offset = offsetof(TransitionDrawContext, camera_y);
    asm("ldrsh %0, [%1, %2]" : "=l"(camera_value) : "l"(context), "l"(camera_offset), "m"(*context));
    camera_y.value = camera_value;
    register void * tiles asm("r0") = context->tiles;
    register void * sprites asm("r1") = context->sprites;
    queues_data.tiles = tiles;
    queues_data.sprites = sprites;
    register TransitionDrawEffects * floating_reader asm("r1") = saved_self.value;
    register TransitionFloatingCollection * candidate asm("r0") = floating_reader->floating;
    if (candidate != nullptr)
    {
        floating_collection.value = candidate;
        floating_base = queues_data;
        register u32 zero asm("r1") = 0;
        floating_index.value = zero;
        register TransitionFloatingCollection * collection asm("r2") = floating_collection.value;
        if (zero < collection->count)
        {
            register TransitionFloatingFlags * flags asm("r5") = &collection->sprites[0].flags;
            floating_palette = &collection->sprites[0].renderer.palette_count;
            register G1AnimatedMapSpriteResource * renderer asm("r6") = reinterpret_cast<G1AnimatedMapSpriteResource *>(floating_collection.value->sprites);
            u32 next_index, count;
            do
            {
                floating_queues = floating_base;
                u8 next_visible;
                if (flags->visible != 0)
                {
                    register i32 world_x asm("r0") = flags->x;
                    register i32 horizontal asm("r2") = camera_x.value;
                    horizontal = world_x - horizontal;
                    asm("" : "+l"(horizontal));
                    register i32 screen_x asm("r9") = horizontal;
                    register i32 screen_y asm("r4");
                    {
                        register u32 y_offset asm("r1") = offsetof(TransitionFloatingFlags, y);
                        register i32 world_y asm("r0");
                        asm("ldrsh %0, [%1, %2]" : "=l"(world_y) : "l"(flags), "l"(y_offset), "m"(*flags));
                        register i32 vertical asm("r2") = camera_y.value;
                        screen_y = world_y - vertical;
                    }
                    register u32 depth_source asm("r0") = 0x7FFF;
                    register u32 depth asm("r10");
                    asm("mov %0, %1" : "=h"(depth) : "l"(depth_source));
                    register TransitionDrawQueues * queue_source asm("r1") = &floating_queues;
                    register TransitionDrawQueues * queues asm("r8");
                    asm("mov %0, %1" : "=h"(queues) : "l"(queue_source));
                    ReadFloatingTransitionFrame(renderer, &frame_data);
                    register TransitionDrawFrame * frame_register asm("r7") = &frame_data;
                    asm("" : : "l"(frame_register));
                    TransitionDrawFrame * frame = &frame_data;
                    register TransitionDrawQueues * reader asm("r2") = queues;
                    register void * sprite_queue asm("r3") = reader->sprites;
                    G1MapGraphics * graphics = renderer->graphics;
                    asm("" : "+&l"(graphics) : "l"(sprite_queue), "l"(screen_y), "l"(flags), "l"(renderer), "l"(frame_register) : "r0", "r1");
                    register i32 handle asm("r1") = renderer->tiles.handle;
                    u32 queued;
                    if ((-handle | handle) < 0)
                    {
                        register u32 tile asm("r0") = renderer->tile_index;
                        queued = QueueTransitionFrame(sprite_queue, screen_x, screen_y, 0xFF, depth, *frame, graphics, tile, floating_palette);
                    }
                    else
                        queued = 0;
                    if (queued != 0)
                    {
                        register u32 pending asm("r0") = flags->refresh_frame;
                        register TransitionFloatingFlags * refresh asm("r4") = flags;
                        if (pending != 0)
                        {
                            UploadTransitionTiles(renderer, queues->tiles, frame->bytes + 8);
                            if (flags->mode != 0)
                                UploadTransitionPalette(renderer, frame->bytes + 16, 1);
                            else if (flags->dirty == 0)
                            {
                                UploadTransitionPalette(renderer, frame->bytes + 16, 1);
                                flags->dirty = 1;
                            }
                            refresh->refresh_frame = 0;
                        }
                    }
                    next_visible = 0;
                }
                else
                    next_visible = 1;
                flags->visible = next_visible;
                flags = reinterpret_cast<TransitionFloatingFlags *>(reinterpret_cast<u8 *>(flags) + sizeof(TransitionFloatingSprite));
                register u32 const * next_palette asm("r1") = floating_palette;
                next_palette += sizeof(TransitionFloatingSprite) / sizeof(u32);
                u32 const * advanced_palette;
                asm("" : "=l"(advanced_palette) : "0"(next_palette), "l"(flags), "l"(renderer) : "r0", "r2", "r3", "r4", "r7");
                asm("" : "=l"(floating_palette) : "0"(advanced_palette), "l"(flags), "l"(renderer) : "r0", "r2", "r3", "r4", "r7");
                renderer = reinterpret_cast<G1AnimatedMapSpriteResource *>(reinterpret_cast<u8 *>(renderer) + sizeof(TransitionFloatingSprite));
                register u32 incremented asm("r2") = floating_index.value;
                ++incremented;
                next_index = incremented;
                floating_index.value = next_index;
                register TransitionFloatingCollection * count_reader asm("r1") = floating_collection.value;
                count = count_reader->count;
            } while (next_index < count);
        }
    }
    register TransitionDrawEffects * departure_reader asm("r2") = saved_self.value;
    register TransitionDepartureSprite * departure asm("r0") = departure_reader->departure;
    if (departure != nullptr)
    {
        register TransitionDepartureSprite * sprite asm("r5") = departure;
        departure_queues = queues_data;
        register i32 fixed_x asm("r0") = 300;
        register i32 horizontal asm("r1") = camera_x.value;
        horizontal = fixed_x - horizontal;
        asm("" : "+l"(horizontal));
        register i32 screen_x asm("r9") = horizontal;
        register i32 fixed_y asm("r0") = 100;
        register i32 vertical asm("r2") = camera_y.value;
        register i32 screen_y asm("r8") = fixed_y - vertical;
        TransitionDrawQueues * queues = &departure_queues;
        register TransitionDrawFrame * frame asm("r6") = &sprite->frame;
        register void * sprite_queue asm("r4") = queues->sprites;
        register G1MapGraphics * const * parameters asm("r0") = &sprite->renderer.graphics;
        register G1MapGraphics * graphics asm("r3") = *parameters++;
        register i32 handle asm("r1") = reinterpret_cast<G1MapTileOwner const *>(parameters)->handle;
        u32 queued;
        if ((-handle | handle) < 0)
        {
            register u32 tile asm("r1") = sprite->renderer.tile_index;
            register u32 const * palette asm("r2") = &sprite->renderer.palette_count;
            queued = QueueTransitionFrame(sprite_queue, screen_x, screen_y, 0xFF, 0x8000, *frame, graphics, tile, palette);
        }
        else
            queued = 0;
        if (queued != 0)
        {
            register u8 * refresh asm("r4") = &sprite->refresh_frame;
            if (*refresh != 0)
            {
                UploadTransitionTiles(&sprite->renderer, queues->tiles, frame->bytes + 8);
                UploadTransitionPalette(&sprite->renderer, frame->bytes + 16, 1);
                *refresh = 0;
            }
        }
    }
    register TransitionDrawEffects * ascending_reader asm("r1") = saved_self.value;
    register TransitionAscendingSprite * ascending asm("r0") = ascending_reader->ascending;
    if (ascending != nullptr)
    {
        register TransitionAscendingSprite * sprite asm("r5") = ascending;
        ascending_queues = queues_data;
        register i32 column_x asm("r0") = sprite->column;
        column_x *= 8;
        register i32 horizontal asm("r1") = camera_x.value;
        horizontal -= 240;
        column_x -= horizontal;
        asm("" : "+l"(column_x));
        register i32 screen_x asm("r9") = column_x;
        register i32 world_y asm("r0") = sprite->y_q16 >> 16;
        register i32 vertical asm("r2") = camera_y.value;
        register i32 screen_y asm("r8") = world_y - vertical;
        TransitionDrawQueues * queues = &ascending_queues;
        register TransitionDrawFrame * frame asm("r6") = &sprite->frame;
        register void * sprite_queue asm("r4") = queues->sprites;
        register G1MapGraphics * const * parameters asm("r0") = &sprite->renderer.graphics;
        register G1MapGraphics * graphics asm("r3") = *parameters++;
        register i32 handle asm("r1") = reinterpret_cast<G1MapTileOwner const *>(parameters)->handle;
        u32 queued;
        if ((-handle | handle) < 0)
        {
            register u32 tile asm("r1") = sprite->renderer.tile_index;
            register u32 const * palette asm("r2") = &sprite->renderer.palette_count;
            queued = QueueTransitionFrame(sprite_queue, screen_x, screen_y, 0xFF, 0x8000, *frame, graphics, tile, palette);
        }
        else
            queued = 0;
        if (queued != 0)
        {
            register u8 * refresh asm("r4") = &sprite->refresh_frame;
            if (*refresh != 0)
            {
                UploadTransitionTiles(&sprite->renderer, queues->tiles, frame->bytes + 8);
                UploadTransitionPalette(&sprite->renderer, frame->bytes + 16, 0);
                *refresh = 0;
            }
        }
    }
    register TransitionDrawEffects * burst_reader asm("r1") = saved_self.value;
    register TransitionBurstCollection * burst asm("r0") = burst_reader->burst;
    if (burst == nullptr)
        return;
    burst_collection.value = burst;
    burst_queues = queues_data;
    register u32 zero asm("r2") = 0;
    burst_index.value = zero;
    burst_palette = &burst_collection.value->sprites[0].palette_count;
    register TransitionBurstPosition const * first_position asm("r1") = gUnk_080F165E;
    asm("" : "+l"(first_position));
    burst_position = first_position;
    register G1AnimatedMapSpriteResource * renderer asm("r5") = burst_collection.value->sprites;
    asm("" : "+l"(renderer));
    u32 next_index;
    do
    {
        register TransitionBurstCollection * collection asm("r2") = burst_collection.value;
        register u32 completed_offset asm("r1") = offsetof(TransitionBurstCollection, completed);
        register u8 * completed asm("r0") = reinterpret_cast<u8 *>(collection) + completed_offset;
        register u32 index asm("r2") = burst_index.value;
        completed += index;
        if (*completed == 0)
        {
            register TransitionBurstPosition const * position asm("r1") = burst_position;
            register i32 world_x asm("r0") = position->x;
            register i32 horizontal asm("r2") = camera_x.value;
            horizontal = world_x - horizontal;
            asm("" : "+l"(horizontal));
            register i32 screen_x asm("r9") = horizontal;
            register i32 world_y asm("r0") = position->y;
            register i32 vertical asm("r1") = camera_y.value;
            register i32 screen_y asm("r4") = world_y - vertical;
            register G1AnimatedMapSpriteResource * resource asm("r6") = renderer;
            register u32 depth_low asm("r2") = 0x8000;
            asm("" : "+l"(depth_low));
            register u32 depth asm("r10") = depth_low;
            register TransitionDrawQueues * queues_low asm("r0") = &burst_queues;
            asm("" : "+l"(queues_low));
            register TransitionDrawQueues * queues asm("r8") = queues_low;
            ReadBurstTransitionFrame(renderer, &frame_data);
            register TransitionDrawFrame * frame_register asm("r7") = &frame_data;
            asm("" : : "l"(frame_register));
            TransitionDrawFrame * frame = &frame_data;
            register TransitionDrawQueues * reader asm("r1") = queues;
            register void * sprite_queue asm("r3") = reader->sprites;
            G1MapGraphics * graphics = renderer->graphics;
            asm("" : "+&l"(graphics) : "l"(sprite_queue), "l"(screen_y), "l"(renderer), "l"(resource), "l"(frame_register) : "r0", "r1");
            register i32 handle asm("r1") = renderer->tiles.handle;
            u32 queued;
            if ((-handle | handle) < 0)
            {
                register u32 tile asm("r0") = renderer->tile_index;
                queued = QueueTransitionFrame(sprite_queue, screen_x, screen_y, 0xFF, depth, *frame, graphics, tile, burst_palette);
            }
            else
                queued = 0;
            if (queued != 0)
            {
                register u8 * refresh_low asm("r0") = &resource->refresh_frame;
                register u32 pending asm("r1") = *refresh_low;
                register u8 * refresh asm("r9") = refresh_low;
                if (pending != 0)
                {
                    UploadTransitionTiles(resource, queues->tiles, frame->bytes + 8);
                    if (resource->mode)
                        UploadTransitionPalette(resource, frame->bytes + 16, 1);
                    else
                    {
                        register bool * dirty asm("r4") = &resource->dirty;
                        if (!*dirty)
                        {
                            UploadTransitionPalette(resource, frame->bytes + 16, 1);
                            *dirty = true;
                        }
                    }
                    register u32 clear_value asm("r0") = 0;
                    register u8 * destination asm("r1") = refresh;
                    *destination = clear_value;
                }
            }
        }
        register u32 const * next_palette asm("r2") = burst_palette;
        next_palette += sizeof(G1AnimatedMapSpriteResource) / sizeof(u32);
        u32 const * advanced_palette;
        asm("" : "=l"(advanced_palette) : "0"(next_palette), "l"(renderer) : "r0", "r1", "r3", "r4", "r6", "r7");
        asm("" : "=l"(burst_palette) : "0"(advanced_palette), "l"(renderer) : "r0", "r1", "r3", "r4", "r6", "r7");
        register TransitionBurstPosition const * next_position asm("r0") = burst_position;
        ++next_position;
        asm("" : "+l"(next_position));
        burst_position = next_position;
        ++renderer;
        register u32 incremented asm("r1") = burst_index.value;
        ++incremented;
        next_index = incremented;
        burst_index.value = next_index;
    } while (next_index <= 4);
}
