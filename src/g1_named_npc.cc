#include "g1_npc.hh"
#include <string.h>

EC char const gUnk_08104108[];

SECTION(".text.g1_0809E9B8") G1NamedNpcState::G1NamedNpcState()
    : Npc(ActorLocation(Location(29, 100, 100), 0))
{
    char * destination = name;
    char const * source = gUnk_08104108;
    u32 length = strlen(source);
    if (length > 12)
        length = 12;
    memcpy(destination, source, length);
    destination[length] = 0;
    variant_flags.kind = 0;
    variant_flags.counter = 0;
    age_days = 0;
}

SECTION(".text.g1_0809EA6C") G1NamedNpcState::G1NamedNpcState(G1NamedNpcVariant const & variant, ActorLocation const & location)
    : Npc(location)
{
    char * destination = name;
    char const * source = gUnk_08104108;
    u32 length = strlen(source);
    if (length > 12)
        length = 12;
    memcpy(destination, source, length);
    destination[length] = 0;
    variant_flags = variant;
    age_days = 0;
    flag = false;
}
