# Focus Nexus Compilation Guide

This guide provides detailed instructions for compiling Focus Nexus with library integration support on both Linux and Windows platforms.

## Table of Contents

1. [Prerequisites](#prerequisites)
2. [Linux Compilation](#linux-compilation)
3. [Windows Compilation](#windows-compilation)
4. [Build Configuration Options](#build-configuration-options)
5. [Library Dependencies](#library-dependencies)
6. [Troubleshooting](#troubleshooting)
7. [Cross-Platform Build Scripts](#cross-platform-build-scripts)

## Prerequisites

### Common Requirements

- **CMake 3.16 or higher**
- **C++17 compatible compiler**
- **Git** (for cloning the repository)

### Linux Requirements

```bash
# Ubuntu/Debian
sudo apt-get update
sudo apt-get install build-essential cmake git

# CentOS/RHEL/Fedora
sudo yum install gcc-c++ cmake git
# or for newer versions:
sudo dnf install gcc-c++ cmake git

# Arch Linux
sudo pacman -S base-devel cmake git
```

### Windows Requirements

- **Visual Studio 2019 or later** (Community Edition is sufficient)
- **CMake 3.16+** (can be installed via Visual Studio Installer)
- **Git for Windows**

## Linux Compilation

### Basic Compilation

```bash
# Clone the repository
git clone https://github.com/your-repo/focus-nexus.git
cd focus-nexus

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
make -j$(nproc)

# The executable will be in build/focusNexus
```

### With Library Support

#### Python Integration

```bash
# Install Python development headers
sudo apt-get install python3-dev python3-pip

# Configure with Python support
cmake .. -DCMAKE_BUILD_TYPE=Release -DPYTHON_SUPPORT=ON

# Build
make -j$(nproc)
```

#### Java Integration

```bash
# Install JDK
sudo apt-get install openjdk-11-jdk

# Set JAVA_HOME if not already set
export JAVA_HOME=/usr/lib/jvm/java-11-openjdk-amd64

# Configure with Java support
cmake .. -DCMAKE_BUILD_TYPE=Release -DJNI_SUPPORT=ON

# Build
make -j$(nproc)
```

#### Full Feature Build

```bash
# Install all dependencies
sudo apt-get install python3-dev openjdk-11-jdk

# Configure with all features
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DPYTHON_SUPPORT=ON \
    -DJNI_SUPPORT=ON \
    -DCUSTOM_PLUGIN_SUPPORT=ON

# Build
make -j$(nproc)
```

### Installation

```bash
# Install system-wide (optional)
sudo make install

# Or create a local installation
make install DESTDIR=/path/to/local/install
```

### Creating Distribution Package

```bash
# Create a distributable package
make package

# This creates:
# - focusNexus-1.0.0-Linux.tar.gz
# - focusNexus-1.0.0-Linux.deb (if CPack DEB generator is available)
# - focusNexus-1.0.0-Linux.rpm (if CPack RPM generator is available)
```

## Windows Compilation

### Using Visual Studio

1. **Open Visual Studio 2019/2022**
2. **Clone Repository**:
   - File → Clone Repository
   - Repository URL: `https://github.com/your-repo/focus-nexus.git`

3. **Configure CMake**:
   - Visual Studio should automatically detect CMakeLists.txt
   - Configure the project (this may take a few minutes)

4. **Build**:
   - Build → Build All
   - Or press `Ctrl+Shift+B`

### Using Command Line

```cmd
REM Clone the repository
git clone https://github.com/your-repo/focus-nexus.git
cd focus-nexus

REM Create build directory
mkdir build
cd build

REM Configure with CMake (Visual Studio 2019)
cmake .. -G "Visual Studio 16 2019" -A x64

REM Build
cmake --build . --config Release

REM The executable will be in build\Release\focusNexus.exe
```

### With Library Support

#### Python Integration

```cmd
REM Install Python 3.x from python.org with development libraries

REM Configure with Python support
cmake .. -G "Visual Studio 16 2019" -A x64 -DPYTHON_SUPPORT=ON

REM Build
cmake --build . --config Release
```

#### Java Integration

```cmd
REM Install JDK 11 or later

REM Set JAVA_HOME
set JAVA_HOME=C:\Program Files\Java\jdk-11.0.x

REM Configure with Java support
cmake .. -G "Visual Studio 16 2019" -A x64 -DJNI_SUPPORT=ON

REM Build
cmake --build . --config Release
```

#### Full Feature Build

```cmd
REM Configure with all features
cmake .. ^
    -G "Visual Studio 16 2019" ^
    -A x64 ^
    -DPYTHON_SUPPORT=ON ^
    -DJNI_SUPPORT=ON ^
    -DCUSTOM_PLUGIN_SUPPORT=ON

REM Build
cmake --build . --config Release
```

### Using the Batch Script

```cmd
REM Use the provided build script
build.bat

REM Or for a clean build
build.bat clean
```

## Build Configuration Options

### CMake Options

| Option | Default | Description |
|--------|---------|-------------|
| `CMAKE_BUILD_TYPE` | `Debug` | Build type: Debug, Release, RelWithDebInfo, MinSizeRel |
| `PYTHON_SUPPORT` | `OFF` | Enable Python library integration |
| `JNI_SUPPORT` | `OFF` | Enable Java library integration |
| `CUSTOM_PLUGIN_SUPPORT` | `ON` | Enable custom plugin system |
| `BUILD_EXAMPLES` | `ON` | Build example programs |
| `BUILD_TESTS` | `OFF` | Build unit tests |
| `BUILD_DOCUMENTATION` | `OFF` | Build documentation |

### Example Configurations

#### Development Build
```bash
cmake .. \
    -DCMAKE_BUILD_TYPE=Debug \
    -DBUILD_TESTS=ON \
    -DBUILD_EXAMPLES=ON
```

#### Production Build
```bash
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DPYTHON_SUPPORT=ON \
    -DJNI_SUPPORT=ON \
    -DBUILD_EXAMPLES=OFF \
    -DBUILD_TESTS=OFF
```

#### Minimal Build
```bash
cmake .. \
    -DCMAKE_BUILD_TYPE=MinSizeRel \
    -DPYTHON_SUPPORT=OFF \
    -DJNI_SUPPORT=OFF \
    -DCUSTOM_PLUGIN_SUPPORT=OFF \
    -DBUILD_EXAMPLES=OFF
```

## Library Dependencies

### Dynamic Library Loading

**Linux:**
- `libdl` (usually included with glibc)

**Windows:**
- `kernel32.lib` (included with Windows SDK)

### Python Integration

**Linux:**
```bash
# Find Python installation
python3-config --includes
python3-config --libs

# Typical locations:
# Headers: /usr/include/python3.x
# Libraries: /usr/lib/python3.x
```

**Windows:**
```cmd
REM Python installation should include:
REM Headers: C:\Python3x\include
REM Libraries: C:\Python3x\libs\python3x.lib
```

### Java Integration

**Linux:**
```bash
# Find JDK installation
echo $JAVA_HOME
find /usr -name "jni.h" 2>/dev/null

# Typical locations:
# Headers: $JAVA_HOME/include
# Libraries: $JAVA_HOME/lib
```

**Windows:**
```cmd
REM JDK installation should include:
REM Headers: %JAVA_HOME%\include
REM Libraries: %JAVA_HOME%\lib
```

### Verifying Dependencies

```bash
# Linux: Check shared library dependencies
ldd build/focusNexus

# Windows: Check DLL dependencies (using Dependency Walker or similar)
# Or use PowerShell:
Get-Command build\Release\focusNexus.exe | Select-Object -ExpandProperty FileVersionInfo
```

## Troubleshooting

### Common Issues

#### CMake Configuration Fails

**Problem:** CMake cannot find required libraries

**Solution:**
```bash
# Linux: Install development packages
sudo apt-get install python3-dev openjdk-11-jdk

# Set environment variables if needed
export PYTHON_ROOT=/usr
export JAVA_HOME=/usr/lib/jvm/java-11-openjdk-amd64

# Windows: Ensure paths are correct
set PYTHON_ROOT=C:\Python39
set JAVA_HOME=C:\Program Files\Java\jdk-11.0.x
```

#### Compilation Errors

**Problem:** C++17 features not supported

**Solution:**
```bash
# Check compiler version
gcc --version  # Should be 7.0 or higher
clang --version  # Should be 5.0 or higher

# Update compiler if necessary
sudo apt-get install gcc-9 g++-9
```

**Problem:** Missing headers

**Solution:**
```bash
# Install missing development packages
sudo apt-get install build-essential
sudo apt-get install python3-dev
sudo apt-get install openjdk-11-jdk
```

#### Linking Errors

**Problem:** Cannot find shared libraries at runtime

**Solution:**
```bash
# Linux: Add to library path
export LD_LIBRARY_PATH=/usr/local/lib:$LD_LIBRARY_PATH

# Or install libraries system-wide
sudo ldconfig

# Windows: Ensure DLLs are in PATH or same directory as executable
```

#### Python Integration Issues

**Problem:** Python module import fails

**Solution:**
```bash
# Check Python installation
python3 -c "import sys; print(sys.path)"

# Set PYTHONPATH if needed
export PYTHONPATH=/path/to/your/modules:$PYTHONPATH
```

#### Java Integration Issues

**Problem:** JVM initialization fails

**Solution:**
```bash
# Check Java installation
java -version
javac -version

# Ensure JAVA_HOME is set correctly
export JAVA_HOME=$(readlink -f /usr/bin/java | sed "s:bin/java::")
```

### Debug Build

For troubleshooting, build with debug information:

```bash
cmake .. -DCMAKE_BUILD_TYPE=Debug
make -j$(nproc)

# Run with debugger
gdb ./focusNexus
```

### Verbose Build

To see detailed compilation commands:

```bash
make VERBOSE=1
# or
cmake --build . --verbose
```

## Cross-Platform Build Scripts

### Universal Build Script

**build.sh** (Linux/macOS):
```bash
#!/bin/bash

set -e

# Configuration
BUILD_TYPE=${1:-Release}
ENABLE_PYTHON=${ENABLE_PYTHON:-ON}
ENABLE_JAVA=${ENABLE_JAVA:-ON}
BUILD_DIR="build"

echo "Building Focus Nexus..."
echo "Build Type: $BUILD_TYPE"
echo "Python Support: $ENABLE_PYTHON"
echo "Java Support: $ENABLE_JAVA"

# Clean previous build if requested
if [ "$1" = "clean" ]; then
    echo "Cleaning previous build..."
    rm -rf $BUILD_DIR
    shift
    BUILD_TYPE=${1:-Release}
fi

# Create build directory
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# Configure
cmake .. \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DPYTHON_SUPPORT=$ENABLE_PYTHON \
    -DJNI_SUPPORT=$ENABLE_JAVA \
    -DCUSTOM_PLUGIN_SUPPORT=ON

# Build
make -j$(nproc)

echo "Build complete!"
echo "Executable: $BUILD_DIR/focusNexus"

# Run tests if available
if [ -f "test_runner" ]; then
    echo "Running tests..."
    ./test_runner
fi
```

**build.bat** (Windows):
```batch
@echo off
setlocal enabledelayedexpansion

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

echo Building Focus Nexus...
echo Build Type: %BUILD_TYPE%
echo Python Support: %ENABLE_PYTHON%
echo Java Support: %ENABLE_JAVA%

REM Create build directory
if not exist build mkdir build
cd build

REM Configure
cmake .. ^
    -G "Visual Studio 16 2019" ^
    -A x64 ^
    -DCMAKE_BUILD_TYPE=%BUILD_TYPE% ^
    -DPYTHON_SUPPORT=%ENABLE_PYTHON% ^
    -DJNI_SUPPORT=%ENABLE_JAVA% ^
    -DCUSTOM_PLUGIN_SUPPORT=ON

if errorlevel 1 (
    echo CMake configuration failed!
    exit /b 1
)

REM Build
cmake --build . --config %BUILD_TYPE% --parallel %NUMBER_OF_PROCESSORS%

if errorlevel 1 (
    echo Build failed!
    exit /b 1
)

echo Build complete!
echo Executable: build\%BUILD_TYPE%\focusNexus.exe

REM Run tests if available
if exist "%BUILD_TYPE%\test_runner.exe" (
    echo Running tests...
    "%BUILD_TYPE%\test_runner.exe"
)

pause
```

### Docker Build

**Dockerfile**:
```dockerfile
FROM ubuntu:20.04

# Install dependencies
RUN apt-get update && apt-get install -y \
    build-essential \
    cmake \
    git \
    python3-dev \
    openjdk-11-jdk \
    && rm -rf /var/lib/apt/lists/*

# Set working directory
WORKDIR /app

# Copy source code
COPY . .

# Build
RUN mkdir build && cd build && \
    cmake .. -DCMAKE_BUILD_TYPE=Release \
             -DPYTHON_SUPPORT=ON \
             -DJNI_SUPPORT=ON && \
    make -j$(nproc)

# Set entry point
ENTRYPOINT ["./build/focusNexus"]
```

Build with Docker:
```bash
docker build -t focus-nexus .
docker run -it focus-nexus
```

This compilation guide provides comprehensive instructions for building Focus Nexus with all its features on both Linux and Windows platforms. The build system is designed to be flexible and accommodate various development and deployment scenarios.