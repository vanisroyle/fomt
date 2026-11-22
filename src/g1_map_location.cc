#include "g1_map.hh"
#include "actor.hh"

EC void SetMapCameraPosition(G1MapState *, int, int);

EC SECTION(".text.g1_080A56DC") void LoadMapAtLocation(G1MapState * self, Location const * location, void * first, void * second, void * third)
{
    self->type = location->map;
    self->camera_fixed[0] = location->x << 16;
    self->camera_fixed[1] = location->y << 16;
    self->vtable->load_map(self, first, second, third);
    SetMapCameraPosition(self, location->x, location->y);
}
