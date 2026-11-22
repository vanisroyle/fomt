#include "g1_map_forage.hh"
#include "rucksack_item.hh"

EC u32 GetMapStateBit(u8 *, u32, u32);
EC i32 FindMapForageLocation(G1MapAvailability *, i32, Box const *, i32);
EC void ClearMapAvailabilityBit(G1MapAvailability *, u32, int);

struct G1MapForageBounds
{
    i16 x1, y1, x2, y2;
    G1MapForageBounds(i32 x, i32 y, i32 width, i32 height)
        : x1(x), y1(y), x2(x + width), y2(y + height) {}
};

static inline bool MapForageOverlaps(G1MapForageBounds const & a, Box const & b)
{
    return ((a.x1 - b.x2) & (b.x1 - a.x2) & (a.y1 - b.y2) & (b.y1 - a.y2)) < 0;
}

EC SECTION(".text.g1_080A1B38") RucksackItem PickUpMapForageItem(G1MapAvailability * self, i32 map, Box const * area, i32 season)
{
    RucksackItem result;
    i32 index = FindMapForageLocation(self, map, area, season);
    if (index >= 0)
    {
        i32 kind = gUnk_081043BC[index].kind;
        if (kind <= 18)
        {
            if (kind <= 13)
            {
                u32 id = FOOD_BAMBOO_SHOOT;
                switch (kind)
                {
                case 0: id = FOOD_BAMBOO_SHOOT; break;
                case 1: id = FOOD_WILD_GRAPES; break;
                case 2: id = FOOD_MUSHROOM; break;
                case 3: id = FOOD_POISONOUS_MUSHROOM; break;
                case 4: id = FOOD_TRUFFLE; break;
                case 5: id = FOOD_BLUE_GRASS; break;
                case 6: id = FOOD_GREEN_GRASS; break;
                case 7: id = FOOD_RED_GRASS; break;
                case 8: id = FOOD_YELLOW_GRASS; break;
                case 9: id = FOOD_ORANGE_GRASS; break;
                case 10: id = FOOD_PURPLE_GRASS; break;
                case 11: id = FOOD_INDIGO_GRASS; break;
                case 12: id = FOOD_WHITE_GRASS; break;
                case 13: id = FOOD_APPLE; break;
                }
                result = RucksackItem(Food(id));
            }
            else
            {
                u32 id = ARTICLE_MOON_DROP_GRASS;
                switch ((u32)(kind - 14))
                {
                case 0: id = ARTICLE_MOON_DROP_GRASS; break;
                case 1: id = ARTICLE_PINK_CAT_GRASS; break;
                case 2: id = ARTICLE_BLUE_MAGIC_GRASS; break;
                case 3: id = ARTICLE_TOY_FLOWER; break;
                case 4: id = ARTICLE_BRANCHES; break;
                }
                result = RucksackItem(Article(id));
            }
            ClearMapAvailabilityBit(self, index, season);
            return result;
        }
    }
    return result;
}

EC SECTION(".text.g1_080A1D20") i32 FindMapForageLocation(G1MapAvailability * self, i32 map, Box const * area, i32 season)
{
    i32 index = 0;
    i32 end = 26;
    switch (season)
    {
    case 0: index = 0; end = 26; break;
    case 1: index = 26; end = 58; break;
    case 2: index = 58; end = 94; break;
    case 3: index = 94; end = 96; break;
    }
    for (; index < end; index++)
    {
        if (gUnk_081043BC[index].map == map)
        {
            i32 x = gUnk_081043BC[index].x;
            i32 y = gUnk_081043BC[index].y;
            G1MapForageBounds bounds(x, y, 16, 16);
            if (MapForageOverlaps(bounds, *area))
                break;
        }
    }
    if (index == end)
    {
        for (index = 96, end = 116; index < end; index++)
        {
            if (gUnk_081043BC[index].map == map)
            {
                i32 width = 32;
                i32 height = 32;
                if (gUnk_081043BC[index].kind == 20)
                    width = 48;
                if (gUnk_081043BC[index].kind == 18)
                {
                    height = 16;
                    width = 16;
                }
                i32 x = gUnk_081043BC[index].x;
                i32 y = gUnk_081043BC[index].y;
                G1MapForageBounds bounds(x, y, width, height);
                if (MapForageOverlaps(bounds, *area))
                    break;
            }
        }
        if (index == end)
            index = -1;
    }
    if (index >= 0 && !static_cast<u8>(GetMapStateBit(reinterpret_cast<u8 *>(self), index, season)))
        index = -1;
    return index;
}

EC SECTION(".text.g1_080A1CBC") G1MapForageObstacle GetMapForageObstacle(G1MapAvailability const *, u32 index)
{
    G1MapForageObstacle result;
    result.location.kind = gUnk_081043BC[index].kind;
    result.location.map = gUnk_081043BC[index].map;
    result.location.x = gUnk_081043BC[index].x;
    result.location.y = gUnk_081043BC[index].y;
    switch (result.location.kind)
    {
    case 19: case 21: result.size = 1; break;
    case 20: result.size = 2; break;
    default: result.size = 0; break;
    }
    return result;
}
