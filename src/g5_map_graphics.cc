#include "prelude.h"
#include "unknown_types.hh"

EC MapData const * GetMapData(u32 map_id);
EC void Unpack(void const * source, void * destination);
EC void Dma3_Copy(void const * source, void * destination, u32 size);
EC u8 const gUnk_086D6368[];
EC u8 const gUnk_08699FF4[];

EC SECTION(".text.g5_080CFB5C") void MapGraphics_LoadPaletteBuffer(u8 * renderer, u32 unused, u32 buffer_index) // 0x080CFB5C
{
    void const * palette = GetMapData(*(u32 *) (renderer + 4))->packed_pal1;
    u32 buffer_offset = buffer_index * sizeof(u8 *);
    asm ("" : "+l" (buffer_offset));
    u8 ** buffers = (u8 **) (renderer + 0x1c);
    asm ("" : "+l" (buffers));
    u8 ** buffer = (u8 **) ((u8 *) buffers + buffer_offset);
    Unpack(palette, *buffer);
    if (*(u32 *) (renderer + 4) == 0x3e)
    {
        Dma3_Copy(gUnk_086D6368, *buffer + 0x100, 0x60);
        Dma3_Copy(gUnk_08699FF4, *buffer + 0x160, 0x80);
    }
}
