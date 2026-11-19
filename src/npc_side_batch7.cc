#include "entity_actor.hh"
#include "g1_npc.hh"
#include "harvest_sprite.hh"

#define NPC_SIDE_CODE(address) SECTION(".text.npc_side." #address)
#define NPC_EXTRA_CODE(address) SECTION(".text.npc_extra." #address)

struct SpriteWorkerEntity : AActorEntity
{
    SpriteWorkerEntity();
    u8 sprite_id;
    STRUCT_PAD(0x31, 0x34);
    HarvestSprite * record;
    G1NpcEventSlot * work_targets;
    void * work_context;
    u8 work_state;
    u8 work_pause;
    u8 work_active;
};

struct NpcSideWorld
{
    STRUCT_PAD(0x00, 0x1CD4);
    u8 roster[0x497];
    u8 marriage_flags_first;
    u8 marriage_flags_second;
};

struct WorldScheduledNpc : AActorEntity
{
    WorldScheduledNpc();
    Npc * record;
    NpcSideWorld * world;
};

struct NpcSideServices;
struct NpcClock
{
    STRUCT_PAD(0x00, 0x0A);
    GameTime time;
};

struct NpcSideServicesVTable
{
    STRUCT_PAD(0x00, 0x144);
    NpcClock * (*get_clock)(NpcSideServices *);
};

struct NpcSideServices
{
    NpcSideServicesVTable * vtable;
};

struct TransitionSideSprite;
struct SideEntityEffects
{
    STRUCT_PAD(0x00, 0x0C);
    TransitionSideSprite * transition_sprite;
    void * particles;
};

EC u32 RollEntityMovementRandom(u32 limit);
EC bool RollSpriteWorkChance(u32 friendship);
EC u32 GetSpriteWorkerFriendship(Npc const * self) asm("GetFriendship__C3Npc");
EC bool func_08034260(SpriteWorkerEntity * self);
EC void func_08034180(AActorEntity * self, u32 animation, u32 facing, u32 speed);
EC void SetNpcWalkingAnimation(AActorEntity * self, u32 animation);
EC void SetNpcIdleAnimation(AActorEntity * self, u32 animation);
EC u8 * GetActiveRosterNpc(u8 * roster);
EC u32 GetNamedNpcAgeDays(u8 * npc);
EC bool IsNpcAtNorthernExit(AEntity * self);
EC bool HasNamedNpcFlag(G1NamedNpcState * npc);
EC u32 GetRosterMarriageCandidate(void const * roster);
EC bool IsFamilyScheduleAwayFromHome(NpcSideWorld * world, GameTime const & time);

EC u32 ComputeSpriteWorkSpeed(u32 task_experience) NPC_SIDE_CODE(08033914);
EC u32 ComputeSpriteWorkSpeed(u32 task_experience)
{
    u32 scale = 0x18000;
    task_experience++;
    task_experience *= scale;
    task_experience >>= 8;
    task_experience += 0x8000;
    return task_experience;
}

EC void ChooseSpriteWorkerNextTask(SpriteWorkerEntity * self) NPC_SIDE_CODE(080342D8);
EC void ChooseSpriteWorkerNextTask(SpriteWorkerEntity * self)
{
    if (!RollSpriteWorkChance(GetSpriteWorkerFriendship(self->record)) || !func_08034260(self))
    {
        struct Timer { u32 duration : 16; u32 animation : 8; u32 facing : 8; } timer;
        timer.duration = RollEntityMovementRandom(180);
        timer.animation = 0;
        HarvestSprite * record = self->record;
        HarvestSprite::Task task = record->GetCurrentTask();
        u32 experience = record->GetTaskExp(task);
        timer.facing = (RollEntityMovementRandom(256 - experience) >> 6) + 2;
        self->record->method_0809E7D8(reinterpret_cast<UnkBarnAnimal2C const *>(&timer));
        func_08034180(self, 0, self->facing, 0);
    }
}

EC u32 HasSideEffectParticles(SideEntityEffects * self) NPC_EXTRA_CODE(080390D0);
EC u32 HasSideEffectParticles(SideEntityEffects * self)
{
    u32 pointer = reinterpret_cast<u32>(self->particles);
    return (0u - pointer | pointer) >> 31;
}

EC void RefreshArrivalReactionAnimations(WorldScheduledNpc * self) NPC_EXTRA_CODE(08036E70);
EC void RefreshArrivalReactionAnimations(WorldScheduledNpc * self)
{
    u8 * npc;
    {
        u8 * active = GetActiveRosterNpc(self->world->roster);
        npc = active;
    }
    if (npc != nullptr)
    {
        u32 animation = 615;
        u32 idle_animation = 623;
        if (GetNamedNpcAgeDays(npc) <= 59)
        {
            if (IsNpcAtNorthernExit(self))
                animation = 12;
        }
        else if (IsNpcAtNorthernExit(self))
            animation = 631;
        else if (!HasNamedNpcFlag(reinterpret_cast<G1NamedNpcState *>(npc)))
        {
            animation = 619;
            idle_animation = 627;
        }
        SetNpcWalkingAnimation(self, animation);
        SetNpcIdleAnimation(self, idle_animation);
    }
}

EC void RefreshScheduledNpcOneAnimations(WorldScheduledNpc * self) NPC_EXTRA_CODE(08035C04);
EC void RefreshScheduledNpcOneAnimations(WorldScheduledNpc * self)
{
    u32 walking_animation = 559;
    u32 idle_animation = 563;
    NpcSideWorld * world = self->world;
    u8 * roster = world->roster;
    if (GetRosterMarriageCandidate(roster) == 3 && self->location_map == 29)
    {
        if (IsNpcAtNorthernExit(self) || ((u32(world->marriage_flags_first) << 25) >> 30) == 1)
            walking_animation = 575;
        else
        {
            u8 * npc = GetActiveRosterNpc(roster);
            if (npc != nullptr && GetNamedNpcAgeDays(npc) <= 59)
            {
                NpcSideServices * game = reinterpret_cast<NpcSideServices *>(self->game_object);
                NpcClock * clock = game->vtable->get_clock(game);
                if (IsFamilyScheduleAwayFromHome(world, clock->time))
                {
                    walking_animation = 583;
                    idle_animation = 587;
                }
            }
        }
    }
    SetNpcWalkingAnimation(self, walking_animation);
    SetNpcIdleAnimation(self, idle_animation);
}

EC void RefreshScheduledNpcTwoAnimations(WorldScheduledNpc * self) NPC_EXTRA_CODE(08036084);
EC void RefreshScheduledNpcTwoAnimations(WorldScheduledNpc * self)
{
    u32 walking_animation = 2067;
    u32 idle_animation = 2071;
    NpcSideWorld * world = self->world;
    u8 * roster = world->roster;
    if (GetRosterMarriageCandidate(roster) == 12 && self->location_map == 29)
    {
        if (IsNpcAtNorthernExit(self) || ((u32(world->marriage_flags_second) << 25) >> 30) == 1)
            walking_animation = 2087;
        else
        {
            u8 * npc = GetActiveRosterNpc(roster);
            if (npc != nullptr && GetNamedNpcAgeDays(npc) <= 59)
            {
                NpcSideServices * game = reinterpret_cast<NpcSideServices *>(self->game_object);
                NpcClock * clock = game->vtable->get_clock(game);
                if (IsFamilyScheduleAwayFromHome(world, clock->time))
                {
                    walking_animation = 2095;
                    idle_animation = 2099;
                }
            }
        }
    }
    SetNpcWalkingAnimation(self, walking_animation);
    SetNpcIdleAnimation(self, idle_animation);
}

EC void RefreshScheduledNpcThreeAnimations(WorldScheduledNpc * self) NPC_EXTRA_CODE(08036430);
EC void RefreshScheduledNpcThreeAnimations(WorldScheduledNpc * self)
{
    u32 walking_animation = 1681;
    u32 idle_animation = 1685;
    NpcSideWorld * world = self->world;
    u8 * roster = world->roster;
    if (GetRosterMarriageCandidate(roster) == 19 && self->location_map == 29)
    {
        if (IsNpcAtNorthernExit(self) || ((u32(world->marriage_flags_second) << 27) >> 30) == 1)
            walking_animation = 1697;
        else
        {
            u8 * npc = GetActiveRosterNpc(roster);
            if (npc != nullptr && GetNamedNpcAgeDays(npc) <= 59)
            {
                NpcSideServices * game = reinterpret_cast<NpcSideServices *>(self->game_object);
                NpcClock * clock = game->vtable->get_clock(game);
                if (IsFamilyScheduleAwayFromHome(world, clock->time))
                {
                    walking_animation = 1709;
                    idle_animation = 1713;
                }
            }
        }
    }
    SetNpcWalkingAnimation(self, walking_animation);
    SetNpcIdleAnimation(self, idle_animation);
}

EC void RefreshScheduledNpcFourAnimations(WorldScheduledNpc * self) NPC_EXTRA_CODE(080365CC);
EC void RefreshScheduledNpcFourAnimations(WorldScheduledNpc * self)
{
    u32 walking_animation = 2180;
    u32 idle_animation = 2184;
    NpcSideWorld * world = self->world;
    u8 * roster = world->roster;
    if (GetRosterMarriageCandidate(roster) == 21 && self->location_map == 29)
    {
        if (IsNpcAtNorthernExit(self) || ((u32(world->marriage_flags_second) << 29) >> 30) == 1)
            walking_animation = 2204;
        else
        {
            u8 * npc = GetActiveRosterNpc(roster);
            if (npc != nullptr && GetNamedNpcAgeDays(npc) <= 59)
            {
                NpcSideServices * game = reinterpret_cast<NpcSideServices *>(self->game_object);
                NpcClock * clock = game->vtable->get_clock(game);
                if (IsFamilyScheduleAwayFromHome(world, clock->time))
                {
                    walking_animation = 2196;
                    idle_animation = 2200;
                }
            }
        }
    }
    SetNpcWalkingAnimation(self, walking_animation);
    SetNpcIdleAnimation(self, idle_animation);
}
