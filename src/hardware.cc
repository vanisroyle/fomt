#include "prelude.h"
#include "m4a.h"
#include "gbasvc.h"
#include "unknown_inlines.hh"

union Unk_hardware_ent_080D6D98
{
    Unk_hardware_ent_080D6D98 * next_free;

    struct
    {
        u16 unk_00;
        u16 unk_02;
    } params;
};

EC Unk_hardware_ent_080D6D98 * func_080D6D98(Unk_hardware_ent_080D6D98 * self, int num, int val);

struct Unk_hardware_03000404
{
    Unk_hardware_03000404()
    {
        unk_00 = func_080D6D98(unk_04, 0x10, 0);
        unk_44 = 0;
        unk_48 = 0;
        unk_4C = 0;
        unk_50 = 0;
        unk_51 = 0;
        unk_44 = 0;
    }

    bool inl_pred_0(unsigned int idx) const { return idx < 0x10 && (unk_44 & (1 << (idx % 0x20u))) != 0; }

    void inl_func_0(unsigned int idx)
    {
        if (idx < 0x10)
            unk_44 &= ~(1 << (idx % 0x20u));
    }

    void inl_func_1(unsigned int idx)
    {
        if (idx < 0x10)
            unk_44 |= (1 << (idx % 0x20u));
    }

    Unk_hardware_ent_080D6D98 & GetEntry(unsigned int idx) { return unk_04[idx]; }

    unsigned int IndexOf(Unk_hardware_ent_080D6D98 const * ent) const { return ent - unk_04; }

    bool Matches(Unk_hardware_ent_080D6D98 const & ent, u32 handle) const
    {
        return u16(handle >> 4) == ent.params.unk_02;
    }

    void FreeEntry(Unk_hardware_ent_080D6D98 * ent)
    {
        ent->next_free = unk_00;
        unk_00 = ent;
    }

    Unk_hardware_ent_080D6D98 * AllocEntry(Unk_hardware_ent_080D6D98 * ent)
    {
        Unk_hardware_ent_080D6D98 * prev = unk_00;

        if (ent != nullptr)
            unk_00 = ent->next_free;

        return prev;
    }

    /* +00 */ Unk_hardware_ent_080D6D98 * unk_00;
    /* +04 */ Unk_hardware_ent_080D6D98 unk_04[0x10];
    /* +44 */ u32 unk_44;
    /* +48 */ u16 unk_48;
    /* +4A */ u16 unk_4A;
    /* +4C */ u32 unk_4C;
    /* +50 */ u8 unk_50;
    /* +51 */ u8 unk_51;
};

extern Unk_hardware_03000404 * gUnk_03000404;

struct Unk_hardware_H
{
    ~Unk_hardware_H();
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

EC Unk_hardware_H * func_08007128(Unk_hardware_H * self)
{
    if (gUnk_03000404 == nullptr)
    {
        gUnk_03000404 = new Unk_hardware_03000404;
    }

    gUnk_03000404->unk_4C++;

    return self;
}

EC Unk_hardware_H * func_08007174(Unk_hardware_H * self, Unk_hardware_H const & other)
{
    gUnk_03000404->unk_4C++;

    return self;
}

Unk_hardware_H::~Unk_hardware_H()
{
    gUnk_03000404->unk_4C--;

    if (gUnk_03000404->unk_4C == 0)
    {
        if (gUnk_03000404 != nullptr)
            delete gUnk_03000404;

        gUnk_03000404 = nullptr;
    }
}

EC u32 SECTION(".text.handle_retain") func_0800745C(Unk_hardware_H * self, u32 handle)
{
    Unk_hardware_03000404 * h = gUnk_03000404;
    if (handle != 0)
    {
        unsigned int idx = HardwareHandle(handle).Index();
        if (h->inl_pred_0(idx))
        {
            Unk_hardware_ent_080D6D98 & ent = h->GetEntry(idx);
            if (h->Matches(ent, handle))
            {
                u16 previous = ent.params.unk_00;
                if (++ent.params.unk_00 != 0)
                    return handle;
                ent.params.unk_00 = previous;
            }
        }
    }
    return 0;
}

EC u16 SECTION(".text.handle_count") func_08007510(Unk_hardware_H * self, u32 handle)
{
    Unk_hardware_03000404 * h = gUnk_03000404;
    if (handle != 0)
    {
        unsigned int idx = HardwareHandle(handle).Index();
        if (h->inl_pred_0(idx))
        {
            Unk_hardware_ent_080D6D98 & ent = h->GetEntry(idx);
            if (h->Matches(ent, handle))
                return ent.params.unk_00;
        }
    }
    return 0;
}

struct HardwarePool
{
    void * data;
};

EC SECTION(".text.func_0800770C") HardwarePool * func_0800770C(HardwarePool * self)
{
    self->data = nullptr;
    return self;
}

struct HardwareBitset
{
    u32 bits;
};

EC void SECTION(".text.func_08007844") func_08007844(HardwareBitset * self, u32 index, u32 order)
{
    if (order <= 4)
        self->bits &= ~(((1u << (1u << order)) - 1) << (index % 32));
    else if (order == 5)
        self->bits = 0;
}

struct HardwareTreeNode
{
    u8 full;
    u8 empty;
    u8 data[122];
};

struct HardwareTree
{
    u8 full;
    u8 empty;
    u16 pad;
    HardwareTreeNode children[2];
};

EC void func_080D6EEC(HardwareTreeNode *);
EC void func_080D6F5C(HardwareTreeNode *);

EC void SECTION(".text.func_08007EA8") func_08007EA8(HardwareTree * self)
{
    func_080D6EEC(&self->children[0]);
    func_080D6EEC(&self->children[1]);
    self->full = 1;
    self->empty = 0;
}

EC void SECTION(".text.func_08007EC8") func_08007EC8(HardwareTree * self)
{
    func_080D6F5C(&self->children[1]);
    func_080D6F5C(&self->children[0]);
    self->full = 0;
    self->empty = 1;
}

struct HardwareDisplayState
{
    u16 unk_00;
    u16 unk_02;
    u16 unk_04;
    u16 unk_06;
    u16 unk_08;
    STRUCT_PAD(0x0A, 0x24);
    u8 unk_24[0x10];
    u8 unk_34[0x58];
    u8 unk_8C[0x404];
    u8 unk_490[4];
    u8 unk_494[0x1C];
};

struct HardwareDisplay
{
    HardwareDisplayState * state;
};

EC u32 func_08009268(void *);

EC void SECTION(".text.func_080088B8") func_080088B8(HardwareDisplay * self)
{
    func_08009268(self->state);
}

EC u16 SECTION(".text.func_080088C4") func_080088C4(HardwareDisplay * self)
{
    return self->state->unk_00;
}

EC u16 SECTION(".text.func_080088CC") func_080088CC(HardwareDisplay * self)
{
    return self->state->unk_04;
}

EC u16 SECTION(".text.func_080088D4") func_080088D4(HardwareDisplay * self)
{
    return self->state->unk_08;
}

EC SECTION(".text.func_080088DC") HardwareDisplayState * func_080088DC(HardwareDisplay * self)
{
    return self->state;
}

EC SECTION(".text.func_08008910") void * func_08008910(HardwareDisplay * self)
{
    return self->state->unk_24;
}

EC SECTION(".text.func_08008918") void * func_08008918(HardwareDisplay * self)
{
    return self->state->unk_34;
}

EC SECTION(".text.func_08008920") void * func_08008920(HardwareDisplay * self)
{
    return self->state->unk_8C;
}

EC SECTION(".text.func_08008940") void * func_08008940(HardwareDisplay * self)
{
    return self->state->unk_494;
}

EC void * func_08008AE0(void ** self);

EC SECTION(".text.func_0800894C") void * func_0800894C(HardwareDisplay * self)
{
    return func_08008AE0(reinterpret_cast<void **>(self->state->unk_490));
}

EC SECTION(".text.func_08008AE0") void * func_08008AE0(void ** self)
{
    return static_cast<u8 *>(*self) + 0xC;
}

EC void func_08000568(u16 flags);

EC void SECTION(".text.func_08008AF0") func_08008AF0(void * self)
{
    func_08000568(1);
}

EC void SECTION(".text.func_08008AFC") func_08008AFC()
{
    m4aSoundInit();
    m4aSoundVSyncOff();
}

EC bool SECTION(".text.func_08008B0C") func_08008B0C()
{
    m4aSoundVSync();
    return true;
}

EC bool SECTION(".text.func_08008B18") func_08008B18()
{
    m4aSoundMain();
    return true;
}

struct HardwareMusicPlayer
{
    MusicPlayer * player;
};

EC MusicPlayer * func_08008B24(u32 index);

EC SECTION(".text.func_08008B54") HardwareMusicPlayer * func_08008B54(HardwareMusicPlayer * self, u8 index)
{
    self->player = func_08008B24(index);
    return self;
}

EC void SECTION(".text.func_08008B6C") func_08008B6C(HardwareMusicPlayer * self, u16 song)
{
    m4aMPlayStart(self->player, gSongTable[song].song);
}

EC void SECTION(".text.func_08008B88") func_08008B88(HardwareMusicPlayer * self, u16 song)
{
    Song * target = gSongTable[song].song;
    MusicPlayer * player = self->player;
    if (player->song != target || i32(player->status) < 0)
        m4aMPlayStart(player, target);
}

EC void SECTION(".text.func_08008BB0") func_08008BB0(HardwareMusicPlayer * self, u16 song)
{
    Song * target = gSongTable[song].song;
    MusicPlayer * player = self->player;
    if (player->song != target)
        m4aMPlayStart(player, target);
    else if (i32(player->status) < 0)
        m4aMPlayContinue(player);
}

EC void SECTION(".text.func_08008BE0") func_08008BE0(HardwareMusicPlayer * self)
{
    m4aMPlayStop(self->player);
}

EC void SECTION(".text.func_08008BF8") func_08008BF8(HardwareMusicPlayer * self, u16 speed)
{
    m4aMPlayFadeOut(self->player, speed);
}

EC void SECTION(".text.func_08008C28") func_08008C28(HardwareMusicPlayer * self, u16 tempo)
{
    m4aMPlayTempoControl(self->player, tempo);
}

EC void SECTION(".text.func_08008C38") func_08008C38(HardwareMusicPlayer * self, u16 volume, u16 tracks)
{
    m4aMPlayVolumeControl(self->player, tracks, volume);
}

EC void SECTION(".text.func_08008C54") func_08008C54(HardwareMusicPlayer * self, i16 pitch, u16 tracks)
{
    m4aMPlayPitchControl(self->player, tracks, pitch);
}

EC void SECTION(".text.func_08008C70") func_08008C70(HardwareMusicPlayer * self, i8 pan, u16 tracks)
{
    m4aMPlayPanpotControl(self->player, tracks, pan);
}

EC void SECTION(".text.func_08008C8C") func_08008C8C(HardwareMusicPlayer * self, u8 depth, u16 tracks)
{
    m4aMPlayModDepthSet(self->player, tracks, depth);
}

EC void SECTION(".text.func_08008CA8") func_08008CA8(HardwareMusicPlayer * self, u8 speed, u16 tracks)
{
    m4aMPlayLFOSpeedSet(self->player, tracks, speed);
}

EC void SECTION(".text.func_08008CC4") func_08008CC4(HardwareMusicPlayer * self)
{
    m4aMPlayImmInit(self->player);
}

EC bool SECTION(".text.func_08008CD0") func_08008CD0(HardwareMusicPlayer * self)
{
    return (self->player->status >> 31) ^ 1;
}

EC SECTION(".text.func_08008D3C") MusicPlayer * func_08008D3C(u16 song)
{
    HardwareMusicPlayer result;
    func_08008B54(&result, gSongTable[song].ms);
    return result.player;
}

EC void SECTION(".text.func_08008D84") func_08008D84()
{
    m4aSoundVSyncOn();
}

EC void SECTION(".text.func_08008D90") func_08008D90()
{
    m4aSoundVSyncOff();
}

EC bool SECTION(".text.func_08008D9C") func_08008D9C()
{
    return *reinterpret_cast<u32 volatile *>(0x040000C4) >> 31;
}

EC void SECTION(".text.func_08008DA8") func_08008DA8(u16 song)
{
    m4aSongNumStart(song);
}

EC void SECTION(".text.func_08008DB8") func_08008DB8(u16 song)
{
    m4aSongNumStartOrChange(song);
}

EC void SECTION(".text.func_08008DE8") func_08008DE8()
{
    m4aMPlayAllStop();
}

inline u32 HardwareDmaControl(u32 src, u32 dst, u32 size)
{
    if (size != 0 && ((src | dst | size) & 1) == 0)
    {
        if (((src | dst | size) & 3) == 0)
            return u16(size / 4) | 0x84000000;
        return u16(size / 2) | 0x80000000;
    }
    return 0;
}

EC void func_080D0EBC(void const *, void *, u32, void volatile *);

EC void SECTION(".text.func_08008EB8") func_08008EB8(u32 value, void * dst, u32 size)
{
    if (dst != nullptr)
    {
        u32 control = HardwareDmaControl(0, u32(dst), size) | 0x01000000;
        func_080D0EBC(&value, dst, control,
            reinterpret_cast<void volatile *>(0x040000D4));
    }
}

struct HardwareDmaCommand
{
    u32 fill;
    u32 src;
    void * dst;
    u32 control;
};

EC SECTION(".text.func_08008F0C") HardwareDmaCommand * func_08008F0C(
    HardwareDmaCommand * self, void const * src, void * dst, u32 size)
{
    u32 control = src != nullptr && dst != nullptr ? HardwareDmaControl(u32(src), u32(dst), size) : 0;
    self->fill = 0;
    self->src = u32(src);
    self->dst = dst;
    self->control = control;
    return self;
}

EC SECTION(".text.func_08008F60") HardwareDmaCommand * func_08008F60(
    HardwareDmaCommand * self, u32 value, void * dst, u32 size)
{
    u32 control = dst != nullptr ? HardwareDmaControl(0, u32(dst), size) | 0x01000000 : 0;
    self->fill = 1;
    self->src = value;
    self->dst = dst;
    self->control = control;
    return self;
}

struct HardwareKeys
{
    u16 current;
    u16 pad;
    u16 pressed;
    u16 pad_06;
    u16 repeat;
    u16 delay;
    u16 period;
    i16 counters[10];
};

EC u32 SECTION(".text.func_0800912C") func_0800912C()
{
    return ~*reinterpret_cast<u16 volatile *>(0x04000130) & 0x3FF;
}

EC SECTION(".text.func_08009140") HardwareKeys * func_08009140(HardwareKeys * self)
{
    self->current = func_0800912C();
    return self;
}

EC SECTION(".text.func_08009154") HardwareKeys * func_08009154(HardwareKeys * self, u32 keys)
{
    self->current = keys;
    return self;
}

EC u32 SECTION(".text.func_08009158") func_08009158(HardwareKeys * self)
{
    u32 keys = func_0800912C();
    self->current = keys;
    return keys;
}

EC SECTION(".text.func_08009168") HardwareKeys * func_08009168(HardwareKeys * self)
{
    func_08009140(self);
    self->pressed = self->current;
    return self;
}

EC SECTION(".text.func_0800917C") HardwareKeys * func_0800917C(HardwareKeys * self, u32 keys)
{
    func_08009154(self, keys);
    self->pressed = self->current;
    return self;
}

EC u32 SECTION(".text.func_08009190") func_08009190(HardwareKeys * self)
{
    u16 previous = self->current;
    u32 pressed = func_08009158(self) & ~previous;
    self->pressed = pressed;
    return pressed;
}

EC SECTION(".text.func_080091A4") HardwareKeys * func_080091A4(HardwareKeys * self, u16 delay, u16 period)
{
    func_08009168(self);
    self->repeat = self->pressed;
    self->delay = delay;
    self->period = period;
    fill_inl(self->counters, self->counters + 10, 0);
    return self;
}

EC SECTION(".text.func_080091E8") HardwareKeys * func_080091E8(HardwareKeys * self, u32 keys)
{
    func_0800917C(self, keys);
    self->repeat = self->pressed;
    self->delay = 30;
    self->period = 4;
    fill_inl(self->counters, self->counters + 10, 0);
    return self;
}

EC u32 SECTION(".text.func_08009268") func_08009268(void * ptr)
{
    HardwareKeys * self = static_cast<HardwareKeys *>(ptr);
    u32 pressed = func_08009190(self);
    u32 current = self->current;
    u32 period = self->period;
    u32 delay = self->delay;
    u32 repeat = 0;
    u32 mask = 0x200;
    i16 * counter = self->counters + 9;
    do
    {
        if (current & mask)
        {
            int value;
            if ((pressed & mask) == 0)
            {
                value = *counter;
                if (value != delay)
                    ++value;
                else
                {
                    repeat |= mask;
                    value -= period;
                }
            }
            else
            {
                repeat |= mask;
                value = 0;
            }
            *counter = value;
        }
        --counter;
        mask >>= 1;
    }
    while (mask != 0);
    self->repeat = repeat;
    return repeat;
}

EC void SECTION(".text.func_080092C8") func_080092C8(HardwareKeys * self, u16 delay)
{
    u32 current = self->current;
    u32 mask = 0x200;
    i16 * counter = self->counters + 9;
    int delta = delay - self->delay;
    do
    {
        if (current & mask)
            *counter += delta;
        --counter;
        mask >>= 1;
    }
    while (mask != 0);
    self->delay = delay;
}

EC void SECTION(".text.func_080092FC") func_080092FC(HardwareKeys * self, u32 period)
{
    self->period = period;
}

struct HardwareFadeState
{
    HardwareFadeState(void * registers)
        : registers(registers), speed(0), mode(0), mosaic(false), progress(0) {}
    void * registers;
    i16 speed;
    u32 mode;
    bool mosaic;
    u16 progress;
};

struct HardwareFade
{
    HardwareFadeState * state;
};

EC SECTION(".text.func_08009300") HardwareFade * func_08009300(HardwareFade * self, void * registers)
{
    self->state = new HardwareFadeState(registers);
    return self;
}

EC void SECTION(".text.func_08009324") func_08009324(HardwareFade * self, u32 flags)
{
    delete self->state;
    if (flags & 1)
        delete self;
}

EC SECTION(".text.func_08009344") void * func_08009344(HardwareFade * self, void * registers)
{
    void * previous = self->state->registers;
    self->state->registers = registers;
    return previous;
}

EC void SECTION(".text.func_0800934C") func_0800934C(HardwareFade * self, i16 speed, u32 mode, u32 mosaic)
{
    HardwareFadeState * state = self->state;
    int value = -(speed < 0 ? -speed : speed);
    bool mosaic_enabled = mosaic != 0;
    state->speed = value;
    state->mode = mode;
    state->mosaic = mosaic_enabled;
    state->progress = 0xFFFF;
}

EC void SECTION(".text.func_08009378") func_08009378(HardwareFade * self, i16 speed, u32 mode, u32 mosaic)
{
    HardwareFadeState * state = self->state;
    int value = speed < 0 ? -speed : speed;
    bool mosaic_enabled = mosaic != 0;
    state->speed = value;
    state->mode = mode;
    state->mosaic = mosaic_enabled;
    state->progress = 0;
}

EC i16 SECTION(".text.func_080093A0") func_080093A0(HardwareFade * self, i16 speed)
{
    i16 previous = self->state->speed;
    self->state->speed = speed;
    return previous;
}

EC i16 SECTION(".text.func_080094A4") func_080094A4(HardwareFade * self)
{
    return self->state->speed;
}

EC bool func_080093AC(HardwareFade *);

EC bool SECTION(".text.func_080094AC") func_080094AC(u8 * self)
{
    return func_080093AC(reinterpret_cast<HardwareFade *>(self + 0xC));
}

struct HardwareRegisters
{
    union
    {
        u32 control;
        u16 halfwords[0x2B];
    };
};

EC SECTION(".text.func_080096B0") HardwareRegisters * func_080096B0(HardwareRegisters * self)
{
    CpuFill16(0, self, 0x56);
    self->control = 0x80;
    self->halfwords[0x20 / 2] = 0x100;
    self->halfwords[0x22 / 2] = 0;
    self->halfwords[0x24 / 2] = 0;
    self->halfwords[0x26 / 2] = 0x100;
    self->halfwords[0x30 / 2] = 0x100;
    self->halfwords[0x32 / 2] = 0;
    self->halfwords[0x34 / 2] = 0;
    self->halfwords[0x36 / 2] = 0x100;
    return self;
}

EC void func_08008E64(void const *, void *, u32);

EC void SECTION(".text.func_080096F0") func_080096F0(HardwareRegisters * self)
{
    func_08008E64(self, reinterpret_cast<void *>(0x04000000), 0x54);
    *reinterpret_cast<u16 volatile *>(0x04000054) = self->halfwords[0x54 / 2];
}

struct HardwareSprite
{
    u8 y;
    u8 flags;
    u16 x;
    u16 tile;
    u16 affine;
};

struct HardwareSprites
{
    u8 count;
    u8 initial_count;
    u8 affine_count;
    u8 initial_affine_count;
    HardwareSprite sprites[128];
};

EC SECTION(".text.func_08009744") HardwareSprites * func_08009744(HardwareSprites * self, u8 count, u8 affine_count)
{
    self->count = self->initial_count = count & 0x7F;
    self->affine_count = self->initial_affine_count = affine_count & 0x1F;
    HardwareSprite * it = self->sprites;
    HardwareSprite * end = self->sprites + 128;
    do
    {
        it->flags = 2;
        ++it;
    }
    while (it != end);
    return self;
}

EC void SECTION(".text.func_0800977C") func_0800977C(HardwareSprites * self)
{
    func_08008E64(self->sprites, reinterpret_cast<void *>(0x07000000), 0x400);
}

EC void SECTION(".text.func_08009834") func_08009834(HardwareSprites * self)
{
    u32 count = self->count;
    u32 initial = self->initial_count;
    if (count > initial)
    {
        HardwareSprite * it = self->sprites + initial;
        HardwareSprite * end = self->sprites + count;
        do
        {
            it->flags = 2;
            ++it;
        }
        while (it != end);
    }
    self->count = initial;
    self->affine_count = self->initial_affine_count;
}

EC void SECTION(".text.func_08009864") func_08009864(HardwareSprites * self)
{
    func_0800977C(self);
    func_08009834(self);
}

struct HardwareTaskNode;
struct HardwareTaskList;

struct HardwareTaskVtable
{
    void (*unk_00)();
    void (*unk_04)();
    bool (*run)(HardwareTaskNode *);
    void (*unk_0C)();
    void (*unk_10)();
    void (*remove)(HardwareTaskList *, HardwareTaskNode *);
};

extern HardwareTaskVtable vtable_unk_080E5BE8;
extern HardwareTaskVtable vtable_unk_080E5BB4;
extern HardwareTaskVtable vtable_unk_080E5B90;

struct HardwareTaskNode
{
    void Unlink()
    {
        if (previous != nullptr)
        {
            *previous = next;
            next->previous = previous;
        }
    }

    HardwareTaskNode ** previous;
    HardwareTaskNode * next;
    HardwareTaskVtable * vtable;
};

struct HardwareTaskList
{
    HardwareTaskNode node;
    HardwareTaskNode * first;
    HardwareTaskNode sentinel;
};

EC void func_08009940(HardwareTaskList *, HardwareTaskNode *);
EC void func_08009968(HardwareTaskList *, HardwareTaskNode *);
EC void func_08009984(HardwareTaskList *);
EC void func_080099B0(HardwareTaskList *, HardwareTaskList *);
EC void func_080099D4(HardwareTaskList *, HardwareTaskList *);
EC void func_080098AC(HardwareTaskNode *, u32);
EC void func_080098DC(HardwareTaskList *, u32);
EC u16 func_08000528(u16);
EC u16 func_080004F4(u16);

struct HardwareInterruptLock
{
    HardwareInterruptLock(u16 mask) { saved = mask & func_08000528(mask); }
    ~HardwareInterruptLock()
    {
        if (saved != 0)
            func_080004F4(saved);
    }
    u16 saved;
};

EC void SECTION(".text.func_0800956C") func_0800956C(HardwareTaskList * self)
{
    HardwareInterruptLock lock(1);
    func_08009984(self);
}

EC void SECTION(".text.func_0800959C") func_0800959C(HardwareTaskList * self, u32 flags)
{
    self->node.vtable = &vtable_unk_080E5B90;
    func_0800956C(self);
    func_080098DC(self, flags);
}

EC void SECTION(".text.func_080095C0") func_080095C0(HardwareTaskList * self, HardwareTaskNode * node)
{
    HardwareInterruptLock lock(1);
    func_08009940(self, node);
}

EC void SECTION(".text.func_080095FC") func_080095FC(HardwareTaskList * self, HardwareTaskNode * node)
{
    HardwareInterruptLock lock(1);
    func_08009968(self, node);
}

EC void SECTION(".text.func_08009638") func_08009638(HardwareTaskList * self, HardwareTaskList * other)
{
    HardwareInterruptLock lock(1);
    func_080099B0(self, other);
}

EC void SECTION(".text.func_08009674") func_08009674(HardwareTaskList * self, HardwareTaskList * other)
{
    HardwareInterruptLock lock(1);
    func_080099D4(self, other);
}

EC void SECTION(".text.func_080098AC") func_080098AC(HardwareTaskNode * self, u32 flags)
{
    self->vtable = &vtable_unk_080E5BE8;
    self->Unlink();
    if (flags & 1)
        delete self;
}

EC void SECTION(".text.func_080098DC") func_080098DC(HardwareTaskList * self, u32 flags)
{
    self->node.vtable = &vtable_unk_080E5BB4;
    func_08009984(self);
    func_080098AC(&self->sentinel, 2);
    func_080098AC(&self->node, flags);
}

EC bool SECTION(".text.func_08009908") func_08009908(HardwareTaskList * self)
{
    HardwareTaskNode * next = self->first;
    HardwareTaskNode * end = &self->sentinel;
    while (next != end)
    {
        HardwareTaskNode * current = next;
        next = next->next;
        if (!current->vtable->run(current))
            self->node.vtable->remove(self, current);
    }
    return true;
}

EC void SECTION(".text.func_08009940") func_08009940(HardwareTaskList * self, HardwareTaskNode * node)
{
    node->Unlink();
    HardwareTaskNode ** last = self->sentinel.previous;
    *last = node;
    node->previous = last;
    node->next = &self->sentinel;
    self->sentinel.previous = &node->next;
}

EC void SECTION(".text.func_08009968") func_08009968(HardwareTaskList * self, HardwareTaskNode * node)
{
    if (node->previous != nullptr)
    {
        node->Unlink();
        node->previous = nullptr;
        node->next = nullptr;
    }
}

EC void SECTION(".text.func_08009984") func_08009984(HardwareTaskList * self)
{
    HardwareTaskNode * node = self->first;
    HardwareTaskNode * end = &self->sentinel;
    while (node != end)
    {
        HardwareTaskNode * current = node;
        node = node->next;
        current->previous = nullptr;
        current->next = nullptr;
    }
    self->first = &self->sentinel;
    self->sentinel.previous = &self->first;
}

EC void SECTION(".text.func_080099B0") func_080099B0(HardwareTaskList * self, HardwareTaskList * other)
{
    HardwareTaskNode ** last = self->sentinel.previous;
    *last = other->first;
    other->first->previous = last;
    HardwareTaskNode ** other_last = other->sentinel.previous;
    *other_last = &self->sentinel;
    self->sentinel.previous = other_last;
    other->first = &other->sentinel;
    other->sentinel.previous = &other->first;
}

EC void SECTION(".text.func_080099D4") func_080099D4(HardwareTaskList * self, HardwareTaskList * other)
{
    HardwareTaskNode * first = self->first;
    self->first = other->first;
    other->first = first;
    HardwareTaskNode ** last = self->sentinel.previous;
    HardwareTaskNode ** other_last = other->sentinel.previous;
    HardwareTaskNode * end = *last;
    *last = *other_last;
    *other_last = end;
}

EC void func_08007184(Unk_hardware_H * self) ALIAS(_._14Unk_hardware_H);

EC void func_080071BC(Unk_hardware_H * self, unsigned int from, unsigned int count)
{
    // https://decomp.me/scratch/e4deE (solved, but messy)

    (void)self;

    Unk_hardware_03000404 * h = gUnk_03000404;

    if (h->unk_51 != 0)
    {
        for (unsigned int i = h->unk_50, end = h->unk_50 + h->unk_51; i < end; i++)
        {
            if (h->inl_pred_0(i))
            {
                h->FreeEntry(&h->GetEntry(i));
                h->inl_func_0(i);
            }
        }

        h->unk_51 = 0;
    }

    if (count != 0)
    {
        unsigned int to = count + from;

        for (unsigned int i = 0; i < 0x10; i++)
        {
            if (h->inl_pred_0(i))
            {
                if (from <= i && i < to)
                {
                    h->GetEntry(i).params.unk_02 = 0;
                    h->FreeEntry(&h->GetEntry(i));
                    h->inl_func_0(i);
                    h->unk_48--;
                }
            }
        }

        unsigned int alloc_mask = 0;
        unsigned int target_mask = ((1 << count) - 1u) << from;

        for (;;)
        {
            Unk_hardware_ent_080D6D98 * ent = h->AllocEntry(h->unk_00);

            if (ent == nullptr)
                goto alloc_loop; /* break doesn't match D= */

            h->inl_func_1(h->IndexOf(ent));
            alloc_mask |= 1u << h->IndexOf(ent);

            if ((alloc_mask & target_mask) == target_mask)
                break;
        }

    alloc_loop:
        for (unsigned int i = 0; (alloc_mask & ~target_mask) != 0 && i < 0x10; i++)
        {
            unsigned int bit = 1u << i;

            if (alloc_mask & bit & ~target_mask)
            {
                h->FreeEntry(&h->GetEntry(i));
                h->inl_func_0(i);

                alloc_mask &= ~bit;
            }
        }

        h->unk_50 = from;
        h->unk_51 = count;
    }
}
