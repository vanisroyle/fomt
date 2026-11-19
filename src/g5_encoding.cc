#include "prelude.h"

EC SECTION(".text.g5_080D0CD4") int func_080D0CD4(int code)
{
    if ((u32) code > 0xffff)
        return -1;
    int low = code & 0xff;
    int high = (code & 0xff00) >> 8;
    if ((u32) (low - 0x40) > 0xbc || (u32) high <= 0x80 || (u32) high > 0xea || (u32) (high - 0xa0) <= 0x3f)
        return -1;
    if ((u32) high <= 0x9f)
        high -= 0x81;
    else
        high -= 0xc1;
    low -= 0x40;
    return high * 189 + low;
}

EC SECTION(".text.g5_080CA6F4") void func_080CA6F4()
{
    switch (*(volatile u8 *) 0x04000006)
    {
    case 23:
    {
        while ((*(volatile u16 *) 0x04000004 & 2) == 0) {}
        *(volatile u16 *) 0x0400000c = 0x3e43;
        *(volatile u16 *) 0x04000004 &= 0xff;
        u32 line = 0x9f00;
        u32 flags = *(volatile u16 *) 0x04000004;
        *(volatile u16 *) 0x04000004 = line | flags;
        break;
    }
    case 159:
    {
        *(volatile u16 *) 0x0400000c = 0x3e41;
        *(volatile u16 *) 0x04000004 &= 0xff;
        u32 line = 0x1700;
        u32 flags = *(volatile u16 *) 0x04000004;
        *(volatile u16 *) 0x04000004 = line | flags;
        break;
    }
    }
}
