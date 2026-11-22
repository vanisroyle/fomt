#ifndef G5_INVENTORY_HH
#define G5_INVENTORY_HH

#include "prelude.h"

// Position records are four packed bytes. Helpers adjust only x; drawing
// uses byte 1 as y. The remaining bytes have not been identified.
struct G5Position
{
    unsigned int x : 8;
    unsigned int y_and_unknown : 24;
};
struct DrawingPosition
{
    unsigned int x : 8;
    unsigned int y_and_unknown : 24;
};

// A slot stores its displayed value and the selected sprite frame's tile and
// palette data addresses. Empty entries clear all three words.
struct UiEntry
{
    u32 value;
    u32 tile_data;
    u32 palette_data;
};

// Slot panels embed this fixed-capacity list at +0x214, before their owner
// pointer (+0x278), special-slot flag (+0x27c) and special entry (+0x280).
struct InventorySlotPanelEntries
{
    u32 count;
    UiEntry entries[8];
};

EC G5Position const gInventorySpecialSlotPosition; // 0x08117838
EC G5Position const gInventorySlotPositions[]; // 0x0811783C
EC DrawingPosition const gInventorySpecialSlotCountPosition; // 0x0811785C
EC DrawingPosition const gInventorySlotCountPositions[]; // 0x08117860
EC void InitializeUiEntry(UiEntry * entry); // 0x080CAC70
EC UiEntry * InitializeUiEntryFromAnimation(UiEntry * entry, void const * data, u16 animation, u32 value); // 0x080CAC7C
EC void AssignUiEntryFromAnimation(UiEntry * entry, void const * data, u16 animation, u32 value); // 0x080CAD18
EC void SetUiEntryValue(UiEntry * entry, u32 value); // 0x080CAD10
EC u32 GetUiEntryValue(UiEntry * entry); // 0x080CAD14
EC void SetUiEntry(UiEntry * entry, u32 tile_data, u32 palette_data, u32 value); // 0x080CADA0
EC void ClearUiEntry(UiEntry * entry); // 0x080CADA8
EC void InitializeUiEntryWithValue(UiEntry * entry, u32 tile_data, u32 palette_data, u32 value); // 0x080CAD08
EC void InventoryMenu_ClearLeftSpecialSlotCount(u8 * panel); // 0x080CC0B4
EC void InventoryMenu_ClearRightSpecialSlotCount(u8 * panel); // 0x080CD4D4
EC void InventoryMenu_GetSpecialSlotCountPosition(DrawingPosition * position, void * panel); // 0x080CD994
EC void InventoryMenu_GetSlotCountPosition(DrawingPosition * position, void * panel, u32 slot); // 0x080CD9A0
EC void InventoryMenu_SetPage(u8 * panel, u32 page); // 0x080CEB5C
EC void GetInventorySlotPosition(G5Position * position, void * panel, u32 slot); // 0x080CC134
EC void GetInventorySpecialSlotPosition(G5Position * position, void * panel); // 0x080CC144
EC void GetRightInventorySlotPosition(G5Position * position, void * panel, u32 slot); // 0x080CD534
EC void GetRightInventorySpecialSlotPosition(G5Position * position, void * panel); // 0x080CD558

#endif
