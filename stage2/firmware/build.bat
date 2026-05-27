@echo off
echo ========================================
echo VL53L1X Stage2 Firmware Universal Builder
echo ========================================
echo.

REM Auto-detect ARM GCC toolchain
echo Detecting ARM GCC toolchain...
set ARM_GCC_FOUND=0

REM Check common installation paths
for %%P in (
    "%USERPROFILE%\.platformio\packages\toolchain-gccarmnoneeabi\bin"
    "C:\Users\%USERNAME%\.platformio\packages\toolchain-gccarmnoneeabi\bin"
    "C:\Users\aimem\.platformio\packages\toolchain-gccarmnoneeabi\bin"
    "C:\Program Files (x86)\GNU Tools ARM Embedded\bin"
    "C:\Program Files\GNU Tools ARM Embedded\bin"
    "C:\devkit\arm\bin"
    "C:\gcc-arm-none-eabi\bin"
    "C:\Users\aimem\Desktop\clear-space-ru\arm-gcc\bin"
) do (
    if exist "%%P\arm-none-eabi-gcc.exe" (
        set ARM_GCC_PATH=%%P
        set ARM_GCC_FOUND=1
        echo [FOUND] ARM GCC at: %%P
        goto :found_toolchain
    )
)

REM Check if toolchain is in PATH
where arm-none-eabi-gcc.exe >nul 2>&1
if not errorlevel 1 (
    set ARM_GCC_PATH=
    set ARM_GCC_FOUND=1
    echo [FOUND] ARM GCC in PATH
    goto :found_toolchain
)

:found_toolchain
if %ARM_GCC_FOUND%==0 (
    echo [ERROR] ARM GCC toolchain not found!
    echo.
    echo Please install one of the following:
    echo 1. PlatformIO: pio install toolchain-gccarmnoneeabi
    echo 2. GNU ARM Embedded: https://developer.arm.com/tools-and-software/open-source-software/developer-tools/gnu-toolchain/gnu-rm/downloads
    echo 3. Add ARM GCC to system PATH
    echo.
    pause
    exit /b 1
)

REM Set project directory (relative to this batch file)
set PROJECT_DIR=%~dp0
echo [INFO] Project directory: %PROJECT_DIR%
echo.

REM Add ARM GCC to PATH if found in specific location
if defined ARM_GCC_PATH (
    echo [INFO] Adding ARM GCC to PATH: %ARM_GCC_PATH%
    set "PATH=%ARM_GCC_PATH%;%PATH%"
)

echo.
echo ========================================
echo Building VL53L1X Stage2 Firmware
echo ========================================
echo.

REM Run make to build the project
echo [INFO] Running make...
if defined ARM_GCC_PATH (
    cd /d "%PROJECT_DIR%"
    make
) else (
    cd /d "%PROJECT_DIR%"
    make
)

if not exist "%PROJECT_DIR%build\vl53l1_tof_firmware.elf" (
    echo [ERROR] Build failed - ELF file not created
    echo.
    echo Troubleshooting:
    echo 1. Check that all source files are present
    echo 2. Verify Makefile configuration
    echo 3. Check for compilation errors above
    echo.
    pause
    exit /b 1
)

echo.
echo ========================================
echo Build Results
echo ========================================

REM Check output files
if exist "%PROJECT_DIR%build\vl53l1_tof_firmware.elf" (
    for %%I in ("%PROJECT_DIR%build\vl53l1_tof_firmware.elf") do echo [SUCCESS] ELF: %%~zI bytes
) else (
    echo [ERROR] ELF file not created
)

if exist "%PROJECT_DIR%build\vl53l1_tof_firmware.bin" (
    for %%I in ("%PROJECT_DIR%build\vl53l1_tof_firmware.bin") do echo [SUCCESS] BIN: %%~zI bytes
) else (
    echo [ERROR] BIN file not created
)

echo.
echo ========================================
echo Build Complete
echo ========================================
echo.
echo Files created in: %PROJECT_DIR%build
echo.
echo - vl53l1_tof_firmware.elf (for debugging)
echo - vl53l1_tof_firmware.bin (for flashing)
echo.
echo Ready for hardware testing!
echo.
pause
