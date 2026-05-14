@echo off
REM ====================================
REM   Rebuild AlgoMan (if needed)
REM ====================================
echo.
echo Rebuilding AlgoMan Console...
echo.

g++ -std=c++17 src\main_console.cpp -o AlgoMan_Console.exe -static

if %ERRORLEVEL% EQU 0 (
    echo.
    echo ✅ Game build successful!
    echo.
    echo Files created:
    echo   - AlgoMan_Console.exe
    echo.
) else (
    echo.
    echo ❌ Game build failed!
    echo Make sure MinGW is in your PATH
    echo.
    pause
)
