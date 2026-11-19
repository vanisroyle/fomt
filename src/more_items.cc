#include "prelude.h"
#include <new>

struct PACKED ALIGN(2) PackedExtraItem
{
    u32 item_id : 10;
    u32 item_flags : 6;
};

struct PACKED ALIGN(2) ExtraItemSet
{
    PackedExtraItem items[3];
};

inline void ExtraItemSet_Initialize(ExtraItemSet * self, u32 first_item)
{
    self->items[0].item_id = first_item;
    self->items[0].item_flags = 0;
    self->items[1].item_id = 0;
    self->items[1].item_flags = 0;
    self->items[2].item_id = 0;
}

struct ItemCollection
{
    u8 * Begin() { return reinterpret_cast<u8 *>(item_bytes); }
    u8 * End() { return reinterpret_cast<u8 *>(this) + (byte_count + 4); }
    u32 byte_count;
    u32 item_bytes[8];
    ExtraItemSet extra_items;
};

// 0x0800FF8C
EC SECTION(".text.ItemCollection_Initialize") ItemCollection * ItemCollection_Initialize(ItemCollection * self)
{
    self->byte_count = 0;
    ExtraItemSet_Initialize(&self->extra_items, 0x234);
    return self;
}

// 0x0800FFF4
EC void SECTION(".text.ItemCollection_Append") ItemCollection_Append(ItemCollection * self, u32 item_id)
{
    if (self->byte_count <= 29)
    {
        new (self->End()) u8(item_id);
        ++self->byte_count;
    }
}

// 0x0800FFD0
EC u32 ItemCollection_GetByteCount(ItemCollection const * self)
{
    return self->byte_count;
}

// 0x0800FFD4
EC u8 * ItemCollection_Begin(ItemCollection * self)
{
    return reinterpret_cast<u8 *>(self->item_bytes);
}

// 0x0800FFD8
EC u8 * ItemCollection_End(ItemCollection * self)
{
    return reinterpret_cast<u8 *>(self->item_bytes) + self->byte_count;
}

// 0x0800FFE0
EC ExtraItemSet ItemCollection_GetExtraItems(ItemCollection const * self)
{
    return self->extra_items;
}

// 0x08010014
EC void SECTION(".text.set") ItemCollection_SetExtraItems(ItemCollection * self, ExtraItemSet const & extra_items)
{
    self->extra_items = extra_items;
}
