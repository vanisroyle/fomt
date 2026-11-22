#include "g1_minigame.hh"
#include "g1_roster.hh"

struct G1SpriteFeedingParameters
{
    G1SpriteFeedingParameters() : sprite(2), experience(18) {}

    u32 sprite;
    u8 experience;
};

struct G1ChickenFeedingGame;

EC HarvestSprite const * GetRosterHarvestSpriteByIdConst(G1NpcRoster const *, u32);
EC G1ChickenFeedingGame * CreateChickenFeedingWrapper(G1ChickenFeedingGame *, G1SpriteFeedingParameters const *);
EC void DestroyChickenFeedingWrapper(G1ChickenFeedingGame *, u32);
EC G1SceneOwner RunChickenFeedingWrapper(G1ChickenFeedingGame *);
EC bool DidChickenFeedingSucceed(G1ChickenFeedingGame *);
EC u32 GetChickenFeedingScore(G1ChickenFeedingGame *);
EC u32 GameProgress_RaiseHalfCounter(void *, u32);

struct G1ChickenFeedingGame
{
    G1ChickenFeedingGame(G1SpriteFeedingParameters const & parameters) { CreateChickenFeedingWrapper(this, &parameters); }
    ~G1ChickenFeedingGame() { DestroyChickenFeedingWrapper(this, 2); }

    void const * vtable;
    void * state;
};

EC SECTION(".text.g1_080A3900") G1SceneOwner RunSpriteFeedingMinigameScene(G1SpriteMinigameScene * self)
{
    G1SpriteFeedingParameters parameters;
    switch (self->npc_id)
    {
    case 36: parameters.sprite = 0; break;
    case 37: parameters.sprite = 2; break;
    case 38: parameters.sprite = 3; break;
    case 39: parameters.sprite = 4; break;
    case 40: parameters.sprite = 5; break;
    case 41: parameters.sprite = 6; break;
    case 42: parameters.sprite = 1; break;
    }

    G1NpcRoster * roster = reinterpret_cast<G1NpcRoster *>(static_cast<u8 *>(self->game_state) + 0x1CD4);
    HarvestSprite * sprite = const_cast<HarvestSprite *>(GetRosterHarvestSpriteByIdConst(roster, self->npc_id));
    parameters.experience = sprite->GetMinigameExp(HarvestSprite::TASK_ANIMALS);

    G1ChickenFeedingGame game(parameters);
    RunChickenFeedingWrapper(&game);
    bool succeeded = DidChickenFeedingSucceed(&game);
    sprite->SetPlayedMinigame(HarvestSprite::TASK_ANIMALS, succeeded);
    static_cast<G1SpriteMinigameResult *>(self->next_scene)->succeeded = succeeded;
    GameProgress_RaiseHalfCounter(self->game_state, GetChickenFeedingScore(&game));
    AUnk_0800080C * next = self->next_scene;
    self->next_scene = nullptr;
    return G1SceneOwner::Adopt(next);
}
