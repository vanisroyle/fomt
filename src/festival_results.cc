#include "prelude.h"
#include <new>

struct AnimalFestivalScore
{
    u8 entryId;
    u16 score;
};

struct FestivalResultsController
{
    void * vtable;
    void * screen;
    void * continuation;
};

EC u8 vtable_unk_080E7934[];
EC void * InitializeFestivalResultsScreen(void *, void *);

EC SECTION(".text.festival_results.0805218C") FestivalResultsController * InitializeFestivalResultsController(FestivalResultsController * controller, void ** continuation, void * game) // 0x0805218C
{
    controller->vtable = vtable_unk_080E7934;
    controller->screen = InitializeFestivalResultsScreen(operator new(0x8c0), game);
    void * next = *continuation;
    *continuation = nullptr;
    controller->continuation = next;
    return controller;
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
