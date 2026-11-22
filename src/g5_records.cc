#include "g5_records.hh"

EC SECTION(".text.g5_080D0C68") void G5ByteRecord_Initialize(G5ByteRecord * record, u8 header_byte, u16 value) // 0x080D0C68
{
    record->unknown_00 = header_byte;
    register u32 cleared_header asm("r1") = 0;
    asm ("" : "+l" (cleared_header));
    record->unknown_01 = cleared_header;
    record->value_bytes[0] = value;
    record->value_bytes[1] = value >> 8;
}

EC SECTION(".text.g5_080D0C78") void func_080D0C78(G5ByteRecord * record, u8 unknown_00, u8 unknown_01, u8 const * value_bytes) // 0x080D0C78
{
    record->unknown_00 = unknown_00;
    record->unknown_01 = unknown_01;
    record->value_bytes[0] = value_bytes[0];
    record->value_bytes[1] = value_bytes[1];
}

EC SECTION(".text.g5_080D0C78") u8 func_080D0C88(G5ByteRecord const * record) // 0x080D0C88
{
    return record->unknown_00;
}

EC SECTION(".text.g5_080D0C78") u16 func_080D0C8C(G5ByteRecord const * record) // 0x080D0C8C
{
    return record->value_bytes[0] | record->value_bytes[1] << 8;
}

EC SECTION(".text.g5_080D0C78") u8 func_080D0C98(G5ByteRecord const * record) // 0x080D0C98
{
    return record->unknown_01;
}

EC SECTION(".text.g5_080D0C78") void func_080D0C9C(G5ByteRecord const * record, u8 * value_bytes) // 0x080D0C9C
{
    value_bytes[0] = record->value_bytes[0];
    value_bytes[1] = record->value_bytes[1];
}

EC SECTION(".text.g5_080D0C78") u16 G5Bytes_ReadLittleEndian16(u8 const * value_bytes) // 0x080D0CA8
{
    return value_bytes[0] | value_bytes[1] << 8;
}

EC SECTION(".text.g5_080D0C78") bool func_080D0CB4(G5ByteRecord const * record, u8 const * value_bytes) // 0x080D0CB4
{
    bool equal = false;
    if (value_bytes[0] == record->value_bytes[0] && value_bytes[1] == record->value_bytes[1])
        equal = true;
    return equal;
}
