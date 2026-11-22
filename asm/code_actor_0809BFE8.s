    .INCLUDE "asm/macro.inc"
    .SYNTAX UNIFIED

	.section .text.0809C6BC, "ax", %progbits
    thumb_func_start InitializeSavedScriptEvents
InitializeSavedScriptEvents: @ 0x0809C6BC
    @ Reset the pending count, selection and progress without clearing the pending IDs.
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
