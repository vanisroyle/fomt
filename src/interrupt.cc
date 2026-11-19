#include "prelude.h"
#include "gbasvc.h"

EC void Irq_Dispatch();
EC void Irq_SetHandler(u32 irq_index, void (*handler)());
EC u16 Irq_SetDispatchFlags(u32 irq_index, u16 irq_mask);
EC u16 Irq_Disable(u16 irq_mask);

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
