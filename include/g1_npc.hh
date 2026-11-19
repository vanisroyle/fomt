#ifndef G1_NPC_HH
#define G1_NPC_HH

#include "prelude.h"

struct G1RosterFlags
{
    u8 prefix[3];
    u8 state : 2;
    u8 selection : 3;
    u8 flags : 3;
};

struct G1NpcEventState
{
    u8 npc[0x14];
    u32 event_id : 4;
    u32 remaining_days : 3;
    u32 new_event : 1;
    u32 location_flag : 1;
};

struct G1NpcMilestoneState
{
    u8 npc[0x14];
    u32 event_id : 6;
    u32 remaining_days : 3;
};

struct G1NamedNpcState
{
    u8 npc_and_name[0x26];
    u32 flag : 1;
};

#endif
