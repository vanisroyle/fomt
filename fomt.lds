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
        . = 0x000418; gHeapHead = .;
        . = 0x00041C; gCurrentJoybusCommand = .;
        . = 0x000420; gJoybusState = .;
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
        src/game_state_part1.o(.text.game_state_08010F0C);
        asm/game_state.o(.text.game_state_08010F14);
        src/game_state_part1.o(.text.game_state_08010F1C);
        asm/game_state.o(.text.game_state_08010F54);
        src/game_state_part1.o(.text.game_state_08011458);
        asm/game_state.o(.text.game_state_08011464);
        src/game_state_part1.o(.text.game_state_08011464);
        asm/game_state.o(.text.game_state_08011498);
        src/game_state_part1.o(.text.game_state_080114C8);
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
        src/game_state_part1.o(.text.game_state_08011588);
        asm/game_state.o(.text.game_state_080115A8);
        src/game_state_part1.o(.text.game_state_080115A8);
        asm/game_state.o(.text.game_state_080115B0);
        src/game_state_part1.o(.text.game_state_08011FE8);
        asm/game_state.o(.text.game_state_08012028);
        src/game_state_part1.o(.text.game_state_08012064);
        src/game_state_part1.o(.text.game_state_080120A4);
        asm/game_state.o(.text.game_state_080120C4);
        src/game_state_part1.o(.text.game_state_080120C4);
        asm/game_state.o(.text.game_state_080120E4);
        src/game_state_part1.o(.text.game_state_080120E4);
        src/game_state_part1.o(.text.game_state_08012114);
        asm/game_state.o(.text.game_state_08012134);
        src/game_state_part1.o(.text.game_state_08012134);
        asm/game_state.o(.text.game_state_08012154);
        src/game_state_part1.o(.text.game_state_08012154);
        asm/game_state.o(.text.game_state_08012174);
        src/game_state_part1.o(.text.game_state_08012174);
        asm/game_state.o(.text.game_state_0801219C);
        src/game_state_part1.o(.text.game_state_0801219C);
        asm/game_state.o(.text.game_state_080121C4);
        src/game_state_part1.o(.text.game_state_080121C4);
        asm/game_state.o(.text.game_state_080121EC);
        src/game_state_part1.o(.text.game_state_080121EC);
        src/game_state_part1.o(.text.game_state_08012218);
        asm/game_state.o(.text.game_state_0801221C);
        src/game_state_part1.o(.text.game_state_0801221C);
        asm/game_state.o(.text.game_state_0801223C);
        src/game_state_part1.o(.text.game_state_0801223C);
        src/game_state_part1.o(.text.game_state_08012274);
        src/game_state_part1.o(.text.game_state_080122A0);
        asm/game_state.o(.text.game_state_080122BC);
        src/game_state_part1.o(.text.game_state_080122BC);
        asm/game_state.o(.text.game_state_080122E0);
        src/game_state_part1.o(.text.game_state_0801238C);
        asm/game_state.o(.text.game_state_080123A4);
        src/game_state_part1.o(.text.game_state_080123A4);
        asm/game_state.o(.text.game_state_080123CC);
        src/game_state_part1.o(.text.game_state_08012454);
        asm/game_state.o(.text.game_state_08012464);
        src/game_state_part1.o(.text.game_state_08012464);
        src/game_state_part1.o(.text.game_state_080124C4);
        asm/game_state.o(.text.game_state_080124D0);
        src/game_state_part1.o(.text.game_state_08012658);
        asm/game_state.o(.text.game_state_0801268C);
        src/game_state_part1.o(.text.game_state_0801268C);
        src/game_state_part1.o(.text.game_state_080126BC);
        asm/game_state.o(.text.game_state_080126E4);
        src/game_state_part1.o(.text.game_state_080126E4);
        src/game_state_part1.o(.text.game_state_08012720);
        asm/game_state.o(.text.game_state_08012720);
        src/game_state_part1.o(.text.game_state_0801274C);
        asm/game_state.o(.text.game_state_08012760);
        src/game_state_part1.o(.text.game_state_08012760);
        src/game_state_part1.o(.text.game_state_0801278C);
        asm/game_state.o(.text.game_state_080127A0);
        src/game_state_part1.o(.text.game_state_0801284C);
        asm/game_state.o(.text.game_state_08012860);
        src/game_state_part1.o(.text.game_state_08012AA4);
        src/game_state_part1.o(.text.game_state_08012ACC);
        asm/game_state.o(.text.game_state_08012ADC);
        src/game_state_part1.o(.text.game_state_08012ADC);
        src/game_state_part1.o(.text.game_state_08012B04);
        asm/game_state.o(.text.game_state_08012B04);
        asm/game_state.o(.text.game_state_08012B24);
        src/game_state_part1.o(.text.game_state_08012BAC);
        asm/game_state.o(.text.game_state_08012BBC);
        src/game_state_part1.o(.text.game_state_08014034);
        asm/game_state.o(.text.game_state_0801410C);
        src/game_state_part1.o(.text.game_state_0801410C);
        asm/game_state.o(.text.game_state_0801412C);
        src/game_state_part1.o(.text.game_state_08014164);
        src/game_state_part1.o(.text.game_state_08014198);
        src/game_state_part1.o(.text.game_state_080141C4);
        asm/game_state.o(.text.game_state_080141EC);
        src/game_state_part1.o(.text.game_state_080141EC);
        src/game_state_part1.o(.text.game_state_08014214);
        src/game_state_part1.o(.text.game_state_08014234);
        asm/game_state.o(.text.game_state_08014264);
        src/game_state_part1.o(.text.game_state_08014264);
        src/game_state_part1.o(.text.game_state_08014290);
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
        src/game_state_part1.o(.text.game_state_080146CC);
        asm/game_state.o(.text.game_state_080146FC);
        src/game_state_part1.o(.text.game_state_08014C0C);
        asm/game_state.o(.text.game_state_08014C34);
        src/game_state_part1.o(.text.game_state_08014D30);
        src/game_state_part1.o(.text.game_state_08014D5C);
        asm/game_state.o(.text.game_state_08014D7C);
        src/game_state_part1.o(.text.game_state_08014D7C);
        asm/game_state.o(.text.game_state_08014D9C);
        src/game_state_part1.o(.text.game_state_0801589C);
        src/game_state_part1.o(.text.game_state_080158CC);
        src/game_state_part1.o(.text.game_state_080158F8);
        asm/game_state.o(.text.game_state_08015920);
        src/game_state_part1.o(.text.game_state_08015920);
        src/game_state_part1.o(.text.game_state_08015950);
        asm/game_state.o(.text.game_state_08015970);
        src/game_state_part1.o(.text.game_state_08015970);
        asm/game_state.o(.text.game_state_08015990);
        src/game_state_part1.o(.text.game_state_08015990);
        asm/game_state.o(.text.game_state_080159B0);
        src/game_state_part1.o(.text.game_state_080167AC);
        src/game_state_part1.o(.text.game_state_080167CC);
        asm/game_state.o(.text.game_state_080167DC);
        src/game_state_part1.o(.text.game_state_080167DC);
        asm/game_state.o(.text.game_state_08016834);
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
        src/game_state_part1.o(.text.game_state_08016CEC);
        asm/game_state.o(.text.game_state_08016D34);
        src/game_state_part1.o(.text.game_state_08016D48);
        src/game_state_part1.o(.text.game_state_08016D80);
        asm/game_state.o(.text.game_state_08016D9C);
        src/game_state_part1.o(.text.game_state_08016D9C);
        asm/game_state.o(.text.game_state_08016DB0);
        src/game_state_part1.o(.text.game_state_08016E7C);
        asm/game_state.o(.text.game_state_08016E9C);
        src/game_state_part1.o(.text.game_state_08016E9C);
        asm/game_state.o(.text.game_state_08016EC4);
        src/game_state_part1.o(.text.game_state_08016EC4);
        src/game_state_part1.o(.text.game_state_08016EF0);
        asm/game_state.o(.text.game_state_08016F0C);
        src/game_state_part1.o(.text.game_state_08016F0C);
        asm/game_state.o(.text.game_state_08016F34);
        src/game_state_part1.o(.text.game_state_08016F34);
        asm/game_state.o(.text.game_state_08016F60);
        src/game_state_part1.o(.text.game_state_08016F60);
        asm/game_state.o(.text.game_state_08016FA4);
        asm/game_state.o(.text);
        src/entity.o(.text);
        src/code_entity_08020018.o(.text);
        *(SORT_BY_NAME(.text.entities_part1.*));
        *(SORT_BY_NAME(.text.entities_part2.*));
        src/entity_actor.o(.text);
        asm/code_entities_080320DC.o(.text.npc_side.0803242C);
        src/npc_side_entities.o(.text.npc_side.080324B8);
        asm/code_entities_080320DC.o(.text.npc_side.080324BC);
        src/npc_side_entities.o(.text.npc_side.08032900);
        asm/code_entities_080320DC.o(.text.npc_side.08032934);
        src/npc_side_entities.o(.text.npc_side.08032A30);
        asm/code_entities_080320DC.o(.text.npc_side.08032A58);
        src/npc_side_entities.o(.text.npc_side.08032A84);
        src/npc_side_entities.o(.text.npc_side.08032A8C);
        src/npc_side_entities.o(.text.npc_side.08032A94);
        src/npc_side_entities.o(.text.npc_side.08032A9C);
        src/npc_side_entities.o(.text.npc_side.08032AA4);
        asm/code_entities_080320DC.o(.text.npc_side.08032AB4);
        src/npc_side_entities.o(.text.npc_side.080338F0);
        asm/code_entities_080320DC.o(.text.npc_side.08033914);
        asm/code_entities_080320DC.o(.text.npc_side.08033928);
        src/npc_side_entities.o(.text.npc_side.08033B7C);
        asm/code_entities_080320DC.o(.text.npc_side.08033B84);
        src/npc_side_entities.o(.text.npc_side.08034248);
        asm/code_entities_080320DC.o(.text.npc_side.08034260);
        src/npc_entity.o(.text);
        asm/code_entities_08034CEC.o(.text);

        asm/code_0803A8A4.o(.text);
        src/code_0803A8A4.o(.text.0803B454);
        asm/code_0803A8A4.o(.text.0803B4DC);
        src/code_0803A8A4.o(.text.0803B6B8);
        asm/code_0803A8A4.o(.text.0803B718);
        src/code_0803A8A4.o(.text.0803BCF8);
        src/code_0803A8A4.o(.text.0803BD60);
        asm/code_0803A8A4.o(.text.0803BF14);
        src/code_0803A8A4.o(.text.0803BF78);
        asm/code_0803A8A4.o(.text.0803BFF0);
        src/code_0803A8A4.o(.text.0803C00C);
        asm/code_0803A8A4.o(.text.0803C028);
        src/code_0803A8A4.o(.text.0803C518);
        asm/code_0803A8A4.o(.text.0803C5B0);
        src/code_0803A8A4.o(.text.0803C7A0);
        asm/code_0803A8A4.o(.text.0803C7C8);
        src/code_0803A8A4.o(.text.0803D080);
        asm/code_0803A8A4.o(.text.0803D0F0);
        src/code_0803A8A4.o(.text.0803D2E8);
        asm/code_0803A8A4.o(.text.0803D464);
        src/code_0803A8A4.o(.text.0803D4D8);
        asm/code_0803A8A4.o(.text.0803D52C);
        src/code_0803A8A4.o(.text.0803D644);
        asm/code_0803A8A4.o(.text.0803D688);
        src/code_0803A8A4.o(.text.0803D7E4);
        asm/code_0803A8A4.o(.text.0803DA7C);
        src/code_0803A8A4.o(.text.0803DC64);
        asm/code_0803A8A4.o(.text.0803DCB4);
        src/code_0803A8A4.o(.text.0803DECC);
        asm/code_0803A8A4.o(.text.0803DFE8);
        src/code_0803A8A4.o(.text.0803E1A4);
        asm/code_0803A8A4.o(.text.0803E264);
        src/code_0803A8A4.o(.text.0803E40C);
        asm/code_0803A8A4.o(.text.0803E4A0);
        src/code_0803A8A4.o(.text.0803E620);
        asm/code_0803A8A4.o(.text.0803E780);
        src/code_0803A8A4.o(.text.0803E8E8);
        src/code_0803A8A4.o(.text.0803ECC0);
        src/code_0803A8A4.o(.text.0803EDCC);
        src/script_engine.o(.text);
        asm/code_0803EE94.o(.text);
        *(SORT_BY_NAME(.text.message_window.*));

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
        src/code_actor_0809BFE8.o(.text.0809C0A0);
        src/code_actor_0809BFE8.o(.text.0809C4EC);
        asm/code_actor_0809BFE8.o(.text.0809C510);
        src/code_actor_0809BFE8.o(.text.0809C5B4);
        asm/code_actor_0809BFE8.o(.text.0809C6BC);
        src/code_actor_0809BFE8.o(.text.0809CD78);
        asm/code_actor_0809BFE8.o(.text.0809CE8C);
        src/code_actor_0809BFE8.o(.text.0809D418);
        asm/code_actor_0809BFE8.o(.text.0809D500);
        src/code_actor_0809BFE8.o(.text.0809D7D8);
        asm/code_actor_0809BFE8.o(.text.0809D8E8);
        src/code_actor_0809BFE8.o(.text.0809D9B4);
        asm/code_actor_0809BFE8.o(.text.0809DA00);
        src/code_actor_0809BFE8.o(.text.0809E174);
        asm/code_actor_0809BFE8.o(.text.0809E1B4);
        src/npc.o(.text);
        src/bachelorette.o(.text);
        src/harvest_sprite.o(.text);

        asm/code_809E804.o(.text);
        asm/code_080C8360.o(.text);
        src/g5_panels.o(.text.g5_080C8550);
        asm/code_080C8360.o(.text.g5_after_080C8550);
        src/g5_encoding.o(.text.g5_080CA6F4);
        asm/code_080C8360.o(.text.g5_after_080CA6F4);
        src/g5_ui.o(.text.g5_buttons);
        asm/code_080C8360.o(.text.g5_CAC3C);
        src/g5_ui.o(.text.g5_entry_init);
        asm/code_080C8360.o(.text.g5_CAC7C);
        src/g5_ui.o(.text.g5_entry_access);
        asm/code_080C8360.o(.text.g5_CAD18);
        src/g5_ui.o(.text.g5_entry_update);
        asm/code_080C8360.o(.text.g5_CADB4);
        src/g5_commands.o(.text.g5_080CADF0);
        asm/code_080C8360.o(.text.g5_after_080CADF0);
        src/g5_commands.o(.text.g5_080CAE60);
        asm/code_080C8360.o(.text.g5_after_080CAE60);
        src/g5_commands.o(.text.g5_080CAF6C);
        asm/code_080C8360.o(.text.g5_after_080CAF6C);
        src/g5_helpers.o(.text.g5_080CB2E8);
        asm/code_080C8360.o(.text.g5_080CB304);
        src/g5_drawing.o(.text.g5_080CB948);
        asm/code_080C8360.o(.text.g5_after_080CB948);
        src/g5_helpers.o(.text.g5_080CC134);
        asm/code_080C8360.o(.text.g5_080CC15C);
        src/g5_helpers.o(.text.g5_080CC70C);
        asm/code_080C8360.o(.text.g5_080CC728);
        src/g5_drawing.o(.text.g5_080CCD48);
        asm/code_080C8360.o(.text.g5_after_080CCD48);
        src/g5_helpers.o(.text.g5_080CD534);
        asm/code_080C8360.o(.text.g5_080CD578);
        src/g5_objects.o(.text.g5_080CD950);
        asm/code_080C8360.o(.text.g5_after_080CD950);
        src/g5_navigation.o(.text.g5_080CDA44);
        asm/code_080C8360.o(.text.g5_after_080CDA44);
        src/g5_helpers.o(.text.g5_080CE184);
        asm/code_080C8360.o(.text.g5_080CE19C);
        src/g5_panels.o(.text.g5_080CE19C);
        asm/code_080C8360.o(.text.g5_after_080CE19C);
        src/g5_helpers.o(.text.g5_080CE1C4);
        src/g5_panels.o(.text.g5_080CE294);
        asm/code_080C8360.o(.text.g5_after_080CE294);
        src/g5_commands.o(.text.g5_080CE3DC);
        asm/code_080C8360.o(.text.g5_after_080CE3DC);
        src/g5_commands.o(.text.g5_080CE480);
        asm/code_080C8360.o(.text.g5_after_080CE480);
        src/g5_panels.o(.text.g5_080CEB80);
        asm/code_080C8360.o(.text.g5_after_080CEB80);
        src/g5_objects.o(.text.g5_080CEC84);
        src/g5_panels.o(.text.g5_080CECD0);
        asm/code_080C8360.o(.text.g5_after_080CECD0);
        src/g5_helpers.o(.text.g5_080CF050);
        asm/code_080C8360.o(.text.g5_080CF05C);
        src/g5_panels.o(.text.g5_080CFB28);
        asm/code_080C8360.o(.text.g5_after_080CFB28);
        src/g5_memory.o(.text);
        src/g5_runtime.o(.text.g5_init);
        asm/code_080D035C.o(.text);
        src/g5_runtime.o(.text.g5_requests);
        asm/code_080D035C.o(.text.g5_0490);
        src/g5_runtime.o(.text.g5_transfer);
        asm/code_080D035C.o(.text.g5_after_transfer);
        src/g5_runtime.o(.text.g5_queries);
        asm/code_080D035C.o(.text.g5_06DC);
        src/g5_runtime.o(.text.g5_commands);
        src/g5_runtime.o(.text);
        asm/code_080D0C68.o(.text);
        src/g5_encoding.o(.text.g5_080D0CD4);
        asm/code_080D0C68.o(.text.g5_after_080D0CD4);
        asm/code_080D102C.o(.text);

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
