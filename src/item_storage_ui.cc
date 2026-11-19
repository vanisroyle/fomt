#include "prelude.h"
#include "item.hh"
#include "rucksack.hh"
#include "held_item.hh"
#include "farm_house.hh"
#include "furniture.hh"
#include "farm.hh"
#include "barn.hh"
#include "coop.hh"
#include "cow.hh"
#include "sheep.hh"
#include "chicken.hh"
#include "horse.hh"

EC Food func_0800F20C(HeldItem const *);
EC Article func_0800F258(HeldItem const *);

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

EC SECTION(".text.item_storage_ui.0804E22C") u32 HasStoredArticle(ItemStorageScriptContext * context, u32 articleId)
{
    HeldItem const * held = (HeldItem *) (context->game + 0x1c2c);
    if (!func_0800F190(held) && func_0800F204(held) == HeldItem::KIND_ARTICLE)
    {
        Article article = func_0800F258(held);
        bool matches = article.GetId() == articleId;
        if (matches) return 1;
    }
    u32 slot = ((Rucksack *) (context->game + 0x1c38))->GetFirstSlotWithArticle(articleId);
    if (slot != ~0u) return 1;
    Shelf * shelf = ((FarmHouse *) (context->game + 0x1f4))->GetShelf();
    if (shelf != nullptr && shelf->GetFirstSlotWith(articleId) != slot) return 1;
    return 0;
}

EC SECTION(".text.item_storage_ui.0804E2C0") u32 HasStoredFood(ItemStorageScriptContext * context, u32 foodId)
{
    HeldItem const * held = (HeldItem *) (context->game + 0x1c2c);
    if (!func_0800F190(held) && func_0800F204(held) == HeldItem::KIND_FOOD)
    {
        Food food = func_0800F20C(held);
        bool matches = food.GetId() == foodId;
        if (matches) return 1;
    }
    u32 slot = ((Rucksack *) (context->game + 0x1c38))->GetFirstSlotWithFood(foodId);
    if (slot != ~0u) return 1;
    Fridge * fridge = ((FarmHouse *) (context->game + 0x1f4))->GetFridge();
    if (fridge != nullptr && fridge->GetFirstSlotWith(foodId) != slot) return 1;
    return 0;
}

EC SECTION(".text.item_storage_ui.0804E354") u32 HasStoredTool(ItemStorageScriptContext * context, u32 toolId)
{
    ToolStack const * held = (ToolStack *) (context->game + 0x1c34);
    if (!held->IsEmpty())
    {
        Tool tool = held->GetTool();
        bool matches = tool.GetId() == toolId;
        if (matches) return 1;
    }
    register u8 ** game asm("r4") = &context->game;
    register u32 slot asm("r5") = ((Rucksack *) (*game + 0x1c38))->GetFirstSlotWithTool(toolId);
    if (slot != ~0u) return 1;
    if (((ToolChest *) (*game + 0x380))->GetFirstSlotWith(toolId) != slot) return 1;
    return 0;
}

EC SECTION(".text.message_rendering.0804E3D8") Animal * GetScriptAnimal(ItemStorageScriptContext * context, u32 kind, i32 index)
{
    Animal * animal = nullptr;
    switch (kind)
    {
    case 0: animal = ((Farm *) (context->game + 0x14))->GetHorse(); break;
    case 1:
        if (index < (i32) ((Barn *) (context->game + 0x5f0))->GetCapacity())
            animal = ((Barn *) (context->game + 0x5f0))->GetCow(index);
        break;
    case 2:
        if (index < (i32) ((Barn *) (context->game + 0x5f0))->GetCapacity())
            animal = ((Barn *) (context->game + 0x5f0))->GetSheep(index);
        break;
    case 3:
        if (index < (i32) ((Coop *) (context->game + 0x410))->GetCapacity())
            animal = ((Coop *) (context->game + 0x410))->GetChicken(index);
        break;
    case 4: animal = (Animal *) (context->game + 0x1c70); break;
    }
    return animal;
}
