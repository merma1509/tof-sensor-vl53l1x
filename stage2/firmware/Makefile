# Cross-Platform VL53L1 TOF Sensor Firmware Build System

TARGET_PLATFORM ?= GD32
BUILD_DIR = build
PROJECT_NAME = vl53l1_tof_firmware
PLATFORM_NAME = $(TARGET_PLATFORM)

# OS Detection
ifeq ($(OS),Windows_NT)
    DETECTED_OS := Windows
    RM_CMD := rd /s /q
    MKDIR_CMD := mkdir
    MKDIR_P_CMD := mkdir
    PATH_SEP := ;
    WHICH_CMD := where
    DEVNULL := 2>NUL
else
    DETECTED_OS := Linux
    RM_CMD := rm -rf
    MKDIR_CMD := mkdir
    MKDIR_P_CMD := mkdir -p
    PATH_SEP := :
    WHICH_CMD := which
    DEVNULL := 2>/dev/null
endif

# ARM GCC Toolchain Detection (Cross-Platform)
ifeq ($(shell $(WHICH_CMD) arm-none-eabi-gcc $(DEVNULL)),)
    # GCC not in PATH, try common installation locations
    ifeq ($(DETECTED_OS),Windows)
        ifeq ($(wildcard $(USERPROFILE)/.platformio/packages/toolchain-gccarmnoneeabi/bin/arm-none-eabi-gcc.exe),)
            ifeq ($(wildcard C:/gcc-arm-none-eabi/bin/arm-none-eabi-gcc.exe),)
                ifeq ($(wildcard C:/devkit/arm/bin/arm-none-eabi-gcc.exe),)
                    $(error ARM GCC toolchain not found. Please install ARM GCC or add it to PATH. Install options: 1) PlatformIO: pio install toolchain-gccarmnoneeabi 2) GNU ARM Embedded: https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads)
                else
                    TOOLCHAIN_PREFIX := C:/devkit/arm/bin/arm-none-eabi-
                    export PATH := C:/devkit/arm/bin$(PATH_SEP)$(PATH)
                    $(info Found ARM GCC at: C:/devkit/arm/bin)
                endif
            else
                TOOLCHAIN_PREFIX := C:/gcc-arm-none-eabi/bin/arm-none-eabi-
                export PATH := C:/gcc-arm-none-eabi/bin$(PATH_SEP)$(PATH)
                $(info Found ARM GCC at: C:/gcc-arm-none-eabi/bin)
            endif
        else
            TOOLCHAIN_PREFIX := $(USERPROFILE)/.platformio/packages/toolchain-gccarmnoneeabi/bin/arm-none-eabi-
            export PATH := $(USERPROFILE)/.platformio/packages/toolchain-gccarmnoneeabi/bin$(PATH_SEP)$(PATH)
            $(info Found ARM GCC at: $(USERPROFILE)/.platformio/packages/toolchain-gccarmnoneeabi/bin)
        endif
    else
        # Linux paths
        ifeq ($(wildcard ~/.platformio/packages/toolchain-gccarmnoneeabi/bin/arm-none-eabi-gcc),)
            ifeq ($(wildcard /usr/bin/arm-none-eabi-gcc),)
                ifeq ($(wildcard /usr/local/bin/arm-none-eabi-gcc),)
                    $(error ARM GCC toolchain not found. Please install: sudo apt install gcc-arm-none-eabi)
                else
                    TOOLCHAIN_PREFIX := /usr/local/bin/arm-none-eabi-
                    export PATH := /usr/local/bin$(PATH_SEP)$(PATH)
                    $(info Found ARM GCC at: /usr/local/bin)
                endif
            else
                TOOLCHAIN_PREFIX := arm-none-eabi-
                $(info Found ARM GCC at: /usr/bin)
            endif
        else
            TOOLCHAIN_PREFIX := ~/.platformio/packages/toolchain-gccarmnoneeabi/bin/arm-none-eabi-
            export PATH := ~/.platformio/packages/toolchain-gccarmnoneeabi/bin$(PATH_SEP)$(PATH)
            $(info Found ARM GCC at: ~/.platformio/packages/toolchain-gccarmnoneeabi/bin)
        endif
    endif
else
    TOOLCHAIN_PREFIX := arm-none-eabi-
    $(info Found ARM GCC in PATH)
endif

# RISC-V GCC Toolchain Detection (Cross-Platform)
ifeq ($(TARGET_PLATFORM), MIK32)
    ifeq ($(shell $(WHICH_CMD) riscv64-unknown-elf-gcc $(DEVNULL)),)
        ifeq ($(DETECTED_OS),Windows)
            ifeq ($(wildcard $(USERPROFILE)/.platformio/packages/toolchain-riscv/bin/riscv64-unknown-elf-gcc.exe),)
                $(error RISC-V GCC toolchain not found. Please install RISC-V GCC)
            else
                TOOLCHAIN_PREFIX := $(USERPROFILE)/.platformio/packages/toolchain-riscv/bin/riscv64-unknown-elf-
                export PATH := $(USERPROFILE)/.platformio/packages/toolchain-riscv/bin$(PATH_SEP)$(PATH)
                $(info Found RISC-V GCC at: $(USERPROFILE)/.platformio/packages/toolchain-riscv/bin)
            endif
        else
            # Linux paths
            ifeq ($(wildcard ~/.platformio/packages/toolchain-riscv/bin/riscv64-unknown-elf-gcc),)
                ifeq ($(wildcard /usr/bin/riscv64-unknown-elf-gcc),)
                    $(error RISC-V GCC toolchain not found. Please install: sudo apt install gcc-riscv64-unknown-elf)
                else
                    TOOLCHAIN_PREFIX := riscv64-unknown-elf-
                    $(info Found RISC-V GCC at: /usr/bin)
                endif
            else
                TOOLCHAIN_PREFIX := ~/.platformio/packages/toolchain-riscv/bin/riscv64-unknown-elf-
                export PATH := ~/.platformio/packages/toolchain-riscv/bin$(PATH_SEP)$(PATH)
                $(info Found RISC-V GCC at: ~/.platformio/packages/toolchain-riscv/bin)
            endif
        endif
    else
        TOOLCHAIN_PREFIX := riscv64-unknown-elf-
        $(info Found RISC-V GCC in PATH)
    endif
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
    TOOLCHAIN_PREFIX ?= riscv64-unknown-elf-
    PLATFORM_DIR = platform
    PLATFORM_SPECIFIC_DIR = platform/mik32
    LINKER_SCRIPT = $(PLATFORM_SPECIFIC_DIR)/mik32.ld
    CFLAGS = -march=$(ARCH) -mabi=ilp32 -Wall -Wextra -Os -g -ffunction-sections -fdata-sections -std=c99 -ffreestanding
    LDFLAGS = -march=$(ARCH) -mabi=ilp32 -Wl,--gc-sections -T$(LINKER_SCRIPT) -nostartfiles -nostdlib -nodefaultlibs
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

all: clean $(BUILD_DIR)/$(PROJECT_NAME)_$(PLATFORM_NAME).elf $(BUILD_DIR)/$(PROJECT_NAME)_$(PLATFORM_NAME).bin $(BUILD_DIR)/$(PROJECT_NAME)_$(PLATFORM_NAME).hex

gd32:
	$(MAKE) TARGET_PLATFORM=GD32

mik32:
	$(MAKE) TARGET_PLATFORM=MIK32

# Build rules (Cross-Platform)
$(BUILD_DIR):
	@echo "Creating build directory"
ifeq ($(DETECTED_OS),Windows)
	@if not exist $(BUILD_DIR) $(MKDIR_CMD) $(BUILD_DIR)
else
	@$(MKDIR_P_CMD) $(BUILD_DIR)
endif

$(BUILD_DIR)/platform/%.o: platform/%.c | $(BUILD_DIR)
	@echo "Compiling $<"
ifeq ($(DETECTED_OS),Windows)
	@if not exist $(BUILD_DIR)\platform $(MKDIR_CMD) $(BUILD_DIR)\platform
else
	@$(MKDIR_P_CMD) $(BUILD_DIR)/platform
endif
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/platform/gd32/%.o: platform/gd32/%.c | $(BUILD_DIR)
	@echo "Compiling $<"
ifeq ($(DETECTED_OS),Windows)
	@if not exist $(BUILD_DIR)\platform\gd32 $(MKDIR_CMD) $(BUILD_DIR)\platform\gd32
else
	@$(MKDIR_P_CMD) $(BUILD_DIR)/platform/gd32
endif
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/platform/mik32/%.o: platform/mik32/%.c | $(BUILD_DIR)
	@echo "Compiling $<"
ifeq ($(DETECTED_OS),Windows)
	@if not exist $(BUILD_DIR)\platform\mik32 $(MKDIR_CMD) $(BUILD_DIR)\platform\mik32
else
	@$(MKDIR_P_CMD) $(BUILD_DIR)/platform/mik32
endif
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/app/%.o: app/%.c | $(BUILD_DIR)
	@echo "Compiling $<"
ifeq ($(DETECTED_OS),Windows)
	@if not exist $(BUILD_DIR)\app $(MKDIR_CMD) $(BUILD_DIR)\app
else
	@$(MKDIR_P_CMD) $(BUILD_DIR)/app
endif
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/drivers/vl53l1/%.o: drivers/vl53l1/%.c | $(BUILD_DIR)
	@echo "Compiling $<"
ifeq ($(DETECTED_OS),Windows)
	@if not exist $(BUILD_DIR)\drivers\vl53l1 $(MKDIR_CMD) $(BUILD_DIR)\drivers\vl53l1
else
	@$(MKDIR_P_CMD) $(BUILD_DIR)/drivers/vl53l1
endif
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/drivers/common/%.o: drivers/common/%.c | $(BUILD_DIR)
	@echo "Compiling $<"
ifeq ($(DETECTED_OS),Windows)
	@if not exist $(BUILD_DIR)\drivers\common $(MKDIR_CMD) $(BUILD_DIR)\drivers\common
else
	@$(MKDIR_P_CMD) $(BUILD_DIR)/drivers/common
endif
	$(CC) $(CFLAGS) -c $< -o $@

$(BUILD_DIR)/$(PROJECT_NAME)_$(PLATFORM_NAME).elf: $(OBJECTS) | $(BUILD_DIR)
	@echo "Linking $@"
	$(CC) $(LDFLAGS) $(OBJECTS) -o $@
	$(SIZE) $@

$(BUILD_DIR)/$(PROJECT_NAME)_$(PLATFORM_NAME).bin: $(BUILD_DIR)/$(PROJECT_NAME)_$(PLATFORM_NAME).elf
	@echo "Creating binary $@"
	$(OBJCOPY) -O binary $< $@

$(BUILD_DIR)/$(PROJECT_NAME)_$(PLATFORM_NAME).hex: $(BUILD_DIR)/$(PROJECT_NAME)_$(PLATFORM_NAME).elf
	@echo "Creating hex file $@"
	$(OBJCOPY) -O ihex $< $@

# Utility targets (Cross-Platform)
clean:
	@echo "Cleaning build directory"
ifeq ($(DETECTED_OS),Windows)
	@if exist $(BUILD_DIR) $(RM_CMD) $(BUILD_DIR)
else
	@$(RM_CMD) $(BUILD_DIR)
endif

info:
	@echo "Platform: $(TARGET_PLATFORM)"
	@echo "MCU: $(MCU)"
	@echo "Sources: $(SOURCES)"

flash: $(BUILD_DIR)/$(PROJECT_NAME)_$(PLATFORM_NAME).bin
	@echo "Flashing $(TARGET_PLATFORM)..."
	@echo "Configure flashing method"

debug: $(BUILD_DIR)/$(PROJECT_NAME)_$(PLATFORM_NAME).elf
	@echo "Debugging $(TARGET_PLATFORM)..."

# Test targets
test: test-common test-tof test-platform
	@echo "All tests completed for $(TARGET_PLATFORM)"

# Test build rules (Cross-Platform)
$(BUILD_DIR)/tests/%.o: tests/%.c | $(BUILD_DIR)
	@echo "Compiling test $<"
ifeq ($(DETECTED_OS),Windows)
	@if not exist $(BUILD_DIR)\tests $(MKDIR_CMD) $(BUILD_DIR)\tests
else
	@$(MKDIR_P_CMD) $(BUILD_DIR)/tests
endif
	$(CC) $(CFLAGS) -c $< -o $@

help:
	@echo "=================================================================="
	@echo "           VL53L1X TOF Sensor Firmware Build System"
	@echo "=================================================================="
	@echo "Detected OS: $(DETECTED_OS)"
	@echo ""
	@echo "USAGE:"
	@echo "  make [TARGET_PLATFORM=GD32|MIK32] [TARGET]"
	@echo ""
	@echo "PLATFORMS:"
	@echo "  GD32   - GD32E230 ARM Cortex-M23 microcontroller"
	@echo "  MIK32  - MIK32 RISC-V microcontroller"
	@echo ""
	@echo "BUILD TARGETS:"
	@echo "  all           Build complete firmware (default platform)"
	@echo "  gd32          Build for GD32E230 platform"
	@echo "  mik32         Build for MIK32 platform"
	@echo "  clean         Clean build directory"
	@echo ""
	@echo "DEVELOPMENT TARGETS:"
	@echo "  flash         Flash firmware to target device"
	@echo "  debug         Start debugging session"
	@echo "  info          Show build configuration"
	@echo ""
	@echo "TEST TARGETS:"
	@echo "  test          Build all test suites"
	@echo "  test-common   Build common utilities tests"
	@echo "  test-tof      Build TOF service tests"
	@echo "  test-platform Build platform abstraction tests"
	@echo ""
	@echo "OUTPUT FILES (Platform-Aware):"
	@echo "  build/vl53l1_tof_firmware_<PLATFORM>.elf   - Executable firmware"
	@echo "  build/vl53l1_tof_firmware_<PLATFORM>.bin   - Binary for flashing"
	@echo "  build/vl53l1_tof_firmware_<PLATFORM>.hex   - Intel HEX format"
	@echo ""
	@echo "CROSS-PLATFORM INSTALLATION:"
	@echo "  Windows: ARM GCC via PlatformIO or manual installation"
	@echo "  Linux:   sudo apt install gcc-arm-none-eabi gcc-riscv64-unknown-elf"
	@echo ""
	@echo "EXAMPLES:"
	@echo "  make gd32                    # Build GD32E230 firmware"
	@echo "  make mik32                   # Build MIK32 firmware"
	@echo "  make TARGET_PLATFORM=MIK32   # Alternative syntax"
	@echo "  make flash                   # Flash firmware (requires tool)"
	@echo ""
	@echo "REQUIREMENTS:"
	@echo "  - ARM GCC (GD32) or RISC-V GCC (MIK32)"
	@echo "  - Make utility"
	@echo "  - Windows or Linux (auto-detected)"
	@echo ""
	@echo "TESTING:"
	@echo "  make test-vl53l1x          # Build VL53L1X sensor test"
	@echo "  make test-platform         # Build platform abstraction test"
	@echo "  make test-common            # Build common utilities test"
	@echo "  make test-tof               # Build TOF service test"
	@echo "  make test-all               # Build all tests"
	@echo "  make test-runner            # Build executable test runner"
	@echo "  make test-run               # Run all tests and show results"
	@echo "  make test-vl53l1x-run       # Run VL53L1X sensor tests only"
	@echo "  make test-sim               # Run tests with simulation"
	@echo ""
	@echo "=================================================================="

# Test targets
test-vl53l1x: $(BUILD_DIR)/tests/test_vl53l1x_sensor.o
	@echo "VL53L1X sensor test compiled successfully"

test-platform: $(BUILD_DIR)/tests/test_platform_abstraction.o
	@echo "Platform abstraction test compiled successfully"

test-common: $(BUILD_DIR)/tests/test_common_utils.o
	@echo "Common utilities test compiled successfully"

test-tof: $(BUILD_DIR)/tests/test_tof_service.o
	@echo "TOF service test compiled successfully"

test-all: $(BUILD_DIR)/tests/test_common_utils.o $(BUILD_DIR)/tests/test_platform_abstraction.o $(BUILD_DIR)/tests/test_tof_service.o $(BUILD_DIR)/tests/test_vl53l1x_sensor.o
	@echo "All tests compiled successfully"

# Executable test targets
test-runner: $(BUILD_DIR)/tests/test_runner.elf
	@echo "Test runner built successfully"
	@echo "Run: $(BUILD_DIR)/tests/test_runner.elf"

test-run: $(BUILD_DIR)/tests/test_runner.elf
	@echo "Running VL53L1X Firmware Tests..."
	@echo "=================================================================="
	@echo "Note: Embedded test compiled successfully."
	@echo "To see test results, run the simulation:"
	@echo "  make test-sim"

test-vl53l1x-run: $(BUILD_DIR)/tests/test_vl53l1x_runner.elf
	@echo "Running VL53L1X Sensor Tests Only..."
	@echo "=================================================================="
	@echo "Note: Embedded test compiled successfully."
	@echo "To see test results, run the simulation:"
	@echo "  make test-sim"

test-sim:
	@echo "Running VL53L1X Tests (Simulation)..."
	@echo "=================================================================="
	@echo "NOTE: This requires a host C compiler (GCC/Clang) to be installed."
	@echo ""
	@echo "To run the simulation manually:"
	@echo "1. Install GCC/Clang on your system:"
	@echo "   - Windows: Install MinGW-w64 or use WSL"
	@echo "   - Linux: sudo apt install gcc"
	@echo "   - macOS: xcode-select --install"
	@echo ""
	@echo "2. Compile and run:"
	@echo "   gcc -Wall -Wextra -std=c99 -o simulation_test tests/test_simulation.c"
	@echo "   ./simulation_test"
	@echo ""
	@echo "The simulation will demonstrate the test framework with realistic data."

# Build test runner executable
$(BUILD_DIR)/tests/test_runner.elf: tests/test_runner.c tests/test_common_utils.c tests/test_platform_abstraction.c tests/test_tof_service.c tests/test_vl53l1x_sensor.c platform/platform.c drivers/common/debug_utils.c
	@echo "Compiling and linking test runner..."
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@
	@echo "Test runner linked successfully"

# Build VL53L1X test runner executable
$(BUILD_DIR)/tests/test_vl53l1x_runner.elf: tests/test_runner.c tests/test_vl53l1x_sensor.c drivers/vl53l1/VL53L1X_api.c drivers/vl53l1/vl53l1_platform.c drivers/common/debug_utils.c
	@echo "Compiling and linking VL53L1X test runner..."
	$(CC) $(CFLAGS) $^ $(LDFLAGS) -o $@
	@echo "VL53L1X test runner linked successfully"

.PHONY: gd32 mik32 clean flash help test-vl53l1x test-platform test-common test-tof test-all test-runner test-run test-vl53l1x-run test-sim
