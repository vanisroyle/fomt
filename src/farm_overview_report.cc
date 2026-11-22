#include "prelude.h"
#include "dma_queue.hh"
#include "farm.hh"
#include "utility/vec2.hh"

#include <stdlib.h>
#include <string.h>

struct HardwareContext;
struct FarmReportData;
struct FarmReportMenu;
struct FarmOverviewReport;
struct FarmOverviewPanel;

EC DmaTransferQueue * HardwareContext_GetDmaQueue(HardwareContext *);
EC void * RetryFarmOverviewAllocation(u32 bytes) asm("func_080D3BC0");

struct FarmOverviewInitFrame
{
    char const * outgoing_text;
    u32 outgoing_tilemap_stride;
    char panel_title[24];
    char property_title[24];
    DmaCommand label_copy;
    DmaCommand name_copy;
    STRUCT_PAD(0x58, 0x1C4);
    FarmOverviewReport * report;
    FarmReportMenu * menu;
    FarmOverviewPanel * panel;
    u32 portrait_row;
    u32 spouse_id;
    void const * spouse;
    void const * active_npc;
    Horse const * horse;
    STRUCT_PAD(0x1E4, 0x1F4);
    u8 * first_blank_pixels;
    u8 * second_blank_pixels;
    u8 * third_blank_pixels;
    u8 * first_wide_blank_pixels;
    u8 * second_wide_blank_pixels;
    u8 * gold_suffix_pixels;
    u8 * farmer_name_pixels;
    u32 content_rows;
    u32 portrait_lower_row;
    u32 dog_row;
    u32 portrait_y_pixels;
    u32 portrait_tilemap_offset;
    u8 * horse_name_pixels;
    DmaCommand * name_copy_pointer;
    Vec2 * first_blank_bounds;
    Vec2 * second_blank_bounds;
    Vec2 * third_blank_bounds;
    STRUCT_PAD(0x238, 0x23C);
    Vec2 * first_wide_blank_bounds;
    Vec2 * second_wide_blank_bounds;
    Vec2 * gold_suffix_bounds;
    Vec2 * farmer_name_fill_bounds;
    Vec2 * farmer_name_bounds;
    STRUCT_PAD(0x250, 0x294);
    DmaCommand * old_end;
    DmaCommand * replacement_buffer;
};

typedef char FarmOverviewInitLayoutCheck[
    sizeof(FarmOverviewInitFrame) == 0x29C &&
    offsetof(FarmOverviewInitFrame, property_title) == 0x20 &&
    offsetof(FarmOverviewInitFrame, label_copy) == 0x38 &&
    offsetof(FarmOverviewInitFrame, name_copy) == 0x48 &&
    offsetof(FarmOverviewInitFrame, report) == 0x1C4 &&
    offsetof(FarmOverviewInitFrame, panel) == 0x1CC &&
    offsetof(FarmOverviewInitFrame, old_end) == 0x294 ? 1 : -1];

enum { MAX_PROPERTY_TITLE_BYTES = 22, FARM_OVERVIEW_PANEL_HEIGHT = 64, FARM_OVERVIEW_LABEL_SIDE = 2 };
enum { REPORT_DIGIT_TILE = 0x120, REPORT_TILEMAP_STRIDE = 32 };

#define OVERVIEW_CLOBBERS "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "lr", "cc", "memory"
#define OVERVIEW_PIN(value) asm volatile ("" : "+l" (value))
#define OVERVIEW_SLOT(field) offsetof(FarmOverviewInitFrame, field)
#define OVERVIEW_LOAD(value, field) \
    asm ("ldr %0, [sp, %1]" : "=l" (value) : "i" (OVERVIEW_SLOT(field)) : "memory")
#define OVERVIEW_ADDRESS(value, field) \
    asm ("add %0, sp, %1" : "=l" (value) : "I" (OVERVIEW_SLOT(field)))
#define OVERVIEW_SET_ARG(value) asm volatile ("str %0, [sp]" : : "l" (value) : "memory")

#define OVERVIEW_BUILD_PANEL() do { \
    register Farm const * farm asm("r0"); \
    asm volatile ("mov %0, sl\n\tadd %0, #20" : "=l" (farm) : : "cc"); \
    register char const * farm_name asm("r0") = farm->GetName(); \
    register char const * name_for_copy asm("r6") = farm_name; \
    OVERVIEW_PIN(name_for_copy); \
    register char * title_buffer asm("r5"); \
    OVERVIEW_ADDRESS(title_buffer, property_title); \
    OVERVIEW_PIN(title_buffer); \
    register size_t name_length asm("r4") = strlen(farm_name); \
    OVERVIEW_PIN(name_length); \
    if (name_length > MAX_PROPERTY_TITLE_BYTES) \
        name_length = MAX_PROPERTY_TITLE_BYTES; \
    OVERVIEW_PIN(name_length); \
    memcpy(title_buffer, name_for_copy, name_length); \
    register char * name_end asm("r0"); \
    asm ("add %0, %1, %2" : "=l" (name_end) : "l" (title_buffer), "l" (name_length)); \
    asm volatile ("mov r1, #0\n\tmov sb, r1\n\tstrb r1, [%0]" : : "l" (name_end) : "r1", "r9", "cc", "memory"); \
    register char * title asm("r7") = title_buffer; \
    OVERVIEW_PIN(title); \
    register char const * suffix asm("r8"); \
    asm volatile ("ldr r2, .Loverview_property_suffix\n\tmov %0, r2" : "=h" (suffix) : : "r2"); \
    register size_t title_length asm("r0") = strlen(title); \
    OVERVIEW_PIN(title_length); \
    register char * append_at asm("r6") = title + title_length; \
    OVERVIEW_PIN(append_at); \
    register size_t remaining asm("r5"); \
    asm volatile ("mov r1, %2\n\tsub %0, r1, %1" : "=l" (remaining) : "l" (title_length), \
        "I" (MAX_PROPERTY_TITLE_BYTES) : "r1", "cc"); \
    asm volatile ("cmp %0, #0\n\tbeq .Loverview_title_ready" : : "l" (remaining) : "cc"); \
    register size_t suffix_length asm("r4") = strlen(suffix); \
    OVERVIEW_PIN(suffix_length); \
    if (suffix_length > remaining) \
        suffix_length = remaining; \
    OVERVIEW_PIN(suffix_length); \
    register char * suffix_dest asm("r0") = append_at; \
    OVERVIEW_PIN(suffix_dest); \
    memcpy(suffix_dest, suffix, suffix_length); \
    register char * suffix_end asm("r0"); \
    asm ("add %0, %1, %2" : "=l" (suffix_end) : "l" (append_at), "l" (suffix_length)); \
    asm volatile ("mov r3, sb\n\tstrb r3, [%0]\n\t.Loverview_title_ready:" : : "l" (suffix_end) : "r3", "memory"); \
    register char * title_arg asm("r0"); \
    OVERVIEW_ADDRESS(title_arg, panel_title); \
    OVERVIEW_PIN(title_arg); \
    strcpy(title_arg, title); \
    OVERVIEW_ADDRESS(title_arg, panel_title); \
    OVERVIEW_PIN(title_arg); \
    asm volatile ("add r1, %0, #0\n\tmov r2, %1\n\tbl memcpy" \
        : "+l" (title_arg) : "I" (sizeof(FarmOverviewInitFrame::panel_title)) : "r1", "r2", "r3", "lr", "cc", "memory"); \
    register char * outgoing_title asm("r4"); \
    OVERVIEW_ADDRESS(outgoing_title, panel_title); \
    OVERVIEW_SET_ARG(outgoing_title); \
    register FarmOverviewReport * report asm("r0"); \
    OVERVIEW_LOAD(report, report); \
    OVERVIEW_PIN(report); \
    register FarmReportMenu * menu asm("r1"); \
    OVERVIEW_LOAD(menu, menu); \
    OVERVIEW_PIN(menu); \
    asm volatile ("mov r2, sl\n\tmov r3, %0\n\tbl func_08076BFC" : : "I" (FARM_OVERVIEW_PANEL_HEIGHT) : OVERVIEW_CLOBBERS); \
} while (0)

#define OVERVIEW_SAVE_PANEL() do { \
    register FarmOverviewReport * report asm("r5"); \
    OVERVIEW_LOAD(report, report); \
    OVERVIEW_PIN(report); \
    register u32 vtable_offset asm("r7"); \
    asm volatile ("ldr %0, .Loverview_report_vtable_offset" : "=l" (vtable_offset)); \
    register void const ** slot asm("r1"); \
    asm ("add %0, %1, %2" : "=l" (slot) : "l" (report), "l" (vtable_offset)); \
    register void const * vtable asm("r0"); \
    asm volatile ("ldr %0, .Loverview_report_vtable" : "=l" (vtable)); \
    *slot = vtable; \
    register FarmOverviewPanel * panel asm("r0"); \
    asm ("ldr %0, [%1, #4]" : "=l" (panel) : "l" (report) : "memory"); \
    asm volatile ("str %0, [sp, %1]" : : "l" (panel), "i" (OVERVIEW_SLOT(panel)) : "memory"); \
} while (0)

#define OVERVIEW_ALLOCATE_COMMANDS(growth_reg, empty_label, bytes_reg, allocated_label, done_label, reload) do { \
    register u32 const * growth_count asm(growth_reg); \
    asm ("" : "=l" (growth_count)); \
    register u32 new_count asm("r0"); \
    asm ("ldr %0, [%1]\n\tadd %0, r4, %0" : "=l" (new_count) : "l" (growth_count) : "memory", "cc"); \
    asm volatile ("cmp %0, #0\n\tbeq " empty_label : : "l" (new_count) : "cc"); \
    register u32 bytes asm("r4") = new_count * sizeof(DmaCommand); \
    OVERVIEW_PIN(bytes); \
    register u32 allocator_arg asm("r0") = bytes; \
    OVERVIEW_PIN(allocator_arg); \
    register void * buffer asm("r0") = malloc(allocator_arg); \
    OVERVIEW_PIN(buffer); \
    register u32 capacity_bytes asm(bytes_reg); \
    asm volatile ("mov %0, r4" : "=h" (capacity_bytes)); \
    asm volatile ("cmp %0, #0\n\tbne " allocated_label : : "l" (buffer) : "cc"); \
    asm volatile ("mov %0, %1" : "=l" (allocator_arg) : "h" (capacity_bytes)); \
    buffer = RetryFarmOverviewAllocation(allocator_arg); \
    OVERVIEW_PIN(buffer); \
    if (reload) \
        asm volatile (allocated_label ":\n\tldr r1, [r6]" : : : "r1", "memory"); \
    asm volatile ("b " done_label "\n\t" empty_label ":\n\tmov r0, #0\n\tmov " bytes_reg ", r0\n\t" done_label ":" \
        : : : "r0", bytes_reg, "cc"); \
} while (0)

#define OVERVIEW_COPY_COMMAND(words) asm volatile ( \
    ".syntax unified\n\tldmia r1!, {" words "}\n\tstmia r0!, {" words "}\n\tldr r1, [r1]\n\tstr r1, [r0]\n\t.syntax divided" \
    : : : "r0", "r1", "r2", "r3", "r4", "r5", "r7", "memory")

#define OVERVIEW_COPY_STACK_COMMAND(words, field) asm volatile ( \
    ".syntax unified\n\tldmia r1!, {" words "}\n\tstmia r0!, {" words "}\n\tldr r1, [sp, %0]\n\tstr r1, [r0]\n\t.syntax divided" \
    : : "I" (OVERVIEW_SLOT(field) + offsetof(DmaCommand, control_flags)) \
    : "r0", "r1", "r2", "r3", "r4", "r5", "r7", "memory")

#define OVERVIEW_TRANSFER_WORDS(regs, last_word) \
    "ldmia r1!, {" regs "}\n\tstmia r0!, {" regs "}\n\tldr r1, " last_word "\n\tstr r1, [r0]\n\t"

#define OVERVIEW_APPEND_GROWN_QUEUE(count_reg, command_reg, single_regs, range_regs, last_word, \
    range, advance_single, copy_range, advance_range, finish_range, publish) \
    asm volatile (".syntax unified\n\t" \
        "cmp " count_reg ", #1\n\tbne " range "\n\tcmp r5, #0\n\tbeq " advance_single "\n\t" \
        "adds r0, r5, #0\n\tmov r1, " command_reg "\n\t" OVERVIEW_TRANSFER_WORDS(single_regs, last_word) \
        advance_single ":\n\tadds r5, %0\n\tb " publish "\n\t" \
        range ":\n\tadds r3, " count_reg ", #0\n\tadds r2, r5, #0\n\tcmp r3, #0\n\tbeq " finish_range "\n\t" \
        copy_range ":\n\tcmp r2, #0\n\tbeq " advance_range "\n\tadds r0, r2, #0\n\tmov r1, " command_reg "\n\t" \
        OVERVIEW_TRANSFER_WORDS(range_regs, last_word) \
        advance_range ":\n\tsubs r3, #1\n\tadds r2, %0\n\tcmp r3, #0\n\tbne " copy_range "\n\t" \
        finish_range ":\n\tadds r5, r2, #0\n\t" publish ":\n\t.syntax divided" \
        : : "I" (sizeof(DmaCommand)) : "r0", "r1", "r2", "r3", "r4", "r5", "r7", "cc", "memory")

#define OVERVIEW_APPEND_FAST(preserve_copy, grow, publish, command_reg, regs, last_word) do { \
    register DmaCommand * tail asm("r1"); \
    register DmaCommand * capacity asm("r0"); \
    asm volatile ("ldr %0, [r6, %2]\n\tldr %1, [r6, %3]" \
        : "=l" (tail), "=l" (capacity) \
        : "I" (offsetof(DmaTransferQueue, end)), "I" (offsetof(DmaTransferQueue, capacity_end)) : "memory"); \
    if (preserve_copy) \
        asm volatile ("mov sl, r8" : : : "r10"); \
    asm volatile (".syntax unified\n\tcmp %1, %0\n\tbeq " grow "\n\tcmp %1, #0\n\tbeq " publish "\n\t" \
        "adds r0, r1, #0\n\tmov r1, " command_reg "\n\t" OVERVIEW_TRANSFER_WORDS(regs, last_word) \
        publish ":\n\t.syntax divided" : "+l" (capacity), "+l" (tail) : : "r2", "r3", "r4", "r5", "r7", "cc", "memory"); \
    register uintptr_t next asm("r0"); \
    asm ("ldr %0, [r6, %1]" : "=l" (next) : "I" (offsetof(DmaTransferQueue, end)) : "memory"); \
    next += sizeof(DmaCommand); \
    asm volatile ("str %0, [r6, %1]" : : "l" (next), "I" (offsetof(DmaTransferQueue, end)) : "memory"); \
} while (0)

#define OVERVIEW_COPY_EXISTING(loop, advance, regs) \
    asm volatile (".syntax unified\n\t" loop ":\n\tcmp r3, #0\n\tbeq " advance "\n\t" \
        "adds r0, r3, #0\n\tadds r1, r2, #0\n\t" OVERVIEW_TRANSFER_WORDS(regs, "[r1]") \
        advance ":\n\tadds r2, %0\n\tadds r3, %0\n\t.syntax divided" \
        : : "I" (sizeof(DmaCommand)) : "r0", "r1", "r2", "r3", "r4", "r5", "r7", "cc", "memory")

#define OVERVIEW_GET_DMA_QUEUE(report_reg, load_again) do { \
    register FarmOverviewReport * report asm(report_reg); \
    if (load_again) \
        OVERVIEW_LOAD(report, report); \
    else \
        asm ("" : "=l" (report)); \
    register HardwareContext * hardware asm("r0"); \
    asm ("ldr %0, [%1]" : "=l" (hardware) : "l" (report) : "memory"); \
    register DmaTransferQueue * queue asm("r0") = HardwareContext_GetDmaQueue(hardware); \
    OVERVIEW_PIN(queue); \
} while (0)

#define OVERVIEW_INIT_COPY(field, source_load, dest_load, fixed_bytes) do { \
    register DmaCommand * command asm("r0"); \
    OVERVIEW_ADDRESS(command, field); \
    OVERVIEW_PIN(command); \
    register void const * source asm("r1"); \
    asm volatile (".syntax unified\n\t" source_load "\n\t.syntax divided" : "=l" (source) : : "memory"); \
    register void * dest asm("r2"); \
    asm volatile (dest_load : "=l" (dest) : : "memory"); \
    register u32 bytes asm("r3"); \
    if (fixed_bytes) { \
        bytes = fixed_bytes; \
    } else { \
        asm ("" : "=l" (bytes)); \
    } \
    OVERVIEW_PIN(bytes); \
    DmaCommand_InitializeCopy(command, source, dest, bytes); \
} while (0)

#define OVERVIEW_DRAW_LABEL_OR_NAME() do { \
    register u32 packed asm("r0"); \
    register void * pixels asm("r1"); \
    asm ("" : "=l" (packed), "=l" (pixels)); \
    register u32 x asm("r2") = 0; \
    OVERVIEW_PIN(x); \
    register u32 y asm("r3") = 0; \
    OVERVIEW_PIN(y); \
    asm volatile ("bl DrawMessageTileString" : : "l" (packed), "l" (pixels), "l" (x), "l" (y) : "lr", "cc", "memory"); \
} while (0)

#define OVERVIEW_DRAW_BLANK_LABEL(field, dim_offset, label) do { \
    register char const * text asm("r2"); \
    asm volatile ("ldr %0, " label : "=l" (text)); \
    register u32 side asm("r1") = FARM_OVERVIEW_LABEL_SIDE; \
    OVERVIEW_PIN(side); \
    asm volatile ("add r0, sp, %0\n\tstrh %1, [r0]\n\tldr r0, [sp, %2]\n\tstrh %1, [r0, #2]\n\tldr r0, [sp, %0]" \
        : : "I" (dim_offset), "l" (side), "i" (OVERVIEW_SLOT(field##_bounds)) : "r0", "memory"); \
    OVERVIEW_SET_ARG(text); \
    register void * pixels asm("r1"); \
    OVERVIEW_LOAD(pixels, field##_pixels); \
    OVERVIEW_PIN(pixels); \
    OVERVIEW_DRAW_LABEL_OR_NAME(); \
} while (0)

#define OVERVIEW_FILL_NAME_BITMAP() asm volatile ("bl FillMessageTiles" : : : "r0", "r1", "r2", "r3", "lr", "cc", "memory")

#define OVERVIEW_DRAW_COUNT(stride_reg) do { \
    register u32 first_tile asm("r2") = REPORT_DIGIT_TILE >> 1; \
    OVERVIEW_PIN(first_tile); \
    first_tile <<= 1; \
    OVERVIEW_PIN(first_tile); \
    register u32 stride asm(stride_reg) = REPORT_TILEMAP_STRIDE; \
    OVERVIEW_PIN(stride); \
    OVERVIEW_SET_ARG(stride); \
    register u32 palette asm("r3") = 0; \
    OVERVIEW_PIN(palette); \
    asm volatile ("bl DrawTallMessageNumber" : : "l" (first_tile), "l" (palette) : "r0", "r1", "lr", "cc", "memory"); \
} while (0)

#define OVERVIEW_SET_SCROLL_HEIGHT() do { \
    register void * panel asm("r0"); \
    register i32 height asm("r1"); \
    asm ("" : "=l" (panel), "=l" (height)); \
    asm volatile ("bl func_08075E24" : : "l" (panel), "l" (height) : "r2", "r3", "lr", "cc", "memory"); \
} while (0)

#define OVERVIEW_READ_COUNTER(owner, member) do { \
    register owner const * data asm("r0"); \
    asm ("" : "=l" (data)); \
    register u32 count asm("r0") = data->member(); \
    OVERVIEW_PIN(count); \
} while (0)

asm (
        "\t.equ .Lfarm_overview_stack_bytes, 0x29C\n"
        "\t.equ .Lfarm_overview_game_data, 0x58C\n"
        "\t.equ .Lfarm_overview_portrait_uploads, 0x594\n"
        "\t.equ .Lfarm_overview_vtable, 0x5B4\n"
        "\t.equ .Lfarm_overview_panel_tilemap, 0x18\n"
        "\n"
        "\t.equ .Lfarm_overview_sp_report, 0x1C4\n"
        "\t.equ .Lfarm_overview_sp_menu, 0x1C8\n"
        "\t.equ .Lfarm_overview_sp_scroll_panel, 0x1CC\n"
        "\t.equ .Lfarm_overview_sp_portrait_tile_row, 0x1D0\n"
        "\t.equ .Lfarm_overview_sp_spouse_id, 0x1D4\n"
        "\t.equ .Lfarm_overview_sp_spouse, 0x1D8\n"
        "\t.equ .Lfarm_overview_sp_active_npc, 0x1DC\n"
        "\t.equ .Lfarm_overview_sp_horse, 0x1E0\n"
        "\t.equ .Lfarm_overview_sp_content_tile_rows, 0x210\n"
        "\t.equ .Lfarm_overview_sp_portrait_lower_tile_row, 0x214\n"
        "\t.equ .Lfarm_overview_sp_dog_tile_row, 0x218\n"
        "\t.equ .Lfarm_overview_sp_portrait_y_pixels, 0x21C\n"
        "\t.equ .Lfarm_overview_sp_portrait_tilemap_offset, 0x220\n"
        "\t.equ .Lfarm_overview_sp_horse_name_tiles, 0x224\n"
        "\t.equ .Lfarm_overview_sp_name_copy_command_ptr, 0x228\n"
        "\t.equ .Lfarm_overview_sp_old_transfer_list_end, 0x294\n"
        "\t.equ .Lfarm_overview_sp_new_transfer_list_buffer, 0x298\n"
        "\n"
        "\t@ r6 = list, r5 = new end. Preserve the original empty destructor loop before freeing.\n"
        "\t.macro CommitFarmOverviewTransferList capacity_bytes, new_begin\n"
        "\tldr r2, [r6, #4]\n"
        "\tldr r0, [r6]\n"
        "\tadds r1, r0, #0\n"
        "\tcmp r0, r2\n"
        "\tbeq .Lfarm_overview_free_transfers\\@\n"
        ".Lfarm_overview_retire_transfer\\@:\n"
        "\tadds r1, #0x10\n"
        "\tcmp r1, r2\n"
        "\tbne .Lfarm_overview_retire_transfer\\@\n"
        ".Lfarm_overview_free_transfers\\@:\n"
        "\tcmp r0, #0\n"
        "\tbeq .Lfarm_overview_publish_transfers\\@\n"
        "\tbl free\n"
        ".Lfarm_overview_publish_transfers\\@:\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadd r0, \\capacity_bytes\n"
        "\tldr \\new_begin, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tstr \\new_begin, [r6]\n"
        "\tstr r5, [r6, #4]\n"
        "\tstr r0, [r6, #0xc]\n"
        "\t.endm\n"
        "\n"
        "\t.equ .Loverview_sp_first_blank_pixels, 0x1f4\n"
        "\t.equ .Loverview_sp_second_blank_pixels, 0x1f8\n"
        "\t.equ .Loverview_sp_third_blank_pixels, 0x1fc\n"
        "\t.equ .Loverview_sp_first_wide_blank_pixels, 0x200\n"
        "\t.equ .Loverview_sp_second_wide_blank_pixels, 0x204\n"
        "\t.equ .Loverview_sp_gold_suffix_pixels, 0x208\n"
        "\t.equ .Loverview_sp_farmer_name_pixels, 0x20c\n"
        "\t.equ .Loverview_sp_first_blank_bounds, 0x22c\n"
        "\t.equ .Loverview_sp_second_blank_bounds, 0x230\n"
        "\t.equ .Loverview_sp_third_blank_bounds, 0x234\n"
        "\t.equ .Loverview_sp_first_wide_blank_bounds, 0x23c\n"
        "\t.equ .Loverview_sp_second_wide_blank_bounds, 0x240\n"
        "\t.equ .Loverview_sp_gold_suffix_bounds, 0x244\n"
        "\t.equ .Loverview_sp_farmer_name_fill_bounds, 0x248\n"
        "\t.equ .Loverview_sp_farmer_name_bounds, 0x24c\n"
);

// The original initializer shares its stack frame across portrait branches and literal pools.
EC SECTION(".text.f3.0805FF14") __attribute__((naked)) FarmOverviewReport * InitFarmOverviewReport(
    FarmOverviewReport *, FarmReportMenu *, FarmReportData const *)
{
    asm volatile (
        ".syntax unified\n"
        "\tpush {r4, r5, r6, r7, lr}\n"
        "\tmov r7, sl\n"
        "\tmov r6, sb\n"
        "\tmov r5, r8\n"
        "\tpush {r5, r6, r7}\n"
        "\tldr r4, .Loverview_stack_adjust\n"
        "\tadd sp, r4\n"
        "\tstr r0, [sp, #.Lfarm_overview_sp_report]\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_menu]\n"
        "\tmov sl, r2\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_BUILD_PANEL();

    OVERVIEW_SAVE_PANEL();

    asm volatile (
        ".syntax unified\n"
        "\tldr r2, .Loverview_day_blank_label\n"
        "\tadd r1, sp, #0x58\n"
        "\tmovs r0, #2\n"
        "\tstrh r0, [r1]\n"
        "\tstrh r0, [r1, #2]\n"
        "\tldr r0, [sp, #0x58]\n"
        "\tldr r1, .Loverview_day_unit_bitmap\n"
        "\tadds r4, r5, r1\n"
        "\tstr r2, [sp]\n"
        "\tadds r1, r4, #0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_DRAW_LABEL_OR_NAME();

    OVERVIEW_GET_DMA_QUEUE("r5", false);

    asm volatile (
        ".syntax unified\n"
        "\tadds r6, r0, #0\n"
        "\tmovs r3, #0x38\n"
        "\tlsls r2, r3, #5\n"
        "\tmovs r0, #0xc0\n"
        "\tlsls r0, r0, #0x13\n"
        "\tadds r2, r2, r0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_INIT_COPY(label_copy, "adds %0, r4, #0", "", 0x80);

    asm volatile (
        ".syntax unified\n"
        "\tadd r4, sp, #0x38\n"
        "\tmov r8, r4\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(true, ".L080600D0", ".L0806000C",
        "sl", "r2, r5, r7", "[r1]");

    asm volatile (
        ".syntax unified\n"
        "\tmov r3, sp\n"
        "\tadds r3, #0x64\n"
        "\tstr r3, [sp, #.Loverview_sp_first_blank_bounds]\n"
        "\tmovs r5, #0xc8\n"
        "\tlsls r5, r5, #3\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_report]\n"
        "\tadds r5, r4, r5\n"
        "\tstr r5, [sp, #.Loverview_sp_first_blank_pixels]\n"
        "\tmov r7, sp\n"
        "\tadds r7, #0x70\n"
        "\tstr r7, [sp, #.Loverview_sp_second_blank_bounds]\n"
        "\tldr r0, .L080600BC\n"
        "\tadds r0, r4, r0\n"
        "\tstr r0, [sp, #.Loverview_sp_second_blank_pixels]\n"
        "\tmov r1, sp\n"
        "\tadds r1, #0x7c\n"
        "\tstr r1, [sp, #.Loverview_sp_third_blank_bounds]\n"
        "\tmovs r2, #0xe9\n"
        "\tlsls r2, r2, #3\n"
        "\tadds r2, r4, r2\n"
        "\tstr r2, [sp, #.Loverview_sp_third_blank_pixels]\n"
        "\tadds r3, #0x24\n"
        "\tstr r3, [sp, #.Loverview_sp_first_wide_blank_bounds]\n"
        "\tldr r5, .L080600C0\n"
        "\tadds r5, r4, r5\n"
        "\tstr r5, [sp, #.Loverview_sp_first_wide_blank_pixels]\n"
        "\tadds r7, #0x24\n"
        "\tstr r7, [sp, #.Loverview_sp_second_wide_blank_bounds]\n"
        "\tmovs r0, #0x85\n"
        "\tlsls r0, r0, #4\n"
        "\tadds r0, r4, r0\n"
        "\tstr r0, [sp, #.Loverview_sp_second_wide_blank_pixels]\n"
        "\tadds r1, #0x24\n"
        "\tstr r1, [sp, #.Loverview_sp_gold_suffix_bounds]\n"
        "\tldr r2, .L080600C4\n"
        "\tadds r2, r4, r2\n"
        "\tstr r2, [sp, #.Loverview_sp_gold_suffix_pixels]\n"
        "\tadds r3, #0x74\n"
        "\tstr r3, [sp, #.Loverview_sp_farmer_name_fill_bounds]\n"
        "\tldr r5, .L080600C8\n"
        "\tadds r5, r4, r5\n"
        "\tstr r5, [sp, #.Loverview_sp_farmer_name_pixels]\n"
        "\tmovs r7, #0x80\n"
        "\tlsls r7, r7, #1\n"
        "\tadd r7, sp\n"
        "\tstr r7, [sp, #.Loverview_sp_farmer_name_bounds]\n"
        "\tmov r0, sp\n"
        "\tadds r0, #0x48\n"
        "\tstr r0, [sp, #.Lfarm_overview_sp_name_copy_command_ptr]\n"
        "\tmovs r1, #0xaa\n"
        "\tlsls r1, r1, #1\n"
        "\tadd r1, sp\n"
        "\tstr r1, [sp, #0x274]\n"
        "\tldr r2, .L080600CC\n"
        "\tadds r2, r4, r2\n"
        "\tstr r2, [sp, #.Lfarm_overview_sp_horse_name_tiles]\n"
        "\tmovs r3, #0xac\n"
        "\tlsls r3, r3, #1\n"
        "\tadd r3, sp\n"
        "\tstr r3, [sp, #0x278]\n"
        "\tmovs r4, #0xb2\n"
        "\tlsls r4, r4, #1\n"
        "\tadd r4, sp\n"
        "\tstr r4, [sp, #0x27c]\n"
        "\tmovs r5, #0xb4\n"
        "\tlsls r5, r5, #1\n"
        "\tadd r5, sp\n"
        "\tstr r5, [sp, #0x280]\n"
        "\tmovs r7, #0x38\n"
        "\tlsls r7, r7, #0x10\n"
        "\tstr r7, [sp, #0x238]\n"
        "\tb .Lfarm_overview_draw_first_blank_label\n"
        "\t.align 2, 0\n"
        ".Loverview_stack_adjust: .4byte -.Lfarm_overview_stack_bytes\n"
        ".Loverview_property_suffix: .4byte gFarmReportPropertySuffix\n"
        ".Loverview_report_vtable_offset: .4byte .Lfarm_overview_vtable\n"
        ".Loverview_report_vtable: .4byte gFarmOverviewReportVTable\n"
        ".Loverview_day_blank_label: .4byte gFarmReportBlankLabel\n"
        ".Loverview_day_unit_bitmap: .4byte 0x000005BC\n"
        ".L080600BC: .4byte 0x000006C4\n"
        ".L080600C0: .4byte 0x000007CC\n"
        ".L080600C4: .4byte 0x000008D4\n"
        ".L080600C8: .4byte 0x00000958\n"
        ".L080600CC: .4byte 0x00001568\n"
        ".L080600D0:\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tmovs r0, #1\n"
        "\tstr r0, [sp, #0x60]\n"
        "\tldr r0, [r6]\n"
        "\tsubs r0, r1, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tstr r4, [sp, #0x5c]\n"
        "\tadd r1, sp, #0x60\n"
        "\tadd r0, sp, #0x5c\n"
        "\tcmp r4, #1\n"
        "\tbhs .L080600E8\n"
        "\tadds r0, r1, #0\n"
        ".L080600E8:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r0", ".L08060106", "sb",
        ".L0806010A", ".L0806010A", false);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tldr r2, [r6]\n"
        "\tadds r3, r5, #0\n"
        "\tmov r4, sp\n"
        "\tadds r4, #0x64\n"
        "\tstr r4, [sp, #.Loverview_sp_first_blank_bounds]\n"
        "\tmovs r7, #0xc8\n"
        "\tlsls r7, r7, #3\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_report]\n"
        "\tadds r7, r5, r7\n"
        "\tstr r7, [sp, #.Loverview_sp_first_blank_pixels]\n"
        "\tmov r0, sp\n"
        "\tadds r0, #0x70\n"
        "\tstr r0, [sp, #.Loverview_sp_second_blank_bounds]\n"
        "\tldr r1, .L080601DC\n"
        "\tadds r1, r5, r1\n"
        "\tstr r1, [sp, #.Loverview_sp_second_blank_pixels]\n"
        "\tadds r4, #0x18\n"
        "\tstr r4, [sp, #.Loverview_sp_third_blank_bounds]\n"
        "\tmovs r7, #0xe9\n"
        "\tlsls r7, r7, #3\n"
        "\tadds r7, r5, r7\n"
        "\tstr r7, [sp, #.Loverview_sp_third_blank_pixels]\n"
        "\tadds r0, #0x18\n"
        "\tstr r0, [sp, #.Loverview_sp_first_wide_blank_bounds]\n"
        "\tldr r1, .L080601E0\n"
        "\tadds r1, r5, r1\n"
        "\tstr r1, [sp, #.Loverview_sp_first_wide_blank_pixels]\n"
        "\tadds r4, #0x18\n"
        "\tstr r4, [sp, #.Loverview_sp_second_wide_blank_bounds]\n"
        "\tmovs r7, #0x85\n"
        "\tlsls r7, r7, #4\n"
        "\tadds r7, r5, r7\n"
        "\tstr r7, [sp, #.Loverview_sp_second_wide_blank_pixels]\n"
        "\tadds r0, #0x18\n"
        "\tstr r0, [sp, #.Loverview_sp_gold_suffix_bounds]\n"
        "\tldr r1, .L080601E4\n"
        "\tadds r1, r5, r1\n"
        "\tstr r1, [sp, #.Loverview_sp_gold_suffix_pixels]\n"
        "\tadds r4, #0x68\n"
        "\tstr r4, [sp, #.Loverview_sp_farmer_name_fill_bounds]\n"
        "\tldr r7, .L080601E8\n"
        "\tadds r7, r5, r7\n"
        "\tstr r7, [sp, #.Loverview_sp_farmer_name_pixels]\n"
        "\tmovs r0, #0x80\n"
        "\tlsls r0, r0, #1\n"
        "\tadd r0, sp\n"
        "\tstr r0, [sp, #.Loverview_sp_farmer_name_bounds]\n"
        "\tmov r1, sp\n"
        "\tadds r1, #0x48\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_name_copy_command_ptr]\n"
        "\tmovs r4, #0xaa\n"
        "\tlsls r4, r4, #1\n"
        "\tadd r4, sp\n"
        "\tstr r4, [sp, #0x274]\n"
        "\tldr r7, .L080601EC\n"
        "\tadds r7, r5, r7\n"
        "\tstr r7, [sp, #.Lfarm_overview_sp_horse_name_tiles]\n"
        "\tmovs r0, #0xac\n"
        "\tlsls r0, r0, #1\n"
        "\tadd r0, sp\n"
        "\tstr r0, [sp, #0x278]\n"
        "\tmovs r1, #0xb2\n"
        "\tlsls r1, r1, #1\n"
        "\tadd r1, sp\n"
        "\tstr r1, [sp, #0x27c]\n"
        "\tmovs r4, #0xb4\n"
        "\tlsls r4, r4, #1\n"
        "\tadd r4, sp\n"
        "\tstr r4, [sp, #0x280]\n"
        "\tmovs r5, #0x38\n"
        "\tlsls r5, r5, #0x10\n"
        "\tstr r5, [sp, #0x238]\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r7\n"
        "\tbeq .L080601BE\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L080601A4", ".L080601B4", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r0\n"
        "\tbne .L080601A4\n"
        ".L080601BE:\n"
        "\tadds r5, r3, #0\n"
        "\tldr r0, [sp, #0x60]\n"
        "\tcmp r0, #1\n"
        "\tbne .L080601F0\n"
        "\tcmp r5, #0\n"
        "\tbeq .L080601D6\n"
        "\tadds r0, r5, #0\n"
        "\tmov r1, r8\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_COMMAND("r2, r3, r4");

    asm volatile (
        ".syntax unified\n"
        ".L080601D6:\n"
        "\tadds r5, #0x10\n"
        "\tb .L08060212\n"
        "\t.align 2, 0\n"
        ".L080601DC: .4byte 0x000006C4\n"
        ".L080601E0: .4byte 0x000007CC\n"
        ".L080601E4: .4byte 0x000008D4\n"
        ".L080601E8: .4byte 0x00000958\n"
        ".L080601EC: .4byte 0x00001568\n"
        ".L080601F0:\n"
        "\tadds r3, r0, #0\n"
        "\tadds r2, r5, #0\n"
        "\tcmp r3, #0\n"
        "\tbeq .L08060210\n"
        ".L080601F8:\n"
        "\tcmp r2, #0\n"
        "\tbeq .L08060208\n"
        "\tadds r0, r2, #0\n"
        "\tmov r1, r8\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_COMMAND("r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        ".L08060208:\n"
        "\tsubs r3, #1\n"
        "\tadds r2, #0x10\n"
        "\tcmp r3, #0\n"
        "\tbne .L080601F8\n"
        ".L08060210:\n"
        "\tadds r5, r2, #0\n"
        ".L08060212:\n"
        "\tCommitFarmOverviewTransferList sb, r7\n"
        "\t@ Rasterize the fixed-width text labels before filling the panel.\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    // Render the date and number labels into fixed-width tile bitmaps.

    asm volatile (
        ".syntax unified\n"
        ".Lfarm_overview_draw_first_blank_label:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_DRAW_BLANK_LABEL(first_blank, 0x64, ".L08060298");

    OVERVIEW_GET_DMA_QUEUE("r1", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r6, r0, #0\n"
        "\tmovs r3, #0x3c\n"
        "\tlsls r2, r3, #5\n"
        "\tmovs r0, #0xc0\n"
        "\tlsls r0, r0, #0x13\n"
        "\tadds r2, r2, r0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_INIT_COPY(label_copy, "ldr %0, [sp, #.Loverview_sp_first_blank_pixels]", "", 0x80);

    asm volatile (
        ".syntax unified\n"
        "\tmov sb, sl\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L0806029C", ".L08060288",
        "sl", "r4, r5, r7", "[r1]");

    asm volatile (
        ".syntax unified\n"
        "\tmovs r0, #0x3c\n"
        "\tlsls r0, r0, #0x10\n"
        "\tstr r0, [sp, #0x260]\n"
        "\tb .L08060366\n"
        "\t.align 2, 0\n"
        ".L08060298: .4byte gFarmReportBlankLabel\n"
        ".L0806029C:\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tmovs r0, #1\n"
        "\tstr r0, [sp, #0x6c]\n"
        "\tldr r0, [r6]\n"
        "\tsubs r0, r1, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tstr r4, [sp, #0x68]\n"
        "\tadd r1, sp, #0x6c\n"
        "\tadd r0, sp, #0x68\n"
        "\tcmp r4, #1\n"
        "\tbhs .L080602B4\n"
        "\tadds r0, r1, #0\n"
        ".L080602B4:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r0", ".L080602D2", "r8",
        ".L080602D6", ".L080602D6", false);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tldr r2, [r6]\n"
        "\tadds r3, r5, #0\n"
        "\tmovs r4, #0x3c\n"
        "\tlsls r4, r4, #0x10\n"
        "\tstr r4, [sp, #0x260]\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r5\n"
        "\tbeq .L08060304\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L080602EA", ".L080602FA", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r7\n"
        "\tbne .L080602EA\n"
        ".L08060304:\n"
        "\tadds r5, r3, #0\n"
        "\tldr r0, [sp, #0x6c]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r0", "sb", "r2, r3, r4", "r4, r5, r7", "[r1]",
        ".L08060320", ".L0806031C", ".L08060328", ".L08060338", ".L08060340", ".L08060342");

    asm volatile (
        ".syntax unified\n"
        "\tCommitFarmOverviewTransferList r8, r7\n"
        ".L08060366:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_DRAW_BLANK_LABEL(second_blank, 0x70, ".L080603B8");

    OVERVIEW_GET_DMA_QUEUE("r1", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r6, r0, #0\n"
        "\tldr r2, .L080603BC\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_INIT_COPY(label_copy, "ldr %0, [sp, #.Loverview_sp_second_blank_pixels]", "", 0x80);

    asm volatile (
        ".syntax unified\n"
        "\tmov sb, sl\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L080603C0", ".L080603B0",
        "sl", "r2, r3, r4", "[r1]");

    asm volatile (
        ".syntax unified\n"
        "\tb .L08060486\n"
        "\t.align 2, 0\n"
        ".L080603B8: .4byte gFarmReportBlankLabel\n"
        ".L080603BC: .4byte 0x06000800\n"
        ".L080603C0:\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tmovs r0, #1\n"
        "\tstr r0, [sp, #0x78]\n"
        "\tldr r0, [r6]\n"
        "\tadds r5, r1, #0\n"
        "\tsubs r0, r5, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tstr r4, [sp, #0x74]\n"
        "\tadd r1, sp, #0x78\n"
        "\tadd r0, sp, #0x74\n"
        "\tcmp r4, #1\n"
        "\tbhs .L080603DA\n"
        "\tadds r0, r1, #0\n"
        ".L080603DA:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r0", ".L080603F8", "r8",
        ".L080603FC", ".L080603FC", false);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tldr r2, [r6]\n"
        "\tadds r3, r5, #0\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r7\n"
        "\tbeq .L08060424\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L0806040A", ".L0806041A", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r0\n"
        "\tbne .L0806040A\n"
        ".L08060424:\n"
        "\tadds r5, r3, #0\n"
        "\tldr r0, [sp, #0x78]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r0", "sb", "r2, r3, r4", "r4, r5, r7", "[r1]",
        ".L08060440", ".L0806043C", ".L08060448", ".L08060458", ".L08060460", ".L08060462");

    asm volatile (
        ".syntax unified\n"
        "\tCommitFarmOverviewTransferList r8, r7\n"
        ".L08060486:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_DRAW_BLANK_LABEL(third_blank, 0x7c, ".L080604E8");

    OVERVIEW_GET_DMA_QUEUE("r1", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r6, r0, #0\n"
        "\tmovs r3, #0x44\n"
        "\tlsls r2, r3, #5\n"
        "\tmovs r0, #0xc0\n"
        "\tlsls r0, r0, #0x13\n"
        "\tadds r2, r2, r0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_INIT_COPY(label_copy, "ldr %0, [sp, #.Loverview_sp_third_blank_pixels]", "", 0x80);

    asm volatile (
        ".syntax unified\n"
        "\tmov sb, sl\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L080604EC", ".L080604D8",
        "sl", "r4, r5, r7", "[r1]");

    asm volatile (
        ".syntax unified\n"
        "\tmovs r0, #0x44\n"
        "\tlsls r0, r0, #0x10\n"
        "\tstr r0, [sp, #0x1e4]\n"
        "\tb .L080605B6\n"
        "\t.align 2, 0\n"
        ".L080604E8: .4byte gFarmReportBlankLabel\n"
        ".L080604EC:\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tmovs r0, #1\n"
        "\tstr r0, [sp, #0x84]\n"
        "\tldr r0, [r6]\n"
        "\tsubs r0, r1, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tstr r4, [sp, #0x80]\n"
        "\tadd r1, sp, #0x84\n"
        "\tadd r0, sp, #0x80\n"
        "\tcmp r4, #1\n"
        "\tbhs .L08060504\n"
        "\tadds r0, r1, #0\n"
        ".L08060504:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r0", ".L08060522", "r8",
        ".L08060526", ".L08060526", false);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tldr r2, [r6]\n"
        "\tadds r3, r5, #0\n"
        "\tmovs r4, #0x44\n"
        "\tlsls r4, r4, #0x10\n"
        "\tstr r4, [sp, #0x1e4]\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r5\n"
        "\tbeq .L08060554\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L0806053A", ".L0806054A", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r7\n"
        "\tbne .L0806053A\n"
        ".L08060554:\n"
        "\tadds r5, r3, #0\n"
        "\tldr r0, [sp, #0x84]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r0", "sb", "r2, r3, r4", "r4, r5, r7", "[r1]",
        ".L08060570", ".L0806056C", ".L08060578", ".L08060588", ".L08060590", ".L08060592");

    asm volatile (
        ".syntax unified\n"
        "\tCommitFarmOverviewTransferList r8, r7\n"
        ".L080605B6:\n"
        "\tldr r2, .L08060618\n"
        "\tmovs r0, #2\n"
        "\tadd r1, sp, #0x88\n"
        "\tstrh r0, [r1]\n"
        "\tldr r3, [sp, #.Loverview_sp_first_wide_blank_bounds]\n"
        "\tstrh r0, [r3, #2]\n"
        "\tldr r0, [r1]\n"
        "\tstr r2, [sp]\n"
        "\tldr r1, [sp, #.Loverview_sp_first_wide_blank_pixels]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_DRAW_LABEL_OR_NAME();

    OVERVIEW_GET_DMA_QUEUE("r4", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r6, r0, #0\n"
        "\tmovs r5, #0x48\n"
        "\tlsls r2, r5, #5\n"
        "\tmovs r0, #0xc0\n"
        "\tlsls r0, r0, #0x13\n"
        "\tadds r2, r2, r0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_INIT_COPY(label_copy, "ldr %0, [sp, #.Loverview_sp_first_wide_blank_pixels]", "", 0x80);

    asm volatile (
        ".syntax unified\n"
        "\tmov sb, sl\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L0806061C", ".L08060608",
        "sl", "r2, r3, r7", "[r1]");

    asm volatile (
        ".syntax unified\n"
        "\tmovs r4, #0x48\n"
        "\tlsls r4, r4, #0x10\n"
        "\tstr r4, [sp, #0x1e8]\n"
        "\tb .L080606E6\n"
        "\t.align 2, 0\n"
        ".L08060618: .4byte gFarmReportBlankLabel\n"
        ".L0806061C:\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0x90\n"
        "\tstr r0, [r1]\n"
        "\tldr r0, [r6]\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tsubs r0, r5, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0x8c\n"
        "\tstr r4, [r0]\n"
        "\tldr r2, [r1]\n"
        "\tcmp r4, r2\n"
        "\tbhs .L08060638\n"
        "\tadds r0, r1, #0\n"
        ".L08060638:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r0", ".L08060656", "r8",
        ".L0806065A", ".L0806065A", false);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tldr r2, [r6]\n"
        "\tadds r3, r5, #0\n"
        "\tmovs r7, #0x48\n"
        "\tlsls r7, r7, #0x10\n"
        "\tstr r7, [sp, #0x1e8]\n"
        "\tb .L0806067E\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L0806066A", ".L0806067A", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        ".L0806067E:\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r0\n"
        "\tbne .L0806066A\n"
        "\tadds r5, r3, #0\n"
        "\tldr r1, [sp, #0x90]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r1", "sb", "r2, r3, r4", "r4, r5, r7", "[r1]",
        ".L080606A0", ".L0806069C", ".L080606A8", ".L080606B8", ".L080606C0", ".L080606C2");

    asm volatile (
        ".syntax unified\n"
        "\tCommitFarmOverviewTransferList r8, r7\n"
        ".L080606E6:\n"
        "\tldr r2, .L08060748\n"
        "\tmovs r0, #2\n"
        "\tadd r1, sp, #0x94\n"
        "\tstrh r0, [r1]\n"
        "\tldr r3, [sp, #.Loverview_sp_second_wide_blank_bounds]\n"
        "\tstrh r0, [r3, #2]\n"
        "\tldr r0, [r1]\n"
        "\tstr r2, [sp]\n"
        "\tldr r1, [sp, #.Loverview_sp_second_wide_blank_pixels]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_DRAW_LABEL_OR_NAME();

    OVERVIEW_GET_DMA_QUEUE("r4", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r6, r0, #0\n"
        "\tmovs r5, #0x4c\n"
        "\tlsls r2, r5, #5\n"
        "\tmovs r0, #0xc0\n"
        "\tlsls r0, r0, #0x13\n"
        "\tadds r2, r2, r0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_INIT_COPY(label_copy, "ldr %0, [sp, #.Loverview_sp_second_wide_blank_pixels]", "", 0x80);

    asm volatile (
        ".syntax unified\n"
        "\tmov sb, sl\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L0806074C", ".L08060738",
        "sl", "r2, r3, r7", "[r1]");

    asm volatile (
        ".syntax unified\n"
        "\tmovs r4, #0x4c\n"
        "\tlsls r4, r4, #0x10\n"
        "\tstr r4, [sp, #0x1ec]\n"
        "\tb .Lfarm_overview_draw_gold_suffix\n"
        "\t.align 2, 0\n"
        ".L08060748: .4byte gFarmReportBlankLabel\n"
        ".L0806074C:\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0x9c\n"
        "\tstr r0, [r1]\n"
        "\tldr r0, [r6]\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tsubs r0, r5, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0x98\n"
        "\tstr r4, [r0]\n"
        "\tldr r2, [r1]\n"
        "\tcmp r4, r2\n"
        "\tbhs .L08060768\n"
        "\tadds r0, r1, #0\n"
        ".L08060768:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r0", ".L08060786", "r8",
        ".L0806078A", ".L0806078A", false);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tldr r2, [r6]\n"
        "\tadds r3, r5, #0\n"
        "\tmovs r7, #0x4c\n"
        "\tlsls r7, r7, #0x10\n"
        "\tstr r7, [sp, #0x1ec]\n"
        "\tb .L080607AE\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L0806079A", ".L080607AA", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        ".L080607AE:\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r0\n"
        "\tbne .L0806079A\n"
        "\tadds r5, r3, #0\n"
        "\tldr r1, [sp, #0x9c]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r1", "sb", "r2, r3, r4", "r4, r5, r7", "[r1]",
        ".L080607D0", ".L080607CC", ".L080607D8", ".L080607E8", ".L080607F0", ".L080607F2");

    asm volatile (
        ".syntax unified\n"
        "\tCommitFarmOverviewTransferList r8, r7\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    // Prepare the gold suffix and report icon graphics.

    asm volatile (
        ".syntax unified\n"
        ".Lfarm_overview_draw_gold_suffix:\n"
        "\tldr r2, .L08060878\n"
        "\tmovs r0, #2\n"
        "\tadd r1, sp, #0xa0\n"
        "\tstrh r0, [r1]\n"
        "\tldr r3, [sp, #.Loverview_sp_gold_suffix_bounds]\n"
        "\tstrh r0, [r3, #2]\n"
        "\tldr r0, [r1]\n"
        "\tstr r2, [sp]\n"
        "\tldr r1, [sp, #.Loverview_sp_gold_suffix_pixels]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_DRAW_LABEL_OR_NAME();

    OVERVIEW_GET_DMA_QUEUE("r4", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r6, r0, #0\n"
        "\tmovs r5, #0x50\n"
        "\tlsls r2, r5, #5\n"
        "\tmovs r0, #0xc0\n"
        "\tlsls r0, r0, #0x13\n"
        "\tadds r2, r2, r0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_INIT_COPY(label_copy, "ldr %0, [sp, #.Loverview_sp_gold_suffix_pixels]", "", 0x80);

    asm volatile (
        ".syntax unified\n"
        "\tmov sb, sl\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L0806087C", ".L08060868",
        "sl", "r2, r3, r7", "[r1]");

    asm volatile (
        ".syntax unified\n"
        "\tmovs r4, #0x50\n"
        "\tlsls r4, r4, #0x10\n"
        "\tstr r4, [sp, #0x1f0]\n"
        "\tb .L08060946\n"
        "\t.align 2, 0\n"
        ".L08060878: .4byte gFarmReportGoldSuffix\n"
        ".L0806087C:\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0xa8\n"
        "\tstr r0, [r1]\n"
        "\tldr r0, [r6]\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tsubs r0, r5, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0xa4\n"
        "\tstr r4, [r0]\n"
        "\tldr r2, [r1]\n"
        "\tcmp r4, r2\n"
        "\tbhs .L08060898\n"
        "\tadds r0, r1, #0\n"
        ".L08060898:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r0", ".L080608B6", "r8",
        ".L080608BA", ".L080608BA", false);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tldr r2, [r6]\n"
        "\tadds r3, r5, #0\n"
        "\tmovs r7, #0x50\n"
        "\tlsls r7, r7, #0x10\n"
        "\tstr r7, [sp, #0x1f0]\n"
        "\tb .L080608DE\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L080608CA", ".L080608DA", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        ".L080608DE:\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r0\n"
        "\tbne .L080608CA\n"
        "\tadds r5, r3, #0\n"
        "\tldr r1, [sp, #0xa8]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r1", "sb", "r2, r3, r4", "r4, r5, r7", "[r1]",
        ".L08060900", ".L080608FC", ".L08060908", ".L08060918", ".L08060920", ".L08060922");

    asm volatile (
        ".syntax unified\n"
        "\tCommitFarmOverviewTransferList r8, r7\n"
        ".L08060946:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_GET_DMA_QUEUE("r1", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r6, r0, #0\n"
        "\tldr r1, .Loverview_trophy_tiles\n"
        "\tmovs r3, #0x54\n"
        "\tlsls r2, r3, #5\n"
        "\tmovs r0, #0xc0\n"
        "\tlsls r0, r0, #0x13\n"
        "\tadds r2, r2, r0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_INIT_COPY(label_copy, "", "", 0x80);

    asm volatile (
        ".syntax unified\n"
        "\tmov sb, sl\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L0806098C", ".L0806097E",
        "sl", "r4, r5, r7", "[r1]");

    asm volatile (
        ".syntax unified\n"
        "\tb .L08060A54\n"
        "\t.align 2, 0\n"
        ".Loverview_trophy_tiles: .4byte gUnk_0875196C\n"
        ".L0806098C:\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0xb0\n"
        "\tstr r0, [r1]\n"
        "\tldr r0, [r6]\n"
        "\tldr r2, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tsubs r0, r2, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0xac\n"
        "\tstr r4, [r0]\n"
        "\tldr r2, [r1]\n"
        "\tcmp r4, r2\n"
        "\tbhs .L080609A8\n"
        "\tadds r0, r1, #0\n"
        ".L080609A8:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r0", ".L080609C6", "r8",
        ".L080609CA", ".L080609CA", false);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tldr r2, [r6]\n"
        "\tadds r3, r5, #0\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r4\n"
        "\tbeq .L080609F2\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L080609D8", ".L080609E8", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r5\n"
        "\tbne .L080609D8\n"
        ".L080609F2:\n"
        "\tadds r5, r3, #0\n"
        "\tldr r1, [sp, #0xb0]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r1", "sb", "r2, r3, r7", "r4, r5, r7", "[r1]",
        ".L08060A0E", ".L08060A0A", ".L08060A16", ".L08060A26", ".L08060A2E", ".L08060A30");

    asm volatile (
        ".syntax unified\n"
        "\tCommitFarmOverviewTransferList r8, r1\n"
        ".L08060A54:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_GET_DMA_QUEUE("r2", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r6, r0, #0\n"
        "\tldr r1, .Loverview_trophy_palette\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_INIT_COPY(label_copy, "", "ldr %0, .L08060A90", 0x20);

    asm volatile (
        ".syntax unified\n"
        "\tmov sb, sl\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L08060A94", ".L08060A84",
        "sl", "r3, r4, r5", "[r1]");

    asm volatile (
        ".syntax unified\n"
        "\tb .L08060B58\n"
        "\t.align 2, 0\n"
        ".Loverview_trophy_palette: .4byte gUnk_087519EC\n"
        ".L08060A90: .4byte 0x05000020\n"
        ".L08060A94:\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0xb8\n"
        "\tstr r0, [r1]\n"
        "\tldr r0, [r6]\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tsubs r0, r7, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0xb4\n"
        "\tstr r4, [r0]\n"
        "\tldr r2, [r1]\n"
        "\tcmp r4, r2\n"
        "\tbhs .L08060AB0\n"
        "\tadds r0, r1, #0\n"
        ".L08060AB0:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r0", ".L08060ACE", "r8",
        ".L08060AD2", ".L08060AD2", false);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tldr r2, [r6]\n"
        "\tadds r3, r5, #0\n"
        "\tb .L08060AF0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L08060ADC", ".L08060AEC", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        ".L08060AF0:\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r0\n"
        "\tbne .L08060ADC\n"
        "\tadds r5, r3, #0\n"
        "\tldr r1, [sp, #0xb8]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r1", "sb", "r2, r3, r4", "r4, r5, r7", "[r1]",
        ".L08060B12", ".L08060B0E", ".L08060B1A", ".L08060B2A", ".L08060B32", ".L08060B34");

    asm volatile (
        ".syntax unified\n"
        "\tCommitFarmOverviewTransferList r8, r7\n"
        ".L08060B58:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_GET_DMA_QUEUE("r1", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r6, r0, #0\n"
        "\tldr r1, .Loverview_heart_tiles\n"
        "\tmovs r3, #0x58\n"
        "\tlsls r2, r3, #5\n"
        "\tmovs r0, #0xc0\n"
        "\tlsls r0, r0, #0x13\n"
        "\tadds r2, r2, r0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_INIT_COPY(label_copy, "", "", 0x20);

    asm volatile (
        ".syntax unified\n"
        "\tmov sb, sl\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L08060B9C", ".L08060B90",
        "sl", "r4, r5, r7", "[r1]");

    asm volatile (
        ".syntax unified\n"
        "\tb .L08060C64\n"
        "\t.align 2, 0\n"
        ".Loverview_heart_tiles: .4byte gUnk_0875254C\n"
        ".L08060B9C:\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0xc0\n"
        "\tstr r0, [r1]\n"
        "\tldr r0, [r6]\n"
        "\tldr r2, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tsubs r0, r2, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0xbc\n"
        "\tstr r4, [r0]\n"
        "\tldr r2, [r1]\n"
        "\tcmp r4, r2\n"
        "\tbhs .L08060BB8\n"
        "\tadds r0, r1, #0\n"
        ".L08060BB8:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r0", ".L08060BD6", "r8",
        ".L08060BDA", ".L08060BDA", false);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tldr r2, [r6]\n"
        "\tadds r3, r5, #0\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r4\n"
        "\tbeq .L08060C02\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L08060BE8", ".L08060BF8", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r5\n"
        "\tbne .L08060BE8\n"
        ".L08060C02:\n"
        "\tadds r5, r3, #0\n"
        "\tldr r1, [sp, #0xc0]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r1", "sb", "r2, r3, r7", "r4, r5, r7", "[r1]",
        ".L08060C1E", ".L08060C1A", ".L08060C26", ".L08060C36", ".L08060C3E", ".L08060C40");

    asm volatile (
        ".syntax unified\n"
        "\tCommitFarmOverviewTransferList r8, r1\n"
        ".L08060C64:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_GET_DMA_QUEUE("r2", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r6, r0, #0\n"
        "\tldr r1, .Loverview_heart_palette\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_INIT_COPY(label_copy, "", "ldr %0, .L08060CA0", 0x20);

    asm volatile (
        ".syntax unified\n"
        "\tmov sb, sl\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L08060CA4", ".L08060C94",
        "sl", "r3, r4, r5", "[r1]");

    asm volatile (
        ".syntax unified\n"
        "\tb .L08060D68\n"
        "\t.align 2, 0\n"
        ".Loverview_heart_palette: .4byte gUnk_087525CC\n"
        ".L08060CA0: .4byte 0x05000040\n"
        ".L08060CA4:\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0xc8\n"
        "\tstr r0, [r1]\n"
        "\tldr r0, [r6]\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tsubs r0, r7, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0xc4\n"
        "\tstr r4, [r0]\n"
        "\tldr r2, [r1]\n"
        "\tcmp r4, r2\n"
        "\tbhs .L08060CC0\n"
        "\tadds r0, r1, #0\n"
        ".L08060CC0:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r0", ".L08060CDE", "r8",
        ".L08060CE2", ".L08060CE2", false);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tldr r2, [r6]\n"
        "\tadds r3, r5, #0\n"
        "\tb .L08060D00\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L08060CEC", ".L08060CFC", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        ".L08060D00:\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r0\n"
        "\tbne .L08060CEC\n"
        "\tadds r5, r3, #0\n"
        "\tldr r1, [sp, #0xc8]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r1", "sb", "r2, r3, r4", "r4, r5, r7", "[r1]",
        ".L08060D22", ".L08060D1E", ".L08060D2A", ".L08060D3A", ".L08060D42", ".L08060D44");

    asm volatile (
        ".syntax unified\n"
        "\tCommitFarmOverviewTransferList r8, r7\n"
        ".L08060D68:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_GET_DMA_QUEUE("r1", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r6, r0, #0\n"
        "\tldr r1, .L08060DA8\n"
        "\tmovs r3, #0x5c\n"
        "\tlsls r2, r3, #5\n"
        "\tmovs r0, #0xc0\n"
        "\tlsls r0, r0, #0x13\n"
        "\tadds r2, r2, r0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_INIT_COPY(label_copy, "", "", 0x20);

    asm volatile (
        ".syntax unified\n"
        "\tmov sb, sl\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L08060DAC", ".L08060DA0",
        "sl", "r4, r5, r7", "[r1]");

    asm volatile (
        ".syntax unified\n"
        "\tb .L08060E74\n"
        "\t.align 2, 0\n"
        ".L08060DA8: .4byte gUnk_087530CC\n"
        ".L08060DAC:\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0xd0\n"
        "\tstr r0, [r1]\n"
        "\tldr r0, [r6]\n"
        "\tldr r2, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tsubs r0, r2, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0xcc\n"
        "\tstr r4, [r0]\n"
        "\tldr r2, [r1]\n"
        "\tcmp r4, r2\n"
        "\tbhs .L08060DC8\n"
        "\tadds r0, r1, #0\n"
        ".L08060DC8:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r0", ".L08060DE6", "r8",
        ".L08060DEA", ".L08060DEA", false);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tldr r2, [r6]\n"
        "\tadds r3, r5, #0\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r4\n"
        "\tbeq .L08060E12\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L08060DF8", ".L08060E08", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r5\n"
        "\tbne .L08060DF8\n"
        ".L08060E12:\n"
        "\tadds r5, r3, #0\n"
        "\tldr r1, [sp, #0xd0]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r1", "sb", "r2, r3, r7", "r4, r5, r7", "[r1]",
        ".L08060E2E", ".L08060E2A", ".L08060E36", ".L08060E46", ".L08060E4E", ".L08060E50");

    asm volatile (
        ".syntax unified\n"
        "\tCommitFarmOverviewTransferList r8, r1\n"
        ".L08060E74:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_GET_DMA_QUEUE("r2", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r6, r0, #0\n"
        "\tldr r1, .L08060EAC\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_INIT_COPY(label_copy, "", "ldr %0, .L08060EB0", 0x20);

    asm volatile (
        ".syntax unified\n"
        "\tmov sb, sl\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L08060EB4", ".L08060EA4",
        "sl", "r3, r4, r5", "[r1]");

    asm volatile (
        ".syntax unified\n"
        "\tb .L08060F78\n"
        "\t.align 2, 0\n"
        ".L08060EAC: .4byte gUnk_0875314C\n"
        ".L08060EB0: .4byte 0x05000060\n"
        ".L08060EB4:\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0xd8\n"
        "\tstr r0, [r1]\n"
        "\tldr r0, [r6]\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tsubs r0, r7, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0xd4\n"
        "\tstr r4, [r0]\n"
        "\tldr r2, [r1]\n"
        "\tcmp r4, r2\n"
        "\tbhs .L08060ED0\n"
        "\tadds r0, r1, #0\n"
        ".L08060ED0:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r0", ".L08060EEE", "r8",
        ".L08060EF2", ".L08060EF2", false);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tldr r2, [r6]\n"
        "\tadds r3, r5, #0\n"
        "\tb .L08060F10\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L08060EFC", ".L08060F0C", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        ".L08060F10:\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r0\n"
        "\tbne .L08060EFC\n"
        "\tadds r5, r3, #0\n"
        "\tldr r1, [sp, #0xd8]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r1", "sb", "r2, r3, r4", "r4, r5, r7", "[r1]",
        ".L08060F32", ".L08060F2E", ".L08060F3A", ".L08060F4A", ".L08060F52", ".L08060F54");

    asm volatile (
        ".syntax unified\n"
        "\tCommitFarmOverviewTransferList r8, r7\n"
        ".L08060F78:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_GET_DMA_QUEUE("r1", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r6, r0, #0\n"
        "\tldr r1, .L08060FB8\n"
        "\tmovs r3, #0x5d\n"
        "\tlsls r2, r3, #5\n"
        "\tmovs r0, #0xc0\n"
        "\tlsls r0, r0, #0x13\n"
        "\tadds r2, r2, r0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_INIT_COPY(label_copy, "", "", 0x20);

    asm volatile (
        ".syntax unified\n"
        "\tmov sb, sl\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L08060FBC", ".L08060FB0",
        "sl", "r4, r5, r7", "[r1]");

    asm volatile (
        ".syntax unified\n"
        "\tb .L08061084\n"
        "\t.align 2, 0\n"
        ".L08060FB8: .4byte gUnk_0875348C\n"
        ".L08060FBC:\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0xe0\n"
        "\tstr r0, [r1]\n"
        "\tldr r0, [r6]\n"
        "\tldr r2, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tsubs r0, r2, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0xdc\n"
        "\tstr r4, [r0]\n"
        "\tldr r2, [r1]\n"
        "\tcmp r4, r2\n"
        "\tbhs .L08060FD8\n"
        "\tadds r0, r1, #0\n"
        ".L08060FD8:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r0", ".L08060FF6", "r8",
        ".L08060FFA", ".L08060FFA", false);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tldr r2, [r6]\n"
        "\tadds r3, r5, #0\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r4\n"
        "\tbeq .L08061022\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L08061008", ".L08061018", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r5\n"
        "\tbne .L08061008\n"
        ".L08061022:\n"
        "\tadds r5, r3, #0\n"
        "\tldr r1, [sp, #0xe0]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r1", "sb", "r2, r3, r7", "r4, r5, r7", "[r1]",
        ".L0806103E", ".L0806103A", ".L08061046", ".L08061056", ".L0806105E", ".L08061060");

    asm volatile (
        ".syntax unified\n"
        "\tCommitFarmOverviewTransferList r8, r1\n"
        ".L08061084:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_GET_DMA_QUEUE("r2", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r6, r0, #0\n"
        "\tldr r1, .L080610BC\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_INIT_COPY(label_copy, "", "ldr %0, .L080610C0", 0x20);

    asm volatile (
        ".syntax unified\n"
        "\tmov sb, sl\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L080610C4", ".L080610B4",
        "sl", "r3, r4, r5", "[r1]");

    asm volatile (
        ".syntax unified\n"
        "\tb .Lfarm_overview_queue_farmer_portrait\n"
        "\t.align 2, 0\n"
        ".L080610BC: .4byte gUnk_0875350C\n"
        ".L080610C0: .4byte 0x05000080\n"
        ".L080610C4:\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0xe8\n"
        "\tstr r0, [r1]\n"
        "\tldr r0, [r6]\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tsubs r0, r7, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0xe4\n"
        "\tstr r4, [r0]\n"
        "\tldr r2, [r1]\n"
        "\tcmp r4, r2\n"
        "\tbhs .L080610E0\n"
        "\tadds r0, r1, #0\n"
        ".L080610E0:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r0", ".L080610FE", "r8",
        ".L08061102", ".L08061102", false);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tldr r2, [r6]\n"
        "\tadds r3, r5, #0\n"
        "\tb .L08061120\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L0806110C", ".L0806111C", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        ".L08061120:\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r0\n"
        "\tbne .L0806110C\n"
        "\tadds r5, r3, #0\n"
        "\tldr r1, [sp, #0xe8]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r1", "sb", "r2, r3, r4", "r4, r5, r7", "[r1]",
        ".L08061142", ".L0806113E", ".L0806114A", ".L0806115A", ".L08061162", ".L08061164");

    asm volatile (
        ".syntax unified\n"
        "\tCommitFarmOverviewTransferList r8, r7\n"
        "\t@ The farmer row contains the name and collected Power Berries.\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    // Queue the player portrait and lay out the player statistics.

    asm volatile (
        ".syntax unified\n"
        ".Lfarm_overview_queue_farmer_portrait:\n"
        "\tmovs r0, #4\n"
        "\tstr r0, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tmovs r0, #0x20\n"
        "\tmovs r1, #0\n"
        "\tmov r2, sl\n"
        "\tstrh r0, [r2]\n"
        "\tstr r1, [r2, #4]\n"
        "\tldr r0, .Loverview_first_row_icon_tiles\n"
        "\tstr r0, [r2, #8]\n"
        "\tldr r0, .Loverview_first_row_icon_palette\n"
        "\tstr r0, [r2, #0xc]\n"
        "\tldr r3, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r4, .L080611D0\n"
        "\tadds r6, r3, r4\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L080611D4", ".L080611BC",
        "sl", "r2, r5, r7", "[sp, #0x44]");

    asm volatile (
        ".syntax unified\n"
        "\tmovs r3, #7\n"
        "\tstr r3, [sp, #.Lfarm_overview_sp_content_tile_rows]\n"
        "\tb .L080612AC\n"
        "\t.align 2, 0\n"
        ".Loverview_first_row_icon_tiles: .4byte gUnk_08752F8C\n"
        ".Loverview_first_row_icon_palette: .4byte gUnk_0875300C\n"
        ".L080611D0: .4byte .Lfarm_overview_portrait_uploads\n"
        ".L080611D4:\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0xf0\n"
        "\tstr r0, [r1]\n"
        "\tldr r3, [r6]\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tsubs r0, r4, r3\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0xec\n"
        "\tstr r4, [r0]\n"
        "\tldr r2, [r1]\n"
        "\tmov sb, r1\n"
        "\tadds r1, r3, #0\n"
        "\tcmp r4, r2\n"
        "\tbhs .L080611F4\n"
        "\tmov r0, sb\n"
        ".L080611F4:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r0", ".L08061214", "r8",
        ".L08061210", ".L08061218", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadds r3, r1, #0\n"
        "\tadds r2, r5, #0\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tadds r5, #3\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_content_tile_rows]\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r3, r7\n"
        "\tbeq .L08061246\n"
        ".L0806122C:\n"
        "\tcmp r2, #0\n"
        "\tbeq .L0806123C\n"
        "\tadds r0, r2, #0\n"
        "\tadds r1, r3, #0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_COMMAND("r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        ".L0806123C:\n"
        "\tadds r3, #0x10\n"
        "\tadds r2, #0x10\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r3, r0\n"
        "\tbne .L0806122C\n"
        ".L08061246:\n"
        "\tadds r5, r2, #0\n"
        "\tmov r2, sb\n"
        "\tldr r1, [r2]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r1", "sl", "r3, r4, r7", "r4, r5, r7", "[sp, #0x44]",
        ".L08061264", ".L08061260", ".L0806126C", ".L0806127C", ".L08061284", ".L08061286");

    asm volatile (
        ".syntax unified\n"
        "\tldr r2, [r6, #4]\n"
        "\tldr r1, [r6]\n"
        "\tadds r0, r1, #0\n"
        "\tcmp r1, r2\n"
        "\tbeq .L08061296\n"
        ".L08061290:\n"
        "\tadds r0, #0x10\n"
        "\tcmp r0, r2\n"
        "\tbne .L08061290\n"
        ".L08061296:\n"
        "\tcmp r1, #0\n"
        "\tbeq .L080612A0\n"
        "\tadds r0, r1, #0\n"
        "\tbl free\n"
        ".L080612A0:\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadd r0, r8\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tstr r1, [r6]\n"
        "\tstr r5, [r6, #4]\n"
        "\tstr r0, [r6, #0xc]\n"
        ".L080612AC:\n"
        "\tldr r2, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r3, .L08061334\n"
        "\tadds r0, r2, r3\n"
        "\tldr r0, [r0]\n"
        "\tldr r4, .L08061338\n"
        "\tadds r0, r0, r4\n"
        "\tldr r0, [r0]\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r1, [r5, #.Lfarm_overview_panel_tilemap]\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r4, r7, #6\n"
        "\tadds r1, r1, r4\n"
        "\tadds r1, #0x2c\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_DRAW_COUNT("r5");

    asm volatile (
        ".syntax unified\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r0, [r1, #.Lfarm_overview_panel_tilemap]\n"
        "\tadds r0, r0, r4\n"
        "\tadds r0, #0x2e\n"
        "\tldr r2, [sp, #0x1f0]\n"
        "\tlsrs r1, r2, #0x10\n"
        "\tmovs r4, #0\n"
        "\tstr r4, [sp]\n"
        "\tstr r5, [sp, #4]\n"
        "\tmovs r2, #2\n"
        "\tmovs r3, #2\n"
        "\tbl FillMessageTilemapRect\n"
        "\tldr r3, [sp, #.Lfarm_overview_sp_content_tile_rows]\n"
        "\tstr r3, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r0, r3, #3\n"
        "\tmov r5, sl\n"
        "\tstrh r0, [r5]\n"
        "\tstr r4, [r5, #4]\n"
        "\tldr r0, .Loverview_farmer_portrait_tiles\n"
        "\tstr r0, [r5, #8]\n"
        "\tldr r0, .Loverview_farmer_portrait_palette\n"
        "\tstr r0, [r5, #0xc]\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r0, .L08061344\n"
        "\tadds r6, r7, r0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L08061348", ".L08061320",
        "sl", "r2, r3, r4", "[sp, #0x44]");

    asm volatile (
        ".syntax unified\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r5, r5, #6\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_portrait_tilemap_offset]\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tadds r7, #4\n"
        "\tstr r7, [sp, #.Lfarm_overview_sp_portrait_lower_tile_row]\n"
        "\tb .Lfarm_overview_draw_farmer_name\n"
        "\t.align 2, 0\n"
        ".L08061334: .4byte .Lfarm_overview_game_data\n"
        ".L08061338: .4byte 0x00001AA8\n"
        ".Loverview_farmer_portrait_tiles: .4byte gUnk_087518CC\n"
        ".Loverview_farmer_portrait_palette: .4byte gUnk_0875194C\n"
        ".L08061344: .4byte .Lfarm_overview_portrait_uploads\n"
        ".L08061348:\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0xf8\n"
        "\tstr r0, [r1]\n"
        "\tldr r3, [r6]\n"
        "\tldr r2, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tsubs r0, r2, r3\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0xf4\n"
        "\tstr r4, [r0]\n"
        "\tldr r2, [r1]\n"
        "\tmov sb, r1\n"
        "\tadds r1, r3, #0\n"
        "\tcmp r4, r2\n"
        "\tbhs .L08061368\n"
        "\tmov r0, sb\n"
        ".L08061368:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r0", ".L08061388", "r8",
        ".L08061384", ".L0806138C", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadds r3, r1, #0\n"
        "\tadds r2, r5, #0\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r4, r4, #6\n"
        "\tstr r4, [sp, #.Lfarm_overview_sp_portrait_tilemap_offset]\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tadds r5, #4\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_portrait_lower_tile_row]\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r3, r7\n"
        "\tbeq .L080613C0\n"
        ".L080613A6:\n"
        "\tcmp r2, #0\n"
        "\tbeq .L080613B6\n"
        "\tadds r0, r2, #0\n"
        "\tadds r1, r3, #0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_COMMAND("r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        ".L080613B6:\n"
        "\tadds r3, #0x10\n"
        "\tadds r2, #0x10\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r3, r0\n"
        "\tbne .L080613A6\n"
        ".L080613C0:\n"
        "\tadds r5, r2, #0\n"
        "\tmov r2, sb\n"
        "\tldr r1, [r2]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r1", "sl", "r3, r4, r7", "r4, r5, r7", "[sp, #0x44]",
        ".L080613DE", ".L080613DA", ".L080613E6", ".L080613F6", ".L080613FE", ".L08061400");

    asm volatile (
        ".syntax unified\n"
        "\tldr r2, [r6, #4]\n"
        "\tldr r1, [r6]\n"
        "\tadds r0, r1, #0\n"
        "\tcmp r1, r2\n"
        "\tbeq .L08061410\n"
        ".L0806140A:\n"
        "\tadds r0, #0x10\n"
        "\tcmp r0, r2\n"
        "\tbne .L0806140A\n"
        ".L08061410:\n"
        "\tcmp r1, #0\n"
        "\tbeq .L0806141A\n"
        "\tadds r0, r1, #0\n"
        "\tbl free\n"
        ".L0806141A:\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadd r0, r8\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tstr r1, [r6]\n"
        "\tstr r5, [r6, #4]\n"
        "\tstr r0, [r6, #0xc]\n"
        ".Lfarm_overview_draw_farmer_name:\n"
        "\tmovs r4, #0xc\n"
        "\tmovs r5, #2\n"
        "\tadd r0, sp, #0xfc\n"
        "\tstrh r4, [r0]\n"
        "\tldr r2, [sp, #.Loverview_sp_farmer_name_fill_bounds]\n"
        "\tstrh r5, [r2, #2]\n"
        "\tldr r0, [r0]\n"
        "\tldr r1, [sp, #.Loverview_sp_farmer_name_pixels]\n"
        "\tmovs r2, #0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_FILL_NAME_BITMAP();

    asm volatile (
        ".syntax unified\n"
        "\tldr r3, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r7, .L080614A0\n"
        "\tadds r0, r3, r7\n"
        "\tldr r0, [r0]\n"
        "\tldr r1, .L080614A4\n"
        "\tadds r0, r0, r1\n"
        "\tbl GetFarmerName\n"
        "\tadds r1, r0, #0\n"
        "\tadd r0, sp, #0x100\n"
        "\tstrh r4, [r0]\n"
        "\tldr r2, [sp, #.Loverview_sp_farmer_name_bounds]\n"
        "\tstrh r5, [r2, #2]\n"
        "\tldr r0, [r0]\n"
        "\tstr r1, [sp]\n"
        "\tldr r1, [sp, #.Loverview_sp_farmer_name_pixels]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_DRAW_LABEL_OR_NAME();

    OVERVIEW_GET_DMA_QUEUE("r3", true);

    asm volatile (
        ".syntax unified\n"
        "\tmovs r3, #0xc0\n"
        "\tlsls r3, r3, #2\n"
        "\tadds r6, r0, #0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_INIT_COPY(name_copy, "ldr %0, [sp, #.Loverview_sp_farmer_name_pixels]", "ldr %0, .L080614A8", 0);

    asm volatile (
        ".syntax unified\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_name_copy_command_ptr]\n"
        "\tmov sb, r4\n"
        "\tldr r1, [r6, #4]\n"
        "\tldr r0, [r6, #0xc]\n"
        "\tcmp r1, r0\n"
        "\tbeq .L080614AC\n"
        "\tcmp r1, #0\n"
        "\tbeq .L08061498\n"
        "\tadds r0, r1, #0\n"
        "\tadds r1, r4, #0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_COMMAND("r2, r5, r7");

    asm volatile (
        ".syntax unified\n"
        ".L08061498:\n"
        "\tldr r0, [r6, #4]\n"
        "\tadds r0, #0x10\n"
        "\tstr r0, [r6, #4]\n"
        "\tb .L08061578\n"
        "\t.align 2, 0\n"
        ".L080614A0: .4byte .Lfarm_overview_game_data\n"
        ".L080614A4: .4byte 0x00001BD8\n"
        ".L080614A8: .4byte 0x06003000\n"
        ".L080614AC:\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tmovs r0, #1\n"
        "\tadd r2, sp, #0x108\n"
        "\tstr r0, [r2]\n"
        "\tldr r0, [r6]\n"
        "\tldr r3, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tsubs r0, r3, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r1, sp, #0x104\n"
        "\tstr r4, [r1]\n"
        "\tadds r3, r2, #0\n"
        "\tldr r0, [r2]\n"
        "\tcmp r4, r0\n"
        "\tbhs .L080614CA\n"
        "\tadds r1, r3, #0\n"
        ".L080614CA:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r1", ".L080614E8", "r8",
        ".L080614EC", ".L080614EC", false);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tldr r2, [r6]\n"
        "\tadds r3, r5, #0\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r4\n"
        "\tbeq .L08061514\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L080614FA", ".L0806150A", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r5\n"
        "\tbne .L080614FA\n"
        ".L08061514:\n"
        "\tadds r5, r3, #0\n"
        "\tadd r0, sp, #0x108\n"
        "\tldr r0, [r0]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r0", "sb", "r2, r3, r7", "r4, r5, r7", "[r1]",
        ".L08061532", ".L0806152E", ".L0806153A", ".L0806154A", ".L08061552", ".L08061554");

    asm volatile (
        ".syntax unified\n"
        "\tCommitFarmOverviewTransferList r8, r1\n"
        ".L08061578:\n"
        "\tldr r2, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r0, [r2, #.Lfarm_overview_panel_tilemap]\n"
        "\tldr r3, [sp, #.Lfarm_overview_sp_portrait_tilemap_offset]\n"
        "\tadds r0, r0, r3\n"
        "\tadds r0, #4\n"
        "\tmovs r1, #0xc0\n"
        "\tlsls r1, r1, #1\n"
        "\tmovs r2, #0\n"
        "\tstr r2, [sp]\n"
        "\tmovs r6, #0x20\n"
        "\tstr r6, [sp, #4]\n"
        "\tmovs r2, #0xc\n"
        "\tmovs r3, #2\n"
        "\tbl FillMessageTilemapRect\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r7, .L08061658\n"
        "\tadds r4, r5, r7\n"
        "\tldr r0, [r4]\n"
        "\tldr r5, .L0806165C\n"
        "\tadds r0, r0, r5\n"
        "\tbl func_0800E53C\n"
        "\tlsls r0, r0, #0x18\n"
        "\tcmp r0, #0\n"
        "\tbeq .L080615CA\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tadds r1, #2\n"
        "\tldr r2, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r0, [r2, #.Lfarm_overview_panel_tilemap]\n"
        "\tlsls r1, r1, #6\n"
        "\tadds r0, r0, r1\n"
        "\tadds r0, #4\n"
        "\tmovs r1, #0x5d\n"
        "\tmovs r2, #4\n"
        "\tstr r2, [sp]\n"
        "\tstr r6, [sp, #4]\n"
        "\tmovs r2, #1\n"
        "\tmovs r3, #1\n"
        "\tbl FillMessageTilemapRect\n"
        ".L080615CA:\n"
        "\tldr r0, [r4]\n"
        "\tadds r0, r0, r5\n"
        "\tbl func_0800E530\n"
        "\tadds r6, r0, #0\n"
        "\tmovs r4, #0\n"
        "\tcmp r4, r6\n"
        "\tbhs .Lfarm_overview_check_for_spouse\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tadds r0, #2\n"
        "\tlsls r0, r0, #6\n"
        "\tmov r8, r0\n"
        "\tmovs r5, #8\n"
        "\tmovs r3, #0x5c\n"
        "\tlsls r7, r3, #0x10\n"
        ".L080615E8:\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r0, [r1, #.Lfarm_overview_panel_tilemap]\n"
        "\tadd r0, r8\n"
        "\tadds r0, r0, r5\n"
        "\tmovs r1, #3\n"
        "\tstr r1, [sp]\n"
        "\tmovs r1, #0x20\n"
        "\tstr r1, [sp, #4]\n"
        "\tlsrs r1, r7, #0x10\n"
        "\tmovs r2, #1\n"
        "\tmovs r3, #1\n"
        "\tbl FillMessageTilemapRect\n"
        "\tadds r5, #2\n"
        "\tadds r4, #1\n"
        "\tcmp r4, r6\n"
        "\tblo .L080615E8\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    // The spouse and active NPC each add an optional portrait row.

    asm volatile (
        ".syntax unified\n"
        ".Lfarm_overview_check_for_spouse:\n"
        "\tldr r2, [sp, #.Lfarm_overview_sp_portrait_lower_tile_row]\n"
        "\tstr r2, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tldr r3, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r5, .L08061658\n"
        "\tadds r4, r3, r5\n"
        "\tldr r0, [r4]\n"
        "\tldr r7, .L08061660\n"
        "\tadds r0, r0, r7\n"
        "\tbl HasRosterEntry\n"
        "\tlsls r0, r0, #0x18\n"
        "\tcmp r0, #0\n"
        "\tbne .Lfarm_overview_select_spouse_portrait\n"
        "\tbl .Lfarm_overview_dog_row\n"
        "\t@ Only spouse NPC IDs with a matching portrait get a row.\n"
        ".Lfarm_overview_select_spouse_portrait:\n"
        "\tldr r0, [r4]\n"
        "\tadds r1, r7, #0\n"
        "\tadds r0, r0, r1\n"
        "\tbl GetRosterMarriageCandidate\n"
        "\tstr r0, [sp, #.Lfarm_overview_sp_spouse_id]\n"
        "\tldr r0, [r4]\n"
        "\tadds r2, r7, #0\n"
        "\tadds r0, r0, r2\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_spouse_id]\n"
        "\tbl GetRosterBacheloretteById\n"
        "\tstr r0, [sp, #.Lfarm_overview_sp_spouse]\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_spouse_id]\n"
        "\tsubs r0, #3\n"
        "\tcmp r0, #0x1c\n"
        "\tbls .L0806164C\n"
        "\tb .Lfarm_overview_skip_unsupported_spouse\n"
        ".L0806164C:\n"
        "\tlsls r0, r0, #2\n"
        "\tldr r1, .L08061664\n"
        "\tadds r0, r0, r1\n"
        "\tldr r0, [r0]\n"
        "\tmov pc, r0\n"
        "\t.align 2, 0\n"
        ".L08061658: .4byte .Lfarm_overview_game_data\n"
        ".L0806165C: .4byte 0x00001BD8\n"
        ".L08061660: .4byte 0x00001CD4\n"
        ".L08061664: .4byte .Lfarm_overview_spouse_portrait_table\n"
        ".Lfarm_overview_spouse_portrait_table: @ NPC ID minus 3\n"
        "\t.4byte .L080616DC @ case 0\n"
        "\t.4byte .Lfarm_overview_skip_unsupported_spouse @ case 1\n"
        "\t.4byte .Lfarm_overview_skip_unsupported_spouse @ case 2\n"
        "\t.4byte .Lfarm_overview_skip_unsupported_spouse @ case 3\n"
        "\t.4byte .Lfarm_overview_skip_unsupported_spouse @ case 4\n"
        "\t.4byte .Lfarm_overview_skip_unsupported_spouse @ case 5\n"
        "\t.4byte .Lfarm_overview_skip_unsupported_spouse @ case 6\n"
        "\t.4byte .Lfarm_overview_skip_unsupported_spouse @ case 7\n"
        "\t.4byte .Lfarm_overview_skip_unsupported_spouse @ case 8\n"
        "\t.4byte .L080616F0 @ case 9\n"
        "\t.4byte .Lfarm_overview_skip_unsupported_spouse @ case 10\n"
        "\t.4byte .Lfarm_overview_skip_unsupported_spouse @ case 11\n"
        "\t.4byte .Lfarm_overview_skip_unsupported_spouse @ case 12\n"
        "\t.4byte .Lfarm_overview_skip_unsupported_spouse @ case 13\n"
        "\t.4byte .Lfarm_overview_skip_unsupported_spouse @ case 14\n"
        "\t.4byte .Lfarm_overview_skip_unsupported_spouse @ case 15\n"
        "\t.4byte .L08061704 @ case 16\n"
        "\t.4byte .Lfarm_overview_skip_unsupported_spouse @ case 17\n"
        "\t.4byte .L0806172C @ case 18\n"
        "\t.4byte .Lfarm_overview_skip_unsupported_spouse @ case 19\n"
        "\t.4byte .Lfarm_overview_skip_unsupported_spouse @ case 20\n"
        "\t.4byte .Lfarm_overview_skip_unsupported_spouse @ case 21\n"
        "\t.4byte .L08061718 @ case 22\n"
        "\t.4byte .Lfarm_overview_skip_unsupported_spouse @ case 23\n"
        "\t.4byte .Lfarm_overview_skip_unsupported_spouse @ case 24\n"
        "\t.4byte .Lfarm_overview_skip_unsupported_spouse @ case 25\n"
        "\t.4byte .Lfarm_overview_skip_unsupported_spouse @ case 26\n"
        "\t.4byte .Lfarm_overview_skip_unsupported_spouse @ case 27\n"
        "\t.4byte .L08061740 @ case 28\n"
        ".L080616DC:\n"
        "\tldr r0, .L080616E8\n"
        "\tmov r3, sl\n"
        "\tstr r0, [r3, #8]\n"
        "\tldr r0, .L080616EC\n"
        "\tstr r0, [r3, #0xc]\n"
        "\tb .L08061758\n"
        "\t.align 2, 0\n"
        ".L080616E8: .4byte gUnk_08751A0C\n"
        ".L080616EC: .4byte gUnk_08751A8C\n"
        ".L080616F0:\n"
        "\tldr r0, .L080616FC\n"
        "\tmov r4, sl\n"
        "\tstr r0, [r4, #8]\n"
        "\tldr r0, .L08061700\n"
        "\tstr r0, [r4, #0xc]\n"
        "\tb .L08061758\n"
        "\t.align 2, 0\n"
        ".L080616FC: .4byte gUnk_08752E4C\n"
        ".L08061700: .4byte gUnk_08752ECC\n"
        ".L08061704:\n"
        "\tldr r0, .L08061710\n"
        "\tmov r5, sl\n"
        "\tstr r0, [r5, #8]\n"
        "\tldr r0, .L08061714\n"
        "\tstr r0, [r5, #0xc]\n"
        "\tb .L08061758\n"
        "\t.align 2, 0\n"
        ".L08061710: .4byte gUnk_087524AC\n"
        ".L08061714: .4byte gUnk_0875252C\n"
        ".L08061718:\n"
        "\tldr r0, .L08061724\n"
        "\tmov r7, sl\n"
        "\tstr r0, [r7, #8]\n"
        "\tldr r0, .L08061728\n"
        "\tstr r0, [r7, #0xc]\n"
        "\tb .L08061758\n"
        "\t.align 2, 0\n"
        ".L08061724: .4byte gUnk_0875286C\n"
        ".L08061728: .4byte gUnk_087528EC\n"
        ".L0806172C:\n"
        "\tldr r0, .L08061738\n"
        "\tmov r1, sl\n"
        "\tstr r0, [r1, #8]\n"
        "\tldr r0, .L0806173C\n"
        "\tstr r0, [r1, #0xc]\n"
        "\tb .L08061758\n"
        "\t.align 2, 0\n"
        ".L08061738: .4byte gUnk_0875302C\n"
        ".L0806173C: .4byte gUnk_087530AC\n"
        ".L08061740:\n"
        "\tldr r0, .L0806174C\n"
        "\tmov r2, sl\n"
        "\tstr r0, [r2, #8]\n"
        "\tldr r0, .L08061750\n"
        "\tstr r0, [r2, #0xc]\n"
        "\tb .L08061758\n"
        "\t.align 2, 0\n"
        ".L0806174C: .4byte gUnk_0875240C\n"
        ".L08061750: .4byte gUnk_0875248C\n"
        ".Lfarm_overview_skip_unsupported_spouse:\n"
        "\tmovs r3, #0\n"
        "\tstr r3, [sp, #.Lfarm_overview_sp_spouse]\n"
        ".L08061758:\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tadds r4, #5\n"
        "\tstr r4, [sp, #.Lfarm_overview_sp_dog_tile_row]\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_spouse]\n"
        "\tcmp r5, #0\n"
        "\tbne .L08061766\n"
        "\tb .Lfarm_overview_find_active_npc\n"
        ".L08061766:\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r0, r7, #3\n"
        "\tmovs r1, #0\n"
        "\tmov r2, sl\n"
        "\tstrh r0, [r2]\n"
        "\tstr r1, [r2, #4]\n"
        "\tldr r3, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r4, .L080617C8\n"
        "\tadds r6, r3, r4\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L080617D0", ".L08061790",
        "sl", "r2, r5, r7", "[sp, #0x44]");

    asm volatile (
        ".syntax unified\n"
        "\tldr r3, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r3, r3, #6\n"
        "\tstr r3, [sp, #.Lfarm_overview_sp_portrait_tilemap_offset]\n"
        "\tmovs r4, #0x8a\n"
        "\tlsls r4, r4, #1\n"
        "\tadd r4, sp\n"
        "\tstr r4, [sp, #0x250]\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r7, .L080617CC\n"
        "\tadds r5, r5, r7\n"
        "\tmov sb, r5\n"
        "\tmovs r0, #0x8c\n"
        "\tlsls r0, r0, #1\n"
        "\tadd r0, sp\n"
        "\tstr r0, [sp, #0x254]\n"
        "\tmovs r1, #0x92\n"
        "\tlsls r1, r1, #1\n"
        "\tadd r1, sp\n"
        "\tstr r1, [sp, #0x258]\n"
        "\tmovs r2, #0x94\n"
        "\tlsls r2, r2, #1\n"
        "\tadd r2, sp\n"
        "\tstr r2, [sp, #0x25c]\n"
        "\tb .L080618D8\n"
        "\t.align 2, 0\n"
        ".L080617C8: .4byte .Lfarm_overview_portrait_uploads\n"
        ".L080617CC: .4byte 0x00000C5C\n"
        ".L080617D0:\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0x110\n"
        "\tstr r0, [r1]\n"
        "\tldr r2, [r6]\n"
        "\tldr r3, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tsubs r0, r3, r2\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0x10c\n"
        "\tstr r4, [r0]\n"
        "\tadds r5, r1, #0\n"
        "\tldr r0, [r1]\n"
        "\tadd r3, sp, #0x10c\n"
        "\tadds r1, r2, #0\n"
        "\tcmp r4, r0\n"
        "\tbhs .L080617F2\n"
        "\tadds r3, r5, #0\n"
        ".L080617F2:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r3", ".L08061812", "r8",
        ".L0806180E", ".L08061816", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadds r2, r1, #0\n"
        "\tadds r3, r5, #0\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r4, r4, #6\n"
        "\tstr r4, [sp, #.Lfarm_overview_sp_portrait_tilemap_offset]\n"
        "\tmovs r5, #0x8a\n"
        "\tlsls r5, r5, #1\n"
        "\tadd r5, sp\n"
        "\tstr r5, [sp, #0x250]\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r0, .L08061854\n"
        "\tadds r7, r7, r0\n"
        "\tmov sb, r7\n"
        "\tmovs r1, #0x8c\n"
        "\tlsls r1, r1, #1\n"
        "\tadd r1, sp\n"
        "\tstr r1, [sp, #0x254]\n"
        "\tmovs r4, #0x92\n"
        "\tlsls r4, r4, #1\n"
        "\tadd r4, sp\n"
        "\tstr r4, [sp, #0x258]\n"
        "\tmovs r5, #0x94\n"
        "\tlsls r5, r5, #1\n"
        "\tadd r5, sp\n"
        "\tstr r5, [sp, #0x25c]\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tadds r7, #5\n"
        "\tstr r7, [sp, #.Lfarm_overview_sp_dog_tile_row]\n"
        "\tb .L0806186C\n"
        "\t.align 2, 0\n"
        ".L08061854: .4byte 0x00000C5C\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L08061858", ".L08061868", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        ".L0806186C:\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r0\n"
        "\tbne .L08061858\n"
        "\tadds r5, r3, #0\n"
        "\tadd r0, sp, #0x110\n"
        "\tldr r0, [r0]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r0", "sl", "r2, r3, r4", "r4, r5, r7", "[sp, #0x44]",
        ".L08061890", ".L0806188C", ".L08061898", ".L080618A8", ".L080618B0", ".L080618B2");

    asm volatile (
        ".syntax unified\n"
        "\tldr r3, [r6, #4]\n"
        "\tldr r1, [r6]\n"
        "\tadds r0, r1, #0\n"
        "\tcmp r1, r3\n"
        "\tbeq .L080618C2\n"
        ".L080618BC:\n"
        "\tadds r0, #0x10\n"
        "\tcmp r0, r3\n"
        "\tbne .L080618BC\n"
        ".L080618C2:\n"
        "\tcmp r1, #0\n"
        "\tbeq .L080618CC\n"
        "\tadds r0, r1, #0\n"
        "\tbl free\n"
        ".L080618CC:\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadd r0, r8\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tstr r7, [r6]\n"
        "\tstr r5, [r6, #4]\n"
        "\tstr r0, [r6, #0xc]\n"
        ".L080618D8:\n"
        "\tmovs r4, #0xc\n"
        "\tmovs r5, #2\n"
        "\tadd r0, sp, #0x114\n"
        "\tstrh r4, [r0]\n"
        "\tldr r1, [sp, #0x250]\n"
        "\tstrh r5, [r1, #2]\n"
        "\tldr r0, [r0]\n"
        "\tmov r1, sb\n"
        "\tmovs r2, #0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_FILL_NAME_BITMAP();

    asm volatile (
        ".syntax unified\n"
        "\tldr r2, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r3, .L08061954\n"
        "\tadds r0, r2, r3\n"
        "\tldr r0, [r0]\n"
        "\tldr r7, .L08061958\n"
        "\tadds r0, r0, r7\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_spouse_id]\n"
        "\tbl GetRosterNpcName\n"
        "\tadds r1, r0, #0\n"
        "\tadd r0, sp, #0x118\n"
        "\tstrh r4, [r0]\n"
        "\tldr r2, [sp, #0x254]\n"
        "\tstrh r5, [r2, #2]\n"
        "\tldr r0, [r0]\n"
        "\tstr r1, [sp]\n"
        "\tmov r1, sb\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_DRAW_LABEL_OR_NAME();

    OVERVIEW_GET_DMA_QUEUE("r3", true);

    asm volatile (
        ".syntax unified\n"
        "\tmovs r3, #0xc0\n"
        "\tlsls r3, r3, #2\n"
        "\tadds r6, r0, #0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_INIT_COPY(name_copy, "mov %0, sb", "ldr %0, .L0806195C", 0);

    asm volatile (
        ".syntax unified\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_name_copy_command_ptr]\n"
        "\tmov sb, r4\n"
        "\tldr r1, [r6, #4]\n"
        "\tldr r0, [r6, #0xc]\n"
        "\tcmp r1, r0\n"
        "\tbeq .L08061960\n"
        "\tcmp r1, #0\n"
        "\tbeq .L0806194C\n"
        "\tadds r0, r1, #0\n"
        "\tadds r1, r4, #0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_COMMAND("r2, r5, r7");

    asm volatile (
        ".syntax unified\n"
        ".L0806194C:\n"
        "\tldr r0, [r6, #4]\n"
        "\tadds r0, #0x10\n"
        "\tstr r0, [r6, #4]\n"
        "\tb .L08061A2E\n"
        "\t.align 2, 0\n"
        ".L08061954: .4byte .Lfarm_overview_game_data\n"
        ".L08061958: .4byte 0x00001CD4\n"
        ".L0806195C: .4byte 0x06003300\n"
        ".L08061960:\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0x120\n"
        "\tstr r0, [r1]\n"
        "\tldr r0, [r6]\n"
        "\tldr r3, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tsubs r0, r3, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0x11c\n"
        "\tstr r4, [r0]\n"
        "\tadds r2, r1, #0\n"
        "\tldr r0, [r1]\n"
        "\tadd r1, sp, #0x11c\n"
        "\tcmp r4, r0\n"
        "\tbhs .L08061980\n"
        "\tadds r1, r2, #0\n"
        ".L08061980:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r1", ".L0806199E", "r8",
        ".L080619A2", ".L080619A2", false);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tldr r2, [r6]\n"
        "\tadds r3, r5, #0\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r4\n"
        "\tbeq .L080619CA\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L080619B0", ".L080619C0", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r5\n"
        "\tbne .L080619B0\n"
        ".L080619CA:\n"
        "\tadds r5, r3, #0\n"
        "\tadd r0, sp, #0x120\n"
        "\tldr r0, [r0]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r0", "sb", "r2, r3, r7", "r4, r5, r7", "[r1]",
        ".L080619E8", ".L080619E4", ".L080619F0", ".L08061A00", ".L08061A08", ".L08061A0A");

    asm volatile (
        ".syntax unified\n"
        "\tCommitFarmOverviewTransferList r8, r1\n"
        ".L08061A2E:\n"
        "\tldr r2, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r0, [r2, #.Lfarm_overview_panel_tilemap]\n"
        "\tldr r3, [sp, #.Lfarm_overview_sp_portrait_tilemap_offset]\n"
        "\tadds r0, r0, r3\n"
        "\tadds r0, #4\n"
        "\tmovs r1, #0xcc\n"
        "\tlsls r1, r1, #1\n"
        "\tmovs r2, #0\n"
        "\tstr r2, [sp]\n"
        "\tmovs r2, #0x20\n"
        "\tstr r2, [sp, #4]\n"
        "\tmovs r2, #0xc\n"
        "\tmovs r3, #2\n"
        "\tbl FillMessageTilemapRect\n"
        "\tmovs r0, #0xa\n"
        "\tadd r4, sp, #0x124\n"
        "\tstr r0, [r4]\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_spouse]\n"
        "\tbl GetLove__C12Bachelorette\n"
        "\tldr r1, .L08061B28\n"
        "\tbl __udivsi3\n"
        "\tadd r1, sp, #0x128\n"
        "\tstr r0, [r1]\n"
        "\tldr r1, [r4]\n"
        "\tldr r2, [sp, #0x258]\n"
        "\tcmp r1, r0\n"
        "\tbls .L08061A6C\n"
        "\tldr r2, [sp, #0x25c]\n"
        ".L08061A6C:\n"
        "\tldr r6, [r2]\n"
        "\tmovs r4, #0\n"
        "\tcmp r4, r6\n"
        "\tbhs .Lfarm_overview_find_active_npc\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tadds r0, #2\n"
        "\tlsls r0, r0, #6\n"
        "\tmov r8, r0\n"
        "\tmovs r5, #8\n"
        "\tmovs r0, #0x58\n"
        "\tlsls r7, r0, #0x10\n"
        ".L08061A82:\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r0, [r1, #.Lfarm_overview_panel_tilemap]\n"
        "\tadd r0, r8\n"
        "\tadds r0, r0, r5\n"
        "\tmovs r1, #2\n"
        "\tstr r1, [sp]\n"
        "\tmovs r1, #0x20\n"
        "\tstr r1, [sp, #4]\n"
        "\tlsrs r1, r7, #0x10\n"
        "\tmovs r2, #1\n"
        "\tmovs r3, #1\n"
        "\tbl FillMessageTilemapRect\n"
        "\tadds r5, #2\n"
        "\tadds r4, #1\n"
        "\tcmp r4, r6\n"
        "\tblo .L08061A82\n"
        "\t@ The active NPC has a separate portrait, name, and friendship meter.\n"
        ".Lfarm_overview_find_active_npc:\n"
        "\tldr r2, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r3, .L08061B2C\n"
        "\tadds r0, r2, r3\n"
        "\tldr r0, [r0]\n"
        "\tldr r4, .L08061B30\n"
        "\tadds r0, r0, r4\n"
        "\tbl GetActiveRosterNpc\n"
        "\tstr r0, [sp, #.Lfarm_overview_sp_active_npc]\n"
        "\tcmp r0, #0\n"
        "\tbne .Lfarm_overview_queue_active_npc_portrait\n"
        "\tb .Lfarm_overview_set_dog_row\n"
        ".Lfarm_overview_queue_active_npc_portrait:\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r0, r5, #3\n"
        "\tmov r7, sl\n"
        "\tstrh r0, [r7]\n"
        "\tmovs r0, #0x78\n"
        "\tstr r0, [r7, #4]\n"
        "\tldr r0, .Loverview_active_npc_portrait_tiles\n"
        "\tstr r0, [r7, #8]\n"
        "\tldr r0, .Loverview_active_npc_portrait_palette\n"
        "\tstr r0, [r7, #0xc]\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r1, .L08061B3C\n"
        "\tadds r6, r0, r1\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L08061B40", ".L08061AEE",
        "sl", "r2, r3, r4", "[sp, #0x44]");

    asm volatile (
        ".syntax unified\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r5, r5, #6\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_portrait_tilemap_offset]\n"
        "\tmovs r7, #0x9a\n"
        "\tlsls r7, r7, #1\n"
        "\tadd r7, sp\n"
        "\tstr r7, [sp, #0x264]\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_report]\n"
        "\tmovs r1, #0xf6\n"
        "\tlsls r1, r1, #4\n"
        "\tadds r0, r0, r1\n"
        "\tmov sb, r0\n"
        "\tmovs r2, #0x9c\n"
        "\tlsls r2, r2, #1\n"
        "\tadd r2, sp\n"
        "\tstr r2, [sp, #0x268]\n"
        "\tmovs r3, #0xa2\n"
        "\tlsls r3, r3, #1\n"
        "\tadd r3, sp\n"
        "\tstr r3, [sp, #0x26c]\n"
        "\tmovs r4, #0xa4\n"
        "\tlsls r4, r4, #1\n"
        "\tadd r4, sp\n"
        "\tstr r4, [sp, #0x270]\n"
        "\tb .L08061C44\n"
        "\t.align 2, 0\n"
        ".L08061B28: .4byte 0x00001999\n"
        ".L08061B2C: .4byte .Lfarm_overview_game_data\n"
        ".L08061B30: .4byte 0x00001CD4\n"
        ".Loverview_active_npc_portrait_tiles: .4byte gUnk_08751AAC\n"
        ".Loverview_active_npc_portrait_palette: .4byte gUnk_08751B2C\n"
        ".L08061B3C: .4byte .Lfarm_overview_portrait_uploads\n"
        ".L08061B40:\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0x130\n"
        "\tstr r0, [r1]\n"
        "\tldr r2, [r6]\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tsubs r0, r5, r2\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0x12c\n"
        "\tstr r4, [r0]\n"
        "\tadds r5, r1, #0\n"
        "\tldr r0, [r1]\n"
        "\tadd r3, sp, #0x12c\n"
        "\tadds r1, r2, #0\n"
        "\tcmp r4, r0\n"
        "\tbhs .L08061B62\n"
        "\tadds r3, r5, #0\n"
        ".L08061B62:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r3", ".L08061B82", "r8",
        ".L08061B7E", ".L08061B86", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadds r2, r1, #0\n"
        "\tadds r3, r5, #0\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r7, r7, #6\n"
        "\tstr r7, [sp, #.Lfarm_overview_sp_portrait_tilemap_offset]\n"
        "\tmovs r0, #0x9a\n"
        "\tlsls r0, r0, #1\n"
        "\tadd r0, sp\n"
        "\tstr r0, [sp, #0x264]\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_report]\n"
        "\tmovs r4, #0xf6\n"
        "\tlsls r4, r4, #4\n"
        "\tadds r1, r1, r4\n"
        "\tmov sb, r1\n"
        "\tmovs r5, #0x9c\n"
        "\tlsls r5, r5, #1\n"
        "\tadd r5, sp\n"
        "\tstr r5, [sp, #0x268]\n"
        "\tmovs r7, #0xa2\n"
        "\tlsls r7, r7, #1\n"
        "\tadd r7, sp\n"
        "\tstr r7, [sp, #0x26c]\n"
        "\tmovs r0, #0xa4\n"
        "\tlsls r0, r0, #1\n"
        "\tadd r0, sp\n"
        "\tstr r0, [sp, #0x270]\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r1\n"
        "\tbeq .L08061BDE\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L08061BC4", ".L08061BD4", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r0\n"
        "\tbne .L08061BC4\n"
        ".L08061BDE:\n"
        "\tadds r5, r3, #0\n"
        "\tadd r0, sp, #0x130\n"
        "\tldr r0, [r0]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r0", "sl", "r2, r3, r4", "r4, r5, r7", "[sp, #0x44]",
        ".L08061BFC", ".L08061BF8", ".L08061C04", ".L08061C14", ".L08061C1C", ".L08061C1E");

    asm volatile (
        ".syntax unified\n"
        "\tldr r3, [r6, #4]\n"
        "\tldr r1, [r6]\n"
        "\tadds r0, r1, #0\n"
        "\tcmp r1, r3\n"
        "\tbeq .L08061C2E\n"
        ".L08061C28:\n"
        "\tadds r0, #0x10\n"
        "\tcmp r0, r3\n"
        "\tbne .L08061C28\n"
        ".L08061C2E:\n"
        "\tcmp r1, #0\n"
        "\tbeq .L08061C38\n"
        "\tadds r0, r1, #0\n"
        "\tbl free\n"
        ".L08061C38:\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadd r0, r8\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tstr r7, [r6]\n"
        "\tstr r5, [r6, #4]\n"
        "\tstr r0, [r6, #0xc]\n"
        ".L08061C44:\n"
        "\tmovs r4, #0xc\n"
        "\tmovs r5, #2\n"
        "\tadd r0, sp, #0x134\n"
        "\tstrh r4, [r0]\n"
        "\tldr r1, [sp, #0x264]\n"
        "\tstrh r5, [r1, #2]\n"
        "\tldr r0, [r0]\n"
        "\tmov r1, sb\n"
        "\tmovs r2, #0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_FILL_NAME_BITMAP();

    asm volatile (
        ".syntax unified\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_active_npc]\n"
        "\tbl GetNamedNpcName\n"
        "\tadds r1, r0, #0\n"
        "\tadd r0, sp, #0x138\n"
        "\tstrh r4, [r0]\n"
        "\tldr r2, [sp, #0x268]\n"
        "\tstrh r5, [r2, #2]\n"
        "\tldr r0, [r0]\n"
        "\tstr r1, [sp]\n"
        "\tmov r1, sb\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_DRAW_LABEL_OR_NAME();

    OVERVIEW_GET_DMA_QUEUE("r3", true);

    asm volatile (
        ".syntax unified\n"
        "\tmovs r3, #0xc0\n"
        "\tlsls r3, r3, #2\n"
        "\tadds r6, r0, #0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_INIT_COPY(name_copy, "mov %0, sb", "ldr %0, .L08061CB4", 0);

    asm volatile (
        ".syntax unified\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_name_copy_command_ptr]\n"
        "\tmov sb, r4\n"
        "\tldr r1, [r6, #4]\n"
        "\tldr r0, [r6, #0xc]\n"
        "\tcmp r1, r0\n"
        "\tbeq .L08061CB8\n"
        "\tcmp r1, #0\n"
        "\tbeq .L08061CAC\n"
        "\tadds r0, r1, #0\n"
        "\tadds r1, r4, #0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_COMMAND("r2, r5, r7");

    asm volatile (
        ".syntax unified\n"
        ".L08061CAC:\n"
        "\tldr r0, [r6, #4]\n"
        "\tadds r0, #0x10\n"
        "\tstr r0, [r6, #4]\n"
        "\tb .L08061D86\n"
        "\t.align 2, 0\n"
        ".L08061CB4: .4byte 0x06003600\n"
        ".L08061CB8:\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0x140\n"
        "\tstr r0, [r1]\n"
        "\tldr r0, [r6]\n"
        "\tldr r3, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tsubs r0, r3, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0x13c\n"
        "\tstr r4, [r0]\n"
        "\tadds r2, r1, #0\n"
        "\tldr r0, [r1]\n"
        "\tadd r1, sp, #0x13c\n"
        "\tcmp r4, r0\n"
        "\tbhs .L08061CD8\n"
        "\tadds r1, r2, #0\n"
        ".L08061CD8:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r1", ".L08061CF6", "r8",
        ".L08061CFA", ".L08061CFA", false);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tldr r2, [r6]\n"
        "\tadds r3, r5, #0\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r4\n"
        "\tbeq .L08061D22\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L08061D08", ".L08061D18", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r5\n"
        "\tbne .L08061D08\n"
        ".L08061D22:\n"
        "\tadds r5, r3, #0\n"
        "\tadd r0, sp, #0x140\n"
        "\tldr r0, [r0]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r0", "sb", "r2, r3, r7", "r4, r5, r7", "[r1]",
        ".L08061D40", ".L08061D3C", ".L08061D48", ".L08061D58", ".L08061D60", ".L08061D62");

    asm volatile (
        ".syntax unified\n"
        "\tCommitFarmOverviewTransferList r8, r1\n"
        ".L08061D86:\n"
        "\tldr r2, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r0, [r2, #.Lfarm_overview_panel_tilemap]\n"
        "\tldr r3, [sp, #.Lfarm_overview_sp_portrait_tilemap_offset]\n"
        "\tadds r0, r0, r3\n"
        "\tadds r0, #0x22\n"
        "\tmovs r1, #0xd8\n"
        "\tlsls r1, r1, #1\n"
        "\tmovs r2, #0\n"
        "\tstr r2, [sp]\n"
        "\tmovs r2, #0x20\n"
        "\tstr r2, [sp, #4]\n"
        "\tmovs r2, #0xc\n"
        "\tmovs r3, #2\n"
        "\tbl FillMessageTilemapRect\n"
        "\tmovs r0, #0xa\n"
        "\tadd r4, sp, #0x144\n"
        "\tstr r0, [r4]\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_active_npc]\n"
        "\tbl GetFriendship__C3Npc\n"
        "\tmovs r1, #0x19\n"
        "\tbl __udivsi3\n"
        "\tadd r1, sp, #0x148\n"
        "\tstr r0, [r1]\n"
        "\tldr r1, [r4]\n"
        "\tldr r2, [sp, #0x26c]\n"
        "\tcmp r1, r0\n"
        "\tbls .L08061DC4\n"
        "\tldr r2, [sp, #0x270]\n"
        ".L08061DC4:\n"
        "\tldr r6, [r2]\n"
        "\tmovs r4, #0\n"
        "\tcmp r4, r6\n"
        "\tbhs .Lfarm_overview_set_dog_row\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tadds r0, #2\n"
        "\tlsls r0, r0, #6\n"
        "\tmov r8, r0\n"
        "\tmovs r5, #0x26\n"
        "\tmovs r0, #0x58\n"
        "\tlsls r7, r0, #0x10\n"
        ".L08061DDA:\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r0, [r1, #.Lfarm_overview_panel_tilemap]\n"
        "\tadd r0, r8\n"
        "\tadds r0, r0, r5\n"
        "\tmovs r1, #2\n"
        "\tstr r1, [sp]\n"
        "\tmovs r1, #0x20\n"
        "\tstr r1, [sp, #4]\n"
        "\tlsrs r1, r7, #0x10\n"
        "\tmovs r2, #1\n"
        "\tmovs r3, #1\n"
        "\tbl FillMessageTilemapRect\n"
        "\tadds r5, #2\n"
        "\tadds r4, #1\n"
        "\tcmp r4, r6\n"
        "\tblo .L08061DDA\n"
        ".Lfarm_overview_set_dog_row:\n"
        "\tldr r2, [sp, #.Lfarm_overview_sp_dog_tile_row]\n"
        "\tstr r2, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\n"
        "\t.align 2, 0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    // Draw permanent pet rows and the optional horse row.

    asm volatile (
        ".syntax unified\n"
        ".Lfarm_overview_dog_row:\n"
        "\tldr r3, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r1, r3, #3\n"
        "\tmovs r0, #0\n"
        "\tmov r4, sl\n"
        "\tstrh r1, [r4]\n"
        "\tstr r0, [r4, #4]\n"
        "\tldr r0, .Loverview_dog_portrait_tiles\n"
        "\tstr r0, [r4, #8]\n"
        "\tldr r0, .Loverview_dog_portrait_palette\n"
        "\tstr r0, [r4, #0xc]\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r7, .L08061E50\n"
        "\tadds r6, r5, r7\n"
        "\tldr r2, [r6, #4]\n"
        "\tldr r0, [r6, #0xc]\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_portrait_y_pixels]\n"
        "\tcmp r2, r0\n"
        "\tbeq .L08061E54\n"
        "\tcmp r2, #0\n"
        "\tbeq .L08061E34\n"
        "\tadds r0, r2, #0\n"
        "\tmov r1, sl\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_STACK_COMMAND("r2, r3, r4", label_copy);

    asm volatile (
        ".syntax unified\n"
        ".L08061E34:\n"
        "\tldr r0, [r6, #4]\n"
        "\tadds r0, #0x10\n"
        "\tstr r0, [r6, #4]\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r5, r5, #6\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_portrait_tilemap_offset]\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tadds r7, #4\n"
        "\tstr r7, [sp, #.Lfarm_overview_sp_portrait_lower_tile_row]\n"
        "\tb .L08061F30\n"
        "\t.align 2, 0\n"
        ".Loverview_dog_portrait_tiles: .4byte gUnk_08751E6C\n"
        ".Loverview_dog_portrait_palette: .4byte gUnk_08751EEC\n"
        ".L08061E50: .4byte .Lfarm_overview_portrait_uploads\n"
        ".L08061E54:\n"
        "\tmov sb, r2\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0x150\n"
        "\tstr r0, [r1]\n"
        "\tldr r2, [r6]\n"
        "\tmov r3, sb\n"
        "\tsubs r0, r3, r2\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0x14c\n"
        "\tstr r4, [r0]\n"
        "\tadds r5, r1, #0\n"
        "\tldr r0, [r1]\n"
        "\tadd r3, sp, #0x14c\n"
        "\tadds r1, r2, #0\n"
        "\tcmp r4, r0\n"
        "\tbhs .L08061E76\n"
        "\tadds r3, r5, #0\n"
        ".L08061E76:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r3", ".L08061E96", "r8",
        ".L08061E92", ".L08061E9A", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadds r2, r1, #0\n"
        "\tadds r3, r5, #0\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r4, r4, #6\n"
        "\tstr r4, [sp, #.Lfarm_overview_sp_portrait_tilemap_offset]\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tadds r5, #4\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_portrait_lower_tile_row]\n"
        "\tcmp r2, sb\n"
        "\tbeq .L08061ECA\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L08061EB2", ".L08061EC2", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        "\tcmp r2, sb\n"
        "\tbne .L08061EB2\n"
        ".L08061ECA:\n"
        "\tadds r5, r3, #0\n"
        "\tadd r0, sp, #0x150\n"
        "\tldr r0, [r0]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r0", "sl", "r2, r3, r7", "r4, r5, r7", "[sp, #0x44]",
        ".L08061EE8", ".L08061EE4", ".L08061EF0", ".L08061F00", ".L08061F08", ".L08061F0A");

    asm volatile (
        ".syntax unified\n"
        "\tldr r3, [r6, #4]\n"
        "\tldr r1, [r6]\n"
        "\tadds r0, r1, #0\n"
        "\tcmp r1, r3\n"
        "\tbeq .L08061F1A\n"
        ".L08061F14:\n"
        "\tadds r0, #0x10\n"
        "\tcmp r0, r3\n"
        "\tbne .L08061F14\n"
        ".L08061F1A:\n"
        "\tcmp r1, #0\n"
        "\tbeq .L08061F24\n"
        "\tadds r0, r1, #0\n"
        "\tbl free\n"
        ".L08061F24:\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadd r0, r8\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tstr r1, [r6]\n"
        "\tstr r5, [r6, #4]\n"
        "\tstr r0, [r6, #0xc]\n"
        ".L08061F30:\n"
        "\tldr r2, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r3, .L08061FD8\n"
        "\tadds r6, r2, r3\n"
        "\tldr r0, [r6]\n"
        "\tldr r7, .L08061FDC\n"
        "\tadds r0, r0, r7\n"
        "\tbl IsFestivalWinner__C6Animal\n"
        "\tlsls r0, r0, #0x18\n"
        "\tcmp r0, #0\n"
        "\tbeq .L08061F64\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tsubs r1, #2\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r0, [r4, #.Lfarm_overview_panel_tilemap]\n"
        "\tlsls r1, r1, #6\n"
        "\tadds r0, r0, r1\n"
        "\tmovs r1, #0x54\n"
        "\tmovs r2, #1\n"
        "\tstr r2, [sp]\n"
        "\tmovs r2, #0x20\n"
        "\tstr r2, [sp, #4]\n"
        "\tmovs r2, #2\n"
        "\tmovs r3, #2\n"
        "\tbl FillMessageTilemapRect\n"
        ".L08061F64:\n"
        "\tmovs r4, #0xc\n"
        "\tmovs r5, #2\n"
        "\tadd r0, sp, #0x154\n"
        "\tstrh r4, [r0]\n"
        "\tldr r1, [sp, #0x274]\n"
        "\tstrh r5, [r1, #2]\n"
        "\tldr r0, [r0]\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_horse_name_tiles]\n"
        "\tmovs r2, #0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_FILL_NAME_BITMAP();

    asm volatile (
        ".syntax unified\n"
        "\tldr r0, [r6]\n"
        "\tadds r0, r0, r7\n"
        "\tbl GetName__C6Animal\n"
        "\tadds r1, r0, #0\n"
        "\tadd r0, sp, #0x158\n"
        "\tstrh r4, [r0]\n"
        "\tldr r2, [sp, #0x278]\n"
        "\tstrh r5, [r2, #2]\n"
        "\tldr r0, [r0]\n"
        "\tstr r1, [sp]\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_horse_name_tiles]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_DRAW_LABEL_OR_NAME();

    OVERVIEW_GET_DMA_QUEUE("r3", true);

    asm volatile (
        ".syntax unified\n"
        "\tmovs r3, #0xc0\n"
        "\tlsls r3, r3, #2\n"
        "\tadds r6, r0, #0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_INIT_COPY(name_copy, "ldr %0, [sp, #.Lfarm_overview_sp_horse_name_tiles]", "ldr %0, .L08061FE0", 0);

    asm volatile (
        ".syntax unified\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_name_copy_command_ptr]\n"
        "\tmov sb, r4\n"
        "\tldr r1, [r6, #4]\n"
        "\tldr r0, [r6, #0xc]\n"
        "\tcmp r1, r0\n"
        "\tbeq .L08061FE4\n"
        "\tcmp r1, #0\n"
        "\tbeq .L08061FCE\n"
        "\tadds r0, r1, #0\n"
        "\tadds r1, r4, #0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_COMMAND("r2, r5, r7");

    asm volatile (
        ".syntax unified\n"
        ".L08061FCE:\n"
        "\tldr r0, [r6, #4]\n"
        "\tadds r0, #0x10\n"
        "\tstr r0, [r6, #4]\n"
        "\tb .L080620B2\n"
        "\t.align 2, 0\n"
        ".L08061FD8: .4byte .Lfarm_overview_game_data\n"
        ".L08061FDC: .4byte 0x00001C70\n"
        ".L08061FE0: .4byte 0x06003900\n"
        ".L08061FE4:\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0x160\n"
        "\tstr r0, [r1]\n"
        "\tldr r0, [r6]\n"
        "\tldr r3, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tsubs r0, r3, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0x15c\n"
        "\tstr r4, [r0]\n"
        "\tadds r2, r1, #0\n"
        "\tldr r0, [r1]\n"
        "\tadd r1, sp, #0x15c\n"
        "\tcmp r4, r0\n"
        "\tbhs .L08062004\n"
        "\tadds r1, r2, #0\n"
        ".L08062004:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r1", ".L08062022", "r8",
        ".L08062026", ".L08062026", false);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tldr r2, [r6]\n"
        "\tadds r3, r5, #0\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r4\n"
        "\tbeq .L0806204E\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L08062034", ".L08062044", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r5\n"
        "\tbne .L08062034\n"
        ".L0806204E:\n"
        "\tadds r5, r3, #0\n"
        "\tadd r0, sp, #0x160\n"
        "\tldr r0, [r0]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r0", "sb", "r2, r3, r7", "r4, r5, r7", "[r1]",
        ".L0806206C", ".L08062068", ".L08062074", ".L08062084", ".L0806208C", ".L0806208E");

    asm volatile (
        ".syntax unified\n"
        "\tCommitFarmOverviewTransferList r8, r1\n"
        ".L080620B2:\n"
        "\tldr r2, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r0, [r2, #.Lfarm_overview_panel_tilemap]\n"
        "\tldr r3, [sp, #.Lfarm_overview_sp_portrait_tilemap_offset]\n"
        "\tadds r0, r0, r3\n"
        "\tadds r0, #4\n"
        "\tmovs r1, #0xe4\n"
        "\tlsls r1, r1, #1\n"
        "\tmovs r2, #0\n"
        "\tstr r2, [sp]\n"
        "\tmovs r2, #0x20\n"
        "\tstr r2, [sp, #4]\n"
        "\tmovs r2, #0xc\n"
        "\tmovs r3, #2\n"
        "\tbl FillMessageTilemapRect\n"
        "\tmovs r0, #0xa\n"
        "\tadd r4, sp, #0x164\n"
        "\tstr r0, [r4]\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r7, .L080621A8\n"
        "\tadds r0, r5, r7\n"
        "\tldr r0, [r0]\n"
        "\tldr r1, .L080621AC\n"
        "\tadds r0, r0, r1\n"
        "\tbl GetAffection__C6Animal\n"
        "\tmovs r1, #0x19\n"
        "\tbl __udivsi3\n"
        "\tadd r1, sp, #0x168\n"
        "\tstr r0, [r1]\n"
        "\tldr r1, [r4]\n"
        "\tldr r2, [sp, #0x27c]\n"
        "\tcmp r1, r0\n"
        "\tbls .L080620FA\n"
        "\tldr r2, [sp, #0x280]\n"
        ".L080620FA:\n"
        "\tldr r6, [r2]\n"
        "\tmovs r4, #0\n"
        "\tcmp r4, r6\n"
        "\tbhs .Lfarm_overview_horse_row\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tadds r0, #2\n"
        "\tlsls r0, r0, #6\n"
        "\tmov r8, r0\n"
        "\tmovs r5, #8\n"
        "\tmovs r2, #0x58\n"
        "\tlsls r7, r2, #0x10\n"
        ".L08062110:\n"
        "\tldr r3, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r0, [r3, #.Lfarm_overview_panel_tilemap]\n"
        "\tadd r0, r8\n"
        "\tadds r0, r0, r5\n"
        "\tmovs r1, #2\n"
        "\tstr r1, [sp]\n"
        "\tmovs r1, #0x20\n"
        "\tstr r1, [sp, #4]\n"
        "\tlsrs r1, r7, #0x10\n"
        "\tmovs r2, #1\n"
        "\tmovs r3, #1\n"
        "\tbl FillMessageTilemapRect\n"
        "\tadds r5, #2\n"
        "\tadds r4, #1\n"
        "\tcmp r4, r6\n"
        "\tblo .L08062110\n"
        ".Lfarm_overview_horse_row:\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r5, .L080621A8\n"
        "\tadds r0, r4, r5\n"
        "\tldr r0, [r0]\n"
        "\tadds r0, #0x14\n"
        "\tbl GetHorse__C4Farm\n"
        "\tstr r0, [sp, #.Lfarm_overview_sp_horse]\n"
        "\tcmp r0, #0\n"
        "\tbne .Lfarm_overview_queue_horse_portrait\n"
        "\tb .Lfarm_overview_draw_chicken_count_row\n"
        ".Lfarm_overview_queue_horse_portrait:\n"
        "\tadd r7, sp, #.Lfarm_overview_sp_portrait_y_pixels\n"
        "\tldrh r0, [r7]\n"
        "\tmov r7, sl\n"
        "\tstrh r0, [r7]\n"
        "\tmovs r0, #0x78\n"
        "\tstr r0, [r7, #4]\n"
        "\tldr r0, .Loverview_horse_portrait_tiles\n"
        "\tstr r0, [r7, #8]\n"
        "\tldr r0, .Loverview_horse_portrait_palette\n"
        "\tstr r0, [r7, #0xc]\n"
        "\tldr r0, .L080621B8\n"
        "\tadds r6, r4, r0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L080621C0", ".L08062178",
        "sl", "r2, r3, r4", "[sp, #0x44]");

    asm volatile (
        ".syntax unified\n"
        "\tmovs r5, #0xba\n"
        "\tlsls r5, r5, #1\n"
        "\tadd r5, sp\n"
        "\tstr r5, [sp, #0x284]\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r0, .L080621BC\n"
        "\tadds r7, r7, r0\n"
        "\tmov sb, r7\n"
        "\tmovs r1, #0xbc\n"
        "\tlsls r1, r1, #1\n"
        "\tadd r1, sp\n"
        "\tstr r1, [sp, #0x288]\n"
        "\tmovs r2, #0xc2\n"
        "\tlsls r2, r2, #1\n"
        "\tadd r2, sp\n"
        "\tstr r2, [sp, #0x28c]\n"
        "\tmovs r3, #0xc4\n"
        "\tlsls r3, r3, #1\n"
        "\tadd r3, sp\n"
        "\tstr r3, [sp, #0x290]\n"
        "\tb .L080622C0\n"
        "\t.align 2, 0\n"
        ".L080621A8: .4byte .Lfarm_overview_game_data\n"
        ".L080621AC: .4byte 0x00001C70\n"
        ".Loverview_horse_portrait_tiles: .4byte gUnk_087527CC\n"
        ".Loverview_horse_portrait_palette: .4byte gUnk_0875284C\n"
        ".L080621B8: .4byte .Lfarm_overview_portrait_uploads\n"
        ".L080621BC: .4byte 0x0000186C\n"
        ".L080621C0:\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0x170\n"
        "\tstr r0, [r1]\n"
        "\tldr r2, [r6]\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tsubs r0, r4, r2\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0x16c\n"
        "\tstr r4, [r0]\n"
        "\tadds r5, r1, #0\n"
        "\tldr r0, [r1]\n"
        "\tadd r3, sp, #0x16c\n"
        "\tadds r1, r2, #0\n"
        "\tcmp r4, r0\n"
        "\tbhs .L080621E2\n"
        "\tadds r3, r5, #0\n"
        ".L080621E2:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r3", ".L08062202", "r8",
        ".L080621FE", ".L08062206", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadds r2, r1, #0\n"
        "\tadds r3, r5, #0\n"
        "\tmovs r5, #0xba\n"
        "\tlsls r5, r5, #1\n"
        "\tadd r5, sp\n"
        "\tstr r5, [sp, #0x284]\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r0, .L08062274\n"
        "\tadds r7, r7, r0\n"
        "\tmov sb, r7\n"
        "\tmovs r1, #0xbc\n"
        "\tlsls r1, r1, #1\n"
        "\tadd r1, sp\n"
        "\tstr r1, [sp, #0x288]\n"
        "\tmovs r4, #0xc2\n"
        "\tlsls r4, r4, #1\n"
        "\tadd r4, sp\n"
        "\tstr r4, [sp, #0x28c]\n"
        "\tmovs r5, #0xc4\n"
        "\tlsls r5, r5, #1\n"
        "\tadd r5, sp\n"
        "\tstr r5, [sp, #0x290]\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r7\n"
        "\tbeq .L08062256\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L0806223C", ".L0806224C", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r0\n"
        "\tbne .L0806223C\n"
        ".L08062256:\n"
        "\tadds r5, r3, #0\n"
        "\tadd r0, sp, #0x170\n"
        "\tldr r0, [r0]\n"
        "\tcmp r0, #1\n"
        "\tbne .L08062278\n"
        "\tcmp r5, #0\n"
        "\tbeq .L08062270\n"
        "\tadds r0, r5, #0\n"
        "\tmov r1, sl\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_STACK_COMMAND("r2, r3, r4", label_copy);

    asm volatile (
        ".syntax unified\n"
        ".L08062270:\n"
        "\tadds r5, #0x10\n"
        "\tb .L0806229A\n"
        "\t.align 2, 0\n"
        ".L08062274: .4byte 0x0000186C\n"
        ".L08062278:\n"
        "\tadds r3, r0, #0\n"
        "\tadds r2, r5, #0\n"
        "\tcmp r3, #0\n"
        "\tbeq .L08062298\n"
        ".L08062280:\n"
        "\tcmp r2, #0\n"
        "\tbeq .L08062290\n"
        "\tadds r0, r2, #0\n"
        "\tmov r1, sl\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_STACK_COMMAND("r4, r5, r7", label_copy);

    asm volatile (
        ".syntax unified\n"
        ".L08062290:\n"
        "\tsubs r3, #1\n"
        "\tadds r2, #0x10\n"
        "\tcmp r3, #0\n"
        "\tbne .L08062280\n"
        ".L08062298:\n"
        "\tadds r5, r2, #0\n"
        ".L0806229A:\n"
        "\tldr r3, [r6, #4]\n"
        "\tldr r1, [r6]\n"
        "\tadds r0, r1, #0\n"
        "\tcmp r1, r3\n"
        "\tbeq .L080622AA\n"
        ".L080622A4:\n"
        "\tadds r0, #0x10\n"
        "\tcmp r0, r3\n"
        "\tbne .L080622A4\n"
        ".L080622AA:\n"
        "\tcmp r1, #0\n"
        "\tbeq .L080622B4\n"
        "\tadds r0, r1, #0\n"
        "\tbl free\n"
        ".L080622B4:\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadd r0, r8\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tstr r7, [r6]\n"
        "\tstr r5, [r6, #4]\n"
        "\tstr r0, [r6, #0xc]\n"
        ".L080622C0:\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_horse]\n"
        "\tbl IsFestivalWinner__C6Animal\n"
        "\tlsls r0, r0, #0x18\n"
        "\tcmp r0, #0\n"
        "\tbeq .Lfarm_overview_draw_horse_name\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tsubs r1, #2\n"
        "\tldr r2, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r0, [r2, #.Lfarm_overview_panel_tilemap]\n"
        "\tlsls r1, r1, #6\n"
        "\tadds r0, r0, r1\n"
        "\tadds r0, #0x1e\n"
        "\tmovs r1, #0x54\n"
        "\tmovs r2, #1\n"
        "\tstr r2, [sp]\n"
        "\tmovs r2, #0x20\n"
        "\tstr r2, [sp, #4]\n"
        "\tmovs r2, #2\n"
        "\tmovs r3, #2\n"
        "\tbl FillMessageTilemapRect\n"
        ".Lfarm_overview_draw_horse_name:\n"
        "\tmovs r4, #0xc\n"
        "\tmovs r5, #2\n"
        "\tadd r0, sp, #0x174\n"
        "\tstrh r4, [r0]\n"
        "\tldr r3, [sp, #0x284]\n"
        "\tstrh r5, [r3, #2]\n"
        "\tldr r0, [r0]\n"
        "\tmov r1, sb\n"
        "\tmovs r2, #0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_FILL_NAME_BITMAP();

    asm volatile (
        ".syntax unified\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_horse]\n"
        "\tbl GetName__C6Animal\n"
        "\tadds r1, r0, #0\n"
        "\tadd r0, sp, #0x178\n"
        "\tstrh r4, [r0]\n"
        "\tldr r4, [sp, #0x288]\n"
        "\tstrh r5, [r4, #2]\n"
        "\tldr r0, [r0]\n"
        "\tstr r1, [sp]\n"
        "\tmov r1, sb\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_DRAW_LABEL_OR_NAME();

    OVERVIEW_GET_DMA_QUEUE("r5", true);

    asm volatile (
        ".syntax unified\n"
        "\tmovs r3, #0xc0\n"
        "\tlsls r3, r3, #2\n"
        "\tadds r6, r0, #0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_INIT_COPY(name_copy, "mov %0, sb", "ldr %0, .L0806235C", 0);

    asm volatile (
        ".syntax unified\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_name_copy_command_ptr]\n"
        "\tmov r8, r7\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L08062360", ".L08062354",
        "r8", "r2, r3, r4", "[r1]");

    asm volatile (
        ".syntax unified\n"
        "\tb .L0806242E\n"
        "\t.align 2, 0\n"
        ".L0806235C: .4byte 0x06003C00\n"
        ".L08062360:\n"
        "\tstr r1, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0x180\n"
        "\tstr r0, [r1]\n"
        "\tldr r0, [r6]\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tsubs r0, r5, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0x17c\n"
        "\tstr r4, [r0]\n"
        "\tadds r2, r1, #0\n"
        "\tldr r0, [r1]\n"
        "\tadd r1, sp, #0x17c\n"
        "\tcmp r4, r0\n"
        "\tbhs .L08062380\n"
        "\tadds r1, r2, #0\n"
        ".L08062380:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r1", ".L0806239E", "sb",
        ".L080623A2", ".L080623A2", false);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tldr r2, [r6]\n"
        "\tadds r3, r5, #0\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r7\n"
        "\tbeq .L080623CA\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L080623B0", ".L080623C0", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_old_transfer_list_end]\n"
        "\tcmp r2, r0\n"
        "\tbne .L080623B0\n"
        ".L080623CA:\n"
        "\tadds r5, r3, #0\n"
        "\tadd r0, sp, #0x180\n"
        "\tldr r0, [r0]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r0", "r8", "r2, r3, r4", "r4, r5, r7", "[r1]",
        ".L080623E8", ".L080623E4", ".L080623F0", ".L08062400", ".L08062408", ".L0806240A");

    asm volatile (
        ".syntax unified\n"
        "\tCommitFarmOverviewTransferList sb, r7\n"
        ".L0806242E:\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r0, [r1, #.Lfarm_overview_panel_tilemap]\n"
        "\tldr r2, [sp, #.Lfarm_overview_sp_portrait_tilemap_offset]\n"
        "\tadds r0, r0, r2\n"
        "\tadds r0, #0x22\n"
        "\tmovs r1, #0xf0\n"
        "\tlsls r1, r1, #1\n"
        "\tmovs r2, #0\n"
        "\tstr r2, [sp]\n"
        "\tmovs r2, #0x20\n"
        "\tstr r2, [sp, #4]\n"
        "\tmovs r2, #0xc\n"
        "\tmovs r3, #2\n"
        "\tbl FillMessageTilemapRect\n"
        "\tmovs r0, #0xa\n"
        "\tadd r4, sp, #0x184\n"
        "\tstr r0, [r4]\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_horse]\n"
        "\tbl GetAffection__C6Animal\n"
        "\tmovs r1, #0x19\n"
        "\tbl __udivsi3\n"
        "\tadd r1, sp, #0x188\n"
        "\tstr r0, [r1]\n"
        "\tldr r1, [r4]\n"
        "\tldr r2, [sp, #0x28c]\n"
        "\tcmp r1, r0\n"
        "\tbls .L0806246C\n"
        "\tldr r2, [sp, #0x290]\n"
        ".L0806246C:\n"
        "\tldr r6, [r2]\n"
        "\tmovs r4, #0\n"
        "\tcmp r4, r6\n"
        "\tbhs .Lfarm_overview_draw_chicken_count_row\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tadds r0, #2\n"
        "\tlsls r0, r0, #6\n"
        "\tmov r8, r0\n"
        "\tmovs r5, #0x26\n"
        "\tmovs r3, #0x58\n"
        "\tlsls r7, r3, #0x10\n"
        ".L08062482:\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r0, [r1, #.Lfarm_overview_panel_tilemap]\n"
        "\tadd r0, r8\n"
        "\tadds r0, r0, r5\n"
        "\tmovs r1, #2\n"
        "\tstr r1, [sp]\n"
        "\tmovs r1, #0x20\n"
        "\tstr r1, [sp, #4]\n"
        "\tlsrs r1, r7, #0x10\n"
        "\tmovs r2, #1\n"
        "\tmovs r3, #1\n"
        "\tbl FillMessageTilemapRect\n"
        "\tadds r5, #2\n"
        "\tadds r4, #1\n"
        "\tcmp r4, r6\n"
        "\tblo .L08062482\n"
        "\t@ The lower panel pairs animal counts with the coop and barn feed stocks.\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    // Lay out livestock and feed counts after the portrait rows.

    asm volatile (
        ".syntax unified\n"
        ".Lfarm_overview_draw_chicken_count_row:\n"
        "\tldr r2, [sp, #.Lfarm_overview_sp_portrait_lower_tile_row]\n"
        "\tstr r2, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r0, r2, #3\n"
        "\tmov r3, sl\n"
        "\tstrh r0, [r3]\n"
        "\tmovs r0, #0x20\n"
        "\tstr r0, [r3, #4]\n"
        "\tldr r0, .L080624E8\n"
        "\tstr r0, [r3, #8]\n"
        "\tldr r0, .L080624EC\n"
        "\tstr r0, [r3, #0xc]\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r5, .L080624F0\n"
        "\tadds r6, r4, r5\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L080624F4", ".L080624D8",
        "sl", "r2, r3, r7", "[sp, #0x44]");

    asm volatile (
        ".syntax unified\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r4, r4, #6\n"
        "\tstr r4, [sp, #.Lfarm_overview_sp_portrait_tilemap_offset]\n"
        "\tb .Lfarm_overview_draw_chicken_count\n"
        "\t.align 2, 0\n"
        ".L080624E8: .4byte gUnk_0875268C\n"
        ".L080624EC: .4byte gUnk_0875270C\n"
        ".L080624F0: .4byte .Lfarm_overview_portrait_uploads\n"
        ".L080624F4:\n"
        "\tmov sb, r1\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0x190\n"
        "\tstr r0, [r1]\n"
        "\tldr r2, [r6]\n"
        "\tmov r5, sb\n"
        "\tsubs r0, r5, r2\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0x18c\n"
        "\tstr r4, [r0]\n"
        "\tadds r5, r1, #0\n"
        "\tldr r0, [r1]\n"
        "\tadd r3, sp, #0x18c\n"
        "\tadds r1, r2, #0\n"
        "\tcmp r4, r0\n"
        "\tbhs .L08062516\n"
        "\tadds r3, r5, #0\n"
        ".L08062516:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r3", ".L08062536", "r8",
        ".L08062532", ".L0806253A", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadds r2, r1, #0\n"
        "\tadds r3, r5, #0\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r7, r7, #6\n"
        "\tstr r7, [sp, #.Lfarm_overview_sp_portrait_tilemap_offset]\n"
        "\tcmp r2, sb\n"
        "\tbeq .L08062564\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L0806254C", ".L0806255C", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        "\tcmp r2, sb\n"
        "\tbne .L0806254C\n"
        ".L08062564:\n"
        "\tadds r5, r3, #0\n"
        "\tadd r0, sp, #0x190\n"
        "\tldr r0, [r0]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r0", "sl", "r2, r3, r4", "r4, r5, r7", "[sp, #0x44]",
        ".L08062582", ".L0806257E", ".L0806258A", ".L0806259A", ".L080625A2", ".L080625A4");

    asm volatile (
        ".syntax unified\n"
        "\tldr r3, [r6, #4]\n"
        "\tldr r1, [r6]\n"
        "\tadds r0, r1, #0\n"
        "\tcmp r1, r3\n"
        "\tbeq .L080625B4\n"
        ".L080625AE:\n"
        "\tadds r0, #0x10\n"
        "\tcmp r0, r3\n"
        "\tbne .L080625AE\n"
        ".L080625B4:\n"
        "\tcmp r1, #0\n"
        "\tbeq .L080625BE\n"
        "\tadds r0, r1, #0\n"
        "\tbl free\n"
        ".L080625BE:\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadd r0, r8\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tstr r7, [r6]\n"
        "\tstr r5, [r6, #4]\n"
        "\tstr r0, [r6, #0xc]\n"
        ".Lfarm_overview_draw_chicken_count:\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r2, .L08062644\n"
        "\tadds r0, r1, r2\n"
        "\tldr r0, [r0]\n"
        "\tmovs r3, #0x82\n"
        "\tlsls r3, r3, #3\n"
        "\tadds r0, r0, r3\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_READ_COUNTER(Coop, CountChickens);

    asm volatile (
        ".syntax unified\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r1, [r4, #.Lfarm_overview_panel_tilemap]\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_portrait_tilemap_offset]\n"
        "\tadds r1, r1, r5\n"
        "\tadds r1, #0x12\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_DRAW_COUNT("r4");

    asm volatile (
        ".syntax unified\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r0, [r7, #.Lfarm_overview_panel_tilemap]\n"
        "\tadds r0, r0, r5\n"
        "\tadds r0, #0x14\n"
        "\tldr r2, [sp, #0x238]\n"
        "\tlsrs r1, r2, #0x10\n"
        "\tmovs r2, #0\n"
        "\tstr r2, [sp]\n"
        "\tstr r4, [sp, #4]\n"
        "\tmovs r2, #2\n"
        "\tmovs r3, #2\n"
        "\tbl FillMessageTilemapRect\n"
        "\tmovs r0, #0x94\n"
        "\tmov r3, sl\n"
        "\tstr r0, [r3, #4]\n"
        "\tldr r0, .L08062648\n"
        "\tstr r0, [r3, #8]\n"
        "\tldr r0, .L0806264C\n"
        "\tstr r0, [r3, #0xc]\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r5, .L08062650\n"
        "\tadds r6, r4, r5\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L08062654", ".L0806263A",
        "sl", "r2, r3, r7", "[sp, #0x44]");

    asm volatile (
        ".syntax unified\n"
        "\tb .Lfarm_overview_draw_chicken_feed_count\n"
        "\t.align 2, 0\n"
        ".L08062644: .4byte .Lfarm_overview_game_data\n"
        ".L08062648: .4byte gUnk_087525EC\n"
        ".L0806264C: .4byte gUnk_0875266C\n"
        ".L08062650: .4byte .Lfarm_overview_portrait_uploads\n"
        ".L08062654:\n"
        "\tmov sb, r1\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0x198\n"
        "\tstr r0, [r1]\n"
        "\tldr r2, [r6]\n"
        "\tmov r4, sb\n"
        "\tsubs r0, r4, r2\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0x194\n"
        "\tstr r4, [r0]\n"
        "\tadds r5, r1, #0\n"
        "\tldr r0, [r1]\n"
        "\tadd r3, sp, #0x194\n"
        "\tadds r1, r2, #0\n"
        "\tcmp r4, r0\n"
        "\tbhs .L08062676\n"
        "\tadds r3, r5, #0\n"
        ".L08062676:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r3", ".L08062696", "r8",
        ".L08062692", ".L0806269A", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadds r2, r1, #0\n"
        "\tadds r3, r5, #0\n"
        "\tcmp r2, sb\n"
        "\tbeq .L080626BE\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L080626A6", ".L080626B6", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        "\tcmp r2, sb\n"
        "\tbne .L080626A6\n"
        ".L080626BE:\n"
        "\tadds r5, r3, #0\n"
        "\tadd r0, sp, #0x198\n"
        "\tldr r0, [r0]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r0", "sl", "r2, r3, r7", "r4, r5, r7", "[sp, #0x44]",
        ".L080626DC", ".L080626D8", ".L080626E4", ".L080626F4", ".L080626FC", ".L080626FE");

    asm volatile (
        ".syntax unified\n"
        "\tldr r3, [r6, #4]\n"
        "\tldr r1, [r6]\n"
        "\tadds r0, r1, #0\n"
        "\tcmp r1, r3\n"
        "\tbeq .L0806270E\n"
        ".L08062708:\n"
        "\tadds r0, #0x10\n"
        "\tcmp r0, r3\n"
        "\tbne .L08062708\n"
        ".L0806270E:\n"
        "\tcmp r1, #0\n"
        "\tbeq .L08062718\n"
        "\tadds r0, r1, #0\n"
        "\tbl free\n"
        ".L08062718:\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadd r0, r8\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tstr r1, [r6]\n"
        "\tstr r5, [r6, #4]\n"
        "\tstr r0, [r6, #0xc]\n"
        ".Lfarm_overview_draw_chicken_feed_count:\n"
        "\tldr r2, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r3, .L080627AC\n"
        "\tadds r0, r2, r3\n"
        "\tldr r0, [r0]\n"
        "\tmovs r4, #0x82\n"
        "\tlsls r4, r4, #3\n"
        "\tadds r0, r0, r4\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_READ_COUNTER(Coop, GetStoredBushelCount);

    asm volatile (
        ".syntax unified\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r1, [r5, #.Lfarm_overview_panel_tilemap]\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r5, r7, #6\n"
        "\tadds r1, r1, r5\n"
        "\tadds r1, #0x32\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_DRAW_COUNT("r4");

    asm volatile (
        ".syntax unified\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r0, [r1, #.Lfarm_overview_panel_tilemap]\n"
        "\tadds r0, r0, r5\n"
        "\tadds r0, #0x34\n"
        "\tldr r2, [sp, #0x1e8]\n"
        "\tlsrs r1, r2, #0x10\n"
        "\tmovs r2, #0\n"
        "\tstr r2, [sp]\n"
        "\tstr r4, [sp, #4]\n"
        "\tmovs r2, #2\n"
        "\tmovs r3, #2\n"
        "\tbl FillMessageTilemapRect\n"
        "\tadds r3, r7, #0\n"
        "\tadds r3, #3\n"
        "\tstr r3, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r0, r3, #3\n"
        "\tmov r5, sl\n"
        "\tstrh r0, [r5]\n"
        "\tstr r4, [r5, #4]\n"
        "\tldr r0, .L080627B0\n"
        "\tstr r0, [r5, #8]\n"
        "\tldr r0, .L080627B4\n"
        "\tstr r0, [r5, #0xc]\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r0, .L080627B8\n"
        "\tadds r6, r7, r0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L080627BC", ".L0806279E",
        "sl", "r2, r3, r4", "[sp, #0x44]");

    asm volatile (
        ".syntax unified\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r5, r5, #6\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_portrait_tilemap_offset]\n"
        "\tb .Lfarm_overview_draw_cow_count\n"
        "\t.align 2, 0\n"
        ".L080627AC: .4byte .Lfarm_overview_game_data\n"
        ".L080627B0: .4byte gUnk_08751B4C\n"
        ".L080627B4: .4byte gUnk_08751BCC\n"
        ".L080627B8: .4byte .Lfarm_overview_portrait_uploads\n"
        ".L080627BC:\n"
        "\tmov sb, r1\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0x1a0\n"
        "\tstr r0, [r1]\n"
        "\tldr r2, [r6]\n"
        "\tmov r7, sb\n"
        "\tsubs r0, r7, r2\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0x19c\n"
        "\tstr r4, [r0]\n"
        "\tadds r5, r1, #0\n"
        "\tldr r0, [r1]\n"
        "\tadd r3, sp, #0x19c\n"
        "\tadds r1, r2, #0\n"
        "\tcmp r4, r0\n"
        "\tbhs .L080627DE\n"
        "\tadds r3, r5, #0\n"
        ".L080627DE:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r3", ".L080627FE", "r8",
        ".L080627FA", ".L08062802", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadds r2, r1, #0\n"
        "\tadds r3, r5, #0\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r0, r0, #6\n"
        "\tstr r0, [sp, #.Lfarm_overview_sp_portrait_tilemap_offset]\n"
        "\tcmp r2, sb\n"
        "\tbeq .L0806282C\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L08062814", ".L08062824", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        "\tcmp r2, sb\n"
        "\tbne .L08062814\n"
        ".L0806282C:\n"
        "\tadds r5, r3, #0\n"
        "\tadd r0, sp, #0x1a0\n"
        "\tldr r0, [r0]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r0", "sl", "r2, r3, r4", "r4, r5, r7", "[sp, #0x44]",
        ".L0806284A", ".L08062846", ".L08062852", ".L08062862", ".L0806286A", ".L0806286C");

    asm volatile (
        ".syntax unified\n"
        "\tldr r3, [r6, #4]\n"
        "\tldr r1, [r6]\n"
        "\tadds r0, r1, #0\n"
        "\tcmp r1, r3\n"
        "\tbeq .L0806287C\n"
        ".L08062876:\n"
        "\tadds r0, #0x10\n"
        "\tcmp r0, r3\n"
        "\tbne .L08062876\n"
        ".L0806287C:\n"
        "\tcmp r1, #0\n"
        "\tbeq .L08062886\n"
        "\tadds r0, r1, #0\n"
        "\tbl free\n"
        ".L08062886:\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadd r0, r8\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tstr r7, [r6]\n"
        "\tstr r5, [r6, #4]\n"
        "\tstr r0, [r6, #0xc]\n"
        ".Lfarm_overview_draw_cow_count:\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r2, .L0806290C\n"
        "\tadds r0, r1, r2\n"
        "\tldr r0, [r0]\n"
        "\tmovs r3, #0xbe\n"
        "\tlsls r3, r3, #3\n"
        "\tadds r0, r0, r3\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_READ_COUNTER(Barn, CountCows);

    asm volatile (
        ".syntax unified\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r1, [r4, #.Lfarm_overview_panel_tilemap]\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_portrait_tilemap_offset]\n"
        "\tadds r1, r1, r5\n"
        "\tadds r1, #0x12\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_DRAW_COUNT("r4");

    asm volatile (
        ".syntax unified\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r0, [r7, #.Lfarm_overview_panel_tilemap]\n"
        "\tadds r0, r0, r5\n"
        "\tadds r0, #0x14\n"
        "\tldr r2, [sp, #0x260]\n"
        "\tlsrs r1, r2, #0x10\n"
        "\tmovs r2, #0\n"
        "\tstr r2, [sp]\n"
        "\tstr r4, [sp, #4]\n"
        "\tmovs r2, #2\n"
        "\tmovs r3, #2\n"
        "\tbl FillMessageTilemapRect\n"
        "\tmovs r0, #0x94\n"
        "\tmov r3, sl\n"
        "\tstr r0, [r3, #4]\n"
        "\tldr r0, .L08062910\n"
        "\tstr r0, [r3, #8]\n"
        "\tldr r0, .L08062914\n"
        "\tstr r0, [r3, #0xc]\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r5, .L08062918\n"
        "\tadds r6, r4, r5\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L0806291C", ".L08062902",
        "sl", "r2, r3, r7", "[sp, #0x44]");

    asm volatile (
        ".syntax unified\n"
        "\tb .Lfarm_overview_draw_fodder_count\n"
        "\t.align 2, 0\n"
        ".L0806290C: .4byte .Lfarm_overview_game_data\n"
        ".L08062910: .4byte gUnk_0875236C\n"
        ".L08062914: .4byte gUnk_087523EC\n"
        ".L08062918: .4byte .Lfarm_overview_portrait_uploads\n"
        ".L0806291C:\n"
        "\tmov sb, r1\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0x1a8\n"
        "\tstr r0, [r1]\n"
        "\tldr r2, [r6]\n"
        "\tmov r4, sb\n"
        "\tsubs r0, r4, r2\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0x1a4\n"
        "\tstr r4, [r0]\n"
        "\tadds r5, r1, #0\n"
        "\tldr r0, [r1]\n"
        "\tadd r3, sp, #0x1a4\n"
        "\tadds r1, r2, #0\n"
        "\tcmp r4, r0\n"
        "\tbhs .L0806293E\n"
        "\tadds r3, r5, #0\n"
        ".L0806293E:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r3", ".L0806295E", "r8",
        ".L0806295A", ".L08062962", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadds r2, r1, #0\n"
        "\tadds r3, r5, #0\n"
        "\tcmp r2, sb\n"
        "\tbeq .L08062986\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L0806296E", ".L0806297E", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        "\tcmp r2, sb\n"
        "\tbne .L0806296E\n"
        ".L08062986:\n"
        "\tadds r5, r3, #0\n"
        "\tadd r0, sp, #0x1a8\n"
        "\tldr r0, [r0]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r0", "sl", "r2, r3, r7", "r4, r5, r7", "[sp, #0x44]",
        ".L080629A4", ".L080629A0", ".L080629AC", ".L080629BC", ".L080629C4", ".L080629C6");

    asm volatile (
        ".syntax unified\n"
        "\tldr r3, [r6, #4]\n"
        "\tldr r1, [r6]\n"
        "\tadds r0, r1, #0\n"
        "\tcmp r1, r3\n"
        "\tbeq .L080629D6\n"
        ".L080629D0:\n"
        "\tadds r0, #0x10\n"
        "\tcmp r0, r3\n"
        "\tbne .L080629D0\n"
        ".L080629D6:\n"
        "\tcmp r1, #0\n"
        "\tbeq .L080629E0\n"
        "\tadds r0, r1, #0\n"
        "\tbl free\n"
        ".L080629E0:\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadd r0, r8\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tstr r1, [r6]\n"
        "\tstr r5, [r6, #4]\n"
        "\tstr r0, [r6, #0xc]\n"
        ".Lfarm_overview_draw_fodder_count:\n"
        "\tldr r2, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r3, .L08062A70\n"
        "\tadds r0, r2, r3\n"
        "\tldr r0, [r0]\n"
        "\tmovs r4, #0xbe\n"
        "\tlsls r4, r4, #3\n"
        "\tadds r0, r0, r4\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_READ_COUNTER(Barn, GetStoredBushelCount);

    asm volatile (
        ".syntax unified\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r1, [r5, #.Lfarm_overview_panel_tilemap]\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r5, r7, #6\n"
        "\tadds r1, r1, r5\n"
        "\tadds r1, #0x32\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_DRAW_COUNT("r4");

    asm volatile (
        ".syntax unified\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r0, [r1, #.Lfarm_overview_panel_tilemap]\n"
        "\tadds r0, r0, r5\n"
        "\tadds r0, #0x34\n"
        "\tldr r2, [sp, #0x1e8]\n"
        "\tlsrs r1, r2, #0x10\n"
        "\tmovs r2, #0\n"
        "\tstr r2, [sp]\n"
        "\tstr r4, [sp, #4]\n"
        "\tmovs r2, #2\n"
        "\tmovs r3, #2\n"
        "\tbl FillMessageTilemapRect\n"
        "\tadds r3, r7, #0\n"
        "\tadds r3, #3\n"
        "\tstr r3, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r0, r3, #3\n"
        "\tmov r5, sl\n"
        "\tstrh r0, [r5]\n"
        "\tstr r4, [r5, #4]\n"
        "\tldr r0, .L08062A74\n"
        "\tstr r0, [r5, #8]\n"
        "\tldr r0, .L08062A78\n"
        "\tstr r0, [r5, #0xc]\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r0, .L08062A7C\n"
        "\tadds r6, r7, r0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L08062A80", ".L08062A66",
        "sl", "r2, r3, r4", "[sp, #0x44]");

    asm volatile (
        ".syntax unified\n"
        "\tb .Lfarm_overview_draw_sheep_count\n"
        "\t.align 2, 0\n"
        ".L08062A70: .4byte .Lfarm_overview_game_data\n"
        ".L08062A74: .4byte gUnk_0875320C\n"
        ".L08062A78: .4byte gUnk_0875328C\n"
        ".L08062A7C: .4byte .Lfarm_overview_portrait_uploads\n"
        ".L08062A80:\n"
        "\tmov sb, r1\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0x1b0\n"
        "\tstr r0, [r1]\n"
        "\tldr r2, [r6]\n"
        "\tmov r5, sb\n"
        "\tsubs r0, r5, r2\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0x1ac\n"
        "\tstr r4, [r0]\n"
        "\tadds r5, r1, #0\n"
        "\tldr r0, [r1]\n"
        "\tadd r3, sp, #0x1ac\n"
        "\tadds r1, r2, #0\n"
        "\tcmp r4, r0\n"
        "\tbhs .L08062AA2\n"
        "\tadds r3, r5, #0\n"
        ".L08062AA2:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r3", ".L08062AC2", "r8",
        ".L08062ABE", ".L08062AC6", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadds r2, r1, #0\n"
        "\tadds r3, r5, #0\n"
        "\tcmp r2, sb\n"
        "\tbeq .L08062AEA\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L08062AD2", ".L08062AE2", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        "\tcmp r2, sb\n"
        "\tbne .L08062AD2\n"
        ".L08062AEA:\n"
        "\tadds r5, r3, #0\n"
        "\tadd r0, sp, #0x1b0\n"
        "\tldr r0, [r0]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r0", "sl", "r2, r3, r7", "r4, r5, r7", "[sp, #0x44]",
        ".L08062B08", ".L08062B04", ".L08062B10", ".L08062B20", ".L08062B28", ".L08062B2A");

    asm volatile (
        ".syntax unified\n"
        "\tldr r3, [r6, #4]\n"
        "\tldr r1, [r6]\n"
        "\tadds r0, r1, #0\n"
        "\tcmp r1, r3\n"
        "\tbeq .L08062B3A\n"
        ".L08062B34:\n"
        "\tadds r0, #0x10\n"
        "\tcmp r0, r3\n"
        "\tbne .L08062B34\n"
        ".L08062B3A:\n"
        "\tcmp r1, #0\n"
        "\tbeq .L08062B44\n"
        "\tadds r0, r1, #0\n"
        "\tbl free\n"
        ".L08062B44:\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadd r0, r8\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tstr r1, [r6]\n"
        "\tstr r5, [r6, #4]\n"
        "\tstr r0, [r6, #0xc]\n"
        ".Lfarm_overview_draw_sheep_count:\n"
        "\tldr r2, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r3, .L08062BE0\n"
        "\tadds r0, r2, r3\n"
        "\tldr r0, [r0]\n"
        "\tmovs r4, #0xbe\n"
        "\tlsls r4, r4, #3\n"
        "\tadds r0, r0, r4\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_READ_COUNTER(Barn, CountSheeps);

    asm volatile (
        ".syntax unified\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r1, [r5, #.Lfarm_overview_panel_tilemap]\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r5, r7, #6\n"
        "\tadds r1, r1, r5\n"
        "\tadds r1, #0x12\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_DRAW_COUNT("r4");

    asm volatile (
        ".syntax unified\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r0, [r1, #.Lfarm_overview_panel_tilemap]\n"
        "\tadds r0, r0, r5\n"
        "\tadds r0, #0x14\n"
        "\tldr r2, [sp, #0x260]\n"
        "\tlsrs r1, r2, #0x10\n"
        "\tmovs r2, #0\n"
        "\tstr r2, [sp]\n"
        "\tstr r4, [sp, #4]\n"
        "\tmovs r2, #2\n"
        "\tmovs r3, #2\n"
        "\tbl FillMessageTilemapRect\n"
        "\tadds r3, r7, #0\n"
        "\tadds r3, #3\n"
        "\tstr r3, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r2, r3, #3\n"
        "\tmov r5, sl\n"
        "\tstrh r2, [r5]\n"
        "\tstr r4, [r5, #4]\n"
        "\tldr r0, .L08062BE4\n"
        "\tstr r0, [r5, #8]\n"
        "\tldr r0, .L08062BE8\n"
        "\tstr r0, [r5, #0xc]\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r0, .L08062BEC\n"
        "\tadds r6, r7, r0\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L08062BF0", ".L08062BCA",
        "sl", "r3, r4, r5", "[sp, #0x44]");

    asm volatile (
        ".syntax unified\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r7, r7, #6\n"
        "\tstr r7, [sp, #.Lfarm_overview_sp_portrait_tilemap_offset]\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tadds r0, #3\n"
        "\tstr r0, [sp, #.Lfarm_overview_sp_content_tile_rows]\n"
        "\tstr r2, [sp, #.Lfarm_overview_sp_portrait_y_pixels]\n"
        "\tb .L08062CD2\n"
        "\t.align 2, 0\n"
        ".L08062BE0: .4byte .Lfarm_overview_game_data\n"
        ".L08062BE4: .4byte gUnk_0875316C\n"
        ".L08062BE8: .4byte gUnk_087531EC\n"
        ".L08062BEC: .4byte .Lfarm_overview_portrait_uploads\n"
        ".L08062BF0:\n"
        "\tmov sb, r1\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0x1b8\n"
        "\tstr r0, [r1]\n"
        "\tldr r2, [r6]\n"
        "\tmov r3, sb\n"
        "\tsubs r0, r3, r2\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0x1b4\n"
        "\tstr r4, [r0]\n"
        "\tadds r5, r1, #0\n"
        "\tldr r0, [r1]\n"
        "\tadd r3, sp, #0x1b4\n"
        "\tadds r1, r2, #0\n"
        "\tcmp r4, r0\n"
        "\tbhs .L08062C12\n"
        "\tadds r3, r5, #0\n"
        ".L08062C12:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r3", ".L08062C32", "r8",
        ".L08062C2E", ".L08062C36", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadds r2, r1, #0\n"
        "\tadds r3, r5, #0\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r4, r4, #6\n"
        "\tstr r4, [sp, #.Lfarm_overview_sp_portrait_tilemap_offset]\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tadds r5, #3\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_content_tile_rows]\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_portrait_tile_row]\n"
        "\tlsls r7, r7, #3\n"
        "\tstr r7, [sp, #.Lfarm_overview_sp_portrait_y_pixels]\n"
        "\tcmp r2, sb\n"
        "\tbeq .L08062C6C\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L08062C54", ".L08062C64", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        "\tcmp r2, sb\n"
        "\tbne .L08062C54\n"
        ".L08062C6C:\n"
        "\tadds r5, r3, #0\n"
        "\tadd r0, sp, #0x1b8\n"
        "\tldr r0, [r0]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r0", "sl", "r2, r3, r4", "r4, r5, r7", "[sp, #0x44]",
        ".L08062C8A", ".L08062C86", ".L08062C92", ".L08062CA2", ".L08062CAA", ".L08062CAC");

    asm volatile (
        ".syntax unified\n"
        "\tldr r3, [r6, #4]\n"
        "\tldr r1, [r6]\n"
        "\tadds r0, r1, #0\n"
        "\tcmp r1, r3\n"
        "\tbeq .L08062CBC\n"
        ".L08062CB6:\n"
        "\tadds r0, #0x10\n"
        "\tcmp r0, r3\n"
        "\tbne .L08062CB6\n"
        ".L08062CBC:\n"
        "\tcmp r1, #0\n"
        "\tbeq .L08062CC6\n"
        "\tadds r0, r1, #0\n"
        "\tbl free\n"
        ".L08062CC6:\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadd r0, r8\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tstr r7, [r6]\n"
        "\tstr r5, [r6, #4]\n"
        "\tstr r0, [r6, #0xc]\n"
        ".L08062CD2:\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r2, .L08062D50\n"
        "\tadds r0, r1, r2\n"
        "\tldr r0, [r0]\n"
        "\tldr r3, .L08062D54\n"
        "\tadds r0, r0, r3\n"
        "\tbl func_0809AA28\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r1, [r4, #.Lfarm_overview_panel_tilemap]\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_portrait_tilemap_offset]\n"
        "\tadds r1, r1, r5\n"
        "\tadds r1, #0x16\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_DRAW_COUNT("r4");

    asm volatile (
        ".syntax unified\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r0, [r7, #.Lfarm_overview_panel_tilemap]\n"
        "\tadds r0, r0, r5\n"
        "\tadds r0, #0x18\n"
        "\tldr r2, [sp, #0x1e4]\n"
        "\tlsrs r1, r2, #0x10\n"
        "\tmovs r2, #0\n"
        "\tstr r2, [sp]\n"
        "\tstr r4, [sp, #4]\n"
        "\tmovs r2, #2\n"
        "\tmovs r3, #2\n"
        "\tbl FillMessageTilemapRect\n"
        "\tadd r3, sp, #0x21c\n"
        "\tldrh r4, [r3]\n"
        "\tmov r3, sl\n"
        "\tstrh r4, [r3]\n"
        "\tmovs r0, #0x94\n"
        "\tstr r0, [r3, #4]\n"
        "\tldr r0, .L08062D58\n"
        "\tstr r0, [r3, #8]\n"
        "\tldr r0, .L08062D5C\n"
        "\tstr r0, [r3, #0xc]\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r5, .L08062D60\n"
        "\tadds r6, r4, r5\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_FAST(false, ".L08062D64", ".L08062D46",
        "sl", "r2, r3, r7", "[sp, #0x44]");

    asm volatile (
        ".syntax unified\n"
        "\tb .L08062E34\n"
        "\t.align 2, 0\n"
        ".L08062D50: .4byte .Lfarm_overview_game_data\n"
        ".L08062D54: .4byte 0x00002214\n"
        ".L08062D58: .4byte gUnk_08752EEC\n"
        ".L08062D5C: .4byte gUnk_08752F6C\n"
        ".L08062D60: .4byte .Lfarm_overview_portrait_uploads\n"
        ".L08062D64:\n"
        "\tmov sb, r1\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #0x1c0\n"
        "\tstr r0, [r1]\n"
        "\tldr r2, [r6]\n"
        "\tmov r4, sb\n"
        "\tsubs r0, r4, r2\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #0x1bc\n"
        "\tstr r4, [r0]\n"
        "\tadds r5, r1, #0\n"
        "\tldr r0, [r1]\n"
        "\tadd r3, sp, #0x1bc\n"
        "\tadds r1, r2, #0\n"
        "\tcmp r4, r0\n"
        "\tbhs .L08062D86\n"
        "\tadds r3, r5, #0\n"
        ".L08062D86:\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_ALLOCATE_COMMANDS("r3", ".L08062DA6", "r8",
        ".L08062DA2", ".L08062DAA", true);

    asm volatile (
        ".syntax unified\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadds r2, r1, #0\n"
        "\tadds r3, r5, #0\n"
        "\tcmp r2, sb\n"
        "\tbeq .L08062DCE\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_COPY_EXISTING(".L08062DB6", ".L08062DC6", "r4, r5, r7");

    asm volatile (
        ".syntax unified\n"
        "\tcmp r2, sb\n"
        "\tbne .L08062DB6\n"
        ".L08062DCE:\n"
        "\tadds r5, r3, #0\n"
        "\tadd r0, sp, #0x1c0\n"
        "\tldr r0, [r0]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_APPEND_GROWN_QUEUE("r0", "sl", "r2, r3, r7", "r4, r5, r7", "[sp, #0x44]",
        ".L08062DEC", ".L08062DE8", ".L08062DF4", ".L08062E04", ".L08062E0C", ".L08062E0E");

    asm volatile (
        ".syntax unified\n"
        "\tldr r3, [r6, #4]\n"
        "\tldr r1, [r6]\n"
        "\tadds r0, r1, #0\n"
        "\tcmp r1, r3\n"
        "\tbeq .L08062E1E\n"
        ".L08062E18:\n"
        "\tadds r0, #0x10\n"
        "\tcmp r0, r3\n"
        "\tbne .L08062E18\n"
        ".L08062E1E:\n"
        "\tcmp r1, #0\n"
        "\tbeq .L08062E28\n"
        "\tadds r0, r1, #0\n"
        "\tbl free\n"
        ".L08062E28:\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tadd r0, r8\n"
        "\tldr r1, [sp, #.Lfarm_overview_sp_new_transfer_list_buffer]\n"
        "\tstr r1, [r6]\n"
        "\tstr r5, [r6, #4]\n"
        "\tstr r0, [r6, #0xc]\n"
        ".L08062E34:\n"
        "\tldr r2, [sp, #.Lfarm_overview_sp_report]\n"
        "\tldr r3, .L08062E98\n"
        "\tadds r0, r2, r3\n"
        "\tldr r0, [r0]\n"
        "\tadds r0, #0x14\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_READ_COUNTER(Farm, GetUnk10);

    asm volatile (
        ".syntax unified\n"
        "\tldr r4, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r1, [r4, #.Lfarm_overview_panel_tilemap]\n"
        "\tldr r5, [sp, #.Lfarm_overview_sp_portrait_tilemap_offset]\n"
        "\tadds r1, r1, r5\n"
        "\tadds r1, #0x32\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_DRAW_COUNT("r4");

    asm volatile (
        ".syntax unified\n"
        "\tldr r7, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        "\tldr r0, [r7, #.Lfarm_overview_panel_tilemap]\n"
        "\tadds r0, r0, r5\n"
        "\tadds r0, #0x34\n"
        "\tldr r2, [sp, #0x1ec]\n"
        "\tlsrs r1, r2, #0x10\n"
        "\tmovs r2, #0\n"
        "\tstr r2, [sp]\n"
        "\tstr r4, [sp, #4]\n"
        "\tmovs r2, #2\n"
        "\tmovs r3, #2\n"
        "\tbl FillMessageTilemapRect\n"
        "\tldr r3, [sp, #.Lfarm_overview_sp_content_tile_rows]\n"
        "\tlsls r1, r3, #0x13\n"
        "\tasrs r1, r1, #0x10\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_scroll_panel]\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);

    OVERVIEW_SET_SCROLL_HEIGHT();

    asm volatile (
        ".syntax unified\n"
        "\tldr r0, [sp, #.Lfarm_overview_sp_report]\n"
        "\tmovs r3, #(.Lfarm_overview_stack_bytes >> 2)\n"
        "\tlsls r3, r3, #2\n"
        "\tadd sp, r3\n"
        "\tpop {r3, r4, r5}\n"
        "\tmov r8, r3\n"
        "\tmov sb, r4\n"
        "\tmov sl, r5\n"
        "\tpop {r4, r5, r6, r7}\n"
        "\tpop {r1}\n"
        "\tbx r1\n"
        "\t.align 2, 0\n"
        ".L08062E98: .4byte .Lfarm_overview_game_data\n"
        ".syntax divided\n" : : : OVERVIEW_CLOBBERS);
}

#undef OVERVIEW_SET_SCROLL_HEIGHT
#undef OVERVIEW_READ_COUNTER
#undef OVERVIEW_DRAW_COUNT
#undef OVERVIEW_FILL_NAME_BITMAP
#undef OVERVIEW_DRAW_BLANK_LABEL
#undef OVERVIEW_DRAW_LABEL_OR_NAME
#undef OVERVIEW_INIT_COPY
#undef OVERVIEW_GET_DMA_QUEUE
#undef OVERVIEW_COPY_STACK_COMMAND
#undef OVERVIEW_COPY_COMMAND
#undef OVERVIEW_COPY_EXISTING
#undef OVERVIEW_APPEND_FAST
#undef OVERVIEW_APPEND_GROWN_QUEUE
#undef OVERVIEW_TRANSFER_WORDS
#undef OVERVIEW_ALLOCATE_COMMANDS
#undef OVERVIEW_SAVE_PANEL
#undef OVERVIEW_BUILD_PANEL
#undef OVERVIEW_SET_ARG
#undef OVERVIEW_ADDRESS
#undef OVERVIEW_LOAD
#undef OVERVIEW_SLOT
#undef OVERVIEW_PIN
#undef OVERVIEW_CLOBBERS
