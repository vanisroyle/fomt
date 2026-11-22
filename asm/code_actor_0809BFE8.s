    .INCLUDE "asm/macro.inc"
    .SYNTAX UNIFIED

	.section .text.0809C6BC, "ax", %progbits
    thumb_func_start func_0809C6BC
func_0809C6BC: @ 0x0809C6BC
    push {r4, r5, r6, r7, lr}
    mov r7, sl
    mov r6, sb
    mov r5, r8
    push {r5, r6, r7}
    sub sp, #4
    adds r7, r0, #0
    movs r0, #0
    str r0, [r7]
    ldr r0, .L0809CAB8 @ =0x0000FFFF
    strh r0, [r7, #0xc]
    movs r5, #7
    rsbs r5, r5, #0
    movs r0, #0
    strb r0, [r7, #0xe]
    subs r0, #3
    mov sb, r0
    movs r4, #0x19
    rsbs r4, r4, #0
    movs r0, #0
    strb r0, [r7, #0xf]
    strb r0, [r7, #0x10]
    ldrb r1, [r7, #0x11]
    subs r0, #2
    ands r0, r1
    mov r1, sb
    ands r0, r1
    movs r2, #5
    rsbs r2, r2, #0
    ands r0, r2
    movs r1, #9
    rsbs r1, r1, #0
    ands r0, r1
    subs r2, #0xc
    ands r0, r2
    movs r3, #0x61
    rsbs r3, r3, #0
    ands r0, r3
    strb r0, [r7, #0x11]
    ldr r0, [r7, #0x10]
    ldr r1, .L0809CABC @ =0xFFFE7FFF
    mov r8, r1
    ands r0, r1
    str r0, [r7, #0x10]
    ldrb r1, [r7, #0x12]
    adds r0, r5, #0
    ands r0, r1
    ands r0, r4
    ands r0, r3
    strb r0, [r7, #0x12]
    ldrh r1, [r7, #0x12]
    ldr r6, .L0809CAC0 @ =0xFFFFFE7F
    adds r0, r6, #0
    ands r0, r1
    strh r0, [r7, #0x12]
    ldrb r1, [r7, #0x13]
    adds r0, r5, #0
    ands r0, r1
    ands r0, r4
    ands r0, r3
    movs r2, #0x7f
    ands r0, r2
    strb r0, [r7, #0x13]
    ldrb r1, [r7, #0x14]
    movs r0, #2
    rsbs r0, r0, #0
    ands r0, r1
    ands r0, r5
    ands r0, r4
    ands r0, r3
    strb r0, [r7, #0x14]
    ldrh r1, [r7, #0x14]
    adds r0, r6, #0
    ands r0, r1
    strh r0, [r7, #0x14]
    ldrb r1, [r7, #0x15]
    adds r0, r5, #0
    ands r0, r1
    ands r0, r4
    ands r0, r3
    strb r0, [r7, #0x15]
    ldr r0, [r7, #0x14]
    mov r1, r8
    ands r0, r1
    str r0, [r7, #0x14]
    ldrb r1, [r7, #0x16]
    adds r0, r5, #0
    ands r0, r1
    ands r0, r4
    ands r0, r3
    strb r0, [r7, #0x16]
    ldrh r1, [r7, #0x16]
    adds r0, r6, #0
    ands r0, r1
    strh r0, [r7, #0x16]
    ldrb r1, [r7, #0x17]
    adds r0, r5, #0
    ands r0, r1
    ands r0, r4
    ands r0, r3
    ands r0, r2
    strb r0, [r7, #0x17]
    ldrb r1, [r7, #0x18]
    movs r0, #2
    rsbs r0, r0, #0
    ands r0, r1
    ands r0, r5
    ands r0, r4
    ands r0, r3
    strb r0, [r7, #0x18]
    ldrh r1, [r7, #0x18]
    adds r0, r6, #0
    ands r0, r1
    strh r0, [r7, #0x18]
    ldrb r1, [r7, #0x19]
    adds r0, r5, #0
    ands r0, r1
    ands r0, r4
    ands r0, r3
    strb r0, [r7, #0x19]
    ldr r0, [r7, #0x18]
    mov r2, r8
    ands r0, r2
    str r0, [r7, #0x18]
    ldrb r1, [r7, #0x1a]
    adds r0, r5, #0
    ands r0, r1
    ands r0, r4
    ands r0, r3
    strb r0, [r7, #0x1a]
    ldrh r1, [r7, #0x1a]
    adds r0, r6, #0
    ands r0, r1
    strh r0, [r7, #0x1a]
    ldrb r1, [r7, #0x1b]
    adds r0, r5, #0
    ands r0, r1
    ands r0, r4
    ands r0, r3
    movs r1, #0x7f
    ands r0, r1
    strb r0, [r7, #0x1b]
    ldrb r1, [r7, #0x1c]
    movs r0, #2
    rsbs r0, r0, #0
    ands r0, r1
    ands r0, r5
    ands r0, r4
    ands r0, r3
    strb r0, [r7, #0x1c]
    ldrh r1, [r7, #0x1c]
    adds r0, r6, #0
    ands r0, r1
    strh r0, [r7, #0x1c]
    ldrb r1, [r7, #0x1d]
    adds r0, r5, #0
    ands r0, r1
    ands r0, r4
    ands r0, r3
    strb r0, [r7, #0x1d]
    ldr r0, [r7, #0x1c]
    ands r0, r2
    str r0, [r7, #0x1c]
    ldrb r1, [r7, #0x1e]
    adds r0, r5, #0
    ands r0, r1
    ands r0, r4
    ands r0, r3
    strb r0, [r7, #0x1e]
    ldrh r1, [r7, #0x1e]
    adds r0, r6, #0
    ands r0, r1
    strh r0, [r7, #0x1e]
    ldrb r1, [r7, #0x1f]
    adds r0, r5, #0
    ands r0, r1
    ands r0, r4
    ands r0, r3
    movs r2, #0x7f
    ands r0, r2
    strb r0, [r7, #0x1f]
    adds r2, r7, #0
    adds r2, #0x20
    ldrb r1, [r2]
    movs r0, #2
    rsbs r0, r0, #0
    ands r0, r1
    ands r0, r5
    ands r0, r4
    ands r0, r3
    strb r0, [r2]
    ldrh r1, [r7, #0x20]
    adds r0, r6, #0
    ands r0, r1
    strh r0, [r7, #0x20]
    adds r2, #1
    ldrb r1, [r2]
    adds r0, r5, #0
    ands r0, r1
    ands r0, r4
    ands r0, r3
    strb r0, [r2]
    ldr r0, [r7, #0x20]
    mov r1, r8
    ands r0, r1
    str r0, [r7, #0x20]
    adds r2, #1
    ldrb r1, [r2]
    adds r0, r5, #0
    ands r0, r1
    ands r0, r4
    movs r1, #0x21
    rsbs r1, r1, #0
    ands r0, r1
    subs r1, #0x20
    ands r0, r1
    strb r0, [r2]
    adds r2, #1
    ldrb r1, [r2]
    movs r0, #0xf
    ands r0, r1
    strb r0, [r2]
    adds r2, #1
    ldrb r1, [r2]
    movs r0, #8
    rsbs r0, r0, #0
    ands r0, r1
    ands r0, r4
    ands r0, r3
    strb r0, [r2]
    ldrh r1, [r7, #0x24]
    adds r0, r6, #0
    ands r0, r1
    strh r0, [r7, #0x24]
    adds r2, #1
    ldrb r1, [r2]
    adds r0, r5, #0
    ands r0, r1
    ands r0, r4
    ands r0, r3
    movs r1, #0x7f
    ands r0, r1
    strb r0, [r2]
    adds r1, r7, #0
    adds r1, #0x26
    movs r2, #4
    rsbs r2, r2, #0
    mov sl, r2
    movs r0, #0
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r2, r7, #0
    adds r2, #0x2a
    ldrb r1, [r2]
    adds r0, r5, #0
    ands r0, r1
    ands r0, r4
    ands r0, r3
    strb r0, [r2]
    ldrh r1, [r7, #0x2a]
    adds r0, r6, #0
    ands r0, r1
    strh r0, [r7, #0x2a]
    adds r2, #1
    ldrb r1, [r2]
    adds r0, r5, #0
    ands r0, r1
    movs r1, #9
    rsbs r1, r1, #0
    ands r0, r1
    subs r1, #0x28
    ands r0, r1
    movs r1, #0x3f
    ands r0, r1
    strb r0, [r2]
    adds r1, r7, #0
    adds r1, #0x2c
    movs r0, #0
    strb r0, [r1]
    adds r2, #2
    ldrb r1, [r2]
    mov r0, sl
    ands r0, r1
    movs r1, #0xd
    rsbs r1, r1, #0
    ands r0, r1
    subs r1, #0x24
    ands r0, r1
    strb r0, [r2]
    ldr r0, [r7, #0x2c]
    ldr r2, .L0809CAC4 @ =0xFFFC3FFF
    mov ip, r2
    ands r0, r2
    str r0, [r7, #0x2c]
    adds r2, r7, #0
    adds r2, #0x2e
    ldrb r1, [r2]
    movs r0, #5
    rsbs r0, r0, #0
    ands r0, r1
    ands r0, r4
    ands r0, r3
    movs r1, #0x7f
    ands r0, r1
    strb r0, [r2]
    adds r1, r7, #0
    adds r1, #0x2f
    movs r0, #0
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r2, #8
    ldrb r1, [r2]
    mov r0, sl
    ands r0, r1
    movs r1, #5
    rsbs r1, r1, #0
    ands r0, r1
    ands r0, r4
    ands r0, r3
    strb r0, [r2]
    ldrh r1, [r7, #0x36]
    adds r0, r6, #0
    ands r0, r1
    strh r0, [r7, #0x36]
    adds r2, #1
    ldrb r1, [r2]
    adds r0, r5, #0
    ands r0, r1
    ands r0, r4
    ands r0, r3
    movs r1, #0x7f
    ands r0, r1
    strb r0, [r2]
    adds r1, r7, #0
    adds r1, #0x38
    movs r0, #0
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r2, #3
    ldrb r1, [r2]
    mov r0, sl
    ands r0, r1
    movs r1, #0x1d
    rsbs r1, r1, #0
    ands r0, r1
    ands r0, r3
    strb r0, [r2]
    ldrh r1, [r7, #0x3a]
    adds r0, r6, #0
    ands r0, r1
    strh r0, [r7, #0x3a]
    adds r2, #1
    ldrb r1, [r2]
    adds r0, r5, #0
    ands r0, r1
    ands r0, r4
    ands r0, r3
    movs r1, #0x7f
    ands r0, r1
    strb r0, [r2]
    adds r2, #1
    ldrb r1, [r2]
    movs r0, #2
    rsbs r0, r0, #0
    ands r0, r1
    ands r0, r5
    ands r0, r4
    ands r0, r3
    strb r0, [r2]
    ldrh r1, [r7, #0x3c]
    ldr r0, .L0809CAC8 @ =0xFFFFFC7F
    ands r0, r1
    strh r0, [r7, #0x3c]
    adds r2, #1
    ldrb r1, [r2]
    movs r0, #0xd
    rsbs r0, r0, #0
    ands r0, r1
    movs r1, #0x31
    rsbs r1, r1, #0
    ands r0, r1
    strb r0, [r2]
    ldr r0, [r7, #0x3c]
    mov r2, ip
    ands r0, r2
    str r0, [r7, #0x3c]
    adds r2, r7, #0
    adds r2, #0x3e
    ldrb r1, [r2]
    movs r0, #0xd
    rsbs r0, r0, #0
    ands r0, r1
    movs r1, #0x31
    rsbs r1, r1, #0
    ands r0, r1
    movs r1, #0x3f
    ands r0, r1
    strb r0, [r2]
    adds r1, r7, #0
    adds r1, #0x3f
    movs r0, #0
    strb r0, [r1]
    adds r2, #2
    ldrb r1, [r2]
    subs r0, #2
    ands r0, r1
    ands r0, r5
    ands r0, r4
    ands r0, r3
    strb r0, [r2]
    ldrh r1, [r2]
    adds r0, r6, #0
    ands r0, r1
    strh r0, [r2]
    adds r2, #1
    ldrb r1, [r2]
    adds r0, r5, #0
    ands r0, r1
    ands r0, r4
    strb r0, [r2]
    ldr r0, [r7, #0x40]
    ldr r1, .L0809CACC @ =0xFFFE1FFF
    ands r0, r1
    str r0, [r7, #0x40]
    adds r2, #1
    ldrb r1, [r2]
    adds r0, r5, #0
    ands r0, r1
    ands r0, r4
    movs r1, #0x21
    rsbs r1, r1, #0
    ands r0, r1
    movs r1, #0x3f
    ands r0, r1
    strb r0, [r2]
    adds r1, r7, #0
    adds r1, #0x43
    movs r0, #0
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r2, #0xb
    ldrb r1, [r2]
    mov r0, sl
    ands r0, r1
    movs r1, #5
    rsbs r1, r1, #0
    ands r0, r1
    subs r1, #4
    ands r0, r1
    subs r1, #8
    ands r0, r1
    subs r1, #0x10
    ands r0, r1
    subs r1, #0x20
    ands r0, r1
    strb r0, [r2]
    ldr r0, [r7, #0x4c]
    mov r2, r8
    ands r0, r2
    str r0, [r7, #0x4c]
    adds r2, r7, #0
    adds r2, #0x4e
    ldrb r1, [r2]
    mov r0, sb
    ands r0, r1
    movs r1, #0xd
    rsbs r1, r1, #0
    ands r0, r1
    subs r1, #4
    ands r0, r1
    ands r0, r3
    strb r0, [r2]
    ldrh r1, [r2]
    adds r0, r6, #0
    ands r0, r1
    b .L0809CAD0
    .align 2, 0
.L0809CAB8: .4byte 0x0000FFFF
.L0809CABC: .4byte 0xFFFE7FFF
.L0809CAC0: .4byte 0xFFFFFE7F
.L0809CAC4: .4byte 0xFFFC3FFF
.L0809CAC8: .4byte 0xFFFFFC7F
.L0809CACC: .4byte 0xFFFE1FFF
.L0809CAD0:
    strh r0, [r2]
    adds r2, #1
    ldrb r1, [r2]
    movs r0, #0xf
    rsbs r0, r0, #0
    ands r0, r1
    movs r1, #0x31
    rsbs r1, r1, #0
    ands r0, r1
    subs r1, #0x10
    ands r0, r1
    movs r1, #0x7f
    ands r0, r1
    strb r0, [r2]
    adds r1, r7, #0
    adds r1, #0x50
    movs r0, #0
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r2, #3
    ldrb r1, [r2]
    subs r0, #2
    ands r0, r1
    ands r0, r5
    movs r1, #9
    rsbs r1, r1, #0
    ands r0, r1
    subs r1, #0x28
    ands r0, r1
    strb r0, [r2]
    ldrh r1, [r2]
    ldr r0, .L0809CD5C @ =0xFFFFFE3F
    ands r0, r1
    strh r0, [r2]
    adds r2, #1
    ldrb r1, [r2]
    adds r0, r5, #0
    ands r0, r1
    movs r1, #9
    rsbs r1, r1, #0
    ands r0, r1
    subs r1, #0x28
    ands r0, r1
    movs r1, #0x3f
    ands r0, r1
    strb r0, [r2]
    adds r1, r7, #0
    adds r1, #0x54
    movs r0, #0
    strb r0, [r1]
    adds r1, #1
    movs r2, #0x3f
    mov r8, r2
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r2, r7, #0
    adds r2, #0x59
    ldrb r1, [r2]
    mov r0, sl
    ands r0, r1
    movs r1, #5
    rsbs r1, r1, #0
    ands r0, r1
    ands r0, r4
    ands r0, r3
    strb r0, [r2]
    ldr r0, [r7, #0x58]
    ldr r1, .L0809CD60 @ =0xFFFC7FFF
    ands r0, r1
    str r0, [r7, #0x58]
    adds r1, r7, #0
    adds r1, #0x5a
    ldrh r0, [r1]
    ands r6, r0
    strh r6, [r1]
    adds r2, #2
    ldrb r1, [r2]
    adds r0, r5, #0
    ands r0, r1
    ands r0, r4
    ands r0, r3
    movs r1, #0x7f
    ands r0, r1
    strb r0, [r2]
    adds r1, r7, #0
    adds r1, #0x5c
    movs r0, #0
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r1, #1
    ldrb r0, [r1]
    mov r2, sl
    ands r2, r0
    movs r0, #0xd
    rsbs r0, r0, #0
    ands r2, r0
    subs r0, #0x24
    ands r2, r0
    strb r2, [r1]
    ldrh r2, [r1]
    ldr r3, .L0809CD64 @ =0xFFFFC03F
    adds r0, r3, #0
    ands r0, r2
    strh r0, [r1]
    ldr r0, [r7, #0x68]
    ldr r6, .L0809CD68 @ =0xFFC03FFF
    ands r0, r6
    str r0, [r7, #0x68]
    adds r2, r7, #0
    adds r2, #0x6a
    ldrh r1, [r2]
    adds r0, r3, #0
    ands r0, r1
    strh r0, [r2]
    adds r2, #1
    ldrb r1, [r2]
    mov r0, r8
    ands r0, r1
    strb r0, [r2]
    adds r1, r7, #0
    adds r1, #0x6c
    ldrb r2, [r1]
    subs r4, #0x27
    adds r0, r4, #0
    ands r0, r2
    strb r0, [r1]
    ldrh r2, [r1]
    adds r0, r3, #0
    ands r0, r2
    strh r0, [r1]
    ldr r0, [r7, #0x6c]
    ands r0, r6
    str r0, [r7, #0x6c]
    adds r2, r7, #0
    adds r2, #0x6e
    ldrh r1, [r2]
    adds r0, r3, #0
    ands r0, r1
    strh r0, [r2]
    adds r2, #1
    ldrb r1, [r2]
    mov r0, r8
    ands r0, r1
    strb r0, [r2]
    adds r1, r7, #0
    adds r1, #0x70
    ldrb r2, [r1]
    adds r0, r4, #0
    ands r0, r2
    strb r0, [r1]
    ldrh r2, [r1]
    adds r0, r3, #0
    ands r0, r2
    strh r0, [r1]
    ldr r0, [r7, #0x70]
    ands r0, r6
    str r0, [r7, #0x70]
    adds r1, #2
    ldrh r0, [r1]
    ands r3, r0
    strh r3, [r1]
    adds r1, #1
    ldrb r0, [r1]
    mov r2, r8
    ands r2, r0
    strb r2, [r1]
    adds r1, #1
    ldrb r0, [r1]
    ands r4, r0
    strb r4, [r1]
    ldrh r2, [r1]
    ldr r0, .L0809CD5C @ =0xFFFFFE3F
    ands r0, r2
    strh r0, [r1]
    adds r1, #1
    ldrb r0, [r1]
    ands r5, r0
    movs r0, #9
    rsbs r0, r0, #0
    ands r5, r0
    movs r2, #0x11
    rsbs r2, r2, #0
    ands r5, r2
    subs r0, #0x18
    ands r5, r0
    subs r2, #0x30
    ands r5, r2
    movs r0, #0x7f
    ands r5, r0
    strb r5, [r1]
    adds r2, r7, #0
    adds r2, #0x76
    ldrb r1, [r2]
    subs r0, #0x81
    ands r0, r1
    mov r1, sb
    ands r0, r1
    movs r1, #5
    rsbs r1, r1, #0
    ands r0, r1
    subs r1, #4
    ands r0, r1
    subs r1, #8
    ands r0, r1
    subs r1, #0x10
    ands r0, r1
    subs r1, #0x20
    ands r0, r1
    strb r0, [r2]
    ldrh r1, [r2]
    ldr r0, .L0809CD6C @ =0xFFFFF87F
    ands r0, r1
    strh r0, [r2]
    adds r2, #1
    ldrb r1, [r2]
    movs r0, #0x79
    rsbs r0, r0, #0
    ands r0, r1
    movs r1, #0x7f
    ands r0, r1
    strb r0, [r2]
    adds r1, r7, #0
    adds r1, #0x78
    movs r0, #0
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    adds r2, #3
    ldrb r0, [r2]
    movs r1, #2
    rsbs r1, r1, #0
    ands r1, r0
    strb r1, [r2]
    ldrh r1, [r2]
    ldr r0, .L0809CD70 @ =0xFFFFFE01
    ands r0, r1
    strh r0, [r2]
    adds r1, r7, #0
    adds r1, #0x7b
    ldrb r0, [r1]
    mov r2, sb
    ands r2, r0
    movs r0, #5
    rsbs r0, r0, #0
    ands r2, r0
    movs r6, #9
    rsbs r6, r6, #0
    ands r2, r6
    movs r5, #0x11
    rsbs r5, r5, #0
    ands r2, r5
    movs r4, #0x21
    rsbs r4, r4, #0
    ands r2, r4
    movs r3, #0x41
    rsbs r3, r3, #0
    ands r2, r3
    movs r0, #0x7f
    ands r2, r0
    strb r2, [r1]
    adds r1, #1
    movs r2, #5
    rsbs r2, r2, #0
    movs r0, #0
    strb r0, [r1]
    adds r1, #1
    strb r0, [r1]
    movs r1, #0x7e
    adds r1, r1, r7
    mov r8, r1
    ldrb r1, [r1]
    subs r0, #4
    ands r0, r1
    ands r0, r2
    ands r0, r6
    ands r0, r5
    ands r0, r4
    ands r0, r3
    mov r2, r8
    strb r0, [r2]
    ldrh r1, [r2]
    ldr r0, .L0809CD74 @ =0xFFFFFC7F
    ands r0, r1
    strh r0, [r2]
    adds r2, r7, #0
    adds r2, #0x7f
    ldrb r1, [r2]
    movs r0, #3
    ands r0, r1
    strb r0, [r2]
    adds r0, r7, #0
    add sp, #4
    pop {r3, r4, r5}
    mov r8, r3
    mov sb, r4
    mov sl, r5
    pop {r4, r5, r6, r7}
    pop {r1}
    bx r1
    .align 2, 0
.L0809CD5C: .4byte 0xFFFFFE3F
.L0809CD60: .4byte 0xFFFC7FFF
.L0809CD64: .4byte 0xFFFFC03F
.L0809CD68: .4byte 0xFFC03FFF
.L0809CD6C: .4byte 0xFFFFF87F
.L0809CD70: .4byte 0xFFFFFE01
.L0809CD74: .4byte 0xFFFFFC7F

	.section .text.0809DA00, "ax", %progbits
    thumb_func_start ResolveMineOutcome
ResolveMineOutcome: @ 0x0809DA00
    push {r4, r5, r6, r7, lr}
    mov r7, r8
    push {r7}
    sub sp, #0x10
    adds r5, r0, #0
    mov r8, r1
    adds r6, r2, #0
    mov r0, r8
    subs r0, #3
    cmp r0, #0x1f
    bls .L0809DA18
    b .L0809DF1C
.L0809DA18:
    lsls r0, r0, #2
    ldr r1, .L0809DA24 @ =.L0809DA28
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L0809DA24: .4byte .L0809DA28
.L0809DA28: @ jump table
    .4byte .L0809DAA8 @ case 0
    .4byte .L0809DB02 @ case 1
    .4byte .L0809DB02 @ case 2
    .4byte .L0809DB02 @ case 3
    .4byte .L0809DB02 @ case 4
    .4byte .L0809DB02 @ case 5
    .4byte .L0809DB02 @ case 6
    .4byte .L0809DB9C @ case 7
    .4byte .L0809DF1C @ case 8
    .4byte .L0809DF1C @ case 9
    .4byte .L0809DF1C @ case 10
    .4byte .L0809DF1C @ case 11
    .4byte .L0809DF1C @ case 12
    .4byte .L0809DF1C @ case 13
    .4byte .L0809DF1C @ case 14
    .4byte .L0809DF1C @ case 15
    .4byte .L0809DF1C @ case 16
    .4byte .L0809DF1C @ case 17
    .4byte .L0809DF1C @ case 18
    .4byte .L0809DF1C @ case 19
    .4byte .L0809DBFC @ case 20
    .4byte .L0809DF1C @ case 21
    .4byte .L0809DF1C @ case 22
    .4byte .L0809DF1C @ case 23
    .4byte .L0809DF1C @ case 24
    .4byte .L0809DF1C @ case 25
    .4byte .L0809DF1C @ case 26
    .4byte .L0809DF1C @ case 27
    .4byte .L0809DF1C @ case 28
    .4byte .L0809DC36 @ case 29
    .4byte .L0809DDA6 @ case 30
    .4byte .L0809DEFC @ case 31
.L0809DAA8:
    ldr r0, .L0809DAD4 @ =0x00001BD8
    adds r1, r6, r0
    mov r0, sp
    bl func_0800E924
    mov r0, sp
    ldrh r0, [r0]
    lsls r0, r0, #0x16
    ldr r1, .L0809DAD8 @ =0x4CC00000
    cmp r0, r1
    bgt .L0809DAE0
    ldr r1, .L0809DADC @ =0x00000625
    adds r2, r5, r1
    ldrb r1, [r2]
    movs r0, #4
    ands r0, r1
    cmp r0, #0
    beq .L0809DACE
    b .L0809DC30
.L0809DACE:
    movs r0, #4
    b .L0809DAF0
    .align 2, 0
.L0809DAD4: .4byte 0x00001BD8
.L0809DAD8: .4byte 0x4CC00000
.L0809DADC: .4byte 0x00000625
.L0809DAE0:
    ldr r0, .L0809DAF8 @ =0x00000624
    adds r2, r5, r0
    ldrb r1, [r2]
    movs r0, #1
    ands r0, r1
    cmp r0, #0
    bne .L0809DAFC
    movs r0, #1
.L0809DAF0:
    orrs r0, r1
    strb r0, [r2]
    b .L0809DF1C
    .align 2, 0
.L0809DAF8: .4byte 0x00000624
.L0809DAFC:
    movs r1, #0
    mov r8, r1
    b .L0809DF1C
.L0809DB02:
    movs r4, #0
    ldr r2, .L0809DB50 @ =0x00001C38
    adds r0, r6, r2
    bl GetFirstFreeToolSlot__C8Rucksack
    movs r1, #1
    rsbs r1, r1, #0
    cmp r0, r1
    bne .L0809DB16
    movs r4, #1
.L0809DB16:
    ldr r1, .L0809DB54 @ =0x00001DAC
    adds r0, r6, r1
    bl HasCompletedAllNpcMilestones
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0809DB26
    movs r4, #1
.L0809DB26:
    adds r0, r5, #0
    mov r1, r8
    adds r2, r6, #0
    bl HasCursedMineTool
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0809DB38
    movs r4, #1
.L0809DB38:
    cmp r4, #1
    beq .L0809DC30
    movs r1, #0x51
    mov r0, r8
    subs r0, #4
    cmp r0, #5
    bhi .L0809DB8A
    lsls r0, r0, #2
    ldr r1, .L0809DB58 @ =.L0809DB5C
    adds r0, r0, r1
    ldr r0, [r0]
    mov pc, r0
    .align 2, 0
.L0809DB50: .4byte 0x00001C38
.L0809DB54: .4byte 0x00001DAC
.L0809DB58: .4byte .L0809DB5C
.L0809DB5C: @ jump table
    .4byte .L0809DB74 @ case 0
    .4byte .L0809DB78 @ case 1
    .4byte .L0809DB7C @ case 2
    .4byte .L0809DB80 @ case 3
    .4byte .L0809DB84 @ case 4
    .4byte .L0809DB88 @ case 5
.L0809DB74:
    movs r1, #5
    b .L0809DB8A
.L0809DB78:
    movs r1, #0xd
    b .L0809DB8A
.L0809DB7C:
    movs r1, #0x15
    b .L0809DB8A
.L0809DB80:
    movs r1, #0x1d
    b .L0809DB8A
.L0809DB84:
    movs r1, #0x25
    b .L0809DB8A
.L0809DB88:
    movs r1, #0x2d
.L0809DB8A:
    cmp r1, #0x51
    bne .L0809DB90
    b .L0809DF1C
.L0809DB90:
    movs r2, #0xd2
    lsls r2, r2, #6
    adds r0, r6, r2
    bl StartCursedToolProgress
    b .L0809DF1C
.L0809DB9C:
    movs r7, #0
    movs r3, #0xc8
    str r3, [sp, #8]
    ldrb r0, [r6, #0x10]
    lsrs r2, r0, #3
    lsls r1, r2, #3
    subs r1, r1, r2
    lsls r0, r0, #0x1d
    lsrs r0, r0, #0x1d
    adds r1, r1, r0
    str r1, [sp, #0xc]
    add r2, sp, #0xc
    add r0, sp, #8
    cmp r3, r1
    bls .L0809DBBC
    adds r0, r2, #0
.L0809DBBC:
    ldr r0, [r0]
    cmp r0, #2
    bhi .L0809DBC4
    movs r7, #1
.L0809DBC4:
    ldr r0, .L0809DBF4 @ =0x00000625
    adds r4, r5, r0
    ldrb r1, [r4]
    movs r0, #0x10
    ands r0, r1
    cmp r0, #0
    beq .L0809DBD4
    movs r7, #1
.L0809DBD4:
    ldr r1, .L0809DBF8 @ =0x00001C38
    adds r0, r6, r1
    bl GetFirstFreeToolSlot__C8Rucksack
    movs r1, #1
    rsbs r1, r1, #0
    cmp r0, r1
    bne .L0809DBE6
    movs r7, #1
.L0809DBE6:
    cmp r7, #0
    bne .L0809DC30
    ldrb r0, [r4]
    movs r1, #0x10
    orrs r0, r1
    strb r0, [r4]
    b .L0809DF1C
    .align 2, 0
.L0809DBF4: .4byte 0x00000625
.L0809DBF8: .4byte 0x00001C38
.L0809DBFC:
    movs r7, #0
    movs r1, #0xd2
    lsls r1, r1, #6
    adds r0, r6, r1
    bl HaveAllCursedToolsCompleted
    lsls r0, r0, #0x18
    cmp r0, #0
    bne .L0809DC10
    movs r7, #1
.L0809DC10:
    movs r4, #4
.L0809DC12:
    adds r0, r5, #0
    adds r1, r4, #0
    adds r2, r6, #0
    bl HasCursedMineTool
    lsls r0, r0, #0x18
    cmp r0, #0
    beq .L0809DC24
    movs r7, #1
.L0809DC24:
    adds r4, #1
    cmp r4, #9
    ble .L0809DC12
    cmp r7, #1
    beq .L0809DC30
    b .L0809DF1C
.L0809DC30:
    movs r2, #0
    mov r8, r2
    b .L0809DF1C
.L0809DC36:
    movs r4, #0
    movs r1, #0xfa
    lsls r1, r1, #1
    adds r0, r6, r1
    bl GetShelf__9FarmHouse
    cmp r0, #0
    bne .L0809DC48
    movs r4, #1
.L0809DC48:
    adds r0, r5, #0
    bl CountHarvestGoddessJewels
    lsls r0, r0, #0x18
    lsrs r0, r0, #0x18
    cmp r0, #8
    bne .L0809DC68
    ldr r2, .L0809DC98 @ =0x00001C38
    adds r0, r6, r2
    bl GetFirstFreeToolSlot__C8Rucksack
    movs r1, #1
    rsbs r1, r1, #0
    cmp r0, r1
    bne .L0809DC68
    movs r4, #1
.L0809DC68:
    cmp r4, #0
    beq .L0809DC6E
    b .L0809DD9E
.L0809DC6E:
    ldr r0, .L0809DC9C @ =0x00001BD8
    adds r1, r6, r0
    mov r0, sp
    bl func_0800E924
    mov r0, sp
    ldrh r0, [r0]
    lsls r0, r0, #0x16
    lsrs r0, r0, #0x16
    subs r0, #0x34
    cmp r0, #0x9b
    beq .L0809DD20
    cmp r0, #0x9b
    bgt .L0809DCAA
    cmp r0, #0x66
    beq .L0809DCD8
    cmp r0, #0x66
    bgt .L0809DCA0
    cmp r0, #0x3c
    beq .L0809DCC2
    b .L0809DD9C
    .align 2, 0
.L0809DC98: .4byte 0x00001C38
.L0809DC9C: .4byte 0x00001BD8
.L0809DCA0:
    cmp r0, #0x7b
    beq .L0809DCF0
    cmp r0, #0x98
    beq .L0809DD08
    b .L0809DD9C
.L0809DCAA:
    cmp r0, #0xbe
    beq .L0809DD50
    cmp r0, #0xbe
    bgt .L0809DCB8
    cmp r0, #0xab
    beq .L0809DD38
    b .L0809DD9C
.L0809DCB8:
    cmp r0, #0xca
    beq .L0809DD68
    cmp r0, #0xde
    beq .L0809DD80
    b .L0809DD9C
.L0809DCC2:
    ldr r2, .L0809DCD4 @ =0x00000625
    adds r1, r5, r2
    ldrb r2, [r1]
    movs r0, #0x20
    ands r0, r2
    cmp r0, #0
    beq .L0809DD8E
    b .L0809DD9C
    .align 2, 0
.L0809DCD4: .4byte 0x00000625
.L0809DCD8:
    ldr r0, .L0809DCEC @ =0x00000625
    adds r1, r5, r0
    ldrb r2, [r1]
    movs r0, #0x40
    ands r0, r2
    cmp r0, #0
    bne .L0809DD9C
    movs r0, #0x40
    b .L0809DD90
    .align 2, 0
.L0809DCEC: .4byte 0x00000625
.L0809DCF0:
    ldr r2, .L0809DD04 @ =0x00000625
    adds r1, r5, r2
    ldrb r2, [r1]
    movs r0, #0x80
    ands r0, r2
    cmp r0, #0
    bne .L0809DD9C
    movs r0, #0x80
    b .L0809DD90
    .align 2, 0
.L0809DD04: .4byte 0x00000625
.L0809DD08:
    ldr r0, .L0809DD1C @ =0x00000626
    adds r1, r5, r0
    ldrb r2, [r1]
    movs r0, #1
    ands r0, r2
    cmp r0, #0
    bne .L0809DD9C
    movs r0, #1
    b .L0809DD90
    .align 2, 0
.L0809DD1C: .4byte 0x00000626
.L0809DD20:
    ldr r2, .L0809DD34 @ =0x00000626
    adds r1, r5, r2
    ldrb r2, [r1]
    movs r0, #2
    ands r0, r2
    cmp r0, #0
    bne .L0809DD9C
    movs r0, #2
    b .L0809DD90
    .align 2, 0
.L0809DD34: .4byte 0x00000626
.L0809DD38:
    ldr r0, .L0809DD4C @ =0x00000626
    adds r1, r5, r0
    ldrb r2, [r1]
    movs r0, #4
    ands r0, r2
    cmp r0, #0
    bne .L0809DD9C
    movs r0, #4
    b .L0809DD90
    .align 2, 0
.L0809DD4C: .4byte 0x00000626
.L0809DD50:
    ldr r2, .L0809DD64 @ =0x00000626
    adds r1, r5, r2
    ldrb r2, [r1]
    movs r0, #8
    ands r0, r2
    cmp r0, #0
    bne .L0809DD9C
    movs r0, #8
    b .L0809DD90
    .align 2, 0
.L0809DD64: .4byte 0x00000626
.L0809DD68:
    ldr r0, .L0809DD7C @ =0x00000626
    adds r1, r5, r0
    ldrb r2, [r1]
    movs r0, #0x10
    ands r0, r2
    cmp r0, #0
    bne .L0809DD9C
    movs r0, #0x10
    b .L0809DD90
    .align 2, 0
.L0809DD7C: .4byte 0x00000626
.L0809DD80:
    ldr r2, .L0809DD98 @ =0x00000626
.L0809DD82:
    adds r1, r5, r2
    ldrb r2, [r1]
    movs r0, #0x20
    ands r0, r2
    cmp r0, #0
    bne .L0809DD9C
.L0809DD8E:
    movs r0, #0x20
.L0809DD90:
    orrs r0, r2
    strb r0, [r1]
    b .L0809DD9E
    .align 2, 0
.L0809DD98: .4byte 0x00000626
.L0809DD9C:
    movs r4, #1
.L0809DD9E:
    cmp r4, #1
    beq .L0809DDA4
    b .L0809DF1C
.L0809DDA4:
    b .L0809DF18
.L0809DDA6:
    movs r4, #0
    movs r1, #0xfa
    lsls r1, r1, #1
    adds r0, r6, r1
    bl GetShelf__9FarmHouse
    cmp r0, #0
    bne .L0809DDB8
    movs r4, #1
.L0809DDB8:
    adds r0, r5, #0
    bl CountKappaJewels
    lsls r0, r0, #0x18
    lsrs r0, r0, #0x18
    cmp r0, #8
    bne .L0809DDD8
    ldr r2, .L0809DE08 @ =0x00001C38
    adds r0, r6, r2
    bl GetFirstFreeToolSlot__C8Rucksack
    movs r1, #1
    rsbs r1, r1, #0
    cmp r0, r1
    bne .L0809DDD8
    movs r4, #1
.L0809DDD8:
    cmp r4, #0
    bne .L0809DD9E
    ldr r0, .L0809DE0C @ =0x00001BD8
    adds r1, r6, r0
    mov r0, sp
    bl func_0800E924
    mov r0, sp
    ldrh r0, [r0]
    lsls r0, r0, #0x16
    lsrs r0, r0, #0x16
    ldr r1, .L0809DE10 @ =0xFFFFFECC
    adds r0, r0, r1
    cmp r0, #0x78
    beq .L0809DE94
    cmp r0, #0x78
    bgt .L0809DE1E
    cmp r0, #0x28
    beq .L0809DE4C
    cmp r0, #0x28
    bgt .L0809DE14
    cmp r0, #0
    beq .L0809DE36
    b .L0809DD9C
    .align 2, 0
.L0809DE08: .4byte 0x00001C38
.L0809DE0C: .4byte 0x00001BD8
.L0809DE10: .4byte 0xFFFFFECC
.L0809DE14:
    cmp r0, #0x3c
    beq .L0809DE64
    cmp r0, #0x50
    beq .L0809DE7C
    b .L0809DD9C
.L0809DE1E:
    cmp r0, #0xa0
    beq .L0809DEB4
    cmp r0, #0xa0
    bgt .L0809DE2C
    cmp r0, #0x8c
    beq .L0809DE9C
    b .L0809DD9C
.L0809DE2C:
    cmp r0, #0xb4
    beq .L0809DECC
    cmp r0, #0xff
    beq .L0809DEE4
    b .L0809DD9C
.L0809DE36:
    ldr r2, .L0809DE48 @ =0x00000624
    adds r1, r5, r2
    ldrb r2, [r1]
    movs r0, #2
    ands r0, r2
    cmp r0, #0
    beq .L0809DEF4
    b .L0809DD9C
    .align 2, 0
.L0809DE48: .4byte 0x00000624
.L0809DE4C:
    ldr r0, .L0809DE60 @ =0x00000624
    adds r1, r5, r0
    ldrb r2, [r1]
    movs r0, #4
    ands r0, r2
    cmp r0, #0
    bne .L0809DD9C
    movs r0, #4
    b .L0809DD90
    .align 2, 0
.L0809DE60: .4byte 0x00000624
.L0809DE64:
    ldr r2, .L0809DE78 @ =0x00000624
    adds r1, r5, r2
    ldrb r2, [r1]
    movs r0, #8
    ands r0, r2
    cmp r0, #0
    bne .L0809DD9C
    movs r0, #8
    b .L0809DD90
    .align 2, 0
.L0809DE78: .4byte 0x00000624
.L0809DE7C:
    ldr r0, .L0809DE90 @ =0x00000624
    adds r1, r5, r0
    ldrb r2, [r1]
    movs r0, #0x10
    ands r0, r2
    cmp r0, #0
    bne .L0809DD9C
    movs r0, #0x10
    b .L0809DD90
    .align 2, 0
.L0809DE90: .4byte 0x00000624
.L0809DE94:
    ldr r2, .L0809DE98 @ =0x00000624
    b .L0809DD82
    .align 2, 0
.L0809DE98: .4byte 0x00000624
.L0809DE9C:
    ldr r0, .L0809DEB0 @ =0x00000624
    adds r1, r5, r0
    ldrb r2, [r1]
    movs r0, #0x40
    ands r0, r2
    cmp r0, #0
    beq .L0809DEAC
    b .L0809DD9C
.L0809DEAC:
    movs r0, #0x40
    b .L0809DD90
    .align 2, 0
.L0809DEB0: .4byte 0x00000624
.L0809DEB4:
    ldr r2, .L0809DEC8 @ =0x00000624
    adds r1, r5, r2
    ldrb r2, [r1]
    movs r0, #0x80
    ands r0, r2
    cmp r0, #0
    beq .L0809DEC4
    b .L0809DD9C
.L0809DEC4:
    movs r0, #0x80
    b .L0809DD90
    .align 2, 0
.L0809DEC8: .4byte 0x00000624
.L0809DECC:
    ldr r0, .L0809DEE0 @ =0x00000625
    adds r1, r5, r0
    ldrb r2, [r1]
    movs r0, #1
    ands r0, r2
    cmp r0, #0
    beq .L0809DEDC
    b .L0809DD9C
.L0809DEDC:
    movs r0, #1
    b .L0809DD90
    .align 2, 0
.L0809DEE0: .4byte 0x00000625
.L0809DEE4:
    ldr r2, .L0809DEF8 @ =0x00000625
    adds r1, r5, r2
    ldrb r2, [r1]
    movs r0, #2
    ands r0, r2
    cmp r0, #0
    beq .L0809DEF4
    b .L0809DD9C
.L0809DEF4:
    movs r0, #2
    b .L0809DD90
    .align 2, 0
.L0809DEF8: .4byte 0x00000625
.L0809DEFC:
    ldr r2, .L0809DF14 @ =0x00000625
    adds r1, r5, r2
    ldrb r2, [r1]
    movs r0, #8
    ands r0, r2
    cmp r0, #0
    bne .L0809DF18
    movs r0, #8
    orrs r0, r2
    strb r0, [r1]
    b .L0809DF1C
    .align 2, 0
.L0809DF14: .4byte 0x00000625
.L0809DF18:
    movs r0, #0
    mov r8, r0
.L0809DF1C:
    mov r0, r8
    add sp, #0x10
    pop {r3}
    mov r8, r3
    pop {r4, r5, r6, r7}
    pop {r1}
    bx r1
    .align 2, 0
