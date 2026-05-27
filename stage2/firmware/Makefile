# Portable VL53L1 TOF Sensor Firmware Build System

TARGET_PLATFORM ?= GD32
BUILD_DIR = build
PROJECT_NAME = vl53l1_tof_firmware

# ARM GCC Toolchain Detection
# Check if arm-none-eabi-gcc is available
ifeq ($(shell where arm-none-eabi-gcc 2>NUL),)
    # GCC not in PATH, try common installation locations
    ifeq ($(wildcard $(USERPROFILE)/.platformio/packages/toolchain-gccarmnoneeabi/bin/arm-none-eabi-gcc.exe),)
        ifeq ($(wildcard C:/gcc-arm-none-eabi/bin/arm-none-eabi-gcc.exe),)
            ifeq ($(wildcard C:/devkit/arm/bin/arm-none-eabi-gcc.exe),)
                $(error ARM GCC toolchain not found. Please install ARM GCC or add it to PATH. Install options: 1) PlatformIO: pio install toolchain-gccarmnoneeabi 2) GNU ARM Embedded: https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
            else
                TOOLCHAIN_PREFIX := C:/devkit/arm/bin/arm-none-eabi-
                export PATH := C:/devkit/arm/bin;$(PATH)
                $(info Found ARM GCC at: C:/devkit/arm/bin)
            endif
        else
            TOOLCHAIN_PREFIX := C:/gcc-arm-none-eabi/bin/arm-none-eabi-
            export PATH := C:/gcc-arm-none-eabi/bin;$(PATH)
            $(info Found ARM GCC at: C:/gcc-arm-none-eabi/bin)
        endif
    else
        TOOLCHAIN_PREFIX := $(USERPROFILE)/.platformio/packages/toolchain-gccarmnoneeabi/bin/arm-none-eabi-
        export PATH := $(USERPROFILE)/.platformio/packages/toolchain-gccarmnoneeabi/bin;$(PATH)
        $(info Found ARM GCC at: $(USERPROFILE)/.platformio/packages/toolchain-gccarmnoneeabi/bin)
    endif
else
    TOOLCHAIN_PREFIX := arm-none-eabi-
    $(info Using ARM GCC from PATH)
endif

# Platform configuration
ifeq ($(TARGET_PLATFORM), GD32)
    MCU = GD32E230
    ARCH = cortex-m23
    # Set default TOOLCHAIN_PREFIX if not already detected
    TOOLCHAIN_PREFIX ?= arm-none-eabi-
    PLATFORM_DIR = platform
    PLATFORM_SPECIFIC_DIR = platform/gd32
    LINKER_SCRIPT = $(PLATFORM_SPECIFIC_DIR)/gd32e230.ld
    CFLAGS = -mcpu=$(ARCH) -mthumb -mfloat-abi=soft -Wall -Wextra -Os -g -ffunction-sections -fdata-sections -std=c99
    LDFLAGS = -mcpu=$(ARCH) -mthumb -mfloat-abi=soft -Wl,--gc-sections -T$(LINKER_SCRIPT) -nostartfiles
else ifeq ($(TARGET_PLATFORM), MIK32)
    MCU = MIK32
    ARCH = rv32imc
    # Set default TOOLCHAIN_PREFIX if not already detected
    TOOLCHAIN_PREFIX ?= riscv-none-embed-
    PLATFORM_DIR = platform/mik32
    LINKER_SCRIPT = $(PLATFORM_DIR)/mik32_flash.ld
    CFLAGS = -march=$(ARCH) -mabi=ilp32 -Wall -Wextra -Os -g -ffunction-sections -fdata-sections -std=c99
    LDFLAGS = -march=$(ARCH) -mabi=ilp32 -Wl,--gc-sections -T$(LINKER_SCRIPT) -nostartfiles
else
    $(error Unsupported platform: $(TARGET_PLATFORM))
endif

# Tools
CC = $(TOOLCHAIN_PREFIX)gcc
OBJCOPY = $(TOOLCHAIN_PREFIX)objcopy
SIZE = $(TOOLCHAIN_PREFIX)size

# Source files
PLATFORM_SOURCES = $(PLATFORM_DIR)/platform.c $(wildcard $(PLATFORM_SPECIFIC_DIR)/*.c)
APP_SOURCES = $(wildcard app/*.c)
DRIVER_SOURCES = $(wildcard drivers/vl53l1/*.c) $(wildcard drivers/common/*.c)
SOURCES = $(PLATFORM_SOURCES) $(APP_SOURCES) $(DRIVER_SOURCES)

# Object files
OBJECTS = $(addprefix $(BUILD_DIR)/, $(SOURCES:.c=.o))

# Include directories
INCLUDES = -I. -Iapp -Idrivers -Idrivers/vl53l1 -Idrivers/common -I$(PLATFORM_DIR) -I$(PLATFORM_DIR)/$(TARGET_PLATFORM) -Iconfig
CFLAGS += $(INCLUDES)

# Build targets
.PHONY: all clean gd32 mik32 flash info help test test-common test-tof test-platform

all: clean $(BUILD_DIR)/$(PROJECT_NAME).elf $(BUILD_DIR)/$(PROJECT_NAME).bin

gd32:
	$(MAKE) TARGET_PLATFORM=GD32

mik32:
	$(MAKE) TARGET_PLATFORM=MIK32

# Build rules
$(BUILD_DIR):
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)

$(BUILD_DIR)/platform/%.o: platform/%.c | $(BUILD_DIR)
	@if not exist $(BUILD_DIR)\platform mkdir $(BUILD_DIR)\platform
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/platform/gd32/%.o: platform/gd32/%.c | $(BUILD_DIR)
	@if not exist $(BUILD_DIR)\platform\gd32 mkdir $(BUILD_DIR)\platform\gd32
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/app/%.o: app/%.c | $(BUILD_DIR)
	@if not exist $(BUILD_DIR)\app mkdir $(BUILD_DIR)\app
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/drivers/vl53l1/%.o: drivers/vl53l1/%.c | $(BUILD_DIR)
	@if not exist $(BUILD_DIR)\drivers\vl53l1 mkdir $(BUILD_DIR)\drivers\vl53l1
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/drivers/common/%.o: drivers/common/%.c | $(BUILD_DIR)
	@if not exist $(BUILD_DIR)\drivers\common mkdir $(BUILD_DIR)\drivers\common
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(PROJECT_NAME).elf: $(OBJECTS) | $(BUILD_DIR)
	@echo "Linking $@"
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	$(SIZE) $@

$(BUILD_DIR)/$(PROJECT_NAME).bin: $(BUILD_DIR)/$(PROJECT_NAME).elf
	@echo "Creating binary $@"
	$(OBJCOPY) -O binary $< $@

# Utility targets
clean:
	@if exist $(BUILD_DIR) rd /s /q $(BUILD_DIR)

info:
	@echo "Platform: $(TARGET_PLATFORM)"
	@echo "MCU: $(MCU)"
	@echo "Sources: $(SOURCES)"

flash: $(BUILD_DIR)/$(PROJECT_NAME).bin
	@echo "Flashing $(TARGET_PLATFORM)..."
	@echo "Configure flashing method"

debug: $(BUILD_DIR)/$(PROJECT_NAME).elf
	@echo "Debugging $(TARGET_PLATFORM)..."

# Test targets
test: test-common test-tof test-platform
	@echo "All tests completed for $(TARGET_PLATFORM)"

test-common: $(BUILD_DIR)/tests/test_common_utils.o
	@echo "Common utilities test compiled"

test-tof: $(BUILD_DIR)/tests/test_tof_service.o
	@echo "TOF service test compiled"

test-platform: $(BUILD_DIR)/tests/test_platform.o
	@echo "Platform test compiled"

# Test build rules
$(BUILD_DIR)/tests/%.o: tests/%.c | $(BUILD_DIR)
	@if not exist $(BUILD_DIR)\tests mkdir $(BUILD_DIR)\tests
	@echo "Compiling test $<"
	$(CC) $(CFLAGS) -c $< -o $@

help:
	@echo "Usage: make [TARGET_PLATFORM=GD32|MIK32]"
	@echo "Targets: all clean gd32 mik32 flash info debug test test-common test-tof test-platform help"
