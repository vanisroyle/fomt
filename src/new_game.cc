#include "new_game.hh"

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
