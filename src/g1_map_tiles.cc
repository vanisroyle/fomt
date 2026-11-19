#include "g1_map.hh"
#include "utility/vec2.hh"

EC SECTION(".text.g1_080A5F78") void ClearMapTileRectangle(G1MapState * self, u32 layer, Vec2 const * start, u32 width, u32 height)
{
    u16 stride = GetMapData(self->map_id)->width;
    for (u32 y = 0; y < height; y++)
        for (u32 x = 0; x < width; x++)
        {
            u32 index = start->x + x + (start->y + y) * stride;
            u32 offset = index * 2;
            *reinterpret_cast<u16 *>(reinterpret_cast<u8 *>(self->tilemaps[layer]) + offset) = 0x3ff;
            self->terrain[index].solid = 0;
            self->terrain[index].interaction_script = 0;
        }
    self->tilemaps_dirty = 1;
}
