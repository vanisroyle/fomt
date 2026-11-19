#include "prelude.h"

extern u16 gSramErrorFlags; // 0x03000400
EC void Sram_CopyBytes(void const * source, void * destination, u32 byte_count);

// 0x080006E4
EC bool SramProxy_Read(void * proxy, void * destination, u32 sram_offset, u32 byte_count)
{
    gSramErrorFlags = 0;
    if (byte_count == 0)
        return false;
    else
    {
        Sram_CopyBytes(reinterpret_cast<void *>(0x0E000000 | sram_offset), destination, byte_count);
        return true;
    }
}
