#include "prelude.h"
#include <new>
#include <stdlib.h>

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
    u8 racers[32];
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
EC void * HardwareContext_GetOamBuffer(void *);
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
