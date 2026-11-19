#include "prelude.h"

struct PACKED ALIGN(2) MoreItem
{
    u32 id : 10;
    u32 flags : 6;
};

struct PACKED ALIGN(2) MoreItemSet
{
    MoreItem items[3];
};

struct MoreItems
{
    u32 size;
    u32 data[8];
    MoreItemSet extra;
};

EC u32 func_0800FFD0(MoreItems const * self)
{
    return self->size;
}

EC u8 * func_0800FFD4(MoreItems * self)
{
    return reinterpret_cast<u8 *>(self->data);
}

EC u8 * func_0800FFD8(MoreItems * self)
{
    return reinterpret_cast<u8 *>(self->data) + self->size;
}

EC MoreItemSet func_0800FFE0(MoreItems const * self)
{
    return self->extra;
}

EC void SECTION(".text.set") func_08010014(MoreItems * self, MoreItemSet const & value)
{
    self->extra = value;
}
