#ifndef G5_IRQ_HH
#define G5_IRQ_HH

#include "prelude.h"

typedef void (*G5IrqHandler)();

// Each setter returns the previous callback or dispatch mask.
EC G5IrqHandler Irq_SetHandler(u32 irq_index, G5IrqHandler handler); // 0x080D100C
EC u16 Irq_SetDispatchFlags(u32 irq_index, u16 irq_mask); // 0x080D101C

#endif
