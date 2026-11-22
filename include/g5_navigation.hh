#ifndef G5_NAVIGATION_HH
#define G5_NAVIGATION_HH

#include "g5_inventory.hh"

// Slots 0..8 belong to the left panel and 9..17 to the right panel.
// Each panel can disable its special slot and limit its selectable entries.
struct InventoryNavigationPanel
{
    u8 pad_00[0x27c];
    bool enabled;
    u8 pad_27d[0x1b];
    int last_slot;
};

struct InventoryNavigationOwner
{
    u8 pad_00[0x78];
    InventoryNavigationPanel * left_panel;
    InventoryNavigationPanel * right_panel;
};

struct InventoryNavigation
{
    InventoryNavigationOwner * owner;
    int selected_slot;
    int previous_slot;
    int left_last_slot;
    int right_last_slot;

    void UpdateLimits()
    {
        left_last_slot = owner->left_panel->last_slot;
        right_last_slot = owner->right_panel->last_slot + 9;
    }
};

EC int InventoryMenu_GetPanelSlotLimit(InventoryNavigationPanel * panel); // 0x080CD938
EC bool InventoryMenu_IsSpecialSlotEnabled(InventoryNavigationPanel * panel); // 0x080CD944
EC void InventoryMenu_SetSelection(InventoryNavigation * navigation, int slot); // 0x080CDDAC
EC int InventoryMenu_SwitchSelectionPanel(InventoryNavigation * navigation); // 0x080CDD7C
EC int InventoryMenu_MoveSelectionUp(InventoryNavigation * navigation); // 0x080CDB08
EC int InventoryMenu_MoveSelectionLeft(InventoryNavigation * navigation); // 0x080CDC00
EC int InventoryMenu_MoveSelectionRight(InventoryNavigation * navigation); // 0x080CDCD0
// Coordinates are defined only when the selected panel exists.
EC G5Position * InventoryMenu_GetSelectionPosition(G5Position * position, InventoryNavigationOwner * owner, int slot); // 0x080CE120

#endif
