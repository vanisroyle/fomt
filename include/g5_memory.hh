#ifndef G5_MEMORY_HH
#define G5_MEMORY_HH

#include "prelude.h"

// First-fit heap: 16-byte headers precede 4-byte-aligned payloads.
// The doubly linked blocks follow physical order, allowing free() to coalesce.
struct HeapBlock
{
    u32 payload_size;
    u32 allocated;
    HeapBlock * previous;
    HeapBlock * next;
};

EC HeapBlock * gHeapHead; // 0x03000418
EC void InitializeHeap(HeapBlock * heap, u32 size); // 0x080D01E0
EC void * malloc(u32 requested); // 0x080D01F8
EC void free(void * ptr); // 0x080D0260
EC u32 GetHeapAllocationSize(void * ptr); // 0x080D02B8

#endif
