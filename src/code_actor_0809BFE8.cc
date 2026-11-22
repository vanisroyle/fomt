#include "prelude.h"

#include "actor.hh"
#include "farmer.hh"
#include "furniture.hh"
#include "item.hh"
#include "rucksack.hh"

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

enum
{
    LAST_PATCH_WITH_ALTERNATE = 13,
    WINTER_VARIANT_PATCH_INDEX = 13,
};

static inline unsigned HasAlternateMapPatch(u32 const & flags, unsigned index)
{
    unsigned hasAlternatePatch = 0;
    if (index <= LAST_PATCH_WITH_ALTERNATE)
    {
        unsigned bit = flags & (1u << (index & 31));
        hasAlternatePatch = ((-bit) | bit) >> 31;
    }
    return hasAlternatePatch;
}

ROM_TEXT(0809C4EC) bool IsDefaultMapPatch(u32 const & flags, unsigned index)
{
    unsigned hasAlternatePatch = HasAlternateMapPatch(flags, index);
    // Preserve the original nonzero test and one-bit inversion.
    return (hasAlternatePatch | 1) ^ hasAlternatePatch;
}

struct MapPatchTiles;

struct MapPatchVariant
{
    MapPatchTiles const * defaultTiles;
    MapPatchTiles const * alternateTiles;
    u8 tileX;
    u8 tileY;
};

struct MapPatch
{
    MapPatchTiles const * tiles;
    unsigned tileX;
    unsigned tileY;
};

EC MapPatchVariant const gMapPatchVariants[];
EC MapPatchTiles const gWinterAlternatePatchTiles;
EC MapPatchTiles const gWinterDefaultPatchTiles;

static inline MapPatchVariant const & GetMapPatchVariant(unsigned index)
{
    uintptr_t tableBase = reinterpret_cast<uintptr_t>(gMapPatchVariants);
    unsigned offset = index * sizeof(MapPatchVariant);
    return *reinterpret_cast<MapPatchVariant const *>(tableBase + offset);
}

ROM_TEXT(0809C510) MapPatch & GetMapPatch(MapPatch & result, u32 const & flags, unsigned index, bool isWinter)
{
    MapPatchTiles const * tiles;
    // Keep the original load order at the shared result assignment.
    unsigned tileX;
    register unsigned tileY asm("r2");
    if (HasAlternateMapPatch(flags, index))
    {
        if (index == WINTER_VARIANT_PATCH_INDEX && isWinter)
        {
            MapPatchVariant const & variant = GetMapPatchVariant(index);
            tileX = variant.tileX;
            tileY = variant.tileY;
            tiles = &gWinterAlternatePatchTiles;
            goto assign;
        }
        // agbcp must form the alternate pointer and coordinate addresses separately.
        uintptr_t tableBase = reinterpret_cast<uintptr_t>(gMapPatchVariants);
        unsigned offset = index * sizeof(MapPatchVariant);
        register uintptr_t alternateBase asm("r1") = tableBase + offsetof(MapPatchVariant, alternateTiles);
        MapPatchTiles const * const * alternate =
            reinterpret_cast<MapPatchTiles const * const *>(offset + alternateBase);
        MapPatchVariant const * variant =
            reinterpret_cast<MapPatchVariant const *>(offset + tableBase);
        unsigned alternateX = variant->tileX;
        unsigned alternateY = variant->tileY;
        result.tiles = *alternate;
        result.tileX = alternateX;
        result.tileY = alternateY;
    }
    else
    {
        if (index == WINTER_VARIANT_PATCH_INDEX && isWinter)
        {
            MapPatchVariant const & variant = GetMapPatchVariant(index);
            tileX = variant.tileX;
            tileY = variant.tileY;
            tiles = &gWinterDefaultPatchTiles;
            goto assign;
        }
        {
            MapPatchVariant const & variant = GetMapPatchVariant(index);
            tileX = variant.tileX;
            tileY = variant.tileY;
            tiles = variant.defaultTiles;
        }
    assign:
        result.tiles = tiles;
        result.tileX = tileX;
        result.tileY = tileY;
    }
    return result;
}

ROM_TEXT(0809C5B4) void SetAlternateMapPatch(u32 & flags, unsigned index)
{
    if (index <= LAST_PATCH_WITH_ALTERNATE)
        flags |= 1u << (index & 31);
}

ROM_TEXT(0809C5B4) void ClearAlternateMapPatch(u32 & flags, unsigned index)
{
    if (index <= LAST_PATCH_WITH_ALTERNATE)
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

enum MineTileState
{
    MINE_TILE_SOIL,
    MINE_TILE_DUG_SOIL,
    MINE_TILE_STAIRS_DOWN,
    MINE_TILE_STAIRS_UP,
    MINE_TILE_ROCK,
};

enum MineOutcome
{
    MINE_OUTCOME_NONE,
    MINE_OUTCOME_STAIRS,
    MINE_OUTCOME_CURSED_SICKLE = 4,
    MINE_OUTCOME_CURSED_HOE,
    MINE_OUTCOME_CURSED_AXE,
    MINE_OUTCOME_CURSED_HAMMER,
    MINE_OUTCOME_CURSED_WATERING_CAN,
    MINE_OUTCOME_CURSED_FISHING_ROD,
};

struct MineFloor
{
    u32 kind;
    struct PACKED ALIGN(2) Tile
    {
        u16 state : 4;
        u16 rockOutcome : 6;
        u16 digOutcome : 6;
    };
    Tile tiles[28][28];
    u8 kappaJewelFlags;
    u8 sharedJewelFlags;
    u8 goddessJewelFlags;
    u8 unk_627;
};

ROM_TEXT(0809CE8C) MineFloor & InitializeMineFloor(MineFloor & floor)
{
    MineFloor * const mine = &floor;
    mine->kind = 0;
    u8 * kappaFlags = &mine->kappaJewelFlags;

    // Goddess jewels 1–3 share the previous byte; preserve bits 6–7 here.
    // Keep the individual masks to reproduce the original register allocation.
    int clearGoddessJewel4 = ~(1 << 0);
    int clearGoddessJewel5 = ~(1 << 1);
    int clearGoddessJewel6 = ~(1 << 2);
    register int clearGoddessJewel7 asm("r5") = ~(1 << 3);
    register int clearGoddessJewel8 asm("r6") = ~(1 << 4);
    register int clearGoddessJewel9 asm("r8") = ~(1 << 5);
    asm("" : "+r"(clearGoddessJewel7), "+r"(clearGoddessJewel8), "+r"(clearGoddessJewel9));
    register unsigned noKappaFlags asm("r1") = 0;
    *kappaFlags = noKappaFlags;
    u8 * sharedFlags = &mine->sharedJewelFlags;
    register unsigned noSharedFlags asm("r1") = 0;
    *sharedFlags = noSharedFlags;
    u8 * const goddessFlags = &mine->goddessJewelFlags;
    *goddessFlags = clearGoddessJewel4 & *goddessFlags & clearGoddessJewel5 & clearGoddessJewel6
        & clearGoddessJewel7 & clearGoddessJewel8 & clearGoddessJewel9;

    unsigned row = 0;
    do
    {
        unsigned column = 0;
        // Keep the original shift and subtraction for a 56-byte row.
        register unsigned rowOffset asm("r1") = row << 3;
        unsigned nextRow = row + 1;
        rowOffset -= row;
        rowOffset <<= 3;
        register MineFloor::Tile * tiles asm("r0") = mine->tiles[0];
        volatile MineFloor::Tile * tile = reinterpret_cast<volatile MineFloor::Tile *>(rowOffset + reinterpret_cast<uintptr_t>(tiles));
        do
        {
            tile->state = 0;
            tile->rockOutcome = MINE_OUTCOME_NONE;
            tile->digOutcome = MINE_OUTCOME_NONE;
            ++tile;
            ++column;
        }
        while (column < ARRAY_COUNT(mine->tiles[0]));
        row = nextRow;
    }
    while (row < ARRAY_COUNT(mine->tiles));
    return *mine;
}

struct ALIGN(4) MineFloorPosition
{
    enum { ReservedCapacity = 12 };

    u8 x;
    u8 y;
};

typedef char MineFloorPositionLayoutCheck[sizeof(MineFloorPosition) == 4 ? 1 : -1];

enum MineType
{
    SpringMine,
    LakeMine,
};

EC int rand();
EC int GetMineFloorWidth(MineFloor const &);
EC int GetMineFloorHeight(MineFloor const &);
EC void GenerateSpringMineOutcomes(MineFloor &, unsigned, int, MineFloorPosition const *);
EC void GenerateLakeMineOutcomes(MineFloor &, unsigned, int, MineFloorPosition const *);

ROM_TEXT(0809CF34) MineFloorPosition * GenerateMineFloor(MineFloorPosition * entry, MineFloor & floor,
    unsigned mineType, unsigned floorDepth, MineFloorPosition const * reservedWorldTiles, unsigned count)
{
    // Match the two-byte coordinates and word-sized copies in the original stack frame.
    struct Scratch
    {
        MineFloorPosition reservedTiles[MineFloorPosition::ReservedCapacity];
        MineFloorPosition entryTile;
        MineFloorPosition * result;
        unsigned selectedMine;
    } scratch;
    typedef char MineFloorScratchLayoutCheck[
        offsetof(Scratch, entryTile) == 0x30 &&
        offsetof(Scratch, result) == 0x34 &&
        offsetof(Scratch, selectedMine) == 0x38 ? 1 : -1];

    register MineFloorPosition * entryArg asm("r0") = entry;
    register MineFloor * floorArg asm("r1") = &floor;
    register unsigned mineArg asm("r2") = mineType;
    register unsigned depthArg asm("r3") = floorDepth;
    register MineFloor * mine asm("r6");
    register unsigned depth asm("r8");
    register unsigned reservedCount asm("sb");
    asm volatile(
        ".syntax unified\n\tstr r0, %0\n\tadds %2, r1, #0\n\t"
        "str r2, %1\n\tmov %3, r3\n\tldr r0, %9\n\tmov %4, r0\n\t"
        ".syntax divided"
        : "=m"(scratch.result), "=m"(scratch.selectedMine), "=l"(mine), "=h"(depth), "=h"(reservedCount)
        : "l"(entryArg), "l"(floorArg), "l"(mineArg), "l"(depthArg), "m"(count)
        : "cc", "memory");
    register MineFloorPosition * spawn asm("r1");
    asm("add %0, sp, %1" : "=l"(spawn) : "I"(offsetof(Scratch, entryTile)));
    spawn->x = 0;
    spawn->y = 0;
    register MineFloorPosition * current asm("r0") = scratch.reservedTiles;
    register unsigned index asm("r2");
    asm("mov %0, %1" : "=l"(index) : "I"(ARRAY_COUNT(scratch.reservedTiles) - 1), "l"(current));
    do
    {
        current->x = 0;
        current->y = 0;
        ++current;
    }
    while (--index != static_cast<unsigned>(-1));

    // Reset the packed tile fields with the original store widths.
    register unsigned stateMask asm("ip");
    unsigned row = 0;
    asm volatile("mov r1, %1\n\tneg r1, r1\n\tmov %0, r1"
        : "=h"(stateMask) : "I"(1 << 4), "l"(row) : "r1", "cc");
    do
    {
        unsigned column = 0;
        register unsigned rowOffset asm("r1") = row << 3;
        unsigned nextRow = row + 1;
        rowOffset -= row;
        rowOffset <<= 3;
        register MineFloor::Tile * tiles asm("r0");
        asm("add %0, %1, %2" : "=l"(tiles) : "l"(mine), "I"(offsetof(MineFloor, tiles)), "l"(rowOffset));
        volatile MineFloor::Tile * tile = reinterpret_cast<volatile MineFloor::Tile *>(rowOffset + reinterpret_cast<uintptr_t>(tiles));
        do
        {
            register unsigned state asm("r0");
            asm volatile("ldrb r1, [%1]\n\tmov %0, %2\n\tand %0, r1\n\tstrb %0, [%1]"
                : "=&l"(state) : "l"(tile), "h"(stateMask) : "r1", "cc", "memory");
            tile->rockOutcome = MINE_OUTCOME_NONE;
            tile->digOutcome = MINE_OUTCOME_NONE;
            ++tile;
            ++column;
        }
        while (column < ARRAY_COUNT(mine->tiles[0]));
        row = nextRow;
    }
    while (row < ARRAY_COUNT(mine->tiles));

    if (reservedWorldTiles)
        for (unsigned i = 0; i < reservedCount; ++i)
            scratch.reservedTiles[i] = reservedWorldTiles[i];

    // One generated mine tile spans two world-map tiles on each axis.
    enum { WorldOriginX = 2, WorldOriginY = 7, LogicalExtent = 28, InvalidTile = 0xff };
    register MineFloorPosition * pos asm("r1");
    register unsigned invalid asm("r3");
    asm(".syntax unified\n\tmovs %0, %2\n\tmov %1, sp\n\t.syntax divided"
        : "=l"(invalid), "=l"(pos) : "I"(InvalidTile));
    do
    {
        unsigned worldX = pos->x;
        unsigned localX;
        if (static_cast<u8>(worldX - WorldOriginX) >= 2 * LogicalExtent)
            localX = worldX | invalid;
        else
        {
            asm volatile("" : : : "memory");
            localX = (pos->x - WorldOriginX) >> 1;
        }
        pos->x = localX;
        unsigned worldY = pos->y;
        unsigned localY;
        if (static_cast<u8>(worldY - WorldOriginY) >= 2 * LogicalExtent)
            localY = worldY | invalid;
        else
        {
            asm volatile("" : : : "memory");
            localY = (pos->y - WorldOriginY) >> 1;
        }
        pos->y = localY;
        ++pos;
        register MineFloorPosition const * last asm("r0");
        asm("add %0, sp, %1" : "=l"(last) : "I"(sizeof(scratch.reservedTiles) - sizeof(*pos)));
        if (reinterpret_cast<int>(pos) > reinterpret_cast<int>(last))
            break;
    }
    while (true);

    enum FloorKind
    {
        Entrance,
        Large,
        Tall,
        Standard,
        LakeLanding,
    };
    enum RockCount
    {
        EntranceRocks = 20,
        LargeRocks = 300,
        TallRocks = 100,
        StandardRocks = 30,
        LakeLandingRocks = 40,
    };
    register unsigned rockCount asm("sl");
    register unsigned commonCount asm("r0");
    unsigned entranceDepth;
    asm volatile("mov %0, %1" : "=&l"(entranceDepth) : "h"(depth), "l"(mine)
        : "r0", "r2", "r3", "r4", "r5", "r7");
    if (entranceDepth == 0)
    {
        asm volatile("str %1, [%2]\n\tmov %0, %3" : "=l"(commonCount)
            : "l"(entranceDepth), "l"(mine), "I"(EntranceRocks) : "cc", "memory");
        goto usualCount;
    }
    unsigned specialDepth;
    asm volatile("mov %0, %1" : "=&l"(specialDepth) : "h"(depth), "l"(mine)
        : "r0", "r2", "r3", "r4", "r5", "r7");
    if (specialDepth == 9 && scratch.selectedMine == LakeMine)
    {
        mine->kind = LakeLanding;
        asm volatile("mov r1, %1\n\tmov %0, r1" : "=h"(rockCount) : "I"(LakeLandingRocks) : "r1", "cc");
        goto placeEntry;
    }
    else if (depth % 5 == 0)
    {
        mine->kind = Large;
        asm volatile("mov %0, %1\n\tlsl %0, %0, #1" : "=l"(commonCount) : "I"(LargeRocks / 2) : "cc");
        goto usualCount;
    }
    else if (depth % 3 == 0)
    {
        mine->kind = Tall;
        asm volatile("mov r1, %1\n\tmov %0, r1" : "=h"(rockCount) : "I"(TallRocks) : "r1", "cc");
        goto placeEntry;
    }
    else
    {
        mine->kind = Standard;
        asm volatile("mov %0, %1" : "=l"(commonCount) : "I"(StandardRocks) : "cc");
    }
usualCount:
    asm volatile("mov %0, %1" : "=h"(rockCount) : "l"(commonCount));
placeEntry:
    unsigned hasStairs;
    asm volatile("mov %0, %1" : "=&l"(hasStairs) : "h"(depth), "l"(mine)
        : "r0", "r2", "r3", "r4", "r5", "r7");
    if (hasStairs != 0)
    {
        int xRoll = rand();
        register unsigned west asm("sb") = (xRoll >> 8) % static_cast<unsigned>(GetMineFloorWidth(*mine) - 2);
        asm("" : "+h"(west));
        register unsigned x asm("r5") = west + 1;
        asm("" : "+l"(x));
        int yRoll = rand();
        register unsigned north asm("r7") = (yRoll >> 8) % static_cast<unsigned>(GetMineFloorHeight(*mine) - 2);
        asm("" : "+l"(north));
        unsigned y = north + 1;
        register unsigned columnBytes asm("r2") = x * sizeof(MineFloor::Tile);
        asm("" : "+l"(columnBytes));
        register unsigned rowBytes asm("r0") = ((y << 3) - y) << 3;
        asm("" : "+l"(rowBytes) : "l"(columnBytes));
        asm("add %0, %0, %1\n\tadd %0, %2, %0" : "+l"(columnBytes) : "l"(rowBytes), "l"(mine));
        asm volatile(
            ".syntax unified\n\tldrb r1, [%0, %1]\n\tmovs r0, %2\n\t"
            "negs r0, r0\n\tands r0, r1\n\tmovs r1, %3\n\torrs r0, r1\n\t"
            "strb r0, [%0, %1]\n\t.syntax divided"
            : : "l"(columnBytes), "I"(offsetof(MineFloor, tiles)), "I"(1 << 4), "I"(MINE_TILE_STAIRS_UP)
            : "r0", "r1", "cc", "memory");

        // Spawn beside the stairs and reserve that tile until outcome placement finishes.
        unsigned direction = (rand() >> 8) & 3;
        if (direction & 2)
        {
            if (direction & 1)
                x = west;
            else
                ++x;
        }
        else
        {
            if (direction & 1)
                y = north;
            else
                ++y;
        }
        register MineFloorPosition * spawn asm("r0");
        asm("add %0, sp, %1" : "=l"(spawn) : "I"(offsetof(Scratch, entryTile)));
        spawn->x = x;
        spawn->y = y;
        register u8 * tileBase asm("r1");
        asm volatile(
            ".syntax unified\n\tldrb %0, [%1]\n\t"
            "lsls %0, %0, #1\n\tldrb r2, [%1, #1]\n\tlsls r0, r2, #3\n\t"
            "subs r0, r0, r2\n\tlsls r0, r0, #3\n\tadds %0, %0, r0\n\t"
            "adds %0, %2, %0\n\t.syntax divided"
            : "=&l"(tileBase), "+l"(spawn)
            : "l"(mine)
            : "r2", "cc", "memory");
        asm volatile(
            ".syntax unified\n\tldrb r2, [%0, %1]\n\tmovs r0, %2\n\t"
            "negs r0, r0\n\tands r0, r2\n\tmovs r2, %3\n\torrs r0, r2\n\t"
            "strb r0, [%0, %1]\n\t.syntax divided"
            : : "l"(tileBase), "I"(offsetof(MineFloor, tiles)), "I"(1 << 4), "I"(MINE_TILE_DUG_SOIL)
            : "r0", "r2", "cc", "memory");
    }

    unsigned mineToGenerate;
    asm volatile("ldr %0, [sp, %1]" : "=&l"(mineToGenerate)
        : "I"(offsetof(Scratch, selectedMine)), "l"(mine)
        : "r0", "r2", "r3", "r4", "r5", "r7", "memory");
    if (mineToGenerate == SpringMine)
        GenerateSpringMineOutcomes(*mine, depth, rockCount, scratch.reservedTiles);
    else
        GenerateLakeMineOutcomes(*mine, depth, rockCount, scratch.reservedTiles);

    unsigned generatedDepth;
    asm volatile("mov %0, %1" : "=l"(generatedDepth) : "h"(depth));
    if (generatedDepth != 0)
    {
        register u8 * tileBase asm("r1");
        asm volatile(
            ".syntax unified\n\tadd r0, sp, %2\n\tldrb %0, [r0]\n\t"
            "lsls %0, %0, #1\n\tldrb r2, [r0, #1]\n\tlsls r0, r2, #3\n\t"
            "subs r0, r0, r2\n\tlsls r0, r0, #3\n\tadds %0, %0, r0\n\t"
            "adds %0, %1, %0\n\t.syntax divided"
            : "=l"(tileBase)
            : "l"(mine), "I"(offsetof(Scratch, entryTile)), "m"(scratch.entryTile)
            : "r0", "r2", "cc");
        register unsigned clearState asm("r0");
        asm volatile(".syntax unified\n\tldrb r2, [%1, %2]\n\tmovs %0, %3\n\t"
            "negs %0, %0\n\tands %0, r2\n\tstrb %0, [%1, %2]\n\t.syntax divided"
            : "=l"(clearState)
            : "l"(tileBase), "I"(offsetof(MineFloor, tiles)), "I"(1 << 4)
            : "r2", "cc", "memory");
    }

    asm volatile("ldr r0, %0\n\tldr r1, %1\n\tstr r0, [r1]"
        : : "m"(scratch.entryTile), "m"(scratch.result) : "r0", "r1", "memory");
    return scratch.result;
}

EC unsigned __umodsi3(unsigned, unsigned);
EC unsigned __udivsi3(unsigned, unsigned);
EC unsigned GetSpringMineDigWeightRow(MineFloor const &, unsigned);
EC unsigned GetSpringMineRockWeightRow(MineFloor const &, unsigned);
EC u32 const gSpringMineDigOutcomeIds[5];
EC u8 const gSpringMineDigOutcomeWeights[][5];
EC u32 const gSpringMineRockOutcomeIds[10];
EC u8 const gSpringMineRockOutcomeWeights[][10];

ROM_TEXT(0809D168) void GenerateSpringMineOutcomes(MineFloor & floor, unsigned depth, int rockCount,
    MineFloorPosition const * reservedTiles)
{
    struct Scratch
    {
        unsigned depth;
        int rockCount;
        MineFloorPosition const * reservedTiles;
        unsigned width;
        unsigned height;
        unsigned weightRow;
        unsigned firstTile;
        unsigned tileCount;
        int remainingRocks;
        unsigned nextTile;
    } scratch;
    typedef char SpringMineScratchLayoutCheck[
        sizeof(Scratch) == 0x28 && offsetof(Scratch, nextTile) == 0x24 ? 1 : -1];

    register MineFloor * floorArg asm("r0") = &floor;
    register unsigned depthArg asm("r1") = depth;
    register int rockCountArg asm("r2") = rockCount;
    register MineFloorPosition const * reservedArg asm("r3") = reservedTiles;
    register MineFloor * mine asm("sl");
    asm volatile("mov %0, %4\n\tstr %5, %1\n\tstr %6, %2\n\tstr %7, %3"
        : "=h"(mine), "=m"(scratch.depth), "=m"(scratch.rockCount), "=m"(scratch.reservedTiles)
        : "l"(floorArg), "l"(depthArg), "l"(rockCountArg), "l"(reservedArg));
    register unsigned widthResult asm("r0") = GetMineFloorWidth(*floorArg);
    asm volatile("str %1, %0" : "=m"(scratch.width) : "l"(widthResult));
    unsigned heightResult = GetMineFloorHeight(*mine);
    asm volatile("str %1, %0" : "=m"(scratch.height) : "l"(heightResult));
    register MineFloor const * weightMineArg asm("r0");
    register unsigned weightDepthArg asm("r1");
    // The fixed y register needs an explicit callee-save in agbcc.
    unsigned savedCalleeRegister;
    asm volatile("" : "=l"(savedCalleeRegister) : : "r0", "r1", "r2", "r3", "r4", "r5", "r6");
    asm("mov %0, %2\n\tldr %1, %3"
        : "=l"(weightMineArg), "=l"(weightDepthArg) : "h"(mine), "m"(scratch.depth));
    unsigned digRow = GetSpringMineDigWeightRow(*weightMineArg, weightDepthArg);
    asm volatile("str %1, %0" : "=m"(scratch.weightRow) : "l"(digRow));
    register int startingRoll asm("r0") = rand() >> 4;
    asm("" : "+l"(startingRoll));
    register unsigned tileCount asm("r4");
    asm volatile("ldr r1, %1\n\tldr r2, %2\n\tmov %0, r1\n\tmul %0, r2"
        : "=l"(tileCount) : "m"(scratch.height), "m"(scratch.width) : "r1", "r2", "cc");
    unsigned firstTile = startingRoll % tileCount;
    asm volatile("str %1, %0" : "=m"(scratch.firstTile) : "l"(firstTile));

    // Visit every tile once, wrapping from a randomly selected start.
    register unsigned tileCursor asm("r5") = 0;
    asm("" : "+l"(tileCursor));
    if (tileCursor < tileCount)
    {
        asm volatile("str %1, %0" : "=m"(scratch.tileCount) : "l"(tileCount));
        do
        {
            register unsigned tileIndex asm("r4");
            register unsigned limit asm("r0");
            asm volatile("ldr r3, %2\n\tadd %0, r3, %4\n\tldr %1, %3"
                : "=l"(tileIndex), "=l"(limit) : "m"(scratch.firstTile), "m"(scratch.tileCount), "l"(tileCursor) : "r3", "cc");
            if (tileIndex >= limit)
                tileIndex -= limit;
            register unsigned tileArg asm("r0");
            register unsigned widthArg asm("r1");
            asm("mov %0, %2\n\tldr %1, %3"
                : "=l"(tileArg), "=l"(widthArg) : "l"(tileIndex), "m"(scratch.width));
            register unsigned x asm("r8") = __umodsi3(tileArg, widthArg);
            asm("" : "+h"(x));
            asm("mov %0, %2\n\tldr %1, %3"
                : "=l"(tileArg), "=l"(widthArg) : "l"(tileIndex), "m"(scratch.width));
            register unsigned y asm("r7") = __udivsi3(tileArg, widthArg);
            asm("" : "+l"(y));
            register int state asm("r0");
            asm volatile("mov r1, %2\n\tlsl r0, r1, #1\n\tlsl r1, %3, #3\n\t"
                "sub r1, r1, %3\n\tlsl r1, r1, #3\n\tadd r0, r0, r1\n\t"
                "add r0, %1\n\tldrb r0, [r0, #4]\n\tlsl r0, r0, #28\n\tlsr r0, r0, #28"
                : "=l"(state) : "h"(mine), "h"(x), "l"(y) : "r1", "cc", "memory");
            asm volatile("add %1, #1\n\tstr %1, %0"
                : "=m"(scratch.nextTile), "+l"(tileCursor) : : "cc");
            if (state <= MINE_TILE_DUG_SOIL)
            {
                register int randomBits asm("r0") = rand();
                register int roll asm("r2");
                asm("asr %0, %1, #4\n\tmov r0, #255\n\tand %0, r0"
                    : "=l"(roll), "+l"(randomBits) : : "cc");
                register int outcomeIndex asm("r3") = 0;
                register unsigned rowScale asm("r6");
                asm("ldr r4, %1\n\tlsl %0, r4, #2"
                    : "=l"(rowScale) : "m"(scratch.weightRow), "l"(outcomeIndex) : "r4", "cc");
                register u8 const * weights asm("r4");
                asm("ldr %0, .LspringDigWeights" : "=l"(weights) : "l"(rowScale));
            rollDigOutcome:
                {
                    register int weight asm("r0");
                    asm volatile("ldr r1, %1\n\tadd r0, %2, r1\n\tadd r0, %3, r0\n\tadd r1, r0, %4\n\tldrb r0, [r1]"
                        : "=l"(weight) : "m"(scratch.weightRow), "l"(rowScale), "l"(outcomeIndex), "l"(weights)
                        : "r1", "cc", "memory");
                    if (weight != 0)
                    {
                        if (roll < weight)
                        {
                            register unsigned outcome asm("r3");
                            asm("ldr r1, .LspringDigIds\n\tlsl r0, %1, #2\n\tadd r0, r0, r1\n\tldr %0, [r0]"
                                : "=l"(outcome) : "l"(outcomeIndex) : "r0", "r1", "memory", "cc");
                            register unsigned columnBytes asm("sb");
                            register unsigned rowScale asm("ip");
                            asm volatile(
                                "mov r2, %3\n\tlsl r2, r2, #1\n\tmov %0, r2\n\t"
                                "lsl r4, %4, #3\n\tmov %1, r4"
                                : "=h"(columnBytes), "=h"(rowScale) : "l"(outcome), "h"(x), "l"(y) : "r2", "r4", "cc");
                            if (outcome != MINE_OUTCOME_STAIRS)
                                goto saveDigOutcome;
                            // Down stairs cannot touch the border or the up stairs.
                            asm("" : "+l"(y));
                            if (y == 0)
                                goto rejectBorder;
                            asm volatile("" : : "l"(y));
                            register unsigned lastRow asm("r0");
                            asm volatile("ldr %0, %1\n\tsub %0, #1"
                                : "=l"(lastRow) : "m"(scratch.height) : "cc");
                            if (y >= lastRow)
                                goto rejectBorder;
                            register unsigned lowX asm("r0");
                            asm("mov %0, %1" : "=l"(lowX) : "h"(x));
                            if (lowX == 0)
                                goto rejectBorder;
                            register unsigned lastColumn asm("r0");
                            asm volatile("ldr %0, %1\n\tsub %0, #1"
                                : "=l"(lastColumn) : "m"(scratch.width) : "cc");
                            if (x >= lastColumn)
                                goto rejectBorder;
                            register int row asm("r4");
                            register int rowEnd asm("r6");
                            asm("sub %0, %2, #1\n\tadd %1, %2, #2"
                                : "=l"(row), "=l"(rowEnd) : "l"(y) : "cc");
                            register int rowBytes asm("r5");
                            if (row < rowEnd)
                            {
                                asm("lsl r0, %1, #3\n\tsub r0, r0, %1\n\tlsl %0, r0, #3"
                                    : "=l"(rowBytes) : "l"(row) : "r0", "cc");
                                do
                                {
                                    if (row < 0)
                                        goto nextNeighborRow;
                                    register int rowLimit asm("r1");
                                    asm volatile("ldr %0, %1" : "=l"(rowLimit) : "m"(scratch.height));
                                    if (row >= rowLimit)
                                        goto nextNeighborRow;
                                    register int column asm("r2");
                                    register int columnEnd asm("r1");
                                    // The row scan skips a column only when it is negative and not below width.
                                    asm volatile(
                                        "mov %0, %3\n\tsub %0, #1\n\tmov %1, %3\n\tadd %1, #2\n\tb 2f\n\t"
                                        ".balign 4, 0\n"
                                        ".LspringDigWeights: .word gSpringMineDigOutcomeWeights\n"
                                        ".LspringDigIds: .word gSpringMineDigOutcomeIds\n"
                                        "1:\n\tadd %0, #1\n"
                                        "2:\n\tcmp %0, %1\n\tbge 4f\n\tcmp %0, #0\n\tbge 3f\n\t"
                                        "ldr r0, %6\n\tcmp %0, r0\n\tbge 1b\n"
                                        "3:\n\tlsl r0, %0, #1\n\tadd r0, r0, %4\n\tadd r0, %5\n\t"
                                        "ldrb r0, [r0, #4]\n\tlsl r0, r0, #28\n\tlsr r0, r0, #28\n\t"
                                        "cmp r0, %7\n\tbne 1b\n\tmov %2, #0\n"
                                        "4:"
                                        : "=&l"(column), "=&l"(columnEnd), "+l"(outcome)
                                        : "h"(x), "l"(rowBytes), "h"(mine), "m"(scratch.width), "I"(MINE_TILE_STAIRS_UP)
                                        : "r0", "cc", "memory");
                                    if (outcome == MINE_OUTCOME_NONE)
                                        goto saveDigOutcome;
                                nextNeighborRow:
                                    rowBytes += sizeof(mine->tiles[0]);
                                    ++row;
                                }
                                while (row < rowEnd);
                            }
                            goto saveDigOutcome;
                        rejectBorder:
                            asm("mov %0, #0\n\tmov r1, %3\n\tlsl r1, r1, #1\n\tmov %1, r1\n\t"
                                "lsl r2, %4, #3\n\tmov %2, r2"
                                : "=l"(outcome), "=h"(columnBytes), "=h"(rowScale) : "h"(x), "l"(y) : "r1", "r2", "cc");
                        saveDigOutcome:
                            asm volatile("mov r4, %1\n\tsub r1, r4, %2\n\tlsl r1, r1, #3\n\t"
                                "add r1, %3\n\tadd r1, %4\n\tlsl %0, %0, #2\n\t"
                                "ldrb r2, [r1, #5]\n\tmov r0, #3\n\tand r0, r2\n\torr r0, %0\n\tstrb r0, [r1, #5]"
                                : "+l"(outcome) : "h"(rowScale), "l"(y), "h"(columnBytes), "h"(mine)
                                : "r0", "r1", "r2", "r4", "cc", "memory");
                            goto digAttemptFinished;
                        }
                        roll -= weight;
                        if (roll < 0)
                            goto digAttemptFinished;
                    }
                    ++outcomeIndex;
                    if (outcomeIndex < static_cast<int>(ARRAY_COUNT(gSpringMineDigOutcomeIds)))
                        goto rollDigOutcome;
                }
            }
        digAttemptFinished:
            asm volatile("ldr %0, %1" : "=l"(tileCursor) : "m"(scratch.nextTile));
        }
        while (tileCursor < scratch.tileCount);
    }

    asm("mov %0, %2\n\tldr %1, %3"
        : "=l"(weightMineArg), "=l"(weightDepthArg) : "h"(mine), "m"(scratch.depth));
    unsigned rockRow = GetSpringMineRockWeightRow(*weightMineArg, weightDepthArg);
    asm volatile("str %1, %0" : "=m"(scratch.weightRow) : "l"(rockRow));
    enum { MaxRockAttempts = 9 };
    register int targetRocks asm("r1");
    asm volatile("ldr %0, %1" : "=l"(targetRocks) : "m"(scratch.rockCount));
    if (targetRocks <= 0)
        return;
    register int rocksRemaining asm("r2");
    do
    {
        register unsigned placed asm("r8");
        register int attempt asm("sb");
        asm volatile("mov r2, #0\n\tmov %0, r2\n\tmov r3, #1\n\tmov %1, r3\n\t"
            "ldr r4, %3\n\tsub r4, #1\n\tstr r4, %2"
            : "=h"(placed), "=h"(attempt), "=m"(scratch.remainingRocks) : "m"(scratch.rockCount)
            : "r2", "r3", "r4", "cc");
    tryRock:
        {
            register unsigned x asm("r4") = (rand() >> 8) % scratch.width;
            asm("" : "+l"(x));
            register unsigned y asm("r7") = (rand() >> 8) % scratch.height;
            asm("" : "+l"(y));
            register int soilBits asm("r0");
            asm volatile("lsl r1, %1, #1\n\tlsl r0, %2, #3\n\tsub r0, r0, %2\n\t"
                "lsl r0, r0, #3\n\tadd r1, r1, r0\n\tadd r1, %3\n\t"
                "ldrb r0, [r1, #4]\n\tlsl r0, r0, #28"
                : "=l"(soilBits) : "l"(x), "l"(y), "h"(mine) : "r1", "cc", "memory");
            if (soilBits != 0)
                goto rockAttemptFinished;

            register int reservationIndex asm("r2");
            register MineFloorPosition const * reserved asm("r1");
            register unsigned rowScale asm("r5");
            register unsigned outcomeIndex asm("ip");
            asm volatile("mov r0, #1\n\tmov %0, r0\n\tmov %1, #0\n\tldr %2, %3"
                : "=h"(placed), "=l"(reservationIndex), "=l"(reserved) : "m"(scratch.reservedTiles) : "r0", "cc");
            if (reserved->x != x || reserved->y != y)
                goto nextReservedTile;
            asm("mov %0, %1" : "=h"(placed) : "l"(reservationIndex));
            goto checkRockPlacement;
        applyRockOutcome:
            {
                register u8 * tileBase asm("r3");
                asm("sub r3, %2, %3\n\tlsl r3, r3, #3\n\tadd %0, %1, r3\n\tadd %0, %4"
                    : "=l"(tileBase) : "l"(x), "l"(rowScale), "l"(y), "h"(mine) : "cc");
                register u32 const * rockIds asm("r1") = gSpringMineRockOutcomeIds;
                asm("" : "+l"(rockIds) : "l"(tileBase));
                register unsigned previousBits asm("r0");
                register unsigned nextOutcomeBits asm("r2");
                asm volatile("mov r2, %4\n\tlsl r0, r2, #2\n\tadd r0, r0, %3\n\t"
                    "ldr r2, [r0]\n\tmov r0, #63\n\tand r2, r0\n\tlsl r2, r2, #4\n\t"
                    "ldrh r0, [%2, #4]"
                    : "=l"(previousBits), "=l"(nextOutcomeBits)
                    : "l"(tileBase), "l"(rockIds), "h"(outcomeIndex) : "memory", "cc");
                register unsigned rockMask asm("r4") = ~(63u << 4);
                asm volatile("mov r1, %3\n\tand %0, r1\n\torr %0, %1\n\tstrh %0, [%2, #4]"
                    : "+l"(previousBits) : "l"(nextOutcomeBits), "l"(tileBase), "l"(rockMask)
                    : "r1", "memory", "cc");
                goto rockAttemptFinished;
            }
        nextReservedTile:
            ++reservationIndex;
            if (reservationIndex < MineFloorPosition::ReservedCapacity)
            {
                asm volatile("lsl r0, %1, #2\n\tldr r3, %2\n\tadd %0, r0, r3"
                    : "=l"(reserved) : "l"(reservationIndex), "m"(scratch.reservedTiles) : "r0", "r3", "cc");
                if (reserved->x != x || reserved->y != y)
                    goto nextReservedTile;
                asm("mov r0, #0\n\tmov %0, r0" : "=h"(placed) : : "r0", "cc");
            }
        checkRockPlacement:
            register unsigned canPlace asm("r1");
            asm("mov %0, %1" : "=l"(canPlace) : "h"(placed));
            if (canPlace != 1)
                goto rockAttemptFinished;
            asm volatile("lsl %0, %0, #1\n\tlsl %1, %2, #3\n\tsub r1, %1, %2\n\t"
                "lsl r1, r1, #3\n\tadd r1, %0, r1\n\tadd r1, %3\n\t"
                "ldrb r0, [r1, #4]\n\tmov r3, #16\n\tneg r3, r3\n\tmov r2, r3\n\t"
                "and r0, r2\n\tmov r2, %4\n\torr r0, r2\n\tstrb r0, [r1, #4]"
                : "+l"(x), "=l"(rowScale) : "l"(y), "h"(mine), "I"(MINE_TILE_ROCK) : "r0", "r1", "r2", "r3", "cc", "memory");

            register int randomBits asm("r0") = rand();
            register int roll asm("r2");
            register u8 const * weights asm("r1");
            register unsigned rockRowScale asm("r6");
            asm volatile("asr %0, %3, #8\n\tmov r0, #255\n\tand %0, r0\n\tmov r0, #0\n\tmov %1, r0\n\t"
                "ldr r1, %4\n\tlsl %2, r1, #2"
                : "=l"(roll), "=h"(outcomeIndex), "=l"(rockRowScale), "+l"(randomBits)
                : "m"(scratch.weightRow), "l"(rowScale) : "r1", "cc");
            asm volatile("ldr %0, =gSpringMineRockOutcomeWeights\n\tldr r3, %1\n\tadd r0, %2, r3\n\t"
                "lsl r0, r0, #1\n\tadd %0, r0, %0"
                : "=l"(weights) : "m"(scratch.weightRow), "l"(rockRowScale) : "r0", "r3", "cc");
            register int lowOutcome asm("r3");
            do
            {
                register int weight asm("r0") = *weights;
                asm("" : "+l"(weight));
                if (weight != 0)
                {
                    if (roll < weight)
                        goto applyRockOutcome;
                    roll -= weight;
                    if (roll < 0)
                        goto rockAttemptFinished;
                }
                asm volatile("add %0, #1\n\tmov r0, #1\n\tadd %1, r0\n\tmov %2, %1"
                    : "+l"(weights), "+h"(outcomeIndex), "=l"(lowOutcome) : : "r0", "cc");
            } while (lowOutcome < static_cast<int>(ARRAY_COUNT(gSpringMineRockOutcomeIds)));
        }
    rockAttemptFinished:
        register unsigned didPlace asm("r4");
        asm("mov %0, %1" : "=l"(didPlace) : "h"(placed));
        if (!didPlace)
        {
            register int attemptNumber asm("r1");
            asm volatile("mov r0, #1\n\tadd %0, r0\n\tmov %1, %0"
                : "+h"(attempt), "=l"(attemptNumber) : : "r0", "cc");
            if (attemptNumber <= MaxRockAttempts)
            {
                asm volatile("");
                goto tryRock;
            }
        }
        asm volatile("ldr %0, %2\n\tstr %0, %1"
            : "=l"(rocksRemaining), "=m"(scratch.rockCount) : "m"(scratch.remainingRocks));
    } while (rocksRemaining > 0);
}

ROM_TEXT(0809D418) unsigned GetSpringMineDigWeightRow(MineFloor const &, unsigned floor)
{
    unsigned row = 0;
    if (floor == 0)
        goto done;
    if (floor <= 9)
        return 1;
    if (floor <= 99)
    {
        row = 2;
        for (unsigned bound = 20; bound <= 99 && floor >= bound; bound += 20)
            ++row;
        return row;
    }
    if (floor == 100)
        return 7;
    if (floor <= 249)
    {
        row = 8;
        for (unsigned bound = 130; bound <= 249 && floor >= bound; bound += 30)
            ++row;
        return row;
    }
    row = 14;
    if (floor > 254)
        goto done;
    row = 13;
done:
    return row;
}

ROM_TEXT(0809D418) unsigned GetSpringMineRockWeightRow(MineFloor const &, unsigned floor)
{
    switch (floor)
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

    unsigned result = floor;
    if (result <= 10)
        goto done;
    result = 11;
    if (floor <= 19)
        goto done;
    result = 13;
    if (floor <= 29)
        goto done;
    result = 15;
    if (floor <= 39)
        goto done;
    result = 19;
    if (floor > 230)
        goto done;
    result = 17;
done:
    return result;
}

EC unsigned GetLakeMineDigWeightRow(MineFloor const &, unsigned);
EC unsigned GetLakeMineRockWeightRow(MineFloor const &, unsigned);
EC u32 const gLakeMineDigOutcomeIds[10];
EC u8 const gLakeMineDigOutcomeWeights[][10];
EC u32 const gLakeMineRockOutcomeIds[14];
EC u8 const gLakeMineRockOutcomeWeights[][14];

ROM_TEXT(0809D500) void GenerateLakeMineOutcomes(MineFloor & floor, unsigned depth, int rockCount,
    MineFloorPosition const * reservedTiles)
{
    struct Scratch
    {
        unsigned depth;
        int rockCount;
        MineFloorPosition const * reservedTiles;
        unsigned width;
        unsigned height;
        unsigned weightRow;
        unsigned firstTile;
        unsigned tileCount;
        int remainingRocks;
        unsigned nextTile;
    } scratch;
    typedef char LakeMineScratchLayoutCheck[
        sizeof(Scratch) == 0x28 && offsetof(Scratch, nextTile) == 0x24 ? 1 : -1];

    register MineFloor * floorArg asm("r0") = &floor;
    register unsigned depthArg asm("r1") = depth;
    register int rockCountArg asm("r2") = rockCount;
    register MineFloorPosition const * reservedArg asm("r3") = reservedTiles;
    register MineFloor * mine asm("sl");
    asm volatile("mov %0, %4\n\tstr %5, %1\n\tstr %6, %2\n\tstr %7, %3"
        : "=h"(mine), "=m"(scratch.depth), "=m"(scratch.rockCount), "=m"(scratch.reservedTiles)
        : "l"(floorArg), "l"(depthArg), "l"(rockCountArg), "l"(reservedArg));
    register unsigned widthResult asm("r0") = GetMineFloorWidth(*floorArg);
    asm volatile("str %1, %0" : "=m"(scratch.width) : "l"(widthResult));
    unsigned heightResult = GetMineFloorHeight(*mine);
    asm volatile("str %1, %0" : "=m"(scratch.height) : "l"(heightResult));
    register MineFloor const * weightMineArg asm("r0");
    register unsigned weightDepthArg asm("r1");
    // The fixed y register needs an explicit callee-save in agbcc.
    unsigned savedCalleeRegister;
    asm volatile("" : "=l"(savedCalleeRegister) : : "r0", "r1", "r2", "r3", "r4", "r5", "r6");
    asm("mov %0, %2\n\tldr %1, %3"
        : "=l"(weightMineArg), "=l"(weightDepthArg) : "h"(mine), "m"(scratch.depth));
    unsigned digRow = GetLakeMineDigWeightRow(*weightMineArg, weightDepthArg);
    asm volatile("str %1, %0" : "=m"(scratch.weightRow) : "l"(digRow));
    register int startingRoll asm("r0") = rand() >> 4;
    asm("" : "+l"(startingRoll));
    register unsigned tileCount asm("r4");
    asm volatile("ldr r1, %1\n\tldr r2, %2\n\tmov %0, r1\n\tmul %0, r2"
        : "=l"(tileCount) : "m"(scratch.height), "m"(scratch.width) : "r1", "r2", "cc");
    unsigned firstTile = startingRoll % tileCount;
    asm volatile("str %1, %0" : "=m"(scratch.firstTile) : "l"(firstTile));

    // Visit every tile once, wrapping from a randomly selected start.
    register unsigned tileCursor asm("r5") = 0;
    asm("" : "+l"(tileCursor));
    if (tileCursor < tileCount)
    {
        asm volatile("str %1, %0" : "=m"(scratch.tileCount) : "l"(tileCount));
        do
        {
            register unsigned tileIndex asm("r4");
            register unsigned limit asm("r0");
            asm volatile("ldr r3, %2\n\tadd %0, r3, %4\n\tldr %1, %3"
                : "=l"(tileIndex), "=l"(limit) : "m"(scratch.firstTile), "m"(scratch.tileCount), "l"(tileCursor) : "r3", "cc");
            if (tileIndex >= limit)
                tileIndex -= limit;
            register unsigned tileArg asm("r0");
            register unsigned widthArg asm("r1");
            asm("mov %0, %2\n\tldr %1, %3"
                : "=l"(tileArg), "=l"(widthArg) : "l"(tileIndex), "m"(scratch.width));
            register unsigned x asm("r7") = __umodsi3(tileArg, widthArg);
            asm("" : "+l"(x));
            asm("mov %0, %2\n\tldr %1, %3"
                : "=l"(tileArg), "=l"(widthArg) : "l"(tileIndex), "m"(scratch.width));
            register unsigned y asm("r6") = __udivsi3(tileArg, widthArg);
            asm("" : "+l"(y));
            register int state asm("r0");
            asm volatile("lsl r0, %2, #1\n\tlsl r1, %3, #3\n\t"
                "sub r1, r1, %3\n\tlsl r1, r1, #3\n\tadd r0, r0, r1\n\t"
                "add r0, %1\n\tldrb r0, [r0, #4]\n\tlsl r0, r0, #28\n\tlsr r0, r0, #28"
                : "=l"(state) : "h"(mine), "l"(x), "l"(y) : "r1", "cc", "memory");
            asm volatile("add %1, #1\n\tstr %1, %0"
                : "=m"(scratch.nextTile), "+l"(tileCursor) : : "cc");
            if (state <= MINE_TILE_DUG_SOIL)
            {
                register int randomBits asm("r0") = rand();
                register int roll asm("r2");
                asm("asr %0, %1, #4\n\tmov r0, #255\n\tand %0, r0"
                    : "=l"(roll), "+l"(randomBits) : : "cc");
                register int outcomeIndex asm("r3") = 0;
                register u8 const * weights asm("r1");
                asm volatile("ldr r1, %1\n\tlsl r0, r1, #2\n\tldr %0, .LlakeDigWeights\n\t"
                    "ldr r4, %1\n\tadd r0, r0, r4\n\tlsl r0, r0, #1\n\tadd %0, r0, %0"
                    : "=l"(weights) : "m"(scratch.weightRow), "l"(outcomeIndex) : "r0", "r4", "cc");
            rollDigOutcome:
                {
                    register int weight asm("r0");
                    asm volatile("ldrb %0, [%1]" : "=l"(weight) : "l"(weights) : "memory");
                    if (weight != 0)
                    {
                        if (roll < weight)
                        {
                            register unsigned outcome asm("r3");
                            asm("ldr r1, .LlakeDigIds\n\tlsl r0, %1, #2\n\tadd r0, r0, r1\n\tldr %0, [r0]"
                                : "=l"(outcome) : "l"(outcomeIndex) : "r0", "r1", "memory", "cc");
                            register unsigned columnBytes asm("sb");
                            register unsigned rowScale asm("r8");
                            asm volatile("lsl r0, %3, #1\n\tmov %0, r0\n\tlsl r1, %4, #3\n\tmov %1, r1"
                                : "=h"(columnBytes), "=h"(rowScale) : "l"(outcome), "l"(x), "l"(y) : "r0", "r1", "cc");
                            if (outcome != MINE_OUTCOME_STAIRS)
                                goto saveDigOutcome;
                            // Down stairs cannot touch the border or the up stairs.
                            asm("" : "+l"(y));
                            if (y == 0)
                                goto rejectBorder;
                            asm volatile("" : : "l"(y));
                            register unsigned lastRow asm("r0");
                            asm volatile("ldr %0, %1\n\tsub %0, #1"
                                : "=l"(lastRow) : "m"(scratch.height) : "cc");
                            if (y >= lastRow)
                                goto rejectBorder;
                            asm("" : "+l"(x));
                            if (x == 0)
                                goto rejectBorder;
                            register unsigned lastColumn asm("r0");
                            asm volatile("ldr %0, %1\n\tsub %0, #1"
                                : "=l"(lastColumn) : "m"(scratch.width) : "cc");
                            if (x >= lastColumn)
                                goto rejectBorder;
                            register int row asm("r4");
                            register int rowEnd asm("ip");
                            asm("sub %0, %2, #1\n\tadd r2, %2, #2\n\tmov %1, r2"
                                : "=l"(row), "=h"(rowEnd) : "l"(y) : "r2", "cc");
                            register int rowBytes asm("r5");
                            if (row < rowEnd)
                            {
                                asm("lsl r0, %1, #3\n\tsub r0, r0, %1\n\tlsl %0, r0, #3"
                                    : "=l"(rowBytes) : "l"(row) : "r0", "cc");
                                do
                                {
                                    if (row < 0)
                                        goto nextNeighborRow;
                                    register int rowLimit asm("r0");
                                    asm volatile("ldr %0, %1" : "=l"(rowLimit) : "m"(scratch.height));
                                    if (row >= rowLimit)
                                        goto nextNeighborRow;
                                    register int column asm("r2");
                                    register int columnEnd asm("r1");
                                    // The row scan skips a column only when it is negative and not below width.
                                    asm volatile(
                                        "sub %0, %3, #1\n\tadd %1, %3, #2\n\tb 2f\n\t"
                                        ".balign 4, 0\n"
                                        ".LlakeDigWeights: .word gLakeMineDigOutcomeWeights\n"
                                        ".LlakeDigIds: .word gLakeMineDigOutcomeIds\n"
                                        "1:\n\tadd %0, #1\n"
                                        "2:\n\tcmp %0, %1\n\tbge 4f\n\tcmp %0, #0\n\tbge 3f\n\t"
                                        "ldr r0, %6\n\tcmp %0, r0\n\tbge 1b\n"
                                        "3:\n\tlsl r0, %0, #1\n\tadd r0, r0, %4\n\tadd r0, %5\n\t"
                                        "ldrb r0, [r0, #4]\n\tlsl r0, r0, #28\n\tlsr r0, r0, #28\n\t"
                                        "cmp r0, %7\n\tbne 1b\n\tmov %2, #0\n"
                                        "4:"
                                        : "=&l"(column), "=&l"(columnEnd), "+l"(outcome)
                                        : "l"(x), "l"(rowBytes), "h"(mine), "m"(scratch.width), "I"(MINE_TILE_STAIRS_UP)
                                        : "r0", "cc", "memory");
                                    if (outcome == MINE_OUTCOME_NONE)
                                        goto saveDigOutcome;
                                nextNeighborRow:
                                    rowBytes += sizeof(mine->tiles[0]);
                                    ++row;
                                }
                                while (row < rowEnd);
                            }
                            goto saveDigOutcome;
                        rejectBorder:
                            asm("mov %0, #0\n\tlsl %3, %3, #1\n\tmov %1, %3\n\tlsl r1, %4, #3\n\tmov %2, r1"
                                : "=l"(outcome), "=h"(columnBytes), "=h"(rowScale), "+l"(x) : "l"(y) : "r1", "cc");
                        saveDigOutcome:
                            asm volatile("mov r2, %1\n\tsub r1, r2, %2\n\tlsl r1, r1, #3\n\t"
                                "add r1, %3\n\tadd r1, %4\n\tlsl %0, %0, #2\n\t"
                                "ldrb r2, [r1, #5]\n\tmov r0, #3\n\tand r0, r2\n\torr r0, %0\n\tstrb r0, [r1, #5]"
                                : "+l"(outcome) : "h"(rowScale), "l"(y), "h"(columnBytes), "h"(mine)
                                : "r0", "r1", "r2", "cc", "memory");
                            goto digAttemptFinished;
                        }
                        roll -= weight;
                        if (roll < 0)
                            goto digAttemptFinished;
                    }
                    asm volatile("add %0, #1\n\tadd %1, #1" : "+l"(weights), "+l"(outcomeIndex) : : "cc");
                    if (outcomeIndex < static_cast<int>(ARRAY_COUNT(gLakeMineDigOutcomeIds)))
                        goto rollDigOutcome;
                }
            }
        digAttemptFinished:
            asm volatile("ldr %0, %1" : "=l"(tileCursor) : "m"(scratch.nextTile));
            register unsigned tileLimit asm("r3");
            asm volatile("ldr %0, %1" : "=l"(tileLimit) : "m"(scratch.tileCount));
            if (tileCursor >= tileLimit)
                break;
        }
        while (true);
    }

    asm("mov %0, %2\n\tldr %1, %3"
        : "=l"(weightMineArg), "=l"(weightDepthArg) : "h"(mine), "m"(scratch.depth));
    unsigned rockRow = GetLakeMineRockWeightRow(*weightMineArg, weightDepthArg);
    asm volatile("str %1, %0" : "=m"(scratch.weightRow) : "l"(rockRow));
    enum { MaxRockAttempts = 9 };
    register int targetRocks asm("r4");
    asm volatile("ldr %0, %1" : "=l"(targetRocks) : "m"(scratch.rockCount));
    if (targetRocks <= 0)
        return;
    register int rocksRemaining asm("r2");
    do
    {
        register unsigned placed asm("r8");
        register int attempt asm("sb");
        asm volatile("mov r0, #0\n\tmov %0, r0\n\tmov r1, #1\n\tmov %1, r1\n\t"
            "ldr r2, %3\n\tsub r2, #1\n\tstr r2, %2"
            : "=h"(placed), "=h"(attempt), "=m"(scratch.remainingRocks) : "m"(scratch.rockCount)
            : "r0", "r1", "r2", "cc");
    tryRock:
        {
            register unsigned x asm("r4") = (rand() >> 8) % scratch.width;
            asm("" : "+l"(x));
            register unsigned y asm("r6") = (rand() >> 8) % scratch.height;
            asm("" : "+l"(y));
            register int soilBits asm("r0");
            asm volatile("lsl r1, %1, #1\n\tlsl r0, %2, #3\n\tsub r0, r0, %2\n\t"
                "lsl r0, r0, #3\n\tadd r1, r1, r0\n\tadd r1, %3\n\t"
                "ldrb r0, [r1, #4]\n\tlsl r0, r0, #28"
                : "=l"(soilBits) : "l"(x), "l"(y), "h"(mine) : "r1", "cc", "memory");
            if (soilBits != 0)
                goto rockAttemptFinished;

            register int reservationIndex asm("r2");
            register MineFloorPosition const * reserved asm("r1");
            register unsigned rowScale asm("r5");
            register int outcomeIndex asm("r7");
            asm volatile("mov r3, #1\n\tmov %0, r3\n\tmov %1, #0\n\tldr %2, %3"
                : "=h"(placed), "=l"(reservationIndex), "=l"(reserved) : "m"(scratch.reservedTiles) : "r3", "cc");
            if (reserved->x != x || reserved->y != y)
                goto nextReservedTile;
            asm("mov %0, %1" : "=h"(placed) : "l"(reservationIndex));
            goto checkRockPlacement;
        applyRockOutcome:
            {
                register u8 * tileBase asm("r3");
                asm("sub r3, %2, %3\n\tlsl r3, r3, #3\n\tadd %0, %1, r3\n\tadd %0, %4"
                    : "=l"(tileBase) : "l"(x), "l"(rowScale), "l"(y), "h"(mine) : "cc");
                register u32 const * rockIds asm("r1") = gLakeMineRockOutcomeIds;
                asm("" : "+l"(rockIds) : "l"(tileBase));
                register unsigned previousBits asm("r0");
                register unsigned nextOutcomeBits asm("r2");
                asm volatile("lsl r0, %4, #2\n\tadd r0, r0, %3\n\t"
                    "ldr r2, [r0]\n\tmov r0, #63\n\tand r2, r0\n\tlsl r2, r2, #4\n\t"
                    "ldrh r0, [%2, #4]"
                    : "=l"(previousBits), "=l"(nextOutcomeBits)
                    : "l"(tileBase), "l"(rockIds), "l"(outcomeIndex) : "memory", "cc");
                register unsigned rockMask asm("r4") = ~(63u << 4);
                asm volatile("mov r1, %3\n\tand %0, r1\n\torr %0, %1\n\tstrh %0, [%2, #4]"
                    : "+l"(previousBits) : "l"(nextOutcomeBits), "l"(tileBase), "l"(rockMask)
                    : "r1", "memory", "cc");
                goto rockAttemptFinished;
            }
        nextReservedTile:
            ++reservationIndex;
            if (reservationIndex < MineFloorPosition::ReservedCapacity)
            {
                asm volatile("lsl r0, %1, #2\n\tldr r3, %2\n\tadd %0, r0, r3"
                    : "=l"(reserved) : "l"(reservationIndex), "m"(scratch.reservedTiles) : "r0", "r3", "cc");
                if (reserved->x != x || reserved->y != y)
                    goto nextReservedTile;
                asm("mov r0, #0\n\tmov %0, r0" : "=h"(placed) : : "r0", "cc");
            }
        checkRockPlacement:
            register unsigned canPlace asm("r1");
            asm("mov %0, %1" : "=l"(canPlace) : "h"(placed));
            if (canPlace != 1)
                goto rockAttemptFinished;
            asm volatile("lsl %0, %0, #1\n\tlsl %1, %2, #3\n\tsub r1, %1, %2\n\t"
                "lsl r1, r1, #3\n\tadd r1, %0, r1\n\tadd r1, %3\n\t"
                "ldrb r0, [r1, #4]\n\tmov r3, #16\n\tneg r3, r3\n\tmov r2, r3\n\t"
                "and r0, r2\n\tmov r2, %4\n\torr r0, r2\n\tstrb r0, [r1, #4]"
                : "+l"(x), "=l"(rowScale) : "l"(y), "h"(mine), "I"(MINE_TILE_ROCK) : "r0", "r1", "r2", "r3", "cc", "memory");

            register int randomBits asm("r0") = rand();
            register int roll asm("r2");
            register u8 const * weights asm("r1");
            asm volatile("asr %0, %3, #8\n\tmov r0, #255\n\tand %0, r0\n\tmov %1, #0\n\t"
                "ldr %2, =gLakeMineRockOutcomeWeights\n\tldr r3, %4\n\tlsl r0, r3, #3\n\t"
                "sub r0, r0, r3\n\tlsl r0, r0, #1\n\tadd %2, r0, %2"
                : "=l"(roll), "=l"(outcomeIndex), "=l"(weights), "+l"(randomBits)
                : "m"(scratch.weightRow), "l"(rowScale) : "r3", "cc");
            do
            {
                register int weight asm("r0") = *weights;
                asm("" : "+l"(weight));
                if (weight != 0)
                {
                    if (roll < weight)
                        goto applyRockOutcome;
                    roll -= weight;
                    if (roll < 0)
                        goto rockAttemptFinished;
                }
                asm volatile("add %0, #1\n\tadd %1, #1" : "+l"(weights), "+l"(outcomeIndex) : : "cc");
            } while (outcomeIndex < static_cast<int>(ARRAY_COUNT(gLakeMineRockOutcomeIds)));
        }
    rockAttemptFinished:
        register unsigned didPlace asm("r4");
        asm("mov %0, %1" : "=l"(didPlace) : "h"(placed));
        if (!didPlace)
        {
            register int attemptNumber asm("r1");
            asm volatile("mov r0, #1\n\tadd %0, r0\n\tmov %1, %0"
                : "+h"(attempt), "=l"(attemptNumber) : : "r0", "cc");
            if (attemptNumber <= MaxRockAttempts)
                goto tryRock;
        }
        asm volatile("ldr %0, %2\n\tstr %0, %1"
            : "=l"(rocksRemaining), "=m"(scratch.rockCount) : "m"(scratch.remainingRocks));
    } while (rocksRemaining > 0);
}

ROM_TEXT(0809D79C) unsigned GetLakeMineDigWeightRow(MineFloor const &, unsigned floor)
{
    unsigned row = floor;
    unsigned lastFloorInGroup;
    // Keep the floor input and the result in separate registers.
    asm("" : "+r"(row) : "r"(floor));
    if (row <= 9)
        goto done;
    row = 25;
    asm("" : "+r"(row) : "r"(floor));
    if (floor == 255)
        goto done;
    row = 24;
    asm("" : "+r"(row) : "r"(floor));
    if (floor > 79)
        goto done;
    row = 10;
    lastFloorInGroup = 19;
    if (floor < lastFloorInGroup)
        goto done;
    if (floor == lastFloorInGroup)
    {
        row = 11;
        goto done;
    }
    do
    {
        row += 2;
        lastFloorInGroup += 10;
        if (lastFloorInGroup > 79 || floor < lastFloorInGroup)
            goto done;
    }
    while (floor != lastFloorInGroup);
    ++row;
done:
    return row;
}

ROM_TEXT(0809D7D8) unsigned GetLakeMineRockWeightRow(MineFloor const &, unsigned floor)
{
    switch (floor)
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
    if (floor <= 20)
        return floor;
    if (floor > 250)
        return 54;
    if (floor <= 99)
        return floor % 10 + 20;
    if (floor > 199)
        return floor % 10 + 43;
    return floor % 10 + 32;
}

ROM_TEXT(0809D7D8) u32 GetMineFloorKind(MineFloor const & self)
{
    return self.kind;
}

ROM_TEXT(0809D7D8) int GetMineFloorWidth(MineFloor const & self)
{
    unsigned count = 13;
    if (self.kind == 1)
        count = 28;
    return count;
}

ROM_TEXT(0809D7D8) int GetMineFloorHeight(MineFloor const & self)
{
    unsigned count = 6;
    if (self.kind == 1)
        count = 28;
    else if (self.kind == 2)
        count = 14;
    return count;
}

ROM_TEXT(0809D7D8) unsigned GetMineTileState(MineFloor const & self, unsigned x, unsigned y)
{
    return self.tiles[y][x].state;
}

struct MineRewardContext
{
    STRUCT_PAD(0x0000, 0x0380);
    ToolChest toolChest;
    u8 padAfterToolChest[0x1C38 - 0x0380 - sizeof(ToolChest)];
    Rucksack rucksack;
};

EC int ResolveMineOutcome(MineFloor &, unsigned, MineRewardContext *);

ROM_TEXT(0809D8E8) int DigMineTile(MineFloor & mine, int x, int y, MineRewardContext * context)
{
    int result = MINE_OUTCOME_NONE;
    if (mine.tiles[y][x].state == MINE_TILE_SOIL)
    {
        result = ResolveMineOutcome(mine, mine.tiles[y][x].digOutcome, context);
        if (result == MINE_OUTCOME_STAIRS)
        {
            mine.tiles[y][x].state = MINE_TILE_STAIRS_DOWN;
            result = MINE_OUTCOME_NONE;
            for (int row = 0; row < GetMineFloorHeight(mine); ++row)
            {
                for (int column = 0; column < GetMineFloorWidth(mine); ++column)
                {
                    if (mine.tiles[row][column].digOutcome == MINE_OUTCOME_STAIRS)
                        mine.tiles[row][column].digOutcome = MINE_OUTCOME_NONE;
                }
            }
        }
        else
        {
            mine.tiles[y][x].state = MINE_TILE_DUG_SOIL;
        }
        mine.tiles[y][x].digOutcome = MINE_OUTCOME_NONE;
    }
    return result;
}

ROM_TEXT(0809D9B4) int BreakMineRock(MineFloor & mine, unsigned x, unsigned y, MineRewardContext * context)
{
    int result = MINE_OUTCOME_NONE;
    unsigned offset = x * sizeof(MineFloor::Tile);
    offset += y * sizeof(mine.tiles[0]);
    char * base = reinterpret_cast<char *>(&mine) + offset;
    u8 firstByte = static_cast<u8>(base[offsetof(MineFloor, tiles)]);
    unsigned state = (static_cast<u32>(firstByte) << 28) >> 28;
    switch (state)
    {
    case MINE_TILE_ROCK:
    {
        MineFloor::Tile & tile = *reinterpret_cast<MineFloor::Tile *>(base + offsetof(MineFloor, tiles));
        result = ResolveMineOutcome(mine, tile.rockOutcome, context);
        tile.state = MINE_TILE_SOIL;
        tile.rockOutcome = MINE_OUTCOME_NONE;
        break;
    }
    }
    return result;
}

ROM_TEXT(0809DF2C) bool HasCursedMineTool(MineFloor const &, unsigned outcome, MineRewardContext const & context)
{
    bool found = false;
    int toolId = TOOL_NONE;
    switch (outcome)
    {
    case MINE_OUTCOME_CURSED_SICKLE: toolId = TOOL_CURSED_SICKLE; break;
    case MINE_OUTCOME_CURSED_HOE: toolId = TOOL_CURSED_HOE; break;
    case MINE_OUTCOME_CURSED_AXE: toolId = TOOL_CURSED_AXE; break;
    case MINE_OUTCOME_CURSED_HAMMER: toolId = TOOL_CURSED_HAMMER; break;
    case MINE_OUTCOME_CURSED_WATERING_CAN: toolId = TOOL_CURSED_WATERING_CAN; break;
    case MINE_OUTCOME_CURSED_FISHING_ROD: toolId = TOOL_CURSED_FISHING_ROD; break;
    }
    if (toolId < NUM_TOOLS)
    {
        if (context.rucksack.GetFirstSlotWithTool(toolId) != static_cast<u32>(-1))
            found = true;
        if (context.toolChest.GetFirstSlotWith(toolId) != static_cast<u32>(-1))
            found = true;
    }
    return found;
}

ROM_TEXT(0809DFAC) u8 CountHarvestGoddessJewels(MineFloor const & mine)
{
    // The first three Goddess jewel flags share a byte with other mine rewards.
    unsigned bits = mine.sharedJewelFlags;
    u8 count = (bits << 26) >> 31;
    if (static_cast<i32>(bits << (31 - 6)) < 0) ++count;
    if (bits >> 7) ++count;
    bits = mine.goddessJewelFlags;
    if ((bits << 31) != 0) ++count;
    if (static_cast<i32>(bits << (31 - 1)) < 0) ++count;
    if (static_cast<i32>(bits << (31 - 2)) < 0) ++count;
    if (static_cast<i32>(bits << (31 - 3)) < 0) ++count;
    if (static_cast<i32>(bits << (31 - 4)) < 0) ++count;
    if (static_cast<i32>(bits << (31 - 5)) < 0) ++count;
    return count;
}

ROM_TEXT(0809E02C) u8 CountKappaJewels(MineFloor const & mine)
{
    // Bit 0 holds a different reward; the last two Kappa jewels use the next byte.
    unsigned bits = mine.kappaJewelFlags;
    u8 count = (bits << 30) >> 31;
    if (static_cast<i32>(bits << (31 - 2)) < 0) ++count;
    if (static_cast<i32>(bits << (31 - 3)) < 0) ++count;
    if (static_cast<i32>(bits << (31 - 4)) < 0) ++count;
    if (static_cast<i32>(bits << (31 - 5)) < 0) ++count;
    if (static_cast<i32>(bits << (31 - 6)) < 0) ++count;
    if (bits >> 7) ++count;
    bits = mine.sharedJewelFlags;
    if ((bits << 31) != 0) ++count;
    if (static_cast<i32>(bits << (31 - 1)) < 0) ++count;
    return count;
}

struct MineTileGraphics;
EC MineTileGraphics const gMineSoilTiles;
EC MineTileGraphics const gMineDugSoilTiles;
EC MineTileGraphics const gMineDownStairsTiles;
EC MineTileGraphics const gMineUpStairsTiles;
EC MineTileGraphics const gMineRockTiles;

ROM_TEXT(0809E0AC) MineTileGraphics const * GetMineTileGraphics(MineFloor const & mine, unsigned x, unsigned y)
{
    MineFloor::Tile const & tile = mine.tiles[y][x];
    switch (tile.state)
    {
        case MINE_TILE_SOIL: return &gMineSoilTiles;
        case MINE_TILE_DUG_SOIL: return &gMineDugSoilTiles;
        case MINE_TILE_STAIRS_DOWN: return &gMineDownStairsTiles;
        case MINE_TILE_STAIRS_UP: return &gMineUpStairsTiles;
        case MINE_TILE_ROCK: return &gMineRockTiles;
        default: return 0;
    }
}

ROM_TEXT(0809E118) void CountMineFloorOutcomes(MineFloor const & floor, u8 * outcomeCounts)
{
    register MineFloor const * mine asm("r5") = &floor;
    u8 * counts = outcomeCounts;
    register unsigned row asm("r6");
    for (row = 0; row < static_cast<unsigned>(GetMineFloorHeight(*mine)); )
    {
        register unsigned column asm("r4") = 0;
        register unsigned nextRow asm("r3") = row + 1;
        volatile unsigned savedNextRow;

        goto testColumn;
    countTile:
        {
            register unsigned columnOffset asm("r2") = column * sizeof(MineFloor::Tile);
            asm("" : "+r"(columnOffset));
            unsigned offset = columnOffset + row * sizeof(mine->tiles[0]);
            u8 const * tileBytes = reinterpret_cast<u8 const *>(mine) + offset + offsetof(MineFloor, tiles);
            MineFloor::Tile const & tile = *reinterpret_cast<MineFloor::Tile const *>(tileBytes);
            register u8 * rockCount asm("r0") = &counts[tile.rockOutcome];
            asm("" : "+r"(rockCount));
            ++*rockCount;
            // The dig outcome occupies the upper six bits of the second byte.
            unsigned digOutcome = reinterpret_cast<u8 const *>(&tile)[1] >> 2;
            ++counts[digOutcome];
            ++column;
        }
    testColumn:
        // Match the original spill of the next row across the width call.
        register MineFloor const * mineArg asm("r0") = mine;
        asm("" : "+r"(mineArg));
        savedNextRow = nextRow;
        register unsigned width asm("r0") = GetMineFloorWidth(*mineArg);
        nextRow = savedNextRow;
        if (column < width)
            goto countTile;
        row = nextRow;
    }
}

ROM_TEXT(0809E174) bool func_0809E174(MineFloor const & self)
{
    return (static_cast<u32>(self.kappaJewelFlags) << 31) >> 31;
}

ROM_TEXT(0809E174) bool func_0809E184(MineFloor const & self)
{
    return (static_cast<u32>(self.sharedJewelFlags) << (31 - 2)) >> 31;
}

ROM_TEXT(0809E174) bool func_0809E194(MineFloor const & self)
{
    return (static_cast<u32>(self.sharedJewelFlags) << (31 - 3)) >> 31;
}

ROM_TEXT(0809E174) bool func_0809E1A4(MineFloor const & self)
{
    return (static_cast<u32>(self.sharedJewelFlags) << (31 - 4)) >> 31;
}

struct MineWeightContext
{
    STRUCT_PAD(0, 0x1BD8);
    Farmer farmer;
};

static inline void CopyMineOutcomeWeights(u8 * weightsByOutcome, u32 const * outcomes, u8 const * weights, int count)
{
    uintptr_t outcomeAddress = reinterpret_cast<uintptr_t>(outcomes);
    uintptr_t weightAddress = reinterpret_cast<uintptr_t>(weights);
    int remaining = count - 1;
    do
    {
        unsigned outcome = *reinterpret_cast<u32 const *>(outcomeAddress);
        outcomeAddress += sizeof(u32);
        *reinterpret_cast<u8 *>(reinterpret_cast<uintptr_t>(weightsByOutcome) + outcome) =
            *reinterpret_cast<u8 const *>(weightAddress++);
        --remaining;
    }
    while (remaining >= 0);
}

// Preserve the register order for the original row lookup and copy loops.
#define COPY_MINE_WEIGHTS(rowIndex, weightTable, outcomeIds, count, idReg) \
{ \
    register unsigned row asm("r2") = (rowIndex); \
    asm("" : "+l"(row)); \
    register uintptr_t table asm("r1") = reinterpret_cast<uintptr_t>(weightTable); \
    asm("" : "+l"(table)); \
    register unsigned offset asm("r0") = row * sizeof(weightTable[0]); \
    asm("" : "+l"(offset) : "l"(table)); \
    register u8 const * rowWeights asm("r2"); \
    asm(".syntax unified\n\tadds %0, %1, %2\n\t.syntax divided" \
        : "=l"(rowWeights) : "l"(offset), "l"(table) : "cc"); \
    register u32 const * outcomes asm(idReg) = (outcomeIds); \
    asm("" : "+l"(outcomes) : "l"(rowWeights)); \
    CopyMineOutcomeWeights(weightsByOutcome, outcomes, rowWeights, count); \
}

// No known callers. The original Spring Mine loops read past each row and ID table.
ROM_TEXT(0809E1B4) void ExportLegacyMineOutcomeWeights(MineFloor const & mine, u8 * weightsByOutcome,
    MineWeightContext const & context)
{
    enum { FIRST_SPRING_MINE_MAP = 52, FIRST_LAKE_MINE_MAP = 308 };
    Farmer const & farmer = context.farmer;
    ActorLocation location = func_0800E924(farmer);
    i32 packedMap = static_cast<i32>(location.GetMap() << 22);
    if (packedMap > ((FIRST_LAKE_MINE_MAP - 1) << 22))
    {
        register unsigned map asm("r0") = func_0800E924(farmer).GetMap();
        asm("" : "+l"(map));
        register unsigned floor asm("r4") = map - FIRST_LAKE_MINE_MAP;
        asm("" : "+l"(floor));
        COPY_MINE_WEIGHTS(GetLakeMineDigWeightRow(mine, floor),
            gLakeMineDigOutcomeWeights, gLakeMineDigOutcomeIds, 5, "r5");
        COPY_MINE_WEIGHTS(GetLakeMineRockWeightRow(mine, floor),
            gLakeMineRockOutcomeWeights, gLakeMineRockOutcomeIds, 10, "r4");
    }
    else
    {
        register unsigned map asm("r0") = func_0800E924(farmer).GetMap();
        asm("" : "+l"(map));
        register unsigned floor asm("r4") = map - FIRST_SPRING_MINE_MAP;
        asm("" : "+l"(floor));
        COPY_MINE_WEIGHTS(GetSpringMineDigWeightRow(mine, floor),
            gSpringMineDigOutcomeWeights, gSpringMineDigOutcomeIds, 10, "r5");
        COPY_MINE_WEIGHTS(GetSpringMineRockWeightRow(mine, floor),
            gSpringMineRockOutcomeWeights, gSpringMineRockOutcomeIds, 14, "r4");
    }
}

#undef COPY_MINE_WEIGHTS
