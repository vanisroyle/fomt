#include "prelude.h"

struct HeapBlock
{
    u32 size;
    u32 used;
    HeapBlock * prev;
    HeapBlock * next;
};

EC HeapBlock * gUnk_03000418;

EC void func_080D01E0(HeapBlock * heap, u32 size)
{
    gUnk_03000418 = heap;
    heap->size = size - sizeof(HeapBlock);
    heap->used = 0;
    heap->prev = nullptr;
    heap->next = nullptr;
}

EC void * malloc(u32 requested)
{
    u32 size = (requested + 3) / 4 * 4;
    HeapBlock * block = gUnk_03000418;
    for (; block != nullptr; block = block->next)
    {
        if (block->used == 0 && block->size >= size)
        {
            if (block->size >= size + 32)
            {
                HeapBlock * next = (HeapBlock *) ((u8 *) block + size + sizeof(HeapBlock));
                next->size = block->size - 16;
                next->size -= size;
                next->used = 0;
                next->prev = block;
                next->next = block->next;
                if (block->next != nullptr)
                    block->next->prev = next;
                block->size = size;
                block->next = next;
            }
            block->used = 1;
            break;
        }
    }
    if (block != nullptr)
        return block + 1;
    else
        return nullptr;
}

EC void free(void * ptr)
{
    if (ptr != nullptr)
    {
        HeapBlock * block = (HeapBlock *) ptr - 1;
        HeapBlock * prev = block->prev;
        if (prev != nullptr && prev->used == 0)
        {
            prev->size += sizeof(HeapBlock) + block->size;
            prev->next = block->next;
            if (block->next != nullptr)
                block->next->prev = prev;
            block = prev;
        }
        HeapBlock * next = block->next;
        if (next != nullptr && next->used == 0)
        {
            block->size += sizeof(HeapBlock) + next->size;
            block->next = next->next;
            if (next->next != nullptr)
                next->next->prev = block;
        }
        block->used = 0;
    }
}

EC u32 func_080D02B8(void * ptr)
{
    return ptr != nullptr ? ((HeapBlock *) ptr - 1)->size : 0;
}
