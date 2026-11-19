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
EC void DestroyMessageWindow(void * self, int flags);
EC void func_080086BC(void * self, int flags);
EC void * func_080088DC(void * self);
EC void * func_08008920(void * self);
EC void * func_08008918(void * self);
EC void * func_08008940(void * self);
EC void * func_0800894C(void * self);
EC unsigned int UpdateMessageWindow(void * self, void * a, void * b, void * c, void * d, void * e, void * f);
EC int GetMessageWindowPhase(void * self);
EC void OpenMessageWindow(void * self);
EC void CloseMessageWindow(void * self);
EC unsigned int GetMessageWindowSelection(void * self);
EC void ConfigureMessageWindowPromptChoices(void * self, char const * a, char const * b, char const * c, u32 d, u32 e, u32 f);
extern char gUnk_080E8998[];
extern char gUnk_080E89A4[];
extern char gUnk_080E89A8[];
extern char gUnk_080E8C2C[];
extern char gUnk_080E8C44[];
extern char gUnk_080E8C48[];

EC void func_08006ED8(NewGameNameState * self) SECTION(".text.func_08006ED8");
EC void func_08006ED8(NewGameNameState * self)
{
    if (self->unk_463C == 0)
        return;
    int previous = GetMessageWindowPhase(self->resource);
    void * resource = self->resource;
    void * a = func_080088DC(self);
    void * b = func_08008920(self);
    void * c = func_08008918(self);
    void * d = func_08008940(self);
    void * e = func_0800894C(self);
    void * f = &self->unk_45F0;
    switch (UpdateMessageWindow(resource, a, b, c, d, e, f))
    {
    case 0:
        if (self->unk_4638 != 0)
        {
            self->unk_463C = 0;
            self->unk_4638 = 0;
        }
        if (self->unk_4639 != 0)
        {
            self->unk_4639 = 0;
            self->unk_4636 = 1;
            strcpy(self->name, self->trimmed_name);
        }
        OpenMessageWindow(self->resource);
        break;
    case 1: break;
    case 2: break;
    case 3:
        switch (previous)
        {
        case 4: case 5: break;
        default:
            CloseMessageWindow(self->resource);
            break;
        case 1:
            if (self->unk_463C == 1)
                ConfigureMessageWindowPromptChoices(self->resource, gUnk_080E8C2C, gUnk_080E8C44, gUnk_080E8C48, 0, 0, 0);
            break;
        case 6:
            switch (GetMessageWindowSelection(self->resource))
            {
            case 1: self->unk_4639 = 1; break;
            case 2: self->unk_4638 = 1; break;
            }
            break;
        }
        break;
    case 4: break;
    case 5: break;
    case 6: break;
    }
}

EC void func_08005800(NewGameChoiceState * self) SECTION(".text.func_08005800");
EC void func_08005800(NewGameChoiceState * self)
{
    int previous = GetMessageWindowPhase(self->resource);
    void * resource = self->resource;
    void * a = func_080088DC(self);
    void * b = func_08008920(self);
    void * c = func_08008918(self);
    void * d = func_08008940(self);
    void * e = func_0800894C(self);
    void * f = &self->unk_1A30;
    switch (UpdateMessageWindow(resource, a, b, c, d, e, f))
    {
    case 0:
        if (self->unk_1A75 != 0)
            self->unk_1A74 = 1;
        OpenMessageWindow(self->resource);
        break;
    case 1: break;
    case 2: break;
    case 3:
        switch (previous)
        {
        case 4: case 5: break;
        default:
            CloseMessageWindow(self->resource);
            break;
        case 1:
            ConfigureMessageWindowPromptChoices(self->resource, gUnk_080E8998, gUnk_080E89A4, gUnk_080E89A8, 0, 0, 0);
            break;
        case 6:
            switch (GetMessageWindowSelection(self->resource))
            {
            case 0: break;
            case 1:
                self->MarkReady();
                self->Select(true);
                break;
            case 2:
                self->MarkReady();
                self->Select(false);
                break;
            case 3: break;
            case 4: break;
            case 5: break;
            case 6: break;
            }
            break;
        }
        break;
    case 4: break;
    case 5: break;
    case 6: break;
    }
}
EC void SetMessageWindowTextOptions(void * self, char const * text, int a, int b);

extern char gUnk_080E8770[];
extern char gUnk_080E877C[];
extern char gUnk_080E878C[];
extern char gUnk_080E87A4[];
extern char gUnk_080E87FC[];
extern char gUnk_080E8828[];
extern char gUnk_080E8878[];
extern char gUnk_080E86C0[];

EC void func_08003EEC(NewGameMenuState * self) SECTION(".text.func_08003EEC");
EC void func_08003EEC(NewGameMenuState * self)
{
    char const * text = nullptr;
    switch (self->selection)
    {
    case 0:
        text = self->values[0] != 0 ? gUnk_080E8770 : gUnk_080E877C;
        break;
    case 1:
        text = !self->names[self->values[1]].Empty() ? self->names[self->values[1]].Get() : gUnk_080E878C;
        break;
    case 2: text = gUnk_080E87A4; break;
    case 3: text = gUnk_080E87FC; break;
    case 4: text = gUnk_080E8828; break;
    case 5: text = gUnk_080E8878; break;
    }
    SetMessageWindowTextOptions(reinterpret_cast<char *>(self) + 0x10, text != nullptr ? text : gUnk_080E86C0, 0, 0);
}

EC void func_08006858(NewGameNameState * self) SECTION(".text.func_08006858");
EC void func_08006858(NewGameNameState * self)
{
    if (self->unk_463C == 0)
    {
        NewGameSpriteList * list = static_cast<NewGameSpriteList *>(func_08008920(self));
        u32 const & attributes = self->cursor_attributes;
        unsigned int index = list->count;
        if (index < 128)
        {
            NewGameSprite & sprite = list->At(index);
            sprite.attributes = attributes;
            sprite.attr2 = self->cursor_attr2;
            list->count = index + 1;
        }
    }
    NewGameSpriteList * list = static_cast<NewGameSpriteList *>(func_08008920(self));
    u32 const & attributes = self->marker_attributes;
    unsigned int index = list->count;
    if (index < 128)
    {
        NewGameSprite & sprite = list->At(index);
        sprite.attributes = attributes;
        sprite.attr2 = self->marker_attr2;
        list->count = index + 1;
    }
}

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
    UpdateMessageWindow(first, a, b, c, d, e, last);
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
        DestroyMessageWindow(self->resource, 3);
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
        DestroyMessageWindow(self->resource, 3);
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
