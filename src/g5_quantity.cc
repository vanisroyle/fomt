#include "g5_quantity.hh"

EC void func_08008EB8(u32, void *, u32);
EC void func_0804EDB4(u32, void *, u32, u32, u32);
EC void func_08075BC8(void *, int, int);
EC void func_0804E9F4(void *, u32, u32, u32, u32, u32);
EC void func_0804EE30(u32, void *, u32, u32, u32);
EC int func_080088D4(void *);
EC void func_08008B6C(void *, u32);

static inline u16 * GetQuantityRow(QuantitySelector * selector, u32 row_offset)
{
    QuantityLayer * layer = selector->context->layer;
    u32 row = selector->row + row_offset;
    u16 * map = layer->map;
    return map + row * 32;
}

static inline void RefreshQuantity(QuantityContext * context)
{
    QuantityLayer * layer = context->layer;
    int scroll = layer->scroll;
    func_08075BC8(layer, scroll / 8, (scroll + 256) / 8);
}

static inline void * GetQuantityTile(u16 * row, u32 column)
{
    return (void *) ((u32) row + column * 2);
}

EC SECTION(".text.g5_080CA8B4") void InventoryMenu_OpenQuantitySelector(QuantitySelector * selector, u32 unit_price, u32 limit, u32 column) // 0x080CA8B4
{
    selector->row = selector->context->rows * 2 + 4;
    selector->column = column;
    selector->unit_price = unit_price;
    if (limit > 99)
        selector->limit = 99;
    else
        selector->limit = limit;
    QuantityContext * context = selector->context;
    u32 affordable = *(u32 *) (context->game + 0x1aa8) / selector->unit_price;
    if (affordable < selector->limit)
        selector->limit = affordable;
    func_0804E9F4(GetQuantityTile(GetQuantityRow(selector, 1), column), 0x2e, 1, 1, 0, 0x20);
    func_0804EE30(selector->quantity, GetQuantityTile(GetQuantityRow(selector, 1), column + 2), 0x24, 0, 0x20);
    func_0804E9F4(GetQuantityTile(GetQuantityRow(selector, 1), column + 4), 0x2f, 1, 1, 0, 0x20);
    RefreshQuantity(selector->context);
    selector->active = 1;
}

EC SECTION(".text.g5_080CAAF4") void InventoryMenu_ClearQuantitySelector(QuantitySelector * selector) // 0x080CAAF4
{
    func_08008EB8(0x03ff03ff, GetQuantityRow(selector, 1) + selector->column, 0x10);
    func_08008EB8(0x03ff03ff, GetQuantityRow(selector, 0) + 12, 0x1c);
    func_08008EB8(0x03ff03ff, GetQuantityRow(selector, 1) + 12, 0x1c);
    func_0804EDB4(selector->unit_price, GetQuantityRow(selector, 0) + 25, 0xa0, 0, 0x20);
    QuantityLayer * layer = selector->context->layer;
    int scroll = layer->scroll;
    func_08075BC8(layer, scroll / 8, (scroll + 256) / 8);
    selector->active = 0;
    selector->row = 0;
    selector->quantity = 1;
}
