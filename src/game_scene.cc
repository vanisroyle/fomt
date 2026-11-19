#include "prelude.h"

struct GameSceneLayout
{
    void const * vtable;
    void * game_state;
    u8 script_engine[0x350];
    u8 script_engine_tail[0x20];
    u8 scene_resource_state[4];
};

extern void const * gGameSceneVtable[]; // 0x080E5BF8
extern GameSceneLayout * gActiveGameScene; // 0x0300040C
EC void SceneResources_Destroy(void * self, int destructor_flags);
EC void DestroyGameScript(void * self, int destructor_flags) ALIAS(_._13AScriptEngine);
EC void GameState_Destroy(void * self, int destructor_flags);
EC void Scene_DestroyBase(void * self, int destructor_flags);

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
