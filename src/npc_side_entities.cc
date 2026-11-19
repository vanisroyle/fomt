#include "entity_actor.hh"

#define SIDE_CODE(address) SECTION(".text.npc_side." #address)

struct SpriteWorkerEntity : AActorEntity
{
    SpriteWorkerEntity();
    u8 sprite_id;
    STRUCT_PAD(0x31, 0x41);
    u8 work_pause;
};

EC u32 func_080AB788(u32 limit);
EC u16 const gUnk_080F1450[];

EC bool SideEntityNeverBlocks() SIDE_CODE(080324B8);
EC bool SideEntityNeverBlocks()
{
    return false;
}

EC u32 ClassifySideEntityFarmRegion(u32 map, i32 x, i32 y) SIDE_CODE(08032900);
EC u32 ClassifySideEntityFarmRegion(u32 map, i32 x, i32 y)
{
    if (map == 2)
    {
        if (u32(x - 272) <= 687 && y > 175 && y <= 591)
            return 1;
        return 2;
    }
    return 0;
}

EC void UpdateSideEntitySprite(AActorEntity * self) SIDE_CODE(08032A30);
EC void UpdateSideEntitySprite(AActorEntity * self)
{
    if (self->unk_24 != 0)
        self->unk_24--;
    else
        self->unk_24 = self->unk_26;
    if (self->unk_10.Get() != nullptr)
        self->unk_10->vfunc_0C();
}

EC u8 IsSideSpriteVisible(UnknownEntityThing * self) SIDE_CODE(08032A84);
EC u8 IsSideSpriteVisible(UnknownEntityThing * self)
{
    return self->unk_44;
}

EC void SetSideSpriteOverlayMode(UnknownEntityThing * self, u8 mode) SIDE_CODE(08032A8C);
EC void SetSideSpriteOverlayMode(UnknownEntityThing * self, u8 mode)
{
    reinterpret_cast<u8 *>(self)[0x8B] = mode;
}

EC void SetSideSpriteOverlayOffset(UnknownEntityThing * self, u8 offset) SIDE_CODE(08032A94);
EC void SetSideSpriteOverlayOffset(UnknownEntityThing * self, u8 offset)
{
    reinterpret_cast<u8 *>(self)[0x89] = offset;
}

EC void SetSideSpriteOverlayVisible(UnknownEntityThing * self, u8 visible) SIDE_CODE(08032A9C);
EC void SetSideSpriteOverlayVisible(UnknownEntityThing * self, u8 visible)
{
    self->unk_88 = visible;
}

EC void HideSideSpriteOverlay(UnknownEntityThing * self) SIDE_CODE(08032AA4);
EC void HideSideSpriteOverlay(UnknownEntityThing * self)
{
    self->unk_8A_0 = 0;
}

EC bool RollSpriteWorkChance(u32 bias) SIDE_CODE(080338F0);
EC bool RollSpriteWorkChance(u32 bias)
{
    return func_080AB788(512) < bias + 256;
}

EC void ClearSpriteWorkPause(SpriteWorkerEntity * self) SIDE_CODE(08033B7C);
EC void ClearSpriteWorkPause(SpriteWorkerEntity * self)
{
    self->work_pause = 0;
}

EC u32 GetSpriteTaskAnimation(SpriteWorkerEntity * self, u32 task) SIDE_CODE(08034248);
EC u32 GetSpriteTaskAnimation(SpriteWorkerEntity * self, u32 task)
{
    return gUnk_080F1450[self->sprite_id * 5 + task];
}
