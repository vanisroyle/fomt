#include "g5_inventory.hh"
#include "sprite_animation.hh"
#include "sprite_rendering.hh"

struct InventoryEntryFrameView
{
    SpriteFrameObject const * objects;
    u16 object_count;
    u8 const * tiles;
    u16 tile_bytes;
    u16 const * palettes;
    u16 palette_bytes;
    SpriteFrameExtra const * extras;
    u16 extra_count;
};

struct InventoryEntryFrameVTable
{
    u32 prefix[4];
    void (*read_frame)(InventoryEntryFrameView *, SpriteAnimationSource const *, u32);
};

EC SpriteAnimationData * InitSpriteAnimationData(SpriteAnimationData *, void const *);
EC InventoryEntryFrameVTable const vtable_unk_080E5A28;

struct InventoryEntryAnimationData
{
    InventoryEntryFrameVTable const * vtable;
    u32 state[11];

    InventoryEntryAnimationData(void const * data)
    {
        InitSpriteAnimationData(reinterpret_cast<SpriteAnimationData *>(this), data);
    }

    ~InventoryEntryAnimationData()
    {
        vtable = &vtable_unk_080E5A28;
    }
};

struct InventoryEntryPreview
{
    InventoryEntryAnimationData data;
    u16 position[2];
    struct AnimationState
    {
        SpriteAnimationSource const * source;
        struct Clip
        {
            SpriteKeyframe const * keyframes;
            u16 length;
        } clip;
        u16 keyframe;
        u16 remaining_q8;
        i16 speed_q8;
        bool refresh_frame;
    } animation;
    SpriteFrameDrawOptions draw;
    u8 active;

    InventoryEntryPreview(void const * sprite_data, u16 animation_id) : data(sprite_data)
    {
        register u16 * coordinates asm("r0") = position;
        asm ("" : "+l" (coordinates));
        register u32 byte_zero asm("r6") = 0;
        register u32 word_zero asm("r4") = 0;
        asm ("" : "+l" (byte_zero), "+l" (word_zero));
        coordinates[0] = word_zero;
        coordinates[1] = word_zero;
        InitSpriteAnimator(reinterpret_cast<SpriteAnimator *>(&animation),
            reinterpret_cast<SpriteAnimationSource const *>(&data), animation_id, 256);

        register SpriteFrameDrawOptions * options asm("r0") = &draw;
        asm ("" : "+l" (options));
        options->offset_x = word_zero;
        options->offset_y = word_zero;
        options->palette_base = word_zero;
        options->tile_base = word_zero;
        options->priority = word_zero;
        options->object_mode = word_zero;
        options->flip = byte_zero;
        options->affine_index = byte_zero;
        options->affine_mode = word_zero;
        options->mosaic = byte_zero;
        register u8 * active_flag asm("r0") = &active;
        asm ("" : "+l" (active_flag));
        *active_flag = byte_zero;
    }
};

struct InventoryEntryAnimationWork
{
    InventoryEntryPreview preview;
    InventoryEntryFrameView frame;

    InventoryEntryAnimationWork(void const * data, u16 animation) : preview(data, animation) {}
};

typedef char InventoryEntryAnimationLayoutCheck[
    sizeof(InventoryEntryAnimationData) == 0x30 &&
    sizeof(InventoryEntryPreview::AnimationState) == sizeof(SpriteAnimator) &&
    offsetof(InventoryEntryPreview::AnimationState, keyframe) == offsetof(SpriteAnimator, keyframe) &&
    sizeof(InventoryEntryFrameView) == sizeof(SpriteFrameView) &&
    offsetof(InventoryEntryFrameView, tiles) == offsetof(SpriteFrameView, tile_bytes) &&
    offsetof(InventoryEntryFrameView, palettes) == offsetof(SpriteFrameView, palette_bytes) &&
    offsetof(InventoryEntryPreview, animation) == 0x34 &&
    offsetof(InventoryEntryAnimationWork, frame) == 0x68 &&
    sizeof(InventoryEntryAnimationWork) == 0x88 ? 1 : -1];

static inline void AssignAnimatedEntry(UiEntry * entry, void const * data, u16 animation, u32 value)
{
    InventoryEntryAnimationWork work(data, animation);
    register InventoryEntryFrameView * destination asm("r0") = &work.frame;
    asm ("" : "+l" (destination));
    register InventoryEntryPreview::AnimationState const * animator asm("r2");
    // The sole local work block starts at sp; rematerialize its animator address.
    asm ("add %0, sp, %1" : "=l" (animator)
        : "I" (offsetof(InventoryEntryPreview, animation)));
    register SpriteAnimationSource const * source asm("r1") = work.preview.animation.source;
    asm ("" : "+l" (source));
    register InventoryEntryFrameVTable const * vtable asm("r4") =
        *reinterpret_cast<InventoryEntryFrameVTable const * const *>(source);
    asm ("" : "+l" (vtable));
    register u32 keyframe_offset asm("r3") = animator->keyframe;
    asm ("" : "+l" (keyframe_offset));
    keyframe_offset *= sizeof(SpriteKeyframe);
    asm ("" : "+l" (keyframe_offset));
    register SpriteKeyframe const * keyframe asm("r2") = work.preview.animation.clip.keyframes;
    asm ("" : "+l" (keyframe));
    keyframe = reinterpret_cast<SpriteKeyframe const *>(reinterpret_cast<uintptr_t>(keyframe) + keyframe_offset);
    asm ("" : "+l" (keyframe));
    register u32 frame_id asm("r2") = keyframe->frame;
    asm ("" : "+l" (frame_id));
    register void (*read_frame)(InventoryEntryFrameView *, SpriteAnimationSource const *, u32) asm("r3") =
        vtable->read_frame;
    asm ("" : "+l" (read_frame));
    read_frame(destination, source, frame_id);
    entry->tile_data = reinterpret_cast<uintptr_t>(work.frame.tiles);
    entry->palette_data = reinterpret_cast<uintptr_t>(work.frame.palettes);
    entry->value = value;
}

EC SECTION(".text.g5_080CAC7C") UiEntry * InitializeUiEntryFromAnimation(
    UiEntry * entry, void const * data, u16 animation, u32 value) // 0x080CAC7C
{
    AssignAnimatedEntry(entry, data, animation, value);
    return entry;
}

EC SECTION(".text.g5_080CAD18") void AssignUiEntryFromAnimation(
    UiEntry * entry, void const * data, u16 animation, u32 value) // 0x080CAD18
{
    AssignAnimatedEntry(entry, data, animation, value);
}
