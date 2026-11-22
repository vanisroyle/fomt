#include "g5_link.hh"

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
    state->unknown_00[8] = send_count + receive_count;
    state->transferred_halfwords = checksum;
}
