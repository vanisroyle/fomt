#include "g1_minigame.hh"

EC void * __builtin_new(u32);
EC void * func_080A220C(void *, void const *);
EC G1SceneOwner func_080A25B0(void *);
EC char const vtable_unk_080E8278[];

EC SECTION(".text.g1_080A3744") G1SpriteFeedingSceneWrapper * InitializeSpriteFeedingSceneWrapper(G1SpriteFeedingSceneWrapper * self, AUnk_0800080C * & next_scene, void const * parameters)
{
    self->vtable = vtable_unk_080E8278;
    self->game = func_080A220C(__builtin_new(0x228), parameters);
    AUnk_0800080C * next = next_scene;
    next_scene = nullptr;
    self->next_scene = next;
    return self;
}

EC SECTION(".text.g1_080A3774") G1SceneOwner RunSpriteFeedingSceneWrapper(G1SpriteFeedingSceneWrapper * self)
{
    func_080A25B0(self->game);
    AUnk_0800080C * next = self->next_scene;
    self->next_scene = nullptr;
    return G1SceneOwner(next);
}
