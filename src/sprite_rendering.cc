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
