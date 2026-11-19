#include "g1_minigame.hh"

EC void Scene_DestroyBase(void *, int);
EC char const vtable_unk_080E8288[];
EC char const vtable_unk_080E8298[];
EC char const vtable_unk_080E82A8[];

EC SECTION(".text.g1_080A3BF4") void DestroySpriteWateringMinigameScene(G1SpriteMinigameScene * self, int flags)
{
    self->vtable = vtable_unk_080E8288;
    delete self->next_scene;
    Scene_DestroyBase(self, flags);
}

EC SECTION(".text.g1_080A3C40") void DestroySpriteHarvestMinigameScene(G1SpriteMinigameScene * self, int flags)
{
    self->vtable = vtable_unk_080E8298;
    delete self->next_scene;
    Scene_DestroyBase(self, flags);
}

EC SECTION(".text.g1_080A3C8C") void DestroySpriteFeedingMinigameScene(G1SpriteMinigameScene * self, int flags)
{
    self->vtable = vtable_unk_080E82A8;
    delete self->next_scene;
    Scene_DestroyBase(self, flags);
}
