#include "g5_quantity.hh"

EC void Dma3_Fill(u32, void *, u32);
EC void DrawTallMessageNumber(u32, void *, u32, u32, u32);
EC void func_08075BC8(void *, int, int);
EC void FillMessageTilemapRect(void *, u32, u32, u32, u32, u32);
EC void DrawSmallMessageNumber(u32, void *, u32, u32, u32);
EC int HardwareContext_GetRepeatedKeys(void *);
EC void AudioPlayer_StartSong(void *, u32);

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
    FillMessageTilemapRect(GetQuantityTile(GetQuantityRow(selector, 1), column), 0x2e, 1, 1, 0, 0x20);
    DrawSmallMessageNumber(selector->quantity, GetQuantityTile(GetQuantityRow(selector, 1), column + 2), 0x24, 0, 0x20);
    FillMessageTilemapRect(GetQuantityTile(GetQuantityRow(selector, 1), column + 4), 0x2f, 1, 1, 0, 0x20);
    RefreshQuantity(selector->context);
    selector->active = 1;
}

static inline u16 GetQuantityRepeatedKeys(void * context, int mask)
{
    return mask & HardwareContext_GetRepeatedKeys(context);
}

static inline u16 * GetChangedQuantityTile(u16 * row, u32 column)
{
    asm ("" : "+l" (column));
    return row + column;
}

EC SECTION(".text.g5_080CA98C") void InventoryMenu_UpdateQuantitySelector(QuantitySelector * selector) // 0x080CA98C
{
    bool changed = false;
    u32 previous_quantity = selector->quantity;
    if (GetQuantityRepeatedKeys(selector->context, 0x40))
    {
        selector->quantity++;
        if (selector->quantity > selector->limit)
            selector->quantity = 1;
    }
    else if (GetQuantityRepeatedKeys(selector->context, 0x80))
    {
        selector->quantity--;
        if (selector->quantity == 0)
            selector->quantity = selector->limit;
    }
    else if (GetQuantityRepeatedKeys(selector->context, 0x10))
    {
        if (selector->quantity == selector->limit)
            selector->quantity = 1;
        else
        {
            selector->quantity += 10;
            if (selector->quantity >= selector->limit)
                selector->quantity = selector->limit;
        }
    }
    else if (GetQuantityRepeatedKeys(selector->context, 0x20))
    {
        if (selector->quantity == 1)
            selector->quantity = selector->limit;
        else if (selector->quantity <= 11)
            selector->quantity = 1;
        else
            selector->quantity -= 10;
    }
    if (previous_quantity != selector->quantity)
        changed = true;
    if (changed)
    {
        Dma3_Fill(0x03ff03ff, GetChangedQuantityTile(GetQuantityRow(selector, 1), selector->column + 1), 4);
        Dma3_Fill(0x03ff03ff, (GetQuantityRow(selector, 0) + 12), 0x1c);
        Dma3_Fill(0x03ff03ff, (GetQuantityRow(selector, 1) + 12), 0x1c);
        DrawSmallMessageNumber(selector->quantity, GetChangedQuantityTile(GetQuantityRow(selector, 1), selector->column + 2), 0x24, 0, 0x20);
        DrawTallMessageNumber(selector->quantity * selector->unit_price, (GetQuantityRow(selector, 0) + 25), 0xa0, 0, 0x20);
        RefreshQuantity(selector->context);
        AudioPlayer_StartSong((u8 *) selector->context + 0x18, 0xcb);
    }
}

EC SECTION(".text.g5_080CAAF4") void InventoryMenu_ClearQuantitySelector(QuantitySelector * selector) // 0x080CAAF4
{
    Dma3_Fill(0x03ff03ff, GetQuantityRow(selector, 1) + selector->column, 0x10);
    Dma3_Fill(0x03ff03ff, GetQuantityRow(selector, 0) + 12, 0x1c);
    Dma3_Fill(0x03ff03ff, GetQuantityRow(selector, 1) + 12, 0x1c);
    DrawTallMessageNumber(selector->unit_price, GetQuantityRow(selector, 0) + 25, 0xa0, 0, 0x20);
    QuantityLayer * layer = selector->context->layer;
    int scroll = layer->scroll;
    func_08075BC8(layer, scroll / 8, (scroll + 256) / 8);
    selector->active = 0;
    selector->row = 0;
    selector->quantity = 1;
}
