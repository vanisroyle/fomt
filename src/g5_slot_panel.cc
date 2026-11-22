#include "g5_graphics.hh"
#include "g5_inventory.hh"
#include <string.h>

EC u8 vtable_unk_080E85BC[];

EC SECTION(".text.g5_080CAFC4") u8 * InventoryMenu_InitializeSlotPanel(u8 * panel, void * owner, DrawCommand * draw_owner, u32 slots, char const * caption, u32 special_slot_enabled) // 0x080CAFC4
{
    register void * owner_value asm("r9") = owner;
    asm ("" : "+r" (owner_value));
    register DrawCommand * base_draw_owner asm("r1") = draw_owner;
    asm ("" : "+l" (base_draw_owner));
    register u32 slot_count asm("r10") = slots;
    asm ("" : "+r" (slot_count));
    register u32 enabled asm("r8") = *(u8 *) &special_slot_enabled;
    asm ("" : "+r" (enabled));
    InventoryMenu_InitializePanelBase(panel, base_draw_owner);
    *(void **) (panel + 0x210) = vtable_unk_080E85BC;
    UiEntry empty_entry;
    InitializeUiEntry(&empty_entry);
    register u32 count_offset asm("r1") = 0x214 + offsetof(InventorySlotPanelEntries, count);
    asm ("" : "+l" (count_offset));
    register u32 * size_pointer asm("r1") = (u32 *) (panel + count_offset);
    asm ("" : "+l" (size_pointer));
    u32 * size_slot = size_pointer;
    asm ("" : : "m" (size_slot));
    register u32 fill_count asm("ip") = slot_count;
    asm ("" : "+r" (fill_count));
    register UiEntry const * fill_pointer asm("r4") = &empty_entry;
    asm ("" : "+l" (fill_pointer));
    UiEntry const * fill_entry = fill_pointer;
    asm ("" : : "m" (fill_entry));
    *size_slot = 0;
    register u32 count_check asm("r6") = slot_count;
    asm ("" : "+l" (count_check));
    if (count_check <= 8)
    {
        register u32 remaining asm("r3") = slot_count;
        asm ("" : "+l" (remaining));
        register u32 entry_offset asm("r7") = 0x214 + offsetof(InventorySlotPanelEntries, entries);
        asm ("" : "+l" (entry_offset));
        UiEntry * destination = (UiEntry *) (panel + entry_offset);
        if (count_check != 0)
        {
            do
            {
                if (destination != nullptr)
                {
                    asm ("" : "+m" (fill_entry));
                    *destination = *fill_entry;
                }
                --remaining;
                ++destination;
            } while (remaining != 0);
        }
        asm ("" : "+m" (size_slot));
        register u32 count_value asm("r0") = fill_count;
        asm ("" : "+l" (count_value));
        register u32 * count_destination asm("r1") = size_slot;
        asm ("" : "+l" (count_destination));
        *count_destination = count_value;
    }
    register u32 owner_offset asm("r4") = 0x278;
    asm ("" : "+l" (owner_offset));
    register void ** owner_destination asm("r0") = (void **) (panel + owner_offset);
    asm ("" : "+l" (owner_destination));
    register void * owner_copy asm("r6") = owner_value;
    asm ("" : "+l" (owner_copy));
    *owner_destination = owner_copy;
    register u32 enabled_offset asm("r7") = 0x27c;
    asm ("" : "+l" (enabled_offset));
    register u8 * enabled_destination asm("r0") = panel + enabled_offset;
    asm ("" : "+l" (enabled_destination));
    register u32 enabled_copy asm("r1") = enabled;
    asm ("" : "+l" (enabled_copy));
    *enabled_destination = enabled_copy;
    owner_offset += 8;
    asm ("" : "+l" (owner_offset));
    InitializeUiEntry((UiEntry *) (panel + owner_offset));
    enabled_offset += 16;
    asm ("" : "+l" (enabled_offset));
    char * caption_buffer = (char *) panel + enabled_offset;
    u32 length = strlen(caption);
    if (length > 8)
        length = 8;
    memcpy(caption_buffer, caption, length);
    caption_buffer[length] = 0;
    *(u32 *) (panel + 0x298) = slot_count;
    return panel;
}
