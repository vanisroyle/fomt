#include "prelude.h"

EC void Dma3_Fill(u32, void *, u32);
EC void func_0804EDB4(void *, void *, u32, u32, u32);
EC void func_080CD578(void *);
EC void func_080CD620(void *);
EC void func_08050E98(void *, u32);
EC void func_080CDDD4(void *);
EC void func_080CECE8(void *);
EC void func_080CE790(void *);
EC void func_080CD8A0(void *);
EC void func_080CE510(void *, u32, void *, u32, u32, u32);
EC u8 vtable_unk_080E85DC[];
EC u8 vtable_unk_080E85E8[];
EC void func_080A4A94(void *);
EC void __9FieldPlot(void *);

EC SECTION(".text.g5_080CFB28") void * func_080CFB28(u8 * self)
{
    func_080A4A94(self);
    *(void **) (self + 0x90) = vtable_unk_080E85E8;
    u8 * plot = self + 0x94;
    for (int i = 83; i != -1; plot += 4, --i)
        __9FieldPlot(plot);
    return self;
}

EC SECTION(".text.g5_080C8550") void func_080C8550(void ** self)
{
    Dma3_Fill(0x03ff03ff, (void *) 0x0600ff8a, 0x28);
    Dma3_Fill(0x03ff03ff, (void *) 0x0600ffca, 0x28);
    func_0804EDB4(*(void **) ((u8 *) self[2] + 0x1aa8), (void *) 0x0600ffb2, 0xa0, 0, 0x20);
}

EC SECTION(".text.g5_080CE19C") void * func_080CE19C(u8 * self)
{
    func_080CD578(self);
    *(void **) (self + 4) = vtable_unk_080E85DC;
    *(u32 *) (self + 0x8c) = 0;
    *(u32 *) (self + 0x90) = 0;
    *(u32 *) (self + 0x94) = 0;
    return self;
}

EC SECTION(".text.g5_080CE294") void func_080CE294(u8 * self)
{
    func_080CD620(self);
    void * buttons = self + 0x80;
    func_08050E98(buttons, 1);
    func_08050E98(buttons, 2);
    func_08050E98(buttons, 4);
    func_08050E98(buttons, 8);
}

EC SECTION(".text.g5_080CE294") void func_080CE2C4(void ** self)
{
    if (self[0x8c / 4] != nullptr)
        func_080CDDD4(self[0x8c / 4]);
    if (self[0x90 / 4] != nullptr)
        func_080CECE8(self[0x90 / 4]);
    if (self[0x94 / 4] != nullptr)
        func_080CE790(self[0x94 / 4]);
    func_080CD8A0(self);
}

EC SECTION(".text.g5_080CEB80") u32 func_080CEB80(u8 * self)
{
    if (*(u32 *) (self + 0xbc) < 7)
        ++*(u32 *) (self + 0xbc);
    else
        *(u32 *) (self + 0xbc) = 0;
    u32 * page = (u32 *) (self + 0xbc);
    func_080CE510(self, *page + 1, (void *) 0x0600f0f6, 0x20, 0, 0x20);
    return *page;
}

EC SECTION(".text.g5_080CEB80") u32 func_080CEBC0(u8 * self)
{
    if (*(u32 *) (self + 0xbc) != 0)
        --*(u32 *) (self + 0xbc);
    else
        *(u32 *) (self + 0xbc) = 7;
    u32 * page = (u32 *) (self + 0xbc);
    func_080CE510(self, *page + 1, (void *) 0x0600f0f6, 0x20, 0, 0x20);
    return *page;
}

struct PanelCursor
{
    u8 pad_00[0x68];
    u8 enabled;
    u8 pad_69[0x13];
    u32 current;
    u32 next;
    u8 active;
};

EC SECTION(".text.g5_080CECD0") void func_080CECD0(PanelCursor * self)
{
    self->active = 0;
    self->enabled = 0;
    self->current = 0x12;
    self->next = 0;
}
