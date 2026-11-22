#include "g1_map.hh"
#include <algorithm>

struct TerrainMapVTable
{
    void (*reserved[6])();
    u32 (*get_map_id)(G1MapState *);
    void (*reserved_1C)();
    void (*reload)(G1MapState *);
};

EC void ReloadMapPalettePair(G1MapState *, u32);
EC void func_080A5DFC(G1MapState *, u32, u32);

static inline void ResetTerrainAnimation(G1MapAnimationState * animation)
{
    animation->frame = 0;
    animation->dirty = 0;
}

EC SECTION(".text.g1_080A5760") void InitializeMapTerrain(G1MapState * self, u32 time, u32 color, u32 blend)
{
    self->map_id = reinterpret_cast<TerrainMapVTable const *>(self->vtable)->get_map_id(self);
    MapData const * map = GetMapData(self->map_id);
    if (map->terrain_map != nullptr && map->terrain_info != nullptr)
    {
        for (u32 y = 0; y < map->height; y++)
            for (u32 x = 0; x < map->width; x++)
            {
                u32 index = x + y * map->width;
                u32 tile = map->terrain_map[index];
                self->terrain[index] = map->terrain_info[tile];
            }
    }
    else
    {
        TerrainInfo zero = {};
        std::fill(self->terrain, self->terrain + 0x3c80, zero);
    }
    reinterpret_cast<TerrainMapVTable const *>(self->vtable)->reload(self);
    ReloadMapPalettePair(self, time);
    func_080A5DFC(self, color, blend);
    self->unk_28 = 1;
    ResetTerrainAnimation(&self->animations[0]);
    ResetTerrainAnimation(&self->animations[1]);
    ResetTerrainAnimation(&self->animations[2]);
    ResetTerrainAnimation(&self->animations[3]);
    ResetTerrainAnimation(&self->animations[4]);
    ResetTerrainAnimation(&self->animations[5]);
    ResetTerrainAnimation(&self->animations[6]);
    ResetTerrainAnimation(&self->animations[7]);
    ResetTerrainAnimation(&self->animations[8]);
    ResetTerrainAnimation(&self->animations[9]);
    self->animation_state = 0;
}
