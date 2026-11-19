// Harvest Sprite harvesting minigame scene and owned rendering resources.
#include "prelude.h"
#include "sprite_animation.hh"
#include "smart_ptr.hh"
#include <new>

struct HarvestingHardwareContext
{
    void * state;
    HarvestingHardwareContext() asm("func_08008444");
    virtual ~HarvestingHardwareContext();
};

struct HarvestingTextTiles
{
    u32 unknown_00;
    u8 pixels[0x100];
    HarvestingTextTiles() {}
};

struct HarvestingTilePool
{
    u32 storage;
    HarvestingTilePool() asm("TilePool_Initialize");
    ~HarvestingTilePool();
};

struct HarvestingResourcePool
{
    u32 storage;
    HarvestingResourcePool() asm("ResourcePool_Initialize");
    ~HarvestingResourcePool();
};

EC void TilePool_Destroy(HarvestingTilePool *, u32);
EC void ResourcePool_Destroy(HarvestingResourcePool *, u32);

inline HarvestingTilePool::~HarvestingTilePool() { TilePool_Destroy(this, 2); }
inline HarvestingResourcePool::~HarvestingResourcePool() { ResourcePool_Destroy(this, 2); }

struct HarvestingMessageWindow
{
    void * window;
    HarvestingMessageWindow(u32, u32, u32, u32,
        HarvestingTilePool const & = HarvestingTilePool(),
        HarvestingResourcePool const & = HarvestingResourcePool()) asm("CreateMessageWindow");
};

struct HarvestingAnimationData : SpriteAnimationSource
{
    u8 storage[0x2c];
    HarvestingAnimationData(void const *) asm("func_0805E6CC");
    virtual SpriteAnimationClip GetClip(u32) const;
};

struct HarvestingTaskNode
{
    void * previous;
    void * next;
    void const * vtable;
    HarvestingTaskNode() {}
};

struct HarvestingDisplayFade
{
    void * state;
};

EC u8 gDisplayFadeTaskVtable[], vtable_unk_080E5B70[], vtable_unk_080E5B60[];
EC void * HardwareContext_GetDisplayRegisters(void *);
EC void DisplayFade_Initialize(HarvestingDisplayFade *, void *);

template <u8 * VTable>
struct HarvestingSceneTask : HarvestingTaskNode
{
    HarvestingSceneTask()
    {
        previous = nullptr;
        next = nullptr;
        vtable = VTable;
    }
};

struct HarvestingFadeTask
{
    HarvestingTaskNode node;
    HarvestingDisplayFade fade;
    HarvestingFadeTask(void * registers)
    {
        node.previous = nullptr;
        node.next = nullptr;
        node.vtable = gDisplayFadeTaskVtable;
        DisplayFade_Initialize(&fade, registers);
    }
};

struct HarvestingPlacement
{
    u16 x, y, palette, tile, priority;
    u32 unknown_0C;
    bool flipX, flipY;
    u32 unknown_14;
    bool visible;
};

struct HarvestingSprite
{
    SmartPtr<SpriteAnimator> animator;
    SmartPtr<HarvestingPlacement> placement;
    SpriteFrameView frame;
};

EC void AudioPlayer_Initialize(void *, u32);

struct HarvestingAudioPlayer
{
    void * player;
    HarvestingAudioPlayer(u32 id) { AudioPlayer_Initialize(this, id); }
};

struct HarvestingTileOwner;
EC void func_08007874(HarvestingTileOwner *);

struct HarvestingTileOwner
{
    u32 heap;
    u32 allocation;
    HarvestingTileOwner()
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

struct SpriteHarvestingScene : HarvestingHardwareContext
{
    HarvestingTextTiles scoreText;
    HarvestingTextTiles textBuffers[3];
    SmartPtr<HarvestingMessageWindow> message;
    SmartPtr<HarvestingAnimationData> animation0, animation1, animation2, animation3;
    SmartPtr<HarvestingAnimationData> animation4, animation5, animation6, animation7;
    SmartPtr<HarvestingFadeTask> fade;
    u8 textObject[0x20];
    HarvestingSprite sprite0, sprite1, sprite2, sprite3, sprite4, sprite5, sprite6;
    HarvestingAudioPlayer audio0, audio1, audio2, audio3;
    HarvestingSceneTask<vtable_unk_080E5B70> task0;
    HarvestingSceneTask<vtable_unk_080E5B60> task1;
    u8 unknown_5A0[0x10];
    HarvestingTileOwner tiles0, tiles1, tiles2, tiles3, tiles4, tiles5, tiles6, tiles7, tiles8;
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
    : message(new HarvestingMessageWindow(0, 24, 0, 0)),
      animation0(new HarvestingAnimationData(gUnk_0873E5B0)), animation1(new HarvestingAnimationData(gUnk_0873DE44)),
      animation2(new HarvestingAnimationData(gUnk_086678A0)), animation3(new HarvestingAnimationData(gUnk_087405A0)),
      animation4(new HarvestingAnimationData(gUnk_0858BA28)), animation5(new HarvestingAnimationData(gUnk_0871D51C)),
      animation6(new HarvestingAnimationData(gUnk_0872BE64)), animation7(new HarvestingAnimationData(gUnk_0873D6D8)),
      fade(new HarvestingFadeTask(HardwareContext_GetDisplayRegisters(this))),
      audio0(0), audio1(1), audio2(2), audio3(3),
      completed(0), settingValue(settings.value), mode(settings.mode)
{
}
