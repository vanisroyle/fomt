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

// A slot stores its displayed value and two as-yet-unidentified asset words.
struct UiEntry
{
    u32 value;
    u32 unknown_04;
    u32 unknown_08;
};

EC G5Position const gInventorySpecialSlotPosition; // 0x08117838
EC G5Position const gInventorySlotPositions[]; // 0x0811783C
EC DrawingPosition const gInventorySpecialSlotCountPosition; // 0x0811785C
EC DrawingPosition const gInventorySlotCountPositions[]; // 0x08117860
EC void InitializeUiEntry(UiEntry * entry); // 0x080CAC70
EC void SetUiEntryValue(UiEntry * entry, u32 value); // 0x080CAD10
EC u32 GetUiEntryValue(UiEntry * entry); // 0x080CAD14
EC void SetUiEntry(UiEntry * entry, u32 unknown_04, u32 unknown_08, u32 value); // 0x080CADA0
EC void ClearUiEntry(UiEntry * entry); // 0x080CADA8
EC void InventoryMenu_ClearLeftSpecialSlotCount(u8 * panel); // 0x080CC0B4
EC void InventoryMenu_ClearRightSpecialSlotCount(u8 * panel); // 0x080CD4D4

#endif
