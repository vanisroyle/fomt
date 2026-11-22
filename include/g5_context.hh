#ifndef G5_CONTEXT_HH
#define G5_CONTEXT_HH

#include "prelude.h"

struct G5InventoryPanelVTable
{
    u32 unknown_00[3];
    void (*update)(u8 * panel);
};

// The large inventory context releases nine palette owners (+0x3ec) and nine
// tile owners (+0x3a4) in reverse order, then its fixed panels and message window.
// Its embedded menu records are 16 bytes; optional state's records are 64 bytes.
EC void InventoryMenu_DestroyHardwareContext(u8 * context, u32 destroy_flags); // 0x080C8360

// The menu starts with a hardware context. Its message handle is at +0x80,
// followed by cached phase/result words; child panels are at +0x78/+0x7c.
EC void InventoryMenu_UpdateMessageWindow(u8 * menu); // 0x080CD8A0
EC void G5Display_SetVCountCompare(void * context, u16 scanline); // 0x080CAC54

#endif
