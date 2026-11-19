#include "g5_inventory.hh"

EC void func_08050EBC(void *, u32);
EC void func_08050E98(void *, u32);
EC void SetMessageWindowText(void *, const char *);
EC void SetMessageWindowTextOptions(void *, const char *, u16, const void *);

EC SECTION(".text.g5_buttons") void func_080CABA0(u8 * self, u32 value)
{
    void * buttons = self + 0x348;
    func_08050EBC(buttons, 4);
    func_08050EBC(buttons, 8);
    func_08050EBC(buttons, 1);
    func_08050EBC(buttons, 2);
    func_08050EBC(buttons, 16);
    func_08050EBC(buttons, 32);
    SetMessageWindowText(buttons, (const char *) value);
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
    SetMessageWindowTextOptions(buttons, (const char *) value, 0, 0);
}

EC SECTION(".text.g5_entry_init") void InitializeUiEntry(UiEntry * self) // 0x080CAC70
{
    self->value = 0;
    self->unknown_04 = 0;
    self->unknown_08 = 0;
}

EC SECTION(".text.g5_entry_access") void SetUiEntryValue(UiEntry * self, u32 value) // 0x080CAD10
{
    self->value = value;
}

EC SECTION(".text.g5_entry_access") u32 GetUiEntryValue(UiEntry * self) // 0x080CAD14
{
    return self->value;
}

EC SECTION(".text.g5_entry_update") void SetUiEntry(UiEntry * self, u32 unknown_04, u32 unknown_08, u32 value) // 0x080CADA0
{
    self->unknown_04 = unknown_04;
    self->unknown_08 = unknown_08;
    self->value = value;
}

EC SECTION(".text.g5_entry_update") void ClearUiEntry(UiEntry * self) // 0x080CADA8
{
    self->value = 0;
    self->unknown_04 = 0;
    self->unknown_08 = 0;
}
