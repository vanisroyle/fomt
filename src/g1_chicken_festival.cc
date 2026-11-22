#include "g1_minigame.hh"
#include "chicken_festival.hh"
#include "coop.hh"
#include <algorithm>

struct PACKED G1FestivalElapsedDays
{
    u32 remainder : 3;
    u32 weeks : 5;
};

struct G1ChickenFestivalGame;
EC G1ChickenFestivalGame * CreateChickenFestivalWrapper(G1ChickenFestivalGame *, ChickenFestivalEntrant const *);
EC void DestroyChickenFestivalWrapper(G1ChickenFestivalGame *, u32);
EC G1SceneOwner RunChickenFestivalWrapper(G1ChickenFestivalGame *);
EC bool DidWrappedChickenFestivalPlayerWin(G1ChickenFestivalGame *);

struct G1ChickenFestivalGame
{
    G1ChickenFestivalGame() {}
    ~G1ChickenFestivalGame() { DestroyChickenFestivalWrapper(this, 2); }

    void const * vtable;
    void * state;
};

EC SECTION(".text.g1_080A37A8") G1SceneOwner RunChickenFestivalScene(G1SpriteMinigameScene * self)
{
    int index = reinterpret_cast<Coop *>(static_cast<u8 *>(self->game_state) + 0x410)->GetUnkEnt();
    if (index == -1)
    {
        AUnk_0800080C * next = self->next_scene;
        self->next_scene = nullptr;
        G1SceneOwner owner(next);
        return owner.Move();
    }
    Chicken * chicken = reinterpret_cast<Coop *>(static_cast<u8 *>(self->game_state) + 0x410)->GetChicken(index);
    if (chicken == nullptr)
    {
        AUnk_0800080C * next = self->next_scene;
        self->next_scene = nullptr;
        G1SceneOwner owner(next);
        return owner.Move();
    }
    ChickenFestivalEntrant entrant;
    G1FestivalElapsedDays * elapsed = reinterpret_cast<G1FestivalElapsedDays *>(static_cast<u8 *>(self->game_state) + 0x10);
    entrant.unknown_10 = std::min<u32>(200, elapsed->weeks * 7 + elapsed->remainder);
    entrant.name = chicken->GetName();
    entrant.affection = chicken->GetAffection();
    G1ChickenFestivalGame game;
    CreateChickenFestivalWrapper(&game, &entrant);
    RunChickenFestivalWrapper(&game);
    if (DidWrappedChickenFestivalPlayerWin(&game))
        chicken->SetFestivalWinner();
    G1SpriteMinigameResult * result = static_cast<G1SpriteMinigameResult *>(self->next_scene);
    result->succeeded = DidWrappedChickenFestivalPlayerWin(&game);
    AUnk_0800080C * next = self->next_scene;
    self->next_scene = nullptr;
    G1SceneOwner owner(next);
    return owner.Move();
}
