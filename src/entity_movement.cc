#include "prelude.h"

#include <stdlib.h>
#include <string.h>

struct ProbeRectangle
{
    i16 left;
    i16 top;
    i16 right;
    i16 bottom;
};

struct EntityCollisionGrid
{
    u32 const * tileFlags;
    u8 const * tileIndices;
    u16 width;
    u16 height;
};

struct EntityCollisionProbe
{
    int down;
    int up;
    int left;
    int right;
    bool downClear;
    bool upClear;
    bool leftClear;
    bool rightClear;
    EntityCollisionGrid grid;
    ProbeRectangle bounds;
    int step;
    int horizontalLookahead;
    int verticalLookahead;
};

EC SECTION(".text.080ABA3C") EntityCollisionProbe * InitializeEntityCollisionProbe(EntityCollisionProbe * probe)
{
    EntityCollisionProbe defaults;
    register EntityCollisionProbe * flagBase asm("r1") = &defaults;
    asm ("" : "+l" (flagBase));
    defaults.down = 0;
    defaults.up = 0;
    defaults.left = 0;
    defaults.right = 0;
    flagBase->downClear = false;
    flagBase->upClear = false;
    flagBase->leftClear = false;
    flagBase->rightClear = false;
    register EntityCollisionGrid * grid asm("r1") = &defaults.grid;
    asm ("" : "+l" (grid));
    defaults.grid.tileFlags = nullptr;
    grid->tileIndices = nullptr;
    grid->width = 0;
    grid->height = 0;
    register ProbeRectangle * bounds asm("r1") = &defaults.bounds;
    asm ("" : "+l" (bounds));
    bounds->left = 0;
    bounds->top = 0;
    bounds->right = 0;
    bounds->bottom = 0;
    defaults.step = 1;
    defaults.horizontalLookahead = 0;
    defaults.verticalLookahead = 0;
    asm ("" : : : "memory");
    defaults.downClear = true;
    defaults.upClear = true;
    defaults.leftClear = true;
    defaults.rightClear = true;
    memcpy(probe, &defaults, sizeof(defaults));
    return probe;
}

struct WanderChoice
{
    u8 weight;
    u8 minimum_seconds;
    u8 random_seconds;
    u8 padding;
};

EC SECTION(".text.080AB788") u32 RollEntityMovementRandom(u32 limit)
{
    u32 sample = rand() & 0xFFFF;
    return (sample * limit) >> 16;
}

EC SECTION(".text.080AB7A4") int RotateEntityMovementFacing(int facing, int turn)
{
    switch (facing)
    {
    case 0:
        switch (turn)
        {
        case 0: return 0;
        case 1: return 3;
        case 2: return 2;
        case 3: return 1;
        }
    case 1:
        switch (turn)
        {
        case 0: return 1;
        case 1: return 2;
        case 2: return 3;
        case 3: return 0;
        }
    case 2:
        switch (turn)
        {
        case 0: return 2;
        case 1: return 0;
        case 2: return 1;
        case 3: return 3;
        }
    case 3:
        switch (turn)
        {
        case 0: return 3;
        case 1: return 1;
        case 2: return 0;
        case 3: return 2;
        }
    }

    return facing;
}

EC SECTION(".text.080AB82C") u32 ChooseWeightedEntityMovement(WanderChoice const * choices, u32 count)
{
    u32 roll = RollEntityMovementRandom(100);
    u32 index = 0;
    u32 runningWeight = choices[0].weight;

check:
    if (roll >= runningWeight)
    {
        ++index;
        if (index < count)
        {
            asm("" : "+r"(index));
            runningWeight += choices[index].weight;
            goto check;
        }
        index = 0;
    }

    return index;
}

struct EntityTerrainGrid
{
    u32 const * tileProperties;
    u8 const * tileIndices;
    u16 width;
    u16 height;
};

struct EntityMovementRect
{
    i16 left;
    i16 top;
    i16 right;
    i16 bottom;
};

struct EntityMovement
{
    int positiveY;
    int negativeY;
    int negativeX;
    int positiveX;
    EntityTerrainGrid terrain;
    EntityMovementRect bounds;
    int maxDistance;
};

EC void func_080AC388(EntityTerrainGrid & terrain, EntityMovementRect const & bounds,
    int * positiveY, int * negativeY, int * negativeX, int * positiveX);

EC SECTION(".text.080ABBA0") void ResetEntityMovementPositiveY(EntityMovement & movement)
{
    movement.positiveY = movement.maxDistance;
    func_080AC388(movement.terrain, movement.bounds, &movement.positiveY, 0, 0, 0);
}

EC SECTION(".text.080ABBC4") void ResetEntityMovementNegativeY(EntityMovement & movement)
{
    movement.negativeY = -movement.maxDistance;
    func_080AC388(movement.terrain, movement.bounds, 0, &movement.negativeY, 0, 0);
}

EC SECTION(".text.080ABBEC") void ResetEntityMovementNegativeX(EntityMovement & movement)
{
    movement.negativeX = -movement.maxDistance;
    func_080AC388(movement.terrain, movement.bounds, 0, 0, &movement.negativeX, 0);
}

EC SECTION(".text.080ABC14") void ResetEntityMovementPositiveX(EntityMovement & movement)
{
    movement.positiveX = movement.maxDistance;
    func_080AC388(movement.terrain, movement.bounds, 0, 0, 0, &movement.positiveX);
}
