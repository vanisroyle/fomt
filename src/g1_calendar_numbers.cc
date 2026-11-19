#include "prelude.h"

struct G1CalendarScene;

EC void func_080A3618(G1CalendarScene *, int, int, int, int);

EC SECTION(".text.g1_080A3588") void DrawCalendarNumber(G1CalendarScene * self, int x, int y, int number, int digits, int palette)
{
    int divisor = digits;
    int started = 0;
    int digit;
    if (divisor > 1)
    {
        divisor--;
        digit = 10;
        while (divisor > 1)
        {
            digit *= 10;
            divisor--;
        }
        divisor = digit;
    }
    else
        divisor = 1;
    while (divisor > 0)
    {
        if (divisor == 1)
            started = 1;
        digit = number / divisor;
        if (digit != 0 || started != 0)
        {
            func_080A3618(self, x, y, digit, palette);
            started = 1;
        }
        x += 16;
        number -= digit * divisor;
        divisor /= 10;
    }
}
