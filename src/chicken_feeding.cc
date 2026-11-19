#include "prelude.h"

struct ChickenFeedingResource
{
    u32 metadata;
    u32 handle;
};

struct ChickenFeedingActor
{
    STRUCT_PAD(0, 0x10);
    u16 animationSpeed;
};

struct ChickenFeedingScene
{
    STRUCT_PAD(0, 0x500);
    ChickenFeedingActor * feedingSprite;
    STRUCT_PAD(0x504, 0x618);
    ChickenFeedingActor * chickens[5];
    STRUCT_PAD(0x62C, 0x6F4);
    ChickenFeedingActor * troughs[5];
    STRUCT_PAD(0x708, 0x884);
    int selectedTrough;
    ChickenFeedingResource resources[11];
    u8 spriteY;
    STRUCT_PAD(0x8E1, 0x90E);
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
EC u8 const gUnk_0810787C[5];

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

static inline void SetFeedingActorSpeeds(u8 * state, unsigned initialSpeed)
{
    register int slot asm("r2") = 0;
    asm ("" : "+l" (slot));
    register unsigned chickensOffset asm("r6") = offsetof(ChickenFeedingScene, chickens);
    asm ("" : "+l" (chickensOffset));
    register unsigned speed asm("r4") = initialSpeed;
    asm ("" : "+l" (speed));
    register unsigned troughsOffset asm("r5") = offsetof(ChickenFeedingScene, troughs);
    asm ("" : "+l" (troughsOffset));
    do
    {
        register unsigned slotOffset asm("r1") = slot * sizeof(ChickenFeedingActor *);
        asm ("" : "+l" (slotOffset));
        register u8 * chickenBase asm("r0");
        asm (".syntax unified\nadds %0, %1, %2\n.syntax divided"
            : "=l" (chickenBase) : "l" (slotOffset), "l" (state));
        (*reinterpret_cast<ChickenFeedingActor **>(chickenBase + chickensOffset))->animationSpeed = speed;
        register u8 * troughBase asm("r1");
        asm (".syntax unified\nadds %0, %1, %2\n.syntax divided"
            : "=l" (troughBase) : "l" (slotOffset), "l" (state));
        (*reinterpret_cast<ChickenFeedingActor **>(troughBase + troughsOffset))->animationSpeed = speed;
        ++slot;
        asm ("" : "+l" (slot));
    }
    while (slot < 5);
}

EC SECTION(".text.080B0934") void PauseChickenFeedingAnimations(ChickenFeedingScene & scene)
{
    register u8 * state asm("r3") = reinterpret_cast<u8 *>(&scene);
    asm ("" : "+l" (state));
    reinterpret_cast<ChickenFeedingScene *>(state)->feedingSprite->animationSpeed = 0;
    SetFeedingActorSpeeds(state, 0);
}

EC SECTION(".text.080B0970") void ResumeChickenFeedingAnimations(ChickenFeedingScene & scene)
{
    register u8 * state asm("r3") = reinterpret_cast<u8 *>(&scene);
    asm ("" : "+l" (state));
    register ChickenFeedingActor * sprite asm("r0") = reinterpret_cast<ChickenFeedingScene *>(state)->feedingSprite;
    asm ("" : "+l" (sprite));
    register unsigned speed asm("r1") = 0x100;
    asm ("" : "+l" (speed));
    sprite->animationSpeed = speed;
    SetFeedingActorSpeeds(state, speed);
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

EC SECTION(".text.080B34DC") void UpdateChickenFeedingSelectedTrough(ChickenFeedingScene & scene)
{
    for (int trough = 0; trough < 5; ++trough)
    {
        if (scene.spriteY == gUnk_0810787C[trough])
        {
            scene.selectedTrough = trough;
            return;
        }
    }
    scene.selectedTrough = 5;
}
