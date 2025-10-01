@echo off
setlocal enabledelayedexpansion

REM Focus Nexus Build Script with Library Support (Windows)
REM This script builds Focus Nexus with full library integration support

echo Focus Nexus Build Script with Library Support
echo ==============================================

REM Configuration
set BUILD_TYPE=%1
if "%BUILD_TYPE%"=="" set BUILD_TYPE=Release
if "%BUILD_TYPE%"=="clean" (
    echo Cleaning previous build...
    rmdir /s /q build 2>nul
    set BUILD_TYPE=%2
    if "!BUILD_TYPE!"=="" set BUILD_TYPE=Release
)

set ENABLE_PYTHON=%ENABLE_PYTHON%
if "%ENABLE_PYTHON%"=="" set ENABLE_PYTHON=ON

set ENABLE_JAVA=%ENABLE_JAVA%
if "%ENABLE_JAVA%"=="" set ENABLE_JAVA=ON

set ENABLE_PLUGINS=%ENABLE_PLUGINS%
if "%ENABLE_PLUGINS%"=="" set ENABLE_PLUGINS=ON

echo Build Configuration:
echo   Build Type: %BUILD_TYPE%
echo   Python Support: %ENABLE_PYTHON%
echo   Java Support: %ENABLE_JAVA%
echo   Plugin Support: %ENABLE_PLUGINS%

REM Check dependencies
echo.
echo Checking dependencies...

REM Check for Python
if "%ENABLE_PYTHON%"=="ON" (
    python --version >nul 2>&1
    if errorlevel 1 (
        echo Python not found
        echo Install Python from python.org
        set ENABLE_PYTHON=OFF
    ) else (
        echo Python found
        python -c "import sys; print('  Python version:', sys.version)"
    )
)

REM Check for Java
if "%ENABLE_JAVA%"=="ON" (
    if "%JAVA_HOME%"=="" (
        echo JAVA_HOME not set
        echo Set JAVA_HOME to JDK installation directory
        set ENABLE_JAVA=OFF
    ) else (
        if exist "%JAVA_HOME%\bin\javac.exe" (
            echo Java development kit found at %JAVA_HOME%
        ) else (
            echo Java development kit not found at %JAVA_HOME%
            set ENABLE_JAVA=OFF
        )
    )
)

REM Create build directory
echo.
echo Creating build directory...
if not exist build mkdir build
cd build

REM Configure with CMake
echo.
echo Configuring with CMake...
cmake .. ^
    -G "Visual Studio 16 2019" ^
    -A x64 ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DPYTHON_SUPPORT=%ENABLE_PYTHON% ^
    -DJNI_SUPPORT=%ENABLE_JAVA% ^
    -DCUSTOM_PLUGIN_SUPPORT=%ENABLE_PLUGINS%

if errorlevel 1 (
    echo CMake configuration failed!
    exit /b 1
)

REM Build
echo.
echo Building Focus Nexus...
cmake --build . --config %BUILD_TYPE% --parallel %NUMBER_OF_PROCESSORS%

if errorlevel 1 (
    echo Build failed!
    exit /b 1
)

echo.
echo Build completed successfully!
echo.
echo Executable: build\%BUILD_TYPE%\focusNexus.exe
echo.

REM Build example libraries
echo Building example libraries...

REM Build C++ example library
if exist "..\examples\cpp_library\math_lib.cpp" (
    echo Building C++ math library...
    cd ..\examples\cpp_library
    g++ -shared -o math.dll math_lib.cpp 2>nul
    if errorlevel 1 (
        cl /LD math_lib.cpp /Fe:math.dll >nul 2>&1
        if errorlevel 1 (
            echo Failed to build C++ library
        ) else (
            echo C++ library built: math.dll
        )
    ) else (
        echo C++ library built: math.dll
    )
    cd ..\..\build
)

REM Build Java example library
if exist "..\examples\java_library\AdvancedMathUtils.java" (
    echo Building Java library...
    cd ..\examples\java_library
    javac AdvancedMathUtils.java >nul 2>&1
    if errorlevel 1 (
        echo Failed to compile Java library
    ) else (
        echo Java class compiled: AdvancedMathUtils.class
        jar cf advanced-math.jar AdvancedMathUtils.class >nul 2>&1
        if errorlevel 1 (
            echo Failed to create JAR file
        ) else (
            echo Java JAR created: advanced-math.jar
        )
    )
    cd ..\..\build
)

REM Build custom plugin
if exist "..\examples\custom_plugin\comprehensive_plugin.cpp" (
    echo Building custom plugin...
    cd ..\examples\custom_plugin
    g++ -shared -o comprehensive_plugin.dll comprehensive_plugin.cpp -I..\..\src 2>nul
    if errorlevel 1 (
        cl /LD comprehensive_plugin.cpp /I..\..\src /Fe:comprehensive_plugin.dll >nul 2>&1
        if errorlevel 1 (
            echo Failed to build custom plugin
        ) else (
            echo Custom plugin built: comprehensive_plugin.dll
        )
    ) else (
        echo Custom plugin built: comprehensive_plugin.dll
    )
    cd ..\..\build
)

REM Set up environment
echo.
echo Setting up environment...

set PATH=%PATH%;..\examples\cpp_library;..\examples\custom_plugin
set PYTHONPATH=%PYTHONPATH%;..\examples\python_library
set CLASSPATH=%CLASSPATH%;..\examples\java_library

echo Environment variables set:
echo   PATH includes example directories
echo   PYTHONPATH includes Python library directory
echo   CLASSPATH includes Java library directory

REM Run tests if available
if exist "..\tests\library_integration\test_all_libraries.fn" (
    echo.
    echo Running library integration tests...
    %BUILD_TYPE%\focusNexus.exe ..\tests\library_integration\test_all_libraries.fn
    
    if errorlevel 1 (
        echo Some tests failed
    ) else (
        echo All tests passed!
    )
)

echo.
echo 🎉 Focus Nexus with library support is ready!
echo.
echo Usage examples:
echo   %BUILD_TYPE%\focusNexus.exe                                    # Interactive mode
echo   %BUILD_TYPE%\focusNexus.exe script.fn                         # Run script
echo   %BUILD_TYPE%\focusNexus.exe ..\tests\library_integration\test_cpp_library.fn
echo   %BUILD_TYPE%\focusNexus.exe ..\tests\library_integration\test_python_library.fn
echo   %BUILD_TYPE%\focusNexus.exe ..\tests\library_integration\test_java_library.fn
echo   %BUILD_TYPE%\focusNexus.exe ..\tests\library_integration\test_custom_plugin.fn
echo.
echo Example libraries available:
echo   C++: ..\examples\cpp_library\math.dll
echo   Python: ..\examples\python_library\advanced_math.py
echo   Java: ..\examples\java_library\AdvancedMathUtils.class
echo   Plugin: ..\examples\custom_plugin\comprehensive_plugin.dll

pause