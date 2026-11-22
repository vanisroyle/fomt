#include "prelude.h"
#include "g5_inventory.hh"
#include "g5_graphics.hh"
#include "dma_queue.hh"
#include <stdlib.h>
EC void *func_080D3BC0(u32);
EC u32 TileHandle_GetTileIndex(void *,u32);
EC u32 ResourceHandle_GetIndex(void *,u32);
struct ResourceSlotPositionWork
{
    u32 draw_arguments[3];
    DmaCommand command;
    u32 tile_size, tile_count, palette_size, palette_count;
    u32 position;
};
enum { ResourceSlotPositionDisplacement = offsetof(ResourceSlotPositionWork, position) };
static inline u32 const *SelectResourceSlotDmaGrowthSize(u32 size, u32 count, u32 const * size_address, u32 const * count_address)
{
    return size < count ? count_address : size_address;
}
EC SECTION(".text.g5_080CCA40") void InventoryMenu_SetResourceSlotEntry(u8 *input,u32 slot,u32 tile_data,u32 palette_data,u32 value) // 0x080CCA40
{
    DmaCommand command;
    u32 size_one,count_one,size_two,count_two,position_storage,saved_palette,palette_index;
    DrawingPosition *position_pointer;
    u32 palette_offset;
    DmaCommand *old_end,*saved_allocation,*second_old_end;
    asm ("" : "=m"(size_one),"=m"(count_one),"=m"(size_two),"=m"(count_two),"=m"(position_storage),"=m"(saved_palette),"=m"(palette_index),"=m"(position_pointer),"=m"(palette_offset));
    asm ("" : "=m"(old_end),"=m"(saved_allocation),"=m"(second_old_end));
    register u8 *panel asm("r10")=input;
    asm ("" : "+r"(panel));
    register u32 slot_index asm("r5")=slot;
    asm ("" : "+l"(slot_index));
    register u32 tiles asm("r8")=tile_data;
    asm ("" : "+r"(tiles));
    saved_palette=palette_data;
    asm ("" : "+m"(saved_palette));
    register u8 *entry asm("r0")=panel+slot_index*12;
    asm ("" : "+l"(entry));
    register u32 entries_offset asm("r1")=0x218;
    asm ("" : "+l"(entries_offset));
    entry+=entries_offset;
    asm ("" : "+l"(entry));
    SetUiEntry((UiEntry *)entry,tiles,palette_data,value);
    register u32 slot_bytes asm("r4")=slot_index*8;
    asm ("" : "+l"(slot_bytes));
    register u8 *base asm("r2")=panel;
    asm ("" : "+l"(base));
    register u8 *tile_handle asm("r0")=(u8 *)(slot_bytes+(u32)base);
    asm ("" : "+l"(tile_handle));
    register u32 tile_offset asm("r3")=0x2ac;
    asm ("" : "+l"(tile_offset));
    tile_handle+=tile_offset;
    asm ("" : "+l"(tile_handle));
    register u32 tile asm("r6")=TileHandle_GetTileIndex(tile_handle,*(u32 *)(tile_handle+4));
    asm ("" : "+l"(tile));
    register u8 *palette_handle asm("r4")=panel+slot_bytes;
    asm ("" : "+l"(palette_handle));
    register u32 palettes_offset asm("r7")=0x2ec;
    asm ("" : "+l"(palettes_offset));
    palette_handle+=palettes_offset;
    asm ("" : "+l"(palette_handle));
    palette_index=ResourceHandle_GetIndex(palette_handle,*(u32 *)(palette_handle+4));
    asm ("" : "+m"(palette_index));
    slot_index*=4;
    asm ("" : "+l"(slot_index));
    register u32 positions asm("r0")=(u32)gInventorySlotCountPositions;
    asm ("" : "+l"(positions));
    slot_index+=positions;
    asm ("" : "+l"(slot_index));
    register u32 packed asm("r1")=*(u32 const *)slot_index;
    asm ("" : "+l"(packed));
    register u32 x asm("r0")=packed+15;
    asm ("" : "+l"(x));
    x=(u8)x;
    register u32 mask asm("r2")=0xffffff00;
    asm ("" : "+l"(mask));
    packed=(packed&mask)|x;
    position_storage=packed;
    register DmaTransferQueue *first_queue=*(DmaTransferQueue **)(panel+0x278);
    asm ("" : "+l"(first_queue));
    tile=(tile<<5)+0x06010000;
    asm ("" : "+l"(tile));
    register DmaCommand *destination asm("r0")=&command;
    asm ("" : "+l"(destination));
    DmaCommand_InitializeCopy(destination,(void const *)tiles,(void *)tile,0x80);
    {
        register DmaCommand *command_pointer asm("r1")=&command;
        asm ("" : "+l"(command_pointer));
        register DmaCommand const *value asm("r9")=command_pointer;
        asm ("" : "+r"(value));
        register DmaCommand *end asm("r1")=first_queue->end;
        asm ("" : "+l"(end));
        register DmaCommand *capacity asm("r0")=first_queue->capacity_end;
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
            ++first_queue->end;
            register u32 shift asm("r5")=palette_index<<5;
            asm ("" : "+l"(shift));
            palette_offset=shift;
            asm ("" : "+m"(palette_offset));
            register u32 displaced asm("r6")=(u32)&position_storage-(ResourceSlotPositionDisplacement);
            asm ("" : "+l"(displaced));
            displaced+=ResourceSlotPositionDisplacement;
            asm ("" : "+l"(displaced));
            position_pointer=(DrawingPosition *)displaced;
            asm ("" : "+m"(position_pointer));
        }
        else
        {
            asm ("" : "=m"(size_one), "=m"(count_one));
            old_end=end;
            asm ("" : "+m"(old_end));
            count_one=1;
            register DmaCommand *begin asm("r1")=first_queue->begin;
            asm ("" : "+l"(begin));
            register DmaCommand *previous_end asm("r2")=old_end;
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
            growth_address=SelectResourceSlotDmaGrowthSize(cached_size,1,growth_address,count_address);
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
                begin=first_queue->begin;
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
            register u32 shift asm("r4")=palette_index<<5;
            asm ("" : "+l"(shift));
            palette_offset=shift;
            asm ("" : "+m"(palette_offset));
            register u32 displaced asm("r5")=(u32)&position_storage-(ResourceSlotPositionDisplacement);
            asm ("" : "+l"(displaced));
            displaced+=ResourceSlotPositionDisplacement;
            asm ("" : "+l"(displaced));
            position_pointer=(DrawingPosition *)displaced;
            asm ("" : "+m"(position_pointer));
            register DmaCommand *limit asm("r6")=old_end;
            asm ("" : "+l"(limit));
            if(input!=limit) do
            {
                if(copy_output) *copy_output=*input;
                ++input;
                ++copy_output;
            } while(input!=old_end);
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
                    register u32 temporary1 asm("r6");
                    asm ("" : "=r"(temporary1));
                    *destination=*source;
                    asm ("" : : "r"(temporary1));
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
            register DmaCommand *destroy_end asm("r2")=first_queue->end;
            asm ("" : "+l"(destroy_end));
            register DmaCommand *destroy_begin asm("r1")=first_queue->begin;
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
            asm ("" : "+l"(new_begin) : "l"(first_queue), "l"(output), "l"(new_end) : "r1", "r2", "r3", "r4");
            first_queue->begin=new_begin;
            first_queue->end=output;
            first_queue->capacity_end=(DmaCommand *)new_end;
        }
    }
    register DmaTransferQueue *queue asm("r6")=*(DmaTransferQueue **)(panel+0x278);
    asm ("" : "+l"(queue));
    register u32 offset asm("r7")=palette_offset;
    asm ("" : "+l"(offset));
    register u32 palette_base asm("r0")=0x05000200;
    asm ("" : "+l"(palette_base));
    register void *palette_destination asm("r2")=(void *)(offset+palette_base);
    asm ("" : "+l"(palette_destination));
    destination=&command;
    asm ("" : "+l"(destination));
    DmaCommand_InitializeCopy(destination,(void const *)saved_palette,palette_destination,0x20);
    {
        register DmaCommand *command_pointer asm("r1")=&command;
        asm ("" : "+l"(command_pointer));
        register DmaCommand const *value asm("r9")=command_pointer;
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
                register u32 temporary2 asm("r7");
                asm ("" : "=r"(temporary2));
                *destination=*source;
                asm ("" : : "r"(temporary2));
            }
            ++queue->end;
        }
        else
        {
            asm ("" : "=m"(size_two), "=m"(count_two));
            second_old_end=end;
            asm ("" : "+m"(second_old_end));
            count_two=1;
            register DmaCommand *begin asm("r1")=queue->begin;
            asm ("" : "+l"(begin));
            register DmaCommand *previous_end asm("r5")=second_old_end;
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
            growth_address=SelectResourceSlotDmaGrowthSize(cached_size,1,growth_address,count_address);
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
            register DmaCommand *output asm("r5")=allocation;
            asm ("" : "+l"(output));
            saved_allocation=output;
            asm ("" : "+m"(saved_allocation));
            register DmaCommand *input asm("r2")=begin;
            asm ("" : "+l"(input));
            register DmaCommand *copy_output asm("r3")=output;
            asm ("" : "+l"(copy_output));
            register DmaCommand *limit asm("r7")=second_old_end;
            asm ("" : "+l"(limit));
            if(input!=limit) do
            {
                if(copy_output) *copy_output=*input;
                ++input;
                ++copy_output;
            } while(input!=second_old_end);
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
            asm ("" : "+l"(new_begin) : "l"(queue), "l"(output), "l"(new_end) : "r1", "r2", "r3", "r4");
            queue->begin=new_begin;
            queue->end=output;
            queue->capacity_end=(DmaCommand *)new_end;
        }
    }
    register DrawingPosition *cached asm("r0")=position_pointer;
    asm ("" : "+l"(cached));
    register u32 y asm("r1")=((u8 *)cached)[1]*64;
    asm ("" : "+l"(y));
    register u8 *bytes asm("r0")=(u8 *)&position_storage;
    asm ("" : "+l"(bytes));
    x=bytes[0]*2;
    asm ("" : "+l"(x));
    register u32 map asm("r2")=0x0600f080;
    asm ("" : "+l"(map));
    u32 zero;
    register u32 stride asm("r4");
    QueuePanelTileDraw(panel,(void *)(y+(x+map)),0x54,2,1,({zero=0;zero;}),({stride=32;asm ("" : "+l"(stride));stride;}));
    register u32 quantity asm("r3")=value;
    asm ("" : "+l"(quantity));
    if(quantity>1)
    {
        register DrawingPosition *count_position asm("r6")=position_pointer;
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
EC SECTION(".text.g5_080CCE58") void InventoryMenu_SetAnimatedResourceSpecialSlotEntry(u8 *input,void const *data,u32 animation,u32 value) // 0x080CCE58
{
    DmaCommand command;
    u32 size_one,count_one,size_two,count_two,position_storage,saved_value,palette_index,palette_offset;
    DrawingPosition *position_pointer;
    DmaCommand *old_end,*saved_allocation;
    asm ("" : "=m"(size_one),"=m"(count_one),"=m"(size_two),"=m"(count_two),"=m"(position_storage),"=m"(saved_value),"=m"(palette_index),"=m"(palette_offset),"=m"(position_pointer));
    asm ("" : "=m"(old_end),"=m"(saved_allocation));
    register u8 *panel asm("r10")=input;
    asm ("" : "+r"(panel));
    saved_value=value;
    asm ("" : "+m"(saved_value));
    register u32 narrowed asm("r2")=(u16)animation;
    asm ("" : : "l"(narrowed));
    register UiEntry *entry asm("r5")=(UiEntry *)(panel+0x280);
    asm ("" : "+l"(entry));
    AssignUiEntryFromAnimation(entry,data,narrowed,value);
    void *tile_handle=panel+0x29c;
    register u32 tile asm("r4")=TileHandle_GetTileIndex(tile_handle,*(u32 *)((u8 *)tile_handle+4));
    asm ("" : "+l"(tile));
    void *palette_handle=panel+0x2a4;
    palette_index=ResourceHandle_GetIndex(palette_handle,*(u32 *)((u8 *)palette_handle+4));
    asm ("" : "+m"(palette_index));
    register void const *source asm("r1")=(void const *)entry->tile_data;
    asm ("" : "+l"(source));
    register DmaTransferQueue *queue asm("r6")=*(DmaTransferQueue **)(panel+0x278);
    asm ("" : "+l"(queue));
    tile=(tile<<5)+0x06010000;
    asm ("" : "+l"(tile));
    register DmaCommand *destination asm("r0")=&command;
    asm ("" : "+l"(destination));
    DmaCommand_InitializeCopy(destination,source,(void *)tile,0x80);
    {
        register DmaCommand *command_pointer asm("r1")=&command;
        asm ("" : "+l"(command_pointer));
        register DmaCommand const *value asm("r9")=command_pointer;
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
            register u32 shift asm("r5")=palette_index<<5;
            asm ("" : "+l"(shift));
            palette_offset=shift;
            asm ("" : "+m"(palette_offset));
            register u32 displaced asm("r7")=(u32)&position_storage-(ResourceSlotPositionDisplacement);
            asm ("" : "+l"(displaced));
            displaced+=ResourceSlotPositionDisplacement;
            asm ("" : "+l"(displaced));
            position_pointer=(DrawingPosition *)displaced;
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
            register DmaCommand *previous_end asm("r2")=old_end;
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
            growth_address=SelectResourceSlotDmaGrowthSize(cached_size,1,growth_address,count_address);
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
            register DmaCommand *output asm("r5")=allocation;
            asm ("" : "+l"(output));
            saved_allocation=output;
            asm ("" : "+m"(saved_allocation));
            register DmaCommand *input asm("r2")=begin;
            asm ("" : "+l"(input));
            register DmaCommand *copy_output asm("r3")=output;
            asm ("" : "+l"(copy_output));
            register u32 shift asm("r4")=palette_index<<5;
            asm ("" : "+l"(shift));
            palette_offset=shift;
            asm ("" : "+m"(palette_offset));
            register u32 displaced asm("r5")=(u32)&position_storage-(ResourceSlotPositionDisplacement);
            asm ("" : "+l"(displaced));
            displaced+=ResourceSlotPositionDisplacement;
            asm ("" : "+l"(displaced));
            position_pointer=(DrawingPosition *)displaced;
            asm ("" : "+m"(position_pointer));
            register DmaCommand *limit asm("r7")=old_end;
            asm ("" : "+l"(limit));
            if(input!=limit) do
            {
                if(copy_output) *copy_output=*input;
                ++input;
                ++copy_output;
            } while(input!=old_end);
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
            asm ("" : "+l"(new_begin) : "l"(queue), "l"(output), "l"(new_end) : "r1", "r2", "r3", "r4");
            queue->begin=new_begin;
            queue->end=output;
            queue->capacity_end=(DmaCommand *)new_end;
        }
    }
    register void const *palette_source asm("r1")=(void const *)((UiEntry *)(panel+0x280))->palette_data;
    asm ("" : "+l"(palette_source));
    queue=*(DmaTransferQueue **)(panel+0x278);
    asm ("" : "+l"(queue));
    register u32 offset asm("r0")=palette_offset;
    asm ("" : "+l"(offset));
    register u32 palette_base asm("r3")=0x05000200;
    asm ("" : "+l"(palette_base));
    register void *palette_destination asm("r2")=(void *)(offset+palette_base);
    asm ("" : "+l"(palette_destination));
    destination=&command;
    asm ("" : "+l"(destination));
    DmaCommand_InitializeCopy(destination,palette_source,palette_destination,0x20);
    {
        register DmaCommand *command_pointer asm("r4")=&command;
        asm ("" : "+l"(command_pointer));
        register DmaCommand const *value asm("r9")=command_pointer;
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
                register u32 reserved3 asm("r3"),reserved4 asm("r4");
                asm ("" : "=r"(reserved3),"=r"(reserved4));
                *destination=*source;
                asm ("" : : "r"(reserved3),"r"(reserved4));
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
            register DmaCommand *previous_end asm("r3")=old_end;
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
            growth_address=SelectResourceSlotDmaGrowthSize(cached_size,1,growth_address,count_address);
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
            register DmaCommand *output asm("r5")=allocation;
            asm ("" : "+l"(output));
            saved_allocation=output;
            asm ("" : "+m"(saved_allocation));
            register DmaCommand *input asm("r2")=begin;
            asm ("" : "+l"(input));
            register DmaCommand *copy_output asm("r3")=output;
            asm ("" : "+l"(copy_output));
            register DmaCommand *limit asm("r4")=old_end;
            asm ("" : "+l"(limit));
            if(input!=limit) do
            {
                if(copy_output) *copy_output=*input;
                ++input;
                ++copy_output;
                register DmaCommand *again asm("r5")=old_end;
                asm ("" : "+l"(again));
                if(input==again)break;
            } while(true);
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
                    register u32 reserve asm("r4");
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
            asm ("" : "+l"(new_begin) : "l"(queue), "l"(output), "l"(new_end) : "r2", "r3", "r4", "r7");
            queue->begin=new_begin;
            queue->end=output;
            queue->capacity_end=(DmaCommand *)new_end;
        }
    }
    register u32 packed asm("r1")=*(u32 const *)&gInventorySpecialSlotCountPosition;
    asm ("" : "+l"(packed));
    register u32 x asm("r0")=packed+15;
    asm ("" : "+l"(x));
    x=(u8)x;
    register u32 mask asm("r2")=0xffffff00;
    asm ("" : "+l"(mask));
    packed=(packed&mask)|x;
    position_storage=packed;
    register DrawingPosition *cached asm("r2")=position_pointer;
    asm ("" : "+l"(cached));
    register u32 y asm("r1")=((u8 *)cached)[1]*64;
    asm ("" : "+l"(y));
    x=((u8 *)&position_storage)[0]*2;
    asm ("" : "+l"(x));
    register u32 map asm("r3")=0x0600f080;
    asm ("" : "+l"(map));
    u32 zero;
    register u32 stride asm("r4");
    QueuePanelTileDraw(panel,(void *)(y+(x+map)),0x54,2,1,({zero=0;zero;}),({stride=32;asm ("" : "+l"(stride));stride;}));
    register u32 quantity asm("r7")=saved_value;
    asm ("" : "+l"(quantity));
    if(quantity>1)
    {
        register DrawingPosition *count_position asm("r0")=position_pointer;
        asm ("" : "+l"(count_position));
        register u32 number_y asm("r2")=((u8 *)count_position)[1]*64;
        asm ("" : "+l"(number_y));
        register u32 number_x asm("r0")=((u8 *)count_position)[0]*2;
        asm ("" : "+l"(number_x));
        register u32 number_map asm("r1")=0x0600f082;
        asm ("" : "+l"(number_map));
        QueuePanelNumberDraw(panel,quantity,(void *)(number_y+(number_x+number_map)),0x20,zero,stride);
    }
}
