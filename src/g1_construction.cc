#include "g1_npc.hh"
#include <string.h>

SECTION(".text.g1_0809EB4C") G1NpcEventState::G1NpcEventState(ActorLocation const & location)
    : Npc(location)
{
    event_bytes[0] = 0;
    bits.location_flag = false;
}

SECTION(".text.g1_0809EC90") G1NpcMilestoneState::G1NpcMilestoneState(ActorLocation const & location)
    : Npc(location)
{
    bits.event_id = 0;
    bits.remaining_days = 0;
    bits.completed_first = false;
    bits.completed_second = false;
    bits.completed_third = false;
    bits.completed_fourth = false;
    bits.completed_fifth = false;
    bits.completed_sixth = false;
}

SECTION(".text.g1_0809EE8C") G1NpcInteractionState::G1NpcInteractionState(ActorLocation const & location)
    : Npc(location), completed_interactions(0), available_interactions(0)
{
}

EC SECTION(".text.g1_0809E988") void SetNpcEventSlot(G1NpcEventSlot * slots, u32 index, G1NpcEventSlot const * value)
{
    memcpy(slots + index, value, sizeof(*value));
}
