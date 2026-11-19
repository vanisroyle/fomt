#include "g5_item_text.hh"

#include "unknown_inlines.hh"

#include <string.h>

EC char const * const gUnk_08117AA4;

static inline char const * FormatWrappedItemDescription(char * description, char const * name)
{
    u32 length = min_inl<u32>(strlen(name), 99);
    memcpy(description, name, length);
    description[length] = '\0';
    char const * suffix = gUnk_08117AA4;
    length = strlen(description);
    char * end = description + length;
    u32 remaining = 99 - length;
    if (remaining != 0)
    {
        u32 suffix_length = min_inl<u32>(strlen(suffix), remaining);
        memcpy(end, suffix, suffix_length);
        end[suffix_length] = '\0';
    }
    return description;
}

EC SECTION(".text.g5_080CFA38") char const * InventoryMenu_GetRucksackItemDescription(void * menu, char * description, RucksackItem item) // 0x080CFA38
{
    char const * result = 0;
    description[0] = '\0';
    switch (item.GetKind())
    {
    case RucksackItem::KIND_FOOD:
        if (item.IsWrapped())
            result = FormatWrappedItemDescription(description, item.GetFood().GetName());
        else
            result = item.GetFood().GetDesc();
        break;
    case RucksackItem::KIND_ARTICLE:
        if (item.IsWrapped())
            result = FormatWrappedItemDescription(description, item.GetArticle().GetName());
        else
            result = item.GetArticle().GetDesc();
        break;
    }
    return result;
}
