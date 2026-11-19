#include "prelude.h"

struct ChickenFeedingScene
{
    STRUCT_PAD(0, 0x92C);
    u8 success;
};

EC void func_08008724(ChickenFeedingScene & scene);

EC SECTION(".text.080AE4D8") u32 GetChickenFeedingResult(ChickenFeedingScene const & scene)
{
    return scene.success;
}

EC SECTION(".text.080AE6C0") void BlankChickenFeedingDisplay(ChickenFeedingScene & scene)
{
    func_08008724(scene);
}
