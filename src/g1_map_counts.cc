#include "g1_map.hh"

EC SECTION(".text.g1_080A1A4C") void InitializeMapAvailability(G1MapAvailability * self)
{
    for (int i = 0; i < 7; i++)
        self->bits[i] |= 0xff;
    self->count_60 = 6;
    self->count_61 = 6;
    self->count_62 = 6;
    self->count_63 = 6;
    self->count_64 = 6;
    self->count_65 = 6;
    self->count_66 = 6;
    self->count_67 = 6;
    self->count_68 = 6;
    self->count_69 = 6;
    self->count_6A = 6;
    self->count_6B = 6;
    self->count_6C = 6;
    self->count_6D = 1;
    self->count_6E = 1;
}

EC SECTION(".text.g1_080A1EF4") u32 GetMapAvailabilityCount(G1MapAvailability const * self, u32 index)
{
    u32 count = 0;
    if (index > 0x5f)
    {
        switch (index)
        {
        case 0x60: count = self->count_60; break;
        case 0x61: count = self->count_61; break;
        case 0x62: count = self->count_62; break;
        case 0x63: count = self->count_63; break;
        case 0x64: count = self->count_64; break;
        case 0x65: count = self->count_65; break;
        case 0x66: count = self->count_66; break;
        case 0x67: count = self->count_67; break;
        case 0x68: count = self->count_68; break;
        case 0x69: count = self->count_69; break;
        case 0x6a: count = self->count_6A; break;
        case 0x6b: count = self->count_6B; break;
        case 0x6c: count = self->count_6C; break;
        case 0x6d: count = self->count_6D; break;
        case 0x6e: count = self->count_6E; break;
        }
    }
    return count;
}

static inline int SubtractAvailableCount(int count, int amount)
{
    int result = count - amount;
    if (result < 0)
        result = 0;
    return result;
}

EC SECTION(".text.g1_080A1FC4") void SubtractMapAvailabilityCount(G1MapAvailability * self, u32 index, int amount)
{
    if (index > 0x5f)
    {
        switch (index)
        {
        case 0x60: self->count_60 = SubtractAvailableCount(self->count_60, amount); break;
        case 0x61: self->count_61 = SubtractAvailableCount(self->count_61, amount); break;
        case 0x62: self->count_62 = SubtractAvailableCount(self->count_62, amount); break;
        case 0x63: self->count_63 = SubtractAvailableCount(self->count_63, amount); break;
        case 0x64: self->count_64 = SubtractAvailableCount(self->count_64, amount); break;
        case 0x65: self->count_65 = SubtractAvailableCount(self->count_65, amount); break;
        case 0x66: self->count_66 = SubtractAvailableCount(self->count_66, amount); break;
        case 0x67: self->count_67 = SubtractAvailableCount(self->count_67, amount); break;
        case 0x68: self->count_68 = SubtractAvailableCount(self->count_68, amount); break;
        case 0x69: self->count_69 = SubtractAvailableCount(self->count_69, amount); break;
        case 0x6a: self->count_6A = SubtractAvailableCount(self->count_6A, amount); break;
        case 0x6b: self->count_6B = SubtractAvailableCount(self->count_6B, amount); break;
        case 0x6c: self->count_6C = SubtractAvailableCount(self->count_6C, amount); break;
        case 0x6d: self->count_6D = SubtractAvailableCount(self->count_6D, amount); break;
        case 0x6e: self->count_6E = SubtractAvailableCount(self->count_6E, amount); break;
        }
    }
}
