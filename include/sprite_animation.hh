#ifndef SPRITE_ANIMATION_HH
#define SPRITE_ANIMATION_HH

#include "prelude.h"

struct SpriteKeyframe
{
    u16 frame;
    u16 duration;
};

struct SpriteAnimationClip
{
    SpriteKeyframe const * keyframes;
    u16 length;

    SpriteAnimationClip(SpriteKeyframe const * keyframes = nullptr, u16 length = 0)
        : keyframes(keyframes), length(length) {}

    u32 GetLength() const { return keyframes != nullptr ? length : 0; }
};

struct SpriteAnimationSource;

struct SpriteAnimationSourceVTable
{
    STRUCT_PAD(0x00, 0x0C);
    void (*get_clip)(SpriteAnimationClip *, SpriteAnimationSource const *, u32);
};

struct SpriteAnimationSource
{
    virtual ~SpriteAnimationSource();
    virtual SpriteAnimationClip GetClip(u32 clip_id) const;
};

struct SpriteAnimationData;

struct SpriteFrameObject
{
    u32 attributes[2];
};

struct SpriteFrameExtra
{
    u32 words[2];
};

template <typename T>
struct SpriteFrameSlice
{
    T const * data;
    u16 length;

    SpriteFrameSlice(T const * data = nullptr, u16 length = 0) : data(data), length(length) {}
};

struct SpriteFrameView
{
    SpriteFrameSlice<SpriteFrameObject> objects;
    SpriteFrameSlice<u8> tile_bytes;
    SpriteFrameSlice<u16> palette_bytes;
    SpriteFrameSlice<SpriteFrameExtra> extras;

    SpriteFrameView() {}
    SpriteFrameView(SpriteFrameObject const * objects, u16 object_count,
        u8 const * tiles, u16 tile_byte_count, u16 const * palettes, u16 palette_byte_count,
        SpriteFrameExtra const * extras, u16 extra_count)
        : objects(objects, object_count),
          tile_bytes(tiles, tile_byte_count),
          palette_bytes(palettes, palette_byte_count),
          extras(extras, extra_count) {}
};

struct SpriteAnimator
{
    enum { KEYFRAME_CHANGED = 1, FRAME_CHANGED = 2, LOOP_FLAG_BIT = 2, LOOPED = 1 << LOOP_FLAG_BIT };

    SpriteAnimationSource const * source;
    SpriteAnimationClip clip;
    u16 keyframe;
    u16 remaining_q8;
    i16 speed_q8;
    bool refresh_frame;
};

EC SpriteFrameView GetSpriteAnimationDataFrame(SpriteAnimationData const * self, u32 frame_id);
EC SpriteAnimator * InitSpriteAnimator(SpriteAnimator * self, SpriteAnimationSource const * source, u32 clip_id, int speed_q8);
EC void BindSpriteAnimator(SpriteAnimator * self, SpriteAnimationSource const * source, u32 clip_id);
EC void SetSpriteAnimatorClip(SpriteAnimator * self, u32 clip_id);
EC bool WillSpriteAnimationLoop(SpriteAnimator const * self);
EC u32 AdvanceSpriteAnimator(SpriteAnimator * self);
EC u32 SumSpriteAnimationFrameDelays(SpriteAnimationSource const * source, u32 animation);

#endif // SPRITE_ANIMATION_HH
