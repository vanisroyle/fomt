#ifndef CHICKEN_FESTIVAL_HH
#define CHICKEN_FESTIVAL_HH

#include "utility/fixed_str.hh"

struct ChickenFestivalEntrant
{
    FixedStr<12> name;
    u8 unknown_10;
    u8 affection;
    SECTION(".text.g3_080B3C90") ChickenFestivalEntrant();
};

#endif
