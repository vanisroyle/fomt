#include "g5_initialization.hh"
#include "sprite_animation.hh"

EC void func_08007874(void * owner);
EC u32 func_08007B54(void * owner, u32 order);
EC void ResourceHandles_Initialize(void * owner);
EC u32 func_0800736C(void * owner);
EC SpriteAnimationData * InitSpriteAnimationData(SpriteAnimationData * data, void const * source);
EC void func_08008444(void * context);
EC void AudioPlayer_Initialize(void * player, u32 tracks);
EC void TilePool_Initialize(void * pool);
EC void * ResourcePool_Initialize(void * pool);
EC void ResourcePool_Destroy(void * pool, u32 flags);
EC void TilePool_Destroy(void * pool, u32 flags);
EC void * CreateMessageWindow(void * window, u32 background, u32 tiles, u32 palette, u32 style, void * graphics, void * font);
EC u8 const gUnk_08754C0C[];
EC u8 const gUnk_08728208[];
EC u8 vtable_unk_080E85A0[];

EC SECTION(".text.g5_080CEC00") u8 * InventoryMenu_InitializeCursor(u8 * cursor, void * owner) // 0x080CEC00
{
    void ** members = (void **) cursor;
    *members++ = owner;
    SpriteAnimationData * data = (SpriteAnimationData *) members;
    InitSpriteAnimationData(data, gUnk_08754C0C);
    register u32 byte_zero asm("r6") = 0;
    register u32 word_zero asm("r5") = 0;
    asm ("" : "+l" (byte_zero), "+l" (word_zero));
    *(u16 *) (cursor + 0x34) = word_zero;
    *(u16 *) (cursor + 0x36) = word_zero;
    {
        register SpriteAnimator * animator asm("r0") = (SpriteAnimator *) (cursor + 0x38);
        asm ("" : "+l" (animator));
        register SpriteAnimationSource const * source asm("r1") = (SpriteAnimationSource const *) data;
        register u32 clip asm("r2") = 1;
        asm ("" : "+l" (source), "+l" (clip));
        register int speed asm("r3") = 256;
        asm ("" : "+l" (animator), "+l" (source), "+l" (clip), "+l" (speed));
        InitSpriteAnimator(animator, source, clip, speed);
    }
    register G5SpriteDrawState * draw_state asm("r0") = (G5SpriteDrawState *) (cursor + 0x4c);
    asm ("" : "+l" (draw_state));
    register u16 halfword_zero asm("r5") = word_zero;
    register u8 flag_zero asm("r6") = byte_zero;
    asm ("" : "+l" (halfword_zero), "+l" (flag_zero));
    draw_state->unknown_00 = halfword_zero;
    draw_state->unknown_02 = halfword_zero;
    draw_state->unknown_04 = halfword_zero;
    draw_state->unknown_06 = halfword_zero;
    draw_state->unknown_08 = halfword_zero;
    draw_state->unknown_0c = word_zero;
    draw_state->unknown_10 = flag_zero;
    draw_state->unknown_11 = flag_zero;
    draw_state->unknown_14 = word_zero;
    draw_state->unknown_18 = flag_zero;
    u8 * enabled = (u8 *) (draw_state + 1);
    asm ("" : "+l" (enabled));
    *enabled = flag_zero;
    u32 * handles = (u32 *) (cursor + 0x6c);
    func_08007874(handles);
    handles[1] = func_08007B54(handles, 4);
    handles += 2;
    ResourceHandles_Initialize(handles);
    handles[1] = func_0800736C(handles);
    *(u32 *) (cursor + 0x7c) = 0x12;
    *(u32 *) (cursor + 0x80) = word_zero;
    *(u8 *) (cursor + 0x84) = byte_zero;
    return cursor;
}

EC SECTION(".text.g5_080CD9B0") u8 * InventoryMenu_InitializeNavigation(u8 * navigation, InventoryNavigationOwner * owner, int slot) // 0x080CD9B0
{
    InventoryNavigation * selection = (InventoryNavigation *) navigation;
    selection->owner = owner;
    selection->selected_slot = slot;
    selection->previous_slot = slot;
    u32 * handles = (u32 *) (navigation + 0x14);
    func_08007874(handles);
    handles[1] = func_08007B54(handles, 2);
    handles += 2;
    ResourceHandles_Initialize(handles);
    handles[1] = func_0800736C(handles);
    SpriteAnimationData * data = (SpriteAnimationData *) (navigation + 0x24);
    InitSpriteAnimationData(data, gUnk_08728208);
    register u16 * position asm("r0") = (u16 *) (navigation + 0x54);
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
    register G5SpriteDrawState * draw_state asm("r0") = (G5SpriteDrawState *) (navigation + 0x6c);
    asm ("" : "+l" (draw_state));
    register u16 halfword_zero asm("r4") = word_zero;
    register u8 flag_zero asm("r5") = byte_zero;
    asm ("" : "+l" (halfword_zero), "+l" (flag_zero));
    draw_state->unknown_00 = halfword_zero;
    draw_state->unknown_02 = halfword_zero;
    draw_state->unknown_04 = halfword_zero;
    draw_state->unknown_06 = halfword_zero;
    draw_state->unknown_08 = halfword_zero;
    draw_state->unknown_0c = word_zero;
    draw_state->unknown_10 = flag_zero;
    draw_state->unknown_11 = flag_zero;
    draw_state->unknown_14 = word_zero;
    draw_state->unknown_18 = flag_zero;
    u8 * enabled = (u8 *) (draw_state + 1);
    asm ("" : "+l" (enabled));
    *enabled = flag_zero;
    selection->UpdateLimits();
    return navigation;
}

EC SECTION(".text.g5_080CD578") u8 * InventoryMenu_InitializeContext(u8 * menu, void * owner) // 0x080CD578
{
    func_08008444(menu);
    *(void **) (menu + 4) = vtable_unk_080E85A0;
    *(void **) (menu + 8) = owner;
    AudioPlayer_Initialize(menu + 0xc, 1);
    SpriteAnimationData * data = (SpriteAnimationData *) (menu + 0x10);
    InitSpriteAnimationData(data, gUnk_08754C0C);
    register u16 * position asm("r0") = (u16 *) (menu + 0x40);
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
    register G5SpriteDrawState * draw_state asm("r0") = (G5SpriteDrawState *) (menu + 0x58);
    asm ("" : "+l" (draw_state));
    register u16 halfword_zero asm("r4") = word_zero;
    register u8 flag_zero asm("r5") = byte_zero;
    asm ("" : "+l" (halfword_zero), "+l" (flag_zero));
    draw_state->unknown_00 = halfword_zero;
    draw_state->unknown_02 = halfword_zero;
    draw_state->unknown_04 = halfword_zero;
    draw_state->unknown_06 = halfword_zero;
    draw_state->unknown_08 = halfword_zero;
    draw_state->unknown_0c = word_zero;
    draw_state->unknown_10 = flag_zero;
    draw_state->unknown_11 = flag_zero;
    draw_state->unknown_14 = word_zero;
    draw_state->unknown_18 = flag_zero;
    u8 * enabled = (u8 *) (draw_state + 1);
    asm ("" : "+l" (enabled));
    *enabled = flag_zero;
    *(u32 *) (menu + 0x78) = word_zero;
    *(u32 *) (menu + 0x7c) = word_zero;
    u32 tile_pool;
    u32 font_pool;
    TilePool_Initialize(&tile_pool);
    ResourcePool_Initialize(&font_pool);
    CreateMessageWindow(menu + 0x80, 0, 0x1d, 0, word_zero, &tile_pool, &font_pool);
    ResourcePool_Destroy(&font_pool, 2);
    TilePool_Destroy(&tile_pool, 2);
    return menu;
}
