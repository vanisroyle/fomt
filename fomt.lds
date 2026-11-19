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
        asm/sram_proxy_1.o(.text);
        asm/interrupt.o(.text);
        src/new.o(.text);
        src/pure_virtual.o(.text);
        asm/sram_proxy_2.o(.text);
        src/scene.o(.text);
        asm/scene.o(.text);
        asm/intro_scene.o(.text);
        asm/new_game.o(.text);
        src/hardware.o(.text);
        asm/hardware.o(.text);
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
        asm/game_scene.o(.text);
        src/game_state_part1.o(.text.game_state_08010348);
        asm/game_state.o(.text.game_state_08010358);
        src/game_state_part1.o(.text.game_state_08010E48);
        asm/game_state.o(.text.game_state_08010E68);
        src/game_state_part1.o(.text.game_state_08010F04);
        asm/game_state.o(.text.game_state_08010F0C);
        src/game_state_part1.o(.text.game_state_08010F1C);
        asm/game_state.o(.text.game_state_08010F54);
        src/game_state_part1.o(.text.game_state_08011458);
        asm/game_state.o(.text.game_state_08011464);
        src/game_state_part1.o(.text.game_state_080114F8);
        src/game_state_part1.o(.text.game_state_08011510);
        asm/game_state.o(.text.game_state_08011524);
        src/game_state_part1.o(.text.game_state_08011524);
        src/game_state_part1.o(.text.game_state_08011540);
        asm/game_state.o(.text.game_state_08011550);
        src/game_state_part1.o(.text.game_state_08011550);
        asm/game_state.o(.text.game_state_08011568);
        src/game_state_part1.o(.text.game_state_08011568);
        asm/game_state.o(.text.game_state_08011588);
        src/game_state_part1.o(.text.game_state_08012218);
        asm/game_state.o(.text.game_state_0801221C);
        src/game_state_part1.o(.text.game_state_0801221C);
        asm/game_state.o(.text.game_state_0801223C);
        src/game_state_part1.o(.text.game_state_080122A0);
        asm/game_state.o(.text.game_state_080122BC);
        src/game_state_part1.o(.text.game_state_08012454);
        asm/game_state.o(.text.game_state_08012464);
        src/game_state_part1.o(.text.game_state_080124C4);
        asm/game_state.o(.text.game_state_080124D0);
        src/game_state_part1.o(.text.game_state_0801274C);
        asm/game_state.o(.text.game_state_08012760);
        src/game_state_part1.o(.text.game_state_0801278C);
        asm/game_state.o(.text.game_state_080127A0);
        src/game_state_part1.o(.text.game_state_0801284C);
        asm/game_state.o(.text.game_state_08012860);
        src/game_state_part1.o(.text.game_state_08012ACC);
        asm/game_state.o(.text.game_state_08012ADC);
        src/game_state_part1.o(.text.game_state_08012BAC);
        asm/game_state.o(.text.game_state_08012BBC);
        src/game_state_part1.o(.text.game_state_08014034);
        asm/game_state.o(.text.game_state_0801410C);
        src/game_state_part1.o(.text.game_state_0801410C);
        asm/game_state.o(.text.game_state_0801412C);
        src/game_state_part1.o(.text.game_state_08014214);
        src/game_state_part1.o(.text.game_state_08014234);
        asm/game_state.o(.text.game_state_08014264);
        src/game_state_part1.o(.text.game_state_080142B8);
        asm/game_state.o(.text.game_state_080142D4);
        src/game_state_part1.o(.text.game_state_080142D4);
        src/game_state_part1.o(.text.game_state_080142F0);
        asm/game_state.o(.text.game_state_08014304);
        src/game_state_part1.o(.text.game_state_08014304);
        asm/game_state.o(.text.game_state_08014318);
        src/game_state_part1.o(.text.game_state_0801468C);
        asm/game_state.o(.text.game_state_08014694);
        src/game_state_part1.o(.text.game_state_08014694);
        asm/game_state.o(.text.game_state_080146B0);
        src/game_state_part1.o(.text.game_state_080146B0);
        asm/game_state.o(.text.game_state_080146CC);
        src/game_state_part1.o(.text.game_state_08014D5C);
        asm/game_state.o(.text.game_state_08014D7C);
        src/game_state_part1.o(.text.game_state_08014D7C);
        asm/game_state.o(.text.game_state_08014D9C);
        src/game_state_part1.o(.text.game_state_08015950);
        asm/game_state.o(.text.game_state_08015970);
        src/game_state_part1.o(.text.game_state_08015970);
        asm/game_state.o(.text.game_state_08015990);
        src/game_state_part1.o(.text.game_state_08015990);
        asm/game_state.o(.text.game_state_080159B0);
        src/game_state_part1.o(.text.game_state_080167AC);
        src/game_state_part1.o(.text.game_state_080167CC);
        asm/game_state.o(.text.game_state_080167DC);
        src/game_state_part1.o(.text.game_state_08016BA4);
        asm/game_state.o(.text.game_state_08016BC0);
        src/game_state_part1.o(.text.game_state_08016BC0);
        src/game_state_part1.o(.text.game_state_08016BDC);
        asm/game_state.o(.text.game_state_08016C10);
        src/game_state_part1.o(.text.game_state_08016C10);
        asm/game_state.o(.text.game_state_08016C2C);
        src/game_state_part1.o(.text.game_state_08016C2C);
        asm/game_state.o(.text.game_state_08016C48);
        src/game_state_part1.o(.text.game_state_08016C48);
        src/game_state_part1.o(.text.game_state_08016C6C);
        asm/game_state.o(.text.game_state_08016C88);
        src/game_state_part1.o(.text.game_state_08016C88);
        src/game_state_part1.o(.text.game_state_08016CAC);
        asm/game_state.o(.text.game_state_08016CAC);
        asm/game_state.o(.text.game_state_08016CCC);
        src/game_state_part1.o(.text.game_state_08016CCC);
        asm/game_state.o(.text.game_state_08016CEC);
        src/game_state_part1.o(.text.game_state_08016D80);
        asm/game_state.o(.text.game_state_08016D9C);
        src/game_state_part1.o(.text.game_state_08016E7C);
        asm/game_state.o(.text.game_state_08016E9C);
        src/game_state_part1.o(.text.game_state_08016EF0);
        asm/game_state.o(.text.game_state_08016F0C);
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
