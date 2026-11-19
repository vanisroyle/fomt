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
    STRUCT_PAD(0x708, 0x814);
    u32 effectsTrack;
    STRUCT_PAD(0x818, 0x838);
    u32 phase;
    STRUCT_PAD(0x83C, 0x884);
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
EC u8 const gUnk_0872FC34[], gUnk_0872FE6C[], gUnk_0872FEF8[], gUnk_08731940[];
EC void Dma3_Fill(u32, void *, u32);
EC void Dma3_Copy(void const *, void *, u32);
EC void Unpack(void const *, void *);
EC void * HardwareContext_GetDisplayRegisters(ChickenFeedingScene *);
EC void AudioPlayer_StartSong(void *, u32);
EC void func_080AF814(ChickenFeedingScene &);
EC void func_080087C8(ChickenFeedingScene &);

struct ChickenFeedingDisplayRegisters
{
    u16 displayControl;
    STRUCT_PAD(2, 0xa);
    u16 bg1Control;
    u16 bg2Control;
};

EC SECTION(".text.080AE7D0") void LoadChickenFeedingBackground(ChickenFeedingScene & scene)
{
    register ChickenFeedingScene * state asm("sb") = &scene;
    asm ("" : "+h" (state));
    Dma3_Fill(0, reinterpret_cast<void *>(0x06007fe0), 0x20);

    register u32 emptyTile asm("r5") = ~0u;
    asm ("" : "+l" (emptyTile));
    register void * bg1Map asm("r8") = reinterpret_cast<void *>(0x0600f000);
    asm ("" : "+h" (bg1Map));
    register u32 mapBytes asm("r6") = 0x800;
    asm ("" : "+l" (mapBytes));
    Dma3_Fill(emptyTile, bg1Map, mapBytes);
    register void * bg2Map asm("r4") = reinterpret_cast<void *>(0x0600f800);
    asm ("" : "+l" (bg2Map));
    Dma3_Fill(emptyTile, bg2Map, mapBytes);

    Unpack(gUnk_0872FC34, bg2Map);
    Unpack(gUnk_0872FE6C, bg1Map);
    Unpack(gUnk_0872FEF8, reinterpret_cast<void *>(0x06000000));
    Dma3_Copy(gUnk_08731940, reinterpret_cast<void *>(0x05000000), 0x200);

    u16 control;
    register ChickenFeedingDisplayRegisters * display asm("r0");
    register u16 * temporary asm("r2");
    register u32 setting asm("r3");
    register u32 bits asm("r1");

    display = static_cast<ChickenFeedingDisplayRegisters *>(HardwareContext_GetDisplayRegisters(state));
    asm ("" : "+l" (display));
    temporary = &control;
    asm ("" : "+l" (temporary));
    setting = 0x1640;
    asm ("" : "+l" (setting));
    bits = setting;
    asm ("" : "+l" (bits));
    *temporary = bits;
    display->displayControl = bits;

    display = static_cast<ChickenFeedingDisplayRegisters *>(HardwareContext_GetDisplayRegisters(state));
    asm ("" : "+l" (display));
    temporary = &control;
    asm ("" : "+l" (temporary));
    setting = 0x1e41;
    asm ("" : "+l" (setting));
    bits = setting;
    asm ("" : "+l" (bits));
    *temporary = bits;
    display->bg1Control = bits;

    display = static_cast<ChickenFeedingDisplayRegisters *>(HardwareContext_GetDisplayRegisters(state));
    asm ("" : "+l" (display));
    temporary = &control;
    asm ("" : "+l" (temporary));
    setting = 0x1f42;
    asm ("" : "+l" (setting));
    bits = setting;
    asm ("" : "+l" (bits));
    *temporary = bits;
    display->bg2Control = bits;
}

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

EC SECTION(".text.080B1D34") void RunChickenFeedingTransition(ChickenFeedingScene & scene)
{
    register int frames asm("r6") = 0;
    asm ("" : "+l" (frames));
    register ChickenFeedingDisplayRegisters * display asm("r0") =
        static_cast<ChickenFeedingDisplayRegisters *>(HardwareContext_GetDisplayRegisters(&scene));
    asm ("" : "+l" (display));
    u16 control;
    register u16 * temporary asm("r2") = &control;
    asm ("" : "+l" (temporary));
    register u32 setting asm("r3") = 0x1640;
    asm ("" : "+l" (setting));
    register u32 bits asm("r1") = setting;
    asm ("" : "+l" (bits));
    *temporary = bits;
    display->displayControl = bits;

    func_080AF814(scene);
    AudioPlayer_StartSong(&scene.effectsTrack, 0xbb);
    register unsigned phaseOffset asm("r3") = offsetof(ChickenFeedingScene, phase);
    asm ("" : "+l" (phaseOffset));
    register u32 const * initialPhase asm("r0") =
        reinterpret_cast<u32 const *>(reinterpret_cast<u8 *>(&scene) + phaseOffset);
    asm ("" : "+l" (initialPhase));
    if (*initialPhase == 2)
    {
        register u32 * phase asm("r5") =
            reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(&scene) + phaseOffset);
        asm ("" : "+l" (phase));
        u32 nextPhase = 3;
        asm ("" : "+l" (nextPhase));
        do
        {
            func_080087C8(scene);
            func_080AF814(scene);
            if (++frames > 120)
                *phase = nextPhase;
        }
        while (*phase == 2);
    }
}
