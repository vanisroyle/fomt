    .section .rodata

	.global init_array
init_array:
	.word 0, 0, 0, 0

	@ "bad_alloc"
	.incbin "baserom.gba", 0xE8620, 0x0C

	.global gSaveHeaderSignature
gSaveHeaderSignature:
	.incbin "baserom.gba", 0xE862C, 0x58

	.global gUnk_080E8684
gUnk_080E8684:
	.include "build/assets/text-tables-0E8684.s" @ slot 0xe8684, 0x3c

	.global gUnk_080E86C0
.set gUnk_080E86C0, gNonScriptText_0E86C0
	.include "build/assets/text-tables-0E86C0.s" @ slot 0xe86c0, 0x2c

	.global gUnk_080E86EC
.set gUnk_080E86EC, gNonScriptText_0E86EC
	.include "build/assets/text-tables-0E86EC.s" @ slot 0xe86ec, 0x4

	.global gUnk_080E86F0
.set gUnk_080E86F0, gNonScriptText_0E86F0
	.include "build/assets/text-tables-0E86F0.s" @ slot 0xe86f0, 0x14

	.global gUnk_080E8704
gUnk_080E8704:
	.include "build/assets/text-tables-0E8704.s" @ slot 0xe8704, 0x48

	.global gUnk_080E874C
.set gUnk_080E874C, gNonScriptText_0E874C
	.include "build/assets/text-tables-0E874C.s" @ slot 0xE874C, 0x8

	.global gUnk_080E8754
.set gUnk_080E8754, gNonScriptText_0E8754
	.include "build/assets/text-tables-0E8754.s" @ slot 0xE8754, 0x4

	.global gUnk_080E8758
.set gUnk_080E8758, gNonScriptText_0E8758
	.include "build/assets/text-tables-0E8758.s" @ slot 0xE8758, 0x8

	.global gUnk_080E8760
.set gUnk_080E8760, gNonScriptText_0E8760
	.include "build/assets/text-tables-0E8760.s" @ slot 0xE8760, 0x4

	.global gUnk_080E8764
.set gUnk_080E8764, gNonScriptText_0E8764
	.include "build/assets/text-tables-0E8764.s" @ slot 0xE8764, 0x4

	.global gUnk_080E8768
.set gUnk_080E8768, gNonScriptText_0E8768
	.include "build/assets/text-tables-0E8768.s" @ slot 0xE8768, 0x8

	.global gUnk_080E8770
.set gUnk_080E8770, gNonScriptText_0E8770
	.include "build/assets/text-tables-0E8770.s" @ slot 0xe8770, 0xc

	.global gUnk_080E877C
.set gUnk_080E877C, gNonScriptText_0E877C
	.include "build/assets/text-tables-0E877C.s" @ slot 0xe877c, 0x10

	.global gUnk_080E878C
.set gUnk_080E878C, gNonScriptText_0E878C
	.include "build/assets/text-tables-0E878C.s" @ slot 0xe878c, 0x18

	.global gUnk_080E87A4
.set gUnk_080E87A4, gNonScriptText_0E87A4
	.include "build/assets/text-tables-0E87A4.s" @ slot 0xe87a4, 0x58

	.global gUnk_080E87FC
.set gUnk_080E87FC, gNonScriptText_0E87FC
	.include "build/assets/text-tables-0E87FC.s" @ slot 0xe87fc, 0x2c

	.global gUnk_080E8828
.set gUnk_080E8828, gNonScriptText_0E8828
	.include "build/assets/text-tables-0E8828.s" @ slot 0xe8828, 0x50

	.global gUnk_080E8878
.set gUnk_080E8878, gNonScriptText_0E8878
	.include "build/assets/text-tables-0E8878.s" @ slot 0xe8878, 0x50

	.global gUnk_080E88C8
gUnk_080E88C8:
	.incbin "baserom.gba", 0xE88C8, 0x10

	.global gUnk_080E88D8
gUnk_080E88D8:
	.incbin "baserom.gba", 0xE88D8, 0x34

	.global gUnk_080E890C
.set gUnk_080E890C, gNonScriptText_0E890C
	.include "build/assets/text-tables-0E890C.s" @ slot 0xe890c, 0x8

	.global gUnk_080E8914
.set gUnk_080E8914, gNonScriptText_0E8914
	.include "build/assets/text-tables-0E8914.s" @ slot 0xe8914, 0x20

	.global gUnk_080E8934
gUnk_080E8934:
	.incbin "baserom.gba", 0xE8934, 0x8

	.global gUnk_080E893C
gUnk_080E893C:
	.incbin "baserom.gba", 0xE893C, 0x8

	.global gUnk_080E8944
gUnk_080E8944:
	.incbin "baserom.gba", 0xE8944, 0x14

	.global gUnk_080E8958
.set gUnk_080E8958, gNonScriptText_0E8958
	.include "build/assets/text-tables-0E8958.s" @ slot 0xe8958, 0x10

	.global gUnk_080E8968
.set gUnk_080E8968, gNonScriptText_0E8968
	.include "build/assets/text-tables-0E8968.s" @ slot 0xe8968, 0x10

	.global gUnk_080E8978
.set gUnk_080E8978, gNonScriptText_0E8978
	.include "build/assets/text-tables-0E8978.s" @ slot 0xe8978, 0x10

	.global gUnk_080E8988
.set gUnk_080E8988, gNonScriptText_0E8988
	.include "build/assets/text-tables-0E8988.s" @ slot 0xe8988, 0xf

	.global gUnk_080E8997
.set gUnk_080E8997, gNonScriptText_0E8997
	.include "build/assets/text-tables-0E8997.s" @ slot 0xe8997, 0x1

	.global gUnk_080E8998
.set gUnk_080E8998, gNonScriptText_0E8998
	.include "build/assets/text-tables-0E8998.s" @ slot 0xe8998, 0xc

	.global gUnk_080E89A4
.set gUnk_080E89A4, gNonScriptText_0E89A4
	.include "build/assets/text-tables-0E89A4.s" @ slot 0xe89a4, 0x4

	.global gUnk_080E89A8
.set gUnk_080E89A8, gNonScriptText_0E89A8
	.include "build/assets/text-tables-0E89A8.s" @ slot 0xe89a8, 0xe

	.global gUnk_080E89B6
.set gUnk_080E89B6, gNonScriptText_0E89B6
	.include "build/assets/text-tables-0E89B6.s" @ slot 0xe89b6, 0x22e

	.global gUnk_080E8BE4
.set gUnk_080E8BE4, gNonScriptText_0E8BE4
	.include "build/assets/text-tables-0E8BE4.s" @ slot 0xe8be4, 0x10

	.global gUnk_080E8BF4
.set gUnk_080E8BF4, gNonScriptText_0E8BF4
	.include "build/assets/text-tables-0E8BF4.s" @ slot 0xe8bf4, 0x8

	.global gUnk_080E8BFC
.set gUnk_080E8BFC, gNonScriptText_0E8BFC
	.include "build/assets/text-tables-0E8BFC.s" @ slot 0xe8bfc, 0x18

	.global gUnk_080E8C14
.set gUnk_080E8C14, gNonScriptText_0E8C14
	.include "build/assets/text-tables-0E8C14.s" @ slot 0xe8c14, 0x18

	.global gUnk_080E8C2C
.set gUnk_080E8C2C, gNonScriptText_0E8C2C
	.include "build/assets/text-tables-0E8C2C.s" @ slot 0xe8c2c, 0x18

	.global gUnk_080E8C44
.set gUnk_080E8C44, gNonScriptText_0E8C44
	.include "build/assets/text-tables-0E8C44.s" @ slot 0xe8c44, 0x4

	.global gUnk_080E8C48
.set gUnk_080E8C48, gNonScriptText_0E8C48
	.include "build/assets/text-tables-0E8C48.s" @ slot 0xe8c48, 0x7c

	.global gUnk_080E8CC4
gUnk_080E8CC4:
	.incbin "baserom.gba", 0xE8CC4, 0x40

	.global gUnk_080E8D04
gUnk_080E8D04:
	.incbin "baserom.gba", 0xE8D04, 0x8

	.global gUnk_080E8D0C
gUnk_080E8D0C:
	.incbin "baserom.gba", 0xE8D0C, 0x8

	.global gUnk_080E8D14
gUnk_080E8D14:
	.incbin "baserom.gba", 0xE8D14, 0x6E4

	.global gUnk_080E93F8
gUnk_080E93F8:
	.incbin "baserom.gba", 0xE93F8, 0x1D4
