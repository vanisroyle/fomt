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

EC SECTION(".text.080AC124") void ClampEntityMovementToSolidTiles(EntityCollisionGrid const & grid,
    ProbeRectangle const & rectangle, int * down, int * up, int * volatile left, int * volatile right)
{
    struct ScanFrame
    {
        ProbeRectangle const * rectangle;
        int * down;
        int * up;
        int endX;
        int firstY;
        int endY;
        int downLimit;
        int lastRow;
        int upLimit;
        int rightLimit;
        int rightFirstRowOffset;
        int leftFirstRowOffset;
        int previousRow;
    };
    volatile ScanFrame frame;
    register EntityCollisionGrid const * collisionGrid asm("r4") = &grid;
    asm ("" : "+l" (collisionGrid));
    register ProbeRectangle const * bounds asm("r1") = &rectangle;
    register int * downRequest asm("r2") = down;
    register int * upRequest asm("r3") = up;
    asm ("" : "+l" (bounds), "+l" (downRequest), "+l" (upRequest));
    frame.rectangle = bounds;
    frame.down = downRequest;
    frame.up = upRequest;
    register int firstX = bounds->left >> 3;
    asm ("" : "+l" (firstX));
    register int rightOffset asm("r2") = offsetof(ProbeRectangle, right);
    asm ("" : "+l" (rightOffset));
    register int endX asm("r0");
    asm ("ldrsh %0, [%1, %2]" : "=l" (endX) : "l" (bounds), "l" (rightOffset));
    endX = (endX + 7) >> 3;
    asm ("" : "+l" (endX));
    frame.endX = endX;
    frame.firstY = bounds->top >> 3;
    register int bottomOffset asm("r3") = offsetof(ProbeRectangle, bottom);
    asm ("" : "+l" (bottomOffset));
    register int bottom asm("r1");
    asm ("ldrsh %0, [%1, %2]" : "=l" (bottom) : "l" (bounds), "l" (bottomOffset));
    asm ("" : "+l" (bottom));
    register int endY asm("r0") = (bottom + 7) >> 3;
    asm ("" : "+l" (endY));
    frame.endY = endY;
    register u32 const * flagsLow asm("r0") = collisionGrid->tileFlags;
    asm ("" : "+l" (flagsLow));
    register u32 const * flags asm("r8") = flagsLow;
    asm ("" : "+h" (flags));
    register int widthLow asm("r2") = collisionGrid->width;
    asm ("" : "+l" (widthLow));
    register int width asm("sb") = widthLow;
    asm ("" : "+h" (width));
    register u8 const * indicesLow asm("r3") = collisionGrid->tileIndices;
    asm ("" : "+l" (indicesLow));
    register u8 const * indices asm("sl") = indicesLow;
    asm ("" : "+h" (indices));

    {
        register int * requested asm("r0") = frame.down;
        asm ("" : "+l" (requested));
        if (!requested)
            goto doneDown;
        register int requestedLimit asm("r2") = *requested;
        asm ("" : "+l" (requestedLimit));
        frame.downLimit = requestedLimit;
        register int edgeTile asm("r3") = frame.endY;
        asm ("" : "+l" (edgeTile));
        register int edgePixels asm("r0") = edgeTile * 8;
        asm ("" : "+l" (edgePixels));
        register int distance asm("r6") = edgePixels - bottom;
        asm ("" : "+l" (distance));
        if (distance >= requestedLimit)
            goto doneDown;
        register int lastRow asm("r0") = collisionGrid->height - 1;
        asm ("" : "+l" (lastRow));
        frame.lastRow = lastRow;
        register int row asm("ip") = edgeTile;
        asm ("" : "+h" (row));
        register int rowOffset asm("r5");
        if (edgeTile > lastRow)
            goto blockedDown;
        rowOffset = width;
        asm ("" : "+l" (rowOffset));
        rowOffset *= edgeTile;
        asm ("" : "+l" (rowOffset));
        do
        {
            register int column asm("r4") = firstX;
            asm ("" : "+l" (column));
            register int innerLimit asm("r0") = frame.endX;
            asm ("" : "+l" (innerLimit));
            if (firstX < innerLimit)
            {
                register unsigned cell asm("r0") = firstX + rowOffset;
                asm ("" : "+l" (cell));
                register unsigned tileOffset asm("r3") = cell * sizeof(u32);
                asm ("" : "+l" (tileOffset));
                register u8 const * indexBase asm("r1") = indices;
                asm ("" : "+l" (indexBase));
                register u8 const * indexRow asm("r0");
                asm ("add %0, %1, %2" : "=l" (indexRow) : "l" (rowOffset), "l" (indexBase) : "cc");
                register u8 const * index asm("r2");
                asm ("add %0, %1, %2" : "=l" (index) : "l" (firstX), "l" (indexRow) : "cc");
                do
                {
                    register u8 const * indexTable asm("r0") = indices;
                    asm ("" : "+l" (indexTable));
                    register u32 const * tile asm("r0");
                    if (indexTable)
                    {
                        tile = flags + *index;
                        asm ("" : "+l" (tile));
                    }
                    else
                    {
                        register u32 const * directFlags asm("r1") = flags;
                        asm ("" : "+l" (directFlags));
                        tile = reinterpret_cast<u32 const *>(reinterpret_cast<u8 const *>(directFlags) + tileOffset);
                        asm ("" : "+l" (tile));
                    }
                    register u32 bits asm("r1") = *tile;
                    asm ("" : "+l" (bits));
                    if (bits << 31)
                        goto blockedDown;
                    register int secondaryFlag asm("r0") = static_cast<int>(bits << 30);
                    asm ("" : "+l" (secondaryFlag));
                    if (secondaryFlag < 0)
                        goto blockedDown;
                    tileOffset += sizeof(u32);
                    index += 1;
                    ++column;
                    asm ("" : "+l" (tileOffset), "+l" (index), "+l" (column));
                } while (column < frame.endX);
            }
            distance += 8;
            asm ("" : "+l" (distance));
            register int maxDistance asm("r1") = frame.downLimit;
            asm ("" : "+l" (maxDistance));
            if (distance >= maxDistance)
                goto doneDown;
            rowOffset += width;
            register int rowStep asm("r2") = 1;
            asm ("" : "+l" (rowStep));
            row += rowStep;
            asm ("" : "+h" (row));
            register int rowLimit asm("r3");
            rowLimit = frame.lastRow;
            asm ("" : "+l" (rowLimit));
            if (row > rowLimit)
                break;
        } while (true);
    blockedDown:
        register int * result asm("r0") = frame.down;
        asm ("" : "+l" (result));
        *result = distance;
    }
doneDown:
    ;
    {
        register int * requested asm("r1") = frame.up;
        asm ("" : "+l" (requested));
        if (!requested)
            goto doneUp;
        register int requestedLimit asm("r2") = *requested;
        asm ("" : "+l" (requestedLimit));
        frame.upLimit = requestedLimit;
        register int edgeTile asm("r3") = frame.firstY;
        asm ("" : "+l" (edgeTile));
        register int edgePixels asm("r1") = edgeTile * 8;
        asm ("" : "+l" (edgePixels));
        register ProbeRectangle const * scanBounds asm("r2") = frame.rectangle;
        asm ("" : "+l" (scanBounds));
        register int edgeOffset asm("r3") = offsetof(ProbeRectangle, top);
        asm ("" : "+l" (edgeOffset));
        register int pixelEdge asm("r0");
        asm ("ldrsh %0, [%1, %2]" : "=l" (pixelEdge) : "l" (scanBounds), "l" (edgeOffset));
        asm ("" : "+l" (pixelEdge));
        register int delta asm("r1") = edgePixels - pixelEdge;
        asm ("" : "+l" (delta));
        register int distance asm("ip") = delta;
        asm ("" : "+h" (distance));
        register int initialLimit asm("r0") = frame.upLimit;
        asm ("" : "+l" (initialLimit));
        if (distance <= initialLimit)
            goto doneUp;
        register int row asm("r6") = frame.firstY - 1;
        asm ("" : "+l" (row));
        register int rowOffset asm("r5");
        if (row < 0)
            goto blockedUp;
        {
            rowOffset = row * width;
            asm ("" : "+l" (rowOffset));
            register int previousRow asm("r1") = width;
            asm ("" : "+l" (previousRow));
            previousRow = -previousRow;
            asm ("" : "+l" (previousRow));
            frame.previousRow = previousRow;
            do
            {
                register int column asm("r4") = firstX;
                asm ("" : "+l" (column));
                register int innerLimit asm("r2") = frame.endX;
                asm ("" : "+l" (innerLimit));
                if (firstX < innerLimit)
                {
                    register unsigned cell asm("r0") = firstX + rowOffset;
                    asm ("" : "+l" (cell));
                    register unsigned tileOffset asm("r3") = cell * sizeof(u32);
                    asm ("" : "+l" (tileOffset));
                    register u8 const * indexBase asm("r1") = indices;
                    asm ("" : "+l" (indexBase));
                    register u8 const * indexRow asm("r0");
                    asm ("add %0, %1, %2" : "=l" (indexRow) : "l" (rowOffset), "l" (indexBase) : "cc");
                    register u8 const * index asm("r1");
                    asm ("add %0, %1, %2" : "=l" (index) : "l" (firstX), "l" (indexRow) : "cc");
                    do
                    {
                        register u8 const * indexTable asm("r2") = indices;
                        asm ("" : "+l" (indexTable));
                        register u32 const * tile asm("r0");
                        if (indexTable)
                        {
                            tile = flags + *index;
                            asm ("" : "+l" (tile));
                        }
                        else
                        {
                            register u32 const * directFlags asm("r2") = flags;
                            asm ("" : "+l" (directFlags));
                            tile = reinterpret_cast<u32 const *>(reinterpret_cast<u8 const *>(directFlags) + tileOffset);
                            asm ("" : "+l" (tile));
                        }
                        register u32 bits asm("r2") = *tile;
                        asm ("" : "+l" (bits));
                        if (bits << 31)
                            goto blockedUp;
                        register int secondaryFlag asm("r0") = static_cast<int>(bits << 30);
                        asm ("" : "+l" (secondaryFlag));
                        if (secondaryFlag < 0)
                            goto blockedUp;
                        tileOffset += sizeof(u32);
                        index += 1;
                        ++column;
                        asm ("" : "+l" (tileOffset), "+l" (index), "+l" (column));
                    } while (column < frame.endX);
                }
                register int tileStep asm("r1") = -8;
                asm ("" : "+l" (tileStep));
                distance += tileStep;
                asm ("" : "+h" (distance));
                register int maxDistance asm("r2") = frame.upLimit;
                asm ("" : "+l" (maxDistance));
                if (distance <= maxDistance)
                    goto doneUp;
                register int rowStep asm("r3") = frame.previousRow;
                asm ("" : "+l" (rowStep));
                rowOffset += rowStep;
                --row;
                asm ("" : "+l" (row));
            } while (row >= 0);
        }
    blockedUp:
        register int blockedDistance asm("r0") = distance;
        asm ("" : "+l" (blockedDistance));
        register int * result asm("r1") = frame.up;
        asm ("" : "+l" (result));
        *result = blockedDistance;
    }
doneUp:
    ;
    {
        register int * requested asm("r2") = left;
        asm ("" : "+l" (requested));
        if (!requested)
            goto doneLeft;
        register int requestedLimit asm("r3") = *requested;
        asm ("" : "+l" (requestedLimit));
        register int limit asm("ip") = requestedLimit;
        asm ("" : "+h" (limit));
        register int edgePixels asm("r1");
        asm ("lsl %0, %1, #3" : "=l" (edgePixels) : "l" (firstX) : "cc");
        register ProbeRectangle const * scanBounds asm("r2") = frame.rectangle;
        asm ("" : "+l" (scanBounds));
        register int edgeOffset asm("r3") = offsetof(ProbeRectangle, left);
        asm ("" : "+l" (edgeOffset));
        register int pixelEdge asm("r0");
        asm ("ldrsh %0, [%1, %2]" : "=l" (pixelEdge) : "l" (scanBounds), "l" (edgeOffset));
        asm ("" : "+l" (pixelEdge));
        register int distance asm("r6") = edgePixels - pixelEdge;
        asm ("" : "+l" (distance));
        if (distance <= limit)
            goto doneLeft;
        register int column asm("r5") = firstX - 1;
        asm ("" : "+l" (column));
        if (column < 0)
            goto blockedLeft;
        {
            register int firstRow asm("r1") = frame.firstY;
            asm ("" : "+l" (firstRow));
            register int scaledRow asm("r0") = firstRow * sizeof(u32);
            asm ("" : "+l" (scaledRow));
            register int rowWidth asm("r2") = width;
            asm ("" : "+l" (rowWidth));
            rowWidth *= scaledRow;
            asm ("" : "+l" (rowWidth));
            frame.leftFirstRowOffset = rowWidth;
            do
            {
                register int row asm("r4") = frame.firstY;
                asm ("" : "+l" (row));
                register int innerLimit asm("r3") = frame.endY;
                asm ("" : "+l" (innerLimit));
                if (row < innerLimit)
                {
                    register unsigned columnOffset asm("r0") = column * sizeof(u32);
                    asm ("" : "+l" (columnOffset));
                    register unsigned rowOffset asm("r1") = frame.leftFirstRowOffset;
                    asm ("" : "+l" (rowOffset));
                    register unsigned tileOffset asm("r3") = rowOffset + columnOffset;
                    asm ("" : "+l" (tileOffset));
                    register int indexRow asm("r1") = width;
                    asm ("" : "+l" (indexRow));
                    indexRow *= row;
                    asm ("" : "+l" (indexRow));
                    register u8 const * indexBase asm("r2") = indices;
                    asm ("" : "+l" (indexBase));
                    register u8 const * indexColumn asm("r0");
                    asm ("add %0, %1, %2" : "=l" (indexColumn) : "l" (column), "l" (indexBase) : "cc");
                    register u8 const * index asm("r1");
                    asm ("add %0, %1, %2" : "=l" (index) : "l" (indexRow), "l" (indexColumn) : "cc");
                    register unsigned strideWidth asm("r0") = width;
                    asm ("" : "+l" (strideWidth));
                    register unsigned flagStride = strideWidth * sizeof(u32);
                    asm ("" : "+l" (flagStride));
                    do
                    {
                        register u8 const * indexTable asm("r2") = indices;
                        asm ("" : "+l" (indexTable));
                        register u32 const * tile asm("r0");
                        if (indexTable)
                        {
                            tile = flags + *index;
                            asm ("" : "+l" (tile));
                        }
                        else
                        {
                            register u32 const * directFlags asm("r2") = flags;
                            asm ("" : "+l" (directFlags));
                            tile = reinterpret_cast<u32 const *>(reinterpret_cast<u8 const *>(directFlags) + tileOffset);
                            asm ("" : "+l" (tile));
                        }
                        register u32 bits asm("r2") = *tile;
                        asm ("" : "+l" (bits));
                        if (bits << 31)
                            goto blockedLeft;
                        register int secondaryFlag asm("r0") = static_cast<int>(bits << 30);
                        asm ("" : "+l" (secondaryFlag));
                        if (secondaryFlag < 0)
                            goto blockedLeft;
                        tileOffset += flagStride;
                        index += width;
                        ++row;
                        asm ("" : "+l" (tileOffset), "+l" (index), "+l" (row));
                    } while (row < frame.endY);
                }
                distance -= 8;
                asm ("" : "+l" (distance));
                if (distance <= limit)
                    goto doneLeft;
                --column;
                asm ("" : "+l" (column));
            } while (column >= 0);
        }
    blockedLeft:
        register int * result asm("r1") = left;
        asm ("" : "+l" (result));
        *result = distance;
    }
doneLeft:
    ;
    {
        register int * requested asm("r2") = right;
        asm ("" : "+l" (requested));
        if (!requested)
            goto doneRight;
        register int requestedLimit asm("r3") = *requested;
        asm ("" : "+l" (requestedLimit));
        frame.rightLimit = requestedLimit;
        register int edgeTile asm("r0") = frame.endX;
        asm ("" : "+l" (edgeTile));
        register int edgePixels asm("r1") = edgeTile * 8;
        asm ("" : "+l" (edgePixels));
        register ProbeRectangle const * scanBounds asm("r2") = frame.rectangle;
        asm ("" : "+l" (scanBounds));
        register int edgeOffset asm("r3") = offsetof(ProbeRectangle, right);
        asm ("" : "+l" (edgeOffset));
        register int pixelEdge asm("r0");
        asm ("ldrsh %0, [%1, %2]" : "=l" (pixelEdge) : "l" (scanBounds), "l" (edgeOffset));
        asm ("" : "+l" (pixelEdge));
        register int distance asm("r6") = edgePixels - pixelEdge;
        asm ("" : "+l" (distance));
        register int initialLimit asm("r0") = frame.rightLimit;
        asm ("" : "+l" (initialLimit));
        if (distance >= initialLimit)
            goto doneRight;
        register int lastColumnLow asm("r1") = -1;
        asm ("" : "+l" (lastColumnLow));
        lastColumnLow += width;
        asm ("" : "+l" (lastColumnLow));
        register int lastColumn asm("ip") = lastColumnLow;
        asm ("" : "+h" (lastColumn));
        register int column asm("r5") = frame.endX;
        asm ("" : "+l" (column));
        if (column > lastColumn)
            goto blockedRight;
        {
            register int firstRow asm("r2") = frame.firstY;
            asm ("" : "+l" (firstRow));
            register int scaledRow asm("r0") = firstRow * sizeof(u32);
            asm ("" : "+l" (scaledRow));
            register int rowWidth asm("r3") = width;
            asm ("" : "+l" (rowWidth));
            rowWidth *= scaledRow;
            asm ("" : "+l" (rowWidth));
            frame.rightFirstRowOffset = rowWidth;
            do
            {
                register int row asm("r3") = frame.firstY;
                asm ("" : "+l" (row));
                register int innerLimit asm("r0") = frame.endY;
                asm ("" : "+l" (innerLimit));
                if (row < innerLimit)
                {
                    register unsigned columnOffset asm("r0") = column * sizeof(u32);
                    asm ("" : "+l" (columnOffset));
                    register unsigned rowOffset asm("r1") = frame.rightFirstRowOffset;
                    asm ("" : "+l" (rowOffset));
                    register unsigned tileOffset asm("r4") = rowOffset + columnOffset;
                    asm ("" : "+l" (tileOffset));
                    register int indexRow asm("r1") = width;
                    asm ("" : "+l" (indexRow));
                    indexRow *= row;
                    asm ("" : "+l" (indexRow));
                    register u8 const * indexBase asm("r2") = indices;
                    asm ("" : "+l" (indexBase));
                    register u8 const * indexColumn asm("r0");
                    asm ("add %0, %1, %2" : "=l" (indexColumn) : "l" (column), "l" (indexBase) : "cc");
                    register u8 const * index asm("r1");
                    asm ("add %0, %1, %2" : "=l" (index) : "l" (indexRow), "l" (indexColumn) : "cc");
                    register unsigned strideWidth asm("r0") = width;
                    asm ("" : "+l" (strideWidth));
                    register unsigned flagStride = strideWidth * sizeof(u32);
                    asm ("" : "+l" (flagStride));
                    do
                    {
                        register u8 const * indexTable asm("r2") = indices;
                        asm ("" : "+l" (indexTable));
                        register u32 const * tile asm("r0");
                        if (indexTable)
                        {
                            tile = flags + *index;
                            asm ("" : "+l" (tile));
                        }
                        else
                        {
                            register u32 const * directFlags asm("r2") = flags;
                            asm ("" : "+l" (directFlags));
                            tile = reinterpret_cast<u32 const *>(reinterpret_cast<u8 const *>(directFlags) + tileOffset);
                            asm ("" : "+l" (tile));
                        }
                        register u32 bits asm("r2") = *tile;
                        asm ("" : "+l" (bits));
                        if (bits << 31)
                            goto blockedRight;
                        register int secondaryFlag asm("r0") = static_cast<int>(bits << 30);
                        asm ("" : "+l" (secondaryFlag));
                        if (secondaryFlag < 0)
                            goto blockedRight;
                        tileOffset += flagStride;
                        index += width;
                        ++row;
                        asm ("" : "+l" (tileOffset), "+l" (index), "+l" (row));
                    } while (row < frame.endY);
                }
                distance += 8;
                asm ("" : "+l" (distance));
                register int maxDistance asm("r1") = frame.rightLimit;
                asm ("" : "+l" (maxDistance));
                if (distance >= maxDistance)
                    goto doneRight;
                ++column;
                asm ("" : "+l" (column));
            } while (column <= lastColumn);
        }
    blockedRight:
        register int * result asm("r2") = right;
        asm ("" : "+l" (result));
        *result = distance;
    }
doneRight:
    ;
}

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
    ClampEntityMovementToSolidTiles(*grid, *bounds, &probe.down, nullptr, nullptr, nullptr);

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
    ClampEntityMovementToSolidTiles(*grid, *bounds, nullptr, &probe.up, nullptr, nullptr);

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
    ClampEntityMovementToSolidTiles(*grid, *bounds, nullptr, nullptr, &probe.left, nullptr);

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
    ClampEntityMovementToSolidTiles(*grid, *bounds, nullptr, nullptr, nullptr, &probe.right);

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

EC SECTION(".text.080ABE58") u32 ChooseEntityFacingAwayFromPoint(
    int x, int y, int targetX, int targetY, EntityCollisionProbe * probe, u32 currentFacing)
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
        register int penalty asm("r3") = 1;
        asm ("" : "+l" (penalty));
        if (away)
            penalty -= 2;
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

EC SECTION(".text.080AC388") void ClampEntityMovementToBlockedTiles(EntityTerrainGrid & grid,
    EntityMovementRect const & rectangle, int * down, int * up, int * volatile left, int * volatile right)
{
    struct ScanFrame
    {
        EntityMovementRect const * rectangle;
        int * down;
        int * up;
        int firstY;
        int endY;
        int downLimit;
        int lastRow;
        int upLimit;
        int rightLimit;
        int leftFirstRowOffset;
        int previousRow;
        int scanProgress;
    };
    volatile ScanFrame frame;
    register EntityTerrainGrid const * collisionGrid asm("r4") = &grid;
    asm ("" : "+l" (collisionGrid));
    register EntityMovementRect const * bounds asm("r1") = &rectangle;
    register int * downRequest asm("r2") = down;
    register int * upRequest asm("r3") = up;
    asm ("" : "+l" (bounds), "+l" (downRequest), "+l" (upRequest));
    frame.rectangle = bounds;
    frame.down = downRequest;
    frame.up = upRequest;
    register i16 leftEdge asm("r0") = bounds->left;
    asm ("" : "+l" (leftEdge));
    register int firstX asm("r6") = static_cast<int>(leftEdge) >> 3;
    asm ("" : "+l" (firstX));
    register int rightOffset asm("r2") = offsetof(EntityMovementRect, right);
    asm ("" : "+l" (rightOffset));
    register int endX asm("r0");
    asm ("ldrsh %0, [%1, %2]" : "=l" (endX) : "l" (bounds), "l" (rightOffset));
    endX = (endX + 7) >> 3;
    asm ("" : "+l" (endX));
    register int endColumn asm("sl") = endX;
    asm ("" : "+h" (endColumn));
    frame.firstY = bounds->top >> 3;
    register int bottomOffset asm("r3") = offsetof(EntityMovementRect, bottom);
    asm ("" : "+l" (bottomOffset));
    register int bottom asm("r1");
    asm ("ldrsh %0, [%1, %2]" : "=l" (bottom) : "l" (bounds), "l" (bottomOffset));
    asm ("" : "+l" (bottom));
    register int endY asm("r0") = (bottom + 7) >> 3;
    asm ("" : "+l" (endY));
    frame.endY = endY;
    register u32 const * flagsLow asm("r7") = collisionGrid->tileProperties;
    asm ("" : "+l" (flagsLow));
    register u32 const * flags asm("ip") = flagsLow;
    asm ("" : "+h" (flags));
    register int widthLow asm("r0") = collisionGrid->width;
    asm ("" : "+l" (widthLow));
    register int width asm("r8") = widthLow;
    asm ("" : "+h" (width));
    register u8 const * indicesLow asm("r2") = collisionGrid->tileIndices;
    asm ("" : "+l" (indicesLow));
    register u8 const * indices asm("sb") = indicesLow;
    asm ("" : "+h" (indices));

    {
        register int * requested asm("r3") = frame.down;
        asm ("" : "+l" (requested));
        if (!requested)
            goto doneDown;
        register int requestedLimit asm("r7") = *requested;
        asm ("" : "+l" (requestedLimit));
        frame.downLimit = requestedLimit;
        register int edgeTile asm("r2") = frame.endY;
        asm ("" : "+l" (edgeTile));
        register int edgePixels asm("r0") = edgeTile * 8;
        asm ("" : "+l" (edgePixels));
        register int distance asm("r5") = edgePixels - bottom;
        asm ("" : "+l" (distance));
        if (distance >= requestedLimit)
            goto doneDown;
        register int lastRow asm("r0") = collisionGrid->height - 1;
        asm ("" : "+l" (lastRow));
        frame.lastRow = lastRow;
        frame.scanProgress = edgeTile;
        register int rowOffset asm("r4");
        if (edgeTile > lastRow)
            goto blockedDown;
        rowOffset = width;
        asm ("" : "+l" (rowOffset));
        rowOffset *= edgeTile;
        asm ("" : "+l" (rowOffset));
        do
        {
            register int column asm("r3") = firstX;
            asm ("" : "+l" (column));
            if (firstX < endColumn)
            {
                register unsigned cell asm("r0") = firstX + rowOffset;
                asm ("" : "+l" (cell));
                register unsigned tileOffset asm("r2") = cell * sizeof(u32);
                asm ("" : "+l" (tileOffset));
                register u8 const * indexBase asm("r7") = indices;
                asm ("" : "+l" (indexBase));
                register u8 const * indexRow asm("r0");
                asm ("add %0, %1, %2" : "=l" (indexRow) : "l" (rowOffset), "l" (indexBase) : "cc");
                register u8 const * index asm("r1");
                asm ("add %0, %1, %2" : "=l" (index) : "l" (firstX), "l" (indexRow) : "cc");
                do
                {
                    register u8 const * indexTable asm("r0") = indices;
                    asm ("" : "+l" (indexTable));
                    register u32 const * tile asm("r0");
                    if (indexTable)
                    {
                        tile = flags + *index;
                        asm ("" : "+l" (tile));
                    }
                    else
                    {
                        register u32 const * directFlags asm("r7") = flags;
                        asm ("" : "+l" (directFlags));
                        tile = reinterpret_cast<u32 const *>(reinterpret_cast<u8 const *>(directFlags) + tileOffset);
                        asm ("" : "+l" (tile));
                    }
                    if (*tile << 31)
                        goto blockedDown;
                    tileOffset += sizeof(u32);
                    index += 1;
                    ++column;
                    asm ("" : "+l" (tileOffset), "+l" (index), "+l" (column));
                } while (column < endColumn);
            }
            distance += 8;
            asm ("" : "+l" (distance));
            register int maxDistance asm("r0") = frame.downLimit;
            asm ("" : "+l" (maxDistance));
            if (distance >= maxDistance)
                goto doneDown;
            rowOffset += width;
            register int nextRow asm("r1") = frame.scanProgress + 1;
            asm ("" : "+l" (nextRow));
            frame.scanProgress = nextRow;
            register int rowLimit asm("r2") = frame.lastRow;
            asm ("" : "+l" (rowLimit));
            if (nextRow > rowLimit)
                break;
        } while (true);
    blockedDown:
        register int * result asm("r3") = frame.down;
        asm ("" : "+l" (result));
        *result = distance;
    }
doneDown:
    ;
    {
        register int * requested;
        asm ("ldr %0, %1" : "=&l" (requested) : "m" (frame.up) : "r0", "r1", "r2", "r3", "r4", "r5", "r6");
        if (!requested)
            goto doneUp;
        register int requestedLimit asm("r0") = *requested;
        asm ("" : "+l" (requestedLimit));
        frame.upLimit = requestedLimit;
        register int edgeTile asm("r2") = frame.firstY;
        asm ("" : "+l" (edgeTile));
        register int edgePixels asm("r1") = edgeTile * 8;
        asm ("" : "+l" (edgePixels));
        register EntityMovementRect const * scanBounds asm("r3") = frame.rectangle;
        asm ("" : "+l" (scanBounds));
        register int edgeOffset asm("r7") = offsetof(EntityMovementRect, top);
        asm ("" : "+l" (edgeOffset));
        register int pixelEdge asm("r0");
        asm ("ldrsh %0, [%1, %2]" : "=l" (pixelEdge) : "l" (scanBounds), "l" (edgeOffset));
        register int distance asm("r1") = edgePixels - pixelEdge;
        asm ("" : "+l" (distance));
        frame.scanProgress = distance;
        register int initialLimit asm("r0") = frame.upLimit;
        asm ("" : "+l" (initialLimit));
        if (distance <= initialLimit)
            goto doneUp;
        register int row asm("r5") = edgeTile;
        asm ("" : "+l" (row));
        row -= 1;
        asm ("" : "+l" (row));
        if (row < 0)
            goto blockedUp;
        {
            register int rowOffset asm("r4") = width;
            asm ("" : "+l" (rowOffset));
            rowOffset *= row;
            asm ("" : "+l" (rowOffset));
            register int previousRow asm("r1") = width;
            asm ("" : "+l" (previousRow));
            previousRow = -previousRow;
            asm ("" : "+l" (previousRow));
            frame.previousRow = previousRow;
            do
            {
                register int column asm("r3") = firstX;
                asm ("" : "+l" (column));
                if (firstX < endColumn)
                {
                    register unsigned cell asm("r0") = firstX + rowOffset;
                    asm ("" : "+l" (cell));
                    register unsigned tileOffset asm("r1") = cell * sizeof(u32);
                    asm ("" : "+l" (tileOffset));
                    register u8 const * indexBase asm("r2") = indices;
                    asm ("" : "+l" (indexBase));
                    register u8 const * indexRow asm("r0");
                    asm ("add %0, %1, %2" : "=l" (indexRow) : "l" (rowOffset), "l" (indexBase) : "cc");
                    register u8 const * index asm("r2");
                    asm ("add %0, %1, %2" : "=l" (index) : "l" (firstX), "l" (indexRow) : "cc");
                    do
                    {
                        register u8 const * indexTable;
                        asm ("mov %0, %1" : "=&l" (indexTable) : "h" (indices) : "r0");
                        register u32 const * tile asm("r0");
                        if (indexTable)
                        {
                            tile = flags + *index;
                            asm ("" : "+l" (tile));
                        }
                        else
                        {
                            register u32 const * directFlags asm("r7") = flags;
                            asm ("" : "+l" (directFlags));
                            tile = reinterpret_cast<u32 const *>(reinterpret_cast<u8 const *>(directFlags) + tileOffset);
                            asm ("" : "+l" (tile));
                        }
                        if (*tile << 31)
                            goto blockedUp;
                        tileOffset += sizeof(u32);
                        index += 1;
                        ++column;
                        asm ("" : "+l" (tileOffset), "+l" (index), "+l" (column));
                    } while (column < endColumn);
                }
                register int remaining asm("r0") = frame.scanProgress - 8;
                asm ("" : "+l" (remaining));
                frame.scanProgress = remaining;
                register int maxDistance asm("r1") = frame.upLimit;
                asm ("" : "+l" (maxDistance));
                if (remaining <= maxDistance)
                    goto doneUp;
                register int rowStep asm("r2") = frame.previousRow;
                asm ("" : "+l" (rowStep));
                rowOffset += rowStep;
                --row;
                asm ("" : "+l" (row));
            } while (row >= 0);
        }
    blockedUp:
        register int blockedDistance asm("r3") = frame.scanProgress;
        asm ("" : "+l" (blockedDistance));
        register int * result asm("r7") = frame.up;
        asm ("" : "+l" (result));
        *result = blockedDistance;
    }
doneUp:
    ;
    {
        register int * requested asm("r0") = left;
        asm ("" : "+l" (requested));
        if (!requested)
            goto doneLeft;
        register int requestedLimit asm("r1") = *requested;
        asm ("" : "+l" (requestedLimit));
        frame.scanProgress = requestedLimit;
        register int edgePixels asm("r1") = firstX * 8;
        asm ("" : "+l" (edgePixels));
        register EntityMovementRect const * scanBounds asm("r2") = frame.rectangle;
        asm ("" : "+l" (scanBounds));
        register int edgeOffset asm("r3") = offsetof(EntityMovementRect, left);
        asm ("" : "+l" (edgeOffset));
        register int pixelEdge asm("r0");
        asm ("ldrsh %0, [%1, %2]" : "=l" (pixelEdge) : "l" (scanBounds), "l" (edgeOffset));
        register int distance asm("r5") = edgePixels - pixelEdge;
        asm ("" : "+l" (distance));
        register int initialLimit asm("r7") = frame.scanProgress;
        asm ("" : "+l" (initialLimit));
        if (distance <= initialLimit)
            goto doneLeft;
        register int column asm("r4") = firstX - 1;
        asm ("" : "+l" (column));
        if (column < 0)
            goto blockedLeft;
        {
            register int firstRow asm("r1") = frame.firstY;
            asm ("" : "+l" (firstRow));
            register int scaledRow asm("r0") = firstRow * sizeof(u32);
            asm ("" : "+l" (scaledRow));
            register int rowWidth asm("r2") = width;
            asm ("" : "+l" (rowWidth));
            rowWidth *= scaledRow;
            asm ("" : "+l" (rowWidth));
            frame.leftFirstRowOffset = rowWidth;
            do
            {
                register int row asm("r3") = frame.firstY;
                asm ("" : "+l" (row));
                register int innerLimit asm("r6") = frame.endY;
                asm ("" : "+l" (innerLimit));
                if (row < innerLimit)
                {
                    register unsigned columnOffset asm("r0") = column * sizeof(u32);
                    asm ("" : "+l" (columnOffset));
                    register unsigned firstRowOffset asm("r7") = frame.leftFirstRowOffset;
                    asm ("" : "+l" (firstRowOffset));
                    register unsigned tileOffset asm("r2");
                    asm ("add %0, %1, %2" : "=l" (tileOffset) : "l" (firstRowOffset), "l" (columnOffset) : "cc");
                    register int indexRow asm("r1") = width;
                    asm ("" : "+l" (indexRow));
                    indexRow *= row;
                    asm ("" : "+l" (indexRow));
                    register u8 const * indexBase asm("r6") = indices;
                    asm ("" : "+l" (indexBase));
                    register u8 const * indexColumn asm("r0");
                    asm ("add %0, %1, %2" : "=l" (indexColumn) : "l" (column), "l" (indexBase) : "cc");
                    register u8 const * index asm("r1");
                    asm ("add %0, %1, %2" : "=l" (index) : "l" (indexRow), "l" (indexColumn) : "cc");
                    register unsigned strideWidth asm("r7") = width;
                    asm ("" : "+l" (strideWidth));
                    register unsigned flagStride asm("r6");
                    asm ("lsl %0, %1, #2" : "=l" (flagStride) : "l" (strideWidth) : "cc");
                    do
                    {
                        register u8 const * indexTable asm("r0") = indices;
                        asm ("" : "+l" (indexTable));
                        register u32 const * tile asm("r0");
                        if (indexTable)
                        {
                            tile = flags + *index;
                            asm ("" : "+l" (tile));
                        }
                        else
                        {
                            register u32 const * directFlags asm("r7") = flags;
                            asm ("" : "+l" (directFlags));
                            tile = reinterpret_cast<u32 const *>(reinterpret_cast<u8 const *>(directFlags) + tileOffset);
                            asm ("" : "+l" (tile));
                        }
                        if (*tile << 31)
                            goto blockedLeft;
                        tileOffset += flagStride;
                        index += width;
                        ++row;
                        asm ("" : "+l" (tileOffset), "+l" (index), "+l" (row));
                    } while (row < frame.endY);
                }
                distance -= 8;
                asm ("" : "+l" (distance));
                register int maxDistance asm("r1") = frame.scanProgress;
                asm ("" : "+l" (maxDistance));
                if (distance <= maxDistance)
                    goto doneLeft;
                --column;
                asm ("" : "+l" (column));
            } while (column >= 0);
        }
    blockedLeft:
        register int * result asm("r2") = left;
        asm ("" : "+l" (result));
        *result = distance;
    }
doneLeft:
    ;
    {
        register int * requested asm("r3") = right;
        asm ("" : "+l" (requested));
        if (!requested)
            goto doneRight;
        register int requestedLimit asm("r6") = *requested;
        asm ("" : "+l" (requestedLimit));
        frame.rightLimit = requestedLimit;
        register int edgeTile asm("r7") = endColumn;
        asm ("" : "+l" (edgeTile));
        register int edgePixels asm("r1");
        asm ("lsl %0, %1, #3" : "=l" (edgePixels) : "l" (edgeTile) : "cc");
        register EntityMovementRect const * scanBounds asm("r2") = frame.rectangle;
        asm ("" : "+l" (scanBounds));
        register int edgeOffset asm("r3") = offsetof(EntityMovementRect, right);
        asm ("" : "+l" (edgeOffset));
        register int pixelEdge asm("r0");
        asm ("ldrsh %0, [%1, %2]" : "=l" (pixelEdge) : "l" (scanBounds), "l" (edgeOffset));
        register int distance asm("r5") = edgePixels - pixelEdge;
        asm ("" : "+l" (distance));
        if (distance >= requestedLimit)
            goto doneRight;
        register int lastColumn asm("r6") = width;
        asm ("" : "+l" (lastColumn));
        --lastColumn;
        asm ("" : "+l" (lastColumn));
        frame.scanProgress = lastColumn;
        register int column asm("r4") = endColumn;
        asm ("" : "+l" (column));
        if (column > lastColumn)
            goto blockedRight;
        {
            register int firstRow asm("r7") = frame.firstY;
            asm ("" : "+l" (firstRow));
            register int scaledRow asm("r0");
            asm ("lsl %0, %1, #2" : "=l" (scaledRow) : "l" (firstRow) : "cc");
            register int rowWidth asm("r1") = width;
            asm ("" : "+l" (rowWidth));
            rowWidth *= scaledRow;
            asm ("" : "+l" (rowWidth));
            register int rightFirstRowOffset asm("sl") = rowWidth;
            asm ("" : "+h" (rightFirstRowOffset));
            do
            {
                register int row asm("r2") = frame.firstY;
                asm ("" : "+l" (row));
                register int innerLimit asm("r3") = frame.endY;
                asm ("" : "+l" (innerLimit));
                if (row < innerLimit)
                {
                    register unsigned columnOffset asm("r0") = column * sizeof(u32);
                    asm ("" : "+l" (columnOffset));
                    register unsigned firstRowOffset asm("r6") = rightFirstRowOffset;
                    asm ("" : "+l" (firstRowOffset));
                    register unsigned tileOffset asm("r3");
                    asm ("add %0, %1, %2" : "=l" (tileOffset) : "l" (firstRowOffset), "l" (columnOffset) : "cc");
                    register int indexRow asm("r1") = width;
                    asm ("" : "+l" (indexRow));
                    indexRow *= row;
                    asm ("" : "+l" (indexRow));
                    register u8 const * indexBase asm("r7") = indices;
                    asm ("" : "+l" (indexBase));
                    register u8 const * indexColumn asm("r0");
                    asm ("add %0, %1, %2" : "=l" (indexColumn) : "l" (column), "l" (indexBase) : "cc");
                    register u8 const * index asm("r1");
                    asm ("add %0, %1, %2" : "=l" (index) : "l" (indexRow), "l" (indexColumn) : "cc");
                    register unsigned strideWidth asm("r0") = width;
                    asm ("" : "+l" (strideWidth));
                    register unsigned flagStride asm("r6") = strideWidth * sizeof(u32);
                    asm ("" : "+l" (flagStride));
                    do
                    {
                        register u8 const * indexTable;
                        asm ("mov %0, %1" : "=&l" (indexTable) : "h" (indices) : "r0");
                        register u32 const * tile asm("r0");
                        if (indexTable)
                        {
                            tile = flags + *index;
                            asm ("" : "+l" (tile));
                        }
                        else
                        {
                            register u32 const * directFlags asm("r7") = flags;
                            asm ("" : "+l" (directFlags));
                            tile = reinterpret_cast<u32 const *>(reinterpret_cast<u8 const *>(directFlags) + tileOffset);
                            asm ("" : "+l" (tile));
                        }
                        if (*tile << 31)
                            goto blockedRight;
                        tileOffset += flagStride;
                        index += width;
                        ++row;
                        asm ("" : "+l" (tileOffset), "+l" (index), "+l" (row));
                    } while (row < frame.endY);
                }
                distance += 8;
                asm ("" : "+l" (distance));
                register int maxDistance asm("r1") = frame.rightLimit;
                asm ("" : "+l" (maxDistance));
                if (distance >= maxDistance)
                    goto doneRight;
                ++column;
                asm ("" : "+l" (column));
                register int columnLimit asm("r2") = frame.scanProgress;
                asm ("" : "+l" (columnLimit));
                if (column > columnLimit)
                    break;
            } while (true);
        }
    blockedRight:
        register int * result asm("r3") = right;
        asm ("" : "+l" (result));
        *result = distance;
    }
doneRight:
    ;
}

EC SECTION(".text.080ABBA0") void ResetEntityMovementPositiveY(EntityMovement & movement)
{
    movement.positiveY = movement.maxDistance;
    ClampEntityMovementToBlockedTiles(movement.terrain, movement.bounds, &movement.positiveY, 0, 0, 0);
}

EC SECTION(".text.080ABBC4") void ResetEntityMovementNegativeY(EntityMovement & movement)
{
    movement.negativeY = -movement.maxDistance;
    ClampEntityMovementToBlockedTiles(movement.terrain, movement.bounds, 0, &movement.negativeY, 0, 0);
}

EC SECTION(".text.080ABBEC") void ResetEntityMovementNegativeX(EntityMovement & movement)
{
    movement.negativeX = -movement.maxDistance;
    ClampEntityMovementToBlockedTiles(movement.terrain, movement.bounds, 0, 0, &movement.negativeX, 0);
}

EC SECTION(".text.080ABC14") void ResetEntityMovementPositiveX(EntityMovement & movement)
{
    movement.positiveX = movement.maxDistance;
    ClampEntityMovementToBlockedTiles(movement.terrain, movement.bounds, 0, 0, 0, &movement.positiveX);
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
