    .syntax unified
    .include "build/assets/vietnamese-mode.inc"
    .if VIETNAMESE_FONT
    .section .vietnamese_font, "ax"
    .thumb
    .align 2
    .global gVietnameseGlyphLookup
    .thumb_func
gVietnameseGlyphLookup:
    adds r0, r0, r1
    movs r2, #0
    ldrsh r5, [r0, r2]
    movs r6, #2
    cmp r5, #0
    blt .LViReturn
    movs r0, #0x40
    lsls r0, r0, #8
    tst r5, r0
    beq .LViReturn
    bics r5, r0
    movs r6, #1
.LViReturn:
    ldr r0, .LViContinue
    bx r0
    .align 2
.LViContinue:
    .4byte 0x080D0E59
    .endif
