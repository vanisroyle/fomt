#include "g5_navigation.hh"

EC int const gUnk_0811788C[][4];

EC SECTION(".text.g5_080CDC00") int InventoryMenu_MoveSelectionLeft(InventoryNavigation * input) // 0x080CDC00
{
    InventoryNavigation * navigation = input;
    asm ("" : "+l" (navigation) : : "r0", "r1", "r2", "r3", "r4", "r5", "r6");
    navigation->UpdateLimits();
    InventoryNavigationPanel * left = navigation->owner->left_panel;
    InventoryNavigationPanel * right = navigation->owner->right_panel;
    int left_limit = navigation->left_last_slot;
    int right_limit = navigation->right_last_slot;
    register int const (* table)[4] asm("r1") = gUnk_0811788C;
    asm ("" : "+l" (table), "+l" (navigation));
    register int selected_slot asm("r3") = navigation->selected_slot;
    asm ("" : "+l" (selected_slot));
    register u32 selected_offset asm("r0") = selected_slot * 16;
    asm ("" : "+l" (selected_offset));
    register u8 const * column asm("r1") = (u8 const *) table + 8;
    asm ("" : "+l" (column));
    int next_slot = *(int const *) (selected_offset + (u32) column);
    register int previous_selection asm("r5");
    if (next_slot <= 8)
    {
        if (next_slot == 0)
        {
            u32 enabled = left->enabled;
            previous_selection = selected_slot;
            asm ("" : "+l" (previous_selection));
            if (!enabled)
                next_slot = previous_selection;
        }
        else
        {
            previous_selection = selected_slot;
            asm ("" : "+l" (previous_selection));
            if (next_slot > left_limit)
            {
                int limit = left_limit;
                asm ("" : "+l" (limit));
                do
                {
                    if (limit <= 4)
                        next_slot = limit;
                    else
                        next_slot = *(int const *) (column + next_slot * 16);
                } while (next_slot > limit);
            }
        }
        if (next_slot == 4 && navigation->previous_slot == navigation->left_last_slot && navigation->previous_slot > 4)
            next_slot = navigation->previous_slot;
    }
    else
    {
        if (next_slot == 9)
        {
            u32 enabled = right->enabled;
            previous_selection = selected_slot;
            asm ("" : "+l" (previous_selection));
            if (!enabled)
                next_slot = 0;
        }
        else
        {
            previous_selection = selected_slot;
            asm ("" : "+l" (previous_selection));
            if (next_slot > right_limit)
            {
                int limit = right_limit;
                asm ("" : "+l" (limit));
                do
                {
                    if (limit <= 13)
                        next_slot = limit;
                    else
                        next_slot = *(int const *) (column + next_slot * 16);
                } while (next_slot > limit);
            }
        }
        if (next_slot == 13 && navigation->previous_slot == navigation->right_last_slot && navigation->previous_slot > 13)
            next_slot = navigation->previous_slot;
    }
    if (next_slot != previous_selection)
    {
        navigation->previous_slot = previous_selection;
        navigation->selected_slot = next_slot;
    }
    return next_slot;
}
