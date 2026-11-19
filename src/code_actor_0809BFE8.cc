#include "prelude.h"

#include "actor.hh"
#include "item.hh"

#include <new>
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

struct CursedToolProgress
{
    u8 active[6];
    u8 completed[6];
    u8 elapsed[6];
};

ROM_TEXT(0809C0A0) CursedToolProgress & InitCursedToolProgress(CursedToolProgress & self)
{
    unsigned i = 0;
    u8 zero = 0;
    for (CursedToolProgress * entry = &self; i < ARRAY_COUNT(self.active); ++i)
    {
        entry->active[i] = zero;
        entry->elapsed[i] = zero;
        entry->completed[i] = zero;
    }
    return self;
}

ROM_TEXT(0809C0A0) bool IsCursedToolForProgress(CursedToolProgress const &, Tool const & tool)
{
    switch (tool.GetId())
    {
    case TOOL_CURSED_SICKLE:
    case TOOL_CURSED_HOE:
    case TOOL_CURSED_AXE:
    case TOOL_CURSED_HAMMER:
    case TOOL_CURSED_WATERING_CAN:
    case TOOL_CURSED_FISHING_ROD:
        return true;
    default:
        return false;
    }
}

ROM_TEXT(0809C0A0) unsigned GetCursedToolSlot(CursedToolProgress const &, unsigned toolId)
{
    unsigned index = 1;
    switch (toolId)
    {
    case TOOL_CURSED_SICKLE: index = 0; break;
    case TOOL_CURSED_HOE: index = 1; break;
    case TOOL_CURSED_AXE: index = 2; break;
    case TOOL_CURSED_HAMMER: index = 3; break;
    case TOOL_CURSED_WATERING_CAN: index = 4; break;
    case TOOL_CURSED_FISHING_ROD: index = 5; break;
    }
    return index;
}

ROM_TEXT(0809C0A0) u8 GetCursedToolActive(CursedToolProgress const & self, unsigned toolId)
{
    return self.active[GetCursedToolSlot(self, toolId)];
}

ROM_TEXT(0809C0A0) u8 GetCursedToolCompleted(CursedToolProgress const & self, unsigned toolId)
{
    return self.completed[GetCursedToolSlot(self, toolId)];
}

static inline bool HasStartedCursedToolProgress(CursedToolProgress const & self, unsigned index)
{
    int completed = -self.completed[index];
    int active = -self.active[index];
    active |= completed;
    return active < 0;
}

ROM_TEXT(0809C0A0) bool HaveAllCursedToolsStarted(CursedToolProgress const & self)
{
    return HasStartedCursedToolProgress(self, 1) && HasStartedCursedToolProgress(self, 0)
        && HasStartedCursedToolProgress(self, 2) && HasStartedCursedToolProgress(self, 3)
        && HasStartedCursedToolProgress(self, 4) && HasStartedCursedToolProgress(self, 5);
}

ROM_TEXT(0809C0A0) bool HaveAllCursedToolsCompleted(CursedToolProgress const & self)
{
    return self.completed[1] && self.completed[0] && self.completed[2] && self.completed[3]
        && self.completed[4] && self.completed[5];
}

ROM_TEXT(0809C0A0) void StartCursedToolProgress(CursedToolProgress & self, unsigned toolId)
{
    unsigned index = GetCursedToolSlot(self, toolId);
    if (self.active[index] == 0)
    {
        self.active[index] = 1;
        self.elapsed[index] = 0;
    }
}

EC u8 const gCursedToolProgressThresholds[6];

ROM_TEXT(0809C0A0) bool AdvanceCursedToolProgress(CursedToolProgress & self, unsigned index)
{
    bool completed = false;
    if (self.active[index])
    {
        if (self.elapsed[index] < gCursedToolProgressThresholds[index])
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

ROM_TEXT(0809C0A0) void ResetSickleOrHammerProgress(CursedToolProgress & self, unsigned index)
{
    if (self.active[index] && (index == 0 || index == 3))
        self.elapsed[index] = 0;
}

ROM_TEXT(0809C0A0) bool AdvanceCursedSickleOrHammerProgress(CursedToolProgress & self, unsigned toolId)
{
    bool completed = false;
    unsigned index = GetCursedToolSlot(self, toolId);
    if (self.active[index] && (index == 0 || index == 3))
        completed = AdvanceCursedToolProgress(self, index);
    return completed;
}

ROM_TEXT(0809C0A0) bool UpdateCursedToolProgressForScript(CursedToolProgress & self, unsigned toolId)
{
    unsigned index = GetCursedToolSlot(self, toolId);
    bool completed = false;
    if (self.active[index])
    {
        if (index == 1 || index == 4)
            completed = AdvanceCursedToolProgress(self, index);
        if (index == 0 || index == 3)
            ResetSickleOrHammerProgress(self, index);
    }
    return completed;
}

ROM_TEXT(0809C0A0) bool AdvanceCursedAxeOrRodProgress(CursedToolProgress & self, unsigned toolId)
{
    unsigned index = GetCursedToolSlot(self, toolId);
    bool completed = false;
    if (self.active[index] && (index == 5 || index == 2))
        completed = AdvanceCursedToolProgress(self, index);
    return completed;
}

ROM_TEXT(0809C0A0) void ResetMapPatchVariants(u32 & flags)
{
    flags = 0;
}

ROM_TEXT(0809C4EC) bool IsDefaultMapPatch(u32 const & flags, unsigned index)
{
    unsigned hasAlternatePatch = 0;
    if (index <= 13)
    {
        unsigned bit = flags & (1u << (index & 31));
        hasAlternatePatch = ((-bit) | bit) >> 31;
    }
    // Preserve the original nonzero test and one-bit inversion.
    return (hasAlternatePatch | 1) ^ hasAlternatePatch;
}

ROM_TEXT(0809C5B4) void SetAlternateMapPatch(u32 & flags, unsigned index)
{
    if (index <= 13)
        flags |= 1u << (index & 31);
}

ROM_TEXT(0809C5B4) void ClearAlternateMapPatch(u32 & flags, unsigned index)
{
    if (index <= 13)
        flags &= ~(1u << (index & 31));
}

struct ScriptEventSelection
{
    u32 pendingCount;
    u16 pendingIds[3];
    STRUCT_PAD(0x0A, 0x0C);
    u16 selectedId;
};

EC u16 * func_080E3DB4(u16 *, u16 *, u16 const &);
EC u16 * func_080E3E28(u16 *, u16 *, u16 const &);

ROM_TEXT(0809C5B4) void ClearPendingScriptEvents(ScriptEventSelection & self)
{
    self.pendingCount = 0;
}

ROM_TEXT(0809C5B4) bool HasPendingScriptEvents(ScriptEventSelection const & self)
{
    return self.pendingCount != 0;
}

ROM_TEXT(0809C5B4) void AddPendingScriptEvent(ScriptEventSelection & self, u16 eventId)
{
    u16 * begin = self.pendingIds;
    unsigned endOffset = offsetof(ScriptEventSelection, pendingIds) + self.pendingCount * sizeof(eventId);
    u16 * end = reinterpret_cast<u16 *>(reinterpret_cast<uintptr_t>(&self) + endOffset);
    if (func_080E3DB4(begin, end, eventId) == end)
    {
        u16 const & sourceId = eventId;
        unsigned pendingCount = self.pendingCount;
        if (pendingCount < ARRAY_COUNT(self.pendingIds))
        {
            unsigned offset = offsetof(ScriptEventSelection, pendingIds) + pendingCount * sizeof(eventId);
            new (reinterpret_cast<char *>(&self) + offset) u16(sourceId);
            ++self.pendingCount;
        }
    }
}

ROM_TEXT(0809C5B4) void RemovePendingScriptEvent(ScriptEventSelection & self, u16 eventId)
{
    u16 * begin = self.pendingIds;
    unsigned offset = offsetof(ScriptEventSelection, pendingIds) + self.pendingCount * sizeof(eventId);
    u16 * end = reinterpret_cast<u16 *>(reinterpret_cast<uintptr_t>(&self) + offset);
    u16 * found = func_080E3E28(begin, end, eventId);
    if (found != end && self.pendingCount)
    {
        u16 * last = self.pendingIds + self.pendingCount;
        u16 * next = found + 1;
        if (next != last)
        {
            // Keep both comparisons to match the inlined copy.
            if (last != next)
                memmove(found, next, reinterpret_cast<char *>(last) - reinterpret_cast<char *>(next));
        }
        --self.pendingCount;
    }
}

ROM_TEXT(0809C5B4) bool IsSelectedScriptEvent(ScriptEventSelection const & self, u16 eventId)
{
    return self.selectedId == eventId;
}

ROM_TEXT(0809C5B4) void SelectScriptEvent(ScriptEventSelection & self, u16 eventId)
{
    self.selectedId = eventId;
}

ROM_TEXT(0809C5B4) void DeselectScriptEvent(ScriptEventSelection & self)
{
    self.selectedId = 0xFFFF;
}

enum { FIRST_FISH_CATCH_ID = 8, MAX_CATCH_COUNT = 1000000000 };

struct FishingRecord
{
    u32 catchCount;
    u32 longestLength;
};

struct FishingRecords
{
    FishingRecord records[59];
};

ROM_TEXT(0809CD78) FishingRecords & ResetFishingRecords(FishingRecords & self)
{
    FishingRecord * record = self.records;
    for (int remaining = ARRAY_COUNT(self.records) - 1; remaining != -1; ++record, --remaining)
    {
        record->catchCount = 0;
        record->longestLength = 0;
    }
    return self;
}

ROM_TEXT(0809CD78) bool RecordFishingCatch(FishingRecords & self, unsigned catchId, u32 length)
{
    bool improved = false;
    if (self.records[catchId].catchCount < MAX_CATCH_COUNT)
        ++self.records[catchId].catchCount;
    if (length > self.records[catchId].longestLength)
    {
        self.records[catchId].longestLength = length;
        improved = true;
    }
    return improved;
}

ROM_TEXT(0809CD78) bool HasCaughtEveryFish(FishingRecords const & self)
{
    bool allRecorded = true;
    for (unsigned i = FIRST_FISH_CATCH_ID; i < ARRAY_COUNT(self.records); ++i)
        if (self.records[i].catchCount == 0)
            allRecorded = false;
    return allRecorded;
}

ROM_TEXT(0809CD78) u32 GetTotalFishCaught(FishingRecords const & self)
{
    u32 total = 0;
    for (unsigned i = FIRST_FISH_CATCH_ID; i < ARRAY_COUNT(self.records); ++i)
    {
        total += self.records[i].catchCount;
        if (total >= MAX_CATCH_COUNT)
            total = MAX_CATCH_COUNT;
    }
    return total;
}

ROM_TEXT(0809CD78) u32 GetFishingCatchCount(FishingRecords const & self, unsigned catchId)
{
    return self.records[catchId].catchCount;
}

ROM_TEXT(0809CD78) u32 GetLongestFishLength(FishingRecords const & self, unsigned catchId)
{
    return self.records[catchId].longestLength;
}

ROM_TEXT(0809CD78) unsigned GetKingFishIconId(FishingRecords const &, unsigned catchId)
{
    switch (catchId)
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

EC char const * const gFishingCatchNames[];

ROM_TEXT(0809CD78) char const * GetFishingCatchName(FishingRecords const &, unsigned catchId)
{
    return gFishingCatchNames[catchId];
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

ROM_TEXT(0809D418) unsigned GetOutcomeChanceRow(Unk_0809D8A0 const &, unsigned value)
{
    unsigned row = 0;
    if (value == 0)
        goto done;
    if (value <= 9)
        return 1;
    if (value <= 99)
    {
        row = 2;
        for (unsigned bound = 20; bound <= 99 && value >= bound; bound += 20)
            ++row;
        return row;
    }
    if (value == 100)
        return 7;
    if (value <= 249)
    {
        row = 8;
        for (unsigned bound = 130; bound <= 249 && value >= bound; bound += 30)
            ++row;
        return row;
    }
    row = 14;
    if (value > 254)
        goto done;
    row = 13;
done:
    return row;
}

ROM_TEXT(0809D418) unsigned func_0809D470(Unk_0809D8A0 const &, unsigned value)
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
