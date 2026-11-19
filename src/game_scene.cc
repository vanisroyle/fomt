#include "prelude.h"

struct GameSceneLayout
{
    void const * vtable;
    void * state;
    u8 script_engine[0x350];
    u8 unk_358[0x20];
    u8 unk_378[4];
};

extern void const * vtable_unk_080E5BF8[];
extern GameSceneLayout * gUnk_0300040C;
EC void func_080D7E64(void * self, int flags);
EC void DestroyGameScript(void * self, int flags) ALIAS(_._13AScriptEngine);
EC void func_080D4480(void * self, int flags);
EC void func_080007EC(void * self, int flags);

EC void func_08010158(GameSceneLayout * self, int flags)
{
    self->vtable = vtable_unk_080E5BF8;
    gUnk_0300040C = nullptr;
    func_080D7E64(self->unk_378, 2);
    DestroyGameScript(self->script_engine, 2);
    if (self->state != nullptr)
        func_080D4480(self->state, 3);
    func_080007EC(self, flags);
}
