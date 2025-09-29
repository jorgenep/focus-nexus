#!/bin/bash

# Focus Nexus Build Script with Library Support
# This script builds Focus Nexus with full library integration support

set -e

echo "Focus Nexus Build Script with Library Support"
echo "=============================================="

# Configuration
BUILD_TYPE=${1:-Release}
ENABLE_PYTHON=${ENABLE_PYTHON:-ON}
ENABLE_JAVA=${ENABLE_JAVA:-ON}
ENABLE_PLUGINS=${ENABLE_PLUGINS:-ON}
BUILD_DIR="build"

echo "Build Configuration:"
echo "  Build Type: $BUILD_TYPE"
echo "  Python Support: $ENABLE_PYTHON"
echo "  Java Support: $ENABLE_JAVA"
echo "  Plugin Support: $ENABLE_PLUGINS"

# Clean previous build if requested
if [ "$1" = "clean" ]; then
    echo "Cleaning previous build..."
    rm -rf $BUILD_DIR
    shift
    BUILD_TYPE=${1:-Release}
fi

# Check dependencies
echo ""
echo "Checking dependencies..."

# Check for Python development headers
if [ "$ENABLE_PYTHON" = "ON" ]; then
    if command -v python3-config &> /dev/null; then
        echo "✓ Python development headers found"
        PYTHON_INCLUDES=$(python3-config --includes)
        PYTHON_LIBS=$(python3-config --libs)
        echo "  Python includes: $PYTHON_INCLUDES"
        echo "  Python libs: $PYTHON_LIBS"
    else
        echo "⚠ Python development headers not found"
        echo "  Install with: sudo apt-get install python3-dev"
        ENABLE_PYTHON=OFF
    fi
fi

# Check for Java development kit
if [ "$ENABLE_JAVA" = "ON" ]; then
    if [ -n "$JAVA_HOME" ] && [ -d "$JAVA_HOME" ]; then
        echo "✓ Java development kit found at $JAVA_HOME"
    elif command -v javac &> /dev/null; then
        echo "✓ Java compiler found"
        JAVA_HOME=$(dirname $(dirname $(readlink -f $(which javac))))
        export JAVA_HOME
        echo "  JAVA_HOME set to: $JAVA_HOME"
    else
        echo "⚠ Java development kit not found"
        echo "  Install with: sudo apt-get install openjdk-11-jdk"
        ENABLE_JAVA=OFF
    fi
fi

# Create build directory
echo ""
echo "Creating build directory..."
mkdir -p $BUILD_DIR
cd $BUILD_DIR

# Configure with CMake
echo ""
echo "Configuring with CMake..."
cmake .. \
    -DCMAKE_BUILD_TYPE=$BUILD_TYPE \
    -DPYTHON_SUPPORT=$ENABLE_PYTHON \
    -DJNI_SUPPORT=$ENABLE_JAVA \
    -DCUSTOM_PLUGIN_SUPPORT=$ENABLE_PLUGINS \
    -DCMAKE_EXPORT_COMPILE_COMMANDS=ON

if [ $? -ne 0 ]; then
    echo "❌ CMake configuration failed!"
    exit 1
fi

# Build
echo ""
echo "Building Focus Nexus..."
make -j$(nproc)

if [ $? -ne 0 ]; then
    echo "❌ Build failed!"
    exit 1
fi

echo ""
echo "✅ Build completed successfully!"
echo ""
echo "Executable: $BUILD_DIR/focusNexus"
echo ""

# Build example libraries
echo "Building example libraries..."

# Build C++ example library
if [ -f "../examples/cpp_library/math_lib.cpp" ]; then
    echo "Building C++ math library..."
    cd ../examples/cpp_library
    g++ -shared -fPIC -o libmath.so math_lib.cpp -lm
    if [ $? -eq 0 ]; then
        echo "✓ C++ library built: libmath.so"
    else
        echo "⚠ Failed to build C++ library"
    fi
    cd ../../build
fi

# Build Java example library
if [ -f "../examples/java_library/AdvancedMathUtils.java" ]; then
    echo "Building Java library..."
    cd ../examples/java_library
    javac AdvancedMathUtils.java
    if [ $? -eq 0 ]; then
        echo "✓ Java class compiled: AdvancedMathUtils.class"
        jar cf advanced-math.jar AdvancedMathUtils.class
        echo "✓ Java JAR created: advanced-math.jar"
    else
        echo "⚠ Failed to compile Java library"
    fi
    cd ../../build
fi

# Build custom plugin
if [ -f "../examples/custom_plugin/comprehensive_plugin.cpp" ]; then
    echo "Building custom plugin..."
    cd ../examples/custom_plugin
    g++ -shared -fPIC -o comprehensive_plugin.so comprehensive_plugin.cpp -I../../src
    if [ $? -eq 0 ]; then
        echo "✓ Custom plugin built: comprehensive_plugin.so"
    else
        echo "⚠ Failed to build custom plugin"
    fi
    cd ../../build
fi

# Set up environment
echo ""
echo "Setting up environment..."

# Set library paths
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:../examples/cpp_library:../examples/custom_plugin
export PYTHONPATH=$PYTHONPATH:../examples/python_library
export CLASSPATH=$CLASSPATH:../examples/java_library

echo "Environment variables set:"
echo "  LD_LIBRARY_PATH includes example directories"
echo "  PYTHONPATH includes Python library directory"
echo "  CLASSPATH includes Java library directory"

# Run tests if available
if [ -f "../tests/library_integration/test_all_libraries.fn" ]; then
    echo ""
    echo "Running library integration tests..."
    ./focusNexus ../tests/library_integration/test_all_libraries.fn
    
    if [ $? -eq 0 ]; then
        echo "✅ All tests passed!"
    else
        echo "⚠ Some tests failed"
    fi
fi

echo ""
echo "🎉 Focus Nexus with library support is ready!"
echo ""
echo "Usage examples:"
echo "  ./focusNexus                                    # Interactive mode"
echo "  ./focusNexus script.fn                         # Run script"
echo "  ./focusNexus ../tests/library_integration/test_cpp_library.fn"
echo "  ./focusNexus ../tests/library_integration/test_python_library.fn"
echo "  ./focusNexus ../tests/library_integration/test_java_library.fn"
echo "  ./focusNexus ../tests/library_integration/test_custom_plugin.fn"
echo ""
echo "Example libraries available:"
echo "  C++: ../examples/cpp_library/libmath.so"
echo "  Python: ../examples/python_library/advanced_math.py"
echo "  Java: ../examples/java_library/AdvancedMathUtils.class"
echo "  Plugin: ../examples/custom_plugin/comprehensive_plugin.so"