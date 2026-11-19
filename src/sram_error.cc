#include "prelude.h"

extern u16 gUnk_03000400;

EC void func_08000728(void * self, u16 error)
{
    switch (error)
    {
    case 1: gUnk_03000400 |= 1; break;
    case 2: gUnk_03000400 |= 2; break;
    case 0x100:
    case 0x200: gUnk_03000400 |= error; break;
    case 4: gUnk_03000400 |= 4; break;
    case 0x20: gUnk_03000400 |= 0x20; break;
    case 8: gUnk_03000400 |= 8; break;
    case 0x10: gUnk_03000400 |= 0x10; break;
    case 0x40: gUnk_03000400 |= 0x40; break;
    case 0x80: gUnk_03000400 |= 0x80; break;
    }
}
