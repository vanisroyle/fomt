    .section .rodata

    .LDATA_BEG = 0x13B288

	.include "build/assets/audio-bank.s"
	.include "build/assets/scripts.s"

	.global gUnk_084F90CC
gUnk_084F90CC:
	.incbin "build/assets/font-small.bin"

	.global gUnk_084FA7A0
gUnk_084FA7A0:
	.incbin "build/assets/font-single.bin"

	.global gUnk_084FA9A0
gUnk_084FA9A0:
	.incbin "build/assets/font-large.bin"

	.global gUnk_08523290
gUnk_08523290:
	.incbin "build/assets/font-double.bin"

	.global gUnk_08527094
gUnk_08527094:
	.incbin "baserom.gba", 0x527094, 0x1A4

	.global gUnk_08527238
.set gUnk_08527238, gUiGraphics_527238
	.include "build/assets/graphics-ui-527238.s"

	.global gUnk_08527348
.set gUnk_08527348, gUiGraphics_527348
	.include "build/assets/graphics-ui-527348.s"

	.global gUnk_08527520
.set gUnk_08527520, gUiGraphics_527520
	.include "build/assets/graphics-ui-527520.s"

	.global gUnk_085275D0
.set gUnk_085275D0, gUiGraphics_5275D0
	.include "build/assets/graphics-ui-5275D0.s"

	.global gUnk_08529794
.set gUnk_08529794, gUiGraphics_529794
	.include "build/assets/graphics-ui-529794.s"

	.global gUnk_08529994
gUnk_08529994:
	.incbin "baserom.gba", 0x529994, 0x214

	.global gUnk_08529BA8
gUnk_08529BA8:
	.incbin "baserom.gba", 0x529BA8, 0x1FC

	.global gUnk_08529DA4
gUnk_08529DA4:
	.incbin "baserom.gba", 0x529DA4, 0x934

	.global gUnk_0852A6D8
.set gUnk_0852A6D8, gUiGraphics_52A6D8
	.include "build/assets/graphics-ui-52A6D8.s"

	.global gUnk_0852A7EC
.set gUnk_0852A7EC, gUiGraphics_52A7EC
	.include "build/assets/graphics-ui-52A7EC.s"

	.global gUnk_0852A9BC
.set gUnk_0852A9BC, gUiGraphics_52A9BC
	.include "build/assets/graphics-ui-52A9BC.s"

	.global gUnk_0852AA6C
.set gUnk_0852AA6C, gUiGraphics_52AA6C
	.include "build/assets/graphics-ui-52AA6C.s"

	.global gUnk_0852CA40
.set gUnk_0852CA40, gUiGraphics_52CA40
	.include "build/assets/graphics-ui-52CA40.s"

	.global gUnk_0852CC40
gUnk_0852CC40:
	.incbin "baserom.gba", 0x52CC40, 0x214

	.global gUnk_0852CE54
gUnk_0852CE54:
	.incbin "baserom.gba", 0x52CE54, 0x1FC

	.global gUnk_0852D050
gUnk_0852D050:
	.incbin "baserom.gba", 0x52D050, 0x934

	.global gUnk_0852D984
gUnk_0852D984:
	.incbin "build/assets/graphics-portraits.bin"

	.global gUnk_0858BA28
gUnk_0858BA28:
	.incbin "baserom.gba", 0x58BA28, 0xDB638

	.global gUnk_08667060
gUnk_08667060:
	.incbin "baserom.gba", 0x667060, 0x840

	.global gUnk_086678A0
gUnk_086678A0:
	.include "build/assets/graphics-more-6678A0.s"

	.global gUnk_08697920
gUnk_08697920:
	.incbin "baserom.gba", 0x697920, 0x11E0

	.global gUnk_08698B00
gUnk_08698B00:
	.incbin "baserom.gba", 0x698B00, 0x314

	.global gUnk_08698E14
gUnk_08698E14:
	.incbin "baserom.gba", 0x698E14, 0x11E0

	.global gUnk_08699FF4
gUnk_08699FF4:
	.incbin "baserom.gba", 0x699FF4, 0xB0

	.global gUnk_0869A0A4
gUnk_0869A0A4:
	.incbin "baserom.gba", 0x69A0A4, 0x11E0

	.global gUnk_0869B284
gUnk_0869B284:
	.include "build/assets/graphics-more-69B284.s"

	.global gUnk_086D5508
gUnk_086D5508:
	.incbin "baserom.gba", 0x6D5508, 0xE60

	.global gUnk_086D6368
gUnk_086D6368:
	.incbin "baserom.gba", 0x6D6368, 0xF0

	.global gUnk_086D6458
gUnk_086D6458:
	.incbin "baserom.gba", 0x6D6458, 0xC0

	.global gUnk_086D6518
gUnk_086D6518:
	.incbin "baserom.gba", 0x6D6518, 0x8

	.global gUnk_086D6520
gUnk_086D6520:
	.incbin "baserom.gba", 0x6D6520, 0x8

	.global gUnk_086D6528
gUnk_086D6528:
	.incbin "baserom.gba", 0x6D6528, 0x20

	.global gUnk_086D6548
gUnk_086D6548:
	.incbin "baserom.gba", 0x6D6548, 0xC0

	.global gUnk_086D6608
gUnk_086D6608:
	.incbin "baserom.gba", 0x6D6608, 0x90

	.global gUnk_086D6698
gUnk_086D6698:
	.incbin "baserom.gba", 0x6D6698, 0xE60

	.global gUnk_086D74F8
gUnk_086D74F8:
	.include "build/assets/graphics-more-6D74F8.s"

	.global gMineSoilTiles
gMineSoilTiles:
	.incbin "baserom.gba", 0x6DC3C4, 0xC

	.global gMineDugSoilTiles
gMineDugSoilTiles:
	.incbin "baserom.gba", 0x6DC3D0, 0xC

	.global gMineDownStairsTiles
gMineDownStairsTiles:
	.incbin "baserom.gba", 0x6DC3DC, 0xC

	.global gMineUpStairsTiles
gMineUpStairsTiles:
	.incbin "baserom.gba", 0x6DC3E8, 0xC

	.global gMineRockTiles
gMineRockTiles:
	.include "build/assets/graphics-more-6DC3F4.s"

	.global gUnk_086DC54C
gUnk_086DC54C:
	.incbin "baserom.gba", 0x6DC54C, 0x30

	.global gUnk_086DC57C
gUnk_086DC57C:
	.incbin "baserom.gba", 0x6DC57C, 0x70C

	.global gUnk_086DCC88
gUnk_086DCC88:
	.incbin "baserom.gba", 0x6DCC88, 0xA08

	.global gUnk_086DD690
gUnk_086DD690:
	.incbin "baserom.gba", 0x6DD690, 0x20

	.global gUnk_086DD6B0
gUnk_086DD6B0:
	.incbin "baserom.gba", 0x6DD6B0, 0x30

	.global gUnk_086DD6E0
gUnk_086DD6E0:
	.incbin "baserom.gba", 0x6DD6E0, 0x70C

	.global gUnk_086DDDEC
gUnk_086DDDEC:
	.incbin "baserom.gba", 0x6DDDEC, 0xA08

	.global gUnk_086DE7F4
gUnk_086DE7F4:
	.incbin "baserom.gba", 0x6DE7F4, 0xC4E8

	.global gUnk_086EACDC
gUnk_086EACDC:
	.incbin "baserom.gba", 0x6EACDC, 0x8

	.global gUnk_086EACE4
gUnk_086EACE4:
	.incbin "baserom.gba", 0x6EACE4, 0x20

	.global gUnk_086EAD04
gUnk_086EAD04:
	.incbin "baserom.gba", 0x6EAD04, 0x20

	.global gUnk_086EAD24
gUnk_086EAD24:
	.incbin "baserom.gba", 0x6EAD24, 0x20

	.global gUnk_086EAD44
gUnk_086EAD44:
	.incbin "baserom.gba", 0x6EAD44, 0x20

	.global gUnk_086EAD64
gUnk_086EAD64:
	.incbin "baserom.gba", 0x6EAD64, 0x20

	.global gUnk_086EAD84
gUnk_086EAD84:
	.incbin "baserom.gba", 0x6EAD84, 0x8

	.global gUnk_086EAD8C
gUnk_086EAD8C:
	.include "build/assets/graphics-ui-block-6EAD8C.s"

	.global gUnk_086F2FAC
gUnk_086F2FAC:
	.incbin "baserom.gba", 0x6F2FAC, 0x7AD4

	.global gUnk_086FAA80
gUnk_086FAA80:
	.incbin "baserom.gba", 0x6FAA80, 0x584

	.global gUnk_086FB004
.set gUnk_086FB004, gMoreGraphics_6FB004
	.include "build/assets/graphics-more-6FB004.s"

	.global gUnk_086FD19C
.set gUnk_086FD19C, gMoreGraphics_6FD19C
	.include "build/assets/graphics-more-6FD19C.s"

	.global gUnk_086FD240
.set gUnk_086FD240, gMoreGraphics_6FD240
	.include "build/assets/graphics-more-6FD240.s"

	.global gUnk_08714A30
.set gUnk_08714A30, gMoreGraphics_714A30
	.include "build/assets/graphics-more-714A30.s"

	.global gUnk_08714B60
.set gUnk_08714B60, gMoreGraphics_714B60
	.include "build/assets/graphics-more-714B60.s"

	.global gUnk_08714BEC
.set gUnk_08714BEC, gMoreGraphics_714BEC
	.include "build/assets/graphics-more-714BEC.s"

	.global gUnk_08716F84
.set gUnk_08716F84, gMoreGraphics_716F84
	.include "build/assets/graphics-more-716F84.s"

	.global gUnk_087170B8
.set gUnk_087170B8, gMoreGraphics_7170B8
	.include "build/assets/graphics-more-7170B8.s"

	.global gUnk_0871D51C
gUnk_0871D51C:
	.incbin "baserom.gba", 0x71D51C, 0x128C

	.global gUnk_0871E7A8
gUnk_0871E7A8:
	.incbin "baserom.gba", 0x71E7A8, 0x504

	.global gUnk_0871ECAC
gUnk_0871ECAC:
	.incbin "baserom.gba", 0x71ECAC, 0x128

	.global gUnk_0871EDD4
gUnk_0871EDD4:
	.incbin "baserom.gba", 0x71EDD4, 0x12C

	.global gUnk_0871EF00
gUnk_0871EF00:
	.incbin "baserom.gba", 0x71EF00, 0x6EA0

	.global gUnk_08725DA0
gUnk_08725DA0:
	.incbin "baserom.gba", 0x725DA0, 0xF2C

	.global gUnk_08726CCC
gUnk_08726CCC:
	.incbin "baserom.gba", 0x726CCC, 0x69C

	.global gUnk_08727368
gUnk_08727368:
	.incbin "baserom.gba", 0x727368, 0x70C

	.global gUnk_08727A74
gUnk_08727A74:
	.incbin "baserom.gba", 0x727A74, 0x794

	.global gUnk_08728208
gUnk_08728208:
	.incbin "baserom.gba", 0x728208, 0x118

	.global gUnk_08728320
gUnk_08728320:
	.incbin "baserom.gba", 0x728320, 0x105C

	.global gUnk_0872937C
gUnk_0872937C:
	.incbin "baserom.gba", 0x72937C, 0xE4

	.global gUnk_08729460
gUnk_08729460:
	.incbin "baserom.gba", 0x729460, 0x2A04

	.global gUnk_0872BE64
gUnk_0872BE64:
	.incbin "baserom.gba", 0x72BE64, 0x76C

	.global gUnk_0872C5D0
.set gUnk_0872C5D0, gUiGraphics_72C5D0
	.include "build/assets/graphics-ui-72C5D0.s"

	.global gUnk_0872C73C
.set gUnk_0872C73C, gUiGraphics_72C73C
	.include "build/assets/graphics-ui-72C73C.s"

	.global gUnk_0872C84C
.set gUnk_0872C84C, gUiGraphics_72C84C
	.include "build/assets/graphics-ui-72C84C.s"

	.global gUnk_0872D5AC
.set gUnk_0872D5AC, gUiGraphics_72D5AC
	.include "build/assets/graphics-ui-72D5AC.s"

	.global gUnk_0872D5CC
.set gUnk_0872D5CC, gUiGraphics_72D5CC
	.include "build/assets/graphics-ui-72D5CC.s"

	.global gUnk_0872D630
.set gUnk_0872D630, gUiGraphics_72D630
	.include "build/assets/graphics-ui-72D630.s"

	.global gUnk_0872D6D8
.set gUnk_0872D6D8, gUiGraphics_72D6D8
	.include "build/assets/graphics-ui-72D6D8.s"

	.global gUnk_0872DDE4
.set gUnk_0872DDE4, gUiGraphics_72DDE4
	.include "build/assets/graphics-ui-72DDE4.s"

	.global gUnk_0872DE44
gUnk_0872DE44:
	.incbin "baserom.gba", 0x72DE44, 0x1034

	.global gUnk_0872EE78
gUnk_0872EE78:
	.incbin "baserom.gba", 0x72EE78, 0x2A4

	.global gUnk_0872F11C
.set gUnk_0872F11C, gUiGraphics_72F11C
	.include "build/assets/graphics-ui-72F11C.s"

	.global gUnk_0872F1BC
.set gUnk_0872F1BC, gUiGraphics_72F1BC
	.include "build/assets/graphics-ui-72F1BC.s"

	.global gUnk_0872F1EC
.set gUnk_0872F1EC, gUiGraphics_72F1EC
	.include "build/assets/graphics-ui-72F1EC.s"

	.global gUnk_0872F21C
.set gUnk_0872F21C, gUiGraphics_72F21C
	.include "build/assets/graphics-ui-72F21C.s"

	.global gUnk_0872FA9C
.set gUnk_0872FA9C, gUiGraphics_72FA9C
	.include "build/assets/graphics-ui-72FA9C.s"

	.global gUnk_0872FBFC
.set gUnk_0872FBFC, gUiGraphics_72FBFC
	.include "build/assets/graphics-ui-72FBFC.s"

	.global gUnk_0872FC34
.set gUnk_0872FC34, gMoreGraphics_72FC34
	.include "build/assets/graphics-more-72FC34.s"

	.global gUnk_0872FE6C
.set gUnk_0872FE6C, gMoreGraphics_72FE6C
	.include "build/assets/graphics-more-72FE6C.s"

	.global gUnk_0872FEF8
.set gUnk_0872FEF8, gMoreGraphics_72FEF8
	.include "build/assets/graphics-more-72FEF8.s"

	.global gUnk_08731940
.set gUnk_08731940, gMoreGraphics_731940
	.include "build/assets/graphics-more-731940.s"

	.global gUnk_08731B40 @ChickenPortraits
gUnk_08731B40:
	.incbin "baserom.gba", 0x731B40, 0x6604

	.global gUnk_08738144 @gCalendarFont?
gUnk_08738144:
	.incbin "baserom.gba", 0x738144, 0x994

	.global gUnk_08738AD8
.set gUnk_08738AD8, gUiGraphics_738AD8
	.include "build/assets/graphics-ui-738AD8.s"

	.global gUnk_08738CC8
.set gUnk_08738CC8, gUiGraphics_738CC8
	.include "build/assets/graphics-ui-738CC8.s"

	.global gUnk_08738CF0
.set gUnk_08738CF0, gUiGraphics_738CF0
	.include "build/assets/graphics-ui-738CF0.s"

	.global gUnk_08738D1C
.set gUnk_08738D1C, gUiGraphics_738D1C
	.include "build/assets/graphics-ui-738D1C.s"

	.global gUnk_087399C4
.set gUnk_087399C4, gUiGraphics_7399C4
	.include "build/assets/graphics-ui-7399C4.s"

	.global gUnk_08739A64
.set gUnk_08739A64, gUiGraphics_739A64
	.include "build/assets/graphics-ui-739A64.s"

	.global gUnk_0873A6E8
gUnk_0873A6E8:
	.incbin "baserom.gba", 0x73A6E8, 0x76C

	.global gUnk_0873AE54
gUnk_0873AE54:
	.incbin "baserom.gba", 0x73AE54, 0x174

	.global gUnk_0873AFC8
gUnk_0873AFC8:
	.incbin "baserom.gba", 0x73AFC8, 0xE5C

	.global gUnk_0873BE24
.set gUnk_0873BE24, gUiGraphics_73BE24
	.include "build/assets/graphics-ui-73BE24.s"

	.global gUnk_0873BF48
.set gUnk_0873BF48, gUiGraphics_73BF48
	.include "build/assets/graphics-ui-73BF48.s"

	.global gUnk_0873BFF0
.set gUnk_0873BFF0, gUiGraphics_73BFF0
	.include "build/assets/graphics-ui-73BFF0.s"

	.global gUnk_0873CBF4
.set gUnk_0873CBF4, gUiGraphics_73CBF4
	.include "build/assets/graphics-ui-73CBF4.s"

	.global gUnk_0873CCB4
gUnk_0873CCB4:
	.incbin "baserom.gba", 0x73CCB4, 0x1F8

	.global gUnk_0873CEAC
gUnk_0873CEAC:
	.incbin "baserom.gba", 0x73CEAC, 0xE4

	.global gUnk_0873CF90
gUnk_0873CF90:
	.incbin "baserom.gba", 0x73CF90, 0x2A4

	.global gUnk_0873D234
gUnk_0873D234:
	.incbin "baserom.gba", 0x73D234, 0x3C8

	.global gUnk_0873D5FC
gUnk_0873D5FC:
	.incbin "baserom.gba", 0x73D5FC, 0xDC

	.global gUnk_0873D6D8
gUnk_0873D6D8:
	.incbin "baserom.gba", 0x73D6D8, 0x76C

	.global gUnk_0873DE44
gUnk_0873DE44:
	.incbin "baserom.gba", 0x73DE44, 0x76C

	.global gUnk_0873E5B0
gUnk_0873E5B0:
	.incbin "baserom.gba", 0x73E5B0, 0x76C

	.global gUnk_0873ED1C
gUnk_0873ED1C:
	.incbin "baserom.gba", 0x73ED1C, 0x6AC

	.global gUnk_0873F3C8
.set gUnk_0873F3C8, gUiGraphics_73F3C8
	.include "build/assets/graphics-ui-73F3C8.s"

	.global gUnk_0873F5D4
.set gUnk_0873F5D4, gUiGraphics_73F5D4
	.include "build/assets/graphics-ui-73F5D4.s"

	.global gUnk_0873F6AC
.set gUnk_0873F6AC, gUiGraphics_73F6AC
	.include "build/assets/graphics-ui-73F6AC.s"

	.global gUnk_087400E4
.set gUnk_087400E4, gUiGraphics_7400E4
	.include "build/assets/graphics-ui-7400E4.s"

	.global gUnk_087401A4
gUnk_087401A4:
	.incbin "baserom.gba", 0x7401A4, 0x2B0

	.global gUnk_08740454
gUnk_08740454:
	.incbin "baserom.gba", 0x740454, 0x14C

	.global gUnk_087405A0
gUnk_087405A0:
	.incbin "baserom.gba", 0x7405A0, 0x368

	.global gUnk_08740908
gUnk_08740908:
	.incbin "baserom.gba", 0x740908, 0xDC

	.global gUnk_087409E4
gUnk_087409E4:
	.incbin "baserom.gba", 0x7409E4, 0x1010
	.include "build/assets/graphics-rest-maps.s"

	.global gUnk_08743058
.set gUnk_08743058, gRestGraphics_743058
	.include "build/assets/graphics-rest-743058.s"

	.global gUnk_08747A14
.set gUnk_08747A14, gRestGraphics_747A14
	.include "build/assets/graphics-rest-747A14.s"
	.incbin "baserom.gba", 0x747A54, 0x20

	.include "build/assets/graphics-title-resource.s"
	.include "build/assets/graphics-title-map-0.s"
	.include "build/assets/graphics-title-map-1.s"
	.include "build/assets/graphics-title-map-2.s"
	.include "build/assets/graphics-title-map-3.s"

	.include "build/assets/graphics-title-background.s"

	.include "build/assets/graphics-title-palette.s"

	.global gUnk_0874E530
gUnk_0874E530:
	.incbin "baserom.gba", 0x74E530, 0x118

	.include "build/assets/graphics.s"

	.global gUnk_0874ECCC
.set gUnk_0874ECCC, gUiGraphics_74ECCC
	.include "build/assets/graphics-ui-74ECCC.s"

	.global gUnk_0874EE38
gUnk_0874EE38:
	.incbin "baserom.gba", 0x74EE38, 0xDC

	.global gUnk_0874EF14
.set gUnk_0874EF14, gUiGraphics_74EF14
	.include "build/assets/graphics-ui-74EF14.s"

	.global gUnk_0874EF3C
.set gUnk_0874EF3C, gUiGraphics_74EF3C
	.include "build/assets/graphics-ui-74EF3C.s"

	.global gUnk_0874EFEC
.set gUnk_0874EFEC, gUiGraphics_74EFEC
	.include "build/assets/graphics-ui-74EFEC.s"

	.global gUnk_0874F050
.set gUnk_0874F050, gUiGraphics_74F050
	.include "build/assets/graphics-ui-74F050.s"

	.global gUnk_0874F2EC
.set gUnk_0874F2EC, gUiGraphics_74F2EC
	.include "build/assets/graphics-ui-74F2EC.s"

	.global gUnk_0874F34C
gUnk_0874F34C:
	.incbin "baserom.gba", 0x74F34C, 0x1394

	.global gUnk_087506E0
gUnk_087506E0:
	.incbin "baserom.gba", 0x7506E0, 0x56C

	.global gUnk_08750C4C
gUnk_08750C4C:
	.incbin "baserom.gba", 0x750C4C, 0x20

	.global gUnk_08750C6C
gUnk_08750C6C:
	.incbin "baserom.gba", 0x750C6C, 0x20

	.global gUnk_08750C8C
gUnk_08750C8C:
	.incbin "baserom.gba", 0x750C8C, 0x1C0

	.global gUnk_08750E4C
gUnk_08750E4C:
	.incbin "baserom.gba", 0x750E4C, 0x120

	.global gUnk_08750F6C
gUnk_08750F6C:
	.incbin "baserom.gba", 0x750F6C, 0x20

	.global gUnk_08750F8C
gUnk_08750F8C:
	.incbin "baserom.gba", 0x750F8C, 0x120

	.global gUnk_087510AC
gUnk_087510AC:
	.incbin "baserom.gba", 0x7510AC, 0x120

	.global gUnk_087511CC
gUnk_087511CC:
	.incbin "baserom.gba", 0x7511CC, 0x120

	.global gUnk_087512EC
gUnk_087512EC:
	.incbin "baserom.gba", 0x7512EC, 0x20

	.global gUnk_0875130C
gUnk_0875130C:
	.incbin "baserom.gba", 0x75130C, 0x120

	.global gUnk_0875142C
gUnk_0875142C:
	.incbin "baserom.gba", 0x75142C, 0x120

	.global gUnk_0875154C
gUnk_0875154C:
	.incbin "baserom.gba", 0x75154C, 0x120

	.global gUnk_0875166C
gUnk_0875166C:
	.incbin "baserom.gba", 0x75166C, 0x120

	.global gUnk_0875178C
gUnk_0875178C:
	.incbin "baserom.gba", 0x75178C, 0x20

	.global gUnk_087517AC
gUnk_087517AC:
	.incbin "baserom.gba", 0x7517AC, 0x120

	.global gUnk_087518CC
gUnk_087518CC:
	.incbin "baserom.gba", 0x7518CC, 0x80

	.global gUnk_0875194C
gUnk_0875194C:
	.incbin "baserom.gba", 0x75194C, 0x20

	.global gUnk_0875196C
gUnk_0875196C:
	.incbin "baserom.gba", 0x75196C, 0x80

	.global gUnk_087519EC
gUnk_087519EC:
	.incbin "baserom.gba", 0x7519EC, 0x20

	.global gUnk_08751A0C
gUnk_08751A0C:
	.incbin "baserom.gba", 0x751A0C, 0x80

	.global gUnk_08751A8C
gUnk_08751A8C:
	.incbin "baserom.gba", 0x751A8C, 0x20

	.global gUnk_08751AAC
gUnk_08751AAC:
	.incbin "baserom.gba", 0x751AAC, 0x80

	.global gUnk_08751B2C
gUnk_08751B2C:
	.incbin "baserom.gba", 0x751B2C, 0x20

	.global gUnk_08751B4C
gUnk_08751B4C:
	.incbin "baserom.gba", 0x751B4C, 0x80

	.global gUnk_08751BCC
gUnk_08751BCC:
	.incbin "baserom.gba", 0x751BCC, 0x20

	.global gUnk_08751BEC
gUnk_08751BEC:
	.incbin "baserom.gba", 0x751BEC, 0x80

	.global gUnk_08751C6C
gUnk_08751C6C:
	.incbin "baserom.gba", 0x751C6C, 0x20

	.global gUnk_08751C8C
gUnk_08751C8C:
	.incbin "baserom.gba", 0x751C8C, 0x80

	.global gUnk_08751D0C
gUnk_08751D0C:
	.incbin "baserom.gba", 0x751D0C, 0x20

	.global gUnk_08751D2C
gUnk_08751D2C:
	.incbin "baserom.gba", 0x751D2C, 0x80

	.global gUnk_08751DAC
gUnk_08751DAC:
	.incbin "baserom.gba", 0x751DAC, 0x20

	.global gUnk_08751DCC
gUnk_08751DCC:
	.incbin "baserom.gba", 0x751DCC, 0x80

	.global gUnk_08751E4C
gUnk_08751E4C:
	.incbin "baserom.gba", 0x751E4C, 0x20

	.global gUnk_08751E6C
gUnk_08751E6C:
	.incbin "baserom.gba", 0x751E6C, 0x80

	.global gUnk_08751EEC
gUnk_08751EEC:
	.incbin "baserom.gba", 0x751EEC, 0x20

	.global gUnk_08751F0C
gUnk_08751F0C:
	.incbin "baserom.gba", 0x751F0C, 0x80

	.global gUnk_08751F8C
gUnk_08751F8C:
	.incbin "baserom.gba", 0x751F8C, 0x20

	.global gUnk_08751FAC
gUnk_08751FAC:
	.incbin "baserom.gba", 0x751FAC, 0x80

	.global gUnk_0875202C
gUnk_0875202C:
	.incbin "baserom.gba", 0x75202C, 0x20

	.global gUnk_0875204C
gUnk_0875204C:
	.incbin "baserom.gba", 0x75204C, 0x80

	.global gUnk_087520CC
gUnk_087520CC:
	.incbin "baserom.gba", 0x7520CC, 0x20

	.global gUnk_087520EC
gUnk_087520EC:
	.incbin "baserom.gba", 0x7520EC, 0x80

	.global gUnk_0875216C
gUnk_0875216C:
	.incbin "baserom.gba", 0x75216C, 0x20

	.global gUnk_0875218C
gUnk_0875218C:
	.incbin "baserom.gba", 0x75218C, 0x80

	.global gUnk_0875220C
gUnk_0875220C:
	.incbin "baserom.gba", 0x75220C, 0x20

	.global gUnk_0875222C
gUnk_0875222C:
	.incbin "baserom.gba", 0x75222C, 0x80

	.global gUnk_087522AC
gUnk_087522AC:
	.incbin "baserom.gba", 0x7522AC, 0x20

	.global gUnk_087522CC
gUnk_087522CC:
	.incbin "baserom.gba", 0x7522CC, 0x80

	.global gUnk_0875234C
gUnk_0875234C:
	.incbin "baserom.gba", 0x75234C, 0x20

	.global gUnk_0875236C
gUnk_0875236C:
	.incbin "baserom.gba", 0x75236C, 0x80

	.global gUnk_087523EC
gUnk_087523EC:
	.incbin "baserom.gba", 0x7523EC, 0x20

	.global gUnk_0875240C
gUnk_0875240C:
	.incbin "baserom.gba", 0x75240C, 0x80

	.global gUnk_0875248C
gUnk_0875248C:
	.incbin "baserom.gba", 0x75248C, 0x20

	.global gUnk_087524AC
gUnk_087524AC:
	.incbin "baserom.gba", 0x7524AC, 0x80

	.global gUnk_0875252C
gUnk_0875252C:
	.incbin "baserom.gba", 0x75252C, 0x20

	.global gUnk_0875254C
gUnk_0875254C:
	.incbin "baserom.gba", 0x75254C, 0x80

	.global gUnk_087525CC
gUnk_087525CC:
	.incbin "baserom.gba", 0x7525CC, 0x20

	.global gUnk_087525EC
gUnk_087525EC:
	.incbin "baserom.gba", 0x7525EC, 0x80

	.global gUnk_0875266C
gUnk_0875266C:
	.incbin "baserom.gba", 0x75266C, 0x20

	.global gUnk_0875268C
gUnk_0875268C:
	.incbin "baserom.gba", 0x75268C, 0x80

	.global gUnk_0875270C
gUnk_0875270C:
	.incbin "baserom.gba", 0x75270C, 0x20

	.global gUnk_0875272C
gUnk_0875272C:
	.incbin "baserom.gba", 0x75272C, 0x80

	.global gUnk_087527AC
gUnk_087527AC:
	.incbin "baserom.gba", 0x7527AC, 0x20

	.global gUnk_087527CC
gUnk_087527CC:
	.incbin "baserom.gba", 0x7527CC, 0x80

	.global gUnk_0875284C
gUnk_0875284C:
	.incbin "baserom.gba", 0x75284C, 0x20

	.global gUnk_0875286C
gUnk_0875286C:
	.incbin "baserom.gba", 0x75286C, 0x80

	.global gUnk_087528EC
gUnk_087528EC:
	.incbin "baserom.gba", 0x7528EC, 0x20

	.global gUnk_0875290C
gUnk_0875290C:
	.incbin "baserom.gba", 0x75290C, 0x80

	.global gUnk_0875298C
gUnk_0875298C:
	.incbin "baserom.gba", 0x75298C, 0x20

	.global gUnk_087529AC
gUnk_087529AC:
	.incbin "baserom.gba", 0x7529AC, 0x80

	.global gUnk_08752A2C
gUnk_08752A2C:
	.incbin "baserom.gba", 0x752A2C, 0x80

	.global gUnk_08752AAC
gUnk_08752AAC:
	.incbin "baserom.gba", 0x752AAC, 0x20

	.global gUnk_08752ACC
gUnk_08752ACC:
	.incbin "baserom.gba", 0x752ACC, 0x80

	.global gUnk_08752B4C
gUnk_08752B4C:
	.incbin "baserom.gba", 0x752B4C, 0x80

	.global gUnk_08752BCC
gUnk_08752BCC:
	.incbin "baserom.gba", 0x752BCC, 0x80

	.global gUnk_08752C4C
gUnk_08752C4C:
	.incbin "baserom.gba", 0x752C4C, 0x80

	.global gUnk_08752CCC
gUnk_08752CCC:
	.incbin "baserom.gba", 0x752CCC, 0x80

	.global gUnk_08752D4C
gUnk_08752D4C:
	.incbin "baserom.gba", 0x752D4C, 0x80

	.global gUnk_08752DCC
gUnk_08752DCC:
	.incbin "baserom.gba", 0x752DCC, 0x80

	.global gUnk_08752E4C
gUnk_08752E4C:
	.incbin "baserom.gba", 0x752E4C, 0x80

	.global gUnk_08752ECC
gUnk_08752ECC:
	.incbin "baserom.gba", 0x752ECC, 0x20

	.global gUnk_08752EEC
gUnk_08752EEC:
	.incbin "baserom.gba", 0x752EEC, 0x80

	.global gUnk_08752F6C
gUnk_08752F6C:
	.incbin "baserom.gba", 0x752F6C, 0x20

	.global gUnk_08752F8C
gUnk_08752F8C:
	.incbin "baserom.gba", 0x752F8C, 0x80

	.global gUnk_0875300C
gUnk_0875300C:
	.incbin "baserom.gba", 0x75300C, 0x20

	.global gUnk_0875302C
gUnk_0875302C:
	.incbin "baserom.gba", 0x75302C, 0x80

	.global gUnk_087530AC
gUnk_087530AC:
	.incbin "baserom.gba", 0x7530AC, 0x20

	.global gUnk_087530CC
gUnk_087530CC:
	.incbin "baserom.gba", 0x7530CC, 0x80

	.global gUnk_0875314C
gUnk_0875314C:
	.incbin "baserom.gba", 0x75314C, 0x20

	.global gUnk_0875316C
gUnk_0875316C:
	.incbin "baserom.gba", 0x75316C, 0x80

	.global gUnk_087531EC
gUnk_087531EC:
	.incbin "baserom.gba", 0x7531EC, 0x20

	.global gUnk_0875320C
gUnk_0875320C:
	.incbin "baserom.gba", 0x75320C, 0x80

	.global gUnk_0875328C
gUnk_0875328C:
	.incbin "baserom.gba", 0x75328C, 0x20

	.global gUnk_087532AC
gUnk_087532AC:
	.incbin "baserom.gba", 0x7532AC, 0x80

	.global gUnk_0875332C
gUnk_0875332C:
	.incbin "baserom.gba", 0x75332C, 0x20

	.global gUnk_0875334C
gUnk_0875334C:
	.incbin "baserom.gba", 0x75334C, 0x80

	.global gUnk_087533CC
gUnk_087533CC:
	.incbin "baserom.gba", 0x7533CC, 0x20

	.global gUnk_087533EC
gUnk_087533EC:
	.incbin "baserom.gba", 0x7533EC, 0x80

	.global gUnk_0875346C
gUnk_0875346C:
	.incbin "baserom.gba", 0x75346C, 0x20

	.global gUnk_0875348C
gUnk_0875348C:
	.incbin "baserom.gba", 0x75348C, 0x80

	.global gUnk_0875350C
gUnk_0875350C:
	.incbin "baserom.gba", 0x75350C, 0x20

	.global gUnk_0875352C
gUnk_0875352C:
	.incbin "baserom.gba", 0x75352C, 0xDC

	.global gUnk_08753608
gUnk_08753608:
	.incbin "baserom.gba", 0x753608, 0xDC

	.global gUnk_087536E4
gUnk_087536E4:
	.incbin "baserom.gba", 0x7536E4, 0xEC

	.global gUnk_087537D0
gUnk_087537D0:
	.incbin "baserom.gba", 0x7537D0, 0xEA4

	.global gUnk_08754674
gUnk_08754674:
	.incbin "baserom.gba", 0x754674, 0x598

	.global gUnk_08754C0C
gUnk_08754C0C:
	.incbin "baserom.gba", 0x754C0C, 0x548

	.global gUnk_08755154
gUnk_08755154:
	.incbin "baserom.gba", 0x755154, 0xDC

	.global gUnk_08755230
.set gUnk_08755230, gUiGraphics_755230
	.include "build/assets/graphics-ui-755230.s"

	.global gUnk_0875553C
.set gUnk_0875553C, gUiGraphics_75553C
	.include "build/assets/graphics-ui-75553C.s"

	.global gUnk_08755848
.set gUnk_08755848, gUiGraphics_755848
	.include "build/assets/graphics-ui-755848.s"

	.global gUnk_08757AE0
.set gUnk_08757AE0, gUiGraphics_757AE0
	.include "build/assets/graphics-ui-757AE0.s"

	.global gUnk_08757BA0
gUnk_08757BA0:
	.incbin "baserom.gba", 0x757BA0, 0xC

	.global gUnk_08757BAC
gUnk_08757BAC:
	.incbin "baserom.gba", 0x757BAC, 0x20

	.global gUnk_08757BCC
gUnk_08757BCC:
	.incbin "baserom.gba", 0x757BCC, 0xC

	.global gUnk_08757BD8
gUnk_08757BD8:
	.incbin "baserom.gba", 0x757BD8, 0x20

	.global gUnk_08757BF8
gUnk_08757BF8:
	.incbin "baserom.gba", 0x757BF8, 0x14

	.global gUnk_08757C0C
gUnk_08757C0C:
	.incbin "baserom.gba", 0x757C0C, 0x8

	.global gUnk_08757C14
.set gUnk_08757C14, gUiGraphics_757C14
	.include "build/assets/graphics-ui-757C14.s"

	.global gUnk_08757F20
.set gUnk_08757F20, gUiGraphics_757F20
	.include "build/assets/graphics-ui-757F20.s"

	.global gUnk_0875822C
.set gUnk_0875822C, gUiGraphics_75822C
	.include "build/assets/graphics-ui-75822C.s"

	.global gUnk_0875A358
.set gUnk_0875A358, gUiGraphics_75A358
	.include "build/assets/graphics-ui-75A358.s"

	.global gUnk_0875A418
gUnk_0875A418:
	.incbin "baserom.gba", 0x75A418, 0xC

	.global gUnk_0875A424
gUnk_0875A424:
	.incbin "baserom.gba", 0x75A424, 0x14

	.global gUnk_0875A438
gUnk_0875A438:
	.incbin "baserom.gba", 0x75A438, 0x8

	.global gClockFont
gClockFont:
	.incbin "baserom.gba", 0x75A440, 0x1004

	.global gUnk_0875B444
gUnk_0875B444:
	.incbin "baserom.gba", 0x75B444, 0x5B4
