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

private:
    struct Transfer
    {
        G1SceneOwner * source;
        AUnk_0800080C * value;

        Transfer(G1SceneOwner * source, AUnk_0800080C * value) : source(source), value(value) {}
    };

    // Adopt's transfer clears the slot before its destructor runs.
    G1SceneOwner() {}
    G1SceneOwner(Transfer const & transfer)
    {
        AUnk_0800080C * next = transfer.value;
        transfer.source->value = nullptr;
        value = next;
    }

public:
    static G1SceneOwner Adopt(AUnk_0800080C * next)
    {
        G1SceneOwner slot;
        return Transfer(&slot, next);
    }
};

struct G1SpriteMinigameResult : AUnk_0800080C
{
    u8 reserved[8];
    u8 succeeded;
};

struct G1SpriteFeedingSceneWrapper
{
    void const * vtable;
    void * game;
    AUnk_0800080C * next_scene;
};

EC G1SceneOwner RunSpriteFeedingMinigameScene(G1SpriteMinigameScene * self);

#endif
