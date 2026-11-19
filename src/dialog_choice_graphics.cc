#include "dialog_choice_graphics.hh"
#include "sprite_animation.hh"

#include <string.h>

#define MATCH(addr) EC SECTION(".text." #addr)
EC void * __builtin_new(size_t);
EC char const gChoiceInterfaceSprites[] asm("gUnk_0875B444");
EC char const gChoiceIndicatorSprites[] asm("gUnk_086678A0");
EC char const gChoiceUploadTaskVTable[] asm("vtable_unk_080E77D0");
enum { CHOICE_TILE_BYTES = 32, INITIAL_CHOICE_FONT_TILE = 29 };
typedef u8 ChoiceTile[CHOICE_TILE_BYTES];

EC ChoiceTile const gClockFont[];
EC void * CopyChoiceTileBytes(void *, void const *, size_t) asm("memcpy");

EC SpriteAnimationData * InitSpriteAnimationData(SpriteAnimationData *, void const *);

struct ChoicePaletteAllocation
{
    STRUCT_PAD(0, 4);
    u32 handle;
};

struct ChoiceTileAllocation
{
    STRUCT_PAD(0, 4);
    u32 handle;
};

struct ResourceHandleOwner;
struct TileHeapOwner;

EC ResourceHandleOwner * ResourceHandles_Initialize(ResourceHandleOwner *);
EC u32 AllocateChoicePalette(ResourceHandleOwner *) asm("func_0800736C");
EC TileHeapOwner * InitializeChoiceTiles(TileHeapOwner *) asm("func_08007874");
EC u32 AllocateChoiceTiles(TileHeapOwner *, u32) asm("func_08007B54");

EC void DestroyDialogChoiceGraphicsStorage(DialogChoiceGraphics *, int);

struct ChoiceUploadTask
{
    ChoiceUploadTask * previous;
    ChoiceUploadTask * next;
    void const * vtable;
    void const * source;
    void * destination;
    unsigned length;
};

struct ChoiceUploadTaskWithFlag
{
    ChoiceUploadTask task;
    u8 needsUpload;
};

struct ChoiceCursorUploadTask
{
    ChoiceUploadTask task;
    u8 needsUpload;
    u8 showAlternateIndicator;
};

struct DialogChoiceGraphics
{
    u8 interfaceAnimation[0x30];
    u8 indicatorAnimation[0x30];
    ChoicePaletteAllocation interfacePalette;
    ChoiceUploadTask interfacePaletteUpload;
    u8 needsInterfacePaletteUpload;
    STRUCT_PAD(0x81, 0x84);
    ChoiceTile leftChoiceTileBuffer[8];
    ChoiceTileAllocation leftChoiceTiles;
    u8 needsLeftChoiceUpload;
    ChoiceUploadTask leftChoiceUpload;
    ChoiceTile rightChoiceTileBuffer[8];
    ChoiceTileAllocation rightChoiceTiles;
    u8 needsRightChoiceUpload;
    ChoiceUploadTask rightChoiceUpload;
    u8 cursorFrame;
    ChoiceTileAllocation cursorTiles;
    ChoiceCursorUploadTask cursorUpload;
    ChoicePaletteAllocation alternateIndicatorPalette;
    ChoiceUploadTask alternateIndicatorPaletteUpload;
    ChoiceTileAllocation alternateIndicatorTiles;
    ChoiceUploadTaskWithFlag alternateIndicatorUpload;
    ChoiceTileAllocation defaultIndicatorTiles;
    ChoiceUploadTaskWithFlag defaultIndicatorUpload;
};

typedef char DialogChoiceGraphicsSizeCheck[sizeof(DialogChoiceGraphics) == 0x35C ? 1 : -1];
typedef char LeftChoiceTileBufferOffsetCheck[offsetof(DialogChoiceGraphics, leftChoiceTileBuffer) == 0x84 ? 1 : -1];
typedef char RightChoiceTileBufferOffsetCheck[offsetof(DialogChoiceGraphics, rightChoiceTileBuffer) == 0x1A8 ? 1 : -1];
typedef char AlternateIndicatorFlagOffsetCheck[
    offsetof(DialogChoiceGraphics, cursorUpload.showAlternateIndicator) == 0x2F1 ? 1 : -1];

// Keep address formation and register allocation identical to the original Thumb code.
#define OFFSET_ADDRESS(Type, address, addrReg, fieldOffset, offReg, member, first, second) \
    register unsigned fieldOffset asm(#offReg) = offsetof(DialogChoiceGraphics, member); \
    asm("" : "+l"(fieldOffset)); \
    register Type * address asm(#addrReg); \
    asm(".syntax unified\n\tadds %0, %1, %2\n\t.syntax divided" \
        : "=l"(address) : "l"(first), "l"(second))

#define ALLOCATE_CHOICE_TILES(member, offReg, order) \
    do { \
        OFFSET_ADDRESS(ChoiceTileAllocation, resource, r5, offset, offReg, member, graphics, offset); \
        InitializeChoiceTiles(reinterpret_cast<TileHeapOwner *>(resource)); \
        resource->handle = AllocateChoiceTiles(reinterpret_cast<TileHeapOwner *>(resource), order); \
    } while (false)

#define TASK_CONTENTS(task, table, tableReg) \
    do { \
        (task)->previous = nullptr; \
        (task)->next = nullptr; \
        register void const * tableValue asm(#tableReg); \
        asm("mov %0, %1" : "=l"(tableValue) : "r"(table)); \
        (task)->vtable = tableValue; \
        (task)->source = nullptr; \
        (task)->destination = nullptr; \
        (task)->length = 0; \
    } while (false)

#define CHOICE_TASK(member, offReg, tableReg) \
    do { \
        OFFSET_ADDRESS(ChoiceUploadTask, task, r0, offset, offReg, member, graphics, offset); \
        TASK_CONTENTS(task, vtable, tableReg); \
    } while (false)

#define FLAGGED_CHOICE_TASK(member, offReg, tableReg, flagValue) \
    do { \
        OFFSET_ADDRESS(ChoiceUploadTask, task, r0, offset, offReg, member, graphics, offset); \
        TASK_CONTENTS(task, vtable, tableReg); \
        offset += sizeof(ChoiceUploadTask); \
        asm("" : "+l"(offset)); \
        register u8 * dirty asm("r0"); \
        asm(".syntax unified\n\tadds %0, %1, %2\n\t.syntax divided" \
            : "=l"(dirty) : "l"(graphics), "l"(offset)); \
        *dirty = flagValue; \
    } while (false)
MATCH(0803C5B0) DialogChoiceGraphicsOwner & InitializeDialogChoiceGraphics(DialogChoiceGraphicsOwner & owner)
{
    DialogChoiceGraphics * graphics = static_cast<DialogChoiceGraphics *>(__builtin_new(sizeof(DialogChoiceGraphics)));
    InitSpriteAnimationData(reinterpret_cast<SpriteAnimationData *>(graphics->interfaceAnimation), gChoiceInterfaceSprites);
    InitSpriteAnimationData(reinterpret_cast<SpriteAnimationData *>(graphics->indicatorAnimation), gChoiceIndicatorSprites);
    register ChoicePaletteAllocation * interfacePalette asm("r4") = &graphics->interfacePalette;
    asm("" : "+l"(interfacePalette));
    ResourceHandles_Initialize(reinterpret_cast<ResourceHandleOwner *>(interfacePalette));
    interfacePalette->handle = AllocateChoicePalette(reinterpret_cast<ResourceHandleOwner *>(interfacePalette));
    register ChoiceUploadTask * paletteTask asm("r0") = &graphics->interfacePaletteUpload;
    asm("" : "+l"(paletteTask));
    graphics->interfacePaletteUpload.previous = nullptr;
    paletteTask->next = nullptr;
    register void const * vtable asm("r8");
    register void const * choiceVTable asm("r1") = gChoiceUploadTaskVTable;
    asm("mov %0, %1" : "=r"(vtable), "+l"(choiceVTable));
    paletteTask->vtable = choiceVTable;
    paletteTask->source = nullptr;
    paletteTask->destination = nullptr;
    paletteTask->length = 0;
    register u8 * paletteDirty asm("r0") = reinterpret_cast<u8 *>(paletteTask) + sizeof(ChoiceUploadTask);
    asm("" : "+l"(paletteDirty));
    register u32 needsUpload asm("r6") = 1;
    asm("" : "+l"(needsUpload));
    *paletteDirty = needsUpload;
    ALLOCATE_CHOICE_TILES(leftChoiceTiles, r2, 3);
    register u8 * leftDirty asm("r10");
    OFFSET_ADDRESS(u8, initialLeftDirty, r0, leftDirtyOffset, r0, needsLeftChoiceUpload, leftDirtyOffset, graphics);
    asm("mov %0, %1" : "=r"(leftDirty), "+l"(initialLeftDirty));
    *initialLeftDirty = needsUpload;
    CHOICE_TASK(leftChoiceUpload, r1, r2);
    ALLOCATE_CHOICE_TILES(rightChoiceTiles, r0, 3);
    register u8 * rightDirty asm("r9");
    OFFSET_ADDRESS(u8, initialRightDirty, r1, rightDirtyOffset, r1, needsRightChoiceUpload, rightDirtyOffset, graphics);
    asm("mov %0, %1" : "=r"(rightDirty), "+l"(initialRightDirty));
    *initialRightDirty = needsUpload;
    FLAGGED_CHOICE_TASK(rightChoiceUpload, r2, r1, 0);
    ALLOCATE_CHOICE_TILES(cursorTiles, r0, 2);
    FLAGGED_CHOICE_TASK(cursorUpload, r1, r2, needsUpload);
    register unsigned alternateIndicatorVisibilityOffset asm("r2") = offsetof(DialogChoiceGraphics, cursorUpload.showAlternateIndicator);
    asm("" : "+l"(alternateIndicatorVisibilityOffset));
    register u8 * alternateIndicatorVisibility asm("r0") = reinterpret_cast<u8 *>(graphics) + alternateIndicatorVisibilityOffset;
    asm("" : "+l"(alternateIndicatorVisibility));
    *alternateIndicatorVisibility = 0;
    register ChoicePaletteAllocation * alternateIndicatorPalette asm("r5") = &graphics->alternateIndicatorPalette;
    asm("" : "+l"(alternateIndicatorPalette));
    ResourceHandles_Initialize(reinterpret_cast<ResourceHandleOwner *>(alternateIndicatorPalette));
    alternateIndicatorPalette->handle = AllocateChoicePalette(reinterpret_cast<ResourceHandleOwner *>(alternateIndicatorPalette));
    CHOICE_TASK(alternateIndicatorPaletteUpload, r1, r2);
    ALLOCATE_CHOICE_TILES(alternateIndicatorTiles, r0, 2);
    FLAGGED_CHOICE_TASK(alternateIndicatorUpload, r1, r2, needsUpload);
    ALLOCATE_CHOICE_TILES(defaultIndicatorTiles, r2, 2);
    FLAGGED_CHOICE_TASK(defaultIndicatorUpload, r1, r2, needsUpload);
    register u8 const * font asm("r1") = gClockFont[0];
    asm("" : "+l"(font));
    register unsigned fontTileOffset asm("r2") = INITIAL_CHOICE_FONT_TILE * CHOICE_TILE_BYTES;
    asm("" : "+l"(fontTileOffset));
    register u8 const * initialFontTile asm("r1") = font + fontTileOffset;
    asm("" : "+l"(initialFontTile));
    register unsigned initialTileOffset asm("r2") = offsetof(DialogChoiceGraphics, leftChoiceTileBuffer) + 5 * CHOICE_TILE_BYTES;
    asm("" : "+l"(initialTileOffset));
    CopyChoiceTileBytes(reinterpret_cast<u8 *>(graphics) + initialTileOffset, initialFontTile, CHOICE_TILE_BYTES);
    *leftDirty = needsUpload;
    u8 blankTile[CHOICE_TILE_BYTES];
    memset(blankTile, 0, sizeof(blankTile));
    register unsigned rightChoiceTile7Offset asm("r1") = offsetof(DialogChoiceGraphics, rightChoiceTileBuffer) + 7 * CHOICE_TILE_BYTES;
    asm("" : "+l"(rightChoiceTile7Offset));
    CopyChoiceTileBytes(reinterpret_cast<u8 *>(graphics) + rightChoiceTile7Offset, blankTile, sizeof(blankTile));
    register u8 * rightDirtyState asm("r2") = rightDirty;
    asm("" : "+l"(rightDirtyState));
    *rightDirtyState = needsUpload;
    CopyChoiceTileBytes(graphics->rightChoiceTileBuffer[3], blankTile, sizeof(blankTile));
    rightDirtyState = rightDirty;
    asm("" : "+l"(rightDirtyState));
    *rightDirtyState = needsUpload;
    register DialogChoiceGraphicsOwner * result asm("r0") = &owner;
    asm("" : "+l"(result));
    result->graphics = graphics;
    return *result;
}

MATCH(0803C7A0) void DestroyDialogChoiceGraphics(DialogChoiceGraphicsOwner & owner, int destructorFlags)
{
    DialogChoiceGraphics * graphics = owner.graphics;
    if (graphics != nullptr)
        DestroyDialogChoiceGraphicsStorage(graphics, 3);
    destructorFlags &= 1;
    if (destructorFlags != 0)
        ::operator delete(&owner);
}
