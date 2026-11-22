#ifndef KEY_REPEAT_HH
#define KEY_REPEAT_HH

#include "prelude.h"

struct KeyRepeatState
{
    u16 held_keys;
    u16 alignment_padding;
    u16 pressed_keys;
    u16 edge_padding;
    u16 repeated_keys;
    u16 repeat_delay;
    u16 repeat_period;
    i16 repeat_counters[10];
};

typedef char KeyRepeatButtonsOffsetCheck[offsetof(KeyRepeatState, repeated_keys) == 8 ? 1 : -1];

#endif // KEY_REPEAT_HH
