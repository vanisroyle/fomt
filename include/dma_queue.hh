#ifndef DMA_QUEUE_HH
#define DMA_QUEUE_HH

#include "prelude.h"

struct DmaCommand
{
    u32 is_fill;
    u32 source_or_fill_value;
    void * destination;
    u32 control_flags;
};

struct DmaTransferQueue
{
    DmaCommand * Begin() const { return begin; }
    DmaCommand * End() const { return end; }
    DmaCommand * begin;
    DmaCommand * end;
    u32 allocation_state;
    DmaCommand * capacity_end;
};

EC DmaCommand * DmaCommand_InitializeCopy(DmaCommand *, void const * source, void * destination, u32 byte_count);

#endif // DMA_QUEUE_HH
