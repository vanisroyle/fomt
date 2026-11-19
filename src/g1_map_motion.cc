#include "g1_map.hh"
#include "utility/vec2.hh"

EC u32 func_0803A8A4(u32, u32, u32, u32, u32);

EC SECTION(".text.g1_080A58EC") Vec2 ClampMapCameraPosition(G1MapState const * self, int x, int y)
{
    int old_x = self->camera.x;
    int old_y = self->camera.y;
    int center_x = old_x + 120;
    int center_y = old_y + 80;
    MapData const * map = GetMapData(self->map_id);
    int width = map->width * 8;
    int height = map->height * 8;
    int delta_x = x - center_x;
    int delta_y = y - center_y;
    int new_x = old_x + delta_x;
    if (new_x < 0)
        new_x = 0;
    else if (new_x > width - 240)
        new_x = width - 240;
    int new_y = old_y + delta_y;
    if (new_y < 0)
        new_y = 0;
    else if (new_y > height - 160)
        new_y = height - 160;
    return Vec2(new_x, new_y);
}

EC SECTION(".text.g1_080A6420") u32 GetDefaultMapGraphicsId(G1MapState const * self)
{
    return func_0803A8A4(self->type, 1, 0, 0, 0);
}
