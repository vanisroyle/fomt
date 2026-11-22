    .INCLUDE "asm/macro.inc"
    .SYNTAX UNIFIED

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
	ldrh r3, [r4, #8] @ KeyRepeatState::repeated_keys
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
	@ Refresh the shared sprite palette and recreate the selected tab label.
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
	bl ResourceHandle_GetIndex
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
	@ Draw the blinking selection cursor before the seven tab sprites.
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
	bl ResourceHandle_GetIndex
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
	bl ResourceHandle_GetIndex
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
	@ The label is 12x2 tiles, drawn as three 32x16 sprites.
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
	bl ResourceHandle_GetIndex
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
