#include "prelude.h"
#include "gbasvc.h"

EC void Irq_Dispatch();
EC void Irq_SetHandler(u32 irq_index, void (*handler)());
EC u16 Irq_SetDispatchFlags(u32 irq_index, u16 irq_mask);
EC u16 Irq_Disable(u16 irq_mask);
extern u8 const Irq_ApplyMaskArm[]; // 0x03000A1C

inline u32 (*Irq_GetMaskRoutine())(u32)
{
    union
    {
        u32 address;
        u32 (*routine)(u32);
    } entry;
    entry.address = u32(Irq_ApplyMaskArm);
    return entry.routine;
}

// 0x080004F4
EC u16 SECTION(".text.Irq_Enable") Irq_Enable(u16 irq_mask)
{
    u32 mask = u32(irq_mask) << 16;
    return Irq_GetMaskRoutine()(mask);
}

// 0x08000528
EC u16 SECTION(".text.Irq_Disable") Irq_Disable(u16 irq_mask)
{
    u32 mask = irq_mask;
    return Irq_GetMaskRoutine()(mask);
}

// 0x080004C4
EC void Irq_Initialize()
{
    Irq_Disable(0xFFFF);
    *reinterpret_cast<void (**)()>(0x03007FFC) = Irq_Dispatch;
    for (u32 irq_index = 0; irq_index <= 13; ++irq_index)
        Irq_SetHandler(irq_index, nullptr);
}

EC u16 Irq_Enable(u16 irq_mask);

// 0x0800050C
EC u16 SECTION(".text.ack") Irq_AcknowledgeAndEnable(u16 irq_mask)
{
    *reinterpret_cast<u16 volatile *>(0x04000202) = irq_mask;
    return Irq_Enable(irq_mask);
}

// 0x08000540
EC u16 SECTION(".text.set") Irq_SetHandlerFlags(u32 irq_index, u16 irq_mask)
{
    return Irq_SetDispatchFlags(irq_index, irq_mask);
}

// 0x08000568
EC void SECTION(".text.wait") Irq_Wait(u16 irq_mask)
{
    IntrWait(true, irq_mask);
}
