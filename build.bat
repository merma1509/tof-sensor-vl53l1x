@echo off
echo ========================================
echo VL53L1X GD32E230 Universal Builder
echo ========================================
echo.

REM Auto-detect ARM GCC toolchain
echo Detecting ARM GCC toolchain...
set ARM_GCC_FOUND=0

REM Check common installation paths
for %%P in (
    "%USERPROFILE%\.platformio\packages\toolchain-gccarmnoneeabi\bin"
    "C:\Users\%USERNAME%\.platformio\packages\toolchain-gccarmnoneeabi\bin"
    "C:\Program Files (x86)\GNU Tools ARM Embedded\bin"
    "C:\Program Files\GNU Tools ARM Embedded\bin"
    "C:\devkit\arm\bin"
    "C:\gcc-arm-none-eabi\bin"
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
set BUILD_DIR=%PROJECT_DIR%.pio\build\gd32e230

echo [INFO] Project directory: %PROJECT_DIR%
echo [INFO] Build directory: %BUILD_DIR%
echo.

REM Create build directory
if not exist "%BUILD_DIR%" mkdir "%BUILD_DIR%"
echo [SUCCESS] Build directory created

REM Set compiler flags
set CFLAGS=-mcpu=cortex-m23 -mthumb -Os -g -ffunction-sections -fdata-sections -I"%PROJECT_DIR%GD32E23Firmware/CMSIS" -I"%PROJECT_DIR%GD32E23Firmware/CMSIS/GD/GD32E23x/Include" -I"%PROJECT_DIR%GD32E23Firmware/GD32E23x_standard_peripheral/Include" -I"%PROJECT_DIR%include" -I"%PROJECT_DIR%API/core" -O2 -Wall -D GD32E230 -D SYSTEM_CLOCK_48MHZ -std=c99
set INCLUDES=-I"%PROJECT_DIR%include"
set LDFLAGS=-mcpu=cortex-m23 -mthumb -mfloat-abi=soft -specs=nano.specs -Wl,--gc-sections -Wl,--print-memory-usage -Wl,--start-group -lc -lgcc -Wl,--end-group -Wl,--no-warn-rwx-segments

echo.
echo ========================================
echo Compiling Source Files
echo ========================================

REM Compile main application
echo Compiling main_vl53l1x_uart.c...
if defined ARM_GCC_PATH (
    "%ARM_GCC_PATH%\arm-none-eabi-gcc.exe" %CFLAGS% %INCLUDES% -c "%PROJECT_DIR%src\main_vl53l1x_uart.c" -o "%BUILD_DIR%\main_vl53l1x_uart.o"
) else (
    arm-none-eabi-gcc.exe %CFLAGS% %INCLUDES% -c "%PROJECT_DIR%src\main_vl53l1x_uart.c" -o "%BUILD_DIR%\main_vl53l1x_uart.o"
)

if not exist "%BUILD_DIR%\main_vl53l1x_uart.o" (
    echo [ERROR] Failed to compile main_vl53l1x_uart.c
    pause
    exit /b 1
)
echo [SUCCESS] main_vl53l1x_uart.c compiled

REM Compile syscalls
echo Compiling syscalls.c...
if defined ARM_GCC_PATH (
    "%ARM_GCC_PATH%\arm-none-eabi-gcc.exe" %CFLAGS% %INCLUDES% -c "%PROJECT_DIR%src\syscalls.c" -o "%BUILD_DIR%\syscalls.o"
) else (
    arm-none-eabi-gcc.exe %CFLAGS% %INCLUDES% -c "%PROJECT_DIR%src\syscalls.c" -o "%BUILD_DIR%\syscalls.o"
)

if not exist "%BUILD_DIR%\syscalls.o" (
    echo [ERROR] Failed to compile syscalls.c
    pause
    exit /b 1
)
echo [SUCCESS] syscalls.c compiled

REM Compile uart_commands
echo Compiling uart_commands.c...
if defined ARM_GCC_PATH (
    "%ARM_GCC_PATH%\arm-none-eabi-gcc.exe" %CFLAGS% %INCLUDES% -c "%PROJECT_DIR%src\uart_commands.c" -o "%BUILD_DIR%\uart_commands.o"
) else (
    arm-none-eabi-gcc.exe %CFLAGS% %INCLUDES% -c "%PROJECT_DIR%src\uart_commands.c" -o "%BUILD_DIR%\uart_commands.o"
)

if not exist "%BUILD_DIR%\uart_commands.o" (
    echo [ERROR] Failed to compile uart_commands.c
    pause
    exit /b 1
)
echo [SUCCESS] uart_commands.c compiled

REM Compile startup file
echo Compiling startup_gd32e23x.c...
if defined ARM_GCC_PATH (
    "%ARM_GCC_PATH%\arm-none-eabi-gcc.exe" %CFLAGS% %INCLUDES% -c "%PROJECT_DIR%src\startup_gd32e23x.c" -o "%BUILD_DIR%\startup_gd32e23x.o"
) else (
    arm-none-eabi-gcc.exe %CFLAGS% %INCLUDES% -c "%PROJECT_DIR%src\startup_gd32e23x.c" -o "%BUILD_DIR%\startup_gd32e23x.o"
)
if not exist "%BUILD_DIR%\startup_gd32e23x.o" (
    echo [ERROR] Failed to compile startup_gd32e23x.c
    pause
    exit /b 1
)
echo [SUCCESS] startup_gd32e23x.c compiled

REM Compile uart_init
echo Compiling uart_init.c...
if defined ARM_GCC_PATH (
    "%ARM_GCC_PATH%\arm-none-eabi-gcc.exe" %CFLAGS% %INCLUDES% -c "%PROJECT_DIR%src\uart_init.c" -o "%BUILD_DIR%\uart_init.o"
) else (
    arm-none-eabi-gcc.exe %CFLAGS% %INCLUDES% -c "%PROJECT_DIR%src\uart_init.c" -o "%BUILD_DIR%\uart_init.o"
)

if not exist "%BUILD_DIR%\uart_init.o" (
    echo [ERROR] Failed to compile uart_init.c
    pause
    exit /b 1
)
echo [SUCCESS] uart_init.c compiled

REM Compile sdk_integration
echo Compiling sdk_integration.c...
if defined ARM_GCC_PATH (
    "%ARM_GCC_PATH%\arm-none-eabi-gcc.exe" %CFLAGS% %INCLUDES% -c "%PROJECT_DIR%src\sdk_integration.c" -o "%BUILD_DIR%\sdk_integration.o"
) else (
    arm-none-eabi-gcc.exe %CFLAGS% %INCLUDES% -c "%PROJECT_DIR%src\sdk_integration.c" -o "%BUILD_DIR%\sdk_integration.o"
)

if not exist "%BUILD_DIR%\sdk_integration.o" (
    echo [ERROR] Failed to compile sdk_integration.c
    pause
    exit /b 1
)
echo [SUCCESS] sdk_integration.c compiled

REM Compile filter module
echo Compiling filter.c...
if defined ARM_GCC_PATH (
    "%ARM_GCC_PATH%\arm-none-eabi-gcc.exe" %CFLAGS% %INCLUDES% -c "%PROJECT_DIR%src\filter.c" -o "%BUILD_DIR%\filter.o"
) else (
    arm-none-eabi-gcc.exe %CFLAGS% %INCLUDES% -c "%PROJECT_DIR%src\filter.c" -o "%BUILD_DIR%\filter.o"
)

if not exist "%BUILD_DIR%\filter.o" (
    echo [ERROR] Failed to compile filter.c
    pause
    exit /b 1
)
echo [SUCCESS] filter.c compiled

echo.
echo ========================================
echo Linking
echo ========================================

REM Define object files
set OBJECT_FILES=%BUILD_DIR%\startup_gd32e23x.o %BUILD_DIR%\main_vl53l1x_uart.o %BUILD_DIR%\uart_commands.o %BUILD_DIR%\uart_init.o %BUILD_DIR%\i2c_driver.o %BUILD_DIR%\sdk_integration.o %BUILD_DIR%\syscalls.o %BUILD_DIR%\filter.o

REM Link
echo Linking objects: %OBJECT_FILES%
if defined ARM_GCC_PATH (
    "%ARM_GCC_PATH%\arm-none-eabi-gcc.exe" %LDFLAGS% %OBJECT_FILES% -o "%BUILD_DIR%\vl53l1x_gd32e230.elf"
) else (
    arm-none-eabi-gcc.exe %LDFLAGS% %OBJECT_FILES% -o "%BUILD_DIR%\vl53l1x_gd32e230.elf"
)

if not exist "%BUILD_DIR%\vl53l1x_gd32e230.elf" (
    echo [ERROR] Failed to link executable
    pause
    exit /b 1
)
echo [SUCCESS] Linking completed

echo.
echo ========================================
echo Creating Output Files
echo ========================================

REM Create binary
echo Creating binary file...
if defined ARM_GCC_PATH (
    "%ARM_GCC_PATH%\arm-none-eabi-objcopy.exe" -O binary "%BUILD_DIR%\vl53l1x_gd32e230.elf" "%BUILD_DIR%\vl53l1x_gd32e230.bin"
) else (
    arm-none-eabi-objcopy.exe -O binary "%BUILD_DIR%\vl53l1x_gd32e230.elf" "%BUILD_DIR%\vl53l1x_gd32e230.bin"
)

REM Create hex file
echo Creating hex file...
if defined ARM_GCC_PATH (
    "%ARM_GCC_PATH%\arm-none-eabi-objcopy.exe" -O ihex "%BUILD_DIR%\vl53l1x_gd32e230.elf" "%BUILD_DIR%\vl53l1x_gd32e230.hex"
) else (
    arm-none-eabi-objcopy.exe -O ihex "%BUILD_DIR%\vl53l1x_gd32e230.elf" "%BUILD_DIR%\vl53l1x_gd32e230.hex"
)

echo.
echo ========================================
echo Build Results
echo ========================================

REM Check output files
if exist "%BUILD_DIR%\vl53l1x_gd32e230.elf" (
    for %%I in ("%BUILD_DIR%\vl53l1x_gd32e230.elf") do echo [SUCCESS] ELF: %%~zI bytes
) else (
    echo [ERROR] ELF file not created
)

if exist "%BUILD_DIR%\vl53l1x_gd32e230.bin" (
    for %%I in ("%BUILD_DIR%\vl53l1x_gd32e230.bin") do echo [SUCCESS] BIN: %%~zI bytes
) else (
    echo [ERROR] BIN file not created
)

if exist "%BUILD_DIR%\vl53l1x_gd32e230.hex" (
    for %%I in ("%BUILD_DIR%\vl53l1x_gd32e230.hex") do echo [SUCCESS] HEX: %%~zI bytes
) else (
    echo [ERROR] HEX file not created
)

echo.
echo ========================================
echo Output Files Ready
echo ========================================
echo.
echo Files created in: %BUILD_DIR%
echo.
echo - vl53l1x_gd32e230.elf (for debugging)
echo - vl53l1x_gd32e230.bin (for flashing)
echo - vl53l1x_gd32e230.hex (for programmers)
echo.
echo Ready for hardware testing!
echo.
pause
