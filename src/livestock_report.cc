#include "prelude.h"

#include "farm.hh"
#include "utility/vec2.hh"

#include <algorithm>
#include <new>

#define REPORT_CODE(addr) EC SECTION(".text.f3." #addr)

struct FarmReportData
{
    STRUCT_PAD(0x00, 0x14);
    Farm farm;
};

enum FarmReportPageId
{
    FARM_OVERVIEW,
    CHICKEN_REPORT,
    COW_REPORT,
    SHEEP_REPORT,
    HARVEST_SPRITE_REPORT,
    EARNINGS_REPORT,
    TOOL_LEVELS
};

struct FarmReportPage
{
    FarmReportPageId id;
    i16 scroll_y;

    FarmReportPage() : id(FARM_OVERVIEW), scroll_y(0) {}
};

struct FarmReportPageList
{
    enum { CAPACITY = 7 };

    u32 count;
    u32 storage[CAPACITY * sizeof(FarmReportPage) / sizeof(u32)];

    FarmReportPage * FirstPage() { return reinterpret_cast<FarmReportPage *>(storage); }

    FarmReportPage * Entry(u32 index)
    {
        size_t offset = sizeof(count) + index * sizeof(FarmReportPage);
        return reinterpret_cast<FarmReportPage *>(reinterpret_cast<u8 *>(this) + offset);
    }

    void Append(FarmReportPage const & page)
    {
        if (count < CAPACITY)
        {
            new (Entry(count)) FarmReportPage(page);
            ++count;
        }
    }
};

struct FarmReportMenu
{
    STRUCT_PAD(0, 4);
    void const * vtable;
    STRUCT_PAD(8, 0x120);
    FarmReportData const * data;
    FarmReportPageList pages;
    u32 selected_page;
};

EC char const gFarmReportMenuVTable[] asm("vtable_unk_080E7A88");
EC void InitScrollableReportMenu(FarmReportMenu * self, u32 mode) asm("func_08075E7C");
EC void OpenFarmReportPage(FarmReportMenu * self, FarmReportPageId id) asm("func_08069C34");

REPORT_CODE(0805FD78) FarmReportMenu * InitFarmReportMenu(
    FarmReportMenu * self, FarmReportData const * data, u8 mode)
{
    InitScrollableReportMenu(self, mode);
    self->vtable = gFarmReportMenuVTable;

    // Preserve the original Thumb register allocation.
    register uintptr_t data_offset asm("r1") = offsetof(FarmReportMenu, data);
    asm ("" : "+r" (data_offset));
    register FarmReportData const ** destination asm("r0") =
        reinterpret_cast<FarmReportData const **>(reinterpret_cast<uintptr_t>(self) + data_offset);
    asm ("" : "+r" (destination));
    *destination = data;

    register FarmReportPageList * initial_pages asm("r6") = &self->pages;
    asm ("" : "+r" (initial_pages));
    initial_pages->count = 0;

    FarmReportPage page;
    page.id = FARM_OVERVIEW;
    new (self->pages.FirstPage()) FarmReportPage(page);
    ++initial_pages->count;

    register FarmReportData const * const * data_slot asm("r4") = &self->data;
    asm ("" : "+r" (data_slot));
    FarmReportData const * const & menu_data = *data_slot;
    bool has_chickens = menu_data->farm.coop.CountChickens();
    if (has_chickens)
    {
        page.id = CHICKEN_REPORT;
        register FarmReportPageList * chicken_pages asm("r1") = initial_pages;
        asm ("" : "+r" (chicken_pages));
        if (chicken_pages->count < FarmReportPageList::CAPACITY)
        {
            new (chicken_pages->Entry(chicken_pages->count)) FarmReportPage(page);
            ++initial_pages->count;
        }
    }
    bool has_cows = menu_data->farm.barn.CountCows();
    if (has_cows)
    {
        page.id = COW_REPORT;
        self->pages.Append(page);
    }
    bool has_sheep = menu_data->farm.barn.CountSheeps();
    if (has_sheep)
    {
        page.id = SHEEP_REPORT;
        self->pages.Append(page);
    }
    page.id = HARVEST_SPRITE_REPORT;
    self->pages.Append(page);
    page.id = EARNINGS_REPORT;
    self->pages.Append(page);
    page.id = TOOL_LEVELS;
    self->pages.Append(page);
    self->selected_page = 0;
    OpenFarmReportPage(self, self->pages.FirstPage()->id);
    return self;
}

struct LivestockReport
{
    enum { MAX_NAME_BITMAPS = 16, NAME_BITMAP_WIDTH = 12, NAME_BITMAP_HEIGHT = 2 };
    enum { NAME_BITMAP_BYTES = NAME_BITMAP_WIDTH * NAME_BITMAP_HEIGHT * 32, NAME_BITMAP_STRIDE = NAME_BITMAP_BYTES + 4 };
    STRUCT_PAD(0x00, 0x04);
    void * scroll_panel;
    STRUCT_PAD(0x08, 0x58C);
    FarmReportData const * data;
    STRUCT_PAD(0x590, 0x5B4);
    void const * vtable;
    STRUCT_PAD(0x5B8, 0x12E0);
    u8 name_bitmaps[(MAX_NAME_BITMAPS - 1) * NAME_BITMAP_STRIDE + NAME_BITMAP_BYTES];
};

EC void FillMessageTiles(Vec2 dimensions, void * tiles, u32 color);
EC void DrawMessageTileString(Vec2 dimensions, void * tiles, u32 x, u32 y, char const * text);
EC void DrawFarmReportPanel(LivestockReport * self) asm("func_08076EA8");
EC void SetFarmReportScrollHeight(void * panel, i16 height) asm("func_08075E24");
EC void InitLivestockReport(LivestockReport * self, void * menu, void * display, u32 content_height, char const * title);
EC void AddLivestockReportRow(LivestockReport * self, u32 row, Livestock const * animal, void const * tiles, void const * palette);

EC char const gUnk_080FB880[];
EC char const gUnk_080FB894[];
EC char const gUnk_080FB8A4[];
EC char const vtable_unk_080E7A58[];
EC char const vtable_unk_080E7A48[];
EC char const vtable_unk_080E7A38[];
EC u8 const gUnk_0875268C[], gUnk_0875270C[], gUnk_0875272C[], gUnk_087527AC[];
EC u8 const gUnk_08751B4C[], gUnk_08751BCC[], gUnk_08751C8C[], gUnk_08751D0C[];
EC u8 const gUnk_08751DCC[], gUnk_08751E4C[], gUnk_08751BEC[], gUnk_08751C6C[], gUnk_08751D2C[], gUnk_08751DAC[];
EC u8 const gUnk_0875320C[], gUnk_0875328C[], gUnk_0875334C[], gUnk_087533CC[];
EC u8 const gUnk_087532AC[], gUnk_0875332C[], gUnk_087533EC[], gUnk_0875346C[];

REPORT_CODE(08063E04) u8 * RenderLivestockReportName(LivestockReport * self, u32 row, char const * name)
{
    if (row < LivestockReport::MAX_NAME_BITMAPS)
    {
        uintptr_t address = row * LivestockReport::NAME_BITMAP_STRIDE;
        address += reinterpret_cast<uintptr_t>(self);
        u8 * bitmap;
        FillMessageTiles(Vec2(LivestockReport::NAME_BITMAP_WIDTH, LivestockReport::NAME_BITMAP_HEIGHT),
            bitmap = reinterpret_cast<u8 *>(address + offsetof(LivestockReport, name_bitmaps)), 0);
        DrawMessageTileString(Vec2(LivestockReport::NAME_BITMAP_WIDTH, LivestockReport::NAME_BITMAP_HEIGHT),
            bitmap, 0, 0, name);
        return bitmap;
    }
    return nullptr;
}

REPORT_CODE(08063E58) bool DrawLivestockReport(LivestockReport * self)
{
    DrawFarmReportPanel(self);
    return true;
}

REPORT_CODE(08063E64) u32 GetLivestockReportHeartCount(Animal const * animal)
{
    return std::min<u32>(10, animal->GetAffection() / 25);
}

REPORT_CODE(08063E8C) u16 GetLivestockReportMoodTile(Livestock const * animal)
{
    enum { HEALTHY_TILE = 0x38, UNHAPPY_TILE = 0x48, SICK_TILE = 0x58 };
    if (animal->IsSick())
        return SICK_TILE;
    if (animal->IsUnhappy())
        return UNHAPPY_TILE;
    return HEALTHY_TILE;
}

REPORT_CODE(08064320) LivestockReport * InitChickenReport(LivestockReport * self, void * menu, void * display)
{
    InitLivestockReport(self, menu, display, 45, gUnk_080FB880);
    self->vtable = vtable_unk_080E7A58;

    u32 row = 0, slot = 0;
    u32 capacity = self->data->farm.coop.GetCapacity();
    for (; slot < capacity; ++slot)
    {
        Chicken const * animal = self->data->farm.coop.GetChicken(slot);
        if (animal == nullptr)
            continue;

        u8 const * tiles;
        u8 const * palette;
        switch (static_cast<u32>(animal->GetGrowthStage()))
        {
            case Chicken::STAGE_1:
                tiles = gUnk_0875268C;
                palette = gUnk_0875270C;
                break;
            case Chicken::STAGE_0:
            default:
                tiles = gUnk_0875272C;
                palette = gUnk_087527AC;
                break;
        }
        AddLivestockReportRow(self, row, animal, tiles, palette);
        ++row;
    }
    SetFarmReportScrollHeight(self->scroll_panel, (row * 5 + 5) * 8);
    return self;
}

REPORT_CODE(080643DC) LivestockReport * InitCowReport(LivestockReport * self, void * menu, void * display)
{
    InitLivestockReport(self, menu, display, 85, gUnk_080FB894);
    self->vtable = vtable_unk_080E7A48;

    u32 row = 0, slot = 0;
    u32 capacity = self->data->farm.barn.GetCapacity();
    for (; slot < capacity; ++slot)
    {
        Cow const * animal = self->data->farm.barn.GetCow(slot);
        if (animal == nullptr)
            continue;

        u8 const * tiles;
        u8 const * palette;
        switch (static_cast<u32>(animal->GetGrowthStage()))
        {
            case Cow::STAGE_2:
                tiles = gUnk_08751B4C;
                palette = gUnk_08751BCC;
                if (animal->IsPregnant())
                {
                    tiles = gUnk_08751C8C;
                    palette = gUnk_08751D0C;
                }
                else if (animal->HasBeenMilked())
                {
                    tiles = gUnk_08751DCC;
                    palette = gUnk_08751E4C;
                }
                break;
            case Cow::STAGE_1:
                tiles = gUnk_08751BEC;
                palette = gUnk_08751C6C;
                break;
            case Cow::STAGE_0:
            default:
                tiles = gUnk_08751D2C;
                palette = gUnk_08751DAC;
                break;
        }
        AddLivestockReportRow(self, row, animal, tiles, palette);
        ++row;
    }
    SetFarmReportScrollHeight(self->scroll_panel, (row * 5 + 5) * 8);
    return self;
}

REPORT_CODE(080644F0) LivestockReport * InitSheepReport(LivestockReport * self, void * menu, void * display)
{
    InitLivestockReport(self, menu, display, 85, gUnk_080FB8A4);
    self->vtable = vtable_unk_080E7A38;

    u32 row = 0, slot = 0;
    u32 capacity = self->data->farm.barn.GetCapacity();
    for (; slot < capacity; ++slot)
    {
        Sheep const * animal = self->data->farm.barn.GetSheep(slot);
        if (animal == nullptr)
            continue;

        u8 const * tiles;
        u8 const * palette;
        switch (animal->GetGrowthStage())
        {
            case Sheep::STAGE_1:
                tiles = gUnk_0875320C;
                palette = gUnk_0875328C;
                if (animal->IsPregnant())
                {
                    tiles = gUnk_0875334C;
                    palette = gUnk_087533CC;
                }
                else if (animal->IsSheared())
                {
                    tiles = gUnk_087532AC;
                    palette = gUnk_0875332C;
                }
                break;
            case Sheep::STAGE_0:
            default:
                tiles = gUnk_087533EC;
                palette = gUnk_0875346C;
                break;
        }
        AddLivestockReportRow(self, row, animal, tiles, palette);
        ++row;
    }
    SetFarmReportScrollHeight(self->scroll_panel, (row * 5 + 5) * 8);
    return self;
}
