#include "g1_map_resources.hh"
#include <string.h>

EC char const vtable_unk_080E82D8[];
EC void TileHeapOwner_Initialize(G1MapTileOwner *);
EC u32 func_08007B54(G1MapTileOwner *, u32);
EC u32 TileHandle_GetTileIndex(G1MapTileOwner *, u32);
EC void TileHandle_Release(G1MapTileOwner *, u32);
EC void TileHeapOwner_Destroy(G1MapTileOwner *, u32);
EC char const vtable_unk_080E681C[];

EC SECTION(".text.g1_080A46AC") G1MapSpriteResource * InitializeMapSpriteResource(G1MapSpriteResource * self, G1MapGraphics * graphics, u32 tile_count, u32 const * palette_ids, u32 palette_count)
{
    self->vtable = vtable_unk_080E82D8;
    self->graphics = graphics;
    G1MapTileOwner * tiles = &self->tiles;
    TileHeapOwner_Initialize(tiles);
    u32 handle = func_08007B54(tiles, tile_count);
    tiles->handle = handle;
    self->tile_index = TileHandle_GetTileIndex(tiles, handle);
    volatile u8 initial_palette = 0;
    self->palette_count = 0;
    if (palette_count <= 16)
    {
        memset(self->palettes, initial_palette, palette_count);
        self->palette_count = palette_count;
    }
    u32 const * palette_source = palette_ids;
    u32 i = 0;
    if (i < palette_count)
    {
        u8 * palette = self->palettes;
        do
        {
            *palette = graphics->vtable->allocate_palette(graphics, *palette_source++);
            palette++;
            i++;
        }
        while (i < palette_count);
    }
    return self;
}

EC SECTION(".text.g1_080A4740") G1MapSpriteResource * InitializeMapSpriteSinglePalette(G1MapSpriteResource * self, G1MapGraphics * graphics, u32 tile_count, u32 palette_id)
{
    self->vtable = vtable_unk_080E82D8;
    self->graphics = graphics;
    G1MapTileOwner * tiles = &self->tiles;
    TileHeapOwner_Initialize(tiles);
    u32 handle = func_08007B54(tiles, tile_count);
    tiles->handle = handle;
    self->tile_index = TileHandle_GetTileIndex(tiles, handle);
    u32 palette_count = 1;
    volatile u8 initial_palette = 0;
    self->palette_count = 0;
    memset(self->palettes, initial_palette, palette_count);
    self->palette_count = palette_count;
    self->palettes[0] = graphics->vtable->allocate_palette(graphics, palette_id);
    return self;
}

EC SECTION(".text.g1_080A47B4") void DestroyMapSpriteResource(G1MapSpriteResource * self, u32 flags)
{
    self->vtable = vtable_unk_080E82D8;
    u32 i = 0;
    if (i < self->palette_count)
    {
        u8 const * palette = self->palettes;
        do
        {
            self->graphics->vtable->release_palette(self->graphics, *palette);
            palette++;
            i++;
        }
        while (i < self->palette_count);
    }
    G1MapTileOwner * tiles = &self->tiles;
    TileHandle_Release(tiles, tiles->handle);
    TileHeapOwner_Destroy(tiles, 2);
    if (flags & 1)
        delete self;
}

EC SECTION(".text.g1_080A4944") void LoadMapSpriteResourcePalettes(G1MapSpriteResource * self, G1MapResourceData const * resource, u8 mode)
{
    u8 const * source = static_cast<u8 const *>(resource->data);
    u32 size = 0;
    if (source != nullptr)
        size = resource->size;
    if (size != 0)
    {
        G1MapGraphics * graphics = self->graphics;
        u32 count = size / 32;
        u8 const * palette = source;
        u32 i = 0;
        if (i < count)
        {
            u8 const * palette_id = self->palettes;
            do
            {
                graphics->vtable->load_palette(graphics, palette, *palette_id, mode);
                palette_id++;
                i++;
                palette += 32;
            }
            while (i < count);
        }
    }
}

EC SECTION(".text.g1_080A49A0") G1AnimatedMapSpriteResource * InitializeAnimatedMapSpriteResource(G1AnimatedMapSpriteResource * self, SpriteAnimationSource const * bank, u32 clip, G1MapGraphics * graphics, u32 tiles, u32 const * palettes, u32 palette_count, bool mode)
{
    InitializeMapSpriteResource(self, graphics, tiles, palettes, palette_count);
    self->vtable = vtable_unk_080E681C;
    InitSpriteAnimator(&self->animator, bank, clip, 256);
    self->refresh_frame = 1;
    self->mode = mode;
    self->dirty = false;
    return self;
}

EC SECTION(".text.g1_080A4A00") G1AnimatedMapSpriteResource * InitializeAnimatedMapSpriteSinglePalette(G1AnimatedMapSpriteResource * self, SpriteAnimationSource const * bank, u32 clip, G1MapGraphics * graphics, u32 tiles, u32 palette, bool mode)
{
    InitializeMapSpriteSinglePalette(self, graphics, tiles, palette);
    self->vtable = vtable_unk_080E681C;
    InitSpriteAnimator(&self->animator, bank, clip, 256);
    self->refresh_frame = 1;
    self->mode = mode;
    self->dirty = false;
    return self;
}
