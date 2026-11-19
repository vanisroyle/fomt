#include "prelude.h"

struct DrawingPosition
{
    unsigned int x : 8;
    unsigned int rest : 24;
};

EC DrawingPosition const gUnk_0811785C;
EC DrawingPosition const gUnk_08117860[];
EC void func_080CAD10(void *, u32);
EC void func_080CADA8(void *);
EC void func_080CAE60(void *, void *, u32, u32, u32, u32, u32);
EC void func_080CAEEC(void *, u32, void *, u32, u32, u32);

static inline void * TileAt(DrawingPosition const & pos, u16 * base)
{
    u8 const * coords = (u8 const *) &pos;
    u32 y = coords[1] * 64;
    u32 x = coords[0] * 2;
    u32 address = x + (u32) base;
    return (void *) (y + address);
}

EC SECTION(".text.g5_080CB948") void func_080CB948(u8 * self, u32 index, u32 count)
{
    func_080CAD10((void *) (index * 12 + (u32) self + 0x218), count);
    DrawingPosition pos = gUnk_08117860[index];
    func_080CAE60(self, TileAt(pos, (u16 *) 0x0600f080), 0x54, 2, 1, 0, 0x20);
    if (count > 1)
        func_080CAEEC(self, count, TileAt(pos, (u16 *) 0x0600f082), 0x20, 0, 0x20);
}

EC SECTION(".text.g5_080CB948") void func_080CB9DC(u8 * self, u32 count)
{
    func_080CAD10(self + 0x280, count);
    DrawingPosition pos = gUnk_0811785C;
    func_080CAE60(self, TileAt(pos, (u16 *) 0x0600f080), 0x54, 2, 1, 0, 0x20);
    if (count > 1)
        func_080CAEEC(self, count, TileAt(pos, (u16 *) 0x0600f082), 0x20, 0, 0x20);
}

EC SECTION(".text.g5_080CB948") void func_080CBA60(u8 * self, u32 index)
{
    func_080CADA8((void *) (index * 12 + (u32) self + 0x218));
    DrawingPosition pos = gUnk_08117860[index];
    func_080CAE60(self, TileAt(pos, (u16 *) 0x0600f000), 0x54, 2, 2, 0, 0x20);
    func_080CAE60(self, TileAt(pos, (u16 *) 0x0600f080), 0x54, 2, 1, 0, 0x20);
}

EC SECTION(".text.g5_080CCD48") void func_080CCD48(u8 * self, u32 index, u32 count)
{
    func_080CAD10((void *) (index * 12 + (u32) self + 0x218), count);
    DrawingPosition base = gUnk_08117860[index];
    base.x += 15;
    DrawingPosition pos = base;
    func_080CAE60(self, TileAt(pos, (u16 *) 0x0600f080), 0x54, 2, 1, 0, 0x20);
    if (count > 1)
        func_080CAEEC(self, count, TileAt(pos, (u16 *) 0x0600f082), 0x20, 0, 0x20);
}

EC SECTION(".text.g5_080CCD48") void func_080CCDEC(u8 * self, u32 index)
{
    func_080CADA8((void *) (index * 12 + (u32) self + 0x218));
    DrawingPosition base = gUnk_08117860[index];
    base.x += 15;
    DrawingPosition pos = base;
    func_080CAE60(self, TileAt(pos, (u16 *) 0x0600f080), 0x54, 2, 1, 0, 0x20);
}
