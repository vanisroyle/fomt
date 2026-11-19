#include "g5_font.hh"

EC SECTION(".text.g5_080D0CD4") int GetShiftJisGlyphIndex(int encoded_character) // 0x080D0CD4
{
    if ((u32) encoded_character > 0xffff)
        return -1;
    int trail_byte = encoded_character & 0xff;
    int lead_byte = (encoded_character & 0xff00) >> 8;
    if ((u32) (trail_byte - 0x40) > 0xbc || (u32) lead_byte <= 0x80 || (u32) lead_byte > 0xea || (u32) (lead_byte - 0xa0) <= 0x3f)
        return -1;
    if ((u32) lead_byte <= 0x9f)
        lead_byte -= 0x81;
    else
        lead_byte -= 0xc1;
    trail_byte -= 0x40;
    return lead_byte * 189 + trail_byte;
}

EC SECTION(".text.g5_080CA6F4") void HandleMenuBg1ScanlineInterrupt() // 0x080CA6F4
{
    switch (*(volatile u8 *) 0x04000006)
    {
    case 23:
    {
        while ((*(volatile u16 *) 0x04000004 & 2) == 0) {}
        *(volatile u16 *) 0x0400000c = 0x3e43;
        *(volatile u16 *) 0x04000004 &= 0xff;
        u32 scanline_compare = 0x9f00;
        u32 flags = *(volatile u16 *) 0x04000004;
        *(volatile u16 *) 0x04000004 = scanline_compare | flags;
        break;
    }
    case 159:
    {
        *(volatile u16 *) 0x0400000c = 0x3e41;
        *(volatile u16 *) 0x04000004 &= 0xff;
        u32 scanline_compare = 0x1700;
        u32 flags = *(volatile u16 *) 0x04000004;
        *(volatile u16 *) 0x04000004 = scanline_compare | flags;
        break;
    }
    }
}
