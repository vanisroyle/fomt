#include "g5_navigation.hh"

EC int const gUnk_0811788C[][4];

static inline int ReadRightNavigationSlotLimit(InventoryNavigationPanel * panel)
{
    register int count asm("r0") = panel->last_slot;
    asm ("" : "+l" (count));
    return count;
}

static inline void UpdateHorizontalNavigationLimits(InventoryNavigation * navigation)
{
    navigation->left_last_slot = navigation->owner->left_panel->last_slot;
    navigation->right_last_slot = ReadRightNavigationSlotLimit(navigation->owner->right_panel) + 9;
}

EC SECTION(".text.g5_080CDCD0") int InventoryMenu_MoveSelectionRight(InventoryNavigation * input) // 0x080CDCD0
{
    InventoryNavigation * navigation = input;
    UpdateHorizontalNavigationLimits(navigation);
    InventoryNavigationPanel * left = navigation->owner->left_panel;
    InventoryNavigationPanel * right = navigation->owner->right_panel;
    int left_limit = navigation->left_last_slot;
    int right_limit = navigation->right_last_slot;
    register InventoryNavigation * navigation_register asm("r5") = navigation;
    register int limit_register asm("r4") = right_limit;
    asm ("" : "+l" (navigation_register), "+l" (limit_register));
    navigation = navigation_register;
    right_limit = limit_register;
    register int const (* table)[4] asm("r1") = gUnk_0811788C;
    asm ("" : "+l" (navigation), "+l" (table));
    register int selected_slot asm("r3") = navigation->selected_slot;
    asm ("" : "+l" (selected_slot));
    register u32 selected_offset asm("r0") = selected_slot * 16;
    asm ("" : "+l" (selected_offset));
    register u8 const * column asm("r1") = (u8 const *) table + 12;
    asm ("" : "+l" (column));
    int next_slot = *(int const *) (selected_offset + (u32) column);
    if (next_slot <= 8)
    {
        if (next_slot == 0)
        {
            if (!left->enabled)
                next_slot = 9;
        }
        else if (next_slot > left_limit)
        {
            if (selected_slot == 17)
            {
                next_slot = 1;
                asm ("" : "+l" (next_slot));
            }
            else
            {
                for (;;)
                {
                    asm volatile ("" : "+l" (next_slot));
                    if (next_slot > 8)
                        break;
                    next_slot = *(int const *) (column + next_slot * 16);
                }
            }
        }
    }
    else
    {
        if (next_slot == 9)
        {
            if (!right->enabled)
                next_slot = 0;
        }
        else if (next_slot > right_limit)
        {
            if (right_limit <= 13)
                next_slot = 1;
            else
                while (next_slot > 8)
                    next_slot = *(int const *) (column + next_slot * 16);
        }
    }
    if (next_slot == 10 && navigation->previous_slot == 14)
        next_slot = 14;
    if (next_slot != selected_slot)
    {
        navigation->previous_slot = selected_slot;
        navigation->selected_slot = next_slot;
    }
    return next_slot;
}
