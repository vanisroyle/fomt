#include "prelude.h"

#include "bachelorette.hh"
#include "farm_house.hh"
#include "fixed_sprite_overlay.hh"
#include "gbasvc.h"
#include "npc.hh"
#include "schedule_info.hh"
#include "sprite_animation.hh"
#include "unknown_types.hh"

#include <algorithm>
#include <stdlib.h>
#include <string.h>

#define ROM_TEXT(addr) EC SECTION(".text." #addr)

struct MapOverlayScene;

struct MapOverlaySceneVtable
{
    STRUCT_PAD(0x00, 0x18);
    int (*getCameraX)(MapOverlayScene *);
    int (*getCameraY)(MapOverlayScene *);
    STRUCT_PAD(0x20, 0x4C);
    void (*releasePalette)(MapOverlayScene *, unsigned);
};

struct MapOverlayScene
{
    MapOverlaySceneVtable const * vtable;
};

struct MapOverlayParticle
{
    SpriteAnimator animation;
    u32 xQ16;
    u32 yQ16;
};

struct GameStateMapOverlay
{
    struct Frame
    {
        STRUCT_PAD(0x00, 0x20);
        OverlayTileAllocation tiles;
    };

    MapOverlayScene * scene;
    struct FrameList
    {
        unsigned count;
        Frame entries[5];
    } frames;
    OverlayPaletteTable palettes;
    u8 kind;
    u8 unk_E5;
    u8 drawHalf;
    STRUCT_PAD(0xE7, 0xE8);
    struct ParticleList
    {
        unsigned count;
        MapOverlayParticle entries[32];
    } particles;
};

struct MapOverlayParticleMotion
{
    i32 dxQ16;
    i32 dyQ16;
};

EC MapOverlayParticleMotion const gMapOverlayParticleMotion[][2];
EC void ResetMapOverlayParticle(MapOverlayParticle &, int cameraX, int cameraY);

ROM_TEXT(0803B2A8) void UpdateGameStateMapOverlay(GameStateMapOverlay & overlay)
{
    unsigned kind = overlay.kind;
    MapOverlayParticleMotion const * motion = gMapOverlayParticleMotion[kind];
    unsigned motionIndex = 0;
    unsigned particlesInGroup = 0;
    unsigned groupSize;
    switch (kind)
    {
    case 0:
    default: groupSize = 2; break;
    case 1: groupSize = 3; break;
    }

    int cameraX = overlay.scene->vtable->getCameraX(overlay.scene);
    int cameraY = overlay.scene->vtable->getCameraY(overlay.scene);
    unsigned count = overlay.particles.count;
    unsigned index = 0;
    if (index >= count)
        return;

    MapOverlayParticle * particle = overlay.particles.entries;
    // Preserve the scaled index and address addition in the original Thumb code.
    MapOverlayParticleMotion const * step = reinterpret_cast<MapOverlayParticleMotion const *>(
        motionIndex * sizeof(MapOverlayParticleMotion) + reinterpret_cast<uintptr_t>(motion));
    do
    {
        if (static_cast<i32>(AdvanceSpriteAnimator(&particle->animation) << (31 - SpriteAnimator::LOOP_FLAG_BIT)) < 0)
            ResetMapOverlayParticle(*particle, cameraX, cameraY);

        if (++particlesInGroup >= groupSize)
        {
            ++step;
            if (++motionIndex >= ARRAY_COUNT(gMapOverlayParticleMotion[0]))
            {
                step = motion;
                motionIndex = 0;
            }
            particlesInGroup = 0;
        }
        particle->xQ16 += step->dxQ16;
        particle->yQ16 += step->dyQ16;
        ++particle;
        ++index;
    } while (index < count);
}

EC void func_08007C28(OverlayTileAllocation *, u32);
EC void TileHeapOwner_Destroy(OverlayTileAllocation *, unsigned);
EC void __builtin_delete(void *);

ROM_TEXT(0803AC78) void DestroyFixedSpriteOverlay(FixedSpriteOverlay * inputOverlay, unsigned inputFlags)
{
    // Match the original Thumb register allocation for the destructor.
    register FixedSpriteOverlay * overlay asm("r6") = inputOverlay;
    asm("" : "+r"(overlay));
    register unsigned destructorFlags asm("r8") = inputFlags;
    asm("" : "+r"(destructorFlags) : "r"(overlay));
    OverlaySpriteResources * resources = overlay->resources;
    OverlaySpriteResourceVtable const * vtable = resources->vtable;
    register FixedSpriteOverlay::Frame * end asm("r4") = overlay->frames + 3;
    asm("" : "+r"(end) : "r"(resources), "r"(vtable));
    vtable->releasePalette(resources, overlay->palettes.ids[0]);

    register FixedSpriteOverlay::Frame * firstFrame asm("r0") = overlay->frames;
    asm("" : "+r"(firstFrame));
    if (firstFrame)
    {
        FixedSpriteOverlay::Frame * frame = end;
        if (firstFrame != frame)
        {
            FixedSpriteOverlay::Frame * first = firstFrame;
            do
            {
                --frame;
                OverlayTileAllocation * tiles = &frame->tiles;
                func_08007C28(tiles, tiles->handle);
                TileHeapOwner_Destroy(tiles, 2);
            }
            while (first != frame);
        }
    }
    if (destructorFlags & 1)
        __builtin_delete(overlay);
}

ROM_TEXT(0803B0A8) void DestroyGameStateMapOverlay(GameStateMapOverlay * overlay, unsigned destructorFlags)
{
    overlay->scene->vtable->releasePalette(overlay->scene, overlay->palettes.ids[0]);

    // The original compiler emitted this traversal even though particles own no tiles.
    GameStateMapOverlay::ParticleList & particles = overlay->particles;
    MapOverlayParticle * particlesEnd = particles.entries + particles.count;
    for (MapOverlayParticle * particle = particles.entries; particle != particlesEnd; ++particle)
    {
    }

    GameStateMapOverlay::FrameList & frames = overlay->frames;
    GameStateMapOverlay::Frame * framesEnd = frames.entries + frames.count;
    for (GameStateMapOverlay::Frame * frame = frames.entries; frame != framesEnd; ++frame)
    {
        OverlayTileAllocation * tiles = &frame->tiles;
        func_08007C28(tiles, tiles->handle);
        TileHeapOwner_Destroy(tiles, 2);
    }
    if (destructorFlags & 1)
        __builtin_delete(overlay);
}

ROM_TEXT(0803B454) MapOverlayParticle & InitMapOverlayParticle(MapOverlayParticle & particle,
    SpriteAnimator const & animation, int cameraX, int cameraY)
{
    particle.animation = animation;
    ResetMapOverlayParticle(particle, cameraX, cameraY);
    return particle;
}

ROM_TEXT(0803B454) void ResetMapOverlayParticle(MapOverlayParticle & particle, int cameraX, int cameraY)
{
    particle.xQ16 = (static_cast<u32>(cameraX) + rand() % 240) << 16;
    particle.yQ16 = (static_cast<u32>(cameraY) + rand() % 160) << 16;
    int sample = rand();
    int variation = 0x90;
    variation &= sample;
    particle.animation.speed_q8 = variation + 0x60;
}

struct Unk_0803B4B0
{
    void const * vtable;
    void * unk_04;
    char labels[4][24];
};

EC char const vtable_unk_080E76E8[];

ROM_TEXT(0803B454) Unk_0803B4B0 & func_0803B4B0(Unk_0803B4B0 & self, void * unk)
{
    self.vtable = vtable_unk_080E76E8;
    self.unk_04 = unk;
    char (* label)[24] = self.labels;
    for (int remaining = ARRAY_COUNT(self.labels) - 1; remaining != -1; ++label, --remaining)
        (*label)[0] = '\0';
    return self;
}

ROM_TEXT(0803B6B8) void func_0803B6B8(Unk_0803B4B0 & self, unsigned index, char const * label)
{
    char buf[24];
    size_t length = strlen(label);
    if (length > 22)
        length = 22;
    memcpy(buf, label, length);
    buf[length] = '\0';
    unsigned offset = index * sizeof(self.labels[0]);
    char * record = reinterpret_cast<char *>(offset + reinterpret_cast<uintptr_t>(&self));
    strcpy(record + offsetof(Unk_0803B4B0, labels), buf);
}

struct Unk_0803B6FC
{
    void const * vtable;
    void * unk_04;
    void * unk_08;
};

EC char const vtable_unk_080E7748[];

ROM_TEXT(0803B6B8) Unk_0803B6FC & func_0803B6FC(Unk_0803B6FC & self, void *& owned, void * arg)
{
    self.vtable = vtable_unk_080E7748;
    void * taken = owned;
    owned = nullptr;
    self.unk_04 = taken;
    self.unk_08 = arg;
    return self;
}

struct DialogWindowTilemapTask
{
    STRUCT_PAD(0x00, 0x0C);
    int left;
    int top;
    unsigned width;
    unsigned height;
    unsigned screenBlock : 5;
    unsigned firstTile : 10;
    unsigned paletteBank : 4;
};

EC void DrawMessageFrame(u16 * tilemap, unsigned x, unsigned y, unsigned width, unsigned height,
    unsigned firstTile, unsigned paletteBank);
EC u16 const * const gUnk_080F9F78;

ROM_TEXT(0803B840) bool RunDialogWindowTilemapTask(DialogWindowTilemapTask & task)
{
    enum
    {
        VideoRam = 0x06000000,
        BackgroundPaletteRam = 0x05000000,
        ScreenBlockBytes = 0x800,
        PaletteBankBytes = 32,
        FillWords = 32 * 10 / 2,
        FillMode = 1 << 24,
        TileIndexMask = 0x3FF,
        CenterTileOffset = 3,
    };

    // Keep the original Thumb register allocation for the VRAM fill and palette copy.
    register unsigned tile asm("r2") = task.firstTile + CenterTileOffset;
    asm("" : "+r"(tile));
    register unsigned mask asm("r1") = TileIndexMask;
    asm("" : "+r"(mask) : "r"(tile));
    register unsigned tileMask asm("r0") = mask;
    asm("" : "+r"(tileMask));
    tile = (tile & tileMask) | (task.paletteBank << 12);
    register unsigned highHalf asm("r0") = tile << 16;
    asm("" : "+r"(highHalf) : "r"(tile));
    tile |= highHalf;

    register unsigned screenOffset asm("r1") = task.screenBlock * ScreenBlockBytes;
    asm("" : "+r"(screenOffset) : "r"(tile));
    register u16 * screen asm("r1") = reinterpret_cast<u16 *>(VideoRam + screenOffset);
    asm("" : "+r"(screen) : "r"(tile));
    u32 repeatedTile = tile;
    register u32 * source asm("r0") = &repeatedTile;
    asm("" : "+r"(source) : "m"(repeatedTile));
    CpuFastSet(source, screen, FillMode | FillWords);

    DrawMessageFrame(reinterpret_cast<u16 *>(VideoRam + task.screenBlock * ScreenBlockBytes),
        task.left, 0, task.width, task.height, task.firstTile, task.paletteBank);

    register unsigned paletteBank asm("r1") = task.paletteBank;
    asm("" : "+r"(paletteBank));
    register u16 const * palette asm("r0") = gUnk_080F9F78;
    asm("" : "+r"(palette) : "r"(paletteBank));
    CpuFastSet(palette, reinterpret_cast<u16 *>(BackgroundPaletteRam + paletteBank * PaletteBankBytes),
        PaletteBankBytes / sizeof(u32));
    return false;
}

struct PACKED ALIGN(4) DialogWindowLayers
{
    i16 x;
    i16 y;
    STRUCT_PAD(0x04, 0x0A);
    u16 : 7;
    u16 layerMode : 8;
    u16 state : 8;
    u8 : 1;
    u8 animationFrames;
    STRUCT_PAD(0x0E, 0x10);
    u8 layerList[0x1C];
    u8 backgroundLayer[0x18];
    DialogWindowTilemapTask tilemapTask;
};

struct DialogWindow
{
    DialogWindowLayers * inner;
    void const * vtable;
};

enum DialogWindowState
{
    DIALOG_WINDOW_CLOSED,
    DIALOG_WINDOW_OPENING,
    DIALOG_WINDOW_CLOSING,
    DIALOG_WINDOW_OPEN,
};

struct BackgroundVideoState;
struct DialogLayerRenderer;
struct TileTransferQueue;
struct OamBuffer;

EC int UpdateDialogWindowLayers(DialogWindowLayers &, BackgroundVideoState *, DialogLayerRenderer *, TileTransferQueue *);
EC void TaskList_Append(void *, void *);

ROM_TEXT(0803BCF8) void OpenDialogWindowLayers(DialogWindowLayers & self)
{
    int state = self.state;
    if (state == DIALOG_WINDOW_CLOSED)
    {
        u32 layerMode = self.layerMode;
        if (layerMode != 1)
            TaskList_Append(self.layerList, self.backgroundLayer);
        TaskList_Append(self.layerList, &self.tilemapTask);
        self.animationFrames = state;
    }
    int nextState = self.state;
    if (nextState < 0)
        return;
    if (nextState > DIALOG_WINDOW_CLOSING)
        return;
    self.state = DIALOG_WINDOW_OPENING;
}

ROM_TEXT(0803BD60) void func_0803BD60(DialogWindowLayers & self)
{
    OpenDialogWindowLayers(self);
    self.state = DIALOG_WINDOW_OPEN;
    self.animationFrames = 9;
}

ROM_TEXT(0803BD60) void func_0803BD88(DialogWindowLayers & self)
{
    if (self.state)
        self.state = DIALOG_WINDOW_CLOSING;
}

ROM_TEXT(0803BD60) void func_0803BDB8(DialogWindowLayers & self)
{
    if (self.state)
        self.state = DIALOG_WINDOW_CLOSED;
}

ROM_TEXT(0803BD60) void func_0803BDE4(DialogWindowLayers & self, int x, int y)
{
    self.x = x;
    self.y = y;
}

ROM_TEXT(0803BD60) void func_0803BDEC(DialogWindowLayers & self, int x, int y)
{
    self.x += x;
    self.y += y;
}

EC char const vtable_unk_080E7778[];
EC DialogWindowLayers * InitializeDialogWindowLayers(void *, int, int, int, int, int, int, int, int);
EC void TaskNode_Destroy(void *, int);
EC void TaskList_Destroy(void *, int);

ROM_TEXT(0803BD60) DialogWindow & func_0803BDFC(DialogWindow & self, int x, int y, int width, int height, int arg5, int arg6, int arg7, int arg8)
{
    self.vtable = vtable_unk_080E7778;
    self.inner = InitializeDialogWindowLayers(::operator new(0x64), x, y, width, height, arg5, arg6, arg7, arg8);
    return self;
}

ROM_TEXT(0803BD60) void func_0803BE50(DialogWindow & self, int flags)
{
    self.vtable = vtable_unk_080E7778;
    if (DialogWindowLayers * inner = self.inner)
    {
        u8 * data = reinterpret_cast<u8 *>(inner);
        TaskNode_Destroy(data + 0x44, 2);
        TaskNode_Destroy(data + 0x2C, 2);
        TaskList_Destroy(data + 0x10, 2);
        ::operator delete(inner);
    }
    if (flags & 1)
        ::operator delete(&self);
}

ROM_TEXT(0803BD60) unsigned func_0803BE9C(DialogWindow const & self)
{
    return self.inner->state;
}

ROM_TEXT(0803BD60) int UpdateDialogWindow(DialogWindow & self, BackgroundVideoState * video,
    DialogLayerRenderer * layerRenderer, TileTransferQueue * tileTransfers)
{
    return UpdateDialogWindowLayers(*self.inner, video, layerRenderer, tileTransfers);
}

ROM_TEXT(0803BD60) void func_0803BEBC(DialogWindow & self)
{
    OpenDialogWindowLayers(*self.inner);
}

ROM_TEXT(0803BD60) void func_0803BEC8(DialogWindow & self)
{
    func_0803BD60(*self.inner);
}

ROM_TEXT(0803BD60) void func_0803BED4(DialogWindow & self)
{
    func_0803BD88(*self.inner);
}

ROM_TEXT(0803BD60) void func_0803BEE0(DialogWindow & self)
{
    func_0803BDB8(*self.inner);
}

ROM_TEXT(0803BD60) int func_0803BEEC(DialogWindow const & self)
{
    return self.inner->x;
}

ROM_TEXT(0803BD60) int func_0803BEF4(DialogWindow const & self)
{
    return self.inner->y;
}

ROM_TEXT(0803BD60) void func_0803BEFC(DialogWindow & self, int x, int y)
{
    func_0803BDE4(*self.inner, x, y);
}

ROM_TEXT(0803BD60) void func_0803BF08(DialogWindow & self, int x, int y)
{
    func_0803BDEC(*self.inner, x, y);
}

struct PauseMenuGraphics;

struct PauseMenuWindow : DialogWindow
{
    PauseMenuGraphics * graphics;
    u8 selectedTab;
    bool needsInitialRefresh;
    u8 cursorBlinkFrames;
    bool selectionEnabled;
};

ROM_TEXT(0803C518) void OpenPauseMenuWindow(PauseMenuWindow & self)
{
    func_0803BEBC(self);
    self.needsInitialRefresh = true;
    self.selectionEnabled = true;
}

ROM_TEXT(0803C518) void OpenPauseMenuWindowImmediately(PauseMenuWindow & self)
{
    func_0803BEC8(self);
    self.needsInitialRefresh = true;
    self.selectionEnabled = true;
}

ROM_TEXT(0803C518) u8 GetPauseMenuSelection(PauseMenuWindow const & self)
{
    return self.selectedTab;
}

ROM_TEXT(0803C518) void DisablePauseMenuSelection(PauseMenuWindow & self)
{
    self.selectionEnabled = false;
}

struct ClockDisplayData
{
    STRUCT_PAD(0x000, 0x084);
    u8 seasonFirstTiles[0x20];
    u8 seasonSecondTiles[0x20];
    u8 dayTensTiles[0x20];
    u8 dayOnesTiles[0x20];
    u8 meridiemTiles[0x20];
    STRUCT_PAD(0x124, 0x144);
    u8 hourTensTiles[0x20];
    u8 hourOnesTiles[0x20];
    STRUCT_PAD(0x184, 0x18C);
    u8 dateTilesDirty;
    STRUCT_PAD(0x18D, 0x1A8);
    u8 weekdayFirstTiles[0x20];
    u8 weekdaySecondTiles[0x20];
    u8 weekdayThirdTiles[0x20];
    STRUCT_PAD(0x208, 0x228);
    u8 colonTiles[0x20];
    u8 minuteTensTiles[0x20];
    u8 minuteOnesTiles[0x20];
    STRUCT_PAD(0x288, 0x2B0);
    u8 tilesDirty;
    STRUCT_PAD(0x2B1, 0x2CC);
    u8 weatherIcon;
    STRUCT_PAD(0x2CD, 0x2F0);
    u8 weatherIconDirty;
    u8 toolIconVisible;
    u16 toolIconId;
    STRUCT_PAD(0x2F4, 0x334);
    u8 toolIconDirty;
};

struct ClockDisplay
{
    ClockDisplayData * inner;
};

EC u8 const gClockFont[][0x20];

ROM_TEXT(0803D080) void SetClockSeasonTiles(ClockDisplay & self, Season season)
{
    ClockDisplayData & inner = *self.inner;
    u32 seasonTileIds[2];
    switch (season)
    {
    case SEASON_SPRING:
    default: seasonTileIds[0] = 11; seasonTileIds[1] = 12; break;
    case SEASON_SUMMER: seasonTileIds[0] = 13; seasonTileIds[1] = 14; break;
    case SEASON_AUTUMN: seasonTileIds[0] = 15; seasonTileIds[1] = 16; break;
    case SEASON_WINTER: seasonTileIds[0] = 17; seasonTileIds[1] = 18; break;
    }
    memcpy(inner.seasonFirstTiles,
           gClockFont[seasonTileIds[0]],
           sizeof(inner.seasonFirstTiles));
    inner.dateTilesDirty = 1;
    u8 const * secondTile = gClockFont[seasonTileIds[1]];
    memcpy(inner.seasonSecondTiles, secondTile, sizeof(inner.seasonSecondTiles));
    inner.dateTilesDirty = 1;
}

ROM_TEXT(0803D0F0) void SetClockDayAndWeekdayTiles(ClockDisplay & self, unsigned day, unsigned weekday)
{
    ClockDisplayData & inner = *self.inner;
    int tens = day / 10;
    unsigned ones = day % 10;
    u32 weekdayTileIds[8]; // Also used as the blank tens tile when the day is one digit.
    u8 * dateDirty;
    if (tens > 0)
    {
        u8 const * tile = gClockFont[tens];
        memcpy(inner.dayTensTiles, tile, sizeof(inner.dayTensTiles));
        dateDirty = &inner.dateTilesDirty;
    }
    else
    {
        memset(weekdayTileIds, 0, sizeof(weekdayTileIds));
        u8 * blankDest = inner.dayTensTiles;
        asm("" : "+r"(blankDest));
        memcpy(blankDest, weekdayTileIds, sizeof(inner.dayTensTiles));
        register unsigned dirtyOffset asm("r2") = offsetof(ClockDisplayData, dateTilesDirty);
        asm("" : "+r"(dirtyOffset));
        dateDirty = reinterpret_cast<u8 *>(&inner) + dirtyOffset;
    }
    *dateDirty = 1;
    u8 const * tile = gClockFont[ones];
    memcpy(inner.dayOnesTiles, tile, sizeof(inner.dayOnesTiles));
    inner.dateTilesDirty = 1;
    switch (weekday)
    {
    case 0:
    default: weekdayTileIds[0] = 0x60; weekdayTileIds[1] = 0x61; weekdayTileIds[2] = 0x62; break;
    case 1: weekdayTileIds[0] = 0x63; weekdayTileIds[1] = 0x64; weekdayTileIds[2] = 0x65; break;
    case 2: weekdayTileIds[0] = 0x66; weekdayTileIds[1] = 0x67; weekdayTileIds[2] = 0x68; break;
    case 3: weekdayTileIds[0] = 0x69; weekdayTileIds[1] = 0x6A; weekdayTileIds[2] = 0x6B; break;
    case 4: weekdayTileIds[0] = 0x6C; weekdayTileIds[1] = 0x6D; weekdayTileIds[2] = 0x6E; break;
    case 5: weekdayTileIds[0] = 0x6F; weekdayTileIds[1] = 0x70; weekdayTileIds[2] = 0x71; break;
    case 6: weekdayTileIds[0] = 0x72; weekdayTileIds[1] = 0x73; weekdayTileIds[2] = 0x74; break;
    }
    // Preserve the original tile copy order and register allocation.
    u8 * firstDest = inner.weekdayFirstTiles;
    register unsigned firstTileOffset asm("r1") = weekdayTileIds[0] * sizeof(inner.weekdayFirstTiles);
    asm("" : "+r"(firstTileOffset) : "r"(firstDest));
    u8 const * font = &gClockFont[0][0];
    memcpy(firstDest, reinterpret_cast<u8 const *>(firstTileOffset + reinterpret_cast<uintptr_t>(font)), sizeof(inner.weekdayFirstTiles));
    unsigned refreshOffset = offsetof(ClockDisplayData, tilesDirty);
    asm("" : "+r"(refreshOffset));
    u8 * dirty = reinterpret_cast<u8 *>(&inner) + refreshOffset;
    *dirty = 1;
    unsigned secondTileOffset = weekdayTileIds[1] * sizeof(inner.weekdaySecondTiles);
    register u8 const * secondTile asm("r1") = reinterpret_cast<u8 const *>(secondTileOffset + reinterpret_cast<uintptr_t>(font));
    asm("" : "+r"(secondTile));
    register unsigned secondDestOffset asm("r2") = refreshOffset
        - (offsetof(ClockDisplayData, tilesDirty) - offsetof(ClockDisplayData, weekdaySecondTiles));
    memcpy(reinterpret_cast<u8 *>(&inner) + secondDestOffset, secondTile, sizeof(inner.weekdaySecondTiles));
    *dirty = 1;
    unsigned thirdTileOffset = weekdayTileIds[2] * sizeof(inner.weekdayThirdTiles);
    register u8 const * thirdTile asm("r1") = reinterpret_cast<u8 const *>(thirdTileOffset + reinterpret_cast<uintptr_t>(font));
    register unsigned thirdDestOffset asm("r2") = offsetof(ClockDisplayData, weekdayThirdTiles);
    asm("" : "+r"(thirdDestOffset) : "r"(thirdTile));
    memcpy(reinterpret_cast<u8 *>(&inner) + thirdDestOffset, thirdTile, sizeof(inner.weekdayThirdTiles));
    *dirty = 1;
}

ROM_TEXT(0803D240) void SetClockHourTiles(ClockDisplay & self, unsigned hour)
{
    ClockDisplayData & inner = *self.inner;
    const unsigned pmTileId = 30;
    const unsigned amTileId = 28;
    unsigned meridiemTileId = pmTileId;
    if (hour <= 11)
        meridiemTileId = amTileId;
    u8 const * meridiemTile = gClockFont[meridiemTileId];
    memcpy(inner.meridiemTiles, meridiemTile, sizeof(inner.meridiemTiles));
    u8 * dirty = &inner.dateTilesDirty;
    *dirty = 1;
    // The clock keeps 12 at noon and subtracts 12 in the afternoon.
    register unsigned displayedHour asm("r4") = 12;
    u8 * dirtyForDigits = dirty;
    if (hour != 12)
    {
        displayedHour = hour;
        if (displayedHour > 11)
            displayedHour -= 12;
    }
    int tens = displayedHour / 10;
    displayedHour %= 10;
    u8 blank[0x20];
    u8 const * tensTile;
    u8 * tensDest;
    if (tens > 0)
    {
        tensTile = gClockFont[tens];
        tensDest = inner.hourTensTiles;
    }
    else
    {
        memset(blank, 0, sizeof(blank));
        tensDest = inner.hourTensTiles;
        tensTile = blank;
    }
    memcpy(tensDest, tensTile, sizeof(inner.hourTensTiles));
    {
        register u8 dirtyValue asm("r0") = 1;
        asm("" : "+r"(dirtyValue));
        *dirtyForDigits = dirtyValue;
    }
    u8 const * onesTile = gClockFont[displayedHour];
    memcpy(inner.hourOnesTiles, onesTile, sizeof(inner.hourOnesTiles));
    {
        register u8 dirtyValue asm("r0") = 1;
        asm("" : "+r"(dirtyValue));
        *dirtyForDigits = dirtyValue;
    }
}

ROM_TEXT(0803D2E8) void SetClockColonVisible(ClockDisplay & self, bool visible)
{
    ClockDisplayData & inner = *self.inner;
    if (visible)
    {
        uintptr_t tileBase = reinterpret_cast<uintptr_t>(gClockFont);
        u8 const * tile = reinterpret_cast<u8 const *>(tileBase + 10 * sizeof(inner.colonTiles));
        memcpy(inner.colonTiles, tile, sizeof(inner.colonTiles));
        inner.tilesDirty = 1;
    }
    else
    {
        u8 blank[0x20];
        memset(blank, 0, sizeof(blank));
        u8 * dest = reinterpret_cast<u8 *>(reinterpret_cast<uintptr_t>(&inner) + offsetof(ClockDisplayData, colonTiles));
        memcpy(dest, blank, sizeof(inner.colonTiles));
        inner.tilesDirty = 1;
    }
}

ROM_TEXT(0803D2E8) void SetClockMinuteTiles(ClockDisplay & self, unsigned minute)
{
    ClockDisplayData & inner = *self.inner;
    unsigned tens = minute / 10;
    unsigned ones = minute % 10;
    u8 const * tensTile = gClockFont[tens];
    memcpy(inner.minuteTensTiles, tensTile, sizeof(inner.minuteTensTiles));
    inner.tilesDirty = 1;
    u8 const * onesTile = gClockFont[ones];
    memcpy(inner.minuteOnesTiles, onesTile, sizeof(inner.minuteOnesTiles));
    inner.tilesDirty = 1;
}

ROM_TEXT(0803D2E8) void SetClockWeatherIcon(ClockDisplay & self, unsigned weather)
{
    ClockDisplayData & inner = *self.inner;
    unsigned current = inner.weatherIcon;
    switch (weather)
    {
        case 0: current = 0; break;
        case 1: current = 1; break;
        case 2: current = 2; break;
        case 3: current = 3; break;
        case 4: current = 4; break;
    }
    if (current != inner.weatherIcon)
    {
        inner.weatherIcon = current;
        inner.weatherIconDirty = 1;
    }
}

ROM_TEXT(0803D2E8) void SetClockToolIcon(ClockDisplay & self, int iconId)
{
    ClockDisplayData * inner = self.inner;
    if (!inner->toolIconVisible || iconId != inner->toolIconId)
    {
        inner->toolIconVisible = 1;
        inner->toolIconId = iconId;
        inner->toolIconDirty = 1;
    }
}

ROM_TEXT(0803D2E8) void HideClockToolIcon(ClockDisplay & self)
{
    ClockDisplayData & inner = *self.inner;
    if (inner.toolIconVisible)
    {
        inner.toolIconVisible = 0;
        inner.toolIconDirty = 1;
    }
}

struct SavedCalendarDate;
EC void SetClockDateAndTimeTiles(ClockDisplay &, SavedCalendarDate const &, bool colonVisible);

struct ClockHud
{
    STRUCT_PAD(0x00, 0x08);
    ClockDisplay display;
};

ROM_TEXT(0803D464) void ClockHud_HideToolIcon(ClockHud & hud)
{
    HideClockToolIcon(hud.display);
}

ROM_TEXT(0803D464) void ClockHud_SetToolIcon(ClockHud & hud, int iconId)
{
    SetClockToolIcon(hud.display, iconId);
}

ROM_TEXT(0803D464) void ClockHud_SetWeatherIcon(ClockHud & hud, unsigned weather)
{
    SetClockWeatherIcon(hud.display, weather);
}

ROM_TEXT(0803D464) void ClockHud_SetMinute(ClockHud & hud, unsigned minute)
{
    SetClockMinuteTiles(hud.display, minute);
}

ROM_TEXT(0803D464) void ClockHud_SetColonVisible(ClockHud & hud, bool visible)
{
    SetClockColonVisible(hud.display, visible);
}

ROM_TEXT(0803D464) void ClockHud_SetHour(ClockHud & hud, unsigned hour)
{
    SetClockHourTiles(hud.display, hour);
}

ROM_TEXT(0803D464) void ClockHud_SetDayAndWeekday(ClockHud & hud, unsigned day, unsigned weekday)
{
    SetClockDayAndWeekdayTiles(hud.display, day, weekday);
}

ROM_TEXT(0803D464) void ClockHud_SetSeason(ClockHud & hud, Season season)
{
    SetClockSeasonTiles(hud.display, season);
}

ROM_TEXT(0803D464) void ClockHud_SetDateAndTime(ClockHud & hud, SavedCalendarDate const & calendar, bool colonVisible)
{
    SetClockDateAndTimeTiles(hud.display, calendar, colonVisible);
}

EC u8 const gNpcScheduleFestivalDays[14];
EC u8 const * func_080E0E80(u8 const *, u8 const *, unsigned const &, int, int);

ROM_TEXT(0803D644) bool IsNpcScheduleFestivalDay(unsigned season, unsigned day)
{
    unsigned dayOfYear = season * 30;
    dayOfYear--;
    dayOfYear += day;
    u8 const * begin = gNpcScheduleFestivalDays;
    u8 const * end = begin + ARRAY_COUNT(gNpcScheduleFestivalDays);
    u8 const * found = func_080E0E80(begin, end, dayOfYear, 0, 0);
    return found != end && dayOfYear >= *found;
}

struct NpcScheduleRoster
{
    STRUCT_PAD(0x000, 0x070);
    Npc npc_070;
    Npc npc_084;
    Npc popuri;
    STRUCT_PAD(0x0AC, 0x0B0);
    Npc npc_0B0;
    Npc npc_0C4;
    Npc npc_0D8;
    STRUCT_PAD(0x0EC, 0x0F0);
    Npc npc_0F0;
    Npc npc_104;
    Npc npc_118;
    Npc npc_12C;
    Npc npc_140;
    Npc mary;
    STRUCT_PAD(0x168, 0x16C);
    Npc npc_16C;
    Npc npc_180;
    Npc npc_194;
    Npc npc_1A8;
    Npc npc_1BC;
    Npc npc_1D0;
    Npc karen;
    STRUCT_PAD(0x1F8, 0x1FC);
    Npc npc_1FC;
    Npc elli;
    STRUCT_PAD(0x224, 0x228);
    Npc npc_228;
    Npc npc_23C;
    Npc npc_250;
    Npc ann;
    STRUCT_PAD(0x278, 0x27C);
    Npc npc_27C;
    Npc npc_290;
    STRUCT_PAD(0x2A4, 0x2A8);
    Npc npc_2A8;
    Npc npc_2BC;
    STRUCT_PAD(0x2D0, 0x310);
    Npc npc_310;
    STRUCT_PAD(0x324, 0x328);
    Npc npc_328;
};

EC ScheduleInfo const gUnk_080F280C;
extern ScheduleInfo const ScheduleInfo_Unk_080F1A80;
EC ScheduleInfo const gPopuriScheduleInfo;
EC ScheduleInfo const gUnk_080F8678;
EC ScheduleInfo const gUnk_080F81BC;
EC ScheduleInfo const gUnk_080F77FC;
EC ScheduleInfo const gUnk_080F7294;
EC ScheduleInfo const gUnk_080F6370;
EC ScheduleInfo const gUnk_080F66C4;
EC ScheduleInfo const gUnk_080F49C0;
EC ScheduleInfo const gUnk_080F5540;
EC ScheduleInfo const gMaryScheduleInfo;
EC ScheduleInfo const gUnk_080F59CC;
EC ScheduleInfo const gUnk_080F6B4C;
EC ScheduleInfo const gUnk_080F33B8;
EC ScheduleInfo const gUnk_080F61FC;
EC ScheduleInfo const gUnk_080F3408;
EC ScheduleInfo const gUnk_080F3FD8;
EC ScheduleInfo const gKarenScheduleInfo;
EC ScheduleInfo const gUnk_080F3010;
EC ScheduleInfo const gElliScheduleInfo;
EC ScheduleInfo const gUnk_080F6DE8;
EC ScheduleInfo const gCliffScheduleInfo;
EC ScheduleInfo const gUnk_080F42F0;
EC ScheduleInfo const gAnnScheduleInfo;
EC ScheduleInfo const gUnk_080F6FF8;
EC ScheduleInfo const gCarpenterScheduleInfo;
EC ScheduleInfo const gUnk_080F2DC0;
EC ScheduleInfo const gUnk_080F597C;
EC ScheduleInfo const gUnk_080F6B10;
EC ScheduleInfo const gUnk_080F4974;
EC ScheduleInfo const gChildScheduleInfo;
EC Npc * GetActiveScheduleNpc(NpcScheduleRoster &);
ROM_TEXT(0803D688) void ResetNpcDailySchedule(Npc & npc, ScheduleInfo const & info, void const * context)
{
    unsigned scheduleIndex = info.select_schedule(context);
    PathInfo const * firstPath = nullptr;
    if (info.schedules)
    {
        Schedule const * schedule = info.schedules[scheduleIndex];
        if (schedule && schedule->num_time_entries && schedule->time_entries)
            firstPath = schedule->time_entries[0].path;
    }

    ActorLocation start = firstPath
        ? ActorLocation(Location(firstPath->location_start, firstPath->x_start, firstPath->y_start),
            firstPath->facing_start)
        : ActorLocation(Location(MAP_NONE, 0, 0), 0);
    npc.SetLocation(start);
    npc.schedule_index = scheduleIndex;
    npc.schedule_path_index = 0;
    npc.path_point_index = 0;
    npc.path_point_elapsed_ticks = 0;
}

ROM_TEXT(0803D7E4) void ResetAllNpcDailySchedules(NpcScheduleRoster & self, void const * context)
{
    ResetNpcDailySchedule(self.npc_070, gUnk_080F280C, context);
    ResetNpcDailySchedule(self.npc_084, ScheduleInfo_Unk_080F1A80, context);
    ResetNpcDailySchedule(self.popuri, gPopuriScheduleInfo, context);
    ResetNpcDailySchedule(self.npc_0B0, gUnk_080F8678, context);
    ResetNpcDailySchedule(self.npc_0C4, gUnk_080F81BC, context);
    ResetNpcDailySchedule(self.npc_0D8, gUnk_080F77FC, context);
    ResetNpcDailySchedule(self.npc_0F0, gUnk_080F7294, context);
    ResetNpcDailySchedule(self.npc_104, gUnk_080F6370, context);
    ResetNpcDailySchedule(self.npc_118, gUnk_080F66C4, context);
    ResetNpcDailySchedule(self.npc_12C, gUnk_080F49C0, context);
    ResetNpcDailySchedule(self.npc_140, gUnk_080F5540, context);
    ResetNpcDailySchedule(self.mary, gMaryScheduleInfo, context);
    ResetNpcDailySchedule(self.npc_16C, gUnk_080F59CC, context);
    ResetNpcDailySchedule(self.npc_180, gUnk_080F6B4C, context);
    ResetNpcDailySchedule(self.npc_194, gUnk_080F33B8, context);
    ResetNpcDailySchedule(self.npc_1A8, gUnk_080F61FC, context);
    ResetNpcDailySchedule(self.npc_1BC, gUnk_080F3408, context);
    ResetNpcDailySchedule(self.npc_1D0, gUnk_080F3FD8, context);
    ResetNpcDailySchedule(self.karen, gKarenScheduleInfo, context);
    ResetNpcDailySchedule(self.npc_1FC, gUnk_080F3010, context);
    ResetNpcDailySchedule(self.elli, gElliScheduleInfo, context);
    ResetNpcDailySchedule(self.npc_228, gUnk_080F6DE8, context);
    ResetNpcDailySchedule(self.npc_23C, gCliffScheduleInfo, context);
    ResetNpcDailySchedule(self.npc_250, gUnk_080F42F0, context);
    ResetNpcDailySchedule(self.ann, gAnnScheduleInfo, context);
    ResetNpcDailySchedule(self.npc_27C, gUnk_080F6FF8, context);
    ResetNpcDailySchedule(self.npc_290, gCarpenterScheduleInfo, context);
    ResetNpcDailySchedule(self.npc_2A8, gUnk_080F2DC0, context);
    ResetNpcDailySchedule(self.npc_2BC, gUnk_080F597C, context);
    ResetNpcDailySchedule(self.npc_310, gUnk_080F6B10, context);
    ResetNpcDailySchedule(self.npc_328, gUnk_080F4974, context);

    if (Npc * npc = GetActiveScheduleNpc(self))
        ResetNpcDailySchedule(*npc, gChildScheduleInfo, context);
}

struct PACKED SavedCalendarDate
{
    u8 yearInCycle : 3;
    u8 yearCycle : 5;
    u8 packedDateAndTime[2];
};

enum NpcScheduleWeekday
{
    SCHEDULE_SUNDAY,
    SCHEDULE_MONDAY,
    SCHEDULE_TUESDAY,
    SCHEDULE_WEDNESDAY,
    SCHEDULE_THURSDAY,
    SCHEDULE_FRIDAY,
    SCHEDULE_SATURDAY,
};

struct CalendarDate
{
    u8 year;
    u8 season;
    u8 day;
    u8 weekday;
};

struct PACKED EventScheduleFlags
{
    u32 : 19;
    u32 unk_217A_3 : 2;
};

union PACKED BacheloretteScheduleEvents
{
    u8 bytes[2];
    struct PACKED
    {
        u16 : 7;
        u16 ann : 2;
        u16 : 7;
    } states;
};

struct NpcScheduleContext
{
    STRUCT_PAD(0x0000, 0x0008);
    u32 weather;
    STRUCT_PAD(0x000C, 0x0010);
    SavedCalendarDate calendar;
    STRUCT_PAD(0x0013, 0x1CD4);
    u8 unk_1CD4[4];
    STRUCT_PAD(0x1CD8, 0x1D6C);
    Bachelorette popuri;
    STRUCT_PAD(0x1D84, 0x1DAC);
    u32 unk_1DAC[2];
    STRUCT_PAD(0x1DB4, 0x1E28);
    Bachelorette mary;
    STRUCT_PAD(0x1E40, 0x1EB8);
    Bachelorette karen;
    STRUCT_PAD(0x1ED0, 0x1EE4);
    Bachelorette elli;
    STRUCT_PAD(0x1EFC, 0x1F38);
    Bachelorette ann;
    STRUCT_PAD(0x1F50, 0x2148);
    u8 marriedScheduleVariant;
    STRUCT_PAD(0x2149, 0x215A);
    u8 unk_215A;
    STRUCT_PAD(0x215B, 0x216B);
    BacheloretteScheduleEvents marriageScheduleEvents;
    STRUCT_PAD(0x216D, 0x2174);
    u32 unk_2174;
    union
    {
        u32 unk_2178;
        EventScheduleFlags scheduleFlags;
    };
    STRUCT_PAD(0x217C, 0x2181);
    u8 cliffEventStates;
    STRUCT_PAD(0x2182, 0x2186);
    u8 : 5;
    u8 unk_2186_5 : 2;
    u8 : 1;
    u8 : 3;
    u8 unk_2187_3 : 2;
    u8 : 3;
    STRUCT_PAD(0x2188, 0x21A2);
    u8 unk_21A2;
    BacheloretteScheduleEvents seasonalScheduleEvents;
    u8 maryWinterScheduleEvent;
    STRUCT_PAD(0x21A6, 0x21CA);
    u8 eventFlags;
};

EC CalendarDate & DecodeNpcScheduleDate(CalendarDate &, SavedCalendarDate const &);
EC unsigned GetRosterMarriageCandidate(void const *);

ROM_TEXT(0803D7E4) unsigned SelectNpcSchedule_080F1A80(void const * context)
{
    NpcScheduleContext const & self = *static_cast<NpcScheduleContext const *>(context);
    if (self.eventFlags & 0x0C)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (IsNpcScheduleFestivalDay(season, day))
        return 4;
    if (self.weather == 0)
    {
        if (weekday == SCHEDULE_TUESDAY)
            return 3;
        return 1;
    }
    return 2;
}

enum CliffSchedule
{
    CLIFF_SCHEDULE_ABSENT,
    CLIFF_SCHEDULE_REGULAR,
    CLIFF_SCHEDULE_WORKING,
    CLIFF_SCHEDULE_EMPLOYED_SATURDAY,
    CLIFF_SCHEDULE_FESTIVAL,
    CLIFF_SCHEDULE_STORY_OVERRIDE,
};

ROM_TEXT(0803DDE8) unsigned SelectCliffSchedule(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 6)) < 0)
        return CLIFF_SCHEDULE_ABSENT;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned year = date.year;
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;

    // Retain the register used by both offsets and the original flag extraction.
    register unsigned flagOffset asm("r1") = offsetof(NpcScheduleContext, cliffEventStates);
    asm("" : "+r"(flagOffset));
    unsigned departureState = static_cast<u32>(reinterpret_cast<u8 const *>(&self)[flagOffset]) << 26;
    asm("" : "+r"(departureState) : "r"(flagOffset));
    if ((departureState >> 30) == 2)
        return CLIFF_SCHEDULE_ABSENT;
    flagOffset -= offsetof(NpcScheduleContext, cliffEventStates) - offsetof(NpcScheduleContext, unk_2174) - 1;
    asm("" : "+r"(flagOffset));
    register u8 const * wineryEventByte asm("r0") = reinterpret_cast<u8 const *>(&self) + flagOffset;
    asm("" : "+r"(wineryEventByte));
    if (static_cast<i32>(static_cast<u32>(*wineryEventByte) << (31 - 4)) >= 0)
    {
        if (year > 1)
            return CLIFF_SCHEDULE_ABSENT;
        if (year == 1 && season == SEASON_WINTER)
        {
            register unsigned winterDay asm("r0") = day;
            asm("" : "+r"(winterDay));
            if (winterDay > 28)
                return CLIFF_SCHEDULE_ABSENT;
        }
    }
    unsigned storyState = static_cast<u32>(self.cliffEventStates) << 30;
    asm("" : "+r"(storyState));
    if ((storyState >> 30) == 1)
        return CLIFF_SCHEDULE_STORY_OVERRIDE;
    if (IsNpcScheduleFestivalDay(season, day))
        return CLIFF_SCHEDULE_FESTIVAL;
    if (year != 1 || season != SEASON_AUTUMN || day != 15)
        goto regularSchedule;
    if (static_cast<i32>(static_cast<u32>(reinterpret_cast<u8 const *>(&self.unk_2174)[1]) << (31 - 4)) >= 0)
        goto regularSchedule;
workingSchedule:
    return CLIFF_SCHEDULE_WORKING;
regularSchedule:
    unsigned schedule;
    if ((self.unk_2174 & 0x61000) == 0x41000)
    {
        if (weekday != SCHEDULE_SATURDAY)
            goto workingSchedule;
        schedule = CLIFF_SCHEDULE_EMPLOYED_SATURDAY;
    }
    else
        schedule = CLIFF_SCHEDULE_REGULAR;
    return schedule;
}

ROM_TEXT(0803DECC) unsigned SelectNpcSchedule_080F2DC0(NpcScheduleContext const & self)
{
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    if (IsNpcScheduleFestivalDay(date.season, date.day))
        return 2;
    return 1;
}

ROM_TEXT(0803DECC) unsigned SelectNpcSchedule_080F3010(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 5)) < 0)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (IsNpcScheduleFestivalDay(season, day))
        return 4;
    if (weekday == SCHEDULE_WEDNESDAY)
    {
        if (self.weather == 0)
            return 2;
        return 3;
    }
    return 1;
}

ROM_TEXT(0803DC64) unsigned SelectNpcSchedule_080F280C(NpcScheduleContext const & self)
{
    if (self.eventFlags & 0xC)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (IsNpcScheduleFestivalDay(season, day))
        return 3;
    if (weekday == SCHEDULE_SUNDAY)
        return 2;
    return 1;
}

ROM_TEXT(0803DECC) unsigned SelectNpcSchedule_080F33B8(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 5)) < 0)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    if (IsNpcScheduleFestivalDay(date.season, date.day))
        return 2;
    return 1;
}

ROM_TEXT(0803DECC) unsigned SelectNpcSchedule_080F3408(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 2)) < 0)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (IsNpcScheduleFestivalDay(season, day))
        return 4;
    if (weekday == SCHEDULE_TUESDAY)
        return 2;
    if (weekday == SCHEDULE_SUNDAY)
        return 3;
    return 1;
}

ROM_TEXT(0803E1A4) unsigned SelectNpcSchedule_080F3FD8(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 2)) < 0)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (IsNpcScheduleFestivalDay(season, day))
        return 4;
    if (self.weather == 0)
    {
        if (weekday == SCHEDULE_TUESDAY)
            return 3;
        return 1;
    }
    return 2;
}

enum KarenSchedule
{
    KAREN_SCHEDULE_ABSENT,
    KAREN_SCHEDULE_CLEAR_WEATHER,
    KAREN_SCHEDULE_OTHER_WEATHER,
    KAREN_SCHEDULE_SUNDAY,
    KAREN_SCHEDULE_TUESDAY_CLEAR,
    KAREN_SCHEDULE_TUESDAY_OTHER_WEATHER,
    KAREN_SCHEDULE_AFTER_RIVAL_EVENTS,
    KAREN_SCHEDULE_AFTER_RIVAL_EVENTS_SUNDAY,
    KAREN_SCHEDULE_FESTIVAL,
    KAREN_SCHEDULE_WINTER_14,
    KAREN_SCHEDULE_AFTER_RIVAL_EVENTS_FESTIVAL,
    KAREN_SCHEDULE_MARRIED_ACTIVE_NPC,
    KAREN_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT,
    KAREN_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT_SUNDAY_TUESDAY,
    KAREN_SCHEDULE_MARRIED,
    KAREN_SCHEDULE_MARRIED_VARIANT,
    KAREN_SCHEDULE_MARRIED_VARIANT_SUNDAY_TUESDAY,
    KAREN_SCHEDULE_MARRIED_ACTIVE_NPC_FESTIVAL_OR_STORM,
    KAREN_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT_FESTIVAL_OR_STORM,
    KAREN_SCHEDULE_MARRIED_FESTIVAL_OR_STORM,
    KAREN_SCHEDULE_MARRIED_VARIANT_FESTIVAL_OR_STORM,
    KAREN_SCHEDULE_MARRIED_AUTUMN_9,
    KAREN_SCHEDULE_MARRIED_AUTUMN_13,
    KAREN_SCHEDULE_MARRIED_EVENT_OVERRIDE,
};

enum { KAREN_NPC_ID = 19 };

struct G1NpcRoster;

EC u8 * GetActiveRosterNpc(u8 *);
EC Bachelorette * GetRosterBacheloretteById(G1NpcRoster *, u32);

ROM_TEXT(0803DFE8) unsigned SelectKarenSchedule(NpcScheduleContext const & self)
{
    if (self.eventFlags & 0xC)
        return KAREN_SCHEDULE_ABSENT;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    u8 * roster = const_cast<u8 *>(self.unk_1CD4);
    unsigned spouseNpcId = GetRosterMarriageCandidate(roster);
    if (spouseNpcId == KAREN_NPC_ID)
    {
        if (season == SEASON_AUTUMN)
        {
            if (day == 9)
                return KAREN_SCHEDULE_MARRIED_AUTUMN_9;
            if (day == 13)
                return KAREN_SCHEDULE_MARRIED_AUTUMN_13;
        }
        if ((static_cast<u32>(self.marriageScheduleEvents.bytes[1]) << 27 >> 30) == 1)
            return KAREN_SCHEDULE_MARRIED_EVENT_OVERRIDE;
        unsigned marriedVariant = self.marriedScheduleVariant;
        uintptr_t activeNpc = reinterpret_cast<uintptr_t>(GetActiveRosterNpc(const_cast<u8 *>(self.unk_1CD4)));
        // Keep the original branchless non-null test.
        unsigned hasActiveNpc = ((0u - activeNpc) | activeNpc) >> 31;
        if (IsNpcScheduleFestivalDay(season, day) || self.weather == 3 || self.weather == 4)
        {
            if (hasActiveNpc)
            {
                if (marriedVariant)
                    return KAREN_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT_FESTIVAL_OR_STORM;
                return KAREN_SCHEDULE_MARRIED_ACTIVE_NPC_FESTIVAL_OR_STORM;
            }
            if (marriedVariant)
                return KAREN_SCHEDULE_MARRIED_VARIANT_FESTIVAL_OR_STORM;
            return KAREN_SCHEDULE_MARRIED_FESTIVAL_OR_STORM;
        }
        if (marriedVariant && (weekday == SCHEDULE_SUNDAY || weekday == SCHEDULE_TUESDAY))
        {
            if (hasActiveNpc)
                return KAREN_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT_SUNDAY_TUESDAY;
            return KAREN_SCHEDULE_MARRIED_VARIANT_SUNDAY_TUESDAY;
        }
        if (hasActiveNpc)
        {
            if (marriedVariant)
                return KAREN_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT;
            return KAREN_SCHEDULE_MARRIED_ACTIVE_NPC;
        }
        if (marriedVariant)
            return KAREN_SCHEDULE_MARRIED_VARIANT;
        return KAREN_SCHEDULE_MARRIED;
    }
    if (self.karen.GetRivalEventCount() > 4)
    {
        if (IsNpcScheduleFestivalDay(season, day))
            return KAREN_SCHEDULE_AFTER_RIVAL_EVENTS_FESTIVAL;
        if (weekday == SCHEDULE_SUNDAY)
            return KAREN_SCHEDULE_AFTER_RIVAL_EVENTS_SUNDAY;
        return KAREN_SCHEDULE_AFTER_RIVAL_EVENTS;
    }
    Bachelorette const * karen = GetRosterBacheloretteById(reinterpret_cast<G1NpcRoster *>(roster), KAREN_NPC_ID);
    if (season == SEASON_WINTER && day == 14 && spouseNpcId == 0 && karen && karen->GetLove() > 9999
        && (static_cast<u32>(self.seasonalScheduleEvents.bytes[1]) << 25 >> 30) != 2)
        return KAREN_SCHEDULE_WINTER_14;
    if (IsNpcScheduleFestivalDay(season, day))
        return KAREN_SCHEDULE_FESTIVAL;
    if (weekday == SCHEDULE_SUNDAY)
        return KAREN_SCHEDULE_SUNDAY;
    if (weekday == SCHEDULE_TUESDAY)
    {
        if (self.weather == 0)
            return KAREN_SCHEDULE_TUESDAY_CLEAR;
        return KAREN_SCHEDULE_TUESDAY_OTHER_WEATHER;
    }
    if (self.weather == 0)
        return KAREN_SCHEDULE_CLEAR_WEATHER;
    return KAREN_SCHEDULE_OTHER_WEATHER;
}

ROM_TEXT(0803E1A4) unsigned SelectNpcSchedule_080F42F0(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 6)) < 0)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    if (IsNpcScheduleFestivalDay(season, day))
        return 2;
    unsigned event = GetRosterMarriageCandidate(self.unk_1CD4);
    if (season == SEASON_AUTUMN && day == 5 && event != 25)
        return 3;
    return 1;
}

enum AnnSchedule
{
    ANN_SCHEDULE_ABSENT,
    ANN_SCHEDULE_CLEAR_WEATHER,
    ANN_SCHEDULE_OTHER_WEATHER,
    ANN_SCHEDULE_AFTER_RIVAL_EVENTS_CLEAR,
    ANN_SCHEDULE_AFTER_RIVAL_EVENTS_OTHER_WEATHER,
    ANN_SCHEDULE_AUTUMN_5,
    ANN_SCHEDULE_FESTIVAL,
    ANN_SCHEDULE_WINTER_14,
    ANN_SCHEDULE_MARRIED_ACTIVE_NPC,
    ANN_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT,
    ANN_SCHEDULE_MARRIED,
    ANN_SCHEDULE_MARRIED_VARIANT,
    ANN_SCHEDULE_MARRIED_ACTIVE_NPC_FESTIVAL_OR_STORM,
    ANN_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT_FESTIVAL_OR_STORM,
    ANN_SCHEDULE_MARRIED_FESTIVAL_OR_STORM,
    ANN_SCHEDULE_MARRIED_VARIANT_FESTIVAL_OR_STORM,
    ANN_SCHEDULE_MARRIED_AUTUMN_9,
    ANN_SCHEDULE_MARRIED_AUTUMN_13,
    ANN_SCHEDULE_MARRIED_EVENT_OVERRIDE,
};

enum { ANN_NPC_ID = 25 };

ROM_TEXT(0803E264) unsigned SelectAnnSchedule(NpcScheduleContext const & self)
{
    // Preserve the original register allocation around the schedule helper calls.
    register NpcScheduleContext const * context asm("r5") = &self;
    asm("" : "+r"(context));
    if (static_cast<i32>(static_cast<u32>(context->eventFlags) << 25) < 0)
        return ANN_SCHEDULE_ABSENT;
    CalendarDate date;
    DecodeNpcScheduleDate(date, context->calendar);
    register unsigned season asm("r8") = date.season;
    register unsigned day asm("r6") = date.day;
    asm("" : "+r"(season), "+r"(day));
    unsigned spouseNpcId = GetRosterMarriageCandidate(context->unk_1CD4);
    if (season == SEASON_AUTUMN && day == 5)
    {
        if (spouseNpcId != ANN_NPC_ID)
            return ANN_SCHEDULE_AUTUMN_5;
        goto married;
    }
    if (spouseNpcId == ANN_NPC_ID)
    {
married:
        if (season == SEASON_AUTUMN)
        {
            if (day == 9)
                return ANN_SCHEDULE_MARRIED_AUTUMN_9;
            if (day == 13)
                return ANN_SCHEDULE_MARRIED_AUTUMN_13;
        }
        if (context->marriageScheduleEvents.states.ann == 1)
            return ANN_SCHEDULE_MARRIED_EVENT_OVERRIDE;
        register unsigned marriedVariant asm("r4") = context->marriedScheduleVariant;
        asm("" : "+r"(marriedVariant));
        uintptr_t activeNpc = reinterpret_cast<uintptr_t>(GetActiveRosterNpc(const_cast<u8 *>(context->unk_1CD4)));
        unsigned hasActiveNpc = ((0u - activeNpc) | activeNpc) >> 31;
        register unsigned hasActiveNpcOnFestival asm("r9") = hasActiveNpc;
        asm("" : "+r"(hasActiveNpcOnFestival) : "r"(hasActiveNpc));
        if (IsNpcScheduleFestivalDay(season, day))
            goto festivalOrStorm;
        {
            register unsigned weather asm("r0") = context->weather;
            asm("" : "+r"(weather));
            if (weather == 3)
                goto festivalOrStorm;
            asm("" : "+r"(weather));
            if (weather != 4)
                goto normalMarriedDay;
        }
festivalOrStorm:
        {
            if (hasActiveNpcOnFestival)
            {
                if (marriedVariant)
                    return ANN_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT_FESTIVAL_OR_STORM;
                return ANN_SCHEDULE_MARRIED_ACTIVE_NPC_FESTIVAL_OR_STORM;
            }
            if (marriedVariant)
                return ANN_SCHEDULE_MARRIED_VARIANT_FESTIVAL_OR_STORM;
            return ANN_SCHEDULE_MARRIED_FESTIVAL_OR_STORM;
        }
normalMarriedDay:
        if (hasActiveNpc)
        {
            if (marriedVariant)
                return ANN_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT;
            return ANN_SCHEDULE_MARRIED_ACTIVE_NPC;
        }
        if (marriedVariant)
            return ANN_SCHEDULE_MARRIED_VARIANT;
        return ANN_SCHEDULE_MARRIED;
    }
    if (context->ann.GetRivalEventCount() > 4)
    {
        if (IsNpcScheduleFestivalDay(season, day))
            goto festival;
        if (context->weather == 0)
            return ANN_SCHEDULE_AFTER_RIVAL_EVENTS_CLEAR;
        return ANN_SCHEDULE_AFTER_RIVAL_EVENTS_OTHER_WEATHER;
    }
    {
        Bachelorette const * ann = GetRosterBacheloretteById(
            reinterpret_cast<G1NpcRoster *>(const_cast<u8 *>(context->unk_1CD4)), ANN_NPC_ID);
        if (season == SEASON_WINTER && day == 14 && spouseNpcId == 0 && ann && ann->GetLove() > 9999
            && context->seasonalScheduleEvents.states.ann != 2)
            return ANN_SCHEDULE_WINTER_14;
    }
    if (IsNpcScheduleFestivalDay(season, day))
    {
festival:
        return ANN_SCHEDULE_FESTIVAL;
    }
    if (context->weather == 0)
        return ANN_SCHEDULE_CLEAR_WEATHER;
    return ANN_SCHEDULE_OTHER_WEATHER;
}

enum PopuriSchedule
{
    POPURI_SCHEDULE_ABSENT,
    POPURI_SCHEDULE_CLEAR_WEATHER,
    POPURI_SCHEDULE_OTHER_WEATHER,
    POPURI_SCHEDULE_SUNDAY_CLEAR,
    POPURI_SCHEDULE_SUNDAY_OTHER_WEATHER,
    POPURI_SCHEDULE_SUMMER_SUNDAY_CLEAR,
    POPURI_SCHEDULE_MARRIED_ACTIVE_NPC,
    POPURI_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT,
    POPURI_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT_SUNDAY,
    POPURI_SCHEDULE_MARRIED,
    POPURI_SCHEDULE_MARRIED_VARIANT,
    POPURI_SCHEDULE_MARRIED_VARIANT_SUNDAY,
    POPURI_SCHEDULE_MARRIED_ACTIVE_NPC_FESTIVAL_OR_STORM,
    POPURI_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT_FESTIVAL_OR_STORM,
    POPURI_SCHEDULE_MARRIED_FESTIVAL_OR_STORM,
    POPURI_SCHEDULE_MARRIED_VARIANT_FESTIVAL_OR_STORM,
    POPURI_SCHEDULE_FESTIVAL,
    POPURI_SCHEDULE_AUTUMN_30,
    POPURI_SCHEDULE_WINTER_14,
    POPURI_SCHEDULE_AFTER_RIVAL_EVENTS_SUMMER_FESTIVAL,
    POPURI_SCHEDULE_AFTER_RIVAL_EVENTS_SUMMER_CLEAR,
    POPURI_SCHEDULE_AFTER_RIVAL_EVENTS_SUMMER_OTHER_WEATHER,
    POPURI_SCHEDULE_MARRIED_AUTUMN_13,
    POPURI_SCHEDULE_MARRIED_EVENT_OVERRIDE,
};

enum { POPURI_NPC_ID = 3 };

ROM_TEXT(0803DA7C) unsigned SelectPopuriSchedule(NpcScheduleContext const & self)
{
    CalendarDate date;
    unsigned season, day, weekday, spouseNpcId;
    Bachelorette const * popuri;
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << 28) < 0)
        goto absent;
    DecodeNpcScheduleDate(date, self.calendar);
    season = date.season;
    day = date.day;
    weekday = date.weekday;
    spouseNpcId = GetRosterMarriageCandidate(self.unk_1CD4);
    if (spouseNpcId == POPURI_NPC_ID)
    {
        if (season == SEASON_AUTUMN && day == 13)
            return POPURI_SCHEDULE_MARRIED_AUTUMN_13;
        if ((static_cast<u32>(self.marriageScheduleEvents.bytes[0]) << 25 >> 30) == 1)
            return POPURI_SCHEDULE_MARRIED_EVENT_OVERRIDE;
        unsigned marriedVariant = self.marriedScheduleVariant;
        uintptr_t activeNpc = reinterpret_cast<uintptr_t>(GetActiveRosterNpc(const_cast<u8 *>(self.unk_1CD4)));
        unsigned hasActiveNpc = ((0u - activeNpc) | activeNpc) >> 31;
        if (IsNpcScheduleFestivalDay(season, day) || self.weather == 3 || self.weather == 4)
        {
            if (hasActiveNpc)
            {
                if (marriedVariant)
                    return POPURI_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT_FESTIVAL_OR_STORM;
                return POPURI_SCHEDULE_MARRIED_ACTIVE_NPC_FESTIVAL_OR_STORM;
            }
            if (marriedVariant)
                return POPURI_SCHEDULE_MARRIED_VARIANT_FESTIVAL_OR_STORM;
            return POPURI_SCHEDULE_MARRIED_FESTIVAL_OR_STORM;
        }
        if (marriedVariant && weekday == SCHEDULE_SUNDAY)
        {
            if (hasActiveNpc)
                return POPURI_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT_SUNDAY;
            return POPURI_SCHEDULE_MARRIED_VARIANT_SUNDAY;
        }
        if (hasActiveNpc)
        {
            if (marriedVariant)
                return POPURI_SCHEDULE_MARRIED_ACTIVE_NPC_VARIANT;
            return POPURI_SCHEDULE_MARRIED_ACTIVE_NPC;
        }
        if (marriedVariant)
            return POPURI_SCHEDULE_MARRIED_VARIANT;
        return POPURI_SCHEDULE_MARRIED;
    }
    if (self.popuri.GetRivalEventCount() > 4)
    {
        if (season != SEASON_SUMMER)
            goto absent;
        if (IsNpcScheduleFestivalDay(SEASON_SUMMER, day))
            return POPURI_SCHEDULE_AFTER_RIVAL_EVENTS_SUMMER_FESTIVAL;
        if (self.weather == 0)
            return POPURI_SCHEDULE_AFTER_RIVAL_EVENTS_SUMMER_CLEAR;
        return POPURI_SCHEDULE_AFTER_RIVAL_EVENTS_SUMMER_OTHER_WEATHER;
    }
    if (season == SEASON_AUTUMN && day == 30 && (static_cast<u32>(self.seasonalScheduleEvents.bytes[0]) << 30 >> 30) != 2)
        return POPURI_SCHEDULE_AUTUMN_30;
    popuri = GetRosterBacheloretteById(
        reinterpret_cast<G1NpcRoster *>(const_cast<u8 *>(self.unk_1CD4)), POPURI_NPC_ID);
    if (season == SEASON_WINTER && day == 14 && spouseNpcId == 0 && popuri && popuri->GetLove() > 9999
        && (static_cast<u32>(self.seasonalScheduleEvents.bytes[0]) << 26 >> 30) != 2)
        return POPURI_SCHEDULE_WINTER_14;
    if (IsNpcScheduleFestivalDay(season, day))
        return POPURI_SCHEDULE_FESTIVAL;
    if (static_cast<i32>(static_cast<u32>(self.unk_215A) << 28) < 0)
    {
absent:
        return POPURI_SCHEDULE_ABSENT;
    }
    if (weekday == SCHEDULE_SUNDAY)
    {
        if (self.weather == 0)
        {
            if (season == SEASON_SUMMER)
                return POPURI_SCHEDULE_SUMMER_SUNDAY_CLEAR;
            return POPURI_SCHEDULE_SUNDAY_CLEAR;
        }
        return POPURI_SCHEDULE_SUNDAY_OTHER_WEATHER;
    }
    if (self.weather == 0)
        return POPURI_SCHEDULE_CLEAR_WEATHER;
    return POPURI_SCHEDULE_OTHER_WEATHER;
}

enum MarySchedule
{
    MARY_SCHEDULE_ABSENT,
    MARY_SCHEDULE_REGULAR,
    MARY_SCHEDULE_MONDAY_CLEAR,
    MARY_SCHEDULE_MONDAY_OTHER_WEATHER,
    MARY_SCHEDULE_AFTER_RIVAL_EVENTS,
    MARY_SCHEDULE_AFTER_RIVAL_EVENTS_THURSDAY_CLEAR,
    MARY_SCHEDULE_AFTER_RIVAL_EVENTS_THURSDAY_OTHER_WEATHER,
    MARY_SCHEDULE_WINTER_14,
    MARY_SCHEDULE_FESTIVAL,
    MARY_SCHEDULE_AFTER_RIVAL_EVENTS_FESTIVAL,
    MARY_SCHEDULE_MARRIED_ACTIVE_NPC,
    MARY_SCHEDULE_MARRIED_ACTIVE_NPC_MONDAY,
    MARY_SCHEDULE_MARRIED,
    MARY_SCHEDULE_MARRIED_MONDAY,
    MARY_SCHEDULE_MARRIED_ACTIVE_NPC_FESTIVAL_OR_STORM,
    MARY_SCHEDULE_MARRIED_FESTIVAL_OR_STORM,
    MARY_SCHEDULE_MARRIED_AUTUMN_9,
    MARY_SCHEDULE_MARRIED_AUTUMN_13,
    MARY_SCHEDULE_MARRIED_EVENT_OVERRIDE,
};

enum { MARY_NPC_ID = 12 };

ROM_TEXT(0803E4A0) unsigned SelectMarySchedule(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << 27) < 0)
        return MARY_SCHEDULE_ABSENT;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    u8 * roster = const_cast<u8 *>(self.unk_1CD4);
    unsigned spouseNpcId = GetRosterMarriageCandidate(roster);
    if (spouseNpcId == MARY_NPC_ID)
    {
        if (season == SEASON_AUTUMN)
        {
            if (day == 9)
                return MARY_SCHEDULE_MARRIED_AUTUMN_9;
            if (day == 13)
                return MARY_SCHEDULE_MARRIED_AUTUMN_13;
        }
        if ((static_cast<u32>(self.marriageScheduleEvents.bytes[1]) << 25 >> 30) == 1)
            return MARY_SCHEDULE_MARRIED_EVENT_OVERRIDE;
        uintptr_t activeNpc = reinterpret_cast<uintptr_t>(GetActiveRosterNpc(const_cast<u8 *>(self.unk_1CD4)));
        unsigned hasActiveNpc = ((0u - activeNpc) | activeNpc) >> 31;
        register unsigned hasActiveNpcOnFestival asm("r8") = hasActiveNpc;
        asm("" : "+r"(hasActiveNpcOnFestival) : "r"(hasActiveNpc));
        if (IsNpcScheduleFestivalDay(season, day) || self.weather == 3 || self.weather == 4)
        {
            if (hasActiveNpcOnFestival)
                return MARY_SCHEDULE_MARRIED_ACTIVE_NPC_FESTIVAL_OR_STORM;
            return MARY_SCHEDULE_MARRIED_FESTIVAL_OR_STORM;
        }
        if (weekday == SCHEDULE_MONDAY)
        {
            if (hasActiveNpc)
                return MARY_SCHEDULE_MARRIED_ACTIVE_NPC_MONDAY;
            return MARY_SCHEDULE_MARRIED_MONDAY;
        }
        if (hasActiveNpc)
            return MARY_SCHEDULE_MARRIED_ACTIVE_NPC;
        return MARY_SCHEDULE_MARRIED;
    }
    if (self.mary.GetRivalEventCount() > 4)
    {
        if (IsNpcScheduleFestivalDay(season, day))
            return MARY_SCHEDULE_AFTER_RIVAL_EVENTS_FESTIVAL;
        if (weekday == SCHEDULE_THURSDAY)
        {
            if (self.weather == 0)
                return MARY_SCHEDULE_AFTER_RIVAL_EVENTS_THURSDAY_CLEAR;
            return MARY_SCHEDULE_AFTER_RIVAL_EVENTS_THURSDAY_OTHER_WEATHER;
        }
        return MARY_SCHEDULE_AFTER_RIVAL_EVENTS;
    }
    Bachelorette const * mary = GetRosterBacheloretteById(reinterpret_cast<G1NpcRoster *>(roster), MARY_NPC_ID);
    if (season == SEASON_WINTER && day == 14 && spouseNpcId == 0 && mary && mary->GetLove() > 9999
        && (static_cast<u32>(self.maryWinterScheduleEvent) << 30 >> 30) != 2)
        return MARY_SCHEDULE_WINTER_14;
    if (IsNpcScheduleFestivalDay(season, day))
        return MARY_SCHEDULE_FESTIVAL;
    if (weekday == SCHEDULE_MONDAY)
    {
        if (self.weather == 0)
            return MARY_SCHEDULE_MONDAY_CLEAR;
        return MARY_SCHEDULE_MONDAY_OTHER_WEATHER;
    }
    return MARY_SCHEDULE_REGULAR;
}

enum ElliSchedule
{
    ELLI_SCHEDULE_ABSENT,
    ELLI_SCHEDULE_REGULAR,
    ELLI_SCHEDULE_WEDNESDAY,
    ELLI_SCHEDULE_AFTER_RIVAL_EVENTS,
    ELLI_SCHEDULE_AFTER_RIVAL_EVENTS_WEDNESDAY,
    ELLI_SCHEDULE_FESTIVAL,
    ELLI_SCHEDULE_WINTER_14,
    ELLI_SCHEDULE_AFTER_RIVAL_EVENTS_FESTIVAL,
    ELLI_SCHEDULE_MARRIED_ACTIVE_NPC,
    ELLI_SCHEDULE_MARRIED_ACTIVE_NPC_WEDNESDAY,
    ELLI_SCHEDULE_MARRIED,
    ELLI_SCHEDULE_MARRIED_WEDNESDAY,
    ELLI_SCHEDULE_MARRIED_ACTIVE_NPC_FESTIVAL_OR_STORM,
    ELLI_SCHEDULE_MARRIED_FESTIVAL_OR_STORM,
    ELLI_SCHEDULE_MARRIED_AUTUMN_9,
    ELLI_SCHEDULE_MARRIED_AUTUMN_13,
    ELLI_SCHEDULE_MARRIED_EVENT_OVERRIDE,
};

enum { ELLI_NPC_ID = 21 };

ROM_TEXT(0803E780) unsigned SelectElliSchedule(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << 26) < 0)
        return ELLI_SCHEDULE_ABSENT;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    u8 * roster = const_cast<u8 *>(self.unk_1CD4);
    unsigned spouseNpcId = GetRosterMarriageCandidate(roster);
    if (spouseNpcId == ELLI_NPC_ID)
    {
        if (season == SEASON_AUTUMN)
        {
            if (day == 9)
                return ELLI_SCHEDULE_MARRIED_AUTUMN_9;
            if (day == 13)
                return ELLI_SCHEDULE_MARRIED_AUTUMN_13;
        }
        if ((static_cast<u32>(self.marriageScheduleEvents.bytes[1]) << 29 >> 30) == 1)
            return ELLI_SCHEDULE_MARRIED_EVENT_OVERRIDE;
        uintptr_t activeNpc = reinterpret_cast<uintptr_t>(GetActiveRosterNpc(const_cast<u8 *>(self.unk_1CD4)));
        unsigned hasActiveNpc = ((0u - activeNpc) | activeNpc) >> 31;
        register unsigned hasActiveNpcOnFestival asm("r8") = hasActiveNpc;
        asm("" : "+r"(hasActiveNpcOnFestival) : "r"(hasActiveNpc));
        if (IsNpcScheduleFestivalDay(season, day) || self.weather == 3 || self.weather == 4)
        {
            if (hasActiveNpcOnFestival)
                return ELLI_SCHEDULE_MARRIED_ACTIVE_NPC_FESTIVAL_OR_STORM;
            return ELLI_SCHEDULE_MARRIED_FESTIVAL_OR_STORM;
        }
        if (weekday == SCHEDULE_WEDNESDAY)
        {
            if (hasActiveNpc)
                return ELLI_SCHEDULE_MARRIED_ACTIVE_NPC_WEDNESDAY;
            return ELLI_SCHEDULE_MARRIED_WEDNESDAY;
        }
        if (hasActiveNpc)
            return ELLI_SCHEDULE_MARRIED_ACTIVE_NPC;
        return ELLI_SCHEDULE_MARRIED;
    }
    if (self.elli.GetRivalEventCount() > 4)
    {
        if (IsNpcScheduleFestivalDay(season, day))
            return ELLI_SCHEDULE_AFTER_RIVAL_EVENTS_FESTIVAL;
        if (weekday == SCHEDULE_WEDNESDAY)
            return ELLI_SCHEDULE_AFTER_RIVAL_EVENTS_WEDNESDAY;
        return ELLI_SCHEDULE_AFTER_RIVAL_EVENTS;
    }
    Bachelorette const * elli = GetRosterBacheloretteById(reinterpret_cast<G1NpcRoster *>(roster), ELLI_NPC_ID);
    if (season == SEASON_WINTER && day == 14 && spouseNpcId == 0 && elli && elli->GetLove() > 9999
        && (static_cast<u32>(self.seasonalScheduleEvents.bytes[1]) << 28 >> 30) != 2)
        return ELLI_SCHEDULE_WINTER_14;
    if (IsNpcScheduleFestivalDay(season, day))
        return ELLI_SCHEDULE_FESTIVAL;
    if (weekday == SCHEDULE_WEDNESDAY)
        return ELLI_SCHEDULE_WEDNESDAY;
    return ELLI_SCHEDULE_REGULAR;
}

enum ChildSchedule
{
    CHILD_SCHEDULE_ABSENT,
    CHILD_SCHEDULE_REGULAR,
    CHILD_SCHEDULE_LATE_MORNING,
    CHILD_SCHEDULE_HOME_ALL_DAY,
    CHILD_SCHEDULE_WALKING_AGE,
    CHILD_SCHEDULE_AUTUMN_13,
};

enum { HARVEST_GODDESS_NPC_ID = 31 };

EC u32 GetNamedNpcAgeDays(u8 *);

ROM_TEXT(0803DCB4) unsigned SelectChildSchedule(NpcScheduleContext const & self)
{
    unsigned spouseNpcId, season, day, weekday, marriedVariant;
    CalendarDate date;
    u8 * roster = const_cast<u8 *>(self.unk_1CD4);
    u8 * child = GetActiveRosterNpc(roster);
    if (!child)
        goto absent;
    if (GetNamedNpcAgeDays(child) > 59)
        goto walkingAge;
    spouseNpcId = GetRosterMarriageCandidate(roster);
    if (spouseNpcId == HARVEST_GODDESS_NPC_ID)
        return CHILD_SCHEDULE_HOME_ALL_DAY;
    DecodeNpcScheduleDate(date, self.calendar);
    season = date.season;
    day = date.day;
    weekday = date.weekday;
    if (season == SEASON_AUTUMN && day == 13)
        return CHILD_SCHEDULE_AUTUMN_13;
    if (IsNpcScheduleFestivalDay(season, day) || self.weather == 3 || self.weather == 4)
        goto lateMorning;
    marriedVariant = self.marriedScheduleVariant;
    switch (spouseNpcId)
    {
    case MARY_NPC_ID:
        if (weekday != SCHEDULE_MONDAY)
            break;
    lateMorning:
        return CHILD_SCHEDULE_LATE_MORNING;
    case POPURI_NPC_ID:
        if (!marriedVariant)
            goto lateMorning;
        if (weekday != SCHEDULE_SUNDAY)
            break;
        // Keep the two Sunday checks separate for the original branch layout.
        asm("" : : "r"(weekday));
        goto lateMorning;
    case KAREN_NPC_ID:
        if (!marriedVariant || weekday == SCHEDULE_TUESDAY || weekday == SCHEDULE_SUNDAY)
            goto lateMorning;
        break;
    case ELLI_NPC_ID:
        if (weekday != SCHEDULE_WEDNESDAY)
            break;
        goto lateMorning;
    case ANN_NPC_ID:
        if (!marriedVariant)
            goto lateMorning;
        break;
    case HARVEST_GODDESS_NPC_ID:
        break;
    }
    return CHILD_SCHEDULE_REGULAR;
walkingAge:
    return CHILD_SCHEDULE_WALKING_AGE;
absent:
    return CHILD_SCHEDULE_ABSENT;
}

ROM_TEXT(0803E40C) unsigned SelectNpcSchedule_080F4974(NpcScheduleContext const & self)
{
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned weekday = date.weekday;
    switch (self.unk_2187_3)
    {
    case 2:
        if (weekday == SCHEDULE_SUNDAY)
            return 1;
        break;
    }
    return 0;
}

ROM_TEXT(0803E40C) unsigned SelectNpcSchedule_080F49C0(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 4)) < 0)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (IsNpcScheduleFestivalDay(season, day))
        return 4;
    if (weekday == SCHEDULE_MONDAY)
    {
        if (self.weather == 0)
            return 2;
        return 3;
    }
    return 1;
}

ROM_TEXT(0803E620) unsigned SelectNpcSchedule_080F5540(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 4)) < 0)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (IsNpcScheduleFestivalDay(season, day))
        return 5;
    if (weekday == SCHEDULE_MONDAY)
    {
        if (self.weather == 0)
            return 3;
        return 4;
    }
    if (self.weather == 0)
        return 1;
    return 2;
}

ROM_TEXT(0803E620) unsigned SelectNpcSchedule_080F597C(NpcScheduleContext const & self)
{
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    u8 season = date.season;
    u8 day = date.day;
    if (IsNpcScheduleFestivalDay(season, day))
        return 2;
    unsigned maxYear = 200;
    unsigned calendarYear = self.calendar.yearCycle * 7 + self.calendar.yearInCycle;
    if (std::min(maxYear, calendarYear) <= 1 && season == SEASON_SPRING && day <= 2)
        return 0;
    return 1;
}

ROM_TEXT(0803E620) unsigned SelectNpcSchedule_080F59CC(NpcScheduleContext const & self)
{
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (season == SEASON_WINTER && day == 2 && (self.unk_2178 & 0x43000) == 0x2000)
    {
        switch (self.scheduleFlags.unk_217A_3)
        {
        default: return 7;
        case 2: break;
        }
        return 0;
    }
    if (IsNpcScheduleFestivalDay(season, day))
        return 6;
    if (weekday == SCHEDULE_SATURDAY)
    {
        if (self.weather == 0)
            return 2;
        return 3;
    }
    if (weekday == SCHEDULE_SUNDAY)
    {
        if (self.weather == 0)
            return 4;
        return 5;
    }
    return 1;
}

ROM_TEXT(0803E8E8) unsigned SelectNpcSchedule_080F61FC(NpcScheduleContext const & self)
{
    unsigned weekday;
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 5)) >= 0)
    {
        CalendarDate date;
        DecodeNpcScheduleDate(date, self.calendar);
        unsigned season = date.season;
        unsigned day = date.day;
        weekday = date.weekday;
        if (season == SEASON_AUTUMN && day == 30 && (self.unk_21A2 >> 6) != 2)
            return 5;
        if (IsNpcScheduleFestivalDay(season, day))
            return 4;
        if (static_cast<i32>(static_cast<u32>(self.unk_215A) << (31 - 3)) >= 0)
            goto active;
    }
    return 0;
active:
    if (weekday == SCHEDULE_MONDAY || weekday == SCHEDULE_WEDNESDAY)
        return 3;
    if (self.weather == 0)
        return 1;
    return 2;
}

ROM_TEXT(0803E8E8) unsigned SelectNpcSchedule_080F6370(NpcScheduleContext const & self)
{
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    u32 weekday = date.weekday;
    // Keep the copy for the original register allocation.
    u32 weekdayForRegularSchedule = weekday;
    if (IsNpcScheduleFestivalDay(season, day))
        return 5;
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 6)) < 0)
        return 0;
    if ((self.unk_2174 & 0x61000) == 0x41000)
    {
        if (weekday == SCHEDULE_SATURDAY)
            return 4;
        return 3;
    }
    if (weekdayForRegularSchedule == SCHEDULE_SATURDAY)
        return 2;
    return 1;
}

ROM_TEXT(0803E8E8) unsigned SelectNpcSchedule_080F66C4(NpcScheduleContext const & self)
{
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    u8 weekday = date.weekday;
    if (IsNpcScheduleFestivalDay(season, day))
        return 5;
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 6)) < 0)
        return 0;
    if (self.weather == 0)
    {
        if (weekday == SCHEDULE_SATURDAY)
        {
            if ((self.unk_2174 & 0x61000) == 0x41000)
                return 4;
            return 3;
        }
        return 1;
    }
    return 2;
}

ROM_TEXT(0803E8E8) unsigned SelectNpcSchedule_080F6B10(NpcScheduleContext const & self)
{
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned weekday = date.weekday;
    switch (self.unk_2186_5)
    {
    case 2:
        if (weekday == SCHEDULE_WEDNESDAY)
            return 1;
        break;
    }
    return 0;
}

ROM_TEXT(0803E8E8) unsigned SelectNpcSchedule_080F6B4C(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 5)) < 0)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    if (IsNpcScheduleFestivalDay(date.season, date.day))
        return 2;
    return 1;
}

ROM_TEXT(0803E8E8) unsigned SelectNpcSchedule_080F6DE8(NpcScheduleContext const & self)
{
    if (self.eventFlags & 0x7C)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (season == SEASON_AUTUMN && day == 3)
        return 9;
    if (IsNpcScheduleFestivalDay(season, day))
        return 8;
    if (weekday == SCHEDULE_MONDAY || weekday == SCHEDULE_WEDNESDAY || self.weather != 0)
        return 1;
    return 2;
}

ROM_TEXT(0803E8E8) unsigned SelectNpcSchedule_080F6FF8(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 3)) >= 0)
    {
        CalendarDate date;
        DecodeNpcScheduleDate(date, self.calendar);
        unsigned season = date.season;
        unsigned day = date.day;
        unsigned weekday = date.weekday;
        if (season == SEASON_SUMMER)
        {
            if (IsNpcScheduleFestivalDay(1, day))
                return 4;
            if (weekday == SCHEDULE_SUNDAY)
            {
                if (self.weather == 0)
                    return 2;
                return 3;
            }
            return 1;
        }
    }
    return 0;
}

ROM_TEXT(0803E8E8) unsigned SelectNpcSchedule_080F7294(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 4)) < 0)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    unsigned rivalEvents = self.mary.GetRivalEventCount();
    if (rivalEvents > 4)
    {
        if (IsNpcScheduleFestivalDay(season, day))
            return 9;
        if (weekday == SCHEDULE_THURSDAY)
        {
            if (self.weather == 0)
                return 6;
            return 7;
        }
        return 5;
    }
    if (IsNpcScheduleFestivalDay(season, day))
        return 8;
    if (rivalEvents != 0)
    {
        if (weekday == SCHEDULE_MONDAY)
            return 3;
        if (weekday == SCHEDULE_THURSDAY)
            return 4;
        return 2;
    }
    return 1;
}

EC unsigned GetNpcMilestoneEventId(void const *);
EC bool IsNpcMilestoneEventReady(void const *);

ROM_TEXT(0803E8E8) unsigned SelectNpcSchedule_080F77FC(NpcScheduleContext const & self)
{
    if (static_cast<i32>(static_cast<u32>(self.eventFlags) << (31 - 4)) < 0)
        return 0;
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (IsNpcScheduleFestivalDay(season, day))
        return 4;
    void const * npc = self.unk_1DAC;
    if (GetNpcMilestoneEventId(npc) != 0 && !IsNpcMilestoneEventReady(npc))
        return 5;
    if (weekday == SCHEDULE_THURSDAY)
    {
        if (self.weather == 0)
            return 2;
        return 3;
    }
    return 1;
}

struct CarpenterConstructionState
{
    STRUCT_PAD(0, 0x14);
    u8 jobAndProgress;
    u8 dailyFlags;
};

enum ConstructionJob
{
    CONSTRUCTION_NONE = 0,
    CONSTRUCTION_HOUSE_UPGRADE = 1,
    CONSTRUCTION_COOP_UPGRADE = 2,
    CONSTRUCTION_BARN_UPGRADE = 3,
    CONSTRUCTION_WINDOW_FIRST = 5,
    CONSTRUCTION_WINDOW_SECOND,
    CONSTRUCTION_WINDOW_THIRD,
    CONSTRUCTION_MAILBOX_FIRST,
    CONSTRUCTION_MAILBOX_SECOND,
    CONSTRUCTION_MAILBOX_THIRD,
    CONSTRUCTION_DOGHOUSE_FIRST,
    CONSTRUCTION_DOGHOUSE_SECOND,
    CONSTRUCTION_DOGHOUSE_THIRD,
};

struct CarpenterScheduleContext
{
    STRUCT_PAD(0, 8);
    unsigned weather;
    STRUCT_PAD(0xC, 0x10);
    SavedCalendarDate date;
    STRUCT_PAD(0x13, 0x1F4);
    FarmHouse farmHouse;
    u8 padAfterFarmHouse[0x1F64 - 0x1F4 - sizeof(FarmHouse)];
    CarpenterConstructionState construction;
};

EC unsigned GetConstructionJob(CarpenterConstructionState const &);
EC bool IsConstructionComplete(CarpenterConstructionState const &);
EC bool IsFirstConstructionDay(CarpenterConstructionState const &);

ROM_TEXT(0803ECC0) unsigned SelectCarpenterSchedule(CarpenterScheduleContext const & self)
{
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.date);
    unsigned season = date.season;
    unsigned day = date.day;
    int weekday = date.weekday;
    if (IsNpcScheduleFestivalDay(season, day))
        return 5;
    unsigned weather = self.weather;
    if (GetConstructionJob(self.construction) != CONSTRUCTION_NONE && !IsConstructionComplete(self.construction) &&
        !IsFirstConstructionDay(self.construction) && weather - 3 > 1)
    {
        switch (GetConstructionJob(self.construction))
        {
        case CONSTRUCTION_NONE: goto regular;
        case CONSTRUCTION_HOUSE_UPGRADE:
            switch (self.farmHouse.GetUpgradeLevel())
            {
            case 0: return 6;
            case 1: return 7;
            case 2: return 13;
            }
            break;
        case CONSTRUCTION_COOP_UPGRADE: return 8;
        case CONSTRUCTION_BARN_UPGRADE: return 9;
        case 4: return 14;
        case CONSTRUCTION_WINDOW_FIRST:
        case CONSTRUCTION_WINDOW_SECOND:
        case CONSTRUCTION_WINDOW_THIRD: return 10;
        case CONSTRUCTION_MAILBOX_FIRST:
        case CONSTRUCTION_MAILBOX_SECOND:
        case CONSTRUCTION_MAILBOX_THIRD: return 11;
        case CONSTRUCTION_DOGHOUSE_FIRST:
        case CONSTRUCTION_DOGHOUSE_SECOND:
        case CONSTRUCTION_DOGHOUSE_THIRD: return 12;
        }
    }
regular:
    switch (weekday)
    {
    default: return 1;
    case SCHEDULE_THURSDAY:
    case SCHEDULE_FRIDAY: return 2;
    case SCHEDULE_SATURDAY: return 3;
    case SCHEDULE_SUNDAY: return 4;
    }
}

ROM_TEXT(0803EDCC) unsigned SelectNpcSchedule_080F81BC(NpcScheduleContext const & self)
{
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    u8 day = date.day;
    unsigned weekday = date.weekday;
    u8 festival = IsNpcScheduleFestivalDay(season, day);
    if (festival != 0)
        return 5;
    if (season == SEASON_AUTUMN && day == 30)
    {
        switch ((static_cast<u32>(self.unk_21A2) << 26) >> 30)
        {
        case 2: goto regularDay;
        }
        return 6;
    }
regularDay:
    if (static_cast<i32>(static_cast<u32>(self.unk_215A) << (31 - 3)) < 0)
        return 0;
    if (self.weather == 0)
    {
        if (weekday == SCHEDULE_MONDAY)
            return 1;
        if (weekday == SCHEDULE_WEDNESDAY)
            return 2;
        return 3;
    }
    return 4;
}

ROM_TEXT(0803EDCC) unsigned SelectNpcSchedule_080F8678(NpcScheduleContext const & self)
{
    CalendarDate date;
    DecodeNpcScheduleDate(date, self.calendar);
    unsigned season = date.season;
    unsigned day = date.day;
    unsigned weekday = date.weekday;
    if (IsNpcScheduleFestivalDay(season, day))
        return 4;
    if (weekday == SCHEDULE_MONDAY)
    {
        if (self.weather == 0)
            return 2;
        return 3;
    }
    return 1;
}

struct ControllerInput;
struct AudioPlayer;

EC int UpdatePauseMenuWindow(PauseMenuWindow &, ControllerInput const &, OamBuffer *,
    BackgroundVideoState *, DialogLayerRenderer *, TileTransferQueue *, AudioPlayer *);
EC void func_0803BE50(DialogWindow &, int);
EC void func_080E0C38(PauseMenuGraphics *, int);
EC void func_080E0D70(void *, int);
EC char const gPauseMenuWindowVtable[];
EC char const vtable_unk_080E77E0[];

struct Unk_0803C7A0
{
    void * inner;
};

EC void func_0803C7A0(Unk_0803C7A0 &, int);
EC void func_0803C5B0(Unk_0803C7A0 &);

struct DialogChoiceWindow : DialogWindow
{
    Unk_0803C7A0 menu;
};

struct DialogChoiceWindowVtable
{
    STRUCT_PAD(0, 0x1C);
    int (* getX)(DialogChoiceWindow &);
    int (* getY)(DialogChoiceWindow &);
};

EC void DrawDialogChoices(Unk_0803C7A0 &, int x, int y, OamBuffer *, DialogLayerRenderer *);

ROM_TEXT(0803D4D8) int UpdateDialogChoiceWindow(DialogChoiceWindow & self, OamBuffer * oam,
    BackgroundVideoState * video, DialogLayerRenderer * layerRenderer, TileTransferQueue * tileTransfers)
{
    // Preserve the original register allocation for the window state.
    register int state asm("r6") = UpdateDialogWindow(self, video, layerRenderer, tileTransfers);
    if (state == DIALOG_WINDOW_OPEN)
    {
        int x = static_cast<DialogChoiceWindowVtable const *>(self.vtable)->getX(self);
        int y = static_cast<DialogChoiceWindowVtable const *>(self.vtable)->getY(self);
        DrawDialogChoices(self.menu, x, y, oam, layerRenderer);
    }
    return state;
}

EC DialogWindow & func_0803BDFC(DialogWindow &, int, int, int, int, int, int, int, int);

EC PauseMenuGraphics * func_080E0A94(void *);

ROM_TEXT(0803BF78) PauseMenuWindow & InitializePauseMenuWindow(PauseMenuWindow & self, int background,
    int screenBlock, int paletteBank, int layerMode, int selectedTab)
{
    func_0803BDFC(self, 0, 136, 240, 24, background, screenBlock, paletteBank, layerMode);
    self.vtable = gPauseMenuWindowVtable;
    self.graphics = func_080E0A94(::operator new(0x41C));
    self.selectedTab = selectedTab;
    self.needsInitialRefresh = false;
    self.cursorBlinkFrames = 1;
    self.selectionEnabled = true;
    return self;
}

ROM_TEXT(0803BF78) void DestroyPauseMenuWindow(PauseMenuWindow & self, int flags)
{
    self.vtable = gPauseMenuWindowVtable;
    if (self.graphics)
        func_080E0C38(self.graphics, 3);
    func_0803BE50(self, flags);
}

ROM_TEXT(0803BFF0) int UpdatePauseMenuWindowWithoutAudio(PauseMenuWindow & self, ControllerInput const & input,
    OamBuffer * sprites, BackgroundVideoState * video, DialogLayerRenderer * renderer, TileTransferQueue * transfers)
{
    return UpdatePauseMenuWindow(self, input, sprites, video, renderer, transfers, nullptr);
}

ROM_TEXT(0803C00C) int UpdatePauseMenuWindowWithAudio(PauseMenuWindow & self, ControllerInput const & input,
    OamBuffer * sprites, BackgroundVideoState * video, DialogLayerRenderer * renderer, TileTransferQueue * transfers,
    AudioPlayer * audio)
{
    return UpdatePauseMenuWindow(self, input, sprites, video, renderer, transfers, audio);
}

ROM_TEXT(0803C518) DialogChoiceWindow & func_0803C54C(DialogChoiceWindow & self, int arg1, int arg2, int arg3, int arg4)
{
    func_0803BDFC(self, 128, 136, 112, 24, arg1, arg2, arg3, arg4);
    self.vtable = vtable_unk_080E77E0;
    func_0803C5B0(self.menu);
    return self;
}

ROM_TEXT(0803C518) void func_0803C588(DialogChoiceWindow & self, int flags)
{
    self.vtable = vtable_unk_080E77E0;
    func_0803C7A0(self.menu, 2);
    func_0803BE50(self, flags);
}

ROM_TEXT(0803C7A0) void func_0803C7A0(Unk_0803C7A0 & self, int flags)
{
    void * inner = self.inner;
    if (inner != 0)
        func_080E0D70(inner, 3);
    flags &= 1;
    if (flags != 0)
        ::operator delete(&self);
}
