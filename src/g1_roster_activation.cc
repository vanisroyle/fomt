#include "g1_roster.hh"
#include <string.h>

struct G1RosterControlFlags
{
    u8 reserved[3];
    bool first : 1;
    bool named_npc_active : 1;
};

static inline void CopyNamedNpcState(G1NamedNpcState * self, G1NamedNpcState const & source)
{
    static_cast<Npc &>(*self) = source;
    strcpy(self->name, source.name);
    self->variant_flags = source.variant_flags;
    self->age_days = source.age_days;
    self->flag = source.flag;
}

EC SECTION(".text.g1_080A05E8") void ActivateRosterNamedNpc(G1NpcRoster * self, G1NamedNpcVariant const * variant)
{
    G1RosterControlFlags * flags = reinterpret_cast<G1RosterControlFlags *>(self);
    if (!flags->named_npc_active)
    {
        flags->named_npc_active = true;
        G1NamedNpcState npc(*variant, ActorLocation(Location(29, 100, 100), 0));
        CopyNamedNpcState(&self->schedule.active_npc, npc);
    }
}
