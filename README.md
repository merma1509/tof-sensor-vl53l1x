# VL53L1X Firmware

## Overview
Multi-stage embedded firmware project for VL53L1X Time-of-Flight (ToF) sensor supporting both GD32E230 and MIK32 microcontrollers. Provides complete distance measurement capabilities with I2C communication and UART command interface.

## Project Structure
```
clear-space-ru/
├── stage1/                    # PlatformIO-based implementation
│   ├── src/                   # Source code for GD32E230
│   ├── lib/                   # VL53L1X library
│   ├── platformio.ini         # PlatformIO configuration
│   └── build.bat              # Windows build script
└── stage2/                    # Multi-platform Makefile implementation
    ├── firmware/              # Main firmware directory
    │   ├── app/               # Application logic
    │   ├── drivers/           # Sensor and utility drivers
    │   ├── platform/          # Hardware abstraction layer
    │   │   ├── gd32/          # GD32E230 platform
    │   │   └── mik32/         # MIK32 platform
    │   ├── tests/             # Unit tests
    │   ├── build.bat          # Windows build script
    │   └── Makefile           # Build configuration
```

## Hardware Support

### GD32E230 Platform
- **Microcontroller**: GD32E230 (ARM Cortex-M23)
- **Sensor**: VL53L1X ToF sensor
- **Communication**: I2C (sensor), UART (commands/debug)
- **Toolchain**: ARM GCC (arm-none-eabi-gcc)

### MIK32 Platform
- **Microcontroller**: MIK32 (RISC-V)
- **Sensor**: VL53L1X ToF sensor
- **Communication**: I2C (sensor), UART (commands/debug)
- **Toolchain**: RISC-V GCC (riscv-none-embed-gcc)

## Features
- VL53L1X sensor initialization and configuration
- Distance measurement (2-4000mm range)
- I2C communication with sensor
- UART command interface
- Multi-platform abstraction layer
- Automatic toolchain detection (ARM GCC/RISC-V GCC)
- Complete error handling and timeout management

## Build Requirements

### Stage 1 (PlatformIO)
- PlatformIO IDE or CLI
- VS Code (recommended)
- Python 3.7+

### Stage 2 (Makefile)
- ARM GCC toolchain (arm-none-eabi-gcc) for GD32E230
- RISC-V GCC toolchain (riscv-none-embed-gcc) for MIK32
- Make utility
- Windows (tested) or Linux

## Quick Start

### Stage 1 - PlatformIO Development
```bash
# Navigate to stage1 directory
cd stage1

# Build with PlatformIO
pio run

# Upload to target
pio run --target upload
```

### Stage 2 - Multi-Platform Development

#### Windows
```bash
# Navigate to firmware directory
cd stage2/firmware

# Build GD32E230 firmware
make gd32

# Build MIK32 firmware
make mik32

# Or use batch script with auto-detection
build.bat
```

#### Linux
```bash
# Navigate to firmware directory
cd stage2/firmware

# Build GD32E230 firmware
make gd32

# Build MIK32 firmware
make mik32
```

## Build Targets
- `make all`      - Build complete firmware (default platform)
- `make gd32`     - Build for GD32E230 platform
- `make mik32`    - Build for MIK32 platform
- `make clean`    - Clean build directory
- `make test`     - Build test suite
- `make help`     - Show available targets

## Output Files

### GD32E230 Platform
- `build/vl53l1_tof_firmware_GD32.elf` - Executable firmware
- `build/vl53l1_tof_firmware_GD32.bin` - Binary for flashing
- `build/vl53l1_tof_firmware_GD32.hex` - Intel HEX format for programmers

### MIK32 Platform
- `build/vl53l1_tof_firmware_MIK32.elf` - Executable firmware
- `build/vl53l1_tof_firmware_MIK32.bin` - Binary for flashing
- `build/vl53l1_tof_firmware_MIK32.hex` - Intel HEX format for programmers

**Note**: Platform-aware builds generate files with platform suffixes to prevent conflicts and make it easy to identify which platform each file was built for.

## Project Structure
```
stage2/firmware/
├── app/                # Application logic
├── drivers/            # Sensor and utility drivers
├── platform/           # Hardware abstraction layer
├── tests/              # Unit tests
├── build.bat           # Windows build script
└── Makefile            # Build configuration
```

## Toolchain Detection
The build system automatically detects toolchains in common locations:

### ARM GCC (GD32E230)
- PlatformIO toolchain
- GNU ARM Embedded toolchain
- System PATH

### RISC-V GCC (MIK32)
- PlatformIO MIK32 toolchain
- System PATH
- Custom installation paths
