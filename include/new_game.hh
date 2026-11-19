#ifndef NEW_GAME_HH
#define NEW_GAME_HH

#include "prelude.h"
#include "scene.hh"

struct NewGameSceneTransitionOwner
{
    AUnk_0800080C * transition;
    ~NewGameSceneTransitionOwner() { delete transition; }
    AUnk_0800080C * Release()
    {
        AUnk_0800080C * releasedTransition = transition;
        transition = nullptr;
        return releasedTransition;
    }
};

struct NewGameChoiceDialogTaskLifetime
{
    u32 renderContext;
    void * vtable;
    STRUCT_PAD(0x0008, 0x1A2C);
    void * dialog;
};

struct NameEntrySceneTaskLifetime
{
    u32 renderContext;
    void * vtable;
    STRUCT_PAD(0x0008, 0x45F8);
    void * dialog;
};

struct NewGameRenderTask
{
    u32 renderContext;
    virtual ~NewGameRenderTask();
};

struct NewGameSceneTaskOwner
{
    void * vtable;
    NewGameRenderTask * task;
};

struct NameEntrySceneSpriteAttributes
{
    u16 y : 8;
    u16 displayFlags : 6;
    u16 shape : 2;
    u16 x : 9;
    u16 matrix : 5;
    u16 size : 2;
};

struct NameEntrySceneTask
{
    STRUCT_PAD(0x0000, 0x45F0);
    u32 textSound;
    u32 inputSound;
    void * dialog;
    u32 promptTextAddress;
    u32 keyboardPage;
    i8 cursorColumn;
    i8 cursorRow;
    STRUCT_PAD(0x4606, 0x4608);
    union
    {
        u32 cursorPositionAndShape;
        NameEntrySceneSpriteAttributes cursorAttributes;
    };
    u16 cursorTileAndPalette;
    STRUCT_PAD(0x460E, 0x4610);
    u8 charIndex;
    STRUCT_PAD(0x4611, 0x4614);
    union
    {
        u32 caretPositionAndShape;
        NameEntrySceneSpriteAttributes caretAttributes;
    };
    u16 caretTileAndPalette;
    STRUCT_PAD(0x461A, 0x461C);
    // Twelve single-byte slots; underscores mark unused positions.
    char enteredName[12];
    u8 nameTerminator;
    char confirmedName[13];
    u8 finished;
    u8 keyboardDirty;
    u8 cancelPending;
    u8 acceptPending;
    STRUCT_PAD(0x463A, 0x463C);
    u32 confirmationMode;
};

struct NewGameChoiceDialogTask
{
    STRUCT_PAD(0x0000, 0x1A2C);
    void * dialog;
    u32 textSound;
    STRUCT_PAD(0x1A34, 0x1A74);
    u8 closed;
    u8 choiceMade;
    u8 accepted;
    void MarkChoiceMade() { choiceMade = 1; }
    void SetAccepted(bool acceptedChoice) { accepted = acceptedChoice; }
};

struct NewGameMenuTask
{
    struct SaveSlotSummary
    {
        char text[128];
        bool IsEmpty() const { return text[0] == 0; }
        char const * CStr() const { return text; }
    };

    STRUCT_PAD(0x00, 0x80);
    u32 selectedItem;
    // Diary mode, save slot, controls, clock, portrait, and speaker name.
    u8 optionValues[6];
    STRUCT_PAD(0x8A, 0x98);
    SaveSlotSummary saveSummaries[2];
};

struct NewGameOamSprite
{
    // OAM attr0/attr1 are copied together; attr2 selects tile, priority, and palette.
    union
    {
        u32 positionAndShape;
        struct
        {
            u32 y : 8;
            u32 objectMode : 2;
            u32 displayFlags : 6;
            u32 x : 9;
            u32 transformAndSize : 7;
        } attributes;
    };
    u16 tileAndPalette;
    NewGameOamSprite & operator=(NewGameOamSprite const & source)
    {
        positionAndShape = source.positionAndShape;
        tileAndPalette = source.tileAndPalette;
        return *this;
    }
};

struct NewGameOamSpriteBuffer
{
    u8 spriteCount;
    STRUCT_PAD(0x01, 0x04);
    NewGameOamSprite sprites[128];

    NewGameOamSprite & SpriteAt(unsigned int index)
    {
        return *reinterpret_cast<NewGameOamSprite *>(reinterpret_cast<char *>(this) + (index * 8 + 4));
    }
    void WriteSprite(unsigned int index, NewGameOamSprite const & sprite)
    {
        SpriteAt(index & 127) = sprite;
    }
};

struct NewGameChoiceDialogScene
{
    void * vtable;
    NewGameChoiceDialogTask * task;
};

struct NameEntryScene
{
    void * vtable;
    NameEntrySceneTask * task;
};

EC void NameEntryScene_UpdateConfirmationDialog(NameEntrySceneTask * self);
EC void NewGameChoiceDialog_Update(NewGameChoiceDialogTask * self);
EC void NewGameMenu_UpdateDescription(NewGameMenuTask * self);
EC void NameEntryScene_SubmitSprites(NameEntrySceneTask * self);
EC void NameEntryScene_UpdateCursor(NameEntrySceneTask * self);
EC void NewGameMenu_PositionOptionSprite(NewGameMenuTask * self, int option, NewGameOamSprite sprite);
EC void NewGameMenu_PositionCursorSprite(NewGameMenuTask * self, int option, NewGameOamSprite source);
EC void NewGameMenu_UpdateMessage(char * self);
EC NewGameSceneTaskOwner * NewGameMenuScene_Initialize(NewGameSceneTaskOwner * self);
EC NewGameSceneTaskOwner * NewGameMenuScene_InitializeWithTransition(NewGameSceneTaskOwner * self, void * gameData, NewGameSceneTransitionOwner & transitionOwner);
EC void NewGameChoiceDialog_DestroyTask(NewGameChoiceDialogTaskLifetime * self, int destructorFlags);
// The legacy assembly ABI carries packed birthday bits in the pointer-sized r2 argument.
EC NewGameSceneTaskOwner * NewGameChoiceDialogScene_Initialize(NewGameSceneTaskOwner * self, void * farmerName, void * packedBirthday, u32 farmNameAddress, u32 dogNameAddress);
EC void NameEntryScene_DestroyTask(NameEntrySceneTaskLifetime * self, int destructorFlags);
EC bool NameEntryScene_PrepareConfirmedName(NameEntrySceneTask * self);
EC void NewGameMenuScene_Destroy(NewGameSceneTaskOwner * self, int destructorFlags);
EC void * NewGameSetupScene_Initialize(void ** self);
EC void NewGameSetupScene_Destroy(void ** self, int destructorFlags);
EC void NewGameChoiceDialogScene_Destroy(NewGameSceneTaskOwner * self, int destructorFlags);
EC NewGameSceneTaskOwner * NameEntryScene_Initialize(NewGameSceneTaskOwner * self, void * promptText, u32 initialNameAddress);
EC void NameEntryScene_Destroy(NewGameSceneTaskOwner * self, int destructorFlags);
EC void NewGameChoiceDialog_BlankDisplay(void * self);
EC void NewGameChoiceDialog_Reset(NewGameChoiceDialogTask * self);
EC u8 NewGameChoiceDialogScene_GetResult(NewGameChoiceDialogScene * self);
EC void NameEntryScene_Reset(NameEntrySceneTask * self);
EC void NameEntryScene_DeleteCharacter(NameEntrySceneTask * self, u8 characterIndex);
EC char * NameEntryScene_GetName(NameEntryScene * self);
EC char * NameEntrySceneTask_GetEnteredName(NameEntrySceneTask * self);

#endif // NEW_GAME_HH
