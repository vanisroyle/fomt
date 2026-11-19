#include "g1_roster.hh"
#include "unknown_types.hh"
#include <string.h>
#include <stdlib.h>

EC MapData * GetMapData(u32);
EC void Unpack(void const *, void *);
EC u32 GetMapAvailabilityBitIndex(void *, u32, u32);
EC void func_080A5760(void *);
EC void DayUpdate__3NpcUi(void *, u32);

EC SECTION(".text.g1_0809EACC") char * GetNamedNpcName(u8 * self) // 0x0809EACC
{
    return (char *) self + 0x14;
}

EC SECTION(".text.g1_0809EAD0") u32 GetNamedNpcVariant(u8 * self) // 0x0809EAD0
{
    return self[0x24];
}

EC SECTION(".text.g1_0809EAD8") u32 GetNamedNpcAgeDays(u8 * self) // 0x0809EAD8
{
    return self[0x25];
}

EC SECTION(".text.g1_0809EAE0") u32 HasNamedNpcFlag(G1NamedNpcState * self) // 0x0809EAE0
{
    return self->flag;
}

EC SECTION(".text.g1_0809EAEC") void SetNamedNpcFlag(u8 * self) // 0x0809EAEC
{
    self[0x26] |= 1;
}

EC SECTION(".text.g1_0809EB20") void SetNamedNpcName(u8 * self, char const * name) // 0x0809EB20
{
    char * destination = (char *) self + 0x14;
    u32 length = strlen(name);
    if (length > 12) length = 12;
    memcpy(destination, name, length);
    destination[length] = 0;
}

EC SECTION(".text.g1_0809EB68") u32 GetConstructionJob(G1NpcEventState * self) // 0x0809EB68
{
    return self->bits.event_id;
}

EC SECTION(".text.g1_0809EBD4") void ClearNpcTimedEvent(G1NpcEventState * self) // 0x0809EBD4
{
    self->bits.event_id = 0;
    self->bits.location_flag = 0;
}

EC SECTION(".text.g1_0809ECD8") u32 GetNpcMilestoneEventId(G1NpcMilestoneState * self) // 0x0809ECD8
{
    return self->bits.event_id;
}

EC SECTION(".text.g1_080A0384") u8 * GetActiveRosterNpc(u8 * self) // 0x080A0384
{
    i32 active = self[3] << 30;
    u8 * npc = nullptr;
    if (active < 0) npc = self + 4;
    return npc;
}

EC SECTION(".text.g1_080A039C") u32 GetRosterSelection(G1RosterFlags * self) // 0x080A039C
{
    return self->selection;
}

EC SECTION(".text.g1_080A03A4") void SetRosterSelection(G1RosterFlags * self, u32 selection) // 0x080A03A4
{
    self->selection = selection;
}

EC SECTION(".text.g1_080A0504") bool HasRosterEntry(G1NpcRoster const * self) // 0x080A0504
{
    return GetRosterMarriageCandidate(self) != 0;
}

EC SECTION(".text.g1_080A0A04") u8 * GetActiveScheduleNpc(u8 * self) // 0x080A0A04
{
    i32 active = self[3] << 30;
    u8 * npc = nullptr;
    if (active < 0) npc = self + 4;
    return npc;
}

EC SECTION(".text.g1_080A0B10") u8 * GetNpcScheduleCurrentSlot(u8 * self) // 0x080A0B10
{
    return self + 0x60;
}

EC SECTION(".text.g1_080A0B14") u8 * GetNpcSchedulePreviousSlot(u8 * self) // 0x080A0B14
{
    return self + 0x68;
}

EC SECTION(".text.g1_080A1478") void SetNpcScheduleContext(u32 * self, u32 const * context) // 0x080A1478
{
    self[0x2c / 4] = *context;
}

EC SECTION(".text.g1_080A1480") u8 * GetNpcScheduleContext(u8 * self) // 0x080A1480
{
    return self + 0x2c;
}

EC SECTION(".text.g1_080A1A48") void NpcScheduleNoOp(void * self) // 0x080A1A48
{
}

EC SECTION(".text.g1_080A5A9C") bool IsMapTransitionIdle(u8 * self) // 0x080A5A9C
{
    bool idle = false;
    if (*(u32 *)(self + 0x8c) == 0) idle = true;
    return idle;
}

EC SECTION(".text.g1_080A601C") void StartMapTransitionDelay(u8 * self) // 0x080A601C
{
    self[0x58] = 20;
}

EC SECTION(".text.g1_0809EAF8") void UpdateNamedNpcDay(u8 * self) // 0x0809EAF8
{
    DayUpdate__3NpcUi(self, rand() % 100);
    if (self[0x25] < 255) self[0x25]++;
}

EC SECTION(".text.g1_080A5EA0") void LoadMapBackgroundTiles(u32 * self) // 0x080A5EA0
{
    MapData * map = GetMapData(self[1]);
    Unpack(map->packed_img, (void *) 0x06000000);
}
EC SECTION(".text.g1_080A1ED4") u32 GetMapStateBit(u8 * self, u32 index, u32 category) // 0x080A1ED4
{
    u32 bit = GetMapAvailabilityBitIndex(self, index, category);
    return (self[bit >> 3] >> (bit & 7)) & 1;
}

EC SECTION(".text.g1_080A6640") void ResetMapTransitionState(u8 * self) // 0x080A6640
{
    func_080A5760(self);
    u8 * first = self + 0xb4;
    first[0] = 0;
    first[1] = 0;
    u8 * second = self + 0xb8;
    second[0] = 0;
    second[1] = 0;
}
