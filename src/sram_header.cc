#include "prelude.h"

#include <string.h>

extern u16 gUnk_03000400;
extern u8 const gUnk_080E862C[];
EC bool func_080006E4(void * self, void * dst, u32 offset, u32 size);
EC bool func_080006A4(void * self, u32 offset, void const * src, u32 size);

EC bool func_080002E0(void * self)
{
    u8 signature[32];
    u32 slots;
    u32 selected;
    func_080006E4(self, signature, 0, 32);
    if (gUnk_03000400 != 0)
        return false;
    slots = 0;
    func_080006E4(self, &slots, 32, 4);
    if (gUnk_03000400 != 0)
        return false;
    selected = 0;
    func_080006E4(self, &selected, 36, 4);
    if (gUnk_03000400 != 0)
        return false;
    return memcmp(signature, gUnk_080E862C, 32) == 0 && (slots & 3) == slots && selected <= 1;
}

EC void func_08000358(void * self)
{
    func_080006A4(self, 0, gUnk_080E862C, 32);
    if (gUnk_03000400 != 0)
        return;
    u32 value = 0;
    func_080006A4(self, 32, &value, 4);
    if (gUnk_03000400 != 0)
        return;
    func_080006A4(self, 36, &value, 4);
}

EC u32 func_080003A0(void * self)
{
    if (func_080002E0(self))
    {
        u32 value = 0;
        func_080006E4(self, &value, 32, 4);
        if (gUnk_03000400 == 0)
            return value;
    }
    return 0;
}

EC void SECTION(".text.flags") func_080003E8(void * self, u32 slot)
{
    u32 value = 0;
    func_080006E4(self, &value, 32, 4);
    if (gUnk_03000400 == 0)
    {
        value |= 1 << slot;
        func_080006A4(self, 32, &value, 4);
    }
}
