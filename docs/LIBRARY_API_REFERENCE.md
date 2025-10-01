# Focus Nexus Library API Reference

This document provides a complete API reference for integrating external libraries with Focus Nexus.

## Table of Contents

1. [Core Library Functions](#core-library-functions)
2. [C++ Library API](#cpp-library-api)
3. [Python Library API](#python-library-api)
4. [Java Library API](#java-library-api)
5. [Custom Plugin API](#custom-plugin-api)
6. [Type Conversion Reference](#type-conversion-reference)
7. [Error Handling](#error-handling)

## Core Library Functions

### Library Loading

#### `extern` Statement
```javascript
extern "library_path" as alias : type
```

**Parameters:**
- `library_path`: Path to the library file
- `alias`: Name to use for the library in Focus Nexus
- `type`: Library type (`cpp`, `python`, `java`)

**Examples:**
```javascript
extern "libmath.so" as mathlib : cpp
extern "numpy" as np : python
extern "java.lang.Math" as JavaMath : java
```

#### `plugin` Statement
```javascript
plugin "plugin_path" as alias
```

**Parameters:**
- `plugin_path`: Path to the plugin file
- `alias`: Name to use for the plugin in Focus Nexus

**Example:**
```javascript
plugin "my_plugin.so" as myplugin
```

#### `load_library()` Function
```javascript
let success = load_library(path, alias, type)
```

**Parameters:**
- `path`: Library file path (string)
- `alias`: Library alias (string)
- `type`: Library type (string)

**Returns:** Boolean indicating success

**Example:**
```javascript
let loaded = load_library("libmath.so", "mathlib", "cpp")
if loaded:
{
    print("Library loaded successfully")
}
```

### Function Calling

#### `call_native()` Function
```javascript
let result = call_native(library.function, arg1, arg2, ...)
```

**Parameters:**
- `library.function`: Library alias and function name
- `arg1, arg2, ...`: Function arguments

**Returns:** Function result converted to Focus Nexus Value

**Examples:**
```javascript
let result = call_native(mathlib.add, 10, 20)
let text = call_native(stringlib.reverse, "hello")
let data = call_native(pymod.process_data, [1, 2, 3])
```

## C++ Library API

### Function Signatures

Focus Nexus supports these C++ function signatures:

#### No Arguments
```cpp
extern "C" {
    double function_name();
    const char* string_function();
    int integer_function();
}
```

#### Single Argument
```cpp
extern "C" {
    double math_function(double x);
    const char* string_function(const char* input);
    int process_integer(int value);
}
```

#### Multiple Arguments
```cpp
extern "C" {
    double binary_operation(double a, double b);
    double ternary_operation(double a, double b, double c);
    double quaternary_operation(double a, double b, double c, double d);
}
```

#### Array Processing
```cpp
extern "C" {
    double process_array(double* array, int size);
    const char* process_strings(const char** strings, int count);
}
```

### Type Mapping

| Focus Nexus Type | C++ Type | Notes |
|------------------|----------|-------|
| Number | `double` | Primary numeric type |
| String | `const char*` | Null-terminated strings |
| Boolean | `int` | 0 = false, non-zero = true |
| List | `double*` + `int` | Array pointer + size |

### Example C++ Library

```cpp
#include <cmath>
#include <cstring>

extern "C" {
    // Mathematical functions
    double add(double a, double b) {
        return a + b;
    }
    
    double power(double base, double exp) {
        return pow(base, exp);
    }
    
    double factorial(double n) {
        if (n <= 1) return 1;
        double result = 1;
        for (int i = 2; i <= (int)n; i++) {
            result *= i;
        }
        return result;
    }
    
    // String functions
    const char* reverse_string(const char* input) {
        static char result[1024];
        int len = strlen(input);
        for (int i = 0; i < len; i++) {
            result[i] = input[len - 1 - i];
        }
        result[len] = '\0';
        return result;
    }
    
    // Array processing
    double sum_array(double* arr, int size) {
        double sum = 0;
        for (int i = 0; i < size; i++) {
            sum += arr[i];
        }
        return sum;
    }
    
    // Library metadata
    const char* get_library_info() {
        return "Example C++ Library v1.0";
    }
}
```

## Python Library API

### Module Structure

Python modules should be standard .py files with functions and classes:

```python
"""
Example Python module for Focus Nexus
"""

def simple_function(arg1, arg2):
    """Simple function with two arguments."""
    return arg1 + arg2

def complex_function(numbers_list):
    """Function that processes a list."""
    return sum(numbers_list) / len(numbers_list)

class ExampleClass:
    """Example class for Focus Nexus integration."""
    
    def __init__(self, initial_value=0):
        self.value = initial_value
    
    def get_value(self):
        return self.value
    
    def set_value(self, new_value):
        self.value = new_value
    
    def add_to_value(self, amount):
        self.value += amount
        return self.value

# Module metadata
def get_module_info():
    return {
        'name': 'Example Module',
        'version': '1.0.0',
        'functions': ['simple_function', 'complex_function'],
        'classes': ['ExampleClass']
    }
```

### Type Mapping

| Focus Nexus Type | Python Type | Notes |
|------------------|-------------|-------|
| Number | `float`, `int` | Automatically converted |
| String | `str` | UTF-8 strings |
| Boolean | `bool` | True/False values |
| List | `list` | Python lists |
| Nil | `None` | Null value |

### Supported Python Features

- **Functions**: Regular functions with any number of arguments
- **Classes**: Class instantiation and method calls
- **Lists**: Python lists converted to/from Focus Nexus lists
- **Dictionaries**: Converted to string representation
- **Exceptions**: Python exceptions caught and converted to Focus Nexus errors

## Java Library API

### Class Requirements

Java classes must be compiled and available in the classpath:

```java
public class ExampleJavaLibrary {
    
    // Static methods (recommended for simple functions)
    public static double add(double a, double b) {
        return a + b;
    }
    
    public static String processString(String input) {
        return input.toUpperCase();
    }
    
    public static boolean checkCondition(int value) {
        return value > 0;
    }
    
    // Array processing
    public static double sum(double[] array) {
        double sum = 0;
        for (double value : array) {
            sum += value;
        }
        return sum;
    }
    
    // Instance methods
    private double value;
    
    public ExampleJavaLibrary(double initialValue) {
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

### Type Mapping

| Focus Nexus Type | Java Type | JNI Signature |
|------------------|-----------|---------------|
| Number | `double` | `D` |
| Number | `int` | `I` |
| Number | `long` | `J` |
| String | `String` | `Ljava/lang/String;` |
| Boolean | `boolean` | `Z` |
| Nil | `null` | `Ljava/lang/Object;` |

### Supported Method Signatures

Focus Nexus automatically detects these Java method signatures:

```java
// No arguments
public static double method();
public static String method();
public static boolean method();

// Single argument
public static double method(double arg);
public static String method(String arg);
public static boolean method(int arg);

// Multiple arguments
public static double method(double a, double b);
public static String method(String a, String b);
public static int method(int a, int b, int c);
```

## Custom Plugin API

### Plugin Structure

Custom plugins must implement the Focus Nexus Plugin API:

```cpp
#include "focus_nexus_plugin.h"

// Plugin lifecycle functions
void plugin_init() {
    // Initialize plugin resources
}

void plugin_cleanup() {
    // Clean up plugin resources
}

const char* plugin_info() {
    return "Example Plugin v1.0";
}

// Plugin functions
Value example_function(const std::vector<Value>& args) {
    PLUGIN_REQUIRE_ARGS(2);
    PLUGIN_REQUIRE_NUMBER(args[0], 0);
    PLUGIN_REQUIRE_NUMBER(args[1], 1);
    
    double result = args[0].asNumber() + args[1].asNumber();
    return Value(result);
}

// Export functions
FOCUS_NEXUS_PLUGIN_INIT(plugin_init)
FOCUS_NEXUS_PLUGIN_CLEANUP(plugin_cleanup)
FOCUS_NEXUS_PLUGIN_INFO(plugin_info)
FOCUS_NEXUS_EXPORT_FUNCTION(example_function, example_function)
```

### Plugin API Macros

#### Lifecycle Macros
```cpp
FOCUS_NEXUS_PLUGIN_INIT(function_name)
FOCUS_NEXUS_PLUGIN_CLEANUP(function_name)
FOCUS_NEXUS_PLUGIN_INFO(info_string)
```

#### Function Export Macro
```cpp
FOCUS_NEXUS_EXPORT_FUNCTION(exported_name, implementation_function)
```

#### Argument Validation Macros
```cpp
PLUGIN_REQUIRE_ARGS(count)           // Exact argument count
PLUGIN_REQUIRE_MIN_ARGS(count)       // Minimum argument count
PLUGIN_REQUIRE_NUMBER(arg, index)    // Require number at index
PLUGIN_REQUIRE_STRING(arg, index)    // Require string at index
PLUGIN_REQUIRE_BOOL(arg, index)      // Require boolean at index
```

### Value Class API

The `Value` class provides these methods for plugin development:

```cpp
class Value {
public:
    // Type checking
    bool isNil() const;
    bool isBool() const;
    bool isNumber() const;
    bool isString() const;
    bool isList() const;
    
    // Value extraction
    bool asBool() const;
    double asNumber() const;
    std::string asString() const;
    std::shared_ptr<std::vector<Value>> asList() const;
    
    // Utility
    bool isTruthy() const;
    std::string toString() const;
    std::string getType() const;
    
    // Constructors
    Value();                                    // nil
    Value(bool b);                             // boolean
    Value(double d);                           // number
    Value(const std::string& s);               // string
    Value(std::shared_ptr<std::vector<Value>> l); // list
};
```

## Type Conversion Reference

### Focus Nexus to External Types

| Focus Nexus | C++ | Python | Java |
|-------------|-----|--------|------|
| Number | `double` | `float` | `double` |
| String | `const char*` | `str` | `String` |
| Boolean | `int` (0/1) | `bool` | `boolean` |
| List | `double*` + `int` | `list` | `double[]` |
| Nil | `NULL` | `None` | `null` |

### External to Focus Nexus Types

| External | Focus Nexus | Notes |
|----------|-------------|-------|
| C++ `double` | Number | Direct conversion |
| C++ `const char*` | String | Copied to Focus Nexus string |
| C++ `int` | Number | Converted to double |
| Python `int/float` | Number | Automatic conversion |
| Python `str` | String | UTF-8 encoding |
| Python `bool` | Boolean | Direct conversion |
| Python `list` | List | Recursive conversion |
| Python `None` | Nil | Null value |
| Java `double` | Number | Direct conversion |
| Java `String` | String | UTF-8 conversion |
| Java `boolean` | Boolean | Direct conversion |
| Java `int/long` | Number | Converted to double |

## Error Handling

### Error Types

#### Library Loading Errors
```javascript
try:
{
    extern "nonexistent.so" as lib : cpp
}
catch(error):
{
    print("Library loading failed:", error)
}
```

#### Function Call Errors
```javascript
try:
{
    let result = call_native(lib.nonexistent_function, 1, 2)
}
catch(error):
{
    print("Function call failed:", error)
}
```

#### Type Conversion Errors
```javascript
try:
{
    let result = call_native(lib.numeric_function, "not a number")
}
catch(error):
{
    print("Type conversion failed:", error)
}
```

### Error Messages

Common error message patterns:

- `"Library 'name' not loaded"`
- `"Function 'name' not found in library"`
- `"Function requires exactly N arguments"`
- `"Argument N must be a number/string/boolean"`
- `"Failed to load library: path (Error: details)"`

### Best Practices for Error Handling

#### C++ Libraries
```cpp
extern "C" {
    double safe_divide(double a, double b) {
        if (b == 0.0) {
            // Return NaN or special value
            return std::numeric_limits<double>::quiet_NaN();
        }
        return a / b;
    }
}
```

#### Python Modules
```python
def safe_operation(value):
    try:
        return risky_calculation(value)
    except Exception as e:
        # Return None or raise with clear message
        raise ValueError(f"Operation failed: {e}")
```

#### Java Classes
```java
public static double safeDivide(double a, double b) {
    if (b == 0.0) {
        throw new ArithmeticException("Division by zero");
    }
    return a / b;
}
```

#### Custom Plugins
```cpp
Value safe_plugin_function(const std::vector<Value>& args) {
    try {
        PLUGIN_REQUIRE_ARGS(2);
        PLUGIN_REQUIRE_NUMBER(args[0], 0);
        PLUGIN_REQUIRE_NUMBER(args[1], 1);
        
        // Function implementation
        return Value(result);
    } catch (const std::exception& e) {
        throw std::runtime_error("Plugin function failed: " + std::string(e.what()));
    }
}
```

## Performance Considerations

### Function Call Overhead

| Library Type | Overhead | Notes |
|--------------|----------|-------|
| C++ | Low | Direct function calls |
| Custom Plugin | Low | Direct function calls |
| Python | Medium | Python C API overhead |
| Java | High | JNI call overhead |

### Optimization Tips

1. **Cache Function Pointers**: Library manager automatically caches loaded functions
2. **Batch Operations**: Process multiple values in single calls when possible
3. **Minimize Type Conversions**: Use compatible types
4. **Prefer Static Methods**: For Java, static methods are faster than instance methods

### Memory Management

#### C++ Libraries
- Use static buffers for string returns
- Avoid dynamic allocation in frequently called functions
- Clean up resources in library destructors

#### Python Integration
- Automatic reference counting handled by Focus Nexus
- Large objects may cause memory pressure
- Consider using generators for large datasets

#### Java Integration
- Local references automatically cleaned up
- Global references managed by Focus Nexus
- Large object creation may trigger GC

#### Custom Plugins
- Implement proper cleanup functions
- Use RAII for resource management
- Avoid memory leaks in plugin functions

This API reference provides complete documentation for integrating external libraries with Focus Nexus, enabling developers to extend the language with powerful external functionality while maintaining type safety and performance.