#include "g1_map_resources.hh"

#define NPC_EXTRA_CODE(address) SECTION(".text.npc_extra." #address)

struct ActivityDrawFrame { u8 bytes[0x20]; };
struct ActivityDrawFrameSource;
struct ActivityDrawFrameMethods
{
    STRUCT_PAD(0x00, 0x10);
    void (*get_frame)(ActivityDrawFrame *, ActivityDrawFrameSource *, u32);
};
struct ActivityDrawFrameSource { ActivityDrawFrameMethods * methods; };

struct ActivityDrawOverlay
{
    G1MapSpriteResource renderer;
    ActivityDrawFrame frame;
    u8 refresh_frame;
};

struct ActivityDrawSprite
{
    void * owner;
    void const * vtable;
    G1AnimatedMapSpriteResource renderer;
    ActivityDrawOverlay * overlay;
};

struct ActivityDrawCamera;
struct ActivityDrawCameraMethods
{
    STRUCT_PAD(0x00, 0x18);
    i32 (*get_x)(ActivityDrawCamera *);
    i32 (*get_y)(ActivityDrawCamera *);
};
struct ActivityDrawCamera { ActivityDrawCameraMethods * methods; };

struct ActivityDrawQueues
{
    void * tiles;
    void * sprites;
};

EC u32 QueueActivityFrame(void *, i32, i32, u32, u32, ActivityDrawFrame const &, G1MapGraphics *, u32, u32 const &) asm("func_030004DC");
inline void UploadActivityFrameTiles(G1MapSpriteResource * renderer, void * queue, void const * tiles)
{
    register void * tile_queue asm("r1") = queue;
    register void const * source asm("r2") = tiles;
    register G1MapSpriteResource * target asm("r0") = renderer;
    asm volatile("bl func_080A480C" : "+r"(target), "+r"(tile_queue), "+r"(source) : : "r3", "r12", "lr", "cc", "memory");
}

inline void UploadActivityFramePalette(G1MapSpriteResource * renderer, void const * palettes, u32 flag)
{
    register void const * source asm("r1") = palettes;
    register G1MapSpriteResource * target asm("r0") = renderer;
    register u32 mode asm("r2") = flag;
    asm volatile("bl LoadMapSpriteResourcePalettes" : "+r"(target), "+r"(source), "+r"(mode) : : "r3", "r12", "lr", "cc", "memory");
}

inline void ReadActivityDrawFrame(ActivityDrawSprite * self, ActivityDrawFrame * frame)
{
    register ActivityDrawSprite * reader asm("r0") = self;
    asm("" : "+l"(reader));
    register ActivityDrawFrameSource * source asm("r1") = reinterpret_cast<ActivityDrawFrameSource *>(const_cast<SpriteAnimationSource *>(reader->renderer.animator.source));
    asm("" : "+l"(source));
    register ActivityDrawFrameMethods * methods asm("r3") = source->methods;
    asm("" : "+l"(methods));
    register u32 keyframe asm("r2") = reader->renderer.animator.keyframe;
    asm("" : "+l"(keyframe));
    keyframe *= sizeof(SpriteKeyframe);
    asm("" : "+l"(keyframe));
    register SpriteKeyframe const * keys asm("r0") = reader->renderer.animator.clip.keyframes;
    asm("" : "+l"(keys));
    keys = reinterpret_cast<SpriteKeyframe const *>(reinterpret_cast<u8 const *>(keys) + keyframe);
    asm("" : "+l"(keys));
    keyframe = keys->frame;
    asm("" : "+l"(keyframe));
    register ActivityDrawFrame * output asm("r0") = frame;
    asm("" : "+l"(output));
    register void (*callback)(ActivityDrawFrame *, ActivityDrawFrameSource *, u32) asm("r3") = methods->get_frame;
    asm("" : "+l"(callback));
    callback(output, source, keyframe);
}

EC void DrawActivityNpcSprite(ActivityDrawSprite *, ActivityDrawCamera *, void *, void *, i32, i32, i32, u32) NPC_EXTRA_CODE(08039F90);
EC void DrawActivityNpcSprite(ActivityDrawSprite * input_self, ActivityDrawCamera * input_camera, void * input_sprites, void * input_tiles, i32 input_x, i32 input_y, i32 input_elevation, u32 layer)
{
    register ActivityDrawSprite * self asm("r10") = input_self;
    register ActivityDrawCamera * camera asm("r4") = input_camera;
    register void * sprites asm("r9") = input_sprites;
    register void * tiles asm("r8") = input_tiles;
    register i32 world_x asm("r5") = input_x;
    register i32 world_y asm("r6") = input_y;
    register i32 elevation asm("r7") = input_elevation;
    asm("" : "+h"(self), "+l"(camera), "+h"(sprites), "+h"(tiles));
    asm("" : "+l"(world_x), "+l"(world_y), "+l"(elevation));
    ActivityDrawFrame frame_data;
    ActivityDrawQueues queues_data;
    i32 screen_x, screen_y, depth;
    screen_x = world_x - camera->methods->get_x(camera);
    screen_y = world_y - camera->methods->get_y(camera);
    depth = 0x8000 - world_y;
    register ActivityDrawQueues * queue_output asm("r0") = &queues_data;
    asm("" : "+l"(queue_output));
    register void * tile_output asm("r1") = tiles;
    asm("" : "+l"(tile_output));
    queues_data.tiles = tile_output;
    register void * sprite_output asm("r3") = sprites;
    asm("" : "+l"(sprite_output));
    queue_output->sprites = sprite_output;
    register i32 original_elevation asm("r5") = elevation;
    asm("" : "+l"(original_elevation));
    register ActivityDrawQueues * queues asm("r9") = queue_output;
    asm("" : "+h"(queues));
    if (original_elevation < 0)
        elevation = -original_elevation;
    register i32 raised_low asm("r4") = screen_y;
    asm("" : "+l"(raised_low));
    raised_low -= elevation;
    asm("" : "+l"(raised_low));
    register i32 raised_y asm("r8") = raised_low;
    asm("" : "+h"(raised_y));
    register G1AnimatedMapSpriteResource * renderer asm("r6") = &self->renderer;
    asm("" : "+l"(renderer));
    register u32 priority asm("r4");
    {
        register u32 bits asm("r0") = 3;
        asm("" : "+l"(bits));
        register u32 input_layer asm("r7") = layer;
        asm("" : "+l"(input_layer));
        bits &= input_layer;
        priority = (bits << 2) | bits;
        asm("" : "+l"(priority));
        register u32 middle asm("r1") = bits << 4;
        asm("" : "+l"(middle));
        priority |= middle;
        bits <<= 6;
        asm("" : "+l"(bits));
        priority |= bits;
    }
    ReadActivityDrawFrame(self, &frame_data);
    register ActivityDrawFrame * frame_register asm("r7") = &frame_data;
    ActivityDrawFrame * frame = &frame_data;
    u32 queued;
    {
        register ActivityDrawQueues * queue_reader asm("r1") = queues;
        asm("" : "+l"(queue_reader) : "l"(frame_register));
        register void * sprite_reader asm("r1") = queue_reader->sprites;
        asm("" : "+l"(sprite_reader));
        register void * sprite_queue asm("r12") = sprite_reader;
        asm("" : "+r"(sprite_queue));
        register ActivityDrawSprite * graphics_reader asm("r3") = self;
        asm("" : "+l"(graphics_reader));
        register G1MapGraphics * graphics asm("r2") = graphics_reader->renderer.graphics;
        asm("" : "+l"(graphics));
        register i32 handle asm("r1") = graphics_reader->renderer.tiles.handle;
        asm("" : "+l"(handle));
        if ((-handle | handle) < 0)
        {
            register u32 tile asm("r0") = renderer->tile_index;
            asm("" : "+l"(tile));
            register ActivityDrawSprite * palette_reader asm("r1") = self;
            asm("" : "+l"(palette_reader));
            register u32 const * palette_count asm("r1") = &palette_reader->renderer.palette_count;
            asm("" : "+l"(palette_count));
            register i32 queue_depth asm("r3") = depth;
            asm("" : "+l"(queue_depth));
            queued = QueueActivityFrame(sprite_queue, screen_x, raised_y, priority, queue_depth, *frame, graphics, tile, *palette_count);
        }
        else
            queued = 0;
    }
    if (queued != 0)
    {
        register u8 * refresh_reader asm("r0") = &renderer->refresh_frame;
        asm("" : "+l"(refresh_reader));
        register u32 pending asm("r1") = *refresh_reader;
        asm("" : "+l"(pending));
        register u8 * refresh asm("r8") = refresh_reader;
        asm("" : "+h"(refresh));
        if (pending != 0)
        {
            UploadActivityFrameTiles(renderer, queues_data.tiles, frame->bytes + 8);
            if (renderer->mode)
                UploadActivityFramePalette(renderer, frame->bytes + 16, 1);
            else
            {
                register bool * dirty asm("r4") = &renderer->dirty;
                asm("" : "+l"(dirty));
                if (!*dirty)
                {
                    UploadActivityFramePalette(renderer, frame->bytes + 16, 1);
                    *dirty = true;
                }
            }
            register u32 zero asm("r0") = 0;
            asm("" : "+l"(zero));
            register u8 * refresh_writer asm("r4") = refresh;
            asm("" : "+l"(refresh_writer));
            *refresh_writer = zero;
        }
    }
    ActivityDrawSprite * overlay_reader = self;
    asm("" : "+&l"(overlay_reader) : "l"(original_elevation) : "r0", "r1", "r2", "r3", "r4", "r6");
    register ActivityDrawOverlay * candidate asm("r0") = overlay_reader->overlay;
    asm("" : "+l"(candidate));
    if (candidate != nullptr && original_elevation >= 0)
    {
        register ActivityDrawOverlay * overlay asm("r6") = candidate;
        asm("" : "+l"(overlay));
        register ActivityDrawQueues * overlay_queues asm("r5") = queues;
        asm("" : "+l"(overlay_queues));
        register u32 overlay_priority asm("r3");
        {
            register u32 bits asm("r0") = 3;
            asm("" : "+l"(bits));
            register u32 input_layer asm("r1") = layer;
            asm("" : "+l"(input_layer));
            bits &= input_layer;
            overlay_priority = (bits << 2) | bits;
            asm("" : "+l"(overlay_priority));
            register u32 middle asm("r1") = bits << 4;
            asm("" : "+l"(middle));
            overlay_priority |= middle;
            bits <<= 6;
            asm("" : "+l"(bits));
            overlay_priority |= bits;
        }
        register ActivityDrawFrame * overlay_frame asm("r7") = &overlay->frame;
        asm("" : "+l"(overlay_frame));
        register void * sprite_reader asm("r4") = overlay_queues->sprites;
        asm("" : "+l"(sprite_reader));
        register void * sprite_queue asm("r8") = sprite_reader;
        asm("" : "+h"(sprite_queue));
        register G1MapGraphics * const * parameters asm("r0") = &overlay->renderer.graphics;
        asm("" : "+l"(parameters));
        register G1MapGraphics * graphics asm("r2") = *parameters++;
        asm("" : "+l"(parameters), "+l"(graphics));
        register i32 handle asm("r1") = reinterpret_cast<G1MapTileOwner const *>(parameters)->handle;
        asm("" : "+l"(handle));
        u32 queued;
        if ((-handle | handle) < 0)
        {
            register u32 tile asm("r0") = overlay->renderer.tile_index;
            asm("" : "+l"(tile));
            register u32 const * palette_count asm("r1") = &overlay->renderer.palette_count;
            asm("" : "+l"(palette_count));
            register i32 queue_depth asm("r4") = depth;
            asm("" : "+l"(queue_depth));
            queued = QueueActivityFrame(sprite_queue, screen_x, screen_y, overlay_priority, queue_depth, *overlay_frame, graphics, tile, *palette_count);
        }
        else
            queued = 0;
        if (queued != 0)
        {
            register u8 * refresh asm("r4") = &overlay->refresh_frame;
            asm("" : "+l"(refresh));
            if (*refresh != 0)
            {
                UploadActivityFrameTiles(&overlay->renderer, overlay_queues->tiles, overlay_frame->bytes + 8);
                UploadActivityFramePalette(&overlay->renderer, overlay_frame->bytes + 16, 1);
                *refresh = 0;
            }
        }
    }
}
