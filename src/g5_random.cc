#include "g5_random.hh"

struct RandomLcgParameters
{
    u32 * state;
    u32 multiplier;
    u32 increment;
};

EC RandomLcgParameters const gRandomLcgParameters;

EC SECTION(".text.g5_rand") int rand(void) // 0x080D11E4
{
    register RandomLcgParameters const * parameters asm("r0");
    register u32 * state asm("r1");
    register u32 multiplier asm("r2");
    register u32 increment asm("r3");
    // Keep the shared seed/multiplier/increment table at its PC-relative slot.
    asm ("add %0, pc, #16\n\tldmia %0!, {%1, %2, %3}"
        : "=l" (parameters), "=l" (state), "=l" (multiplier), "=l" (increment));
    u32 next = *state;
    next *= multiplier;
    next += increment;
    *state = next;
    next <<= 1;
    asm ("" : "+l" (next));
    return next >> 1;
}

EC SECTION(".text.g5_rand") RandomLcgParameters const gRandomLcgParameters =
{
    (u32 *) 0x03000A50,
    0x41C64E6D,
    12345,
};
