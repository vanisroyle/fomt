#include "prelude.h"
#include <new>

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
EC void func_080007EC(void *, u32);

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
    func_080007EC(controller, flags);
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
