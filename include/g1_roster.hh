#ifndef G1_ROSTER_HH
#define G1_ROSTER_HH

#include "g1_npc.hh"
#include "bachelorette.hh"
#include "harvest_sprite.hh"

struct G1NpcSchedule
{
    u32 counter6 : 6;
    u32 counter7 : 7;
    u32 daily_count : 8;
    bool advanced_today : 1;
    u32 : 10;
    G1NamedNpcState active_npc;
    u32 unk_2C;
    G1NpcEventSlot day_slots[3];
    G1NpcEventSlot night_slots[3];
    G1NpcEventSlot current_slot;
    G1NpcEventSlot previous_slot;
};

struct G1NpcRoster
{
    G1NpcSchedule schedule;
    Npc npc_01;
    Npc npc_02;
    Bachelorette npc_03;
    Npc npc_04;
    Npc npc_05;
    G1NpcMilestoneState npc_06;
    Npc npc_07;
    Npc npc_08;
    Npc npc_09;
    Npc npc_10;
    Npc npc_11;
    Bachelorette npc_12;
    Npc npc_13;
    Npc npc_14;
    Npc npc_15;
    Npc npc_16;
    Npc npc_17;
    Npc npc_18;
    Bachelorette npc_19;
    Npc npc_20;
    Bachelorette npc_21;
    Npc npc_22;
    Npc npc_23;
    Npc npc_24;
    Bachelorette npc_25;
    Npc npc_26;
    G1NpcEventState npc_27;
    Npc npc_28;
    Npc npc_29;
    Npc npc_30;
    Bachelorette npc_31;
    Npc npc_32;
    G1NpcInteractionState npc_33;
    Npc npc_34;
    HarvestSprite sprites[7];
};

struct G1NpcPenaltyState
{
    u8 reserved[0x3a];
    u8 : 5;
    u8 npc_33_stage : 2;
    u8 : 1;
    u8 : 3;
    u8 npc_34_stage : 2;
};

EC Npc * GetRosterNpcById(G1NpcRoster *, u32);
EC u32 GetRosterMarriageCandidate(G1NpcRoster const *);

#endif
