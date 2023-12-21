PROJECT_NAME     := blinky_pca10059_mbr
TARGETS          := nrf52840_xxaa
OUTPUT_DIRECTORY := _build
DFU_PACKAGE      := $(OUTPUT_DIRECTORY)/nrf52840_xxaa.dfu
DFU_PORT         ?= COM5
RUN_TESTS        ?= FALSE


SDK_ROOT ?= C:/esl-nsdk
PROJ_DIR := ../../..

$(OUTPUT_DIRECTORY)/nrf52840_xxaa.out: \
  LINKER_SCRIPT  := blinky_gcc_nrf52.ld

# Source files common to all targets
ifeq ($(RUN_TESTS), TRUE)
SRC_FILES += \
    $(PROJ_DIR)/simpletest/simpletest.cpp \
    $(wildcard $(PROJ_DIR)/tests/*.cpp)
else
SRC_FILES += \
    $(PROJ_DIR)/main.cpp
endif
SRC_FILES += \
	$(SDK_ROOT)/integration/nrfx/legacy/nrf_drv_clock.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_clock.c \
	$(SDK_ROOT)/modules/nrfx/drivers/src/nrfx_gpiote.c \
	$(SDK_ROOT)/modules/nrfx/mdk/gcc_startup_nrf52840.s \
	$(SDK_ROOT)/modules/nrfx/mdk/system_nrf52840.c \
	$(SDK_ROOT)/modules/nrfx/drivers//src/nrfx_systick.c \
	$(SDK_ROOT)/components/boards/boards.c \
	$(SDK_ROOT)/components/libraries/atomic_fifo/nrf_atfifo.c \
	$(SDK_ROOT)/components/libraries/timer/app_timer2.c \
	$(SDK_ROOT)/components/libraries/timer/drv_rtc.c \
	$(SDK_ROOT)/components/libraries/sortlist/nrf_sortlist.c \
	$(SDK_ROOT)/components/libraries/util/app_util_platform.c \
	$(SDK_ROOT)/components/libraries/atomic/nrf_atomic.c

# Include folders common to all targets
INC_FOLDERS += \
	$(PROJ_DIR) \
	../config \
	$(SDK_ROOT)/integration/nrfx \
	$(SDK_ROOT)/integration/nrfx/legacy \
	$(SDK_ROOT)/modules/nrfx/drivers/include \
	$(SDK_ROOT)/modules/nrfx/mdk \
	$(SDK_ROOT)/modules/nrfx/hal \
	$(SDK_ROOT)/modules/nrfx \
	$(SDK_ROOT)/components \
	$(SDK_ROOT)/components/boards \
	$(SDK_ROOT)/components/drivers_nrf/nrf_soc_nosd \
	$(SDK_ROOT)/components/softdevice/mbr/headers \
	$(SDK_ROOT)/components/toolchain/cmsis/include \
	$(SDK_ROOT)/components/libraries/util \
	$(SDK_ROOT)/components/libraries/log \
	$(SDK_ROOT)/components/libraries/experimental_section_vars \
	$(SDK_ROOT)/components/libraries/delay \
	$(SDK_ROOT)/components/libraries/log/src \
	$(SDK_ROOT)/components/libraries/timer \
	$(SDK_ROOT)/components/libraries/sortlist \
	$(SDK_ROOT)/components/libraries/atomic_fifo \
	$(SDK_ROOT)/components/libraries/fifo \
	$(SDK_ROOT)/components/libraries/mutex \
	$(SDK_ROOT)/components/libraries/atomic

# Libraries common to all targets
LIB_FILES += \

# Optimization flags
OPT = -O3 -g3
# Uncomment the line below to enable link time optimization
#OPT += -flto

# C flags common to all targets
CFLAGS += $(OPT)
CFLAGS += -DBOARD_PCA10059
CFLAGS += -DBSP_DEFINES_ONLY
CFLAGS += -DCONFIG_GPIO_AS_PINRESET
CFLAGS += -DFLOAT_ABI_HARD
CFLAGS += -DMBR_PRESENT
CFLAGS += -DNRF52840_XXAA
CFLAGS += -mcpu=cortex-m4
CFLAGS += -mthumb -mabi=aapcs
CFLAGS += -Wall -Werror
CFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in a separate section, this allows linker to discard unused ones
CFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CFLAGS += -fno-builtin -fshort-enums
CFLAGS += -DAPP_TIMER_V2
CFLAGS += -DAPP_TIMER_V2_RTC1_ENABLED
CFLAGS += -DNRFX_SYSTICK_ENABLED

# C++ flags common to all targets
CXXFLAGS += $(OPT)
CXXFLAGS += -DBOARD_PCA10059
CXXFLAGS += -DBSP_DEFINES_ONLY
CXXFLAGS += -DCONFIG_GPIO_AS_PINRESET
CXXFLAGS += -DFLOAT_ABI_HARD
CXXFLAGS += -DMBR_PRESENT
CXXFLAGS += -DNRF52840_XXAA
CXXFLAGS += -mcpu=cortex-m4
CXXFLAGS += -mthumb -mabi=aapcs
CXXFLAGS += -Wall -Werror
CXXFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# keep every function in a separate section, this allows linker to discard unused ones
CXXFLAGS += -ffunction-sections -fdata-sections -fno-strict-aliasing
CXXFLAGS += -fno-builtin -fshort-enums
CXXFLAGS += -DAPP_TIMER_V2
CXXFLAGS += -DAPP_TIMER_V2_RTC1_ENABLED

CXXFLAGS += -fno-rtti -fno-exceptions -fno-threadsafe-statics -nostdlib -fno-unwind-tables
CXXFLAGS += -std=c++17

# Assembler flags common to all targets
ASMFLAGS += -g3
ASMFLAGS += -mcpu=cortex-m4
ASMFLAGS += -mthumb -mabi=aapcs
ASMFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
ASMFLAGS += -DBOARD_PCA10059
ASMFLAGS += -DBSP_DEFINES_ONLY
ASMFLAGS += -DCONFIG_GPIO_AS_PINRESET
ASMFLAGS += -DFLOAT_ABI_HARD
ASMFLAGS += -DMBR_PRESENT
ASMFLAGS += -DNRF52840_XXAA
ASMFLAGS += -DAPP_TIMER_V2
ASMFLAGS += -DAPP_TIMER_V2_RTC1_ENABLED

# Linker flags
LDFLAGS += $(OPT)
LDFLAGS += -mthumb -mabi=aapcs -L$(SDK_ROOT)/modules/nrfx/mdk -T$(LINKER_SCRIPT)
LDFLAGS += -mcpu=cortex-m4
LDFLAGS += -mfloat-abi=hard -mfpu=fpv4-sp-d16
# let linker dump unused sections
LDFLAGS += -Wl,--gc-sections
# use newlib in nano version
LDFLAGS += --specs=nano.specs

nrf52840_xxaa: CFLAGS += -D__HEAP_SIZE=8192
nrf52840_xxaa: CFLAGS += -D__STACK_SIZE=8192
nrf52840_xxaa: CXXFLAGS += -D__HEAP_SIZE=8192
nrf52840_xxaa: CXXFLAGS += -D__STACK_SIZE=8192
nrf52840_xxaa: ASMFLAGS += -D__HEAP_SIZE=8192
nrf52840_xxaa: ASMFLAGS += -D__STACK_SIZE=8192

# Add standard libraries at the very end of the linker input, after all objects
# that may need symbols provided by these libraries.
LIB_FILES += -lc -lnosys -lm


.PHONY: default help

# Default target - first one defined
default: nrf52840_xxaa

# Print all targets that can be built
help:
	@echo following targets are available:
	@echo		nrf52840_xxaa
	@echo		flash      - flashing binary

TEMPLATE_PATH := $(SDK_ROOT)/components/toolchain/gcc


include $(TEMPLATE_PATH)/Makefile.common

$(foreach target, $(TARGETS), $(call define_target, $(target)))

.PHONY: dfu

dfu_package: $(DFU_PACKAGE)

$(DFU_PACKAGE): $(OUTPUT_DIRECTORY)/nrf52840_xxaa.hex
	@echo Creating DFU package: $(DFU_PACKAGE)
	nrfutil pkg generate \
	   --hw-version 52 \
	   --application-version 1 \
	   --sd-req 0x0,0x102 \
	   --sd-id 0x102 \
	   --application $< \
	   --softdevice $(SDK_ROOT)/components/softdevice/s113/hex/s113_nrf52_7.2.0_softdevice.hex $@

dfu: $(DFU_PACKAGE)
	@echo Performing DFU with generated package
	nrfutil dfu usb-serial -pkg $< -p $(DFU_PORT) -b 115200
