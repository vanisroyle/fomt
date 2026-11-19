#include "prelude.h"

extern u16 gUnk_03000400;
EC bool func_080002E0(void * self);
EC bool func_080006E4(void * self, void * dst, u32 offset, u32 size);
EC bool func_080006A4(void * self, u32 offset, void const * src, u32 size);

EC void func_08000470(void * self, u32 value)
{
    func_080006A4(self, 36, &value, 4);
}

EC u32 func_08000488(void * self)
{
    if (func_080002E0(self))
    {
        u32 value = 0;
        func_080006E4(self, &value, 36, 4);
        if (gUnk_03000400 == 0)
            return value;
    }
    return 0;
}
