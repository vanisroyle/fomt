#include "prelude.h"

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
