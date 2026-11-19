#include "new_game.hh"

extern char vtable_unk_080E5A68[];
extern char vtable_unk_080E5A88[];
extern char vtable_unk_080E5AA4[];
extern char vtable_unk_080E5AC0[];

EC void * __builtin_new(size_t size);
EC NewGameState * func_08005A58(void * self, void * param, u32 value);
EC void func_080007EC(void * self, int flags);

EC void func_08004BDC(NewGameScene * self, int flags) SECTION(".text.func_08004BDC");
EC void func_08004BDC(NewGameScene * self, int flags)
{
    self->vtable = vtable_unk_080E5A68;
    delete self->state;
    func_080007EC(self, flags);
}

EC void * func_08004C48(void ** self) SECTION(".text.func_08004C48");
EC void * func_08004C48(void ** self)
{
    *self = vtable_unk_080E5A88;
    return self;
}

EC void func_08004C54(void ** self, int flags) SECTION(".text.func_08004C54");
EC void func_08004C54(void ** self, int flags)
{
    *self = vtable_unk_080E5A88;
    func_080007EC(self, flags);
}

EC void func_080059D0(NewGameScene * self, int flags) SECTION(".text.func_080059D0");
EC void func_080059D0(NewGameScene * self, int flags)
{
    self->vtable = vtable_unk_080E5AA4;
    delete self->state;
    func_080007EC(self, flags);
}

EC NewGameScene * func_08007078(NewGameScene * self, void * param, u32 value) SECTION(".text.func_08007078");
EC NewGameScene * func_08007078(NewGameScene * self, void * param, u32 value)
{
    self->vtable = vtable_unk_080E5AC0;
    self->state = func_08005A58(__builtin_new(0x4640), param, value);
    return self;
}

EC void func_080070A4(NewGameScene * self, int flags) SECTION(".text.func_080070A4");
EC void func_080070A4(NewGameScene * self, int flags)
{
    self->vtable = vtable_unk_080E5AC0;
    delete self->state;
    func_080007EC(self, flags);
}

EC void func_08008724(void * self);

EC void func_08005254(void * self) SECTION(".text.func_08005254");
EC void func_08005254(void * self)
{
    func_08008724(self);
}

EC void func_08005260(NewGameChoiceState * self) SECTION(".text.func_08005260");
EC void func_08005260(NewGameChoiceState * self)
{
    self->unk_1A75 = 0;
    self->unk_1A74 = 0;
}

EC u8 func_08005A3C(NewGameChoiceScene * self) SECTION(".text.func_08005A3C");
EC u8 func_08005A3C(NewGameChoiceScene * self)
{
    return self->state->choice;
}

EC void func_08006334(NewGameNameState * self) SECTION(".text.func_08006334");
EC void func_08006334(NewGameNameState * self)
{
    self->unk_4639 = 0;
    self->unk_4638 = 0;
    self->unk_4636 = 0;
    self->unk_4637 = 0;
    self->row = 0;
    self->column = 0;
    self->page = 0;
    self->length = 0;
    self->unk_463C = 0;
}

EC void func_08006C28(NewGameNameState * self, u8 index) SECTION(".text.func_08006C28");
EC void func_08006C28(NewGameNameState * self, u8 index)
{
    for (; index < 11; index++)
        self->name[index] = self->name[index + 1];
    self->name[11] = '_';
}

EC char * func_08007110(NewGameNameScene * self) SECTION(".text.func_08007110");
EC char * func_08007110(NewGameNameScene * self)
{
    return self->state->name;
}
