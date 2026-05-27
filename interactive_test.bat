@echo off
setlocal enabledelayedexpansion

echo ========================================
echo VL53L1X Interactive Command Tester
echo ========================================
echo.
echo This simulates the GD32E230 UART interface
echo without requiring hardware.
echo.
echo Type commands exactly as you would to the real device.
echo Type 'exit' to quit.
echo Type 'stop' to stop continuous distance readings.
echo.
echo NOTE: In continuous mode, press Ctrl+C then type 'stop' to exit streaming.

set FILTER_STATE=ON
set MEASURING=OFF
set MODE=Short
set DISTANCE=1234

:loop
echo.
set /p CMD="GD32E230^> "

if /i "!CMD!"=="exit" goto :end
if /i "!CMD!"=="quit" goto :end
if /i "!CMD!"=="help" goto :help
if /i "!CMD!"=="start" goto :start
if /i "!CMD!"=="stop" goto :stop
if /i "!CMD!"=="status" goto :status
if /i "!CMD!"=="cal" goto :cal
if /i "!CMD!"=="calibrate" goto :cal
if /i "!CMD!"=="short" goto :short
if /i "!CMD!"=="med" goto :med
if /i "!CMD!"=="medium" goto :med
if /i "!CMD!"=="long" goto :long
if /i "!CMD!"=="filter on" goto :filter_on
if /i "!CMD!"=="filter off" goto :filter_off
if /i "!CMD!"=="filter status" goto :filter_status

echo Unknown command: !CMD!
echo Type 'help' for available commands.
goto :loop

:help
echo.
echo === Available Commands ===
echo   START           - Begin continuous distance measurements
echo   STOP            - Stop distance measurements
echo   CAL             - Calibrate sensor
echo   SHORT           - Set short-range mode (15ms)
echo   MED             - Set medium-range mode (50ms)
echo   LONG            - Set long-range mode (100ms)
echo   STATUS          - Show current system status
echo   FILTER ON       - Enable distance filter
echo   FILTER OFF      - Disable distance filter (raw data)
echo   FILTER STATUS   - Show filter state
echo   HELP            - Show this help
echo   EXIT            - Quit tester
echo.
echo === Continuous Mode ===
echo When you type START, distance readings will stream every second.
echo Type STOP to return to command mode.
echo Distance is shown in mm and meters (e.g., 1234 mm = 1.23 m)
echo.
goto :loop

:start
if "!MEASURING!"=="ON" (
    echo BUSY - Already measuring
    goto :loop
)
set MEASURING=ON
echo OK - Ranging started
echo [Streaming distance data - type STOP to exit]
echo.

:stream_loop
if "!MEASURING!"=="OFF" goto :loop

REM Calculate meters from mm
set /a METERS=!DISTANCE! / 1000
set /a CM=!DISTANCE! %% 1000 / 10

REM Add small random variation to simulate real sensor
set /a VAR=!RANDOM! %% 20 - 10
set /a DISTANCE=!DISTANCE! + !VAR!

REM Keep within realistic bounds for mode
if "!MODE!"=="Short" (
    if !DISTANCE! LSS 100 set DISTANCE=100
    if !DISTANCE! GTR 1200 set DISTANCE=1200
)
if "!MODE!"=="Medium" (
    if !DISTANCE! LSS 800 set DISTANCE=800
    if !DISTANCE! GTR 2500 set DISTANCE=2500
)
if "!MODE!"=="Long" (
    if !DISTANCE! LSS 2000 set DISTANCE=2000
    if !DISTANCE! GTR 4500 set DISTANCE=4500
)

REM Recalculate after variation
set /a METERS=!DISTANCE! / 1000
set /a CM=!DISTANCE! %% 1000 / 10

REM Show distance with filter indicator
echo | set /p="Distance: !DISTANCE! mm"
echo | set /p=" ^(=!METERS!.!CM:~0,2! m^)"
if "!FILTER_STATE!"=="ON" (
    echo  [FILTERED]
) else (
    echo  [RAW]
)

REM Wait 1 second between readings
ping -n 2 127.0.0.1 >nul

goto :stream_loop

:stop
if "!MEASURING!"=="OFF" (
    echo READY - Not currently measuring
) else (
    set MEASURING=OFF
    echo OK - Ranging stopped
)
goto :loop

:status
echo ========================================
echo Status Report
echo ========================================
echo State: !MEASURING!
echo Mode: !MODE!
echo Filter: !FILTER_STATE!
if "!MEASURING!"=="ON" (
    set /a METERS=!DISTANCE! / 1000
    set /a CM=!DISTANCE! %% 1000 / 10
    echo Current: !DISTANCE! mm ^(=!METERS!.!CM:~0,2! m^)
)
echo ========================================
goto :loop

:cal
echo Calibrating sensor...
timeout /t 2 /nobreak >nul
echo OK - Calibration complete
echo   Offset corrected: +3mm
echo   Crosstalk compensated
goto :loop

:short
set MODE=Short
set DISTANCE=456
echo OK - Short range mode (15ms timing)
echo   Range: 10cm - 1.2m
goto :loop

:med
set MODE=Medium
set DISTANCE=1500
echo OK - Medium range mode (50ms timing)
echo   Range: 0.8m - 2.5m
goto :loop

:long
set MODE=Long
set DISTANCE=3200
echo OK - Long range mode (100ms timing)
echo   Range: 2.0m - 4.0m
goto :loop

:filter_on
if "!FILTER_STATE!"=="ON" (
    echo READY - Filter already enabled
) else (
    set FILTER_STATE=ON
    echo OK - Filter enabled
echo   Median(5) + Average(4) active
)
goto :loop

:filter_off
if "!FILTER_STATE!"=="OFF" (
    echo READY - Filter already disabled
) else (
    set FILTER_STATE=OFF
    echo OK - Filter disabled
    echo   Raw sensor data mode
)
goto :loop

:filter_status
echo ========================================
echo Filter Status
echo ========================================
echo State: !FILTER_STATE!
if "!FILTER_STATE!"=="ON" (
    echo Type: Combined Median + Moving Average
    echo   Median window: 5 samples
    echo   Average window: 4 samples
    echo Effect: Removes outliers, smooths output
) else (
    echo Type: RAW (unfiltered)
    echo   Direct sensor readings
    echo Effect: Real-time, may show noise
)
echo ========================================
goto :loop

:end
echo.
echo Exiting interactive tester...
echo.
exit /b 0
