#include "actor.hh"

struct G1HarvestSpriteHomeLocationEntry
{
    u8 x;
    u8 y;
    u8 facing;
    u8 reserved;
};

EC G1HarvestSpriteHomeLocationEntry const gUnk_081040EC[];

EC SECTION(".text.g1_0809E804") ActorLocation GetHarvestSpriteHomeLocation(u32 index)
{
    if (index <= 6)
    {
        G1HarvestSpriteHomeLocationEntry const * table = gUnk_081040EC;
        G1HarvestSpriteHomeLocationEntry const * entry = table + index;
        return ActorLocation(Location(28, entry->x, entry->y), entry->facing);
    }
    else
        return ActorLocation(Location(28, 0, 0), 0);
}
