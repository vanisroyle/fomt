#include "g1_map.hh"

struct G1TilemapDmaLayer
{
    void const * source;
    void * destination;
};

struct G1TilemapDmaTask
{
    G1MapTaskNode node;
    G1TilemapDmaLayer layers[3];
    u16 width;
};

EC void Dma_ExecuteTransfer(void const *, void *, u32, void volatile *);

EC SECTION(".text.g1_080A5670") u32 CopyMapTilemapRows(G1TilemapDmaTask * self)
{
    u32 stride = self->width * 2;
    register void volatile * dma = reinterpret_cast<void volatile *>(0x040000d4);
    u32 layer = 0;
    do
    {
        void const * data = self->layers[layer].source;
        if (data != nullptr)
        {
            u8 const * source = static_cast<u8 const *>(data);
            u8 * destination = static_cast<u8 *>(self->layers[layer].destination);
            u8 * end = destination + 0x540;
            do
            {
                Dma_ExecuteTransfer(source, destination, 0x8000001f, dma);
                source += stride;
                destination += 0x40;
            } while (destination != end);
        }
        layer += 1;
    } while (layer <= 2);
    return 0;
}
