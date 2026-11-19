#include "g5_context.hh"

struct HardwareContext;
struct HardwareContextState;
struct MessageWindow;

EC i32 GetMessageWindowPhase(MessageWindow * window);
EC u32 UpdateMessageWindow(MessageWindow * window, u32 input, u32 oam, u32 registers, u32 frame_tasks, u32 vblank_tasks, u32 setting);
EC HardwareContextState * HardwareContext_GetState(HardwareContext * context);
EC void * HardwareContext_GetOamBuffer(HardwareContext * context);
EC void * HardwareContext_GetDisplayRegisters(HardwareContext * context);
EC void * HardwareContext_GetFrameTasks(HardwareContext * context);
EC void * HardwareContext_GetVBlankTasks(HardwareContext * context);
EC void func_080CC58C(void * panel, void * oam);

EC SECTION(".text.g5_080CD8A0") void InventoryMenu_UpdateMessageWindow(u8 * menu) // 0x080CD8A0
{
    HardwareContext * context = (HardwareContext *) menu;
    MessageWindow * window = (MessageWindow *) (menu + 0x80);
    *(u32 *) (menu + 0x84) = GetMessageWindowPhase(window);
    HardwareContextState * state = HardwareContext_GetState(context);
    void * oam = HardwareContext_GetOamBuffer(context);
    void * registers = HardwareContext_GetDisplayRegisters(context);
    void * frame_tasks = HardwareContext_GetFrameTasks(context);
    void * vblank_tasks = HardwareContext_GetVBlankTasks(context);
    void * setting = menu + 0xc;
    *(u32 *) (menu + 0x88) = UpdateMessageWindow(window, (u32) state, (u32) oam, (u32) registers, (u32) frame_tasks, (u32) vblank_tasks, (u32) setting);
    u8 * panel = *(u8 **) (menu + 0x78);
    if (panel != nullptr)
    {
        G5InventoryPanelVTable * vtable = *(G5InventoryPanelVTable **) (panel + 0x210);
        vtable->update(panel);
    }
    void * cursor = *(void **) (menu + 0x7c);
    if (cursor != nullptr)
        func_080CC58C(cursor, HardwareContext_GetOamBuffer(context));
}
