#include "prelude.h"
#include <new>

struct DrawCommand;
struct CommandVtable
{
    u32 pad[3];
    void (*destroy)(DrawCommand *, int);
    void (*add)(DrawCommand *, DrawCommand *);
};
struct DrawCommand
{
    u32 state;
    u32 pad;
    CommandVtable * vtable;
};
struct CommandNode
{
    CommandNode * next;
    CommandNode * prev;
    DrawCommand * value;
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
};

EC void * malloc(u32);
EC void * func_080D3BC0(u32);

static inline CommandNode * AllocNode()
{
    CommandNode * node = (CommandNode *) malloc(sizeof(CommandNode));
    if (node == nullptr)
        node = (CommandNode *) func_080D3BC0(sizeof(CommandNode));
    return node;
}

static inline CommandIterator InsertCommand(CommandIterator position, DrawCommand * command)
{
    CommandNode * node = AllocNode();
    new (&node->value) DrawCommand *(command);
    CommandNode * end = position.node;
    CommandNode * prev = end->prev;
    node->next = end;
    node->prev = prev;
    prev->next = node;
    end->prev = node;
    return CommandIterator(node);
}

struct TileCommand { u8 data[0x20]; };
struct NumberCommand { u8 data[0x1c]; };
EC DrawCommand * func_0804EA58(void *, void *, u16, u32, u32, u16, u32);
EC DrawCommand * func_0804EE64(void *, u32, void *, u16, u16, u32);
EC DrawCommand * func_08008940(void *);

EC SECTION(".text.g5_080CAE60") void func_080CAE60(u8 * self, void * destination, u16 tile, u32 width, u32 height, u16 palette, u32 stride)
{
    DrawCommand * command = func_0804EA58(new TileCommand, destination, tile, width, height, palette, stride);
    InsertCommand(CommandIterator(*(CommandNode **) (self + 8)), command);
    DrawCommand * owner = *(DrawCommand **) self;
    owner->vtable->add(owner, command);
}

EC SECTION(".text.g5_080CAE60") void func_080CAEEC(u8 * self, u32 value, void * destination, u16 style, u16 palette, u32 stride)
{
    DrawCommand * command = func_0804EE64(new NumberCommand, value, destination, style, palette, stride);
    InsertCommand(CommandIterator(*(CommandNode **) (self + 8)), command);
    DrawCommand * owner = *(DrawCommand **) self;
    owner->vtable->add(owner, command);
}

EC SECTION(".text.g5_080CE480") void func_080CE480(u8 * self, void * destination, u16 tile, u32 width, u32 height, u16 palette, u32 stride)
{
    DrawCommand * command = func_0804EA58(new TileCommand, destination, tile, width, height, palette, stride);
    InsertCommand(CommandIterator(((CommandList *) (self + 0xc0))->sentinel), command);
    DrawCommand * owner = func_08008940(*(void **) self);
    owner->vtable->add(owner, command);
}

EC SECTION(".text.g5_080CE480") void func_080CE510(u8 * self, u32 value, void * destination, u16 style, u16 palette, u32 stride)
{
    DrawCommand * command = func_0804EE64(new NumberCommand, value, destination, style, palette, stride);
    InsertCommand(CommandIterator(((CommandList *) (self + 0xc0))->sentinel), command);
    DrawCommand * owner = func_08008940(*(void **) self);
    owner->vtable->add(owner, command);
}
