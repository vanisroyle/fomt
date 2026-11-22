#include "prelude.h"

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
