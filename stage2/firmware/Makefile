# Portable VL53L1 TOF Sensor Firmware Makefile
# Supports GD32 (ARM Cortex-M23) and MIK32 (RISC-V) platforms

# PROJECT CONFIGURATION
TARGET_PLATFORM ?= GD32               # Default platform: GD32 or MIK32
BUILD_DIR = build                     # Output directory for build artifacts
PROJECT_NAME = vl53l1_tof_firmware    # Name of output files

# PLATFORM-SPECIFIC CONFIGURATION
# GD32E230 (ARM Cortex-M23) configuration
ifeq ($(TARGET_PLATFORM), GD32)
    MCU = cortex-m23
    ARCH = armv8-m.main
    TOOLCHAIN_PREFIX = arm-none-eabi-
    PLATFORM_CFLAGS = -DTARGET_PLATFORM=$(TARGET_PLATFORM) -DGD32E230
    PLATFORM_LDFLAGS = -mcpu=$(MCU) -mthumb -mfloat-abi=soft
    PLATFORM_SRC = platform/gd32/platform_gd32.c
    STARTUP_FILE = platform/gd32/startup_gd32e23x.S
    LINKER_SCRIPT = platform/gd32/gd32e230.ld

# MIK32 Amur (RISC-V) configuration
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

# Error handling for unsupported platforms
else
    $(error Unsupported TARGET_PLATFORM: $(TARGET_PLATFORM). Use GD32 or MIK32)
endif

# TOOLCHAIN CONFIGURATION
# MIK32 uses custom toolchain path, GD32 uses system PATH
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

# SOURCE DIRECTORY STRUCTURE
SRC_DIR = src            # Legacy source directory (not used in current structure)
APP_DIR = app            # Application layer (platform-independent)
PLATFORM_DIR = platform  # Platform abstraction layer
DRIVERS_DIR = drivers    # Hardware drivers (VL53L1X, common utilities)
CONFIG_DIR = config      # Configuration files
HAL_DIR = hal            # Hardware Abstraction Layer (legacy, not used)

# SOURCE FILES
# Platform abstraction layer (platform-independent + platform-specific)
PLATFORM_SOURCES = \
    $(PLATFORM_DIR)/platform.c \
    $(PLATFORM_SRC)

# Application layer (platform-independent business logic)
APP_SOURCES = \
    $(APP_DIR)/main.c \
    $(APP_DIR)/tof_service.c \
    $(APP_DIR)/command_parser.c

# Hardware drivers (VL53L1X sensor + common utilities)
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

# INCLUDE DIRECTORIES
INCLUDES = \
    -I$(CONFIG_DIR) \
    -I$(PLATFORM_DIR) \
    -I$(APP_DIR) \
    -I$(DRIVERS_DIR)/vl53l1 \
    -I$(DRIVERS_DIR)/common

# COMPILER AND LINKER FLAGS
# GD32 ARM Cortex-M23 compiler flags
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

# MIK32 RISC-V compiler flags
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

# Linker flags (common to both platforms)
LDFLAGS = \
    $(PLATFORM_LDFLAGS) \
    -Wl,--gc-sections \
    -Wl,--print-memory-usage \
    -T$(LINKER_SCRIPT) \
    -nostartfiles

# BUILD OBJECTS AND SOURCES
SOURCES = $(PLATFORM_SOURCES) $(APP_SOURCES) $(DRIVER_SOURCES)
OBJECTS = $(addprefix $(BUILD_DIR)/, $(SOURCES:.c=.o))
OBJECTS += $(addprefix $(BUILD_DIR)/, $(STARTUP_FILE:.S=.o))

# BUILD TARGETS
.PHONY: all clean gd32 mik32 flash debug info help

# MAIN BUILD TARGETS
# Default target: build all output formats (ELF, BIN, HEX)
all: $(BUILD_DIR)/$(PROJECT_NAME).elf $(BUILD_DIR)/$(PROJECT_NAME).bin $(BUILD_DIR)/$(PROJECT_NAME).hex

# Platform-specific convenience targets
gd32:
	$(MAKE) TARGET_PLATFORM=GD32

mik32:
	$(MAKE) TARGET_PLATFORM=MIK32

# BUILD RULES
# Create build directory structure (Windows-compatible)
$(BUILD_DIR):
	@if not exist $(BUILD_DIR) mkdir $(BUILD_DIR)
	@if not exist $(BUILD_DIR)\$(APP_DIR) mkdir $(BUILD_DIR)\$(APP_DIR)
	@if not exist $(BUILD_DIR)\$(PLATFORM_DIR) mkdir $(BUILD_DIR)\$(PLATFORM_DIR)
	@if not exist $(BUILD_DIR)\$(PLATFORM_DIR)\gd32 mkdir $(BUILD_DIR)\$(PLATFORM_DIR)\gd32
	@if not exist $(BUILD_DIR)\$(PLATFORM_DIR)\mik32 mkdir $(BUILD_DIR)\$(PLATFORM_DIR)\mik32
	@if not exist $(BUILD_DIR)\$(DRIVERS_DIR) mkdir $(BUILD_DIR)\$(DRIVERS_DIR)
	@if not exist $(BUILD_DIR)\$(DRIVERS_DIR)\vl53l1 mkdir $(BUILD_DIR)\$(DRIVERS_DIR)\vl53l1
	@if not exist $(BUILD_DIR)\$(DRIVERS_DIR)\common mkdir $(BUILD_DIR)\$(DRIVERS_DIR)\common

# Compile C source files
$(BUILD_DIR)/%.o: %.c | $(BUILD_DIR)
	@echo "Compiling $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Compile assembly source files
$(BUILD_DIR)/%.o: %.S | $(BUILD_DIR)
	@echo "Assembling $<"
	$(CC) $(CFLAGS) -c $< -o $@

# Link object files into ELF executable
$(BUILD_DIR)/$(PROJECT_NAME).elf: $(OBJECTS) | $(BUILD_DIR)
	@echo "Linking $@"
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	$(SIZE) $@

# Create binary file for flashing
$(BUILD_DIR)/$(PROJECT_NAME).bin: $(BUILD_DIR)/$(PROJECT_NAME).elf
	@echo "Creating binary $@"
	$(OBJCOPY) -O binary $< $@

# Create Intel HEX file for programming
$(BUILD_DIR)/$(PROJECT_NAME).hex: $(BUILD_DIR)/$(PROJECT_NAME).elf
	@echo "Creating hex $@"
	$(OBJCOPY) -O ihex $< $@

# Create disassembly listing for debugging
$(BUILD_DIR)/$(PROJECT_NAME).dis: $(BUILD_DIR)/$(PROJECT_NAME).elf
	@echo "Creating disassembly $@"
	$(OBJDUMP) -d $< > $@

# UTILITY TARGETS
# Clean all build artifacts (cross-platform compatible)
clean:
	-rd /s /q $(BUILD_DIR) 2>nul || rmdir /s /q $(BUILD_DIR) 2>nul || rm -rf $(BUILD_DIR)

# Display build configuration information
info:
	@echo "Target Platform: $(TARGET_PLATFORM)"
	@echo "MCU: $(MCU)"
	@echo "Toolchain: $(TOOLCHAIN_PREFIX)"
	@echo "Sources: $(SOURCES)"
	@echo "Objects: $(OBJECTS)"

# DEVELOPMENT TARGETS
# Flash firmware to target (platform-specific)
flash: $(BUILD_DIR)/$(PROJECT_NAME).bin
ifeq ($(TARGET_PLATFORM), GD32)
	@echo "Flashing GD32..."
	# Option 1: Using J-Link
	# JLinkExe -device GD32E230 -if SWD -speed 4000 -autoconnect 1 -CommanderScript flash.jlink
	# Option 2: Using ST-Link and OpenOCD
	# openocd -f interface/stlink.cfg -f target/gd32e23x.cfg -c "program $(BUILD_DIR)/$(PROJECT_NAME).bin verify reset exit"
	# Option 3: Using pyOCD
	# pyocd flash -t gd32e230 $(BUILD_DIR)/$(PROJECT_NAME).bin
	@echo "Configure your preferred flashing method above"
else ifeq ($(TARGET_PLATFORM), MIK32)
	@echo "Flashing MIK32..."
	# Option 1: Using OpenOCD with MIK32 configuration
	# openocd -f interface/jlink.cfg -f target/mik32.cfg -c "program $(BUILD_DIR)/$(PROJECT_NAME).bin verify reset exit"
	# Option 2: Using custom MIK32 programmer
	# mik32-programmer --flash $(BUILD_DIR)/$(PROJECT_NAME).bin
	# Option 3: Using RISC-V GDB with OpenOCD
	# riscv-none-elf-gdb -ex "target remote localhost:3333" -ex "load $(BUILD_DIR)/$(PROJECT_NAME).elf" -ex "monitor reset" -ex "continue"
	@echo "Configure your preferred flashing method above"
endif

# Start debugging session with GDB
debug: $(BUILD_DIR)/$(PROJECT_NAME).elf
ifeq ($(TARGET_PLATFORM), GD32)
	@echo "Starting GDB for GD32..."
	$(TOOLCHAIN_PREFIX)gdb $<
else ifeq ($(TARGET_PLATFORM), MIK32)
	@echo "Starting GDB for MIK32..."
	$(TOOLCHAIN_PREFIX)gdb $<
endif

# HELP TARGET
help:
	@echo "Portable VL53L1 TOF Sensor Firmware Build System"
	@echo ""
	@echo "Usage:"
	@echo "  make [TARGET_PLATFORM=GD32|MIK32]    # Build with specified platform"
	@echo "  make gd32                            # Build for GD32 platform"
	@echo "  make mik32                           # Build for MIK32 platform"
	@echo "  make clean                           # Clean all build artifacts"
	@echo "  make info                            # Show build configuration"
	@echo "  make flash                           # Flash firmware to target"
	@echo "  make debug                           # Start GDB debugging session"
	@echo "  make help                            # Show this help message"
	@echo ""
	@echo "Output files:"
	@echo "  $(BUILD_DIR)/$(PROJECT_NAME).elf     # ELF executable"
	@echo "  $(BUILD_DIR)/$(PROJECT_NAME).bin     # Binary for flashing"
	@echo "  $(BUILD_DIR)/$(PROJECT_NAME).hex     # Intel HEX for programming"
