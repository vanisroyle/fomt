#include "scene.hh"

#include "farmer.hh"
#include "utility/vec2.hh"

EC void * __builtin_new(size_t size);
EC void Scene_DestroyBase(void * self, int flags);

struct HardwareContextState;

struct MedalExchangeScreen
{
    HardwareContextState * hardware;
    virtual ~MedalExchangeScreen();
};

struct MedalExchangeScene
{
    void const * vtable;
    MedalExchangeScreen * screen;
    AUnk_0800080C * next_scene;
};

EC char const gMedalExchangeSceneVTable[];
EC MedalExchangeScreen * InitMedalExchangeScreen(void * storage, void * game_state);
EC void RunMedalExchangeScreen(AUnk_0800080C ** result, MedalExchangeScreen * screen);

EC SECTION(".text.f3.0805E624") MedalExchangeScene * InitMedalExchangeScene(MedalExchangeScene * self, AUnk_0800080C ** next, void * game_state)
{
    self->vtable = gMedalExchangeSceneVTable;
    self->screen = InitMedalExchangeScreen(__builtin_new(0xAF4), game_state);
    AUnk_0800080C * next_scene = *next;
    *next = nullptr;
    self->next_scene = next_scene;
    return self;
}

EC SECTION(".text.f3.0805E658") void DestroyMedalExchangeScene(MedalExchangeScene * self, int flags)
{
    self->vtable = gMedalExchangeSceneVTable;
    delete self->next_scene;
    delete self->screen;
    Scene_DestroyBase(self, flags);
}

EC SECTION(".text.f3.0805E698") AUnk_0800080C ** RunMedalExchangeScene(AUnk_0800080C ** result, MedalExchangeScene * self)
{
    AUnk_0800080C * screen_result;
    RunMedalExchangeScreen(&screen_result, self->screen);
    delete screen_result;
    AUnk_0800080C * next_scene = self->next_scene;
    self->next_scene = nullptr;
    *result = next_scene;
    return result;
}

struct ChildhoodMemoryScreen
{
    HardwareContextState * hardware;
    virtual ~ChildhoodMemoryScreen();
};

struct MessageWindow;
struct MusicPlayer;
struct AudioPlayer { MusicPlayer * player; };
struct ResourcePool { void * storage; };
struct TileResources { void * handles; };

struct ChildhoodMemoryGameState
{
    STRUCT_PAD(0, 0x1BD8);
    Farmer farmer;
};

struct FarmerNameSubstitution
{
    void const * vtable;
    char const * value;
};

struct ChildhoodMemoryScreenLayout
{
    HardwareContextState * hardware;
    void const * vtable;
    ChildhoodMemoryGameState * game_state;
    u32 dialogue_style;
    MessageWindow * dialogue;
    AudioPlayer dialogue_audio;
    AudioPlayer music;
    AudioPlayer auxiliary_audio;
    STRUCT_PAD(0x20, 0x6420);
    i16 scene_cue;
    i16 scene_phase;
    i16 dialogue_line;
    i16 dialogue_phase;
    FarmerNameSubstitution farmer_name;
};

EC void InitScreenHardware(ChildhoodMemoryScreenLayout *, Vec2 const *) asm("func_080084DC");
EC void InitTileResources(TileResources *) asm("func_0800835C");
EC void DestroyTileResources(TileResources *, int) asm("func_08008364");
EC ResourcePool * ResourcePool_Initialize(ResourcePool *);
EC void DestroyResourcePool(ResourcePool *, int) asm("func_08007714");
EC MessageWindow * CreateMessageWindow(MessageWindow *, u32, u32, u32, u32, void *, void *);
EC void ClearMessageWindowButtonMask(MessageWindow *, u32);
EC AudioPlayer * AudioPlayer_Initialize(AudioPlayer *, u8);
EC char const * GetFarmerName(Farmer const &) asm("func_0800E4E0");
EC char const gChildhoodMemoryScreenVTable[] asm("vtable_unk_080E79DC");
EC char const gChildhoodMemoryFarmerNameVTable[] asm("vtable_unk_080E79E8");

static inline void InitFarmerNameSubstitution(FarmerNameSubstitution * substitution, char const * name)
{
    substitution->vtable = gChildhoodMemoryFarmerNameVTable;
    substitution->value = name;
}

EC SECTION(".text.f3.0805ED4C") ChildhoodMemoryScreen * InitChildhoodMemoryScreen(
    void * screen_storage, void * game_state)
{
    ChildhoodMemoryScreenLayout * self = static_cast<ChildhoodMemoryScreenLayout *>(screen_storage);
    struct
    {
        TileResources graphics;
        ResourcePool font;
        Vec2 key_repeat;
    } scratch;
    new (&scratch.key_repeat) Vec2(30, 1);
    InitScreenHardware(self, &scratch.key_repeat);
    self->vtable = gChildhoodMemoryScreenVTable;
    self->game_state = static_cast<ChildhoodMemoryGameState *>(game_state);
    self->dialogue_style = 0;

    MessageWindow * window_storage = static_cast<MessageWindow *>(__builtin_new(4));
    InitTileResources(&scratch.graphics);
    ResourcePool_Initialize(&scratch.font);
    self->dialogue = CreateMessageWindow(window_storage, 0, 31, 15, self->dialogue_style,
        &scratch.graphics, &scratch.font);
    DestroyResourcePool(&scratch.font, 2);
    DestroyTileResources(&scratch.graphics, 2);

    AudioPlayer_Initialize(&self->dialogue_audio, 1);
    AudioPlayer_Initialize(&self->music, 0);
    AudioPlayer_Initialize(&self->auxiliary_audio, 2);
    InitFarmerNameSubstitution(&self->farmer_name, nullptr);
    ClearMessageWindowButtonMask(self->dialogue, 0xB);
    self->scene_cue = 0;
    self->scene_phase = 0;
    self->dialogue_phase = 2;
    self->dialogue_line = 0;
    self->farmer_name.value = GetFarmerName(self->game_state->farmer);
    return static_cast<ChildhoodMemoryScreen *>(screen_storage);
}

struct ChildhoodMemoryScene
{
    void const * vtable;
    ChildhoodMemoryScreen * screen;
    AUnk_0800080C * next_scene;
};

EC char const gChildhoodMemorySceneVTable[];
EC void RunChildhoodMemoryScreen(AUnk_0800080C ** result, ChildhoodMemoryScreen * screen);

EC SECTION(".text.f3.0805FCD0") ChildhoodMemoryScene * InitChildhoodMemoryScene(ChildhoodMemoryScene * self, AUnk_0800080C ** next, void * game_state)
{
    self->vtable = gChildhoodMemorySceneVTable;
    self->screen = InitChildhoodMemoryScreen(__builtin_new(sizeof(ChildhoodMemoryScreenLayout)), game_state);
    AUnk_0800080C * next_scene = *next;
    *next = nullptr;
    self->next_scene = next_scene;
    return self;
}

EC SECTION(".text.f3.0805FD04") void DestroyChildhoodMemoryScene(ChildhoodMemoryScene * self, int flags)
{
    self->vtable = gChildhoodMemorySceneVTable;
    delete self->next_scene;
    delete self->screen;
    Scene_DestroyBase(self, flags);
}

EC SECTION(".text.f3.0805FD44") AUnk_0800080C ** RunChildhoodMemoryScene(AUnk_0800080C ** result, ChildhoodMemoryScene * self)
{
    AUnk_0800080C * screen_result;
    RunChildhoodMemoryScreen(&screen_result, self->screen);
    delete screen_result;
    AUnk_0800080C * next_scene = self->next_scene;
    self->next_scene = nullptr;
    *result = next_scene;
    return result;
}

struct HorseRaceScreen
{
    HardwareContextState * hardware;
    virtual ~HorseRaceScreen();
};

struct HorseRaceScene
{
    void const * vtable;
    HorseRaceScreen * screen;
    AUnk_0800080C * next_scene;
};

EC char const gHorseRaceSceneVTable[];
EC HorseRaceScreen * func_0805806C(void * storage, void * game_state, u32 mode);
EC void func_080588AC(AUnk_0800080C ** result, HorseRaceScreen * screen);

EC SECTION(".text.f3.0805CEB8") HorseRaceScene * InitHorseRaceScene(
    HorseRaceScene * self, AUnk_0800080C ** next, void * game_state, u8 mode)
{
    self->vtable = gHorseRaceSceneVTable;
    self->screen = func_0805806C(__builtin_new(0x14FC), game_state, mode);
    AUnk_0800080C * next_scene = *next;
    *next = nullptr;
    self->next_scene = next_scene;
    return self;
}

EC SECTION(".text.f3.0805CEFC") void DestroyHorseRaceScene(HorseRaceScene * self, int flags)
{
    self->vtable = gHorseRaceSceneVTable;
    delete self->next_scene;
    delete self->screen;
    Scene_DestroyBase(self, flags);
}

EC SECTION(".text.f3.0805CF3C") AUnk_0800080C ** RunHorseRaceScene(
    AUnk_0800080C ** result, HorseRaceScene * self)
{
    AUnk_0800080C * screen_result;
    func_080588AC(&screen_result, self->screen);
    delete screen_result;
    AUnk_0800080C * next_scene = self->next_scene;
    self->next_scene = nullptr;
    *result = next_scene;
    return result;
}
