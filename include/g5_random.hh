#ifndef G5_RANDOM_HH
#define G5_RANDOM_HH

#include "prelude.h"
#include <stdlib.h>

// rand (0x080D11E4) advances the u32 state at 0x03000A50:
//     state = (state * 0x41C64E6D + 12345) modulo 2^32.
// It returns state & 0x7FFFFFFF, not the high 15 bits used by some libc LCGs.
// The full 32-bit state, including bit 31, is retained for the next draw.
// The seed setter at 0x080D11DC stores its u32 argument unchanged and shares
// rand's parameter table. The generator has one global, non-reentrant stream.

#endif
