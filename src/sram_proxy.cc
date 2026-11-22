#include "prelude.h"

inline i32 SaveSlot_ComputeOffset(i32 slot_index, i32 slot_stride)
{
    return slot_index * slot_stride + 0x28;
}

// 0x080003DC
EC u32 SECTION(".text.SaveSlot_GetSramOffset") SaveSlot_GetSramOffset(void * proxy, i32 slot_index)
{
    return SaveSlot_ComputeOffset(slot_index, 0x3FEC);
}

extern u16 gSramErrorFlags; // 0x03000400
extern u8 gSramProxyInitialized; // 0x03000402

// 0x0800063C
EC void SramProxy_Shutdown() {}

// 0x08000640
EC void * SramProxy_Initialize(void * proxy)
{
    if (gSramProxyInitialized == 0)
    {
        gSramProxyInitialized = 1;
        gSramErrorFlags = 0;
    }
    return proxy;
}
