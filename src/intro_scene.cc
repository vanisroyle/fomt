#include "intro_scene.hh"
#include "sprite_animation.hh"

EC void IntroScene_InitializeHardwareContext(void * self) __asm__("func_08008444");
EC void AudioPlayer_Initialize(void * self, u32 channel);
EC void Unpack(void const * source, void * destination);
EC SpriteAnimationData * InitSpriteAnimationData(SpriteAnimationData * self, void const * data);
EC void IntroScene_InitializeTileOwner(void * self) __asm__("func_08007874");
EC void IntroScene_ReserveTileRange(void * self, u32 first, u32 count) __asm__("func_08007A28");
EC u32 IntroScene_AllocateTileBlock(void * self, u32 count) __asm__("func_08007B54");
EC void ResourceHandles_Initialize(void * self);
EC void ResourceHandles_ReserveRange(void * self, u32 first, u32 count);
EC u32 IntroScene_AllocatePalette(void * self) __asm__("func_0800736C");

extern char gIntroSceneTaskVtable[] __asm__("vtable_unk_080E5A0C");
extern char gIntroScenePackedSpriteData[] __asm__("gUnk_08747A74");
extern char gIntroSceneOverlaySpriteData[] __asm__("gUnk_0874E530");
extern char gIntroSceneSharedSpriteData[] __asm__("gUnk_0858BA28");

struct IntroSceneAnimationStorage
{
    u32 words[12];
    SpriteAnimationData * Data() { return reinterpret_cast<SpriteAnimationData *>(this); }
    SpriteAnimationSource const * Source() const { return reinterpret_cast<SpriteAnimationSource const *>(this); }
};

struct IntroSceneAnimatedActor
{
    SpriteAnimator animator;
    STRUCT_PAD(0x14, 0x18);
    i32 xQ16;
    i32 yQ16;

    void Initialize(IntroSceneAnimationStorage const & source, u32 animation)
    {
        // Q8 animation speed: 0x100 is normal playback.
        InitSpriteAnimator(&animator, source.Source(), animation, 0x100);
        xQ16 = 0;
        yQ16 = 0;
    }
};

struct IntroSceneResourceSlot
{
    u32 owner;
    u32 allocation;

    void AllocateTiles(u32 count)
    {
        IntroScene_InitializeTileOwner(this);
        allocation = IntroScene_AllocateTileBlock(this, count);
    }
    void AllocatePalette()
    {
        ResourceHandles_Initialize(this);
        allocation = IntroScene_AllocatePalette(this);
    }
};

struct IntroSceneTileReservation
{
    u32 owner;
    void Reserve(u32 first, u32 count)
    {
        IntroScene_ReserveTileRange(this, first, count);
    }
};

struct IntroSceneConstructionView
{
    u32 renderContext;
    void * vtable;
    u32 musicPlayer;
    u32 soundPlayer;
    u8 unpackedSprites[0x6A00];
    IntroSceneAnimationStorage sceneAnimations;
    IntroSceneAnimationStorage overlayAnimations;
    IntroSceneAnimationStorage sharedAnimations;
    IntroSceneAnimatedActor actors[14];
    STRUCT_PAD(0x6C60, 0x6C6D);
    u8 transitionRequested;
    u8 transitionChoice;
    STRUCT_PAD(0x6C6F, 0x6C70);
    IntroSceneTileReservation tileReservations;
    u32 paletteReservations;
    IntroSceneResourceSlot tileGroup0;
    IntroSceneResourceSlot palette0;
    IntroSceneResourceSlot tileGroup1;
    IntroSceneResourceSlot palette1;
    IntroSceneResourceSlot tileGroup2;
    IntroSceneResourceSlot palette2;
    IntroSceneResourceSlot tileGroup3;
    IntroSceneResourceSlot tileGroup4;
    IntroSceneResourceSlot palette3;
    IntroSceneResourceSlot tileGroup5;
    IntroSceneResourceSlot palette4;
    IntroSceneResourceSlot spriteTiles0;
    IntroSceneResourceSlot spriteTiles1;
    IntroSceneResourceSlot spriteTiles2;
    IntroSceneResourceSlot spritePalette;
    IntroSceneResourceSlot tileGroup6;
    IntroSceneResourceSlot tileGroup7;
    IntroSceneResourceSlot palette5;
    IntroSceneResourceSlot tileGroup8;
    IntroSceneResourceSlot palette6;
    IntroSceneResourceSlot tileGroup9;
    IntroSceneResourceSlot palette7;
    u32 phase;
    u32 delay;
    STRUCT_PAD(0x6D30, 0x6D34);
};

EC IntroSceneTask * IntroScene_CreateTask(void * storage) SECTION(".text.func_08000914");
EC IntroSceneTask * IntroScene_CreateTask(void * storage)
{
    IntroSceneConstructionView * self = static_cast<IntroSceneConstructionView *>(storage);
    IntroScene_InitializeHardwareContext(self);
    self->vtable = gIntroSceneTaskVtable;
    AudioPlayer_Initialize(&self->musicPlayer, 0);
    AudioPlayer_Initialize(&self->soundPlayer, 1);
    void * unpackedSprites = self->unpackedSprites;
    Unpack(gIntroScenePackedSpriteData, unpackedSprites);
    InitSpriteAnimationData(self->sceneAnimations.Data(), unpackedSprites);
    InitSpriteAnimationData(self->overlayAnimations.Data(), gIntroSceneOverlaySpriteData);
    InitSpriteAnimationData(self->sharedAnimations.Data(), gIntroSceneSharedSpriteData);
    self->actors[0].Initialize(self->sharedAnimations, 0x160);
    self->actors[1].Initialize(self->sharedAnimations, 0x342);
    self->actors[2].Initialize(self->sharedAnimations, 0x788);
    self->actors[3].Initialize(self->sharedAnimations, 0x2B2);
    self->actors[4].Initialize(self->sharedAnimations, 0x924);
    self->actors[5].Initialize(self->sharedAnimations, 0x730);
    self->actors[6].Initialize(self->sharedAnimations, 0x730);
    self->actors[7].Initialize(self->sharedAnimations, 0x91A);
    self->actors[8].Initialize(self->sceneAnimations, 0);
    self->actors[9].Initialize(self->sceneAnimations, 0);
    self->actors[10].Initialize(self->sceneAnimations, 1);
    self->actors[11].Initialize(self->sceneAnimations, 4);
    self->actors[12].Initialize(self->sceneAnimations, 7);
    self->actors[13].Initialize(self->overlayAnimations, 0);
    IntroScene_InitializeTileOwner(&self->tileReservations);
    self->tileReservations.Reserve(0, 0x300);
    ResourceHandles_Initialize(&self->paletteReservations);
    ResourceHandles_ReserveRange(&self->paletteReservations, 9, 7);
    self->tileGroup0.AllocateTiles(4);
    self->palette0.AllocatePalette();
    self->tileGroup1.AllocateTiles(4);
    self->palette1.AllocatePalette();
    self->tileGroup2.AllocateTiles(4);
    self->palette2.AllocatePalette();
    self->tileGroup3.AllocateTiles(4);
    self->tileGroup4.AllocateTiles(2);
    self->palette3.AllocatePalette();
    self->tileGroup5.AllocateTiles(2);
    self->palette4.AllocatePalette();
    self->spriteTiles0.AllocateTiles(1);
    self->spriteTiles1.AllocateTiles(1);
    self->spriteTiles2.AllocateTiles(3);
    self->spritePalette.AllocatePalette();
    self->tileGroup6.AllocateTiles(5);
    self->tileGroup7.AllocateTiles(5);
    self->palette5.AllocatePalette();
    self->tileGroup8.AllocateTiles(6);
    self->palette6.AllocatePalette();
    self->tileGroup9.AllocateTiles(2);
    self->palette7.AllocatePalette();
    self->phase = 0;
    self->delay = 0;
    self->transitionRequested = 0;
    self->transitionChoice = 0;
    return reinterpret_cast<IntroSceneTask *>(self);
}


extern char gIntroSceneVtable[] __asm__("vtable_unk_080E5A18");

EC void * __builtin_new(size_t size);
EC void Scene_DestroyBase(void * self, int destructorFlags);

EC IntroSceneOwner * IntroScene_Initialize(IntroSceneOwner * self) SECTION(".text.func_080036F8");
EC IntroSceneOwner * IntroScene_Initialize(IntroSceneOwner * self)
{
    self->vtable = gIntroSceneVtable;
    self->task = IntroScene_CreateTask(__builtin_new(0x6D34));
    return self;
}

EC void IntroScene_Destroy(IntroSceneOwner * self, int destructorFlags) SECTION(".text.func_0800371C");
EC void IntroScene_Destroy(IntroSceneOwner * self, int destructorFlags)
{
    self->vtable = gIntroSceneVtable;
    delete self->task;
    Scene_DestroyBase(self, destructorFlags);
}

EC IntroSceneOwner * func_080036F8(IntroSceneOwner * self) ALIAS(IntroScene_Initialize);
