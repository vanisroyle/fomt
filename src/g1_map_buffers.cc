#include "g1_map.hh"

EC char const vtable_unk_080E82E4[];
EC char const vtable_unk_080E830C[];
EC char const vtable_unk_080E831C[];
EC void * __builtin_vec_new(u32);
EC void __builtin_vec_delete(void *);
EC void TaskNode_Destroy(G1MapTaskNode *, u32);
EC void func_080E43FC(void *);

static inline void ResetMapAnimation(G1MapAnimationState * animation)
{
    animation->frame = 0;
    animation->dirty = 0;
    animation->timer = 0;
}

static inline void InitializeMapTask(G1MapTaskNode * task)
{
    task->previous = nullptr;
    task->next = nullptr;
    task->vtable = vtable_unk_080E830C;
}

EC SECTION(".text.g1_080A4A94") G1MapState * InitializeMapBuffers(G1MapState * self)
{
    self->vtable = reinterpret_cast<G1MapVTable const *>(vtable_unk_080E82E4);
    self->type = 0x234;
    self->map_id = 66;
    self->camera_fixed[0] = 0;
    self->camera_fixed[1] = 0;
    self->unk_28 = 0;
    self->tilemaps_dirty = 0;
    self->palettes_dirty = 0;
    self->terrain = nullptr;
    ResetMapAnimation(&self->animations[0]);
    ResetMapAnimation(&self->animations[1]);
    ResetMapAnimation(&self->animations[2]);
    ResetMapAnimation(&self->animations[3]);
    ResetMapAnimation(&self->animations[4]);
    ResetMapAnimation(&self->animations[5]);
    ResetMapAnimation(&self->animations[6]);
    ResetMapAnimation(&self->animations[7]);
    ResetMapAnimation(&self->animations[8]);
    ResetMapAnimation(&self->animations[9]);
    self->animation_state = 0;
    InitializeMapTask(&self->task);
    self->camera_steps = 0;
    for (u32 i = 0; i < 3; i++)
        self->palettes[i] = __builtin_vec_new(480);
    for (u32 i = 0; i < 3; i++)
        self->tilemaps[i] = static_cast<u16 *>(__builtin_vec_new(0x7900));
    self->terrain = static_cast<TerrainInfo *>(__builtin_vec_new(0xf200));
    return self;
}

EC SECTION(".text.g1_080A4B6C") void DestroyMapBuffers(G1MapState * self, u32 flags)
{
    self->vtable = reinterpret_cast<G1MapVTable const *>(vtable_unk_080E82E4);
    if (self->terrain != nullptr)
        __builtin_vec_delete(self->terrain);
    for (u32 i = 0; i < 3; i++)
        if (self->tilemaps[i] != nullptr)
            __builtin_vec_delete(self->tilemaps[i]);
    for (u32 i = 0; i < 3; i++)
        if (self->palettes[i] != nullptr)
            __builtin_vec_delete(self->palettes[i]);
    TaskNode_Destroy(&self->task, 2);
    if (flags & 1)
        delete self;
}

EC SECTION(".text.g1_080A6474") G1WorldMapState * InitializeWorldMapBuffers(G1WorldMapState * self, G1MapSaveState * save)
{
    InitializeMapBuffers(self);
    self->vtable = reinterpret_cast<G1MapVTable const *>(vtable_unk_080E831C);
    self->save = save;
    self->refresh_stocking = false;
    func_080E43FC(self->helper);
    ResetMapAnimation(&self->extra_animations[0]);
    ResetMapAnimation(&self->extra_animations[1]);
    return self;
}
