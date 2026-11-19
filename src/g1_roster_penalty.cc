#include "g1_roster.hh"

EC Npc const * GetRosterNpcByIdConst(G1NpcRoster const *, u32);
EC Bachelorette const * GetRosterBacheloretteByIdConst(G1NpcRoster const *, u32);

EC SECTION(".text.g1_080A1484") void ApplyRosterLitteringPenalty(G1NpcRoster * self, Location const * location, G1NpcPenaltyState const * state)
{
    switch (location->map)
    {
    case 0: case 1: case 3: case 4: case 5: case 6: case 7: case 8:
        for (u32 id = 1; id <= 42; id++)
        {
            u32 available = 1;
            switch (id)
            {
            case 34:
            {
                u32 stage = state->npc_34_stage;
                if (stage != 2) available = 0;
                break;
            }
            case 33:
            {
                u32 stage = state->npc_33_stage;
                if (stage != 2) available = 0;
                break;
            }
            case 32: available = 0; break;
            }
            if (available)
            {
                Npc * npc = const_cast<Npc *>(GetRosterNpcByIdConst(self, id));
                if (npc != nullptr)
                    npc->SubtractFriendship(1);
            }
            switch (id)
            {
            case 3: case 12: case 19: case 21: case 25: case 31:
                const_cast<Bachelorette *>(GetRosterBacheloretteByIdConst(self, id))->SubtractLove(200);
                break;
            }
        }
        break;
    case 10: case 11:
        self->npc_01.SubtractFriendship(5);
        self->npc_02.SubtractFriendship(5);
        self->npc_03.SubtractFriendship(5);
        self->npc_03.SubtractLove(500);
        break;
    case 12: self->npc_22.SubtractFriendship(5); break;
    case 13: self->npc_28.SubtractFriendship(5); break;
    case 14:
        self->npc_15.SubtractFriendship(5);
        self->npc_16.SubtractFriendship(5);
        self->npc_21.SubtractFriendship(5);
        self->npc_21.SubtractLove(500);
        break;
    case 15: case 16:
        self->npc_17.SubtractFriendship(5);
        self->npc_18.SubtractFriendship(5);
        self->npc_19.SubtractFriendship(5);
        self->npc_19.SubtractLove(500);
        break;
    case 19: case 20:
        self->npc_20.SubtractFriendship(5);
        self->npc_21.SubtractFriendship(5);
        self->npc_21.SubtractLove(500);
        break;
    case 21: case 22: case 23:
        self->npc_24.SubtractFriendship(5);
        self->npc_25.SubtractFriendship(5);
        self->npc_25.SubtractLove(500);
        break;
    case 24: case 25: case 26: case 27:
        self->npc_10.SubtractFriendship(5);
        self->npc_11.SubtractFriendship(5);
        self->npc_12.SubtractFriendship(5);
        self->npc_12.SubtractLove(500);
        break;
    case 28:
        self->sprites[0].SubtractFriendship(5);
        self->sprites[1].SubtractFriendship(5);
        self->sprites[2].SubtractFriendship(5);
        self->sprites[3].SubtractFriendship(5);
        self->sprites[4].SubtractFriendship(5);
        self->sprites[5].SubtractFriendship(5);
        self->sprites[6].SubtractFriendship(5);
        break;
    case 30:
        self->npc_13.SubtractFriendship(5);
        self->npc_14.SubtractFriendship(5);
        break;
    case 31: case 32: case 33: case 34:
        self->npc_08.SubtractFriendship(5);
        self->npc_09.SubtractFriendship(5);
        break;
    case 36: self->npc_26.SubtractFriendship(5); break;
    case 38:
        self->npc_06.SubtractFriendship(5);
        self->npc_07.SubtractFriendship(5);
        break;
    case 40: self->npc_27.SubtractFriendship(5); break;
    case 41: case 42:
        self->npc_04.SubtractFriendship(5);
        self->npc_05.SubtractFriendship(5);
        break;
    }
}
