#ifndef G5_QUANTITY_HH
#define G5_QUANTITY_HH

#include "prelude.h"

// Quantity UI overlays a scrolling 32-column tilemap. The selector tracks
// unit price, an affordability/stock limit, and the currently selected count.
struct QuantityLayer
{
    u8 unknown_00[0xc];
    i16 scroll;
    u8 unknown_0e[10];
    u16 * map;
};
struct QuantityContext
{
    u8 unknown_00[8];
    u8 * game;
    u32 unknown_0c;
    u32 rows;
    u32 unknown_14[2];
    QuantityLayer * layer;
};
struct QuantitySelector
{
    u32 unit_price;
    u32 limit;
    u8 active;
    u8 unknown_09[3];
    u32 row;
    u32 column;
    u32 quantity;
    QuantityContext * context;
};

EC void InventoryMenu_ClearQuantitySelector(QuantitySelector * selector); // 0x080CAAF4
EC void InventoryMenu_UpdateQuantitySelector(QuantitySelector * selector); // 0x080CA98C
EC void InventoryMenu_OpenQuantitySelector(QuantitySelector * selector, u32 unit_price, u32 limit, u32 column); // 0x080CA8B4

#endif
