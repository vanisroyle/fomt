#include "g5_irq.hh"

EC G5IrqHandler gUnk_03000450[];
EC u16 gUnk_03000430[];

EC SECTION(".text.g5_080D100C") G5IrqHandler Irq_SetHandler(u32 irq_index, G5IrqHandler handler) // 0x080D100C
{
    G5IrqHandler * handlers = gUnk_03000450;
    asm ("" : "+l" (handlers));
    handlers += irq_index;
    G5IrqHandler previous = *handlers;
    *handlers = handler;
    return previous;
}

EC SECTION(".text.g5_080D100C") u16 Irq_SetDispatchFlags(u32 irq_index, u16 irq_mask) // 0x080D101C
{
    u16 * flags = gUnk_03000430;
    asm ("" : "+l" (flags));
    flags += irq_index;
    u16 previous = *flags;
    *flags = irq_mask;
    return previous;
}
