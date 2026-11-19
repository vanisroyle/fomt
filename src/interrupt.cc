#include "prelude.h"
#include "gbasvc.h"

EC void func_03000958();
EC void func_080D100C(u32 index, void (*handler)());
EC u16 func_080D101C(u32 index, u16 flags);
EC u16 func_08000528(u16 flags);

EC void func_080004C4()
{
    func_08000528(0xFFFF);
    *reinterpret_cast<void (**)()>(0x03007FFC) = func_03000958;
    for (u32 i = 0; i <= 13; ++i)
        func_080D100C(i, nullptr);
}

EC u16 func_080004F4(u16 flags);

EC u16 SECTION(".text.ack") func_0800050C(u16 flags)
{
    *reinterpret_cast<u16 volatile *>(0x04000202) = flags;
    return func_080004F4(flags);
}

EC u16 SECTION(".text.set") func_08000540(u32 index, u16 flags)
{
    return func_080D101C(index, flags);
}

EC void SECTION(".text.wait") func_08000568(u16 flags)
{
    IntrWait(true, flags);
}
