// Chicken Festival chicken sprite ownership and placement initialization.
#include "prelude.h"
#include "sprite_animation.hh"
#include "smart_ptr.hh"
#include <new>

struct FestivalSpritePlacement
{
    u16 x, y, palette, tile, priority;
    u32 unknown_0C;
    bool flipX, flipY;
    u32 unknown_14;
    bool visible;

    FestivalSpritePlacement()
        : x(0), y(0), palette(0), tile(0), priority(0), unknown_0C(0),
          flipX(false), flipY(false), unknown_14(0), visible(false) {}
};

struct FestivalGraphicsChicken
{
    u32 action, previousAction, facing, previousFacing;
    u8 unknown_10[0x30];
};

struct FestivalGraphicsScene
{
    u8 unknown_000[0x6c];
    FestivalGraphicsChicken chickens[2];
    u8 unknown_0EC[0x690];
    SpriteAnimationSource const * source;
    u8 unknown_780[0x24];
    SmartPtr<SpriteAnimator> sprites[2];
    SmartPtr<FestivalSpritePlacement> placements[2];
};

EC u16 GetFestivalChickenAnimation(u8 *, u32, u32, u8);

EC SECTION(".text.g3_080B8DF4") void CreateFestivalChickenSprites(FestivalGraphicsScene * state, u8 chicken)
{
    state->sprites[chicken] = InitSpriteAnimator((SpriteAnimator *) operator new(sizeof(SpriteAnimator)), state->source, 0, 0x100);
    state->placements[chicken] = new FestivalSpritePlacement;
    SpriteAnimator * sprite = state->sprites[chicken].Get();
    u16 animation = GetFestivalChickenAnimation((u8 *) state, state->chickens[chicken].action, state->chickens[chicken].facing, chicken);
    SetSpriteAnimatorClip(sprite, animation);
    state->sprites[chicken]->speed_q8 = 0x100;
}
