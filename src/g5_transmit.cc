#include "g5_link.hh"

EC void ClearJoybusCommandStatus();

EC SECTION(".text.g5_080D091C") u32 Joybus_HandleTransmitReady() // 0x080D091C
{
    switch (((JoybusCommand volatile *) gCurrentJoybusCommand)->kind)
    {
    case 0:
        break;
    case 1:
    {
        JoybusCommand volatile * command = gCurrentJoybusCommand;
        if (command->progress == 1)
            command->progress++;
        break;
    }
    case 2:
    case 4:
    {
        JoybusCommand volatile * command = gCurrentJoybusCommand;
        if (command->progress == 1)
        {
            u32 packet_word;
            if (command->unknown_08 < command->args[3])
            {
                register u32 index asm("r1") = command->unknown_08;
                register u8 * packet asm("r2") = (u8 *) &packet_word;
                asm ("" : "+l" (packet));
                register u32 tag asm("r3") = 0x44;
                asm ("" : "+l" (tag));
                u32 sequence = command->unknown_08;
                index <<= 1;
                asm ("" : "+l" (index));
                index += command->args[1];
                asm ("" : "+l" (index));
                u8 const * data = (u8 const *) index;
                packet[0] = tag;
                packet[1] = sequence;
                packet[2] = data[0];
                packet[3] = data[1];
                u32 offset = command->unknown_08 * 2;
                register u8 const * checksum_data asm("r0") = (u8 const *) (offset + command->args[1]);
                asm ("" : "+l" (checksum_data));
                register u32 halfword asm("r1") = checksum_data[0] | (checksum_data[1] << 8);
                asm ("" : "+l" (halfword));
                u32 previous_checksum = *(u16 volatile *) &command->args[4];
                asm ("" : "+l" (previous_checksum));
                halfword += previous_checksum;
                *(u16 volatile *) &command->args[4] = halfword;
                *(u32 volatile *) 0x04000154 = packet_word;
                command->unknown_08++;
                if (command->kind != 4)
                    gJoybusState->transferred_halfwords++;
            }
            else
            {
                register u32 checksum asm("r2") = *(u16 volatile *) &command->args[4];
                asm ("" : "+l" (checksum));
                register u8 * packet asm("r1") = (u8 *) &packet_word;
                asm ("" : "+l" (packet));
                packet[0] = 0x45;
                packet[1] = 0;
                packet[2] = checksum;
                checksum >>= 8;
                asm ("" : "+l" (checksum));
                packet[3] = checksum;
                *(u32 volatile *) 0x04000154 = packet_word;
                command->progress++;
            }
        }
        break;
    }
    case 3:
    {
        JoybusCommand volatile * command = gCurrentJoybusCommand;
        u32 progress = command->progress;
        if (progress > 1)
        {
            asm ("" : "+l" (progress));
            if (progress == 2)
                ClearJoybusCommandStatus();
        }
        break;
    }
    default:
        return 9;
    }
    return 0;
}
