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

	.global gKarenScheduleInfo
gKarenScheduleInfo:
	.4byte SelectKarenSchedule
	.4byte 24, .LKarenSchedules
.LKarenSchedules: @ In KarenSchedule order.
	.4byte 0
	.4byte .LKarenClearSchedule
	.4byte .LKarenOtherWeatherSchedule
	.4byte .LKarenSundaySchedule
	.4byte .LKarenTuesdayClearSchedule
	.4byte .LKarenTuesdayOtherWeatherSchedule
	.4byte .LKarenAfterRivalEventsSchedule
	.4byte .LKarenAfterRivalEventsSundaySchedule
	.4byte .LKarenFestivalSchedule
	.4byte .LKarenWinter14Schedule
	.4byte .LKarenAfterRivalEventsFestivalSchedule
	.4byte .LKarenMarriedActiveNpcSchedule
	.4byte .LKarenMarriedActiveNpcVariantSchedule
	.4byte .LKarenMarriedActiveNpcVariantSundayTuesdaySchedule
	.4byte .LKarenMarriedSchedule
	.4byte .LKarenMarriedVariantSchedule
	.4byte .LKarenMarriedVariantSundayTuesdaySchedule
	.4byte .LKarenMarriedActiveNpcFestivalOrStormSchedule
	.4byte .LKarenMarriedActiveNpcVariantFestivalOrStormSchedule
	.4byte .LKarenMarriedFestivalOrStormSchedule
	.4byte .LKarenMarriedVariantFestivalOrStormSchedule
	.4byte .LKarenMarriedAutumn9Schedule
	.4byte .LKarenMarriedAutumn13Schedule
	.4byte .LKarenMarriedEventOverrideSchedule

.LKarenClearSchedule:
	.2byte 13, 0
	.4byte .LKarenClearEntries
.LKarenOtherWeatherSchedule:
	.2byte 5, 0
	.4byte .LKarenOtherWeatherEntries
.LKarenSundaySchedule:
	.2byte 6, 0
	.4byte .LKarenSundayEntries
.LKarenTuesdayClearSchedule:
	.2byte 15, 0
	.4byte .LKarenTuesdayClearEntries
.LKarenTuesdayOtherWeatherSchedule:
	.2byte 13, 0
	.4byte .LKarenTuesdayOtherWeatherEntries
.LKarenAfterRivalEventsSchedule:
	.2byte 6, 0
	.4byte .LKarenAfterRivalEventsEntries
.LKarenAfterRivalEventsSundaySchedule:
	.2byte 7, 0
	.4byte .LKarenAfterRivalEventsSundayEntries
.LKarenFestivalSchedule:
	.2byte 1, 0
	.4byte .LKarenFestivalEntries
.LKarenWinter14Schedule:
	.2byte 5, 0
	.4byte .LKarenWinter14Entries
.LKarenAfterRivalEventsFestivalSchedule:
	.2byte 1, 0
	.4byte .LKarenAfterRivalEventsFestivalEntries
.LKarenMarriedActiveNpcSchedule:
	.2byte 4, 0
	.4byte .LKarenMarriedActiveNpcEntries
.LKarenMarriedActiveNpcVariantSchedule:
	.2byte 4, 0
	.4byte .LKarenMarriedActiveNpcVariantEntries
.LKarenMarriedActiveNpcVariantSundayTuesdaySchedule:
	.2byte 4, 0
	.4byte .LKarenMarriedActiveNpcVariantSundayTuesdayEntries
.LKarenMarriedSchedule:
	.2byte 4, 0
	.4byte .LKarenMarriedEntries
.LKarenMarriedVariantSchedule:
	.2byte 4, 0
	.4byte .LKarenMarriedVariantEntries
.LKarenMarriedVariantSundayTuesdaySchedule:
	.2byte 4, 0
	.4byte .LKarenMarriedVariantSundayTuesdayEntries
.LKarenMarriedActiveNpcFestivalOrStormSchedule:
	.2byte 4, 0
	.4byte .LKarenMarriedActiveNpcFestivalOrStormEntries
.LKarenMarriedActiveNpcVariantFestivalOrStormSchedule:
	.2byte 4, 0
	.4byte .LKarenMarriedActiveNpcVariantFestivalOrStormEntries
.LKarenMarriedFestivalOrStormSchedule:
	.2byte 4, 0
	.4byte .LKarenMarriedFestivalOrStormEntries
.LKarenMarriedVariantFestivalOrStormSchedule:
	.2byte 4, 0
	.4byte .LKarenMarriedVariantFestivalOrStormEntries
.LKarenMarriedAutumn9Schedule:
	.2byte 4, 0
	.4byte .LKarenMarriedAutumn9Entries
.LKarenMarriedAutumn13Schedule:
	.2byte 3, 0
	.4byte .LKarenMarriedAutumn13Entries
.LKarenMarriedEventOverrideSchedule:
	.2byte 1, 0
	.4byte .LKarenMarriedEventOverrideEntries

	@ Time is minutes since 06:00; route indices refer to the original path records.
	.macro karen_schedule_entry time, path
	.2byte \time, 0
	.4byte .LKarenPaths + (\path * 12)
	.endm

.LKarenClearEntries:
	karen_schedule_entry 0, 0
	karen_schedule_entry 120, 1
	karen_schedule_entry 126, 2
	karen_schedule_entry 240, 3
	karen_schedule_entry 246, 0
	karen_schedule_entry 420, 4
	karen_schedule_entry 600, 0
	karen_schedule_entry 780, 5
	karen_schedule_entry 826, 6
	karen_schedule_entry 843, 7
	karen_schedule_entry 960, 8
	karen_schedule_entry 978, 9
	karen_schedule_entry 1023, 0
.LKarenOtherWeatherEntries:
	karen_schedule_entry 0, 0
	karen_schedule_entry 120, 4
	karen_schedule_entry 240, 0
	karen_schedule_entry 420, 4
	karen_schedule_entry 600, 0
.LKarenSundayEntries:
	karen_schedule_entry 0, 0
	karen_schedule_entry 120, 4
	karen_schedule_entry 780, 11
	karen_schedule_entry 809, 10
	karen_schedule_entry 969, 12
	karen_schedule_entry 998, 0
.LKarenTuesdayClearEntries:
	karen_schedule_entry 0, 0
	karen_schedule_entry 120, 1
	karen_schedule_entry 126, 2
	karen_schedule_entry 240, 3
	karen_schedule_entry 246, 0
	karen_schedule_entry 420, 5
	karen_schedule_entry 466, 13
	karen_schedule_entry 480, 14
	karen_schedule_entry 600, 15
	karen_schedule_entry 615, 9
	karen_schedule_entry 660, 0
	karen_schedule_entry 780, 11
	karen_schedule_entry 809, 10
	karen_schedule_entry 969, 12
	karen_schedule_entry 998, 0
.LKarenTuesdayOtherWeatherEntries:
	karen_schedule_entry 0, 0
	karen_schedule_entry 120, 4
	karen_schedule_entry 240, 0
	karen_schedule_entry 420, 5
	karen_schedule_entry 466, 16
	karen_schedule_entry 489, 17
	karen_schedule_entry 600, 18
	karen_schedule_entry 623, 9
	karen_schedule_entry 668, 0
	karen_schedule_entry 780, 11
	karen_schedule_entry 809, 10
	karen_schedule_entry 969, 12
	karen_schedule_entry 998, 0
.LKarenAfterRivalEventsEntries:
	karen_schedule_entry 0, 20
	karen_schedule_entry 120, 21
	karen_schedule_entry 125, 22
	karen_schedule_entry 240, 23
	karen_schedule_entry 245, 19
	karen_schedule_entry 780, 20
.LKarenAfterRivalEventsSundayEntries:
	karen_schedule_entry 0, 20
	karen_schedule_entry 65, 24
	karen_schedule_entry 95, 25
	karen_schedule_entry 140, 4
	karen_schedule_entry 780, 32
	karen_schedule_entry 826, 33
	karen_schedule_entry 856, 20
.LKarenFestivalEntries:
	karen_schedule_entry 0, 0
.LKarenWinter14Entries:
	karen_schedule_entry 0, 0
	karen_schedule_entry 360, 26
	karen_schedule_entry 370, 27
	karen_schedule_entry 420, 31
	karen_schedule_entry 430, 0
.LKarenAfterRivalEventsFestivalEntries:
	karen_schedule_entry 0, 19
.LKarenMarriedActiveNpcEntries:
	karen_schedule_entry 0, 30
	karen_schedule_entry 300, 28
	karen_schedule_entry 720, 30
	karen_schedule_entry 840, 29
.LKarenMarriedActiveNpcVariantEntries:
	karen_schedule_entry 0, 30
	karen_schedule_entry 180, 4
	karen_schedule_entry 720, 30
	karen_schedule_entry 840, 29
.LKarenMarriedActiveNpcVariantSundayTuesdayEntries:
	karen_schedule_entry 0, 30
	karen_schedule_entry 300, 28
	karen_schedule_entry 720, 30
	karen_schedule_entry 840, 29
.LKarenMarriedEntries:
	karen_schedule_entry 0, 30
	karen_schedule_entry 300, 28
	karen_schedule_entry 720, 30
	karen_schedule_entry 840, 29
.LKarenMarriedVariantEntries:
	karen_schedule_entry 0, 30
	karen_schedule_entry 180, 4
	karen_schedule_entry 720, 30
	karen_schedule_entry 840, 29
.LKarenMarriedVariantSundayTuesdayEntries:
	karen_schedule_entry 0, 30
	karen_schedule_entry 300, 28
	karen_schedule_entry 720, 30
	karen_schedule_entry 840, 29
.LKarenMarriedActiveNpcFestivalOrStormEntries:
	karen_schedule_entry 0, 30
	karen_schedule_entry 300, 28
	karen_schedule_entry 720, 30
	karen_schedule_entry 840, 29
.LKarenMarriedActiveNpcVariantFestivalOrStormEntries:
	karen_schedule_entry 0, 30
	karen_schedule_entry 300, 28
	karen_schedule_entry 720, 30
	karen_schedule_entry 840, 29
.LKarenMarriedFestivalOrStormEntries:
	karen_schedule_entry 0, 30
	karen_schedule_entry 300, 28
	karen_schedule_entry 720, 30
	karen_schedule_entry 840, 29
.LKarenMarriedVariantFestivalOrStormEntries:
	karen_schedule_entry 0, 30
	karen_schedule_entry 300, 28
	karen_schedule_entry 720, 30
	karen_schedule_entry 840, 29
.LKarenMarriedAutumn9Entries:
	karen_schedule_entry 0, 30
	karen_schedule_entry 240, 34
	karen_schedule_entry 720, 30
	karen_schedule_entry 840, 29
.LKarenMarriedAutumn13Entries:
	karen_schedule_entry 0, 30
	karen_schedule_entry 300, 28
	karen_schedule_entry 720, 34
.LKarenMarriedEventOverrideEntries:
	karen_schedule_entry 0, 29

.LKarenPaths:
	.incbin "baserom.gba", 0xF3AC8, 0x510

	.global gUnk_080F3FD8
gUnk_080F3FD8:
	.incbin "baserom.gba", 0xF3FD8, 0x318

	.global gUnk_080F42F0
gUnk_080F42F0:
	.incbin "baserom.gba", 0xF42F0, 0xEC

	.global gAnnScheduleInfo
gAnnScheduleInfo:
	.4byte SelectAnnSchedule
	.4byte 19, .LAnnSchedules
.LAnnSchedules: @ In AnnSchedule order.
	.4byte 0
	.4byte .LAnnClearSchedule
	.4byte .LAnnOtherWeatherSchedule
	.4byte .LAnnAfterRivalEventsClearSchedule
	.4byte .LAnnAfterRivalEventsOtherWeatherSchedule
	.4byte .LAnnAutumn5Schedule
	.4byte .LAnnFestivalSchedule
	.4byte .LAnnWinter14Schedule
	.4byte .LAnnMarriedActiveNpcSchedule
	.4byte .LAnnMarriedActiveNpcVariantSchedule
	.4byte .LAnnMarriedSchedule
	.4byte .LAnnMarriedVariantSchedule
	.4byte .LAnnMarriedActiveNpcFestivalOrStormSchedule
	.4byte .LAnnMarriedActiveNpcVariantFestivalOrStormSchedule
	.4byte .LAnnMarriedFestivalOrStormSchedule
	.4byte .LAnnMarriedVariantFestivalOrStormSchedule
	.4byte .LAnnMarriedAutumn9Schedule
	.4byte .LAnnMarriedAutumn13Schedule
	.4byte .LAnnMarriedEventOverrideSchedule

.LAnnClearSchedule:
	.2byte 11, 0
	.4byte .LAnnClearEntries
.LAnnOtherWeatherSchedule:
	.2byte 6, 0
	.4byte .LAnnOtherWeatherEntries
.LAnnAfterRivalEventsClearSchedule:
	.2byte 11, 0
	.4byte .LAnnAfterRivalEventsClearEntries
.LAnnAfterRivalEventsOtherWeatherSchedule:
	.2byte 6, 0
	.4byte .LAnnAfterRivalEventsOtherWeatherEntries
.LAnnAutumn5Schedule:
	.2byte 3, 0
	.4byte .LAnnAutumn5Entries
.LAnnFestivalSchedule:
	.2byte 1, 0
	.4byte .LAnnFestivalEntries
.LAnnWinter14Schedule:
	.2byte 5, 0
	.4byte .LAnnWinter14Entries
.LAnnMarriedActiveNpcSchedule:
	.2byte 4, 0
	.4byte .LAnnMarriedActiveNpcEntries
.LAnnMarriedActiveNpcVariantSchedule:
	.2byte 4, 0
	.4byte .LAnnMarriedActiveNpcVariantEntries
.LAnnMarriedSchedule:
	.2byte 4, 0
	.4byte .LAnnMarriedEntries
.LAnnMarriedVariantSchedule:
	.2byte 4, 0
	.4byte .LAnnMarriedVariantEntries
.LAnnMarriedActiveNpcFestivalOrStormSchedule:
	.2byte 4, 0
	.4byte .LAnnMarriedActiveNpcFestivalOrStormEntries
.LAnnMarriedActiveNpcVariantFestivalOrStormSchedule:
	.2byte 4, 0
	.4byte .LAnnMarriedActiveNpcVariantFestivalOrStormEntries
.LAnnMarriedFestivalOrStormSchedule:
	.2byte 4, 0
	.4byte .LAnnMarriedFestivalOrStormEntries
.LAnnMarriedVariantFestivalOrStormSchedule:
	.2byte 4, 0
	.4byte .LAnnMarriedVariantFestivalOrStormEntries
.LAnnMarriedAutumn9Schedule:
	.2byte 4, 0
	.4byte .LAnnMarriedAutumn9Entries
.LAnnMarriedAutumn13Schedule:
	.2byte 3, 0
	.4byte .LAnnMarriedAutumn13Entries
.LAnnMarriedEventOverrideSchedule:
	.2byte 1, 0
	.4byte .LAnnMarriedEventOverrideEntries

	@ Time is minutes since 06:00; route indices refer to the original path records.
	.macro ann_schedule_entry time, path
	.2byte \time, 0
	.4byte .LAnnPaths + (\path * 12)
	.endm

.LAnnClearEntries:
	ann_schedule_entry 0, 1
	ann_schedule_entry 60, 3
	ann_schedule_entry 95, 4
	ann_schedule_entry 107, 5
	ann_schedule_entry 240, 6
	ann_schedule_entry 253, 7
	ann_schedule_entry 287, 2
	ann_schedule_entry 420, 0
	ann_schedule_entry 600, 2
	ann_schedule_entry 780, 0
	ann_schedule_entry 960, 1
.LAnnOtherWeatherEntries:
	ann_schedule_entry 0, 1
	ann_schedule_entry 240, 2
	ann_schedule_entry 420, 0
	ann_schedule_entry 600, 2
	ann_schedule_entry 780, 0
	ann_schedule_entry 960, 1
.LAnnAfterRivalEventsClearEntries:
	ann_schedule_entry 0, 1
	ann_schedule_entry 60, 3
	ann_schedule_entry 95, 4
	ann_schedule_entry 107, 5
	ann_schedule_entry 240, 6
	ann_schedule_entry 253, 7
	ann_schedule_entry 287, 2
	ann_schedule_entry 420, 0
	ann_schedule_entry 600, 2
	ann_schedule_entry 780, 0
	ann_schedule_entry 960, 1
.LAnnAfterRivalEventsOtherWeatherEntries:
	ann_schedule_entry 0, 1
	ann_schedule_entry 240, 2
	ann_schedule_entry 420, 0
	ann_schedule_entry 600, 2
	ann_schedule_entry 780, 0
	ann_schedule_entry 960, 1
.LAnnAutumn5Entries:
	ann_schedule_entry 0, 1
	ann_schedule_entry 240, 0
	ann_schedule_entry 660, 1
.LAnnFestivalEntries:
	ann_schedule_entry 0, 1
.LAnnWinter14Entries:
	ann_schedule_entry 0, 1
	ann_schedule_entry 120, 8
	ann_schedule_entry 130, 9
	ann_schedule_entry 180, 13
	ann_schedule_entry 190, 1
.LAnnMarriedActiveNpcEntries:
	ann_schedule_entry 0, 12
	ann_schedule_entry 300, 10
	ann_schedule_entry 720, 12
	ann_schedule_entry 840, 11
.LAnnMarriedActiveNpcVariantEntries:
	ann_schedule_entry 0, 12
	ann_schedule_entry 180, 0
	ann_schedule_entry 720, 12
	ann_schedule_entry 840, 11
.LAnnMarriedEntries:
	ann_schedule_entry 0, 12
	ann_schedule_entry 300, 10
	ann_schedule_entry 720, 12
	ann_schedule_entry 840, 11
.LAnnMarriedVariantEntries:
	ann_schedule_entry 0, 12
	ann_schedule_entry 180, 0
	ann_schedule_entry 720, 12
	ann_schedule_entry 840, 11
.LAnnMarriedActiveNpcFestivalOrStormEntries:
	ann_schedule_entry 0, 12
	ann_schedule_entry 300, 10
	ann_schedule_entry 720, 12
	ann_schedule_entry 840, 11
.LAnnMarriedActiveNpcVariantFestivalOrStormEntries:
	ann_schedule_entry 0, 12
	ann_schedule_entry 300, 10
	ann_schedule_entry 720, 12
	ann_schedule_entry 840, 11
.LAnnMarriedFestivalOrStormEntries:
	ann_schedule_entry 0, 12
	ann_schedule_entry 300, 10
	ann_schedule_entry 720, 12
	ann_schedule_entry 840, 11
.LAnnMarriedVariantFestivalOrStormEntries:
	ann_schedule_entry 0, 12
	ann_schedule_entry 300, 10
	ann_schedule_entry 720, 12
	ann_schedule_entry 840, 11
.LAnnMarriedAutumn9Entries:
	ann_schedule_entry 0, 12
	ann_schedule_entry 240, 14
	ann_schedule_entry 720, 12
	ann_schedule_entry 840, 11
.LAnnMarriedAutumn13Entries:
	ann_schedule_entry 0, 12
	ann_schedule_entry 300, 10
	ann_schedule_entry 720, 14
.LAnnMarriedEventOverrideEntries:
	ann_schedule_entry 0, 11

.LAnnPaths:
	.incbin "baserom.gba", 0xF475C, 0x218

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
