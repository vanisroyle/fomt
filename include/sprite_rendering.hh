#ifndef SPRITE_RENDERING_HH
#define SPRITE_RENDERING_HH

#include "prelude.h"

struct SpriteAttributeBits
{
    i32 y : 8;
    u32 affine_mode : 2;
    u32 object_mode : 2;
    u32 mosaic : 1;
    u32 color_mode : 1;
    u32 shape : 2;
    i32 x : 9;
    u32 affine_index_low : 3;
    u32 horizontal_flip : 1;
    u32 vertical_flip : 1;
    u32 size : 2;
    u32 tile : 10;
    u32 priority : 2;
    u32 palette : 4;
    i16 affine_parameter;
};

union SpriteAttributes
{
    SpriteAttributeBits bits;
    u32 words[2];
};

struct SpriteFrameDrawOptions
{
    i16 offset_x;
    i16 offset_y;
    u16 palette_base;
    u16 tile_base;
    u16 priority;
    u32 object_mode;
    u8 flip;
    u8 affine_index;
    u32 affine_mode;
    u8 mosaic;
};

struct SpriteOamBuffer
{
    u8 sprite_count;
    STRUCT_PAD(1, 4);
    SpriteAttributes sprites[128];
};

EC void DrawSpriteFrameObjects(SpriteOamBuffer * oam, SpriteAttributes const * objects,
    u32 count, SpriteFrameDrawOptions const * options);

#endif // SPRITE_RENDERING_HH
