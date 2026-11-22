#include "g5_link.hh"

EC SECTION(".text.g5_080D0490") void Joybus_QueueSendBuffer(u8 const * buffer, u32 byte_count) // 0x080D0490
{
    register u8 const * transfer_buffer asm("r9") = buffer;
    register u32 send_count asm("r6") = byte_count;
    asm ("" : "+r" (transfer_buffer), "+l" (send_count));
    register JoybusState ** state_slot asm("r8") = &gJoybusState;
    asm ("" : : "r" (state_slot));
    JoybusCommand command;
    register JoybusState * state asm("r1") = gJoybusState;
    asm ("" : "+l" (state));
    register u32 command_kind asm("r4") = 2;
    asm ("" : "+l" (command_kind));
    state->unknown_00[JOYBUS_TRANSFER_KIND] = command_kind;
    command.kind = 4;
    command.args[1] = (u32) &state->unknown_00[9];
    command.args[3] = command_kind;
    register JoybusCommand * first_command asm("r0") = &command;
    asm ("" : "+l" (first_command));
    register u32 checksum asm("r5") = 0;
    asm ("" : "+l" (checksum));
    *(u16 *) &first_command->args[4] = checksum;
    EnqueueJoybusCommand(first_command);
    command.kind = command_kind;
    register u8 const * buffer_argument asm("r1") = transfer_buffer;
    asm ("" : "+l" (buffer_argument));
    command.args[1] = (u32) buffer_argument;
    send_count = (send_count + 1) >> 1;
    command.args[3] = send_count;
    *(u16 *) &command.args[4] = checksum;
    EnqueueJoybusCommand(&command);
    register JoybusState ** last_slot asm("r1") = state_slot;
    asm ("" : "+l" (last_slot));
    register JoybusState * last_state asm("r0") = *last_slot;
    asm ("" : "+l" (last_state));
    last_state->unknown_00[JOYBUS_TRANSFER_PROGRESS] = checksum;
    last_state->unknown_00[JOYBUS_TRANSFER_TOTAL] = send_count;
    last_state->transferred_halfwords = checksum;
}

EC SECTION(".text.g5_080D04F4") void Joybus_QueueReceiveBuffer(u8 * buffer, u32 byte_count) // 0x080D04F4
{
    register u8 * transfer_buffer asm("r9") = buffer;
    register u32 receive_count asm("r5") = byte_count;
    asm ("" : "+r" (transfer_buffer), "+l" (receive_count));
    register JoybusState ** state_slot asm("r8") = &gJoybusState;
    asm ("" : : "r" (state_slot));
    JoybusCommand command;
    register JoybusState * state asm("r1") = gJoybusState;
    asm ("" : "+l" (state));
    register u32 command_kind asm("r6") = 3;
    asm ("" : "+l" (command_kind));
    state->unknown_00[JOYBUS_TRANSFER_KIND] = command_kind;
    command.kind = 4;
    command.args[1] = (u32) &state->unknown_00[9];
    command.args[3] = 2;
    register JoybusCommand * first_command asm("r0") = &command;
    asm ("" : "+l" (first_command));
    register u32 checksum asm("r4") = 0;
    asm ("" : "+l" (checksum));
    *(u16 *) &first_command->args[4] = checksum;
    EnqueueJoybusCommand(first_command);
    command.kind = command_kind;
    register u8 * buffer_argument asm("r1") = transfer_buffer;
    asm ("" : "+l" (buffer_argument));
    command.args[1] = (u32) buffer_argument;
    receive_count = (receive_count + 1) >> 1;
    command.args[3] = receive_count;
    *(u16 *) &command.args[4] = checksum;
    EnqueueJoybusCommand(&command);
    register JoybusState ** last_slot asm("r1") = state_slot;
    asm ("" : "+l" (last_slot));
    register JoybusState * last_state asm("r0") = *last_slot;
    asm ("" : "+l" (last_state));
    last_state->unknown_00[JOYBUS_TRANSFER_PROGRESS] = checksum;
    last_state->unknown_00[JOYBUS_TRANSFER_TOTAL] = receive_count;
    last_state->transferred_halfwords = checksum;
}

EC SECTION(".text.g5_080D0558") void Joybus_QueueBufferExchange(u8 * send_buffer, u32 send_bytes, u8 * receive_buffer, u32 receive_bytes) // 0x080D0558
{
    register u32 send_count asm("r4") = send_bytes;
    register u8 * receive asm("r9") = receive_buffer;
    register u32 receive_count asm("r6") = receive_bytes;
    asm ("" : "+l" (send_count), "+r" (receive), "+l" (receive_count));
    register JoybusState ** state_slot asm("r10") = &gJoybusState;
    asm ("" : : "r" (state_slot));
    JoybusCommand command;
    register JoybusState * state asm("r0") = gJoybusState;
    asm ("" : "+l" (state));
    state->unknown_00[JOYBUS_TRANSFER_KIND] = 4;
    command.kind = 4;
    command.args[1] = (u32) &state->unknown_00[9];
    register u32 kind asm("r1") = 2;
    asm ("" : "+l" (kind));
    register u32 send_kind asm("r8") = kind;
    asm ("" : : "r" (send_kind));
    command.args[3] = kind;
    register JoybusCommand * first_command asm("r0") = &command;
    asm ("" : "+l" (first_command));
    register u32 checksum asm("r5") = 0;
    asm ("" : "+l" (checksum));
    *(u16 *) &first_command->args[4] = checksum;
    EnqueueJoybusCommand(first_command);
    command.kind = 3;
    register u8 * receive_arg asm("r0") = receive;
    asm ("" : "+l" (receive_arg));
    command.args[1] = (u32) receive_arg;
    receive_count = (receive_count + 1) >> 1;
    command.args[3] = receive_count;
    *(u16 *) &command.args[4] = checksum;
    EnqueueJoybusCommand(&command);
    kind = send_kind;
    asm ("" : "+l" (kind));
    command.kind = kind;
    command.args[1] = (u32) send_buffer;
    send_count = (send_count + 1) >> 1;
    command.args[3] = send_count;
    *(u16 *) &command.args[4] = checksum;
    EnqueueJoybusCommand(&command);
    register JoybusState ** last_slot asm("r1") = state_slot;
    asm ("" : "+l" (last_slot));
    state = *last_slot;
    asm ("" : "+l" (state));
    state->unknown_00[JOYBUS_TRANSFER_PROGRESS] = checksum;
    state->unknown_00[JOYBUS_TRANSFER_TOTAL] = send_count + receive_count;
    state->transferred_halfwords = checksum;
}
