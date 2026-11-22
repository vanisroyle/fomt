#include "g1_map.hh"

#include <string.h>

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

enum MineMapTileState
{
    MineSoil,
    MineDugSoil,
    MineDownStairs,
    MineUpStairs,
    MineRock,
};

struct MineTileGraphics
{
    u16 rows[3][2];
};

EC SECTION(".text.080AA540") void ApplyMineTileToMap(G1MapState * map, unsigned x, unsigned y, MineMapTileState state, MineTileGraphics const * graphics)
{
    register G1MapState * buffers asm("sb") = map;
    asm ("" : "+h" (buffers));
    register unsigned column asm("sl") = x;
    asm ("" : "+h" (column));
    unsigned row = y;
    asm ("" : "+l" (row), "+h" (buffers), "+h" (column));
    volatile MineMapTileState tileState = state;
    register MineTileGraphics const * art asm("r5") = graphics;
    asm ("" : "+l" (art));
    register unsigned width asm("r8") = GetMapData(map->map_id)->width;
    asm ("" : "+h" (width));
    if (art)
    {
        register unsigned priorRow asm("r0") = row - 1;
        asm ("" : "+l" (priorRow));
        register unsigned topIndex asm("r1") = width;
        asm ("" : "+l" (topIndex));
        topIndex *= priorRow;
        asm ("" : "+l" (topIndex));
        register unsigned topOffset asm("r0") = topIndex;
        asm ("" : "+l" (topOffset));
        register G1MapState * topMap asm("r2") = buffers;
        asm ("" : "+l" (topMap));
        register u16 * upperLayer asm("r1") = topMap->tilemaps[2];
        asm ("" : "+l" (upperLayer));
        topOffset *= sizeof(u16);
        asm ("" : "+l" (topOffset));
        register unsigned topColumn asm("r3") = column;
        asm ("" : "+l" (topColumn));
        register unsigned columnOffset asm("r4") = topColumn * sizeof(u16);
        asm ("" : "+l" (columnOffset));
        asm ("add %0, %1, %0" : "+l" (upperLayer) : "l" (columnOffset) : "cc");
        asm ("add %0, %0, %1" : "+l" (topOffset) : "l" (upperLayer) : "cc");
        memcpy(reinterpret_cast<u8 *>(topOffset), art->rows[0], sizeof(art->rows[0]));

        register unsigned middleIndex asm("r0") = width;
        asm ("" : "+l" (middleIndex));
        middleIndex *= row;
        asm ("" : "+l" (middleIndex));
        register G1MapState * middleMap asm("r2") = buffers;
        asm ("" : "+l" (middleMap));
        register u16 * middleLayer asm("r1") = middleMap->tilemaps[1];
        asm ("" : "+l" (middleLayer));
        middleIndex *= sizeof(u16);
        asm ("" : "+l" (middleIndex));
        asm ("add %0, %1, %0" : "+l" (middleLayer) : "l" (columnOffset) : "cc");
        asm ("add %0, %0, %1" : "+l" (middleIndex) : "l" (middleLayer) : "cc");
        memcpy(reinterpret_cast<u8 *>(middleIndex), art->rows[1], sizeof(art->rows[1]));

        register unsigned nextRow asm("r0") = row + 1;
        asm ("" : "+l" (nextRow));
        register unsigned bottomIndex asm("r3") = width;
        asm ("" : "+l" (bottomIndex));
        bottomIndex *= nextRow;
        asm ("" : "+l" (bottomIndex));
        register unsigned bottomOffset asm("r0") = bottomIndex;
        asm ("" : "+l" (bottomOffset));
        register G1MapState * bottomMap asm("r2") = buffers;
        asm ("" : "+l" (bottomMap));
        register u16 * bottomLayer asm("r1") = bottomMap->tilemaps[1];
        asm ("" : "+l" (bottomLayer));
        bottomOffset *= sizeof(u16);
        asm ("" : "+l" (bottomOffset));
        columnOffset += reinterpret_cast<unsigned>(bottomLayer);
        asm ("" : "+l" (columnOffset));
        asm ("add %0, %0, %1" : "+l" (bottomOffset) : "l" (columnOffset) : "cc");
        register MineTileGraphics const * bottomArt asm("r1") = art;
        asm ("" : "+l" (bottomArt));
        memcpy(reinterpret_cast<u8 *>(bottomOffset), bottomArt->rows[2], sizeof(art->rows[2]));
        register G1MapState * dirtyMap asm("r1") = buffers;
        asm ("" : "+l" (dirtyMap));
        register u8 * dirty asm("r1") = &dirtyMap->tilemaps_dirty;
        asm ("" : "+l" (dirty));
        *dirty = 1;
    }

    register unsigned clearSolid asm("r3") = 0;
    asm ("" : "+l" (clearSolid));
    register unsigned solid asm("ip") = clearSolid;
    asm ("" : "+h" (solid));
    register unsigned rowIndex asm("r6") = width;
    asm ("" : "+l" (rowIndex));
    rowIndex *= row;
    asm ("" : "+l" (rowIndex));
    register unsigned nextRow asm("r5") = row + 1;
    asm ("" : "+l" (nextRow));

    register u32 * upper asm("r2");
    register u32 * lower asm("r1");
    register u32 mask asm("r4");
    register u32 warp asm("r3");
    register u32 terrainValue asm("r0");

    register MineMapTileState selectedState asm("r0") = tileState;
    asm ("" : "+l" (selectedState));
    switch (selectedState)
    {
    case MineSoil:
    case MineDugSoil:
        goto updateSolidity;
    case MineDownStairs:
        {
            rowIndex = width;
            asm ("" : "+l" (rowIndex));
            rowIndex *= row;
            asm ("" : "+l" (rowIndex));
            register unsigned terrainColumn asm("r1") = column;
            asm ("" : "+l" (terrainColumn));
            register unsigned upperIndex asm("r2") = terrainColumn + rowIndex;
            asm ("" : "+l" (upperIndex));
            register G1MapState * terrainMap asm("r3") = buffers;
            asm ("" : "+l" (terrainMap));
            register u32 * terrain asm("r0") = reinterpret_cast<u32 *>(terrainMap->terrain);
            asm ("" : "+l" (terrain));
            upperIndex *= sizeof(u32);
            asm ("add %0, %1, %2" : "=l" (upper) : "l" (upperIndex), "l" (terrain) : "cc");
            asm ("add %0, %1, #1" : "=l" (nextRow) : "l" (row) : "cc");
            register unsigned lowerIndex asm("r1") = width;
            asm ("" : "+l" (lowerIndex));
            lowerIndex *= nextRow;
            asm ("" : "+l" (lowerIndex));
            lowerIndex += column;
            asm ("" : "+l" (lowerIndex));
            lowerIndex *= sizeof(u32);
            asm ("add %0, %1, %2" : "=l" (lower) : "l" (lowerIndex), "l" (terrain) : "cc");
            terrainValue = lower[1];
            mask = 0xfffe0003;
            asm ("" : "+l" (terrainValue), "+l" (mask));
            terrainValue &= mask;
            asm ("" : "+l" (terrainValue));
            warp = 0x0f48;
            asm ("" : "+l" (warp));
            goto applyWarp;
        }
    case MineUpStairs:
        {
            rowIndex = width;
            asm ("" : "+l" (rowIndex));
            rowIndex *= row;
            asm ("" : "+l" (rowIndex));
            register unsigned terrainColumn asm("r1") = column;
            asm ("" : "+l" (terrainColumn));
            register unsigned upperIndex asm("r2") = terrainColumn + rowIndex;
            asm ("" : "+l" (upperIndex));
            register G1MapState * terrainMap asm("r3") = buffers;
            asm ("" : "+l" (terrainMap));
            register u32 * terrain asm("r0") = reinterpret_cast<u32 *>(terrainMap->terrain);
            asm ("" : "+l" (terrain));
            upperIndex *= sizeof(u32);
            asm ("add %0, %1, %2" : "=l" (upper) : "l" (upperIndex), "l" (terrain) : "cc");
            asm ("add %0, %1, #1" : "=l" (nextRow) : "l" (row) : "cc");
            register unsigned lowerIndex asm("r1") = width;
            asm ("" : "+l" (lowerIndex));
            lowerIndex *= nextRow;
            asm ("" : "+l" (lowerIndex));
            lowerIndex += column;
            asm ("" : "+l" (lowerIndex));
            lowerIndex *= sizeof(u32);
            asm ("add %0, %1, %2" : "=l" (lower) : "l" (lowerIndex), "l" (terrain) : "cc");
            terrainValue = lower[1];
            mask = 0xfffe0003;
            asm ("" : "+l" (terrainValue), "+l" (mask));
            terrainValue &= mask;
            asm ("" : "+l" (terrainValue));
            warp = 0x0f4c;
            asm ("" : "+l" (warp));
        }
applyWarp:
        lower[1] = terrainValue | warp;
        lower[0] = (lower[0] & mask) | warp;
        upper[1] = (upper[1] & mask) | warp;
        upper[0] = (upper[0] & mask) | warp;
        solid = 0;
        asm ("" : "+h" (solid));
        goto updateSolidity;

    case MineRock:
        {
            register unsigned rockSolid asm("r1") = 1;
            asm ("" : "+l" (rockSolid));
            solid = rockSolid;
            asm ("" : "+h" (solid));
        }
        rowIndex = width;
        asm ("" : "+l" (rowIndex));
        rowIndex *= row;
        asm ("" : "+l" (rowIndex));
        asm ("add %0, %1, #1" : "=l" (nextRow) : "l" (row) : "cc");
        goto updateSolidity;
    default:
        goto updateSolidity;
    }
updateSolidity:
    register unsigned terrainColumn asm("r2") = column;
    asm ("" : "+l" (terrainColumn));
    register unsigned upperIndex asm("r4") = terrainColumn + rowIndex;
    asm ("" : "+l" (upperIndex));
    register G1MapState * terrainMap asm("r3") = buffers;
    asm ("" : "+l" (terrainMap));
    register u32 * terrain asm("r0") = reinterpret_cast<u32 *>(terrainMap->terrain);
    asm ("" : "+l" (terrain));
    upperIndex *= sizeof(u32);
    register u32 * upperTerrain asm("r4");
    asm ("add %0, %1, %2" : "=l" (upperTerrain) : "l" (upperIndex), "l" (terrain) : "cc");
    register unsigned lowerIndex asm("r1") = width;
    asm ("" : "+l" (lowerIndex));
    lowerIndex *= nextRow;
    asm ("" : "+l" (lowerIndex));
    lowerIndex += column;
    asm ("" : "+l" (lowerIndex));
    lowerIndex *= sizeof(u32);
    register u32 * lowerTerrain asm("r1");
    asm ("add %0, %1, %2" : "=l" (lowerTerrain) : "l" (lowerIndex), "l" (terrain) : "cc");

    register unsigned oldFlags asm("r3") = reinterpret_cast<u8 *>(lowerTerrain)[4];
    asm ("" : "+l" (oldFlags));
    register unsigned solidMask asm("r2") = ~1u;
    asm ("" : "+l" (solidMask));
    register unsigned modified asm("r0") = solidMask;
    asm ("" : "+l" (modified));
    modified &= oldFlags;
    asm ("" : "+l" (modified));
    register unsigned lowerRightSolid asm("r3") = solid;
    asm ("" : "+l" (lowerRightSolid));
    reinterpret_cast<u8 *>(lowerTerrain)[4] = modified | lowerRightSolid;

    register unsigned isSolid asm("r5") = solid;
    asm ("" : "+l" (isSolid));
    oldFlags = reinterpret_cast<u8 *>(lowerTerrain)[0];
    asm ("" : "+l" (oldFlags));
    modified = solidMask;
    asm ("" : "+l" (modified));
    modified &= oldFlags;
    asm ("" : "+l" (modified));
    reinterpret_cast<u8 *>(lowerTerrain)[0] = modified | isSolid;

    register unsigned upperFlags asm("r1") = reinterpret_cast<u8 *>(upperTerrain)[4];
    asm ("" : "+l" (upperFlags));
    modified = solidMask;
    asm ("" : "+l" (modified));
    modified &= upperFlags;
    asm ("" : "+l" (modified));
    reinterpret_cast<u8 *>(upperTerrain)[4] = modified | isSolid;

    register unsigned upperLeft asm("r0") = reinterpret_cast<u8 *>(upperTerrain)[0];
    asm ("" : "+l" (upperLeft));
    solidMask &= upperLeft;
    asm ("" : "+l" (solidMask));
    reinterpret_cast<u8 *>(upperTerrain)[0] = solidMask | isSolid;
}
