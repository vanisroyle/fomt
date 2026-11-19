#include "g5_graphics.hh"
#include "g5_memory.hh"
#include <new>

EC void * func_080D3BC0(u32);
EC void func_080E586C(CommandList *);
EC void func_080073E0(void *, u32);
EC void func_08007184(void *, u32);
EC void func_08007C28(void *, u32);
EC void func_080079E8(void *, u32);
EC u8 vtable_unk_080E85CC[];
EC u8 vtable_unk_080E5A28[];

static inline CommandNode * AllocateCommandNode()
{
    CommandNode * node = (CommandNode *) malloc(sizeof(CommandNode));
    if (node == nullptr)
        node = (CommandNode *) func_080D3BC0(sizeof(CommandNode));
    return node;
}

static inline CommandIterator InsertCommand(CommandIterator position, DrawCommand * command)
{
    CommandNode * node = AllocateCommandNode();
    new (&node->command) DrawCommand *(command);
    CommandNode * end = position.node;
    CommandNode * previous = end->previous;
    node->next = end;
    node->previous = previous;
    previous->next = node;
    end->previous = node;
    return CommandIterator(node);
}

static inline CommandIterator EraseCommand(CommandIterator position)
{
    CommandNode * node = position.node;
    CommandNode * next = node->next;
    CommandNode * previous = node->previous;
    previous->next = next;
    next->previous = previous;
    free(node);
    return CommandIterator(next);
}

EC SECTION(".text.g5_080CADF0") void DestroyPanelDrawCommands(u8 * self, u32 flags) // 0x080CADF0
{
    *(void **) (self + 0x210) = vtable_unk_080E85CC;
    CommandNode * sentinel = *(CommandNode **) (self + 8);
    CommandIterator it(sentinel->next), end(sentinel);
    for (; it != end; ++it)
    {
        DrawCommand * command = it.node->command;
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

EC SECTION(".text.g5_080CE3DC") void DestroyPagedPanelDrawCommands(u8 * self, u32 flags) // 0x080CE3DC
{
    CommandNode * sentinel = ((CommandList *) (self + 0xc0))->sentinel;
    CommandIterator it(sentinel->next), end(sentinel);
    for (; it != end; ++it)
    {
        DrawCommand * command = it.node->command;
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

EC SECTION(".text.g5_080CAF6C") void PruneInactivePanelDrawCommands(u8 * self) // 0x080CAF6C
{
    CommandList * list = (CommandList *) (self + 4);
    CommandIterator it(list->sentinel->next), end(list->sentinel);
    while (it != end)
    {
        DrawCommand * command = it.node->command;
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

EC SECTION(".text.g5_080CAE60") void QueuePanelTileDraw(u8 * self, void * destination, u16 tile, u32 width, u32 height, u16 palette, u32 stride) // 0x080CAE60
{
    DrawCommand * command = func_0804EA58(new TileCommand, destination, tile, width, height, palette, stride);
    InsertCommand(CommandIterator(*(CommandNode **) (self + 8)), command);
    DrawCommand * owner = *(DrawCommand **) self;
    owner->vtable->add(owner, command);
}

EC SECTION(".text.g5_080CAE60") void QueuePanelNumberDraw(u8 * self, u32 value, void * destination, u16 style, u16 palette, u32 stride) // 0x080CAEEC
{
    DrawCommand * command = func_0804EE64(new NumberCommand, value, destination, style, palette, stride);
    InsertCommand(CommandIterator(*(CommandNode **) (self + 8)), command);
    DrawCommand * owner = *(DrawCommand **) self;
    owner->vtable->add(owner, command);
}

EC SECTION(".text.g5_080CE480") void QueuePagedPanelTileDraw(u8 * self, void * destination, u16 tile, u32 width, u32 height, u16 palette, u32 stride) // 0x080CE480
{
    DrawCommand * command = func_0804EA58(new TileCommand, destination, tile, width, height, palette, stride);
    InsertCommand(CommandIterator(((CommandList *) (self + 0xc0))->sentinel), command);
    DrawCommand * owner = func_08008940(*(void **) self);
    owner->vtable->add(owner, command);
}

EC SECTION(".text.g5_080CE480") void QueuePagedPanelNumberDraw(u8 * self, u32 value, void * destination, u16 style, u16 palette, u32 stride) // 0x080CE510
{
    DrawCommand * command = func_0804EE64(new NumberCommand, value, destination, style, palette, stride);
    InsertCommand(CommandIterator(((CommandList *) (self + 0xc0))->sentinel), command);
    DrawCommand * owner = func_08008940(*(void **) self);
    owner->vtable->add(owner, command);
}
