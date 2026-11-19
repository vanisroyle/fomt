#include "intro_scene.hh"

extern char gIntroSceneVtable[] __asm__("vtable_unk_080E5A18");

EC void * __builtin_new(size_t size);
EC IntroSceneTask * IntroScene_CreateTask(void * self) __asm__("func_08000914");
EC void Scene_DestroyBase(void * self, int destructorFlags);

EC IntroSceneOwner * IntroScene_Initialize(IntroSceneOwner * self) SECTION(".text.func_080036F8");
EC IntroSceneOwner * IntroScene_Initialize(IntroSceneOwner * self)
{
    self->vtable = gIntroSceneVtable;
    self->task = IntroScene_CreateTask(__builtin_new(0x6D34));
    return self;
}

EC void IntroScene_Destroy(IntroSceneOwner * self, int destructorFlags) SECTION(".text.func_0800371C");
EC void IntroScene_Destroy(IntroSceneOwner * self, int destructorFlags)
{
    self->vtable = gIntroSceneVtable;
    delete self->task;
    Scene_DestroyBase(self, destructorFlags);
}

EC IntroSceneOwner * func_080036F8(IntroSceneOwner * self) ALIAS(IntroScene_Initialize);
