#include "g5_inventory.hh"

EC void SetMessageWindowButtonMask(void *, u32);
EC void ClearMessageWindowButtonMask(void *, u32);
EC void SetMessageWindowText(void *, const char *);
EC void SetMessageWindowTextOptions(void *, const char *, u16, const void *);

EC SECTION(".text.g5_buttons") void func_080CABA0(u8 * self, u32 value)
{
    void * buttons = self + 0x348;
    SetMessageWindowButtonMask(buttons, 4);
    SetMessageWindowButtonMask(buttons, 8);
    SetMessageWindowButtonMask(buttons, 1);
    SetMessageWindowButtonMask(buttons, 2);
    SetMessageWindowButtonMask(buttons, 16);
    SetMessageWindowButtonMask(buttons, 32);
    SetMessageWindowText(buttons, (const char *) value);
}

EC SECTION(".text.g5_buttons") void func_080CABEC(u8 * self, u32 value)
{
    void * buttons = self + 0x348;
    ClearMessageWindowButtonMask(buttons, 4);
    ClearMessageWindowButtonMask(buttons, 8);
    ClearMessageWindowButtonMask(buttons, 1);
    ClearMessageWindowButtonMask(buttons, 2);
    ClearMessageWindowButtonMask(buttons, 16);
    ClearMessageWindowButtonMask(buttons, 32);
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
