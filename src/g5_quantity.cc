#include "g5_quantity.hh"

EC void func_08008EB8(u32, void *, u32);
EC void func_0804EDB4(u32, void *, u32, u32, u32);
EC void func_08075BC8(void *, int, int);

static inline u16 * GetQuantityRow(QuantitySelector * selector, u32 row_offset)
{
    QuantityLayer * layer = selector->context->layer;
    u32 row = selector->row + row_offset;
    u16 * map = layer->map;
    return map + row * 32;
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
