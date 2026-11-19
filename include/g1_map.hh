#ifndef G1_MAP_HH
#define G1_MAP_HH

#include "unknown_types.hh"

struct G1MapState
{
    u32 type;
    i32 map_id;
    u8 unk_08[8];
    u16 * tilemaps[3];
    void * palettes[3];
    u8 unk_28;
    u8 tilemaps_dirty;
    u8 palettes_dirty;
    u8 unk_2B;
    TerrainInfo * terrain;
};

struct G1MapPatch
{
    u8 width;
    u8 height;
    u8 reserved[2];
    u16 const * tilemaps[3];
    TerrainInfo const * terrain;
    u8 const * terrain_map;
};

struct G1MapAvailability
{
    u8 bits[7];
};

EC MapData * GetMapData(u32 map);
EC void ApplyMapPatch(G1MapState *, G1MapPatch const *, u32 x, u32 y);

#endif
