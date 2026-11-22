#include "g5_graphics.hh"
#include "g5_inventory.hh"
#include "dma_queue.hh"
#include <string.h>
#include <stdlib.h>
#include <new>

EC void Dma3_Fill(u32,void *,u32);
EC void *func_080D3BC0(u32);
struct MessageTileSize
{
    u16 width,height;
    MessageTileSize(u16 w,u16 h):width(w),height(h)
    {
    }
};
EC void FillMessageTiles(MessageTileSize,void *,u32);
EC void DrawMessageTileStringWithColors(MessageTileSize,void *,u32,u32,char const *,u32,u32);
static inline u32 const *SelectDmaGrowthSize(u32 size, u32 count, u32 const * size_address, u32 const * count_address)
{
    return size < count ? count_address : size_address;
}

static inline void AppendPanelCaptionTransfer(DmaTransferQueue *queue,DmaCommand const *command,DmaCommand const *input_value)
{
    register DmaCommand const *value asm("r10")=input_value;
    asm ("" : "+r"(value));
    register DmaCommand *end asm("r1")=queue->end;
    asm ("" : "+l"(end));
    register DmaCommand *capacity asm("r0")=queue->capacity_end;
    asm ("" : "+l"(capacity));
    if(end!=capacity)
    {
        if(end)
        {
            register DmaCommand *destination asm("r0")=end;
            asm ("" : "+l"(destination));
            register DmaCommand const *source asm("r1")=command;
            asm ("" : "+l"(source));
            *destination=*source;
        }
        ++queue->end;
    }
    else
    {
        u32 old_size, count;
        asm ("" : "=m"(old_size), "=m"(count));
        DmaCommand *old_end, *saved_allocation;
        old_end=end;
        asm ("" : "+m"(old_end));
        register u32 count_value asm("r5")=1;
        asm ("" : : "l"(count_value));
        count=count_value;
        register DmaCommand *begin asm("r1")=queue->begin;
        asm ("" : "+l"(begin));
        register DmaCommand *previous_end asm("r7")=old_end;
        asm ("" : "+l"(previous_end));
        register i32 difference asm("r0")=(u8 *)previous_end-(u8 *)begin;
        asm ("" : "+l"(difference));
        register u32 cached_size asm("r4")=difference>>4;
        asm ("" : "+l"(cached_size));
        old_size=cached_size;
        register u32 const *count_address asm("r2")=&count;
        asm ("" : "+l"(count_address));
        register u32 const *growth_address asm("r0")=&old_size;
        asm ("" : "+l"(growth_address));
        growth_address=SelectDmaGrowthSize(cached_size,1,growth_address,count_address);
        register u32 growth asm("r0")=*growth_address;
        asm ("" : "+l"(growth));
        register u32 size asm("r0")=cached_size+growth;
        asm ("" : "+l"(size));
        register u32 bytes asm("r9");
        register DmaCommand *allocation asm("r0");
        if(size!=0)
        {
            register u32 allocation_bytes asm("r4")=size*sizeof(DmaCommand);
            asm ("" : "+l"(allocation_bytes));
            allocation=(DmaCommand *)malloc(allocation_bytes);
            bytes=allocation_bytes;
            asm ("" : "+r"(bytes));
            if(allocation==0) allocation=(DmaCommand *)func_080D3BC0(bytes);
            begin=queue->begin;
        }
        else
        {
            allocation=0;
            bytes=0;
            asm ("" : "+r"(bytes));
        }
        register DmaCommand *output asm("r5")=allocation;
        asm ("" : "+l"(output));
        saved_allocation=output;
        asm ("" : "+m"(saved_allocation));
        register DmaCommand *input asm("r2")=begin;
        asm ("" : "+l"(input));
        register DmaCommand *copy_output asm("r3")=output;
        asm ("" : "+l"(copy_output));
        while(input!=old_end)
        {
            if(copy_output) *copy_output=*input;
            ++input;
            ++copy_output;
        }
        output=copy_output;
        asm ("" : "+l"(output));
        register u32 insert_count asm("r0")=count;
        asm ("" : "+l"(insert_count));
        if(insert_count==1)
        {
            if(output)
            {
                register DmaCommand *destination asm("r0")=output;
                asm ("" : "+l"(destination));
                register DmaCommand const *source asm("r1")=value;
                asm ("" : "+l"(source));
                register u32 reserve asm("r7");
                asm ("" : "=r"(reserve));
                *destination=*source;
                asm ("" : : "r"(reserve));
            }
            ++output;
        }
        else
        {
            register u32 remaining asm("r3")=insert_count;
            asm ("" : "+l"(remaining));
            register DmaCommand *tail asm("r2")=output;
            asm ("" : "+l"(tail));
            while(remaining!=0)
            {
                asm ("" : "+r"(value));
                if(tail)*tail=*value;
                --remaining;
                ++tail;
            }
            output=tail;
            asm ("" : "+l"(output));
        }
        register DmaCommand *destroy_end asm("r2")=queue->end;
        asm ("" : "+l"(destroy_end));
        register DmaCommand *destroy_begin asm("r1")=queue->begin;
        asm ("" : "+l"(destroy_begin));
        DmaCommand *it=destroy_begin;
        if(destroy_begin!=destroy_end) do
        {
            ++it;
        }
        while(it!=destroy_end);
        if(destroy_begin)free(destroy_begin);
        register u8 *new_end asm("r0")=(u8 *)saved_allocation;
        asm ("" : "+l"(new_end));
        new_end+=bytes;
        asm ("" : "+l"(new_end));
        DmaCommand *new_begin=saved_allocation;
        asm ("" : "+l"(new_begin) : "l"(queue), "l"(output), "l"(new_end) : "r1", "r2", "r3", "r4");
        queue->begin=new_begin;
        queue->end=output;
        queue->capacity_end=(DmaCommand *)new_end;
    }
}

static inline void *GetCountTileDestination(DrawingPosition const &position)
{
    u8 const *bytes=(u8 const *)&position;
    register u32 y asm("r1")=bytes[1]*64;
    asm ("" : "+l"(y));
    register u32 x asm("r0")=bytes[0]*2;
    asm ("" : "+l"(x));
    register u32 base asm("r2")=0x0600f800;
    asm ("" : "+l"(base));
    return (void *)(y+(x+base));
}

EC SECTION(".text.g5_080CB090") void InventoryMenu_RedrawLeftSlotPanel(u8 *input) // 0x080CB090
{
    DmaCommand command_storage;
    DrawingPosition position;
    register u8 *panel asm("r8")=input;
    asm ("" : "+r"(panel));
    register u8 *foreground asm("r6")=(u8 *)0x0600f000;
    asm ("" : "+l"(foreground));
    register u8 *background asm("r5")=(u8 *)0x0600f800;
    asm ("" : "+l"(background));
    register int row asm("r4")=19;
    asm ("" : "+l"(row));
    do
    {
        Dma3_Fill(0x00030003,background,0x1c);
        Dma3_Fill(0x03ff03ff,foreground,0x20);
        foreground+=0x40;
        background+=0x40;
        --row;
    }
    while(row>=0);
    position=gInventorySpecialSlotCountPosition;
    if(*(u8 *)(panel+0x27c))QueuePanelTileDraw(panel,GetCountTileDestination(position),0x1c,2,2,1,0x20);
    register u32 slot asm("r4")=0;
    asm ("" : "+l"(slot));
    while(slot<*(u32 *)(panel+0x214))
    {
        position=gInventorySlotCountPositions[slot];
        QueuePanelTileDraw(panel,GetCountTileDestination(position),0x1c,2,2,1,0x20);
        ++slot;
    }
    char *caption=(char *)panel+0x28c;
    u32 caption_empty=(*caption==0);
    register u32 zero asm("r9")=caption_empty;
    asm ("" : "+r"(zero));
    if(caption_empty==0)
    {
        MessageTileSize fill_size(8,2);
        register MessageTileSize fill_value asm("r0")=fill_size;
        asm ("" : "+r"(fill_value));
        u8 *pixels=panel+0x10;
        FillMessageTiles(fill_value,pixels,0);
        u32 offset=64-strlen(caption)*8;
        MessageTileSize text_size(8,2);
        DrawMessageTileStringWithColors(text_size,pixels,offset,0,caption,(
        {
            register u32 foreground_value asm("r3")=zero;
            asm ("" : "+l"(foreground_value));
            foreground_value;
        }
        ),(
        {
            register u32 background_value asm("r5")=1;
            asm ("" : "+l"(background_value));
            background_value;
        }
        ));
        register u32 transfer_bytes asm("r3")=0x200;
        asm ("" : "+l"(transfer_bytes));
        register DmaTransferQueue *queue asm("r6")=*(DmaTransferQueue **)(panel+0x278);
        asm ("" : "+l"(queue));
        register DmaCommand *command_destination asm("r0")=&command_storage;
        asm ("" : "+l"(command_destination));
        register u8 *command_source asm("r1")=pixels;
        asm ("" : "+l"(command_source));
        DmaCommand_InitializeCopy(command_destination,command_source,(void *)0x06000b00,transfer_bytes);
        DmaCommand *command=&command_storage;
        asm ("" : "+l"(command) : "l"(queue) : "r0", "r1", "r2", "r3", "r4", "r5");
        register DmaCommand const *entry asm("r10")=command;
        asm ("" : "+r"(entry));
        AppendPanelCaptionTransfer(queue,&command_storage,entry);
        QueuePanelTileDraw(panel,(void *)0x0600f00c,0x58,8,2,0,0x20);
    }
}

EC SECTION(".text.g5_080CC310") void InventoryMenu_RedrawRightSlotPanel(u8 *input) // 0x080CC310
{
    DmaCommand command_storage;
    DrawingPosition position;
    register u8 *panel asm("r8")=input;
    asm ("" : "+r"(panel));
    register u8 *foreground asm("r6")=(u8 *)0x0600f01e;
    asm ("" : "+l"(foreground));
    register u8 *background asm("r5")=(u8 *)0x0600f822;
    asm ("" : "+l"(background));
    register int row asm("r4")=19;
    asm ("" : "+l"(row));
    do
    {
        Dma3_Fill(0x00030003,background,0x1c);
        Dma3_Fill(0x03ff03ff,foreground,0x20);
        foreground+=0x40;
        background+=0x40;
        --row;
    }
    while(row>=0);
    {
        register u32 packed asm("r1")=*(u32 const *)&gInventorySpecialSlotCountPosition;
        asm ("" : "+l"(packed));
        register u32 x asm("r0")=packed+15;
        x=(u8)x;
        asm ("" : "+l"(x));
        packed&=0xffffff00;
        packed|=x;
        *(u32 *)&position=packed;
    }
    if(*(u8 *)(panel+0x27c))QueuePanelTileDraw(panel,GetCountTileDestination(position),0x1c,2,2,1,0x20);
    register u32 slot asm("r4")=0;
    asm ("" : "+l"(slot));
    while(slot<*(u32 *)(panel+0x214))
    {
        {
            register u32 packed asm("r2")=*(u32 const *)&gInventorySlotCountPositions[slot];
            asm ("" : "+l"(packed));
            register u32 x asm("r1")=packed+15;
            x=(u8)x;
            asm ("" : "+l"(x));
            packed&=0xffffff00;
            packed|=x;
            *(u32 *)&position=packed;
        }
        QueuePanelTileDraw(panel,GetCountTileDestination(position),0x1c,2,2,1,0x20);
        ++slot;
    }
    char *caption=(char *)panel+0x28c;
    u32 caption_empty=(*caption==0);
    register u32 zero asm("r9")=caption_empty;
    asm ("" : "+r"(zero));
    if(caption_empty==0)
    {
        MessageTileSize fill_size(8,2);
        register MessageTileSize fill_value asm("r0")=fill_size;
        asm ("" : "+r"(fill_value));
        u8 *pixels=panel+0x10;
        FillMessageTiles(fill_value,pixels,0);
        u32 offset=64-strlen(caption)*8;
        MessageTileSize text_size(8,2);
        DrawMessageTileStringWithColors(text_size,pixels,offset,0,caption,(
        {
            register u32 foreground_value asm("r3")=zero;
            asm ("" : "+l"(foreground_value));
            foreground_value;
        }
        ),(
        {
            register u32 background_value asm("r5")=1;
            asm ("" : "+l"(background_value));
            background_value;
        }
        ));
        register u32 transfer_bytes asm("r3")=0x200;
        asm ("" : "+l"(transfer_bytes));
        register DmaTransferQueue *queue asm("r6")=*(DmaTransferQueue **)(panel+0x278);
        asm ("" : "+l"(queue));
        register DmaCommand *command_destination asm("r0")=&command_storage;
        asm ("" : "+l"(command_destination));
        register u8 *command_source asm("r1")=pixels;
        asm ("" : "+l"(command_source));
        DmaCommand_InitializeCopy(command_destination,command_source,(void *)0x06000d00,transfer_bytes);
        DmaCommand *command=&command_storage;
        asm ("" : "+l"(command) : "l"(queue) : "r0", "r1", "r2", "r3", "r4", "r5");
        register DmaCommand const *entry asm("r10")=command;
        asm ("" : "+r"(entry));
        AppendPanelCaptionTransfer(queue,&command_storage,entry);
        QueuePanelTileDraw(panel,(void *)0x0600f02a,0x68,8,2,0,0x20);
    }
}
