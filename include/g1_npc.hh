#ifndef G1_NPC_HH
#define G1_NPC_HH

#include "prelude.h"
#include "npc.hh"

struct G1RosterFlags
{
    u8 prefix[3];
    u8 state : 2;
    u8 selection : 3;
    u8 flags : 3;
};

struct G1ConstructionFlags
{
    u32 event_id : 4;
    u32 remaining_days : 3;
    bool new_event : 1;
    bool location_flag : 1;
};

struct G1NpcEventState : public Npc
{
    G1NpcEventState(ActorLocation const & location);

    union
    {
        G1ConstructionFlags bits;
        u16 event_flags;
        u8 event_bytes[2];
    };
};

struct G1MilestoneFlags
{
    u32 event_id : 6;
    u32 remaining_days : 3;
    bool completed_first : 1;
    bool completed_second : 1;
    bool completed_third : 1;
    bool completed_fourth : 1;
    bool completed_fifth : 1;
    bool completed_sixth : 1;
};

struct G1NpcMilestoneState : public Npc
{
    G1NpcMilestoneState(ActorLocation const & location);

    union
    {
        G1MilestoneFlags bits;
        u16 event_flags;
        u8 event_bytes[2];
    };
};

struct G1NpcInteractionState : public Npc
{
    G1NpcInteractionState(ActorLocation const & location);

    u16 completed_interactions;
    u8 available_interactions;
};

struct G1NpcEventSlot
{
    u32 kind : 4;
    u32 : 4;
    u32 value : 16;
    u32 : 8;
    u32 extra;
};

struct PACKED G1NamedNpcVariant
{
    u32 kind : 2;
    u32 counter : 5;
    u32 : 1;
};

struct G1NamedNpcState : public Npc
{
    G1NamedNpcState();
    G1NamedNpcState(G1NamedNpcVariant const & variant, ActorLocation const & location);

    char name[16];
    G1NamedNpcVariant variant_flags;
    u8 age_days;
    u32 flag : 1;
};

#endif
