#ifndef NEW_GAME_HH
#define NEW_GAME_HH

#include "prelude.h"
#include "scene.hh"

struct SceneTransitionOwner
{
    AUnk_0800080C * transition;
    ~SceneTransitionOwner() { delete transition; }
    AUnk_0800080C * Release()
    {
        AUnk_0800080C * releasedTransition = transition;
        transition = nullptr;
        return releasedTransition;
    }
};

struct ChoiceDialogTaskLifetime
{
    u32 renderContext;
    void * vtable;
    STRUCT_PAD(0x0008, 0x1A2C);
    void * dialog;
};

struct NameEntryTaskLifetime
{
    u32 renderContext;
    void * vtable;
    STRUCT_PAD(0x0008, 0x45F8);
    void * dialog;
};

struct RenderTask
{
    u32 renderContext;
    virtual ~RenderTask();
};

struct SceneTaskOwner
{
    void * vtable;
    RenderTask * task;
};

struct NameEntryTask
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
    u32 cursorPositionAndShape;
    u16 cursorTileAndPalette;
    STRUCT_PAD(0x460E, 0x4610);
    u8 charIndex;
    STRUCT_PAD(0x4611, 0x4614);
    u32 caretPositionAndShape;
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

struct ChoiceDialogTask
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

struct DiaryMenuTask
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

struct OamSprite
{
    // OAM attr0/attr1 are copied together; attr2 selects tile, priority, and palette.
    u32 positionAndShape;
    u16 tileAndPalette;
};

struct OamSpriteBuffer
{
    u8 spriteCount;
    STRUCT_PAD(0x01, 0x04);
    OamSprite sprites[128];

    OamSprite & SpriteAt(unsigned int index)
    {
        return *reinterpret_cast<OamSprite *>(reinterpret_cast<char *>(this) + (index * 8 + 4));
    }
};

struct ChoiceDialogScene
{
    void * vtable;
    ChoiceDialogTask * task;
};

struct NameEntryScene
{
    void * vtable;
    NameEntryTask * task;
};

EC void NameEntry_UpdateConfirmationDialog(NameEntryTask * self);
EC void ChoiceDialog_Update(ChoiceDialogTask * self);
EC void DiaryMenu_UpdateDescription(DiaryMenuTask * self);
EC void NameEntry_SubmitSprites(NameEntryTask * self);
EC void DiaryMenu_UpdateMessage(char * self);
EC SceneTaskOwner * InitializeDiaryMenuScene(SceneTaskOwner * self);
EC SceneTaskOwner * InitializeDiaryMenuSceneWithTransition(SceneTaskOwner * self, void * gameData, SceneTransitionOwner & transitionOwner);
EC void ChoiceDialog_DestroyTask(ChoiceDialogTaskLifetime * self, int destructorFlags);
EC SceneTaskOwner * InitializeChoiceDialogScene(SceneTaskOwner * self, void * summaryText, void * selectionContext, u32 detailTextAddress, u32 footerTextAddress);
EC void NameEntry_DestroyTask(NameEntryTaskLifetime * self, int destructorFlags);
EC bool NameEntry_PrepareConfirmedName(NameEntryTask * self);
EC void DestroyDiaryMenuScene(SceneTaskOwner * self, int destructorFlags);
EC void * InitializeNewGameSetupScene(void ** self);
EC void DestroyNewGameSetupScene(void ** self, int destructorFlags);
EC void DestroyChoiceDialogScene(SceneTaskOwner * self, int destructorFlags);
EC SceneTaskOwner * InitializeNameEntryScene(SceneTaskOwner * self, void * promptText, u32 initialNameAddress);
EC void DestroyNameEntryScene(SceneTaskOwner * self, int destructorFlags);
EC void ChoiceDialog_BlankDisplay(void * self);
EC void ChoiceDialog_Reset(ChoiceDialogTask * self);
EC u8 ChoiceDialog_GetResult(ChoiceDialogScene * self);
EC void NameEntry_Reset(NameEntryTask * self);
EC void NameEntry_DeleteCharacter(NameEntryTask * self, u8 characterIndex);
EC char * NameEntry_GetName(NameEntryScene * self);

#endif // NEW_GAME_HH
