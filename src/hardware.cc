#include "prelude.h"

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

EC void func_08009268(HardwareDisplayState *);

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
