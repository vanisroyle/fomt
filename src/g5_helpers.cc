#include "g5_inventory.hh"
#include "g5_graphics.hh"
#include "item.hh"

EC SECTION(".text.g5_080CE1C4") bool IsCursedTool(void *, Tool tool) // 0x080CE1C4
{
    switch (tool.GetId())
    {
    case TOOL_CURSED_SICKLE:
    case TOOL_CURSED_HOE:
    case TOOL_CURSED_AXE:
    case TOOL_CURSED_HAMMER:
    case TOOL_CURSED_WATERING_CAN:
    case TOOL_CURSED_FISHING_ROD:
        return true;
    default:
        return false;
    }
}

EC SECTION(".text.g5_080CB2E8") void * GetLeftInventoryUiEntry(u8 * self, u32 index) // 0x080CB2E8
{
    return (void *) (index * 12 + (u32) self + 0x218);
}

EC SECTION(".text.g5_080CB2E8") void * GetLeftInventorySpecialUiEntry(u8 * self) // 0x080CB2FC
{
    return self + 0x280;
}

EC SECTION(".text.g5_080CC134") void GetInventorySlotPosition(G5Position * out, void *, u32 index) // 0x080CC134
{
    *out = gInventorySlotPositions[index];
}

EC SECTION(".text.g5_080CC134") void GetInventorySpecialSlotPosition(G5Position * out, void *) // 0x080CC144
{
    *out = gInventorySpecialSlotPosition;
}

EC SECTION(".text.g5_080CC134") void UpdateInventoryDrawCommands(void * self) // 0x080CC150
{
    PruneInactivePanelDrawCommands((u8 *) self);
}

EC SECTION(".text.g5_080CC70C") void * GetRightInventoryUiEntry(u8 * self, u32 index) // 0x080CC70C
{
    return (void *) (index * 12 + (u32) self + 0x218);
}

EC SECTION(".text.g5_080CC70C") void * GetRightInventorySpecialUiEntry(u8 * self) // 0x080CC720
{
    return self + 0x280;
}

EC SECTION(".text.g5_080CD534") void GetRightInventorySlotPosition(G5Position * out, void *, u32 index) // 0x080CD534
{
    G5Position pos = gInventorySlotPositions[index];
    pos.x += 120;
    *out = pos;
}

EC SECTION(".text.g5_080CD534") void GetRightInventorySpecialSlotPosition(G5Position * out, void *) // 0x080CD558
{
    G5Position pos = gInventorySpecialSlotPosition;
    pos.x += 120;
    *out = pos;
}

EC SECTION(".text.g5_080CE184") int GetInventoryGridCellIndex(void *, int index, int row) // 0x080CE184
{
    int offset;
    if (index <= 8)
        offset = row * 8 - 1;
    else
        offset = row * 8 - 10;
    return index + offset;
}

EC SECTION(".text.g5_080CF050") u32 GetInventoryCursorSlot(u32 * self) // 0x080CF050
{
    return self[0x7c / 4];
}

EC SECTION(".text.g5_080CF050") u32 GetInventoryCursorPage(u32 * self) // 0x080CF054
{
    return self[0x80 / 4];
}
