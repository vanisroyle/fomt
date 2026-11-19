#include "intro_scene.hh"

extern char gIntroSceneVtable[] __asm__("vtable_unk_080E5A18");

EC void * __builtin_new(size_t size);
EC IntroTask * CreateIntroTask(void * self) __asm__("func_08000914");
EC void DestroySceneBase(void * self, int destructorFlags) __asm__("func_080007EC");

EC IntroScene * InitializeIntroScene(IntroScene * self) SECTION(".text.func_080036F8");
EC IntroScene * InitializeIntroScene(IntroScene * self)
{
    self->vtable = gIntroSceneVtable;
    self->task = CreateIntroTask(__builtin_new(0x6D34));
    return self;
}

EC void DestroyIntroScene(IntroScene * self, int destructorFlags) SECTION(".text.func_0800371C");
EC void DestroyIntroScene(IntroScene * self, int destructorFlags)
{
    self->vtable = gIntroSceneVtable;
    delete self->task;
    DestroySceneBase(self, destructorFlags);
}

EC IntroScene * func_080036F8(IntroScene * self) ALIAS(InitializeIntroScene);
