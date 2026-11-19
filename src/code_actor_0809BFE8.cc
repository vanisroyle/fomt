#include "prelude.h"

#include "actor.hh"

#include <string.h>

#define ROM_TEXT(addr) EC SECTION(".text." #addr)

enum ActorAction
{
    ACTOR_ACTION_NONE,
    ACTOR_ACTION_1,
    ACTOR_ACTION_2,
    ACTOR_ACTION_3,
    ACTOR_ACTION_4,
};

struct ActorActionData
{
    u32 first;
    u32 second;
};

struct Unk_Actor_0809BFE8 : Actor
{
    Unk_Actor_0809BFE8();

    u32 unk_08_0 : 7;
    ActorAction action;
    ActorActionData actionData;
};

Unk_Actor_0809BFE8::Unk_Actor_0809BFE8()
    : Actor(ActorLocation(Location(MAP_NONE, 0, 0), 0)), unk_08_0(100), action(ACTOR_ACTION_NONE)
{
}

EC Unk_Actor_0809BFE8 * func_0809BFE8() ALIAS(__18Unk_Actor_0809BFE8);

EC unsigned int func_0809C060(Unk_Actor_0809BFE8 const & self)
{
    return self.unk_08_0;
}

EC void func_0809C068(Unk_Actor_0809BFE8 & self, int arg_0)
{
    int val = self.unk_08_0 - arg_0;

    if (val < 0)
        val = 0;
    else if (val > 100)
        val = 100;

    self.unk_08_0 = val;
}

struct ActorActionReset {};

EC void func_0809C098(Unk_Actor_0809BFE8 & self, ActorActionReset const &)
{
    self.action = ACTOR_ACTION_NONE;
}

ROM_TEXT(0809C0A0) void func_0809C0A0(Unk_Actor_0809BFE8 & self, u32 const & data)
{
    self.actionData.first = data;
    self.action = ACTOR_ACTION_1;
}

ROM_TEXT(0809C0A0) void func_0809C0AC(Unk_Actor_0809BFE8 & self, ActorActionData const & data)
{
    self.actionData = data;
    self.action = ACTOR_ACTION_2;
}

ROM_TEXT(0809C0A0) void func_0809C0BC(Unk_Actor_0809BFE8 & self, u32 const & data)
{
    self.actionData.first = data;
    self.action = ACTOR_ACTION_3;
}

ROM_TEXT(0809C0A0) void func_0809C0C8(Unk_Actor_0809BFE8 & self, u32 const & data)
{
    self.actionData.first = data;
    self.action = ACTOR_ACTION_4;
}

ROM_TEXT(0809C0A0) void func_0809C0D4(Unk_Actor_0809BFE8 & self)
{
    self.SetLocation(ActorLocation(Location(MAP_NONE, 0, 0), 0));
    ActorActionReset reset;
    func_0809C098(self, reset);
    self.unk_08_0 = 100;
}

struct ActivityFlags
{
    u8 active[6];
    u8 completed[6];
    u8 elapsed[6];
};

ROM_TEXT(0809C0A0) ActivityFlags & func_0809C144(ActivityFlags & self)
{
    unsigned i = 0;
    u8 zero = 0;
    for (ActivityFlags * entry = &self; i < ARRAY_COUNT(self.active); ++i)
    {
        entry->active[i] = zero;
        entry->elapsed[i] = zero;
        entry->completed[i] = zero;
    }
    return self;
}

ROM_TEXT(0809C22C) unsigned func_0809C22C(ActivityFlags const &, unsigned id)
{
    unsigned index = 1;
    switch (id)
    {
    case 5: index = 0; break;
    case 13: index = 1; break;
    case 21: index = 2; break;
    case 29: index = 3; break;
    case 37: index = 4; break;
    case 45: index = 5; break;
    }
    return index;
}

ROM_TEXT(0809C22C) u8 func_0809C304(ActivityFlags const & self, unsigned id)
{
    return self.active[func_0809C22C(self, id)];
}

static inline bool HasActivity(ActivityFlags const & self, unsigned index)
{
    int completed = -self.completed[index];
    int active = -self.active[index];
    active |= completed;
    return active < 0;
}

ROM_TEXT(0809C32C) bool func_0809C32C(ActivityFlags const & self)
{
    return HasActivity(self, 1) && HasActivity(self, 0) && HasActivity(self, 2)
        && HasActivity(self, 3) && HasActivity(self, 4) && HasActivity(self, 5);
}

ROM_TEXT(0809C32C) bool func_0809C38C(ActivityFlags const & self)
{
    return self.completed[1] && self.completed[0] && self.completed[2] && self.completed[3]
        && self.completed[4] && self.completed[5];
}

ROM_TEXT(0809C32C) void func_0809C3BC(ActivityFlags & self, unsigned id)
{
    unsigned index = func_0809C22C(self, id);
    if (self.active[index] == 0)
    {
        self.active[index] = 1;
        self.elapsed[index] = 0;
    }
}

EC u8 const gUnk_081036C0[];

ROM_TEXT(0809C32C) bool func_0809C3E0(ActivityFlags & self, unsigned index)
{
    bool completed = false;
    if (self.active[index])
    {
        if (self.elapsed[index] < gUnk_081036C0[index])
            ++self.elapsed[index];
        else
        {
            self.active[index] = 0;
            self.completed[index] = 1;
            completed = true;
        }
    }
    return completed;
}

ROM_TEXT(0809C32C) void func_0809C420(ActivityFlags & self, unsigned index)
{
    if (self.active[index] && (index == 0 || index == 3))
        self.elapsed[index] = 0;
}

ROM_TEXT(0809C32C) bool func_0809C444(ActivityFlags & self, unsigned id)
{
    bool completed = false;
    unsigned index = func_0809C22C(self, id);
    if (self.active[index] && (index == 0 || index == 3))
        completed = func_0809C3E0(self, index);
    return completed;
}

ROM_TEXT(0809C32C) bool func_0809C474(ActivityFlags & self, unsigned id)
{
    unsigned index = func_0809C22C(self, id);
    bool completed = false;
    if (self.active[index])
    {
        if (index == 1 || index == 4)
            completed = func_0809C3E0(self, index);
        if (index == 0 || index == 3)
            func_0809C420(self, index);
    }
    return completed;
}

ROM_TEXT(0809C32C) bool func_0809C4B4(ActivityFlags & self, unsigned id)
{
    unsigned index = func_0809C22C(self, id);
    bool completed = false;
    if (self.active[index] && (index == 5 || index == 2))
        completed = func_0809C3E0(self, index);
    return completed;
}

ROM_TEXT(0809C32C) void func_0809C4E4(u32 & flags)
{
    flags = 0;
}

ROM_TEXT(0809C5B4) void func_0809C5B4(u32 & flags, unsigned index)
{
    if (index <= 13)
        flags |= 1u << (index & 31);
}

ROM_TEXT(0809C5B4) void func_0809C5D0(u32 & flags, unsigned index)
{
    if (index <= 13)
        flags &= ~(1u << (index & 31));
}

struct Unk_0809C5EC
{
    u32 count;
    u16 values[3];
    STRUCT_PAD(0x0A, 0x0C);
    u16 selected;
};

EC u16 * func_080E3E28(u16 *, u16 *, u16 const &);

ROM_TEXT(0809C5B4) void func_0809C5EC(Unk_0809C5EC & self)
{
    self.count = 0;
}

ROM_TEXT(0809C5B4) bool func_0809C5F4(Unk_0809C5EC const & self)
{
    return self.count != 0;
}

ROM_TEXT(0809C644) void func_0809C644(Unk_0809C5EC & self, u16 value)
{
    u16 * begin = self.values;
    unsigned offset = offsetof(Unk_0809C5EC, values) + self.count * sizeof(value);
    u16 * end = reinterpret_cast<u16 *>(reinterpret_cast<uintptr_t>(&self) + offset);
    u16 * found = func_080E3E28(begin, end, value);
    if (found != end && self.count)
    {
        u16 * last = self.values + self.count;
        u16 * next = found + 1;
        if (next != last)
        {
            // Keep both comparisons to match the inlined copy.
            if (last != next)
                memmove(found, next, reinterpret_cast<char *>(last) - reinterpret_cast<char *>(next));
        }
        --self.count;
    }
}

ROM_TEXT(0809C644) bool func_0809C694(Unk_0809C5EC const & self, u16 value)
{
    return self.selected == value;
}

ROM_TEXT(0809C644) void func_0809C6AC(Unk_0809C5EC & self, u16 value)
{
    self.selected = value;
}

ROM_TEXT(0809C644) void func_0809C6B0(Unk_0809C5EC & self)
{
    self.selected = 0xFFFF;
}

struct Record
{
    u32 count;
    u32 best;
};

struct RecordBook
{
    Record records[59];
};

ROM_TEXT(0809CD78) RecordBook & func_0809CD78(RecordBook & self)
{
    Record * record = self.records;
    for (int remaining = ARRAY_COUNT(self.records) - 1; remaining != -1; ++record, --remaining)
    {
        record->count = 0;
        record->best = 0;
    }
    return self;
}

ROM_TEXT(0809CD78) bool func_0809CD98(RecordBook & self, unsigned index, u32 value)
{
    bool improved = false;
    if (self.records[index].count < 1000000000)
        ++self.records[index].count;
    if (value > self.records[index].best)
    {
        self.records[index].best = value;
        improved = true;
    }
    return improved;
}

ROM_TEXT(0809CD78) bool func_0809CDCC(RecordBook const & self)
{
    bool allRecorded = true;
    for (unsigned i = 8; i < ARRAY_COUNT(self.records); ++i)
        if (self.records[i].count == 0)
            allRecorded = false;
    return allRecorded;
}

ROM_TEXT(0809CD78) u32 func_0809CDEC(RecordBook const & self)
{
    u32 total = 0;
    for (unsigned i = 8; i < ARRAY_COUNT(self.records); ++i)
    {
        total += self.records[i].count;
        if (total >= 1000000000)
            total = 1000000000;
    }
    return total;
}

ROM_TEXT(0809CD78) u32 func_0809CE1C(RecordBook const & self, unsigned index)
{
    return self.records[index].count;
}

ROM_TEXT(0809CD78) u32 func_0809CE24(RecordBook const & self, unsigned index)
{
    return self.records[index].best;
}

ROM_TEXT(0809CD78) unsigned func_0809CE30(RecordBook const &, unsigned index)
{
    switch (index)
    {
    default:
    case 53: return 252;
    case 54: return 249;
    case 55: return 254;
    case 56: return 253;
    case 57: return 250;
    case 58: return 251;
    }
}

EC u32 const gUnk_08103A18[];

ROM_TEXT(0809CD78) u32 func_0809CE7C(RecordBook const &, unsigned index)
{
    return gUnk_08103A18[index];
}

struct Unk_0809D8A0
{
    u32 kind;
    struct PACKED ALIGN(2) Entry
    {
        u16 unk_00 : 4;
        u16 unk_04 : 6;
        u16 unk_0A : 6;
    };
    Entry entries[28][28];
    u32 unk_624_0 : 1;
    u32 : 9;
    u32 unk_625_2 : 1;
    u32 unk_625_3 : 1;
    u32 unk_625_4 : 1;
};

ROM_TEXT(0809D470) unsigned func_0809D470(Unk_0809D8A0 const &, unsigned value)
{
    switch (value)
    {
    case 20:
        return 12;
    case 30:
        return 14;
    case 40:
        return 16;
    case 60:
    case 102:
    case 123:
    case 152:
    case 155:
    case 171:
    case 190:
    case 202:
    case 222:
        return 18;
    case 255:
        return 20;
    }

    unsigned result = value;
    if (result <= 10)
        goto done;
    result = 11;
    if (value <= 19)
        goto done;
    result = 13;
    if (value <= 29)
        goto done;
    result = 15;
    if (value <= 39)
        goto done;
    result = 19;
    if (value > 230)
        goto done;
    result = 17;
done:
    return result;
}

ROM_TEXT(0809D7D8) unsigned func_0809D7D8(Unk_0809D8A0 const &, unsigned value)
{
    switch (value)
    {
        case 255: return 55;
        case 30:
        case 70:
        case 90: return 30;
        case 40:
        case 60:
        case 80: return 31;
        case 50:
        case 100:
        case 150:
        case 200: return 32;
        case 110:
        case 130:
        case 170:
        case 190: return 42;
        case 120:
        case 140:
        case 160:
        case 180: return 43;
    }
    if (value <= 20)
        return value;
    if (value > 250)
        return 54;
    if (value <= 99)
        return value % 10 + 20;
    if (value > 199)
        return value % 10 + 43;
    return value % 10 + 32;
}

ROM_TEXT(0809D7D8) u32 func_0809D8A0(Unk_0809D8A0 const & self)
{
    return self.kind;
}

ROM_TEXT(0809D7D8) unsigned func_0809D8A4(Unk_0809D8A0 const & self)
{
    unsigned count = 13;
    if (self.kind == 1)
        count = 28;
    return count;
}

ROM_TEXT(0809D7D8) unsigned func_0809D8B8(Unk_0809D8A0 const & self)
{
    unsigned count = 6;
    if (self.kind == 1)
        count = 28;
    else if (self.kind == 2)
        count = 14;
    return count;
}

ROM_TEXT(0809D7D8) unsigned func_0809D8D4(Unk_0809D8A0 const & self, unsigned index, unsigned row)
{
    return self.entries[row][index].unk_00;
}

EC int func_0809DA00(Unk_0809D8A0 &, unsigned, void *);

ROM_TEXT(0809D9B4) int func_0809D9B4(Unk_0809D8A0 & self, unsigned index, unsigned row, void * context)
{
    int result = 0;
    unsigned offset = index * sizeof(Unk_0809D8A0::Entry);
    offset += row * sizeof(self.entries[0]);
    char * base = reinterpret_cast<char *>(&self) + offset;
    u8 firstByte = static_cast<u8>(base[offsetof(Unk_0809D8A0, entries)]);
    unsigned kind = (static_cast<u32>(firstByte) << 28) >> 28;
    switch (kind)
    {
    case 4:
    {
        Unk_0809D8A0::Entry & entry = *reinterpret_cast<Unk_0809D8A0::Entry *>(base + offsetof(Unk_0809D8A0, entries));
        result = func_0809DA00(self, entry.unk_04, context);
        entry.unk_00 = 0;
        entry.unk_04 = 0;
        break;
    }
    }
    return result;
}

ROM_TEXT(0809E174) bool func_0809E174(Unk_0809D8A0 const & self)
{
    return self.unk_624_0;
}

ROM_TEXT(0809E174) bool func_0809E184(Unk_0809D8A0 const & self)
{
    return self.unk_625_2;
}

ROM_TEXT(0809E174) bool func_0809E194(Unk_0809D8A0 const & self)
{
    return self.unk_625_3;
}

ROM_TEXT(0809E174) bool func_0809E1A4(Unk_0809D8A0 const & self)
{
    return self.unk_625_4;
}
