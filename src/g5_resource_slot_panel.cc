#include "g5_graphics.hh"
#include "g5_inventory.hh"
#include <string.h>

EC u8 vtable_unk_080E85AC[];
EC void TileHeapOwner_Initialize(void *);
EC void ResourceHandles_Initialize(void *);
EC void TileHandle_Release(void *, u32);
EC u32 func_08007B54(void *, u32);
EC void ResourceHandle_Release(void *, u32);
EC u32 func_0800736C(void *);

EC SECTION(".text.g5_080CC15C") u8 * InventoryMenu_InitializeResourceSlotPanel(u8 * input, void * owner, DrawCommand * draw_owner, u32 slot_count, char const * caption, u32 special_enabled) // 0x080CC15C
{
    u8 * panel = input;
    register void * owner_value asm("r9") = owner;
    asm ("" : "+r" (owner_value));
    register DrawCommand * base_owner asm("r1") = draw_owner;
    asm ("" : "+l" (base_owner));
    u32 count = slot_count;
    asm ("" : : "m"(count));
    register u32 enabled asm("r10") = *(u8 *)&special_enabled;
    asm ("" : "+r" (enabled));
    InventoryMenu_InitializePanelBase(panel, base_owner);
    *(void **)(panel + 0x210) = vtable_unk_080E85AC;
    UiEntry empty;
    InitializeUiEntry(&empty);
    {
        register u32 size_offset asm("r1") = 0x214;
        asm ("" : "+l" (size_offset));
        register u32 * size_pointer asm("r1") = (u32 *)(size_offset + (u32)panel);
        asm ("" : "+l" (size_pointer));
        register u32 * size_slot asm("ip") = size_pointer;
        asm ("" : : "r"(size_slot));
        register u32 count_check asm("r4") = count;
        asm ("" : "+l" (count_check));
        register u32 fill_count asm("r8") = count_check;
        asm ("" : "+r" (fill_count));
        register UiEntry const * fill_pointer asm("r5") = &empty;
        asm ("" : "+l" (fill_pointer));
        UiEntry const * fill_entry = fill_pointer;
        asm ("" : : "m"(fill_entry));
        *size_pointer = 0;
        if (count_check <= 8)
        {
            register u32 remaining asm("r3") = count;
            asm ("" : "+l" (remaining));
            register u32 entry_offset asm("r6") = 0x218;
            asm ("" : "+l" (entry_offset));
            register UiEntry * out asm("r2") = (UiEntry *)(panel + entry_offset);
            asm ("" : "+l" (out));
            if (count_check != 0)
            {
                do
                {
                    if (out != 0)
                    {
                        asm ("" : "+m"(fill_entry));
                        *out = *fill_entry;
                    }
                    --remaining;
                    ++out;
                } while (remaining != 0);
            }
            register u32 count_value asm("r0") = fill_count;
            asm ("" : "+l" (count_value));
            register u32 * destination asm("r1") = size_slot;
            asm ("" : "+l" (destination));
            *destination = count_value;
        }
    }
    register u32 owner_offset asm("r4") = 0x278;
    asm ("" : "+l" (owner_offset));
    register void ** owner_destination asm("r0") = (void **)(panel + owner_offset);
    asm ("" : "+l" (owner_destination));
    register void * owner_copy asm("r5") = owner_value;
    asm ("" : "+l" (owner_copy));
    *owner_destination = owner_copy;
    register u32 enabled_offset asm("r6") = 0x27c;
    asm ("" : "+l" (enabled_offset));
    register u8 * enabled_destination asm("r0") = panel + enabled_offset;
    asm ("" : "+l" (enabled_destination));
    register u32 zero asm("r8") = 0;
    asm ("" : "+r" (zero));
    register u32 enabled_copy asm("r4") = enabled;
    asm ("" : "+l" (enabled_copy));
    *enabled_destination = enabled_copy;
    register u32 special_offset asm("r5") = 0x280;
    asm ("" : "+l" (special_offset));
    InitializeUiEntry((UiEntry *)(panel + special_offset));
    enabled_offset += 0x10;
    asm ("" : "+l" (enabled_offset));
    register char * caption_buffer asm("r5") = (char *)panel + enabled_offset;
    asm ("" : "+l" (caption_buffer));
    u32 length = strlen(caption);
    if (length>8)length = 8;
    memcpy(caption_buffer, caption, length);
    register char * terminator asm("r0") = caption_buffer + length;
    asm ("" : "+l" (terminator));
    register u32 terminator_zero asm("r1") = zero;
    asm ("" : "+l" (terminator_zero));
    *terminator = terminator_zero;
    register u32 count_offset asm("r4") = 0x298;
    asm ("" : "+l" (count_offset));
    register u32 * count_destination asm("r0") = (u32 *)(panel + count_offset);
    asm ("" : "+l" (count_destination));
    register u32 saved_count asm("r5") = count;
    asm ("" : "+l" (saved_count));
    *count_destination = saved_count;
    {
        register u32 offset asm("r6") = 0x29c;
        asm ("" : "+l" (offset));
        register u32 * handle asm("r4") = (u32 *)(panel + offset);
        asm ("" : "+l" (handle));
        TileHeapOwner_Initialize(handle);
        register u32 tile_zero asm("r0") = zero;
        asm ("" : "+l" (tile_zero));
        handle[1] = tile_zero;
        register u32 palette_offset asm("r1") = 0x2a4;
        asm ("" : "+l" (palette_offset));
        handle = (u32 *)(panel + palette_offset);
        asm ("" : "+l" (handle));
        ResourceHandles_Initialize(handle);
        register u32 palette_zero asm("r5") = zero;
        asm ("" : "+l" (palette_zero));
        handle[1] = palette_zero;
        offset += 0x10;
        asm ("" : "+l" (offset));
        handle = (u32 *)(panel + offset);
        asm ("" : "+l" (handle));
        register int remaining asm("r5") = 7;
        asm ("" : "+l" (remaining));
        register u32 raw_zero asm("r0") = 0;
        asm ("" : "+l" (raw_zero));
        register u32 array_zero asm("r8") = raw_zero;
        asm ("" : "+r" (array_zero));
        register int final asm("r6") = 1;
        asm ("" : "+l" (final));
        final = -final;
        asm ("" : "+l" (final));
        do
        {
            TileHeapOwner_Initialize(handle);
            register u32 handle_zero asm("r1") = array_zero;
            asm ("" : "+l" (handle_zero));
            handle[1] = handle_zero;
            handle += 2;
            --remaining;
        } while (remaining != final);
    }
    {
        register u32 offset asm("r5") = 0x2ec;
        asm ("" : "+l" (offset));
        register u32 * handle asm("r4") = (u32 *)(panel + offset);
        asm ("" : "+l" (handle));
        register int remaining asm("r5") = 7;
        asm ("" : "+l" (remaining));
        register int final asm("r6") = 0;
        asm ("" : "+l" (final));
        register u32 array_zero asm("r8") = final;
        asm ("" : "+r" (array_zero));
        --final;
        asm ("" : "+l" (final));
        do
        {
            ResourceHandles_Initialize(handle);
            register u32 handle_zero asm("r0") = array_zero;
            asm ("" : "+l" (handle_zero));
            handle[1] = handle_zero;
            handle += 2;
            --remaining;
        } while (remaining != final);
    }
    {
        register u32 initial_slot asm("r1") = 0;
        asm ("" : "+l" (initial_slot));
        register u32 slot asm("r8") = initial_slot;
        asm ("" : "+r" (slot));
        register u32 offset asm("r4") = 0x2b0;
        asm ("" : "+l" (offset));
        register u32 * handles asm("r6") = (u32 *)(panel + offset);
        asm ("" : "+l" (handles));
        register u32 next_slot asm("r0");
        do
        {
            register u32 slot_value asm("r0") = slot;
            asm ("" : "+l" (slot_value));
            register u32 scaled asm("r5") = slot_value*8;
            asm ("" : "+l" (scaled));
            register u32 * tiles asm("r4") = (u32 *)(scaled + (u32)panel);
            asm ("" : "+l" (tiles));
            register u32 tile_offset asm("r1") = 0x2ac;
            asm ("" : "+l" (tile_offset));
            tiles = (u32 *)((u8 *)tiles + tile_offset);
            asm ("" : "+l" (tiles));
            register u32 tile_id asm("r1") = handles[0];
            asm ("" : "+l" (tile_id));
            TileHandle_Release(tiles, tile_id);
            handles[0] = func_08007B54(tiles, 2);
            scaled += (u32)panel;
            asm ("" : "+l" (scaled));
            register u32 palette_offset asm("r4") = 0x2ec;
            asm ("" : "+l" (palette_offset));
            register u32 * palettes asm("r5") = (u32 *)(scaled + palette_offset);
            asm ("" : "+l" (palettes));
            ResourceHandle_Release(palettes, handles[16]);
            handles[16] = func_0800736C(palettes);
            handles += 2;
            register u32 step asm("r5") = 1;
            asm ("" : "+l" (step));
            slot += step;
            next_slot = slot;
            asm ("" : "+l" (next_slot));
        } while (next_slot <= 7);
    }
    u32 enabled_check = enabled;
    asm ("" : "+l"(enabled_check) : : "r4", "r5", "r6", "r0");
    if (enabled_check != 0)
    {
        register u32 special_tile_offset asm("r5") = 0x29c;
        asm ("" : "+l" (special_tile_offset));
        register u32 * tiles asm("r4") = (u32 *)(panel + special_tile_offset);
        asm ("" : "+l" (tiles));
        TileHandle_Release(tiles, tiles[1]);
        tiles[1] = func_08007B54(tiles, 2);
        register u32 special_palette_offset asm("r6") = 0x2a4;
        asm ("" : "+l" (special_palette_offset));
        register u32 * palettes asm("r4") = (u32 *)(panel + special_palette_offset);
        asm ("" : "+l" (palettes));
        ResourceHandle_Release(palettes, palettes[1]);
        palettes[1] = func_0800736C(palettes);
    }
    return panel;
}
