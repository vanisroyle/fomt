#include "g1_roster.hh"

EC char const gUnk_08104108[];
struct G1NpcNameEntry
{
    char const * name;
    u32 unknown;
};
EC G1NpcNameEntry const gUnk_08104258[];
EC char const * GetNamedNpcName(G1NamedNpcState const *);
EC Npc const * GetActiveScheduleNpc(G1NpcSchedule const *);
EC void * func_080E3E9C(void *, void *, G1NpcEventSlot const *);
EC u32 GetFriendship__C3Npc(Npc const *);

struct G1NpcEventQueue
{
    G1NpcEventSlot slots[7];
    u32 sentinel[2];
};

EC SECTION(".text.g1_0809E968") bool HasNpcEventSlot(G1NpcEventQueue * self, G1NpcEventSlot const * slot)
{
    void * end = self->sentinel;
    return func_080E3E9C(self, end, slot) != end;
}

EC SECTION(".text.g1_0809FE3C") char const * GetRosterNpcName(G1NpcRoster const * self, u32 id)
{
    bool valid = false;
    if (id <= 42)
        valid = true;
    if (!valid)
        return gUnk_08104108;
    switch (id)
    {
    case 0: break;
    case 35: return GetNamedNpcName(&self->schedule.active_npc);
    }
    return gUnk_08104258[id].name;
}

EC SECTION(".text.g1_080A0518") bool HasMaximumTownFriendships(G1NpcRoster * self)
{
    bool complete = true;
    for (u32 id = 1; id <= 42; id++)
    {
        switch (id)
        {
        case 23: case 26: case 30: case 32: case 35: break;
        default:
            Npc * npc = GetRosterNpcById(self, id);
            if (npc != nullptr && GetFriendship__C3Npc(npc) <= 249)
                complete = false;
            break;
        }
    }
    return complete;
}

EC SECTION(".text.g1_080A0AD8") G1NpcEventSlot * GetNpcScheduleDaySlot(G1NpcSchedule * self, u32 index)
{
    G1NpcEventSlot * slot;
    switch (index)
    {
    default: case 0: slot = &self->day_slots[0]; break;
    case 1: slot = &self->day_slots[1]; break;
    case 2: slot = &self->day_slots[2]; break;
    }
    return slot;
}

EC SECTION(".text.g1_080A06B0") Npc const * GetRosterNpcByIdConst(G1NpcRoster const * self, u32 id)
{
    switch (id)
    {
    default: return nullptr;
    case 35: return GetActiveScheduleNpc(&self->schedule);
    case 1: return &self->npc_01;
    case 2: return &self->npc_02;
    case 3: return &self->npc_03;
    case 4: return &self->npc_04;
    case 5: return &self->npc_05;
    case 6: return &self->npc_06;
    case 7: return &self->npc_07;
    case 8: return &self->npc_08;
    case 9: return &self->npc_09;
    case 10: return &self->npc_10;
    case 11: return &self->npc_11;
    case 12: return &self->npc_12;
    case 13: return &self->npc_13;
    case 14: return &self->npc_14;
    case 15: return &self->npc_15;
    case 16: return &self->npc_16;
    case 17: return &self->npc_17;
    case 18: return &self->npc_18;
    case 19: return &self->npc_19;
    case 20: return &self->npc_20;
    case 21: return &self->npc_21;
    case 22: return &self->npc_22;
    case 23: return &self->npc_23;
    case 24: return &self->npc_24;
    case 25: return &self->npc_25;
    case 26: return &self->npc_26;
    case 27: return &self->npc_27;
    case 28: return &self->npc_28;
    case 29: return &self->npc_29;
    case 30: return &self->npc_30;
    case 31: return &self->npc_31;
    case 32: return &self->npc_32;
    case 33: return &self->npc_33;
    case 34: return &self->npc_34;
    case 36: return &self->sprites[0];
    case 37: return &self->sprites[1];
    case 38: return &self->sprites[2];
    case 39: return &self->sprites[3];
    case 40: return &self->sprites[4];
    case 41: return &self->sprites[5];
    case 42: return &self->sprites[6];
    }
}

EC SECTION(".text.g1_080A0AF4") G1NpcEventSlot * GetNpcScheduleNightSlot(G1NpcSchedule * self, u32 index)
{
    G1NpcEventSlot * slot;
    switch (index)
    {
    default: case 0: slot = &self->night_slots[0]; break;
    case 1: slot = &self->night_slots[1]; break;
    case 2: slot = &self->night_slots[2]; break;
    }
    return slot;
}
