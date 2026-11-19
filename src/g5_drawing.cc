#include "g5_inventory.hh"
#include "g5_graphics.hh"

static inline void * GetTilemapAddress(DrawingPosition const & position, u16 * base)
{
    u8 const * coordinates = (u8 const *) &position;
    u32 row_offset = coordinates[1] * 64;
    u32 column_offset = coordinates[0] * 2;
    u32 address = column_offset + (u32) base;
    return (void *) (row_offset + address);
}

EC SECTION(".text.g5_080CB948") void DrawInventorySlotCount(u8 * self, u32 index, u32 count) // 0x080CB948
{
    SetUiEntryValue((UiEntry *) (index * 12 + (u32) self + 0x218), count);
    DrawingPosition position = gInventorySlotCountPositions[index];
    QueuePanelTileDraw(self, GetTilemapAddress(position, (u16 *) 0x0600f080), 0x54, 2, 1, 0, 0x20);
    if (count > 1)
        QueuePanelNumberDraw(self, count, GetTilemapAddress(position, (u16 *) 0x0600f082), 0x20, 0, 0x20);
}

EC SECTION(".text.g5_080CB948") void DrawInventorySpecialSlotCount(u8 * self, u32 count) // 0x080CB9DC
{
    SetUiEntryValue((UiEntry *) (self + 0x280), count);
    DrawingPosition position = gInventorySpecialSlotCountPosition;
    QueuePanelTileDraw(self, GetTilemapAddress(position, (u16 *) 0x0600f080), 0x54, 2, 1, 0, 0x20);
    if (count > 1)
        QueuePanelNumberDraw(self, count, GetTilemapAddress(position, (u16 *) 0x0600f082), 0x20, 0, 0x20);
}

EC SECTION(".text.g5_080CB948") void ClearInventorySlotCount(u8 * self, u32 index) // 0x080CBA60
{
    ClearUiEntry((UiEntry *) (index * 12 + (u32) self + 0x218));
    DrawingPosition position = gInventorySlotCountPositions[index];
    QueuePanelTileDraw(self, GetTilemapAddress(position, (u16 *) 0x0600f000), 0x54, 2, 2, 0, 0x20);
    QueuePanelTileDraw(self, GetTilemapAddress(position, (u16 *) 0x0600f080), 0x54, 2, 1, 0, 0x20);
}

EC SECTION(".text.g5_080CCD48") void DrawRightInventorySlotCount(u8 * self, u32 index, u32 count) // 0x080CCD48
{
    SetUiEntryValue((UiEntry *) (index * 12 + (u32) self + 0x218), count);
    DrawingPosition base = gInventorySlotCountPositions[index];
    base.x += 15;
    DrawingPosition position = base;
    QueuePanelTileDraw(self, GetTilemapAddress(position, (u16 *) 0x0600f080), 0x54, 2, 1, 0, 0x20);
    if (count > 1)
        QueuePanelNumberDraw(self, count, GetTilemapAddress(position, (u16 *) 0x0600f082), 0x20, 0, 0x20);
}

EC SECTION(".text.g5_080CCD48") void ClearRightInventorySlotCount(u8 * self, u32 index) // 0x080CCDEC
{
    ClearUiEntry((UiEntry *) (index * 12 + (u32) self + 0x218));
    DrawingPosition base = gInventorySlotCountPositions[index];
    base.x += 15;
    DrawingPosition position = base;
    QueuePanelTileDraw(self, GetTilemapAddress(position, (u16 *) 0x0600f080), 0x54, 2, 1, 0, 0x20);
}
