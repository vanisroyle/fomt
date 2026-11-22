#include "g5_graphics.hh"
#include "g5_initialization.hh"
#include "g5_memory.hh"
#include "sprite_animation.hh"

EC void * func_080D3BC0(u32);
EC void func_08007874(void *);
EC u32 func_08007B54(void *, u32);
EC void ResourceHandles_Initialize(void *);
EC u32 func_0800736C(void *);
EC SpriteAnimationData * InitSpriteAnimationData(SpriteAnimationData *, void const *);
EC u8 const gUnk_08527094[];

EC SECTION(".text.g5_080CE2FC") u8 * InventoryMenu_InitializePagedDrawPanel(u8 * panel, void * owner) // 0x080CE2FC
{
    u32 constructor_frame[2];
    asm ("" : "=m" (constructor_frame));
    void ** members = (void **) panel;
    *members++ = owner;
    u32 * handles = (u32 *) members;
    func_08007874(handles);
    handles[1] = func_08007B54(handles, 2);
    handles = (u32 *) (panel + 0xc);
    func_08007874(handles);
    handles[1] = func_08007B54(handles, 2);
    handles += 2;
    ResourceHandles_Initialize(handles);
    handles[1] = func_0800736C(handles);
    register SpriteAnimationData * data asm("r6") = (SpriteAnimationData *) (panel + 0x1c);
    asm ("" : "+l" (data));
    InitSpriteAnimationData(data, gUnk_08527094);
    register u16 * position asm("r0") = (u16 *) (panel + 0x4c);
    asm ("" : "+l" (position));
    register u32 byte_zero asm("r5") = 0;
    register u32 word_zero asm("r4") = 0;
    asm ("" : "+l" (byte_zero), "+l" (word_zero));
    position[0] = word_zero;
    position[1] = word_zero;
    position += 2;
    {
        register SpriteAnimator * animator asm("r0") = (SpriteAnimator *) position;
        asm ("" : "+l" (animator));
        register SpriteAnimationSource const * source asm("r1") = (SpriteAnimationSource const *) data;
        register u32 clip asm("r2") = 0;
        asm ("" : "+l" (source), "+l" (clip));
        register int speed asm("r3") = 256;
        asm ("" : "+l" (animator), "+l" (source), "+l" (clip), "+l" (speed));
        InitSpriteAnimator(animator, source, clip, speed);
    }
    {
        register G5SpriteDrawState * draw asm("r0") = (G5SpriteDrawState *) (panel + 0x64);
        asm ("" : "+l" (draw));
        register u16 half_zero asm("r4") = word_zero;
        register u8 flag_zero asm("r5") = byte_zero;
        asm ("" : "+l" (half_zero), "+l" (flag_zero));
        draw->unknown_00 = half_zero;
        draw->unknown_02 = half_zero;
        draw->unknown_04 = half_zero;
        draw->unknown_06 = half_zero;
        draw->unknown_08 = half_zero;
        draw->unknown_0c = word_zero;
        draw->unknown_10 = flag_zero;
        draw->unknown_11 = flag_zero;
        draw->unknown_14 = word_zero;
        draw->unknown_18 = flag_zero;
        register u8 * enabled asm("r0") = (u8 *) (draw + 1);
        asm ("" : "+l" (enabled));
        *enabled = flag_zero;
        position = (u16 *) (enabled + 4);
        asm ("" : "+l" (position));
    }
    position[0] = word_zero;
    position[1] = word_zero;
    position += 2;
    {
        register SpriteAnimator * animator asm("r0") = (SpriteAnimator *) position;
        asm ("" : "+l" (animator));
        register SpriteAnimationSource const * source asm("r1") = (SpriteAnimationSource const *) data;
        register u32 clip asm("r2") = 1;
        asm ("" : "+l" (source), "+l" (clip));
        register int speed asm("r3") = 256;
        asm ("" : "+l" (animator), "+l" (source), "+l" (clip), "+l" (speed));
        InitSpriteAnimator(animator, source, clip, speed);
    }
    {
        register G5SpriteDrawState * draw asm("r0") = (G5SpriteDrawState *) (panel + 0x9c);
        asm ("" : "+l" (draw));
        register u16 half_zero asm("r4") = word_zero;
        register u8 flag_zero asm("r5") = byte_zero;
        asm ("" : "+l" (half_zero), "+l" (flag_zero));
        draw->unknown_00 = half_zero;
        draw->unknown_02 = half_zero;
        draw->unknown_04 = half_zero;
        draw->unknown_06 = half_zero;
        draw->unknown_08 = half_zero;
        draw->unknown_0c = word_zero;
        draw->unknown_10 = flag_zero;
        draw->unknown_11 = flag_zero;
        draw->unknown_14 = word_zero;
        draw->unknown_18 = flag_zero;
        register u8 * enabled asm("r0") = (u8 *) (draw + 1);
        asm ("" : "+l" (enabled));
        *enabled = flag_zero;
        register u32 * page asm("r0") = (u32 *) (enabled + 4);
        asm ("" : "+l" (page));
        *page = word_zero;
    }
    register CommandList * commands asm("r5") = (CommandList *) (panel + 0xc0);
    asm ("" : "+l" (commands));
    commands->sentinel = (CommandNode *) word_zero;
    CommandNode * sentinel = (CommandNode *) malloc(sizeof(CommandNode));
    if (sentinel == nullptr)
        sentinel = (CommandNode *) func_080D3BC0(sizeof(CommandNode));
    sentinel->next = sentinel;
    sentinel->previous = sentinel;
    commands->sentinel = sentinel;
    return panel;
}
