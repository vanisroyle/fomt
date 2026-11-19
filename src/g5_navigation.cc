#include "prelude.h"

struct NavigationPanel
{
    u8 pad_00[0x27c];
    bool enabled;
    u8 pad_27d[0x1b];
    int limit;
};
struct NavigationOwner
{
    u8 pad_00[0x78];
    NavigationPanel * first;
    NavigationPanel * second;
};
struct Navigation
{
    NavigationOwner * owner;
    int current;
    int previous;
    int first_limit;
    int second_limit;

    void UpdateLimits()
    {
        first_limit = owner->first->limit;
        second_limit = owner->second->limit + 9;
    }
};
EC int const gUnk_0811788C[][4];

EC SECTION(".text.g5_080CDA44") int func_080CDA44(Navigation * self)
{
    self->UpdateLimits();
    int next = gUnk_0811788C[self->current][0];
    if (self->current == 0)
    {
        int previous = self->previous;
        if (previous != 9)
        {
            u32 index = previous - 5;
            next = previous + 4;
            if (index <= 3)
                next = previous;
        }
    }
    else if (self->current == 9)
    {
        int previous = self->previous;
        if (previous != 0)
        {
            u32 index = previous - 14;
            next = previous + 4;
            if (index <= 3)
                next = previous;
        }
    }
    int current;
    if (next <= 8)
    {
        current = self->current;
        if (next == 0 && self->owner->first->enabled == 0)
            next = current + 4;
        while (next > self->first_limit)
            next = gUnk_0811788C[next][0];
    }
    else
    {
        current = self->current;
        if (next == 9 && self->owner->second->enabled == 0)
            next = current + 4;
        while (next > self->second_limit)
            next = gUnk_0811788C[next][0];
    }
    if (next != current)
    {
        self->previous = current;
        self->current = next;
    }
    return next;
}

