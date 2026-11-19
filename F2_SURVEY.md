# F2 survey and batch 1

Range: 0804E0F8–08059C94 (end exclusive). 120 function entry points, 48,028 bytes including pools/padding. Batch 1 matches 21 functions (356 bytes); 99 remain.

## Subsystems and evidence

- 0804E0F8 onward: script-facing inventory queries, inspecting held tools/items, rucksack, fridge, shelf and tool chest. Direct calls include ToolStack::IsEmpty, Rucksack::GetEmptyItemSlotCount, FarmHouse::GetFridge/GetShelf. Context +0x350 holds the game-state pointer.
- 0804E7A0 onward: text tile/string drawing helpers (08050B50 calls 0804E7A0/0804E8F0 to draw a speaker name in three four-character strips).
- 0804F288–08050EBC: allocated dialogue/text-window state and public owning handle. Constructor 08050CC0 allocates 0x554 bytes; destructor 08050D0C destroys the pointee. Update 0804F7A4 dispatches phases 0–6 and reads input. Public wrappers are the first matched batch. Callers of the handle constructor include 080521xx/08057Fxx and multiple later UI systems. Portrait helper 08050AD8 loads indexed resources through 080ADCA4 and flags at +0x1DC; name helper 08050B50 accepts strings of 1–12 bytes. Naming of choices/layout/indicator remains provisional pending full callee reconstruction.
- Later F2 includes event/minigame UI state machines. 08052984 calls SetFestivalWinner and updates a ten-entry score ordering through 080522D8/080522E8: animal-festival competition/results logic. Exact identities of every later dispatcher are not yet established.

## Matching and follow-up

The C++ descriptive names retain original external assembly labels through asm declarations, avoiding edits to shared vtable/data files and callers owned by other pieces. Names were checked against origin/main (no F2Dialogue collisions). SECTION and named assembly sections interleave in fomt.lds in ROM order.

08050D0C was attempted but left in assembly due to register allocation mismatch (r4/r5 swapped). Larger state machines, notably 08053960 (5076 bytes) and 080558C4 (8432 bytes), were surveyed by size but not attempted. Unknown forwarded parameters remain a–g rather than inventing semantics. No gameplay/visual check was performed; complete ROM SHA-1 comparison is the matching check.

## Complete entry-point inventory

Sizes include each function's literal pools, alignment, and any unlabelled bytes before the next entry point; they are not necessarily pure instruction sizes.

| ROM address | Bytes | Batch 1 C++ name / status |
|---|---:|---|
| 0804E0F8 | 132 | assembly |
| 0804E17C | 88 | assembly |
| 0804E1D4 | 88 | assembly |
| 0804E22C | 148 | assembly |
| 0804E2C0 | 148 | assembly |
| 0804E354 | 132 | assembly |
| 0804E3D8 | 212 | assembly |
| 0804E4AC | 256 | assembly |
| 0804E5AC | 500 | assembly |
| 0804E7A0 | 60 | assembly |
| 0804E7DC | 276 | assembly |
| 0804E8F0 | 104 | assembly |
| 0804E958 | 112 | assembly |
| 0804E9C8 | 4 | assembly |
| 0804E9CC | 40 | assembly |
| 0804E9F4 | 100 | assembly |
| 0804EA58 | 40 | assembly |
| 0804EA80 | 20 | assembly |
| 0804EA94 | 496 | assembly |
| 0804EC84 | 164 | assembly |
| 0804ED28 | 120 | assembly |
| 0804EDA0 | 20 | assembly |
| 0804EDB4 | 104 | assembly |
| 0804EE1C | 20 | assembly |
| 0804EE30 | 52 | assembly |
| 0804EE64 | 36 | assembly |
| 0804EE88 | 20 | assembly |
| 0804EE9C | 32 | assembly |
| 0804EEBC | 32 | assembly |
| 0804EEDC | 32 | assembly |
| 0804EEFC | 36 | assembly |
| 0804EF20 | 140 | assembly |
| 0804EFAC | 180 | assembly |
| 0804F060 | 128 | assembly |
| 0804F0E0 | 124 | assembly |
| 0804F15C | 4 | assembly |
| 0804F160 | 20 | assembly |
| 0804F174 | 4 | assembly |
| 0804F178 | 36 | assembly |
| 0804F19C | 236 | assembly |
| 0804F288 | 1044 | assembly |
| 0804F69C | 264 | assembly |
| 0804F7A4 | 3064 | assembly |
| 0805039C | 72 | assembly |
| 080503E4 | 24 | assembly |
| 080503FC | 20 | assembly |
| 08050410 | 20 | assembly |
| 08050424 | 84 | assembly |
| 08050478 | 492 | assembly |
| 08050664 | 364 | assembly |
| 080507D0 | 40 | assembly |
| 080507F8 | 112 | assembly |
| 08050868 | 624 | assembly |
| 08050AD8 | 100 | assembly |
| 08050B3C | 20 | assembly |
| 08050B50 | 200 | assembly |
| 08050C18 | 20 | assembly |
| 08050C2C | 56 | assembly |
| 08050C64 | 92 | assembly |
| 08050CC0 | 76 | assembly |
| 08050D0C | 40 | assembly |
| 08050D34 | 8 | GetF2DialoguePhase |
| 08050D3C | 32 | UpdateF2Dialogue |
| 08050D5C | 12 | OpenF2Dialogue |
| 08050D68 | 12 | FinishOpeningF2Dialogue |
| 08050D74 | 12 | CloseF2Dialogue |
| 08050D80 | 12 | HideF2Dialogue |
| 08050D8C | 20 | SetF2DialogueText |
| 08050DA0 | 20 | SetF2DialogueTextFlags |
| 08050DB4 | 20 | SetF2DialogueTextSpeed |
| 08050DC8 | 16 | SetF2DialogueTextOptions |
| 08050DD8 | 12 | DisableF2DialogueInstantText |
| 08050DE4 | 12 | EnableF2DialogueInstantText |
| 08050DF0 | 28 | GetF2DialogueSelection |
| 08050E0C | 36 | ConfigureF2DialogueChoices |
| 08050E30 | 32 | ConfigureF2DialogueLayout |
| 08050E50 | 12 | SetF2DialoguePortrait |
| 08050E5C | 12 | ClearF2DialoguePortrait |
| 08050E68 | 12 | SetF2DialogueSpeakerName |
| 08050E74 | 12 | ClearF2DialogueSpeakerName |
| 08050E80 | 12 | SetF2DialogueIndicator |
| 08050E8C | 12 | ClearF2DialogueIndicator |
| 08050E98 | 36 | assembly |
| 08050EBC | 1124 | assembly |
| 08051320 | 284 | assembly |
| 0805143C | 200 | assembly |
| 08051504 | 492 | assembly |
| 080516F0 | 1600 | assembly |
| 08051D30 | 64 | assembly |
| 08051D70 | 1052 | assembly |
| 0805218C | 48 | assembly |
| 080521BC | 64 | assembly |
| 080521FC | 52 | assembly |
| 08052230 | 168 | assembly |
| 080522D8 | 16 | assembly |
| 080522E8 | 16 | assembly |
| 080522F8 | 1676 | assembly |
| 08052984 | 3248 | assembly |
| 08053634 | 812 | assembly |
| 08053960 | 5076 | assembly |
| 08054D34 | 524 | assembly |
| 08054F40 | 364 | assembly |
| 080550AC | 440 | assembly |
| 08055264 | 1632 | assembly |
| 080558C4 | 8432 | assembly |
| 080579B4 | 464 | assembly |
| 08057B84 | 300 | assembly |
| 08057CB0 | 132 | assembly |
| 08057D34 | 164 | assembly |
| 08057DD8 | 68 | assembly |
| 08057E1C | 64 | assembly |
| 08057E5C | 52 | assembly |
| 08057E90 | 148 | assembly |
| 08057F24 | 40 | assembly |
| 08057F4C | 24 | assembly |
| 08057F64 | 264 | assembly |
| 0805806C | 2112 | assembly |
| 080588AC | 3796 | assembly |
| 08059780 | 248 | assembly |
| 08059878 | 1052 | assembly |
