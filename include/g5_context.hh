#ifndef G5_CONTEXT_HH
#define G5_CONTEXT_HH

#include "prelude.h"

struct G5InventoryPanelVTable
{
    u32 unknown_00[3];
    void (*update)(u8 * panel);
};

// The menu starts with a hardware context. Its message handle is at +0x80,
// followed by cached phase/result words; child panels are at +0x78/+0x7c.
EC void InventoryMenu_UpdateMessageWindow(u8 * menu); // 0x080CD8A0

#endif
