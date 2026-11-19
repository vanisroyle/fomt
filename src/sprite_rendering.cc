#include "prelude.h"

struct SpriteAffineMatrix
{
    i16 pa;
    i16 pb;
    i16 pc;
    i16 pd;
};

EC SECTION(".text.f3.0805EC24") void AdjustAffineSpriteOrigin(
    int * x, int * y, u32 width, u32 height, SpriteAffineMatrix const * matrix)
{
    const int q8_one = 256;
    const int matrix_scale_q8 = 272;
    int center_x = (*x + static_cast<int>(width >> 1)) * q8_one;
    int center_y = (*y + static_cast<int>(height >> 1)) * q8_one;
    i16 pa = matrix->pa * matrix_scale_q8 / q8_one;
    i16 pb = matrix->pb * matrix_scale_q8 / q8_one;
    i16 pc = matrix->pc * matrix_scale_q8 / q8_one;
    i16 pd = matrix->pd * matrix_scale_q8 / q8_one;
    int determinant = (pa * pd - pb * pc) / q8_one;
    if (determinant != 0)
    {
        // OAM uses a screen-to-texture matrix; invert it to move the sprite center.
        int transformed_x = pd * center_x - pb * center_y;
        int transformed_y = pa * center_y - pc * center_x;
        int adjusted_x, adjusted_y;
        if (determinant == q8_one)
        {
            adjusted_x = transformed_x / q8_one;
            adjusted_y = transformed_y / q8_one;
        }
        else if (determinant == -q8_one)
        {
            adjusted_x = transformed_x / q8_one;
            adjusted_x = -adjusted_x;
            adjusted_y = transformed_y / q8_one;
            adjusted_y = -adjusted_y;
        }
        else
        {
            adjusted_x = transformed_x / determinant;
            adjusted_y = transformed_y / determinant;
        }
        *x += (adjusted_x - center_x) >> 8;
        *y += (adjusted_y - center_y) >> 8;
    }
}

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

EC SECTION(".text.f3.0805E99C") void DrawSpriteFrameObjects(
    SpriteOamBuffer * oam, SpriteAttributes const * objects, u32 count, SpriteFrameDrawOptions const * options)
{
    const u32 shape_square = 0;
    const u32 shape_wide = 1;
    const u32 affine_double_size = 3;
    const int screen_width = 240;
    const int screen_height = 160;
    const u32 flip_horizontal = 1;
    const u32 flip_vertical = 2;
    const u32 oam_capacity = 128;

    SpriteFrameDrawOptions const * config = options;
    SpriteAttributes const * cursor = objects;
    SpriteAttributes const * end = cursor + count;
    while (cursor != end)
    {
        SpriteAttributes sprite = *cursor;
        // These register constraints and bit-pack instructions match agbcc's original Thumb output.
        u32 shape_bits = sprite.bits.shape;
        asm ("" : "+r" (shape_bits));
        u32 shape = shape_bits;
        u32 size = sprite.bits.size;
        int width, height;
        if (shape == shape_square)
        {
            height = 8 << size;
            width = height;
        }
        else
        {
            u32 long_side = 8 << (((size + 1) >> 1) + 1);
            u32 short_side = 8;
            if (size != 1)
                short_side = long_side >> 1;
            if (shape != shape_wide)
            {
                height = long_side;
                width = short_side;
            }
            else
            {
                height = short_side;
                width = long_side;
            }
        }

        register u32 x_shifted asm("r0") = sprite.words[0] << 7;
        register int sprite_x asm("r5") = static_cast<i32>(x_shifted) >> 23;
        int x = sprite_x;
        int y = sprite.bits.y;
        register u32 mode asm("r2") = config->affine_mode;
        register u32 affine_mode asm("r8") = mode;
        if (mode == 0)
        {
            register u32 flip asm("r2") = config->flip;
            u32 flip_bit = flip_horizontal;
            if (flip_bit & flip)
            {
                register int mirrored_x asm("r0") = sprite_x + width;
                x = -mirrored_x;
                register u32 toggled asm("r1") = sprite.bits.horizontal_flip;
                toggled ^= flip_bit;
                toggled &= flip_bit;
                toggled <<= 28;
                register u32 preserved asm("r0") = ~(1u << 28);
                preserved &= sprite.words[0];
                preserved |= toggled;
                sprite.words[0] = preserved;
            }
            if (flip & flip_vertical)
            {
                y = -(y + height);
                u32 toggled = sprite.bits.vertical_flip;
                toggled ^= flip_bit;
                toggled &= flip_bit;
                toggled <<= 29;
                u32 preserved = ~(1u << 29);
                preserved &= sprite.words[0];
                preserved |= toggled;
                sprite.words[0] = preserved;
            }
        }
        else
        {
            // Affine objects use the flip bits as the high bits of the matrix index.
            u32 affine_index = config->affine_index;
            sprite.bits.affine_index_low = affine_index & 7;
            sprite.bits.horizontal_flip = (affine_index & 8) >> 3;
            sprite.bits.vertical_flip = (affine_index & 16) >> 4;
            SpriteAttributes const * matrix_entries = oam->sprites + (affine_index & 31) * 4;
            SpriteAffineMatrix matrix;
            matrix.pa = matrix_entries[0].bits.affine_parameter;
            matrix.pb = matrix_entries[1].bits.affine_parameter;
            matrix.pc = matrix_entries[2].bits.affine_parameter;
            matrix.pd = matrix_entries[3].bits.affine_parameter;
            SpriteAffineMatrix transform = matrix;
            AdjustAffineSpriteOrigin(&x, &y, width, height, &transform);
            if (affine_mode == affine_double_size)
            {
                x -= width / 2;
                y -= height / 2;
            }
        }

        x += config->offset_x;
        y += config->offset_y;
        if (affine_mode != 0 ||
            (x + width > 0 && x < screen_width && y + height > 0 && y < screen_height))
        {
            u32 y_field = static_cast<u8>(y);
            register u32 y_mask asm("r2") = ~0xffu;
            u32 packed = sprite.words[0];
            packed = (packed & y_mask) | y_field;
            u32 mode_raw = affine_mode;
            u32 mode_field;
            asm ("mov %0, #3\n\tand %1, %0\n\tlsl %0, %1, #8" : "=&l" (mode_field), "+l" (mode_raw) : : "cc");
            u32 mode_field_mask = ~(3u << 8);
            packed = (packed & mode_field_mask) | mode_field;
            register u32 object_mode asm("r0") = config->object_mode;
            object_mode &= 3;
            object_mode <<= 10;
            u32 object_mode_mask = ~(3u << 10);
            packed = (packed & object_mode_mask) | object_mode;
            u32 mosaic = config->mosaic;
            mosaic &= 1;
            mosaic <<= 12;
            u32 mosaic_mask = ~(1u << 12);
            packed = (packed & mosaic_mask) | mosaic;
            register u32 x_field asm("r2") = (static_cast<u32>(x) & 0x1ff) << 16;
            u32 x_mask = ~(0x1ffu << 16);
            packed = (packed & x_mask) | x_field;
            sprite.words[0] = packed;
            u32 tile_attrs = sprite.words[1];
            register u32 tile_index asm("r0") = (tile_attrs << 22) >> 22;
            register u32 tile_base asm("r2") = config->tile_base;
            asm ("" : "+r" (tile_base));
            tile_index = ((tile_index + tile_base) << 22) >> 22;
            u32 packed_tile_attrs = ~0x3ffu;
            packed_tile_attrs = (packed_tile_attrs & tile_attrs) | tile_index;
            register u32 priority asm("r0") = config->priority;
            register u32 priority_bits asm("r3") = 3;
            asm ("and %0, %1\n\tlsl %0, %0, #10" : "+l" (priority) : "l" (priority_bits) : "cc");
            u32 priority_mask = ~(3u << 10);
            packed_tile_attrs = (packed_tile_attrs & priority_mask) | priority;
            u32 palette = (packed_tile_attrs << 16) >> 28;
            u32 palette_base = config->palette_base;
            palette = static_cast<u16>(palette + palette_base);
            palette &= 15;
            palette <<= 12;
            u32 palette_mask = ~(15u << 12);
            palette_mask &= packed_tile_attrs;
            palette_mask |= palette;
            sprite.words[1] = palette_mask;

            SpriteOamBuffer * buffer = oam;
            u32 index = buffer->sprite_count;
            if (index < oam_capacity)
            {
                SpriteAttributes * dest = buffer->sprites + index;
                // The fourth halfword belongs to the OAM affine matrix, not this sprite.
                dest->words[0] = sprite.words[0];
                reinterpret_cast<u16 *>(dest)[2] = reinterpret_cast<u16 const *>(&sprite)[2];
                oam->sprite_count = index + 1;
            }
        }
        ++cursor;
    }
}

EC void func_0805E99C(SpriteOamBuffer *, SpriteAttributes const *, u32, SpriteFrameDrawOptions const *)
    ALIAS(DrawSpriteFrameObjects);
