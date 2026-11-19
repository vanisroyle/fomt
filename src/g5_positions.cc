#include "g5_navigation.hh"

EC SECTION(".text.g5_080CE120") G5Position * InventoryMenu_GetSelectionPosition(G5Position * position, InventoryNavigationOwner * owner, int slot) // 0x080CE120
{
    G5Position selected_position;
    switch (slot)
    {
    case 0:
        if (owner->left_panel != nullptr)
        {
            G5Position special;
            GetInventorySpecialSlotPosition(&special, owner->left_panel);
            selected_position = special;
        }
        break;
    case 9:
        if (owner->right_panel != nullptr)
        {
            G5Position special;
            GetRightInventorySpecialSlotPosition(&special, owner->right_panel);
            selected_position = special;
        }
        break;
    default:
        if (slot <= 8)
        {
            if (owner->left_panel != nullptr)
            {
                u32 panel_slot = slot - 1;
                G5Position entry;
                GetInventorySlotPosition(&entry, owner->left_panel, panel_slot);
                selected_position = entry;
            }
        }
        else if (owner->right_panel != nullptr)
        {
            u32 panel_slot = slot - 10;
            G5Position entry;
            GetRightInventorySlotPosition(&entry, owner->right_panel, panel_slot);
            selected_position = entry;
        }
        break;
    }
    *position = selected_position;
    return position;
}
