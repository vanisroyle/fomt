#include "prelude.h"
#include "g5_inventory.hh"
#include "g5_graphics.hh"
#include "dma_queue.hh"
#include <stdlib.h>

EC void *func_080D3BC0(u32);

struct SlotEntryPositionWork
{
    u32 draw_arguments[3];
    DmaCommand command;
    u32 tile_size, tile_count, palette_size, palette_count;
    u32 position;
};
enum { SlotEntryPositionDisplacement = offsetof(SlotEntryPositionWork, position) };

static inline u32 const *SelectSlotEntryDmaGrowthSize(u32 size, u32 count, u32 const * size_address, u32 const * count_address)
{
    return size < count ? count_address : size_address;
}

EC SECTION(".text.g5_080CB304") void InventoryMenu_SetAnimatedSlotEntry(u8 *input,u32 slot,void const *data,u32 animation,u32 value) // 0x080CB304
{
    DmaCommand command;
    u32 size_one,count_one,size_two,count_two,position_storage;
    u8 *saved_panel;
    u32 palette_slot,double_slot,position_offset;
    DrawingPosition *position_pointer;
    u32 tile_offset;
    DmaCommand *old_end;
    asm ("" : "=m"(size_one),"=m"(count_one),"=m"(size_two),"=m"(count_two),"=m"(position_storage));

    saved_panel=input;
    u32 saved_slot=slot;
    register void const *animation_data asm("r1")=data;
    asm ("" : "+l"(animation_data));
    register u32 narrowed asm("r2")=animation;
    asm ("" : "+l"(narrowed));
    narrowed=(u16)narrowed;
    asm ("" : : "l"(narrowed));
    register u32 initial_slot asm("r0")=saved_slot;
    asm ("" : : "l"(initial_slot));
    register u32 doubled asm("r7")=initial_slot<<1;
    asm ("" : : "l"(doubled));
    register u32 entry_offset asm("r4")=doubled+initial_slot;
    asm ("" : "+l"(entry_offset));
    entry_offset*=4;
    asm ("" : "+l"(entry_offset));
    register u8 *entry_base asm("r3")=saved_panel;
    asm ("" : "+l"(entry_base));
    entry_offset+=(u32)entry_base;
    asm ("" : "+l"(entry_offset));
    register u32 entries asm("r6")=0x218;
    asm ("" : "+l"(entries));
    entry_offset+=entries;
    asm ("" : "+l"(entry_offset));
    AssignUiEntryFromAnimation((UiEntry *)entry_offset,animation_data,narrowed,value);
    register void const *tile_source asm("r1")=(void const *)((UiEntry *)entry_offset)->tile_data;
    asm ("" : "+l"(tile_source));
    register u8 *panel_base asm("r2")=saved_panel;
    asm ("" : "+l"(panel_base));
    register u32 queue_offset asm("r3")=0x278;
    asm ("" : "+l"(queue_offset));
    register u32 queue_address asm("r0")=(u32)panel_base+queue_offset;
    asm ("" : "+l"(queue_address));
    register DmaTransferQueue *first_queue asm("r5")=*(DmaTransferQueue **)queue_address;
    asm ("" : "+l"(first_queue));
    register u32 slot_index asm("r4")=saved_slot;
    asm ("" : "+l"(slot_index));
    register u32 tile_destination asm("r2")=slot_index<<7;
    asm ("" : "+l"(tile_destination));
    register u32 tile_base asm("r6")=0x06000680;
    asm ("" : "+l"(tile_base));
    tile_destination+=tile_base;
    asm ("" : "+l"(tile_destination));
    register DmaCommand *destination asm("r0")=&command;
    asm ("" : "+l"(destination));
    DmaCommand_InitializeCopy(destination,tile_source,(void *)tile_destination,0x80);
    register DmaCommand *command_pointer asm("r0")=&command;
    asm ("" : "+l"(command_pointer));
    {
        register DmaCommand const *value asm("r10")=command_pointer;
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
            double_slot=doubled;
            register u32 palette_input asm("r6")=saved_slot;
            asm ("" : "+l"(palette_input));
            register u32 palette_bytes asm("r6")=palette_input<<5;
            asm ("" : "+l"(palette_bytes));
            tile_offset=palette_bytes;
            register u32 position_input asm("r7")=saved_slot;
            asm ("" : "+l"(position_input));
            register u32 position_bytes asm("r7")=position_input<<2;
            asm ("" : "+l"(position_bytes));
            position_offset=position_bytes;
            register u32 displaced asm("r0")=(u32)&position_storage-SlotEntryPositionDisplacement;
            asm ("" : "+l"(displaced));
            displaced+=SlotEntryPositionDisplacement;
            asm ("" : "+l"(displaced));
            position_pointer=(DrawingPosition *)displaced;
            register u32 palette_input_index asm("r1")=saved_slot;
            asm ("" : "+l"(palette_input_index));
            palette_input_index+=3;
            asm ("" : "+l"(palette_input_index));
            palette_slot=palette_input_index;

        }
        else
        {
            asm ("" : "=m"(size_one), "=m"(count_one));
            old_end=end;

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
            growth_address=SelectSlotEntryDmaGrowthSize(cached_size,1,growth_address,count_address);
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
            register DmaCommand *output asm("r4")=allocation;
            asm ("" : "+l"(output));
            register DmaCommand *first_allocation asm("r8")=output;
            asm ("" : "+r"(first_allocation));
            register DmaCommand *input asm("r2")=begin;
            asm ("" : "+l"(input));
            register DmaCommand *copy_output asm("r3")=first_allocation;
            asm ("" : "+l"(copy_output));

            register u32 double_input asm("r4")=saved_slot;
            asm ("" : "+l"(double_input));
            double_input<<=1;
            asm ("" : "+l"(double_input));
            double_slot=double_input;
            register u32 palette_input asm("r6")=saved_slot;
            asm ("" : "+l"(palette_input));
            register u32 palette_bytes asm("r6")=palette_input<<5;
            asm ("" : "+l"(palette_bytes));
            tile_offset=palette_bytes;
            register u32 position_input asm("r7")=saved_slot;
            asm ("" : "+l"(position_input));
            register u32 position_bytes asm("r7")=position_input<<2;
            asm ("" : "+l"(position_bytes));
            position_offset=position_bytes;
            register u32 displaced asm("r0")=(u32)&position_storage-SlotEntryPositionDisplacement;
            asm ("" : "+l"(displaced));
            displaced+=SlotEntryPositionDisplacement;
            asm ("" : "+l"(displaced));
            position_pointer=(DrawingPosition *)displaced;
            register u32 palette_input_index asm("r1")=saved_slot;
            asm ("" : "+l"(palette_input_index));
            palette_input_index+=3;
            asm ("" : "+l"(palette_input_index));
            palette_slot=palette_input_index;
            register DmaCommand *limit asm("r4")=old_end;
            asm ("" : "+l"(limit));
            if(input!=limit) do
            {
                if(copy_output) *copy_output=*input;
                ++input;
                ++copy_output;
            } while(input!=({register DmaCommand *loop_end asm("r6")=old_end;asm ("" : "+l"(loop_end));loop_end;}));
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
                    register u32 reserve asm("r6");
                    asm ("" : "=r"(reserve));
                    *destination=*source;
                    asm ("" : : "r"(reserve));
                }
                ++output;
            }
            else
            {
                register u32 remaining asm("r12")=insert_count;
                asm ("" : : "r"(remaining));
                register DmaCommand *tail asm("r2")=output;
                asm ("" : "+l"(tail));
                if(insert_count!=0)do
                {
                    asm ("" : "+r"(value));
                    if(tail)*tail=*value;
                    register i32 minus_one asm("r0")=-1;
                    asm ("" : : "l"(minus_one));
                    remaining+=minus_one;
                    ++tail;
                } while(({register u32 left asm("r1")=remaining;asm ("" : "+l"(left));left;})!=0);
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
            register u32 new_end asm("r0")=bytes;
            asm ("" : "+l"(new_end));
            new_end+=(u32)first_allocation;
            asm ("" : "+l"(new_end));
            register DmaCommand *new_begin asm("r2")=first_allocation;
            asm ("" : "+l"(new_begin) : "l"(first_queue),"l"(output),"l"(new_end) : "r1","r3","r6","r7");
            first_queue->begin=new_begin;
            first_queue->end=output;
            first_queue->capacity_end=(DmaCommand *)new_end;
        }
    }
    register u32 entry_double asm("r3")=double_slot;
    asm ("" : "+l"(entry_double));
    register u32 entry_slot asm("r4")=saved_slot;
    asm ("" : "+l"(entry_slot));
    register u32 palette_entry asm("r0")=(entry_double+entry_slot)*4;
    asm ("" : "+l"(palette_entry));
    register u8 *second_base asm("r6")=saved_panel;
    asm ("" : "+l"(second_base));
    palette_entry+=(u32)second_base;
    asm ("" : "+l"(palette_entry));
    register u32 entry_base_offset asm("r7")=0x218;
    asm ("" : "+l"(entry_base_offset));
    palette_entry+=entry_base_offset;
    asm ("" : "+l"(palette_entry));
    register void const *palette_source asm("r1")=(void const *)((UiEntry *)palette_entry)->palette_data;
    asm ("" : "+l"(palette_source));
    register u32 second_offset asm("r2")=0x278;
    asm ("" : "+l"(second_offset));
    queue_address=(u32)second_base+second_offset;
    asm ("" : "+l"(queue_address));
    register DmaTransferQueue *queue asm("r5")=*(DmaTransferQueue **)queue_address;
    asm ("" : "+l"(queue));
    register u32 palette_bytes asm("r3")=tile_offset;
    asm ("" : "+l"(palette_bytes));
    register u32 palette_base asm("r4")=0x05000060;
    asm ("" : "+l"(palette_base));
    register void *palette_destination asm("r2")=(void *)(palette_bytes+palette_base);
    asm ("" : "+l"(palette_destination));
    destination=&command;
    asm ("" : "+l"(destination));
    DmaCommand_InitializeCopy(destination,palette_source,palette_destination,0x20);
    {
        register DmaCommand *command_pointer asm("r6")=&command;
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
                register u32 block4 asm("r4"),block6 asm("r6");
                asm ("" : "=r"(block4),"=r"(block6));
                *destination=*source;
                asm ("" : : "r"(block4),"r"(block6));
            }
            ++queue->end;

        }
        else
        {
            asm ("" : "=m"(size_two), "=m"(count_two));
            register DmaCommand *second_end asm("r10")=end;
            asm ("" : "+r"(second_end));
            count_two=1;
            register DmaCommand *begin asm("r1")=queue->begin;
            asm ("" : "+l"(begin));
            register DmaCommand *previous_end asm("r4")=second_end;
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
            growth_address=SelectSlotEntryDmaGrowthSize(cached_size,1,growth_address,count_address);
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
            old_end=output;

            register DmaCommand *input asm("r2")=begin;
            asm ("" : "+l"(input));
            register DmaCommand *copy_output asm("r3")=output;
            asm ("" : "+l"(copy_output));

            if(input!=second_end)do
            {
                if(copy_output) *copy_output=*input;
                ++input;
                ++copy_output;
            } while(input!=second_end);
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
                    register u32 reserve asm("r3");
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
            register u8 *new_end asm("r0")=(u8 *)old_end;
            asm ("" : "+l"(new_end));
            new_end+=bytes;
            asm ("" : "+l"(new_end));
            register DmaCommand *new_begin asm("r1")=old_end;
            asm ("" : "+l"(new_begin) : "l"(queue), "l"(output), "l"(new_end) : "r2", "r3", "r6", "r7");
            queue->begin=new_begin;
            queue->end=output;
            queue->capacity_end=(DmaCommand *)new_end;
        }
    }
    register u32 positions asm("r0")=(u32)gInventorySlotCountPositions;
    asm ("" : "+l"(positions));
    register u32 offset asm("r2")=position_offset;
    asm ("" : "+l"(offset));
    positions=offset+positions;
    asm ("" : "+l"(positions));
    position_storage=*(u32 *)positions;
    asm ("" : "+m"(position_storage));
    register DrawingPosition *cached asm("r3")=position_pointer;
    asm ("" : "+l"(cached));
    register u32 y asm("r1")=((u8 *)cached)[1]*64;
    asm ("" : "+l"(y));
    register DrawingPosition *position asm("r4")=(DrawingPosition *)&position_storage;
    asm ("" : "+l"(position));
    register u32 x asm("r0")=((u8 *)position)[0]*2;
    asm ("" : "+l"(x));
    register u32 map asm("r2")=0x0600f000;
    asm ("" : "+l"(map));
    register u32 draw_destination asm("r1")=y+(x+map);
    asm ("" : "+l"(draw_destination));
    register u32 tile asm("r2")=position_offset;
    asm ("" : "+l"(tile));
    tile+=0x34;
    tile=(u16)tile;
    asm ("" : : "l"(tile));
    register u32 stride asm("r5");
    QueuePanelTileDraw(saved_panel,(void *)draw_destination,tile,2,2,({register u32 pal asm("r6")=palette_slot;asm ("" : "+l"(pal));register u32 narrow asm("r0")=(u16)pal;asm ("" : : "l"(narrow));narrow;}),({stride=32;asm ("" : "+l"(stride));stride;}));
    register DrawingPosition *second_position=position_pointer;
    asm ("" : "+l"(second_position) : : "r6");
    y=((u8 *)second_position)[1]*64;
    asm ("" : "+l"(y));
    x=((u8 *)position)[0]*2;
    asm ("" : "+l"(x));
    register u32 bottom_map asm("r2")=0x0600f080;
    asm ("" : "+l"(bottom_map));
    u32 zero;
    QueuePanelTileDraw(saved_panel,(void *)(y+(x+bottom_map)),0x54,2,1,({zero=0;zero;}),stride);
    register u32 quantity asm("r3")=value;
    asm ("" : "+l"(quantity));
    if(quantity>1)
    {
        register u32 number_y asm("r2")=((u8 *)second_position)[1]*64;
        asm ("" : "+l"(number_y));
        register u32 number_x asm("r0")=((u8 *)second_position)[0]*2;
        asm ("" : "+l"(number_x));
        register u32 number_map asm("r6")=0x0600f082;
        asm ("" : "+l"(number_map));
        QueuePanelNumberDraw(saved_panel,quantity,(void *)(number_y+(number_x+number_map)),0x20,zero,stride);
    }
}

EC SECTION(".text.g5_080CB63C") void InventoryMenu_SetSlotEntry(u8 *input,u32 slot,u32 tiles,u32 palettes,u32 value) // 0x080CB63C
{
    DmaCommand command;
    u32 size_one,count_one,size_two,count_two,position_storage;
    u8 *saved_panel;
    u32 saved_palette,palette_slot,position_offset;
    DrawingPosition *position_pointer;
    u32 tile_offset;
    DmaCommand *old_end;
    asm ("" : "=m"(size_one),"=m"(count_one),"=m"(size_two),"=m"(count_two),"=m"(position_storage));

    saved_panel=input;
    register u32 slot_index asm("r5")=slot;
    asm ("" : "+l"(slot_index));
    register u32 tile_data asm("r4")=tiles;
    asm ("" : "+l"(tile_data));
    saved_palette=palettes;
    register u32 entry_offset asm("r0")=slot_index*12;
    asm ("" : "+l"(entry_offset));
    register u8 *entry_base asm("r1")=saved_panel;
    asm ("" : "+l"(entry_base));
    entry_offset+=(u32)entry_base;
    asm ("" : "+l"(entry_offset));
    register u32 entries asm("r2")=0x218;
    asm ("" : "+l"(entries));
    entry_offset+=entries;
    asm ("" : "+l"(entry_offset));
    SetUiEntry((UiEntry *)entry_offset,tile_data,palettes,value);
    register u8 *panel_base asm("r3")=saved_panel;
    asm ("" : "+l"(panel_base));
    register u32 queue_offset asm("r6")=0x278;
    asm ("" : "+l"(queue_offset));
    register u32 queue_address asm("r0")=(u32)panel_base+queue_offset;
    asm ("" : "+l"(queue_address));
    register DmaTransferQueue *first_queue asm("r6")=*(DmaTransferQueue **)queue_address;
    asm ("" : "+l"(first_queue));
    register u32 tile_destination asm("r2")=slot_index<<7;
    asm ("" : "+l"(tile_destination));
    register u32 tile_base asm("r7")=0x06000680;
    asm ("" : "+l"(tile_base));
    tile_destination+=tile_base;
    asm ("" : "+l"(tile_destination));
    register DmaCommand *destination asm("r0")=&command;
    asm ("" : "+l"(destination));
    DmaCommand_InitializeCopy(destination,(void const *)tile_data,(void *)tile_destination,0x80);
    register DmaCommand *command_pointer asm("r0")=&command;
    asm ("" : "+l"(command_pointer));
    {
        register DmaCommand const *value asm("r10")=command_pointer;
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
            register u32 palette_bytes asm("r6")=slot_index<<5;
            asm ("" : "+l"(palette_bytes));
            tile_offset=palette_bytes;
            register u32 position_bytes asm("r7")=slot_index<<2;
            asm ("" : "+l"(position_bytes));
            position_offset=position_bytes;
            register u32 displaced asm("r0")=(u32)&position_storage-SlotEntryPositionDisplacement;
            asm ("" : "+l"(displaced));
            displaced+=SlotEntryPositionDisplacement;
            asm ("" : "+l"(displaced));
            position_pointer=(DrawingPosition *)displaced;
            slot_index+=3;
            asm ("" : "+l"(slot_index));
            palette_slot=slot_index;

        }
        else
        {
            asm ("" : "=m"(size_one), "=m"(count_one));
            old_end=end;

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
            growth_address=SelectSlotEntryDmaGrowthSize(cached_size,1,growth_address,count_address);
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
            register DmaCommand *output asm("r4")=allocation;
            asm ("" : "+l"(output));
            register DmaCommand *first_allocation asm("r8")=output;
            asm ("" : "+r"(first_allocation));
            register DmaCommand *input asm("r2")=begin;
            asm ("" : "+l"(input));
            register DmaCommand *copy_output asm("r3")=first_allocation;
            asm ("" : "+l"(copy_output));

            register u32 palette_bytes asm("r4")=slot_index<<5;
            asm ("" : "+l"(palette_bytes));
            tile_offset=palette_bytes;
            register u32 position_bytes asm("r7")=slot_index<<2;
            asm ("" : "+l"(position_bytes));
            position_offset=position_bytes;
            register u32 displaced asm("r0")=(u32)&position_storage-SlotEntryPositionDisplacement;
            asm ("" : "+l"(displaced));
            displaced+=SlotEntryPositionDisplacement;
            asm ("" : "+l"(displaced));
            position_pointer=(DrawingPosition *)displaced;
            slot_index+=3;
            asm ("" : "+l"(slot_index));
            palette_slot=slot_index;
            register DmaCommand *limit asm("r1")=old_end;
            asm ("" : "+l"(limit));
            if(input!=limit) do
            {
                if(copy_output) *copy_output=*input;
                ++input;
                ++copy_output;
            } while(input!=({register DmaCommand *loop_end asm("r0")=old_end;asm ("" : "+l"(loop_end));loop_end;}));
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
            register u32 new_end asm("r0")=bytes;
            asm ("" : "+l"(new_end));
            new_end+=(u32)first_allocation;
            asm ("" : "+l"(new_end));
            register DmaCommand *new_begin asm("r7")=first_allocation;
            asm ("" : "+l"(new_begin) : "l"(first_queue),"l"(output),"l"(new_end) : "r1","r2","r3");
            first_queue->begin=new_begin;
            first_queue->end=output;
            first_queue->capacity_end=(DmaCommand *)new_end;
        }
    }
    register u8 *second_base asm("r1")=saved_panel;
    asm ("" : "+l"(second_base));
    register u32 second_offset asm("r2")=0x278;
    asm ("" : "+l"(second_offset));
    queue_address=(u32)second_base+second_offset;
    asm ("" : "+l"(queue_address));
    register DmaTransferQueue *queue asm("r5")=*(DmaTransferQueue **)queue_address;
    asm ("" : "+l"(queue));
    register u32 palette_bytes asm("r3")=tile_offset;
    asm ("" : "+l"(palette_bytes));
    register u32 palette_base asm("r4")=0x05000060;
    asm ("" : "+l"(palette_base));
    register void *palette_destination asm("r2")=(void *)(palette_bytes+palette_base);
    asm ("" : "+l"(palette_destination));
    destination=&command;
    asm ("" : "+l"(destination));
    DmaCommand_InitializeCopy(destination,(void const *)saved_palette,palette_destination,0x20);
    {
        register DmaCommand *command_pointer asm("r6")=&command;
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
                register u32 block4 asm("r4"),block6 asm("r6");
                asm ("" : "=r"(block4),"=r"(block6));
                *destination=*source;
                asm ("" : : "r"(block4),"r"(block6));
            }
            ++queue->end;

        }
        else
        {
            asm ("" : "=m"(size_two), "=m"(count_two));
            register DmaCommand *second_end asm("r10")=end;
            asm ("" : "+r"(second_end));
            count_two=1;
            register DmaCommand *begin asm("r1")=queue->begin;
            asm ("" : "+l"(begin));
            register DmaCommand *previous_end asm("r4")=second_end;
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
            growth_address=SelectSlotEntryDmaGrowthSize(cached_size,1,growth_address,count_address);
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
            old_end=output;

            register DmaCommand *input asm("r2")=begin;
            asm ("" : "+l"(input));
            register DmaCommand *copy_output asm("r3")=output;
            asm ("" : "+l"(copy_output));

            if(input!=second_end)do
            {
                if(copy_output) *copy_output=*input;
                ++input;
                ++copy_output;
            } while(input!=second_end);
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
                    register u32 reserve asm("r3");
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
            register u8 *new_end asm("r0")=(u8 *)old_end;
            asm ("" : "+l"(new_end));
            new_end+=bytes;
            asm ("" : "+l"(new_end));
            register DmaCommand *new_begin asm("r1")=old_end;
            asm ("" : "+l"(new_begin) : "l"(queue), "l"(output), "l"(new_end) : "r2", "r3", "r6", "r7");
            queue->begin=new_begin;
            queue->end=output;
            queue->capacity_end=(DmaCommand *)new_end;
        }
    }
    register u32 positions asm("r0")=(u32)gInventorySlotCountPositions;
    asm ("" : "+l"(positions));
    register u32 offset asm("r2")=position_offset;
    asm ("" : "+l"(offset));
    positions=offset+positions;
    asm ("" : "+l"(positions));
    position_storage=*(u32 *)positions;
    asm ("" : "+m"(position_storage));
    register DrawingPosition *cached asm("r3")=position_pointer;
    asm ("" : "+l"(cached));
    register u32 y asm("r1")=((u8 *)cached)[1]*64;
    asm ("" : "+l"(y));
    register DrawingPosition *position asm("r4")=(DrawingPosition *)&position_storage;
    asm ("" : "+l"(position));
    register u32 x asm("r0")=((u8 *)position)[0]*2;
    asm ("" : "+l"(x));
    register u32 map asm("r2")=0x0600f000;
    asm ("" : "+l"(map));
    register u32 draw_destination asm("r1")=y+(x+map);
    asm ("" : "+l"(draw_destination));
    register u32 tile asm("r2")=position_offset;
    asm ("" : "+l"(tile));
    tile+=0x34;
    tile=(u16)tile;
    asm ("" : : "l"(tile));
    register u32 stride asm("r5");
    QueuePanelTileDraw(saved_panel,(void *)draw_destination,tile,2,2,({register u32 pal asm("r5")=palette_slot;asm ("" : "+l"(pal));register u32 narrow asm("r0")=(u16)pal;asm ("" : : "l"(narrow));narrow;}),({stride=32;asm ("" : "+l"(stride));stride;}));
    register DrawingPosition *second_position asm("r6")=position_pointer;
    asm ("" : "+l"(second_position));
    y=((u8 *)second_position)[1]*64;
    asm ("" : "+l"(y));
    x=((u8 *)position)[0]*2;
    asm ("" : "+l"(x));
    register u32 bottom_map asm("r7")=0x0600f080;
    asm ("" : "+l"(bottom_map));
    u32 zero;
    QueuePanelTileDraw(saved_panel,(void *)(y+(x+bottom_map)),0x54,2,1,({zero=0;zero;}),stride);
    register u32 quantity asm("r0")=value;
    asm ("" : "+l"(quantity));
    if(quantity>1)
    {
        register u32 number_y asm("r2")=((u8 *)second_position)[1]*64;
        asm ("" : "+l"(number_y));
        register u32 number_x asm("r0")=((u8 *)second_position)[0]*2;
        asm ("" : "+l"(number_x));
        register u32 number_map asm("r1")=0x0600f082;
        asm ("" : "+l"(number_map));
        QueuePanelNumberDraw(saved_panel,value,(void *)(number_y+(number_x+number_map)),0x20,zero,stride);
    }
}
