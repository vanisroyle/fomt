#include "g1_map.hh"
#include "actor.hh"
#include <stdlib.h>

struct G1MapRegionReward
{
    u32 amount : 16;
    u32 bonus : 16;
    i32 region;
};

EC u16 const gUnk_08104E74[][4];
EC i32 func_080A45A8(Location const *);

EC SECTION(".text.g1_080A3E90") G1MapRegionReward GetMapLocationReward(Location const * location, GameTime const * time)
{
    G1MapRegionReward result;
    result.region = func_080A45A8(location);
    if (result.region == 7)
    {
        result.amount = 0;
        result.bonus = 0;
    }
    else
    {
        u32 period = 3;
        if (static_cast<u8>(time->clock_hour - 6) <= 2)
            period = 0;
        else if (static_cast<u8>(time->clock_hour - 9) <= 7)
            period = 1;
        else if (static_cast<u8>(time->clock_hour - 17) <= 2)
            period = 2;
        result.amount = gUnk_08104E74[result.region][period] * 60;
        i32 bonus = 0;
        int random = rand() >> 8;
        switch (result.region)
        {
        case 0: case 1: case 2: case 3: case 4: case 5: case 6:
            bonus = random % 6 + 5;
            break;
        }
        result.bonus = bonus * 6;
    }
    return result;
}
