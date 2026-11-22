#include "g5_link.hh"

EC SECTION(".text.g5_080D0644") void Joybus_QueueWordExchange(u32 send_word, u32 require_handshake) // 0x080D0644
{
    register JoybusState ** state_slot asm("r4") = &gJoybusState;
    asm ("" : "+l" (state_slot));
    u8 * bytes = (u8 *) *state_slot;
    bytes[0x28] = send_word;
    bytes = (u8 *) *state_slot;
    register u32 next_byte asm("r3") = send_word >> 8;
    asm ("" : "+l" (next_byte));
    bytes[0x29] = next_byte;
    bytes = (u8 *) *state_slot;
    next_byte = send_word >> 16;
    asm ("" : "+l" (next_byte));
    bytes[0x2a] = next_byte;
    bytes = (u8 *) *state_slot;
    send_word >>= 24;
    asm ("" : "+l" (send_word));
    bytes[0x2b] = send_word;
    (*state_slot)->unknown_00[12] = require_handshake;
    Joybus_QueueExchangeCommands();
}

EC SECTION(".text.g5_init") void InitializeJoybusState(JoybusState * state) // 0x080D02CC
{
    gJoybusState = state;
    state->unknown_00[JOYBUS_ENABLED] = 0;
    ResetJoybusCommandQueue();
    gJoybusState->unknown_00[JOYBUS_HANDSHAKE_SEND] = 0;
    gJoybusState->unknown_00[JOYBUS_HANDSHAKE_EXPECTED] = 0;
}

EC SECTION(".text.g5_init") void func_080D02EC()
{
}

EC SECTION(".text.g5_init") void StartJoybus() // 0x080D02F0
{
    JoybusCommand command;
    JoybusState * state = gJoybusState;
    if (state->unknown_00[JOYBUS_ENABLED] == 0)
    {
        state->unknown_00[JOYBUS_ENABLED] = 1;
        state->unknown_00[5] = 0;
        state->unknown_00[6] = 0;
        state->unknown_00[7] = 0;
        state->unknown_00[JOYBUS_TRANSFER_TOTAL] = 1;
        state->status = 0;
        state->transferred_halfwords = 0;
        state->busy = 0;
        state->unknown_00[3] = 0;
        state->unknown_00[4] = 0;
        command.kind = 0;
        EnqueueJoybusCommand(&command);
        ConfigureJoybus(0, 0);
        EnableJoybusSerialInterrupt();
    }
}

EC SECTION(".text.g5_init") void StopJoybus() // 0x080D0340
{
    JoybusState * state = gJoybusState;
    if (state->unknown_00[JOYBUS_ENABLED] != 0)
    {
        state->unknown_00[JOYBUS_ENABLED] = 0;
        DisableJoybusSerialInterrupt();
    }
}

EC void ResetJoybusCommandQueue() // 0x080D0B88
{
    JoybusState * state = gJoybusState;
    state->read_index = 0;
    state->write_index = 0;
    gCurrentJoybusCommand = state->commands;
    state->commands[0].kind = 0;
}

EC void EnqueueJoybusCommand(JoybusCommand * command) // 0x080D0BA8
{
    JoybusState * state = gJoybusState;
    state->write_index = (state->write_index + 1) % 10;
    command->progress = 0;
    command->unknown_08 = 0;
    JoybusCommand * dest = (JoybusCommand *) ((u8 *) state + state->write_index * sizeof(JoybusCommand));
    dest = (JoybusCommand *) ((u8 *) dest + 0x3c);
    *dest = *command;
}

EC void AdvanceJoybusCommandQueue() // 0x080D0BE0
{
    JoybusState * state = gJoybusState;
    if (state->read_index != state->write_index)
    {
        state->read_index = (state->read_index + 1) % 10;
        gCurrentJoybusCommand = &state->commands[state->read_index];
    }
}

EC u32 IsJoybusCommandQueueEmpty() // 0x080D0C10
{
    return gJoybusState->read_index == gJoybusState->write_index;
}

EC void CancelJoybusCommands() // 0x080D0C2C
{
    JoybusState * state = gJoybusState;
    state->write_index = (state->read_index + 1) % 10;
    state->commands[state->write_index].kind = 0;
    state->read_index = state->write_index;
    gCurrentJoybusCommand = &state->commands[state->write_index];
}

EC SECTION(".text.g5_commands") void RetryJoybusHandshake() // 0x080D0A14
{
    JoybusCommand * command = gCurrentJoybusCommand;
    if (command->kind == 1 && command->progress <= 2)
    {
        (void) *(volatile u32 *) 0x04000150;
        *(volatile u32 *) 0x04000154 = gJoybusState->unknown_00[JOYBUS_HANDSHAKE_SEND];
        *(volatile u16 *) 0x04000158 = 0x20;
        command->progress++;
    }
}

EC SECTION(".text.g5_commands") void CompleteJoybusCommand(u32 status) // 0x080D0A54
{
    gCurrentJoybusCommand->kind = 0;
    gJoybusState->status = status;
}

EC SECTION(".text.g5_commands") void ClearJoybusCommandStatus() // 0x080D0A74
{
    gCurrentJoybusCommand->kind = 0;
    gJoybusState->status = 0;
}

EC SECTION(".text.g5_commands") void SetJoybusStatusWhileBusy(u32 status) // 0x080D0A94
{
    gJoybusState->busy = 1;
    CompleteJoybusCommand(status);
    gJoybusState->busy = 0;
}

EC SECTION(".text.g5_commands") void EnableJoybusSerialInterrupt() // 0x080D0ABC
{
    u16 ime = *(volatile u16 *) 0x04000208;
    *(volatile u16 *) 0x04000208 = 0;
    *(volatile u16 *) 0x04000200 |= 0x80;
    *(volatile u16 *) 0x04000208 = ime;
}

EC SECTION(".text.g5_commands") void DisableJoybusSerialInterrupt() // 0x080D0AE0
{
    u16 ime = *(volatile u16 *) 0x04000208;
    *(volatile u16 *) 0x04000208 = 0;
    *(volatile u16 *) 0x04000200 &= 0xff7f;
    *(volatile u16 *) 0x04000202 = 0x80;
    *(volatile u16 *) 0x04000208 = ime;
}

EC SECTION(".text.g5_commands") void ConfigureJoybus(u32 reset, u32 mode) // 0x080D0B14
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

EC SECTION(".text.g5_requests") void SetJoybusHandshakeWords(u32 handshake_send, u32 handshake_expected) // 0x080D0444
{
    gJoybusState->unknown_00[JOYBUS_HANDSHAKE_SEND] = handshake_send;
    gJoybusState->unknown_00[JOYBUS_HANDSHAKE_EXPECTED] = handshake_expected;
}

EC SECTION(".text.g5_requests") void func_080D0454()
{
    JoybusCommand command;
    gJoybusState->unknown_00[5] = 1;
    command.kind = 1;
    EnqueueJoybusCommand(&command);
    command.kind = 3;
    command.args[1] = (u32) &gJoybusState->unknown_00[9];
    command.args[3] = 2;
    *(u16 *) &command.args[4] = 0;
    EnqueueJoybusCommand(&command);
}

EC SECTION(".text.g5_queries") u32 func_080D0678()
{
    JoybusState * state = gJoybusState;
    return ((u8 *) state)[0x2c] | ((u8 *) state)[0x2d] << 8 |
        ((u8 *) state)[0x2e] << 16 | ((u8 *) state)[0x2f] << 24;
}

EC SECTION(".text.g5_transfer") void Joybus_QueueExchangeCommands() // 0x080D05DC
{
    JoybusCommand command;
    gJoybusState->unknown_00[5] = 5;
    if (gJoybusState->unknown_00[12] != 0)
    {
        command.kind = 1;
        EnqueueJoybusCommand(&command);
    }
    command.kind = 4;
    command.args[1] = (u32) &gJoybusState->unknown_00[9];
    command.args[3] = 2;
    *(u16 *) &command.args[4] = 0;
    EnqueueJoybusCommand(&command);
    command.kind = 3;
    command.args[1] = (u32) &gJoybusState->unknown_00[11];
    command.args[3] = 2;
    *(u16 *) &command.args[4] = 0;
    EnqueueJoybusCommand(&command);
    command.kind = 2;
    command.args[1] = (u32) &gJoybusState->unknown_00[10];
    command.args[3] = 2;
    *(u16 *) &command.args[4] = 0;
    EnqueueJoybusCommand(&command);
}

EC SECTION(".text.g5_queries") void func_080D06A8()
{
    SetJoybusStatusWhileBusy(11);
}

EC SECTION(".text.g5_queries") bool func_080D06B4()
{
    return gJoybusState->unknown_00[5] == 0;
}

EC SECTION(".text.g5_queries") u32 func_080D06D0()
{
    return gJoybusState->unknown_00[6];
}

static inline u32 GetScaledJoybusProgress(JoybusState * state)
{
    register u32 completed asm("r1") = state->unknown_00[JOYBUS_TRANSFER_PROGRESS];
    asm ("" : "+l" (completed));
    return 100 * completed;
}

EC SECTION(".text.g5_080D06DC") u32 Joybus_GetTransferProgressPercent() // 0x080D06DC
{
    u32 total = gJoybusState->unknown_00[JOYBUS_TRANSFER_TOTAL];
    return total != 0 ? GetScaledJoybusProgress(gJoybusState) / total : 0;
}

EC SECTION(".text.g5_080D0704") void Joybus_HandleSerialInterrupt() // 0x080D0704
{
    u32 status = 0;
    u32 control = *(volatile u16 *) 0x04000140;
    if (gJoybusState->busy == 0)
    {
        if ((control & 2) != 0)
        {
            u32 packet = *(volatile u32 *) 0x04000150;
            status = Joybus_HandleReceivePacket(&packet);
        }
        if (status == 0 && (control & 4) != 0)
            status = Joybus_HandleTransmitReady();
        if ((control & 1) != 0)
            RetryJoybusHandshake();
    }
    if (status != 0)
    {
        *(volatile u16 *) 0x04000158 = 0;
        CompleteJoybusCommand(status);
    }
    *(volatile u16 *) 0x04000140 = control;
    gJoybusState->unknown_00[3] = 0;
    gJoybusState->unknown_00[4] = 0;
}

EC SECTION(".text.g5_080D035C") void Joybus_UpdateTransfers() // 0x080D035C
{
    if (((volatile JoybusState *) gJoybusState)->unknown_00[JOYBUS_ENABLED] != 0)
    {
        ((volatile JoybusState *) gJoybusState)->unknown_00[JOYBUS_TRANSFER_PROGRESS] = ((volatile JoybusState *) gJoybusState)->transferred_halfwords;
        if (gCurrentJoybusCommand->kind == 0)
        {
            if (!IsJoybusCommandQueueEmpty() && ((volatile JoybusState *) gJoybusState)->status == 0)
            {
                AdvanceJoybusCommandQueue();
                ((volatile JoybusState *) gJoybusState)->unknown_00[JOYBUS_IDLE_TICKS] = 0;
            }
            else if (((volatile JoybusState *) gJoybusState)->unknown_00[JOYBUS_TRANSFER_KIND] != 0)
            {
                ((volatile JoybusState *) gJoybusState)->unknown_00[JOYBUS_TRANSFER_KIND] = 0;
                ((volatile JoybusState *) gJoybusState)->unknown_00[JOYBUS_TRANSFER_RESULT] = ((volatile JoybusState *) gJoybusState)->status;
                ((volatile JoybusState *) gJoybusState)->status = 0;
                CancelJoybusCommands();
            }
        }
        ++((volatile JoybusState *) gJoybusState)->unknown_00[JOYBUS_RETRY_TICKS];
        if (((volatile JoybusState *) gJoybusState)->unknown_00[JOYBUS_RETRY_TICKS] > 10)
        {
            ((volatile JoybusState *) gJoybusState)->unknown_00[JOYBUS_RETRY_TICKS] = 0;
            if (gCurrentJoybusCommand->kind != 0)
            {
                if (gCurrentJoybusCommand->kind == 1)
                    ConfigureJoybus(1, 0);
                else
                    ConfigureJoybus(1, 1);
            }
        }
        ++((volatile JoybusState *) gJoybusState)->unknown_00[JOYBUS_IDLE_TICKS];
        if (gCurrentJoybusCommand->kind > 1 && ((volatile JoybusState *) gJoybusState)->unknown_00[JOYBUS_IDLE_TICKS] > 180)
        {
            SetJoybusStatusWhileBusy(11);
            ((volatile JoybusState *) gJoybusState)->unknown_00[JOYBUS_TRANSFER_KIND] = 0;
            ((volatile JoybusState *) gJoybusState)->unknown_00[JOYBUS_TRANSFER_RESULT] = 10;
            ((volatile JoybusState *) gJoybusState)->status = 0;
            CancelJoybusCommands();
        }
    }
}
