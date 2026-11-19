#include "prelude.h"

EC void func_08050EBC(void *, u32);
EC void func_08050E98(void *, u32);
EC void func_08050D8C(void *, u32);
EC void func_08050DC8(void *, u32, u32, u32);

struct UiEntry
{
    u32 value;
    u32 first;
    u32 second;
};

EC SECTION(".text.g5_buttons") void func_080CABA0(u8 * self, u32 value)
{
    void * buttons = self + 0x348;
    func_08050EBC(buttons, 4);
    func_08050EBC(buttons, 8);
    func_08050EBC(buttons, 1);
    func_08050EBC(buttons, 2);
    func_08050EBC(buttons, 16);
    func_08050EBC(buttons, 32);
    func_08050D8C(buttons, value);
}

EC SECTION(".text.g5_buttons") void func_080CABEC(u8 * self, u32 value)
{
    void * buttons = self + 0x348;
    func_08050E98(buttons, 4);
    func_08050E98(buttons, 8);
    func_08050E98(buttons, 1);
    func_08050E98(buttons, 2);
    func_08050E98(buttons, 16);
    func_08050E98(buttons, 32);
    func_08050DC8(buttons, value, 0, 0);
}

EC SECTION(".text.g5_entry_init") void func_080CAC70(UiEntry * self)
{
    self->value = 0;
    self->first = 0;
    self->second = 0;
}

EC SECTION(".text.g5_entry_access") void func_080CAD10(UiEntry * self, u32 value)
{
    self->value = value;
}

EC SECTION(".text.g5_entry_access") u32 func_080CAD14(UiEntry * self)
{
    return self->value;
}

EC SECTION(".text.g5_entry_update") void func_080CADA0(UiEntry * self, u32 first, u32 second, u32 value)
{
    self->first = first;
    self->second = second;
    self->value = value;
}

EC SECTION(".text.g5_entry_update") void func_080CADA8(UiEntry * self)
{
    self->value = 0;
    self->first = 0;
    self->second = 0;
}
