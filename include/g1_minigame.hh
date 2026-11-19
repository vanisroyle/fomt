#ifndef G1_MINIGAME_HH
#define G1_MINIGAME_HH

#include "scene.hh"

struct G1SpriteMinigameScene
{
    void const * vtable;
    AUnk_0800080C * next_scene;
    void * game_state;
    u32 npc_id;
};

struct G1SceneOwner
{
    G1SceneOwner(AUnk_0800080C * source) : value(source) {}
    G1SceneOwner(G1SceneOwner const & other) : value(other.value) { other.value = nullptr; }
    ~G1SceneOwner() { delete value; }

    mutable AUnk_0800080C * value;
};

struct G1SpriteFeedingSceneWrapper
{
    void const * vtable;
    void * game;
    AUnk_0800080C * next_scene;
};

#endif
