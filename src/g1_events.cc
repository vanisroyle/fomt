#include "g1_npc.hh"
#include <stdlib.h>

EC SECTION(".text.g1_0809EB8C") u32 IsFirstConstructionDay(G1NpcEventState const * self) // 0x0809EB8C
{
    u32 first_day = 0;
    if ((u32)(self->event_bytes[0] << 28) != 0 && (self->event_bytes[0] & 0x70) != 0)
        first_day = self->bits.new_event;
    return first_day;
}

EC SECTION(".text.g1_0809EBA8") void StartNpcConstruction(G1NpcEventState * self, u32 job) // 0x0809EBA8
{
    self->bits.event_id = job;
    self->bits.remaining_days = 3;
    self->bits.new_event = true;
    self->bits.location_flag = false;
}

EC SECTION(".text.g1_0809EBEC") void UpdateNpcConstructionDay(G1NpcEventState * self) // 0x0809EBEC
{
    self->DayUpdate(rand() % 100);
    if (self->bits.new_event)
        self->bits.new_event = false;
    else if (self->bits.location_flag)
    {
        self->bits.location_flag = false;
        if (self->bits.remaining_days != 0)
            self->bits.remaining_days--;
    }
}

EC SECTION(".text.g1_0809EC48") void UpdateNpcConstructionLocation(G1NpcEventState * self) // 0x0809EC48
{
    int map = self->GetLocation().GetMap();
    switch (map)
    {
    default:
        self->bits.location_flag = false;
        break;
    case 7:
    case 17:
    case 29:
    case 37:
        self->bits.location_flag = true;
        break;
    }
}

EC SECTION(".text.g1_0809ECE0") u32 IsNpcMilestoneEventReady(G1NpcMilestoneState const * self) // 0x0809ECE0
{
    u32 ready = false;
    if ((u32)(self->event_bytes[0] << 26) != 0 && (self->event_flags & 0x1c0) == 0)
        ready = true;
    return ready;
}

EC SECTION(".text.g1_0809ED04") bool HasCompletedAllNpcMilestones(G1NpcMilestoneState const * self) // 0x0809ED04
{
    u8 count = 0;
    if ((u8)(self->event_bytes[1] & 2)) count++;
    if (self->event_bytes[1] & 4) count++;
    if (self->event_bytes[1] & 8) count++;
    if (self->event_bytes[1] & 16) count++;
    if (self->event_bytes[1] & 32) count++;
    if (self->event_bytes[1] & 64) count++;
    switch (count)
    {
    case 6: return true;
    default: return false;
    }
}

EC SECTION(".text.g1_0809ED64") void CompleteNpcMilestoneEvent(G1NpcMilestoneState * self) // 0x0809ED64
{
    switch (self->bits.event_id)
    {
    case 4: self->bits.completed_first = true; break;
    case 9: self->bits.completed_second = true; break;
    case 14: self->bits.completed_third = true; break;
    case 19: self->bits.completed_fourth = true; break;
    case 24: self->bits.completed_fifth = true; break;
    case 29: self->bits.completed_sixth = true; break;
    }
    self->bits.event_id = 0;
}

EC SECTION(".text.g1_0809EE20") void StartNpcMilestoneEvent(G1NpcMilestoneState * self, u32 event, u32 days) // 0x0809EE20
{
    self->bits.event_id = event;
    self->bits.remaining_days = days;
}

EC SECTION(".text.g1_0809EE4C") void UpdateNpcMilestoneDay(G1NpcMilestoneState * self) // 0x0809EE4C
{
    self->DayUpdate(rand() % 100);
    if (self->bits.remaining_days != 0)
        self->bits.remaining_days--;
}

EC SECTION(".text.g1_0809EEA4") bool IsNpcInteractionAvailable(G1NpcInteractionState const * self, u32 index) // 0x0809EEA4
{
    u32 mask = 1 << index;
    if (index < self->available_interactions && !(self->completed_interactions & mask))
        return true;
    return false;
}

EC SECTION(".text.g1_0809EED0") bool AdvanceNpcInteractionCount(G1NpcInteractionState * self) // 0x0809EED0
{
    if (self->available_interactions != 10)
    {
        self->available_interactions++;
        return true;
    }
    return false;
}

EC SECTION(".text.g1_0809E8E4") bool NpcEventSlotsEqual(G1NpcEventSlot const * left, G1NpcEventSlot const * right) // 0x0809E8E4
{
    u32 kind = left->kind;
    if (kind == right->kind)
    {
        switch (kind)
        {
        case 0: return true;
        case 1:
        case 2: return left->value == right->value;
        case 3:
        case 4:
        case 6:
        case 7:
        case 8: return ((u8 const *) left)[1] == ((u8 const *) right)[1];
        }
    }
    return false;
}
