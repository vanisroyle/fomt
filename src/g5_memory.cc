#include "g5_memory.hh"

EC void InitializeHeap(HeapBlock * heap, u32 size) // 0x080D01E0
{
    gHeapHead = heap;
    heap->payload_size = size - sizeof(HeapBlock);
    heap->allocated = 0;
    heap->previous = nullptr;
    heap->next = nullptr;
}

EC void * malloc(u32 requested) // 0x080D01F8
{
    u32 payload_size = (requested + 3) / 4 * 4;
    HeapBlock * block = gHeapHead;
    for (; block != nullptr; block = block->next)
    {
        if (block->allocated == 0 && block->payload_size >= payload_size)
        {
            if (block->payload_size >= payload_size + 32)
            {
                HeapBlock * next = (HeapBlock *) ((u8 *) block + payload_size + sizeof(HeapBlock));
                next->payload_size = block->payload_size - 16;
                next->payload_size -= payload_size;
                next->allocated = 0;
                next->previous = block;
                next->next = block->next;
                if (block->next != nullptr)
                    block->next->previous = next;
                block->payload_size = payload_size;
                block->next = next;
            }
            block->allocated = 1;
            break;
        }
    }
    if (block != nullptr)
        return block + 1;
    else
        return nullptr;
}

EC void free(void * ptr) // 0x080D0260
{
    if (ptr != nullptr)
    {
        HeapBlock * block = (HeapBlock *) ptr - 1;
        HeapBlock * previous = block->previous;
        if (previous != nullptr && previous->allocated == 0)
        {
            previous->payload_size += sizeof(HeapBlock) + block->payload_size;
            previous->next = block->next;
            if (block->next != nullptr)
                block->next->previous = previous;
            block = previous;
        }
        HeapBlock * next = block->next;
        if (next != nullptr && next->allocated == 0)
        {
            block->payload_size += sizeof(HeapBlock) + next->payload_size;
            block->next = next->next;
            if (next->next != nullptr)
                next->next->previous = block;
        }
        block->allocated = 0;
    }
}

EC u32 GetHeapAllocationSize(void * ptr) // 0x080D02B8
{
    return ptr != nullptr ? ((HeapBlock *) ptr - 1)->payload_size : 0;
}
