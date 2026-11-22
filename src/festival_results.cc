#include "prelude.h"
#include <new>
#include <stdlib.h>
#include "sprite_animation.hh"
#include "sprite_rendering.hh"

struct AnimalFestivalScore
{
    u8 entryId;
    u16 score;
};

struct FestivalSceneVtable
{
    u32 unknown_00[2];
    void (*destroy)(void *, u32);
};

struct FestivalScene
{
    FestivalSceneVtable * vtable;
};

struct FestivalResultsScreen
{
    u32 unknown_00;
    FestivalSceneVtable * vtable;
};

struct FestivalResultsController
{
    void * vtable;
    FestivalResultsScreen * screen;
    FestivalScene * continuation;
};

EC u8 vtable_unk_080E7934[];
EC FestivalResultsScreen * InitializeFestivalResultsScreen(void *, void *);
EC void RunFestivalResultsScreen(FestivalScene **, FestivalResultsScreen *);
EC void Scene_DestroyBase(void *, u32);
EC u8 vtable_unk_080E7960[];
EC FestivalResultsScreen * InitializeFrisbeeScreen(void *, void *, u8);
EC void RunFrisbeeScreen(FestivalScene **, FestivalResultsScreen *);
EC const u16 gUnk_080FA174[][10];

struct FestivalResultsAnimatedSprite
{
    SpriteAnimator animator;
    SpriteFrameDrawOptions options;
    u32 unknown_30;
};

struct FestivalResultsResource
{
    u32 owner;
    u32 handle;
};

struct FestivalResultsInitializationState
{
    u32 unknown_00;
    FestivalSceneVtable * vtable;
    void * game;
    u32 audioPlayer;
    u8 animationData[0x30];
    FestivalResultsAnimatedSprite sprites[2];
    FestivalResultsResource tiles;
    FestivalResultsResource palettes;
    u16 tilemap[32][32];
    u16 phase;
};

struct FestivalResultsKeyRepeat
{
    u16 delay, interval;
    FestivalResultsKeyRepeat(u16 delay, u16 interval) : delay(delay), interval(interval) {}
};

EC void * HardwareContext_InitializeWithKeyRepeat(void *, const FestivalResultsKeyRepeat *);
EC void * AudioPlayer_Initialize(void *, u32);
EC SpriteAnimationData * InitSpriteAnimationData(SpriteAnimationData *, const void *);
EC void * func_08007874(void *);
EC u32 func_08007B54(void *, u32);
EC void * ResourceHandles_Initialize(void *);
EC u32 func_0800736C(void *);
EC u8 vtable_unk_080E7928[];
EC const u8 gUnk_08527094[];

inline void InitializeFestivalResultsDrawOptions(SpriteFrameDrawOptions * options)
{
    options->offset_x = 0;
    options->offset_y = 0;
    options->palette_base = 0;
    options->tile_base = 0;
    options->priority = 0;
    options->object_mode = 0;
    options->flip = 0;
    options->affine_index = 0;
    options->affine_mode = 0;
    options->mosaic = 0;
}

EC SECTION(".text.festival_results.0805143C") FestivalResultsScreen * InitializeFestivalResultsScreen(void * memory, void * game)
{
    typedef char FestivalResultsInitializationLayout[
        sizeof(FestivalResultsAnimatedSprite) == 0x34 &&
        offsetof(FestivalResultsInitializationState, animationData) == 0x10 &&
        offsetof(FestivalResultsInitializationState, sprites) == 0x40 &&
        offsetof(FestivalResultsInitializationState, tiles) == 0xa8 &&
        offsetof(FestivalResultsInitializationState, palettes) == 0xb0 &&
        offsetof(FestivalResultsInitializationState, tilemap) == 0xb8 &&
        offsetof(FestivalResultsInitializationState, phase) == 0x8b8 ? 1 : -1];
    FestivalResultsInitializationState * screen = (FestivalResultsInitializationState *) memory;
    FestivalResultsKeyRepeat repeat(30, 1);
    HardwareContext_InitializeWithKeyRepeat(screen, &repeat);
    screen->vtable = (FestivalSceneVtable *) vtable_unk_080E7928;
    screen->game = game;
    AudioPlayer_Initialize(&screen->audioPlayer, 2);
    SpriteAnimationData * data = (SpriteAnimationData *) screen->animationData;
    InitSpriteAnimationData(data, gUnk_08527094);
    {
        register SpriteAnimator * destination asm("r0") = &screen->sprites[0].animator;
        register SpriteAnimationSource * animationSource asm("r1") = (SpriteAnimationSource *) data;
        register u32 animationClip asm("r2") = 0;
        asm ("" : "+r" (destination), "+r" (animationSource), "+r" (animationClip));
        register u32 speed asm("r3") = 256;
        asm ("" : "+r" (speed));
        InitSpriteAnimator(destination, animationSource, animationClip, speed);
    }
    InitializeFestivalResultsDrawOptions(&screen->sprites[0].options);
    {
        register SpriteAnimator * destination asm("r0") = &screen->sprites[1].animator;
        register SpriteAnimationSource * animationSource asm("r1") = (SpriteAnimationSource *) data;
        register u32 animationClip asm("r2") = 1;
        asm ("" : "+r" (destination), "+r" (animationSource), "+r" (animationClip));
        register u32 speed asm("r3") = 256;
        asm ("" : "+r" (speed));
        InitSpriteAnimator(destination, animationSource, animationClip, speed);
    }
    InitializeFestivalResultsDrawOptions(&screen->sprites[1].options);
    FestivalResultsResource * tiles = &screen->tiles;
    func_08007874(&tiles->owner);
    tiles->handle = func_08007B54(&tiles->owner, 4);
    FestivalResultsResource * palettes = &screen->palettes;
    ResourceHandles_Initialize(&palettes->owner);
    palettes->handle = func_0800736C(&palettes->owner);
    screen->phase = 0;
    return (FestivalResultsScreen *) screen;
}

EC SECTION(".text.festival_results.0805218C") FestivalResultsController * InitializeFestivalResultsController(FestivalResultsController * controller, FestivalScene ** continuation, void * game) // 0x0805218C
{
    controller->vtable = vtable_unk_080E7934;
    controller->screen = InitializeFestivalResultsScreen(operator new(0x8c0), game);
    FestivalScene * next = *continuation;
    *continuation = nullptr;
    controller->continuation = next;
    return controller;
}

EC SECTION(".text.festival_results.080521BC") void DestroyFestivalResultsController(FestivalResultsController * controller, u32 flags) // 0x080521BC
{
    controller->vtable = vtable_unk_080E7934;
    FestivalScene * next = controller->continuation;
    if (next != nullptr)
        next->vtable->destroy(next, 3);
    FestivalResultsScreen * screen = controller->screen;
    if (screen != nullptr)
        screen->vtable->destroy(screen, 3);
    Scene_DestroyBase(controller, flags);
}

EC SECTION(".text.festival_results.080521FC") FestivalScene ** RunFestivalResultsController(FestivalScene ** result, FestivalResultsController * controller) // 0x080521FC
{
    FestivalScene * completed;
    RunFestivalResultsScreen(&completed, controller->screen);
    if (completed != nullptr)
        completed->vtable->destroy(completed, 3);
    FestivalScene * next = controller->continuation;
    controller->continuation = nullptr;
    *result = next;
    return result;
}

EC SECTION(".text.festival_results.080522D8") void GetAnimalFestivalScore(const AnimalFestivalScore * scores, AnimalFestivalScore * result, u32 index) // 0x080522D8
{
    scores += index;
    result->entryId = scores->entryId;
    result->score = scores->score;
}

EC SECTION(".text.festival_results.080522E8") void SetAnimalFestivalScore(AnimalFestivalScore * scores, const AnimalFestivalScore * value, u32 index) // 0x080522E8
{
    scores += index;
    scores->entryId = value->entryId;
    scores->score = value->score;
}

struct AnimalFestivalScoreTable
{
    AnimalFestivalScore entries[10];
};

struct HorseRaceLineup
{
    u8 entrants[12];
    struct Racer
    {
        u8 speed;
        u8 stamina;
        u8 profile;
        u8 unknown_03;
        u16 paceDelay;
        u16 unknown_06;
    } racers[4];
    u8 heat : 2;
    u8 unused_2c : 6;
    u8 unknown_2d;
    u8 unused_2e : 5;
    u8 specialRace : 1;
    u8 unused_2e_high : 2;
};

EC void PrepareHorseRaceHeat(HorseRaceLineup *, u32);

EC SECTION(".text.horse_race.08057E90") void InitializeHorseRaceLineup(HorseRaceLineup * lineup, u8 specialRace, u32 kind)
{
    lineup->heat = 0;
    lineup->specialRace = specialRace;
    for (i32 index = 0; index <= 11; ++index) lineup->entrants[index] = 255;
    for (i32 index = 0; index <= 11; ++index)
    {
        bool duplicate;
        do
        {
            lineup->entrants[index] = (rand() >> 8) % 40;
            duplicate = false;
            for (i32 other = 0; other <= 11; ++other)
                if (index != other && lineup->entrants[other] == lineup->entrants[index]) duplicate = true;
        } while (duplicate);
    }
    PrepareHorseRaceHeat(lineup, kind);
}

EC SECTION(".text.horse_race.08057F24") void AdvanceHorseRaceHeat(HorseRaceLineup * lineup, u32 kind)
{
    lineup->heat++;
    PrepareHorseRaceHeat(lineup, kind);
}

EC SECTION(".text.horse_race.08057F4C") bool AreHorseRaceHeatsFinished(HorseRaceLineup * lineup)
{
    return lineup->heat == 3;
}

struct HorseRaceStatRange
{
    i32 lowSpeed, highSpeed;
    i32 lowStamina, highStamina;
};

EC const HorseRaceStatRange gUnk_080FA71C[];

EC SECTION(".text.horse_race.08057F64") void PrepareHorseRaceHeat(HorseRaceLineup * lineup, u32 kind)
{
    typedef char HorseRaceRacerLayout[sizeof(HorseRaceLineup::Racer) == 8 ? 1 : -1];
    register u32 special asm("r1");
    if (lineup->heat == 2)
    {
        register u32 raceFlags asm("r0") = ((const u8 *) lineup)[0x2e];
        asm ("" : "+r" (raceFlags));
        special = raceFlags >> 5;
        asm ("" : "+r" (special));
        special &= 1;
    }
    else special = 0;
    asm ("" : "+r" (special));
    u32 useSpecialPresets = special;
    HorseRaceLineup::Racer * racer = lineup->racers;
    for (i32 index = 0; index <= 3; ++index, ++racer)
    {
        i32 preset;
        if (!useSpecialPresets) preset = index;
        else
        {
            switch (kind)
            {
            case 1:
            case 3: preset = index; break;
            case 2: preset = 4; break;
            case 4: preset = 5; break;
            default: preset = 6; break;
            }
        }
        i32 low = gUnk_080FA71C[preset].lowSpeed;
        i32 high = gUnk_080FA71C[preset].highSpeed;
        racer->speed = rand() % (high - low) + low;
        low = gUnk_080FA71C[preset].lowStamina;
        high = gUnk_080FA71C[preset].highStamina;
        racer->stamina = rand() % (high - low) + low;
        racer->profile = lineup->entrants[lineup->heat * 4 + index];
        i32 difference = 250 - racer->speed;
        i32 delay = difference * difference / 1000;
        racer->paceDelay = delay + ((rand() & 3) + 1);
    }
}

struct FestivalResultsDigitSurface
{
    u8 padding00[0xb8];
    u16 tilemap[32][32];
};

EC SECTION(".text.festival_results.08051D30") void DrawFestivalResultsDigit(FestivalResultsDigitSurface * screen, u32 column, u32 row, u32 digit)
{
    u32 topTile = digit * 2 + 0x20c0;
    i32 index = 0;
    register u32 rowOffset asm("r4") = row * 64;
    asm ("" : "+r" (rowOffset));
    u32 bottomTile = digit * 2 + 0x20c1;
    u32 columnOffset = column * 2;
    rowOffset += offsetof(FestivalResultsDigitSurface, tilemap) + 64;
    rowOffset = columnOffset + rowOffset;
    rowOffset += (u32) screen;
    u16 * bottom = (u16 *) rowOffset;
    asm ("" : "+r" (row));
    register u32 topOffset asm("r2") = row * 64;
    asm ("" : "+r" (topOffset));
    topOffset += offsetof(FestivalResultsDigitSurface, tilemap);
    register u32 topAddress asm("r1") = columnOffset + topOffset;
    topAddress += (u32) screen;
    u16 * top = (u16 *) topAddress;
    do
    {
        *top = topTile + index;
        *bottom = bottomTile;
        ++bottomTile;
        ++bottom;
        ++top;
        ++index;
    } while (index <= 0);
}

EC SECTION(".text.festival_results.08052230") void GenerateAnimalFestivalScores(AnimalFestivalScoreTable * table, u32 affection)
{
    u32 level = affection != 0 ? (affection - 1) / 25 : 0;
    for (i32 index = 0; index <= 9; ++index)
    {
        bool unique;
        do
        {
            table->entries[index].entryId = (rand() >> 8) % 30 + 1;
            unique = true;
            for (i32 other = 0; other <= 9; ++other)
                if (index != other && table->entries[index].entryId == table->entries[other].entryId)
                    unique = false;
        } while (!unique);
        i32 roll = rand();
        table->entries[index].score = gUnk_080FA174[level][index] + (roll >> 8) % 10;
    }
}

struct FrisbeeDisplayRegisters
{
    u16 control;
    u8 padding02[6];
    u16 backgroundControl[4];
    u16 scroll[4][2];
};

EC FrisbeeDisplayRegisters * HardwareContext_GetDisplayRegisters(void *);
EC void Dma3_Fill(u32, void *, u32);
EC void Dma3_Copy(const void *, void *, u32);
EC void Unpack(const void *, void *);
EC const u8 gUnk_08738AD8[];
EC const u8 gUnk_08738CC8[];
EC const u8 gUnk_08738CF0[];
EC const u8 gUnk_08738D1C[];
EC const u8 gUnk_087399C4[];
EC const u8 gUnk_08739A64[];

#define WRITE_FRISBEE_REGISTER(field, value, reg) { \
    FrisbeeDisplayRegisters * display = HardwareContext_GetDisplayRegisters(screen); \
    volatile u16 * scratch = &settingScratch; \
    register u32 setting asm("r3") = value; \
    asm ("" : : "r" (setting)); \
    register u32 output asm(reg) = setting; \
    *scratch = output; \
    display->field = output; \
}
#define CLEAR_FRISBEE_SCROLL(index) { \
    FrisbeeDisplayRegisters * display = HardwareContext_GetDisplayRegisters(screen); \
    volatile u16 * scratch = &settingScratch; \
    *scratch = zero; \
    display->scroll[index][0] = zero; \
}

EC SECTION(".text.frisbee.08054F40") void LoadFrisbeeBackground(void * screen)
{
    volatile u16 settingScratch;
    Dma3_Fill(0, (void *) 0x06007fe0, 32);
    Dma3_Fill(~0u, (void *) 0x0600e000, 0x800);
    register void * mapInput asm("r1") = (void *) 0x0600e800;
    asm ("" : "+r" (mapInput));
    void * secondMap = mapInput;
    Dma3_Fill(~0u, secondMap, 0x800);
    Dma3_Fill(~0u, (void *) 0x0600f000, 0x800);
    register const u8 * pattern asm("r0") = gUnk_08738AD8;
    asm ("" : : "r" (pattern));
    register u32 tilemapOffset asm("r2") = 0x1d5a;
    asm ("" : : "r" (pattern), "r" (tilemapOffset));
    u8 * tilemap = (u8 *) screen + tilemapOffset;
    Unpack(pattern, tilemap);
    Dma3_Copy(tilemap, (void *) 0x0600e000, 0x500);
    Dma3_Copy(tilemap, secondMap, 0xc0);
    Unpack(gUnk_08738CC8, tilemap);
    Unpack(gUnk_08738CF0, tilemap);
    Unpack(gUnk_08738D1C, (void *) 0x06000000);
    Dma3_Copy(gUnk_087399C4, (void *) 0x05000000, 0x200);
    register const u8 * animationPattern asm("r0") = gUnk_08739A64;
    asm ("" : : "r" (animationPattern));
    register u32 patternOffset asm("r3") = 0x558;
    asm ("" : : "r" (animationPattern), "r" (patternOffset));
    Unpack(animationPattern, (u8 *) screen + patternOffset);
    WRITE_FRISBEE_REGISTER(control, 0x1e40, "r2");
    u32 zero = 0;
    WRITE_FRISBEE_REGISTER(backgroundControl[3], 0x1c43, "r1");
    WRITE_FRISBEE_REGISTER(backgroundControl[2], 0x1d42, "r1");
    WRITE_FRISBEE_REGISTER(backgroundControl[1], 0x1e41, "r1");
    CLEAR_FRISBEE_SCROLL(1);
    {
        FrisbeeDisplayRegisters * display = HardwareContext_GetDisplayRegisters(screen);
        settingScratch = zero;
        display->scroll[1][1] = zero;
    }
    CLEAR_FRISBEE_SCROLL(2);
    {
        FrisbeeDisplayRegisters * display = HardwareContext_GetDisplayRegisters(screen);
        settingScratch = zero;
        display->scroll[2][1] = zero;
    }
    CLEAR_FRISBEE_SCROLL(3);
    {
        FrisbeeDisplayRegisters * display = HardwareContext_GetDisplayRegisters(screen);
        settingScratch = zero;
        display->scroll[3][1] = zero;
    }
}

#undef WRITE_FRISBEE_REGISTER
#undef CLEAR_FRISBEE_SCROLL

enum FrisbeeDialoguePhase
{
    FrisbeeDialogueStart,
    FrisbeeDialogueClosing,
    FrisbeeDialogueComplete,
    FrisbeeDialogueReady,
    FrisbeeDialogueChoosing,
    FrisbeeDialogueFinish
};

struct FrisbeeDialogueState
{
    u8 padding00[0x10];
    void * message;
    u8 messageState[1];
    u8 padding15[0x2328 - 0x15];
    i16 page;
    u8 padding232a[2];
    i16 phase;
    i16 exitRequested;
    u8 padding2330[0x70];
    u32 substitutions[1];
};

EC void * HardwareContext_GetState(void *);
EC void * HardwareContext_GetOamBuffer(void *);
EC void * HardwareContext_GetFrameTasks(void *);
EC void * HardwareContext_GetVBlankTasks(void *);
EC u32 UpdateMessageWindow(void *, void *, void *, void *, void *, void *, void *);
EC void OpenMessageWindow(void *);
EC void CloseMessageWindow(void *);
EC i32 GetMessageWindowSelection(void *);
EC void SetMessageWindowText(void *, const char *);
EC void SetMessageWindowTextSubstitutions(void *, const char *, void *);
EC void ConfigureMessageWindowPromptChoices(void *, const char *, const char *, const char *, u32, u32, void *);
EC const char gUnk_080FA39C[], gUnk_080FA3B8[], gUnk_080FA3BC[], gUnk_080FA3C0[];
EC const char gUnk_080FA458[], gUnk_080FA49C[], gUnk_080FA510[], gUnk_080FA614[];
EC const char gUnk_080FA660[], gUnk_080FA670[], gUnk_080FA684[], gUnk_080FA6A0[];
EC const char gUnk_080FA6AC[], gUnk_080FA6E0[], gUnk_080FA6F4[];

EC SECTION(".text.frisbee.08053634") void UpdateFrisbeeDialogue(FrisbeeDialogueState * state)
{
    typedef char FrisbeeDialogueLayout[
        offsetof(FrisbeeDialogueState, messageState) == 0x14 &&
        offsetof(FrisbeeDialogueState, page) == 0x2328 &&
        offsetof(FrisbeeDialogueState, phase) == 0x232c &&
        offsetof(FrisbeeDialogueState, exitRequested) == 0x232e &&
        offsetof(FrisbeeDialogueState, substitutions) == 0x23a0 ? 1 : -1];
    void * message = state->message;
    void * input = HardwareContext_GetState(state);
    void * oam = HardwareContext_GetOamBuffer(state);
    void * display = HardwareContext_GetDisplayRegisters(state);
    void * frames = HardwareContext_GetFrameTasks(state);
    void * vblank = HardwareContext_GetVBlankTasks(state);
    register void * messageState asm("r1") = state->messageState;
    asm ("" : "+r" (messageState) : "r" (frames), "r" (vblank));
    switch (UpdateMessageWindow(message, input, oam, display, frames, vblank, messageState))
    {
    case 0:
        if (state->phase == FrisbeeDialogueStart)
        {
            OpenMessageWindow(state->message);
            state->phase = FrisbeeDialogueReady;
        }
        else state->phase = FrisbeeDialogueComplete;
        break;
    case 3:
        if (state->phase == FrisbeeDialogueFinish)
        {
            CloseMessageWindow(state->message);
            state->phase = FrisbeeDialogueClosing;
        }
        else if (state->phase == FrisbeeDialogueChoosing)
        {
            i32 choice = GetMessageWindowSelection(state->message);
            state->phase = FrisbeeDialogueReady;
            switch (state->page)
            {
            case 2:
                if (choice == 1)
                {
                    state->exitRequested = 0;
                    state->page = 3;
                }
                else if (choice == 2)
                {
                    state->exitRequested = 1;
                    state->phase = FrisbeeDialogueFinish;
                }
                break;
            case 9:
                if (choice == 1) state->exitRequested = 0;
                else if (choice == 2) state->exitRequested = 1;
                state->phase = FrisbeeDialogueFinish;
                break;
            case 23:
                if (choice == 1) state->exitRequested = 0;
                else if (choice == 2) state->exitRequested = 1;
                state->phase = FrisbeeDialogueFinish;
                break;
            }
        }
        else if (state->phase == FrisbeeDialogueReady)
        {
            state->phase = FrisbeeDialogueFinish;
            switch ((u32) (i16) (state->page - 2))
            {
            case 0:
                ConfigureMessageWindowPromptChoices(state->message, gUnk_080FA39C, gUnk_080FA3B8, gUnk_080FA3BC, 0, 0, nullptr);
                state->phase = FrisbeeDialogueChoosing;
                break;
            case 1:
                SetMessageWindowText(state->message, gUnk_080FA3C0);
                break;
            case 3:
                SetMessageWindowText(state->message, gUnk_080FA458);
                break;
            case 4:
                SetMessageWindowText(state->message, gUnk_080FA49C);
                break;
            case 5:
                SetMessageWindowText(state->message, gUnk_080FA510);
                state->page = 9;
                state->phase = FrisbeeDialogueReady;
                break;
            case 6:
                SetMessageWindowText(state->message, gUnk_080FA614);
                state->page = 9;
                state->phase = FrisbeeDialogueReady;
                break;
            case 7:
                ConfigureMessageWindowPromptChoices(state->message, gUnk_080FA660, gUnk_080FA3BC, gUnk_080FA3B8, 0, 0, nullptr);
                state->phase = FrisbeeDialogueChoosing;
                break;
            case 22:
                SetMessageWindowText(state->message, gUnk_080FA670);
                break;
            case 23:
                SetMessageWindowText(state->message, gUnk_080FA684);
                break;
            case 21:
                ConfigureMessageWindowPromptChoices(state->message, gUnk_080FA6A0, gUnk_080FA3B8, gUnk_080FA3BC, 0, 0, nullptr);
                state->phase = FrisbeeDialogueChoosing;
                break;
            case 24:
                SetMessageWindowText(state->message, gUnk_080FA6AC);
                break;
            case 18:
                SetMessageWindowTextSubstitutions(state->message, gUnk_080FA6E0, state->substitutions);
                break;
            case 19:
                SetMessageWindowText(state->message, gUnk_080FA6F4);
                break;
            }
        }
        break;
    case 1:
    case 2:
    case 4:
    case 5:
    case 6:
        break;
    }
}

EC void DrawFrisbeeSymbol(void *, i32, i32, i32);

struct FrisbeeScoreboardState
{
    u8 padding0000[0x22c4];
    i16 distance;
    i16 recordDistance;
    i16 firstThrowDistance;
    i16 secondThrowDistance;
    u8 visibleScores;
    u8 flashingScores;
    u8 flashTimer;
    u8 padding22cf[0x55];
    u8 scoreDisplayMode;
};
EC void DrawFrisbeeNumber(void *, i32, i32, i32, u32);
EC SECTION(".text.frisbee.08057B84") void DrawFrisbeeScoreboard(void * screen)
{
    FrisbeeScoreboardState * state = (FrisbeeScoreboardState *) screen;
    DrawFrisbeeNumber(screen, 8, 12, state->distance, 0);
    DrawFrisbeeSymbol(screen, 64, 20, 11);
    if (state->scoreDisplayMode == 0)
    {
        if (state->visibleScores & 1)
        {
            DrawFrisbeeSymbol(screen, 160, 16, 26);
            DrawFrisbeeNumber(screen, 192, 16, state->recordDistance, 1);
            DrawFrisbeeSymbol(screen, 224, 16, 23);
        }
    }
    else
    {
        if (state->visibleScores & 2)
        {
            DrawFrisbeeSymbol(screen, 160, 8, 24);
            DrawFrisbeeNumber(screen, 192, 8, state->firstThrowDistance, 1);
            DrawFrisbeeSymbol(screen, 224, 8, 23);
        }
        if (state->visibleScores & 4)
        {
            DrawFrisbeeSymbol(screen, 160, 24, 25);
            DrawFrisbeeNumber(screen, 192, 22, state->secondThrowDistance, 1);
            DrawFrisbeeSymbol(screen, 224, 22, 23);
        }
    }
    ++state->flashTimer;
    if (!(state->flashTimer & 15)) state->visibleScores ^= state->flashingScores;
}

EC SECTION(".text.frisbee.08057CB0") void DrawFrisbeeNumber(void * volatile screen, i32 x, i32 inputY, i32 inputValue, u32 small)
{
    register i32 y asm("r10") = inputY;
    register i32 value asm("r9") = inputValue;
    register u32 mode asm("r1") = small;
    asm ("" : "+r" (mode));
    i32 divisor = 100;
    register i32 symbol asm("r3") = 0;
    if (mode)
    {
        symbol = 12;
    }
    i32 initialSpacing = 8;
    if (!mode)
    {
        initialSpacing = 16;
    }
    register i32 firstSymbol asm("r8") = symbol;
    asm ("" : "+r" (initialSpacing));
    i32 spacing = initialSpacing;
    do
    {
        i32 digit = value / divisor;
        register i32 base asm("r0") = firstSymbol;
        asm ("" : "+r" (base));
        DrawFrisbeeSymbol(screen, x, y, digit + base);
        x += spacing;
        value -= digit * divisor;
        divisor /= 10;
        if (divisor == 1)
        {
            void * markerScreen = screen;
            register i32 markerX asm("r1") = x;
            register i32 markerY asm("r3") = y;
            asm ("" : "+r" (markerX), "+r" (markerY));
            DrawFrisbeeSymbol(markerScreen, markerX, markerY + spacing, firstSymbol + 10);
            x += 8;
        }
    } while (divisor > 0);
}

struct FrisbeeSymbolGraphics
{
    u8 padding[0x2334];
    u32 tileOffsets[29];
};
struct FrisbeeSpritePlacement
{
    u16 x, y, palette, tile, priority;
    u32 objectMode;
    u8 flip, affineIndex;
    u32 affineMode;
    u8 mosaic;
    FrisbeeSpritePlacement() : x(0), y(0), palette(0), tile(0), priority(0), objectMode(0), flip(0), affineIndex(0), affineMode(0), mosaic(0) {}
};
struct FrisbeeSymbolFrame
{
    const void * objects;
    u16 count;
    u16 padding;
    u32 remaining[6];
};
EC void func_0805E790(FrisbeeSymbolFrame *, void *, u32);
struct ResourceHandleOwner;
struct TileHeapOwner;
EC u32 ResourceHandle_GetIndex(ResourceHandleOwner *, u32);
EC u32 TileHandle_GetTileIndex(TileHeapOwner *, u32);
EC void func_0805E99C(void *, const void *, u32, const FrisbeeSpritePlacement *);
EC SECTION(".text.frisbee.08057D34") void DrawFrisbeeSymbol(void * screen, i32 x, i32 y, i32 symbol)
{
    FrisbeeSpritePlacement placement;
    FrisbeeSymbolFrame frame;
    func_0805E790(&frame, (u8 *) screen + 0x50, symbol);
    placement.x = x;
    placement.y = y;
    ResourceHandleOwner * paletteOwner = (ResourceHandleOwner *) ((u8 *) screen + 0x480);
    placement.palette = ResourceHandle_GetIndex(paletteOwner, *((u32 *) paletteOwner + 1));
    TileHeapOwner * tileOwner = (TileHeapOwner *) ((u8 *) screen + 0x478);
    u32 tileBase = TileHandle_GetTileIndex(tileOwner, *((u32 *) tileOwner + 1));
    placement.tile = ((FrisbeeSymbolGraphics *) screen)->tileOffsets[symbol] + tileBase;
    placement.priority = 1;
    placement.mosaic = true;
    void * oam = HardwareContext_GetOamBuffer(screen);
    register const void * rawObjects asm("r0") = frame.objects;
    asm ("" : "+r" (rawObjects));
    register const void * objects asm("r1") = rawObjects;
    asm ("" : "+r" (objects));
    u32 count = objects != nullptr ? frame.count : 0;
    func_0805E99C(oam, objects, count, &placement);
}

struct FrisbeePaletteAllocation
{
    u32 allocatorContext;
    u32 handle;
};

EC SECTION(".text.frisbee.080579B4") void DrawFrisbeeSpriteList(
    void * hardware, SpriteAttributes const * objects, u32 count,
    SpriteFrameDrawOptions const * options, FrisbeePaletteAllocation * sprite_allocations)
{
    // Preserve the original register allocation and OAM bit packing.
    struct
    {
        SpriteAttributes object;
        void * hardware_context;
        SpriteAttributes const * end;
    } work;
    enum { SHAPE_SQUARE = 0, SHAPE_WIDE = 1, SCREEN_WIDTH = 240, SCREEN_HEIGHT = 160 };
    work.hardware_context = hardware;
    register SpriteFrameDrawOptions const * config asm("r8") = options;
    SpriteAttributes const * cursor = objects;
    SpriteAttributes const * limit = cursor + count;
    work.end = limit;
    if (cursor == limit)
        return;
    do
    {
        SpriteAttributes const * object = cursor;
        work.object = *object;
        u32 position_bits = work.object.words[0];
        register u32 x_shifted asm("r0") = position_bits << 7;
        asm ("" : "+r" (x_shifted));
        register int x asm("r3") = static_cast<i32>(x_shifted) >> 23;
        asm ("" : "+r" (x));
        SpriteAttributes * sprite_address = &work.object;
        int y = sprite_address->bits.y;
        register u32 shape_bits asm("r0") = (position_bits << 16) >> 30;
        asm ("" : "+r" (shape_bits));
        u32 shape = shape_bits;
        u32 size = position_bits >> 30;
        int width;
        int height;
        if (shape == SHAPE_SQUARE)
        {
            height = 8 << size;
            width = height;
        }
        else
        {
            u32 long_side = 8 << (((size + 1) >> 1) + 1);
            u32 short_side = 8;
            if (size != 1)
                short_side = long_side >> 1;
            if (shape != SHAPE_WIDE)
            {
                height = long_side;
                width = short_side;
            }
            else
            {
                height = short_side;
                width = long_side;
            }
        }

        u32 flip = config->flip;
        register u32 one_low asm("r1") = 1;
        asm ("" : "+r" (one_low));
        register u32 flip_bit asm("r10") = one_low;
        register u32 horizontal asm("r0") = flip_bit;
        asm ("" : "+r" (horizontal));
        horizontal &= flip;
        if (horizontal)
        {
            register int right asm("r0") = x + width;
            asm ("" : "+r" (right));
            x = -right;
            u32 packed_flip = work.object.words[0];
            u32 toggled = (packed_flip << 3) >> 31;
            u32 one = flip_bit;
            toggled ^= one;
            asm ("" : "+r" (toggled));
            toggled &= one;
            toggled <<= 28;
            u32 preserved = ~(1u << 28);
            preserved &= packed_flip;
            preserved |= toggled;
            work.object.words[0] = preserved;
        }
        u32 vertical = 2;
        vertical &= flip;
        if (vertical)
        {
            int bottom = y + height;
            y = -bottom;
            u32 packed_flip = work.object.words[0];
            u32 toggled = (packed_flip << 2) >> 31;
            u32 one = flip_bit;
            toggled ^= one;
            asm ("" : "+r" (toggled));
            toggled &= one;
            toggled <<= 29;
            u32 preserved = ~(1u << 29);
            preserved &= packed_flip;
            preserved |= toggled;
            work.object.words[0] = preserved;
        }

        SpriteFrameDrawOptions const * position = config;
        register int offset_x asm("r0");
        offset_x = position->offset_x;
        x += offset_x;
        register int offset_y asm("r0");
        offset_y = position->offset_y;
        y += offset_y;
        int right_edge = x + width;
        if (right_edge > 0 && x < SCREEN_WIDTH)
        {
            int bottom_edge = y + height;
            if (bottom_edge <= 0 || y >= SCREEN_HEIGHT)
                goto next_object;
            register u32 coordinate_mask asm("r0") = 0x1ff;
            x &= coordinate_mask;
            register u32 shifted_x asm("r2") = static_cast<u32>(x) << 16;
            asm ("" : "+r" (shifted_x));
            u32 x_mask = ~(0x1ffu << 16);
            register u32 x_preserve asm("r1") = x_mask;
            register u32 packed asm("r0") = work.object.words[0];
            packed = (packed & x_preserve) | shifted_x;
            u32 y_field = static_cast<u8>(y);
            register u32 y_mask asm("r1") = ~0xffu;
            packed = (packed & y_mask) | y_field;
            work.object.words[0] = packed;

            SpriteAttributes * source = &work.object;
            u32 tile_attrs = work.object.words[1];
            u32 palette_offset = ((tile_attrs << 16) >> 28) * sizeof(*sprite_allocations);
            register FrisbeePaletteAllocation * base asm("r3");
            base = sprite_allocations;
            asm ("" : "+r" (base));
            palette_offset += reinterpret_cast<uintptr_t>(base);
            // The first allocation belongs to the tiles; frame palette indices start after it.
            const unsigned first_palette_allocation = 1;
            FrisbeePaletteAllocation * allocation =
                reinterpret_cast<FrisbeePaletteAllocation *>(palette_offset) + first_palette_allocation;
            u32 palette = static_cast<u16>(ResourceHandle_GetIndex((ResourceHandleOwner *) allocation, allocation->handle));
            register u32 palette_bits asm("r1") = 15;
            palette &= palette_bits;
            palette <<= 12;
            register u32 palette_mask asm("r1") = ~(15u << 12);
            u32 final_tile_attrs = work.object.words[1];
            final_tile_attrs = (final_tile_attrs & palette_mask) | palette;
            register u32 tile asm("r0") = (final_tile_attrs << 22) >> 22;
            register SpriteFrameDrawOptions const * tile_config asm("r1") = config;
            register u32 tile_base asm("r1") = tile_config->tile_base;
            tile = ((tile + tile_base) << 22) >> 22;
            u32 tiles_and_priority = ~0x3ffu;
            tiles_and_priority &= final_tile_attrs;
            tiles_and_priority |= tile;
            register SpriteFrameDrawOptions const * priority_config asm("r2") = config;
            register u32 priority asm("r0") = priority_config->priority;
            register u32 priority_bits asm("r3") = 3;
            asm ("" : "+r" (priority_bits));
            priority &= priority_bits;
            priority <<= 10;
            u32 priority_mask = ~(3u << 10);
            tiles_and_priority = (tiles_and_priority & priority_mask) | priority;
            work.object.words[1] = tiles_and_priority;

            register SpriteFrameDrawOptions const * mode_config asm("r1") = config;
            u32 object_mode = mode_config->object_mode;
            object_mode &= priority_bits;
            object_mode <<= 10;
            u32 position_and_flags = work.object.words[0];
            position_and_flags &= priority_mask;
            position_and_flags |= object_mode;
            SpriteFrameDrawOptions const * flags_config = config;
            register u32 affine_mode asm("r2") = flags_config->affine_mode;
            register u32 affine_bits asm("r0") = 3;
            affine_mode &= affine_bits;
            affine_mode <<= 8;
            register u32 affine_mode_mask asm("r0") = ~(3u << 8);
            position_and_flags = (position_and_flags & affine_mode_mask) | affine_mode;
            register u32 mosaic asm("r2") = flags_config->mosaic;
            register u32 mosaic_bit asm("r3") = flip_bit;
            asm ("" : "+r" (mosaic_bit) : "r" (mosaic));
            mosaic &= mosaic_bit;
            mosaic <<= 12;
            register u32 mosaic_mask asm("r0") = ~(1u << 12);
            position_and_flags = (position_and_flags & mosaic_mask) | mosaic;
            work.object.words[0] = position_and_flags;

            SpriteOamBuffer * oam = static_cast<SpriteOamBuffer *>(HardwareContext_GetOamBuffer(work.hardware_context));
            u32 index = oam->sprite_count;
            if (index < 128)
            {
                SpriteAttributes * dest = oam->sprites + index;
                dest->words[0] = work.object.words[0];
                // The fourth halfword belongs to the affine matrix.
                reinterpret_cast<u16 *>(dest)[2] = reinterpret_cast<u16 const *>(source)[2];
                oam->sprite_count = index + 1;
            }
        }
    next_object:
        register u32 step asm("r0") = sizeof(*cursor);
        asm ("" : "+r" (step));
        cursor = reinterpret_cast<SpriteAttributes const *>(reinterpret_cast<u8 const *>(cursor) + step);
        register SpriteAttributes const * end asm("r1") = work.end;
        if (cursor == end)
            break;
    } while (true);
}

EC SECTION(".text.frisbee.08057DD8") FestivalResultsController * InitializeFrisbeeController(FestivalResultsController * controller, FestivalScene ** continuation, void * game, u8 kind)
{
    controller->vtable = vtable_unk_080E7960;
    controller->screen = InitializeFrisbeeScreen(operator new(0x23a8), game, kind);
    FestivalScene * next = *continuation;
    *continuation = nullptr;
    controller->continuation = next;
    return controller;
}

EC SECTION(".text.frisbee.08057E1C") void DestroyFrisbeeController(FestivalResultsController * controller, u32 flags)
{
    controller->vtable = vtable_unk_080E7960;
    FestivalScene * next = controller->continuation;
    if (next != nullptr) next->vtable->destroy(next, 3);
    FestivalResultsScreen * screen = controller->screen;
    if (screen != nullptr) screen->vtable->destroy(screen, 3);
    Scene_DestroyBase(controller, flags);
}

EC SECTION(".text.frisbee.08057E5C") FestivalScene ** RunFrisbeeController(FestivalScene ** result, FestivalResultsController * controller)
{
    FestivalScene * completed;
    RunFrisbeeScreen(&completed, controller->screen);
    if (completed != nullptr) completed->vtable->destroy(completed, 3);
    FestivalScene * next = controller->continuation;
    controller->continuation = nullptr;
    *result = next;
    return result;
}

enum HorseRaceDialoguePhase
{
    HorseRaceDialogueStart,
    HorseRaceDialogueClosing,
    HorseRaceDialogueComplete,
    HorseRaceDialogueReady,
    HorseRaceDialogueChoosing,
    HorseRaceDialogueFinish
};

struct HorseRaceDialogueState
{
    u8 padding00[0x10];
    void * message;
    u8 messageState[0x140e - 0x14];
    u8 playerIsRacing;
    u8 padding140f[0x1424 - 0x140f];
    i16 page;
    u8 padding1426[3];
    u8 phase;
    u8 exitRequested;
    u8 padding142b[0x149e - 0x142b];
    i16 timer;
    u8 padding14a0[0x40];
    u32 substitutions[1];
};

EC const char gUnk_080FAA40[], gUnk_080FAA5C[], gUnk_080FAA60[], gUnk_080FAA64[], gUnk_080FAA74[], gUnk_080FAA88[], gUnk_080FAAA4[], gUnk_080FAACC[], gUnk_080FAAE0[], gUnk_080FAAFC[], gUnk_080FAB94[], gUnk_080FABE0[], gUnk_080FAC24[], gUnk_080FAC8C[], gUnk_080FAD08[], gUnk_080FAD20[], gUnk_080FAD28[], gUnk_080FAD34[], gUnk_080FAD50[], gUnk_080FAD94[], gUnk_080FADDC[], gUnk_080FAE00[];

EC SECTION(".text.horse_race.08059878") void UpdateHorseRaceDialogue(HorseRaceDialogueState * state)
{
    typedef char HorseRaceDialogueLayout[
        offsetof(HorseRaceDialogueState, messageState) == 0x14 &&
        offsetof(HorseRaceDialogueState, page) == 0x1424 &&
        offsetof(HorseRaceDialogueState, phase) == 0x1429 &&
        offsetof(HorseRaceDialogueState, exitRequested) == 0x142a &&
        offsetof(HorseRaceDialogueState, timer) == 0x149e &&
        offsetof(HorseRaceDialogueState, substitutions) == 0x14e0 ? 1 : -1];
    void * message = state->message;
    void * input = HardwareContext_GetState(state);
    void * oam = HardwareContext_GetOamBuffer(state);
    void * display = HardwareContext_GetDisplayRegisters(state);
    void * frames = HardwareContext_GetFrameTasks(state);
    void * vblank = HardwareContext_GetVBlankTasks(state);
    register void * messageState asm("r1") = state->messageState;
    asm ("" : "+r" (messageState) : "r" (frames), "r" (vblank));
    switch (UpdateMessageWindow(message, input, oam, display, frames, vblank, messageState))
    {
    case 0:
        if (state->phase == HorseRaceDialogueStart)
        {
            OpenMessageWindow(state->message);
            state->phase = HorseRaceDialogueReady;
        }
        else state->phase = HorseRaceDialogueComplete;
        break;
    case 3:
        if (state->phase == HorseRaceDialogueFinish)
        {
            CloseMessageWindow(state->message);
            state->phase = HorseRaceDialogueClosing;
        }
        else if (state->phase == HorseRaceDialogueChoosing)
        {
            i32 choice = GetMessageWindowSelection(state->message);
            state->phase = HorseRaceDialogueReady;
            switch (state->page)
            {
            case 2:
                if (choice == 1) { state->exitRequested = 0; state->page = 3; }
                else if (choice == 2) { state->exitRequested = 1; state->page = 4; }
                break;
            case 7:
                if (choice == 1) { state->exitRequested = 0; state->page = 8; }
                else if (choice == 2) { state->exitRequested = 1; state->page = 9; }
                break;
            case 14:
                if (choice == 1) state->page = 15;
                else if (choice == 2) state->page = 21;
                break;
            case 20:
                if (choice == 1) state->page = 21;
                else if (choice == 2) state->page = 15;
                break;
            }
        }
        else if (state->phase == HorseRaceDialogueReady)
        {
            register u8 * dialoguePhase asm("r4") = &state->phase;
            *dialoguePhase = HorseRaceDialogueFinish;
            register void * window asm("r0");
            register const char * text asm("r1");
            register const char * firstChoice asm("r2");
            register const char * secondChoice asm("r3");
            switch ((u32) (i16) (state->page - 2))
            {
            case 0:
                window = state->message; text = gUnk_080FAA40;
                firstChoice = gUnk_080FAA5C; secondChoice = gUnk_080FAA60;
                asm ("" : "+r" (window), "+r" (text), "+r" (firstChoice), "+r" (secondChoice));
                goto showChoices;
            case 1:
                window = state->message; text = gUnk_080FAA64;
                goto showTextWithTemporaryOffset;
            case 2:
                window = state->message; text = gUnk_080FAA74;
                goto showTextWithSavedOffset;
            case 5:
                window = state->message; text = gUnk_080FAA88;
                firstChoice = gUnk_080FAA5C; secondChoice = gUnk_080FAA60;
                asm ("" : "+r" (window), "+r" (text), "+r" (firstChoice), "+r" (secondChoice));
                goto showChoices;
            case 6:
            {
                u32 playerFlag = state->playerIsRacing;
                asm ("" : : "r" (playerFlag), "r" (dialoguePhase));
                if (playerFlag == 0)
                {
                    window = state->message; text = gUnk_080FAAA4;
                    goto showTextWithTemporaryOffset;
                }
                else
                {
                    window = state->message; text = gUnk_080FAAA4;
                    goto showTextWithSavedOffset;
                }
                break;
            }
            case 7:
                window = state->message; text = gUnk_080FAACC;
                goto showTextWithTemporaryOffset;
            case 12:
                window = state->message; text = gUnk_080FAAE0;
                firstChoice = gUnk_080FAA5C; secondChoice = gUnk_080FAA60;
                asm ("" : "+r" (window), "+r" (text), "+r" (firstChoice), "+r" (secondChoice));
                goto showChoices;
            case 13:
                SetMessageWindowTextSubstitutions(state->message, gUnk_080FAAFC, state->substitutions);
                state->page = 16; state->phase = HorseRaceDialogueReady;
                break;
            case 14:
                SetMessageWindowTextSubstitutions(state->message, gUnk_080FAB94, state->substitutions);
                state->page = 17; state->phase = HorseRaceDialogueReady;
                break;
            case 15:
                SetMessageWindowTextSubstitutions(state->message, gUnk_080FABE0, state->substitutions);
                state->timer = 120; state->page = 18; state->phase = HorseRaceDialogueReady;
                break;
            case 16:
                SetMessageWindowTextSubstitutions(state->message, gUnk_080FAC24, state->substitutions);
                state->timer = 60; state->page = 19; state->phase = HorseRaceDialogueReady;
                break;
            case 17:
                SetMessageWindowTextSubstitutions(state->message, gUnk_080FAC8C, state->substitutions);
                state->timer = 255; state->page = 20; state->phase = HorseRaceDialogueReady;
                break;
            case 18:
                window = state->message; text = gUnk_080FAD08;
                firstChoice = gUnk_080FAD20; secondChoice = gUnk_080FAD28;
            showChoices:
                asm ("" : "+r" (window), "+r" (text), "+r" (firstChoice), "+r" (secondChoice));
                {
                    register u32 substitutionOffset asm("r4") = offsetof(HorseRaceDialogueState, substitutions);
                    asm ("" : "+r" (substitutionOffset));
                    register void * substitutions asm("r5") = (u8 *) state + substitutionOffset;
                    asm ("" : "+r" (substitutions));
                    ConfigureMessageWindowPromptChoices(window, text, firstChoice, secondChoice, 0, 0, substitutions);
                }
                {
                    register u32 phaseOffset asm("r0") = offsetof(HorseRaceDialogueState, phase);
                    asm ("" : "+r" (phaseOffset));
                    register u8 * phaseAddress asm("r1") = (u8 *) state + phaseOffset;
                    asm ("" : "+r" (phaseAddress));
                    *phaseAddress = HorseRaceDialogueChoosing;
                }
                break;
            case 19:
                window = state->message; text = gUnk_080FAD34;
                goto showTextWithTemporaryOffset;
            case 20:
                window = state->message; text = gUnk_080FAD50;
                goto showTextWithSavedOffset;
            case 24:
                window = state->message; text = gUnk_080FAD94;
            showTextWithTemporaryOffset:
            {
                register u32 offset asm("r3") = offsetof(HorseRaceDialogueState, substitutions);
                asm ("" : "+r" (offset));
                SetMessageWindowTextSubstitutions(window, text, (u8 *) state + offset);
                break;
            }
            case 28:
                window = state->message; text = gUnk_080FADDC;
            showTextWithSavedOffset:
            {
                register u32 offset asm("r4") = offsetof(HorseRaceDialogueState, substitutions);
                asm ("" : "+r" (offset));
                SetMessageWindowTextSubstitutions(window, text, (u8 *) state + offset);
                break;
            }
            case 31:
            {
                window = state->message; text = gUnk_080FAE00;
                asm ("" : "+r" (window), "+r" (text));
                register u32 offset asm("r3") = offsetof(HorseRaceDialogueState, substitutions);
                asm ("" : "+r" (offset));
                SetMessageWindowTextSubstitutions(window, text, (u8 *) state + offset);
                break;
            }
            }
        }
        break;
    case 1: case 2: case 4: case 5: case 6:
        break;
    }
}
