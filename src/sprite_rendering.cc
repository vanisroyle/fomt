#include "sprite_rendering.hh"

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

struct HorseRaceObjAllocation
{
    u32 allocator_context;
    u32 handle;
};

struct HardwareContext;

EC void * HardwareContext_GetOamBuffer(HardwareContext *);
EC u32 ResolveHorseRaceObjPalette(HorseRaceObjAllocation *, u32) asm("func_080074C0");

EC SECTION(".text.f3.0805C63C") void DrawHorseRaceEntrantObjects(
    HardwareContext * hardware, SpriteAttributes const * objects, u32 count,
    SpriteFrameDrawOptions const * options, HorseRaceObjAllocation * sprite_allocations)
{
    // Register constraints keep the original agbcc layout and OAM bit packing.
    struct
    {
        SpriteAttributes object;
        HardwareContext * hardware_context;
        SpriteAttributes const * end;
    } work;
    enum { SHAPE_SQUARE = 0, SHAPE_WIDE = 1, SCREEN_WIDTH = 240, SCREEN_HEIGHT = 160 };
    work.hardware_context = hardware;
    register SpriteFrameDrawOptions const * config asm("r8") = options;
    SpriteAttributes const * cursor = objects;
    SpriteAttributes const * limit = cursor + count;
    work.end = limit;
    if (cursor == limit)
        return;
    do
    {
        SpriteAttributes const * object = cursor;
        work.object = *object;
        u32 position_bits = work.object.words[0];
        u32 x_shifted = position_bits << 7;
        int x = static_cast<i32>(x_shifted) >> 23;
        SpriteAttributes * sprite_address = &work.object;
        int y = sprite_address->bits.y;
        register u32 shape_bits asm("r0") = (position_bits << 16) >> 30;
        asm ("" : "+r" (shape_bits));
        u32 shape = shape_bits;
        u32 size = position_bits >> 30;
        int width;
        int height;
        if (shape == SHAPE_SQUARE)
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
            if (shape != SHAPE_WIDE)
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

        u32 flip = config->flip;
        register u32 one_low asm("r1") = 1;
        asm ("" : "+r" (one_low));
        register u32 flip_bit asm("r10") = one_low;
        u32 horizontal = flip_bit;
        asm ("and %0, %1" : "+l" (horizontal) : "l" (flip) : "cc");
        if (horizontal)
        {
            int right = x + width;
            x = -right;
            u32 packed_flip = work.object.words[0];
            u32 toggled = (packed_flip << 3) >> 31;
            u32 one = flip_bit;
            asm ("eor %0, %1\n\tand %0, %1" : "+l" (toggled) : "l" (one) : "cc");
            toggled <<= 28;
            u32 preserved = ~(1u << 28);
            preserved &= packed_flip;
            preserved |= toggled;
            work.object.words[0] = preserved;
        }
        u32 vertical = 2;
        asm ("and %0, %1" : "+l" (vertical) : "l" (flip) : "cc");
        if (vertical)
        {
            int bottom = y + height;
            y = -bottom;
            u32 packed_flip = work.object.words[0];
            u32 toggled = (packed_flip << 2) >> 31;
            u32 one = flip_bit;
            asm ("eor %0, %1\n\tand %0, %1" : "+l" (toggled) : "l" (one) : "cc");
            toggled <<= 29;
            u32 preserved = ~(1u << 29);
            preserved &= packed_flip;
            preserved |= toggled;
            work.object.words[0] = preserved;
        }

        SpriteFrameDrawOptions const * position = config;
        u32 x_offset = offsetof(SpriteFrameDrawOptions, offset_x);
        register int offset_x asm("r0");
        asm ("ldrsh %0, [%1, %2]" : "=l" (offset_x) : "l" (position), "l" (x_offset), "m" (position->offset_x));
        x += offset_x;
        u32 y_offset = offsetof(SpriteFrameDrawOptions, offset_y);
        register int offset_y asm("r0");
        asm ("ldrsh %0, [%1, %2]" : "=l" (offset_y) : "l" (position), "l" (y_offset), "m" (position->offset_y));
        y += offset_y;
        int right_edge = x + width;
        if (right_edge > 0 && x < SCREEN_WIDTH)
        {
            int bottom_edge = y + height;
            if (bottom_edge <= 0 || y >= SCREEN_HEIGHT)
                goto next_object;
            register u32 coordinate_mask asm("r0") = 0x1ff;
            asm ("and %0, %1" : "+l" (x) : "l" (coordinate_mask) : "cc");
            u32 shifted_x = static_cast<u32>(x) << 16;
            u32 x_mask = ~(0x1ffu << 16);
            register u32 x_preserve asm("r1") = x_mask;
            register u32 packed asm("r0") = work.object.words[0];
            packed = (packed & x_preserve) | shifted_x;
            u32 y_field = static_cast<u8>(y);
            register u32 y_mask asm("r1") = ~0xffu;
            packed = (packed & y_mask) | y_field;
            work.object.words[0] = packed;

            SpriteAttributes * source = &work.object;
            u32 tile_attrs = work.object.words[1];
            u32 palette_offset = ((tile_attrs << 16) >> 28) * sizeof(*sprite_allocations);
            register HorseRaceObjAllocation * base asm("r3");
            asm ("ldr %0, %1" : "=l" (base) : "m" (sprite_allocations));
            asm ("add %0, %0, %1" : "+l" (palette_offset) : "l" (base) : "cc");
            // The first allocation belongs to the tiles; frame palette indices start after it.
            const unsigned first_palette_allocation = 1;
            HorseRaceObjAllocation * allocation =
                reinterpret_cast<HorseRaceObjAllocation *>(palette_offset) + first_palette_allocation;
            u32 palette = static_cast<u16>(ResolveHorseRaceObjPalette(allocation, allocation->handle));
            register u32 palette_bits asm("r1") = 15;
            asm ("and %0, %1" : "+l" (palette) : "l" (palette_bits) : "cc");
            palette <<= 12;
            register u32 palette_mask asm("r1") = ~(15u << 12);
            u32 final_tile_attrs = work.object.words[1];
            final_tile_attrs = (final_tile_attrs & palette_mask) | palette;
            register u32 tile asm("r0") = (final_tile_attrs << 22) >> 22;
            register SpriteFrameDrawOptions const * tile_config asm("r1") = config;
            register u32 tile_base asm("r1") = tile_config->tile_base;
            tile = ((tile + tile_base) << 22) >> 22;
            u32 tiles_and_priority = ~0x3ffu;
            asm ("and %0, %1\n\torr %0, %2" : "+l" (tiles_and_priority) : "l" (final_tile_attrs), "l" (tile) : "cc");
            register SpriteFrameDrawOptions const * priority_config asm("r2") = config;
            register u32 priority asm("r0") = priority_config->priority;
            u32 priority_bits = 3;
            asm ("and %0, %1\n\tlsl %0, %0, #10" : "+l" (priority) : "l" (priority_bits) : "cc");
            u32 priority_mask = ~(3u << 10);
            tiles_and_priority = (tiles_and_priority & priority_mask) | priority;
            work.object.words[1] = tiles_and_priority;

            register SpriteFrameDrawOptions const * mode_config asm("r1") = config;
            u32 object_mode = mode_config->object_mode;
            asm ("and %0, %1\n\tlsl %0, %0, #10" : "+l" (object_mode) : "l" (priority_bits) : "cc");
            u32 position_and_flags = work.object.words[0];
            asm ("and %0, %1\n\torr %0, %2" : "+l" (position_and_flags) : "l" (priority_mask), "l" (object_mode) : "cc");
            SpriteFrameDrawOptions const * flags_config = config;
            register u32 affine_mode asm("r2") = flags_config->affine_mode;
            register u32 affine_bits asm("r0") = 3;
            asm ("and %0, %1\n\tlsl %0, %0, #8" : "+l" (affine_mode) : "l" (affine_bits) : "cc");
            register u32 affine_mode_mask asm("r0") = ~(3u << 8);
            position_and_flags = (position_and_flags & affine_mode_mask) | affine_mode;
            register u32 mosaic asm("r2") = flags_config->mosaic;
            register u32 mosaic_bit asm("r3") = flip_bit;
            asm ("and %0, %1\n\tlsl %0, %0, #12" : "+l" (mosaic) : "l" (mosaic_bit) : "cc");
            register u32 mosaic_mask asm("r0") = ~(1u << 12);
            position_and_flags = (position_and_flags & mosaic_mask) | mosaic;
            work.object.words[0] = position_and_flags;

            SpriteOamBuffer * oam = static_cast<SpriteOamBuffer *>(HardwareContext_GetOamBuffer(work.hardware_context));
            u32 index = oam->sprite_count;
            if (index < 128)
            {
                SpriteAttributes * dest = oam->sprites + index;
                dest->words[0] = work.object.words[0];
                // The fourth halfword belongs to the affine matrix.
                reinterpret_cast<u16 *>(dest)[2] = reinterpret_cast<u16 const *>(source)[2];
                oam->sprite_count = index + 1;
            }
        }
    next_object:
        ++cursor;
        register SpriteAttributes const * end asm("r1") = work.end;
        if (cursor == end)
            break;
    } while (true);
}
