#include "intro_scene.hh"

extern char vtable_unk_080E5A18[];

EC void * __builtin_new(size_t size);
EC IntroSceneState * func_08000914(void * self);
EC void func_080007EC(void * self, int flags);

EC IntroScene * func_080036F8(IntroScene * self) SECTION(".text.func_080036F8");
EC IntroScene * func_080036F8(IntroScene * self)
{
    self->vtable = vtable_unk_080E5A18;
    self->state = func_08000914(__builtin_new(0x6D34));
    return self;
}

EC void func_0800371C(IntroScene * self, int flags) SECTION(".text.func_0800371C");
EC void func_0800371C(IntroScene * self, int flags)
{
    self->vtable = vtable_unk_080E5A18;
    delete self->state;
    func_080007EC(self, flags);
}
