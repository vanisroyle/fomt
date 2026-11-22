#include "field.hh"
#include "g5_graphics.hh"
#include <string.h>

EC MapData const * GetMapData(u32 map_id);

struct FieldPlotTileFlags
{
    u8 occupied : 1;
    u8 other : 7;
    u8 padding[3];
};

#define PIN_TILE_VALUE(value) asm ("" : "+r" (value))

EC SECTION(".text.g5_080CFEA8") void MapGraphics_DrawFieldPlot(u8 * renderer, int x, int y, FieldPlot * plot, Unk_Something const * variant) // 0x080CFEA8
{
    asm ("" : : "m" (plot));
    register u32 stride asm("r8") = GetMapData(*(u32 *) (renderer + 4))->width;
    PIN_TILE_VALUE(stride);
    register u8 const * tiles asm("r5") = (u8 const *) plot->method_0800AF20();
    PIN_TILE_VALUE(tiles);
    if (tiles)
    {
        register u32 row asm("r0") = stride;
        PIN_TILE_VALUE(row);
        row *= y;
        PIN_TILE_VALUE(row);
        register u8 * map asm("r1") = *(u8 **) (renderer + 0x10);
        PIN_TILE_VALUE(map);
        row *= 2;
        PIN_TILE_VALUE(row);
        register u32 column asm("r2") = x;
        PIN_TILE_VALUE(column);
        register u32 offset asm("r4") = column * 2;
        PIN_TILE_VALUE(offset);
        map = (u8 *) (offset + (u32) map);
        PIN_TILE_VALUE(map);
        row += (u32) map;
        PIN_TILE_VALUE(row);
        memcpy((void *) row, tiles, 4);
        register u32 next_y asm("r0") = y + 1;
        PIN_TILE_VALUE(next_y);
        register u32 product asm("r3") = stride;
        PIN_TILE_VALUE(product);
        product *= next_y;
        PIN_TILE_VALUE(product);
        row = product;
        PIN_TILE_VALUE(row);
        map = *(u8 **) (renderer + 0x10);
        PIN_TILE_VALUE(map);
        row *= 2;
        PIN_TILE_VALUE(row);
        offset += (u32) map;
        PIN_TILE_VALUE(offset);
        row += offset;
        PIN_TILE_VALUE(row);
        memcpy((void *) row, tiles + 4, 4);
        renderer[0x29] = 1;
    }
    register u32 row_start asm("r5") = stride;
    PIN_TILE_VALUE(row_start);
    row_start *= y;
    PIN_TILE_VALUE(row_start);
    register u32 after_y asm("r0") = y + 1;
    PIN_TILE_VALUE(after_y);
    register u32 next_y asm("r10") = after_y;
    PIN_TILE_VALUE(next_y);
    register Unk_Something const * style asm("r1") = variant;
    PIN_TILE_VALUE(style);
    if (style)
    {
        register u32 before_y asm("r0") = y - 1;
        PIN_TILE_VALUE(before_y);
        register u32 product asm("r2") = stride;
        PIN_TILE_VALUE(product);
        product *= before_y;
        PIN_TILE_VALUE(product);
        register u32 row asm("r0") = product;
        PIN_TILE_VALUE(row);
        register u8 * map asm("r1") = *(u8 **) (renderer + 0x18);
        PIN_TILE_VALUE(map);
        row *= 2;
        PIN_TILE_VALUE(row);
        register u32 column asm("r3") = x;
        PIN_TILE_VALUE(column);
        register u32 offset asm("r4") = column * 2;
        PIN_TILE_VALUE(offset);
        map = (u8 *) (offset + (u32) map);
        PIN_TILE_VALUE(map);
        row += (u32) map;
        PIN_TILE_VALUE(row);
        memcpy((void *) row, variant, 4);
        map = *(u8 **) (renderer + 0x14);
        PIN_TILE_VALUE(map);
        row = row_start * 2;
        PIN_TILE_VALUE(row);
        map = (u8 *) (offset + (u32) map);
        PIN_TILE_VALUE(map);
        row += (u32) map;
        PIN_TILE_VALUE(row);
        memcpy((void *) row, (u8 const *) variant + 4, 4);
        register u32 width asm("r1") = stride;
        PIN_TILE_VALUE(width);
        row = next_y;
        PIN_TILE_VALUE(row);
        row *= width;
        PIN_TILE_VALUE(row);
        map = *(u8 **) (renderer + 0x14);
        PIN_TILE_VALUE(map);
        row *= 2;
        PIN_TILE_VALUE(row);
        offset += (u32) map;
        PIN_TILE_VALUE(offset);
        row += offset;
        PIN_TILE_VALUE(row);
        memcpy((void *) row, (u8 const *) variant + 8, 4);
        renderer[0x29] = 1;
    }
    register u32 value asm("r0");
    asm ("" : "=r" (value) : "0" (plot->method_0800A07C()));
    register u32 column asm("r2") = x;
    PIN_TILE_VALUE(column);
    register u32 upper_index asm("r4") = column + row_start;
    PIN_TILE_VALUE(upper_index);
    register FieldPlotTileFlags * flags asm("r2") = *(FieldPlotTileFlags **) (renderer + 0x2c);
    PIN_TILE_VALUE(flags);
    upper_index *= sizeof(FieldPlotTileFlags);
    PIN_TILE_VALUE(upper_index);
    register FieldPlotTileFlags * upper asm("r4") = (FieldPlotTileFlags *) (upper_index + (u32) flags);
    PIN_TILE_VALUE(upper);
    register u32 width asm("r3") = stride;
    PIN_TILE_VALUE(width);
    register u32 lower_index asm("r1") = next_y;
    PIN_TILE_VALUE(lower_index);
    lower_index *= width;
    PIN_TILE_VALUE(lower_index);
    lower_index += x;
    PIN_TILE_VALUE(lower_index);
    lower_index *= sizeof(FieldPlotTileFlags);
    PIN_TILE_VALUE(lower_index);
    register FieldPlotTileFlags * lower asm("r1") = (FieldPlotTileFlags *) (lower_index + (u32) flags);
    PIN_TILE_VALUE(lower);
    register u32 occupied asm("r5") = 1;
    asm ("" : : "r" (occupied));
    occupied &= value;
    asm ("" : : "r" (occupied));
    lower[1].occupied = occupied;
    lower[0].occupied = occupied;
    upper[1].occupied = occupied;
    upper[0].occupied = occupied;
}

#undef PIN_TILE_VALUE
