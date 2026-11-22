    .INCLUDE "asm/macro.inc"
    .SYNTAX UNIFIED

    .section .text.npc_extra.08035380, "ax", %progbits

    thumb_func_start func_08035380
func_08035380: @ 0x08035380
    push {r4, r5, r6, r7, lr}
    mov r7, sl
    mov r6, sb
    mov r5, r8
    push {r5, r6, r7}
    sub sp, #0xbc
    adds r6, r0, #0
    ldr r5, [r6]
    ldr r1, [r6, #0x38]
    cmp r1, #0
    bne .L08035398
    b .L080358F8
.L08035398:
    adds r0, #0x3c
    ldrb r0, [r0]
    ldr r1, [r1, #8]
    lsls r0, r0, #2
    adds r0, r0, r1
    ldr r0, [r0]
    str r0, [sp, #0x70]
    cmp r0, #0
    bne .L080353AC
    b .L080358F8
.L080353AC:
    adds r0, r6, #0
    adds r0, #0x3d
    ldrb r1, [r0]
    str r0, [sp, #0xb0]
    ldr r0, [sp, #0x70]
    ldrh r0, [r0]
    cmp r1, r0
    blo .L080353BE
    b .L080358F8
.L080353BE:
    lsls r1, r1, #3
    ldr r2, [sp, #0x70]
    ldr r0, [r2, #4]
    adds r7, r0, r1
    ldr r3, [r7, #4]
    str r3, [sp, #0x74]
    cmp r3, #0
    bne .L080353D0
    b .L080358F8
.L080353D0:
    ldr r0, [r5]
    movs r4, #0xa2
    lsls r4, r4, #1
    adds r0, r0, r4
    ldr r1, [r0]
    adds r0, r5, #0
    bl _call_via_r1
    ldrh r3, [r0, #0xa]
    lsls r0, r3, #0x1b
    lsrs r0, r0, #0x1b
    subs r2, r0, #6
    cmp r2, #0
    bge .L080353EE
    adds r2, #0x18
.L080353EE:
    lsls r1, r2, #4
    subs r1, r1, r2
    lsls r1, r1, #2
    lsls r0, r3, #0x15
    lsrs r0, r0, #0x1a
    adds r1, r1, r0
    movs r0, #0
    str r0, [sp, #0x78]
    ldr r2, [sp, #0x74]
    ldr r3, [r2]
    cmp r3, #0
    bne .L08035408
    b .L080358C0
.L08035408:
    ldrh r4, [r2, #4]
    cmp r4, #0
    bne .L08035410
    b .L080358C0
.L08035410:
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r2, [r0]
    str r0, [sp, #0xb4]
    cmp r2, r4
    blo .L0803541E
    b .L080358F8
.L0803541E:
    ldr r4, [sp, #0x74]
    ldrb r0, [r4, #0xb]
    lsls r0, r0, #0x1b
    cmp r0, #0
    bge .L0803542A
    b .L08035638
.L0803542A:
    lsls r0, r2, #3
    adds r3, r3, r0
    str r3, [sp, #0x7c]
    movs r1, #0
    ldrb r0, [r3, #4]
    lsls r0, r0, #0x1f
    cmp r0, #0
    bne .L0803543C
    movs r1, #1
.L0803543C:
    mov r8, r1
    ldr r0, [r6, #8]
    str r0, [sp]
    ldr r0, [r6, #0xc]
    str r0, [sp, #8]
    ldr r0, [r6, #0x18]
    str r0, [sp, #4]
    ldr r0, [r6, #0x1c]
    str r0, [sp, #0xc]
    cmp r1, #0
    beq .L0803545C
    mov sl, sp
    mov r7, sp
    adds r7, #4
    str r7, [sp, #0x80]
    b .L08035466
.L0803545C:
    add r0, sp, #8
    mov sl, r0
    mov r1, sp
    adds r1, #0xc
    str r1, [sp, #0x80]
.L08035466:
    mov r2, sl
    ldr r1, [r2]
    asrs r3, r1, #0x10
    str r3, [sp, #0x84]
    ldr r4, [sp, #0x80]
    ldr r0, [r4]
    adds r1, r1, r0
    str r1, [sp, #0x88]
    asrs r7, r1, #0x10
    str r7, [sp, #0x8c]
    subs r7, r7, r3
    movs r3, #0
    adds r0, r6, #0
    adds r0, #0x40
    ldrh r2, [r0]
    ldr r1, .L080354AC @ =0x000002EE
    str r0, [sp, #0xb8]
    cmp r2, r1
    blt .L0803548E
    movs r3, #1
.L0803548E:
    movs r0, #1
    str r0, [sp, #0x90]
    adds r0, r6, #0
    adds r0, #0x3f
    ldrb r2, [r0]
    adds r1, r0, #0
    cmp r2, #0
    beq .L080354BE
    movs r4, #0
    str r4, [sp, #0x90]
    cmp r3, #0
    beq .L080354B0
    strb r4, [r1]
    b .L080354B4
    .align 2, 0
.L080354AC: .4byte 0x000002EE
.L080354B0:
    subs r0, r2, #1
    strb r0, [r1]
.L080354B4:
    ldrb r0, [r1]
    cmp r0, #0
    bne .L080354BE
    movs r1, #1
    str r1, [sp, #0x78]
.L080354BE:
    cmp r3, #0
    bne .L080355A0
    cmp r7, #0
    beq .L080355A0
    ldr r0, [r5]
    ldr r2, [r0, #0x40]
    adds r0, r5, #0
    movs r1, #0
    bl _call_via_r2
    adds r4, r0, #0
    cmp r4, #0
    beq .L080355A0
    ldrh r0, [r4, #4]
    ldrh r2, [r6, #4]
    cmp r0, r2
    bne .L080355A0
    ldr r1, [r6, #0x14]
    add r0, sp, #0x10
    ldr r2, [r1, #0xc]
    adds r1, r6, #0
    bl _call_via_r2
    ldr r0, [sp, #0x10]
    ldr r1, [sp, #0x14]
    str r0, [sp, #0x18]
    str r1, [sp, #0x1c]
    add r5, sp, #0x18
    ldr r1, [r4, #0x14]
    add r0, sp, #0x20
    ldr r2, [r1, #0xc]
    adds r1, r4, #0
    bl _call_via_r2
    ldr r0, [sp, #0x20]
    ldr r1, [sp, #0x24]
    str r0, [sp, #0x28]
    str r1, [sp, #0x2c]
    add r4, sp, #0x28
    mov r3, r8
    cmp r3, #0
    beq .L08035518
    adds r2, r7, #0
    movs r7, #0
    b .L0803551A
.L08035518:
    movs r2, #0
.L0803551A:
    mov sb, r7
    movs r1, #0
    ldrsh r0, [r5, r1]
    mov ip, r0
    movs r3, #4
    ldrsh r1, [r4, r3]
    subs r1, r0, r1
    movs r7, #0
    ldrsh r0, [r4, r7]
    movs r7, #4
    ldrsh r3, [r5, r7]
    mov r8, r3
    subs r0, r0, r3
    ands r1, r0
    movs r0, #2
    ldrsh r7, [r5, r0]
    movs r3, #6
    ldrsh r0, [r4, r3]
    subs r0, r7, r0
    ands r1, r0
    movs r3, #2
    ldrsh r0, [r4, r3]
    movs r3, #6
    ldrsh r5, [r5, r3]
    subs r0, r0, r5
    ands r1, r0
    cmp r1, #0
    blt .L080355A0
    add r3, sp, #0x30
    mov r1, ip
    adds r0, r2, r1
    strh r0, [r3]
    mov r1, sb
    adds r0, r1, r7
    strh r0, [r3, #2]
    mov r7, r8
    adds r0, r2, r7
    strh r0, [r3, #4]
    adds r0, r1, r5
    strh r0, [r3, #6]
    movs r0, #0
    ldrsh r1, [r3, r0]
    movs r2, #4
    ldrsh r0, [r4, r2]
    subs r1, r1, r0
    movs r5, #0
    ldrsh r0, [r4, r5]
    movs r7, #4
    ldrsh r2, [r3, r7]
    subs r0, r0, r2
    ands r1, r0
    movs r2, #2
    ldrsh r0, [r3, r2]
    movs r5, #6
    ldrsh r2, [r4, r5]
    subs r0, r0, r2
    ands r1, r0
    movs r7, #2
    ldrsh r0, [r4, r7]
    movs r4, #6
    ldrsh r2, [r3, r4]
    subs r0, r0, r2
    ands r1, r0
    cmp r1, #0
    bge .L080355A0
    movs r5, #0
    str r5, [sp, #0x90]
.L080355A0:
    ldr r7, [sp, #0x90]
    cmp r7, #0
    beq .L0803562E
    ldr r1, [sp, #0x7c]
    movs r2, #2
    ldrsh r0, [r1, r2]
    ldr r3, [sp, #0x84]
    cmp r3, r0
    beq .L080355D0
    ldr r4, [sp, #0x8c]
    cmp r4, r0
    beq .L080355D0
    movs r1, #0
    cmp r0, r3
    bge .L080355C0
    movs r1, #1
.L080355C0:
    ldr r5, [sp, #0x8c]
    cmp r0, r5
    bge .L080355CC
    cmp r1, #0
    beq .L080355D0
    b .L08035616
.L080355CC:
    cmp r1, #0
    beq .L08035616
.L080355D0:
    lsls r0, r0, #0x10
    mov r7, sl
    str r0, [r7]
    movs r1, #0
    ldr r0, [sp, #0x80]
    str r1, [r0]
    ldr r2, [sp, #0xb4]
    ldrb r0, [r2]
    adds r0, #1
    strb r0, [r2]
    lsls r0, r0, #0x18
    lsrs r0, r0, #0x18
    ldr r3, [sp, #0x74]
    ldrh r3, [r3, #4]
    cmp r0, r3
    bhs .L080355F6
    movs r4, #1
    str r4, [sp, #0x78]
    b .L0803561C
.L080355F6:
    ldr r5, [sp, #0xb0]
    ldrb r0, [r5]
    adds r0, #1
    ldr r7, [sp, #0x70]
    ldrh r7, [r7]
    cmp r0, r7
    blt .L08035606
    b .L080358F8
.L08035606:
    ldr r0, [sp, #0xb4]
    strb r1, [r0]
    ldrb r0, [r5]
    adds r0, #1
    strb r0, [r5]
    movs r1, #2
    str r1, [sp, #0x78]
    b .L0803561C
.L08035616:
    ldr r2, [sp, #0x88]
    mov r3, sl
    str r2, [r3]
.L0803561C:
    ldr r0, [sp]
    str r0, [r6, #8]
    ldr r0, [sp, #8]
    str r0, [r6, #0xc]
    ldr r0, [sp, #4]
    str r0, [r6, #0x18]
    ldr r0, [sp, #0xc]
    str r0, [r6, #0x1c]
    b .L080358E6
.L0803562E:
    ldr r4, [sp, #0xb8]
    ldrh r0, [r4]
    adds r0, #1
    strh r0, [r4]
    b .L080358E6
.L08035638:
    lsls r0, r2, #3
    adds r3, r3, r0
    str r3, [sp, #0x94]
    movs r2, #0
    ldrb r0, [r3, #6]
    lsls r0, r0, #0x1d
    cmp r0, #0
    blt .L0803564A
    movs r2, #1
.L0803564A:
    mov r8, r2
    movs r2, #0
    ldr r3, [sp, #0xb0]
    ldrb r0, [r3]
    adds r0, #1
    ldr r4, [sp, #0x70]
    ldrh r4, [r4]
    cmp r0, r4
    bge .L08035664
    ldrh r7, [r7, #8]
    cmp r1, r7
    blo .L08035664
    movs r2, #1
.L08035664:
    mov sl, r2
    cmp r2, #0
    beq .L08035682
    ldrh r4, [r6, #4]
    ldr r0, [r5]
    ldr r1, [r0, #0x14]
    adds r0, r5, #0
    bl _call_via_r1
    cmp r4, r0
    beq .L08035682
    movs r0, #0
    ldr r5, [sp, #0xb4]
    strb r0, [r5]
    b .L080358AC
.L08035682:
    adds r1, r6, #0
    adds r1, #0x40
    ldrh r0, [r1]
    str r1, [sp, #0xb8]
    cmp r0, #0
    beq .L08035690
    b .L08035874
.L08035690:
    ldr r0, [r6, #8]
    str r0, [sp, #0x38]
    ldr r0, [r6, #0xc]
    str r0, [sp, #0x40]
    ldr r0, [r6, #0x18]
    str r0, [sp, #0x3c]
    ldr r0, [r6, #0x1c]
    str r0, [sp, #0x44]
    mov r1, r8
    cmp r1, #0
    beq .L080356B4
    mov r2, sp
    adds r2, #0x38
    str r2, [sp, #0x98]
    mov r3, sp
    adds r3, #0x3c
    str r3, [sp, #0x9c]
    b .L080356C0
.L080356B4:
    mov r4, sp
    adds r4, #0x40
    str r4, [sp, #0x98]
    mov r7, sp
    adds r7, #0x44
    str r7, [sp, #0x9c]
.L080356C0:
    ldr r1, [sp, #0x98]
    ldr r0, [r1]
    asrs r2, r0, #0x10
    str r2, [sp, #0xa0]
    ldr r3, [sp, #0x9c]
    ldr r1, [r3]
    adds r0, r0, r1
    str r0, [sp, #0xa4]
    asrs r4, r0, #0x10
    str r4, [sp, #0xa8]
    subs r7, r4, r2
    movs r0, #1
    str r0, [sp, #0xac]
    adds r0, r6, #0
    adds r0, #0x3f
    ldrb r2, [r0]
    adds r1, r0, #0
    cmp r2, #0
    beq .L08035702
    movs r3, #0
    str r3, [sp, #0xac]
    mov r4, sl
    cmp r4, #0
    beq .L080356F4
    strb r3, [r1]
    b .L080356F8
.L080356F4:
    subs r0, r2, #1
    strb r0, [r1]
.L080356F8:
    ldrb r0, [r1]
    cmp r0, #0
    bne .L08035702
    movs r1, #1
    str r1, [sp, #0x78]
.L08035702:
    mov r2, sl
    cmp r2, #0
    bne .L080357E8
    cmp r7, #0
    beq .L080357E8
    ldr r0, [r5]
    ldr r2, [r0, #0x40]
    adds r0, r5, #0
    movs r1, #0
    bl _call_via_r2
    adds r4, r0, #0
    cmp r4, #0
    beq .L080357E8
    ldrh r0, [r4, #4]
    ldrh r3, [r6, #4]
    cmp r0, r3
    bne .L080357E8
    ldr r1, [r6, #0x14]
    add r0, sp, #0x48
    ldr r2, [r1, #0xc]
    adds r1, r6, #0
    bl _call_via_r2
    ldr r0, [sp, #0x48]
    ldr r1, [sp, #0x4c]
    str r0, [sp, #0x50]
    str r1, [sp, #0x54]
    add r5, sp, #0x50
    ldr r1, [r4, #0x14]
    add r0, sp, #0x58
    ldr r2, [r1, #0xc]
    adds r1, r4, #0
    bl _call_via_r2
    ldr r0, [sp, #0x58]
    ldr r1, [sp, #0x5c]
    str r0, [sp, #0x60]
    str r1, [sp, #0x64]
    add r4, sp, #0x60
    mov r0, r8
    cmp r0, #0
    beq .L08035760
    adds r2, r7, #0
    movs r1, #0
    mov sb, r1
    b .L08035764
.L08035760:
    movs r2, #0
    mov sb, r7
.L08035764:
    movs r7, #0
    ldrsh r3, [r5, r7]
    mov r8, r3
    movs r0, #4
    ldrsh r1, [r4, r0]
    subs r1, r3, r1
    movs r3, #0
    ldrsh r0, [r4, r3]
    movs r3, #4
    ldrsh r7, [r5, r3]
    mov ip, r7
    subs r0, r0, r7
    ands r1, r0
    movs r0, #2
    ldrsh r7, [r5, r0]
    movs r3, #6
    ldrsh r0, [r4, r3]
    subs r0, r7, r0
    ands r1, r0
    movs r3, #2
    ldrsh r0, [r4, r3]
    movs r3, #6
    ldrsh r5, [r5, r3]
    subs r0, r0, r5
    ands r1, r0
    cmp r1, #0
    blt .L080357E8
    add r3, sp, #0x68
    mov r1, r8
    adds r0, r2, r1
    strh r0, [r3]
    mov r1, sb
    adds r0, r1, r7
    strh r0, [r3, #2]
    mov r7, ip
    adds r0, r2, r7
    strh r0, [r3, #4]
    adds r0, r1, r5
    strh r0, [r3, #6]
    movs r0, #0
    ldrsh r1, [r3, r0]
    movs r2, #4
    ldrsh r0, [r4, r2]
    subs r1, r1, r0
    movs r5, #0
    ldrsh r0, [r4, r5]
    movs r7, #4
    ldrsh r2, [r3, r7]
    subs r0, r0, r2
    ands r1, r0
    movs r2, #2
    ldrsh r0, [r3, r2]
    movs r5, #6
    ldrsh r2, [r4, r5]
    subs r0, r0, r2
    ands r1, r0
    movs r7, #2
    ldrsh r0, [r4, r7]
    movs r4, #6
    ldrsh r2, [r3, r4]
    subs r0, r0, r2
    ands r1, r0
    cmp r1, #0
    bge .L080357E8
    movs r5, #0
    str r5, [sp, #0xac]
.L080357E8:
    ldr r7, [sp, #0xac]
    cmp r7, #0
    beq .L080358E6
    ldr r0, [sp, #0x94]
    movs r1, #0
    ldrsh r2, [r0, r1]
    ldr r3, [sp, #0xa8]
    cmp r3, r2
    beq .L08035806
    ldr r4, [sp, #0xa0]
    subs r0, r2, r4
    subs r1, r2, r3
    eors r0, r1
    cmp r0, #0
    bge .L0803585C
.L08035806:
    lsls r0, r2, #0x10
    ldr r5, [sp, #0x98]
    str r0, [r5]
    movs r1, #0
    ldr r7, [sp, #0x9c]
    str r1, [r7]
    mov r0, sl
    cmp r0, #0
    bne .L08035820
    ldr r2, [sp, #0x94]
    ldrh r0, [r2, #4]
    cmp r0, #0
    bne .L08035850
.L08035820:
    ldr r3, [sp, #0xb4]
    ldrb r0, [r3]
    adds r0, #1
    strb r0, [r3]
    lsls r0, r0, #0x18
    lsrs r0, r0, #0x18
    ldr r4, [sp, #0x74]
    ldrh r4, [r4, #4]
    cmp r0, r4
    blo .L0803584A
    strb r1, [r3]
    mov r5, sl
    cmp r5, #0
    beq .L08035844
    ldr r7, [sp, #0xb0]
    ldrb r0, [r7]
    adds r0, #1
    strb r0, [r7]
.L08035844:
    movs r0, #2
    str r0, [sp, #0x78]
    b .L08035862
.L0803584A:
    movs r1, #1
    str r1, [sp, #0x78]
    b .L08035862
.L08035850:
    movs r0, #1
    ldr r2, [sp, #0xb8]
    strh r0, [r2]
    movs r3, #1
    str r3, [sp, #0x78]
    b .L08035862
.L0803585C:
    ldr r4, [sp, #0xa4]
    ldr r5, [sp, #0x98]
    str r4, [r5]
.L08035862:
    ldr r0, [sp, #0x38]
    str r0, [r6, #8]
    ldr r0, [sp, #0x40]
    str r0, [r6, #0xc]
    ldr r0, [sp, #0x3c]
    str r0, [r6, #0x18]
    ldr r0, [sp, #0x44]
    str r0, [r6, #0x1c]
    b .L080358E6
.L08035874:
    mov r7, sl
    cmp r7, #0
    bne .L0803588A
    adds r0, #1
    strh r0, [r1]
    lsls r0, r0, #0x10
    lsrs r0, r0, #0x10
    ldr r2, [sp, #0x94]
    ldrh r2, [r2, #4]
    cmp r0, r2
    bls .L080358E6
.L0803588A:
    movs r0, #0
    strh r0, [r1]
    ldr r3, [sp, #0xb4]
    ldrb r0, [r3]
    adds r0, #1
    strb r0, [r3]
    lsls r0, r0, #0x18
    lsrs r0, r0, #0x18
    ldr r4, [sp, #0x74]
    ldrh r4, [r4, #4]
    cmp r0, r4
    blo .L080358BA
    movs r0, #0
    strb r0, [r3]
    mov r5, sl
    cmp r5, #0
    beq .L080358B4
.L080358AC:
    ldr r7, [sp, #0xb0]
    ldrb r0, [r7]
    adds r0, #1
    strb r0, [r7]
.L080358B4:
    movs r0, #2
    str r0, [sp, #0x78]
    b .L080358EC
.L080358BA:
    movs r1, #1
    str r1, [sp, #0x78]
    b .L080358EC
.L080358C0:
    ldr r2, [sp, #0xb0]
    ldrb r0, [r2]
    adds r0, #1
    ldr r3, [sp, #0x70]
    ldrh r3, [r3]
    cmp r0, r3
    bge .L080358E6
    ldrh r7, [r7, #8]
    cmp r1, r7
    blo .L080358E6
    adds r1, r6, #0
    adds r1, #0x3e
    movs r0, #0
    strb r0, [r1]
    ldrb r0, [r2]
    adds r0, #1
    strb r0, [r2]
    movs r4, #2
    str r4, [sp, #0x78]
.L080358E6:
    ldr r5, [sp, #0x78]
    cmp r5, #0
    ble .L080358F8
.L080358EC:
    ldr r0, [r6, #0x14]
    ldr r2, [r0, #0x3c]
    adds r0, r6, #0
    ldr r1, [sp, #0x78]
    bl _call_via_r2
.L080358F8:
    add sp, #0xbc
    pop {r3, r4, r5}
    mov r8, r3
    mov sb, r4
    mov sl, r5
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0

    .section .text.npc_extra.08035978, "ax", %progbits

    .section .text.npc_extra.08035AFC, "ax", %progbits

    .section .text.npc_extra.08035B64, "ax", %progbits

    .section .text.npc_extra.08035BC8, "ax", %progbits

    .section .text.npc_extra.08035C04, "ax", %progbits

    .section .text.npc_extra.08035D04, "ax", %progbits

    .section .text.npc_extra.08035D70, "ax", %progbits

    .section .text.npc_extra.08035DD8, "ax", %progbits

    .section .text.npc_extra.08035E40, "ax", %progbits

    .section .text.npc_extra.08035EAC, "ax", %progbits

    .section .text.npc_extra.08035F10, "ax", %progbits

    .section .text.npc_extra.08035F74, "ax", %progbits

    .section .text.npc_extra.08035FE0, "ax", %progbits

    .section .text.npc_extra.08036048, "ax", %progbits

    .section .text.npc_extra.08036084, "ax", %progbits

    .section .text.npc_extra.08036184, "ax", %progbits

    .section .text.npc_extra.080361EC, "ax", %progbits

    .section .text.npc_extra.08036258, "ax", %progbits

    .section .text.npc_extra.080362C0, "ax", %progbits

    .section .text.npc_extra.08036328, "ax", %progbits

    .section .text.npc_extra.08036390, "ax", %progbits

    .section .text.npc_extra.080363F8, "ax", %progbits

    .section .text.npc_extra.08036430, "ax", %progbits

    .section .text.npc_extra.08036530, "ax", %progbits

    .section .text.npc_extra.08036590, "ax", %progbits

    .section .text.npc_extra.080365CC, "ax", %progbits

    .section .text.npc_extra.080366CC, "ax", %progbits

    .section .text.npc_extra.08036730, "ax", %progbits

    .section .text.npc_extra.08036768, "ax", %progbits

    .section .text.npc_extra.080367F8, "ax", %progbits

    .section .text.npc_extra.08036860, "ax", %progbits

    .section .text.npc_extra.080368C4, "ax", %progbits

    .section .text.npc_extra.08036900, "ax", %progbits

    .section .text.npc_extra.08036A10, "ax", %progbits

    .section .text.npc_extra.08036A78, "ax", %progbits

    .section .text.npc_extra.08036B4C, "ax", %progbits

    .section .text.npc_extra.08036BB0, "ax", %progbits

    .section .text.npc_extra.08036BF8, "ax", %progbits

    .section .text.npc_extra.08036CAC, "ax", %progbits

    .section .text.npc_extra.08036D0C, "ax", %progbits

    .section .text.npc_extra.08036D68, "ax", %progbits

    .section .text.npc_extra.08036DC4, "ax", %progbits

    .section .text.npc_extra.08036E00, "ax", %progbits
    .section .text.npc_extra.08036E70, "ax", %progbits

    .section .text.npc_extra.08036F0C, "ax", %progbits

    .section .text.npc_extra.08037008, "ax", %progbits

    .section .text.npc_extra.08037048, "ax", %progbits

    .section .text.npc_extra.080370D8, "ax", %progbits
    .section .text.npc_extra.08037118, "ax", %progbits


    .section .text.npc_extra.08037250, "ax", %progbits

    .section .text.npc_extra.0803726C, "ax", %progbits


    .section .text.npc_extra.08037434, "ax", %progbits

    .section .text.npc_extra.08037470, "ax", %progbits

    .section .text.npc_extra.08037494, "ax", %progbits

    .section .text.npc_extra.08037568, "ax", %progbits

    .section .text.npc_extra.0803763C, "ax", %progbits

    .section .text.npc_extra.08037714, "ax", %progbits

    .section .text.npc_extra.080377E8, "ax", %progbits

    .section .text.npc_extra.08037958, "ax", %progbits

    .section .text.npc_extra.080379D8, "ax", %progbits

    .section .text.npc_extra.08037A5C, "ax", %progbits


    .section .text.npc_extra.08037CDC, "ax", %progbits
    .section .text.npc_extra.08037D08, "ax", %progbits

    .section .text.npc_extra.08037E48, "ax", %progbits

    .section .text.npc_extra.08037EF0, "ax", %progbits

    .section .text.npc_extra.08038110, "ax", %progbits

    thumb_func_start func_08038110
func_08038110: @ 0x08038110
    push {r4, r5, r6, r7, lr}
    mov r7, r8
    push {r7}
    sub sp, #0x44
    adds r6, r0, #0
    adds r4, r6, #0
    adds r4, #0x25
    ldrb r0, [r4]
    cmp r0, #0
    bne .L08038126
    b .L080382F4
.L08038126:
    ldr r7, [r6]
    ldrh r2, [r6, #4]
    ldr r1, [r7]
    mov r0, sp
    ldr r3, [r1, #0x34]
    adds r1, r7, #0
    bl _call_via_r3
    mov r8, sp
    ldr r1, [r6, #0x20]
    ldr r0, .L080381A0 @ =0xFFFFB85D
    adds r1, r1, r0
    str r1, [r6, #0x20]
    ldr r0, [r6, #0x1c]
    adds r0, r0, r1
    str r0, [r6, #0x1c]
    cmp r0, #0
    bgt .L0803814C
    b .L0803825C
.L0803814C:
    ldr r1, [r6, #0x14]
    add r0, sp, #0x34
    ldr r2, [r1, #0xc]
    adds r1, r6, #0
    bl _call_via_r2
    ldr r1, [sp, #0x34]
    ldr r2, [sp, #0x38]
    str r1, [sp, #0x3c]
    str r2, [sp, #0x40]
    add r5, sp, #0xc
    movs r2, #0x20
    mov r8, r2
    movs r1, #0x21
    str r1, [sp, #0xc]
    movs r0, #0x21
    rsbs r0, r0, #0
    str r0, [r5, #4]
    str r0, [r5, #8]
    str r1, [r5, #0xc]
    add r1, sp, #0x1c
    mov r0, sp
    ldm r0!, {r2, r3, r4}
    stm r1!, {r2, r3, r4}
    ldr r3, [sp, #0x3c]
    ldr r4, [sp, #0x40]
    str r3, [sp, #0x28]
    str r4, [sp, #0x2c]
    mov r4, r8
    str r4, [r5, #0x24]
    ldr r4, [r6, #8]
    ldr r7, [r6, #0xc]
    adds r0, r6, #0
    adds r0, #0x24
    ldrb r0, [r0]
    cmp r0, #1
    beq .L080381AE
    cmp r0, #1
    bgt .L080381A4
    cmp r0, #0
    beq .L080381D4
    b .L08038250
    .align 2, 0
.L080381A0: .4byte 0xFFFFB85D
.L080381A4:
    cmp r0, #2
    beq .L08038230
    cmp r0, #3
    beq .L08038204
    b .L08038250
.L080381AE:
    ldr r1, [sp, #0x10]
    ldr r0, [sp, #0x30]
    cmn r1, r0
    bge .L080381C0
    adds r0, r5, #0
    bl ResetEntityMovementNegativeY
    ldr r0, [sp, #0x10]
    b .L080381C2
.L080381C0:
    adds r0, r1, #0
.L080381C2:
    lsls r1, r0, #0x10
    ldr r0, .L080381D0 @ =0xFFFE38E4
    cmp r1, r0
    bgt .L080381F0
    ldr r5, .L080381D0 @ =0xFFFE38E4
    adds r7, r7, r5
    b .L08038250
    .align 2, 0
.L080381D0: .4byte 0xFFFE38E4
.L080381D4:
    ldr r1, [sp, #0xc]
    ldr r0, [sp, #0x30]
    cmp r1, r0
    ble .L080381E6
    adds r0, r5, #0
    bl ResetEntityMovementPositiveY
    ldr r0, [sp, #0xc]
    b .L080381E8
.L080381E6:
    adds r0, r1, #0
.L080381E8:
    lsls r1, r0, #0x10
    ldr r0, .L080381F4 @ =0x0001C71B
    cmp r1, r0
    bgt .L080381F8
.L080381F0:
    adds r7, r7, r1
    b .L08038250
    .align 2, 0
.L080381F4: .4byte 0x0001C71B
.L080381F8:
    ldr r0, .L08038200 @ =0x0001C71C
    adds r7, r7, r0
    b .L08038250
    .align 2, 0
.L08038200: .4byte 0x0001C71C
.L08038204:
    ldr r1, [sp, #0x18]
    ldr r0, [sp, #0x30]
    cmp r1, r0
    ble .L08038216
    adds r0, r5, #0
    bl ResetEntityMovementPositiveX
    ldr r0, [sp, #0x18]
    b .L08038218
.L08038216:
    adds r0, r1, #0
.L08038218:
    lsls r1, r0, #0x10
    ldr r0, .L08038228 @ =0x0001C71B
    cmp r1, r0
    ble .L08038222
    ldr r1, .L0803822C @ =0x0001C71C
.L08038222:
    adds r4, r4, r1
    b .L08038250
    .align 2, 0
.L08038228: .4byte 0x0001C71B
.L0803822C: .4byte 0x0001C71C
.L08038230:
    ldr r1, [sp, #0x14]
    ldr r0, [sp, #0x30]
    cmn r1, r0
    bge .L08038242
    adds r0, r5, #0
    bl ResetEntityMovementNegativeX
    ldr r0, [sp, #0x14]
    b .L08038244
.L08038242:
    adds r0, r1, #0
.L08038244:
    lsls r1, r0, #0x10
    ldr r0, .L08038258 @ =0xFFFE38E4
    cmp r1, r0
    bgt .L08038222
    ldr r2, .L08038258 @ =0xFFFE38E4
    adds r4, r4, r2
.L08038250:
    str r4, [r6, #8]
    str r7, [r6, #0xc]
    b .L080382F4
    .align 2, 0
.L08038258: .4byte 0xFFFE38E4
.L0803825C:
    movs r0, #0
    str r0, [r6, #0x1c]
    strb r0, [r4]
    add r4, sp, #0xc
    adds r0, r4, #0
    adds r1, r6, #0
    bl GetLocation__C7AEntity
    ldr r0, [r6, #0x18]
    adds r1, r4, #0
    movs r2, #6
    bl memcpy
    ldr r0, [r6, #8]
    ldr r1, [r6, #0xc]
    asrs r0, r0, #0x13
    mov ip, r0
    asrs r3, r1, #0x13
    ldr r1, [sp]
    mov r5, r8
    ldrh r0, [r5, #8]
    cmp r1, #0
    beq .L080382A8
    cmp ip, r0
    bhi .L080382A8
    ldrh r2, [r5, #0xa]
    cmp r3, r2
    bhi .L080382A8
    ldr r2, [sp, #4]
    muls r0, r3, r0
    add r0, ip
    cmp r2, #0
    beq .L080382A2
    adds r0, r2, r0
    ldrb r0, [r0]
.L080382A2:
    lsls r0, r0, #2
    adds r0, r1, r0
    b .L080382AA
.L080382A8:
    movs r0, #0
.L080382AA:
    cmp r0, #0
    bne .L080382C2
    ldr r0, [r7]
    movs r3, #0x9e
    lsls r3, r3, #1
    adds r0, r0, r3
    ldr r2, [r0]
    adds r0, r7, #0
    movs r1, #1
    bl _call_via_r2
    b .L080382F4
.L080382C2:
    ldr r0, [r0]
    lsls r0, r0, #0x1e
    cmp r0, #0
    bge .L080382DE
    ldr r0, [r7]
    movs r4, #0x9e
    lsls r4, r4, #1
    adds r0, r0, r4
    ldr r2, [r0]
    adds r0, r7, #0
    movs r1, #0
    bl _call_via_r2
    b .L080382F4
.L080382DE:
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2b
    bl _call_via_r2
    cmp r0, #0
    beq .L080382F4
    adds r1, r4, #0
    bl DogEntity_PlayIfNearby
.L080382F4:
    add sp, #0x44
    pop {r3}
    mov r8, r3
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0

    .section .text.npc_extra.08038374, "ax", %progbits

    .section .text.npc_extra.08038398, "ax", %progbits

    .section .text.npc_extra.0803853C, "ax", %progbits

    .section .text.npc_extra.080385B0, "ax", %progbits

    thumb_func_start func_080385B0
func_080385B0: @ 0x080385B0
    push {r4, r5, r6, r7, lr}
    mov r7, sl
    mov r6, sb
    mov r5, r8
    push {r5, r6, r7}
    sub sp, #0x50
    adds r5, r0, #0
    mov sl, r1
    ldr r0, [r5]
    mov sb, r0
    movs r1, #0xa
    ldrsh r6, [r0, r1]
    mov r2, sl
    movs r3, #0xc
    ldrsh r0, [r2, r3]
    subs r0, r6, r0
    str r0, [sp, #0x3c]
    mov r0, sb
    movs r1, #0xe
    ldrsh r4, [r0, r1]
    movs r3, #0xe
    ldrsh r0, [r2, r3]
    subs r0, r4, r0
    str r0, [sp, #0x40]
    mov r0, sb
    adds r0, #0x26
    ldrb r0, [r0]
    str r0, [sp, #0x44]
    movs r0, #0x80
    lsls r0, r0, #8
    subs r0, r0, r4
    mov r8, r0
    ldr r0, [sp, #0x44]
    cmp r0, #0
    beq .L080385FC
    movs r1, #1
    rsbs r1, r1, #0
    add r8, r1
.L080385FC:
    add r0, sp, #0x34
    mov r2, sl
    ldr r1, [r2]
    ldr r2, [r2, #8]
    str r1, [sp, #0x34]
    str r2, [r0, #4]
    ldr r3, [sp, #0x40]
    str r3, [sp, #0x48]
    movs r7, #2
    ldr r0, [sp, #0x44]
    cmp r0, #0
    bne .L08038646
    mov r0, sb
    bl GetSideSpriteHeight
    ldr r1, [sp, #0x40]
    subs r1, r1, r0
    str r1, [sp, #0x48]
    cmp r0, #0xf
    ble .L08038646
    mov r2, sb
    ldrh r0, [r2, #4]
    cmp r0, #8
    bgt .L08038644
    cmp r0, #2
    bne .L08038646
    ldr r3, .L080386AC @ =0xFFFFFEF0
    adds r1, r6, r3
    ldr r0, .L080386B0 @ =0x000002AF
    cmp r1, r0
    bhi .L08038646
    cmp r4, #0xaf
    ble .L08038646
    subs r0, #0x60
    cmp r4, r0
    bgt .L08038646
.L08038644:
    movs r7, #1
.L08038646:
    adds r6, r5, #0
    adds r6, #8
    mov r0, sp
    adds r0, #0x34
    str r0, [sp, #0x4c]
    adds r0, r7, #0
    lsls r4, r0, #2
    orrs r4, r0
    lsls r1, r0, #4
    orrs r4, r1
    lsls r0, r0, #6
    orrs r4, r0
    ldr r1, [r5, #0x30]
    ldr r3, [r1]
    ldrh r2, [r5, #0x3c]
    lsls r2, r2, #2
    ldr r0, [r5, #0x34]
    adds r0, r0, r2
    ldrh r2, [r0]
    add r0, sp, #0x14
    ldr r3, [r3, #0x10]
    bl _call_via_r3
    add r7, sp, #0x14
    ldr r1, [sp, #0x4c]
    ldr r3, [r1, #4]
    ldr r2, [r5, #8]
    ldr r1, [r5, #0x10]
    rsbs r0, r1, #0
    orrs r0, r1
    cmp r0, #0
    bge .L080386B8
    ldrh r0, [r6, #0xc]
    adds r1, r5, #0
    adds r1, #0x18
    mov r5, r8
    str r5, [sp]
    adds r5, r7, #0
    str r5, [sp, #4]
    str r2, [sp, #8]
    str r0, [sp, #0xc]
    str r1, [sp, #0x10]
    adds r0, r3, #0
    ldr r1, [sp, #0x3c]
    ldr r2, [sp, #0x48]
    adds r3, r4, #0
    ldr r4, .L080386B4 @ =func_030004DC
    bl _call_via_r4
    b .L080386BA
    .align 2, 0
.L080386AC: .4byte 0xFFFFFEF0
.L080386B0: .4byte 0x000002AF
.L080386B4: .4byte func_030004DC
.L080386B8:
    movs r0, #0
.L080386BA:
    cmp r0, #0
    beq .L0803870E
    adds r0, r6, #0
    adds r0, #0x3c
    ldrb r1, [r0]
    adds r5, r0, #0
    cmp r1, #0
    beq .L0803870E
    ldr r0, [sp, #0x4c]
    ldr r1, [r0]
    adds r2, r7, #0
    adds r2, #8
    adds r0, r6, #0
    bl func_080A480C
    adds r0, r6, #0
    adds r0, #0x3d
    ldrb r0, [r0]
    cmp r0, #0
    beq .L080386F0
    adds r1, r7, #0
    adds r1, #0x10
    adds r0, r6, #0
    movs r2, #1
    bl LoadMapSpriteResourcePalettes
    b .L0803870A
.L080386F0:
    adds r4, r6, #0
    adds r4, #0x3e
    ldrb r0, [r4]
    cmp r0, #0
    bne .L0803870A
    adds r1, r7, #0
    adds r1, #0x10
    adds r0, r6, #0
    movs r2, #1
    bl LoadMapSpriteResourcePalettes
    movs r0, #1
    strb r0, [r4]
.L0803870A:
    movs r0, #0
    strb r0, [r5]
.L0803870E:
    ldr r1, [sp, #0x44]
    cmp r1, #0
    bne .L08038730
    mov r2, sb
    ldr r0, [r2]
    ldr r1, [r0]
    ldr r1, [r1, #0x58]
    bl _call_via_r1
    mov r3, sl
    ldr r1, [r3, #8]
    ldr r5, [sp, #0x44]
    str r5, [sp]
    ldr r2, [sp, #0x3c]
    ldr r3, [sp, #0x40]
    bl DrawFixedSpriteOverlay
.L08038730:
    add sp, #0x50
    pop {r3, r4, r5}
    mov r8, r3
    mov sb, r4
    mov sl, r5
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0

    .section .text.npc_extra.0803880C, "ax", %progbits

    .section .text.npc_extra.08038EB8, "ax", %progbits

    .section .text.npc_extra.08038EE0, "ax", %progbits

    thumb_func_start func_08038EE0
func_08038EE0: @ 0x08038EE0
    push {r4, r5, r6, r7, lr}
    mov r7, sl
    mov r6, sb
    mov r5, r8
    push {r5, r6, r7}
    sub sp, #0x4c
    mov sb, r0
    movs r0, #0xa6
    lsls r0, r0, #1
    bl __builtin_new
    mov r2, sb
    ldr r1, [r2]
    adds r5, r0, #0
    ldr r6, [r1]
    movs r0, #0
    str r0, [r5]
    mov sl, r0
    add r3, sp, #8
    mov r8, r3
.L08038F08:
    mov r7, sl
    lsls r0, r7, #2
    ldr r1, .L08038F14 @ =.L08038F18
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L08038F14: .4byte .L08038F18
.L08038F18: @ jump table
    .4byte .L08038F2C @ case 0
    .4byte .L08038F4C @ case 1
    .4byte .L08038F4C @ case 2
    .4byte .L08038FDC @ case 3
    .4byte .L08038FDC @ case 4
.L08038F2C:
    ldr r0, [r6]
    ldr r1, [r0, #0x78]
    adds r0, r6, #0
    bl _call_via_r1
    adds r1, r0, #0
    movs r0, #4
    str r0, [sp]
    movs r0, #0x1c
    str r0, [sp, #4]
    movs r0, #0
    mov r2, r8
    strb r0, [r2]
    add r0, sp, #0xc
    movs r2, #2
    b .L08038F6A
.L08038F4C:
    ldr r0, [r6]
    ldr r1, [r0, #0x78]
    adds r0, r6, #0
    bl _call_via_r1
    adds r1, r0, #0
    movs r0, #4
    str r0, [sp]
    movs r0, #0x1c
    str r0, [sp, #4]
    movs r0, #0
    mov r2, r8
    strb r0, [r2]
    add r0, sp, #0xc
    movs r2, #1
.L08038F6A:
    adds r3, r6, #0
    bl InitializeAnimatedMapSpriteSinglePalette
    ldr r0, [r5]
    cmp r0, #4
    bhi .L08038FCC
    lsls r0, r0, #6
    adds r0, #4
    adds r4, r5, r0
    cmp r4, #0
    beq .L08038FC6
    adds r0, r4, #0
    add r1, sp, #0xc
    bl func_080DC840
    ldr r0, .L08038FD8 @ =vtable_unk_080E681C
    str r0, [r4, #0x24]
    adds r1, r4, #0
    adds r1, #0x28
    add r0, sp, #0x34
    ldm r0!, {r2, r3, r7}
    stm r1!, {r2, r3, r7}
    ldm r0!, {r3, r7}
    stm r1!, {r3, r7}
    add r0, sp, #0x48
    ldrb r1, [r0]
    adds r0, r4, #0
    adds r0, #0x3c
    strb r1, [r0]
    mov r0, sp
    adds r0, #0x49
    ldrb r0, [r0]
    adds r1, r4, #0
    adds r1, #0x3d
    strb r0, [r1]
    mov r0, sp
    adds r0, #0x4a
    ldrb r0, [r0]
    adds r1, #1
    strb r0, [r1]
    mov r0, sp
    adds r0, #0x4b
    ldrb r1, [r0]
    adds r0, r4, #0
    adds r0, #0x3f
    strb r1, [r0]
.L08038FC6:
    ldr r0, [r5]
    adds r0, #1
    str r0, [r5]
.L08038FCC:
    add r0, sp, #0xc
    movs r1, #2
    bl DestroyMapSpriteResource
    b .L08039064
    .align 2, 0
.L08038FD8: .4byte vtable_unk_080E681C
.L08038FDC:
    ldr r0, [r6]
    ldr r1, [r0, #0x78]
    adds r0, r6, #0
    bl _call_via_r1
    adds r1, r0, #0
    movs r0, #4
    str r0, [sp]
    movs r0, #0x1c
    str r0, [sp, #4]
    movs r0, #0
    mov r2, r8
    strb r0, [r2]
    add r0, sp, #0xc
    movs r2, #0
    adds r3, r6, #0
    bl InitializeAnimatedMapSpriteSinglePalette
    ldr r0, [r5]
    cmp r0, #4
    bhi .L0803905C
    lsls r0, r0, #6
    adds r0, #4
    adds r4, r5, r0
    cmp r4, #0
    beq .L08039056
    adds r0, r4, #0
    add r1, sp, #0xc
    bl func_080DC840
    ldr r0, .L080390CC @ =vtable_unk_080E681C
    str r0, [r4, #0x24]
    adds r1, r4, #0
    adds r1, #0x28
    add r0, sp, #0x34
    ldm r0!, {r2, r3, r7}
    stm r1!, {r2, r3, r7}
    ldm r0!, {r3, r7}
    stm r1!, {r3, r7}
    add r0, sp, #0x48
    ldrb r1, [r0]
    adds r0, r4, #0
    adds r0, #0x3c
    strb r1, [r0]
    mov r0, sp
    adds r0, #0x49
    ldrb r0, [r0]
    adds r1, r4, #0
    adds r1, #0x3d
    strb r0, [r1]
    mov r0, sp
    adds r0, #0x4a
    ldrb r0, [r0]
    adds r1, #1
    strb r0, [r1]
    mov r0, sp
    adds r0, #0x4b
    ldrb r1, [r0]
    adds r0, r4, #0
    adds r0, #0x3f
    strb r1, [r0]
.L08039056:
    ldr r0, [r5]
    adds r0, #1
    str r0, [r5]
.L0803905C:
    add r0, sp, #0xc
    movs r1, #2
    bl DestroyMapSpriteResource
.L08039064:
    movs r1, #0xa2
    lsls r1, r1, #1
    adds r0, r5, r1
    add r0, sl
    movs r1, #0
    strb r1, [r0]
    movs r2, #1
    add sl, r2
    mov r3, sl
    cmp r3, #4
    bhi .L0803907C
    b .L08038F08
.L0803907C:
    movs r7, #0x10
    add r7, sb
    mov r8, r7
    adds r7, r5, #0
    mov r0, sb
    ldr r6, [r0, #0x10]
    cmp r7, r6
    beq .L080390B6
    cmp r6, #0
    beq .L080390B6
    ldr r0, [r6]
    lsls r0, r0, #6
    adds r0, #4
    adds r5, r6, r0
    adds r4, r6, #4
    cmp r4, r5
    beq .L080390B0
.L0803909E:
    ldr r0, [r4, #0x24]
    ldr r2, [r0, #8]
    adds r0, r4, #0
    movs r1, #2
    bl _call_via_r2
    adds r4, #0x40
    cmp r4, r5
    bne .L0803909E
.L080390B0:
    adds r0, r6, #0
    bl __builtin_delete
.L080390B6:
    mov r1, r8
    str r7, [r1]
    add sp, #0x4c
    pop {r3, r4, r5}
    mov r8, r3
    mov sb, r4
    mov sl, r5
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0
    .align 2, 0
.L080390CC: .4byte vtable_unk_080E681C

    .section .text.npc_extra.080390D0, "ax", %progbits

    .section .text.npc_extra.080390DC, "ax", %progbits
    .section .text.npc_extra.08039134, "ax", %progbits

    .section .text.npc_extra.08039204, "ax", %progbits

    .section .text.npc_extra.08039310, "ax", %progbits


    .section .text.npc_extra.08039708, "ax", %progbits

    .section .text.npc_extra.080398A4, "ax", %progbits

    .section .text.npc_extra.080399C0, "ax", %progbits

    .section .text.npc_extra.08039A5C, "ax", %progbits

    .section .text.npc_extra.08039D5C, "ax", %progbits

    .section .text.npc_extra.08039DA8, "ax", %progbits

    .section .text.npc_extra.08039E98, "ax", %progbits

    .section .text.npc_extra.08039F90, "ax", %progbits

    .section .text.npc_extra.0803A144, "ax", %progbits

    .section .text.npc_extra.0803A180, "ax", %progbits


    .section .text.npc_extra.0803A350, "ax", %progbits
