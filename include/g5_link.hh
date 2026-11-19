#ifndef G5_LINK_HH
#define G5_LINK_HH

#include "prelude.h"

// Joybus serial commands use a ten-entry ring. The current command pointer
// follows read_index; command-specific arguments and unknown state remain raw.
struct JoybusCommand
{
    u32 kind;
    u32 progress;
    u32 unknown_08;
    u32 args[5];
};

struct JoybusState
{
    u32 unknown_00[13];
    u32 read_index;
    u32 write_index;
    JoybusCommand commands[10];
    u32 busy;
    u32 status;
    u32 transferred_halfwords;
};

enum
{
    JOYBUS_ENABLED = 0,
    JOYBUS_HANDSHAKE_SEND = 1,
    JOYBUS_HANDSHAKE_EXPECTED = 2,
    JOYBUS_RETRY_TICKS = 3,
    JOYBUS_IDLE_TICKS = 4,
    JOYBUS_TRANSFER_KIND = 5,
    JOYBUS_TRANSFER_RESULT = 6,
    JOYBUS_TRANSFER_PROGRESS = 7,
};

EC JoybusState * gJoybusState; // 0x03000420
EC JoybusCommand * gCurrentJoybusCommand; // 0x0300041C
EC void InitializeJoybusState(JoybusState * state); // 0x080D02CC
EC void SetJoybusHandshakeWords(u32 handshake_send, u32 handshake_expected); // 0x080D0444
EC void StartJoybus(); // 0x080D02F0
EC void StopJoybus(); // 0x080D0340
EC void ResetJoybusCommandQueue(); // 0x080D0B88
EC void EnqueueJoybusCommand(JoybusCommand * command); // 0x080D0BA8
EC void AdvanceJoybusCommandQueue(); // 0x080D0BE0
EC u32 IsJoybusCommandQueueEmpty(); // 0x080D0C10; returns 0 or 1
EC void CancelJoybusCommands(); // 0x080D0C2C
EC void CompleteJoybusCommand(u32 status); // 0x080D0A54
EC void SetJoybusStatusWhileBusy(u32 status); // 0x080D0A94
EC void ConfigureJoybus(u32 reset, u32 mode); // 0x080D0B14
EC void EnableJoybusSerialInterrupt(); // 0x080D0ABC
EC void DisableJoybusSerialInterrupt(); // 0x080D0AE0

EC void Joybus_HandleSerialInterrupt(); // 0x080D0704
EC void Joybus_UpdateTransfers(); // 0x080D035C

#endif
