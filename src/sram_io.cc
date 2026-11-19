#include "prelude.h"

extern u16 gUnk_03000400;
EC void func_080D379C(void const * src, void * dst, u32 size);

EC bool func_080006E4(void * self, void * dst, u32 offset, u32 size)
{
    gUnk_03000400 = 0;
    if (size == 0)
        return false;
    else
    {
        func_080D379C(reinterpret_cast<void *>(0x0E000000 | offset), dst, size);
        return true;
    }
}
