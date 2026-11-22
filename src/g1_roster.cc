#include "g1_roster.hh"

EC Npc * GetActiveRosterNpc(G1NpcRoster *);

EC SECTION(".text.g1_080A0030") Npc * GetRosterNpcById(G1NpcRoster * self, u32 id)
{
    switch (id)
    {
    default: return nullptr;
    case 35: return GetActiveRosterNpc(self);
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

EC SECTION(".text.g1_080A01F8") Bachelorette * GetRosterBacheloretteById(G1NpcRoster * self, u32 id)
{
    switch (id)
    {
    default: return nullptr;
    case 3: return &self->npc_03;
    case 12: return &self->npc_12;
    case 19: return &self->npc_19;
    case 21: return &self->npc_21;
    case 25: return &self->npc_25;
    case 31: return &self->npc_31;
    }
}

EC SECTION(".text.g1_080A031C") HarvestSprite * GetRosterHarvestSpriteByIndex(G1NpcRoster * self, u32 index)
{
    switch (index)
    {
    default: return nullptr;
    case 0: return &self->sprites[0];
    case 1: return &self->sprites[1];
    case 2: return &self->sprites[2];
    case 3: return &self->sprites[3];
    case 4: return &self->sprites[4];
    case 5: return &self->sprites[5];
    case 6: return &self->sprites[6];
    }
}

EC SECTION(".text.g1_080A03B8") ActorLocation GetRosterNpcLocation(G1NpcRoster * self, u32 id)
{
    Npc * npc = GetRosterNpcById(self, id);
    if (npc != nullptr)
        return npc->GetLocation();
    return ActorLocation(Location(2, 0, 0), 0);
}

EC SECTION(".text.g1_080A041C") u32 GetRosterProposalCandidate(G1NpcRoster const * self)
{
    if (self->npc_03.GetPlayerEventCount() > 4) return 3;
    if (self->npc_12.GetPlayerEventCount() > 4) return 12;
    if (self->npc_19.GetPlayerEventCount() > 4) return 19;
    if (self->npc_21.GetPlayerEventCount() > 4) return 21;
    if (self->npc_25.GetPlayerEventCount() > 4) return 25;
    if (self->npc_31.GetPlayerEventCount() > 4) return 31;
    return 0;
}

EC SECTION(".text.g1_080A0490") u32 GetRosterMarriageCandidate(G1NpcRoster const * self)
{
    if (self->npc_03.GetPlayerEventCount() > 5) return 3;
    if (self->npc_12.GetPlayerEventCount() > 5) return 12;
    if (self->npc_19.GetPlayerEventCount() > 5) return 19;
    if (self->npc_21.GetPlayerEventCount() > 5) return 21;
    if (self->npc_25.GetPlayerEventCount() > 5) return 25;
    if (self->npc_31.GetPlayerEventCount() > 5) return 31;
    return 0;
}

EC SECTION(".text.g1_080A0878") Bachelorette const * GetRosterBacheloretteByIdConst(G1NpcRoster const * self, u32 id)
{
    switch (id)
    {
    default: return nullptr;
    case 3: return &self->npc_03;
    case 12: return &self->npc_12;
    case 19: return &self->npc_19;
    case 21: return &self->npc_21;
    case 25: return &self->npc_25;
    case 31: return &self->npc_31;
    }
}

EC SECTION(".text.g1_080A0930") HarvestSprite const * GetRosterHarvestSpriteByIdConst(G1NpcRoster const * self, u32 id)
{
    switch (id)
    {
    default: return nullptr;
    case 36: return &self->sprites[0];
    case 37: return &self->sprites[1];
    case 38: return &self->sprites[2];
    case 39: return &self->sprites[3];
    case 40: return &self->sprites[4];
    case 41: return &self->sprites[5];
    case 42: return &self->sprites[6];
    }
}

EC SECTION(".text.g1_080A099C") HarvestSprite const * GetRosterHarvestSpriteByIndexConst(G1NpcRoster const * self, u32 index)
{
    switch (index)
    {
    default: return nullptr;
    case 0: return &self->sprites[0];
    case 1: return &self->sprites[1];
    case 2: return &self->sprites[2];
    case 3: return &self->sprites[3];
    case 4: return &self->sprites[4];
    case 5: return &self->sprites[5];
    case 6: return &self->sprites[6];
    }
}

template <class T> inline T const & G1ScheduleMin(T const & left, T const & right)
{
    return right < left ? right : left;
}

EC SECTION(".text.g1_080A0A1C") void AddNpcScheduleCounter6(G1NpcSchedule * self, u32 amount)
{
    self->counter6 = G1ScheduleMin(self->counter6 + amount, 63U);
}

EC SECTION(".text.g1_080A0A54") void AddNpcScheduleCounter7(G1NpcSchedule * self, u32 amount)
{
    self->counter7 = G1ScheduleMin(self->counter7 + amount, 127U);
}

EC SECTION(".text.g1_080A0A90") void AdvanceNpcScheduleDailyCounter(G1NpcSchedule * self)
{
    if (!self->advanced_today)
    {
        self->daily_count = G1ScheduleMin(self->daily_count + 1, 255U);
        self->advanced_today = true;
    }
}

EC SECTION(".text.g1_080A198C") ActorLocation GetNpcScheduleLocation41()
{
    return ActorLocation(Location(41, 184, 72), 0);
}

EC SECTION(".text.g1_080A19EC") ActorLocation GetNpcScheduleLocation23()
{
    return ActorLocation(Location(23, 240, 256), 0);
}

EC SECTION(".text.g1_080A17A0") ActorLocation GetNpcScheduleMap7Arrival(u32 index)
{
    switch (index)
    {
    default:
    case 0: return ActorLocation(Location(7, 392, 344), 0);
    case 1: return ActorLocation(Location(7, 376, 296), 0);
    case 2: return ActorLocation(Location(7, 392, 248), 0);
    }
}

EC SECTION(".text.g1_080A1890") ActorLocation GetNpcScheduleMap7Departure(u32 index)
{
    switch (index)
    {
    default:
    case 0: return ActorLocation(Location(7, 960, 408), 0);
    case 1: return ActorLocation(Location(7, 1096, 416), 0);
    case 2: return ActorLocation(Location(7, 1176, 344), 0);
    }
}
