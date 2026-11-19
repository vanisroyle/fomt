    .section .rodata

	@ schedules

	@ .incbin "baserom.gba", 0xF1AC0, 0x500

	.incbin "baserom.gba", 0xF1FB4, 12 @ bad_alloc

	.global gPopuriScheduleInfo
gPopuriScheduleInfo:
	.4byte SelectPopuriSchedule
	.4byte 24, .LPopuriSchedules
.LPopuriSchedules: @ In PopuriSchedule order.
	.4byte 0
	.4byte .LPopuriClearWeatherSchedule
	.4byte .LPopuriOtherWeatherSchedule
	.4byte .LPopuriSundayClearSchedule
	.4byte .LPopuriSundayOtherWeatherSchedule
	.4byte .LPopuriSummerSundayClearSchedule
	.4byte .LPopuriMarriedActiveNpcSchedule
	.4byte .LPopuriMarriedActiveNpcVariantSchedule
	.4byte .LPopuriMarriedActiveNpcVariantSundaySchedule
	.4byte .LPopuriMarriedSchedule
	.4byte .LPopuriMarriedVariantSchedule
	.4byte .LPopuriMarriedVariantSundaySchedule
	.4byte .LPopuriMarriedActiveNpcFestivalOrStormSchedule
	.4byte .LPopuriMarriedActiveNpcVariantFestivalOrStormSchedule
	.4byte .LPopuriMarriedFestivalOrStormSchedule
	.4byte .LPopuriMarriedVariantFestivalOrStormSchedule
	.4byte .LPopuriFestivalSchedule
	.4byte .LPopuriAutumn30Schedule
	.4byte .LPopuriWinter14Schedule
	.4byte .LPopuriAfterRivalEventsSummerFestivalSchedule
	.4byte .LPopuriAfterRivalEventsSummerClearSchedule
	.4byte .LPopuriAfterRivalEventsSummerOtherWeatherSchedule
	.4byte .LPopuriMarriedAutumn13Schedule
	.4byte .LPopuriMarriedEventOverrideSchedule

.LPopuriClearWeatherSchedule:
	.2byte 8, 0
	.4byte .LPopuriClearWeatherEntries
.LPopuriOtherWeatherSchedule:
	.2byte 2, 0
	.4byte .LPopuriOtherWeatherEntries
.LPopuriSundayClearSchedule:
	.2byte 11, 0
	.4byte .LPopuriSundayClearEntries
.LPopuriSundayOtherWeatherSchedule:
	.2byte 8, 0
	.4byte .LPopuriSundayOtherWeatherEntries
.LPopuriSummerSundayClearSchedule:
	.2byte 8, 0
	.4byte .LPopuriSummerSundayClearEntries
.LPopuriMarriedActiveNpcSchedule:
	.2byte 4, 0
	.4byte .LPopuriMarriedActiveNpcEntries
.LPopuriMarriedActiveNpcVariantSchedule:
	.2byte 4, 0
	.4byte .LPopuriMarriedActiveNpcVariantEntries
.LPopuriMarriedActiveNpcVariantSundaySchedule:
	.2byte 4, 0
	.4byte .LPopuriMarriedActiveNpcVariantSundayEntries
.LPopuriMarriedSchedule:
	.2byte 4, 0
	.4byte .LPopuriMarriedEntries
.LPopuriMarriedVariantSchedule:
	.2byte 4, 0
	.4byte .LPopuriMarriedVariantEntries
.LPopuriMarriedVariantSundaySchedule:
	.2byte 4, 0
	.4byte .LPopuriMarriedVariantSundayEntries
.LPopuriMarriedActiveNpcFestivalOrStormSchedule:
	.2byte 4, 0
	.4byte .LPopuriMarriedActiveNpcFestivalOrStormEntries
.LPopuriMarriedActiveNpcVariantFestivalOrStormSchedule:
	.2byte 4, 0
	.4byte .LPopuriMarriedActiveNpcVariantFestivalOrStormEntries
.LPopuriMarriedFestivalOrStormSchedule:
	.2byte 4, 0
	.4byte .LPopuriMarriedFestivalOrStormEntries
.LPopuriMarriedVariantFestivalOrStormSchedule:
	.2byte 4, 0
	.4byte .LPopuriMarriedVariantFestivalOrStormEntries
.LPopuriFestivalSchedule:
	.2byte 1, 0
	.4byte .LPopuriFestivalEntries
.LPopuriAutumn30Schedule:
	.2byte 5, 0
	.4byte .LPopuriAutumn30Entries
.LPopuriWinter14Schedule:
	.2byte 4, 0
	.4byte .LPopuriWinter14Entries
.LPopuriAfterRivalEventsSummerFestivalSchedule:
	.2byte 1, 0
	.4byte .LPopuriAfterRivalEventsSummerFestivalEntries
.LPopuriAfterRivalEventsSummerClearSchedule:
	.2byte 8, 0
	.4byte .LPopuriAfterRivalEventsSummerClearEntries
.LPopuriAfterRivalEventsSummerOtherWeatherSchedule:
	.2byte 2, 0
	.4byte .LPopuriAfterRivalEventsSummerOtherWeatherEntries
.LPopuriMarriedAutumn13Schedule:
	.2byte 3, 0
	.4byte .LPopuriMarriedAutumn13Entries
.LPopuriMarriedEventOverrideSchedule:
	.2byte 1, 0
	.4byte .LPopuriMarriedEventOverrideEntries

	@ Time is minutes since 06:00; route indices refer to the original path records.
	.macro popuri_schedule_entry time, path
	.2byte \time, 0
	.4byte .LPopuriPaths + (\path * 12)
	.endm

.LPopuriClearWeatherEntries:
	popuri_schedule_entry 0, 0
	popuri_schedule_entry 60, 1
	popuri_schedule_entry 98, 2
	popuri_schedule_entry 113, 3
	popuri_schedule_entry 240, 4
	popuri_schedule_entry 256, 5
	popuri_schedule_entry 293, 0
	popuri_schedule_entry 780, 6
.LPopuriOtherWeatherEntries:
	popuri_schedule_entry 0, 0
	popuri_schedule_entry 780, 6
.LPopuriSundayClearEntries:
	popuri_schedule_entry 0, 0
	popuri_schedule_entry 180, 7
	popuri_schedule_entry 227, 8
	popuri_schedule_entry 251, 9
	popuri_schedule_entry 421, 10
	popuri_schedule_entry 446, 11
	popuri_schedule_entry 462, 12
	popuri_schedule_entry 600, 13
	popuri_schedule_entry 608, 14
	popuri_schedule_entry 655, 0
	popuri_schedule_entry 780, 6
.LPopuriSundayOtherWeatherEntries:
	popuri_schedule_entry 0, 0
	popuri_schedule_entry 180, 7
	popuri_schedule_entry 227, 8
	popuri_schedule_entry 251, 9
	popuri_schedule_entry 600, 10
	popuri_schedule_entry 625, 14
	popuri_schedule_entry 672, 0
	popuri_schedule_entry 780, 6
.LPopuriSummerSundayClearEntries:
	popuri_schedule_entry 0, 0
	popuri_schedule_entry 420, 7
	popuri_schedule_entry 467, 15
	popuri_schedule_entry 486, 16
	popuri_schedule_entry 600, 17
	popuri_schedule_entry 620, 14
	popuri_schedule_entry 667, 0
	popuri_schedule_entry 780, 6
.LPopuriMarriedActiveNpcEntries:
	popuri_schedule_entry 0, 18
	popuri_schedule_entry 300, 19
	popuri_schedule_entry 720, 18
	popuri_schedule_entry 840, 20
.LPopuriMarriedActiveNpcVariantEntries:
	popuri_schedule_entry 0, 18
	popuri_schedule_entry 180, 0
	popuri_schedule_entry 720, 18
	popuri_schedule_entry 840, 20
.LPopuriMarriedActiveNpcVariantSundayEntries:
	popuri_schedule_entry 0, 18
	popuri_schedule_entry 300, 19
	popuri_schedule_entry 720, 18
	popuri_schedule_entry 840, 20
.LPopuriMarriedEntries:
	popuri_schedule_entry 0, 18
	popuri_schedule_entry 300, 19
	popuri_schedule_entry 720, 18
	popuri_schedule_entry 840, 20
.LPopuriMarriedVariantEntries:
	popuri_schedule_entry 0, 18
	popuri_schedule_entry 180, 0
	popuri_schedule_entry 720, 18
	popuri_schedule_entry 840, 20
.LPopuriMarriedVariantSundayEntries:
	popuri_schedule_entry 0, 18
	popuri_schedule_entry 300, 19
	popuri_schedule_entry 720, 18
	popuri_schedule_entry 840, 20
.LPopuriMarriedActiveNpcFestivalOrStormEntries:
	popuri_schedule_entry 0, 18
	popuri_schedule_entry 300, 19
	popuri_schedule_entry 720, 18
	popuri_schedule_entry 840, 20
.LPopuriMarriedActiveNpcVariantFestivalOrStormEntries:
	popuri_schedule_entry 0, 18
	popuri_schedule_entry 300, 19
	popuri_schedule_entry 720, 18
	popuri_schedule_entry 840, 20
.LPopuriMarriedFestivalOrStormEntries:
	popuri_schedule_entry 0, 18
	popuri_schedule_entry 300, 19
	popuri_schedule_entry 720, 18
	popuri_schedule_entry 840, 20
.LPopuriMarriedVariantFestivalOrStormEntries:
	popuri_schedule_entry 0, 18
	popuri_schedule_entry 300, 19
	popuri_schedule_entry 720, 18
	popuri_schedule_entry 840, 20
.LPopuriFestivalEntries:
	popuri_schedule_entry 0, 0
.LPopuriAutumn30Entries:
	popuri_schedule_entry 0, 0
	popuri_schedule_entry 240, 23
	popuri_schedule_entry 250, 21
	popuri_schedule_entry 300, 22
	popuri_schedule_entry 310, 0
.LPopuriWinter14Entries:
	popuri_schedule_entry 0, 23
	popuri_schedule_entry 10, 21
	popuri_schedule_entry 60, 22
	popuri_schedule_entry 70, 0
.LPopuriAfterRivalEventsSummerFestivalEntries:
	popuri_schedule_entry 0, 0
.LPopuriAfterRivalEventsSummerClearEntries:
	popuri_schedule_entry 0, 0
	popuri_schedule_entry 120, 7
	popuri_schedule_entry 167, 15
	popuri_schedule_entry 186, 16
	popuri_schedule_entry 240, 17
	popuri_schedule_entry 260, 14
	popuri_schedule_entry 307, 0
	popuri_schedule_entry 780, 6
.LPopuriAfterRivalEventsSummerOtherWeatherEntries:
	popuri_schedule_entry 0, 0
	popuri_schedule_entry 780, 6
.LPopuriMarriedAutumn13Entries:
	popuri_schedule_entry 0, 18
	popuri_schedule_entry 300, 19
	popuri_schedule_entry 720, 24
.LPopuriMarriedEventOverrideEntries:
	popuri_schedule_entry 0, 20

.LPopuriPaths:
	.incbin "baserom.gba", 0xF2414, 0x3F8

	.global gUnk_080F280C
gUnk_080F280C:
	.incbin "baserom.gba", 0xF280C, 0x1B4

	.global gChildScheduleInfo
gChildScheduleInfo:
	.4byte SelectChildSchedule
	.4byte 6, .LChildSchedules
.LChildSchedules: @ In ChildSchedule order.
	.4byte 0
	.4byte .LChildRegularSchedule
	.4byte .LChildLateMorningSchedule
	.4byte .LChildHomeAllDaySchedule
	.4byte .LChildWalkingAgeSchedule
	.4byte .LChildAutumn13Schedule

.LChildRegularSchedule:
	.2byte 4, 0
	.4byte .LChildRegularEntries
.LChildLateMorningSchedule:
	.2byte 4, 0
	.4byte .LChildLateMorningEntries
.LChildHomeAllDaySchedule:
	.2byte 1, 0
	.4byte .LChildHomeAllDayEntries
.LChildWalkingAgeSchedule:
	.2byte 3, 0
	.4byte .LChildWalkingAgeEntries
.LChildAutumn13Schedule:
	.2byte 2, 0
	.4byte .LChildAutumn13Entries

	@ Time is minutes since 06:00; route indices refer to the original path records.
	.macro child_schedule_entry time, path
	.2byte \time, 0
	.4byte .LChildPaths + (\path * 12)
	.endm

.LChildRegularEntries:
	child_schedule_entry 0, 0
	child_schedule_entry 180, 1
	child_schedule_entry 720, 0
	child_schedule_entry 840, 1
.LChildLateMorningEntries:
	child_schedule_entry 0, 0
	child_schedule_entry 300, 1
	child_schedule_entry 720, 0
	child_schedule_entry 840, 1
.LChildHomeAllDayEntries:
	child_schedule_entry 0, 1
.LChildWalkingAgeEntries:
	child_schedule_entry 0, 2
	child_schedule_entry 300, 3
	child_schedule_entry 720, 1
.LChildAutumn13Entries:
	child_schedule_entry 0, 0
	child_schedule_entry 180, 1

.LChildPaths:
	.incbin "baserom.gba", 0xF2A7C, 0x7C

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

	.global gMaryScheduleInfo
gMaryScheduleInfo:
	.4byte SelectMarySchedule
	.4byte 19, .LMarySchedules
.LMarySchedules: @ In MarySchedule order.
	.4byte 0
	.4byte .LMaryRegularSchedule
	.4byte .LMaryMondayClearSchedule
	.4byte .LMaryMondayOtherWeatherSchedule
	.4byte .LMaryAfterRivalEventsSchedule
	.4byte .LMaryAfterRivalEventsThursdayClearSchedule
	.4byte .LMaryAfterRivalEventsThursdayOtherWeatherSchedule
	.4byte .LMaryWinter14Schedule
	.4byte .LMaryFestivalSchedule
	.4byte .LMaryAfterRivalEventsFestivalSchedule
	.4byte .LMaryMarriedActiveNpcSchedule
	.4byte .LMaryMarriedActiveNpcMondaySchedule
	.4byte .LMaryMarriedSchedule
	.4byte .LMaryMarriedMondaySchedule
	.4byte .LMaryMarriedActiveNpcFestivalOrStormSchedule
	.4byte .LMaryMarriedFestivalOrStormSchedule
	.4byte .LMaryMarriedAutumn9Schedule
	.4byte .LMaryMarriedAutumn13Schedule
	.4byte .LMaryMarriedEventOverrideSchedule

.LMaryRegularSchedule:
	.2byte 7, 0
	.4byte .LMaryRegularEntries
.LMaryMondayClearSchedule:
	.2byte 12, 0
	.4byte .LMaryMondayClearEntries
.LMaryMondayOtherWeatherSchedule:
	.2byte 7, 0
	.4byte .LMaryMondayOtherWeatherEntries
.LMaryAfterRivalEventsSchedule:
	.2byte 7, 0
	.4byte .LMaryAfterRivalEventsEntries
.LMaryAfterRivalEventsThursdayClearSchedule:
	.2byte 9, 0
	.4byte .LMaryAfterRivalEventsThursdayClearEntries
.LMaryAfterRivalEventsThursdayOtherWeatherSchedule:
	.2byte 9, 0
	.4byte .LMaryAfterRivalEventsThursdayOtherWeatherEntries
.LMaryWinter14Schedule:
	.2byte 5, 0
	.4byte .LMaryWinter14Entries
.LMaryFestivalSchedule:
	.2byte 1, 0
	.4byte .LMaryFestivalEntries
.LMaryAfterRivalEventsFestivalSchedule:
	.2byte 1, 0
	.4byte .LMaryAfterRivalEventsFestivalEntries
.LMaryMarriedActiveNpcSchedule:
	.2byte 4, 0
	.4byte .LMaryMarriedActiveNpcEntries
.LMaryMarriedActiveNpcMondaySchedule:
	.2byte 4, 0
	.4byte .LMaryMarriedActiveNpcMondayEntries
.LMaryMarriedSchedule:
	.2byte 4, 0
	.4byte .LMaryMarriedEntries
.LMaryMarriedMondaySchedule:
	.2byte 4, 0
	.4byte .LMaryMarriedMondayEntries
.LMaryMarriedActiveNpcFestivalOrStormSchedule:
	.2byte 4, 0
	.4byte .LMaryMarriedActiveNpcFestivalOrStormEntries
.LMaryMarriedFestivalOrStormSchedule:
	.2byte 4, 0
	.4byte .LMaryMarriedFestivalOrStormEntries
.LMaryMarriedAutumn9Schedule:
	.2byte 4, 0
	.4byte .LMaryMarriedAutumn9Entries
.LMaryMarriedAutumn13Schedule:
	.2byte 3, 0
	.4byte .LMaryMarriedAutumn13Entries
.LMaryMarriedEventOverrideSchedule:
	.2byte 1, 0
	.4byte .LMaryMarriedEventOverrideEntries

	@ Time is minutes since 06:00; route indices refer to the original path records.
	.macro mary_schedule_entry time, path
	.2byte \time, 0
	.4byte .LMaryPaths + (\path * 12)
	.endm

.LMaryRegularEntries:
	mary_schedule_entry 0, 0
	mary_schedule_entry 60, 1
	mary_schedule_entry 240, 2
	mary_schedule_entry 247, 3
	mary_schedule_entry 720, 4
	mary_schedule_entry 727, 1
	mary_schedule_entry 960, 0
.LMaryMondayClearEntries:
	mary_schedule_entry 0, 0
	mary_schedule_entry 63, 5
	mary_schedule_entry 90, 6
	mary_schedule_entry 148, 7
	mary_schedule_entry 240, 8
	mary_schedule_entry 298, 9
	mary_schedule_entry 324, 1
	mary_schedule_entry 420, 10
	mary_schedule_entry 444, 11
	mary_schedule_entry 603, 12
	mary_schedule_entry 627, 1
	mary_schedule_entry 960, 0
.LMaryMondayOtherWeatherEntries:
	mary_schedule_entry 0, 0
	mary_schedule_entry 60, 1
	mary_schedule_entry 420, 10
	mary_schedule_entry 444, 11
	mary_schedule_entry 603, 12
	mary_schedule_entry 627, 1
	mary_schedule_entry 960, 0
.LMaryAfterRivalEventsEntries:
	mary_schedule_entry 0, 0
	mary_schedule_entry 60, 1
	mary_schedule_entry 240, 2
	mary_schedule_entry 247, 3
	mary_schedule_entry 720, 4
	mary_schedule_entry 727, 1
	mary_schedule_entry 960, 0
.LMaryAfterRivalEventsThursdayClearEntries:
	mary_schedule_entry 0, 0
	mary_schedule_entry 60, 1
	mary_schedule_entry 426, 5
	mary_schedule_entry 453, 23
	mary_schedule_entry 462, 24
	mary_schedule_entry 600, 25
	mary_schedule_entry 610, 9
	mary_schedule_entry 636, 1
	mary_schedule_entry 960, 0
.LMaryAfterRivalEventsThursdayOtherWeatherEntries:
	mary_schedule_entry 0, 0
	mary_schedule_entry 60, 1
	mary_schedule_entry 420, 5
	mary_schedule_entry 447, 19
	mary_schedule_entry 463, 20
	mary_schedule_entry 600, 21
	mary_schedule_entry 616, 9
	mary_schedule_entry 642, 1
	mary_schedule_entry 960, 0
.LMaryWinter14Entries:
	mary_schedule_entry 0, 1
	mary_schedule_entry 480, 16
	mary_schedule_entry 490, 17
	mary_schedule_entry 540, 18
	mary_schedule_entry 550, 1
.LMaryFestivalEntries:
	mary_schedule_entry 0, 1
.LMaryAfterRivalEventsFestivalEntries:
	mary_schedule_entry 0, 1
.LMaryMarriedActiveNpcEntries:
	mary_schedule_entry 0, 15
	mary_schedule_entry 180, 3
	mary_schedule_entry 720, 15
	mary_schedule_entry 840, 14
.LMaryMarriedActiveNpcMondayEntries:
	mary_schedule_entry 0, 15
	mary_schedule_entry 300, 13
	mary_schedule_entry 720, 15
	mary_schedule_entry 840, 14
.LMaryMarriedEntries:
	mary_schedule_entry 0, 15
	mary_schedule_entry 180, 3
	mary_schedule_entry 720, 15
	mary_schedule_entry 840, 14
.LMaryMarriedMondayEntries:
	mary_schedule_entry 0, 15
	mary_schedule_entry 300, 13
	mary_schedule_entry 720, 15
	mary_schedule_entry 840, 14
.LMaryMarriedActiveNpcFestivalOrStormEntries:
	mary_schedule_entry 0, 15
	mary_schedule_entry 300, 13
	mary_schedule_entry 720, 15
	mary_schedule_entry 840, 14
.LMaryMarriedFestivalOrStormEntries:
	mary_schedule_entry 0, 15
	mary_schedule_entry 300, 13
	mary_schedule_entry 720, 15
	mary_schedule_entry 840, 14
.LMaryMarriedAutumn9Entries:
	mary_schedule_entry 0, 15
	mary_schedule_entry 240, 22
	mary_schedule_entry 720, 15
	mary_schedule_entry 840, 14
.LMaryMarriedAutumn13Entries:
	mary_schedule_entry 0, 15
	mary_schedule_entry 300, 13
	mary_schedule_entry 720, 22
.LMaryMarriedEventOverrideEntries:
	mary_schedule_entry 0, 14

.LMaryPaths:
	.incbin "baserom.gba", 0xF512C, 0x414

	.global gUnk_080F5540
gUnk_080F5540:
	.incbin "baserom.gba", 0xF5540, 0x43C

	.global gUnk_080F597C
gUnk_080F597C:
	.incbin "baserom.gba", 0xF597C, 0x50

	.global gUnk_080F59CC
gUnk_080F59CC:
	.incbin "baserom.gba", 0xF59CC, 0x3C8

	.global gElliScheduleInfo
gElliScheduleInfo:
	.4byte SelectElliSchedule
	.4byte 17, .LElliSchedules
.LElliSchedules: @ In ElliSchedule order.
	.4byte 0
	.4byte .LElliRegularSchedule
	.4byte .LElliWednesdaySchedule
	.4byte .LElliAfterRivalEventsSchedule
	.4byte .LElliAfterRivalEventsWednesdaySchedule
	.4byte .LElliFestivalSchedule
	.4byte .LElliWinter14Schedule
	.4byte .LElliAfterRivalEventsFestivalSchedule
	.4byte .LElliMarriedActiveNpcSchedule
	.4byte .LElliMarriedActiveNpcWednesdaySchedule
	.4byte .LElliMarriedSchedule
	.4byte .LElliMarriedWednesdaySchedule
	.4byte .LElliMarriedActiveNpcFestivalOrStormSchedule
	.4byte .LElliMarriedFestivalOrStormSchedule
	.4byte .LElliMarriedAutumn9Schedule
	.4byte .LElliMarriedAutumn13Schedule
	.4byte .LElliMarriedEventOverrideSchedule

.LElliRegularSchedule:
	.2byte 3, 0
	.4byte .LElliRegularEntries
.LElliWednesdaySchedule:
	.2byte 9, 0
	.4byte .LElliWednesdayEntries
.LElliAfterRivalEventsSchedule:
	.2byte 3, 0
	.4byte .LElliAfterRivalEventsEntries
.LElliAfterRivalEventsWednesdaySchedule:
	.2byte 9, 0
	.4byte .LElliAfterRivalEventsWednesdayEntries
.LElliFestivalSchedule:
	.2byte 1, 0
	.4byte .LElliFestivalEntries
.LElliWinter14Schedule:
	.2byte 5, 0
	.4byte .LElliWinter14Entries
.LElliAfterRivalEventsFestivalSchedule:
	.2byte 1, 0
	.4byte .LElliAfterRivalEventsFestivalEntries
.LElliMarriedActiveNpcSchedule:
	.2byte 4, 0
	.4byte .LElliMarriedActiveNpcEntries
.LElliMarriedActiveNpcWednesdaySchedule:
	.2byte 4, 0
	.4byte .LElliMarriedActiveNpcWednesdayEntries
.LElliMarriedSchedule:
	.2byte 4, 0
	.4byte .LElliMarriedEntries
.LElliMarriedWednesdaySchedule:
	.2byte 4, 0
	.4byte .LElliMarriedWednesdayEntries
.LElliMarriedActiveNpcFestivalOrStormSchedule:
	.2byte 4, 0
	.4byte .LElliMarriedActiveNpcFestivalOrStormEntries
.LElliMarriedFestivalOrStormSchedule:
	.2byte 4, 0
	.4byte .LElliMarriedFestivalOrStormEntries
.LElliMarriedAutumn9Schedule:
	.2byte 4, 0
	.4byte .LElliMarriedAutumn9Entries
.LElliMarriedAutumn13Schedule:
	.2byte 3, 0
	.4byte .LElliMarriedAutumn13Entries
.LElliMarriedEventOverrideSchedule:
	.2byte 1, 0
	.4byte .LElliMarriedEventOverrideEntries

	@ Time is minutes since 06:00; route indices refer to the original path records.
	.macro elli_schedule_entry time, path
	.2byte \time, 0
	.4byte .LElliPaths + (\path * 12)
	.endm

.LElliRegularEntries:
	elli_schedule_entry 0, 0
	elli_schedule_entry 180, 1
	elli_schedule_entry 780, 0
.LElliWednesdayEntries:
	elli_schedule_entry 0, 0
	elli_schedule_entry 180, 2
	elli_schedule_entry 202, 3
	elli_schedule_entry 420, 4
	elli_schedule_entry 436, 5
	elli_schedule_entry 600, 6
	elli_schedule_entry 616, 3
	elli_schedule_entry 780, 7
	elli_schedule_entry 802, 0
.LElliAfterRivalEventsEntries:
	elli_schedule_entry 0, 0
	elli_schedule_entry 180, 1
	elli_schedule_entry 780, 0
.LElliAfterRivalEventsWednesdayEntries:
	elli_schedule_entry 0, 0
	elli_schedule_entry 180, 2
	elli_schedule_entry 202, 3
	elli_schedule_entry 420, 4
	elli_schedule_entry 436, 5
	elli_schedule_entry 600, 6
	elli_schedule_entry 616, 3
	elli_schedule_entry 780, 7
	elli_schedule_entry 802, 0
.LElliFestivalEntries:
	elli_schedule_entry 0, 0
.LElliWinter14Entries:
	elli_schedule_entry 0, 1
	elli_schedule_entry 240, 8
	elli_schedule_entry 250, 9
	elli_schedule_entry 300, 13
	elli_schedule_entry 310, 1
.LElliAfterRivalEventsFestivalEntries:
	elli_schedule_entry 0, 0
.LElliMarriedActiveNpcEntries:
	elli_schedule_entry 0, 12
	elli_schedule_entry 180, 1
	elli_schedule_entry 720, 12
	elli_schedule_entry 840, 11
.LElliMarriedActiveNpcWednesdayEntries:
	elli_schedule_entry 0, 12
	elli_schedule_entry 300, 10
	elli_schedule_entry 720, 12
	elli_schedule_entry 840, 11
.LElliMarriedEntries:
	elli_schedule_entry 0, 12
	elli_schedule_entry 180, 1
	elli_schedule_entry 720, 12
	elli_schedule_entry 840, 11
.LElliMarriedWednesdayEntries:
	elli_schedule_entry 0, 12
	elli_schedule_entry 300, 10
	elli_schedule_entry 720, 12
	elli_schedule_entry 840, 11
.LElliMarriedActiveNpcFestivalOrStormEntries:
	elli_schedule_entry 0, 12
	elli_schedule_entry 300, 10
	elli_schedule_entry 720, 12
	elli_schedule_entry 840, 11
.LElliMarriedFestivalOrStormEntries:
	elli_schedule_entry 0, 12
	elli_schedule_entry 300, 10
	elli_schedule_entry 720, 12
	elli_schedule_entry 840, 11
.LElliMarriedAutumn9Entries:
	elli_schedule_entry 0, 12
	elli_schedule_entry 240, 14
	elli_schedule_entry 720, 12
	elli_schedule_entry 840, 11
.LElliMarriedAutumn13Entries:
	elli_schedule_entry 0, 12
	elli_schedule_entry 300, 10
	elli_schedule_entry 720, 14
.LElliMarriedEventOverrideEntries:
	elli_schedule_entry 0, 11

.LElliPaths:
	.incbin "baserom.gba", 0xF605C, 0x1A0

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
