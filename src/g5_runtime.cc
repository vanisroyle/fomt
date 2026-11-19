#include "prelude.h"

struct RuntimeCommand
{
    u32 kind;
    u32 unk_04;
    u32 unk_08;
    u32 args[5];
};

struct RuntimeState
{
    u32 unk_00[13];
    u32 read;
    u32 write;
    RuntimeCommand commands[10];
    u32 busy;
    u32 status;
    u32 unk_184;
};

EC RuntimeState * gUnk_03000420;
EC RuntimeCommand * gUnk_0300041C;

EC void func_080D0B88();
EC void func_080D0BA8(RuntimeCommand * command);
EC void func_080D0B14(u32, u32);
EC void func_080D0ABC();
EC void func_080D0AE0();
EC void func_080D0A94(u32 status);

EC SECTION(".text.g5_init") void func_080D02CC(RuntimeState * state)
{
    gUnk_03000420 = state;
    state->unk_00[0] = 0;
    func_080D0B88();
    gUnk_03000420->unk_00[1] = 0;
    gUnk_03000420->unk_00[2] = 0;
}

EC SECTION(".text.g5_init") void func_080D02EC()
{
}

EC SECTION(".text.g5_init") void func_080D02F0()
{
    RuntimeCommand command;
    RuntimeState * state = gUnk_03000420;
    if (state->unk_00[0] == 0)
    {
        state->unk_00[0] = 1;
        state->unk_00[5] = 0;
        state->unk_00[6] = 0;
        state->unk_00[7] = 0;
        state->unk_00[8] = 1;
        state->status = 0;
        state->unk_184 = 0;
        state->busy = 0;
        state->unk_00[3] = 0;
        state->unk_00[4] = 0;
        command.kind = 0;
        func_080D0BA8(&command);
        func_080D0B14(0, 0);
        func_080D0ABC();
    }
}

EC SECTION(".text.g5_init") void func_080D0340()
{
    RuntimeState * state = gUnk_03000420;
    if (state->unk_00[0] != 0)
    {
        state->unk_00[0] = 0;
        func_080D0AE0();
    }
}

EC void func_080D0B88()
{
    RuntimeState * state = gUnk_03000420;
    state->read = 0;
    state->write = 0;
    gUnk_0300041C = state->commands;
    state->commands[0].kind = 0;
}

EC void func_080D0BA8(RuntimeCommand * command)
{
    RuntimeState * state = gUnk_03000420;
    state->write = (state->write + 1) % 10;
    command->unk_04 = 0;
    command->unk_08 = 0;
    RuntimeCommand * dest = (RuntimeCommand *) ((u8 *) state + state->write * sizeof(RuntimeCommand));
    dest = (RuntimeCommand *) ((u8 *) dest + 0x3c);
    *dest = *command;
}

EC void func_080D0BE0()
{
    RuntimeState * state = gUnk_03000420;
    if (state->read != state->write)
    {
        state->read = (state->read + 1) % 10;
        gUnk_0300041C = &state->commands[state->read];
    }
}

EC bool func_080D0C10()
{
    return gUnk_03000420->read == gUnk_03000420->write;
}

EC void func_080D0C2C()
{
    RuntimeState * state = gUnk_03000420;
    state->write = (state->read + 1) % 10;
    state->commands[state->write].kind = 0;
    state->read = state->write;
    gUnk_0300041C = &state->commands[state->write];
}

EC SECTION(".text.g5_commands") void func_080D0A14()
{
    RuntimeCommand * command = gUnk_0300041C;
    if (command->kind == 1 && command->unk_04 <= 2)
    {
        (void) *(volatile u32 *) 0x04000150;
        *(volatile u32 *) 0x04000154 = gUnk_03000420->unk_00[1];
        *(volatile u16 *) 0x04000158 = 0x20;
        command->unk_04++;
    }
}

EC SECTION(".text.g5_commands") void func_080D0A54(u32 status)
{
    gUnk_0300041C->kind = 0;
    gUnk_03000420->status = status;
}

EC SECTION(".text.g5_commands") void func_080D0A74()
{
    gUnk_0300041C->kind = 0;
    gUnk_03000420->status = 0;
}

EC SECTION(".text.g5_commands") void func_080D0A94(u32 status)
{
    gUnk_03000420->busy = 1;
    func_080D0A54(status);
    gUnk_03000420->busy = 0;
}

EC SECTION(".text.g5_commands") void func_080D0ABC()
{
    u16 ime = *(volatile u16 *) 0x04000208;
    *(volatile u16 *) 0x04000208 = 0;
    *(volatile u16 *) 0x04000200 |= 0x80;
    *(volatile u16 *) 0x04000208 = ime;
}

EC SECTION(".text.g5_commands") void func_080D0AE0()
{
    u16 ime = *(volatile u16 *) 0x04000208;
    *(volatile u16 *) 0x04000208 = 0;
    *(volatile u16 *) 0x04000200 &= 0xff7f;
    *(volatile u16 *) 0x04000202 = 0x80;
    *(volatile u16 *) 0x04000208 = ime;
}

EC SECTION(".text.g5_commands") void func_080D0B14(u32 reset, u32 mode)
{
    u16 ime = *(volatile u16 *) 0x04000208;
    *(volatile u16 *) 0x04000208 = 0;
    if (reset != 0)
        *(volatile u16 *) 0x04000134 = 0x8000;
    *(volatile u16 *) 0x04000134 = 0xc000;
    if (mode == 0)
        *(volatile u16 *) 0x04000158 = 0;
    else
        *(volatile u16 *) 0x04000158 = 0x30;
    (void) *(volatile u32 *) 0x04000150;
    *(volatile u32 *) 0x04000154 = 0;
    *(volatile u16 *) 0x04000140 = 0x47;
    *(volatile u16 *) 0x04000202 = 0x80;
    *(volatile u16 *) 0x04000208 = ime;
}

EC SECTION(".text.g5_requests") void func_080D0444(u32 first, u32 second)
{
    gUnk_03000420->unk_00[1] = first;
    gUnk_03000420->unk_00[2] = second;
}

EC SECTION(".text.g5_requests") void func_080D0454()
{
    RuntimeCommand command;
    gUnk_03000420->unk_00[5] = 1;
    command.kind = 1;
    func_080D0BA8(&command);
    command.kind = 3;
    command.args[1] = (u32) &gUnk_03000420->unk_00[9];
    command.args[3] = 2;
    *(u16 *) &command.args[4] = 0;
    func_080D0BA8(&command);
}

EC SECTION(".text.g5_queries") u32 func_080D0678()
{
    RuntimeState * state = gUnk_03000420;
    return ((u8 *) state)[0x2c] | ((u8 *) state)[0x2d] << 8 |
        ((u8 *) state)[0x2e] << 16 | ((u8 *) state)[0x2f] << 24;
}

EC SECTION(".text.g5_queries") void func_080D06A8()
{
    func_080D0A94(11);
}

EC SECTION(".text.g5_queries") bool func_080D06B4()
{
    return gUnk_03000420->unk_00[5] == 0;
}

EC SECTION(".text.g5_queries") u32 func_080D06D0()
{
    return gUnk_03000420->unk_00[6];
}
