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
EC FestivalResultsScreen * InitializeAnimalFestivalScreen(void *, void *, u8);
EC void RunAnimalFestivalScreen(FestivalScene **, FestivalResultsScreen *);
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

EC SECTION(".text.animal_festival.08057E90") void InitializeHorseRaceLineup(HorseRaceLineup * lineup, u8 specialRace, u32 kind)
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

EC SECTION(".text.animal_festival.08057F24") void AdvanceHorseRaceHeat(HorseRaceLineup * lineup, u32 kind)
{
    lineup->heat++;
    PrepareHorseRaceHeat(lineup, kind);
}

EC SECTION(".text.animal_festival.08057F4C") bool AreHorseRaceHeatsFinished(HorseRaceLineup * lineup)
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

EC SECTION(".text.animal_festival.08057DD8") FestivalResultsController * InitializeAnimalFestivalController(FestivalResultsController * controller, FestivalScene ** continuation, void * game, u8 kind)
{
    controller->vtable = vtable_unk_080E7960;
    controller->screen = InitializeAnimalFestivalScreen(operator new(0x23a8), game, kind);
    FestivalScene * next = *continuation;
    *continuation = nullptr;
    controller->continuation = next;
    return controller;
}

EC SECTION(".text.animal_festival.08057E1C") void DestroyAnimalFestivalController(FestivalResultsController * controller, u32 flags)
{
    controller->vtable = vtable_unk_080E7960;
    FestivalScene * next = controller->continuation;
    if (next != nullptr) next->vtable->destroy(next, 3);
    FestivalResultsScreen * screen = controller->screen;
    if (screen != nullptr) screen->vtable->destroy(screen, 3);
    Scene_DestroyBase(controller, flags);
}

EC SECTION(".text.animal_festival.08057E5C") FestivalScene ** RunAnimalFestivalController(FestivalScene ** result, FestivalResultsController * controller)
{
    FestivalScene * completed;
    RunAnimalFestivalScreen(&completed, controller->screen);
    if (completed != nullptr) completed->vtable->destroy(completed, 3);
    FestivalScene * next = controller->continuation;
    controller->continuation = nullptr;
    *result = next;
    return result;
}
