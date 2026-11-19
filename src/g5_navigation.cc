#include "g5_navigation.hh"

EC int const gUnk_0811788C[][4];

EC SECTION(".text.g5_080CDD7C") int InventoryMenu_SwitchSelectionPanel(InventoryNavigation * navigation) // 0x080CDD7C
{
    register int next_slot asm("r3") = 10;
    asm ("" : "+l" (next_slot));
    int selected_slot = navigation->selected_slot;
    InventoryNavigationPanel * panel;
    if (selected_slot <= 8)
        panel = navigation->owner->right_panel;
    else
    {
        panel = navigation->owner->left_panel;
        next_slot = 1;
    }
    if (panel == nullptr)
        next_slot = selected_slot;
    if (next_slot != navigation->selected_slot)
    {
        navigation->previous_slot = navigation->selected_slot;
        navigation->selected_slot = next_slot;
    }
    return next_slot;
}

EC SECTION(".text.g5_080CD938") int InventoryMenu_GetPanelSlotLimit(InventoryNavigationPanel * panel) // 0x080CD938
{
    return panel->last_slot;
}

EC SECTION(".text.g5_080CD938") bool InventoryMenu_IsSpecialSlotEnabled(InventoryNavigationPanel * panel) // 0x080CD944
{
    return panel->enabled;
}

EC SECTION(".text.g5_080CDDAC") void InventoryMenu_SetSelection(InventoryNavigation * navigation, int slot) // 0x080CDDAC
{
    navigation->UpdateLimits();
    navigation->previous_slot = navigation->selected_slot;
    navigation->selected_slot = slot;
}

EC SECTION(".text.g5_080CDA44") int MoveInventorySelectionDown(InventoryNavigation * self) // 0x080CDA44
{
    self->UpdateLimits();
    int next_slot = gUnk_0811788C[self->selected_slot][0];
    if (self->selected_slot == 0)
    {
        int previous_slot = self->previous_slot;
        if (previous_slot != 9)
        {
            u32 column_index = previous_slot - 5;
            next_slot = previous_slot + 4;
            if (column_index <= 3)
                next_slot = previous_slot;
        }
    }
    else if (self->selected_slot == 9)
    {
        int previous_slot = self->previous_slot;
        if (previous_slot != 0)
        {
            u32 column_index = previous_slot - 14;
            next_slot = previous_slot + 4;
            if (column_index <= 3)
                next_slot = previous_slot;
        }
    }
    int selected_slot;
    if (next_slot <= 8)
    {
        selected_slot = self->selected_slot;
        if (next_slot == 0 && self->owner->left_panel->enabled == 0)
            next_slot = selected_slot + 4;
        while (next_slot > self->left_last_slot)
            next_slot = gUnk_0811788C[next_slot][0];
    }
    else
    {
        selected_slot = self->selected_slot;
        if (next_slot == 9 && self->owner->right_panel->enabled == 0)
            next_slot = selected_slot + 4;
        while (next_slot > self->right_last_slot)
            next_slot = gUnk_0811788C[next_slot][0];
    }
    if (next_slot != selected_slot)
    {
        self->previous_slot = selected_slot;
        self->selected_slot = next_slot;
    }
    return next_slot;
}
