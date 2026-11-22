#include "g1_map.hh"
#include <string.h>

struct G1CameraPoint
{
    G1CameraPoint(G1CameraPoint const & other) { memcpy(this, &other, sizeof(*this)); }
    i16 x;
    i16 y;
};

EC G1CameraPoint ClampMapCameraPosition(G1MapState const *, int, int);
EC u16 Sqrt(u32);

EC SECTION(".text.g1_080A5960") void SetMapCameraPosition(G1MapState * self, int x, int y)
{
    G1CameraPoint clamped = ClampMapCameraPosition(self, x, y);
    G1CameraPoint target(clamped);
    G1CameraPoint const * point = &target;
    G1CameraPoint const * vertical = &target;
    if (self->camera.x != point->x)
    {
        self->tilemaps_dirty = true;
        self->camera_fixed[0] = point->x;
        self->camera_fixed[0] <<= 16;
    }
    int old_y = self->camera.y;
    if (old_y != vertical->y)
    {
        self->tilemaps_dirty = true;
        self->camera_fixed[1] = vertical->y << 16;
    }
}

EC SECTION(".text.g1_080A59BC") void StartMapCameraPan(G1MapState * self, int x, int y, u32 speed)
{
    G1CameraPoint clamped = ClampMapCameraPosition(self, x, y);
    G1CameraPoint target(clamped);
    G1CameraPoint const * point = &target;
    if (speed == 0)
        speed = 1;
    int old_x = self->camera.x;
    int dx = point->x - old_x;
    int old_y = self->camera.y;
    int dy = point->y - old_y;
    if (dx != 0 || dy != 0)
    {
        u32 squared_distance = dx * dx + dy * dy;
        u32 shift = 0;
        while ((squared_distance << (shift * 2)) <= 0x3fffffff)
            shift++;
        squared_distance <<= shift * 2;
        u32 steps = (Sqrt(squared_distance) >> shift) / speed;
        if (steps == 0)
            steps = 1;
        self->camera_velocity_x = ((point->x << 16) - self->camera_fixed[0]) / static_cast<int>(steps);
        self->camera_velocity_y = ((point->y << 16) - self->camera_fixed[1]) / static_cast<int>(steps);
        self->camera_steps = steps;
    }
    else
        self->camera_steps = 0;
}
