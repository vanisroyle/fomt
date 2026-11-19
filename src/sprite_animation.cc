#include "sprite_animation.hh"

#define ANIMATION_CODE(addr) EC SECTION(".text.f3." #addr)

inline void * operator new(size_t, SpriteAnimationClip * clip)
{
    return clip;
}

static inline SpriteKeyframe const & GetKeyframe(SpriteKeyframe const * keyframes, u32 index)
{
    u32 offset = index * sizeof(SpriteKeyframe);
    return *reinterpret_cast<SpriteKeyframe const *>(reinterpret_cast<u8 const *>(keyframes) + offset);
}

struct SpriteAnimationClipHeader
{
    u16 length;
    u16 first_keyframe;
};

struct SpriteFrameHeader
{
    u16 object_count;
    u16 first_object;
    u16 tile_count;
    u16 first_tile;
    u16 palette_count;
    u16 first_palette;
    u16 extra_count;
    u16 first_extra;
};

struct SpriteFrameObject
{
    u32 attributes[2];
};

struct SpriteFrameExtra
{
    u32 words[2];
};

struct SpriteAnimationData
{
    u32 const * vtable;
    SpriteAnimationClipHeader const * clips;
    SpriteFrameHeader const * frames;
    SpriteFrameObject const * objects;
    u8 const (*tiles)[32];
    u16 const (*palettes)[16];
    SpriteFrameExtra const * extras;
    SpriteKeyframe const * keyframes;
    u16 clip_count;
    u16 frame_count;
    u16 object_count;
    u16 tile_count;
    u16 palette_count;
    u16 extra_count;
    u16 keyframe_count;
};

EC u32 const gSpriteAnimationDataVTable[];

ANIMATION_CODE(0805E6CC) SpriteAnimationData * InitSpriteAnimationData(SpriteAnimationData * self, void const * data)
{
    self->vtable = gSpriteAnimationDataVTable;
    if (data != nullptr)
    {
        // The packed sprite data stores a four-byte count header before each table.
        u16 const * count = static_cast<u16 const *>(data);
        self->clip_count = *count;
        self->clips = reinterpret_cast<SpriteAnimationClipHeader const *>(count + 2);
        count = reinterpret_cast<u16 const *>(self->clips + self->clip_count);
        self->frame_count = *count;
        self->frames = reinterpret_cast<SpriteFrameHeader const *>(count + 2);
        count = reinterpret_cast<u16 const *>(self->frames + self->frame_count);
        self->object_count = *count;
        self->objects = reinterpret_cast<SpriteFrameObject const *>(count + 2);
        count = reinterpret_cast<u16 const *>(self->objects + self->object_count);
        self->tile_count = *count;
        self->tiles = reinterpret_cast<u8 const (*)[32]>(count + 2);
        count = reinterpret_cast<u16 const *>(self->tiles + self->tile_count);
        self->palette_count = *count;
        self->palettes = reinterpret_cast<u16 const (*)[16]>(count + 2);
        count = reinterpret_cast<u16 const *>(self->palettes + self->palette_count);
        self->extra_count = *count;
        self->extras = reinterpret_cast<SpriteFrameExtra const *>(count + 2);
        count = reinterpret_cast<u16 const *>(self->extras + self->extra_count);
        self->keyframe_count = *count;
        self->keyframes = reinterpret_cast<SpriteKeyframe const *>(count + 2);
    }
    else
    {
        self->clip_count = 0;
        self->clips = nullptr;
        self->frame_count = 0;
        self->frames = nullptr;
        self->object_count = 0;
        self->objects = nullptr;
        self->tile_count = 0;
        self->tiles = nullptr;
        self->palette_count = 0;
        self->palettes = nullptr;
        self->extra_count = 0;
        self->extras = nullptr;
        self->keyframe_count = 0;
        self->keyframes = nullptr;
    }
    return self;
}

ANIMATION_CODE(0805E760) SpriteAnimationClip GetSpriteAnimationDataClip(SpriteAnimationData const * self, u32 clip_id)
{
    if (clip_id < self->clip_count)
    {
        SpriteAnimationClipHeader const & clip = self->clips[clip_id];
        return SpriteAnimationClip(self->keyframes + clip.first_keyframe, clip.length);
    }
    return SpriteAnimationClip();
}

ANIMATION_CODE(0805E81C) u32 GetSpriteAnimationDataFrameCount(SpriteAnimationData const * self)
{
    return self->frame_count;
}

ANIMATION_CODE(0805E820) u32 GetSpriteAnimationDataClipCount(SpriteAnimationData const * self)
{
    return self->clip_count;
}

ANIMATION_CODE(0805E824) SpriteAnimator * InitSpriteAnimator(
    SpriteAnimator * self, SpriteAnimationSource const * source, u32 clip_id, int speed_q8)
{
    self->source = source;
    new (&self->clip) SpriteAnimationClip(source->GetClip(clip_id));
    self->keyframe = 0;
    self->remaining_q8 = self->clip.keyframes[0].duration << 8;
    self->speed_q8 = speed_q8;
    self->refresh_frame = true;
    return self;
}

ANIMATION_CODE(0805E850) void BindSpriteAnimator(SpriteAnimator * self, SpriteAnimationSource const * source, u32 clip_id)
{
    self->source = source;
    SetSpriteAnimatorClip(self, clip_id);
}

ANIMATION_CODE(0805E860) void SetSpriteAnimatorClip(SpriteAnimator * self, u32 clip_id)
{
    self->clip = self->source->GetClip(clip_id);
    self->keyframe = 0;
    self->remaining_q8 = self->clip.keyframes[0].duration << 8;
    self->refresh_frame = true;
}

ANIMATION_CODE(0805E894) bool WillSpriteAnimationLoop(SpriteAnimator const * self)
{
    int speed = self->speed_q8;
    int remaining = self->remaining_q8;
    if (speed != 0 && remaining != 0)
    {
        remaining -= speed < 0 ? -speed : speed;
        if (remaining <= 0)
        {
            u32 keyframe = self->keyframe;
            u32 length = self->clip.GetLength();
            SpriteKeyframe const * keyframes = self->clip.keyframes;
            do
            {
                if (speed > 0)
                {
                    if (++keyframe >= length)
                        return true;
                }
                else
                {
                    if (keyframe == 0)
                        return true;
                    --keyframe;
                }

                int duration = GetKeyframe(keyframes, keyframe).duration;
                if (duration == 0)
                    break;
                remaining += duration << 8;
            } while (remaining <= 0);
        }
    }
    return false;
}

ANIMATION_CODE(0805E8F0) u32 AdvanceSpriteAnimator(SpriteAnimator * self)
{
    u32 changes = 0;
    if (self->refresh_frame)
    {
        changes |= SpriteAnimator::FRAME_CHANGED;
        self->refresh_frame = false;
    }

    int speed = self->speed_q8;
    int remaining = self->remaining_q8;
    if (speed != 0 && remaining != 0)
    {
        remaining -= speed < 0 ? -speed : speed;

        if (remaining <= 0)
        {
            u32 keyframe = self->keyframe;
            SpriteAnimationClip const & clip = self->clip;
            u32 previous_offset = keyframe * sizeof(SpriteKeyframe);
            u16 previous_frame = reinterpret_cast<SpriteKeyframe const *>(
                previous_offset + reinterpret_cast<u8 const *>(clip.keyframes))->frame;
            changes |= SpriteAnimator::KEYFRAME_CHANGED;
            SpriteKeyframe const * keyframes;
            u32 length;
            {
                u32 count = 0;
                keyframes = clip.keyframes;
                if (keyframes != nullptr)
                    count = clip.length;
                length = count;
            }

            while (true)
            {
                if (speed > 0)
                {
                    ++keyframe;
                    if (keyframe >= length)
                    {
                        keyframe = 0;
                        changes |= SpriteAnimator::LOOPED;
                    }
                }
                else
                {
                    if (keyframe == 0)
                    {
                        keyframe = length;
                        changes |= SpriteAnimator::LOOPED;
                    }
                    --keyframe;
                }

                int duration = GetKeyframe(keyframes, keyframe).duration;
                // A zero-duration keyframe holds until a new clip is selected.
                if (duration != 0)
                {
                    remaining += duration << 8;
                    if (remaining > 0)
                        goto finish;
                }
                else
                {
                    remaining = 0;
                    goto finish;
                }
            }

        finish:
            if (GetKeyframe(keyframes, keyframe).frame != previous_frame)
                changes |= SpriteAnimator::FRAME_CHANGED;
            self->keyframe = keyframe;
        }
        self->remaining_q8 = remaining;
    }
    return changes;
}

EC SpriteAnimationData * func_0805E6CC(SpriteAnimationData *, void const *) ALIAS(InitSpriteAnimationData);
EC SpriteAnimationClip func_0805E760(SpriteAnimationData const *, u32) ALIAS(GetSpriteAnimationDataClip);
EC SpriteAnimator * func_0805E824(SpriteAnimator *, SpriteAnimationSource const *, u32, int) ALIAS(InitSpriteAnimator);
EC void func_0805E850(SpriteAnimator *, SpriteAnimationSource const *, u32) ALIAS(BindSpriteAnimator);
EC void func_0805E860(SpriteAnimator *, u32) ALIAS(SetSpriteAnimatorClip);
EC bool func_0805E894(SpriteAnimator const *) ALIAS(WillSpriteAnimationLoop);
EC u32 func_0805E8F0(SpriteAnimator *) ALIAS(AdvanceSpriteAnimator);
