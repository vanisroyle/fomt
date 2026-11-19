    .INCLUDE "asm/macro.inc"
    .SYNTAX UNIFIED

.L080D0C68:
	.byte 0x01, 0x70, 0x00, 0x21, 0x41, 0x70, 0x82, 0x70
	.byte 0x12, 0x0A, 0xC2, 0x70, 0x70, 0x47, 0x00, 0x00, 0x01, 0x70, 0x42, 0x70, 0x19, 0x78, 0x81, 0x70
	.byte 0x59, 0x78, 0xC1, 0x70, 0x70, 0x47, 0x00, 0x00, 0x00, 0x78, 0x70, 0x47, 0x01, 0x1C, 0x88, 0x78
	.byte 0xC9, 0x78, 0x09, 0x02, 0x08, 0x43, 0x70, 0x47, 0x40, 0x78, 0x70, 0x47, 0x82, 0x78, 0x0A, 0x70
	.byte 0xC0, 0x78, 0x48, 0x70, 0x70, 0x47, 0x00, 0x00, 0x01, 0x1C, 0x08, 0x78, 0x49, 0x78, 0x09, 0x02
	.byte 0x08, 0x43, 0x70, 0x47, 0x10, 0xB5, 0x02, 0x1C, 0x00, 0x23, 0x08, 0x78, 0x94, 0x78, 0xA0, 0x42
	.byte 0x04, 0xD1, 0x48, 0x78, 0xD2, 0x78, 0x90, 0x42, 0x00, 0xD1, 0x01, 0x23, 0x18, 0x1C, 0x10, 0xBC
	.byte 0x02, 0xBC, 0x08, 0x47

	thumb_func_start func_080D0CD4
func_080D0CD4: @ 0x080D0CD4
	push {lr}
	adds r1, r0, #0
	ldr r0, .L080D0D08 @ =0x0000FFFF
	cmp r1, r0
	bhi .L080D0D02
	movs r3, #0xff
	ands r3, r1
	movs r0, #0xff
	lsls r0, r0, #8
	ands r0, r1
	asrs r2, r0, #8
	adds r0, r3, #0
	subs r0, #0x40
	cmp r0, #0xbc
	bhi .L080D0D02
	cmp r2, #0x80
	bls .L080D0D02
	cmp r2, #0xea
	bhi .L080D0D02
	adds r0, r2, #0
	subs r0, #0xa0
	cmp r0, #0x3f
	bhi .L080D0D0C
.L080D0D02:
	movs r0, #1
	rsbs r0, r0, #0
	b .L080D0D22
	.align 2, 0
.L080D0D08: .4byte 0x0000FFFF
.L080D0D0C:
	cmp r2, #0x9f
	bhi .L080D0D14
	subs r2, #0x81
	b .L080D0D16
.L080D0D14:
	subs r2, #0xc1
.L080D0D16:
	subs r3, #0x40
	lsls r1, r2, #1
	adds r1, r1, r2
	lsls r0, r1, #6
	subs r0, r0, r1
	adds r0, r0, r3
.L080D0D22:
	pop {r1}
	bx r1
	.align 2, 0

	thumb_func_start func_080D0D28
func_080D0D28: @ 0x080D0D28
	push {r4, r5, r6, lr}
	adds r4, r0, #0
	adds r2, r1, #0
	movs r6, #0
	movs r5, #1
	rsbs r5, r5, #0
	adds r0, r2, #0
	subs r0, #0xb1
	cmp r0, #0x12
	bls .L080D0D3E
	b .L080D0E20
.L080D0D3E:
	lsls r0, r0, #2
	ldr r1, .L080D0D48 @ =.L080D0D4C
	adds r0, r0, r1
	ldr r0, [r0]
	mov pc, r0
	.align 2, 0
.L080D0D48: .4byte .L080D0D4C
.L080D0D4C: @ jump table
	.4byte .L080D0DB0 @ case 0
	.4byte .L080D0DB8 @ case 1
	.4byte .L080D0DC0 @ case 2
	.4byte .L080D0D98 @ case 3
	.4byte .L080D0E20 @ case 4
	.4byte .L080D0DA0 @ case 5
	.4byte .L080D0DA8 @ case 6
	.4byte .L080D0E20 @ case 7
	.4byte .L080D0E20 @ case 8
	.4byte .L080D0E20 @ case 9
	.4byte .L080D0DC8 @ case 10
	.4byte .L080D0DD0 @ case 11
	.4byte .L080D0DD8 @ case 12
	.4byte .L080D0DE0 @ case 13
	.4byte .L080D0DE8 @ case 14
	.4byte .L080D0DF0 @ case 15
	.4byte .L080D0DF8 @ case 16
	.4byte .L080D0E00 @ case 17
	.4byte .L080D0E08 @ case 18
.L080D0D98:
	ldr r0, .L080D0D9C @ =gUnk_08117B20
	b .L080D0E0A
	.align 2, 0
.L080D0D9C: .4byte gUnk_08117B20
.L080D0DA0:
	ldr r0, .L080D0DA4 @ =gUnk_08117B2C
	b .L080D0E0A
	.align 2, 0
.L080D0DA4: .4byte gUnk_08117B2C
.L080D0DA8:
	ldr r0, .L080D0DAC @ =gUnk_08117B38
	b .L080D0E0A
	.align 2, 0
.L080D0DAC: .4byte gUnk_08117B38
.L080D0DB0:
	ldr r0, .L080D0DB4 @ =gUnk_08117B44
	b .L080D0E0A
	.align 2, 0
.L080D0DB4: .4byte gUnk_08117B44
.L080D0DB8:
	ldr r0, .L080D0DBC @ =gUnk_08117B50
	b .L080D0E0A
	.align 2, 0
.L080D0DBC: .4byte gUnk_08117B50
.L080D0DC0:
	ldr r0, .L080D0DC4 @ =gUnk_08117B5C
	b .L080D0E0A
	.align 2, 0
.L080D0DC4: .4byte gUnk_08117B5C
.L080D0DC8:
	ldr r0, .L080D0DCC @ =gUnk_08117B68
	b .L080D0E0A
	.align 2, 0
.L080D0DCC: .4byte gUnk_08117B68
.L080D0DD0:
	ldr r0, .L080D0DD4 @ =gUnk_08117B74
	b .L080D0E0A
	.align 2, 0
.L080D0DD4: .4byte gUnk_08117B74
.L080D0DD8:
	ldr r0, .L080D0DDC @ =gUnk_08117B80
	b .L080D0E0A
	.align 2, 0
.L080D0DDC: .4byte gUnk_08117B80
.L080D0DE0:
	ldr r0, .L080D0DE4 @ =gUnk_08117B8C
	b .L080D0E0A
	.align 2, 0
.L080D0DE4: .4byte gUnk_08117B8C
.L080D0DE8:
	ldr r0, .L080D0DEC @ =gUnk_08117B98
	b .L080D0E0A
	.align 2, 0
.L080D0DEC: .4byte gUnk_08117B98
.L080D0DF0:
	ldr r0, .L080D0DF4 @ =gUnk_08117BA4
	b .L080D0E0A
	.align 2, 0
.L080D0DF4: .4byte gUnk_08117BA4
.L080D0DF8:
	ldr r0, .L080D0DFC @ =gUnk_08117BB0
	b .L080D0E0A
	.align 2, 0
.L080D0DFC: .4byte gUnk_08117BB0
.L080D0E00:
	ldr r0, .L080D0E04 @ =gUnk_08117BBC
	b .L080D0E0A
	.align 2, 0
.L080D0E04: .4byte gUnk_08117BBC
.L080D0E08:
	ldr r0, .L080D0E18 @ =gUnk_08117BC8
.L080D0E0A:
	adds r1, r4, #0
	ldr r2, .L080D0E1C @ =func_0300085C
	bl _call_via_r2
	movs r0, #1
	b .L080D0EB6
	.align 2, 0
.L080D0E18: .4byte gUnk_08117BC8
.L080D0E1C: .4byte func_0300085C
.L080D0E20:
	cmp r2, #0
	ble .L080D0E58
	cmp r2, #0xff
	bgt .L080D0E3C
	ldr r1, .L080D0E38 @ =gUnk_084FA7A0
	lsls r0, r2, #1
	adds r0, r0, r1
	movs r1, #0
	ldrsh r5, [r0, r1]
	movs r6, #1
	b .L080D0E58
	.align 2, 0
.L080D0E38: .4byte gUnk_084FA7A0
.L080D0E3C:
	ldr r0, .L080D0E78 @ =0x0000FFFF
	cmp r2, r0
	bgt .L080D0E58
	adds r0, r2, #0
	bl func_080D0CD4
	cmp r0, #0
	blt .L080D0E58
	ldr r1, .L080D0E7C @ =gUnk_08523290
	lsls r0, r0, #1
	adds r0, r0, r1
	movs r1, #0
	ldrsh r5, [r0, r1]
	movs r6, #2
.L080D0E58:
	cmp r4, #0
	beq .L080D0EAE
	cmp r5, #0
	blt .L080D0EA4
	cmp r6, #1
	beq .L080D0E88
	lsls r0, r5, #1
	adds r0, r0, r5
	lsls r0, r0, #3
	ldr r1, .L080D0E80 @ =gUnk_084FA9A0
	adds r0, r0, r1
	adds r1, r4, #0
	ldr r2, .L080D0E84 @ =func_03000714
	bl _call_via_r2
	b .L080D0EAE
	.align 2, 0
.L080D0E78: .4byte 0x0000FFFF
.L080D0E7C: .4byte gUnk_08523290
.L080D0E80: .4byte gUnk_084FA9A0
.L080D0E84: .4byte func_03000714
.L080D0E88:
	lsls r0, r5, #1
	adds r0, r0, r5
	lsls r0, r0, #2
	ldr r1, .L080D0E9C @ =gUnk_084F90CC
	adds r0, r0, r1
	adds r1, r4, #0
	ldr r2, .L080D0EA0 @ =func_0300085C
	bl _call_via_r2
	b .L080D0EAE
	.align 2, 0
.L080D0E9C: .4byte gUnk_084F90CC
.L080D0EA0: .4byte func_0300085C
.L080D0EA4:
	adds r0, r4, #0
	movs r1, #0
	movs r2, #0x80
	bl memset
.L080D0EAE:
	movs r0, #0
	cmp r5, #0
	blt .L080D0EB6
	adds r0, r6, #0
.L080D0EB6:
	pop {r4, r5, r6}
	pop {r1}
	bx r1

	thumb_func_start func_080D0EBC
func_080D0EBC: @ 0x080D0EBC
	stm r3!, {r0, r1, r2}
	bx lr
    
    .global func_080D0EC0
func_080D0EC0:
	push {lr}
	movs r0, #0
	pop {r1}
	bx r1

    .global __divsi3
__divsi3:
	cmp r1, #0
	beq func_080D0EC0
	svc #6
	bx lr

    .global __modsi3
__modsi3:
	cmp r1, #0
	beq func_080D0EC0
	svc #6
	adds r0, r1, #0
	bx lr

	non_word_aligned_thumb_func_start __udivsi3
__udivsi3: @ 0x080D0EDA
	adds r3, r0, #0
	orrs r3, r1
	bpl __divsi3
	cmp r1, #0
	beq func_080D0EC0
	movs r3, #1
	movs r2, #0
	push {r4}
	cmp r0, r1
	blo .L080D0F48
	movs r4, #1
	lsls r4, r4, #0x1c
.L080D0EF2:
	cmp r1, r4
	bhs .L080D0F00
	cmp r1, r0
	bhs .L080D0F00
	lsls r1, r1, #4
	lsls r3, r3, #4
	b .L080D0EF2
.L080D0F00:
	lsls r4, r4, #3
.L080D0F02:
	cmp r1, r4
	bhs .L080D0F10
	cmp r1, r0
	bhs .L080D0F10
	lsls r1, r1, #1
	lsls r3, r3, #1
	b .L080D0F02
.L080D0F10:
	cmp r0, r1
	blo .L080D0F18
	subs r0, r0, r1
	orrs r2, r3
.L080D0F18:
	lsrs r4, r1, #1
	cmp r0, r4
	blo .L080D0F24
	subs r0, r0, r4
	lsrs r4, r3, #1
	orrs r2, r4
.L080D0F24:
	lsrs r4, r1, #2
	cmp r0, r4
	blo .L080D0F30
	subs r0, r0, r4
	lsrs r4, r3, #2
	orrs r2, r4
.L080D0F30:
	lsrs r4, r1, #3
	cmp r0, r4
	blo .L080D0F3C
	subs r0, r0, r4
	lsrs r4, r3, #3
	orrs r2, r4
.L080D0F3C:
	cmp r0, #0
	beq .L080D0F48
	lsrs r3, r3, #4
	beq .L080D0F48
	lsrs r1, r1, #4
	b .L080D0F10
.L080D0F48:
	adds r0, r2, #0
	pop {r4}
	bx lr

	non_word_aligned_thumb_func_start __umodsi3
__umodsi3: @ 0x080D0F4E
	adds r3, r0, #0
	orrs r3, r1
	bpl __modsi3
	cmp r1, #0
	beq func_080D0EC0
	movs r3, #1
	cmp r0, r1
	bhs sub_080D0F60
	bx lr

	thumb_func_start sub_080D0F60
sub_080D0F60: @ 0x080D0F60
	push {r4}
	movs r4, #1
	lsls r4, r4, #0x1c
.L080D0F66:
	cmp r1, r4
	bhs .L080D0F74
	cmp r1, r0
	bhs .L080D0F74
	lsls r1, r1, #4
	lsls r3, r3, #4
	b .L080D0F66
.L080D0F74:
	lsls r4, r4, #3
.L080D0F76:
	cmp r1, r4
	bhs .L080D0F84
	cmp r1, r0
	bhs .L080D0F84
	lsls r1, r1, #1
	lsls r3, r3, #1
	b .L080D0F76
.L080D0F84:
	movs r2, #0
	cmp r0, r1
	blo .L080D0F8C
	subs r0, r0, r1
.L080D0F8C:
	lsrs r4, r1, #1
	cmp r0, r4
	blo .L080D0F9E
	subs r0, r0, r4
	mov ip, r3
	movs r4, #1
	rors r3, r4
	orrs r2, r3
	mov r3, ip
.L080D0F9E:
	lsrs r4, r1, #2
	cmp r0, r4
	blo .L080D0FB0
	subs r0, r0, r4
	mov ip, r3
	movs r4, #2
	rors r3, r4
	orrs r2, r3
	mov r3, ip
.L080D0FB0:
	lsrs r4, r1, #3
	cmp r0, r4
	blo .L080D0FC2
	subs r0, r0, r4
	mov ip, r3
	movs r4, #3
	rors r3, r4
	orrs r2, r3
	mov r3, ip
.L080D0FC2:
	mov ip, r3
	cmp r0, #0
	beq .L080D0FD0
	lsrs r3, r3, #4
	beq .L080D0FD0
	lsrs r1, r1, #4
	b .L080D0F84
.L080D0FD0:
	movs r4, #0xe
	lsls r4, r4, #0x1c
	ands r2, r4
	bne sub_080D0FDC
	pop {r4}
	bx lr

	thumb_func_start sub_080D0FDC
sub_080D0FDC: @ 0x080D0FDC
	mov r3, ip
	movs r4, #3
	rors r3, r4
	tst r2, r3
	beq .L080D0FEA
	lsrs r4, r1, #3
	adds r0, r0, r4
.L080D0FEA:
	mov r3, ip
	movs r4, #2
	rors r3, r4
	tst r2, r3
	beq .L080D0FF8
	lsrs r4, r1, #2
	adds r0, r0, r4
.L080D0FF8:
	mov r3, ip
	movs r4, #1
	rors r3, r4
	tst r2, r3
	beq .L080D1006
	lsrs r4, r1, #1
	adds r0, r0, r4
.L080D1006:
	pop {r4}
	bx lr
	.align 2, 0

	thumb_func_start func_080D100C
func_080D100C: @ 0x080D100C
	ldr r2, .L080D1018 @ =gUnk_03000450
	lsls r0, r0, #2
	adds r2, r2, r0
	ldr r0, [r2]
	str r1, [r2]
	bx lr
	.align 2, 0
.L080D1018: .4byte gUnk_03000450

	thumb_func_start func_080D101C
func_080D101C: @ 0x080D101C
	ldr r2, .L080D1028 @ =gUnk_03000430
	lsls r0, r0, #1
	adds r2, r2, r0
	ldrh r0, [r2]
	strh r1, [r2]
	bx lr
	.align 2, 0
.L080D1028: .4byte gUnk_03000430

