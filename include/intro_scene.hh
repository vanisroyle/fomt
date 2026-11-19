#ifndef INTRO_SCENE_HH
#define INTRO_SCENE_HH

#include "scene.hh"

struct IntroSceneState
{
    u32 unk_00;
    virtual ~IntroSceneState();
};

struct IntroScene
{
    void * vtable;
    IntroSceneState * state;
};

#endif // INTRO_SCENE_HH
