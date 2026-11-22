#include "scene.hh"

#include "farmer.hh"
#include "message_script.hh"
#include "utility/vec2.hh"

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

EC void InitScreenHardware(ChildhoodMemoryScreenLayout *, Vec2 const *) asm("HardwareContext_InitializeWithKeyRepeat");
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
EC void RunChildhoodMemoryScreen(AUnk_0800080C ** result, ChildhoodMemoryScreen * screen);

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
