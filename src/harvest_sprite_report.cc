#include "prelude.h"
#include "dma_queue.hh"
#include <stdlib.h>

struct FarmReportData;
struct HardwareContext;
struct HarvestSpriteReport;
struct FarmReportPanel;

struct ReportLabelDimensions
{
    i16 width;
    i16 height;
};

enum
{
    BG_TILES_VRAM = 0x06000000,
    TILE_BYTES = 32,
    PALETTE_BYTES = 32,
    REPORT_HARVEST_SPRITE_MODE = 0x35,
};

EC DmaTransferQueue * HardwareContext_GetDmaQueue(HardwareContext *);
EC void * RetryHarvestSpriteDmaAllocation(u32 bytes) asm("func_080D3BC0");

struct HarvestSpriteReport
{
    HardwareContext * hardware;
    FarmReportPanel * scroll_panel;
    STRUCT_PAD(0x08, 0x58C);
    FarmReportData const * data;
    STRUCT_PAD(0x590, 0x5B4);
    void const * vtable;
    STRUCT_PAD(0x5B8, 0x5BC);
    u8 day_suffix[4 * TILE_BYTES];
    STRUCT_PAD(0x63C, 0x640);
    u8 days_left[8 * TILE_BYTES];
    STRUCT_PAD(0x740, 0x744);
    u8 work[16 * TILE_BYTES];
    STRUCT_PAD(0x944, 0x948);
    u8 not_assigned[24 * TILE_BYTES];
    STRUCT_PAD(0xC48, 0x2164);
    void * skill_uploads_begin;
};

struct HarvestSpriteReportSetup
{
    char const * outgoing_text;
    STRUCT_PAD(0x04, 0x08);
    DmaCommand copy;
    STRUCT_PAD(0x18, 0x28);
    ReportLabelDimensions day_suffix_dimensions;
    u32 day_suffix_queued;
    u32 day_suffix_inserting;
    ReportLabelDimensions days_left_dimensions;
    u32 days_left_queued;
    u32 days_left_inserting;
    ReportLabelDimensions work_dimensions;
    u32 work_queued;
    u32 work_inserting;
    ReportLabelDimensions not_assigned_dimensions;
    u32 not_assigned_queued;
    u32 not_assigned_inserting;
    STRUCT_PAD(0x58, 0xF0);
    HarvestSpriteReport * self;
    FarmReportPanel * panel;
    u32 roster_index;
    STRUCT_PAD(0xFC, 0x10C);
    u8 * work_pixels;
    ReportLabelDimensions * days_left_bounds;
    ReportLabelDimensions * work_bounds;
    ReportLabelDimensions * not_assigned_bounds;
    STRUCT_PAD(0x11C, 0x130);
    u8 * not_assigned_pixels;
    STRUCT_PAD(0x134, 0x160);
    DmaCommand * old_end;
    DmaCommand * new_begin;
};

typedef char HarvestSpriteReportLayoutCheck[
    offsetof(HarvestSpriteReport, vtable) == 0x5B4 &&
    offsetof(HarvestSpriteReport, day_suffix) == 0x5BC &&
    offsetof(HarvestSpriteReport, days_left) == 0x640 &&
    offsetof(HarvestSpriteReport, work) == 0x744 &&
    offsetof(HarvestSpriteReport, not_assigned) == 0x948 &&
    offsetof(HarvestSpriteReport, skill_uploads_begin) == 0x2164 ? 1 : -1];

typedef char HarvestSpriteSetupLayoutCheck[
    offsetof(HarvestSpriteReportSetup, copy) == 8 &&
    offsetof(HarvestSpriteReportSetup, self) == 0xF0 &&
    offsetof(HarvestSpriteReportSetup, panel) == 0xF4 &&
    offsetof(HarvestSpriteReportSetup, roster_index) == 0xF8 &&
    offsetof(HarvestSpriteReportSetup, old_end) == 0x160 &&
    offsetof(HarvestSpriteReportSetup, new_begin) == 0x164 &&
    sizeof(HarvestSpriteReportSetup) == 0x168 ? 1 : -1];

#define HARVEST_PIN(value) asm volatile ("" : "+l" (value))
#define HARVEST_READ(type, name, reg) \
    register type name asm(reg); asm ("" : "=l" (name))

#define HARVEST_INIT_PANEL() do { \
    HARVEST_READ(HarvestSpriteReport *, self, "r0"); \
    asm volatile ("str %0, [sp, %1]\n\tldr %0, .Lhs_report_title_text\n\tstr %0, [sp]\n\tldr %0, [sp, %1]" \
        : "+l" (self) : "i" (offsetof(HarvestSpriteReportSetup, self)) : "memory"); \
    register u32 mode asm("r3") = REPORT_HARVEST_SPRITE_MODE; \
    HARVEST_PIN(mode); \
    asm volatile ("bl func_08076BFC" : : : "r0", "r1", "r2", "r3", "lr", "memory", "cc"); \
} while (0)

#define HARVEST_SET_VTABLE() do { \
    register HarvestSpriteReport * self asm("r0"); \
    register u32 vtable_offset asm("r2"); \
    asm volatile ("ldr %0, [sp, %2]\n\tldr %1, .Lhs_report_vtable_offset" \
        : "=l" (self), "=l" (vtable_offset) : "i" (offsetof(HarvestSpriteReportSetup, self)) : "memory"); \
    register void const ** slot asm("r1"); \
    asm ("add %0, %1, %2" : "=l" (slot) : "l" (self), "l" (vtable_offset)); \
    register void const * vtable asm("r0"); \
    asm volatile ("ldr %0, .Lhs_report_vtable_address" : "=l" (vtable)); \
    *slot = vtable; \
} while (0)

#define HARVEST_DRAW_LABEL() do { \
    register u32 dimensions asm("r0"); \
    register void * pixels asm("r1"); \
    register u32 x asm("r2"); \
    register u32 y asm("r3"); \
    asm volatile ("bl DrawMessageTileString" : "=l" (dimensions), "=l" (pixels), "=l" (x), "=l" (y) \
        : : "lr", "memory", "cc"); \
} while (0)

#define HARVEST_RENDER_LABEL(field, width, text, get_pixels) do { \
    register char const * label asm("r3"); \
    asm volatile ("ldr %0, " text : "=l" (label)); \
    register u32 width_in_tiles asm("r1") = width; \
    HARVEST_PIN(width_in_tiles); \
    register u32 height_in_tiles asm("r2") = 2; \
    HARVEST_PIN(height_in_tiles); \
    register ReportLabelDimensions * dimensions asm("r0"); \
    asm ("add %0, sp, %1" : "=l" (dimensions) : "I" (offsetof(HarvestSpriteReportSetup, field##_dimensions))); \
    asm volatile ("strh %1, [%0]" : : "l" (dimensions), "l" (width_in_tiles) : "memory"); \
    register ReportLabelDimensions * bounds asm("r4"); \
    asm ("ldr %0, [sp, %1]" : "=l" (bounds) : "i" (offsetof(HarvestSpriteReportSetup, field##_bounds))); \
    asm volatile ("strh %1, [%0, %2]" : : "l" (bounds), "l" (height_in_tiles), "I" (offsetof(ReportLabelDimensions, height)) : "memory"); \
    register u32 packed_dimensions asm("r0"); \
    asm ("ldr %0, [sp, %1]" : "=l" (packed_dimensions) : "I" (offsetof(HarvestSpriteReportSetup, field##_dimensions)) : "memory"); \
    HARVEST_PIN(packed_dimensions); \
    asm volatile ("str %0, [sp]" : : "l" (label) : "memory"); \
    register void * pixels asm("r1"); \
    asm volatile (get_pixels : "=l" (pixels) : : "memory"); \
    register u32 x asm("r2") = 0; \
    HARVEST_PIN(x); \
    register u32 y asm("r3") = 0; \
    HARVEST_PIN(y); \
    asm volatile ("bl DrawMessageTileString" : : : "r0", "r1", "r2", "r3", "lr", "cc", "memory"); \
} while (0)

#define HARVEST_INIT_COPY() do { \
    register DmaCommand * command asm("r0"); \
    register void const * source asm("r1"); \
    register void * dest asm("r2"); \
    register u32 bytes asm("r3"); \
    asm volatile ("" : "=l" (command), "=l" (source), "=l" (dest), "=l" (bytes)); \
    DmaCommand_InitializeCopy(command, source, dest, bytes); \
} while (0)

#define HARVEST_GET_QUEUE() do { \
    HARVEST_READ(HardwareContext *, hardware, "r0"); \
    register DmaTransferQueue * queue asm("r0") = HardwareContext_GetDmaQueue(hardware); \
    HARVEST_PIN(queue); \
} while (0)

#define HARVEST_QUEUE(self_reg) \
    register HarvestSpriteReport * self asm(self_reg); \
    asm ("ldr %0, [sp, %1]" : "=l" (self) : "I" (offsetof(HarvestSpriteReportSetup, self)) : "memory"); \
    register HardwareContext * context asm("r0") = self->hardware; \
    HARVEST_PIN(context); \
    register DmaTransferQueue * result asm("r0") = HardwareContext_GetDmaQueue(context); \
    register DmaTransferQueue * queue asm("r6"); \
    asm volatile ("add %0, %1, #0" : "=l" (queue) : "l" (result))

#define HARVEST_UPLOAD_TILE(self_reg, asset, tile, size) do { \
    HARVEST_QUEUE(self_reg); \
    register void const * source asm("r1"); \
    asm volatile ("ldr %0, " asset : "=l" (source)); \
    register u32 tile_id asm("r3") = tile; \
    HARVEST_PIN(tile_id); \
    register u32 offset asm("r2") = tile_id * TILE_BYTES; \
    HARVEST_PIN(offset); \
    register u32 bg_tiles asm("r0") = BG_TILES_VRAM >> 19; \
    HARVEST_PIN(bg_tiles); \
    bg_tiles <<= 19; \
    HARVEST_PIN(bg_tiles); \
    register void * dest asm("r2"); \
    asm ("add %0, %1, %2" : "=l" (dest) : "l" (offset), "l" (bg_tiles)); \
    HARVEST_PIN(dest); \
    register DmaCommand * command asm("r0"); \
    asm volatile ("add %0, sp, %1" : "=l" (command) : "I" (offsetof(HarvestSpriteReportSetup, copy))); \
    register u32 bytes asm("r3"); \
    if ((size) > 255) { \
        bytes = tile_id; \
        asm ("add %0, %1" : "+l" (bytes) : "I" ((size) - (tile))); \
    } else { bytes = size; } \
    HARVEST_PIN(bytes); \
    DmaCommand_InitializeCopy(command, source, dest, bytes); \
} while (0)

#define HARVEST_UPLOAD_LINEAR(self_reg, asset, address, size) do { \
    HARVEST_QUEUE(self_reg); \
    register void const * source asm("r1"); \
    asm volatile ("ldr %0, " asset : "=l" (source)); \
    register DmaCommand * command asm("r0"); \
    asm volatile ("add %0, sp, %1" : "=l" (command) : "I" (offsetof(HarvestSpriteReportSetup, copy))); \
    register void * dest asm("r2"); \
    asm volatile ("ldr %0, " address : "=l" (dest)); \
    register u32 bytes asm("r3") = size; \
    HARVEST_PIN(bytes); \
    DmaCommand_InitializeCopy(command, source, dest, bytes); \
} while (0)

#define HARVEST_APPEND_COPY(command_reg, copy_registers, grow, publish) do { \
    register DmaCommand * local_copy asm(command_reg); \
    asm volatile ("add %0, sp, %1\n\tmov sb, %0" : "=l" (local_copy) \
        : "I" (offsetof(HarvestSpriteReportSetup, copy)) : "r9"); \
    register DmaTransferQueue * queue asm("r6"); \
    asm ("" : "=l" (queue)); \
    register DmaCommand * end asm("r1") = queue->end; \
    HARVEST_PIN(end); \
    register DmaCommand * capacity asm("r0") = queue->capacity_end; \
    HARVEST_PIN(capacity); \
    asm volatile ("cmp %1, %0\n\tbeq " grow "\n\tcmp %1, #0\n\tbeq " publish "\n\t" \
        ".syntax unified\n\tadds r0, r1, #0\n\tadd r1, sp, %2\n\t" \
        "ldmia r1!, {" copy_registers "}\n\tstmia r0!, {" copy_registers "}\n\t" \
        "ldr r1, [r1]\n\tstr r1, [r0]\n\t" publish ":\n\t.syntax divided" \
        : "+l" (capacity), "+l" (end) : "I" (offsetof(HarvestSpriteReportSetup, copy)) \
        : "r2", "r3", "r4", "r5", "r7", "cc", "memory"); \
    register DmaCommand * next asm("r0") = queue->end; \
    HARVEST_PIN(next); \
    ++next; \
    HARVEST_PIN(next); \
    queue->end = next; \
} while (0)

#define HARVEST_GROW_QUEUE(kind, site, args, replay_head) do { \
    register u32 growth_address asm("r0"); \
    asm volatile (".syntax unified\n\tHarvestSprite" #kind "Capacity " site ", " args "\n\t.syntax divided" \
        : "=l" (growth_address) : : "r1", "r2", "r3", "r4", "r10", "memory", "cc"); \
    register u32 capacity asm("r0"); \
    asm ("ldr %0, [%1]\n\tadd %0, r4, %0" : "=l" (capacity) : "l" (growth_address) : "memory", "cc"); \
    asm volatile ("cmp %0, #0\n\tbeq .Lhs_report_dma_empty_capacity" site : : "l" (capacity) : "cc"); \
    register u32 bytes asm("r4") = capacity * sizeof(DmaCommand); \
    HARVEST_PIN(bytes); \
    register u32 allocation_bytes asm("r0") = bytes; \
    HARVEST_PIN(allocation_bytes); \
    register void * allocation asm("r0") = malloc(allocation_bytes); \
    HARVEST_PIN(allocation); \
    register u32 new_capacity_bytes asm("r8"); \
    asm volatile ("mov %0, r4" : "=h" (new_capacity_bytes)); \
    if (replay_head) { \
        asm volatile ("cmp %0, #0\n\tbne .Lhs_report_dma_reload_head" site : : "l" (allocation) : "cc"); \
    } else { \
        asm volatile ("cmp %0, #0\n\tbne .Lhs_report_dma_have_buffer" site : : "l" (allocation) : "cc"); \
    } \
    asm volatile ("mov %0, %1" : "=l" (allocation_bytes) : "h" (new_capacity_bytes)); \
    allocation = RetryHarvestSpriteDmaAllocation(allocation_bytes); \
    HARVEST_PIN(allocation); \
    if (replay_head) { \
        asm volatile (".Lhs_report_dma_reload_head" site ":\n\tldr r1, [r6]" : : : "r1", "memory"); \
    } \
    asm volatile ("b .Lhs_report_dma_have_buffer" site); \
    asm volatile (".Lhs_report_dma_empty_capacity" site ":\n\tmov r0, #0\n\tmov r8, r0" \
        : : : "r0", "r8", "cc"); \
    asm volatile (".syntax unified\n\tHarvestSprite" #kind "Publish " site ", " args "\n\t.syntax divided" \
        : : : "r0", "r1", "r2", "r3", "r4", "r5", "r7", "lr", "memory", "cc"); \
} while (0)

#define HARVEST_RELOAD_HEAD_Label false
#define HARVEST_RELOAD_HEAD_SmallIcon true
#define HARVEST_RELOAD_HEAD_Tile false
#define HARVEST_RELOAD_HEAD_Palette false

#define HARVEST_UPLOAD_ENTRY(name) asm volatile (".Lhs_report_upload_" #name ":")
#define HARVEST_UPLOAD_CLOSE(name, next, asset) \
    asm volatile ("b " next "\n\t.align 2, 0\n\t.Lhs_report_asset_" #name ":\n\t.word " #asset "\n\t" \
        ".Lhs_report_grow_" #name ":")

// Keep each upload's constants before its slow path, in the original pool order.
#define HARVEST_REPORT_TILE(name, next, self_reg, asset, tile, bytes, copy_reg, copy_words, kind, grow_args) do { \
    HARVEST_UPLOAD_ENTRY(name); \
    HARVEST_UPLOAD_TILE(self_reg, ".Lhs_report_asset_" #name, tile, bytes); \
    HARVEST_APPEND_COPY(copy_reg, copy_words, ".Lhs_report_grow_" #name, ".Lhs_report_publish_" #name); \
    HARVEST_UPLOAD_CLOSE(name, next, asset); \
    HARVEST_GROW_QUEUE(kind, "_" #name, grow_args, HARVEST_RELOAD_HEAD_##kind); \
} while (0)

#define HARVEST_REPORT_LINEAR(name, next, self_reg, asset, dest, bytes, copy_reg, copy_words, kind, grow_args) do { \
    HARVEST_UPLOAD_ENTRY(name); \
    HARVEST_UPLOAD_LINEAR(self_reg, ".Lhs_report_asset_" #name, ".Lhs_report_destination_" #name, bytes); \
    HARVEST_APPEND_COPY(copy_reg, copy_words, ".Lhs_report_grow_" #name, ".Lhs_report_publish_" #name); \
    asm volatile ("b " next "\n\t.align 2, 0\n\t.Lhs_report_asset_" #name ":\n\t.word " #asset "\n\t" \
        ".Lhs_report_destination_" #name ":\n\t.word " dest "\n\t.Lhs_report_grow_" #name ":"); \
    HARVEST_GROW_QUEUE(kind, "_" #name, grow_args, HARVEST_RELOAD_HEAD_##kind); \
} while (0)

asm (
        ".set gHarvestSpriteReportFriendshipIconTiles, gUnk_0875254C\n"
        ".set gHarvestSpriteReportFriendshipIconPalette, gUnk_087525CC\n"
        ".set gHarvestSpriteReportSmallWateringIconTiles, gUnk_08752DCC\n"
        ".set gHarvestSpriteReportSmallHarvestingIconTiles, gUnk_08752B4C\n"
        ".set gHarvestSpriteReportSmallAnimalCareIconTiles, gUnk_087529AC\n"
        ".set gHarvestSpriteReportWateringTaskIconTiles, gUnk_08752D4C\n"
        ".set gHarvestSpriteReportHarvestingTaskIconTiles, gUnk_08752ACC\n"
        ".set gHarvestSpriteReportAnimalCareTaskIconTiles, gUnk_0875290C\n"
        ".set gHarvestSpriteReportTaskIconPalette, gUnk_0875298C\n"
        ".set gHarvestSpriteReportWateringSkillBarTiles, gUnk_087517AC\n"
        ".set gHarvestSpriteReportHarvestingSkillBarTiles, gUnk_08750F8C\n"
        ".set gHarvestSpriteReportAnimalCareSkillBarTiles, gUnk_08750E4C\n"
        ".set gHarvestSpriteReportSkillBarPalette, gUnk_08750F6C\n"
        ".set gHarvestSpriteReportSkillMarkerTiles, gUnk_08750C4C\n"
        ".set gHarvestSpriteReportSkillMarkerPalette, gUnk_08750C6C\n"
        "\t.equ .Lhs_report_sp_report, 0xf0\n"
        "\t.equ .Lhs_report_sp_panel, 0xf4\n"
        "\t.equ .Lhs_report_sp_roster_index, 0xf8\n"
        "\t.equ .Lhs_report_sp_sprite, 0xfc\n"
        "\t.equ .Lhs_report_sp_npc_id, 0x100\n"
        "\t.equ .Lhs_report_sp_friendship_row, 0x134\n"
        "\t.equ .Lhs_report_sp_next_roster_index, 0x138\n"
        "\t.equ .Lhs_report_sp_watering_row, 0x13c\n"
        "\t.equ .Lhs_report_sp_harvesting_row, 0x140\n"
        "\t.equ .Lhs_report_sp_animal_care_row, 0x144\n"
        "\t.equ .Lhs_report_sp_next_tile_row, 0x148\n"
        "\t.equ .Lhs_report_sp_name_tilemap_offset, 0x154\n"
        "\t.equ .Lhs_report_game_data, 0x58c\n"
        "\t.equ .Lhs_report_portrait_queue, 0x594\n"
        "\t.equ .Lhs_report_roster, 0x1cd4\n"
        "\t.equ .Lhs_report_panel_tilemap, 0x18\n"
        "\t.equ .Lhs_report_skill_uploads, 0x2164\n"
        "\t.equ .Lhs_report_last_roster_index, 6\n"
        "\t.equ .Lhs_report_task_harvest, 0\n"
        "\t.equ .Lhs_report_task_water, 1\n"
        "\t.equ .Lhs_report_task_animals, 2\n"
        "\t.equ .Lhs_report_task_none, 3\n"
        "\n"
        "\t.equ .Lhs_report_sp_copy_command, 8\n"
        "\t.equ .Lhs_report_sp_dma_old_end, 0x160\n"
        "\t.equ .Lhs_report_sp_dma_new_buffer, 0x164\n"
        "\t.equ .Lhs_report_vtable, 0x5b4\n"
        "\t.equ .Lhs_report_day_suffix_bitmap, 0x5bc\n"
        "\t.equ .Lhs_report_days_left_bitmap, 0x640\n"
        "\t.equ .Lhs_report_work_label_bitmap, 0x744\n"
        "\t.equ .Lhs_report_not_assigned_bitmap, 0x948\n"
        "\t.equ .Lhs_report_bg_tile_vram, 0x06000000\n"
        "\t.equ .Lhs_report_bg_palette_ram, 0x05000000\n"
        "\t.equ .Lhs_report_obj_tile_vram, 0x06010000\n"
        "\t.equ .Lhs_report_obj_palette_ram, 0x05000200\n"
        "\t.equ .Lhs_report_tile_bytes, 32\n"
        "\t.equ .Lhs_report_palette_bytes, 32\n"
        "\t.equ .Lhs_report_day_suffix_tile, 0x38\n"
        "\t.equ .Lhs_report_days_left_tile, 0x3c\n"
        "\t.equ .Lhs_report_work_label_tile, 0x44\n"
        "\t.equ .Lhs_report_not_assigned_tile, 0x54\n"
        "\t.equ .Lhs_report_friendship_tile, 0x6c\n"
        "\t.equ .Lhs_report_small_watering_icon_tile, 0x70\n"
        "\t.equ .Lhs_report_small_harvesting_icon_tile, 0x71\n"
        "\t.equ .Lhs_report_small_animal_care_icon_tile, 0x72\n"
        "\t.equ .Lhs_report_watering_task_tile, 0x73\n"
        "\t.equ .Lhs_report_harvesting_task_tile, 0x77\n"
        "\t.equ .Lhs_report_animal_care_task_tile, 0x7b\n"
        "\t.equ .Lhs_report_watering_skill_tile, 0x9c\n"
        "\t.equ .Lhs_report_harvesting_skill_tile, 0xa5\n"
        "\t.equ .Lhs_report_animal_care_skill_tile, 0xae\n"
        "\n"
        "\t@ Retire the old commands and publish the new DMA buffer.\n"
        "\t.macro CommitHarvestSpriteReportDmaBuffer new_begin, old_begin_reg\n"
        "\t.ifc \\old_begin_reg,reload_r1\n"
        "\tldr r2, [r6, #4]\n"
        "\tldr r1, [r6]\n"
        "\tadds r0, r1, #0\n"
        "\tcmp r1, r2\n"
        "\tbeq .Lhs_report_dma_free_old\\@\n"
        ".Lhs_report_dma_walk_old\\@:\n"
        "\tadds r0, #0x10\n"
        "\tcmp r0, r2\n"
        "\tbne .Lhs_report_dma_walk_old\\@\n"
        ".Lhs_report_dma_free_old\\@:\n"
        "\tcmp r1, #0\n"
        "\tbeq .Lhs_report_dma_publish_replacement\\@\n"
        "\tadds r0, r1, #0\n"
        "\tbl free\n"
        ".Lhs_report_dma_publish_replacement\\@:\n"
        "\t.else\n"
        "\tldr r2, [r6, #4]\n"
        "\tldr r0, [r6]\n"
        "\tadds r1, r0, #0\n"
        "\tcmp r0, r2\n"
        "\tbeq .Lhs_report_dma_free_old\\@\n"
        ".Lhs_report_dma_walk_old\\@:\n"
        "\tadds r1, #0x10\n"
        "\tcmp r1, r2\n"
        "\tbne .Lhs_report_dma_walk_old\\@\n"
        ".Lhs_report_dma_free_old\\@:\n"
        "\tcmp r0, #0\n"
        "\tbeq .Lhs_report_dma_publish_replacement\\@\n"
        "\tbl free\n"
        ".Lhs_report_dma_publish_replacement\\@:\n"
        "\t.endif\n"
        "\tldr r0, [sp, #.Lhs_report_sp_dma_new_buffer]\n"
        "\tadd r0, r8\n"
        "\tldr \\new_begin, [sp, #.Lhs_report_sp_dma_new_buffer]\n"
        "\tstr \\new_begin, [r6]\n"
        "\tstr r5, [r6, #4]\n"
        "\tstr r0, [r6, #0xc]\n"
        "\t.endm\n"
        "\n"
        "\t@ Grow by max(size, 1); sl holds the old end until insertion.\n"
        "\t.macro HarvestSpriteLabelCapacity site, occupied_slot, new_begin\n"
        "\tmov sl, r1\n"
        "\tmovs r0, #1\n"
        "\tstr r0, [sp, #(\\occupied_slot + 4)]\n"
        "\tldr r0, [r6]\n"
        "\tsubs r0, r1, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tstr r4, [sp, #\\occupied_slot]\n"
        "\tadd r1, sp, #(\\occupied_slot + 4)\n"
        "\tadd r0, sp, #\\occupied_slot\n"
        "\tcmp r4, #1\n"
        "\tbhs .Lhs_report_dma_choose_growth\\site\n"
        "\tadds r0, r1, #0\n"
        ".Lhs_report_dma_choose_growth\\site:\n"
        "\t.endm\n"
        "\t.macro HarvestSpriteLabelPublish site, occupied_slot, new_begin\n"
        ".Lhs_report_dma_have_buffer\\site:\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lhs_report_sp_dma_new_buffer]\n"
        "\tldr r2, [r6]\n"
        "\tadds r3, r5, #0\n"
        "\tcmp r2, sl\n"
        "\tbeq .Lhs_report_dma_insert\\site\n"
        ".Lhs_report_dma_copy_existing\\site:\n"
        "\tcmp r3, #0\n"
        "\tbeq .Lhs_report_dma_next_existing\\site\n"
        "\tadds r0, r3, #0\n"
        "\tadds r1, r2, #0\n"
        "\tldm r1!, {r4, r5, r7}\n"
        "\tstm r0!, {r4, r5, r7}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".Lhs_report_dma_next_existing\\site:\n"
        "\tadds r2, #0x10\n"
        "\tadds r3, #0x10\n"
        "\tcmp r2, sl\n"
        "\tbne .Lhs_report_dma_copy_existing\\site\n"
        ".Lhs_report_dma_insert\\site:\n"
        "\tadds r5, r3, #0\n"
        "\tldr r0, [sp, #(\\occupied_slot + 4)]\n"
        "\tcmp r0, #1\n"
        "\tbne .Lhs_report_dma_insert_many\\site\n"
        "\tcmp r5, #0\n"
        "\tbeq .Lhs_report_dma_skip_single_copy\\site\n"
        "\tadds r0, r5, #0\n"
        "\tmov r1, sb\n"
        "\tldm r1!, {r2, r3, r7}\n"
        "\tstm r0!, {r2, r3, r7}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".Lhs_report_dma_skip_single_copy\\site:\n"
        "\tadds r5, #0x10\n"
        "\tb .Lhs_report_dma_release_old\\site\n"
        ".Lhs_report_dma_insert_many\\site:\n"
        "\tadds r3, r0, #0\n"
        "\tadds r2, r5, #0\n"
        "\tcmp r3, #0\n"
        "\tbeq .Lhs_report_dma_finish_insert\\site\n"
        ".Lhs_report_dma_copy_inserted\\site:\n"
        "\tcmp r2, #0\n"
        "\tbeq .Lhs_report_dma_next_inserted\\site\n"
        "\tadds r0, r2, #0\n"
        "\tmov r1, sb\n"
        "\tldm r1!, {r4, r5, r7}\n"
        "\tstm r0!, {r4, r5, r7}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".Lhs_report_dma_next_inserted\\site:\n"
        "\tsubs r3, #1\n"
        "\tadds r2, #0x10\n"
        "\tcmp r3, #0\n"
        "\tbne .Lhs_report_dma_copy_inserted\\site\n"
        ".Lhs_report_dma_finish_insert\\site:\n"
        "\tadds r5, r2, #0\n"
        ".Lhs_report_dma_release_old\\site:\n"
        "\tCommitHarvestSpriteReportDmaBuffer \\new_begin, reload_r0\n"
        "\t.endm\n"
        "\n"
        "\t@ Same append, reloading the original queue start after allocation.\n"
        "\t.macro HarvestSpriteSmallIconCapacity site, occupied_slot, end_tmp, new_begin\n"
        "\tmov sl, r1\n"
        "\tmovs r0, #1\n"
        "\tstr r0, [sp, #(\\occupied_slot + 4)]\n"
        "\tldr r1, [r6]\n"
        "\tmov \\end_tmp, sl\n"
        "\tsubs r0, \\end_tmp, r1\n"
        "\tasrs r4, r0, #4\n"
        "\tstr r4, [sp, #\\occupied_slot]\n"
        "\tadd r2, sp, #(\\occupied_slot + 4)\n"
        "\tadd r0, sp, #\\occupied_slot\n"
        "\tcmp r4, #1\n"
        "\tbhs .Lhs_report_dma_choose_growth\\site\n"
        "\tadds r0, r2, #0\n"
        ".Lhs_report_dma_choose_growth\\site:\n"
        "\t.endm\n"
        "\t.macro HarvestSpriteSmallIconPublish site, occupied_slot, end_tmp, new_begin\n"
        ".Lhs_report_dma_have_buffer\\site:\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lhs_report_sp_dma_new_buffer]\n"
        "\tadds r2, r1, #0\n"
        "\tadds r3, r5, #0\n"
        "\tcmp r2, sl\n"
        "\tbeq .Lhs_report_dma_insert\\site\n"
        ".Lhs_report_dma_copy_existing\\site:\n"
        "\tcmp r3, #0\n"
        "\tbeq .Lhs_report_dma_next_existing\\site\n"
        "\tadds r0, r3, #0\n"
        "\tadds r1, r2, #0\n"
        "\tldm r1!, {r4, r5, r7}\n"
        "\tstm r0!, {r4, r5, r7}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".Lhs_report_dma_next_existing\\site:\n"
        "\tadds r2, #0x10\n"
        "\tadds r3, #0x10\n"
        "\tcmp r2, sl\n"
        "\tbne .Lhs_report_dma_copy_existing\\site\n"
        ".Lhs_report_dma_insert\\site:\n"
        "\tadds r5, r3, #0\n"
        "\tldr r0, [sp, #(\\occupied_slot + 4)]\n"
        "\tcmp r0, #1\n"
        "\tbne .Lhs_report_dma_insert_many\\site\n"
        "\tcmp r5, #0\n"
        "\tbeq .Lhs_report_dma_skip_single_copy\\site\n"
        "\tadds r0, r5, #0\n"
        "\tmov r1, sb\n"
        "\tldm r1!, {r2, r3, r4}\n"
        "\tstm r0!, {r2, r3, r4}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".Lhs_report_dma_skip_single_copy\\site:\n"
        "\tadds r5, #0x10\n"
        "\tb .Lhs_report_dma_release_old\\site\n"
        ".Lhs_report_dma_insert_many\\site:\n"
        "\tadds r3, r0, #0\n"
        "\tadds r2, r5, #0\n"
        "\tcmp r3, #0\n"
        "\tbeq .Lhs_report_dma_finish_insert\\site\n"
        ".Lhs_report_dma_copy_inserted\\site:\n"
        "\tcmp r2, #0\n"
        "\tbeq .Lhs_report_dma_next_inserted\\site\n"
        "\tadds r0, r2, #0\n"
        "\tmov r1, sb\n"
        "\tldm r1!, {r4, r5, r7}\n"
        "\tstm r0!, {r4, r5, r7}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".Lhs_report_dma_next_inserted\\site:\n"
        "\tsubs r3, #1\n"
        "\tadds r2, #0x10\n"
        "\tcmp r3, #0\n"
        "\tbne .Lhs_report_dma_copy_inserted\\site\n"
        ".Lhs_report_dma_finish_insert\\site:\n"
        "\tadds r5, r2, #0\n"
        ".Lhs_report_dma_release_old\\site:\n"
        "\tCommitHarvestSpriteReportDmaBuffer \\new_begin, reload_r1\n"
        "\t.endm\n"
        "\n"
        "\t@ Later uploads keep the old end and insert count in stack slots.\n"
        "\t.macro HarvestSpriteTileCapacity site, occupied_slot, end_tmp, new_begin\n"
        "\tstr r1, [sp, #.Lhs_report_sp_dma_old_end]\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #(\\occupied_slot + 4)\n"
        "\tstr r0, [r1]\n"
        "\tldr r0, [r6]\n"
        "\tldr \\end_tmp, [sp, #.Lhs_report_sp_dma_old_end]\n"
        "\tsubs r0, \\end_tmp, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #\\occupied_slot\n"
        "\tstr r4, [r0]\n"
        "\tldr r2, [r1]\n"
        "\tmov sl, r1\n"
        "\tcmp r4, r2\n"
        "\tbhs .Lhs_report_dma_choose_growth\\site\n"
        "\tmov r0, sl\n"
        ".Lhs_report_dma_choose_growth\\site:\n"
        "\t.endm\n"
        "\t.macro HarvestSpriteTilePublish site, occupied_slot, end_tmp, new_begin\n"
        ".Lhs_report_dma_have_buffer\\site:\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lhs_report_sp_dma_new_buffer]\n"
        "\tldr r2, [r6]\n"
        "\tadds r3, r5, #0\n"
        "\tldr r4, [sp, #.Lhs_report_sp_dma_old_end]\n"
        "\tcmp r2, r4\n"
        "\tbeq .Lhs_report_dma_insert\\site\n"
        ".Lhs_report_dma_copy_existing\\site:\n"
        "\tcmp r3, #0\n"
        "\tbeq .Lhs_report_dma_next_existing\\site\n"
        "\tadds r0, r3, #0\n"
        "\tadds r1, r2, #0\n"
        "\tldm r1!, {r4, r5, r7}\n"
        "\tstm r0!, {r4, r5, r7}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".Lhs_report_dma_next_existing\\site:\n"
        "\tadds r2, #0x10\n"
        "\tadds r3, #0x10\n"
        "\tldr r5, [sp, #.Lhs_report_sp_dma_old_end]\n"
        "\tcmp r2, r5\n"
        "\tbne .Lhs_report_dma_copy_existing\\site\n"
        ".Lhs_report_dma_insert\\site:\n"
        "\tadds r5, r3, #0\n"
        "\tmov r7, sl\n"
        "\tldr r1, [r7]\n"
        "\tcmp r1, #1\n"
        "\tbne .Lhs_report_dma_insert_many\\site\n"
        "\tcmp r5, #0\n"
        "\tbeq .Lhs_report_dma_skip_single_copy\\site\n"
        "\tadds r0, r5, #0\n"
        "\tmov r1, sb\n"
        "\tldm r1!, {r2, r3, r4}\n"
        "\tstm r0!, {r2, r3, r4}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".Lhs_report_dma_skip_single_copy\\site:\n"
        "\tadds r5, #0x10\n"
        "\tb .Lhs_report_dma_release_old\\site\n"
        ".Lhs_report_dma_insert_many\\site:\n"
        "\tadds r3, r1, #0\n"
        "\tadds r2, r5, #0\n"
        "\tcmp r3, #0\n"
        "\tbeq .Lhs_report_dma_finish_insert\\site\n"
        ".Lhs_report_dma_copy_inserted\\site:\n"
        "\tcmp r2, #0\n"
        "\tbeq .Lhs_report_dma_next_inserted\\site\n"
        "\tadds r0, r2, #0\n"
        "\tmov r1, sb\n"
        "\tldm r1!, {r4, r5, r7}\n"
        "\tstm r0!, {r4, r5, r7}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".Lhs_report_dma_next_inserted\\site:\n"
        "\tsubs r3, #1\n"
        "\tadds r2, #0x10\n"
        "\tcmp r3, #0\n"
        "\tbne .Lhs_report_dma_copy_inserted\\site\n"
        ".Lhs_report_dma_finish_insert\\site:\n"
        "\tadds r5, r2, #0\n"
        ".Lhs_report_dma_release_old\\site:\n"
        "\tCommitHarvestSpriteReportDmaBuffer \\new_begin, reload_r0\n"
        "\t.endm\n"
        "\n"
        "\t@ Palette uploads test for the end of the old buffer at the loop tail.\n"
        "\t.macro HarvestSpritePaletteCapacity site, occupied_slot, new_begin\n"
        "\tstr r1, [sp, #.Lhs_report_sp_dma_old_end]\n"
        "\tmovs r0, #1\n"
        "\tadd r1, sp, #(\\occupied_slot + 4)\n"
        "\tstr r0, [r1]\n"
        "\tldr r0, [r6]\n"
        "\tldr r7, [sp, #.Lhs_report_sp_dma_old_end]\n"
        "\tsubs r0, r7, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tadd r0, sp, #\\occupied_slot\n"
        "\tstr r4, [r0]\n"
        "\tldr r2, [r1]\n"
        "\tmov sl, r1\n"
        "\tcmp r4, r2\n"
        "\tbhs .Lhs_report_dma_choose_growth\\site\n"
        "\tmov r0, sl\n"
        ".Lhs_report_dma_choose_growth\\site:\n"
        "\t.endm\n"
        "\t.macro HarvestSpritePalettePublish site, occupied_slot, new_begin\n"
        ".Lhs_report_dma_have_buffer\\site:\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lhs_report_sp_dma_new_buffer]\n"
        "\tldr r2, [r6]\n"
        "\tadds r3, r5, #0\n"
        "\tb .Lhs_report_dma_check_existing_end\\site\n"
        ".Lhs_report_dma_copy_existing\\site:\n"
        "\tcmp r3, #0\n"
        "\tbeq .Lhs_report_dma_next_existing\\site\n"
        "\tadds r0, r3, #0\n"
        "\tadds r1, r2, #0\n"
        "\tldm r1!, {r4, r5, r7}\n"
        "\tstm r0!, {r4, r5, r7}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".Lhs_report_dma_next_existing\\site:\n"
        "\tadds r2, #0x10\n"
        "\tadds r3, #0x10\n"
        ".Lhs_report_dma_check_existing_end\\site:\n"
        "\tldr r0, [sp, #.Lhs_report_sp_dma_old_end]\n"
        "\tcmp r2, r0\n"
        "\tbne .Lhs_report_dma_copy_existing\\site\n"
        "\tadds r5, r3, #0\n"
        "\tmov r2, sl\n"
        "\tldr r1, [r2]\n"
        "\tcmp r1, #1\n"
        "\tbne .Lhs_report_dma_insert_many\\site\n"
        "\tcmp r5, #0\n"
        "\tbeq .Lhs_report_dma_skip_single_copy\\site\n"
        "\tadds r0, r5, #0\n"
        "\tmov r1, sb\n"
        "\tldm r1!, {r3, r4, r7}\n"
        "\tstm r0!, {r3, r4, r7}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".Lhs_report_dma_skip_single_copy\\site:\n"
        "\tadds r5, #0x10\n"
        "\tb .Lhs_report_dma_release_old\\site\n"
        ".Lhs_report_dma_insert_many\\site:\n"
        "\tadds r3, r1, #0\n"
        "\tadds r2, r5, #0\n"
        "\tcmp r3, #0\n"
        "\tbeq .Lhs_report_dma_finish_insert\\site\n"
        ".Lhs_report_dma_copy_inserted\\site:\n"
        "\tcmp r2, #0\n"
        "\tbeq .Lhs_report_dma_next_inserted\\site\n"
        "\tadds r0, r2, #0\n"
        "\tmov r1, sb\n"
        "\tldm r1!, {r4, r5, r7}\n"
        "\tstm r0!, {r4, r5, r7}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".Lhs_report_dma_next_inserted\\site:\n"
        "\tsubs r3, #1\n"
        "\tadds r2, #0x10\n"
        "\tcmp r3, #0\n"
        "\tbne .Lhs_report_dma_copy_inserted\\site\n"
        ".Lhs_report_dma_finish_insert\\site:\n"
        "\tadds r5, r2, #0\n"
        ".Lhs_report_dma_release_old\\site:\n"
        "\tCommitHarvestSpriteReportDmaBuffer \\new_begin, reload_r0\n"
        "\t.endm\n"
        "\n"
);

// The F4 row loop continues directly after the graphics setup and owns the epilogue.
EC SECTION(".text.f3.080645F0") __attribute__((naked)) HarvestSpriteReport * InitHarvestSpriteReport(
    HarvestSpriteReport *, HardwareContext *, FarmReportData const *)
{
    asm volatile (
        ".syntax unified\n"
        "\tpush {r4, r5, r6, r7, lr}\n"
        "\tmov r7, sl\n"
        "\tmov r6, sb\n"
        "\tmov r5, r8\n"
        "\tpush {r5, r6, r7}\n"
        "\tsub sp, #0x168\n"
        ".syntax divided\n" : : : "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "lr", "cc", "memory");

    HARVEST_INIT_PANEL();

    HARVEST_SET_VTABLE();

    asm volatile (
        ".syntax unified\n"
        "\tmovs r0, #6\n"
        "\tadd r3, sp, #0x28\n"
        "\tmov r4, sp\n"
        "\tadds r4, #0x34\n"
        "\tstr r4, [sp, #0x110]\n"
        "\tmov r5, sp\n"
        "\tadds r5, #0x40\n"
        "\tstr r5, [sp, #0x114]\n"
        "\tmov r7, sp\n"
        "\tadds r7, #0x4c\n"
        "\tstr r7, [sp, #0x118]\n"
        "\tmovs r1, #1\n"
        "\trsbs r1, r1, #0\n"
        ".Lhs_report_countdown:\n"
        "\tsubs r0, #1\n"
        "\tcmp r0, r1\n"
        "\tbne .Lhs_report_countdown\n"
        "\tldr r0, [sp, #.Lhs_report_sp_report]\n"
        "\tldr r2, .Lhs_report_skill_uploads_offset\n"
        "\tadds r1, r0, r2\n"
        "\tmovs r0, #0\n"
        "\tstr r0, [r1]\n"
        "\tldr r4, [sp, #.Lhs_report_sp_report]\n"
        "\tldr r4, [r4, #4]\n"
        "\tstr r4, [sp, #.Lhs_report_sp_panel]\n"
        "\tldr r2, .Lhs_report_day_suffix_text\n"
        "\tmovs r1, #2\n"
        "\tadd r0, sp, #0x28\n"
        "\tstrh r1, [r0]\n"
        "\tstrh r1, [r3, #2]\n"
        "\tldr r0, [sp, #0x28]\n"
        "\tldr r5, [sp, #.Lhs_report_sp_report]\n"
        "\tldr r7, .Lhs_report_day_suffix_pixels_offset\n"
        "\tadds r4, r5, r7\n"
        "\tstr r2, [sp]\n"
        "\tadds r1, r4, #0\n"
        "\tmovs r2, #0\n"
        "\tmovs r3, #0\n"
        ".syntax divided\n" : : : "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "lr", "cc", "memory");

    HARVEST_DRAW_LABEL();

    asm volatile (
        ".syntax unified\n"
        "\tldr r0, [r5]\n"
        ".syntax divided\n" : : : "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "lr", "cc", "memory");

    HARVEST_GET_QUEUE();

    asm volatile (
        ".syntax unified\n"
        "\tadds r6, r0, #0\n"
        "\tmovs r0, #0x38\n"
        "\tlsls r2, r0, #5\n"
        "\tmovs r0, #(.Lhs_report_bg_tile_vram >> 19)\n"
        "\tlsls r0, r0, #0x13\n"
        "\tadds r2, r2, r0\n"
        "\tadd r0, sp, #.Lhs_report_sp_copy_command\n"
        "\tadds r1, r4, #0\n"
        "\tmovs r3, #0x80\n"
        ".syntax divided\n" : : : "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "lr", "cc", "memory");

    HARVEST_INIT_COPY();

    HARVEST_APPEND_COPY("r1", "r2, r3, r4", ".Lhs_report_grow_day_suffix", ".Lhs_report_publish_day_suffix");

    asm volatile (
        ".syntax unified\n"
        "\tldr r5, [sp, #.Lhs_report_sp_report]\n"
        "\tmovs r7, #(.Lhs_report_days_left_bitmap >> 3)\n"
        "\tlsls r7, r7, #3\n"
        "\tadds r5, r5, r7\n"
        "\tmov sl, r5\n"
        "\tldr r1, .Lhs_report_fast_work_pixels_offset\n"
        "\tldr r0, [sp, #.Lhs_report_sp_report]\n"
        "\tadds r1, r0, r1\n"
        "\tstr r1, [sp, #0x10c]\n"
        "\tldr r2, .Lhs_report_fast_not_assigned_pixels_offset\n"
        "\tadds r2, r0, r2\n"
        "\tstr r2, [sp, #0x130]\n"
        "\tb .Lhs_report_upload_days_left_label\n"
        "\t.align 2, 0\n"
        ".Lhs_report_title_text: .4byte gHarvestSpriteReportTitle\n"
        ".Lhs_report_vtable_offset: .4byte .Lhs_report_vtable\n"
        ".Lhs_report_vtable_address: .4byte gHarvestSpriteReportVTable\n"
        ".Lhs_report_skill_uploads_offset: .4byte .Lhs_report_skill_uploads\n"
        ".Lhs_report_day_suffix_text: .4byte gFarmReportDaySuffix\n"
        ".Lhs_report_day_suffix_pixels_offset: .4byte .Lhs_report_day_suffix_bitmap\n"
        ".Lhs_report_fast_work_pixels_offset: .4byte .Lhs_report_work_label_bitmap\n"
        ".Lhs_report_fast_not_assigned_pixels_offset: .4byte .Lhs_report_not_assigned_bitmap\n"
        ".Lhs_report_grow_day_suffix:\n"
        "\tstr r1, [sp, #.Lhs_report_sp_dma_old_end]\n"
        "\tmovs r0, #1\n"
        "\tstr r0, [sp, #0x30]\n"
        "\tldr r0, [r6]\n"
        "\tadds r3, r1, #0\n"
        "\tsubs r0, r3, r0\n"
        "\tasrs r4, r0, #4\n"
        "\tstr r4, [sp, #0x2c]\n"
        "\tadd r1, sp, #0x30\n"
        "\tadd r0, sp, #0x2c\n"
        "\tcmp r4, #1\n"
        "\tbhs .Lhs_report_choose_day_suffix_growth\n"
        "\tadds r0, r1, #0\n"
        ".Lhs_report_choose_day_suffix_growth:\n"
        "\tldr r0, [r0]\n"
        "\tadds r0, r4, r0\n"
        "\tcmp r0, #0\n"
        "\tbeq .Lhs_report_empty_day_suffix_capacity\n"
        "\tlsls r4, r0, #4\n"
        "\tadds r0, r4, #0\n"
        "\tbl malloc\n"
        "\tmov r8, r4\n"
        "\tcmp r0, #0\n"
        "\tbne .Lhs_report_have_day_suffix_buffer\n"
        "\tmov r0, r8\n"
        "\tbl func_080D3BC0\n"
        "\tb .Lhs_report_have_day_suffix_buffer\n"
        ".Lhs_report_empty_day_suffix_capacity:\n"
        "\tmovs r0, #0\n"
        "\tmov r8, r0\n"
        ".Lhs_report_have_day_suffix_buffer:\n"
        "\tadds r5, r0, #0\n"
        "\tstr r5, [sp, #.Lhs_report_sp_dma_new_buffer]\n"
        "\tldr r2, [r6]\n"
        "\tadds r3, r5, #0\n"
        "\tldr r4, [sp, #.Lhs_report_sp_report]\n"
        "\tmovs r5, #(.Lhs_report_days_left_bitmap >> 3)\n"
        "\tlsls r5, r5, #3\n"
        "\tadds r4, r4, r5\n"
        "\tmov sl, r4\n"
        "\tldr r0, .Lhs_report_grown_work_pixels_offset\n"
        "\tldr r7, [sp, #.Lhs_report_sp_report]\n"
        "\tadds r0, r7, r0\n"
        "\tstr r0, [sp, #0x10c]\n"
        "\tldr r1, .Lhs_report_grown_not_assigned_pixels_offset\n"
        "\tadds r1, r7, r1\n"
        "\tstr r1, [sp, #0x130]\n"
        "\tldr r4, [sp, #.Lhs_report_sp_dma_old_end]\n"
        "\tcmp r2, r4\n"
        "\tbeq .Lhs_report_insert_day_suffix_command\n"
        ".Lhs_report_copy_old_day_suffix_command:\n"
        "\tcmp r3, #0\n"
        "\tbeq .Lhs_report_next_old_day_suffix_command\n"
        "\tadds r0, r3, #0\n"
        "\tadds r1, r2, #0\n"
        "\tldm r1!, {r4, r5, r7}\n"
        "\tstm r0!, {r4, r5, r7}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".Lhs_report_next_old_day_suffix_command:\n"
        "\tadds r2, #0x10\n"
        "\tadds r3, #0x10\n"
        "\tldr r5, [sp, #.Lhs_report_sp_dma_old_end]\n"
        "\tcmp r2, r5\n"
        "\tbne .Lhs_report_copy_old_day_suffix_command\n"
        ".Lhs_report_insert_day_suffix_command:\n"
        "\tadds r5, r3, #0\n"
        "\tldr r0, [sp, #0x30]\n"
        "\tcmp r0, #1\n"
        "\tbne .Lhs_report_insert_many_day_suffix_commands\n"
        "\tcmp r5, #0\n"
        "\tbeq .Lhs_report_advance_day_suffix_tail\n"
        "\tadds r0, r5, #0\n"
        "\tmov r1, sb\n"
        "\tldm r1!, {r2, r3, r7}\n"
        "\tstm r0!, {r2, r3, r7}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".Lhs_report_advance_day_suffix_tail:\n"
        "\tadds r5, #0x10\n"
        "\tb .Lhs_report_commit_day_suffix_buffer\n"
        "\t.align 2, 0\n"
        ".Lhs_report_grown_work_pixels_offset: .4byte .Lhs_report_work_label_bitmap\n"
        ".Lhs_report_grown_not_assigned_pixels_offset: .4byte .Lhs_report_not_assigned_bitmap\n"
        ".Lhs_report_insert_many_day_suffix_commands:\n"
        "\tadds r3, r0, #0\n"
        "\tadds r2, r5, #0\n"
        "\tcmp r3, #0\n"
        "\tbeq .Lhs_report_finish_day_suffix_copies\n"
        ".Lhs_report_copy_next_day_suffix_command:\n"
        "\tcmp r2, #0\n"
        "\tbeq .Lhs_report_advance_day_suffix_copy\n"
        "\tadds r0, r2, #0\n"
        "\tmov r1, sb\n"
        "\tldm r1!, {r4, r5, r7}\n"
        "\tstm r0!, {r4, r5, r7}\n"
        "\tldr r1, [r1]\n"
        "\tstr r1, [r0]\n"
        ".Lhs_report_advance_day_suffix_copy:\n"
        "\tsubs r3, #1\n"
        "\tadds r2, #0x10\n"
        "\tcmp r3, #0\n"
        "\tbne .Lhs_report_copy_next_day_suffix_command\n"
        ".Lhs_report_finish_day_suffix_copies:\n"
        "\tadds r5, r2, #0\n"
        ".Lhs_report_commit_day_suffix_buffer:\n"
        "\tCommitHarvestSpriteReportDmaBuffer r1, reload_r0\n"
        ".Lhs_report_upload_days_left_label:\n"
        ".syntax divided\n" : : : "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "lr", "cc", "memory");

    HARVEST_RENDER_LABEL(days_left, 4, ".Lhs_report_days_left_text", "mov r1, sl");

    asm volatile (
        ".syntax unified\n"
        "\tldr r5, [sp, #.Lhs_report_sp_report]\n"
        "\tldr r0, [r5]\n"
        ".syntax divided\n" : : : "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "lr", "cc", "memory");

    HARVEST_GET_QUEUE();

    asm volatile (
        ".syntax unified\n"
        "\tmovs r3, #0x80\n"
        "\tlsls r3, r3, #1\n"
        "\tadds r6, r0, #0\n"
        "\tmovs r7, #0x3c\n"
        "\tlsls r2, r7, #5\n"
        "\tmovs r0, #(.Lhs_report_bg_tile_vram >> 19)\n"
        "\tlsls r0, r0, #0x13\n"
        "\tadds r2, r2, r0\n"
        "\tadd r0, sp, #.Lhs_report_sp_copy_command\n"
        "\tmov r1, sl\n"
        ".syntax divided\n" : : : "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "lr", "cc", "memory");

    HARVEST_INIT_COPY();

    HARVEST_APPEND_COPY("r0", "r2, r3, r4", ".Lhs_report_grow_days_left_label", ".Lhs_report_publish_days_left");

    asm volatile (
        ".syntax unified\n"
        "\tb .Lhs_report_upload_work_label\n"
        "\t.align 2, 0\n"
        ".Lhs_report_days_left_text: .4byte gHarvestSpriteReportDaysLeftLabel\n"
        ".Lhs_report_grow_days_left_label:\n"
        ".syntax divided\n" : : : "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "lr", "cc", "memory");

    HARVEST_GROW_QUEUE(Label, "_site1", "0x38, r1", false);

    asm volatile (
        ".syntax unified\n"
        ".Lhs_report_upload_work_label:\n"
        ".syntax divided\n" : : : "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "lr", "cc", "memory");

    HARVEST_RENDER_LABEL(work, 8, ".Lhs_report_work_text", "ldr r1, [sp, #0x10c]");

    asm volatile (
        ".syntax unified\n"
        "\tldr r5, [sp, #.Lhs_report_sp_report]\n"
        "\tldr r0, [r5]\n"
        ".syntax divided\n" : : : "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "lr", "cc", "memory");

    HARVEST_GET_QUEUE();

    asm volatile (
        ".syntax unified\n"
        "\tmovs r3, #0x80\n"
        "\tlsls r3, r3, #2\n"
        "\tadds r6, r0, #0\n"
        "\tmovs r7, #0x44\n"
        "\tlsls r2, r7, #5\n"
        "\tmovs r0, #(.Lhs_report_bg_tile_vram >> 19)\n"
        "\tlsls r0, r0, #0x13\n"
        "\tadds r2, r2, r0\n"
        "\tadd r0, sp, #.Lhs_report_sp_copy_command\n"
        "\tldr r1, [sp, #0x10c]\n"
        ".syntax divided\n" : : : "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "lr", "cc", "memory");

    HARVEST_INIT_COPY();

    HARVEST_APPEND_COPY("r0", "r2, r3, r4", ".Lhs_report_grow_work_label", ".Lhs_report_publish_work");

    asm volatile (
        ".syntax unified\n"
        "\tb .Lhs_report_upload_not_assigned_label\n"
        "\t.align 2, 0\n"
        ".Lhs_report_work_text: .4byte gHarvestSpriteReportWorkLabel\n"
        ".Lhs_report_grow_work_label:\n"
        ".syntax divided\n" : : : "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "lr", "cc", "memory");

    HARVEST_GROW_QUEUE(Label, "_site2", "0x44, r1", false);

    asm volatile (
        ".syntax unified\n"
        ".Lhs_report_upload_not_assigned_label:\n"
        ".syntax divided\n" : : : "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "lr", "cc", "memory");

    HARVEST_RENDER_LABEL(not_assigned, 12, ".Lhs_report_not_assigned_text", "ldr r1, [sp, #0x130]");

    asm volatile (
        ".syntax unified\n"
        "\tldr r5, [sp, #.Lhs_report_sp_report]\n"
        "\tldr r0, [r5]\n"
        ".syntax divided\n" : : : "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "lr", "cc", "memory");

    HARVEST_GET_QUEUE();

    asm volatile (
        ".syntax unified\n"
        "\tmovs r3, #0xc0\n"
        "\tlsls r3, r3, #2\n"
        "\tadds r6, r0, #0\n"
        "\tmovs r7, #0x54\n"
        "\tlsls r2, r7, #5\n"
        "\tmovs r0, #(.Lhs_report_bg_tile_vram >> 19)\n"
        "\tlsls r0, r0, #0x13\n"
        "\tadds r2, r2, r0\n"
        "\tadd r0, sp, #.Lhs_report_sp_copy_command\n"
        "\tldr r1, [sp, #0x130]\n"
        ".syntax divided\n" : : : "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "lr", "cc", "memory");

    HARVEST_INIT_COPY();

    HARVEST_APPEND_COPY("r0", "r2, r3, r4", ".Lhs_report_grow_not_assigned_label", ".Lhs_report_publish_not_assigned");

    asm volatile (
        ".syntax unified\n"
        "\tb .Lhs_report_upload_friendship_tiles\n"
        "\t.align 2, 0\n"
        ".Lhs_report_not_assigned_text: .4byte gHarvestSpriteReportNotAssignedLabel\n"
        ".Lhs_report_grow_not_assigned_label:\n"
        ".syntax divided\n" : : : "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "lr", "cc", "memory");

    HARVEST_GROW_QUEUE(Label, "_site3", "0x50, r1", false);

    HARVEST_REPORT_TILE(friendship_tiles, ".Lhs_report_upload_friendship_palette", "r2", gHarvestSpriteReportFriendshipIconTiles,
        0x6c, 4 * TILE_BYTES, "r4", "r2, r5, r7", SmallIcon, "0x58, r3, r7");

    HARVEST_REPORT_LINEAR(friendship_palette, ".Lhs_report_upload_small_watering_icon", "r1", gHarvestSpriteReportFriendshipIconPalette,
        ".Lhs_report_bg_palette_ram + .Lhs_report_palette_bytes", PALETTE_BYTES, "r2", "r3, r4, r5", SmallIcon, "0x60, r7, r7");

    HARVEST_REPORT_TILE(small_watering_icon, ".Lhs_report_upload_small_harvesting_icon", "r1", gHarvestSpriteReportSmallWateringIconTiles,
        0x70, 1 * TILE_BYTES, "r4", "r2, r5, r7", SmallIcon, "0x68, r3, r7");

    HARVEST_REPORT_TILE(small_harvesting_icon, ".Lhs_report_upload_small_animal_care_icon", "r1", gHarvestSpriteReportSmallHarvestingIconTiles,
        0x71, 1 * TILE_BYTES, "r4", "r2, r5, r7", SmallIcon, "0x70, r3, r7");

    HARVEST_REPORT_TILE(small_animal_care_icon, ".Lhs_report_upload_watering_task_tiles", "r1", gHarvestSpriteReportSmallAnimalCareIconTiles,
        0x72, 1 * TILE_BYTES, "r4", "r2, r5, r7", SmallIcon, "0x78, r3, r7");

    HARVEST_REPORT_TILE(watering_task_tiles, ".Lhs_report_upload_harvesting_task_tiles", "r1", gHarvestSpriteReportWateringTaskIconTiles,
        0x73, 4 * TILE_BYTES, "r4", "r2, r5, r7", SmallIcon, "0x80, r3, r7");

    HARVEST_REPORT_TILE(harvesting_task_tiles, ".Lhs_report_upload_animal_care_task_tiles", "r1", gHarvestSpriteReportHarvestingTaskIconTiles,
        0x77, 4 * TILE_BYTES, "r4", "r2, r5, r7", Tile, "0x88, r3, r7");

    HARVEST_REPORT_TILE(animal_care_task_tiles, ".Lhs_report_upload_task_icon_palette", "r1", gHarvestSpriteReportAnimalCareTaskIconTiles,
        0x7b, 4 * TILE_BYTES, "r4", "r2, r5, r7", Tile, "0x90, r3, r7");

    HARVEST_REPORT_LINEAR(task_icon_palette, ".Lhs_report_upload_watering_skill_tiles", "r1", gHarvestSpriteReportTaskIconPalette,
        ".Lhs_report_bg_palette_ram + 2 * .Lhs_report_palette_bytes", PALETTE_BYTES, "r2", "r3, r4, r5", Palette, "0x98, r1");

    HARVEST_REPORT_TILE(watering_skill_tiles, ".Lhs_report_upload_harvesting_skill_tiles", "r2", gHarvestSpriteReportWateringSkillBarTiles,
        0x9c, 9 * TILE_BYTES, "r4", "r2, r5, r7", Tile, "0xa0, r3, r7");

    HARVEST_REPORT_TILE(harvesting_skill_tiles, ".Lhs_report_upload_animal_care_skill_tiles", "r1", gHarvestSpriteReportHarvestingSkillBarTiles,
        0xa5, 9 * TILE_BYTES, "r4", "r2, r5, r7", Tile, "0xa8, r3, r7");

    HARVEST_REPORT_TILE(animal_care_skill_tiles, ".Lhs_report_upload_skill_bar_palette", "r1", gHarvestSpriteReportAnimalCareSkillBarTiles,
        0xae, 9 * TILE_BYTES, "r4", "r2, r5, r7", Tile, "0xb0, r3, r7");

    HARVEST_REPORT_LINEAR(skill_bar_palette, ".Lhs_report_upload_skill_marker_obj_tile", "r1", gHarvestSpriteReportSkillBarPalette,
        ".Lhs_report_bg_palette_ram + 3 * .Lhs_report_palette_bytes", PALETTE_BYTES, "r2", "r3, r4, r5", Palette, "0xb8, r1");

    HARVEST_REPORT_LINEAR(skill_marker_obj_tile, ".Lhs_report_upload_skill_marker_obj_palette", "r2", gHarvestSpriteReportSkillMarkerTiles,
        ".Lhs_report_obj_tile_vram + 5 * .Lhs_report_tile_bytes", 1 * TILE_BYTES, "r3", "r4, r5, r7", Tile, "0xc0, r2, r7");

    HARVEST_REPORT_LINEAR(skill_marker_obj_palette, ".Lharvest_sprite_report_start_rows", "r1", gHarvestSpriteReportSkillMarkerPalette,
        ".Lhs_report_obj_palette_ram + .Lhs_report_palette_bytes", PALETTE_BYTES, "r2", "r3, r4, r5", Palette, "0xc8, r1");

    asm volatile (
        ".syntax unified\n"
        ".Lharvest_sprite_report_start_rows:\n"
        "\tmovs r2, #4\n"
        "\tmov r8, r2\n"
        "\tmovs r3, #0\n"
        "\tstr r3, [sp, #.Lhs_report_sp_roster_index]\n"
        "\n"
        "\t.purgem CommitHarvestSpriteReportDmaBuffer\n"
        ".syntax divided\n" : : : "r0", "r1", "r2", "r3", "r4", "r5", "r6", "r7", "r8", "r9", "r10", "lr", "cc", "memory");
}
