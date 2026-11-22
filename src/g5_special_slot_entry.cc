#include "prelude.h"
#include "g5_inventory.hh"
#include "g5_graphics.hh"
#include "dma_queue.hh"
#include <stdlib.h>
EC void *func_080D3BC0(u32);
struct SpecialSlotPositionWork
{
    u32 draw_arguments[3];
    DmaCommand command;
    u32 tile_size, tile_count, palette_size, palette_count;
    u32 position;
};
enum { SpecialSlotPositionDisplacement = offsetof(SpecialSlotPositionWork, position) };

static inline u32 const *SelectSpecialSlotDmaGrowthSize(u32 size, u32 count, u32 const * size_address, u32 const * count_address)
{
    return size < count ? count_address : size_address;
}


EC SECTION(".text.g5_080CBAF0") void InventoryMenu_SetAnimatedSpecialSlotEntry(u8 *input,void const *data,u32 animation,u32 value) // 0x080CBAF0
{
    DmaCommand command;
    u32 size_one,count_one,size_two,count_two;
    u32 position_storage;
    DrawingPosition *position_pointer;
    DmaCommand *old_end,*saved_allocation;
    asm ("" : "=m"(size_one),"=m"(count_one),"=m"(size_two),"=m"(count_two),"=m"(position_storage),"=m"(position_pointer),"=m"(old_end),"=m"(saved_allocation));
    register u8 *panel asm("r10")=input;
    asm ("" : "+r"(panel));
    register u32 narrowed asm("r2")=(u16)animation;
    asm ("" : : "l"(narrowed));
    register UiEntry *entry asm("r4")=(UiEntry *)(panel+0x280);
    asm ("" : "+l"(entry));
    AssignUiEntryFromAnimation(entry,data,narrowed,value);
    register void const *source asm("r1")=(void const *)entry->tile_data;
    asm ("" : "+l"(source));
    register DmaTransferQueue *queue asm("r5")=*(DmaTransferQueue **)(panel+0x278);
    asm ("" : "+l"(queue));
    register DmaCommand *destination asm("r0")=&command;
    asm ("" : "+l"(destination));
    DmaCommand_InitializeCopy(destination,source,(void *)0x06000600,0x80);

    {
        register DmaCommand const *value asm("r9")=&command;
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
                register DmaCommand const *source asm("r1")=&command;
                asm ("" : "+l"(source));
                *destination=*source;
            }
            ++queue->end;
            register u32 displaced asm("r6")=(u32)&position_storage-(SpecialSlotPositionDisplacement);
            asm ("" : "+l"(displaced));
            position_pointer=(DrawingPosition *)(displaced+SpecialSlotPositionDisplacement);
            asm ("" : "+m"(position_pointer));
        }
        else
        {
            asm ("" : "=m"(size_one), "=m"(count_one));
            old_end=end;
            asm ("" : "+m"(old_end));
            count_one=1;
            register DmaCommand *begin asm("r1")=queue->begin;
            asm ("" : "+l"(begin));
            register DmaCommand *previous_end asm("r7")=old_end;
            asm ("" : "+l"(previous_end));
            register i32 difference asm("r0")=(u8 *)previous_end-(u8 *)begin;
            asm ("" : "+l"(difference));
            register u32 cached_size asm("r4")=difference>>4;
            asm ("" : "+l"(cached_size));
            size_one=cached_size;
            register u32 const *count_address asm("r2")=&count_one;
            asm ("" : "+l"(count_address));
            register u32 const *growth_address asm("r0")=&size_one;
            asm ("" : "+l"(growth_address));
            growth_address=SelectSpecialSlotDmaGrowthSize(cached_size,1,growth_address,count_address);
            register u32 growth asm("r0")=*growth_address;
            asm ("" : "+l"(growth));
            register u32 size asm("r0")=cached_size+growth;
            asm ("" : "+l"(size));
            register u32 bytes asm("r8");
            register DmaCommand *allocation asm("r0");
            if(size!=0)
            {
                register u32 allocation_bytes asm("r4")=size*sizeof(DmaCommand);
                asm ("" : "+l"(allocation_bytes));
                allocation=(DmaCommand *)malloc(allocation_bytes);
                asm ("" : "+l"(allocation));
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
            register DmaCommand *output asm("r4")=allocation;
            asm ("" : "+l"(output));
            saved_allocation=output;
            asm ("" : "+m"(saved_allocation));
            register DmaCommand *input asm("r2")=begin;
            asm ("" : "+l"(input));
            register DmaCommand *copy_output asm("r3")=output;
            asm ("" : "+l"(copy_output));
            register u32 displaced asm("r0")=(u32)&position_storage-(SpecialSlotPositionDisplacement);
            asm ("" : "+l"(displaced));
            position_pointer=(DrawingPosition *)(displaced+SpecialSlotPositionDisplacement);
            asm ("" : "+m"(position_pointer));
            register DmaCommand *limit asm("r1")=old_end;
            asm ("" : "+l"(limit));
            if(input!=limit)do
            {
                if(copy_output) *copy_output=*input;
                ++input;
                ++copy_output;
            }while(input!=old_end);
            output=copy_output;
            asm ("" : "+l"(output));
            register u32 insert_count asm("r0")=count_one;
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
            asm ("" : "+l"(new_begin) : "l"(queue), "l"(output), "l"(new_end) : "r1", "r2", "r3", "r6");
            queue->begin=new_begin;
            queue->end=output;
            queue->capacity_end=(DmaCommand *)new_end;
        }
    }
    register void const *palette_source asm("r1")=(void const *)((UiEntry *)(panel+0x280))->palette_data;
    asm ("" : "+l"(palette_source));
    queue=*(DmaTransferQueue **)(panel+0x278);
    asm ("" : "+l"(queue));
    destination=&command;
    asm ("" : "+l"(destination));
    DmaCommand_InitializeCopy(destination,palette_source,(void *)0x05000040,0x20);
    {
        register DmaCommand const *value asm("r9")=&command;
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
                register DmaCommand const *source asm("r1")=&command;
                asm ("" : "+l"(source));
                *destination=*source;
            }
            ++queue->end;

        }
        else
        {
            asm ("" : "=m"(size_two), "=m"(count_two));
            old_end=end;
            asm ("" : "+m"(old_end));
            count_two=1;
            register DmaCommand *begin asm("r1")=queue->begin;
            asm ("" : "+l"(begin));
            register DmaCommand *previous_end asm("r6")=old_end;
            asm ("" : "+l"(previous_end));
            register i32 difference asm("r0")=(u8 *)previous_end-(u8 *)begin;
            asm ("" : "+l"(difference));
            register u32 cached_size asm("r4")=difference>>4;
            asm ("" : "+l"(cached_size));
            size_two=cached_size;
            register u32 const *count_address asm("r2")=&count_two;
            asm ("" : "+l"(count_address));
            register u32 const *growth_address asm("r0")=&size_two;
            asm ("" : "+l"(growth_address));
            growth_address=SelectSpecialSlotDmaGrowthSize(cached_size,1,growth_address,count_address);
            register u32 growth asm("r0")=*growth_address;
            asm ("" : "+l"(growth));
            register u32 size asm("r0")=cached_size+growth;
            asm ("" : "+l"(size));
            register u32 bytes asm("r8");
            register DmaCommand *allocation asm("r0");
            if(size!=0)
            {
                register u32 allocation_bytes asm("r4")=size*sizeof(DmaCommand);
                asm ("" : "+l"(allocation_bytes));
                allocation=(DmaCommand *)malloc(allocation_bytes);
                asm ("" : "+l"(allocation));
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
            register DmaCommand *output asm("r4")=allocation;
            asm ("" : "+l"(output));
            saved_allocation=output;
            asm ("" : "+m"(saved_allocation));
            register DmaCommand *input asm("r2")=begin;
            asm ("" : "+l"(input));
            register DmaCommand *copy_output asm("r3")=output;
            asm ("" : "+l"(copy_output));

            register DmaCommand *limit asm("r7")=old_end;
            asm ("" : "+l"(limit));
            if(input!=limit)do
            {
                if(copy_output) *copy_output=*input;
                ++input;
                ++copy_output;
            }while(input!=old_end);
            output=copy_output;
            asm ("" : "+l"(output));
            register u32 insert_count asm("r0")=count_two;
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
            asm ("" : "+l"(new_begin) : "l"(queue), "l"(output), "l"(new_end) : "r1", "r2", "r3", "r6");
            queue->begin=new_begin;
            queue->end=output;
            queue->capacity_end=(DmaCommand *)new_end;
        }
    }
    position_storage=*(u32 const *)&gInventorySpecialSlotCountPosition;
    register DrawingPosition *cached asm("r0")=position_pointer;
    asm ("" : "+l"(cached));
    register u32 y asm("r1")=((u8 *)cached)[1]*64;
    asm ("" : "+l"(y));
    register DrawingPosition *position asm("r4")=(DrawingPosition *)&position_storage;
    asm ("" : "+l"(position));
    register u32 x asm("r0")=((u8 *)position)[0]*2;
    asm ("" : "+l"(x));
    register u32 map asm("r2")=0x0600f000;
    asm ("" : "+l"(map));
    register u32 stride asm("r5");
    QueuePanelTileDraw(panel,(void *)(y+(x+map)),0x30,2,2,2,({stride=32;asm ("" : "+l"(stride));stride;}));
    register DrawingPosition *cached_second asm("r2")=position_pointer;
    asm ("" : "+l"(cached_second));
    y=((u8 *)cached_second)[1]*64;
    asm ("" : "+l"(y));
    x=((u8 *)position)[0]*2;
    asm ("" : "+l"(x));
    register u32 bottom_map asm("r3")=0x0600f080;
    asm ("" : "+l"(bottom_map));
    u32 zero;
    QueuePanelTileDraw(panel,(void *)(y+(x+bottom_map)),0x54,2,1,({zero=0;zero;}),stride);
    entry=(UiEntry *)(panel+0x280);
    asm ("" : "+l"(entry));
    if(GetUiEntryValue(entry)>1)
    {
        register u32 quantity asm("r1")=GetUiEntryValue(entry);
        asm ("" : "+l"(quantity));
        register DrawingPosition *count_position asm("r4")=position_pointer;
        asm ("" : "+l"(count_position));
        register u32 number_y asm("r2")=((u8 *)count_position)[1]*64;
        asm ("" : "+l"(number_y));
        register u32 number_x asm("r0")=((u8 *)count_position)[0]*2;
        asm ("" : "+l"(number_x));
        register u32 number_map asm("r7")=0x0600f082;
        asm ("" : "+l"(number_map));
        QueuePanelNumberDraw(panel,quantity,(void *)(number_y+(number_x+number_map)),0x20,zero,stride);
    }
}

EC SECTION(".text.g5_080CBDD4") void InventoryMenu_SetSpecialSlotEntry(u8 *input,u32 tile_data,u32 palette_data,u32 value) // 0x080CBDD4
{
    DmaCommand command;
    u32 size_one,count_one,size_two,count_two;
    u32 position_storage;
    DrawingPosition *position_pointer;
    DmaCommand *old_end,*saved_allocation;
    asm ("" : "=m"(size_one),"=m"(count_one),"=m"(size_two),"=m"(count_two),"=m"(position_storage),"=m"(position_pointer),"=m"(old_end),"=m"(saved_allocation));
    register u8 *panel asm("r10")=input;
    asm ("" : "+r"(panel));
    register UiEntry *entry asm("r4")=(UiEntry *)(panel+0x280);
    asm ("" : "+l"(entry));
    SetUiEntry(entry,tile_data,palette_data,value);
    register void const *source asm("r1")=(void const *)entry->tile_data;
    asm ("" : "+l"(source));
    register DmaTransferQueue *queue asm("r5")=*(DmaTransferQueue **)(panel+0x278);
    asm ("" : "+l"(queue));
    register DmaCommand *destination asm("r0")=&command;
    asm ("" : "+l"(destination));
    DmaCommand_InitializeCopy(destination,source,(void *)0x06000600,0x80);

    {
        register DmaCommand const *value asm("r9")=&command;
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
                register DmaCommand const *source asm("r1")=&command;
                asm ("" : "+l"(source));
                *destination=*source;
            }
            ++queue->end;
            register u32 displaced asm("r6")=(u32)&position_storage-(SpecialSlotPositionDisplacement);
            asm ("" : "+l"(displaced));
            position_pointer=(DrawingPosition *)(displaced+SpecialSlotPositionDisplacement);
            asm ("" : "+m"(position_pointer));
        }
        else
        {
            asm ("" : "=m"(size_one), "=m"(count_one));
            old_end=end;
            asm ("" : "+m"(old_end));
            count_one=1;
            register DmaCommand *begin asm("r1")=queue->begin;
            asm ("" : "+l"(begin));
            register DmaCommand *previous_end asm("r7")=old_end;
            asm ("" : "+l"(previous_end));
            register i32 difference asm("r0")=(u8 *)previous_end-(u8 *)begin;
            asm ("" : "+l"(difference));
            register u32 cached_size asm("r4")=difference>>4;
            asm ("" : "+l"(cached_size));
            size_one=cached_size;
            register u32 const *count_address asm("r2")=&count_one;
            asm ("" : "+l"(count_address));
            register u32 const *growth_address asm("r0")=&size_one;
            asm ("" : "+l"(growth_address));
            growth_address=SelectSpecialSlotDmaGrowthSize(cached_size,1,growth_address,count_address);
            register u32 growth asm("r0")=*growth_address;
            asm ("" : "+l"(growth));
            register u32 size asm("r0")=cached_size+growth;
            asm ("" : "+l"(size));
            register u32 bytes asm("r8");
            register DmaCommand *allocation asm("r0");
            if(size!=0)
            {
                register u32 allocation_bytes asm("r4")=size*sizeof(DmaCommand);
                asm ("" : "+l"(allocation_bytes));
                allocation=(DmaCommand *)malloc(allocation_bytes);
                asm ("" : "+l"(allocation));
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
            register DmaCommand *output asm("r4")=allocation;
            asm ("" : "+l"(output));
            saved_allocation=output;
            asm ("" : "+m"(saved_allocation));
            register DmaCommand *input asm("r2")=begin;
            asm ("" : "+l"(input));
            register DmaCommand *copy_output asm("r3")=output;
            asm ("" : "+l"(copy_output));
            register u32 displaced asm("r0")=(u32)&position_storage-(SpecialSlotPositionDisplacement);
            asm ("" : "+l"(displaced));
            position_pointer=(DrawingPosition *)(displaced+SpecialSlotPositionDisplacement);
            asm ("" : "+m"(position_pointer));
            register DmaCommand *limit asm("r1")=old_end;
            asm ("" : "+l"(limit));
            if(input!=limit)do
            {
                if(copy_output) *copy_output=*input;
                ++input;
                ++copy_output;
            }while(input!=old_end);
            output=copy_output;
            asm ("" : "+l"(output));
            register u32 insert_count asm("r0")=count_one;
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
            asm ("" : "+l"(new_begin) : "l"(queue), "l"(output), "l"(new_end) : "r1", "r2", "r3", "r6");
            queue->begin=new_begin;
            queue->end=output;
            queue->capacity_end=(DmaCommand *)new_end;
        }
    }
    register void const *palette_source asm("r1")=(void const *)((UiEntry *)(panel+0x280))->palette_data;
    asm ("" : "+l"(palette_source));
    queue=*(DmaTransferQueue **)(panel+0x278);
    asm ("" : "+l"(queue));
    destination=&command;
    asm ("" : "+l"(destination));
    DmaCommand_InitializeCopy(destination,palette_source,(void *)0x05000040,0x20);
    {
        register DmaCommand const *value asm("r9")=&command;
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
                register DmaCommand const *source asm("r1")=&command;
                asm ("" : "+l"(source));
                *destination=*source;
            }
            ++queue->end;

        }
        else
        {
            asm ("" : "=m"(size_two), "=m"(count_two));
            old_end=end;
            asm ("" : "+m"(old_end));
            count_two=1;
            register DmaCommand *begin asm("r1")=queue->begin;
            asm ("" : "+l"(begin));
            register DmaCommand *previous_end asm("r6")=old_end;
            asm ("" : "+l"(previous_end));
            register i32 difference asm("r0")=(u8 *)previous_end-(u8 *)begin;
            asm ("" : "+l"(difference));
            register u32 cached_size asm("r4")=difference>>4;
            asm ("" : "+l"(cached_size));
            size_two=cached_size;
            register u32 const *count_address asm("r2")=&count_two;
            asm ("" : "+l"(count_address));
            register u32 const *growth_address asm("r0")=&size_two;
            asm ("" : "+l"(growth_address));
            growth_address=SelectSpecialSlotDmaGrowthSize(cached_size,1,growth_address,count_address);
            register u32 growth asm("r0")=*growth_address;
            asm ("" : "+l"(growth));
            register u32 size asm("r0")=cached_size+growth;
            asm ("" : "+l"(size));
            register u32 bytes asm("r8");
            register DmaCommand *allocation asm("r0");
            if(size!=0)
            {
                register u32 allocation_bytes asm("r4")=size*sizeof(DmaCommand);
                asm ("" : "+l"(allocation_bytes));
                allocation=(DmaCommand *)malloc(allocation_bytes);
                asm ("" : "+l"(allocation));
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
            register DmaCommand *output asm("r4")=allocation;
            asm ("" : "+l"(output));
            saved_allocation=output;
            asm ("" : "+m"(saved_allocation));
            register DmaCommand *input asm("r2")=begin;
            asm ("" : "+l"(input));
            register DmaCommand *copy_output asm("r3")=output;
            asm ("" : "+l"(copy_output));

            register DmaCommand *limit asm("r7")=old_end;
            asm ("" : "+l"(limit));
            if(input!=limit)do
            {
                if(copy_output) *copy_output=*input;
                ++input;
                ++copy_output;
            }while(input!=old_end);
            output=copy_output;
            asm ("" : "+l"(output));
            register u32 insert_count asm("r0")=count_two;
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
            asm ("" : "+l"(new_begin) : "l"(queue), "l"(output), "l"(new_end) : "r1", "r2", "r3", "r6");
            queue->begin=new_begin;
            queue->end=output;
            queue->capacity_end=(DmaCommand *)new_end;
        }
    }
    position_storage=*(u32 const *)&gInventorySpecialSlotCountPosition;
    register DrawingPosition *cached asm("r0")=position_pointer;
    asm ("" : "+l"(cached));
    register u32 y asm("r1")=((u8 *)cached)[1]*64;
    asm ("" : "+l"(y));
    register DrawingPosition *position asm("r4")=(DrawingPosition *)&position_storage;
    asm ("" : "+l"(position));
    register u32 x asm("r0")=((u8 *)position)[0]*2;
    asm ("" : "+l"(x));
    register u32 map asm("r2")=0x0600f000;
    asm ("" : "+l"(map));
    register u32 stride asm("r5");
    QueuePanelTileDraw(panel,(void *)(y+(x+map)),0x30,2,2,2,({stride=32;asm ("" : "+l"(stride));stride;}));
    register DrawingPosition *cached_second asm("r2")=position_pointer;
    asm ("" : "+l"(cached_second));
    y=((u8 *)cached_second)[1]*64;
    asm ("" : "+l"(y));
    x=((u8 *)position)[0]*2;
    asm ("" : "+l"(x));
    register u32 bottom_map asm("r3")=0x0600f080;
    asm ("" : "+l"(bottom_map));
    u32 zero;
    QueuePanelTileDraw(panel,(void *)(y+(x+bottom_map)),0x54,2,1,({zero=0;zero;}),stride);
    entry=(UiEntry *)(panel+0x280);
    asm ("" : "+l"(entry));
    if(GetUiEntryValue(entry)>1)
    {
        register u32 quantity asm("r1")=GetUiEntryValue(entry);
        asm ("" : "+l"(quantity));
        register DrawingPosition *count_position asm("r4")=position_pointer;
        asm ("" : "+l"(count_position));
        register u32 number_y asm("r2")=((u8 *)count_position)[1]*64;
        asm ("" : "+l"(number_y));
        register u32 number_x asm("r0")=((u8 *)count_position)[0]*2;
        asm ("" : "+l"(number_x));
        register u32 number_map asm("r7")=0x0600f082;
        asm ("" : "+l"(number_map));
        QueuePanelNumberDraw(panel,quantity,(void *)(number_y+(number_x+number_map)),0x20,zero,stride);
    }
}
