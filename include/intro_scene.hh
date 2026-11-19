#ifndef INTRO_SCENE_HH
#define INTRO_SCENE_HH

#include "scene.hh"

struct IntroSceneTask
{
    u32 renderContext;
    virtual ~IntroSceneTask();
};

struct IntroSceneOwner
{
    void * vtable;
    IntroSceneTask * task;
};

EC IntroSceneOwner * IntroScene_Initialize(IntroSceneOwner * self);
EC void IntroScene_Destroy(IntroSceneOwner * self, int destructorFlags);

#endif // INTRO_SCENE_HH
