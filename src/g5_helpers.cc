#include "prelude.h"

struct G5Position
{
    unsigned int x : 8;
    unsigned int rest : 24;
};

EC G5Position const gUnk_08117838;
EC G5Position const gUnk_0811783C[];
EC void func_080CAF6C(void *);

EC SECTION(".text.g5_080CB2E8") void * func_080CB2E8(u8 * self, u32 index)
{
    return (void *) (index * 12 + (u32) self + 0x218);
}

EC SECTION(".text.g5_080CB2E8") void * func_080CB2FC(u8 * self)
{
    return self + 0x280;
}

EC SECTION(".text.g5_080CC134") void func_080CC134(G5Position * out, void *, u32 index)
{
    *out = gUnk_0811783C[index];
}

EC SECTION(".text.g5_080CC134") void func_080CC144(G5Position * out, void *)
{
    *out = gUnk_08117838;
}

EC SECTION(".text.g5_080CC134") void func_080CC150(void * self)
{
    func_080CAF6C(self);
}

EC SECTION(".text.g5_080CC70C") void * func_080CC70C(u8 * self, u32 index)
{
    return (void *) (index * 12 + (u32) self + 0x218);
}

EC SECTION(".text.g5_080CC70C") void * func_080CC720(u8 * self)
{
    return self + 0x280;
}

EC SECTION(".text.g5_080CD534") void func_080CD534(G5Position * out, void *, u32 index)
{
    G5Position pos = gUnk_0811783C[index];
    pos.x += 120;
    *out = pos;
}

EC SECTION(".text.g5_080CD534") void func_080CD558(G5Position * out, void *)
{
    G5Position pos = gUnk_08117838;
    pos.x += 120;
    *out = pos;
}

EC SECTION(".text.g5_080CE184") int func_080CE184(void *, int index, int row)
{
    int offset;
    if (index <= 8)
        offset = row * 8 - 1;
    else
        offset = row * 8 - 10;
    return index + offset;
}

EC SECTION(".text.g5_080CF050") u32 func_080CF050(u32 * self)
{
    return self[0x7c / 4];
}

EC SECTION(".text.g5_080CF050") u32 func_080CF054(u32 * self)
{
    return self[0x80 / 4];
}
