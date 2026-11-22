#include "prelude.h"
#include "scene.hh"

struct GameSceneLayout
{
    void const * vtable;
    void * game_state;
    u8 script_engine[0x350];
    u8 script_engine_tail[0x0C];
    u8 resume_game;
    u8 script_engine_state[0x13];
    u8 scene_resource_state[4];
};

extern void const * gGameSceneVtable[]; // 0x080E5BF8
extern GameSceneLayout * gActiveGameScene; // 0x0300040C
EC void SceneResources_Destroy(void * self, int destructor_flags);
EC void DestroyGameScript(void * self, int destructor_flags) ALIAS(_._13AScriptEngine);
EC void GameState_Destroy(void * self, int destructor_flags);
EC void Scene_DestroyBase(void * self, int destructor_flags);

struct GameSceneEntrySpec
{
    explicit GameSceneEntrySpec(u32 variant) : variant(variant) {}

    u32 variant;
    u8 arguments[8];
};

EC void * VBlankDispatcher_Initialize(void *);
EC void VBlankDispatcher_Destroy(void *, u32);
EC void * __builtin_new(u32);
EC AScene * GameScript_InitializeWithSceneSpec(void *, void *, GameSceneEntrySpec const *);

struct GameSceneVBlankGuard
{
    GameSceneVBlankGuard() { VBlankDispatcher_Initialize(this); }
    ~GameSceneVBlankGuard() { VBlankDispatcher_Destroy(this, 2); }

    void * dispatcher_state;
};

// 0x0801004C
EC SECTION(".text.GameScene_RunScript") SmartPtr<AUnk_0800080C> GameScene_RunScript(GameSceneLayout * self)
{
    GameSceneVBlankGuard dispatcher;
    SmartPtr<AScene> script;
    if (self->resume_game)
    {
        GameSceneEntrySpec spec = GameSceneEntrySpec(0);
        script = GameScript_InitializeWithSceneSpec(__builtin_new(8), self->game_state, &spec);
    }
    else
    {
        GameSceneEntrySpec spec = GameSceneEntrySpec(1);
        script = GameScript_InitializeWithSceneSpec(__builtin_new(8), self->game_state, &spec);
    }
    SceneMain(script);
    return SmartPtr<AUnk_0800080C>();
}

// 0x08010158
EC void GameScene_Destroy(GameSceneLayout * self, int destructor_flags)
{
    self->vtable = gGameSceneVtable;
    gActiveGameScene = nullptr;
    SceneResources_Destroy(self->scene_resource_state, 2);
    DestroyGameScript(self->script_engine, 2);
    if (self->game_state != nullptr)
        GameState_Destroy(self->game_state, 3);
    Scene_DestroyBase(self, destructor_flags);
}
