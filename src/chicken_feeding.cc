#include "prelude.h"

#include <stdlib.h>
#include <string.h>

struct MessageWindow;

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
    STRUCT_PAD(0, 0x418);
    MessageWindow * message;
    STRUCT_PAD(0x41C, 0x500);
    ChickenFeedingActor * feedingSprite;
    STRUCT_PAD(0x504, 0x618);
    ChickenFeedingActor * chickens[5];
    STRUCT_PAD(0x62C, 0x6F4);
    ChickenFeedingActor * troughs[5];
    STRUCT_PAD(0x708, 0x810);
    u32 messageAudio;
    u32 effectsTrack;
    STRUCT_PAD(0x818, 0x81C);
    u32 feedingRewardTrack;
    STRUCT_PAD(0x820, 0x838);
    u32 phase;
    u32 inputPhase;
    u32 resultPhase;
    int movementState;
    u32 chickenPhase[5];
    u32 chickenReaction[5];
    u32 chickenEffect[5];
    int selectedTrough;
    ChickenFeedingResource resources[11];
    u8 spriteY;
    u8 chickenY[5];
    u16 chickenDelay[5];
    u16 chickenOffsets[5];
    u8 actionTimer;
    STRUCT_PAD(0x8FB, 0x8FC);
    u16 chickenTimers[5];
    u8 troughTimers[5];
    u8 roundTimer;
    u8 feedingCount;
    u8 missedFeedings;
    u8 targetFeedings;
    STRUCT_PAD(0x90F, 0x910);
    u32 score;
    u8 actionReady;
    u8 resetChickenTimer[5];
    u8 refreshChicken[5];
    u8 refreshReaction[5];
    u8 refreshEffect[5];
    u8 refreshSprite;
    u8 resultMessageReady;
    u8 resultSoundReady;
    u8 success;
    STRUCT_PAD(0x92D, 0x930);
    u32 difficultyIndex;
    u32 levelIndex;
    u8 difficultyProgress;
};

EC SECTION(".text.080B0AA0") void ResetChickenFeedingRound(ChickenFeedingScene & scene)
{
    register u8 * bytes asm("r3") = reinterpret_cast<u8 *>(&scene);
    asm ("" : "+l" (bytes));
    register unsigned movementOffset asm("r0") = offsetof(ChickenFeedingScene, movementState);
    asm ("" : "+l" (movementOffset));
    register int * movement asm("r1") = reinterpret_cast<int *>(bytes + movementOffset);
    asm ("" : "+l" (movement));
    register unsigned zeroMovement asm("r0") = 0;
    asm ("" : "+l" (zeroMovement));
    *movement = zeroMovement;
    register int index asm("r2") = 0;
    asm ("" : "+l" (index));
    register unsigned clear asm("r5") = 0;
    asm ("" : "+l" (clear));
    unsigned enabled = 1;
    asm ("" : "+l" (enabled));
    register unsigned phaseOffset asm("r1") = offsetof(ChickenFeedingScene, chickenPhase);
    asm ("" : "+l" (phaseOffset));
    register u8 * phase asm("r4") = bytes + phaseOffset;
    asm ("" : "+l" (phase));
    register unsigned yOffset asm("r6") = offsetof(ChickenFeedingScene, chickenY);
    asm ("" : "+l" (yOffset));
    register u8 * firstY asm("r6");
    asm ("add %0, %1, %2" : "=l" (firstY) : "l" (yOffset), "l" (bytes) : "cc");
    register u8 * chickenY asm("r8") = firstY;
    asm ("" : "+h" (chickenY));
    register unsigned offsetsOffset asm("r0") = offsetof(ChickenFeedingScene, chickenOffsets);
    asm ("" : "+l" (offsetsOffset));
    register u8 * firstOffset asm("r0");
    asm ("add %0, %1, %2" : "=l" (firstOffset) : "l" (offsetsOffset), "l" (bytes) : "cc");
    register u8 * chickenOffset asm("sb") = firstOffset;
    asm ("" : "+h" (chickenOffset));
    phaseOffset = phaseOffset + (offsetof(ChickenFeedingScene, chickenReaction) - offsetof(ChickenFeedingScene, chickenPhase));
    asm ("" : "+l" (phaseOffset));
    register u8 * firstReaction asm("r1");
    asm ("add %0, %1, %2" : "=l" (firstReaction) : "l" (phaseOffset), "l" (bytes) : "cc");
    register u8 * nextReaction asm("ip") = firstReaction;
    asm ("" : "+h" (nextReaction));
    register u32 flagsOffset asm("r6");
    register u32 troughOffset asm("r1");
    register u8 * target asm("r0");
    do
    {
        *reinterpret_cast<u32 *>(phase) = clear;
        *reinterpret_cast<u32 *>(phase + (offsetof(ChickenFeedingScene, chickenEffect) - offsetof(ChickenFeedingScene, chickenPhase))) = clear;
        register u8 * reaction asm("r6") = nextReaction;
        asm ("" : "+l" (reaction));
        reaction += sizeof(u32);
        asm ("" : "+l" (reaction));
        nextReaction = reaction;
        asm ("" : "+h" (nextReaction));
        reaction -= sizeof(u32);
        asm ("" : "+l" (reaction));
        asm volatile ("stmia %0!, {%1}" : "+l" (reaction) : "l" (clear) : "memory");
        register u8 * yBase asm("r0") = chickenY;
        asm ("" : "+l" (yBase));
        register u8 * y asm("r1") = yBase + index;
        asm ("" : "+l" (y));
        *y = 112;
        register u32 timerIndex asm("r1") = index * sizeof(u16);
        asm ("" : "+l" (timerIndex));
        register u8 * offset asm("r6") = chickenOffset;
        asm ("" : "+l" (offset));
        *reinterpret_cast<u16 *>(offset) = clear;
        flagsOffset = offsetof(ChickenFeedingScene, resetChickenTimer);
        asm ("" : "+l" (flagsOffset));
        target = bytes + flagsOffset;
        asm ("" : "+l" (target));
        target += index;
        asm ("" : "+l" (target));
        *reinterpret_cast<u8 *>(target) = enabled;
        flagsOffset = flagsOffset - (offsetof(ChickenFeedingScene, resetChickenTimer) - offsetof(ChickenFeedingScene, chickenTimers));
        asm ("" : "+l" (flagsOffset));
        target = bytes + flagsOffset;
        asm ("" : "+l" (target));
        target += timerIndex;
        asm ("" : "+l" (target));
        *reinterpret_cast<u16 *>(target) = clear;
        troughOffset = offsetof(ChickenFeedingScene, troughTimers);
        asm ("" : "+l" (troughOffset));
        target = bytes + troughOffset;
        asm ("" : "+l" (target));
        target += index;
        asm ("" : "+l" (target));
        *reinterpret_cast<u8 *>(target) = clear;
        flagsOffset = flagsOffset + (offsetof(ChickenFeedingScene, refreshChicken) - offsetof(ChickenFeedingScene, chickenTimers));
        asm ("" : "+l" (flagsOffset));
        target = bytes + flagsOffset;
        asm ("" : "+l" (target));
        target += index;
        asm ("" : "+l" (target));
        *reinterpret_cast<u8 *>(target) = enabled;
        troughOffset = troughOffset + (offsetof(ChickenFeedingScene, refreshReaction) - offsetof(ChickenFeedingScene, troughTimers));
        asm ("" : "+l" (troughOffset));
        target = bytes + troughOffset;
        asm ("" : "+l" (target));
        target += index;
        asm ("" : "+l" (target));
        *reinterpret_cast<u8 *>(target) = enabled;
        flagsOffset = flagsOffset + (offsetof(ChickenFeedingScene, refreshEffect) - offsetof(ChickenFeedingScene, refreshChicken));
        asm ("" : "+l" (flagsOffset));
        target = bytes + flagsOffset;
        asm ("" : "+l" (target));
        target += index;
        asm ("" : "+l" (target));
        *reinterpret_cast<u8 *>(target) = enabled;
        phase += sizeof(u32);
        asm ("" : "+l" (phase));
        register unsigned offsetStride asm("r0") = sizeof(u16);
        asm ("" : "+l" (offsetStride));
        chickenOffset += offsetStride;
        asm ("" : "+h" (chickenOffset));
        ++index;
        asm ("" : "+l" (index));
    } while (index <= 4);

    register unsigned spriteOffset asm("r4");
    register unsigned tailOffset asm("r1");
    register u8 * spriteY asm("r1");
    spriteOffset = offsetof(ChickenFeedingScene, spriteY);
    asm ("" : "+l" (spriteOffset));
    spriteY = bytes + spriteOffset;
    asm ("" : "+l" (spriteY));
    register unsigned reset asm("r2") = 0;
    asm ("" : "+l" (reset));
    *spriteY = 120;
    flagsOffset -= (offsetof(ChickenFeedingScene, refreshEffect) - offsetof(ChickenFeedingScene, roundTimer));
    asm ("" : "+l" (flagsOffset));
    register u8 * roundTimer asm("r1") = bytes + flagsOffset;
    asm ("" : "+l" (roundTimer));
    *roundTimer = 60;
    tailOffset = offsetof(ChickenFeedingScene, actionTimer);
    asm ("" : "+l" (tailOffset));
    target = bytes + tailOffset;
    asm ("" : "+l" (target));
    *reinterpret_cast<u8 *>(target) = reset;
    spriteOffset = spriteOffset + (offsetof(ChickenFeedingScene, feedingCount) - offsetof(ChickenFeedingScene, spriteY));
    asm ("" : "+l" (spriteOffset));
    target = bytes + spriteOffset;
    asm ("" : "+l" (target));
    *reinterpret_cast<u8 *>(target) = reset;
    flagsOffset = flagsOffset + (offsetof(ChickenFeedingScene, missedFeedings) - offsetof(ChickenFeedingScene, roundTimer));
    asm ("" : "+l" (flagsOffset));
    target = bytes + flagsOffset;
    asm ("" : "+l" (target));
    *reinterpret_cast<u8 *>(target) = reset;
    register unsigned troughSelectionOffset asm("r0") = offsetof(ChickenFeedingScene, selectedTrough);
    asm ("" : "+l" (troughSelectionOffset));
    register int * selectedTrough asm("r1") = reinterpret_cast<int *>(bytes + troughSelectionOffset);
    asm ("" : "+l" (selectedTrough));
    *selectedTrough = 2;
    tailOffset = offsetof(ChickenFeedingScene, phase);
    asm ("" : "+l" (tailOffset));
    target = bytes + tailOffset;
    asm ("" : "+l" (target));
    *reinterpret_cast<u32 *>(target) = reset;
    spriteOffset = spriteOffset - (offsetof(ChickenFeedingScene, feedingCount) - offsetof(ChickenFeedingScene, inputPhase));
    asm ("" : "+l" (spriteOffset));
    target = bytes + spriteOffset;
    asm ("" : "+l" (target));
    *reinterpret_cast<u32 *>(target) = reset;
    flagsOffset = flagsOffset - (offsetof(ChickenFeedingScene, missedFeedings) - offsetof(ChickenFeedingScene, resultPhase));
    asm ("" : "+l" (flagsOffset));
    target = bytes + flagsOffset;
    asm ("" : "+l" (target));
    *reinterpret_cast<u32 *>(target) = reset;
    tailOffset = tailOffset + (offsetof(ChickenFeedingScene, actionReady) - offsetof(ChickenFeedingScene, phase));
    asm ("" : "+l" (tailOffset));
    target = bytes + tailOffset;
    asm ("" : "+l" (target));
    register unsigned ready asm("r1") = 1;
    asm ("" : "+l" (ready));
    *target = ready;
    spriteOffset = spriteOffset + (offsetof(ChickenFeedingScene, refreshSprite) - offsetof(ChickenFeedingScene, inputPhase));
    asm ("" : "+l" (spriteOffset));
    target = bytes + spriteOffset;
    asm ("" : "+l" (target));
    *reinterpret_cast<u8 *>(target) = ready;
    flagsOffset = flagsOffset + (offsetof(ChickenFeedingScene, resultMessageReady) - offsetof(ChickenFeedingScene, resultPhase));
    asm ("" : "+l" (flagsOffset));
    target = bytes + flagsOffset;
    asm ("" : "+l" (target));
    *reinterpret_cast<u8 *>(target) = reset;
    tailOffset = offsetof(ChickenFeedingScene, resultSoundReady);
    asm ("" : "+l" (tailOffset));
    target = bytes + tailOffset;
    asm ("" : "+l" (target));
    *reinterpret_cast<u8 *>(target) = reset;
    spriteOffset = spriteOffset + (offsetof(ChickenFeedingScene, success) - offsetof(ChickenFeedingScene, refreshSprite));
    asm ("" : "+l" (spriteOffset));
    target = bytes + spriteOffset;
    asm ("" : "+l" (target));
    *reinterpret_cast<u8 *>(target) = reset;
}

EC void HardwareContext_ForceBlank(ChickenFeedingScene & scene);
EC void TileHandle_Release(ChickenFeedingResource & resource, u32 handle);
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
EC void FormatMessageInteger(i32, char *, u32);
EC void ClearMessageWindowPortrait(MessageWindow *);
EC i32 GetMessageWindowPhase(MessageWindow *);
EC void OpenMessageWindow(MessageWindow *);
EC void CloseMessageWindow(MessageWindow *);
EC void SetMessageWindowText(MessageWindow *, const char *);
EC u8 DisableMessageWindowInstantText(MessageWindow *);
EC u8 EnableMessageWindowInstantText(MessageWindow *);
EC u32 UpdateMessageWindow(MessageWindow *, void *, void *, void *, void *, void *, void *);
EC void * HardwareContext_GetState(ChickenFeedingScene *);
EC void * HardwareContext_GetOamBuffer(ChickenFeedingScene *);
EC void * HardwareContext_GetFrameTasks(ChickenFeedingScene *);
EC void * HardwareContext_GetVBlankTasks(ChickenFeedingScene *);
EC void HardwareContext_UpdateKeys(ChickenFeedingScene *);
EC char const gUnk_0810782C[], gUnk_08107834[], gUnk_08107844[];

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
    TileHandle_Release(resource, resource.handle);
    resource.handle = func_08007B54(resource, capacity);
}

EC SECTION(".text.080AE4D8") u32 GetChickenFeedingResult(ChickenFeedingScene const & scene)
{
    return scene.success;
}

EC SECTION(".text.080AE6C0") void BlankChickenFeedingDisplay(ChickenFeedingScene & scene)
{
    HardwareContext_ForceBlank(scene);
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

EC u32 HardwareContext_GetRepeatedKeys(ChickenFeedingScene *);
EC u32 HardwareContext_GetHeldKeys(ChickenFeedingScene *);
EC u32 HardwareContext_GetPressedKeys(ChickenFeedingScene *);
EC void HandleChickenFeedingAction(ChickenFeedingScene *);
EC void ToggleFeedingPause(ChickenFeedingScene *);

EC SECTION(".text.080B3518") void HandleChickenFeedingInput(ChickenFeedingScene & scene)
{
    HardwareContext_GetRepeatedKeys(&scene);
    unsigned held = HardwareContext_GetHeldKeys(&scene);
    register unsigned pressed asm("r8") = HardwareContext_GetPressedKeys(&scene);
    asm ("" : "+h" (pressed));
    switch (scene.movementState)
    {
    case 1:
    case 2:
        if (!(held & 0x30))
        {
            UpdateChickenFeedingSelectedTrough(scene);
            if (scene.selectedTrough != 5)
            {
                scene.movementState = 0;
                scene.refreshSprite = 1;
            }
        }
        else if ((held & 0x20) && scene.movementState != 1)
        {
            scene.movementState = 1;
            scene.refreshSprite = 1;
        }
        else if ((held & 0x10) && scene.movementState != 2)
        {
            scene.movementState = 2;
            scene.refreshSprite = 1;
        }
        break;
    case 0:
        if ((held & 1) && scene.actionReady)
            HandleChickenFeedingAction(&scene);
        if (scene.movementState == 0)
        {
            if (held & 0x20)
            {
                scene.movementState = 1;
                scene.refreshSprite = 1;
            }
            else if (held & 0x10)
            {
                scene.movementState = 2;
                scene.refreshSprite = 1;
            }
        }
        break;
    case 3:
    {
        register unsigned timerOffset asm("r1") = offsetof(ChickenFeedingScene, actionTimer);
        asm ("" : "+l" (timerOffset));
        register u8 * timer asm("r2") = reinterpret_cast<u8 *>(&scene) + timerOffset;
        asm ("" : "+l" (timer));
        register unsigned nextTimer asm("r0") = *timer + 1;
        asm ("" : "+l" (nextTimer));
        register unsigned reset asm("r3") = 0;
        asm ("" : "+l" (reset));
        *timer = nextTimer;
        if (static_cast<u8>(nextTimer) > 30)
        {
            scene.movementState = reset;
            timerOffset += offsetof(ChickenFeedingScene, refreshSprite) - offsetof(ChickenFeedingScene, actionTimer);
            asm ("" : "+l" (timerOffset));
            register u8 * refresh asm("r0") = reinterpret_cast<u8 *>(&scene) + timerOffset;
            asm ("" : "+l" (refresh));
            register unsigned actionButton asm("r1") = 1;
            asm ("" : "+l" (actionButton));
            *refresh = actionButton;
            *timer = reset;
            register unsigned actionHeld asm("r0") = held;
            asm ("" : "+l" (actionHeld));
            if (actionHeld & actionButton)
            {
                register unsigned readyOffset asm("r2") = offsetof(ChickenFeedingScene, actionReady);
                asm ("" : "+l" (readyOffset));
                register u8 * ready asm("r0") = reinterpret_cast<u8 *>(&scene) + readyOffset;
                asm ("" : "+l" (ready));
                *ready = reset;
            }
        }
        break;
    }
    }
    register unsigned readyOffset asm("r0") = offsetof(ChickenFeedingScene, actionReady);
    asm ("" : "+l" (readyOffset));
    register u8 * actionReady asm("r1") = reinterpret_cast<u8 *>(&scene) + readyOffset;
    asm ("" : "+l" (actionReady));
    if (!*actionReady)
    {
        register unsigned actionButton asm("r0") = 1;
        asm ("" : "+l" (actionButton));
        register unsigned actionHeld asm("r7") = held;
        actionHeld &= actionButton;
        asm ("" : "+l" (actionHeld));
        if (!actionHeld)
            *actionReady = actionButton;
    }
    if (scene.movementState == 1 && scene.spriteY > 73)
        scene.spriteY -= 2;
    else if (scene.movementState == 2 && scene.spriteY <= 166)
        scene.spriteY += 2;
    if (pressed & 8)
        ToggleFeedingPause(&scene);
}

EC SECTION(".text.080B1B40") void RunChickenFeedingTargetMessage(ChickenFeedingScene & scene)
{
    struct TargetMessage
    {
        char number[4];
        char prefix[8];
        char leadingText[16];
        char trailingText[12];
        char text[36];
        u16 control;
        u32 needsOpening;
        int visibleFrames;
    } message;

    FormatMessageInteger(scene.targetFeedings, message.number, 0);
    register char * copyTo asm("r0") = message.prefix;
    asm ("" : "+l" (copyTo));
    register char const * prefix asm("r4") = gUnk_0810782C;
    asm ("" : "+l" (prefix));
    memcpy(copyTo, prefix, 6);
    copyTo = message.leadingText;
    asm ("" : "+l" (copyTo));
    register char const * leadingText asm("r6") = gUnk_08107834;
    asm ("" : "+l" (leadingText));
    memcpy(copyTo, leadingText, 14);
    copyTo = message.trailingText;
    asm ("" : "+l" (copyTo));
    register char const * trailingData asm("r2") = gUnk_08107844;
    asm ("" : "+l" (trailingData));
    register char const * trailingText asm("r8") = trailingData;
    asm ("" : "+h" (trailingText));
    memcpy(copyTo, trailingText, 10);
    char * text = message.text;
    register u32 prefixStart asm("r1") = *reinterpret_cast<u32 const *>(prefix);
    asm ("" : "+l" (prefixStart));
    *reinterpret_cast<volatile u32 *>(message.text) = prefixStart;
    register unsigned prefixEnd asm("r0") = *reinterpret_cast<u16 const *>(prefix + sizeof(prefixStart));
    asm ("" : "+l" (prefixEnd));
    reinterpret_cast<volatile u16 *>(text)[2] = prefixEnd;
    *reinterpret_cast<volatile u32 *>(message.text) = prefixStart;
    reinterpret_cast<volatile u16 *>(text)[2] = prefixEnd;
    strcat(text, leadingText);
    strcat(text, message.number);
    strcat(text, trailingText);

    register unsigned needsOpening asm("r3") = 1;
    asm ("" : "+l" (needsOpening));
    message.needsOpening = needsOpening;
    message.visibleFrames = 0;
    register ChickenFeedingDisplayRegisters * display asm("r0") =
        static_cast<ChickenFeedingDisplayRegisters *>(HardwareContext_GetDisplayRegisters(&scene));
    asm ("" : "+l" (display));
    register u16 * temporary asm("r2") = &message.control;
    asm ("" : "+l" (temporary));
    register u32 setting asm("r3") = 0x1740;
    asm ("" : "+l" (setting));
    register u32 bits asm("r1") = setting;
    asm ("" : "+l" (bits));
    *temporary = bits;
    display->displayControl = bits;

    register unsigned initialWindowOffset asm("r0") = offsetof(ChickenFeedingScene, message) / 8;
    asm ("" : "+l" (initialWindowOffset));
    register MessageWindow ** initialWindow asm("r4") =
        reinterpret_cast<MessageWindow **>(reinterpret_cast<u8 *>(&scene) + initialWindowOffset * 8);
    asm ("" : "+l" (initialWindow));
    ClearMessageWindowPortrait(*initialWindow);
    register MessageWindow * loadedWindow asm("r4") = *initialWindow;
    asm ("" : "+l" (loadedWindow));
    register MessageWindow * window asm("sl") = loadedWindow;
    asm ("" : "+h" (window));
    register void * state asm("sb") = HardwareContext_GetState(&scene);
    asm ("" : "+h" (state));
    register void * oam asm("r8") = HardwareContext_GetOamBuffer(&scene);
    asm ("" : "+h" (oam));
    register void * registers asm("r6") = HardwareContext_GetDisplayRegisters(&scene);
    asm ("" : "+l" (registers));
    register void * frameTasks asm("r4") = HardwareContext_GetFrameTasks(&scene);
    asm ("" : "+l" (frameTasks));
    register void * vblankTasks asm("r0") = HardwareContext_GetVBlankTasks(&scene);
    asm ("" : "+l" (vblankTasks));
    register unsigned audioOffset asm("r2") = offsetof(ChickenFeedingScene, messageAudio);
    asm ("" : "+l" (audioOffset));
    register void * messageAudio asm("r1") = reinterpret_cast<u8 *>(&scene) + audioOffset;
    asm ("" : "+l" (messageAudio));
    UpdateMessageWindow(window, state, oam, registers, frameTasks, vblankTasks, messageAudio);

    goto render;
update:
    {
        func_080087C8(scene);
        HardwareContext_UpdateKeys(&scene);
        register unsigned currentWindowOffset asm("r0") = offsetof(ChickenFeedingScene, message) / 8;
        asm ("" : "+l" (currentWindowOffset));
        register MessageWindow ** currentWindow asm("r4") =
            reinterpret_cast<MessageWindow **>(reinterpret_cast<u8 *>(&scene) + currentWindowOffset * 8);
        asm ("" : "+l" (currentWindow));
        register i32 previousPhase asm("sl") = GetMessageWindowPhase(*currentWindow);
        asm ("" : "+h" (previousPhase));
        loadedWindow = *currentWindow;
        asm ("" : "+l" (loadedWindow));
        register MessageWindow * frameWindow asm("sb") = loadedWindow;
        asm ("" : "+h" (frameWindow));
        register void * frameState asm("r8") = HardwareContext_GetState(&scene);
        asm ("" : "+h" (frameState));
        register void * frameOam asm("r6") = HardwareContext_GetOamBuffer(&scene);
        asm ("" : "+l" (frameOam));
        register void * frameRegisters asm("r5") = HardwareContext_GetDisplayRegisters(&scene);
        asm ("" : "+l" (frameRegisters));
        frameTasks = HardwareContext_GetFrameTasks(&scene);
        asm ("" : "+l" (frameTasks));
        vblankTasks = HardwareContext_GetVBlankTasks(&scene);
        asm ("" : "+l" (vblankTasks));
        audioOffset = offsetof(ChickenFeedingScene, messageAudio);
        asm ("" : "+l" (audioOffset));
        messageAudio = reinterpret_cast<u8 *>(&scene) + audioOffset;
        asm ("" : "+l" (messageAudio));
        switch (UpdateMessageWindow(frameWindow, frameState, frameOam, frameRegisters, frameTasks, vblankTasks, messageAudio))
        {
        case 0:
            needsOpening = message.needsOpening;
            asm ("" : "+l" (needsOpening));
            if (!needsOpening)
                scene.phase = 2;
            else
            {
                register unsigned windowOffset asm("r1") = offsetof(ChickenFeedingScene, message) / 8;
                asm ("" : "+l" (windowOffset));
                windowOffset *= 8;
                asm ("" : "+l" (windowOffset));
                register MessageWindow ** dialog asm("r0") =
                    reinterpret_cast<MessageWindow **>(reinterpret_cast<u8 *>(&scene) + windowOffset);
                asm ("" : "+l" (dialog));
                OpenMessageWindow(*dialog);
            }
            break;
        case 3:
            register i32 oldPhase asm("r2");
            oldPhase = previousPhase;
            asm ("" : "+l" (oldPhase));
            if (oldPhase == 1)
            {
                register unsigned windowOffset asm("r3") = offsetof(ChickenFeedingScene, message) / 8;
                asm ("" : "+l" (windowOffset));
                register MessageWindow ** dialog asm("r4") =
                    reinterpret_cast<MessageWindow **>(reinterpret_cast<u8 *>(&scene) + windowOffset * 8);
                asm ("" : "+l" (dialog));
                SetMessageWindowText(*dialog, message.text);
                DisableMessageWindowInstantText(*dialog);
                message.needsOpening = 0;
            }
            // fall through
        case 1:
        case 2:
        case 4:
        case 5:
        case 6:
            register int visibleFrames asm("r1");
            visibleFrames = message.visibleFrames + 1;
            asm ("" : "+l" (visibleFrames));
            message.visibleFrames = visibleFrames;
            if (visibleFrames > 120)
            {
                register unsigned windowOffset asm("r2") = offsetof(ChickenFeedingScene, message) / 8;
                asm ("" : "+l" (windowOffset));
                register MessageWindow ** dialog asm("r4") =
                    reinterpret_cast<MessageWindow **>(reinterpret_cast<u8 *>(&scene) + windowOffset * 8);
                asm ("" : "+l" (dialog));
                EnableMessageWindowInstantText(*dialog);
                CloseMessageWindow(*dialog);
            }
            break;
        }
    }
render:
    func_080AF814(scene);
    register unsigned phaseOffset asm("r3") = offsetof(ChickenFeedingScene, phase);
    asm ("" : "+l" (phaseOffset));
    register u32 * phase asm("r0") = reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(&scene) + phaseOffset);
    asm ("" : "+l" (phase));
    if (*phase == 1)
        goto update;
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

EC SECTION(".text.080B36B4") void UpdateChickenFeedingChickens(ChickenFeedingScene & feedingScene)
{
    register ChickenFeedingScene * initialScene asm("r5") = &feedingScene;
    asm ("" : "+l" (initialScene));
    int chickenIndex = 0;
    ChickenFeedingScene * scene = initialScene;
    do
    {
        register unsigned resetOffset asm("r1") = offsetof(ChickenFeedingScene, resetChickenTimer);
        asm ("" : "+l" (resetOffset));
        register u8 * resetFlags asm("r0") = reinterpret_cast<u8 *>(scene) + resetOffset;
        asm ("" : "+l" (resetFlags));
        register u8 * reset asm("r6");
        asm ("add %0, %1, %2" : "=l" (reset) : "l" (resetFlags), "l" (chickenIndex) : "cc");
        if (*reset)
        {
            register int delayRange asm("r4") = 180;
            asm ("" : "+l" (delayRange));
            register unsigned randomSample asm("r0") = static_cast<u16>(rand());
            asm ("" : "+l" (randomSample));
            register int delay asm("r1") = randomSample;
            asm ("" : "+l" (delay));
            delay *= delayRange;
            asm ("" : "+l" (delay));
            delay >>= 16;
            asm ("" : "+l" (delay));
            register unsigned chickenOffset asm("r2") = chickenIndex * sizeof(u16);
            asm ("" : "+l" (chickenOffset));
            register unsigned delayOffset asm("r3") = offsetof(ChickenFeedingScene, chickenDelay);
            asm ("" : "+l" (delayOffset));
            register u8 * delays asm("r0") = reinterpret_cast<u8 *>(scene) + delayOffset;
            asm ("" : "+l" (delays));
            register u16 * timer asm("r0") = reinterpret_cast<u16 *>(delays + chickenOffset);
            asm ("" : "+l" (timer));
            register unsigned clear asm("r2") = 0;
            asm ("" : "+l" (clear));
            *timer = delay + 180;
            *reset = clear;
        }
        unsigned wordOffset = chickenIndex * sizeof(u32);
        asm ("" : : "l" (wordOffset));
        register unsigned phaseField asm("r4") = offsetof(ChickenFeedingScene, chickenPhase);
        asm ("" : "+l" (phaseField));
        register u8 * phases asm("r1") = reinterpret_cast<u8 *>(scene) + phaseField;
        asm ("" : "+l" (phases));
        switch (*reinterpret_cast<u32 *>(phases + wordOffset))
        {
        case 3:
        {
            register unsigned yOffset asm("r6") = offsetof(ChickenFeedingScene, chickenY);
            asm ("" : "+l" (yOffset));
            register u8 * yBase asm("r0") = reinterpret_cast<u8 *>(scene) + yOffset;
            asm ("" : "+l" (yBase));
            register u8 * y asm("r1");
            asm ("add %0, %1, %2" : "=l" (y) : "l" (yBase), "l" (chickenIndex) : "cc");
            unsigned currentY = *y;
            if (currentY > 112)
                *y = currentY - 1;
            if (*y != 112)
                break;
            register unsigned chickenPhaseField asm("r1") = offsetof(ChickenFeedingScene, chickenPhase);
            asm ("" : "+l" (chickenPhaseField));
            register u32 * chickenPhaseEntry asm("r0") = reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(scene) + chickenPhaseField);
            asm ("" : "+l" (chickenPhaseEntry));
            chickenPhaseEntry += chickenIndex;
            register unsigned one asm("r3") = 1;
            asm ("" : "+l" (one));
            *chickenPhaseEntry = one;
            register unsigned chickenReactionField asm("r2") = offsetof(ChickenFeedingScene, chickenReaction);
            asm ("" : "+l" (chickenReactionField));
            register u32 * chickenReactionEntry asm("r0") = reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(scene) + chickenReactionField);
            asm ("" : "+l" (chickenReactionEntry));
            chickenReactionEntry += chickenIndex;
            *chickenReactionEntry = 2;
            register unsigned resetChickenTimerField asm("r4") = offsetof(ChickenFeedingScene, resetChickenTimer);
            asm ("" : "+l" (resetChickenTimerField));
            register u8 * resetChickenTimerEntry asm("r0") = reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(scene) + resetChickenTimerField);
            asm ("" : "+l" (resetChickenTimerEntry));
            asm ("add %0, %0, %1" : "+l" (resetChickenTimerEntry) : "l" (chickenIndex) : "cc");
            register unsigned zero asm("r2") = 0;
            asm ("" : "+l" (zero));
            *resetChickenTimerEntry = one;
            register unsigned halfwordIndex asm("r1") = chickenIndex * sizeof(u16);
            asm ("" : "+l" (halfwordIndex));
            register unsigned chickenOffsetsField asm("r6") = offsetof(ChickenFeedingScene, chickenOffsets);
            asm ("" : "+l" (chickenOffsetsField));
            register u16 * chickenOffsetsEntry asm("r0") = reinterpret_cast<u16 *>(reinterpret_cast<u8 *>(scene) + chickenOffsetsField);
            asm ("" : "+l" (chickenOffsetsEntry));
            asm ("add %0, %0, %1" : "+l" (chickenOffsetsEntry) : "l" (halfwordIndex) : "cc");
            *chickenOffsetsEntry = zero;
            register unsigned refreshChickenField asm("r1") = offsetof(ChickenFeedingScene, refreshChicken);
            asm ("" : "+l" (refreshChickenField));
            register u8 * refreshChickenEntry asm("r0") = reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(scene) + refreshChickenField);
            asm ("" : "+l" (refreshChickenEntry));
            asm ("add %0, %0, %1" : "+l" (refreshChickenEntry) : "l" (chickenIndex) : "cc");
            *refreshChickenEntry = one;
            register unsigned refreshReactionField asm("r2") = offsetof(ChickenFeedingScene, refreshReaction);
            asm ("" : "+l" (refreshReactionField));
            register u8 * refreshReactionEntry asm("r0") = reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(scene) + refreshReactionField);
            asm ("" : "+l" (refreshReactionEntry));
            asm ("add %0, %0, %1" : "+l" (refreshReactionEntry) : "l" (chickenIndex) : "cc");
            *refreshReactionEntry = one;
            break;
        }
        case 4:
        {
            register unsigned yOffset asm("r3") = offsetof(ChickenFeedingScene, chickenY);
            asm ("" : "+l" (yOffset));
            register u8 * y asm("r1") = reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(scene) + yOffset);
            asm ("" : "+l" (y));
            y += chickenIndex;
            asm ("" : "+l" (y));
            if (++*y != 168)
                break;
            register unsigned phaseField asm("r6") = offsetof(ChickenFeedingScene, chickenPhase);
            asm ("" : "+l" (phaseField));
            register u32 * phase asm("r0") = reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(scene) + phaseField);
            asm ("" : "+l" (phase));
            phase += chickenIndex;
            *phase = 5;
            register unsigned reactionField asm("r1") = offsetof(ChickenFeedingScene, chickenReaction);
            asm ("" : "+l" (reactionField));
            register u32 * reaction asm("r0") = reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(scene) + reactionField);
            asm ("" : "+l" (reaction));
            reaction += chickenIndex;
            register unsigned refresh asm("r2") = 1;
            asm ("" : "+l" (refresh));
            *reaction = refresh;
            yOffset += offsetof(ChickenFeedingScene, missedFeedings) - offsetof(ChickenFeedingScene, chickenY);
            asm ("" : "+l" (yOffset));
            register u8 * misses asm("r1") = reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(scene) + yOffset);
            asm ("" : "+l" (misses));
            ++*misses;
            register unsigned refreshOffset asm("r4") = offsetof(ChickenFeedingScene, refreshChicken);
            asm ("" : "+l" (refreshOffset));
            register u8 * refreshChicken asm("r0") = reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(scene) + refreshOffset);
            asm ("" : "+l" (refreshChicken));
            refreshChicken += chickenIndex;
            asm ("" : "+l" (refreshChicken));
            *refreshChicken = refresh;
            phaseField += offsetof(ChickenFeedingScene, refreshReaction) - offsetof(ChickenFeedingScene, chickenPhase);
            asm ("" : "+l" (phaseField));
            register u8 * refreshReaction asm("r0") = reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(scene) + phaseField);
            asm ("" : "+l" (refreshReaction));
            refreshReaction += chickenIndex;
            asm ("" : "+l" (refreshReaction));
            *refreshReaction = refresh;
            register unsigned soundOffset asm("r1") = offsetof(ChickenFeedingScene, effectsTrack);
            asm ("" : "+l" (soundOffset));
            register u8 * audio asm("r0") = reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(scene) + soundOffset);
            asm ("" : "+l" (audio));
            AudioPlayer_StartSong(audio, 0xc7);
            break;
        }
        case 6:
        {
            register unsigned timerIndex asm("r1") = chickenIndex * sizeof(u16);
            asm ("" : "+l" (timerIndex));
            register unsigned timerOffset asm("r2") = offsetof(ChickenFeedingScene, chickenTimers);
            asm ("" : "+l" (timerOffset));
            register u8 * timers asm("r0") = reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(scene) + timerOffset);
            asm ("" : "+l" (timers));
            register u16 * timer asm("r2") = reinterpret_cast<u16 *>(timers + timerIndex);
            asm ("" : "+l" (timer));
            register unsigned ticks asm("r1") = *timer + 1;
            asm ("" : "+l" (ticks));
            register unsigned clearTimer asm("r3") = 0;
            asm ("" : "+l" (clearTimer));
            *timer = ticks;
            if (static_cast<u16>(ticks) == 300)
            {
                *timer = clearTimer;
                register unsigned phaseField asm("r3") = offsetof(ChickenFeedingScene, chickenPhase);
                asm ("" : "+l" (phaseField));
                register u32 * phase asm("r0") = reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(scene) + phaseField);
                asm ("" : "+l" (phase));
                phase += chickenIndex;
                *phase = 3;
                register unsigned refreshOffset asm("r4") = offsetof(ChickenFeedingScene, refreshChicken);
                asm ("" : "+l" (refreshOffset));
                register u8 * refresh asm("r0") = reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(scene) + refreshOffset);
                asm ("" : "+l" (refresh));
                refresh += chickenIndex;
                asm ("" : "+l" (refresh));
                *refresh = 1;
            }
            break;
        }
        case 1:
        {
            unsigned slotOffset = chickenIndex * sizeof(u16);
            asm ("" : : "l" (slotOffset));
            register unsigned timerField asm("r6") = offsetof(ChickenFeedingScene, chickenOffsets);
            asm ("" : "+l" (timerField));
            register u8 * timers asm("r0") = reinterpret_cast<u8 *>(scene) + timerField;
            asm ("" : "+l" (timers));
            register u16 * timer asm("r3") = reinterpret_cast<u16 *>(timers + slotOffset);
            asm ("" : "+l" (timer));
            register unsigned ticks asm("r1") = *timer + 1;
            asm ("" : "+l" (ticks));
            register unsigned clearedFlagLow asm("r0") = 0;
            asm ("" : "+l" (clearedFlagLow));
            register unsigned clearedFlag asm("ip") = clearedFlagLow;
            asm ("" : "+h" (clearedFlag));
            register unsigned clearedTimerLow asm("r6") = 0;
            asm ("" : "+l" (clearedTimerLow));
            register unsigned clearedTimer asm("r8") = clearedTimerLow;
            asm ("" : "+h" (clearedTimer));
            *timer = ticks;
            if (static_cast<u16>(ticks) == (scene->chickenDelay[chickenIndex] >> 1))
            {
                register unsigned zero asm("r0") = clearedTimer;
                asm ("" : "+l" (zero));
                *timer = zero;
                scene->chickenPhase[chickenIndex] = 2;
                register unsigned animationOffset asm("r2") = offsetof(ChickenFeedingScene, chickenEffect);
                asm ("" : "+l" (animationOffset));
                register u32 * animation asm("r0") = reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(scene) + animationOffset);
                asm ("" : "+l" (animation));
                animation += chickenIndex;
                *animation = 1;
                register unsigned refreshOffset asm("r3") = offsetof(ChickenFeedingScene, refreshChicken);
                asm ("" : "+l" (refreshOffset));
                register u8 * refresh asm("r0") = reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(scene) + refreshOffset);
                asm ("" : "+l" (refresh));
                refresh += chickenIndex;
                asm ("" : "+l" (refresh));
                *refresh = 1;
            }
            {
                register unsigned effectOffset asm("r4") = offsetof(ChickenFeedingScene, refreshEffect);
                asm ("" : "+l" (effectOffset));
                register u8 * effectBase asm("r0") = reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(scene) + effectOffset);
                asm ("" : "+l" (effectBase));
                register u8 * effect asm("r2") = effectBase + chickenIndex;
                asm ("" : "+l" (effect));
                if (*effect)
                {
                    register unsigned feedingCountOffset asm("r6") = offsetof(ChickenFeedingScene, feedingCount);
                    asm ("" : "+l" (feedingCountOffset));
                    register u8 * feedingCount asm("r0") = reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(scene) + feedingCountOffset);
                    asm ("" : "+l" (feedingCount));
                    ++*feedingCount;
                    register unsigned zeroFlag asm("r1") = clearedFlag;
                    asm ("" : "+l" (zeroFlag));
                    *effect = zeroFlag;
                    if (*feedingCount > 5)
                    {
                        register unsigned soundOffset asm("r2") = offsetof(ChickenFeedingScene, feedingRewardTrack);
                        asm ("" : "+l" (soundOffset));
                        register u8 * audio asm("r0") = reinterpret_cast<u8 *>(reinterpret_cast<u8 *>(scene) + soundOffset);
                        asm ("" : "+l" (audio));
                        AudioPlayer_StartSong(audio, 0xbc);
                    }
                }
            }
            break;
        }
        case 2:
        {
            unsigned slotOffset = chickenIndex * sizeof(u16);
            asm ("" : : "l" (slotOffset));
            register unsigned timerField asm("r3") = offsetof(ChickenFeedingScene, chickenOffsets);
            asm ("" : "+l" (timerField));
            register u8 * timers asm("r0") = reinterpret_cast<u8 *>(scene) + timerField;
            asm ("" : "+l" (timers));
            register u16 * timer asm("r3") = reinterpret_cast<u16 *>(timers + slotOffset);
            asm ("" : "+l" (timer));
            register unsigned ticks asm("r1") = *timer + 1;
            asm ("" : "+l" (ticks));
            register unsigned clearedTimerLow asm("r6") = 0;
            asm ("" : "+l" (clearedTimerLow));
            register unsigned clearedTimer asm("r8") = clearedTimerLow;
            asm ("" : "+h" (clearedTimer));
            *timer = ticks;
            if (static_cast<u16>(ticks) == (scene->chickenDelay[chickenIndex] >> 1))
            {
                register unsigned zero asm("r0") = clearedTimer;
                asm ("" : "+l" (zero));
                *timer = zero;
                scene->chickenPhase[chickenIndex] = 4;
                register unsigned animationOffset asm("r2") = offsetof(ChickenFeedingScene, chickenEffect);
                asm ("" : "+l" (animationOffset));
                register u32 * animation asm("r0") = reinterpret_cast<u32 *>(reinterpret_cast<u8 *>(scene) + animationOffset);
                asm ("" : "+l" (animation));
                animation += chickenIndex;
                *animation = 2;
                register unsigned refreshOffset asm("r3") = offsetof(ChickenFeedingScene, refreshChicken);
                asm ("" : "+l" (refreshOffset));
                register u8 * refresh asm("r0") = reinterpret_cast<u8 *>(scene) + refreshOffset;
                asm ("" : "+l" (refresh));
                refresh += chickenIndex;
                asm ("" : "+l" (refresh));
                *refresh = 1;
                register unsigned soundOffset asm("r4") = offsetof(ChickenFeedingScene, effectsTrack);
                asm ("" : "+l" (soundOffset));
                AudioPlayer_StartSong(reinterpret_cast<u8 *>(scene) + soundOffset, 0xa6);
            }
            break;
        }
        }
    } while (++chickenIndex < 5);
}
