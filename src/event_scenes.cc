#include "scene.hh"

#include "dma_queue.hh"
#include "farmer.hh"
#include "message_script.hh"
#include "utility/vec2.hh"

#include <stdlib.h>
#include <string.h>

EC void * __builtin_new(size_t size);
EC void Scene_DestroyBase(void * self, int flags);

struct HardwareContextState;

struct MedalExchangeScreen
{
    HardwareContextState * hardware;
    virtual ~MedalExchangeScreen();
};

struct MedalExchangeScene
{
    void const * vtable;
    MedalExchangeScreen * screen;
    AUnk_0800080C * next_scene;
};

EC char const gMedalExchangeSceneVTable[];
EC MedalExchangeScreen * InitMedalExchangeScreen(void * storage, void * game_state);
EC void RunMedalExchangeScreen(AUnk_0800080C ** result, MedalExchangeScreen * screen);

EC SECTION(".text.f3.0805E624") MedalExchangeScene * InitMedalExchangeScene(MedalExchangeScene * self, AUnk_0800080C ** next, void * game_state)
{
    self->vtable = gMedalExchangeSceneVTable;
    self->screen = InitMedalExchangeScreen(__builtin_new(0xAF4), game_state);
    AUnk_0800080C * next_scene = *next;
    *next = nullptr;
    self->next_scene = next_scene;
    return self;
}

EC SECTION(".text.f3.0805E658") void DestroyMedalExchangeScene(MedalExchangeScene * self, int flags)
{
    self->vtable = gMedalExchangeSceneVTable;
    delete self->next_scene;
    delete self->screen;
    Scene_DestroyBase(self, flags);
}

EC SECTION(".text.f3.0805E698") AUnk_0800080C ** RunMedalExchangeScene(AUnk_0800080C ** result, MedalExchangeScene * self)
{
    AUnk_0800080C * screen_result;
    RunMedalExchangeScreen(&screen_result, self->screen);
    delete screen_result;
    AUnk_0800080C * next_scene = self->next_scene;
    self->next_scene = nullptr;
    *result = next_scene;
    return result;
}

struct ChildhoodMemoryScreen
{
    HardwareContextState * hardware;
    virtual ~ChildhoodMemoryScreen();
};

struct MessageWindow;
struct MusicPlayer;
struct AudioPlayer { MusicPlayer * player; };
struct ResourcePool { void * storage; };
struct TileResources { void * handles; };

struct ChildhoodMemoryGameState
{
    STRUCT_PAD(0, 0x1BD8);
    Farmer farmer;
};

struct FarmerNameSubstitution
{
    void const * vtable;
    char const * name;
};

enum
{
    CHILDHOOD_MAP_COUNT = 20,
    CHILDHOOD_MAP_BYTES = 32 * 20 * sizeof(u16)
};

struct ChildhoodMemoryScreenLayout
{
    HardwareContextState * hardware;
    void const * vtable;
    ChildhoodMemoryGameState * game_state;
    u32 dialogue_style;
    MessageWindow * dialogue;
    AudioPlayer dialogue_audio;
    AudioPlayer music;
    AudioPlayer auxiliary_audio;
    u8 background_maps[CHILDHOOD_MAP_COUNT][CHILDHOOD_MAP_BYTES];
    i16 scene_cue;
    i16 scene_phase;
    i16 dialogue_line;
    i16 dialogue_phase;
    FarmerNameSubstitution farmer_name;
};

EC void InitScreenHardware(ChildhoodMemoryScreenLayout *, Vec2 const *) asm("func_080084DC");
EC void InitTileResources(TileResources *) asm("TilePool_Initialize");
EC void DestroyTileResources(TileResources *, int) asm("TilePool_Destroy");
EC ResourcePool * ResourcePool_Initialize(ResourcePool *);
EC void DestroyResourcePool(ResourcePool *, int) asm("ResourcePool_Destroy");
EC MessageWindow * CreateMessageWindow(MessageWindow *, u32, u32, u32, u32, void *, void *);
EC void ClearMessageWindowButtonMask(MessageWindow *, u32);
EC AudioPlayer * AudioPlayer_Initialize(AudioPlayer *, u8);
EC char const gChildhoodMemoryScreenVTable[] asm("vtable_unk_080E79DC");
EC char const gChildhoodMemoryFarmerNameVTable[];

struct SceneDisplayRegisters
{
    u16 control;
    STRUCT_PAD(2, 8);
    u16 background_control[4];
    u16 background_scroll[4][2];
};

EC SceneDisplayRegisters * HardwareContext_GetDisplayRegisters(ChildhoodMemoryScreenLayout *);
EC void Dma3_Fill(u32, void *, u32);
EC void Dma3_Copy(void const *, void *, u32);
EC void Unpack(void const *, void *);

EC u8 const gChildhoodMemoryBackgroundTiles[] asm("gUnk_08743058");
EC u16 const gChildhoodMemoryBackgroundPalette[] asm("gUnk_08747A14");
EC void const * const gChildhoodMemoryCompressedMaps[CHILDHOOD_MAP_COUNT] asm("gUnk_080FB1E4");

static inline void InitFarmerNameSubstitution(FarmerNameSubstitution * substitution, char const * name)
{
    substitution->vtable = gChildhoodMemoryFarmerNameVTable;
    substitution->name = name;
}

EC SECTION(".text.childhood_memory.080E19A4") MessageScriptReplacement * ResolveChildhoodMemoryFarmerName(
    MessageScriptReplacement * output, FarmerNameSubstitution const * substitution, u32 placeholder)
{
    enum { FARMER_NAME_PLACEHOLDER = 0xFF };
    register MessageScriptReplacement * result asm("r5") = output;
    asm ("" : "+l" (result));
    char text[sizeof(output->text)];
    if (placeholder == FARMER_NAME_PLACEHOLDER)
    {
        char const * name = substitution->name;
        size_t length = strlen(name);
        if (length > sizeof(text) - 1)
            length = sizeof(text) - 1;
        memcpy(text, name, length);
        text[length] = '\0';
        result->found = true;
        strcpy(result->text, text);
    }
    else
    {
        text[0] = '\0';
        result->found = false;
        strcpy(result->text, text);
    }
    return result;
}

EC SECTION(".text.f3.0805ED4C") ChildhoodMemoryScreen * InitChildhoodMemoryScreen(
    void * screen_storage, void * game_state)
{
    ChildhoodMemoryScreenLayout * self = static_cast<ChildhoodMemoryScreenLayout *>(screen_storage);
    struct
    {
        TileResources graphics;
        ResourcePool font;
        Vec2 key_repeat;
    } scratch;
    new (&scratch.key_repeat) Vec2(30, 1);
    InitScreenHardware(self, &scratch.key_repeat);
    self->vtable = gChildhoodMemoryScreenVTable;
    self->game_state = static_cast<ChildhoodMemoryGameState *>(game_state);
    self->dialogue_style = 0;

    MessageWindow * window_storage = static_cast<MessageWindow *>(__builtin_new(4));
    InitTileResources(&scratch.graphics);
    ResourcePool_Initialize(&scratch.font);
    self->dialogue = CreateMessageWindow(window_storage, 0, 31, 15, self->dialogue_style,
        &scratch.graphics, &scratch.font);
    DestroyResourcePool(&scratch.font, 2);
    DestroyTileResources(&scratch.graphics, 2);

    AudioPlayer_Initialize(&self->dialogue_audio, 1);
    AudioPlayer_Initialize(&self->music, 0);
    AudioPlayer_Initialize(&self->auxiliary_audio, 2);
    InitFarmerNameSubstitution(&self->farmer_name, nullptr);
    ClearMessageWindowButtonMask(self->dialogue, 0xB);
    self->scene_cue = 0;
    self->scene_phase = 0;
    self->dialogue_phase = 2;
    self->dialogue_line = 0;
    self->farmer_name.name = GetFarmerName(self->game_state->farmer);
    return static_cast<ChildhoodMemoryScreen *>(screen_storage);
}

// Preserve the original halfword stores to both the stack and the display shadow.
#define WRITE_SCENE_DISPLAY(field, value, scratch_reg, value_reg) { \
    register SceneDisplayRegisters * display asm("r0") = HardwareContext_GetDisplayRegisters(self); \
    register volatile u16 * scratch asm(scratch_reg) = &display_value; \
    asm ("" : : "r" (scratch), "r" (display)); \
    register u32 setting asm("r3") = (value); \
    asm ("" : : "r" (setting)); \
    register u32 output asm(value_reg) = setting; \
    asm ("" : : "r" (output)); \
    *scratch = output; \
    display->field = output; \
}

#define CLEAR_SCENE_SCROLL(background, axis) { \
    register SceneDisplayRegisters * display asm("r0") = HardwareContext_GetDisplayRegisters(self); \
    register volatile u16 * scratch asm("r1") = &display_value; \
    asm ("" : : "r" (scratch), "r" (display)); \
    *scratch = 0; \
    display->background_scroll[background][axis] = 0; \
}

struct OamBuffer;
struct FrameTaskList;
struct VBlankTaskList;

EC HardwareContextState * HardwareContext_GetState(ChildhoodMemoryScreenLayout *);
EC OamBuffer * HardwareContext_GetOamBuffer(ChildhoodMemoryScreenLayout *);
EC FrameTaskList * HardwareContext_GetFrameTasks(ChildhoodMemoryScreenLayout *);
EC VBlankTaskList * HardwareContext_GetVBlankTasks(ChildhoodMemoryScreenLayout *);
EC u32 UpdateMessageWindow(MessageWindow *, HardwareContextState *, OamBuffer *,
    SceneDisplayRegisters *, FrameTaskList *, VBlankTaskList *, AudioPlayer *);
EC void ClearMessageWindowSpeakerName(MessageWindow *);
EC void OpenMessageWindow(MessageWindow *);
EC void CloseMessageWindow(MessageWindow *);
EC void SetMessageWindowSpeakerName(MessageWindow *, char const *);
EC void SetMessageWindowTextSubstitutions(MessageWindow *, char const *, void const *);

EC char const gChildhoodMemorySpeakerMother[];
EC char const gChildhoodMemoryTripOpening[];
EC char const gChildhoodMemorySpeakerFather[];
EC char const gChildhoodMemoryFishingInvite[];
EC char const gChildhoodMemoryLostChild[];
EC char const gChildhoodMemoryLostChildReaction[];
EC char const gChildhoodMemorySpeakerOldMan[];
EC char const gChildhoodMemoryFoundChild[];
EC char const gChildhoodMemoryThankYou[];
EC char const gChildhoodMemoryCountryTrip[];
EC char const gChildhoodMemoryFarmInvite[];
EC char const gChildhoodMemoryFarmInviteReaction[];
EC char const gChildhoodMemoryWelcomeToFarm[];
EC char const gChildhoodMemoryFarmPlay[];
EC char const gChildhoodMemorySpeakerUnseenGirl[];
EC char const gChildhoodMemoryCallingOut[];
EC char const gChildhoodMemorySurprise[];
EC char const gChildhoodMemorySpeakerGirl[];
EC char const gChildhoodMemoryCheckingOnChild[];
EC char const gChildhoodMemoryPlayInvite[];
EC char const gChildhoodMemoryTellMeAboutYourself[];
EC char const gChildhoodMemoryFarmFarewell[];
EC char const gChildhoodMemoryLetterRequest[];
EC char const gChildhoodMemoryFarmAddress[];
EC char const gChildhoodMemoryLeavingReaction[];
EC char const gChildhoodMemoryPromiseToReturn[];
EC char const gChildhoodMemoryMadeAFriend[];
EC char const gChildhoodMemoryWaitingForLetter[];

EC SECTION(".text.f3.0805F5F4") void UpdateChildhoodMemoryDialogue(ChildhoodMemoryScreenLayout * self)
{
    enum
    {
        WINDOW_CLOSED = 0, WINDOW_TEXT_COMPLETE = 3,
        OPEN_REQUESTED = 0, CLOSING = 1, IDLE = 2, PREPARE_TEXT = 3,
        CLOSE_AFTER_LINE = 5, REOPEN_FOR_NEXT_LINE = 6,
    };
    enum
    {
        TRIP_OPENING = 0,
        FISHING_INVITE = 1,
        LOST_CHILD = 2,
        LOST_CHILD_REACTION = 3,
        FOUND_CHILD = 4,
        THANK_YOU = 5,
        COUNTRY_TRIP = 6,
        FARM_INVITE = 7,
        FARM_INVITE_REACTION = 9,
        WELCOME_TO_FARM = 10,
        FARM_PLAY = 11,
        CALLING_OUT = 14,
        SURPRISE = 15,
        CHECKING_ON_CHILD = 16,
        PLAY_INVITE = 17,
        TELL_ME_ABOUT_YOURSELF = 18,
        FARM_FAREWELL = 19,
        LETTER_REQUEST = 20,
        FARM_ADDRESS = 21,
        LEAVING_REACTION = 22,
        PROMISE_TO_RETURN = 23,
        MADE_A_FRIEND = 24,
        WAITING_FOR_LETTER = 25,
    };

    MessageWindow * window = self->dialogue;
    HardwareContextState * hardware_state = HardwareContext_GetState(self);
    OamBuffer * sprites = HardwareContext_GetOamBuffer(self);
    SceneDisplayRegisters * display = HardwareContext_GetDisplayRegisters(self);
    FrameTaskList * frame_tasks = HardwareContext_GetFrameTasks(self);
    VBlankTaskList * vblank_tasks = HardwareContext_GetVBlankTasks(self);

    register AudioPlayer * dialogue_audio asm("r1") = &self->dialogue_audio;
    asm ("" : "+r" (dialogue_audio));

    switch (UpdateMessageWindow(window, hardware_state, sprites, display,
        frame_tasks, vblank_tasks, dialogue_audio))
    {
    case WINDOW_CLOSED:
        if (self->dialogue_phase == OPEN_REQUESTED)
        {
            ClearMessageWindowSpeakerName(self->dialogue);
            OpenMessageWindow(self->dialogue);
            self->dialogue_phase = PREPARE_TEXT;
        }
        else if (self->dialogue_phase == REOPEN_FOR_NEXT_LINE)
            self->dialogue_phase = OPEN_REQUESTED;
        else
            self->dialogue_phase = IDLE;
        return;
    case WINDOW_TEXT_COMPLETE:
        if (self->dialogue_phase == CLOSE_AFTER_LINE)
        {
            CloseMessageWindow(self->dialogue);
            self->dialogue_phase = CLOSING;
        }
        else if (self->dialogue_phase == REOPEN_FOR_NEXT_LINE)
            CloseMessageWindow(self->dialogue);
        else if (self->dialogue_phase == PREPARE_TEXT)
        {
            self->dialogue_phase = CLOSE_AFTER_LINE;
            // The scene cues select the next line when a conversation pauses for animation.
            switch (self->dialogue_line)
            {
            case TRIP_OPENING:
                SetMessageWindowSpeakerName(self->dialogue, gChildhoodMemorySpeakerMother);
                SetMessageWindowTextSubstitutions(self->dialogue, gChildhoodMemoryTripOpening, &self->farmer_name);
                self->dialogue_phase = REOPEN_FOR_NEXT_LINE;
                self->dialogue_line = FISHING_INVITE;
                break;
            case FISHING_INVITE:
                SetMessageWindowSpeakerName(self->dialogue, gChildhoodMemorySpeakerFather);
                SetMessageWindowTextSubstitutions(self->dialogue, gChildhoodMemoryFishingInvite, &self->farmer_name);
                self->dialogue_phase = REOPEN_FOR_NEXT_LINE;
                self->dialogue_line = LOST_CHILD;
                break;
            case LOST_CHILD:
                SetMessageWindowSpeakerName(self->dialogue, gChildhoodMemorySpeakerMother);
                SetMessageWindowTextSubstitutions(self->dialogue, gChildhoodMemoryLostChild, &self->farmer_name);
                self->dialogue_phase = REOPEN_FOR_NEXT_LINE;
                self->dialogue_line = LOST_CHILD_REACTION;
                break;
            case LOST_CHILD_REACTION:
                SetMessageWindowSpeakerName(self->dialogue, gChildhoodMemorySpeakerFather);
                SetMessageWindowTextSubstitutions(self->dialogue, gChildhoodMemoryLostChildReaction, &self->farmer_name);
                break;
            case FOUND_CHILD:
                SetMessageWindowSpeakerName(self->dialogue, gChildhoodMemorySpeakerOldMan);
                SetMessageWindowTextSubstitutions(self->dialogue, gChildhoodMemoryFoundChild, &self->farmer_name);
                break;
            case THANK_YOU:
                SetMessageWindowSpeakerName(self->dialogue, gChildhoodMemorySpeakerMother);
                SetMessageWindowTextSubstitutions(self->dialogue, gChildhoodMemoryThankYou, &self->farmer_name);
                self->dialogue_phase = REOPEN_FOR_NEXT_LINE;
                self->dialogue_line = COUNTRY_TRIP;
                break;
            case COUNTRY_TRIP:
                SetMessageWindowSpeakerName(self->dialogue, gChildhoodMemorySpeakerFather);
                SetMessageWindowTextSubstitutions(self->dialogue, gChildhoodMemoryCountryTrip, &self->farmer_name);
                self->dialogue_phase = REOPEN_FOR_NEXT_LINE;
                self->dialogue_line = FARM_INVITE;
                break;
            case FARM_INVITE:
                SetMessageWindowSpeakerName(self->dialogue, gChildhoodMemorySpeakerOldMan);
                SetMessageWindowTextSubstitutions(self->dialogue, gChildhoodMemoryFarmInvite, &self->farmer_name);
                self->dialogue_phase = REOPEN_FOR_NEXT_LINE;
                self->dialogue_line = FARM_INVITE_REACTION;
                break;
            case FARM_INVITE_REACTION:
                SetMessageWindowSpeakerName(self->dialogue, gChildhoodMemorySpeakerFather);
                SetMessageWindowTextSubstitutions(self->dialogue, gChildhoodMemoryFarmInviteReaction, &self->farmer_name);
                self->dialogue_phase = REOPEN_FOR_NEXT_LINE;
                self->dialogue_line = WELCOME_TO_FARM;
                break;
            case WELCOME_TO_FARM:
                SetMessageWindowSpeakerName(self->dialogue, gChildhoodMemorySpeakerOldMan);
                SetMessageWindowTextSubstitutions(self->dialogue, gChildhoodMemoryWelcomeToFarm, &self->farmer_name);
                self->dialogue_phase = REOPEN_FOR_NEXT_LINE;
                self->dialogue_line = FARM_PLAY;
                break;
            case FARM_PLAY:
                SetMessageWindowSpeakerName(self->dialogue, gChildhoodMemorySpeakerMother);
                SetMessageWindowTextSubstitutions(self->dialogue, gChildhoodMemoryFarmPlay, &self->farmer_name);
                break;
            case CALLING_OUT:
                SetMessageWindowSpeakerName(self->dialogue, gChildhoodMemorySpeakerUnseenGirl);
                SetMessageWindowTextSubstitutions(self->dialogue, gChildhoodMemoryCallingOut, &self->farmer_name);
                self->dialogue_phase = REOPEN_FOR_NEXT_LINE;
                self->dialogue_line = SURPRISE;
                break;
            case SURPRISE:
                SetMessageWindowTextSubstitutions(self->dialogue, gChildhoodMemorySurprise, &self->farmer_name);
                break;
            case CHECKING_ON_CHILD:
                SetMessageWindowSpeakerName(self->dialogue, gChildhoodMemorySpeakerGirl);
                SetMessageWindowTextSubstitutions(self->dialogue, gChildhoodMemoryCheckingOnChild, &self->farmer_name);
                self->dialogue_phase = REOPEN_FOR_NEXT_LINE;
                self->dialogue_line = PLAY_INVITE;
                break;
            case PLAY_INVITE:
                SetMessageWindowSpeakerName(self->dialogue, gChildhoodMemorySpeakerGirl);
                SetMessageWindowTextSubstitutions(self->dialogue, gChildhoodMemoryPlayInvite, &self->farmer_name);
                self->dialogue_phase = REOPEN_FOR_NEXT_LINE;
                self->dialogue_line = TELL_ME_ABOUT_YOURSELF;
                break;
            case TELL_ME_ABOUT_YOURSELF:
                SetMessageWindowSpeakerName(self->dialogue, gChildhoodMemorySpeakerGirl);
                SetMessageWindowTextSubstitutions(self->dialogue, gChildhoodMemoryTellMeAboutYourself, &self->farmer_name);
                break;
            case FARM_FAREWELL:
                SetMessageWindowSpeakerName(self->dialogue, gChildhoodMemorySpeakerOldMan);
                SetMessageWindowTextSubstitutions(self->dialogue, gChildhoodMemoryFarmFarewell, &self->farmer_name);
                self->dialogue_phase = REOPEN_FOR_NEXT_LINE;
                self->dialogue_line = LETTER_REQUEST;
                break;
            case LETTER_REQUEST:
                SetMessageWindowSpeakerName(self->dialogue, gChildhoodMemorySpeakerOldMan);
                SetMessageWindowTextSubstitutions(self->dialogue, gChildhoodMemoryLetterRequest, &self->farmer_name);
                self->dialogue_phase = REOPEN_FOR_NEXT_LINE;
                self->dialogue_line = FARM_ADDRESS;
                break;
            case FARM_ADDRESS:
                SetMessageWindowSpeakerName(self->dialogue, gChildhoodMemorySpeakerOldMan);
                SetMessageWindowTextSubstitutions(self->dialogue, gChildhoodMemoryFarmAddress, &self->farmer_name);
                break;
            case LEAVING_REACTION:
                SetMessageWindowSpeakerName(self->dialogue, gChildhoodMemorySpeakerGirl);
                SetMessageWindowTextSubstitutions(self->dialogue, gChildhoodMemoryLeavingReaction, &self->farmer_name);
                self->dialogue_phase = REOPEN_FOR_NEXT_LINE;
                self->dialogue_line = PROMISE_TO_RETURN;
                break;
            case PROMISE_TO_RETURN:
                SetMessageWindowSpeakerName(self->dialogue, gChildhoodMemorySpeakerGirl);
                SetMessageWindowTextSubstitutions(self->dialogue, gChildhoodMemoryPromiseToReturn, &self->farmer_name);
                self->dialogue_phase = REOPEN_FOR_NEXT_LINE;
                self->dialogue_line = MADE_A_FRIEND;
                break;
            case MADE_A_FRIEND:
                SetMessageWindowSpeakerName(self->dialogue, gChildhoodMemorySpeakerOldMan);
                SetMessageWindowTextSubstitutions(self->dialogue, gChildhoodMemoryMadeAFriend, &self->farmer_name);
                break;
            case WAITING_FOR_LETTER:
                SetMessageWindowSpeakerName(self->dialogue, gChildhoodMemorySpeakerOldMan);
                SetMessageWindowTextSubstitutions(self->dialogue, gChildhoodMemoryWaitingForLetter, &self->farmer_name);
                break;
            }
        }
        return;
    case 1: case 2: case 4: case 5: case 6:
        return;
    }
}

EC SECTION(".text.f3.0805FBB8") void LoadChildhoodMemoryGraphics(ChildhoodMemoryScreenLayout * screen)
{
    register ChildhoodMemoryScreenLayout * self asm("r6") = screen;
    asm ("" : "+r" (self));
    enum
    {
        TILE_BYTES = 32,
        SCREEN_BLOCK_BYTES = 0x800,
        BLANK_MAP_ENTRY = 0x1066,
        BLANK_MAP_WORD = BLANK_MAP_ENTRY * 0x10001,
        OBJ_TILE_MAPPING_1D = 1 << 6,
        SHOW_BG2_BG3_OBJ = (1 << 10) | (1 << 11) | (1 << 12),
        BG_MOSAIC = 1 << 6,
        BG1_SCREEN_BLOCK = 28,
        BG2_SCREEN_BLOCK = 29,
        BG3_SCREEN_BLOCK = 30,
        VRAM = 0x06000000,
        BG_PALETTE_RAM = 0x05000000,
        TWO_PALETTE_BYTES = 2 * 16 * sizeof(u16),
        WHITE = 0x7FFF
    };
    volatile u16 display_value;
    Dma3_Fill(0, reinterpret_cast<void *>(VRAM + 0x7FE0), TILE_BYTES);
    Dma3_Fill(BLANK_MAP_WORD, reinterpret_cast<void *>(VRAM + BG1_SCREEN_BLOCK * SCREEN_BLOCK_BYTES),
        SCREEN_BLOCK_BYTES);
    Dma3_Fill(BLANK_MAP_WORD, reinterpret_cast<void *>(VRAM + BG2_SCREEN_BLOCK * SCREEN_BLOCK_BYTES),
        SCREEN_BLOCK_BYTES);
    Dma3_Fill(BLANK_MAP_WORD, reinterpret_cast<void *>(VRAM + BG3_SCREEN_BLOCK * SCREEN_BLOCK_BYTES),
        SCREEN_BLOCK_BYTES);
    Unpack(gChildhoodMemoryBackgroundTiles, reinterpret_cast<void *>(VRAM));
    register u16 const * palette_source asm("r0") = gChildhoodMemoryBackgroundPalette;
    asm ("" : "+r" (palette_source));
    u16 * palette = reinterpret_cast<u16 *>(BG_PALETTE_RAM);
    Dma3_Copy(palette_source, palette, TWO_PALETTE_BYTES);
    *palette = WHITE;

    WRITE_SCENE_DISPLAY(control, OBJ_TILE_MAPPING_1D | SHOW_BG2_BG3_OBJ, "r1", "r2")
    WRITE_SCENE_DISPLAY(background_control[3], (BG3_SCREEN_BLOCK << 8) | BG_MOSAIC | 3, "r2", "r1")
    WRITE_SCENE_DISPLAY(background_control[2], (BG2_SCREEN_BLOCK << 8) | BG_MOSAIC | 2, "r2", "r1")
    WRITE_SCENE_DISPLAY(background_control[1], (BG1_SCREEN_BLOCK << 8) | BG_MOSAIC | 1, "r2", "r1")
    CLEAR_SCENE_SCROLL(2, 0)
    CLEAR_SCENE_SCROLL(2, 1)
    CLEAR_SCENE_SCROLL(3, 0)
    CLEAR_SCENE_SCROLL(3, 1)

    u8 * map_destination = reinterpret_cast<u8 *>(&self->background_maps);
    void const * const * compressed_map = gChildhoodMemoryCompressedMaps;
    for (int remaining = CHILDHOOD_MAP_COUNT - 1; remaining >= 0; --remaining)
    {
        Unpack(*compressed_map++, map_destination);
        map_destination += CHILDHOOD_MAP_BYTES;
    }
}

#undef CLEAR_SCENE_SCROLL
#undef WRITE_SCENE_DISPLAY

struct ChildhoodMemoryScene
{
    void const * vtable;
    ChildhoodMemoryScreen * screen;
    AUnk_0800080C * next_scene;
};

EC char const gChildhoodMemorySceneVTable[];
EC AUnk_0800080C ** RunChildhoodMemoryScreen(AUnk_0800080C ** result, ChildhoodMemoryScreen * screen);

EC SECTION(".text.f3.0805FCD0") ChildhoodMemoryScene * InitChildhoodMemoryScene(ChildhoodMemoryScene * self, AUnk_0800080C ** next, void * game_state)
{
    self->vtable = gChildhoodMemorySceneVTable;
    self->screen = InitChildhoodMemoryScreen(__builtin_new(sizeof(ChildhoodMemoryScreenLayout)), game_state);
    AUnk_0800080C * next_scene = *next;
    *next = nullptr;
    self->next_scene = next_scene;
    return self;
}

EC SECTION(".text.f3.0805FD04") void DestroyChildhoodMemoryScene(ChildhoodMemoryScene * self, int flags)
{
    self->vtable = gChildhoodMemorySceneVTable;
    delete self->next_scene;
    delete self->screen;
    Scene_DestroyBase(self, flags);
}

EC SECTION(".text.f3.0805FD44") AUnk_0800080C ** RunChildhoodMemoryScene(AUnk_0800080C ** result, ChildhoodMemoryScene * self)
{
    AUnk_0800080C * screen_result;
    RunChildhoodMemoryScreen(&screen_result, self->screen);
    delete screen_result;
    AUnk_0800080C * next_scene = self->next_scene;
    self->next_scene = nullptr;
    *result = next_scene;
    return result;
}

struct HorseRaceScreen
{
    HardwareContextState * hardware;
    virtual ~HorseRaceScreen();
};

struct HorseRaceScene
{
    void const * vtable;
    HorseRaceScreen * screen;
    AUnk_0800080C * next_scene;
};

EC char const gHorseRaceSceneVTable[];
EC HorseRaceScreen * InitializeHorseRaceScreen(void * storage, void * game_state, u32 mode);
EC void RunHorseRaceScreen(AUnk_0800080C ** result, HorseRaceScreen * screen);

EC SECTION(".text.f3.0805CEB8") HorseRaceScene * InitHorseRaceScene(
    HorseRaceScene * self, AUnk_0800080C ** next, void * game_state, u8 mode)
{
    self->vtable = gHorseRaceSceneVTable;
    self->screen = InitializeHorseRaceScreen(__builtin_new(0x14FC), game_state, mode);
    AUnk_0800080C * next_scene = *next;
    *next = nullptr;
    self->next_scene = next_scene;
    return self;
}

EC SECTION(".text.f3.0805CEFC") void DestroyHorseRaceScene(HorseRaceScene * self, int flags)
{
    self->vtable = gHorseRaceSceneVTable;
    delete self->next_scene;
    delete self->screen;
    Scene_DestroyBase(self, flags);
}

EC SECTION(".text.f3.0805CF3C") AUnk_0800080C ** RunHorseRaceScene(
    AUnk_0800080C ** result, HorseRaceScene * self)
{
    AUnk_0800080C * screen_result;
    RunHorseRaceScreen(&screen_result, self->screen);
    delete screen_result;
    AUnk_0800080C * next_scene = self->next_scene;
    self->next_scene = nullptr;
    *result = next_scene;
    return result;
}

struct ChildhoodFrameTask;
struct ChildhoodTaskList;

struct ChildhoodTaskVtable
{
    void const * reserved[4];
    void (*append)(ChildhoodTaskList *, ChildhoodFrameTask *);
};

struct ChildhoodTaskList
{
    void * head;
    void * tail;
    ChildhoodTaskVtable const * vtable;
};

struct ChildhoodFadeRegisters
{
    STRUCT_PAD(0, 0x50);
    u16 blend_control;
};

struct ChildhoodDisplayFade
{
    u32 state;
};

struct ChildhoodFadeTaskVtable
{
    void const * reserved[3];
    void (*destroy)(ChildhoodFrameTask *, u32);
};

struct ChildhoodFrameTask
{
    void * previous_link;
    void * next;
    ChildhoodFadeTaskVtable const * vtable;
    ChildhoodDisplayFade fade;
};

struct ChildhoodMapQueueGrowth
{
    u32 occupied;
    u32 inserted;
};

struct ChildhoodResultVtable
{
    void const * reserved[2];
    void (*destroy)(AUnk_0800080C *, u32);
};

struct ChildhoodResultScene
{
    ChildhoodResultVtable const * vtable;
};

struct ChildhoodResultOwner
{
    AUnk_0800080C ** slot;
    AUnk_0800080C * value;
};

struct ChildhoodFrameWork
{
    u16 display_value;
    ChildhoodFrameTask * fade;
    union
    {
        DmaCommand copy;
        AUnk_0800080C * discarded_scene;
    };
    ChildhoodMapQueueGrowth bg3_growth;
    ChildhoodMapQueueGrowth bg1_growth;
    ChildhoodResultOwner empty_result;
    AUnk_0800080C ** result;
    u32 delay_frames;
    u32 scene_done;
    DmaCommand * bg3_command;
    DmaCommand * copy_command;
    ChildhoodResultOwner * empty_result_ptr;
    u32 map_index_times_four;
    DmaCommand * old_end;
};

EC void BlankChildhoodDisplay(ChildhoodMemoryScreenLayout *) asm("func_08008724");
EC ChildhoodFadeRegisters * GetChildhoodFadeRegisters(ChildhoodMemoryScreenLayout *) asm("HardwareContext_GetDisplayRegisters");
EC void InitializeChildhoodFade(ChildhoodDisplayFade *, ChildhoodFadeRegisters *) asm("DisplayFade_Initialize");
EC void StartChildhoodFadeIn(ChildhoodDisplayFade *, i32, u32, u32) asm("DisplayFade_StartIn");
EC void StartChildhoodFadeOut(ChildhoodDisplayFade *, i32, u32, u32) asm("DisplayFade_StartOut");
EC ChildhoodTaskList * GetChildhoodFrameTasks(ChildhoodMemoryScreenLayout *) asm("HardwareContext_GetFrameTasks");
EC DmaTransferQueue * GetChildhoodDmaQueue(ChildhoodMemoryScreenLayout *) asm("HardwareContext_GetDmaQueue");
EC void * RetryChildhoodQueueAllocation(u32) asm("func_080D3BC0");
EC void RenderChildhoodFrame(ChildhoodMemoryScreenLayout *) asm("func_080087C8");
EC void HardwareContext_UpdateKeys(ChildhoodMemoryScreenLayout *);
EC char const gDisplayFadeTaskVtable[];
EC void AudioPlayer_StartSong(AudioPlayer *, u32);

// agbcc must keep the original register assignments, branch layout, and literal pools.
#define MEMORY_PIN(value) asm ("" : "+l" (value))
#define MEMORY_LOAD(value, member) \
    asm volatile ("ldr %0, [sp, %1]" : "=l" (value) : "I" (offsetof(ChildhoodFrameWork, member)) : "memory")
#define MEMORY_SAVE(value, member) \
    asm volatile ("str %0, [sp, %1]" : : "l" (value), "I" (offsetof(ChildhoodFrameWork, member)) : "memory")
#define MEMORY_LITERAL(value, label) asm ("ldr %0, " label : "=l" (value))
#define MEMORY_LABEL(name) asm volatile (".Lmemory_" #name ":" : : : "memory")
#define MEMORY_BRANCH(name) asm volatile ("b .Lmemory_" #name : : : "memory")
#define MEMORY_FIELD(ptr, ptr_reg, offset_reg, label) \
    register uintptr_t ptr##_offset asm(offset_reg); \
    MEMORY_LITERAL(ptr##_offset, label); \
    register i16 * ptr asm(ptr_reg) = reinterpret_cast<i16 *>(reinterpret_cast<uintptr_t>(self) + ptr##_offset); \
    MEMORY_PIN(ptr)
#define MEMORY_WRITE(ptr, value, reg) do { \
    register i32 constant asm(reg) = value; \
    MEMORY_PIN(constant); \
    *ptr = constant; \
} while (0)
#define MEMORY_SIGNED(value, dest_reg, ptr, zero_reg) \
    register i32 value asm(dest_reg); \
    asm ("mov " zero_reg ", #0\n\tldrsh %0, [%1, " zero_reg "]" \
        : "=l" (value) : "l" (ptr) : zero_reg, "cc", "memory")
#define MEMORY_DELAY(frames, reg) do { \
    register u32 delay asm(reg) = (frames); \
    MEMORY_PIN(delay); \
    MEMORY_SAVE(delay, delay_frames); \
} while (0)
#define MEMORY_TICK(delay_reg, tick_reg, remaining_reg, label, target) do { \
    register u32 delay asm(delay_reg); \
    MEMORY_LOAD(delay, delay_frames); \
    register u32 timer asm("r0") = delay << 16; \
    MEMORY_PIN(timer); \
    register u32 tick asm(tick_reg); \
    MEMORY_LITERAL(tick, label); \
    timer += tick; \
    MEMORY_PIN(timer); \
    register u32 remaining asm(remaining_reg) = timer >> 16; \
    MEMORY_PIN(remaining); \
    MEMORY_SAVE(remaining, delay_frames); \
    asm volatile ("cmp %0, #0\n\tblt .Lmemory_" #target "\n\tb .Lmemory_update_dialogue" \
        : : "l" (timer) : "cc", "memory"); \
} while (0)
#define MEMORY_FADE_READY(target) do { \
    register ChildhoodFrameTask * task asm("r0"); \
    MEMORY_LOAD(task, fade); \
    register uintptr_t previous asm("r1") = reinterpret_cast<uintptr_t>(task->previous_link); \
    MEMORY_PIN(previous); \
    register i32 busy asm("r0"); \
    asm volatile ("neg %0, %1\n\torr %0, %1\n\tcmp %0, #0\n\tbge .Lmemory_" #target "\n\tb .Lmemory_update_dialogue" \
        : "=&l" (busy) : "l" (previous) : "cc", "memory"); \
} while (0)
#define MEMORY_WORD(label, value) \
    asm volatile (".balign 4, 0\n" label ": .word " value : : : "memory")
#define MEMORY_FIELD_WORD(label, member) \
    MEMORY_WORD(label, ".Lmemory_" #member)
#define CHILDHOOD_COPY_COMMAND(source, target, copy_set) \
    "ldmia " source "!, {" copy_set "}\n\tstmia " target "!, {" copy_set "}\n\t" \
    "ldr " source ", [" source "]\n\tstr " source ", [" target "]"
#define CHILDHOOD_BG3_COPY_SOURCE "ldr r1, [sp, %1]\n\t"
#define CHILDHOOD_BG1_COPY_SOURCE "mov r1, sl\n\t"
// Both maps grow the queue by max(occupied, inserted), but keep their commands in different places.
#define CHILDHOOD_GROW_MAP_QUEUE(growth, old_size_end, first_copy_end, destination_reg, setup_destination, load_command) do { \
    register DmaTransferQueue * queue asm("r6"); \
    register DmaCommand * tail asm("r1"); \
    asm ("" : "=l" (queue), "=l" (tail)); \
    MEMORY_SAVE(tail, old_end); \
    register u32 inserted asm("r0") = 1; \
    MEMORY_PIN(inserted); \
    MEMORY_SAVE(inserted, growth.inserted); \
    register DmaCommand * old_begin asm("r1") = queue->begin; \
    MEMORY_PIN(old_begin); \
    register DmaCommand * old_end asm(old_size_end); \
    MEMORY_LOAD(old_end, old_end); \
    register u32 occupied asm("r4"); \
    asm ("sub r0, %2, %1\n\tasr %0, r0, #4" : "=l" (occupied) \
        : "l" (old_begin), "l" (old_end) : "r0", "cc"); \
    MEMORY_SAVE(occupied, growth.occupied); \
    register u32 const * inserted_ptr asm("r2"); \
    asm ("add %0, sp, %1" : "=l" (inserted_ptr) : "I" (offsetof(ChildhoodFrameWork, growth.inserted))); \
    register u32 const * increment asm("r0"); \
    asm ("add %0, sp, %1" : "=l" (increment) : "I" (offsetof(ChildhoodFrameWork, growth.occupied))); \
    asm ("cmp %1, #1\n\tbhs 1f\n\tadd %0, %2, #0\n1:" : "+l" (increment) \
        : "l" (occupied), "l" (inserted_ptr) : "cc"); \
    register u32 add_count asm("r0") = *increment; \
    MEMORY_PIN(add_count); \
    register u32 capacity asm("r0") = occupied + add_count; \
    MEMORY_PIN(capacity); \
    register DmaCommand * allocation asm("r0"); \
    register u32 capacity_bytes asm("r9"); \
    asm volatile ("cmp %0, #0\n\tbeq 5f" : : "l" (capacity) : "cc", "memory"); \
    { \
        register u32 bytes asm("r4") = capacity * sizeof(DmaCommand); \
        MEMORY_PIN(bytes); \
        allocation = static_cast<DmaCommand *>(malloc(bytes)); \
        capacity_bytes = bytes; \
        asm ("" : "+h" (capacity_bytes), "+l" (allocation)); \
        asm volatile ("cmp %0, #0\n\tbne 4f" : : "l" (allocation) : "cc", "memory"); \
        { \
            allocation = static_cast<DmaCommand *>(RetryChildhoodQueueAllocation(capacity_bytes)); \
            MEMORY_PIN(allocation); \
        } \
        asm volatile ("4:" : "+l" (allocation)); \
        old_begin = queue->begin; \
        MEMORY_PIN(old_begin); \
        asm volatile ("b 6f" : : "l" (allocation), "l" (old_begin), "h" (capacity_bytes) : "memory"); \
    } \
    { \
        asm volatile ("5:" : : : "memory"); \
        allocation = nullptr; \
        MEMORY_PIN(allocation); \
        capacity_bytes = reinterpret_cast<uintptr_t>(allocation); \
        asm ("" : "+h" (capacity_bytes)); \
    } \
    asm volatile ("6:" : "+l" (allocation), "+l" (old_begin), "+h" (capacity_bytes)); \
    register DmaCommand * buffer asm("r4") = allocation; \
    MEMORY_PIN(buffer); \
    register DmaCommand * begin asm("r8") = buffer; \
    asm ("" : "+h" (begin)); \
    register DmaCommand * input asm("r2") = old_begin; \
    MEMORY_PIN(input); \
    register DmaCommand * output asm("r12") = begin; \
    asm ("" : "+h" (output)); \
    register DmaCommand * end asm(first_copy_end); \
    MEMORY_LOAD(end, old_end); \
    if (input != end) { \
        register DmaCommand * end asm("r1"); \
        do { \
            asm volatile ("mov " destination_reg ", %0\n\tcmp " destination_reg ", #0\n\tbeq 1f\n\t" \
                setup_destination "add r1, %1, #0\n\t" CHILDHOOD_COPY_COMMAND("r1", "r0", "r3,r4,r7") "\n1:" \
                : : "h" (output), "l" (input) : "r0", "r1", "r3", "r4", "r7", "cc", "memory"); \
            ++input; \
            MEMORY_PIN(input); \
            register u32 step asm("r0") = sizeof(DmaCommand); \
            MEMORY_PIN(step); \
            asm ("add %0, %1" : "+h" (output) : "l" (step)); \
            MEMORY_LOAD(end, old_end); \
        } while (input != end); \
    } \
    register DmaCommand * new_end asm("r4") = output; \
    MEMORY_PIN(new_end); \
    register u32 append_count asm("r0"); \
    MEMORY_LOAD(append_count, growth.inserted); \
    if (append_count == 1) { \
        asm volatile ("cmp %0, #0\n\tbeq 1f\n\tadd r0, %0, #0\n\t" load_command \
            CHILDHOOD_COPY_COMMAND("r1", "r0", "r2,r3,r7") "\n1:" \
            : : "l" (new_end), "I" (offsetof(ChildhoodFrameWork, bg3_command)) : "r0", "r1", "r2", "r3", "r7", "cc", "memory"); \
        ++new_end; \
        MEMORY_PIN(new_end); \
        asm volatile ("b 7f" : : "l" (new_end) : "memory"); \
    } \
    { \
        register u32 remaining asm("r12") = append_count; \
        asm volatile ("" : "+h" (remaining)); \
        register DmaCommand * output asm("r2") = new_end; \
        MEMORY_PIN(output); \
        asm volatile ("cmp %0, #0\n\tbeq 8f" : : "l" (append_count) : "cc", "memory"); \
        { \
            register u32 count asm("r1"); \
            asm volatile ("9:" : : : "memory"); \
            { \
                asm volatile ("cmp %0, #0\n\tbeq 1f\n\tadd r0, %0, #0\n\t" load_command \
                    CHILDHOOD_COPY_COMMAND("r1", "r0", "r3,r4,r7") "\n1:" \
                    : : "l" (output), "I" (offsetof(ChildhoodFrameWork, bg3_command)) : "r0", "r1", "r3", "r4", "r7", "cc", "memory"); \
                register u32 step asm("r0") = 1; \
                MEMORY_PIN(step); \
                asm volatile ("neg %1, %1\n\tadd %0, %1" : "+h" (remaining), "+l" (step) : : "cc"); \
                ++output; \
                MEMORY_PIN(output); \
                count = remaining; \
                asm volatile ("cmp %0, #0\n\tbne 9b" : : "l" (count) : "cc", "memory"); \
            } \
        } \
        asm volatile ("8:" : "+l" (output)); \
        new_end = output; \
        MEMORY_PIN(new_end); \
    } \
    asm volatile ("7:" : "+l" (new_end)); \
    register DmaCommand * discard_end asm("r2") = queue->end; \
    MEMORY_PIN(discard_end); \
    register DmaCommand * discard_begin asm("r1") = queue->begin; \
    MEMORY_PIN(discard_begin); \
    register DmaCommand * discard asm("r0") = discard_begin; \
    MEMORY_PIN(discard); \
    if (discard_begin != discard_end) { \
        do { \
            ++discard; \
            MEMORY_PIN(discard); \
        } while (discard != discard_end); \
    } \
    MEMORY_PIN(discard_begin); \
    asm volatile ("cmp %0, #0\n\tbeq 3f" : : "l" (discard_begin) : "cc", "memory"); \
    free(discard_begin); \
    asm volatile ("3:" : : : "memory"); \
    register uintptr_t limit asm("r0") = capacity_bytes; \
    MEMORY_PIN(limit); \
    asm ("add %0, %1" : "+l" (limit) : "h" (begin)); \
    register DmaCommand * new_begin asm("r2") = begin; \
    MEMORY_PIN(new_begin); \
    asm volatile ("str %1, [%0, %4]\n\tstr %2, [%0, %5]\n\tstr %3, [%0, %6]" \
        : : "l" (queue), "l" (new_begin), "l" (new_end), "l" (limit), \
          "I" (offsetof(DmaTransferQueue, begin)), "I" (offsetof(DmaTransferQueue, end)), \
          "I" (offsetof(DmaTransferQueue, capacity_end)) : "memory"); \
} while (0)
#define CHILDHOOD_APPEND_COMMAND(command, copy_register, advance_label) do { \
    register DmaCommand * destination asm("r0"); \
    asm volatile ("cmp %0, #0\n\tbeq .Lmemory_" #advance_label "\n\t" \
        "add %1, %0, #0\n\tadd %0, %2, #0\n\t" CHILDHOOD_COPY_COMMAND("%0", "%1", "r2,r3," copy_register) \
        : "+l" (tail), "=&l" (destination) : "l" (command) : "r2", "r3", copy_register, "cc", "memory"); \
    MEMORY_LABEL(advance_label); \
    register DmaCommand * end asm("r0") = queue->end; \
    MEMORY_PIN(end); \
    ++end; \
    MEMORY_PIN(end); \
    queue->end = end; \
} while (0)
#define MEMORY_START_LINE(label, offset_reg, literal, line) \
    MEMORY_LABEL(label); \
    { \
        MEMORY_FIELD(line_ptr, "r1", offset_reg, literal); \
        MEMORY_WRITE(line_ptr, line, "r0"); \
    } \
    MEMORY_BRANCH(update_dialogue); \
    MEMORY_FIELD_WORD(literal, dialogue_line)
#define MEMORY_DISPLAY_LAYERS(layers, value_reg) do { \
    register SceneDisplayRegisters * regs asm("r0") = HardwareContext_GetDisplayRegisters(self); \
    MEMORY_PIN(regs); \
    register u16 * stack_value asm("r2"); \
    asm ("mov %0, sp" : "=l" (stack_value)); \
    register u32 flags asm(value_reg) = (layers); \
    MEMORY_PIN(flags); \
    register u32 value asm("r1") = flags; \
    MEMORY_PIN(value); \
    *stack_value = value; \
    regs->control = value; \
} while (0)
#define MEMORY_MAP_PAIR(first_cue, correction) do { \
    register u32 cue_id asm("r0") = static_cast<u32>(first_cue); \
    register u32 map_index asm("r10"); \
    asm volatile ("sub %1, %2\n\tlsl %1, %1, #1\n\tmov %0, %1" \
        : "=h" (map_index), "+l" (cue_id) : "I" (correction) : "cc"); \
} while (0)

#define QUEUE_CHILDHOOD_FADE() do { \
    register ChildhoodTaskList * tasks asm("r0") = GetChildhoodFrameTasks(self); \
    register ChildhoodTaskVtable const * vtable asm("r2") = tasks->vtable; \
    asm ("" : "+l" (tasks), "+l" (vtable)); \
    register ChildhoodFrameTask * task asm("r1"); \
    MEMORY_LOAD(task, fade); \
    register void (*append)(ChildhoodTaskList *, ChildhoodFrameTask *) asm("r2") = vtable->append; \
    MEMORY_PIN(append); \
    append(tasks, task); \
} while (0)
#define START_CHILDHOOD_FADE(fade_function, mode, use_mosaic, label) do { \
    register ChildhoodFrameTask * task asm("r0"); \
    MEMORY_LOAD(task, fade); \
    register i32 step asm("r1"); \
    MEMORY_LITERAL(step, label); \
    register ChildhoodDisplayFade * controller asm("r0") = &task->fade; \
    MEMORY_PIN(controller); \
    register u32 direction asm("r2") = mode; \
    MEMORY_PIN(direction); \
    register u32 mosaic asm("r3") = use_mosaic; \
    MEMORY_PIN(mosaic); \
    fade_function(controller, step, direction, mosaic); \
    QUEUE_CHILDHOOD_FADE(); \
} while (0)

EC SECTION(".text.f3.0805EE44") AUnk_0800080C ** RunChildhoodMemoryScreen(
    AUnk_0800080C ** output, ChildhoodMemoryScreen * screen)
{
    enum { BLEND_ALL_FIRST_TARGETS = 0x3F, FLASHBACK_SONG = 0x25,
        FADE_BRIGHTEN = 1, FADE_DARKEN = 2 };
    enum { START_DARK_FADE = 1, PAUSE_BEFORE_FADE, START_LIGHT_FADE, WAIT_FOR_FADE_IN,
        PAUSE_BEFORE_TEXT, WAIT_TEXT, PAUSE_WITHOUT_TEXT, ADVANCE_CUE, WAIT_FOR_TRANSITION, EXIT_AFTER_FADE };
    enum { LOST_CHILD = 1, UNSEEN_GIRL = 9, CABIN_FAREWELL = 11, GIRL_FAREWELL = 12, LETTER_FAREWELL = 13 };
    enum { LONG_DIALOGUE_PAUSE = 60, SILENT_PAUSE = 180, FAREWELL_PAUSE = 120 };
    enum { DIALOGUE_REQUESTED = 0, DIALOGUE_IDLE = 2 };
    enum { TRIP_LINE = 0, FOUND_CHILD_LINE = 4, PARENTS_THANK_YOU_LINE = 5, CALLING_OUT_LINE = 14,
        MEET_GIRL_LINE = 16, FARM_FAREWELL_LINE = 19, LEAVING_REACTION_LINE = 22, WAITING_FOR_LETTER_LINE = 25 };
    enum { TILE_MAPPING_1D = 1 << 6, DISPLAY_BG1 = 1 << 9, DISPLAY_BG2 = 1 << 10,
        DISPLAY_BG3 = 1 << 11, DISPLAY_OBJ = 1 << 12, KEEP_BACKGROUND = -1 };
    // With agbcc's Thumb operand printer, .word literals cannot take C++ immediates.
    typedef char SceneCueOffset[offsetof(ChildhoodMemoryScreenLayout, scene_cue) == 0x6420 ? 1 : -1];
    typedef char ScenePhaseOffset[offsetof(ChildhoodMemoryScreenLayout, scene_phase) == 0x6422 ? 1 : -1];
    typedef char DialogueLineOffset[offsetof(ChildhoodMemoryScreenLayout, dialogue_line) == 0x6424 ? 1 : -1];
    typedef char DialoguePhaseOffset[offsetof(ChildhoodMemoryScreenLayout, dialogue_phase) == 0x6426 ? 1 : -1];
    asm volatile (
        ".equ .Lmemory_scene_cue, 0x6420\n"
        ".equ .Lmemory_scene_phase, 0x6422\n"
        ".equ .Lmemory_dialogue_line, 0x6424\n"
        ".equ .Lmemory_dialogue_phase, 0x6426\n"
        ".equ .Lmemory_fade_step, 0x889\n"
        ".equ .Lmemory_timer_tick, -0x10000\n"
        ".equ .Lmemory_bg1_tilemap_vram, 0x0600E800\n"
        ".equ .Lmemory_bg3_tilemap_vram, 0x0600F000");
    typedef char FrameSize[sizeof(ChildhoodFrameWork) == 0x50 ? 1 : -1];
    ChildhoodFrameWork work;
    register AUnk_0800080C ** result asm("r0") = output;
    MEMORY_SAVE(result, result);
    register ChildhoodMemoryScreenLayout * self asm("r5") = reinterpret_cast<ChildhoodMemoryScreenLayout *>(screen);
    MEMORY_PIN(self);
    register u32 reservation asm("r0");
    asm ("" : "=l" (reservation) : "m" (work));
    register u32 zero asm("r0") = 0;
    MEMORY_PIN(zero);
    MEMORY_SAVE(zero, delay_frames);
    register u32 not_done asm("r1") = 0;
    MEMORY_PIN(not_done);
    MEMORY_SAVE(not_done, scene_done);

    BlankChildhoodDisplay(self);
    register ChildhoodFadeRegisters * regs asm("r0") = GetChildhoodFadeRegisters(self);
    MEMORY_PIN(regs);
    register u16 * display asm("r1");
    asm ("mov %0, sp" : "=l" (display));
    register u32 display_value asm("r2") = BLEND_ALL_FIRST_TARGETS;
    MEMORY_PIN(display_value);
    *display = display_value;
    asm volatile ("add %0, %2\n\tstrh %1, [%0]" : "+l" (regs) : "l" (display_value),
        "I" (offsetof(ChildhoodFadeRegisters, blend_control)) : "cc", "memory");

    register ChildhoodFrameTask * fade asm("r4") = static_cast<ChildhoodFrameTask *>(__builtin_new(sizeof(ChildhoodFrameTask)));
    MEMORY_PIN(fade);
    register ChildhoodFadeRegisters * fade_regs asm("r1") = GetChildhoodFadeRegisters(self);
    MEMORY_PIN(fade_regs);
    register uintptr_t empty_link asm("r2");
    MEMORY_LOAD(empty_link, scene_done);
    fade->previous_link = reinterpret_cast<void *>(empty_link);
    fade->next = reinterpret_cast<void *>(empty_link);
    register ChildhoodFadeTaskVtable const * vtable asm("r0");
    MEMORY_LITERAL(vtable, ".Lmemory_opening_fade_vtable");
    fade->vtable = vtable;
    register ChildhoodDisplayFade * controller asm("r0") = &fade->fade;
    MEMORY_PIN(controller);
    InitializeChildhoodFade(controller, fade_regs);
    MEMORY_SAVE(fade, fade);
    LoadChildhoodMemoryGraphics(self);
    register AudioPlayer * music asm("r0");
    asm ("add %0, %1, #0\n\tadd %0, %2" : "=l" (music) : "l" (self), "I" (offsetof(ChildhoodMemoryScreenLayout, music)) : "cc");
    register u32 song asm("r1") = FLASHBACK_SONG;
    MEMORY_PIN(song);
    AudioPlayer_StartSong(music, song);

    register ChildhoodFrameTask * fade_task asm("r0");
    MEMORY_LOAD(fade_task, fade);
    register i32 fade_step asm("r1");
    MEMORY_LITERAL(fade_step, ".Lmemory_opening_fade_step");
    register ChildhoodDisplayFade * opening asm("r0") = &fade_task->fade;
    MEMORY_PIN(opening);
    register u32 brighten asm("r2") = FADE_BRIGHTEN;
    MEMORY_PIN(brighten);
    register u32 no_mosaic asm("r3") = 0;
    MEMORY_PIN(no_mosaic);
    StartChildhoodFadeIn(opening, fade_step, brighten, no_mosaic);
    QUEUE_CHILDHOOD_FADE();

    register uintptr_t phase_offset asm("r3");
    MEMORY_LITERAL(phase_offset, ".Lmemory_opening_phase_offset");
    register i16 * phase asm("r1") =
        reinterpret_cast<i16 *>(reinterpret_cast<uintptr_t>(self) + phase_offset);
    MEMORY_PIN(phase);
    register u32 wait asm("r0") = WAIT_FOR_FADE_IN;
    MEMORY_PIN(wait);
    *phase = wait;

    register DmaCommand * copy asm("r4");
    asm ("mov %0, sp\n\tadd %0, %1" : "=l" (copy) : "I" (offsetof(ChildhoodFrameWork, copy)) : "cc");
    MEMORY_SAVE(copy, copy_command);
    register u64 coupled asm("r6");
    asm ("" : "=l" (coupled));
    asm volatile ("mov %H0, sp\n\tadd %H0, %1\n\tstr %H0, [sp, %2]" : "+l" (coupled)
        : "I" (offsetof(ChildhoodFrameWork, empty_result)), "I" (offsetof(ChildhoodFrameWork, empty_result_ptr))
        : "cc", "memory");


    // The opening and farewells use dialogue; the montage advances on a timer.
    MEMORY_LABEL(next_frame);
    RenderChildhoodFrame(self);
    HardwareContext_UpdateKeys(self);
    {
        MEMORY_FIELD(phase, "r0", "r1", ".Lmemory_opening_phase_offset");
        register i32 phase_index asm("r0") = *reinterpret_cast<u16 *>(phase);
        MEMORY_PIN(phase_index);
        asm ("sub %0, %1\n\tlsl %0, %0, #16\n\tasr %0, %0, #16" : "+l" (phase_index)
            : "I" (START_DARK_FADE) : "cc");
        asm volatile ("cmp %0, %1\n\tbls .Lmemory_dispatch_phase\n\tb .Lmemory_update_dialogue" :
            : "l" (phase_index), "I" (EXIT_AFTER_FADE - START_DARK_FADE) : "cc", "memory");
    }

    // Dispatch entries are even addresses inside this function, not Thumb function pointers.
    asm volatile (
        ".Lmemory_dispatch_phase:\n"
        "lsl r0, r0, #2\n"
        "ldr r1, .Lmemory_phase_table_address\n"
        "add r0, r0, r1\n"
        "ldr r0, [r0]\n"
        "mov pc, r0\n"
        ".balign 4, 0\n"
        ".Lmemory_opening_fade_vtable: .word gDisplayFadeTaskVtable\n"
        ".Lmemory_opening_fade_step: .word .Lmemory_fade_step\n"
        ".Lmemory_opening_phase_offset: .word .Lmemory_scene_phase\n"
        ".Lmemory_phase_table_address: .word .Lmemory_phase_table\n"
        ".Lmemory_phase_table:\n"
        ".word .Lmemory_fade_in_darken\n"
        ".word .Lmemory_wait_before_fade_in\n"
        ".word .Lmemory_fade_in_brighten\n"
        ".word .Lmemory_wait_for_fade_in\n"
        ".word .Lmemory_wait_before_dialogue\n"
        ".word .Lmemory_wait_for_dialogue\n"
        ".word .Lmemory_wait_silent_pause\n"
        ".word .Lmemory_advance_cue\n"
        ".word .Lmemory_wait_for_transition\n"
        ".word .Lmemory_wait_for_exit\n"
        : : : "r1", "cc", "memory");
    MEMORY_LABEL(fade_in_darken);
    START_CHILDHOOD_FADE(StartChildhoodFadeIn, FADE_DARKEN, false, ".Lmemory_darken_fade_step");
    {
        MEMORY_FIELD(phase, "r1", "r2", ".Lmemory_darken_phase_offset");
        asm volatile ("b .Lmemory_store_wait_fade_in_phase" : : "l" (phase) : "memory");
    }

    asm volatile (
        ".balign 4, 0\n"
        ".Lmemory_darken_fade_step: .word .Lmemory_fade_step\n"
        ".Lmemory_darken_phase_offset: .word .Lmemory_scene_phase\n"
        : : : "memory");
    MEMORY_LABEL(wait_before_fade_in);
    MEMORY_TICK("r3", "r4", "r7", ".Lmemory_pre_fade_timer_tick", begin_bright_fade_in);
    MEMORY_LABEL(begin_bright_fade_in);
    {
        MEMORY_FIELD(phase, "r1", "r0", ".Lmemory_pre_fade_phase_offset");
        MEMORY_WRITE(phase, START_LIGHT_FADE, "r0");
    }
    MEMORY_BRANCH(update_dialogue);

    asm volatile (
        ".balign 4, 0\n"
        ".Lmemory_pre_fade_timer_tick: .word .Lmemory_timer_tick\n"
        ".Lmemory_pre_fade_phase_offset: .word .Lmemory_scene_phase\n"
        : : : "memory");
    MEMORY_LABEL(fade_in_brighten);
    START_CHILDHOOD_FADE(StartChildhoodFadeIn, FADE_BRIGHTEN, false, ".Lmemory_brighten_fade_step");
    {
        MEMORY_FIELD(phase, "r1", "r2", ".Lmemory_brighten_phase_offset");
        asm volatile ("b .Lmemory_store_wait_fade_in_phase" : : "l" (phase) : "memory");
    }

    asm volatile (
        ".balign 4, 0\n"
        ".Lmemory_brighten_fade_step: .word .Lmemory_fade_step\n"
        ".Lmemory_brighten_phase_offset: .word .Lmemory_scene_phase\n"
        : : : "memory");
    MEMORY_LABEL(wait_for_fade_in);
    MEMORY_FADE_READY(start_dialogue_pause);
    MEMORY_LABEL(start_dialogue_pause);
    {
        MEMORY_FIELD(phase, "r0", "r3", ".Lmemory_post_fade_phase_offset");
        MEMORY_WRITE(phase, PAUSE_BEFORE_TEXT, "r1");
        MEMORY_FIELD(cue, "r0", "r4", ".Lmemory_post_fade_cue_offset");
        MEMORY_SIGNED(cue_id, "r0", cue, "r7");
        asm volatile ("cmp %0, %1\n\tbeq .Lmemory_use_longer_dialogue_pause\n\t"
            "cmp %0, %1\n\tblt .Lmemory_skip_dialogue_pause\n\t"
            "cmp %0, %2\n\tbgt .Lmemory_skip_dialogue_pause\n\t"
            "cmp %0, %3\n\tblt .Lmemory_skip_dialogue_pause"
            : : "l" (cue_id), "I" (LOST_CHILD), "I" (GIRL_FAREWELL), "I" (CABIN_FAREWELL) : "cc", "memory");
    }
    MEMORY_LABEL(use_longer_dialogue_pause);
    MEMORY_DELAY(LONG_DIALOGUE_PAUSE, "r0");
    MEMORY_BRANCH(update_dialogue);

    asm volatile (
        ".balign 4, 0\n"
        ".Lmemory_post_fade_phase_offset: .word .Lmemory_scene_phase\n"
        ".Lmemory_post_fade_cue_offset: .word .Lmemory_scene_cue\n"
        : : : "memory");
    MEMORY_LABEL(skip_dialogue_pause);
    MEMORY_DELAY(0, "r1");
    MEMORY_BRANCH(update_dialogue);

    MEMORY_LABEL(wait_before_dialogue);
    MEMORY_TICK("r2", "r3", "r4", ".Lmemory_pre_dialogue_timer_tick", request_dialogue);
    MEMORY_LABEL(request_dialogue);
    {
        MEMORY_FIELD(phase, "r1", "r7", ".Lmemory_pre_dialogue_phase_offset");
        register i32 open_dialogue asm("r2") = DIALOGUE_REQUESTED;
        MEMORY_PIN(open_dialogue);
        MEMORY_WRITE(phase, WAIT_TEXT, "r0");
        MEMORY_FIELD(dialogue_phase, "r0", "r1", ".Lmemory_open_dialogue_phase_offset");
        *dialogue_phase = open_dialogue;
        MEMORY_FIELD(cue, "r0", "r2", ".Lmemory_dialogue_cue_offset");
        MEMORY_SIGNED(cue_id, "r0", cue, "r3");
        asm volatile ("cmp %0, %1\n\tbhi .Lmemory_pause_without_dialogue\n\t"
            "lsl %0, %0, #2\n\tldr r1, .Lmemory_dialogue_table_address\n\tadd %0, %0, r1\n\tldr %0, [%0]\n\tmov pc, %0"
            : "+l" (cue_id) : "I" (LETTER_FAREWELL) : "r1", "cc", "memory");
    }

    asm volatile (
        ".balign 4, 0\n"
        ".Lmemory_pre_dialogue_timer_tick: .word .Lmemory_timer_tick\n"
        ".Lmemory_pre_dialogue_phase_offset: .word .Lmemory_scene_phase\n"
        ".Lmemory_open_dialogue_phase_offset: .word .Lmemory_dialogue_phase\n"
        ".Lmemory_dialogue_cue_offset: .word .Lmemory_scene_cue\n"
        ".Lmemory_dialogue_table_address: .word .Lmemory_dialogue_cue_table\n"
        ".Lmemory_dialogue_cue_table:\n"
        ".word .Lmemory_start_trip_dialogue\n"
        ".word .Lmemory_start_lost_child_dialogue\n"
        ".word .Lmemory_start_parents_thank_you\n"
        ".word .Lmemory_pause_without_dialogue\n"
        ".word .Lmemory_pause_without_dialogue\n"
        ".word .Lmemory_pause_without_dialogue\n"
        ".word .Lmemory_pause_without_dialogue\n"
        ".word .Lmemory_pause_without_dialogue\n"
        ".word .Lmemory_pause_without_dialogue\n"
        ".word .Lmemory_start_unseen_girl_dialogue\n"
        ".word .Lmemory_start_girl_dialogue\n"
        ".word .Lmemory_start_farewell_dialogue\n"
        ".word .Lmemory_start_departure_dialogue\n"
        ".word .Lmemory_start_final_line\n"
        : : : "memory");
    MEMORY_START_LINE(start_trip_dialogue, "r4", ".Lmemory_trip_line_offset", TRIP_LINE);
    MEMORY_LABEL(start_lost_child_dialogue);
    {
        MEMORY_FIELD(line_ptr, "r1", "r7", ".Lmemory_lost_child_line_offset");
        typedef char SharedPhaseAndLineValue[int(FOUND_CHILD_LINE) == int(WAIT_FOR_FADE_IN) ? 1 : -1];
        asm volatile (".Lmemory_store_wait_fade_in_phase:" : "+l" (line_ptr) : : "memory");
        MEMORY_WRITE(line_ptr, FOUND_CHILD_LINE, "r0");
    }
    MEMORY_BRANCH(update_dialogue);
    MEMORY_FIELD_WORD(".Lmemory_lost_child_line_offset", dialogue_line);
    MEMORY_START_LINE(start_parents_thank_you, "r0", ".Lmemory_thanks_line_offset", PARENTS_THANK_YOU_LINE);
    MEMORY_START_LINE(start_unseen_girl_dialogue, "r2", ".Lmemory_calling_out_line_offset", CALLING_OUT_LINE);
    MEMORY_START_LINE(start_girl_dialogue, "r3", ".Lmemory_meet_girl_line_offset", MEET_GIRL_LINE);
    MEMORY_START_LINE(start_farewell_dialogue, "r4", ".Lmemory_farewell_line_offset", FARM_FAREWELL_LINE);
    MEMORY_START_LINE(start_departure_dialogue, "r7", ".Lmemory_departure_line_offset", LEAVING_REACTION_LINE);
    MEMORY_START_LINE(start_final_line, "r0", ".Lmemory_final_line_offset", WAITING_FOR_LETTER_LINE);
    MEMORY_LABEL(pause_without_dialogue);
    MEMORY_DELAY(SILENT_PAUSE, "r1");
    {
        MEMORY_FIELD(dialogue_phase, "r1", "r2", ".Lmemory_silent_dialogue_phase_offset");
        MEMORY_WRITE(dialogue_phase, DIALOGUE_IDLE, "r0");
        MEMORY_FIELD(phase, "r1", "r3", ".Lmemory_silent_scene_phase_offset");
        MEMORY_WRITE(phase, PAUSE_WITHOUT_TEXT, "r0");
    }
    MEMORY_BRANCH(update_dialogue);

    asm volatile (
        ".balign 4, 0\n"
        ".Lmemory_silent_dialogue_phase_offset: .word .Lmemory_dialogue_phase\n"
        ".Lmemory_silent_scene_phase_offset: .word .Lmemory_scene_phase\n"
        : : : "memory");
    MEMORY_LABEL(wait_for_dialogue);
    {
        MEMORY_FIELD(dialogue_phase, "r0", "r4", ".Lmemory_check_dialogue_phase_offset");
        MEMORY_SIGNED(state, "r0", dialogue_phase, "r7");
        asm volatile ("cmp %0, %1\n\tbeq .Lmemory_advance_after_dialogue\n\tb .Lmemory_update_dialogue"
            : : "l" (state), "I" (DIALOGUE_IDLE) : "cc", "memory");
    }
    MEMORY_LABEL(advance_after_dialogue);
    {
        MEMORY_FIELD(phase, "r1", "r0", ".Lmemory_post_dialogue_phase_offset");
        asm volatile ("b .Lmemory_store_advance_cue_phase" : : "l" (phase) : "memory");
    }

    asm volatile (
        ".balign 4, 0\n"
        ".Lmemory_check_dialogue_phase_offset: .word .Lmemory_dialogue_phase\n"
        ".Lmemory_post_dialogue_phase_offset: .word .Lmemory_scene_phase\n"
        : : : "memory");
    MEMORY_LABEL(wait_silent_pause);
    MEMORY_TICK("r1", "r2", "r3", ".Lmemory_silent_timer_tick", advance_after_silent_pause);
    MEMORY_LABEL(advance_after_silent_pause);
    {
        MEMORY_FIELD(phase, "r1", "r4", ".Lmemory_post_silence_phase_offset");
        asm volatile (".Lmemory_store_advance_cue_phase:" : "+l" (phase) : : "memory");
        MEMORY_WRITE(phase, ADVANCE_CUE, "r0");
    }
    MEMORY_BRANCH(update_dialogue);

    asm volatile (
        ".balign 4, 0\n"
        ".Lmemory_silent_timer_tick: .word .Lmemory_timer_tick\n"
        ".Lmemory_post_silence_phase_offset: .word .Lmemory_scene_phase\n"
        : : : "memory");
    MEMORY_LABEL(advance_cue);
    {
        MEMORY_FIELD(cue, "r1", "r7", ".Lmemory_advance_cue_offset");
        register u32 previous asm("r2") = static_cast<u16>(*cue);
        MEMORY_PIN(previous);
        register u32 next asm("r0") = previous + 1;
        MEMORY_PIN(next);
        *cue = next;
        // Fade through white for the unseen girl, meeting the girl, and the cabin farewell.
        asm volatile (
            "lsl %0, %0, #16\n\tasr %0, %0, #16\n\tcmp %0, %2\n\tbgt .Lmemory_fade_out_to_exit\n\t"
            "add %0, %1, #0\n\tsub %0, %3\n\tlsl %0, %0, #16\n\tlsr %0, %0, #16\n\t"
            "cmp %0, %4\n\tbhi .Lmemory_fade_out_darken"
            : "+l" (next) : "l" (previous), "I" (LETTER_FAREWELL), "I" (UNSEEN_GIRL - 1),
                "I" (CABIN_FAREWELL - UNSEEN_GIRL) : "cc", "memory");
    }
    START_CHILDHOOD_FADE(StartChildhoodFadeOut, FADE_BRIGHTEN, false, ".Lmemory_brighten_out_step");
    MEMORY_BRANCH(begin_wait_for_transition);

    asm volatile (
        ".balign 4, 0\n"
        ".Lmemory_advance_cue_offset: .word .Lmemory_scene_cue\n"
        ".Lmemory_brighten_out_step: .word .Lmemory_fade_step\n"
        : : : "memory");
    MEMORY_LABEL(fade_out_darken);
    START_CHILDHOOD_FADE(StartChildhoodFadeOut, FADE_DARKEN, false, ".Lmemory_darken_out_step");
    MEMORY_LABEL(begin_wait_for_transition);
    {
        MEMORY_FIELD(phase, "r1", "r0", ".Lmemory_transition_phase_offset");
        register i32 wait_phase asm("r0") = WAIT_FOR_TRANSITION;
        MEMORY_PIN(wait_phase);
        asm volatile ("b .Lmemory_store_wait_phase" : : "l" (phase), "l" (wait_phase) : "memory");
    }

    asm volatile (
        ".balign 4, 0\n"
        ".Lmemory_darken_out_step: .word .Lmemory_fade_step\n"
        ".Lmemory_transition_phase_offset: .word .Lmemory_scene_phase\n"
        : : : "memory");
    MEMORY_LABEL(fade_out_to_exit);
    START_CHILDHOOD_FADE(StartChildhoodFadeOut, FADE_BRIGHTEN, true, ".Lmemory_exit_fade_step");
    {
        MEMORY_FIELD(phase, "r1", "r2", ".Lmemory_exit_phase_offset");
        register i32 wait_phase asm("r0") = EXIT_AFTER_FADE;
        MEMORY_PIN(wait_phase);
        asm volatile (".Lmemory_store_wait_phase:" : "+l" (phase), "+l" (wait_phase) : : "memory");
        *phase = wait_phase;
    }
    MEMORY_LABEL(wait_for_transition);
    MEMORY_FADE_READY(select_next_background);
    // A cue changes backgrounds only after the previous fade completes.
    MEMORY_LABEL(select_next_background);
    {
        register i32 initial_index asm("r3") = -KEEP_BACKGROUND;
        MEMORY_PIN(initial_index);
        register i32 map_index asm("r10");
        asm volatile ("neg %1, %1\n\tmov %0, %1" : "=h" (map_index), "+l" (initial_index) : : "cc");
        MEMORY_FIELD(cue, "r0", "r4", ".Lmemory_background_cue_offset");
        register i32 cue_index asm("r0") = static_cast<u16>(*cue);
        MEMORY_PIN(cue_index);
        asm ("sub %0, %1\n\tlsl %0, %0, #16\n\tasr %0, %0, #16" : "+l" (cue_index) : "I" (LOST_CHILD) : "cc");
        asm volatile ("cmp %0, %1\n\tbls .Lmemory_dispatch_background_cue\n\tb .Lmemory_check_for_background_upload"
            : : "l" (cue_index), "I" (LETTER_FAREWELL - LOST_CHILD) : "cc", "memory");
    }
    MEMORY_LABEL(dispatch_background_cue);
    {
        register i32 cue_index asm("r0");
        asm ("" : "=l" (cue_index));
        asm volatile ("lsl %0, %0, #2\n\tldr r1, .Lmemory_background_table_address\n\tadd %0, %0, r1\n\tldr %0, [%0]\n\tmov pc, %0"
            : "+l" (cue_index) : : "r1", "cc", "memory");
    }

    asm volatile (
        ".balign 4, 0\n"
        ".Lmemory_exit_fade_step: .word .Lmemory_fade_step\n"
        ".Lmemory_exit_phase_offset: .word .Lmemory_scene_phase\n"
        ".Lmemory_background_cue_offset: .word .Lmemory_scene_cue\n"
        ".Lmemory_background_table_address: .word .Lmemory_background_cue_table\n"
        ".Lmemory_background_cue_table:\n"
        ".word .Lmemory_show_first_background_pair\n"
        ".word .Lmemory_show_bg1_only\n"
        ".word .Lmemory_show_montage_background_pair\n"
        ".word .Lmemory_show_montage_background_pair\n"
        ".word .Lmemory_show_montage_background_pair\n"
        ".word .Lmemory_show_montage_background_pair\n"
        ".word .Lmemory_show_montage_background_pair\n"
        ".word .Lmemory_show_montage_background_pair\n"
        ".word .Lmemory_hide_backgrounds\n"
        ".word .Lmemory_show_girl_background_pair\n"
        ".word .Lmemory_show_farewell_background_pair\n"
        ".word .Lmemory_show_departure_background_pair\n"
        ".word .Lmemory_show_bg1_only\n"
        : : : "memory");
    MEMORY_LABEL(show_first_background_pair);
    {
        register u32 first_map asm("r7") = 0;
        MEMORY_PIN(first_map);
        register i32 map_index asm("r10");
        asm volatile ("mov %0, %1" : "=h" (map_index) : "l" (first_map));
        MEMORY_FIELD(phase, "r1", "r0", ".Lmemory_first_map_phase_offset");
        asm volatile ("b .Lmemory_store_darken_fade_in_phase" : : "l" (phase) : "memory");
    }
    MEMORY_FIELD_WORD(".Lmemory_first_map_phase_offset", scene_phase);

    MEMORY_LABEL(show_bg1_only);
    MEMORY_DISPLAY_LAYERS(TILE_MAPPING_1D | DISPLAY_OBJ | DISPLAY_BG1, "r3");
    {
        MEMORY_FIELD(phase, "r1", "r4", ".Lmemory_single_layer_phase_offset");
        asm volatile ("b .Lmemory_store_darken_fade_in_phase" : : "l" (phase) : "memory");
    }
    MEMORY_FIELD_WORD(".Lmemory_single_layer_phase_offset", scene_phase);

    MEMORY_LABEL(hide_backgrounds);
    MEMORY_DISPLAY_LAYERS(TILE_MAPPING_1D | DISPLAY_OBJ, "r7");
    {
        MEMORY_FIELD(phase, "r1", "r0", ".Lmemory_hide_layers_phase_offset");
        MEMORY_WRITE(phase, START_LIGHT_FADE, "r0");
    }
    MEMORY_BRANCH(check_for_background_upload);
    MEMORY_FIELD_WORD(".Lmemory_hide_layers_phase_offset", scene_phase);

    MEMORY_LABEL(show_montage_background_pair);
    {
        MEMORY_FIELD(cue, "r0", "r1", ".Lmemory_montage_cue_offset");
        MEMORY_SIGNED(cue_id, "r0", cue, "r2");
        MEMORY_MAP_PAIR(cue_id, 2);
        MEMORY_FIELD(phase, "r1", "r3", ".Lmemory_montage_phase_offset");
        asm volatile (".Lmemory_store_darken_fade_in_phase:" : "+l" (phase) : : "memory");
        MEMORY_WRITE(phase, START_DARK_FADE, "r0");
    }
    MEMORY_BRANCH(check_for_background_upload);
    MEMORY_FIELD_WORD(".Lmemory_montage_cue_offset", scene_cue);
    MEMORY_FIELD_WORD(".Lmemory_montage_phase_offset", scene_phase);

    MEMORY_LABEL(show_girl_background_pair);
    {
        MEMORY_FIELD(phase, "r1", "r4", ".Lmemory_girl_map_phase_offset");
        MEMORY_WRITE(phase, START_LIGHT_FADE, "r0");
        MEMORY_FIELD(cue, "r0", "r7", ".Lmemory_girl_map_cue_offset");
        MEMORY_SIGNED(cue_id, "r0", cue, "r1");
        asm volatile ("b .Lmemory_select_late_background_pair" : : "l" (cue_id) : "memory");
    }
    MEMORY_FIELD_WORD(".Lmemory_girl_map_phase_offset", scene_phase);
    MEMORY_FIELD_WORD(".Lmemory_girl_map_cue_offset", scene_cue);

    MEMORY_LABEL(show_farewell_background_pair);
    MEMORY_DELAY(FAREWELL_PAUSE, "r2");
    {
        MEMORY_FIELD(phase, "r1", "r3", ".Lmemory_farewell_phase_offset");
        MEMORY_WRITE(phase, PAUSE_BEFORE_FADE, "r0");
        MEMORY_FIELD(cue, "r0", "r4", ".Lmemory_farewell_cue_offset");
        MEMORY_SIGNED(cue_id, "r0", cue, "r7");
        asm volatile ("b .Lmemory_select_late_background_pair" : : "l" (cue_id) : "memory");
    }
    MEMORY_FIELD_WORD(".Lmemory_farewell_phase_offset", scene_phase);
    MEMORY_FIELD_WORD(".Lmemory_farewell_cue_offset", scene_cue);

    MEMORY_LABEL(show_departure_background_pair);
    {
        MEMORY_FIELD(phase, "r1", "r0", ".Lmemory_departure_phase_offset");
        MEMORY_WRITE(phase, START_DARK_FADE, "r0");
        MEMORY_FIELD(cue, "r0", "r1", ".Lmemory_departure_cue_offset");
        MEMORY_SIGNED(cue_id, "r0", cue, "r2");
        asm volatile (".Lmemory_select_late_background_pair:" : "+l" (cue_id) : : "memory");
        MEMORY_MAP_PAIR(cue_id, 3);
    }
    MEMORY_LABEL(check_for_background_upload);
    {
        register i32 map_index asm("r10");
        asm ("" : "=h" (map_index));
        register i32 keep_background asm("r0") = -KEEP_BACKGROUND;
        MEMORY_PIN(keep_background);
        asm volatile ("neg %0, %0\n\tcmp %1, %0\n\tbne .Lmemory_queue_bg3_map\n\tb .Lmemory_update_dialogue"
            : "+l" (keep_background) : "h" (map_index) : "cc", "memory");
    }

    // Each illustration uses consecutive BG3 and BG1 tilemaps.
    MEMORY_LABEL(queue_bg3_map);
    {
        register DmaTransferQueue * queue asm("r6") = GetChildhoodDmaQueue(self);
        MEMORY_PIN(queue);
        register u32 map_index asm("r10");
        asm ("" : "=h" (map_index));
        register u32 index asm("r3") = map_index;
        MEMORY_PIN(index);
        register u32 times_four asm("r4") = index * 4;
        MEMORY_PIN(times_four);
        register uintptr_t map_base asm("r1") = (times_four + index) * (CHILDHOOD_MAP_BYTES / 5);
        MEMORY_PIN(map_base);
        map_base += reinterpret_cast<uintptr_t>(self);
        MEMORY_PIN(map_base);
        map_base += offsetof(ChildhoodMemoryScreenLayout, background_maps);
        MEMORY_PIN(map_base);
        register DmaCommand * copy asm("r0");
        asm ("add %0, sp, %1" : "=l" (copy) : "I" (offsetof(ChildhoodFrameWork, copy)));
        register void * destination asm("r2");
        MEMORY_LITERAL(destination, ".Lmemory_bg3_tilemap_address");
        register u32 bytes asm("r3") = CHILDHOOD_MAP_BYTES;
        MEMORY_PIN(bytes);
        DmaCommand_InitializeCopy(copy, reinterpret_cast<void const *>(map_base), destination, bytes);
        register DmaCommand const * command asm("r7");
        MEMORY_LOAD(command, copy_command);
        MEMORY_SAVE(command, bg3_command);
        register DmaCommand * tail asm("r1") = queue->end;
        MEMORY_PIN(tail);
        register DmaCommand * capacity asm("r0") = queue->capacity_end;
        MEMORY_PIN(capacity);
        MEMORY_SAVE(times_four, map_index_times_four);
        asm volatile ("cmp %0, %1\n\tbeq .Lmemory_grow_bg3_dma_queue" : : "l" (tail), "l" (capacity) : "cc", "memory");
        CHILDHOOD_APPEND_COMMAND(command, "r4", advance_bg3_dma_queue);
    }
    MEMORY_BRANCH(queue_bg1_map);
    MEMORY_FIELD_WORD(".Lmemory_departure_phase_offset", scene_phase);
    MEMORY_FIELD_WORD(".Lmemory_departure_cue_offset", scene_cue);
    MEMORY_WORD(".Lmemory_bg3_tilemap_address", ".Lmemory_bg3_tilemap_vram");

    MEMORY_LABEL(grow_bg3_dma_queue);
    CHILDHOOD_GROW_MAP_QUEUE(bg3_growth, "r7", "r0", "r1", "mov r0, %0\n\t", CHILDHOOD_BG3_COPY_SOURCE);

    MEMORY_LABEL(queue_bg1_map);
    {
        register DmaTransferQueue * queue asm("r6") = GetChildhoodDmaQueue(self);
        MEMORY_PIN(queue);
        register u32 map_index asm("r10");
        asm ("" : "=h" (map_index));
        register uintptr_t bg1_map asm("r1");
        MEMORY_LOAD(bg1_map, map_index_times_four);
        asm ("add %0, %1" : "+l" (bg1_map) : "h" (map_index));
        bg1_map *= CHILDHOOD_MAP_BYTES / 5;
        MEMORY_PIN(bg1_map);
        bg1_map += reinterpret_cast<uintptr_t>(self);
        MEMORY_PIN(bg1_map);
        register u32 bg1_offset asm("r3") = offsetof(ChildhoodMemoryScreenLayout, background_maps) + CHILDHOOD_MAP_BYTES;
        MEMORY_PIN(bg1_offset);
        bg1_map += bg1_offset;
        MEMORY_PIN(bg1_map);
        register DmaCommand * copy asm("r0");
        asm ("add %0, sp, %1" : "=l" (copy) : "I" (offsetof(ChildhoodFrameWork, copy)));
        register void * destination asm("r2");
        MEMORY_LITERAL(destination, ".Lmemory_bg1_tilemap_address");
        bg1_offset -= offsetof(ChildhoodMemoryScreenLayout, background_maps);
        MEMORY_PIN(bg1_offset);
        DmaCommand_InitializeCopy(copy, reinterpret_cast<void const *>(bg1_map), destination, bg1_offset);
        register DmaCommand const * command asm("r4");
        MEMORY_LOAD(command, copy_command);
        register DmaCommand const * command_for_growth asm("r10") = command;
        asm volatile ("" : "+h" (command_for_growth));
        register DmaCommand * tail asm("r1") = queue->end;
        MEMORY_PIN(tail);
        register DmaCommand * capacity asm("r0") = queue->capacity_end;
        MEMORY_PIN(capacity);
        asm volatile ("cmp %0, %1\n\tbeq .Lmemory_grow_bg1_dma_queue" : : "l" (tail), "l" (capacity) : "cc", "memory");
        CHILDHOOD_APPEND_COMMAND(command, "r7", advance_bg1_dma_queue);
    }
    MEMORY_BRANCH(enable_background_layers);
    MEMORY_WORD(".Lmemory_bg1_tilemap_address", ".Lmemory_bg1_tilemap_vram");

    MEMORY_LABEL(grow_bg1_dma_queue);
    CHILDHOOD_GROW_MAP_QUEUE(bg1_growth, "r4", "r7", "r0", "", CHILDHOOD_BG1_COPY_SOURCE);

    MEMORY_LABEL(enable_background_layers);
    MEMORY_DISPLAY_LAYERS(TILE_MAPPING_1D | DISPLAY_OBJ | DISPLAY_BG2 | DISPLAY_BG3, "r3");
    MEMORY_BRANCH(update_dialogue);

    MEMORY_LABEL(wait_for_exit);
    {
        register ChildhoodFrameTask * task asm("r0");
        MEMORY_LOAD(task, fade);
        register uintptr_t previous asm("r1") = reinterpret_cast<uintptr_t>(task->previous_link);
        MEMORY_PIN(previous);
        register i32 busy asm("r0");
        asm volatile ("neg %0, %1\n\torr %0, %1\n\tcmp %0, #0\n\tblt .Lmemory_update_dialogue"
            : "=&l" (busy) : "l" (previous) : "cc", "memory");
        register u32 done asm("r4") = true;
        MEMORY_PIN(done);
        MEMORY_SAVE(done, scene_done);
    }
    MEMORY_LABEL(update_dialogue);
    UpdateChildhoodMemoryDialogue(self);
    {
        register u32 done asm("r7");
        MEMORY_LOAD(done, scene_done);
        asm volatile ("cmp %0, #0\n\tbne .Lmemory_clear_scene_result\n\tb .Lmemory_next_frame"
            : : "l" (done) : "cc", "memory");
    }

    // The flashback returns no replacement scene.
    MEMORY_LABEL(clear_scene_result);
    {
        register AUnk_0800080C * empty asm("r0") = nullptr;
        MEMORY_PIN(empty);
        MEMORY_SAVE(empty, discarded_scene);
        // The map command's stack storage is reused for the empty result slot.
        register AUnk_0800080C ** result_slot asm("r1");
        MEMORY_LOAD(result_slot, copy_command);
        MEMORY_SAVE(result_slot, empty_result.slot);
        register ChildhoodResultOwner * owner asm("r2");
        MEMORY_LOAD(owner, empty_result_ptr);
        owner->value = empty;
        *result_slot = empty;
        register AUnk_0800080C ** result_destination asm("r3");
        MEMORY_LOAD(result_destination, result);
        *result_destination = empty;
    }
    register AUnk_0800080C * discarded asm("r1");
    MEMORY_LOAD(discarded, discarded_scene);
    {
        asm volatile ("cmp %0, #0\n\tbeq .Lmemory_destroy_fade_task" : : "l" (discarded) : "cc", "memory");
        register ChildhoodResultVtable const * vtable asm("r0") =
            reinterpret_cast<ChildhoodResultScene *>(discarded)->vtable;
        MEMORY_PIN(vtable);
        register void (*destroy)(AUnk_0800080C *, u32) asm("r2") = vtable->destroy;
        MEMORY_PIN(destroy);
        register AUnk_0800080C * argument asm("r0");
        asm ("add %0, %1, #0" : "=l" (argument) : "l" (discarded) : "cc");
        register u32 delete_flags asm("r1") = 3;
        MEMORY_PIN(delete_flags);
        destroy(argument, delete_flags);
    }
    MEMORY_LABEL(destroy_fade_task);
    {
        register ChildhoodFrameTask * task asm("r1");
        MEMORY_LOAD(task, fade);
        asm volatile ("cmp %0, #0\n\tbeq .Lmemory_return_scene_result" : : "l" (task) : "cc", "memory");
        register ChildhoodFadeTaskVtable const * vtable asm("r0") = task->vtable;
        MEMORY_PIN(vtable);
        register void (*destroy)(ChildhoodFrameTask *, u32) asm("r2") = vtable->destroy;
        MEMORY_PIN(destroy);
        register ChildhoodFrameTask * argument asm("r0");
        asm ("add %0, %1, #0" : "=l" (argument) : "l" (task) : "cc");
        register u32 delete_flags asm("r1") = 3;
        MEMORY_PIN(delete_flags);
        destroy(argument, delete_flags);
    }

    MEMORY_LABEL(return_scene_result);
    MEMORY_LOAD(result, result);
    return result;
}

#undef START_CHILDHOOD_FADE
#undef QUEUE_CHILDHOOD_FADE
#undef MEMORY_MAP_PAIR
#undef MEMORY_DISPLAY_LAYERS
#undef MEMORY_START_LINE
#undef CHILDHOOD_APPEND_COMMAND
#undef CHILDHOOD_GROW_MAP_QUEUE
#undef CHILDHOOD_BG1_COPY_SOURCE
#undef CHILDHOOD_BG3_COPY_SOURCE
#undef CHILDHOOD_COPY_COMMAND
#undef MEMORY_FIELD_WORD
#undef MEMORY_WORD
#undef MEMORY_FADE_READY
#undef MEMORY_TICK
#undef MEMORY_DELAY
#undef MEMORY_SIGNED
#undef MEMORY_WRITE
#undef MEMORY_FIELD
#undef MEMORY_BRANCH
#undef MEMORY_LABEL
#undef MEMORY_LITERAL
#undef MEMORY_SAVE
#undef MEMORY_LOAD
#undef MEMORY_PIN
