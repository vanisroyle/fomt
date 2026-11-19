#include "prelude.h"

extern u16 gSramErrorFlags; // 0x03000400

// 0x08000728
EC void SramProxy_RecordError(void * proxy, u16 error_code)
{
    switch (error_code)
    {
    case 1: gSramErrorFlags |= 1; break;
    case 2: gSramErrorFlags |= 2; break;
    case 0x100:
    case 0x200: gSramErrorFlags |= error_code; break;
    case 4: gSramErrorFlags |= 4; break;
    case 0x20: gSramErrorFlags |= 0x20; break;
    case 8: gSramErrorFlags |= 8; break;
    case 0x10: gSramErrorFlags |= 0x10; break;
    case 0x40: gSramErrorFlags |= 0x40; break;
    case 0x80: gSramErrorFlags |= 0x80; break;
    }
}
