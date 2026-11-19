#include "g1_npc.hh"

EC SECTION(".text.g1_0809EB70") bool IsConstructionComplete(G1NpcEventState const * self)
{
    bool complete = false;
    if (self->bits.event_id != 0 && (self->event_bytes[0] & 0x70) == 0)
        complete = true;
    return complete;
}
