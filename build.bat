@echo off

REM Create build directory
if not exist build mkdir build
cd build

REM Run cmake and make (using number of processors)
cmake ..
cmake --build . --parallel %NUMBER_OF_PROCESSORS%

echo Build complete! Run with: focusNexus.exe [file.fn] or focusNexus.exe for REPL
pause