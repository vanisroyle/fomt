    .INCLUDE "asm/macro.inc"
    .SYNTAX UNIFIED

	.section .text.0803B8CC, "ax", %progbits
	.align 2, 0
	thumb_func_start InitializeDialogWindowLayers
InitializeDialogWindowLayers: @ 0x0803B8CC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0xc
	mov ip, r0
	ldr r0, [sp, #0x2c]
	ldr r5, [sp, #0x30]
	ldr r4, [sp, #0x34]
	ldr r6, [sp, #0x38]
	movs r7, #0
	mov sb, r7
	mov r7, ip
	strh r1, [r7]
	strh r2, [r7, #2]
	strh r3, [r7, #4]
	strh r0, [r7, #6]
	movs r0, #3
	ands r5, r0
	ldrb r0, [r7, #8]
	movs r1, #4
	rsbs r1, r1, #0
	mov r8, r1
	mov r2, r8
	ands r2, r0
	orrs r2, r5
	adds r0, r4, #0
	movs r7, #0x1f
	ands r0, r7
	lsls r0, r0, #2
	subs r1, #0x79
	ands r2, r1
	orrs r2, r0
	mov r8, r2
	mov r0, ip
	strb r2, [r0, #8]
	lsrs r1, r4, #3
	movs r0, #3
	ands r1, r0
	lsls r1, r1, #7
	mov r2, ip
	ldrh r0, [r2, #8]
	ldr r3, .L0803BA7C @ =0xFFFFFE7F
	ands r3, r0
	orrs r3, r1
	strh r3, [r2, #8]
	movs r0, #7
	ands r4, r0
	lsls r4, r4, #6
	adds r4, #0x14
	ldr r7, .L0803BA80 @ =0x000003FF
	str r7, [sp]
	lsls r4, r4, #9
	ldr r5, [r2, #8]
	ldr r0, .L0803BA84 @ =0xFFF801FF
	ands r5, r0
	orrs r5, r4
	str r5, [r2, #8]
	movs r0, #0xf
	ands r6, r0
	lsls r6, r6, #3
	ldrb r0, [r2, #0xa]
	movs r1, #0x79
	rsbs r1, r1, #0
	mov sl, r1
	mov r2, sl
	ands r2, r0
	orrs r2, r6
	mov sl, r2
	mov r6, ip
	strb r2, [r6, #0xa]
	movs r0, #0xff
	ldr r1, [sp, #0x3c]
	ands r1, r0
	lsls r1, r1, #7
	ldrh r2, [r6, #0xa]
	ldr r0, .L0803BA88 @ =0xFFFF807F
	ands r0, r2
	orrs r0, r1
	strh r0, [r6, #0xa]
	ldrb r1, [r6, #0xb]
	movs r0, #0x7f
	ands r0, r1
	strb r0, [r6, #0xb]
	ldrb r1, [r6, #0xc]
	movs r0, #0x80
	rsbs r0, r0, #0
	ands r0, r1
	strb r0, [r6, #0xc]
	movs r7, #0
	strb r7, [r6, #0xd]
	mov r2, ip
	adds r2, #0x10
	mov r0, sb
	str r0, [r6, #0x10]
	str r0, [r2, #4]
	ldr r0, .L0803BA8C @ =gTaskListVtable
	str r0, [r2, #8]
	adds r1, r2, #0
	adds r1, #0x10
	str r1, [r2, #0xc]
	mov r6, sb
	str r6, [r1, #4]
	ldr r0, .L0803BA90 @ =vtable_unk_080E5BD8
	str r0, [r1, #8]
	mov r0, ip
	adds r0, #0x1c
	str r0, [r2, #0x10]
	str r1, [r2, #0x14]
	lsls r3, r3, #0x17
	lsrs r3, r3, #0x1e
	lsls r5, r5, #0xd
	lsrs r7, r5, #0x16
	lsls r3, r3, #9
	adds r2, r3, r7
	ldr r0, .L0803BA94 @ =gUnk_080F9F74
	ldr r0, [r0]
	lsrs r5, r0, #5
	movs r4, #0
	ldr r0, [sp, #0x3c]
	cmp r0, #1
	bne .L0803B9C4
	movs r4, #1
.L0803B9C4:
	mov r1, ip
	adds r1, #0x2c
	ldr r0, .L0803BA98 @ =gUnk_080F9F70
	ldr r3, [r0]
	lsls r2, r2, #5
	movs r0, #0xc0
	lsls r0, r0, #0x13
	adds r2, r2, r0
	mov r6, sb
	mov r0, ip
	str r6, [r0, #0x2c]
	str r6, [r1, #4]
	ldr r0, .L0803BA9C @ =vtable_unk_080E7768
	str r0, [r1, #8]
	str r3, [r1, #0xc]
	str r2, [r1, #0x10]
	strb r5, [r1, #0x14]
	strb r4, [r1, #0x15]
	movs r2, #0
	strb r2, [r1, #0x16]
	mov r6, ip
	movs r1, #0
	ldrsh r0, [r6, r1]
	cmp r0, #0
	bge .L0803B9F8
	adds r0, #7
.L0803B9F8:
	asrs r0, r0, #3
	str r0, [sp, #4]
	mov r2, ip
	movs r0, #2
	ldrsh r6, [r2, r0]
	cmp r6, #0
	bge .L0803BA08
	adds r6, #7
.L0803BA08:
	asrs r6, r6, #3
	str r6, [sp, #8]
	mov r1, ip
	ldrh r4, [r1, #4]
	lsrs r4, r4, #3
	ldrh r1, [r1, #6]
	lsrs r1, r1, #3
	mov r2, r8
	lsls r3, r2, #0x19
	lsrs r3, r3, #0x1b
	mov r6, sl
	lsls r5, r6, #0x19
	mov r2, ip
	adds r2, #0x44
	mov r0, sb
	mov r6, ip
	str r0, [r6, #0x44]
	str r0, [r2, #4]
	ldr r0, .L0803BAA0 @ =gDialogWindowTilemapTaskVtable
	str r0, [r2, #8]
	ldr r0, [sp, #4]
	str r0, [r2, #0xc]
	ldr r6, [sp, #8]
	str r6, [r2, #0x10]
	str r4, [r2, #0x14]
	str r1, [r2, #0x18]
	movs r0, #0x1f
	ands r3, r0
	ldrb r1, [r2, #0x1c]
	subs r0, #0x3f
	ands r0, r1
	orrs r0, r3
	strb r0, [r2, #0x1c]
	ldr r1, [sp]
	ands r7, r1
	lsls r3, r7, #5
	ldrh r1, [r2, #0x1c]
	ldr r0, .L0803BAA4 @ =0xFFFF801F
	ands r0, r1
	orrs r0, r3
	strh r0, [r2, #0x1c]
	lsrs r5, r5, #0x1c
	lsls r5, r5, #0xf
	ldr r0, [r2, #0x1c]
	ldr r1, .L0803BAA8 @ =0xFFF87FFF
	ands r0, r1
	orrs r0, r5
	str r0, [r2, #0x1c]
	mov r0, ip
	add sp, #0xc
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0
.L0803BA7C: .4byte 0xFFFFFE7F
.L0803BA80: .4byte 0x000003FF
.L0803BA84: .4byte 0xFFF801FF
.L0803BA88: .4byte 0xFFFF807F
.L0803BA8C: .4byte gTaskListVtable
.L0803BA90: .4byte vtable_unk_080E5BD8
.L0803BA94: .4byte gUnk_080F9F74
.L0803BA98: .4byte gUnk_080F9F70
.L0803BA9C: .4byte vtable_unk_080E7768
.L0803BAA0: .4byte gDialogWindowTilemapTaskVtable
.L0803BAA4: .4byte 0xFFFF801F
.L0803BAA8: .4byte 0xFFF87FFF

	thumb_func_start UpdateDialogWindowLayers
UpdateDialogWindowLayers: @ 0x0803BAAC
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #8
	adds r7, r0, #0
	mov sb, r1
	adds r4, r2, #0
	mov sl, r3
	ldrb r2, [r7, #0xb]
	lsrs r1, r2, #7
	ldrb r0, [r7, #0xc]
	movs r3, #0x7f
	ands r0, r3
	lsls r0, r0, #1
	orrs r0, r1
	cmp r0, #1
	beq .L0803BADC
	cmp r0, #1
	ble .L0803BB18
	cmp r0, #2
	beq .L0803BAFC
	b .L0803BB18
.L0803BADC:
	ldrb r0, [r7, #0xd]
	cmp r0, #8
	bls .L0803BAF8
	movs r0, #0x80
	orrs r0, r2
	strb r0, [r7, #0xb]
	ldrb r1, [r7, #0xc]
	movs r0, #0x80
	rsbs r0, r0, #0
	ands r0, r1
	movs r1, #1
	orrs r0, r1
	strb r0, [r7, #0xc]
	b .L0803BB18
.L0803BAF8:
	adds r0, #1
	b .L0803BB16
.L0803BAFC:
	ldrb r0, [r7, #0xd]
	cmp r0, #0
	bne .L0803BB14
	adds r0, r3, #0
	ands r0, r2
	strb r0, [r7, #0xb]
	ldrb r1, [r7, #0xc]
	movs r0, #0x80
	rsbs r0, r0, #0
	ands r0, r1
	strb r0, [r7, #0xc]
	b .L0803BB18
.L0803BB14:
	subs r0, #1
.L0803BB16:
	strb r0, [r7, #0xd]
.L0803BB18:
	adds r3, r7, #0
	adds r3, #0x10
	movs r2, #0
	adds r1, r3, #0
	adds r1, #0x10
	ldr r0, [r3, #0xc]
	cmp r0, r1
	bne .L0803BB2A
	movs r2, #1
.L0803BB2A:
	cmp r2, #0
	bne .L0803BB3A
	ldr r0, [r4, #8]
	ldr r2, [r0, #0x1c]
	adds r0, r4, #0
	adds r1, r3, #0
	bl _call_via_r2
.L0803BB3A:
	ldrb r0, [r7, #0xb]
	lsrs r0, r0, #7
	ldrb r1, [r7, #0xc]
	movs r2, #0x7f
	ands r1, r2
	lsls r1, r1, #1
	orrs r1, r0
	cmp r1, #0
	bne .L0803BB4E
	b .L0803BCA8
.L0803BB4E:
	movs r0, #0
	mov r8, r0
	ldrb r4, [r7, #8]
	lsls r2, r4, #0x1e
	lsrs r2, r2, #0x1e
	movs r0, #0x80
	lsls r0, r0, #1
	lsls r0, r2
	movs r3, #0x80
	lsls r3, r3, #6
	adds r1, r3, #0
	orrs r0, r1
	mov r5, sb
	ldrh r1, [r5]
	orrs r0, r1
	strh r0, [r5]
	lsls r3, r2, #1
	lsls r2, r2, #2
	mov r5, sp
	ldrh r1, [r7, #8]
	lsls r1, r1, #0x17
	lsrs r1, r1, #0x1e
	lsls r1, r1, #2
	lsls r0, r4, #0x19
	lsrs r0, r0, #0x1b
	lsls r0, r0, #8
	orrs r1, r0
	movs r0, #0x40
	orrs r1, r0
	strh r1, [r5]
	adds r3, #8
	add r3, sb
	strh r1, [r3]
	mov r0, sp
	mov r1, r8
	strh r1, [r0]
	adds r0, r2, #0
	adds r0, #0x10
	add r0, sb
	strh r1, [r0]
	mov r1, sp
	ldrh r5, [r7, #2]
	rsbs r0, r5, #0
	strh r0, [r1]
	adds r2, #0x12
	add r2, sb
	strh r0, [r2]
	ldrb r6, [r7, #0xd]
	strb r4, [r1, #4]
	cmp r6, #8
	bls .L0803BBDC
	mov r3, sp
	ldrh r0, [r7]
	lsls r2, r0, #8
	ldrh r1, [r7, #4]
	adds r0, r0, r1
	orrs r2, r0
	strh r2, [r3]
	mov r0, sb
	adds r0, #0x40
	strh r2, [r0]
	mov r2, sp
	lsls r1, r5, #8
	ldrh r0, [r7, #6]
	adds r0, r5, r0
	orrs r1, r0
	strh r1, [r2]
	mov r0, sb
	adds r0, #0x44
	strh r1, [r0]
	b .L0803BC44
.L0803BBDC:
	ldrh r4, [r7, #6]
	adds r0, r4, #0
	muls r0, r6, r0
	movs r1, #9
	bl __udivsi3
	mov r8, r0
	movs r2, #2
	ldrsh r5, [r7, r2]
	adds r4, r5, r4
	adds r5, r5, r4
	lsrs r0, r5, #0x1f
	adds r5, r5, r0
	asrs r5, r5, #1
	mov r3, r8
	lsrs r0, r3, #0x1f
	add r0, r8
	asrs r0, r0, #1
	subs r5, r5, r0
	ldrh r4, [r7, #4]
	adds r0, r4, #0
	muls r0, r6, r0
	movs r1, #9
	bl __udivsi3
	movs r1, #0
	ldrsh r2, [r7, r1]
	adds r4, r2, r4
	adds r2, r2, r4
	lsrs r1, r2, #0x1f
	adds r2, r2, r1
	asrs r2, r2, #1
	lsrs r1, r0, #0x1f
	adds r1, r0, r1
	asrs r1, r1, #1
	subs r2, r2, r1
	mov r3, sp
	lsls r1, r2, #8
	adds r2, r2, r0
	orrs r1, r2
	strh r1, [r3]
	mov r0, sb
	adds r0, #0x40
	strh r1, [r0]
	mov r1, sp
	lsls r0, r5, #8
	add r5, r8
	orrs r0, r5
	strh r0, [r1]
	mov r1, sb
	adds r1, #0x44
	strh r0, [r1]
.L0803BC44:
	mov r0, sp
	movs r1, #0x3f
	strh r1, [r0]
	mov r3, sb
	adds r3, #0x48
	strh r1, [r3]
	mov r2, sp
	ldrb r4, [r2, #4]
	lsls r0, r4, #0x1e
	lsrs r0, r0, #0x1e
	movs r4, #1
	lsls r4, r0
	bics r1, r4
	strh r1, [r2]
	mov r0, sb
	adds r0, #0x4a
	strh r1, [r0]
	ldrh r0, [r7, #0xa]
	lsls r0, r0, #0x11
	lsrs r2, r0, #0x18
	cmp r2, #2
	bne .L0803BC80
	ldrh r0, [r3]
	movs r1, #0x20
	orrs r0, r1
	strh r0, [r3]
	ldr r0, .L0803BCA4 @ =0x08083F40
	orrs r4, r0
	mov r5, sb
	str r4, [r5, #0x50]
.L0803BC80:
	cmp r2, #1
	bne .L0803BCD8
	adds r3, r7, #0
	adds r3, #0x2c
	ldr r1, [r7, #0x2c]
	rsbs r0, r1, #0
	orrs r0, r1
	cmp r0, #0
	blt .L0803BCD8
	mov r1, sl
	ldr r0, [r1, #8]
	ldr r2, [r0, #0x10]
	mov r0, sl
	adds r1, r3, #0
	bl _call_via_r2
	b .L0803BCD8
	.align 2, 0
.L0803BCA4: .4byte 0x08083F40
.L0803BCA8:
	ldrb r0, [r7, #8]
	lsls r0, r0, #0x1e
	lsrs r0, r0, #0x1e
	movs r1, #0x80
	lsls r1, r1, #1
	lsls r1, r0
	mov r2, sb
	ldrh r0, [r2]
	bics r0, r1
	strh r0, [r2]
	adds r3, r7, #0
	adds r3, #0x2c
	ldr r1, [r7, #0x2c]
	rsbs r0, r1, #0
	orrs r0, r1
	cmp r0, #0
	bge .L0803BCD8
	mov r4, sl
	ldr r0, [r4, #8]
	ldr r2, [r0, #0x14]
	mov r0, sl
	adds r1, r3, #0
	bl _call_via_r2
.L0803BCD8:
	ldrb r1, [r7, #0xb]
	lsrs r1, r1, #7
	ldrb r0, [r7, #0xc]
	movs r2, #0x7f
	ands r0, r2
	lsls r0, r0, #1
	orrs r0, r1
	add sp, #8
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1
	.align 2, 0

	.section .text.0803C028, "ax", %progbits
	thumb_func_start UpdatePauseMenuWindow
UpdatePauseMenuWindow: @ 0x0803C028
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x74
	adds r7, r0, #0
	adds r4, r1, #0
	str r2, [sp, #0x44]
	adds r1, r3, #0
	ldr r3, [sp, #0x98]
	ldr r5, [sp, #0x9c]
	ldr r2, [sp, #0x94]
	bl UpdateDialogWindow
	str r0, [sp, #0x48]
	cmp r0, #0
	bge .L0803C04E
	b .L0803C506
.L0803C04E:
	cmp r0, #2
	bgt .L0803C054
	b .L0803C506
.L0803C054:
	cmp r0, #3
	beq .L0803C05A
	b .L0803C506
.L0803C05A:
	movs r0, #0
	str r0, [sp, #0x4c]
	ldrb r0, [r7, #0xf]
	cmp r0, #0
	beq .L0803C0DC
	ldrh r3, [r4, #8]
	adds r2, r3, #0
	lsls r1, r2, #0x1a
	lsrs r1, r1, #0x1f
	lsls r0, r2, #0x1b
	lsrs r0, r0, #0x1f
	orrs r1, r0
	cmp r1, #0
	beq .L0803C0C8
	movs r0, #0x20
	ands r0, r2
	cmp r0, #0
	beq .L0803C08C
	ldrb r0, [r7, #0xc]
	cmp r0, #0
	bne .L0803C088
	movs r0, #6
	b .L0803C0A4
.L0803C088:
	subs r0, #1
	b .L0803C0A4
.L0803C08C:
	movs r0, #0x10
	ands r2, r0
	cmp r2, #0
	beq .L0803C0A6
	ldrb r0, [r7, #0xc]
	cmp r0, #6
	bne .L0803C0A2
	add r1, sp, #0x4c
	ldrb r1, [r1]
	strb r1, [r7, #0xc]
	b .L0803C0A6
.L0803C0A2:
	adds r0, #1
.L0803C0A4:
	strb r0, [r7, #0xc]
.L0803C0A6:
	cmp r5, #0
	beq .L0803C0B2
	adds r0, r5, #0
	movs r1, #0xcb
	bl AudioPlayer_StartSong
.L0803C0B2:
	ldr r0, .L0803C0C4 @ =gPauseMenuTabLabels
	ldrb r1, [r7, #0xc]
	lsls r1, r1, #2
	adds r1, r1, r0
	ldr r1, [r1]
	str r1, [sp, #0x4c]
	movs r0, #0x14
	strb r0, [r7, #0xe]
	b .L0803C0DC
	.align 2, 0
.L0803C0C4: .4byte gPauseMenuTabLabels
.L0803C0C8:
	movs r0, #1
	ands r3, r0
	cmp r3, #0
	beq .L0803C0DC
	cmp r5, #0
	beq .L0803C0DC
	adds r0, r5, #0
	movs r1, #0xc5
	bl AudioPlayer_StartSong
.L0803C0DC:
	ldrb r0, [r7, #0xd]
	cmp r0, #0
	beq .L0803C142
	ldr r4, [r7, #8]
	ldr r0, [r4, #0x50]
	ldr r1, [r0, #0x20]
	ldr r3, [r1]
	ldrh r2, [r0, #0x2c]
	lsls r2, r2, #2
	ldr r0, [r0, #0x24]
	adds r0, r0, r2
	ldrh r2, [r0]
	add r0, sp, #4
	ldr r3, [r3, #0x10]
	bl _call_via_r3
	ldr r5, [sp, #0x14]
	adds r0, r4, #0
	adds r0, #0x30
	ldr r1, [r0, #4]
	bl func_080074C0
	adds r2, r0, #0
	add r1, sp, #0x14
	ldr r0, [sp, #0x14]
	movs r3, #0
	cmp r0, #0
	beq .L0803C116
	ldrh r3, [r1, #4]
.L0803C116:
	adds r1, r4, #0
	adds r1, #0x38
	lsls r0, r2, #5
	ldr r2, .L0803C4D0 @ =0x05000200
	adds r0, r0, r2
	str r5, [r1, #0xc]
	str r0, [r1, #0x10]
	str r3, [r1, #0x14]
	ldr r3, [sp, #0x94]
	ldr r0, [r3, #8]
	ldr r2, [r0, #0x10]
	adds r0, r3, #0
	bl _call_via_r2
	ldr r1, .L0803C4D4 @ =gPauseMenuTabLabels
	ldrb r0, [r7, #0xc]
	lsls r0, r0, #2
	adds r0, r0, r1
	ldr r0, [r0]
	str r0, [sp, #0x4c]
	movs r0, #0
	strb r0, [r7, #0xd]
.L0803C142:
	ldrb r0, [r7, #0xe]
	subs r0, #1
	strb r0, [r7, #0xe]
	lsls r0, r0, #0x18
	cmp r0, #0
	bne .L0803C152
	movs r0, #0x28
	strb r0, [r7, #0xe]
.L0803C152:
	ldrb r0, [r7, #0xe]
	cmp r0, #0x13
	bhi .L0803C24E
	ldr r0, [r7, #8]
	ldrb r1, [r7, #0xc]
	lsls r1, r1, #4
	adds r1, #0x10
	mov r8, r1
	ldr r4, [r0, #0x50]
	adds r0, #0x30
	ldr r1, [r0, #4]
	bl func_080074C0
	adds r6, r4, #0
	mov sb, r0
	adds r5, r6, #0
	adds r5, #0x20
	adds r0, r5, #0
	bl func_0805E8F0
	adds r4, r0, #0
	lsls r4, r4, #0x1e
	lsrs r4, r4, #0x1f
	ldr r1, [r6, #0x20]
	ldr r3, [r1]
	ldrh r2, [r5, #0xc]
	lsls r2, r2, #2
	ldr r0, [r6, #0x24]
	adds r0, r0, r2
	ldrh r2, [r0]
	add r0, sp, #4
	ldr r3, [r3, #0x10]
	bl _call_via_r3
	cmp r4, #0
	beq .L0803C1CE
	ldr r4, [sp, #0xc]
	ldr r1, [r6, #4]
	adds r0, r6, #0
	bl func_08007D4C
	adds r2, r0, #0
	add r1, sp, #0xc
	ldr r0, [sp, #0xc]
	movs r3, #0
	cmp r0, #0
	beq .L0803C1B2
	ldrh r3, [r1, #4]
.L0803C1B2:
	adds r1, r6, #0
	adds r1, #8
	lsls r0, r2, #5
	ldr r5, .L0803C4D8 @ =0x06010000
	adds r0, r0, r5
	str r4, [r1, #0xc]
	str r0, [r1, #0x10]
	str r3, [r1, #0x14]
	ldr r2, [sp, #0x94]
	ldr r0, [r2, #8]
	ldr r2, [r0, #0x10]
	ldr r0, [sp, #0x94]
	bl _call_via_r2
.L0803C1CE:
	ldr r0, [sp, #4]
	ldr r1, [r0, #4]
	ldr r0, [r0]
	str r0, [sp, #0x24]
	str r1, [sp, #0x28]
	ldr r3, [sp, #0x24]
	lsls r0, r3, #7
	lsrs r0, r0, #0x17
	add r0, r8
	ldr r1, .L0803C4DC @ =0x000001FF
	ands r0, r1
	lsls r0, r0, #0x10
	ldr r2, .L0803C4E0 @ =0xFE00FFFF
	ands r2, r3
	orrs r2, r0
	str r2, [sp, #0x24]
	add r4, sp, #0x24
	ldrb r0, [r4]
	adds r0, #0x96
	lsls r0, r0, #0x18
	lsrs r0, r0, #0x18
	ldr r1, .L0803C4E4 @ =0xFFFFFF00
	ands r1, r2
	orrs r1, r0
	str r1, [sp, #0x24]
	mov r3, sb
	lsls r1, r3, #0x10
	lsrs r1, r1, #0x10
	movs r0, #0xf
	ands r1, r0
	lsls r1, r1, #0xc
	ldr r2, .L0803C4E8 @ =0xFFFF0FFF
	ldr r0, [r4, #4]
	ands r0, r2
	orrs r0, r1
	str r0, [r4, #4]
	ldr r1, [r6, #4]
	adds r0, r6, #0
	bl func_08007D4C
	lsls r0, r0, #0x16
	lsrs r0, r0, #0x16
	ldr r2, .L0803C4EC @ =0xFFFFFC00
	ldr r1, [r4, #4]
	ands r1, r2
	orrs r1, r0
	str r1, [r4, #4]
	movs r0, #0x80
	lsls r0, r0, #5
	ldr r1, [sp, #0x24]
	orrs r1, r0
	str r1, [sp, #0x24]
	ldr r5, [sp, #0x44]
	ldrb r2, [r5]
	cmp r2, #0x7f
	bhi .L0803C24E
	lsls r0, r2, #3
	adds r0, #4
	adds r0, r5, r0
	str r1, [r0]
	ldrh r1, [r4, #4]
	strh r1, [r0, #4]
	adds r0, r2, #1
	strb r0, [r5]
.L0803C24E:
	ldr r0, [r7, #8]
	mov sl, r0
	movs r1, #0
	mov sb, r1
	mov r2, sp
	adds r2, #0x2c
	str r2, [sp, #0x60]
	ldr r3, [sp, #0x4c]
	rsbs r3, r3, #0
	str r3, [sp, #0x68]
	mov r4, sp
	adds r4, #0x3c
	str r4, [sp, #0x64]
	movs r5, #0x10
	str r5, [sp, #0x6c]
.L0803C26C:
	mov r1, sb
	lsls r0, r1, #2
	add r0, sl
	ldr r4, [r0, #0x54]
	mov r0, sl
	adds r0, #0x30
	ldr r1, [r0, #4]
	bl func_080074C0
	adds r6, r4, #0
	mov r8, r0
	adds r5, r6, #0
	adds r5, #0x20
	adds r0, r5, #0
	bl func_0805E8F0
	adds r4, r0, #0
	lsls r4, r4, #0x1e
	lsrs r4, r4, #0x1f
	ldr r1, [r6, #0x20]
	ldr r3, [r1]
	ldrh r2, [r5, #0xc]
	lsls r2, r2, #2
	ldr r0, [r6, #0x24]
	adds r0, r0, r2
	ldrh r2, [r0]
	add r0, sp, #4
	ldr r3, [r3, #0x10]
	bl _call_via_r3
	cmp r4, #0
	beq .L0803C2E0
	ldr r4, [sp, #0xc]
	ldr r1, [r6, #4]
	adds r0, r6, #0
	bl func_08007D4C
	adds r2, r0, #0
	add r1, sp, #0xc
	ldr r0, [sp, #0xc]
	movs r3, #0
	cmp r0, #0
	beq .L0803C2C4
	ldrh r3, [r1, #4]
.L0803C2C4:
	adds r1, r6, #0
	adds r1, #8
	lsls r0, r2, #5
	ldr r2, .L0803C4D8 @ =0x06010000
	adds r0, r0, r2
	str r4, [r1, #0xc]
	str r0, [r1, #0x10]
	str r3, [r1, #0x14]
	ldr r3, [sp, #0x94]
	ldr r0, [r3, #8]
	ldr r2, [r0, #0x10]
	adds r0, r3, #0
	bl _call_via_r2
.L0803C2E0:
	ldr r0, [sp, #4]
	ldr r1, [r0, #4]
	ldr r0, [r0]
	str r0, [sp, #0x2c]
	str r1, [sp, #0x30]
	ldr r3, [sp, #0x2c]
	lsls r0, r3, #7
	lsrs r0, r0, #0x17
	ldr r4, [sp, #0x6c]
	adds r0, r0, r4
	ldr r1, .L0803C4DC @ =0x000001FF
	ands r0, r1
	lsls r0, r0, #0x10
	ldr r2, .L0803C4E0 @ =0xFE00FFFF
	ands r2, r3
	orrs r2, r0
	str r2, [sp, #0x2c]
	ldr r5, [sp, #0x60]
	ldrb r1, [r5]
	adds r1, #0x96
	lsls r1, r1, #0x18
	lsrs r1, r1, #0x18
	ldr r0, .L0803C4E4 @ =0xFFFFFF00
	ands r0, r2
	orrs r0, r1
	str r0, [sp, #0x2c]
	mov r0, r8
	lsls r1, r0, #0x10
	lsrs r1, r1, #0x10
	movs r2, #0xf
	ands r1, r2
	lsls r1, r1, #0xc
	add r4, sp, #0x2c
	ldr r0, [r4, #4]
	ldr r3, .L0803C4E8 @ =0xFFFF0FFF
	ands r0, r3
	orrs r0, r1
	str r0, [r4, #4]
	ldr r1, [r6, #4]
	adds r0, r6, #0
	bl func_08007D4C
	lsls r0, r0, #0x16
	lsrs r0, r0, #0x16
	ldr r2, .L0803C4EC @ =0xFFFFFC00
	ldr r1, [r4, #4]
	ands r1, r2
	orrs r1, r0
	str r1, [r4, #4]
	ldr r1, [sp, #0x2c]
	movs r5, #0x80
	lsls r5, r5, #5
	orrs r1, r5
	str r1, [sp, #0x2c]
	ldr r0, [sp, #0x44]
	ldrb r2, [r0]
	cmp r2, #0x7f
	bhi .L0803C366
	lsls r0, r2, #3
	adds r0, #4
	ldr r3, [sp, #0x44]
	adds r0, r3, r0
	str r1, [r0]
	ldrh r1, [r4, #4]
	strh r1, [r0, #4]
	adds r0, r2, #1
	strb r0, [r3]
.L0803C366:
	ldr r4, [sp, #0x6c]
	adds r4, #0x10
	str r4, [sp, #0x6c]
	movs r5, #1
	add sb, r5
	mov r0, sb
	cmp r0, #6
	bhi .L0803C378
	b .L0803C26C
.L0803C378:
	ldr r7, [r7, #8]
	mov r8, r7
	ldr r1, [sp, #0x4c]
	cmp r1, #0
	beq .L0803C3B0
	add r0, sp, #0x34
	movs r4, #0xc
	movs r5, #2
	strh r4, [r0]
	strh r5, [r0, #2]
	ldr r0, [sp, #0x34]
	mov r6, r8
	adds r6, #0x74
	adds r1, r6, #0
	movs r2, #0
	bl FillMessageTiles
	add r0, sp, #0x38
	strh r4, [r0]
	strh r5, [r0, #2]
	ldr r0, [sp, #0x38]
	ldr r2, [sp, #0x4c]
	str r2, [sp]
	adds r1, r6, #0
	movs r2, #0
	movs r3, #0
	bl DrawMessageTileString
.L0803C3B0:
	mov r0, r8
	adds r0, #0x30
	ldr r1, [r0, #4]
	bl func_080074C0
	ldr r3, [sp, #0x68]
	ldr r4, [sp, #0x4c]
	orrs r3, r4
	mov r5, r8
	adds r5, #0x70
	str r5, [sp, #0x50]
	lsrs r1, r3, #0x1f
	str r1, [sp, #0x54]
	movs r1, #0
	movs r2, #0
	str r1, [sp, #0x3c]
	str r2, [sp, #0x40]
	ldr r2, .L0803C4F0 @ =0x3FFFFFFF
	ldr r1, [sp, #0x3c]
	ands r1, r2
	movs r2, #0x80
	lsls r2, r2, #0x18
	orrs r1, r2
	ldr r2, .L0803C4F4 @ =0xFFFF3FFF
	ands r1, r2
	movs r2, #0x80
	lsls r2, r2, #7
	orrs r1, r2
	str r1, [sp, #0x3c]
	lsls r0, r0, #0x10
	lsrs r0, r0, #0x10
	movs r2, #0xf
	ands r0, r2
	lsls r0, r0, #0xc
	ldr r3, [sp, #0x64]
	ldr r1, [r3, #4]
	ldr r4, .L0803C4E8 @ =0xFFFF0FFF
	ands r1, r4
	orrs r1, r0
	str r1, [r3, #4]
	ldr r0, [sp, #0x3c]
	ldr r5, .L0803C4E4 @ =0xFFFFFF00
	ands r0, r5
	movs r1, #0x8e
	orrs r0, r1
	movs r1, #0x80
	lsls r1, r1, #5
	orrs r0, r1
	str r0, [sp, #0x3c]
	movs r4, #0
	mov r2, r8
	adds r2, #0x74
	str r2, [sp, #0x58]
.L0803C41A:
	lsls r0, r4, #3
	ldr r3, [sp, #0x50]
	adds r0, r0, r3
	movs r5, #0xc1
	lsls r5, r5, #2
	adds r0, r0, r5
	ldr r1, [r0, #4]
	bl func_08007D4C
	mov r8, r0
	lsls r0, r4, #5
	mov sl, r0
	adds r1, r4, #1
	str r1, [sp, #0x5c]
	ldr r2, [sp, #0x54]
	cmp r2, #0
	beq .L0803C498
	movs r3, #0
	lsls r2, r4, #1
	lsls r1, r4, #2
	movs r4, #0x80
	mov sb, r4
	adds r1, r1, r2
	lsls r1, r1, #3
	ldr r5, [sp, #0x50]
	movs r4, #0xca
	lsls r4, r4, #2
	adds r0, r5, r4
	adds r4, r1, r0
	mov r5, r8
	lsls r0, r5, #5
	ldr r5, .L0803C4D8 @ =0x06010000
	adds r7, r0, r5
	lsls r2, r2, #6
	ldr r0, [sp, #0x58]
	adds r6, r2, r0
	ldr r2, [sp, #0x50]
	movs r5, #0xc7
	lsls r5, r5, #2
	adds r0, r2, r5
	adds r5, r1, r0
.L0803C46C:
	str r6, [r4]
	str r7, [r4, #4]
	mov r0, sb
	str r0, [r4, #8]
	ldr r1, [sp, #0x94]
	ldr r0, [r1, #8]
	ldr r2, [r0, #0x10]
	adds r0, r1, #0
	adds r1, r5, #0
	str r3, [sp, #0x70]
	bl _call_via_r2
	adds r4, #0x18
	adds r7, #0x80
	movs r2, #0xc0
	lsls r2, r2, #1
	adds r6, r6, r2
	adds r5, #0x18
	ldr r3, [sp, #0x70]
	adds r3, #1
	cmp r3, #1
	bls .L0803C46C
.L0803C498:
	mov r3, r8
	lsls r1, r3, #0x16
	lsrs r1, r1, #0x16
	ldr r2, .L0803C4EC @ =0xFFFFFC00
	ldr r4, [sp, #0x64]
	ldr r0, [r4, #4]
	ands r0, r2
	orrs r0, r1
	str r0, [r4, #4]
	mov r1, sl
	adds r1, #0x88
	ldr r0, .L0803C4DC @ =0x000001FF
	ands r1, r0
	lsls r1, r1, #0x10
	ldr r0, .L0803C4E0 @ =0xFE00FFFF
	ldr r2, [sp, #0x3c]
	ands r2, r0
	orrs r2, r1
	str r2, [sp, #0x3c]
	ldr r5, [sp, #0x44]
	ldrb r3, [r5]
	cmp r3, #0x7f
	bhi .L0803C500
	lsls r0, r3, #3
	adds r0, #4
	adds r0, r5, r0
	str r2, [r0]
	b .L0803C4F8
	.align 2, 0
.L0803C4D0: .4byte 0x05000200
.L0803C4D4: .4byte gPauseMenuTabLabels
.L0803C4D8: .4byte 0x06010000
.L0803C4DC: .4byte 0x000001FF
.L0803C4E0: .4byte 0xFE00FFFF
.L0803C4E4: .4byte 0xFFFFFF00
.L0803C4E8: .4byte 0xFFFF0FFF
.L0803C4EC: .4byte 0xFFFFFC00
.L0803C4F0: .4byte 0x3FFFFFFF
.L0803C4F4: .4byte 0xFFFF3FFF
.L0803C4F8:
	ldrh r1, [r4, #4]
	strh r1, [r0, #4]
	adds r0, r3, #1
	strb r0, [r5]
.L0803C500:
	ldr r4, [sp, #0x5c]
	cmp r4, #2
	bls .L0803C41A
.L0803C506:
	ldr r0, [sp, #0x48]
	add sp, #0x74
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r1}
	bx r1

	.section .text.0803C7C8, "ax", %progbits
	thumb_func_start DrawClockDisplay
DrawClockDisplay: @ 0x0803C7C8
	push {r4, r5, r6, r7, lr}
	mov r7, sl
	mov r6, sb
	mov r5, r8
	push {r5, r6, r7}
	sub sp, #0x6c
	mov sb, r1
	mov sl, r2
	mov r8, r3
	ldr r6, [r0]
	adds r2, r6, #0
	adds r2, #0x60
	ldr r1, [r2, #4]
	rsbs r0, r1, #0
	orrs r0, r1
	cmp r0, #0
	blt .L0803C7EC
	b .L0803CD28
.L0803C7EC:
	adds r0, r2, #0
	bl func_080074C0
	str r0, [sp, #0x58]
	adds r7, r6, #0
	adds r7, #0x80
	ldrb r0, [r7]
	cmp r0, #0
	beq .L0803C83A
	adds r4, r6, #0
	adds r4, #0x68
	ldr r1, [r6, #0x68]
	rsbs r0, r1, #0
	orrs r0, r1
	lsrs r5, r0, #0x1f
	cmp r5, #0
	bne .L0803C83A
	mov r0, sp
	adds r1, r6, #0
	movs r2, #0
	bl func_0805E790
	ldr r2, [sp, #0x10]
	ldr r1, [sp, #0x58]
	lsls r0, r1, #5
	ldr r1, .L0803CBC8 @ =0x05000200
	adds r0, r0, r1
	movs r1, #0x20
	str r2, [r4, #0xc]
	str r0, [r4, #0x10]
	str r1, [r4, #0x14]
	ldr r2, [sp, #0x8c]
	ldr r0, [r2, #8]
	ldr r2, [r0, #0x10]
	ldr r0, [sp, #0x8c]
	adds r1, r4, #0
	bl _call_via_r2
	strb r5, [r7]
.L0803C83A:
	mov r0, sb
	adds r0, #4
	str r0, [sp, #0x5c]
	mov r1, sl
	adds r1, #4
	str r1, [sp, #0x60]
	adds r7, r6, #0
	adds r7, #0x84
	movs r0, #0xc2
	lsls r0, r0, #1
	adds r2, r6, r0
	ldr r1, [r2, #4]
	rsbs r0, r1, #0
	orrs r0, r1
	cmp r0, #0
	bge .L0803C922
	adds r0, r2, #0
	bl func_08007D4C
	mov sb, r0
	movs r1, #0xc6
	lsls r1, r1, #1
	adds r5, r6, r1
	ldrb r0, [r5]
	cmp r0, #0
	beq .L0803C8A2
	movs r2, #0xc8
	lsls r2, r2, #1
	adds r3, r6, r2
	ldr r1, [r3]
	rsbs r0, r1, #0
	orrs r0, r1
	lsrs r4, r0, #0x1f
	cmp r4, #0
	bne .L0803C8A2
	mov r1, sb
	lsls r0, r1, #5
	ldr r2, .L0803CBCC @ =0x06010000
	adds r0, r0, r2
	movs r1, #0x80
	lsls r1, r1, #1
	str r7, [r3, #0xc]
	str r0, [r3, #0x10]
	str r1, [r3, #0x14]
	ldr r1, [sp, #0x8c]
	ldr r0, [r1, #8]
	ldr r2, [r0, #0x10]
	adds r0, r1, #0
	adds r1, r3, #0
	bl _call_via_r2
	strb r4, [r5]
.L0803C8A2:
	movs r0, #0
	movs r1, #0
	str r0, [sp, #0x20]
	str r1, [sp, #0x24]
	movs r1, #0x80
	lsls r1, r1, #5
	ldr r0, [sp, #0x20]
	orrs r0, r1
	ldr r1, .L0803CBD0 @ =0xFFFF3FFF
	ands r0, r1
	movs r1, #0x80
	lsls r1, r1, #7
	orrs r0, r1
	ldr r1, .L0803CBD4 @ =0x3FFFFFFF
	ands r0, r1
	movs r1, #0x80
	lsls r1, r1, #0x18
	orrs r0, r1
	ldr r2, [sp, #0x60]
	lsls r1, r2, #0x18
	lsrs r1, r1, #0x18
	ldr r2, .L0803CBD8 @ =0xFFFFFF00
	ands r0, r2
	orrs r0, r1
	ldr r1, .L0803CBDC @ =0x000001FF
	ldr r2, [sp, #0x5c]
	ands r1, r2
	lsls r1, r1, #0x10
	ldr r2, .L0803CBE0 @ =0xFE00FFFF
	ands r0, r2
	orrs r0, r1
	str r0, [sp, #0x20]
	mov r1, sb
	lsls r0, r1, #0x16
	lsrs r0, r0, #0x16
	ldr r2, .L0803CBE4 @ =0xFFFFFC00
	add r3, sp, #0x20
	ldr r1, [r3, #4]
	ands r1, r2
	orrs r1, r0
	ldr r2, [sp, #0x58]
	lsls r0, r2, #0x10
	lsrs r0, r0, #0x10
	movs r2, #0xf
	ands r0, r2
	lsls r0, r0, #0xc
	ldr r2, .L0803CBE8 @ =0xFFFF0FFF
	ands r1, r2
	orrs r1, r0
	str r1, [r3, #4]
	mov r0, r8
	ldrb r2, [r0]
	cmp r2, #0x7f
	bhi .L0803C922
	lsls r0, r2, #3
	adds r0, #4
	add r0, r8
	ldr r1, [sp, #0x20]
	str r1, [r0]
	ldrh r1, [r3, #4]
	strh r1, [r0, #4]
	adds r0, r2, #1
	mov r1, r8
	strb r0, [r1]
.L0803C922:
	movs r2, #0xd4
	lsls r2, r2, #1
	adds r2, r2, r6
	mov sb, r2
	ldr r5, [sp, #0x5c]
	adds r5, #0x20
	movs r0, #0xaa
	lsls r0, r0, #2
	adds r2, r6, r0
	ldr r1, [r2, #4]
	rsbs r0, r1, #0
	orrs r0, r1
	cmp r0, #0
	bge .L0803CA06
	adds r0, r2, #0
	bl func_08007D4C
	mov sl, r0
	movs r1, #0xac
	lsls r1, r1, #2
	adds r7, r6, r1
	ldrb r0, [r7]
	cmp r0, #0
	beq .L0803C988
	movs r2, #0xad
	lsls r2, r2, #2
	adds r3, r6, r2
	ldr r1, [r3]
	rsbs r0, r1, #0
	orrs r0, r1
	lsrs r4, r0, #0x1f
	cmp r4, #0
	bne .L0803C988
	mov r1, sl
	lsls r0, r1, #5
	ldr r2, .L0803CBCC @ =0x06010000
	adds r0, r0, r2
	movs r1, #0x80
	lsls r1, r1, #1
	mov r2, sb
	str r2, [r3, #0xc]
	str r0, [r3, #0x10]
	str r1, [r3, #0x14]
	ldr r1, [sp, #0x8c]
	ldr r0, [r1, #8]
	ldr r2, [r0, #0x10]
	adds r0, r1, #0
	adds r1, r3, #0
	bl _call_via_r2
	strb r4, [r7]
.L0803C988:
	movs r0, #0
	movs r1, #0
	str r0, [sp, #0x28]
	str r1, [sp, #0x2c]
	movs r1, #0x80
	lsls r1, r1, #5
	ldr r0, [sp, #0x28]
	orrs r0, r1
	ldr r1, .L0803CBD0 @ =0xFFFF3FFF
	ands r0, r1
	movs r1, #0x80
	lsls r1, r1, #7
	orrs r0, r1
	ldr r1, .L0803CBD4 @ =0x3FFFFFFF
	ands r0, r1
	movs r1, #0x80
	lsls r1, r1, #0x18
	orrs r0, r1
	ldr r2, [sp, #0x60]
	lsls r1, r2, #0x18
	lsrs r1, r1, #0x18
	ldr r2, .L0803CBD8 @ =0xFFFFFF00
	ands r0, r2
	orrs r0, r1
	ldr r1, .L0803CBDC @ =0x000001FF
	ands r5, r1
	lsls r2, r5, #0x10
	ldr r1, .L0803CBE0 @ =0xFE00FFFF
	ands r0, r1
	orrs r0, r2
	str r0, [sp, #0x28]
	mov r1, sl
	lsls r0, r1, #0x16
	lsrs r0, r0, #0x16
	ldr r2, .L0803CBE4 @ =0xFFFFFC00
	add r3, sp, #0x28
	ldr r1, [r3, #4]
	ands r1, r2
	orrs r1, r0
	ldr r2, [sp, #0x58]
	lsls r0, r2, #0x10
	lsrs r0, r0, #0x10
	movs r2, #0xf
	ands r0, r2
	lsls r0, r0, #0xc
	ldr r2, .L0803CBE8 @ =0xFFFF0FFF
	ands r1, r2
	orrs r1, r0
	str r1, [r3, #4]
	mov r0, r8
	ldrb r2, [r0]
	cmp r2, #0x7f
	bhi .L0803CA06
	lsls r0, r2, #3
	adds r0, #4
	add r0, r8
	ldr r1, [sp, #0x28]
	str r1, [r0]
	ldrh r1, [r3, #4]
	strh r1, [r0, #4]
	adds r0, r2, #1
	mov r1, r8
	strb r0, [r1]
.L0803CA06:
	movs r0, #0
	movs r1, #0
	str r0, [sp, #0x64]
	str r1, [sp, #0x68]
	movs r1, #0x80
	lsls r1, r1, #5
	str r1, [sp, #0x64]
	movs r0, #0x80
	lsls r0, r0, #0x17
	orrs r1, r0
	str r1, [sp, #0x64]
	movs r1, #0xb4
	lsls r1, r1, #2
	adds r2, r6, r1
	ldr r1, [r2, #4]
	rsbs r0, r1, #0
	orrs r0, r1
	cmp r0, #0
	bge .L0803CB02
	adds r0, r2, #0
	bl func_08007D4C
	mov sl, r0
	movs r2, #0xbc
	lsls r2, r2, #2
	adds r2, r2, r6
	mov sb, r2
	ldrb r0, [r2]
	cmp r0, #0
	beq .L0803CA9A
	movs r0, #0xb6
	lsls r0, r0, #2
	adds r5, r6, r0
	ldr r1, [r5]
	rsbs r0, r1, #0
	orrs r0, r1
	lsrs r7, r0, #0x1f
	cmp r7, #0
	bne .L0803CA9A
	movs r1, #0xb3
	lsls r1, r1, #2
	adds r0, r6, r1
	ldrb r2, [r0]
	ldr r4, [r6]
	add r0, sp, #0x30
	ldr r3, [r4, #0xc]
	adds r1, r6, #0
	bl _call_via_r3
	ldr r0, [sp, #0x30]
	ldrh r2, [r0]
	mov r0, sp
	ldr r3, [r4, #0x10]
	adds r1, r6, #0
	bl _call_via_r3
	ldr r2, [sp, #8]
	mov r1, sl
	lsls r0, r1, #5
	ldr r1, .L0803CBCC @ =0x06010000
	adds r0, r0, r1
	movs r1, #0x80
	str r2, [r5, #0xc]
	str r0, [r5, #0x10]
	str r1, [r5, #0x14]
	ldr r2, [sp, #0x8c]
	ldr r0, [r2, #8]
	ldr r2, [r0, #0x10]
	ldr r0, [sp, #0x8c]
	adds r1, r5, #0
	bl _call_via_r2
	mov r0, sb
	strb r7, [r0]
.L0803CA9A:
	ldr r1, [sp, #0x64]
	ldr r2, [sp, #0x68]
	str r1, [sp, #0x38]
	str r2, [sp, #0x3c]
	ldr r2, [sp, #0x60]
	lsls r1, r2, #0x18
	lsrs r1, r1, #0x18
	ldr r2, .L0803CBD8 @ =0xFFFFFF00
	ldr r0, [sp, #0x38]
	ands r0, r2
	orrs r0, r1
	ldr r1, [sp, #0x5c]
	adds r1, #0x40
	ldr r2, .L0803CBDC @ =0x000001FF
	ands r1, r2
	lsls r1, r1, #0x10
	ldr r2, .L0803CBE0 @ =0xFE00FFFF
	ands r0, r2
	orrs r0, r1
	str r0, [sp, #0x38]
	mov r1, sl
	lsls r0, r1, #0x16
	lsrs r0, r0, #0x16
	ldr r2, .L0803CBE4 @ =0xFFFFFC00
	add r3, sp, #0x38
	ldr r1, [r3, #4]
	ands r1, r2
	orrs r1, r0
	ldr r2, [sp, #0x58]
	lsls r0, r2, #0x10
	lsrs r0, r0, #0x10
	movs r2, #0xf
	ands r0, r2
	lsls r0, r0, #0xc
	ldr r2, .L0803CBE8 @ =0xFFFF0FFF
	ands r1, r2
	orrs r1, r0
	str r1, [r3, #4]
	mov r0, r8
	ldrb r2, [r0]
	cmp r2, #0x7f
	bhi .L0803CB02
	lsls r0, r2, #3
	adds r0, #4
	add r0, r8
	ldr r1, [sp, #0x38]
	str r1, [r0]
	ldrh r1, [r3, #4]
	strh r1, [r0, #4]
	adds r0, r2, #1
	mov r1, r8
	strb r0, [r1]
.L0803CB02:
	movs r2, #0xc5
	lsls r2, r2, #2
	adds r3, r6, r2
	ldr r1, [r3, #4]
	rsbs r0, r1, #0
	orrs r0, r1
	cmp r0, #0
	blt .L0803CB14
	b .L0803CD28
.L0803CB14:
	movs r0, #0xbd
	lsls r0, r0, #2
	adds r4, r6, r0
	ldr r1, [r4, #4]
	rsbs r0, r1, #0
	orrs r0, r1
	cmp r0, #0
	blt .L0803CB26
	b .L0803CD28
.L0803CB26:
	ldr r1, [sp, #0x64]
	ldr r2, [sp, #0x68]
	str r1, [sp, #0x48]
	str r2, [sp, #0x4c]
	ldr r2, [sp, #0x60]
	lsls r1, r2, #0x18
	lsrs r1, r1, #0x18
	ldr r2, .L0803CBD8 @ =0xFFFFFF00
	ldr r0, [sp, #0x48]
	ands r0, r2
	orrs r0, r1
	ldr r1, [sp, #0x5c]
	adds r1, #0x58
	ldr r2, .L0803CBDC @ =0x000001FF
	ands r1, r2
	lsls r1, r1, #0x10
	ldr r2, .L0803CBE0 @ =0xFE00FFFF
	ands r0, r2
	orrs r0, r1
	str r0, [sp, #0x48]
	ldr r1, .L0803CBEC @ =0x000002F1
	adds r0, r6, r1
	ldrb r0, [r0]
	cmp r0, #0
	bne .L0803CB5A
	b .L0803CC72
.L0803CB5A:
	ldr r1, [r3, #4]
	adds r0, r3, #0
	bl func_08007D4C
	mov sb, r0
	ldr r1, [r4, #4]
	adds r0, r4, #0
	bl func_080074C0
	mov sl, r0
	movs r2, #0xcd
	lsls r2, r2, #2
	adds r7, r6, r2
	ldrb r0, [r7]
	cmp r0, #0
	beq .L0803CC32
	ldr r1, .L0803CBF0 @ =0x000002F2
	adds r0, r6, r1
	ldrh r2, [r0]
	adds r5, r6, #0
	adds r5, #0x30
	ldr r4, [r6, #0x30]
	add r0, sp, #0x40
	ldr r3, [r4, #0xc]
	adds r1, r5, #0
	bl _call_via_r3
	ldr r0, [sp, #0x40]
	ldrh r2, [r0]
	mov r0, sp
	ldr r3, [r4, #0x10]
	adds r1, r5, #0
	bl _call_via_r3
	movs r2, #0xc7
	lsls r2, r2, #2
	adds r3, r6, r2
	ldr r1, [r3]
	rsbs r0, r1, #0
	orrs r0, r1
	cmp r0, #0
	blt .L0803CBFE
	ldr r2, [sp, #8]
	mov r1, sb
	lsls r0, r1, #5
	ldr r1, .L0803CBCC @ =0x06010000
	adds r0, r0, r1
	movs r1, #0x80
	str r2, [r3, #0xc]
	str r0, [r3, #0x10]
	str r1, [r3, #0x14]
	ldr r2, [sp, #0x8c]
	ldr r0, [r2, #8]
	b .L0803CBF4
	.align 2, 0
.L0803CBC8: .4byte 0x05000200
.L0803CBCC: .4byte 0x06010000
.L0803CBD0: .4byte 0xFFFF3FFF
.L0803CBD4: .4byte 0x3FFFFFFF
.L0803CBD8: .4byte 0xFFFFFF00
.L0803CBDC: .4byte 0x000001FF
.L0803CBE0: .4byte 0xFE00FFFF
.L0803CBE4: .4byte 0xFFFFFC00
.L0803CBE8: .4byte 0xFFFF0FFF
.L0803CBEC: .4byte 0x000002F1
.L0803CBF0: .4byte 0x000002F2
.L0803CBF4:
	ldr r2, [r0, #0x10]
	ldr r0, [sp, #0x8c]
	adds r1, r3, #0
	bl _call_via_r2
.L0803CBFE:
	movs r0, #0xbf
	lsls r0, r0, #2
	adds r3, r6, r0
	ldr r1, [r3]
	rsbs r0, r1, #0
	orrs r0, r1
	cmp r0, #0
	blt .L0803CC2E
	ldr r2, [sp, #0x10]
	mov r1, sl
	lsls r0, r1, #5
	ldr r1, .L0803CD38 @ =0x05000200
	adds r0, r0, r1
	movs r1, #0x20
	str r2, [r3, #0xc]
	str r0, [r3, #0x10]
	str r1, [r3, #0x14]
	ldr r2, [sp, #0x8c]
	ldr r0, [r2, #8]
	ldr r2, [r0, #0x10]
	ldr r0, [sp, #0x8c]
	adds r1, r3, #0
	bl _call_via_r2
.L0803CC2E:
	movs r0, #0
	strb r0, [r7]
.L0803CC32:
	mov r1, sb
	lsls r0, r1, #0x16
	lsrs r0, r0, #0x16
	ldr r2, .L0803CD3C @ =0xFFFFFC00
	add r3, sp, #0x48
	ldr r1, [r3, #4]
	ands r1, r2
	orrs r1, r0
	mov r2, sl
	lsls r0, r2, #0x10
	lsrs r0, r0, #0x10
	movs r2, #0xf
	ands r0, r2
	lsls r0, r0, #0xc
	ldr r2, .L0803CD40 @ =0xFFFF0FFF
	ands r1, r2
	orrs r1, r0
	str r1, [r3, #4]
	mov r0, r8
	ldrb r2, [r0]
	cmp r2, #0x7f
	bhi .L0803CC72
	lsls r0, r2, #3
	adds r0, #4
	add r0, r8
	ldr r1, [sp, #0x48]
	str r1, [r0]
	ldrh r1, [r3, #4]
	strh r1, [r0, #4]
	adds r0, r2, #1
	mov r1, r8
	strb r0, [r1]
.L0803CC72:
	movs r0, #0xce
	lsls r0, r0, #2
	adds r2, r6, r0
	ldr r1, [r2, #4]
	rsbs r0, r1, #0
	orrs r0, r1
	cmp r0, #0
	bge .L0803CD28
	adds r0, r2, #0
	bl func_08007D4C
	adds r7, r0, #0
	movs r1, #0xd6
	lsls r1, r1, #2
	adds r1, r1, r6
	mov sl, r1
	ldrb r0, [r1]
	cmp r0, #0
	beq .L0803CCEC
	movs r2, #0xd0
	lsls r2, r2, #2
	adds r5, r6, r2
	ldr r1, [r5]
	rsbs r0, r1, #0
	orrs r0, r1
	lsrs r0, r0, #0x1f
	mov sb, r0
	cmp r0, #0
	bne .L0803CCEC
	ldr r4, [r6]
	add r0, sp, #0x50
	ldr r3, [r4, #0xc]
	adds r1, r6, #0
	movs r2, #5
	bl _call_via_r3
	ldr r0, [sp, #0x50]
	ldrh r2, [r0]
	mov r0, sp
	ldr r3, [r4, #0x10]
	adds r1, r6, #0
	bl _call_via_r3
	ldr r2, [sp, #8]
	lsls r0, r7, #5
	ldr r1, .L0803CD44 @ =0x06010000
	adds r0, r0, r1
	movs r1, #0x80
	str r2, [r5, #0xc]
	str r0, [r5, #0x10]
	str r1, [r5, #0x14]
	ldr r2, [sp, #0x8c]
	ldr r0, [r2, #8]
	ldr r2, [r0, #0x10]
	ldr r0, [sp, #0x8c]
	adds r1, r5, #0
	bl _call_via_r2
	mov r1, sb
	mov r0, sl
	strb r1, [r0]
.L0803CCEC:
	add r3, sp, #0x48
	ldr r2, .L0803CD48 @ =0x000003FF
	adds r0, r2, #0
	ands r7, r0
	ldrh r1, [r3, #4]
	ldr r0, .L0803CD3C @ =0xFFFFFC00
	ands r0, r1
	orrs r0, r7
	strh r0, [r3, #4]
	ldr r0, [sp, #0x58]
	lsls r2, r0, #4
	ldrb r1, [r3, #5]
	movs r0, #0xf
	ands r0, r1
	orrs r0, r2
	strb r0, [r3, #5]
	mov r1, r8
	ldrb r2, [r1]
	cmp r2, #0x7f
	bhi .L0803CD28
	lsls r0, r2, #3
	adds r0, #4
	add r0, r8
	ldr r1, [sp, #0x48]
	str r1, [r0]
	ldrh r1, [r3, #4]
	strh r1, [r0, #4]
	adds r0, r2, #1
	mov r2, r8
	strb r0, [r2]
.L0803CD28:
	add sp, #0x6c
	pop {r3, r4, r5}
	mov r8, r3
	mov sb, r4
	mov sl, r5
	pop {r4, r5, r6, r7}
	pop {r0}
	bx r0
	.align 2, 0
.L0803CD38: .4byte 0x05000200
.L0803CD3C: .4byte 0xFFFFFC00
.L0803CD40: .4byte 0xFFFF0FFF
.L0803CD44: .4byte 0x06010000
.L0803CD48: .4byte 0x000003FF
