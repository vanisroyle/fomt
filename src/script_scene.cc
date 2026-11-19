#include "prelude.h"

#include "scene.hh"
#include "script_engine.hh"

EC void * __builtin_new(size_t);
EC char const gSceneBaseVTable[] asm("__vt_6AScene");
EC char const gSceneWithHardwareVTable[] asm("vtable_unk_080E7738");
EC char const gScriptSceneHardwareVTable[] asm("vtable_unk_080E7708");
EC char const gScriptSceneEngineVTable[] asm("vtable_unk_080E7714");
EC char const gScriptSceneVTable[] asm("vtable_unk_080E7728");
EC char const gScriptSceneFactoryVTable[];

struct HardwareContextState;
struct HardwareContext
{
    HardwareContextState * state;
    void const * vtable;
};

EC void InitializeHardwareContext(HardwareContext *) asm("func_08008444");
EC void InitializeScriptEngine(AScriptEngine *) asm("__13AScriptEngine");

struct MusicPlayer;
struct AudioPlayer
{
    MusicPlayer * player;
};

EC AudioPlayer * AudioPlayer_Initialize(AudioPlayer *, u8 index);

struct MapScene;
struct MessageWindow;
struct DisplayFade;
struct AActorEntity;

struct ScriptSceneFactory
{
    void const * vtable;
    AUnk_0800080C * returnSceneFactory;
    unsigned scriptSlot;
};

EC SECTION(".text.0803B6FC") ScriptSceneFactory & InitializeScriptSceneFactory(
    ScriptSceneFactory & factory, AUnk_0800080C *& returnSceneFactory, unsigned scriptSlot)
{
    factory.vtable = gScriptSceneFactoryVTable;
    AUnk_0800080C * nextFactory = returnSceneFactory;
    returnSceneFactory = nullptr;
    factory.returnSceneFactory = nextFactory;
    factory.scriptSlot = scriptSlot;
    return factory;
}

struct ScriptScene
{
    void const * vtable;
    HardwareContext hardware;
    // agbcc places the script engine vtable after its fields.
    u8 engineStorage[sizeof(AScriptEngine) - sizeof(void *)];
    void const * engineVtable;
    AUnk_0800080C * returnSceneFactory;
    unsigned scriptSlot;
    unsigned scenePhase;
    MapScene * mapScene;
    MessageWindow * message;
    DisplayFade * fade;
    AudioPlayer music;
    AudioPlayer scriptAudioChannels[4];

    struct ScriptAudioQueue
    {
        AudioPlayer ** begin;
        AudioPlayer ** end;
        struct StorageLimit
        {
            STRUCT_PAD(0, 4);
            AudioPlayer ** capacityEnd;
        } limit;
    } scriptAudio;

    u16 waitFrames;
    STRUCT_PAD(0x39a, 0x39c);
    AActorEntity * actorToWaitFor;
    u8 fillBackdrop;
    u8 useBlackBackdrop;
};

typedef char ScriptSceneAbiSize[sizeof(ScriptScene) == 0x3A4 ? 1 : -1];
typedef char ScriptSceneEngineVTableOffset[offsetof(ScriptScene, engineVtable) == 0x358 ? 1 : -1];
typedef char ScriptSceneAudioQueueOffset[offsetof(ScriptScene, scriptAudio) == 0x388 ? 1 : -1];

EC SECTION(".text.0803B718") AScene *& CreateScriptScene(AScene *& result, ScriptSceneFactory & factory)
{
    ScriptScene * scene = static_cast<ScriptScene *>(__builtin_new(sizeof(ScriptScene)));

    struct SceneOwnershipTransfer
    {
        AScene * builtScene;
        AUnk_0800080C * returnSceneFactory;
        STRUCT_PAD(0x08, 0x0C);
        AScene ** source;
        AScene * replacement;
    } transfer;

    AUnk_0800080C * nextSceneFactory = factory.returnSceneFactory;
    factory.returnSceneFactory = nullptr;
    transfer.returnSceneFactory = nextSceneFactory;
    unsigned scriptSlot = factory.scriptSlot;

    scene->vtable = gSceneBaseVTable;
    asm("" : : : "memory");
    scene->vtable = gSceneWithHardwareVTable;
    InitializeHardwareContext(&scene->hardware);
    InitializeScriptEngine(reinterpret_cast<AScriptEngine *>(scene->engineStorage));
    scene->hardware.vtable = gScriptSceneHardwareVTable;
    scene->engineVtable = gScriptSceneEngineVTable;
    scene->vtable = gScriptSceneVTable;

    register AUnk_0800080C ** returnSlot asm("r1") = &scene->returnSceneFactory;
    asm("" : "+l"(returnSlot) : "m"(transfer.returnSceneFactory));
    AUnk_0800080C * returnSceneFactory = transfer.returnSceneFactory;
    transfer.returnSceneFactory = nullptr;
    *returnSlot = returnSceneFactory;
    scene->scriptSlot = scriptSlot;
    scene->mapScene = nullptr;
    scene->message = nullptr;
    scene->fade = nullptr;

    AudioPlayer_Initialize(&scene->music, 0);
    AudioPlayer_Initialize(&scene->scriptAudioChannels[0], 1);
    AudioPlayer_Initialize(&scene->scriptAudioChannels[1], 2);
    AudioPlayer_Initialize(&scene->scriptAudioChannels[2], 3);
    AudioPlayer_Initialize(&scene->scriptAudioChannels[3], 4);

    register ScriptScene::ScriptAudioQueue * playback asm("r0") = &scene->scriptAudio;
    asm("" : "+l"(playback));
    playback->begin = nullptr;
    playback->end = nullptr;
    register ScriptScene::ScriptAudioQueue::StorageLimit * capacity asm("r0") = &scene->scriptAudio.limit;
    asm("" : "+l"(capacity));
    capacity->capacityEnd = nullptr;
    scene->waitFrames = 0;
    scene->actorToWaitFor = nullptr;
    scene->fillBackdrop = 1;
    scene->useBlackBackdrop = 0;

    // Retain the temporary owners and moved-from cleanup paths emitted by agbcc.
    asm("" : "+m"(transfer.returnSceneFactory));
    delete transfer.returnSceneFactory;
    transfer.builtScene = reinterpret_cast<AScene *>(scene);
    register AScene ** source asm("r0") = &transfer.builtScene;
    transfer.source = source;
    transfer.replacement = reinterpret_cast<AScene *>(scene);
    asm("" : "+l"(source) : "m"(transfer.source), "m"(transfer.replacement));
    *source = nullptr;
    result = reinterpret_cast<AScene *>(scene);
    asm("" : "+m"(transfer.builtScene));
    delete transfer.builtScene;
    return result;
}
