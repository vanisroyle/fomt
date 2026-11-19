#include "prelude.h"

struct ChickenFeedingResource
{
    u32 metadata;
    u32 handle;
};

struct ChickenFeedingScene
{
    STRUCT_PAD(0, 0x888);
    ChickenFeedingResource resources[11];
    STRUCT_PAD(0x8E0, 0x90E);
    u8 targetFeedings;
    STRUCT_PAD(0x90F, 0x92C);
    u8 success;
    STRUCT_PAD(0x92D, 0x930);
    u32 difficultyIndex;
    u32 levelIndex;
    u8 difficultyProgress;
};

EC void func_08008724(ChickenFeedingScene & scene);
EC void func_08007C28(ChickenFeedingResource & resource, u32 handle);
EC u32 func_08007B54(ChickenFeedingResource & resource, int capacity);

static inline void ResetFeedingResource(ChickenFeedingResource & resource, int capacity)
{
    func_08007C28(resource, resource.handle);
    resource.handle = func_08007B54(resource, capacity);
}

EC SECTION(".text.080AE4D8") u32 GetChickenFeedingResult(ChickenFeedingScene const & scene)
{
    return scene.success;
}

EC SECTION(".text.080AE6C0") void BlankChickenFeedingDisplay(ChickenFeedingScene & scene)
{
    func_08008724(scene);
}

EC SECTION(".text.080AE6CC") void ResetChickenFeedingResourceHandles(ChickenFeedingScene & scene)
{
    ResetFeedingResource(scene.resources[0], 3);
    ResetFeedingResource(scene.resources[1], 6);
    ResetFeedingResource(scene.resources[3], 5);
    ResetFeedingResource(scene.resources[4], 2);
    ResetFeedingResource(scene.resources[5], 4);
    ResetFeedingResource(scene.resources[6], 5);
    ResetFeedingResource(scene.resources[7], 5);
    ResetFeedingResource(scene.resources[8], 5);
    ResetFeedingResource(scene.resources[9], 6);
    ResetFeedingResource(scene.resources[10], 6);
}

struct ChickenFeedingLevelTable
{
    u32 levels[7];
};

EC ChickenFeedingLevelTable const gUnk_0810756C;

EC SECTION(".text.080B09B0") void SetChickenFeedingTargets(ChickenFeedingScene & scene)
{
    ChickenFeedingLevelTable table = gUnk_0810756C;
    scene.levelIndex = table.levels[scene.difficultyIndex];
    unsigned progress = scene.difficultyProgress;
    if (progress <= 2)
        scene.targetFeedings = 10;
    else if (u8(progress - 3) <= 2)
        scene.targetFeedings = 15;
    else if (u8(progress - 6) <= 2)
        scene.targetFeedings = 20;
    else if (u8(progress - 9) <= 1)
        scene.targetFeedings = 30;
    else if (u8(progress - 11) <= 2)
        scene.targetFeedings = 35;
    else if (u8(progress - 14) <= 2)
        scene.targetFeedings = 45;
    else if (u8(progress - 17) <= 2)
        scene.targetFeedings = 55;
    else
        scene.targetFeedings = 60;
}
