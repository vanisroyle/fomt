#include "prelude.h"

#include "dma_queue.hh"
#include "farm.hh"
#include "utility/vec2.hh"

#include <algorithm>
#include <new>
#include <stdlib.h>

#define REPORT_CODE(addr) EC SECTION(".text.f3." #addr)

struct FarmReportData
{
    STRUCT_PAD(0x00, 0x11);
    GameDate report_date;
    GameTime report_time;
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

EC char const gFarmReportMenuVTable[];
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

struct FarmReportPanel
{
    STRUCT_PAD(0, 0x18);
    u16 * tilemap;
};

struct LivestockPortraitUpload
{
    i16 y;
    int x;
    void const * tiles;
    void const * palette;
};

struct LivestockNameUpload
{
    i16 y;
    void const * source;
    void * destination;
    u32 bytes;
};

EC void * RetryReportUploadAllocation(u32 bytes) asm("func_080D3BC0");

template<class Entry>
struct ReportUploadList
{
    Entry * begin;
    Entry * end;
    STRUCT_PAD(8, 0xC);
    Entry * capacity_end;
};

struct ReportInsertionCount
{
    u32 size;
    u32 count;
};

struct LivestockReport;

// Spills kept in their original order for agbcp.
struct LivestockRowFrame
{
    LivestockPortraitUpload portrait;
    LivestockNameUpload name;
    ReportInsertionCount portraits;
    ReportInsertionCount names;
    GameDate birthday;
    LivestockReport * volatile report;
    u32 volatile index;
    Livestock const * volatile animal;
    LivestockPortraitUpload * volatile portrait_source;
    FarmReportPanel * volatile panel;
    u32 reserved;
    u32 volatile lower_row;
    u32 volatile pixel_y;
    u32 volatile upper_tilemap_offset;
    GameDate * volatile birthday_ptr;
    LivestockPortraitUpload * volatile portrait_old_end;
    void * volatile new_buffer;
};

// Preserve the original Thumb register allocation at each scheduling boundary.
#define MATCH_LOW(value) asm ("" : "+l" (value))
#define MATCH_HIGH(value) asm ("" : "+h" (value))

#define EXTRACT_REPORT_FIELD(value, source, bit, width) \
    asm ("lsl %0, %1, %2\n\tlsr %0, %0, %3" : "=l"(value) \
        : "l"(source), "I"(32 - (bit) - (width)), "I"(32 - (width)) : "cc")

#define COPY_UPLOAD(target, source, a, b, c) \
    do { \
        register __typeof__(target) out asm("r0") = target; \
        register __typeof__(source) in asm("r1") = source; \
        asm volatile (".syntax unified\n\tldmia %1!, {" #a ", " #b ", " #c "}\n" \
            "\tstmia %0!, {" #a ", " #b ", " #c "}\n\tldr %1, [%1]\n\tstr %1, [%0]\n\t.syntax divided" \
            : "+l"(out), "+l"(in) : : #a, #b, #c, "memory"); \
    } while (false)

// The two outgoing call arguments sit below the row frame on the stack.
enum { ROW_FRAME_STACK_OFFSET = 2 * sizeof(u32) };

#define COPY_NAME_UPLOAD(target, source, a, b, c) \
    do { \
        register __typeof__(target) out asm("r0") = target; \
        register __typeof__(source) in asm("r1") = source; \
        asm volatile (".syntax unified\n\tldmia %1!, {" #a ", " #b ", " #c "}\n" \
            "\tstmia %0!, {" #a ", " #b ", " #c "}\n\tldr %1, [sp, %2]\n\tstr %1, [%0]\n\t.syntax divided" \
            : "+l"(out), "+l"(in) : "I"(ROW_FRAME_STACK_OFFSET + offsetof(LivestockRowFrame, name) + offsetof(LivestockNameUpload, bytes)) \
            : #a, #b, #c, "memory"); \
    } while (false)

static inline u16 * OffsetReportTilemap(u16 * tilemap, u32 offset)
{
    return reinterpret_cast<u16 *>(reinterpret_cast<u8 *>(tilemap) + offset);
}

template<class Entry>
struct ReportQueueAppend
{
    static void Copy(Entry *& target, Entry const * source)
    {
        if (target)
            new (target) Entry(*source);
    }
};

struct LivestockReport
{
    enum { MAX_NAME_BITMAPS = 16, NAME_BITMAP_WIDTH = 12, NAME_BITMAP_HEIGHT = 2 };
    enum { NAME_BITMAP_BYTES = NAME_BITMAP_WIDTH * NAME_BITMAP_HEIGHT * 32, NAME_BITMAP_STRIDE = NAME_BITMAP_BYTES + 4 };
    STRUCT_PAD(0x00, 0x04);
    FarmReportPanel * scroll_panel;
    STRUCT_PAD(0x08, 0x58C);
    FarmReportData const * data;
    STRUCT_PAD(0x590, 0x594);
    ReportUploadList<LivestockPortraitUpload> portrait_uploads;
    ReportUploadList<LivestockNameUpload> name_uploads;
    void const * vtable;
    STRUCT_PAD(0x5B8, 0x12E0);
    u8 name_bitmaps[(MAX_NAME_BITMAPS - 1) * NAME_BITMAP_STRIDE + NAME_BITMAP_BYTES];
};

typedef char LivestockUploadLayoutCheck[
    sizeof(LivestockPortraitUpload) == 0x10 && sizeof(LivestockNameUpload) == 0x10 &&
    offsetof(LivestockReport, portrait_uploads) == 0x594 &&
    offsetof(LivestockReport, name_uploads) == 0x5A4 ? 1 : -1];
typedef char LivestockReportDateLayoutCheck[
    offsetof(FarmReportData, report_date) == 0x11 && offsetof(FarmReportData, report_time) == 0x12 &&
    offsetof(FarmReportData, farm) == 0x14 ? 1 : -1];

EC void FillMessageTiles(Vec2 dimensions, void * tiles, u32 color);
EC void DrawMessageTileString(Vec2 dimensions, void * tiles, u32 x, u32 y, char const * text);
EC void DrawFarmReportPanel(void * self) asm("func_08076EA8");
EC void DestroyFarmReportPanel(void * self, u32 flags) asm("func_08076E0C");
EC void SetFarmReportScrollHeight(void * panel, i16 height) asm("func_08075E24");
EC void InitLivestockReport(LivestockReport * self, void * menu, void * display, u32 content_height, char const * title);
EC void AddLivestockReportRow(LivestockReport * self, u32 row, Livestock const * animal, void const * tiles, void const * palette);

EC char const gChickenReportTitle[];
EC char const gCowReportTitle[];
EC char const gSheepReportTitle[];
EC char const gChickenReportVTable[];
EC char const gCowReportVTable[];
EC char const gSheepReportVTable[];
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


EC void FillMessageTilemapRect(u16 *, u32, u32, u32, u32, u32);
EC void DrawTallMessageNumber(u32, u16 *, u32, u32, u32);

REPORT_CODE(08063EB8) void AddLivestockReportRow(
    LivestockReport * self, u32 row, Livestock const * animal,
    void const * tiles, void const * palette)
{
    enum
    {
        REPORT_TILEMAP_STRIDE = 32,
        TILE_BYTES = 32,
        TILE_PIXELS = 8,
        FIRST_TILE_ROW = 5,
        ROW_HEIGHT = 5,
        PORTRAIT_X = 8,
        NAME_SLOT_COUNT = 4,
        FIRST_NAME_TILE = 0x180,
        TROPHY_TILE = 0xA0,
        HEART_TILE = 0xA4,
        DIGIT_TILE = 0x120,
        YEAR_TILE = 0x98,
        DAY_TILE = 0x9C,
        FIRST_SEASON_TILE = 0x68,
        SEASON_TILE_WIDTH = 6,
        SEASON_TILE_HEIGHT = 2,
        DAYS_PER_SEASON = 30,
        DAYS_PER_YEAR = DAYS_PER_SEASON * NUM_SEASONS,
        START_OF_DAY_HOUR = 6
    };

    LivestockRowFrame frame;
    register LivestockReport * report_arg asm("r0") = self;
    register u32 row_arg asm("r1") = row;
    register Livestock const * animal_arg asm("r2") = animal;
    asm ("" : "+l"(report_arg), "+l"(row_arg), "+l"(animal_arg));
    frame.report = report_arg;
    frame.index = row_arg;
    frame.animal = animal_arg;
    register void const * colors asm("r1") = palette;
    MATCH_LOW(colors);
    register u32 idx asm("r2") = frame.index;
    MATCH_LOW(idx);
    register u32 four_rows asm("r0") = idx * (ROW_HEIGHT - 1);
    MATCH_LOW(four_rows);
    register u32 row_offset asm("r4") = four_rows + idx;
    MATCH_LOW(row_offset);
    register u32 first_row asm("r5") = row_offset + FIRST_TILE_ROW;
    MATCH_LOW(first_row);
    register u32 upper_row asm("sl") = first_row;
    MATCH_HIGH(upper_row);
    register LivestockPortraitUpload * portrait asm("r0") = &frame.portrait;
    MATCH_LOW(portrait);
    register u32 pixel_y asm("r2") = first_row * TILE_PIXELS;
    MATCH_LOW(pixel_y);
    portrait->y = pixel_y;
    frame.portrait.x = PORTRAIT_X;
    frame.portrait.tiles = tiles;
    frame.portrait.palette = colors;
    register LivestockReport * owner asm("r7") = frame.report;
    MATCH_LOW(owner);
    register uintptr_t portrait_queue_offset asm("r0") = offsetof(LivestockReport, portrait_uploads);
    MATCH_LOW(portrait_queue_offset);
    register ReportUploadList<LivestockPortraitUpload> * portrait_queue asm("r6") =
        reinterpret_cast<ReportUploadList<LivestockPortraitUpload> *>(reinterpret_cast<uintptr_t>(owner) + portrait_queue_offset);
    MATCH_LOW(portrait_queue);
    register LivestockPortraitUpload * address asm("r1");
    asm ("mov %0, sp\n\tadd %0, %1" : "=&l"(address) : "I"(ROW_FRAME_STACK_OFFSET + offsetof(LivestockRowFrame, portrait)) : "cc");
    frame.portrait_source = address;
    register LivestockPortraitUpload * position asm("r1") = portrait_queue->end;
    MATCH_LOW(position);
    register LivestockPortraitUpload * portrait_capacity asm("r0") = portrait_queue->capacity_end;
    MATCH_LOW(portrait_capacity);
    register LivestockNameUpload * info asm("r8");
    // Append the portrait, growing the upload list when full.
    if (position != portrait_capacity)
    {
        ReportQueueAppend<LivestockPortraitUpload>::Copy(position, &frame.portrait);
        ++portrait_queue->end;
        frame.pixel_y = pixel_y;
        frame.lower_row = row_offset + FIRST_TILE_ROW + 2;
        register LivestockNameUpload * name_slot asm("r0") = &frame.name;
        MATCH_LOW(name_slot);
        info = name_slot;
        MATCH_HIGH(info);
        register u32 upper_offset asm("r1") = upper_row;
        MATCH_LOW(upper_offset);
        upper_offset *= REPORT_TILEMAP_STRIDE * sizeof(u16);
        MATCH_LOW(upper_offset);
        frame.upper_tilemap_offset = upper_offset;
        register GameDate * birthday_pointer asm("r2");
        asm ("mov %0, sp\n\tadd %0, %1" : "=&l"(birthday_pointer) :
            "I"(ROW_FRAME_STACK_OFFSET + offsetof(LivestockRowFrame, birthday)) : "cc");
        frame.birthday_ptr = birthday_pointer;
    }
    else
    {
        frame.portrait_old_end = position;
        frame.portraits.count = 1;
        register LivestockPortraitUpload * begin asm("r1") = portrait_queue->begin;
        MATCH_LOW(begin);
        register LivestockPortraitUpload * old_end asm("r3") = frame.portrait_old_end;
        MATCH_LOW(old_end);
        register intptr_t occupied_bytes asm("r0") =
            reinterpret_cast<intptr_t>(old_end) - reinterpret_cast<intptr_t>(begin);
        MATCH_LOW(occupied_bytes);
        register u32 size asm("r4") = occupied_bytes >> 4;
        MATCH_LOW(size);
        frame.portraits.size = size;
        register u32 * insert_count asm("r2") = &frame.portraits.count;
        MATCH_LOW(insert_count);
        register u32 * growth_count asm("r0") = &frame.portraits.size;
        MATCH_LOW(growth_count);
        asm ("cmp %1, #1\n\tbhs 1f\n\tadd %0, %2, #0\n1:" : "+l"(growth_count) : "l"(size), "l"(insert_count) : "cc");
        register u32 growth asm("r0") = *growth_count;
        MATCH_LOW(growth);
        register u32 sum asm("r0");
        asm ("add %0, %1, %2" : "=l"(sum) : "l"(size), "l"(growth) : "cc");
        u32 capacity = sum;
        register u32 capacity_bytes asm("r9");
        register LivestockPortraitUpload * buffer asm("r0");
        if (capacity)
        {
            register u32 bytes asm("r4") = capacity * sizeof(LivestockPortraitUpload);
            MATCH_LOW(bytes);
            LivestockPortraitUpload * allocated = static_cast<LivestockPortraitUpload *>(malloc(bytes));
            capacity_bytes = bytes;
            asm ("" : "+h"(capacity_bytes) : "l"(allocated));
            if (!allocated)
                allocated = static_cast<LivestockPortraitUpload *>(RetryReportUploadAllocation(capacity_bytes));
            buffer = allocated;
            MATCH_LOW(buffer);
            begin = portrait_queue->begin;
            asm ("" : "+l"(begin) : "l"(buffer));
        }
        else
        {
            buffer = 0;
            MATCH_LOW(buffer);
            capacity_bytes = reinterpret_cast<u32>(buffer);
            MATCH_HIGH(capacity_bytes);
        }
        register LivestockPortraitUpload * replacement asm("r5") = buffer;
        MATCH_LOW(replacement);
        frame.new_buffer = replacement;
        register LivestockPortraitUpload * entry asm("r2") = begin;
        MATCH_LOW(entry);
        register LivestockPortraitUpload * target asm("r3") = replacement;
        MATCH_LOW(target);
        register u32 full_row asm("r4") = upper_row;
        MATCH_LOW(full_row);
        frame.pixel_y = full_row * TILE_PIXELS;
        register u32 lower_row asm("r5") = upper_row;
        MATCH_LOW(lower_row);
        frame.lower_row = lower_row + 2;
        register LivestockNameUpload * name_slot asm("r7") = &frame.name;
        MATCH_LOW(name_slot);
        info = name_slot;
        MATCH_HIGH(info);
        register u32 upper_offset asm("r0") = upper_row;
        MATCH_LOW(upper_offset);
        upper_offset *= REPORT_TILEMAP_STRIDE * sizeof(u16);
        MATCH_LOW(upper_offset);
        frame.upper_tilemap_offset = upper_offset;
        register GameDate * birthday_pointer asm("r1");
        asm ("mov %0, sp\n\tadd %0, %1" : "=&l"(birthday_pointer) :
            "I"(ROW_FRAME_STACK_OFFSET + offsetof(LivestockRowFrame, birthday)) : "cc");
        frame.birthday_ptr = birthday_pointer;
        register LivestockPortraitUpload * limit asm("r4") = frame.portrait_old_end;
        MATCH_LOW(limit);
        if (entry != limit)
        {
            do
            {
                if (target)
                    COPY_UPLOAD(target, entry, r4, r5, r7);
                ++entry;
                ++target;
                asm ("" : "+l"(entry), "+l"(target));
                register LivestockPortraitUpload * reload asm("r5") = frame.portrait_old_end;
                MATCH_LOW(reload);
                if (entry == reload)
                    break;
            } while (true);
        }
        register LivestockPortraitUpload * output asm("r5") = target;
        MATCH_LOW(output);
        register u32 count asm("r0") = frame.portraits.count;
        MATCH_LOW(count);
        if (count == 1)
        {
            if (output)
                COPY_UPLOAD(output, static_cast<LivestockPortraitUpload *>(frame.portrait_source), r2, r3, r7);
            ++output;
        }
        else
        {
            register u32 remaining asm("r3") = count;
            MATCH_LOW(remaining);
            register LivestockPortraitUpload * next asm("r2") = output;
            MATCH_LOW(next);
            if (remaining)
            {
                do
                {
                    if (next)
                        COPY_UPLOAD(next, static_cast<LivestockPortraitUpload *>(frame.portrait_source), r4, r5, r7);
                    --remaining;
                    ++next;
                    asm ("" : "+l"(remaining), "+l"(next));
                } while (remaining);
            }
            output = next;
        }
        MATCH_LOW(output);
        register LivestockPortraitUpload * old_limit asm("r2") = portrait_queue->end;
        MATCH_LOW(old_limit);
        register LivestockPortraitUpload * old_begin asm("r1") = portrait_queue->begin;
        MATCH_LOW(old_begin);
        register LivestockPortraitUpload * walk asm("r0") = old_begin;
        MATCH_LOW(walk);
        if (old_begin != old_limit)
        {
            do
            {
                ++walk;
                MATCH_LOW(walk);
            } while (walk != old_limit);
        }
        if (old_begin)
            free(old_begin);
        register uintptr_t allocation_end asm("r0") = reinterpret_cast<uintptr_t>(frame.new_buffer);
        MATCH_LOW(allocation_end);
        allocation_end += capacity_bytes;
        MATCH_LOW(allocation_end);
        register LivestockPortraitUpload * allocation asm("r1") =
            static_cast<LivestockPortraitUpload *>(frame.new_buffer);
        MATCH_LOW(allocation);
        portrait_queue->begin = allocation;
        portrait_queue->end = output;
        portrait_queue->capacity_end = reinterpret_cast<LivestockPortraitUpload *>(allocation_end);
    }
    register LivestockReport * report asm("r2") = frame.report;
    MATCH_LOW(report);
    register FarmReportPanel * panel asm("r2") = report->scroll_panel;
    MATCH_LOW(panel);
    frame.panel = panel;
    if (frame.animal->IsFestivalWinner())
    {
        register u32 trophy_row asm("r1") = upper_row;
        MATCH_LOW(trophy_row);
        trophy_row -= 2;
        MATCH_LOW(trophy_row);
        register FarmReportPanel * panel asm("r3") = frame.panel;
        MATCH_LOW(panel);
        register u16 * tilemap asm("r0") = panel->tilemap;
        MATCH_LOW(tilemap);
        register u32 offset asm("r1") = trophy_row * REPORT_TILEMAP_STRIDE * sizeof(u16);
        MATCH_LOW(offset);
        u16 * cell = OffsetReportTilemap(tilemap, offset) + 1;
        FillMessageTilemapRect(cell, TROPHY_TILE, 2, 2, 1, REPORT_TILEMAP_STRIDE);
    }
    register u32 hearts asm("r6") = GetLivestockReportHeartCount(frame.animal);
    MATCH_LOW(hearts);
    register u32 heart asm("r4") = 0;
    MATCH_LOW(heart);
    if (heart < hearts)
    {
        register u32 heart_offset asm("r5") = 5 * sizeof(u16);
        MATCH_LOW(heart_offset);
        do
        {
            register FarmReportPanel * panel asm("r7") = frame.panel;
            MATCH_LOW(panel);
            register u16 * tilemap asm("r0") = *reinterpret_cast<u16 * volatile *>(&panel->tilemap);
            MATCH_LOW(tilemap);
            register u32 lower_row asm("r2") = frame.lower_row;
            MATCH_LOW(lower_row);
            register u32 offset asm("r1") = lower_row * REPORT_TILEMAP_STRIDE * sizeof(u16);
            MATCH_LOW(offset);
            register u16 * cell asm("r0") = OffsetReportTilemap(tilemap, offset);
            MATCH_LOW(cell);
            cell = OffsetReportTilemap(cell, heart_offset);
            MATCH_LOW(cell);
            FillMessageTilemapRect(cell, HEART_TILE, 1, 1, 2, REPORT_TILEMAP_STRIDE);
            heart_offset += sizeof(u16);
            MATCH_LOW(heart_offset);
            ++heart;
            MATCH_LOW(heart);
        } while (heart < hearts);
    }

    // Draw the name into its bitmap and schedule the tile upload.
    register u32 normal_palette asm("r5") = 0;
    MATCH_LOW(normal_palette);
    register volatile u32 * pixel_row asm("r3") = &frame.pixel_y;
    MATCH_LOW(pixel_row);
    register u16 name_y asm("r4") = *static_cast<volatile u16 *>(static_cast<volatile void *>(pixel_row));
    MATCH_LOW(name_y);
    register LivestockNameUpload * screen_name asm("r3") = info;
    MATCH_LOW(screen_name);
    screen_name->y = name_y;
    register char const * name asm("r2") = frame.animal->GetName();
    MATCH_LOW(name);
    register LivestockReport * title_report asm("r0") = frame.report;
    MATCH_LOW(title_report);
    register u32 title_row asm("r1") = frame.index;
    MATCH_LOW(title_row);
    register u8 * name_bitmap asm("r0") = RenderLivestockReportName(title_report, title_row, name);
    MATCH_LOW(name_bitmap);
    register LivestockNameUpload * named_upload asm("r4") = info;
    MATCH_LOW(named_upload);
    named_upload->source = name_bitmap;
    register unsigned name_slot asm("r0") = NAME_SLOT_COUNT - 1;
    MATCH_LOW(name_slot);
    register unsigned row_index asm("r7") = frame.index;
    MATCH_LOW(row_index);
    name_slot &= row_index;
    MATCH_LOW(name_slot);
    register unsigned tile asm("r4") = FIRST_NAME_TILE + name_slot
        * LivestockReport::NAME_BITMAP_WIDTH * LivestockReport::NAME_BITMAP_HEIGHT;
    MATCH_LOW(tile);
    register FarmReportPanel * name_panel asm("r1") = frame.panel;
    MATCH_LOW(name_panel);
    register u16 * name_tilemap asm("r0") = name_panel->tilemap;
    MATCH_LOW(name_tilemap);
    register u32 name_offset asm("r2") = frame.upper_tilemap_offset;
    MATCH_LOW(name_offset);
    register u16 * name_cell asm("r0") = OffsetReportTilemap(name_tilemap, name_offset);
    MATCH_LOW(name_cell);
    name_cell += 3;
    MATCH_LOW(name_cell);
    FillMessageTilemapRect(name_cell,
        tile, LivestockReport::NAME_BITMAP_WIDTH, LivestockReport::NAME_BITMAP_HEIGHT, normal_palette, REPORT_TILEMAP_STRIDE);
    register u32 tile_offset asm("r4") = tile * TILE_BYTES;
    MATCH_LOW(tile_offset);
    register u32 bg_tile_vram asm("r0") = 0x06000000;
    MATCH_LOW(bg_tile_vram);
    tile_offset += bg_tile_vram;
    MATCH_LOW(tile_offset);
    register LivestockNameUpload * upload asm("r3") = info;
    MATCH_LOW(upload);
    upload->destination = reinterpret_cast<void *>(tile_offset);
    upload->bytes = LivestockReport::NAME_BITMAP_BYTES;

    register LivestockReport * restored_report asm("r4") = frame.report;
    MATCH_LOW(restored_report);
    register uintptr_t name_queue_offset asm("r5") = offsetof(LivestockReport, name_uploads);
    MATCH_LOW(name_queue_offset);
    register ReportUploadList<LivestockNameUpload> * names asm("r6") =
        reinterpret_cast<ReportUploadList<LivestockNameUpload> *>(reinterpret_cast<uintptr_t>(restored_report) + name_queue_offset);
    MATCH_LOW(names);
    register LivestockNameUpload * name_position asm("r1") = names->end;
    MATCH_LOW(name_position);
    register LivestockNameUpload * name_capacity asm("r0") = names->capacity_end;
    MATCH_LOW(name_capacity);
    if (name_position != name_capacity)
    {
        if (name_position)
            COPY_NAME_UPLOAD(name_position, info, r2, r3, r7);
        ++names->end;
    }
    else
    {
        register LivestockNameUpload * old_end asm("r10") = name_position;
        MATCH_HIGH(old_end);
        frame.names.count = 1;
        register LivestockNameUpload * begin asm("r1") = names->begin;
        MATCH_LOW(begin);
        register LivestockNameUpload * end asm("r4") = old_end;
        MATCH_LOW(end);
        register intptr_t occupied_bytes asm("r0") =
            reinterpret_cast<intptr_t>(end) - reinterpret_cast<intptr_t>(begin);
        MATCH_LOW(occupied_bytes);
        register u32 size asm("r4") = occupied_bytes >> 4;
        MATCH_LOW(size);
        frame.names.size = size;
        register u32 * insert_count asm("r2") = &frame.names.count;
        MATCH_LOW(insert_count);
        register u32 * growth_count asm("r0") = &frame.names.size;
        MATCH_LOW(growth_count);
        asm ("cmp %1, #1\n\tbhs 1f\n\tadd %0, %2, #0\n1:" : "+l"(growth_count) : "l"(size), "l"(insert_count) : "cc");
        register u32 growth asm("r0") = *growth_count;
        MATCH_LOW(growth);
        register u32 sum asm("r0");
        asm ("add %0, %1, %2" : "=l"(sum) : "l"(size), "l"(growth) : "cc");
        u32 capacity = sum;
        register u32 capacity_bytes asm("r9");
        register LivestockNameUpload * buffer asm("r0");
        if (capacity)
        {
            register u32 bytes asm("r4") = capacity * sizeof(LivestockNameUpload);
            MATCH_LOW(bytes);
            LivestockNameUpload * allocated = static_cast<LivestockNameUpload *>(malloc(bytes));
            capacity_bytes = bytes;
            asm ("" : "+h"(capacity_bytes) : "l"(allocated));
            if (!allocated)
                allocated = static_cast<LivestockNameUpload *>(RetryReportUploadAllocation(capacity_bytes));
            buffer = allocated;
            MATCH_LOW(buffer);
            begin = names->begin;
            asm ("" : "+l"(begin) : "l"(buffer));
        }
        else
        {
            buffer = 0;
            MATCH_LOW(buffer);
            capacity_bytes = reinterpret_cast<u32>(buffer);
            MATCH_HIGH(capacity_bytes);
        }
        register LivestockNameUpload * replacement asm("r5") = buffer;
        MATCH_LOW(replacement);
        frame.new_buffer = replacement;
        register LivestockNameUpload * entry asm("r2") = begin;
        MATCH_LOW(entry);
        register LivestockNameUpload * target asm("r3") = replacement;
        MATCH_LOW(target);
        if (entry != old_end)
        {
            do
            {
                register LivestockNameUpload * checked_slot asm("r3") = target;
                MATCH_LOW(checked_slot);
                LivestockNameUpload * output_slot = checked_slot;
                if (output_slot)
                    COPY_UPLOAD(target, entry, r4, r5, r7);
                ++entry;
                ++target;
                asm ("" : "+l"(entry), "+l"(target));
                if (entry == old_end)
                    break;
            } while (true);
        }
        register LivestockNameUpload * output asm("r5") = target;
        MATCH_LOW(output);
        register u32 count_value asm("r0") = frame.names.count;
        MATCH_LOW(count_value);
        u32 count = count_value;
        if (count == 1)
        {
            if (output)
                COPY_NAME_UPLOAD(output, info, r2, r3, r7);
            ++output;
        }
        else
        {
            register u32 copies asm("r3") = count;
            MATCH_LOW(copies);
            u32 remaining = copies;
            register LivestockNameUpload * next asm("r2") = output;
            MATCH_LOW(next);
            if (remaining)
            {
                do
                {
                    if (next)
                        COPY_NAME_UPLOAD(next, info, r4, r5, r7);
                    --remaining;
                    register u32 countdown asm("r3") = remaining;
                    MATCH_LOW(countdown);
                    remaining = countdown;
                    ++next;
                    MATCH_LOW(next);
                } while (remaining);
            }
            output = next;
        }
        MATCH_LOW(output);
        register LivestockNameUpload * old_limit asm("r2") = names->end;
        MATCH_LOW(old_limit);
        register LivestockNameUpload * old_begin asm("r1") = names->begin;
        MATCH_LOW(old_begin);
        register LivestockNameUpload * walk asm("r0") = old_begin;
        MATCH_LOW(walk);
        if (old_begin != old_limit)
        {
            do
            {
                ++walk;
                MATCH_LOW(walk);
            } while (walk != old_limit);
        }
        if (old_begin)
            free(old_begin);
        register uintptr_t allocation_end asm("r0") = reinterpret_cast<uintptr_t>(frame.new_buffer);
        MATCH_LOW(allocation_end);
        allocation_end += capacity_bytes;
        MATCH_LOW(allocation_end);
        register LivestockNameUpload * allocation asm("r1") =
            static_cast<LivestockNameUpload *>(frame.new_buffer);
        MATCH_LOW(allocation);
        names->begin = allocation;
        names->end = output;
        names->capacity_end = reinterpret_cast<LivestockNameUpload *>(allocation_end);
    }

    register FarmReportPanel * mood_panel asm("r2") = frame.panel;
    MATCH_LOW(mood_panel);
    register u16 * mood_tilemap asm("r4") = mood_panel->tilemap;
    MATCH_LOW(mood_tilemap);
    register u32 mood_offset asm("r3") = frame.upper_tilemap_offset;
    MATCH_LOW(mood_offset);
    asm ("add %0, %0, %1" : "+l"(mood_tilemap) : "l"(mood_offset) : "cc");
    mood_tilemap += 16;
    MATCH_LOW(mood_tilemap);
    register u16 base_mood_tile asm("r1") = GetLivestockReportMoodTile(frame.animal);
    MATCH_LOW(base_mood_tile);
    register u32 mood_tile asm("r1") = base_mood_tile;
    MATCH_LOW(mood_tile);
    register u32 base_palette asm("r6") = 0;
    MATCH_LOW(base_palette);
    FillMessageTilemapRect(mood_tilemap, mood_tile, 8, 2, base_palette, REPORT_TILEMAP_STRIDE);

    register u32 years asm("r0") = frame.animal->GetAge() / DAYS_PER_YEAR;
    MATCH_LOW(years);
    register FarmReportPanel * age_panel asm("r4") = frame.panel;
    MATCH_LOW(age_panel);
    register u16 * age_tilemap asm("r1") = age_panel->tilemap;
    MATCH_LOW(age_tilemap);
    register u32 age_offset asm("r7") = frame.upper_tilemap_offset;
    MATCH_LOW(age_offset);
    asm ("add %0, %0, %1" : "+l"(age_tilemap) : "l"(age_offset) : "cc");
    age_tilemap += 26;
    MATCH_LOW(age_tilemap);
    register u32 digit_tile asm("r2") = DIGIT_TILE;
    MATCH_LOW(digit_tile);
    DrawTallMessageNumber(years, age_tilemap, digit_tile, 0, REPORT_TILEMAP_STRIDE);
    register u16 * year_tilemap asm("r0") = age_panel->tilemap;
    MATCH_LOW(year_tilemap);
    asm ("add %0, %0, %1" : "+l"(year_tilemap) : "l"(age_offset) : "cc");
    year_tilemap += 27;
    MATCH_LOW(year_tilemap);
    FillMessageTilemapRect(year_tilemap, YEAR_TILE, 2, 2, base_palette, REPORT_TILEMAP_STRIDE);

    // Before 06:00 the displayed date has advanced, but the animal age has not.
    register unsigned day_adjustment asm("r5") = 0;
    MATCH_LOW(day_adjustment);
    register LivestockReport * date_report asm("r0") = frame.report;
    MATCH_LOW(date_report);
    register u32 date_offset asm("r1") = offsetof(LivestockReport, data);
    MATCH_LOW(date_offset);
    register FarmReportData const * const * data_slot asm("r4") =
        reinterpret_cast<FarmReportData const * const *>(reinterpret_cast<u8 *>(date_report) + date_offset);
    MATCH_LOW(data_slot);
    register FarmReportData const * time_data asm("r0") = *data_slot;
    MATCH_LOW(time_data);
    register u32 hour_bits asm("r0") = *reinterpret_cast<u8 const *>(&time_data->report_time);
    MATCH_LOW(hour_bits);
    EXTRACT_REPORT_FIELD(hour_bits, hour_bits, 0, 5);
    if (hour_bits < START_OF_DAY_HOUR)
        day_adjustment = 1;
    MATCH_LOW(day_adjustment);
    register u32 age asm("r0") = frame.animal->GetAge();
    MATCH_LOW(age);
    register FarmReportData const * data asm("r1") = *data_slot;
    MATCH_LOW(data);
    register u32 days_old asm("r4") = age + day_adjustment;
    MATCH_LOW(days_old);
    register GameDate today asm("r0") = data->report_date;
    MATCH_LOW(today);
    register GameDate * output_date asm("r2") = frame.birthday_ptr;
    MATCH_LOW(output_date);
    *output_date = today;
    register GameDate * date asm("r6") = &frame.birthday;
    MATCH_LOW(date);
    register u32 days asm("r5");
    register u32 seasons asm("r3");
    if (days_old < DAYS_PER_SEASON)
    {
        days = days_old;
        MATCH_LOW(days);
        seasons = 0;
        MATCH_LOW(seasons);
    }
    else
    {
        days = days_old % DAYS_PER_SEASON;
        MATCH_LOW(days);
        seasons = days_old / DAYS_PER_SEASON;
        MATCH_LOW(seasons);
    }
    // Subtract the age from the packed date, borrowing a season if needed.
    register u32 today_bits asm("r0") = *reinterpret_cast<u8 *>(date);
    MATCH_LOW(today_bits);
    register u32 today_day asm("r1");
    EXTRACT_REPORT_FIELD(today_day, today_bits, 2, 5);
    register unsigned birth_day asm("r2") = days - DAYS_PER_SEASON;
    MATCH_LOW(birth_day);
    birth_day = today_day - birth_day;
    MATCH_LOW(birth_day);
    if (birth_day < DAYS_PER_SEASON)
        ++seasons;
    else
        birth_day -= DAYS_PER_SEASON;
    asm ("" : "+l"(birth_day), "+l"(seasons));
    register u32 birth_season asm("r0");
    EXTRACT_REPORT_FIELD(birth_season, today_bits, 0, 2);
    register u32 birthday_bits asm("r4") = ~(NUM_SEASONS - 1);
    MATCH_LOW(birthday_bits);
    birth_season -= seasons;
    MATCH_LOW(birth_season);
    register u32 season_mask asm("r1") = NUM_SEASONS - 1;
    MATCH_LOW(season_mask);
    birth_season &= season_mask;
    MATCH_LOW(birth_season);
    register u32 reloaded_date asm("r1") = *reinterpret_cast<u8 volatile *>(date);
    MATCH_LOW(reloaded_date);
    birthday_bits &= reloaded_date;
    MATCH_LOW(birthday_bits);
    birthday_bits |= birth_season;
    MATCH_LOW(birthday_bits);
    register u32 day_mask asm("r0") = 31;
    MATCH_LOW(day_mask);
    birth_day &= day_mask;
    MATCH_LOW(birth_day);
    register u32 day_field asm("r1") = birth_day << 2;
    MATCH_LOW(day_field);
    register u32 preserve_other_fields asm("r0") = ~(31 << 2);
    MATCH_LOW(preserve_other_fields);
    birthday_bits &= preserve_other_fields;
    MATCH_LOW(birthday_bits);
    birthday_bits |= day_field;
    MATCH_LOW(birthday_bits);
    *reinterpret_cast<u8 *>(date) = birthday_bits;

    register FarmReportPanel * birth_panel asm("r3") = frame.panel;
    MATCH_LOW(birth_panel);
    register u16 * season_tilemap asm("r0") = birth_panel->tilemap;
    MATCH_LOW(season_tilemap);
    register u32 birth_row asm("r5") = frame.lower_row;
    MATCH_LOW(birth_row);
    register u32 birth_row_offset asm("r6") = birth_row * REPORT_TILEMAP_STRIDE * sizeof(u16);
    MATCH_LOW(birth_row_offset);
    season_tilemap = OffsetReportTilemap(season_tilemap, birth_row_offset) + 17;
    MATCH_LOW(season_tilemap);
    register u32 season_index asm("r2");
    EXTRACT_REPORT_FIELD(season_index, birthday_bits, 0, 2);
    register u32 season_tile asm("r1") =
        (season_index * SEASON_TILE_WIDTH * SEASON_TILE_HEIGHT) << 16;
    MATCH_LOW(season_tile);
    register u32 season_base asm("r7") = FIRST_SEASON_TILE << 16;
    MATCH_LOW(season_base);
    season_tile = (season_tile + season_base) >> 16;
    MATCH_LOW(season_tile);
    register u32 season_palette asm("r2") = 0;
    MATCH_LOW(season_palette);
    register u32 date_palette asm("r8") = season_palette;
    MATCH_HIGH(date_palette);
    FillMessageTilemapRect(season_tilemap, season_tile, SEASON_TILE_WIDTH, SEASON_TILE_HEIGHT, season_palette, REPORT_TILEMAP_STRIDE);
    EXTRACT_REPORT_FIELD(birthday_bits, birthday_bits, 2, 5);
    ++birthday_bits;
    MATCH_LOW(birthday_bits);
    register FarmReportPanel * day_panel asm("r3") = frame.panel;
    MATCH_LOW(day_panel);
    register u16 * day_number_tilemap asm("r1") = day_panel->tilemap;
    MATCH_LOW(day_number_tilemap);
    day_number_tilemap = OffsetReportTilemap(day_number_tilemap, birth_row_offset) + 26;
    MATCH_LOW(day_number_tilemap);
    DrawTallMessageNumber(birthday_bits, day_number_tilemap, DIGIT_TILE, 0, REPORT_TILEMAP_STRIDE);
    register FarmReportPanel * unit_panel asm("r4") = frame.panel;
    MATCH_LOW(unit_panel);
    register u16 * day_unit_tilemap asm("r0") = unit_panel->tilemap;
    MATCH_LOW(day_unit_tilemap);
    day_unit_tilemap = OffsetReportTilemap(day_unit_tilemap, birth_row_offset) + 27;
    MATCH_LOW(day_unit_tilemap);
    register u32 unit_palette asm("r7") = date_palette;
    MATCH_LOW(unit_palette);
    FillMessageTilemapRect(day_unit_tilemap, DAY_TILE, 2, 2, unit_palette, REPORT_TILEMAP_STRIDE);
}

#undef COPY_NAME_UPLOAD
#undef COPY_UPLOAD
#undef EXTRACT_REPORT_FIELD
#undef MATCH_HIGH
#undef MATCH_LOW

REPORT_CODE(08064320) LivestockReport * InitChickenReport(LivestockReport * self, void * menu, void * display)
{
    InitLivestockReport(self, menu, display, 45, gChickenReportTitle);
    self->vtable = gChickenReportVTable;

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
    InitLivestockReport(self, menu, display, 85, gCowReportTitle);
    self->vtable = gCowReportVTable;

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
    InitLivestockReport(self, menu, display, 85, gSheepReportTitle);
    self->vtable = gSheepReportVTable;

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

struct FarmOverviewReport;

EC SECTION(".text.farm_report_callbacks.080E1D8C") void DestroySheepReport(LivestockReport * self, u32 flags)
{
    DestroyFarmReportPanel(self, flags);
}

EC SECTION(".text.farm_report_callbacks.080E1D98") void DestroyCowReport(LivestockReport * self, u32 flags)
{
    DestroyFarmReportPanel(self, flags);
}

EC SECTION(".text.farm_report_callbacks.080E1DA4") void DestroyChickenReport(LivestockReport * self, u32 flags)
{
    DestroyFarmReportPanel(self, flags);
}

EC SECTION(".text.farm_report_callbacks.080E1DB0") void DestroyLivestockReport(LivestockReport * self, u32 flags)
{
    DestroyFarmReportPanel(self, flags);
}

EC SECTION(".text.farm_report_callbacks.080E1DBC") bool DrawFarmOverviewReport(FarmOverviewReport * self)
{
    DrawFarmReportPanel(self);
    return true;
}

EC SECTION(".text.farm_report_callbacks.080E1DC8") void DestroyFarmOverviewReport(FarmOverviewReport * self, u32 flags)
{
    DestroyFarmReportPanel(self, flags);
}

struct HardwareContext;

struct LivestockReportLabelDimensions
{
    i16 width;
    i16 height;
};

struct LivestockReportInsertion
{
    u32 occupied;
    u32 inserting;
};

struct LivestockReportInitFrame
{
    char const * outgoing_text;
    DmaCommand copy;
    LivestockReportLabelDimensions healthy_fill;
    LivestockReportLabelDimensions healthy_dimensions;
    STRUCT_PAD(0x1C, 0x24);
    LivestockReportLabelDimensions unhappy_dimensions;
    LivestockReportInsertion unhappy_upload;
    LivestockReportLabelDimensions sick_fill;
    LivestockReportLabelDimensions sick_dimensions;
    LivestockReportInsertion sick_upload;
    LivestockReportLabelDimensions spring_dimensions;
    LivestockReportInsertion spring_upload;
    LivestockReportLabelDimensions summer_dimensions;
    LivestockReportInsertion summer_upload;
    LivestockReportLabelDimensions autumn_dimensions;
    LivestockReportInsertion autumn_upload;
    LivestockReportLabelDimensions winter_dimensions;
    LivestockReportInsertion winter_upload;
    LivestockReportLabelDimensions year_suffix_dimensions;
    LivestockReportInsertion year_suffix_upload;
    LivestockReportLabelDimensions birthday_unit_dimensions;
    LivestockReportInsertion birthday_unit_upload;
    LivestockReportInsertion trophy_tiles_upload;
    LivestockReportInsertion trophy_palette_upload;
    LivestockReportInsertion heart_tiles_upload;
    LivestockReportInsertion heart_palette_upload;
    LivestockReport * report;
    LivestockReportLabelDimensions * winter_bounds;
    LivestockReportLabelDimensions * year_suffix_bounds;
    LivestockReportLabelDimensions * birthday_unit_bounds;
    u8 * year_suffix_pixels;
    u8 * autumn_pixels;
    u8 * birthday_unit_pixels;
    u8 * spring_pixels;
    u8 * sick_pixels;
    u8 * winter_pixels;
    u8 * summer_pixels;
    LivestockReportLabelDimensions * unhappy_bounds;
    LivestockReportLabelDimensions * sick_fill_bounds;
    LivestockReportLabelDimensions * sick_bounds;
    LivestockReportLabelDimensions * spring_bounds;
    LivestockReportLabelDimensions * summer_bounds;
    LivestockReportLabelDimensions * autumn_bounds;
    DmaCommand * old_end;
    DmaCommand * new_begin;
};

typedef char LivestockReportInitLayoutCheck[
    offsetof(LivestockReport, vtable) == 0x5B4 &&
    offsetof(LivestockReportInitFrame, copy) == 4 &&
    offsetof(LivestockReportInitFrame, report) == 0xA8 &&
    offsetof(LivestockReportInitFrame, new_begin) == 0xF0 &&
    sizeof(LivestockReportInitFrame) == 0xF4 ? 1 : -1];

EC DmaTransferQueue * HardwareContext_GetDmaQueue(HardwareContext *);
EC void * RetryLivestockReportUploadAllocation(u32 bytes) asm("func_080D3BC0");

#define INIT_CLOBBERS "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "lr", "cc", "memory"
#define INIT_PIN(value) asm volatile ("" : "+l" (value))

#define LIVESTOCK_INIT_PANEL() do { \
    register LivestockReport * self asm("r0"); \
    asm ("" : "=l" (self)); \
    asm volatile ("str %0, [sp, %1]\n\tldr %0, [sp, %2]\n\tstr %0, [sp]\n\tldr %0, [sp, %1]" \
        : "+l" (self) : "i" (offsetof(LivestockReportInitFrame, report)), \
        "i" (sizeof(LivestockReportInitFrame) + 8 * sizeof(u32)) : "memory"); \
    asm volatile ("bl func_08076BFC" : : : "r0", "r1", "r2", "r3", "lr", "cc", "memory"); \
} while (0)

#define LIVESTOCK_SET_VTABLE() do { \
    register LivestockReport * self asm("r0"); \
    register u32 vtable_offset asm("r2"); \
    asm volatile ("ldr %0, [sp, %2]\n\tldr %1, .L08062FA4" \
        : "=l" (self), "=l" (vtable_offset) : "i" (offsetof(LivestockReportInitFrame, report)) : "memory"); \
    register void const ** slot asm("r1"); \
    asm ("add %0, %1, %2" : "=l" (slot) : "l" (self), "l" (vtable_offset)); \
    register void const * vtable asm("r0"); \
    asm volatile ("ldr %0, .L08062FA8" : "=l" (vtable)); \
    *slot = vtable; \
} while (0)

#define LIVESTOCK_GET_QUEUE() do { \
    register HardwareContext * hardware asm("r0"); \
    asm ("" : "=l" (hardware)); \
    register DmaTransferQueue * queue asm("r0") = HardwareContext_GetDmaQueue(hardware); \
    INIT_PIN(queue); \
} while (0)

#define LIVESTOCK_INIT_COPY() do { \
    register DmaCommand * command asm("r0"); \
    register void const * source asm("r1"); \
    register void * dest asm("r2"); \
    register u32 bytes asm("r3"); \
    asm volatile ("" : "=l" (command), "=l" (source), "=l" (dest), "=l" (bytes)); \
    DmaCommand_InitializeCopy(command, source, dest, bytes); \
} while (0)

#define LIVESTOCK_DRAW_TEXT() asm volatile ("bl DrawMessageTileString" : : : "r0", "r1", "r2", "r3", "lr", "cc", "memory")
#define LIVESTOCK_FILL_TILES() asm volatile ("bl FillMessageTiles" : : : "r0", "r1", "r2", "r3", "lr", "cc", "memory")

#define LIVESTOCK_REPORT_PIXELS(field, pixels) \
    asm volatile ("ldr %0, [sp, %1]" : "=l" (pixels) \
        : "I" (offsetof(LivestockReportInitFrame, field##_pixels)) : "memory")

#define LIVESTOCK_REPORT_PIXELS_unhappy(pixels) asm volatile ("mov %0, sl" : "=l" (pixels))

#define LIVESTOCK_RENDER_TEXT(field, width, text_reg, text, bounds_reg, load_pixels) do { \
    register char const * label asm(text_reg); \
    asm volatile ("ldr %0, " text : "=l" (label)); \
    register u32 width_in_tiles asm("r1") = width; \
    INIT_PIN(width_in_tiles); \
    register u32 height_in_tiles asm("r2"); \
    if ((width) != 2) { \
        height_in_tiles = 2; \
        INIT_PIN(height_in_tiles); \
    } \
    register LivestockReportLabelDimensions * dimensions asm("r0"); \
    asm ("add %0, sp, %1" : "=l" (dimensions) : "I" (offsetof(LivestockReportInitFrame, field##_dimensions))); \
    asm volatile ("strh %1, [%0]" : : "l" (dimensions), "l" (width_in_tiles) : "memory"); \
    register LivestockReportLabelDimensions * bounds asm(bounds_reg); \
    asm ("ldr %0, [sp, %1]" : "=l" (bounds) : "i" (offsetof(LivestockReportInitFrame, field##_bounds))); \
    if ((width) == 2) { \
        asm volatile ("strh %1, [%0, #2]" : : "l" (bounds), "l" (width_in_tiles) : "memory"); \
    } else { \
        asm volatile ("strh %1, [%0, #2]" : : "l" (bounds), "l" (height_in_tiles) : "memory"); \
    } \
    register u32 packed_dimensions asm("r0"); \
    asm ("ldr %0, [sp, %1]" : "=l" (packed_dimensions) : "I" (offsetof(LivestockReportInitFrame, field##_dimensions)) : "memory"); \
    INIT_PIN(packed_dimensions); \
    asm volatile ("str %0, [sp]" : : "l" (label) : "memory"); \
    register void * pixels asm("r1"); \
    load_pixels(pixels); \
    register u32 x asm("r2") = 0; \
    INIT_PIN(x); \
    register u32 y asm("r3") = 0; \
    INIT_PIN(y); \
    LIVESTOCK_DRAW_TEXT(); \
} while (0)

#define LIVESTOCK_APPEND_COPY(command_reg, copy_registers, grow, publish) do { \
    register DmaCommand * local_copy asm(command_reg); \
    asm volatile ("add %0, sp, %1\n\tmov sb, %0" : "=l" (local_copy) \
        : "I" (offsetof(LivestockReportInitFrame, copy)) : "r9"); \
    register DmaCommand * end asm("r1"); \
    register DmaCommand * capacity asm("r0"); \
    asm volatile ("ldr %0, [r7, %2]\n\tldr %1, [r7, %3]" \
        : "=l" (end), "=l" (capacity) : "I" (offsetof(DmaTransferQueue, end)), \
        "I" (offsetof(DmaTransferQueue, capacity_end)) : "memory"); \
    asm volatile ("cmp %1, %0\n\tbeq " grow "\n\tcmp %1, #0\n\tbeq " publish "\n\t" \
        ".syntax unified\n\tadds r0, r1, #0\n\tadd r1, sp, %2\n\t" \
        "ldmia r1!, {" copy_registers "}\n\tstmia r0!, {" copy_registers "}\n\t" \
        "ldr r1, [r1]\n\tstr r1, [r0]\n\t" publish ":\n\t.syntax divided" \
        : "+l" (capacity), "+l" (end) : "I" (offsetof(LivestockReportInitFrame, copy)) \
        : "r2", "r3", "r4", "r5", "r6", "cc", "memory"); \
    register DmaCommand * next asm("r0"); \
    asm ("ldr %0, [r7, %1]" : "=l" (next) : "I" (offsetof(DmaTransferQueue, end)) : "memory"); \
    ++next; \
    INIT_PIN(next); \
    asm volatile ("str %0, [r7, %1]" : : "l" (next), "I" (offsetof(DmaTransferQueue, end)) : "memory"); \
} while (0)

#define LIVESTOCK_GROW_QUEUE(kind, site, args, reload_head) do { \
    register u32 growth_slot asm("r0"); \
    asm volatile (".syntax unified\n\tLivestockInit" #kind "Capacity " site ", " args "\n\t.syntax divided" \
        : "=l" (growth_slot) : : "r1", "r2", "r3", "r4", "r5", "r6", "r10", "memory", "cc"); \
    register u32 capacity asm("r0"); \
    asm ("ldr %0, [%1]\n\tadd %0, r4, %0" : "=l" (capacity) : "l" (growth_slot) : "memory", "cc"); \
    asm volatile ("cmp %0, #0\n\tbeq .Llivestock_init_grow_no_allocation" site : : "l" (capacity) : "cc"); \
    register u32 bytes asm("r4") = capacity * sizeof(DmaCommand); \
    INIT_PIN(bytes); \
    register u32 allocation_bytes asm("r0") = bytes; \
    INIT_PIN(allocation_bytes); \
    register void * allocation asm("r0") = malloc(allocation_bytes); \
    INIT_PIN(allocation); \
    register u32 new_capacity_bytes asm("r8"); \
    asm volatile ("mov %0, r4" : "=h" (new_capacity_bytes)); \
    if (reload_head) { \
        asm volatile ("cmp %0, #0\n\tbne .Llivestock_init_grow_reload_old_start" site : : "l" (allocation) : "cc"); \
    } else { \
        asm volatile ("cmp %0, #0\n\tbne .Llivestock_init_grow_prepare_copy" site : : "l" (allocation) : "cc"); \
    } \
    asm volatile ("mov %0, %1" : "=l" (allocation_bytes) : "h" (new_capacity_bytes)); \
    allocation = RetryLivestockReportUploadAllocation(allocation_bytes); \
    INIT_PIN(allocation); \
    if (reload_head) { \
        asm volatile (".Llivestock_init_grow_reload_old_start" site ":\n\tldr r1, [r7]" : : : "r1", "memory"); \
    } \
    asm volatile ("b .Llivestock_init_grow_prepare_copy" site); \
    asm volatile (".Llivestock_init_grow_no_allocation" site ":\n\tmov r0, #0\n\tmov r8, r0" \
        : : : "r0", "r8", "cc"); \
    asm volatile (".syntax unified\n\tLivestockInit" #kind "Publish " site ", " args "\n\t.syntax divided" \
        : : : "r0", "r1", "r2", "r3", "r4", "r5", "r6", "lr", "memory", "cc"); \
} while (0)

enum { LIVESTOCK_REPORT_TILE_BYTES = 32 };

#define LIVESTOCK_UPLOAD_LABEL(load_pixels, address, tile_count) do { \
    register u32 bytes asm("r3"); \
    if ((tile_count) > 4) { \
        enum { SHIFT = (tile_count) == 12 ? 1 : 2 }; \
        bytes = (tile_count) * LIVESTOCK_REPORT_TILE_BYTES >> SHIFT; \
        INIT_PIN(bytes); \
        bytes <<= SHIFT; \
        INIT_PIN(bytes); \
    } \
    register DmaCommand * command asm("r0"); \
    asm volatile ("add %0, sp, %1" : "=l" (command) : "I" (offsetof(LivestockReportInitFrame, copy))); \
    register void const * pixels asm("r1"); \
    load_pixels(pixels); \
    register void * dest asm("r2"); \
    asm volatile ("ldr %0, " address : "=l" (dest)); \
    if ((tile_count) == 4) { \
        bytes = (tile_count) * LIVESTOCK_REPORT_TILE_BYTES; \
        INIT_PIN(bytes); \
    } \
    DmaCommand_InitializeCopy(command, pixels, dest, bytes); \
} while (0)

#define LIVESTOCK_LABEL(name, label, tile, width, text_reg, bounds_reg, copy_reg, copy_words, next, growth_regs, load_pixels) do { \
    asm volatile (".Llivestock_init_render_" #name ":"); \
    LIVESTOCK_RENDER_TEXT(name, width, text_reg, ".Llivestock_init_label_" #name, bounds_reg, load_pixels); \
    LIVESTOCK_UPLOAD_LABEL(load_pixels, ".Llivestock_init_dest_" #name, (width) * 2); \
    LIVESTOCK_APPEND_COPY(copy_reg, copy_words, ".Llivestock_init_grow_" #name, ".Llivestock_init_publish_" #name); \
    asm volatile ("b " next "\n\t.align 2, 0\n\t.Llivestock_init_label_" #name ":\n\t.word " #label "\n\t" \
        ".Llivestock_init_dest_" #name ":\n\t.word " tile "\n\t.Llivestock_init_grow_" #name ":"); \
    LIVESTOCK_GROW_QUEUE(Label, "_" #name, #name "_insertion, " growth_regs, true); \
} while (0)

#define LIVESTOCK_UPLOAD_ICON(name, next, asset, address, tile_count, copy_reg, copy_words, growth_slot) do { \
    asm volatile (".Llivestock_init_upload_" #name ":"); \
    register void const * source asm("r1"); \
    asm volatile ("ldr %0, .Llivestock_init_asset_" #name : "=l" (source)); \
    register DmaCommand * command asm("r0"); \
    asm volatile ("add %0, sp, %1" : "=l" (command) : "I" (offsetof(LivestockReportInitFrame, copy))); \
    register void * dest asm("r2"); \
    asm volatile ("ldr %0, .Llivestock_init_dest_" #name : "=l" (dest)); \
    register u32 bytes asm("r3") = (tile_count) * LIVESTOCK_REPORT_TILE_BYTES; \
    INIT_PIN(bytes); \
    DmaCommand_InitializeCopy(command, source, dest, bytes); \
    LIVESTOCK_APPEND_COPY(copy_reg, copy_words, ".Llivestock_init_grow_" #name, ".Llivestock_init_publish_" #name); \
    asm volatile ("b " next "\n\t.align 2, 0\n\t.Llivestock_init_asset_" #name ":\n\t.word " #asset "\n\t" \
        ".Llivestock_init_dest_" #name ":\n\t.word " address "\n\t.Llivestock_init_grow_" #name ":"); \
    LIVESTOCK_GROW_QUEUE(Icon, "_" #name, #growth_slot, false); \
} while (0)

#define LOAD_unhappy(pixels) LIVESTOCK_REPORT_PIXELS_unhappy(pixels)
#define LOAD_sick(pixels) LIVESTOCK_REPORT_PIXELS(sick, pixels)
#define LOAD_spring(pixels) LIVESTOCK_REPORT_PIXELS(spring, pixels)
#define LOAD_summer(pixels) LIVESTOCK_REPORT_PIXELS(summer, pixels)
#define LOAD_autumn(pixels) LIVESTOCK_REPORT_PIXELS(autumn, pixels)
#define LOAD_winter(pixels) LIVESTOCK_REPORT_PIXELS(winter, pixels)
#define LOAD_year_suffix(pixels) LIVESTOCK_REPORT_PIXELS(year_suffix, pixels)
#define LOAD_birthday_unit(pixels) LIVESTOCK_REPORT_PIXELS(birthday_unit, pixels)

asm (
        "\t.equ .Llivestock_trophy_tile, 0xA0\n"
        "\t.equ .Llivestock_heart_tile, 0xA4\n"
        "\t.equ .Llivestock_year_tile, 0x98\n"
        "\t.equ .Llivestock_day_tile, 0x9C\n"
        "\t.equ .Llivestock_first_season_tile, 0x68\n"
        "\t.equ .Llivestock_init_stack_bytes, 0xf4\n"
        "\t.equ .Llivestock_init_sp_title_arg, (.Llivestock_init_stack_bytes + 0x20)\n"
        "\t.equ .Llivestock_init_sp_dma_command, 4\n"
        "\t.equ .Llivestock_init_sp_report, 0xa8\n"
        "\t.equ .Llivestock_init_sp_old_dma_end, 0xec\n"
        "\t.equ .Llivestock_init_sp_dma_buffer, 0xf0\n"
        "\t.equ .Llivestock_init_healthy_bitmap, 0x5BC\n"
        "\t.equ .Llivestock_init_sick_bitmap, 0x9C4\n"
        "\t.equ .Llivestock_init_spring_bitmap, 0xCD0\n"
        "\t.equ .Llivestock_init_summer_bitmap, 0xE54\n"
        "\t.equ .Llivestock_init_autumn_bitmap, 0xFD8\n"
        "\t.equ .Llivestock_init_winter_bitmap, 0x115C\n"
        "\t.equ .Llivestock_init_years_bitmap, 0xBC8\n"
        "\t.equ .Llivestock_init_birthday_unit_bitmap, 0xC4C\n"
        "\t.equ .Llivestock_init_unhappy_bitmap, 0x7C0\n"
        "\t.equ .Llivestock_init_bg_char_base, 0x06000000\n"
        "\t.equ .Llivestock_init_tile_size, 32\n"
        "\n"
        "\t@ Retire the old DMA commands and publish the new buffer.\n"
        "\t.macro CommitLivestockInitDmaBuffer new_begin\n"
        "\tldr r2, [r7, #4]\n"
        "\tldr r1, [r7]\n"
        "\tadds r0, r1, #0\n"
        "\tcmp r1, r2\n"
        "\tbeq .Llivestock_init_free_old_buffer\\@\n"
        ".Llivestock_init_retire_old_buffer\\@:\n"
        "\tadds r0, #0x10\n"
        "\tcmp r0, r2\n"
        "\tbne .Llivestock_init_retire_old_buffer\\@\n"
        ".Llivestock_init_free_old_buffer\\@:\n"
        "\tcmp r1, #0\n"
        "\tbeq .Llivestock_init_publish_new_buffer\\@\n"
        "\tadds r0, r1, #0\n"
        "\tbl free\n"
        ".Llivestock_init_publish_new_buffer\\@:\n"
        "\tldr r0, [sp, #.Llivestock_init_sp_dma_buffer]\n"
        "\tadd r0, r8\n"
        "\tldr \\new_begin, [sp, #.Llivestock_init_sp_dma_buffer]\n"
        "\tstr \\new_begin, [r7]\n"
        "\tstr r5, [r7, #4]\n"
        "\tstr r0, [r7, #0xc]\n"
        "\t.endm\n"
        "\n"
        "\t@ Insert one label copy command, growing the queue by max(size, 1).\n"
        "\t@ r7 = DMA queue, r1 = old end, sb = new command.\n"
        "\t.macro LivestockInitLabelCapacity site, occupied_slot, end_tmp, copy_last, new_begin\n"
        "\tmov sl, r1\n"
        "\tmovs r0, #1\n"
        "\tstr r0, [sp, #(\\occupied_slot + 4)]\n"
        "\tldr r1, [r7]\n"
        "\tmov \\end_tmp, sl\n"
        "\tsubs r0, \\end_tmp, r1\n"
        "\tasrs r4, r0, #4\n"
        "\tstr r4, [sp, #\\occupied_slot]\n"
        "\tadd r2, sp, #(\\occupied_slot + 4)\n"
        "\tadd r0, sp, #\\occupied_slot\n"
        "\tcmp r4, #1\n"
        "\tbhs .Llivestock_init_grow_use_larger_count\\site\n"
        "\tadds r0, r2, #0\n"
        ".Llivestock_init_grow_use_larger_count\\site:\n"
        "\t.endm\n"
        "\t.macro LivestockInitLabelPublish site, occupied_slot, end_tmp, copy_last, new_begin\n"
        ".Llivestock_init_grow_prepare_copy\\site:\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Llivestock_init_sp_dma_buffer]\n"
        "\tadds r2, r1, #0\n"
        "\tadds r3, r5, #0\n"
        "\tcmp r2, sl\n"
        "\tbeq .Llivestock_init_grow_insert\\site\n"
        ".Llivestock_init_grow_copy_existing\\site:\n"
        "\tcmp r3, #0\n"
        "\tbeq .Llivestock_init_grow_next_existing\\site\n"
        "\tadds r0, r3, #0\n"
        "\tadds r1, r2, #0\n"
        "\tldm r1!, {r4, r5, r6}\n"
        "\tstm r0!, {r4, r5, r6}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".Llivestock_init_grow_next_existing\\site:\n"
        "\tadds r2, #0x10\n"
        "\tadds r3, #0x10\n"
        "\tcmp r2, sl\n"
        "\tbne .Llivestock_init_grow_copy_existing\\site\n"
        ".Llivestock_init_grow_insert\\site:\n"
        "\tadds r5, r3, #0\n"
        "\tldr r0, [sp, #(\\occupied_slot + 4)]\n"
        "\tcmp r0, #1\n"
        "\tbne .Llivestock_init_grow_insert_many\\site\n"
        "\tcmp r5, #0\n"
        "\tbeq .Llivestock_init_grow_skip_single_copy\\site\n"
        "\tadds r0, r5, #0\n"
        "\tmov r1, sb\n"
        "\tldm r1!, {r2, r3, \\copy_last}\n"
        "\tstm r0!, {r2, r3, \\copy_last}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".Llivestock_init_grow_skip_single_copy\\site:\n"
        "\tadds r5, #0x10\n"
        "\tb .Llivestock_init_grow_publish\\site\n"
        ".Llivestock_init_grow_insert_many\\site:\n"
        "\tadds r3, r0, #0\n"
        "\tadds r2, r5, #0\n"
        "\tcmp r3, #0\n"
        "\tbeq .Llivestock_init_grow_insert_complete\\site\n"
        ".Llivestock_init_grow_copy_inserted\\site:\n"
        "\tcmp r2, #0\n"
        "\tbeq .Llivestock_init_grow_next_inserted\\site\n"
        "\tadds r0, r2, #0\n"
        "\tmov r1, sb\n"
        "\tldm r1!, {r4, r5, r6}\n"
        "\tstm r0!, {r4, r5, r6}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".Llivestock_init_grow_next_inserted\\site:\n"
        "\tsubs r3, #1\n"
        "\tadds r2, #0x10\n"
        "\tcmp r3, #0\n"
        "\tbne .Llivestock_init_grow_copy_inserted\\site\n"
        ".Llivestock_init_grow_insert_complete\\site:\n"
        "\tadds r5, r2, #0\n"
        ".Llivestock_init_grow_publish\\site:\n"
        "\tCommitLivestockInitDmaBuffer \\new_begin\n"
        "\t.endm\n"
        "\n"
        "\t@ Same growth for icon uploads; the old end and insert count use stack slots.\n"
        "\t.macro LivestockInitIconCapacity site, occupied_slot\n"
        "\tstr r1, [sp, #.Llivestock_init_sp_old_dma_end]\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #(\\occupied_slot + 4)\n"
        "\tstr r0, [r1]\n"
        "\tldr r0, [r7]\n"
        "\tldr r6, [sp, #.Llivestock_init_sp_old_dma_end]\n"
        "\tsubs r0, r6, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #\\occupied_slot\n"
        "\tstr r4, [r0]\n"
        "\tldr r2, [r1]\n"
        "\tmov sl, r1\n"
        "\tcmp r4, r2\n"
        "\tbhs .Llivestock_init_grow_use_larger_count\\site\n"
        "\tmov r0, sl\n"
        ".Llivestock_init_grow_use_larger_count\\site:\n"
        "\t.endm\n"
        "\t.macro LivestockInitIconPublish site, occupied_slot\n"
        ".Llivestock_init_grow_prepare_copy\\site:\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Llivestock_init_sp_dma_buffer]\n"
        "\tldr r2, [r7]\n"
        "\tadds r3, r5, #0\n"
        "\tb .Llivestock_init_grow_check_existing_end\\site\n"
        ".Llivestock_init_grow_copy_existing\\site:\n"
        "\tcmp r3, #0\n"
        "\tbeq .Llivestock_init_grow_next_existing\\site\n"
        "\tadds r0, r3, #0\n"
        "\tadds r1, r2, #0\n"
        "\tldm r1!, {r4, r5, r6}\n"
        "\tstm r0!, {r4, r5, r6}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".Llivestock_init_grow_next_existing\\site:\n"
        "\tadds r2, #0x10\n"
        "\tadds r3, #0x10\n"
        ".Llivestock_init_grow_check_existing_end\\site:\n"
        "\tldr r0, [sp, #.Llivestock_init_sp_old_dma_end]\n"
        "\tcmp r2, r0\n"
        "\tbne .Llivestock_init_grow_copy_existing\\site\n"
        "\tadds r5, r3, #0\n"
        "\tmov r2, sl\n"
        "\tldr r1, [r2]\n"
        "\tcmp r1, #1\n"
        "\tbne .Llivestock_init_grow_insert_many\\site\n"
        "\tcmp r5, #0\n"
        "\tbeq .Llivestock_init_grow_skip_single_copy\\site\n"
        "\tadds r0, r5, #0\n"
        "\tmov r1, sb\n"
        "\tldm r1!, {r3, r4, r6}\n"
        "\tstm r0!, {r3, r4, r6}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".Llivestock_init_grow_skip_single_copy\\site:\n"
        "\tadds r5, #0x10\n"
        "\tb .Llivestock_init_grow_publish\\site\n"
        ".Llivestock_init_grow_insert_many\\site:\n"
        "\tadds r3, r1, #0\n"
        "\tadds r2, r5, #0\n"
        "\tcmp r3, #0\n"
        "\tbeq .Llivestock_init_grow_insert_complete\\site\n"
        ".Llivestock_init_grow_copy_inserted\\site:\n"
        "\tcmp r2, #0\n"
        "\tbeq .Llivestock_init_grow_next_inserted\\site\n"
        "\tadds r0, r2, #0\n"
        "\tmov r1, sb\n"
        "\tldm r1!, {r4, r5, r6}\n"
        "\tstm r0!, {r4, r5, r6}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".Llivestock_init_grow_next_inserted\\site:\n"
        "\tsubs r3, #1\n"
        "\tadds r2, #0x10\n"
        "\tcmp r3, #0\n"
        "\tbne .Llivestock_init_grow_copy_inserted\\site\n"
        ".Llivestock_init_grow_insert_complete\\site:\n"
        "\tadds r5, r2, #0\n"
        ".Llivestock_init_grow_publish\\site:\n"
        "\tCommitLivestockInitDmaBuffer r1\n"
        "\t.endm\n"
        "\n"
        "\t.equ unhappy_insertion, 0x28\n"
        "\t.equ spring_insertion, 0x44\n"
        "\t.equ summer_insertion, 0x50\n"
        "\t.equ autumn_insertion, 0x5c\n"
        "\t.equ winter_insertion, 0x68\n"
        "\t.equ year_suffix_insertion, 0x74\n"
);

// Fixed Thumb registers keep the inline queue branches and their literal pools at the original offsets.
EC SECTION(".text.f3.08062E9C") __attribute__((naked)) void InitLivestockReport(
    LivestockReport *, void *, void *, u32, char const *)
{
    asm volatile (
        ".syntax unified\n"
        "\tpush {r4, r5, r6, r7, lr}\n"
        "\tmov r7, sl\n"
        "\tmov r6, sb\n"
        "\tmov r5, r8\n"
        "\tpush {r5, r6, r7}\n"
        "\tsub sp, #.Llivestock_init_stack_bytes\n"
        ".syntax divided\n" : : : INIT_CLOBBERS);

    LIVESTOCK_INIT_PANEL();

    LIVESTOCK_SET_VTABLE();

    asm volatile (
        ".syntax unified\n"
        "\tmovs r0, #0xf\n"
        "\tadd r4, sp, #0x14\n"
        "\tadd r3, sp, #0x18\n"
        "\tmov r8, r3\n"
        "\tmov r5, sp\n"
        "\tadds r5, #0x24\n"
        "\tstr r5, [sp, #0xd4]\n"
        "\tmov r6, sp\n"
        "\tadds r6, #0x30\n"
        "\tstr r6, [sp, #0xd8]\n"
        "\tmov r1, sp\n"
        "\tadds r1, #0x34\n"
        "\tstr r1, [sp, #0xdc]\n"
        "\tmov r2, sp\n"
        "\tadds r2, #0x40\n"
        "\tstr r2, [sp, #0xe0]\n"
        "\tmov r3, sp\n"
        "\tadds r3, #0x4c\n"
        "\tstr r3, [sp, #0xe4]\n"
        "\tadds r5, #0x34\n"
        "\tstr r5, [sp, #0xe8]\n"
        "\tadds r6, #0x34\n"
        "\tstr r6, [sp, #0xac]\n"
        "\tadds r1, #0x3c\n"
        "\tstr r1, [sp, #0xb0]\n"
        "\tadds r2, #0x3c\n"
        "\tstr r2, [sp, #0xb4]\n"
        "\tmovs r1, #1\n"
        "\trsbs r1, r1, #0\n"
        ".Llivestock_init_countdown:\n"
        "\tsubs r0, #1\n"
        "\tcmp r0, r1\n"
        "\tbne .Llivestock_init_countdown\n"
        "\tldr r3, [sp, #.Llivestock_init_sp_report]\n"
        "\tldr r0, [r3]\n"
        ".syntax divided\n" : : : INIT_CLOBBERS);

    LIVESTOCK_GET_QUEUE();

    asm volatile (
        ".syntax unified\n"
        "\tadds r7, r0, #0\n"
        "\tmovs r5, #8\n"
        "\tmovs r6, #2\n"
        "\tadd r0, sp, #0x14\n"
        "\tstrh r5, [r0]\n"
        "\tstrh r6, [r4, #2]\n"
        "\tldr r0, [sp, #0x14]\n"
        "\tldr r1, [sp, #.Llivestock_init_sp_report]\n"
        "\tldr r2, .L08062FAC\n"
        "\tadds r4, r1, r2\n"
        "\tadds r1, r4, #0\n"
        "\tmovs r2, #0\n"
        ".syntax divided\n" : : : INIT_CLOBBERS);

    LIVESTOCK_FILL_TILES();

    asm volatile (
        ".syntax unified\n"
        "\tldr r1, .L08062FB0\n"
        "\tadd r0, sp, #0x18\n"
        "\tstrh r5, [r0]\n"
        "\tmov r3, r8\n"
        "\tstrh r6, [r3, #2]\n"
        "\tldr r0, [sp, #0x18]\n"
        "\tstr r1, [sp]\n"
        "\tadds r1, r4, #0\n"
        "\tmovs r2, #0\n"
        "\tmovs r3, #0\n"
        ".syntax divided\n" : : : INIT_CLOBBERS);

    LIVESTOCK_DRAW_TEXT();

    asm volatile (
        ".syntax unified\n"
        "\tmovs r3, #0x80\n"
        "\tlsls r3, r3, #2\n"
        "\tadd r0, sp, #.Llivestock_init_sp_dma_command\n"
        "\tadds r1, r4, #0\n"
        "\tldr r2, .L08062FB4\n"
        ".syntax divided\n" : : : INIT_CLOBBERS);

    LIVESTOCK_INIT_COPY();

    LIVESTOCK_APPEND_COPY("r4", "r2, r5, r6", ".Llivestock_init_grow_healthy_label", ".Llivestock_init_finish_healthy_fast_append");

    asm volatile (
        ".syntax unified\n"
        "\tldr r3, [sp, #.Llivestock_init_sp_report]\n"
        "\tmovs r4, #(.Llivestock_init_unhappy_bitmap >> 3)\n"
        "\tlsls r4, r4, #3\n"
        "\tadds r3, r3, r4\n"
        "\tmov sl, r3\n"
        "\tldr r6, .L08062FB8\n"
        "\tldr r5, [sp, #.Llivestock_init_sp_report]\n"
        "\tadds r6, r5, r6\n"
        "\tstr r6, [sp, #0xc8]\n"
        "\tmovs r0, #(.Llivestock_init_spring_bitmap >> 4)\n"
        "\tlsls r0, r0, #4\n"
        "\tadds r0, r5, r0\n"
        "\tstr r0, [sp, #0xc4]\n"
        "\tldr r1, .L08062FBC\n"
        "\tadds r1, r5, r1\n"
        "\tstr r1, [sp, #0xd0]\n"
        "\tldr r2, .L08062FC0\n"
        "\tadds r2, r5, r2\n"
        "\tstr r2, [sp, #0xbc]\n"
        "\tldr r3, .L08062FC4\n"
        "\tadds r3, r5, r3\n"
        "\tstr r3, [sp, #0xcc]\n"
        "\tldr r4, .L08062FC8\n"
        "\tadds r4, r5, r4\n"
        "\tstr r4, [sp, #0xb8]\n"
        "\tldr r6, .L08062FCC\n"
        "\tadds r6, r5, r6\n"
        "\tstr r6, [sp, #0xc0]\n"
        "\tb .Llivestock_init_render_unhappy\n"
        "\t.align 2, 0\n"
        ".L08062FA4: .4byte 0x000005B4\n"
        ".L08062FA8: .4byte gLivestockReportVTable\n"
        ".L08062FAC: .4byte .Llivestock_init_healthy_bitmap\n"
        ".L08062FB0: .4byte gLivestockReportHealthyLabel\n"
        ".L08062FB4: .4byte (.Llivestock_init_bg_char_base + 0x38 * .Llivestock_init_tile_size)\n"
        ".L08062FB8: .4byte .Llivestock_init_sick_bitmap\n"
        ".L08062FBC: .4byte .Llivestock_init_summer_bitmap\n"
        ".L08062FC0: .4byte .Llivestock_init_autumn_bitmap\n"
        ".L08062FC4: .4byte .Llivestock_init_winter_bitmap\n"
        ".L08062FC8: .4byte .Llivestock_init_years_bitmap\n"
        ".L08062FCC: .4byte .Llivestock_init_birthday_unit_bitmap\n"
        ".Llivestock_init_grow_healthy_label:\n"
        "\tstr r1, [sp, #.Llivestock_init_sp_old_dma_end]\n"
        "\tmovs r0, #1\n"
        "\tstr r0, [sp, #0x20]\n"
        "\tldr r1, [r7]\n"
        "\tldr r2, [sp, #.Llivestock_init_sp_old_dma_end]\n"
        "\tsubs r0, r2, r1\n"
        "\tasrs r4, r0, #4\n"
        "\tstr r4, [sp, #0x1c]\n"
        "\tadd r2, sp, #0x20\n"
        "\tadd r0, sp, #0x1c\n"
        "\tcmp r4, #1\n"
        "\tbhs .L08062FEA\n"
        "\tadds r0, r2, #0\n"
        ".L08062FEA:\n"
        "\tldr r0, [r0]\n"
        "\tadds r0, r4, r0\n"
        "\tcmp r0, #0\n"
        "\tbeq .L0806300A\n"
        "\tlsls r4, r0, #4\n"
        "\tadds r0, r4, #0\n"
        "\tbl malloc\n"
        "\tmov r8, r4\n"
        "\tcmp r0, #0\n"
        "\tbne .L08063006\n"
        "\tmov r0, r8\n"
        "\tbl func_080D3BC0\n"
        ".L08063006:\n"
        "\tldr r1, [r7]\n"
        "\tb .L0806300E\n"
        ".L0806300A:\n"
        "\tmovs r0, #0\n"
        "\tmov r8, r0\n"
        ".L0806300E:\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Llivestock_init_sp_dma_buffer]\n"
        "\tadds r2, r1, #0\n"
        "\tadds r3, r5, #0\n"
        "\tldr r4, [sp, #.Llivestock_init_sp_report]\n"
        "\tmovs r5, #(.Llivestock_init_unhappy_bitmap >> 3)\n"
        "\tlsls r5, r5, #3\n"
        "\tadds r4, r4, r5\n"
        "\tmov sl, r4\n"
        "\tldr r0, .L0806308C\n"
        "\tldr r6, [sp, #.Llivestock_init_sp_report]\n"
        "\tadds r0, r6, r0\n"
        "\tstr r0, [sp, #0xc8]\n"
        "\tmovs r1, #(.Llivestock_init_spring_bitmap >> 4)\n"
        "\tlsls r1, r1, #4\n"
        "\tadds r1, r6, r1\n"
        "\tstr r1, [sp, #0xc4]\n"
        "\tldr r4, .L08063090\n"
        "\tadds r4, r6, r4\n"
        "\tstr r4, [sp, #0xd0]\n"
        "\tldr r5, .L08063094\n"
        "\tadds r5, r6, r5\n"
        "\tstr r5, [sp, #0xbc]\n"
        "\tldr r0, .L08063098\n"
        "\tadds r0, r6, r0\n"
        "\tstr r0, [sp, #0xcc]\n"
        "\tldr r1, .L0806309C\n"
        "\tadds r1, r6, r1\n"
        "\tstr r1, [sp, #0xb8]\n"
        "\tldr r4, .L080630A0\n"
        "\tadds r4, r6, r4\n"
        "\tstr r4, [sp, #0xc0]\n"
        "\tldr r5, [sp, #.Llivestock_init_sp_old_dma_end]\n"
        "\tcmp r2, r5\n"
        "\tbeq .L0806306E\n"
        ".L08063054:\n"
        "\tcmp r3, #0\n"
        "\tbeq .L08063064\n"
        "\tadds r0, r3, #0\n"
        "\tadds r1, r2, #0\n"
        "\tldm r1!, {r4, r5, r6}\n"
        "\tstm r0!, {r4, r5, r6}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".L08063064:\n"
        "\tadds r2, #0x10\n"
        "\tadds r3, #0x10\n"
        "\tldr r6, [sp, #.Llivestock_init_sp_old_dma_end]\n"
        "\tcmp r2, r6\n"
        "\tbne .L08063054\n"
        ".L0806306E:\n"
        "\tadds r5, r3, #0\n"
        "\tldr r0, [sp, #0x20]\n"
        "\tcmp r0, #1\n"
        "\tbne .L080630A4\n"
        "\tcmp r5, #0\n"
        "\tbeq .L08063086\n"
        "\tadds r0, r5, #0\n"
        "\tmov r1, sb\n"
        "\tldm r1!, {r2, r3, r4}\n"
        "\tstm r0!, {r2, r3, r4}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".L08063086:\n"
        "\tadds r5, #0x10\n"
        "\tb .L080630C6\n"
        "\t.align 2, 0\n"
        ".L0806308C: .4byte .Llivestock_init_sick_bitmap\n"
        ".L08063090: .4byte .Llivestock_init_summer_bitmap\n"
        ".L08063094: .4byte .Llivestock_init_autumn_bitmap\n"
        ".L08063098: .4byte .Llivestock_init_winter_bitmap\n"
        ".L0806309C: .4byte .Llivestock_init_years_bitmap\n"
        ".L080630A0: .4byte .Llivestock_init_birthday_unit_bitmap\n"
        ".L080630A4:\n"
        "\tadds r3, r0, #0\n"
        "\tadds r2, r5, #0\n"
        "\tcmp r3, #0\n"
        "\tbeq .L080630C4\n"
        ".L080630AC:\n"
        "\tcmp r2, #0\n"
        "\tbeq .L080630BC\n"
        "\tadds r0, r2, #0\n"
        "\tmov r1, sb\n"
        "\tldm r1!, {r4, r5, r6}\n"
        "\tstm r0!, {r4, r5, r6}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".L080630BC:\n"
        "\tsubs r3, #1\n"
        "\tadds r2, #0x10\n"
        "\tcmp r3, #0\n"
        "\tbne .L080630AC\n"
        ".L080630C4:\n"
        "\tadds r5, r2, #0\n"
        ".L080630C6:\n"
        "\tCommitLivestockInitDmaBuffer r6\n"
        ".syntax divided\n" : : : INIT_CLOBBERS);

    LIVESTOCK_LABEL(unhappy, gLivestockReportUnhappyLabel, "(.Llivestock_init_bg_char_base + 0x48 * .Llivestock_init_tile_size)", 8,
        "r3", "r0", "r1", "r2, r3, r4", ".Llivestock_init_render_sick_label", "r5, r6, r1", LOAD_unhappy);

    asm volatile (
        ".syntax unified\n"
        ".Llivestock_init_render_sick_label:\n"
        "\tmovs r4, #8\n"
        "\tmovs r5, #2\n"
        "\tadd r0, sp, #0x30\n"
        "\tstrh r4, [r0]\n"
        "\tldr r2, [sp, #0xd8]\n"
        "\tstrh r5, [r2, #2]\n"
        "\tldr r0, [sp, #0x30]\n"
        "\tldr r1, [sp, #0xc8]\n"
        "\tmovs r2, #0\n"
        ".syntax divided\n" : : : INIT_CLOBBERS);

    LIVESTOCK_FILL_TILES();

    asm volatile (
        ".syntax unified\n"
        "\tldr r1, .L0806326C\n"
        "\tadd r0, sp, #0x34\n"
        "\tstrh r4, [r0]\n"
        "\tldr r3, [sp, #0xdc]\n"
        "\tstrh r5, [r3, #2]\n"
        "\tldr r0, [sp, #0x34]\n"
        "\tstr r1, [sp]\n"
        "\tldr r1, [sp, #0xc8]\n"
        "\tmovs r2, #0\n"
        "\tmovs r3, #0\n"
        ".syntax divided\n" : : : INIT_CLOBBERS);

    LIVESTOCK_DRAW_TEXT();

    LIVESTOCK_UPLOAD_LABEL(LOAD_sick, ".L08063270", 16);

    LIVESTOCK_APPEND_COPY("r4", "r2, r5, r6", ".Llivestock_init_grow_sick_label", ".L08063262");

    asm volatile (
        ".syntax unified\n"
        "\tb .Llivestock_init_render_spring\n"
        "\t.align 2, 0\n"
        ".L0806326C: .4byte gLivestockReportSickLabel\n"
        ".L08063270: .4byte (.Llivestock_init_bg_char_base + 0x58 * .Llivestock_init_tile_size)\n"
        ".Llivestock_init_grow_sick_label:\n"
        ".syntax divided\n" : : : INIT_CLOBBERS);

    LIVESTOCK_GROW_QUEUE(Label, "_site2", "0x38, r3, r4, r6", true);

    LIVESTOCK_LABEL(spring, gLivestockReportSpringLabel, "(.Llivestock_init_bg_char_base + .Llivestock_first_season_tile * .Llivestock_init_tile_size)", 6,
        "r3", "r0", "r1", "r2, r3, r4", ".Llivestock_init_render_summer", "r5, r6, r1", LOAD_spring);

    LIVESTOCK_LABEL(summer, gLivestockReportSummerLabel, "(.Llivestock_init_bg_char_base + (.Llivestock_first_season_tile + 12) * .Llivestock_init_tile_size)", 6,
        "r3", "r4", "r5", "r2, r3, r6", ".Llivestock_init_render_autumn", "r4, r6, r1", LOAD_summer);

    LIVESTOCK_LABEL(autumn, gLivestockReportFallLabel, "(.Llivestock_init_bg_char_base + (.Llivestock_first_season_tile + 24) * .Llivestock_init_tile_size)", 6,
        "r3", "r4", "r5", "r2, r3, r6", ".Llivestock_init_render_winter", "r4, r6, r1", LOAD_autumn);

    LIVESTOCK_LABEL(winter, gLivestockReportWinterLabel, "(.Llivestock_init_bg_char_base + (.Llivestock_first_season_tile + 36) * .Llivestock_init_tile_size)", 6,
        "r3", "r4", "r5", "r2, r3, r6", ".Llivestock_init_render_year_suffix", "r4, r6, r1", LOAD_winter);

    LIVESTOCK_LABEL(year_suffix, gLivestockReportYearAgeSuffix, "(.Llivestock_init_bg_char_base + .Llivestock_year_tile * .Llivestock_init_tile_size)", 2,
        "r2", "r3", "r4", "r2, r5, r6", ".Llivestock_init_render_birthday_unit", "r3, r4, r6", LOAD_year_suffix);

    asm volatile (
        ".syntax unified\n"
        ".Llivestock_init_render_birthday_unit:\n"
        ".syntax divided\n" : : : INIT_CLOBBERS);

    LIVESTOCK_RENDER_TEXT(birthday_unit, 2, "r2", ".L0806390C", "r0", LOAD_birthday_unit);

    LIVESTOCK_UPLOAD_LABEL(LOAD_birthday_unit, ".L08063910", 4);

    LIVESTOCK_APPEND_COPY("r1", "r2, r3, r4", ".Llivestock_init_grow_birthday_unit", ".L08063904");

    asm volatile (
        ".syntax unified\n"
        "\tb .Llivestock_init_upload_trophy_tiles\n"
        "\t.align 2, 0\n"
        ".L0806390C: .4byte gFarmReportBlankLabel\n"
        ".L08063910: .4byte (.Llivestock_init_bg_char_base + .Llivestock_day_tile * .Llivestock_init_tile_size)\n"
        ".Llivestock_init_grow_birthday_unit:\n"
        "\tstr r1, [sp, #.Llivestock_init_sp_old_dma_end]\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0x84\n"
        "\tstr r0, [r1]\n"
        "\tldr r0, [r7]\n"
        "\tldr r5, [sp, #.Llivestock_init_sp_old_dma_end]\n"
        "\tsubs r0, r5, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tstr r4, [sp, #0x80]\n"
        "\tldr r0, [r1]\n"
        "\tadd r2, sp, #0x80\n"
        "\tmov sl, r1\n"
        "\tcmp r4, r0\n"
        "\tbhs .L08063932\n"
        "\tmov r2, sl\n"
        ".L08063932:\n"
        "\tldr r0, [r2]\n"
        "\tadds r0, r4, r0\n"
        "\tcmp r0, #0\n"
        "\tbeq .L08063950\n"
        "\tlsls r4, r0, #4\n"
        "\tadds r0, r4, #0\n"
        "\tbl malloc\n"
        "\tmov r8, r4\n"
        "\tcmp r0, #0\n"
        "\tbne .L08063954\n"
        "\tmov r0, r8\n"
        "\tbl func_080D3BC0\n"
        "\tb .L08063954\n"
        ".L08063950:\n"
        "\tmovs r0, #0\n"
        "\tmov r8, r0\n"
        ".L08063954:\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Llivestock_init_sp_dma_buffer]\n"
        "\tldr r2, [r7]\n"
        "\tadds r3, r5, #0\n"
        "\tldr r6, [sp, #.Llivestock_init_sp_old_dma_end]\n"
        "\tcmp r2, r6\n"
        "\tbeq .L0806397C\n"
        ".L08063962:\n"
        "\tcmp r3, #0\n"
        "\tbeq .L08063972\n"
        "\tadds r0, r3, #0\n"
        "\tadds r1, r2, #0\n"
        "\tldm r1!, {r4, r5, r6}\n"
        "\tstm r0!, {r4, r5, r6}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".L08063972:\n"
        "\tadds r2, #0x10\n"
        "\tadds r3, #0x10\n"
        "\tldr r0, [sp, #.Llivestock_init_sp_old_dma_end]\n"
        "\tcmp r2, r0\n"
        "\tbne .L08063962\n"
        ".L0806397C:\n"
        "\tadds r5, r3, #0\n"
        "\tmov r2, sl\n"
        "\tldr r1, [r2]\n"
        "\tcmp r1, #1\n"
        "\tbne .L0806399A\n"
        "\tcmp r5, #0\n"
        "\tbeq .L08063996\n"
        "\tadds r0, r5, #0\n"
        "\tmov r1, sb\n"
        "\tldm r1!, {r3, r4, r6}\n"
        "\tstm r0!, {r3, r4, r6}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".L08063996:\n"
        "\tadds r5, #0x10\n"
        "\tb .L080639BC\n"
        ".L0806399A:\n"
        "\tadds r3, r1, #0\n"
        "\tadds r2, r5, #0\n"
        "\tcmp r3, #0\n"
        "\tbeq .L080639BA\n"
        ".L080639A2:\n"
        "\tcmp r2, #0\n"
        "\tbeq .L080639B2\n"
        "\tadds r0, r2, #0\n"
        "\tmov r1, sb\n"
        "\tldm r1!, {r4, r5, r6}\n"
        "\tstm r0!, {r4, r5, r6}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".L080639B2:\n"
        "\tsubs r3, #1\n"
        "\tadds r2, #0x10\n"
        "\tcmp r3, #0\n"
        "\tbne .L080639A2\n"
        ".L080639BA:\n"
        "\tadds r5, r2, #0\n"
        ".L080639BC:\n"
        "\tCommitLivestockInitDmaBuffer r1\n"
        ".syntax divided\n" : : : INIT_CLOBBERS);

    LIVESTOCK_UPLOAD_ICON(trophy_tiles, ".Llivestock_init_upload_trophy_palette", gUnk_0875196C,
        "(.Llivestock_init_bg_char_base + .Llivestock_trophy_tile * .Llivestock_init_tile_size)", 4, "r2", "r3, r4, r5", 0x88);

    LIVESTOCK_UPLOAD_ICON(trophy_palette, ".Llivestock_init_upload_heart_tiles", gUnk_087519EC,
        "0x05000020", 1, "r2", "r3, r4, r5", 0x90);

    LIVESTOCK_UPLOAD_ICON(heart_tiles, ".Llivestock_init_upload_heart_palette", gUnk_0875254C,
        "(.Llivestock_init_bg_char_base + .Llivestock_heart_tile * .Llivestock_init_tile_size)", 1, "r2", "r3, r4, r5", 0x98);

    LIVESTOCK_UPLOAD_ICON(heart_palette, ".Llivestock_init_return_report", gUnk_087525CC,
        "0x05000040", 1, "r2", "r3, r4, r5", 0xa0);

    asm volatile (
        ".syntax unified\n"
        ".Llivestock_init_return_report:\n"
        "\tldr r0, [sp, #.Llivestock_init_sp_report]\n"
        "\tadd sp, #.Llivestock_init_stack_bytes\n"
        "\tpop {r3, r4, r5}\n"
        "\tmov r8, r3\n"
        "\tmov sb, r4\n"
        "\tmov sl, r5\n"
        "\tpop {r4, r5, r6, r7}\n"
        "\tpop {r1}\n"
        "\tbx r1\n"
        "\n"
        "\t.purgem CommitLivestockInitDmaBuffer\n"
        ".syntax divided\n" : : : INIT_CLOBBERS);
}

#undef LOAD_unhappy
#undef LOAD_sick
#undef LOAD_spring
#undef LOAD_summer
#undef LOAD_autumn
#undef LOAD_winter
#undef LOAD_year_suffix
#undef LOAD_birthday_unit

#undef LIVESTOCK_GROW_QUEUE
#undef LIVESTOCK_UPLOAD_ICON
#undef LIVESTOCK_LABEL
#undef LIVESTOCK_UPLOAD_LABEL
#undef LIVESTOCK_APPEND_COPY
#undef LIVESTOCK_RENDER_TEXT
#undef LIVESTOCK_REPORT_PIXELS_unhappy
#undef LIVESTOCK_REPORT_PIXELS
#undef LIVESTOCK_FILL_TILES
#undef LIVESTOCK_DRAW_TEXT
#undef LIVESTOCK_INIT_COPY
#undef LIVESTOCK_GET_QUEUE
#undef LIVESTOCK_SET_VTABLE
#undef LIVESTOCK_INIT_PANEL
#undef INIT_PIN
#undef INIT_CLOBBERS
