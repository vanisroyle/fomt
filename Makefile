.SUFFIXES:

# ==================
# = PROJECT CONFIG =
# ==================

BUILD_NAME := fomt

INCLUDE_DIRS := \
  tools/agbcc/include \
  tools/libagbc++ \
  tools/libsix/include

SRC_DIR = src
ASM_DIR = asm
DATA_ASM_DIR = asm/data
BUILD_DIR = build
PYTHON ?= python3
SCRIPTS_OVERFLOW_FROM ?= 1329
VIETNAMESE_FONT ?= 0

# ====================
# = TOOL DEFINITIONS =
# ====================

TOOLCHAIN ?= $(DEVKITARM)

ifneq (,$(TOOLCHAIN))
  export PATH := $(TOOLCHAIN)/bin:$(PATH)
endif

PREFIX := arm-none-eabi-

export OBJCOPY := $(PREFIX)objcopy
export AS := $(PREFIX)as
export CPP := $(PREFIX)cpp
export LD := $(PREFIX)ld
export STRIP := $(PREFIX)strip

ifeq ($(OS),Windows_NT)
  EXE := .exe
else
  EXE :=
endif

CC1      := tools/agbcc/bin/agbcc$(EXE)
CC1PLUS  := tools/agbcc/bin/agbcp$(EXE)

OLD_CC1  := tools/agbcc/bin/old_agbcc$(EXE)

# ================
# = BUILD CONFIG =
# ================

INCFLAGS     := $(foreach dir, $(INCLUDE_DIRS), -I "$(dir)")

CPPFLAGS := $(INCFLAGS) -iquote . -iquote include -Wno-trigraphs -fno-exceptions
CFLAGS   := -g -mthumb-interwork -Wimplicit -Wparentheses -Werror -O2 -fhex-asm
CXXFLAGS := -quiet -fno-exceptions -fno-rtti -fvtable-thunks $(CFLAGS)
ASFLAGS  := $(INCFLAGS) -I . -I include -mcpu=arm7tdmi

ROM := $(BUILD_NAME).gba
ELF := $(ROM:.gba=.elf)
MAP := $(ROM:.gba=.map)
LDS := $(BUILD_NAME).lds

C_SRCS := $(wildcard $(SRC_DIR)/*.c $(SRC_DIR)/rt/*.c)
C_OBJS := $(C_SRCS:%.c=$(BUILD_DIR)/%.o)

CXX_SRCS := $(wildcard $(SRC_DIR)/*.cc $(SRC_DIR)/rt/*.cc)
CXX_OBJS := $(CXX_SRCS:%.cc=$(BUILD_DIR)/%.o)

ASM_SRCS := $(wildcard $(SRC_DIR)/*.s $(ASM_DIR)/*.s)
ASM_OBJS := $(ASM_SRCS:%.s=$(BUILD_DIR)/%.o)

DATA_ASM_SRCS := $(wildcard $(DATA_ASM_DIR)/*.s)
DATA_ASM_OBJS := $(DATA_ASM_SRCS:%.s=$(BUILD_DIR)/%.o)

ALL_OBJS := $(C_OBJS) $(CXX_OBJS) $(ASM_OBJS) $(DATA_ASM_OBJS)
ALL_DEPS := $(ALL_OBJS:%.o=%.d)

SUBDIRS := $(sort $(dir $(ALL_OBJS)))
$(shell mkdir -p $(SUBDIRS))

# ===========
# = RECIPES =
# ===========

compare: $(ROM)
	sha1sum -c $(BUILD_NAME).sha1

.PHONY: compare

# The generator checks all editable inputs and only replaces changed outputs.
$(BUILD_DIR)/assets/scripts.s: FORCE
	@$(PYTHON) tools/assets/fomt_assets.py build --overflow-from $(SCRIPTS_OVERFLOW_FROM)

$(BUILD_DIR)/asm/data/data_080F1A80.o $(BUILD_DIR)/asm/data/data_0813B288.o: $(BUILD_DIR)/assets/scripts.s

FONT_BINS := $(addprefix $(BUILD_DIR)/assets/font-,small.bin large.bin single.bin double.bin)

$(BUILD_DIR)/assets/fonts.stamp: FORCE
	@$(PYTHON) tools/assets/font_assets.py build $(if $(filter 1,$(VIETNAMESE_FONT)),--halfwidth,)

$(BUILD_DIR)/assets/vietnamese-mode.inc: $(BUILD_DIR)/assets/fonts.stamp
	@test -f $@

$(BUILD_DIR)/asm/code_080D0C68.o $(BUILD_DIR)/asm/vietnamese_font.o: $(BUILD_DIR)/assets/fonts.stamp $(BUILD_DIR)/assets/vietnamese-mode.inc

$(FONT_BINS): $(BUILD_DIR)/assets/fonts.stamp
	@test -f $@

$(BUILD_DIR)/asm/data/data_0813B288.o: $(BUILD_DIR)/assets/fonts.stamp $(FONT_BINS)

$(BUILD_DIR)/assets/graphics.stamp: FORCE
	@$(PYTHON) tools/assets/graphics_assets.py build

GRAPHICS_PRODUCTS := $(BUILD_DIR)/assets/graphics.s $(foreach part,tiles palette map,$(BUILD_DIR)/assets/graphics-$(part)-slot.bin $(BUILD_DIR)/assets/graphics-$(part)-overflow.bin)
GRAPHICS_PRODUCTS += $(foreach part,title-resource title-background title-palette title-map-0 title-map-1 title-map-2 title-map-3,$(BUILD_DIR)/assets/graphics-$(part).s $(BUILD_DIR)/assets/graphics-$(part)-slot.bin $(BUILD_DIR)/assets/graphics-$(part)-overflow.bin)

$(GRAPHICS_PRODUCTS): $(BUILD_DIR)/assets/graphics.stamp
	@test -f $@

$(BUILD_DIR)/asm/data/data_0813B288.o: $(BUILD_DIR)/assets/graphics.stamp $(GRAPHICS_PRODUCTS)

$(BUILD_DIR)/assets/audio.stamp: FORCE
	@$(PYTHON) tools/assets/audio_assets.py build $(if $(AUDIO_OVERFLOW_SONG),--overflow-song $(AUDIO_OVERFLOW_SONG),)

AUDIO_ASM := $(addprefix $(BUILD_DIR)/assets/audio-,bank.s voices.s table.s)

$(AUDIO_ASM): $(BUILD_DIR)/assets/audio.stamp
	@test -f $@

$(BUILD_DIR)/src/sound_data.o $(BUILD_DIR)/asm/data/data_080F9EB8.o $(BUILD_DIR)/asm/data/data_0813B288.o: $(BUILD_DIR)/assets/audio.stamp $(AUDIO_ASM)

$(BUILD_DIR)/assets/text-tables.stamp: FORCE $(BUILD_DIR)/assets/fonts.stamp $(BUILD_DIR)/assets/scripts.s
	@$(PYTHON) tools/assets/text_tables.py build

$(BUILD_DIR)/src/item.o $(BUILD_DIR)/asm/data/data_080E8610.o $(BUILD_DIR)/asm/data/data_080F0348.o $(BUILD_DIR)/asm/data/data_080F9EB8.o: $(BUILD_DIR)/assets/text-tables.stamp
$(BUILD_DIR)/src/item.o: ASFLAGS += -L
$(BUILD_DIR)/src/item.o: Makefile tools/assets/text_tables.py

ifneq (,$(and $(filter extract,$(MAKECMDGOALS)),$(filter-out extract clean assets-test,$(MAKECMDGOALS))))
$(error run `make extract` on its own, then build in a separate make invocation)
endif

extract:
	@$(PYTHON) tools/assets/fomt_assets.py extract $(if $(filter 1,$(FORCE)),--force,)
	@$(PYTHON) tools/assets/font_assets.py extract $(if $(filter 1,$(FORCE)),--force,)
	@$(PYTHON) tools/assets/graphics_assets.py extract $(if $(filter 1,$(FORCE)),--force,)
	@$(PYTHON) tools/assets/audio_assets.py extract $(if $(filter 1,$(FORCE)),--force,)
	@$(PYTHON) tools/assets/text_tables.py extract $(if $(filter 1,$(FORCE)),--force,)

assets-test:
	@$(PYTHON) -m unittest discover -s tools/assets -p 'test_*.py'

FORCE:

.PHONY: extract assets-test FORCE

# ROM from ELF
%.gba: %.elf
	$(OBJCOPY) -O binary $< $@
	@$(PYTHON) tools/assets/text_tables.py verify --built "$@" --elf "$<" || { rm -f "$@"; exit 1; }

# ELF
$(ELF): $(ALL_OBJS) $(LDS)
	@echo "LD $(LDS) $(ALL_OBJS:$(BUILD_DIR)/%=%)"
	@cd $(BUILD_DIR) && $(LD) -T ../$(LDS) -Map ../$(MAP) -L../tools/agbcc/lib -lgcc -lc $(ALL_OBJS:$(BUILD_DIR)/%=%) -o ../$@
	@$(STRIP) -N .gcc2_compiled. $(ELF)

# C dependency file
$(BUILD_DIR)/%.d: %.c
	@$(CPP) $(CPPFLAGS) $< -o $@ -MM -MG -MT $@ -MT $(BUILD_DIR)/$*.o

# C object
$(BUILD_DIR)/%.o: %.c $(BUILD_DIR)/%.d
	@echo "CC $<"
	@$(CPP) $(CPPFLAGS) $< | $(CC1) $(CFLAGS) -o $(BUILD_DIR)/$*.s
	@tools/scripts/align_sections.sh $(BUILD_DIR)/$*.s
	@$(AS) $(ASFLAGS) $(BUILD_DIR)/$*.s -o $@ 

# C++ dependency file
$(BUILD_DIR)/%.d: %.cc
	@$(CPP) $(CPPFLAGS) $< -o $@ -MM -MG -MT $@ -MT $(BUILD_DIR)/$*.o

# C++ object
$(BUILD_DIR)/%.o: %.cc $(BUILD_DIR)/%.d
	@echo "CP $<"
	@$(CPP) $(CPPFLAGS) $< | ($(CC1PLUS) $(CXXFLAGS) -o $(BUILD_DIR)/$*.s || false)
	@tools/scripts/align_sections.sh $(BUILD_DIR)/$*.s
	@$(AS) $(ASFLAGS) $(BUILD_DIR)/$*.s -o $@
	@if test "$<" = "src/item.cc"; then \
	  $(PYTHON) tools/assets/text_tables.py patch-item --assembly $(BUILD_DIR)/$*.s --object $@ && \
	  $(AS) $(ASFLAGS) $(BUILD_DIR)/$*.s -o $@; \
	fi

# ASM dependency file (dummy, generated with the object)
$(BUILD_DIR)/%.d: $(BUILD_DIR)/%.o
	@touch $@

# ASM object
$(BUILD_DIR)/%.o: %.s
	@echo "AS $<"
	@$(AS) $(ASFLAGS) $< -o $@ --MD $(BUILD_DIR)/$*.d

# overrides for matching
$(BUILD_DIR)/src/m4a.o: CC1 := $(OLD_CC1)
$(BUILD_DIR)/src/fixed_sprite_overlay_draw.o: CXXFLAGS += -mlong-calls
$(BUILD_DIR)/src/map_overlay_draw.o: CXXFLAGS += -mlong-calls
$(BUILD_DIR)/src/npc_side_batch12.o: CXXFLAGS += -mlong-calls

clean:
	@echo "RM $(ROM) $(ELF) $(MAP) $(BUILD_DIR)"
	@rm -f $(ROM) $(ELF) $(MAP) 
	@rm -r $(BUILD_DIR)/

.PHONY: clean

ifeq (,$(and $(MAKECMDGOALS),$(if $(filter-out clean extract assets-test,$(MAKECMDGOALS)),,1)))
-include $(ALL_DEPS)
.PRECIOUS: $(BUILD_DIR)/%.d
endif
