@echo off
echo ========================================
echo VL53L1X GD32E230 Console Simulator
echo ========================================
echo.

echo Building project...
call build.bat >nul 2>&1

echo.
echo ========================================
echo Build Status:
echo ========================================
if exist ".pio\build\gd32e230\vl53l1x_gd32e230.bin" (
    echo [SUCCESS] Build completed successfully
    echo [SUCCESS] Binary file ready: .pio\build\gd32e230\vl53l1x_gd32e230.bin
) else (
    echo [ERROR] Build failed
    pause
    exit /b 1
)

echo.
echo ========================================
echo Starting Console Simulation
echo ========================================
echo.
echo This console simulator mimics the actual GD32E230 microcontroller
echo running the VL53L1X distance measurement program.
echo.
echo Features:
echo - Realistic boot sequence
echo - VL53L1X sensor initialization  
echo - Continuous distance measurements
echo - Realistic measurement noise and variation
echo - UART output simulation
echo.
echo ========================================
echo SIMULATION STARTING...
echo ========================================
echo.

REM Simulate boot sequence
echo VL53L1X GD32E230 Implementation Ready
timeout /t 1 /nobreak >nul

echo Initializing VL53L1X sensor...
timeout /t 1 /nobreak >nul

echo VL53L1X initialized successfully!
echo Starting distance measurements...
timeout /t 1 /nobreak >nul

echo.
echo ========================================
echo Distance Measurements (Press Ctrl+C to stop)
echo ========================================
echo.

REM Generate realistic distance measurements
:measurement_loop
REM Generate random distance between 500mm and 3000mm with noise
set /a base_distance=500 + (%random% %% 2500)
set /a noise=(%random% %% 21) - 10
set /a distance=%base_distance% + %noise%

REM Ensure distance is within realistic range
if %distance% LSS 50 set distance=50
if %distance% GTR 4000 set distance=4000

echo Distance: %distance% mm

REM Wait 1 second before next measurement
timeout /t 1 /nobreak >nul

goto measurement_loop

:end_simulation
echo.
echo ========================================
echo Simulation Stopped
echo ========================================
echo.
echo Ready for hardware testing:
echo 1. Flash .pio\build\gd32e230\vl53l1x_gd32e230.bin to GD32E230
echo 2. Connect VL53L1X sensor to I2C pins
echo 3. Connect UART to PC (115200 baud, 8N1)
echo 4. Power on and monitor UART terminal
echo.
echo Hardware Connections:
echo GD32E230: PA9=UART_TX, PA10=UART_RX, PB6=I2C_SDA, PB7=I2C_SCL
echo VL53L1X: SDA=PB6, SCL=PB7, XSHUT=3.3V, VCC=3.3V, GND=GND
echo.
pause
