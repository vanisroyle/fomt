#include "g1_npc.hh"

struct G1EmptyNpcEventSlot
{
    G1EmptyNpcEventSlot() : kind(0) {}

    u8 kind : 4;
    u8 reserved : 4;
    u8 payload[3];
    u32 extra;
};

EC SECTION(".text.g1_0809E994") void ClearNpcEventSlot(G1NpcEventSlot * slots, u32 index)
{
    reinterpret_cast<G1EmptyNpcEventSlot *>(slots)[index] = G1EmptyNpcEventSlot();
}
