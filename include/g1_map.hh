#ifndef G1_MAP_HH
#define G1_MAP_HH

#include "unknown_types.hh"

struct G1MapState;

struct G1MapCameraCoordinates
{
    u16 x_fraction;
    i16 x;
    u16 y_fraction;
    i16 y;
};

struct G1MapAnimationState
{
    u8 frame;
    u8 dirty;
    u8 timer;
    u8 reserved;
};

struct G1MapTaskNode
{
    void * previous;
    void * next;
    void const * vtable;
};

struct G1MapVTable
{
    void (*reserved[5])();
    void (*load_map)(G1MapState *, void *, void *, void *);
    void (*reserved_18[3])();
    void (*reload_palette)(G1MapState *, int, u32);
};

struct G1MapState
{
    u32 type;
    i32 map_id;
    union
    {
        u8 unk_08[8];
        G1MapCameraCoordinates camera;
        i32 camera_fixed[2];
    };
    u16 * tilemaps[3];
    void * palettes[3];
    u8 unk_28;
    u8 tilemaps_dirty;
    u8 palettes_dirty;
    u8 unk_2B;
    TerrainInfo * terrain;
    G1MapAnimationState animations[10];
    u8 animation_state;
    u8 reserved_59[3];
    G1MapTaskNode task;
    u8 reserved_68[0x1c];
    i32 camera_velocity_x;
    i32 camera_velocity_y;
    u32 camera_steps;
    G1MapVTable const * vtable;
};

struct G1MapSaveState;

struct G1WorldMapState : public G1MapState
{
    G1MapSaveState * save;
    bool refresh_stocking;
    u8 reserved_99[3];
    u8 helper[24];
    G1MapAnimationState extra_animations[2];
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

struct PACKED ALIGN(2) G1MapAvailability
{
    u8 bits[7];
    u32 count_60 : 3;
    u32 count_61 : 3;
    u32 count_62 : 3;
    u32 count_63 : 3;
    u32 count_64 : 3;
    u32 count_65 : 3;
    u32 count_66 : 3;
    u32 count_67 : 3;
    u32 count_68 : 3;
    u32 count_69 : 3;
    u32 count_6A : 3;
    u32 count_6B : 3;
    u32 count_6C : 3;
    u32 count_6D : 3;
    u32 count_6E : 3;
};

EC MapData * GetMapData(u32 map);
EC void ApplyMapPatch(G1MapState *, G1MapPatch const *, u32 x, u32 y);

#endif
