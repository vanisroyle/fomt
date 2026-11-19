    .INCLUDE "asm/macro.inc"
    .SYNTAX UNIFIED

    .section .text.entities_part2.08029558,"ax",%progbits

    thumb_func_start func_08029558
func_08029558: @ 0x08029558
    push {r4, r5, r6, r7, lr}
    sub sp, #8
    adds r6, r0, #0
    adds r0, #0xa4
    movs r4, #1
    strb r4, [r0]
    ldr r0, [r6, #0x34]
    ldr r1, .L08029578 @ =0x00001C2C
    adds r0, r0, r1
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802957C
    movs r0, #0
    b .L080299C8
    .align 2, 0
.L08029578: .4byte 0x00001C2C
.L0802957C:
    adds r0, r6, #0
    adds r0, #0xc0
    strb r4, [r0]
    adds r0, r6, #0
    bl func_08025FC4
    movs r5, #0xc9
    lsls r5, r5, #1
    adds r1, r6, #0
    adds r1, #0x7c
    ldr r0, .L080295AC @ =0x0000FFFF
    strh r0, [r1]
    adds r0, r6, #0
    adds r0, #0x3c
    ldrb r0, [r0]
    adds r7, r1, #0
    cmp r0, #0x39
    bls .L080295A2
    b .L080299BC
.L080295A2:
    lsls r0, r0, #2
    ldr r1, .L080295B0 @ =.L080295B4
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L080295AC: .4byte 0x0000FFFF
.L080295B0: .4byte .L080295B4
.L080295B4: @ jump table
    .4byte .L0802969C @ case 0
    .4byte .L08029776 @ case 1
    .4byte .L08029710 @ case 2
    .4byte .L08029776 @ case 3
    .4byte .L08029760 @ case 4
    .4byte .L08029776 @ case 5
    .4byte .L08029892 @ case 6
    .4byte .L0802989E @ case 7
    .4byte .L080299AE @ case 8
    .4byte .L080297AE @ case 9
    .4byte .L08029992 @ case 10
    .4byte .L08029794 @ case 11
    .4byte .L08029794 @ case 12
    .4byte .L0802978E @ case 13
    .4byte .L08029794 @ case 14
    .4byte .L0802979A @ case 15
    .4byte .L0802979E @ case 16
    .4byte .L08029980 @ case 17
    .4byte .L0802998C @ case 18
    .4byte .L080297A2 @ case 19
    .4byte .L080297A8 @ case 20
    .4byte .L08029992 @ case 21
    .4byte .L08029886 @ case 22
    .4byte .L0802988C @ case 23
    .4byte .L08029898 @ case 24
    .4byte .L080297FC @ case 25
    .4byte .L080297FC @ case 26
    .4byte .L080297CE @ case 27
    .4byte .L080297EC @ case 28
    .4byte .L080297EC @ case 29
    .4byte .L0802982C @ case 30
    .4byte .L08029846 @ case 31
    .4byte .L08029862 @ case 32
    .4byte .L08029980 @ case 33
    .4byte .L080298E8 @ case 34
    .4byte .L08029904 @ case 35
    .4byte .L0802991C @ case 36
    .4byte .L08029922 @ case 37
    .4byte .L08029928 @ case 38
    .4byte .L0802992C @ case 39
    .4byte .L08029930 @ case 40
    .4byte .L08029936 @ case 41
    .4byte .L0802993C @ case 42
    .4byte .L0802993C @ case 43
    .4byte .L08029942 @ case 44
    .4byte .L08029958 @ case 45
    .4byte .L0802997C @ case 46
    .4byte .L08029992 @ case 47
    .4byte .L08029986 @ case 48
    .4byte .L08029980 @ case 49
    .4byte .L08029986 @ case 50
    .4byte .L0802998C @ case 51
    .4byte .L08029992 @ case 52
    .4byte .L08029996 @ case 53
    .4byte .L08029980 @ case 54
    .4byte .L08029980 @ case 55
    .4byte .L08029986 @ case 56
    .4byte .L08029986 @ case 57
.L0802969C:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L080296CE
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    bne .L080296B6
    b .L080299A8
.L080296B6:
    cmp r0, #1
    bgt .L080296C2
    cmp r0, #0
    bne .L080296C0
    b .L08029986
.L080296C0:
    b .L080299BC
.L080296C2:
    cmp r0, #2
    beq .L080296C8
    b .L080299BC
.L080296C8:
    movs r5, #0xa7
    lsls r5, r5, #1
    b .L080299BC
.L080296CE:
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    beq .L080296EA
    cmp r0, #1
    bgt .L080296E4
    cmp r0, #0
    bne .L080296E2
    b .L080297FC
.L080296E2:
    b .L080299BC
.L080296E4:
    cmp r0, #2
    beq .L080296FC
    b .L080299BC
.L080296EA:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L0802970C
    adds r1, #0x94
    b .L0802970C
.L080296FC:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L0802970C
    adds r1, #0x2c
.L0802970C:
    adds r5, r1, #0
    b .L080299BC
.L08029710:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08029720
    b .L08029986
.L08029720:
    adds r0, r6, #0
    adds r0, #0x3f
    ldrb r3, [r0]
    cmp r3, #5
    bgt .L080297FC
    cmp r3, #0
    blt .L080297FC
    ldr r2, .L0802975C @ =gUnk_080F0FCC
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
    b .L0802987E
    .align 2, 0
.L0802975C: .4byte gUnk_080F0FCC
.L08029760:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L08029774
    b .L080299BC
.L08029774:
    b .L08029986
.L08029776:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L0802978A
    b .L080299BC
.L0802978A:
    subs r5, #0x40
    b .L080299BC
.L0802978E:
    movs r5, #0x9b
    lsls r5, r5, #1
    b .L080299BC
.L08029794:
    movs r5, #0xcf
    lsls r5, r5, #1
    b .L080299BC
.L0802979A:
    movs r5, #0x4e
    b .L080299BC
.L0802979E:
    movs r5, #0x46
    b .L080299BC
.L080297A2:
    movs r5, #0x9f
    lsls r5, r5, #1
    b .L080299BC
.L080297A8:
    movs r5, #0xa1
    lsls r5, r5, #1
    b .L080299BC
.L080297AE:
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
    b .L0802987C
.L080297CE:
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
    b .L0802987C
.L080297EC:
    adds r2, r6, #0
    adds r2, #0x4e
    adds r1, r6, #0
    adds r1, #0x4f
    ldrb r0, [r2]
    ldrb r3, [r1]
    cmp r0, r3
    bne .L08029802
.L080297FC:
    movs r5, #0xc9
    lsls r5, r5, #1
    b .L080299BC
.L08029802:
    ldrb r0, [r1]
    ldrb r1, [r2]
    subs r0, r0, r1
    movs r5, #0xc1
    lsls r5, r5, #1
    cmp r0, #1
    bne .L08029812
    adds r5, #4
.L08029812:
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
    b .L0802987C
.L0802982C:
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
    b .L0802987C
.L08029846:
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
    b .L0802987C
.L08029862:
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
.L0802987C:
    strb r1, [r0]
.L0802987E:
    bl GetIconId__C4Tool
    strh r0, [r7]
    b .L080299BC
.L08029886:
    movs r5, #0xfd
    lsls r5, r5, #1
    b .L080299BC
.L0802988C:
    movs r5, #0xcb
    lsls r5, r5, #1
    b .L080299BC
.L08029892:
    movs r5, #0x89
    lsls r5, r5, #1
    b .L080299BC
.L08029898:
    movs r5, #0x8b
    lsls r5, r5, #1
    b .L080299BC
.L0802989E:
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
    beq .L080298D0
    cmp r0, #1
    bgt .L080298C0
    cmp r0, #0
    beq .L080298C6
    b .L080299BC
.L080298C0:
    cmp r0, #2
    beq .L080298DC
    b .L080299BC
.L080298C6:
    movs r5, #0xea
    ldr r1, .L080298CC @ =0x0000077A
    b .L0802996A
    .align 2, 0
.L080298CC: .4byte 0x0000077A
.L080298D0:
    movs r5, #0xee
    ldr r1, .L080298D8 @ =0x0000077E
    b .L0802996A
    .align 2, 0
.L080298D8: .4byte 0x0000077E
.L080298DC:
    movs r5, #0xe6
    ldr r1, .L080298E4 @ =0x00000782
    b .L0802996A
    .align 2, 0
.L080298E4: .4byte 0x00000782
.L080298E8:
    movs r5, #0xad
    lsls r5, r5, #1
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08029900 @ =0x0000077A
    b .L0802996A
    .align 2, 0
.L08029900: .4byte 0x0000077A
.L08029904:
    movs r5, #0x9a
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08029918 @ =0x0000077A
    b .L0802996A
    .align 2, 0
.L08029918: .4byte 0x0000077A
.L0802991C:
    movs r5, #0x83
    lsls r5, r5, #1
    b .L080299BC
.L08029922:
    movs r5, #0x81
    lsls r5, r5, #1
    b .L080299BC
.L08029928:
    movs r5, #0xfe
    b .L080299BC
.L0802992C:
    movs r5, #0xfa
    b .L080299BC
.L08029930:
    movs r5, #0x91
    lsls r5, r5, #1
    b .L080299BC
.L08029936:
    movs r5, #0x8f
    lsls r5, r5, #1
    b .L080299BC
.L0802993C:
    movs r5, #0x8d
    lsls r5, r5, #1
    b .L080299BC
.L08029942:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xa5
    lsls r5, r5, #1
    cmp r0, #0
    beq .L080299BC
    subs r5, #0x40
    b .L080299BC
.L08029958:
    movs r5, #0xe2
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08029978 @ =0x00000792
.L0802996A:
    ldrh r0, [r2, #0x22]
    cmp r0, r1
    beq .L080299BC
    adds r0, r2, #0
    bl SetAnim__12AActorEntityUi
    b .L080299BC
    .align 2, 0
.L08029978: .4byte 0x00000792
.L0802997C:
    movs r5, #0x3e
    b .L080299BC
.L08029980:
    movs r5, #0xa3
    lsls r5, r5, #1
    b .L080299BC
.L08029986:
    movs r5, #0xa9
    lsls r5, r5, #1
    b .L080299BC
.L0802998C:
    movs r5, #0x9d
    lsls r5, r5, #1
    b .L080299BC
.L08029992:
    movs r5, #0x42
    b .L080299BC
.L08029996:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xe3
    lsls r5, r5, #1
    cmp r0, #0
    bne .L080299BC
.L080299A8:
    movs r5, #0xab
    lsls r5, r5, #1
    b .L080299BC
.L080299AE:
    adds r0, r6, #0
    adds r0, #0x3d
    ldrb r0, [r0]
    movs r5, #0xf6
    cmp r0, #0
    beq .L080299BC
    movs r5, #0xf2
.L080299BC:
    lsls r1, r5, #0x10
    lsrs r1, r1, #0x10
    adds r0, r6, #0
    bl SetAnim__12AActorEntityUi
    movs r0, #1
.L080299C8:
    add sp, #8
    pop {r4, r5, r6, r7}
    pop {r1}
    bx r1

    .section .text.entities_part2.080299EC,"ax",%progbits

    thumb_func_start func_080299EC
func_080299EC: @ 0x080299EC
    push {r4, r5, r6, r7, lr}
    mov r7, sl
    mov r6, sb
    mov r5, r8
    push {r5, r6, r7}
    sub sp, #8
    adds r6, r0, #0
    adds r1, r6, #0
    adds r1, #0xa4
    movs r0, #1
    strb r0, [r1]
    adds r0, r6, #0
    bl func_0802A588
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08029AD0
    ldr r0, [r6]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    movs r3, #0x3c
    adds r3, r3, r6
    mov r8, r3
    movs r0, #0x20
    adds r0, r0, r6
    mov sb, r0
    adds r7, r6, #0
    adds r7, #0x7c
    movs r1, #0xc0
    adds r1, r1, r6
    mov sl, r1
    cmp r4, r5
    beq .L08029A56
.L08029A44:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08029A7A
    adds r4, #4
    cmp r4, r5
    bne .L08029A44
.L08029A56:
    subs r0, r5, #4
    ldr r0, [r0]
.L08029A5A:
    movs r1, #0x6a
    bl func_08008B6C
    movs r4, #0
    movs r0, #0x11
    mov r2, r8
    strb r0, [r2]
    mov r0, sb
    ldrb r3, [r0]
    cmp r3, #1
    beq .L08029AA8
    cmp r3, #1
    bgt .L08029A7E
    cmp r3, #0
    beq .L08029ABC
    b .L08029AE4
.L08029A7A:
    ldr r0, [r4]
    b .L08029A5A
.L08029A7E:
    cmp r3, #2
    beq .L08029A88
    cmp r3, #3
    beq .L08029A94
    b .L08029AE4
.L08029A88:
    adds r0, r6, #0
    adds r0, #0x88
    ldr r1, .L08029A90 @ =0xFFF60000
    b .L08029A9C
    .align 2, 0
.L08029A90: .4byte 0xFFF60000
.L08029A94:
    adds r0, r6, #0
    adds r0, #0x88
    movs r1, #0xa0
    lsls r1, r1, #0xc
.L08029A9C:
    movs r2, #0xc0
    lsls r2, r2, #0xc
    str r1, [r0, #4]
    str r4, [r0, #8]
    str r2, [r0, #0x14]
    b .L08029AE4
.L08029AA8:
    adds r0, r6, #0
    adds r0, #0x88
    ldr r1, .L08029AB8 @ =0xFFF60000
    movs r2, #0xc0
    lsls r2, r2, #0xc
    str r4, [r0, #4]
    b .L08029ACA
    .align 2, 0
.L08029AB8: .4byte 0xFFF60000
.L08029ABC:
    adds r0, r6, #0
    adds r0, #0x88
    movs r1, #0xa0
    lsls r1, r1, #0xc
    movs r2, #0xc0
    lsls r2, r2, #0xc
    str r3, [r0, #4]
.L08029ACA:
    str r1, [r0, #8]
    str r2, [r0, #0x14]
    b .L08029AE4
.L08029AD0:
    adds r1, r6, #0
    adds r1, #0x3c
    movs r0, #0
    strb r0, [r1]
    mov r8, r1
    adds r7, r6, #0
    adds r7, #0x7c
    movs r1, #0xc0
    adds r1, r1, r6
    mov sl, r1
.L08029AE4:
    movs r5, #0xc9
    lsls r5, r5, #1
    ldr r0, .L08029B00 @ =0x0000FFFF
    strh r0, [r7]
    mov r2, r8
    ldrb r0, [r2]
    cmp r0, #0x39
    bls .L08029AF6
    b .L08029F10
.L08029AF6:
    lsls r0, r0, #2
    ldr r1, .L08029B04 @ =.L08029B08
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L08029B00: .4byte 0x0000FFFF
.L08029B04: .4byte .L08029B08
.L08029B08: @ jump table
    .4byte .L08029BF0 @ case 0
    .4byte .L08029CCA @ case 1
    .4byte .L08029C64 @ case 2
    .4byte .L08029CCA @ case 3
    .4byte .L08029CB4 @ case 4
    .4byte .L08029CCA @ case 5
    .4byte .L08029DE6 @ case 6
    .4byte .L08029DF2 @ case 7
    .4byte .L08029F02 @ case 8
    .4byte .L08029D02 @ case 9
    .4byte .L08029EE6 @ case 10
    .4byte .L08029CE8 @ case 11
    .4byte .L08029CE8 @ case 12
    .4byte .L08029CE2 @ case 13
    .4byte .L08029CE8 @ case 14
    .4byte .L08029CEE @ case 15
    .4byte .L08029CF2 @ case 16
    .4byte .L08029ED4 @ case 17
    .4byte .L08029EE0 @ case 18
    .4byte .L08029CF6 @ case 19
    .4byte .L08029CFC @ case 20
    .4byte .L08029EE6 @ case 21
    .4byte .L08029DDA @ case 22
    .4byte .L08029DE0 @ case 23
    .4byte .L08029DEC @ case 24
    .4byte .L08029D50 @ case 25
    .4byte .L08029D50 @ case 26
    .4byte .L08029D22 @ case 27
    .4byte .L08029D40 @ case 28
    .4byte .L08029D40 @ case 29
    .4byte .L08029D80 @ case 30
    .4byte .L08029D9A @ case 31
    .4byte .L08029DB6 @ case 32
    .4byte .L08029ED4 @ case 33
    .4byte .L08029E3C @ case 34
    .4byte .L08029E58 @ case 35
    .4byte .L08029E70 @ case 36
    .4byte .L08029E76 @ case 37
    .4byte .L08029E7C @ case 38
    .4byte .L08029E80 @ case 39
    .4byte .L08029E84 @ case 40
    .4byte .L08029E8A @ case 41
    .4byte .L08029E90 @ case 42
    .4byte .L08029E90 @ case 43
    .4byte .L08029E96 @ case 44
    .4byte .L08029EAC @ case 45
    .4byte .L08029ED0 @ case 46
    .4byte .L08029EE6 @ case 47
    .4byte .L08029EDA @ case 48
    .4byte .L08029ED4 @ case 49
    .4byte .L08029EDA @ case 50
    .4byte .L08029EE0 @ case 51
    .4byte .L08029EE6 @ case 52
    .4byte .L08029EEA @ case 53
    .4byte .L08029ED4 @ case 54
    .4byte .L08029ED4 @ case 55
    .4byte .L08029EDA @ case 56
    .4byte .L08029EDA @ case 57
.L08029BF0:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08029C22
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    bne .L08029C0A
    b .L08029EFC
.L08029C0A:
    cmp r0, #1
    bgt .L08029C16
    cmp r0, #0
    bne .L08029C14
    b .L08029EDA
.L08029C14:
    b .L08029F10
.L08029C16:
    cmp r0, #2
    beq .L08029C1C
    b .L08029F10
.L08029C1C:
    movs r5, #0xa7
    lsls r5, r5, #1
    b .L08029F10
.L08029C22:
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    beq .L08029C3E
    cmp r0, #1
    bgt .L08029C38
    cmp r0, #0
    bne .L08029C36
    b .L08029D50
.L08029C36:
    b .L08029F10
.L08029C38:
    cmp r0, #2
    beq .L08029C50
    b .L08029F10
.L08029C3E:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L08029C60
    adds r1, #0x94
    b .L08029C60
.L08029C50:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L08029C60
    adds r1, #0x2c
.L08029C60:
    adds r5, r1, #0
    b .L08029F10
.L08029C64:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08029C74
    b .L08029EDA
.L08029C74:
    adds r0, r6, #0
    adds r0, #0x3f
    ldrb r3, [r0]
    cmp r3, #5
    bgt .L08029D50
    cmp r3, #0
    blt .L08029D50
    ldr r2, .L08029CB0 @ =gUnk_080F0FCC
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
    b .L08029DD2
    .align 2, 0
.L08029CB0: .4byte gUnk_080F0FCC
.L08029CB4:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L08029CC8
    b .L08029F10
.L08029CC8:
    b .L08029EDA
.L08029CCA:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L08029CDE
    b .L08029F10
.L08029CDE:
    subs r5, #0x40
    b .L08029F10
.L08029CE2:
    movs r5, #0x9b
    lsls r5, r5, #1
    b .L08029F10
.L08029CE8:
    movs r5, #0xcf
    lsls r5, r5, #1
    b .L08029F10
.L08029CEE:
    movs r5, #0x4e
    b .L08029F10
.L08029CF2:
    movs r5, #0x46
    b .L08029F10
.L08029CF6:
    movs r5, #0x9f
    lsls r5, r5, #1
    b .L08029F10
.L08029CFC:
    movs r5, #0xa1
    lsls r5, r5, #1
    b .L08029F10
.L08029D02:
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
    b .L08029DD0
.L08029D22:
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
    b .L08029DD0
.L08029D40:
    adds r2, r6, #0
    adds r2, #0x4e
    adds r1, r6, #0
    adds r1, #0x4f
    ldrb r0, [r2]
    ldrb r3, [r1]
    cmp r0, r3
    bne .L08029D56
.L08029D50:
    movs r5, #0xc9
    lsls r5, r5, #1
    b .L08029F10
.L08029D56:
    ldrb r0, [r1]
    ldrb r1, [r2]
    subs r0, r0, r1
    movs r5, #0xc1
    lsls r5, r5, #1
    cmp r0, #1
    bne .L08029D66
    adds r5, #4
.L08029D66:
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
    b .L08029DD0
.L08029D80:
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
    b .L08029DD0
.L08029D9A:
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
    b .L08029DD0
.L08029DB6:
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
.L08029DD0:
    strb r1, [r0]
.L08029DD2:
    bl GetIconId__C4Tool
    strh r0, [r7]
    b .L08029F10
.L08029DDA:
    movs r5, #0xfd
    lsls r5, r5, #1
    b .L08029F10
.L08029DE0:
    movs r5, #0xcb
    lsls r5, r5, #1
    b .L08029F10
.L08029DE6:
    movs r5, #0x89
    lsls r5, r5, #1
    b .L08029F10
.L08029DEC:
    movs r5, #0x8b
    lsls r5, r5, #1
    b .L08029F10
.L08029DF2:
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
    beq .L08029E24
    cmp r0, #1
    bgt .L08029E14
    cmp r0, #0
    beq .L08029E1A
    b .L08029F10
.L08029E14:
    cmp r0, #2
    beq .L08029E30
    b .L08029F10
.L08029E1A:
    movs r5, #0xea
    ldr r1, .L08029E20 @ =0x0000077A
    b .L08029EBE
    .align 2, 0
.L08029E20: .4byte 0x0000077A
.L08029E24:
    movs r5, #0xee
    ldr r1, .L08029E2C @ =0x0000077E
    b .L08029EBE
    .align 2, 0
.L08029E2C: .4byte 0x0000077E
.L08029E30:
    movs r5, #0xe6
    ldr r1, .L08029E38 @ =0x00000782
    b .L08029EBE
    .align 2, 0
.L08029E38: .4byte 0x00000782
.L08029E3C:
    movs r5, #0xad
    lsls r5, r5, #1
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08029E54 @ =0x0000077A
    b .L08029EBE
    .align 2, 0
.L08029E54: .4byte 0x0000077A
.L08029E58:
    movs r5, #0x9a
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08029E6C @ =0x0000077A
    b .L08029EBE
    .align 2, 0
.L08029E6C: .4byte 0x0000077A
.L08029E70:
    movs r5, #0x83
    lsls r5, r5, #1
    b .L08029F10
.L08029E76:
    movs r5, #0x81
    lsls r5, r5, #1
    b .L08029F10
.L08029E7C:
    movs r5, #0xfe
    b .L08029F10
.L08029E80:
    movs r5, #0xfa
    b .L08029F10
.L08029E84:
    movs r5, #0x91
    lsls r5, r5, #1
    b .L08029F10
.L08029E8A:
    movs r5, #0x8f
    lsls r5, r5, #1
    b .L08029F10
.L08029E90:
    movs r5, #0x8d
    lsls r5, r5, #1
    b .L08029F10
.L08029E96:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xa5
    lsls r5, r5, #1
    cmp r0, #0
    beq .L08029F10
    subs r5, #0x40
    b .L08029F10
.L08029EAC:
    movs r5, #0xe2
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L08029ECC @ =0x00000792
.L08029EBE:
    ldrh r0, [r2, #0x22]
    cmp r0, r1
    beq .L08029F10
    adds r0, r2, #0
    bl SetAnim__12AActorEntityUi
    b .L08029F10
    .align 2, 0
.L08029ECC: .4byte 0x00000792
.L08029ED0:
    movs r5, #0x3e
    b .L08029F10
.L08029ED4:
    movs r5, #0xa3
    lsls r5, r5, #1
    b .L08029F10
.L08029EDA:
    movs r5, #0xa9
    lsls r5, r5, #1
    b .L08029F10
.L08029EE0:
    movs r5, #0x9d
    lsls r5, r5, #1
    b .L08029F10
.L08029EE6:
    movs r5, #0x42
    b .L08029F10
.L08029EEA:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xe3
    lsls r5, r5, #1
    cmp r0, #0
    bne .L08029F10
.L08029EFC:
    movs r5, #0xab
    lsls r5, r5, #1
    b .L08029F10
.L08029F02:
    adds r0, r6, #0
    adds r0, #0x3d
    ldrb r0, [r0]
    movs r5, #0xf6
    cmp r0, #0
    beq .L08029F10
    movs r5, #0xf2
.L08029F10:
    lsls r1, r5, #0x10
    lsrs r1, r1, #0x10
    adds r0, r6, #0
    bl SetAnim__12AActorEntityUi
    movs r0, #1
    mov r1, sl
    strb r0, [r1]
    add sp, #8
    pop {r3, r4, r5}
    mov r8, r3
    mov sb, r4
    mov sl, r5
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0

    thumb_func_start func_08029F30
func_08029F30: @ 0x08029F30
    push {r4, r5, r6, r7, lr}
    mov r7, r8
    push {r7}
    sub sp, #8
    adds r6, r0, #0
    adds r3, r1, #0
    adds r0, #0xa4
    movs r1, #0
    mov r8, r1
    movs r7, #1
    strb r7, [r0]
    subs r0, #0x1c
    strb r7, [r0]
    mov r1, r8
    strb r1, [r0, #1]
    ldr r1, .L08029FD0 @ =0xFFF80000
    movs r2, #0x88
    lsls r2, r2, #0xd
    str r1, [r0, #4]
    mov r1, r8
    str r1, [r0, #8]
    str r2, [r0, #0x14]
    ldr r5, [r6, #0x10]
    cmp r5, #0
    beq .L08029F9E
    mov r0, sp
    adds r1, r3, #0
    bl __4ToolUi
    mov r0, sp
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
    strb r7, [r0]
    adds r0, #2
    mov r3, r8
    strb r3, [r0]
    adds r0, #1
    strb r7, [r0]
.L08029F9E:
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
    ldr r0, .L08029FD4 @ =0x0000FFFF
    strh r0, [r1]
    ldrb r0, [r3]
    adds r7, r1, #0
    cmp r0, #0x39
    bls .L08029FC4
    b .L0802A3E0
.L08029FC4:
    lsls r0, r0, #2
    ldr r1, .L08029FD8 @ =.L08029FDC
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L08029FD0: .4byte 0xFFF80000
.L08029FD4: .4byte 0x0000FFFF
.L08029FD8: .4byte .L08029FDC
.L08029FDC: @ jump table
    .4byte .L0802A0C4 @ case 0
    .4byte .L0802A19E @ case 1
    .4byte .L0802A138 @ case 2
    .4byte .L0802A19E @ case 3
    .4byte .L0802A188 @ case 4
    .4byte .L0802A19E @ case 5
    .4byte .L0802A2BA @ case 6
    .4byte .L0802A2C6 @ case 7
    .4byte .L0802A3D6 @ case 8
    .4byte .L0802A1D6 @ case 9
    .4byte .L0802A3BA @ case 10
    .4byte .L0802A1BC @ case 11
    .4byte .L0802A1BC @ case 12
    .4byte .L0802A1B6 @ case 13
    .4byte .L0802A1BC @ case 14
    .4byte .L0802A1C2 @ case 15
    .4byte .L0802A1C6 @ case 16
    .4byte .L0802A3A8 @ case 17
    .4byte .L0802A3B4 @ case 18
    .4byte .L0802A1CA @ case 19
    .4byte .L0802A1D0 @ case 20
    .4byte .L0802A3BA @ case 21
    .4byte .L0802A2AE @ case 22
    .4byte .L0802A2B4 @ case 23
    .4byte .L0802A2C0 @ case 24
    .4byte .L0802A224 @ case 25
    .4byte .L0802A224 @ case 26
    .4byte .L0802A1F6 @ case 27
    .4byte .L0802A214 @ case 28
    .4byte .L0802A214 @ case 29
    .4byte .L0802A252 @ case 30
    .4byte .L0802A26E @ case 31
    .4byte .L0802A28A @ case 32
    .4byte .L0802A3A8 @ case 33
    .4byte .L0802A310 @ case 34
    .4byte .L0802A32C @ case 35
    .4byte .L0802A344 @ case 36
    .4byte .L0802A34A @ case 37
    .4byte .L0802A350 @ case 38
    .4byte .L0802A354 @ case 39
    .4byte .L0802A358 @ case 40
    .4byte .L0802A35E @ case 41
    .4byte .L0802A364 @ case 42
    .4byte .L0802A364 @ case 43
    .4byte .L0802A36A @ case 44
    .4byte .L0802A380 @ case 45
    .4byte .L0802A3A4 @ case 46
    .4byte .L0802A3BA @ case 47
    .4byte .L0802A3AE @ case 48
    .4byte .L0802A3A8 @ case 49
    .4byte .L0802A3AE @ case 50
    .4byte .L0802A3B4 @ case 51
    .4byte .L0802A3BA @ case 52
    .4byte .L0802A3BE @ case 53
    .4byte .L0802A3A8 @ case 54
    .4byte .L0802A3A8 @ case 55
    .4byte .L0802A3AE @ case 56
    .4byte .L0802A3AE @ case 57
.L0802A0C4:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802A0F6
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    bne .L0802A0DE
    b .L0802A3D0
.L0802A0DE:
    cmp r0, #1
    bgt .L0802A0EA
    cmp r0, #0
    bne .L0802A0E8
    b .L0802A3AE
.L0802A0E8:
    b .L0802A3E0
.L0802A0EA:
    cmp r0, #2
    beq .L0802A0F0
    b .L0802A3E0
.L0802A0F0:
    movs r5, #0xa7
    lsls r5, r5, #1
    b .L0802A3E0
.L0802A0F6:
    adds r0, r6, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    beq .L0802A112
    cmp r0, #1
    bgt .L0802A10C
    cmp r0, #0
    bne .L0802A10A
    b .L0802A224
.L0802A10A:
    b .L0802A3E0
.L0802A10C:
    cmp r0, #2
    beq .L0802A124
    b .L0802A3E0
.L0802A112:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L0802A134
    adds r1, #0x94
    b .L0802A134
.L0802A124:
    adds r0, r6, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L0802A134
    adds r1, #0x2c
.L0802A134:
    adds r5, r1, #0
    b .L0802A3E0
.L0802A138:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802A148
    b .L0802A3AE
.L0802A148:
    adds r0, r6, #0
    adds r0, #0x3f
    ldrb r3, [r0]
    cmp r3, #5
    bgt .L0802A224
    cmp r3, #0
    blt .L0802A224
    ldr r2, .L0802A184 @ =gUnk_080F0FCC
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
    mov r0, sp
    adds r0, #1
    b .L0802A2A4
    .align 2, 0
.L0802A184: .4byte gUnk_080F0FCC
.L0802A188:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L0802A19C
    b .L0802A3E0
.L0802A19C:
    b .L0802A3AE
.L0802A19E:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xc9
    lsls r5, r5, #1
    cmp r0, #0
    beq .L0802A1B2
    b .L0802A3E0
.L0802A1B2:
    subs r5, #0x40
    b .L0802A3E0
.L0802A1B6:
    movs r5, #0x9b
    lsls r5, r5, #1
    b .L0802A3E0
.L0802A1BC:
    movs r5, #0xcf
    lsls r5, r5, #1
    b .L0802A3E0
.L0802A1C2:
    movs r5, #0x4e
    b .L0802A3E0
.L0802A1C6:
    movs r5, #0x46
    b .L0802A3E0
.L0802A1CA:
    movs r5, #0x9f
    lsls r5, r5, #1
    b .L0802A3E0
.L0802A1D0:
    movs r5, #0xa1
    lsls r5, r5, #1
    b .L0802A3E0
.L0802A1D6:
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
    adds r0, #2
    b .L0802A2A4
.L0802A1F6:
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
    adds r0, #3
    b .L0802A2A4
.L0802A214:
    adds r2, r6, #0
    adds r2, #0x4e
    adds r1, r6, #0
    adds r1, #0x4f
    ldrb r0, [r2]
    ldrb r3, [r1]
    cmp r0, r3
    bne .L0802A22A
.L0802A224:
    movs r5, #0xc9
    lsls r5, r5, #1
    b .L0802A3E0
.L0802A22A:
    ldrb r0, [r1]
    ldrb r1, [r2]
    subs r0, r0, r1
    movs r5, #0xc1
    lsls r5, r5, #1
    cmp r0, #1
    bne .L0802A23A
    adds r5, #4
.L0802A23A:
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    add r0, sp, #4
    b .L0802A2A4
.L0802A252:
    movs r5, #0x76
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
    b .L0802A2A4
.L0802A26E:
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
    adds r0, #6
    b .L0802A2A4
.L0802A28A:
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
    adds r0, #7
.L0802A2A4:
    strb r1, [r0]
    bl GetIconId__C4Tool
    strh r0, [r7]
    b .L0802A3E0
.L0802A2AE:
    movs r5, #0xfd
    lsls r5, r5, #1
    b .L0802A3E0
.L0802A2B4:
    movs r5, #0xcb
    lsls r5, r5, #1
    b .L0802A3E0
.L0802A2BA:
    movs r5, #0x89
    lsls r5, r5, #1
    b .L0802A3E0
.L0802A2C0:
    movs r5, #0x8b
    lsls r5, r5, #1
    b .L0802A3E0
.L0802A2C6:
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
    beq .L0802A2F8
    cmp r0, #1
    bgt .L0802A2E8
    cmp r0, #0
    beq .L0802A2EE
    b .L0802A3E0
.L0802A2E8:
    cmp r0, #2
    beq .L0802A304
    b .L0802A3E0
.L0802A2EE:
    movs r5, #0xea
    ldr r1, .L0802A2F4 @ =0x0000077A
    b .L0802A392
    .align 2, 0
.L0802A2F4: .4byte 0x0000077A
.L0802A2F8:
    movs r5, #0xee
    ldr r1, .L0802A300 @ =0x0000077E
    b .L0802A392
    .align 2, 0
.L0802A300: .4byte 0x0000077E
.L0802A304:
    movs r5, #0xe6
    ldr r1, .L0802A30C @ =0x00000782
    b .L0802A392
    .align 2, 0
.L0802A30C: .4byte 0x00000782
.L0802A310:
    movs r5, #0xad
    lsls r5, r5, #1
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L0802A328 @ =0x0000077A
    b .L0802A392
    .align 2, 0
.L0802A328: .4byte 0x0000077A
.L0802A32C:
    movs r5, #0x9a
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L0802A340 @ =0x0000077A
    b .L0802A392
    .align 2, 0
.L0802A340: .4byte 0x0000077A
.L0802A344:
    movs r5, #0x83
    lsls r5, r5, #1
    b .L0802A3E0
.L0802A34A:
    movs r5, #0x81
    lsls r5, r5, #1
    b .L0802A3E0
.L0802A350:
    movs r5, #0xfe
    b .L0802A3E0
.L0802A354:
    movs r5, #0xfa
    b .L0802A3E0
.L0802A358:
    movs r5, #0x91
    lsls r5, r5, #1
    b .L0802A3E0
.L0802A35E:
    movs r5, #0x8f
    lsls r5, r5, #1
    b .L0802A3E0
.L0802A364:
    movs r5, #0x8d
    lsls r5, r5, #1
    b .L0802A3E0
.L0802A36A:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xa5
    lsls r5, r5, #1
    cmp r0, #0
    beq .L0802A3E0
    subs r5, #0x40
    b .L0802A3E0
.L0802A380:
    movs r5, #0xe2
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L0802A3A0 @ =0x00000792
.L0802A392:
    ldrh r0, [r2, #0x22]
    cmp r0, r1
    beq .L0802A3E0
    adds r0, r2, #0
    bl SetAnim__12AActorEntityUi
    b .L0802A3E0
    .align 2, 0
.L0802A3A0: .4byte 0x00000792
.L0802A3A4:
    movs r5, #0x3e
    b .L0802A3E0
.L0802A3A8:
    movs r5, #0xa3
    lsls r5, r5, #1
    b .L0802A3E0
.L0802A3AE:
    movs r5, #0xa9
    lsls r5, r5, #1
    b .L0802A3E0
.L0802A3B4:
    movs r5, #0x9d
    lsls r5, r5, #1
    b .L0802A3E0
.L0802A3BA:
    movs r5, #0x42
    b .L0802A3E0
.L0802A3BE:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r5, #0xe3
    lsls r5, r5, #1
    cmp r0, #0
    bne .L0802A3E0
.L0802A3D0:
    movs r5, #0xab
    lsls r5, r5, #1
    b .L0802A3E0
.L0802A3D6:
    ldrb r0, [r2]
    movs r5, #0xf6
    cmp r0, #0
    beq .L0802A3E0
    movs r5, #0xf2
.L0802A3E0:
    lsls r1, r5, #0x10
    lsrs r1, r1, #0x10
    adds r0, r6, #0
    bl SetAnim__12AActorEntityUi
    adds r1, r6, #0
    adds r1, #0xc0
    movs r0, #1
    strb r0, [r1]
    add sp, #8
    pop {r3}
    mov r8, r3
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0
    .align 2, 0

    thumb_func_start func_0802A400
func_0802A400: @ 0x0802A400
    push {r4, r5, r6, r7, lr}
    mov r7, sb
    mov r6, r8
    push {r6, r7}
    adds r6, r0, #0
    adds r7, r1, #0
    adds r0, #0xa4
    movs r1, #0
    mov r8, r1
    movs r2, #1
    mov sb, r2
    mov r1, sb
    strb r1, [r0]
    subs r0, #0x1c
    strb r1, [r0]
    mov r2, r8
    strb r2, [r0, #1]
    movs r1, #0xa8
    lsls r1, r1, #0xd
    mov r2, r8
    str r2, [r0, #4]
    str r2, [r0, #8]
    str r1, [r0, #0x14]
    ldr r5, [r6, #0x10]
    cmp r5, #0
    beq .L0802A46A
    adds r0, r7, #0
    bl GetIconId__C4Food
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
    mov r1, sb
    strb r1, [r0]
    adds r0, #2
    mov r2, r8
    strb r2, [r0]
    adds r0, #1
    strb r1, [r0]
.L0802A46A:
    ldr r5, [r6, #0x14]
    adds r0, r7, #0
    bl GetStaminaGain__C4Food
    adds r4, r0, #0
    adds r0, r7, #0
    bl GetFatigueGain__C4Food
    adds r2, r0, #0
    ldr r3, [r5, #0x64]
    adds r0, r6, #0
    adds r1, r4, #0
    bl _call_via_r3
    adds r0, r7, #0
    bl IsDrink__C4Food
    cmp r0, #0
    bne .L0802A498
    adds r1, r6, #0
    adds r1, #0x3c
    movs r0, #0x36
    b .L0802A49E
.L0802A498:
    adds r1, r6, #0
    adds r1, #0x3c
    movs r0, #0x37
.L0802A49E:
    strb r0, [r1]
    adds r0, r6, #0
    adds r0, #0xc0
    movs r1, #1
    strb r1, [r0]
    movs r1, #0xa3
    lsls r1, r1, #1
    adds r0, r6, #0
    bl SetAnim__12AActorEntityUi
    pop {r3, r4}
    mov r8, r3
    mov sb, r4
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0
    .align 2, 0

    thumb_func_start func_0802A4C0
func_0802A4C0: @ 0x0802A4C0
    push {r4, r5, lr}
    adds r3, r0, #0
    adds r5, r1, #0
    adds r1, r3, #0
    adds r1, #0x88
    movs r0, #1
    strb r0, [r1]
    cmp r2, #1
    beq .L0802A4D6
    cmp r2, #0
    bne .L0802A4D8
.L0802A4D6:
    strb r2, [r1, #1]
.L0802A4D8:
    ldr r4, [r3, #0x10]
    cmp r4, #0
    beq .L0802A508
    ldr r0, [r4]
    ldr r0, [r0]
    ldr r1, [r0]
    ldr r1, [r1, #0x64]
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
.L0802A508:
    pop {r4, r5}
    pop {r0}
    bx r0
    .align 2, 0

    .section .text.entities_part2.0802A588,"ax",%progbits

    thumb_func_start func_0802A588
func_0802A588: @ 0x0802A588
    push {r4, r5, r6, r7, lr}
    mov r7, r8
    push {r7}
    sub sp, #8
    adds r6, r0, #0
    ldr r0, [r6, #0x38]
    adds r4, r0, #0
    adds r4, #0x54
    adds r0, r4, #0
    bl func_0800F190
    lsls r0, r0, #0x18
    lsrs r7, r0, #0x18
    cmp r7, #0
    beq .L0802A5A8
    b .L0802A7C2
.L0802A5A8:
    adds r0, r6, #0
    adds r0, #0x88
    movs r1, #1
    mov r8, r1
    mov r2, r8
    strb r2, [r0]
    strb r7, [r0, #1]
    movs r1, #0xa8
    lsls r1, r1, #0xd
    str r7, [r0, #4]
    str r7, [r0, #8]
    str r1, [r0, #0x14]
    ldr r0, [r6, #0x10]
    cmp r0, #0
    bne .L0802A5C8
    b .L0802A7CA
.L0802A5C8:
    adds r5, r0, #0
    adds r0, r4, #0
    bl func_0800F388
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802A5FE
    ldr r0, [r5]
    ldr r0, [r0]
    ldr r1, [r0]
    ldr r1, [r1, #0x64]
    bl _call_via_r1
    adds r1, r0, #0
    adds r0, r5, #0
    adds r0, #0xb4
    movs r2, #0xb0
    lsls r2, r2, #1
    bl func_0805E850
    adds r0, r5, #0
    adds r0, #0xc8
    mov r1, r8
    strb r1, [r0]
    adds r0, #2
    strb r7, [r0]
    b .L0802A7BC
.L0802A5FE:
    adds r0, r4, #0
    bl func_0800F204
    cmp r0, #5
    bls .L0802A60A
    b .L0802A7CA
.L0802A60A:
    lsls r0, r0, #2
    ldr r1, .L0802A614 @ =.L0802A618
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L0802A614: .4byte .L0802A618
.L0802A618: @ jump table
    .4byte .L0802A630 @ case 0
    .4byte .L0802A646 @ case 1
    .4byte .L0802A6BE @ case 2
    .4byte .L0802A714 @ case 3
    .4byte .L0802A778 @ case 4
    .4byte .L0802A78E @ case 5
.L0802A630:
    mov r0, sp
    adds r1, r4, #0
    bl func_0800F20C
    mov r0, sp
    bl GetIconId__C4Food
    adds r4, r0, #0
    lsls r4, r4, #0x10
    lsrs r4, r4, #0x10
    b .L0802A796
.L0802A646:
    adds r0, r4, #0
    bl func_0800F258
    adds r1, r0, #0
    add r0, sp, #4
    strb r1, [r0]
    bl GetId__C7Article
    cmp r0, #0x1b
    bne .L0802A6A4
    adds r0, r6, #0
    bl func_08025AFC
    lsls r0, r0, #0x18
    lsrs r4, r0, #0x18
    cmp r4, #0
    beq .L0802A67E
    ldr r0, [r5]
    ldr r0, [r0]
    ldr r1, [r0]
    ldr r1, [r1, #0x64]
    bl _call_via_r1
    adds r1, r0, #0
    adds r0, r5, #0
    adds r0, #0xb4
    movs r2, #4
    b .L0802A7AA
.L0802A67E:
    ldr r0, [r5]
    ldr r0, [r0]
    ldr r1, [r0]
    ldr r1, [r1, #0x64]
    bl _call_via_r1
    adds r1, r0, #0
    adds r0, r5, #0
    adds r0, #0xb4
    movs r2, #3
    bl func_0805E850
    adds r0, r5, #0
    adds r0, #0xc8
    movs r1, #1
    strb r1, [r0]
    adds r0, #2
    strb r4, [r0]
    b .L0802A7BC
.L0802A6A4:
    adds r0, r4, #0
    bl func_0800F258
    adds r1, r0, #0
    mov r0, sp
    adds r0, #5
    strb r1, [r0]
    bl GetIconId__C7Article
    adds r4, r0, #0
    lsls r4, r4, #0x10
    lsrs r4, r4, #0x10
    b .L0802A796
.L0802A6BE:
    ldr r0, [r6, #0x34]
    ldr r2, .L0802A6F4 @ =0x00001C70
    adds r0, r0, r2
    bl GetGrowthStage__C3Dog
    adds r4, r0, #0
    cmp r4, #0
    bne .L0802A6FC
    ldr r0, [r5]
    ldr r0, [r0]
    ldr r1, [r0]
    ldr r1, [r1, #0x68]
    bl _call_via_r1
    adds r1, r0, #0
    adds r0, r5, #0
    adds r0, #0xb4
    ldr r2, .L0802A6F8 @ =0x000003DA
    bl func_0805E850
    adds r0, r5, #0
    adds r0, #0xc8
    movs r1, #1
    strb r1, [r0]
    adds r0, #2
    strb r4, [r0]
    b .L0802A7BC
    .align 2, 0
.L0802A6F4: .4byte 0x00001C70
.L0802A6F8: .4byte 0x000003DA
.L0802A6FC:
    ldr r0, [r5]
    ldr r0, [r0]
    ldr r1, [r0]
    ldr r1, [r1, #0x68]
    bl _call_via_r1
    adds r1, r0, #0
    adds r0, r5, #0
    adds r0, #0xb4
    movs r2, #0xdd
    lsls r2, r2, #2
    b .L0802A7AA
.L0802A714:
    adds r0, r4, #0
    bl func_0800F344
    adds r1, r0, #0
    ldr r0, [r6, #0x34]
    movs r2, #0x82
    lsls r2, r2, #3
    adds r0, r0, r2
    bl GetChicken__4CoopUi
    bl GetGrowthStage__C7Chicken
    adds r4, r0, #0
    cmp r4, #0
    bne .L0802A75C
    ldr r0, [r5]
    ldr r0, [r0]
    ldr r1, [r0]
    ldr r1, [r1, #0x68]
    bl _call_via_r1
    adds r1, r0, #0
    adds r0, r5, #0
    adds r0, #0xb4
    ldr r2, .L0802A758 @ =0x0000073D
    bl func_0805E850
    adds r0, r5, #0
    adds r0, #0xc8
    movs r1, #1
    strb r1, [r0]
    adds r0, #2
    strb r4, [r0]
    b .L0802A7BC
    .align 2, 0
.L0802A758: .4byte 0x0000073D
.L0802A75C:
    ldr r0, [r5]
    ldr r0, [r0]
    ldr r1, [r0]
    ldr r1, [r1, #0x68]
    bl _call_via_r1
    adds r1, r0, #0
    adds r0, r5, #0
    adds r0, #0xb4
    ldr r2, .L0802A774 @ =0x00000734
    b .L0802A7AA
    .align 2, 0
.L0802A774: .4byte 0x00000734
.L0802A778:
    ldr r0, [r5]
    ldr r0, [r0]
    ldr r1, [r0]
    ldr r1, [r1, #0x64]
    bl _call_via_r1
    adds r1, r0, #0
    adds r0, r5, #0
    adds r0, #0xb4
    movs r2, #0x35
    b .L0802A7AA
.L0802A78E:
    adds r0, r4, #0
    bl func_0800F360
    adds r4, r0, #0
.L0802A796:
    ldr r0, [r5]
    ldr r0, [r0]
    ldr r1, [r0]
    ldr r1, [r1, #0x64]
    bl _call_via_r1
    adds r1, r0, #0
    adds r0, r5, #0
    adds r0, #0xb4
    adds r2, r4, #0
.L0802A7AA:
    bl func_0805E850
    adds r0, r5, #0
    adds r0, #0xc8
    movs r2, #0
    movs r1, #1
    strb r1, [r0]
    adds r0, #2
    strb r2, [r0]
.L0802A7BC:
    adds r0, #1
    strb r1, [r0]
    b .L0802A7CA
.L0802A7C2:
    adds r1, r6, #0
    adds r1, #0x88
    movs r0, #0
    strb r0, [r1]
.L0802A7CA:
    add sp, #8
    pop {r3}
    mov r8, r3
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0
    .align 2, 0

    .section .text.entities_part2.0802A7E0,"ax",%progbits

    thumb_func_start func_0802A7E0
func_0802A7E0: @ 0x0802A7E0
    push {r4, r5, r6, r7, lr}
    mov r7, sl
    mov r6, sb
    mov r5, r8
    push {r5, r6, r7}
    sub sp, #0x34
    adds r5, r0, #0
    ldr r0, [r5, #0x38]
    adds r0, #0x54
    mov r8, r0
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802A800
    b .L0802AA72
.L0802A800:
    mov r0, r8
    bl func_0800F388
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802A80E
    b .L0802AA40
.L0802A80E:
    movs r1, #0xa
    ldrsh r0, [r5, r1]
    mov sb, r0
    movs r0, #0xe
    ldrsh r4, [r5, r0]
    mov sl, r4
    mov r4, sb
    mov r7, sl
    adds r0, r5, #0
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, #1
    beq .L0802A83C
    cmp r0, #1
    bgt .L0802A832
    cmp r0, #0
    beq .L0802A848
    b .L0802A866
.L0802A832:
    cmp r0, #2
    beq .L0802A852
    cmp r0, #3
    beq .L0802A85E
    b .L0802A866
.L0802A83C:
    movs r1, #0x14
    rsbs r1, r1, #0
    adds r1, r1, r7
    mov sl, r1
    subs r7, #0x10
    b .L0802A866
.L0802A848:
    movs r0, #0x14
    adds r0, r0, r7
    mov sl, r0
    adds r7, #0x10
    b .L0802A866
.L0802A852:
    movs r1, #0x14
    rsbs r1, r1, #0
    adds r1, r1, r4
    mov sb, r1
    subs r4, #0x10
    b .L0802A866
.L0802A85E:
    movs r0, #0x14
    adds r0, r0, r4
    mov sb, r0
    adds r4, #0x10
.L0802A866:
    ldr r1, [r5]
    str r1, [sp, #0x30]
    ldrh r6, [r5, #4]
    ldr r1, [r1]
    mov r0, sp
    ldr r3, [r1, #0x34]
    ldr r1, [sp, #0x30]
    adds r2, r6, #0
    bl _call_via_r3
    mov r5, sp
    mov r0, r8
    bl func_0800F204
    cmp r0, #5
    bls .L0802A888
    b .L0802AA72
.L0802A888:
    lsls r0, r0, #2
    ldr r1, .L0802A894 @ =.L0802A898
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L0802A894: .4byte .L0802A898
.L0802A898: @ jump table
    .4byte .L0802AA40 @ case 0
    .4byte .L0802A8B0 @ case 1
    .4byte .L0802AA44 @ case 2
    .4byte .L0802AA48 @ case 3
    .4byte .L0802AA4C @ case 4
    .4byte .L0802AA72 @ case 5
.L0802A8B0:
    mov r0, r8
    bl func_0800F258
    add r1, sp, #0x14
    strb r0, [r1]
    adds r0, r1, #0
    bl CanBeDiscarded__C7Article
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802A8C8
    b .L0802AA72
.L0802A8C8:
    add r0, sp, #0x14
    bl GetId__C7Article
    subs r0, #0x31
    cmp r0, #0x29
    bls .L0802A8D6
    b .L0802AA40
.L0802A8D6:
    lsls r0, r0, #2
    ldr r1, .L0802A8E0 @ =.L0802A8E4
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L0802A8E0: .4byte .L0802A8E4
.L0802A8E4: @ jump table
    .4byte .L0802A98C @ case 0
    .4byte .L0802A9A2 @ case 1
    .4byte .L0802AA40 @ case 2
    .4byte .L0802AA40 @ case 3
    .4byte .L0802A99E @ case 4
    .4byte .L0802AA40 @ case 5
    .4byte .L0802AA40 @ case 6
    .4byte .L0802AA40 @ case 7
    .4byte .L0802AA40 @ case 8
    .4byte .L0802AA40 @ case 9
    .4byte .L0802AA40 @ case 10
    .4byte .L0802AA40 @ case 11
    .4byte .L0802AA40 @ case 12
    .4byte .L0802AA40 @ case 13
    .4byte .L0802AA40 @ case 14
    .4byte .L0802AA40 @ case 15
    .4byte .L0802AA40 @ case 16
    .4byte .L0802AA40 @ case 17
    .4byte .L0802AA40 @ case 18
    .4byte .L0802AA40 @ case 19
    .4byte .L0802AA40 @ case 20
    .4byte .L0802AA40 @ case 21
    .4byte .L0802AA40 @ case 22
    .4byte .L0802AA40 @ case 23
    .4byte .L0802AA40 @ case 24
    .4byte .L0802AA40 @ case 25
    .4byte .L0802AA40 @ case 26
    .4byte .L0802AA40 @ case 27
    .4byte .L0802AA40 @ case 28
    .4byte .L0802AA40 @ case 29
    .4byte .L0802AA40 @ case 30
    .4byte .L0802AA40 @ case 31
    .4byte .L0802AA40 @ case 32
    .4byte .L0802AA40 @ case 33
    .4byte .L0802AA40 @ case 34
    .4byte .L0802AA40 @ case 35
    .4byte .L0802AA40 @ case 36
    .4byte .L0802AA40 @ case 37
    .4byte .L0802AA40 @ case 38
    .4byte .L0802AA40 @ case 39
    .4byte .L0802A9A2 @ case 40
    .4byte .L0802A9A2 @ case 41
.L0802A98C:
    adds r0, r5, #0
    mov r1, sb
    mov r2, sl
    bl func_080AC5D0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802A9A2
    b .L0802AA40
.L0802A99E:
    movs r0, #3
    b .L0802AA74
.L0802A9A2:
    ldr r0, [sp, #0x30]
    ldr r0, [r0]
    mov sb, r0
    movs r1, #0xe8
    add sb, r1
    add r1, sp, #0xc
    ldr r0, .L0802AA24 @ =0x000003FF
    mov r8, r0
    mov r0, r8
    ands r6, r0
    ldrh r2, [r1]
    ldr r5, .L0802AA28 @ =0xFFFFFC00
    adds r0, r5, #0
    ands r0, r2
    orrs r0, r6
    strh r0, [r1]
    lsls r4, r4, #0x10
    lsrs r2, r4, #0x10
    movs r0, #0x3f
    mov sl, r0
    ands r2, r0
    lsls r2, r2, #2
    ldrb r6, [r1, #1]
    movs r3, #3
    adds r0, r3, #0
    ands r0, r6
    orrs r0, r2
    strb r0, [r1, #1]
    lsrs r4, r4, #0x16
    mov r0, r8
    ands r4, r0
    ldrh r2, [r1, #2]
    adds r0, r5, #0
    ands r0, r2
    orrs r0, r4
    strh r0, [r1, #2]
    lsls r2, r7, #0x10
    lsrs r0, r2, #0x10
    mov r4, sl
    ands r0, r4
    lsls r0, r0, #2
    ldrb r4, [r1, #3]
    ands r3, r4
    orrs r3, r0
    strb r3, [r1, #3]
    lsrs r2, r2, #0x16
    mov r0, r8
    ands r2, r0
    ldrh r0, [r1, #4]
    ands r5, r0
    orrs r5, r2
    strh r5, [r1, #4]
    mov r4, sb
    ldr r3, [r4]
    ldr r0, [sp, #0x30]
    add r2, sp, #0x14
    bl _call_via_r3
    cmp r0, #1
    beq .L0802AA72
    cmp r0, #1
    bgt .L0802AA2C
    cmp r0, #0
    beq .L0802AA32
    b .L0802AA72
    .align 2, 0
.L0802AA24: .4byte 0x000003FF
.L0802AA28: .4byte 0xFFFFFC00
.L0802AA2C:
    cmp r0, #2
    beq .L0802AA36
    b .L0802AA72
.L0802AA32:
    movs r0, #1
    b .L0802AA74
.L0802AA36:
    add r0, sp, #0x14
    bl GetId__C7Article
    cmp r0, #0x31
    beq .L0802AA72
.L0802AA40:
    movs r0, #0
    b .L0802AA74
.L0802AA44:
    add r1, sp, #0x18
    b .L0802AA4E
.L0802AA48:
    add r1, sp, #0x20
    b .L0802AA4E
.L0802AA4C:
    add r1, sp, #0x28
.L0802AA4E:
    subs r0, r4, #7
    strh r0, [r1]
    subs r0, r7, #4
    strh r0, [r1, #2]
    adds r0, r4, #7
    strh r0, [r1, #4]
    adds r0, r7, #5
    strh r0, [r1, #6]
    adds r0, r5, #0
    bl func_080AC070
    lsls r0, r0, #0x18
    movs r1, #1
    cmp r0, #0
    beq .L0802AA6E
    movs r1, #2
.L0802AA6E:
    adds r0, r1, #0
    b .L0802AA74
.L0802AA72:
    movs r0, #2
.L0802AA74:
    add sp, #0x34
    pop {r3, r4, r5}
    mov r8, r3
    mov sb, r4
    mov sl, r5
    pop {r4, r5, r6, r7}
    pop {r1}
    bx r1

    thumb_func_start func_0802AA84
func_0802AA84: @ 0x0802AA84
    push {r4, r5, r6, r7, lr}
    mov r7, sb
    mov r6, r8
    push {r6, r7}
    sub sp, #0xc
    mov r8, r0
    movs r0, #0
    mov sb, r0
    mov r1, r8
    ldr r0, [r1, #0x34]
    ldr r1, .L0802AAD8 @ =0x00001C34
    adds r5, r0, r1
    adds r0, r5, #0
    bl IsEmpty__C9ToolStack
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802AACA
    mov r0, r8
    ldr r4, [r0, #0x34]
    ldr r1, .L0802AADC @ =0x00001BD8
    adds r4, r4, r1
    adds r0, r5, #0
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    add r1, sp, #8
    strb r0, [r1]
    movs r0, #0
    bl func_080246BC
    mov sb, r0
.L0802AACA:
    mov r1, r8
    ldr r0, [r1, #0x34]
    ldr r1, .L0802AAE0 @ =0x00001C38
    adds r7, r0, r1
    movs r6, #0
    b .L0802AB1E
    .align 2, 0
.L0802AAD8: .4byte 0x00001C34
.L0802AADC: .4byte 0x00001BD8
.L0802AAE0: .4byte 0x00001C38
.L0802AAE4:
    adds r0, r7, #0
    adds r1, r6, #0
    bl GetToolAt__C8RucksackUi
    adds r5, r0, #0
    bl IsEmpty__C9ToolStack
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802AB1C
    mov r0, r8
    ldr r4, [r0, #0x34]
    ldr r1, .L0802AB90 @ =0x00001BD8
    adds r4, r4, r1
    adds r0, r5, #0
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    mov r1, sp
    adds r1, #9
    strb r0, [r1]
    mov r0, sb
    bl func_080246BC
    mov sb, r0
.L0802AB1C:
    adds r6, #1
.L0802AB1E:
    adds r0, r7, #0
    bl GetSize__C8Rucksack
    cmp r6, r0
    blo .L0802AAE4
    mov r1, r8
    ldr r0, [r1, #0x34]
    movs r1, #0xe0
    lsls r1, r1, #2
    adds r7, r0, r1
    movs r6, #0
.L0802AB34:
    adds r0, r7, #0
    adds r1, r6, #0
    bl GetToolStackAt__C9ToolChestUi
    adds r5, r0, #0
    bl IsEmpty__C9ToolStack
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802AB6C
    mov r0, r8
    ldr r4, [r0, #0x34]
    ldr r1, .L0802AB90 @ =0x00001BD8
    adds r4, r4, r1
    adds r0, r5, #0
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    mov r1, sp
    adds r1, #0xa
    strb r0, [r1]
    mov r0, sb
    bl func_080246BC
    mov sb, r0
.L0802AB6C:
    adds r6, #1
    cmp r6, #0x3f
    bls .L0802AB34
    ldr r1, .L0802AB94 @ =gUnk_080F11B4
    mov r0, sp
    movs r2, #7
    bl memcpy
    mov r0, sp
    add r0, sb
    ldrb r0, [r0]
    add sp, #0xc
    pop {r3, r4}
    mov r8, r3
    mov sb, r4
    pop {r4, r5, r6, r7}
    pop {r1}
    bx r1
    .align 2, 0
.L0802AB90: .4byte 0x00001BD8
.L0802AB94: .4byte gUnk_080F11B4

    thumb_func_start func_0802AB98
func_0802AB98: @ 0x0802AB98
    push {r4, lr}
    adds r3, r0, #0
    adds r0, r1, #0
    cmp r2, #1
    beq .L0802ABB6
    cmp r2, #1
    bgt .L0802ABAC
    cmp r2, #0
    beq .L0802ABCC
    b .L0802AC1A
.L0802ABAC:
    cmp r2, #2
    beq .L0802ABE6
    cmp r2, #3
    beq .L0802ABFE
    b .L0802AC1A
.L0802ABB6:
    movs r2, #0xa
    ldrsh r1, [r0, r2]
    movs r4, #0xe
    ldrsh r2, [r0, r4]
    subs r0, r1, #4
    strh r0, [r3]
    adds r0, r2, #0
    subs r0, #0x14
    strh r0, [r3, #2]
    adds r1, #4
    b .L0802AC12
.L0802ABCC:
    movs r2, #0xa
    ldrsh r1, [r0, r2]
    movs r4, #0xe
    ldrsh r2, [r0, r4]
    subs r0, r1, #4
    strh r0, [r3]
    subs r0, r2, #6
    strh r0, [r3, #2]
    adds r1, #4
    strh r1, [r3, #4]
    adds r2, #0x12
    strh r2, [r3, #6]
    b .L0802AC24
.L0802ABE6:
    movs r2, #0xa
    ldrsh r1, [r0, r2]
    movs r4, #0xe
    ldrsh r2, [r0, r4]
    adds r0, r1, #0
    subs r0, #0x14
    strh r0, [r3]
    adds r0, r2, #0
    subs r0, #0xe
    strh r0, [r3, #2]
    adds r1, #4
    b .L0802AC12
.L0802ABFE:
    movs r2, #0xa
    ldrsh r1, [r0, r2]
    movs r4, #0xe
    ldrsh r2, [r0, r4]
    subs r0, r1, #4
    strh r0, [r3]
    adds r0, r2, #0
    subs r0, #0xe
    strh r0, [r3, #2]
    adds r1, #0x14
.L0802AC12:
    strh r1, [r3, #4]
    adds r2, #2
    strh r2, [r3, #6]
    b .L0802AC24
.L0802AC1A:
    movs r0, #0
    strh r0, [r3]
    strh r0, [r3, #2]
    strh r0, [r3, #4]
    strh r0, [r3, #6]
.L0802AC24:
    adds r0, r3, #0
    pop {r4}
    pop {r2}
    bx r2

    thumb_func_start func_0802AC2C
func_0802AC2C: @ 0x0802AC2C
    push {r4, r5, r6, r7, lr}
    mov r7, sl
    mov r6, sb
    mov r5, r8
    push {r5, r6, r7}
    sub sp, #0x38
    mov sb, r0
    adds r6, r2, #0
    adds r5, r1, #0
    ldr r0, [r0]
    str r0, [sp, #0x18]
    mov r1, sb
    ldrh r1, [r1, #4]
    str r1, [sp, #0x1c]
    mov r2, sb
    movs r3, #0xa
    ldrsh r2, [r2, r3]
    str r2, [sp, #0x20]
    mov r0, sb
    movs r1, #0xe
    ldrsh r0, [r0, r1]
    str r0, [sp, #0x24]
    mov r0, sb
    adds r0, #0x20
    ldrb r2, [r0]
    mov r0, sp
    mov r1, sb
    bl func_0802AB98
    ldr r0, [sp]
    ldr r1, [sp, #4]
    str r0, [sp, #8]
    str r1, [sp, #0xc]
    add r2, sp, #8
    mov r8, r2
    movs r3, #0x64
    str r3, [sp, #0x28]
    movs r0, #0
    str r0, [sp, #0x2c]
    movs r1, #0x2b
    mov sl, r1
    add r7, sp, #0x10
.L0802AC80:
    mov r2, sl
    str r2, [sp, #0x30]
    ldr r3, [sp, #0x18]
    ldr r0, [r3]
    ldr r2, [r0, #0x40]
    adds r0, r3, #0
    mov r1, sl
    bl _call_via_r2
    adds r4, r0, #0
    movs r0, #0
    add r1, sp, #0x34
    strb r0, [r1]
    cmp r4, #0
    beq .L0802AD20
    ldrh r0, [r4, #4]
    ldr r2, [sp, #0x1c]
    cmp r0, r2
    bne .L0802AD20
    ldr r1, [r4, #0x14]
    adds r0, r7, #0
    ldr r2, [r1, #0xc]
    adds r1, r4, #0
    bl _call_via_r2
    mov r3, r8
    movs r0, #0
    ldrsh r2, [r3, r0]
    movs r1, #4
    ldrsh r0, [r7, r1]
    subs r2, r2, r0
    movs r3, #0
    ldrsh r0, [r7, r3]
    mov r1, r8
    movs r3, #4
    ldrsh r1, [r1, r3]
    subs r0, r0, r1
    ands r2, r0
    mov r1, r8
    movs r3, #2
    ldrsh r0, [r1, r3]
    movs r3, #6
    ldrsh r1, [r7, r3]
    subs r0, r0, r1
    ands r2, r0
    movs r1, #2
    ldrsh r0, [r7, r1]
    mov r3, r8
    movs r1, #6
    ldrsh r3, [r3, r1]
    subs r0, r0, r3
    ands r2, r0
    cmp r2, #0
    bge .L0802AD20
    asrs r2, r5, #0x10
    cmp r2, #0
    beq .L0802AD1A
    cmp r2, #0
    ble .L0802AD08
    lsls r0, r6, #0x10
    asrs r0, r0, #0x10
    adds r0, r4, r0
    ldr r1, [r0]
    lsls r0, r2, #2
    adds r0, r0, r1
    subs r0, #4
    ldr r1, [r0]
    b .L0802AD0A
.L0802AD08:
    adds r1, r6, #0
.L0802AD0A:
    lsls r0, r5, #0x10
    asrs r0, r0, #0x10
    adds r0, r4, r0
    bl _call_via_r1
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802AD20
.L0802AD1A:
    movs r2, #1
    add r3, sp, #0x34
    strb r2, [r3]
.L0802AD20:
    add r0, sp, #0x34
    ldrb r0, [r0]
    cmp r0, #0
    beq .L0802AD5A
    movs r2, #0xa
    ldrsh r1, [r4, r2]
    ldr r3, [sp, #0x20]
    subs r1, r1, r3
    movs r2, #0xe
    ldrsh r0, [r4, r2]
    ldr r3, [sp, #0x24]
    subs r0, r0, r3
    adds r2, r1, #0
    muls r2, r1, r2
    adds r1, r2, #0
    adds r3, r0, #0
    muls r3, r0, r3
    adds r0, r3, #0
    adds r0, r1, r0
    adds r1, r0, #0
    ldr r2, [sp, #0x28]
    cmp r2, #0x64
    beq .L0802AD54
    ldr r3, [sp, #0x2c]
    cmp r3, r0
    ble .L0802AD5A
.L0802AD54:
    ldr r0, [sp, #0x30]
    str r0, [sp, #0x28]
    str r1, [sp, #0x2c]
.L0802AD5A:
    movs r1, #1
    add sl, r1
    mov r2, sl
    cmp r2, #0x45
    bls .L0802AC80
    mov r0, sb
    adds r0, #0xb2
    add r3, sp, #0x28
    ldrb r3, [r3]
    strb r3, [r0]
    add sp, #0x38
    pop {r3, r4, r5}
    mov r8, r3
    mov sb, r4
    mov sl, r5
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0
    .align 2, 0

    thumb_func_start func_0802AD80
func_0802AD80: @ 0x0802AD80
    push {r4, r5, lr}
    sub sp, #0x30
    adds r5, r0, #0
    ldr r0, [r5, #0x38]
    bl func_0800E958
    adds r4, r0, #0
    ldr r0, [r5, #0x38]
    bl func_0800ED8C
    ldr r0, [r5, #0x38]
    bl func_0800E958
    cmp r4, r0
    bne .L0802ADA0
    b .L0802AE98
.L0802ADA0:
    ldr r0, [r5, #0x38]
    bl func_0800E958
    adds r1, r0, #0
    ldr r0, .L0802ADC0 @ =0x000F4240
    cmp r1, r0
    beq .L0802AE28
    cmp r1, r0
    bhi .L0802ADCC
    ldr r0, .L0802ADC4 @ =0x00002710
    cmp r1, r0
    beq .L0802ADF4
    ldr r0, .L0802ADC8 @ =0x000186A0
    cmp r1, r0
    beq .L0802AE10
    b .L0802AE98
    .align 2, 0
.L0802ADC0: .4byte 0x000F4240
.L0802ADC4: .4byte 0x00002710
.L0802ADC8: .4byte 0x000186A0
.L0802ADCC:
    ldr r0, .L0802ADE0 @ =0x05F5E100
    cmp r1, r0
    beq .L0802AE58
    cmp r1, r0
    bhi .L0802ADE8
    ldr r0, .L0802ADE4 @ =0x00989680
    cmp r1, r0
    beq .L0802AE3E
    b .L0802AE98
    .align 2, 0
.L0802ADE0: .4byte 0x05F5E100
.L0802ADE4: .4byte 0x00989680
.L0802ADE8:
    ldr r0, .L0802ADF0 @ =0x3B9ACA00
    cmp r1, r0
    beq .L0802AE7C
    b .L0802AE98
    .align 2, 0
.L0802ADF0: .4byte 0x3B9ACA00
.L0802ADF4:
    ldr r0, [r5]
    ldr r1, [r0]
    movs r2, #0xa0
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r2, .L0802AE0C @ =0x00000336
    movs r3, #0
    str r2, [sp]
    str r3, [sp, #4]
    ldr r3, [r1]
    mov r1, sp
    b .L0802AE6E
    .align 2, 0
.L0802AE0C: .4byte 0x00000336
.L0802AE10:
    ldr r0, [r5]
    ldr r2, [r0]
    movs r1, #0xa0
    lsls r1, r1, #1
    adds r2, r2, r1
    add r1, sp, #8
    ldr r3, .L0802AE24 @ =0x00000337
    movs r4, #0
    str r3, [sp, #8]
    b .L0802AE6A
    .align 2, 0
.L0802AE24: .4byte 0x00000337
.L0802AE28:
    ldr r0, [r5]
    ldr r2, [r0]
    movs r1, #0xa0
    lsls r1, r1, #1
    adds r2, r2, r1
    add r1, sp, #0x10
    movs r3, #0xce
    lsls r3, r3, #2
    movs r4, #0
    str r3, [sp, #0x10]
    b .L0802AE6A
.L0802AE3E:
    ldr r0, [r5]
    ldr r2, [r0]
    movs r1, #0xa0
    lsls r1, r1, #1
    adds r2, r2, r1
    add r1, sp, #0x18
    ldr r3, .L0802AE54 @ =0x00000339
    movs r4, #0
    str r3, [sp, #0x18]
    b .L0802AE6A
    .align 2, 0
.L0802AE54: .4byte 0x00000339
.L0802AE58:
    ldr r0, [r5]
    ldr r2, [r0]
    movs r1, #0xa0
    lsls r1, r1, #1
    adds r2, r2, r1
    add r1, sp, #0x20
    ldr r3, .L0802AE78 @ =0x0000033A
    movs r4, #0
    str r3, [sp, #0x20]
.L0802AE6A:
    str r4, [r1, #4]
    ldr r3, [r2]
.L0802AE6E:
    movs r2, #1
    bl _call_via_r3
    b .L0802AE98
    .align 2, 0
.L0802AE78: .4byte 0x0000033A
.L0802AE7C:
    ldr r0, [r5]
    ldr r2, [r0]
    movs r1, #0xa0
    lsls r1, r1, #1
    adds r2, r2, r1
    add r1, sp, #0x28
    ldr r3, .L0802AEA0 @ =0x0000033B
    movs r4, #0
    str r3, [sp, #0x28]
    str r4, [r1, #4]
    ldr r3, [r2]
    movs r2, #1
    bl _call_via_r3
.L0802AE98:
    add sp, #0x30
    pop {r4, r5}
    pop {r0}
    bx r0
    .align 2, 0
.L0802AEA0: .4byte 0x0000033B

    thumb_func_start func_0802AEA4
func_0802AEA4: @ 0x0802AEA4
    push {r4, r5, r6, lr}
    sub sp, #0x1c
    adds r4, r0, #0
    ldr r1, [r4]
    ldrh r6, [r4, #4]
    ldr r2, [r1]
    mov r0, sp
    ldr r3, [r2, #0x34]
    adds r2, r6, #0
    bl _call_via_r3
    mov r5, sp
    ldr r1, [r4, #0x14]
    add r0, sp, #0xc
    ldr r2, [r1, #0xc]
    adds r1, r4, #0
    bl _call_via_r2
    ldr r0, [sp, #0xc]
    ldr r1, [sp, #0x10]
    str r0, [sp, #0x14]
    str r1, [sp, #0x18]
    add r4, sp, #0x14
    mov r0, sp
    adds r1, r4, #0
    bl func_080AC070
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802AEE2
    b .L0802B03E
.L0802AEE2:
    adds r1, r6, #0
    subs r1, #0x34
    ldr r0, .L0802AF2C @ =0x000001FF
    cmp r1, r0
    bls .L0802AEEE
    b .L0802B048
.L0802AEEE:
    movs r1, #0
    ldrsh r0, [r4, r1]
    cmp r0, #0
    bge .L0802AEF8
    adds r0, #7
.L0802AEF8:
    asrs r6, r0, #3
    movs r1, #2
    ldrsh r0, [r4, r1]
    cmp r0, #0
    bge .L0802AF04
    adds r0, #7
.L0802AF04:
    asrs r0, r0, #3
    ldr r2, [sp]
    ldrh r3, [r5, #8]
    cmp r2, #0
    beq .L0802AF30
    cmp r6, r3
    bhi .L0802AF30
    ldrh r1, [r5, #0xa]
    cmp r0, r1
    bhi .L0802AF30
    ldr r1, [sp, #4]
    muls r0, r3, r0
    adds r0, r0, r6
    cmp r1, #0
    beq .L0802AF26
    adds r0, r1, r0
    ldrb r0, [r0]
.L0802AF26:
    lsls r0, r0, #2
    adds r0, r2, r0
    b .L0802AF32
    .align 2, 0
.L0802AF2C: .4byte 0x000001FF
.L0802AF30:
    movs r0, #0
.L0802AF32:
    cmp r0, #0
    beq .L0802AF42
    ldr r0, [r0]
    ldr r1, .L0802AF80 @ =0x0001FFFC
    ands r0, r1
    cmp r0, #0
    beq .L0802AF42
    b .L0802B03E
.L0802AF42:
    movs r1, #0
    ldrsh r0, [r4, r1]
    cmp r0, #0
    bge .L0802AF4C
    adds r0, #7
.L0802AF4C:
    asrs r6, r0, #3
    movs r0, #6
    ldrsh r1, [r4, r0]
    subs r0, r1, #1
    cmp r0, #0
    bge .L0802AF5A
    adds r0, r1, #6
.L0802AF5A:
    asrs r0, r0, #3
    ldr r2, [r5]
    cmp r2, #0
    beq .L0802AF84
    cmp r6, r3
    bhi .L0802AF84
    ldrh r1, [r5, #0xa]
    cmp r0, r1
    bhi .L0802AF84
    ldr r1, [r5, #4]
    muls r0, r3, r0
    adds r0, r0, r6
    cmp r1, #0
    beq .L0802AF7A
    adds r0, r1, r0
    ldrb r0, [r0]
.L0802AF7A:
    lsls r0, r0, #2
    adds r0, r2, r0
    b .L0802AF86
    .align 2, 0
.L0802AF80: .4byte 0x0001FFFC
.L0802AF84:
    movs r0, #0
.L0802AF86:
    cmp r0, #0
    beq .L0802AF94
    ldr r0, [r0]
    ldr r1, .L0802AFD0 @ =0x0001FFFC
    ands r0, r1
    cmp r0, #0
    bne .L0802B03E
.L0802AF94:
    movs r0, #4
    ldrsh r1, [r4, r0]
    subs r0, r1, #1
    cmp r0, #0
    bge .L0802AFA0
    adds r0, r1, #6
.L0802AFA0:
    asrs r6, r0, #3
    movs r1, #2
    ldrsh r0, [r4, r1]
    cmp r0, #0
    bge .L0802AFAC
    adds r0, #7
.L0802AFAC:
    asrs r0, r0, #3
    cmp r2, #0
    beq .L0802AFD4
    cmp r6, r3
    bhi .L0802AFD4
    ldrh r1, [r5, #0xa]
    cmp r0, r1
    bhi .L0802AFD4
    ldr r1, [r5, #4]
    muls r0, r3, r0
    adds r0, r0, r6
    cmp r1, #0
    beq .L0802AFCA
    adds r0, r1, r0
    ldrb r0, [r0]
.L0802AFCA:
    lsls r0, r0, #2
    adds r0, r2, r0
    b .L0802AFD6
    .align 2, 0
.L0802AFD0: .4byte 0x0001FFFC
.L0802AFD4:
    movs r0, #0
.L0802AFD6:
    cmp r0, #0
    beq .L0802AFE4
    ldr r0, [r0]
    ldr r1, .L0802B024 @ =0x0001FFFC
    ands r0, r1
    cmp r0, #0
    bne .L0802B03E
.L0802AFE4:
    movs r0, #4
    ldrsh r1, [r4, r0]
    subs r0, r1, #1
    cmp r0, #0
    bge .L0802AFF0
    adds r0, r1, #6
.L0802AFF0:
    asrs r6, r0, #3
    movs r0, #6
    ldrsh r1, [r4, r0]
    subs r0, r1, #1
    cmp r0, #0
    bge .L0802AFFE
    adds r0, r1, #6
.L0802AFFE:
    asrs r0, r0, #3
    adds r4, r2, #0
    cmp r4, #0
    beq .L0802B02E
    cmp r6, r3
    bhi .L0802B02E
    ldrh r1, [r5, #0xa]
    cmp r0, r1
    bhi .L0802B02E
    ldr r1, [r5, #4]
    muls r0, r3, r0
    adds r0, r0, r6
    cmp r1, #0
    beq .L0802B028
    adds r0, r1, r0
    ldrb r0, [r0]
    lsls r0, r0, #2
    adds r0, r4, r0
    b .L0802B030
    .align 2, 0
.L0802B024: .4byte 0x0001FFFC
.L0802B028:
    lsls r0, r0, #2
    adds r0, r2, r0
    b .L0802B030
.L0802B02E:
    movs r0, #0
.L0802B030:
    cmp r0, #0
    beq .L0802B048
    ldr r0, [r0]
    ldr r1, .L0802B044 @ =0x0001FFFC
    ands r0, r1
    cmp r0, #0
    beq .L0802B048
.L0802B03E:
    movs r0, #1
    b .L0802B04A
    .align 2, 0
.L0802B044: .4byte 0x0001FFFC
.L0802B048:
    movs r0, #0
.L0802B04A:
    add sp, #0x1c
    pop {r4, r5, r6}
    pop {r1}
    bx r1
    .align 2, 0

    thumb_func_start func_0802B054
func_0802B054: @ 0x0802B054
    push {r4, r5, r6, r7, lr}
    sub sp, #0x28
    adds r5, r0, #0
    ldr r0, [r5, #0x34]
    ldr r1, .L0802B130 @ =0x00002E58
    adds r0, r0, r1
    bl func_0809DFAC
    lsls r0, r0, #0x18
    lsrs r0, r0, #0x18
    cmp r0, #9
    bne .L0802B0BA
    ldr r0, [r5, #0x34]
    ldr r1, .L0802B134 @ =0x00002189
    adds r0, r0, r1
    ldrb r0, [r0]
    movs r4, #0x30
    ands r4, r0
    cmp r4, #0
    bne .L0802B0BA
    ldr r0, [r5, #0x38]
    adds r0, #0x54
    bl func_0800F204
    cmp r0, #1
    bne .L0802B0BA
    ldr r0, [r5, #0x38]
    adds r0, #0x54
    bl func_0800F258
    mov r1, sp
    strb r0, [r1]
    mov r0, sp
    bl GetId__C7Article
    cmp r0, #0x25
    bne .L0802B0BA
    ldr r0, [r5]
    ldr r3, [r0]
    movs r1, #0xa0
    lsls r1, r1, #1
    adds r3, r3, r1
    add r1, sp, #4
    movs r2, #0xcc
    lsls r2, r2, #2
    str r2, [sp, #4]
    str r4, [r1, #4]
    ldr r3, [r3]
    movs r2, #0
    bl _call_via_r3
.L0802B0BA:
    ldr r0, [r5, #0x34]
    ldr r1, .L0802B130 @ =0x00002E58
    adds r0, r0, r1
    bl func_0809E02C
    lsls r0, r0, #0x18
    lsrs r0, r0, #0x18
    cmp r0, #9
    bne .L0802B118
    ldr r0, [r5, #0x34]
    ldr r1, .L0802B138 @ =0x0000218B
    adds r0, r0, r1
    ldrb r0, [r0]
    movs r4, #0x18
    ands r4, r0
    cmp r4, #0
    bne .L0802B118
    ldr r0, [r5, #0x38]
    adds r0, #0x54
    bl func_0800F204
    cmp r0, #1
    bne .L0802B118
    ldr r0, [r5, #0x38]
    adds r0, #0x54
    bl func_0800F258
    adds r1, r0, #0
    add r0, sp, #0xc
    strb r1, [r0]
    bl GetId__C7Article
    cmp r0, #0x26
    bne .L0802B118
    ldr r0, [r5]
    ldr r3, [r0]
    movs r1, #0xa0
    lsls r1, r1, #1
    adds r3, r3, r1
    add r1, sp, #0x10
    ldr r2, .L0802B13C @ =0x00000332
    str r2, [sp, #0x10]
    str r4, [r1, #4]
    ldr r3, [r3]
    movs r2, #0
    bl _call_via_r3
.L0802B118:
    ldr r0, [r5, #0x34]
    ldr r1, .L0802B140 @ =0x00002194
    adds r0, r0, r1
    ldrb r1, [r0]
    movs r0, #0xc0
    ands r0, r1
    cmp r0, #0
    bne .L0802B1BC
    movs r7, #1
    movs r6, #0
    ldr r4, .L0802B144 @ =gUnk_080F11BC
    b .L0802B14C
    .align 2, 0
.L0802B130: .4byte 0x00002E58
.L0802B134: .4byte 0x00002189
.L0802B138: .4byte 0x0000218B
.L0802B13C: .4byte 0x00000332
.L0802B140: .4byte 0x00002194
.L0802B144: .4byte gUnk_080F11BC
.L0802B148:
    adds r4, #4
    adds r6, #1
.L0802B14C:
    cmp r6, #8
    bhi .L0802B162
    ldr r0, [r5, #0x34]
    adds r0, #0x54
    ldr r1, [r4]
    bl IsDisplayEnabled__C11ShippingBini
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802B148
    movs r7, #0
.L0802B162:
    cmp r7, #0
    beq .L0802B1BC
    ldr r0, [r5, #0x34]
    ldr r4, .L0802B1D4 @ =0x00002E58
    adds r0, r0, r4
    bl func_0809E184
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802B1BC
    ldr r0, [r5, #0x34]
    adds r0, r0, r4
    bl func_0809E194
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802B1BC
    ldr r0, [r5, #0x34]
    adds r0, r0, r4
    bl func_0809DFAC
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802B1BC
    ldr r0, [r5, #0x34]
    adds r0, r0, r4
    bl func_0809E1A4
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802B1BC
    ldr r0, [r5]
    ldr r2, [r0]
    movs r1, #0xa0
    lsls r1, r1, #1
    adds r2, r2, r1
    add r1, sp, #0x18
    ldr r3, .L0802B1D8 @ =0x0000034D
    movs r4, #0
    str r3, [sp, #0x18]
    str r4, [r1, #4]
    ldr r3, [r2]
    movs r2, #0
    bl _call_via_r3
.L0802B1BC:
    ldr r0, [r5, #0x34]
    ldr r1, .L0802B1DC @ =0x00002195
    adds r0, r0, r1
    ldrb r1, [r0]
    movs r0, #3
    ands r0, r1
    cmp r0, #0
    bne .L0802B250
    movs r7, #1
    movs r6, #0
    ldr r4, .L0802B1E0 @ =gUnk_080F11E0
    b .L0802B1E8
    .align 2, 0
.L0802B1D4: .4byte 0x00002E58
.L0802B1D8: .4byte 0x0000034D
.L0802B1DC: .4byte 0x00002195
.L0802B1E0: .4byte gUnk_080F11E0
.L0802B1E4:
    adds r4, #4
    adds r6, #1
.L0802B1E8:
    cmp r6, #0xd
    bhi .L0802B1FE
    ldr r0, [r5, #0x34]
    adds r0, #0x54
    ldr r1, [r4]
    bl IsDisplayEnabled__C11ShippingBini
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802B1E4
    movs r7, #0
.L0802B1FE:
    cmp r7, #0
    beq .L0802B250
    ldr r0, [r5, #0x34]
    movs r1, #0xd2
    lsls r1, r1, #6
    adds r0, r0, r1
    bl func_0809C32C
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802B250
    ldr r0, [r5, #0x34]
    ldr r1, .L0802B258 @ =0x00002E58
    adds r0, r0, r1
    bl func_0809E174
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802B250
    ldr r0, [r5, #0x34]
    ldr r1, .L0802B258 @ =0x00002E58
    adds r0, r0, r1
    bl func_0809E02C
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802B250
    ldr r0, [r5]
    ldr r2, [r0]
    movs r1, #0xa0
    lsls r1, r1, #1
    adds r2, r2, r1
    add r1, sp, #0x20
    ldr r3, .L0802B25C @ =0x0000034E
    movs r4, #0
    str r3, [sp, #0x20]
    str r4, [r1, #4]
    ldr r3, [r2]
    movs r2, #0
    bl _call_via_r3
.L0802B250:
    add sp, #0x28
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0
    .align 2, 0
.L0802B258: .4byte 0x00002E58
.L0802B25C: .4byte 0x0000034E

    thumb_func_start func_0802B260
func_0802B260: @ 0x0802B260
    push {r4, r5, r6, r7, lr}
    mov r7, sl
    mov r6, sb
    mov r5, r8
    push {r5, r6, r7}
    sub sp, #0x6c
    mov sb, r0
    mov r8, r1
    ldr r3, .L0802B3A4 @ =gUnk_080F1218
    movs r0, #3
    str r0, [sp]
    movs r4, #1
    str r4, [sp, #4]
    movs r5, #0
    str r5, [sp, #8]
    str r4, [sp, #0xc]
    add r0, sp, #0x10
    strb r4, [r0]
    mov r0, sb
    movs r2, #6
    bl func_08032560
    ldr r0, .L0802B3A8 @ =vtable_unk_080E6708
    mov r1, sb
    str r0, [r1, #4]
    mov r2, r8
    ldr r0, [r2]
    ldr r1, [r0]
    ldr r1, [r1, #0x64]
    bl _call_via_r1
    adds r1, r0, #0
    mov r0, r8
    ldr r3, [r0]
    mov r0, sb
    adds r0, #0x8c
    movs r2, #2
    str r2, [sp]
    str r4, [sp, #4]
    add r4, sp, #8
    strb r5, [r4]
    movs r2, #0
    bl func_080A4A00
    mov r1, sb
    adds r1, #0xcc
    str r5, [r1]
    ldr r0, .L0802B3AC @ =0x00000281
    add r0, sb
    strb r5, [r0]
    movs r2, #0
    str r2, [sp, #0x5c]
    str r4, [sp, #0x68]
    mov r3, sp
    adds r3, #0x18
    str r3, [sp, #0x60]
    mov r4, sp
    adds r4, #0x58
    str r4, [sp, #0x64]
    add r0, sp, #0x14
    mov sl, r0
    movs r7, #0
    adds r6, r1, #0
.L0802B2DE:
    mov r1, r8
    ldr r0, [r1]
    ldr r1, [r0]
    ldr r1, [r1, #0x68]
    bl _call_via_r1
    adds r1, r0, #0
    mov r2, r8
    ldr r3, [r2]
    mov r4, sl
    strb r7, [r4]
    str r7, [sp]
    movs r0, #0x12
    str r0, [sp, #4]
    ldr r0, [sp, #0x68]
    strb r7, [r0]
    add r0, sp, #0x18
    movs r2, #0
    bl func_080A4A00
    ldr r1, [sp, #0x64]
    strh r7, [r1]
    strh r7, [r1, #2]
    ldr r1, [r6]
    cmp r1, #5
    bhi .L0802B374
    lsls r0, r1, #3
    adds r0, r0, r1
    lsls r0, r0, #3
    adds r0, #4
    adds r5, r6, r0
    cmp r5, #0
    beq .L0802B36E
    ldrb r0, [r4]
    strb r0, [r5]
    adds r4, r5, #4
    adds r0, r4, #0
    add r1, sp, #0x18
    bl func_080DC840
    ldr r0, .L0802B3B0 @ =vtable_unk_080E681C
    str r0, [r4, #0x24]
    adds r1, r5, #0
    adds r1, #0x2c
    add r0, sp, #0x40
    ldm r0!, {r2, r3, r4}
    stm r1!, {r2, r3, r4}
    ldm r0!, {r2, r3}
    stm r1!, {r2, r3}
    add r0, sp, #0x54
    ldrb r1, [r0]
    adds r0, r5, #0
    adds r0, #0x40
    strb r1, [r0]
    mov r0, sp
    adds r0, #0x55
    ldrb r0, [r0]
    adds r1, r5, #0
    adds r1, #0x41
    strb r0, [r1]
    mov r0, sp
    adds r0, #0x56
    ldrb r0, [r0]
    adds r1, #1
    strb r0, [r1]
    mov r0, sp
    adds r0, #0x57
    ldrb r0, [r0]
    adds r1, #1
    strb r0, [r1]
    ldr r0, [sp, #0x58]
    str r0, [r5, #0x44]
.L0802B36E:
    ldr r0, [r6]
    adds r0, #1
    str r0, [r6]
.L0802B374:
    ldr r0, [sp, #0x60]
    movs r1, #2
    bl func_080A47B4
    ldr r4, [sp, #0x5c]
    adds r4, #1
    str r4, [sp, #0x5c]
    cmp r4, #5
    bls .L0802B2DE
    movs r1, #0xa0
    lsls r1, r1, #2
    add r1, sb
    movs r0, #0
    strb r0, [r1]
    mov r0, sb
    add sp, #0x6c
    pop {r3, r4, r5}
    mov r8, r3
    mov sb, r4
    mov sl, r5
    pop {r4, r5, r6, r7}
    pop {r1}
    bx r1
    .align 2, 0
.L0802B3A4: .4byte gUnk_080F1218
.L0802B3A8: .4byte vtable_unk_080E6708
.L0802B3AC: .4byte 0x00000281
.L0802B3B0: .4byte vtable_unk_080E681C

    thumb_func_start func_0802B3B4
func_0802B3B4: @ 0x0802B3B4
    push {r4, r5, r6, r7, lr}
    sub sp, #4
    adds r5, r0, #0
    bl func_0803260C
    ldr r4, [r5]
    adds r0, r5, #0
    adds r0, #0x44
    ldrb r3, [r0]
    adds r2, r4, #0
    adds r2, #0x7c
    ldrh r1, [r2]
    ldr r0, .L0802B40C @ =0x0000FFFF
    cmp r1, r0
    beq .L0802B3DA
    cmp r3, #0
    bne .L0802B3DA
    orrs r0, r1
    strh r0, [r2]
.L0802B3DA:
    adds r0, r4, #0
    adds r0, #0x88
    movs r1, #0
    ldrb r0, [r0]
    cmp r0, #1
    bne .L0802B3E8
    movs r1, #1
.L0802B3E8:
    cmp r1, #0
    beq .L0802B414
    adds r1, r5, #0
    adds r1, #0xcb
    ldrb r0, [r1]
    cmp r0, #0
    bne .L0802B410
    adds r0, r5, #0
    adds r0, #0xb4
    bl func_0805E8F0
    lsls r0, r0, #0x1e
    cmp r0, #0
    bge .L0802B414
    adds r1, r5, #0
    adds r1, #0xc8
    movs r0, #1
    b .L0802B412
    .align 2, 0
.L0802B40C: .4byte 0x0000FFFF
.L0802B410:
    movs r0, #0
.L0802B412:
    strb r0, [r1]
.L0802B414:
    movs r7, #0
    movs r2, #0
    adds r4, r5, #0
    adds r4, #0xfc
    ldr r0, .L0802B448 @ =0x00000113
    adds r6, r5, r0
    adds r5, #0xd0
.L0802B422:
    ldrb r0, [r5]
    cmp r0, #0
    beq .L0802B458
    ldrb r0, [r4, #0x17]
    cmp r0, #0
    bne .L0802B44C
    adds r0, r4, #0
    str r2, [sp]
    bl func_0805E8F0
    adds r1, r0, #0
    lsls r0, r1, #0x1e
    ldr r2, [sp]
    cmp r0, #0
    bge .L0802B444
    movs r0, #1
    strb r0, [r4, #0x14]
.L0802B444:
    adds r0, r1, #0
    b .L0802B450
    .align 2, 0
.L0802B448: .4byte 0x00000113
.L0802B44C:
    strb r2, [r6]
    movs r0, #2
.L0802B450:
    lsls r0, r0, #0x1d
    cmp r0, #0
    bge .L0802B458
    strb r2, [r5]
.L0802B458:
    adds r4, #0x48
    adds r6, #0x48
    adds r5, #0x48
    adds r7, #1
    cmp r7, #5
    bls .L0802B422
    add sp, #4
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0

    thumb_func_start func_0802B46C
func_0802B46C: @ 0x0802B46C
    push {r4, r5, r6, r7, lr}
    mov r7, r8
    push {r7}
    sub sp, #4
    mov r8, r0
    ldr r1, [r0]
    movs r0, #0xa0
    lsls r0, r0, #2
    add r0, r8
    ldrb r7, [r0]
    mov r4, r8
    adds r4, #0xcc
    lsls r0, r7, #3
    adds r0, r0, r7
    lsls r0, r0, #3
    adds r0, #4
    movs r3, #0xa
    ldrsh r2, [r1, r3]
    movs r5, #0xe
    ldrsh r3, [r1, r5]
    adds r1, #0x20
    ldrb r6, [r1]
    movs r1, #1
    adds r5, r7, #0
    ands r5, r1
    adds r4, r4, r0
    strb r1, [r4]
    mov r1, sp
    mov r0, sp
    strh r2, [r0]
    strh r3, [r1, #2]
    ldr r0, [sp]
    str r0, [r4, #0x44]
    cmp r6, #1
    beq .L0802B4C6
    cmp r6, #1
    ble .L0802B4BE
    cmp r6, #2
    beq .L0802B4D0
    cmp r6, #3
    beq .L0802B4DC
.L0802B4BE:
    movs r0, #0xca
    lsls r0, r0, #3
    adds r1, r5, r0
    b .L0802B4E0
.L0802B4C6:
    ldr r3, .L0802B4CC @ =0x00000652
    b .L0802B4DE
    .align 2, 0
.L0802B4CC: .4byte 0x00000652
.L0802B4D0:
    ldr r0, .L0802B4D8 @ =0x00000654
    adds r1, r5, r0
    b .L0802B4E0
    .align 2, 0
.L0802B4D8: .4byte 0x00000654
.L0802B4DC:
    ldr r3, .L0802B518 @ =0x00000656
.L0802B4DE:
    adds r1, r5, r3
.L0802B4E0:
    adds r0, r4, #0
    adds r0, #0x2c
    bl func_0805E860
    adds r0, r4, #0
    adds r0, #0x40
    movs r2, #0
    movs r1, #1
    strb r1, [r0]
    adds r0, #2
    strb r2, [r0]
    adds r0, #1
    strb r1, [r0]
    adds r0, r7, #1
    movs r1, #0
    cmp r0, #5
    bhi .L0802B504
    adds r1, r0, #0
.L0802B504:
    movs r0, #0xa0
    lsls r0, r0, #2
    add r0, r8
    strb r1, [r0]
    add sp, #4
    pop {r3}
    mov r8, r3
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0
    .align 2, 0
.L0802B518: .4byte 0x00000656
.L0802B51C:
    .byte 0xF0, 0xB5, 0x57, 0x46
    .byte 0x4E, 0x46, 0x45, 0x46, 0xE0, 0xB4, 0x9E, 0xB0, 0x13, 0x90, 0x14, 0x91, 0x00, 0x68, 0x15, 0x90
    .byte 0x81, 0x88, 0x89, 0x46, 0x02, 0x68, 0x92, 0x46, 0x13, 0x98, 0x44, 0x30, 0x00, 0x78, 0x16, 0x90
    .byte 0x15, 0x9B, 0x9E, 0x68, 0x31, 0x14, 0xD8, 0x68, 0x07, 0x1C, 0x3A, 0x14, 0x0D, 0xAC, 0x14, 0x9D
    .byte 0x28, 0x68, 0xAB, 0x68, 0x0D, 0x90, 0x63, 0x60, 0x48, 0x46, 0x07, 0xF0, 0xD1, 0xF9, 0x80, 0x46
    .byte 0x13, 0x98, 0x8B, 0x30, 0x41, 0x46, 0x01, 0x70, 0x15, 0x9D, 0x88, 0x35, 0x00, 0x21, 0x28, 0x78
    .byte 0x01, 0x28, 0x00, 0xD1, 0x01, 0x21, 0x00, 0x29, 0x00, 0xD1, 0xA2, 0xE0, 0x15, 0x98, 0xA4, 0x30
    .byte 0x00, 0x78, 0x00, 0x28, 0x00, 0xD1, 0x9C, 0xE0, 0x68, 0x68, 0x30, 0x18, 0x00, 0x14, 0x14, 0x9A
    .byte 0x0C, 0x23, 0xD1, 0x5E, 0x40, 0x1A, 0x17, 0x90, 0xA8, 0x68, 0x3C, 0x18, 0x21, 0x14, 0x0E, 0x27
    .byte 0xD0, 0x5F, 0x0E, 0x1A, 0x80, 0x20, 0x00, 0x02, 0x40, 0x1A, 0x18, 0x90, 0x00, 0x21, 0x68, 0x78
    .byte 0x01, 0x28, 0x00, 0xD1, 0x01, 0x21, 0x00, 0x29, 0x0D, 0xD0, 0x51, 0x46, 0x08, 0x68, 0x81, 0x6D
    .byte 0x50, 0x46, 0xA8, 0xF0, 0xA5, 0xF9, 0x14, 0x9A, 0x91, 0x68, 0x00, 0x22, 0x00, 0x92, 0x17, 0x9A
    .byte 0x33, 0x1C, 0x0F, 0xF0, 0x41, 0xFC, 0x68, 0x69, 0x21, 0x1A, 0x09, 0x14, 0x14, 0x9B, 0x0E, 0x24
    .byte 0x1A, 0x5F, 0x89, 0x1A, 0x1D, 0x91, 0x02, 0x21, 0x00, 0x14, 0x0F, 0x28, 0x06, 0xDD, 0x4D, 0x46
    .byte 0x08, 0x2D, 0x02, 0xDC, 0x47, 0x46, 0x01, 0x2F, 0x00, 0xD1, 0x01, 0x21, 0x13, 0x9D, 0x8C, 0x35
    .byte 0x0D, 0xA8, 0x80, 0x46, 0x08, 0x1C, 0x84, 0x00, 0x04, 0x43, 0x01, 0x01, 0x0C, 0x43, 0x80, 0x01
    .byte 0x04, 0x43, 0x13, 0x98, 0xB4, 0x30, 0x01, 0x68, 0x0B, 0x68, 0x82, 0x89, 0x13, 0x98, 0xB8, 0x30
    .byte 0x92, 0x00, 0x00, 0x68, 0x80, 0x18, 0x02, 0x88, 0x05, 0xA8, 0x1B, 0x69, 0xA8, 0xF0, 0x74, 0xF9
    .byte 0x05, 0xAE, 0x0E, 0x9B, 0x2A, 0x68, 0x13, 0x98, 0x90, 0x30, 0x41, 0x68, 0x48, 0x42, 0x08, 0x43
    .byte 0x00, 0x28, 0x13, 0xDA, 0xA8, 0x89, 0x13, 0x99, 0x9C, 0x31, 0x18, 0x9F, 0x00, 0x97, 0x01, 0x96
    .byte 0x02, 0x92, 0x03, 0x90, 0x04, 0x91, 0x18, 0x1C, 0x17, 0x99, 0x1D, 0x9A, 0x23, 0x1C, 0x02, 0x4C
    .byte 0xA8, 0xF0, 0x5C, 0xF9, 0x03, 0xE0, 0x00, 0x00, 0xDC, 0x04, 0x00, 0x03, 0x00, 0x20, 0x00, 0x28
    .byte 0x27, 0xD0, 0x28, 0x1C, 0x3C, 0x30, 0x01, 0x78, 0x07, 0x1C, 0x00, 0x29, 0x21, 0xD0, 0x40, 0x46
    .byte 0x01, 0x68, 0x32, 0x1C, 0x08, 0x32, 0x28, 0x1C, 0x79, 0xF0, 0xC0, 0xF8, 0x28, 0x1C, 0x3D, 0x30
    .byte 0x00, 0x78, 0x00, 0x28, 0x06, 0xD0, 0x31, 0x1C, 0x10, 0x31, 0x28, 0x1C, 0x01, 0x22, 0x79, 0xF0
    .byte 0x51, 0xF9, 0x0C, 0xE0, 0x2C, 0x1C, 0x3E, 0x34, 0x20, 0x78, 0x00, 0x28, 0x07, 0xD1, 0x31, 0x1C
    .byte 0x10, 0x31, 0x28, 0x1C, 0x01, 0x22, 0x79, 0xF0, 0x45, 0xF9, 0x01, 0x20, 0x20, 0x70, 0x00, 0x20
    .byte 0x38, 0x70, 0x00, 0x21, 0x19, 0x91, 0x6A, 0x46, 0x34, 0x32, 0x1A, 0x92, 0x13, 0x9B, 0xE4, 0x33
    .byte 0x1B, 0x93, 0x13, 0x9D, 0xD8, 0x35, 0x13, 0x9C, 0xD4, 0x34, 0xA0, 0x46, 0x13, 0x9F, 0xD0, 0x37
    .byte 0x1C, 0x97, 0x14, 0x98, 0x0C, 0x22, 0x81, 0x5E, 0x0E, 0x23, 0xC2, 0x5E, 0x1C, 0x9C, 0x20, 0x78
    .byte 0x00, 0x28, 0x5B, 0xD0, 0x3C, 0x27, 0xE8, 0x5F, 0x40, 0x1A, 0x81, 0x46, 0x3E, 0x21, 0x68, 0x5E
    .byte 0x84, 0x1A, 0x46, 0x46, 0x69, 0x6A, 0x0B, 0x68, 0x2A, 0x8E, 0x92, 0x00, 0xA8, 0x6A, 0x80, 0x18
    .byte 0x02, 0x88, 0x05, 0xA8, 0x1B, 0x69, 0xA8, 0xF0, 0xFF, 0xF8, 0x05, 0xAF, 0x0E, 0x9B, 0x32, 0x68
    .byte 0x69, 0x68, 0x48, 0x42, 0x08, 0x43, 0x00, 0x28, 0x14, 0xDA, 0x28, 0x89, 0x80, 0x21, 0x09, 0x02
    .byte 0x00, 0x91, 0x39, 0x1C, 0x01, 0x91, 0x02, 0x92, 0x03, 0x90, 0x1B, 0x9A, 0x04, 0x92, 0x18, 0x1C
    .byte 0x49, 0x46, 0x22, 0x1C, 0xAA, 0x23, 0x02, 0x4C, 0xA8, 0xF0, 0xE8, 0xF8, 0x03, 0xE0, 0x00, 0x00
    .byte 0xDC, 0x04, 0x00, 0x03, 0x00, 0x20, 0x00, 0x28, 0x28, 0xD0, 0x30, 0x1C, 0x3C, 0x30, 0x01, 0x78
    .byte 0x81, 0x46, 0x00, 0x29, 0x22, 0xD0, 0x1A, 0x9B, 0x19, 0x68, 0x3A, 0x1C, 0x08, 0x32, 0x30, 0x1C
    .byte 0x79, 0xF0, 0x4C, 0xF8, 0x30, 0x1C, 0x3D, 0x30, 0x00, 0x78, 0x00, 0x28, 0x06, 0xD0, 0x39, 0x1C
    .byte 0x10, 0x31, 0x30, 0x1C, 0x01, 0x22, 0x79, 0xF0, 0xDD, 0xF8, 0x0C, 0xE0, 0x34, 0x1C, 0x3E, 0x34
    .byte 0x20, 0x78, 0x00, 0x28, 0x07, 0xD1, 0x39, 0x1C, 0x10, 0x31, 0x30, 0x1C, 0x01, 0x22, 0x79, 0xF0
    .byte 0xD1, 0xF8, 0x01, 0x20, 0x20, 0x70, 0x00, 0x20, 0x4C, 0x46, 0x20, 0x70, 0x1B, 0x9F, 0x48, 0x37
    .byte 0x1B, 0x97, 0x48, 0x35, 0x48, 0x20, 0x80, 0x44, 0x1C, 0x99, 0x48, 0x31, 0x1C, 0x91, 0x19, 0x9A
    .byte 0x01, 0x32, 0x19, 0x92, 0x05, 0x2A, 0x8C, 0xD9, 0x13, 0x98, 0x14, 0x99, 0x06, 0xF0, 0x60, 0xFF
    .byte 0x16, 0x9B, 0x00, 0x2B, 0x05, 0xD1, 0x13, 0x9C, 0x0D, 0x4D, 0x60, 0x19, 0x00, 0x78, 0x00, 0x28
    .byte 0x77, 0xD0, 0x15, 0x9F, 0xB8, 0x6B, 0xE2, 0xF7, 0x89, 0xFE, 0x4F, 0x28, 0x14, 0xD9, 0x51, 0x46
    .byte 0x08, 0x68, 0x82, 0x6C, 0x50, 0x46, 0x00, 0x21, 0xA8, 0xF0, 0x8C, 0xF8, 0x02, 0x1C, 0x53, 0x46
    .byte 0x18, 0x68, 0x04, 0x49, 0x44, 0x6D, 0x50, 0x46, 0x01, 0x23, 0xA8, 0xF0, 0x87, 0xF8, 0x30, 0xE0
    .byte 0x81, 0x02, 0x00, 0x00, 0x08, 0xBA, 0x58, 0x08, 0x13, 0x9C, 0x32, 0x4D, 0x60, 0x19, 0x00, 0x78
    .byte 0x00, 0x28, 0x26, 0xD0, 0x57, 0x46, 0x38, 0x68, 0x81, 0x6E, 0x50, 0x46, 0xA8, 0xF0, 0x70, 0xF8
    .byte 0x04, 0x1C, 0x15, 0x98, 0x42, 0x8C, 0x20, 0x30, 0x00, 0x78, 0x12, 0x18, 0x25, 0x68, 0x0F, 0xA8
    .byte 0xEB, 0x68, 0x21, 0x1C, 0xA8, 0xF0, 0x68, 0xF8, 0x0F, 0x98, 0x02, 0x88, 0x05, 0xA8, 0x2B, 0x69
    .byte 0x21, 0x1C, 0xA8, 0xF0, 0x61, 0xF8, 0x38, 0x68, 0x82, 0x6C, 0x50, 0x46, 0x00, 0x21, 0xA8, 0xF0
    .byte 0x59, 0xF8, 0x02, 0x1C, 0x38, 0x68, 0x09, 0x99, 0x44, 0x6D, 0x50, 0x46, 0x01, 0x23, 0xA8, 0xF0
    .byte 0x55, 0xF8, 0x15, 0x9D, 0x7C, 0x35, 0x29, 0x88, 0x1B, 0x48, 0x81, 0x42, 0x24, 0xD0, 0x51, 0x46
    .byte 0x08, 0x68, 0x41, 0x6E, 0x50, 0x46, 0xA8, 0xF0, 0x43, 0xF8, 0x04, 0x1C, 0x2A, 0x88, 0x25, 0x68
    .byte 0x11, 0xA8, 0xEB, 0x68, 0x21, 0x1C, 0xA8, 0xF0, 0x3F, 0xF8, 0x11, 0x98, 0x02, 0x88, 0x05, 0xA8
    .byte 0x2B, 0x69, 0x21, 0x1C, 0xA8, 0xF0, 0x38, 0xF8, 0x52, 0x46, 0x10, 0x68, 0x82, 0x6C, 0x50, 0x46
    .byte 0x02, 0x21, 0xA8, 0xF0, 0x2F, 0xF8, 0x02, 0x1C, 0x53, 0x46, 0x18, 0x68, 0x09, 0x99, 0x44, 0x6D
    .byte 0x50, 0x46, 0x01, 0x23, 0xA8, 0xF0, 0x2A, 0xF8, 0x13, 0x9C, 0x06, 0x4D, 0x61, 0x19, 0x00, 0x20
    .byte 0x08, 0x70, 0x1E, 0xB0, 0x38, 0xBC, 0x98, 0x46, 0xA1, 0x46, 0xAA, 0x46, 0xF0, 0xBC, 0x01, 0xBC
    .byte 0x00, 0x47, 0x00, 0x00, 0x81, 0x02, 0x00, 0x00, 0xFF, 0xFF, 0x00, 0x00


    .section .text.entities_part2.0802B944,"ax",%progbits

    thumb_func_start func_0802B944
func_0802B944: @ 0x0802B944
    push {r4, r5, r6, lr}
    mov r6, sb
    mov r5, r8
    push {r5, r6}
    sub sp, #0x14
    adds r5, r0, #0
    mov sb, r1
    add r1, sp, #0xc
    movs r4, #0x8d
    lsls r4, r4, #2
    movs r0, #0
    mov r8, r0
    ldrh r2, [r1]
    ldr r3, .L0802B9B8 @ =0xFFFFFC00
    adds r0, r3, #0
    ands r0, r2
    orrs r0, r4
    strh r0, [r1]
    movs r6, #0
    ldrb r4, [r1, #1]
    movs r2, #3
    adds r0, r2, #0
    ands r0, r4
    strb r0, [r1, #1]
    ldrh r4, [r1, #2]
    adds r0, r3, #0
    ands r0, r4
    strh r0, [r1, #2]
    ldrb r0, [r1, #3]
    ands r2, r0
    strb r2, [r1, #3]
    ldrh r0, [r1, #4]
    ands r3, r0
    strh r3, [r1, #4]
    add r4, sp, #4
    adds r0, r4, #0
    movs r2, #6
    bl memcpy
    strb r6, [r4, #6]
    mov r0, r8
    str r0, [sp]
    adds r0, r5, #0
    mov r1, sb
    add r2, sp, #4
    movs r3, #2
    bl __12AActorEntityP10GameObjectRC13ActorLocationUiUi
    ldr r0, .L0802B9BC @ =vtable_unk_080E671C
    str r0, [r5, #0x14]
    adds r0, r5, #0
    add sp, #0x14
    pop {r3, r4}
    mov r8, r3
    mov sb, r4
    pop {r4, r5, r6}
    pop {r1}
    bx r1
    .align 2, 0
.L0802B9B8: .4byte 0xFFFFFC00
.L0802B9BC: .4byte vtable_unk_080E671C

    thumb_func_start func_0802B9C0
func_0802B9C0: @ 0x0802B9C0
    push {r4, lr}
    adds r4, r0, #0
    bl func_0802B944
    ldr r0, .L0802BA0C @ =vtable_unk_080E65F4
    str r0, [r4, #0x14]
    adds r0, r4, #0
    adds r0, #0x34
    movs r1, #0
    strb r1, [r0]
    adds r0, #1
    strb r1, [r0]
    adds r0, #3
    strb r1, [r0]
    strb r1, [r0, #1]
    str r1, [r0, #4]
    str r1, [r0, #8]
    str r1, [r0, #0xc]
    str r1, [r0, #0x10]
    str r1, [r0, #0x14]
    str r1, [r0, #0x18]
    adds r0, #0x1c
    strb r1, [r0]
    adds r0, #2
    movs r2, #0
    strh r1, [r0]
    adds r0, #2
    strb r2, [r0]
    adds r0, #1
    strb r2, [r0]
    adds r0, r4, #0
    bl vfunc_10__7AEntity
    adds r0, r4, #0
    pop {r4}
    pop {r1}
    bx r1
    .align 2, 0
.L0802BA0C: .4byte vtable_unk_080E65F4
.L0802BA10:
    .byte 0x10, 0xB5, 0x04, 0x1C, 0xD0, 0x20, 0xD4, 0xF7, 0xDD, 0xFD, 0x21, 0x1C, 0x00, 0xF0, 0x86, 0xFA
    .byte 0x10, 0xBC, 0x02, 0xBC, 0x08, 0x47, 0x00, 0x00, 0x30, 0xB5, 0x04, 0x1C, 0x0D, 0x1C, 0x21, 0x1C
    .byte 0x58, 0x31, 0x08, 0x78, 0x00, 0x28, 0x13, 0xD1, 0x20, 0x1C, 0x54, 0x30, 0x00, 0x78, 0x00, 0x28
    .byte 0x10, 0xD0, 0x20, 0x1C, 0x00, 0xF0, 0x16, 0xF8, 0x20, 0x1C, 0x00, 0xF0, 0xFD, 0xF8, 0x00, 0x04
    .byte 0x01, 0x0C, 0x60, 0x8C, 0x88, 0x42, 0x05, 0xD0, 0x20, 0x1C, 0x06, 0xF0, 0xA9, 0xFB, 0x01, 0xE0
    .byte 0x00, 0x20, 0x08, 0x70, 0x20, 0x1C, 0x29, 0x1C, 0x06, 0xF0, 0xE0, 0xFC, 0x30, 0xBC, 0x01, 0xBC
    .byte 0x00, 0x47, 0x00, 0x00

    thumb_func_start func_0802BA74
func_0802BA74: @ 0x0802BA74
    push {r4, r5, r6, r7, lr}
    mov r7, sl
    mov r6, sb
    mov r5, r8
    push {r5, r6, r7}
    sub sp, #8
    adds r7, r0, #0
    adds r0, #0x34
    ldrb r0, [r0]
    cmp r0, #4
    bls .L0802BA8C
    b .L0802BC38
.L0802BA8C:
    lsls r0, r0, #2
    ldr r1, .L0802BA98 @ =.L0802BA9C
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L0802BA98: .4byte .L0802BA9C
.L0802BA9C: @ jump table
    .4byte .L0802BAB0 @ case 0
    .4byte .L0802BABA @ case 1
    .4byte .L0802BBD0 @ case 2
    .4byte .L0802BBF0 @ case 3
    .4byte .L0802BC1C @ case 4
.L0802BAB0:
    adds r1, r7, #0
    adds r1, #0x54
    movs r0, #0
    strb r0, [r1]
    b .L0802BC38
.L0802BABA:
    adds r1, r7, #0
    adds r1, #0x38
    ldr r0, [r1, #4]
    ldr r2, [r1, #0xc]
    adds r0, r0, r2
    str r0, [r1, #4]
    ldr r0, [r1, #8]
    ldr r2, [r1, #0x10]
    adds r0, r0, r2
    str r0, [r1, #8]
    ldr r0, [r1, #0x18]
    ldr r2, .L0802BAE4 @ =0xFFFFB556
    adds r0, r0, r2
    str r0, [r1, #0x18]
    ldr r2, [r1, #0x14]
    adds r2, r2, r0
    mov sl, r1
    cmp r2, #0
    ble .L0802BAE8
    str r2, [r1, #0x14]
    b .L0802BAF0
    .align 2, 0
.L0802BAE4: .4byte 0xFFFFB556
.L0802BAE8:
    movs r0, #0
    mov r3, sl
    str r0, [r3, #0x14]
    str r0, [r3, #0x18]
.L0802BAF0:
    movs r1, #0
    ldrh r0, [r7, #0x24]
    cmp r0, #0
    bne .L0802BAFA
    movs r1, #1
.L0802BAFA:
    cmp r1, #0
    bne .L0802BB00
    b .L0802BC38
.L0802BB00:
    adds r0, r7, #0
    adds r0, #0x34
    movs r1, #0
    strb r1, [r0]
    movs r0, #0xa
    ldrsh r3, [r7, r0]
    movs r2, #0xe
    ldrsh r1, [r7, r2]
    mov ip, r1
    adds r0, r7, #0
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, #1
    beq .L0802BB36
    cmp r0, #1
    bgt .L0802BB26
    cmp r0, #0
    beq .L0802BB30
    b .L0802BB44
.L0802BB26:
    cmp r0, #2
    beq .L0802BB3E
    cmp r0, #3
    beq .L0802BB42
    b .L0802BB44
.L0802BB30:
    movs r0, #0x14
    add ip, r0
    b .L0802BB44
.L0802BB36:
    movs r1, #0x14
    rsbs r1, r1, #0
    add ip, r1
    b .L0802BB44
.L0802BB3E:
    subs r3, #0x14
    b .L0802BB44
.L0802BB42:
    adds r3, #0x14
.L0802BB44:
    ldrh r1, [r7, #4]
    mov r5, sp
    ldr r2, .L0802BBC8 @ =0x000003FF
    mov r8, r2
    mov r0, r8
    ands r1, r0
    ldrh r2, [r5]
    ldr r4, .L0802BBCC @ =0xFFFFFC00
    adds r0, r4, #0
    ands r0, r2
    orrs r0, r1
    strh r0, [r5]
    lsls r3, r3, #0x10
    lsrs r1, r3, #0x10
    movs r2, #0x3f
    mov sb, r2
    ands r1, r2
    lsls r1, r1, #2
    ldrb r6, [r5, #1]
    movs r2, #3
    adds r0, r2, #0
    ands r0, r6
    orrs r0, r1
    strb r0, [r5, #1]
    lsrs r3, r3, #0x16
    mov r0, r8
    ands r3, r0
    ldrh r1, [r5, #2]
    adds r0, r4, #0
    ands r0, r1
    orrs r0, r3
    strh r0, [r5, #2]
    mov r3, ip
    lsls r1, r3, #0x10
    lsrs r0, r1, #0x10
    mov r3, sb
    ands r0, r3
    lsls r0, r0, #2
    ldrb r3, [r5, #3]
    ands r2, r3
    orrs r2, r0
    strb r2, [r5, #3]
    lsrs r1, r1, #0x16
    mov r0, r8
    ands r1, r0
    ldrh r0, [r5, #4]
    ands r4, r0
    orrs r4, r1
    strh r4, [r5, #4]
    ldr r0, [r7]
    ldr r1, [r0]
    adds r1, #0xbc
    adds r2, r7, #0
    adds r2, #0x56
    ldrh r2, [r2]
    ldr r3, [r1]
    mov r1, sp
    bl _call_via_r3
    movs r1, #0
    mov r2, sl
    strb r1, [r2]
    adds r0, r7, #0
    adds r0, #0x59
    strb r1, [r0]
    b .L0802BC38
    .align 2, 0
.L0802BBC8: .4byte 0x000003FF
.L0802BBCC: .4byte 0xFFFFFC00
.L0802BBD0:
    movs r1, #0
    ldrh r0, [r7, #0x24]
    cmp r0, #0
    bne .L0802BBDA
    movs r1, #1
.L0802BBDA:
    cmp r1, #0
    beq .L0802BC38
    adds r0, r7, #0
    adds r0, #0x34
    movs r1, #0
    strb r1, [r0]
    adds r0, #4
    strb r1, [r0]
    adds r0, #0x21
    strb r1, [r0]
    b .L0802BC38
.L0802BBF0:
    movs r1, #0
    ldrh r0, [r7, #0x24]
    cmp r0, #0
    bne .L0802BBFA
    movs r1, #1
.L0802BBFA:
    cmp r1, #0
    beq .L0802BC38
    adds r0, r7, #0
    adds r0, #0x38
    movs r1, #0
    movs r2, #0xa8
    lsls r2, r2, #0xd
    str r1, [r0, #4]
    str r1, [r0, #8]
    str r2, [r0, #0x14]
    subs r0, #4
    strb r1, [r0]
    adds r1, r7, #0
    adds r1, #0x59
    movs r0, #1
    strb r0, [r1]
    b .L0802BC38
.L0802BC1C:
    adds r1, r7, #0
    adds r1, #0x35
    ldrb r0, [r1]
    subs r0, #1
    strb r0, [r1]
    lsls r0, r0, #0x18
    lsrs r2, r0, #0x18
    cmp r2, #0
    bne .L0802BC38
    adds r0, r7, #0
    adds r0, #0x34
    strb r2, [r0]
    adds r0, #4
    strb r2, [r0]
.L0802BC38:
    add sp, #8
    pop {r3, r4, r5}
    mov r8, r3
    mov sb, r4
    mov sl, r5
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0

    .section .text.entities_part2.0802BCA4,"ax",%progbits
.L0802BCA4:
    .byte 0xF0, 0xB5, 0x47, 0x46, 0x80, 0xB4, 0x81, 0xB0, 0x06, 0x1C, 0x0B, 0x1C
    .byte 0x38, 0x30, 0x00, 0x27, 0x01, 0x21, 0x88, 0x46, 0x41, 0x46, 0x01, 0x70, 0x47, 0x70, 0x23, 0x49
    .byte 0x88, 0x22, 0x52, 0x03, 0x41, 0x60, 0x87, 0x60, 0x42, 0x61, 0x35, 0x69, 0x00, 0x2D, 0x1F, 0xD0
    .byte 0x68, 0x46, 0x19, 0x1C, 0xE1, 0xF7, 0x2A, 0xFF, 0x68, 0x46, 0xE1, 0xF7, 0x41, 0xFF, 0x04, 0x1C
    .byte 0x24, 0x04, 0x24, 0x0C, 0x28, 0x68, 0x00, 0x68, 0x01, 0x68, 0x49, 0x6E, 0xA7, 0xF0, 0x10, 0xFE
    .byte 0x01, 0x1C, 0x28, 0x1C, 0xB4, 0x30, 0x22, 0x1C, 0x32, 0xF0, 0xAA, 0xFD, 0x28, 0x1C, 0xC8, 0x30
    .byte 0x41, 0x46, 0x01, 0x70, 0x02, 0x30, 0x07, 0x70, 0x01, 0x30, 0x01, 0x70, 0x01, 0x30, 0x04, 0x80
    .byte 0x31, 0x1C, 0x34, 0x31, 0x04, 0x20, 0x08, 0x70, 0x01, 0x31, 0x14, 0x20, 0x08, 0x70, 0x30, 0x1C
    .byte 0x58, 0x30, 0x41, 0x46, 0x01, 0x70, 0x30, 0x1C, 0xFF, 0xF7, 0x8E, 0xFF, 0x01, 0x1C, 0x09, 0x04
    .byte 0x09, 0x0C, 0x30, 0x1C, 0x06, 0xF0, 0x3C, 0xFA, 0x30, 0x1C, 0x54, 0x30, 0x41, 0x46, 0x01, 0x70
    .byte 0x01, 0xB0, 0x08, 0xBC, 0x98, 0x46, 0xF0, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00, 0xF8, 0xFF
    .byte 0xF0, 0xB5, 0x4F, 0x46, 0x46, 0x46, 0xC0, 0xB4, 0x05, 0x1C, 0x09, 0x04, 0x0E, 0x0C, 0xB1, 0x46
    .byte 0x12, 0x06, 0x13, 0x0E, 0x56, 0x30, 0x00, 0x21, 0x88, 0x46, 0x00, 0x22, 0x06, 0x80, 0x1E, 0x38
    .byte 0x01, 0x27, 0x07, 0x70, 0x41, 0x46, 0x41, 0x70, 0xA8, 0x21, 0x49, 0x03, 0x42, 0x60, 0x82, 0x60
    .byte 0x41, 0x61, 0x2C, 0x69, 0x00, 0x2C, 0x20, 0xD0, 0x00, 0x2B, 0x0A, 0xD1, 0x20, 0x68, 0x00, 0x68
    .byte 0x01, 0x68, 0x49, 0x6E, 0xA7, 0xF0, 0xBC, 0xFD, 0x01, 0x1C, 0x20, 0x1C, 0xB4, 0x30, 0x32, 0x1C
    .byte 0x09, 0xE0, 0x20, 0x68, 0x00, 0x68, 0x01, 0x68, 0x89, 0x6E, 0xA7, 0xF0, 0xB1, 0xFD, 0x01, 0x1C
    .byte 0x20, 0x1C, 0xB4, 0x30, 0x4A, 0x46, 0x32, 0xF0, 0x4B, 0xFD, 0x20, 0x1C, 0xC8, 0x30, 0x07, 0x70
    .byte 0x02, 0x30, 0x41, 0x46, 0x01, 0x70, 0x01, 0x30, 0x07, 0x70, 0x28, 0x1C, 0x34, 0x30, 0x00, 0x24
    .byte 0x03, 0x21, 0x01, 0x70, 0x14, 0x38, 0x03, 0x78, 0x01, 0x2B, 0x1C, 0xD0, 0x01, 0x2B, 0x02, 0xDC
    .byte 0x00, 0x2B, 0x21, 0xD0, 0x29, 0xE0, 0x02, 0x2B, 0x02, 0xD0, 0x03, 0x2B, 0x0A, 0xD0, 0x24, 0xE0
    .byte 0x28, 0x1C, 0x38, 0x30, 0x02, 0x49, 0xC0, 0x22, 0x12, 0x03, 0x41, 0x60, 0x84, 0x60, 0x1B, 0xE0
    .byte 0x00, 0x00, 0xF6, 0xFF, 0x28, 0x1C, 0x38, 0x30, 0xA0, 0x21, 0x09, 0x03, 0xC0, 0x22, 0x12, 0x03
    .byte 0x41, 0x60, 0x84, 0x60, 0x10, 0xE0, 0x28, 0x1C, 0x38, 0x30, 0x02, 0x49, 0xC0, 0x22, 0x12, 0x03
    .byte 0x44, 0x60, 0x08, 0xE0, 0x00, 0x00, 0xF6, 0xFF, 0x28, 0x1C, 0x38, 0x30, 0xA0, 0x21, 0x09, 0x03
    .byte 0xC0, 0x22, 0x12, 0x03, 0x43, 0x60, 0x81, 0x60, 0x42, 0x61, 0x28, 0x1C, 0x58, 0x30, 0x01, 0x24
    .byte 0x04, 0x70, 0x28, 0x1C, 0xFF, 0xF7, 0x00, 0xFF, 0x01, 0x1C, 0x09, 0x04, 0x09, 0x0C, 0x28, 0x1C
    .byte 0x06, 0xF0, 0xAE, 0xF9, 0x28, 0x1C, 0x54, 0x30, 0x04, 0x70, 0x18, 0xBC, 0x98, 0x46, 0xA1, 0x46
    .byte 0xF0, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00

    thumb_func_start func_0802BE68
func_0802BE68: @ 0x0802BE68
    push {r4, r5, lr}
    adds r5, r0, #0
    adds r0, #0x20
    ldrb r2, [r0]
    cmp r2, #1
    beq .L0802BEB0
    cmp r2, #1
    bgt .L0802BE7E
    cmp r2, #0
    beq .L0802BEC0
    b .L0802BECC
.L0802BE7E:
    cmp r2, #2
    beq .L0802BE88
    cmp r2, #3
    beq .L0802BE9C
    b .L0802BECC
.L0802BE88:
    adds r0, r5, #0
    adds r0, #0x38
    ldr r1, .L0802BE98 @ =0xFFFE5556
    movs r2, #0
    str r1, [r0, #0xc]
    str r2, [r0, #0x10]
    b .L0802BECA
    .align 2, 0
.L0802BE98: .4byte 0xFFFE5556
.L0802BE9C:
    adds r0, r5, #0
    adds r0, #0x38
    ldr r1, .L0802BEAC @ =0x0001AAAA
    movs r2, #0
    str r1, [r0, #0xc]
    str r2, [r0, #0x10]
    b .L0802BECA
    .align 2, 0
.L0802BEAC: .4byte 0x0001AAAA
.L0802BEB0:
    adds r0, r5, #0
    adds r0, #0x38
    movs r2, #0
    ldr r1, .L0802BEBC @ =0xFFFE5556
    b .L0802BEC6
    .align 2, 0
.L0802BEBC: .4byte 0xFFFE5556
.L0802BEC0:
    adds r0, r5, #0
    adds r0, #0x38
    ldr r1, .L0802BEF8 @ =0x0001AAAA
.L0802BEC6:
    str r2, [r0, #0xc]
    str r1, [r0, #0x10]
.L0802BECA:
    str r2, [r0, #0x18]
.L0802BECC:
    adds r0, r5, #0
    adds r0, #0x34
    movs r4, #1
    strb r4, [r0]
    adds r0, #0x24
    strb r4, [r0]
    adds r0, r5, #0
    bl GetItemDisplayStateAnimation
    adds r1, r0, #0
    lsls r1, r1, #0x10
    lsrs r1, r1, #0x10
    adds r0, r5, #0
    bl SetAnim__12AActorEntityUi
    adds r0, r5, #0
    adds r0, #0x54
    strb r4, [r0]
    pop {r4, r5}
    pop {r0}
    bx r0
    .align 2, 0
.L0802BEF8: .4byte 0x0001AAAA

    .section .text.entities_part2.0802BF2C,"ax",%progbits

    thumb_func_start func_0802BF2C
func_0802BF2C: @ 0x0802BF2C
    push {r4, r5, r6, lr}
    mov r6, r8
    push {r6}
    sub sp, #0x14
    mov r8, r0
    adds r4, r1, #0
    ldr r3, .L0802BF94 @ =gUnk_080F1224
    movs r0, #3
    str r0, [sp]
    movs r5, #1
    str r5, [sp, #4]
    movs r6, #0
    str r6, [sp, #8]
    str r6, [sp, #0xc]
    add r0, sp, #0x10
    strb r5, [r0]
    mov r0, r8
    movs r2, #6
    bl func_08032560
    ldr r0, .L0802BF98 @ =vtable_unk_080E6644
    mov r1, r8
    str r0, [r1, #4]
    ldr r0, [r4]
    ldr r1, [r0]
    ldr r1, [r1, #0x64]
    bl _call_via_r1
    adds r1, r0, #0
    ldr r3, [r4]
    mov r0, r8
    adds r0, #0x8c
    movs r2, #2
    str r2, [sp]
    str r5, [sp, #4]
    add r2, sp, #8
    strb r6, [r2]
    movs r2, #0
    bl func_080A4A00
    mov r1, r8
    adds r1, #0xcc
    ldr r0, .L0802BF9C @ =0x0000FFFF
    strh r0, [r1]
    mov r0, r8
    add sp, #0x14
    pop {r3}
    mov r8, r3
    pop {r4, r5, r6}
    pop {r1}
    bx r1
    .align 2, 0
.L0802BF94: .4byte gUnk_080F1224
.L0802BF98: .4byte vtable_unk_080E6644
.L0802BF9C: .4byte 0x0000FFFF

    thumb_func_start func_0802BFA0
func_0802BFA0: @ 0x0802BFA0
    push {r4, lr}
    adds r4, r0, #0
    bl func_0803260C
    ldr r0, [r4]
    adds r0, #0x38
    movs r1, #0
    ldrb r0, [r0]
    cmp r0, #1
    bne .L0802BFB6
    movs r1, #1
.L0802BFB6:
    cmp r1, #0
    beq .L0802BFDE
    adds r1, r4, #0
    adds r1, #0xcb
    ldrb r0, [r1]
    cmp r0, #0
    bne .L0802BFDA
    adds r0, r4, #0
    adds r0, #0xb4
    bl func_0805E8F0
    lsls r0, r0, #0x1e
    cmp r0, #0
    bge .L0802BFDE
    adds r1, r4, #0
    adds r1, #0xc8
    movs r0, #1
    b .L0802BFDC
.L0802BFDA:
    movs r0, #0
.L0802BFDC:
    strb r0, [r1]
.L0802BFDE:
    pop {r4}
    pop {r0}
    bx r0

    thumb_func_start func_0802BFE4
func_0802BFE4: @ 0x0802BFE4
    push {r4, r5, r6, r7, lr}
    mov r7, sl
    mov r6, sb
    mov r5, r8
    push {r5, r6, r7}
    sub sp, #0x6c
    adds r6, r0, #0
    mov r8, r1
    ldr r4, [r6]
    ldrh r0, [r4, #4]
    str r0, [sp, #0x44]
    ldr r1, [r4]
    mov sb, r1
    adds r0, r6, #0
    adds r0, #0x44
    ldrb r0, [r0]
    str r0, [sp, #0x48]
    ldr r2, [r4, #8]
    str r2, [sp, #0x68]
    asrs r1, r2, #0x10
    ldr r0, [r4, #0xc]
    str r0, [sp, #0x4c]
    asrs r2, r0, #0x10
    add r5, sp, #0x34
    mov r3, r8
    ldr r0, [r3]
    ldr r3, [r3, #8]
    str r0, [sp, #0x34]
    str r3, [r5, #4]
    ldr r0, [sp, #0x44]
    bl func_08032900
    str r0, [sp, #0x50]
    adds r0, r6, #0
    adds r0, #0x8b
    add r7, sp, #0x50
    ldrb r7, [r7]
    strb r7, [r0]
    adds r4, #0x38
    mov sl, r4
    movs r1, #0
    ldrb r0, [r4]
    cmp r0, #1
    bne .L0802C03E
    movs r1, #1
.L0802C03E:
    cmp r1, #0
    bne .L0802C044
    b .L0802C18C
.L0802C044:
    mov r1, sl
    ldr r0, [r1, #4]
    ldr r2, [sp, #0x68]
    adds r0, r2, r0
    asrs r0, r0, #0x10
    mov r3, r8
    movs r7, #0xc
    ldrsh r1, [r3, r7]
    subs r0, r0, r1
    str r0, [sp, #0x54]
    mov r1, sl
    ldr r0, [r1, #8]
    ldr r2, [sp, #0x4c]
    adds r4, r2, r0
    asrs r1, r4, #0x10
    movs r7, #0xe
    ldrsh r0, [r3, r7]
    subs r7, r1, r0
    movs r0, #0x80
    lsls r0, r0, #8
    subs r0, r0, r1
    str r0, [sp, #0x58]
    movs r1, #0
    mov r2, sl
    ldrb r0, [r2, #1]
    cmp r0, #1
    bne .L0802C07C
    movs r1, #1
.L0802C07C:
    cmp r1, #0
    beq .L0802C09C
    mov r3, sb
    ldr r0, [r3]
    ldr r1, [r0, #0x58]
    mov r0, sb
    bl _call_via_r1
    mov r2, r8
    ldr r1, [r2, #8]
    movs r2, #0
    str r2, [sp]
    ldr r2, [sp, #0x54]
    adds r3, r7, #0
    bl func_0803AE58
.L0802C09C:
    mov r3, sl
    ldr r0, [r3, #0x14]
    subs r0, r4, r0
    asrs r0, r0, #0x10
    mov r7, r8
    movs r2, #0xe
    ldrsh r1, [r7, r2]
    subs r0, r0, r1
    str r0, [sp, #0x5c]
    ldr r3, [sp, #0x44]
    cmp r3, #8
    bgt .L0802C0C0
    movs r0, #1
    ldr r7, [sp, #0x50]
    cmp r7, #1
    beq .L0802C0C2
    movs r0, #2
    b .L0802C0C2
.L0802C0C0:
    movs r0, #1
.L0802C0C2:
    adds r5, r6, #0
    adds r5, #0x8c
    mov r1, sp
    adds r1, #0x34
    str r1, [sp, #0x60]
    lsls r4, r0, #2
    orrs r4, r0
    lsls r1, r0, #4
    orrs r4, r1
    lsls r0, r0, #6
    orrs r4, r0
    adds r0, r6, #0
    adds r0, #0xb4
    ldr r1, [r0]
    ldr r3, [r1]
    ldrh r2, [r0, #0xc]
    adds r0, #4
    lsls r2, r2, #2
    ldr r0, [r0]
    adds r0, r0, r2
    ldrh r2, [r0]
    add r0, sp, #0x14
    ldr r3, [r3, #0x10]
    bl _call_via_r3
    add r7, sp, #0x14
    ldr r2, [sp, #0x60]
    ldr r2, [r2, #4]
    mov ip, r2
    ldr r2, [r5]
    adds r0, r6, #0
    adds r0, #0x90
    ldr r1, [r0, #4]
    rsbs r0, r1, #0
    orrs r0, r1
    cmp r0, #0
    bge .L0802C134
    ldrh r0, [r5, #0xc]
    adds r1, r6, #0
    adds r1, #0x9c
    ldr r3, [sp, #0x58]
    str r3, [sp]
    adds r3, r7, #0
    str r3, [sp, #4]
    str r2, [sp, #8]
    str r0, [sp, #0xc]
    str r1, [sp, #0x10]
    mov r0, ip
    ldr r1, [sp, #0x54]
    ldr r2, [sp, #0x5c]
    adds r3, r4, #0
    ldr r4, .L0802C130 @ =func_030004DC
    bl _call_via_r4
    b .L0802C136
    .align 2, 0
.L0802C130: .4byte func_030004DC
.L0802C134:
    movs r0, #0
.L0802C136:
    cmp r0, #0
    beq .L0802C18C
    adds r0, r5, #0
    adds r0, #0x3c
    ldrb r1, [r0]
    str r0, [sp, #0x64]
    cmp r1, #0
    beq .L0802C18C
    ldr r0, [sp, #0x60]
    ldr r1, [r0]
    adds r2, r7, #0
    adds r2, #8
    adds r0, r5, #0
    bl func_080A480C
    adds r0, r5, #0
    adds r0, #0x3d
    ldrb r0, [r0]
    cmp r0, #0
    beq .L0802C16C
    adds r1, r7, #0
    adds r1, #0x10
    adds r0, r5, #0
    movs r2, #1
    bl func_080A4944
    b .L0802C186
.L0802C16C:
    adds r4, r5, #0
    adds r4, #0x3e
    ldrb r0, [r4]
    cmp r0, #0
    bne .L0802C186
    adds r1, r7, #0
    adds r1, #0x10
    adds r0, r5, #0
    movs r2, #1
    bl func_080A4944
    movs r0, #1
    strb r0, [r4]
.L0802C186:
    movs r0, #0
    ldr r1, [sp, #0x64]
    strb r0, [r1]
.L0802C18C:
    adds r0, r6, #0
    mov r1, r8
    bl func_08032690
    ldr r2, [sp, #0x48]
    cmp r2, #0
    beq .L0802C1FC
    movs r1, #0
    mov r3, sl
    ldrb r0, [r3]
    cmp r0, #1
    bne .L0802C1A6
    movs r1, #1
.L0802C1A6:
    cmp r1, #0
    bne .L0802C1FC
    adds r5, r6, #0
    adds r5, #0xcc
    ldrh r1, [r5]
    ldr r0, .L0802C20C @ =0x0000FFFF
    cmp r1, r0
    beq .L0802C1FC
    mov r7, sb
    ldr r0, [r7]
    ldr r1, [r0, #0x64]
    mov r0, sb
    bl _call_via_r1
    adds r4, r0, #0
    ldrh r2, [r5]
    ldr r5, [r4]
    add r0, sp, #0x3c
    ldr r3, [r5, #0xc]
    adds r1, r4, #0
    bl _call_via_r3
    ldr r0, [sp, #0x3c]
    ldrh r2, [r0]
    add r0, sp, #0x14
    ldr r3, [r5, #0x10]
    adds r1, r4, #0
    bl _call_via_r3
    ldr r0, [r7]
    ldr r2, [r0, #0x48]
    mov r0, sb
    movs r1, #2
    bl _call_via_r2
    adds r2, r0, #0
    ldr r0, [r7]
    ldr r1, [sp, #0x24]
    ldr r4, [r0, #0x54]
    mov r0, sb
    movs r3, #1
    bl _call_via_r4
.L0802C1FC:
    add sp, #0x6c
    pop {r3, r4, r5}
    mov r8, r3
    mov sb, r4
    mov sl, r5
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0
    .align 2, 0
.L0802C20C: .4byte 0x0000FFFF

    .section .text.entities_part2.0802C228,"ax",%progbits
.L0802C228:
    .byte 0x30, 0xB5, 0x05, 0x1C, 0x0C, 0x1C, 0x28, 0x68
    .byte 0x00, 0x68, 0x01, 0x68, 0x49, 0x6E, 0xA7, 0xF0, 0x6B, 0xFB, 0x01, 0x1C, 0x28, 0x1C, 0xB4, 0x30
    .byte 0x22, 0x1C, 0x32, 0xF0, 0x05, 0xFB, 0x28, 0x1C, 0xC8, 0x30, 0x00, 0x22, 0x01, 0x21, 0x01, 0x70
    .byte 0x02, 0x30, 0x02, 0x70, 0x01, 0x30, 0x01, 0x70, 0x01, 0x30, 0x04, 0x80, 0x30, 0xBC, 0x01, 0xBC
    .byte 0x00, 0x47, 0x00, 0x00, 0x30, 0xB5, 0x05, 0x1C, 0x0C, 0x1C, 0x28, 0x68, 0x00, 0x68, 0x01, 0x68
    .byte 0x89, 0x6E, 0xA7, 0xF0, 0x4D, 0xFB, 0x01, 0x1C, 0x28, 0x1C, 0xB4, 0x30, 0x22, 0x1C, 0x32, 0xF0
    .byte 0xE7, 0xFA, 0x28, 0x1C, 0xC8, 0x30, 0x00, 0x22, 0x01, 0x21, 0x01, 0x70, 0x02, 0x30, 0x02, 0x70
    .byte 0x01, 0x30, 0x01, 0x70, 0x30, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00, 0x30, 0xB5, 0x05, 0x1C
    .byte 0x0C, 0x1C, 0x28, 0x68, 0x00, 0x68, 0x01, 0x68, 0x49, 0x6E, 0xA7, 0xF0, 0x31, 0xFB, 0x01, 0x1C
    .byte 0x28, 0x1C, 0xB4, 0x30, 0x22, 0x1C, 0x32, 0xF0, 0xCB, 0xFA, 0x28, 0x1C, 0xC8, 0x30, 0x00, 0x22
    .byte 0x01, 0x21, 0x01, 0x70, 0x02, 0x30, 0x02, 0x70, 0x01, 0x30, 0x01, 0x70, 0x30, 0xBC, 0x01, 0xBC
    .byte 0x00, 0x47, 0x00, 0x00, 0x54, 0x30, 0x00, 0x78, 0x70, 0x47, 0x00, 0x00

    .section .text.entities_part2.0802C2E0,"ax",%progbits

    thumb_func_start func_0802C2E0
func_0802C2E0: @ 0x0802C2E0
    push {r4, r5, r6, r7, lr}
    mov r7, r8
    push {r7}
    mov r8, r0
    mov r1, r8
    adds r1, #0x3c
    movs r0, #9
    strb r0, [r1]
    movs r5, #0
    mov r0, r8
    adds r0, #0x3f
    ldrb r0, [r0]
    cmp r0, #0xd
    bls .L0802C2FE
    b .L0802C840
.L0802C2FE:
    lsls r0, r0, #2
    ldr r1, .L0802C308 @ =.L0802C30C
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L0802C308: .4byte .L0802C30C
.L0802C30C: @ jump table
    .4byte .L0802C3BC @ case 0
    .4byte .L0802C3BC @ case 1
    .4byte .L0802C440 @ case 2
    .4byte .L0802C3BC @ case 3
    .4byte .L0802C3BC @ case 4
    .4byte .L0802C344 @ case 5
    .4byte .L0802C4E4 @ case 6
    .4byte .L0802C554 @ case 7
    .4byte .L0802C5A4 @ case 8
    .4byte .L0802C5EE @ case 9
    .4byte .L0802C682 @ case 10
    .4byte .L0802C6D4 @ case 11
    .4byte .L0802C76A @ case 12
    .4byte .L0802C7B0 @ case 13
.L0802C344:
    mov r0, r8
    movs r1, #0xa2
    movs r2, #1
    bl func_0802A4C0
    mov r2, r8
    adds r2, #0x88
    movs r4, #0
    movs r0, #0x80
    lsls r0, r0, #0xd
    str r4, [r2, #4]
    str r4, [r2, #8]
    str r0, [r2, #0x14]
    mov r0, r8
    adds r0, #0x20
    ldrb r3, [r0]
    cmp r3, #1
    beq .L0802C39C
    cmp r3, #1
    bgt .L0802C372
    cmp r3, #0
    beq .L0802C3B0
    b .L0802C3BC
.L0802C372:
    cmp r3, #2
    beq .L0802C37C
    cmp r3, #3
    beq .L0802C38C
    b .L0802C3BC
.L0802C37C:
    ldr r0, .L0802C388 @ =0xFFFE4925
    movs r1, #0xa0
    lsls r1, r1, #0xa
    str r0, [r2, #0xc]
    str r4, [r2, #0x10]
    b .L0802C3BA
    .align 2, 0
.L0802C388: .4byte 0xFFFE4925
.L0802C38C:
    ldr r0, .L0802C398 @ =0x0001B6DB
    movs r1, #0xa0
    lsls r1, r1, #0xa
    str r0, [r2, #0xc]
    str r4, [r2, #0x10]
    b .L0802C3BA
    .align 2, 0
.L0802C398: .4byte 0x0001B6DB
.L0802C39C:
    ldr r0, .L0802C3A8 @ =0xFFFE4925
    movs r1, #0xa0
    lsls r1, r1, #0xa
    str r4, [r2, #0xc]
    b .L0802C3B8
    .align 2, 0
.L0802C3A8: .4byte 0xFFFE4925
.L0802C3AC:
    ldr r2, [r6]
    b .L0802C40C
.L0802C3B0:
    ldr r0, .L0802C434 @ =0x0001B6DB
    movs r1, #0xa0
    lsls r1, r1, #0xa
    str r3, [r2, #0xc]
.L0802C3B8:
    str r0, [r2, #0x10]
.L0802C3BA:
    str r1, [r2, #0x18]
.L0802C3BC:
    ldr r2, .L0802C438 @ =gUnk_080F1020
    mov r5, r8
    adds r5, #0x40
    mov r4, r8
    adds r4, #0x3f
    ldrb r1, [r4]
    lsls r0, r1, #3
    subs r0, r0, r1
    ldrb r1, [r5]
    adds r0, r0, r1
    lsls r0, r0, #3
    adds r0, r0, r2
    ldrh r1, [r0]
    mov r0, r8
    adds r0, #0x44
    strh r1, [r0]
    mov r2, r8
    ldr r0, [r2]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r6, [r0]
    ldr r7, [r0, #4]
    cmp r6, r7
    beq .L0802C408
.L0802C3F6:
    ldr r0, [r6]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802C3AC
    adds r6, #4
    cmp r6, r7
    bne .L0802C3F6
.L0802C408:
    subs r0, r7, #4
    ldr r2, [r0]
.L0802C40C:
    ldr r6, .L0802C438 @ =gUnk_080F1020
    ldrb r1, [r4]
    lsls r0, r1, #3
    subs r0, r0, r1
    ldrb r1, [r5]
    adds r0, r0, r1
    lsls r0, r0, #3
    adds r0, r0, r6
    ldrh r1, [r0, #2]
    adds r0, r2, #0
    bl func_08008B6C
    mov r0, r8
    bl func_08025944
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802C4B4
    b .L0802C4CC
    .align 2, 0
.L0802C434: .4byte 0x0001B6DB
.L0802C438: .4byte gUnk_080F1020
.L0802C43C:
    ldr r2, [r6]
    b .L0802C490
.L0802C440:
    ldr r2, .L0802C4C8 @ =gUnk_080F1020
    mov r5, r8
    adds r5, #0x40
    mov r4, r8
    adds r4, #0x3f
    ldrb r1, [r4]
    lsls r0, r1, #3
    subs r0, r0, r1
    ldrb r1, [r5]
    adds r0, r0, r1
    lsls r0, r0, #3
    adds r0, r0, r2
    ldrh r1, [r0]
    mov r0, r8
    adds r0, #0x44
    strh r1, [r0]
    mov r2, r8
    ldr r0, [r2]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r6, [r0]
    ldr r7, [r0, #4]
    cmp r6, r7
    beq .L0802C48C
.L0802C47A:
    ldr r0, [r6]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802C43C
    adds r6, #4
    cmp r6, r7
    bne .L0802C47A
.L0802C48C:
    subs r0, r7, #4
    ldr r2, [r0]
.L0802C490:
    ldr r6, .L0802C4C8 @ =gUnk_080F1020
    ldrb r1, [r4]
    lsls r0, r1, #3
    subs r0, r0, r1
    ldrb r1, [r5]
    adds r0, r0, r1
    lsls r0, r0, #3
    adds r0, r0, r6
    ldrh r1, [r0, #2]
    adds r0, r2, #0
    bl func_08008B6C
    mov r0, r8
    bl func_08025944
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802C4CC
.L0802C4B4:
    ldrb r1, [r4]
    lsls r0, r1, #3
    subs r0, r0, r1
    ldrb r5, [r5]
    adds r0, r0, r5
    lsls r0, r0, #3
    adds r0, r0, r6
    movs r5, #4
    ldrsb r5, [r0, r5]
    b .L0802C840
    .align 2, 0
.L0802C4C8: .4byte gUnk_080F1020
.L0802C4CC:
    ldrb r1, [r4]
    lsls r0, r1, #3
    subs r0, r0, r1
    ldrb r5, [r5]
    adds r0, r0, r5
    lsls r0, r0, #3
    adds r0, r0, r6
    movs r5, #5
    ldrsb r5, [r0, r5]
    b .L0802C840
.L0802C4E0:
    ldr r0, [r4]
    b .L0802C522
.L0802C4E4:
    mov r1, r8
    adds r1, #0x44
    movs r0, #0xd7
    lsls r0, r0, #1
    strh r0, [r1]
    mov r2, r8
    ldr r0, [r2]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    mov r6, r8
    adds r6, #0xb2
    cmp r4, r5
    beq .L0802C51E
.L0802C50C:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802C4E0
    adds r4, #4
    cmp r4, r5
    bne .L0802C50C
.L0802C51E:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802C522:
    movs r1, #0x9d
    bl func_08008B6C
    ldr r0, .L0802C54C @ =gUnk_080F1184
.L0802C52A:
    ldr r1, [r0]
    ldr r2, [r0, #4]
    mov r0, r8
    bl func_0802AC2C
    ldrb r3, [r6]
    movs r1, #0
    adds r0, r3, #0
    subs r0, #0x2b
    cmp r0, #0x1a
    bhi .L0802C542
    movs r1, #1
.L0802C542:
    cmp r1, #0
    beq .L0802C548
    b .L0802C812
.L0802C548:
    b .L0802C828
    .align 2, 0
.L0802C54C: .4byte gUnk_080F1184
.L0802C550:
    ldr r0, [r4]
    b .L0802C592
.L0802C554:
    mov r1, r8
    adds r1, #0x44
    movs r0, #0xdf
    lsls r0, r0, #1
    strh r0, [r1]
    mov r2, r8
    ldr r0, [r2]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    mov r6, r8
    adds r6, #0xb2
    cmp r4, r5
    beq .L0802C58E
.L0802C57C:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802C550
    adds r4, #4
    cmp r4, r5
    bne .L0802C57C
.L0802C58E:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802C592:
    movs r1, #0x9d
    bl func_08008B6C
    ldr r0, .L0802C59C @ =gUnk_080F118C
    b .L0802C52A
    .align 2, 0
.L0802C59C: .4byte gUnk_080F118C
.L0802C5A0:
    ldr r0, [r4]
    b .L0802C5DE
.L0802C5A4:
    mov r1, r8
    adds r1, #0x44
    movs r0, #0xdd
    lsls r0, r0, #1
    strh r0, [r1]
    mov r2, r8
    ldr r0, [r2]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    cmp r4, r5
    beq .L0802C5DA
.L0802C5C8:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802C5A0
    adds r4, #4
    cmp r4, r5
    bne .L0802C5C8
.L0802C5DA:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802C5DE:
    movs r1, #0x94
    bl func_08008B6C
.L0802C5E4:
    movs r5, #2
    rsbs r5, r5, #0
    b .L0802C840
.L0802C5EA:
    ldr r0, [r4]
    b .L0802C62C
.L0802C5EE:
    mov r1, r8
    adds r1, #0x44
    movs r0, #0xd5
    lsls r0, r0, #1
    strh r0, [r1]
    mov r1, r8
    ldr r0, [r1]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    mov r6, r8
    adds r6, #0xb2
    cmp r4, r5
    beq .L0802C628
.L0802C616:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802C5EA
    adds r4, #4
    cmp r4, r5
    bne .L0802C616
.L0802C628:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802C62C:
    movs r1, #0x93
    bl func_08008B6C
    ldr r0, .L0802C664 @ =gUnk_080F1194
    ldr r1, [r0]
    ldr r2, [r0, #4]
    mov r0, r8
    bl func_0802AC2C
    ldrb r3, [r6]
    movs r1, #0
    adds r0, r3, #0
    subs r0, #0x2b
    cmp r0, #0x1a
    bhi .L0802C64C
    movs r1, #1
.L0802C64C:
    cmp r1, #0
    beq .L0802C668
    mov r1, r8
    ldr r0, [r1]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    adds r1, r3, #0
    bl _call_via_r2
    adds r1, r0, #0
    b .L0802C66A
    .align 2, 0
.L0802C664: .4byte gUnk_080F1194
.L0802C668:
    movs r1, #0
.L0802C66A:
    cmp r1, #0
    beq .L0802C5E4
    ldr r0, [r1, #0x14]
    adds r0, #0x88
    ldr r2, [r0]
    adds r0, r1, #0
    movs r1, #0x3c
    bl _call_via_r2
    b .L0802C5E4
.L0802C67E:
    ldr r0, [r4]
    b .L0802C6C0
.L0802C682:
    mov r1, r8
    adds r1, #0x44
    movs r0, #0xdb
    lsls r0, r0, #1
    strh r0, [r1]
    mov r2, r8
    ldr r0, [r2]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    mov r6, r8
    adds r6, #0xb2
    cmp r4, r5
    beq .L0802C6BC
.L0802C6AA:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802C67E
    adds r4, #4
    cmp r4, r5
    bne .L0802C6AA
.L0802C6BC:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802C6C0:
    movs r1, #0x9f
    bl func_08008B6C
    ldr r0, .L0802C6CC @ =gUnk_080F119C
    b .L0802C52A
    .align 2, 0
.L0802C6CC: .4byte gUnk_080F119C
.L0802C6D0:
    ldr r0, [r4]
    b .L0802C712
.L0802C6D4:
    mov r1, r8
    adds r1, #0x44
    movs r0, #0xd9
    lsls r0, r0, #1
    strh r0, [r1]
    mov r2, r8
    ldr r0, [r2]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    mov r6, r8
    adds r6, #0xb2
    cmp r4, r5
    beq .L0802C70E
.L0802C6FC:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802C6D0
    adds r4, #4
    cmp r4, r5
    bne .L0802C6FC
.L0802C70E:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802C712:
    movs r1, #0xa0
    bl func_08008B6C
    ldr r0, .L0802C748 @ =gUnk_080F11A4
    ldr r1, [r0]
    ldr r2, [r0, #4]
    mov r0, r8
    bl func_0802AC2C
    ldrb r3, [r6]
    movs r1, #0
    adds r0, r3, #0
    subs r0, #0x2b
    cmp r0, #0x1a
    bhi .L0802C732
    movs r1, #1
.L0802C732:
    cmp r1, #0
    beq .L0802C74C
    mov r1, r8
    ldr r0, [r1]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    adds r1, r3, #0
    bl _call_via_r2
    adds r1, r0, #0
    b .L0802C74E
    .align 2, 0
.L0802C748: .4byte gUnk_080F11A4
.L0802C74C:
    movs r1, #0
.L0802C74E:
    cmp r1, #0
    beq .L0802C760
    ldr r0, [r1, #0x14]
    adds r0, #0x88
    ldr r2, [r0]
    adds r0, r1, #0
    movs r1, #0x3c
    bl _call_via_r2
.L0802C760:
    movs r5, #8
    rsbs r5, r5, #0
    b .L0802C840
.L0802C766:
    ldr r0, [r4]
    b .L0802C7A4
.L0802C76A:
    mov r1, r8
    adds r1, #0x44
    movs r0, #0xd3
    lsls r0, r0, #1
    strh r0, [r1]
    mov r2, r8
    ldr r0, [r2]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    cmp r4, r5
    beq .L0802C7A0
.L0802C78E:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802C766
    adds r4, #4
    cmp r4, r5
    bne .L0802C78E
.L0802C7A0:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802C7A4:
    movs r1, #0x9e
    bl func_08008B6C
    b .L0802C83C
.L0802C7AC:
    ldr r0, [r4]
    b .L0802C7EE
.L0802C7B0:
    mov r1, r8
    adds r1, #0x44
    movs r0, #0xd1
    lsls r0, r0, #1
    strh r0, [r1]
    mov r1, r8
    ldr r0, [r1]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    mov r6, r8
    adds r6, #0xb2
    cmp r4, r5
    beq .L0802C7EA
.L0802C7D8:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802C7AC
    adds r4, #4
    cmp r4, r5
    bne .L0802C7D8
.L0802C7EA:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802C7EE:
    movs r1, #0x9d
    bl func_08008B6C
    ldr r0, .L0802C824 @ =gUnk_080F11AC
    ldr r1, [r0]
    ldr r2, [r0, #4]
    mov r0, r8
    bl func_0802AC2C
    ldrb r3, [r6]
    movs r1, #0
    adds r0, r3, #0
    subs r0, #0x2b
    cmp r0, #0x1a
    bhi .L0802C80E
    movs r1, #1
.L0802C80E:
    cmp r1, #0
    beq .L0802C828
.L0802C812:
    mov r1, r8
    ldr r0, [r1]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    adds r1, r3, #0
    bl _call_via_r2
    adds r1, r0, #0
    b .L0802C82A
    .align 2, 0
.L0802C824: .4byte gUnk_080F11AC
.L0802C828:
    movs r1, #0
.L0802C82A:
    cmp r1, #0
    beq .L0802C83C
    ldr r0, [r1, #0x14]
    adds r0, #0x88
    ldr r2, [r0]
    adds r0, r1, #0
    movs r1, #0x3c
    bl _call_via_r2
.L0802C83C:
    movs r5, #4
    rsbs r5, r5, #0
.L0802C840:
    mov r2, r8
    ldr r4, [r2, #0x14]
    mov r0, r8
    bl func_08025B10
    adds r2, r0, #0
    ldr r3, [r4, #0x64]
    mov r0, r8
    adds r1, r5, #0
    bl _call_via_r3
    pop {r3}
    mov r8, r3
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0

    thumb_func_start func_0802C860
func_0802C860: @ 0x0802C860
    push {r4, r5, r6, lr}
    sub sp, #4
    adds r6, r0, #0
    adds r1, r6, #0
    adds r1, #0x3c
    movs r0, #0xa
    strb r0, [r1]
    adds r1, #1
    movs r0, #0x14
    strb r0, [r1]
    adds r0, r6, #0
    bl ShowFarmerToolDisplay
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
    beq .L0802C8A4
.L0802C892:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802C8E8
    adds r4, #4
    cmp r4, r5
    bne .L0802C892
.L0802C8A4:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802C8A8:
    movs r1, #0xbc
    bl func_08008B6C
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
    bl GetId__C4Tool
    cmp r0, #0x4d
    bne .L0802C8EC
    ldr r4, [r6, #0x14]
    movs r5, #2
    rsbs r5, r5, #0
    adds r0, r6, #0
    bl func_08025B10
    adds r2, r0, #0
    ldr r3, [r4, #0x64]
    adds r0, r6, #0
    adds r1, r5, #0
    bl _call_via_r3
    b .L0802C900
.L0802C8E8:
    ldr r0, [r4]
    b .L0802C8A8
.L0802C8EC:
    ldr r4, [r6, #0x14]
    adds r0, r6, #0
    bl func_08025B10
    adds r2, r0, #0
    ldr r3, [r4, #0x64]
    adds r0, r6, #0
    movs r1, #0
    bl _call_via_r3
.L0802C900:
    add sp, #4
    pop {r4, r5, r6}
    pop {r0}
    bx r0

    thumb_func_start func_0802C908
func_0802C908: @ 0x0802C908
    push {r4, r5, lr}
    adds r4, r0, #0
    adds r3, r4, #0
    adds r3, #0x41
    ldrb r0, [r3]
    cmp r0, #0
    beq .L0802C97E
    adds r2, r4, #0
    adds r2, #0x40
    ldrb r1, [r2]
    subs r0, #1
    cmp r1, r0
    bge .L0802C97E
    adds r1, r4, #0
    adds r1, #0x42
    ldrb r0, [r1]
    adds r0, #1
    strb r0, [r1]
    lsls r0, r0, #0x18
    lsrs r0, r0, #0x18
    cmp r0, #0x14
    bne .L0802C97E
    movs r0, #0
    strb r0, [r1]
    ldrb r0, [r2]
    adds r0, #1
    strb r0, [r2]
    lsls r0, r0, #0x18
    lsrs r0, r0, #0x18
    cmp r0, #5
    bne .L0802C97E
    ldr r0, [r4]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    cmp r4, r5
    beq .L0802C974
.L0802C95E:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802C96E
    ldr r0, [r4]
    b .L0802C978
.L0802C96E:
    adds r4, #4
    cmp r4, r5
    bne .L0802C95E
.L0802C974:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802C978:
    movs r1, #0xb8
    bl func_08008B6C
.L0802C97E:
    pop {r4, r5}
    pop {r0}
    bx r0

    thumb_func_start func_0802C984
func_0802C984: @ 0x0802C984
    push {r4, r5, r6, r7, lr}
    sub sp, #0x10
    adds r6, r0, #0
    adds r0, #0x40
    movs r1, #0
    strb r1, [r0]
    adds r0, #2
    strb r1, [r0]
    ldr r0, [r6, #0x38]
    adds r0, #0x5c
    bl IsEmpty__C9ToolStack
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802C9AA
    adds r1, r6, #0
    adds r1, #0x3f
    movs r0, #0xf
    b .L0802CDC2
.L0802C9AA:
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    add r0, sp, #0xc
    strb r1, [r0]
    bl GetId__C4Tool
    cmp r0, #0x50
    bls .L0802C9CC
    b .L0802CC8A
.L0802C9CC:
    lsls r0, r0, #2
    ldr r1, .L0802C9D8 @ =.L0802C9DC
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L0802C9D8: .4byte .L0802C9DC
.L0802C9DC: @ jump table
    .4byte .L0802CB20 @ case 0
    .4byte .L0802CB20 @ case 1
    .4byte .L0802CB20 @ case 2
    .4byte .L0802CB20 @ case 3
    .4byte .L0802CB20 @ case 4
    .4byte .L0802CB20 @ case 5
    .4byte .L0802CB20 @ case 6
    .4byte .L0802CB20 @ case 7
    .4byte .L0802CB28 @ case 8
    .4byte .L0802CB28 @ case 9
    .4byte .L0802CB28 @ case 10
    .4byte .L0802CB28 @ case 11
    .4byte .L0802CB28 @ case 12
    .4byte .L0802CB28 @ case 13
    .4byte .L0802CB28 @ case 14
    .4byte .L0802CB28 @ case 15
    .4byte .L0802CB30 @ case 16
    .4byte .L0802CB30 @ case 17
    .4byte .L0802CB30 @ case 18
    .4byte .L0802CB30 @ case 19
    .4byte .L0802CB30 @ case 20
    .4byte .L0802CB30 @ case 21
    .4byte .L0802CB30 @ case 22
    .4byte .L0802CB30 @ case 23
    .4byte .L0802CB38 @ case 24
    .4byte .L0802CB38 @ case 25
    .4byte .L0802CB38 @ case 26
    .4byte .L0802CB38 @ case 27
    .4byte .L0802CB38 @ case 28
    .4byte .L0802CB38 @ case 29
    .4byte .L0802CB38 @ case 30
    .4byte .L0802CB38 @ case 31
    .4byte .L0802CB40 @ case 32
    .4byte .L0802CB40 @ case 33
    .4byte .L0802CB40 @ case 34
    .4byte .L0802CB40 @ case 35
    .4byte .L0802CB40 @ case 36
    .4byte .L0802CB40 @ case 37
    .4byte .L0802CB40 @ case 38
    .4byte .L0802CB40 @ case 39
    .4byte .L0802CC3A @ case 40
    .4byte .L0802CC3A @ case 41
    .4byte .L0802CC3A @ case 42
    .4byte .L0802CC3A @ case 43
    .4byte .L0802CC3A @ case 44
    .4byte .L0802CC3A @ case 45
    .4byte .L0802CC3A @ case 46
    .4byte .L0802CC3A @ case 47
    .4byte .L0802CC42 @ case 48
    .4byte .L0802CC4A @ case 49
    .4byte .L0802CC52 @ case 50
    .4byte .L0802CC52 @ case 51
    .4byte .L0802CC52 @ case 52
    .4byte .L0802CC52 @ case 53
    .4byte .L0802CC52 @ case 54
    .4byte .L0802CC52 @ case 55
    .4byte .L0802CC52 @ case 56
    .4byte .L0802CC52 @ case 57
    .4byte .L0802CC52 @ case 58
    .4byte .L0802CC52 @ case 59
    .4byte .L0802CC52 @ case 60
    .4byte .L0802CC52 @ case 61
    .4byte .L0802CC52 @ case 62
    .4byte .L0802CC52 @ case 63
    .4byte .L0802CC52 @ case 64
    .4byte .L0802CC52 @ case 65
    .4byte .L0802CC52 @ case 66
    .4byte .L0802CC52 @ case 67
    .4byte .L0802CC52 @ case 68
    .4byte .L0802CC52 @ case 69
    .4byte .L0802CC5A @ case 70
    .4byte .L0802CC62 @ case 71
    .4byte .L0802CC6A @ case 72
    .4byte .L0802CC72 @ case 73
    .4byte .L0802CC7A @ case 74
    .4byte .L0802CC82 @ case 75
    .4byte .L0802CC82 @ case 76
    .4byte .L0802CC82 @ case 77
    .4byte .L0802CC82 @ case 78
    .4byte .L0802CC82 @ case 79
    .4byte .L0802CC82 @ case 80
.L0802CB20:
    adds r1, r6, #0
    adds r1, #0x3f
    movs r0, #0
    b .L0802CC90
.L0802CB28:
    adds r1, r6, #0
    adds r1, #0x3f
    movs r0, #3
    b .L0802CC90
.L0802CB30:
    adds r1, r6, #0
    adds r1, #0x3f
    movs r0, #1
    b .L0802CC90
.L0802CB38:
    adds r1, r6, #0
    adds r1, #0x3f
    movs r0, #4
    b .L0802CC90
.L0802CB40:
    movs r0, #0xa
    ldrsh r5, [r6, r0]
    movs r1, #0xe
    ldrsh r4, [r6, r1]
    adds r0, r6, #0
    adds r0, #0x20
    ldrb r0, [r0]
    adds r1, r0, #0
    cmp r0, #1
    beq .L0802CB68
    cmp r0, #1
    bgt .L0802CB5E
    cmp r0, #0
    beq .L0802CB6C
    b .L0802CB76
.L0802CB5E:
    cmp r1, #2
    beq .L0802CB70
    cmp r1, #3
    beq .L0802CB74
    b .L0802CB76
.L0802CB68:
    subs r4, #0x10
    b .L0802CB76
.L0802CB6C:
    adds r4, #0x10
    b .L0802CB76
.L0802CB70:
    subs r5, #0x10
    b .L0802CB76
.L0802CB74:
    adds r5, #0x10
.L0802CB76:
    ldrh r2, [r6, #4]
    ldr r7, [r6]
    ldr r1, [r7]
    mov r0, sp
    ldr r3, [r1, #0x34]
    adds r1, r7, #0
    bl _call_via_r3
    mov r1, sp
    asrs r5, r5, #3
    asrs r3, r4, #3
    ldr r2, [sp]
    ldrh r0, [r1, #8]
    cmp r2, #0
    beq .L0802CBB6
    cmp r5, r0
    bhi .L0802CBB6
    ldrh r1, [r1, #0xa]
    cmp r3, r1
    bhi .L0802CBB6
    ldr r1, [sp, #4]
    muls r0, r3, r0
    adds r0, r0, r5
    cmp r1, #0
    beq .L0802CBAC
    adds r0, r1, r0
    ldrb r0, [r0]
.L0802CBAC:
    lsls r0, r0, #2
    adds r0, r2, r0
    b .L0802CBB8
.L0802CBB2:
    ldr r0, [r4]
    b .L0802CC18
.L0802CBB6:
    movs r0, #0
.L0802CBB8:
    cmp r0, #0
    beq .L0802CC20
    ldr r0, [r0]
    lsls r0, r0, #0x1e
    cmp r0, #0
    bge .L0802CC20
    adds r1, r6, #0
    adds r1, #0x3c
    movs r0, #0x1b
    strb r0, [r1]
    adds r1, #3
    movs r0, #0xf
    strb r0, [r1]
    ldr r4, [r6, #0x14]
    movs r5, #2
    rsbs r5, r5, #0
    adds r0, r6, #0
    bl func_08025B10
    adds r2, r0, #0
    ldr r3, [r4, #0x64]
    adds r0, r6, #0
    adds r1, r5, #0
    bl _call_via_r3
    ldr r0, [r7]
    movs r1, #0xa6
    lsls r1, r1, #1
    adds r0, r0, r1
    ldr r1, [r0]
    adds r0, r7, #0
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    cmp r4, r5
    beq .L0802CC14
.L0802CC02:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802CBB2
    adds r4, #4
    cmp r4, r5
    bne .L0802CC02
.L0802CC14:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802CC18:
    movs r1, #0x92
    bl func_08008B6C
    b .L0802CDC4
.L0802CC20:
    ldr r0, [r6, #0x38]
    bl func_0800E94C
    cmp r0, #0
    beq .L0802CC32
    adds r1, r6, #0
    adds r1, #0x3f
    movs r0, #2
    b .L0802CC90
.L0802CC32:
    adds r1, r6, #0
    adds r1, #0x3f
    movs r0, #0xf
    b .L0802CDC2
.L0802CC3A:
    adds r1, r6, #0
    adds r1, #0x3f
    movs r0, #5
    b .L0802CC90
.L0802CC42:
    adds r1, r6, #0
    adds r1, #0x3f
    movs r0, #6
    b .L0802CC90
.L0802CC4A:
    adds r1, r6, #0
    adds r1, #0x3f
    movs r0, #7
    b .L0802CC90
.L0802CC52:
    adds r1, r6, #0
    adds r1, #0x3f
    movs r0, #8
    b .L0802CC90
.L0802CC5A:
    adds r1, r6, #0
    adds r1, #0x3f
    movs r0, #9
    b .L0802CC90
.L0802CC62:
    adds r1, r6, #0
    adds r1, #0x3f
    movs r0, #0xa
    b .L0802CC90
.L0802CC6A:
    adds r1, r6, #0
    adds r1, #0x3f
    movs r0, #0xb
    b .L0802CC90
.L0802CC72:
    adds r1, r6, #0
    adds r1, #0x3f
    movs r0, #0xc
    b .L0802CC90
.L0802CC7A:
    adds r1, r6, #0
    adds r1, #0x3f
    movs r0, #0xd
    b .L0802CC90
.L0802CC82:
    adds r1, r6, #0
    adds r1, #0x3f
    movs r0, #0xe
    b .L0802CC90
.L0802CC8A:
    adds r1, r6, #0
    adds r1, #0x3f
    movs r0, #0xf
.L0802CC90:
    strb r0, [r1]
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
    strb r1, [r0]
    bl GetId__C4Tool
    cmp r0, #0x2f
    bls .L0802CCB6
    b .L0802CDBC
.L0802CCB6:
    lsls r0, r0, #2
    ldr r1, .L0802CCC0 @ =.L0802CCC4
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L0802CCC0: .4byte .L0802CCC4
.L0802CCC4: @ jump table
    .4byte .L0802CD84 @ case 0
    .4byte .L0802CD8C @ case 1
    .4byte .L0802CD94 @ case 2
    .4byte .L0802CD9C @ case 3
    .4byte .L0802CDA4 @ case 4
    .4byte .L0802CDAC @ case 5
    .4byte .L0802CDAC @ case 6
    .4byte .L0802CDB4 @ case 7
    .4byte .L0802CD84 @ case 8
    .4byte .L0802CD8C @ case 9
    .4byte .L0802CD94 @ case 10
    .4byte .L0802CD9C @ case 11
    .4byte .L0802CDA4 @ case 12
    .4byte .L0802CDAC @ case 13
    .4byte .L0802CDAC @ case 14
    .4byte .L0802CDB4 @ case 15
    .4byte .L0802CD84 @ case 16
    .4byte .L0802CD8C @ case 17
    .4byte .L0802CD94 @ case 18
    .4byte .L0802CD9C @ case 19
    .4byte .L0802CDA4 @ case 20
    .4byte .L0802CDAC @ case 21
    .4byte .L0802CDAC @ case 22
    .4byte .L0802CDB4 @ case 23
    .4byte .L0802CD84 @ case 24
    .4byte .L0802CD8C @ case 25
    .4byte .L0802CD94 @ case 26
    .4byte .L0802CD9C @ case 27
    .4byte .L0802CDA4 @ case 28
    .4byte .L0802CDAC @ case 29
    .4byte .L0802CDAC @ case 30
    .4byte .L0802CDB4 @ case 31
    .4byte .L0802CD84 @ case 32
    .4byte .L0802CD8C @ case 33
    .4byte .L0802CD94 @ case 34
    .4byte .L0802CD9C @ case 35
    .4byte .L0802CDA4 @ case 36
    .4byte .L0802CDAC @ case 37
    .4byte .L0802CDAC @ case 38
    .4byte .L0802CDB4 @ case 39
    .4byte .L0802CD84 @ case 40
    .4byte .L0802CD8C @ case 41
    .4byte .L0802CD94 @ case 42
    .4byte .L0802CD9C @ case 43
    .4byte .L0802CDA4 @ case 44
    .4byte .L0802CDAC @ case 45
    .4byte .L0802CDAC @ case 46
    .4byte .L0802CDB4 @ case 47
.L0802CD84:
    adds r1, r6, #0
    adds r1, #0x41
    movs r0, #1
    b .L0802CDC2
.L0802CD8C:
    adds r1, r6, #0
    adds r1, #0x41
    movs r0, #2
    b .L0802CDC2
.L0802CD94:
    adds r1, r6, #0
    adds r1, #0x41
    movs r0, #3
    b .L0802CDC2
.L0802CD9C:
    adds r1, r6, #0
    adds r1, #0x41
    movs r0, #4
    b .L0802CDC2
.L0802CDA4:
    adds r1, r6, #0
    adds r1, #0x41
    movs r0, #5
    b .L0802CDC2
.L0802CDAC:
    adds r1, r6, #0
    adds r1, #0x41
    movs r0, #6
    b .L0802CDC2
.L0802CDB4:
    adds r1, r6, #0
    adds r1, #0x41
    movs r0, #7
    b .L0802CDC2
.L0802CDBC:
    adds r1, r6, #0
    adds r1, #0x41
    movs r0, #0
.L0802CDC2:
    strb r0, [r1]
.L0802CDC4:
    add sp, #0x10
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0

    thumb_func_start func_0802CDCC
func_0802CDCC: @ 0x0802CDCC
    push {r4, r5, r6, r7, lr}
    mov r7, sl
    mov r6, sb
    mov r5, r8
    push {r5, r6, r7}
    sub sp, #0x80
    adds r7, r0, #0
    adds r1, r7, #0
    adds r1, #0xb3
    ldrb r0, [r1]
    cmp r0, #0
    beq .L0802CE22
    subs r0, #1
    strb r0, [r1]
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802CE14
    movs r4, #0x2b
.L0802CDF0:
    ldr r0, [r7]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    adds r1, r4, #0
    bl _call_via_r2
    adds r2, r0, #0
    cmp r2, #0
    beq .L0802CE0C
    ldr r0, [r2, #0x14]
    ldr r1, [r0, #0x40]
    adds r0, r2, #0
    bl _call_via_r1
.L0802CE0C:
    adds r4, #1
    cmp r4, #0x45
    bls .L0802CDF0
    b .L0802CE30
.L0802CE14:
    adds r1, r7, #0
    adds r1, #0xb4
    ldrb r0, [r1]
    cmp r0, #0x59
    bhi .L0802CE30
    adds r0, #1
    b .L0802CE2E
.L0802CE22:
    adds r1, r7, #0
    adds r1, #0xb4
    ldrb r0, [r1]
    cmp r0, #0
    beq .L0802CE30
    subs r0, #1
.L0802CE2E:
    strb r0, [r1]
.L0802CE30:
    ldr r0, [r7]
    str r0, [sp, #0x54]
    ldrh r1, [r7, #4]
    str r1, [sp, #0x58]
    ldr r0, [r7, #0x18]
    cmp r0, #0
    bne .L0802CE46
    ldr r1, [r7, #0x1c]
    cmp r1, #0
    bne .L0802CE46
    b .L0802CFEA
.L0802CE46:
    adds r0, r7, #0
    adds r0, #0x3c
    ldrb r0, [r0]
    cmp r0, #0x2c
    bne .L0802CE52
    b .L0802CFEA
.L0802CE52:
    cmp r0, #0x35
    bne .L0802CE58
    b .L0802CFEA
.L0802CE58:
    ldr r1, [r7, #0x14]
    add r0, sp, #0xc
    ldr r2, [r1, #0xc]
    adds r1, r7, #0
    bl _call_via_r2
    ldr r0, [sp, #0xc]
    ldr r1, [sp, #0x10]
    str r0, [sp, #0x14]
    str r1, [sp, #0x18]
    add r2, sp, #0x14
    mov sb, r2
    ldr r0, [r7, #0x14]
    ldr r1, [r0, #0x40]
    adds r0, r7, #0
    bl _call_via_r1
    movs r1, #1
    eors r0, r1
    lsls r0, r0, #0x18
    lsrs r0, r0, #0x18
    str r0, [sp, #0x5c]
    adds r0, r7, #0
    adds r0, #0x20
    ldrb r0, [r0]
    str r0, [sp, #0x60]
    adds r0, r7, #0
    adds r0, #0xb4
    ldrb r0, [r0]
    str r0, [sp, #0x64]
    movs r4, #0x1a
    ldrsh r3, [r7, r4]
    movs r5, #0x1e
    ldrsh r4, [r7, r5]
    mov r6, sb
    movs r1, #0
    ldrsh r0, [r6, r1]
    movs r5, #2
    ldrsh r2, [r6, r5]
    movs r1, #4
    ldrsh r5, [r6, r1]
    movs r1, #6
    ldrsh r6, [r6, r1]
    mov r8, r6
    add r1, sp, #0x1c
    adds r0, r3, r0
    strh r0, [r1]
    adds r2, r4, r2
    strh r2, [r1, #2]
    adds r3, r3, r5
    strh r3, [r1, #4]
    add r4, r8
    strh r4, [r1, #6]
    ldr r0, [sp, #0x1c]
    ldr r1, [sp, #0x20]
    str r0, [sp, #0x24]
    str r1, [sp, #0x28]
    add r2, sp, #0x24
    mov sl, r2
    adds r0, r7, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #2
    beq .L0802CEE8
    movs r3, #0x3c
    str r3, [sp, #0x68]
    ldr r4, .L0802CEE4 @ =gUnk_080F0E88
    str r4, [sp, #0x6c]
    b .L0802CEF0
    .align 2, 0
.L0802CEE4: .4byte gUnk_080F0E88
.L0802CEE8:
    movs r5, #0x15
    str r5, [sp, #0x68]
    ldr r6, .L0802D010 @ =gUnk_080F0F78
    str r6, [sp, #0x6c]
.L0802CEF0:
    movs r0, #0
    str r0, [sp, #0x70]
    ldr r1, [sp, #0x68]
    cmp r0, r1
    bhs .L0802CFEA
    add r2, sp, #0x34
    mov r8, r2
.L0802CEFE:
    ldr r3, [sp, #0x70]
    lsls r0, r3, #2
    ldr r4, [sp, #0x6c]
    adds r0, r0, r4
    ldr r0, [r0]
    str r0, [sp, #0x74]
    ldr r5, [sp, #0x54]
    ldr r0, [r5]
    ldr r2, [r0, #0x40]
    adds r0, r5, #0
    ldr r1, [sp, #0x74]
    bl _call_via_r2
    adds r3, r0, #0
    str r3, [sp, #0x78]
    cmp r3, #0
    beq .L0802CFDE
    ldrh r0, [r3, #4]
    ldr r6, [sp, #0x58]
    cmp r0, r6
    bne .L0802CFDE
    ldr r1, [r3, #0x14]
    add r0, sp, #0x2c
    ldr r2, [r1, #0xc]
    adds r1, r3, #0
    bl _call_via_r2
    ldr r0, [sp, #0x2c]
    ldr r1, [sp, #0x30]
    str r0, [sp, #0x34]
    str r1, [sp, #0x38]
    mov r0, r8
    movs r1, #0
    ldrsh r0, [r0, r1]
    mov ip, r0
    mov r2, sb
    movs r3, #4
    ldrsh r1, [r2, r3]
    subs r1, r0, r1
    movs r4, #0
    ldrsh r0, [r2, r4]
    mov r5, r8
    movs r6, #4
    ldrsh r4, [r5, r6]
    subs r0, r0, r4
    ands r1, r0
    movs r2, #2
    ldrsh r0, [r5, r2]
    str r0, [sp, #0x7c]
    mov r3, sb
    movs r5, #6
    ldrsh r0, [r3, r5]
    ldr r6, [sp, #0x7c]
    subs r0, r6, r0
    ands r1, r0
    movs r2, #2
    ldrsh r0, [r3, r2]
    mov r5, r8
    movs r6, #6
    ldrsh r3, [r5, r6]
    subs r0, r0, r3
    ands r1, r0
    lsrs r2, r1, #0x1f
    cmp r2, #0
    bne .L0802CFDE
    mov r0, sl
    movs r5, #4
    ldrsh r1, [r0, r5]
    mov r6, ip
    subs r1, r6, r1
    movs r5, #0
    ldrsh r0, [r0, r5]
    subs r0, r0, r4
    ands r1, r0
    mov r6, sl
    movs r4, #6
    ldrsh r0, [r6, r4]
    ldr r5, [sp, #0x7c]
    subs r0, r5, r0
    ands r1, r0
    movs r4, #2
    ldrsh r0, [r6, r4]
    subs r0, r0, r3
    ands r1, r0
    cmp r1, #0
    bge .L0802CFDE
    str r2, [r7, #0x18]
    str r2, [r7, #0x1c]
    ldr r5, [sp, #0x5c]
    cmp r5, #0
    beq .L0802CFDE
    movs r1, #0
    ldr r0, [sp, #0x74]
    subs r0, #0x2b
    cmp r0, #0x1a
    bhi .L0802CFC0
    movs r1, #1
.L0802CFC0:
    cmp r1, #0
    beq .L0802CFDE
    ldr r6, [sp, #0x78]
    ldr r0, [r6, #0x14]
    adds r0, #0x84
    ldr r3, [r0]
    adds r0, r6, #0
    ldr r1, [sp, #0x60]
    ldr r2, [sp, #0x64]
    bl _call_via_r3
    adds r1, r7, #0
    adds r1, #0xb3
    movs r0, #8
    strb r0, [r1]
.L0802CFDE:
    ldr r0, [sp, #0x70]
    adds r0, #1
    str r0, [sp, #0x70]
    ldr r1, [sp, #0x68]
    cmp r0, r1
    blo .L0802CEFE
.L0802CFEA:
    ldr r0, [r7, #0x18]
    ldr r1, [r7, #0x1c]
    cmp r0, #0
    bne .L0802CFF8
    cmp r1, #0
    bne .L0802CFF8
    b .L0802D148
.L0802CFF8:
    ldr r5, [r7, #8]
    ldr r6, [r7, #0xc]
    adds r0, r0, r5
    mov r8, r0
    adds r1, r1, r6
    mov sb, r1
    cmp r0, #0
    bge .L0802D014
    movs r2, #0
    mov r8, r2
    b .L0802D02A
    .align 2, 0
.L0802D010: .4byte gUnk_080F0F78
.L0802D014:
    ldr r3, [sp, #0x54]
    ldr r0, [r3]
    ldr r2, [r0, #0x2c]
    adds r0, r3, #0
    ldr r1, [sp, #0x58]
    bl _call_via_r2
    lsls r0, r0, #0x10
    cmp r8, r0
    ble .L0802D02A
    mov r8, r0
.L0802D02A:
    mov r4, sb
    cmp r4, #0
    bge .L0802D034
    movs r0, #0
    b .L0802D048
.L0802D034:
    ldr r1, [sp, #0x54]
    ldr r0, [r1]
    ldr r2, [r0, #0x30]
    adds r0, r1, #0
    ldr r1, [sp, #0x58]
    bl _call_via_r2
    lsls r0, r0, #0x10
    cmp sb, r0
    ble .L0802D04A
.L0802D048:
    mov sb, r0
.L0802D04A:
    adds r0, r7, #0
    adds r0, #0x3c
    ldrb r0, [r0]
    subs r0, #0x2c
    lsls r0, r0, #0x18
    lsrs r0, r0, #0x18
    cmp r0, #1
    bls .L0802D0DA
    ldr r4, [sp, #0x54]
    ldr r1, [r4]
    mov r0, sp
    ldr r3, [r1, #0x34]
    adds r1, r4, #0
    ldr r2, [sp, #0x58]
    bl _call_via_r3
    ldr r1, [r7, #0x14]
    add r0, sp, #0x3c
    ldr r2, [r1, #0xc]
    adds r1, r7, #0
    bl _call_via_r2
    ldr r0, [sp, #0x3c]
    ldr r1, [sp, #0x40]
    str r0, [sp, #0x44]
    str r1, [sp, #0x48]
    add r4, sp, #0x44
    mov r0, sp
    adds r1, r4, #0
    bl func_080AC070
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802D098
    mov r5, r8
    str r5, [r7, #8]
    mov r6, sb
    str r6, [r7, #0xc]
    b .L0802D122
.L0802D098:
    mov r0, r8
    asrs r1, r0, #0x10
    asrs r0, r5, #0x10
    subs r5, r1, r0
    mov r2, sb
    asrs r1, r2, #0x10
    asrs r0, r6, #0x10
    subs r6, r1, r0
    movs r3, #0
    ldrsh r0, [r4, r3]
    movs r1, #2
    ldrsh r2, [r4, r1]
    movs r1, #4
    ldrsh r3, [r4, r1]
    movs r1, #6
    ldrsh r4, [r4, r1]
    add r1, sp, #0x4c
    adds r0, r5, r0
    strh r0, [r1]
    adds r2, r6, r2
    strh r2, [r1, #2]
    adds r3, r5, r3
    strh r3, [r1, #4]
    adds r4, r6, r4
    strh r4, [r1, #6]
    mov r0, sp
    bl func_080AC070
    movs r1, #1
    eors r0, r1
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802D0E4
.L0802D0DA:
    mov r2, r8
    str r2, [r7, #8]
    mov r3, sb
    str r3, [r7, #0xc]
    b .L0802D122
.L0802D0E4:
    cmp r5, #0
    ble .L0802D0F4
    adds r0, r7, #0
    movs r1, #3
    adds r2, r5, #0
    bl func_0802536C
    b .L0802D122
.L0802D0F4:
    cmp r5, #0
    bge .L0802D104
    rsbs r2, r5, #0
    adds r0, r7, #0
    movs r1, #2
    bl func_0802536C
    b .L0802D122
.L0802D104:
    cmp r6, #0
    ble .L0802D114
    adds r0, r7, #0
    movs r1, #0
    adds r2, r6, #0
    bl func_0802536C
    b .L0802D122
.L0802D114:
    cmp r6, #0
    bge .L0802D122
    rsbs r2, r6, #0
    adds r0, r7, #0
    movs r1, #1
    bl func_0802536C
.L0802D122:
    ldr r0, [r7, #0x14]
    ldr r1, [r0, #0x40]
    adds r0, r7, #0
    bl _call_via_r1
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802D148
    ldr r4, [sp, #0x54]
    ldr r0, [r4]
    ldr r2, [r0, #0x40]
    adds r0, r4, #0
    movs r1, #0x2c
    bl _call_via_r2
    ldr r1, [r7, #8]
    str r1, [r0, #8]
    ldr r1, [r7, #0xc]
    str r1, [r0, #0xc]
.L0802D148:
    add sp, #0x80
    pop {r3, r4, r5}
    mov r8, r3
    mov sb, r4
    mov sl, r5
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0

    thumb_func_start func_0802D158
func_0802D158: @ 0x0802D158
    push {r4, r5, r6, lr}
    sub sp, #8
    adds r5, r0, #0
    movs r6, #0xc9
    lsls r6, r6, #1
    adds r0, #0x7c
    ldr r1, .L0802D17C @ =0x0000FFFF
    strh r1, [r0]
    subs r0, #0x40
    ldrb r0, [r0]
    cmp r0, #0x39
    bls .L0802D172
    b .L0802D590
.L0802D172:
    lsls r0, r0, #2
    ldr r1, .L0802D180 @ =.L0802D184
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L0802D17C: .4byte 0x0000FFFF
.L0802D180: .4byte .L0802D184
.L0802D184: @ jump table
    .4byte .L0802D26C @ case 0
    .4byte .L0802D346 @ case 1
    .4byte .L0802D2E0 @ case 2
    .4byte .L0802D346 @ case 3
    .4byte .L0802D330 @ case 4
    .4byte .L0802D346 @ case 5
    .4byte .L0802D466 @ case 6
    .4byte .L0802D472 @ case 7
    .4byte .L0802D582 @ case 8
    .4byte .L0802D37E @ case 9
    .4byte .L0802D566 @ case 10
    .4byte .L0802D364 @ case 11
    .4byte .L0802D364 @ case 12
    .4byte .L0802D35E @ case 13
    .4byte .L0802D364 @ case 14
    .4byte .L0802D36A @ case 15
    .4byte .L0802D36E @ case 16
    .4byte .L0802D554 @ case 17
    .4byte .L0802D560 @ case 18
    .4byte .L0802D372 @ case 19
    .4byte .L0802D378 @ case 20
    .4byte .L0802D566 @ case 21
    .4byte .L0802D45A @ case 22
    .4byte .L0802D460 @ case 23
    .4byte .L0802D46C @ case 24
    .4byte .L0802D3CC @ case 25
    .4byte .L0802D3CC @ case 26
    .4byte .L0802D39E @ case 27
    .4byte .L0802D3BC @ case 28
    .4byte .L0802D3BC @ case 29
    .4byte .L0802D3FC @ case 30
    .4byte .L0802D416 @ case 31
    .4byte .L0802D432 @ case 32
    .4byte .L0802D554 @ case 33
    .4byte .L0802D4BC @ case 34
    .4byte .L0802D4D8 @ case 35
    .4byte .L0802D4F0 @ case 36
    .4byte .L0802D4F6 @ case 37
    .4byte .L0802D4FC @ case 38
    .4byte .L0802D500 @ case 39
    .4byte .L0802D504 @ case 40
    .4byte .L0802D50A @ case 41
    .4byte .L0802D510 @ case 42
    .4byte .L0802D510 @ case 43
    .4byte .L0802D516 @ case 44
    .4byte .L0802D52C @ case 45
    .4byte .L0802D550 @ case 46
    .4byte .L0802D566 @ case 47
    .4byte .L0802D55A @ case 48
    .4byte .L0802D554 @ case 49
    .4byte .L0802D55A @ case 50
    .4byte .L0802D560 @ case 51
    .4byte .L0802D566 @ case 52
    .4byte .L0802D56A @ case 53
    .4byte .L0802D554 @ case 54
    .4byte .L0802D554 @ case 55
    .4byte .L0802D55A @ case 56
    .4byte .L0802D55A @ case 57
.L0802D26C:
    ldr r0, [r5, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802D29E
    adds r0, r5, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    bne .L0802D286
    b .L0802D57C
.L0802D286:
    cmp r0, #1
    bgt .L0802D292
    cmp r0, #0
    bne .L0802D290
    b .L0802D55A
.L0802D290:
    b .L0802D590
.L0802D292:
    cmp r0, #2
    beq .L0802D298
    b .L0802D590
.L0802D298:
    movs r6, #0xa7
    lsls r6, r6, #1
    b .L0802D590
.L0802D29E:
    adds r0, r5, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    beq .L0802D2BA
    cmp r0, #1
    bgt .L0802D2B4
    cmp r0, #0
    bne .L0802D2B2
    b .L0802D3CC
.L0802D2B2:
    b .L0802D590
.L0802D2B4:
    cmp r0, #2
    beq .L0802D2CC
    b .L0802D590
.L0802D2BA:
    adds r0, r5, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L0802D2DC
    adds r1, #0x94
    b .L0802D2DC
.L0802D2CC:
    adds r0, r5, #0
    adds r0, #0xb3
    ldrb r0, [r0]
    movs r1, #0x99
    lsls r1, r1, #1
    cmp r0, #0
    bne .L0802D2DC
    adds r1, #0x2c
.L0802D2DC:
    adds r6, r1, #0
    b .L0802D590
.L0802D2E0:
    ldr r0, [r5, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802D2F0
    b .L0802D55A
.L0802D2F0:
    adds r0, r5, #0
    adds r0, #0x3f
    ldrb r3, [r0]
    cmp r3, #5
    bgt .L0802D3CC
    cmp r3, #0
    blt .L0802D3CC
    ldr r2, .L0802D32C @ =gUnk_080F0FCC
    adds r1, r5, #0
    adds r1, #0x40
    lsls r0, r3, #3
    subs r0, r0, r3
    ldrb r1, [r1]
    adds r0, r0, r1
    lsls r0, r0, #1
    adds r0, r0, r2
    ldrh r6, [r0]
    ldr r4, [r5, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    mov r1, sp
    strb r0, [r1]
    mov r0, sp
    b .L0802D44E
    .align 2, 0
.L0802D32C: .4byte gUnk_080F0FCC
.L0802D330:
    ldr r0, [r5, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r6, #0xc9
    lsls r6, r6, #1
    cmp r0, #0
    beq .L0802D344
    b .L0802D590
.L0802D344:
    b .L0802D55A
.L0802D346:
    ldr r0, [r5, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r6, #0xc9
    lsls r6, r6, #1
    cmp r0, #0
    beq .L0802D35A
    b .L0802D590
.L0802D35A:
    subs r6, #0x40
    b .L0802D590
.L0802D35E:
    movs r6, #0x9b
    lsls r6, r6, #1
    b .L0802D590
.L0802D364:
    movs r6, #0xcf
    lsls r6, r6, #1
    b .L0802D590
.L0802D36A:
    movs r6, #0x4e
    b .L0802D590
.L0802D36E:
    movs r6, #0x46
    b .L0802D590
.L0802D372:
    movs r6, #0x9f
    lsls r6, r6, #1
    b .L0802D590
.L0802D378:
    movs r6, #0xa1
    lsls r6, r6, #1
    b .L0802D590
.L0802D37E:
    adds r0, r5, #0
    adds r0, #0x44
    ldrh r6, [r0]
    ldr r4, [r5, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #1
    b .L0802D44C
.L0802D39E:
    movs r6, #0xf5
    lsls r6, r6, #1
    ldr r4, [r5, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #2
    b .L0802D44C
.L0802D3BC:
    adds r2, r5, #0
    adds r2, #0x4e
    adds r1, r5, #0
    adds r1, #0x4f
    ldrb r0, [r2]
    ldrb r3, [r1]
    cmp r0, r3
    bne .L0802D3D2
.L0802D3CC:
    movs r6, #0xc9
    lsls r6, r6, #1
    b .L0802D590
.L0802D3D2:
    ldrb r0, [r1]
    ldrb r1, [r2]
    subs r0, r0, r1
    movs r6, #0xc1
    lsls r6, r6, #1
    cmp r0, #1
    bne .L0802D3E2
    adds r6, #4
.L0802D3E2:
    ldr r4, [r5, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #3
    b .L0802D44C
.L0802D3FC:
    movs r6, #0x76
    ldr r4, [r5, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    add r0, sp, #4
    b .L0802D44C
.L0802D416:
    movs r6, #0x6e
    ldr r4, [r5, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #5
    b .L0802D44C
.L0802D432:
    movs r6, #0x72
    ldr r4, [r5, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #6
.L0802D44C:
    strb r1, [r0]
.L0802D44E:
    bl GetIconId__C4Tool
    adds r1, r5, #0
    adds r1, #0x7c
    strh r0, [r1]
    b .L0802D590
.L0802D45A:
    movs r6, #0xfd
    lsls r6, r6, #1
    b .L0802D590
.L0802D460:
    movs r6, #0xcb
    lsls r6, r6, #1
    b .L0802D590
.L0802D466:
    movs r6, #0x89
    lsls r6, r6, #1
    b .L0802D590
.L0802D46C:
    movs r6, #0x8b
    lsls r6, r6, #1
    b .L0802D590
.L0802D472:
    ldr r0, [r5]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    adds r0, r5, #0
    adds r0, #0x3e
    ldrb r0, [r0]
    cmp r0, #1
    beq .L0802D4A4
    cmp r0, #1
    bgt .L0802D494
    cmp r0, #0
    beq .L0802D49A
    b .L0802D590
.L0802D494:
    cmp r0, #2
    beq .L0802D4B0
    b .L0802D590
.L0802D49A:
    movs r6, #0xea
    ldr r1, .L0802D4A0 @ =0x0000077A
    b .L0802D53E
    .align 2, 0
.L0802D4A0: .4byte 0x0000077A
.L0802D4A4:
    movs r6, #0xee
    ldr r1, .L0802D4AC @ =0x0000077E
    b .L0802D53E
    .align 2, 0
.L0802D4AC: .4byte 0x0000077E
.L0802D4B0:
    movs r6, #0xe6
    ldr r1, .L0802D4B8 @ =0x00000782
    b .L0802D53E
    .align 2, 0
.L0802D4B8: .4byte 0x00000782
.L0802D4BC:
    movs r6, #0xad
    lsls r6, r6, #1
    ldr r0, [r5]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L0802D4D4 @ =0x0000077A
    b .L0802D53E
    .align 2, 0
.L0802D4D4: .4byte 0x0000077A
.L0802D4D8:
    movs r6, #0x9a
    ldr r0, [r5]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L0802D4EC @ =0x0000077A
    b .L0802D53E
    .align 2, 0
.L0802D4EC: .4byte 0x0000077A
.L0802D4F0:
    movs r6, #0x83
    lsls r6, r6, #1
    b .L0802D590
.L0802D4F6:
    movs r6, #0x81
    lsls r6, r6, #1
    b .L0802D590
.L0802D4FC:
    movs r6, #0xfe
    b .L0802D590
.L0802D500:
    movs r6, #0xfa
    b .L0802D590
.L0802D504:
    movs r6, #0x91
    lsls r6, r6, #1
    b .L0802D590
.L0802D50A:
    movs r6, #0x8f
    lsls r6, r6, #1
    b .L0802D590
.L0802D510:
    movs r6, #0x8d
    lsls r6, r6, #1
    b .L0802D590
.L0802D516:
    ldr r0, [r5, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r6, #0xa5
    lsls r6, r6, #1
    cmp r0, #0
    beq .L0802D590
    subs r6, #0x40
    b .L0802D590
.L0802D52C:
    movs r6, #0xe2
    ldr r0, [r5]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r2, r0, #0
    ldr r1, .L0802D54C @ =0x00000792
.L0802D53E:
    ldrh r0, [r2, #0x22]
    cmp r0, r1
    beq .L0802D590
    adds r0, r2, #0
    bl SetAnim__12AActorEntityUi
    b .L0802D590
    .align 2, 0
.L0802D54C: .4byte 0x00000792
.L0802D550:
    movs r6, #0x3e
    b .L0802D590
.L0802D554:
    movs r6, #0xa3
    lsls r6, r6, #1
    b .L0802D590
.L0802D55A:
    movs r6, #0xa9
    lsls r6, r6, #1
    b .L0802D590
.L0802D560:
    movs r6, #0x9d
    lsls r6, r6, #1
    b .L0802D590
.L0802D566:
    movs r6, #0x42
    b .L0802D590
.L0802D56A:
    ldr r0, [r5, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    movs r6, #0xe3
    lsls r6, r6, #1
    cmp r0, #0
    bne .L0802D590
.L0802D57C:
    movs r6, #0xab
    lsls r6, r6, #1
    b .L0802D590
.L0802D582:
    adds r0, r5, #0
    adds r0, #0x3d
    ldrb r0, [r0]
    movs r6, #0xf6
    cmp r0, #0
    beq .L0802D590
    movs r6, #0xf2
.L0802D590:
    adds r0, r6, #0
    add sp, #8
    pop {r4, r5, r6}
    pop {r1}
    bx r1
    .align 2, 0

    thumb_func_start func_0802D59C
func_0802D59C: @ 0x0802D59C
    push {r4, r5, r6, r7, lr}
    mov r7, sl
    mov r6, sb
    mov r5, r8
    push {r5, r6, r7}
    sub sp, #0x20
    adds r6, r0, #0
    movs r2, #0
    str r2, [sp, #0x1c]
    mov r8, r2
    cmp r1, #0
    beq .L0802D5BC
    ldrh r0, [r1]
    str r0, [sp, #0x1c]
    ldrh r1, [r1, #4]
    mov r8, r1
.L0802D5BC:
    adds r0, r6, #0
    adds r0, #0x3e
    strb r2, [r0]
    adds r1, r6, #0
    adds r1, #0xc3
    ldrb r2, [r1]
    adds r7, r0, #0
    mov sl, r1
    movs r0, #0x20
    adds r0, r0, r6
    mov sb, r0
    cmp r2, #0
    beq .L0802D5D8
    b .L0802D71C
.L0802D5D8:
    movs r0, #1
    mov r1, r8
    ands r0, r1
    cmp r0, #0
    bne .L0802D5E4
    b .L0802D71C
.L0802D5E4:
    mov r0, sp
    adds r1, r6, #0
    bl GetLocation__C7AEntity
    mov r2, sb
    ldrb r0, [r2]
    cmp r0, #1
    beq .L0802D608
    cmp r0, #1
    bgt .L0802D5FE
    cmp r0, #0
    beq .L0802D62C
    b .L0802D68E
.L0802D5FE:
    cmp r0, #2
    beq .L0802D670
    cmp r0, #3
    beq .L0802D67A
    b .L0802D68E
.L0802D608:
    mov r3, sp
    mov r0, sp
    ldrb r4, [r0, #3]
    lsrs r2, r4, #2
    ldrh r1, [r0, #4]
    ldr r5, .L0802D624 @ =0x000003FF
    ands r1, r5
    lsls r1, r1, #6
    orrs r1, r2
    lsls r1, r1, #0x10
    ldr r0, .L0802D628 @ =0xFFF00000
    adds r1, r1, r0
    b .L0802D646
    .align 2, 0
.L0802D624: .4byte 0x000003FF
.L0802D628: .4byte 0xFFF00000
.L0802D62C:
    mov r3, sp
    mov r0, sp
    ldrb r4, [r0, #3]
    lsrs r2, r4, #2
    ldrh r1, [r0, #4]
    ldr r5, .L0802D668 @ =0x000003FF
    ands r1, r5
    lsls r1, r1, #6
    orrs r1, r2
    lsls r1, r1, #0x10
    movs r2, #0x80
    lsls r2, r2, #0xd
    adds r1, r1, r2
.L0802D646:
    lsrs r2, r1, #0x10
    movs r0, #0x3f
    ands r2, r0
    lsls r2, r2, #2
    movs r0, #3
    ands r0, r4
    orrs r0, r2
    strb r0, [r3, #3]
    lsrs r1, r1, #0x16
    ands r1, r5
    ldrh r2, [r3, #4]
    ldr r0, .L0802D66C @ =0xFFFFFC00
    ands r0, r2
    orrs r0, r1
    strh r0, [r3, #4]
    b .L0802D68E
    .align 2, 0
.L0802D668: .4byte 0x000003FF
.L0802D66C: .4byte 0xFFFFFC00
.L0802D670:
    ldr r0, [sp]
    lsls r1, r0, #6
    asrs r1, r1, #0x10
    subs r1, #0x10
    b .L0802D682
.L0802D67A:
    ldr r0, [sp]
    lsls r1, r0, #6
    asrs r1, r1, #0x10
    adds r1, #0x10
.L0802D682:
    lsls r1, r1, #0x10
    lsrs r1, r1, #6
    ldr r2, .L0802D6E0 @ =0xFC0003FF
    ands r2, r0
    orrs r2, r1
    str r2, [sp]
.L0802D68E:
    ldrh r2, [r6, #4]
    ldr r1, [r6]
    add r4, sp, #8
    ldr r3, [r1]
    adds r0, r4, #0
    ldr r3, [r3, #0x34]
    bl _call_via_r3
    ldr r0, [sp]
    lsls r0, r0, #6
    asrs r5, r0, #0x13
    mov r0, sp
    ldrb r2, [r0, #3]
    lsrs r2, r2, #2
    ldrh r0, [r0, #4]
    ldr r1, .L0802D6E4 @ =0x000003FF
    ands r0, r1
    lsls r0, r0, #6
    orrs r0, r2
    lsls r0, r0, #0x10
    asrs r3, r0, #0x13
    ldr r2, [sp, #8]
    ldrh r0, [r4, #8]
    cmp r2, #0
    beq .L0802D6E8
    cmp r5, r0
    bhi .L0802D6E8
    ldrh r1, [r4, #0xa]
    cmp r3, r1
    bhi .L0802D6E8
    ldr r1, [r4, #4]
    muls r0, r3, r0
    adds r0, r0, r5
    cmp r1, #0
    beq .L0802D6D8
    adds r0, r1, r0
    ldrb r0, [r0]
.L0802D6D8:
    lsls r0, r0, #2
    adds r0, r2, r0
    adds r2, r0, #0
    b .L0802D6EA
    .align 2, 0
.L0802D6E0: .4byte 0xFC0003FF
.L0802D6E4: .4byte 0x000003FF
.L0802D6E8:
    movs r2, #0
.L0802D6EA:
    cmp r2, #0
    beq .L0802D71C
    ldrh r1, [r2, #2]
    ldr r0, .L0802D718 @ =0x0000FFFE
    ands r0, r1
    cmp r0, #0
    beq .L0802D71C
    ldr r0, [r6]
    ldr r3, [r0]
    movs r1, #0xa0
    lsls r1, r1, #1
    adds r3, r3, r1
    ldr r2, [r2]
    lsrs r2, r2, #0x11
    add r1, sp, #0x14
    movs r4, #0
    str r2, [sp, #0x14]
    str r4, [r1, #4]
    ldr r3, [r3]
    movs r2, #0
    bl _call_via_r3
    b .L0802D832
    .align 2, 0
.L0802D718: .4byte 0x0000FFFE
.L0802D71C:
    movs r3, #0
    movs r5, #0
    mov r2, sb
    ldrb r4, [r2]
    mov r1, sl
    ldrb r0, [r1]
    cmp r0, #0
    bne .L0802D736
    movs r0, #3
    mov r2, r8
    ands r2, r0
    cmp r2, #0
    beq .L0802D740
.L0802D736:
    adds r1, r6, #0
    adds r1, #0x3c
    movs r0, #0x23
    strb r0, [r1]
    b .L0802D7F8
.L0802D740:
    ldr r1, [sp, #0x1c]
    lsls r2, r1, #0x19
    lsrs r2, r2, #0x1f
    lsls r0, r1, #0x18
    lsrs r0, r0, #0x1f
    orrs r2, r0
    lsls r0, r1, #0x1a
    lsrs r0, r0, #0x1f
    orrs r0, r2
    lsls r2, r1, #0x1b
    lsrs r2, r2, #0x1f
    orrs r2, r0
    lsls r1, r1, #0x17
    lsrs r1, r1, #0x1f
    orrs r1, r2
    cmp r1, #0
    beq .L0802D7F8
    add r1, sp, #0x1c
    ldrb r2, [r1]
    movs r0, #0x20
    ands r0, r2
    cmp r0, #0
    beq .L0802D772
    movs r4, #2
    b .L0802D794
.L0802D772:
    movs r0, #0x10
    ands r0, r2
    cmp r0, #0
    beq .L0802D77E
    movs r4, #3
    b .L0802D794
.L0802D77E:
    movs r0, #0x40
    ands r0, r2
    cmp r0, #0
    beq .L0802D78A
    movs r4, #1
    b .L0802D794
.L0802D78A:
    movs r0, #0x80
    ands r0, r2
    cmp r0, #0
    beq .L0802D794
    movs r4, #0
.L0802D794:
    cmp r4, #1
    beq .L0802D7B4
    cmp r4, #1
    bgt .L0802D7A2
    cmp r4, #0
    beq .L0802D7B8
    b .L0802D7BA
.L0802D7A2:
    cmp r4, #2
    beq .L0802D7AC
    cmp r4, #3
    beq .L0802D7B0
    b .L0802D7BA
.L0802D7AC:
    movs r3, #0xfe
    b .L0802D7BA
.L0802D7B0:
    movs r3, #2
    b .L0802D7BA
.L0802D7B4:
    movs r5, #0xfe
    b .L0802D7BA
.L0802D7B8:
    movs r5, #2
.L0802D7BA:
    ldr r0, [r6, #0x34]
    ldr r2, .L0802D7D8 @ =0x00002210
    adds r0, r0, r2
    ldrb r0, [r0]
    lsls r0, r0, #0x1f
    cmp r0, #0
    bne .L0802D7DC
    ldrb r1, [r1, #1]
    movs r2, #1
    adds r0, r2, #0
    ands r0, r1
    cmp r0, #0
    beq .L0802D7F6
    b .L0802D7E8
    .align 2, 0
.L0802D7D8: .4byte 0x00002210
.L0802D7DC:
    ldrb r1, [r1, #1]
    movs r2, #1
    adds r0, r2, #0
    ands r0, r1
    cmp r0, #0
    bne .L0802D7F6
.L0802D7E8:
    movs r0, #2
    strb r0, [r7]
    lsls r0, r3, #0x19
    lsrs r3, r0, #0x18
    lsls r0, r5, #0x19
    lsrs r5, r0, #0x18
    b .L0802D7F8
.L0802D7F6:
    strb r2, [r7]
.L0802D7F8:
    lsls r0, r3, #0x18
    asrs r0, r0, #8
    str r0, [r6, #0x18]
    lsls r0, r5, #0x18
    asrs r0, r0, #8
    str r0, [r6, #0x1c]
    mov r1, sb
    ldrb r0, [r1]
    cmp r0, r4
    beq .L0802D814
    adds r0, r6, #0
    adds r1, r4, #0
    bl SetAnimFacing__12AActorEntityUi
.L0802D814:
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    adds r1, r0, #0
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, r4
    beq .L0802D832
    adds r0, r1, #0
    adds r1, r4, #0
    bl SetAnimFacing__12AActorEntityUi
.L0802D832:
    add sp, #0x20
    pop {r3, r4, r5}
    mov r8, r3
    mov sb, r4
    mov sl, r5
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0
    .align 2, 0

    thumb_func_start func_0802D844
func_0802D844: @ 0x0802D844
    push {r4, r5, lr}
    adds r4, r0, #0
    movs r5, #0
    cmp r1, #0
    beq .L0802D850
    ldrh r5, [r1, #4]
.L0802D850:
    adds r0, r4, #0
    adds r0, #0xc3
    ldrb r0, [r0]
    cmp r0, #0
    bne .L0802D862
    ldr r0, .L0802D88C @ =0x000003F3
    ands r0, r5
    cmp r0, #0
    beq .L0802D8E2
.L0802D862:
    adds r0, r4, #0
    bl func_080323C8
    adds r1, r4, #0
    adds r1, #0x3c
    movs r0, #0x18
    strb r0, [r1]
    movs r0, #0x20
    ands r0, r5
    cmp r0, #0
    beq .L0802D890
    adds r0, r4, #0
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, #2
    beq .L0802D8E2
    adds r0, r4, #0
    movs r1, #2
    bl SetAnimFacing__12AActorEntityUi
    b .L0802D8E2
    .align 2, 0
.L0802D88C: .4byte 0x000003F3
.L0802D890:
    movs r0, #0x10
    ands r0, r5
    cmp r0, #0
    beq .L0802D8AC
    adds r0, r4, #0
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, #3
    beq .L0802D8E2
    adds r0, r4, #0
    movs r1, #3
    bl SetAnimFacing__12AActorEntityUi
    b .L0802D8E2
.L0802D8AC:
    movs r0, #0x40
    ands r0, r5
    cmp r0, #0
    beq .L0802D8C8
    adds r0, r4, #0
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, #1
    beq .L0802D8E2
    adds r0, r4, #0
    movs r1, #1
    bl SetAnimFacing__12AActorEntityUi
    b .L0802D8E2
.L0802D8C8:
    movs r0, #0x80
    ands r5, r0
    cmp r5, #0
    beq .L0802D8E2
    adds r0, r4, #0
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, #0
    beq .L0802D8E2
    adds r0, r4, #0
    movs r1, #0
    bl SetAnimFacing__12AActorEntityUi
.L0802D8E2:
    pop {r4, r5}
    pop {r0}
    bx r0

    thumb_func_start func_0802D8E8
func_0802D8E8: @ 0x0802D8E8
    push {r4, r5, lr}
    sub sp, #0xc
    adds r5, r0, #0
    movs r3, #0
    str r3, [sp, #8]
    movs r4, #0
    cmp r1, #0
    beq .L0802D900
    ldrh r3, [r1]
    ldrh r0, [r1, #4]
    str r0, [sp, #8]
    ldrh r4, [r1, #8]
.L0802D900:
    adds r0, r5, #0
    adds r0, #0xc3
    ldrb r0, [r0]
    cmp r0, #0
    beq .L0802D914
    adds r1, r5, #0
    adds r1, #0x3c
    movs r0, #0
    strb r0, [r1]
    b .L0802D9A4
.L0802D914:
    lsls r0, r3, #0x1e
    lsrs r2, r0, #0x1f
    lsls r0, r3, #0x16
    lsrs r0, r0, #0x1f
    orrs r2, r0
    cmp r2, #0
    beq .L0802D99E
    lsls r1, r4, #0x19
    lsrs r1, r1, #0x1f
    lsls r0, r4, #0x1a
    lsrs r0, r0, #0x1f
    orrs r1, r0
    cmp r1, #0
    beq .L0802D93A
    adds r0, r5, #0
    movs r1, #1
    bl func_08025A40
    b .L0802D9A4
.L0802D93A:
    lsls r2, r4, #0x18
    lsrs r2, r2, #0x1f
    lsls r0, r4, #0x1b
    lsrs r0, r0, #0x1f
    orrs r2, r0
    ldr r1, [sp, #8]
    lsls r0, r1, #0x1e
    lsrs r0, r0, #0x1f
    orrs r0, r2
    lsls r1, r1, #0x16
    lsrs r4, r1, #0x1f
    orrs r4, r0
    cmp r4, #0
    beq .L0802D960
    adds r0, r5, #0
    movs r1, #0
    bl func_08025A40
    b .L0802D9A4
.L0802D960:
    add r1, sp, #8
    ldrb r0, [r1]
    movs r3, #1
    adds r2, r3, #0
    ands r2, r0
    cmp r2, #0
    beq .L0802D994
    ldr r0, [r5]
    ldr r2, [r0]
    movs r1, #0xa0
    lsls r1, r1, #1
    adds r2, r2, r1
    ldr r1, .L0802D990 @ =0x000003CE
    str r1, [sp]
    str r4, [sp, #4]
    ldr r3, [r2]
    mov r1, sp
    movs r2, #0
    bl _call_via_r3
    adds r0, r5, #0
    adds r0, #0x3c
    strb r4, [r0]
    b .L0802D9A4
    .align 2, 0
.L0802D990: .4byte 0x000003CE
.L0802D994:
    ldrb r1, [r1, #1]
    adds r0, r3, #0
    ands r0, r1
    cmp r0, #0
    beq .L0802D9A4
.L0802D99E:
    adds r0, r5, #0
    adds r0, #0x3c
    strb r2, [r0]
.L0802D9A4:
    add sp, #0xc
    pop {r4, r5}
    pop {r0}
    bx r0

    thumb_func_start func_0802D9AC
func_0802D9AC: @ 0x0802D9AC
    push {r4, r5, r6, lr}
    sub sp, #0x1c
    adds r6, r0, #0
    movs r2, #0
    str r2, [sp, #0x14]
    str r2, [sp, #0x18]
    movs r3, #0
    cmp r1, #0
    beq .L0802D9C8
    ldrh r0, [r1]
    str r0, [sp, #0x14]
    ldrh r0, [r1, #4]
    str r0, [sp, #0x18]
    ldrh r3, [r1, #8]
.L0802D9C8:
    adds r0, r6, #0
    adds r0, #0xc3
    ldrb r0, [r0]
    cmp r0, #0
    beq .L0802D9DA
    adds r0, r6, #0
    adds r0, #0x3c
    strb r2, [r0]
    b .L0802DBE6
.L0802D9DA:
    ldr r0, [sp, #0x14]
    lsls r1, r0, #0x1f
    lsrs r1, r1, #0x1f
    lsls r0, r0, #0x16
    lsrs r0, r0, #0x1f
    orrs r1, r0
    cmp r1, #0
    bne .L0802D9EC
    b .L0802DBE0
.L0802D9EC:
    lsls r1, r3, #0x19
    lsrs r1, r1, #0x1f
    lsls r0, r3, #0x1a
    lsrs r0, r0, #0x1f
    orrs r1, r0
    cmp r1, #0
    beq .L0802DA1C
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802DA12
    adds r0, r6, #0
    movs r1, #1
    bl func_080257D0
    b .L0802DBE6
.L0802DA12:
    adds r0, r6, #0
    movs r1, #1
    bl func_08025838
    b .L0802DBE6
.L0802DA1C:
    lsls r2, r3, #0x18
    lsrs r2, r2, #0x1f
    lsls r0, r3, #0x1b
    lsrs r0, r0, #0x1f
    orrs r2, r0
    ldr r1, [sp, #0x18]
    lsls r0, r1, #0x1f
    lsrs r0, r0, #0x1f
    orrs r0, r2
    lsls r1, r1, #0x16
    lsrs r4, r1, #0x1f
    orrs r4, r0
    cmp r4, #0
    beq .L0802DA5A
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802DA50
    adds r0, r6, #0
    movs r1, #0
    bl func_080257D0
    b .L0802DBE6
.L0802DA50:
    adds r0, r6, #0
    movs r1, #0
    bl func_08025838
    b .L0802DBE6
.L0802DA5A:
    add r2, sp, #0x18
    ldrb r1, [r2]
    movs r5, #2
    adds r0, r5, #0
    ands r0, r1
    lsls r0, r0, #0x18
    lsrs r3, r0, #0x18
    cmp r3, #0
    bne .L0802DA6E
    b .L0802DBCE
.L0802DA6E:
    add r3, sp, #0x14
    ldrh r2, [r3]
    ldr r1, .L0802DA8C @ =0x00000201
    adds r0, r1, #0
    ands r0, r2
    cmp r0, r1
    bne .L0802DA94
    ldr r0, [r6]
    ldr r3, [r0]
    subs r1, #0xc1
    adds r3, r3, r1
    add r1, sp, #4
    ldr r2, .L0802DA90 @ =0x000003CE
    str r2, [sp, #4]
    b .L0802DAAE
    .align 2, 0
.L0802DA8C: .4byte 0x00000201
.L0802DA90: .4byte 0x000003CE
.L0802DA94:
    ldrb r1, [r3]
    movs r0, #1
    ands r0, r1
    cmp r0, #0
    beq .L0802DAC8
    ldr r0, [r6]
    ldr r3, [r0]
    movs r2, #0xa0
    lsls r2, r2, #1
    adds r3, r3, r2
    add r1, sp, #0xc
    ldr r2, .L0802DAC0 @ =0x000003CE
    str r2, [sp, #0xc]
.L0802DAAE:
    str r4, [r1, #4]
    ldr r3, [r3]
    movs r2, #0
    bl _call_via_r3
    adds r0, r6, #0
    adds r0, #0x3c
    strb r4, [r0]
    b .L0802DBE6
    .align 2, 0
.L0802DAC0: .4byte 0x000003CE
.L0802DAC4:
    ldr r0, [r4]
    b .L0802DB62
.L0802DAC8:
    ldrb r1, [r3, #1]
    adds r0, r5, #0
    ands r0, r1
    cmp r0, #0
    bne .L0802DAD4
    b .L0802DBE6
.L0802DAD4:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802DBC4
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F388
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802DBBA
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F204
    cmp r0, #0
    bne .L0802DBBA
    ldr r1, [r6, #0x38]
    adds r1, #0x54
    mov r0, sp
    bl func_0800F20C
    ldr r5, [r6, #0x14]
    mov r0, sp
    bl GetStaminaGain__C4Food
    adds r4, r0, #0
    mov r0, sp
    bl GetFatigueGain__C4Food
    adds r2, r0, #0
    ldr r3, [r5, #0x64]
    adds r0, r6, #0
    adds r1, r4, #0
    bl _call_via_r3
    mov r0, sp
    bl IsDrink__C4Food
    cmp r0, #0
    bne .L0802DB6E
    adds r1, r6, #0
    adds r1, #0x3c
    movs r0, #0xf
    strb r0, [r1]
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
    beq .L0802DB5E
.L0802DB4C:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802DAC4
    adds r4, #4
    cmp r4, r5
    bne .L0802DB4C
.L0802DB5E:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802DB62:
    movs r1, #0x65
    bl func_08008B6C
    b .L0802DBAA
.L0802DB6A:
    ldr r0, [r4]
    b .L0802DBA4
.L0802DB6E:
    adds r1, r6, #0
    adds r1, #0x3c
    movs r0, #0x10
    strb r0, [r1]
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
    beq .L0802DBA0
.L0802DB8E:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802DB6A
    adds r4, #4
    cmp r4, r5
    bne .L0802DB8E
.L0802DBA0:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802DBA4:
    movs r1, #0x66
    bl func_08008B6C
.L0802DBAA:
    adds r0, r6, #0
    bl CancelFarmerItemDisplay
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F390
    b .L0802DBE6
.L0802DBBA:
    adds r1, r6, #0
    adds r1, #0x3c
    movs r0, #0
    strb r0, [r1]
    b .L0802DBE6
.L0802DBC4:
    adds r0, r6, #0
    movs r1, #0
    bl func_08025A40
    b .L0802DBE6
.L0802DBCE:
    ldrb r1, [r2, #1]
    movs r0, #1
    ands r0, r1
    cmp r0, #0
    beq .L0802DBE6
    adds r0, r6, #0
    adds r0, #0x3c
    strb r3, [r0]
    b .L0802DBE6
.L0802DBE0:
    adds r0, r6, #0
    adds r0, #0x3c
    strb r1, [r0]
.L0802DBE6:
    add sp, #0x1c
    pop {r4, r5, r6}
    pop {r0}
    bx r0
    .align 2, 0

    thumb_func_start func_0802DBF0
func_0802DBF0: @ 0x0802DBF0
    push {r4, r5, r6, lr}
    sub sp, #0xc
    adds r6, r0, #0
    movs r2, #0
    str r2, [sp, #4]
    str r2, [sp, #8]
    cmp r1, #0
    beq .L0802DC08
    ldrh r0, [r1]
    str r0, [sp, #4]
    ldrh r0, [r1, #4]
    str r0, [sp, #8]
.L0802DC08:
    adds r0, r6, #0
    adds r0, #0xc3
    ldrb r0, [r0]
    cmp r0, #0
    beq .L0802DC14
    b .L0802DE26
.L0802DC14:
    add r0, sp, #4
    ldrb r1, [r0, #1]
    movs r5, #2
    adds r0, r5, #0
    ands r0, r1
    cmp r0, #0
    bne .L0802DC24
    b .L0802DE32
.L0802DC24:
    ldr r2, [sp, #8]
    lsls r1, r2, #0x19
    lsrs r1, r1, #0x1f
    lsls r0, r2, #0x1a
    lsrs r0, r0, #0x1f
    orrs r1, r0
    cmp r1, #0
    beq .L0802DC84
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    lsrs r1, r0, #0x18
    cmp r1, #0
    bne .L0802DC46
    b .L0802DE86
.L0802DC46:
    adds r1, r6, #0
    adds r1, #0x3c
    movs r0, #0x16
    strb r0, [r1]
    adds r1, #0x3d
    movs r0, #1
    strb r0, [r1]
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
    bne .L0802DC6E
    b .L0802DE7A
.L0802DC6E:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802DC7C
    b .L0802DE2E
.L0802DC7C:
    adds r4, #4
    cmp r4, r5
    bne .L0802DC6E
    b .L0802DE7A
.L0802DC84:
    lsls r1, r2, #0x18
    lsrs r1, r1, #0x1f
    lsls r0, r2, #0x1b
    lsrs r0, r0, #0x1f
    orrs r1, r0
    cmp r1, #0
    beq .L0802DCE2
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    lsrs r1, r0, #0x18
    cmp r1, #0
    bne .L0802DCA4
    b .L0802DE86
.L0802DCA4:
    adds r1, r6, #0
    adds r1, #0x3c
    movs r0, #0x16
    strb r0, [r1]
    adds r0, r6, #0
    adds r0, #0x79
    strb r5, [r0]
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
    bne .L0802DCCC
    b .L0802DE7A
.L0802DCCC:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802DCDA
    b .L0802DE2E
.L0802DCDA:
    adds r4, #4
    cmp r4, r5
    bne .L0802DCCC
    b .L0802DE7A
.L0802DCE2:
    add r3, sp, #8
    ldrb r1, [r3]
    movs r4, #1
    adds r0, r4, #0
    ands r0, r1
    cmp r0, #0
    beq .L0802DD12
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802DD08
    adds r0, r6, #0
    movs r1, #1
    bl func_08025838
    b .L0802DE8C
.L0802DD08:
    adds r0, r6, #0
    movs r1, #1
    bl func_080257D0
    b .L0802DE8C
.L0802DD12:
    adds r0, r5, #0
    ands r0, r1
    lsls r0, r0, #0x18
    lsrs r2, r0, #0x18
    cmp r2, #0
    beq .L0802DE1C
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802DD3A
    adds r0, r6, #0
    movs r1, #0
    bl func_08025A40
    b .L0802DE8C
.L0802DD36:
    ldr r0, [r4]
    b .L0802DDBA
.L0802DD3A:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F388
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802DE12
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F204
    cmp r0, #0
    bne .L0802DE12
    ldr r1, [r6, #0x38]
    adds r1, #0x54
    mov r0, sp
    bl func_0800F20C
    ldr r5, [r6, #0x14]
    mov r0, sp
    bl GetStaminaGain__C4Food
    adds r4, r0, #0
    mov r0, sp
    bl GetFatigueGain__C4Food
    adds r2, r0, #0
    ldr r3, [r5, #0x64]
    adds r0, r6, #0
    adds r1, r4, #0
    bl _call_via_r3
    mov r0, sp
    bl IsDrink__C4Food
    cmp r0, #0
    bne .L0802DDC6
    adds r1, r6, #0
    adds r1, #0x3c
    movs r0, #0xf
    strb r0, [r1]
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
    beq .L0802DDB6
.L0802DDA4:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802DD36
    adds r4, #4
    cmp r4, r5
    bne .L0802DDA4
.L0802DDB6:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802DDBA:
    movs r1, #0x65
    bl func_08008B6C
    b .L0802DE02
.L0802DDC2:
    ldr r0, [r4]
    b .L0802DDFC
.L0802DDC6:
    adds r1, r6, #0
    adds r1, #0x3c
    movs r0, #0x10
    strb r0, [r1]
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
    beq .L0802DDF8
.L0802DDE6:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802DDC2
    adds r4, #4
    cmp r4, r5
    bne .L0802DDE6
.L0802DDF8:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802DDFC:
    movs r1, #0x66
    bl func_08008B6C
.L0802DE02:
    adds r0, r6, #0
    bl CancelFarmerItemDisplay
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F390
    b .L0802DE8C
.L0802DE12:
    adds r1, r6, #0
    adds r1, #0x3c
    movs r0, #0
    strb r0, [r1]
    b .L0802DE8C
.L0802DE1C:
    ldrb r1, [r3, #1]
    adds r0, r4, #0
    ands r0, r1
    cmp r0, #0
    beq .L0802DE8C
.L0802DE26:
    adds r0, r6, #0
    adds r0, #0x3c
    strb r2, [r0]
    b .L0802DE8C
.L0802DE2E:
    ldr r0, [r4]
    b .L0802DE7E
.L0802DE32:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    lsrs r1, r0, #0x18
    cmp r1, #0
    beq .L0802DE86
    adds r1, r6, #0
    adds r1, #0x3c
    movs r0, #0x16
    strb r0, [r1]
    adds r1, #0x3d
    movs r0, #3
    strb r0, [r1]
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
    beq .L0802DE7A
.L0802DE68:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802DE2E
    adds r4, #4
    cmp r4, r5
    bne .L0802DE68
.L0802DE7A:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802DE7E:
    movs r1, #0x67
    bl func_08008B6C
    b .L0802DE8C
.L0802DE86:
    adds r0, r6, #0
    adds r0, #0x3c
    strb r1, [r0]
.L0802DE8C:
    add sp, #0xc
    pop {r4, r5, r6}
    pop {r0}
    bx r0

    thumb_func_start func_0802DE94
func_0802DE94: @ 0x0802DE94
    push {r4, r5, r6, lr}
    sub sp, #0x10
    adds r6, r0, #0
    movs r2, #0
    str r2, [sp, #4]
    cmp r1, #0
    beq .L0802DEA8
    ldrh r2, [r1]
    ldrh r0, [r1, #4]
    str r0, [sp, #4]
.L0802DEA8:
    adds r0, r6, #0
    adds r0, #0xc3
    ldrb r0, [r0]
    cmp r0, #0
    beq .L0802DEB4
    b .L0802E0AE
.L0802DEB4:
    movs r5, #2
    ands r2, r5
    lsls r0, r2, #0x18
    lsrs r4, r0, #0x18
    cmp r4, #0
    bne .L0802DEC2
    b .L0802E0B8
.L0802DEC2:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802DED6
    adds r0, r6, #0
    bl func_0802C908
.L0802DED6:
    ldr r2, [sp, #4]
    lsls r1, r2, #0x19
    lsrs r1, r1, #0x1f
    lsls r0, r2, #0x18
    lsrs r0, r0, #0x1f
    orrs r1, r0
    lsls r0, r2, #0x1a
    lsrs r0, r0, #0x1f
    orrs r0, r1
    lsls r2, r2, #0x1b
    lsrs r2, r2, #0x1f
    orrs r2, r0
    cmp r2, #0
    beq .L0802DF70
    add r0, sp, #4
    ldrb r1, [r0]
    movs r0, #0x20
    ands r0, r1
    cmp r0, #0
    beq .L0802DF14
    adds r0, r6, #0
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, #2
    bne .L0802DF0A
    b .L0802E0F2
.L0802DF0A:
    adds r0, r6, #0
    movs r1, #2
    bl SetAnimFacing__12AActorEntityUi
    b .L0802E0F2
.L0802DF14:
    movs r0, #0x10
    ands r0, r1
    cmp r0, #0
    beq .L0802DF32
    adds r0, r6, #0
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, #3
    bne .L0802DF28
    b .L0802E0F2
.L0802DF28:
    adds r0, r6, #0
    movs r1, #3
    bl SetAnimFacing__12AActorEntityUi
    b .L0802E0F2
.L0802DF32:
    movs r0, #0x40
    ands r0, r1
    cmp r0, #0
    beq .L0802DF50
    adds r0, r6, #0
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, #1
    bne .L0802DF46
    b .L0802E0F2
.L0802DF46:
    adds r0, r6, #0
    movs r1, #1
    bl SetAnimFacing__12AActorEntityUi
    b .L0802E0F2
.L0802DF50:
    movs r0, #0x80
    ands r0, r1
    cmp r0, #0
    bne .L0802DF5A
    b .L0802E0F2
.L0802DF5A:
    adds r0, r6, #0
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, #0
    bne .L0802DF66
    b .L0802E0F2
.L0802DF66:
    adds r0, r6, #0
    movs r1, #0
    bl SetAnimFacing__12AActorEntityUi
    b .L0802E0F2
.L0802DF70:
    add r1, sp, #4
    ldrb r3, [r1, #1]
    movs r0, #1
    adds r4, r0, #0
    ands r4, r3
    cmp r4, #0
    beq .L0802DF86
    adds r0, r6, #0
    adds r0, #0x3c
    strb r2, [r0]
    b .L0802E0F2
.L0802DF86:
    ldrb r1, [r1]
    ands r0, r1
    cmp r0, #0
    beq .L0802DFB0
    ldr r0, [r6]
    ldr r3, [r0]
    movs r1, #0xa0
    lsls r1, r1, #1
    adds r3, r3, r1
    add r1, sp, #8
    ldr r2, .L0802DFAC @ =0x000003CE
    str r2, [sp, #8]
    str r4, [r1, #4]
    ldr r3, [r3]
    movs r2, #0
    bl _call_via_r3
    b .L0802E0D8
    .align 2, 0
.L0802DFAC: .4byte 0x000003CE
.L0802DFB0:
    adds r0, r5, #0
    ands r0, r3
    cmp r0, #0
    bne .L0802DFBA
    b .L0802E0F2
.L0802DFBA:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802DFD6
    adds r0, r6, #0
    movs r1, #0
    bl func_08025A40
    b .L0802E0F2
.L0802DFD2:
    ldr r0, [r4]
    b .L0802E056
.L0802DFD6:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F388
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802E0AE
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F204
    cmp r0, #0
    bne .L0802E0AE
    ldr r1, [r6, #0x38]
    adds r1, #0x54
    mov r0, sp
    bl func_0800F20C
    ldr r5, [r6, #0x14]
    mov r0, sp
    bl GetStaminaGain__C4Food
    adds r4, r0, #0
    mov r0, sp
    bl GetFatigueGain__C4Food
    adds r2, r0, #0
    ldr r3, [r5, #0x64]
    adds r0, r6, #0
    adds r1, r4, #0
    bl _call_via_r3
    mov r0, sp
    bl IsDrink__C4Food
    cmp r0, #0
    bne .L0802E062
    adds r1, r6, #0
    adds r1, #0x3c
    movs r0, #0xf
    strb r0, [r1]
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
    beq .L0802E052
.L0802E040:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802DFD2
    adds r4, #4
    cmp r4, r5
    bne .L0802E040
.L0802E052:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802E056:
    movs r1, #0x65
    bl func_08008B6C
    b .L0802E09E
.L0802E05E:
    ldr r0, [r4]
    b .L0802E098
.L0802E062:
    adds r1, r6, #0
    adds r1, #0x3c
    movs r0, #0x10
    strb r0, [r1]
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
    beq .L0802E094
.L0802E082:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802E05E
    adds r4, #4
    cmp r4, r5
    bne .L0802E082
.L0802E094:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802E098:
    movs r1, #0x66
    bl func_08008B6C
.L0802E09E:
    adds r0, r6, #0
    bl CancelFarmerItemDisplay
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F390
    b .L0802E0F2
.L0802E0AE:
    adds r1, r6, #0
    adds r1, #0x3c
    movs r0, #0
    strb r0, [r1]
    b .L0802E0F2
.L0802E0B8:
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802E0CE
    adds r0, r6, #0
    bl func_080258A0
    b .L0802E0F2
.L0802E0CE:
    adds r0, r6, #0
    adds r0, #0x3f
    ldrb r0, [r0]
    cmp r0, #0xf
    bne .L0802E0E0
.L0802E0D8:
    adds r0, r6, #0
    adds r0, #0x3c
    strb r4, [r0]
    b .L0802E0F2
.L0802E0E0:
    cmp r0, #0xe
    bne .L0802E0EC
    adds r0, r6, #0
    bl func_0802C860
    b .L0802E0F2
.L0802E0EC:
    adds r0, r6, #0
    bl func_0802C2E0
.L0802E0F2:
    add sp, #0x10
    pop {r4, r5, r6}
    pop {r0}
    bx r0
    .align 2, 0

    thumb_func_start func_0802E0FC
func_0802E0FC: @ 0x0802E0FC
    push {r4, r5, r6, r7, lr}
    mov r7, sl
    mov r6, sb
    mov r5, r8
    push {r5, r6, r7}
    sub sp, #0x84
    adds r7, r0, #0
    movs r2, #0
    str r2, [sp, #0x28]
    cmp r1, #0
    beq .L0802E118
    ldrh r2, [r1]
    ldrh r0, [r1, #4]
    str r0, [sp, #0x28]
.L0802E118:
    adds r0, r7, #0
    adds r0, #0xc3
    ldrb r4, [r0]
    cmp r4, #0
    beq .L0802E12E
    adds r1, r7, #0
    adds r1, #0x3c
    movs r0, #0
    strb r0, [r1]
    bl .L0802EBE6
.L0802E12E:
    movs r5, #1
    adds r0, r5, #0
    ands r0, r2
    cmp r0, #0
    beq .L0802E1BA
    add r2, sp, #0x28
    ldrb r1, [r2]
    movs r3, #2
    adds r0, r3, #0
    ands r0, r1
    cmp r0, #0
    beq .L0802E170
    ldr r0, [r7]
    ldr r3, [r0]
    movs r1, #0xa0
    lsls r1, r1, #1
    adds r3, r3, r1
    add r1, sp, #0x20
    ldr r2, .L0802E16C @ =0x000003CE
    str r2, [sp, #0x20]
    str r4, [r1, #4]
    ldr r3, [r3]
    movs r2, #0
    bl _call_via_r3
    adds r0, r7, #0
    adds r0, #0x3c
    strb r4, [r0]
    bl .L0802EBE6
    .align 2, 0
.L0802E16C: .4byte 0x000003CE
.L0802E170:
    ldrb r1, [r2, #1]
    adds r0, r3, #0
    ands r0, r1
    lsls r0, r0, #0x18
    lsrs r2, r0, #0x18
    cmp r2, #0
    beq .L0802E1A4
    ldr r0, [r7, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802E198
    adds r0, r7, #0
    movs r1, #1
    bl func_080257D0
    bl .L0802EBE6
.L0802E198:
    adds r0, r7, #0
    movs r1, #0
    bl func_08025838
    bl .L0802EBE6
.L0802E1A4:
    adds r0, r5, #0
    ands r0, r1
    cmp r0, #0
    bne .L0802E1B0
    bl .L0802EBE6
.L0802E1B0:
    adds r0, r7, #0
    adds r0, #0x3c
    strb r2, [r0]
    bl .L0802EBE6
.L0802E1BA:
    adds r5, r7, #0
    adds r5, #0x3c
    strb r0, [r5]
    ldr r2, [r7]
    mov sb, r2
    mov r0, sp
    adds r1, r7, #0
    bl GetLocation__C7AEntity
    mov r3, sp
    str r3, [sp, #0x68]
    adds r4, r7, #0
    adds r4, #0x20
    ldrb r2, [r4]
    add r0, sp, #0x2c
    adds r1, r7, #0
    bl func_0802AB98
    ldr r0, [sp, #0x2c]
    ldr r1, [sp, #0x30]
    str r0, [sp, #0x34]
    str r1, [sp, #0x38]
    add r2, sp, #0x34
    mov r1, sb
    ldr r0, [r1]
    movs r3, #0x9a
    lsls r3, r3, #1
    adds r0, r0, r3
    ldr r3, [r0]
    mov r0, sb
    mov r1, sp
    bl _call_via_r3
    adds r6, r0, #0
    movs r0, #0
    str r0, [sp, #0x6c]
    movs r1, #0
    str r1, [sp, #0x70]
    adds r0, r6, #0
    subs r0, #0x2b
    str r5, [sp, #0x7c]
    str r4, [sp, #0x78]
    cmp r0, #0x39
    bls .L0802E214
    b .L0802E792
.L0802E214:
    lsls r0, r0, #2
    ldr r1, .L0802E220 @ =.L0802E224
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L0802E220: .4byte .L0802E224
.L0802E224: @ jump table
    .4byte .L0802E516 @ case 0
    .4byte .L0802E30C @ case 1
    .4byte .L0802E792 @ case 2
    .4byte .L0802E60C @ case 3
    .4byte .L0802E60C @ case 4
    .4byte .L0802E60C @ case 5
    .4byte .L0802E60C @ case 6
    .4byte .L0802E60C @ case 7
    .4byte .L0802E60C @ case 8
    .4byte .L0802E60C @ case 9
    .4byte .L0802E60C @ case 10
    .4byte .L0802E65A @ case 11
    .4byte .L0802E65A @ case 12
    .4byte .L0802E65A @ case 13
    .4byte .L0802E65A @ case 14
    .4byte .L0802E65A @ case 15
    .4byte .L0802E65A @ case 16
    .4byte .L0802E65A @ case 17
    .4byte .L0802E65A @ case 18
    .4byte .L0802E65A @ case 19
    .4byte .L0802E65A @ case 20
    .4byte .L0802E65A @ case 21
    .4byte .L0802E65A @ case 22
    .4byte .L0802E65A @ case 23
    .4byte .L0802E65A @ case 24
    .4byte .L0802E65A @ case 25
    .4byte .L0802E65A @ case 26
    .4byte .L0802E792 @ case 27
    .4byte .L0802E792 @ case 28
    .4byte .L0802E792 @ case 29
    .4byte .L0802E792 @ case 30
    .4byte .L0802E55C @ case 31
    .4byte .L0802E740 @ case 32
    .4byte .L0802E680 @ case 33
    .4byte .L0802E680 @ case 34
    .4byte .L0802E680 @ case 35
    .4byte .L0802E680 @ case 36
    .4byte .L0802E680 @ case 37
    .4byte .L0802E680 @ case 38
    .4byte .L0802E680 @ case 39
    .4byte .L0802E680 @ case 40
    .4byte .L0802E792 @ case 41
    .4byte .L0802E792 @ case 42
    .4byte .L0802E792 @ case 43
    .4byte .L0802E792 @ case 44
    .4byte .L0802E792 @ case 45
    .4byte .L0802E792 @ case 46
    .4byte .L0802E792 @ case 47
    .4byte .L0802E792 @ case 48
    .4byte .L0802E792 @ case 49
    .4byte .L0802E792 @ case 50
    .4byte .L0802E792 @ case 51
    .4byte .L0802E792 @ case 52
    .4byte .L0802E792 @ case 53
    .4byte .L0802E792 @ case 54
    .4byte .L0802E792 @ case 55
    .4byte .L0802E792 @ case 56
    .4byte .L0802E7BA @ case 57
.L0802E30C:
    ldr r0, [r7, #0x34]
    adds r0, #0x14
    bl GetHorse__4Farm
    bl GetGrowthStage__C5Horse
    cmp r0, #1
    beq .L0802E31E
    b .L0802E510
.L0802E31E:
    ldr r0, [r7, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802E32E
    b .L0802E486
.L0802E32E:
    ldrh r2, [r7, #4]
    add r3, sp, #8
    mov r8, r3
    mov r5, sb
    ldr r1, [r5]
    mov r0, r8
    ldr r3, [r1, #0x34]
    mov r1, sb
    bl _call_via_r3
    movs r0, #0xa
    ldrsh r5, [r7, r0]
    movs r1, #0xe
    ldrsh r4, [r7, r1]
    add r1, sp, #0x3c
    subs r0, r5, #7
    strh r0, [r1]
    adds r0, r4, #0
    subs r0, #9
    strh r0, [r1, #2]
    adds r0, r5, #7
    strh r0, [r1, #4]
    adds r0, r4, #5
    strh r0, [r1, #6]
    ldr r0, [sp, #0x3c]
    ldr r1, [sp, #0x40]
    str r0, [sp, #0x44]
    str r1, [sp, #0x48]
    add r6, sp, #0x44
    mov r0, r8
    adds r1, r6, #0
    bl func_080AC070
    movs r2, #1
    mov sl, r2
    mov r3, sl
    eors r0, r3
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802E3C0
    add r0, sp, #0x4c
    mov r1, r8
    adds r2, r6, #0
    bl func_08024724
    ldr r0, [sp, #0x4c]
    str r0, [sp, #0x50]
    add r1, sp, #0x50
    movs r2, #0
    ldrsh r0, [r1, r2]
    adds r5, r5, r0
    movs r3, #2
    ldrsh r0, [r1, r3]
    adds r4, r4, r0
    add r1, sp, #0x54
    subs r0, r5, #7
    strh r0, [r1]
    adds r0, r4, #0
    subs r0, #9
    strh r0, [r1, #2]
    adds r0, r5, #7
    strh r0, [r1, #4]
    adds r0, r4, #5
    strh r0, [r1, #6]
    mov r0, r8
    bl func_080AC070
    mov r1, sl
    eors r0, r1
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802E3C0
    b .L0802E7BA
.L0802E3C0:
    lsls r6, r5, #0x10
    lsls r4, r4, #0x10
    mov r8, r4
    mov r2, sb
    ldr r0, [r2]
    ldr r2, [r0, #0x40]
    mov r0, sb
    movs r1, #0x2c
    bl _call_via_r2
    adds r4, r0, #0
    bl func_08021EE0
    str r6, [r4, #8]
    mov r3, r8
    str r3, [r4, #0xc]
    movs r5, #0
    str r5, [r4, #0x18]
    str r5, [r4, #0x1c]
    ldr r1, [sp, #0x78]
    ldrb r0, [r1]
    cmp r0, #1
    beq .L0802E402
    cmp r0, #1
    bgt .L0802E3F8
    cmp r0, #0
    beq .L0802E416
    b .L0802E450
.L0802E3F8:
    cmp r0, #2
    beq .L0802E42A
    cmp r0, #3
    beq .L0802E43E
    b .L0802E450
.L0802E402:
    adds r0, r4, #0
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, #3
    beq .L0802E450
    adds r0, r4, #0
    movs r1, #3
    bl SetAnimFacing__12AActorEntityUi
    b .L0802E450
.L0802E416:
    adds r0, r4, #0
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, #2
    beq .L0802E450
    adds r0, r4, #0
    movs r1, #2
    bl SetAnimFacing__12AActorEntityUi
    b .L0802E450
.L0802E42A:
    adds r0, r4, #0
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, #1
    beq .L0802E450
    adds r0, r4, #0
    movs r1, #1
    bl SetAnimFacing__12AActorEntityUi
    b .L0802E450
.L0802E43E:
    adds r0, r4, #0
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, #0
    beq .L0802E450
    adds r0, r4, #0
    movs r1, #0
    bl SetAnimFacing__12AActorEntityUi
.L0802E450:
    ldr r0, [r7, #8]
    adds r5, r7, #0
    adds r5, #0xb8
    subs r0, r6, r0
    movs r1, #0x18
    bl __divsi3
    str r0, [r5]
    ldr r0, [r7, #0xc]
    adds r4, r7, #0
    adds r4, #0xbc
    mov r2, r8
    subs r0, r2, r0
    movs r1, #0x18
    bl __divsi3
    str r0, [r4]
    ldr r0, [r5]
    str r0, [r7, #0x18]
    ldr r0, [r4]
    str r0, [r7, #0x1c]
    movs r3, #1
    str r3, [sp, #0x70]
    movs r0, #0x22
    ldr r5, [sp, #0x7c]
    strb r0, [r5]
    b .L0802E7BA
.L0802E486:
    adds r0, r7, #0
    bl func_080262C4
    add r4, sp, #0x5c
    strb r0, [r4]
    adds r0, r4, #0
    bl GetPrice__C7Product
    cmp r0, #0
    bne .L0802E49C
    b .L0802E7BA
.L0802E49C:
    ldr r5, [r7]
    ldr r0, [r5]
    movs r1, #0xae
    lsls r1, r1, #1
    adds r0, r0, r1
    ldr r2, [r0]
    adds r0, r5, #0
    adds r1, r4, #0
    bl _call_via_r2
    movs r0, #0xe
    ldr r2, [sp, #0x7c]
    strb r0, [r2]
    movs r3, #1
    str r3, [sp, #0x70]
    ldr r0, [r5]
    ldr r2, [r0, #0x40]
    adds r0, r5, #0
    movs r1, #0x2c
    bl _call_via_r2
    adds r4, r0, #0
    cmp r4, #0
    beq .L0802E4E4
    ldr r0, [r4, #0x14]
    adds r0, #0x88
    ldr r2, [r0]
    adds r0, r4, #0
    movs r1, #0x3c
    bl _call_via_r2
    adds r0, r4, #0
    movs r1, #5
    movs r2, #0
    bl func_08032384
.L0802E4E4:
    ldr r0, [r5]
    movs r1, #0xa6
    lsls r1, r1, #1
    adds r0, r0, r1
    ldr r1, [r0]
    adds r0, r5, #0
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    cmp r4, r5
    beq .L0802E5F6
.L0802E4FC:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802E57E
    adds r4, #4
    cmp r4, r5
    bne .L0802E4FC
    b .L0802E5F6
.L0802E510:
    movs r2, #9
    str r2, [sp, #0x6c]
    b .L0802E7BA
.L0802E516:
    ldr r0, [r7, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802E526
    b .L0802E7BA
.L0802E526:
    ldr r4, [r7]
    ldr r0, [r4]
    ldr r2, [r0, #0x40]
    adds r0, r4, #0
    adds r1, r6, #0
    bl _call_via_r2
    adds r1, r0, #0
    cmp r1, #0
    beq .L0802E548
    ldr r0, [r1, #0x14]
    adds r0, #0x88
    ldr r2, [r0]
    adds r0, r1, #0
    movs r1, #0x3c
    bl _call_via_r2
.L0802E548:
    ldr r0, [r4]
    ldr r2, [r0, #0x3c]
    adds r0, r4, #0
    adds r1, r6, #0
    bl _call_via_r2
    adds r0, r7, #0
    bl func_08025E38
    b .L0802EBE6
.L0802E55C:
    ldr r0, [r7, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802E582
    ldr r0, [r7]
    ldr r1, [r0]
    ldr r2, [r1, #0x3c]
    adds r1, r6, #0
    bl _call_via_r2
    adds r0, r7, #0
    bl func_08025FC4
    b .L0802EBE6
.L0802E57E:
    ldr r0, [r4]
    b .L0802E5FA
.L0802E582:
    adds r0, r7, #0
    bl func_080262C4
    mov r4, sp
    adds r4, #0x5d
    strb r0, [r4]
    adds r0, r4, #0
    bl GetPrice__C7Product
    cmp r0, #0
    bne .L0802E59A
    b .L0802E7BA
.L0802E59A:
    ldr r0, [r7]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x4a
    bl _call_via_r2
    adds r5, r0, #0
    ldr r0, [r5, #0x18]
    bl func_0800FFD0
    movs r1, #0
    cmp r0, #0x1e
    bne .L0802E5B6
    movs r1, #1
.L0802E5B6:
    cmp r1, #0
    bne .L0802E602
    adds r0, r5, #0
    adds r1, r4, #0
    bl func_08037EB4
    movs r0, #0xe
    ldr r3, [sp, #0x7c]
    strb r0, [r3]
    movs r5, #1
    str r5, [sp, #0x70]
    ldr r0, [r7]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    cmp r4, r5
    beq .L0802E5F6
.L0802E5E4:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802E57E
    adds r4, #4
    cmp r4, r5
    bne .L0802E5E4
.L0802E5F6:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802E5FA:
    movs r1, #0x6e
    bl func_08008B6C
    b .L0802E7BA
.L0802E602:
    ldr r3, .L0802E608 @ =0x000003CD
    str r3, [sp, #0x6c]
    b .L0802E7BA
    .align 2, 0
.L0802E608: .4byte 0x000003CD
.L0802E60C:
    ldr r0, [r7, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802E61C
    b .L0802E7BA
.L0802E61C:
    ldr r4, [r7]
    ldr r0, [r4]
    ldr r2, [r0, #0x40]
    adds r0, r4, #0
    adds r1, r6, #0
    bl _call_via_r2
    adds r1, r0, #0
    cmp r1, #0
    beq .L0802E63E
    ldr r0, [r1, #0x14]
    adds r0, #0x88
    ldr r2, [r0]
    adds r0, r1, #0
    movs r1, #0x3c
    bl _call_via_r2
.L0802E63E:
    ldr r0, [r4]
    ldr r2, [r0, #0x3c]
    adds r0, r4, #0
    adds r1, r6, #0
    bl _call_via_r2
    adds r1, r6, #0
    subs r1, #0x2e
    lsls r1, r1, #0x18
    lsrs r1, r1, #0x18
    adds r0, r7, #0
    bl func_08025EFC
    b .L0802EBE6
.L0802E65A:
    adds r1, r6, #0
    subs r1, #0x36
    adds r0, r7, #0
    adds r0, #0xb1
    strb r1, [r0]
    ldr r0, [r7, #0x34]
    movs r5, #0xbe
    lsls r5, r5, #3
    adds r0, r0, r5
    bl GetCow__4BarnUi
    movs r1, #0xd
    str r1, [sp, #0x6c]
    cmp r0, #0
    bne .L0802E67A
    b .L0802E7BA
.L0802E67A:
    movs r2, #8
    str r2, [sp, #0x6c]
    b .L0802E7BA
.L0802E680:
    ldr r0, [r7, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802E690
    b .L0802E7BA
.L0802E690:
    adds r4, r6, #0
    subs r4, #0x4c
    ldr r0, [r7, #0x34]
    movs r3, #0x82
    lsls r3, r3, #3
    adds r0, r0, r3
    adds r1, r4, #0
    bl GetEgg__C4CoopUi
    cmp r0, #0
    bne .L0802E6A8
    b .L0802E7BA
.L0802E6A8:
    ldrb r0, [r0, #6]
    lsls r0, r0, #0x1d
    lsrs r0, r0, #0x1d
    movs r5, #0xf
    mov r8, r5
    cmp r0, #5
    bhi .L0802E6FC
    lsls r0, r0, #2
    ldr r1, .L0802E6C0 @ =.L0802E6C4
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L0802E6C0: .4byte .L0802E6C4
.L0802E6C4: @ jump table
    .4byte .L0802E6DC @ case 0
    .4byte .L0802E6E0 @ case 1
    .4byte .L0802E6E6 @ case 2
    .4byte .L0802E6EC @ case 3
    .4byte .L0802E6F2 @ case 4
    .4byte .L0802E6F8 @ case 5
.L0802E6DC:
    movs r0, #0xf
    b .L0802E6FA
.L0802E6E0:
    movs r1, #0x10
    mov r8, r1
    b .L0802E6FC
.L0802E6E6:
    movs r2, #0x11
    mov r8, r2
    b .L0802E6FC
.L0802E6EC:
    movs r3, #0x12
    mov r8, r3
    b .L0802E6FC
.L0802E6F2:
    movs r5, #0x13
    mov r8, r5
    b .L0802E6FC
.L0802E6F8:
    movs r0, #0x14
.L0802E6FA:
    mov r8, r0
.L0802E6FC:
    ldr r0, [r7]
    ldr r1, [r0]
    ldr r2, [r1, #0x3c]
    adds r1, r6, #0
    bl _call_via_r2
    ldr r0, [r7, #0x34]
    movs r1, #0x82
    lsls r1, r1, #3
    adds r0, r0, r1
    adds r1, r4, #0
    bl method_0800C9B8__4CoopUi
    add r5, sp, #8
    add r4, sp, #0xc
    adds r0, r4, #0
    mov r1, r8
    bl __4FoodUi
    ldrb r1, [r4]
    mov r0, sp
    adds r0, #0xd
    ldrb r0, [r0]
    lsls r0, r0, #8
    orrs r0, r1
    mov r1, sp
    adds r1, #0xe
    ldrb r1, [r1]
    lsls r1, r1, #0x10
    orrs r1, r0
    adds r0, r5, #0
    bl __12RucksackItemG4Food
    b .L0802E786
.L0802E740:
    ldr r0, [r7, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802E7BA
    ldr r0, [r7]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x4b
    bl _call_via_r2
    bl func_08038108
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802E7BA
    ldr r0, [r7]
    ldr r1, [r0]
    ldr r2, [r1, #0x3c]
    adds r1, r6, #0
    bl _call_via_r2
    add r5, sp, #8
    mov r4, sp
    adds r4, #0x5e
    adds r0, r4, #0
    movs r1, #0x35
    bl __7ArticleUi
    ldrb r1, [r4]
    adds r0, r5, #0
    bl __12RucksackItemG7Article
.L0802E786:
    adds r0, r7, #0
    adds r1, r5, #0
    movs r2, #0
    bl func_08026150
    b .L0802EBE6
.L0802E792:
    ldr r0, [r7]
    ldr r1, [r0]
    movs r2, #0x96
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r2, [r1]
    adds r1, r6, #0
    bl _call_via_r2
    adds r2, r0, #0
    cmp r2, #0
    beq .L0802E7BA
    ldr r0, [r2, #0x14]
    ldr r1, [r0, #0x34]
    adds r0, r2, #0
    bl _call_via_r1
    lsls r0, r0, #0x10
    lsrs r0, r0, #0x10
    str r0, [sp, #0x6c]
.L0802E7BA:
    ldr r3, [sp, #0x70]
    cmp r3, #0
    beq .L0802E7C2
    b .L0802EBE6
.L0802E7C2:
    ldr r5, [sp, #0x6c]
    cmp r5, #0
    beq .L0802E7CA
    b .L0802E906
.L0802E7CA:
    add r4, sp, #8
    adds r0, r4, #0
    ldr r1, [sp, #0x68]
    movs r2, #6
    bl memcpy
    ldr r1, [sp, #0x78]
    ldrb r0, [r1]
    adds r1, r0, #0
    cmp r0, #1
    beq .L0802E7F4
    cmp r0, #1
    bgt .L0802E7EA
    cmp r0, #0
    beq .L0802E83C
    b .L0802E89A
.L0802E7EA:
    cmp r1, #2
    beq .L0802E87C
    cmp r1, #3
    beq .L0802E886
    b .L0802E89A
.L0802E7F4:
    mov r2, sp
    ldrb r3, [r2, #0xb]
    lsrs r0, r3, #2
    ldrh r1, [r2, #0xc]
    ldr r4, .L0802E830 @ =0x000003FF
    ands r1, r4
    lsls r1, r1, #6
    orrs r1, r0
    lsls r1, r1, #0x10
    ldr r5, .L0802E834 @ =0xFFF00000
    adds r1, r1, r5
    lsrs r2, r1, #0x10
    movs r0, #0x3f
    ands r2, r0
    lsls r2, r2, #2
    movs r0, #3
    ands r0, r3
    orrs r0, r2
    mov r2, sp
    strb r0, [r2, #0xb]
    lsrs r1, r1, #0x16
    ands r1, r4
    ldrh r2, [r2, #0xc]
    ldr r0, .L0802E838 @ =0xFFFFFC00
    ands r0, r2
    orrs r0, r1
    mov r3, sp
    strh r0, [r3, #0xc]
    b .L0802E89A
    .align 2, 0
.L0802E830: .4byte 0x000003FF
.L0802E834: .4byte 0xFFF00000
.L0802E838: .4byte 0xFFFFFC00
.L0802E83C:
    mov r5, sp
    ldrb r3, [r5, #0xb]
    lsrs r0, r3, #2
    ldrh r1, [r5, #0xc]
    ldr r4, .L0802E874 @ =0x000003FF
    ands r1, r4
    lsls r1, r1, #6
    orrs r1, r0
    lsls r1, r1, #0x10
    movs r0, #0x80
    lsls r0, r0, #0xd
    adds r1, r1, r0
    lsrs r2, r1, #0x10
    movs r0, #0x3f
    ands r2, r0
    lsls r2, r2, #2
    movs r0, #3
    ands r0, r3
    orrs r0, r2
    strb r0, [r5, #0xb]
    lsrs r1, r1, #0x16
    ands r1, r4
    ldrh r2, [r5, #0xc]
    ldr r0, .L0802E878 @ =0xFFFFFC00
    ands r0, r2
    orrs r0, r1
    strh r0, [r5, #0xc]
    b .L0802E89A
    .align 2, 0
.L0802E874: .4byte 0x000003FF
.L0802E878: .4byte 0xFFFFFC00
.L0802E87C:
    ldr r0, [sp, #8]
    lsls r1, r0, #6
    asrs r1, r1, #0x10
    subs r1, #0x10
    b .L0802E88E
.L0802E886:
    ldr r0, [sp, #8]
    lsls r1, r0, #6
    asrs r1, r1, #0x10
    adds r1, #0x10
.L0802E88E:
    lsls r1, r1, #0x10
    lsrs r1, r1, #6
    ldr r2, .L0802E8EC @ =0xFC0003FF
    ands r2, r0
    orrs r2, r1
    str r2, [sp, #8]
.L0802E89A:
    ldrh r2, [r7, #4]
    ldr r1, [r7]
    add r4, sp, #0x14
    ldr r3, [r1]
    adds r0, r4, #0
    ldr r3, [r3, #0x34]
    bl _call_via_r3
    ldr r0, [sp, #8]
    lsls r0, r0, #6
    asrs r5, r0, #0x13
    mov r1, sp
    ldrb r2, [r1, #0xb]
    lsrs r2, r2, #2
    ldrh r0, [r1, #0xc]
    ldr r1, .L0802E8F0 @ =0x000003FF
    ands r0, r1
    lsls r0, r0, #6
    orrs r0, r2
    lsls r0, r0, #0x10
    asrs r3, r0, #0x13
    ldr r2, [sp, #0x14]
    ldrh r0, [r4, #8]
    cmp r2, #0
    beq .L0802E8F4
    cmp r5, r0
    bhi .L0802E8F4
    ldrh r1, [r4, #0xa]
    cmp r3, r1
    bhi .L0802E8F4
    ldr r1, [r4, #4]
    muls r0, r3, r0
    adds r0, r0, r5
    cmp r1, #0
    beq .L0802E8E4
    adds r0, r1, r0
    ldrb r0, [r0]
.L0802E8E4:
    lsls r0, r0, #2
    adds r0, r2, r0
    b .L0802E8F6
    .align 2, 0
.L0802E8EC: .4byte 0xFC0003FF
.L0802E8F0: .4byte 0x000003FF
.L0802E8F4:
    movs r0, #0
.L0802E8F6:
    cmp r0, #0
    beq .L0802E900
    ldr r0, [r0]
    lsrs r0, r0, #0x11
    str r0, [sp, #0x6c]
.L0802E900:
    ldr r2, [sp, #0x6c]
    cmp r2, #0
    beq .L0802E928
.L0802E906:
    ldr r0, [r7]
    ldr r2, [r0]
    movs r3, #0xa0
    lsls r3, r3, #1
    adds r2, r2, r3
    add r1, sp, #0x60
    movs r4, #0
    ldr r5, [sp, #0x6c]
    str r5, [sp, #0x60]
    str r4, [r1, #4]
    ldr r3, [r2]
    movs r2, #0
    bl _call_via_r3
    ldr r0, [sp, #0x7c]
    strb r4, [r0]
    b .L0802EBE6
.L0802E928:
    ldr r0, [r7, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    lsrs r5, r0, #0x18
    cmp r5, #0
    beq .L0802E93A
    b .L0802EB14
.L0802E93A:
    adds r0, r7, #0
    bl func_0802A7E0
    cmp r0, #1
    beq .L0802E9E8
    cmp r0, #1
    bgt .L0802E94E
    cmp r0, #0
    beq .L0802E95C
    b .L0802EBE6
.L0802E94E:
    cmp r0, #2
    bne .L0802E954
    b .L0802EBE6
.L0802E954:
    cmp r0, #3
    bne .L0802E95A
    b .L0802EA86
.L0802E95A:
    b .L0802EBE6
.L0802E95C:
    movs r0, #0xb
    ldr r1, [sp, #0x7c]
    strb r0, [r1]
    adds r1, r7, #0
    adds r1, #0x88
    movs r0, #0xa8
    lsls r0, r0, #0xd
    str r5, [r1, #4]
    str r5, [r1, #8]
    str r0, [r1, #0x14]
    ldr r2, [sp, #0x78]
    ldrb r4, [r2]
    cmp r4, #1
    beq .L0802E9A4
    cmp r4, #1
    bgt .L0802E982
    cmp r4, #0
    beq .L0802E9AC
    b .L0802E9B4
.L0802E982:
    cmp r4, #2
    beq .L0802E98C
    cmp r4, #3
    beq .L0802E998
    b .L0802E9B4
.L0802E98C:
    ldr r0, .L0802E994 @ =0xFFFE5556
    str r0, [r1, #0xc]
    str r5, [r1, #0x10]
    b .L0802E9B2
    .align 2, 0
.L0802E994: .4byte 0xFFFE5556
.L0802E998:
    ldr r0, .L0802E9A0 @ =0x0001AAAA
    str r0, [r1, #0xc]
    str r5, [r1, #0x10]
    b .L0802E9B2
    .align 2, 0
.L0802E9A0: .4byte 0x0001AAAA
.L0802E9A4:
    ldr r0, .L0802E9A8 @ =0xFFFE5556
    b .L0802E9AE
    .align 2, 0
.L0802E9A8: .4byte 0xFFFE5556
.L0802E9AC:
    ldr r0, .L0802E9E4 @ =0x0001AAAA
.L0802E9AE:
    str r5, [r1, #0xc]
    str r0, [r1, #0x10]
.L0802E9B2:
    str r5, [r1, #0x18]
.L0802E9B4:
    ldr r0, [r7]
    ldr r1, [r0]
    movs r3, #0xa6
    lsls r3, r3, #1
    adds r1, r1, r3
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    cmp r4, r5
    bne .L0802E9CE
    b .L0802EB02
.L0802E9CE:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802EA82
    adds r4, #4
    cmp r4, r5
    bne .L0802E9CE
    b .L0802EB02
    .align 2, 0
.L0802E9E4: .4byte 0x0001AAAA
.L0802E9E8:
    movs r0, #0xd
    ldr r1, [sp, #0x7c]
    strb r0, [r1]
    ldr r2, [sp, #0x78]
    ldrb r4, [r2]
    cmp r4, #1
    beq .L0802EA32
    cmp r4, #1
    bgt .L0802EA00
    cmp r4, #0
    beq .L0802EA44
    b .L0802EA56
.L0802EA00:
    cmp r4, #2
    beq .L0802EA0A
    cmp r4, #3
    beq .L0802EA20
    b .L0802EA56
.L0802EA0A:
    adds r0, r7, #0
    adds r0, #0x88
    ldr r1, .L0802EA1C @ =0xFFF60000
    movs r2, #0xc0
    lsls r2, r2, #0xc
    str r1, [r0, #4]
    str r5, [r0, #8]
    b .L0802EA54
    .align 2, 0
.L0802EA1C: .4byte 0xFFF60000
.L0802EA20:
    adds r0, r7, #0
    adds r0, #0x88
    movs r1, #0xa0
    lsls r1, r1, #0xc
    movs r2, #0xc0
    lsls r2, r2, #0xc
    str r1, [r0, #4]
    str r5, [r0, #8]
    b .L0802EA54
.L0802EA32:
    adds r0, r7, #0
    adds r0, #0x88
    ldr r1, .L0802EA40 @ =0xFFF60000
    movs r2, #0xc0
    lsls r2, r2, #0xc
    str r5, [r0, #4]
    b .L0802EA52
    .align 2, 0
.L0802EA40: .4byte 0xFFF60000
.L0802EA44:
    adds r0, r7, #0
    adds r0, #0x88
    movs r1, #0xa0
    lsls r1, r1, #0xc
    movs r2, #0xc0
    lsls r2, r2, #0xc
    str r4, [r0, #4]
.L0802EA52:
    str r1, [r0, #8]
.L0802EA54:
    str r2, [r0, #0x14]
.L0802EA56:
    ldr r0, [r7]
    ldr r1, [r0]
    movs r3, #0xa6
    lsls r3, r3, #1
    adds r1, r1, r3
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    cmp r4, r5
    beq .L0802EB02
.L0802EA6E:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802EA82
    adds r4, #4
    cmp r4, r5
    bne .L0802EA6E
    b .L0802EB02
.L0802EA82:
    ldr r0, [r4]
    b .L0802EB06
.L0802EA86:
    movs r0, #0xc
    ldr r5, [sp, #0x7c]
    strb r0, [r5]
    adds r0, r7, #0
    bl CancelFarmerItemDisplay
    ldr r0, [r7, #0x38]
    adds r0, #0x54
    bl func_0800F390
    ldr r4, [r7, #0x34]
    ldr r0, .L0802EB10 @ =0x00001CCC
    adds r4, r4, r0
    add r0, sp, #8
    adds r1, r7, #0
    bl GetLocation__C7AEntity
    adds r0, r4, #0
    add r1, sp, #8
    movs r2, #6
    bl memcpy
    ldr r0, [r7]
    ldr r1, [r0]
    ldr r2, [r1, #0x38]
    movs r1, #0x4b
    bl _call_via_r2
    ldr r0, [r7]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x4b
    bl _call_via_r2
    cmp r0, #0
    bne .L0802EAD0
    b .L0802EBE6
.L0802EAD0:
    ldr r2, [sp, #0x78]
    ldrb r1, [r2]
    bl func_080380EC
    ldr r0, [r7]
    ldr r1, [r0]
    movs r3, #0xa6
    lsls r3, r3, #1
    adds r1, r1, r3
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    cmp r4, r5
    beq .L0802EB02
.L0802EAF0:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802EA82
    adds r4, #4
    cmp r4, r5
    bne .L0802EAF0
.L0802EB02:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802EB06:
    movs r1, #0x6c
    bl func_08008B6C
    b .L0802EBE6
    .align 2, 0
.L0802EB10: .4byte 0x00001CCC
.L0802EB14:
    movs r0, #0xa
    ldrsh r5, [r7, r0]
    movs r2, #0xe
    ldrsh r1, [r7, r2]
    mov sl, r1
    ldr r3, [sp, #0x78]
    ldrb r4, [r3]
    cmp r4, #1
    beq .L0802EB3A
    cmp r4, #1
    bgt .L0802EB30
    cmp r4, #0
    beq .L0802EB42
    b .L0802EB4E
.L0802EB30:
    cmp r4, #2
    beq .L0802EB48
    cmp r4, #3
    beq .L0802EB4C
    b .L0802EB4E
.L0802EB3A:
    movs r0, #0x10
    rsbs r0, r0, #0
    add sl, r0
    b .L0802EB4E
.L0802EB42:
    movs r1, #0x10
    add sl, r1
    b .L0802EB4E
.L0802EB48:
    subs r5, #0x10
    b .L0802EB4E
.L0802EB4C:
    adds r5, #0x10
.L0802EB4E:
    add r0, sp, #8
    bl __12RucksackItem
    ldr r0, [r7]
    ldr r2, [r0]
    adds r2, #0xe0
    str r2, [sp, #0x74]
    ldrh r3, [r7, #4]
    add r1, sp, #0xc
    ldr r2, .L0802EBF8 @ =0x000003FF
    mov sb, r2
    mov r2, sb
    ands r3, r2
    ldrh r4, [r1]
    ldr r6, .L0802EBFC @ =0xFFFFFC00
    adds r2, r6, #0
    ands r2, r4
    orrs r2, r3
    strh r2, [r1]
    lsls r5, r5, #0x10
    lsrs r3, r5, #0x10
    movs r2, #0x3f
    mov ip, r2
    ands r3, r2
    lsls r3, r3, #2
    str r3, [sp, #0x80]
    ldrb r3, [r1, #1]
    mov r8, r3
    movs r4, #3
    adds r2, r4, #0
    mov r3, r8
    ands r2, r3
    ldr r3, [sp, #0x80]
    orrs r2, r3
    strb r2, [r1, #1]
    lsrs r5, r5, #0x16
    mov r2, sb
    ands r5, r2
    ldrh r3, [r1, #2]
    adds r2, r6, #0
    ands r2, r3
    orrs r2, r5
    strh r2, [r1, #2]
    mov r5, sl
    lsls r3, r5, #0x10
    lsrs r2, r3, #0x10
    mov r5, ip
    ands r2, r5
    lsls r2, r2, #2
    ldrb r5, [r1, #3]
    ands r4, r5
    orrs r4, r2
    strb r4, [r1, #3]
    lsrs r3, r3, #0x16
    mov r2, sb
    ands r3, r2
    ldrh r2, [r1, #4]
    ands r6, r2
    orrs r6, r3
    strh r6, [r1, #4]
    ldr r5, [sp, #0x74]
    ldr r3, [r5]
    add r2, sp, #8
    bl _call_via_r3
    add r0, sp, #8
    bl IsEmpty__C12RucksackItem
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802EBE6
    adds r0, r7, #0
    add r1, sp, #8
    movs r2, #0
    bl func_08026150
.L0802EBE6:
    add sp, #0x84
    pop {r3, r4, r5}
    mov r8, r3
    mov sb, r4
    mov sl, r5
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0
    .align 2, 0
.L0802EBF8: .4byte 0x000003FF
.L0802EBFC: .4byte 0xFFFFFC00

    thumb_func_start func_0802EC00
func_0802EC00: @ 0x0802EC00
    push {r4, r5, r6, r7, lr}
    mov r7, sl
    mov r6, sb
    mov r5, r8
    push {r5, r6, r7}
    sub sp, #0x14
    adds r7, r0, #0
    mov sb, r1
    adds r0, #0xc0
    movs r2, #0
    strb r2, [r0]
    subs r0, #0x46
    strb r2, [r0]
    adds r3, r7, #0
    adds r3, #0xc3
    ldrb r1, [r3]
    adds r4, r1, #0
    cmp r4, #0
    beq .L0802EC40
    subs r0, #0x3e
    strb r1, [r0]
    adds r1, r7, #0
    adds r1, #0xa6
    movs r0, #0
    strh r2, [r1]
    strb r0, [r3]
    ldr r0, [r7, #0x14]
    ldr r1, [r0, #0x6c]
    adds r0, r7, #0
    bl _call_via_r1
    b .L0802F0DC
.L0802EC40:
    adds r0, r7, #0
    bl func_0802AEA4
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802EC5A
    adds r0, r7, #0
    adds r0, #0x3c
    movs r1, #0x35
    strb r1, [r0]
    adds r0, #0x6a
    strh r4, [r0]
    b .L0802F0DC
.L0802EC5A:
    ldr r0, [r7, #0x38]
    adds r4, r0, #0
    adds r4, #0x54
    adds r0, r4, #0
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802EC6E
    b .L0802EDA2
.L0802EC6E:
    adds r0, r4, #0
    bl func_0800F204
    cmp r0, #5
    beq .L0802EC7A
    b .L0802EDA2
.L0802EC7A:
    adds r0, r4, #0
    bl func_0800F360
    adds r1, r0, #0
    ldr r0, .L0802ECA0 @ =0x00000155
    cmp r1, r0
    beq .L0802ED78
    cmp r1, r0
    bgt .L0802ECB8
    cmp r1, #0x6b
    beq .L0802ED78
    cmp r1, #0x6b
    bgt .L0802ECA4
    cmp r1, #0xb
    beq .L0802ED78
    cmp r1, #0x62
    beq .L0802ED78
    b .L0802EDA2
    .align 2, 0
.L0802ECA0: .4byte 0x00000155
.L0802ECA4:
    cmp r1, #0xf7
    beq .L0802ED78
    cmp r1, #0xf7
    bgt .L0802ECB2
    cmp r1, #0xd6
    beq .L0802ED82
    b .L0802EDA2
.L0802ECB2:
    movs r0, #0x91
    lsls r0, r0, #1
    b .L0802ECF2
.L0802ECB8:
    movs r0, #0xc6
    lsls r0, r0, #1
    cmp r1, r0
    beq .L0802ED78
    cmp r1, r0
    bgt .L0802ECDC
    subs r0, #0x25
    cmp r1, r0
    beq .L0802ED82
    cmp r1, r0
    bgt .L0802ECD6
    subs r0, #8
    cmp r1, r0
    beq .L0802ED00
    b .L0802EDA2
.L0802ECD6:
    movs r0, #0xc0
    lsls r0, r0, #1
    b .L0802ECEA
.L0802ECDC:
    movs r0, #0xd3
    lsls r0, r0, #1
    cmp r1, r0
    beq .L0802ED78
    cmp r1, r0
    bgt .L0802ECF0
    subs r0, #0x15
.L0802ECEA:
    cmp r1, r0
    beq .L0802ED78
    b .L0802EDA2
.L0802ECF0:
    ldr r0, .L0802ECF8 @ =0x000001CD
.L0802ECF2:
    cmp r1, r0
    beq .L0802ED82
    b .L0802EDA2
    .align 2, 0
.L0802ECF8: .4byte 0x000001CD
.L0802ECFC:
    ldr r0, [r4]
    b .L0802ED44
.L0802ED00:
    ldr r0, [r7]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    movs r3, #0x3c
    adds r3, r3, r7
    mov sb, r3
    adds r6, r7, #0
    adds r6, #0xa6
    movs r0, #0x88
    adds r0, r0, r7
    mov r8, r0
    movs r1, #0x3d
    adds r1, r1, r7
    mov sl, r1
    cmp r4, r5
    beq .L0802ED40
.L0802ED2E:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802ECFC
    adds r4, #4
    cmp r4, r5
    bne .L0802ED2E
.L0802ED40:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802ED44:
    movs r1, #0xb6
    bl func_08008B6C
    ldr r0, [r7, #0x38]
    adds r0, #0x54
    bl func_0800F390
    ldr r0, .L0802ED74 @ =0xFFF80000
    movs r2, #0
    movs r1, #0x88
    lsls r1, r1, #0xd
    mov r3, r8
    str r0, [r3, #4]
    str r2, [r3, #8]
    str r1, [r3, #0x14]
    movs r0, #0x2f
    mov r1, sb
    strb r0, [r1]
    strh r2, [r6]
    movs r0, #0x78
    mov r2, sl
    strb r0, [r2]
    b .L0802F0DC
    .align 2, 0
.L0802ED74: .4byte 0xFFF80000
.L0802ED78:
    adds r1, r7, #0
    adds r1, #0x3c
    movs r2, #0
    movs r0, #0x38
    b .L0802ED8A
.L0802ED82:
    adds r1, r7, #0
    adds r1, #0x3c
    movs r2, #0
    movs r0, #0x39
.L0802ED8A:
    strb r0, [r1]
    adds r0, r7, #0
    adds r0, #0xa6
    strh r2, [r0]
    adds r1, #1
    movs r0, #0x1e
    strb r0, [r1]
    ldr r0, [r7, #0x38]
    adds r0, #0x54
    bl func_0800F390
    b .L0802F0DC
.L0802EDA2:
    adds r0, r7, #0
    adds r0, #0x3e
    ldrb r5, [r0]
    mov r8, r0
    cmp r5, #0
    bne .L0802EDDC
    adds r0, r4, #0
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802EDDC
    adds r0, r7, #0
    adds r0, #0xa6
    ldrh r2, [r0]
    movs r1, #0x96
    lsls r1, r1, #2
    adds r6, r0, #0
    cmp r2, r1
    bls .L0802EDD6
    adds r1, r7, #0
    adds r1, #0x3c
    movs r0, #0x17
    strb r0, [r1]
    strh r5, [r6]
    b .L0802F0DC
.L0802EDD6:
    adds r0, r2, #1
    strh r0, [r6]
    b .L0802EDE6
.L0802EDDC:
    adds r1, r7, #0
    adds r1, #0xa6
    movs r0, #0
    strh r0, [r1]
    adds r6, r1, #0
.L0802EDE6:
    mov r3, r8
    ldrb r1, [r3]
    cmp r1, #0
    bne .L0802EDF6
    adds r0, r7, #0
    adds r0, #0xc4
    strb r1, [r0]
    b .L0802EE08
.L0802EDF6:
    cmp r1, #0
    blt .L0802EE08
    cmp r1, #2
    bgt .L0802EE08
    adds r1, r7, #0
    adds r1, #0xc4
    ldrb r0, [r1]
    adds r0, #1
    strb r0, [r1]
.L0802EE08:
    movs r2, #0
    str r2, [sp, #0x10]
    str r2, [sp, #0xc]
    mov r0, sb
    cmp r0, #0
    beq .L0802EE1E
    ldrh r0, [r0]
    str r0, [sp, #0x10]
    mov r1, sb
    ldrh r0, [r1, #4]
    str r0, [sp, #0xc]
.L0802EE1E:
    mov r3, r8
    strb r2, [r3]
    add r4, sp, #0xc
    ldrb r3, [r4]
    movs r5, #1
    adds r1, r5, #0
    ands r1, r3
    cmp r1, #0
    beq .L0802EE3A
    adds r0, r7, #0
    adds r0, #0x3c
    strb r5, [r0]
    strh r2, [r6]
    b .L0802EFA2
.L0802EE3A:
    movs r2, #2
    adds r0, r2, #0
    ands r0, r3
    lsls r0, r0, #0x18
    lsrs r3, r0, #0x18
    cmp r3, #0
    beq .L0802EE68
    adds r0, r7, #0
    adds r0, #0x3c
    strb r2, [r0]
    strh r1, [r6]
    ldr r0, [r7, #0x38]
    adds r0, #0x54
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0802EE60
    b .L0802EFA2
.L0802EE60:
    adds r0, r7, #0
    bl func_0802C984
    b .L0802EFA2
.L0802EE68:
    ldrb r1, [r4, #1]
    adds r0, r2, #0
    ands r0, r1
    cmp r0, #0
    beq .L0802EE7E
    adds r1, r7, #0
    adds r1, #0x3c
    movs r0, #3
    strb r0, [r1]
    strh r3, [r6]
    b .L0802EFA2
.L0802EE7E:
    ldr r1, [sp, #0x10]
    lsls r2, r1, #0x19
    lsrs r2, r2, #0x1f
    lsls r0, r1, #0x18
    lsrs r0, r0, #0x1f
    orrs r2, r0
    lsls r0, r1, #0x1a
    lsrs r0, r0, #0x1f
    orrs r0, r2
    lsls r2, r1, #0x1b
    lsrs r2, r2, #0x1f
    orrs r2, r0
    lsls r1, r1, #0x17
    lsrs r1, r1, #0x1f
    orrs r1, r2
    cmp r1, #0
    bne .L0802EEA2
    b .L0802EFA2
.L0802EEA2:
    adds r2, r7, #0
    adds r2, #0x20
    ldrb r5, [r2]
    adds r4, r5, #0
    add r1, sp, #0x10
    ldrb r3, [r1]
    movs r0, #0x20
    ands r0, r3
    adds r6, r2, #0
    mov sb, r1
    cmp r0, #0
    beq .L0802EEBE
    movs r5, #2
    b .L0802EEE0
.L0802EEBE:
    movs r0, #0x10
    ands r0, r3
    cmp r0, #0
    beq .L0802EECA
    movs r5, #3
    b .L0802EEE0
.L0802EECA:
    movs r0, #0x40
    ands r0, r3
    cmp r0, #0
    beq .L0802EED6
    movs r5, #1
    b .L0802EEE0
.L0802EED6:
    movs r0, #0x80
    ands r0, r3
    cmp r0, #0
    beq .L0802EEE0
    movs r5, #0
.L0802EEE0:
    cmp r5, r4
    beq .L0802EF10
    adds r1, r7, #0
    adds r1, #0xb3
    movs r0, #0
    strb r0, [r1]
    movs r4, #0x2b
.L0802EEEE:
    ldr r0, [r7]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    adds r1, r4, #0
    bl _call_via_r2
    adds r2, r0, #0
    cmp r2, #0
    beq .L0802EF0A
    ldr r0, [r2, #0x14]
    ldr r1, [r0, #0x40]
    adds r0, r2, #0
    bl _call_via_r1
.L0802EF0A:
    adds r4, #1
    cmp r4, #0x45
    bls .L0802EEEE
.L0802EF10:
    ldrb r0, [r6]
    cmp r0, r5
    beq .L0802EF1E
    adds r0, r7, #0
    adds r1, r5, #0
    bl SetAnimFacing__12AActorEntityUi
.L0802EF1E:
    ldrb r2, [r6]
    cmp r2, #1
    beq .L0802EF42
    cmp r2, #1
    bgt .L0802EF2E
    cmp r2, #0
    beq .L0802EF48
    b .L0802EF36
.L0802EF2E:
    cmp r2, #2
    beq .L0802EF36
    cmp r2, #3
    beq .L0802EF3C
.L0802EF36:
    movs r2, #0xff
    movs r4, #0
    b .L0802EF4C
.L0802EF3C:
    movs r2, #1
    movs r4, #0
    b .L0802EF4C
.L0802EF42:
    movs r2, #0
    movs r4, #0xff
    b .L0802EF4C
.L0802EF48:
    movs r2, #0
    movs r4, #1
.L0802EF4C:
    ldr r0, [r7, #0x34]
    ldr r1, .L0802EF6C @ =0x00002210
    adds r0, r0, r1
    ldrb r0, [r0]
    lsls r0, r0, #0x1f
    cmp r0, #0
    bne .L0802EF70
    mov r3, sb
    ldrb r1, [r3, #1]
    movs r3, #1
    adds r0, r3, #0
    ands r0, r1
    cmp r0, #0
    beq .L0802EF92
    b .L0802EF7E
    .align 2, 0
.L0802EF6C: .4byte 0x00002210
.L0802EF70:
    mov r3, sb
    ldrb r1, [r3, #1]
    movs r3, #1
    adds r0, r3, #0
    ands r0, r1
    cmp r0, #0
    bne .L0802EF92
.L0802EF7E:
    movs r0, #2
    mov r1, r8
    strb r0, [r1]
    lsls r0, r2, #0x19
    lsrs r2, r0, #0x18
    lsls r0, r4, #0x19
    lsrs r4, r0, #0x18
    b .L0802EF96
.L0802EF8E:
    ldr r0, [r4]
    b .L0802F02E
.L0802EF92:
    mov r0, r8
    strb r3, [r0]
.L0802EF96:
    lsls r0, r2, #0x18
    asrs r0, r0, #8
    str r0, [r7, #0x18]
    lsls r0, r4, #0x18
    asrs r0, r0, #8
    str r0, [r7, #0x1c]
.L0802EFA2:
    ldr r1, .L0802F038 @ =gUnk_080F117F
    mov r0, sp
    movs r2, #3
    bl memcpy
    mov r1, r8
    ldrb r0, [r1]
    add r0, sp
    ldrb r1, [r0]
    cmp r1, #0
    beq .L0802F0AE
    adds r2, r7, #0
    adds r2, #0xc4
    ldrb r0, [r2]
    cmp r0, r1
    blo .L0802F0AE
    movs r0, #0
    strb r0, [r2]
    ldr r6, [r7]
    ldr r4, [r6]
    movs r2, #0x90
    lsls r2, r2, #1
    adds r4, r4, r2
    add r5, sp, #4
    adds r0, r5, #0
    adds r1, r7, #0
    bl GetLocation__C7AEntity
    ldr r2, [r4]
    adds r0, r6, #0
    adds r1, r5, #0
    bl _call_via_r2
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802F07A
    ldr r0, [r7, #0x10]
    cmp r0, #0
    beq .L0802EFF4
    bl func_0802B46C
.L0802EFF4:
    ldr r0, [r7, #0x34]
    adds r0, #0x10
    bl func_0800E324
    cmp r0, #3
    beq .L0802F040
    ldr r0, [r7]
    ldr r1, [r0]
    movs r3, #0xa6
    lsls r3, r3, #1
    adds r1, r1, r3
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    cmp r4, r5
    beq .L0802F02A
.L0802F018:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802EF8E
    adds r4, #4
    cmp r4, r5
    bne .L0802F018
.L0802F02A:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802F02E:
    movs r1, #0xd0
    bl func_08008B6C
    b .L0802F0AE
    .align 2, 0
.L0802F038: .4byte gUnk_080F117F
.L0802F03C:
    ldr r0, [r4]
    b .L0802F06E
.L0802F040:
    ldr r0, [r7]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    cmp r4, r5
    beq .L0802F06A
.L0802F058:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802F03C
    adds r4, #4
    cmp r4, r5
    bne .L0802F058
.L0802F06A:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802F06E:
    movs r1, #0xd1
    bl func_08008B6C
    b .L0802F0AE
.L0802F076:
    ldr r0, [r4]
    b .L0802F0A8
.L0802F07A:
    ldr r0, [r7]
    ldr r1, [r0]
    movs r3, #0xa6
    lsls r3, r3, #1
    adds r1, r1, r3
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    cmp r4, r5
    beq .L0802F0A4
.L0802F092:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802F076
    adds r4, #4
    cmp r4, r5
    bne .L0802F092
.L0802F0A4:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802F0A8:
    movs r1, #0xd0
    bl func_08008B6C
.L0802F0AE:
    movs r1, #0
    ldrh r0, [r7, #0x24]
    cmp r0, #0
    bne .L0802F0B8
    movs r1, #1
.L0802F0B8:
    cmp r1, #0
    beq .L0802F0DC
    mov r1, r8
    ldrb r0, [r1]
    cmp r0, #1
    bne .L0802F0CC
    adds r0, r7, #0
    bl func_0802AD80
    b .L0802F0DC
.L0802F0CC:
    cmp r0, #2
    bne .L0802F0DC
    adds r0, r7, #0
    bl func_0802AD80
    adds r0, r7, #0
    bl func_0802AD80
.L0802F0DC:
    add sp, #0x14
    pop {r3, r4, r5}
    mov r8, r3
    mov sb, r4
    mov sl, r5
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0

    thumb_func_start func_0802F0EC
func_0802F0EC: @ 0x0802F0EC
    push {r4, r5, r6, r7, lr}
    mov r7, sl
    mov r6, sb
    mov r5, r8
    push {r5, r6, r7}
    sub sp, #0x18c
    mov sl, r0
    adds r4, r1, #0
    lsls r2, r2, #0x18
    lsrs r2, r2, #0x18
    movs r3, #0
    str r3, [r0, #0x18]
    str r3, [r0, #0x1c]
    mov r1, sl
    adds r1, #0xc5
    ldrb r0, [r1]
    cmp r0, #0
    beq .L0802F116
    subs r0, #1
    strb r0, [r1]
    movs r4, #0
.L0802F116:
    mov r1, sl
    adds r1, #0xc1
    strb r3, [r1]
    mov r0, sl
    adds r0, #0x3c
    ldrb r3, [r0]
    adds r5, r1, #0
    str r0, [sp, #0x13c]
    cmp r3, #0x39
    bls .L0802F12E
    bl .L08031F0E
.L0802F12E:
    lsls r0, r3, #2
    ldr r1, .L0802F138 @ =.L0802F13C
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L0802F138: .4byte .L0802F13C
.L0802F13C: @ jump table
    .4byte .L0802F224 @ case 0
    .4byte .L0802F238 @ case 1
    .4byte .L0802F244 @ case 2
    .4byte .L0802F250 @ case 3
    .4byte .L0802F264 @ case 4
    .4byte .L0802F270 @ case 5
    .4byte .L0802F27C @ case 6
    .4byte .L0802F296 @ case 7
    .4byte .L0802F2A2 @ case 8
    .4byte .L0802FC14 @ case 9
    .4byte .L0802FA30 @ case 10
    .4byte .L0802F884 @ case 11
    .4byte .L0802F396 @ case 12
    .4byte .L0802F3C8 @ case 13
    .4byte .L0802F332 @ case 14
    .4byte .L08031A92 @ case 15
    .4byte .L08031A92 @ case 16
    .4byte .L08031A64 @ case 17
    .4byte .L08031936 @ case 18
    .4byte .L08031950 @ case 19
    .4byte .L080319A6 @ case 20
    .4byte .L08031886 @ case 21
    .4byte .L080318A0 @ case 22
    .4byte .L080319BC @ case 23
    .4byte .L08031A92 @ case 24
    .4byte .L0803123E @ case 25
    .4byte .L080316C6 @ case 26
    .4byte .L08031698 @ case 27
    .4byte .L080314E6 @ case 28
    .4byte .L08031682 @ case 29
    .4byte .L08030E50 @ case 30
    .4byte .L08030EE2 @ case 31
    .4byte .L08030F5A @ case 32
    .4byte .L080311F4 @ case 33
    .4byte .L080319E0 @ case 34
    .4byte .L08031A38 @ case 35
    .4byte .L08031AA8 @ case 36
    .4byte .L08031AA8 @ case 37
    .4byte .L08031AA8 @ case 38
    .4byte .L08031AA8 @ case 39
    .4byte .L08031AA8 @ case 40
    .4byte .L08031AA8 @ case 41
    .4byte .L08031AC0 @ case 42
    .4byte .L08031F0E @ case 43
    .4byte .L08031B82 @ case 44
    .4byte .L08031AFC @ case 45
    .4byte .L08031C64 @ case 46
    .4byte .L08031C84 @ case 47
    .4byte .L08031CB6 @ case 48
    .4byte .L08031D0C @ case 49
    .4byte .L08031D2A @ case 50
    .4byte .L08031D80 @ case 51
    .4byte .L08031E8A @ case 52
    .4byte .L08031EAE @ case 53
    .4byte .L08031D9A @ case 54
    .4byte .L08031E10 @ case 55
    .4byte .L08031DB8 @ case 56
    .4byte .L08031E34 @ case 57
.L0802F224:
    mov r0, sl
    adds r1, r4, #0
    bl func_0802EC00
    movs r1, #0
    ldr r2, [sp, #0x13c]
    ldrb r0, [r2]
    cmp r0, #0
    bne .L0802F290
    b .L0802F28E
.L0802F238:
    mov r0, sl
    adds r1, r4, #0
    bl func_0802E0FC
    bl .L08031F0E
.L0802F244:
    mov r0, sl
    adds r1, r4, #0
    bl func_0802DE94
    bl .L08031F0E
.L0802F250:
    mov r0, sl
    adds r1, r4, #0
    bl func_0802DBF0
    movs r1, #0
    ldr r3, [sp, #0x13c]
    ldrb r0, [r3]
    cmp r0, #3
    bne .L0802F290
    b .L0802F28E
.L0802F264:
    mov r0, sl
    adds r1, r4, #0
    bl func_0802D9AC
    bl .L08031F0E
.L0802F270:
    mov r0, sl
    adds r1, r4, #0
    bl func_0802D8E8
    bl .L08031F0E
.L0802F27C:
    mov r0, sl
    adds r1, r4, #0
    bl func_0802D844
    movs r1, #0
    ldr r4, [sp, #0x13c]
    ldrb r0, [r4]
    cmp r0, #6
    bne .L0802F290
.L0802F28E:
    movs r1, #1
.L0802F290:
    strb r1, [r5]
    bl .L08031F0E
.L0802F296:
    mov r0, sl
    adds r1, r4, #0
    bl func_0802D59C
    bl .L08031F0E
.L0802F2A2:
    mov r5, sl
    adds r5, #0xc3
    ldrb r0, [r5]
    cmp r0, #0
    beq .L0802F2E0
    cmp r0, #0x2a
    bne .L0802F2DC
    mov r6, sl
    ldr r0, [r6]
    ldr r2, [r0]
    movs r7, #0xa0
    lsls r7, r7, #1
    adds r2, r2, r7
    add r1, sp, #0x30
    ldr r3, .L0802F2D8 @ =0x000002B6
    movs r4, #0
    str r3, [sp, #0x30]
    str r4, [r1, #4]
    ldr r3, [r2]
    movs r2, #2
    bl _call_via_r3
    strb r4, [r5]
    ldr r0, [r6, #0x14]
    ldr r1, [r0, #0x6c]
    bl .L08031EA6
    .align 2, 0
.L0802F2D8: .4byte 0x000002B6
.L0802F2DC:
    movs r0, #0
    strb r0, [r5]
.L0802F2E0:
    cmp r4, #0
    beq .L0802F306
    ldrh r0, [r4, #8]
    movs r1, #1
    ands r0, r1
    cmp r0, #0
    beq .L0802F306
    mov r1, sl
    ldr r0, [r1]
    ldr r2, [r0]
    movs r3, #0xa0
    lsls r3, r3, #1
    adds r2, r2, r3
    add r1, sp, #0x38
    movs r3, #0x56
    movs r4, #0
    str r3, [sp, #0x38]
    bl sub_0802FBFC
.L0802F306:
    mov r1, sl
    adds r1, #0x3d
    ldrb r0, [r1]
    cmp r0, #0
    beq .L0802F314
    subs r0, #1
    strb r0, [r1]
.L0802F314:
    cmp r2, #0
    bne .L0802F31C
    bl .L08031F0E
.L0802F31C:
    mov r4, sl
    ldr r0, [r4, #0x14]
    movs r2, #1
    rsbs r2, r2, #0
    ldr r3, [r0, #0x64]
    mov r0, sl
    movs r1, #1
    bl _call_via_r3
    bl .L08031F0E
.L0802F332:
    movs r1, #0
    mov r5, sl
    ldrh r0, [r5, #0x24]
    cmp r0, #0
    bne .L0802F33E
    movs r1, #1
.L0802F33E:
    cmp r1, #0
    bne .L0802F346
    bl .L08031F0E
.L0802F346:
    movs r0, #0
    ldr r6, [sp, #0x13c]
    strb r0, [r6]
    mov r7, sl
    ldr r0, [r7, #0x38]
    adds r4, r0, #0
    adds r4, #0x54
    adds r0, r4, #0
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802F364
    bl .L08031D78
.L0802F364:
    adds r0, r4, #0
    bl func_0800F204
    cmp r0, #1
    bne .L0802F388
    adds r0, r4, #0
    bl func_0800F258
    adds r1, r0, #0
    add r0, sp, #0x40
    strb r1, [r0]
    bl GetId__C7Article
    cmp r0, #0x35
    bne .L0802F388
    ldr r0, [r7, #0x38]
    bl func_0800ED2C
.L0802F388:
    mov r1, sl
    ldr r0, [r1, #0x38]
    adds r0, #0x54
    bl func_0800F390
    bl .L08031D78
.L0802F396:
    movs r1, #0
    mov r2, sl
    ldrh r0, [r2, #0x24]
    cmp r0, #0
    bne .L0802F3A2
    movs r1, #1
.L0802F3A2:
    cmp r1, #0
    bne .L0802F3AA
    bl .L08031F0E
.L0802F3AA:
    movs r2, #0
    ldr r3, [sp, #0x13c]
    strb r2, [r3]
    mov r1, sl
    adds r1, #0xa8
    movs r0, #1
    str r0, [r1]
    mov r0, sl
    adds r0, #0xb0
    strb r2, [r0]
    adds r1, #4
    movs r0, #0x35
    str r0, [r1]
    bl .L08031F0E
.L0802F3C8:
    movs r1, #0
    mov r4, sl
    ldrh r0, [r4, #0x24]
    cmp r0, #0
    bne .L0802F3D4
    movs r1, #1
.L0802F3D4:
    cmp r1, #0
    bne .L0802F3DC
    bl .L08031F0E
.L0802F3DC:
    movs r0, #0
    ldr r5, [sp, #0x13c]
    strb r0, [r5]
    mov r6, sl
    ldr r0, [r6, #0x38]
    adds r4, r0, #0
    adds r4, #0x54
    movs r0, #0xa
    ldrsh r7, [r6, r0]
    movs r2, #0xe
    ldrsh r1, [r6, r2]
    str r1, [sp, #0xec]
    mov r0, sl
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, #1
    beq .L0802F412
    cmp r0, #1
    bgt .L0802F408
    cmp r0, #0
    beq .L0802F41A
    b .L0802F428
.L0802F408:
    cmp r0, #2
    beq .L0802F422
    cmp r0, #3
    beq .L0802F426
    b .L0802F428
.L0802F412:
    ldr r3, [sp, #0xec]
    subs r3, #0x10
    str r3, [sp, #0xec]
    b .L0802F428
.L0802F41A:
    ldr r5, [sp, #0xec]
    adds r5, #0x10
    str r5, [sp, #0xec]
    b .L0802F428
.L0802F422:
    subs r7, #0x10
    b .L0802F428
.L0802F426:
    adds r7, #0x10
.L0802F428:
    adds r0, r4, #0
    bl func_0800F204
    cmp r0, #5
    bls .L0802F436
    bl .L08031D78
.L0802F436:
    lsls r0, r0, #2
    ldr r1, .L0802F440 @ =.L0802F444
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L0802F440: .4byte .L0802F444
.L0802F444: @ jump table
    .4byte .L08031D78 @ case 0
    .4byte .L0802F45C @ case 1
    .4byte .L0802F500 @ case 2
    .4byte .L0802F648 @ case 3
    .4byte .L0802F7E6 @ case 4
    .4byte .L08031D78 @ case 5
.L0802F45C:
    adds r0, r4, #0
    bl func_0800F258
    mov r6, sp
    adds r6, #0x41
    str r6, [sp, #0x128]
    strb r0, [r6]
    mov r1, sl
    ldr r0, [r1, #0x38]
    adds r0, #0x54
    bl func_0800F390
    mov r2, sl
    ldr r0, [r2]
    ldr r1, [r0]
    adds r1, #0xe4
    str r1, [sp, #0xf0]
    ldrh r2, [r2, #4]
    mov r6, sp
    ldr r3, .L0802F4F8 @ =0x000003FF
    mov sb, r3
    mov r4, sb
    ands r2, r4
    ldrh r3, [r6]
    ldr r5, .L0802F4FC @ =0xFFFFFC00
    adds r1, r5, #0
    ands r1, r3
    orrs r1, r2
    strh r1, [r6]
    lsls r4, r7, #0x10
    lsrs r2, r4, #0x10
    movs r7, #0x3f
    mov ip, r7
    mov r1, ip
    ands r2, r1
    lsls r2, r2, #2
    ldrb r3, [r6, #1]
    mov r8, r3
    movs r3, #3
    adds r1, r3, #0
    mov r7, r8
    ands r1, r7
    orrs r1, r2
    strb r1, [r6, #1]
    lsrs r4, r4, #0x16
    mov r1, sb
    ands r4, r1
    ldrh r2, [r6, #2]
    adds r1, r5, #0
    ands r1, r2
    orrs r1, r4
    strh r1, [r6, #2]
    ldr r4, [sp, #0xec]
    lsls r2, r4, #0x10
    lsrs r1, r2, #0x10
    mov r7, ip
    ands r1, r7
    lsls r1, r1, #2
    ldrb r4, [r6, #3]
    ands r3, r4
    orrs r3, r1
    strb r3, [r6, #3]
    lsrs r2, r2, #0x16
    mov r1, sb
    ands r2, r1
    ldrh r1, [r6, #4]
    ands r5, r1
    orrs r5, r2
    strh r5, [r6, #4]
    ldr r2, [sp, #0xf0]
    ldr r3, [r2]
    mov r1, sp
    ldr r2, [sp, #0x128]
    bl _call_via_r3
    bl .L08031D78
    .align 2, 0
.L0802F4F8: .4byte 0x000003FF
.L0802F4FC: .4byte 0xFFFFFC00
.L0802F500:
    mov r3, sl
    ldr r0, [r3, #0x38]
    adds r0, #0x54
    bl func_0800F390
    mov r4, sl
    ldr r0, [r4, #0x34]
    ldr r5, .L0802F5E8 @ =0x00001C70
    adds r5, r0, r5
    str r5, [sp, #0xf4]
    ldrh r1, [r4, #4]
    mov r5, sp
    ldr r6, .L0802F5EC @ =0x000003FF
    mov r8, r6
    mov r0, r8
    ands r1, r0
    ldrh r2, [r5]
    ldr r4, .L0802F5F0 @ =0xFFFFFC00
    adds r0, r4, #0
    ands r0, r2
    orrs r0, r1
    strh r0, [r5]
    lsls r3, r7, #0x10
    lsrs r1, r3, #0x10
    movs r2, #0x3f
    mov sb, r2
    ands r1, r2
    lsls r1, r1, #2
    ldrb r6, [r5, #1]
    movs r2, #3
    adds r0, r2, #0
    ands r0, r6
    orrs r0, r1
    strb r0, [r5, #1]
    lsrs r3, r3, #0x16
    mov r6, r8
    ands r3, r6
    ldrh r1, [r5, #2]
    adds r0, r4, #0
    ands r0, r1
    orrs r0, r3
    strh r0, [r5, #2]
    ldr r7, [sp, #0xec]
    lsls r1, r7, #0x10
    lsrs r0, r1, #0x10
    mov r3, sb
    ands r0, r3
    lsls r0, r0, #2
    ldrb r3, [r5, #3]
    ands r2, r3
    orrs r2, r0
    strb r2, [r5, #3]
    lsrs r1, r1, #0x16
    ands r1, r6
    ldrh r0, [r5, #4]
    ands r4, r0
    orrs r4, r1
    strh r4, [r5, #4]
    add r4, sp, #8
    movs r5, #0
    adds r0, r4, #0
    mov r1, sp
    movs r2, #6
    bl memcpy
    strb r5, [r4, #6]
    ldr r0, [sp, #0xf4]
    adds r1, r4, #0
    bl SetLocation__5ActorRC13ActorLocation
    ldr r0, [sp, #0xf4]
    bl HasBeenTalkedTo__C6Animal
    movs r1, #1
    eors r0, r1
    lsls r0, r0, #0x18
    lsrs r6, r0, #0x18
    cmp r6, #0
    beq .L0802F5AC
    ldr r0, [sp, #0xf4]
    bl SetTalkedTo__6Animal
    ldr r0, [sp, #0xf4]
    movs r1, #1
    bl AddAffection__6Animali
.L0802F5AC:
    mov r4, sl
    ldr r5, [r4]
    ldr r0, [r5]
    ldr r2, [r0, #0x38]
    adds r0, r5, #0
    movs r1, #0x2b
    bl _call_via_r2
    ldr r0, [r5]
    ldr r2, [r0, #0x40]
    adds r0, r5, #0
    movs r1, #0x2b
    bl _call_via_r2
    adds r4, r0, #0
    cmp r4, #0
    bne .L0802F5D2
    bl .L08031D78
.L0802F5D2:
    mov r7, sl
    ldr r0, [r7, #0x14]
    ldr r1, [r0, #0x28]
    mov r0, sl
    bl _call_via_r1
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802F5F8
    b .L0802F742
    .align 2, 0
.L0802F5E8: .4byte 0x00001C70
.L0802F5EC: .4byte 0x000003FF
.L0802F5F0: .4byte 0xFFFFFC00
.L0802F5F4:
    ldr r0, [r4]
    b .L0802F642
.L0802F5F8:
    ldr r0, [r4, #0x14]
    adds r0, #0x88
    ldr r2, [r0]
    adds r0, r4, #0
    movs r1, #0x3c
    bl _call_via_r2
    cmp r6, #0
    beq .L0802F614
    adds r0, r4, #0
    movs r1, #1
    movs r2, #0
    bl func_08032384
.L0802F614:
    ldr r0, [r5]
    movs r1, #0xa6
    lsls r1, r1, #1
    adds r0, r0, r1
    ldr r1, [r0]
    adds r0, r5, #0
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    cmp r4, r5
    beq .L0802F63E
.L0802F62C:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802F5F4
    adds r4, #4
    cmp r4, r5
    bne .L0802F62C
.L0802F63E:
    subs r0, r5, #4
    ldr r0, [r0]
.L0802F642:
    movs r1, #0xd2
    bl sub_08031D74
.L0802F648:
    adds r0, r4, #0
    bl func_0800F344
    str r0, [sp, #0xfc]
    mov r2, sl
    ldr r0, [r2, #0x34]
    movs r3, #0x82
    lsls r3, r3, #3
    adds r0, r0, r3
    ldr r1, [sp, #0xfc]
    bl GetChicken__4CoopUi
    str r0, [sp, #0xf8]
    mov r4, sl
    ldr r0, [r4, #0x38]
    adds r0, #0x54
    bl func_0800F390
    ldrh r2, [r4, #4]
    add r1, sp, #8
    ldr r5, .L0802F750 @ =0x000003FF
    mov r8, r5
    mov r6, r8
    ands r2, r6
    ldrh r3, [r1]
    ldr r5, .L0802F754 @ =0xFFFFFC00
    adds r0, r5, #0
    ands r0, r3
    orrs r0, r2
    strh r0, [r1]
    lsls r4, r7, #0x10
    lsrs r2, r4, #0x10
    movs r7, #0x3f
    mov sb, r7
    mov r0, sb
    ands r2, r0
    lsls r2, r2, #2
    ldrb r6, [r1, #1]
    movs r3, #3
    adds r0, r3, #0
    ands r0, r6
    orrs r0, r2
    strb r0, [r1, #1]
    lsrs r4, r4, #0x16
    mov r2, r8
    ands r4, r2
    ldrh r2, [r1, #2]
    adds r0, r5, #0
    ands r0, r2
    orrs r0, r4
    strh r0, [r1, #2]
    ldr r4, [sp, #0xec]
    lsls r2, r4, #0x10
    lsrs r0, r2, #0x10
    mov r6, sb
    ands r0, r6
    lsls r0, r0, #2
    ldrb r4, [r1, #3]
    ands r3, r4
    orrs r3, r0
    strb r3, [r1, #3]
    lsrs r2, r2, #0x16
    mov r7, r8
    ands r2, r7
    ldrh r0, [r1, #4]
    ands r5, r0
    orrs r5, r2
    strh r5, [r1, #4]
    mov r5, sp
    movs r4, #0
    mov r0, sp
    movs r2, #6
    bl memcpy
    strb r4, [r5, #6]
    ldr r0, [sp, #0xf8]
    mov r1, sp
    bl SetLocation__5ActorRC13ActorLocation
    ldr r0, [sp, #0xf8]
    bl HasBeenTalkedTo__C6Animal
    movs r1, #1
    eors r0, r1
    lsls r0, r0, #0x18
    lsrs r6, r0, #0x18
    cmp r6, #0
    beq .L0802F706
    ldr r0, [sp, #0xf8]
    bl SetTalkedTo__6Animal
    ldr r0, [sp, #0xf8]
    movs r1, #1
    bl AddAffection__6Animali
.L0802F706:
    mov r0, sl
    ldr r5, [r0]
    ldr r4, [sp, #0xfc]
    adds r4, #0x2e
    ldr r0, [r5]
    ldr r2, [r0, #0x38]
    adds r0, r5, #0
    adds r1, r4, #0
    bl _call_via_r2
    ldr r0, [r5]
    ldr r2, [r0, #0x40]
    adds r0, r5, #0
    adds r1, r4, #0
    bl _call_via_r2
    adds r4, r0, #0
    cmp r4, #0
    bne .L0802F730
    bl .L08031D78
.L0802F730:
    mov r1, sl
    ldr r0, [r1, #0x14]
    ldr r1, [r0, #0x28]
    mov r0, sl
    bl _call_via_r1
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802F758
.L0802F742:
    ldr r0, [r4, #0x14]
    ldr r1, [r0, #0x20]
    adds r0, r4, #0
    bl _call_via_r1
    bl .L08031D78
    .align 2, 0
.L0802F750: .4byte 0x000003FF
.L0802F754: .4byte 0xFFFFFC00
.L0802F758:
    ldr r0, [r4, #0x14]
    adds r0, #0x88
    ldr r2, [r0]
    adds r0, r4, #0
    movs r1, #0x3c
    bl _call_via_r2
    ldr r0, [sp, #0xf8]
    bl IsSick__C9Livestock
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802F778
    adds r0, r4, #0
    movs r1, #7
    b .L0802F788
.L0802F778:
    ldr r0, [sp, #0xf8]
    bl IsUnhappy__C9Livestock
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802F796
    adds r0, r4, #0
    movs r1, #6
.L0802F788:
    movs r2, #0
    bl func_08032384
    bl .L08031D78
.L0802F792:
    ldr r4, [r4]
    b .L0802F7D2
.L0802F796:
    cmp r6, #0
    beq .L0802F7A4
    adds r0, r4, #0
    movs r1, #1
    movs r2, #0
    bl func_08032384
.L0802F7A4:
    ldr r0, [r5]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r0, r0, r2
    ldr r1, [r0]
    adds r0, r5, #0
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    cmp r4, r5
    beq .L0802F7CE
.L0802F7BC:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802F792
    adds r4, #4
    cmp r4, r5
    bne .L0802F7BC
.L0802F7CE:
    subs r0, r5, #4
    ldr r4, [r0]
.L0802F7D2:
    ldr r0, [sp, #0xf8]
    bl GetGrowthStage__C7Chicken
    movs r1, #0xa5
    cmp r0, #1
    bne .L0802F7E0
    movs r1, #0xcc
.L0802F7E0:
    adds r0, r4, #0
    bl sub_08031D74
.L0802F7E6:
    mov r3, sl
    ldr r0, [r3, #0x38]
    adds r0, #0x54
    bl func_0800F390
    mov r4, sl
    ldr r0, [r4, #0x34]
    movs r5, #0xe5
    lsls r5, r5, #5
    adds r0, r0, r5
    ldrh r2, [r4, #4]
    mov r6, sp
    ldr r1, .L0802F87C @ =0x000003FF
    mov sb, r1
    mov r3, sb
    ands r2, r3
    ldrh r3, [r6]
    ldr r5, .L0802F880 @ =0xFFFFFC00
    adds r1, r5, #0
    ands r1, r3
    orrs r1, r2
    strh r1, [r6]
    lsls r4, r7, #0x10
    lsrs r2, r4, #0x10
    movs r7, #0x3f
    mov ip, r7
    mov r1, ip
    ands r2, r1
    lsls r2, r2, #2
    ldrb r3, [r6, #1]
    mov r8, r3
    movs r3, #3
    adds r1, r3, #0
    mov r7, r8
    ands r1, r7
    orrs r1, r2
    strb r1, [r6, #1]
    lsrs r4, r4, #0x16
    mov r1, sb
    ands r4, r1
    ldrh r2, [r6, #2]
    adds r1, r5, #0
    ands r1, r2
    orrs r1, r4
    strh r1, [r6, #2]
    ldr r4, [sp, #0xec]
    lsls r2, r4, #0x10
    lsrs r1, r2, #0x10
    mov r7, ip
    ands r1, r7
    lsls r1, r1, #2
    ldrb r4, [r6, #3]
    ands r3, r4
    orrs r3, r1
    strb r3, [r6, #3]
    lsrs r2, r2, #0x16
    mov r1, sb
    ands r2, r1
    ldrh r1, [r6, #4]
    ands r5, r1
    orrs r5, r2
    strh r5, [r6, #4]
    mov r1, sp
    bl func_08010014
    mov r2, sl
    ldr r0, [r2]
    ldr r1, [r0]
    ldr r2, [r1, #0x38]
    movs r1, #0x4a
    bl _call_via_r2
    bl .L08031D78
    .align 2, 0
.L0802F87C: .4byte 0x000003FF
.L0802F880: .4byte 0xFFFFFC00
.L0802F884:
    mov r2, sl
    adds r2, #0x88
    ldr r0, [r2, #4]
    ldr r1, [r2, #0xc]
    adds r0, r0, r1
    str r0, [r2, #4]
    ldr r0, [r2, #8]
    ldr r1, [r2, #0x10]
    adds r0, r0, r1
    str r0, [r2, #8]
    ldr r0, [r2, #0x18]
    ldr r3, .L0802F8AC @ =0xFFFFB556
    adds r0, r0, r3
    str r0, [r2, #0x18]
    ldr r1, [r2, #0x14]
    adds r1, r1, r0
    cmp r1, #0
    ble .L0802F8B0
    str r1, [r2, #0x14]
    b .L0802F8B6
    .align 2, 0
.L0802F8AC: .4byte 0xFFFFB556
.L0802F8B0:
    movs r0, #0
    str r0, [r2, #0x14]
    str r0, [r2, #0x18]
.L0802F8B6:
    movs r1, #0
    mov r4, sl
    ldrh r0, [r4, #0x24]
    cmp r0, #0
    bne .L0802F8C2
    movs r1, #1
.L0802F8C2:
    cmp r1, #0
    bne .L0802F8CA
    bl .L08031F0E
.L0802F8CA:
    movs r0, #0
    ldr r5, [sp, #0x13c]
    strb r0, [r5]
    mov r6, sl
    ldr r1, [r6, #0x38]
    adds r1, #0x54
    mov r0, sp
    bl func_0800F294
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F204
    mov r4, sl
    adds r4, #0xa8
    str r0, [r4]
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    bl func_0800F388
    mov r1, sl
    adds r1, #0xb0
    strb r0, [r1]
    ldr r0, [r4]
    adds r5, r4, #0
    cmp r0, #0
    beq .L0802F906
    cmp r0, #1
    beq .L0802F924
    b .L0802F940
.L0802F906:
    mov r7, sl
    ldr r1, [r7, #0x38]
    adds r1, #0x54
    add r4, sp, #4
    adds r0, r4, #0
    bl func_0800F20C
    adds r0, r4, #0
    bl GetId__C4Food
    mov r1, sl
    adds r1, #0xac
    str r0, [r1]
    adds r7, r4, #0
    b .L0802F942
.L0802F924:
    mov r1, sl
    ldr r0, [r1, #0x38]
    adds r0, #0x54
    bl func_0800F258
    adds r1, r0, #0
    mov r0, sp
    adds r0, #0x42
    strb r1, [r0]
    bl GetId__C7Article
    mov r1, sl
    adds r1, #0xac
    str r0, [r1]
.L0802F940:
    add r7, sp, #4
.L0802F942:
    mov r2, sl
    ldr r0, [r2, #0x38]
    adds r0, #0x54
    bl func_0800F390
    ldr r0, [r5]
    cmp r0, #1
    bne .L0802F964
    mov r0, sl
    adds r0, #0xac
    ldr r0, [r0]
    cmp r0, #0x35
    bne .L0802F964
    mov r3, sl
    ldr r0, [r3, #0x38]
    bl func_0800ED2C
.L0802F964:
    mov r0, sl
    bl CancelFarmerItemDisplay
    mov r4, sl
    movs r5, #0xa
    ldrsh r3, [r4, r5]
    movs r0, #0xe
    ldrsh r6, [r4, r0]
    mov sb, r6
    mov r0, sl
    adds r0, #0x20
    ldrb r0, [r0]
    adds r1, r0, #0
    cmp r0, #1
    beq .L0802F99C
    cmp r0, #1
    bgt .L0802F98C
    cmp r0, #0
    beq .L0802F996
    b .L0802F9AA
.L0802F98C:
    cmp r1, #2
    beq .L0802F9A4
    cmp r1, #3
    beq .L0802F9A8
    b .L0802F9AA
.L0802F996:
    movs r1, #0x14
    add sb, r1
    b .L0802F9AA
.L0802F99C:
    movs r2, #0x14
    rsbs r2, r2, #0
    add sb, r2
    b .L0802F9AA
.L0802F9A4:
    subs r3, #0x14
    b .L0802F9AA
.L0802F9A8:
    adds r3, #0x14
.L0802F9AA:
    mov r4, sl
    ldrh r1, [r4, #4]
    ldr r5, .L0802FA28 @ =0x000003FF
    adds r6, r5, #0
    ands r1, r6
    ldrh r2, [r7]
    ldr r4, .L0802FA2C @ =0xFFFFFC00
    adds r0, r4, #0
    ands r0, r2
    orrs r0, r1
    strh r0, [r7]
    lsls r3, r3, #0x10
    lsrs r1, r3, #0x10
    movs r0, #0x3f
    mov r8, r0
    ands r1, r0
    lsls r1, r1, #2
    ldrb r5, [r7, #1]
    movs r2, #3
    adds r0, r2, #0
    ands r0, r5
    orrs r0, r1
    strb r0, [r7, #1]
    lsrs r3, r3, #0x16
    ands r3, r6
    ldrh r1, [r7, #2]
    adds r0, r4, #0
    ands r0, r1
    orrs r0, r3
    strh r0, [r7, #2]
    mov r3, sb
    lsls r1, r3, #0x10
    lsrs r0, r1, #0x10
    mov r5, r8
    ands r0, r5
    lsls r0, r0, #2
    ldrb r3, [r7, #3]
    ands r2, r3
    orrs r2, r0
    strb r2, [r7, #3]
    lsrs r1, r1, #0x16
    ands r1, r6
    ldrh r0, [r7, #4]
    ands r4, r0
    orrs r4, r1
    strh r4, [r7, #4]
    mov r6, sl
    ldr r0, [r6]
    ldr r1, [r0]
    movs r2, #0xac
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r3, [r1]
    adds r1, r7, #0
    mov r2, sp
    bl _call_via_r3
    mov r1, sl
    adds r1, #0xc5
    strb r0, [r1]
    bl .L08031F0E
    .align 2, 0
.L0802FA28: .4byte 0x000003FF
.L0802FA2C: .4byte 0xFFFFFC00
.L0802FA30:
    mov r1, sl
    adds r1, #0x3d
    ldrb r0, [r1]
    subs r0, #1
    strb r0, [r1]
    lsls r0, r0, #0x18
    lsrs r5, r0, #0x18
    cmp r5, #0
    beq .L0802FA46
    bl .L08031F0E
.L0802FA46:
    ldr r3, [sp, #0x13c]
    strb r5, [r3]
    adds r1, #0x3d
    movs r0, #1
    strb r0, [r1]
    mov r0, sl
    bl CancelFarmerItemDisplay
    mov r6, sl
    ldr r4, [r6, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #0x43
    strb r1, [r0]
    bl GetId__C4Tool
    cmp r0, #0x4c
    bne .L0802FA7E
    bl .L08031BEE
.L0802FA7E:
    cmp r0, #0x4c
    bgt .L0802FA8A
    cmp r0, #0x4b
    beq .L0802FABC
    bl .L08031F0E
.L0802FA8A:
    cmp r0, #0x4d
    beq .L0802FA92
    bl .L08031F0E
.L0802FA92:
    mov r7, sl
    ldr r0, [r7, #0x38]
    bl func_0800E4FC
    cmp r0, #0x64
    bne .L0802FAA2
    bl .L08031F0E
.L0802FAA2:
    ldr r0, [r7]
    ldr r3, [r0]
    movs r1, #0xa0
    lsls r1, r1, #1
    adds r3, r3, r1
    add r1, sp, #0x44
    ldr r2, .L0802FAB8 @ =0x000003DF
    str r2, [sp, #0x44]
    str r5, [r1, #4]
    ldr r3, [r3]
    b .L0802FC00
    .align 2, 0
.L0802FAB8: .4byte 0x000003DF
.L0802FABC:
    mov r0, sp
    mov r1, sl
    bl GetLocation__C7AEntity
    mov r4, sl
    adds r4, #0x20
    ldrb r2, [r4]
    add r0, sp, #0x4c
    mov r1, sl
    bl func_0802AB98
    ldr r0, [sp, #0x4c]
    ldr r1, [sp, #0x50]
    str r0, [sp, #0x54]
    str r1, [sp, #0x58]
    add r2, sp, #0x54
    mov r3, sl
    ldr r0, [r3]
    ldr r1, [r0]
    movs r6, #0x9a
    lsls r6, r6, #1
    adds r1, r1, r6
    ldr r3, [r1]
    mov r1, sp
    bl _call_via_r3
    adds r3, r0, #0
    subs r0, r3, #1
    cmp r0, #0x29
    bhi .L0802FB3C
    mov r7, sl
    ldr r0, [r7]
    ldr r1, [r0]
    movs r2, #0x96
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r2, [r1]
    adds r1, r3, #0
    bl _call_via_r2
    adds r2, r0, #0
    cmp r2, #0
    bne .L0802FB16
    bl .L08031F0E
.L0802FB16:
    ldr r0, [r2, #0x14]
    ldr r1, [r0, #0x34]
    adds r0, r2, #0
    bl _call_via_r1
    adds r3, r0, #0
    cmp r3, #0
    bne .L0802FB2A
    bl .L08031F0E
.L0802FB2A:
    ldr r0, [r7]
    ldr r2, [r0]
    movs r4, #0xa0
    lsls r4, r4, #1
    adds r2, r2, r4
    add r1, sp, #0x5c
    str r3, [sp, #0x5c]
    str r5, [r1, #4]
    b .L0802FBFE
.L0802FB3C:
    ldrb r0, [r4]
    cmp r0, #1
    beq .L0802FB56
    cmp r0, #1
    bgt .L0802FB4C
    cmp r0, #0
    beq .L0802FB78
    b .L0802FBDA
.L0802FB4C:
    cmp r0, #2
    beq .L0802FBBC
    cmp r0, #3
    beq .L0802FBC6
    b .L0802FBDA
.L0802FB56:
    mov r3, sp
    mov r0, sp
    ldrb r4, [r0, #3]
    lsrs r2, r4, #2
    ldrh r1, [r0, #4]
    ldr r5, .L0802FB70 @ =0x000003FF
    ands r1, r5
    lsls r1, r1, #6
    orrs r1, r2
    lsls r1, r1, #0x10
    ldr r6, .L0802FB74 @ =0xFFF00000
    adds r1, r1, r6
    b .L0802FB92
    .align 2, 0
.L0802FB70: .4byte 0x000003FF
.L0802FB74: .4byte 0xFFF00000
.L0802FB78:
    mov r3, sp
    mov r0, sp
    ldrb r4, [r0, #3]
    lsrs r2, r4, #2
    ldrh r1, [r0, #4]
    ldr r5, .L0802FBB4 @ =0x000003FF
    ands r1, r5
    lsls r1, r1, #6
    orrs r1, r2
    lsls r1, r1, #0x10
    movs r7, #0x80
    lsls r7, r7, #0xd
    adds r1, r1, r7
.L0802FB92:
    lsrs r2, r1, #0x10
    movs r0, #0x3f
    ands r2, r0
    lsls r2, r2, #2
    movs r0, #3
    ands r0, r4
    orrs r0, r2
    strb r0, [r3, #3]
    lsrs r1, r1, #0x16
    ands r1, r5
    ldrh r2, [r3, #4]
    ldr r0, .L0802FBB8 @ =0xFFFFFC00
    ands r0, r2
    orrs r0, r1
    strh r0, [r3, #4]
    b .L0802FBDA
    .align 2, 0
.L0802FBB4: .4byte 0x000003FF
.L0802FBB8: .4byte 0xFFFFFC00
.L0802FBBC:
    ldr r0, [sp]
    lsls r1, r0, #6
    asrs r1, r1, #0x10
    subs r1, #0x10
    b .L0802FBCE
.L0802FBC6:
    ldr r0, [sp]
    lsls r1, r0, #6
    asrs r1, r1, #0x10
    adds r1, #0x10
.L0802FBCE:
    lsls r1, r1, #0x10
    lsrs r1, r1, #6
    ldr r2, .L0802FC0C @ =0xFC0003FF
    ands r2, r0
    orrs r2, r1
    str r2, [sp]
.L0802FBDA:
    mov r0, sp
    bl func_080A45A8
    cmp r0, #2
    beq .L0802FBE8
    bl .L08031F0E
.L0802FBE8:
    mov r1, sl
    ldr r0, [r1]
    ldr r2, [r0]
    movs r3, #0xa0
    lsls r3, r3, #1
    adds r2, r2, r3
    add r1, sp, #0x64
    ldr r3, .L0802FC10 @ =0x0000034F
    movs r4, #0
    str r3, [sp, #0x64]

    thumb_func_start sub_0802FBFC
sub_0802FBFC: @ 0x0802FBFC
    str r4, [r1, #4]
.L0802FBFE:
    ldr r3, [r2]
.L0802FC00:
    movs r2, #0
    bl _call_via_r3
    bl .L08031F0E
    .align 2, 0
.L0802FC0C: .4byte 0xFC0003FF
.L0802FC10: .4byte 0x0000034F
.L0802FC14:
    movs r1, #0
    mov r4, sl
    ldrh r0, [r4, #0x24]
    cmp r0, #0
    bne .L0802FC20
    movs r1, #1
.L0802FC20:
    adds r3, r1, #0
    cmp r3, #0
    bne .L0802FC2A
    bl sub_08030E0A
.L0802FC2A:
    mov r1, sl
    adds r1, #0x78
    movs r0, #0
    strb r0, [r1]
    movs r3, #0
    mov r5, sl
    ldrh r2, [r5, #4]
    str r1, [sp, #0x160]
    cmp r2, #0x33
    ble .L0802FC46
    ldr r0, .L0802FC94 @ =0x00000233
    cmp r2, r0
    bgt .L0802FC46
    movs r3, #1
.L0802FC46:
    adds r6, r3, #0
    mov r0, sl
    adds r0, #0x3f
    ldrb r1, [r0]
    adds r7, r0, #0
    cmp r1, #0
    blt .L0802FCBC
    cmp r1, #1
    ble .L0802FC60
    cmp r1, #4
    bgt .L0802FCBC
    cmp r1, #3
    blt .L0802FCBC
.L0802FC60:
    ldr r0, .L0802FC98 @ =gUnk_080F1170
    ldr r1, [r0]
    ldr r2, [r0, #4]
    mov r0, sl
    bl func_0802AC2C
    mov r0, sl
    adds r0, #0xb2
    ldrb r3, [r0]
    movs r1, #0
    adds r0, r3, #0
    subs r0, #0x2b
    cmp r0, #0x1a
    bhi .L0802FC7E
    movs r1, #1
.L0802FC7E:
    cmp r1, #0
    beq .L0802FC9C
    mov r1, sl
    ldr r0, [r1]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    adds r1, r3, #0
    bl _call_via_r2
    adds r4, r0, #0
    b .L0802FC9E
    .align 2, 0
.L0802FC94: .4byte 0x00000233
.L0802FC98: .4byte gUnk_080F1170
.L0802FC9C:
    movs r4, #0
.L0802FC9E:
    cmp r4, #0
    beq .L0802FCBC
    ldr r0, [r4, #0x14]
    ldr r1, [r0, #0x44]
    adds r0, r4, #0
    bl _call_via_r1
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802FCBC
    ldr r0, [r4, #0x14]
    ldr r1, [r0, #0x64]
    adds r0, r4, #0
    bl _call_via_r1
.L0802FCBC:
    mov r2, sl
    ldr r4, [r2, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    add r0, sp, #0x6c
    strb r1, [r0]
    bl GetId__C4Tool
    cmp r0, #0x15
    beq .L0802FCE4
    cmp r0, #0x2d
    beq .L0802FD44
    b .L0802FD9A
.L0802FCE4:
    mov r3, sl
    ldr r0, [r3, #0x34]
    movs r4, #0xd2
    lsls r4, r4, #6
    adds r0, r0, r4
    movs r1, #0x15
    bl func_0809C4B4
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802FD9A
    mov r0, sl
    ldr r5, [r0, #0x38]
    mov r4, sp
    adds r4, #0x6d
    adds r0, r4, #0
    movs r1, #0x16
    bl __4ToolUi
    ldrb r1, [r4]
    mov r0, sp
    movs r2, #1
    bl __9ToolStackG4ToolUi
    adds r5, #0x5c
    adds r0, r5, #0
    mov r1, sp
    movs r2, #2
    bl memcpy
    mov r1, sl
    ldr r0, [r1]
    ldr r2, [r0]
    movs r3, #0xa0
    lsls r3, r3, #1
    adds r2, r2, r3
    add r1, sp, #0x70
    ldr r3, .L0802FD40 @ =0x000003DE
    movs r4, #0
    str r3, [sp, #0x70]
    str r4, [r1, #4]
    ldr r3, [r2]
    movs r2, #0
    bl _call_via_r3
    b .L0802FD9A
    .align 2, 0
.L0802FD40: .4byte 0x000003DE
.L0802FD44:
    mov r4, sl
    ldr r0, [r4, #0x34]
    movs r5, #0xd2
    lsls r5, r5, #6
    adds r0, r0, r5
    movs r1, #0x2d
    bl func_0809C4B4
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0802FD9A
    ldr r5, [r4, #0x38]
    add r4, sp, #0x78
    adds r0, r4, #0
    movs r1, #0x2e
    bl __4ToolUi
    ldrb r1, [r4]
    mov r0, sp
    movs r2, #1
    bl __9ToolStackG4ToolUi
    adds r5, #0x5c
    adds r0, r5, #0
    mov r1, sp
    movs r2, #2
    bl memcpy
    mov r1, sl
    ldr r0, [r1]
    ldr r2, [r0]
    movs r3, #0xa0
    lsls r3, r3, #1
    adds r2, r2, r3
    add r1, sp, #0x7c
    ldr r3, .L0802FDB0 @ =0x000003DE
    movs r4, #0
    str r3, [sp, #0x7c]
    str r4, [r1, #4]
    ldr r3, [r2]
    movs r2, #0
    bl _call_via_r3
.L0802FD9A:
    ldrb r0, [r7]
    cmp r0, #0xd
    bls .L0802FDA4
    bl .L08031C7C
.L0802FDA4:
    lsls r0, r0, #2
    ldr r1, .L0802FDB4 @ =.L0802FDB8
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L0802FDB0: .4byte 0x000003DE
.L0802FDB4: .4byte .L0802FDB8
.L0802FDB8: @ jump table
    .4byte .L0802FDF0 @ case 0
    .4byte .L0802FECA @ case 1
    .4byte .L0802FFE8 @ case 2
    .4byte .L080300E2 @ case 3
    .4byte .L08030260 @ case 4
    .4byte .L0803058C @ case 5
    .4byte .L080309F0 @ case 6
    .4byte .L08030AA6 @ case 7
    .4byte .L0803080C @ case 8
    .4byte .L08030BD6 @ case 9
    .4byte .L08030C2A @ case 10
    .4byte .L08030D08 @ case 11
    .4byte .L08030DD4 @ case 12
    .4byte .L08030B5A @ case 13
.L0802FDF0:
    ldr r0, .L0802FE14 @ =gUnk_080F1230
    mov r4, sl
    str r0, [r4, #0x48]
    mov r1, sl
    adds r1, #0x4e
    movs r0, #1
    strb r0, [r1]
    mov r0, sl
    adds r0, #0x40
    ldrb r0, [r0]
    cmp r0, #6
    bhi .L0802FEC4
    lsls r0, r0, #2
    ldr r1, .L0802FE18 @ =.L0802FE1C
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L0802FE14: .4byte gUnk_080F1230
.L0802FE18: .4byte .L0802FE1C
.L0802FE1C: @ jump table
    .4byte .L0802FE38 @ case 0
    .4byte .L0802FE4C @ case 1
    .4byte .L0802FE60 @ case 2
    .4byte .L0802FE74 @ case 3
    .4byte .L0802FE88 @ case 4
    .4byte .L0802FE9C @ case 5
    .4byte .L0802FEB0 @ case 6
.L0802FE38:
    mov r0, sl
    adds r0, #0x4c
    movs r1, #3
    strb r1, [r0]
    adds r0, #1
    strb r1, [r0]
    mov r1, sl
    adds r1, #0x4f
    movs r0, #1
    b .L0802FEC2
.L0802FE4C:
    mov r1, sl
    adds r1, #0x4c
    movs r0, #2
    strb r0, [r1]
    adds r1, #1
    movs r0, #4
    strb r0, [r1]
    adds r1, #2
    movs r0, #1
    b .L0802FEC2
.L0802FE60:
    mov r1, sl
    adds r1, #0x4c
    movs r0, #0
    strb r0, [r1]
    adds r1, #1
    movs r0, #5
    strb r0, [r1]
    adds r1, #2
    movs r0, #1
    b .L0802FEC2
.L0802FE74:
    mov r1, sl
    adds r1, #0x4c
    movs r0, #0
    strb r0, [r1]
    adds r1, #1
    movs r0, #8
    strb r0, [r1]
    adds r1, #2
    movs r0, #1
    b .L0802FEC2
.L0802FE88:
    mov r1, sl
    adds r1, #0x4c
    movs r0, #0
    strb r0, [r1]
    adds r1, #1
    movs r0, #0x18
    strb r0, [r1]
    adds r1, #2
    movs r0, #2
    b .L0802FEC2
.L0802FE9C:
    mov r1, sl
    adds r1, #0x4c
    movs r0, #0
    strb r0, [r1]
    adds r1, #1
    movs r0, #0x50
    strb r0, [r1]
    adds r1, #2
    movs r0, #3
    b .L0802FEC2
.L0802FEB0:
    mov r1, sl
    adds r1, #0x4c
    movs r0, #0
    strb r0, [r1]
    adds r1, #1
    movs r0, #0xe0
    strb r0, [r1]
    adds r1, #2
    movs r0, #4
.L0802FEC2:
    strb r0, [r1]
.L0802FEC4:
    movs r0, #0x1c
    bl sub_08031A5E
.L0802FECA:
    mov r0, sl
    adds r0, #0x40
    ldrb r1, [r0]
    str r0, [sp, #0x140]
    cmp r1, #4
    bls .L0802FEF0
    mov r6, sl
    ldr r0, [r6]
    ldr r3, [r0]
    adds r3, #0xf0
    ldrh r1, [r6, #4]
    ldr r7, [sp, #0x140]
    ldrb r2, [r7]
    ldr r3, [r3]
    bl _call_via_r3
    ldr r1, [sp, #0x160]
    strb r0, [r1]
    b .L0802FFAA
.L0802FEF0:
    mov r2, sl
    movs r3, #0xa
    ldrsh r4, [r2, r3]
    movs r5, #0xe
    ldrsh r7, [r2, r5]
    mov r0, sl
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, #1
    beq .L0802FF18
    cmp r0, #1
    bgt .L0802FF0E
    cmp r0, #0
    beq .L0802FF1C
    b .L0802FF26
.L0802FF0E:
    cmp r0, #2
    beq .L0802FF20
    cmp r0, #3
    beq .L0802FF24
    b .L0802FF26
.L0802FF18:
    subs r7, #0x10
    b .L0802FF26
.L0802FF1C:
    adds r7, #0x10
    b .L0802FF26
.L0802FF20:
    subs r4, #0x10
    b .L0802FF26
.L0802FF24:
    adds r4, #0x10
.L0802FF26:
    mov r6, sl
    ldr r0, [r6]
    ldr r1, [r0]
    adds r1, #0xd0
    str r1, [sp, #0x100]
    ldrh r2, [r6, #4]
    mov r6, sp
    ldr r1, .L0802FFDC @ =0x000003FF
    mov sb, r1
    mov r3, sb
    ands r2, r3
    ldrh r3, [r6]
    ldr r5, .L0802FFE0 @ =0xFFFFFC00
    adds r1, r5, #0
    ands r1, r3
    orrs r1, r2
    strh r1, [r6]
    lsls r4, r4, #0x10
    lsrs r2, r4, #0x10
    movs r1, #0x3f
    mov ip, r1
    ands r2, r1
    lsls r2, r2, #2
    str r2, [sp, #0x184]
    ldrb r2, [r6, #1]
    mov r8, r2
    movs r3, #3
    adds r1, r3, #0
    mov r2, r8
    ands r1, r2
    ldr r2, [sp, #0x184]
    orrs r1, r2
    strb r1, [r6, #1]
    lsrs r4, r4, #0x16
    mov r1, sb
    ands r4, r1
    ldrh r2, [r6, #2]
    adds r1, r5, #0
    ands r1, r2
    orrs r1, r4
    strh r1, [r6, #2]
    lsls r2, r7, #0x10
    lsrs r1, r2, #0x10
    mov r4, ip
    ands r1, r4
    lsls r1, r1, #2
    ldrb r4, [r6, #3]
    ands r3, r4
    orrs r3, r1
    strb r3, [r6, #3]
    lsrs r2, r2, #0x16
    mov r7, sb
    ands r2, r7
    ldrh r1, [r6, #4]
    ands r5, r1
    orrs r5, r2
    strh r5, [r6, #4]
    ldr r1, [sp, #0x140]
    ldrb r2, [r1]
    ldr r4, [sp, #0x100]
    ldr r3, [r4]
    mov r1, sp
    bl _call_via_r3
    ldr r5, [sp, #0x160]
    strb r0, [r5]
.L0802FFAA:
    mov r6, sl
    ldr r0, [r6, #0x34]
    ldr r7, .L0802FFE4 @ =0x000034C4
    adds r0, r0, r7
    ldrb r0, [r0]
    cmp r0, #0
    bne .L0802FFD4
    ldr r0, [r6, #0x38]
    movs r1, #2
.L0802FFBC:
    bl func_0800EB2C
    adds r2, r0, #0
    ldr r1, [sp, #0x160]
    ldrb r0, [r1]
    movs r1, #0x32
    cmp r0, #0
    beq .L0802FFCE
    movs r1, #0x64
.L0802FFCE:
    adds r0, r2, #0
    bl func_0800EF88
.L0802FFD4:
    movs r0, #0
    bl .L08031F0A
    .align 2, 0
.L0802FFDC: .4byte 0x000003FF
.L0802FFE0: .4byte 0xFFFFFC00
.L0802FFE4: .4byte 0x000034C4
.L0802FFE8:
    mov r1, sl
    adds r1, #0x4a
    movs r0, #0
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    mov r0, sl
    adds r0, #0x40
    ldrb r0, [r0]
    cmp r0, #6
    bhi .L080300DC
    lsls r0, r0, #2
    ldr r1, .L08030008 @ =.L0803000C
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L08030008: .4byte .L0803000C
.L0803000C: @ jump table
    .4byte .L08030028 @ case 0
    .4byte .L0803003A @ case 1
    .4byte .L08030058 @ case 2
    .4byte .L08030072 @ case 3
    .4byte .L0803008E @ case 4
    .4byte .L080300A8 @ case 5
    .4byte .L080300C2 @ case 6
.L08030028:
    mov r0, sl
    adds r0, #0x48
    movs r2, #0
    movs r1, #1
    strb r1, [r0]
    adds r0, #1
    strb r2, [r0]
    adds r0, #3
    b .L08030086
.L0803003A:
    mov r0, sl
    adds r0, #0x48
    movs r1, #1
    strb r1, [r0]
    mov r2, sl
    adds r2, #0x49
    movs r0, #0xff
    strb r0, [r2]
    mov r0, sl
    adds r0, #0x4c
    strb r1, [r0]
    mov r1, sl
    adds r1, #0x4d
    movs r0, #3
    b .L080300DA
.L08030058:
    mov r1, sl
    adds r1, #0x48
    movs r0, #1
    strb r0, [r1]
    adds r1, #1
    movs r0, #0xff
    strb r0, [r1]
    adds r1, #3
    movs r0, #2
    strb r0, [r1]
    adds r1, #1
    movs r0, #3
    b .L080300DA
.L08030072:
    mov r1, sl
    adds r1, #0x48
    movs r0, #1
    strb r0, [r1]
    adds r1, #1
    movs r0, #0xff
    strb r0, [r1]
    mov r0, sl
    adds r0, #0x4c
    movs r1, #3
.L08030086:
    strb r1, [r0]
    adds r0, #1
    strb r1, [r0]
    b .L080300DC
.L0803008E:
    mov r1, sl
    adds r1, #0x48
    movs r0, #1
    strb r0, [r1]
    adds r1, #1
    movs r0, #0xfe
    strb r0, [r1]
    adds r1, #3
    movs r0, #3
    strb r0, [r1]
    adds r1, #1
    movs r0, #5
    b .L080300DA
.L080300A8:
    mov r1, sl
    adds r1, #0x48
    movs r0, #1
    strb r0, [r1]
    adds r1, #1
    movs r0, #0xfa
    strb r0, [r1]
    adds r1, #3
    movs r0, #6
    strb r0, [r1]
    adds r1, #1
    movs r0, #0xd
    b .L080300DA
.L080300C2:
    mov r1, sl
    adds r1, #0x48
    movs r0, #1
    strb r0, [r1]
    adds r1, #1
    movs r0, #0xf6
    strb r0, [r1]
    adds r1, #3
    movs r0, #0xc
    strb r0, [r1]
    adds r1, #1
    movs r0, #0x15
.L080300DA:
    strb r0, [r1]
.L080300DC:
    movs r0, #0x1a
    bl .L08031E84
.L080300E2:
    cmp r6, #0
    beq .L080301C8
    mov r5, sl
    movs r6, #0xa
    ldrsh r4, [r5, r6]
    movs r0, #0xe
    ldrsh r7, [r5, r0]
    mov r0, sl
    adds r0, #0x20
    ldrb r0, [r0]
    adds r1, r0, #0
    cmp r0, #1
    beq .L08030110
    cmp r0, #1
    bgt .L08030106
    cmp r0, #0
    beq .L08030114
    b .L0803011E
.L08030106:
    cmp r1, #2
    beq .L08030118
    cmp r1, #3
    beq .L0803011C
    b .L0803011E
.L08030110:
    subs r7, #0x10
    b .L0803011E
.L08030114:
    adds r7, #0x10
    b .L0803011E
.L08030118:
    subs r4, #0x10
    b .L0803011E
.L0803011C:
    adds r4, #0x10
.L0803011E:
    mov r1, sl
    ldr r0, [r1]
    ldr r1, [r0]
    adds r1, #0xc8
    str r1, [sp, #0x104]
    mov r3, sl
    ldrh r2, [r3, #4]
    mov r6, sp
    ldr r5, .L080301BC @ =0x000003FF
    mov sb, r5
    mov r1, sb
    ands r2, r1
    ldrh r3, [r6]
    ldr r5, .L080301C0 @ =0xFFFFFC00
    adds r1, r5, #0
    ands r1, r3
    orrs r1, r2
    strh r1, [r6]
    lsls r4, r4, #0x10
    lsrs r2, r4, #0x10
    movs r3, #0x3f
    mov ip, r3
    ands r2, r3
    lsls r2, r2, #2
    str r2, [sp, #0x184]
    ldrb r1, [r6, #1]
    mov r8, r1
    movs r3, #3
    adds r1, r3, #0
    mov r2, r8
    ands r1, r2
    ldr r2, [sp, #0x184]
    orrs r1, r2
    strb r1, [r6, #1]
    lsrs r4, r4, #0x16
    mov r1, sb
    ands r4, r1
    ldrh r2, [r6, #2]
    adds r1, r5, #0
    ands r1, r2
    orrs r1, r4
    strh r1, [r6, #2]
    lsls r2, r7, #0x10
    lsrs r1, r2, #0x10
    mov r4, ip
    ands r1, r4
    lsls r1, r1, #2
    ldrb r4, [r6, #3]
    ands r3, r4
    orrs r3, r1
    strb r3, [r6, #3]
    lsrs r2, r2, #0x16
    mov r7, sb
    ands r2, r7
    ldrh r1, [r6, #4]
    ands r5, r1
    orrs r5, r2
    strh r5, [r6, #4]
    add r4, sp, #0x84
    ldr r1, [sp, #0x104]
    ldr r3, [r1]
    mov r1, sp
    adds r2, r4, #0
    bl _call_via_r3
    ldr r2, [sp, #0x160]
    strb r0, [r2]
    mov r3, sl
    ldr r0, [r3, #0x34]
    ldr r5, .L080301C4 @ =0x000034C4
    adds r0, r0, r5
    ldrb r0, [r0]
    cmp r0, #0
    beq .L080301B4
    b .L0803034C
.L080301B4:
    ldr r0, [r3, #0x38]
    movs r1, #0
    b .L08030334
    .align 2, 0
.L080301BC: .4byte 0x000003FF
.L080301C0: .4byte 0xFFFFFC00
.L080301C4: .4byte 0x000034C4
.L080301C8:
    mov r0, sl
    adds r0, #0x48
    strb r6, [r0]
    adds r0, #8
    movs r4, #0
    strh r6, [r0]
    adds r0, #2
    strb r4, [r0]
    mov r7, sl
    ldr r0, [r7]
    ldr r1, [r0]
    adds r1, #0xf4
    ldr r1, [r1]
    bl _call_via_r1
    ldr r1, .L08030218 @ =gUnk_080F1178
    mov r2, sl
    adds r2, #0x40
    ldrb r0, [r2]
    adds r0, r0, r1
    ldrb r0, [r0]
    mov r1, sl
    adds r1, #0x49
    strb r0, [r1]
    ldrb r0, [r2]
    cmp r0, #4
    bhi .L0803021C
    mov r0, sl
    adds r0, #0x4a
    strb r4, [r0]
    adds r0, #1
    strb r4, [r0]
    adds r0, #1
    movs r1, #1
    strb r1, [r0]
    adds r0, #1
    strb r4, [r0]
    adds r0, #1
    strb r4, [r0]
    b .L0803025A
    .align 2, 0
.L08030218: .4byte gUnk_080F1178
.L0803021C:
    cmp r0, #5
    bhi .L0803023E
    mov r0, sl
    adds r0, #0x4a
    strb r4, [r0]
    mov r1, sl
    adds r1, #0x4b
    movs r0, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    mov r0, sl
    adds r0, #0x4e
    strb r4, [r0]
    b .L0803025A
.L0803023E:
    cmp r0, #6
    bhi .L0803025A
    mov r1, sl
    adds r1, #0x4a
    movs r0, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
.L0803025A:
    movs r0, #0x19
    bl .L08031C7E
.L08030260:
    cmp r6, #0
    bne .L08030266
    b .L08030364
.L08030266:
    mov r2, sl
    movs r3, #0xa
    ldrsh r4, [r2, r3]
    movs r5, #0xe
    ldrsh r7, [r2, r5]
    mov r0, sl
    adds r0, #0x20
    ldrb r0, [r0]
    adds r1, r0, #0
    cmp r0, #1
    beq .L08030290
    cmp r0, #1
    bgt .L08030286
    cmp r0, #0
    beq .L08030294
    b .L0803029E
.L08030286:
    cmp r1, #2
    beq .L08030298
    cmp r1, #3
    beq .L0803029C
    b .L0803029E
.L08030290:
    subs r7, #0x10
    b .L0803029E
.L08030294:
    adds r7, #0x10
    b .L0803029E
.L08030298:
    subs r4, #0x10
    b .L0803029E
.L0803029C:
    adds r4, #0x10
.L0803029E:
    mov r6, sl
    ldr r0, [r6]
    ldr r1, [r0]
    adds r1, #0xcc
    str r1, [sp, #0x108]
    ldrh r2, [r6, #4]
    mov r6, sp
    ldr r1, .L08030358 @ =0x000003FF
    mov sb, r1
    mov r3, sb
    ands r2, r3
    ldrh r3, [r6]
    ldr r5, .L0803035C @ =0xFFFFFC00
    adds r1, r5, #0
    ands r1, r3
    orrs r1, r2
    strh r1, [r6]
    lsls r4, r4, #0x10
    lsrs r2, r4, #0x10
    movs r1, #0x3f
    mov ip, r1
    ands r2, r1
    lsls r2, r2, #2
    str r2, [sp, #0x184]
    ldrb r2, [r6, #1]
    mov r8, r2
    movs r3, #3
    adds r1, r3, #0
    mov r2, r8
    ands r1, r2
    ldr r2, [sp, #0x184]
    orrs r1, r2
    strb r1, [r6, #1]
    lsrs r4, r4, #0x16
    mov r1, sb
    ands r4, r1
    ldrh r2, [r6, #2]
    adds r1, r5, #0
    ands r1, r2
    orrs r1, r4
    strh r1, [r6, #2]
    lsls r2, r7, #0x10
    lsrs r1, r2, #0x10
    mov r4, ip
    ands r1, r4
    lsls r1, r1, #2
    ldrb r4, [r6, #3]
    ands r3, r4
    orrs r3, r1
    strb r3, [r6, #3]
    lsrs r2, r2, #0x16
    mov r7, sb
    ands r2, r7
    ldrh r1, [r6, #4]
    ands r5, r1
    orrs r5, r2
    strh r5, [r6, #4]
    add r4, sp, #0x88
    ldr r1, [sp, #0x108]
    ldr r3, [r1]
    mov r1, sp
    adds r2, r4, #0
    bl _call_via_r3
    ldr r2, [sp, #0x160]
    strb r0, [r2]
    mov r3, sl
    ldr r0, [r3, #0x34]
    ldr r5, .L08030360 @ =0x000034C4
    adds r0, r0, r5
    ldrb r0, [r0]
    cmp r0, #0
    bne .L0803034C
    ldr r0, [r3, #0x38]
    movs r1, #3
.L08030334:
    bl func_0800EB2C
    adds r2, r0, #0
    ldr r6, [sp, #0x160]
    ldrb r0, [r6]
    movs r1, #0x32
    cmp r0, #0
    beq .L08030346
    movs r1, #0x64
.L08030346:
    adds r0, r2, #0
    bl func_0800EF88
.L0803034C:
    ldr r1, [r4]
    mov r0, sl
    bl func_08025B64
    bl .L08031F0E
    .align 2, 0
.L08030358: .4byte 0x000003FF
.L0803035C: .4byte 0xFFFFFC00
.L08030360: .4byte 0x000034C4
.L08030364:
    mov r0, sl
    adds r0, #0x40
    ldrb r1, [r0]
    str r0, [sp, #0x140]
    cmp r1, #6
    bne .L08030382
    mov r7, sl
    ldr r0, [r7]
    ldr r3, [r0]
    adds r3, #0xec
    ldrh r1, [r7, #4]
    ldr r4, [sp, #0x140]
    ldrb r2, [r4]
    ldr r3, [r3]
    b .L08030560
.L08030382:
    cmp r1, #5
    beq .L08030388
    b .L080304AC
.L08030388:
    movs r1, #0
    mov r6, sl
    adds r6, #0x20
    str r6, [sp, #0x138]
    add r7, sp, #0x8c
    mov r8, r7
    mov r7, sp
    mov sb, r8
.L08030398:
    movs r0, #0
    subs r2, r1, #4
    str r2, [sp, #0x12c]
    adds r1, #1
    str r1, [sp, #0x130]
    str r2, [sp, #0x10c]
.L080303A4:
    adds r0, #1
    mov r3, r8
    str r0, [r3]
    ldr r4, [sp, #0x10c]
    mov r5, sb
    str r4, [r5, #4]
    ldr r6, [sp, #0x138]
    ldrb r1, [r6]
    str r0, [sp, #0x134]
    cmp r1, #1
    beq .L080303D8
    cmp r1, #1
    bgt .L080303C4
    cmp r1, #0
    beq .L080303CA
    b .L080303F4
.L080303C4:
    cmp r1, #2
    beq .L080303EA
    b .L080303F4
.L080303CA:
    mov r0, r8
    ldr r1, [r0]
    ldr r2, [sp, #0x10c]
    mov r3, sb
    str r2, [r3]
    str r1, [r3, #4]
    b .L080303F4
.L080303D8:
    mov r4, r8
    ldr r1, [r4]
    ldr r5, [sp, #0x12c]
    rsbs r0, r5, #0
    mov r6, sb
    str r0, [r6]
    rsbs r0, r1, #0
    str r0, [r6, #4]
    b .L080303F4
.L080303EA:
    mov r1, r8
    ldr r0, [r1]
    rsbs r0, r0, #0
    mov r2, sb
    str r0, [r2]
.L080303F4:
    mov r3, sl
    ldr r0, [r3]
    ldr r6, [r0]
    adds r6, #0xc4
    mov ip, r6
    ldrh r4, [r3, #4]
    movs r5, #0xa
    ldrsh r1, [r3, r5]
    mov r6, r8
    ldr r3, [r6]
    mov r5, sl
    movs r6, #0xe
    ldrsh r2, [r5, r6]
    mov r6, sb
    ldr r5, [r6, #4]
    lsls r3, r3, #4
    adds r3, r3, r1
    lsls r5, r5, #4
    adds r5, r5, r2
    ldr r1, .L080304A4 @ =0x000003FF
    ands r4, r1
    ldrh r2, [r7]
    ldr r1, .L080304A8 @ =0xFFFFFC00
    ands r1, r2
    orrs r1, r4
    strh r1, [r7]
    lsls r3, r3, #0x10
    lsrs r2, r3, #0x10
    movs r4, #0x3f
    ands r2, r4
    lsls r2, r2, #2
    ldrb r4, [r7, #1]
    movs r1, #3
    ands r1, r4
    orrs r1, r2
    strb r1, [r7, #1]
    lsrs r3, r3, #0x16
    ldr r6, .L080304A4 @ =0x000003FF
    ands r3, r6
    ldrh r2, [r7, #2]
    ldr r1, .L080304A8 @ =0xFFFFFC00
    ands r1, r2
    orrs r1, r3
    strh r1, [r7, #2]
    lsls r5, r5, #0x10
    lsrs r2, r5, #0x10
    movs r1, #0x3f
    ands r2, r1
    lsls r2, r2, #2
    ldrb r3, [r7, #3]
    movs r1, #3
    ands r1, r3
    orrs r1, r2
    strb r1, [r7, #3]
    lsrs r5, r5, #0x16
    ands r5, r6
    ldrh r2, [r7, #4]
    ldr r1, .L080304A8 @ =0xFFFFFC00
    ands r1, r2
    orrs r1, r5
    strh r1, [r7, #4]
    ldr r3, [sp, #0x140]
    ldrb r2, [r3]
    mov r4, ip
    ldr r3, [r4]
    mov r1, sp
    bl _call_via_r3
    ldr r5, [sp, #0x160]
    ldrb r1, [r5]
    rsbs r2, r1, #0
    orrs r2, r1
    lsls r0, r0, #0x18
    lsrs r0, r0, #0x18
    rsbs r1, r0, #0
    orrs r1, r0
    orrs r1, r2
    lsrs r1, r1, #0x1f
    strb r1, [r5]
    ldr r0, [sp, #0x134]
    cmp r0, #8
    ble .L080303A4
    ldr r1, [sp, #0x130]
    cmp r1, #8
    bgt .L080304A0
    b .L08030398
.L080304A0:
    b .L08030568
    .align 2, 0
.L080304A4: .4byte 0x000003FF
.L080304A8: .4byte 0xFFFFFC00
.L080304AC:
    mov r6, sl
    movs r7, #0xa
    ldrsh r4, [r6, r7]
    movs r0, #0xe
    ldrsh r7, [r6, r0]
    mov r0, sl
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, #1
    beq .L080304D4
    cmp r0, #1
    bgt .L080304CA
    cmp r0, #0
    beq .L080304D8
    b .L080304E2
.L080304CA:
    cmp r0, #2
    beq .L080304DC
    cmp r0, #3
    beq .L080304E0
    b .L080304E2
.L080304D4:
    subs r7, #0x10
    b .L080304E2
.L080304D8:
    adds r7, #0x10
    b .L080304E2
.L080304DC:
    subs r4, #0x10
    b .L080304E2
.L080304E0:
    adds r4, #0x10
.L080304E2:
    mov r1, sl
    ldr r0, [r1]
    ldr r1, [r0]
    adds r1, #0xc4
    str r1, [sp, #0x110]
    mov r3, sl
    ldrh r2, [r3, #4]
    mov r6, sp
    ldr r5, .L08030580 @ =0x000003FF
    mov sb, r5
    mov r1, sb
    ands r2, r1
    ldrh r3, [r6]
    ldr r5, .L08030584 @ =0xFFFFFC00
    adds r1, r5, #0
    ands r1, r3
    orrs r1, r2
    strh r1, [r6]
    lsls r4, r4, #0x10
    lsrs r2, r4, #0x10
    movs r3, #0x3f
    mov ip, r3
    ands r2, r3
    lsls r2, r2, #2
    str r2, [sp, #0x184]
    ldrb r1, [r6, #1]
    mov r8, r1
    movs r3, #3
    adds r1, r3, #0
    mov r2, r8
    ands r1, r2
    ldr r2, [sp, #0x184]
    orrs r1, r2
    strb r1, [r6, #1]
    lsrs r4, r4, #0x16
    mov r1, sb
    ands r4, r1
    ldrh r2, [r6, #2]
    adds r1, r5, #0
    ands r1, r2
    orrs r1, r4
    strh r1, [r6, #2]
    lsls r2, r7, #0x10
    lsrs r1, r2, #0x10
    mov r4, ip
    ands r1, r4
    lsls r1, r1, #2
    ldrb r4, [r6, #3]
    ands r3, r4
    orrs r3, r1
    strb r3, [r6, #3]
    lsrs r2, r2, #0x16
    mov r7, sb
    ands r2, r7
    ldrh r1, [r6, #4]
    ands r5, r1
    orrs r5, r2
    strh r5, [r6, #4]
    ldr r1, [sp, #0x140]
    ldrb r2, [r1]
    ldr r4, [sp, #0x110]
    ldr r3, [r4]
    mov r1, sp
.L08030560:
    bl _call_via_r3
    ldr r5, [sp, #0x160]
    strb r0, [r5]
.L08030568:
    mov r6, sl
    ldr r0, [r6, #0x34]
    ldr r7, .L08030588 @ =0x000034C4
    adds r0, r0, r7
    ldrb r0, [r0]
    cmp r0, #0
    beq .L08030578
    b .L0802FFD4
.L08030578:
    ldr r0, [r6, #0x38]
    movs r1, #3
    b .L0802FFBC
    .align 2, 0
.L08030580: .4byte 0x000003FF
.L08030584: .4byte 0xFFFFFC00
.L08030588: .4byte 0x000034C4
.L0803058C:
    mov r2, sl
    adds r2, #0x88
    ldr r0, [r2, #4]
    ldr r1, [r2, #0xc]
    adds r0, r0, r1
    str r0, [r2, #4]
    ldr r0, [r2, #8]
    ldr r1, [r2, #0x10]
    adds r0, r0, r1
    str r0, [r2, #8]
    ldr r0, [r2, #0x18]
    ldr r3, .L080305B4 @ =0xFFFFC7D7
    adds r0, r0, r3
    str r0, [r2, #0x18]
    ldr r1, [r2, #0x14]
    adds r1, r1, r0
    cmp r1, #0
    ble .L080305B8
    str r1, [r2, #0x14]
    b .L080305BE
    .align 2, 0
.L080305B4: .4byte 0xFFFFC7D7
.L080305B8:
    movs r0, #0
    str r0, [r2, #0x14]
    str r0, [r2, #0x18]
.L080305BE:
    mov r4, sl
    ldrh r4, [r4, #4]
    str r4, [sp, #0x114]
    mov r5, sl
    movs r6, #0xa
    ldrsh r5, [r5, r6]
    mov r8, r5
    mov r7, sl
    movs r0, #0xe
    ldrsh r6, [r7, r0]
    mov r0, sl
    adds r0, #0x20
    ldrb r0, [r0]
    adds r1, r0, #0
    cmp r0, #1
    beq .L080305F6
    cmp r0, #1
    bgt .L080305E8
    cmp r0, #0
    beq .L080305F2
    b .L08030606
.L080305E8:
    cmp r1, #2
    beq .L080305FA
    cmp r1, #3
    beq .L08030602
    b .L08030606
.L080305F2:
    adds r6, #0x30
    b .L08030606
.L080305F6:
    subs r6, #0x30
    b .L08030606
.L080305FA:
    movs r1, #0x30
    rsbs r1, r1, #0
    add r8, r1
    b .L08030606
.L08030602:
    movs r2, #0x30
    add r8, r2
.L08030606:
    mov r3, sl
    ldr r3, [r3]
    mov sb, r3
    ldr r1, [r3]
    mov r0, sp
    ldr r3, [r1, #0x34]
    mov r1, sb
    ldr r2, [sp, #0x114]
    bl _call_via_r3
    mov r1, sp
    mov r5, r8
    asrs r4, r5, #3
    asrs r3, r6, #3
    ldr r2, [sp]
    ldrh r0, [r1, #8]
    cmp r2, #0
    beq .L0803064C
    cmp r4, r0
    bhi .L0803064C
    ldrh r1, [r1, #0xa]
    cmp r3, r1
    bhi .L0803064C
    ldr r1, [sp, #4]
    muls r0, r3, r0
    adds r0, r0, r4
    cmp r1, #0
    beq .L08030642
    adds r0, r1, r0
    ldrb r0, [r0]
.L08030642:
    lsls r0, r0, #2
    adds r0, r2, r0
    b .L0803064E
.L08030648:
    ldr r0, [r4]
    b .L080306B0
.L0803064C:
    movs r0, #0
.L0803064E:
    cmp r0, #0
    bne .L08030654
    b .L080307D8
.L08030654:
    ldr r0, [r0]
    lsls r0, r0, #0x1e
    cmp r0, #0
    blt .L0803065E
    b .L080307D8
.L0803065E:
    mov r7, sb
    ldr r0, [r7]
    movs r1, #0xa6
    lsls r1, r1, #1
    adds r0, r0, r1
    ldr r1, [r0]
    mov r0, sb
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    mov r2, sl
    adds r2, #0x54
    str r2, [sp, #0x158]
    add r7, sp, #0x10
    mov r3, r8
    lsls r3, r3, #0x10
    str r3, [sp, #0x16c]
    lsls r6, r6, #0x10
    str r6, [sp, #0x170]
    mov r6, sp
    adds r6, #0x94
    str r6, [sp, #0x164]
    add r0, sp, #0x18
    mov r8, r0
    mov r1, sl
    adds r1, #0x60
    str r1, [sp, #0x15c]
    cmp r4, r5
    beq .L080306AC
.L0803069A:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08030648
    adds r4, #4
    cmp r4, r5
    bne .L0803069A
.L080306AC:
    subs r0, r5, #4
    ldr r0, [r0]
.L080306B0:
    movs r1, #0x6d
    bl func_08008B6C
    movs r6, #0
    ldr r2, [sp, #0x158]
    strh r6, [r2]
    ldr r3, .L080307B0 @ =0x000003FF
    adds r4, r3, #0
    ldr r5, [sp, #0x114]
    ands r5, r4
    ldrh r1, [r7]
    ldr r3, .L080307B4 @ =0xFFFFFC00
    adds r0, r3, #0
    ands r0, r1
    orrs r0, r5
    strh r0, [r7]
    ldr r0, [sp, #0x16c]
    lsrs r1, r0, #0x10
    movs r5, #0x3f
    ands r1, r5
    lsls r1, r1, #2
    ldrb r2, [r7, #1]
    movs r0, #3
    ands r0, r2
    orrs r0, r1
    strb r0, [r7, #1]
    ldr r2, [sp, #0x16c]
    lsrs r1, r2, #0x16
    ands r1, r4
    ldrh r2, [r7, #2]
    adds r0, r3, #0
    ands r0, r2
    orrs r0, r1
    strh r0, [r7, #2]
    ldr r0, [sp, #0x170]
    lsrs r1, r0, #0x10
    ands r1, r5
    lsls r1, r1, #2
    ldrb r2, [r7, #3]
    movs r0, #3
    ands r0, r2
    orrs r0, r1
    strb r0, [r7, #3]
    ldr r1, [sp, #0x170]
    lsrs r0, r1, #0x16
    ands r0, r4
    ldrh r1, [r7, #4]
    ands r3, r1
    orrs r3, r0
    strh r3, [r7, #4]
    mov r2, sb
    ldr r0, [r2]
    movs r3, #0xa2
    lsls r3, r3, #1
    adds r0, r0, r3
    ldr r1, [r0]
    mov r0, sb
    bl _call_via_r1
    adds r2, r0, #0
    adds r2, #0xa
    ldr r0, [sp, #0x164]
    adds r1, r7, #0
    bl func_080A3E90
    add r0, sp, #0x94
    ldr r1, [r0, #4]
    ldr r0, [r0]
    mov r4, sl
    str r0, [r4, #0x58]
    str r1, [r4, #0x5c]
    ldr r0, .L080307B8 @ =gUnk_080F0E80
    str r0, [sp, #0x18]
    mov r5, r8
    strh r6, [r5, #4]
    add r0, sp, #0x20
    bl __12RucksackItem
    strb r6, [r5, #0xc]
    strb r6, [r5, #0xd]
    movs r0, #0x3b
    str r0, [r5, #0x10]
    strb r6, [r5, #0x14]
    ldr r1, [sp, #0x15c]
    mov r0, r8
    ldm r0!, {r2, r3, r7}
    stm r1!, {r2, r3, r7}
    ldm r0!, {r4, r5, r6}
    stm r1!, {r4, r5, r6}
    mov r7, sl
    ldr r0, [r7, #0x5c]
    cmp r0, #0
    bne .L080307C4
    ldr r0, [r7, #0x34]
    ldr r1, .L080307BC @ =0x0000218B
    adds r0, r0, r1
    ldrb r0, [r0]
    movs r4, #3
    ands r4, r0
    cmp r4, #0
    bne .L080307C4
    bl rand
    asrs r0, r0, #4
    movs r1, #0xff
    ands r0, r1
    cmp r0, #1
    bgt .L080307C4
    mov r3, sb
    ldr r2, [r3]
    movs r5, #0xa0
    lsls r5, r5, #1
    adds r2, r2, r5
    add r1, sp, #0x9c
    ldr r0, .L080307C0 @ =0x00000331
    str r0, [r1]
    str r4, [r1, #4]
    ldr r3, [r2]
    mov r0, sb
    movs r2, #0
    bl _call_via_r3
    movs r7, #0
    ldr r6, [sp, #0x13c]
    strb r7, [r6]
    bl .L08031D78
    .align 2, 0
.L080307B0: .4byte 0x000003FF
.L080307B4: .4byte 0xFFFFFC00
.L080307B8: .4byte gUnk_080F0E80
.L080307BC: .4byte 0x0000218B
.L080307C0: .4byte 0x00000331
.L080307C4:
    movs r0, #0x1e
    ldr r1, [sp, #0x13c]
    strb r0, [r1]
    mov r0, sl
    movs r1, #0xa0
    movs r2, #0
    bl func_0802A4C0
    bl .L08031F0E
.L080307D8:
    add r4, sp, #0x10
    ldr r0, .L08030808 @ =gUnk_080F0E80
    str r0, [sp, #0x10]
    movs r5, #0
    movs r0, #0
    strh r0, [r4, #4]
    add r0, sp, #0x18
    bl __12RucksackItem
    strb r5, [r4, #0xc]
    strb r5, [r4, #0xd]
    movs r0, #0x3b
    str r0, [r4, #0x10]
    strb r5, [r4, #0x14]
    mov r0, sl
    adds r0, #0x60
    ldm r4!, {r2, r3, r5}
    stm r0!, {r2, r3, r5}
    ldm r4!, {r1, r6, r7}
    stm r0!, {r1, r6, r7}
    movs r0, #0x20
    ldr r2, [sp, #0x13c]
    strb r0, [r2]
    b .L08030F4C
    .align 2, 0
.L08030808: .4byte gUnk_080F0E80
.L0803080C:
    mov r3, sl
    ldr r4, [r3, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    add r0, sp, #0xa4
    strb r1, [r0]
    bl GetId__C4Tool
    cmp r0, #0x44
    bne .L08030908
    movs r0, #0
    mov r4, sp
    adds r4, #0xa5
    str r4, [sp, #0x168]
    mov r7, sp
.L08030838:
    movs r5, #0
    mov sb, r5
    lsls r6, r0, #4
    str r6, [sp, #0x178]
    adds r0, #1
    str r0, [sp, #0x174]
.L08030844:
    mov r0, sl
    ldr r4, [r0, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    ldr r1, [sp, #0x168]
    strb r0, [r1]
    mov r0, sp
    adds r0, #0xa5
    bl GetId__C4Tool
    bl func_080245E0
    adds r2, r0, #0
    mov r3, sl
    ldr r0, [r3]
    ldr r4, [r0]
    mov r8, r4
    movs r5, #0xdc
    add r8, r5
    ldrh r5, [r3, #4]
    movs r6, #0xa
    ldrsh r1, [r3, r6]
    movs r4, #0xe
    ldrsh r6, [r3, r4]
    mov r3, sb
    lsls r4, r3, #4
    adds r4, r4, r1
    subs r4, #0x10
    ldr r1, [sp, #0x178]
    adds r6, r1, r6
    subs r6, #0x10
    ldr r3, .L08030900 @ =0x000003FF
    ands r5, r3
    ldrh r3, [r7]
    ldr r1, .L08030904 @ =0xFFFFFC00
    ands r1, r3
    orrs r1, r5
    strh r1, [r7]
    lsls r4, r4, #0x10
    lsrs r3, r4, #0x10
    movs r5, #0x3f
    ands r3, r5
    lsls r3, r3, #2
    ldrb r5, [r7, #1]
    movs r1, #3
    ands r1, r5
    orrs r1, r3
    strb r1, [r7, #1]
    lsrs r4, r4, #0x16
    ldr r1, .L08030900 @ =0x000003FF
    ands r4, r1
    ldrh r3, [r7, #2]
    ldr r1, .L08030904 @ =0xFFFFFC00
    ands r1, r3
    orrs r1, r4
    strh r1, [r7, #2]
    lsls r6, r6, #0x10
    lsrs r3, r6, #0x10
    movs r4, #0x3f
    ands r3, r4
    lsls r3, r3, #2
    ldrb r4, [r7, #3]
    movs r1, #3
    ands r1, r4
    orrs r1, r3
    strb r1, [r7, #3]
    lsrs r6, r6, #0x16
    ldr r5, .L08030900 @ =0x000003FF
    ands r6, r5
    ldrh r3, [r7, #4]
    ldr r1, .L08030904 @ =0xFFFFFC00
    ands r1, r3
    orrs r1, r6
    strh r1, [r7, #4]
    mov r6, r8
    ldr r3, [r6]
    mov r1, sp
    bl _call_via_r3
    movs r0, #1
    add sb, r0
    mov r1, sb
    cmp r1, #2
    ble .L08030844
    ldr r0, [sp, #0x174]
    cmp r0, #2
    ble .L08030838
    b .L080309D6
    .align 2, 0
.L08030900: .4byte 0x000003FF
.L08030904: .4byte 0xFFFFFC00
.L08030908:
    mov r2, sl
    ldr r4, [r2, #0x38]
    adds r0, r4, #0
    adds r0, #0x5c
    bl GetTool__C9ToolStack
    adds r1, r0, #0
    adds r0, r4, #0
    bl func_0800E550
    adds r1, r0, #0
    mov r0, sp
    adds r0, #0xa6
    strb r1, [r0]
    bl GetId__C4Tool
    bl func_080245E0
    str r0, [sp, #0x118]
    movs r0, #0
    mov r7, sp
    ldr r3, .L080309E8 @ =0x000003FF
    mov sb, r3
.L08030936:
    movs r4, #0
    mov r8, r4
    lsls r5, r0, #4
    str r5, [sp, #0x180]
    adds r0, #1
    str r0, [sp, #0x17c]
.L08030942:
    mov r6, sl
    ldr r0, [r6]
    ldr r6, [r0]
    adds r6, #0xdc
    mov r1, sl
    ldrh r4, [r1, #4]
    movs r2, #0xa
    ldrsh r1, [r1, r2]
    mov r3, sl
    movs r2, #0xe
    ldrsh r5, [r3, r2]
    mov r2, r8
    lsls r3, r2, #4
    adds r3, r3, r1
    subs r3, #0x10
    ldr r1, [sp, #0x180]
    adds r5, r1, r5
    subs r5, #0x10
    mov r2, sb
    ands r4, r2
    ldrh r2, [r7]
    ldr r1, .L080309EC @ =0xFFFFFC00
    ands r1, r2
    orrs r1, r4
    strh r1, [r7]
    lsls r3, r3, #0x10
    lsrs r2, r3, #0x10
    movs r4, #0x3f
    ands r2, r4
    lsls r2, r2, #2
    ldrb r4, [r7, #1]
    movs r1, #3
    ands r1, r4
    orrs r1, r2
    strb r1, [r7, #1]
    lsrs r3, r3, #0x16
    mov r1, sb
    ands r3, r1
    ldrh r2, [r7, #2]
    ldr r1, .L080309EC @ =0xFFFFFC00
    ands r1, r2
    orrs r1, r3
    strh r1, [r7, #2]
    lsls r5, r5, #0x10
    lsrs r2, r5, #0x10
    movs r3, #0x3f
    ands r2, r3
    lsls r2, r2, #2
    ldrb r3, [r7, #3]
    movs r1, #3
    ands r1, r3
    orrs r1, r2
    strb r1, [r7, #3]
    lsrs r5, r5, #0x16
    mov r4, sb
    ands r5, r4
    ldrh r2, [r7, #4]
    ldr r1, .L080309EC @ =0xFFFFFC00
    ands r1, r2
    orrs r1, r5
    strh r1, [r7, #4]
    ldr r3, [r6]
    mov r1, sp
    ldr r2, [sp, #0x118]
    bl _call_via_r3
    movs r5, #1
    add r8, r5
    mov r6, r8
    cmp r6, #2
    ble .L08030942
    ldr r0, [sp, #0x17c]
    cmp r0, #2
    ble .L08030936
.L080309D6:
    mov r7, sl
    ldr r0, [r7, #0x38]
    adds r0, #0x5c
    movs r1, #1
    bl SubtractAmount__9ToolStackUi
    bl .L08031C7C
    .align 2, 0
.L080309E8: .4byte 0x000003FF
.L080309EC: .4byte 0xFFFFFC00
.L080309F0:
    movs r2, #3
    mov r8, r2
    mov r1, sl
    adds r1, #0xb2
    ldrb r3, [r1]
    movs r2, #0
    adds r0, r3, #0
    subs r0, #0x2b
    adds r4, r1, #0
    cmp r0, #0x1a
    bhi .L08030A08
    movs r2, #1
.L08030A08:
    cmp r2, #0
    beq .L08030A1E
    mov r5, sl
    ldr r0, [r5]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    adds r1, r3, #0
    bl _call_via_r2
    adds r5, r0, #0
    b .L08030A20
.L08030A1E:
    movs r5, #0
.L08030A20:
    cmp r5, #0
    beq .L08030A74
    ldr r0, [r5, #0x14]
    ldr r1, [r0, #0x48]
    adds r0, r5, #0
    bl _call_via_r1
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08030A74
    mov r6, sl
    ldr r0, [r6, #0x34]
    movs r7, #0xbe
    lsls r7, r7, #3
    adds r0, r0, r7
    bl GetFreePregnancyStall__C4Barn
    adds r6, r0, #0
    cmp r6, #0
    blt .L08030A74
    ldrb r0, [r4]
    adds r4, r0, #0
    subs r4, #0x36
    cmp r4, #0xf
    bhi .L08030A74
    ldr r0, [r5, #0x14]
    ldr r1, [r0, #0x68]
    adds r0, r5, #0
    bl _call_via_r1
    mov r1, sl
    ldr r0, [r1, #0x34]
    adds r0, r0, r7
    adds r1, r6, #0
    adds r2, r4, #0
    bl MoveToPregnancyStall__4BarnUiUi
    mov r0, sl
    adds r0, #0xb1
    strb r4, [r0]
    movs r2, #1
    mov r8, r2
.L08030A74:
    mov r3, sl
    ldr r0, [r3]
    ldr r2, [r0]
    movs r4, #0xa0
    lsls r4, r4, #1
    adds r2, r2, r4
    add r1, sp, #0xa8
    movs r4, #0
    mov r5, r8
    str r5, [r1]
    str r4, [r1, #4]
    ldr r3, [r2]
    movs r2, #0
    bl _call_via_r3
    mov r6, sl
    ldr r0, [r6, #0x38]
    adds r0, #0x5c
    movs r1, #1
    bl SubtractAmount__9ToolStackUi
    ldr r7, [sp, #0x13c]
    strb r4, [r7]
    bl .L08031F0E
.L08030AA6:
    movs r0, #3
    mov r8, r0
    mov r1, sl
    adds r1, #0xb2
    ldrb r3, [r1]
    movs r2, #0
    adds r0, r3, #0
    subs r0, #0x2b
    adds r4, r1, #0
    cmp r0, #0x1a
    bhi .L08030ABE
    movs r2, #1
.L08030ABE:
    cmp r2, #0
    beq .L08030AD4
    mov r1, sl
    ldr r0, [r1]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    adds r1, r3, #0
    bl _call_via_r2
    adds r5, r0, #0
    b .L08030AD6
.L08030AD4:
    movs r5, #0
.L08030AD6:
    cmp r5, #0
    beq .L08030B2A
    ldr r0, [r5, #0x14]
    ldr r1, [r0, #0x4c]
    adds r0, r5, #0
    bl _call_via_r1
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08030B2A
    mov r2, sl
    ldr r0, [r2, #0x34]
    movs r7, #0xbe
    lsls r7, r7, #3
    adds r0, r0, r7
    bl GetFreePregnancyStall__C4Barn
    adds r6, r0, #0
    cmp r6, #0
    blt .L08030B2A
    ldrb r0, [r4]
    adds r4, r0, #0
    subs r4, #0x36
    cmp r4, #0xf
    bhi .L08030B2A
    ldr r0, [r5, #0x14]
    ldr r1, [r0, #0x6c]
    adds r0, r5, #0
    bl _call_via_r1
    mov r3, sl
    ldr r0, [r3, #0x34]
    adds r0, r0, r7
    adds r1, r6, #0
    adds r2, r4, #0
    bl MoveToPregnancyStall__4BarnUiUi
    mov r0, sl
    adds r0, #0xb1
    strb r4, [r0]
    movs r4, #1
    mov r8, r4
.L08030B2A:
    mov r5, sl
    ldr r0, [r5]
    ldr r2, [r0]
    movs r6, #0xa0
    lsls r6, r6, #1
    adds r2, r2, r6
    add r1, sp, #0xb0
    movs r4, #0
    mov r7, r8
    str r7, [r1]
    str r4, [r1, #4]
    ldr r3, [r2]
    movs r2, #0
    bl _call_via_r3
    ldr r0, [r5, #0x38]
    adds r0, #0x5c
    movs r1, #1
    bl SubtractAmount__9ToolStackUi
    ldr r0, [sp, #0x13c]
    strb r4, [r0]
    bl .L08031F0E
.L08030B5A:
    movs r5, #3
    mov r0, sl
    adds r0, #0xb2
    ldrb r3, [r0]
    movs r1, #0
    adds r0, r3, #0
    subs r0, #0x2b
    cmp r0, #0x1a
    bhi .L08030B6E
    movs r1, #1
.L08030B6E:
    cmp r1, #0
    beq .L08030B84
    mov r1, sl
    ldr r0, [r1]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    adds r1, r3, #0
    bl _call_via_r2
    adds r4, r0, #0
    b .L08030B86
.L08030B84:
    movs r4, #0
.L08030B86:
    cmp r4, #0
    beq .L08030BA6
    ldr r0, [r4, #0x14]
    ldr r1, [r0, #0x5c]
    adds r0, r4, #0
    bl _call_via_r1
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08030BA6
    ldr r0, [r4, #0x14]
    ldr r1, [r0, #0x7c]
    adds r0, r4, #0
    bl _call_via_r1
    movs r5, #2
.L08030BA6:
    mov r2, sl
    ldr r0, [r2]
    ldr r2, [r0]
    movs r3, #0xa0
    lsls r3, r3, #1
    adds r2, r2, r3
    add r1, sp, #0xb8
    movs r4, #0
    str r5, [r1]
    str r4, [r1, #4]
    ldr r3, [r2]
    movs r2, #0
    bl _call_via_r3
    mov r5, sl
    ldr r0, [r5, #0x38]
    adds r0, #0x5c
    movs r1, #1
    bl SubtractAmount__9ToolStackUi
    ldr r6, [sp, #0x13c]
    strb r4, [r6]
    bl .L08031F0E
.L08030BD6:
    mov r0, sl
    adds r0, #0xb2
    ldrb r3, [r0]
    movs r1, #0
    adds r0, r3, #0
    subs r0, #0x2b
    cmp r0, #0x1a
    bhi .L08030BE8
    movs r1, #1
.L08030BE8:
    cmp r1, #0
    beq .L08030BFE
    mov r7, sl
    ldr r0, [r7]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    adds r1, r3, #0
    bl _call_via_r2
    adds r4, r0, #0
    b .L08030C00
.L08030BFE:
    movs r4, #0
.L08030C00:
    cmp r4, #0
    bne .L08030C08
    bl .L08031C7C
.L08030C08:
    ldr r0, [r4, #0x14]
    ldr r1, [r0, #0x50]
    adds r0, r4, #0
    bl _call_via_r1
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08030C1C
    bl .L08031C7C
.L08030C1C:
    ldr r0, [r4, #0x14]
    ldr r1, [r0, #0x70]
    adds r0, r4, #0
    bl _call_via_r1
    bl .L08031C7C
.L08030C2A:
    mov r0, sl
    adds r0, #0xb2
    ldrb r3, [r0]
    movs r1, #0
    adds r0, r3, #0
    subs r0, #0x2b
    cmp r0, #0x1a
    bhi .L08030C3C
    movs r1, #1
.L08030C3C:
    cmp r1, #0
    beq .L08030C52
    mov r2, sl
    ldr r0, [r2]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    adds r1, r3, #0
    bl _call_via_r2
    adds r4, r0, #0
    b .L08030C54
.L08030C52:
    movs r4, #0
.L08030C54:
    cmp r4, #0
    beq .L08030CEA
    ldr r0, [r4, #0x14]
    ldr r1, [r0, #0x54]
    adds r0, r4, #0
    bl _call_via_r1
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08030CEA
    ldr r0, [r4, #0x14]
    ldr r1, [r0, #0x74]
    adds r0, r4, #0
    bl _call_via_r1
    movs r1, #0x1c
    cmp r0, #5
    bhi .L08030CB6
    lsls r0, r0, #2
    ldr r1, .L08030C84 @ =.L08030C88
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L08030C84: .4byte .L08030C88
.L08030C88: @ jump table
    .4byte .L08030CA0 @ case 0
    .4byte .L08030CA4 @ case 1
    .4byte .L08030CA8 @ case 2
    .4byte .L08030CAC @ case 3
    .4byte .L08030CB0 @ case 4
    .4byte .L08030CB4 @ case 5
.L08030CA0:
    movs r1, #0x1c
    b .L08030CB6
.L08030CA4:
    movs r1, #0x1d
    b .L08030CB6
.L08030CA8:
    movs r1, #0x1e
    b .L08030CB6
.L08030CAC:
    movs r1, #0x1f
    b .L08030CB6
.L08030CB0:
    movs r1, #0x20
    b .L08030CB6
.L08030CB4:
    movs r1, #0x21
.L08030CB6:
    mov r3, sl
    ldr r5, [r3, #0x14]
    add r4, sp, #4
    adds r0, r4, #0
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
    ldr r3, [r5, #0x5c]
    mov r0, sl
    mov r1, sp
    movs r2, #0
    bl _call_via_r3
.L08030CEA:
    mov r4, sl
    ldr r0, [r4, #0x34]
    ldr r5, .L08030D04 @ =0x00001C2C
    adds r0, r0, r5
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08030D00
    bl .L08031F0E
.L08030D00:
    bl .L08031D92
    .align 2, 0
.L08030D04: .4byte 0x00001C2C
.L08030D08:
    mov r0, sl
    adds r0, #0xb2
    ldrb r3, [r0]
    movs r1, #0
    adds r0, r3, #0
    subs r0, #0x2b
    cmp r0, #0x1a
    bhi .L08030D1A
    movs r1, #1
.L08030D1A:
    cmp r1, #0
    beq .L08030D30
    mov r7, sl
    ldr r0, [r7]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    adds r1, r3, #0
    bl _call_via_r2
    adds r4, r0, #0
    b .L08030D32
.L08030D30:
    movs r4, #0
.L08030D32:
    cmp r4, #0
    beq .L08030DB2
    ldr r0, [r4, #0x14]
    ldr r1, [r0, #0x58]
    adds r0, r4, #0
    bl _call_via_r1
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08030DB2
    ldr r0, [r4, #0x14]
    ldr r1, [r0, #0x78]
    adds r0, r4, #0
    bl _call_via_r1
    movs r1, #5
    cmp r0, #5
    bhi .L08030D92
    lsls r0, r0, #2
    ldr r1, .L08030D60 @ =.L08030D64
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L08030D60: .4byte .L08030D64
.L08030D64: @ jump table
    .4byte .L08030D7C @ case 0
    .4byte .L08030D80 @ case 1
    .4byte .L08030D84 @ case 2
    .4byte .L08030D88 @ case 3
    .4byte .L08030D8C @ case 4
    .4byte .L08030D90 @ case 5
.L08030D7C:
    movs r1, #5
    b .L08030D92
.L08030D80:
    movs r1, #6
    b .L08030D92
.L08030D84:
    movs r1, #7
    b .L08030D92
.L08030D88:
    movs r1, #8
    b .L08030D92
.L08030D8C:
    movs r1, #9
    b .L08030D92
.L08030D90:
    movs r1, #0xa
.L08030D92:
    mov r0, sl
    ldr r5, [r0, #0x14]
    add r4, sp, #0xc0
    adds r0, r4, #0
    bl __7ArticleUi
    ldrb r1, [r4]
    mov r0, sp
    bl __12RucksackItemG7Article
    ldr r3, [r5, #0x5c]
    mov r0, sl
    mov r1, sp
    movs r2, #0
    bl _call_via_r3
.L08030DB2:
    mov r1, sl
    ldr r0, [r1, #0x34]
    ldr r2, .L08030DD0 @ =0x00001C2C
    adds r0, r0, r2
    bl func_0800F190
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08030DC8
    bl .L08031F0E
.L08030DC8:
    movs r0, #0
    bl .L08031E84
    .align 2, 0
.L08030DD0: .4byte 0x00001C2C
.L08030DD4:
    mov r0, sp
    mov r1, sl
    bl GetLocation__C7AEntity
    mov r5, sp
    movs r4, #0x2b
.L08030DE0:
    mov r6, sl
    ldr r0, [r6]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    adds r1, r4, #0
    bl _call_via_r2
    adds r1, r0, #0
    cmp r1, #0
    beq .L08030E00
    ldr r0, [r1, #0x14]
    ldr r2, [r0, #0x3c]
    adds r0, r1, #0
    adds r1, r5, #0
    bl _call_via_r2
.L08030E00:
    adds r4, #1
    cmp r4, #0x45
    bls .L08030DE0
    bl sub_0803192E

    non_word_aligned_thumb_func_start sub_08030E0A
sub_08030E0A: @ 0x08030E0A
    mov r0, sl
    adds r0, #0x3f
    ldrb r0, [r0]
    cmp r0, #5
    beq .L08030E18
    bl .L08031F0E
.L08030E18:
    mov r2, sl
    adds r2, #0x88
    ldr r0, [r2, #4]
    ldr r1, [r2, #0xc]
    adds r0, r0, r1
    str r0, [r2, #4]
    ldr r0, [r2, #8]
    ldr r1, [r2, #0x10]
    adds r0, r0, r1
    str r0, [r2, #8]
    ldr r0, [r2, #0x18]
    ldr r4, .L08030E44 @ =0xFFFFC7D7
    adds r0, r0, r4
    str r0, [r2, #0x18]
    ldr r1, [r2, #0x14]
    adds r1, r1, r0
    cmp r1, #0
    ble .L08030E48
    str r1, [r2, #0x14]
    bl .L08031F0E
    .align 2, 0
.L08030E44: .4byte 0xFFFFC7D7
.L08030E48:
    str r3, [r2, #0x14]
    str r3, [r2, #0x18]
    bl .L08031F0E
.L08030E50:
    mov r0, sl
    adds r0, #0xc3
    ldrb r0, [r0]
    cmp r0, #0
    bne .L08030F46
    cmp r4, #0
    beq .L08030E70
    ldrh r0, [r4, #4]
    movs r1, #2
    ands r0, r1
    cmp r0, #0
    beq .L08030E70
    movs r0, #0x20
    ldr r6, [sp, #0x13c]
    strb r0, [r6]
    b .L08030F4C
.L08030E70:
    mov r2, sl
    adds r2, #0x58
    ldrh r0, [r2]
    cmp r0, #0
    bne .L08030E7E
    bl .L08031F0E
.L08030E7E:
    mov r1, sl
    adds r1, #0x54
    ldrh r0, [r1]
    adds r0, #1
    strh r0, [r1]
    lsls r0, r0, #0x10
    lsrs r0, r0, #0x10
    ldrh r2, [r2]
    cmp r0, r2
    beq .L08030E96
    bl .L08031F0E
.L08030E96:
    movs r0, #0
    strh r0, [r1]
    mov r0, sl
    adds r0, #0x40
    ldrb r0, [r0]
    movs r4, #0x1e
    cmp r0, #6
    bne .L08030EA8
    movs r4, #0x3c
.L08030EA8:
    bl rand
    asrs r0, r0, #2
    movs r1, #0x64
    bl __modsi3
    cmp r0, r4
    ble .L08030EBC
    bl .L08031F0E
.L08030EBC:
    movs r0, #0x1f
    ldr r7, [sp, #0x13c]
    strb r0, [r7]
    mov r0, sl
    movs r1, #0xa1
    movs r2, #0
    bl func_0802A4C0
    mov r0, sl
    movs r1, #2
    movs r2, #0
    bl func_08032384
    mov r1, sl
    adds r1, #0x78
    movs r0, #1
    strb r0, [r1]
    bl .L08031F0E
.L08030EE2:
    mov r2, sl
    adds r2, #0x54
    ldrh r0, [r2]
    adds r0, #1
    strh r0, [r2]
    mov r1, sl
    adds r1, #0x5a
    lsls r0, r0, #0x10
    lsrs r0, r0, #0x10
    ldrh r1, [r1]
    cmp r0, r1
    bne .L08030F0E
    movs r0, #0
    strh r0, [r2]
    movs r0, #0x1e
    ldr r1, [sp, #0x13c]
    strb r0, [r1]
    mov r0, sl
    movs r1, #0xa0
    movs r2, #0
    bl func_0802A4C0
.L08030F0E:
    cmp r4, #0
    bne .L08030F16
    bl .L08031F0E
.L08030F16:
    ldrh r0, [r4, #4]
    movs r1, #2
    ands r0, r1
    cmp r0, #0
    bne .L08030F24
    bl .L08031F0E
.L08030F24:
    mov r2, sl
    ldr r1, [r2, #0x5c]
    mov r0, sl
    adds r0, #0x40
    ldrb r2, [r0]
    mov r4, sl
    ldr r3, [r4, #0x34]
    mov r0, sp
    bl func_080A3F4C
    mov r1, sl
    adds r1, #0x60
    mov r0, sp
    ldm r0!, {r5, r6, r7}
    stm r1!, {r5, r6, r7}
    ldm r0!, {r2, r3, r4}
    stm r1!, {r2, r3, r4}
.L08030F46:
    movs r0, #0x20
    ldr r5, [sp, #0x13c]
    strb r0, [r5]
.L08030F4C:
    mov r0, sl
    movs r1, #0xa2
    movs r2, #1
    bl func_0802A4C0
    bl .L08031F0E
.L08030F5A:
    movs r1, #0
    mov r6, sl
    ldrh r0, [r6, #0x24]
    cmp r0, #0
    bne .L08030F66
    movs r1, #1
.L08030F66:
    cmp r1, #0
    bne .L08030F6E
    bl .L08031F0E
.L08030F6E:
    mov r0, sl
    bl CancelFarmerItemDisplay
    mov r7, sl
    ldr r0, [r7, #0x34]
    ldr r1, .L08030FBC @ =0x000034C4
    adds r0, r0, r1
    ldrb r0, [r0]
    cmp r0, #0
    bne .L08030FA0
    ldr r0, [r7, #0x38]
    movs r1, #5
    bl func_0800EB2C
    adds r2, r0, #0
    mov r0, sl
    adds r0, #0x78
    ldrb r0, [r0]
    movs r1, #0x32
    cmp r0, #0
    beq .L08030F9A
    movs r1, #0x64
.L08030F9A:
    adds r0, r2, #0
    bl func_0800EF88
.L08030FA0:
    mov r0, sl
    adds r0, #0x60
    ldrb r0, [r0, #0xc]
    cmp r0, #0
    bne .L08030FB8
    mov r0, sl
    adds r0, #0x68
    bl IsEmpty__C12RucksackItem
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08030FC0
.L08030FB8:
    movs r0, #1
    b .L08030FC2
    .align 2, 0
.L08030FBC: .4byte 0x000034C4
.L08030FC0:
    movs r0, #0
.L08030FC2:
    cmp r0, #0
    bne .L08030FCA
    bl sub_08031E0A
.L08030FCA:
    mov r0, sl
    adds r0, #0x6c
    ldrb r0, [r0]
    cmp r0, #0
    beq .L08030FE0
    mov r2, sl
    ldr r0, [r2, #0x14]
    adds r0, #0x80
    ldr r1, [r0]
    bl .L08031EA6
.L08030FE0:
    mov r0, sl
    adds r0, #0x6d
    ldrb r4, [r0]
    cmp r4, #0
    bne .L08030FEC
    b .L08031120
.L08030FEC:
    mov r3, sl
    ldr r0, [r3, #0x70]
    subs r0, #0x35
    cmp r0, #5
    bhi .L0803101C
    lsls r0, r0, #2
    ldr r1, .L08031000 @ =.L08031004
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L08031000: .4byte .L08031004
.L08031004: @ jump table
    .4byte .L0803101C @ case 0
    .4byte .L08031030 @ case 1
    .4byte .L0803103E @ case 2
    .4byte .L08031050 @ case 3
    .4byte .L08031060 @ case 4
    .4byte .L0803106E @ case 5
.L0803101C:
    mov r4, sl
    ldr r0, [r4, #0x38]
    adds r0, #0x54
    ldr r1, .L0803102C @ =0x00000103
    bl func_0800F510
    b .L0803107A
    .align 2, 0
.L0803102C: .4byte 0x00000103
.L08031030:
    mov r5, sl
    ldr r0, [r5, #0x38]
    adds r0, #0x54
    movs r1, #7
    bl func_0800F510
    b .L0803107A
.L0803103E:
    mov r6, sl
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    ldr r1, .L0803104C @ =0x00000141
    bl func_0800F510
    b .L0803107A
    .align 2, 0
.L0803104C: .4byte 0x00000141
.L08031050:
    mov r7, sl
    ldr r0, [r7, #0x38]
    adds r0, #0x54
    movs r1, #0x85
    lsls r1, r1, #1
    bl func_0800F510
    b .L0803107A
.L08031060:
    mov r1, sl
    ldr r0, [r1, #0x38]
    adds r0, #0x54
    movs r1, #0x69
    bl func_0800F510
    b .L0803107A
.L0803106E:
    mov r2, sl
    ldr r0, [r2, #0x38]
    adds r0, #0x54
    movs r1, #0xff
    bl func_0800F510
.L0803107A:
    mov r0, sl
    bl func_0802A588
    movs r4, #0
    movs r0, #0x21
    ldr r3, [sp, #0x13c]
    strb r0, [r3]
    mov r0, sl
    adds r0, #0x20
    ldrb r3, [r0]
    cmp r3, #1
    beq .L080310CE
    cmp r3, #1
    bgt .L0803109C
    cmp r3, #0
    beq .L080310E0
    b .L080310F2
.L0803109C:
    cmp r3, #2
    beq .L080310A6
    cmp r3, #3
    beq .L080310BC
    b .L080310F2
.L080310A6:
    mov r0, sl
    adds r0, #0x88
    ldr r1, .L080310B8 @ =0xFFF60000
    movs r2, #0xc0
    lsls r2, r2, #0xc
    str r1, [r0, #4]
    str r4, [r0, #8]
    b .L080310F0
    .align 2, 0
.L080310B8: .4byte 0xFFF60000
.L080310BC:
    mov r0, sl
    adds r0, #0x88
    movs r1, #0xa0
    lsls r1, r1, #0xc
    movs r2, #0xc0
    lsls r2, r2, #0xc
    str r1, [r0, #4]
    str r4, [r0, #8]
    b .L080310F0
.L080310CE:
    mov r0, sl
    adds r0, #0x88
    ldr r1, .L080310DC @ =0xFFF60000
    movs r2, #0xc0
    lsls r2, r2, #0xc
    str r4, [r0, #4]
    b .L080310EE
    .align 2, 0
.L080310DC: .4byte 0xFFF60000
.L080310E0:
    mov r0, sl
    adds r0, #0x88
    movs r1, #0xa0
    lsls r1, r1, #0xc
    movs r2, #0xc0
    lsls r2, r2, #0xc
    str r3, [r0, #4]
.L080310EE:
    str r1, [r0, #8]
.L080310F0:
    str r2, [r0, #0x14]
.L080310F2:
    mov r4, sl
    ldr r0, [r4]
    ldr r1, [r0]
    movs r5, #0xa6
    lsls r5, r5, #1
    adds r1, r1, r5
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    cmp r4, r5
    beq .L080311DE
.L0803110C:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0803119C
    adds r4, #4
    cmp r4, r5
    bne .L0803110C
    b .L080311DE
.L08031120:
    mov r0, sl
    adds r0, #0x68
    bl IsEmpty__C12RucksackItem
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L080311EC
    mov r6, sl
    ldr r0, [r6, #0x38]
    adds r0, #0x54
    ldr r1, [r6, #0x68]
    str r1, [sp]
    bl func_0800F418
    mov r0, sl
    bl func_0802A588
    movs r0, #0x21
    ldr r7, [sp, #0x13c]
    strb r0, [r7]
    mov r0, sl
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, #1
    beq .L0803118E
    cmp r0, #1
    bgt .L0803115C
    cmp r0, #0
    beq .L080311A0
    b .L080311B2
.L0803115C:
    cmp r0, #2
    beq .L08031166
    cmp r0, #3
    beq .L0803117C
    b .L080311B2
.L08031166:
    mov r0, sl
    adds r0, #0x88
    ldr r1, .L08031178 @ =0xFFF60000
    movs r2, #0xc0
    lsls r2, r2, #0xc
    str r1, [r0, #4]
    str r4, [r0, #8]
    b .L080311B0
    .align 2, 0
.L08031178: .4byte 0xFFF60000
.L0803117C:
    mov r0, sl
    adds r0, #0x88
    movs r1, #0xa0
    lsls r1, r1, #0xc
    movs r2, #0xc0
    lsls r2, r2, #0xc
    str r1, [r0, #4]
    str r4, [r0, #8]
    b .L080311B0
.L0803118E:
    mov r0, sl
    adds r0, #0x88
    ldr r1, .L08031198 @ =0xFFF60000
    b .L080311A8
    .align 2, 0
.L08031198: .4byte 0xFFF60000
.L0803119C:
    ldr r0, [r4]
    b .L080311E2
.L080311A0:
    mov r0, sl
    adds r0, #0x88
    movs r1, #0xa0
    lsls r1, r1, #0xc
.L080311A8:
    movs r2, #0xc0
    lsls r2, r2, #0xc
    str r4, [r0, #4]
    str r1, [r0, #8]
.L080311B0:
    str r2, [r0, #0x14]
.L080311B2:
    mov r1, sl
    ldr r0, [r1]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    cmp r4, r5
    beq .L080311DE
.L080311CC:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0803119C
    adds r4, #4
    cmp r4, r5
    bne .L080311CC
.L080311DE:
    subs r0, r5, #4
    ldr r0, [r0]
.L080311E2:
    movs r1, #0x6a
    bl func_08008B6C
    bl .L08031F0E
.L080311EC:
    ldr r3, [sp, #0x13c]
    strb r4, [r3]
    bl .L08031F0E
.L080311F4:
    movs r1, #0
    mov r5, sl
    ldrh r0, [r5, #0x24]
    cmp r0, #0
    bne .L08031200
    movs r1, #1
.L08031200:
    cmp r1, #0
    bne .L08031208
    bl .L08031F0E
.L08031208:
    mov r6, sl
    ldr r0, [r6]
    ldr r2, [r0]
    movs r7, #0xa0
    lsls r7, r7, #1
    adds r2, r2, r7
    add r1, sp, #0xc4
    movs r3, #0xf4
    lsls r3, r3, #2
    movs r4, #0
    str r3, [r1]
    str r4, [r1, #4]
    ldr r3, [r2]
    movs r2, #0
    bl _call_via_r3
    ldr r0, [sp, #0x13c]
    strb r4, [r0]
    mov r0, sl
    adds r0, #0x88
    movs r1, #0xa8
    lsls r1, r1, #0xd
    str r4, [r0, #4]
    str r4, [r0, #8]
    str r1, [r0, #0x14]
    bl .L08031F0E
.L0803123E:
    mov r1, sl
    ldr r1, [r1]
    str r1, [sp, #0x11c]
    movs r2, #0
    str r2, [sp, #0x120]
    mov r3, sl
    adds r3, #0x4a
    str r3, [sp, #0x14c]
    mov r4, sl
    adds r4, #0x48
    str r4, [sp, #0x144]
    mov r5, sl
    adds r5, #0x49
    str r5, [sp, #0x148]
.L0803125A:
    ldr r6, [sp, #0x14c]
    ldr r7, [sp, #0x120]
    adds r0, r6, r7
    ldrb r0, [r0]
    cmp r0, #0
    bne .L08031268
    b .L0803144E
.L08031268:
    ldr r1, [sp, #0x144]
    ldrb r0, [r1]
    adds r3, r0, #1
    add r1, sp, #0xcc
    str r3, [r1]
    adds r2, r7, #0
    subs r2, #2
    str r2, [r1, #4]
    mov r0, sl
    adds r0, #0x20
    ldrb r0, [r0]
    adds r4, r1, #0
    cmp r0, #1
    beq .L0803129C
    cmp r0, #1
    bgt .L0803128E
    cmp r0, #0
    beq .L08031294
    b .L080312AA
.L0803128E:
    cmp r0, #2
    beq .L080312A6
    b .L080312AA
.L08031294:
    ldr r0, [r4]
    str r2, [r4]
    str r0, [r4, #4]
    b .L080312AA
.L0803129C:
    rsbs r0, r2, #0
    str r0, [r4]
    rsbs r0, r3, #0
    str r0, [r4, #4]
    b .L080312AA
.L080312A6:
    rsbs r0, r3, #0
    str r0, [r4]
.L080312AA:
    mov r3, sl
    ldrh r2, [r3, #4]
    movs r5, #0xa
    ldrsh r1, [r3, r5]
    ldr r4, [r4]
    movs r6, #0xe
    ldrsh r3, [r3, r6]
    add r0, sp, #0xcc
    ldr r6, [r0, #4]
    mov sb, sp
    lsls r4, r4, #4
    adds r4, r4, r1
    lsls r6, r6, #4
    adds r6, r6, r3
    ldr r7, .L0803138C @ =0x000003FF
    mov ip, r7
    adds r1, r2, #0
    mov r0, ip
    ands r1, r0
    mov r5, sb
    ldrh r3, [r5]
    ldr r7, .L08031390 @ =0xFFFFFC00
    mov r8, r7
    mov r0, r8
    ands r0, r3
    orrs r0, r1
    strh r0, [r5]
    lsls r4, r4, #0x10
    lsrs r1, r4, #0x10
    movs r7, #0x3f
    ands r1, r7
    lsls r1, r1, #2
    ldrb r5, [r5, #1]
    movs r3, #3
    adds r0, r3, #0
    ands r0, r5
    orrs r0, r1
    mov r1, sb
    strb r0, [r1, #1]
    lsrs r4, r4, #0x16
    mov r5, ip
    ands r4, r5
    ldrh r1, [r1, #2]
    mov r0, r8
    ands r0, r1
    orrs r0, r4
    mov r1, sb
    strh r0, [r1, #2]
    lsls r6, r6, #0x10
    lsrs r0, r6, #0x10
    ands r0, r7
    lsls r0, r0, #2
    ldrb r1, [r1, #3]
    ands r3, r1
    orrs r3, r0
    mov r4, sb
    strb r3, [r4, #3]
    lsrs r6, r6, #0x16
    ands r6, r5
    ldrh r0, [r4, #4]
    mov r5, r8
    ands r5, r0
    orrs r5, r6
    strh r5, [r4, #4]
    add r4, sp, #8
    ldr r6, [sp, #0x11c]
    ldr r1, [r6]
    adds r0, r4, #0
    ldr r3, [r1, #0x34]
    adds r1, r6, #0
    bl _call_via_r3
    ldr r0, [sp]
    lsls r0, r0, #6
    asrs r0, r0, #0x10
    cmp r0, #0
    bge .L08031346
    adds r0, #7
.L08031346:
    asrs r5, r0, #3
    mov r0, sp
    ldrb r2, [r0, #3]
    lsrs r2, r2, #2
    ldrh r0, [r0, #4]
    ldr r1, .L0803138C @ =0x000003FF
    ands r0, r1
    lsls r0, r0, #6
    orrs r0, r2
    lsls r0, r0, #0x10
    asrs r0, r0, #0x10
    cmp r0, #0
    bge .L08031362
    adds r0, #7
.L08031362:
    asrs r3, r0, #3
    ldr r2, [sp, #8]
    ldrh r0, [r4, #8]
    cmp r2, #0
    beq .L08031394
    cmp r5, r0
    bhi .L08031394
    ldrh r7, [r4, #0xa]
    cmp r3, r7
    bhi .L08031394
    ldr r1, [r4, #4]
    muls r0, r3, r0
    adds r0, r0, r5
    cmp r1, #0
    beq .L08031384
    adds r0, r1, r0
    ldrb r0, [r0]
.L08031384:
    lsls r0, r0, #2
    adds r0, r2, r0
    b .L08031396
    .align 2, 0
.L0803138C: .4byte 0x000003FF
.L08031390: .4byte 0xFFFFFC00
.L08031394:
    movs r0, #0
.L08031396:
    cmp r0, #0
    beq .L0803144E
    ldr r1, [r0]
    lsls r0, r1, #0x1f
    cmp r0, #0
    bne .L080313A8
    lsls r0, r1, #0x1e
    cmp r0, #0
    bge .L080313B4
.L080313A8:
    ldr r0, [sp, #0x14c]
    ldr r2, [sp, #0x120]
    adds r1, r0, r2
    movs r0, #0
    strb r0, [r1]
    b .L0803144E
.L080313B4:
    ldr r3, [sp, #0x11c]
    ldr r0, [r3]
    adds r0, #0xc0
    ldr r2, [r0]
    adds r0, r3, #0
    mov r1, sp
    bl _call_via_r2
    adds r4, r0, #0
    cmp r4, #4
    bne .L080313DC
    mov r5, sl
    ldr r0, [r5, #0x34]
    adds r0, #0x14
    bl GetUnkFlag12__C4Farm
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L080313DC
    movs r4, #1
.L080313DC:
    cmp r4, #4
    bhi .L08031436
    lsls r0, r4, #2
    ldr r1, .L080313EC @ =.L080313F0
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L080313EC: .4byte .L080313F0
.L080313F0: @ jump table
    .4byte .L08031436 @ case 0
    .4byte .L08031436 @ case 1
    .4byte .L08031404 @ case 2
    .4byte .L08031436 @ case 3
    .4byte .L08031424 @ case 4
.L08031404:
    bl rand
    adds r1, r0, #0
    mov r2, sl
    adds r2, #0x50
    cmp r1, #0
    bge .L08031414
    adds r0, #0xf
.L08031414:
    asrs r0, r0, #4
    lsls r0, r0, #4
    subs r0, r1, r0
    adds r0, #5
    ldrh r6, [r2]
    adds r0, r0, r6
    strh r0, [r2]
    b .L08031436
.L08031424:
    mov r7, sl
    ldr r0, [r7, #0x34]
    adds r0, #0x14
    bl method_08009F80__4Farm
    mov r1, sl
    adds r1, #0x52
    movs r0, #1
    strb r0, [r1]
.L08031436:
    movs r3, #0
    mov r2, sl
    adds r2, #0x78
    ldrb r1, [r2]
    cmp r4, #0
    beq .L08031446
    movs r0, #1
    orrs r1, r0
.L08031446:
    cmp r1, #0
    beq .L0803144C
    movs r3, #1
.L0803144C:
    strb r3, [r2]
.L0803144E:
    ldr r0, [sp, #0x120]
    adds r0, #1
    str r0, [sp, #0x120]
    cmp r0, #4
    bhi .L0803145A
    b .L0803125A
.L0803145A:
    ldr r1, [sp, #0x144]
    ldrb r0, [r1]
    adds r0, #1
    strb r0, [r1]
    lsls r0, r0, #0x18
    lsrs r0, r0, #0x18
    ldr r2, [sp, #0x148]
    ldrb r2, [r2]
    cmp r0, r2
    beq .L08031472
    bl .L08031F0E
.L08031472:
    mov r3, sl
    ldr r0, [r3, #0x34]
    ldr r4, .L080314B8 @ =0x000034C4
    adds r0, r0, r4
    ldrb r0, [r0]
    cmp r0, #0
    bne .L0803149E
    ldr r0, [r3, #0x38]
    movs r1, #0
    bl func_0800EB2C
    adds r2, r0, #0
    mov r0, sl
    adds r0, #0x78
    ldrb r0, [r0]
    movs r1, #0x32
    cmp r0, #0
    beq .L08031498
    movs r1, #0x64
.L08031498:
    adds r0, r2, #0
    bl func_0800EF88
.L0803149E:
    mov r0, sl
    adds r0, #0x52
    ldrb r0, [r0]
    cmp r0, #0
    beq .L080314BC
    mov r5, sl
    ldr r0, [r5, #0x14]
    adds r0, #0x80
    ldr r1, [r0]
    mov r0, sl
    bl _call_via_r1
    b .L080314D4
    .align 2, 0
.L080314B8: .4byte 0x000034C4
.L080314BC:
    mov r1, sl
    adds r1, #0x50
    ldrh r0, [r1]
    cmp r0, #0
    beq .L080314D0
    adds r1, r0, #0
    mov r0, sl
    bl func_0802771C
    b .L080314D4
.L080314D0:
    ldr r6, [sp, #0x13c]
    strb r0, [r6]
.L080314D4:
    ldr r7, [sp, #0x11c]
    ldr r0, [r7]
    adds r0, #0xf8
    ldr r1, [r0]
    adds r0, r7, #0
    bl _call_via_r1
    bl .L08031F0E
.L080314E6:
    mov r3, sl
    adds r3, #0x4e
    mov r0, sl
    adds r0, #0x4f
    ldrb r2, [r3]
    ldrb r0, [r0]
    cmp r2, r0
    beq .L0803150A
    movs r1, #0
    mov r4, sl
    ldrh r0, [r4, #0x24]
    cmp r0, #0
    bne .L08031502
    movs r1, #1
.L08031502:
    cmp r1, #0
    beq .L0803150A
    adds r0, r2, #1
    strb r0, [r3]
.L0803150A:
    mov r5, sl
    ldr r6, [r5, #0x48]
    adds r5, r6, #0
    adds r5, #0xe1
    mov r4, sl
    adds r4, #0x4c
    adds r0, r6, #0
    adds r1, r5, #0
    adds r2, r4, #0
    bl func_080DC67C
    str r4, [sp, #0x154]
    cmp r0, r5
    beq .L08031618
    subs r5, r0, r6
    adds r0, r5, #0
    movs r1, #0xf
    bl __umodsi3
    subs r6, r0, #7
    add r4, sp, #0xd4
    str r6, [r4]
    adds r0, r5, #0
    movs r1, #0xf
    bl __udivsi3
    subs r1, r0, #7
    str r1, [r4, #4]
    mov r0, sl
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, #1
    beq .L08031564
    cmp r0, #1
    bgt .L08031556
    cmp r0, #0
    beq .L0803155C
    b .L08031572
.L08031556:
    cmp r0, #2
    beq .L0803156E
    b .L08031572
.L0803155C:
    ldr r0, [r4]
    str r1, [r4]
    str r0, [r4, #4]
    b .L08031572
.L08031564:
    rsbs r0, r1, #0
    str r0, [r4]
    rsbs r0, r6, #0
    str r0, [r4, #4]
    b .L08031572
.L0803156E:
    rsbs r0, r6, #0
    str r0, [r4]
.L08031572:
    mov r6, sl
    ldrh r2, [r6, #4]
    movs r7, #0xa
    ldrsh r1, [r6, r7]
    ldr r3, [r4]
    movs r0, #0xe
    ldrsh r5, [r6, r0]
    add r0, sp, #0xd4
    ldr r4, [r0, #4]
    mov r6, sp
    lsls r3, r3, #4
    adds r3, r3, r1
    lsls r4, r4, #4
    adds r4, r4, r5
    ldr r1, .L0803166C @ =0x000003FF
    mov sb, r1
    mov r5, sb
    ands r2, r5
    ldrh r1, [r6]
    ldr r5, .L08031670 @ =0xFFFFFC00
    adds r0, r5, #0
    ands r0, r1
    orrs r0, r2
    strh r0, [r6]
    lsls r3, r3, #0x10
    lsrs r1, r3, #0x10
    movs r7, #0x3f
    mov ip, r7
    mov r0, ip
    ands r1, r0
    lsls r1, r1, #2
    ldrb r2, [r6, #1]
    mov r8, r2
    movs r2, #3
    adds r0, r2, #0
    mov r7, r8
    ands r0, r7
    orrs r0, r1
    strb r0, [r6, #1]
    lsrs r3, r3, #0x16
    mov r0, sb
    ands r3, r0
    ldrh r1, [r6, #2]
    adds r0, r5, #0
    ands r0, r1
    orrs r0, r3
    strh r0, [r6, #2]
    lsls r4, r4, #0x10
    lsrs r0, r4, #0x10
    mov r1, ip
    ands r0, r1
    lsls r0, r0, #2
    ldrb r1, [r6, #3]
    ands r2, r1
    orrs r2, r0
    strb r2, [r6, #3]
    lsrs r4, r4, #0x16
    mov r2, sb
    ands r4, r2
    ldrh r0, [r6, #4]
    ands r5, r0
    orrs r5, r4
    strh r5, [r6, #4]
    mov r3, sl
    ldr r0, [r3]
    ldr r1, [r0]
    adds r1, #0xd4
    ldr r2, [r1]
    mov r1, sp
    bl _call_via_r2
    mov r3, sl
    adds r3, #0x78
    ldrb r1, [r3]
    rsbs r2, r1, #0
    orrs r2, r1
    lsls r0, r0, #0x18
    lsrs r0, r0, #0x18
    rsbs r1, r0, #0
    orrs r1, r0
    orrs r1, r2
    lsrs r1, r1, #0x1f
    strb r1, [r3]
.L08031618:
    mov r0, sl
    adds r0, #0x4d
    ldr r4, [sp, #0x154]
    ldrb r1, [r4]
    ldrb r0, [r0]
    cmp r0, r1
    bhi .L08031678
    mov r5, sl
    ldr r0, [r5, #0x34]
    ldr r6, .L08031674 @ =0x000034C4
    adds r0, r0, r6
    ldrb r0, [r0]
    cmp r0, #0
    bne .L08031652
    ldr r0, [r5, #0x38]
    movs r1, #1
    bl func_0800EB2C
    adds r2, r0, #0
    mov r0, sl
    adds r0, #0x78
    ldrb r0, [r0]
    movs r1, #0x32
    cmp r0, #0
    beq .L0803164C
    movs r1, #0x64
.L0803164C:
    adds r0, r2, #0
    bl func_0800EF88
.L08031652:
    movs r1, #0
    mov r7, sl
    ldrh r0, [r7, #0x24]
    cmp r0, #0
    bne .L0803165E
    movs r1, #1
.L0803165E:
    cmp r1, #0
    beq .L08031664
    b .L08031C7C
.L08031664:
    movs r0, #0x1d
    bl .L08031F0A
    .align 2, 0
.L0803166C: .4byte 0x000003FF
.L08031670: .4byte 0xFFFFFC00
.L08031674: .4byte 0x000034C4
.L08031678:
    adds r0, r1, #1
    ldr r3, [sp, #0x154]
    strb r0, [r3]
    bl .L08031F0E
.L08031682:
    movs r1, #0
    mov r4, sl
    ldrh r0, [r4, #0x24]
    cmp r0, #0
    bne .L0803168E
    movs r1, #1
.L0803168E:
    cmp r1, #0
    bne .L08031696
    bl .L08031F0E
.L08031696:
    b .L08031A5C
.L08031698:
    movs r1, #0
    mov r6, sl
    ldrh r0, [r6, #0x24]
    cmp r0, #0
    bne .L080316A4
    movs r1, #1
.L080316A4:
    cmp r1, #0
    bne .L080316AC
    bl .L08031F0E
.L080316AC:
    movs r0, #0
    ldr r7, [sp, #0x13c]
    strb r0, [r7]
    mov r0, sl
    bl func_0802AA84
    adds r1, r0, #0
    mov r2, sl
    ldr r0, [r2, #0x38]
    bl func_0800ED3C
    bl .L08031F0E
.L080316C6:
    mov r3, sl
    ldr r0, [r3, #0x38]
    bl func_0800E94C
    cmp r0, #0
    bne .L08031708
    mov r4, sl
    ldr r0, [r4, #0x34]
    ldr r5, .L08031704 @ =0x000034C4
    adds r0, r0, r5
    ldrb r0, [r0]
    cmp r0, #0
    beq .L080316E2
    b .L08031D92
.L080316E2:
    ldr r0, [r4, #0x38]
    movs r1, #4
    bl func_0800EB2C
    adds r2, r0, #0
    mov r0, sl
    adds r0, #0x78
    ldrb r0, [r0]
    movs r1, #0x32
    cmp r0, #0
    beq .L080316FA
    movs r1, #0x64
.L080316FA:
    adds r0, r2, #0
    bl func_0800EF88
    b .L08031D92
    .align 2, 0
.L08031704: .4byte 0x000034C4
.L08031708:
    mov r0, sl
    adds r0, #0x48
    mov r3, sl
    adds r3, #0x4a
    ldrb r0, [r0]
    lsls r0, r0, #0x18
    asrs r0, r0, #0x18
    ldrb r7, [r3]
    adds r6, r0, r7
    add r1, sp, #0xdc
    str r6, [r1]
    mov r0, sl
    adds r0, #0x49
    mov r2, sl
    adds r2, #0x4b
    ldrb r0, [r0]
    lsls r0, r0, #0x18
    asrs r0, r0, #0x18
    ldrb r4, [r2]
    adds r5, r0, r4
    str r5, [r1, #4]
    mov r0, sl
    adds r0, #0x20
    ldrb r0, [r0]
    str r3, [sp, #0x14c]
    str r2, [sp, #0x150]
    cmp r0, #1
    beq .L08031758
    cmp r0, #1
    bgt .L0803174A
    cmp r0, #0
    beq .L08031750
    b .L08031766
.L0803174A:
    cmp r0, #2
    beq .L08031762
    b .L08031766
.L08031750:
    ldr r0, [r1]
    str r5, [r1]
    str r0, [r1, #4]
    b .L08031766
.L08031758:
    rsbs r0, r5, #0
    str r0, [r1]
    rsbs r0, r6, #0
    str r0, [r1, #4]
    b .L08031766
.L08031762:
    rsbs r0, r6, #0
    str r0, [r1]
.L08031766:
    mov r5, sl
    ldr r0, [r5]
    ldr r2, [r0]
    adds r2, #0xd8
    str r2, [sp, #0x124]
    ldrh r3, [r5, #4]
    movs r6, #0xa
    ldrsh r2, [r5, r6]
    ldr r4, [r1]
    movs r7, #0xe
    ldrsh r6, [r5, r7]
    add r1, sp, #0xdc
    ldr r5, [r1, #4]
    mov r8, sp
    lsls r4, r4, #4
    adds r4, r4, r2
    lsls r5, r5, #4
    adds r5, r5, r6
    str r5, [sp, #0x188]
    ldr r1, .L08031874 @ =0x000003FF
    adds r7, r1, #0
    ands r3, r7
    mov r5, r8
    ldrh r2, [r5]
    ldr r6, .L08031878 @ =0xFFFFFC00
    adds r1, r6, #0
    ands r1, r2
    orrs r1, r3
    strh r1, [r5]
    lsls r4, r4, #0x10
    lsrs r2, r4, #0x10
    movs r1, #0x3f
    mov ip, r1
    ands r2, r1
    lsls r2, r2, #2
    ldrb r3, [r5, #1]
    mov sb, r3
    movs r3, #3
    adds r1, r3, #0
    mov r5, sb
    ands r1, r5
    orrs r1, r2
    mov r2, r8
    strb r1, [r2, #1]
    lsrs r4, r4, #0x16
    ands r4, r7
    ldrh r2, [r2, #2]
    adds r1, r6, #0
    ands r1, r2
    orrs r1, r4
    mov r4, r8
    strh r1, [r4, #2]
    ldr r1, [sp, #0x188]
    lsls r5, r1, #0x10
    lsrs r1, r5, #0x10
    mov r2, ip
    ands r1, r2
    lsls r1, r1, #2
    ldrb r2, [r4, #3]
    ands r3, r2
    orrs r3, r1
    strb r3, [r4, #3]
    lsrs r5, r5, #0x16
    ands r5, r7
    ldrh r1, [r4, #4]
    ands r6, r1
    orrs r6, r5
    strh r6, [r4, #4]
    ldr r3, [sp, #0x124]
    ldr r2, [r3]
    mov r1, sp
    bl _call_via_r2
    mov r4, sl
    adds r4, #0x78
    ldrb r1, [r4]
    rsbs r2, r1, #0
    orrs r2, r1
    lsls r0, r0, #0x18
    lsrs r0, r0, #0x18
    rsbs r1, r0, #0
    orrs r1, r0
    orrs r1, r2
    lsrs r1, r1, #0x1f
    strb r1, [r4]
    mov r5, sl
    ldr r0, [r5, #0x38]
    movs r1, #1
    bl func_0800ED54
    ldr r1, [sp, #0x150]
    ldrb r0, [r1]
    adds r0, #1
    strb r0, [r1]
    movs r2, #0xff
    mov r1, sl
    adds r1, #0x4d
    lsls r0, r0, #0x18
    lsrs r0, r0, #0x18
    ldrb r1, [r1]
    cmp r0, r1
    beq .L08031834
    b .L08031F0E
.L08031834:
    ldr r6, [sp, #0x14c]
    ldrb r0, [r6]
    adds r0, #1
    strb r0, [r6]
    mov r1, sl
    adds r1, #0x4c
    ands r0, r2
    ldrb r1, [r1]
    cmp r0, r1
    bne .L08031880
    ldr r0, [r5, #0x34]
    ldr r7, .L0803187C @ =0x000034C4
    adds r0, r0, r7
    ldrb r0, [r0]
    cmp r0, #0
    beq .L08031856
    b .L08031C7C
.L08031856:
    ldr r0, [r5, #0x38]
    movs r1, #4
    bl func_0800EB2C
    adds r2, r0, #0
    ldrb r0, [r4]
    movs r1, #0x32
    cmp r0, #0
    beq .L0803186A
    movs r1, #0x64
.L0803186A:
    adds r0, r2, #0
    bl func_0800EF88
    b .L08031C7C
    .align 2, 0
.L08031874: .4byte 0x000003FF
.L08031878: .4byte 0xFFFFFC00
.L0803187C: .4byte 0x000034C4
.L08031880:
    movs r0, #0
    ldr r2, [sp, #0x150]
    b .L08031F0C
.L08031886:
    mov r1, sl
    adds r1, #0x3d
    ldrb r0, [r1]
    subs r0, #1
    strb r0, [r1]
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08031898
    b .L08031F0E
.L08031898:
    movs r0, #5
    ldr r3, [sp, #0x13c]
    strb r0, [r3]
    b .L08031D78
.L080318A0:
    movs r1, #0
    mov r4, sl
    ldrh r0, [r4, #0x24]
    cmp r0, #0
    bne .L080318AC
    movs r1, #1
.L080318AC:
    cmp r1, #0
    bne .L080318B2
    b .L08031F0E
.L080318B2:
    mov r6, sl
    ldr r5, [r6]
    add r4, sp, #8
    adds r0, r4, #0
    mov r1, sl
    bl GetLocation__C12AActorEntity
    mov r0, sp
    adds r1, r4, #0
    movs r2, #6
    bl memcpy
    mov r2, sl
    adds r2, #0x79
    ldrb r1, [r2]
    movs r0, #1
    ands r0, r1
    adds r4, r2, #0
    cmp r0, #0
    beq .L080318F0
    ldr r0, [r5]
    ldr r2, [r0, #0x40]
    adds r0, r5, #0
    movs r1, #0x2b
    bl _call_via_r2
    cmp r0, #0
    beq .L080318F0
    mov r1, sp
    bl func_08021444
.L080318F0:
    ldrb r1, [r4]
    movs r0, #2
    ands r0, r1
    cmp r0, #0
    beq .L08031910
    ldr r0, [r5]
    ldr r2, [r0, #0x40]
    adds r0, r5, #0
    movs r1, #0x2c
    bl _call_via_r2
    cmp r0, #0
    beq .L08031910
    mov r1, sp
    bl func_080221F0
.L08031910:
    movs r4, #1
.L08031912:
    ldr r0, [r5]
    ldr r2, [r0, #0x40]
    adds r0, r5, #0
    adds r1, r4, #0
    bl _call_via_r2
    cmp r0, #0
    beq .L08031928
    mov r1, sp
    bl func_08035A48
.L08031928:
    adds r4, #1
    cmp r4, #0x23
    ble .L08031912

    non_word_aligned_thumb_func_start sub_0803192E
sub_0803192E: @ 0x0803192E
    movs r0, #0
    ldr r7, [sp, #0x13c]
    strb r0, [r7]
    b .L08031F0E
.L08031936:
    movs r1, #0
    mov r2, sl
    ldrh r0, [r2, #0x24]
    cmp r0, #0
    bne .L08031942
    movs r1, #1
.L08031942:
    cmp r1, #0
    bne .L08031948
    b .L08031F0E
.L08031948:
    movs r0, #4
    b .L08031E84
.L0803194C:
    ldr r0, [r4]
    b .L0803199E
.L08031950:
    movs r1, #0
    mov r4, sl
    ldrh r0, [r4, #0x24]
    cmp r0, #0
    bne .L0803195C
    movs r1, #1
.L0803195C:
    cmp r1, #0
    bne .L08031962
    b .L08031F0E
.L08031962:
    mov r0, sl
    bl func_0802A588
    movs r0, #0x14
    ldr r5, [sp, #0x13c]
    strb r0, [r5]
    mov r6, sl
    ldr r0, [r6]
    ldr r1, [r0]
    movs r7, #0xa6
    lsls r7, r7, #1
    adds r1, r1, r7
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    cmp r4, r5
    beq .L0803199A
.L08031988:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0803194C
    adds r4, #4
    cmp r4, r5
    bne .L08031988
.L0803199A:
    subs r0, r5, #4
    ldr r0, [r0]
.L0803199E:
    movs r1, #0x6b
    bl func_08008B6C
    b .L08031F0E
.L080319A6:
    movs r1, #0
    mov r2, sl
    ldrh r0, [r2, #0x24]
    cmp r0, #0
    bne .L080319B2
    movs r1, #1
.L080319B2:
    cmp r1, #0
    bne .L080319B8
    b .L08031F0E
.L080319B8:
    movs r0, #4
    b .L08031E84
.L080319BC:
    movs r1, #0
    mov r4, sl
    ldrh r0, [r4, #0x24]
    cmp r0, #0
    bne .L080319C8
    movs r1, #1
.L080319C8:
    cmp r1, #0
    bne .L080319CE
    b .L08031F0E
.L080319CE:
    movs r0, #6
    ldr r5, [sp, #0x13c]
    strb r0, [r5]
    mov r0, sl
    movs r1, #8
    movs r2, #1
    bl func_08032384
    b .L08031F0E
.L080319E0:
    movs r1, #0
    mov r6, sl
    ldrh r0, [r6, #0x24]
    cmp r0, #0
    bne .L080319EC
    movs r1, #1
.L080319EC:
    cmp r1, #0
    beq .L08031A24
    mov r7, sl
    ldr r0, [r7]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    ldr r1, [r0, #8]
    str r1, [r7, #8]
    ldr r1, [r0, #0xc]
    str r1, [r7, #0xc]
    movs r1, #0
    str r1, [r7, #0x18]
    str r1, [r7, #0x1c]
    adds r0, #0x20
    ldrb r1, [r0]
    mov r0, sl
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, r1
    beq .L08031A20
    mov r0, sl
    bl SetAnimFacing__12AActorEntityUi
.L08031A20:
    movs r0, #7
    b .L08031C7E
.L08031A24:
    mov r0, sl
    adds r0, #0xb8
    ldr r0, [r0]
    mov r2, sl
    str r0, [r2, #0x18]
    mov r0, sl
    adds r0, #0xbc
    ldr r0, [r0]
    str r0, [r2, #0x1c]
    b .L08031F0E
.L08031A38:
    movs r1, #0
    mov r3, sl
    ldrh r0, [r3, #0x24]
    cmp r0, #0
    bne .L08031A44
    movs r1, #1
.L08031A44:
    cmp r1, #0
    bne .L08031A4A
    b .L08031F0E
.L08031A4A:
    mov r4, sl
    ldr r0, [r4]
    ldr r1, [r0]
    ldr r2, [r1, #0x40]
    movs r1, #0x2c
    bl _call_via_r2
    bl func_08021F3C
.L08031A5C:
    movs r0, #0

    non_word_aligned_thumb_func_start sub_08031A5E
sub_08031A5E: @ 0x08031A5E
    ldr r5, [sp, #0x13c]
    strb r0, [r5]
    b .L08031F0E
.L08031A64:
    movs r1, #0
    mov r6, sl
    ldrh r0, [r6, #0x24]
    cmp r0, #0
    bne .L08031A70
    movs r1, #1
.L08031A70:
    cmp r1, #0
    bne .L08031A76
    b .L08031F0E
.L08031A76:
    movs r0, #0
    ldr r7, [sp, #0x13c]
    strb r0, [r7]
    mov r1, sl
    adds r1, #0x88
    movs r2, #0xa8
    lsls r2, r2, #0xd
    str r0, [r1, #4]
    str r0, [r1, #8]
    str r2, [r1, #0x14]
    mov r0, sl
    bl func_0802B054
    b .L08031F0E
.L08031A92:
    movs r1, #0
    mov r2, sl
    ldrh r0, [r2, #0x24]
    cmp r0, #0
    bne .L08031A9E
    movs r1, #1
.L08031A9E:
    cmp r1, #0
    bne .L08031AA4
    b .L08031F0E
.L08031AA4:
    movs r0, #0
    b .L08031E84
.L08031AA8:
    movs r1, #0
    mov r4, sl
    ldrh r0, [r4, #0x24]
    cmp r0, #0
    bne .L08031AB4
    movs r1, #1
.L08031AB4:
    cmp r1, #0
    bne .L08031ABA
    b .L08031F0E
.L08031ABA:
    mov r5, sl
    ldr r0, [r5, #0x14]
    b .L08031EA4
.L08031AC0:
    movs r1, #0
    mov r6, sl
    ldrh r0, [r6, #0x24]
    cmp r0, #0
    bne .L08031ACC
    movs r1, #1
.L08031ACC:
    cmp r1, #0
    bne .L08031AD2
    b .L08031F0E
.L08031AD2:
    movs r4, #0
    movs r0, #0x2b
    ldr r7, [sp, #0x13c]
    strb r0, [r7]
    mov r1, sl
    ldr r0, [r1]
    ldr r3, [r0]
    movs r2, #0xa0
    lsls r2, r2, #1
    adds r3, r3, r2
    add r1, sp, #0xe4
    ldr r2, .L08031AF8 @ =0x000002B6
    str r2, [r1]
    str r4, [r1, #4]
    ldr r3, [r3]
    movs r2, #2
    bl _call_via_r3
    b .L08031F0E
    .align 2, 0
.L08031AF8: .4byte 0x000002B6
.L08031AFC:
    movs r1, #0
    mov r3, sl
    ldrh r0, [r3, #0x24]
    cmp r0, #0
    bne .L08031B08
    movs r1, #1
.L08031B08:
    cmp r1, #0
    beq .L08031B54
    movs r0, #0
    movs r1, #7
    ldr r4, [sp, #0x13c]
    strb r1, [r4]
    mov r1, sl
    adds r1, #0x3e
    strb r0, [r1]
    mov r0, sl
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, #0
    blt .L08031BEE
    cmp r0, #1
    ble .L08031B40
    cmp r0, #3
    bgt .L08031BEE
    mov r5, sl
    ldr r1, [r5, #8]
    mov r0, sl
    adds r0, #0x84
    movs r6, #0
    ldrsh r0, [r0, r6]
    lsls r0, r0, #0x10
    subs r0, r0, r1
    str r0, [r5, #0x18]
    b .L08031BEE
.L08031B40:
    mov r7, sl
    ldr r1, [r7, #0xc]
    mov r0, sl
    adds r0, #0x84
    movs r2, #0
    ldrsh r0, [r0, r2]
    lsls r0, r0, #0x10
    subs r0, r0, r1
    str r0, [r7, #0x1c]
    b .L08031BEE
.L08031B54:
    mov r0, sl
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, #0
    bge .L08031B60
    b .L08031F0E
.L08031B60:
    cmp r0, #1
    ble .L08031B76
    cmp r0, #3
    ble .L08031B6A
    b .L08031F0E
.L08031B6A:
    mov r0, sl
    adds r0, #0x80
    ldr r0, [r0]
    mov r3, sl
    str r0, [r3, #0x18]
    b .L08031F0E
.L08031B76:
    mov r0, sl
    adds r0, #0x80
    ldr r0, [r0]
    mov r4, sl
    str r0, [r4, #0x1c]
    b .L08031F0E
.L08031B82:
    movs r1, #0
    mov r5, sl
    ldrh r0, [r5, #0x24]
    cmp r0, #0
    bne .L08031B8E
    movs r1, #1
.L08031B8E:
    cmp r1, #0
    beq .L08031BF6
    movs r0, #0
    ldr r6, [sp, #0x13c]
    strb r0, [r6]
    mov r1, sl
    adds r1, #0x3e
    strb r0, [r1]
    mov r0, sl
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, #0
    blt .L08031BD6
    cmp r0, #1
    ble .L08031BC4
    cmp r0, #3
    bgt .L08031BD6
    mov r7, sl
    ldr r1, [r7, #8]
    mov r0, sl
    adds r0, #0x84
    movs r2, #0
    ldrsh r0, [r0, r2]
    lsls r0, r0, #0x10
    subs r0, r0, r1
    str r0, [r7, #0x18]
    b .L08031BD6
.L08031BC4:
    mov r3, sl
    ldr r1, [r3, #0xc]
    mov r0, sl
    adds r0, #0x84
    movs r4, #0
    ldrsh r0, [r0, r4]
    lsls r0, r0, #0x10
    subs r0, r0, r1
    str r0, [r3, #0x1c]
.L08031BD6:
    mov r3, sl
    adds r3, #0x88
    movs r1, #0
    ldrb r0, [r3]
    cmp r0, #1
    bne .L08031BE4
    movs r1, #1
.L08031BE4:
    cmp r1, #0
    beq .L08031BEE
    movs r2, #0xa8
    lsls r2, r2, #0xd
    str r2, [r3, #0x14]
.L08031BEE:
    mov r0, sl
    bl func_0802AD80
    b .L08031F0E
.L08031BF6:
    mov r5, sl
    ldrh r3, [r5, #0x24]
    cmp r3, #0x10
    bhi .L08031C24
    mov r0, sl
    adds r0, #0x20
    ldrb r0, [r0]
    cmp r0, #0
    blt .L08031C24
    cmp r0, #1
    ble .L08031C1A
    cmp r0, #3
    bgt .L08031C24
    mov r0, sl
    adds r0, #0x80
    ldr r0, [r0]
    str r0, [r5, #0x18]
    b .L08031C24
.L08031C1A:
    mov r0, sl
    adds r0, #0x80
    ldr r0, [r0]
    mov r6, sl
    str r0, [r6, #0x1c]
.L08031C24:
    mov r0, sl
    adds r0, #0x88
    movs r2, #0
    ldrb r1, [r0]
    adds r4, r0, #0
    cmp r1, #1
    bne .L08031C34
    movs r2, #1
.L08031C34:
    cmp r2, #0
    bne .L08031C3A
    b .L08031F0E
.L08031C3A:
    ldr r2, [r4, #0x14]
    adds r0, r2, #0
    cmp r3, #0xc
    beq .L08031C56
    cmp r3, #0xc
    bhi .L08031C4C
    cmp r3, #4
    beq .L08031C50
    b .L08031C5A
.L08031C4C:
    cmp r3, #0x10
    bne .L08031C5A
.L08031C50:
    movs r2, #0xe8
    lsls r2, r2, #0xd
    b .L08031C5A
.L08031C56:
    movs r2, #0x84
    lsls r2, r2, #0xe
.L08031C5A:
    cmp r2, r0
    bne .L08031C60
    b .L08031F0E
.L08031C60:
    str r2, [r4, #0x14]
    b .L08031F0E
.L08031C64:
    movs r1, #0
    mov r7, sl
    ldrh r0, [r7, #0x24]
    cmp r0, #0
    bne .L08031C70
    movs r1, #1
.L08031C70:
    cmp r1, #0
    bne .L08031C76
    b .L08031F0E
.L08031C76:
    mov r0, sl
    bl func_0802A588
.L08031C7C:
    movs r0, #0
.L08031C7E:
    ldr r1, [sp, #0x13c]
    strb r0, [r1]
    b .L08031F0E
.L08031C84:
    mov r1, sl
    adds r1, #0x3d
    ldrb r0, [r1]
    subs r0, #1
    strb r0, [r1]
    lsls r0, r0, #0x18
    lsrs r2, r0, #0x18
    cmp r2, #0
    beq .L08031C98
    b .L08031F0E
.L08031C98:
    movs r0, #0x30
    ldr r3, [sp, #0x13c]
    strb r0, [r3]
    movs r0, #0x1e
    strb r0, [r1]
    mov r0, sl
    adds r0, #0x88
    movs r1, #0xa8
    lsls r1, r1, #0xd
    str r2, [r0, #4]
    str r2, [r0, #8]
    str r1, [r0, #0x14]
    b .L08031F0E
.L08031CB2:
    ldr r0, [r4]
    b .L08031D04
.L08031CB6:
    mov r1, sl
    adds r1, #0x3d
    ldrb r0, [r1]
    subs r0, #1
    strb r0, [r1]
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08031CC8
    b .L08031F0E
.L08031CC8:
    movs r0, #0xf
    ldr r4, [sp, #0x13c]
    strb r0, [r4]
    mov r0, sl
    bl CancelFarmerItemDisplay
    mov r5, sl
    ldr r0, [r5]
    ldr r1, [r0]
    movs r6, #0xa6
    lsls r6, r6, #1
    adds r1, r1, r6
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    cmp r4, r5
    beq .L08031D00
.L08031CEE:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08031CB2
    adds r4, #4
    cmp r4, r5
    bne .L08031CEE
.L08031D00:
    subs r0, r5, #4
    ldr r0, [r0]
.L08031D04:
    movs r1, #0x65
    bl func_08008B6C
    b .L08031F0E
.L08031D0C:
    movs r1, #0
    mov r7, sl
    ldrh r0, [r7, #0x24]
    cmp r0, #0
    bne .L08031D18
    movs r1, #1
.L08031D18:
    cmp r1, #0
    bne .L08031D1E
    b .L08031F0E
.L08031D1E:
    movs r0, #0x32
    ldr r1, [sp, #0x13c]
    strb r0, [r1]
    b .L08031E26
.L08031D26:
    ldr r0, [r4]
    b .L08031D72
.L08031D2A:
    mov r1, sl
    adds r1, #0x3d
    ldrb r0, [r1]
    subs r0, #1
    strb r0, [r1]
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08031D3C
    b .L08031F0E
.L08031D3C:
    movs r0, #0x33
    ldr r2, [sp, #0x13c]
    strb r0, [r2]
    mov r3, sl
    ldr r0, [r3]
    ldr r1, [r0]
    movs r4, #0xa6
    lsls r4, r4, #1
    adds r1, r1, r4
    ldr r1, [r1]
    bl _call_via_r1
    ldr r4, [r0]
    ldr r5, [r0, #4]
    cmp r4, r5
    beq .L08031D6E
.L08031D5C:
    ldr r0, [r4]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08031D26
    adds r4, #4
    cmp r4, r5
    bne .L08031D5C
.L08031D6E:
    subs r0, r5, #4
    ldr r0, [r0]
.L08031D72:
    movs r1, #0x6b

    thumb_func_start sub_08031D74
sub_08031D74: @ 0x08031D74
    bl func_08008B6C
.L08031D78:
    mov r0, sl
    bl CancelFarmerItemDisplay
    b .L08031F0E
.L08031D80:
    movs r1, #0
    mov r5, sl
    ldrh r0, [r5, #0x24]
    cmp r0, #0
    bne .L08031D8C
    movs r1, #1
.L08031D8C:
    cmp r1, #0
    bne .L08031D92
    b .L08031F0E
.L08031D92:
    movs r0, #0
    ldr r6, [sp, #0x13c]
    strb r0, [r6]
    b .L08031F0E
.L08031D9A:
    movs r1, #0
    mov r7, sl
    ldrh r0, [r7, #0x24]
    cmp r0, #0
    bne .L08031DA6
    movs r1, #1
.L08031DA6:
    cmp r1, #0
    bne .L08031DAC
    b .L08031F0E
.L08031DAC:
    movs r0, #0x38
    ldr r1, [sp, #0x13c]
    strb r0, [r1]
    b .L08031E26
.L08031DB4:
    ldr r0, [r5]
    b .L08031DFE
.L08031DB8:
    mov r1, sl
    adds r1, #0x3d
    ldrb r0, [r1]
    subs r0, #1
    strb r0, [r1]
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08031DCA
    b .L08031F0E
.L08031DCA:
    mov r2, sl
    ldr r0, [r2]
    ldr r1, [r0]
    movs r3, #0xa6
    lsls r3, r3, #1
    adds r1, r1, r3
    ldr r1, [r1]
    bl _call_via_r1
    ldr r5, [r0]
    ldr r6, [r0, #4]
    mov r4, sl
    adds r4, #0x88
    cmp r5, r6
    beq .L08031DFA
.L08031DE8:
    ldr r0, [r5]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08031DB4
    adds r5, #4
    cmp r5, r6
    bne .L08031DE8
.L08031DFA:
    subs r0, r6, #4
    ldr r0, [r0]
.L08031DFE:
    movs r1, #0x65
    bl func_08008B6C
    movs r0, #0
    strb r0, [r4]
    movs r0, #0xf

    non_word_aligned_thumb_func_start sub_08031E0A
sub_08031E0A: @ 0x08031E0A
    ldr r4, [sp, #0x13c]
    strb r0, [r4]
    b .L08031F0E
.L08031E10:
    movs r1, #0
    mov r5, sl
    ldrh r0, [r5, #0x24]
    cmp r0, #0
    bne .L08031E1C
    movs r1, #1
.L08031E1C:
    cmp r1, #0
    beq .L08031F0E
    movs r0, #0x39
    ldr r6, [sp, #0x13c]
    strb r0, [r6]
.L08031E26:
    mov r1, sl
    adds r1, #0x3d
    movs r0, #0x1e
    strb r0, [r1]
    b .L08031F0E
.L08031E30:
    ldr r0, [r5]
    b .L08031E78
.L08031E34:
    mov r1, sl
    adds r1, #0x3d
    ldrb r0, [r1]
    subs r0, #1
    strb r0, [r1]
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08031F0E
    mov r7, sl
    ldr r0, [r7]
    ldr r1, [r0]
    movs r2, #0xa6
    lsls r2, r2, #1
    adds r1, r1, r2
    ldr r1, [r1]
    bl _call_via_r1
    ldr r5, [r0]
    ldr r6, [r0, #4]
    mov r4, sl
    adds r4, #0x88
    cmp r5, r6
    beq .L08031E74
.L08031E62:
    ldr r0, [r5]
    bl func_08008CD0
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L08031E30
    adds r5, #4
    cmp r5, r6
    bne .L08031E62
.L08031E74:
    subs r0, r6, #4
    ldr r0, [r0]
.L08031E78:
    movs r1, #0x66
    bl func_08008B6C
    movs r0, #0
    strb r0, [r4]
    movs r0, #0x10
.L08031E84:
    ldr r3, [sp, #0x13c]
    strb r0, [r3]
    b .L08031F0E
.L08031E8A:
    mov r1, sl
    adds r1, #0x3d
    ldrb r0, [r1]
    subs r0, #1
    strb r0, [r1]
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L08031F0E
    mov r0, sl
    bl func_0802B054
    mov r4, sl
    ldr r0, [r4, #0x14]
.L08031EA4:
    ldr r1, [r0, #0x70]
.L08031EA6:
    mov r0, sl
    bl _call_via_r1
    b .L08031F0E
.L08031EAE:
    mov r0, sl
    bl func_0802AEA4
    lsls r0, r0, #0x18
    lsrs r0, r0, #0x18
    cmp r0, #0
    beq .L08031F0A
    mov r0, sl
    adds r0, #0x20
    ldrb r0, [r0]
    adds r1, r0, #0
    cmp r0, #1
    beq .L08031EE8
    cmp r0, #1
    bgt .L08031ED2
    cmp r0, #0
    beq .L08031EDC
    b .L08031F0E
.L08031ED2:
    cmp r1, #2
    beq .L08031F00
    cmp r1, #3
    beq .L08031EF2
    b .L08031F0E
.L08031EDC:
    ldr r0, .L08031EE4 @ =0xFFFF0000
    mov r5, sl
    str r0, [r5, #0x1c]
    b .L08031F0E
    .align 2, 0
.L08031EE4: .4byte 0xFFFF0000
.L08031EE8:
    movs r0, #0x80
    lsls r0, r0, #9
    mov r6, sl
    str r0, [r6, #0x1c]
    b .L08031F0E
.L08031EF2:
    ldr r0, .L08031EFC @ =0xFFFF0000
    mov r7, sl
    str r0, [r7, #0x18]
    b .L08031F0E
    .align 2, 0
.L08031EFC: .4byte 0xFFFF0000
.L08031F00:
    movs r0, #0x80
    lsls r0, r0, #9
    mov r1, sl
    str r0, [r1, #0x18]
    b .L08031F0E
.L08031F0A:
    ldr r2, [sp, #0x13c]
.L08031F0C:
    strb r0, [r2]
.L08031F0E:
    add sp, #0x18c
    pop {r3, r4, r5}
    mov r8, r3
    mov sb, r4
    mov sl, r5
    pop {r4, r5, r6, r7}
    pop {r0}
    bx r0
    .align 2, 0
.L08031F20:
    .byte 0x02, 0x49, 0x40, 0x18, 0x01, 0x21, 0x01, 0x70, 0x70, 0x47, 0x00, 0x00, 0x81, 0x02, 0x00, 0x00
    .byte 0x30, 0xB5, 0x05, 0x1C, 0x0C, 0x1C, 0x28, 0x68, 0x00, 0x68, 0x01, 0x68, 0x89, 0x6E, 0xA1, 0xF0
    .byte 0xE7, 0xFC, 0x01, 0x1C, 0x28, 0x1C, 0xB4, 0x30, 0x22, 0x1C, 0x2C, 0xF0, 0x81, 0xFC, 0x28, 0x1C
    .byte 0xC8, 0x30, 0x00, 0x22, 0x01, 0x21, 0x01, 0x70, 0x02, 0x30, 0x02, 0x70, 0x01, 0x30, 0x01, 0x70
    .byte 0x30, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00, 0x30, 0xB5, 0x05, 0x1C, 0x0C, 0x1C, 0x28, 0x68
    .byte 0x00, 0x68, 0x01, 0x68, 0x49, 0x6E, 0xA1, 0xF0, 0xCB, 0xFC, 0x01, 0x1C, 0x28, 0x1C, 0xB4, 0x30
    .byte 0x22, 0x1C, 0x2C, 0xF0, 0x65, 0xFC, 0x28, 0x1C, 0xC8, 0x30, 0x00, 0x22, 0x01, 0x21, 0x01, 0x70
    .byte 0x02, 0x30, 0x02, 0x70, 0x01, 0x30, 0x01, 0x70, 0x30, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00
    .byte 0x00, 0xB5, 0x02, 0x1C, 0xB2, 0x30, 0x03, 0x78, 0x00, 0x21, 0x18, 0x1C, 0x2B, 0x38, 0x1A, 0x28
    .byte 0x00, 0xD8, 0x01, 0x21, 0x00, 0x29, 0x01, 0xD1, 0x00, 0x20, 0x05, 0xE0, 0x10, 0x68, 0x01, 0x68
    .byte 0x0A, 0x6C, 0x19, 0x1C, 0xA1, 0xF0, 0xA6, 0xFC, 0x02, 0xBC, 0x08, 0x47, 0x00, 0xB5, 0x00, 0x20
    .byte 0x2B, 0x39, 0x1A, 0x29, 0x00, 0xD8, 0x01, 0x20, 0x02, 0xBC, 0x08, 0x47, 0x00, 0xB5, 0x01, 0x2A
    .byte 0x0C, 0xD0, 0x01, 0x2A, 0x02, 0xDC, 0x00, 0x2A, 0x03, 0xD0, 0x11, 0xE0, 0x02, 0x2A, 0x0C, 0xD0
    .byte 0x0E, 0xE0, 0x0A, 0x68, 0x48, 0x68, 0x08, 0x60, 0x4A, 0x60, 0x09, 0xE0, 0x0A, 0x68, 0x48, 0x68
    .byte 0x40, 0x42, 0x08, 0x60, 0x50, 0x42, 0x48, 0x60, 0x02, 0xE0, 0x08, 0x68, 0x40, 0x42, 0x08, 0x60
    .byte 0x01, 0xBC, 0x00, 0x47, 0x88, 0x30, 0x70, 0x47, 0xC2, 0x30, 0x00, 0x78, 0x70, 0x47, 0x00, 0x00
    .byte 0xC2, 0x30, 0x01, 0x70, 0x70, 0x47, 0x00, 0x00, 0x60, 0x30, 0x70, 0x47, 0xC1, 0x30, 0x00, 0x78
    .byte 0x70, 0x47, 0x00, 0x00, 0xB0, 0x30, 0x00, 0x78, 0x70, 0x47, 0x00, 0x00, 0xAC, 0x30, 0x00, 0x68
    .byte 0x70, 0x47, 0x00, 0x00, 0xA8, 0x30, 0x00, 0x68, 0x70, 0x47, 0x00, 0x00, 0xB1, 0x30, 0x00, 0x78
    .byte 0x70, 0x47, 0x00, 0x00, 0x7A, 0x30, 0x00, 0x78, 0x70, 0x47, 0x00, 0x00

    .section .text.entities_part2.08032074,"ax",%progbits
.L08032074:
    .byte 0xC0, 0x30, 0x00, 0x78, 0x70, 0x47, 0x00, 0x00, 0xCB, 0x1F, 0x03, 0x80
    .byte 0x13, 0x1C, 0x09, 0x3B, 0x43, 0x80, 0x07, 0x31, 0x81, 0x80, 0x05, 0x32, 0xC2, 0x80, 0x70, 0x47
