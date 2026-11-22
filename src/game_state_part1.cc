#include "prelude.h"

#include "entity_actor.hh"
#include "farm.hh"
#include "coop.hh"
#include "message_script.hh"
#include "script_engine.hh"
#include "scene.hh"
#include "shipping_bin.hh"
#include "bachelorette.hh"
#include "farmer.hh"
#include "dog.hh"

#include <stdlib.h>
#include <new>

struct PACKED GameStateHeader
{
    u8 flagA : 1;
    u8 flagB : 1;
    u8 flagC : 1;
    u8 flagD : 1;
    u8 flagE : 1;
    u8 reservedBits : 3;
    u8 reservedBytes[2];
    u8 reservedBit : 1;
    u8 counter : 6;
};

struct GameStateSelection
{
    u32 current;
    u32 previous;
    u8 delay;
};

struct GameStateWordHeader
{
    u32 unused : 13;
    u32 counter : 5;
};

// 0x08010F0C
EC u32 SECTION(".text.game_state_08010F0C") SaveProgress_GetWordCounter(GameStateWordHeader const & header)
{
    return header.counter;
}

// 0x08011588
EC u32 SECTION(".text.game_state_08011588") SaveData_SumBytes(void const * data, u32 length)
{
    u32 sum = 0;
    if (data != nullptr && length != 0)
    {
        u8 const * cursor = static_cast<u8 const *>(data);
        do
        {
            sum += *cursor++;
        } while (--length != 0);
    }
    return sum;
}

extern u16 gSramErrorFlags;
EC bool SramProxy_Write(void * proxy, u32 offset, void const * data, u32 size);

// 0x080115B0
EC u32 SECTION(".text.game_state_080115B0") SaveData_WriteToStorage(void const * saveData, void * storage, u32 offset)
{
    u32 headerOffset = offset;
    u32 const saveSize = 0x34F4;
    if (!SramProxy_Write(storage, offset, &saveSize, sizeof(saveSize)))
        return gSramErrorFlags | 0x10000;
    offset += sizeof(saveSize);
    if (!SramProxy_Write(storage, offset, saveData, saveSize))
        return gSramErrorFlags | 0x20000;
    offset = headerOffset + 0x34F8;
    u32 checksum = SaveData_SumBytes(saveData, saveSize);
    if (!SramProxy_Write(storage, offset, &checksum, sizeof(checksum)))
        return gSramErrorFlags | 0x30000;
    return 0;
}

struct GameStateAffectionSaveView
{
    STRUCT_PAD(0x00, 0x14);
    Farm farm;
    u8 beforeDog[0x1C70 - 0x14 - sizeof(Farm)];
    Dog dog;
};

// 0x08011650
EC u8 * SaveData_LoadFromStorage(u8 * saveData, void * storage, u32 offset, u32 & errors);

// 0x08010E68
EC bool SECTION(".text.game_state_08010E68") SaveData_HaveAllAnimalsHighAffection(GameStateAffectionSaveView const & save)
{
    bool allHighAffection = true;
    if (save.dog.GetAffection() < 200)
        allHighAffection = false;
    Horse const * horse = save.farm.GetHorse();
    if (horse != nullptr && horse->GetAffection() < 200)
        allHighAffection = false;
    for (u32 index = 0; index < save.farm.coop.GetCapacity(); index++)
    {
        Chicken const * chicken = save.farm.coop.GetChicken(index);
        if (chicken != nullptr && chicken->GetAffection() < 200)
            allHighAffection = false;
    }
    for (u32 index = 0; index < save.farm.barn.GetCapacity(); index++)
    {
        Animal const * animal = save.farm.barn.GetCow(index);
        if (animal == nullptr)
            animal = save.farm.barn.GetSheep(index);
        if (animal != nullptr && animal->GetAffection() < 200)
            allHighAffection = false;
    }
    return allHighAffection;
}

// 0x08010348
EC int SECTION(".text.game_state_08010348") Random_NonZero()
{
    int value;
    do
    {
        value = rand();
    } while (value == 0);
    return value;
}

// 0x08010E48
EC u32 SECTION(".text.game_state_08010E48") SaveProgress_GetFlagA(GameStateHeader const & header)
{
    return header.flagA;
}

// 0x08010E50
EC u32 SECTION(".text.game_state_08010E48") SaveProgress_GetFlagB(GameStateHeader const & header)
{
    return header.flagB;
}

// 0x08010E58
EC u32 SECTION(".text.game_state_08010E48") SaveProgress_GetFlagC(GameStateHeader const & header)
{
    return header.flagC;
}

// 0x08010E60
EC u32 SECTION(".text.game_state_08010E48") SaveProgress_GetFlagD(GameStateHeader const & header)
{
    return header.flagD;
}

// 0x08010F04
EC u32 SECTION(".text.game_state_08010F04") SaveProgress_GetFlagE(GameStateHeader const & header)
{
    return header.flagE;
}

// 0x08010F1C
EC u32 SECTION(".text.game_state_08010F1C") SaveProgress_GetByteCounter(GameStateHeader const & header)
{
    return header.counter;
}

// 0x08010F24
EC void SECTION(".text.game_state_08010F1C") SaveProgress_SetFlagA(GameStateHeader & header)
{
    header.flagA = 1;
}

// 0x08010F30
EC void SECTION(".text.game_state_08010F1C") SaveProgress_SetFlagB(GameStateHeader & header)
{
    header.flagB = 1;
}

// 0x08010F3C
EC void SECTION(".text.game_state_08010F1C") SaveProgress_SetFlagC(GameStateHeader & header)
{
    header.flagC = 1;
}

// 0x08010F48
EC void SECTION(".text.game_state_08010F1C") SaveProgress_SetFlagD(GameStateHeader & header)
{
    header.flagD = 1;
}

// 0x08011458
EC void SECTION(".text.game_state_08011458") SaveProgress_SetFlagE(GameStateHeader & header)
{
    header.flagE = 1;
}

// 0x08011510
EC void SECTION(".text.game_state_08011510") Selection_Reset(GameStateSelection & selection)
{
    selection.current = 0x10;
    selection.previous = 0x10;
    selection.delay = 0;
}

// 0x0801151C
EC u32 SECTION(".text.game_state_08011510") Selection_GetCurrent(GameStateSelection const & selection)
{
    return selection.current;
}

// 0x08011520
EC u32 SECTION(".text.game_state_08011510") Selection_GetPrevious(GameStateSelection const & selection)
{
    return selection.previous;
}

// 0x08011540
EC void SECTION(".text.game_state_08011540") Selection_SetCurrent(GameStateSelection & selection, u32 value)
{
    selection.current = value;
}

// 0x08011544
EC void SECTION(".text.game_state_08011540") Selection_Commit(GameStateSelection & selection)
{
    selection.previous = selection.current;
    selection.delay = 2;
}

// 0x08012218
EC void SECTION(".text.game_state_08012218") GameScript_NoOp()
{
}

struct GameStateActorEntity : public AActorEntity
{
    GameStateActorEntity();

    virtual void vfunc_3C(u32 mode);
    virtual u32 vfunc_40();
    virtual u32 vfunc_44();
    virtual bool vfunc_48();
    virtual void vfunc_4C();
    virtual void vfunc_50();
    virtual void vfunc_54();
    virtual void vfunc_58();
    virtual void vfunc_5C(u32 value, u32 enabled);
    virtual void vfunc_60();
    virtual void vfunc_64(u32 value_1, u32 value_2);
    virtual void vfunc_68(u32 value);
    virtual void vfunc_6C();
    virtual void vfunc_70();
    virtual void vfunc_74(u32 value);
    virtual u32 vfunc_78();
    virtual void vfunc_7C();
    virtual void vfunc_80();
    virtual u32 vfunc_84();
    virtual u32 vfunc_88();
    virtual u32 vfunc_8C();
    virtual u32 vfunc_90();
    virtual void vfunc_94();
    virtual void vfunc_98();
    virtual void vfunc_9C(u32 value);
    virtual void vfunc_A0();
    virtual void vfunc_A4();
    virtual void vfunc_A8();
    virtual void vfunc_AC();
};

struct GameStateActiveEntity : public AActorEntity
{
    GameStateActiveEntity();

    virtual void vfunc_3C();
    virtual void vfunc_40();
    virtual void vfunc_44();
    virtual void vfunc_48();
    virtual void vfunc_4C();
    virtual void vfunc_50();
    virtual void vfunc_54();
    virtual void vfunc_58();
    virtual bool vfunc_5C();
    virtual void vfunc_60();
    virtual void vfunc_64();
    virtual void vfunc_68();
    virtual void vfunc_6C();
    virtual void vfunc_70();
    virtual void vfunc_74();
    virtual void vfunc_78();
    virtual void vfunc_7C();
};


struct GameStateObject
{
    virtual ~GameStateObject();
    virtual void vfunc_C();
    virtual void vfunc_10();
    virtual u32 vfunc_14();
    virtual void vfunc_18();
    virtual void vfunc_1C();
    virtual void vfunc_20();
    virtual void vfunc_24(u32 value_1, u32 value_2, u32 value_3);
    virtual bool vfunc_28();
    virtual void vfunc_2C();
    virtual void vfunc_30();
    virtual void vfunc_34();
    virtual void CreateActor(u32 actorId);
    virtual void RemoveActor(u32 actorId);
    virtual GameStateActorEntity * GetActor(u32 actorId);

    virtual void vfunc_44();
    virtual void vfunc_48();
    virtual void vfunc_4C();
    virtual void vfunc_50();
    virtual void vfunc_54();
    virtual void vfunc_58();
    virtual void vfunc_5C();
    virtual void vfunc_60();
    virtual void vfunc_64();
    virtual void vfunc_68();
    virtual void vfunc_6C();
    virtual void vfunc_70();
    virtual void vfunc_74();
    virtual void vfunc_78();
    virtual void vfunc_7C();
    virtual void vfunc_80();
    virtual void ApplyRow15TilePatchVariant1();
    virtual void ApplyRow15TilePatchVariant2();
    virtual void ApplyRow3TilePatchVariant1();
    virtual void ApplyRow3TilePatchVariant2();
    virtual void ApplyRow2TilePatchVariant1();
    virtual void ApplyRow2TilePatchVariant2();
    virtual void ApplyMapTilePatchAtIndex(u32 index);
    virtual void ApplyMapTilePatchInColumn(u32 index);
    virtual i32 RemoveRecordPlayerAlbum();
    virtual i32 SetRecordPlayerAlbum(u32 albumId);
    virtual void vfunc_AC(u32 index);
    virtual bool KeepClockColonVisible();
    virtual void WarpToLocation(Location const & location, bool preserveEventMusic);
    virtual void vfunc_B8();
    virtual void SetGameTime(GameTime const & time);
    virtual void vfunc_C0();
    virtual void vfunc_C4();
    virtual void vfunc_C8();
    virtual void vfunc_CC();
    virtual void vfunc_D0();
    virtual void vfunc_D4();
    virtual void vfunc_D8();
    virtual void vfunc_DC();
    virtual void vfunc_E0();
    virtual void vfunc_E4();
    virtual void vfunc_E8();
    virtual void vfunc_EC();
    virtual void vfunc_F0();
    virtual void vfunc_F4();
    virtual void vfunc_F8();
    virtual void ApplyMapTilePatchAtRow20();
    virtual void ApplyMapTilePatchAtRow23();
    virtual void QueueMapEvent1D();
    virtual void vfunc_108();
    virtual void SetMapTileVariant(u32 index);
    virtual void ClearMapTileVariant(u32 index);
    virtual void vfunc_114();
    virtual void StartMapInteraction(u32 animation);
    virtual void ClearMapInteraction();
    virtual void vfunc_120();
    virtual void vfunc_124();
    virtual void vfunc_128();
    virtual void vfunc_12C();
    virtual void vfunc_130();
    virtual void vfunc_134();
    virtual void vfunc_138();
    virtual void vfunc_13C();
    virtual void vfunc_140();
    virtual void vfunc_144();
    virtual void vfunc_148();
    virtual void vfunc_14C();
    virtual void vfunc_150(u32 value);
    virtual void vfunc_154(u32 value);
    virtual void vfunc_158();
    virtual void vfunc_15C();
    virtual void vfunc_160(u32 value_1, u32 value_2, u32 value_3, u32 value_4, u16 value_5);
    virtual void vfunc_164(u32 value);
};

struct GameStateAudioList
{
    void ** begin;
    void ** end;
    void ** capacity;
    u32 reserved;
};

struct GameStateTextPanel
{
    u32 reserved;
    virtual ~GameStateTextPanel();
};

EC void DestroyMessageWindow(void * dialogue, u32 mode);

struct GameStateDialogueOwner
{
    ~GameStateDialogueOwner()
    {
        if (inner != nullptr)
            DestroyMessageWindow(inner, 3);
    }

    void operator=(void * dialogue)
    {
        if (dialogue != inner)
            if (inner != nullptr)
                DestroyMessageWindow(inner, 3);
        inner = dialogue;
    }

    void * Get() const { return inner; }
    void * inner;
};

typedef AUnk_0800080C GameStateSceneFactory;

struct GameScriptSceneSpec
{
    GameScriptSceneSpec() {}
    GameScriptSceneSpec(GameScriptSceneSpec const & other) { *this = other; }

    u32 variant;
    u32 firstArgument;
    u32 secondArgument;
};

struct GameStateSceneFactoryOwner
{
    GameStateSceneFactory * Move()
    {
        GameStateSceneFactory * result = factory;
        factory = nullptr;
        return result;
    }

    explicit GameStateSceneFactoryOwner(GameStateSceneFactory * factory) : factory(factory) {}
    GameStateSceneFactoryOwner(GameStateSceneFactoryOwner & other) : factory(other.Move()) {}
    ~GameStateSceneFactoryOwner() { delete factory; }

    GameStateSceneFactory * factory;
};

extern u8 const vtable_unk_080E5E74[];
extern u8 const vtable_unk_080E5D74[];
extern u8 const vtable_unk_080E5E44[];
extern u8 const vtable_unk_080E5C24[];
extern u8 const vtable_unk_080E5C14[];

struct GameStateLocatedSceneAction
{
    GameStateLocatedSceneAction()
        : vtable(vtable_unk_080E5E44), stateFlag(false), location(MAP_NONE, 0, 0) {}

    void const * vtable;
    bool stateFlag;
    Location location;
};

struct GameStateModeFlagSceneAction : public GameStateLocatedSceneAction
{
    GameStateModeFlagSceneAction(u8 * saveData, u32 modeFlags)
    {
        vtable = vtable_unk_080E5C24;
        this->saveData = saveData;
        this->modeFlags = modeFlags;
    }

    u8 * saveData;
    u32 modeFlags;
};

struct GameStateModeFlagSceneFactory
{
    GameStateModeFlagSceneFactory(u32 modeFlags, u8 * saveData)
        : vtable(vtable_unk_080E5C14), saveData(saveData)
    {
        action = new GameStateModeFlagSceneAction(saveData, modeFlags);
        this->modeFlags = modeFlags;
    }

    void const * vtable;
    u8 * saveData;
    GameStateModeFlagSceneAction * action;
    u32 modeFlags;
};

struct GameScriptSceneFactory
{
    GameScriptSceneFactory(u8 * saveData, GameScriptSceneSpec spec)
        : vtable(vtable_unk_080E5E74), saveData(saveData), spec(spec) {}

    void const * vtable;
    u8 * saveData;
    GameScriptSceneSpec spec;
};

struct GameStateNestedSceneFactory
{
    GameStateNestedSceneFactory(GameStateSceneFactoryOwner factory)
        : vtable(vtable_unk_080E5D74), factory(factory.Move()) {}

    void const * vtable;
    GameStateSceneFactory * factory;
};

struct GameStateSavedSceneFactoryOwner
{
    GameStateSceneFactory * Move()
    {
        GameStateSceneFactory * result = factory;
        factory = nullptr;
        return result;
    }

    explicit GameStateSavedSceneFactoryOwner(GameStateSceneFactory * factory) : factory(factory) {}
    GameStateSavedSceneFactoryOwner(GameStateSavedSceneFactoryOwner & other) : factory(other.Move())
    {
        asm ("" : : "m" (factory));
    }
    ~GameStateSavedSceneFactoryOwner() { delete factory; }

    GameStateSceneFactory * factory;
};

extern u8 const vtable_unk_080E5E34[];
extern u8 const vtable_unk_080E5CA4[];
extern u8 const vtable_unk_080E5CF4[];
extern u8 const vtable_unk_080E5C44[];
extern u8 const vtable_unk_080E5C34[];
extern u8 const vtable_unk_080E5E54[];
extern u8 const vtable_unk_080E5CD4[];
extern u8 const vtable_unk_080E5CC4[];
extern u8 const vtable_unk_080E5CE4[];
extern u8 const vtable_unk_080E5CB4[];
extern u8 const vtable_unk_080E5D84[];
extern u8 const vtable_unk_080E5D64[];
extern u8 const vtable_unk_080E5DA4[];
extern u8 const vtable_unk_080E5DC4[];
extern u8 const vtable_unk_080E5DB4[];
extern u8 const vtable_unk_080E5DD4[];
extern u8 const vtable_unk_080E5E04[];
extern u8 const vtable_unk_080E5DF4[];
extern u8 const vtable_unk_080E5DE4[];

template <u8 const * Methods>
struct GameStateSavedSceneFactory
{
    GameStateSavedSceneFactory(GameStateSavedSceneFactoryOwner factory, u8 * saveData)
        : vtable(Methods), factory(factory.Move()), saveData(saveData) {}

    void const * vtable;
    GameStateSceneFactory * factory;
    u8 * saveData;
};

template <u8 const * Methods, typename Argument>
struct GameStateSavedSceneArgumentFactory
{
    GameStateSavedSceneArgumentFactory(GameStateSavedSceneFactoryOwner factory, u8 * saveData, Argument argument)
        : vtable(Methods), factory(factory.Move()), saveData(saveData), argument(argument) {}

    void const * vtable;
    GameStateSceneFactory * factory;
    u8 * saveData;
    Argument argument;
};

template <>
struct GameStateSavedSceneFactory<vtable_unk_080E5E54>
{
    GameStateSavedSceneFactory(GameStateSavedSceneFactoryOwner factory, u8 * saveData)
        : vtable(vtable_unk_080E5E54), saveData(saveData), factory(factory.Move()) {}

    void const * vtable;
    u8 * saveData;
    GameStateSceneFactory * factory;
};

struct GameStateScript;

struct GameStateRuntimeDmaQueue
{
    void * begin;
    void * end;
    u32 allocationState;
    void * capacityEnd;
};

struct GameStateRuntime
{
    GameStateRuntimeDmaQueue dmaQueue;
    u16 displayControl;
    u16 displayRegisters[0x2A];
    void * taskDispatcher;
    STRUCT_PAD(0x6C, 0x88);
    GameStateScript * script;
    u8 * saveData;
    GameScriptSceneSpec sceneSpec;
    u32 pendingCommand;
    void * displayFade;
    SmartPtr<GameStateSceneFactory> pendingSceneFactory;
    GameStateObject * scene;
    GameStateDialogueOwner dialogue;
    GameScriptTextVariables * textVariables;
    SmartPtr<GameStateTextPanel> textPanel;
    SmartPtr<GameStateTextPanel> secondaryTextPanel;
    u8 musicPlayer[0x14];
    GameStateAudioList soundPlayers;
    u16 pendingArgument;
    STRUCT_PAD(0xE2, 0xE4);
    GameStateActorEntity * waitActor;
    bool fadeState;
    u8 fadeTarget;
    bool scriptBackgroundEnabled;
    u16 scriptBackgroundColor;
    char * temporaryText;
    virtual ~GameStateRuntime();

    void * GetTextVariables() const { return textVariables; }
    u8 * GetSaveData() const { return saveData; }
};

struct GameStateScript
{
    u32 vtable;
    GameStateRuntime * runtime;
};

// 0x08013D04
EC void SECTION(".text.game_state_08013D04") GameScript_QueueModeFlagScene(GameStateScript & script, u8 modeEnabled)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.pendingSceneFactory = reinterpret_cast<GameStateSceneFactory *>(
        new GameStateModeFlagSceneFactory(modeEnabled ? 2 : 0, runtime.GetSaveData()));
    runtime.pendingCommand = 0x18;
}

// 0x08014C34
EC void SECTION(".text.game_state_08014C34") GameScript_CreateHorseActor(GameStateScript & script, u32 kind, u32 ageYears, u32 mapId, i32 x, i32 y)
{
    GameStateRuntime & runtime = *script.runtime;
    if (kind == 0)
    {
        Farm & farm = *reinterpret_cast<Farm *>(runtime.GetSaveData() + 0x14);
        farm.SetHorse(Horse(ActorLocation(Location(mapId, x, y), 0), ageYears * 120));
        runtime.scene->CreateActor(0x2C);
        GameStateActorEntity * horseActor = runtime.scene->GetActor(0x2C);
        if (horseActor != nullptr && !horseActor->vfunc_28())
            horseActor->vfunc_20();
    }
}

extern u8 const gTaskNodeVtable[];
extern u8 const gTaskListVtable[];
extern u8 const vtable_unk_080E5BD8[];
extern u8 const vtable_unk_080E5AF0[];
extern u8 const vtable_unk_080E5AE0[];
extern u8 const vtable_unk_080E5AD0[];
EC void TaskNode_Destroy(void * node, u32 flags);
EC void TaskList_Destroy(void * list, u32 flags);
EC void TaskList_Append(void * list, void * node);

struct GameStateMapObstacleRecord
{
    i32 kind;
    u32 flags;
    i16 x;
    i16 y;
    u32 state;
};

struct GameStateMapObstacleResult
{
    bool removed;
    STRUCT_PAD(0x01, 0x04);
    u32 effectId;
    i32 x;
    i32 y;
    bool additionalAction;
};

EC void GetMapForageObstacle(GameStateMapObstacleRecord & record, u8 * mapState, u32 index);
EC void SubtractMapAvailabilityCount(u8 * mapState, u32 index, i32 amount);
EC u32 GetMapAvailabilityCount(u8 const * mapState, u32 index);
EC void ClearMapAvailabilityBit(u8 * mapState, u32 index, u32 bit);

// 0x080174C8
EC bool SECTION(".text.game_state_080174C8") GameState_TryBreakMapObstacle(Farm & farm, u8 * mapState, u32 index, u32 bit, i32 power, bool stoneTool, GameStateMapObstacleResult & result)
{
    GameStateMapObstacleRecord record;
    GetMapForageObstacle(record, mapState, index);
    GameStateMapObstacleRecord & obstacle = record;
    i32 requiredPower = 0;
    if (stoneTool)
    {
        switch (obstacle.kind)
        {
        case 0x13:
            requiredPower = 2;
            break;
        case 0x14:
            requiredPower = 7;
            break;
        default:
            return false;
        }
    }
    else
    {
        switch (obstacle.kind)
        {
        case 0x12:
            break;
        case 0x15:
            requiredPower = 1;
            break;
        default:
            return false;
        }
    }

    result.additionalAction = false;
    i32 remainingPower = power - requiredPower;
    if (remainingPower > 0)
    {
        if (obstacle.kind != 0x12)
            SubtractMapAvailabilityCount(mapState, index, remainingPower);
        if (obstacle.kind == 0x12 || GetMapAvailabilityCount(mapState, index) == 0)
        {
            ClearMapAvailabilityBit(mapState, index, bit);
            result.removed = true;
            result.x = obstacle.x;
            result.y = obstacle.y;
            switch (obstacle.kind)
            {
            case 0x15:
                farm.method_08009B68(6);
                result.effectId = 7;
                break;
            case 0x12:
                farm.method_08009B68(1);
                result.effectId = 4;
                break;
            case 0x13:
                result.effectId = 6;
                break;
            case 0x14:
                result.effectId = 9;
                break;
            }
            return true;
        }
    }
    return false;
}

struct GameStateFrameTaskNode
{
    GameStateFrameTaskNode()
    {
        vtable = gTaskNodeVtable;
        previous = nullptr;
        next = nullptr;
    }

    GameStateFrameTaskNode ** previous;
    GameStateFrameTaskNode * next;
    void const * vtable;
};

template <u8 const * TaskVtable> struct GameStateFrameTask
{
    explicit GameStateFrameTask(void * taskContext)
    {
        node.vtable = TaskVtable;
        context = taskContext;
    }

    ~GameStateFrameTask() { TaskNode_Destroy(&node, 2); }

    GameStateFrameTaskNode node;
    void * context;
};

struct GameStateFrameTaskListNode : public GameStateFrameTaskNode
{
    GameStateFrameTaskListNode() { vtable = gTaskListVtable; }
};

struct GameStateFrameTaskEndNode : public GameStateFrameTaskNode
{
    GameStateFrameTaskEndNode() { vtable = vtable_unk_080E5BD8; }
};

struct GameStateFrameTaskList
{
    GameStateFrameTaskList() : first(&sentinel)
    {
        sentinel.previous = &first;
        sentinel.next = &sentinel;
    }

    ~GameStateFrameTaskList() { TaskList_Destroy(this, 2); }

    GameStateFrameTaskListNode node;
    GameStateFrameTaskNode * first;
    GameStateFrameTaskEndNode sentinel;
};

struct GameStateFrameTaskDispatch
{
    GameStateFrameTaskNode ** previous;
    GameStateFrameTaskNode * next;

    virtual bool Run();
    virtual ~GameStateFrameTaskDispatch();
    virtual void Append(void * node);
    virtual void Remove(void * node);
};

EC GameStateFrameTaskDispatch & VBlankDispatcher_GetTasks(void * dispatcher);
EC void VBlankDispatcher_Wait(void * dispatcher);

// 0x08011CD8
EC void SECTION(".text.game_state_08011CD8") GameState_RunVBlankFrameTasks(GameStateRuntime & runtime, void * oamBuffer)
{
    u8 * hardware = reinterpret_cast<u8 *>(&runtime);
    GameStateFrameTask<vtable_unk_080E5AF0> display(hardware + 0x10);
    GameStateFrameTask<vtable_unk_080E5AE0> oam(oamBuffer);
    GameStateFrameTask<vtable_unk_080E5AD0> dma(hardware);
    GameStateFrameTaskList tasks;
    TaskList_Append(&tasks, hardware + 0x6C);
    TaskList_Append(&tasks, &display);
    TaskList_Append(&tasks, &dma);
    TaskList_Append(&tasks, &oam);
    void * dispatcher = hardware + 0x68;
    VBlankDispatcher_GetTasks(dispatcher).Append(&tasks);
    VBlankDispatcher_Wait(dispatcher);
    VBlankDispatcher_GetTasks(dispatcher).Remove(&tasks);
}

// 0x08012BBC
EC void SECTION(".text.game_state_08012BBC") GameScript_QueueSceneVariant0A(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    GameScriptSceneSpec spec;
    spec.variant = 0x0A;
    GameStateSavedSceneFactoryOwner factory(reinterpret_cast<GameStateSceneFactory *>(new GameScriptSceneFactory(runtime.GetSaveData(), spec)));
    runtime.pendingSceneFactory = reinterpret_cast<GameStateSceneFactory *>(new GameStateSavedSceneFactory<vtable_unk_080E5E34>(factory, runtime.GetSaveData()));
    runtime.pendingCommand = 0x18;
}

// 0x08012CF4
EC void SECTION(".text.game_state_08012CF4") GameScript_QueueSceneVariant0C(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    GameScriptSceneSpec spec;
    spec.variant = 0x0C;
    GameStateSavedSceneFactoryOwner factory(reinterpret_cast<GameStateSceneFactory *>(new GameScriptSceneFactory(runtime.GetSaveData(), spec)));
    runtime.pendingSceneFactory = reinterpret_cast<GameStateSceneFactory *>(new GameStateSavedSceneFactory<vtable_unk_080E5DE4>(factory, runtime.GetSaveData()));
    runtime.pendingCommand = 0x18;
}

// 0x08012D88
EC void SECTION(".text.game_state_08012D88") GameScript_QueueSceneVariant0D(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    GameScriptSceneSpec spec;
    spec.variant = 0x0D;
    GameStateSavedSceneFactoryOwner factory(reinterpret_cast<GameStateSceneFactory *>(new GameScriptSceneFactory(runtime.GetSaveData(), spec)));
    runtime.pendingSceneFactory = reinterpret_cast<GameStateSceneFactory *>(new GameStateSavedSceneFactory<vtable_unk_080E5DF4>(factory, runtime.GetSaveData()));
    runtime.pendingCommand = 0x18;
}

// 0x08012EC0
EC void SECTION(".text.game_state_08012EC0") GameScript_QueueSceneVariant0F(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    GameScriptSceneSpec spec;
    spec.variant = 0x0F;
    GameStateSavedSceneFactoryOwner factory(reinterpret_cast<GameStateSceneFactory *>(new GameScriptSceneFactory(runtime.GetSaveData(), spec)));
    runtime.pendingSceneFactory = reinterpret_cast<GameStateSceneFactory *>(new GameStateSavedSceneFactory<vtable_unk_080E5E04>(factory, runtime.GetSaveData()));
    runtime.pendingCommand = 0x18;
}

// 0x08012F54
EC void SECTION(".text.game_state_08012F54") GameScript_QueueSceneVariant10(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    GameScriptSceneSpec spec;
    spec.variant = 0x10;
    GameStateSavedSceneFactoryOwner factory(reinterpret_cast<GameStateSceneFactory *>(new GameScriptSceneFactory(runtime.GetSaveData(), spec)));
    runtime.pendingSceneFactory = reinterpret_cast<GameStateSceneFactory *>(new GameStateSavedSceneFactory<vtable_unk_080E5DD4>(factory, runtime.GetSaveData()));
    runtime.pendingCommand = 0x18;
}

// 0x08013080
EC void SECTION(".text.game_state_08013080") GameScript_QueueSceneVariant12(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    GameScriptSceneSpec spec;
    spec.variant = 0x12;
    GameStateSavedSceneFactoryOwner factory(reinterpret_cast<GameStateSceneFactory *>(new GameScriptSceneFactory(runtime.GetSaveData(), spec)));
    runtime.pendingSceneFactory = reinterpret_cast<GameStateSceneFactory *>(new GameStateSavedSceneFactory<vtable_unk_080E5DB4>(factory, runtime.GetSaveData()));
    runtime.pendingCommand = 0x18;
}

// 0x08013114
EC void SECTION(".text.game_state_08013114") GameScript_QueueSceneVariant13(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    GameScriptSceneSpec spec;
    spec.variant = 0x13;
    GameStateSavedSceneFactoryOwner factory(reinterpret_cast<GameStateSceneFactory *>(new GameScriptSceneFactory(runtime.GetSaveData(), spec)));
    runtime.pendingSceneFactory = reinterpret_cast<GameStateSceneFactory *>(new GameStateSavedSceneFactory<vtable_unk_080E5DC4>(factory, runtime.GetSaveData()));
    runtime.pendingCommand = 0x18;
}

// 0x080131A8
EC void SECTION(".text.game_state_080131A8") GameScript_QueueSceneVariant14(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    GameScriptSceneSpec spec;
    spec.variant = 0x14;
    GameStateSavedSceneFactoryOwner factory(reinterpret_cast<GameStateSceneFactory *>(new GameScriptSceneFactory(runtime.GetSaveData(), spec)));
    runtime.pendingSceneFactory = reinterpret_cast<GameStateSceneFactory *>(new GameStateSavedSceneArgumentFactory<vtable_unk_080E5DA4, u32>(factory, runtime.GetSaveData(), 0));
    runtime.pendingCommand = 0x18;
}

// 0x0801323C
EC void SECTION(".text.game_state_0801323C") GameScript_QueueSceneVariant16(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    GameScriptSceneSpec spec;
    spec.variant = 0x16;
    GameStateSavedSceneFactoryOwner factory(reinterpret_cast<GameStateSceneFactory *>(new GameScriptSceneFactory(runtime.GetSaveData(), spec)));
    runtime.pendingSceneFactory = reinterpret_cast<GameStateSceneFactory *>(new GameStateSavedSceneFactory<vtable_unk_080E5D64>(factory, runtime.GetSaveData()));
    runtime.pendingCommand = 0x18;
}

// 0x080132D0
EC void SECTION(".text.game_state_080132D0") GameScript_QueueSceneVariant17(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    GameScriptSceneSpec spec;
    spec.variant = 0x17;
    GameStateSavedSceneFactoryOwner factory(reinterpret_cast<GameStateSceneFactory *>(new GameScriptSceneFactory(runtime.GetSaveData(), spec)));
    runtime.pendingSceneFactory = reinterpret_cast<GameStateSceneFactory *>(new GameStateSavedSceneFactory<vtable_unk_080E5D84>(factory, runtime.GetSaveData()));
    runtime.pendingCommand = 0x18;
}

// 0x08013544
EC void SECTION(".text.game_state_08013544") GameScript_QueueSceneVariant19(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    GameScriptSceneSpec spec;
    spec.variant = 0x19;
    GameStateSavedSceneFactoryOwner factory(reinterpret_cast<GameStateSceneFactory *>(new GameScriptSceneFactory(runtime.GetSaveData(), spec)));
    runtime.pendingSceneFactory = reinterpret_cast<GameStateSceneFactory *>(new GameStateSavedSceneFactory<vtable_unk_080E5CB4>(factory, runtime.GetSaveData()));
    runtime.pendingCommand = 0x18;
}

// 0x080135D8
EC void SECTION(".text.game_state_080135D8") GameScript_QueueSceneVariant1A(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    GameScriptSceneSpec spec;
    spec.variant = 0x1A;
    GameStateSavedSceneFactoryOwner factory(reinterpret_cast<GameStateSceneFactory *>(new GameScriptSceneFactory(runtime.GetSaveData(), spec)));
    runtime.pendingSceneFactory = reinterpret_cast<GameStateSceneFactory *>(new GameStateSavedSceneFactory<vtable_unk_080E5CE4>(factory, runtime.GetSaveData()));
    runtime.pendingCommand = 0x18;
}

// 0x0801366C
EC void SECTION(".text.game_state_0801366C") GameScript_QueueSceneVariant1B(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    GameScriptSceneSpec spec;
    spec.variant = 0x1B;
    GameStateSavedSceneFactoryOwner factory(reinterpret_cast<GameStateSceneFactory *>(new GameScriptSceneFactory(runtime.GetSaveData(), spec)));
    runtime.pendingSceneFactory = reinterpret_cast<GameStateSceneFactory *>(new GameStateSavedSceneFactory<vtable_unk_080E5CC4>(factory, runtime.GetSaveData()));
    runtime.pendingCommand = 0x18;
}

// 0x08013700
EC void SECTION(".text.game_state_08013700") GameScript_QueueSceneVariant1C(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    GameScriptSceneSpec spec;
    spec.variant = 0x1C;
    GameStateSavedSceneFactoryOwner factory(reinterpret_cast<GameStateSceneFactory *>(new GameScriptSceneFactory(runtime.GetSaveData(), spec)));
    runtime.pendingSceneFactory = reinterpret_cast<GameStateSceneFactory *>(new GameStateSavedSceneFactory<vtable_unk_080E5CD4>(factory, runtime.GetSaveData()));
    runtime.pendingCommand = 0x18;
}

// 0x08013794
EC void SECTION(".text.game_state_08013794") GameScript_QueueSceneVariant1D(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    GameScriptSceneSpec spec;
    spec.variant = 0x1D;
    GameStateSavedSceneFactoryOwner factory(reinterpret_cast<GameStateSceneFactory *>(new GameScriptSceneFactory(runtime.GetSaveData(), spec)));
    runtime.pendingSceneFactory = reinterpret_cast<GameStateSceneFactory *>(new GameStateSavedSceneFactory<vtable_unk_080E5E54>(factory, runtime.GetSaveData()));
    runtime.pendingCommand = 0x18;
}

// 0x0801395C
EC void SECTION(".text.game_state_0801395C") GameScript_QueueSceneVariant20(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    GameScriptSceneSpec spec;
    spec.variant = 0x20;
    GameStateSavedSceneFactoryOwner factory(reinterpret_cast<GameStateSceneFactory *>(new GameScriptSceneFactory(runtime.GetSaveData(), spec)));
    runtime.pendingSceneFactory = reinterpret_cast<GameStateSceneFactory *>(new GameStateSavedSceneFactory<vtable_unk_080E5C34>(factory, runtime.GetSaveData()));
    runtime.pendingCommand = 0x18;
}

// 0x08013BDC
EC void SECTION(".text.game_state_08013BDC") GameScript_QueueSceneVariant29(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    GameScriptSceneSpec spec;
    spec.variant = 0x29;
    GameStateSavedSceneFactoryOwner factory(reinterpret_cast<GameStateSceneFactory *>(new GameScriptSceneFactory(runtime.GetSaveData(), spec)));
    runtime.pendingSceneFactory = reinterpret_cast<GameStateSceneFactory *>(new GameStateSavedSceneFactory<vtable_unk_080E5C44>(factory, runtime.GetSaveData()));
    runtime.pendingCommand = 0x18;
}

// 0x08013C70
EC void SECTION(".text.game_state_08013C70") GameScript_QueueSceneVariant2B(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    GameScriptSceneSpec spec;
    spec.variant = 0x2B;
    GameStateSavedSceneFactoryOwner factory(reinterpret_cast<GameStateSceneFactory *>(new GameScriptSceneFactory(runtime.GetSaveData(), spec)));
    runtime.pendingSceneFactory = reinterpret_cast<GameStateSceneFactory *>(new GameStateSavedSceneFactory<vtable_unk_080E5CF4>(factory, runtime.GetSaveData()));
    runtime.pendingCommand = 0x18;
}

// 0x0801437C
EC void SECTION(".text.game_state_0801437C") GameScript_QueueSceneVariant24(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    GameScriptSceneSpec spec;
    spec.variant = 0x24;
    GameStateSavedSceneFactoryOwner factory(reinterpret_cast<GameStateSceneFactory *>(new GameScriptSceneFactory(runtime.GetSaveData(), spec)));
    runtime.pendingSceneFactory = reinterpret_cast<GameStateSceneFactory *>(new GameStateSavedSceneFactory<vtable_unk_080E5CA4>(factory, runtime.GetSaveData()));
    runtime.pendingCommand = 0x18;
}

// 0x08013E6C
EC void SECTION(".text.game_state_08013E6C") GameScript_QueueSceneVariant2E(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    GameScriptSceneSpec spec;
    spec.variant = 0x2E;
    GameStateSceneFactoryOwner factory(reinterpret_cast<GameStateSceneFactory *>(new GameScriptSceneFactory(runtime.GetSaveData(), spec)));
    runtime.pendingSceneFactory = reinterpret_cast<GameStateSceneFactory *>(new GameStateNestedSceneFactory(factory));
    runtime.pendingCommand = 0x18;
}

struct GameStateSceneContainerFactory
{
    void const * vtable;
    GameStateSceneFactory * factory;
    void * argument;
};

EC GameStateSceneContainerFactory & InitializeScriptSceneFactory(GameStateSceneContainerFactory & self, GameStateSceneFactoryOwner factory, void * argument);

// 0x08013EF8
EC void SECTION(".text.game_state_08013EF8") GameScript_QueueSceneVariant2F(GameStateScript & script, void * argument)
{
    GameStateRuntime & runtime = *script.runtime;
    GameScriptSceneSpec spec;
    spec.variant = 0x2F;
    GameStateSceneFactoryOwner factory(reinterpret_cast<GameStateSceneFactory *>(new GameScriptSceneFactory(runtime.GetSaveData(), spec)));
    runtime.pendingSceneFactory = reinterpret_cast<GameStateSceneFactory *>(&InitializeScriptSceneFactory(*new GameStateSceneContainerFactory, factory, argument));
    runtime.pendingCommand = 0x18;
}

// 0x080125EC
EC void SECTION(".text.game_state_080125EC") GameScript_CloseDialoguePanels(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.textPanel = nullptr;
    runtime.secondaryTextPanel = nullptr;
    runtime.dialogue = nullptr;
    runtime.displayControl &= ~0x100;
}



struct GameStateShippingView
{
    ShippingBin & GetShippingBin() { return shippingBin; }

    STRUCT_PAD(0x00, 0x54);
    ShippingBin shippingBin;
    u8 reserved[0x1AA8 - 0x54 - sizeof(ShippingBin)];
    u8 earnings[0x34C5 - 0x1AA8];
    u8 doubleShippingValue;
};

struct GameStateMapQuery
{
    struct MapContext
    {
        u32 reserved;
        u32 mapId;
    };

    u32 vtable;
    MapContext * context;
    STRUCT_PAD(0x08, 0x103C);
    u8 forceInterior;
};

EC MapData const * GetMapData(u32 mapId);
EC void func_0809ABD8(void * earnings, u32 amount);

// 0x0801140C
EC bool SECTION(".text.game_state_0801140C") SaveData_CollectShippingValue(GameStateShippingView & save)
{
    ShippingBin & shipping = save.GetShippingBin();
    u32 amount = shipping.GetValueShipped();
    void * earnings = save.earnings;
    func_0809ABD8(earnings, amount);
    u8 & doubled = save.doubleShippingValue;
    if (doubled != 0)
    {
        func_0809ABD8(earnings, amount);
        doubled = 0;
    }
    shipping.ResetValueShipped();
    return amount != 0;
}


// 0x08017C00
EC u32 SECTION(".text.game_state_08017C00") GameMap_IsInteriorOrForced(GameStateMapQuery & query)
{
    if (query.forceInterior != 0)
        goto interior;
    if (!GetMapData(query.context->mapId)->is_interior)
        return 0;
interior:
    return 1;
}


struct GameStateGlobalStatus
{
    u32 GetMapId() const { return mapId; }

    u8 hasReturnLocation;
    u32 mapId;
    u16 mapFlags;
};

struct GameStateScriptEngine : public ScriptEngine
{
    GameStateScriptEngine();

    virtual int OnCall(int id);
};

struct GameStateScriptEvent
{
    GameStateScriptEvent(u32 id, u32 argument) : id(id), argument(argument) {}

    u32 id;
    u32 argument;
};

template <u32 Capacity> struct GameStateScriptEventQueue
{
    void Push(GameStateScriptEvent const & event)
    {
        if (count < Capacity)
        {
            u32 offset = sizeof(count) + count * sizeof(GameStateScriptEvent);
            new (reinterpret_cast<u8 *>(this) + offset) GameStateScriptEvent(event);
            count++;
        }
    }

    u32 count;
    GameStateScriptEvent events[Capacity];
};

struct GameStateGlobal
{
    GameStateGlobal();

    STRUCT_PAD(0x000, 0x008);
    GameStateScriptEngine engine;
    STRUCT_PAD(0x364, 0x36C);
    GameStateGlobalStatus status;
    GameStateScriptEventQueue<2> smallEventQueue;
    GameStateScriptEventQueue<40> largeEventQueue;
    GameStateScriptEventQueue<1> singleEventQueue;
};

extern GameStateGlobal * gActiveGameScene;

// 0x08011DC4
EC GameStateScript * GameScript_InitializeWithSceneSpec(GameStateScript * script, u8 * saveData, GameScriptSceneSpec const & sceneSpec);

// 0x08011ED8
EC GameStateScript * GameScript_InitializeDefaultScene(GameStateScript * script, u8 * saveData);

// 0x08012B24
EC void SECTION(".text.game_state_08012B24") GameScript_QueueSceneEvent(GameStateScript & script, GameStateScriptEvent const & event, i32 queue)
{
    GameStateGlobal & game = *gActiveGameScene;
    GameStateScriptEventQueue<2> & small = game.smallEventQueue;
    switch (queue)
    {
    case 0:
        small.Push(event);
        break;
    case 1:
        game.largeEventQueue.Push(event);
        break;
    case 2:
        game.singleEventQueue.Push(event);
        break;
    }
}

// 0x08016D9C
EC u32 SECTION(".text.game_state_08016D9C") GameScript_GetReturnMap()
{
    return gActiveGameScene->status.GetMapId();
}

EC void Audio_StopAllPlayers();
EC void AudioPlayer_Stop(void * self);
EC u32 AudioPlayer_IsPlaying(void * self);
EC void ClearMessageWindowSpeakerName(void * self);
EC void ClearMessageWindowPortrait(void * self);
EC void ClearMessageWindowIndicator(void * self);
EC void func_080323C8(AActorEntity & self);
EC void func_08032308(AActorEntity & self, u32 target, i32 speed);
EC void func_0803233C(AActorEntity & self, u32 target, i32 speed);
EC void func_080323E0(AActorEntity & self, u32 value);
EC void CreateSideEntityEffectSprite(void * self);
EC void ClearSideEntityEffectSprite(void * self);
EC void BeginSideEntityTransition(void * self);
EC void CreateSideEntityParticleSprites(void * self);
EC void func_080D7F3C(GameStateRuntime & self);
EC void func_080D7F08(GameStateRuntime & self);
EC void FormatMessageInteger(i32 value, char * text, u32 digits);
EC void SetMessageWindowTextOptions(void * self, const char * text, u16 speed, const void * substitutions);
EC void func_08032384(AActorEntity & self, u32 value, bool enabled);
EC u32 IsSpriteWorkerOutOfTasks(AActorEntity & self);
EC void CloseMessageWindow(void * self);
EC void func_080D7F88(GameStateRuntime & self);
EC void func_080D7F74(GameStateRuntime & self);
EC void func_080D7F60(GameStateRuntime & self);
EC void SetMessageWindowSpeakerName(void * self, const char * name);
EC void SetMessageWindowPortrait(void * self, u32 value);
EC u32 GetRosterNpcName(void * self, u32 value);
EC void ShowSpriteWorkerEffect(AActorEntity & self, u32 map, i32 x, i32 y);
extern u8 const vtable_unk_080E5E64[];
EC void Scene_DestroyBase(GameStateScript & self, u32 flags);
EC void func_080D7F9C(GameStateRuntime & self, i32 id, u32 value_1, u32 value_2, u32 value_3, u32 value_4);
EC void ActivateRosterNamedNpc(void * self, void * value);
EC void AudioPlayer_StartSong(void * self, u16 amount);
EC void AudioPlayer_StartSongIfStoppedOrChanged(void * self, u16 amount);
EC void AudioPlayer_StartOrResumeSong(void * self, u16 amount);
EC void AudioPlayer_FadeOut(void * self, u32 speed);
EC void AudioPlayer_ResetChannels(void * player);
EC void AudioPlayer_SetTempo(void * player, u16 tempo);
EC void AudioPlayer_SetTrackVolume(void * player, u16 volume, u16 tracks);
EC void AudioPlayer_SetTrackPitch(void * player, i16 pitch, u16 tracks);
EC void AudioPlayer_SetTrackPan(void * player, i8 pan, u16 tracks);
EC void AudioPlayer_SetTrackModDepth(void * player, u8 depth, u16 tracks);
EC void AudioPlayer_SetTrackLfoSpeed(void * player, u8 speed, u16 tracks);

struct GameStateScriptMusicSettings
{
    u16 tempo;
    u16 volume;
    i16 pitch;
    i8 pan;
    u8 modulation;
    u8 lfoSpeed;
};

struct GameStateMusicPlayer
{
    void StartWithMode(i32 mode, u16 song)
    {
        switch (mode)
        {
        case 0:
            AudioPlayer_StartSong(this, song);
            break;
        case 1:
            AudioPlayer_StartSongIfStoppedOrChanged(this, song);
            break;
        case 2:
            AudioPlayer_StartOrResumeSong(this, song);
            break;
        }
    }

    void ResetChannels() { AudioPlayer_ResetChannels(this); }
    void SetTempo(u16 tempo) { AudioPlayer_SetTempo(this, tempo); }
    void SetVolume(u16 volume) { AudioPlayer_SetTrackVolume(this, volume, 0xFFFF); }
    void SetPitch(i16 pitch) { AudioPlayer_SetTrackPitch(this, pitch, 0xFFFF); }
    void SetPan(i8 pan) { AudioPlayer_SetTrackPan(this, pan, 0xFFFF); }
    void SetModulation(u8 depth) { AudioPlayer_SetTrackModDepth(this, depth, 0xFFFF); }
    void SetLfoSpeed(u8 speed) { AudioPlayer_SetTrackLfoSpeed(this, speed, 0xFFFF); }
};

// 0x080123CC
EC void SECTION(".text.game_state_080123CC") GameScript_ConfigureMusic(GameStateScript & script, i32 mode, u32 songId, GameStateScriptMusicSettings const & settings)
{
    GameStateRuntime & runtime = *script.runtime;
    reinterpret_cast<GameStateMusicPlayer *>(runtime.musicPlayer)->StartWithMode(mode, songId);
    GameStateMusicPlayer * music = reinterpret_cast<GameStateMusicPlayer *>(runtime.musicPlayer);
    music->ResetChannels();
    music->SetTempo(settings.tempo);
    music->SetVolume(settings.volume);
    music->SetPitch(settings.pitch);
    music->SetPan(settings.pan);
    music->SetModulation(settings.modulation);
    music->SetLfoSpeed(settings.lfoSpeed);
}

struct G1NpcSchedule;
EC void AdvanceNpcScheduleDailyCounter(G1NpcSchedule * schedule);

// 0x08016FA4
EC void SECTION(".text.game_state_08016FA4") GameScript_SetActorMapLocation(GameStateScript & script, i32 actorId, u32 mapId, i32 x, i32 y, i32 facing)
{
    GameStateRuntime & runtime = *script.runtime;
    u32 targetMapId = mapId;
    if (actorId <= 0x49)
    {
        AActorEntity * actor = runtime.scene->GetActor(actorId);
        if (actor != nullptr)
        {
            actor->SetLocation(ActorLocation(Location(targetMapId, x, y), facing));
            actor->vfunc_20();
        }
    }
    else
    {
        AEntity * entity = runtime.scene->GetActor(actorId);
        if (entity != nullptr)
        {
            entity->SetLocation(Location(targetMapId, x, y));
            entity->vfunc_20();
        }
    }
    if (actorId == 0)
    {
        if (targetMapId == 0x19)
            AdvanceNpcScheduleDailyCounter(reinterpret_cast<G1NpcSchedule *>(runtime.saveData + 0x1CD4));
        i32 mineFloor = 0;
        if (targetMapId - 0x35 <= 0xFE)
            mineFloor = targetMapId - 0x34;
        else if (targetMapId - 0x135 <= 0xFE)
            mineFloor = targetMapId - 0x134;
        if (mineFloor > 0)
            func_0800EECC(*reinterpret_cast<Farmer *>(runtime.saveData + 0x1BD8), mineFloor);
    }
}

// 0x080122E0
EC void SECTION(".text.game_state_080122E0") GameScript_WarpToLocation(GameStateScript & script, u32 mapId, i32 x, i32 y)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.scene->WarpToLocation(Location(mapId, x, y), true);
    func_080D7F08(runtime);
}

EC Bachelorette const * GetRosterBacheloretteByIdConst(void * data, u32 characterId);
EC void SetMessageWindowIndicator(void * dialogue, u32 level);
EC void ConfigureMessageWindowChoices(void * dialogue, char const * first, char const * second, char const * third, char const * fourth, char const * fifth, char const * sixth, void const * substitutions);
EC void ConfigureMessageWindowPromptChoices(void * dialogue, char const * prompt, char const * first, char const * second, char const * third, char const * fourth, void const * substitutions);

// 0x08012860
EC void SECTION(".text.game_state_08012860") GameScript_StartPromptChoices(GameStateScript & script, char const * prompt, char const * first, char const * second, char const * third, char const * fourth)
{
    GameStateRuntime & runtime = *script.runtime;
    if (runtime.dialogue.Get() != nullptr && first != nullptr && second != nullptr)
    {
        if (third == nullptr)
            ConfigureMessageWindowPromptChoices(runtime.dialogue.Get(), prompt, first, second, nullptr, nullptr, runtime.GetTextVariables());
        else if (fourth == nullptr)
            ConfigureMessageWindowPromptChoices(runtime.dialogue.Get(), prompt, first, second, third, nullptr, runtime.GetTextVariables());
        else
            ConfigureMessageWindowPromptChoices(runtime.dialogue.Get(), prompt, first, second, third, fourth, runtime.GetTextVariables());
        runtime.pendingCommand = 0x14;
    }
}

// 0x080128D0
EC void SECTION(".text.game_state_080128D0") GameScript_StartChoices(GameStateScript & script, char const * first, char const * second, char const * third, char const * fourth, char const * fifth, char const * sixth)
{
    GameStateRuntime & runtime = *script.runtime;
    if (runtime.dialogue.Get() != nullptr && first != nullptr && second != nullptr)
    {
        if (third == nullptr)
            ConfigureMessageWindowChoices(runtime.dialogue.Get(), first, second, nullptr, nullptr, nullptr, nullptr, runtime.GetTextVariables());
        else if (fourth == nullptr)
            ConfigureMessageWindowChoices(runtime.dialogue.Get(), first, second, third, nullptr, nullptr, nullptr, runtime.GetTextVariables());
        else if (fifth == nullptr)
            ConfigureMessageWindowChoices(runtime.dialogue.Get(), first, second, third, fourth, nullptr, nullptr, runtime.GetTextVariables());
        else if (sixth == nullptr)
            ConfigureMessageWindowChoices(runtime.dialogue.Get(), first, second, third, fourth, fifth, nullptr, runtime.GetTextVariables());
        else
            ConfigureMessageWindowChoices(runtime.dialogue.Get(), first, second, third, fourth, fifth, sixth, runtime.GetTextVariables());
        runtime.pendingCommand = 0x14;
    }
}

// 0x08016AFC
EC i32 SECTION(".text.game_state_08016AFC") GameScript_GetFarmAnimalEventIndex(GameStateScript & script, u32 kind)
{
    GameStateRuntime & runtime = *script.runtime;
    i32 index;
    u32 eventFlag;
    switch (kind)
    {
    case 0:
        if ((runtime.saveData[0x219A] & 2) != 0 || (runtime.saveData[0x21A1] & 4) != 0)
            goto firstAnimal;
    default:
        index = -1;
        break;
    case 4:
    {
        eventFlag = runtime.saveData[0x219C] & 0x10;
        index = -1;
        if (eventFlag != 0)
        {
        firstAnimal:
            index = 0;
        }
        break;
    }
    case 1:
    case 2:
        index = reinterpret_cast<Barn const *>(runtime.saveData + 0x5F0)->method_0800D058();
        break;
    case 3:
        index = reinterpret_cast<Coop const *>(runtime.saveData + 0x410)->GetUnkEnt();
        break;
    }
    return index;
}


// 0x08016834
EC void SECTION(".text.game_state_08016834") GameScript_ClearFarmAnimalActor(GameStateScript & script, u32 kind, u32 index)
{
    GameStateRuntime & runtime = *script.runtime;
    switch (kind)
    {
    case 0:
        runtime.scene->RemoveActor(0x2C);
        break;
    case 4:
        runtime.scene->RemoveActor(0x2B);
        break;
    case 1:
    case 2:
        reinterpret_cast<Barn *>(runtime.saveData + 0x5F0)->method_0800D80C(index);
        runtime.scene->RemoveActor(index + 0x36);
        break;
    case 3:
        reinterpret_cast<Coop *>(runtime.saveData + 0x410)->method_0800CB7C(index);
        runtime.scene->RemoveActor(index + 0x2E);
        break;
    }
}

// 0x080168D4
EC void GameScript_RelocateSelectedAnimalActor(GameStateScript & script, u32 kind);

// 0x08014318
EC void GameScript_SetGameTime(GameStateScript & script, GameTime const & time);

struct GameStateMapOverlay;

struct GameStateMapCleanupResource
{
    u32 resourceState[9];
    virtual ~GameStateMapCleanupResource();
};

template <u32 Count>
struct GameStateMapCleanupOwners
{
    SmartPtr<GameStateMapCleanupResource> entries[Count];
};

struct GameStateMapCleanupSlot
{
    u32 metadata[2];
    SmartPtr<GameStateMapCleanupResource> resource;
};

struct GameStateMapCleanupSlots
{
    GameStateMapCleanupSlot entries[4];
};

typedef GameStateMapCleanupOwners<6> GameStateMapSpecialResources;
typedef GameStateMapCleanupOwners<2> GameStateMapResourcePair;

struct GameStateMapSceneCleanupView
{
    void const * vtable;
    u8 contents[0x1034];
    u8 * saveData;
    STRUCT_PAD(0x103C, 0x1050);
    u8 * tileWorkBuffer;
    STRUCT_PAD(0x1054, 0x1058);
    SmartPtr<GameStateMapCleanupResource> resource1058;
    SmartPtr<GameStateMapCleanupResource> resource105C;
    STRUCT_PAD(0x1060, 0x1064);
    SmartPtr<GameStateMapCleanupResource> resource1064;
    SmartPtr<GameStateMapCleanupResource> resource1068;
    GameStateMapResourcePair resourcePair106C;
    GameStateMapResourcePair resourcePair1074;
    GameStateMapOverlay * overlay;
    GameStateMapCleanupSlots resourceSlots;
    GameStateMapSpecialResources specialResources;
    bool applyExitPenalty;
    bool skipExitPenaltyEvent;
};

typedef char GameStateMapSceneCleanupLayout[
    sizeof(GameStateMapCleanupResource) == 0x28 &&
    offsetof(GameStateMapSceneCleanupView, saveData) == 0x1038 &&
    offsetof(GameStateMapSceneCleanupView, specialResources) == 0x10B0 &&
    offsetof(GameStateMapSceneCleanupView, skipExitPenaltyEvent) == 0x10C9 ? 1 : -1];

// 0x080179CC
EC void GameStateMapScene_Destroy(GameStateMapSceneCleanupView * scene, u32 flags);

// 0x080127A0
EC void SECTION(".text.game_state_080127A0") GameScript_SetDialogueLoveIndicator(GameStateScript & script, u32 characterId)
{
    GameStateRuntime & runtime = *script.runtime;
    if (runtime.dialogue.Get() != nullptr && (i32(u32(runtime.saveData[0x2210]) << 29) < 0) && characterId != 0x1F)
    {
        Bachelorette const * character = GetRosterBacheloretteByIdConst(runtime.saveData + 0x1CD4, characterId);
        if (character != nullptr)
        {
            u32 love = character->GetLove();
            u32 indicator;
            if (love >= 60000)
                indicator = 6;
            else if (love >= 50000)
                indicator = 5;
            else if (love >= 40000)
                indicator = 4;
            else if (love >= 30000)
                indicator = 3;
            else if (love >= 20000)
                indicator = 2;
            else if (love >= 10000)
                indicator = 1;
            else
                indicator = 0;
            SetMessageWindowIndicator(runtime.dialogue.Get(), indicator);
        }
    }
}
struct GameStateVariant
{
    u32 reserved : 2;
    u32 variant : 6;
};

// 0x0801740C
EC u32 SECTION(".text.game_state_0801740C") GameStateVariant_GetColumn(GameStateVariant const & header)
{
    switch (header.variant)
    {
    default:
    case 27: case 31: case 35:
        return 0;
    case 28: case 32: case 36:
        return 1;
    case 29: case 33: case 37:
        return 2;
    case 30: case 34: case 38:
        return 3;
    }
}

// 0x0801746C
EC u32 SECTION(".text.game_state_0801746C") GameStateVariant_GetRow(GameStateVariant const & header)
{
    switch (header.variant)
    {
    default:
    case 27: case 28: case 29: case 30:
        return 0;
    case 31: case 32: case 33: case 34:
        return 1;
    case 35: case 36: case 37: case 38:
        return 2;
    }
}


struct __attribute__((packed, aligned(2))) GameStateHalfCounter
{
    u16 reserved;
    struct __attribute__((packed, aligned(2)))
    {
        u16 reserved : 2;
        u16 counter : 7;
    } progress;
};

// 0x08010F14
EC u32 SECTION(".text.game_state_08010F14") GameProgress_GetHalfCounter(GameStateHalfCounter const & header)
{
    return header.progress.counter;
}

// 0x08011498
EC u32 SECTION(".text.game_state_08011498") GameProgress_RaiseHalfCounter(GameStateHalfCounter & header, u32 minimum)
{
    if (minimum > 99)
        minimum = 99;
    u32 changed;
    if (header.progress.counter >= minimum)
        changed = 0;
    else
    {
        header.progress.counter = minimum;
        changed = 1;
    }
    return changed;
}

// 0x0801412C
EC u32 SECTION(".text.game_state_0801412C") GameScript_BeginPlayerAction48(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    u32 started;
    if (runtime.scene->GetActor(0)->vfunc_48())
    {
        runtime.pendingCommand = 0x19;
        started = 1;
    }
    else
        started = 0;
    return started;
}

// 0x08014BD8
EC void SECTION(".text.game_state_08014BD8") GameScript_SetNonPlayerActorMode(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    u32 actorId = 1;
    GameStateObject * & scene = runtime.scene;
    for (; actorId <= 0x23; actorId++)
    {
        GameStateActorEntity * actor = scene->GetActor(actorId);
        if (actor != nullptr)
            actor->vfunc_3C(3);
    }
}

// 0x08016784
EC u32 SECTION(".text.game_state_08016784") GameScript_FadeMusicIfPlaying(GameStateScript & script)
{
    u8 * music = script.runtime->musicPlayer;
    u32 started;
    if ((u8) AudioPlayer_IsPlaying(music))
    {
        AudioPlayer_FadeOut(music, 5);
        started = 1;
    }
    else
        started = 0;
    return started;
}

// 0x08016D34
EC void SECTION(".text.game_state_08016D34") GameScript_RemoveSceneActor(GameStateScript & script, u32 actorId)
{
    script.runtime->scene->RemoveActor(actorId);
}

// 0x08012454
EC void SECTION(".text.game_state_08012454") GameScript_StopMusic(GameStateScript & script)
{
    AudioPlayer_Stop(script.runtime->musicPlayer);
}

// 0x080124C4
EC void SECTION(".text.game_state_080124C4") GameScript_ResetAudio()
{
    Audio_StopAllPlayers();
}

// 0x0801274C
EC void SECTION(".text.game_state_0801274C") GameScript_ClearDialogueSpeakerName(GameStateScript & script)
{
    void * window = script.runtime->dialogue.Get();
    if (window != nullptr)
        ClearMessageWindowSpeakerName(window);
}

// 0x0801278C
EC void SECTION(".text.game_state_0801278C") GameScript_ClearDialoguePortrait(GameStateScript & script)
{
    void * window = script.runtime->dialogue.Get();
    if (window != nullptr)
        ClearMessageWindowPortrait(window);
}

// 0x0801284C
EC void SECTION(".text.game_state_0801284C") GameScript_ClearDialogueIndicator(GameStateScript & script)
{
    void * window = script.runtime->dialogue.Get();
    if (window != nullptr)
        ClearMessageWindowIndicator(window);
}

// 0x08012ACC
EC void SECTION(".text.game_state_08012ACC") GameScript_SetTextVariable(GameStateScript & script, u32 variableIndex, char const * text)
{
    SetGameScriptTextVariable(*script.runtime->textVariables, variableIndex, text);
}

// 0x08012BAC
EC void SECTION(".text.game_state_08012BAC") GameScript_SetPendingArgument(GameStateScript & script, u32 value)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.pendingArgument = value;
    runtime.pendingCommand = 0x15;
}

// 0x080142F0
EC void SECTION(".text.game_state_080142F0") GameScript_EnableSaveFlag(GameStateScript & script)
{
    script.runtime->saveData[0x34C4] = 1;
}

// 0x08014304
EC void SECTION(".text.game_state_08014304") GameScript_DisableSaveFlag(GameStateScript & script)
{
    script.runtime->saveData[0x34C4] = 0;
}

// 0x0801468C
EC SECTION(".text.game_state_0801468C") u8 * GameScript_GetSoundPlayers(GameStateScript & script)
{
    return reinterpret_cast<u8 *>(&script.runtime->soundPlayers);
}

// 0x080167CC
EC u32 SECTION(".text.game_state_080167CC") GameScript_IsMusicActive(GameStateScript & script)
{
    return AudioPlayer_IsPlaying(script.runtime->musicPlayer);
}

// 0x08016D80
EC void SECTION(".text.game_state_08016D80") GameScript_ClearReturnLocation()
{
    GameStateGlobalStatus & status = gActiveGameScene->status;
    status.hasReturnLocation = 0;
    status.mapId = 0x234;
}

// 0x08014034
EC void SECTION(".text.game_state_08014034") GameScript_ApplyRow15TilePatchVariant1(GameStateScript & script)
{
    script.runtime->scene->ApplyRow15TilePatchVariant1();
}

// 0x0801404C
EC void SECTION(".text.game_state_08014034") GameScript_ApplyRow15TilePatchVariant2(GameStateScript & script)
{
    script.runtime->scene->ApplyRow15TilePatchVariant2();
}

// 0x08014064
EC void SECTION(".text.game_state_08014034") GameScript_ApplyRow3TilePatchVariant1(GameStateScript & script)
{
    script.runtime->scene->ApplyRow3TilePatchVariant1();
}

// 0x0801407C
EC void SECTION(".text.game_state_08014034") GameScript_ApplyRow3TilePatchVariant2(GameStateScript & script)
{
    script.runtime->scene->ApplyRow3TilePatchVariant2();
}

// 0x08014094
EC void SECTION(".text.game_state_08014034") GameScript_ApplyRow2TilePatchVariant1(GameStateScript & script)
{
    script.runtime->scene->ApplyRow2TilePatchVariant1();
}

// 0x080140AC
EC void SECTION(".text.game_state_08014034") GameScript_ApplyRow2TilePatchVariant2(GameStateScript & script)
{
    script.runtime->scene->ApplyRow2TilePatchVariant2();
}

// 0x080140C4
EC void SECTION(".text.game_state_08014034") GameScript_ApplyMapTilePatchAtIndex(GameStateScript & script, u32 index)
{
    script.runtime->scene->ApplyMapTilePatchAtIndex(index);
}

// 0x080140DC
EC void SECTION(".text.game_state_08014034") GameScript_ApplyMapTilePatchInColumn(GameStateScript & script, u32 index)
{
    script.runtime->scene->ApplyMapTilePatchInColumn(index);
}

// 0x080140F4
EC void SECTION(".text.game_state_08014034") GameScript_InvokeSceneAction80(GameStateScript & script)
{
    script.runtime->scene->vfunc_80();
}

// 0x08014234
EC i32 SECTION(".text.game_state_08014234") GameScript_RemoveRecordPlayerAlbum(GameStateScript & script)
{
    return script.runtime->scene->RemoveRecordPlayerAlbum();
}

// 0x0801424C
EC i32 SECTION(".text.game_state_08014234") GameScript_SetRecordPlayerAlbum(GameStateScript & script, u32 albumId)
{
    return script.runtime->scene->SetRecordPlayerAlbum(albumId);
}

// 0x080142B8
EC void SECTION(".text.game_state_080142B8") GameScript_StartMapInteraction(GameStateScript & script, u32 animation)
{
    script.runtime->scene->StartMapInteraction(animation);
}

// 0x08016BDC
EC void SECTION(".text.game_state_08016BDC") GameScript_ApplyMapTilePatchAtRow20(GameStateScript & script)
{
    script.runtime->scene->ApplyMapTilePatchAtRow20();
}

// 0x08016BF4
EC void SECTION(".text.game_state_08016BDC") GameScript_ApplyMapTilePatchAtRow23(GameStateScript & script)
{
    script.runtime->scene->ApplyMapTilePatchAtRow23();
}

// 0x080114F8
EC void SECTION(".text.game_state_080114F8") SaveProgress_ResetPrimaryFlags(GameStateHeader & header)
{
    header.flagA = 0;
    header.flagB = 1;
    header.flagC = 1;
    header.flagD = 1;
}

// 0x08011524
EC bool SECTION(".text.game_state_08011524") Selection_IsPreviousReady(GameStateSelection const & selection)
{
    return selection.previous != 0x10 && selection.delay == 0;
}

// 0x08011550
EC void SECTION(".text.game_state_08011550") Selection_ClearPrevious(GameStateSelection & selection)
{
    if (selection.previous == selection.current)
        selection.current = 0x10;
    selection.previous = 0x10;
}

// 0x08011568
EC void SECTION(".text.game_state_08011568") Selection_TickDelay(GameStateSelection & selection)
{
    if (selection.current != 0x10 && selection.previous != 0x10 && selection.delay != 0)
        selection.delay--;
}

// 0x080122A0
EC void SECTION(".text.game_state_080122A0") GameScript_StopActorMovement(GameStateScript & script, u32 actorId)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    if (entity != nullptr)
        func_080323C8(*entity);
}

// 0x080142D4
EC void SECTION(".text.game_state_080142D4") GameScript_ClearMapInteraction(GameStateScript & script)
{
    script.runtime->scene->ClearMapInteraction();
}

// 0x08014694
EC void SECTION(".text.game_state_08014694") GameScript_InvokeSceneAction150(GameStateScript & script, u32 value)
{
    script.runtime->scene->vfunc_150(value);
}

// 0x080146B0
EC void SECTION(".text.game_state_080146B0") GameScript_InvokeSceneAction154(GameStateScript & script, u32 value)
{
    script.runtime->scene->vfunc_154(value);
}

// 0x08016BA4
EC void SECTION(".text.game_state_08016BA4") GameScript_SetMapTileVariant(GameStateScript & script, u32 index)
{
    script.runtime->scene->SetMapTileVariant(index);
}

// 0x08016BC0
EC void SECTION(".text.game_state_08016BC0") GameScript_ClearMapTileVariant(GameStateScript & script, u32 index)
{
    script.runtime->scene->ClearMapTileVariant(index);
}

// 0x08016C10
EC void SECTION(".text.game_state_08016C10") GameScript_QueueMapEvent1D(GameStateScript & script)
{
    script.runtime->scene->QueueMapEvent1D();
}

// 0x08016C2C
EC void SECTION(".text.game_state_08016C2C") GameScript_CreateSpecialActorEffectSprite(GameStateScript & script)
{
    CreateSideEntityEffectSprite(script.runtime->scene->GetActor(0x5D));
}

// 0x08016C6C
EC void SECTION(".text.game_state_08016C6C") GameScript_ClearSpecialActorEffectSprite(GameStateScript & script)
{
    ClearSideEntityEffectSprite(script.runtime->scene->GetActor(0x5D));
}

// 0x08016EF0
EC void SECTION(".text.game_state_08016EF0") GameScript_InvokeSceneAction164(GameStateScript & script, u32 value)
{
    script.runtime->scene->vfunc_164(value);
}

// 0x0801410C
EC u32 SECTION(".text.game_state_0801410C") GameScript_GetPlayerProperty40(GameStateScript & script)
{
    return script.runtime->scene->GetActor(0)->vfunc_40();
}

// 0x08014214
EC void SECTION(".text.game_state_08014214") GameScript_InvokePlayerAction54(GameStateScript & script)
{
    script.runtime->scene->GetActor(0)->vfunc_54();
}

// 0x08014D5C
EC u32 SECTION(".text.game_state_08014D5C") GameScript_GetPlayerProperty78(GameStateScript & script)
{
    return script.runtime->scene->GetActor(0)->vfunc_78();
}

// 0x08014D7C
EC u32 SECTION(".text.game_state_08014D7C") GameScript_GetPlayerProperty44(GameStateScript & script)
{
    return script.runtime->scene->GetActor(0)->vfunc_44();
}

// 0x08015950
EC u32 SECTION(".text.game_state_08015950") GameScript_GetPlayerProperty84(GameStateScript & script)
{
    return script.runtime->scene->GetActor(0)->vfunc_84();
}

// 0x08015970
EC u32 SECTION(".text.game_state_08015970") GameScript_GetPlayerProperty88(GameStateScript & script)
{
    return script.runtime->scene->GetActor(0)->vfunc_88();
}

// 0x08015990
EC u32 SECTION(".text.game_state_08015990") GameScript_GetPlayerProperty8C(GameStateScript & script)
{
    return script.runtime->scene->GetActor(0)->vfunc_8C();
}

// 0x080167AC
EC u32 SECTION(".text.game_state_080167AC") GameScript_GetPlayerProperty90(GameStateScript & script)
{
    return script.runtime->scene->GetActor(0)->vfunc_90();
}

// 0x08016CAC
EC void SECTION(".text.game_state_08016CAC") GameScript_InvokePlayerActionA8(GameStateScript & script)
{
    script.runtime->scene->GetActor(0)->vfunc_A8();
}

// 0x08016CCC
EC void SECTION(".text.game_state_08016CCC") GameScript_InvokePlayerActionAC(GameStateScript & script)
{
    script.runtime->scene->GetActor(0)->vfunc_AC();
}

// 0x08016E7C
EC void SECTION(".text.game_state_08016E7C") GameScript_InvokePlayerAction6C(GameStateScript & script)
{
    script.runtime->scene->GetActor(0)->vfunc_6C();
}

// 0x08016C48
EC void SECTION(".text.game_state_08016C48") GameScript_StartSpecialActorTransition(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    BeginSideEntityTransition(runtime.scene->GetActor(0x5D));
    runtime.pendingCommand = 0x1C;
}

// 0x08016C88
EC void SECTION(".text.game_state_08016C88") GameScript_CreateSpecialActorParticles(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    CreateSideEntityParticleSprites(runtime.scene->GetActor(0x5D));
    runtime.pendingCommand = 0x1B;
}

// 0x0801221C
EC void SECTION(".text.game_state_0801221C") GameScript_ClearActorMap(GameStateScript & script, u32 actorId)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    if (entity != nullptr)
        entity->SetMap(0x234);
}

// 0x080120A4
EC i32 SECTION(".text.game_state_080120A4") GameScript_GetActorX(GameStateScript & script, u32 actorId)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    i32 x = 0;
    if (entity != nullptr)
        x = entity->x_q16;
    return x >> 16;
}

// 0x080120C4
EC i32 SECTION(".text.game_state_080120C4") GameScript_GetActorY(GameStateScript & script, u32 actorId)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    i32 y = 0;
    if (entity != nullptr)
        y = entity->y_q16;
    return y >> 16;
}

// 0x08012114
EC u32 SECTION(".text.game_state_08012114") GameScript_GetActorFacing(GameStateScript & script, u32 actorId)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    u32 facing;
    if (entity != nullptr)
        facing = entity->facing;
    else
        facing = 0;
    return facing;
}

// 0x08012134
EC u32 SECTION(".text.game_state_08012134") GameScript_GetActorMap(GameStateScript & script, u32 actorId)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    u32 map = 0x234;
    if (entity != nullptr)
        map = entity->location_map;
    return map;
}

// 0x08012154
EC void SECTION(".text.game_state_08012154") GameScript_SetActorStateByte(GameStateScript & script, u32 actorId, u32 value)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    if (entity != nullptr)
        entity->unk_21 = value;
}

// 0x08012174
EC void SECTION(".text.game_state_08012174") GameScript_MoveActorX(GameStateScript & script, u32 actorId, u32 target, i32 speed)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    if (entity != nullptr)
        func_08032308(*entity, target, speed);
}

// 0x0801219C
EC void SECTION(".text.game_state_0801219C") GameScript_MoveActorY(GameStateScript & script, u32 actorId, u32 target, i32 speed)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    if (entity != nullptr)
        func_0803233C(*entity, target, speed);
}

// 0x080122BC
EC void SECTION(".text.game_state_080122BC") GameScript_SetActorMovementValue(GameStateScript & script, u32 actorId, u32 value)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    if (entity != nullptr)
        func_080323E0(*entity, value);
}

// 0x080141C4
EC void SECTION(".text.game_state_080141C4") GameScript_BeginPlayerAction4C(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.scene->GetActor(0)->vfunc_4C();
    runtime.pendingCommand = 0x19;
}

// 0x080141EC
EC void SECTION(".text.game_state_080141EC") GameScript_BeginPlayerAction50(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.scene->GetActor(0)->vfunc_50();
    runtime.pendingCommand = 0x19;
}

// 0x08014C0C
EC void SECTION(".text.game_state_08014C0C") GameScript_InvokePlayerAction64(GameStateScript & script, u32 value_1, u32 value_2)
{
    script.runtime->scene->GetActor(0)->vfunc_64(value_1, value_2);
}

// 0x080158F8
EC void SECTION(".text.game_state_080158F8") GameScript_BeginPlayerAction7C(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.scene->GetActor(0)->vfunc_7C();
    runtime.pendingCommand = 0x19;
}

// 0x08016E9C
EC void SECTION(".text.game_state_08016E9C") GameScript_BeginPlayerAction70(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.scene->GetActor(0)->vfunc_70();
    runtime.pendingCommand = 0x19;
}

// 0x0801238C
EC void SECTION(".text.game_state_0801238C") GameScript_InvokeSceneAction24(GameStateScript & script, u32 value_1, u32 value_2, u32 value_3)
{
    script.runtime->scene->vfunc_24(value_1, value_2, value_3);
}

// 0x08012B04
EC void SECTION(".text.game_state_08012B04") GameScript_LoadScript(GameStateScript & script, int scriptId, int argument)
{
    GameStateRuntime & runtime = *script.runtime;
    gActiveGameScene->engine.LoadById(scriptId, argument);
    func_080D7F3C(runtime);
}

// 0x08012AA4
EC void SECTION(".text.game_state_08012AA4") GameScript_SetIntegerTextVariable(GameStateScript & script, u32 variableIndex, i32 value)
{
    GameStateRuntime & runtime = *script.runtime;
    char formattedNumber[50];
    FormatMessageInteger(value, formattedNumber, 0);
    SetGameScriptTextVariable(*runtime.textVariables, variableIndex, formattedNumber);
}

// 0x08012ADC
EC void SECTION(".text.game_state_08012ADC") GameScript_SetPaddedIntegerTextVariable(GameStateScript & script, u32 variableIndex, i32 value, u32 minimumDigits)
{
    GameStateRuntime & runtime = *script.runtime;
    char formattedNumber[50];
    FormatMessageInteger(value, formattedNumber, minimumDigits);
    SetGameScriptTextVariable(*runtime.textVariables, variableIndex, formattedNumber);
}

// 0x08016F0C
EC void SECTION(".text.game_state_08016F0C") GameScript_InvokePlayerAction9C(GameStateScript & script, u8 value)
{
    script.runtime->scene->GetActor(0)->vfunc_9C(value);
}

// 0x08014290
EC void SECTION(".text.game_state_08014290") GameScript_ResetActorAction38(GameStateScript & script, u32 actorId)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    if (entity != nullptr)
        entity->vfunc_38(0);
}

// 0x080121C4
EC void SECTION(".text.game_state_080121C4") GameScript_WaitForActor(GameStateScript & script, u32 actorId)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.waitActor = runtime.scene->GetActor(actorId);
    if (runtime.waitActor != nullptr)
        runtime.pendingCommand = 0x16;
}

// 0x080123A4
EC void SECTION(".text.game_state_080123A4") GameScript_WaitForScene(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    if (!runtime.scene->vfunc_28())
        runtime.pendingCommand = 0x17;
}

// 0x080126BC
EC void SECTION(".text.game_state_080126BC") GameScript_StartDialogueWithDefaultSpeed(GameStateScript & script, char const * text)
{
    GameStateRuntime & runtime = *script.runtime;
    if (runtime.dialogue.Get() != nullptr)
    {
        SetMessageWindowTextOptions(runtime.dialogue.Get(), text, 0x40, runtime.textVariables);
        runtime.pendingCommand = 0x11;
    }
}

// 0x08014198
EC bool SECTION(".text.game_state_08014198") GameScript_BeginPlayerAction60(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.scene->GetActor(0)->vfunc_60();
    runtime.pendingCommand = 0x19;
    return true;
}

// 0x080158CC
EC void SECTION(".text.game_state_080158CC") GameScript_BeginPlayerAction80(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.scene->GetActor(0)->vfunc_80();
    runtime.pendingCommand = 0x19;
}

// 0x08016F34
EC void SECTION(".text.game_state_08016F34") GameScript_BeginPlayerAction74(GameStateScript & script, u32 value)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.scene->GetActor(0)->vfunc_74(value);
    runtime.pendingCommand = 0x19;
}

// 0x080121EC
EC void SECTION(".text.game_state_080121EC") GameScript_SetActorAnimation(GameStateScript & script, u32 actorId, u32 animationId)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    if (entity != nullptr && entity->anim_id != animationId)
        entity->SetAnim(animationId);
}

// 0x08012274
EC void SECTION(".text.game_state_08012274") GameScript_SetActorMovementOption(GameStateScript & script, u32 actorId, u32 value, u32 enabled)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    if (entity != nullptr)
        func_08032384(*entity, value, enabled != 0);
}

// 0x08014264
EC void SECTION(".text.game_state_08014264") GameScript_InvokeActorAction38(GameStateScript & script, u32 actorId, u32 value)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    if (entity != nullptr)
        entity->vfunc_38(value);
}

// 0x080120E4
EC void SECTION(".text.game_state_080120E4") GameScript_SetActorFacing(GameStateScript & script, u32 actorId, u32 facing)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    if (entity != nullptr && entity->facing != facing)
        entity->SetAnimFacing(facing);
}

// 0x08012658
EC void SECTION(".text.game_state_08012658") GameScript_ResetDialogue(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    if (runtime.dialogue.Get() != nullptr)
    {
        CloseMessageWindow(runtime.dialogue.Get());
        func_080D7F88(runtime);
        func_080D7F74(runtime);
        func_080D7F60(runtime);
        runtime.pendingCommand = 0xF;
    }
}

// 0x0801268C
EC void SECTION(".text.game_state_0801268C") GameScript_StartDialogue(GameStateScript & script, char const * text, u32 speed)
{
    GameStateRuntime & runtime = *script.runtime;
    if (runtime.dialogue.Get() != nullptr)
    {
        SetMessageWindowTextOptions(runtime.dialogue.Get(), text, (u16) speed, runtime.textVariables);
        runtime.pendingCommand = 0x10;
    }
}

// 0x0801589C
EC void SECTION(".text.game_state_0801589C") GameScript_BeginPlayerAction68(GameStateScript & script, u16 value)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.scene->GetActor(0)->vfunc_68(value);
    runtime.pendingCommand = 0x19;
}

// 0x08014164
EC bool SECTION(".text.game_state_08014164") GameScript_BeginPlayerAction5C(GameStateScript & script, u32 value, u8 enabled)
{
    GameStateRuntime & runtime = *script.runtime;
    runtime.scene->GetActor(0)->vfunc_5C(value, enabled);
    runtime.pendingCommand = 0x19;
    return true;
}

// 0x08015920
EC u32 SECTION(".text.game_state_08015920") GameScript_GetFarmActorProperty(GameStateScript & script, u32 actorId)
{
    GameStateRuntime & runtime = *script.runtime;
    if (actorId - 0x24 <= 6)
    {
        AActorEntity * entity = runtime.scene->GetActor(actorId);
        if (entity != nullptr)
            return IsSpriteWorkerOutOfTasks(*entity);
    }
    return 0;
}

// 0x08016EC4
EC void SECTION(".text.game_state_08016EC4") GameScript_InvokeSceneAction160(GameStateScript & script, u32 value_1, u32 value_2, u32 value_3, u32 value_4, u16 value_5)
{
    script.runtime->scene->vfunc_160(value_1, value_2, value_3, value_4, value_5);
}

// 0x08014D30
EC void SECTION(".text.game_state_08014D30") GameScript_RemoveHorse(GameStateScript & script, u32 value)
{
    GameStateRuntime & runtime = *script.runtime;
    if (value == 0)
    {
        runtime.scene->RemoveActor(0x2C);
        reinterpret_cast<Farm *>(runtime.saveData + 0x14)->RemoveHorse();
    }
}

// 0x080146CC
EC void SECTION(".text.game_state_080146CC") GameScript_BeginEggIncubation(GameStateScript & script, u32 index)
{
    GameStateRuntime & runtime = *script.runtime;
    reinterpret_cast<Coop *>(runtime.saveData + 0x410)->BeginIncubation(index);
    runtime.scene->vfunc_AC(index);
}

// 0x080115A8
EC u32 SECTION(".text.game_state_080115A8") SaveData_GetSize()
{
    return 0x34FC;
}

// 0x08011464
EC u32 SECTION(".text.game_state_08011464") SaveProgress_RaiseWordCounter(GameStateWordHeader & header, u32 value)
{
    if (value > 99)
        value = 99;
    u32 result;
    if (header.counter >= value)
        result = 0;
    else
    {
        header.counter = value;
        result = 1;
    }
    return result;
}

// 0x080114C8
EC u32 SECTION(".text.game_state_080114C8") SaveProgress_RaiseByteCounter(GameStateHeader & header, u32 value)
{
    if (value > 99)
        value = 99;
    u32 result;
    if (header.counter >= value)
        result = 0;
    else
    {
        header.counter = value;
        result = 1;
    }
    return result;
}

// 0x08012720
EC void SECTION(".text.game_state_08012720") GameScript_SetDialogueSpeakerName(GameStateScript & script, char const * speakerName)
{
    GameStateRuntime & runtime = *script.runtime;
    if (runtime.dialogue.Get() != nullptr && (i32(u32(runtime.saveData[0x2210]) << 28) < 0))
        SetMessageWindowSpeakerName(runtime.dialogue.Get(), speakerName);
}

// 0x08012760
EC void SECTION(".text.game_state_08012760") GameScript_SetDialoguePortrait(GameStateScript & script, u32 portraitId)
{
    GameStateRuntime & runtime = *script.runtime;
    if (runtime.dialogue.Get() != nullptr && (i32(u32(runtime.saveData[0x2210]) << 29) < 0))
        SetMessageWindowPortrait(runtime.dialogue.Get(), portraitId);
}

// 0x080126E4
EC void SECTION(".text.game_state_080126E4") GameScript_SetDialogueCharacterName(GameStateScript & script, u32 characterId)
{
    GameStateRuntime & runtime = *script.runtime;
    void * window = runtime.dialogue.Get();
    if (window != nullptr && (i32(u32(runtime.saveData[0x2210]) << 28) < 0))
        SetMessageWindowSpeakerName(window, (char const *) GetRosterNpcName(runtime.saveData + 0x1CD4, characterId));
}

// 0x0801223C
EC void SECTION(".text.game_state_0801223C") GameScript_OffsetActorPosition(GameStateScript & script, u32 actorId, i32 deltaX, i32 deltaY)
{
    AActorEntity * entity = script.runtime->scene->GetActor(actorId);
    if (entity != nullptr)
    {
        i32 currentX = reinterpret_cast<i16 const *>(&entity->x_q16)[1];
        i32 currentY = reinterpret_cast<i16 const *>(&entity->y_q16)[1];
        currentX += deltaX;
        currentY += deltaY;
        ShowSpriteWorkerEffect(*entity, reinterpret_cast<u8 *>(entity)[0x30], currentX, currentY);
    }
}

// 0x08016D48
EC void SECTION(".text.game_state_08016D48") GameScript_SaveReturnLocation(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    AActorEntity * entity = runtime.scene->GetActor(0);
    GameStateGlobalStatus & status = gActiveGameScene->status;
    u16 map = entity->location_map;
    u8 * state = runtime.saveData;
    status.hasReturnLocation = 1;
    status.mapId = map;
    status.mapFlags = *reinterpret_cast<u16 *>(state + 0x12);
}

// 0x08016F60
EC void SECTION(".text.game_state_08016F60") GameScript_UpdateActiveActors(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    u32 actorId = 0x2E;
    GameStateObject * & object = runtime.scene;
    for (; actorId <= 0x45; actorId++)
    {
        GameStateActiveEntity * entity = reinterpret_cast<GameStateActiveEntity *>(object->GetActor(actorId));
        if (entity != nullptr && entity->vfunc_5C())
            entity->vfunc_7C();
    }
}

// 0x08011FE8
EC SECTION(".text.game_state_08011FE8") void GameScript_Destroy(GameStateScript & script, u32 flags)
{
    script.vtable = reinterpret_cast<u32>(vtable_unk_080E5E64);
    gActiveGameScene->engine.ClearUnk();
    delete script.runtime;
    Scene_DestroyBase(script, flags);
}

// 0x08012064
EC SECTION(".text.game_state_08012064") void GameScript_StartRuntimeOperation(GameStateScript & script, i32 id, u32 value_2, u32 value_3, u32 value_4)
{
    GameStateRuntime & runtime = *script.runtime;
    u32 value_1 = runtime.scene->vfunc_14();
    func_080D7F9C(runtime, id, value_1, value_2, value_3, value_4);
}

// 0x08016CEC
EC SECTION(".text.game_state_08016CEC") void GameScript_ActivateActor(GameStateScript & script, u32 actorId)
{
    GameStateObject * & object = script.runtime->scene;
    object->CreateActor(actorId);
    AActorEntity * entity = object->GetActor(actorId);
    if (entity != nullptr && !entity->vfunc_28())
        entity->vfunc_20();
}

// 0x080167DC
EC SECTION(".text.game_state_080167DC") void GameScript_RefreshActor23(GameStateScript & script)
{
    GameStateRuntime & runtime = *script.runtime;
    ActivateRosterNamedNpc(runtime.saveData + 0x1CD4, runtime.saveData + 0x11);
    GameStateObject * & object = runtime.scene;
    object->CreateActor(0x23);
    AActorEntity * entity = object->GetActor(0x23);
    if (entity != nullptr && !entity->vfunc_28())
        entity->vfunc_20();
}

// 0x08012464
EC SECTION(".text.game_state_08012464") void GameScript_PlaySound(GameStateScript & script, i32 mode, u32 songId)
{
    GameStateAudioList & players = script.runtime->soundPlayers;
    void ** cursor = players.begin;
    void ** end = players.end;
    void * player;
    if (cursor != end)
    {
        do
        {
            if (!(u8) AudioPlayer_IsPlaying(*cursor))
            {
                player = *cursor;
                goto play;
            }
            cursor++;
        } while (cursor != end);
    }
    player = end[-1];
play:
    u16 song = songId;
    switch (mode)
    {
    case 0:
        AudioPlayer_StartSong(player, song);
        break;
    case 1:
        AudioPlayer_StartSongIfStoppedOrChanged(player, song);
        break;
    case 2:
        AudioPlayer_StartOrResumeSong(player, song);
        break;
    }
}
