@echo off
echo ========================================
echo VL53L1X GD32E230 Realistic Test Runner
echo ========================================
echo.

echo Building project...
call build.bat

echo.
echo ========================================
echo Build Results:
echo ========================================
echo.

if exist ".pio\build\gd32e230\vl53l1x_gd32e230.elf" (
    echo [SUCCESS] ELF file created: .pio\build\gd32e230\vl53l1x_gd32e230.elf
) else (
    echo [ERROR] ELF file not found
)

if exist ".pio\build\gd32e230\vl53l1x_gd32e230.bin" (
    echo [SUCCESS] BIN file created: .pio\build\gd32e230\vl53l1x_gd32e230.bin
) else (
    echo [ERROR] BIN file not found
)

if exist ".pio\build\gd32e230\vl53l1x_gd32e230.hex" (
    echo [SUCCESS] HEX file created: .pio\build\gd32e230\vl53l1x_gd32e230.hex
) else (
    echo [ERROR] HEX file not found
)

echo.
echo ========================================
echo File Sizes:
echo ========================================
if exist ".pio\build\gd32e230\vl53l1x_gd32e230.elf" (
    for %%I in (".pio\build\gd32e230\vl53l1x_gd32e230.elf") do echo ELF: %%~zI bytes
)
if exist ".pio\build\gd32e230\vl53l1x_gd32e230.bin" (
    for %%I in (".pio\build\gd32e230\vl53l1x_gd32e230.bin") do echo BIN: %%~zI bytes
)
if exist ".pio\build\gd32e230\vl53l1x_gd32e230.hex" (
    for %%I in (".pio\build\gd32e230\vl53l1x_gd32e230.hex") do echo HEX: %%~zI bytes
)

echo.
echo ========================================
echo Starting Realistic Simulation
echo ========================================
echo.
echo This simulator mimics the actual GD32E230 microcontroller
echo running the VL53L1X distance measurement program.
echo.
echo Features:
echo - Realistic boot sequence
echo - VL53L1X sensor initialization  
echo - Continuous distance measurements
echo - Realistic measurement noise and variation
echo - UART output at 115200 baud
echo.
echo Expected Output:
echo VL53L1X GD32E230 Implementation Ready
echo Initializing VL53L1X sensor...
echo VL53L1X initialized successfully!
echo Starting distance measurements...
echo Distance: XXXX mm
echo Distance: XXXX mm
echo ...
echo.

REM Check if Python is available
python --version >nul 2>&1
if errorlevel 1 (
    echo [ERROR] Python not found. Please install Python 3.
    echo Download from: https://www.python.org/downloads/
    pause
    exit /b 1
)

REM Check if pyserial is available
py -c "import serial" >nul 2>&1
if errorlevel 1 (
    echo [INFO] Installing pyserial library...
    pip install pyserial
    if errorlevel 1 (
        echo [ERROR] Failed to install pyserial. Please run: pip install pyserial
        pause
        exit /b 1
    )
)

REM Run the realistic simulator
echo Starting VL53L1X realistic simulator...
python vl53l1x_simulator.py

echo.
echo ========================================
echo Simulation Complete
echo ========================================
echo.
echo Next Steps:
echo 1. Flash the .bin or .hex file to GD32E230
echo 2. Connect VL53L1X sensor to I2C pins
echo 3. Connect UART to PC (115200 baud, 8N1)
echo 4. Power on the system
echo 5. Monitor UART terminal for real distance readings
echo.
echo Hardware Connections:
echo GD32E230:
echo - PA9: UART TX
echo - PA10: UART RX  
echo - PB6: I2C SDA
echo - PB7: I2C SCL
echo - VCC: 3.3V
echo - GND: Ground
echo.
echo VL53L1X:
echo - SDA: GD32E230 PB6
echo - SCL: GD32E230 PB7
echo - XSHUT: 3.3V (enable)
echo - VCC: 3.3V
echo - GND: Ground

echo.
echo Press any key to exit...
pause > nul
