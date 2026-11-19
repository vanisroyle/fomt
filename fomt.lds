OUTPUT_ARCH(arm)

_M4aNumMusicPlayers = 5;
_M4aMaxLines = 0x50;

gMPlayJumpTableTemplate = 0x08139A88;
gScaleTable = 0x08139B18;
gFreqTable = 0x08139BCC;
gPcmSamplesPerVBlankTable = 0x08139BFC;
gCgbScaleTable = 0x08139C14;
gCgbFreqTable = 0x08139C98;
gNoiseTable = 0x08139CB0;
gCgb3Vol = 0x08139CEC;
gClockTable = 0x08139CFC;
gXcmdTable = 0x08139D30;

MEMORY
{
    EWRAM (w!x) : ORIGIN = 0x02000000, LENGTH = 256K
    IWRAM (w!x) : ORIGIN = 0x03000000, LENGTH = 32K
    ROM   (rx)  : ORIGIN = 0x08000000, LENGTH = 32M
}

SECTIONS
{
    .ewram_bss (NOLOAD) : ALIGN(4)
    {
        . = ALIGN(4); src/sound_data.o(.sbss);
        . = ALIGN(4); src/m4a.o(.gSoundInfo);
        . = ALIGN(4); src/m4a.o(.gMPlayJumpTable);
        . = ALIGN(4); src/m4a.o(.gCgbChans);
        . = ALIGN(4); src/m4a.o(.gMusicPlayerA);
        . = ALIGN(4); src/m4a.o(.gMusicPlayerB);
        . = ALIGN(4); src/m4a.o(.gMusicPlayerC);
        . = ALIGN(4); src/m4a.o(.gMusicPlayerE);
        . = ALIGN(4); src/m4a.o(.gMPlayMemAccArea);
        . = ALIGN(4); src/m4a.o(.gMusicPlayerD);
        __ewram_bss_end = .;
    } > EWRAM

    .bss (NOLOAD) : ALIGN(4)
    {
        . = ALIGN(4); src/m4a.o(.bss);
        . = ALIGN(4);
        . = 0x000400; gUnk_03000400 = .;
        . = 0x000402; gUnk_03000402 = .;
        . = 0x000404; gUnk_03000404 = .;
        . = 0x000408; gUnk_03000408 = .;
        . = 0x00040C; gUnk_0300040C = .;
        . = 0x000410; gUnk_03000410 = .;
        . = 0x000414; gUnk_03000414 = .;
        . = 0x000418; gUnk_03000418 = .;
        . = 0x00041C; gUnk_0300041C = .;
        . = 0x000420; gUnk_03000420 = .;
        . = 0x000430; gUnk_03000430 = .;
        . = 0x000450; gUnk_03000450 = .;
        . = 0x000490; /* end */
    } > IWRAM

    .rom : ALIGN(4)
    {
        src/crt0.o(.rom_header);
        asm/main.o(.text);
        src/sram_header.o(.text);
        asm/sram_proxy_1.o(.text);
        src/sram_header.o(.text.flags);
        asm/sram_proxy_1.o(.text.tail);
        src/sram_selection.o(.text);
        src/interrupt.o(.text);
        asm/interrupt.o(.text.enable);
        src/interrupt.o(.text.ack);
        asm/interrupt.o(.text.disable);
        src/interrupt.o(.text.set);
        asm/interrupt.o(.text);
        src/interrupt.o(.text.wait);
        asm/interrupt.o(.text.tail);
        src/new.o(.text);
        src/pure_virtual.o(.text);
        src/sram_proxy.o(.text);
        asm/sram_proxy_2.o(.text);
        asm/sram_proxy_2.o(.text.write);
        src/sram_io.o(.text);
        asm/sram_proxy_2.o(.text.tail);
        src/sram_error.o(.text);
        src/scene.o(.text);
        asm/scene.o(.text);
        asm/intro_scene.o(.text);
        asm/new_game.o(.text);
        src/hardware.o(.text);
        asm/hardware.o(.text.handle_alloc);
        src/hardware.o(.text.handle_retain);
        asm/hardware.o(.text.handle_index);
        src/hardware.o(.text.handle_count);
        asm/hardware.o(.text);
        src/hardware.o(.text.func_0800770C);
        asm/hardware.o(.text.after_0800770C);
        src/hardware.o(.text.func_08007844);
        asm/hardware.o(.text.after_08007844);
        src/hardware.o(.text.func_08007EA8);
        asm/hardware.o(.text.after_08007EA8);
        src/hardware.o(.text.func_08007EC8);
        asm/hardware.o(.text.after_08007EC8);
        src/hardware.o(.text.func_080088B8);
        asm/hardware.o(.text.after_080088B8);
        src/hardware.o(.text.func_080088C4);
        asm/hardware.o(.text.after_080088C4);
        src/hardware.o(.text.func_080088CC);
        asm/hardware.o(.text.after_080088CC);
        src/hardware.o(.text.func_080088D4);
        asm/hardware.o(.text.after_080088D4);
        src/hardware.o(.text.func_080088DC);
        asm/hardware.o(.text.after_080088DC);
        src/hardware.o(.text.func_08008910);
        asm/hardware.o(.text.after_08008910);
        src/hardware.o(.text.func_08008918);
        asm/hardware.o(.text.after_08008918);
        src/hardware.o(.text.func_08008920);
        asm/hardware.o(.text.after_08008920);
        src/hardware.o(.text.func_08008940);
        asm/hardware.o(.text.after_08008940);
        src/hardware.o(.text.func_0800894C);
        asm/hardware.o(.text.after_0800894C);
        src/hardware.o(.text.func_08008AE0);
        asm/hardware.o(.text.after_08008AE0);
        src/hardware.o(.text.func_08008AF0);
        asm/hardware.o(.text.after_08008AF0);
        src/hardware.o(.text.func_08008AFC);
        asm/hardware.o(.text.after_08008AFC);
        src/hardware.o(.text.func_08008B0C);
        asm/hardware.o(.text.after_08008B0C);
        src/hardware.o(.text.func_08008B18);
        asm/hardware.o(.text.after_08008B18);
        src/hardware.o(.text.func_08008B54);
        asm/hardware.o(.text.after_08008B54);
        src/hardware.o(.text.func_08008B6C);
        asm/hardware.o(.text.after_08008B6C);
        src/hardware.o(.text.func_08008B88);
        asm/hardware.o(.text.after_08008B88);
        src/hardware.o(.text.func_08008BB0);
        asm/hardware.o(.text.after_08008BB0);
        src/hardware.o(.text.func_08008BE0);
        asm/hardware.o(.text.after_08008BE0);
        src/hardware.o(.text.func_08008BF8);
        asm/hardware.o(.text.after_08008BF8);
        src/hardware.o(.text.func_08008C28);
        asm/hardware.o(.text.after_08008C28);
        src/hardware.o(.text.func_08008C38);
        asm/hardware.o(.text.after_08008C38);
        src/hardware.o(.text.func_08008C54);
        asm/hardware.o(.text.after_08008C54);
        src/hardware.o(.text.func_08008C70);
        asm/hardware.o(.text.after_08008C70);
        src/hardware.o(.text.func_08008C8C);
        asm/hardware.o(.text.after_08008C8C);
        src/hardware.o(.text.func_08008CA8);
        asm/hardware.o(.text.after_08008CA8);
        src/hardware.o(.text.func_08008CC4);
        asm/hardware.o(.text.after_08008CC4);
        src/hardware.o(.text.func_08008CD0);
        asm/hardware.o(.text.after_08008CD0);
        src/farm.o(.text);
        src/field.o(.text);
        src/shipping_bin.o(.text);
        src/tool_chest.o(.text);
        src/fridge.o(.text);
        src/shelf.o(.text);
        src/record_player.o(.text);
        src/code_0800BC58.o(.text);
        src/farm_house.o(.text);
        src/coop.o(.text);
        src/barn.o(.text);
        src/item.o(.text);
        src/code_0800E2E4.o(.text);
        src/farmer.o(.text);
        src/rucksack_item.o(.text);
        src/held_item.o(.text);
        src/rucksack.o(.text);
        asm/more_items.o(.text);
        src/more_items.o(.text);
        asm/more_items.o(.text.push);
        src/more_items.o(.text.set);
        asm/more_items.o(.text.clear);
        asm/game_scene.o(.text);
        src/game_scene.o(.text);
        asm/game_scene.o(.text.tail);
        asm/game_state.o(.text);
        src/entity.o(.text);
        src/code_entity_08020018.o(.text);
        asm/code_entities.o(.text);
        src/entity_actor.o(.text);
        asm/code_entities_080320DC.o(.text);
        src/npc_entity.o(.text);
        asm/code_entities_08034CEC.o(.text);

        asm/code_0803A8A4.o(.text);
        src/script_engine.o(.text);
        asm/code_0803EE94.o(.text);

        src/actor.o(.text);
        src/animal.o(.text);
        src/pet.o(.text);
        src/livestock.o(.text);
        src/barn_animal.o(.text);
        src/dog.o(.text);
        src/horse.o(.text);
        src/chicken.o(.text);
        src/cow.o(.text);
        src/sheep.o(.text);
        src/code_actor_0809BFE8.o(.text);
        asm/code_actor_0809BFE8.o(.text);
        src/npc.o(.text);
        src/bachelorette.o(.text);
        src/harvest_sprite.o(.text);

        asm/code_809E804.o(.text);

        src/m4a_1.o(.text);
        src/m4a.o(.text);
        asm/code_lib_syscall.o(.text);
        asm/code_lib_sram.o(.text);
        *libgcc.a:_call_via_rX.o(.text);
        /* these need to be recompiled using -mthumb-interwork */
        /* *libc.a:memcmp.o(.text); */
        /* *libc.a:memcpy.o(.text); */
        /* *libc.a:memmove.o(.text); */
        /* *libc.a:memset.o(.text); */
        /* *libc.a:strcat.o(.text); */
        /* *libc.a:strcpy.o(.text); */
        /* *libc.a:strlen.o(.text); */
        asm/code_libc_string.o(.text);

        asm/code_linkonce.o(.text.code_080D3BC0);
        *(.gnu.linkonce.t.__as__6AnimalRC6Animal);
        asm/code_linkonce.o(.text.code_080D68C0);
        *(.gnu.linkonce.t.__as__5SheepRC5Sheep);
        *(.gnu.linkonce.t.__as__3CowRC3Cow);
        *(.gnu.linkonce.t.__as__9LivestockRC9Livestock);
        *(.gnu.linkonce.t.push_back__t8FixedVec2Z12RucksackItemUl8G12RucksackItem);
        *(.gnu.linkonce.t.push_front__t8FixedVec2Z12RucksackItemUl8G12RucksackItem);
        *(.gnu.linkonce.t.push_back__t8FixedVec2Z9ToolStackUl8G9ToolStack);
        *(.gnu.linkonce.t.push_front__t8FixedVec2Z9ToolStackUl8G9ToolStack);
        asm/code_linkonce.o(.text.code_080D7CFC);
        *(.gnu.linkonce.t.__lower_bound__H4ZPC12JumpTableEntZiZ28ScriptJumpTableSearchCompareZl_X01T0RCX11X21PX31_X01);
        asm/code_linkonce.o(.text.code_080E0EF0);

        *(.gnu.linkonce.d.__vt_6AScene);
        *(.gnu.linkonce.d.__vt_13AUnk_0800080C);
        . = ALIGN(4); asm/vtables.o(.rodata);
        *(.gnu.linkonce.d.__vt_7AEntity);
        . = ALIGN(4); asm/vtables.o(.rodata.080E6284);
        *(.gnu.linkonce.d.__vt_13AScriptEngine);
        . = ALIGN(4); asm/vtables.o(.rodata.080E7824);

        . = ALIGN(4); asm/data/data_080E8610.o(.rodata);
        src/shipping_bin.o(.rodata);
        src/record_player.o(.rodata);
        . = ALIGN(4); src/item.o(.rodata);
        /* . = ALIGN(4); src/code_0800E2E4.o(.rodata); */
        . = ALIGN(4); asm/data/data_080F0348.o(.rodata);
        . = ALIGN(4); src/data_schedules.o(.rodata);
        . = ALIGN(4); asm/data/data_080F1A80.o(.rodata);
        . = ALIGN(4); src/script_engine.o(.rodata);
        . = ALIGN(4); asm/data/data_080F9EB8.o(.rodata);
        . = ALIGN(4); src/sound_data.o(.rodata.*);
        . = ALIGN(4); asm/data/data_0813B288.o(.rodata);
    } > ROM = 0xFF

    .iwram : ALIGN(4)
    {
        asm/code_iwram.o(.iwram);
    } > IWRAM AT> ROM = 0xFF

    __iwram_vma     = ADDR(.iwram);
    __iwram_vma_end = ADDR(.iwram) + SIZEOF(.iwram);
    __iwram_lma     = LOADADDR(.iwram);

    .pad : ALIGN(4)
    {
        LONG(0);
        . = 0x08800000 - 0x0875C240;
    } > ROM = 0xFF

    /* DWARF 2 sections */
    .debug_aranges  0 : { *(.debug_aranges) }
    .debug_pubnames 0 : { *(.debug_pubnames) }
    .debug_info     0 : { *(.debug_info) }
    .debug_abbrev   0 : { *(.debug_abbrev) }
    .debug_line     0 : { *(.debug_line) }
    .debug_frame    0 : { *(.debug_frame) }
    .debug_str      0 : { *(.debug_str) }
    .debug_loc      0 : { *(.debug_loc) }
    .debug_macinfo  0 : { *(.debug_macinfo) }

    /* Discard everything not specifically mentioned above. */
    /DISCARD/ :
    {
        *(*);
    }
}
