    .INCLUDE "asm/macro.inc"
    .SYNTAX UNIFIED

    .section .text.npc_side.0803242C, "ax", %progbits

    thumb_func_start func_0803242C
func_0803242C: @ 0x0803242C
    push {r4, r5, r6, r7, lr}
    mov r7, r8
    push {r7}
    adds r3, r0, #0
    mov r8, r1
    ldrh r0, [r3, #0x24]
    cmp r0, #0
    beq .L08032440
    subs r0, #1
    b .L08032442
.L08032440:
    ldrh r0, [r3, #0x26]
.L08032442:
    strh r0, [r3, #0x24]
    ldr r4, [r3, #0x28]
    cmp r4, #0
    beq .L080324A6
    movs r2, #0
    adds r0, r3, #0
    adds r0, #0x2e
    ldrb r0, [r0]
    cmp r0, #0
    bne .L08032458
    movs r2, #1
.L08032458:
    mov ip, r2
    cmp r2, #0
    beq .L08032462
    ldr r2, [r3, #8]
    b .L08032464
.L08032462:
    ldr r2, [r3, #0xc]
.L08032464:
    asrs r5, r2, #0x10
    movs r0, #0x2c
    ldrsh r6, [r3, r0]
    subs r0, r6, r5
    adds r7, r0, #0
    cmp r0, #0
    bne .L08032476
    str r0, [r3, #0x28]
    b .L080324A6
.L08032476:
    adds r1, r2, r4
    cmp r0, #0
    bgt .L0803247E
    subs r1, r2, r4
.L0803247E:
    adds r2, r1, #0
    asrs r0, r2, #0x10
    cmp r0, r5
    beq .L0803249A
    subs r0, r6, r0
    cmp r7, #0
    ble .L08032492
    cmp r0, #0
    ble .L08032496
    b .L0803249A
.L08032492:
    cmp r0, #0
    blt .L0803249A
.L08032496:
    movs r0, #0
    str r0, [r3, #0x28]
.L0803249A:
    mov r0, ip
    cmp r0, #0
    beq .L080324A4
    str r2, [r3, #8]
    b .L080324A6
.L080324A4:
    str r2, [r3, #0xc]
.L080324A6:
    adds r0, r3, #0
    mov r1, r8
    bl vfunc_2C__7AEntityUi
    pop {r3}
    mov r8, r3
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0

    .section .text.npc_side.080324BC, "ax", %progbits

    .section .text.npc_side.0803260C, "ax", %progbits

    .section .text.npc_side.08032690, "ax", %progbits

    thumb_func_start func_08032690
func_08032690: @ 0x08032690
    push {r4, r5, r6, r7, lr}
    mov r7, sl
    mov r6, sb
    mov r5, r8
    push {r5, r6, r7}
    sub sp, #0x50
    adds r6, r0, #0
    mov sl, r1
    ldr r0, [r6]
    mov r8, r0
    ldr r1, [r0]
    str r1, [sp, #0x3c]
    movs r2, #0xa
    ldrsh r1, [r0, r2]
    mov r3, sl
    movs r4, #0xc
    ldrsh r0, [r3, r4]
    subs r1, r1, r0
    str r1, [sp, #0x40]
    mov r0, r8
    movs r2, #0xe
    ldrsh r1, [r0, r2]
    movs r4, #0xe
    ldrsh r0, [r3, r4]
    subs r0, r1, r0
    str r0, [sp, #0x44]
    movs r0, #0x80
    lsls r0, r0, #8
    subs r0, r0, r1
    str r0, [sp, #0x48]
    add r2, sp, #0x34
    ldr r0, [r3]
    ldr r1, [r3, #8]
    str r0, [sp, #0x34]
    str r1, [r2, #4]
    adds r0, r6, #0
    adds r0, #0x8a
    ldrb r1, [r0]
    lsls r0, r1, #0x1e
    lsrs r0, r0, #0x1e
    cmp r0, #0
    beq .L080327C2
    cmp r0, #0
    blt .L080327C2
    cmp r0, #2
    bgt .L080327C2
    mov r0, r8
    adds r0, #0x20
    ldrb r0, [r0]
    lsrs r1, r1, #2
    lsls r1, r1, #4
    lsls r0, r0, #2
    ldr r2, .L08032764 @ =gUnk_080F1328
    adds r0, r0, r2
    adds r1, r1, r0
    movs r2, #0
    ldrsh r0, [r1, r2]
    movs r3, #2
    ldrsh r1, [r1, r3]
    adds r5, r6, #0
    adds r5, #0x48
    ldr r4, [sp, #0x40]
    adds r0, r4, r0
    str r0, [sp, #0x4c]
    ldr r0, [sp, #0x44]
    adds r0, r0, r1
    mov sb, r0
    adds r0, r6, #0
    adds r0, #0x70
    ldr r1, [r6, #0x70]
    ldr r3, [r1]
    ldrh r2, [r0, #0xc]
    lsls r2, r2, #2
    ldr r0, [r6, #0x74]
    adds r0, r0, r2
    ldrh r2, [r0]
    add r0, sp, #0x14
    ldr r3, [r3, #0x10]
    bl _call_via_r3
    add r7, sp, #0x14
    ldr r3, [sp, #0x38]
    ldr r2, [r6, #0x48]
    ldr r1, [r6, #0x50]
    rsbs r0, r1, #0
    orrs r0, r1
    cmp r0, #0
    bge .L0803276C
    ldrh r0, [r5, #0xc]
    adds r1, r6, #0
    adds r1, #0x58
    ldr r4, [sp, #0x48]
    str r4, [sp]
    adds r4, r7, #0
    str r4, [sp, #4]
    str r2, [sp, #8]
    str r0, [sp, #0xc]
    str r1, [sp, #0x10]
    adds r0, r3, #0
    ldr r1, [sp, #0x4c]
    mov r2, sb
    movs r3, #0x55
    ldr r4, .L08032768 @ =func_030004DC
    bl _call_via_r4
    b .L0803276E
    .align 2, 0
.L08032764: .4byte gUnk_080F1328
.L08032768: .4byte func_030004DC
.L0803276C:
    movs r0, #0
.L0803276E:
    cmp r0, #0
    beq .L080327C2
    adds r0, r5, #0
    adds r0, #0x3c
    ldrb r1, [r0]
    mov sb, r0
    cmp r1, #0
    beq .L080327C2
    ldr r1, [sp, #0x34]
    adds r2, r7, #0
    adds r2, #8
    adds r0, r5, #0
    bl func_080A480C
    adds r0, r5, #0
    adds r0, #0x3d
    ldrb r0, [r0]
    cmp r0, #0
    beq .L080327A2
    adds r1, r7, #0
    adds r1, #0x10
    adds r0, r5, #0
    movs r2, #1
    bl LoadMapSpriteResourcePalettes
    b .L080327BC
.L080327A2:
    adds r4, r5, #0
    adds r4, #0x3e
    ldrb r0, [r4]
    cmp r0, #0
    bne .L080327BC
    adds r1, r7, #0
    adds r1, #0x10
    adds r0, r5, #0
    movs r2, #1
    bl LoadMapSpriteResourcePalettes
    movs r0, #1
    strb r0, [r4]
.L080327BC:
    movs r0, #0
    mov r1, sb
    strb r0, [r1]
.L080327C2:
    adds r0, r6, #0
    adds r0, #0x8b
    ldrb r0, [r0]
    cmp r0, #1
    beq .L080327EC
    cmp r0, #1
    ble .L080327D4
    cmp r0, #2
    beq .L080327F0
.L080327D4:
    mov r0, r8
    adds r0, #0x21
    ldrb r0, [r0]
    movs r1, #3
    ands r0, r1
    lsls r4, r0, #2
    orrs r4, r0
    lsls r1, r0, #4
    orrs r4, r1
    lsls r0, r0, #6
    orrs r4, r0
    b .L080327F2
.L080327EC:
    movs r4, #0x19
    b .L080327F2
.L080327F0:
    movs r4, #0x1a
.L080327F2:
    adds r5, r6, #0
    adds r5, #8
    add r2, sp, #0x34
    mov r8, r2
    ldr r1, [r6, #0x30]
    ldr r3, [r1]
    ldrh r2, [r6, #0x3c]
    lsls r2, r2, #2
    ldr r0, [r6, #0x34]
    adds r0, r0, r2
    ldrh r2, [r0]
    add r0, sp, #0x14
    ldr r3, [r3, #0x10]
    bl _call_via_r3
    add r7, sp, #0x14
    mov r3, r8
    ldr r3, [r3, #4]
    mov sb, r3
    ldr r2, [r6, #8]
    ldr r1, [r6, #0x10]
    rsbs r0, r1, #0
    orrs r0, r1
    cmp r0, #0
    bge .L0803284C
    ldrh r0, [r5, #0xc]
    adds r1, r6, #0
    adds r1, #0x18
    ldr r3, [sp, #0x48]
    str r3, [sp]
    adds r3, r7, #0
    str r3, [sp, #4]
    str r2, [sp, #8]
    str r0, [sp, #0xc]
    str r1, [sp, #0x10]
    mov r0, sb
    ldr r1, [sp, #0x40]
    ldr r2, [sp, #0x44]
    adds r3, r4, #0
    ldr r4, .L08032848 @ =func_030004DC
    bl _call_via_r4
    b .L0803284E
    .align 2, 0
.L08032848: .4byte func_030004DC
.L0803284C:
    movs r0, #0
.L0803284E:
    cmp r0, #0
    beq .L080328A4
    adds r0, r5, #0
    adds r0, #0x3c
    ldrb r1, [r0]
    mov sb, r0
    cmp r1, #0
    beq .L080328A4
    mov r4, r8
    ldr r1, [r4]
    adds r2, r7, #0
    adds r2, #8
    adds r0, r5, #0
    bl func_080A480C
    adds r0, r5, #0
    adds r0, #0x3d
    ldrb r0, [r0]
    cmp r0, #0
    beq .L08032884
    adds r1, r7, #0
    adds r1, #0x10
    adds r0, r5, #0
    movs r2, #1
    bl LoadMapSpriteResourcePalettes
    b .L0803289E
.L08032884:
    adds r4, r5, #0
    adds r4, #0x3e
    ldrb r0, [r4]
    cmp r0, #0
    bne .L0803289E
    adds r1, r7, #0
    adds r1, #0x10
    adds r0, r5, #0
    movs r2, #1
    bl LoadMapSpriteResourcePalettes
    movs r0, #1
    strb r0, [r4]
.L0803289E:
    movs r0, #0
    mov r1, sb
    strb r0, [r1]
.L080328A4:
    adds r0, r6, #0
    adds r0, #0x88
    ldrb r0, [r0]
    cmp r0, #1
    beq .L080328C6
    cmp r0, #1
    bgt .L080328B8
    cmp r0, #0
    beq .L080328C2
    b .L080328C6
.L080328B8:
    cmp r0, #2
    beq .L080328CA
    cmp r0, #3
    bne .L080328C6
    b .L080328F0
.L080328C2:
    movs r4, #0
    b .L080328CC
.L080328C6:
    movs r4, #1
    b .L080328CC
.L080328CA:
    movs r4, #2
.L080328CC:
    ldr r2, [sp, #0x3c]
    ldr r0, [r2]
    ldr r1, [r0, #0x58]
    adds r0, r2, #0
    bl _call_via_r1
    mov r3, sl
    ldr r1, [r3, #8]
    adds r2, r6, #0
    adds r2, #0x89
    movs r3, #0
    ldrsb r3, [r2, r3]
    ldr r2, [sp, #0x44]
    adds r3, r2, r3
    str r4, [sp]
    ldr r2, [sp, #0x40]
    bl DrawFixedSpriteOverlay
.L080328F0:
    add sp, #0x50
    pop {r3, r4, r5}
    mov r8, r3
    mov sb, r4
    mov sl, r5
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0

    .section .text.npc_side.08032934, "ax", %progbits

    .section .text.npc_side.0803295C, "ax", %progbits

    .section .text.npc_side.080329DC, "ax", %progbits

    .section .text.npc_side.08032A58, "ax", %progbits

    .section .text.npc_side.08032A64, "ax", %progbits

    .section .text.npc_side.08032AB4, "ax", %progbits
    .section .text.npc_side.08032B44, "ax", %progbits

    .section .text.npc_side.08032B5C, "ax", %progbits

    .section .text.npc_side.08032B68, "ax", %progbits

    .section .text.npc_side.08032B88, "ax", %progbits

    .section .text.npc_side.08032BB4, "ax", %progbits

    thumb_func_start func_08032BB4
func_08032BB4: @ 0x08032BB4
    push {r4, r5, r6, r7, lr}
    mov r7, sl
    mov r6, sb
    mov r5, r8
    push {r5, r6, r7}
    sub sp, #0x48
    str r0, [sp, #0x28]
    str r1, [sp, #0x2c]
    mov sl, r2
    str r3, [sp, #0x30]
    mov r2, sp
    ldrb r1, [r2]
    movs r4, #0x10
    rsbs r4, r4, #0
    adds r0, r4, #0
    ands r0, r1
    strb r0, [r2]
    movs r0, #0
    str r0, [sp, #0x34]
    ldr r1, [sp, #0x30]
    cmp r1, #0x25
    beq .L08032BE2
    b .L08032E1C
.L08032BE2:
    ldr r2, [sp, #0x2c]
    ldr r3, .L08032C30 @ =0x000005DC
    adds r7, r2, r3
    adds r0, r7, #0
    bl GetStoredBushelCount__C4Barn
    cmp r0, #0
    bne .L08032BF4
    b .L08032E1C
.L08032BF4:
    movs r6, #0
    str r6, [sp, #0x38]
    adds r0, r7, #0
    bl GetPregnancyStallCapacity__C4Barn
    mov sb, r0
    ldr r0, [sp, #0x34]
    cmp r0, sb
    bhs .L08032CA6
    add r1, sp, #0x14
    mov r8, r1
.L08032C0A:
    adds r0, r7, #0
    adds r1, r6, #0
    bl GetPregnancyStallLinkedStall__C4BarnUi
    mvns r0, r0
    lsrs r4, r0, #0x1f
    adds r0, r7, #0
    adds r1, r6, #0
    bl HasBushelForPregnancyStall__C4BarnUi
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08032C34
    cmp r4, #0
    bne .L08032CA0
    ldr r2, [sp, #0x38]
    adds r2, #1
    str r2, [sp, #0x38]
    b .L08032CA0
    .align 2, 0
.L08032C30: .4byte 0x000005DC
.L08032C34:
    cmp r4, #0
    beq .L08032CA0
    add r0, sp, #0x10
    adds r1, r7, #0
    adds r2, r6, #0
    bl method_0800D0C0__C4BarnUi
    ldr r0, [sp, #0x10]
    str r0, [sp, #0x14]
    mov r3, r8
    movs r0, #0
    ldrsh r1, [r3, r0]
    ldr r2, [sp, #0x68]
    subs r1, r1, r2
    movs r2, #2
    ldrsh r0, [r3, r2]
    ldr r3, [sp, #0x6c]
    subs r0, r0, r3
    adds r2, r1, #0
    muls r2, r1, r2
    adds r1, r2, #0
    adds r3, r0, #0
    muls r3, r0, r3
    adds r0, r3, #0
    adds r5, r1, r0
    mov r0, sp
    ldrb r0, [r0]
    lsls r0, r0, #0x1c
    cmp r0, #0
    beq .L08032C76
    ldr r0, [sp, #0x34]
    cmp r5, r0
    bhs .L08032CA0
.L08032C76:
    add r4, sp, #8
    ldrb r1, [r4]
    movs r0, #0x10
    rsbs r0, r0, #0
    ands r0, r1
    movs r1, #5
    orrs r0, r1
    strb r0, [r4]
    strb r6, [r4, #1]
    ldr r0, [sp, #0x70]
    adds r1, r4, #0
    bl HasNpcEventSlot
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08032CA0
    mov r1, sp
    adds r0, r4, #0
    ldm r0!, {r2, r3}
    stm r1!, {r2, r3}
    str r5, [sp, #0x34]
.L08032CA0:
    adds r6, #1
    cmp r6, sb
    blo .L08032C0A
.L08032CA6:
    movs r6, #0
    mov sb, r6
    ldr r1, [sp, #0x2c]
    ldr r2, .L08032D30 @ =0x000005DC
    adds r0, r1, r2
    bl GetCapacity__C4Barn
    adds r6, r0, #0
    movs r5, #0
    cmp sb, r6
    bhs .L08032D06
.L08032CBC:
    adds r0, r7, #0
    adds r1, r5, #0
    bl GetBarnAnimal__C4BarnUi
    adds r4, r0, #0
    cmp r4, #0
    beq .L08032D00
    mov r3, sl
    ldr r0, [r3]
    adds r1, r5, #0
    adds r1, #0x36
    ldr r2, [r0, #0x44]
    mov r0, sl
    bl _call_via_r2
    cmp r0, #0
    beq .L08032D00
    ldrh r0, [r0, #4]
    cmp r0, #0x25
    bne .L08032D00
    adds r0, r4, #0
    bl HasBeenFed__C9Livestock
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08032D00
    adds r0, r4, #0
    bl IsPregnant__C10BarnAnimal
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08032D00
    movs r0, #1
    add sb, r0
.L08032D00:
    adds r5, #1
    cmp r5, r6
    blo .L08032CBC
.L08032D06:
    mov r1, sb
    cmp r1, #0
    bne .L08032D0E
    b .L08032E1C
.L08032D0E:
    str r6, [sp, #0x3c]
    movs r2, #0
    mov r8, r2
    movs r4, #0
    cmp r8, r6
    bhs .L08032D4E
.L08032D1A:
    adds r0, r7, #0
    adds r1, r4, #0
    bl HasBushelForStall__C4BarnUi
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08032D34
    ldr r3, [sp, #0x38]
    adds r3, #1
    str r3, [sp, #0x38]
    b .L08032D46
    .align 2, 0
.L08032D30: .4byte 0x000005DC
.L08032D34:
    cmp r4, #0xf
    bhi .L08032D46
    movs r1, #0x1f
    ands r1, r4
    movs r0, #1
    lsls r0, r1
    mov r6, r8
    orrs r6, r0
    mov r8, r6
.L08032D46:
    adds r4, #1
    ldr r0, [sp, #0x3c]
    cmp r4, r0
    blo .L08032D1A
.L08032D4E:
    ldr r1, [sp, #0x38]
    cmp sb, r1
    bls .L08032E1C
    movs r3, #0
    movs r4, #0x1f
    movs r5, #1
    ldr r2, [sp, #0x70]
.L08032D5C:
    ldrb r0, [r2]
    lsls r0, r0, #0x1c
    lsrs r0, r0, #0x1c
    cmp r0, #4
    bne .L08032D7E
    ldr r6, [sp, #0x38]
    adds r6, #1
    str r6, [sp, #0x38]
    ldrb r1, [r2, #1]
    cmp r1, #0xf
    bhi .L08032D7E
    ands r1, r4
    adds r0, r5, #0
    lsls r0, r1
    mov r1, r8
    bics r1, r0
    mov r8, r1
.L08032D7E:
    adds r2, #8
    adds r3, #1
    cmp r3, #6
    bls .L08032D5C
    ldr r2, [sp, #0x38]
    cmp sb, r2
    bls .L08032E1C
    movs r4, #0
    ldr r3, [sp, #0x3c]
    cmp r4, r3
    bhs .L08032E1C
    mov r6, sp
    adds r6, #0x1c
    str r6, [sp, #0x44]
    add r5, sp, #8
    movs r0, #0x10
    rsbs r0, r0, #0
    mov sb, r0
.L08032DA2:
    movs r0, #0
    cmp r4, #0xf
    bhi .L08032DBA
    movs r0, #0x1f
    ands r0, r4
    movs r1, #1
    lsls r1, r0
    mov r2, r8
    ands r1, r2
    rsbs r0, r1, #0
    orrs r0, r1
    lsrs r0, r0, #0x1f
.L08032DBA:
    cmp r0, #0
    beq .L08032E14
    add r0, sp, #0x18
    adds r1, r7, #0
    adds r2, r4, #0
    bl method_0800D074__C4BarnUi
    ldr r0, [sp, #0x18]
    str r0, [sp, #0x1c]
    ldr r3, [sp, #0x44]
    movs r6, #0
    ldrsh r1, [r3, r6]
    ldr r0, [sp, #0x68]
    subs r1, r1, r0
    movs r2, #2
    ldrsh r0, [r3, r2]
    ldr r3, [sp, #0x6c]
    subs r0, r0, r3
    adds r6, r1, #0
    muls r6, r1, r6
    adds r1, r6, #0
    adds r2, r0, #0
    muls r2, r0, r2
    adds r0, r2, #0
    adds r2, r1, r0
    mov r0, sp
    ldrb r0, [r0]
    lsls r0, r0, #0x1c
    cmp r0, #0
    beq .L08032DFC
    ldr r3, [sp, #0x34]
    cmp r2, r3
    bhs .L08032E14
.L08032DFC:
    ldrb r1, [r5]
    mov r0, sb
    ands r0, r1
    movs r1, #4
    orrs r0, r1
    strb r0, [r5]
    strb r4, [r5, #1]
    mov r1, sp
    adds r0, r5, #0
    ldm r0!, {r3, r6}
    stm r1!, {r3, r6}
    str r2, [sp, #0x34]
.L08032E14:
    adds r4, #1
    ldr r6, [sp, #0x3c]
    cmp r4, r6
    blo .L08032DA2
.L08032E1C:
    ldr r0, [sp, #0x30]
    cmp r0, #0x11
    beq .L08032E24
    b .L08032F8C
.L08032E24:
    ldr r1, [sp, #0x2c]
    movs r2, #0xff
    lsls r2, r2, #2
    adds r7, r1, r2
    adds r0, r7, #0
    bl GetStoredBushelCount__C4Coop
    cmp r0, #0
    bne .L08032E38
    b .L08032F8C
.L08032E38:
    adds r0, r7, #0
    bl GetCapacity__C4Coop
    adds r6, r0, #0
    movs r3, #0
    mov sb, r3
    movs r5, #0
    cmp sb, r6
    bhs .L08032E88
.L08032E4A:
    adds r0, r7, #0
    adds r1, r5, #0
    bl GetChicken__C4CoopUi
    adds r4, r0, #0
    cmp r4, #0
    beq .L08032E82
    mov r1, sl
    ldr r0, [r1]
    adds r1, r5, #0
    adds r1, #0x2e
    ldr r2, [r0, #0x44]
    mov r0, sl
    bl _call_via_r2
    cmp r0, #0
    beq .L08032E82
    ldrh r0, [r0, #4]
    cmp r0, #0x11
    bne .L08032E82
    adds r0, r4, #0
    bl HasBeenFed__C9Livestock
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08032E82
    movs r2, #1
    add sb, r2
.L08032E82:
    adds r5, #1
    cmp r5, r6
    blo .L08032E4A
.L08032E88:
    mov r3, sb
    cmp r3, #0
    beq .L08032F8C
    str r6, [sp, #0x40]
    movs r5, #0
    mov r8, r5
    movs r4, #0
    cmp r8, r6
    bhs .L08032EC6
.L08032E9A:
    adds r0, r7, #0
    adds r1, r4, #0
    bl HasBushelForEnt__C4CoopUi
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08032EAC
    adds r5, #1
    b .L08032EBE
.L08032EAC:
    cmp r4, #7
    bhi .L08032EBE
    movs r1, #0x1f
    ands r1, r4
    movs r0, #1
    lsls r0, r1
    mov r6, r8
    orrs r6, r0
    mov r8, r6
.L08032EBE:
    adds r4, #1
    ldr r0, [sp, #0x40]
    cmp r4, r0
    blo .L08032E9A
.L08032EC6:
    cmp sb, r5
    bls .L08032F8C
    movs r3, #0
    movs r4, #0x1f
    movs r6, #1
    ldr r2, [sp, #0x70]
.L08032ED2:
    ldrb r0, [r2]
    lsls r0, r0, #0x1c
    lsrs r0, r0, #0x1c
    cmp r0, #3
    bne .L08032EF0
    adds r5, #1
    ldrb r1, [r2, #1]
    cmp r1, #7
    bhi .L08032EF0
    ands r1, r4
    adds r0, r6, #0
    lsls r0, r1
    mov r1, r8
    bics r1, r0
    mov r8, r1
.L08032EF0:
    adds r2, #8
    adds r3, #1
    cmp r3, #6
    bls .L08032ED2
    cmp sb, r5
    bls .L08032F8C
    movs r4, #0
    ldr r2, [sp, #0x40]
    cmp r4, r2
    bhs .L08032F8C
    mov r3, sp
    adds r3, #0x24
    str r3, [sp, #0x44]
    add r5, sp, #8
    movs r6, #0x10
    rsbs r6, r6, #0
    mov sb, r6
.L08032F12:
    movs r0, #0
    cmp r4, #7
    bhi .L08032F2A
    movs r0, #0x1f
    ands r0, r4
    movs r1, #1
    lsls r1, r0
    mov r0, r8
    ands r1, r0
    rsbs r0, r1, #0
    orrs r0, r1
    lsrs r0, r0, #0x1f
.L08032F2A:
    cmp r0, #0
    beq .L08032F84
    add r0, sp, #0x20
    adds r1, r7, #0
    adds r2, r4, #0
    bl method_0800C6F0__C4CoopUi
    ldr r0, [sp, #0x20]
    str r0, [sp, #0x24]
    ldr r2, [sp, #0x44]
    movs r3, #0
    ldrsh r1, [r2, r3]
    ldr r6, [sp, #0x68]
    subs r1, r1, r6
    movs r3, #2
    ldrsh r0, [r2, r3]
    ldr r6, [sp, #0x6c]
    subs r0, r0, r6
    adds r2, r1, #0
    muls r2, r1, r2
    adds r1, r2, #0
    adds r3, r0, #0
    muls r3, r0, r3
    adds r0, r3, #0
    adds r2, r1, r0
    mov r0, sp
    ldrb r0, [r0]
    lsls r0, r0, #0x1c
    cmp r0, #0
    beq .L08032F6C
    ldr r6, [sp, #0x34]
    cmp r2, r6
    bhs .L08032F84
.L08032F6C:
    ldrb r1, [r5]
    mov r0, sb
    ands r0, r1
    movs r1, #3
    orrs r0, r1
    strb r0, [r5]
    strb r4, [r5, #1]
    mov r1, sp
    adds r0, r5, #0
    ldm r0!, {r3, r6}
    stm r1!, {r3, r6}
    str r2, [sp, #0x34]
.L08032F84:
    adds r4, #1
    ldr r0, [sp, #0x40]
    cmp r4, r0
    blo .L08032F12
.L08032F8C:
    movs r6, #0x36
    movs r7, #0x10
    rsbs r7, r7, #0
.L08032F92:
    mov r1, sl
    ldr r0, [r1]
    ldr r2, [r0, #0x44]
    mov r0, sl
    adds r1, r6, #0
    bl _call_via_r2
    adds r4, r0, #0
    cmp r4, #0
    beq .L0803303A
    ldrh r0, [r4, #4]
    ldr r2, [sp, #0x30]
    cmp r0, r2
    bne .L0803303A
    ldr r0, [r4, #0x14]
    ldr r1, [r0, #0x50]
    adds r0, r4, #0
    bl _call_via_r1
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08032FDE
    ldr r0, [r4, #0x14]
    ldr r1, [r0, #0x54]
    adds r0, r4, #0
    bl _call_via_r1
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08032FDE
    ldr r0, [r4, #0x14]
    ldr r1, [r0, #0x58]
    adds r0, r4, #0
    bl _call_via_r1
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0803303A
.L08032FDE:
    movs r3, #0xa
    ldrsh r1, [r4, r3]
    ldr r0, [sp, #0x68]
    subs r1, r1, r0
    movs r2, #0xe
    ldrsh r0, [r4, r2]
    ldr r3, [sp, #0x6c]
    subs r0, r0, r3
    adds r2, r1, #0
    muls r2, r1, r2
    adds r1, r2, #0
    adds r3, r0, #0
    muls r3, r0, r3
    adds r0, r3, #0
    adds r1, r1, r0
    adds r5, r1, #0
    mov r0, sp
    ldrb r0, [r0]
    lsls r0, r0, #0x1c
    cmp r0, #0
    beq .L0803300E
    ldr r0, [sp, #0x34]
    cmp r1, r0
    bhs .L0803303A
.L0803300E:
    add r4, sp, #8
    ldrb r1, [r4]
    adds r0, r7, #0
    ands r0, r1
    movs r1, #6
    orrs r0, r1
    strb r0, [r4]
    adds r0, r6, #0
    subs r0, #0x36
    strb r0, [r4, #1]
    ldr r0, [sp, #0x70]
    adds r1, r4, #0
    bl HasNpcEventSlot
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0803303A
    mov r1, sp
    adds r0, r4, #0
    ldm r0!, {r2, r3}
    stm r1!, {r2, r3}
    str r5, [sp, #0x34]
.L0803303A:
    adds r6, #1
    cmp r6, #0x45
    bls .L08032F92
    movs r4, #0x2e
.L08033042:
    mov r6, sl
    ldr r0, [r6]
    ldr r2, [r0, #0x44]
    mov r0, sl
    adds r1, r4, #0
    bl _call_via_r2
    adds r4, #1
    cmp r4, #0x35
    bls .L08033042
    movs r5, #0x4c
    movs r7, #0x10
    rsbs r7, r7, #0
.L0803305C:
    mov r1, sl
    ldr r0, [r1]
    ldr r2, [r0, #0x44]
    mov r0, sl
    adds r1, r5, #0
    bl _call_via_r2
    adds r2, r0, #0
    cmp r2, #0
    beq .L080330D4
    ldrh r0, [r2, #4]
    ldr r3, [sp, #0x30]
    cmp r0, r3
    bne .L080330D4
    movs r6, #0xa
    ldrsh r1, [r2, r6]
    ldr r0, [sp, #0x68]
    subs r1, r1, r0
    movs r3, #0xe
    ldrsh r0, [r2, r3]
    ldr r6, [sp, #0x6c]
    subs r0, r0, r6
    adds r2, r1, #0
    muls r2, r1, r2
    adds r1, r2, #0
    adds r3, r0, #0
    muls r3, r0, r3
    adds r0, r3, #0
    adds r1, r1, r0
    adds r6, r1, #0
    mov r0, sp
    ldrb r0, [r0]
    lsls r0, r0, #0x1c
    cmp r0, #0
    beq .L080330A8
    ldr r0, [sp, #0x34]
    cmp r1, r0
    bhs .L080330D4
.L080330A8:
    add r4, sp, #8
    ldrb r1, [r4]
    adds r0, r7, #0
    ands r0, r1
    movs r1, #8
    orrs r0, r1
    strb r0, [r4]
    adds r0, r5, #0
    subs r0, #0x4c
    strb r0, [r4, #1]
    ldr r0, [sp, #0x70]
    adds r1, r4, #0
    bl HasNpcEventSlot
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L080330D4
    mov r1, sp
    adds r0, r4, #0
    ldm r0!, {r2, r3}
    stm r1!, {r2, r3}
    str r6, [sp, #0x34]
.L080330D4:
    adds r5, #1
    cmp r5, #0x53
    bls .L0803305C
    ldr r1, [sp, #0x28]
    mov r0, sp
    ldm r0!, {r2, r6}
    stm r1!, {r2, r6}
    ldr r0, [sp, #0x28]
    add sp, #0x48
    pop {r3, r4, r5}
    mov r8, r3
    mov sb, r4
    mov sl, r5
    pop {r4, r5, r6, r7}
    pop {r1}
    bx r1

    thumb_func_start func_080330F4
func_080330F4: @ 0x080330F4
    push {r4, r5, r6, r7, lr}
    mov r7, sl
    mov r6, sb
    mov r5, r8
    push {r5, r6, r7}
    sub sp, #0x6c
    str r0, [sp, #0x44]
    adds r5, r1, #0
    adds r7, r2, #0
    adds r4, r3, #0
    ldr r3, [sp, #0x8c]
    add r2, sp, #0xc
    ldrb r1, [r2]
    movs r6, #0x10
    rsbs r6, r6, #0
    adds r0, r6, #0
    ands r0, r1
    strb r0, [r2]
    ldrb r1, [r4, #1]
    lsrs r1, r1, #2
    ldrh r0, [r4, #2]
    ldr r2, .L08033150 @ =0x000003FF
    ands r0, r2
    lsls r0, r0, #6
    orrs r0, r1
    lsls r0, r0, #0x10
    asrs r0, r0, #0x10
    str r0, [sp, #0x48]
    ldrb r1, [r4, #3]
    lsrs r1, r1, #2
    ldrh r0, [r4, #4]
    ands r0, r2
    lsls r0, r0, #6
    orrs r0, r1
    lsls r0, r0, #0x10
    asrs r0, r0, #0x10
    mov sl, r0
    cmp r3, #1
    bne .L08033144
    b .L08033286
.L08033144:
    cmp r3, #1
    bgt .L08033154
    cmp r3, #0
    beq .L0803315C
    b .L0803356A
    .align 2, 0
.L08033150: .4byte 0x000003FF
.L08033154:
    cmp r3, #2
    bne .L0803315A
    b .L0803333C
.L0803315A:
    b .L0803356A
.L0803315C:
    movs r0, #0
    str r0, [sp, #0x4c]
    mov sb, r0
    ldr r1, .L080331F4 @ =0x000009C8
    adds r1, r5, r1
    str r1, [sp, #0x50]
    mov r2, sp
    adds r2, #0x20
    str r2, [sp, #0x54]
.L0803316E:
    movs r7, #0
    mov r3, sb
    lsls r3, r3, #1
    str r3, [sp, #0x64]
    mov r4, sb
    adds r4, #1
    str r4, [sp, #0x60]
    ldr r6, [sp, #0x48]
    rsbs r0, r6, #0
    movs r1, #0x8c
    lsls r1, r1, #1
    adds r6, r0, r1
.L08033186:
    ldr r0, [sp, #0x64]
    adds r0, #0x17
    lsls r0, r0, #3
    mov r2, sl
    subs r0, r0, r2
    adds r1, r6, #0
    muls r1, r6, r1
    adds r3, r0, #0
    muls r3, r0, r3
    adds r0, r3, #0
    adds r1, r1, r0
    mov r8, r1
    add r0, sp, #0xc
    ldrb r0, [r0]
    lsls r0, r0, #0x1c
    cmp r0, #0
    beq .L080331AE
    ldr r4, [sp, #0x4c]
    cmp r8, r4
    bhs .L08033272
.L080331AE:
    movs r0, #0x2b
    mov r1, sb
    muls r1, r0, r1
    adds r0, r1, #0
    adds r0, r7, r0
    lsls r0, r0, #2
    ldr r2, [sp, #0x50]
    adds r0, r2, r0
    ldr r1, [r0]
    lsls r0, r1, #0x14
    lsrs r0, r0, #0x1c
    cmp r0, #5
    bne .L080331EE
    str r1, [sp, #0x1c]
    add r0, sp, #0x14
    add r1, sp, #0x1c
    bl method_0800A4A4__9FieldPlot
    add r5, sp, #0x14
    adds r0, r5, #0
    bl IsEmpty__C12RucksackItem
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L080331EE
    adds r0, r5, #0
    bl GetKind__C12RucksackItem
    cmp r0, #0
    beq .L080331F8
    cmp r0, #1
    beq .L08033220
.L080331EE:
    movs r1, #0
    b .L0803323E
    .align 2, 0
.L080331F4: .4byte 0x000009C8
.L080331F8:
    add r0, sp, #0x18
    adds r1, r5, #0
    bl GetFood__C12RucksackItem
    add r0, sp, #0x18
    ldrb r1, [r0]
    adds r0, #1
    ldrb r0, [r0]
    lsls r0, r0, #8
    orrs r0, r1
    mov r1, sp
    adds r1, #0x1a
    ldrb r1, [r1]
    lsls r1, r1, #0x10
    orrs r1, r0
    ldr r0, [sp, #0x54]
    bl __7ProductG4Food
    ldr r0, [sp, #0x54]
    b .L08033234
.L08033220:
    mov r4, sp
    adds r4, #0x21
    adds r0, r5, #0
    bl GetArticle__C12RucksackItem
    adds r1, r0, #0
    adds r0, r4, #0
    bl __7ProductG7Article
    adds r0, r4, #0
.L08033234:
    bl GetPrice__C7Product
    rsbs r1, r0, #0
    orrs r1, r0
    lsrs r1, r1, #0x1f
.L0803323E:
    cmp r1, #0
    beq .L08033272
    add r4, sp, #0x14
    ldrb r1, [r4]
    movs r0, #0x10
    rsbs r0, r0, #0
    ands r0, r1
    movs r1, #2
    orrs r0, r1
    strb r0, [r4]
    strb r7, [r4, #1]
    mov r3, sb
    strb r3, [r4, #2]
    ldr r0, [sp, #0x90]
    adds r1, r4, #0
    bl HasNpcEventSlot
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08033272
    add r1, sp, #0xc
    adds r0, r4, #0
    ldm r0!, {r2, r4}
    stm r1!, {r2, r4}
    mov r3, r8
    str r3, [sp, #0x4c]
.L08033272:
    adds r6, #0x10
    adds r7, #1
    cmp r7, #0x2a
    bls .L08033186
    ldr r4, [sp, #0x60]
    mov sb, r4
    cmp r4, #0x18
    bhi .L08033284
    b .L0803316E
.L08033284:
    b .L0803356A
.L08033286:
    movs r0, #0
    str r0, [sp, #0x58]
    movs r7, #0
    ldr r1, .L08033338 @ =0x000009C8
    adds r1, r1, r5
    mov sb, r1
.L08033292:
    movs r6, #0
    adds r2, r7, #1
    mov r8, r2
    ldr r3, [sp, #0x48]
    rsbs r0, r3, #0
    movs r4, #0x8c
    lsls r4, r4, #1
    adds r5, r0, r4
.L080332A2:
    movs r0, #0x2b
    muls r0, r7, r0
    adds r0, r6, r0
    lsls r0, r0, #2
    add r0, sb
    ldr r1, [r0]
    lsls r0, r1, #0x14
    lsrs r2, r0, #0x1c
    movs r3, #0
    lsls r0, r1, #0x1e
    lsrs r0, r0, #0x1e
    cmp r0, #1
    bne .L080332CE
    subs r0, r2, #1
    cmp r0, #3
    bhi .L080332CE
    lsls r0, r1, #0x18
    lsrs r0, r0, #0x1a
    movs r1, #0x14
    eors r0, r1
    rsbs r0, r0, #0
    lsrs r3, r0, #0x1f
.L080332CE:
    cmp r3, #0
    beq .L08033328
    lsls r0, r7, #4
    adds r0, #0xb8
    mov r1, sl
    subs r0, r0, r1
    adds r1, r5, #0
    muls r1, r5, r1
    adds r2, r0, #0
    muls r2, r0, r2
    adds r0, r2, #0
    adds r1, r1, r0
    adds r2, r1, #0
    add r0, sp, #0xc
    ldrb r0, [r0]
    lsls r0, r0, #0x1c
    cmp r0, #0
    beq .L080332F8
    ldr r3, [sp, #0x58]
    cmp r1, r3
    bhs .L08033328
.L080332F8:
    add r4, sp, #0x14
    ldrb r1, [r4]
    movs r0, #0x10
    rsbs r0, r0, #0
    ands r0, r1
    movs r1, #1
    orrs r0, r1
    strb r0, [r4]
    strb r6, [r4, #1]
    strb r7, [r4, #2]
    ldr r0, [sp, #0x90]
    adds r1, r4, #0
    str r2, [sp, #0x68]
    bl HasNpcEventSlot
    lsls r0, r0, #0x18
    ldr r2, [sp, #0x68]
    cmp r0, #0
    bne .L08033328
    add r1, sp, #0xc
    adds r0, r4, #0
    ldm r0!, {r3, r4}
    stm r1!, {r3, r4}
    str r2, [sp, #0x58]
.L08033328:
    adds r5, #0x10
    adds r6, #1
    cmp r6, #0x2a
    bls .L080332A2
    mov r7, r8
    cmp r7, #0x18
    bls .L08033292
    b .L0803356A
    .align 2, 0
.L08033338: .4byte 0x000009C8
.L0803333C:
    add r0, sp, #0x24
    bl smethod_08009ADC__4Farm
    ldr r0, [sp, #0x24]
    str r0, [sp, #0x28]
    add r6, sp, #0x28
    mov r8, r6
    add r0, sp, #0x2c
    bl smethod_08009AE8__4Farm
    ldr r0, [sp, #0x2c]
    str r0, [sp, #0x30]
    add r6, sp, #0x30
    add r0, sp, #0x34
    bl method_0800CE58__4Barn
    ldr r0, [sp, #0x34]
    str r0, [sp, #0x38]
    mov r0, sp
    adds r0, #0x38
    str r0, [sp, #0x5c]
    add r0, sp, #0x3c
    bl method_0800C554__4Coop
    ldr r0, [sp, #0x3c]
    str r0, [sp, #0x40]
    add r1, sp, #0x40
    mov sb, r1
    ldrh r0, [r4]
    lsls r0, r0, #0x16
    lsrs r0, r0, #0x16
    cmp r0, #0x11
    bne .L08033380
    b .L080334EA
.L08033380:
    cmp r0, #0x11
    bgt .L0803338A
    cmp r0, #2
    beq .L08033390
    b .L0803356A
.L0803338A:
    cmp r0, #0x25
    beq .L08033466
    b .L0803356A
.L08033390:
    ldr r2, [sp, #0x48]
    str r2, [sp]
    mov r3, sl
    str r3, [sp, #4]
    ldr r4, [sp, #0x90]
    str r4, [sp, #8]
    add r4, sp, #0x14
    adds r0, r4, #0
    adds r1, r5, #0
    adds r2, r7, #0
    movs r3, #2
    bl func_08032BB4
    add r1, sp, #0xc
    adds r0, r4, #0
    ldm r0!, {r2, r3}
    stm r1!, {r2, r3}
    add r0, sp, #0xc
    ldrb r0, [r0]
    lsls r0, r0, #0x1c
    cmp r0, #0
    beq .L080333BE
    b .L0803356A
.L080333BE:
    mov r1, r8
    movs r2, #0
    ldrsh r0, [r1, r2]
    ldr r1, [sp, #0x48]
    subs r3, r0, r1
    mov r2, r8
    movs r1, #2
    ldrsh r0, [r2, r1]
    mov r2, sl
    subs r1, r0, r2
    adds r2, r3, #0
    muls r2, r3, r2
    adds r0, r1, #0
    muls r0, r1, r0
    adds r2, r2, r0
    movs r3, #0
    ldrsh r0, [r6, r3]
    ldr r1, [sp, #0x48]
    subs r3, r0, r1
    movs r1, #2
    ldrsh r0, [r6, r1]
    mov r6, sl
    subs r1, r0, r6
    adds r0, r3, #0
    muls r0, r3, r0
    adds r3, r1, #0
    muls r3, r1, r3
    adds r1, r3, #0
    adds r0, r0, r1
    cmp r2, r0
    bhi .L08033418
    ldr r6, [sp, #0x5c]
    movs r1, #0
    ldrsh r0, [r6, r1]
    str r0, [sp]
    movs r2, #2
    ldrsh r0, [r6, r2]
    str r0, [sp, #4]
    ldr r3, [sp, #0x90]
    str r3, [sp, #8]
    adds r0, r4, #0
    adds r1, r5, #0
    adds r2, r7, #0
    movs r3, #0x25
    b .L080334AC
.L08033418:
    mov r6, sb
    movs r1, #0
    ldrsh r0, [r6, r1]
    str r0, [sp]
    movs r2, #2
    ldrsh r0, [r6, r2]
    str r0, [sp, #4]
    ldr r3, [sp, #0x90]
    str r3, [sp, #8]
    adds r0, r4, #0
    adds r1, r5, #0
    adds r2, r7, #0
    movs r3, #0x11
    bl func_08032BB4
    add r1, sp, #0xc
    adds r0, r4, #0
    ldm r0!, {r2, r6}
    stm r1!, {r2, r6}
    add r0, sp, #0xc
    ldrb r0, [r0]
    lsls r0, r0, #0x1c
    cmp r0, #0
    beq .L0803344A
    b .L0803356A
.L0803344A:
    ldr r3, [sp, #0x5c]
    movs r6, #0
    ldrsh r0, [r3, r6]
    str r0, [sp]
    movs r1, #2
    ldrsh r0, [r3, r1]
    str r0, [sp, #4]
    ldr r2, [sp, #0x90]
    str r2, [sp, #8]
    adds r0, r4, #0
    adds r1, r5, #0
    adds r2, r7, #0
    movs r3, #0x25
    b .L080334DC
.L08033466:
    ldr r6, [sp, #0x48]
    str r6, [sp]
    mov r0, sl
    str r0, [sp, #4]
    ldr r1, [sp, #0x90]
    str r1, [sp, #8]
    add r4, sp, #0x14
    adds r0, r4, #0
    adds r1, r5, #0
    adds r2, r7, #0
    movs r3, #0x25
    bl func_08032BB4
    add r1, sp, #0xc
    adds r0, r4, #0
    ldm r0!, {r2, r3}
    stm r1!, {r2, r3}
    add r0, sp, #0xc
    ldrb r0, [r0]
    lsls r0, r0, #0x1c
    cmp r0, #0
    bne .L0803356A
    mov r6, r8
    movs r1, #0
    ldrsh r0, [r6, r1]
    str r0, [sp]
    movs r2, #2
    ldrsh r0, [r6, r2]
    str r0, [sp, #4]
    ldr r3, [sp, #0x90]
    str r3, [sp, #8]
    adds r0, r4, #0
    adds r1, r5, #0
    adds r2, r7, #0
    movs r3, #2
.L080334AC:
    bl func_08032BB4
    add r1, sp, #0xc
    adds r0, r4, #0
    ldm r0!, {r2, r6}
    stm r1!, {r2, r6}
    add r0, sp, #0xc
    ldrb r0, [r0]
    lsls r0, r0, #0x1c
    cmp r0, #0
    bne .L0803356A
    mov r3, sb
    movs r6, #0
    ldrsh r0, [r3, r6]
    str r0, [sp]
    movs r1, #2
    ldrsh r0, [r3, r1]
    str r0, [sp, #4]
    ldr r2, [sp, #0x90]
    str r2, [sp, #8]
    adds r0, r4, #0
    adds r1, r5, #0
    adds r2, r7, #0
    movs r3, #0x11
.L080334DC:
    bl func_08032BB4
    add r1, sp, #0xc
    adds r0, r4, #0
    ldm r0!, {r3, r4}
    stm r1!, {r3, r4}
    b .L0803356A
.L080334EA:
    ldr r0, [sp, #0x48]
    str r0, [sp]
    mov r1, sl
    str r1, [sp, #4]
    ldr r2, [sp, #0x90]
    str r2, [sp, #8]
    add r4, sp, #0x14
    adds r0, r4, #0
    adds r1, r5, #0
    adds r2, r7, #0
    movs r3, #0x11
    bl func_08032BB4
    add r1, sp, #0xc
    adds r0, r4, #0
    ldm r0!, {r2, r3}
    stm r1!, {r2, r3}
    add r0, sp, #0xc
    ldrb r0, [r0]
    lsls r0, r0, #0x1c
    cmp r0, #0
    bne .L0803356A
    movs r3, #0
    ldrsh r0, [r6, r3]
    str r0, [sp]
    movs r1, #2
    ldrsh r0, [r6, r1]
    str r0, [sp, #4]
    ldr r2, [sp, #0x90]
    str r2, [sp, #8]
    adds r0, r4, #0
    adds r1, r5, #0
    adds r2, r7, #0
    movs r3, #2
    bl func_08032BB4
    add r1, sp, #0xc
    adds r0, r4, #0
    ldm r0!, {r3, r6}
    stm r1!, {r3, r6}
    add r0, sp, #0xc
    ldrb r0, [r0]
    lsls r0, r0, #0x1c
    cmp r0, #0
    bne .L0803356A
    ldr r1, [sp, #0x5c]
    movs r2, #0
    ldrsh r0, [r1, r2]
    str r0, [sp]
    movs r3, #2
    ldrsh r0, [r1, r3]
    str r0, [sp, #4]
    ldr r6, [sp, #0x90]
    str r6, [sp, #8]
    adds r0, r4, #0
    adds r1, r5, #0
    adds r2, r7, #0
    movs r3, #0x25
    bl func_08032BB4
    add r1, sp, #0xc
    adds r0, r4, #0
    ldm r0!, {r2, r3}
    stm r1!, {r2, r3}
.L0803356A:
    ldr r1, [sp, #0x44]
    add r0, sp, #0xc
    ldm r0!, {r4, r6}
    stm r1!, {r4, r6}
    ldr r0, [sp, #0x44]
    add sp, #0x6c
    pop {r3, r4, r5}
    mov r8, r3
    mov sb, r4
    mov sl, r5
    pop {r4, r5, r6, r7}
    pop {r1}
    bx r1

    thumb_func_start func_08033584
func_08033584: @ 0x08033584
    push {r4, r5, r6, lr}
    sub sp, #0x10
    adds r4, r0, #0
    adds r5, r1, #0
    adds r3, r2, #0
    ldrb r0, [r4]
    lsls r0, r0, #0x1c
    lsrs r0, r0, #0x1c
    cmp r0, #8
    bhi .L08033648
    lsls r0, r0, #2
    ldr r1, .L080335A4 @ =.L080335A8
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L080335A4: .4byte .L080335A8
.L080335A8: @ jump table
    .4byte .L08033648 @ case 0
    .4byte .L080335CC @ case 1
    .4byte .L08033608 @ case 2
    .4byte .L080336A4 @ case 3
    .4byte .L080336C0 @ case 4
    .4byte .L080336E0 @ case 5
    .4byte .L08033708 @ case 6
    .4byte .L08033754 @ case 7
    .4byte .L08033764 @ case 8
.L080335CC:
    ldrb r2, [r4, #2]
    ldr r0, .L08033604 @ =0x000009C8
    adds r1, r5, r0
    movs r0, #0x2b
    muls r0, r2, r0
    ldrb r4, [r4, #1]
    adds r0, r0, r4
    lsls r0, r0, #2
    adds r1, r1, r0
    ldr r1, [r1]
    lsls r0, r1, #0x14
    lsrs r2, r0, #0x1c
    movs r3, #0
    lsls r0, r1, #0x1e
    lsrs r0, r0, #0x1e
    cmp r0, #1
    bne .L08033600
    subs r0, r2, #1
    cmp r0, #3
    bhi .L08033600
    lsls r0, r1, #0x18
    lsrs r0, r0, #0x1a
    movs r1, #0x14
    eors r0, r1
    rsbs r0, r0, #0
    lsrs r3, r0, #0x1f
.L08033600:
    adds r0, r3, #0
    b .L0803377A
    .align 2, 0
.L08033604: .4byte 0x000009C8
.L08033608:
    ldrb r2, [r4, #2]
    ldr r0, .L0803364C @ =0x000009C8
    adds r1, r5, r0
    movs r0, #0x2b
    muls r0, r2, r0
    ldrb r4, [r4, #1]
    adds r0, r0, r4
    lsls r0, r0, #2
    adds r1, r1, r0
    ldr r1, [r1]
    lsls r0, r1, #0x14
    lsrs r0, r0, #0x1c
    cmp r0, #5
    bne .L08033648
    str r1, [sp, #8]
    add r1, sp, #8
    mov r0, sp
    bl method_0800A4A4__9FieldPlot
    mov r0, sp
    bl IsEmpty__C12RucksackItem
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08033648
    mov r0, sp
    bl GetKind__C12RucksackItem
    cmp r0, #0
    beq .L08033650
    cmp r0, #1
    beq .L08033684
.L08033648:
    movs r0, #0
    b .L0803377A
    .align 2, 0
.L0803364C: .4byte 0x000009C8
.L08033650:
    add r4, sp, #0xc
    add r0, sp, #4
    mov r1, sp
    bl GetFood__C12RucksackItem
    add r0, sp, #4
    ldrb r1, [r0]
    adds r0, #1
    ldrb r0, [r0]
    lsls r0, r0, #8
    orrs r0, r1
    mov r1, sp
    adds r1, #6
    ldrb r1, [r1]
    lsls r1, r1, #0x10
    orrs r1, r0
    adds r0, r4, #0
    bl __7ProductG4Food
    adds r0, r4, #0
    bl GetPrice__C7Product
    rsbs r1, r0, #0
    orrs r1, r0
    lsrs r0, r1, #0x1f
    b .L0803377A
.L08033684:
    mov r4, sp
    adds r4, #0xd
    mov r0, sp
    bl GetArticle__C12RucksackItem
    adds r1, r0, #0
    adds r0, r4, #0
    bl __7ProductG7Article
    adds r0, r4, #0
    bl GetPrice__C7Product
    rsbs r1, r0, #0
    orrs r1, r0
    lsrs r0, r1, #0x1f
    b .L0803377A
.L080336A4:
    movs r6, #0
    movs r0, #0xff
    lsls r0, r0, #2
    adds r5, r5, r0
    adds r0, r5, #0
    bl GetStoredBushelCount__C4Coop
    cmp r0, #0
    beq .L08033700
    ldrb r1, [r4, #1]
    adds r0, r5, #0
    bl HasBushelForEnt__C4CoopUi
    b .L080336F8
.L080336C0:
    movs r6, #0
    ldr r0, .L080336DC @ =0x000005DC
    adds r5, r5, r0
    adds r0, r5, #0
    bl GetStoredBushelCount__C4Barn
    cmp r0, #0
    beq .L08033700
    ldrb r1, [r4, #1]
    adds r0, r5, #0
    bl HasBushelForStall__C4BarnUi
    b .L080336F8
    .align 2, 0
.L080336DC: .4byte 0x000005DC
.L080336E0:
    movs r6, #0
    ldr r0, .L08033704 @ =0x000005DC
    adds r5, r5, r0
    adds r0, r5, #0
    bl GetStoredBushelCount__C4Barn
    cmp r0, #0
    beq .L08033700
    ldrb r1, [r4, #1]
    adds r0, r5, #0
    bl HasBushelForPregnancyStall__C4BarnUi
.L080336F8:
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08033700
    movs r6, #1
.L08033700:
    adds r0, r6, #0
    b .L0803377A
    .align 2, 0
.L08033704: .4byte 0x000005DC
.L08033708:
    ldrb r1, [r4, #1]
    adds r1, #0x36
    ldr r0, [r3]
    ldr r2, [r0, #0x44]
    adds r0, r3, #0
    bl _call_via_r2
    adds r4, r0, #0
    movs r5, #0
    cmp r4, #0
    beq .L08033750
    ldr r0, [r4, #0x14]
    ldr r1, [r0, #0x50]
    adds r0, r4, #0
    bl _call_via_r1
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0803374E
    ldr r0, [r4, #0x14]
    ldr r1, [r0, #0x54]
    adds r0, r4, #0
    bl _call_via_r1
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0803374E
    ldr r0, [r4, #0x14]
    ldr r1, [r0, #0x58]
    adds r0, r4, #0
    bl _call_via_r1
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08033750
.L0803374E:
    movs r5, #1
.L08033750:
    adds r0, r5, #0
    b .L0803377A
.L08033754:
    ldrb r1, [r4, #1]
    adds r1, #0x2e
    ldr r0, [r3]
    ldr r2, [r0, #0x44]
    adds r0, r3, #0
    bl _call_via_r2
    b .L08033648
.L08033764:
    ldrb r1, [r4, #1]
    adds r1, #0x4c
    ldr r0, [r3]
    ldr r2, [r0, #0x44]
    adds r0, r3, #0
    bl _call_via_r2
    adds r1, r0, #0
    rsbs r0, r1, #0
    orrs r0, r1
    lsrs r0, r0, #0x1f
.L0803377A:
    add sp, #0x10
    pop {r4, r5, r6}
    pop {r1}
    bx r1
    .align 2, 0

    .section .text.npc_side.08033784, "ax", %progbits

    .section .text.npc_side.08033914, "ax", %progbits

    .section .text.npc_side.08033928, "ax", %progbits

    thumb_func_start func_08033928
func_08033928: @ 0x08033928
    push {r4, r5, r6, r7, lr}
    mov r7, r8
    push {r7}
    sub sp, #0x14
    adds r7, r0, #0
    adds r5, r1, #0
    adds r6, r2, #0
    adds r4, r3, #0
    ldr r1, .L080339A4 @ =gUnk_080F149C
    lsls r0, r6, #2
    adds r0, r0, r1
    ldr r2, [r0]
    add r0, sp, #4
    adds r1, r4, #0
    bl GetRosterNpcLocation
    ldr r1, .L080339A8 @ =gUnk_080F14E2
    lsls r0, r6, #1
    adds r0, r0, r1
    ldrh r0, [r0]
    str r0, [sp]
    adds r0, r7, #0
    adds r1, r5, #0
    add r2, sp, #4
    movs r3, #2
    bl __12AActorEntityP10GameObjectRC13ActorLocationUiUi
    ldr r0, .L080339AC @ =vtable_unk_080E68C8
    str r0, [r7, #0x14]
    adds r0, r7, #0
    adds r0, #0x30
    movs r5, #0
    strb r6, [r0]
    adds r0, r4, #0
    adds r1, r6, #0
    bl GetRosterHarvestSpriteByIndexConst
    str r0, [r7, #0x34]
    movs r0, #0x87
    lsls r0, r0, #3
    adds r4, r4, r0
    str r4, [r7, #0x38]
    ldr r0, [sp, #0x2c]
    str r0, [r7, #0x3c]
    adds r0, r7, #0
    adds r0, #0x41
    strb r5, [r0]
    adds r0, #1
    strb r5, [r0]
    movs r0, #0x80
    lsls r0, r0, #8
    mov r8, r0
    ldr r0, [r7, #0x34]
    ldr r0, [r0, #0x1c]
    cmp r0, #5
    bhi .L08033A08
    lsls r0, r0, #2
    ldr r1, .L080339B0 @ =.L080339B4
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L080339A4: .4byte gUnk_080F149C
.L080339A8: .4byte gUnk_080F14E2
.L080339AC: .4byte vtable_unk_080E68C8
.L080339B0: .4byte .L080339B4
.L080339B4: @ jump table
    .4byte .L080339D8 @ case 0
    .4byte .L08033A06 @ case 1
    .4byte .L080339D0 @ case 2
    .4byte .L080339EC @ case 3
    .4byte .L080339CC @ case 4
    .4byte .L08033A06 @ case 5
.L080339CC:
    movs r5, #4
    b .L08033A08
.L080339D0:
    ldr r0, [r7, #0x34]
    adds r0, #0x22
    ldrb r5, [r0]
    b .L08033A08
.L080339D8:
    add r4, sp, #0xc
    adds r0, r4, #0
    adds r1, r6, #0
    bl GetHarvestSpriteHomeLocation
    adds r0, r7, #0
    adds r1, r4, #0
    bl SetLocation__12AActorEntityRC13ActorLocation
    b .L08033A08
.L080339EC:
    movs r5, #1
    ldr r4, [r7, #0x34]
    adds r0, r4, #0
    bl GetCurrentTask__C13HarvestSprite
    adds r1, r0, #0
    adds r0, r4, #0
    bl GetTaskExp__C13HarvestSpriteQ213HarvestSprite4Task
    bl ComputeSpriteWorkSpeed
    mov r8, r0
    b .L08033A08
.L08033A06:
    movs r5, #1
.L08033A08:
    mvns r1, r5
    adds r0, r7, #0
    adds r0, #0x40
    strb r1, [r0]
    subs r0, #0x20
    ldrb r2, [r0]
    adds r0, r7, #0
    adds r1, r5, #0
    mov r3, r8
    bl func_08034180
    adds r0, r7, #0
    add sp, #0x14
    pop {r3}
    mov r8, r3
    pop {r4, r5, r6, r7}
    pop {r1}
    bx r1
    .section .text.npc_side.08033B10, "ax", %progbits

    .section .text.npc_side.08033B24, "ax", %progbits

    .section .text.npc_side.08033B84, "ax", %progbits

    .section .text.npc_side.08033B94, "ax", %progbits

    thumb_func_start UpdateSpriteWorkerState
UpdateSpriteWorkerState: @ 0x08033B94
    push	{r4, r5, r6, r7, lr}
    mov	r7, sl
    mov	r6, r9
    mov	r5, r8
    push	{r5, r6, r7}
    sub	sp, #172
    adds	r7, r0, #0
    adds	r2, r1, #0
    ldr	r0, [r7, #52]
    mov	r8, r0
    ldr	r1, [r7, #0]
    mov	sl, r1
    ldr	r0, [r0, #28]
    cmp	r0, #5
    bhi .L08033BD8
    lsls	r0, r0, #2
    ldr	r1, .L08033BBC
    adds	r0, r0, r1
    ldr	r0, [r0, #0]
    mov	pc, r0
.L08033BBC:
    .4byte .L08033BC0
.L08033BC0:
    .4byte .L08033BD8
    .4byte .L08033C64
    .4byte .L08033C94
    .4byte .L08033DFC
    .4byte .L08034054
    .4byte .L08034078
.L08033BD8:
    ldrb	r0, [r2, #4]
    cmp	r0, #0
    bne .L08033BE0
    b .L08034110
.L08033BE0:
    mov	r0, r8
    bl GetCurrentTask__C13HarvestSprite
    cmp	r0, #3
    bne .L08033BEC
    b .L08034110
.L08033BEC:
    mov	r0, r8
    bl GetWorkDaysLeft__C13HarvestSprite
    cmp	r0, #0
    bne .L08033BF8
    b .L08034110
.L08033BF8:
    mov	r2, sl
    ldr	r0, [r2, #0]
    movs	r3, #162
    lsls	r3, r3, #1
    adds	r0, r0, r3
    ldr	r1, [r0, #0]
    mov	r0, sl
    bl _call_via_r1
    adds	r2, r0, #0
    ldrb	r1, [r2, #10]
    movs	r0, #31
    ands	r0, r1
    cmp	r0, #6
    beq .L08033C18
    b .L08034110
.L08033C18:
    ldrh	r0, [r2, #10]
    lsls	r0, r0, #21
    lsrs	r0, r0, #26
    adds	r1, r7, #0
    adds	r1, #48
    ldrb	r1, [r1, #0]
    adds	r1, #1
    cmp	r0, r1
    beq .L08033C2C
    b .L08034110
.L08033C2C:
    ldr	r0, [r2, #0]
    subs	r0, #3
    cmp	r0, #1
    bls .L08033C5C
    movs	r2, #180
    lsls	r2, r2, #1
    movs	r3, #8
    negs	r3, r3
    adds	r0, r7, #0
    movs	r1, #2
    bl SetLocation__7AEntityUiii
    add	r1, sp, #72
    mov	r0, r8
    bl method_0809E7D0__13HarvestSprite
    movs	r3, #128
    lsls	r3, r3, #8
    adds	r0, r7, #0
    movs	r1, #1
    movs	r2, #0
    bl func_08034180
    b .L08034110
.L08033C5C:
    mov	r0, r8
    bl TaskDayUpdate__13HarvestSprite
    b .L08034110
.L08033C64:
    ldrh	r0, [r7, #4]
    cmp	r0, #2
    beq .L08033C88
    movs	r2, #180
    lsls	r2, r2, #1
    movs	r3, #8
    negs	r3, r3
    adds	r0, r7, #0
    movs	r1, #2
    bl SetLocation__7AEntityUiii
    movs	r3, #128
    lsls	r3, r3, #8
    adds	r0, r7, #0
    movs	r1, #1
    movs	r2, #0
    bl func_08034180
.L08033C88:
    movs	r4, #14
    ldrsh	r0, [r7, r4]
    cmp	r0, #119
    bgt .L08033C92
    b .L08034110
.L08033C92:
    b .L08034068
.L08033C94:
    mov	r5, r8
    adds	r5, #32
    str	r5, [sp, #148]
    mov	r6, r8
    ldrh	r0, [r6, #32]
    cmp	r0, #0
    beq .L08033CA4
    b .L08033DF4
.L08033CA4:
    adds	r0, r7, #0
    bl func_08034940
    lsls	r0, r0, #24
    cmp	r0, #0
    beq .L08033CB2
    b .L08034110
.L08033CB2:
    ldrb	r0, [r5, #3]
    cmp	r0, #0
    beq .L08033CC2
    subs	r0, #1
    strb	r0, [r5, #3]
    lsls	r0, r0, #24
    cmp	r0, #0
    bne .L08033CD0
.L08033CC2:
    adds	r0, r7, #0
    bl TryAssignSpriteWorkerTask
    lsls	r0, r0, #24
    cmp	r0, #0
    beq .L08033CD0
    b .L08034110
.L08033CD0:
    ldr	r5, .L08033D10
    adds	r0, r5, #0
    movs	r1, #5
    bl ChooseWeightedEntityMovement
    lsls	r4, r0, #2
    adds	r4, r4, r5
    movs	r6, #0
    ldr	r1, [sp, #148]
    strb	r0, [r1, #2]
    movs	r5, #60
    ldrb	r0, [r4, #2]
    muls	r0, r5
    bl RollEntityMovementRandom
    ldrb	r1, [r4, #1]
    muls	r1, r5
    adds	r1, r1, r0
    ldr	r2, [sp, #148]
    strh	r1, [r2, #0]
    ldrh	r4, [r7, #4]
    add	r0, sp, #76
    strh	r6, [r0, #0]
    strh	r6, [r0, #2]
    mov	r9, r0
    cmp	r4, #17
    beq .L08033D52
    cmp	r4, #17
    bgt .L08033D14
    cmp	r4, #2
    beq .L08033D1A
    b .L08033D6A
.L08033D10:
    .4byte 0x080F143C
.L08033D14:
    cmp	r4, #37
    beq .L08033D38
    b .L08033D6A
.L08033D1A:
    movs	r0, #72
    bl RollEntityMovementRandom
    adds	r0, #128
    mov	r3, r9
    strh	r0, [r3, #0]
    movs	r0, #72
    bl RollEntityMovementRandom
    movs	r5, #148
    lsls	r5, r5, #1
    adds	r0, r0, r5
    mov	r6, r9
    strh	r0, [r6, #2]
    b .L08033D6A
.L08033D38:
    movs	r0, #104
    bl RollEntityMovementRandom
    adds	r0, #72
    mov	r1, r9
    strh	r0, [r1, #0]
    movs	r0, #48
    bl RollEntityMovementRandom
    adds	r0, #96
    mov	r2, r9
    strh	r0, [r2, #2]
    b .L08033D6A
.L08033D52:
    movs	r0, #64
    bl RollEntityMovementRandom
    adds	r0, #144
    mov	r3, r9
    strh	r0, [r3, #0]
    movs	r0, #48
    bl RollEntityMovementRandom
    adds	r0, #112
    mov	r5, r9
    strh	r0, [r5, #2]
.L08033D6A:
    adds	r0, r7, #0
    adds	r0, #32
    ldrb	r0, [r0, #0]
    mov	r8, r0
    mov	r6, sl
    ldr	r1, [r6, #0]
    add	r0, sp, #8
    ldr	r3, [r1, #52]
    mov	r1, sl
    adds	r2, r4, #0
    bl _call_via_r3
    ldr	r1, [r7, #20]
    add	r0, sp, #80
    ldr	r2, [r1, #12]
    adds	r1, r7, #0
    bl _call_via_r2
    ldr	r0, [sp, #80]
    ldr	r1, [sp, #84]
    str	r0, [sp, #164]
    str	r1, [sp, #168]
    add	r4, sp, #20
    movs	r1, #32
    mov	sl, r1
    movs	r5, #0
    movs	r1, #33
    str	r1, [sp, #20]
    movs	r0, #33
    negs	r0, r0
    str	r0, [r4, #4]
    str	r0, [r4, #8]
    str	r1, [r4, #12]
    add	r1, sp, #40
    add	r0, sp, #8
    ldmia	r0!, {r2, r3, r6}
    stmia	r1!, {r2, r3, r6}
    ldr	r0, [sp, #164]
    ldr	r1, [sp, #168]
    str	r0, [sp, #52]
    str	r1, [sp, #56]
    mov	r1, sl
    str	r1, [r4, #40]
    str	r5, [r4, #44]
    str	r5, [r4, #48]
    movs	r2, #10
    ldrsh	r0, [r7, r2]
    movs	r3, #14
    ldrsh	r1, [r7, r3]
    mov	r5, r9
    movs	r6, #0
    ldrsh	r2, [r5, r6]
    add	r3, sp, #76
    movs	r5, #2
    ldrsh	r3, [r3, r5]
    str	r4, [sp, #0]
    mov	r6, r8
    str	r6, [sp, #4]
    bl ChooseEntityFacingTowardPoint
    adds	r2, r0, #0
    ldr	r0, [sp, #148]
    ldrb	r1, [r0, #2]
    movs	r3, #128
    lsls	r3, r3, #8
    adds	r0, r7, #0
    bl func_08034180
    b .L08034110
.L08033DF4:
    subs	r0, #1
    mov	r1, r8
    strh	r0, [r1, #32]
    b .L08034110
.L08033DFC:
    mov	r2, r8
    adds	r2, #32
    str	r2, [sp, #152]
    adds	r4, r7, #0
    adds	r4, #48
    ldrb	r0, [r4, #0]
    ldr	r1, [r7, #56]
    lsls	r0, r0, #3
    adds	r1, r1, r0
    mov	r9, r1
    ldr	r1, [r7, #60]
    mov	r0, r9
    mov	r2, sl
    bl GetSpriteWorkTargetMap
    adds	r6, r0, #0
    ldrh	r5, [r7, #4]
    movs	r0, #10
    ldrsh	r3, [r7, r0]
    str	r3, [sp, #156]
    movs	r2, #14
    ldrsh	r1, [r7, r2]
    str	r1, [sp, #160]
    add	r0, sp, #88
    strh	r3, [r0, #0]
    strh	r1, [r0, #2]
    cmp	r6, r5
    bne .L08033E46
    ldr	r2, [r7, #60]
    add	r0, sp, #92
    mov	r1, r9
    mov	r3, sl
    bl GetSpriteWorkTargetCoordinates
    ldr	r0, [sp, #92]
    str	r0, [sp, #88]
    b .L08033E96
.L08033E46:
    cmp	r5, #17
    beq .L08033E86
    cmp	r5, #17
    bgt .L08033E54
    cmp	r5, #2
    beq .L08033E5A
    b .L08033E92
.L08033E54:
    cmp	r5, #37
    beq .L08033E7A
    b .L08033E92
.L08033E5A:
    cmp	r6, #17
    beq .L08033E6E
    cmp	r6, #37
    bne .L08033E92
    add	r0, sp, #96
    bl smethod_08009ADC__4Farm
    ldr	r0, [sp, #96]
    str	r0, [sp, #88]
    b .L08033E96
.L08033E6E:
    add	r0, sp, #100
    bl smethod_08009AE8__4Farm
    ldr	r0, [sp, #100]
    str	r0, [sp, #88]
    b .L08033E96
.L08033E7A:
    add	r0, sp, #104
    bl method_0800CE58__4Barn
    ldr	r0, [sp, #104]
    str	r0, [sp, #88]
    b .L08033E96
.L08033E86:
    add	r0, sp, #108
    bl method_0800C554__4Coop
    ldr	r0, [sp, #108]
    str	r0, [sp, #88]
    b .L08033E96
.L08033E92:
    movs	r6, #141
    lsls	r6, r6, #2
.L08033E96:
    movs	r0, #141
    lsls	r0, r0, #2
    cmp	r6, r0
    beq .L08033F8E
    add	r3, sp, #88
    movs	r2, #0
    ldrsh	r0, [r3, r2]
    ldr	r1, [sp, #156]
    subs	r0, r0, r1
    cmp	r0, #0
    bge .L08033EAE
    negs	r0, r0
.L08033EAE:
    cmp	r0, #7
    bgt .L08033F8E
    movs	r3, #90
    add	r3, sp
    movs	r2, #0
    ldrsh	r0, [r3, r2]
    ldr	r1, [sp, #160]
    subs	r0, r0, r1
    cmp	r0, #0
    bge .L08033EC4
    negs	r0, r0
.L08033EC4:
    cmp	r0, #7
    bgt .L08033F8E
    adds	r0, r7, #0
    bl func_08034940
    lsls	r0, r0, #24
    lsrs	r1, r0, #24
    cmp	r1, #0
    beq .L08033ED8
    b .L08034110
.L08033ED8:
    cmp	r6, r5
    bne .L08033F10
    adds	r0, r7, #0
    bl func_0803436C
    lsls	r0, r0, #24
    cmp	r0, #0
    bne .L08033EEA
    b .L08034068
.L08033EEA:
    ldr	r1, .L08033F0C
    ldr	r0, [sp, #112]
    ands	r0, r1
    movs	r1, #60
    orrs	r0, r1
    str	r0, [sp, #112]
    add	r1, sp, #112
    mov	r0, r8
    bl method_0809E7F0__13HarvestSpritePC15UnkBarnAnimal2C
    adds	r0, r7, #0
    adds	r0, #32
    ldrb	r2, [r0, #0]
    adds	r0, r7, #0
    movs	r1, #4
    b .L080340D2
    movs	r0, r0
.L08033F0C:
    .4byte 0xFFFF0000
.L08033F10:
    movs r4, #2
    add	r0, sp, #116
    strh	r1, [r0, #0]
    strh	r1, [r0, #2]
    mov	r8, r0
    cmp	r5, #17
    beq .L08033F58
    cmp	r5, #17
    bgt .L08033F28
    cmp	r5, #2
    beq .L08033F2E
    b .L08033F62
.L08033F28:
    cmp	r5, #37
    beq .L08033F4E
    b .L08033F62
.L08033F2E:
    cmp	r6, #17
    beq .L08033F42
    cmp	r6, #37
    bne .L08033F62
    movs	r4, #37
    add	r0, sp, #120
    bl method_0800CE58__4Barn
    ldr	r0, [sp, #120]
    b .L08033F60
.L08033F42:
    movs	r4, #17
    add	r0, sp, #124
    bl method_0800C554__4Coop
    ldr	r0, [sp, #124]
    b .L08033F60
.L08033F4E:
    add	r0, sp, #128
    bl smethod_08009ADC__4Farm
    ldr	r0, [sp, #128]
    b .L08033F60
.L08033F58:
    add	r0, sp, #132
    bl smethod_08009AE8__4Farm
    ldr	r0, [sp, #132]
.L08033F60:
    str	r0, [sp, #116]
.L08033F62:
    mov	r3, r8
    movs	r5, #0
    ldrsh	r2, [r3, r5]
    add	r0, sp, #116
    movs	r6, #2
    ldrsh	r3, [r0, r6]
    adds	r0, r7, #0
    adds	r1, r4, #0
    bl SetLocation__7AEntityUiii
    adds	r0, r7, #0
    adds	r0, #32
    ldrb	r2, [r0, #0]
    adds	r0, r7, #0
    movs	r1, #0
    movs	r3, #0
    bl func_08034180
    movs	r0, #0
    ldr	r1, [sp, #152]
    strh	r0, [r1, #0]
    b .L08034110
.L08033F8E:
    ldr	r2, [sp, #152]
    ldrh	r0, [r2, #0]
    cmp	r0, #0
    bne .L0803404C
    adds	r0, r7, #0
    bl func_08034940
    lsls	r0, r0, #24
    cmp	r0, #0
    beq .L08033FA4
    b .L08034110
.L08033FA4:
    ldrb	r1, [r4, #0]
    ldr	r0, [r7, #56]
    lsls	r1, r1, #3
    adds	r0, r0, r1
    ldr	r1, [r7, #60]
    mov	r2, sl
    bl func_08033584
    lsls	r0, r0, #24
    cmp	r0, #0
    beq .L08034068
    mov	r0, r8
    bl GetCurrentTask__C13HarvestSprite
    adds	r1, r0, #0
    mov	r0, r8
    bl GetTaskExp__C13HarvestSpriteQ213HarvestSprite4Task
    bl ComputeSpriteWorkSpeed
    adds	r6, r0, #0
    add	r4, sp, #88
    movs	r3, #0
    ldrsh	r1, [r4, r3]
    adds	r5, r4, #0
    movs	r2, #2
    ldrsh	r0, [r5, r2]
    ldr	r3, [sp, #160]
    subs	r0, r0, r3
    ldr	r4, [sp, #156]
    subs	r1, r1, r4
    cmp	r1, #0
    bge .L08033FE8
    negs	r1, r1
.L08033FE8:
    cmp	r0, #0
    bge .L08033FEE
    negs	r0, r0
.L08033FEE:
    adds	r0, r1, r0
    lsrs	r1, r0, #31
    adds	r0, r0, r1
    asrs	r0, r0, #1
    lsls	r0, r0, #16
    adds	r1, r6, #0
    bl __divsi3
    add	r3, sp, #140
    str	r0, [r3, #0]
    movs	r2, #180
    add	r0, sp, #136
    str	r2, [r0, #0]
    ldr	r1, [r3, #0]
    cmp	r2, r1
    bls .L08034010
    adds	r0, r3, #0
.L08034010:
    ldr	r0, [r0, #0]
    bl RollEntityMovementRandom
    ldr	r1, [sp, #152]
    strh	r0, [r1, #0]
    adds	r0, r7, #0
    adds	r0, #32
    ldrb	r4, [r0, #0]
    add	r0, sp, #8
    bl InitializeEntityCollisionProbe
    add	r0, sp, #88
    movs	r3, #0
    ldrsh	r2, [r0, r3]
    movs	r1, #2
    ldrsh	r3, [r5, r1]
    add	r5, sp, #8
    str	r5, [sp, #0]
    str	r4, [sp, #4]
    ldr	r0, [sp, #156]
    ldr	r1, [sp, #160]
    bl ChooseEntityFacingTowardPoint
    adds	r2, r0, #0
    adds	r0, r7, #0
    movs	r1, #1
    adds	r3, r6, #0
    bl func_08034180
    b .L08034110
.L0803404C:
    subs	r0, #1
    ldr	r6, [sp, #152]
    strh	r0, [r6, #0]
    b .L08034110
.L08034054:
    mov	r1, r8
    ldrh	r0, [r1, #32]
    cmp	r0, #0
    bne .L08034070
    adds	r0, r7, #0
    bl func_08034940
    lsls	r0, r0, #24
    cmp	r0, #0
    bne .L08034110
.L08034068:
    adds	r0, r7, #0
    bl ChooseSpriteWorkerNextTask
    b .L08034110
.L08034070:
    subs	r0, #1
    mov	r2, r8
    strh	r0, [r2, #32]
    b .L08034110
.L08034078:
    movs	r3, #10
    ldrsh	r1, [r7, r3]
    movs	r0, #180
    lsls	r0, r0, #1
    cmp	r1, r0
    bne .L080340DA
    ldr	r0, [r7, #28]
    cmp	r0, #0
    blt .L0803409A
    movs	r3, #128
    lsls	r3, r3, #8
    adds	r0, r7, #0
    movs	r1, #1
    movs	r2, #1
    bl func_08034180
    b .L08034110
.L0803409A:
    movs	r4, #14
    ldrsh	r1, [r7, r4]
    movs	r0, #8
    negs	r0, r0
    cmp	r1, r0
    bgt .L08034110
    mov	r0, r8
    bl TaskDayUpdate__13HarvestSprite
    add	r1, sp, #144
    mov	r0, r8
    bl method_0809E7C8__13HarvestSprite
    adds	r0, r7, #0
    adds	r0, #48
    ldrb	r1, [r0, #0]
    add	r0, sp, #8
    bl GetHarvestSpriteHomeLocation
    adds	r0, r7, #0
    add	r1, sp, #8
    bl SetLocation__12AActorEntityRC13ActorLocation
    adds	r0, r7, #0
    adds	r0, #32
    ldrb	r2, [r0, #0]
    adds	r0, r7, #0
    movs	r1, #0
.L080340D2:
    movs	r3, #0
    bl func_08034180
    b .L08034110
.L080340DA:
    ldr	r2, [r7, #24]
    movs	r3, #0
    ldr	r0, .L080340F4
    cmp	r1, r0
    bgt .L080340E6
    movs	r3, #1
.L080340E6:
    adds	r0, r3, #0
    cmp	r0, #0
    beq .L080340F8
    cmp	r2, #0
    ble .L080340FC
    b .L08034110
    movs	r0, r0
.L080340F4:
    .4byte 0x00000167
.L080340F8:
    cmp	r2, #0
    blt .L08034110
.L080340FC:
    movs	r2, #2
    cmp	r0, #0
    beq .L08034104
    movs	r2, #3
.L08034104:
    movs	r3, #128
    lsls	r3, r3, #8
    adds	r0, r7, #0
    movs	r1, #1
    bl func_08034180
.L08034110:
    adds	r4, r7, #0
    adds	r4, #66
    ldrb	r0, [r4, #0]
    cmp	r0, #0
    beq .L08034138
    adds	r0, r7, #0
    adds	r0, #64
    ldrb	r1, [r0, #0]
    adds	r0, r7, #0
    bl GetSpriteTaskAnimation
    adds	r1, r0, #0
    ldrh	r0, [r7, #34]
    cmp	r0, r1
    beq .L08034134
    adds	r0, r7, #0
    bl SetAnim__12AActorEntityUi
.L08034134:
    movs	r0, #0
    strb	r0, [r4, #0]
.L08034138:
    ldr	r1, [r7, #28]
    cmp	r1, #0
    beq .L08034144
    ldr	r0, [r7, #12]
    adds	r0, r0, r1
    str	r0, [r7, #12]
.L08034144:
    ldr	r1, [r7, #24]
    cmp	r1, #0
    beq .L08034150
    ldr	r0, [r7, #8]
    adds	r0, r0, r1
    str	r0, [r7, #8]
.L08034150:
    ldrh	r0, [r7, #36]
    cmp	r0, #0
    beq .L0803415A
    subs	r0, #1
    b .L0803415C
.L0803415A:
    ldrh	r0, [r7, #38]
.L0803415C:
    strh	r0, [r7, #36]
    ldr	r4, [r7, #16]
    cmp	r4, #0
    beq .L0803416E
    ldr	r0, [r4, #4]
    ldr	r1, [r0, #12]
    adds	r0, r4, #0
    bl _call_via_r1
.L0803416E:
    add	sp, #172
    pop	{r3, r4, r5}
    mov	r8, r3
    mov	r9, r4
    mov	sl, r5
    pop	{r4, r5, r6, r7}
    pop	{r0}
    bx	r0
    .short 0x0000

    .section .text.npc_side.08034180, "ax", %progbits

    thumb_func_start func_08034180
func_08034180: @ 0x08034180
    push {r4, r5, r6, r7, lr}
    adds r4, r0, #0
    adds r7, r1, #0
    adds r5, r2, #0
    adds r6, r3, #0
    bl GetSpriteTaskAnimation
    adds r1, r0, #0
    ldrh r0, [r4, #0x22]
    cmp r0, r1
    beq .L0803419C
    adds r0, r4, #0
    bl SetAnim__12AActorEntityUi
.L0803419C:
    adds r0, r4, #0
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, r5
    beq .L080341AE
    adds r0, r4, #0
    adds r1, r5, #0
    bl SetAnimFacing__12AActorEntityUi
.L080341AE:
    adds r0, r4, #0
    adds r0, #0x42
    movs r1, #0
    strb r1, [r0]
    cmp r7, #1
    beq .L080341D0
    cmp r7, #1
    bgt .L080341C4
    cmp r7, #0
    beq .L080341C8
    b .L0803420A
.L080341C4:
    cmp r7, #4
    bgt .L0803420A
.L080341C8:
    movs r0, #0
    str r0, [r4, #0x18]
    str r0, [r4, #0x1c]
    b .L0803420A
.L080341D0:
    adds r0, r6, #0
    cmp r6, #0
    bge .L080341D8
    rsbs r0, r6, #0
.L080341D8:
    adds r6, r0, #0
    cmp r5, #1
    beq .L080341F8
    cmp r5, #1
    bgt .L080341E8
    cmp r5, #0
    beq .L080341F2
    b .L0803420A
.L080341E8:
    cmp r5, #2
    beq .L08034200
    cmp r5, #3
    beq .L08034206
    b .L0803420A
.L080341F2:
    str r6, [r4, #0x1c]
    str r1, [r4, #0x18]
    b .L0803420A
.L080341F8:
    rsbs r0, r6, #0
    str r0, [r4, #0x1c]
    str r1, [r4, #0x18]
    b .L0803420A
.L08034200:
    rsbs r0, r6, #0
    str r0, [r4, #0x18]
    b .L08034208
.L08034206:
    str r6, [r4, #0x18]
.L08034208:
    str r1, [r4, #0x1c]
.L0803420A:
    adds r0, r4, #0
    adds r0, #0x40
    adds r5, r0, #0
    ldrb r0, [r5]
    cmp r7, r0
    beq .L0803423E
    movs r1, #1
    rsbs r1, r1, #0
    cmp r7, #2
    beq .L08034224
    cmp r7, #3
    beq .L08034228
    b .L0803422A
.L08034224:
    movs r1, #8
    b .L0803422E
.L08034228:
    movs r1, #5
.L0803422A:
    cmp r1, #0
    blt .L08034238
.L0803422E:
    adds r0, r4, #0
    movs r2, #1
    bl func_08032384
    b .L0803423E
.L08034238:
    adds r0, r4, #0
    bl func_080323C8
.L0803423E:
    strb r7, [r5]
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0
    .align 2, 0

    .section .text.npc_side.08034260, "ax", %progbits

    .section .text.npc_side.0803436C, "ax", %progbits

    thumb_func_start func_0803436C
func_0803436C: @ 0x0803436C
    push {r4, r5, r6, r7, lr}
    mov r7, sl
    mov r6, sb
    mov r5, r8
    push {r5, r6, r7}
    sub sp, #0x2c
    str r0, [sp, #0x24]
    ldr r0, [r0]
    str r0, [sp, #0x28]
    ldr r3, [sp, #0x24]
    adds r3, #0x30
    ldrb r1, [r3]
    ldr r2, [sp, #0x24]
    ldr r0, [r2, #0x38]
    lsls r1, r1, #3
    mov r2, sp
    adds r1, r0, r1
    ldm r1!, {r4, r5}
    stm r2!, {r4, r5}
    ldrb r1, [r3]
    bl ClearNpcEventSlot
    ldr r0, [sp, #0x24]
    ldr r1, [r0, #0x3c]
    mov r0, sp
    ldr r2, [sp, #0x28]
    bl func_08033584
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L080343AC
    b .L080348FA
.L080343AC:
    ldr r1, [sp, #0x24]
    ldr r5, [r1, #0x10]
    cmp r5, #0
    beq .L0803441A
    ldr r2, [r1, #0x3c]
    add r4, sp, #0x18
    adds r0, r4, #0
    mov r1, sp
    ldr r3, [sp, #0x28]
    bl GetSpriteWorkTargetCoordinates
    mov r0, sp
    ldrb r0, [r0]
    lsls r0, r0, #0x1c
    lsrs r0, r0, #0x1c
    cmp r0, #4
    beq .L080343E4
    cmp r0, #4
    bgt .L080343D8
    cmp r0, #3
    beq .L080343DE
    b .L08034404
.L080343D8:
    cmp r0, #5
    beq .L080343FE
    b .L08034404
.L080343DE:
    ldrh r0, [r4, #2]
    subs r0, #0x18
    b .L08034402
.L080343E4:
    movs r0, #2
    ldrsh r2, [r4, r0]
    mov r0, sp
    ldrb r0, [r0, #1]
    movs r1, #7
    ands r0, r1
    adds r1, r2, #0
    adds r1, #0x18
    cmp r0, #3
    bhi .L080343FA
    subs r1, #0x38
.L080343FA:
    strh r1, [r4, #2]
    b .L08034404
.L080343FE:
    ldrh r0, [r4, #2]
    subs r0, #0x28
.L08034402:
    strh r0, [r4, #2]
.L08034404:
    ldr r0, [sp, #0x24]
    adds r0, #0x30
    ldrb r1, [r0]
    movs r0, #0
    ldrsh r2, [r4, r0]
    add r0, sp, #0x18
    movs r4, #2
    ldrsh r3, [r0, r4]
    adds r0, r5, #0
    bl StartSpriteWorkerEffect
.L0803441A:
    mov r0, sp
    ldrb r0, [r0]
    lsls r0, r0, #0x1c
    lsrs r0, r0, #0x1c
    cmp r0, #8
    bls .L08034428
    b .L080348FA
.L08034428:
    lsls r0, r0, #2
    ldr r1, .L08034434 @ =.L08034438
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L08034434: .4byte .L08034438
.L08034438: @ jump table
    .4byte .L080348FA @ case 0
    .4byte .L0803445C @ case 1
    .4byte .L080344E4 @ case 2
    .4byte .L080345EA @ case 3
    .4byte .L08034616 @ case 4
    .4byte .L08034644 @ case 5
    .4byte .L08034680 @ case 6
    .4byte .L080347EC @ case 7
    .4byte .L08034834 @ case 8
.L0803445C:
    ldr r5, [sp, #0x28]
    ldr r5, [r5]
    mov sb, r5
    movs r0, #0xd8
    add sb, r0
    ldr r1, [sp, #0x24]
    movs r2, #0xa
    ldrsh r6, [r1, r2]
    movs r5, #0xe
    ldrsh r4, [r1, r5]
    mov r8, r4
    add r4, sp, #8
    movs r2, #2
    ldrh r1, [r4]
    ldr r3, .L080344E0 @ =0xFFFFFC00
    adds r0, r3, #0
    ands r0, r1
    orrs r0, r2
    strh r0, [r4]
    lsls r6, r6, #0x10
    lsrs r1, r6, #0x10
    movs r0, #0x3f
    mov sl, r0
    ands r1, r0
    lsls r1, r1, #2
    ldrb r5, [r4, #1]
    movs r2, #3
    adds r0, r2, #0
    ands r0, r5
    orrs r0, r1
    strb r0, [r4, #1]
    lsrs r6, r6, #0x16
    ldrh r1, [r4, #2]
    adds r0, r3, #0
    ands r0, r1
    orrs r0, r6
    strh r0, [r4, #2]
    mov r1, r8
    lsls r1, r1, #0x10
    mov r8, r1
    lsrs r0, r1, #0x10
    mov r5, sl
    ands r0, r5
    lsls r0, r0, #2
    ldrb r1, [r4, #3]
    ands r2, r1
    orrs r2, r0
    strb r2, [r4, #3]
    mov r0, r8
    lsrs r0, r0, #0x16
    mov r8, r0
    ldrh r0, [r4, #4]
    ands r3, r0
    mov r1, r8
    orrs r3, r1
    strh r3, [r4, #4]
    mov r5, sb
    ldr r2, [r5]
    ldr r0, [sp, #0x28]
    adds r1, r4, #0
    bl _call_via_r2
    movs r6, #1
    mov r8, r4
    b .L080348FE
    .align 2, 0
.L080344E0: .4byte 0xFFFFFC00
.L080344E4:
    add r0, sp, #8
    mov sb, r0
    bl __12RucksackItem
    ldr r1, [sp, #0x28]
    ldr r1, [r1]
    mov r8, r1
    movs r2, #0xe0
    add r8, r2
    ldr r4, [sp, #0x24]
    movs r0, #0xa
    ldrsh r5, [r4, r0]
    movs r1, #0xe
    ldrsh r6, [r4, r1]
    add r7, sp, #0x10
    movs r2, #2
    ldrh r1, [r7]
    ldr r3, .L08034580 @ =0xFFFFFC00
    adds r0, r3, #0
    ands r0, r1
    orrs r0, r2
    strh r0, [r7]
    lsls r5, r5, #0x10
    lsrs r1, r5, #0x10
    movs r2, #0x3f
    mov sl, r2
    ands r1, r2
    lsls r1, r1, #2
    ldrb r4, [r7, #1]
    movs r2, #3
    adds r0, r2, #0
    ands r0, r4
    orrs r0, r1
    strb r0, [r7, #1]
    lsrs r5, r5, #0x16
    ldrh r1, [r7, #2]
    adds r0, r3, #0
    ands r0, r1
    orrs r0, r5
    strh r0, [r7, #2]
    lsls r6, r6, #0x10
    lsrs r0, r6, #0x10
    mov r4, sl
    ands r0, r4
    lsls r0, r0, #2
    ldrb r1, [r7, #3]
    ands r2, r1
    orrs r2, r0
    strb r2, [r7, #3]
    lsrs r6, r6, #0x16
    ldrh r0, [r7, #4]
    ands r3, r0
    orrs r3, r6
    strh r3, [r7, #4]
    mov r5, r8
    ldr r3, [r5]
    ldr r0, [sp, #0x28]
    adds r1, r7, #0
    mov r2, sb
    bl _call_via_r3
    mov r0, sb
    bl IsEmpty__C12RucksackItem
    lsls r0, r0, #0x18
    mov r8, sb
    cmp r0, #0
    beq .L0803456E
    b .L080348FA
.L0803456E:
    mov r0, r8
    bl GetKind__C12RucksackItem
    cmp r0, #0
    beq .L08034584
    cmp r0, #1
    beq .L080345C0
    b .L080348FA
    .align 2, 0
.L08034580: .4byte 0xFFFFFC00
.L08034584:
    ldr r0, [sp, #0x28]
    ldr r4, [r0]
    movs r1, #0xae
    lsls r1, r1, #1
    adds r4, r4, r1
    add r5, sp, #0x1c
    adds r0, r7, #0
    mov r1, r8
    bl GetFood__C12RucksackItem
    ldrb r1, [r7]
    mov r0, sp
    adds r0, #0x11
    ldrb r0, [r0]
    lsls r0, r0, #8
    orrs r0, r1
    mov r1, sp
    adds r1, #0x12
    ldrb r1, [r1]
    lsls r1, r1, #0x10
    orrs r1, r0
    adds r0, r5, #0
    bl __7ProductG4Food
    ldr r2, [r4]
    ldr r0, [sp, #0x28]
    adds r1, r5, #0
    bl _call_via_r2
    b .L080345E6
.L080345C0:
    ldr r2, [sp, #0x28]
    ldr r4, [r2]
    movs r5, #0xae
    lsls r5, r5, #1
    adds r4, r4, r5
    mov r5, sp
    adds r5, #0x1d
    mov r0, r8
    bl GetArticle__C12RucksackItem
    adds r1, r0, #0
    adds r0, r5, #0
    bl __7ProductG7Article
    ldr r2, [r4]
    ldr r0, [sp, #0x28]
    adds r1, r5, #0
    bl _call_via_r2
.L080345E6:
    movs r6, #0
    b .L080348FE
.L080345EA:
    ldr r1, [sp, #0x24]
    ldr r0, [r1, #0x3c]
    movs r4, #0xff
    lsls r4, r4, #2
    adds r0, r0, r4
    movs r1, #1
    bl SubtractStoredBushels__4CoopUi
    ldr r2, [sp, #0x24]
    ldr r0, [r2, #0x3c]
    adds r0, r0, r4
    mov r1, sp
    ldrb r1, [r1, #1]
    bl SetEntBushel__4CoopUi
    ldr r4, [sp, #0x28]
    ldr r0, [r4]
    adds r0, #0xa0
    mov r1, sp
    ldrb r1, [r1, #1]
    ldr r2, [r0]
    b .L0803466E
.L08034616:
    ldr r1, [sp, #0x24]
    ldr r0, [r1, #0x3c]
    ldr r4, .L08034640 @ =0x000005DC
    adds r0, r0, r4
    movs r1, #1
    bl SubtractStoredBushels__4BarnUi
    ldr r2, [sp, #0x24]
    ldr r0, [r2, #0x3c]
    adds r0, r0, r4
    mov r1, sp
    ldrb r1, [r1, #1]
    bl SetBushelForStall__4BarnUi
    ldr r4, [sp, #0x28]
    ldr r0, [r4]
    adds r0, #0x9c
    mov r1, sp
    ldrb r1, [r1, #1]
    ldr r2, [r0]
    b .L0803466E
    .align 2, 0
.L08034640: .4byte 0x000005DC
.L08034644:
    ldr r1, [sp, #0x24]
    ldr r0, [r1, #0x3c]
    ldr r4, .L0803467C @ =0x000005DC
    adds r0, r0, r4
    movs r1, #1
    bl SubtractStoredBushels__4BarnUi
    ldr r2, [sp, #0x24]
    ldr r0, [r2, #0x3c]
    adds r0, r0, r4
    mov r1, sp
    ldrb r1, [r1, #1]
    bl SetBushelForPregnancyStall__4BarnUi
    ldr r4, [sp, #0x28]
    ldr r2, [r4]
    adds r2, #0x9c
    mov r0, sp
    ldrb r1, [r0, #1]
    adds r1, #0x10
    ldr r2, [r2]
.L0803466E:
    adds r0, r4, #0
    bl _call_via_r2
    movs r6, #2
    add r5, sp, #8
    mov r8, r5
    b .L080348FE
    .align 2, 0
.L0803467C: .4byte 0x000005DC
.L08034680:
    mov r0, sp
    ldrb r1, [r0, #1]
    adds r1, #0x36
    ldr r2, [sp, #0x28]
    ldr r0, [r2]
    ldr r2, [r0, #0x40]
    ldr r0, [sp, #0x28]
    bl _call_via_r2
    adds r7, r0, #0
    cmp r7, #0
    bne .L0803469A
    b .L080348FA
.L0803469A:
    ldr r0, [r7, #0x14]
    ldr r1, [r0, #0x50]
    adds r0, r7, #0
    bl _call_via_r1
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L080346B4
    ldr r0, [r7, #0x14]
    ldr r1, [r0, #0x70]
    adds r0, r7, #0
    bl _call_via_r1
.L080346B4:
    ldr r0, [r7, #0x14]
    ldr r1, [r0, #0x54]
    adds r0, r7, #0
    bl _call_via_r1
    lsls r0, r0, #0x18
    add r4, sp, #8
    mov r8, r4
    cmp r0, #0
    beq .L08034754
    ldr r0, [r7, #0x14]
    ldr r1, [r0, #0x74]
    adds r0, r7, #0
    bl _call_via_r1
    movs r1, #0x1c
    cmp r0, #5
    bhi .L08034716
    lsls r0, r0, #2
    ldr r1, .L080346E4 @ =.L080346E8
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L080346E4: .4byte .L080346E8
.L080346E8: @ jump table
    .4byte .L08034700 @ case 0
    .4byte .L08034704 @ case 1
    .4byte .L08034708 @ case 2
    .4byte .L0803470C @ case 3
    .4byte .L08034710 @ case 4
    .4byte .L08034714 @ case 5
.L08034700:
    movs r1, #0x1c
    b .L08034716
.L08034704:
    movs r1, #0x1d
    b .L08034716
.L08034708:
    movs r1, #0x1e
    b .L08034716
.L0803470C:
    movs r1, #0x1f
    b .L08034716
.L08034710:
    movs r1, #0x20
    b .L08034716
.L08034714:
    movs r1, #0x21
.L08034716:
    ldr r5, [sp, #0x28]
    ldr r4, [r5]
    movs r0, #0xae
    lsls r0, r0, #1
    adds r4, r4, r0
    mov r6, sp
    adds r6, #0x1e
    add r5, sp, #8
    adds r0, r5, #0
    bl __4FoodUi
    ldrb r1, [r5]
    mov r0, sp
    adds r0, #9
    ldrb r0, [r0]
    lsls r0, r0, #8
    orrs r0, r1
    mov r1, sp
    adds r1, #0xa
    ldrb r1, [r1]
    lsls r1, r1, #0x10
    orrs r1, r0
    adds r0, r6, #0
    bl __7ProductG4Food
    ldr r2, [r4]
    ldr r0, [sp, #0x28]
    adds r1, r6, #0
    bl _call_via_r2
    mov r8, r5
.L08034754:
    ldr r0, [r7, #0x14]
    ldr r1, [r0, #0x58]
    adds r0, r7, #0
    bl _call_via_r1
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L080347DA
    ldr r0, [r7, #0x14]
    ldr r1, [r0, #0x78]
    adds r0, r7, #0
    bl _call_via_r1
    movs r1, #5
    cmp r0, #5
    bhi .L080347B2
    lsls r0, r0, #2
    ldr r1, .L08034780 @ =.L08034784
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L08034780: .4byte .L08034784
.L08034784: @ jump table
    .4byte .L0803479C @ case 0
    .4byte .L080347A0 @ case 1
    .4byte .L080347A4 @ case 2
    .4byte .L080347A8 @ case 3
    .4byte .L080347AC @ case 4
    .4byte .L080347B0 @ case 5
.L0803479C:
    movs r1, #5
    b .L080347B2
.L080347A0:
    movs r1, #6
    b .L080347B2
.L080347A4:
    movs r1, #7
    b .L080347B2
.L080347A8:
    movs r1, #8
    b .L080347B2
.L080347AC:
    movs r1, #9
    b .L080347B2
.L080347B0:
    movs r1, #0xa
.L080347B2:
    ldr r2, [sp, #0x28]
    ldr r5, [r2]
    movs r4, #0xae
    lsls r4, r4, #1
    adds r5, r5, r4
    mov r6, sp
    adds r6, #0x1f
    add r4, sp, #0x20
    adds r0, r4, #0
    bl __7ArticleUi
    ldrb r1, [r4]
    adds r0, r6, #0
    bl __7ProductG7Article
    ldr r2, [r5]
    ldr r0, [sp, #0x28]
    adds r1, r6, #0
    bl _call_via_r2
.L080347DA:
    ldr r0, [r7, #0x14]
    adds r0, #0x88
    ldr r2, [r0]
    adds r0, r7, #0
    movs r1, #0x3c
    bl _call_via_r2
    movs r6, #2
    b .L080348FE
.L080347EC:
    mov r0, sp
    ldrb r1, [r0, #1]
    adds r1, #0x2e
    ldr r5, [sp, #0x28]
    ldr r0, [r5]
    ldr r2, [r0, #0x40]
    adds r0, r5, #0
    bl _call_via_r2
    adds r4, r0, #0
    cmp r4, #0
    beq .L080348FA
    ldr r0, [r4, #0x14]
    ldr r1, [r0, #0x50]
    adds r0, r4, #0
    bl _call_via_r1
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0803481E
    ldr r0, [r4, #0x14]
    ldr r1, [r0, #0x70]
    adds r0, r4, #0
    bl _call_via_r1
.L0803481E:
    ldr r0, [r4, #0x14]
    adds r0, #0x88
    ldr r2, [r0]
    adds r0, r4, #0
    movs r1, #0x3c
    bl _call_via_r2
    movs r6, #2
    add r0, sp, #8
    mov r8, r0
    b .L080348FE
.L08034834:
    mov r0, sp
    ldrb r1, [r0, #1]
    adds r4, r1, #0
    adds r4, #0x4c
    ldr r2, [sp, #0x24]
    ldr r0, [r2, #0x3c]
    movs r5, #0xff
    lsls r5, r5, #2
    adds r0, r0, r5
    bl GetEgg__C4CoopUi
    cmp r0, #0
    beq .L080348FA
    ldrb r0, [r0, #6]
    lsls r0, r0, #0x1d
    lsrs r0, r0, #0x1d
    movs r7, #0xf
    cmp r0, #5
    bhi .L08034896
    lsls r0, r0, #2
    ldr r1, .L08034864 @ =.L08034868
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L08034864: .4byte .L08034868
.L08034868: @ jump table
    .4byte .L08034880 @ case 0
    .4byte .L08034884 @ case 1
    .4byte .L08034888 @ case 2
    .4byte .L0803488C @ case 3
    .4byte .L08034890 @ case 4
    .4byte .L08034894 @ case 5
.L08034880:
    movs r7, #0xf
    b .L08034896
.L08034884:
    movs r7, #0x10
    b .L08034896
.L08034888:
    movs r7, #0x11
    b .L08034896
.L0803488C:
    movs r7, #0x12
    b .L08034896
.L08034890:
    movs r7, #0x13
    b .L08034896
.L08034894:
    movs r7, #0x14
.L08034896:
    ldr r1, [sp, #0x24]
    ldr r0, [r1]
    ldr r1, [r0]
    ldr r2, [r1, #0x3c]
    adds r1, r4, #0
    bl _call_via_r2
    ldr r2, [sp, #0x24]
    ldr r0, [r2, #0x3c]
    movs r4, #0xff
    lsls r4, r4, #2
    adds r0, r0, r4
    mov r1, sp
    ldrb r1, [r1, #1]
    bl method_0800C9B8__4CoopUi
    ldr r5, [sp, #0x28]
    ldr r4, [r5]
    movs r0, #0xae
    lsls r0, r0, #1
    adds r4, r4, r0
    mov r6, sp
    adds r6, #0x21
    add r5, sp, #8
    adds r0, r5, #0
    adds r1, r7, #0
    bl __4FoodUi
    ldrb r1, [r5]
    mov r0, sp
    adds r0, #9
    ldrb r0, [r0]
    lsls r0, r0, #8
    orrs r0, r1
    mov r1, sp
    adds r1, #0xa
    ldrb r1, [r1]
    lsls r1, r1, #0x10
    orrs r1, r0
    adds r0, r6, #0
    bl __7ProductG4Food
    ldr r2, [r4]
    ldr r0, [sp, #0x28]
    adds r1, r6, #0
    bl _call_via_r2
    movs r6, #2
    mov r8, r5
    b .L080348FE
.L080348FA:
    movs r0, #0
    b .L0803492A
.L080348FE:
    ldr r1, .L0803493C @ =gUnk_080F1496
    mov r0, r8
    movs r2, #3
    bl memcpy
    cmp r6, #3
    beq .L08034928
    movs r0, #0xc8
    bl RollEntityMovementRandom
    mov r2, r8
    adds r1, r2, r6
    ldrb r1, [r1]
    cmp r0, r1
    bhs .L08034928
    ldr r4, [sp, #0x24]
    ldr r0, [r4, #0x34]
    adds r1, r6, #0
    movs r2, #1
    bl AddTaskExp__13HarvestSpriteQ213HarvestSprite4Taski
.L08034928:
    movs r0, #1
.L0803492A:
    add sp, #0x2c
    pop {r3, r4, r5}
    mov r8, r3
    mov sb, r4
    mov sl, r5
    pop {r4, r5, r6, r7}
    pop {r1}
    bx r1
    .align 2, 0
.L0803493C: .4byte gUnk_080F1496

    thumb_func_start func_08034940
func_08034940: @ 0x08034940
    push {r4, r5, lr}
    sub sp, #0xc
    adds r5, r0, #0
    ldr r0, [r5]
    ldr r1, [r0]
    movs r2, #0xa2
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldrh r2, [r0, #0xa]
    lsls r0, r2, #0x1b
    lsrs r0, r0, #0x1b
    cmp r0, #5
    bls .L0803497C
    subs r0, #6
    lsls r1, r0, #4
    subs r1, r1, r0
    lsls r1, r1, #2
    lsls r0, r2, #0x15
    lsrs r0, r0, #0x1a
    adds r1, r1, r0
    adds r0, r5, #0
    adds r0, #0x30
    ldrb r0, [r0]
    ldr r2, .L080349A8 @ =0x00000385
    adds r0, r0, r2
    cmp r1, r0
    blo .L08034A08
.L0803497C:
    ldrh r0, [r5, #4]
    cmp r0, #0x11
    beq .L080349AC
    cmp r0, #0x11
    ble .L080349C4
    cmp r0, #0x25
    bne .L080349C4
    mov r4, sp
    mov r0, sp
    bl smethod_08009ADC__4Farm
    mov r0, sp
    movs r1, #0
    ldrsh r2, [r0, r1]
    movs r0, #2
    ldrsh r3, [r4, r0]
    adds r0, r5, #0
    movs r1, #2
    bl SetLocation__7AEntityUiii
    b .L080349C4
    .align 2, 0
.L080349A8: .4byte 0x00000385
.L080349AC:
    add r4, sp, #4
    adds r0, r4, #0
    bl smethod_08009AE8__4Farm
    movs r1, #0
    ldrsh r2, [r4, r1]
    movs r0, #2
    ldrsh r3, [r4, r0]
    adds r0, r5, #0
    movs r1, #2
    bl SetLocation__7AEntityUiii
.L080349C4:
    ldr r0, [r5, #0x38]
    adds r1, r5, #0
    adds r1, #0x30
    ldrb r1, [r1]
    bl ClearNpcEventSlot
    ldr r0, [r5, #0x34]
    add r1, sp, #8
    bl method_0809E7FC__13HarvestSprite
    movs r2, #0xa
    ldrsh r1, [r5, r2]
    movs r0, #0xb4
    lsls r0, r0, #1
    cmp r1, r0
    bne .L080349E8
    movs r2, #1
    b .L080349F2
.L080349E8:
    ldr r0, .L08034A04 @ =0x00000167
    movs r2, #2
    cmp r1, r0
    bgt .L080349F2
    movs r2, #3
.L080349F2:
    movs r3, #0x80
    lsls r3, r3, #8
    adds r0, r5, #0
    movs r1, #1
    bl func_08034180
    movs r0, #1
    b .L08034A0A
    .align 2, 0
.L08034A04: .4byte 0x00000167
.L08034A08:
    movs r0, #0
.L08034A0A:
    add sp, #0xc
    pop {r4, r5}
    pop {r1}
    bx r1
    .align 2, 0

    thumb_func_start func_08034A14
func_08034A14: @ 0x08034A14
    push {r4, r5, r6, lr}
    mov r6, sb
    mov r5, r8
    push {r5, r6}
    sub sp, #0x10
    mov r8, r0
    adds r4, r1, #0
    ldr r0, .L08034A94 @ =gUnk_080F14B8
    mov sb, r0
    adds r5, r4, #0
    adds r5, #0x30
    ldrb r0, [r5]
    lsls r0, r0, #2
    add r0, sb
    ldr r3, [r0]
    movs r6, #0
    str r6, [sp]
    movs r0, #4
    str r0, [sp, #4]
    str r6, [sp, #8]
    add r0, sp, #0xc
    strb r6, [r0]
    mov r0, r8
    movs r2, #3
    bl InitializeSideActorSpriteLayers
    ldr r0, .L08034A98 @ =vtable_unk_080E6904
    mov r1, r8
    str r0, [r1, #4]
    ldr r0, [r4]
    ldr r1, [r0]
    ldr r1, [r1, #0x68]
    bl _call_via_r1
    adds r1, r0, #0
    ldr r2, .L08034A9C @ =gUnk_080F14F0
    ldrb r5, [r5]
    lsls r0, r5, #1
    adds r0, r0, r2
    ldrh r2, [r0]
    ldr r3, [r4]
    lsls r5, r5, #2
    add r5, sb
    mov r0, r8
    adds r0, #0x8c
    movs r4, #2
    str r4, [sp]
    ldr r4, [r5]
    str r4, [sp, #4]
    add r4, sp, #8
    strb r6, [r4]
    bl InitializeAnimatedMapSpriteSinglePalette
    mov r0, r8
    adds r0, #0xd0
    strh r6, [r0]
    mov r0, r8
    add sp, #0x10
    pop {r3, r4}
    mov r8, r3
    mov sb, r4
    pop {r4, r5, r6}
    pop {r1}
    bx r1
    .align 2, 0
.L08034A94: .4byte gUnk_080F14B8
.L08034A98: .4byte vtable_unk_080E6904
.L08034A9C: .4byte gUnk_080F14F0
    .section .text.npc_side.08034AE8, "ax", %progbits

    thumb_func_start DrawSpriteWorkerEffectLayer
DrawSpriteWorkerEffectLayer: @ 0x08034AE8
    push	{r4, r5, r6, r7, lr}
    mov	r7, sl
    mov	r6, r9
    mov	r5, r8
    push	{r5, r6, r7}
    sub	sp, #60
    adds	r5, r0, #0
    adds	r4, r1, #0
    bl func_08032690
    adds	r0, r5, #0
    adds	r0, #208
    ldrh	r0, [r0, #0]
    cmp	r0, #0
    beq .L08034BEA
    add	r3, sp, #52
    ldr	r0, [r4, #0]
    ldr	r1, [r4, #8]
    str	r0, [sp, #52]
    str	r1, [r3, #4]
    adds	r0, r5, #0
    adds	r0, #204
    movs	r2, #0
    ldrsh	r1, [r0, r2]
    adds	r0, #2
    movs	r6, #0
    ldrsh	r2, [r0, r6]
    movs	r6, #12
    ldrsh	r0, [r4, r6]
    subs	r1, r1, r0
    mov	r9, r1
    movs	r1, #14
    ldrsh	r0, [r4, r1]
    subs	r0, r2, r0
    mov	r8, r0
    movs	r0, #128
    lsls	r0, r0, #8
    subs	r4, r0, r2
    adds	r6, r5, #0
    adds	r6, #140
    mov	sl, r3
    adds	r0, r5, #0
    adds	r0, #180
    ldr	r1, [r0, #0]
    ldr	r3, [r1, #0]
    ldrh	r2, [r0, #12]
    adds	r0, #4
    lsls	r2, r2, #2
    ldr	r0, [r0, #0]
    adds	r0, r0, r2
    ldrh	r2, [r0, #0]
    add	r0, sp, #20
    ldr	r3, [r3, #16]
    bl _call_via_r3
    add	r7, sp, #20
    mov	r2, sl
    ldr	r3, [r2, #4]
    ldr	r2, [r6, #0]
    adds	r0, r5, #0
    adds	r0, #144
    ldr	r1, [r0, #4]
    negs	r0, r1
    orrs	r0, r1
    cmp	r0, #0
    bge .L08034B94
    ldrh	r0, [r6, #12]
    adds	r1, r5, #0
    adds	r1, #156
    str	r4, [sp, #0]
    adds	r4, r7, #0
    str	r4, [sp, #4]
    str	r2, [sp, #8]
    str	r0, [sp, #12]
    str	r1, [sp, #16]
    adds	r0, r3, #0
    mov	r1, r9
    mov	r2, r8
    movs	r3, #85
    ldr	r4, .L08034B90
    bl _call_via_r4
    b .L08034B96
    movs	r0, r0
.L08034B90:
    .4byte func_030004DC
.L08034B94:
    movs	r0, #0
.L08034B96:
    cmp	r0, #0
    beq .L08034BEA
    adds	r0, r6, #0
    adds	r0, #60
    ldrb	r1, [r0, #0]
    adds	r5, r0, #0
    cmp	r1, #0
    beq .L08034BEA
    mov	r0, sl
    ldr	r1, [r0, #0]
    adds	r2, r7, #0
    adds	r2, #8
    adds	r0, r6, #0
    bl func_080A480C
    adds	r0, r6, #0
    adds	r0, #61
    ldrb	r0, [r0, #0]
    cmp	r0, #0
    beq .L08034BCC
    adds	r1, r7, #0
    adds	r1, #16
    adds	r0, r6, #0
    movs	r2, #1
    bl LoadMapSpriteResourcePalettes
    b .L08034BE6
.L08034BCC:
    adds	r4, r6, #0
    adds	r4, #62
    ldrb	r0, [r4, #0]
    cmp	r0, #0
    bne .L08034BE6
    adds	r1, r7, #0
    adds	r1, #16
    adds	r0, r6, #0
    movs	r2, #1
    bl LoadMapSpriteResourcePalettes
    movs	r0, #1
    strb	r0, [r4, #0]
.L08034BE6:
    movs	r0, #0
    strb	r0, [r5, #0]
.L08034BEA:
    add	sp, #60
    pop	{r3, r4, r5}
    mov	r8, r3
    mov	r9, r4
    mov	sl, r5
    pop	{r4, r5, r6, r7}
    pop	{r0}
    bx	r0
    .short 0x0000

    .section .text.npc_side.08034C40, "ax", %progbits

    .section .text.npc_side.08034C4C, "ax", %progbits

    .section .text.npc_side.08034C64, "ax", %progbits
