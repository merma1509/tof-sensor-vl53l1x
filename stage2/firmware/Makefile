# Portable VL53L1 TOF Sensor Firmware Makefile

# Configuration
TARGET_PLATFORM ?= GD32
BUILD_DIR = build
PROJECT_NAME = vl53l1_tof_firmware

# Platform-specific settings
ifeq ($(TARGET_PLATFORM), GD32)
    MCU = cortex-m23
    ARCH = armv8-m.main
    TOOLCHAIN_PREFIX = arm-none-eabi-
    PLATFORM_CFLAGS = -DTARGET_PLATFORM=$(TARGET_PLATFORM) -DGD32E230
    PLATFORM_LDFLAGS = -mcpu=$(MCU) -mthumb -mfloat-abi=soft
    PLATFORM_SRC = platform/gd32/platform_gd32.c
    STARTUP_FILE = platform/gd32/startup_gd32e23x.S
    LINKER_SCRIPT = platform/gd32/gd32e230.ld
else ifeq ($(TARGET_PLATFORM), MIK32)
    MCU = rv32imc
    ARCH = rv32i
    TOOLCHAIN_PATH = ../mik32-toolchain/riscv-gcc/bin/
    TOOLCHAIN_PREFIX = riscv-none-elf-
    PLATFORM_CFLAGS = -DTARGET_PLATFORM=$(TARGET_PLATFORM) -DTARGET_PLATFORM_MIK32 -DMIK32
    PLATFORM_LDFLAGS = -march=$(MCU) -mabi=ilp32 -mno-relax -nostartfiles
    PLATFORM_SRC = platform/mik32/platform_mik32.c
    STARTUP_FILE = platform/mik32/startup_mik32.S
    LINKER_SCRIPT = platform/mik32/mik32.ld
else
    $(error Unsupported TARGET_PLATFORM: $(TARGET_PLATFORM))
endif

# Toolchain
ifeq ($(TARGET_PLATFORM), MIK32)
    CC = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)gcc
    OBJCOPY = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)objcopy
    OBJDUMP = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)objdump
    SIZE = $(TOOLCHAIN_PATH)$(TOOLCHAIN_PREFIX)size
else
    CC = $(TOOLCHAIN_PREFIX)gcc
    OBJCOPY = $(TOOLCHAIN_PREFIX)objcopy
    OBJDUMP = $(TOOLCHAIN_PREFIX)objdump
    SIZE = $(TOOLCHAIN_PREFIX)size
endif

# Directories
SRC_DIR = src
APP_DIR = app
PLATFORM_DIR = platform
DRIVERS_DIR = drivers
CONFIG_DIR = config
HAL_DIR = hal

# Source files
PLATFORM_SOURCES = \
    $(PLATFORM_DIR)/platform.c \
    $(PLATFORM_SRC)

APP_SOURCES = \
    $(APP_DIR)/main.c \
    $(APP_DIR)/tof_service.c \
    $(APP_DIR)/command_parser.c

DRIVER_SOURCES = \
    $(DRIVERS_DIR)/vl53l1/vl53l1_platform.c \
    $(DRIVERS_DIR)/vl53l1/VL53L1X_api.c \
    $(DRIVERS_DIR)/vl53l1/VL53L1X_calibration.c \
    $(DRIVERS_DIR)/common/string_utils.c \
    $(DRIVERS_DIR)/common/math_utils.c \
    $(DRIVERS_DIR)/common/delay_utils.c \
    $(DRIVERS_DIR)/common/debug_utils.c \
    $(DRIVERS_DIR)/common/crc_utils.c \
    $(DRIVERS_DIR)/common/endian_utils.c

# Include directories
INCLUDES = \
    -I$(CONFIG_DIR) \
    -I$(PLATFORM_DIR) \
    -I$(APP_DIR) \
    -I$(DRIVERS_DIR)/vl53l1 \
    -I$(DRIVERS_DIR)/common

# Compiler flags
ifeq ($(TARGET_PLATFORM), GD32)
    CFLAGS = \
        -mcpu=$(MCU) \
        -mthumb \
        -O2 \
        -g \
        -Wall \
        -Wextra \
        -ffunction-sections \
        -fdata-sections \
        $(PLATFORM_CFLAGS) \
        $(INCLUDES) \
        -std=c99
else ifeq ($(TARGET_PLATFORM), MIK32)
    CFLAGS = \
        -march=$(MCU) \
        -mabi=ilp32 \
        -mno-relax \
        -O2 \
        -g \
        -Wall \
        -Wextra \
        -ffunction-sections \
        -fdata-sections \
        $(PLATFORM_CFLAGS) \
        $(INCLUDES) \
        -std=c99
endif

# Linker flags
LDFLAGS = \
    $(PLATFORM_LDFLAGS) \
    -Wl,--gc-sections \
    -Wl,--print-memory-usage \
    -T$(LINKER_SCRIPT) \
    -nostartfiles

# Object files
OBJECTS = $(addprefix $(BUILD_DIR)/, $(SOURCES:.c=.o))
OBJECTS += $(addprefix $(BUILD_DIR)/, $(STARTUP_FILE:.S=.o))

# All sources
SOURCES = $(PLATFORM_SOURCES) $(APP_SOURCES) $(DRIVER_SOURCES)

# Build targets
.PHONY: all clean gd32 mik32 flash debug

all: $(BUILD_DIR)/$(PROJECT_NAME).elf $(BUILD_DIR)/$(PROJECT_NAME).bin $(BUILD_DIR)/$(PROJECT_NAME).hex

gd32:
	$(MAKE) TARGET_PLATFORM=GD32

mik32:
	$(MAKE) TARGET_PLATFORM=MIK32

# Create build directory
$(BUILD_DIR):
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	@if not exist $(BUILD_DIR)\$(APP_DIR) mkdir $(BUILD_DIR)\$(APP_DIR)
	@if not exist $(BUILD_DIR)\$(PLATFORM_DIR) mkdir $(BUILD_DIR)\$(PLATFORM_DIR)
	@if not exist $(BUILD_DIR)\$(PLATFORM_DIR)\gd32 mkdir $(BUILD_DIR)\$(PLATFORM_DIR)\gd32
	@if not exist $(BUILD_DIR)\$(PLATFORM_DIR)\mik32 mkdir $(BUILD_DIR)\$(PLATFORM_DIR)\mik32
	@if not exist $(BUILD_DIR)\$(DRIVERS_DIR) mkdir $(BUILD_DIR)\$(DRIVERS_DIR)
	@if not exist $(BUILD_DIR)\$(DRIVERS_DIR)\vl53l1 mkdir $(BUILD_DIR)\$(DRIVERS_DIR)\vl53l1
	@if not exist $(BUILD_DIR)\$(DRIVERS_DIR)\common mkdir $(BUILD_DIR)\$(DRIVERS_DIR)\common

# Compile C files
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Compile assembly files
$(BUILD_DIR)/%.o: %.S | $(BUILD_DIR)
	@echo "Assembling $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Link
$(BUILD_DIR)/$(PROJECT_NAME).elf: $(OBJECTS) | $(BUILD_DIR)
	@echo "Linking $@"
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	$(SIZE) $@

# Create binary
$(BUILD_DIR)/$(PROJECT_NAME).bin: $(BUILD_DIR)/$(PROJECT_NAME).elf
	@echo "Creating binary $@"
	$(OBJCOPY) -O binary $< $@

# Create hex
$(BUILD_DIR)/$(PROJECT_NAME).hex: $(BUILD_DIR)/$(PROJECT_NAME).elf
	@echo "Creating hex $@"
	$(OBJCOPY) -O ihex $< $@

# Create disassembly
$(BUILD_DIR)/$(PROJECT_NAME).dis: $(BUILD_DIR)/$(PROJECT_NAME).elf
	@echo "Creating disassembly $@"
	$(OBJDUMP) -d $< > $@

# Clean
clean:
	-rd /s /q $(BUILD_DIR) 2>nul || rmdir /s /q $(BUILD_DIR) 2>nul || rm -rf $(BUILD_DIR)

# Info
info:
	@echo "Target Platform: $(TARGET_PLATFORM)"
	@echo "MCU: $(MCU)"
	@echo "Toolchain: $(TOOLCHAIN_PREFIX)"
	@echo "Sources: $(SOURCES)"
	@echo "Objects: $(OBJECTS)"

# Flash (platform-specific)
flash: $(BUILD_DIR)/$(PROJECT_NAME).bin
ifeq ($(TARGET_PLATFORM), GD32)
	@echo "Flashing GD32..."
	# TODO: Add GD32 flashing command
else ifeq ($(TARGET_PLATFORM), MIK32)
	@echo "Flashing MIK32..."
	# TODO: Add MIK32 flashing command
endif

# Debug
debug: $(BUILD_DIR)/$(PROJECT_NAME).elf
ifeq ($(TARGET_PLATFORM), GD32)
	@echo "Starting GDB for GD32..."
	$(TOOLCHAIN_PREFIX)gdb $<
else ifeq ($(TARGET_PLATFORM), MIK32)
	@echo "Starting GDB for MIK32..."
	$(TOOLCHAIN_PREFIX)gdb $<
endif

# Help
help:
	@echo "Usage:"
	@echo "  make [TARGET_PLATFORM=GD32|MIK32]"
	@echo "  make gd32                    # Build for GD32"
	@echo "  make mik32                   # Build for MIK32"
	@echo "  make clean                   # Clean build"
	@echo "  make info                    # Show configuration"
	@echo "  make flash                   # Flash firmware"
	@echo "  make debug                   # Start debugger"
