#include "g1_map.hh"
#include "dma_queue.hh"
#include "farm_house.hh"

struct G1MapAnimationFrames
{
    u16 tiles_per_frame;
    u16 frame_count;
};

struct G1MapAnimationData
{
    u16 tile_index;
    u16 reserved;
    G1MapAnimationFrames const * frames;
    u16 delay;
    u16 reserved_0A;
};

struct G1MapSaveState
{
    u8 reserved[0x1f4];
    FarmHouse house;
    u8 reserved_house[0x21a0 - 0x1f4 - sizeof(FarmHouse)];
    u32 event_stage : 2;
};

EC G1MapAnimationData const gUnk_0810710C;
EC G1MapAnimationData const gUnk_08107114;
EC G1MapAnimationData const gUnk_08107120;
EC G1MapAnimationData const gUnk_0810712C;
EC G1MapAnimationData const gUnk_08107138;
EC G1MapAnimationData const gUnk_0810715C;
EC G1MapAnimationData const gUnk_08107168;
EC G1MapAnimationData const gUnk_08107174;
EC G1MapAnimationData const gUnk_08107180;
EC G1MapAnimationData const gUnk_0810718C;
EC G1MapAnimationData const gUnk_08107198;
EC G1MapAnimationData const gUnk_081071A4;
EC G1MapAnimationData const gUnk_081071B0;
EC G1MapAnimationData const gUnk_081071BC;
EC G1MapAnimationData const gUnk_081071C8;
EC G1MapAnimationData const gUnk_081071D4;
EC G1MapAnimationData const gUnk_081071E0;
EC G1MapAnimationData const gUnk_081071EC;
EC G1MapAnimationData const gUnk_081071F8;
EC G1MapAnimationData const gUnk_08107338;
EC G1MapAnimationData const gUnk_08107344;
EC void UpdateMapAnimationTimers(G1MapState *);
EC void func_080A4F50(G1MapState *, DmaTransferQueue *, void *, void *);
EC void func_080A5AB0(G1MapState *, G1MapAnimationData const *, u32, DmaTransferQueue *);

static inline void AdvanceMapAnimation(G1MapAnimationState * animation, G1MapAnimationData const * data)
{
    if (++animation->timer >= data->delay)
    {
        animation->timer = 0;
        animation->dirty = 1;
    }
}

EC SECTION(".text.g1_080A4BEC") void UpdateMapAnimationTimers(G1MapState * self)
{
    if (self->camera_steps != 0)
    {
        self->camera_fixed[0] += self->camera_velocity_x;
        self->camera_fixed[1] += self->camera_velocity_y;
        self->camera_steps--;
        self->tilemaps_dirty = true;
    }
    switch (self->map_id)
    {
    case 14:
        AdvanceMapAnimation(&self->animations[4], &gUnk_0810718C);
        AdvanceMapAnimation(&self->animations[5], &gUnk_08107198);
        AdvanceMapAnimation(&self->animations[6], &gUnk_081071A4);
        break;
    case 15:
        AdvanceMapAnimation(&self->animations[4], &gUnk_081071B0);
        AdvanceMapAnimation(&self->animations[5], &gUnk_081071BC);
        AdvanceMapAnimation(&self->animations[6], &gUnk_081071C8);
        break;
    case 60:
        AdvanceMapAnimation(&self->animations[8], &gUnk_081071F8);
        break;
    case 31:
        AdvanceMapAnimation(&self->animations[9], &gUnk_081071E0);
        break;
    case 8:
        AdvanceMapAnimation(&self->animations[7], &gUnk_081071D4);
        break;
    case 9:
        AdvanceMapAnimation(&self->animations[7], &gUnk_081071EC);
        break;
    case 6:
        AdvanceMapAnimation(&self->animations[2], &gUnk_0810715C);
        AdvanceMapAnimation(&self->animations[3], &gUnk_08107168);
        break;
    case 7:
        AdvanceMapAnimation(&self->animations[2], &gUnk_08107174);
        AdvanceMapAnimation(&self->animations[3], &gUnk_08107180);
        break;
    case 0: case 62: case 63:
        AdvanceMapAnimation(&self->animations[0], &gUnk_08107114);
        AdvanceMapAnimation(&self->animations[1], &gUnk_0810712C);
        break;
    case 1:
        AdvanceMapAnimation(&self->animations[0], &gUnk_08107120);
        AdvanceMapAnimation(&self->animations[1], &gUnk_08107138);
        break;
    }
}

EC SECTION(".text.g1_080A64B8") void UpdateWorldMapAnimationTimers(G1WorldMapState * self)
{
    UpdateMapAnimationTimers(self);
    G1MapSaveState * save = self->save;
    if (self->type == 6)
    {
        switch (save->event_stage)
        {
        case 1:
            AdvanceMapAnimation(&self->extra_animations[0], &gUnk_08107338);
            AdvanceMapAnimation(&self->extra_animations[1], &gUnk_08107344);
            break;
        }
    }
}

static inline void QueueWorldMapAnimation(G1WorldMapState * self, G1MapAnimationState * animation,
    G1MapAnimationData const * data, DmaTransferQueue * queue)
{
    if (animation->dirty)
    {
        animation->dirty = false;
        func_080A5AB0(self, data, animation->frame, queue);
        u32 length = data->frames->frame_count;
        bool wrapped = false;
        if (++animation->frame == length)
            wrapped = true;
        if (wrapped)
            animation->frame = 0;
    }
}

EC SECTION(".text.g1_080A6524") void QueueWorldMapAnimations(G1WorldMapState * self, DmaTransferQueue * queue,
    void * display, void * tasks)
{
    func_080A4F50(self, queue, display, tasks);
    G1MapSaveState const * save = self->save;
    switch (self->type)
    {
    case 29:
        if (self->refresh_stocking)
        {
            self->refresh_stocking = false;
            if (self->save->house.HasStocking())
            {
                if (self->save->house.GetStockingArticleId() == 95)
                    func_080A5AB0(self, &gUnk_0810710C, 1, queue);
                else
                    func_080A5AB0(self, &gUnk_0810710C, 2, queue);
            }
            else
                func_080A5AB0(self, &gUnk_0810710C, 0, queue);
        }
        break;
    case 6:
        if (save->event_stage == 1)
        {
            QueueWorldMapAnimation(self, &self->extra_animations[0], &gUnk_08107338, queue);
            QueueWorldMapAnimation(self, &self->extra_animations[1], &gUnk_08107344, queue);
        }
        break;
    }
}
