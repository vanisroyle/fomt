#ifndef SAVED_SCRIPT_EVENTS_HH
#define SAVED_SCRIPT_EVENTS_HH

#include "prelude.h"

enum { NO_SELECTED_SCRIPT_EVENT = 0xFFFF };

struct PACKED PackedScriptEventProgress
{
    u8 unknown0E[0x25 - 0x0E];
    u8 : 3;
    u8 farmerSpritePriorityStage : 2;
    u8 : 3;
    u8 unknown26[0x3A - 0x26];
    u8 : 5;
    u8 npc33StoryStage : 2;
    u8 : 1;
    u8 : 3;
    u8 npc34StoryStage : 2;
    u8 : 3;
    u8 unknown3C[0x80 - 0x3C];
};

struct SavedScriptEvents
{
    u32 pendingCount;
    u16 pendingIds[3];
    STRUCT_PAD(0x0A, 0x0C);
    u16 selectedId;
    PackedScriptEventProgress packedProgress;
};

typedef char SavedScriptEventsSizeCheck[sizeof(SavedScriptEvents) == 0x80 ? 1 : -1];
typedef char SavedScriptEventsSelectionOffsetCheck[offsetof(SavedScriptEvents, selectedId) == 0x0C ? 1 : -1];
typedef char SavedScriptEventsProgressOffsetCheck[offsetof(SavedScriptEvents, packedProgress) == 0x0E ? 1 : -1];
typedef char SavedScriptEventsTailOffsetCheck[offsetof(SavedScriptEvents, packedProgress.unknown3C) == 0x3C ? 1 : -1];

EC SavedScriptEvents & InitializeSavedScriptEvents(SavedScriptEvents & events);
EC void ClearPendingScriptEvents(SavedScriptEvents & events);
EC bool HasPendingScriptEvents(SavedScriptEvents const & events);
EC void AddPendingScriptEvent(SavedScriptEvents & events, u16 eventId);
EC void RemovePendingScriptEvent(SavedScriptEvents & events, u16 eventId);
EC bool IsSelectedScriptEvent(SavedScriptEvents const & events, u16 eventId);
EC void SelectScriptEvent(SavedScriptEvents & events, u16 eventId);
EC void DeselectScriptEvent(SavedScriptEvents & events);

#endif // SAVED_SCRIPT_EVENTS_HH
