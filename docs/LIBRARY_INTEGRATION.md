# Focus Nexus Library Integration Guide

Focus Nexus supports integration with external libraries written in C++, Python, Java, and custom plugins. This guide provides comprehensive documentation and examples for each type of library integration.

## Table of Contents

1. [Overview](#overview)
2. [C++ Library Integration](#cpp-library-integration)
3. [Python Library Integration](#python-library-integration)
4. [Java Library Integration](#java-library-integration)
5. [Custom Plugin Development](#custom-plugin-development)
6. [Usage Examples](#usage-examples)
7. [Compilation Instructions](#compilation-instructions)
8. [Troubleshooting](#troubleshooting)

## Overview

The Focus Nexus interpreter provides a flexible library integration system that allows you to:

- Load and use C++ shared libraries (.dll/.so)
- Execute Python modules and functions
- Call Java methods through JNI
- Create custom plugins with the Focus Nexus Plugin API

### New Language Syntax

The following new tokens and syntax have been added:

```javascript
// Load external libraries
extern "path/to/library.so" as mylib : cpp
plugin "path/to/plugin.so" as myplugin

// Call external functions
let result = call_native(mylib.function_name, arg1, arg2)
let loaded = load_library("path/to/lib.so", "alias", "cpp")
```

## C++ Library Integration

### Creating a C++ Library

Create a simple C++ library that can be used with Focus Nexus:

**math_lib.cpp:**
```cpp
extern "C" {
    double add_numbers(double a, double b) {
        return a + b;
    }
    
    double multiply_numbers(double a, double b) {
        return a * b;
    }
    
    double calculate_power(double base, double exponent) {
        return pow(base, exponent);
    }
}
```

**Compile the library:**
```bash
# Linux
g++ -shared -fPIC -o libmath.so math_lib.cpp -lm

# Windows
g++ -shared -o math.dll math_lib.cpp
```

### Using C++ Libraries in Focus Nexus

```javascript
// Load the C++ library
extern "libmath.so" as mathlib : cpp

// Call C++ functions
let sum = call_native(mathlib.add_numbers, 10, 20)
print("Sum:", sum)

let product = call_native(mathlib.multiply_numbers, 5, 6)
print("Product:", product)

let power = call_native(mathlib.calculate_power, 2, 8)
print("2^8 =", power)
```

### Advanced C++ Integration

For more complex data types and better integration:

**advanced_lib.cpp:**
```cpp
#include <string>
#include <vector>

extern "C" {
    // String manipulation
    const char* reverse_string(const char* input) {
        static std::string result;
        result = std::string(input);
        std::reverse(result.begin(), result.end());
        return result.c_str();
    }
    
    // Array processing
    double sum_array(double* arr, int size) {
        double sum = 0.0;
        for (int i = 0; i < size; i++) {
            sum += arr[i];
        }
        return sum;
    }
}
```

## Python Library Integration

### Prerequisites

Ensure Python development headers are installed:

```bash
# Ubuntu/Debian
sudo apt-get install python3-dev

# CentOS/RHEL
sudo yum install python3-devel

# Windows
# Install Python from python.org with development libraries
```

### Creating a Python Module

**math_module.py:**
```python
def fibonacci(n):
    """Calculate the nth Fibonacci number."""
    if n <= 1:
        return n
    return fibonacci(n-1) + fibonacci(n-2)

def factorial(n):
    """Calculate factorial of n."""
    if n <= 1:
        return 1
    return n * factorial(n-1)

def is_prime(n):
    """Check if a number is prime."""
    if n < 2:
        return False
    for i in range(2, int(n**0.5) + 1):
        if n % i == 0:
            return False
    return True

class Calculator:
    """A simple calculator class."""
    
    def __init__(self):
        self.history = []
    
    def add(self, a, b):
        result = a + b
        self.history.append(f"{a} + {b} = {result}")
        return result
    
    def get_history(self):
        return self.history
```

### Using Python Libraries in Focus Nexus

```javascript
// Load the Python module
extern "math_module.py" as pymath : python

// Call Python functions
let fib10 = call_native(pymath.fibonacci, 10)
print("Fibonacci(10):", fib10)

let fact5 = call_native(pymath.factorial, 5)
print("5! =", fact5)

let prime_check = call_native(pymath.is_prime, 17)
print("Is 17 prime?", prime_check)

// Use Python classes (advanced)
let calc = call_native(pymath.Calculator)
let sum = call_native(calc.add, 15, 25)
print("Calculator result:", sum)
```

## Java Library Integration

### Prerequisites

Ensure JDK is installed and JNI headers are available:

```bash
# Ubuntu/Debian
sudo apt-get install openjdk-11-jdk

# Windows
# Install Oracle JDK or OpenJDK
```

### Creating a Java Class

**MathUtils.java:**
```java
public class MathUtils {
    
    public static double add(double a, double b) {
        return a + b;
    }
    
    public static double subtract(double a, double b) {
        return a - b;
    }
    
    public static boolean isPerfectSquare(int n) {
        int sqrt = (int) Math.sqrt(n);
        return sqrt * sqrt == n;
    }
    
    public static String reverseString(String input) {
        return new StringBuilder(input).reverse().toString();
    }
    
    // Instance methods
    private double value;
    
    public MathUtils(double initialValue) {
        this.value = initialValue;
    }
    
    public double getValue() {
        return value;
    }
    
    public void setValue(double newValue) {
        this.value = newValue;
    }
    
    public double multiply(double factor) {
        this.value *= factor;
        return this.value;
    }
}
```

**Compile the Java class:**
```bash
javac MathUtils.java
jar cf mathutils.jar MathUtils.class
```

### Using Java Libraries in Focus Nexus

```javascript
// Load the Java class
extern "MathUtils" as javamath : java

// Call static Java methods
let sum = call_native(javamath.add, 15, 30)
print("Java add result:", sum)

let is_perfect = call_native(javamath.isPerfectSquare, 16)
print("Is 16 a perfect square?", is_perfect)

let reversed = call_native(javamath.reverseString, "Hello World")
print("Reversed string:", reversed)

// Use Java objects (advanced)
let math_obj = call_native(javamath.new, 10.0)
let multiplied = call_native(math_obj.multiply, 3.5)
print("Multiplied value:", multiplied)
```

## Custom Plugin Development

### Plugin API

Focus Nexus provides a C API for creating custom plugins:

**my_plugin.cpp:**
```cpp
#include "focus_nexus_plugin.h"
#include <cmath>
#include <string>

// Plugin initialization
void plugin_init() {
    // Initialize plugin resources
}

// Plugin cleanup
void plugin_cleanup() {
    // Clean up plugin resources
}

// Plugin information
const char* plugin_info() {
    return "My Custom Plugin v1.0 - Mathematical functions";
}

// Custom function: Calculate distance between two points
Value calculate_distance(const std::vector<Value>& args) {
    if (args.size() != 4) {
        throw std::runtime_error("calculate_distance requires 4 arguments: x1, y1, x2, y2");
    }
    
    double x1 = args[0].asNumber();
    double y1 = args[1].asNumber();
    double x2 = args[2].asNumber();
    double y2 = args[3].asNumber();
    
    double distance = sqrt(pow(x2 - x1, 2) + pow(y2 - y1, 2));
    return Value(distance);
}

// Custom function: Generate random number
Value random_number(const std::vector<Value>& args) {
    double min = 0.0, max = 1.0;
    
    if (args.size() >= 1) min = args[0].asNumber();
    if (args.size() >= 2) max = args[1].asNumber();
    
    double random = min + (max - min) * (rand() / (double)RAND_MAX);
    return Value(random);
}

// Custom function: String manipulation
Value capitalize_words(const std::vector<Value>& args) {
    if (args.size() != 1 || !args[0].isString()) {
        throw std::runtime_error("capitalize_words requires one string argument");
    }
    
    std::string input = args[0].asString();
    std::string result;
    bool capitalize_next = true;
    
    for (char c : input) {
        if (std::isspace(c)) {
            capitalize_next = true;
            result += c;
        } else if (capitalize_next) {
            result += std::toupper(c);
            capitalize_next = false;
        } else {
            result += std::tolower(c);
        }
    }
    
    return Value(result);
}

// Export functions using macros
FOCUS_NEXUS_PLUGIN_INIT(plugin_init)
FOCUS_NEXUS_PLUGIN_CLEANUP(plugin_cleanup)
FOCUS_NEXUS_PLUGIN_INFO(plugin_info)
FOCUS_NEXUS_EXPORT_FUNCTION(calculate_distance, calculate_distance)
FOCUS_NEXUS_EXPORT_FUNCTION(random_number, random_number)
FOCUS_NEXUS_EXPORT_FUNCTION(capitalize_words, capitalize_words)
```

### Plugin Header File

**focus_nexus_plugin.h:**
```cpp
#pragma once
#include "../runtime/value.hpp"
#include <vector>
#include <stdexcept>

// Plugin API macros (already defined in library_manager.hpp)
#define FOCUS_NEXUS_PLUGIN_INIT(func) extern "C" void focus_nexus_plugin_init() { func(); }
#define FOCUS_NEXUS_PLUGIN_CLEANUP(func) extern "C" void focus_nexus_plugin_cleanup() { func(); }
#define FOCUS_NEXUS_PLUGIN_INFO(info) extern "C" const char* focus_nexus_plugin_info() { return info; }
#define FOCUS_NEXUS_EXPORT_FUNCTION(name, func) extern "C" Value focus_nexus_##name(const std::vector<Value>& args) { return func(args); }
```

### Compile the Plugin

```bash
# Linux
g++ -shared -fPIC -o my_plugin.so my_plugin.cpp -I/path/to/focus-nexus/src

# Windows
g++ -shared -o my_plugin.dll my_plugin.cpp -I/path/to/focus-nexus/src
```

### Using Custom Plugins in Focus Nexus

```javascript
// Load the custom plugin
plugin "my_plugin.so" as myplugin

// Use plugin functions
let distance = call_native(myplugin.calculate_distance, 0, 0, 3, 4)
print("Distance:", distance)  // Should print 5.0

let random = call_native(myplugin.random_number, 1, 100)
print("Random number:", random)

let capitalized = call_native(myplugin.capitalize_words, "hello world from focus nexus")
print("Capitalized:", capitalized)  // "Hello World From Focus Nexus"
```

## Usage Examples

### Complete Example Program

**library_demo.fn:**
```javascript
// Focus Nexus Library Integration Demo

print("=== Focus Nexus Library Integration Demo ===")

// 1. C++ Library Integration
print("\n1. C++ Library Integration:")
extern "libmath.so" as mathlib : cpp

let cpp_sum = call_native(mathlib.add_numbers, 15, 25)
print("C++ Addition: 15 + 25 =", cpp_sum)

let cpp_power = call_native(mathlib.calculate_power, 2, 10)
print("C++ Power: 2^10 =", cpp_power)

// 2. Python Library Integration
print("\n2. Python Library Integration:")
extern "math_module.py" as pymath : python

let py_fib = call_native(pymath.fibonacci, 8)
print("Python Fibonacci(8):", py_fib)

let py_prime = call_native(pymath.is_prime, 29)
print("Python is_prime(29):", py_prime)

// 3. Java Library Integration
print("\n3. Java Library Integration:")
extern "MathUtils" as javamath : java

let java_sum = call_native(javamath.add, 100, 200)
print("Java Addition: 100 + 200 =", java_sum)

let java_reversed = call_native(javamath.reverseString, "Focus Nexus")
print("Java Reversed String:", java_reversed)

// 4. Custom Plugin Integration
print("\n4. Custom Plugin Integration:")
plugin "my_plugin.so" as myplugin

let plugin_distance = call_native(myplugin.calculate_distance, 1, 1, 4, 5)
print("Plugin Distance Calculation:", plugin_distance)

let plugin_random = call_native(myplugin.random_number, 50, 100)
print("Plugin Random Number:", plugin_random)

// 5. Dynamic Library Loading
print("\n5. Dynamic Library Loading:")
let loaded = load_library("additional_lib.so", "extralib", "cpp")
if loaded:
{
    print("Additional library loaded successfully")
    let extra_result = call_native(extralib.some_function, 42)
    print("Extra library result:", extra_result)
}
else:
{
    print("Failed to load additional library")
}

print("\n=== Demo Complete ===")
```

### Error Handling Example

```javascript
// Error handling with external libraries
function safe_library_call(lib, func, args):
{
    try:
    {
        return call_native(lib.func, args)
    }
    catch(error):
    {
        print("Library call failed:", error)
        return nil
    }
    finally:
    {
        print("Library call completed")
    }
}

// Usage
extern "mylib.so" as mylib : cpp
let result = safe_library_call(mylib, "risky_function", [1, 2, 3])
```

## Compilation Instructions

### Linux Compilation

**Prerequisites:**
```bash
# Install required packages
sudo apt-get update
sudo apt-get install build-essential cmake
sudo apt-get install python3-dev  # For Python support
sudo apt-get install openjdk-11-jdk  # For Java support
```

**Build Focus Nexus:**
```bash
# Clone and build
git clone <repository-url>
cd focus-nexus

# Create build directory
mkdir build && cd build

# Configure with CMake
cmake .. -DCMAKE_BUILD_TYPE=Release

# Build
make -j$(nproc)

# Install (optional)
sudo make install
```

**Build with specific library support:**
```bash
# Build with Python support
cmake .. -DPYTHON_SUPPORT=ON

# Build with Java support
cmake .. -DJNI_SUPPORT=ON

# Build with all features
cmake .. -DPYTHON_SUPPORT=ON -DJNI_SUPPORT=ON
```

### Windows Compilation

**Prerequisites:**
- Visual Studio 2019 or later
- CMake 3.16 or later
- Python 3.x (for Python support)
- JDK 11 or later (for Java support)

**Build Focus Nexus:**
```cmd
REM Clone and build
git clone <repository-url>
cd focus-nexus

REM Create build directory
mkdir build
cd build

REM Configure with CMake
cmake .. -G "Visual Studio 16 2019" -A x64

REM Build
cmake --build . --config Release

REM Or use the batch file
cd ..
build.bat
```

**Build with library support:**
```cmd
REM Configure with Python support
cmake .. -G "Visual Studio 16 2019" -A x64 -DPYTHON_SUPPORT=ON

REM Configure with Java support
cmake .. -G "Visual Studio 16 2019" -A x64 -DJNI_SUPPORT=ON
```

### Cross-Platform Build Script

**build_all.sh:**
```bash
#!/bin/bash

echo "Building Focus Nexus with library support..."

# Detect platform
if [[ "$OSTYPE" == "linux-gnu"* ]]; then
    PLATFORM="linux"
elif [[ "$OSTYPE" == "darwin"* ]]; then
    PLATFORM="macos"
elif [[ "$OSTYPE" == "msys" ]] || [[ "$OSTYPE" == "win32" ]]; then
    PLATFORM="windows"
else
    echo "Unsupported platform: $OSTYPE"
    exit 1
fi

echo "Detected platform: $PLATFORM"

# Create build directory
mkdir -p build
cd build

# Configure based on platform
if [ "$PLATFORM" = "linux" ] || [ "$PLATFORM" = "macos" ]; then
    cmake .. -DCMAKE_BUILD_TYPE=Release -DPYTHON_SUPPORT=ON -DJNI_SUPPORT=ON
    make -j$(nproc)
elif [ "$PLATFORM" = "windows" ]; then
    cmake .. -G "Visual Studio 16 2019" -A x64 -DPYTHON_SUPPORT=ON -DJNI_SUPPORT=ON
    cmake --build . --config Release
fi

echo "Build complete!"
```

## Troubleshooting

### Common Issues

1. **Library not found:**
   ```
   Error: Failed to load library: libmath.so
   ```
   - Ensure the library path is correct
   - Check that the library is in the system PATH or LD_LIBRARY_PATH
   - Verify library architecture matches Focus Nexus (32-bit vs 64-bit)

2. **Function not found:**
   ```
   Error: Function 'my_function' not found in C++ library
   ```
   - Ensure functions are exported with `extern "C"`
   - Check function name spelling
   - Use `nm` (Linux) or `dumpbin` (Windows) to list exported symbols

3. **Python module import error:**
   ```
   Error: Failed to import Python module
   ```
   - Ensure Python is properly installed
   - Check PYTHONPATH environment variable
   - Verify module file exists and is accessible

4. **Java class loading error:**
   ```
   Error: Java class not found
   ```
   - Ensure CLASSPATH includes the jar file
   - Check class name spelling and package structure
   - Verify JVM is properly initialized

### Debug Tips

1. **Enable verbose logging:**
   ```javascript
   // Set debug mode (if implemented)
   set_debug_mode(true)
   ```

2. **Check loaded libraries:**
   ```javascript
   // List all loaded libraries
   let libs = get_loaded_libraries()
   print("Loaded libraries:", libs)
   ```

3. **Verify library functions:**
   ```javascript
   // Check if function exists
   let has_func = has_function("mylib", "my_function")
   print("Function exists:", has_func)
   ```

### Performance Considerations

1. **Library Loading:** Load libraries once at startup, not in loops
2. **Function Calls:** Cache function pointers when possible
3. **Data Marshalling:** Minimize type conversions between languages
4. **Memory Management:** Ensure proper cleanup of external resources

This completes the comprehensive library integration system for Focus Nexus. The system provides flexible, extensible support for multiple programming languages and custom plugins, making Focus Nexus a powerful platform for integrating existing codebases and extending functionality.