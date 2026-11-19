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
    u8 downClear;
    u8 upClear;
    u8 leftClear;
    u8 rightClear;
    EntityCollisionGrid grid;
    ProbeRectangle bounds;
    int step;
    int horizontalLookahead;
    int verticalLookahead;
};

EC void func_080AC124(EntityCollisionGrid const &, ProbeRectangle const &, int *, int *, int *, int *);

EC SECTION(".text.080AC070") u32 DoesEntityCollisionRectangleHitSolidTile(EntityCollisionGrid const & grid, ProbeRectangle const & rectangle)
{
    volatile int firstX = rectangle.left >> 3;
    register int endX asm("r8") = (rectangle.right + 7) >> 3;
    asm ("" : "+h" (endX));
    int firstY = rectangle.top >> 3;
    register int endY asm("sl") = (rectangle.bottom + 7) >> 3;
    asm ("" : "+h" (endY));
    u32 const * flags = grid.tileFlags;
    asm ("" : "+l" (flags));
    register unsigned width asm("sb") = grid.width;
    asm ("" : "+h" (width));
    register u8 const * indices asm("ip");
    int y;
    if (!flags)
        goto solid;
    {
        register int x asm("r1") = firstX;
        asm ("" : "+l" (x));
        if (x < 0 || static_cast<unsigned>(endX) > width || firstY < 0)
            goto solid;
    }
    register unsigned height asm("r4");
    height = grid.height;
    asm ("" : "+l" (height));
    if (endY > static_cast<int>(height))
        goto solid;
    indices = grid.tileIndices;
    asm ("" : "+h" (indices));
    y = firstY;
    if (y < endY)
    {
        unsigned row = y * width;
        do
        {
            register int x asm("r2") = firstX;
            asm ("" : "+l" (x));
            if (x < endX)
            {
                register unsigned cell asm("r0");
                asm ("add %0, %1, %2" : "=l" (cell) : "l" (x), "l" (row) : "cc");
                register unsigned tileOffset asm("r4") = cell * sizeof(u32);
                asm ("" : "+l" (tileOffset));
                register u8 const * indexBase asm("r1") = indices;
                asm ("" : "+l" (indexBase));
                register u8 const * indexRow asm("r0");
                asm ("add %0, %1, %2" : "=l" (indexRow) : "l" (row), "l" (indexBase) : "cc");
                register u8 const * index asm("r3");
                asm ("add %0, %1, %2" : "=l" (index) : "l" (x), "l" (indexRow) : "cc");
                do
                {
                    u32 const * tile = indices ? flags + *index
                        : reinterpret_cast<u32 const *>(reinterpret_cast<u8 const *>(flags) + tileOffset);
                    asm ("" : "+l" (tile));
                    u32 bits = *tile;
                    if (bits << 31)
                        goto solid;
                    if (static_cast<i32>(bits << 30) < 0)
                        goto solid;
                    tileOffset += sizeof(u32);
                    ++index;
                    asm ("" : "+l" (tileOffset), "+l" (index));
                }
                while (++x < endX);
            }
            row += width;
        }
        while (++y < endY);
    }
    return 0;
solid:
    return 1;
}

EC SECTION(".text.080AC5D0") bool AreEntityFootTilesMarked(EntityCollisionGrid const & grid, int pixelX, int pixelY)
{
    register EntityCollisionGrid const * collisionGrid asm("r4") = &grid;
    asm ("" : "+l" (collisionGrid));
    register int topPixel asm("r0") = pixelY - 8;
    asm ("" : "+l" (topPixel));
    register int firstY asm("r3") = topPixel >> 3;
    asm ("" : "+l" (firstY));
    register int firstX asm("sl") = (pixelX - 6) >> 3;
    asm ("" : "+h" (firstX));
    register int lastY asm("r8") = (pixelY + 3) >> 3;
    asm ("" : "+h" (lastY));
    int lastX = (pixelX + 5) >> 3;
    if (firstY > lastY || firstX > lastX)
        return false;
    register unsigned width asm("sb") = collisionGrid->width;
    asm ("" : "+h" (width));
    if (firstY < 0)
        return false;
    register int leftmostTile asm("r1") = firstX;
    asm ("" : "+l" (leftmostTile));
    if (leftmostTile < 0 || static_cast<unsigned>(lastY) >= collisionGrid->height || static_cast<unsigned>(lastX) >= width)
        return false;
    u32 const * flags = collisionGrid->tileFlags;
    if (!flags)
        return false;
    register u8 const * indexBase asm("r4") = collisionGrid->tileIndices;
    asm ("" : "+l" (indexBase));
    register u8 const * indices asm("ip") = indexBase;
    asm ("" : "+h" (indices));
    int y = firstY;
    if (y <= lastY)
    {
        unsigned row = width * y;
        do
        {
            register int x asm("r3") = firstX;
            asm ("" : "+l" (x));
            if (x <= lastX)
            {
                register unsigned cell asm("r0") = x + row;
                asm ("" : "+l" (cell));
                register unsigned tileOffset asm("r2") = cell * sizeof(u32);
                asm ("" : "+l" (tileOffset));
                register unsigned index asm("r1") = reinterpret_cast<unsigned>(indices);
                asm ("" : "+l" (index));
                register unsigned indexRow asm("r0");
                asm ("add %0, %1, %2" : "=l" (indexRow) : "l" (row), "l" (index) : "cc");
                asm ("add %0, %1, %2" : "=l" (index) : "l" (x), "l" (indexRow) : "cc");
                do
                {
                    u32 const * tile = indices ? flags + *reinterpret_cast<u8 const *>(index)
                        : reinterpret_cast<u32 const *>(reinterpret_cast<u8 const *>(flags) + tileOffset);
                    asm ("" : "+l" (tile));
                    if (static_cast<i32>(*tile << 30) >= 0)
                        return false;
                    tileOffset += sizeof(u32);
                    ++index;
                    asm ("" : "+l" (tileOffset), "+l" (index));
                } while (++x <= lastX);
            }
            row += width;
        } while (++y <= lastY);
    }
    return true;
}

EC SECTION(".text.080AB85C") void ProbeEntityCollisionDown(EntityCollisionProbe & probe)
{
    probe.down = probe.step;
    register EntityCollisionGrid const * grid asm("r8") = &probe.grid;
    asm ("" : "+h" (grid));
    register ProbeRectangle const * bounds asm("r6") = &probe.bounds;
    asm ("" : "+l" (bounds));
    func_080AC124(*grid, *bounds, &probe.down, nullptr, nullptr, nullptr);

    u32 clear;
    if (probe.verticalLookahead)
    {
        register int offset asm("r3") = probe.down + probe.verticalLookahead;
        asm ("" : "+l" (offset));
        register int left asm("r4") = probe.bounds.left;
        asm ("" : "+l" (left));
        register int top asm("r1") = bounds->top;
        asm ("" : "+l" (top));
        register int right asm("r5") = bounds->right;
        asm ("" : "+l" (right));
        register int bottom asm("r6") = bounds->bottom;
        asm ("" : "+l" (bottom));
        ProbeRectangle shifted;
        register ProbeRectangle * destination asm("r2") = &shifted;
        asm ("" : "+l" (destination));
        register ProbeRectangle * firstEdge asm("r0") = destination;
        asm ("" : "+l" (firstEdge));
        firstEdge->left = left;
        destination->top = offset + top;
        destination->right = right;
        destination->bottom = offset + bottom;
        clear = DoesEntityCollisionRectangleHitSolidTile(*grid, *destination) ^ 1;
    }
    else
        clear = 0;
    probe.downClear = clear;
}

EC SECTION(".text.080AB8D0") void ProbeEntityCollisionUp(EntityCollisionProbe & probe)
{
    probe.up = -probe.step;
    register EntityCollisionGrid const * grid asm("r8") = &probe.grid;
    asm ("" : "+h" (grid));
    register ProbeRectangle const * bounds asm("r6") = &probe.bounds;
    asm ("" : "+l" (bounds));
    func_080AC124(*grid, *bounds, nullptr, &probe.up, nullptr, nullptr);

    u32 clear;
    if (probe.verticalLookahead)
    {
        register int offset asm("r3") = probe.up - probe.verticalLookahead;
        asm ("" : "+l" (offset));
        register int left asm("r4") = probe.bounds.left;
        asm ("" : "+l" (left));
        register int top asm("r1") = bounds->top;
        asm ("" : "+l" (top));
        register int right asm("r5") = bounds->right;
        asm ("" : "+l" (right));
        register int bottom asm("r6") = bounds->bottom;
        asm ("" : "+l" (bottom));
        ProbeRectangle shifted;
        register ProbeRectangle * destination asm("r2") = &shifted;
        asm ("" : "+l" (destination));
        register ProbeRectangle * firstEdge asm("r0") = destination;
        asm ("" : "+l" (firstEdge));
        firstEdge->left = left;
        destination->top = offset + top;
        destination->right = right;
        destination->bottom = offset + bottom;
        clear = DoesEntityCollisionRectangleHitSolidTile(*grid, *destination) ^ 1;
    }
    else
        clear = 0;
    probe.upClear = clear;
}

EC SECTION(".text.080AB948") void ProbeEntityCollisionLeft(EntityCollisionProbe & probe)
{
    probe.left = -probe.step;
    register EntityCollisionGrid const * grid asm("r8") = &probe.grid;
    asm ("" : "+h" (grid));
    register ProbeRectangle const * bounds asm("r6") = &probe.bounds;
    asm ("" : "+l" (bounds));
    func_080AC124(*grid, *bounds, nullptr, nullptr, &probe.left, nullptr);

    u32 clear;
    if (probe.horizontalLookahead)
    {
        register int offset asm("r1") = probe.left - probe.horizontalLookahead;
        asm ("" : "+l" (offset));
        register int left asm("r2") = probe.bounds.left;
        asm ("" : "+l" (left));
        register int top asm("r4") = bounds->top;
        asm ("" : "+l" (top));
        register int right asm("r5") = bounds->right;
        asm ("" : "+l" (right));
        register int bottom asm("r6") = bounds->bottom;
        asm ("" : "+l" (bottom));
        ProbeRectangle shifted;
        register ProbeRectangle * destination asm("r3") = &shifted;
        asm ("" : "+l" (destination));
        left = offset + left;
        asm ("" : "+l" (left));
        register ProbeRectangle * firstEdge asm("r0") = destination;
        asm ("" : "+l" (firstEdge));
        firstEdge->left = left;
        destination->top = top;
        destination->right = offset + right;
        destination->bottom = bottom;
        clear = DoesEntityCollisionRectangleHitSolidTile(*grid, *destination) ^ 1;
    }
    else
        clear = 0;
    probe.leftClear = clear;
}

EC SECTION(".text.080AB9C4") void ProbeEntityCollisionRight(EntityCollisionProbe & probe)
{
    probe.right = probe.step;
    register EntityCollisionGrid const * grid asm("r8") = &probe.grid;
    asm ("" : "+h" (grid));
    register ProbeRectangle const * bounds asm("r6") = &probe.bounds;
    asm ("" : "+l" (bounds));
    func_080AC124(*grid, *bounds, nullptr, nullptr, nullptr, &probe.right);

    u32 clear;
    if (probe.horizontalLookahead)
    {
        register int offset asm("r1") = probe.right + probe.horizontalLookahead;
        asm ("" : "+l" (offset));
        register int left asm("r2") = probe.bounds.left;
        asm ("" : "+l" (left));
        register int top asm("r4") = bounds->top;
        asm ("" : "+l" (top));
        register int right asm("r5") = bounds->right;
        asm ("" : "+l" (right));
        register int bottom asm("r6") = bounds->bottom;
        asm ("" : "+l" (bottom));
        ProbeRectangle shifted;
        register ProbeRectangle * destination asm("r3") = &shifted;
        asm ("" : "+l" (destination));
        left = offset + left;
        asm ("" : "+l" (left));
        register ProbeRectangle * firstEdge asm("r0") = destination;
        asm ("" : "+l" (firstEdge));
        firstEdge->left = left;
        destination->top = top;
        destination->right = offset + right;
        destination->bottom = bottom;
        clear = DoesEntityCollisionRectangleHitSolidTile(*grid, *destination) ^ 1;
    }
    else
        clear = 0;
    probe.rightClear = clear;
}

EC SECTION(".text.080ABA90") void ConstrainEntityMovementAgainstRectangle(EntityCollisionProbe & probe, ProbeRectangle const & obstacle, unsigned markBlocked)
{
    if (probe.bounds.left < obstacle.right && obstacle.left < probe.bounds.right)
    {
        int gap = probe.bounds.bottom - obstacle.top;
        if (gap <= 0)
        {
            register int current asm("r1") = probe.down;
            register int step asm("r0") = probe.step;
            asm ("" : "+l" (current), "+l" (step));
            register int travel asm("r0");
            if (current > step)
            {
                ProbeEntityCollisionDown(probe);
                travel = probe.down;
                asm ("" : "+l" (travel));
            }
            else
            {
                travel = current;
                asm ("" : "+l" (travel));
            }
            if (gap + travel > 0)
            {
                probe.down = -gap;
                if (markBlocked == 1)
                    probe.downClear = 0;
            }
        }
        gap = probe.bounds.top - obstacle.bottom;
        if (gap >= 0)
        {
            register int current asm("r1") = probe.up;
            register int step asm("r0") = probe.step;
            asm ("" : "+l" (current), "+l" (step));
            register int travel asm("r0");
            if (current < -step)
            {
                ProbeEntityCollisionUp(probe);
                travel = probe.up;
                asm ("" : "+l" (travel));
            }
            else
            {
                travel = current;
                asm ("" : "+l" (travel));
            }
            if (gap + travel < 0)
            {
                probe.up = -gap;
                if (markBlocked == 1)
                    probe.upClear = 0;
            }
        }
    }
    if (probe.bounds.top < obstacle.bottom && obstacle.top < probe.bounds.bottom)
    {
        int gap = probe.bounds.left - obstacle.right;
        if (gap >= 0)
        {
            register int current asm("r1") = probe.left;
            register int step asm("r0") = probe.step;
            asm ("" : "+l" (current), "+l" (step));
            register int travel asm("r0");
            if (current < -step)
            {
                ProbeEntityCollisionLeft(probe);
                travel = probe.left;
                asm ("" : "+l" (travel));
            }
            else
            {
                travel = current;
                asm ("" : "+l" (travel));
            }
            if (gap + travel < 0)
            {
                probe.left = -gap;
                if (markBlocked == 1)
                    probe.leftClear = 0;
            }
        }
        gap = probe.bounds.right - obstacle.left;
        if (gap <= 0)
        {
            register int current asm("r1") = probe.right;
            register int step asm("r0") = probe.step;
            asm ("" : "+l" (current), "+l" (step));
            register int travel asm("r0");
            if (current > step)
            {
                ProbeEntityCollisionRight(probe);
                travel = probe.right;
                asm ("" : "+l" (travel));
            }
            else
            {
                travel = current;
                asm ("" : "+l" (travel));
            }
            if (gap + travel > 0)
            {
                probe.right = -gap;
                if (markBlocked == 1)
                    probe.rightClear = 0;
            }
        }
    }
}

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

EC SECTION(".text.080ABC40") int ChooseEntityFacingTowardPoint(
    int x, int y, int targetX, int targetY, EntityCollisionProbe * probe, int currentFacing)
{
    int bestFacings[4];
    register int deltaX asm("r2") = targetX - x;
    asm ("" : "+l" (deltaX));
    volatile int dx = deltaX;
    register int deltaY asm("r3") = targetY - y;
    asm ("" : "+l" (deltaY));
    register int dy asm("sl") = deltaY;
    asm ("" : "+h" (dy));
    register int absX asm("r1") = deltaX;
    asm ("" : "+l" (absX));
    if (deltaX < 0)
        absX = -deltaX;
    asm ("" : "+l" (absX));
    register int absY asm("r0") = dy;
    asm ("" : "+l" (absY));
    if (absY < 0)
        absY = -absY;
    asm ("" : "+l" (absY));
    register int neutralAxis asm("r2") = 0;
    asm ("" : "+l" (neutralAxis));
    volatile int axisPreference = neutralAxis;
    register int doubleX asm("r2") = 2 * absX;
    register int doubleY asm("r3") = 2 * absY;
    asm ("" : "+l" (doubleX), "+l" (doubleY));
    if (doubleX > doubleY + absY)
    {
        register int horizontalAxis asm("r5") = 1;
        asm ("" : "+l" (horizontalAxis));
        axisPreference = horizontalAxis;
    }
    else
    {
        register int tripleX asm("r0") = doubleX + absX;
        asm ("" : "+l" (tripleX));
        if (tripleX < doubleY)
            axisPreference = -1;
    }
    register int initialBestScore asm("r1") = 0x7fffffff;
    asm ("" : "+l" (initialBestScore));
    volatile int bestScore = initialBestScore;
    register unsigned initialCount asm("r2") = 0;
    asm ("" : "+l" (initialCount));
    volatile unsigned bestCount = initialCount;
    register int firstTurn asm("r5") = 0;
    asm ("" : "+l" (firstTurn));
    volatile int turn = firstTurn;
    do
    {
        int offset = turn;
        register int facing asm("r5") = RotateEntityMovementFacing(currentFacing, offset);
        asm ("" : "+l" (facing));
        register unsigned away asm("r8") = 1;
        asm ("" : "+h" (away));
        register unsigned weighted asm("sb") = 0;
        asm ("" : "+h" (weighted));
        register unsigned blocked asm("r1") = 1;
        asm ("" : "+l" (blocked));
        register unsigned directionBlocked asm("r6");
        switch (facing)
        {
        case 0:
        {
            register unsigned awayFlag asm("r0") = 0;
            asm ("" : "+l" (awayFlag));
            register int verticalDelta asm("r2") = dy;
            asm ("" : "+l" (verticalDelta));
            if (verticalDelta <= 0)
                awayFlag = 1;
            asm ("" : "+l" (awayFlag));
            away = awayFlag;
            weighted = axisPreference < 0;
            directionBlocked = 0;
            asm ("" : "+l" (directionBlocked));
            register int distance asm("r1") = probe->down;
            asm ("" : "+l" (distance));
            register int limit asm("r0") = probe->step;
            asm ("" : "+l" (limit));
            register int remaining asm("r0");
            if (distance > limit)
            {
                ProbeEntityCollisionDown(*probe);
                remaining = probe->down;
                asm ("" : "+l" (remaining));
            }
            else
            {
                remaining = distance;
                asm ("" : "+l" (remaining) : "l" (distance));
            }
            asm ("" : "+l" (remaining));
            if (remaining == 0)
            {
                if (static_cast<volatile EntityCollisionProbe *>(probe)->down > probe->step)
                    ProbeEntityCollisionDown(*probe);
                if (!probe->downClear)
                    directionBlocked = 1;
            }
            goto setBlocked;
        }
        case 1:
        {
            away = dy >= 0;
            register unsigned axisWeight asm("r2") = static_cast<unsigned>(axisPreference) >> 31;
            asm ("" : "+l" (axisWeight));
            weighted = axisWeight;
            directionBlocked = 0;
            register int distance asm("r1") = probe->up;
            asm ("" : "+l" (distance));
            register int limit asm("r0") = probe->step;
            asm ("" : "+l" (limit));
            register int remaining asm("r0");
            if (distance < -limit)
            {
                ProbeEntityCollisionUp(*probe);
                remaining = probe->up;
                asm ("" : "+l" (remaining));
            }
            else
            {
                remaining = distance;
            }
            if (remaining == 0)
            {
                if (static_cast<volatile EntityCollisionProbe *>(probe)->up < -probe->step)
                    ProbeEntityCollisionUp(*probe);
                if (!probe->upClear)
                    directionBlocked = 1;
            }
            goto setBlocked;
        }
        case 2:
        {
            register int horizontalDelta asm("r1") = dx;
            asm ("" : "+l" (horizontalDelta));
            register unsigned awayFlag asm("r0") = static_cast<unsigned>(~horizontalDelta) >> 31;
            asm ("" : "+l" (awayFlag));
            away = awayFlag;
            register unsigned axisWeight asm("r0") = 0;
            register int axis asm("r2") = axisPreference;
            if (axis > 0)
                axisWeight = 1;
            weighted = axisWeight;
            directionBlocked = 0;
            asm ("" : "+l" (directionBlocked));
            register int distance asm("r1") = probe->left;
            register int limit asm("r0") = probe->step;
            asm ("" : "+l" (limit));
            register int remaining asm("r0");
            if (distance < -limit)
            {
                ProbeEntityCollisionLeft(*probe);
                remaining = probe->left;
                asm ("" : "+l" (remaining));
            }
            else
            {
                remaining = distance;
                asm ("" : "+l" (remaining) : "l" (distance));
            }
            if (remaining == 0)
            {
                if (static_cast<volatile EntityCollisionProbe *>(probe)->left < -probe->step)
                    ProbeEntityCollisionLeft(*probe);
                if (!probe->leftClear)
                    directionBlocked = 1;
            }
            goto setBlocked;
        }
        case 3:
        {
            register unsigned awayFlag asm("r0") = 0;
            register int horizontalDelta asm("r1") = dx;
            if (horizontalDelta <= 0)
                awayFlag = 1;
            away = awayFlag;
            register unsigned axisWeight asm("r0") = 0;
            asm ("" : "+l" (axisWeight));
            register int axis asm("r2") = axisPreference;
            asm ("" : "+l" (axis));
            if (axis > 0)
                axisWeight = 1;
            weighted = axisWeight;
            directionBlocked = 0;
            register int distance asm("r1") = probe->right;
            register int limit asm("r0") = probe->step;
            register int remaining asm("r0");
            if (distance > limit)
            {
                ProbeEntityCollisionRight(*probe);
                remaining = probe->right;
                asm ("" : "+l" (remaining));
            }
            else
            {
                remaining = distance;
            }
            if (remaining == 0)
            {
                if (static_cast<volatile EntityCollisionProbe *>(probe)->right > probe->step)
                    ProbeEntityCollisionRight(*probe);
                if (!probe->rightClear)
                    directionBlocked = 1;
            }
            goto setBlocked;
        }
        default: goto computeScore;
        }
    setBlocked:
        asm ("" : "+l" (directionBlocked));
        blocked = directionBlocked;
    computeScore:
        asm ("" : "+l" (blocked), "+l" (facing));
        register int penalty asm("r3") = -1;
        asm ("" : "+l" (penalty));
        if (away)
            penalty = 1;
        asm ("" : "+l" (penalty));
        register int blockedCost asm("r1");
        register unsigned blockedBits asm("r0");
        asm ("neg %0, %2\n\torr %0, %2\n\tasr %1, %0, #31\n\tmovs %0, #8\n\tand %1, %0"
             : "=&l" (blockedBits), "=l" (blockedCost) : "1" (blocked) : "cc");
        register int turnCost asm("r2");
        asm ("cmp %1, #2\n\tbgt 1f\n\tcmp %1, #1\n\tbge 2f\n\tcmp %1, #0\n\tbne 1f\n\tmovs %0, #0\n\tb 3f\n"
             "2:\n\tmovs %0, #1\n\tb 3f\n1:\n\tmovs %0, #2\n3:" : "=l" (turnCost) : "l" (offset) : "cc");
        register int score asm("r0") = weighted ? blockedCost + penalty * 2 : blockedCost + penalty;
        asm ("" : "+l" (score));
        score += turnCost;
        asm ("" : "+l" (score));
        if (score == bestScore)
        {
            register unsigned count asm("r2") = bestCount;
            asm ("" : "+l" (count));
            bestFacings[count] = facing;
            bestCount = count + 1;
        }
        else if (score < bestScore)
        {
            bestFacings[0] = facing;
            register unsigned newCount asm("r2") = 1;
            asm ("" : "+l" (newCount));
            bestCount = newCount;
            bestScore = score;
        }
        register int nextTurn asm("r5") = turn + 1;
        asm ("" : "+l" (nextTurn));
        turn = nextTurn;
        if (nextTurn > 3)
            break;
    } while (true);
    register int * choices asm("r4") = bestFacings;
    asm ("" : "+l" (choices));
    unsigned count = bestCount;
    register unsigned byteOffset asm("r0");
    if (count > 1)
    {
        byteOffset = RollEntityMovementRandom(count) * sizeof(int);
        asm ("" : "+l" (byteOffset));
    }
    else
    {
        byteOffset = 0;
        asm ("" : "+l" (byteOffset));
    }
    register const int * selected asm("r0");
    asm ("add %0, %1, %2" : "=l" (selected) : "l" (choices), "l" (byteOffset) : "cc");
    return *selected;
}
