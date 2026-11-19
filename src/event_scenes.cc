#include "scene.hh"

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

struct ChildhoodMemoryScene
{
    void const * vtable;
    ChildhoodMemoryScreen * screen;
    AUnk_0800080C * next_scene;
};

EC char const gChildhoodMemorySceneVTable[];
EC ChildhoodMemoryScreen * InitChildhoodMemoryScreen(void * storage, void * game_state);
EC void RunChildhoodMemoryScreen(AUnk_0800080C ** result, ChildhoodMemoryScreen * screen);

EC SECTION(".text.f3.0805FCD0") ChildhoodMemoryScene * InitChildhoodMemoryScene(ChildhoodMemoryScene * self, AUnk_0800080C ** next, void * game_state)
{
    self->vtable = gChildhoodMemorySceneVTable;
    self->screen = InitChildhoodMemoryScreen(__builtin_new(0x6430), game_state);
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
