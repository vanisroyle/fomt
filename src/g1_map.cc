#include "g1_map.hh"

EC MapData gMapDefinitions[];
EC u32 GetMapAvailabilityBitIndex(G1MapAvailability const *, u32, int);

EC G1MapPatch const gMapPatch_08106A7C;
EC G1MapPatch const gMapPatch_08106A94;
EC G1MapPatch const gMapPatch_08106AAC;
EC G1MapPatch const gMapPatch_08106AC4;
EC G1MapPatch const gMapPatch_08106ADC;
EC G1MapPatch const gMapPatch_08106AF4;
EC G1MapPatch const gMapPatch_08106B3C;
EC G1MapPatch const gMapPatch_08106B24;
EC G1MapPatch const gMapBuildingDoorPatch;

EC SECTION(".text.g1_080A4698") MapData * GetMapData(u32 map) // 0x080A4698
{
    return gMapDefinitions + map;
}

EC SECTION(".text.g1_080A1C94") void ClearMapAvailabilityBit(G1MapAvailability * self, u32 map, int category) // 0x080A1C94
{
    u32 bit = GetMapAvailabilityBitIndex(self, map, category);
    u32 byte = bit >> 3;
    if (byte <= 6)
        self->bits[byte] &= ~(1 << (bit & 7));
}

EC SECTION(".text.g1_080A1EA8") u32 GetMapAvailabilityBitIndex(G1MapAvailability const *, u32 map, int category) // 0x080A1EA8
{
    if (map <= 0x5f)
    {
        switch (category)
        {
        case 0: break;
        case 1: map -= 0x1a; break;
        case 2: map -= 0x3a; break;
        case 3: map -= 0x5e; break;
        }
    }
    else
        map -= 0x3c;
    return map;
}

EC SECTION(".text.g1_080A6138") void ApplyMapRow15PatchA(G1MapState * self) // 0x080A6138
{
    switch (self->map_id)
    {
    case 0x29: ApplyMapPatch(self, &gMapPatch_08106A7C, 25, 15); break;
    case 0x2a: ApplyMapPatch(self, &gMapPatch_08106A7C, 40, 15); break;
    case 0x2b: ApplyMapPatch(self, &gMapPatch_08106A7C, 54, 15); break;
    }
    self->tilemaps_dirty = true;
}

EC SECTION(".text.g1_080A6194") void ApplyMapRow15PatchB(G1MapState * self) // 0x080A6194
{
    switch (self->map_id)
    {
    case 0x29: ApplyMapPatch(self, &gMapPatch_08106A94, 25, 15); break;
    case 0x2a: ApplyMapPatch(self, &gMapPatch_08106A94, 40, 15); break;
    case 0x2b: ApplyMapPatch(self, &gMapPatch_08106A94, 54, 15); break;
    }
    self->tilemaps_dirty = true;
}

EC SECTION(".text.g1_080A61F0") void ApplyMapRow3PatchA(G1MapState * self) // 0x080A61F0
{
    switch (self->map_id)
    {
    case 0x2a: ApplyMapPatch(self, &gMapPatch_08106AAC, 11, 3); break;
    case 0x2b: ApplyMapPatch(self, &gMapPatch_08106AAC, 19, 3); break;
    }
    self->tilemaps_dirty = true;
}

EC SECTION(".text.g1_080A6234") void ApplyMapRow3PatchB(G1MapState * self) // 0x080A6234
{
    switch (self->map_id)
    {
    case 0x2a: ApplyMapPatch(self, &gMapPatch_08106AC4, 11, 3); break;
    case 0x2b: ApplyMapPatch(self, &gMapPatch_08106AC4, 19, 3); break;
    }
    self->tilemaps_dirty = true;
}

EC SECTION(".text.g1_080A6278") void ApplyMapRow2PatchA(G1MapState * self) // 0x080A6278
{
    switch (self->map_id)
    {
    case 0x2a: ApplyMapPatch(self, &gMapPatch_08106ADC, 15, 2); break;
    case 0x2b: ApplyMapPatch(self, &gMapPatch_08106ADC, 23, 2); break;
    }
    self->tilemaps_dirty = true;
}

EC SECTION(".text.g1_080A62BC") void ApplyMapRow2PatchB(G1MapState * self) // 0x080A62BC
{
    switch (self->map_id)
    {
    case 0x2a: ApplyMapPatch(self, &gMapPatch_08106AF4, 15, 2); break;
    case 0x2b: ApplyMapPatch(self, &gMapPatch_08106AF4, 23, 2); break;
    }
    self->tilemaps_dirty = true;
}

EC SECTION(".text.g1_080A6300") void ApplyMapRow17PatchA(G1MapState * self) // 0x080A6300
{
    switch (self->map_id)
    {
    case 0x29: ApplyMapPatch(self, &gMapPatch_08106B3C, 1, 17); break;
    case 0x2a: ApplyMapPatch(self, &gMapPatch_08106B3C, 16, 17); break;
    case 0x2b: ApplyMapPatch(self, &gMapPatch_08106B3C, 24, 17); break;
    default: return;
    }
    self->tilemaps_dirty = true;
}

EC SECTION(".text.g1_080A635C") void ApplyMapRow17PatchB(G1MapState * self) // 0x080A635C
{
    switch (self->map_id)
    {
    case 0x29: ApplyMapPatch(self, &gMapPatch_08106B24, 1, 17); break;
    case 0x2a: ApplyMapPatch(self, &gMapPatch_08106B24, 16, 17); break;
    case 0x2b: ApplyMapPatch(self, &gMapPatch_08106B24, 24, 17); break;
    default: return;
    }
    self->tilemaps_dirty = true;
}

EC SECTION(".text.g1_080A63B8") void ApplyMapBuildingDoorPatch(G1MapState * self, u32 expanded) // 0x080A63B8
{
    switch (self->map_id)
    {
    case 0x40: ApplyMapPatch(self, &gMapBuildingDoorPatch, 1, 13); break;
    case 0x24: ApplyMapPatch(self, &gMapBuildingDoorPatch, 1, 17); break;
    case 0x25:
        ApplyMapPatch(self, &gMapBuildingDoorPatch, 1, expanded == 0 ? 17 : 13);
        break;
    default: return;
    }
    self->tilemaps_dirty = true;
}

EC G1MapPatch const gMapIndexedFixturePatch;
EC u8 const gMapFixtureColumns[];
EC u8 const gMapSmallFixtureColumns[];

EC SECTION(".text.g1_080A6024") void ApplyMapIndexedFixturePatch(G1MapState * self, u32 index) // 0x080A6024
{
    switch (self->map_id)
    {
    case 0x24:
    case 0x25:
        ApplyMapPatch(self, &gMapIndexedFixturePatch, gMapFixtureColumns[index], 8);
        break;
    case 0x40:
        ApplyMapPatch(self, &gMapIndexedFixturePatch, gMapSmallFixtureColumns[index], 4);
        break;
    }
    self->tilemaps_dirty = true;
}
