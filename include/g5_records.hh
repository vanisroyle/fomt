#ifndef G5_RECORDS_HH
#define G5_RECORDS_HH

#include "prelude.h"

// Four-byte records retain two unidentified header bytes and a little-endian
// value. The owning subsystem is unknown, so their accessors retain addresses.
struct G5ByteRecord
{
    u8 unknown_00;
    u8 unknown_01;
    u8 value_bytes[2];
};

EC void func_080D0C78(G5ByteRecord * record, u8 unknown_00, u8 unknown_01, u8 const * value_bytes); // 0x080D0C78
EC u8 func_080D0C88(G5ByteRecord const * record); // 0x080D0C88
EC u16 func_080D0C8C(G5ByteRecord const * record); // 0x080D0C8C
EC u8 func_080D0C98(G5ByteRecord const * record); // 0x080D0C98
EC void func_080D0C9C(G5ByteRecord const * record, u8 * value_bytes); // 0x080D0C9C
EC u16 G5Bytes_ReadLittleEndian16(u8 const * value_bytes); // 0x080D0CA8
EC bool func_080D0CB4(G5ByteRecord const * record, u8 const * value_bytes); // 0x080D0CB4

#endif
