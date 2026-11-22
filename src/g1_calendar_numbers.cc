#include "g1_map_resources.hh"
#include "sprite_rendering.hh"
#include <string.h>

struct CalendarGlyphDrawOptions : SpriteFrameDrawOptions
{
    CalendarGlyphDrawOptions()
    {
        offset_x = 0;
        offset_y = 0;
        palette_base = 0;
        tile_base = 0;
        priority = 0;
        object_mode = 0;
        flip = 0;
        affine_index = 0;
        affine_mode = 0;
        mosaic = 0;
    }
};

struct G1CalendarScene
{
    STRUCT_PAD(0x000, 0x0B0);
    SpriteAnimator digit_animator;
    CalendarGlyphDrawOptions digit_render;
    STRUCT_PAD(0x0E0, 0x140);
    u16 digit_tiles[22];
    STRUCT_PAD(0x16C, 0x174);
    G1MapTileOwner digit_owner;
    G1MapTileOwner digit_palettes[3];
};

struct CalendarGlyphFrame : SpriteFrameView
{
    SpriteFrameObject const * GetObjects() const { return objects.data; }
    u32 GetCount() const { return objects.data ? objects.length : 0; }
    CalendarGlyphFrame() {}
    CalendarGlyphFrame(CalendarGlyphFrame const & other)
    {
        memcpy(this, &other, sizeof(*this));
    }
};

struct CalendarGlyphFrameSource : SpriteAnimationSource
{
    virtual CalendarGlyphFrame GetFrame(u32) const;
};

EC u32 func_08007D4C(G1MapTileOwner *, u32);
EC u32 ResourceHandle_GetIndex(G1MapTileOwner *, u32);
EC SpriteOamBuffer * HardwareContext_GetOamBuffer(G1CalendarScene *);

static inline SpriteKeyframe const & GetCalendarKeyframe(SpriteAnimationClip const & clip, u32 index)
{
    return clip.keyframes[index];
}

static inline CalendarGlyphFrame GetCalendarGlyphFrame(G1CalendarScene const * self, SpriteAnimator const & animator)
{
    CalendarGlyphFrameSource const * source = reinterpret_cast<CalendarGlyphFrameSource const *>(animator.source);
    return source->GetFrame(GetCalendarKeyframe(self->digit_animator.clip, animator.keyframe).frame);
}

static inline u32 CalendarResourceHandle(G1MapTileOwner owner)
{
    return owner.handle;
}

EC void DrawCalendarGlyph(G1CalendarScene *, int, int, int, int);

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
            DrawCalendarGlyph(self, x, y, digit, palette);
            started = 1;
        }
        x += 16;
        number -= digit * divisor;
        divisor /= 10;
    }
}

EC SECTION(".text.g1_080A3618") void DrawCalendarGlyph(G1CalendarScene * self, int x, int y, int glyph, int color)
{
    CalendarGlyphDrawOptions render;
    CalendarGlyphFrame frame;
    {
        SetSpriteAnimatorClip(&self->digit_animator, glyph);
        frame = GetCalendarGlyphFrame(self, self->digit_animator);
        render = self->digit_render;
    }
    render.offset_x = x;
    render.offset_y = y;
    render.tile_base = func_08007D4C(&self->digit_owner, CalendarResourceHandle(self->digit_owner)) + self->digit_tiles[glyph];
    switch (color)
    {
        case 0:
            render.palette_base = ResourceHandle_GetIndex(&self->digit_palettes[0], CalendarResourceHandle(self->digit_palettes[0]));
            break;
        case 1:
            render.palette_base = ResourceHandle_GetIndex(&self->digit_palettes[1], CalendarResourceHandle(self->digit_palettes[1]));
            break;
        case 2:
            render.palette_base = ResourceHandle_GetIndex(&self->digit_palettes[2], CalendarResourceHandle(self->digit_palettes[2]));
            break;
    }
    render.priority = 1;
    render.mosaic = true;
    SpriteOamBuffer * oam = HardwareContext_GetOamBuffer(self);
    DrawSpriteFrameObjects(oam, reinterpret_cast<SpriteAttributes const *>(frame.GetObjects()),
        frame.GetCount(), &render);
}
