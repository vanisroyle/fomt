#include "prelude.h"

struct PACKED ALIGN(2) PackedExtraItem
{
    u32 item_id : 10;
    u32 item_flags : 6;
};

struct PACKED ALIGN(2) ExtraItemSet
{
    PackedExtraItem items[3];
};

struct ItemCollection
{
    u32 byte_count;
    u32 item_bytes[8];
    ExtraItemSet extra_items;
};

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
