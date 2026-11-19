#include "prelude.h"

#include <string.h>

extern u16 gSramErrorFlags; // 0x03000400
extern u8 const gSaveHeaderSignature[]; // 0x080E862C
EC bool SramProxy_Read(void * proxy, void * destination, u32 sram_offset, u32 byte_count);
EC bool SramProxy_Write(void * proxy, u32 sram_offset, void const * source, u32 byte_count);

// 0x080002E0
EC bool SaveHeader_IsValid(void * proxy)
{
    u8 signature[32];
    u32 used_slot_mask;
    u32 selected_slot;
    SramProxy_Read(proxy, signature, 0, 32);
    if (gSramErrorFlags != 0)
        return false;
    used_slot_mask = 0;
    SramProxy_Read(proxy, &used_slot_mask, 32, 4);
    if (gSramErrorFlags != 0)
        return false;
    selected_slot = 0;
    SramProxy_Read(proxy, &selected_slot, 36, 4);
    if (gSramErrorFlags != 0)
        return false;
    return memcmp(signature, gSaveHeaderSignature, 32) == 0 && (used_slot_mask & 3) == used_slot_mask && selected_slot <= 1;
}

// 0x08000358
EC void SaveHeader_Initialize(void * proxy)
{
    SramProxy_Write(proxy, 0, gSaveHeaderSignature, 32);
    if (gSramErrorFlags != 0)
        return;
    u32 used_slot_mask = 0;
    SramProxy_Write(proxy, 32, &used_slot_mask, 4);
    if (gSramErrorFlags != 0)
        return;
    SramProxy_Write(proxy, 36, &used_slot_mask, 4);
}

// 0x080003A0
EC u32 SaveHeader_GetUsedSlots(void * proxy)
{
    if (SaveHeader_IsValid(proxy))
    {
        u32 used_slot_mask = 0;
        SramProxy_Read(proxy, &used_slot_mask, 32, 4);
        if (gSramErrorFlags == 0)
            return used_slot_mask;
    }
    return 0;
}

// 0x080003E8
EC void SECTION(".text.flags") SaveHeader_MarkSlotUsed(void * proxy, u32 slot_index)
{
    u32 used_slot_mask = 0;
    SramProxy_Read(proxy, &used_slot_mask, 32, 4);
    if (gSramErrorFlags == 0)
    {
        used_slot_mask |= 1 << slot_index;
        SramProxy_Write(proxy, 32, &used_slot_mask, 4);
    }
}
