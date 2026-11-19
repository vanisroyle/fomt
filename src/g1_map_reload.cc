#include "g1_map.hh"
#include <string.h>

EC void Unpack(void const *, void *);

struct G1MapPaletteKey
{
    u16 minute;
    u8 palette;
    u8 reserved;
};

EC G1MapPaletteKey const * gUnk_08105708[][4];
EC G1MapPatch const gUnk_08106C74;
EC u8 const gUnk_081073D0[][2];
EC u8 const gUnk_081073F0[][2];
EC u8 const gUnk_08107400[][2];
EC u8 const gUnk_08107420[];

EC SECTION(".text.g1_080A4650") u32 GetMapTimePalette(u8 hour, u8 minute, u32 season, u32 weather)
{
    G1MapPaletteKey const * key = gUnk_08105708[season][weather];
    u16 now = hour * 60 + minute;
    u32 palette = 0;
    u32 key_minute = key->minute;
    do
    {
        if (key_minute == 0xffff)
            break;
        if (key_minute > now)
            break;
        palette = key->palette;
        key++;
        key_minute = key->minute;
    }
    while (true);
    return palette;
}

EC SECTION(".text.g1_080A5BD8") void ApplyMapPatch(G1MapState * self, G1MapPatch const * patch, u32 x, u32 y)
{
    u32 map_width = GetMapData(self->map_id)->width;
    u32 width = patch->width;
    u32 height = patch->height;
    u16 const * const * tilemaps = patch->tilemaps;
    for (u32 layer = 0; layer < 3; layer++)
    {
        if (*(layer + tilemaps) != nullptr)
        {
            for (u32 row = 0; row < height; row++)
                memcpy(self->tilemaps[layer] + x + (y + row) * map_width,
                    *(layer + tilemaps) + row * width, width * 2);
        }
    }
    if (patch->terrain_map != nullptr && patch->terrain != nullptr)
    {
        for (u32 row = 0; row < height; row++)
        {
            u8 const * indices = patch->terrain_map + row * width;
            TerrainInfo * destination = self->terrain + x + (y + row) * map_width;
            for (u32 column = 0; column < width; column++)
                *destination++ = patch->terrain[*indices++];
        }
    }
}

EC SECTION(".text.g1_080A5D14") void ReloadMapPalettePair(G1MapState * self, int mode)
{
    switch (mode)
    {
    default: break;
    case 3:
        self->vtable->reload_palette(self, 3, 0);
        self->vtable->reload_palette(self, 0, 1);
        break;
    case 0:
        self->vtable->reload_palette(self, 0, 0);
        self->vtable->reload_palette(self, 1, 1);
        break;
    case 1:
        self->vtable->reload_palette(self, 1, 0);
        self->vtable->reload_palette(self, 2, 1);
        break;
    case 2:
        self->vtable->reload_palette(self, 2, 0);
        self->vtable->reload_palette(self, 3, 1);
        break;
    }
    self->palettes_dirty = true;
}

EC SECTION(".text.g1_080A5DB8") void ReloadMapPalette(G1MapState * self, int mode, u32 index)
{
    MapData const * map = GetMapData(self->map_id);
    Unpack(map->packed_pal1, self->palettes[index]);
    switch (mode)
    {
    case 0:
    case 1:
    case 2:
        Unpack(map->packed_pal1, self->palettes[index]);
        break;
    case 3:
        Unpack(map->packed_pal2, self->palettes[index]);
        break;
    }
}

EC SECTION(".text.g1_080A607C") void ApplyMapIndexedOverlayPatch(G1MapState * self, u32 index)
{
    switch (self->map_id)
    {
    case 0x26:
        if (index > 15)
            ApplyMapPatch(self, &gUnk_08106C74, gUnk_081073D0[index - 16][0], gUnk_081073D0[index - 16][1]);
        else
            ApplyMapPatch(self, &gUnk_08106C74, gUnk_081073F0[index][0], gUnk_081073F0[index][1]);
        break;
    case 0x27:
        if (index > 15)
            ApplyMapPatch(self, &gUnk_08106C74, gUnk_081073D0[index - 16][0], gUnk_081073D0[index - 16][1]);
        else
            ApplyMapPatch(self, &gUnk_08106C74, gUnk_08107400[index][0], gUnk_08107400[index][1]);
        break;
    case 0x41:
        ApplyMapPatch(self, &gUnk_08106C74, gUnk_08107420[index], 5);
        break;
    }
    self->tilemaps_dirty = true;
}
