#include "prelude.h"
#include "g5_graphics.hh"
#include "sprite_animation.hh"

EC void func_080CE120(void *, void *, u32);
EC u8 vtable_unk_080E85BC[];

EC SECTION(".text.g5_080CEC84") void ActivateInventoryCursor(u8 * self, u32 index, u32 page) // 0x080CEC84
{
    self[0x84] = 1;
    self[0x68] = 1;
    while (static_cast<i32>(AdvanceSpriteAnimator(reinterpret_cast<SpriteAnimator *>(self + 0x38))
        << (31 - SpriteAnimator::LOOP_FLAG_BIT)) >= 0) {}
    void * owner = *(void **) self;
    u32 position;
    u8 * coords = (u8 *) &position;
    func_080CE120(&position, owner, index);
    *(u16 *) (self + 0x34) = coords[0] + 8;
    *(u16 *) (self + 0x36) = coords[1];
    *(u32 *) (self + 0x7c) = index;
    *(u32 *) (self + 0x80) = page;
}

EC SECTION(".text.g5_080CD950") void func_080CD950(u8 * self, u32 flags)
{
    *(void **) (self + 0x210) = vtable_unk_080E85BC;
    u32 * size = (u32 *) (self + 0x214);
    u32 offset = *size * 12 + 4;
    u8 * end = (u8 *) size + offset;
    for (u8 * entry = self + 0x218; entry != end; entry += 12) {}
    DestroyPanelDrawCommands(self, flags);
}
