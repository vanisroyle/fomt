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

EC SpriteAnimator * InitSpriteAnimator(SpriteAnimator * self, SpriteAnimationSource const * source, u32 clip_id, int speed_q8);
EC void BindSpriteAnimator(SpriteAnimator * self, SpriteAnimationSource const * source, u32 clip_id);
EC void SetSpriteAnimatorClip(SpriteAnimator * self, u32 clip_id);
EC bool WillSpriteAnimationLoop(SpriteAnimator const * self);
EC u32 AdvanceSpriteAnimator(SpriteAnimator * self);
EC u32 SumSpriteAnimationFrameDelays(SpriteAnimationSource const * source, u32 animation);

#endif // SPRITE_ANIMATION_HH
