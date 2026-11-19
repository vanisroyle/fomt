    .INCLUDE "asm/macro.inc"
    .SYNTAX UNIFIED

	thumb_func_start func_080D035C
func_080D035C: @ 0x080D035C
	push {r4, r5, lr}
	ldr r5, .L080D039C @ =gUnk_03000420
	ldr r1, [r5]
	ldr r0, [r1]
	cmp r0, #0
	beq .L080D0436
	movs r2, #0xc2
	lsls r2, r2, #1
	adds r0, r1, r2
	ldr r0, [r0]
	str r0, [r1, #0x1c]
	ldr r0, .L080D03A0 @ =gUnk_0300041C
	ldr r0, [r0]
	ldr r0, [r0]
	cmp r0, #0
	bne .L080D03C2
	bl func_080D0C10
	cmp r0, #0
	bne .L080D03A4
	ldr r0, [r5]
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r0, r0, r1
	ldr r4, [r0]
	cmp r4, #0
	bne .L080D03A4
	bl func_080D0BE0
	ldr r0, [r5]
	str r4, [r0, #0x10]
	b .L080D03C2
	.align 2, 0
.L080D039C: .4byte gUnk_03000420
.L080D03A0: .4byte gUnk_0300041C
.L080D03A4:
	ldr r0, .L080D03F0 @ =gUnk_03000420
	ldr r3, [r0]
	ldr r0, [r3, #0x14]
	cmp r0, #0
	beq .L080D03C2
	movs r2, #0
	str r2, [r3, #0x14]
	movs r0, #0xc0
	lsls r0, r0, #1
	adds r1, r3, r0
	ldr r0, [r1]
	str r0, [r3, #0x18]
	str r2, [r1]
	bl func_080D0C2C
.L080D03C2:
	ldr r0, .L080D03F0 @ =gUnk_03000420
	ldr r1, [r0]
	ldr r0, [r1, #0xc]
	adds r0, #1
	str r0, [r1, #0xc]
	ldr r0, [r1, #0xc]
	cmp r0, #0xa
	bls .L080D0400
	movs r0, #0
	str r0, [r1, #0xc]
	ldr r0, .L080D03F4 @ =gUnk_0300041C
	ldr r0, [r0]
	ldr r0, [r0]
	cmp r0, #0
	beq .L080D0400
	cmp r0, #1
	bne .L080D03F8
	movs r0, #1
	movs r1, #0
	bl func_080D0B14
	b .L080D0400
	.align 2, 0
.L080D03F0: .4byte gUnk_03000420
.L080D03F4: .4byte gUnk_0300041C
.L080D03F8:
	movs r0, #1
	movs r1, #1
	bl func_080D0B14
.L080D0400:
	ldr r4, .L080D043C @ =gUnk_03000420
	ldr r1, [r4]
	ldr r0, [r1, #0x10]
	adds r0, #1
	str r0, [r1, #0x10]
	ldr r0, .L080D0440 @ =gUnk_0300041C
	ldr r0, [r0]
	ldr r0, [r0]
	cmp r0, #1
	bls .L080D0436
	ldr r0, [r1, #0x10]
	cmp r0, #0xb4
	bls .L080D0436
	movs r0, #0xb
	bl func_080D0A94
	ldr r0, [r4]
	movs r2, #0
	str r2, [r0, #0x14]
	movs r1, #0xa
	str r1, [r0, #0x18]
	movs r1, #0xc0
	lsls r1, r1, #1
	adds r0, r0, r1
	str r2, [r0]
	bl func_080D0C2C
.L080D0436:
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
.L080D043C: .4byte gUnk_03000420
.L080D0440: .4byte gUnk_0300041C

.section .text.g5_0490, "ax"
.L080D0490:
	.byte 0x70, 0xB5, 0x4E, 0x46, 0x45, 0x46, 0x60, 0xB4, 0x88, 0xB0, 0x81, 0x46, 0x0E, 0x1C, 0x14, 0x48
	.byte 0x80, 0x46, 0x01, 0x68, 0x02, 0x24, 0x4C, 0x61, 0x04, 0x20, 0x00, 0x90, 0x24, 0x31, 0x04, 0x91
	.byte 0x06, 0x94, 0x68, 0x46, 0x00, 0x25, 0x85, 0x83, 0x00, 0xF0, 0x76, 0xFB, 0x00, 0x94, 0x49, 0x46
	.byte 0x04, 0x91, 0x01, 0x36, 0x76, 0x08, 0x06, 0x96, 0x68, 0x46, 0x85, 0x83, 0x00, 0xF0, 0x6C, 0xFB
	.byte 0x41, 0x46, 0x08, 0x68, 0xC5, 0x61, 0x06, 0x62, 0xC2, 0x21, 0x49, 0x00, 0x40, 0x18, 0x05, 0x60
	.byte 0x08, 0xB0, 0x18, 0xBC, 0x98, 0x46, 0xA1, 0x46, 0x70, 0xBC, 0x01, 0xBC, 0x00, 0x47, 0x00, 0x00
	.byte 0x20, 0x04, 0x00, 0x03, 0x70, 0xB5, 0x4E, 0x46, 0x45, 0x46, 0x60, 0xB4, 0x88, 0xB0, 0x81, 0x46
	.byte 0x0D, 0x1C, 0x14, 0x48, 0x80, 0x46, 0x01, 0x68, 0x03, 0x26, 0x4E, 0x61, 0x04, 0x20, 0x00, 0x90
	.byte 0x24, 0x31, 0x04, 0x91, 0x02, 0x20, 0x06, 0x90, 0x68, 0x46, 0x00, 0x24, 0x84, 0x83, 0x00, 0xF0
	.byte 0x43, 0xFB, 0x00, 0x96, 0x49, 0x46, 0x04, 0x91, 0x01, 0x35, 0x6D, 0x08, 0x06, 0x95, 0x68, 0x46
	.byte 0x84, 0x83, 0x00, 0xF0, 0x39, 0xFB, 0x41, 0x46, 0x08, 0x68, 0xC4, 0x61, 0x05, 0x62, 0xC2, 0x21
	.byte 0x49, 0x00, 0x40, 0x18, 0x04, 0x60, 0x08, 0xB0, 0x18, 0xBC, 0x98, 0x46, 0xA1, 0x46, 0x70, 0xBC
	.byte 0x01, 0xBC, 0x00, 0x47, 0x20, 0x04, 0x00, 0x03

	thumb_func_start func_080D0558
func_080D0558: @ 0x080D0558
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x20
	adds r7, r0, #0
	adds r4, r1, #0
	mov sb, r2
	adds r6, r3, #0
	ldr r0, .L080D05D8 @ =gUnk_03000420
	mov sl, r0
	ldr r0, [r0]
	movs r1, #4
	str r1, [r0, #0x14]
	str r1, [sp]
	adds r0, #0x24
	str r0, [sp, #0x10]
	movs r1, #2
	mov r8, r1
	str r1, [sp, #0x18]
	mov r0, sp
	movs r5, #0
	strh r5, [r0, #0x1c]
	bl func_080D0BA8
	movs r0, #3
	str r0, [sp]
	mov r0, sb
	str r0, [sp, #0x10]
	adds r6, #1
	lsrs r6, r6, #1
	str r6, [sp, #0x18]
	mov r0, sp
	strh r5, [r0, #0x1c]
	bl func_080D0BA8
	mov r1, r8
	str r1, [sp]
	str r7, [sp, #0x10]
	adds r4, #1
	lsrs r4, r4, #1
	str r4, [sp, #0x18]
	mov r0, sp
	strh r5, [r0, #0x1c]
	bl func_080D0BA8
	mov r1, sl
	ldr r0, [r1]
	str r5, [r0, #0x1c]
	adds r4, r4, r6
	str r4, [r0, #0x20]
	movs r1, #0xc2
	lsls r1, r1, #1
	adds r0, r0, r1
	str r5, [r0]
	add sp, #0x20
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
.L080D05D8: .4byte gUnk_03000420

	thumb_func_start func_080D05DC
func_080D05DC: @ 0x080D05DC
	push {r4, r5, r6, lr}
	sub sp, #0x20
	ldr r6, .L080D0640 @ =gUnk_03000420
	ldr r0, [r6]
	movs r1, #5
	str r1, [r0, #0x14]
	ldr r0, [r0, #0x30]
	cmp r0, #0
	beq .L080D05F8
	movs r0, #1
	str r0, [sp]
	mov r0, sp
	bl func_080D0BA8
.L080D05F8:
	movs r0, #4
	str r0, [sp]
	ldr r0, [r6]
	adds r0, #0x24
	str r0, [sp, #0x10]
	movs r4, #2
	str r4, [sp, #0x18]
	mov r0, sp
	movs r5, #0
	strh r5, [r0, #0x1c]
	bl func_080D0BA8
	movs r0, #3
	str r0, [sp]
	ldr r0, [r6]
	adds r0, #0x2c
	str r0, [sp, #0x10]
	str r4, [sp, #0x18]
	mov r0, sp
	strh r5, [r0, #0x1c]
	bl func_080D0BA8
	str r4, [sp]
	ldr r0, [r6]
	adds r0, #0x28
	str r0, [sp, #0x10]
	str r4, [sp, #0x18]
	mov r0, sp
	strh r5, [r0, #0x1c]
	bl func_080D0BA8
	add sp, #0x20
	pop {r4, r5, r6}
	pop {r0}
	bx r0
	.align 2, 0
.L080D0640: .4byte gUnk_03000420

	thumb_func_start func_080D0644
func_080D0644: @ 0x080D0644
	push {r4, lr}
	ldr r4, .L080D0674 @ =gUnk_03000420
	ldr r2, [r4]
	adds r2, #0x28
	strb r0, [r2]
	ldr r2, [r4]
	lsrs r3, r0, #8
	adds r2, #0x29
	strb r3, [r2]
	ldr r2, [r4]
	lsrs r3, r0, #0x10
	adds r2, #0x2a
	strb r3, [r2]
	ldr r2, [r4]
	lsrs r0, r0, #0x18
	adds r2, #0x2b
	strb r0, [r2]
	ldr r0, [r4]
	str r1, [r0, #0x30]
	bl func_080D05DC
	pop {r4}
	pop {r0}
	bx r0
	.align 2, 0
.L080D0674: .4byte gUnk_03000420

.section .text.g5_06DC, "ax"
.L080D06DC:
	.byte 0x00, 0xB5, 0x06, 0x48
	.byte 0x00, 0x68, 0x02, 0x6A, 0x00, 0x2A, 0x09, 0xD0, 0xC1, 0x69, 0x64, 0x20, 0x48, 0x43, 0x11, 0x1C
	.byte 0x00, 0xF0, 0xF3, 0xFB, 0x03, 0xE0, 0x00, 0x00, 0x20, 0x04, 0x00, 0x03, 0x00, 0x20, 0x02, 0xBC
	.byte 0x08, 0x47, 0x00, 0x00

	thumb_func_start func_080D0704
func_080D0704: @ 0x080D0704
	push {r4, r5, lr}
	sub sp, #4
	movs r5, #0
	ldr r0, .L080D077C @ =0x04000140
	ldrh r0, [r0]
	adds r4, r0, #0
	ldr r0, .L080D0780 @ =gUnk_03000420
	ldr r0, [r0]
	movs r1, #0xbe
	lsls r1, r1, #1
	adds r0, r0, r1
	ldr r0, [r0]
	cmp r0, #0
	bne .L080D0754
	movs r0, #2
	ands r0, r4
	cmp r0, #0
	beq .L080D0736
	ldr r0, .L080D0784 @ =0x04000150
	ldr r0, [r0]
	str r0, [sp]
	mov r0, sp
	bl func_080D078C
	adds r5, r0, #0
.L080D0736:
	cmp r5, #0
	bne .L080D0748
	movs r0, #4
	ands r0, r4
	cmp r0, #0
	beq .L080D0748
	bl func_080D091C
	adds r5, r0, #0
.L080D0748:
	movs r0, #1
	ands r0, r4
	cmp r0, #0
	beq .L080D0754
	bl func_080D0A14
.L080D0754:
	cmp r5, #0
	beq .L080D0764
	ldr r1, .L080D0788 @ =0x04000158
	movs r0, #0
	strh r0, [r1]
	adds r0, r5, #0
	bl func_080D0A54
.L080D0764:
	ldr r0, .L080D077C @ =0x04000140
	strh r4, [r0]
	ldr r0, .L080D0780 @ =gUnk_03000420
	ldr r1, [r0]
	movs r0, #0
	str r0, [r1, #0xc]
	str r0, [r1, #0x10]
	add sp, #4
	pop {r4, r5}
	pop {r0}
	bx r0
	.align 2, 0
.L080D077C: .4byte 0x04000140
.L080D0780: .4byte gUnk_03000420
.L080D0784: .4byte 0x04000150
.L080D0788: .4byte 0x04000158

	thumb_func_start func_080D078C
func_080D078C: @ 0x080D078C
	push {r4, r5, lr}
	sub sp, #8
	adds r3, r0, #0
	ldr r0, .L080D07AC @ =gUnk_0300041C
	ldr r2, [r0]
	ldr r1, [r2]
	adds r5, r0, #0
	cmp r1, #4
	bls .L080D07A0
	b .L080D090C
.L080D07A0:
	ldr r0, [r2]
	lsls r0, r0, #2
	ldr r1, .L080D07B0 @ =.L080D07B4
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
.L080D07AC: .4byte gUnk_0300041C
.L080D07B0: .4byte .L080D07B4
.L080D07B4: @ jump table
	.4byte .L080D0910 @ case 0
	.4byte .L080D07C8 @ case 1
	.4byte .L080D07FC @ case 2
	.4byte .L080D083A @ case 3
	.4byte .L080D07FC @ case 4
.L080D07C8:
	ldr r0, [r5]
	ldr r0, [r0, #4]
	cmp r0, #1
	bhi .L080D07D2
	b .L080D0910
.L080D07D2:
	cmp r0, #2
	beq .L080D07D8
	b .L080D0910
.L080D07D8:
	ldr r0, .L080D07E8 @ =gUnk_03000420
	ldr r0, [r0]
	ldr r1, [r3]
	ldr r0, [r0, #8]
	cmp r1, r0
	beq .L080D07EC
	movs r0, #1
	b .L080D0912
	.align 2, 0
.L080D07E8: .4byte gUnk_03000420
.L080D07EC:
	ldr r1, .L080D07F8 @ =0x04000158
	movs r0, #0x30
	strh r0, [r1]
.L080D07F2:
	bl func_080D0A74
	b .L080D0910
	.align 2, 0
.L080D07F8: .4byte 0x04000158
.L080D07FC:
	ldr r4, [r5]
	ldr r0, [r4, #4]
	cmp r0, #1
	beq .L080D0836
	cmp r0, #1
	blo .L080D080E
	cmp r0, #2
	beq .L080D0830
	b .L080D0910
.L080D080E:
	ldrb r0, [r3]
	cmp r0, #0x42
	bne .L080D0828
	ldrb r0, [r3, #2]
	ldrb r1, [r3, #3]
	lsls r1, r1, #8
	orrs r0, r1
	ldr r1, [r4, #0x18]
	cmp r0, r1
	bne .L080D0860
	mov r1, sp
	movs r2, #0x43
	b .L080D08EC
.L080D0828:
	cmp r0, #0x41
	bne .L080D0910
	movs r0, #5
	b .L080D0912
.L080D0830:
	ldrb r0, [r3]
	cmp r0, #0x46
	beq .L080D07F2
.L080D0836:
	movs r0, #7
	b .L080D0912
.L080D083A:
	ldr r4, [r5]
	ldr r0, [r4, #4]
	cmp r0, #1
	beq .L080D0872
	cmp r0, #1
	blo .L080D084C
	cmp r0, #2
	beq .L080D0908
	b .L080D0910
.L080D084C:
	ldrb r0, [r3]
	cmp r0, #0x41
	bne .L080D086A
	ldrb r0, [r3, #2]
	ldrb r1, [r3, #3]
	lsls r1, r1, #8
	orrs r0, r1
	ldr r1, [r4, #0x18]
	cmp r0, r1
	beq .L080D0864
.L080D0860:
	movs r0, #4
	b .L080D0912
.L080D0864:
	mov r1, sp
	movs r2, #0x43
	b .L080D08EC
.L080D086A:
	cmp r0, #0x42
	bne .L080D0910
	movs r0, #6
	b .L080D0912
.L080D0872:
	ldr r1, [r4, #8]
	ldr r0, [r4, #0x18]
	cmp r1, r0
	bhs .L080D08D0
	ldrb r0, [r3]
	cmp r0, #0x44
	bne .L080D0908
	ldrb r0, [r3, #1]
	ldr r1, [r4, #8]
	movs r2, #0xff
	ands r1, r2
	cmp r0, r1
	bne .L080D0908
	ldr r0, [r4, #8]
	lsls r0, r0, #1
	ldr r1, [r4, #0x10]
	adds r0, r0, r1
	ldrb r1, [r3, #2]
	strb r1, [r0]
	ldrb r1, [r3, #3]
	strb r1, [r0, #1]
	ldr r2, [r5]
	ldr r0, [r2, #8]
	lsls r0, r0, #1
	ldr r1, [r2, #0x10]
	adds r0, r0, r1
	ldrb r1, [r0]
	ldrb r0, [r0, #1]
	lsls r0, r0, #8
	orrs r1, r0
	ldrh r0, [r2, #0x1c]
	adds r1, r1, r0
	strh r1, [r2, #0x1c]
	ldr r0, [r2, #8]
	adds r0, #1
	str r0, [r2, #8]
	ldr r0, .L080D08CC @ =gUnk_03000420
	ldr r1, [r0]
	movs r0, #0xc2
	lsls r0, r0, #1
	adds r1, r1, r0
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
	b .L080D0910
	.align 2, 0
.L080D08CC: .4byte gUnk_03000420
.L080D08D0:
	ldrb r0, [r3]
	cmp r0, #0x45
	bne .L080D0908
	ldrb r0, [r3, #2]
	ldrb r1, [r3, #3]
	lsls r1, r1, #8
	orrs r0, r1
	ldrh r1, [r4, #0x1c]
	cmp r1, r0
	beq .L080D08E8
	movs r0, #3
	b .L080D0912
.L080D08E8:
	mov r1, sp
	movs r2, #0x46
.L080D08EC:
	movs r0, #0
	strb r2, [r1]
	strb r0, [r1, #1]
	strb r0, [r1, #2]
	strb r0, [r1, #3]
	ldr r1, .L080D0904 @ =0x04000154
	ldr r0, [sp]
	str r0, [r1]
	ldr r0, [r4, #4]
	adds r0, #1
	str r0, [r4, #4]
	b .L080D0910
	.align 2, 0
.L080D0904: .4byte 0x04000154
.L080D0908:
	movs r0, #8
	b .L080D0912
.L080D090C:
	movs r0, #9
	b .L080D0912
.L080D0910:
	movs r0, #0
.L080D0912:
	add sp, #8
	pop {r4, r5}
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start func_080D091C
func_080D091C: @ 0x080D091C
	push {r4, r5, lr}
	sub sp, #4
	ldr r0, .L080D0938 @ =gUnk_0300041C
	ldr r2, [r0]
	ldr r1, [r2]
	adds r3, r0, #0
	cmp r1, #4
	bhi .L080D0A06
	ldr r0, [r2]
	lsls r0, r0, #2
	ldr r1, .L080D093C @ =.L080D0940
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
.L080D0938: .4byte gUnk_0300041C
.L080D093C: .4byte .L080D0940
.L080D0940: @ jump table
	.4byte .L080D0A0A @ case 0
	.4byte .L080D0954 @ case 1
	.4byte .L080D0964 @ case 2
	.4byte .L080D09F4 @ case 3
	.4byte .L080D0964 @ case 4
.L080D0954:
	ldr r1, [r3]
	ldr r0, [r1, #4]
	cmp r0, #1
	bne .L080D0A0A
	ldr r0, [r1, #4]
	adds r0, #1
	str r0, [r1, #4]
	b .L080D0A0A
.L080D0964:
	ldr r5, [r3]
	ldr r0, [r5, #4]
	cmp r0, #1
	bne .L080D0A0A
	ldr r1, [r5, #8]
	ldr r0, [r5, #0x18]
	cmp r1, r0
	bhs .L080D09D0
	ldr r1, [r5, #8]
	mov r2, sp
	movs r3, #0x44
	ldr r4, [r5, #8]
	lsls r1, r1, #1
	ldr r0, [r5, #0x10]
	adds r1, r1, r0
	strb r3, [r2]
	strb r4, [r2, #1]
	ldrb r0, [r1]
	strb r0, [r2, #2]
	ldrb r0, [r1, #1]
	strb r0, [r2, #3]
	ldr r0, [r5, #8]
	lsls r0, r0, #1
	ldr r1, [r5, #0x10]
	adds r0, r0, r1
	ldrb r1, [r0]
	ldrb r0, [r0, #1]
	lsls r0, r0, #8
	orrs r1, r0
	ldrh r0, [r5, #0x1c]
	adds r1, r1, r0
	strh r1, [r5, #0x1c]
	ldr r1, .L080D09C8 @ =0x04000154
	ldr r0, [sp]
	str r0, [r1]
	ldr r0, [r5, #8]
	adds r0, #1
	str r0, [r5, #8]
	ldr r0, [r5]
	cmp r0, #4
	beq .L080D0A0A
	ldr r0, .L080D09CC @ =gUnk_03000420
	ldr r1, [r0]
	movs r0, #0xc2
	lsls r0, r0, #1
	adds r1, r1, r0
	ldr r0, [r1]
	adds r0, #1
	str r0, [r1]
	b .L080D0A0A
	.align 2, 0
.L080D09C8: .4byte 0x04000154
.L080D09CC: .4byte gUnk_03000420
.L080D09D0:
	ldrh r2, [r5, #0x1c]
	mov r1, sp
	movs r0, #0x45
	strb r0, [r1]
	movs r0, #0
	strb r0, [r1, #1]
	strb r2, [r1, #2]
	lsrs r2, r2, #8
	strb r2, [r1, #3]
	ldr r1, .L080D09F0 @ =0x04000154
	ldr r0, [sp]
	str r0, [r1]
	ldr r0, [r5, #4]
	adds r0, #1
	str r0, [r5, #4]
	b .L080D0A0A
	.align 2, 0
.L080D09F0: .4byte 0x04000154
.L080D09F4:
	ldr r0, [r3]
	ldr r0, [r0, #4]
	cmp r0, #1
	bls .L080D0A0A
	cmp r0, #2
	bne .L080D0A0A
	bl func_080D0A74
	b .L080D0A0A
.L080D0A06:
	movs r0, #9
	b .L080D0A0C
.L080D0A0A:
	movs r0, #0
.L080D0A0C:
	add sp, #4
	pop {r4, r5}
	pop {r1}
	bx r1
