// Chicken Festival and Harvest Sprite minigame scene/resource initialization.
#include "prelude.h"
#include "sprite_animation.hh"
#include "smart_ptr.hh"
#include "utility/fixed_str.hh"
#include <new>

struct MinigameHardwareContext
{
    void * state;
    MinigameHardwareContext() asm("func_08008444");
    virtual ~MinigameHardwareContext();
};

struct MinigameTextTiles
{
    u32 unknown_00;
    u8 pixels[0x100];
    MinigameTextTiles() {}
};

struct MinigameTilePool
{
    u32 storage;
    MinigameTilePool() asm("TilePool_Initialize");
    ~MinigameTilePool();
};

struct MinigameResourcePool
{
    u32 storage;
    MinigameResourcePool() asm("ResourcePool_Initialize");
    ~MinigameResourcePool();
};

EC void TilePool_Destroy(MinigameTilePool *, u32);
EC void ResourcePool_Destroy(MinigameResourcePool *, u32);

inline MinigameTilePool::~MinigameTilePool() { TilePool_Destroy(this, 2); }
inline MinigameResourcePool::~MinigameResourcePool() { ResourcePool_Destroy(this, 2); }

struct MinigameMessageWindow
{
    void * window;
    MinigameMessageWindow(u32, u32, u32, u32,
        MinigameTilePool const & = MinigameTilePool(),
        MinigameResourcePool const & = MinigameResourcePool()) asm("CreateMessageWindow");
};

struct MinigameAnimationData : SpriteAnimationSource
{
    u8 storage[0x2c];
    MinigameAnimationData(void const *) asm("func_0805E6CC");
    virtual SpriteAnimationClip GetClip(u32) const;
};

struct MinigameTaskNode
{
    void * previous;
    void * next;
    void const * vtable;
    MinigameTaskNode() {}
};

struct MinigameDisplayFade
{
    void * state;
};

EC u8 gDisplayFadeTaskVtable[], vtable_unk_080E5B70[], vtable_unk_080E5B60[];
EC void * HardwareContext_GetDisplayRegisters(void *);
EC void DisplayFade_Initialize(MinigameDisplayFade *, void *);

template <u8 * VTable>
struct MinigameSceneTask : MinigameTaskNode
{
    MinigameSceneTask()
    {
        previous = nullptr;
        next = nullptr;
        vtable = VTable;
    }
};

struct MinigameFadeTask
{
    MinigameTaskNode node;
    MinigameDisplayFade fade;
    MinigameFadeTask(void * registers)
    {
        node.previous = nullptr;
        node.next = nullptr;
        node.vtable = gDisplayFadeTaskVtable;
        DisplayFade_Initialize(&fade, registers);
    }
};

struct MinigameSpritePlacement
{
    u16 x, y, palette, tile, priority;
    u32 unknown_0C;
    bool flipX, flipY;
    u32 unknown_14;
    bool visible;
};

struct MinigameSprite
{
    SmartPtr<SpriteAnimator> animator;
    SmartPtr<MinigameSpritePlacement> placement;
    SpriteFrameView frame;
};

EC void AudioPlayer_Initialize(void *, u32);

struct MinigameAudioPlayer
{
    void * player;
    MinigameAudioPlayer(u32 id) { AudioPlayer_Initialize(this, id); }
};

struct MinigameTileOwner;
EC void func_08007874(MinigameTileOwner *);

struct MinigameTileOwner
{
    u32 heap;
    u32 allocation;
    MinigameTileOwner()
    {
        func_08007874(this);
        allocation = 0;
    }
};

struct HarvestingSettings
{
    u32 value;
    u8 mode;
};

struct SpriteHarvestingScene : MinigameHardwareContext
{
    MinigameTextTiles scoreText;
    MinigameTextTiles textBuffers[3];
    SmartPtr<MinigameMessageWindow> message;
    SmartPtr<MinigameAnimationData> animation0, animation1, animation2, animation3;
    SmartPtr<MinigameAnimationData> animation4, animation5, animation6, animation7;
    SmartPtr<MinigameFadeTask> fade;
    u8 textObject[0x20];
    MinigameSprite sprite0, sprite1, sprite2, sprite3, sprite4, sprite5, sprite6;
    MinigameAudioPlayer audio0, audio1, audio2, audio3;
    MinigameSceneTask<vtable_unk_080E5B70> task0;
    MinigameSceneTask<vtable_unk_080E5B60> task1;
    u8 unknown_5A0[0x10];
    MinigameTileOwner tiles0, tiles1, tiles2, tiles3, tiles4, tiles5, tiles6, tiles7, tiles8;
    u8 unknown_5F8[0xc];
    u32 completed;
    u8 unknown_608[0xc];
    u32 settingValue;
    u8 unknown_618[4];
    u8 mode;

    SECTION(".text.g3_080BC938") SpriteHarvestingScene(HarvestingSettings const & settings) asm("InitializeSpriteHarvestingScene");
    virtual ~SpriteHarvestingScene();
};

EC u8 gUnk_0873E5B0[], gUnk_0873DE44[], gUnk_086678A0[], gUnk_087405A0[];
EC u8 gUnk_0858BA28[], gUnk_0871D51C[], gUnk_0872BE64[], gUnk_0873D6D8[];

SpriteHarvestingScene::SpriteHarvestingScene(HarvestingSettings const & settings)
    : message(new MinigameMessageWindow(0, 24, 0, 0)),
      animation0(new MinigameAnimationData(gUnk_0873E5B0)), animation1(new MinigameAnimationData(gUnk_0873DE44)),
      animation2(new MinigameAnimationData(gUnk_086678A0)), animation3(new MinigameAnimationData(gUnk_087405A0)),
      animation4(new MinigameAnimationData(gUnk_0858BA28)), animation5(new MinigameAnimationData(gUnk_0871D51C)),
      animation6(new MinigameAnimationData(gUnk_0872BE64)), animation7(new MinigameAnimationData(gUnk_0873D6D8)),
      fade(new MinigameFadeTask(HardwareContext_GetDisplayRegisters(this))),
      audio0(0), audio1(1), audio2(2), audio3(3),
      completed(0), settingValue(settings.value), mode(settings.mode)
{
}

struct FestivalPaletteResource;
EC void ResourceHandles_Initialize(FestivalPaletteResource *);
EC void func_080073E0(FestivalPaletteResource *, u32);
EC u32 func_0800736C(FestivalPaletteResource *);
EC void SetMessageWindowPortrait(MinigameMessageWindow *, u32);

struct FestivalPaletteResource
{
    u32 owner;
    u32 handle;
    FestivalPaletteResource()
    {
        ResourceHandles_Initialize(this);
        handle = 0;
    }
    void Allocate()
    {
        func_080073E0(this, handle);
        handle = func_0800736C(this);
    }
};

struct FestivalEntrantSettings
{
    FixedStr<12> name;
    u8 unknown_10;
    u8 affection;
    FestivalEntrantSettings() asm("__22ChickenFestivalEntrant");
};

struct FestivalSceneChicken { u8 state[0x40]; };

struct ChickenFestivalScene : MinigameHardwareContext
{
    FixedStr<99> dialogue;
    FestivalSceneChicken chickens[2];
    MinigameTextTiles text[6];
    MinigameAudioPlayer audio0, audio1, audio2, audio3, audio4;
    SmartPtr<MinigameMessageWindow> message;
    FestivalPaletteResource palette0, palette1;
    FestivalPaletteResource chickenPalettes[2], overlayPalettes[8];
    SmartPtr<MinigameAnimationData> animation0, animation1, animation2, animation3, animation4;
    SmartPtr<MinigameAnimationData> animation5, animation6, animation7, animation8;
    SmartPtr<MinigameFadeTask> fade;
    SmartPtr<SpriteAnimator> chickenAnimators[2];
    SmartPtr<MinigameSpritePlacement> chickenPlacements[2];
    SmartPtr<SpriteAnimator> overlayAnimators[5];
    SmartPtr<MinigameSpritePlacement> overlayPlacements[5];
    SpriteFrameView overlayFrames[5];
    SmartPtr<SpriteAnimator> reactionAnimators[2];
    SmartPtr<MinigameSpritePlacement> reactionPlacements[2];
    SpriteFrameView reactionFrames[2];
    SmartPtr<SpriteAnimator> indicatorAnimators;
    SmartPtr<MinigameSpritePlacement> indicatorPlacements;
    SpriteFrameView indicatorFrames;
    SmartPtr<SpriteAnimator> extra0Animators;
    SmartPtr<MinigameSpritePlacement> extra0Placements;
    SpriteFrameView extra0Frames;
    SmartPtr<SpriteAnimator> extraPairAnimators[2];
    SmartPtr<MinigameSpritePlacement> extraPairPlacements[2];
    SpriteFrameView extraPairFrames[2];
    SmartPtr<SpriteAnimator> extra1Animators;
    SmartPtr<MinigameSpritePlacement> extra1Placements;
    SpriteFrameView extra1Frames;
    SmartPtr<SpriteAnimator> extra2Animators;
    SmartPtr<MinigameSpritePlacement> extra2Placements;
    SpriteFrameView extra2Frames;
    SmartPtr<SpriteAnimator> extra3Animators;
    SmartPtr<MinigameSpritePlacement> extra3Placements;
    SpriteFrameView extra3Frames;
    SmartPtr<SpriteAnimator> marker0Animator;
    SmartPtr<MinigameSpritePlacement> marker0Placements[2];
    SpriteFrameView marker0Frames[2];
    SmartPtr<SpriteAnimator> marker1Animator;
    SmartPtr<MinigameSpritePlacement> marker1Placements[2];
    SpriteFrameView marker1Frames[2];
    SmartPtr<SpriteAnimator> extra4Animators;
    SmartPtr<MinigameSpritePlacement> extra4Placements;
    SpriteFrameView extra4Frames;
    SmartPtr<SpriteAnimator> chickenMarker;
    SpriteFrameView chickenMarkerFrame;
    SmartPtr<SpriteAnimator> audienceMarker;
    SpriteFrameView audienceMarkerFrame;
    SmartPtr<MinigameSpritePlacement> audiencePlacements[10];
    u32 sceneState, difficulty;
    FestivalEntrantSettings entrant;
    u8 playerWon;
    u8 unknown_B31[0x27];
    u8 unknown_B58, unknown_B59, unknown_B5A, cheerDelay, unknown_B5C, cheerRadius, chickenSpeed;
    u8 unknown_B5F;
    u16 unknown_B60, unknown_B62, unknown_B64, unknown_B66, unknown_B68;
    u8 unknown_B6A, unknown_B6B, unknown_B6C, unknown_B6D, unknown_B6E, unknown_B6F, unknown_B70, unknown_B71;
    u16 cheerHalfAngle;
    u8 unknown_B74, unknown_B75, unknown_B76, unknown_B77;

    SECTION(".text.g3_080B3CC8") ChickenFestivalScene(FestivalEntrantSettings const & settings) asm("InitializeChickenFestivalScene");
    virtual ~ChickenFestivalScene();
};

EC u8 gUnk_08738144[], gUnk_0873AFC8[], gUnk_08731B40[], gUnk_0873CEAC[];
EC u8 gUnk_08729460[], gUnk_087409E4[];

ChickenFestivalScene::ChickenFestivalScene(FestivalEntrantSettings const & settings)
    : audio0(0), audio1(1), audio2(2), audio3(3), audio4(4),
      message(new MinigameMessageWindow(0, 15, 0, 0)),
      animation0(new MinigameAnimationData(gUnk_0858BA28)), animation1(new MinigameAnimationData(gUnk_0873DE44)),
      animation2(new MinigameAnimationData(gUnk_0871D51C)), animation3(new MinigameAnimationData(gUnk_08738144)),
      animation4(new MinigameAnimationData(gUnk_0873AFC8)), animation5(new MinigameAnimationData(gUnk_08731B40)),
      animation6(new MinigameAnimationData(gUnk_0873CEAC)), animation7(new MinigameAnimationData(gUnk_08729460)),
      animation8(new MinigameAnimationData(gUnk_087409E4)),
      fade(new MinigameFadeTask(HardwareContext_GetDisplayRegisters(this))),
      playerWon(0), unknown_B58(0), unknown_B59(90), unknown_B5A(25), cheerDelay(25),
      unknown_B5C(2), cheerRadius(16), chickenSpeed(25), unknown_B60(25), unknown_B62(25), unknown_B64(34),
      unknown_B66(25), unknown_B68(36), unknown_B6A(70), unknown_B6B(20), unknown_B6C(60), unknown_B6D(115),
      unknown_B6E(60), unknown_B6F(115), unknown_B70(60), unknown_B71(115), cheerHalfAngle(45),
      unknown_B74(180), unknown_B75(120), unknown_B76(180), unknown_B77(180)
{
    entrant.name = settings.name;
    entrant.unknown_10 = settings.unknown_10;
    entrant.affection = settings.affection;
    cheerHalfAngle = (cheerHalfAngle << 16) / 360;
    palette0.Allocate();
    palette1.Allocate();
    for (i32 i = 0; i < 2; ++i)
        chickenPalettes[i].Allocate();
    for (i32 i = 0; i < 8; ++i)
        overlayPalettes[i].Allocate();
    SetMessageWindowPortrait(message.Get(), 0);
}
