#ifndef G5_INITIALIZATION_HH
#define G5_INITIALIZATION_HH

#include "g5_navigation.hh"

// Shared sprite draw state. The cursor position and animator are separate;
// the meanings of these renderer fields are not yet identified.
struct G5SpriteDrawState
{
    u16 unknown_00, unknown_02, unknown_04, unknown_06, unknown_08;
    u16 pad_0a;
    u32 unknown_0c;
    u8 unknown_10, unknown_11;
    u16 pad_12;
    u32 unknown_14;
    u8 unknown_18;
    u8 pad_19[3];
};

EC u8 * InventoryMenu_InitializeCursor(u8 * cursor, void * owner); // 0x080CEC00
EC u8 * InventoryMenu_InitializeNavigation(u8 * navigation, InventoryNavigationOwner * owner, int slot); // 0x080CD9B0
EC u8 * InventoryMenu_InitializeContext(u8 * menu, void * owner); // 0x080CD578

#endif
