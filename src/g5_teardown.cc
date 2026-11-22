#include "g5_context.hh"
#include <stdlib.h>
EC u8 vtable_unk_080E8570[], vtable_unk_080E5A28[];
EC void Irq_SetHandlerFlags(u32, u32);
EC void func_080E5478(void *, u32);
EC void ResourceHandle_Release(void *, u32);
EC void ResourceHandles_Destroy(void *, u32);
EC void TileHandle_Release(void *, u32);
EC void TileHeapOwner_Destroy(void *, u32);
EC void DestroyMessageWindow(void *, u32);
EC void func_08075DEC(void *, u32);
EC void HardwareContext_Destroy(void *, u32);
EC void __builtin_delete(void *);
static inline void DestroyPaletteOwner(u8 * owner)
{
    ResourceHandle_Release(owner, *(u32 *)  (owner + 4));
    ResourceHandles_Destroy(owner, 2);
}
static inline void DestroyTileOwner(u8 * owner)
{
    TileHandle_Release(owner, *(u32 *)  (owner + 4));
    TileHeapOwner_Destroy(owner, 2);
}
EC SECTION(".text.g5_080C8360") void InventoryMenu_DestroyHardwareContext(u8 * context, u32 destroy_flags) // 0x080C8360
{
    register u8 *menu asm("r5")=context;
    asm ("" : "+l" (menu));
    register u32 flags asm("r9")=destroy_flags;
    asm ("" : "+r" (flags));
    *(void **) (menu + 4)=vtable_unk_080E8570;
    u8 *palette_end = menu + 0x434;
    Irq_SetHandlerFlags(0,*(u16 *) palette_end);
    *(void **) (menu + 0x534)=vtable_unk_080E5A28;
    *(void **) (menu + 0x504)=vtable_unk_080E5A28;
    *(void **) (menu + 0x4d4)=vtable_unk_080E5A28;
    func_080E5478(menu + 0x438, 2);
    register u8 *palette_array asm("r0")=menu + 0x3ec;
    asm ("" : "+l" (palette_array));
    if (palette_array != nullptr && palette_array != palette_end)
    {
        u8 *palette_begin = palette_array;
        do
        {
            palette_end -= 8;
            DestroyPaletteOwner(palette_end);
        }
        while (palette_begin != palette_end);
    }
    register u32 tile_offset asm("r1")=0x3a4;
    asm ("" : "+l" (tile_offset));
    register u8 *tile_array asm("r0")=menu + tile_offset;
    u8 *entry_array = menu + 0x20;
    register u8 *first_entry asm("r8")=menu + 0x24;
    asm ("" : "+l" (tile_array), "+l" (entry_array), "+r" (first_entry));
    if (tile_array != nullptr)
    {
        tile_offset += 0x48;
        u8 *tile_end = menu + tile_offset;
        if (tile_array != tile_end)
        {
            register u32 tile_start_offset asm("r2")=0x3a4;
            asm ("" : "+l" (tile_start_offset));
            u8 *tile_begin = menu + tile_start_offset;
            do
            {
                tile_end -= 8;
                DestroyTileOwner(tile_end);
            }
            while (tile_begin != tile_end);
        }
    }
    DestroyPaletteOwner(menu + 0x39c);
    DestroyTileOwner(menu + 0x394);
    DestroyTileOwner(menu + 0x38c);
    DestroyPaletteOwner(menu + 0x384);
    DestroyTileOwner(menu + 0x37c);
    DestroyTileOwner(menu + 0x374);
    DestroyPaletteOwner(menu + 0x36c);
    DestroyTileOwner(menu + 0x364);
    DestroyPaletteOwner(menu + 0x35c);
    DestroyTileOwner(menu + 0x354);
    DestroyMessageWindow(menu + 0x348, 2);
    register u32 entry_offset asm("r0")=*(u32 *) (menu + 0x20)*16 + 4;
    asm ("" : "+l" (entry_offset));
    register u8 *entry_array_end asm("r1") = entry_array + entry_offset;
    register u8 *entry asm("r0")=first_entry;
    for (; entry != entry_array_end; entry += 16)
    {
    }
    register u8 *state_allocation asm("r0")=*(u8 **) (menu + 0x1c);
    asm ("" : "+l" (state_allocation));
    if (state_allocation != nullptr)
    {
        u8 *state = state_allocation;
        func_08075DEC(state + 0x28, 2);
        register u8 **state_entries asm("r0")=(u8 **) (state + 0x18);
        asm ("" : "+l" (state_entries));
        register u8 *state_entry_end asm("r2")=state_entries[1];
        register u8 *state_entry asm("r1")=*(u8 **) (state + 0x18);
        asm ("" : "+l" (state_entry_end), "+l" (state_entry));
        while (state_entry != state_entry_end)
            state_entry += 0x40;
        void *allocation = *state_entries;
        if (allocation != nullptr)
            free(allocation);
        __builtin_delete(state);
    }
    HardwareContext_Destroy(menu, flags);
}
