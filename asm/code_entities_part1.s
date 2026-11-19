    .INCLUDE "asm/macro.inc"
    .SYNTAX UNIFIED
    .section .text.entities_part1.08025248, "ax", %progbits
    thumb_func_start func_08025248
func_08025248: @ 0x08025248
    push {r4, r5, r6, r7, lr}
    mov r7, r8
    push {r7}
    sub sp, #8
    mov r8, r1
    adds r6, r3, #0
    ldr r7, [sp, #0x20]
    ldr r0, [r2]
    ldr r1, [r2, #4]
    str r0, [sp]
    str r1, [sp, #4]
    movs r5, #1
    cmp r5, r7
    bhi .L08025286
    mov r4, sp
.L08025266:
    ldrh r0, [r4, #2]
    adds r0, r0, r6
    strh r0, [r4, #2]
    ldrh r0, [r4, #6]
    adds r0, r0, r6
    strh r0, [r4, #6]
    mov r0, r8
    mov r1, sp
    bl func_080AC070
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08025286
    adds r5, #1
    cmp r5, r7
    bls .L08025266
.L08025286:
    adds r0, r5, #0
    add sp, #8
    pop {r3}
    mov r8, r3
    pop {r4, r5, r6, r7}
    pop {r1}
    bx r1
    .section .text.entities_part1.080251FC, "ax", %progbits
    thumb_func_start func_080251FC
func_080251FC: @ 0x080251FC
    push {r4, r5, r6, r7, lr}
    mov r7, r8
    push {r7}
    sub sp, #8
    mov r8, r1
    adds r6, r3, #0
    ldr r7, [sp, #0x20]
    ldr r0, [r2]
    ldr r1, [r2, #4]
    str r0, [sp]
    str r1, [sp, #4]
    movs r5, #1
    cmp r5, r7
    bhi .L0802523A
    mov r4, sp
.L0802521A:
    ldrh r0, [r4]
    adds r0, r0, r6
    strh r0, [r4]
    ldrh r0, [r4, #4]
    adds r0, r0, r6
    strh r0, [r4, #4]
    mov r0, r8
    mov r1, sp
    bl func_080AC070
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802523A
    adds r5, #1
    cmp r5, r7
    bls .L0802521A
.L0802523A:
    adds r0, r5, #0
    add sp, #8
    pop {r3}
    mov r8, r3
    pop {r4, r5, r6, r7}
    pop {r1}
    bx r1





    .section .text.entities_part1.0802405C, "ax", %progbits
    thumb_func_start func_0802405C
func_0802405C: @ 0x0802405C
    push {r4, r5, r6, r7, lr}
    adds r5, r0, #0
    adds r4, r1, #0
    movs r6, #0
    ldr r0, [r5, #0x34]
    bl IsPregnant__C10BarnAnimal
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802407C
    ldr r0, [r5, #0x34]
    bl GetDaysPregnant__C10BarnAnimal
    rsbs r1, r0, #0
    orrs r1, r0
    lsrs r6, r1, #0x1f
.L0802407C:
    ldr r7, .L0802408C @ =gUnk_080F0E34
    lsls r4, r4, #1
    cmp r6, #0
    beq .L08024090
    adds r0, r4, #0
    adds r0, #0x2a
    b .L080240B0
    .align 2, 0
.L0802408C: .4byte gUnk_080F0E34
.L08024090:
    ldr r0, [r5, #0x34]
    bl IsSheared__C5Sheep
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L080240A2
    adds r0, r4, #0
    adds r0, #0x1c
    b .L080240B0
.L080240A2:
    ldr r0, [r5, #0x34]
    bl GetGrowthStage__C5Sheep
    lsls r1, r0, #3
    subs r1, r1, r0
    lsls r1, r1, #1
    adds r0, r4, r1
.L080240B0:
    adds r0, r7, r0
    ldrh r0, [r0]
    pop {r4, r5, r6, r7}
    pop {r1}
    bx r1
    .align 2, 0
    .section .text.entities_part1.080200C4, "ax", %progbits
    thumb_func_start func_080200C4
func_080200C4: @ 0x080200C4
    push {r4, r5, r6, r7, lr}
    mov r7, r8
    push {r7}
    adds r4, r0, #0
    mov r8, r1
    adds r6, r4, #0
    adds r6, #0x30
    ldrb r0, [r6]
    cmp r0, #0
    bne .L08020126
    adds r0, r4, #0
    bl AEntity_IsOnCurrentMap
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08020126
    ldr r0, [r4]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    adds r7, r6, #0
    mov r0, r8
    lsls r6, r0, #0x10
    cmp r4, r5
    beq .L08020118
.L08020102:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08020112
    ldr r0, [r4]
    b .L0802011C
.L08020112:
    adds r4, #4
    cmp r4, r5
    bne .L08020102
.L08020118:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802011C:
    lsrs r1, r6, #0x10
    bl func_08008B6C
    movs r0, #1
    strb r0, [r7]
.L08020126:
    pop {r3}
    mov r8, r3
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0
    .section .text.entities_part1.080202C8.tail, "ax", %progbits
.L080202CC:
    .byte 0x00, 0x20, 0x70, 0x47
    .byte 0x00, 0x20, 0x70, 0x47, 0x00, 0x20, 0x70, 0x47, 0x00, 0x20, 0x70, 0x47, 0x00, 0x20, 0x70, 0x47
    .byte 0x00, 0x20, 0x70, 0x47, 0x00, 0x20, 0x70, 0x47, 0x00, 0x20, 0x70, 0x47, 0x70, 0x47, 0x00, 0x00
    .byte 0x70, 0x47, 0x00, 0x00, 0x70, 0x47, 0x00, 0x00, 0x70, 0x47, 0x00, 0x00, 0x05, 0x20, 0x70, 0x47
    .byte 0x05, 0x20, 0x70, 0x47, 0x70, 0x47, 0x00, 0x00, 0x70, 0x47, 0x00, 0x00, 0x70, 0x47, 0x00, 0x00

    .section .text.entities_part1.08020310.tail, "ax", %progbits
.L080203A4:
    .byte 0x70, 0xB5, 0x82, 0xB0, 0x05, 0x1C, 0x0E, 0x1C, 0x16, 0x48, 0x68, 0x61
    .byte 0x28, 0x1C, 0x40, 0x30, 0x00, 0x78, 0x00, 0x28, 0x07, 0xD0, 0x3C, 0x21, 0x68, 0x5E, 0x00, 0x04
    .byte 0xA8, 0x60, 0x3E, 0x21, 0x68, 0x5E, 0x00, 0x04, 0xE8, 0x60, 0x6C, 0x6B, 0x68, 0x46, 0x29, 0x1C
    .byte 0x11, 0xF0, 0xCE, 0xFE, 0x20, 0x1C, 0x69, 0x46, 0x7A, 0xF0, 0xA4, 0xFE, 0x0B, 0x48, 0x68, 0x61
    .byte 0x29, 0x69, 0x00, 0x29, 0x05, 0xD0, 0x48, 0x68, 0x82, 0x68, 0x08, 0x1C, 0x03, 0x21, 0xB3, 0xF0
    .byte 0x91, 0xFA, 0x01, 0x20, 0x30, 0x40, 0x00, 0x28, 0x02, 0xD0, 0x28, 0x1C, 0xE0, 0xF7, 0x04, 0xF9
    .byte 0x02, 0xB0, 0x70, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0xC8, 0x64, 0x0E, 0x08, 0x50, 0x62, 0x0E, 0x08


    .section .text.entities_part1.08020460.tail, "ax", %progbits
.L08020474:
    .byte 0x10, 0xB5, 0x04, 0x1C, 0x00, 0xF0, 0x28, 0xF8, 0x21, 0x1C, 0x30, 0x31
    .byte 0x00, 0x20, 0x08, 0x70, 0xA0, 0x8C, 0x00, 0x28, 0x01, 0xD0, 0x01, 0x38, 0x00, 0xE0, 0xE0, 0x8C
    .byte 0xA0, 0x84, 0x22, 0x69, 0x00, 0x2A, 0x04, 0xD0, 0x50, 0x68, 0xC1, 0x68, 0x10, 0x1C, 0xB3, 0xF0
    .byte 0x37, 0xFA, 0x10, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x30, 0xB5, 0x04, 0x1C, 0x0D, 0x1C, 0x11, 0xF0
    .byte 0x5F, 0xFF, 0x00, 0x06, 0x00, 0x28, 0x02, 0xD0, 0x20, 0x1C, 0x00, 0xF0, 0x07, 0xF8, 0x20, 0x1C
    .byte 0x29, 0x1C, 0x11, 0xF0, 0xB3, 0xFF, 0x30, 0xBC, 0x01, 0xBC, 0x00, 0x47

    .section .text.entities_part1.080204CC, "ax", %progbits
    thumb_func_start func_080204CC
func_080204CC: @ 0x080204CC
    push {r4, r5, r6, r7, lr}
    mov r7, sl
    mov r6, sb
    mov r5, r8
    push {r5, r6, r7}
    sub sp, #0xfc
    adds r6, r0, #0
    ldr r0, [r6]
    str r0, [sp, #0xd8]
    ldr r1, [r6, #0x34]
    mov sl, r1
    mov r0, sl
    bl GetGrowthStage__C3Dog
    adds r5, r0, #0
    ldrh r2, [r6, #4]
    str r2, [sp, #0xdc]
    ldr r3, [sp, #0xd8]
    ldr r1, [r3]
    add r0, sp, #8
    ldr r3, [r1, #0x34]
    ldr r1, [sp, #0xd8]
    bl _call_via_r3
    ldr r1, [r6, #0x14]
    add r4, sp, #0x7c
    adds r0, r4, #0
    ldr r2, [r1, #0xc]
    adds r1, r6, #0
    bl _call_via_r2
    movs r1, #0
    movs r0, #0
    mov r8, r4
    ldr r4, [sp, #0xdc]
    cmp r4, #8
    bgt .L08020518
    movs r0, #1
.L08020518:
    cmp r0, #0
    beq .L08020522
    cmp r5, #1
    bne .L08020522
    movs r1, #1
.L08020522:
    adds r2, r1, #0
    cmp r2, #0
    beq .L0802053C
    mov r7, r8
    movs r1, #4
    ldrsh r0, [r7, r1]
    add r1, sp, #0x7c
    movs r3, #0
    ldrsh r1, [r1, r3]
    subs r0, r0, r1
    adds r0, #0x10
    str r0, [sp, #0xe0]
    b .L08020540
.L0802053C:
    movs r4, #0
    str r4, [sp, #0xe0]
.L08020540:
    cmp r2, #0
    beq .L08020556
    mov r7, r8
    movs r1, #6
    ldrsh r0, [r7, r1]
    movs r2, #2
    ldrsh r1, [r7, r2]
    subs r0, r0, r1
    adds r0, #0x10
    str r0, [sp, #0xe4]
    b .L0802055A
.L08020556:
    movs r3, #0
    str r3, [sp, #0xe4]
.L0802055A:
    add r4, sp, #0x14
    movs r7, #0x21
    str r7, [sp, #0x14]
    movs r0, #0x21
    rsbs r0, r0, #0
    str r0, [r4, #4]
    str r0, [r4, #8]
    str r7, [r4, #0xc]
    add r1, sp, #0x28
    add r0, sp, #8
    ldm r0!, {r2, r3, r7}
    stm r1!, {r2, r3, r7}
    ldr r0, [sp, #0x7c]
    ldr r1, [sp, #0x80]
    str r0, [sp, #0x34]
    str r1, [sp, #0x38]
    movs r0, #0x20
    str r0, [r4, #0x28]
    ldr r1, [sp, #0xe0]
    str r1, [sp, #0x40]
    ldr r2, [sp, #0xe4]
    str r2, [sp, #0x44]
    ldr r3, [sp, #0xd8]
    ldr r0, [r3]
    ldr r2, [r0, #0x40]
    adds r0, r3, #0
    movs r1, #0
    bl _call_via_r2
    mov sb, r0
    ldr r7, [sp, #0xd8]
    ldr r0, [r7]
    ldr r2, [r0, #0x40]
    adds r0, r7, #0
    movs r1, #0x4a
    bl _call_via_r2
    adds r7, r0, #0
    movs r0, #0
    str r0, [sp, #0xe8]
    adds r0, r6, #0
    adds r0, #0x40
    ldrb r1, [r0]
    str r0, [sp, #0xf8]
    cmp r1, #0
    bne .L080205B8
    b .L080206EE
.L080205B8:
    movs r1, #0
    ldrh r0, [r6, #0x24]
    cmp r0, #0
    bne .L080205C2
    movs r1, #1
.L080205C2:
    cmp r1, #0
    bne .L080205C8
    b .L080206EE
.L080205C8:
    add r1, sp, #0xe8
    ldrb r2, [r1]
    ldr r1, [sp, #0xf8]
    strb r2, [r1]
    movs r2, #0x3c
    ldrsh r0, [r6, r2]
    lsls r0, r0, #0x10
    str r0, [r6, #8]
    movs r3, #0x3e
    ldrsh r0, [r6, r3]
    lsls r0, r0, #0x10
    str r0, [r6, #0xc]
    ldr r1, [r6, #0x14]
    add r4, sp, #0x84
    adds r0, r4, #0
    ldr r2, [r1, #0xc]
    adds r1, r6, #0
    bl _call_via_r2
    ldr r0, [r4]
    ldr r1, [r4, #4]
    str r0, [sp, #0x7c]
    str r1, [sp, #0x80]
    mov r4, r8
    movs r0, #4
    ldrsh r5, [r4, r0]
    movs r1, #0
    ldrsh r0, [r4, r1]
    subs r5, r5, r0
    adds r5, #0x10
    mov ip, r5
    movs r2, #6
    ldrsh r4, [r4, r2]
    mov r3, r8
    movs r5, #2
    ldrsh r0, [r3, r5]
    subs r4, r4, r0
    adds r4, #0x10
    mov r8, r4
    add r1, sp, #0x48
    movs r0, #0x21
    str r0, [sp, #0x48]
    movs r2, #0x21
    rsbs r2, r2, #0
    str r2, [r1, #4]
    str r2, [r1, #8]
    str r0, [r1, #0xc]
    add r2, sp, #0x5c
    add r0, sp, #8
    ldm r0!, {r3, r4, r5}
    stm r2!, {r3, r4, r5}
    ldr r2, [sp, #0x7c]
    ldr r3, [sp, #0x80]
    str r2, [sp, #0x68]
    str r3, [sp, #0x6c]
    movs r0, #0x20
    str r0, [r1, #0x28]
    mov r2, ip
    str r2, [sp, #0x74]
    mov r3, r8
    str r3, [sp, #0x78]
    add r0, sp, #0x14
    movs r2, #0x34
    bl memcpy
    mov r4, sb
    cmp r4, #0
    beq .L08020670
    ldrh r0, [r4, #4]
    ldr r5, [sp, #0xdc]
    cmp r0, r5
    bne .L08020670
    ldr r1, [r4, #0x14]
    add r4, sp, #0x8c
    adds r0, r4, #0
    ldr r2, [r1, #0xc]
    mov r1, sb
    bl _call_via_r2
    add r0, sp, #0x14
    adds r1, r4, #0
    movs r2, #1
    bl func_080ABA90
.L08020670:
    cmp r7, #0
    beq .L08020694
    ldrh r0, [r7, #4]
    ldr r1, [sp, #0xdc]
    cmp r0, r1
    bne .L08020694
    ldr r1, [r7, #0x14]
    add r4, sp, #0x94
    adds r0, r4, #0
    ldr r2, [r1, #0xc]
    adds r1, r7, #0
    bl _call_via_r2
    add r0, sp, #0x14
    adds r1, r4, #0
    movs r2, #0
    bl func_080ABA90
.L08020694:
    adds r0, r6, #0
    bl DogEntity_GetAnimationState
    adds r1, r0, #0
    adds r4, r6, #0
    adds r4, #0x20
    ldrb r2, [r4]
    adds r0, r6, #0
    bl DogEntity_SetAnimationState
    mov r2, sl
    ldr r0, [r2, #0x20]
    cmp r0, #5
    beq .L080206B4
    bl .L080210A0
.L080206B4:
    ldr r3, [sp, #0xe8]
    cmp r3, #0
    bne .L080206D2
    ldr r5, [sp, #0xd8]
    ldr r0, [r5]
    ldr r2, [r0, #0x40]
    adds r0, r5, #0
    movs r1, #0x4b
    bl _call_via_r2
    str r0, [sp, #0xe8]
    cmp r0, #0
    bne .L080206D2
    bl .L080210A0
.L080206D2:
    ldr r7, [sp, #0xe8]
    ldrh r0, [r7, #4]
    ldr r1, [sp, #0xdc]
    cmp r0, r1
    beq .L080206E0
    bl .L080210A0
.L080206E0:
    ldrh r1, [r6, #0x22]
    ldrb r2, [r4]
    adds r0, r7, #0
    bl func_08038398
    bl .L080210A0
.L080206EE:
    mov r2, sb
    cmp r2, #0
    beq .L08020714
    ldrh r0, [r2, #4]
    ldr r3, [sp, #0xdc]
    cmp r0, r3
    bne .L08020714
    ldr r1, [r2, #0x14]
    add r4, sp, #0x9c
    adds r0, r4, #0
    ldr r2, [r1, #0xc]
    mov r1, sb
    bl _call_via_r2
    add r0, sp, #0x14
    adds r1, r4, #0
    movs r2, #1
    bl func_080ABA90
.L08020714:
    cmp r7, #0
    beq .L08020738
    ldrh r0, [r7, #4]
    ldr r4, [sp, #0xdc]
    cmp r0, r4
    bne .L08020738
    ldr r1, [r7, #0x14]
    add r4, sp, #0xa4
    adds r0, r4, #0
    ldr r2, [r1, #0xc]
    adds r1, r7, #0
    bl _call_via_r2
    add r0, sp, #0x14
    adds r1, r4, #0
    movs r2, #0
    bl func_080ABA90
.L08020738:
    ldr r7, [sp, #0xf8]
    ldrb r0, [r7]
    cmp r0, #0
    beq .L08020742
    b .L0802089A
.L08020742:
    cmp r5, #1
    beq .L08020748
    b .L0802089A
.L08020748:
    movs r1, #0xa
    ldrsh r0, [r6, r1]
    str r0, [sp, #0xec]
    movs r3, #0xe
    ldrsh r2, [r6, r3]
    str r2, [sp, #0xf0]
    ldr r4, [r6, #0x18]
    mov r8, r4
    ldr r7, [r6, #0x1c]
    adds r4, r0, #0
    adds r5, r2, #0
    cmp r7, #0
    ble .L08020794
    ldr r1, [sp, #0x14]
    ldr r0, [sp, #0x3c]
    cmp r1, r0
    ble .L08020774
    add r0, sp, #0x14
    bl func_080AB85C
    ldr r0, [sp, #0x14]
    b .L08020776
.L08020774:
    adds r0, r1, #0
.L08020776:
    cmp r0, #0
    bne .L08020794
    ldr r1, [sp, #0x14]
    ldr r0, [sp, #0x3c]
    cmp r1, r0
    ble .L08020788
    add r0, sp, #0x14
    bl func_080AB85C
.L08020788:
    add r1, sp, #0x24
    ldrb r0, [r1]
    cmp r0, #0
    beq .L08020794
    ldr r2, [sp, #0xe4]
    adds r5, r5, r2
.L08020794:
    cmp r7, #0
    bge .L080207CC
    ldr r1, [sp, #0x18]
    ldr r0, [sp, #0x3c]
    cmn r1, r0
    bge .L080207AA
    add r0, sp, #0x14
    bl func_080AB8D0
    ldr r0, [sp, #0x18]
    b .L080207AC
.L080207AA:
    adds r0, r1, #0
.L080207AC:
    cmp r0, #0
    bne .L080207CC
    ldr r1, [sp, #0x3c]
    ldr r0, [sp, #0x18]
    cmn r0, r1
    bge .L080207BE
    add r0, sp, #0x14
    bl func_080AB8D0
.L080207BE:
    movs r3, #0x25
    add r3, sp
    ldrb r0, [r3]
    cmp r0, #0
    beq .L080207CC
    ldr r7, [sp, #0xe4]
    subs r5, r5, r7
.L080207CC:
    mov r0, r8
    cmp r0, #0
    bge .L08020806
    ldr r1, [sp, #0x1c]
    ldr r0, [sp, #0x3c]
    cmn r1, r0
    bge .L080207E4
    add r0, sp, #0x14
    bl func_080AB948
    ldr r0, [sp, #0x1c]
    b .L080207E6
.L080207E4:
    adds r0, r1, #0
.L080207E6:
    cmp r0, #0
    bne .L08020806
    ldr r1, [sp, #0x3c]
    ldr r0, [sp, #0x1c]
    cmn r0, r1
    bge .L080207F8
    add r0, sp, #0x14
    bl func_080AB948
.L080207F8:
    movs r1, #0x26
    add r1, sp
    ldrb r0, [r1]
    cmp r0, #0
    beq .L08020806
    ldr r2, [sp, #0xe0]
    subs r4, r4, r2
.L08020806:
    mov r3, r8
    cmp r3, #0
    ble .L08020840
    ldr r1, [sp, #0x20]
    ldr r0, [sp, #0x3c]
    cmp r1, r0
    ble .L0802081E
    add r0, sp, #0x14
    bl func_080AB9C4
    ldr r0, [sp, #0x20]
    b .L08020820
.L0802081E:
    adds r0, r1, #0
.L08020820:
    cmp r0, #0
    bne .L08020840
    ldr r1, [sp, #0x20]
    ldr r0, [sp, #0x3c]
    cmp r1, r0
    ble .L08020832
    add r0, sp, #0x14
    bl func_080AB9C4
.L08020832:
    movs r7, #0x27
    add r7, sp
    ldrb r0, [r7]
    cmp r0, #0
    beq .L08020840
    ldr r0, [sp, #0xe0]
    adds r4, r4, r0
.L08020840:
    ldr r1, [sp, #0xec]
    cmp r4, r1
    bne .L0802084C
    ldr r2, [sp, #0xf0]
    cmp r5, r2
    beq .L0802089A
.L0802084C:
    strh r4, [r6, #0x3c]
    strh r5, [r6, #0x3e]
    movs r0, #1
    ldr r3, [sp, #0xf8]
    strb r0, [r3]
    adds r4, r6, #0
    adds r4, #0x20
    ldrb r2, [r4]
    adds r0, r6, #0
    movs r1, #0xc
    bl DogEntity_SetAnimationState
    mov r5, sl
    ldr r0, [r5, #0x20]
    cmp r0, #5
    bne .L0802089A
    ldr r7, [sp, #0xe8]
    cmp r7, #0
    bne .L08020886
    ldr r1, [sp, #0xd8]
    ldr r0, [r1]
    ldr r2, [r0, #0x40]
    adds r0, r1, #0
    movs r1, #0x4b
    bl _call_via_r2
    str r0, [sp, #0xe8]
    cmp r0, #0
    beq .L0802089A
.L08020886:
    ldr r2, [sp, #0xe8]
    ldrh r0, [r2, #4]
    ldr r3, [sp, #0xdc]
    cmp r0, r3
    bne .L0802089A
    ldrh r1, [r6, #0x22]
    ldrb r2, [r4]
    ldr r0, [sp, #0xe8]
    bl func_08038398
.L0802089A:
    mov r4, sl
    ldr r0, [r4, #0x20]
    cmp r0, #6
    bls .L080208A6
    bl .L080210A0
.L080208A6:
    lsls r0, r0, #2
    ldr r1, .L080208B0 @ =.L080208B4
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L080208B0: .4byte .L080208B4
.L080208B4: @ jump table
    .4byte .L080208D0 @ case 0
    .4byte .L08020A54 @ case 1
    .4byte .L08020B38 @ case 2
    .4byte .L08020C14 @ case 3
    .4byte .L08020D38 @ case 4
    .4byte .L08020DE4 @ case 5
    .4byte .L08020F0C @ case 6
.L080208D0:
    movs r5, #0x24
    add r5, sl
    mov sb, r5
    mov r7, sl
    ldrh r4, [r7, #0x24]
    cmp r4, #0
    beq .L08020968
    ldr r5, [r6, #0x18]
    ldr r7, [r6, #0x1c]
    movs r0, #0
    mov r8, r0
    cmp r5, #0
    bge .L08020902
    ldr r1, [sp, #0x1c]
    ldr r0, [sp, #0x3c]
    cmn r1, r0
    bge .L080208FA
    add r0, sp, #0x14
    bl func_080AB948
    ldr r1, [sp, #0x1c]
.L080208FA:
    movs r0, #0x10
    rsbs r0, r0, #0
    cmp r1, r0
    bgt .L08020956
.L08020902:
    cmp r5, #0
    ble .L0802091E
    ldr r1, [sp, #0x20]
    ldr r0, [sp, #0x3c]
    cmp r1, r0
    ble .L08020918
    add r0, sp, #0x14
    bl func_080AB9C4
    ldr r0, [sp, #0x20]
    b .L0802091A
.L08020918:
    adds r0, r1, #0
.L0802091A:
    cmp r0, #0xf
    ble .L08020956
.L0802091E:
    cmp r7, #0
    bge .L0802093A
    ldr r1, [sp, #0x18]
    ldr r0, [sp, #0x3c]
    cmn r1, r0
    bge .L08020932
    add r0, sp, #0x14
    bl func_080AB8D0
    ldr r1, [sp, #0x18]
.L08020932:
    movs r0, #0x10
    rsbs r0, r0, #0
    cmp r1, r0
    bgt .L08020956
.L0802093A:
    cmp r7, #0
    ble .L0802095A
    ldr r1, [sp, #0x14]
    ldr r0, [sp, #0x3c]
    cmp r1, r0
    ble .L08020950
    add r0, sp, #0x14
    bl func_080AB85C
    ldr r0, [sp, #0x14]
    b .L08020952
.L08020950:
    adds r0, r1, #0
.L08020952:
    cmp r0, #0xf
    bgt .L0802095A
.L08020956:
    movs r1, #1
    mov r8, r1
.L0802095A:
    mov r2, r8
    cmp r2, #0
    beq .L08020964
    movs r4, #0
    b .L08020A48
.L08020964:
    subs r4, #1
    b .L08020A48
.L08020968:
    ldr r3, [sp, #0xf8]
    ldrb r0, [r3]
    cmp r0, #0
    bne .L08020A48
    adds r0, r6, #0
    bl AEntity_IsAnimationFinished
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08020A48
    mov r0, sl
    bl GetGrowthStage__C3Dog
    cmp r0, #1
    bne .L080209C8
    ldr r4, [sp, #0xd8]
    ldr r0, [r4]
    ldr r2, [r0, #0x40]
    adds r0, r4, #0
    movs r1, #0x2d
    bl _call_via_r2
    cmp r0, #0
    beq .L080209C8
    ldrh r0, [r0, #4]
    ldr r5, [sp, #0xdc]
    cmp r0, r5
    bne .L080209C8
    adds r0, r6, #0
    movs r1, #0xa9
    bl func_080200C4
    ldr r2, .L080209C0 @ =0xFFFF0000
    add r1, sp, #0xac
    ldr r0, [r1]
    ands r0, r2
    ldr r2, .L080209C4 @ =0xFF00FFFF
    ands r0, r2
    str r0, [r1]
    mov r0, sl
    bl method_0809BB9C__3DogPC15UnkBarnAnimal2C
    b .L08020FD6
    .align 2, 0
.L080209C0: .4byte 0xFFFF0000
.L080209C4: .4byte 0xFF00FFFF
.L080209C8:
    ldr r7, [sp, #0xd8]
    ldr r0, [r7]
    movs r1, #0xa2
    lsls r1, r1, #1
    adds r0, r0, r1
    ldr r1, [r0]
    adds r0, r7, #0
    bl _call_via_r1
    ldrb r0, [r0, #0xa]
    lsls r0, r0, #0x1b
    lsrs r0, r0, #0x1b
    movs r4, #0
    subs r0, #6
    cmp r0, #0xe
    bls .L080209EA
    movs r4, #1
.L080209EA:
    mov r0, sl
    bl GetGrowthStage__C3Dog
    movs r1, #0x68
    muls r4, r1, r4
    movs r1, #0x34
    muls r0, r1, r0
    ldr r1, .L08020A50 @ =gUnk_080F0838
    adds r0, r0, r1
    adds r4, r4, r0
    adds r0, r4, #0
    movs r1, #0xd
    bl func_080AB82C
    adds r7, r0, #0
    lsls r0, r7, #2
    adds r4, r4, r0
    movs r5, #0x3c
    ldrb r0, [r4, #2]
    muls r0, r5, r0
    bl func_080AB788
    ldrb r1, [r4, #1]
    muls r1, r5, r1
    adds r4, r1, r0
    adds r0, r6, #0
    adds r0, #0x20
    ldrb r5, [r0]
    movs r0, #4
    bl func_080AB788
    adds r1, r0, #0
    cmp r1, #0
    beq .L08020A3A
    cmp r1, #3
    beq .L08020A3A
    adds r0, r5, #0
    bl func_080AB7A4
    adds r5, r0, #0
.L08020A3A:
    mov r2, sb
    strb r7, [r2, #2]
    adds r0, r6, #0
    adds r1, r7, #0
    adds r2, r5, #0
    bl DogEntity_SetAnimationState
.L08020A48:
    mov r3, sb
    strh r4, [r3]
    b .L080210A0
    .align 2, 0
.L08020A50: .4byte gUnk_080F0838
.L08020A54:
    mov r4, sl
    adds r4, #0x24
    mov r7, sl
    ldrh r5, [r7, #0x24]
    cmp r5, #0
    beq .L08020A62
    subs r5, #1
.L08020A62:
    ldrh r2, [r4, #2]
    cmp r2, #0
    beq .L08020A6A
    subs r2, #1
.L08020A6A:
    ldr r1, [sp, #0xf8]
    ldrb r0, [r1]
    cmp r0, #0
    beq .L08020A7A
    mov r3, sl
    strh r5, [r3, #0x24]
    strh r2, [r4, #2]
    b .L080210A0
.L08020A7A:
    cmp r5, #0
    beq .L08020B1A
    mov r7, sb
    cmp r7, #0
    beq .L08020B1A
    ldrh r0, [r7, #4]
    ldr r1, [sp, #0xdc]
    cmp r0, r1
    bne .L08020B1A
    movs r7, #0xa
    ldrsh r3, [r6, r7]
    movs r0, #0xe
    ldrsh r1, [r6, r0]
    mov r7, sb
    movs r0, #0xa
    ldrsh r7, [r7, r0]
    mov r8, r7
    mov r7, sb
    movs r0, #0xe
    ldrsh r7, [r7, r0]
    mov sb, r7
    mov r7, r8
    subs r0, r7, r3
    cmp r0, #0
    bge .L08020AAE
    rsbs r0, r0, #0
.L08020AAE:
    cmp r0, #0xf
    bgt .L08020AC0
    mov r7, sb
    subs r0, r7, r1
    cmp r0, #0
    bge .L08020ABC
    rsbs r0, r0, #0
.L08020ABC:
    cmp r0, #0xf
    ble .L08020B1A
.L08020AC0:
    cmp r2, #0
    beq .L08020AE4
    movs r7, #4
    ldrsh r0, [r4, r7]
    subs r0, r0, r3
    cmp r0, #0
    bge .L08020AD0
    rsbs r0, r0, #0
.L08020AD0:
    cmp r0, #0xf
    bgt .L08020B14
    movs r7, #6
    ldrsh r0, [r4, r7]
    subs r0, r0, r1
    cmp r0, #0
    bge .L08020AE0
    rsbs r0, r0, #0
.L08020AE0:
    cmp r0, #0xf
    bgt .L08020B14
.L08020AE4:
    adds r0, r6, #0
    adds r0, #0x20
    ldrb r0, [r0]
    add r2, sp, #0x14
    str r2, [sp]
    str r0, [sp, #4]
    adds r0, r3, #0
    mov r2, r8
    mov r3, sb
    bl func_080ABC40
    adds r2, r0, #0
    adds r0, r6, #0
    movs r1, #2
    bl DogEntity_SetAnimationState
    movs r0, #0x1e
    bl func_080AB788
    adds r2, r0, #1
    mov r3, r8
    strh r3, [r4, #4]
    mov r7, sb
    strh r7, [r4, #6]
.L08020B14:
    strh r5, [r4]
    strh r2, [r4, #2]
    b .L080210A0
.L08020B1A:
    ldr r2, .L08020B30 @ =0xFFFF0000
    add r1, sp, #0xb0
    ldr r0, [r1]
    ands r0, r2
    movs r2, #0xb4
    orrs r0, r2
    ldr r2, .L08020B34 @ =0xFF00FFFF
    ands r0, r2
    str r0, [r1]
    b .L08020FD0
    .align 2, 0
.L08020B30: .4byte 0xFFFF0000
.L08020B34: .4byte 0xFF00FFFF
.L08020B38:
    mov r7, sl
    adds r7, #0x24
    mov r0, sl
    ldrh r0, [r0, #0x24]
    mov r8, r0
    cmp r0, #0
    beq .L08020B4C
    movs r1, #1
    rsbs r1, r1, #0
    add r8, r1
.L08020B4C:
    mov r2, sl
    ldr r1, [r2, #0x24]
    lsls r0, r1, #9
    lsrs r5, r0, #0x19
    cmp r5, #0
    beq .L08020B5A
    subs r5, #1
.L08020B5A:
    ldrb r4, [r7, #3]
    cmp r4, #0
    beq .L08020B62
    subs r4, #1
.L08020B62:
    ldr r3, [sp, #0xf8]
    ldrb r0, [r3]
    cmp r0, #0
    beq .L08020B72
    mov r1, r8
    mov r0, sl
    strh r1, [r0, #0x24]
    b .L08020BDC
.L08020B72:
    mov r2, r8
    cmp r2, #0
    beq .L08020BF0
    lsls r0, r1, #8
    cmp r0, #0
    bge .L08020B90
    ldr r3, [sp, #0xd8]
    ldr r0, [r3]
    ldr r2, [r0, #0x40]
    adds r0, r3, #0
    movs r1, #0x2d
    bl _call_via_r2
    adds r3, r0, #0
    b .L08020B92
.L08020B90:
    mov r3, sb
.L08020B92:
    cmp r3, #0
    beq .L08020BF0
    ldrh r0, [r3, #4]
    ldr r1, [sp, #0xdc]
    cmp r0, r1
    bne .L08020BF0
    cmp r4, #0
    bne .L08020BD8
    movs r2, #0xa
    ldrsh r0, [r6, r2]
    movs r4, #0xe
    ldrsh r1, [r6, r4]
    movs r4, #0xa
    ldrsh r2, [r3, r4]
    movs r4, #0xe
    ldrsh r3, [r3, r4]
    adds r4, r6, #0
    adds r4, #0x20
    ldrb r4, [r4]
    mov sb, r4
    add r4, sp, #0x14
    str r4, [sp]
    mov r4, sb
    str r4, [sp, #4]
    bl func_080ABE58
    adds r2, r0, #0
    adds r0, r6, #0
    movs r1, #2
    bl DogEntity_SetAnimationState
    movs r0, #0x1e
    bl func_080AB788
    adds r4, r0, #1
.L08020BD8:
    mov r0, r8
    strh r0, [r7]
.L08020BDC:
    movs r0, #0x7f
    ands r5, r0
    ldrb r1, [r7, #2]
    movs r0, #0x80
    rsbs r0, r0, #0
    ands r0, r1
    orrs r0, r5
    strb r0, [r7, #2]
    strb r4, [r7, #3]
    b .L080210A0
.L08020BF0:
    adds r0, r6, #0
    bl func_080323C8
    ldr r2, .L08020C0C @ =0xFFFF0000
    add r1, sp, #0xb4
    ldr r0, [r1]
    ands r0, r2
    movs r2, #0xb4
    orrs r0, r2
    ldr r2, .L08020C10 @ =0xFF00FFFF
    ands r0, r2
    str r0, [r1]
    b .L08020FD0
    .align 2, 0
.L08020C0C: .4byte 0xFFFF0000
.L08020C10: .4byte 0xFF00FFFF
.L08020C14:
    mov r5, sl
    adds r5, #0x24
    mov r1, sl
    ldrh r7, [r1, #0x24]
    cmp r7, #0
    beq .L08020C22
    subs r7, #1
.L08020C22:
    ldrh r4, [r5, #2]
    cmp r4, #0
    beq .L08020C2A
    subs r4, #1
.L08020C2A:
    ldr r2, [sp, #0xf8]
    ldrb r0, [r2]
    cmp r0, #0
    beq .L08020C3A
    mov r3, sl
    strh r7, [r3, #0x24]
    strh r4, [r5, #2]
    b .L080210A0
.L08020C3A:
    cmp r7, #0
    beq .L08020D1A
    ldr r1, [sp, #0xd8]
    ldr r0, [r1]
    ldr r2, [r0, #0x40]
    adds r0, r1, #0
    movs r1, #0x4b
    bl _call_via_r2
    str r0, [sp, #0xe8]
    cmp r0, #0
    beq .L08020D08
    ldrh r0, [r0, #4]
    ldr r2, [sp, #0xdc]
    cmp r0, r2
    bne .L08020D08
    movs r3, #0xa
    ldrsh r2, [r6, r3]
    movs r0, #0xe
    ldrsh r1, [r6, r0]
    ldr r3, [sp, #0xe8]
    movs r0, #0xa
    ldrsh r3, [r3, r0]
    mov sb, r3
    ldr r3, [sp, #0xe8]
    movs r0, #0xe
    ldrsh r3, [r3, r0]
    mov r8, r3
    mov r3, sb
    subs r0, r3, r2
    cmp r0, #0
    bge .L08020C7C
    rsbs r0, r0, #0
.L08020C7C:
    cmp r0, #7
    bgt .L08020C8E
    mov r3, r8
    subs r0, r3, r1
    cmp r0, #0
    bge .L08020C8A
    rsbs r0, r0, #0
.L08020C8A:
    cmp r0, #7
    ble .L08020CE4
.L08020C8E:
    cmp r4, #0
    beq .L08020CB2
    movs r3, #4
    ldrsh r0, [r5, r3]
    subs r0, r0, r2
    cmp r0, #0
    bge .L08020C9E
    rsbs r0, r0, #0
.L08020C9E:
    cmp r0, #7
    bgt .L08020D0C
    movs r3, #6
    ldrsh r0, [r5, r3]
    subs r0, r0, r1
    cmp r0, #0
    bge .L08020CAE
    rsbs r0, r0, #0
.L08020CAE:
    cmp r0, #7
    bgt .L08020D0C
.L08020CB2:
    adds r0, r6, #0
    adds r0, #0x20
    ldrb r0, [r0]
    add r4, sp, #0x14
    str r4, [sp]
    str r0, [sp, #4]
    adds r0, r2, #0
    mov r2, sb
    mov r3, r8
    bl func_080ABC40
    adds r2, r0, #0
    adds r0, r6, #0
    movs r1, #2
    bl DogEntity_SetAnimationState
    movs r0, #0x1e
    bl func_080AB788
    adds r4, r0, #1
    mov r0, sb
    strh r0, [r5, #4]
    mov r1, r8
    strh r1, [r5, #6]
    b .L08020D0C
.L08020CE4:
    movs r0, #0x78
    bl func_080AB788
    lsls r0, r0, #0x10
    lsrs r0, r0, #0x10
    ldr r3, .L08020D04 @ =0xFFFF0000
    add r1, sp, #0xb8
    ldr r2, [r1]
    ands r2, r3
    orrs r2, r0
    str r2, [r1]
    mov r0, sl
    bl method_0809BB80__3DogPC15UnkBarnAnimal2C
    b .L08020FD6
    .align 2, 0
.L08020D04: .4byte 0xFFFF0000
.L08020D08:
    cmp r4, #0
    beq .L08020D12
.L08020D0C:
    strh r7, [r5]
    strh r4, [r5, #2]
    b .L080210A0
.L08020D12:
    adds r0, r6, #0
    movs r1, #0xaa
    bl func_080200C4
.L08020D1A:
    ldr r2, .L08020D30 @ =0xFFFF0000
    add r1, sp, #0xbc
    ldr r0, [r1]
    ands r0, r2
    movs r2, #0xb4
    orrs r0, r2
    ldr r2, .L08020D34 @ =0xFF00FFFF
    ands r0, r2
    str r0, [r1]
    b .L08020FD0
    .align 2, 0
.L08020D30: .4byte 0xFFFF0000
.L08020D34: .4byte 0xFF00FFFF
.L08020D38:
    ldr r2, [sp, #0xd8]
    ldr r0, [r2]
    ldr r2, [r0, #0x40]
    ldr r0, [sp, #0xd8]
    movs r1, #0x4b
    bl _call_via_r2
    str r0, [sp, #0xe8]
    mov r3, sl
    ldrh r1, [r3, #0x24]
    cmp r1, #0
    beq .L08020D52
    subs r1, #1
.L08020D52:
    ldr r4, [sp, #0xe8]
    cmp r4, #0
    beq .L08020DC0
    ldrh r0, [r4, #4]
    ldr r5, [sp, #0xdc]
    cmp r0, r5
    bne .L08020DC0
    cmp r1, #0
    bne .L08020DBA
    adds r0, r6, #0
    movs r1, #0xa9
    bl func_080200C4
    mov r0, sl
    bl GetAffection__C6Animal
    adds r1, r0, #0
    lsls r0, r1, #4
    subs r0, r0, r1
    lsls r0, r0, #6
    movs r1, #0xfb
    bl __udivsi3
    movs r7, #0xf0
    lsls r7, r7, #1
    adds r0, r0, r7
    lsls r0, r0, #0x10
    add r1, sp, #0xc0
    lsrs r0, r0, #0x10
    str r0, [r1]
    ldr r0, [r6, #0x34]
    bl method_0809BB8C__3DogPC18UnkBarnAnimal2C_x2
    adds r0, r6, #0
    bl DogEntity_GetAnimationState
    adds r1, r0, #0
    adds r4, r6, #0
    adds r4, #0x20
    ldrb r2, [r4]
    adds r0, r6, #0
    bl DogEntity_SetAnimationState
    ldr r1, [r6, #8]
    ldr r2, [r6, #0xc]
    ldrh r3, [r6, #0x22]
    ldrb r0, [r4]
    str r0, [sp]
    ldr r0, [sp, #0xe8]
    bl func_08038374
    b .L080210A0
.L08020DBA:
    mov r0, sl
    strh r1, [r0, #0x24]
    b .L080210A0
.L08020DC0:
    adds r0, r6, #0
    movs r1, #0xaa
    bl func_080200C4
    ldr r2, .L08020DDC @ =0xFFFF0000
    add r1, sp, #0xc8
    ldr r0, [r1]
    ands r0, r2
    movs r2, #0xb4
    orrs r0, r2
    ldr r2, .L08020DE0 @ =0xFF00FFFF
    ands r0, r2
    str r0, [r1]
    b .L08020FD0
    .align 2, 0
.L08020DDC: .4byte 0xFFFF0000
.L08020DE0: .4byte 0xFF00FFFF
.L08020DE4:
    mov r5, sl
    adds r5, #0x24
    ldr r1, [sp, #0xd8]
    ldr r0, [r1]
    ldr r2, [r0, #0x40]
    adds r0, r1, #0
    movs r1, #0x4b
    bl _call_via_r2
    str r0, [sp, #0xe8]
    mov r2, sl
    ldrh r7, [r2, #0x24]
    cmp r7, #0
    beq .L08020E02
    subs r7, #1
.L08020E02:
    ldrh r2, [r5, #2]
    cmp r2, #0
    beq .L08020E0A
    subs r2, #1
.L08020E0A:
    ldr r3, [sp, #0xf8]
    ldrb r0, [r3]
    cmp r0, #0
    beq .L08020E1A
    mov r4, sl
    strh r7, [r4, #0x24]
    strh r2, [r5, #2]
    b .L080210A0
.L08020E1A:
    ldr r0, [sp, #0xe8]
    cmp r0, #0
    beq .L08020ED8
    ldrh r0, [r0, #4]
    ldr r1, [sp, #0xdc]
    cmp r0, r1
    bne .L08020ED8
    cmp r7, #0
    beq .L08020EEA
    mov r3, sb
    cmp r3, #0
    beq .L08020EEA
    ldrh r0, [r3, #4]
    cmp r0, r1
    bne .L08020EEA
    movs r4, #0xa
    ldrsh r3, [r6, r4]
    movs r0, #0xe
    ldrsh r1, [r6, r0]
    mov r4, sb
    movs r0, #0xa
    ldrsh r4, [r4, r0]
    str r4, [sp, #0xf4]
    mov r4, sb
    movs r0, #0xe
    ldrsh r4, [r4, r0]
    mov r8, r4
    ldr r4, [sp, #0xf4]
    subs r0, r4, r3
    cmp r0, #0
    bge .L08020E5A
    rsbs r0, r0, #0
.L08020E5A:
    cmp r0, #0xf
    bgt .L08020E6C
    mov r4, r8
    subs r0, r4, r1
    cmp r0, #0
    bge .L08020E68
    rsbs r0, r0, #0
.L08020E68:
    cmp r0, #0xf
    ble .L08020ECE
.L08020E6C:
    cmp r2, #0
    beq .L08020E90
    movs r4, #4
    ldrsh r0, [r5, r4]
    subs r0, r0, r3
    cmp r0, #0
    bge .L08020E7C
    rsbs r0, r0, #0
.L08020E7C:
    cmp r0, #0xf
    bgt .L08020EDC
    movs r4, #6
    ldrsh r0, [r5, r4]
    subs r0, r0, r1
    cmp r0, #0
    bge .L08020E8C
    rsbs r0, r0, #0
.L08020E8C:
    cmp r0, #0xf
    bgt .L08020EDC
.L08020E90:
    adds r4, r6, #0
    adds r4, #0x20
    ldrb r0, [r4]
    add r2, sp, #0x14
    str r2, [sp]
    str r0, [sp, #4]
    adds r0, r3, #0
    ldr r2, [sp, #0xf4]
    mov r3, r8
    bl func_080ABC40
    adds r2, r0, #0
    adds r0, r6, #0
    movs r1, #2
    bl DogEntity_SetAnimationState
    ldrh r1, [r6, #0x22]
    ldrb r2, [r4]
    ldr r0, [sp, #0xe8]
    bl func_08038398
    movs r0, #0x1e
    bl func_080AB788
    adds r2, r0, #1
    add r3, sp, #0xf4
    ldrh r3, [r3]
    strh r3, [r5, #4]
    mov r4, r8
    strh r4, [r5, #6]
    b .L08020EDC
.L08020ECE:
    adds r0, r6, #0
    movs r1, #0xa9
    bl func_080200C4
    b .L08020EEA
.L08020ED8:
    cmp r2, #0
    beq .L08020EE2
.L08020EDC:
    strh r7, [r5]
    strh r2, [r5, #2]
    b .L080210A0
.L08020EE2:
    adds r0, r6, #0
    movs r1, #0xaa
    bl func_080200C4
.L08020EEA:
    adds r0, r6, #0
    bl DogEntity_PlaceCarriedTarget
    ldr r2, .L08020F04 @ =0xFFFF0000
    add r1, sp, #0xcc
    ldr r0, [r1]
    ands r0, r2
    movs r2, #0xb4
    orrs r0, r2
    ldr r2, .L08020F08 @ =0xFF00FFFF
    ands r0, r2
    str r0, [r1]
    b .L08020FD0
    .align 2, 0
.L08020F04: .4byte 0xFFFF0000
.L08020F08: .4byte 0xFF00FFFF
.L08020F0C:
    movs r5, #0x24
    add r5, sl
    mov r8, r5
    mov sb, r8
    mov r7, sl
    ldrh r4, [r7, #0x24]
    cmp r4, #0
    beq .L08020F1E
    subs r4, #1
.L08020F1E:
    ldr r1, [sp, #0xf8]
    ldrb r0, [r1]
    cmp r0, #0
    beq .L08020F2C
    mov r2, sl
    strh r4, [r2, #0x24]
    b .L080210A0
.L08020F2C:
    ldr r3, [sp, #0xd8]
    ldr r0, [r3]
    ldr r2, [r0, #0x40]
    adds r0, r3, #0
    movs r1, #0x2d
    bl _call_via_r2
    adds r5, r0, #0
    cmp r5, #0
    bne .L08020F42
    b .L08021074
.L08020F42:
    ldrh r0, [r5, #4]
    ldr r7, [sp, #0xdc]
    cmp r0, r7
    beq .L08020F4C
    b .L08021074
.L08020F4C:
    movs r1, #0xa
    ldrsh r0, [r6, r1]
    mov ip, r0
    movs r2, #0xe
    ldrsh r1, [r6, r2]
    movs r3, #0xa
    ldrsh r2, [r5, r3]
    movs r7, #0xe
    ldrsh r3, [r5, r7]
    mov r7, r8
    ldrb r0, [r7, #2]
    cmp r0, #0
    beq .L08021008
    cmp r4, #0
    bne .L08021002
    mov r4, ip
    subs r0, r2, r4
    cmp r0, #0
    bge .L08020F74
    rsbs r0, r0, #0
.L08020F74:
    cmp r0, #0xf
    bgt .L08020FA0
    subs r1, r3, r1
    cmp r1, #0
    bge .L08020F80
    rsbs r1, r1, #0
.L08020F80:
    cmp r1, #0xf
    bgt .L08020FA0
    ldr r0, [r5, #0x14]
    ldr r1, [r0, #0x44]
    adds r0, r5, #0
    bl _call_via_r1
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08020FA0
    ldr r0, [r5, #0x14]
    adds r0, #0x80
    ldr r1, [r0]
    adds r0, r5, #0
    bl _call_via_r1
.L08020FA0:
    movs r0, #0xfb
    bl func_080AB788
    adds r4, r0, #0
    mov r0, sl
    bl GetAdequacy__C3Pet
    movs r1, #0xfb
    subs r1, r1, r0
    cmp r4, r1
    bhs .L08020FF4
    movs r0, #0xb4
    bl func_080AB788
    lsls r0, r0, #0x10
    lsrs r0, r0, #0x10
    ldr r3, .L08020FEC @ =0xFFFF0000
    add r1, sp, #0xd0
    ldr r2, [r1]
    ands r2, r3
    orrs r2, r0
    ldr r0, .L08020FF0 @ =0xFF00FFFF
    ands r2, r0
    str r2, [r1]
.L08020FD0:
    mov r0, sl
    bl method_0809BB48__3DogPC15UnkBarnAnimal2C
.L08020FD6:
    adds r0, r6, #0
    bl DogEntity_GetAnimationState
    adds r1, r0, #0
    adds r0, r6, #0
    adds r0, #0x20
    ldrb r2, [r0]
    adds r0, r6, #0
    bl DogEntity_SetAnimationState
    b .L080210A0
    .align 2, 0
.L08020FEC: .4byte 0xFFFF0000
.L08020FF0: .4byte 0xFF00FFFF
.L08020FF4:
    movs r0, #0
    mov r5, sb
    strb r0, [r5, #2]
    movs r0, #0x1e
    bl func_080AB788
    adds r4, r0, #1
.L08021002:
    mov r7, sb
    strh r4, [r7]
    b .L080210A0
.L08021008:
    mov r5, ip
    subs r0, r2, r5
    cmp r0, #0
    bge .L08021012
    rsbs r0, r0, #0
.L08021012:
    cmp r0, #0xf
    bgt .L08021022
    subs r0, r3, r1
    cmp r0, #0
    bge .L0802101E
    rsbs r0, r0, #0
.L0802101E:
    cmp r0, #0xf
    ble .L08021050
.L08021022:
    cmp r4, #0
    bne .L0802104A
    adds r0, r6, #0
    adds r0, #0x20
    ldrb r0, [r0]
    add r7, sp, #0x14
    str r7, [sp]
    str r0, [sp, #4]
    mov r0, ip
    bl func_080ABC40
    adds r2, r0, #0
    adds r0, r6, #0
    movs r1, #2
    bl DogEntity_SetAnimationState
    movs r0, #0x1e
    bl func_080AB788
    adds r4, r0, #1
.L0802104A:
    mov r0, sb
    strh r4, [r0]
    b .L080210A0
.L08021050:
    adds r0, r6, #0
    movs r1, #0xd2
    bl func_080200C4
    adds r0, r6, #0
    adds r0, #0x20
    ldrb r2, [r0]
    adds r0, r6, #0
    movs r1, #0xb
    bl DogEntity_SetAnimationState
    movs r0, #1
    mov r1, sb
    strb r0, [r1, #2]
    movs r0, #0x24
    mov r2, sl
    strh r0, [r2, #0x24]
    b .L080210A0
.L08021074:
    ldr r2, .L08021104 @ =0xFFFF0000
    add r1, sp, #0xd4
    ldr r0, [r1]
    ands r0, r2
    movs r2, #0xb4
    orrs r0, r2
    ldr r2, .L08021108 @ =0xFF00FFFF
    ands r0, r2
    str r0, [r1]
    mov r0, sl
    bl method_0809BB48__3DogPC15UnkBarnAnimal2C
    adds r0, r6, #0
    bl DogEntity_GetAnimationState
    adds r1, r0, #0
    adds r0, r6, #0
    adds r0, #0x20
    ldrb r2, [r0]
    adds r0, r6, #0
    bl DogEntity_SetAnimationState
.L080210A0:
    ldr r0, [r6, #0x18]
    cmp r0, #0
    bne .L080210AC
    ldr r0, [r6, #0x1c]
    cmp r0, #0
    beq .L080210D6
.L080210AC:
    ldr r3, [sp, #0xdc]
    cmp r3, #2
    beq .L080210C4
    ldr r4, [sp, #0xd8]
    ldr r0, [r4]
    ldr r1, [r0, #0x14]
    adds r0, r4, #0
    bl _call_via_r1
    ldr r5, [sp, #0xdc]
    cmp r5, r0
    bne .L080210D6
.L080210C4:
    ldr r7, [sp, #0xf8]
    ldrb r0, [r7]
    add r1, sp, #0x14
    cmp r0, #0
    beq .L080210D0
    movs r1, #0
.L080210D0:
    adds r0, r6, #0
    bl AActorEntity_ApplyBoundedMovement
.L080210D6:
    mov r1, sl
    ldr r0, [r1, #0x20]
    cmp r0, #5
    bne .L080210F4
    ldr r2, [sp, #0xe8]
    cmp r2, #0
    beq .L080210F4
    ldrh r0, [r2, #4]
    ldr r3, [sp, #0xdc]
    cmp r0, r3
    bne .L080210F4
    ldr r0, [r6, #8]
    str r0, [r2, #8]
    ldr r0, [r6, #0xc]
    str r0, [r2, #0xc]
.L080210F4:
    add sp, #0xfc
    pop {r3, r4, r5}
    mov r8, r3
    mov sb, r4
    mov sl, r5
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0
    .align 2, 0
.L08021104: .4byte 0xFFFF0000
.L08021108: .4byte 0xFF00FFFF





    .section .text.entities_part1.08021130, "ax", %progbits
    thumb_func_start func_08021130
func_08021130: @ 0x08021130
    push {r4, lr}
    sub sp, #0x10
    adds r4, r0, #0
    cmp r1, #0xc
    bhi .L0802117C
    lsls r0, r1, #2
    ldr r1, .L08021144 @ =.L08021148
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L08021144: .4byte .L08021148
.L08021148: @ jump table
    .4byte .L0802117C @ case 0
    .4byte .L08021180 @ case 1
    .4byte .L08021186 @ case 2
    .4byte .L0802117C @ case 3
    .4byte .L0802117C @ case 4
    .4byte .L0802117C @ case 5
    .4byte .L0802117C @ case 6
    .4byte .L0802117C @ case 7
    .4byte .L0802117C @ case 8
    .4byte .L0802117C @ case 9
    .4byte .L0802117C @ case 10
    .4byte .L0802117C @ case 11
    .4byte .L0802118C @ case 12
.L0802117C:
    movs r0, #0
    b .L080211DA
.L08021180:
    movs r0, #0x80
    lsls r0, r0, #8
    b .L080211DA
.L08021186:
    movs r0, #0x80
    lsls r0, r0, #9
    b .L080211DA
.L0802118C:
    ldr r1, [r4, #0x14]
    mov r0, sp
    ldr r2, [r1, #0xc]
    adds r1, r4, #0
    bl _call_via_r2
    ldr r0, [sp]
    ldr r1, [sp, #4]
    str r0, [sp, #8]
    str r1, [sp, #0xc]
    add r1, sp, #8
    movs r2, #0x10
    adds r0, r4, #0
    adds r0, #0x20
    ldrb r0, [r0]
    adds r3, r1, #0
    cmp r0, #0
    blt .L080211D2
    cmp r0, #1
    ble .L080211BA
    cmp r0, #3
    ble .L080211C4
    b .L080211D2
.L080211BA:
    movs r1, #6
    ldrsh r0, [r3, r1]
    movs r2, #2
    ldrsh r1, [r3, r2]
    b .L080211CC
.L080211C4:
    movs r2, #4
    ldrsh r0, [r1, r2]
    movs r2, #0
    ldrsh r1, [r1, r2]
.L080211CC:
    subs r0, r0, r1
    adds r2, r0, #0
    adds r2, #0x10
.L080211D2:
    lsls r0, r2, #0x10
    movs r1, #0x1e
    bl __divsi3
.L080211DA:
    add sp, #0x10
    pop {r4}
    pop {r1}
    bx r1
    .align 2, 0
    .section .text.entities_part1.080212C0.tail, "ax", %progbits
.L080213A4:
    .byte 0x40, 0x30, 0x00, 0x78, 0x01, 0x21, 0x48, 0x40, 0x70, 0x47, 0x00, 0x00
    .byte 0x10, 0xB5, 0x81, 0xB0, 0x04, 0x1C, 0x40, 0x30, 0x00, 0x78, 0x00, 0x28, 0x35, 0xD1, 0x60, 0x6B
    .byte 0x00, 0x6A, 0x05, 0x28, 0x02, 0xD1, 0x20, 0x1C, 0xFF, 0xF7, 0x7A, 0xFF, 0x78, 0x20, 0x8A, 0xF0
    .byte 0xDB, 0xF9, 0xF0, 0x30, 0x00, 0x04, 0x00, 0x0C, 0x16, 0x4A, 0x00, 0x99, 0x11, 0x40, 0x01, 0x43
    .byte 0x15, 0x48, 0x01, 0x40, 0xF0, 0x20, 0x80, 0x03, 0x01, 0x43, 0x14, 0x48, 0x01, 0x40, 0x14, 0x48
    .byte 0x01, 0x40, 0x00, 0x91, 0x60, 0x6B, 0x69, 0x46, 0x7A, 0xF0, 0xB4, 0xFB, 0x20, 0x1C, 0xFF, 0xF7
    .byte 0x31, 0xFF, 0x01, 0x1C, 0x20, 0x1C, 0x20, 0x30, 0x02, 0x78, 0x20, 0x1C, 0xFF, 0xF7, 0xEA, 0xFE
    .byte 0x60, 0x6B, 0x0A, 0x21, 0x79, 0xF0, 0x62, 0xFF, 0x20, 0x1C, 0x02, 0x21, 0x00, 0x22, 0x10, 0xF0
    .byte 0xB1, 0xFF, 0x20, 0x1C, 0xAA, 0x21, 0xFE, 0xF7, 0x4D, 0xFE, 0x01, 0xB0, 0x10, 0xBC, 0x01, 0xBC
    .byte 0x00, 0x47, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF
    .byte 0xFF, 0xFF, 0xFF, 0x00


    .section .text.entities_part1.0802151C.tail, "ax", %progbits
.L080215FC:
    .byte 0x10, 0xB5, 0x0A, 0x23
    .byte 0xCA, 0x5E, 0x0E, 0x24, 0x0B, 0x5F, 0xD1, 0x1F, 0x01, 0x80, 0x19, 0x1F, 0x41, 0x80, 0x07, 0x32
    .byte 0x82, 0x80, 0x05, 0x33, 0xC3, 0x80, 0x10, 0xBC, 0x04, 0xBC, 0x10, 0x47

    .section .text.entities_part1.08021620.tail, "ax", %progbits
.L08021634:
    .byte 0x70, 0xB5, 0x46, 0x46, 0x40, 0xB4, 0x84, 0xB0, 0x05, 0x1C, 0x68, 0x6B
    .byte 0x7A, 0xF0, 0x78, 0xFA, 0x06, 0x1C, 0x8C, 0x20, 0xDE, 0xF7, 0xC4, 0xFF, 0x80, 0x46, 0x30, 0x1C
    .byte 0xFF, 0xF7, 0xE4, 0xFF, 0x04, 0x1C, 0x30, 0x1C, 0xFF, 0xF7, 0xE2, 0xFF, 0x00, 0x94, 0x01, 0x90
    .byte 0x00, 0x26, 0x02, 0x96, 0x03, 0xA8, 0x06, 0x70, 0x40, 0x46, 0x29, 0x1C, 0x02, 0x22, 0x06, 0x23
    .byte 0x10, 0xF0, 0x24, 0xFF, 0x04, 0x1C, 0xA0, 0x46, 0x41, 0x35, 0x28, 0x78, 0x08, 0x28, 0x15, 0xD1
    .byte 0x20, 0x1C, 0x70, 0x30, 0x08, 0x21, 0x3D, 0xF0, 0xEB, 0xF8, 0x20, 0x1C, 0x84, 0x30, 0x01, 0x21
    .byte 0x01, 0x70, 0x02, 0x30, 0x06, 0x70, 0x01, 0x30, 0x01, 0x70, 0x22, 0x1C, 0x8A, 0x32, 0x02, 0x23
    .byte 0x11, 0x78, 0x04, 0x20, 0x40, 0x42, 0x08, 0x40, 0x18, 0x43, 0x10, 0x70, 0x40, 0x46, 0x04, 0xB0
    .byte 0x08, 0xBC, 0x98, 0x46, 0x70, 0xBC, 0x02, 0xBC, 0x08, 0x47, 0x00, 0x00

    .section .text.entities_part1.080216BC.tail, "ax", %progbits
.L0802173C:
    .byte 0x70, 0xB5, 0x82, 0xB0
    .byte 0x05, 0x1C, 0x0E, 0x1C, 0x16, 0x48, 0x68, 0x61, 0x28, 0x1C, 0x3D, 0x30, 0x00, 0x78, 0x00, 0x28
    .byte 0x07, 0xD0, 0x38, 0x21, 0x68, 0x5E, 0x00, 0x04, 0xA8, 0x60, 0x3A, 0x21, 0x68, 0x5E, 0x00, 0x04
    .byte 0xE8, 0x60, 0x6C, 0x6B, 0x68, 0x46, 0x29, 0x1C, 0x10, 0xF0, 0x02, 0xFD, 0x20, 0x1C, 0x69, 0x46
    .byte 0x79, 0xF0, 0xD8, 0xFC, 0x0B, 0x48, 0x68, 0x61, 0x29, 0x69, 0x00, 0x29, 0x05, 0xD0, 0x48, 0x68
    .byte 0x82, 0x68, 0x08, 0x1C, 0x03, 0x21, 0xB2, 0xF0, 0xC5, 0xF8, 0x01, 0x20, 0x30, 0x40, 0x00, 0x28
    .byte 0x02, 0xD0, 0x28, 0x1C, 0xDE, 0xF7, 0x38, 0xFF, 0x02, 0xB0, 0x70, 0xBC, 0x01, 0xBC, 0x00, 0x47
    .byte 0x28, 0x64, 0x0E, 0x08, 0x50, 0x62, 0x0E, 0x08, 0x10, 0xB5, 0x81, 0xB0, 0x04, 0x1C, 0x09, 0x04
    .byte 0x09, 0x0C, 0x0C, 0x4A, 0x00, 0x98, 0x10, 0x40, 0x08, 0x43, 0x0B, 0x49, 0x08, 0x40, 0x00, 0x90
    .byte 0x60, 0x6B, 0x69, 0x46, 0x7A, 0xF0, 0x2E, 0xFA, 0x20, 0x1C, 0x00, 0xF0, 0x83, 0xFC, 0x01, 0x1C
    .byte 0x20, 0x1C, 0x20, 0x30, 0x02, 0x78, 0x20, 0x1C, 0x00, 0xF0, 0x36, 0xFC, 0x01, 0xB0, 0x10, 0xBC
    .byte 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xF0, 0xB5, 0x57, 0x46
    .byte 0x4E, 0x46, 0x45, 0x46, 0xE0, 0xB4, 0xB6, 0xB0, 0x06, 0x1C, 0x30, 0x68, 0x82, 0x46, 0x30, 0x1C
    .byte 0x3E, 0x30, 0x00, 0x78, 0x00, 0x28, 0x00, 0xD0, 0x2F, 0xE3, 0x71, 0x6B, 0x2F, 0x91, 0x08, 0x1C
    .byte 0x7A, 0xF0, 0xFE, 0xF9, 0x05, 0x1C, 0xB2, 0x88, 0x30, 0x92, 0x53, 0x46, 0x19, 0x68, 0x02, 0xA8
    .byte 0x4B, 0x6B, 0x51, 0x46, 0xB2, 0xF0, 0x78, 0xF8, 0x71, 0x69, 0x1F, 0xAC, 0x20, 0x1C, 0xCA, 0x68
    .byte 0x31, 0x1C, 0xB2, 0xF0, 0x6F, 0xF8, 0x00, 0x21, 0x00, 0x20, 0xA0, 0x46, 0x30, 0x9C, 0x08, 0x2C
    .byte 0x00, 0xDC, 0x01, 0x20, 0x00, 0x28, 0x02, 0xD0, 0x01, 0x2D, 0x00, 0xD1, 0x01, 0x21, 0x0A, 0x1C
    .byte 0x00, 0x2A, 0x09, 0xD0, 0x47, 0x46, 0x04, 0x21, 0x78, 0x5E, 0x1F, 0xA9, 0x00, 0x23, 0xC9, 0x5E
    .byte 0x40, 0x1A, 0x10, 0x30, 0x31, 0x90, 0x01, 0xE0, 0x00, 0x24, 0x31, 0x94, 0x00, 0x2A, 0x08, 0xD0
    .byte 0x47, 0x46, 0x06, 0x21, 0x78, 0x5E, 0x02, 0x22, 0xB9, 0x5E, 0x40, 0x1A, 0x10, 0x30, 0x32, 0x90
    .byte 0x01, 0xE0, 0x00, 0x23, 0x32, 0x93, 0x05, 0xAC, 0x21, 0x27, 0x05, 0x97, 0x21, 0x20, 0x40, 0x42
    .byte 0x60, 0x60, 0xA0, 0x60, 0xE7, 0x60, 0x0A, 0xA9, 0x02, 0xA8, 0x8C, 0xC8, 0x8C, 0xC1, 0x1F, 0x98
    .byte 0x20, 0x99, 0x0D, 0x90, 0x0E, 0x91, 0x20, 0x20, 0xA0, 0x62, 0x31, 0x99, 0x10, 0x91, 0x32, 0x9A
    .byte 0x11, 0x92, 0x53, 0x46, 0x18, 0x68, 0x02, 0x6C, 0x50, 0x46, 0x00, 0x21, 0xB2, 0xF0, 0x2A, 0xF8
    .byte 0x81, 0x46, 0x57, 0x46, 0x38, 0x68, 0x02, 0x6C, 0x50, 0x46, 0x4A, 0x21, 0xB2, 0xF0, 0x22, 0xF8
    .byte 0x07, 0x1C, 0x30, 0x1C, 0x3D, 0x30, 0x01, 0x78, 0x35, 0x90, 0x00, 0x29, 0x6E, 0xD0, 0x00, 0x21
    .byte 0xB0, 0x8C, 0x00, 0x28, 0x00, 0xD1, 0x01, 0x21, 0x00, 0x29, 0x67, 0xD0, 0x00, 0x20, 0x35, 0x99
    .byte 0x08, 0x70, 0x38, 0x22, 0xB0, 0x5E, 0x00, 0x04, 0xB0, 0x60, 0x3A, 0x23, 0xF0, 0x5E, 0x00, 0x04
    .byte 0xF0, 0x60, 0x71, 0x69, 0x21, 0xAC, 0x20, 0x1C, 0xCA, 0x68, 0x31, 0x1C, 0xB2, 0xF0, 0x02, 0xF8
    .byte 0x20, 0x68, 0x61, 0x68, 0x1F, 0x90, 0x20, 0x91, 0x44, 0x46, 0x04, 0x20, 0x25, 0x5E, 0x00, 0x21
    .byte 0x60, 0x5E, 0x2D, 0x1A, 0x10, 0x35, 0xAA, 0x46, 0x06, 0x22, 0xA4, 0x5E, 0x43, 0x46, 0x02, 0x25
    .byte 0x58, 0x5F, 0x24, 0x1A, 0x10, 0x34, 0xA0, 0x46, 0x12, 0xA9, 0x21, 0x20, 0x12, 0x90, 0x21, 0x22
    .byte 0x52, 0x42, 0x4A, 0x60, 0x8A, 0x60, 0xC8, 0x60, 0x17, 0xAA, 0x02, 0xA8, 0x38, 0xC8, 0x38, 0xC2
    .byte 0x1F, 0x9A, 0x20, 0x9B, 0x1A, 0x92, 0x1B, 0x93, 0x20, 0x20, 0x88, 0x62, 0x52, 0x46, 0x1D, 0x92
    .byte 0x43, 0x46, 0x1E, 0x93, 0x05, 0xA8, 0x34, 0x22, 0xB2, 0xF0, 0x14, 0xF8, 0x4C, 0x46, 0x00, 0x2C
    .byte 0x0F, 0xD0, 0xA0, 0x88, 0x30, 0x9D, 0xA8, 0x42, 0x0B, 0xD1, 0x61, 0x69, 0x23, 0xAC, 0x20, 0x1C
    .byte 0xCA, 0x68, 0x49, 0x46, 0xB1, 0xF0, 0xC6, 0xFF, 0x05, 0xA8, 0x21, 0x1C, 0x01, 0x22, 0x8A, 0xF0
    .byte 0x7F, 0xF8, 0x00, 0x2F, 0x00, 0xD1, 0xE2, 0xE1, 0xB8, 0x88, 0x30, 0x99, 0x88, 0x42, 0x00, 0xD0
    .byte 0xDD, 0xE1, 0x79, 0x69, 0x25, 0xAC, 0x20, 0x1C, 0xCA, 0x68, 0x39, 0x1C, 0xB1, 0xF0, 0xB2, 0xFF
    .byte 0x05, 0xA8, 0x21, 0x1C, 0x00, 0x22, 0x8A, 0xF0, 0x6B, 0xF8, 0xD0, 0xE1, 0x4A, 0x46, 0x00, 0x2A
    .byte 0x0F, 0xD0, 0x90, 0x88, 0x30, 0x9B, 0x98, 0x42, 0x0B, 0xD1, 0x51, 0x69, 0x27, 0xAC, 0x20, 0x1C
    .byte 0xCA, 0x68, 0x49, 0x46, 0xB1, 0xF0, 0x9E, 0xFF, 0x05, 0xA8, 0x21, 0x1C, 0x01, 0x22, 0x8A, 0xF0
    .byte 0x57, 0xF8, 0x00, 0x2F, 0x0F, 0xD0, 0xB8, 0x88, 0x30, 0x9C, 0xA0, 0x42, 0x0B, 0xD1, 0x79, 0x69
    .byte 0x29, 0xAC, 0x20, 0x1C, 0xCA, 0x68, 0x39, 0x1C, 0xB1, 0xF0, 0x8C, 0xFF, 0x05, 0xA8, 0x21, 0x1C
    .byte 0x00, 0x22, 0x8A, 0xF0, 0x45, 0xF8, 0x35, 0x9F, 0x38, 0x78, 0x00, 0x28, 0x00, 0xD0, 0x90, 0xE0
    .byte 0x01, 0x2D, 0x00, 0xD0, 0x8D, 0xE0, 0x0A, 0x21, 0x70, 0x5E, 0x33, 0x90, 0x0E, 0x23, 0xF2, 0x5E
    .byte 0x34, 0x92, 0xB4, 0x69, 0xA0, 0x46, 0xF7, 0x69, 0x04, 0x1C, 0x15, 0x1C, 0x00, 0x2F, 0x18, 0xDD
    .byte 0x05, 0x99, 0x0F, 0x98, 0x81, 0x42, 0x04, 0xDD, 0x05, 0xA8, 0x89, 0xF0, 0x0F, 0xFF, 0x05, 0x98
    .byte 0x00, 0xE0, 0x08, 0x1C, 0x00, 0x28, 0x0C, 0xD1, 0x05, 0x99, 0x0F, 0x98, 0x81, 0x42, 0x02, 0xDD
    .byte 0x05, 0xA8, 0x89, 0xF0, 0x03, 0xFF, 0x09, 0xA9, 0x08, 0x78, 0x00, 0x28, 0x01, 0xD0, 0x32, 0x9A
    .byte 0xAD, 0x18, 0x00, 0x2F, 0x19, 0xDA, 0x06, 0x99, 0x0F, 0x98, 0xC1, 0x42, 0x04, 0xDA, 0x05, 0xA8
    .byte 0x89, 0xF0, 0x2E, 0xFF, 0x06, 0x98, 0x00, 0xE0, 0x08, 0x1C, 0x00, 0x28, 0x0D, 0xD1, 0x0F, 0x99
    .byte 0x06, 0x98, 0xC8, 0x42, 0x02, 0xDA, 0x05, 0xA8, 0x89, 0xF0, 0x22, 0xFF, 0x25, 0x23, 0x6B, 0x44
    .byte 0x18, 0x78, 0x00, 0x28, 0x01, 0xD0, 0x32, 0x9F, 0xED, 0x1B, 0x40, 0x46, 0x00, 0x28, 0x19, 0xDA
    .byte 0x07, 0x99, 0x0F, 0x98, 0xC1, 0x42, 0x04, 0xDA, 0x05, 0xA8, 0x89, 0xF0, 0x4D, 0xFF, 0x07, 0x98
    .byte 0x00, 0xE0, 0x08, 0x1C, 0x00, 0x28, 0x0D, 0xD1, 0x0F, 0x99, 0x07, 0x98, 0xC8, 0x42, 0x02, 0xDA
    .byte 0x05, 0xA8, 0x89, 0xF0, 0x41, 0xFF, 0x26, 0x21, 0x69, 0x44, 0x08, 0x78, 0x00, 0x28, 0x01, 0xD0
    .byte 0x31, 0x9A, 0xA4, 0x1A, 0x43, 0x46, 0x00, 0x2B, 0x19, 0xDD, 0x08, 0x99, 0x0F, 0x98, 0x81, 0x42
    .byte 0x04, 0xDD, 0x05, 0xA8, 0x89, 0xF0, 0x6E, 0xFF, 0x08, 0x98, 0x00, 0xE0, 0x08, 0x1C, 0x00, 0x28
    .byte 0x0D, 0xD1, 0x08, 0x99, 0x0F, 0x98, 0x81, 0x42, 0x02, 0xDD, 0x05, 0xA8, 0x89, 0xF0, 0x62, 0xFF
    .byte 0x27, 0x27, 0x6F, 0x44, 0x38, 0x78, 0x00, 0x28, 0x01, 0xD0, 0x31, 0x98, 0x24, 0x18, 0x33, 0x99
    .byte 0x8C, 0x42, 0x02, 0xD1, 0x34, 0x9A, 0x95, 0x42, 0x0B, 0xD0, 0x34, 0x87, 0x75, 0x87, 0x01, 0x20
    .byte 0x35, 0x9B, 0x18, 0x70, 0x30, 0x1C, 0x20, 0x30, 0x02, 0x78, 0x30, 0x1C, 0x06, 0x21, 0x00, 0xF0
    .byte 0x8B, 0xFA, 0x2F, 0x9C, 0x20, 0x6A, 0x01, 0x28, 0x00, 0xD1, 0xA5, 0xE0, 0x01, 0x28, 0x02, 0xDC
    .byte 0x00, 0x28, 0x04, 0xD0, 0x88, 0xE1, 0x02, 0x28, 0x00, 0xD1, 0x17, 0xE1, 0x84, 0xE1, 0x2F, 0x9D
    .byte 0x24, 0x35, 0xA9, 0x46, 0x2F, 0x9F, 0xBC, 0x8C, 0x00, 0x2C, 0x44, 0xD0, 0xB5, 0x69, 0xF7, 0x69
    .byte 0x00, 0x20, 0x80, 0x46, 0x00, 0x2D, 0x0B, 0xDA, 0x07, 0x99, 0x0F, 0x98, 0xC1, 0x42, 0x03, 0xDA
    .byte 0x05, 0xA8, 0x89, 0xF0, 0xE9, 0xFE, 0x07, 0x99, 0x10, 0x20, 0x40, 0x42, 0x81, 0x42, 0x29, 0xDC
    .byte 0x00, 0x2D, 0x0B, 0xDD, 0x08, 0x99, 0x0F, 0x98, 0x81, 0x42, 0x04, 0xDD, 0x05, 0xA8, 0x89, 0xF0
    .byte 0x19, 0xFF, 0x08, 0x98, 0x00, 0xE0, 0x08, 0x1C, 0x0F, 0x28, 0x1B, 0xDD, 0x00, 0x2F, 0x0B, 0xDA
    .byte 0x06, 0x99, 0x0F, 0x98, 0xC1, 0x42, 0x03, 0xDA, 0x05, 0xA8, 0x89, 0xF0, 0x91, 0xFE, 0x06, 0x99
    .byte 0x10, 0x20, 0x40, 0x42, 0x81, 0x42, 0x0D, 0xDC, 0x00, 0x2F, 0x0D, 0xDD, 0x05, 0x99, 0x0F, 0x98
    .byte 0x81, 0x42, 0x04, 0xDD, 0x05, 0xA8, 0x89, 0xF0, 0x49, 0xFE, 0x05, 0x98, 0x00, 0xE0, 0x08, 0x1C
    .byte 0x0F, 0x28, 0x01, 0xDC, 0x01, 0x21, 0x88, 0x46, 0x42, 0x46, 0x00, 0x2A, 0x01, 0xD0, 0x00, 0x24
    .byte 0x4D, 0xE0, 0x01, 0x3C, 0x4B, 0xE0, 0x35, 0x9B, 0x18, 0x78, 0x00, 0x28, 0x47, 0xD1, 0x30, 0x1C
    .byte 0xFE, 0xF7, 0x9E, 0xFA, 0x00, 0x06, 0x00, 0x28, 0x41, 0xD0, 0x54, 0x46, 0x20, 0x68, 0xA2, 0x25
    .byte 0x6D, 0x00, 0x40, 0x19, 0x01, 0x68, 0x50, 0x46, 0xB1, 0xF0, 0x82, 0xFE, 0x80, 0x7A, 0xC0, 0x06
    .byte 0xC0, 0x0E, 0x00, 0x25, 0x06, 0x38, 0x0E, 0x28, 0x00, 0xD9, 0x01, 0x25, 0x2F, 0x98, 0x79, 0xF0
    .byte 0xF7, 0xFF, 0xEC, 0x00, 0x64, 0x1B, 0xE4, 0x00, 0xC1, 0x00, 0x09, 0x1A, 0x89, 0x00, 0x15, 0x48
    .byte 0x09, 0x18, 0x64, 0x18, 0x20, 0x1C, 0x07, 0x21, 0x89, 0xF0, 0xF8, 0xFD, 0x07, 0x1C, 0xB8, 0x00
    .byte 0x24, 0x18, 0x3C, 0x25, 0xA0, 0x78, 0x68, 0x43, 0x89, 0xF0, 0x9E, 0xFD, 0x61, 0x78, 0x69, 0x43
    .byte 0x0C, 0x18, 0x30, 0x1C, 0x20, 0x30, 0x05, 0x78, 0x04, 0x20, 0x89, 0xF0, 0x95, 0xFD, 0x01, 0x1C
    .byte 0x00, 0x29, 0x05, 0xD0, 0x03, 0x29, 0x03, 0xD0, 0x28, 0x1C, 0x89, 0xF0, 0x9B, 0xFD, 0x05, 0x1C
    .byte 0x48, 0x46, 0x87, 0x70, 0x30, 0x1C, 0x39, 0x1C, 0x2A, 0x1C, 0x00, 0xF0, 0xE5, 0xF9, 0x49, 0x46
    .byte 0x0C, 0x80, 0xE9, 0xE0, 0x3C, 0x09, 0x0F, 0x08, 0x2F, 0x9C, 0x24, 0x34, 0x2F, 0x9A, 0x95, 0x8C
    .byte 0x00, 0x2D, 0x00, 0xD0, 0x01, 0x3D, 0x62, 0x88, 0x00, 0x2A, 0x00, 0xD0, 0x01, 0x3A, 0x00, 0x2D
    .byte 0x51, 0xD0, 0x4B, 0x46, 0x00, 0x2B, 0x4E, 0xD0, 0x98, 0x88, 0x30, 0x9F, 0xB8, 0x42, 0x4A, 0xD1
    .byte 0x0A, 0x20, 0x33, 0x5E, 0x0E, 0x27, 0xF1, 0x5F, 0x48, 0x46, 0x0A, 0x27, 0xC0, 0x5F, 0x80, 0x46
    .byte 0x48, 0x46, 0x0E, 0x27, 0xC0, 0x5F, 0x81, 0x46, 0x47, 0x46, 0xF8, 0x1A, 0x00, 0x28, 0x00, 0xDA
    .byte 0x40, 0x42, 0x0F, 0x28, 0x06, 0xDC, 0x4F, 0x46, 0x78, 0x1A, 0x00, 0x28, 0x00, 0xDA, 0x40, 0x42
    .byte 0x0F, 0x28, 0x30, 0xDD, 0x35, 0x9F, 0x38, 0x78, 0x00, 0x28, 0x29, 0xD1, 0x00, 0x2A, 0x0F, 0xD0
    .byte 0x04, 0x27, 0xE0, 0x5F, 0xC0, 0x1A, 0x00, 0x28, 0x00, 0xDA, 0x40, 0x42, 0x0F, 0x28, 0x1F, 0xDC
    .byte 0x06, 0x27, 0xE0, 0x5F, 0x40, 0x1A, 0x00, 0x28, 0x00, 0xDA, 0x40, 0x42, 0x0F, 0x28, 0x17, 0xDC
    .byte 0x30, 0x1C, 0x20, 0x30, 0x00, 0x78, 0x05, 0xAA, 0x00, 0x92, 0x01, 0x90, 0x18, 0x1C, 0x42, 0x46
    .byte 0x4B, 0x46, 0x89, 0xF0, 0x8D, 0xFF, 0x02, 0x1C, 0x30, 0x1C, 0x02, 0x21, 0x00, 0xF0, 0x8C, 0xF9
    .byte 0x1E, 0x20, 0x89, 0xF0, 0x29, 0xFD, 0x42, 0x1C, 0x43, 0x46, 0xA3, 0x80, 0x4F, 0x46, 0xE7, 0x80
    .byte 0x25, 0x80, 0x62, 0x80, 0x88, 0xE0, 0x0B, 0x4A, 0x2B, 0xA9, 0x08, 0x68, 0x10, 0x40, 0xB4, 0x22
    .byte 0x10, 0x43, 0x09, 0x4A, 0x10, 0x40, 0x08, 0x60, 0x2F, 0x98, 0x79, 0xF0, 0x63, 0xFF, 0x30, 0x1C
    .byte 0x00, 0xF0, 0xB8, 0xF9, 0x01, 0x1C, 0x30, 0x1C, 0x20, 0x30, 0x02, 0x78, 0x30, 0x1C, 0x00, 0xF0
    .byte 0x6B, 0xF9, 0x71, 0xE0, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0x2F, 0x98, 0x24, 0x30
    .byte 0x81, 0x46, 0x33, 0x1C, 0x20, 0x33, 0x1C, 0x78, 0x0A, 0x22, 0xB1, 0x5E, 0x8C, 0x46, 0x0E, 0x27
    .byte 0xF5, 0x5F, 0x2C, 0xAA, 0x78, 0x20, 0xD0, 0x21, 0x10, 0x80, 0x51, 0x80, 0x2D, 0xA8, 0xBC, 0x21
    .byte 0x89, 0x00, 0x88, 0x46, 0x84, 0x21, 0x47, 0x46, 0x07, 0x80, 0x41, 0x80, 0x9A, 0x46, 0x01, 0x1C
    .byte 0x30, 0x98, 0x09, 0x28, 0x12, 0xD1, 0x00, 0x2C, 0x10, 0xD1, 0x02, 0x23, 0xD1, 0x5E, 0x28, 0x1C
    .byte 0x08, 0x38, 0x08, 0x1A, 0x00, 0x28, 0x00, 0xDA, 0x40, 0x42, 0x07, 0x28, 0x28, 0xDC, 0x30, 0x1C
    .byte 0x02, 0x21, 0x42, 0x46, 0x94, 0x23, 0xFE, 0xF7, 0x4F, 0xF8, 0x21, 0xE0, 0x30, 0x9F, 0x02, 0x2F
    .byte 0x1E, 0xD1, 0x01, 0x2C, 0x1C, 0xD1, 0x00, 0x23, 0xC8, 0x5E, 0x64, 0x46, 0x00, 0x1B, 0x00, 0x28
    .byte 0x00, 0xDA, 0x40, 0x42, 0x0F, 0x28, 0x13, 0xDC, 0x02, 0x27, 0xC9, 0x5F, 0x28, 0x1C, 0x08, 0x30
    .byte 0x08, 0x1A, 0x00, 0x28, 0x00, 0xDA, 0x40, 0x42, 0x07, 0x28, 0x09, 0xDC, 0x00, 0x20, 0x12, 0x5E
    .byte 0x2C, 0xA8, 0x02, 0x21, 0x43, 0x5E, 0x20, 0x3B, 0x30, 0x1C, 0x09, 0x21, 0xFE, 0xF7, 0x2C, 0xF8
    .byte 0x4A, 0x46, 0x10, 0x88, 0x00, 0x28, 0x02, 0xD0, 0x01, 0x38, 0x10, 0x80, 0x14, 0xE0, 0x13, 0x4A
    .byte 0x2E, 0xA9, 0x08, 0x68, 0x10, 0x40, 0xB4, 0x22, 0x10, 0x43, 0x11, 0x4A, 0x10, 0x40, 0x08, 0x60
    .byte 0x2F, 0x98, 0x79, 0xF0, 0xEF, 0xFE, 0x30, 0x1C, 0x00, 0xF0, 0x44, 0xF9, 0x01, 0x1C, 0x53, 0x46
    .byte 0x1A, 0x78, 0x30, 0x1C, 0x00, 0xF0, 0xF8, 0xF8, 0x35, 0x9C, 0x20, 0x78, 0x05, 0xA9, 0x00, 0x28
    .byte 0x00, 0xD0, 0x00, 0x21, 0x30, 0x1C, 0xFE, 0xF7, 0x83, 0xF9, 0x31, 0x1C, 0x30, 0x31, 0x00, 0x20
    .byte 0x08, 0x70, 0xB0, 0x8C, 0x00, 0x28, 0x05, 0xD0, 0x01, 0x38, 0x04, 0xE0, 0x00, 0x00, 0xFF, 0xFF
    .byte 0xFF, 0xFF, 0x00, 0xFF, 0xF0, 0x8C, 0xB0, 0x84, 0x33, 0x69, 0x00, 0x2B, 0x04, 0xD0, 0x58, 0x68
    .byte 0xC1, 0x68, 0x18, 0x1C, 0xB1, 0xF0, 0x3C, 0xFD, 0x36, 0xB0, 0x38, 0xBC, 0x98, 0x46, 0xA1, 0x46
    .byte 0xAA, 0x46, 0xF0, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x30, 0xB5, 0x04, 0x1C, 0x0D, 0x1C, 0x3E, 0x30
    .byte 0x00, 0x78, 0x00, 0x28, 0x0D, 0xD0, 0x20, 0x68, 0x01, 0x68, 0x0A, 0x6C, 0x00, 0x21, 0xB1, 0xF0
    .byte 0x29, 0xFD, 0x41, 0x69, 0xA0, 0x31, 0x09, 0x68, 0xB1, 0xF0, 0x22, 0xFD, 0x00, 0x06, 0x00, 0x28
    .byte 0x03, 0xD1, 0x20, 0x1C, 0x29, 0x1C, 0x10, 0xF0, 0xA9, 0xFA, 0x30, 0xBC, 0x01, 0xBC, 0x00, 0x47





    .section .text.entities_part1.08021FAC, "ax", %progbits
    thumb_func_start func_08021FAC
func_08021FAC: @ 0x08021FAC
    push {r4, lr}
    sub sp, #0x10
    adds r4, r0, #0
    cmp r1, #6
    bhi .L08021FE0
    lsls r0, r1, #2
    ldr r1, .L08021FC0 @ =.L08021FC4
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L08021FC0: .4byte .L08021FC4
.L08021FC4: @ jump table
    .4byte .L08021FE0 @ case 0
    .4byte .L08021FE4 @ case 1
    .4byte .L08021FEA @ case 2
    .4byte .L08021FE0 @ case 3
    .4byte .L08021FE0 @ case 4
    .4byte .L08021FE0 @ case 5
    .4byte .L08021FF0 @ case 6
.L08021FE0:
    movs r0, #0
    b .L0802203E
.L08021FE4:
    movs r0, #0x80
    lsls r0, r0, #8
    b .L0802203E
.L08021FEA:
    movs r0, #0x80
    lsls r0, r0, #9
    b .L0802203E
.L08021FF0:
    ldr r1, [r4, #0x14]
    mov r0, sp
    ldr r2, [r1, #0xc]
    adds r1, r4, #0
    bl _call_via_r2
    ldr r0, [sp]
    ldr r1, [sp, #4]
    str r0, [sp, #8]
    str r1, [sp, #0xc]
    add r1, sp, #8
    movs r2, #0x10
    adds r0, r4, #0
    adds r0, #0x20
    ldrb r0, [r0]
    adds r3, r1, #0
    cmp r0, #0
    blt .L08022036
    cmp r0, #1
    ble .L0802201E
    cmp r0, #3
    ble .L08022028
    b .L08022036
.L0802201E:
    movs r1, #6
    ldrsh r0, [r3, r1]
    movs r2, #2
    ldrsh r1, [r3, r2]
    b .L08022030
.L08022028:
    movs r2, #4
    ldrsh r0, [r1, r2]
    movs r2, #0
    ldrsh r1, [r1, r2]
.L08022030:
    subs r0, r0, r1
    adds r2, r0, #0
    adds r2, #0x10
.L08022036:
    lsls r0, r2, #0x10
    movs r1, #0xc
    bl __divsi3
.L0802203E:
    add sp, #0x10
    pop {r4}
    pop {r1}
    bx r1
    .align 2, 0
    .section .text.entities_part1.080220D4.tail, "ax", %progbits
.L08022100:
    .byte 0x10, 0xB5, 0x81, 0xB0, 0x04, 0x1C, 0x62, 0x6B, 0x10, 0x6A, 0x02, 0x28, 0x1A, 0xD1, 0x0F, 0x49
    .byte 0x00, 0x98, 0x08, 0x40, 0xB4, 0x21, 0x08, 0x43, 0x0D, 0x49, 0x08, 0x40, 0x00, 0x90, 0x10, 0x1C
    .byte 0x69, 0x46, 0x79, 0xF0, 0x7F, 0xFD, 0x20, 0x1C, 0x3D, 0x30, 0x00, 0x78, 0x00, 0x28, 0x09, 0xD1
    .byte 0x20, 0x1C, 0xFF, 0xF7, 0xCF, 0xFF, 0x01, 0x1C, 0x20, 0x1C, 0x20, 0x30, 0x02, 0x78, 0x20, 0x1C
    .byte 0xFF, 0xF7, 0x82, 0xFF, 0x01, 0xB0, 0x10, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00, 0xFF, 0xFF
    .byte 0xFF, 0xFF, 0x00, 0xFF, 0x00, 0xB5, 0x40, 0x6B, 0x79, 0xF0, 0x6A, 0xF8, 0x01, 0x21, 0x48, 0x40
    .byte 0x02, 0xBC, 0x08, 0x47, 0x10, 0xB5, 0x04, 0x1C, 0x60, 0x6B, 0x79, 0xF0, 0x61, 0xF8, 0x00, 0x06
    .byte 0x00, 0x28, 0x06, 0xD1, 0x60, 0x6B, 0x01, 0x21, 0x79, 0xF0, 0x96, 0xF8, 0x60, 0x6B, 0x79, 0xF0
    .byte 0x7B, 0xF8, 0x20, 0x1C, 0x01, 0x21, 0x00, 0x22, 0x10, 0xF0, 0xFC, 0xF8, 0x20, 0x1C, 0xA7, 0x21
    .byte 0xFD, 0xF7, 0x98, 0xFF, 0x10, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00, 0x30, 0xB5, 0x81, 0xB0
    .byte 0x04, 0x1C, 0x3B, 0x2A, 0x1E, 0xD9, 0x25, 0x1C, 0x20, 0x35, 0x28, 0x78, 0x88, 0x42, 0x02, 0xD0
    .byte 0x20, 0x1C, 0x0F, 0xF0, 0xF1, 0xFF, 0x0D, 0x49, 0x00, 0x98, 0x08, 0x40, 0x3C, 0x21, 0x08, 0x43
    .byte 0x00, 0x90, 0x60, 0x6B, 0x69, 0x46, 0x79, 0xF0, 0x3B, 0xFD, 0x20, 0x1C, 0x3D, 0x30, 0x00, 0x78
    .byte 0x00, 0x28, 0x07, 0xD1, 0x20, 0x1C, 0xFF, 0xF7, 0x7D, 0xFF, 0x01, 0x1C, 0x2A, 0x78, 0x20, 0x1C
    .byte 0xFF, 0xF7, 0x32, 0xFF, 0x01, 0xB0, 0x30, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00, 0xFF, 0xFF


    .section .text.entities_part1.080222A8, "ax", %progbits
    thumb_func_start func_080222A8
func_080222A8: @ 0x080222A8
    push {r4, r5, r6, lr}
    mov r6, r8
    push {r6}
    sub sp, #0x10
    adds r6, r0, #0
    mov r8, r1
    adds r4, r2, #0
    adds r0, r4, #0
    bl func_08022320
    adds r5, r0, #0
    adds r0, r4, #0
    bl func_08022334
    str r5, [sp]
    str r0, [sp, #4]
    movs r1, #0
    str r1, [sp, #8]
    add r0, sp, #0xc
    strb r1, [r0]
    adds r0, r6, #0
    mov r1, r8
    movs r2, #5
    movs r3, #0xd
    bl func_080324BC
    ldr r0, .L080222F0 @ =vtable_unk_080E64B4
    str r0, [r6, #4]
    adds r0, r6, #0
    add sp, #0x10
    pop {r3}
    mov r8, r3
    pop {r4, r5, r6}
    pop {r1}
    bx r1
    .align 2, 0
.L080222F0: .4byte vtable_unk_080E64B4
.L080222F4:
    .byte 0x30, 0xB5, 0x04, 0x1C, 0x0D, 0x1C, 0x22, 0x68, 0x90, 0x88, 0x0A, 0x23
    .byte 0xD1, 0x5E, 0x0E, 0x23, 0xD2, 0x5E, 0x10, 0xF0, 0xFB, 0xFA, 0x21, 0x1C, 0x8B, 0x31, 0x08, 0x70
    .byte 0x20, 0x1C, 0x29, 0x1C, 0x10, 0xF0, 0xBC, 0xF9, 0x30, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00

    .section .text.entities_part1.08022334.tail, "ax", %progbits
.L08022348:
    .byte 0x70, 0xB5, 0x04, 0x1C, 0x8C, 0x20, 0xDE, 0xF7
    .byte 0x41, 0xF9, 0x05, 0x1C, 0x60, 0x6B, 0x79, 0xF0, 0x5B, 0xFC, 0x02, 0x1C, 0x28, 0x1C, 0x21, 0x1C
    .byte 0xFF, 0xF7, 0xA2, 0xFF, 0x05, 0x1C, 0x2E, 0x1C, 0x3C, 0x34, 0x20, 0x78, 0x04, 0x28, 0x16, 0xD1
    .byte 0x28, 0x1C, 0x70, 0x30, 0x08, 0x21, 0x3C, 0xF0, 0x73, 0xFA, 0x28, 0x1C, 0x84, 0x30, 0x00, 0x22
    .byte 0x01, 0x21, 0x01, 0x70, 0x02, 0x30, 0x02, 0x70, 0x01, 0x30, 0x01, 0x70, 0x2A, 0x1C, 0x8A, 0x32
    .byte 0x02, 0x23, 0x11, 0x78, 0x04, 0x20, 0x40, 0x42, 0x08, 0x40, 0x18, 0x43, 0x10, 0x70, 0x30, 0x1C
    .byte 0x70, 0xBC, 0x02, 0xBC, 0x08, 0x47, 0x00, 0x00

    .section .text.entities_part1.080223A8.tail, "ax", %progbits
.L080223F0:
    .byte 0x70, 0xB5, 0x82, 0xB0, 0x05, 0x1C, 0x0E, 0x1C, 0x10, 0x48, 0x68, 0x61, 0x6C, 0x6B, 0x68, 0x46
    .byte 0x29, 0x1C, 0x0F, 0xF0, 0xB5, 0xFE, 0x20, 0x1C, 0x69, 0x46, 0x78, 0xF0, 0x8B, 0xFE, 0x0C, 0x48
    .byte 0x68, 0x61, 0x29, 0x69, 0x00, 0x29, 0x05, 0xD0, 0x48, 0x68, 0x82, 0x68, 0x08, 0x1C, 0x03, 0x21
    .byte 0xB1, 0xF0, 0x78, 0xFA, 0x01, 0x20, 0x30, 0x40, 0x00, 0x28, 0x02, 0xD0, 0x28, 0x1C, 0xDE, 0xF7
    .byte 0xEB, 0xF8, 0x02, 0xB0, 0x70, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00, 0x9C, 0x63, 0x0E, 0x08
    .byte 0x50, 0x62, 0x0E, 0x08, 0x10, 0xB5, 0x81, 0xB0, 0x04, 0x1C, 0x09, 0x04, 0x09, 0x0C, 0x0C, 0x4A
    .byte 0x00, 0x98, 0x10, 0x40, 0x08, 0x43, 0x0B, 0x49, 0x08, 0x40, 0x00, 0x90, 0x60, 0x6B, 0x69, 0x46
    .byte 0x79, 0xF0, 0x5E, 0xFC, 0x20, 0x1C, 0x00, 0xF0, 0x73, 0xFA, 0x01, 0x1C, 0x20, 0x1C, 0x20, 0x30
    .byte 0x02, 0x78, 0x20, 0x1C, 0x00, 0xF0, 0x1E, 0xFA, 0x01, 0xB0, 0x10, 0xBC, 0x01, 0xBC, 0x00, 0x47
    .byte 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0x10, 0xB5, 0x04, 0x1C, 0x00, 0xF0, 0x2A, 0xF8
    .byte 0x21, 0x1C, 0x30, 0x31, 0x00, 0x20, 0x08, 0x70, 0xA0, 0x8C, 0x00, 0x28, 0x01, 0xD0, 0x01, 0x38
    .byte 0x00, 0xE0, 0xE0, 0x8C, 0xA0, 0x84, 0x22, 0x69, 0x00, 0x2A, 0x04, 0xD0, 0x50, 0x68, 0xC1, 0x68
    .byte 0x10, 0x1C, 0xB1, 0xF0, 0x2D, 0xFA, 0x10, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x30, 0xB5, 0x04, 0x1C
    .byte 0x0D, 0x1C, 0x0F, 0xF0, 0x55, 0xFF, 0x00, 0x06, 0x00, 0x28, 0x03, 0xD0, 0x20, 0x1C, 0x29, 0x1C
    .byte 0x00, 0xF0, 0x08, 0xF8, 0x20, 0x1C, 0x29, 0x1C, 0x0F, 0xF0, 0xA8, 0xFF, 0x30, 0xBC, 0x01, 0xBC
    .byte 0x00, 0x47, 0x00, 0x00

    .section .text.entities_part1.080224E4, "ax", %progbits
    thumb_func_start func_080224E4
func_080224E4: @ 0x080224E4
    push {r4, r5, r6, r7, lr}
    mov r7, sl
    mov r6, sb
    mov r5, r8
    push {r5, r6, r7}
    sub sp, #0x74
    mov sb, r0
    ldr r7, [r0, #0x34]
    ldr r0, [r0]
    mov r8, r0
    mov r2, sb
    ldrh r2, [r2, #4]
    str r2, [sp, #0x64]
    cmp r2, #2
    bne .L080225A6
    ldrb r0, [r1, #4]
    cmp r0, #0
    beq .L080225A6
    adds r0, r7, #0
    bl GetCurrentOutdoorMinutes__C9Livestock
    adds r5, r0, #0
    adds r0, r7, #0
    movs r1, #1
    bl AddOutdoorMinutes__9LivestockUi
    adds r0, r7, #0
    bl GetCurrentOutdoorMinutes__C9Livestock
    adds r4, r0, #0
    cmp r5, #0xb3
    bhi .L08022552
    cmp r4, #0xb3
    bls .L08022552
    mov r3, r8
    ldr r0, [r3]
    movs r6, #0xa2
    lsls r6, r6, #1
    adds r0, r0, r6
    ldr r1, [r0]
    mov r0, r8
    bl _call_via_r1
    ldr r0, [r0]
    cmp r0, #0
    beq .L08022552
    adds r0, r7, #0
    bl IsUnhappy__C9Livestock
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08022552
    adds r0, r7, #0
    bl SetUnhappy__9Livestock
.L08022552:
    cmp r5, #0xef
    bhi .L08022560
    cmp r4, #0xef
    bls .L08022560
    adds r0, r7, #0
    bl SetFed__7Chicken
.L08022560:
    ldr r0, .L08022670 @ =0x0000012B
    cmp r5, r0
    bhi .L080225A6
    cmp r4, r0
    bls .L080225A6
    mov r1, r8
    ldr r0, [r1]
    movs r2, #0xa2
    lsls r2, r2, #1
    adds r0, r0, r2
    ldr r1, [r0]
    mov r0, r8
    bl _call_via_r1
    ldr r0, [r0]
    cmp r0, #0
    bne .L080225A6
    adds r0, r7, #0
    bl IsUnhappy__C9Livestock
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802259E
    movs r0, #0x64
    bl func_080AB788
    cmp r0, #0x1d
    bhi .L0802259E
    adds r0, r7, #0
    bl ResetUnhappy__9Livestock
.L0802259E:
    adds r0, r7, #0
    movs r1, #1
    bl AddAffection__6Animali
.L080225A6:
    mov r3, r8
    ldr r1, [r3]
    add r0, sp, #8
    ldr r3, [r1, #0x34]
    mov r1, r8
    ldr r2, [sp, #0x64]
    bl _call_via_r3
    mov r6, sb
    ldr r1, [r6, #0x14]
    add r0, sp, #0x48
    ldr r2, [r1, #0xc]
    mov r1, sb
    bl _call_via_r2
    ldr r0, [sp, #0x48]
    ldr r1, [sp, #0x4c]
    str r0, [sp, #0x6c]
    str r1, [sp, #0x70]
    add r4, sp, #0x14
    movs r1, #0x10
    mov sl, r1
    movs r5, #0
    movs r1, #0x11
    str r1, [sp, #0x14]
    movs r0, #0x11
    rsbs r0, r0, #0
    str r0, [r4, #4]
    str r0, [r4, #8]
    str r1, [r4, #0xc]
    add r1, sp, #0x28
    add r0, sp, #8
    ldm r0!, {r2, r3, r6}
    stm r1!, {r2, r3, r6}
    ldr r0, [sp, #0x6c]
    ldr r1, [sp, #0x70]
    str r0, [sp, #0x34]
    str r1, [sp, #0x38]
    mov r1, sl
    str r1, [r4, #0x28]
    str r5, [r4, #0x2c]
    str r5, [r4, #0x30]
    mov r2, r8
    ldr r0, [r2]
    ldr r2, [r0, #0x40]
    mov r0, r8
    movs r1, #0
    bl _call_via_r2
    adds r6, r0, #0
    mov r3, r8
    ldr r0, [r3]
    ldr r2, [r0, #0x40]
    mov r0, r8
    movs r1, #0x4a
    bl _call_via_r2
    adds r5, r0, #0
    cmp r6, #0
    beq .L0802263E
    ldrh r0, [r6, #4]
    ldr r1, [sp, #0x64]
    cmp r0, r1
    bne .L0802263E
    ldr r1, [r6, #0x14]
    add r4, sp, #0x50
    adds r0, r4, #0
    ldr r2, [r1, #0xc]
    adds r1, r6, #0
    bl _call_via_r2
    add r0, sp, #0x14
    adds r1, r4, #0
    movs r2, #0
    bl func_080ABA90
.L0802263E:
    cmp r5, #0
    beq .L08022662
    ldrh r0, [r5, #4]
    ldr r2, [sp, #0x64]
    cmp r0, r2
    bne .L08022662
    ldr r1, [r5, #0x14]
    add r4, sp, #0x58
    adds r0, r4, #0
    ldr r2, [r1, #0xc]
    adds r1, r5, #0
    bl _call_via_r2
    add r0, sp, #0x14
    adds r1, r4, #0
    movs r2, #0
    bl func_080ABA90
.L08022662:
    ldr r0, [r7, #0x24]
    cmp r0, #0
    beq .L08022674
    cmp r0, #1
    bne .L0802266E
    b .L080227B4
.L0802266E:
    b .L08022894
    .align 2, 0
.L08022670: .4byte 0x0000012B
.L08022674:
    movs r3, #0x28
    adds r3, r3, r7
    mov sl, r3
    ldrh r4, [r7, #0x28]
    cmp r4, #0
    beq .L08022706
    mov r6, sb
    ldr r5, [r6, #0x18]
    ldr r6, [r6, #0x1c]
    movs r7, #0
    cmp r5, #0
    bge .L080226A4
    ldr r1, [sp, #0x1c]
    ldr r0, [sp, #0x3c]
    cmn r1, r0
    bge .L0802269C
    add r0, sp, #0x14
    bl func_080AB948
    ldr r1, [sp, #0x1c]
.L0802269C:
    movs r0, #0x10
    rsbs r0, r0, #0
    cmp r1, r0
    bgt .L080226F8
.L080226A4:
    cmp r5, #0
    ble .L080226C0
    ldr r1, [sp, #0x20]
    ldr r0, [sp, #0x3c]
    cmp r1, r0
    ble .L080226BA
    add r0, sp, #0x14
    bl func_080AB9C4
    ldr r0, [sp, #0x20]
    b .L080226BC
.L080226BA:
    adds r0, r1, #0
.L080226BC:
    cmp r0, #0xf
    ble .L080226F8
.L080226C0:
    cmp r6, #0
    bge .L080226DC
    ldr r1, [sp, #0x18]
    ldr r0, [sp, #0x3c]
    cmn r1, r0
    bge .L080226D4
    add r0, sp, #0x14
    bl func_080AB8D0
    ldr r1, [sp, #0x18]
.L080226D4:
    movs r0, #0x10
    rsbs r0, r0, #0
    cmp r1, r0
    bgt .L080226F8
.L080226DC:
    cmp r6, #0
    ble .L080226FA
    ldr r1, [sp, #0x14]
    ldr r0, [sp, #0x3c]
    cmp r1, r0
    ble .L080226F2
    add r0, sp, #0x14
    bl func_080AB85C
    ldr r0, [sp, #0x14]
    b .L080226F4
.L080226F2:
    adds r0, r1, #0
.L080226F4:
    cmp r0, #0xf
    bgt .L080226FA
.L080226F8:
    movs r7, #1
.L080226FA:
    cmp r7, #0
    beq .L08022702
    movs r4, #0
    b .L080227AA
.L08022702:
    subs r4, #1
    b .L080227AA
.L08022706:
    mov r0, sb
    bl AEntity_IsAnimationFinished
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L080227AA
    mov r1, r8
    ldr r0, [r1]
    movs r2, #0xa2
    lsls r2, r2, #1
    adds r0, r0, r2
    ldr r1, [r0]
    mov r0, r8
    bl _call_via_r1
    ldrb r0, [r0, #0xa]
    lsls r0, r0, #0x1b
    lsrs r0, r0, #0x1b
    movs r6, #0
    subs r0, #6
    cmp r0, #0xe
    bls .L08022734
    movs r6, #1
.L08022734:
    adds r0, r7, #0
    bl GetGrowthStage__C7Chicken
    adds r5, r0, #0
    adds r0, r7, #0
    bl IsSick__C9Livestock
    lsls r4, r6, #2
    adds r4, r4, r6
    lsls r4, r4, #4
    lsls r1, r5, #2
    adds r1, r1, r5
    lsls r1, r1, #3
    ldr r2, .L080227B0 @ =gUnk_080F09C8
    adds r1, r1, r2
    adds r4, r4, r1
    lsls r0, r0, #0x18
    lsrs r0, r0, #0x18
    lsls r1, r0, #2
    adds r1, r1, r0
    lsls r1, r1, #2
    adds r4, r4, r1
    adds r0, r4, #0
    movs r1, #5
    bl func_080AB82C
    adds r6, r0, #0
    lsls r0, r6, #2
    adds r4, r4, r0
    movs r5, #0x3c
    ldrb r0, [r4, #2]
    muls r0, r5, r0
    bl func_080AB788
    ldrb r1, [r4, #1]
    muls r1, r5, r1
    adds r4, r1, r0
    mov r0, sb
    adds r0, #0x20
    ldrb r5, [r0]
    movs r0, #4
    bl func_080AB788
    adds r1, r0, #0
    cmp r1, #0
    beq .L0802279C
    cmp r1, #3
    beq .L0802279C
    adds r0, r5, #0
    bl func_080AB7A4
    adds r5, r0, #0
.L0802279C:
    mov r0, sb
    adds r1, r6, #0
    adds r2, r5, #0
    bl ChickenEntity_SetAnimationState
    mov r3, sl
    strb r6, [r3, #2]
.L080227AA:
    mov r6, sl
    strh r4, [r6]
    b .L08022894
    .align 2, 0
.L080227B0: .4byte gUnk_080F09C8
.L080227B4:
    adds r0, r7, #0
    adds r0, #0x28
    str r0, [sp, #0x68]
    ldrh r1, [r7, #0x28]
    mov sl, r1
    cmp r1, #0
    beq .L080227C8
    movs r2, #1
    rsbs r2, r2, #0
    add sl, r2
.L080227C8:
    ldr r1, [r7, #0x28]
    lsls r0, r1, #9
    lsrs r5, r0, #0x19
    cmp r5, #0
    beq .L080227D4
    subs r5, #1
.L080227D4:
    ldr r3, [sp, #0x68]
    ldrb r4, [r3, #3]
    cmp r4, #0
    beq .L080227DE
    subs r4, #1
.L080227DE:
    mov r0, sl
    cmp r0, #0
    beq .L08022868
    lsls r0, r1, #8
    cmp r0, #0
    bge .L080227FC
    mov r1, r8
    ldr r0, [r1]
    ldr r2, [r0, #0x40]
    mov r0, r8
    movs r1, #0x2d
    bl _call_via_r2
    adds r1, r0, #0
    b .L080227FE
.L080227FC:
    adds r1, r6, #0
.L080227FE:
    cmp r1, #0
    beq .L08022868
    ldrh r0, [r1, #4]
    ldr r2, [sp, #0x64]
    cmp r0, r2
    bne .L08022868
    cmp r4, #0
    bne .L0802284E
    mov r3, sb
    movs r0, #0xa
    ldrsh r6, [r3, r0]
    movs r2, #0xe
    ldrsh r4, [r3, r2]
    movs r3, #0xa
    ldrsh r2, [r1, r3]
    movs r0, #0xe
    ldrsh r3, [r1, r0]
    movs r7, #1
    cmp r5, #0
    beq .L08022828
    movs r7, #4
.L08022828:
    mov r0, sb
    adds r0, #0x20
    ldrb r0, [r0]
    add r1, sp, #0x14
    str r1, [sp]
    str r0, [sp, #4]
    adds r0, r6, #0
    adds r1, r4, #0
    bl func_080ABE58
    adds r2, r0, #0
    mov r0, sb
    adds r1, r7, #0
    bl ChickenEntity_SetAnimationState
    movs r0, #0x1e
    bl func_080AB788
    adds r4, r0, #1
.L0802284E:
    mov r3, sl
    ldr r2, [sp, #0x68]
    strh r3, [r2]
    movs r0, #0x7f
    ands r5, r0
    ldrb r1, [r2, #2]
    movs r0, #0x80
    rsbs r0, r0, #0
    ands r0, r1
    orrs r0, r5
    strb r0, [r2, #2]
    strb r4, [r2, #3]
    b .L08022894
.L08022868:
    ldr r1, .L080228AC @ =0xFFFF0000
    ldr r0, [sp, #0x60]
    ands r0, r1
    movs r1, #0xb4
    orrs r0, r1
    ldr r1, .L080228B0 @ =0xFF00FFFF
    ands r0, r1
    str r0, [sp, #0x60]
    add r1, sp, #0x60
    adds r0, r7, #0
    bl method_0809BD20__7ChickenPC15UnkBarnAnimal2C
    mov r0, sb
    bl ChickenEntity_GetAnimationState
    adds r1, r0, #0
    mov r0, sb
    adds r0, #0x20
    ldrb r2, [r0]
    mov r0, sb
    bl ChickenEntity_SetAnimationState
.L08022894:
    mov r0, sb
    add r1, sp, #0x14
    bl AActorEntity_ApplyBoundedMovement
    add sp, #0x74
    pop {r3, r4, r5}
    mov r8, r3
    mov sb, r4
    mov sl, r5
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0
    .align 2, 0
.L080228AC: .4byte 0xFFFF0000
.L080228B0: .4byte 0xFF00FFFF


    .section .text.entities_part1.080229A4.tail, "ax", %progbits
.L080229B8:
    .byte 0x01, 0x20, 0x70, 0x47, 0x00, 0xB5, 0x40, 0x6B
    .byte 0x78, 0xF0, 0xA4, 0xFD, 0x02, 0xBC, 0x08, 0x47, 0x30, 0xB5, 0x04, 0x1C, 0x00, 0x25, 0xA0, 0x88
    .byte 0x02, 0x28, 0x0C, 0xD1, 0x60, 0x6B, 0x78, 0xF0, 0x99, 0xFD, 0x00, 0x06, 0x00, 0x28, 0x06, 0xD1
    .byte 0x60, 0x6B, 0x78, 0xF0, 0x8F, 0xFD, 0x00, 0x06, 0x00, 0x28, 0x00, 0xD1, 0x01, 0x25, 0x28, 0x1C
    .byte 0x30, 0xBC, 0x02, 0xBC, 0x08, 0x47, 0x00, 0x00, 0x10, 0xB5, 0x81, 0xB0, 0x04, 0x1C, 0x78, 0x20
    .byte 0x88, 0xF0, 0xC2, 0xFE, 0xF0, 0x30, 0x00, 0x04, 0x00, 0x0C, 0x1B, 0x4A, 0x00, 0x99, 0x11, 0x40
    .byte 0x01, 0x43, 0x1A, 0x48, 0x01, 0x40, 0xF0, 0x20, 0x80, 0x03, 0x01, 0x43, 0x18, 0x48, 0x01, 0x40
    .byte 0x18, 0x48, 0x01, 0x40, 0x00, 0x91, 0x60, 0x6B, 0x69, 0x46, 0x79, 0xF0, 0x7F, 0xF9, 0x20, 0x1C
    .byte 0xFF, 0xF7, 0x8E, 0xFF, 0x01, 0x1C, 0x20, 0x1C, 0x20, 0x30, 0x02, 0x78, 0x20, 0x1C, 0xFF, 0xF7
    .byte 0x39, 0xFF, 0x60, 0x6B, 0x0A, 0x21, 0x78, 0xF0, 0x49, 0xFC, 0x60, 0x6B, 0x78, 0xF0, 0xF4, 0xFD
    .byte 0x20, 0x1C, 0x02, 0x21, 0x00, 0x22, 0x0F, 0xF0, 0x95, 0xFC, 0x60, 0x6B, 0x79, 0xF0, 0x26, 0xF9
    .byte 0xA5, 0x21, 0x01, 0x28, 0x00, 0xD1, 0xA6, 0x21, 0x20, 0x1C, 0xFD, 0xF7, 0x2B, 0xFB, 0x01, 0xB0
    .byte 0x10, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0xFF
    .byte 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x10, 0xB5, 0x04, 0x1C, 0x60, 0x6B, 0x78, 0xF0
    .byte 0xC9, 0xFD, 0x60, 0x6B, 0x78, 0xF0, 0x36, 0xFD, 0x00, 0x06, 0x00, 0x28, 0x02, 0xD0, 0x60, 0x6B
    .byte 0x78, 0xF0, 0xD0, 0xFD, 0x10, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00, 0x10, 0xB5, 0x81, 0xB0
    .byte 0x04, 0x1C, 0x78, 0x20, 0x88, 0xF0, 0x68, 0xFE, 0xF0, 0x30, 0x00, 0x04, 0x00, 0x0C, 0x1B, 0x4A
    .byte 0x00, 0x99, 0x11, 0x40, 0x01, 0x43, 0x1A, 0x48, 0x01, 0x40, 0xF0, 0x20, 0x80, 0x03, 0x01, 0x43
    .byte 0x80, 0x20, 0x00, 0x04, 0x01, 0x43, 0x17, 0x48, 0x01, 0x40, 0x00, 0x91, 0x60, 0x6B, 0x69, 0x46
    .byte 0x79, 0xF0, 0x24, 0xF9, 0x20, 0x1C, 0xFF, 0xF7, 0x33, 0xFF, 0x01, 0x1C, 0x20, 0x1C, 0x20, 0x30
    .byte 0x02, 0x78, 0x20, 0x1C, 0xFF, 0xF7, 0xDE, 0xFE, 0x60, 0x6B, 0x0A, 0x21, 0x78, 0xF0, 0xEE, 0xFB
    .byte 0x60, 0x6B, 0x78, 0xF0, 0x99, 0xFD, 0x20, 0x1C, 0x02, 0x21, 0x00, 0x22, 0x0F, 0xF0, 0x3A, 0xFC
    .byte 0x60, 0x6B, 0x79, 0xF0, 0xCB, 0xF8, 0xA5, 0x21, 0x01, 0x28, 0x00, 0xD1, 0xA6, 0x21, 0x20, 0x1C
    .byte 0xFD, 0xF7, 0xD0, 0xFA, 0x01, 0xB0, 0x10, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00, 0xFF, 0xFF
    .byte 0xFF, 0xFF, 0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x10, 0xB5, 0x0A, 0x23, 0xCA, 0x5E, 0x0E, 0x24
    .byte 0x0B, 0x5F, 0xD1, 0x1F, 0x01, 0x80, 0x19, 0x1F, 0x41, 0x80, 0x07, 0x32, 0x82, 0x80, 0x05, 0x33
    .byte 0xC3, 0x80, 0x10, 0xBC, 0x04, 0xBC, 0x10, 0x47

    .section .text.entities_part1.08022B5C.tail, "ax", %progbits
.L08022B70:
    .byte 0xF0, 0xB5, 0x84, 0xB0, 0x07, 0x1C, 0x78, 0x6B, 0x79, 0xF0, 0x98, 0xF8, 0x05, 0x1C, 0x8C, 0x20
    .byte 0xDD, 0xF7, 0x28, 0xFD, 0x06, 0x1C, 0x28, 0x1C, 0xFF, 0xF7, 0xE6, 0xFF, 0x04, 0x1C, 0x28, 0x1C
    .byte 0xFF, 0xF7, 0xE4, 0xFF, 0x00, 0x94, 0x01, 0x90, 0x00, 0x25, 0x02, 0x95, 0x03, 0xA8, 0x05, 0x70
    .byte 0x30, 0x1C, 0x39, 0x1C, 0x02, 0x22, 0x09, 0x23, 0x0F, 0xF0, 0x88, 0xFC, 0x04, 0x1C, 0x78, 0x6B
    .byte 0x79, 0xF0, 0x7C, 0xF8, 0x00, 0x21, 0x00, 0x28, 0x00, 0xD0, 0x03, 0x21, 0x38, 0x1C, 0x38, 0x30
    .byte 0x00, 0x78, 0x88, 0x42, 0x23, 0xD1, 0x38, 0x68, 0x01, 0x68, 0xA2, 0x22, 0x52, 0x00, 0x89, 0x18
    .byte 0x09, 0x68, 0xB0, 0xF0, 0x9D, 0xFE, 0x80, 0x7A, 0xC0, 0x06, 0xC0, 0x0E, 0x06, 0x38, 0x0E, 0x28
    .byte 0x15, 0xD9, 0x20, 0x1C, 0x70, 0x30, 0x08, 0x21, 0x3B, 0xF0, 0x3A, 0xFE, 0x20, 0x1C, 0x84, 0x30
    .byte 0x01, 0x21, 0x01, 0x70, 0x02, 0x30, 0x05, 0x70, 0x01, 0x30, 0x01, 0x70, 0x23, 0x1C, 0x8A, 0x33
    .byte 0x02, 0x22, 0x19, 0x78, 0x04, 0x20, 0x40, 0x42, 0x08, 0x40, 0x10, 0x43, 0x18, 0x70, 0x20, 0x1C
    .byte 0x04, 0xB0, 0xF0, 0xBC, 0x02, 0xBC, 0x08, 0x47

    .section .text.entities_part1.08022C18.tail, "ax", %progbits
.L08022C60:
    .byte 0x70, 0xB5, 0x82, 0xB0, 0x05, 0x1C, 0x0E, 0x1C, 0x10, 0x48, 0x68, 0x61, 0x6C, 0x6B, 0x68, 0x46
    .byte 0x29, 0x1C, 0x0F, 0xF0, 0x7D, 0xFA, 0x20, 0x1C, 0x69, 0x46, 0x78, 0xF0, 0x53, 0xFA, 0x0C, 0x48
    .byte 0x68, 0x61, 0x29, 0x69, 0x00, 0x29, 0x05, 0xD0, 0x48, 0x68, 0x82, 0x68, 0x08, 0x1C, 0x03, 0x21
    .byte 0xB0, 0xF0, 0x40, 0xFE, 0x01, 0x20, 0x30, 0x40, 0x00, 0x28, 0x02, 0xD0, 0x28, 0x1C, 0xDD, 0xF7
    .byte 0xB3, 0xFC, 0x02, 0xB0, 0x70, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00, 0x10, 0x63, 0x0E, 0x08
    .byte 0x50, 0x62, 0x0E, 0x08, 0x10, 0xB5, 0x81, 0xB0, 0x04, 0x1C, 0x09, 0x04, 0x09, 0x0C, 0x0C, 0x4A
    .byte 0x00, 0x98, 0x10, 0x40, 0x08, 0x43, 0x0B, 0x49, 0x08, 0x40, 0x00, 0x90, 0x60, 0x6B, 0x69, 0x46
    .byte 0x78, 0xF0, 0x36, 0xFE, 0x20, 0x1C, 0x00, 0xF0, 0x4F, 0xFB, 0x01, 0x1C, 0x20, 0x1C, 0x20, 0x30
    .byte 0x02, 0x78, 0x20, 0x1C, 0x00, 0xF0, 0x10, 0xFB, 0x01, 0xB0, 0x10, 0xBC, 0x01, 0xBC, 0x00, 0x47
    .byte 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xF0, 0xB5, 0x57, 0x46, 0x4E, 0x46, 0x45, 0x46
    .byte 0xE0, 0xB4, 0xA8, 0xB0, 0x80, 0x46, 0x07, 0x68, 0x40, 0x6B, 0x81, 0x46, 0x42, 0x46, 0x92, 0x88
    .byte 0x24, 0x92, 0x02, 0x2A, 0x62, 0xD1, 0x08, 0x79, 0x00, 0x28, 0x5F, 0xD0, 0x48, 0x46, 0x78, 0xF0
    .byte 0xFD, 0xFB, 0x05, 0x1C, 0x48, 0x46, 0x01, 0x21, 0x78, 0xF0, 0x28, 0xFC, 0x48, 0x46, 0x78, 0xF0
    .byte 0xF5, 0xFB, 0x04, 0x1C, 0xB3, 0x2D, 0x15, 0xD8, 0xB3, 0x2C, 0x13, 0xD9, 0x38, 0x68, 0xA2, 0x23
    .byte 0x5B, 0x00, 0xC0, 0x18, 0x01, 0x68, 0x38, 0x1C, 0xB0, 0xF0, 0xE2, 0xFD, 0x00, 0x68, 0x00, 0x28
    .byte 0x08, 0xD0, 0x48, 0x46, 0x78, 0xF0, 0xD6, 0xFB, 0x00, 0x06, 0x00, 0x28, 0x02, 0xD1, 0x48, 0x46
    .byte 0x78, 0xF0, 0x6A, 0xFC, 0xEF, 0x2D, 0x17, 0xD8, 0xEF, 0x2C, 0x15, 0xD9, 0x38, 0x68, 0x84, 0x26
    .byte 0x76, 0x00, 0x80, 0x19, 0x41, 0x46, 0x0A, 0x23, 0xCA, 0x5E, 0x0E, 0x26, 0x8B, 0x5F, 0x12, 0xA9
    .byte 0x0A, 0x80, 0x4B, 0x80, 0x02, 0x68, 0x38, 0x1C, 0xB0, 0xF0, 0xC4, 0xFD, 0x00, 0x06, 0x00, 0x28
    .byte 0x02, 0xD0, 0x48, 0x46, 0x78, 0xF0, 0xAC, 0xFD, 0x43, 0x48, 0x85, 0x42, 0x1E, 0xD8, 0x84, 0x42
    .byte 0x1C, 0xD9, 0x38, 0x68, 0xA2, 0x21, 0x49, 0x00, 0x40, 0x18, 0x01, 0x68, 0x38, 0x1C, 0xB0, 0xF0
    .byte 0xAF, 0xFD, 0x00, 0x68, 0x00, 0x28, 0x11, 0xD1, 0x48, 0x46, 0x78, 0xF0, 0xA3, 0xFB, 0x00, 0x06
    .byte 0x00, 0x28, 0x07, 0xD0, 0x64, 0x20, 0x88, 0xF0, 0xDF, 0xFC, 0x1D, 0x28, 0x02, 0xD8, 0x48, 0x46
    .byte 0x78, 0xF0, 0x38, 0xFC, 0x48, 0x46, 0x01, 0x21, 0x78, 0xF0, 0x66, 0xFA, 0x39, 0x68, 0x02, 0xA8
    .byte 0x4B, 0x6B, 0x39, 0x1C, 0x24, 0x9A, 0xB0, 0xF0, 0x97, 0xFD, 0x42, 0x46, 0x51, 0x69, 0x13, 0xA8
    .byte 0xCA, 0x68, 0x41, 0x46, 0xB0, 0xF0, 0x8E, 0xFD, 0x13, 0x98, 0x14, 0x99, 0x26, 0x90, 0x27, 0x91
    .byte 0x05, 0xAC, 0x10, 0x21, 0x8A, 0x46, 0x00, 0x25, 0x11, 0x21, 0x05, 0x91, 0x11, 0x20, 0x40, 0x42
    .byte 0x60, 0x60, 0xA0, 0x60, 0xE1, 0x60, 0x0A, 0xA9, 0x02, 0xA8, 0x4C, 0xC8, 0x4C, 0xC1, 0x26, 0x98
    .byte 0x27, 0x99, 0x0D, 0x90, 0x0E, 0x91, 0x51, 0x46, 0xA1, 0x62, 0xE5, 0x62, 0x25, 0x63, 0x38, 0x68
    .byte 0x02, 0x6C, 0x38, 0x1C, 0x00, 0x21, 0xB0, 0xF0, 0x6D, 0xFD, 0x06, 0x1C, 0x38, 0x68, 0x02, 0x6C
    .byte 0x38, 0x1C, 0x4A, 0x21, 0xB0, 0xF0, 0x66, 0xFD, 0x05, 0x1C, 0x00, 0x2E, 0x0F, 0xD0, 0xB0, 0x88
    .byte 0x24, 0x9A, 0x90, 0x42, 0x0B, 0xD1, 0x71, 0x69, 0x15, 0xAC, 0x20, 0x1C, 0xCA, 0x68, 0x31, 0x1C
    .byte 0xB0, 0xF0, 0x58, 0xFD, 0x05, 0xA8, 0x21, 0x1C, 0x00, 0x22, 0x88, 0xF0, 0x11, 0xFE, 0x00, 0x2D
    .byte 0x0F, 0xD0, 0xA8, 0x88, 0x24, 0x9B, 0x98, 0x42, 0x0B, 0xD1, 0x69, 0x69, 0x17, 0xAC, 0x20, 0x1C
    .byte 0xCA, 0x68, 0x29, 0x1C, 0xB0, 0xF0, 0x46, 0xFD, 0x05, 0xA8, 0x21, 0x1C, 0x00, 0x22, 0x88, 0xF0
    .byte 0xFF, 0xFD, 0x49, 0x46, 0x88, 0x6A, 0x01, 0x28, 0x00, 0xD1, 0xB7, 0xE0, 0x01, 0x28, 0x05, 0xDC
    .byte 0x00, 0x28, 0x0A, 0xD0, 0x08, 0xE2, 0x00, 0x00, 0x2B, 0x01, 0x00, 0x00, 0x02, 0x28, 0x00, 0xD1
    .byte 0x16, 0xE1, 0x03, 0x28, 0x00, 0xD1, 0x85, 0xE1, 0xFE, 0xE1, 0x2C, 0x22, 0x4A, 0x44, 0x92, 0x46
    .byte 0x4B, 0x46, 0x9C, 0x8D, 0x00, 0x2C, 0x42, 0xD0, 0x46, 0x46, 0xB5, 0x69, 0xF6, 0x69, 0x00, 0x27
    .byte 0x00, 0x2D, 0x0B, 0xDA, 0x07, 0x99, 0x0F, 0x98, 0xC1, 0x42, 0x03, 0xDA, 0x05, 0xA8, 0x88, 0xF0
    .byte 0x33, 0xFD, 0x07, 0x99, 0x10, 0x20, 0x40, 0x42, 0x81, 0x42, 0x29, 0xDC, 0x00, 0x2D, 0x0B, 0xDD
    .byte 0x08, 0x99, 0x0F, 0x98, 0x81, 0x42, 0x04, 0xDD, 0x05, 0xA8, 0x88, 0xF0, 0x63, 0xFD, 0x08, 0x98
    .byte 0x00, 0xE0, 0x08, 0x1C, 0x0F, 0x28, 0x1B, 0xDD, 0x00, 0x2E, 0x0B, 0xDA, 0x06, 0x99, 0x0F, 0x98
    .byte 0xC1, 0x42, 0x03, 0xDA, 0x05, 0xA8, 0x88, 0xF0, 0xDB, 0xFC, 0x06, 0x99, 0x10, 0x20, 0x40, 0x42
    .byte 0x81, 0x42, 0x0D, 0xDC, 0x00, 0x2E, 0x0C, 0xDD, 0x05, 0x99, 0x0F, 0x98, 0x81, 0x42, 0x04, 0xDD
    .byte 0x05, 0xA8, 0x88, 0xF0, 0x93, 0xFC, 0x05, 0x98, 0x00, 0xE0, 0x08, 0x1C, 0x0F, 0x28, 0x00, 0xDC
    .byte 0x01, 0x27, 0x00, 0x2F, 0x01, 0xD0, 0x00, 0x24, 0x5D, 0xE0, 0x01, 0x3C, 0x5B, 0xE0, 0x40, 0x46
    .byte 0xFD, 0xF7, 0xEE, 0xF8, 0x00, 0x06, 0x00, 0x28, 0x55, 0xD0, 0x38, 0x68, 0xA2, 0x21, 0x49, 0x00
    .byte 0x40, 0x18, 0x01, 0x68, 0x38, 0x1C, 0xB0, 0xF0, 0xD3, 0xFC, 0x80, 0x7A, 0xC0, 0x06, 0xC0, 0x0E
    .byte 0x00, 0x21, 0x06, 0x38, 0x0E, 0x28, 0x00, 0xD9, 0x01, 0x21, 0xC8, 0x00, 0x40, 0x1A, 0x40, 0x01
    .byte 0x09, 0x49, 0x45, 0x18, 0x48, 0x46, 0x78, 0xF0, 0xC1, 0xFA, 0x00, 0x06, 0x00, 0x0E, 0xC1, 0x00
    .byte 0x09, 0x1A, 0x8C, 0x00, 0x48, 0x46, 0x78, 0xF0, 0x8B, 0xFC, 0x00, 0x06, 0x00, 0x28, 0x05, 0xD0
    .byte 0x20, 0x1C, 0xA8, 0x30, 0x2D, 0x18, 0x09, 0xE0, 0x7C, 0x0A, 0x0F, 0x08, 0x48, 0x46, 0x78, 0xF0
    .byte 0xEF, 0xFE, 0xC1, 0x00, 0x09, 0x1A, 0xC9, 0x00, 0x69, 0x18, 0x0D, 0x19, 0x28, 0x1C, 0x07, 0x21
    .byte 0x88, 0xF0, 0x34, 0xFC, 0x06, 0x1C, 0xB4, 0x00, 0x2C, 0x19, 0x3C, 0x25, 0xA0, 0x78, 0x68, 0x43
    .byte 0x88, 0xF0, 0xDA, 0xFB, 0x61, 0x78, 0x69, 0x43, 0x0C, 0x18, 0x40, 0x46, 0x20, 0x30, 0x05, 0x78
    .byte 0x04, 0x20, 0x88, 0xF0, 0xD1, 0xFB, 0x01, 0x1C, 0x00, 0x29, 0x05, 0xD0, 0x03, 0x29, 0x03, 0xD0
    .byte 0x28, 0x1C, 0x88, 0xF0, 0xD7, 0xFB, 0x05, 0x1C, 0x40, 0x46, 0x31, 0x1C, 0x2A, 0x1C, 0x00, 0xF0
    .byte 0x83, 0xF9, 0x52, 0x46, 0x96, 0x70, 0x53, 0x46, 0x1C, 0x80, 0x55, 0xE1, 0x48, 0x46, 0x2C, 0x30
    .byte 0x25, 0x90, 0x49, 0x46, 0x89, 0x8D, 0x8A, 0x46, 0x00, 0x29, 0x02, 0xD0, 0x01, 0x22, 0x52, 0x42
    .byte 0x92, 0x44, 0x4B, 0x46, 0xD9, 0x6A, 0x48, 0x02, 0x45, 0x0E, 0x00, 0x2D, 0x00, 0xD0, 0x01, 0x3D
    .byte 0x25, 0x98, 0xC4, 0x78, 0x00, 0x2C, 0x00, 0xD0, 0x01, 0x3C, 0x52, 0x46, 0x00, 0x2A, 0x40, 0xD0
    .byte 0x08, 0x02, 0x00, 0x28, 0x07, 0xDA, 0x38, 0x68, 0x02, 0x6C, 0x38, 0x1C, 0x2D, 0x21, 0xB0, 0xF0
    .byte 0x61, 0xFC, 0x01, 0x1C, 0x00, 0xE0, 0x31, 0x1C, 0x00, 0x29, 0x32, 0xD0, 0x88, 0x88, 0x24, 0x9B
    .byte 0x98, 0x42, 0x2E, 0xD1, 0x00, 0x2C, 0x1F, 0xD1, 0x47, 0x46, 0x0A, 0x20, 0x3E, 0x5E, 0x0E, 0x22
    .byte 0xBC, 0x5E, 0x0A, 0x23, 0xCA, 0x5E, 0x0E, 0x27, 0xCB, 0x5F, 0x01, 0x27, 0x00, 0x2D, 0x00, 0xD0
    .byte 0x06, 0x27, 0x40, 0x46, 0x20, 0x30, 0x00, 0x78, 0x05, 0xA9, 0x00, 0x91, 0x01, 0x90, 0x30, 0x1C
    .byte 0x21, 0x1C, 0x88, 0xF0, 0xE1, 0xFE, 0x02, 0x1C, 0x40, 0x46, 0x39, 0x1C, 0x00, 0xF0, 0x34, 0xF9
    .byte 0x1E, 0x20, 0x88, 0xF0, 0x71, 0xFB, 0x44, 0x1C, 0x53, 0x46, 0x25, 0x9A, 0x13, 0x80, 0x7F, 0x20
    .byte 0x05, 0x40, 0x91, 0x78, 0x80, 0x20, 0x40, 0x42, 0x08, 0x40, 0x28, 0x43, 0x90, 0x70, 0xD4, 0x70
    .byte 0xFA, 0xE0, 0x05, 0x49, 0x19, 0x98, 0x08, 0x40, 0xB4, 0x21, 0x08, 0x43, 0x03, 0x49, 0x08, 0x40
    .byte 0x19, 0x90, 0x19, 0xA9, 0x63, 0xE0, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF
    .byte 0x4C, 0x46, 0x2C, 0x34, 0x4F, 0x46, 0xBD, 0x8D, 0x00, 0x2D, 0x00, 0xD0, 0x01, 0x3D, 0x62, 0x88
    .byte 0x00, 0x2A, 0x00, 0xD0, 0x01, 0x3A, 0x00, 0x2D, 0x48, 0xD0, 0x00, 0x2E, 0x46, 0xD0, 0xB0, 0x88
    .byte 0x24, 0x99, 0x88, 0x42, 0x42, 0xD1, 0x47, 0x46, 0x0A, 0x20, 0x3B, 0x5E, 0x0E, 0x20, 0x39, 0x5E
    .byte 0x0A, 0x20, 0x37, 0x5E, 0xBA, 0x46, 0x0E, 0x27, 0xF6, 0x5F, 0x57, 0x46, 0xF8, 0x1A, 0x00, 0x28
    .byte 0x00, 0xDA, 0x40, 0x42, 0x0F, 0x28, 0x05, 0xDC, 0x70, 0x1A, 0x00, 0x28, 0x00, 0xDA, 0x40, 0x42
    .byte 0x0F, 0x28, 0x2B, 0xDD, 0x00, 0x2A, 0x0F, 0xD0, 0x04, 0x27, 0xE0, 0x5F, 0xC0, 0x1A, 0x00, 0x28
    .byte 0x00, 0xDA, 0x40, 0x42, 0x0F, 0x28, 0x1E, 0xDC, 0x06, 0x27, 0xE0, 0x5F, 0x40, 0x1A, 0x00, 0x28
    .byte 0x00, 0xDA, 0x40, 0x42, 0x0F, 0x28, 0x16, 0xDC, 0x40, 0x46, 0x20, 0x30, 0x00, 0x78, 0x05, 0xAA
    .byte 0x00, 0x92, 0x01, 0x90, 0x18, 0x1C, 0x52, 0x46, 0x33, 0x1C, 0x88, 0xF0, 0x69, 0xFD, 0x02, 0x1C
    .byte 0x40, 0x46, 0x01, 0x21, 0x00, 0xF0, 0xC8, 0xF8, 0x1E, 0x20, 0x88, 0xF0, 0x05, 0xFB, 0x42, 0x1C
    .byte 0x53, 0x46, 0xA3, 0x80, 0xE6, 0x80, 0x25, 0x80, 0x62, 0x80, 0x95, 0xE0, 0x0B, 0x49, 0x1A, 0x98
    .byte 0x08, 0x40, 0xB4, 0x21, 0x08, 0x43, 0x0A, 0x49, 0x08, 0x40, 0x1A, 0x90, 0x1A, 0xA9, 0x48, 0x46
    .byte 0x78, 0xF0, 0xCE, 0xFB, 0x40, 0x46, 0x00, 0xF0, 0xE7, 0xF8, 0x01, 0x1C, 0x40, 0x46, 0x20, 0x30
    .byte 0x02, 0x78, 0x40, 0x46, 0x00, 0xF0, 0xA8, 0xF8, 0x7E, 0xE0, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF
    .byte 0xFF, 0xFF, 0x00, 0xFF, 0x4E, 0x46, 0x2C, 0x36, 0x40, 0x46, 0x20, 0x30, 0x01, 0x78, 0x47, 0x46
    .byte 0x0A, 0x22, 0xBC, 0x5E, 0x0E, 0x23, 0xFD, 0x5E, 0x07, 0x1C, 0x24, 0x98, 0x25, 0x28, 0x21, 0xD1
    .byte 0x00, 0x29, 0x1F, 0xD1, 0x1B, 0xA8, 0xE9, 0xF7, 0x37, 0xFE, 0x1B, 0x98, 0x1C, 0x90, 0x1C, 0xA8
    .byte 0x02, 0x22, 0x81, 0x5E, 0x28, 0x1C, 0x08, 0x38, 0x08, 0x1A, 0x00, 0x28, 0x00, 0xDA, 0x40, 0x42
    .byte 0x07, 0x28, 0x3F, 0xDC, 0x1D, 0xA8, 0xE6, 0xF7, 0x69, 0xFC, 0x1D, 0x98, 0x1E, 0x90, 0x1E, 0xA8
    .byte 0x00, 0x23, 0xC2, 0x5E, 0x02, 0x21, 0x43, 0x5E, 0x10, 0x33, 0x40, 0x46, 0x02, 0x21, 0xFC, 0xF7
    .byte 0x2B, 0xFE, 0x2F, 0xE0, 0x24, 0x9A, 0x02, 0x2A, 0x2C, 0xD1, 0x01, 0x29, 0x2A, 0xD1, 0x1F, 0xA8
    .byte 0xE6, 0xF7, 0x54, 0xFC, 0x1F, 0x98, 0x20, 0x90, 0x20, 0xA8, 0x01, 0x1C, 0x00, 0x23, 0xC8, 0x5E
    .byte 0x00, 0x1B, 0x00, 0x28, 0x00, 0xDA, 0x40, 0x42, 0x0F, 0x28, 0x1B, 0xDC, 0x02, 0x20, 0x09, 0x5E
    .byte 0x28, 0x1C, 0x08, 0x30, 0x08, 0x1A, 0x00, 0x28, 0x00, 0xDA, 0x40, 0x42, 0x07, 0x28, 0x11, 0xDC
    .byte 0x21, 0xA8, 0xE9, 0xF7, 0xF9, 0xFD, 0x21, 0x99, 0x22, 0xA8, 0x01, 0x60, 0x00, 0x21, 0x42, 0x5E
    .byte 0x02, 0x21, 0x43, 0x5E, 0x20, 0x3B, 0x40, 0x46, 0x25, 0x21, 0xFC, 0xF7, 0xFD, 0xFD, 0x48, 0x46
    .byte 0x78, 0xF0, 0xC2, 0xF9, 0x30, 0x88, 0x00, 0x28, 0x02, 0xD0, 0x01, 0x38, 0x30, 0x80, 0x13, 0xE0
    .byte 0x11, 0x4A, 0x23, 0xA9, 0x08, 0x68, 0x10, 0x40, 0xB4, 0x22, 0x10, 0x43, 0x0F, 0x4A, 0x10, 0x40
    .byte 0x08, 0x60, 0x48, 0x46, 0x78, 0xF0, 0x4C, 0xFB, 0x40, 0x46, 0x00, 0xF0, 0x65, 0xF8, 0x01, 0x1C
    .byte 0x3A, 0x78, 0x40, 0x46, 0x00, 0xF0, 0x28, 0xF8, 0x40, 0x46, 0x05, 0xA9, 0xFC, 0xF7, 0x58, 0xFF
    .byte 0x41, 0x46, 0x30, 0x31, 0x00, 0x20, 0x08, 0x70, 0x42, 0x46, 0x90, 0x8C, 0x00, 0x28, 0x07, 0xD0
    .byte 0x01, 0x38, 0x90, 0x84, 0x07, 0xE0, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF
    .byte 0x43, 0x46, 0xD8, 0x8C, 0x98, 0x84, 0x46, 0x46, 0x32, 0x69, 0x00, 0x2A, 0x04, 0xD0, 0x50, 0x68
    .byte 0xC1, 0x68, 0x10, 0x1C, 0xB0, 0xF0, 0x0C, 0xFB, 0x28, 0xB0, 0x38, 0xBC, 0x98, 0x46, 0xA1, 0x46
    .byte 0xAA, 0x46, 0xF0, 0xBC, 0x01, 0xBC, 0x00, 0x47



    .section .text.entities_part1.08023400.tail, "ax", %progbits
.L08023414:
    .byte 0xF0, 0xB5, 0x47, 0x46, 0x80, 0xB4, 0x82, 0xB0, 0x06, 0x1C, 0x0F, 0x1C
    .byte 0x38, 0x88, 0x80, 0x05, 0x80, 0x0D, 0xB1, 0x88, 0x88, 0x42, 0x5F, 0xD1, 0x70, 0x6B, 0x78, 0xF0
    .byte 0x3F, 0xFA, 0x00, 0x06, 0x00, 0x28, 0x59, 0xD1, 0x70, 0x6B, 0x77, 0xF0, 0xF5, 0xFE, 0x80, 0x46
    .byte 0xFC, 0xF7, 0xEA, 0xFD, 0x0A, 0x21, 0x73, 0x5E, 0x79, 0x78, 0x89, 0x08, 0x7A, 0x88, 0x13, 0x4C
    .byte 0x22, 0x40, 0x92, 0x01, 0x0A, 0x43, 0x12, 0x04, 0x12, 0x14, 0xD2, 0x1A, 0x0E, 0x23, 0xF5, 0x5E
    .byte 0xFB, 0x78, 0x9B, 0x08, 0xB9, 0x88, 0x21, 0x40, 0x89, 0x01, 0x19, 0x43, 0x09, 0x04, 0x09, 0x14
    .byte 0x49, 0x1B, 0x13, 0x1C, 0x53, 0x43, 0x1A, 0x1C, 0x0B, 0x1C, 0x4B, 0x43, 0x19, 0x1C, 0x52, 0x18
    .byte 0x01, 0x1C, 0x41, 0x43, 0x08, 0x1C, 0x82, 0x42, 0x30, 0xDC, 0x70, 0x6B, 0x78, 0xF0, 0x3E, 0xF8
    .byte 0x00, 0x06, 0x00, 0x28, 0x04, 0xD0, 0x1E, 0x24, 0x0A, 0xE0, 0x00, 0x00, 0xFF, 0x03, 0x00, 0x00
    .byte 0x70, 0x6B, 0x78, 0xF0, 0x2F, 0xF8, 0x00, 0x06, 0x64, 0x24, 0x00, 0x28, 0x00, 0xD0, 0x46, 0x24
    .byte 0x64, 0x20, 0x88, 0xF0, 0x69, 0xF9, 0xA0, 0x42, 0x18, 0xD2, 0x43, 0x46, 0x18, 0x01, 0xC0, 0x1A
    .byte 0x80, 0x01, 0x00, 0x0A, 0xF0, 0x21, 0x49, 0x00, 0x40, 0x18, 0x00, 0x04, 0x00, 0x0C, 0x00, 0x90
    .byte 0x70, 0x6B, 0x69, 0x46, 0x78, 0xF0, 0x40, 0xFA, 0x30, 0x1C, 0xFF, 0xF7, 0x4D, 0xFF, 0x01, 0x1C
    .byte 0x30, 0x1C, 0x20, 0x30, 0x02, 0x78, 0x30, 0x1C, 0xFF, 0xF7, 0x0E, 0xFF, 0x02, 0xB0, 0x08, 0xBC
    .byte 0x98, 0x46, 0xF0, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x10, 0xB5, 0x81, 0xB0, 0x04, 0x1C, 0x62, 0x6B
    .byte 0x90, 0x6A, 0x03, 0x28, 0x15, 0xD1, 0x0D, 0x49, 0x00, 0x98, 0x08, 0x40, 0xB4, 0x21, 0x08, 0x43
    .byte 0x0B, 0x49, 0x08, 0x40, 0x00, 0x90, 0x10, 0x1C, 0x69, 0x46, 0x78, 0xF0, 0x11, 0xFA, 0x20, 0x1C
    .byte 0xFF, 0xF7, 0x2A, 0xFF, 0x01, 0x1C, 0x20, 0x1C, 0x20, 0x30, 0x02, 0x78, 0x20, 0x1C, 0xFF, 0xF7
    .byte 0xEB, 0xFE, 0x01, 0xB0, 0x10, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF
    .byte 0xFF, 0xFF, 0x00, 0xFF, 0x01, 0x20, 0x70, 0x47, 0x30, 0xB5, 0x04, 0x1C, 0x00, 0x25, 0x60, 0x6B
    .byte 0x78, 0xF0, 0xAE, 0xF9, 0x00, 0x06, 0x00, 0x28, 0x06, 0xD1, 0x60, 0x6B, 0x78, 0xF0, 0x54, 0xFC
    .byte 0x00, 0x06, 0x00, 0x0E, 0x40, 0x42, 0xC5, 0x0F, 0x28, 0x1C, 0x30, 0xBC, 0x02, 0xBC, 0x08, 0x47
    .byte 0x00, 0xB5, 0x40, 0x6B, 0x77, 0xF0, 0x5C, 0xFE, 0x01, 0x21, 0x48, 0x40, 0x02, 0xBC, 0x08, 0x47
    .byte 0x00, 0xB5, 0x40, 0x6B, 0x78, 0xF0, 0x1C, 0xFC, 0x02, 0xBC, 0x08, 0x47, 0x00, 0xB5, 0x40, 0x6B
    .byte 0x77, 0xF0, 0xBC, 0xFF, 0x02, 0xBC, 0x08, 0x47, 0x30, 0xB5, 0x04, 0x1C, 0x00, 0x25, 0xA0, 0x88
    .byte 0x02, 0x28, 0x0C, 0xD1, 0x60, 0x6B, 0x77, 0xF0, 0xB1, 0xFF, 0x00, 0x06, 0x00, 0x28, 0x06, 0xD1
    .byte 0x60, 0x6B, 0x77, 0xF0, 0xA7, 0xFF, 0x00, 0x06, 0x00, 0x28, 0x00, 0xD1, 0x01, 0x25, 0x28, 0x1C
    .byte 0x30, 0xBC, 0x02, 0xBC, 0x08, 0x47, 0x00, 0x00, 0x10, 0xB5, 0x81, 0xB0, 0x04, 0x1C, 0x60, 0x6B
    .byte 0x78, 0xF0, 0x6E, 0xF9, 0x00, 0x06, 0x00, 0x28, 0x21, 0xD1, 0x78, 0x20, 0x88, 0xF0, 0xD4, 0xF8
    .byte 0xF0, 0x30, 0x00, 0x04, 0x00, 0x0C, 0x18, 0x4A, 0x00, 0x99, 0x11, 0x40, 0x01, 0x43, 0x17, 0x48
    .byte 0x01, 0x40, 0xF0, 0x20, 0x80, 0x03, 0x01, 0x43, 0x15, 0x48, 0x01, 0x40, 0x15, 0x48, 0x01, 0x40
    .byte 0x00, 0x91, 0x60, 0x6B, 0x69, 0x46, 0x78, 0xF0, 0xA1, 0xF9, 0x20, 0x1C, 0xFF, 0xF7, 0xB4, 0xFE
    .byte 0x01, 0x1C, 0x20, 0x1C, 0x20, 0x30, 0x02, 0x78, 0x20, 0x1C, 0xFF, 0xF7, 0x75, 0xFE, 0x60, 0x6B
    .byte 0x0A, 0x21, 0x77, 0xF0, 0x5B, 0xFE, 0x60, 0x6B, 0x78, 0xF0, 0x06, 0xF8, 0x20, 0x1C, 0x00, 0x21
    .byte 0x00, 0x22, 0x0E, 0xF0, 0xA7, 0xFE, 0x20, 0x1C, 0xA2, 0x21, 0xFC, 0xF7, 0x43, 0xFD, 0x01, 0xB0
    .byte 0x10, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0xFF
    .byte 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xB5, 0x40, 0x6B, 0x78, 0xF0, 0x4E, 0xF9
    .byte 0x01, 0xBC, 0x00, 0x47, 0x10, 0xB5, 0x04, 0x1C, 0x60, 0x6B, 0x77, 0xF0, 0xE1, 0xFD, 0x00, 0x06
    .byte 0x00, 0x28, 0x06, 0xD1, 0x60, 0x6B, 0x01, 0x21, 0x77, 0xF0, 0x16, 0xFE, 0x60, 0x6B, 0x77, 0xF0
    .byte 0xFB, 0xFD, 0x60, 0x6B, 0x77, 0xF0, 0x3E, 0xFF, 0x00, 0x06, 0x00, 0x28, 0x07, 0xD0, 0x64, 0x20
    .byte 0x88, 0xF0, 0x7A, 0xF8, 0x1D, 0x28, 0x02, 0xD8, 0x60, 0x6B, 0x77, 0xF0, 0xD3, 0xFF, 0x60, 0x6B
    .byte 0x77, 0xF0, 0x34, 0xFF, 0x00, 0x06, 0x00, 0x28, 0x08, 0xD1, 0x20, 0x1C, 0x01, 0x21, 0x00, 0x22
    .byte 0x0E, 0xF0, 0x68, 0xFE, 0x20, 0x1C, 0xA1, 0x21, 0xFC, 0xF7, 0x04, 0xFD, 0x10, 0xBC, 0x01, 0xBC
    .byte 0x00, 0x47, 0x00, 0x00, 0x10, 0xB5, 0x04, 0x1C, 0x01, 0x21, 0x00, 0x22, 0x0E, 0xF0, 0x5A, 0xFE
    .byte 0x20, 0x1C, 0xA1, 0x21, 0xFC, 0xF7, 0xF6, 0xFC, 0x60, 0x6B, 0x78, 0xF0, 0xB3, 0xFB, 0x10, 0xBC
    .byte 0x02, 0xBC, 0x08, 0x47, 0x10, 0xB5, 0x04, 0x1C, 0x60, 0x6B, 0x77, 0xF0, 0x9B, 0xFF, 0x60, 0x6B
    .byte 0x77, 0xF0, 0x08, 0xFF, 0x00, 0x06, 0x00, 0x28, 0x02, 0xD0, 0x60, 0x6B, 0x77, 0xF0, 0xA2, 0xFF
    .byte 0x10, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00, 0x10, 0xB5, 0x81, 0xB0, 0x04, 0x1C, 0x60, 0x6B
    .byte 0x78, 0xF0, 0xCE, 0xF8, 0x00, 0x06, 0x00, 0x28, 0x22, 0xD1, 0x78, 0x20, 0x88, 0xF0, 0x34, 0xF8
    .byte 0xF0, 0x30, 0x00, 0x04, 0x00, 0x0C, 0x18, 0x4A, 0x00, 0x99, 0x11, 0x40, 0x01, 0x43, 0x17, 0x48
    .byte 0x01, 0x40, 0xF0, 0x20, 0x80, 0x03, 0x01, 0x43, 0x80, 0x20, 0x00, 0x04, 0x01, 0x43, 0x14, 0x48
    .byte 0x01, 0x40, 0x00, 0x91, 0x60, 0x6B, 0x69, 0x46, 0x78, 0xF0, 0x00, 0xF9, 0x20, 0x1C, 0xFF, 0xF7
    .byte 0x13, 0xFE, 0x01, 0x1C, 0x20, 0x1C, 0x20, 0x30, 0x02, 0x78, 0x20, 0x1C, 0xFF, 0xF7, 0xD4, 0xFD
    .byte 0x60, 0x6B, 0x0A, 0x21, 0x77, 0xF0, 0xBA, 0xFD, 0x60, 0x6B, 0x77, 0xF0, 0x65, 0xFF, 0x20, 0x1C
    .byte 0x00, 0x21, 0x00, 0x22, 0x0E, 0xF0, 0x06, 0xFE, 0x20, 0x1C, 0xA2, 0x21, 0xFC, 0xF7, 0xA2, 0xFC
    .byte 0x01, 0xB0, 0x10, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x80, 0xFF
    .byte 0xFF, 0xFF, 0xFF, 0x00, 0x70, 0xB5, 0x81, 0xB0, 0x04, 0x1C, 0x0E, 0x1C, 0x15, 0x1C, 0x60, 0x6B
    .byte 0x78, 0xF0, 0x86, 0xF8, 0x00, 0x06, 0x00, 0x28, 0x1C, 0xD1, 0x3B, 0x2D, 0x1A, 0xD9, 0x25, 0x1C
    .byte 0x20, 0x35, 0x28, 0x78, 0xB0, 0x42, 0x03, 0xD0, 0x20, 0x1C, 0x31, 0x1C, 0x0E, 0xF0, 0xEC, 0xFC
    .byte 0x0A, 0x49, 0x00, 0x98, 0x08, 0x40, 0x3C, 0x21, 0x08, 0x43, 0x00, 0x90, 0x60, 0x6B, 0x69, 0x46
    .byte 0x78, 0xF0, 0xCA, 0xF8, 0x20, 0x1C, 0xFF, 0xF7, 0xCF, 0xFD, 0x01, 0x1C, 0x2A, 0x78, 0x20, 0x1C
    .byte 0xFF, 0xF7, 0x92, 0xFD, 0x01, 0xB0, 0x70, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00, 0xFF, 0xFF

    .section .text.entities_part1.08023804.tail, "ax", %progbits
.L08023820:
    .byte 0x70, 0xB5, 0x46, 0x46, 0x40, 0xB4, 0x84, 0xB0, 0x05, 0x1C, 0x68, 0x6B, 0x78, 0xF0, 0xB0, 0xFA
    .byte 0x06, 0x1C, 0x8C, 0x20, 0xDC, 0xF7, 0xCE, 0xFE, 0x80, 0x46, 0x30, 0x1C, 0xFF, 0xF7, 0xD8, 0xFF
    .byte 0x04, 0x1C, 0x30, 0x1C, 0xFF, 0xF7, 0xDE, 0xFF, 0x00, 0x94, 0x01, 0x90, 0x00, 0x26, 0x02, 0x96
    .byte 0x03, 0xA8, 0x06, 0x70, 0x40, 0x46, 0x29, 0x1C, 0x04, 0x22, 0x0C, 0x23, 0x0E, 0xF0, 0x2E, 0xFE
    .byte 0x04, 0x1C, 0xA0, 0x46, 0x38, 0x35, 0x28, 0x78, 0x04, 0x28, 0x15, 0xD1, 0x20, 0x1C, 0x70, 0x30
    .byte 0x08, 0x21, 0x3A, 0xF0, 0xF5, 0xFF, 0x20, 0x1C, 0x84, 0x30, 0x01, 0x21, 0x01, 0x70, 0x02, 0x30
    .byte 0x06, 0x70, 0x01, 0x30, 0x01, 0x70, 0x22, 0x1C, 0x8A, 0x32, 0x02, 0x23, 0x11, 0x78, 0x04, 0x20
    .byte 0x40, 0x42, 0x08, 0x40, 0x18, 0x43, 0x10, 0x70, 0x40, 0x46, 0x04, 0xB0, 0x08, 0xBC, 0x98, 0x46
    .byte 0x70, 0xBC, 0x02, 0xBC, 0x08, 0x47, 0x00, 0x00

    .section .text.entities_part1.080238A8.tail, "ax", %progbits
.L080238F4:
    .byte 0x70, 0xB5, 0x82, 0xB0, 0x05, 0x1C, 0x0E, 0x1C, 0x10, 0x48, 0x68, 0x61
    .byte 0x6C, 0x6B, 0x68, 0x46, 0x29, 0x1C, 0x0E, 0xF0, 0x33, 0xFC, 0x20, 0x1C, 0x69, 0x46, 0x77, 0xF0
    .byte 0x09, 0xFC, 0x0C, 0x48, 0x68, 0x61, 0x29, 0x69, 0x00, 0x29, 0x05, 0xD0, 0x48, 0x68, 0x82, 0x68
    .byte 0x08, 0x1C, 0x03, 0x21, 0xAF, 0xF0, 0xF6, 0xFF, 0x01, 0x20, 0x30, 0x40, 0x00, 0x28, 0x02, 0xD0
    .byte 0x28, 0x1C, 0xDC, 0xF7, 0x69, 0xFE, 0x02, 0xB0, 0x70, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00
    .byte 0x84, 0x62, 0x0E, 0x08, 0x50, 0x62, 0x0E, 0x08, 0x10, 0xB5, 0x81, 0xB0, 0x04, 0x1C, 0x09, 0x04
    .byte 0x09, 0x0C, 0x0C, 0x4A, 0x00, 0x98, 0x10, 0x40, 0x08, 0x43, 0x0B, 0x49, 0x08, 0x40, 0x00, 0x90
    .byte 0x60, 0x6B, 0x69, 0x46, 0x77, 0xF0, 0xEC, 0xFF, 0x20, 0x1C, 0x00, 0xF0, 0x59, 0xFB, 0x01, 0x1C
    .byte 0x20, 0x1C, 0x20, 0x30, 0x02, 0x78, 0x20, 0x1C, 0x00, 0xF0, 0x1A, 0xFB, 0x01, 0xB0, 0x10, 0xBC
    .byte 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0xF0, 0xB5, 0x57, 0x46
    .byte 0x4E, 0x46, 0x45, 0x46, 0xE0, 0xB4, 0xA8, 0xB0, 0x80, 0x46, 0x07, 0x68, 0x40, 0x6B, 0x81, 0x46
    .byte 0x42, 0x46, 0x92, 0x88, 0x24, 0x92, 0x02, 0x2A, 0x62, 0xD1, 0x08, 0x79, 0x00, 0x28, 0x5F, 0xD0
    .byte 0x48, 0x46, 0x77, 0xF0, 0xB3, 0xFD, 0x05, 0x1C, 0x48, 0x46, 0x01, 0x21, 0x77, 0xF0, 0xDE, 0xFD
    .byte 0x48, 0x46, 0x77, 0xF0, 0xAB, 0xFD, 0x04, 0x1C, 0xB3, 0x2D, 0x15, 0xD8, 0xB3, 0x2C, 0x13, 0xD9
    .byte 0x38, 0x68, 0xA2, 0x23, 0x5B, 0x00, 0xC0, 0x18, 0x01, 0x68, 0x38, 0x1C, 0xAF, 0xF0, 0x98, 0xFF
    .byte 0x00, 0x68, 0x00, 0x28, 0x08, 0xD0, 0x48, 0x46, 0x77, 0xF0, 0x8C, 0xFD, 0x00, 0x06, 0x00, 0x28
    .byte 0x02, 0xD1, 0x48, 0x46, 0x77, 0xF0, 0x20, 0xFE, 0xEF, 0x2D, 0x17, 0xD8, 0xEF, 0x2C, 0x15, 0xD9
    .byte 0x38, 0x68, 0x84, 0x26, 0x76, 0x00, 0x80, 0x19, 0x41, 0x46, 0x0A, 0x23, 0xCA, 0x5E, 0x0E, 0x26
    .byte 0x8B, 0x5F, 0x12, 0xA9, 0x0A, 0x80, 0x4B, 0x80, 0x02, 0x68, 0x38, 0x1C, 0xAF, 0xF0, 0x7A, 0xFF
    .byte 0x00, 0x06, 0x00, 0x28, 0x02, 0xD0, 0x48, 0x46, 0x77, 0xF0, 0x62, 0xFF, 0x43, 0x48, 0x85, 0x42
    .byte 0x1E, 0xD8, 0x84, 0x42, 0x1C, 0xD9, 0x38, 0x68, 0xA2, 0x21, 0x49, 0x00, 0x40, 0x18, 0x01, 0x68
    .byte 0x38, 0x1C, 0xAF, 0xF0, 0x65, 0xFF, 0x00, 0x68, 0x00, 0x28, 0x11, 0xD1, 0x48, 0x46, 0x77, 0xF0
    .byte 0x59, 0xFD, 0x00, 0x06, 0x00, 0x28, 0x07, 0xD0, 0x64, 0x20, 0x87, 0xF0, 0x95, 0xFE, 0x1D, 0x28
    .byte 0x02, 0xD8, 0x48, 0x46, 0x77, 0xF0, 0xEE, 0xFD, 0x48, 0x46, 0x01, 0x21, 0x77, 0xF0, 0x1C, 0xFC
    .byte 0x39, 0x68, 0x02, 0xA8, 0x4B, 0x6B, 0x39, 0x1C, 0x24, 0x9A, 0xAF, 0xF0, 0x4D, 0xFF, 0x42, 0x46
    .byte 0x51, 0x69, 0x13, 0xA8, 0xCA, 0x68, 0x41, 0x46, 0xAF, 0xF0, 0x44, 0xFF, 0x13, 0x98, 0x14, 0x99
    .byte 0x26, 0x90, 0x27, 0x91, 0x05, 0xAC, 0x10, 0x21, 0x8A, 0x46, 0x00, 0x25, 0x11, 0x21, 0x05, 0x91
    .byte 0x11, 0x20, 0x40, 0x42, 0x60, 0x60, 0xA0, 0x60, 0xE1, 0x60, 0x0A, 0xA9, 0x02, 0xA8, 0x4C, 0xC8
    .byte 0x4C, 0xC1, 0x26, 0x98, 0x27, 0x99, 0x0D, 0x90, 0x0E, 0x91, 0x51, 0x46, 0xA1, 0x62, 0xE5, 0x62
    .byte 0x25, 0x63, 0x38, 0x68, 0x02, 0x6C, 0x38, 0x1C, 0x00, 0x21, 0xAF, 0xF0, 0x23, 0xFF, 0x06, 0x1C
    .byte 0x38, 0x68, 0x02, 0x6C, 0x38, 0x1C, 0x4A, 0x21, 0xAF, 0xF0, 0x1C, 0xFF, 0x05, 0x1C, 0x00, 0x2E
    .byte 0x0F, 0xD0, 0xB0, 0x88, 0x24, 0x9A, 0x90, 0x42, 0x0B, 0xD1, 0x71, 0x69, 0x15, 0xAC, 0x20, 0x1C
    .byte 0xCA, 0x68, 0x31, 0x1C, 0xAF, 0xF0, 0x0E, 0xFF, 0x05, 0xA8, 0x21, 0x1C, 0x00, 0x22, 0x87, 0xF0
    .byte 0xC7, 0xFF, 0x00, 0x2D, 0x0F, 0xD0, 0xA8, 0x88, 0x24, 0x9B, 0x98, 0x42, 0x0B, 0xD1, 0x69, 0x69
    .byte 0x17, 0xAC, 0x20, 0x1C, 0xCA, 0x68, 0x29, 0x1C, 0xAF, 0xF0, 0xFC, 0xFE, 0x05, 0xA8, 0x21, 0x1C
    .byte 0x00, 0x22, 0x87, 0xF0, 0xB5, 0xFF, 0x49, 0x46, 0x88, 0x6A, 0x01, 0x28, 0x00, 0xD1, 0xC1, 0xE0
    .byte 0x01, 0x28, 0x05, 0xDC, 0x00, 0x28, 0x0A, 0xD0, 0x12, 0xE2, 0x00, 0x00, 0x2B, 0x01, 0x00, 0x00
    .byte 0x02, 0x28, 0x00, 0xD1, 0x20, 0xE1, 0x03, 0x28, 0x00, 0xD1, 0x8F, 0xE1, 0x08, 0xE2, 0x2C, 0x22
    .byte 0x4A, 0x44, 0x92, 0x46, 0x4B, 0x46, 0x9C, 0x8D, 0x00, 0x2C, 0x42, 0xD0, 0x46, 0x46, 0xB5, 0x69
    .byte 0xF6, 0x69, 0x00, 0x27, 0x00, 0x2D, 0x0B, 0xDA, 0x07, 0x99, 0x0F, 0x98, 0xC1, 0x42, 0x03, 0xDA
    .byte 0x05, 0xA8, 0x87, 0xF0, 0xE9, 0xFE, 0x07, 0x99, 0x10, 0x20, 0x40, 0x42, 0x81, 0x42, 0x29, 0xDC
    .byte 0x00, 0x2D, 0x0B, 0xDD, 0x08, 0x99, 0x0F, 0x98, 0x81, 0x42, 0x04, 0xDD, 0x05, 0xA8, 0x87, 0xF0
    .byte 0x19, 0xFF, 0x08, 0x98, 0x00, 0xE0, 0x08, 0x1C, 0x0F, 0x28, 0x1B, 0xDD, 0x00, 0x2E, 0x0B, 0xDA
    .byte 0x06, 0x99, 0x0F, 0x98, 0xC1, 0x42, 0x03, 0xDA, 0x05, 0xA8, 0x87, 0xF0, 0x91, 0xFE, 0x06, 0x99
    .byte 0x10, 0x20, 0x40, 0x42, 0x81, 0x42, 0x0D, 0xDC, 0x00, 0x2E, 0x0C, 0xDD, 0x05, 0x99, 0x0F, 0x98
    .byte 0x81, 0x42, 0x04, 0xDD, 0x05, 0xA8, 0x87, 0xF0, 0x49, 0xFE, 0x05, 0x98, 0x00, 0xE0, 0x08, 0x1C
    .byte 0x0F, 0x28, 0x00, 0xDC, 0x01, 0x27, 0x00, 0x2F, 0x01, 0xD0, 0x00, 0x24, 0x67, 0xE0, 0x01, 0x3C
    .byte 0x65, 0xE0, 0x40, 0x46, 0xFC, 0xF7, 0xA4, 0xFA, 0x00, 0x06, 0x00, 0x28, 0x5F, 0xD0, 0x38, 0x68
    .byte 0xA2, 0x21, 0x49, 0x00, 0x40, 0x18, 0x01, 0x68, 0x38, 0x1C, 0xAF, 0xF0, 0x89, 0xFE, 0x80, 0x7A
    .byte 0xC0, 0x06, 0xC0, 0x0E, 0x00, 0x21, 0x06, 0x38, 0x0E, 0x28, 0x00, 0xD9, 0x01, 0x21, 0xC8, 0x00
    .byte 0x40, 0x1A, 0x40, 0x01, 0x09, 0x49, 0x45, 0x18, 0x48, 0x46, 0x77, 0xF0, 0x77, 0xFC, 0x00, 0x06
    .byte 0x00, 0x0E, 0xC1, 0x00, 0x09, 0x1A, 0x8C, 0x00, 0x48, 0x46, 0x77, 0xF0, 0x41, 0xFE, 0x00, 0x06
    .byte 0x00, 0x28, 0x05, 0xD0, 0x20, 0x1C, 0xA8, 0x30, 0x2D, 0x18, 0x13, 0xE0, 0x74, 0x0C, 0x0F, 0x08
    .byte 0x48, 0x46, 0x78, 0xF0, 0x5D, 0xF9, 0x00, 0x06, 0x00, 0x28, 0x03, 0xD0, 0x20, 0x1C, 0x70, 0x30
    .byte 0x2D, 0x18, 0x07, 0xE0, 0x48, 0x46, 0x78, 0xF0, 0x41, 0xF9, 0xC1, 0x00, 0x09, 0x1A, 0xC9, 0x00
    .byte 0x69, 0x18, 0x0D, 0x19, 0x28, 0x1C, 0x07, 0x21, 0x87, 0xF0, 0xE0, 0xFD, 0x06, 0x1C, 0xB4, 0x00
    .byte 0x2C, 0x19, 0x3C, 0x25, 0xA0, 0x78, 0x68, 0x43, 0x87, 0xF0, 0x86, 0xFD, 0x61, 0x78, 0x69, 0x43
    .byte 0x0C, 0x18, 0x40, 0x46, 0x20, 0x30, 0x05, 0x78, 0x04, 0x20, 0x87, 0xF0, 0x7D, 0xFD, 0x01, 0x1C
    .byte 0x00, 0x29, 0x05, 0xD0, 0x03, 0x29, 0x03, 0xD0, 0x28, 0x1C, 0x87, 0xF0, 0x83, 0xFD, 0x05, 0x1C
    .byte 0x40, 0x46, 0x31, 0x1C, 0x2A, 0x1C, 0x00, 0xF0, 0x83, 0xF9, 0x52, 0x46, 0x96, 0x70, 0x53, 0x46
    .byte 0x1C, 0x80, 0x55, 0xE1, 0x48, 0x46, 0x2C, 0x30, 0x25, 0x90, 0x49, 0x46, 0x89, 0x8D, 0x8A, 0x46
    .byte 0x00, 0x29, 0x02, 0xD0, 0x01, 0x22, 0x52, 0x42, 0x92, 0x44, 0x4B, 0x46, 0xD9, 0x6A, 0x48, 0x02
    .byte 0x45, 0x0E, 0x00, 0x2D, 0x00, 0xD0, 0x01, 0x3D, 0x25, 0x98, 0xC4, 0x78, 0x00, 0x2C, 0x00, 0xD0
    .byte 0x01, 0x3C, 0x52, 0x46, 0x00, 0x2A, 0x40, 0xD0, 0x08, 0x02, 0x00, 0x28, 0x07, 0xDA, 0x38, 0x68
    .byte 0x02, 0x6C, 0x38, 0x1C, 0x2D, 0x21, 0xAF, 0xF0, 0x0D, 0xFE, 0x01, 0x1C, 0x00, 0xE0, 0x31, 0x1C
    .byte 0x00, 0x29, 0x32, 0xD0, 0x88, 0x88, 0x24, 0x9B, 0x98, 0x42, 0x2E, 0xD1, 0x00, 0x2C, 0x1F, 0xD1
    .byte 0x47, 0x46, 0x0A, 0x20, 0x3E, 0x5E, 0x0E, 0x22, 0xBC, 0x5E, 0x0A, 0x23, 0xCA, 0x5E, 0x0E, 0x27
    .byte 0xCB, 0x5F, 0x01, 0x27, 0x00, 0x2D, 0x00, 0xD0, 0x06, 0x27, 0x40, 0x46, 0x20, 0x30, 0x00, 0x78
    .byte 0x05, 0xA9, 0x00, 0x91, 0x01, 0x90, 0x30, 0x1C, 0x21, 0x1C, 0x88, 0xF0, 0x8D, 0xF8, 0x02, 0x1C
    .byte 0x40, 0x46, 0x39, 0x1C, 0x00, 0xF0, 0x34, 0xF9, 0x1E, 0x20, 0x87, 0xF0, 0x1D, 0xFD, 0x44, 0x1C
    .byte 0x53, 0x46, 0x25, 0x9A, 0x13, 0x80, 0x7F, 0x20, 0x05, 0x40, 0x91, 0x78, 0x80, 0x20, 0x40, 0x42
    .byte 0x08, 0x40, 0x28, 0x43, 0x90, 0x70, 0xD4, 0x70, 0xFA, 0xE0, 0x05, 0x49, 0x19, 0x98, 0x08, 0x40
    .byte 0xB4, 0x21, 0x08, 0x43, 0x03, 0x49, 0x08, 0x40, 0x19, 0x90, 0x19, 0xA9, 0x63, 0xE0, 0x00, 0x00
    .byte 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0x4C, 0x46, 0x2C, 0x34, 0x4F, 0x46, 0xBD, 0x8D
    .byte 0x00, 0x2D, 0x00, 0xD0, 0x01, 0x3D, 0x62, 0x88, 0x00, 0x2A, 0x00, 0xD0, 0x01, 0x3A, 0x00, 0x2D
    .byte 0x48, 0xD0, 0x00, 0x2E, 0x46, 0xD0, 0xB0, 0x88, 0x24, 0x99, 0x88, 0x42, 0x42, 0xD1, 0x47, 0x46
    .byte 0x0A, 0x20, 0x3B, 0x5E, 0x0E, 0x20, 0x39, 0x5E, 0x0A, 0x20, 0x37, 0x5E, 0xBA, 0x46, 0x0E, 0x27
    .byte 0xF6, 0x5F, 0x57, 0x46, 0xF8, 0x1A, 0x00, 0x28, 0x00, 0xDA, 0x40, 0x42, 0x0F, 0x28, 0x05, 0xDC
    .byte 0x70, 0x1A, 0x00, 0x28, 0x00, 0xDA, 0x40, 0x42, 0x0F, 0x28, 0x2B, 0xDD, 0x00, 0x2A, 0x0F, 0xD0
    .byte 0x04, 0x27, 0xE0, 0x5F, 0xC0, 0x1A, 0x00, 0x28, 0x00, 0xDA, 0x40, 0x42, 0x0F, 0x28, 0x1E, 0xDC
    .byte 0x06, 0x27, 0xE0, 0x5F, 0x40, 0x1A, 0x00, 0x28, 0x00, 0xDA, 0x40, 0x42, 0x0F, 0x28, 0x16, 0xDC
    .byte 0x40, 0x46, 0x20, 0x30, 0x00, 0x78, 0x05, 0xAA, 0x00, 0x92, 0x01, 0x90, 0x18, 0x1C, 0x52, 0x46
    .byte 0x33, 0x1C, 0x87, 0xF0, 0x15, 0xFF, 0x02, 0x1C, 0x40, 0x46, 0x01, 0x21, 0x00, 0xF0, 0xC8, 0xF8
    .byte 0x1E, 0x20, 0x87, 0xF0, 0xB1, 0xFC, 0x42, 0x1C, 0x53, 0x46, 0xA3, 0x80, 0xE6, 0x80, 0x25, 0x80
    .byte 0x62, 0x80, 0x95, 0xE0, 0x0B, 0x49, 0x1A, 0x98, 0x08, 0x40, 0xB4, 0x21, 0x08, 0x43, 0x0A, 0x49
    .byte 0x08, 0x40, 0x1A, 0x90, 0x1A, 0xA9, 0x48, 0x46, 0x77, 0xF0, 0x7A, 0xFD, 0x40, 0x46, 0x00, 0xF0
    .byte 0xE7, 0xF8, 0x01, 0x1C, 0x40, 0x46, 0x20, 0x30, 0x02, 0x78, 0x40, 0x46, 0x00, 0xF0, 0xA8, 0xF8
    .byte 0x7E, 0xE0, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0x4E, 0x46, 0x2C, 0x36
    .byte 0x40, 0x46, 0x20, 0x30, 0x01, 0x78, 0x47, 0x46, 0x0A, 0x22, 0xBC, 0x5E, 0x0E, 0x23, 0xFD, 0x5E
    .byte 0x07, 0x1C, 0x24, 0x98, 0x25, 0x28, 0x21, 0xD1, 0x00, 0x29, 0x1F, 0xD1, 0x1B, 0xA8, 0xE8, 0xF7
    .byte 0xE3, 0xFF, 0x1B, 0x98, 0x1C, 0x90, 0x1C, 0xA8, 0x02, 0x22, 0x81, 0x5E, 0x28, 0x1C, 0x08, 0x38
    .byte 0x08, 0x1A, 0x00, 0x28, 0x00, 0xDA, 0x40, 0x42, 0x07, 0x28, 0x3F, 0xDC, 0x1D, 0xA8, 0xE5, 0xF7
    .byte 0x15, 0xFE, 0x1D, 0x98, 0x1E, 0x90, 0x1E, 0xA8, 0x00, 0x23, 0xC2, 0x5E, 0x02, 0x21, 0x43, 0x5E
    .byte 0x10, 0x33, 0x40, 0x46, 0x02, 0x21, 0xFB, 0xF7, 0xD7, 0xFF, 0x2F, 0xE0, 0x24, 0x9A, 0x02, 0x2A
    .byte 0x2C, 0xD1, 0x01, 0x29, 0x2A, 0xD1, 0x1F, 0xA8, 0xE5, 0xF7, 0x00, 0xFE, 0x1F, 0x98, 0x20, 0x90
    .byte 0x20, 0xA8, 0x01, 0x1C, 0x00, 0x23, 0xC8, 0x5E, 0x00, 0x1B, 0x00, 0x28, 0x00, 0xDA, 0x40, 0x42
    .byte 0x0F, 0x28, 0x1B, 0xDC, 0x02, 0x20, 0x09, 0x5E, 0x28, 0x1C, 0x08, 0x30, 0x08, 0x1A, 0x00, 0x28
    .byte 0x00, 0xDA, 0x40, 0x42, 0x07, 0x28, 0x11, 0xDC, 0x21, 0xA8, 0xE8, 0xF7, 0xA5, 0xFF, 0x21, 0x99
    .byte 0x22, 0xA8, 0x01, 0x60, 0x00, 0x21, 0x42, 0x5E, 0x02, 0x21, 0x43, 0x5E, 0x20, 0x3B, 0x40, 0x46
    .byte 0x25, 0x21, 0xFB, 0xF7, 0xA9, 0xFF, 0x48, 0x46, 0x77, 0xF0, 0x6E, 0xFB, 0x30, 0x88, 0x00, 0x28
    .byte 0x02, 0xD0, 0x01, 0x38, 0x30, 0x80, 0x13, 0xE0, 0x11, 0x4A, 0x23, 0xA9, 0x08, 0x68, 0x10, 0x40
    .byte 0xB4, 0x22, 0x10, 0x43, 0x0F, 0x4A, 0x10, 0x40, 0x08, 0x60, 0x48, 0x46, 0x77, 0xF0, 0xF8, 0xFC
    .byte 0x40, 0x46, 0x00, 0xF0, 0x65, 0xF8, 0x01, 0x1C, 0x3A, 0x78, 0x40, 0x46, 0x00, 0xF0, 0x28, 0xF8
    .byte 0x40, 0x46, 0x05, 0xA9, 0xFC, 0xF7, 0x04, 0xF9, 0x41, 0x46, 0x30, 0x31, 0x00, 0x20, 0x08, 0x70
    .byte 0x42, 0x46, 0x90, 0x8C, 0x00, 0x28, 0x07, 0xD0, 0x01, 0x38, 0x90, 0x84, 0x07, 0xE0, 0x00, 0x00
    .byte 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0x43, 0x46, 0xD8, 0x8C, 0x98, 0x84, 0x46, 0x46
    .byte 0x32, 0x69, 0x00, 0x2A, 0x04, 0xD0, 0x50, 0x68, 0xC1, 0x68, 0x10, 0x1C, 0xAF, 0xF0, 0xB8, 0xFC
    .byte 0x28, 0xB0, 0x38, 0xBC, 0x98, 0x46, 0xA1, 0x46, 0xAA, 0x46, 0xF0, 0xBC, 0x01, 0xBC, 0x00, 0x47



    .section .text.entities_part1.080240BC.tail, "ax", %progbits
.L080240D0:
    .byte 0xF0, 0xB5, 0x82, 0xB0, 0x06, 0x1C, 0x0F, 0x1C, 0x38, 0x88, 0x80, 0x05, 0x80, 0x0D, 0xB1, 0x88
    .byte 0x88, 0x42, 0x5F, 0xD1, 0x70, 0x6B, 0x77, 0xF0, 0xE3, 0xFB, 0x00, 0x06, 0x00, 0x28, 0x59, 0xD1
    .byte 0x70, 0x6B, 0x77, 0xF0, 0x99, 0xF8, 0xFB, 0xF7, 0x8F, 0xFF, 0x0A, 0x21, 0x73, 0x5E, 0x79, 0x78
    .byte 0x89, 0x08, 0x7A, 0x88, 0x12, 0x4C, 0x22, 0x40, 0x92, 0x01, 0x0A, 0x43, 0x12, 0x04, 0x12, 0x14
    .byte 0xD2, 0x1A, 0x0E, 0x23, 0xF5, 0x5E, 0xFB, 0x78, 0x9B, 0x08, 0xB9, 0x88, 0x21, 0x40, 0x89, 0x01
    .byte 0x19, 0x43, 0x09, 0x04, 0x09, 0x14, 0x49, 0x1B, 0x13, 0x1C, 0x53, 0x43, 0x1A, 0x1C, 0x0B, 0x1C
    .byte 0x4B, 0x43, 0x19, 0x1C, 0x52, 0x18, 0x01, 0x1C, 0x41, 0x43, 0x08, 0x1C, 0x82, 0x42, 0x31, 0xDC
    .byte 0x70, 0x6B, 0x77, 0xF0, 0xE3, 0xF9, 0x00, 0x06, 0x00, 0x28, 0x03, 0xD0, 0x1E, 0x24, 0x09, 0xE0
    .byte 0xFF, 0x03, 0x00, 0x00, 0x70, 0x6B, 0x77, 0xF0, 0xD5, 0xF9, 0x00, 0x06, 0x64, 0x24, 0x00, 0x28
    .byte 0x00, 0xD0, 0x46, 0x24, 0x64, 0x20, 0x87, 0xF0, 0x0F, 0xFB, 0xA0, 0x42, 0x1A, 0xD2, 0x70, 0x6B
    .byte 0x77, 0xF0, 0x5A, 0xF8, 0x01, 0x01, 0x09, 0x1A, 0x89, 0x01, 0x09, 0x0A, 0xF0, 0x23, 0x5B, 0x00
    .byte 0xC9, 0x18, 0x09, 0x04, 0x09, 0x0C, 0x00, 0x91, 0x70, 0x6B, 0x69, 0x46, 0x77, 0xF0, 0xE4, 0xFB
    .byte 0x30, 0x1C, 0xFF, 0xF7, 0x45, 0xFF, 0x01, 0x1C, 0x30, 0x1C, 0x20, 0x30, 0x02, 0x78, 0x30, 0x1C
    .byte 0xFF, 0xF7, 0x06, 0xFF, 0x02, 0xB0, 0xF0, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x10, 0xB5, 0x81, 0xB0
    .byte 0x04, 0x1C, 0x62, 0x6B, 0x90, 0x6A, 0x03, 0x28, 0x15, 0xD1, 0x0D, 0x49, 0x00, 0x98, 0x08, 0x40
    .byte 0xB4, 0x21, 0x08, 0x43, 0x0B, 0x49, 0x08, 0x40, 0x00, 0x90, 0x10, 0x1C, 0x69, 0x46, 0x77, 0xF0
    .byte 0xB7, 0xFB, 0x20, 0x1C, 0xFF, 0xF7, 0x24, 0xFF, 0x01, 0x1C, 0x20, 0x1C, 0x20, 0x30, 0x02, 0x78
    .byte 0x20, 0x1C, 0xFF, 0xF7, 0xE5, 0xFE, 0x01, 0xB0, 0x10, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00
    .byte 0x00, 0x00, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0xFF, 0x01, 0x20, 0x70, 0x47, 0x30, 0xB5, 0x04, 0x1C
    .byte 0x00, 0x25, 0x60, 0x6B, 0x77, 0xF0, 0x54, 0xFB, 0x00, 0x06, 0x00, 0x28, 0x06, 0xD1, 0x60, 0x6B
    .byte 0x77, 0xF0, 0x9A, 0xFE, 0x00, 0x06, 0x00, 0x0E, 0x40, 0x42, 0xC5, 0x0F, 0x28, 0x1C, 0x30, 0xBC
    .byte 0x02, 0xBC, 0x08, 0x47, 0x00, 0xB5, 0x40, 0x6B, 0x77, 0xF0, 0x02, 0xF8, 0x01, 0x21, 0x48, 0x40
    .byte 0x02, 0xBC, 0x08, 0x47, 0x00, 0xB5, 0x40, 0x6B, 0x77, 0xF0, 0x6E, 0xFE, 0x02, 0xBC, 0x08, 0x47
    .byte 0x00, 0xB5, 0x40, 0x6B, 0x77, 0xF0, 0x62, 0xF9, 0x02, 0xBC, 0x08, 0x47, 0x30, 0xB5, 0x04, 0x1C
    .byte 0x00, 0x25, 0xA0, 0x88, 0x02, 0x28, 0x0C, 0xD1, 0x60, 0x6B, 0x77, 0xF0, 0x57, 0xF9, 0x00, 0x06
    .byte 0x00, 0x28, 0x06, 0xD1, 0x60, 0x6B, 0x77, 0xF0, 0x4D, 0xF9, 0x00, 0x06, 0x00, 0x28, 0x00, 0xD1
    .byte 0x01, 0x25, 0x28, 0x1C, 0x30, 0xBC, 0x02, 0xBC, 0x08, 0x47, 0x00, 0x00, 0x10, 0xB5, 0x81, 0xB0
    .byte 0x04, 0x1C, 0x60, 0x6B, 0x77, 0xF0, 0x14, 0xFB, 0x00, 0x06, 0x00, 0x28, 0x21, 0xD1, 0x78, 0x20
    .byte 0x87, 0xF0, 0x7A, 0xFA, 0xF0, 0x30, 0x00, 0x04, 0x00, 0x0C, 0x18, 0x4A, 0x00, 0x99, 0x11, 0x40
    .byte 0x01, 0x43, 0x17, 0x48, 0x01, 0x40, 0xF0, 0x20, 0x80, 0x03, 0x01, 0x43, 0x15, 0x48, 0x01, 0x40
    .byte 0x15, 0x48, 0x01, 0x40, 0x00, 0x91, 0x60, 0x6B, 0x69, 0x46, 0x77, 0xF0, 0x47, 0xFB, 0x20, 0x1C
    .byte 0xFF, 0xF7, 0xAE, 0xFE, 0x01, 0x1C, 0x20, 0x1C, 0x20, 0x30, 0x02, 0x78, 0x20, 0x1C, 0xFF, 0xF7
    .byte 0x6F, 0xFE, 0x60, 0x6B, 0x0A, 0x21, 0x77, 0xF0, 0x01, 0xF8, 0x60, 0x6B, 0x77, 0xF0, 0xAC, 0xF9
    .byte 0x20, 0x1C, 0x00, 0x21, 0x00, 0x22, 0x0E, 0xF0, 0x4D, 0xF8, 0x20, 0x1C, 0xA4, 0x21, 0xFB, 0xF7
    .byte 0xE9, 0xFE, 0x01, 0xB0, 0x10, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00, 0x00, 0x00, 0xFF, 0xFF
    .byte 0xFF, 0xFF, 0x80, 0xFF, 0xFF, 0xFF, 0x7F, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x00, 0xB5, 0x40, 0x6B
    .byte 0x77, 0xF0, 0xF4, 0xFA, 0x01, 0xBC, 0x00, 0x47, 0x10, 0xB5, 0x04, 0x1C, 0x60, 0x6B, 0x76, 0xF0
    .byte 0x87, 0xFF, 0x00, 0x06, 0x00, 0x28, 0x06, 0xD1, 0x60, 0x6B, 0x01, 0x21, 0x76, 0xF0, 0xBC, 0xFF
    .byte 0x60, 0x6B, 0x76, 0xF0, 0xA1, 0xFF, 0x60, 0x6B, 0x77, 0xF0, 0xE4, 0xF8, 0x00, 0x06, 0x00, 0x28
    .byte 0x07, 0xD0, 0x64, 0x20, 0x87, 0xF0, 0x20, 0xFA, 0x1D, 0x28, 0x02, 0xD8, 0x60, 0x6B, 0x77, 0xF0
    .byte 0x79, 0xF9, 0x60, 0x6B, 0x77, 0xF0, 0xDA, 0xF8, 0x00, 0x06, 0x00, 0x28, 0x08, 0xD1, 0x20, 0x1C
    .byte 0x01, 0x21, 0x00, 0x22, 0x0E, 0xF0, 0x0E, 0xF8, 0x20, 0x1C, 0xA3, 0x21, 0xFB, 0xF7, 0xAA, 0xFE
    .byte 0x10, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00, 0x30, 0xB5, 0x04, 0x1C, 0x01, 0x21, 0x00, 0x22
    .byte 0x0E, 0xF0, 0x00, 0xF8, 0x20, 0x1C, 0xA3, 0x21, 0xFB, 0xF7, 0x9C, 0xFE, 0x60, 0x6B, 0x77, 0xF0
    .byte 0xF9, 0xFD, 0x05, 0x1C, 0x20, 0x1C, 0x38, 0x30, 0x01, 0x78, 0x20, 0x1C, 0xFF, 0xF7, 0x5E, 0xFE
    .byte 0x01, 0x1C, 0x60, 0x8C, 0x88, 0x42, 0x02, 0xD0, 0x20, 0x1C, 0x0D, 0xF0, 0x01, 0xFF, 0x28, 0x1C
    .byte 0x30, 0xBC, 0x02, 0xBC, 0x08, 0x47, 0x00, 0x00, 0x10, 0xB5, 0x04, 0x1C, 0x60, 0x6B, 0x77, 0xF0
    .byte 0x31, 0xF9, 0x60, 0x6B, 0x77, 0xF0, 0x9E, 0xF8, 0x00, 0x06, 0x00, 0x28, 0x02, 0xD0, 0x60, 0x6B
    .byte 0x77, 0xF0, 0x38, 0xF9, 0x10, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00, 0x10, 0xB5, 0x81, 0xB0
    .byte 0x04, 0x1C, 0x60, 0x6B, 0x77, 0xF0, 0x64, 0xFA, 0x00, 0x06, 0x00, 0x28, 0x22, 0xD1, 0x78, 0x20
    .byte 0x87, 0xF0, 0xCA, 0xF9, 0xF0, 0x30, 0x00, 0x04, 0x00, 0x0C, 0x18, 0x4A, 0x00, 0x99, 0x11, 0x40
    .byte 0x01, 0x43, 0x17, 0x48, 0x01, 0x40, 0xF0, 0x20, 0x80, 0x03, 0x01, 0x43, 0x80, 0x20, 0x00, 0x04
    .byte 0x01, 0x43, 0x14, 0x48, 0x01, 0x40, 0x00, 0x91, 0x60, 0x6B, 0x69, 0x46, 0x77, 0xF0, 0x96, 0xFA
    .byte 0x20, 0x1C, 0xFF, 0xF7, 0xFD, 0xFD, 0x01, 0x1C, 0x20, 0x1C, 0x20, 0x30, 0x02, 0x78, 0x20, 0x1C
    .byte 0xFF, 0xF7, 0xBE, 0xFD, 0x60, 0x6B, 0x0A, 0x21, 0x76, 0xF0, 0x50, 0xFF, 0x60, 0x6B, 0x77, 0xF0
    .byte 0xFB, 0xF8, 0x20, 0x1C, 0x00, 0x21, 0x00, 0x22, 0x0D, 0xF0, 0x9C, 0xFF, 0x20, 0x1C, 0xA4, 0x21
    .byte 0xFB, 0xF7, 0x38, 0xFE, 0x01, 0xB0, 0x10, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00, 0xFF, 0xFF
    .byte 0xFF, 0xFF, 0x80, 0xFF, 0xFF, 0xFF, 0xFF, 0x00, 0x70, 0xB5, 0x81, 0xB0, 0x04, 0x1C, 0x0E, 0x1C
    .byte 0x15, 0x1C, 0x60, 0x6B, 0x77, 0xF0, 0x1C, 0xFA, 0x00, 0x06, 0x00, 0x28, 0x1C, 0xD1, 0x3B, 0x2D
    .byte 0x1A, 0xD9, 0x25, 0x1C, 0x20, 0x35, 0x28, 0x78, 0xB0, 0x42, 0x03, 0xD0, 0x20, 0x1C, 0x31, 0x1C
    .byte 0x0D, 0xF0, 0x82, 0xFE, 0x0A, 0x49, 0x00, 0x98, 0x08, 0x40, 0x3C, 0x21, 0x08, 0x43, 0x00, 0x90
    .byte 0x60, 0x6B, 0x69, 0x46, 0x77, 0xF0, 0x60, 0xFA, 0x20, 0x1C, 0xFF, 0xF7, 0xB9, 0xFD, 0x01, 0x1C
    .byte 0x2A, 0x78, 0x20, 0x1C, 0xFF, 0xF7, 0x7C, 0xFD, 0x01, 0xB0, 0x70, 0xBC, 0x01, 0xBC, 0x00, 0x47
    .byte 0x00, 0x00, 0xFF, 0xFF

    .section .text.entities_part1.080244D8.tail, "ax", %progbits
.L080244EC:
    .byte 0x70, 0xB5, 0x46, 0x46
    .byte 0x40, 0xB4, 0x84, 0xB0, 0x05, 0x1C, 0x68, 0x6B, 0x77, 0xF0, 0xF0, 0xFC, 0x06, 0x1C, 0x8C, 0x20
    .byte 0xDC, 0xF7, 0x68, 0xF8, 0x80, 0x46, 0x30, 0x1C, 0xFF, 0xF7, 0xDC, 0xFF, 0x04, 0x1C, 0x30, 0x1C
    .byte 0xFF, 0xF7, 0xE2, 0xFF, 0x00, 0x94, 0x01, 0x90, 0x00, 0x26, 0x02, 0x96, 0x03, 0xA8, 0x06, 0x70
    .byte 0x40, 0x46, 0x29, 0x1C, 0x04, 0x22, 0x0B, 0x23, 0x0D, 0xF0, 0xC8, 0xFF, 0x04, 0x1C, 0xA0, 0x46
    .byte 0x38, 0x35, 0x28, 0x78, 0x04, 0x28, 0x15, 0xD1, 0x20, 0x1C, 0x70, 0x30, 0x08, 0x21, 0x3A, 0xF0
    .byte 0x8F, 0xF9, 0x20, 0x1C, 0x84, 0x30, 0x01, 0x21, 0x01, 0x70, 0x02, 0x30, 0x06, 0x70, 0x01, 0x30
    .byte 0x01, 0x70, 0x22, 0x1C, 0x8A, 0x32, 0x02, 0x23, 0x11, 0x78, 0x04, 0x20, 0x40, 0x42, 0x08, 0x40
    .byte 0x18, 0x43, 0x10, 0x70, 0x40, 0x46, 0x04, 0xB0, 0x08, 0xBC, 0x98, 0x46, 0x70, 0xBC, 0x02, 0xBC
    .byte 0x08, 0x47, 0x00, 0x00, 0xCB, 0x1F, 0x03, 0x80, 0x13, 0x1F, 0x43, 0x80, 0x07, 0x31, 0x81, 0x80
    .byte 0x05, 0x32, 0xC2, 0x80, 0x70, 0x47, 0x00, 0x00, 0xCB, 0x1F, 0x03, 0x80, 0x13, 0x1F, 0x43, 0x80
    .byte 0x07, 0x31, 0x81, 0x80, 0x05, 0x32, 0xC2, 0x80, 0x70, 0x47, 0x00, 0x00, 0x00, 0xB5, 0x02, 0x1C
    .byte 0x11, 0x1C, 0x30, 0x31, 0x00, 0x20, 0x08, 0x70, 0x90, 0x8C, 0x00, 0x28, 0x01, 0xD0, 0x01, 0x38
    .byte 0x00, 0xE0, 0xD0, 0x8C, 0x90, 0x84, 0x12, 0x69, 0x00, 0x2A, 0x04, 0xD0, 0x50, 0x68, 0xC1, 0x68
    .byte 0x10, 0x1C, 0xAF, 0xF0, 0xA5, 0xF9, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00, 0xCB, 0x1F, 0x03, 0x80
    .byte 0x13, 0x1C, 0x09, 0x3B, 0x43, 0x80, 0x07, 0x31, 0x81, 0x80, 0x05, 0x32, 0xC2, 0x80, 0x70, 0x47



    .section .text.entities_part1.08024724, "ax", %progbits
    thumb_func_start func_08024724
func_08024724: @ 0x08024724
    push {r4, r5, r6, r7, lr}
    mov r7, sl
    mov r6, sb
    mov r5, r8
    push {r5, r6, r7}
    sub sp, #0x5c
    adds r7, r0, #0
    str r1, [sp, #0x40]
    adds r6, r2, #0
    adds r0, r1, #0
    adds r1, r6, #0
    bl func_080AC070
    lsls r0, r0, #0x18
    lsrs r0, r0, #0x18
    cmp r0, #0
    bne .L08024748
    b .L0802495C
.L08024748:
    movs r0, #6
    ldrsh r4, [r6, r0]
    subs r0, r4, #1
    movs r1, #7
    ands r0, r1
    mvns r0, r0
    mov sb, r0
    mov r3, sb
    str r3, [sp, #0x44]
    ldrh r2, [r6, #2]
    adds r0, r1, #0
    ands r0, r2
    movs r2, #8
    subs r0, r2, r0
    str r0, [sp, #0x4c]
    str r0, [sp, #0x48]
    movs r0, #4
    ldrsh r5, [r6, r0]
    subs r0, r5, #1
    ands r0, r1
    mvns r0, r0
    mov r8, r0
    mov r3, r8
    str r3, [sp, #0x50]
    ldrh r0, [r6]
    ands r1, r0
    subs r2, r2, r1
    mov sl, r2
    mov r0, sl
    str r0, [sp, #0x54]
    movs r1, #0
    ldrsh r3, [r6, r1]
    movs r0, #2
    ldrsh r1, [r6, r0]
    mov r2, sp
    mov r0, sp
    strh r3, [r0]
    add r1, sb
    strh r1, [r2, #2]
    strh r5, [r2, #4]
    add r4, sb
    strh r4, [r2, #6]
    ldr r0, [sp, #0x40]
    mov r1, sp
    bl func_080AC070
    movs r1, #1
    eors r0, r1
    lsls r0, r0, #0x18
    lsrs r5, r0, #0x18
    cmp r5, #0
    beq .L080247BA
    movs r0, #0
    strh r0, [r7]
    mov r3, sb
    strh r3, [r7, #2]
    b .L08024960
.L080247BA:
    movs r0, #0
    ldrsh r3, [r6, r0]
    movs r1, #2
    ldrsh r0, [r6, r1]
    movs r1, #4
    ldrsh r4, [r6, r1]
    movs r1, #6
    ldrsh r2, [r6, r1]
    add r1, sp, #8
    strh r3, [r1]
    ldr r3, [sp, #0x4c]
    adds r0, r3, r0
    strh r0, [r1, #2]
    strh r4, [r1, #4]
    adds r2, r3, r2
    strh r2, [r1, #6]
    ldr r0, [sp, #0x40]
    bl func_080AC070
    movs r1, #1
    eors r0, r1
    lsls r0, r0, #0x18
    lsrs r0, r0, #0x18
    str r0, [sp, #0x58]
    cmp r0, #0
    beq .L080247F8
    strh r5, [r7]
    add r3, sp, #0x4c
    ldrh r3, [r3]
    strh r3, [r7, #2]
    b .L08024960
.L080247F8:
    movs r5, #0
    ldrsh r0, [r6, r5]
    movs r1, #2
    ldrsh r3, [r6, r1]
    movs r5, #4
    ldrsh r2, [r6, r5]
    movs r1, #6
    ldrsh r4, [r6, r1]
    add r1, sp, #0x10
    add r0, r8
    strh r0, [r1]
    strh r3, [r1, #2]
    add r2, r8
    strh r2, [r1, #4]
    strh r4, [r1, #6]
    ldr r0, [sp, #0x40]
    bl func_080AC070
    movs r3, #1
    eors r0, r3
    lsls r0, r0, #0x18
    lsrs r5, r0, #0x18
    cmp r5, #0
    beq .L08024832
    mov r5, r8
    strh r5, [r7]
    add r0, sp, #0x58
    ldrh r0, [r0]
    b .L0802495E
.L08024832:
    movs r1, #0
    ldrsh r0, [r6, r1]
    movs r1, #2
    ldrsh r3, [r6, r1]
    movs r1, #4
    ldrsh r2, [r6, r1]
    movs r1, #6
    ldrsh r4, [r6, r1]
    add r1, sp, #0x18
    add r0, sl
    strh r0, [r1]
    strh r3, [r1, #2]
    add r2, sl
    strh r2, [r1, #4]
    strh r4, [r1, #6]
    ldr r0, [sp, #0x40]
    bl func_080AC070
    movs r3, #1
    eors r0, r3
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08024868
    mov r0, sl
    strh r0, [r7]
    strh r5, [r7, #2]
    b .L08024960
.L08024868:
    movs r1, #0
    ldrsh r0, [r6, r1]
    movs r3, #2
    ldrsh r2, [r6, r3]
    movs r5, #4
    ldrsh r3, [r6, r5]
    movs r1, #6
    ldrsh r4, [r6, r1]
    add r1, sp, #0x20
    add r0, r8
    strh r0, [r1]
    add r2, sb
    strh r2, [r1, #2]
    add r3, r8
    strh r3, [r1, #4]
    add r4, sb
    strh r4, [r1, #6]
    ldr r0, [sp, #0x40]
    bl func_080AC070
    movs r3, #1
    eors r0, r3
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L080248A2
    mov r5, r8
    strh r5, [r7]
    mov r0, sb
    b .L0802495E
.L080248A2:
    movs r1, #0
    ldrsh r0, [r6, r1]
    movs r3, #2
    ldrsh r2, [r6, r3]
    movs r5, #4
    ldrsh r3, [r6, r5]
    movs r1, #6
    ldrsh r4, [r6, r1]
    add r1, sp, #0x28
    add r0, sl
    strh r0, [r1]
    ldr r5, [sp, #0x44]
    adds r2, r5, r2
    strh r2, [r1, #2]
    add r3, sl
    strh r3, [r1, #4]
    adds r4, r5, r4
    strh r4, [r1, #6]
    ldr r0, [sp, #0x40]
    bl func_080AC070
    movs r1, #1
    eors r0, r1
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L080248DE
    mov r3, sl
    strh r3, [r7]
    strh r5, [r7, #2]
    b .L08024960
.L080248DE:
    movs r1, #0
    ldrsh r0, [r6, r1]
    movs r3, #2
    ldrsh r2, [r6, r3]
    movs r5, #4
    ldrsh r3, [r6, r5]
    movs r1, #6
    ldrsh r4, [r6, r1]
    add r1, sp, #0x30
    ldr r5, [sp, #0x50]
    adds r0, r5, r0
    strh r0, [r1]
    ldr r0, [sp, #0x4c]
    adds r2, r0, r2
    strh r2, [r1, #2]
    adds r3, r5, r3
    strh r3, [r1, #4]
    adds r4, r0, r4
    strh r4, [r1, #6]
    ldr r0, [sp, #0x40]
    bl func_080AC070
    movs r1, #1
    eors r0, r1
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802491A
    strh r5, [r7]
    add r5, sp, #0x4c
    b .L08024956
.L0802491A:
    movs r1, #0
    ldrsh r0, [r6, r1]
    movs r3, #2
    ldrsh r2, [r6, r3]
    movs r5, #4
    ldrsh r3, [r6, r5]
    movs r1, #6
    ldrsh r4, [r6, r1]
    add r1, sp, #0x38
    ldr r5, [sp, #0x54]
    adds r0, r5, r0
    strh r0, [r1]
    ldr r0, [sp, #0x48]
    adds r2, r0, r2
    strh r2, [r1, #2]
    adds r3, r5, r3
    strh r3, [r1, #4]
    adds r4, r0, r4
    strh r4, [r1, #6]
    ldr r0, [sp, #0x40]
    bl func_080AC070
    movs r1, #1
    eors r0, r1
    lsls r0, r0, #0x18
    lsrs r0, r0, #0x18
    cmp r0, #0
    beq .L0802495C
    strh r5, [r7]
    add r5, sp, #0x48
.L08024956:
    ldrh r5, [r5]
    strh r5, [r7, #2]
    b .L08024960
.L0802495C:
    strh r0, [r7]
.L0802495E:
    strh r0, [r7, #2]
.L08024960:
    adds r0, r7, #0
    add sp, #0x5c
    pop {r3, r4, r5}
    mov r8, r3
    mov sb, r4
    mov sl, r5
    pop {r4, r5, r6, r7}
    pop {r1}
    bx r1
    .align 2, 0



    .section .text.entities_part1.08024B9C, "ax", %progbits
    thumb_func_start func_08024B9C
func_08024B9C: @ 0x08024B9C
    push {r4, r5, lr}
    adds r4, r0, #0
    movs r5, #2
    ldrh r0, [r4, #4]
    cmp r0, #0x13
    bne .L08024BCA
    ldr r1, [r4, #0x34]
    ldr r2, .L08024BE4 @ =0x00002171
    adds r0, r1, r2
    ldrb r0, [r0]
    lsls r0, r0, #0x1b
    lsrs r0, r0, #0x1e
    cmp r0, #1
    beq .L08024BC8
    subs r2, #0x25
    adds r0, r1, r2
    movs r1, #0x4a
    bl func_0809C694
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08024BCA
.L08024BC8:
    movs r5, #1
.L08024BCA:
    adds r0, r4, #0
    adds r0, #0x21
    strb r5, [r0]
    movs r0, #0xa1
    lsls r0, r0, #2
    bl __builtin_new
    adds r1, r4, #0
    bl func_0802B260
    pop {r4, r5}
    pop {r1}
    bx r1
    .align 2, 0
.L08024BE4: .4byte 0x00002171




    .section .text.entities_part1.08024CD0, "ax", %progbits
    thumb_func_start func_08024CD0
func_08024CD0: @ 0x08024CD0
    push {r4, r5, r6, r7, lr}
    mov r7, sl
    mov r6, sb
    mov r5, r8
    push {r5, r6, r7}
    sub sp, #0x60
    adds r5, r0, #0
    mov sl, r1
    ldr r1, [r1]
    mov r0, sl
    ldrb r2, [r0, #4]
    adds r0, r5, #0
    bl func_0802F0EC
    adds r0, r5, #0
    bl func_0802CDCC
    adds r0, r5, #0
    bl func_0802D158
    lsls r0, r0, #0x10
    lsrs r1, r0, #0x10
    ldrh r0, [r5, #0x22]
    cmp r0, r1
    beq .L08024D08
    adds r0, r5, #0
    bl SetAnim__12AActorEntityUi
.L08024D08:
    ldrh r0, [r5, #0x24]
    cmp r0, #0
    beq .L08024D12
    subs r0, #1
    b .L08024D14
.L08024D12:
    ldrh r0, [r5, #0x26]
.L08024D14:
    strh r0, [r5, #0x24]
    ldr r2, [r5, #0x10]
    cmp r2, #0
    beq .L08024D26
    ldr r0, [r2, #4]
    ldr r1, [r0, #0xc]
    adds r0, r2, #0
    @ 08024D22 bl [func_0802B3B4]
    bl _call_via_r1
.L08024D26:
    ldr r0, [r5, #0x18]
    cmp r0, #0
    bne .L08024D34
    ldr r0, [r5, #0x1c]
    cmp r0, #0
    bne .L08024D34
    b .L08024F7E
.L08024D34:
    adds r0, r5, #0
    adds r0, #0x3c
    ldrb r0, [r0]
    cmp r0, #0x35
    bne .L08024D40
    b .L08024F7E
.L08024D40:
    ldr r1, [r5, #0x14]
    add r0, sp, #0x20
    ldr r2, [r1, #0xc]
    adds r1, r5, #0
    @ 08024D48 bl [FarmerEntity_GetCollisionBox]
    bl _call_via_r2
    ldr r0, [sp, #0x20]
    ldr r1, [sp, #0x24]
    str r0, [sp, #0x28]
    str r1, [sp, #0x2c]
    add r3, sp, #0x28
    movs r0, #0
    str r0, [sp]
    adds r0, r5, #0
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, #1
    beq .L08024DD2
    cmp r0, #1
    bgt .L08024D6E
    cmp r0, #0
    beq .L08024D7A
    b .L08024EEE
.L08024D6E:
    cmp r0, #2
    beq .L08024E22
    cmp r0, #3
    bne .L08024D78
    b .L08024E94
.L08024D78:
    b .L08024EEE
.L08024D7A:
    movs r1, #0
    ldrsh r0, [r3, r1]
    movs r2, #6
    ldrsh r1, [r3, r2]
    add r2, sp, #0x30
    subs r1, #1
    strh r0, [r2]
    strh r1, [r2, #2]
    ldr r0, [sp]
    cmp r0, #3
    bhi .L08024DA6
    lsls r0, r0, #2
    adds r0, #4
    mov r7, sp
    adds r1, r7, r0
    cmp r1, #0
    beq .L08024DA0
    ldr r0, [sp, #0x30]
    str r0, [r1]
.L08024DA0:
    ldr r0, [sp]
    adds r0, #1
    str r0, [sp]
.L08024DA6:
    movs r1, #4
    ldrsh r0, [r3, r1]
    movs r2, #6
    ldrsh r1, [r3, r2]
    add r2, sp, #0x34
    subs r0, #1
    subs r1, #1
    strh r0, [r2]
    strh r1, [r2, #2]
    ldr r0, [sp]
    cmp r0, #3
    bls .L08024DC0
    b .L08024EEE
.L08024DC0:
    lsls r0, r0, #2
    adds r0, #4
    mov r7, sp
    adds r1, r7, r0
    cmp r1, #0
    bne .L08024DCE
    b .L08024EE8
.L08024DCE:
    ldr r0, [sp, #0x34]
    b .L08024EE6
.L08024DD2:
    movs r1, #0
    ldrsh r0, [r3, r1]
    movs r7, #2
    ldrsh r2, [r3, r7]
    add r1, sp, #0x38
    strh r0, [r1]
    strh r2, [r1, #2]
    ldr r0, [sp]
    cmp r0, #3
    bhi .L08024DFC
    lsls r0, r0, #2
    adds r0, #4
    mov r2, sp
    adds r1, r2, r0
    cmp r1, #0
    beq .L08024DF6
    ldr r0, [sp, #0x38]
    str r0, [r1]
.L08024DF6:
    ldr r0, [sp]
    adds r0, #1
    str r0, [sp]
.L08024DFC:
    movs r7, #4
    ldrsh r0, [r3, r7]
    movs r1, #2
    ldrsh r2, [r3, r1]
    add r1, sp, #0x3c
    subs r0, #1
    strh r0, [r1]
    strh r2, [r1, #2]
    ldr r0, [sp]
    cmp r0, #3
    bhi .L08024EEE
    lsls r0, r0, #2
    adds r0, #4
    mov r2, sp
    adds r1, r2, r0
    cmp r1, #0
    beq .L08024EE8
    ldr r0, [sp, #0x3c]
    b .L08024EE6
.L08024E22:
    movs r7, #0
    ldrsh r0, [r3, r7]
    movs r1, #2
    ldrsh r2, [r3, r1]
    add r1, sp, #0x40
    strh r0, [r1]
    strh r2, [r1, #2]
    ldr r0, [sp]
    cmp r0, #3
    bhi .L08024E4C
    lsls r0, r0, #2
    adds r0, #4
    mov r2, sp
    adds r1, r2, r0
    cmp r1, #0
    beq .L08024E46
    ldr r0, [sp, #0x40]
    str r0, [r1]
.L08024E46:
    ldr r0, [sp]
    adds r0, #1
    str r0, [sp]
.L08024E4C:
    movs r7, #0
    ldrsh r0, [r3, r7]
    movs r2, #6
    ldrsh r1, [r3, r2]
    add r2, sp, #0x44
    subs r1, #1
    strh r0, [r2]
    strh r1, [r2, #2]
    ldr r0, [sp]
    cmp r0, #3
    bhi .L08024EEE
    lsls r0, r0, #2
    adds r0, #4
    mov r7, sp
    adds r1, r7, r0
    cmp r1, #0
    beq .L08024EE8
    ldr r0, [sp, #0x44]
    b .L08024EE6
.L08024E72:
    mov r0, r8
    ldr r2, [r0]
    movs r7, #0xa0
    lsls r7, r7, #1
    adds r2, r2, r7
    lsls r0, r1, #0xf
    lsrs r0, r0, #0x11
    add r1, sp, #0x50
    movs r3, #0
    str r0, [sp, #0x50]
    str r3, [r1, #4]
    ldr r3, [r2]
    mov r0, r8
    movs r2, #0
    @ 08024E8E bl [func_0801D9A8]
    bl _call_via_r3
    b .L08024F68
.L08024E94:
    movs r1, #4
    ldrsh r0, [r3, r1]
    movs r7, #2
    ldrsh r2, [r3, r7]
    add r1, sp, #0x48
    subs r0, #1
    strh r0, [r1]
    strh r2, [r1, #2]
    ldr r0, [sp]
    cmp r0, #3
    bhi .L08024EC0
    lsls r0, r0, #2
    adds r0, #4
    mov r2, sp
    adds r1, r2, r0
    cmp r1, #0
    beq .L08024EBA
    ldr r0, [sp, #0x48]
    str r0, [r1]
.L08024EBA:
    ldr r0, [sp]
    adds r0, #1
    str r0, [sp]
.L08024EC0:
    movs r7, #4
    ldrsh r0, [r3, r7]
    movs r2, #6
    ldrsh r1, [r3, r2]
    add r2, sp, #0x4c
    subs r0, #1
    subs r1, #1
    strh r0, [r2]
    strh r1, [r2, #2]
    ldr r0, [sp]
    cmp r0, #3
    bhi .L08024EEE
    lsls r0, r0, #2
    adds r0, #4
    mov r7, sp
    adds r1, r7, r0
    cmp r1, #0
    beq .L08024EE8
    ldr r0, [sp, #0x4c]
.L08024EE6:
    str r0, [r1]
.L08024EE8:
    ldr r0, [sp]
    adds r0, #1
    str r0, [sp]
.L08024EEE:
    ldrh r2, [r5, #4]
    ldr r0, [r5]
    mov r8, r0
    add r4, sp, #0x14
    ldr r1, [r0]
    adds r0, r4, #0
    ldr r3, [r1, #0x34]
    mov r1, r8
    @ 08024EFE bl [func_0801A054]
    bl _call_via_r3
    mov ip, r4
    movs r6, #0
    ldr r0, [sp]
    add r1, sp, #4
    mov sb, r1
    cmp r6, r0
    bhs .L08024F68
    ldr r3, [sp, #0x14]
    ldrh r2, [r4, #8]
    str r2, [sp, #0x5c]
    mov r4, sb
.L08024F18:
    ldrh r0, [r4]
    lsls r0, r0, #0x10
    asrs r2, r0, #0x13
    ldrh r0, [r4, #2]
    lsls r0, r0, #0x10
    asrs r0, r0, #0x13
    cmp r3, #0
    beq .L08024F4E
    ldr r7, [sp, #0x5c]
    cmp r2, r7
    bhi .L08024F4E
    mov r1, ip
    ldrh r1, [r1, #0xa]
    cmp r0, r1
    bhi .L08024F4E
    mov r7, ip
    ldr r1, [r7, #4]
    ldr r7, [sp, #0x5c]
    muls r0, r7, r0
    adds r0, r0, r2
    cmp r1, #0
    beq .L08024F48
    adds r0, r1, r0
    ldrb r0, [r0]
.L08024F48:
    lsls r0, r0, #2
    adds r0, r3, r0
    b .L08024F50
.L08024F4E:
    movs r0, #0
.L08024F50:
    cmp r0, #0
    beq .L08024F5E
    ldr r1, [r0]
    ldr r0, .L0802500C @ =0x0001FFFC
    ands r0, r1
    cmp r0, #0
    bne .L08024E72
.L08024F5E:
    adds r4, #4
    adds r6, #1
    ldr r0, [sp]
    cmp r6, r0
    blo .L08024F18
.L08024F68:
    ldr r0, [sp]
    lsls r0, r0, #2
    adds r0, #4
    mov r2, sp
    adds r1, r2, r0
    mov r0, sb
    cmp r0, r1
    beq .L08024F7E
.L08024F78:
    adds r0, #4
    cmp r0, r1
    bne .L08024F78
.L08024F7E:
    mov r7, sl
    ldrb r0, [r7, #4]
    cmp r0, #0
    beq .L08024FFC
    ldr r0, [r5, #0x34]
    ldr r1, .L08025010 @ =0x00001C34
    adds r4, r0, r1
    adds r0, r4, #0
    bl IsEmpty__C9ToolStack
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08024FBA
    adds r0, r4, #0
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    add r0, sp, #0x58
    strb r1, [r0]
    bl GetId__C4Tool
    cmp r0, #0x4e
    bne .L08024FBA
    ldr r0, [r5, #0x14]
    ldr r3, [r0, #0x64]
    adds r0, r5, #0
    movs r1, #1
    movs r2, #0
    bl _call_via_r3
.L08024FBA:
    adds r0, r4, #0
    bl IsEmpty__C9ToolStack
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08024FFC
    adds r0, r4, #0
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    mov r0, sp
    adds r0, #0x59
    strb r1, [r0]
    bl GetId__C4Tool
    cmp r0, #0x4f
    bne .L08024FFC
    ldr r0, [r5, #0x34]
    ldr r2, .L08025014 @ =0x00001BD8
    adds r0, r0, r2
    bl func_0800EE2C
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08024FFC
    ldr r0, [r5, #0x14]
    movs r2, #1
    rsbs r2, r2, #0
    ldr r3, [r0, #0x64]
    adds r0, r5, #0
    movs r1, #0
    bl _call_via_r3
.L08024FFC:
    add sp, #0x60
    pop {r3, r4, r5}
    mov r8, r3
    mov sb, r4
    mov sl, r5
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0
    .align 2, 0
.L0802500C: .4byte 0x0001FFFC
.L08025010: .4byte 0x00001C34
.L08025014: .4byte 0x00001BD8





    .section .text.entities_part1.08025294, "ax", %progbits
    thumb_func_start func_08025294
func_08025294: @ 0x08025294
    push {r4, r5, r6, r7, lr}
    sub sp, #0x28
    mov ip, r1
    adds r7, r2, #0
    mov r4, sp
    movs r1, #0x88
    lsls r1, r1, #1
    subs r2, r1, #1
    mov r0, sp
    strh r2, [r0]
    movs r2, #0xaf
    strh r2, [r4, #2]
    ldr r0, .L080252F0 @ =0x000002B1
    adds r1, r1, r0
    strh r1, [r4, #4]
    subs r0, #0x70
    strh r0, [r4, #6]
    mov r0, sp
    movs r1, #0
    ldrsh r0, [r0, r1]
    movs r6, #4
    ldrsh r5, [r3, r6]
    subs r0, r0, r5
    movs r1, #0
    ldrsh r4, [r3, r1]
    ldr r6, .L080252F4 @ =0xFFFFFC3F
    adds r1, r4, r6
    ands r0, r1
    movs r1, #6
    ldrsh r6, [r3, r1]
    subs r2, r2, r6
    ands r0, r2
    movs r2, #2
    ldrsh r3, [r3, r2]
    ldr r2, .L080252F8 @ =0xFFFFFDBF
    adds r1, r3, r2
    ands r0, r1
    cmp r0, #0
    bge .L08025360
    cmp r7, #1
    beq .L0802531A
    cmp r7, #1
    bgt .L080252FC
    cmp r7, #0
    beq .L08025306
    b .L08025360
    .align 2, 0
.L080252F0: .4byte 0x000002B1
.L080252F4: .4byte 0xFFFFFC3F
.L080252F8: .4byte 0xFFFFFDBF
.L080252FC:
    cmp r7, #2
    beq .L08025330
    cmp r7, #3
    beq .L0802533A
    b .L08025360
.L08025306:
    subs r2, r6, r3
    adds r2, #0x10
    add r1, sp, #8
    strh r4, [r1]
    adds r0, r2, r3
    strh r0, [r1, #2]
    strh r5, [r1, #4]
    adds r2, r2, r6
    strh r2, [r1, #6]
    b .L0802534C
.L0802531A:
    subs r0, r6, r3
    adds r0, #0x10
    rsbs r0, r0, #0
    add r1, sp, #0x10
    strh r4, [r1]
    adds r2, r0, r3
    strh r2, [r1, #2]
    strh r5, [r1, #4]
    adds r0, r0, r6
    strh r0, [r1, #6]
    b .L0802534C
.L08025330:
    subs r0, r5, r4
    adds r0, #0x10
    rsbs r0, r0, #0
    add r1, sp, #0x18
    b .L08025340
.L0802533A:
    subs r0, r5, r4
    adds r0, #0x10
    add r1, sp, #0x20
.L08025340:
    adds r2, r0, r4
    strh r2, [r1]
    strh r3, [r1, #2]
    adds r0, r0, r5
    strh r0, [r1, #4]
    strh r6, [r1, #6]
.L0802534C:
    mov r0, ip
    bl func_080AC070
    movs r1, #1
    eors r0, r1
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08025360
    movs r0, #1
    b .L08025362
.L08025360:
    movs r0, #0
.L08025362:
    add sp, #0x28
    pop {r4, r5, r6, r7}
    pop {r1}
    bx r1
    .align 2, 0

    .section .text.entities_part1.0802536C, "ax", %progbits
    thumb_func_start func_0802536C
func_0802536C: @ 0x0802536C
    push {r4, r5, r6, r7, lr}
    mov r7, sl
    mov r6, sb
    mov r5, r8
    push {r5, r6, r7}
    sub sp, #0x5c
    mov sb, r0
    mov r8, r1
    str r2, [sp, #0x4c]
    ldrh r0, [r0, #4]
    str r0, [sp, #0x50]
    mov r2, sb
    ldr r1, [r2]
    ldr r2, [r1]
    add r0, sp, #8
    ldr r3, [r2, #0x34]
    ldr r2, [sp, #0x50]
    bl _call_via_r3
    add r5, sp, #8
    mov sl, r5
    mov r0, sb
    movs r1, #0xa
    ldrsh r0, [r0, r1]
    str r0, [sp, #0x54]
    mov r2, sb
    movs r5, #0xe
    ldrsh r2, [r2, r5]
    str r2, [sp, #0x58]
    mov r0, sb
    ldr r1, [r0, #0x14]
    add r4, sp, #0x14
    adds r0, r4, #0
    ldr r2, [r1, #0xc]
    mov r1, sb
    bl _call_via_r2
    adds r6, r4, #0
    mov r1, r8
    cmp r1, #1
    beq .L08025404
    cmp r1, #1
    bgt .L080253C8
    cmp r1, #0
    beq .L080253D4
    b .L080257C0
.L080253C8:
    mov r2, r8
    cmp r2, #2
    beq .L08025436
    cmp r2, #3
    beq .L0802545E
    b .L080257C0
.L080253D4:
    ldr r0, [sp, #0x4c]
    str r0, [sp, #0x1c]
    add r2, sp, #0x1c
    mov r5, r8
    str r5, [sp]
    str r5, [sp, #4]
    add r0, sp, #8
    adds r1, r6, #0
    movs r3, #0
    bl func_080AC124
    ldr r1, [sp, #0x1c]
    cmp r1, #0
    beq .L0802549C
    ldr r0, [sp, #0x58]
    adds r0, r0, r1
    str r0, [sp, #0x58]
    ldrh r0, [r6, #2]
    adds r0, r0, r1
    strh r0, [r6, #2]
    ldrh r0, [r6, #6]
    adds r0, r0, r1
    strh r0, [r6, #6]
    b .L0802548E
.L08025404:
    ldr r0, [sp, #0x4c]
    rsbs r0, r0, #0
    str r0, [sp, #0x1c]
    add r3, sp, #0x1c
    movs r0, #0
    str r0, [sp]
    str r0, [sp, #4]
    add r0, sp, #8
    adds r1, r6, #0
    movs r2, #0
    bl func_080AC124
    ldr r1, [sp, #0x1c]
    cmp r1, #0
    beq .L0802549C
    ldr r2, [sp, #0x58]
    adds r2, r2, r1
    str r2, [sp, #0x58]
    ldrh r0, [r6, #2]
    adds r0, r0, r1
    strh r0, [r6, #2]
    ldrh r0, [r6, #6]
    adds r0, r0, r1
    strh r0, [r6, #6]
    b .L0802548E
.L08025436:
    ldr r0, [sp, #0x4c]
    rsbs r0, r0, #0
    str r0, [sp, #0x1c]
    add r0, sp, #0x1c
    str r0, [sp]
    movs r0, #0
    str r0, [sp, #4]
    add r0, sp, #8
    adds r1, r6, #0
    movs r2, #0
    movs r3, #0
    bl func_080AC124
    ldr r1, [sp, #0x1c]
    cmp r1, #0
    beq .L0802549C
    ldr r5, [sp, #0x54]
    adds r5, r5, r1
    str r5, [sp, #0x54]
    b .L08025482
.L0802545E:
    ldr r0, [sp, #0x4c]
    str r0, [sp, #0x1c]
    movs r0, #0
    str r0, [sp]
    add r0, sp, #0x1c
    str r0, [sp, #4]
    add r0, sp, #8
    adds r1, r6, #0
    movs r2, #0
    movs r3, #0
    bl func_080AC124
    ldr r1, [sp, #0x1c]
    cmp r1, #0
    beq .L0802549C
    ldr r0, [sp, #0x54]
    adds r0, r0, r1
    str r0, [sp, #0x54]
.L08025482:
    ldrh r0, [r6]
    adds r0, r0, r1
    strh r0, [r6]
    ldrh r0, [r6, #4]
    adds r0, r0, r1
    strh r0, [r6, #4]
.L0802548E:
    ldr r1, [sp, #0x1c]
    cmp r1, #0
    bge .L08025496
    rsbs r1, r1, #0
.L08025496:
    ldr r0, [sp, #0x4c]
    subs r0, r0, r1
    str r0, [sp, #0x4c]
.L0802549C:
    ldr r0, [sp, #0x4c]
    cmp r0, #0
    bne .L080254A4
    b .L080257B2
.L080254A4:
    mov r7, r8
    movs r0, #0
    str r0, [sp, #0x48]
    ldr r0, [sp, #0x14]
    ldr r1, [sp, #0x18]
    str r0, [sp, #0x20]
    str r1, [sp, #0x24]
    cmp r7, #1
    beq .L080254DA
    cmp r7, #1
    bgt .L080254C0
    cmp r7, #0
    beq .L080254CA
    b .L0802550A
.L080254C0:
    cmp r7, #2
    beq .L080254EA
    cmp r7, #3
    beq .L080254FC
    b .L0802550A
.L080254CA:
    add r1, sp, #0x20
    ldrh r0, [r1, #2]
    adds r0, #1
    strh r0, [r1, #2]
    ldrh r0, [r1, #6]
    adds r0, #1
    strh r0, [r1, #6]
    b .L0802550A
.L080254DA:
    add r1, sp, #0x20
    ldrh r0, [r1, #2]
    subs r0, #1
    strh r0, [r1, #2]
    ldrh r0, [r1, #6]
    subs r0, #1
    strh r0, [r1, #6]
    b .L0802550A
.L080254EA:
    add r1, sp, #0x20
    movs r2, #1
    rsbs r2, r2, #0
    ldrh r0, [r1]
    adds r0, r0, r2
    strh r0, [r1]
    ldrh r0, [r1, #4]
    adds r0, r0, r2
    b .L08025508
.L080254FC:
    add r1, sp, #0x20
    ldrh r0, [r1]
    adds r0, #1
    strh r0, [r1]
    ldrh r0, [r1, #4]
    adds r0, #1
.L08025508:
    strh r0, [r1, #4]
.L0802550A:
    mov r1, r8
    cmp r1, #0
    bge .L08025512
    b .L08025664
.L08025512:
    cmp r1, #1
    ble .L0802556A
    cmp r1, #3
    ble .L0802551C
    b .L08025664
.L0802551C:
    add r4, sp, #0x20
    movs r3, #1
    rsbs r3, r3, #0
    movs r0, #8
    str r0, [sp]
    mov r0, sb
    mov r1, sl
    adds r2, r4, #0
    bl func_08025248
    str r0, [sp, #0x48]
    cmp r0, #8
    bhi .L08025616
    rsbs r2, r0, #0
    add r0, sp, #0x14
    movs r5, #0
    ldrsh r3, [r0, r5]
    movs r1, #2
    ldrsh r0, [r6, r1]
    movs r5, #4
    ldrsh r4, [r6, r5]
    movs r1, #6
    ldrsh r5, [r6, r1]
    add r1, sp, #0x28
    strh r3, [r1]
    adds r0, r2, r0
    strh r0, [r1, #2]
    strh r4, [r1, #4]
    adds r2, r2, r5
    strh r2, [r1, #6]
    mov r0, sl
    bl func_080AC070
    movs r1, #1
    eors r0, r1
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08025616
    b .L0802560E
.L0802556A:
    add r4, sp, #0x20
    movs r3, #1
    rsbs r3, r3, #0
    movs r0, #8
    str r0, [sp]
    mov r0, sb
    mov r1, sl
    adds r2, r4, #0
    bl func_080251FC
    str r0, [sp, #0x48]
    cmp r0, #8
    bhi .L080255BA
    rsbs r2, r0, #0
    add r0, sp, #0x14
    movs r5, #0
    ldrsh r0, [r0, r5]
    movs r1, #2
    ldrsh r3, [r6, r1]
    movs r5, #4
    ldrsh r4, [r6, r5]
    movs r1, #6
    ldrsh r5, [r6, r1]
    add r1, sp, #0x30
    adds r0, r2, r0
    strh r0, [r1]
    strh r3, [r1, #2]
    adds r2, r2, r4
    strh r2, [r1, #4]
    strh r5, [r1, #6]
    mov r0, sl
    bl func_080AC070
    movs r1, #1
    eors r0, r1
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L080255BA
    movs r7, #2
    b .L08025664
.L080255BA:
    movs r0, #8
    str r0, [sp]
    mov r0, sb
    mov r1, sl
    add r2, sp, #0x20
    movs r3, #1
    bl func_080251FC
    str r0, [sp, #0x48]
    cmp r0, #8
    bhi .L08025664
    add r0, sp, #0x14
    movs r2, #2
    ldrsh r3, [r6, r2]
    movs r5, #4
    ldrsh r4, [r6, r5]
    movs r2, #6
    ldrsh r1, [r6, r2]
    mov ip, r1
    add r1, sp, #0x38
    add r2, sp, #0x48
    movs r5, #0
    ldrsh r0, [r0, r5]
    ldrh r5, [r2]
    adds r0, r0, r5
    strh r0, [r1]
    strh r3, [r1, #2]
    ldrh r0, [r2]
    adds r0, r0, r4
    strh r0, [r1, #4]
    mov r0, ip
    strh r0, [r1, #6]
    mov r0, sl
    bl func_080AC070
    movs r1, #1
    eors r0, r1
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08025664
    movs r7, #3
    b .L08025664
.L0802560E:
    movs r7, #1
    b .L08025664
.L08025612:
    ldr r0, [r4]
    b .L080256C4
.L08025616:
    movs r0, #8
    str r0, [sp]
    mov r0, sb
    mov r1, sl
    add r2, sp, #0x20
    movs r3, #1
    bl func_08025248
    str r0, [sp, #0x48]
    cmp r0, #8
    bhi .L08025664
    add r0, sp, #0x14
    movs r1, #0
    ldrsh r0, [r0, r1]
    movs r2, #2
    ldrsh r3, [r6, r2]
    movs r5, #4
    ldrsh r4, [r6, r5]
    movs r1, #6
    ldrsh r5, [r6, r1]
    add r1, sp, #0x40
    strh r0, [r1]
    add r2, sp, #0x48
    ldrh r0, [r2]
    adds r0, r0, r3
    strh r0, [r1, #2]
    strh r4, [r1, #4]
    ldrh r0, [r2]
    adds r0, r0, r5
    strh r0, [r1, #6]
    mov r0, sl
    bl func_080AC070
    movs r1, #1
    eors r0, r1
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08025664
    movs r7, #0
.L08025664:
    cmp r7, r8
    beq .L0802566E
    ldr r0, [sp, #0x48]
    cmp r0, #3
    bls .L08025768
.L0802566E:
    ldr r2, [sp, #0x50]
    cmp r2, #2
    bne .L08025768
    mov r0, sb
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #2
    bne .L08025768
    mov r0, sb
    mov r1, sl
    mov r2, r8
    adds r3, r6, #0
    bl func_08025294
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08025768
    mov r5, sb
    ldr r0, [r5]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    mov r7, sb
    adds r7, #0x3c
    cmp r4, r5
    beq .L080256C0
.L080256AE:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08025612
    adds r4, #4
    cmp r4, r5
    bne .L080256AE
.L080256C0:
    subs r0, r5, #4
    ldr r0, [r0]
.L080256C4:
    movs r1, #0x69
    bl func_08008B6C
    movs r3, #0
    movs r5, #0
    mov r0, r8
    cmp r0, #1
    beq .L080256FC
    cmp r0, #1
    bgt .L080256DE
    cmp r0, #0
    beq .L080256EA
    b .L08025734
.L080256DE:
    mov r1, r8
    cmp r1, #2
    beq .L0802570E
    cmp r1, #3
    beq .L08025720
    b .L08025734
.L080256EA:
    movs r2, #6
    ldrsh r0, [r6, r2]
    movs r5, #2
    ldrsh r1, [r6, r5]
    subs r0, r0, r1
    adds r3, r0, #0
    adds r3, #0x10
    ldr r0, [sp, #0x58]
    b .L08025732
.L080256FC:
    movs r1, #6
    ldrsh r0, [r6, r1]
    movs r2, #2
    ldrsh r1, [r6, r2]
    subs r0, r0, r1
    adds r0, #0x10
    rsbs r3, r0, #0
    ldr r0, [sp, #0x58]
    b .L08025732
.L0802570E:
    movs r1, #4
    ldrsh r0, [r6, r1]
    add r1, sp, #0x14
    movs r2, #0
    ldrsh r1, [r1, r2]
    subs r0, r0, r1
    adds r0, #0x10
    rsbs r3, r0, #0
    b .L08025730
.L08025720:
    movs r1, #4
    ldrsh r0, [r6, r1]
    add r1, sp, #0x14
    movs r2, #0
    ldrsh r1, [r1, r2]
    subs r0, r0, r1
    adds r3, r0, #0
    adds r3, #0x10
.L08025730:
    ldr r0, [sp, #0x54]
.L08025732:
    adds r5, r0, r3
.L08025734:
    adds r1, r7, #0
    ldrb r0, [r1]
    cmp r0, #0
    bne .L0802574A
    movs r0, #0x2c
    strb r0, [r1]
    mov r1, sb
    adds r1, #0x80
    lsls r0, r3, #0xc
    str r0, [r1]
    b .L08025760
.L0802574A:
    cmp r0, #7
    bne .L080257B2
    movs r0, #0x2d
    strb r0, [r7]
    mov r4, sb
    adds r4, #0x80
    lsls r0, r3, #0x10
    movs r1, #0xc
    bl __divsi3
    str r0, [r4]
.L08025760:
    mov r0, sb
    adds r0, #0x84
    strh r5, [r0]
    b .L080257B2
.L08025768:
    cmp r7, r8
    beq .L080257B2
    add r3, sp, #0x4c
    ldr r1, [sp, #0x48]
    ldr r0, [sp, #0x4c]
    add r2, sp, #0x48
    cmp r1, r0
    bls .L0802577A
    adds r2, r3, #0
.L0802577A:
    ldr r0, [r2]
    cmp r7, #1
    beq .L0802579C
    cmp r7, #1
    bgt .L0802578A
    cmp r7, #0
    beq .L08025794
    b .L080257B2
.L0802578A:
    cmp r7, #2
    beq .L080257A4
    cmp r7, #3
    beq .L080257AC
    b .L080257B2
.L08025794:
    ldr r1, [sp, #0x58]
    adds r1, r1, r0
    str r1, [sp, #0x58]
    b .L080257B2
.L0802579C:
    ldr r2, [sp, #0x58]
    subs r2, r2, r0
    str r2, [sp, #0x58]
    b .L080257B2
.L080257A4:
    ldr r5, [sp, #0x54]
    subs r5, r5, r0
    str r5, [sp, #0x54]
    b .L080257B2
.L080257AC:
    ldr r1, [sp, #0x54]
    adds r1, r1, r0
    str r1, [sp, #0x54]
.L080257B2:
    ldr r2, [sp, #0x54]
    lsls r0, r2, #0x10
    mov r5, sb
    str r0, [r5, #8]
    ldr r1, [sp, #0x58]
    lsls r0, r1, #0x10
    str r0, [r5, #0xc]
.L080257C0:
    add sp, #0x5c
    pop {r3, r4, r5}
    mov r8, r3
    mov sb, r4
    mov sl, r5
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0








    .section .text.entities_part1.08025B64, "ax", %progbits
    thumb_func_start func_08025B64
func_08025B64: @ 0x08025B64
    push {r4, r5, r6, lr}
    sub sp, #0x28
    adds r6, r0, #0
    cmp r1, #0x22
    bhi .L08025C08
    lsls r0, r1, #2
    ldr r1, .L08025B78 @ =.L08025B7C
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L08025B78: .4byte .L08025B7C
.L08025B7C: @ jump table
    .4byte .L08025C08 @ case 0
    .4byte .L08025C08 @ case 1
    .4byte .L08025CB2 @ case 2
    .4byte .L08025C12 @ case 3
    .4byte .L08025C20 @ case 4
    .4byte .L08025C28 @ case 5
    .4byte .L08025C32 @ case 6
    .4byte .L08025C3C @ case 7
    .4byte .L08025C46 @ case 8
    .4byte .L08025C4E @ case 9
    .4byte .L08025C68 @ case 10
    .4byte .L08025CCE @ case 11
    .4byte .L08025CF8 @ case 12
    .4byte .L08025D04 @ case 13
    .4byte .L08025D0E @ case 14
    .4byte .L08025D1A @ case 15
    .4byte .L08025D26 @ case 16
    .4byte .L08025D32 @ case 17
    .4byte .L08025D3C @ case 18
    .4byte .L08025D48 @ case 19
    .4byte .L08025D54 @ case 20
    .4byte .L08025D60 @ case 21
    .4byte .L08025D6A @ case 22
    .4byte .L08025D76 @ case 23
    .4byte .L08025D82 @ case 24
    .4byte .L08025D8E @ case 25
    .4byte .L08025D98 @ case 26
    .4byte .L08025DA4 @ case 27
    .4byte .L08025DB0 @ case 28
    .4byte .L08025DBC @ case 29
    .4byte .L08025DC6 @ case 30
    .4byte .L08025DD2 @ case 31
    .4byte .L08025DDE @ case 32
    .4byte .L08025DEA @ case 33
    .4byte .L08025E0C @ case 34
.L08025C08:
    adds r1, r6, #0
    adds r1, #0x3c
    movs r0, #0
    strb r0, [r1]
    b .L08025E2E
.L08025C12:
    ldr r0, [r6, #0x14]
    adds r0, #0x80
    ldr r1, [r0]
    adds r0, r6, #0
    bl _call_via_r1
    b .L08025E2E
.L08025C20:
    add r4, sp, #8
    adds r0, r4, #0
    movs r1, #5
    b .L08025C56
.L08025C28:
    mov r4, sp
    adds r4, #9
    adds r0, r4, #0
    movs r1, #0xd
    b .L08025C56
.L08025C32:
    mov r4, sp
    adds r4, #0xa
    adds r0, r4, #0
    movs r1, #0x15
    b .L08025C56
.L08025C3C:
    mov r4, sp
    adds r4, #0xb
    adds r0, r4, #0
    movs r1, #0x1d
    b .L08025C56
.L08025C46:
    add r4, sp, #0xc
    adds r0, r4, #0
    movs r1, #0x25
    b .L08025C56
.L08025C4E:
    mov r4, sp
    adds r4, #0xd
    adds r0, r4, #0
    movs r1, #0x2d
.L08025C56:
    bl __4ToolUi
    adds r0, r6, #0
    adds r1, r4, #0
    bl func_0802720C
    b .L08025E2E
.L08025C64:
    ldr r0, [r4]
    b .L08025CAA
.L08025C68:
    mov r4, sp
    adds r4, #0xe
    adds r0, r4, #0
    movs r1, #0x4d
    bl __4ToolUi
    adds r0, r6, #0
    adds r1, r4, #0
    bl func_0802720C
    ldr r0, [r6]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    cmp r4, r5
    beq .L08025CA6
.L08025C94:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08025C64
    adds r4, #4
    cmp r4, r5
    bne .L08025C94
.L08025CA6:
    subs r0, r5, #4
    ldr r0, [r0]
.L08025CAA:
    movs r1, #0xb6
    bl func_08008B6C
    b .L08025E2E
.L08025CB2:
    bl rand
    adds r1, r0, #0
    cmp r0, #0
    bge .L08025CBE
    adds r1, #0xf
.L08025CBE:
    asrs r1, r1, #4
    lsls r1, r1, #4
    subs r1, r0, r1
    adds r1, #5
    adds r0, r6, #0
    bl func_0802771C
    b .L08025E2E
.L08025CCE:
    ldr r5, [r6, #0x14]
    add r4, sp, #4
    adds r0, r4, #0
    movs r1, #0x36
    bl __4FoodUi
    ldrb r1, [r4]
    mov r0, sp
    adds r0, #5
    ldrb r0, [r0]
    lsls r0, r0, #8
    orrs r0, r1
    mov r1, sp
    adds r1, #6
    ldrb r1, [r1]
    lsls r1, r1, #0x10
    orrs r1, r0
    mov r0, sp
    bl __12RucksackItemG4Food
    b .L08025DFE
.L08025CF8:
    ldr r5, [r6, #0x14]
    mov r4, sp
    adds r4, #0xf
    adds r0, r4, #0
    movs r1, #0x11
    b .L08025DF2
.L08025D04:
    ldr r5, [r6, #0x14]
    add r4, sp, #0x10
    adds r0, r4, #0
    movs r1, #0x12
    b .L08025DF2
.L08025D0E:
    ldr r5, [r6, #0x14]
    mov r4, sp
    adds r4, #0x11
    adds r0, r4, #0
    movs r1, #0x13
    b .L08025DF2
.L08025D1A:
    ldr r5, [r6, #0x14]
    mov r4, sp
    adds r4, #0x12
    adds r0, r4, #0
    movs r1, #0x14
    b .L08025DF2
.L08025D26:
    ldr r5, [r6, #0x14]
    mov r4, sp
    adds r4, #0x13
    adds r0, r4, #0
    movs r1, #0x15
    b .L08025DF2
.L08025D32:
    ldr r5, [r6, #0x14]
    add r4, sp, #0x14
    adds r0, r4, #0
    movs r1, #0x16
    b .L08025DF2
.L08025D3C:
    ldr r5, [r6, #0x14]
    mov r4, sp
    adds r4, #0x15
    adds r0, r4, #0
    movs r1, #0x17
    b .L08025DF2
.L08025D48:
    ldr r5, [r6, #0x14]
    mov r4, sp
    adds r4, #0x16
    adds r0, r4, #0
    movs r1, #0x18
    b .L08025DF2
.L08025D54:
    ldr r5, [r6, #0x14]
    mov r4, sp
    adds r4, #0x17
    adds r0, r4, #0
    movs r1, #0x19
    b .L08025DF2
.L08025D60:
    ldr r5, [r6, #0x14]
    add r4, sp, #0x18
    adds r0, r4, #0
    movs r1, #0x1a
    b .L08025DF2
.L08025D6A:
    ldr r5, [r6, #0x14]
    mov r4, sp
    adds r4, #0x19
    adds r0, r4, #0
    movs r1, #0x1b
    b .L08025DF2
.L08025D76:
    ldr r5, [r6, #0x14]
    mov r4, sp
    adds r4, #0x1a
    adds r0, r4, #0
    movs r1, #0x1c
    b .L08025DF2
.L08025D82:
    ldr r5, [r6, #0x14]
    mov r4, sp
    adds r4, #0x1b
    adds r0, r4, #0
    movs r1, #0x1d
    b .L08025DF2
.L08025D8E:
    ldr r5, [r6, #0x14]
    add r4, sp, #0x1c
    adds r0, r4, #0
    movs r1, #0x1e
    b .L08025DF2
.L08025D98:
    ldr r5, [r6, #0x14]
    mov r4, sp
    adds r4, #0x1d
    adds r0, r4, #0
    movs r1, #0x1f
    b .L08025DF2
.L08025DA4:
    ldr r5, [r6, #0x14]
    mov r4, sp
    adds r4, #0x1e
    adds r0, r4, #0
    movs r1, #0x20
    b .L08025DF2
.L08025DB0:
    ldr r5, [r6, #0x14]
    mov r4, sp
    adds r4, #0x1f
    adds r0, r4, #0
    movs r1, #0x21
    b .L08025DF2
.L08025DBC:
    ldr r5, [r6, #0x14]
    add r4, sp, #0x20
    adds r0, r4, #0
    movs r1, #0x22
    b .L08025DF2
.L08025DC6:
    ldr r5, [r6, #0x14]
    mov r4, sp
    adds r4, #0x21
    adds r0, r4, #0
    movs r1, #0x23
    b .L08025DF2
.L08025DD2:
    ldr r5, [r6, #0x14]
    mov r4, sp
    adds r4, #0x22
    adds r0, r4, #0
    movs r1, #0x24
    b .L08025DF2
.L08025DDE:
    ldr r5, [r6, #0x14]
    mov r4, sp
    adds r4, #0x23
    adds r0, r4, #0
    movs r1, #0x25
    b .L08025DF2
.L08025DEA:
    ldr r5, [r6, #0x14]
    add r4, sp, #0x24
    adds r0, r4, #0
    movs r1, #0x26
.L08025DF2:
    bl __7ArticleUi
    ldrb r1, [r4]
    mov r0, sp
    bl __12RucksackItemG7Article
.L08025DFE:
    ldr r3, [r5, #0x5c]
    adds r0, r6, #0
    mov r1, sp
    movs r2, #0
    bl _call_via_r3
    b .L08025E2E
.L08025E0C:
    ldr r5, [r6, #0x14]
    mov r4, sp
    adds r4, #0x25
    adds r0, r4, #0
    movs r1, #0x33
    bl __7ArticleUi
    ldrb r1, [r4]
    mov r0, sp
    bl __12RucksackItemG7Article
    ldr r3, [r5, #0x5c]
    adds r0, r6, #0
    mov r1, sp
    movs r2, #0
    bl _call_via_r3
.L08025E2E:
    add sp, #0x28
    pop {r4, r5, r6}
    pop {r0}
    bx r0
    .align 2, 0



    .section .text.entities_part1.08025FC4.tail, "ax", %progbits
.L08026088:
    .byte 0x30, 0xB5, 0x04, 0x1C, 0x09, 0x04, 0x09, 0x0C
    .byte 0xA0, 0x6B, 0x54, 0x30, 0xE9, 0xF7, 0x3C, 0xFA, 0x20, 0x1C, 0x04, 0xF0, 0x75, 0xFA, 0x21, 0x1C
    .byte 0x3C, 0x31, 0x00, 0x25, 0x11, 0x20, 0x08, 0x70, 0x20, 0x1C, 0x20, 0x30, 0x03, 0x78, 0x01, 0x2B
    .byte 0x1D, 0xD0, 0x01, 0x2B, 0x02, 0xDC, 0x00, 0x2B, 0x24, 0xD0, 0x2C, 0xE0, 0x02, 0x2B, 0x02, 0xD0
    .byte 0x03, 0x2B, 0x0B, 0xD0, 0x27, 0xE0, 0x20, 0x1C, 0x88, 0x30, 0x03, 0x49, 0xC0, 0x22, 0x12, 0x03
    .byte 0x41, 0x60, 0x85, 0x60, 0x1E, 0xE0, 0x00, 0x00, 0x00, 0x00, 0xF6, 0xFF, 0x20, 0x1C, 0x88, 0x30
    .byte 0xA0, 0x21, 0x09, 0x03, 0xC0, 0x22, 0x12, 0x03, 0x41, 0x60, 0x85, 0x60, 0x12, 0xE0, 0x20, 0x1C
    .byte 0x88, 0x30, 0x02, 0x49, 0xC0, 0x22, 0x12, 0x03, 0x45, 0x60, 0x0A, 0xE0, 0x00, 0x00, 0xF6, 0xFF
    .byte 0x20, 0x68, 0x1F, 0xE0, 0x20, 0x1C, 0x88, 0x30, 0xA0, 0x21, 0x09, 0x03, 0xC0, 0x22, 0x12, 0x03
    .byte 0x43, 0x60, 0x81, 0x60, 0x42, 0x61, 0x20, 0x68, 0x01, 0x68, 0xA6, 0x22, 0x52, 0x00, 0x89, 0x18
    .byte 0x09, 0x68, 0xAD, 0xF0, 0xF5, 0xFB, 0x04, 0x68, 0x45, 0x68, 0xAC, 0x42, 0x08, 0xD0, 0x20, 0x68
    .byte 0xE2, 0xF7, 0xCE, 0xFD, 0x00, 0x06, 0x00, 0x28, 0xE2, 0xD0, 0x04, 0x34, 0xAC, 0x42, 0xF6, 0xD1
    .byte 0x28, 0x1F, 0x00, 0x68, 0x6A, 0x21, 0xE2, 0xF7, 0x11, 0xFD, 0x30, 0xBC, 0x01, 0xBC, 0x00, 0x47



    .section .text.entities_part1.0802634C, "ax", %progbits
    thumb_func_start func_0802634C
func_0802634C: @ 0x0802634C
    push {r4, r5, r6, r7, lr}
    sub sp, #8
    adds r6, r0, #0
    ldr r0, [r6, #0x38]
    bl func_0800EAF0
    adds r0, r6, #0
    adds r0, #0x88
    movs r5, #0
    movs r7, #1
    strb r7, [r0]
    strb r5, [r0, #1]
    ldr r1, .L080263CC @ =0xFFF80000
    movs r2, #0x88
    lsls r2, r2, #0xd
    str r1, [r0, #4]
    str r5, [r0, #8]
    str r2, [r0, #0x14]
    ldr r4, [r6, #0x10]
    cmp r4, #0
    beq .L0802639C
    ldr r0, [r4]
    ldr r0, [r0]
    ldr r1, [r0]
    ldr r1, [r1, #0x64]
    bl _call_via_r1
    adds r1, r0, #0
    adds r0, r4, #0
    adds r0, #0xb4
    ldr r2, .L080263D0 @ =0x000001DD
    bl func_0805E850
    adds r0, r4, #0
    adds r0, #0xc8
    strb r7, [r0]
    adds r0, #2
    strb r5, [r0]
    adds r0, #1
    strb r7, [r0]
.L0802639C:
    adds r3, r6, #0
    adds r3, #0x3c
    movs r0, #0x2f
    strb r0, [r3]
    adds r2, r6, #0
    adds r2, #0x3d
    movs r0, #0x78
    strb r0, [r2]
    movs r5, #0xc9
    lsls r5, r5, #1
    adds r1, r6, #0
    adds r1, #0x7c
    ldr r0, .L080263D4 @ =0x0000FFFF
    strh r0, [r1]
    ldrb r0, [r3]
    adds r7, r1, #0
    cmp r0, #0x39
    bls .L080263C2
    b .L080267E4
.L080263C2:
    lsls r0, r0, #2
    ldr r1, .L080263D8 @ =.L080263DC
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L080263CC: .4byte 0xFFF80000
.L080263D0: .4byte 0x000001DD
.L080263D4: .4byte 0x0000FFFF
.L080263D8: .4byte .L080263DC
.L080263DC: @ jump table
    .4byte .L080264C4 @ case 0
    .4byte .L0802659E @ case 1
    .4byte .L08026538 @ case 2
    .4byte .L0802659E @ case 3
    .4byte .L08026588 @ case 4
    .4byte .L0802659E @ case 5
    .4byte .L080266BA @ case 6
    .4byte .L080266C6 @ case 7
    .4byte .L080267DA @ case 8
    .4byte .L080265D6 @ case 9
    .4byte .L080267BA @ case 10
    .4byte .L080265BC @ case 11
    .4byte .L080265BC @ case 12
    .4byte .L080265B6 @ case 13
    .4byte .L080265BC @ case 14
    .4byte .L080265C2 @ case 15
    .4byte .L080265C6 @ case 16
    .4byte .L080267A8 @ case 17
    .4byte .L080267B4 @ case 18
    .4byte .L080265CA @ case 19
    .4byte .L080265D0 @ case 20
    .4byte .L080267BA @ case 21
    .4byte .L080266AE @ case 22
    .4byte .L080266B4 @ case 23
    .4byte .L080266C0 @ case 24
    .4byte .L08026624 @ case 25
    .4byte .L08026624 @ case 26
    .4byte .L080265F6 @ case 27
    .4byte .L08026614 @ case 28
    .4byte .L08026614 @ case 29
    .4byte .L08026654 @ case 30
    .4byte .L0802666E @ case 31
    .4byte .L0802668A @ case 32
    .4byte .L080267A8 @ case 33
    .4byte .L08026710 @ case 34
    .4byte .L0802672C @ case 35
    .4byte .L08026744 @ case 36
    .4byte .L0802674A @ case 37
    .4byte .L08026750 @ case 38
    .4byte .L08026754 @ case 39
    .4byte .L08026758 @ case 40
    .4byte .L0802675E @ case 41
    .4byte .L08026764 @ case 42
    .4byte .L08026764 @ case 43
    .4byte .L0802676A @ case 44
    .4byte .L08026780 @ case 45
    .4byte .L080267A4 @ case 46
    .4byte .L080267BA @ case 47
    .4byte .L080267AE @ case 48
    .4byte .L080267A8 @ case 49
    .4byte .L080267AE @ case 50
    .4byte .L080267B4 @ case 51
    .4byte .L080267BA @ case 52
    .4byte .L080267BE @ case 53
    .4byte .L080267A8 @ case 54
    .4byte .L080267A8 @ case 55
    .4byte .L080267AE @ case 56
    .4byte .L080267AE @ case 57
.L080264C4:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L080264F6
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    bne .L080264DE
    b .L080267D0
.L080264DE:
    cmp r0, #1
    bgt .L080264EA
    cmp r0, #0
    bne .L080264E8
    b .L080267AE
.L080264E8:
    b .L080267E4
.L080264EA:
    cmp r0, #2
    beq .L080264F0
    b .L080267E4
.L080264F0:
    movs r5, #0xa7
    lsls r5, r5, #1
    b .L080267E4
.L080264F6:
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    beq .L08026512
    cmp r0, #1
    bgt .L0802650C
    cmp r0, #0
    bne .L0802650A
    b .L08026624
.L0802650A:
    b .L080267E4
.L0802650C:
    cmp r0, #2
    beq .L08026524
    b .L080267E4
.L08026512:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L08026534
    adds r1, #0x94
    b .L08026534
.L08026524:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L08026534
    adds r1, #0x2c
.L08026534:
    adds r5, r1, #0
    b .L080267E4
.L08026538:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08026548
    b .L080267AE
.L08026548:
    adds r0, r6, #0
    adds r0, #0x3f
    ldrb r3, [r0]
    cmp r3, #5
    bgt .L08026624
    cmp r3, #0
    blt .L08026624
    ldr r2, .L08026584 @ =gUnk_080F0FCC
    adds r1, r6, #0
    adds r1, #0x40
    lsls r0, r3, #3
    subs r0, r0, r3
    ldrb r1, [r1]
    adds r0, r0, r1
    lsls r0, r0, #1
    adds r0, r0, r2
    ldrh r5, [r0]
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    mov r1, sp
    strb r0, [r1]
    mov r0, sp
    b .L080266A6
    .align 2, 0
.L08026584: .4byte gUnk_080F0FCC
.L08026588:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L0802659C
    b .L080267E4
.L0802659C:
    b .L080267AE
.L0802659E:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L080265B2
    b .L080267E4
.L080265B2:
    subs r5, #0x40
    b .L080267E4
.L080265B6:
    movs r5, #0x9b
    lsls r5, r5, #1
    b .L080267E4
.L080265BC:
    movs r5, #0xcf
    lsls r5, r5, #1
    b .L080267E4
.L080265C2:
    movs r5, #0x4e
    b .L080267E4
.L080265C6:
    movs r5, #0x46
    b .L080267E4
.L080265CA:
    movs r5, #0x9f
    lsls r5, r5, #1
    b .L080267E4
.L080265D0:
    movs r5, #0xa1
    lsls r5, r5, #1
    b .L080267E4
.L080265D6:
    adds r0, r6, #0
    adds r0, #0x44
    ldrh r5, [r0]
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #1
    b .L080266A4
.L080265F6:
    movs r5, #0xf5
    lsls r5, r5, #1
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #2
    b .L080266A4
.L08026614:
    adds r2, r6, #0
    adds r2, #0x4e
    adds r1, r6, #0
    adds r1, #0x4f
    ldrb r0, [r2]
    ldrb r3, [r1]
    cmp r0, r3
    bne .L0802662A
.L08026624:
    movs r5, #0xc9
    lsls r5, r5, #1
    b .L080267E4
.L0802662A:
    ldrb r0, [r1]
    ldrb r1, [r2]
    subs r0, r0, r1
    movs r5, #0xc1
    lsls r5, r5, #1
    cmp r0, #1
    bne .L0802663A
    adds r5, #4
.L0802663A:
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #3
    b .L080266A4
.L08026654:
    movs r5, #0x76
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    add r0, sp, #4
    b .L080266A4
.L0802666E:
    movs r5, #0x6e
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #5
    b .L080266A4
.L0802668A:
    movs r5, #0x72
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #6
.L080266A4:
    strb r1, [r0]
.L080266A6:
    bl GetIconId__C4Tool
    strh r0, [r7]
    b .L080267E4
.L080266AE:
    movs r5, #0xfd
    lsls r5, r5, #1
    b .L080267E4
.L080266B4:
    movs r5, #0xcb
    lsls r5, r5, #1
    b .L080267E4
.L080266BA:
    movs r5, #0x89
    lsls r5, r5, #1
    b .L080267E4
.L080266C0:
    movs r5, #0x8b
    lsls r5, r5, #1
    b .L080267E4
.L080266C6:
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    beq .L080266F8
    cmp r0, #1
    bgt .L080266E8
    cmp r0, #0
    beq .L080266EE
    b .L080267E4
.L080266E8:
    cmp r0, #2
    beq .L08026704
    b .L080267E4
.L080266EE:
    movs r5, #0xea
    ldr r1, .L080266F4 @ =0x0000077A
    b .L08026792
    .align 2, 0
.L080266F4: .4byte 0x0000077A
.L080266F8:
    movs r5, #0xee
    ldr r1, .L08026700 @ =0x0000077E
    b .L08026792
    .align 2, 0
.L08026700: .4byte 0x0000077E
.L08026704:
    movs r5, #0xe6
    ldr r1, .L0802670C @ =0x00000782
    b .L08026792
    .align 2, 0
.L0802670C: .4byte 0x00000782
.L08026710:
    movs r5, #0xad
    lsls r5, r5, #1
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08026728 @ =0x0000077A
    b .L08026792
    .align 2, 0
.L08026728: .4byte 0x0000077A
.L0802672C:
    movs r5, #0x9a
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08026740 @ =0x0000077A
    b .L08026792
    .align 2, 0
.L08026740: .4byte 0x0000077A
.L08026744:
    movs r5, #0x83
    lsls r5, r5, #1
    b .L080267E4
.L0802674A:
    movs r5, #0x81
    lsls r5, r5, #1
    b .L080267E4
.L08026750:
    movs r5, #0xfe
    b .L080267E4
.L08026754:
    movs r5, #0xfa
    b .L080267E4
.L08026758:
    movs r5, #0x91
    lsls r5, r5, #1
    b .L080267E4
.L0802675E:
    movs r5, #0x8f
    lsls r5, r5, #1
    b .L080267E4
.L08026764:
    movs r5, #0x8d
    lsls r5, r5, #1
    b .L080267E4
.L0802676A:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xa5
    lsls r5, r5, #1
    cmp r0, #0
    beq .L080267E4
    subs r5, #0x40
    b .L080267E4
.L08026780:
    movs r5, #0xe2
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L080267A0 @ =0x00000792
.L08026792:
    ldrh r0, [r2, #0x22]
    cmp r0, r1
    beq .L080267E4
    adds r0, r2, #0
    bl SetAnim__12AActorEntityUi
    b .L080267E4
    .align 2, 0
.L080267A0: .4byte 0x00000792
.L080267A4:
    movs r5, #0x3e
    b .L080267E4
.L080267A8:
    movs r5, #0xa3
    lsls r5, r5, #1
    b .L080267E4
.L080267AE:
    movs r5, #0xa9
    lsls r5, r5, #1
    b .L080267E4
.L080267B4:
    movs r5, #0x9d
    lsls r5, r5, #1
    b .L080267E4
.L080267BA:
    movs r5, #0x42
    b .L080267E4
.L080267BE:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xe3
    lsls r5, r5, #1
    cmp r0, #0
    bne .L080267E4
.L080267D0:
    movs r5, #0xab
    lsls r5, r5, #1
    b .L080267E4
.L080267D6:
    ldr r0, [r4]
    b .L0802681E
.L080267DA:
    ldrb r0, [r2]
    movs r5, #0xf6
    cmp r0, #0
    beq .L080267E4
    movs r5, #0xf2
.L080267E4:
    lsls r1, r5, #0x10
    lsrs r1, r1, #0x10
    adds r0, r6, #0
    bl SetAnim__12AActorEntityUi
    ldr r0, [r6]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    adds r6, #0xc0
    cmp r4, r5
    beq .L0802681A
.L08026808:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L080267D6
    adds r4, #4
    cmp r4, r5
    bne .L08026808
.L0802681A:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802681E:
    movs r1, #0xb6
    bl func_08008B6C
    movs r0, #1
    strb r0, [r6]
    add sp, #8
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0

    .section .text.entities_part1.08026830, "ax", %progbits
    thumb_func_start func_08026830
func_08026830: @ 0x08026830
    push {r4, r5, r6, r7, lr}
    sub sp, #8
    adds r6, r0, #0
    ldr r0, [r6, #0x38]
    bl func_0800EAFC
    adds r0, r6, #0
    adds r0, #0x88
    movs r5, #0
    movs r7, #1
    strb r7, [r0]
    strb r5, [r0, #1]
    ldr r1, .L080268B0 @ =0xFFF80000
    movs r2, #0x88
    lsls r2, r2, #0xd
    str r1, [r0, #4]
    str r5, [r0, #8]
    str r2, [r0, #0x14]
    ldr r4, [r6, #0x10]
    cmp r4, #0
    beq .L08026880
    ldr r0, [r4]
    ldr r0, [r0]
    ldr r1, [r0]
    ldr r1, [r1, #0x64]
    bl _call_via_r1
    adds r1, r0, #0
    adds r0, r4, #0
    adds r0, #0xb4
    ldr r2, .L080268B4 @ =0x0000015F
    bl func_0805E850
    adds r0, r4, #0
    adds r0, #0xc8
    strb r7, [r0]
    adds r0, #2
    strb r5, [r0]
    adds r0, #1
    strb r7, [r0]
.L08026880:
    adds r3, r6, #0
    adds r3, #0x3c
    movs r0, #0x2f
    strb r0, [r3]
    adds r2, r6, #0
    adds r2, #0x3d
    movs r0, #0x78
    strb r0, [r2]
    movs r5, #0xc9
    lsls r5, r5, #1
    adds r1, r6, #0
    adds r1, #0x7c
    ldr r0, .L080268B8 @ =0x0000FFFF
    strh r0, [r1]
    ldrb r0, [r3]
    adds r7, r1, #0
    cmp r0, #0x39
    bls .L080268A6
    b .L08026CC8
.L080268A6:
    lsls r0, r0, #2
    ldr r1, .L080268BC @ =.L080268C0
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L080268B0: .4byte 0xFFF80000
.L080268B4: .4byte 0x0000015F
.L080268B8: .4byte 0x0000FFFF
.L080268BC: .4byte .L080268C0
.L080268C0: @ jump table
    .4byte .L080269A8 @ case 0
    .4byte .L08026A82 @ case 1
    .4byte .L08026A1C @ case 2
    .4byte .L08026A82 @ case 3
    .4byte .L08026A6C @ case 4
    .4byte .L08026A82 @ case 5
    .4byte .L08026B9E @ case 6
    .4byte .L08026BAA @ case 7
    .4byte .L08026CBE @ case 8
    .4byte .L08026ABA @ case 9
    .4byte .L08026C9E @ case 10
    .4byte .L08026AA0 @ case 11
    .4byte .L08026AA0 @ case 12
    .4byte .L08026A9A @ case 13
    .4byte .L08026AA0 @ case 14
    .4byte .L08026AA6 @ case 15
    .4byte .L08026AAA @ case 16
    .4byte .L08026C8C @ case 17
    .4byte .L08026C98 @ case 18
    .4byte .L08026AAE @ case 19
    .4byte .L08026AB4 @ case 20
    .4byte .L08026C9E @ case 21
    .4byte .L08026B92 @ case 22
    .4byte .L08026B98 @ case 23
    .4byte .L08026BA4 @ case 24
    .4byte .L08026B08 @ case 25
    .4byte .L08026B08 @ case 26
    .4byte .L08026ADA @ case 27
    .4byte .L08026AF8 @ case 28
    .4byte .L08026AF8 @ case 29
    .4byte .L08026B38 @ case 30
    .4byte .L08026B52 @ case 31
    .4byte .L08026B6E @ case 32
    .4byte .L08026C8C @ case 33
    .4byte .L08026BF4 @ case 34
    .4byte .L08026C10 @ case 35
    .4byte .L08026C28 @ case 36
    .4byte .L08026C2E @ case 37
    .4byte .L08026C34 @ case 38
    .4byte .L08026C38 @ case 39
    .4byte .L08026C3C @ case 40
    .4byte .L08026C42 @ case 41
    .4byte .L08026C48 @ case 42
    .4byte .L08026C48 @ case 43
    .4byte .L08026C4E @ case 44
    .4byte .L08026C64 @ case 45
    .4byte .L08026C88 @ case 46
    .4byte .L08026C9E @ case 47
    .4byte .L08026C92 @ case 48
    .4byte .L08026C8C @ case 49
    .4byte .L08026C92 @ case 50
    .4byte .L08026C98 @ case 51
    .4byte .L08026C9E @ case 52
    .4byte .L08026CA2 @ case 53
    .4byte .L08026C8C @ case 54
    .4byte .L08026C8C @ case 55
    .4byte .L08026C92 @ case 56
    .4byte .L08026C92 @ case 57
.L080269A8:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L080269DA
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    bne .L080269C2
    b .L08026CB4
.L080269C2:
    cmp r0, #1
    bgt .L080269CE
    cmp r0, #0
    bne .L080269CC
    b .L08026C92
.L080269CC:
    b .L08026CC8
.L080269CE:
    cmp r0, #2
    beq .L080269D4
    b .L08026CC8
.L080269D4:
    movs r5, #0xa7
    lsls r5, r5, #1
    b .L08026CC8
.L080269DA:
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    beq .L080269F6
    cmp r0, #1
    bgt .L080269F0
    cmp r0, #0
    bne .L080269EE
    b .L08026B08
.L080269EE:
    b .L08026CC8
.L080269F0:
    cmp r0, #2
    beq .L08026A08
    b .L08026CC8
.L080269F6:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L08026A18
    adds r1, #0x94
    b .L08026A18
.L08026A08:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L08026A18
    adds r1, #0x2c
.L08026A18:
    adds r5, r1, #0
    b .L08026CC8
.L08026A1C:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08026A2C
    b .L08026C92
.L08026A2C:
    adds r0, r6, #0
    adds r0, #0x3f
    ldrb r3, [r0]
    cmp r3, #5
    bgt .L08026B08
    cmp r3, #0
    blt .L08026B08
    ldr r2, .L08026A68 @ =gUnk_080F0FCC
    adds r1, r6, #0
    adds r1, #0x40
    lsls r0, r3, #3
    subs r0, r0, r3
    ldrb r1, [r1]
    adds r0, r0, r1
    lsls r0, r0, #1
    adds r0, r0, r2
    ldrh r5, [r0]
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    mov r1, sp
    strb r0, [r1]
    mov r0, sp
    b .L08026B8A
    .align 2, 0
.L08026A68: .4byte gUnk_080F0FCC
.L08026A6C:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L08026A80
    b .L08026CC8
.L08026A80:
    b .L08026C92
.L08026A82:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L08026A96
    b .L08026CC8
.L08026A96:
    subs r5, #0x40
    b .L08026CC8
.L08026A9A:
    movs r5, #0x9b
    lsls r5, r5, #1
    b .L08026CC8
.L08026AA0:
    movs r5, #0xcf
    lsls r5, r5, #1
    b .L08026CC8
.L08026AA6:
    movs r5, #0x4e
    b .L08026CC8
.L08026AAA:
    movs r5, #0x46
    b .L08026CC8
.L08026AAE:
    movs r5, #0x9f
    lsls r5, r5, #1
    b .L08026CC8
.L08026AB4:
    movs r5, #0xa1
    lsls r5, r5, #1
    b .L08026CC8
.L08026ABA:
    adds r0, r6, #0
    adds r0, #0x44
    ldrh r5, [r0]
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #1
    b .L08026B88
.L08026ADA:
    movs r5, #0xf5
    lsls r5, r5, #1
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #2
    b .L08026B88
.L08026AF8:
    adds r2, r6, #0
    adds r2, #0x4e
    adds r1, r6, #0
    adds r1, #0x4f
    ldrb r0, [r2]
    ldrb r3, [r1]
    cmp r0, r3
    bne .L08026B0E
.L08026B08:
    movs r5, #0xc9
    lsls r5, r5, #1
    b .L08026CC8
.L08026B0E:
    ldrb r0, [r1]
    ldrb r1, [r2]
    subs r0, r0, r1
    movs r5, #0xc1
    lsls r5, r5, #1
    cmp r0, #1
    bne .L08026B1E
    adds r5, #4
.L08026B1E:
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #3
    b .L08026B88
.L08026B38:
    movs r5, #0x76
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    add r0, sp, #4
    b .L08026B88
.L08026B52:
    movs r5, #0x6e
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #5
    b .L08026B88
.L08026B6E:
    movs r5, #0x72
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #6
.L08026B88:
    strb r1, [r0]
.L08026B8A:
    bl GetIconId__C4Tool
    strh r0, [r7]
    b .L08026CC8
.L08026B92:
    movs r5, #0xfd
    lsls r5, r5, #1
    b .L08026CC8
.L08026B98:
    movs r5, #0xcb
    lsls r5, r5, #1
    b .L08026CC8
.L08026B9E:
    movs r5, #0x89
    lsls r5, r5, #1
    b .L08026CC8
.L08026BA4:
    movs r5, #0x8b
    lsls r5, r5, #1
    b .L08026CC8
.L08026BAA:
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    beq .L08026BDC
    cmp r0, #1
    bgt .L08026BCC
    cmp r0, #0
    beq .L08026BD2
    b .L08026CC8
.L08026BCC:
    cmp r0, #2
    beq .L08026BE8
    b .L08026CC8
.L08026BD2:
    movs r5, #0xea
    ldr r1, .L08026BD8 @ =0x0000077A
    b .L08026C76
    .align 2, 0
.L08026BD8: .4byte 0x0000077A
.L08026BDC:
    movs r5, #0xee
    ldr r1, .L08026BE4 @ =0x0000077E
    b .L08026C76
    .align 2, 0
.L08026BE4: .4byte 0x0000077E
.L08026BE8:
    movs r5, #0xe6
    ldr r1, .L08026BF0 @ =0x00000782
    b .L08026C76
    .align 2, 0
.L08026BF0: .4byte 0x00000782
.L08026BF4:
    movs r5, #0xad
    lsls r5, r5, #1
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08026C0C @ =0x0000077A
    b .L08026C76
    .align 2, 0
.L08026C0C: .4byte 0x0000077A
.L08026C10:
    movs r5, #0x9a
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08026C24 @ =0x0000077A
    b .L08026C76
    .align 2, 0
.L08026C24: .4byte 0x0000077A
.L08026C28:
    movs r5, #0x83
    lsls r5, r5, #1
    b .L08026CC8
.L08026C2E:
    movs r5, #0x81
    lsls r5, r5, #1
    b .L08026CC8
.L08026C34:
    movs r5, #0xfe
    b .L08026CC8
.L08026C38:
    movs r5, #0xfa
    b .L08026CC8
.L08026C3C:
    movs r5, #0x91
    lsls r5, r5, #1
    b .L08026CC8
.L08026C42:
    movs r5, #0x8f
    lsls r5, r5, #1
    b .L08026CC8
.L08026C48:
    movs r5, #0x8d
    lsls r5, r5, #1
    b .L08026CC8
.L08026C4E:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xa5
    lsls r5, r5, #1
    cmp r0, #0
    beq .L08026CC8
    subs r5, #0x40
    b .L08026CC8
.L08026C64:
    movs r5, #0xe2
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08026C84 @ =0x00000792
.L08026C76:
    ldrh r0, [r2, #0x22]
    cmp r0, r1
    beq .L08026CC8
    adds r0, r2, #0
    bl SetAnim__12AActorEntityUi
    b .L08026CC8
    .align 2, 0
.L08026C84: .4byte 0x00000792
.L08026C88:
    movs r5, #0x3e
    b .L08026CC8
.L08026C8C:
    movs r5, #0xa3
    lsls r5, r5, #1
    b .L08026CC8
.L08026C92:
    movs r5, #0xa9
    lsls r5, r5, #1
    b .L08026CC8
.L08026C98:
    movs r5, #0x9d
    lsls r5, r5, #1
    b .L08026CC8
.L08026C9E:
    movs r5, #0x42
    b .L08026CC8
.L08026CA2:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xe3
    lsls r5, r5, #1
    cmp r0, #0
    bne .L08026CC8
.L08026CB4:
    movs r5, #0xab
    lsls r5, r5, #1
    b .L08026CC8
.L08026CBA:
    ldr r0, [r4]
    b .L08026D02
.L08026CBE:
    ldrb r0, [r2]
    movs r5, #0xf6
    cmp r0, #0
    beq .L08026CC8
    movs r5, #0xf2
.L08026CC8:
    lsls r1, r5, #0x10
    lsrs r1, r1, #0x10
    adds r0, r6, #0
    bl SetAnim__12AActorEntityUi
    ldr r0, [r6]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    adds r6, #0xc0
    cmp r4, r5
    beq .L08026CFE
.L08026CEC:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08026CBA
    adds r4, #4
    cmp r4, r5
    bne .L08026CEC
.L08026CFE:
    subs r0, r5, #4
    ldr r0, [r0]
.L08026D02:
    movs r1, #0xb6
    bl func_08008B6C
    movs r0, #1
    strb r0, [r6]
    add sp, #8
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0

    .section .text.entities_part1.08026D14, "ax", %progbits
    thumb_func_start func_08026D14
func_08026D14: @ 0x08026D14
    push {r4, r5, r6, r7, lr}
    sub sp, #8
    adds r6, r0, #0
    lsls r1, r1, #0x10
    lsrs r5, r1, #0x10
    adds r1, r6, #0
    adds r1, #0x3c
    movs r4, #0
    movs r0, #0x11
    strb r0, [r1]
    adds r2, r6, #0
    adds r2, #0x88
    movs r0, #1
    strb r0, [r2]
    strb r4, [r2, #1]
    adds r0, r6, #0
    adds r0, #0x20
    ldrb r3, [r0]
    cmp r3, #1
    beq .L08026D6E
    cmp r3, #1
    bgt .L08026D46
    cmp r3, #0
    beq .L08026D7C
    b .L08026D8A
.L08026D46:
    cmp r3, #2
    beq .L08026D50
    cmp r3, #3
    beq .L08026D60
    b .L08026D8A
.L08026D50:
    ldr r0, .L08026D5C @ =0xFFF60000
    movs r1, #0xc0
    lsls r1, r1, #0xc
    str r0, [r2, #4]
    str r4, [r2, #8]
    b .L08026D88
    .align 2, 0
.L08026D5C: .4byte 0xFFF60000
.L08026D60:
    movs r0, #0xa0
    lsls r0, r0, #0xc
    movs r1, #0xc0
    lsls r1, r1, #0xc
    str r0, [r2, #4]
    str r4, [r2, #8]
    b .L08026D88
.L08026D6E:
    ldr r0, .L08026D78 @ =0xFFF60000
    movs r1, #0xc0
    lsls r1, r1, #0xc
    str r4, [r2, #4]
    b .L08026D86
    .align 2, 0
.L08026D78: .4byte 0xFFF60000
.L08026D7C:
    movs r0, #0xa0
    lsls r0, r0, #0xc
    movs r1, #0xc0
    lsls r1, r1, #0xc
    str r3, [r2, #4]
.L08026D86:
    str r0, [r2, #8]
.L08026D88:
    str r1, [r2, #0x14]
.L08026D8A:
    ldr r4, [r6, #0x10]
    cmp r4, #0
    beq .L08026DBA
    ldr r0, [r4]
    ldr r0, [r0]
    ldr r1, [r0]
    ldr r1, [r1, #0x68]
    bl _call_via_r1
    adds r1, r0, #0
    adds r0, r4, #0
    adds r0, #0xb4
    adds r2, r5, #0
    bl func_0805E850
    adds r0, r4, #0
    adds r0, #0xc8
    movs r2, #0
    movs r1, #1
    strb r1, [r0]
    adds r0, #2
    strb r2, [r0]
    adds r0, #1
    strb r1, [r0]
.L08026DBA:
    movs r5, #0xc9
    lsls r5, r5, #1
    adds r0, r6, #0
    adds r0, #0x7c
    ldr r1, .L08026DE0 @ =0x0000FFFF
    strh r1, [r0]
    adds r1, r6, #0
    adds r1, #0x3c
    ldrb r1, [r1]
    adds r7, r0, #0
    cmp r1, #0x39
    bls .L08026DD4
    b .L080271F0
.L08026DD4:
    lsls r0, r1, #2
    ldr r1, .L08026DE4 @ =.L08026DE8
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L08026DE0: .4byte 0x0000FFFF
.L08026DE4: .4byte .L08026DE8
.L08026DE8: @ jump table
    .4byte .L08026ED0 @ case 0
    .4byte .L08026FAA @ case 1
    .4byte .L08026F44 @ case 2
    .4byte .L08026FAA @ case 3
    .4byte .L08026F94 @ case 4
    .4byte .L08026FAA @ case 5
    .4byte .L080270C6 @ case 6
    .4byte .L080270D2 @ case 7
    .4byte .L080271E2 @ case 8
    .4byte .L08026FE2 @ case 9
    .4byte .L080271C6 @ case 10
    .4byte .L08026FC8 @ case 11
    .4byte .L08026FC8 @ case 12
    .4byte .L08026FC2 @ case 13
    .4byte .L08026FC8 @ case 14
    .4byte .L08026FCE @ case 15
    .4byte .L08026FD2 @ case 16
    .4byte .L080271B4 @ case 17
    .4byte .L080271C0 @ case 18
    .4byte .L08026FD6 @ case 19
    .4byte .L08026FDC @ case 20
    .4byte .L080271C6 @ case 21
    .4byte .L080270BA @ case 22
    .4byte .L080270C0 @ case 23
    .4byte .L080270CC @ case 24
    .4byte .L08027030 @ case 25
    .4byte .L08027030 @ case 26
    .4byte .L08027002 @ case 27
    .4byte .L08027020 @ case 28
    .4byte .L08027020 @ case 29
    .4byte .L08027060 @ case 30
    .4byte .L0802707A @ case 31
    .4byte .L08027096 @ case 32
    .4byte .L080271B4 @ case 33
    .4byte .L0802711C @ case 34
    .4byte .L08027138 @ case 35
    .4byte .L08027150 @ case 36
    .4byte .L08027156 @ case 37
    .4byte .L0802715C @ case 38
    .4byte .L08027160 @ case 39
    .4byte .L08027164 @ case 40
    .4byte .L0802716A @ case 41
    .4byte .L08027170 @ case 42
    .4byte .L08027170 @ case 43
    .4byte .L08027176 @ case 44
    .4byte .L0802718C @ case 45
    .4byte .L080271B0 @ case 46
    .4byte .L080271C6 @ case 47
    .4byte .L080271BA @ case 48
    .4byte .L080271B4 @ case 49
    .4byte .L080271BA @ case 50
    .4byte .L080271C0 @ case 51
    .4byte .L080271C6 @ case 52
    .4byte .L080271CA @ case 53
    .4byte .L080271B4 @ case 54
    .4byte .L080271B4 @ case 55
    .4byte .L080271BA @ case 56
    .4byte .L080271BA @ case 57
.L08026ED0:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08026F02
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    bne .L08026EEA
    b .L080271DC
.L08026EEA:
    cmp r0, #1
    bgt .L08026EF6
    cmp r0, #0
    bne .L08026EF4
    b .L080271BA
.L08026EF4:
    b .L080271F0
.L08026EF6:
    cmp r0, #2
    beq .L08026EFC
    b .L080271F0
.L08026EFC:
    movs r5, #0xa7
    lsls r5, r5, #1
    b .L080271F0
.L08026F02:
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    beq .L08026F1E
    cmp r0, #1
    bgt .L08026F18
    cmp r0, #0
    bne .L08026F16
    b .L08027030
.L08026F16:
    b .L080271F0
.L08026F18:
    cmp r0, #2
    beq .L08026F30
    b .L080271F0
.L08026F1E:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L08026F40
    adds r1, #0x94
    b .L08026F40
.L08026F30:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L08026F40
    adds r1, #0x2c
.L08026F40:
    adds r5, r1, #0
    b .L080271F0
.L08026F44:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08026F54
    b .L080271BA
.L08026F54:
    adds r0, r6, #0
    adds r0, #0x3f
    ldrb r3, [r0]
    cmp r3, #5
    bgt .L08027030
    cmp r3, #0
    blt .L08027030
    ldr r2, .L08026F90 @ =gUnk_080F0FCC
    adds r1, r6, #0
    adds r1, #0x40
    lsls r0, r3, #3
    subs r0, r0, r3
    ldrb r1, [r1]
    adds r0, r0, r1
    lsls r0, r0, #1
    adds r0, r0, r2
    ldrh r5, [r0]
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    mov r1, sp
    strb r0, [r1]
    mov r0, sp
    b .L080270B2
    .align 2, 0
.L08026F90: .4byte gUnk_080F0FCC
.L08026F94:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L08026FA8
    b .L080271F0
.L08026FA8:
    b .L080271BA
.L08026FAA:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L08026FBE
    b .L080271F0
.L08026FBE:
    subs r5, #0x40
    b .L080271F0
.L08026FC2:
    movs r5, #0x9b
    lsls r5, r5, #1
    b .L080271F0
.L08026FC8:
    movs r5, #0xcf
    lsls r5, r5, #1
    b .L080271F0
.L08026FCE:
    movs r5, #0x4e
    b .L080271F0
.L08026FD2:
    movs r5, #0x46
    b .L080271F0
.L08026FD6:
    movs r5, #0x9f
    lsls r5, r5, #1
    b .L080271F0
.L08026FDC:
    movs r5, #0xa1
    lsls r5, r5, #1
    b .L080271F0
.L08026FE2:
    adds r0, r6, #0
    adds r0, #0x44
    ldrh r5, [r0]
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #1
    b .L080270B0
.L08027002:
    movs r5, #0xf5
    lsls r5, r5, #1
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #2
    b .L080270B0
.L08027020:
    adds r2, r6, #0
    adds r2, #0x4e
    adds r1, r6, #0
    adds r1, #0x4f
    ldrb r0, [r2]
    ldrb r3, [r1]
    cmp r0, r3
    bne .L08027036
.L08027030:
    movs r5, #0xc9
    lsls r5, r5, #1
    b .L080271F0
.L08027036:
    ldrb r0, [r1]
    ldrb r1, [r2]
    subs r0, r0, r1
    movs r5, #0xc1
    lsls r5, r5, #1
    cmp r0, #1
    bne .L08027046
    adds r5, #4
.L08027046:
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #3
    b .L080270B0
.L08027060:
    movs r5, #0x76
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    add r0, sp, #4
    b .L080270B0
.L0802707A:
    movs r5, #0x6e
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #5
    b .L080270B0
.L08027096:
    movs r5, #0x72
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #6
.L080270B0:
    strb r1, [r0]
.L080270B2:
    bl GetIconId__C4Tool
    strh r0, [r7]
    b .L080271F0
.L080270BA:
    movs r5, #0xfd
    lsls r5, r5, #1
    b .L080271F0
.L080270C0:
    movs r5, #0xcb
    lsls r5, r5, #1
    b .L080271F0
.L080270C6:
    movs r5, #0x89
    lsls r5, r5, #1
    b .L080271F0
.L080270CC:
    movs r5, #0x8b
    lsls r5, r5, #1
    b .L080271F0
.L080270D2:
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    beq .L08027104
    cmp r0, #1
    bgt .L080270F4
    cmp r0, #0
    beq .L080270FA
    b .L080271F0
.L080270F4:
    cmp r0, #2
    beq .L08027110
    b .L080271F0
.L080270FA:
    movs r5, #0xea
    ldr r1, .L08027100 @ =0x0000077A
    b .L0802719E
    .align 2, 0
.L08027100: .4byte 0x0000077A
.L08027104:
    movs r5, #0xee
    ldr r1, .L0802710C @ =0x0000077E
    b .L0802719E
    .align 2, 0
.L0802710C: .4byte 0x0000077E
.L08027110:
    movs r5, #0xe6
    ldr r1, .L08027118 @ =0x00000782
    b .L0802719E
    .align 2, 0
.L08027118: .4byte 0x00000782
.L0802711C:
    movs r5, #0xad
    lsls r5, r5, #1
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08027134 @ =0x0000077A
    b .L0802719E
    .align 2, 0
.L08027134: .4byte 0x0000077A
.L08027138:
    movs r5, #0x9a
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L0802714C @ =0x0000077A
    b .L0802719E
    .align 2, 0
.L0802714C: .4byte 0x0000077A
.L08027150:
    movs r5, #0x83
    lsls r5, r5, #1
    b .L080271F0
.L08027156:
    movs r5, #0x81
    lsls r5, r5, #1
    b .L080271F0
.L0802715C:
    movs r5, #0xfe
    b .L080271F0
.L08027160:
    movs r5, #0xfa
    b .L080271F0
.L08027164:
    movs r5, #0x91
    lsls r5, r5, #1
    b .L080271F0
.L0802716A:
    movs r5, #0x8f
    lsls r5, r5, #1
    b .L080271F0
.L08027170:
    movs r5, #0x8d
    lsls r5, r5, #1
    b .L080271F0
.L08027176:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xa5
    lsls r5, r5, #1
    cmp r0, #0
    beq .L080271F0
    subs r5, #0x40
    b .L080271F0
.L0802718C:
    movs r5, #0xe2
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L080271AC @ =0x00000792
.L0802719E:
    ldrh r0, [r2, #0x22]
    cmp r0, r1
    beq .L080271F0
    adds r0, r2, #0
    bl SetAnim__12AActorEntityUi
    b .L080271F0
    .align 2, 0
.L080271AC: .4byte 0x00000792
.L080271B0:
    movs r5, #0x3e
    b .L080271F0
.L080271B4:
    movs r5, #0xa3
    lsls r5, r5, #1
    b .L080271F0
.L080271BA:
    movs r5, #0xa9
    lsls r5, r5, #1
    b .L080271F0
.L080271C0:
    movs r5, #0x9d
    lsls r5, r5, #1
    b .L080271F0
.L080271C6:
    movs r5, #0x42
    b .L080271F0
.L080271CA:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xe3
    lsls r5, r5, #1
    cmp r0, #0
    bne .L080271F0
.L080271DC:
    movs r5, #0xab
    lsls r5, r5, #1
    b .L080271F0
.L080271E2:
    adds r0, r6, #0
    adds r0, #0x3d
    ldrb r0, [r0]
    movs r5, #0xf6
    cmp r0, #0
    beq .L080271F0
    movs r5, #0xf2
.L080271F0:
    lsls r1, r5, #0x10
    lsrs r1, r1, #0x10
    adds r0, r6, #0
    bl SetAnim__12AActorEntityUi
    adds r1, r6, #0
    adds r1, #0xc0
    movs r0, #1
    strb r0, [r1]
    add sp, #8
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0
    .align 2, 0

    .section .text.entities_part1.0802720C, "ax", %progbits
    thumb_func_start func_0802720C
func_0802720C: @ 0x0802720C
    push {r4, r5, r6, r7, lr}
    mov r7, sb
    mov r6, r8
    push {r6, r7}
    sub sp, #8
    adds r6, r0, #0
    mov sb, r1
    adds r0, #0x88
    movs r7, #0
    movs r1, #1
    mov r8, r1
    mov r2, r8
    strb r2, [r0]
    strb r7, [r0, #1]
    ldr r1, .L080272B4 @ =0xFFF80000
    movs r2, #0x88
    lsls r2, r2, #0xd
    str r1, [r0, #4]
    str r7, [r0, #8]
    str r2, [r0, #0x14]
    ldr r5, [r6, #0x10]
    cmp r5, #0
    beq .L0802726E
    mov r0, sb
    bl GetIconId__C4Tool
    adds r4, r0, #0
    lsls r4, r4, #0x10
    lsrs r4, r4, #0x10
    ldr r0, [r5]
    ldr r0, [r0]
    ldr r1, [r0]
    ldr r1, [r1, #0x64]
    bl _call_via_r1
    adds r1, r0, #0
    adds r0, r5, #0
    adds r0, #0xb4
    adds r2, r4, #0
    bl func_0805E850
    adds r0, r5, #0
    adds r0, #0xc8
    mov r3, r8
    strb r3, [r0]
    adds r0, #2
    strb r7, [r0]
    adds r0, #1
    strb r3, [r0]
.L0802726E:
    ldr r4, [r6, #0x38]
    adds r4, #0x60
    mov r0, sb
    bl GetId__C4Tool
    adds r1, r0, #0
    adds r0, r4, #0
    movs r2, #1
    bl AddAmountOfTool__8RucksackUiUi
    adds r3, r6, #0
    adds r3, #0x3c
    movs r0, #0x34
    strb r0, [r3]
    adds r2, r6, #0
    adds r2, #0x3d
    movs r0, #0x78
    strb r0, [r2]
    movs r5, #0xc9
    lsls r5, r5, #1
    adds r1, r6, #0
    adds r1, #0x7c
    ldr r0, .L080272B8 @ =0x0000FFFF
    strh r0, [r1]
    ldrb r0, [r3]
    adds r7, r1, #0
    cmp r0, #0x39
    bls .L080272A8
    b .L080276C8
.L080272A8:
    lsls r0, r0, #2
    ldr r1, .L080272BC @ =.L080272C0
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L080272B4: .4byte 0xFFF80000
.L080272B8: .4byte 0x0000FFFF
.L080272BC: .4byte .L080272C0
.L080272C0: @ jump table
    .4byte .L080273A8 @ case 0
    .4byte .L08027482 @ case 1
    .4byte .L0802741C @ case 2
    .4byte .L08027482 @ case 3
    .4byte .L0802746C @ case 4
    .4byte .L08027482 @ case 5
    .4byte .L0802759E @ case 6
    .4byte .L080275AA @ case 7
    .4byte .L080276BE @ case 8
    .4byte .L080274BA @ case 9
    .4byte .L0802769E @ case 10
    .4byte .L080274A0 @ case 11
    .4byte .L080274A0 @ case 12
    .4byte .L0802749A @ case 13
    .4byte .L080274A0 @ case 14
    .4byte .L080274A6 @ case 15
    .4byte .L080274AA @ case 16
    .4byte .L0802768C @ case 17
    .4byte .L08027698 @ case 18
    .4byte .L080274AE @ case 19
    .4byte .L080274B4 @ case 20
    .4byte .L0802769E @ case 21
    .4byte .L08027592 @ case 22
    .4byte .L08027598 @ case 23
    .4byte .L080275A4 @ case 24
    .4byte .L08027508 @ case 25
    .4byte .L08027508 @ case 26
    .4byte .L080274DA @ case 27
    .4byte .L080274F8 @ case 28
    .4byte .L080274F8 @ case 29
    .4byte .L08027538 @ case 30
    .4byte .L08027552 @ case 31
    .4byte .L0802756E @ case 32
    .4byte .L0802768C @ case 33
    .4byte .L080275F4 @ case 34
    .4byte .L08027610 @ case 35
    .4byte .L08027628 @ case 36
    .4byte .L0802762E @ case 37
    .4byte .L08027634 @ case 38
    .4byte .L08027638 @ case 39
    .4byte .L0802763C @ case 40
    .4byte .L08027642 @ case 41
    .4byte .L08027648 @ case 42
    .4byte .L08027648 @ case 43
    .4byte .L0802764E @ case 44
    .4byte .L08027664 @ case 45
    .4byte .L08027688 @ case 46
    .4byte .L0802769E @ case 47
    .4byte .L08027692 @ case 48
    .4byte .L0802768C @ case 49
    .4byte .L08027692 @ case 50
    .4byte .L08027698 @ case 51
    .4byte .L0802769E @ case 52
    .4byte .L080276A2 @ case 53
    .4byte .L0802768C @ case 54
    .4byte .L0802768C @ case 55
    .4byte .L08027692 @ case 56
    .4byte .L08027692 @ case 57
.L080273A8:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L080273DA
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    bne .L080273C2
    b .L080276B4
.L080273C2:
    cmp r0, #1
    bgt .L080273CE
    cmp r0, #0
    bne .L080273CC
    b .L08027692
.L080273CC:
    b .L080276C8
.L080273CE:
    cmp r0, #2
    beq .L080273D4
    b .L080276C8
.L080273D4:
    movs r5, #0xa7
    lsls r5, r5, #1
    b .L080276C8
.L080273DA:
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    beq .L080273F6
    cmp r0, #1
    bgt .L080273F0
    cmp r0, #0
    bne .L080273EE
    b .L08027508
.L080273EE:
    b .L080276C8
.L080273F0:
    cmp r0, #2
    beq .L08027408
    b .L080276C8
.L080273F6:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L08027418
    adds r1, #0x94
    b .L08027418
.L08027408:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L08027418
    adds r1, #0x2c
.L08027418:
    adds r5, r1, #0
    b .L080276C8
.L0802741C:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802742C
    b .L08027692
.L0802742C:
    adds r0, r6, #0
    adds r0, #0x3f
    ldrb r3, [r0]
    cmp r3, #5
    bgt .L08027508
    cmp r3, #0
    blt .L08027508
    ldr r2, .L08027468 @ =gUnk_080F0FCC
    adds r1, r6, #0
    adds r1, #0x40
    lsls r0, r3, #3
    subs r0, r0, r3
    ldrb r1, [r1]
    adds r0, r0, r1
    lsls r0, r0, #1
    adds r0, r0, r2
    ldrh r5, [r0]
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    mov r1, sp
    strb r0, [r1]
    mov r0, sp
    b .L0802758A
    .align 2, 0
.L08027468: .4byte gUnk_080F0FCC
.L0802746C:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L08027480
    b .L080276C8
.L08027480:
    b .L08027692
.L08027482:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L08027496
    b .L080276C8
.L08027496:
    subs r5, #0x40
    b .L080276C8
.L0802749A:
    movs r5, #0x9b
    lsls r5, r5, #1
    b .L080276C8
.L080274A0:
    movs r5, #0xcf
    lsls r5, r5, #1
    b .L080276C8
.L080274A6:
    movs r5, #0x4e
    b .L080276C8
.L080274AA:
    movs r5, #0x46
    b .L080276C8
.L080274AE:
    movs r5, #0x9f
    lsls r5, r5, #1
    b .L080276C8
.L080274B4:
    movs r5, #0xa1
    lsls r5, r5, #1
    b .L080276C8
.L080274BA:
    adds r0, r6, #0
    adds r0, #0x44
    ldrh r5, [r0]
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #1
    b .L08027588
.L080274DA:
    movs r5, #0xf5
    lsls r5, r5, #1
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #2
    b .L08027588
.L080274F8:
    adds r2, r6, #0
    adds r2, #0x4e
    adds r1, r6, #0
    adds r1, #0x4f
    ldrb r0, [r2]
    ldrb r3, [r1]
    cmp r0, r3
    bne .L0802750E
.L08027508:
    movs r5, #0xc9
    lsls r5, r5, #1
    b .L080276C8
.L0802750E:
    ldrb r0, [r1]
    ldrb r1, [r2]
    subs r0, r0, r1
    movs r5, #0xc1
    lsls r5, r5, #1
    cmp r0, #1
    bne .L0802751E
    adds r5, #4
.L0802751E:
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #3
    b .L08027588
.L08027538:
    movs r5, #0x76
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    add r0, sp, #4
    b .L08027588
.L08027552:
    movs r5, #0x6e
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #5
    b .L08027588
.L0802756E:
    movs r5, #0x72
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #6
.L08027588:
    strb r1, [r0]
.L0802758A:
    bl GetIconId__C4Tool
    strh r0, [r7]
    b .L080276C8
.L08027592:
    movs r5, #0xfd
    lsls r5, r5, #1
    b .L080276C8
.L08027598:
    movs r5, #0xcb
    lsls r5, r5, #1
    b .L080276C8
.L0802759E:
    movs r5, #0x89
    lsls r5, r5, #1
    b .L080276C8
.L080275A4:
    movs r5, #0x8b
    lsls r5, r5, #1
    b .L080276C8
.L080275AA:
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    beq .L080275DC
    cmp r0, #1
    bgt .L080275CC
    cmp r0, #0
    beq .L080275D2
    b .L080276C8
.L080275CC:
    cmp r0, #2
    beq .L080275E8
    b .L080276C8
.L080275D2:
    movs r5, #0xea
    ldr r1, .L080275D8 @ =0x0000077A
    b .L08027676
    .align 2, 0
.L080275D8: .4byte 0x0000077A
.L080275DC:
    movs r5, #0xee
    ldr r1, .L080275E4 @ =0x0000077E
    b .L08027676
    .align 2, 0
.L080275E4: .4byte 0x0000077E
.L080275E8:
    movs r5, #0xe6
    ldr r1, .L080275F0 @ =0x00000782
    b .L08027676
    .align 2, 0
.L080275F0: .4byte 0x00000782
.L080275F4:
    movs r5, #0xad
    lsls r5, r5, #1
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L0802760C @ =0x0000077A
    b .L08027676
    .align 2, 0
.L0802760C: .4byte 0x0000077A
.L08027610:
    movs r5, #0x9a
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08027624 @ =0x0000077A
    b .L08027676
    .align 2, 0
.L08027624: .4byte 0x0000077A
.L08027628:
    movs r5, #0x83
    lsls r5, r5, #1
    b .L080276C8
.L0802762E:
    movs r5, #0x81
    lsls r5, r5, #1
    b .L080276C8
.L08027634:
    movs r5, #0xfe
    b .L080276C8
.L08027638:
    movs r5, #0xfa
    b .L080276C8
.L0802763C:
    movs r5, #0x91
    lsls r5, r5, #1
    b .L080276C8
.L08027642:
    movs r5, #0x8f
    lsls r5, r5, #1
    b .L080276C8
.L08027648:
    movs r5, #0x8d
    lsls r5, r5, #1
    b .L080276C8
.L0802764E:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xa5
    lsls r5, r5, #1
    cmp r0, #0
    beq .L080276C8
    subs r5, #0x40
    b .L080276C8
.L08027664:
    movs r5, #0xe2
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08027684 @ =0x00000792
.L08027676:
    ldrh r0, [r2, #0x22]
    cmp r0, r1
    beq .L080276C8
    adds r0, r2, #0
    bl SetAnim__12AActorEntityUi
    b .L080276C8
    .align 2, 0
.L08027684: .4byte 0x00000792
.L08027688:
    movs r5, #0x3e
    b .L080276C8
.L0802768C:
    movs r5, #0xa3
    lsls r5, r5, #1
    b .L080276C8
.L08027692:
    movs r5, #0xa9
    lsls r5, r5, #1
    b .L080276C8
.L08027698:
    movs r5, #0x9d
    lsls r5, r5, #1
    b .L080276C8
.L0802769E:
    movs r5, #0x42
    b .L080276C8
.L080276A2:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xe3
    lsls r5, r5, #1
    cmp r0, #0
    bne .L080276C8
.L080276B4:
    movs r5, #0xab
    lsls r5, r5, #1
    b .L080276C8
.L080276BA:
    ldr r0, [r4]
    b .L08027702
.L080276BE:
    ldrb r0, [r2]
    movs r5, #0xf6
    cmp r0, #0
    beq .L080276C8
    movs r5, #0xf2
.L080276C8:
    lsls r1, r5, #0x10
    lsrs r1, r1, #0x10
    adds r0, r6, #0
    bl SetAnim__12AActorEntityUi
    ldr r0, [r6]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    adds r6, #0xc0
    cmp r4, r5
    beq .L080276FE
.L080276EC:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L080276BA
    adds r4, #4
    cmp r4, r5
    bne .L080276EC
.L080276FE:
    subs r0, r5, #4
    ldr r0, [r0]
.L08027702:
    movs r1, #0xbc
    bl func_08008B6C
    movs r0, #1
    strb r0, [r6]
    add sp, #8
    pop {r3, r4}
    mov r8, r3
    mov sb, r4
    pop {r4, r5, r6, r7}
    pop {r1}
    bx r1
    .align 2, 0

    .section .text.entities_part1.0802771C, "ax", %progbits
    thumb_func_start func_0802771C
func_0802771C: @ 0x0802771C
    push {r4, r5, r6, r7, lr}
    sub sp, #8
    adds r6, r0, #0
    ldr r0, [r6, #0x34]
    ldr r2, .L08027798 @ =0x00001AA8
    adds r0, r0, r2
    bl func_0809ABD8
    adds r0, r6, #0
    adds r0, #0x88
    movs r5, #0
    movs r7, #1
    strb r7, [r0]
    strb r5, [r0, #1]
    movs r1, #0xa8
    lsls r1, r1, #0xd
    str r5, [r0, #4]
    str r5, [r0, #8]
    str r1, [r0, #0x14]
    ldr r4, [r6, #0x10]
    cmp r4, #0
    beq .L0802776E
    ldr r0, [r4]
    ldr r0, [r0]
    ldr r1, [r0]
    ldr r1, [r1, #0x64]
    bl _call_via_r1
    adds r1, r0, #0
    adds r0, r4, #0
    adds r0, #0xb4
    movs r2, #0x6a
    bl func_0805E850
    adds r0, r4, #0
    adds r0, #0xc8
    strb r7, [r0]
    adds r0, #2
    strb r5, [r0]
    adds r0, #1
    strb r7, [r0]
.L0802776E:
    adds r2, r6, #0
    adds r2, #0x3c
    movs r0, #0x31
    strb r0, [r2]
    movs r5, #0xc9
    lsls r5, r5, #1
    adds r1, r6, #0
    adds r1, #0x7c
    ldr r0, .L0802779C @ =0x0000FFFF
    strh r0, [r1]
    ldrb r0, [r2]
    adds r7, r1, #0
    cmp r0, #0x39
    bls .L0802778C
    b .L08027BB0
.L0802778C:
    lsls r0, r0, #2
    ldr r1, .L080277A0 @ =.L080277A4
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L08027798: .4byte 0x00001AA8
.L0802779C: .4byte 0x0000FFFF
.L080277A0: .4byte .L080277A4
.L080277A4: @ jump table
    .4byte .L0802788C @ case 0
    .4byte .L08027966 @ case 1
    .4byte .L08027900 @ case 2
    .4byte .L08027966 @ case 3
    .4byte .L08027950 @ case 4
    .4byte .L08027966 @ case 5
    .4byte .L08027A82 @ case 6
    .4byte .L08027A8E @ case 7
    .4byte .L08027BA2 @ case 8
    .4byte .L0802799E @ case 9
    .4byte .L08027B82 @ case 10
    .4byte .L08027984 @ case 11
    .4byte .L08027984 @ case 12
    .4byte .L0802797E @ case 13
    .4byte .L08027984 @ case 14
    .4byte .L0802798A @ case 15
    .4byte .L0802798E @ case 16
    .4byte .L08027B70 @ case 17
    .4byte .L08027B7C @ case 18
    .4byte .L08027992 @ case 19
    .4byte .L08027998 @ case 20
    .4byte .L08027B82 @ case 21
    .4byte .L08027A76 @ case 22
    .4byte .L08027A7C @ case 23
    .4byte .L08027A88 @ case 24
    .4byte .L080279EC @ case 25
    .4byte .L080279EC @ case 26
    .4byte .L080279BE @ case 27
    .4byte .L080279DC @ case 28
    .4byte .L080279DC @ case 29
    .4byte .L08027A1C @ case 30
    .4byte .L08027A36 @ case 31
    .4byte .L08027A52 @ case 32
    .4byte .L08027B70 @ case 33
    .4byte .L08027AD8 @ case 34
    .4byte .L08027AF4 @ case 35
    .4byte .L08027B0C @ case 36
    .4byte .L08027B12 @ case 37
    .4byte .L08027B18 @ case 38
    .4byte .L08027B1C @ case 39
    .4byte .L08027B20 @ case 40
    .4byte .L08027B26 @ case 41
    .4byte .L08027B2C @ case 42
    .4byte .L08027B2C @ case 43
    .4byte .L08027B32 @ case 44
    .4byte .L08027B48 @ case 45
    .4byte .L08027B6C @ case 46
    .4byte .L08027B82 @ case 47
    .4byte .L08027B76 @ case 48
    .4byte .L08027B70 @ case 49
    .4byte .L08027B76 @ case 50
    .4byte .L08027B7C @ case 51
    .4byte .L08027B82 @ case 52
    .4byte .L08027B86 @ case 53
    .4byte .L08027B70 @ case 54
    .4byte .L08027B70 @ case 55
    .4byte .L08027B76 @ case 56
    .4byte .L08027B76 @ case 57
.L0802788C:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L080278BE
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    bne .L080278A6
    b .L08027B98
.L080278A6:
    cmp r0, #1
    bgt .L080278B2
    cmp r0, #0
    bne .L080278B0
    b .L08027B76
.L080278B0:
    b .L08027BB0
.L080278B2:
    cmp r0, #2
    beq .L080278B8
    b .L08027BB0
.L080278B8:
    movs r5, #0xa7
    lsls r5, r5, #1
    b .L08027BB0
.L080278BE:
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    beq .L080278DA
    cmp r0, #1
    bgt .L080278D4
    cmp r0, #0
    bne .L080278D2
    b .L080279EC
.L080278D2:
    b .L08027BB0
.L080278D4:
    cmp r0, #2
    beq .L080278EC
    b .L08027BB0
.L080278DA:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L080278FC
    adds r1, #0x94
    b .L080278FC
.L080278EC:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L080278FC
    adds r1, #0x2c
.L080278FC:
    adds r5, r1, #0
    b .L08027BB0
.L08027900:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08027910
    b .L08027B76
.L08027910:
    adds r0, r6, #0
    adds r0, #0x3f
    ldrb r3, [r0]
    cmp r3, #5
    bgt .L080279EC
    cmp r3, #0
    blt .L080279EC
    ldr r2, .L0802794C @ =gUnk_080F0FCC
    adds r1, r6, #0
    adds r1, #0x40
    lsls r0, r3, #3
    subs r0, r0, r3
    ldrb r1, [r1]
    adds r0, r0, r1
    lsls r0, r0, #1
    adds r0, r0, r2
    ldrh r5, [r0]
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    mov r1, sp
    strb r0, [r1]
    mov r0, sp
    b .L08027A6E
    .align 2, 0
.L0802794C: .4byte gUnk_080F0FCC
.L08027950:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L08027964
    b .L08027BB0
.L08027964:
    b .L08027B76
.L08027966:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L0802797A
    b .L08027BB0
.L0802797A:
    subs r5, #0x40
    b .L08027BB0
.L0802797E:
    movs r5, #0x9b
    lsls r5, r5, #1
    b .L08027BB0
.L08027984:
    movs r5, #0xcf
    lsls r5, r5, #1
    b .L08027BB0
.L0802798A:
    movs r5, #0x4e
    b .L08027BB0
.L0802798E:
    movs r5, #0x46
    b .L08027BB0
.L08027992:
    movs r5, #0x9f
    lsls r5, r5, #1
    b .L08027BB0
.L08027998:
    movs r5, #0xa1
    lsls r5, r5, #1
    b .L08027BB0
.L0802799E:
    adds r0, r6, #0
    adds r0, #0x44
    ldrh r5, [r0]
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #1
    b .L08027A6C
.L080279BE:
    movs r5, #0xf5
    lsls r5, r5, #1
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #2
    b .L08027A6C
.L080279DC:
    adds r2, r6, #0
    adds r2, #0x4e
    adds r1, r6, #0
    adds r1, #0x4f
    ldrb r0, [r2]
    ldrb r3, [r1]
    cmp r0, r3
    bne .L080279F2
.L080279EC:
    movs r5, #0xc9
    lsls r5, r5, #1
    b .L08027BB0
.L080279F2:
    ldrb r0, [r1]
    ldrb r1, [r2]
    subs r0, r0, r1
    movs r5, #0xc1
    lsls r5, r5, #1
    cmp r0, #1
    bne .L08027A02
    adds r5, #4
.L08027A02:
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #3
    b .L08027A6C
.L08027A1C:
    movs r5, #0x76
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    add r0, sp, #4
    b .L08027A6C
.L08027A36:
    movs r5, #0x6e
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #5
    b .L08027A6C
.L08027A52:
    movs r5, #0x72
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #6
.L08027A6C:
    strb r1, [r0]
.L08027A6E:
    bl GetIconId__C4Tool
    strh r0, [r7]
    b .L08027BB0
.L08027A76:
    movs r5, #0xfd
    lsls r5, r5, #1
    b .L08027BB0
.L08027A7C:
    movs r5, #0xcb
    lsls r5, r5, #1
    b .L08027BB0
.L08027A82:
    movs r5, #0x89
    lsls r5, r5, #1
    b .L08027BB0
.L08027A88:
    movs r5, #0x8b
    lsls r5, r5, #1
    b .L08027BB0
.L08027A8E:
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    beq .L08027AC0
    cmp r0, #1
    bgt .L08027AB0
    cmp r0, #0
    beq .L08027AB6
    b .L08027BB0
.L08027AB0:
    cmp r0, #2
    beq .L08027ACC
    b .L08027BB0
.L08027AB6:
    movs r5, #0xea
    ldr r1, .L08027ABC @ =0x0000077A
    b .L08027B5A
    .align 2, 0
.L08027ABC: .4byte 0x0000077A
.L08027AC0:
    movs r5, #0xee
    ldr r1, .L08027AC8 @ =0x0000077E
    b .L08027B5A
    .align 2, 0
.L08027AC8: .4byte 0x0000077E
.L08027ACC:
    movs r5, #0xe6
    ldr r1, .L08027AD4 @ =0x00000782
    b .L08027B5A
    .align 2, 0
.L08027AD4: .4byte 0x00000782
.L08027AD8:
    movs r5, #0xad
    lsls r5, r5, #1
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08027AF0 @ =0x0000077A
    b .L08027B5A
    .align 2, 0
.L08027AF0: .4byte 0x0000077A
.L08027AF4:
    movs r5, #0x9a
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08027B08 @ =0x0000077A
    b .L08027B5A
    .align 2, 0
.L08027B08: .4byte 0x0000077A
.L08027B0C:
    movs r5, #0x83
    lsls r5, r5, #1
    b .L08027BB0
.L08027B12:
    movs r5, #0x81
    lsls r5, r5, #1
    b .L08027BB0
.L08027B18:
    movs r5, #0xfe
    b .L08027BB0
.L08027B1C:
    movs r5, #0xfa
    b .L08027BB0
.L08027B20:
    movs r5, #0x91
    lsls r5, r5, #1
    b .L08027BB0
.L08027B26:
    movs r5, #0x8f
    lsls r5, r5, #1
    b .L08027BB0
.L08027B2C:
    movs r5, #0x8d
    lsls r5, r5, #1
    b .L08027BB0
.L08027B32:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xa5
    lsls r5, r5, #1
    cmp r0, #0
    beq .L08027BB0
    subs r5, #0x40
    b .L08027BB0
.L08027B48:
    movs r5, #0xe2
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08027B68 @ =0x00000792
.L08027B5A:
    ldrh r0, [r2, #0x22]
    cmp r0, r1
    beq .L08027BB0
    adds r0, r2, #0
    bl SetAnim__12AActorEntityUi
    b .L08027BB0
    .align 2, 0
.L08027B68: .4byte 0x00000792
.L08027B6C:
    movs r5, #0x3e
    b .L08027BB0
.L08027B70:
    movs r5, #0xa3
    lsls r5, r5, #1
    b .L08027BB0
.L08027B76:
    movs r5, #0xa9
    lsls r5, r5, #1
    b .L08027BB0
.L08027B7C:
    movs r5, #0x9d
    lsls r5, r5, #1
    b .L08027BB0
.L08027B82:
    movs r5, #0x42
    b .L08027BB0
.L08027B86:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xe3
    lsls r5, r5, #1
    cmp r0, #0
    bne .L08027BB0
.L08027B98:
    movs r5, #0xab
    lsls r5, r5, #1
    b .L08027BB0
.L08027B9E:
    ldr r0, [r4]
    b .L08027BEA
.L08027BA2:
    adds r0, r6, #0
    adds r0, #0x3d
    ldrb r0, [r0]
    movs r5, #0xf6
    cmp r0, #0
    beq .L08027BB0
    movs r5, #0xf2
.L08027BB0:
    lsls r1, r5, #0x10
    lsrs r1, r1, #0x10
    adds r0, r6, #0
    bl SetAnim__12AActorEntityUi
    ldr r0, [r6]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    adds r6, #0xc0
    cmp r4, r5
    beq .L08027BE6
.L08027BD4:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08027B9E
    adds r4, #4
    cmp r4, r5
    bne .L08027BD4
.L08027BE6:
    subs r0, r5, #4
    ldr r0, [r0]
.L08027BEA:
    movs r1, #0x6a
    bl func_08008B6C
    movs r0, #1
    strb r0, [r6]
    add sp, #8
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0

    .section .text.entities_part1.08027BFC, "ax", %progbits
    thumb_func_start func_08027BFC
func_08027BFC: @ 0x08027BFC
    push {r4, r5, r6, r7, lr}
    mov r7, sl
    mov r6, sb
    mov r5, r8
    push {r5, r6, r7}
    sub sp, #0x14
    adds r6, r0, #0
    ldr r0, [r6, #0x34]
    ldr r1, .L08027D00 @ =0x00001C2C
    adds r4, r0, r1
    adds r0, r4, #0
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08027C1E
    b .L0802816C
.L08027C1E:
    adds r0, r4, #0
    bl func_0800F204
    cmp r0, #4
    beq .L08027C2A
    b .L0802816C
.L08027C2A:
    ldr r0, [r6, #0x34]
    movs r2, #0xe5
    lsls r2, r2, #5
    adds r0, r0, r2
    bl func_0800FFD0
    cmp r0, #0
    bne .L08027C3C
    b .L08028182
.L08027C3C:
    ldr r0, [r6, #0x34]
    movs r3, #0xe5
    lsls r3, r3, #5
    adds r0, r0, r3
    bl func_0800FFD4
    adds r4, r0, #0
    ldr r0, [r6, #0x34]
    movs r1, #0xe5
    lsls r1, r1, #5
    adds r0, r0, r1
    bl func_0800FFD8
    adds r5, r0, #0
    adds r7, r6, #0
    adds r7, #0x88
    movs r2, #0x3c
    adds r2, r2, r6
    mov sl, r2
    adds r3, r6, #0
    adds r3, #0xc0
    str r3, [sp, #0x10]
    movs r0, #0x7c
    adds r0, r0, r6
    mov sb, r0
    cmp r4, r5
    beq .L08027C8A
.L08027C72:
    ldr r0, [r6]
    ldr r1, [r0]
    movs r2, #0xae
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r2, [r1]
    adds r1, r4, #0
    bl _call_via_r2
    adds r4, #1
    cmp r4, r5
    bne .L08027C72
.L08027C8A:
    ldr r0, [r6, #0x34]
    movs r3, #0xe5
    lsls r3, r3, #5
    adds r0, r0, r3
    bl func_08010024
    movs r0, #0
    mov r8, r0
    str r0, [r7, #4]
    str r0, [r7, #8]
    str r0, [r7, #0x14]
    ldr r5, [r6, #0x10]
    cmp r5, #0
    beq .L08027CD8
    adds r0, r6, #0
    adds r0, #0x20
    ldrb r4, [r0]
    adds r4, #0x36
    ldr r0, [r5]
    ldr r0, [r0]
    ldr r1, [r0]
    ldr r1, [r1, #0x64]
    bl _call_via_r1
    adds r1, r0, #0
    adds r0, r5, #0
    adds r0, #0xb4
    adds r2, r4, #0
    bl func_0805E850
    adds r0, r5, #0
    adds r0, #0xc8
    movs r1, #1
    strb r1, [r0]
    adds r0, #2
    mov r2, r8
    strb r2, [r0]
    adds r0, #1
    strb r1, [r0]
.L08027CD8:
    movs r0, #0x2e
    mov r3, sl
    strb r0, [r3]
    movs r0, #1
    ldr r1, [sp, #0x10]
    strb r0, [r1]
    movs r5, #0xc9
    lsls r5, r5, #1
    ldr r0, .L08027D04 @ =0x0000FFFF
    mov r2, sb
    strh r0, [r2]
    ldrb r0, [r3]
    cmp r0, #0x39
    bls .L08027CF6
    b .L08028130
.L08027CF6:
    lsls r0, r0, #2
    ldr r1, .L08027D08 @ =.L08027D0C
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L08027D00: .4byte 0x00001C2C
.L08027D04: .4byte 0x0000FFFF
.L08027D08: .4byte .L08027D0C
.L08027D0C: @ jump table
    .4byte .L08027DF4 @ case 0
    .4byte .L08027ECE @ case 1
    .4byte .L08027E68 @ case 2
    .4byte .L08027ECE @ case 3
    .4byte .L08027EB8 @ case 4
    .4byte .L08027ECE @ case 5
    .4byte .L08028000 @ case 6
    .4byte .L0802800C @ case 7
    .4byte .L08028122 @ case 8
    .4byte .L08027F06 @ case 9
    .4byte .L08028102 @ case 10
    .4byte .L08027EEC @ case 11
    .4byte .L08027EEC @ case 12
    .4byte .L08027EE6 @ case 13
    .4byte .L08027EEC @ case 14
    .4byte .L08027EF2 @ case 15
    .4byte .L08027EF6 @ case 16
    .4byte .L080280F0 @ case 17
    .4byte .L080280FC @ case 18
    .4byte .L08027EFA @ case 19
    .4byte .L08027F00 @ case 20
    .4byte .L08028102 @ case 21
    .4byte .L08027FF4 @ case 22
    .4byte .L08027FFA @ case 23
    .4byte .L08028006 @ case 24
    .4byte .L08027F54 @ case 25
    .4byte .L08027F54 @ case 26
    .4byte .L08027F26 @ case 27
    .4byte .L08027F44 @ case 28
    .4byte .L08027F44 @ case 29
    .4byte .L08027F84 @ case 30
    .4byte .L08027FA8 @ case 31
    .4byte .L08027FCE @ case 32
    .4byte .L080280F0 @ case 33
    .4byte .L08028058 @ case 34
    .4byte .L08028074 @ case 35
    .4byte .L0802808C @ case 36
    .4byte .L08028092 @ case 37
    .4byte .L08028098 @ case 38
    .4byte .L0802809C @ case 39
    .4byte .L080280A0 @ case 40
    .4byte .L080280A6 @ case 41
    .4byte .L080280AC @ case 42
    .4byte .L080280AC @ case 43
    .4byte .L080280B2 @ case 44
    .4byte .L080280C8 @ case 45
    .4byte .L080280EC @ case 46
    .4byte .L08028102 @ case 47
    .4byte .L080280F6 @ case 48
    .4byte .L080280F0 @ case 49
    .4byte .L080280F6 @ case 50
    .4byte .L080280FC @ case 51
    .4byte .L08028102 @ case 52
    .4byte .L08028106 @ case 53
    .4byte .L080280F0 @ case 54
    .4byte .L080280F0 @ case 55
    .4byte .L080280F6 @ case 56
    .4byte .L080280F6 @ case 57
.L08027DF4:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08027E26
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    bne .L08027E0E
    b .L08028118
.L08027E0E:
    cmp r0, #1
    bgt .L08027E1A
    cmp r0, #0
    bne .L08027E18
    b .L080280F6
.L08027E18:
    b .L08028130
.L08027E1A:
    cmp r0, #2
    beq .L08027E20
    b .L08028130
.L08027E20:
    movs r5, #0xa7
    lsls r5, r5, #1
    b .L08028130
.L08027E26:
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    beq .L08027E42
    cmp r0, #1
    bgt .L08027E3C
    cmp r0, #0
    bne .L08027E3A
    b .L08027F54
.L08027E3A:
    b .L08028130
.L08027E3C:
    cmp r0, #2
    beq .L08027E54
    b .L08028130
.L08027E42:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L08027E64
    adds r1, #0x94
    b .L08027E64
.L08027E54:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L08027E64
    adds r1, #0x2c
.L08027E64:
    adds r5, r1, #0
    b .L08028130
.L08027E68:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08027E78
    b .L080280F6
.L08027E78:
    adds r0, r6, #0
    adds r0, #0x3f
    ldrb r3, [r0]
    cmp r3, #5
    bgt .L08027F54
    cmp r3, #0
    blt .L08027F54
    ldr r2, .L08027EB4 @ =gUnk_080F0FCC
    adds r1, r6, #0
    adds r1, #0x40
    lsls r0, r3, #3
    subs r0, r0, r3
    ldrb r1, [r1]
    adds r0, r0, r1
    lsls r0, r0, #1
    adds r0, r0, r2
    ldrh r5, [r0]
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    mov r1, sp
    strb r0, [r1]
    mov r0, sp
    b .L08027FC4
    .align 2, 0
.L08027EB4: .4byte gUnk_080F0FCC
.L08027EB8:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L08027ECC
    b .L08028130
.L08027ECC:
    b .L080280F6
.L08027ECE:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L08027EE2
    b .L08028130
.L08027EE2:
    subs r5, #0x40
    b .L08028130
.L08027EE6:
    movs r5, #0x9b
    lsls r5, r5, #1
    b .L08028130
.L08027EEC:
    movs r5, #0xcf
    lsls r5, r5, #1
    b .L08028130
.L08027EF2:
    movs r5, #0x4e
    b .L08028130
.L08027EF6:
    movs r5, #0x46
    b .L08028130
.L08027EFA:
    movs r5, #0x9f
    lsls r5, r5, #1
    b .L08028130
.L08027F00:
    movs r5, #0xa1
    lsls r5, r5, #1
    b .L08028130
.L08027F06:
    adds r0, r6, #0
    adds r0, #0x44
    ldrh r5, [r0]
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #1
    b .L08027FE8
.L08027F26:
    movs r5, #0xf5
    lsls r5, r5, #1
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #2
    b .L08027F9C
.L08027F44:
    adds r2, r6, #0
    adds r2, #0x4e
    adds r1, r6, #0
    adds r1, #0x4f
    ldrb r0, [r2]
    ldrb r3, [r1]
    cmp r0, r3
    bne .L08027F5A
.L08027F54:
    movs r5, #0xc9
    lsls r5, r5, #1
    b .L08028130
.L08027F5A:
    ldrb r0, [r1]
    ldrb r1, [r2]
    subs r0, r0, r1
    movs r5, #0xc1
    lsls r5, r5, #1
    cmp r0, #1
    bne .L08027F6A
    adds r5, #4
.L08027F6A:
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #3
    b .L08027FE8
.L08027F84:
    movs r5, #0x76
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    add r0, sp, #4
.L08027F9C:
    strb r1, [r0]
    bl GetIconId__C4Tool
    mov r2, sb
    strh r0, [r2]
    b .L08028130
.L08027FA8:
    movs r5, #0x6e
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #5
    strb r1, [r0]
.L08027FC4:
    bl GetIconId__C4Tool
    mov r3, sb
    strh r0, [r3]
    b .L08028130
.L08027FCE:
    movs r5, #0x72
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #6
.L08027FE8:
    strb r1, [r0]
    bl GetIconId__C4Tool
    mov r1, sb
    strh r0, [r1]
    b .L08028130
.L08027FF4:
    movs r5, #0xfd
    lsls r5, r5, #1
    b .L08028130
.L08027FFA:
    movs r5, #0xcb
    lsls r5, r5, #1
    b .L08028130
.L08028000:
    movs r5, #0x89
    lsls r5, r5, #1
    b .L08028130
.L08028006:
    movs r5, #0x8b
    lsls r5, r5, #1
    b .L08028130
.L0802800C:
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    beq .L08028040
    cmp r0, #1
    bgt .L0802802E
    cmp r0, #0
    beq .L08028034
    b .L08028130
.L0802802E:
    cmp r0, #2
    beq .L0802804C
    b .L08028130
.L08028034:
    movs r5, #0xea
    ldr r1, .L0802803C @ =0x0000077A
    b .L080280DA
    .align 2, 0
.L0802803C: .4byte 0x0000077A
.L08028040:
    movs r5, #0xee
    ldr r1, .L08028048 @ =0x0000077E
    b .L080280DA
    .align 2, 0
.L08028048: .4byte 0x0000077E
.L0802804C:
    movs r5, #0xe6
    ldr r1, .L08028054 @ =0x00000782
    b .L080280DA
    .align 2, 0
.L08028054: .4byte 0x00000782
.L08028058:
    movs r5, #0xad
    lsls r5, r5, #1
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08028070 @ =0x0000077A
    b .L080280DA
    .align 2, 0
.L08028070: .4byte 0x0000077A
.L08028074:
    movs r5, #0x9a
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08028088 @ =0x0000077A
    b .L080280DA
    .align 2, 0
.L08028088: .4byte 0x0000077A
.L0802808C:
    movs r5, #0x83
    lsls r5, r5, #1
    b .L08028130
.L08028092:
    movs r5, #0x81
    lsls r5, r5, #1
    b .L08028130
.L08028098:
    movs r5, #0xfe
    b .L08028130
.L0802809C:
    movs r5, #0xfa
    b .L08028130
.L080280A0:
    movs r5, #0x91
    lsls r5, r5, #1
    b .L08028130
.L080280A6:
    movs r5, #0x8f
    lsls r5, r5, #1
    b .L08028130
.L080280AC:
    movs r5, #0x8d
    lsls r5, r5, #1
    b .L08028130
.L080280B2:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xa5
    lsls r5, r5, #1
    cmp r0, #0
    beq .L08028130
    subs r5, #0x40
    b .L08028130
.L080280C8:
    movs r5, #0xe2
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L080280E8 @ =0x00000792
.L080280DA:
    ldrh r0, [r2, #0x22]
    cmp r0, r1
    beq .L08028130
    adds r0, r2, #0
    bl SetAnim__12AActorEntityUi
    b .L08028130
    .align 2, 0
.L080280E8: .4byte 0x00000792
.L080280EC:
    movs r5, #0x3e
    b .L08028130
.L080280F0:
    movs r5, #0xa3
    lsls r5, r5, #1
    b .L08028130
.L080280F6:
    movs r5, #0xa9
    lsls r5, r5, #1
    b .L08028130
.L080280FC:
    movs r5, #0x9d
    lsls r5, r5, #1
    b .L08028130
.L08028102:
    movs r5, #0x42
    b .L08028130
.L08028106:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xe3
    lsls r5, r5, #1
    cmp r0, #0
    bne .L08028130
.L08028118:
    movs r5, #0xab
    lsls r5, r5, #1
    b .L08028130
.L0802811E:
    ldr r0, [r4]
    b .L08028168
.L08028122:
    adds r0, r6, #0
    adds r0, #0x3d
    ldrb r0, [r0]
    movs r5, #0xf6
    cmp r0, #0
    beq .L08028130
    movs r5, #0xf2
.L08028130:
    lsls r1, r5, #0x10
    lsrs r1, r1, #0x10
    adds r0, r6, #0
    bl SetAnim__12AActorEntityUi
    ldr r0, [r6]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    cmp r4, r5
    beq .L08028164
.L08028152:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802811E
    adds r4, #4
    cmp r4, r5
    bne .L08028152
.L08028164:
    subs r0, r5, #4
    ldr r0, [r0]
.L08028168:
    movs r1, #0x6e
    b .L0802862E
.L0802816C:
    adds r0, r6, #0
    bl FarmerEntity_GetHeldProduct
    mov r4, sp
    adds r4, #7
    strb r0, [r4]
    adds r0, r4, #0
    bl GetPrice__C7Product
    cmp r0, #0
    bne .L08028186
.L08028182:
    movs r0, #0
    b .L08028634
.L08028186:
    ldr r0, [r6]
    ldr r1, [r0]
    movs r3, #0xae
    lsls r3, r3, #1
    adds r1, r1, r3
    ldr r2, [r1]
    adds r1, r4, #0
    bl _call_via_r2
    adds r1, r6, #0
    adds r1, #0xc0
    movs r0, #1
    strb r0, [r1]
    adds r2, r6, #0
    adds r2, #0x3c
    movs r0, #0xe
    strb r0, [r2]
    movs r5, #0xc9
    lsls r5, r5, #1
    subs r1, #0x44
    ldr r0, .L080281C8 @ =0x0000FFFF
    strh r0, [r1]
    ldrb r0, [r2]
    mov sb, r1
    cmp r0, #0x39
    bls .L080281BC
    b .L080285F4
.L080281BC:
    lsls r0, r0, #2
    ldr r1, .L080281CC @ =.L080281D0
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L080281C8: .4byte 0x0000FFFF
.L080281CC: .4byte .L080281D0
.L080281D0: @ jump table
    .4byte .L080282B8 @ case 0
    .4byte .L08028392 @ case 1
    .4byte .L0802832C @ case 2
    .4byte .L08028392 @ case 3
    .4byte .L0802837C @ case 4
    .4byte .L08028392 @ case 5
    .4byte .L080284C4 @ case 6
    .4byte .L080284D0 @ case 7
    .4byte .L080285E6 @ case 8
    .4byte .L080283CA @ case 9
    .4byte .L080285C6 @ case 10
    .4byte .L080283B0 @ case 11
    .4byte .L080283B0 @ case 12
    .4byte .L080283AA @ case 13
    .4byte .L080283B0 @ case 14
    .4byte .L080283B6 @ case 15
    .4byte .L080283BA @ case 16
    .4byte .L080285B4 @ case 17
    .4byte .L080285C0 @ case 18
    .4byte .L080283BE @ case 19
    .4byte .L080283C4 @ case 20
    .4byte .L080285C6 @ case 21
    .4byte .L080284B8 @ case 22
    .4byte .L080284BE @ case 23
    .4byte .L080284CA @ case 24
    .4byte .L08028418 @ case 25
    .4byte .L08028418 @ case 26
    .4byte .L080283EA @ case 27
    .4byte .L08028408 @ case 28
    .4byte .L08028408 @ case 29
    .4byte .L08028448 @ case 30
    .4byte .L0802846C @ case 31
    .4byte .L08028492 @ case 32
    .4byte .L080285B4 @ case 33
    .4byte .L0802851C @ case 34
    .4byte .L08028538 @ case 35
    .4byte .L08028550 @ case 36
    .4byte .L08028556 @ case 37
    .4byte .L0802855C @ case 38
    .4byte .L08028560 @ case 39
    .4byte .L08028564 @ case 40
    .4byte .L0802856A @ case 41
    .4byte .L08028570 @ case 42
    .4byte .L08028570 @ case 43
    .4byte .L08028576 @ case 44
    .4byte .L0802858C @ case 45
    .4byte .L080285B0 @ case 46
    .4byte .L080285C6 @ case 47
    .4byte .L080285BA @ case 48
    .4byte .L080285B4 @ case 49
    .4byte .L080285BA @ case 50
    .4byte .L080285C0 @ case 51
    .4byte .L080285C6 @ case 52
    .4byte .L080285CA @ case 53
    .4byte .L080285B4 @ case 54
    .4byte .L080285B4 @ case 55
    .4byte .L080285BA @ case 56
    .4byte .L080285BA @ case 57
.L080282B8:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L080282EA
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    bne .L080282D2
    b .L080285DC
.L080282D2:
    cmp r0, #1
    bgt .L080282DE
    cmp r0, #0
    bne .L080282DC
    b .L080285BA
.L080282DC:
    b .L080285F4
.L080282DE:
    cmp r0, #2
    beq .L080282E4
    b .L080285F4
.L080282E4:
    movs r5, #0xa7
    lsls r5, r5, #1
    b .L080285F4
.L080282EA:
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    beq .L08028306
    cmp r0, #1
    bgt .L08028300
    cmp r0, #0
    bne .L080282FE
    b .L08028418
.L080282FE:
    b .L080285F4
.L08028300:
    cmp r0, #2
    beq .L08028318
    b .L080285F4
.L08028306:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L08028328
    adds r1, #0x94
    b .L08028328
.L08028318:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L08028328
    adds r1, #0x2c
.L08028328:
    adds r5, r1, #0
    b .L080285F4
.L0802832C:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802833C
    b .L080285BA
.L0802833C:
    adds r0, r6, #0
    adds r0, #0x3f
    ldrb r3, [r0]
    cmp r3, #5
    bgt .L08028418
    cmp r3, #0
    blt .L08028418
    ldr r2, .L08028378 @ =gUnk_080F0FCC
    adds r1, r6, #0
    adds r1, #0x40
    lsls r0, r3, #3
    subs r0, r0, r3
    ldrb r1, [r1]
    adds r0, r0, r1
    lsls r0, r0, #1
    adds r0, r0, r2
    ldrh r5, [r0]
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    add r0, sp, #8
    b .L080284AC
    .align 2, 0
.L08028378: .4byte gUnk_080F0FCC
.L0802837C:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L08028390
    b .L080285F4
.L08028390:
    b .L080285BA
.L08028392:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L080283A6
    b .L080285F4
.L080283A6:
    subs r5, #0x40
    b .L080285F4
.L080283AA:
    movs r5, #0x9b
    lsls r5, r5, #1
    b .L080285F4
.L080283B0:
    movs r5, #0xcf
    lsls r5, r5, #1
    b .L080285F4
.L080283B6:
    movs r5, #0x4e
    b .L080285F4
.L080283BA:
    movs r5, #0x46
    b .L080285F4
.L080283BE:
    movs r5, #0x9f
    lsls r5, r5, #1
    b .L080285F4
.L080283C4:
    movs r5, #0xa1
    lsls r5, r5, #1
    b .L080285F4
.L080283CA:
    adds r0, r6, #0
    adds r0, #0x44
    ldrh r5, [r0]
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #9
    b .L08028460
.L080283EA:
    movs r5, #0xf5
    lsls r5, r5, #1
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #0xa
    b .L08028486
.L08028408:
    adds r2, r6, #0
    adds r2, #0x4e
    adds r1, r6, #0
    adds r1, #0x4f
    ldrb r0, [r2]
    ldrb r3, [r1]
    cmp r0, r3
    bne .L0802841E
.L08028418:
    movs r5, #0xc9
    lsls r5, r5, #1
    b .L080285F4
.L0802841E:
    ldrb r0, [r1]
    ldrb r1, [r2]
    subs r0, r0, r1
    movs r5, #0xc1
    lsls r5, r5, #1
    cmp r0, #1
    bne .L0802842E
    adds r5, #4
.L0802842E:
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #0xb
    b .L080284AC
.L08028448:
    movs r5, #0x76
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    add r0, sp, #0xc
.L08028460:
    strb r1, [r0]
    bl GetIconId__C4Tool
    mov r2, sb
    strh r0, [r2]
    b .L080285F4
.L0802846C:
    movs r5, #0x6e
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #0xd
.L08028486:
    strb r1, [r0]
    bl GetIconId__C4Tool
    mov r3, sb
    strh r0, [r3]
    b .L080285F4
.L08028492:
    movs r5, #0x72
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #0xe
.L080284AC:
    strb r1, [r0]
    bl GetIconId__C4Tool
    mov r1, sb
    strh r0, [r1]
    b .L080285F4
.L080284B8:
    movs r5, #0xfd
    lsls r5, r5, #1
    b .L080285F4
.L080284BE:
    movs r5, #0xcb
    lsls r5, r5, #1
    b .L080285F4
.L080284C4:
    movs r5, #0x89
    lsls r5, r5, #1
    b .L080285F4
.L080284CA:
    movs r5, #0x8b
    lsls r5, r5, #1
    b .L080285F4
.L080284D0:
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    beq .L08028504
    cmp r0, #1
    bgt .L080284F2
    cmp r0, #0
    beq .L080284F8
    b .L080285F4
.L080284F2:
    cmp r0, #2
    beq .L08028510
    b .L080285F4
.L080284F8:
    movs r5, #0xea
    ldr r1, .L08028500 @ =0x0000077A
    b .L0802859E
    .align 2, 0
.L08028500: .4byte 0x0000077A
.L08028504:
    movs r5, #0xee
    ldr r1, .L0802850C @ =0x0000077E
    b .L0802859E
    .align 2, 0
.L0802850C: .4byte 0x0000077E
.L08028510:
    movs r5, #0xe6
    ldr r1, .L08028518 @ =0x00000782
    b .L0802859E
    .align 2, 0
.L08028518: .4byte 0x00000782
.L0802851C:
    movs r5, #0xad
    lsls r5, r5, #1
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08028534 @ =0x0000077A
    b .L0802859E
    .align 2, 0
.L08028534: .4byte 0x0000077A
.L08028538:
    movs r5, #0x9a
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L0802854C @ =0x0000077A
    b .L0802859E
    .align 2, 0
.L0802854C: .4byte 0x0000077A
.L08028550:
    movs r5, #0x83
    lsls r5, r5, #1
    b .L080285F4
.L08028556:
    movs r5, #0x81
    lsls r5, r5, #1
    b .L080285F4
.L0802855C:
    movs r5, #0xfe
    b .L080285F4
.L08028560:
    movs r5, #0xfa
    b .L080285F4
.L08028564:
    movs r5, #0x91
    lsls r5, r5, #1
    b .L080285F4
.L0802856A:
    movs r5, #0x8f
    lsls r5, r5, #1
    b .L080285F4
.L08028570:
    movs r5, #0x8d
    lsls r5, r5, #1
    b .L080285F4
.L08028576:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xa5
    lsls r5, r5, #1
    cmp r0, #0
    beq .L080285F4
    subs r5, #0x40
    b .L080285F4
.L0802858C:
    movs r5, #0xe2
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L080285AC @ =0x00000792
.L0802859E:
    ldrh r0, [r2, #0x22]
    cmp r0, r1
    beq .L080285F4
    adds r0, r2, #0
    bl SetAnim__12AActorEntityUi
    b .L080285F4
    .align 2, 0
.L080285AC: .4byte 0x00000792
.L080285B0:
    movs r5, #0x3e
    b .L080285F4
.L080285B4:
    movs r5, #0xa3
    lsls r5, r5, #1
    b .L080285F4
.L080285BA:
    movs r5, #0xa9
    lsls r5, r5, #1
    b .L080285F4
.L080285C0:
    movs r5, #0x9d
    lsls r5, r5, #1
    b .L080285F4
.L080285C6:
    movs r5, #0x42
    b .L080285F4
.L080285CA:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xe3
    lsls r5, r5, #1
    cmp r0, #0
    bne .L080285F4
.L080285DC:
    movs r5, #0xab
    lsls r5, r5, #1
    b .L080285F4
.L080285E2:
    ldr r0, [r4]
    b .L0802862C
.L080285E6:
    adds r0, r6, #0
    adds r0, #0x3d
    ldrb r0, [r0]
    movs r5, #0xf6
    cmp r0, #0
    beq .L080285F4
    movs r5, #0xf2
.L080285F4:
    lsls r1, r5, #0x10
    lsrs r1, r1, #0x10
    adds r0, r6, #0
    bl SetAnim__12AActorEntityUi
    ldr r0, [r6]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    cmp r4, r5
    beq .L08028628
.L08028616:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L080285E2
    adds r4, #4
    cmp r4, r5
    bne .L08028616
.L08028628:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802862C:
    movs r1, #0x6c
.L0802862E:
    bl func_08008B6C
    movs r0, #1
.L08028634:
    add sp, #0x14
    pop {r3, r4, r5}
    mov r8, r3
    mov sb, r4
    mov sl, r5
    pop {r4, r5, r6, r7}
    pop {r1}
    bx r1

    .section .text.entities_part1.08028644, "ax", %progbits
    thumb_func_start func_08028644
func_08028644: @ 0x08028644
    push {r4, r5, r6, r7, lr}
    sub sp, #8
    adds r6, r0, #0
    ldr r0, [r6, #0x34]
    ldr r1, .L08028690 @ =0x00001C2C
    adds r0, r0, r1
    bl func_0800F190
    lsls r0, r0, #0x18
    lsrs r4, r0, #0x18
    cmp r4, #0
    beq .L0802865E
    b .L08028B46
.L0802865E:
    adds r1, r6, #0
    adds r1, #0xc0
    movs r0, #1
    strb r0, [r1]
    subs r1, #0x84
    movs r0, #0xb
    strb r0, [r1]
    adds r2, r6, #0
    adds r2, #0x88
    movs r0, #0xa8
    lsls r0, r0, #0xd
    str r4, [r2, #4]
    str r4, [r2, #8]
    str r0, [r2, #0x14]
    adds r0, r6, #0
    adds r0, #0x20
    ldrb r3, [r0]
    adds r7, r1, #0
    cmp r3, #1
    beq .L080286B8
    cmp r3, #1
    bgt .L08028694
    cmp r3, #0
    beq .L080286C8
    b .L080286D0
    .align 2, 0
.L08028690: .4byte 0x00001C2C
.L08028694:
    cmp r3, #2
    beq .L0802869E
    cmp r3, #3
    beq .L080286A8
    b .L080286D0
.L0802869E:
    ldr r0, .L080286A4 @ =0xFFFE5556
    b .L080286AA
    .align 2, 0
.L080286A4: .4byte 0xFFFE5556
.L080286A8:
    ldr r0, .L080286B4 @ =0x0001AAAA
.L080286AA:
    str r0, [r2, #0xc]
    str r4, [r2, #0x10]
    str r4, [r2, #0x18]
    b .L080286D0
    .align 2, 0
.L080286B4: .4byte 0x0001AAAA
.L080286B8:
    ldr r0, .L080286C4 @ =0xFFFE5556
    str r4, [r2, #0xc]
    str r0, [r2, #0x10]
    str r4, [r2, #0x18]
    b .L080286D0
    .align 2, 0
.L080286C4: .4byte 0xFFFE5556
.L080286C8:
    ldr r0, .L080286F0 @ =0x0001AAAA
    str r3, [r2, #0xc]
    str r0, [r2, #0x10]
    str r3, [r2, #0x18]
.L080286D0:
    movs r5, #0xc9
    lsls r5, r5, #1
    adds r1, r6, #0
    adds r1, #0x7c
    ldr r0, .L080286F4 @ =0x0000FFFF
    strh r0, [r1]
    ldrb r0, [r7]
    adds r7, r1, #0
    cmp r0, #0x39
    bls .L080286E6
    b .L08028B08
.L080286E6:
    lsls r0, r0, #2
    ldr r1, .L080286F8 @ =.L080286FC
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L080286F0: .4byte 0x0001AAAA
.L080286F4: .4byte 0x0000FFFF
.L080286F8: .4byte .L080286FC
.L080286FC: @ jump table
    .4byte .L080287E4 @ case 0
    .4byte .L080288BE @ case 1
    .4byte .L08028858 @ case 2
    .4byte .L080288BE @ case 3
    .4byte .L080288A8 @ case 4
    .4byte .L080288BE @ case 5
    .4byte .L080289DA @ case 6
    .4byte .L080289E6 @ case 7
    .4byte .L08028AFA @ case 8
    .4byte .L080288F6 @ case 9
    .4byte .L08028ADA @ case 10
    .4byte .L080288DC @ case 11
    .4byte .L080288DC @ case 12
    .4byte .L080288D6 @ case 13
    .4byte .L080288DC @ case 14
    .4byte .L080288E2 @ case 15
    .4byte .L080288E6 @ case 16
    .4byte .L08028AC8 @ case 17
    .4byte .L08028AD4 @ case 18
    .4byte .L080288EA @ case 19
    .4byte .L080288F0 @ case 20
    .4byte .L08028ADA @ case 21
    .4byte .L080289CE @ case 22
    .4byte .L080289D4 @ case 23
    .4byte .L080289E0 @ case 24
    .4byte .L08028944 @ case 25
    .4byte .L08028944 @ case 26
    .4byte .L08028916 @ case 27
    .4byte .L08028934 @ case 28
    .4byte .L08028934 @ case 29
    .4byte .L08028974 @ case 30
    .4byte .L0802898E @ case 31
    .4byte .L080289AA @ case 32
    .4byte .L08028AC8 @ case 33
    .4byte .L08028A30 @ case 34
    .4byte .L08028A4C @ case 35
    .4byte .L08028A64 @ case 36
    .4byte .L08028A6A @ case 37
    .4byte .L08028A70 @ case 38
    .4byte .L08028A74 @ case 39
    .4byte .L08028A78 @ case 40
    .4byte .L08028A7E @ case 41
    .4byte .L08028A84 @ case 42
    .4byte .L08028A84 @ case 43
    .4byte .L08028A8A @ case 44
    .4byte .L08028AA0 @ case 45
    .4byte .L08028AC4 @ case 46
    .4byte .L08028ADA @ case 47
    .4byte .L08028ACE @ case 48
    .4byte .L08028AC8 @ case 49
    .4byte .L08028ACE @ case 50
    .4byte .L08028AD4 @ case 51
    .4byte .L08028ADA @ case 52
    .4byte .L08028ADE @ case 53
    .4byte .L08028AC8 @ case 54
    .4byte .L08028AC8 @ case 55
    .4byte .L08028ACE @ case 56
    .4byte .L08028ACE @ case 57
.L080287E4:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08028816
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    bne .L080287FE
    b .L08028AF0
.L080287FE:
    cmp r0, #1
    bgt .L0802880A
    cmp r0, #0
    bne .L08028808
    b .L08028ACE
.L08028808:
    b .L08028B08
.L0802880A:
    cmp r0, #2
    beq .L08028810
    b .L08028B08
.L08028810:
    movs r5, #0xa7
    lsls r5, r5, #1
    b .L08028B08
.L08028816:
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    beq .L08028832
    cmp r0, #1
    bgt .L0802882C
    cmp r0, #0
    bne .L0802882A
    b .L08028944
.L0802882A:
    b .L08028B08
.L0802882C:
    cmp r0, #2
    beq .L08028844
    b .L08028B08
.L08028832:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L08028854
    adds r1, #0x94
    b .L08028854
.L08028844:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L08028854
    adds r1, #0x2c
.L08028854:
    adds r5, r1, #0
    b .L08028B08
.L08028858:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08028868
    b .L08028ACE
.L08028868:
    adds r0, r6, #0
    adds r0, #0x3f
    ldrb r3, [r0]
    cmp r3, #5
    bgt .L08028944
    cmp r3, #0
    blt .L08028944
    ldr r2, .L080288A4 @ =gUnk_080F0FCC
    adds r1, r6, #0
    adds r1, #0x40
    lsls r0, r3, #3
    subs r0, r0, r3
    ldrb r1, [r1]
    adds r0, r0, r1
    lsls r0, r0, #1
    adds r0, r0, r2
    ldrh r5, [r0]
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    mov r1, sp
    strb r0, [r1]
    mov r0, sp
    b .L080289C6
    .align 2, 0
.L080288A4: .4byte gUnk_080F0FCC
.L080288A8:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L080288BC
    b .L08028B08
.L080288BC:
    b .L08028ACE
.L080288BE:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L080288D2
    b .L08028B08
.L080288D2:
    subs r5, #0x40
    b .L08028B08
.L080288D6:
    movs r5, #0x9b
    lsls r5, r5, #1
    b .L08028B08
.L080288DC:
    movs r5, #0xcf
    lsls r5, r5, #1
    b .L08028B08
.L080288E2:
    movs r5, #0x4e
    b .L08028B08
.L080288E6:
    movs r5, #0x46
    b .L08028B08
.L080288EA:
    movs r5, #0x9f
    lsls r5, r5, #1
    b .L08028B08
.L080288F0:
    movs r5, #0xa1
    lsls r5, r5, #1
    b .L08028B08
.L080288F6:
    adds r0, r6, #0
    adds r0, #0x44
    ldrh r5, [r0]
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #1
    b .L080289C4
.L08028916:
    movs r5, #0xf5
    lsls r5, r5, #1
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #2
    b .L080289C4
.L08028934:
    adds r2, r6, #0
    adds r2, #0x4e
    adds r1, r6, #0
    adds r1, #0x4f
    ldrb r0, [r2]
    ldrb r3, [r1]
    cmp r0, r3
    bne .L0802894A
.L08028944:
    movs r5, #0xc9
    lsls r5, r5, #1
    b .L08028B08
.L0802894A:
    ldrb r0, [r1]
    ldrb r1, [r2]
    subs r0, r0, r1
    movs r5, #0xc1
    lsls r5, r5, #1
    cmp r0, #1
    bne .L0802895A
    adds r5, #4
.L0802895A:
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #3
    b .L080289C4
.L08028974:
    movs r5, #0x76
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    add r0, sp, #4
    b .L080289C4
.L0802898E:
    movs r5, #0x6e
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #5
    b .L080289C4
.L080289AA:
    movs r5, #0x72
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #6
.L080289C4:
    strb r1, [r0]
.L080289C6:
    bl GetIconId__C4Tool
    strh r0, [r7]
    b .L08028B08
.L080289CE:
    movs r5, #0xfd
    lsls r5, r5, #1
    b .L08028B08
.L080289D4:
    movs r5, #0xcb
    lsls r5, r5, #1
    b .L08028B08
.L080289DA:
    movs r5, #0x89
    lsls r5, r5, #1
    b .L08028B08
.L080289E0:
    movs r5, #0x8b
    lsls r5, r5, #1
    b .L08028B08
.L080289E6:
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    beq .L08028A18
    cmp r0, #1
    bgt .L08028A08
    cmp r0, #0
    beq .L08028A0E
    b .L08028B08
.L08028A08:
    cmp r0, #2
    beq .L08028A24
    b .L08028B08
.L08028A0E:
    movs r5, #0xea
    ldr r1, .L08028A14 @ =0x0000077A
    b .L08028AB2
    .align 2, 0
.L08028A14: .4byte 0x0000077A
.L08028A18:
    movs r5, #0xee
    ldr r1, .L08028A20 @ =0x0000077E
    b .L08028AB2
    .align 2, 0
.L08028A20: .4byte 0x0000077E
.L08028A24:
    movs r5, #0xe6
    ldr r1, .L08028A2C @ =0x00000782
    b .L08028AB2
    .align 2, 0
.L08028A2C: .4byte 0x00000782
.L08028A30:
    movs r5, #0xad
    lsls r5, r5, #1
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08028A48 @ =0x0000077A
    b .L08028AB2
    .align 2, 0
.L08028A48: .4byte 0x0000077A
.L08028A4C:
    movs r5, #0x9a
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08028A60 @ =0x0000077A
    b .L08028AB2
    .align 2, 0
.L08028A60: .4byte 0x0000077A
.L08028A64:
    movs r5, #0x83
    lsls r5, r5, #1
    b .L08028B08
.L08028A6A:
    movs r5, #0x81
    lsls r5, r5, #1
    b .L08028B08
.L08028A70:
    movs r5, #0xfe
    b .L08028B08
.L08028A74:
    movs r5, #0xfa
    b .L08028B08
.L08028A78:
    movs r5, #0x91
    lsls r5, r5, #1
    b .L08028B08
.L08028A7E:
    movs r5, #0x8f
    lsls r5, r5, #1
    b .L08028B08
.L08028A84:
    movs r5, #0x8d
    lsls r5, r5, #1
    b .L08028B08
.L08028A8A:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xa5
    lsls r5, r5, #1
    cmp r0, #0
    beq .L08028B08
    subs r5, #0x40
    b .L08028B08
.L08028AA0:
    movs r5, #0xe2
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08028AC0 @ =0x00000792
.L08028AB2:
    ldrh r0, [r2, #0x22]
    cmp r0, r1
    beq .L08028B08
    adds r0, r2, #0
    bl SetAnim__12AActorEntityUi
    b .L08028B08
    .align 2, 0
.L08028AC0: .4byte 0x00000792
.L08028AC4:
    movs r5, #0x3e
    b .L08028B08
.L08028AC8:
    movs r5, #0xa3
    lsls r5, r5, #1
    b .L08028B08
.L08028ACE:
    movs r5, #0xa9
    lsls r5, r5, #1
    b .L08028B08
.L08028AD4:
    movs r5, #0x9d
    lsls r5, r5, #1
    b .L08028B08
.L08028ADA:
    movs r5, #0x42
    b .L08028B08
.L08028ADE:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xe3
    lsls r5, r5, #1
    cmp r0, #0
    bne .L08028B08
.L08028AF0:
    movs r5, #0xab
    lsls r5, r5, #1
    b .L08028B08
.L08028AF6:
    ldr r0, [r4]
    b .L08028B40
.L08028AFA:
    adds r0, r6, #0
    adds r0, #0x3d
    ldrb r0, [r0]
    movs r5, #0xf6
    cmp r0, #0
    beq .L08028B08
    movs r5, #0xf2
.L08028B08:
    lsls r1, r5, #0x10
    lsrs r1, r1, #0x10
    adds r0, r6, #0
    bl SetAnim__12AActorEntityUi
    ldr r0, [r6]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    cmp r4, r5
    beq .L08028B3C
.L08028B2A:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08028AF6
    adds r4, #4
    cmp r4, r5
    bne .L08028B2A
.L08028B3C:
    subs r0, r5, #4
    ldr r0, [r0]
.L08028B40:
    movs r1, #0x6c
    bl func_08008B6C
.L08028B46:
    add sp, #8
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0
    .align 2, 0

    .section .text.entities_part1.08028B50, "ax", %progbits
    thumb_func_start func_08028B50
func_08028B50: @ 0x08028B50
    push {r4, r5, r6, r7, lr}
    sub sp, #8
    adds r6, r0, #0
    adds r7, r6, #0
    adds r7, #0x3c
    movs r0, #0xe
    strb r0, [r7]
    adds r1, r6, #0
    adds r1, #0xc0
    movs r0, #1
    strb r0, [r1]
    ldr r0, [r6, #0x34]
    ldr r1, .L08028BA0 @ =0x00001C2C
    adds r4, r0, r1
    adds r0, r4, #0
    bl func_0800F190
    lsls r0, r0, #0x18
    lsrs r5, r0, #0x18
    cmp r5, #0
    bne .L08028BFA
    adds r0, r4, #0
    bl func_0800F204
    cmp r0, #3
    bgt .L08028BFA
    cmp r0, #2
    blt .L08028BFA
    movs r0, #0xd
    strb r0, [r7]
    adds r0, r6, #0
    adds r0, #0x20
    ldrb r3, [r0]
    cmp r3, #1
    beq .L08028BD6
    cmp r3, #1
    bgt .L08028BA4
    cmp r3, #0
    beq .L08028BE8
    b .L08028BFA
    .align 2, 0
.L08028BA0: .4byte 0x00001C2C
.L08028BA4:
    cmp r3, #2
    beq .L08028BAE
    cmp r3, #3
    beq .L08028BC4
    b .L08028BFA
.L08028BAE:
    adds r0, r6, #0
    adds r0, #0x88
    ldr r1, .L08028BC0 @ =0xFFF60000
    movs r2, #0xc0
    lsls r2, r2, #0xc
    str r1, [r0, #4]
    str r5, [r0, #8]
    b .L08028BF8
    .align 2, 0
.L08028BC0: .4byte 0xFFF60000
.L08028BC4:
    adds r0, r6, #0
    adds r0, #0x88
    movs r1, #0xa0
    lsls r1, r1, #0xc
    movs r2, #0xc0
    lsls r2, r2, #0xc
    str r1, [r0, #4]
    str r5, [r0, #8]
    b .L08028BF8
.L08028BD6:
    adds r0, r6, #0
    adds r0, #0x88
    ldr r1, .L08028BE4 @ =0xFFF60000
    movs r2, #0xc0
    lsls r2, r2, #0xc
    str r5, [r0, #4]
    b .L08028BF6
    .align 2, 0
.L08028BE4: .4byte 0xFFF60000
.L08028BE8:
    adds r0, r6, #0
    adds r0, #0x88
    movs r1, #0xa0
    lsls r1, r1, #0xc
    movs r2, #0xc0
    lsls r2, r2, #0xc
    str r3, [r0, #4]
.L08028BF6:
    str r1, [r0, #8]
.L08028BF8:
    str r2, [r0, #0x14]
.L08028BFA:
    movs r5, #0xc9
    lsls r5, r5, #1
    adds r0, r6, #0
    adds r0, #0x7c
    ldr r1, .L08028C20 @ =0x0000FFFF
    strh r1, [r0]
    adds r1, r6, #0
    adds r1, #0x3c
    ldrb r1, [r1]
    adds r7, r0, #0
    cmp r1, #0x39
    bls .L08028C14
    b .L08029034
.L08028C14:
    lsls r0, r1, #2
    ldr r1, .L08028C24 @ =.L08028C28
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L08028C20: .4byte 0x0000FFFF
.L08028C24: .4byte .L08028C28
.L08028C28: @ jump table
    .4byte .L08028D10 @ case 0
    .4byte .L08028DEA @ case 1
    .4byte .L08028D84 @ case 2
    .4byte .L08028DEA @ case 3
    .4byte .L08028DD4 @ case 4
    .4byte .L08028DEA @ case 5
    .4byte .L08028F06 @ case 6
    .4byte .L08028F12 @ case 7
    .4byte .L08029026 @ case 8
    .4byte .L08028E22 @ case 9
    .4byte .L08029006 @ case 10
    .4byte .L08028E08 @ case 11
    .4byte .L08028E08 @ case 12
    .4byte .L08028E02 @ case 13
    .4byte .L08028E08 @ case 14
    .4byte .L08028E0E @ case 15
    .4byte .L08028E12 @ case 16
    .4byte .L08028FF4 @ case 17
    .4byte .L08029000 @ case 18
    .4byte .L08028E16 @ case 19
    .4byte .L08028E1C @ case 20
    .4byte .L08029006 @ case 21
    .4byte .L08028EFA @ case 22
    .4byte .L08028F00 @ case 23
    .4byte .L08028F0C @ case 24
    .4byte .L08028E70 @ case 25
    .4byte .L08028E70 @ case 26
    .4byte .L08028E42 @ case 27
    .4byte .L08028E60 @ case 28
    .4byte .L08028E60 @ case 29
    .4byte .L08028EA0 @ case 30
    .4byte .L08028EBA @ case 31
    .4byte .L08028ED6 @ case 32
    .4byte .L08028FF4 @ case 33
    .4byte .L08028F5C @ case 34
    .4byte .L08028F78 @ case 35
    .4byte .L08028F90 @ case 36
    .4byte .L08028F96 @ case 37
    .4byte .L08028F9C @ case 38
    .4byte .L08028FA0 @ case 39
    .4byte .L08028FA4 @ case 40
    .4byte .L08028FAA @ case 41
    .4byte .L08028FB0 @ case 42
    .4byte .L08028FB0 @ case 43
    .4byte .L08028FB6 @ case 44
    .4byte .L08028FCC @ case 45
    .4byte .L08028FF0 @ case 46
    .4byte .L08029006 @ case 47
    .4byte .L08028FFA @ case 48
    .4byte .L08028FF4 @ case 49
    .4byte .L08028FFA @ case 50
    .4byte .L08029000 @ case 51
    .4byte .L08029006 @ case 52
    .4byte .L0802900A @ case 53
    .4byte .L08028FF4 @ case 54
    .4byte .L08028FF4 @ case 55
    .4byte .L08028FFA @ case 56
    .4byte .L08028FFA @ case 57
.L08028D10:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08028D42
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    bne .L08028D2A
    b .L0802901C
.L08028D2A:
    cmp r0, #1
    bgt .L08028D36
    cmp r0, #0
    bne .L08028D34
    b .L08028FFA
.L08028D34:
    b .L08029034
.L08028D36:
    cmp r0, #2
    beq .L08028D3C
    b .L08029034
.L08028D3C:
    movs r5, #0xa7
    lsls r5, r5, #1
    b .L08029034
.L08028D42:
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    beq .L08028D5E
    cmp r0, #1
    bgt .L08028D58
    cmp r0, #0
    bne .L08028D56
    b .L08028E70
.L08028D56:
    b .L08029034
.L08028D58:
    cmp r0, #2
    beq .L08028D70
    b .L08029034
.L08028D5E:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L08028D80
    adds r1, #0x94
    b .L08028D80
.L08028D70:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L08028D80
    adds r1, #0x2c
.L08028D80:
    adds r5, r1, #0
    b .L08029034
.L08028D84:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08028D94
    b .L08028FFA
.L08028D94:
    adds r0, r6, #0
    adds r0, #0x3f
    ldrb r3, [r0]
    cmp r3, #5
    bgt .L08028E70
    cmp r3, #0
    blt .L08028E70
    ldr r2, .L08028DD0 @ =gUnk_080F0FCC
    adds r1, r6, #0
    adds r1, #0x40
    lsls r0, r3, #3
    subs r0, r0, r3
    ldrb r1, [r1]
    adds r0, r0, r1
    lsls r0, r0, #1
    adds r0, r0, r2
    ldrh r5, [r0]
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    mov r1, sp
    strb r0, [r1]
    mov r0, sp
    b .L08028EF2
    .align 2, 0
.L08028DD0: .4byte gUnk_080F0FCC
.L08028DD4:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L08028DE8
    b .L08029034
.L08028DE8:
    b .L08028FFA
.L08028DEA:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L08028DFE
    b .L08029034
.L08028DFE:
    subs r5, #0x40
    b .L08029034
.L08028E02:
    movs r5, #0x9b
    lsls r5, r5, #1
    b .L08029034
.L08028E08:
    movs r5, #0xcf
    lsls r5, r5, #1
    b .L08029034
.L08028E0E:
    movs r5, #0x4e
    b .L08029034
.L08028E12:
    movs r5, #0x46
    b .L08029034
.L08028E16:
    movs r5, #0x9f
    lsls r5, r5, #1
    b .L08029034
.L08028E1C:
    movs r5, #0xa1
    lsls r5, r5, #1
    b .L08029034
.L08028E22:
    adds r0, r6, #0
    adds r0, #0x44
    ldrh r5, [r0]
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #1
    b .L08028EF0
.L08028E42:
    movs r5, #0xf5
    lsls r5, r5, #1
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #2
    b .L08028EF0
.L08028E60:
    adds r2, r6, #0
    adds r2, #0x4e
    adds r1, r6, #0
    adds r1, #0x4f
    ldrb r0, [r2]
    ldrb r3, [r1]
    cmp r0, r3
    bne .L08028E76
.L08028E70:
    movs r5, #0xc9
    lsls r5, r5, #1
    b .L08029034
.L08028E76:
    ldrb r0, [r1]
    ldrb r1, [r2]
    subs r0, r0, r1
    movs r5, #0xc1
    lsls r5, r5, #1
    cmp r0, #1
    bne .L08028E86
    adds r5, #4
.L08028E86:
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #3
    b .L08028EF0
.L08028EA0:
    movs r5, #0x76
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    add r0, sp, #4
    b .L08028EF0
.L08028EBA:
    movs r5, #0x6e
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #5
    b .L08028EF0
.L08028ED6:
    movs r5, #0x72
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #6
.L08028EF0:
    strb r1, [r0]
.L08028EF2:
    bl GetIconId__C4Tool
    strh r0, [r7]
    b .L08029034
.L08028EFA:
    movs r5, #0xfd
    lsls r5, r5, #1
    b .L08029034
.L08028F00:
    movs r5, #0xcb
    lsls r5, r5, #1
    b .L08029034
.L08028F06:
    movs r5, #0x89
    lsls r5, r5, #1
    b .L08029034
.L08028F0C:
    movs r5, #0x8b
    lsls r5, r5, #1
    b .L08029034
.L08028F12:
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    beq .L08028F44
    cmp r0, #1
    bgt .L08028F34
    cmp r0, #0
    beq .L08028F3A
    b .L08029034
.L08028F34:
    cmp r0, #2
    beq .L08028F50
    b .L08029034
.L08028F3A:
    movs r5, #0xea
    ldr r1, .L08028F40 @ =0x0000077A
    b .L08028FDE
    .align 2, 0
.L08028F40: .4byte 0x0000077A
.L08028F44:
    movs r5, #0xee
    ldr r1, .L08028F4C @ =0x0000077E
    b .L08028FDE
    .align 2, 0
.L08028F4C: .4byte 0x0000077E
.L08028F50:
    movs r5, #0xe6
    ldr r1, .L08028F58 @ =0x00000782
    b .L08028FDE
    .align 2, 0
.L08028F58: .4byte 0x00000782
.L08028F5C:
    movs r5, #0xad
    lsls r5, r5, #1
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08028F74 @ =0x0000077A
    b .L08028FDE
    .align 2, 0
.L08028F74: .4byte 0x0000077A
.L08028F78:
    movs r5, #0x9a
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08028F8C @ =0x0000077A
    b .L08028FDE
    .align 2, 0
.L08028F8C: .4byte 0x0000077A
.L08028F90:
    movs r5, #0x83
    lsls r5, r5, #1
    b .L08029034
.L08028F96:
    movs r5, #0x81
    lsls r5, r5, #1
    b .L08029034
.L08028F9C:
    movs r5, #0xfe
    b .L08029034
.L08028FA0:
    movs r5, #0xfa
    b .L08029034
.L08028FA4:
    movs r5, #0x91
    lsls r5, r5, #1
    b .L08029034
.L08028FAA:
    movs r5, #0x8f
    lsls r5, r5, #1
    b .L08029034
.L08028FB0:
    movs r5, #0x8d
    lsls r5, r5, #1
    b .L08029034
.L08028FB6:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xa5
    lsls r5, r5, #1
    cmp r0, #0
    beq .L08029034
    subs r5, #0x40
    b .L08029034
.L08028FCC:
    movs r5, #0xe2
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08028FEC @ =0x00000792
.L08028FDE:
    ldrh r0, [r2, #0x22]
    cmp r0, r1
    beq .L08029034
    adds r0, r2, #0
    bl SetAnim__12AActorEntityUi
    b .L08029034
    .align 2, 0
.L08028FEC: .4byte 0x00000792
.L08028FF0:
    movs r5, #0x3e
    b .L08029034
.L08028FF4:
    movs r5, #0xa3
    lsls r5, r5, #1
    b .L08029034
.L08028FFA:
    movs r5, #0xa9
    lsls r5, r5, #1
    b .L08029034
.L08029000:
    movs r5, #0x9d
    lsls r5, r5, #1
    b .L08029034
.L08029006:
    movs r5, #0x42
    b .L08029034
.L0802900A:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xe3
    lsls r5, r5, #1
    cmp r0, #0
    bne .L08029034
.L0802901C:
    movs r5, #0xab
    lsls r5, r5, #1
    b .L08029034
.L08029022:
    ldr r0, [r4]
    b .L0802906C
.L08029026:
    adds r0, r6, #0
    adds r0, #0x3d
    ldrb r0, [r0]
    movs r5, #0xf6
    cmp r0, #0
    beq .L08029034
    movs r5, #0xf2
.L08029034:
    lsls r1, r5, #0x10
    lsrs r1, r1, #0x10
    adds r0, r6, #0
    bl SetAnim__12AActorEntityUi
    ldr r0, [r6]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    cmp r4, r5
    beq .L08029068
.L08029056:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08029022
    adds r4, #4
    cmp r4, r5
    bne .L08029056
.L08029068:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802906C:
    movs r1, #0x6c
    bl func_08008B6C
    add sp, #8
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0
    .align 2, 0


    .section .text.entities_part1.080290D4, "ax", %progbits
    thumb_func_start func_080290D4
func_080290D4: @ 0x080290D4
    push {r4, r5, r6, r7, lr}
    sub sp, #8
    adds r6, r0, #0
    adds r7, r1, #0
    lsls r2, r2, #0x18
    lsrs r5, r2, #0x18
    adds r0, #0xa4
    movs r4, #1
    strb r4, [r0]
    ldr r0, [r6, #0x34]
    ldr r1, .L080290FC @ =0x00001C2C
    adds r0, r0, r1
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08029100
    movs r0, #0
    b .L08029550
    .align 2, 0
.L080290FC: .4byte 0x00001C2C
.L08029100:
    adds r0, r6, #0
    adds r0, #0xc0
    strb r4, [r0]
    adds r0, r6, #0
    adds r1, r7, #0
    adds r2, r5, #0
    bl FarmerEntity_LiftRucksackItem
    movs r5, #0xc9
    lsls r5, r5, #1
    adds r1, r6, #0
    adds r1, #0x7c
    ldr r0, .L08029134 @ =0x0000FFFF
    strh r0, [r1]
    adds r0, r6, #0
    adds r0, #0x3c
    ldrb r0, [r0]
    adds r7, r1, #0
    cmp r0, #0x39
    bls .L0802912A
    b .L08029544
.L0802912A:
    lsls r0, r0, #2
    ldr r1, .L08029138 @ =.L0802913C
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L08029134: .4byte 0x0000FFFF
.L08029138: .4byte .L0802913C
.L0802913C: @ jump table
    .4byte .L08029224 @ case 0
    .4byte .L080292FE @ case 1
    .4byte .L08029298 @ case 2
    .4byte .L080292FE @ case 3
    .4byte .L080292E8 @ case 4
    .4byte .L080292FE @ case 5
    .4byte .L0802941A @ case 6
    .4byte .L08029426 @ case 7
    .4byte .L08029536 @ case 8
    .4byte .L08029336 @ case 9
    .4byte .L0802951A @ case 10
    .4byte .L0802931C @ case 11
    .4byte .L0802931C @ case 12
    .4byte .L08029316 @ case 13
    .4byte .L0802931C @ case 14
    .4byte .L08029322 @ case 15
    .4byte .L08029326 @ case 16
    .4byte .L08029508 @ case 17
    .4byte .L08029514 @ case 18
    .4byte .L0802932A @ case 19
    .4byte .L08029330 @ case 20
    .4byte .L0802951A @ case 21
    .4byte .L0802940E @ case 22
    .4byte .L08029414 @ case 23
    .4byte .L08029420 @ case 24
    .4byte .L08029384 @ case 25
    .4byte .L08029384 @ case 26
    .4byte .L08029356 @ case 27
    .4byte .L08029374 @ case 28
    .4byte .L08029374 @ case 29
    .4byte .L080293B4 @ case 30
    .4byte .L080293CE @ case 31
    .4byte .L080293EA @ case 32
    .4byte .L08029508 @ case 33
    .4byte .L08029470 @ case 34
    .4byte .L0802948C @ case 35
    .4byte .L080294A4 @ case 36
    .4byte .L080294AA @ case 37
    .4byte .L080294B0 @ case 38
    .4byte .L080294B4 @ case 39
    .4byte .L080294B8 @ case 40
    .4byte .L080294BE @ case 41
    .4byte .L080294C4 @ case 42
    .4byte .L080294C4 @ case 43
    .4byte .L080294CA @ case 44
    .4byte .L080294E0 @ case 45
    .4byte .L08029504 @ case 46
    .4byte .L0802951A @ case 47
    .4byte .L0802950E @ case 48
    .4byte .L08029508 @ case 49
    .4byte .L0802950E @ case 50
    .4byte .L08029514 @ case 51
    .4byte .L0802951A @ case 52
    .4byte .L0802951E @ case 53
    .4byte .L08029508 @ case 54
    .4byte .L08029508 @ case 55
    .4byte .L0802950E @ case 56
    .4byte .L0802950E @ case 57
.L08029224:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08029256
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    bne .L0802923E
    b .L08029530
.L0802923E:
    cmp r0, #1
    bgt .L0802924A
    cmp r0, #0
    bne .L08029248
    b .L0802950E
.L08029248:
    b .L08029544
.L0802924A:
    cmp r0, #2
    beq .L08029250
    b .L08029544
.L08029250:
    movs r5, #0xa7
    lsls r5, r5, #1
    b .L08029544
.L08029256:
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    beq .L08029272
    cmp r0, #1
    bgt .L0802926C
    cmp r0, #0
    bne .L0802926A
    b .L08029384
.L0802926A:
    b .L08029544
.L0802926C:
    cmp r0, #2
    beq .L08029284
    b .L08029544
.L08029272:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L08029294
    adds r1, #0x94
    b .L08029294
.L08029284:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L08029294
    adds r1, #0x2c
.L08029294:
    adds r5, r1, #0
    b .L08029544
.L08029298:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L080292A8
    b .L0802950E
.L080292A8:
    adds r0, r6, #0
    adds r0, #0x3f
    ldrb r3, [r0]
    cmp r3, #5
    bgt .L08029384
    cmp r3, #0
    blt .L08029384
    ldr r2, .L080292E4 @ =gUnk_080F0FCC
    adds r1, r6, #0
    adds r1, #0x40
    lsls r0, r3, #3
    subs r0, r0, r3
    ldrb r1, [r1]
    adds r0, r0, r1
    lsls r0, r0, #1
    adds r0, r0, r2
    ldrh r5, [r0]
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    mov r1, sp
    strb r0, [r1]
    mov r0, sp
    b .L08029406
    .align 2, 0
.L080292E4: .4byte gUnk_080F0FCC
.L080292E8:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L080292FC
    b .L08029544
.L080292FC:
    b .L0802950E
.L080292FE:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L08029312
    b .L08029544
.L08029312:
    subs r5, #0x40
    b .L08029544
.L08029316:
    movs r5, #0x9b
    lsls r5, r5, #1
    b .L08029544
.L0802931C:
    movs r5, #0xcf
    lsls r5, r5, #1
    b .L08029544
.L08029322:
    movs r5, #0x4e
    b .L08029544
.L08029326:
    movs r5, #0x46
    b .L08029544
.L0802932A:
    movs r5, #0x9f
    lsls r5, r5, #1
    b .L08029544
.L08029330:
    movs r5, #0xa1
    lsls r5, r5, #1
    b .L08029544
.L08029336:
    adds r0, r6, #0
    adds r0, #0x44
    ldrh r5, [r0]
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #1
    b .L08029404
.L08029356:
    movs r5, #0xf5
    lsls r5, r5, #1
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #2
    b .L08029404
.L08029374:
    adds r2, r6, #0
    adds r2, #0x4e
    adds r1, r6, #0
    adds r1, #0x4f
    ldrb r0, [r2]
    ldrb r3, [r1]
    cmp r0, r3
    bne .L0802938A
.L08029384:
    movs r5, #0xc9
    lsls r5, r5, #1
    b .L08029544
.L0802938A:
    ldrb r0, [r1]
    ldrb r1, [r2]
    subs r0, r0, r1
    movs r5, #0xc1
    lsls r5, r5, #1
    cmp r0, #1
    bne .L0802939A
    adds r5, #4
.L0802939A:
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #3
    b .L08029404
.L080293B4:
    movs r5, #0x76
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    add r0, sp, #4
    b .L08029404
.L080293CE:
    movs r5, #0x6e
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #5
    b .L08029404
.L080293EA:
    movs r5, #0x72
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #6
.L08029404:
    strb r1, [r0]
.L08029406:
    bl GetIconId__C4Tool
    strh r0, [r7]
    b .L08029544
.L0802940E:
    movs r5, #0xfd
    lsls r5, r5, #1
    b .L08029544
.L08029414:
    movs r5, #0xcb
    lsls r5, r5, #1
    b .L08029544
.L0802941A:
    movs r5, #0x89
    lsls r5, r5, #1
    b .L08029544
.L08029420:
    movs r5, #0x8b
    lsls r5, r5, #1
    b .L08029544
.L08029426:
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    beq .L08029458
    cmp r0, #1
    bgt .L08029448
    cmp r0, #0
    beq .L0802944E
    b .L08029544
.L08029448:
    cmp r0, #2
    beq .L08029464
    b .L08029544
.L0802944E:
    movs r5, #0xea
    ldr r1, .L08029454 @ =0x0000077A
    b .L080294F2
    .align 2, 0
.L08029454: .4byte 0x0000077A
.L08029458:
    movs r5, #0xee
    ldr r1, .L08029460 @ =0x0000077E
    b .L080294F2
    .align 2, 0
.L08029460: .4byte 0x0000077E
.L08029464:
    movs r5, #0xe6
    ldr r1, .L0802946C @ =0x00000782
    b .L080294F2
    .align 2, 0
.L0802946C: .4byte 0x00000782
.L08029470:
    movs r5, #0xad
    lsls r5, r5, #1
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08029488 @ =0x0000077A
    b .L080294F2
    .align 2, 0
.L08029488: .4byte 0x0000077A
.L0802948C:
    movs r5, #0x9a
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L080294A0 @ =0x0000077A
    b .L080294F2
    .align 2, 0
.L080294A0: .4byte 0x0000077A
.L080294A4:
    movs r5, #0x83
    lsls r5, r5, #1
    b .L08029544
.L080294AA:
    movs r5, #0x81
    lsls r5, r5, #1
    b .L08029544
.L080294B0:
    movs r5, #0xfe
    b .L08029544
.L080294B4:
    movs r5, #0xfa
    b .L08029544
.L080294B8:
    movs r5, #0x91
    lsls r5, r5, #1
    b .L08029544
.L080294BE:
    movs r5, #0x8f
    lsls r5, r5, #1
    b .L08029544
.L080294C4:
    movs r5, #0x8d
    lsls r5, r5, #1
    b .L08029544
.L080294CA:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xa5
    lsls r5, r5, #1
    cmp r0, #0
    beq .L08029544
    subs r5, #0x40
    b .L08029544
.L080294E0:
    movs r5, #0xe2
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08029500 @ =0x00000792
.L080294F2:
    ldrh r0, [r2, #0x22]
    cmp r0, r1
    beq .L08029544
    adds r0, r2, #0
    bl SetAnim__12AActorEntityUi
    b .L08029544
    .align 2, 0
.L08029500: .4byte 0x00000792
.L08029504:
    movs r5, #0x3e
    b .L08029544
.L08029508:
    movs r5, #0xa3
    lsls r5, r5, #1
    b .L08029544
.L0802950E:
    movs r5, #0xa9
    lsls r5, r5, #1
    b .L08029544
.L08029514:
    movs r5, #0x9d
    lsls r5, r5, #1
    b .L08029544
.L0802951A:
    movs r5, #0x42
    b .L08029544
.L0802951E:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xe3
    lsls r5, r5, #1
    cmp r0, #0
    bne .L08029544
.L08029530:
    movs r5, #0xab
    lsls r5, r5, #1
    b .L08029544
.L08029536:
    adds r0, r6, #0
    adds r0, #0x3d
    ldrb r0, [r0]
    movs r5, #0xf6
    cmp r0, #0
    beq .L08029544
    movs r5, #0xf2
.L08029544:
    lsls r1, r5, #0x10
    lsrs r1, r1, #0x10
    adds r0, r6, #0
    bl SetAnim__12AActorEntityUi
    movs r0, #1
.L08029550:
    add sp, #8
    pop {r4, r5, r6, r7}
    pop {r1}
    bx r1
