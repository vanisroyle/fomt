#include "prelude.h"

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
EC int const gUnk_0811788C[][4];

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

