#include "prelude.h"
#include "item.hh"
#include "rucksack.hh"
#include "held_item.hh"
#include "farm_house.hh"
#include "furniture.hh"

struct ItemStorageScriptContext
{
    u8 unknown_000[0x350];
    u8 * game;
};

EC SECTION(".text.item_storage_ui.0804E17C") u32 CanReceiveStoredFood(ItemStorageScriptContext * context, u32 foodId) // 0x0804E17C
{
    if (func_0800F190((HeldItem *) (context->game + 0x1c2c))) return 1;
    if (((Rucksack *) (context->game + 0x1c38))->GetEmptyItemSlotCount()) return 1;
    Fridge * storage = ((FarmHouse *) (context->game + 0x1f4))->GetFridge();
    if (storage != nullptr && storage->GetAvailableSpaceFor(foodId)) return 1;
    return 0;
}

EC SECTION(".text.item_storage_ui.0804E1D4") u32 CanReceiveStoredArticle(ItemStorageScriptContext * context, u32 articleId) // 0x0804E1D4
{
    if (func_0800F190((HeldItem *) (context->game + 0x1c2c))) return 1;
    if (((Rucksack *) (context->game + 0x1c38))->GetEmptyItemSlotCount()) return 1;
    Shelf * storage = ((FarmHouse *) (context->game + 0x1f4))->GetShelf();
    if (storage != nullptr && storage->GetAvailableSpaceFor(articleId)) return 1;
    return 0;
}
