    .section .rodata

	@ schedules

	@ .incbin "baserom.gba", 0xF1AC0, 0x500

	.incbin "baserom.gba", 0xF1FB4, 12 @ bad_alloc

	.global gUnk_080F1FC0
gUnk_080F1FC0:
	.incbin "baserom.gba", 0xF1FC0, 0x84C

	.global gUnk_080F280C
gUnk_080F280C:
	.incbin "baserom.gba", 0xF280C, 0x1B4

	.global gUnk_080F29C0
gUnk_080F29C0:
	.incbin "baserom.gba", 0xF29C0, 0x138

	.global gCliffScheduleInfo
gCliffScheduleInfo:
	.4byte SelectCliffSchedule
	.4byte 6, .LCliffSchedules
.LCliffSchedules: @ In CliffSchedule order.
	.4byte 0, .LCliffRegularSchedule, .LCliffWorkingSchedule
	.4byte .LCliffEmployedSaturdaySchedule, .LCliffFestivalSchedule, .LCliffStoryOverrideSchedule

.LCliffRegularSchedule:
	.2byte 5, 0
	.4byte .LCliffRegularEntries
.LCliffWorkingSchedule:
	.2byte 9, 0
	.4byte .LCliffWorkingEntries
.LCliffEmployedSaturdaySchedule:
	.2byte 6, 0
	.4byte .LCliffEmployedSaturdayEntries
.LCliffFestivalSchedule:
	.2byte 1, 0
	.4byte .LCliffFestivalEntries
.LCliffStoryOverrideSchedule:
	.2byte 1, 0
	.4byte .LCliffStoryOverrideEntries

	.macro cliff_schedule_entry time, path
	.2byte \time, 0
	.4byte \path
	.endm

.LCliffRegularEntries:
	cliff_schedule_entry 0, .LCliffStartOfDay
	cliff_schedule_entry 180, .LCliffRegularDeparture
	cliff_schedule_entry 224, .LCliffRegularMidday
	cliff_schedule_entry 605, .LCliffRegularEvening
	cliff_schedule_entry 646, .LCliffStartOfDay
.LCliffWorkingEntries:
	cliff_schedule_entry 0, .LCliffStartOfDay
	cliff_schedule_entry 240, .LCliffWorkDeparture
	cliff_schedule_entry 265, .LCliffWorkMorning
	cliff_schedule_entry 420, .LCliffWorkMidday
	cliff_schedule_entry 427, .LCliffWorkAfternoon
	cliff_schedule_entry 600, .LCliffWorkEvening
	cliff_schedule_entry 667, .LCliffRegularMidday
	cliff_schedule_entry 783, .LCliffRegularEvening
	cliff_schedule_entry 824, .LCliffStartOfDay
.LCliffEmployedSaturdayEntries:
	cliff_schedule_entry 0, .LCliffStartOfDay
	cliff_schedule_entry 420, .LCliffSaturdayOuting
	cliff_schedule_entry 611, .LCliffRegularDeparture
	cliff_schedule_entry 655, .LCliffRegularMidday
	cliff_schedule_entry 780, .LCliffSaturdayReturn
	cliff_schedule_entry 824, .LCliffStartOfDay
.LCliffFestivalEntries:
	cliff_schedule_entry 0, .LCliffStartOfDay
.LCliffStoryOverrideEntries:
	cliff_schedule_entry 0, .LCliffStoryOverridePath

.LCliffStartOfDay:
	.incbin "baserom.gba", 0xF2BF4, 0xC
.LCliffRegularDeparture:
	.incbin "baserom.gba", 0xF2C00, 0xC
.LCliffRegularMidday:
	.incbin "baserom.gba", 0xF2C0C, 0xC
.LCliffRegularEvening:
	.incbin "baserom.gba", 0xF2C18, 0xC
.LCliffWorkDeparture:
	.incbin "baserom.gba", 0xF2C24, 0xC
.LCliffWorkMorning:
	.incbin "baserom.gba", 0xF2C30, 0xC
.LCliffWorkMidday:
	.incbin "baserom.gba", 0xF2C3C, 0xC
.LCliffWorkAfternoon:
	.incbin "baserom.gba", 0xF2C48, 0xC
.LCliffWorkEvening:
	.incbin "baserom.gba", 0xF2C54, 0xC
.LCliffSaturdayOuting:
	.incbin "baserom.gba", 0xF2C60, 0xC
.LCliffSaturdayReturn:
	.incbin "baserom.gba", 0xF2C6C, 0xC
.LCliffStoryOverridePath:
	.incbin "baserom.gba", 0xF2C78, 0x148

	.global gUnk_080F2DC0
gUnk_080F2DC0:
	.incbin "baserom.gba", 0xF2DC0, 0x250

	.global gUnk_080F3010
gUnk_080F3010:
	.incbin "baserom.gba", 0xF3010, 0x3A8

	.global gUnk_080F33B8
gUnk_080F33B8:
	.incbin "baserom.gba", 0xF33B8, 0x50

	.global gUnk_080F3408
gUnk_080F3408:
	.incbin "baserom.gba", 0xF3408, 0x1DC

	.global gUnk_080F35E4
gUnk_080F35E4:
	.incbin "baserom.gba", 0xF35E4, 0x9F4

	.global gUnk_080F3FD8
gUnk_080F3FD8:
	.incbin "baserom.gba", 0xF3FD8, 0x318

	.global gUnk_080F42F0
gUnk_080F42F0:
	.incbin "baserom.gba", 0xF42F0, 0xEC

	.global gUnk_080F43DC
gUnk_080F43DC:
	.incbin "baserom.gba", 0xF43DC, 0x598

	.global gUnk_080F4974
gUnk_080F4974:
	.incbin "baserom.gba", 0xF4974, 0x4C

	.global gUnk_080F49C0
gUnk_080F49C0:
	.incbin "baserom.gba", 0xF49C0, 0x3B4

	.global gUnk_080F4D74
gUnk_080F4D74:
	.incbin "baserom.gba", 0xF4D74, 0x7CC

	.global gUnk_080F5540
gUnk_080F5540:
	.incbin "baserom.gba", 0xF5540, 0x43C

	.global gUnk_080F597C
gUnk_080F597C:
	.incbin "baserom.gba", 0xF597C, 0x50

	.global gUnk_080F59CC
gUnk_080F59CC:
	.incbin "baserom.gba", 0xF59CC, 0x3C8

	.global gUnk_080F5D94
gUnk_080F5D94:
	.incbin "baserom.gba", 0xF5D94, 0x468

	.global gUnk_080F61FC
gUnk_080F61FC:
	.incbin "baserom.gba", 0xF61FC, 0x174

	.global gUnk_080F6370
gUnk_080F6370:
	.incbin "baserom.gba", 0xF6370, 0x354

	.global gUnk_080F66C4
gUnk_080F66C4:
	.incbin "baserom.gba", 0xF66C4, 0x44C

	.global gUnk_080F6B10
gUnk_080F6B10:
	.incbin "baserom.gba", 0xF6B10, 0x3C

	.global gUnk_080F6B4C
gUnk_080F6B4C:
	.incbin "baserom.gba", 0xF6B4C, 0x29C

	.global gUnk_080F6DE8
gUnk_080F6DE8:
	.incbin "baserom.gba", 0xF6DE8, 0x210

	.global gUnk_080F6FF8
gUnk_080F6FF8:
	.incbin "baserom.gba", 0xF6FF8, 0x29C

	.global gUnk_080F7294
gUnk_080F7294:
	.incbin "baserom.gba", 0xF7294, 0x568

	.global gUnk_080F77FC
gUnk_080F77FC:
	.incbin "baserom.gba", 0xF77FC, 0x344

	.global gCarpenterScheduleInfo
gCarpenterScheduleInfo:
	.incbin "baserom.gba", 0xF7B40, 0x67C

	.global gUnk_080F81BC
gUnk_080F81BC:
	.incbin "baserom.gba", 0xF81BC, 0x4BC

	.global gUnk_080F8678
gUnk_080F8678:
	.incbin "baserom.gba", 0xF8678, 0x35C

	.global gUnk_080F89D4 @ script table
gUnk_080F89D4:
	.incbin "baserom.gba", 0xF89D4, 0x14C4
