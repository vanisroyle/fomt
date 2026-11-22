#ifndef G5_GRAPHICS_HH
#define G5_GRAPHICS_HH

#include "prelude.h"

struct FieldPlot;
struct Unk_Something;

// Panels retain draw commands in a circular, sentinel-based doubly linked
// list. Inactive commands are destroyed and unlinked by the panel update.
// The renderer itself and the tile/number command constructors are outside G5.
struct DrawCommand;
struct CommandVtable
{
    u32 unknown_00[3];
    void (*destroy)(DrawCommand *, int);
    void (*add)(DrawCommand *, DrawCommand *);
};
struct DrawCommand
{
    u32 state;
    u32 unknown_04;
    CommandVtable * vtable;
    bool IsActive() const { return state; }
};
struct CommandNode
{
    CommandNode * next;
    CommandNode * previous;
    DrawCommand * command;
};
struct CommandList
{
    u32 allocator;
    CommandNode * sentinel;
};
struct CommandIterator
{
    CommandNode * node;
    CommandIterator(CommandNode * node) : node(node) {}
    CommandIterator(CommandIterator const & other) : node(other.node) {}
    bool operator!=(CommandIterator const & other) const { return node != other.node; }
    CommandIterator & operator++() { node = node->next; return *this; }
};

// Simple panels keep their draw owner at +0, this list at +4 and their base
// vtable at +0x210. The sentinel has links but no initialized command payload.
EC u8 * InventoryMenu_InitializePanelBase(u8 * panel, DrawCommand * draw_owner); // 0x080CADB4

// Slot captions occupy +0x28c and truncate to eight bytes, plus a terminator.
// The last selectable slot at +0x298 is retained separately from entry count.
EC u8 * InventoryMenu_InitializeSlotPanel(u8 * panel, void * owner, DrawCommand * draw_owner, u32 slots, char const * caption, u32 special_slot_enabled); // 0x080CAFC4
// Resource-backed panels initialize eight tile/palette handles and reserve
// two tile units per slot. The low byte of the flag enables the special slot.
EC u8 * InventoryMenu_InitializeResourceSlotPanel(u8 * panel, void * owner, DrawCommand * draw_owner, u32 slots, char const * caption, u32 special_slot_enabled); // 0x080CC15C

// Clear the left/right tilemap regions, queue slot count tiles, and upload an
// optional eight-character caption. Right-panel count positions shift 15 tiles.
EC void InventoryMenu_RedrawLeftSlotPanel(u8 * panel); // 0x080CB090
EC void InventoryMenu_RedrawRightSlotPanel(u8 * panel); // 0x080CC310

// Destructors also release panel resources; flags bit 0 deletes the panel.
EC void DestroyPanelDrawCommands(u8 * panel, u32 flags); // 0x080CADF0
EC void PruneInactivePanelDrawCommands(u8 * panel); // 0x080CAF6C
EC void QueuePanelTileDraw(u8 * panel, void * destination, u16 tile, u32 width, u32 height, u16 palette, u32 stride); // 0x080CAE60
EC void QueuePanelNumberDraw(u8 * panel, u32 value, void * destination, u16 style, u16 palette, u32 stride); // 0x080CAEEC
EC u8 * InventoryMenu_InitializePagedDrawPanel(u8 * panel, void * owner); // 0x080CE2FC
EC void DestroyPagedPanelDrawCommands(u8 * panel, u32 flags); // 0x080CE3DC
EC void QueuePagedPanelTileDraw(u8 * panel, void * destination, u16 tile, u32 width, u32 height, u16 palette, u32 stride); // 0x080CE480
EC void QueuePagedPanelNumberDraw(u8 * panel, u32 value, void * destination, u16 style, u16 palette, u32 stride); // 0x080CE510
EC void MapGraphics_LoadPaletteBuffer(u8 * renderer, u32 unused, u32 buffer_index); // 0x080CFB5C
EC void MapGraphics_DrawFieldPlot(u8 * renderer, int x, int y, FieldPlot * plot, Unk_Something const * variant); // 0x080CFEA8

#endif
