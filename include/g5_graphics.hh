#ifndef G5_GRAPHICS_HH
#define G5_GRAPHICS_HH

#include "prelude.h"

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

// Destructors also release panel resources; flags bit 0 deletes the panel.
EC void DestroyPanelDrawCommands(u8 * panel, u32 flags); // 0x080CADF0
EC void PruneInactivePanelDrawCommands(u8 * panel); // 0x080CAF6C
EC void QueuePanelTileDraw(u8 * panel, void * destination, u16 tile, u32 width, u32 height, u16 palette, u32 stride); // 0x080CAE60
EC void QueuePanelNumberDraw(u8 * panel, u32 value, void * destination, u16 style, u16 palette, u32 stride); // 0x080CAEEC
EC void DestroyPagedPanelDrawCommands(u8 * panel, u32 flags); // 0x080CE3DC
EC void QueuePagedPanelTileDraw(u8 * panel, void * destination, u16 tile, u32 width, u32 height, u16 palette, u32 stride); // 0x080CE480
EC void QueuePagedPanelNumberDraw(u8 * panel, u32 value, void * destination, u16 style, u16 palette, u32 stride); // 0x080CE510

#endif
