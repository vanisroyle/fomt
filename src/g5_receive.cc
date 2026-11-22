#include "g5_link.hh"

EC void ClearJoybusCommandStatus();

EC SECTION(".text.g5_080D078C") u32 Joybus_HandleReceivePacket(u32 const * packet_word) // 0x080D078C
{
    u8 const * packet = (u8 const *) packet_word;
    u32 response_words[2];
    asm ("" : "=m" (response_words));
    u32 progress;
    u32 checksum;
    register u32 response_tag asm("r2");
    register u8 * reply asm("r1");
    JoybusCommand volatile * command;
    switch (((JoybusCommand volatile *) gCurrentJoybusCommand)->kind)
    {
    case 0:
        break;
    case 1:
    {
        progress = ((JoybusCommand volatile *) gCurrentJoybusCommand)->progress;
        if (progress > 1)
        {
            if (progress == 2)
            {
                if (*(u32 const *) packet != gJoybusState->unknown_00[JOYBUS_HANDSHAKE_EXPECTED])
                    return 1;
                *(u16 volatile *) 0x04000158 = 0x30;
clear_status:
                ClearJoybusCommandStatus();
            }
        }
        break;
    }
    case 2:
    case 4:
        command = gCurrentJoybusCommand;
        switch (command->progress)
        {
        case 0:
            if (packet[0] == 0x42)
            {
                if ((u32) (packet[2] | (packet[3] << 8)) != command->args[3])
                    return 4;
                reply = (u8 *) response_words;
                response_tag = 0x43;
                asm ("" : "+l" (response_tag));
                goto send_response;
            }
            if (packet[0] == 0x41)
                return 5;
            break;
        case 1:
            return 7;
        case 2:
            if (packet[0] == 0x46)
                goto clear_status;
            return 7;
        }
        break;
    case 3:
        command = gCurrentJoybusCommand;
        switch (command->progress)
        {
        case 0:
            if (packet[0] == 0x41)
            {
                if ((u32) (packet[2] | (packet[3] << 8)) != command->args[3])
                    return 4;
                reply = (u8 *) response_words;
                response_tag = 0x43;
                goto send_response;
            }
            if (packet[0] == 0x42)
                return 6;
            break;
        case 1:
            if (command->unknown_08 < command->args[3])
            {
                if (packet[0] != 0x44)
                    return 8;
                if (packet[1] != (command->unknown_08 & 0xff))
                    return 8;
                u8 * destination = (u8 *) (command->unknown_08 * 2 + command->args[1]);
                destination[0] = packet[2];
                destination[1] = packet[3];
                JoybusCommand volatile * current = gCurrentJoybusCommand;
                u8 * data = (u8 *) (current->unknown_08 * 2 + current->args[1]);
                u32 halfword = data[0] | (data[1] << 8);
                register u32 previous_checksum asm("r0") = *(u16 volatile *) &current->args[4];
                asm ("" : "+l" (previous_checksum));
                halfword += previous_checksum;
                *(u16 volatile *) &current->args[4] = halfword;
                current->unknown_08++;
                gJoybusState->transferred_halfwords++;
            }
            else
            {
                if (packet[0] != 0x45)
                    return 8;
                checksum = packet[2] | (packet[3] << 8);
                if (*(u16 volatile *) &command->args[4] != checksum)
                    return 3;
                reply = (u8 *) response_words;
                response_tag = 0x46;
send_response:;
                register u32 zero asm("r0") = 0;
                asm ("" : "+l" (zero));
                reply[0] = response_tag;
                reply[1] = zero;
                reply[2] = zero;
                reply[3] = zero;
                *(u32 volatile *) 0x04000154 = response_words[0];
                command->progress++;
                goto complete;
            }
            break;
        case 2:
            return 8;
        }
        break;
    default:
        return 9;
    }
complete:
    return 0;
}
