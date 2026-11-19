#ifndef INTRO_SCENE_HH
#define INTRO_SCENE_HH

#include "scene.hh"

struct IntroTask
{
    u32 renderContext;
    virtual ~IntroTask();
};

struct IntroScene
{
    void * vtable;
    IntroTask * task;
};

EC IntroScene * InitializeIntroScene(IntroScene * self);
EC void DestroyIntroScene(IntroScene * self, int destructorFlags);

#endif // INTRO_SCENE_HH
