#ifndef G5_ITEM_TEXT_HH
#define G5_ITEM_TEXT_HH

#include "rucksack_item.hh"

// Unwrapped items use the ROM description directly. Wrapped items use a
// caller-owned 100-byte buffer for the item name and wrapped-item suffix.
EC char const * InventoryMenu_GetRucksackItemDescription(void * menu, char * description, RucksackItem item); // 0x080CFA38

#endif
