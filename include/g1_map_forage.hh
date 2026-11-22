#ifndef G1_MAP_FORAGE_HH
#define G1_MAP_FORAGE_HH

#include "g1_map.hh"

struct RucksackItem;

struct G1MapForageLocation
{
    i32 kind;
    i32 map;
    i16 x;
    i16 y;
};

struct G1MapForageObstacle
{
    G1MapForageLocation location;
    i32 size;
};

EC G1MapForageLocation const gUnk_081043BC[];
EC RucksackItem PickUpMapForageItem(G1MapAvailability *, i32, Box const *, i32);

#endif
