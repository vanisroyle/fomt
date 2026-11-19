#include "new_game.hh"

#include <string.h>

extern char gDiaryMenuSceneVtable[] __asm__("vtable_unk_080E5A68");
extern char gNewGameSetupSceneVtable[] __asm__("vtable_unk_080E5A88");
extern char gChoiceDialogSceneVtable[] __asm__("vtable_unk_080E5AA4");
extern char gNameEntrySceneVtable[] __asm__("vtable_unk_080E5AC0");
extern char gNewGameTransitionVtable[] __asm__("vtable_unk_080E5A3C");
extern char gChoiceDialogTaskVtable[] __asm__("vtable_unk_080E5A98");
extern char gNameEntryTaskVtable[] __asm__("vtable_unk_080E5AB4");

EC void * __builtin_new(size_t size);
EC RenderTask * CreateNameEntryTask(void * storage, void * promptText, u32 initialNameAddress) __asm__("func_08005A58");
EC void DestroySceneBase(void * self, int destructorFlags) __asm__("func_080007EC");
EC RenderTask * CreateDiaryMenuTask(void * storage, void * gameData, void * transitionOwner) __asm__("func_080041DC");
EC RenderTask * CreateChoiceDialogTask(void * storage, void * summaryText, void * selectionContext, u32 detailTextAddress, u32 footerTextAddress) __asm__("func_08004EFC");
EC void DestroyMessageWindow(void * self, int destructorFlags);
EC void DestroyRenderTaskBase(void * self, int destructorFlags) __asm__("func_080086BC");
EC void * GetRenderContext(void * self) __asm__("func_080088DC");
EC void * GetOamBuffer(void * self) __asm__("func_08008920");
EC void * GetDisplayRegisters(void * self) __asm__("func_08008918");
EC void * GetRenderJobQueue(void * self) __asm__("func_08008940");
EC void * GetFrameJobQueue(void * self) __asm__("func_0800894C");
EC unsigned int UpdateMessageWindow(void * dialog, void * renderContext, void * oamBuffer, void * displayRegisters, void * renderJobs, void * frameJobs, void * textSound);
EC int GetMessageWindowPhase(void * self);
EC void OpenMessageWindow(void * self);
EC void CloseMessageWindow(void * self);
EC unsigned int GetMessageWindowSelection(void * self);
EC void ConfigureMessageWindowPromptChoices(void * dialog, char const * promptText, char const * firstChoice, char const * secondChoice, u32 promptParameter, u32 firstChoiceParameter, u32 secondChoiceParameter);
extern char gChoiceConfirmationPrompt[] __asm__("gUnk_080E8998");
extern char gChoiceConfirmationYes[] __asm__("gUnk_080E89A4");
extern char gChoiceConfirmationNo[] __asm__("gUnk_080E89A8");
extern char gNameConfirmationPrompt[] __asm__("gUnk_080E8C2C");
extern char gNameConfirmationYes[] __asm__("gUnk_080E8C44");
extern char gNameConfirmationNo[] __asm__("gUnk_080E8C48");

EC void NameEntry_UpdateConfirmationDialog(NameEntryTask * self) SECTION(".text.func_08006ED8");
EC void NameEntry_UpdateConfirmationDialog(NameEntryTask * self)
{
    if (self->confirmationMode == 0)
        return;
    int previousPhase = GetMessageWindowPhase(self->dialog);
    void * dialog = self->dialog;
    void * renderContext = GetRenderContext(self);
    void * oamBuffer = GetOamBuffer(self);
    void * displayRegisters = GetDisplayRegisters(self);
    void * renderJobs = GetRenderJobQueue(self);
    void * frameJobs = GetFrameJobQueue(self);
    void * textSound = &self->textSound;
    switch (UpdateMessageWindow(dialog, renderContext, oamBuffer, displayRegisters, renderJobs, frameJobs, textSound))
    {
    case 0:
        if (self->cancelPending != 0)
        {
            self->confirmationMode = 0;
            self->cancelPending = 0;
        }
        if (self->acceptPending != 0)
        {
            // Commit the normalized name only after the confirmation dialog closes.
            self->acceptPending = 0;
            self->finished = 1;
            strcpy(self->enteredName, self->confirmedName);
        }
        OpenMessageWindow(self->dialog);
        break;
    case 1: break;
    case 2: break;
    case 3:
        switch (previousPhase)
        {
        case 4: case 5: break;
        default:
            CloseMessageWindow(self->dialog);
            break;
        case 1:
            if (self->confirmationMode == 1)
                ConfigureMessageWindowPromptChoices(self->dialog, gNameConfirmationPrompt, gNameConfirmationYes, gNameConfirmationNo, 0, 0, 0);
            break;
        case 6:
            switch (GetMessageWindowSelection(self->dialog))
            {
            case 1: self->acceptPending = 1; break;
            case 2: self->cancelPending = 1; break;
            }
            break;
        }
        break;
    case 4: break;
    case 5: break;
    case 6: break;
    }
}

EC void ChoiceDialog_Update(ChoiceDialogTask * self) SECTION(".text.func_08005800");
EC void ChoiceDialog_Update(ChoiceDialogTask * self)
{
    int previousPhase = GetMessageWindowPhase(self->dialog);
    void * dialog = self->dialog;
    void * renderContext = GetRenderContext(self);
    void * oamBuffer = GetOamBuffer(self);
    void * displayRegisters = GetDisplayRegisters(self);
    void * renderJobs = GetRenderJobQueue(self);
    void * frameJobs = GetFrameJobQueue(self);
    void * textSound = &self->textSound;
    switch (UpdateMessageWindow(dialog, renderContext, oamBuffer, displayRegisters, renderJobs, frameJobs, textSound))
    {
    case 0:
        if (self->choiceMade != 0)
            self->closed = 1;
        OpenMessageWindow(self->dialog);
        break;
    case 1: break;
    case 2: break;
    case 3:
        switch (previousPhase)
        {
        case 4: case 5: break;
        default:
            CloseMessageWindow(self->dialog);
            break;
        case 1:
            ConfigureMessageWindowPromptChoices(self->dialog, gChoiceConfirmationPrompt, gChoiceConfirmationYes, gChoiceConfirmationNo, 0, 0, 0);
            break;
        case 6:
            switch (GetMessageWindowSelection(self->dialog))
            {
            case 0: break;
            case 1:
                self->MarkChoiceMade();
                self->SetAccepted(true);
                break;
            case 2:
                self->MarkChoiceMade();
                self->SetAccepted(false);
                break;
            case 3: break;
            case 4: break;
            case 5: break;
            case 6: break;
            }
            break;
        }
        break;
    case 4: break;
    case 5: break;
    case 6: break;
    }
}
EC void SetMessageWindowTextOptions(void * dialog, char const * text, int textParameter, int layoutParameter);

extern char gDiaryReadDescription[] __asm__("gUnk_080E8770");
extern char gDiaryWriteDescription[] __asm__("gUnk_080E877C");
extern char gEmptySaveSlotDescription[] __asm__("gUnk_080E878C");
extern char gControlSchemeDescription[] __asm__("gUnk_080E87A4");
extern char gClockOptionDescription[] __asm__("gUnk_080E87FC");
extern char gPortraitOptionDescription[] __asm__("gUnk_080E8828");
extern char gSpeakerNameOptionDescription[] __asm__("gUnk_080E8878");
extern char gEmptyMenuDescription[] __asm__("gUnk_080E86C0");

EC void DiaryMenu_UpdateDescription(DiaryMenuTask * self) SECTION(".text.func_08003EEC");
EC void DiaryMenu_UpdateDescription(DiaryMenuTask * self)
{
    char const * text = nullptr;
    switch (self->selectedItem)
    {
    case 0:
        text = self->optionValues[0] != 0 ? gDiaryReadDescription : gDiaryWriteDescription;
        break;
    case 1:
        text = !self->saveSummaries[self->optionValues[1]].IsEmpty() ? self->saveSummaries[self->optionValues[1]].CStr() : gEmptySaveSlotDescription;
        break;
    case 2: text = gControlSchemeDescription; break;
    case 3: text = gClockOptionDescription; break;
    case 4: text = gPortraitOptionDescription; break;
    case 5: text = gSpeakerNameOptionDescription; break;
    }
    SetMessageWindowTextOptions(reinterpret_cast<char *>(self) + 0x10, text != nullptr ? text : gEmptyMenuDescription, 0, 0);
}

EC void NameEntry_SubmitSprites(NameEntryTask * self) SECTION(".text.func_08006858");
EC void NameEntry_SubmitSprites(NameEntryTask * self)
{
    if (self->confirmationMode == 0)
    {
        OamSpriteBuffer * oamBuffer = static_cast<OamSpriteBuffer *>(GetOamBuffer(self));
        u32 const & positionAndShape = self->cursorPositionAndShape;
        unsigned int spriteIndex = oamBuffer->spriteCount;
        if (spriteIndex < 128)
        {
            OamSprite & oamEntry = oamBuffer->SpriteAt(spriteIndex);
            oamEntry.positionAndShape = positionAndShape;
            oamEntry.tileAndPalette = self->cursorTileAndPalette;
            oamBuffer->spriteCount = spriteIndex + 1;
        }
    }
    OamSpriteBuffer * oamBuffer = static_cast<OamSpriteBuffer *>(GetOamBuffer(self));
    u32 const & positionAndShape = self->caretPositionAndShape;
    unsigned int spriteIndex = oamBuffer->spriteCount;
    if (spriteIndex < 128)
    {
        OamSprite & oamEntry = oamBuffer->SpriteAt(spriteIndex);
        oamEntry.positionAndShape = positionAndShape;
        oamEntry.tileAndPalette = self->caretTileAndPalette;
        oamBuffer->spriteCount = spriteIndex + 1;
    }
}

EC void DiaryMenu_UpdateMessage(char * self) SECTION(".text.func_08003C28");
EC void DiaryMenu_UpdateMessage(char * self)
{
    void * renderContext = GetRenderContext(self);
    void * oamBuffer = GetOamBuffer(self);
    void * displayRegisters = GetDisplayRegisters(self);
    void * renderJobs = GetRenderJobQueue(self);
    void * frameJobs = GetFrameJobQueue(self);
    void * dialog = self + 0x10;
    void * textSound = self + 0x14;
    UpdateMessageWindow(dialog, renderContext, oamBuffer, displayRegisters, renderJobs, frameJobs, textSound);
}

EC SceneTaskOwner * InitializeDiaryMenuScene(SceneTaskOwner * self) SECTION(".text.func_08004B58");
EC SceneTaskOwner * InitializeDiaryMenuScene(SceneTaskOwner * self)
{
    self->vtable = gDiaryMenuSceneVtable;
    void ** initialTransition = static_cast<void **>(__builtin_new(4));
    *initialTransition = gNewGameTransitionVtable;
    void * transitionOwner = initialTransition;
    self->task = CreateDiaryMenuTask(__builtin_new(0x1A0), nullptr, &transitionOwner);
    return self;
}

EC SceneTaskOwner * InitializeDiaryMenuSceneWithTransition(SceneTaskOwner * self, void * gameData, SceneTransitionOwner & transitionOwner) SECTION(".text.func_08004B94");
EC SceneTaskOwner * InitializeDiaryMenuSceneWithTransition(SceneTaskOwner * self, void * gameData, SceneTransitionOwner & transitionOwner)
{
    self->vtable = gDiaryMenuSceneVtable;
    AUnk_0800080C * releasedTransition = transitionOwner.Release();
    self->task = CreateDiaryMenuTask(__builtin_new(0x1A0), gameData, &releasedTransition);
    delete transitionOwner.transition;
    return self;
}

EC void ChoiceDialog_DestroyTask(ChoiceDialogTaskLifetime * self, int destructorFlags) SECTION(".text.func_080050C8");
EC void ChoiceDialog_DestroyTask(ChoiceDialogTaskLifetime * self, int destructorFlags)
{
    self->vtable = gChoiceDialogTaskVtable;
    if (self->dialog != nullptr)
        DestroyMessageWindow(self->dialog, 3);
    DestroyRenderTaskBase(self, destructorFlags);
}

EC SceneTaskOwner * InitializeChoiceDialogScene(SceneTaskOwner * self, void * summaryText, void * selectionContext, u32 detailTextAddress, u32 footerTextAddress) SECTION(".text.func_0800598C");
EC SceneTaskOwner * InitializeChoiceDialogScene(SceneTaskOwner * self, void * summaryText, void * selectionContext, u32 detailTextAddress, u32 footerTextAddress)
{
    self->vtable = gChoiceDialogSceneVtable;
    self->task = CreateChoiceDialogTask(__builtin_new(0x1A78), summaryText, selectionContext, detailTextAddress, footerTextAddress);
    return self;
}

EC void NameEntry_DestroyTask(NameEntryTaskLifetime * self, int destructorFlags) SECTION(".text.func_08005B38");
EC void NameEntry_DestroyTask(NameEntryTaskLifetime * self, int destructorFlags)
{
    self->vtable = gNameEntryTaskVtable;
    if (self->dialog != nullptr)
        DestroyMessageWindow(self->dialog, 3);
    DestroyRenderTaskBase(self, destructorFlags);
}

EC bool NameEntry_PrepareConfirmedName(NameEntryTask * self) SECTION(".text.func_08006C60");
EC bool NameEntry_PrepareConfirmedName(NameEntryTask * self)
{
    // Keep internal gaps as spaces, but reject names made entirely of empty slots.
    int contentLength = 0;
    int blankCount = 0;
    strcpy(self->confirmedName, self->enteredName);
    for (int characterIndex = 0; characterIndex < 12; characterIndex++)
    {
        if (self->confirmedName[characterIndex] == '_')
            blankCount++;
        else
        {
            if (self->confirmedName[characterIndex] == ' ')
                blankCount++;
            contentLength = characterIndex + 1;
        }
    }
    if (blankCount == 12)
        contentLength = 0;
    else
        for (int characterIndex = 0; characterIndex < contentLength; characterIndex++)
            if (self->confirmedName[characterIndex] == '_')
                self->confirmedName[characterIndex] = ' ';
    self->confirmedName[contentLength] = 0;
    return contentLength != 0;
}

EC void DestroyDiaryMenuScene(SceneTaskOwner * self, int destructorFlags) SECTION(".text.func_08004BDC");
EC void DestroyDiaryMenuScene(SceneTaskOwner * self, int destructorFlags)
{
    self->vtable = gDiaryMenuSceneVtable;
    delete self->task;
    DestroySceneBase(self, destructorFlags);
}

EC void * InitializeNewGameSetupScene(void ** self) SECTION(".text.func_08004C48");
EC void * InitializeNewGameSetupScene(void ** self)
{
    *self = gNewGameSetupSceneVtable;
    return self;
}

EC void DestroyNewGameSetupScene(void ** self, int destructorFlags) SECTION(".text.func_08004C54");
EC void DestroyNewGameSetupScene(void ** self, int destructorFlags)
{
    *self = gNewGameSetupSceneVtable;
    DestroySceneBase(self, destructorFlags);
}

EC void DestroyChoiceDialogScene(SceneTaskOwner * self, int destructorFlags) SECTION(".text.func_080059D0");
EC void DestroyChoiceDialogScene(SceneTaskOwner * self, int destructorFlags)
{
    self->vtable = gChoiceDialogSceneVtable;
    delete self->task;
    DestroySceneBase(self, destructorFlags);
}

EC SceneTaskOwner * InitializeNameEntryScene(SceneTaskOwner * self, void * promptText, u32 initialNameAddress) SECTION(".text.func_08007078");
EC SceneTaskOwner * InitializeNameEntryScene(SceneTaskOwner * self, void * promptText, u32 initialNameAddress)
{
    self->vtable = gNameEntrySceneVtable;
    self->task = CreateNameEntryTask(__builtin_new(0x4640), promptText, initialNameAddress);
    return self;
}

EC void DestroyNameEntryScene(SceneTaskOwner * self, int destructorFlags) SECTION(".text.func_080070A4");
EC void DestroyNameEntryScene(SceneTaskOwner * self, int destructorFlags)
{
    self->vtable = gNameEntrySceneVtable;
    delete self->task;
    DestroySceneBase(self, destructorFlags);
}

EC void BlankSceneDisplay(void * self) __asm__("func_08008724");

EC void ChoiceDialog_BlankDisplay(void * self) SECTION(".text.func_08005254");
EC void ChoiceDialog_BlankDisplay(void * self)
{
    BlankSceneDisplay(self);
}

EC void ChoiceDialog_Reset(ChoiceDialogTask * self) SECTION(".text.func_08005260");
EC void ChoiceDialog_Reset(ChoiceDialogTask * self)
{
    self->choiceMade = 0;
    self->closed = 0;
}

EC u8 ChoiceDialog_GetResult(ChoiceDialogScene * self) SECTION(".text.func_08005A3C");
EC u8 ChoiceDialog_GetResult(ChoiceDialogScene * self)
{
    return self->task->accepted;
}

EC void NameEntry_Reset(NameEntryTask * self) SECTION(".text.func_08006334");
EC void NameEntry_Reset(NameEntryTask * self)
{
    self->acceptPending = 0;
    self->cancelPending = 0;
    self->finished = 0;
    self->keyboardDirty = 0;
    self->cursorRow = 0;
    self->cursorColumn = 0;
    self->keyboardPage = 0;
    self->charIndex = 0;
    self->confirmationMode = 0;
}

EC void NameEntry_DeleteCharacter(NameEntryTask * self, u8 characterIndex) SECTION(".text.func_08006C28");
EC void NameEntry_DeleteCharacter(NameEntryTask * self, u8 characterIndex)
{
    for (; characterIndex < 11; characterIndex++)
        self->enteredName[characterIndex] = self->enteredName[characterIndex + 1];
    self->enteredName[11] = '_';
}

EC char * NameEntry_GetName(NameEntryScene * self) SECTION(".text.func_08007110");
EC char * NameEntry_GetName(NameEntryScene * self)
{
    return self->task->enteredName;
}

EC void func_08006ED8(NameEntryTask * self) ALIAS(NameEntry_UpdateConfirmationDialog);
EC void func_08005800(ChoiceDialogTask * self) ALIAS(ChoiceDialog_Update);
EC void func_08003EEC(DiaryMenuTask * self) ALIAS(DiaryMenu_UpdateDescription);
EC void func_08006858(NameEntryTask * self) ALIAS(NameEntry_SubmitSprites);
EC void func_08003C28(char * self) ALIAS(DiaryMenu_UpdateMessage);
EC SceneTaskOwner * func_08004B58(SceneTaskOwner * self) ALIAS(InitializeDiaryMenuScene);
EC SceneTaskOwner * func_08004B94(SceneTaskOwner * self, void * gameData, SceneTransitionOwner & transitionOwner) ALIAS(InitializeDiaryMenuSceneWithTransition);
EC SceneTaskOwner * func_0800598C(SceneTaskOwner * self, void * summaryText, void * selectionContext, u32 detailTextAddress, u32 footerTextAddress) ALIAS(InitializeChoiceDialogScene);
EC bool func_08006C60(NameEntryTask * self) ALIAS(NameEntry_PrepareConfirmedName);
EC void * func_08004C48(void ** self) ALIAS(InitializeNewGameSetupScene);
EC void func_080059D0(SceneTaskOwner * self, int destructorFlags) ALIAS(DestroyChoiceDialogScene);
EC SceneTaskOwner * func_08007078(SceneTaskOwner * self, void * promptText, u32 initialNameAddress) ALIAS(InitializeNameEntryScene);
EC void func_080070A4(SceneTaskOwner * self, int destructorFlags) ALIAS(DestroyNameEntryScene);
EC void func_08005254(void * self) ALIAS(ChoiceDialog_BlankDisplay);
EC void func_08005260(ChoiceDialogTask * self) ALIAS(ChoiceDialog_Reset);
EC u8 func_08005A3C(ChoiceDialogScene * self) ALIAS(ChoiceDialog_GetResult);
EC void func_08006334(NameEntryTask * self) ALIAS(NameEntry_Reset);
EC void func_08006C28(NameEntryTask * self, u8 characterIndex) ALIAS(NameEntry_DeleteCharacter);
EC char * func_08007110(NameEntryScene * self) ALIAS(NameEntry_GetName);
