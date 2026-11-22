#include "prelude.h"
#include "dma_queue.hh"
#include "key_repeat.hh"
#include "m4a.h"
#include "gbasvc.h"
#include "unknown_inlines.hh"
#include <stdlib.h>

union ResourceHandleEntry
{
    ResourceHandleEntry * next_free;

    struct
    {
        u16 ref_count;
        u16 generation;
    } params;
};

EC ResourceHandleEntry * ResourceHandleEntries_InitializeFreeList(ResourceHandleEntry * self, int count, int initial_value);

struct ResourceHandleRegistry
{
    ResourceHandleRegistry()
    {
        free_head = ResourceHandleEntries_InitializeFreeList(entries, 0x10, 0);
        allocated_mask = 0;
        allocated_count = 0;
        owner_count = 0;
        reserved_start = 0;
        reserved_count = 0;
        allocated_mask = 0;
    }

    bool IsAllocated(unsigned int index) const { return index < 0x10 && (allocated_mask & (1 << (index % 0x20u))) != 0; }

    void ClearAllocated(unsigned int index)
    {
        if (index < 0x10)
            allocated_mask &= ~(1 << (index % 0x20u));
    }

    void SetAllocated(unsigned int index)
    {
        if (index < 0x10)
            allocated_mask |= (1 << (index % 0x20u));
    }

    ResourceHandleEntry & GetEntry(unsigned int index) { return entries[index]; }

    unsigned int IndexOf(ResourceHandleEntry const * entry) const { return entry - entries; }

    bool Matches(ResourceHandleEntry const & entry, u32 handle) const
    {
        return u16(handle >> 4) == entry.params.generation;
    }

    void FreeEntry(ResourceHandleEntry * entry)
    {
        entry->next_free = free_head;
        free_head = entry;
    }

    ResourceHandleEntry * AllocEntry(ResourceHandleEntry * entry)
    {
        ResourceHandleEntry * prev = free_head;

        if (entry != nullptr)
            free_head = entry->next_free;

        return prev;
    }

    /* +00 */ ResourceHandleEntry * free_head;
    /* +04 */ ResourceHandleEntry entries[0x10];
    /* +44 */ u32 allocated_mask;
    /* +48 */ u16 allocated_count;
    /* +4A */ u16 next_generation;
    /* +4C */ u32 owner_count;
    /* +50 */ u8 reserved_start;
    /* +51 */ u8 reserved_count;
};

extern ResourceHandleRegistry * gResourceHandleRegistry; // 0x03000404

struct ResourceHandleOwner
{
    ~ResourceHandleOwner();
};

union HardwareHandle
{
    u32 value;
    struct
    {
        u32 index : 4;
        u32 generation : 16;
    } fields;

    HardwareHandle(u32 value) : value(value) {}
    u32 Index() const { return fields.index; }
};

// 0x08007128
EC ResourceHandleOwner * ResourceHandles_Initialize(ResourceHandleOwner * self)
{
    if (gResourceHandleRegistry == nullptr)
    {
        gResourceHandleRegistry = new ResourceHandleRegistry;
    }

    gResourceHandleRegistry->owner_count++;

    return self;
}

// 0x08007174
EC ResourceHandleOwner * ResourceHandles_Copy(ResourceHandleOwner * self, ResourceHandleOwner const & other)
{
    gResourceHandleRegistry->owner_count++;

    return self;
}

// 0x08007184
ResourceHandleOwner::~ResourceHandleOwner()
{
    gResourceHandleRegistry->owner_count--;

    if (gResourceHandleRegistry->owner_count == 0)
    {
        if (gResourceHandleRegistry != nullptr)
            delete gResourceHandleRegistry;

        gResourceHandleRegistry = nullptr;
    }
}

// 0x080073E0
EC void SECTION(".text.ResourceHandle_Release") ResourceHandle_Release(ResourceHandleOwner * self, u32 handle)
{
    ResourceHandleRegistry * registry = gResourceHandleRegistry;
    bool generation_matches;
    int generation_status;
    if (handle != 0)
    {
        u32 index = HardwareHandle(handle).Index();
        if (registry->IsAllocated(index))
        {
            ResourceHandleEntry * entry = &registry->GetEntry(index);
            generation_status = registry->Matches(*entry, handle);
            generation_matches = generation_status;
            if (generation_matches && --entry->params.ref_count == 0)
            {
                entry->params.generation = 0;
                registry->FreeEntry(entry);
                registry->ClearAllocated(index);
                --registry->allocated_count;
            }
        }
    }
}

// 0x0800745C
EC u32 SECTION(".text.handle_retain") ResourceHandle_Retain(ResourceHandleOwner * self, u32 handle)
{
    ResourceHandleRegistry * registry = gResourceHandleRegistry;
    if (handle != 0)
    {
        unsigned int index = HardwareHandle(handle).Index();
        if (registry->IsAllocated(index))
        {
            ResourceHandleEntry & entry = registry->GetEntry(index);
            if (registry->Matches(entry, handle))
            {
                u16 previous = entry.params.ref_count;
                if (++entry.params.ref_count != 0)
                    return handle;
                entry.params.ref_count = previous;
            }
        }
    }
    return 0;
}

// 0x080074C0
EC u32 SECTION(".text.ResourceHandle_GetIndex") ResourceHandle_GetIndex(ResourceHandleOwner * self, u32 handle)
{
    ResourceHandleRegistry * registry = gResourceHandleRegistry;
    u32 result;
    bool generation_matches;
    if (handle != 0)
    {
        u32 index = HardwareHandle(handle).Index();
        if (registry->IsAllocated(index))
        {
            ResourceHandleEntry * entry = &registry->GetEntry(index);
            generation_matches = registry->Matches(*entry, handle);
            result = index;
            if (generation_matches)
                return result;
        }
    }
    result = ~0u;
    return result;
}

// 0x08007510
EC u32 SECTION(".text.handle_count") ResourceHandle_GetRefCount(ResourceHandleOwner * self, u32 handle)
{
    ResourceHandleRegistry * registry = gResourceHandleRegistry;
    if (handle != 0)
    {
        unsigned int index = HardwareHandle(handle).Index();
        if (registry->IsAllocated(index))
        {
            ResourceHandleEntry & entry = registry->GetEntry(index);
            if (registry->Matches(entry, handle))
                return entry.params.ref_count;
        }
    }
    return 0;
}

struct ResourceHandleValue
{
    ResourceHandleOwner owner;
    u32 handle;
};

struct ResourcePoolStorage
{
    ResourceHandleValue * begin;
    ResourceHandleValue * end;
    ResourceHandleValue * allocated_begin;
    ResourceHandleValue * capacity_end;
    ResourceHandleValue * cursor;
};

struct ResourcePool
{
    ResourcePoolStorage * storage;
};

// 0x0800770C
EC SECTION(".text.ResourcePool_Initialize") ResourcePool * ResourcePool_Initialize(ResourcePool * self)
{
    self->storage = nullptr;
    return self;
}

EC void ResourceHandle_Release(ResourceHandleOwner *, u32);
EC u32 func_0800736C(ResourceHandleOwner *);
EC void ResourceHandles_Destroy(ResourceHandleOwner *, u32);

// 0x08007714
EC void SECTION(".text.ResourcePool_Destroy") ResourcePool_Destroy(ResourcePool * self, u32 destructor_flags)
{
    if (self->storage != nullptr)
    {
        ResourcePoolStorage * storage = self->storage;
        ResourceHandleValue * end = storage->end;
        for (ResourceHandleValue * value = storage->begin; value != end; ++value)
        {
            ResourceHandle_Release(&value->owner, value->handle);
            ResourceHandles_Destroy(&value->owner, 2);
        }
        if (storage->begin != nullptr)
            free(storage->begin);
        delete storage;
    }
    if (destructor_flags & 1)
        delete self;
}

// 0x08007770
EC SECTION(".text.ResourcePool_Acquire") ResourceHandleValue * ResourcePool_Acquire(ResourceHandleValue * self, ResourcePool const * pool)
{
    if (pool->storage != nullptr)
    {
        ResourcePoolStorage * storage = pool->storage;
        ResourceHandleValue * end = storage->end;
        ResourceHandleValue * next = storage->cursor;
        if (next != end)
        {
            ResourceHandleValue * start = next;
            do
            {
                ResourceHandleValue * candidate = next;
                if (++next == end)
                    next = storage->begin;
                if (ResourceHandle_GetRefCount(&candidate->owner, candidate->handle) == 1)
                {
                    storage->cursor = next;
                    next = candidate;
                    break;
                }
            }
            while (next != start);
        }
        ResourceHandles_Initialize(&self->owner);
        self->handle = ResourceHandle_Retain(&self->owner, next->handle);
    }
    else
    {
        ResourceHandles_Initialize(&self->owner);
        self->handle = func_0800736C(&self->owner);
    }
    return self;
}

struct AllocationBitset
{
    bool IsFull() const { return bits + 1u == 0; }
    bool IsEmpty() const { return bits == 0; }
    u32 bits;
};

// 0x080077DC
EC u32 SECTION(".text.AllocationBitset_AllocateBlock") AllocationBitset_AllocateBlock(AllocationBitset * self, u32 order)
{
    u32 occupied_bits;
    if (!self->IsFull())
    {
        if (self->IsEmpty())
        {
            self->bits |= (1u << (1u << order)) - 1;
            return 0;
        }
        if (order <= 4)
        {
            u32 bits = self->bits;
            u32 width = 1u << order;
            u32 index = 0;
            u32 mask = (1u << width) - 1;
            occupied_bits = bits;
            do
            {
                if ((occupied_bits & mask) == 0)
                {
                    self->bits |= mask;
                    return index;
                }
                index += width;
                mask <<= width;
            }
            while (mask != 0);
        }
    }
    return 32;
}

// 0x08007844
EC void SECTION(".text.AllocationBitset_FreeBlock") AllocationBitset_FreeBlock(AllocationBitset * self, u32 index, u32 order)
{
    if (order <= 4)
        self->bits &= ~(((1u << (1u << order)) - 1) << (index % 32));
    else if (order == 5)
        self->bits = 0;
}

struct AllocationTreeBranch
{
    u8 full;
    u8 empty;
    u8 subtree_storage[122];
};

struct AllocationTree
{
    u8 full;
    u8 empty;
    u16 alignment_padding;
    AllocationTreeBranch children[2];
};

EC void AllocationTreeBranch_MarkFull(AllocationTreeBranch *);
EC void AllocationTreeBranch_MarkEmpty(AllocationTreeBranch *);

// 0x08007EA8
EC void SECTION(".text.AllocationTree_MarkFull") AllocationTree_MarkFull(AllocationTree * self)
{
    AllocationTreeBranch_MarkFull(&self->children[0]);
    AllocationTreeBranch_MarkFull(&self->children[1]);
    self->full = 1;
    self->empty = 0;
}

// 0x08007EC8
EC void SECTION(".text.AllocationTree_MarkEmpty") AllocationTree_MarkEmpty(AllocationTree * self)
{
    AllocationTreeBranch_MarkEmpty(&self->children[1]);
    AllocationTreeBranch_MarkEmpty(&self->children[0]);
    self->full = 0;
    self->empty = 1;
}

EC u32 func_080D7568(AllocationTreeBranch *, u32);

inline bool AllocationTreeBranchIndex_IsValid(u32 index) { return index <= 511; }

// 0x08008020
EC u32 SECTION(".text.AllocationTree_AllocateBlock") AllocationTree_AllocateBlock(AllocationTree * self, u32 order)
{
    i32 full;
    do
    {
        if (order <= 9)
        {
            i32 child_index = 0;
            AllocationTreeBranch * child = &self->children[0];
            do
            {
                full = child->full;
                if (full == 0)
                {
                    u32 tile_index = func_080D7568(child, order);
                    if (AllocationTreeBranchIndex_IsValid(tile_index))
                    {
                        self->empty = full;
                        self->full = self->children[0].full && self->children[1].full;
                        return (child_index << 9) | tile_index;
                    }
                }
                ++child;
                ++child_index;
            }
            while (child_index <= 1);
        }
        else if (order == 10 && self->empty != 0)
        {
            AllocationTree_MarkFull(self);
            return 0;
        }
    }
    while (0);
    return 1024;
}

struct TileHeapOwner {};

EC void func_080D76C0(AllocationTreeBranch *, u32, u32);

inline u8 AllocationTreeBranch_GetEmptyFlag(AllocationTreeBranch const * child) { return child->empty; }

// 0x080080A0
EC void SECTION(".text.AllocationTree_FreeBlock") AllocationTree_FreeBlock(AllocationTree * self, u32 tile_index, u32 order)
{
    if (order <= 9)
    {
        u32 child_selector = tile_index & 512;
        AllocationTreeBranch * first_child = &self->children[0];
        AllocationTreeBranch * child = first_child;
        if (child_selector)
            child = &self->children[1];
        func_080D76C0(child, tile_index, order);
        self->full = 0;
        self->empty = first_child->empty && AllocationTreeBranch_GetEmptyFlag(&self->children[1]);
    }
    else if (order == 10)
        AllocationTree_MarkEmpty(self);
}

union TileHeapEntry
{
    TileHeapEntry * next_free;
    struct
    {
        u16 tile_index : 10;
        u16 order : 4;
        u16 reserved : 2;
        u16 ref_count;
        u16 generation;
        u16 alignment_padding;
    } params;
};

struct TileHeapEntryPool
{
    TileHeapEntry & GetEntry(u32 index) { return entries[index]; }
    void FreeEntry(TileHeapEntry * entry) { entry->next_free = free_head; free_head = entry; }
    TileHeapEntry * free_head;
    TileHeapEntry entries[256];
};

struct TileHeapState
{
    bool IsAllocated(u32 index) const
    {
        return index < 256 && (allocated_mask[index / 32] & (1u << (index % 32))) != 0;
    }
    void ClearAllocated(u32 index)
    {
        if (index < 256)
            allocated_mask[index / 32] &= ~(1u << (index % 32));
    }
    u32 allocated_mask[8];
    TileHeapEntryPool entry_pool;
    AllocationTree tree;
    u16 allocated_count;
    u16 next_generation;
    u32 owner_count;
    u16 reserved_start;
    u16 reserved_count;
};

extern TileHeapState * gTileHeapState; // 0x03000408

union TileHandle
{
    u32 value;
    u8 bytes[4];
    struct
    {
        u32 index : 8;
        u32 generation : 16;
    } fields;
    TileHandle(u32 value) : value(value) {}
    u32 Index() const { return bytes[0]; }
};

inline bool TileHeapEntry_Matches(TileHeapEntry const & entry, u32 handle)
{
    return u16(handle >> 8) == entry.params.generation;
}


// 0x08007C28
EC void SECTION(".text.TileHandle_Release") TileHandle_Release(TileHeapOwner * self, u32 handle)
{
    int no_references;
    TileHeapState * heap = gTileHeapState;
    TileHeapState * allocation_heap;
    bool generation_matches;
    u32 entry_index;
    if (handle != 0)
    {
        u32 index = TileHandle(handle).Index();
        entry_index = index;
        allocation_heap = heap;
        if (heap->IsAllocated(entry_index))
        {
            TileHeapEntry * entry = &heap->entry_pool.GetEntry(entry_index);
            generation_matches = TileHeapEntry_Matches(*entry, handle);
            no_references = 0;
            if (generation_matches)
            {
                u16 ref_count = --entry->params.ref_count;
                if (ref_count == no_references)
                {
                    AllocationTree_FreeBlock(&heap->tree, entry->params.tile_index, entry->params.order);
                    entry->params.generation = ref_count;
                    heap->entry_pool.FreeEntry(entry);
                    heap->ClearAllocated(entry_index);
                    --allocation_heap->allocated_count;
                }
            }
        }
    }
}

// 0x08007CD8
EC u32 SECTION(".text.TileHandle_Retain") TileHandle_Retain(TileHeapOwner * self, u32 handle)
{
    u16 retained_ref_count;
    TileHeapState * heap = gTileHeapState;
    bool generation_matches;
    int generation_status;
    u32 entry_index;
    if (handle != 0)
    {
        u32 index = TileHandle(handle).Index();
        entry_index = index;
        if (heap->IsAllocated(entry_index))
        {
            TileHeapEntry * entry = &heap->entry_pool.GetEntry(entry_index);
            generation_status = TileHeapEntry_Matches(*entry, handle);
            generation_matches = generation_status;
            if (generation_matches)
            {
                u16 previous_ref_count = entry->params.ref_count;
                int ref_count = previous_ref_count + 1;
                entry->params.ref_count = ref_count;
                if ((retained_ref_count = ref_count) != 0)
                    return handle;
                entry->params.ref_count = previous_ref_count;
            }
        }
    }
    return 0;
}

// 0x08007D4C
EC u32 SECTION(".text.TileHandle_GetTileIndex") TileHandle_GetTileIndex(TileHeapOwner * self, u32 handle)
{
    TileHeapState * heap = gTileHeapState;
    bool generation_matches;
    u32 entry_index;
    if (handle != 0)
    {
        u32 index = TileHandle(handle).Index();
        entry_index = index;
        if (heap->IsAllocated(entry_index))
        {
            TileHeapEntry * entry = &heap->entry_pool.GetEntry(entry_index);
            generation_matches = TileHeapEntry_Matches(*entry, handle);
            if (generation_matches)
                return entry->params.tile_index;
        }
    }
    return ~0u;
}

// 0x08007DB8
EC u32 SECTION(".text.TileHandle_GetOrder") TileHandle_GetOrder(TileHeapOwner * self, u32 handle)
{
    TileHeapState * heap = gTileHeapState;
    bool generation_matches;
    u32 entry_index;
    if (handle != 0)
    {
        u32 index = TileHandle(handle).Index();
        entry_index = index;
        if (heap->IsAllocated(entry_index))
        {
            TileHeapEntry * entry = &heap->entry_pool.GetEntry(entry_index);
            generation_matches = TileHeapEntry_Matches(*entry, handle);
            if (generation_matches)
                return entry->params.order;
        }
    }
    return 11;
}

// 0x08007E24
EC u32 SECTION(".text.TileHandle_GetRefCount") TileHandle_GetRefCount(TileHeapOwner * self, u32 handle)
{
    TileHeapState * heap = gTileHeapState;
    bool generation_matches;
    u32 entry_index;
    if (handle != 0)
    {
        u32 index = TileHandle(handle).Index();
        entry_index = index;
        if (heap->IsAllocated(entry_index))
        {
            TileHeapEntry * entry = &heap->entry_pool.GetEntry(entry_index);
            generation_matches = TileHeapEntry_Matches(*entry, handle);
            if (generation_matches)
                return entry->params.ref_count;
        }
    }
    return 0;
}

// 0x080079E8
EC void SECTION(".text.TileHeapOwner_Destroy") TileHeapOwner_Destroy(TileHeapOwner * self, u32 destructor_flags)
{
    TileHeapState * heap = gTileHeapState;
    if (--heap->owner_count == 0)
    {
        if (heap != nullptr)
            delete heap;
        gTileHeapState = nullptr;
    }
    if (destructor_flags & 1)
        delete self;
}

struct TileHandleValue
{
    TileHeapOwner owner;
    u32 handle;
};

struct TilePoolBucket
{
    TileHandleValue * begin;
    TileHandleValue * end;
    TileHandleValue * allocated_begin;
    TileHandleValue * capacity_end;
    TileHandleValue * cursor;
};

struct TilePoolStorage
{
    TilePoolBucket buckets[8];
};

struct TilePool
{
    TilePoolStorage * storage;
};

// 0x0800835C
EC void SECTION(".text.TilePool_Initialize") TilePool_Initialize(TilePool * self)
{
    self->storage = nullptr;
}

EC TileHeapOwner * func_08007874(TileHeapOwner *);
EC void TileHandle_Release(TileHeapOwner *, u32);
EC u32 TileHandle_GetRefCount(TileHeapOwner *, u32);
EC u32 TileHandle_Retain(TileHeapOwner *, u32);
EC u32 func_08007B54(TileHeapOwner *, u32);

// 0x08008364
EC void SECTION(".text.TilePool_Destroy") TilePool_Destroy(TilePool * self, u32 destructor_flags)
{
    if (self->storage != nullptr)
    {
        TilePoolStorage * storage = self->storage;
        TilePoolBucket * bucket = storage->buckets + 8;
        while (storage->buckets != bucket)
        {
            --bucket;
            TileHandleValue * end = bucket->end;
            for (TileHandleValue * value = bucket->begin; value != end; ++value)
            {
                TileHandle_Release(&value->owner, value->handle);
                TileHeapOwner_Destroy(&value->owner, 2);
            }
            if (bucket->begin != nullptr)
                free(bucket->begin);
        }
        delete storage;
    }
    if (destructor_flags & 1)
        delete self;
}

// 0x080083D0
EC SECTION(".text.TilePool_Acquire") TileHandleValue * TilePool_Acquire(TileHandleValue * self, TilePool const * pool, u32 order)
{
    TilePoolStorage * storage = pool->storage;
    if (storage != nullptr)
    {
        TilePoolBucket * bucket = reinterpret_cast<TilePoolBucket *>(order * sizeof(TilePoolBucket) + u32(storage));
        TileHandleValue * end = bucket->end;
        TileHandleValue * next = bucket->cursor;
        if (next != end)
        {
            TileHandleValue * start = next;
            do
            {
                TileHandleValue * candidate = next;
                if (++next == end)
                    next = bucket->begin;
                if (TileHandle_GetRefCount(&candidate->owner, candidate->handle) == 1)
                {
                    bucket->cursor = next;
                    next = candidate;
                    break;
                }
            }
            while (next != start);
        }
        func_08007874(&self->owner);
        self->handle = TileHandle_Retain(&self->owner, next->handle);
    }
    else
    {
        func_08007874(&self->owner);
        self->handle = func_08007B54(&self->owner, order);
    }
    return self;
}

struct HardwareContextState
{
    u16 held_keys;
    u16 held_padding;
    u16 pressed_keys;
    u16 pressed_padding;
    u16 repeated_keys;
    STRUCT_PAD(0x0A, 0x24);
    u8 dma_queue[0x10];
    u8 display_registers[0x58];
    u8 oam_buffer[0x404];
    u8 vblank_dispatcher[4];
    u8 frame_tasks[0x1C];
};

struct HardwareContext
{
    HardwareContextState * state;
    void const * vtable;
};

EC u32 KeyRepeat_Poll(void *);

// 0x080088B8
EC void SECTION(".text.HardwareContext_UpdateKeys") HardwareContext_UpdateKeys(HardwareContext * self)
{
    KeyRepeat_Poll(self->state);
}

// 0x080088C4
EC u16 SECTION(".text.HardwareContext_GetHeldKeys") HardwareContext_GetHeldKeys(HardwareContext * self)
{
    return self->state->held_keys;
}

// 0x080088CC
EC u16 SECTION(".text.HardwareContext_GetPressedKeys") HardwareContext_GetPressedKeys(HardwareContext * self)
{
    return self->state->pressed_keys;
}

// 0x080088D4
EC u16 SECTION(".text.HardwareContext_GetRepeatedKeys") HardwareContext_GetRepeatedKeys(HardwareContext * self)
{
    return self->state->repeated_keys;
}

// 0x080088DC
EC SECTION(".text.HardwareContext_GetState") HardwareContextState * HardwareContext_GetState(HardwareContext * self)
{
    return self->state;
}

// 0x08008910
EC SECTION(".text.HardwareContext_GetDmaQueue") void * HardwareContext_GetDmaQueue(HardwareContext * self)
{
    return self->state->dma_queue;
}

// 0x08008918
EC SECTION(".text.HardwareContext_GetDisplayRegisters") void * HardwareContext_GetDisplayRegisters(HardwareContext * self)
{
    return self->state->display_registers;
}

// 0x08008920
EC SECTION(".text.HardwareContext_GetOamBuffer") void * HardwareContext_GetOamBuffer(HardwareContext * self)
{
    return self->state->oam_buffer;
}

// 0x08008940
EC SECTION(".text.HardwareContext_GetFrameTasks") void * HardwareContext_GetFrameTasks(HardwareContext * self)
{
    return self->state->frame_tasks;
}

EC void * VBlankDispatcher_GetTasks(void ** self);

// 0x0800894C
EC SECTION(".text.HardwareContext_GetVBlankTasks") void * HardwareContext_GetVBlankTasks(HardwareContext * self)
{
    return VBlankDispatcher_GetTasks(reinterpret_cast<void **>(self->state->vblank_dispatcher));
}

// 0x08008AE0
EC SECTION(".text.VBlankDispatcher_GetTasks") void * VBlankDispatcher_GetTasks(void ** self)
{
    return static_cast<u8 *>(*self) + 0xC;
}

EC void Irq_Wait(u16 irq_mask);

// 0x08008AF0
EC void SECTION(".text.VBlankDispatcher_Wait") VBlankDispatcher_Wait(void * self)
{
    Irq_Wait(1);
}

// 0x08008AFC
EC void SECTION(".text.Audio_Initialize") Audio_Initialize()
{
    m4aSoundInit();
    m4aSoundVSyncOff();
}

// 0x08008B0C
EC bool SECTION(".text.Audio_OnVBlank") Audio_OnVBlank()
{
    m4aSoundVSync();
    return true;
}

// 0x08008B18
EC bool SECTION(".text.Audio_Update") Audio_Update()
{
    m4aSoundMain();
    return true;
}

struct AudioPlayer
{
    MusicPlayer * player;
};

EC MusicPlayer * Audio_GetPlayerByIndex(u32 index);

extern u8 const _M4aNumMusicPlayers[];

// 0x08008B24
EC SECTION(".text.Audio_GetPlayerByIndex") MusicPlayer * Audio_GetPlayerByIndex(u32 index)
{
    u16 player_count = u32(_M4aNumMusicPlayers);
    if (index >= player_count)
        index %= player_count;
    return gMusicPlayerTable[index].music_player;
}

// 0x08008B54
EC SECTION(".text.AudioPlayer_Initialize") AudioPlayer * AudioPlayer_Initialize(AudioPlayer * self, u8 index)
{
    self->player = Audio_GetPlayerByIndex(index);
    return self;
}

// 0x08008B6C
EC void SECTION(".text.AudioPlayer_StartSong") AudioPlayer_StartSong(AudioPlayer * self, u16 song_id)
{
    m4aMPlayStart(self->player, gSongTable[song_id].song);
}

// 0x08008B88
EC void SECTION(".text.AudioPlayer_StartSongIfStoppedOrChanged") AudioPlayer_StartSongIfStoppedOrChanged(AudioPlayer * self, u16 song_id)
{
    Song * target = gSongTable[song_id].song;
    MusicPlayer * player = self->player;
    if (player->song != target || i32(player->status) < 0)
        m4aMPlayStart(player, target);
}

// 0x08008BB0
EC void SECTION(".text.AudioPlayer_StartOrResumeSong") AudioPlayer_StartOrResumeSong(AudioPlayer * self, u16 song_id)
{
    Song * target = gSongTable[song_id].song;
    MusicPlayer * player = self->player;
    if (player->song != target)
        m4aMPlayStart(player, target);
    else if (i32(player->status) < 0)
        m4aMPlayContinue(player);
}

// 0x08008BE0
EC void SECTION(".text.AudioPlayer_Stop") AudioPlayer_Stop(AudioPlayer * self)
{
    m4aMPlayStop(self->player);
}

// 0x08008BF8
EC void SECTION(".text.AudioPlayer_FadeOut") AudioPlayer_FadeOut(AudioPlayer * self, u16 speed)
{
    m4aMPlayFadeOut(self->player, speed);
}

// 0x08008C28
EC void SECTION(".text.AudioPlayer_SetTempo") AudioPlayer_SetTempo(AudioPlayer * self, u16 tempo)
{
    m4aMPlayTempoControl(self->player, tempo);
}

// 0x08008C38
EC void SECTION(".text.AudioPlayer_SetTrackVolume") AudioPlayer_SetTrackVolume(AudioPlayer * self, u16 volume, u16 track_mask)
{
    m4aMPlayVolumeControl(self->player, track_mask, volume);
}

// 0x08008C54
EC void SECTION(".text.AudioPlayer_SetTrackPitch") AudioPlayer_SetTrackPitch(AudioPlayer * self, i16 pitch, u16 track_mask)
{
    m4aMPlayPitchControl(self->player, track_mask, pitch);
}

// 0x08008C70
EC void SECTION(".text.AudioPlayer_SetTrackPan") AudioPlayer_SetTrackPan(AudioPlayer * self, i8 pan, u16 track_mask)
{
    m4aMPlayPanpotControl(self->player, track_mask, pan);
}

// 0x08008C8C
EC void SECTION(".text.AudioPlayer_SetTrackModDepth") AudioPlayer_SetTrackModDepth(AudioPlayer * self, u8 depth, u16 track_mask)
{
    m4aMPlayModDepthSet(self->player, track_mask, depth);
}

// 0x08008CA8
EC void SECTION(".text.AudioPlayer_SetTrackLfoSpeed") AudioPlayer_SetTrackLfoSpeed(AudioPlayer * self, u8 speed, u16 track_mask)
{
    m4aMPlayLFOSpeedSet(self->player, track_mask, speed);
}

// 0x08008CC4
EC void SECTION(".text.AudioPlayer_ResetChannels") AudioPlayer_ResetChannels(AudioPlayer * self)
{
    m4aMPlayImmInit(self->player);
}

// 0x08008CD0
EC bool SECTION(".text.AudioPlayer_IsPlaying") AudioPlayer_IsPlaying(AudioPlayer * self)
{
    return (self->player->status >> 31) ^ 1;
}

// 0x08008D3C
EC SECTION(".text.Audio_GetSongPlayer") MusicPlayer * Audio_GetSongPlayer(u16 song_id)
{
    AudioPlayer song_player;
    AudioPlayer_Initialize(&song_player, gSongTable[song_id].ms);
    return song_player.player;
}

// 0x08008D84
EC void SECTION(".text.Audio_EnableVBlankDma") Audio_EnableVBlankDma()
{
    m4aSoundVSyncOn();
}

// 0x08008D90
EC void SECTION(".text.Audio_DisableVBlankDma") Audio_DisableVBlankDma()
{
    m4aSoundVSyncOff();
}

// 0x08008D9C
EC bool SECTION(".text.Audio_IsDma1Enabled") Audio_IsDma1Enabled()
{
    return *reinterpret_cast<u32 volatile *>(0x040000C4) >> 31;
}

// 0x08008DA8
EC void SECTION(".text.Audio_StartSong") Audio_StartSong(u16 song_id)
{
    m4aSongNumStart(song_id);
}

// 0x08008DB8
EC void SECTION(".text.Audio_StartOrChangeSong") Audio_StartOrChangeSong(u16 song_id)
{
    m4aSongNumStartOrChange(song_id);
}

// 0x08008DE8
EC void SECTION(".text.Audio_StopAllPlayers") Audio_StopAllPlayers()
{
    m4aMPlayAllStop();
}

inline u32 Dma_BuildCopyControl(u32 source, u32 destination, u32 byte_count)
{
    if (byte_count != 0 && ((source | destination | byte_count) & 1) == 0)
    {
        if (((source | destination | byte_count) & 3) == 0)
            return u16(byte_count / 4) | 0x84000000;
        return u16(byte_count / 2) | 0x80000000;
    }
    return 0;
}

EC void Dma_ExecuteTransfer(void const *, void *, u32, void volatile *);

// 0x08008E64
EC void SECTION(".text.Dma3_Copy") Dma3_Copy(void const * source, void * destination, u32 byte_count)
{
    if (source != nullptr && destination != nullptr)
    {
        void const * transfer_source = source;
        u32 control = Dma_BuildCopyControl(u32(transfer_source), u32(destination), byte_count);
        Dma_ExecuteTransfer(transfer_source, destination, control, reinterpret_cast<void volatile *>(0x040000D4));
    }
}

// 0x08008EB8
EC void SECTION(".text.Dma3_Fill") Dma3_Fill(u32 fill_value, void * destination, u32 byte_count)
{
    if (destination != nullptr)
    {
        u32 control_flags = Dma_BuildCopyControl(0, u32(destination), byte_count) | 0x01000000;
        Dma_ExecuteTransfer(&fill_value, destination, control_flags,
            reinterpret_cast<void volatile *>(0x040000D4));
    }
}

// 0x08008FE4
EC void SECTION(".text.DmaCommand_ExecuteRange") DmaCommand_ExecuteRange(DmaCommand const * begin, DmaCommand const * end)
{
    DmaCommand const * limit = end;
    void volatile * channel = reinterpret_cast<void volatile *>(0x040000D4);
    for (DmaCommand const * command = begin; command != limit; ++command)
    {
        void const * source = reinterpret_cast<void const *>(command->source_or_fill_value);
        u32 fill_value;
        if (command->is_fill == 1)
        {
            fill_value = u32(source);
            source = &fill_value;
        }
        Dma_ExecuteTransfer(source, command->destination, command->control_flags, channel);
    }
}

// 0x08008F0C
EC SECTION(".text.DmaCommand_InitializeCopy") DmaCommand * DmaCommand_InitializeCopy(
    DmaCommand * self, void const * source, void * destination, u32 byte_count)
{
    u32 control_flags = source != nullptr && destination != nullptr ? Dma_BuildCopyControl(u32(source), u32(destination), byte_count) : 0;
    self->is_fill = 0;
    self->source_or_fill_value = u32(source);
    self->destination = destination;
    self->control_flags = control_flags;
    return self;
}

// 0x08008F60
EC SECTION(".text.DmaCommand_InitializeFill") DmaCommand * DmaCommand_InitializeFill(
    DmaCommand * self, u32 fill_value, void * destination, u32 byte_count)
{
    u32 control_flags = destination != nullptr ? Dma_BuildCopyControl(0, u32(destination), byte_count) | 0x01000000 : 0;
    self->is_fill = 1;
    self->source_or_fill_value = fill_value;
    self->destination = destination;
    self->control_flags = control_flags;
    return self;
}

// 0x0800912C
EC u32 SECTION(".text.Keys_ReadHardware") Keys_ReadHardware()
{
    return ~*reinterpret_cast<u16 volatile *>(0x04000130) & 0x3FF;
}

// 0x08009140
EC SECTION(".text.Keys_InitializeFromHardware") KeyRepeatState * Keys_InitializeFromHardware(KeyRepeatState * self)
{
    self->held_keys = Keys_ReadHardware();
    return self;
}

// 0x08009154
EC SECTION(".text.Keys_Initialize") KeyRepeatState * Keys_Initialize(KeyRepeatState * self, u32 keys)
{
    self->held_keys = keys;
    return self;
}

// 0x08009158
EC u32 SECTION(".text.Keys_PollHeld") Keys_PollHeld(KeyRepeatState * self)
{
    u32 keys = Keys_ReadHardware();
    self->held_keys = keys;
    return keys;
}

// 0x08009168
EC SECTION(".text.KeyEdges_InitializeFromHardware") KeyRepeatState * KeyEdges_InitializeFromHardware(KeyRepeatState * self)
{
    Keys_InitializeFromHardware(self);
    self->pressed_keys = self->held_keys;
    return self;
}

// 0x0800917C
EC SECTION(".text.KeyEdges_Initialize") KeyRepeatState * KeyEdges_Initialize(KeyRepeatState * self, u32 keys)
{
    Keys_Initialize(self, keys);
    self->pressed_keys = self->held_keys;
    return self;
}

// 0x08009190
EC u32 SECTION(".text.KeyEdges_PollPressed") KeyEdges_PollPressed(KeyRepeatState * self)
{
    u16 previous = self->held_keys;
    u32 pressed_keys = Keys_PollHeld(self) & ~previous;
    self->pressed_keys = pressed_keys;
    return pressed_keys;
}

// 0x080091A4
EC SECTION(".text.KeyRepeat_Initialize") KeyRepeatState * KeyRepeat_Initialize(KeyRepeatState * self, u16 delay, u16 period)
{
    KeyEdges_InitializeFromHardware(self);
    self->repeated_keys = self->pressed_keys;
    self->repeat_delay = delay;
    self->repeat_period = period;
    fill_inl(self->repeat_counters, self->repeat_counters + 10, 0);
    return self;
}

// 0x080091E8
EC SECTION(".text.KeyRepeat_InitializeWithHeldKeys") KeyRepeatState * KeyRepeat_InitializeWithHeldKeys(KeyRepeatState * self, u32 keys)
{
    KeyEdges_Initialize(self, keys);
    self->repeated_keys = self->pressed_keys;
    self->repeat_delay = 30;
    self->repeat_period = 4;
    fill_inl(self->repeat_counters, self->repeat_counters + 10, 0);
    return self;
}

// 0x08009268
EC u32 SECTION(".text.KeyRepeat_Poll") KeyRepeat_Poll(void * key_state)
{
    KeyRepeatState * self = static_cast<KeyRepeatState *>(key_state);
    u32 pressed_keys = KeyEdges_PollPressed(self);
    u32 held_keys = self->held_keys;
    u32 period = self->repeat_period;
    u32 delay = self->repeat_delay;
    u32 repeated_keys = 0;
    u32 key_mask = 0x200;
    i16 * repeat_counter = self->repeat_counters + 9;
    do
    {
        if (held_keys & key_mask)
        {
            int counter_value;
            if ((pressed_keys & key_mask) == 0)
            {
                counter_value = *repeat_counter;
                if (counter_value != delay)
                    ++counter_value;
                else
                {
                    repeated_keys |= key_mask;
                    counter_value -= period;
                }
            }
            else
            {
                repeated_keys |= key_mask;
                counter_value = 0;
            }
            *repeat_counter = counter_value;
        }
        --repeat_counter;
        key_mask >>= 1;
    }
    while (key_mask != 0);
    self->repeated_keys = repeated_keys;
    return repeated_keys;
}

// 0x080092C8
EC void SECTION(".text.KeyRepeat_SetDelay") KeyRepeat_SetDelay(KeyRepeatState * self, u16 delay)
{
    u32 held_keys = self->held_keys;
    u32 key_mask = 0x200;
    i16 * repeat_counter = self->repeat_counters + 9;
    int delta = delay - self->repeat_delay;
    do
    {
        if (held_keys & key_mask)
            *repeat_counter += delta;
        --repeat_counter;
        key_mask >>= 1;
    }
    while (key_mask != 0);
    self->repeat_delay = delay;
}

// 0x080092FC
EC void SECTION(".text.KeyRepeat_SetPeriod") KeyRepeat_SetPeriod(KeyRepeatState * self, u32 period)
{
    self->repeat_period = period;
}

struct DisplayFadeState
{
    DisplayFadeState(void * register_buffer)
        : register_buffer(register_buffer), step(0), blend_mode(0), mosaic_enabled(false), intensity(0) {}
    void * register_buffer;
    i16 step;
    u32 blend_mode;
    bool mosaic_enabled;
    u16 intensity;
};

struct DisplayFade
{
    DisplayFadeState * state;
};

// 0x08009300
EC SECTION(".text.DisplayFade_Initialize") DisplayFade * DisplayFade_Initialize(DisplayFade * self, void * register_buffer)
{
    self->state = new DisplayFadeState(register_buffer);
    return self;
}

// 0x08009324
EC void SECTION(".text.DisplayFade_Destroy") DisplayFade_Destroy(DisplayFade * self, u32 destructor_flags)
{
    delete self->state;
    if (destructor_flags & 1)
        delete self;
}

// 0x08009344
EC SECTION(".text.DisplayFade_SetRegisterBuffer") void * DisplayFade_SetRegisterBuffer(DisplayFade * self, void * register_buffer)
{
    void * previous = self->state->register_buffer;
    self->state->register_buffer = register_buffer;
    return previous;
}

// 0x0800934C
EC void SECTION(".text.DisplayFade_StartIn") DisplayFade_StartIn(DisplayFade * self, i16 step, u32 blend_mode, u32 mosaic_enabled)
{
    DisplayFadeState * state = self->state;
    int signed_step = -(step < 0 ? -step : step);
    bool use_mosaic = mosaic_enabled != 0;
    state->step = signed_step;
    state->blend_mode = blend_mode;
    state->mosaic_enabled = use_mosaic;
    state->intensity = 0xFFFF;
}

// 0x08009378
EC void SECTION(".text.DisplayFade_StartOut") DisplayFade_StartOut(DisplayFade * self, i16 step, u32 blend_mode, u32 mosaic_enabled)
{
    DisplayFadeState * state = self->state;
    int signed_step = step < 0 ? -step : step;
    bool use_mosaic = mosaic_enabled != 0;
    state->step = signed_step;
    state->blend_mode = blend_mode;
    state->mosaic_enabled = use_mosaic;
    state->intensity = 0;
}

// 0x080093A0
EC i16 SECTION(".text.DisplayFade_SetSpeed") DisplayFade_SetSpeed(DisplayFade * self, i16 step)
{
    i16 previous = self->state->step;
    self->state->step = step;
    return previous;
}

// 0x080094A4
EC i16 SECTION(".text.DisplayFade_GetSpeed") DisplayFade_GetSpeed(DisplayFade * self)
{
    return self->state->step;
}

EC bool DisplayFade_Update(DisplayFade *);

// 0x080094AC
EC bool SECTION(".text.DisplayFadeTask_Update") DisplayFadeTask_Update(u8 * self)
{
    return DisplayFade_Update(reinterpret_cast<DisplayFade *>(self + 0xC));
}

struct DisplayRegisterBuffer
{
    union
    {
        u32 display_control;
        u16 register_halfwords[0x2B];
    };
};

// 0x080096B0
EC SECTION(".text.DisplayRegisters_Initialize") DisplayRegisterBuffer * DisplayRegisters_Initialize(DisplayRegisterBuffer * self)
{
    CpuFill16(0, self, 0x56);
    self->display_control = 0x80;
    self->register_halfwords[0x20 / 2] = 0x100;
    self->register_halfwords[0x22 / 2] = 0;
    self->register_halfwords[0x24 / 2] = 0;
    self->register_halfwords[0x26 / 2] = 0x100;
    self->register_halfwords[0x30 / 2] = 0x100;
    self->register_halfwords[0x32 / 2] = 0;
    self->register_halfwords[0x34 / 2] = 0;
    self->register_halfwords[0x36 / 2] = 0x100;
    return self;
}

EC void Dma3_Copy(void const *, void *, u32);

// 0x080096F0
EC void SECTION(".text.DisplayRegisters_Commit") DisplayRegisters_Commit(DisplayRegisterBuffer * self)
{
    Dma3_Copy(self, reinterpret_cast<void *>(0x04000000), 0x54);
    *reinterpret_cast<u16 volatile *>(0x04000054) = self->register_halfwords[0x54 / 2];
}

struct OamEntry
{
    u8 y;
    u8 attribute0_high;
    u16 attribute1;
    u16 attribute2;
    u16 affine_parameter;
};

struct OamBuffer
{
    u8 sprite_count;
    u8 reserved_sprite_count;
    u8 affine_count;
    u8 reserved_affine_count;
    OamEntry sprites[128];
};

// 0x08009744
EC SECTION(".text.OamBuffer_Initialize") OamBuffer * OamBuffer_Initialize(OamBuffer * self, u8 sprite_count, u8 affine_count)
{
    self->sprite_count = self->reserved_sprite_count = sprite_count & 0x7F;
    self->affine_count = self->reserved_affine_count = affine_count & 0x1F;
    OamEntry * sprite = self->sprites;
    OamEntry * end = self->sprites + 128;
    do
    {
        sprite->attribute0_high = 2;
        ++sprite;
    }
    while (sprite != end);
    return self;
}

// 0x0800977C
EC void SECTION(".text.OamBuffer_Commit") OamBuffer_Commit(OamBuffer * self)
{
    Dma3_Copy(self->sprites, reinterpret_cast<void *>(0x07000000), 0x400);
}

// 0x08009834
EC void SECTION(".text.OamBuffer_Reset") OamBuffer_Reset(OamBuffer * self)
{
    u32 sprite_count = self->sprite_count;
    u32 reserved_count = self->reserved_sprite_count;
    if (sprite_count > reserved_count)
    {
        OamEntry * sprite = self->sprites + reserved_count;
        OamEntry * end = self->sprites + sprite_count;
        do
        {
            sprite->attribute0_high = 2;
            ++sprite;
        }
        while (sprite != end);
    }
    self->sprite_count = reserved_count;
    self->affine_count = self->reserved_affine_count;
}

// 0x08009864
EC void SECTION(".text.OamBuffer_CommitAndReset") OamBuffer_CommitAndReset(OamBuffer * self)
{
    OamBuffer_Commit(self);
    OamBuffer_Reset(self);
}

struct TaskNode;
struct TaskList;

struct TaskVtable
{
    void (*abi_header_00)();
    void (*abi_header_04)();
    bool (*run)(TaskNode *);
    void (*destroy)();
    void (*append)();
    void (*remove)(TaskList *, TaskNode *);
};

extern TaskVtable gTaskNodeVtable; // 0x080E5BE8
extern TaskVtable gTaskListVtable; // 0x080E5BB4
extern TaskVtable gIrqTaskListVtable; // 0x080E5B90

struct TaskNode
{
    void Unlink()
    {
        if (previous_link != nullptr)
        {
            *previous_link = next;
            next->previous_link = previous_link;
        }
    }

    TaskNode ** previous_link;
    TaskNode * next;
    TaskVtable * vtable;
};

struct TaskList
{
    TaskNode node;
    TaskNode * first;
    TaskNode sentinel;
};

EC void TaskList_Append(TaskList *, TaskNode *);
EC void TaskList_Remove(TaskList *, TaskNode *);
EC void TaskList_Clear(TaskList *);
EC void TaskList_Splice(TaskList *, TaskList *);
EC void TaskList_SwapHeads(TaskList *, TaskList *);
EC void TaskNode_Destroy(TaskNode *, u32);
EC void TaskList_Destroy(TaskList *, u32);
EC u16 Irq_Disable(u16);
EC u16 Irq_Enable(u16);

struct ScopedIrqDisable
{
    ScopedIrqDisable(u16 mask) { previously_enabled = mask & Irq_Disable(mask); }
    ~ScopedIrqDisable()
    {
        if (previously_enabled != 0)
            Irq_Enable(previously_enabled);
    }
    u16 previously_enabled;
};

// 0x0800956C
EC void SECTION(".text.IrqTaskList_Clear") IrqTaskList_Clear(TaskList * self)
{
    ScopedIrqDisable lock(1);
    TaskList_Clear(self);
}

// 0x0800959C
EC void SECTION(".text.IrqTaskList_Destroy") IrqTaskList_Destroy(TaskList * self, u32 destructor_flags)
{
    self->node.vtable = &gIrqTaskListVtable;
    IrqTaskList_Clear(self);
    TaskList_Destroy(self, destructor_flags);
}

// 0x080095C0
EC void SECTION(".text.IrqTaskList_Append") IrqTaskList_Append(TaskList * self, TaskNode * node)
{
    ScopedIrqDisable lock(1);
    TaskList_Append(self, node);
}

// 0x080095FC
EC void SECTION(".text.IrqTaskList_Remove") IrqTaskList_Remove(TaskList * self, TaskNode * node)
{
    ScopedIrqDisable lock(1);
    TaskList_Remove(self, node);
}

// 0x08009638
EC void SECTION(".text.IrqTaskList_Splice") IrqTaskList_Splice(TaskList * self, TaskList * other)
{
    ScopedIrqDisable lock(1);
    TaskList_Splice(self, other);
}

// 0x08009674
EC void SECTION(".text.IrqTaskList_SwapHeads") IrqTaskList_SwapHeads(TaskList * self, TaskList * other)
{
    ScopedIrqDisable lock(1);
    TaskList_SwapHeads(self, other);
}

// 0x080098AC
EC void SECTION(".text.TaskNode_Destroy") TaskNode_Destroy(TaskNode * self, u32 destructor_flags)
{
    self->vtable = &gTaskNodeVtable;
    self->Unlink();
    if (destructor_flags & 1)
        delete self;
}

// 0x080098DC
EC void SECTION(".text.TaskList_Destroy") TaskList_Destroy(TaskList * self, u32 destructor_flags)
{
    self->node.vtable = &gTaskListVtable;
    TaskList_Clear(self);
    TaskNode_Destroy(&self->sentinel, 2);
    TaskNode_Destroy(&self->node, destructor_flags);
}

// 0x08009908
EC bool SECTION(".text.TaskList_Update") TaskList_Update(TaskList * self)
{
    TaskNode * next = self->first;
    TaskNode * end = &self->sentinel;
    while (next != end)
    {
        TaskNode * current = next;
        next = next->next;
        if (!current->vtable->run(current))
            self->node.vtable->remove(self, current);
    }
    return true;
}

// 0x08009940
EC void SECTION(".text.TaskList_Append") TaskList_Append(TaskList * self, TaskNode * node)
{
    node->Unlink();
    TaskNode ** last = self->sentinel.previous_link;
    *last = node;
    node->previous_link = last;
    node->next = &self->sentinel;
    self->sentinel.previous_link = &node->next;
}

// 0x08009968
EC void SECTION(".text.TaskList_Remove") TaskList_Remove(TaskList * self, TaskNode * node)
{
    if (node->previous_link != nullptr)
    {
        node->Unlink();
        node->previous_link = nullptr;
        node->next = nullptr;
    }
}

// 0x08009984
EC void SECTION(".text.TaskList_Clear") TaskList_Clear(TaskList * self)
{
    TaskNode * node = self->first;
    TaskNode * end = &self->sentinel;
    while (node != end)
    {
        TaskNode * current = node;
        node = node->next;
        current->previous_link = nullptr;
        current->next = nullptr;
    }
    self->first = &self->sentinel;
    self->sentinel.previous_link = &self->first;
}

// 0x080099B0
EC void SECTION(".text.TaskList_Splice") TaskList_Splice(TaskList * self, TaskList * other)
{
    TaskNode ** last = self->sentinel.previous_link;
    *last = other->first;
    other->first->previous_link = last;
    TaskNode ** other_last = other->sentinel.previous_link;
    *other_last = &self->sentinel;
    self->sentinel.previous_link = other_last;
    other->first = &other->sentinel;
    other->sentinel.previous_link = &other->first;
}

// 0x080099D4
EC void SECTION(".text.TaskList_SwapHeads") TaskList_SwapHeads(TaskList * self, TaskList * other)
{
    TaskNode * first = self->first;
    self->first = other->first;
    other->first = first;
    TaskNode ** last = self->sentinel.previous_link;
    TaskNode ** other_last = other->sentinel.previous_link;
    TaskNode * end = *last;
    *last = *other_last;
    *other_last = end;
}

EC void ResourceHandles_Destroy(ResourceHandleOwner * self, u32 destructor_flags) ALIAS(_._19ResourceHandleOwner);

struct BootDmaChannel
{
    u32 source;
    u32 destination;
    u16 count;
    u16 volatile control;
};

inline void Boot_StopDmaChannel(BootDmaChannel * channel)
{
    u16 volatile * registers = reinterpret_cast<u16 volatile *>(channel);
    registers[5] = registers[5] & 0xC5FF;
    registers[5] = registers[5] & 0x7FFF;
    (void)registers[5];
}

// 0x08000240
EC void SECTION(".text.Boot_SoftResetFromKeys") Boot_SoftResetFromKeys()
{
    Irq_Disable(0xFFFF);
    *reinterpret_cast<u16 volatile *>(0x04000000) = 0x80;
    *reinterpret_cast<u8 volatile *>(0x04000084) = 0;
    while ((0xF & ~*reinterpret_cast<u16 volatile *>(0x04000130)) != 0) {}
    Boot_StopDmaChannel(reinterpret_cast<BootDmaChannel *>(0x040000B0));
    Boot_StopDmaChannel(reinterpret_cast<BootDmaChannel *>(0x040000BC));
    Boot_StopDmaChannel(reinterpret_cast<BootDmaChannel *>(0x040000C8));
    Boot_StopDmaChannel(reinterpret_cast<BootDmaChannel *>(0x040000D4));
    SoftReset(0xFF);
    for (;;) {}
}

struct VBlankIrqSubscription
{
    u8 storage[0x14];
    void const * vtable;
};

struct VBlankRootTaskList
{
    TaskList tasks;
    VBlankIrqSubscription irq_subscription;
};

struct VBlankRuntime
{
    TaskNode audio_update;
    TaskList pending_tasks;
    TaskNode audio_vblank;
    VBlankRootTaskList root;
    bool restore_audio_dma;
};

struct VBlankDispatcher
{
    VBlankRuntime * state;
};

struct HardwareDmaAllocation
{
    u32 allocation_state;
    DmaCommand * capacity_end;
};

struct HardwareDmaQueueStorage
{
    DmaCommand * begin;
    DmaCommand * end;
    HardwareDmaAllocation allocation;
};

struct HardwareRuntime
{
    KeyRepeatState keys;
    union
    {
        DmaTransferQueue dma_queue;
        HardwareDmaQueueStorage dma_storage;
    };
    DisplayRegisterBuffer display;
    OamBuffer oam;
    VBlankDispatcher vblank;
    TaskList frame_tasks;
};

extern void const * gHardwareContextVtable[]; // 0x080E5B00
extern void const * gVBlankIrqSubscriptionVtable[]; // 0x080E5B0C
extern TaskVtable gVBlankRootTaskListVtable; // 0x080E5B18
extern TaskVtable gDisplayFadeTaskVtable; // 0x080E5B80
EC void func_080D7944(VBlankIrqSubscription *, u32);

EC void func_08008980(VBlankDispatcher *);
extern TaskVtable vtable_unk_080E5BD8;

inline void HardwareDmaAllocation_Initialize(HardwareDmaAllocation * self) { self->capacity_end = nullptr; }
inline u16 DisplayRegisters_ReadHalfword(u16 * registers, int index) { return registers[index]; }

// 0x08008444
EC SECTION(".text.HardwareContext_Initialize") HardwareContext * HardwareContext_Initialize(HardwareContext * self)
{
    TaskList * task_list;
    TaskList * frame_tasks;
    u16 * display_registers;
    HardwareContextState * initialized_state;
    volatile u32 stack_padding;
    self->vtable = gHardwareContextVtable;
    HardwareRuntime * state = new HardwareRuntime;
    KeyRepeat_Initialize(&state->keys, 30, 4);
    state->dma_storage.begin = nullptr;
    state->dma_storage.end = nullptr;
    HardwareDmaAllocation_Initialize(&state->dma_storage.allocation);
    DisplayRegisters_Initialize(&state->display);
    OamBuffer_Initialize(&state->oam, 0, 32);
    func_08008980(&state->vblank);
    TaskList * tasks = &state->frame_tasks;
    frame_tasks = tasks;
    task_list = tasks;
    frame_tasks->node.previous_link = nullptr;
    frame_tasks->node.next = nullptr;
    tasks->node.vtable = &gTaskListVtable;
    frame_tasks->first = &task_list->sentinel;
    TaskNode * sentinel = &frame_tasks->sentinel;
    sentinel->previous_link = nullptr;
    display_registers = state->display.register_halfwords;
    sentinel->next = nullptr;
    sentinel->vtable = &vtable_unk_080E5BD8;
    frame_tasks->sentinel.previous_link = &frame_tasks->first;
    task_list->sentinel.next = &frame_tasks->sentinel;
    u16 display_status = DisplayRegisters_ReadHalfword(display_registers, 2) | 8;
    state->display.register_halfwords[2] = display_status;
    initialized_state = reinterpret_cast<HardwareContextState *>(state);
    *reinterpret_cast<u16 volatile *>(0x04000004) = display_status;
    self->state = initialized_state;
    return self;
}

// 0x08008A68
EC void SECTION(".text.VBlankDispatcher_Destroy") VBlankDispatcher_Destroy(VBlankDispatcher * self, u32 destructor_flags)
{
    VBlankRuntime * state = self->state;
    if (state != nullptr)
    {
        if (state->restore_audio_dma)
            Audio_DisableVBlankDma();
        VBlankRootTaskList * root = &state->root;
        root->irq_subscription.vtable = gVBlankIrqSubscriptionVtable;
        root->tasks.node.vtable = &gVBlankRootTaskListVtable;
        func_080D7944(&root->irq_subscription, 0);
        IrqTaskList_Destroy(&root->tasks, 2);
        TaskNode_Destroy(&state->audio_vblank, 2);
        IrqTaskList_Destroy(&state->pending_tasks, 2);
        TaskNode_Destroy(&state->audio_update, 2);
        delete state;
    }
    if (destructor_flags & 1)
        delete self;
}

// 0x080086BC
EC void SECTION(".text.HardwareContext_Destroy") HardwareContext_Destroy(HardwareContext * self, u32 destructor_flags)
{
    self->vtable = gHardwareContextVtable;
    if (self->state != nullptr)
    {
        HardwareRuntime * state = reinterpret_cast<HardwareRuntime *>(self->state);
        TaskList_Destroy(&state->frame_tasks, 2);
        VBlankDispatcher_Destroy(&state->vblank, 2);
        DmaTransferQueue * queue = &state->dma_queue;
        DmaCommand * end = queue->End();
        DmaCommand * command = queue->Begin();
        while (command != end)
            ++command;
        if (queue->Begin() != nullptr)
            free(queue->Begin());
        delete state;
    }
    if (destructor_flags & 1)
        delete self;
}

struct DisplayFadeTask
{
    TaskNode node;
    DisplayFade fade;
};

// 0x08009508
EC i16 SECTION(".text.DisplayFadeTask_SetSpeed") DisplayFadeTask_SetSpeed(DisplayFadeTask * self, i16 step)
{
    return DisplayFade_SetSpeed(&self->fade, step);
}

// 0x080094B8
EC void SECTION(".text.DisplayFadeTask_Destroy") DisplayFadeTask_Destroy(DisplayFadeTask * self, u32 destructor_flags)
{
    self->node.vtable = &gDisplayFadeTaskVtable;
    DisplayFade_Destroy(&self->fade, 2);
    TaskNode_Destroy(&self->node, destructor_flags);
}

// 0x08009094
EC void SECTION(".text.DmaCommand_ExecuteCpuRange") DmaCommand_ExecuteCpuRange(DmaCommand const * begin, DmaCommand const * end)
{
    DmaCommand const * limit = end;
    for (DmaCommand const * command = begin; command != limit; ++command)
    {
        u8 const * source = reinterpret_cast<u8 const *>(command->source_or_fill_value);
        u32 fill_value;
        if (command->is_fill == 1)
        {
            fill_value = u32(source);
            source = reinterpret_cast<u8 const *>(&fill_value);
        }
        u32 control = command->control_flags;
        u8 * destination = static_cast<u8 *>(command->destination);
        if (control & 0x04000000)
        {
            u32 count = control & 0xFFFF;
            u32 tail = count & 7;
            count -= tail;
            if (count != 0 || tail == 0)
            {
                CpuFastSet(source, destination, control - tail);
                u32 byte_count = count * 4;
                destination += byte_count;
                if (!(control & 0x01000000))
                    source += byte_count;
            }
            if (tail != 0)
                CpuSet(source, destination, control - count);
        }
        else
            CpuSet(source, destination, control);
    }
}

// 0x080071BC
EC void ResourceHandles_ReserveRange(ResourceHandleOwner * self, unsigned int first_index, unsigned int entry_count)
{
    // https://decomp.me/scratch/e4deE (solved, but messy)

    (void)self;

    ResourceHandleRegistry * registry = gResourceHandleRegistry;

    if (registry->reserved_count != 0)
    {
        for (unsigned int index = registry->reserved_start, end = registry->reserved_start + registry->reserved_count; index < end; index++)
        {
            if (registry->IsAllocated(index))
            {
                registry->FreeEntry(&registry->GetEntry(index));
                registry->ClearAllocated(index);
            }
        }

        registry->reserved_count = 0;
    }

    if (entry_count != 0)
    {
        unsigned int end_index = entry_count + first_index;

        for (unsigned int index = 0; index < 0x10; index++)
        {
            if (registry->IsAllocated(index))
            {
                if (first_index <= index && index < end_index)
                {
                    registry->GetEntry(index).params.generation = 0;
                    registry->FreeEntry(&registry->GetEntry(index));
                    registry->ClearAllocated(index);
                    registry->allocated_count--;
                }
            }
        }

        unsigned int alloc_mask = 0;
        unsigned int target_mask = ((1 << entry_count) - 1u) << first_index;

        for (;;)
        {
            ResourceHandleEntry * entry = registry->AllocEntry(registry->free_head);

            if (entry == nullptr)
                goto alloc_loop; /* break doesn't match D= */

            registry->SetAllocated(registry->IndexOf(entry));
            alloc_mask |= 1u << registry->IndexOf(entry);

            if ((alloc_mask & target_mask) == target_mask)
                break;
        }

    alloc_loop:
        for (unsigned int index = 0; (alloc_mask & ~target_mask) != 0 && index < 0x10; index++)
        {
            unsigned int bit = 1u << index;

            if (alloc_mask & bit & ~target_mask)
            {
                registry->FreeEntry(&registry->GetEntry(index));
                registry->ClearAllocated(index);

                alloc_mask &= ~bit;
            }
        }

        registry->reserved_start = first_index;
        registry->reserved_count = entry_count;
    }
}
