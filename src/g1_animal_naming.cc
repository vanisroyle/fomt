#include "g1_minigame.hh"
#include "g1_roster.hh"
#include "farm.hh"
#include "new_game.hh"

EC char gUnk_08104E0C[], gUnk_08104E14[], gUnk_08104E1C[], gUnk_08104E24[], gUnk_08104E30[], gUnk_08104E38[];
EC G1SceneOwner func_080070D4(NameEntryScene *);
EC G1NamedNpcState * GetActiveScheduleNpc(G1NpcSchedule *);
EC void SetNamedNpcName(G1NamedNpcState *, char const *);
EC void func_0800E9A8(void *, char const *);

struct G1NamingGame
{
    G1NamingGame(void * prompt) { NameEntryScene_Initialize(&scene, prompt, 0); }
    ~G1NamingGame() { NameEntryScene_Destroy(&scene, 2); }
    NameEntryScene * GetScene() { return reinterpret_cast<NameEntryScene *>(&scene); }
    NewGameSceneTaskOwner scene;
};

EC SECTION(".text.g1_080A3CF4") G1SceneOwner RunAnimalNamingScene(G1AnimalNamingScene * self)
{
    void * prompt = nullptr;
    switch (self->target_kind)
    {
    case 0: prompt = gUnk_08104E0C; break;
    case 1: prompt = gUnk_08104E14; break;
    case 2: prompt = gUnk_08104E1C; break;
    case 3: prompt = gUnk_08104E24; break;
    case 4: prompt = gUnk_08104E30; break;
    case 5: prompt = gUnk_08104E38; break;
    default:
        AUnk_0800080C * next = self->next_scene;
        self->next_scene = nullptr;
        return G1SceneOwner(next);
    }
    G1NamingGame game(prompt);
    func_080070D4(game.GetScene());
    Animal * animal;
    switch (self->target_kind)
    {
    case 0:
        animal = reinterpret_cast<Farm *>(static_cast<u8 *>(self->game_state) + 0x14)->GetHorse();
        goto name_animal;
    case 1:
        animal = reinterpret_cast<Barn *>(static_cast<u8 *>(self->game_state) + 0x5F0)->GetCow(self->animal_index);
        goto name_animal;
    case 2:
        animal = reinterpret_cast<Barn *>(static_cast<u8 *>(self->game_state) + 0x5F0)->GetSheep(self->animal_index);
        goto name_animal;
    case 3:
        animal = reinterpret_cast<Coop *>(static_cast<u8 *>(self->game_state) + 0x410)->GetChicken(self->animal_index);
    name_animal:
        if (animal != nullptr)
            animal->SetName(NameEntryScene_GetName(game.GetScene()));
        break;
    case 4:
        {
            G1NamedNpcState * npc = GetActiveScheduleNpc(reinterpret_cast<G1NpcSchedule *>(static_cast<u8 *>(self->game_state) + 0x1CD4));
            if (npc != nullptr)
                SetNamedNpcName(npc, NameEntryScene_GetName(game.GetScene()));
        }
        break;
    case 5:
        func_0800E9A8(static_cast<u8 *>(self->game_state) + 0x1BD8, NameEntryScene_GetName(game.GetScene()));
        break;
    default:
        AUnk_0800080C * next = self->next_scene;
        self->next_scene = nullptr;
        return G1SceneOwner(next);
    }
    AUnk_0800080C * next = self->next_scene;
    self->next_scene = nullptr;
    return G1SceneOwner(next);
}
