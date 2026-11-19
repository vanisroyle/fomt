#include "prelude.h"

extern u16 gSramErrorFlags; // 0x03000400
EC bool SaveHeader_IsValid(void * proxy);
EC bool SramProxy_Read(void * proxy, void * destination, u32 sram_offset, u32 byte_count);
EC bool SramProxy_Write(void * proxy, u32 sram_offset, void const * source, u32 byte_count);

// 0x08000470
EC void SaveHeader_SetSelectedSlot(void * proxy, u32 selected_slot)
{
    SramProxy_Write(proxy, 36, &selected_slot, 4);
}

// 0x08000488
EC u32 SaveHeader_GetSelectedSlot(void * proxy)
{
    if (SaveHeader_IsValid(proxy))
    {
        u32 selected_slot = 0;
        SramProxy_Read(proxy, &selected_slot, 36, 4);
        if (gSramErrorFlags == 0)
            return selected_slot;
    }
    return 0;
}
