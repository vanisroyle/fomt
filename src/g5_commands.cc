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
    bool IsActive() const { return state; }
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
    bool operator!=(CommandIterator const & other) const { return node != other.node; }
    CommandIterator & operator++() { node = node->next; return *this; }
};

EC void * malloc(u32);
EC void * func_080D3BC0(u32);
EC void free(void *);
EC void func_080E586C(CommandList *);
EC void func_080073E0(void *, u32);
EC void func_08007184(void *, u32);
EC void func_08007C28(void *, u32);
EC void func_080079E8(void *, u32);
EC u8 vtable_unk_080E85CC[];
EC u8 vtable_unk_080E5A28[];

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

static inline CommandIterator EraseCommand(CommandIterator position)
{
    CommandNode * node = position.node;
    CommandNode * next = node->next;
    CommandNode * prev = node->prev;
    prev->next = next;
    next->prev = prev;
    free(node);
    return CommandIterator(next);
}

EC SECTION(".text.g5_080CADF0") void func_080CADF0(u8 * self, u32 flags)
{
    *(void **) (self + 0x210) = vtable_unk_080E85CC;
    CommandNode * sentinel = *(CommandNode **) (self + 8);
    CommandIterator it(sentinel->next), end(sentinel);
    for (; it != end; ++it)
    {
        DrawCommand * command = it.node->value;
        if (command != nullptr)
            command->vtable->destroy(command, 3);
    }
    CommandList * list = (CommandList *) (self + 4);
    func_080E586C(list);
    if (list->sentinel != nullptr)
        free(list->sentinel);
    if (flags & 1)
        operator delete(self);
}

EC SECTION(".text.g5_080CE3DC") void func_080CE3DC(u8 * self, u32 flags)
{
    CommandNode * sentinel = ((CommandList *) (self + 0xc0))->sentinel;
    CommandIterator it(sentinel->next), end(sentinel);
    for (; it != end; ++it)
    {
        DrawCommand * command = it.node->value;
        if (command != nullptr)
            command->vtable->destroy(command, 3);
    }
    CommandList * list = (CommandList *) (self + 0xc0);
    func_080E586C(list);
    if (list->sentinel != nullptr)
        free(list->sentinel);
    *(void **) (self + 0x1c) = vtable_unk_080E5A28;
    u32 * palette = (u32 *) (self + 0x14);
    func_080073E0(palette, palette[1]);
    func_08007184(palette, 2);
    u32 * tiles = (u32 *) (self + 0xc);
    func_08007C28(tiles, tiles[1]);
    func_080079E8(tiles, 2);
    tiles = (u32 *) (self + 4);
    func_08007C28(tiles, tiles[1]);
    func_080079E8(tiles, 2);
    if (flags & 1)
        operator delete(self);
}

EC SECTION(".text.g5_080CAF6C") void func_080CAF6C(u8 * self)
{
    CommandList * list = (CommandList *) (self + 4);
    CommandIterator it(list->sentinel->next), end(list->sentinel);
    while (it != end)
    {
        DrawCommand * command = it.node->value;
        if (!command->IsActive())
        {
            command->vtable->destroy(command, 3);
            it = EraseCommand(it);
        }
        else
            ++it;
    }
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
