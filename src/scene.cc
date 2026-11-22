#include "scene.hh"

AScene::~AScene() {}

AUnk_0800080C::~AUnk_0800080C() {}

// 0x080007EC
EC void Scene_DestroyBase() ALIAS(_._6AScene);
EC void func_0800080C() ALIAS(_._13AUnk_0800080C);

// 0x0800082C
EC void SECTION(".text.SceneMain") SceneMain(SmartPtr<AScene> scene_ptr)
{
    while (scene_ptr.Get() != nullptr)
    {
        SmartPtr<AUnk_0800080C> transition = scene_ptr->Run();
        scene_ptr = nullptr;
        if (transition.Get() == nullptr)
            break;
        SmartPtr<AScene> next_scene = transition->vfunc_0C();
        scene_ptr = next_scene.Move();
    }
}
