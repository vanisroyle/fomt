#include "new_game.hh"

#include <string.h>

extern char vtable_unk_080E5A68[];
extern char vtable_unk_080E5A88[];
extern char vtable_unk_080E5AA4[];
extern char vtable_unk_080E5AC0[];
extern char vtable_unk_080E5A3C[];
extern char vtable_unk_080E5A98[];
extern char vtable_unk_080E5AB4[];

EC void * __builtin_new(size_t size);
EC NewGameState * func_08005A58(void * self, void * param, u32 value);
EC void func_080007EC(void * self, int flags);
EC NewGameState * func_080041DC(void * self, void * param, void * owner);
EC NewGameState * func_08004EFC(void * self, void * a, void * b, u32 c, u32 d);
EC void func_08050D0C(void * self, int flags);
EC void func_080086BC(void * self, int flags);
EC void * func_080088DC(void * self);
EC void * func_08008920(void * self);
EC void * func_08008918(void * self);
EC void * func_08008940(void * self);
EC void * func_0800894C(void * self);
EC void func_08050D3C(void * self, void * a, void * b, void * c, void * d, void * e, void * f);

EC void func_08003C28(char * self) SECTION(".text.func_08003C28");
EC void func_08003C28(char * self)
{
    void * a = func_080088DC(self);
    void * b = func_08008920(self);
    void * c = func_08008918(self);
    void * d = func_08008940(self);
    void * e = func_0800894C(self);
    void * first = self + 0x10;
    void * last = self + 0x14;
    func_08050D3C(first, a, b, c, d, e, last);
}

EC NewGameScene * func_08004B58(NewGameScene * self) SECTION(".text.func_08004B58");
EC NewGameScene * func_08004B58(NewGameScene * self)
{
    self->vtable = vtable_unk_080E5A68;
    void ** transition = static_cast<void **>(__builtin_new(4));
    *transition = vtable_unk_080E5A3C;
    void * owner = transition;
    self->state = func_080041DC(__builtin_new(0x1A0), nullptr, &owner);
    return self;
}

EC NewGameScene * func_08004B94(NewGameScene * self, void * param, NewGameTransitionOwner & owner) SECTION(".text.func_08004B94");
EC NewGameScene * func_08004B94(NewGameScene * self, void * param, NewGameTransitionOwner & owner)
{
    self->vtable = vtable_unk_080E5A68;
    AUnk_0800080C * inner = owner.Release();
    self->state = func_080041DC(__builtin_new(0x1A0), param, &inner);
    delete owner.inner;
    return self;
}

EC void func_080050C8(NewGameChoiceLifetime * self, int flags) SECTION(".text.func_080050C8");
EC void func_080050C8(NewGameChoiceLifetime * self, int flags)
{
    self->vtable = vtable_unk_080E5A98;
    if (self->resource != nullptr)
        func_08050D0C(self->resource, 3);
    func_080086BC(self, flags);
}

EC NewGameScene * func_0800598C(NewGameScene * self, void * a, void * b, u32 c, u32 d) SECTION(".text.func_0800598C");
EC NewGameScene * func_0800598C(NewGameScene * self, void * a, void * b, u32 c, u32 d)
{
    self->vtable = vtable_unk_080E5AA4;
    self->state = func_08004EFC(__builtin_new(0x1A78), a, b, c, d);
    return self;
}

EC void func_08005B38(NewGameNameLifetime * self, int flags) SECTION(".text.func_08005B38");
EC void func_08005B38(NewGameNameLifetime * self, int flags)
{
    self->vtable = vtable_unk_080E5AB4;
    if (self->resource != nullptr)
        func_08050D0C(self->resource, 3);
    func_080086BC(self, flags);
}

EC bool func_08006C60(NewGameNameState * self) SECTION(".text.func_08006C60");
EC bool func_08006C60(NewGameNameState * self)
{
    int length = 0;
    int blanks = 0;
    strcpy(self->trimmed_name, self->name);
    for (int i = 0; i < 12; i++)
    {
        if (self->trimmed_name[i] == '_')
            blanks++;
        else
        {
            if (self->trimmed_name[i] == ' ')
                blanks++;
            length = i + 1;
        }
    }
    if (blanks == 12)
        length = 0;
    else
        for (int i = 0; i < length; i++)
            if (self->trimmed_name[i] == '_')
                self->trimmed_name[i] = ' ';
    self->trimmed_name[length] = 0;
    return length != 0;
}

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
