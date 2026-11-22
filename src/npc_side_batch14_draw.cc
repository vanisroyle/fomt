#include "g1_map_resources.hh"

#define NPC_SIDE_CODE(address) SECTION(".text.npc_side." #address)

struct WorkerDrawServices;
struct WorkerDrawServicesMethods
{
    STRUCT_PAD(0x00, 0x58);
    void * (*get_overlay)(WorkerDrawServices *);
};
struct WorkerDrawServices { WorkerDrawServicesMethods * methods; };

struct WorkerDrawActor
{
    WorkerDrawServices * services;
    STRUCT_PAD(0x04, 0x08);
    u16 x_fraction;
    i16 x;
    u16 y_fraction;
    i16 y;
    STRUCT_PAD(0x10, 0x20);
    u8 facing;
    u8 priority;
};

struct WorkerDrawSprite
{
    WorkerDrawActor * actor;
    void const * vtable;
    G1AnimatedMapSpriteResource body;
    G1AnimatedMapSpriteResource held;
    u8 overlay;
    i8 overlay_offset;
    struct PACKED { u8 kind : 2; u8 frame : 6; } attachment;
    u8 priority;
};

struct WorkerDrawContext
{
    void * tiles;
    u32 reserved;
    void * sprites;
    i16 camera_x, camera_y;
};

struct WorkerDrawFrame { u8 bytes[0x20]; };
struct WorkerDrawFrameSource;
struct WorkerDrawFrameMethods
{
    STRUCT_PAD(0x00, 0x10);
    void (*get_frame)(WorkerDrawFrame *, WorkerDrawFrameSource *, u32);
};

struct WorkerDrawFrameSource { WorkerDrawFrameMethods * methods; };

struct WorkerDrawQueues { void * tiles; void * sprites; };

struct WorkerDrawOffset { i16 x, y; };
EC WorkerDrawOffset const gSpriteWorkerAttachmentOffsets[][4] asm("gUnk_080F1328");
EC u32 QueueWorkerDrawSprites(void *, i32, i32, u32, u32, WorkerDrawFrame const &, G1MapGraphics *, u32, u32 const &) asm("func_030004DC");

inline void UploadWorkerDrawFrameTiles(G1MapSpriteResource * renderer, void * queue, void const * tiles)
{
    register void * tile_queue asm("r1") = queue;
    register void const * source asm("r2") = tiles;
    register G1MapSpriteResource * target asm("r0") = renderer;
    asm volatile("bl func_080A480C" : "+r"(target), "+r"(tile_queue), "+r"(source) : : "r3", "r12", "lr", "cc", "memory");
}

inline void LoadWorkerDrawFramePalettes(G1MapSpriteResource * renderer, void const * palettes)
{
    register void const * source asm("r1") = palettes;
    register G1MapSpriteResource * target asm("r0") = renderer;
    register u32 mode asm("r2") = 1;
    asm volatile("bl LoadMapSpriteResourcePalettes" : "+r"(target), "+r"(source), "+r"(mode) : : "r3", "r12", "lr", "cc", "memory");
}

EC void DrawSpriteWorkerLayers(WorkerDrawSprite *, WorkerDrawContext *) NPC_SIDE_CODE(08032690);
EC void DrawSpriteWorkerLayers(WorkerDrawSprite * input_self, WorkerDrawContext * input_context)
{
    register WorkerDrawSprite * self asm("r6") = input_self;
    asm("" : "+l"(self));
    register WorkerDrawContext * context asm("r10") = input_context;
    asm("" : "+h"(context));
    WorkerDrawFrame frame_data;
    WorkerDrawQueues queues_data;
    WorkerDrawServices * draw_services;
    i32 screen_x;
    i32 screen_y;
    u32 depth;
    i32 attachment_x;
    register WorkerDrawActor * actor asm("r8");
    {
        register WorkerDrawActor * position asm("r0") = self->actor;
        actor = position;
        asm("" : : "h"(actor));
        register WorkerDrawServices * services asm("r1") = position->services;
        asm("" : "+l"(services));
        draw_services = services;
        register u32 actor_offset asm("r2") = 10;
        register i32 actor_coordinate asm("r1");
        asm("ldrsh %0, [%1, %2]" : "=l"(actor_coordinate) : "l"(position), "l"(actor_offset));
        register WorkerDrawContext * camera asm("r3") = context;
        register u32 camera_offset asm("r4") = 12;
        register i32 coordinate asm("r0");
        asm("ldrsh %0, [%1, %2]" : "=l"(coordinate) : "l"(camera), "l"(camera_offset));
        actor_coordinate -= coordinate;
        asm("" : "+l"(actor_coordinate));
        screen_x = actor_coordinate;
        position = actor;
        actor_offset = 14;
        asm("ldrsh %0, [%1, %2]" : "=l"(actor_coordinate) : "l"(position), "l"(actor_offset));
        camera_offset = 14;
        asm("ldrsh %0, [%1, %2]" : "=l"(coordinate) : "l"(camera), "l"(camera_offset));
        coordinate = actor_coordinate - coordinate;
        screen_y = coordinate;
        coordinate = 0x8000;
        asm("" : "+l"(coordinate));
        coordinate -= actor_coordinate;
        asm("" : "+l"(coordinate));
        depth = coordinate;
        register WorkerDrawQueues * queues asm("r2") = &queues_data;
        asm("" : "+l"(queues));
        register void * tiles asm("r0") = camera->tiles;
        register void * sprites asm("r1") = camera->sprites;
        asm("" : "+l"(tiles), "+l"(sprites));
        queues_data.tiles = tiles;
        queues->sprites = sprites;
    }
    register u32 attachment_bits asm("r1") = *reinterpret_cast<u8 *>(&self->attachment);
    asm("" : "+l"(attachment_bits));
    switch (i32((attachment_bits << 30) >> 30))
    {
    case 0: break;
    case 1:
    case 2:
    {
        register u32 facing asm("r0") = actor->facing;
        asm("" : "+l"(facing));
        attachment_bits >>= 2;
        register WorkerDrawOffset const * offset asm("r1") = &gSpriteWorkerAttachmentOffsets[attachment_bits][facing];
        asm("" : "+l"(offset));
        register u32 zero asm("r2") = 0;
        register i32 offset_x asm("r0");
        asm("ldrsh %0, [%1, %2]" : "=l"(offset_x) : "l"(offset), "l"(zero));
        register u32 y_offset asm("r3") = 2;
        register i32 offset_y asm("r1");
        asm("ldrsh %0, [%1, %2]" : "=l"(offset_y) : "l"(offset), "l"(y_offset));
        register G1AnimatedMapSpriteResource * resource asm("r5") = &self->held;
        register i32 held_x asm("r4") = screen_x;
        asm("" : "+l"(resource), "+l"(held_x));
        offset_x = held_x + offset_x;
        asm("" : "+l"(offset_x));
        attachment_x = offset_x;
        register i32 held_y asm("r0") = screen_y;
        asm("" : "+l"(held_y));
        held_y += offset_y;
        register i32 y asm("r9") = held_y;
        asm("" : "+l"(held_y), "+h"(y));
        {
            register SpriteAnimator const * animation asm("r0") = &self->held.animator;
            asm("" : "+l"(animation));
            register WorkerDrawFrameSource * source asm("r1") = reinterpret_cast<WorkerDrawFrameSource *>(const_cast<SpriteAnimationSource *>(self->held.animator.source));
            register WorkerDrawFrameMethods * methods asm("r3") = source->methods;
            asm("" : "+l"(source), "+l"(methods));
            register u32 index asm("r2") = animation->keyframe;
            index *= sizeof(SpriteKeyframe);
            asm("" : "+l"(index));
            register u8 const * keyframe asm("r0") = reinterpret_cast<u8 const *>(self->held.animator.clip.keyframes);
            keyframe += index;
            asm("" : "+l"(keyframe));
            index = reinterpret_cast<SpriteKeyframe const *>(keyframe)->frame;
            asm("" : "+l"(index));
            register WorkerDrawFrame * output asm("r0") = &frame_data;
            asm("" : "+l"(output));
            register void (*read_frame)(WorkerDrawFrame *, WorkerDrawFrameSource *, u32) asm("r3") = methods->get_frame;
            read_frame(output, source, index);
        }
        register WorkerDrawFrame * frame_register asm("r7") = &frame_data;
        WorkerDrawFrame * frame = &frame_data;
        register void * queue asm("r3") = queues_data.sprites;
        register G1MapGraphics * graphics asm("r2") = self->held.graphics;
        asm("" : "+l"(queue), "+l"(graphics) : "l"(frame_register));
        i32 handle = self->held.tiles.handle;
        u32 queued;
        if ((-handle | handle) < 0)
        {
            register u32 tile asm("r0") = resource->tile_index;
            register u32 const * palettes asm("r1") = &self->held.palette_count;
            asm("" : "+l"(tile), "+l"(palettes));
            queued = QueueWorkerDrawSprites(queue, attachment_x, y, 0x55, depth, *frame, graphics, tile, *palettes);
        }
        else
            queued = 0;
        if (queued != 0)
        {
            register u8 * refresh_address asm("r0") = &resource->refresh_frame;
            register u32 refresh_flag asm("r1") = *refresh_address;
            asm("" : "+l"(refresh_address), "+l"(refresh_flag));
            register u8 * refresh asm("r9") = refresh_address;
            asm("" : "+h"(refresh));
            if (refresh_flag != 0)
            {
                UploadWorkerDrawFrameTiles(resource, queues_data.tiles, frame->bytes + 8);
                if (resource->mode)
                    LoadWorkerDrawFramePalettes(resource, frame->bytes + 16);
                else
                {
                    bool * loaded = &resource->dirty;
                    if (!*loaded)
                    {
                        LoadWorkerDrawFramePalettes(resource, frame->bytes + 16);
                        *loaded = true;
                    }
                }
                register u32 cleared asm("r0") = 0;
                asm("" : "+l"(cleared));
                register u8 * target asm("r1") = refresh;
                *target = cleared;
            }
        }
        break;
    }
    }
    register u32 priority asm("r4");
    switch (self->priority)
    {
    case 0:
    default:
    {
        register u32 layer asm("r0") = actor->priority;
        asm("" : "+l"(layer));
        layer &= 3;
        priority = (layer << 2) | layer;
        asm("" : "+l"(priority));
        register u32 middle asm("r1") = layer << 4;
        asm("" : "+l"(middle));
        priority |= middle;
        layer <<= 6;
        asm("" : "+l"(layer));
        priority |= layer;
        break;
    }
    case 1: priority = 0x19; break;
    case 2: priority = 0x1A; break;
    }
    register G1AnimatedMapSpriteResource * resource asm("r5") = &self->body;
    asm("" : "+l"(resource));
    register WorkerDrawQueues * body_queues asm("r8");
    {
        register WorkerDrawQueues * queue_address asm("r2") = &queues_data;
        asm("" : "+l"(queue_address));
        body_queues = queue_address;
        asm("" : "+h"(body_queues));
    }
    {
        register WorkerDrawFrameSource * source asm("r1") = reinterpret_cast<WorkerDrawFrameSource *>(const_cast<SpriteAnimationSource *>(self->body.animator.source));
        asm("" : "+l"(source));
        register WorkerDrawFrameMethods * methods asm("r3") = source->methods;
        asm("" : "+l"(methods));
        register u32 index asm("r2") = self->body.animator.keyframe;
        asm("" : "+l"(index));
        index *= sizeof(SpriteKeyframe);
        asm("" : "+l"(index));
        register u8 const * keyframe asm("r0") = reinterpret_cast<u8 const *>(self->body.animator.clip.keyframes);
        asm("" : "+l"(keyframe));
        keyframe += index;
        asm("" : "+l"(keyframe));
        index = reinterpret_cast<SpriteKeyframe const *>(keyframe)->frame;
        asm("" : "+l"(index));
        register WorkerDrawFrame * output asm("r0") = &frame_data;
        asm("" : "+l"(output));
        register void (*read_frame)(WorkerDrawFrame *, WorkerDrawFrameSource *, u32) asm("r3") = methods->get_frame;
        asm("" : "+l"(read_frame));
        read_frame(output, source, index);
    }
    register WorkerDrawFrame * frame_register asm("r7") = &frame_data;
    WorkerDrawFrame * frame = &frame_data;
    register WorkerDrawQueues * read_queues asm("r3") = body_queues;
    asm("" : "+l"(read_queues) : "l"(frame_register));
    register void * queue asm("r3") = read_queues->sprites;
    register void * sprite_queue asm("r9") = queue;
    register G1MapGraphics * graphics asm("r2") = self->body.graphics;
    asm("" : "+h"(sprite_queue), "+l"(graphics));
    i32 handle = self->body.tiles.handle;
    u32 queued;
    if ((-handle | handle) < 0)
    {
        register u32 tile asm("r0") = resource->tile_index;
        asm("" : "+l"(tile));
        register u32 const * palettes asm("r1") = &self->body.palette_count;
        asm("" : "+l"(palettes));
        queued = QueueWorkerDrawSprites(sprite_queue, screen_x, screen_y, priority, depth, *frame, graphics, tile, *palettes);
    }
    else
        queued = 0;
    if (queued != 0)
    {
        register u8 * refresh_address asm("r0") = &resource->refresh_frame;
        register u32 refresh_flag asm("r1") = *refresh_address;
        asm("" : "+l"(refresh_address), "+l"(refresh_flag));
        register u8 * refresh asm("r9") = refresh_address;
        asm("" : "+h"(refresh));
        if (refresh_flag != 0)
        {
            register WorkerDrawQueues * tile_queues asm("r4") = body_queues;
            asm("" : "+l"(tile_queues));
            UploadWorkerDrawFrameTiles(resource, tile_queues->tiles, frame->bytes + 8);
            if (resource->mode)
                LoadWorkerDrawFramePalettes(resource, frame->bytes + 16);
            else
            {
                bool * loaded = &resource->dirty;
                if (!*loaded)
                {
                    LoadWorkerDrawFramePalettes(resource, frame->bytes + 16);
                    *loaded = true;
                }
            }
            register u32 cleared asm("r0") = 0;
            asm("" : "+l"(cleared));
            register u8 * target asm("r1") = refresh;
            *target = cleared;
        }
    }
    register u32 overlay asm("r4");
    switch (i32(self->overlay))
    {
    case 3: return;
    case 0: overlay = 0; break;
    default:
    case 1: overlay = 1; break;
    case 2: overlay = 2; break;
    }
    register WorkerDrawServices * overlay_services asm("r2") = draw_services;
    asm("" : "+l"(overlay_services));
    register void * overlay_renderer asm("r0") = overlay_services->methods->get_overlay(overlay_services);
    asm("" : "+l"(overlay_renderer));
    register WorkerDrawContext * overlay_context asm("r3") = context;
    asm("" : "+l"(overlay_context));
    register void * overlay_sprites asm("r1") = overlay_context->sprites;
    asm("" : "+l"(overlay_sprites));
    register i8 * offset_address asm("r2") = &self->overlay_offset;
    asm("" : "+l"(offset_address));
    register i32 offset asm("r3") = *offset_address;
    asm("" : "+l"(offset));
    register i32 overlay_y asm("r2") = screen_y;
    asm("" : "+l"(overlay_y));
    offset = overlay_y + offset;
    asm volatile("str %3, [sp]" : : "l"(overlay_renderer), "l"(overlay_sprites), "l"(offset), "l"(overlay) : "memory");
    register i32 overlay_x asm("r2") = screen_x;
    asm volatile("bl DrawFixedSpriteOverlay" : "+r"(overlay_renderer), "+r"(overlay_sprites), "+r"(overlay_x), "+r"(offset) : : "r12", "lr", "cc", "memory");
}
