#include "new_game.hh"

#include <string.h>

extern char gNewGameMenuSceneVtable[] __asm__("vtable_unk_080E5A68");
extern char gNewGameSetupSceneVtable[] __asm__("vtable_unk_080E5A88");
extern char gNewGameChoiceDialogSceneVtable[] __asm__("vtable_unk_080E5AA4");
extern char gNameEntrySceneVtable[] __asm__("vtable_unk_080E5AC0");
extern char gNewGameSetupTransitionVtable[] __asm__("vtable_unk_080E5A3C");
extern char gNewGameChoiceDialogTaskVtable[] __asm__("vtable_unk_080E5A98");
extern char gNameEntrySceneTaskVtable[] __asm__("vtable_unk_080E5AB4");

EC void * __builtin_new(size_t size);
EC NewGameRenderTask * NameEntryScene_CreateTask(void * storage, void * promptText, u32 initialNameAddress) __asm__("func_08005A58");
EC void Scene_DestroyBase(void * self, int destructorFlags);
EC NewGameRenderTask * NewGameMenu_CreateTask(void * storage, void * gameData, void * transitionOwner) __asm__("func_080041DC");
EC NewGameRenderTask * NewGameChoiceDialog_CreateTask(void * storage, void * farmerName, void * packedBirthday, u32 farmNameAddress, u32 dogNameAddress) __asm__("func_08004EFC");
EC void DestroyMessageWindow(void * self, int destructorFlags);
EC void NewGameScene_DestroyRenderTaskBase(void * self, int destructorFlags) __asm__("func_080086BC");
EC void * HardwareContext_GetState(void * self);
EC void * HardwareContext_GetOamBuffer(void * self);
EC void * HardwareContext_GetDisplayRegisters(void * self);
EC void * HardwareContext_GetFrameTasks(void * self);
EC void * HardwareContext_GetVBlankTasks(void * self);
EC unsigned int UpdateMessageWindow(void * dialog, void * renderContext, void * oamBuffer, void * displayRegisters, void * renderJobs, void * frameJobs, void * textSound);
EC int GetMessageWindowPhase(void * self);
EC void OpenMessageWindow(void * self);
EC void CloseMessageWindow(void * self);
EC unsigned int GetMessageWindowSelection(void * self);
EC void ConfigureMessageWindowPromptChoices(void * dialog, char const * promptText, char const * firstChoice, char const * secondChoice, u32 promptParameter, u32 firstChoiceParameter, u32 secondChoiceParameter);
extern char gNewGameChoiceConfirmationPrompt[] __asm__("gUnk_080E8998");
extern char gNewGameChoiceConfirmationYes[] __asm__("gUnk_080E89A4");
extern char gNewGameChoiceConfirmationNo[] __asm__("gUnk_080E89A8");
extern char gNameEntrySceneConfirmationPrompt[] __asm__("gUnk_080E8C2C");
extern char gNameEntrySceneConfirmationYes[] __asm__("gUnk_080E8C44");
extern char gNameEntrySceneConfirmationNo[] __asm__("gUnk_080E8C48");

struct ALIGN(4) NameEntrySceneKeySnapshot
{
    enum { FACE_BANK, SHOULDER_BANK };
    enum { A_BUTTON = 1, B_BUTTON = 2, START_BUTTON = 8, L_BUTTON = 2, R_BUTTON = 1 };
    u8 bytes[4];
    NameEntrySceneKeySnapshot(u32 bits) { *reinterpret_cast<u32 *>(bytes) = bits; }
    NameEntrySceneKeySnapshot(NameEntrySceneKeySnapshot const & source)
    {
        memcpy(bytes, source.bytes, sizeof(bytes));
    }
};

EC u32 NameEntryScene_GetRepeatedKeyBits(void * self) __asm__("HardwareContext_GetRepeatedKeys");
EC void AudioPlayer_StartSong(void * player, u16 song);
EC void NameEntryScene_MoveCursor(NameEntrySceneTask * self) __asm__("func_080069F4");
EC void NameEntryScene_InsertCharacter(NameEntrySceneTask * self) __asm__("func_08006B90");

EC void NameEntryScene_HandleInput(NameEntrySceneTask * self) SECTION(".text.func_08006CDC");
EC void NameEntryScene_HandleInput(NameEntrySceneTask * self)
{
    if (self->confirmationMode != 0)
        return;
    NameEntryScene_MoveCursor(self);
    NameEntrySceneKeySnapshot keys(NameEntryScene_GetRepeatedKeyBits(self));
    if ((keys.bytes[NameEntrySceneKeySnapshot::FACE_BANK] & NameEntrySceneKeySnapshot::A_BUTTON) != 0)
    {
        if (self->cursorRow <= 5)
        {
            AudioPlayer_StartSong(&self->inputSound, 0x6A);
            NameEntryScene_InsertCharacter(self);
        }
        else
        {
            // Bottom-row groups select next page, previous page, or confirmation.
            u8 column = self->cursorColumn;
            if (self->cursorColumn <= 4)
            {
                if (self->keyboardPage == 2)
                    self->keyboardPage = 0;
                else
                    ++self->keyboardPage;
                self->keyboardDirty = 1;
                AudioPlayer_StartSong(&self->inputSound, 0xCA);
            }
            else if (static_cast<u8>(column - 5) <= 4)
            {
                if (self->keyboardPage == 0)
                    self->keyboardPage = 2;
                else
                    --self->keyboardPage;
                self->keyboardDirty = 1;
                AudioPlayer_StartSong(&self->inputSound, 0xCA);
            }
            else if (NameEntryScene_PrepareConfirmedName(self))
            {
                self->confirmationMode = 1;
                AudioPlayer_StartSong(&self->inputSound, 0xC5);
            }
            else
                AudioPlayer_StartSong(&self->inputSound, 0xC7);
        }
    }
    else if ((keys.bytes[NameEntrySceneKeySnapshot::FACE_BANK] & NameEntrySceneKeySnapshot::B_BUTTON) != 0)
    {
        if (self->charIndex != 0)
        {
            --self->charIndex;
            NameEntryScene_DeleteCharacter(self, self->charIndex);
            AudioPlayer_StartSong(&self->inputSound, 0x6C);
        }
        else
        {
            u8 emptySlots = 0;
            for (int index = 0; index <= 11; ++index)
                if (self->enteredName[index] == '_')
                    ++emptySlots;
            if (emptySlots == 12)
                AudioPlayer_StartSong(&self->inputSound, 0xC7);
            else
            {
                NameEntryScene_DeleteCharacter(self, 0);
                AudioPlayer_StartSong(&self->inputSound, 0x6C);
            }
        }
    }
    else if ((keys.bytes[NameEntrySceneKeySnapshot::FACE_BANK] & NameEntrySceneKeySnapshot::START_BUTTON) != 0)
    {
        if (self->cursorRow == 6 && self->cursorColumn > 9 && self->cursorColumn <= 14)
        {
            if (NameEntryScene_PrepareConfirmedName(self))
                self->confirmationMode = 1;
            AudioPlayer_StartSong(&self->inputSound, 0xC5);
        }
        else
        {
            self->cursorColumn = 12;
            self->cursorRow = 6;
            AudioPlayer_StartSong(&self->inputSound, 0xCB);
        }
    }
    else if ((keys.bytes[NameEntrySceneKeySnapshot::SHOULDER_BANK] & NameEntrySceneKeySnapshot::L_BUTTON) != 0)
    {
        if (self->charIndex != 0)
            --self->charIndex;
        AudioPlayer_StartSong(&self->inputSound, 0xCB);
    }
    else if ((keys.bytes[NameEntrySceneKeySnapshot::SHOULDER_BANK] & NameEntrySceneKeySnapshot::R_BUTTON) != 0)
    {
        if (self->charIndex != 11)
            ++self->charIndex;
        AudioPlayer_StartSong(&self->inputSound, 0xCB);
    }
}


EC void func_08006CDC(NameEntrySceneTask * self) ALIAS(NameEntryScene_HandleInput);

EC void NameEntryScene_UpdateCursor(NameEntrySceneTask * self) SECTION(".text.func_080068CC");
EC void NameEntryScene_UpdateCursor(NameEntrySceneTask * self)
{
    self->caretAttributes.x = 0x90 + self->charIndex * 8;
    self->caretAttributes.y = 8;
    if (self->cursorRow <= 5)
    {
        self->cursorAttributes.x = self->cursorColumn * 16;
        self->cursorAttributes.y = self->cursorRow * 16 + 32;
        self->cursorAttributes.size = 0;
        self->cursorAttributes.shape = 2;
    }
    else
    {
        // Five logical columns share each wide action-button cursor.
        u8 group = 0;
        u8 column = self->cursorColumn;
        while (column > 4)
        {
            column -= 5;
            ++group;
        }
        self->cursorAttributes.x = group * 80 + 24;
        self->cursorAttributes.y = 136;
        self->cursorAttributes.size = 2;
        self->cursorAttributes.shape = 1;
    }
}

EC void func_080068CC(NameEntrySceneTask * self) ALIAS(NameEntryScene_UpdateCursor);

EC void NewGameMenu_PositionCursorSprite(NewGameMenuTask * self, int option, NewGameOamSprite source) SECTION(".text.func_08003E88");
EC void NewGameMenu_PositionCursorSprite(NewGameMenuTask * self, int option, NewGameOamSprite source)
{
    // Options 0 and 1 share the cursor's first visual row.
    if (option == 1)
        option = 0;
    NewGameOamSprite sprite = source;
    sprite.attributes.y += 8 + (option * 2 + 1) * 8;
    sprite.attributes.x += 16;
    static_cast<NewGameOamSpriteBuffer *>(HardwareContext_GetOamBuffer(self))->WriteSprite(6, sprite);
}

EC void func_08003E88(NewGameMenuTask * self, int option, NewGameOamSprite source) ALIAS(NewGameMenu_PositionCursorSprite);

EC void NewGameMenu_PositionOptionSprite(NewGameMenuTask * self, int option, NewGameOamSprite sprite) SECTION(".text.func_08003DF0");
EC void NewGameMenu_PositionOptionSprite(NewGameMenuTask * self, int option, NewGameOamSprite sprite)
{
    sprite.attributes.y += 8 + (option * 2 + 1) * 8;
    sprite.attributes.x += 128 + (self->optionValues[option] != 0 ? 64 : 0);
    // OBJ mode 2 hides the inactive second option.
    if (option == 1 && self->selectedItem != 1)
        sprite.attributes.objectMode = 2;
    static_cast<NewGameOamSpriteBuffer *>(HardwareContext_GetOamBuffer(self))->WriteSprite(option, sprite);
}

EC void func_08003DF0(NewGameMenuTask * self, int option, NewGameOamSprite sprite) ALIAS(NewGameMenu_PositionOptionSprite);

EC void NameEntryScene_UpdateConfirmationDialog(NameEntrySceneTask * self) SECTION(".text.func_08006ED8");
EC void NameEntryScene_UpdateConfirmationDialog(NameEntrySceneTask * self)
{
    if (self->confirmationMode == 0)
        return;
    int previousPhase = GetMessageWindowPhase(self->dialog);
    void * dialog = self->dialog;
    void * renderContext = HardwareContext_GetState(self);
    void * oamBuffer = HardwareContext_GetOamBuffer(self);
    void * displayRegisters = HardwareContext_GetDisplayRegisters(self);
    void * renderJobs = HardwareContext_GetFrameTasks(self);
    void * frameJobs = HardwareContext_GetVBlankTasks(self);
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
                ConfigureMessageWindowPromptChoices(self->dialog, gNameEntrySceneConfirmationPrompt, gNameEntrySceneConfirmationYes, gNameEntrySceneConfirmationNo, 0, 0, 0);
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

EC void NewGameChoiceDialog_Update(NewGameChoiceDialogTask * self) SECTION(".text.func_08005800");
EC void NewGameChoiceDialog_Update(NewGameChoiceDialogTask * self)
{
    int previousPhase = GetMessageWindowPhase(self->dialog);
    void * dialog = self->dialog;
    void * renderContext = HardwareContext_GetState(self);
    void * oamBuffer = HardwareContext_GetOamBuffer(self);
    void * displayRegisters = HardwareContext_GetDisplayRegisters(self);
    void * renderJobs = HardwareContext_GetFrameTasks(self);
    void * frameJobs = HardwareContext_GetVBlankTasks(self);
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
            ConfigureMessageWindowPromptChoices(self->dialog, gNewGameChoiceConfirmationPrompt, gNewGameChoiceConfirmationYes, gNewGameChoiceConfirmationNo, 0, 0, 0);
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

extern char gNewGameMenuReadDescription[] __asm__("gUnk_080E8770");
extern char gNewGameMenuWriteDescription[] __asm__("gUnk_080E877C");
extern char gNewGameMenuEmptySaveSlotDescription[] __asm__("gUnk_080E878C");
extern char gNewGameMenuControlSchemeDescription[] __asm__("gUnk_080E87A4");
extern char gNewGameMenuClockOptionDescription[] __asm__("gUnk_080E87FC");
extern char gNewGameMenuPortraitOptionDescription[] __asm__("gUnk_080E8828");
extern char gNewGameMenuSpeakerNameOptionDescription[] __asm__("gUnk_080E8878");
extern char gNewGameMenuEmptyDescription[] __asm__("gUnk_080E86C0");

EC void NewGameMenu_UpdateDescription(NewGameMenuTask * self) SECTION(".text.func_08003EEC");
EC void NewGameMenu_UpdateDescription(NewGameMenuTask * self)
{
    char const * text = nullptr;
    switch (self->selectedItem)
    {
    case 0:
        text = self->optionValues[0] != 0 ? gNewGameMenuReadDescription : gNewGameMenuWriteDescription;
        break;
    case 1:
        text = !self->saveSummaries[self->optionValues[1]].IsEmpty() ? self->saveSummaries[self->optionValues[1]].CStr() : gNewGameMenuEmptySaveSlotDescription;
        break;
    case 2: text = gNewGameMenuControlSchemeDescription; break;
    case 3: text = gNewGameMenuClockOptionDescription; break;
    case 4: text = gNewGameMenuPortraitOptionDescription; break;
    case 5: text = gNewGameMenuSpeakerNameOptionDescription; break;
    }
    SetMessageWindowTextOptions(reinterpret_cast<char *>(self) + 0x10, text != nullptr ? text : gNewGameMenuEmptyDescription, 0, 0);
}

EC void NameEntryScene_SubmitSprites(NameEntrySceneTask * self) SECTION(".text.func_08006858");
EC void NameEntryScene_SubmitSprites(NameEntrySceneTask * self)
{
    if (self->confirmationMode == 0)
    {
        NewGameOamSpriteBuffer * oamBuffer = static_cast<NewGameOamSpriteBuffer *>(HardwareContext_GetOamBuffer(self));
        u32 const & positionAndShape = self->cursorPositionAndShape;
        unsigned int spriteIndex = oamBuffer->spriteCount;
        if (spriteIndex < 128)
        {
            NewGameOamSprite & oamEntry = oamBuffer->SpriteAt(spriteIndex);
            oamEntry.positionAndShape = positionAndShape;
            oamEntry.tileAndPalette = self->cursorTileAndPalette;
            oamBuffer->spriteCount = spriteIndex + 1;
        }
    }
    NewGameOamSpriteBuffer * oamBuffer = static_cast<NewGameOamSpriteBuffer *>(HardwareContext_GetOamBuffer(self));
    u32 const & positionAndShape = self->caretPositionAndShape;
    unsigned int spriteIndex = oamBuffer->spriteCount;
    if (spriteIndex < 128)
    {
        NewGameOamSprite & oamEntry = oamBuffer->SpriteAt(spriteIndex);
        oamEntry.positionAndShape = positionAndShape;
        oamEntry.tileAndPalette = self->caretTileAndPalette;
        oamBuffer->spriteCount = spriteIndex + 1;
    }
}

EC void NewGameMenu_UpdateMessage(char * self) SECTION(".text.func_08003C28");
EC void NewGameMenu_UpdateMessage(char * self)
{
    void * renderContext = HardwareContext_GetState(self);
    void * oamBuffer = HardwareContext_GetOamBuffer(self);
    void * displayRegisters = HardwareContext_GetDisplayRegisters(self);
    void * renderJobs = HardwareContext_GetFrameTasks(self);
    void * frameJobs = HardwareContext_GetVBlankTasks(self);
    void * dialog = self + 0x10;
    void * textSound = self + 0x14;
    UpdateMessageWindow(dialog, renderContext, oamBuffer, displayRegisters, renderJobs, frameJobs, textSound);
}

EC NewGameSceneTaskOwner * NewGameMenuScene_Initialize(NewGameSceneTaskOwner * self) SECTION(".text.func_08004B58");
EC NewGameSceneTaskOwner * NewGameMenuScene_Initialize(NewGameSceneTaskOwner * self)
{
    self->vtable = gNewGameMenuSceneVtable;
    void ** initialTransition = static_cast<void **>(__builtin_new(4));
    *initialTransition = gNewGameSetupTransitionVtable;
    void * transitionOwner = initialTransition;
    self->task = NewGameMenu_CreateTask(__builtin_new(0x1A0), nullptr, &transitionOwner);
    return self;
}

EC NewGameSceneTaskOwner * NewGameMenuScene_InitializeWithTransition(NewGameSceneTaskOwner * self, void * gameData, NewGameSceneTransitionOwner & transitionOwner) SECTION(".text.func_08004B94");
EC NewGameSceneTaskOwner * NewGameMenuScene_InitializeWithTransition(NewGameSceneTaskOwner * self, void * gameData, NewGameSceneTransitionOwner & transitionOwner)
{
    self->vtable = gNewGameMenuSceneVtable;
    AUnk_0800080C * releasedTransition = transitionOwner.Release();
    self->task = NewGameMenu_CreateTask(__builtin_new(0x1A0), gameData, &releasedTransition);
    delete transitionOwner.transition;
    return self;
}

EC void NewGameChoiceDialog_DestroyTask(NewGameChoiceDialogTaskLifetime * self, int destructorFlags) SECTION(".text.func_080050C8");
EC void NewGameChoiceDialog_DestroyTask(NewGameChoiceDialogTaskLifetime * self, int destructorFlags)
{
    self->vtable = gNewGameChoiceDialogTaskVtable;
    if (self->dialog != nullptr)
        DestroyMessageWindow(self->dialog, 3);
    NewGameScene_DestroyRenderTaskBase(self, destructorFlags);
}

EC NewGameSceneTaskOwner * NewGameChoiceDialogScene_Initialize(NewGameSceneTaskOwner * self, void * farmerName, void * packedBirthday, u32 farmNameAddress, u32 dogNameAddress) SECTION(".text.func_0800598C");
EC NewGameSceneTaskOwner * NewGameChoiceDialogScene_Initialize(NewGameSceneTaskOwner * self, void * farmerName, void * packedBirthday, u32 farmNameAddress, u32 dogNameAddress)
{
    self->vtable = gNewGameChoiceDialogSceneVtable;
    self->task = NewGameChoiceDialog_CreateTask(__builtin_new(0x1A78), farmerName, packedBirthday, farmNameAddress, dogNameAddress);
    return self;
}

EC void NameEntryScene_DestroyTask(NameEntrySceneTaskLifetime * self, int destructorFlags) SECTION(".text.func_08005B38");
EC void NameEntryScene_DestroyTask(NameEntrySceneTaskLifetime * self, int destructorFlags)
{
    self->vtable = gNameEntrySceneTaskVtable;
    if (self->dialog != nullptr)
        DestroyMessageWindow(self->dialog, 3);
    NewGameScene_DestroyRenderTaskBase(self, destructorFlags);
}

EC bool NameEntryScene_PrepareConfirmedName(NameEntrySceneTask * self) SECTION(".text.func_08006C60");
EC bool NameEntryScene_PrepareConfirmedName(NameEntrySceneTask * self)
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

EC void NewGameMenuScene_Destroy(NewGameSceneTaskOwner * self, int destructorFlags) SECTION(".text.func_08004BDC");
EC void NewGameMenuScene_Destroy(NewGameSceneTaskOwner * self, int destructorFlags)
{
    self->vtable = gNewGameMenuSceneVtable;
    delete self->task;
    Scene_DestroyBase(self, destructorFlags);
}

EC void * NewGameSetupScene_Initialize(void ** self) SECTION(".text.func_08004C48");
EC void * NewGameSetupScene_Initialize(void ** self)
{
    *self = gNewGameSetupSceneVtable;
    return self;
}

EC void NewGameSetupScene_Destroy(void ** self, int destructorFlags) SECTION(".text.func_08004C54");
EC void NewGameSetupScene_Destroy(void ** self, int destructorFlags)
{
    *self = gNewGameSetupSceneVtable;
    Scene_DestroyBase(self, destructorFlags);
}

EC void NewGameChoiceDialogScene_Destroy(NewGameSceneTaskOwner * self, int destructorFlags) SECTION(".text.func_080059D0");
EC void NewGameChoiceDialogScene_Destroy(NewGameSceneTaskOwner * self, int destructorFlags)
{
    self->vtable = gNewGameChoiceDialogSceneVtable;
    delete self->task;
    Scene_DestroyBase(self, destructorFlags);
}

EC NewGameSceneTaskOwner * NameEntryScene_Initialize(NewGameSceneTaskOwner * self, void * promptText, u32 initialNameAddress) SECTION(".text.func_08007078");
EC NewGameSceneTaskOwner * NameEntryScene_Initialize(NewGameSceneTaskOwner * self, void * promptText, u32 initialNameAddress)
{
    self->vtable = gNameEntrySceneVtable;
    self->task = NameEntryScene_CreateTask(__builtin_new(0x4640), promptText, initialNameAddress);
    return self;
}

EC void NameEntryScene_Destroy(NewGameSceneTaskOwner * self, int destructorFlags) SECTION(".text.func_080070A4");
EC void NameEntryScene_Destroy(NewGameSceneTaskOwner * self, int destructorFlags)
{
    self->vtable = gNameEntrySceneVtable;
    delete self->task;
    Scene_DestroyBase(self, destructorFlags);
}

EC void NewGameScene_BlankDisplay(void * self) __asm__("func_08008724");

EC void NewGameChoiceDialog_BlankDisplay(void * self) SECTION(".text.func_08005254");
EC void NewGameChoiceDialog_BlankDisplay(void * self)
{
    NewGameScene_BlankDisplay(self);
}

EC void NewGameChoiceDialog_Reset(NewGameChoiceDialogTask * self) SECTION(".text.func_08005260");
EC void NewGameChoiceDialog_Reset(NewGameChoiceDialogTask * self)
{
    self->choiceMade = 0;
    self->closed = 0;
}

EC u8 NewGameChoiceDialogScene_GetResult(NewGameChoiceDialogScene * self) SECTION(".text.func_08005A3C");
EC u8 NewGameChoiceDialogScene_GetResult(NewGameChoiceDialogScene * self)
{
    return self->task->accepted;
}

EC void NameEntryScene_Reset(NameEntrySceneTask * self) SECTION(".text.func_08006334");
EC void NameEntryScene_Reset(NameEntrySceneTask * self)
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

EC void NameEntryScene_DeleteCharacter(NameEntrySceneTask * self, u8 characterIndex) SECTION(".text.func_08006C28");
EC void NameEntryScene_DeleteCharacter(NameEntrySceneTask * self, u8 characterIndex)
{
    for (; characterIndex < 11; characterIndex++)
        self->enteredName[characterIndex] = self->enteredName[characterIndex + 1];
    self->enteredName[11] = '_';
}

EC char * NameEntryScene_GetName(NameEntryScene * self) SECTION(".text.func_08007110");
EC char * NameEntryScene_GetName(NameEntryScene * self)
{
    return self->task->enteredName;
}

EC char * NameEntrySceneTask_GetEnteredName(NameEntrySceneTask * self) SECTION(".text.func_0800711C");
EC char * NameEntrySceneTask_GetEnteredName(NameEntrySceneTask * self)
{
    return self->enteredName;
}

EC void func_08006ED8(NameEntrySceneTask * self) ALIAS(NameEntryScene_UpdateConfirmationDialog);
EC void func_08005800(NewGameChoiceDialogTask * self) ALIAS(NewGameChoiceDialog_Update);
EC void func_08003EEC(NewGameMenuTask * self) ALIAS(NewGameMenu_UpdateDescription);
EC void func_08006858(NameEntrySceneTask * self) ALIAS(NameEntryScene_SubmitSprites);
EC void func_08003C28(char * self) ALIAS(NewGameMenu_UpdateMessage);
EC NewGameSceneTaskOwner * func_08004B58(NewGameSceneTaskOwner * self) ALIAS(NewGameMenuScene_Initialize);
EC NewGameSceneTaskOwner * func_08004B94(NewGameSceneTaskOwner * self, void * gameData, NewGameSceneTransitionOwner & transitionOwner) ALIAS(NewGameMenuScene_InitializeWithTransition);
EC NewGameSceneTaskOwner * func_0800598C(NewGameSceneTaskOwner * self, void * farmerName, void * packedBirthday, u32 farmNameAddress, u32 dogNameAddress) ALIAS(NewGameChoiceDialogScene_Initialize);
EC bool func_08006C60(NameEntrySceneTask * self) ALIAS(NameEntryScene_PrepareConfirmedName);
EC void * func_08004C48(void ** self) ALIAS(NewGameSetupScene_Initialize);
EC void func_080059D0(NewGameSceneTaskOwner * self, int destructorFlags) ALIAS(NewGameChoiceDialogScene_Destroy);
EC NewGameSceneTaskOwner * func_08007078(NewGameSceneTaskOwner * self, void * promptText, u32 initialNameAddress) ALIAS(NameEntryScene_Initialize);
EC void func_080070A4(NewGameSceneTaskOwner * self, int destructorFlags) ALIAS(NameEntryScene_Destroy);
EC void func_08005254(void * self) ALIAS(NewGameChoiceDialog_BlankDisplay);
EC void func_08005260(NewGameChoiceDialogTask * self) ALIAS(NewGameChoiceDialog_Reset);
EC u8 func_08005A3C(NewGameChoiceDialogScene * self) ALIAS(NewGameChoiceDialogScene_GetResult);
EC void func_08006334(NameEntrySceneTask * self) ALIAS(NameEntryScene_Reset);
EC void func_08006C28(NameEntrySceneTask * self, u8 characterIndex) ALIAS(NameEntryScene_DeleteCharacter);
EC char * func_08007110(NameEntryScene * self) ALIAS(NameEntryScene_GetName);
