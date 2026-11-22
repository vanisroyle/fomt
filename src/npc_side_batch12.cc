#include "entity_actor.hh"
#include "g1_map_resources.hh"

#define NPC_EXTRA_CODE(address) SECTION(".text.npc_extra." #address)

struct NpcSpriteFrame
{
    u8 bytes[0x20];
};

struct NpcFrameSprite
{
    AEntity * actor;
    void const * vtable;
    G1MapSpriteResource renderer;
    NpcSpriteFrame frame;
    u8 refresh_frame;
    STRUCT_PAD(0x51, 0x54);
    u8 raised;
};

struct NpcSpriteDrawContext
{
    void * tile_queue;
    u32 reserved;
    void * sprites;
    i16 camera_x;
    i16 camera_y;
};

EC u32 QueueNpcFrameSprites(void * sprites, i32 x, i32 y, u32 priority, u32 depth, NpcSpriteFrame const & frame, G1MapGraphics * graphics, u32 tile, u32 const & palettes) asm("func_030004DC");
inline void UploadNpcFrameTiles(G1MapSpriteResource * renderer, void * queue, void const * tiles)
{
    register void * tile_queue asm("r1") = queue;
    register void const * source asm("r2") = tiles;
    register G1MapSpriteResource * target asm("r0") = renderer;
    asm volatile("bl func_080A480C" : "+r"(target), "+r"(tile_queue), "+r"(source) : : "r3", "r12", "lr", "cc", "memory");
}

inline void LoadNpcFramePalettes(G1MapSpriteResource * renderer, void const * palettes)
{
    register void const * source asm("r1") = palettes;
    register G1MapSpriteResource * target asm("r0") = renderer;
    register u32 flag asm("r2") = 1;
    asm volatile("bl LoadMapSpriteResourcePalettes" : "+r"(target), "+r"(source), "+r"(flag) : : "r3", "r12", "lr", "cc", "memory");
}

struct NpcDrawQueues
{
    void * tiles;
    void * sprites;

    NpcDrawQueues(void * tile_queue, void * sprite_queue) : tiles(tile_queue), sprites(sprite_queue) {}
};

inline u32 QueueVisibleNpcFrame(G1MapSpriteResource * renderer, i32 handle, void * sprites, i32 x, i32 y, u32 depth, NpcSpriteFrame * frame, G1MapGraphics * graphics)
{
    if ((-handle | handle) < 0)
    {
        u32 tile = renderer->tile_index;
        u32 const * palettes = &renderer->palette_count;
        return QueueNpcFrameSprites(sprites, x, y, 0xAA, depth, *frame, graphics, tile, *palettes);
    }
    return 0;
}

inline u8 * GetNpcFrameRefresh(G1MapSpriteResource * renderer)
{
    return reinterpret_cast<u8 *>(renderer) + 0x48;
}

EC void DrawScheduledNpcFrame(NpcFrameSprite * self, NpcSpriteDrawContext * context) NPC_EXTRA_CODE(08037D74);
EC void DrawScheduledNpcFrame(NpcFrameSprite * self, NpcSpriteDrawContext * context)
{
    i32 x = (self->actor->x_q16 >> 16) - context->camera_x;
    i32 actor_y = self->actor->y_q16 >> 16;
    i32 y = actor_y - context->camera_y;
    void * tiles = context->tile_queue;
    void * sprites = context->sprites;
    NpcDrawQueues queues(tiles, sprites);
    asm("" : "+m"(queues));
    u32 depth = 0x8000 - actor_y;
    do
    {
        G1MapSpriteResource * renderer = &self->renderer;
        NpcSpriteFrame * frame = &self->frame;
        G1MapGraphics * graphics = renderer->graphics;
        u32 queued = QueueVisibleNpcFrame(renderer, self->renderer.tiles.handle, sprites, x, y, depth, frame, graphics);
        if (queued != 0)
        {
            u8 * refresh_frame = GetNpcFrameRefresh(renderer);
            if (*refresh_frame != 0)
            {
                UploadNpcFrameTiles(renderer, queues.tiles, frame->bytes + 8);
                LoadNpcFramePalettes(renderer, frame->bytes + 16);
                *refresh_frame = 0;
            }
        }
    } while (0);
}

EC void DrawRaisedNpcFrame(NpcFrameSprite * self, NpcSpriteDrawContext * context) NPC_EXTRA_CODE(08037F7C);
EC void DrawRaisedNpcFrame(NpcFrameSprite * self, NpcSpriteDrawContext * context)
{
    i32 x = (self->actor->x_q16 >> 16) - context->camera_x;
    i32 actor_y = self->actor->y_q16 >> 16;
    i32 y = actor_y - context->camera_y;
    u8 * frame_bytes;
    void * tiles = context->tile_queue;
    void * sprites = context->sprites;
    NpcDrawQueues queues(tiles, sprites);
    asm("" : "+m"(queues));
    if (self->raised != 0)
    {
        y -= 2;
    }
    u32 depth = 0x8000 - actor_y;
    {
        G1MapSpriteResource * renderer = &self->renderer;
        NpcSpriteFrame * frame = &self->frame;
        G1MapGraphics * graphics = renderer->graphics;
        u32 queued = QueueVisibleNpcFrame(renderer, self->renderer.tiles.handle, sprites, x, y, depth, frame, graphics);
        if (queued != 0)
        {
            u8 * refresh_frame = GetNpcFrameRefresh(renderer);
            if (*refresh_frame != 0)
            {
                UploadNpcFrameTiles(renderer, queues.tiles, frame->bytes + 8);
                {
                    frame_bytes = frame->bytes;
                    LoadNpcFramePalettes(renderer, frame_bytes + 16);
                }
                *refresh_frame = 0;
            }
        }
    }
}
