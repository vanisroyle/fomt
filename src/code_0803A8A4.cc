#include "prelude.h"

#include "bachelorette.hh"
#include "npc.hh"
#include "schedule_info.hh"

#include <stdlib.h>
#include <string.h>

#define ROM_TEXT(addr) EC SECTION(".text." #addr)

struct Unk_0803B454
{
    u8 unk_00[16];
    u16 unk_10;
    u16 unk_12;
};

struct Unk_0803B474 : Unk_0803B454
{
    u32 x;
    u32 y;
};

EC void func_0803B474(Unk_0803B474 &, int, int);

ROM_TEXT(0803B454) Unk_0803B474 & func_0803B454(Unk_0803B474 & self, Unk_0803B454 const & source, int x, int y)
{
    static_cast<Unk_0803B454 &>(self) = source;
    func_0803B474(self, x, y);
    return self;
}

ROM_TEXT(0803B454) void func_0803B474(Unk_0803B474 & self, int x, int y)
{
    self.x = (static_cast<u32>(x) + rand() % 240) << 16;
    self.y = (static_cast<u32>(y) + rand() % 160) << 16;
    int sample = rand();
    int variation = 0x90;
    variation &= sample;
    self.unk_10 = variation + 0x60;
}

struct Unk_0803B4B0
{
    void const * vtable;
    void * unk_04;
    char labels[4][24];
};

EC char const vtable_unk_080E76E8[];

ROM_TEXT(0803B454) Unk_0803B4B0 & func_0803B4B0(Unk_0803B4B0 & self, void * unk)
{
    self.vtable = vtable_unk_080E76E8;
    self.unk_04 = unk;
    char (* label)[24] = self.labels;
    for (int remaining = ARRAY_COUNT(self.labels) - 1; remaining != -1; ++label, --remaining)
        (*label)[0] = '\0';
    return self;
}

ROM_TEXT(0803B6B8) void func_0803B6B8(Unk_0803B4B0 & self, unsigned index, char const * label)
{
    char buf[24];
    size_t length = strlen(label);
    if (length > 22)
        length = 22;
    memcpy(buf, label, length);
    buf[length] = '\0';
    unsigned offset = index * sizeof(self.labels[0]);
    char * record = reinterpret_cast<char *>(offset + reinterpret_cast<uintptr_t>(&self));
    strcpy(record + offsetof(Unk_0803B4B0, labels), buf);
}

struct Unk_0803B6FC
{
    void const * vtable;
    void * unk_04;
    void * unk_08;
};

EC char const vtable_unk_080E7748[];

ROM_TEXT(0803B6B8) Unk_0803B6FC & func_0803B6FC(Unk_0803B6FC & self, void *& owned, void * arg)
{
    self.vtable = vtable_unk_080E7748;
    void * taken = owned;
    owned = nullptr;
    self.unk_04 = taken;
    self.unk_08 = arg;
    return self;
}

struct PACKED ALIGN(2) Unk_0803B8CC
{
    i16 x;
    i16 y;
    STRUCT_PAD(0x04, 0x0B);
    u16 : 7;
    u16 state : 8;
    u16 : 1;
    u8 unk_0D;
};

struct Unk_0803BDFC
{
    Unk_0803B8CC * inner;
    void const * vtable;
};

EC int func_0803BAAC(Unk_0803B8CC &, int, int, int);
EC void func_0803BCF8(Unk_0803B8CC &);

ROM_TEXT(0803BD60) void func_0803BD60(Unk_0803B8CC & self)
{
    func_0803BCF8(self);
    self.state = 3;
    self.unk_0D = 9;
}

ROM_TEXT(0803BD60) void func_0803BD88(Unk_0803B8CC & self)
{
    if (self.state)
        self.state = 2;
}

ROM_TEXT(0803BD60) void func_0803BDB8(Unk_0803B8CC & self)
{
    if (self.state)
        self.state = 0;
}

ROM_TEXT(0803BD60) void func_0803BDE4(Unk_0803B8CC & self, int x, int y)
{
    self.x = x;
    self.y = y;
}

ROM_TEXT(0803BD60) void func_0803BDEC(Unk_0803B8CC & self, int x, int y)
{
    self.x += x;
    self.y += y;
}

EC char const vtable_unk_080E7778[];
EC Unk_0803B8CC * func_0803B8CC(void *, int, int, int, int, int, int, int, int);
EC void func_080098AC(void *, int);
EC void func_080098DC(void *, int);

ROM_TEXT(0803BD60) Unk_0803BDFC & func_0803BDFC(Unk_0803BDFC & self, int x, int y, int width, int height, int arg5, int arg6, int arg7, int arg8)
{
    self.vtable = vtable_unk_080E7778;
    self.inner = func_0803B8CC(::operator new(0x64), x, y, width, height, arg5, arg6, arg7, arg8);
    return self;
}

ROM_TEXT(0803BD60) void func_0803BE50(Unk_0803BDFC & self, int flags)
{
    self.vtable = vtable_unk_080E7778;
    if (Unk_0803B8CC * inner = self.inner)
    {
        u8 * data = reinterpret_cast<u8 *>(inner);
        func_080098AC(data + 0x44, 2);
        func_080098AC(data + 0x2C, 2);
        func_080098DC(data + 0x10, 2);
        ::operator delete(inner);
    }
    if (flags & 1)
        ::operator delete(&self);
}

ROM_TEXT(0803BD60) unsigned func_0803BE9C(Unk_0803BDFC const & self)
{
    return self.inner->state;
}

ROM_TEXT(0803BD60) int func_0803BEB0(Unk_0803BDFC & self, int arg1, int arg2, int arg3)
{
    return func_0803BAAC(*self.inner, arg1, arg2, arg3);
}

ROM_TEXT(0803BD60) void func_0803BEBC(Unk_0803BDFC & self)
{
    func_0803BCF8(*self.inner);
}

ROM_TEXT(0803BD60) void func_0803BEC8(Unk_0803BDFC & self)
{
    func_0803BD60(*self.inner);
}

ROM_TEXT(0803BD60) void func_0803BED4(Unk_0803BDFC & self)
{
    func_0803BD88(*self.inner);
}

ROM_TEXT(0803BD60) void func_0803BEE0(Unk_0803BDFC & self)
{
    func_0803BDB8(*self.inner);
}

ROM_TEXT(0803BD60) int func_0803BEEC(Unk_0803BDFC const & self)
{
    return self.inner->x;
}

ROM_TEXT(0803BD60) int func_0803BEF4(Unk_0803BDFC const & self)
{
    return self.inner->y;
}

ROM_TEXT(0803BD60) void func_0803BEFC(Unk_0803BDFC & self, int x, int y)
{
    func_0803BDE4(*self.inner, x, y);
}

ROM_TEXT(0803BD60) void func_0803BF08(Unk_0803BDFC & self, int x, int y)
{
    func_0803BDEC(*self.inner, x, y);
}

struct Unk_0803BF78 : Unk_0803BDFC
{
    void * unk_08;
    u8 unk_0C;
    u8 unk_0D;
    u8 unk_0E;
    u8 unk_0F;
};

ROM_TEXT(0803C518) void func_0803C518(Unk_0803BF78 & self)
{
    func_0803BEBC(self);
    self.unk_0D = 1;
    self.unk_0F = 1;
}

ROM_TEXT(0803C518) void func_0803C52C(Unk_0803BF78 & self)
{
    func_0803BEC8(self);
    self.unk_0D = 1;
    self.unk_0F = 1;
}

ROM_TEXT(0803C518) u8 func_0803C540(Unk_0803BF78 const & self)
{
    return self.unk_0C;
}

ROM_TEXT(0803C518) void func_0803C544(Unk_0803BF78 & self)
{
    self.unk_0F = 0;
}

struct Unk_0803D404Data
{
    STRUCT_PAD(0x000, 0x248);
    u8 tens[0x20];
    u8 ones[0x20];
    STRUCT_PAD(0x288, 0x2B0);
    u8 tilesDirty;
    STRUCT_PAD(0x2B1, 0x2CC);
    u8 unk_2CC;
    STRUCT_PAD(0x2CD, 0x2F0);
    u8 unk_2F0;
    u8 unk_2F1;
    u16 unk_2F2;
    STRUCT_PAD(0x2F4, 0x334);
    u8 unk_334;
};

struct Unk_0803D404
{
    Unk_0803D404Data * inner;
};

EC u8 const gUnk_0875A440[][0x20];

ROM_TEXT(0803D340) void func_0803D340(Unk_0803D404 & self, unsigned number)
{
    Unk_0803D404Data & inner = *self.inner;
    unsigned tens = number / 10;
    unsigned ones = number % 10;
    u8 const * tensTile = gUnk_0875A440[tens];
    memcpy(inner.tens, tensTile, sizeof(inner.tens));
    inner.tilesDirty = 1;
    u8 const * onesTile = gUnk_0875A440[ones];
    memcpy(inner.ones, onesTile, sizeof(inner.ones));
    inner.tilesDirty = 1;
}

ROM_TEXT(0803D340) void func_0803D3A0(Unk_0803D404 & self, unsigned selection)
{
    Unk_0803D404Data & inner = *self.inner;
    unsigned current = inner.unk_2CC;
    switch (selection)
    {
        case 0: current = 0; break;
        case 1: current = 1; break;
        case 2: current = 2; break;
        case 3: current = 3; break;
        case 4: current = 4; break;
    }
    if (current != inner.unk_2CC)
    {
        inner.unk_2CC = current;
        inner.unk_2F0 = 1;
    }
}

ROM_TEXT(0803D340) void func_0803D404(Unk_0803D404 & self, int arg)
{
    Unk_0803D404Data * inner = self.inner;
    if (!inner->unk_2F1 || arg != inner->unk_2F2)
    {
        inner->unk_2F1 = 1;
        inner->unk_2F2 = arg;
        inner->unk_334 = 1;
    }
}

ROM_TEXT(0803D340) void func_0803D440(Unk_0803D404 & self)
{
    Unk_0803D404Data & inner = *self.inner;
    if (inner.unk_2F1)
    {
        inner.unk_2F1 = 0;
        inner.unk_334 = 1;
    }
}

EC u8 const gUnk_080F1A66[14];
EC u8 const * func_080E0E80(u8 const *, u8 const *, unsigned const &, int, int);

ROM_TEXT(0803D644) bool func_0803D644(unsigned month, unsigned day)
{
    unsigned date = month * 30;
    date--;
    date += day;
    u8 const * begin = gUnk_080F1A66;
    u8 const * end = begin + ARRAY_COUNT(gUnk_080F1A66);
    u8 const * found = func_080E0E80(begin, end, date, 0, 0);
    return found != end && date >= *found;
}

struct NpcScheduleRoster
{
    STRUCT_PAD(0x000, 0x070);
    Npc npc_070;
    Npc npc_084;
    Npc npc_098;
    STRUCT_PAD(0x0AC, 0x0B0);
    Npc npc_0B0;
    Npc npc_0C4;
    Npc npc_0D8;
    STRUCT_PAD(0x0EC, 0x0F0);
    Npc npc_0F0;
    Npc npc_104;
    Npc npc_118;
    Npc npc_12C;
    Npc npc_140;
    Npc npc_154;
    STRUCT_PAD(0x168, 0x16C);
    Npc npc_16C;
    Npc npc_180;
    Npc npc_194;
    Npc npc_1A8;
    Npc npc_1BC;
    Npc npc_1D0;
    Npc npc_1E4;
    STRUCT_PAD(0x1F8, 0x1FC);
    Npc npc_1FC;
    Npc npc_210;
    STRUCT_PAD(0x224, 0x228);
    Npc npc_228;
    Npc npc_23C;
    Npc npc_250;
    Npc npc_264;
    STRUCT_PAD(0x278, 0x27C);
    Npc npc_27C;
    Npc npc_290;
    STRUCT_PAD(0x2A4, 0x2A8);
    Npc npc_2A8;
    Npc npc_2BC;
    STRUCT_PAD(0x2D0, 0x310);
    Npc npc_310;
    STRUCT_PAD(0x324, 0x328);
    Npc npc_328;
};

EC ScheduleInfo const gUnk_080F280C;
extern ScheduleInfo const ScheduleInfo_Unk_080F1A80;
EC ScheduleInfo const gUnk_080F1FC0;
EC ScheduleInfo const gUnk_080F8678;
EC ScheduleInfo const gUnk_080F81BC;
EC ScheduleInfo const gUnk_080F77FC;
EC ScheduleInfo const gUnk_080F7294;
EC ScheduleInfo const gUnk_080F6370;
EC ScheduleInfo const gUnk_080F66C4;
EC ScheduleInfo const gUnk_080F49C0;
EC ScheduleInfo const gUnk_080F5540;
EC ScheduleInfo const gUnk_080F4D74;
EC ScheduleInfo const gUnk_080F59CC;
EC ScheduleInfo const gUnk_080F6B4C;
EC ScheduleInfo const gUnk_080F33B8;
EC ScheduleInfo const gUnk_080F61FC;
EC ScheduleInfo const gUnk_080F3408;
EC ScheduleInfo const gUnk_080F3FD8;
EC ScheduleInfo const gUnk_080F35E4;
EC ScheduleInfo const gUnk_080F3010;
EC ScheduleInfo const gUnk_080F5D94;
EC ScheduleInfo const gUnk_080F6DE8;
EC ScheduleInfo const gUnk_080F2AF8;
EC ScheduleInfo const gUnk_080F42F0;
EC ScheduleInfo const gUnk_080F43DC;
EC ScheduleInfo const gUnk_080F6FF8;
EC ScheduleInfo const gUnk_080F7B40;
EC ScheduleInfo const gUnk_080F2DC0;
EC ScheduleInfo const gUnk_080F597C;
EC ScheduleInfo const gUnk_080F6B10;
EC ScheduleInfo const gUnk_080F4974;
EC ScheduleInfo const gUnk_080F29C0;
EC Npc * GetActiveScheduleNpc(NpcScheduleRoster &);
EC void func_0803D688(Npc &, ScheduleInfo const &, void const *);

ROM_TEXT(0803D7E4) void func_0803D7E4(NpcScheduleRoster & self, void const * context)
{
    func_0803D688(self.npc_070, gUnk_080F280C, context);
    func_0803D688(self.npc_084, ScheduleInfo_Unk_080F1A80, context);
    func_0803D688(self.npc_098, gUnk_080F1FC0, context);
    func_0803D688(self.npc_0B0, gUnk_080F8678, context);
    func_0803D688(self.npc_0C4, gUnk_080F81BC, context);
    func_0803D688(self.npc_0D8, gUnk_080F77FC, context);
    func_0803D688(self.npc_0F0, gUnk_080F7294, context);
    func_0803D688(self.npc_104, gUnk_080F6370, context);
    func_0803D688(self.npc_118, gUnk_080F66C4, context);
    func_0803D688(self.npc_12C, gUnk_080F49C0, context);
    func_0803D688(self.npc_140, gUnk_080F5540, context);
    func_0803D688(self.npc_154, gUnk_080F4D74, context);
    func_0803D688(self.npc_16C, gUnk_080F59CC, context);
    func_0803D688(self.npc_180, gUnk_080F6B4C, context);
    func_0803D688(self.npc_194, gUnk_080F33B8, context);
    func_0803D688(self.npc_1A8, gUnk_080F61FC, context);
    func_0803D688(self.npc_1BC, gUnk_080F3408, context);
    func_0803D688(self.npc_1D0, gUnk_080F3FD8, context);
    func_0803D688(self.npc_1E4, gUnk_080F35E4, context);
    func_0803D688(self.npc_1FC, gUnk_080F3010, context);
    func_0803D688(self.npc_210, gUnk_080F5D94, context);
    func_0803D688(self.npc_228, gUnk_080F6DE8, context);
    func_0803D688(self.npc_23C, gUnk_080F2AF8, context);
    func_0803D688(self.npc_250, gUnk_080F42F0, context);
    func_0803D688(self.npc_264, gUnk_080F43DC, context);
    func_0803D688(self.npc_27C, gUnk_080F6FF8, context);
    func_0803D688(self.npc_290, gUnk_080F7B40, context);
    func_0803D688(self.npc_2A8, gUnk_080F2DC0, context);
    func_0803D688(self.npc_2BC, gUnk_080F597C, context);
    func_0803D688(self.npc_310, gUnk_080F6B10, context);
    func_0803D688(self.npc_328, gUnk_080F4974, context);

    if (Npc * npc = GetActiveScheduleNpc(self))
        func_0803D688(*npc, gUnk_080F29C0, context);
}

struct PACKED SavedCalendarDate
{
    u8 data[3];
};

struct CalendarDate
{
    u8 year;
    u8 season;
    u8 day;
    u8 weekday;
};

struct PACKED EventScheduleFlags
{
    u32 : 19;
    u32 unk_217A_3 : 2;
};

struct ScriptEventContext
{
    STRUCT_PAD(0x0000, 0x0008);
    u32 unk_08;
    STRUCT_PAD(0x000C, 0x0010);
    SavedCalendarDate calendar;
    STRUCT_PAD(0x0013, 0x1CD4);
    u8 unk_1CD4[4];
    STRUCT_PAD(0x1CD8, 0x1DAC);
    u32 unk_1DAC[2];
    STRUCT_PAD(0x1DB4, 0x1E28);
    Bachelorette npc_1E28;
    STRUCT_PAD(0x1E40, 0x215A);
    u8 unk_215A;
    STRUCT_PAD(0x215B, 0x2178);
    union
    {
        u32 unk_2178;
        EventScheduleFlags scheduleFlags;
    };
    STRUCT_PAD(0x217C, 0x2186);
    u8 : 5;
    u8 unk_2186_5 : 2;
    u8 : 1;
    u8 : 3;
    u8 unk_2187_3 : 2;
    u8 : 3;
    STRUCT_PAD(0x2188, 0x21A2);
    u8 unk_21A2;
    STRUCT_PAD(0x21A3, 0x21CA);
    u8 eventFlags;
};

EC CalendarDate & func_0803D52C(CalendarDate &, SavedCalendarDate const &);
EC unsigned func_080A0490(void const *);

ROM_TEXT(0803D7E4) unsigned func_0803DA24(void const * context)
{
    ScriptEventContext const & self = *static_cast<ScriptEventContext const *>(context);
    if (self.eventFlags & 0x0C)
        return 0;
    CalendarDate date;
    func_0803D52C(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (func_0803D644(season, day))
        return 4;
    if (self.unk_08 == 0)
    {
        if (weekday == 2)
            return 3;
        return 1;
    }
    return 2;
}

ROM_TEXT(0803DECC) unsigned func_0803DECC(ScriptEventContext const & self)
{
    CalendarDate date;
    func_0803D52C(date, self.calendar);
    if (func_0803D644(date.season, date.day))
        return 2;
    return 1;
}

ROM_TEXT(0803DECC) unsigned func_0803DEF8(ScriptEventContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 5)) < 0)
        return 0;
    CalendarDate date;
    func_0803D52C(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (func_0803D644(season, day))
        return 4;
    if (weekday == 3)
    {
        if (self.unk_08 == 0)
            return 2;
        return 3;
    }
    return 1;
}

ROM_TEXT(0803DECC) unsigned func_0803DF50(ScriptEventContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 5)) < 0)
        return 0;
    CalendarDate date;
    func_0803D52C(date, self.calendar);
    if (func_0803D644(date.season, date.day))
        return 2;
    return 1;
}

ROM_TEXT(0803DECC) unsigned func_0803DF94(ScriptEventContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 2)) < 0)
        return 0;
    CalendarDate date;
    func_0803D52C(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (func_0803D644(season, day))
        return 4;
    if (weekday == 2)
        return 2;
    if (weekday == 0)
        return 3;
    return 1;
}

ROM_TEXT(0803E1A4) unsigned func_0803E1A4(ScriptEventContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 2)) < 0)
        return 0;
    CalendarDate date;
    func_0803D52C(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (func_0803D644(season, day))
        return 4;
    if (self.unk_08 == 0)
    {
        if (weekday == 2)
            return 3;
        return 1;
    }
    return 2;
}

ROM_TEXT(0803E1A4) unsigned func_0803E1FC(ScriptEventContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 6)) < 0)
        return 0;
    CalendarDate date;
    func_0803D52C(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    if (func_0803D644(season, day))
        return 2;
    unsigned event = func_080A0490(self.unk_1CD4);
    if (season == 2 && day == 5 && event != 25)
        return 3;
    return 1;
}

ROM_TEXT(0803E40C) unsigned func_0803E40C(ScriptEventContext const & self)
{
    CalendarDate date;
    func_0803D52C(date, self.calendar);
    unsigned weekday = date.weekday;
    switch (self.unk_2187_3)
    {
    case 2:
        if (weekday == 0)
            return 1;
        break;
    }
    return 0;
}

ROM_TEXT(0803E40C) unsigned func_0803E448(ScriptEventContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 4)) < 0)
        return 0;
    CalendarDate date;
    func_0803D52C(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (func_0803D644(season, day))
        return 4;
    if (weekday == 1)
    {
        if (self.unk_08 == 0)
            return 2;
        return 3;
    }
    return 1;
}

ROM_TEXT(0803E620) unsigned func_0803E620(ScriptEventContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 4)) < 0)
        return 0;
    CalendarDate date;
    func_0803D52C(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (func_0803D644(season, day))
        return 5;
    if (weekday == 1)
    {
        if (self.unk_08 == 0)
            return 3;
        return 4;
    }
    if (self.unk_08 == 0)
        return 1;
    return 2;
}

ROM_TEXT(0803E6EC) unsigned func_0803E6EC(ScriptEventContext const & self)
{
    CalendarDate date;
    func_0803D52C(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (season == 3 && day == 2 && (self.unk_2178 & 0x43000) == 0x2000)
    {
        switch (self.scheduleFlags.unk_217A_3)
        {
        default: return 7;
        case 2: break;
        }
        return 0;
    }
    if (func_0803D644(season, day))
        return 6;
    if (weekday == 6)
    {
        if (self.unk_08 == 0)
            return 2;
        return 3;
    }
    if (weekday == 0)
    {
        if (self.unk_08 == 0)
            return 4;
        return 5;
    }
    return 1;
}

ROM_TEXT(0803E8E8) unsigned func_0803E8E8(ScriptEventContext const & self)
{
    unsigned weekday;
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 5)) >= 0)
    {
        CalendarDate date;
        func_0803D52C(date, self.calendar);
        unsigned season = date.season;
        unsigned day = date.day;
        weekday = date.weekday;
        if (season == 2 && day == 30 && (self.unk_21A2 >> 6) != 2)
            return 5;
        if (func_0803D644(season, day))
            return 4;
        if (static_cast<i32>(static_cast<u32>(self.unk_215A) << (31 - 3)) >= 0)
            goto active;
    }
    return 0;
active:
    if (weekday == 1 || weekday == 3)
        return 3;
    if (self.unk_08 == 0)
        return 1;
    return 2;
}

ROM_TEXT(0803EA5C) unsigned func_0803EA5C(ScriptEventContext const & self)
{
    CalendarDate date;
    func_0803D52C(date, self.calendar);
    unsigned weekday = date.weekday;
    switch (self.unk_2186_5)
    {
    case 2:
        if (weekday == 3)
            return 1;
        break;
    }
    return 0;
}

ROM_TEXT(0803EA5C) unsigned func_0803EA98(ScriptEventContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 5)) < 0)
        return 0;
    CalendarDate date;
    func_0803D52C(date, self.calendar);
    if (func_0803D644(date.season, date.day))
        return 2;
    return 1;
}

ROM_TEXT(0803EA5C) unsigned func_0803EADC(ScriptEventContext const & self)
{
    if (self.eventFlags & 0x7C)
        return 0;
    CalendarDate date;
    func_0803D52C(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (season == 2 && day == 3)
        return 9;
    if (func_0803D644(season, day))
        return 8;
    if (weekday == 1 || weekday == 3 || self.unk_08 != 0)
        return 1;
    return 2;
}

ROM_TEXT(0803EA5C) unsigned func_0803EB40(ScriptEventContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 3)) >= 0)
    {
        CalendarDate date;
        func_0803D52C(date, self.calendar);
        unsigned season = date.season;
        unsigned day = date.day;
        unsigned weekday = date.weekday;
        if (season == 1)
        {
            if (func_0803D644(1, day))
                return 4;
            if (weekday == 0)
            {
                if (self.unk_08 == 0)
                    return 2;
                return 3;
            }
            return 1;
        }
    }
    return 0;
}

ROM_TEXT(0803EA5C) unsigned func_0803EB9C(ScriptEventContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 4)) < 0)
        return 0;
    CalendarDate date;
    func_0803D52C(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    unsigned events = self.npc_1E28.GetRivalEventCount();
    if (events > 4)
    {
        if (func_0803D644(season, day))
            return 9;
        if (weekday == 4)
        {
            if (self.unk_08 == 0)
                return 6;
            return 7;
        }
        return 5;
    }
    if (func_0803D644(season, day))
        return 8;
    if (events != 0)
    {
        if (weekday == 1)
            return 3;
        if (weekday == 4)
            return 4;
        return 2;
    }
    return 1;
}

EC unsigned GetNpcMilestoneEventId(void const *);
EC bool IsNpcMilestoneEventReady(void const *);

ROM_TEXT(0803EA5C) unsigned func_0803EC44(ScriptEventContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 4)) < 0)
        return 0;
    CalendarDate date;
    func_0803D52C(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (func_0803D644(season, day))
        return 4;
    void const * npc = self.unk_1DAC;
    if (GetNpcMilestoneEventId(npc) != 0 && !IsNpcMilestoneEventReady(npc))
        return 5;
    if (weekday == 4)
    {
        if (self.unk_08 == 0)
            return 2;
        return 3;
    }
    return 1;
}

ROM_TEXT(0803EE50) unsigned func_0803EE50(ScriptEventContext const & self)
{
    CalendarDate date;
    func_0803D52C(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (func_0803D644(season, day))
        return 4;
    if (weekday == 1)
    {
        if (self.unk_08 == 0)
            return 2;
        return 3;
    }
    return 1;
}

EC Unk_0803BF78 & func_0803C028(Unk_0803BF78 &, void *, int, int, int, int, int);
EC void func_0803BE50(Unk_0803BDFC &, int);
EC void func_080E0C38(void *, int);
EC void func_080E0D70(void *, int);
EC char const vtable_unk_080E77A4[];
EC char const vtable_unk_080E77E0[];

struct Unk_0803C7A0
{
    void * inner;
};

EC void func_0803C7A0(Unk_0803C7A0 &, int);
EC void func_0803C5B0(Unk_0803C7A0 &);

struct Unk_0803C54C : Unk_0803BDFC
{
    Unk_0803C7A0 menu;
};

EC Unk_0803BDFC & func_0803BDFC(Unk_0803BDFC &, int, int, int, int, int, int, int, int);

EC void * func_080E0A94(void *);

ROM_TEXT(0803BF78) Unk_0803BF78 & func_0803BF78(Unk_0803BF78 & self, int arg1, int arg2, int arg3, int arg4, int arg5)
{
    func_0803BDFC(self, 0, 136, 240, 24, arg1, arg2, arg3, arg4);
    self.vtable = vtable_unk_080E77A4;
    self.unk_08 = func_080E0A94(::operator new(0x41C));
    self.unk_0C = arg5;
    self.unk_0D = 0;
    self.unk_0E = 1;
    self.unk_0F = 1;
    return self;
}

ROM_TEXT(0803BF78) void func_0803BFC8(Unk_0803BF78 & self, int flags)
{
    self.vtable = vtable_unk_080E77A4;
    if (self.unk_08)
        func_080E0C38(self.unk_08, 3);
    func_0803BE50(self, flags);
}

ROM_TEXT(0803C00C) Unk_0803BF78 & func_0803C00C(Unk_0803BF78 & self, void * arg1, int arg2, int arg3, int arg4, int arg5, int arg6)
{
    return func_0803C028(self, arg1, arg2, arg3, arg4, arg5, arg6);
}

ROM_TEXT(0803C518) Unk_0803C54C & func_0803C54C(Unk_0803C54C & self, int arg1, int arg2, int arg3, int arg4)
{
    func_0803BDFC(self, 128, 136, 112, 24, arg1, arg2, arg3, arg4);
    self.vtable = vtable_unk_080E77E0;
    func_0803C5B0(self.menu);
    return self;
}

ROM_TEXT(0803C518) void func_0803C588(Unk_0803C54C & self, int flags)
{
    self.vtable = vtable_unk_080E77E0;
    func_0803C7A0(self.menu, 2);
    func_0803BE50(self, flags);
}

ROM_TEXT(0803C7A0) void func_0803C7A0(Unk_0803C7A0 & self, int flags)
{
    void * inner = self.inner;
    if (inner != 0)
        func_080E0D70(inner, 3);
    flags &= 1;
    if (flags != 0)
        ::operator delete(&self);
}
