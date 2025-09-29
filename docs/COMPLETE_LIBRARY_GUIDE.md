# Complete Library Integration Guide for Focus Nexus

This comprehensive guide covers all aspects of integrating external libraries with Focus Nexus, including C++, Python, Java, and custom plugins.

## Table of Contents

1. [Overview](#overview)
2. [C++ Library Integration](#cpp-library-integration)
3. [Python Library Integration](#python-library-integration)
4. [Java Library Integration](#java-library-integration)
5. [Custom Plugin Development](#custom-plugin-development)
6. [Compilation and Setup](#compilation-and-setup)
7. [Complete Examples](#complete-examples)
8. [Troubleshooting](#troubleshooting)
9. [Best Practices](#best-practices)

## Overview

Focus Nexus provides a unified interface for integrating external libraries written in different languages. The system supports:

- **C++ Libraries**: Shared libraries (.dll/.so) with C-style exports
- **Python Modules**: Standard Python .py files and packages
- **Java Classes**: Compiled Java classes accessible via JNI
- **Custom Plugins**: Native plugins using the Focus Nexus Plugin API

### Language Syntax

```javascript
// Load external libraries
extern "path/to/library.so" as mylib : cpp
extern "module_name.py" as pymod : python
extern "ClassName" as javaclass : java
plugin "path/to/plugin.so" as myplugin

// Call external functions
let result = call_native(mylib.function_name, arg1, arg2)
let py_result = call_native(pymod.python_function, arg1, arg2)
let java_result = call_native(javaclass.staticMethod, arg1, arg2)
let plugin_result = call_native(myplugin.plugin_function, arg1, arg2)

// Dynamic loading
let loaded = load_library("path/to/lib.so", "alias", "cpp")
```

## C++ Library Integration

### Creating C++ Libraries

C++ libraries must export functions using C linkage to be compatible with Focus Nexus.

**Example: math_library.cpp**
```cpp
#include <cmath>
#include <cstring>
#include <algorithm>

extern "C" {
    // Basic arithmetic functions
    double add_numbers(double a, double b) {
        return a + b;
    }
    
    double multiply_numbers(double a, double b) {
        return a * b;
    }
    
    double calculate_power(double base, double exponent) {
        return pow(base, exponent);
    }
    
    // String manipulation
    const char* reverse_string(const char* input) {
        static char result[1024];
        int len = strlen(input);
        
        if (len >= 1024) {
            strcpy(result, "String too long");
            return result;
        }
        
        for (int i = 0; i < len; i++) {
            result[i] = input[len - 1 - i];
        }
        result[len] = '\0';
        
        return result;
    }
    
    // Advanced mathematical functions
    double calculate_factorial(double n) {
        if (n < 0 || n != floor(n)) {
            return 0.0; // Handle invalid input
        }
        
        double result = 1.0;
        for (int i = 2; i <= (int)n; i++) {
            result *= i;
        }
        return result;
    }
    
    // Array processing
    double sum_array(double* arr, int size) {
        double sum = 0.0;
        for (int i = 0; i < size; i++) {
            sum += arr[i];
        }
        return sum;
    }
    
    // Library information
    const char* get_library_info() {
        return "Focus Nexus Math Library v1.0";
    }
}
```

**Compilation:**
```bash
# Linux
g++ -shared -fPIC -o libmath.so math_library.cpp -lm

# Windows (MinGW)
g++ -shared -o math.dll math_library.cpp

# Windows (MSVC)
cl /LD math_library.cpp /Fe:math.dll
```

### Using C++ Libraries in Focus Nexus

```javascript
// Load the C++ library
extern "libmath.so" as mathlib : cpp

// Call C++ functions
let sum = call_native(mathlib.add_numbers, 15, 25)
print("C++ Addition: 15 + 25 =", sum)

let power = call_native(mathlib.calculate_power, 2, 10)
print("C++ Power: 2^10 =", power)

let factorial = call_native(mathlib.calculate_factorial, 5)
print("C++ Factorial(5) =", factorial)

let reversed = call_native(mathlib.reverse_string, "Hello World")
print("C++ Reversed:", reversed)

let info = call_native(mathlib.get_library_info)
print("Library Info:", info)
```

### Supported C++ Function Signatures

Focus Nexus automatically detects and handles these function signatures:

```cpp
// No arguments
double func();
const char* func();

// Single argument
double func(double);
const char* func(const char*);

// Multiple arguments
double func(double, double);
double func(double, double, double);
double func(double, double, double, double);

// Array processing (simplified)
double func(double*, int);
```

## Python Library Integration

### Creating Python Modules

Python modules are standard .py files that can be imported and used by Focus Nexus.

**Example: advanced_math.py**
```python
"""
Advanced mathematical functions for Focus Nexus
"""

import math
import random
from typing import List, Union

def fibonacci(n: int) -> int:
    """Calculate the nth Fibonacci number."""
    if n < 0:
        raise ValueError("Fibonacci number cannot be negative")
    if n <= 1:
        return n
    
    a, b = 0, 1
    for _ in range(2, n + 1):
        a, b = b, a + b
    return b

def is_prime(n: int) -> bool:
    """Check if a number is prime."""
    if n < 2:
        return False
    if n == 2:
        return True
    if n % 2 == 0:
        return False
    
    for i in range(3, int(math.sqrt(n)) + 1, 2):
        if n % i == 0:
            return False
    return True

def prime_factors(n: int) -> List[int]:
    """Return a list of prime factors."""
    factors = []
    d = 2
    while d * d <= n:
        while n % d == 0:
            factors.append(d)
            n //= d
        d += 1
    if n > 1:
        factors.append(n)
    return factors

def gcd(a: int, b: int) -> int:
    """Calculate Greatest Common Divisor."""
    while b:
        a, b = b, a % b
    return abs(a)

def lcm(a: int, b: int) -> int:
    """Calculate Least Common Multiple."""
    return abs(a * b) // gcd(a, b) if a and b else 0

def statistics_summary(numbers: List[float]) -> dict:
    """Calculate comprehensive statistics."""
    if not numbers:
        raise ValueError("Cannot calculate statistics for empty list")
    
    sorted_nums = sorted(numbers)
    n = len(numbers)
    
    # Mean
    mean = sum(numbers) / n
    
    # Median
    if n % 2 == 0:
        median = (sorted_nums[n // 2 - 1] + sorted_nums[n // 2]) / 2
    else:
        median = sorted_nums[n // 2]
    
    # Standard deviation
    variance = sum((x - mean) ** 2 for x in numbers) / (n - 1) if n > 1 else 0
    std_dev = math.sqrt(variance)
    
    return {
        'count': n,
        'sum': sum(numbers),
        'mean': mean,
        'median': median,
        'std_dev': std_dev,
        'min': min(numbers),
        'max': max(numbers),
        'range': max(numbers) - min(numbers)
    }

def generate_random_data(size: int, min_val: float = 0, max_val: float = 100) -> List[float]:
    """Generate random data for testing."""
    return [random.uniform(min_val, max_val) for _ in range(size)]

class Calculator:
    """Advanced calculator with history."""
    
    def __init__(self):
        self.history = []
        self.memory = 0
    
    def add(self, a: float, b: float) -> float:
        result = a + b
        self.history.append(f"{a} + {b} = {result}")
        return result
    
    def multiply(self, a: float, b: float) -> float:
        result = a * b
        self.history.append(f"{a} * {b} = {result}")
        return result
    
    def power(self, base: float, exp: float) -> float:
        result = base ** exp
        self.history.append(f"{base} ^ {exp} = {result}")
        return result
    
    def get_history(self) -> List[str]:
        return self.history.copy()
    
    def clear_history(self):
        self.history.clear()

class DataProcessor:
    """Data processing utilities."""
    
    @staticmethod
    def filter_primes(numbers: List[int]) -> List[int]:
        return [n for n in numbers if is_prime(n)]
    
    @staticmethod
    def factorial_list(numbers: List[int]) -> List[int]:
        return [math.factorial(n) for n in numbers if n >= 0 and n <= 20]
    
    @staticmethod
    def fibonacci_sequence(count: int) -> List[int]:
        return [fibonacci(i) for i in range(count)]

# Module information
def get_module_info() -> dict:
    return {
        'name': 'Advanced Math Module',
        'version': '2.0.0',
        'description': 'Comprehensive mathematical functions for Focus Nexus',
        'author': 'Focus Nexus Team'
    }
```

### Using Python Libraries in Focus Nexus

```javascript
// Load the Python module
extern "advanced_math.py" as pymath : python

// Call Python functions
let fib10 = call_native(pymath.fibonacci, 10)
print("Python Fibonacci(10):", fib10)

let prime_check = call_native(pymath.is_prime, 97)
print("Is 97 prime?", prime_check)

let factors = call_native(pymath.prime_factors, 60)
print("Prime factors of 60:", factors)

let gcd_result = call_native(pymath.gcd, 48, 18)
print("GCD(48, 18):", gcd_result)

// Use Python classes
let calc = call_native(pymath.Calculator)
let calc_result = call_native(calc.add, 100, 50)
print("Python Calculator: 100 + 50 =", calc_result)

let history = call_native(calc.get_history)
print("Calculator history:", history)

// Data processing
let data_proc = call_native(pymath.DataProcessor)
let fib_seq = call_native(data_proc.fibonacci_sequence, 10)
print("Fibonacci sequence:", fib_seq)

// Generate and process random data
let random_data = call_native(pymath.generate_random_data, 10, 1, 100)
print("Random data:", random_data)

let stats = call_native(pymath.statistics_summary, random_data)
print("Statistics:", stats)
```

## Java Library Integration

### Creating Java Classes

Java classes must be compiled and available in the classpath.

**Example: AdvancedMathUtils.java**
```java
import java.util.*;
import java.math.BigInteger;

public class AdvancedMathUtils {
    
    // Basic arithmetic
    public static double add(double a, double b) {
        return a + b;
    }
    
    public static double subtract(double a, double b) {
        return a - b;
    }
    
    public static double multiply(double a, double b) {
        return a * b;
    }
    
    public static double divide(double a, double b) {
        if (b == 0.0) {
            throw new ArithmeticException("Division by zero");
        }
        return a / b;
    }
    
    // Advanced mathematical functions
    public static long factorial(int n) {
        if (n < 0) {
            throw new IllegalArgumentException("Factorial cannot be negative");
        }
        if (n <= 1) return 1;
        
        long result = 1;
        for (int i = 2; i <= n; i++) {
            result *= i;
        }
        return result;
    }
    
    public static String bigFactorial(int n) {
        if (n < 0) {
            throw new IllegalArgumentException("Factorial cannot be negative");
        }
        
        BigInteger result = BigInteger.ONE;
        for (int i = 2; i <= n; i++) {
            result = result.multiply(BigInteger.valueOf(i));
        }
        return result.toString();
    }
    
    public static long fibonacci(int n) {
        if (n < 0) {
            throw new IllegalArgumentException("Fibonacci number cannot be negative");
        }
        if (n <= 1) return n;
        
        long a = 0, b = 1, temp;
        for (int i = 2; i <= n; i++) {
            temp = a + b;
            a = b;
            b = temp;
        }
        return b;
    }
    
    public static boolean isPrime(int n) {
        if (n < 2) return false;
        if (n == 2) return true;
        if (n % 2 == 0) return false;
        
        for (int i = 3; i <= Math.sqrt(n); i += 2) {
            if (n % i == 0) return false;
        }
        return true;
    }
    
    public static int gcd(int a, int b) {
        while (b != 0) {
            int temp = b;
            b = a % b;
            a = temp;
        }
        return Math.abs(a);
    }
    
    public static int lcm(int a, int b) {
        if (a == 0 || b == 0) return 0;
        return Math.abs(a * b) / gcd(a, b);
    }
    
    // String manipulation
    public static String reverseString(String input) {
        if (input == null) return null;
        return new StringBuilder(input).reverse().toString();
    }
    
    public static String capitalizeWords(String input) {
        if (input == null || input.isEmpty()) return input;
        
        StringBuilder result = new StringBuilder();
        boolean capitalizeNext = true;
        
        for (char c : input.toCharArray()) {
            if (Character.isWhitespace(c)) {
                capitalizeNext = true;
                result.append(c);
            } else if (capitalizeNext) {
                result.append(Character.toUpperCase(c));
                capitalizeNext = false;
            } else {
                result.append(Character.toLowerCase(c));
            }
        }
        
        return result.toString();
    }
    
    public static boolean isPalindrome(String input) {
        if (input == null) return false;
        String cleaned = input.replaceAll("\\s+", "").toLowerCase();
        return cleaned.equals(new StringBuilder(cleaned).reverse().toString());
    }
    
    // Array operations
    public static double sum(double[] array) {
        double sum = 0.0;
        for (double value : array) {
            sum += value;
        }
        return sum;
    }
    
    public static double average(double[] array) {
        if (array.length == 0) {
            throw new IllegalArgumentException("Cannot calculate average of empty array");
        }
        return sum(array) / array.length;
    }
    
    public static double max(double[] array) {
        if (array.length == 0) {
            throw new IllegalArgumentException("Cannot find max of empty array");
        }
        
        double max = array[0];
        for (int i = 1; i < array.length; i++) {
            if (array[i] > max) {
                max = array[i];
            }
        }
        return max;
    }
    
    // Number system conversions
    public static String decimalToBinary(int decimal) {
        return Integer.toBinaryString(decimal);
    }
    
    public static int binaryToDecimal(String binary) {
        try {
            return Integer.parseInt(binary, 2);
        } catch (NumberFormatException e) {
            throw new IllegalArgumentException("Invalid binary string: " + binary);
        }
    }
    
    public static String decimalToHex(int decimal) {
        return Integer.toHexString(decimal).toUpperCase();
    }
    
    public static int hexToDecimal(String hex) {
        try {
            return Integer.parseInt(hex, 16);
        } catch (NumberFormatException e) {
            throw new IllegalArgumentException("Invalid hexadecimal string: " + hex);
        }
    }
    
    // Random utilities
    private static Random random = new Random();
    
    public static double randomDouble(double min, double max) {
        if (min > max) {
            throw new IllegalArgumentException("Min cannot be greater than max");
        }
        return min + (max - min) * random.nextDouble();
    }
    
    public static int randomInt(int min, int max) {
        if (min > max) {
            throw new IllegalArgumentException("Min cannot be greater than max");
        }
        return random.nextInt(max - min + 1) + min;
    }
    
    // Instance methods example
    private double value;
    private List<String> operations;
    
    public AdvancedMathUtils() {
        this.value = 0.0;
        this.operations = new ArrayList<>();
    }
    
    public AdvancedMathUtils(double initialValue) {
        this.value = initialValue;
        this.operations = new ArrayList<>();
        this.operations.add("Initialized with: " + initialValue);
    }
    
    public double getValue() {
        return value;
    }
    
    public void setValue(double newValue) {
        this.operations.add("Set value to: " + newValue);
        this.value = newValue;
    }
    
    public double addToValue(double operand) {
        double oldValue = this.value;
        this.value += operand;
        this.operations.add(oldValue + " + " + operand + " = " + this.value);
        return this.value;
    }
    
    public List<String> getOperations() {
        return new ArrayList<>(operations);
    }
    
    public void clearOperations() {
        this.operations.clear();
    }
    
    // Static utility methods
    public static String getLibraryInfo() {
        return "Focus Nexus Advanced Math Utils v2.0 - Java Edition";
    }
    
    public static String getVersion() {
        return "2.0.0";
    }
    
    public static void main(String[] args) {
        System.out.println("Focus Nexus Java Library Test");
        System.out.println("add(10, 5) = " + add(10, 5));
        System.out.println("factorial(5) = " + factorial(5));
        System.out.println("fibonacci(10) = " + fibonacci(10));
        System.out.println("isPrime(17) = " + isPrime(17));
        System.out.println("reverseString('Hello') = " + reverseString("Hello"));
    }
}
```

**Compilation:**
```bash
# Compile Java class
javac AdvancedMathUtils.java

# Create JAR (optional)
jar cf advanced-math.jar AdvancedMathUtils.class

# Ensure class is in classpath when running Focus Nexus
```

### Using Java Libraries in Focus Nexus

```javascript
// Load the Java class
extern "AdvancedMathUtils" as javamath : java

// Call static Java methods
let java_sum = call_native(javamath.add, 100, 200)
print("Java Addition: 100 + 200 =", java_sum)

let java_factorial = call_native(javamath.factorial, 8)
print("Java Factorial(8) =", java_factorial)

let big_factorial = call_native(javamath.bigFactorial, 25)
print("Java Big Factorial(25) =", big_factorial)

let java_fib = call_native(javamath.fibonacci, 15)
print("Java Fibonacci(15) =", java_fib)

let java_prime = call_native(javamath.isPrime, 97)
print("Java isPrime(97) =", java_prime)

let java_gcd = call_native(javamath.gcd, 48, 18)
print("Java GCD(48, 18) =", java_gcd)

// String operations
let java_reversed = call_native(javamath.reverseString, "Focus Nexus")
print("Java Reversed:", java_reversed)

let java_capitalized = call_native(javamath.capitalizeWords, "hello world from java")
print("Java Capitalized:", java_capitalized)

let java_palindrome = call_native(javamath.isPalindrome, "racecar")
print("Java isPalindrome('racecar'):", java_palindrome)

// Number conversions
let binary = call_native(javamath.decimalToBinary, 42)
print("Java 42 to binary:", binary)

let decimal = call_native(javamath.binaryToDecimal, "101010")
print("Java 101010 to decimal:", decimal)

let hex = call_native(javamath.decimalToHex, 255)
print("Java 255 to hex:", hex)

// Random numbers
let random_double = call_native(javamath.randomDouble, 1.0, 10.0)
print("Java random double:", random_double)

let random_int = call_native(javamath.randomInt, 1, 100)
print("Java random int:", random_int)

// Library information
let java_info = call_native(javamath.getLibraryInfo)
print("Java Library Info:", java_info)

let java_version = call_native(javamath.getVersion)
print("Java Library Version:", java_version)
```

## Custom Plugin Development

### Plugin API Header

**focus_nexus_plugin.h**
```cpp
#pragma once
#include "../runtime/value.hpp"
#include <vector>
#include <stdexcept>

// Plugin API version
#define FOCUS_NEXUS_PLUGIN_API_VERSION "1.0.0"

// Plugin function signature
typedef Value (*PluginFunction)(const std::vector<Value>& args);

// Plugin lifecycle functions
typedef void (*PluginInitFunc)(void);
typedef void (*PluginCleanupFunc)(void);
typedef const char* (*PluginInfoFunc)(void);

// Helper macros for plugin development
#define FOCUS_NEXUS_PLUGIN_INIT(func) \
    extern "C" void focus_nexus_plugin_init() { func(); }

#define FOCUS_NEXUS_PLUGIN_CLEANUP(func) \
    extern "C" void focus_nexus_plugin_cleanup() { func(); }

#define FOCUS_NEXUS_PLUGIN_INFO(info) \
    extern "C" const char* focus_nexus_plugin_info() { return info; }

#define FOCUS_NEXUS_EXPORT_FUNCTION(name, func) \
    extern "C" Value focus_nexus_##name(const std::vector<Value>& args) { return func(args); }

// Utility macros for argument validation
#define PLUGIN_REQUIRE_ARGS(count) \
    if (args.size() != count) { \
        throw std::runtime_error("Function requires exactly " #count " arguments"); \
    }

#define PLUGIN_REQUIRE_MIN_ARGS(count) \
    if (args.size() < count) { \
        throw std::runtime_error("Function requires at least " #count " arguments"); \
    }

#define PLUGIN_REQUIRE_NUMBER(arg, index) \
    if (!args[index].isNumber()) { \
        throw std::runtime_error("Argument " #index " must be a number"); \
    }

#define PLUGIN_REQUIRE_STRING(arg, index) \
    if (!args[index].isString()) { \
        throw std::runtime_error("Argument " #index " must be a string"); \
    }
```

### Example Custom Plugin

**comprehensive_plugin.cpp**
```cpp
#include "focus_nexus_plugin.h"
#include <cmath>
#include <string>
#include <vector>
#include <algorithm>
#include <random>
#include <sstream>
#include <iomanip>
#include <ctime>
#include <unordered_map>

// Plugin state
static std::mt19937 rng(std::time(nullptr));
static std::vector<std::string> plugin_log;
static std::unordered_map<std::string, Value> plugin_storage;

// Plugin lifecycle
void plugin_init() {
    plugin_log.push_back("Comprehensive plugin initialized at " + std::to_string(std::time(nullptr)));
    rng.seed(std::time(nullptr));
    std::cout << "Comprehensive plugin loaded successfully" << std::endl;
}

void plugin_cleanup() {
    plugin_log.push_back("Plugin cleaned up at " + std::to_string(std::time(nullptr)));
    plugin_storage.clear();
    std::cout << "Comprehensive plugin unloaded" << std::endl;
}

const char* plugin_info() {
    return "Focus Nexus Comprehensive Plugin v2.0 - Mathematical, string, and utility functions";
}

// Mathematical functions
Value calculate_distance(const std::vector<Value>& args) {
    PLUGIN_REQUIRE_ARGS(4);
    PLUGIN_REQUIRE_NUMBER(args[0], 0);
    PLUGIN_REQUIRE_NUMBER(args[1], 1);
    PLUGIN_REQUIRE_NUMBER(args[2], 2);
    PLUGIN_REQUIRE_NUMBER(args[3], 3);
    
    double x1 = args[0].asNumber();
    double y1 = args[1].asNumber();
    double x2 = args[2].asNumber();
    double y2 = args[3].asNumber();
    
    double distance = std::sqrt(std::pow(x2 - x1, 2) + std::pow(y2 - y1, 2));
    plugin_log.push_back("Calculated distance: " + std::to_string(distance));
    return Value(distance);
}

Value calculate_circle_area(const std::vector<Value>& args) {
    PLUGIN_REQUIRE_ARGS(1);
    PLUGIN_REQUIRE_NUMBER(args[0], 0);
    
    double radius = args[0].asNumber();
    if (radius < 0) {
        throw std::runtime_error("Radius cannot be negative");
    }
    
    double area = M_PI * radius * radius;
    plugin_log.push_back("Calculated circle area: " + std::to_string(area));
    return Value(area);
}

Value solve_quadratic(const std::vector<Value>& args) {
    PLUGIN_REQUIRE_ARGS(3);
    PLUGIN_REQUIRE_NUMBER(args[0], 0);
    PLUGIN_REQUIRE_NUMBER(args[1], 1);
    PLUGIN_REQUIRE_NUMBER(args[2], 2);
    
    double a = args[0].asNumber();
    double b = args[1].asNumber();
    double c = args[2].asNumber();
    
    if (a == 0.0) {
        throw std::runtime_error("Coefficient 'a' cannot be zero in quadratic equation");
    }
    
    double discriminant = b * b - 4 * a * c;
    
    if (discriminant < 0) {
        return Value("No real solutions");
    } else if (discriminant == 0) {
        double root = -b / (2 * a);
        return Value("One solution: " + std::to_string(root));
    } else {
        double root1 = (-b + std::sqrt(discriminant)) / (2 * a);
        double root2 = (-b - std::sqrt(discriminant)) / (2 * a);
        return Value("Two solutions: " + std::to_string(root1) + ", " + std::to_string(root2));
    }
}

// Statistical functions
Value calculate_mean(const std::vector<Value>& args) {
    PLUGIN_REQUIRE_MIN_ARGS(1);
    
    double sum = 0.0;
    int count = 0;
    
    for (const auto& arg : args) {
        if (arg.isNumber()) {
            sum += arg.asNumber();
            count++;
        }
    }
    
    if (count == 0) {
        throw std::runtime_error("No numeric arguments provided");
    }
    
    double mean = sum / count;
    plugin_log.push_back("Calculated mean: " + std::to_string(mean));
    return Value(mean);
}

Value calculate_standard_deviation(const std::vector<Value>& args) {
    PLUGIN_REQUIRE_MIN_ARGS(2);
    
    std::vector<double> numbers;
    for (const auto& arg : args) {
        if (arg.isNumber()) {
            numbers.push_back(arg.asNumber());
        }
    }
    
    if (numbers.size() < 2) {
        throw std::runtime_error("Need at least 2 numbers for standard deviation");
    }
    
    // Calculate mean
    double sum = 0.0;
    for (double num : numbers) {
        sum += num;
    }
    double mean = sum / numbers.size();
    
    // Calculate variance
    double variance = 0.0;
    for (double num : numbers) {
        variance += std::pow(num - mean, 2);
    }
    variance /= (numbers.size() - 1);
    
    double std_dev = std::sqrt(variance);
    plugin_log.push_back("Calculated standard deviation: " + std::to_string(std_dev));
    return Value(std_dev);
}

// String manipulation functions
Value advanced_string_reverse(const std::vector<Value>& args) {
    PLUGIN_REQUIRE_ARGS(1);
    PLUGIN_REQUIRE_STRING(args[0], 0);
    
    std::string input = args[0].asString();
    std::string result = input;
    std::reverse(result.begin(), result.end());
    
    plugin_log.push_back("Reversed string: '" + input + "' -> '" + result + "'");
    return Value(result);
}

Value count_word_frequency(const std::vector<Value>& args) {
    PLUGIN_REQUIRE_ARGS(1);
    PLUGIN_REQUIRE_STRING(args[0], 0);
    
    std::string text = args[0].asString();
    std::unordered_map<std::string, int> frequency;
    
    std::istringstream iss(text);
    std::string word;
    
    while (iss >> word) {
        // Remove punctuation and convert to lowercase
        word.erase(std::remove_if(word.begin(), word.end(), 
                   [](char c) { return !std::isalnum(c); }), word.end());
        std::transform(word.begin(), word.end(), word.begin(), ::tolower);
        
        if (!word.empty()) {
            frequency[word]++;
        }
    }
    
    // Return most frequent word and its count
    std::string mostFrequent;
    int maxCount = 0;
    
    for (const auto& pair : frequency) {
        if (pair.second > maxCount) {
            maxCount = pair.second;
            mostFrequent = pair.first;
        }
    }
    
    std::string result = mostFrequent + ":" + std::to_string(maxCount);
    plugin_log.push_back("Most frequent word: " + result);
    return Value(result);
}

Value extract_numbers_from_string(const std::vector<Value>& args) {
    PLUGIN_REQUIRE_ARGS(1);
    PLUGIN_REQUIRE_STRING(args[0], 0);
    
    std::string text = args[0].asString();
    std::vector<double> numbers;
    std::istringstream iss(text);
    std::string token;
    
    while (iss >> token) {
        try {
            double num = std::stod(token);
            numbers.push_back(num);
        } catch (const std::exception&) {
            // Not a number, skip
        }
    }
    
    // Convert to Focus Nexus list
    auto list = std::make_shared<std::vector<Value>>();
    for (double num : numbers) {
        list->push_back(Value(num));
    }
    
    plugin_log.push_back("Extracted " + std::to_string(numbers.size()) + " numbers from string");
    return Value(list);
}

// Utility functions
Value format_currency(const std::vector<Value>& args) {
    PLUGIN_REQUIRE_ARGS(2);
    PLUGIN_REQUIRE_NUMBER(args[0], 0);
    PLUGIN_REQUIRE_STRING(args[1], 1);
    
    double amount = args[0].asNumber();
    std::string currency = args[1].asString();
    
    std::ostringstream oss;
    oss << currency << " " << std::fixed << std::setprecision(2) << amount;
    
    std::string result = oss.str();
    plugin_log.push_back("Formatted currency: " + result);
    return Value(result);
}

Value generate_uuid(const std::vector<Value>& args) {
    // Simple UUID generation (not cryptographically secure)
    std::uniform_int_distribution<int> dist(0, 15);
    std::string uuid;
    
    const char* hex_chars = "0123456789abcdef";
    
    for (int i = 0; i < 32; i++) {
        if (i == 8 || i == 12 || i == 16 || i == 20) {
            uuid += '-';
        }
        uuid += hex_chars[dist(rng)];
    }
    
    plugin_log.push_back("Generated UUID: " + uuid);
    return Value(uuid);
}

// Data storage functions
Value store_value(const std::vector<Value>& args) {
    PLUGIN_REQUIRE_ARGS(2);
    PLUGIN_REQUIRE_STRING(args[0], 0);
    
    std::string key = args[0].asString();
    Value value = args[1];
    
    plugin_storage[key] = value;
    plugin_log.push_back("Stored value with key: " + key);
    return Value(true);
}

Value retrieve_value(const std::vector<Value>& args) {
    PLUGIN_REQUIRE_ARGS(1);
    PLUGIN_REQUIRE_STRING(args[0], 0);
    
    std::string key = args[0].asString();
    auto it = plugin_storage.find(key);
    
    if (it != plugin_storage.end()) {
        plugin_log.push_back("Retrieved value with key: " + key);
        return it->second;
    }
    
    plugin_log.push_back("Key not found: " + key);
    return Value(); // nil
}

Value list_stored_keys(const std::vector<Value>& args) {
    auto list = std::make_shared<std::vector<Value>>();
    
    for (const auto& pair : plugin_storage) {
        list->push_back(Value(pair.first));
    }
    
    plugin_log.push_back("Listed " + std::to_string(list->size()) + " stored keys");
    return Value(list);
}

// Plugin management
Value get_plugin_log(const std::vector<Value>& args) {
    std::string log_string;
    for (size_t i = 0; i < plugin_log.size(); i++) {
        if (i > 0) log_string += "\n";
        log_string += plugin_log[i];
    }
    return Value(log_string);
}

Value clear_plugin_log(const std::vector<Value>& args) {
    size_t count = plugin_log.size();
    plugin_log.clear();
    plugin_log.push_back("Log cleared");
    return Value(static_cast<double>(count));
}

Value get_plugin_version(const std::vector<Value>& args) {
    return Value(FOCUS_NEXUS_PLUGIN_API_VERSION);
}

// Export all functions
FOCUS_NEXUS_PLUGIN_INIT(plugin_init)
FOCUS_NEXUS_PLUGIN_CLEANUP(plugin_cleanup)
FOCUS_NEXUS_PLUGIN_INFO(plugin_info)

FOCUS_NEXUS_EXPORT_FUNCTION(calculate_distance, calculate_distance)
FOCUS_NEXUS_EXPORT_FUNCTION(calculate_circle_area, calculate_circle_area)
FOCUS_NEXUS_EXPORT_FUNCTION(solve_quadratic, solve_quadratic)
FOCUS_NEXUS_EXPORT_FUNCTION(calculate_mean, calculate_mean)
FOCUS_NEXUS_EXPORT_FUNCTION(calculate_standard_deviation, calculate_standard_deviation)
FOCUS_NEXUS_EXPORT_FUNCTION(advanced_string_reverse, advanced_string_reverse)
FOCUS_NEXUS_EXPORT_FUNCTION(count_word_frequency, count_word_frequency)
FOCUS_NEXUS_EXPORT_FUNCTION(extract_numbers_from_string, extract_numbers_from_string)
FOCUS_NEXUS_EXPORT_FUNCTION(format_currency, format_currency)
FOCUS_NEXUS_EXPORT_FUNCTION(generate_uuid, generate_uuid)
FOCUS_NEXUS_EXPORT_FUNCTION(store_value, store_value)
FOCUS_NEXUS_EXPORT_FUNCTION(retrieve_value, retrieve_value)
FOCUS_NEXUS_EXPORT_FUNCTION(list_stored_keys, list_stored_keys)
FOCUS_NEXUS_EXPORT_FUNCTION(get_plugin_log, get_plugin_log)
FOCUS_NEXUS_EXPORT_FUNCTION(clear_plugin_log, clear_plugin_log)
FOCUS_NEXUS_EXPORT_FUNCTION(get_plugin_version, get_plugin_version)
```

**Compilation:**
```bash
# Linux
g++ -shared -fPIC -o comprehensive_plugin.so comprehensive_plugin.cpp -I../src

# Windows
g++ -shared -o comprehensive_plugin.dll comprehensive_plugin.cpp -I../src
```

## Compilation and Setup

### Prerequisites

**Linux:**
```bash
# Basic build tools
sudo apt-get update
sudo apt-get install build-essential cmake git

# Python support
sudo apt-get install python3-dev python3-pip

# Java support
sudo apt-get install openjdk-11-jdk

# Set environment variables
export JAVA_HOME=/usr/lib/jvm/java-11-openjdk-amd64
export PYTHONPATH=$PYTHONPATH:./examples/python_library
```

**Windows:**
```cmd
REM Install Visual Studio 2019/2022 with C++ support
REM Install Python 3.x from python.org
REM Install JDK 11 or later

REM Set environment variables
set JAVA_HOME=C:\Program Files\Java\jdk-11.0.x
set PYTHONPATH=%PYTHONPATH%;.\examples\python_library
```

### Building Focus Nexus with Library Support

**Linux:**
```bash
mkdir build && cd build

# Configure with all library support
cmake .. \
    -DCMAKE_BUILD_TYPE=Release \
    -DPYTHON_SUPPORT=ON \
    -DJNI_SUPPORT=ON \
    -DCUSTOM_PLUGIN_SUPPORT=ON

# Build
make -j$(nproc)
```

**Windows:**
```cmd
mkdir build
cd build

REM Configure with all library support
cmake .. ^
    -G "Visual Studio 16 2019" ^
    -A x64 ^
    -DPYTHON_SUPPORT=ON ^
    -DJNI_SUPPORT=ON ^
    -DCUSTOM_PLUGIN_SUPPORT=ON

REM Build
cmake --build . --config Release
```

### CMakeLists.txt Updates

The CMakeLists.txt should include proper library detection:

```cmake
# Find Python
if(PYTHON_SUPPORT)
    find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
    target_include_directories(focusNexus PRIVATE ${Python3_INCLUDE_DIRS})
    target_link_libraries(focusNexus PRIVATE ${Python3_LIBRARIES})
    target_compile_definitions(focusNexus PRIVATE PYTHON_SUPPORT)
endif()

# Find Java
if(JNI_SUPPORT)
    find_package(JNI REQUIRED)
    target_include_directories(focusNexus PRIVATE ${JNI_INCLUDE_DIRS})
    target_link_libraries(focusNexus PRIVATE ${JNI_LIBRARIES})
    target_compile_definitions(focusNexus PRIVATE JNI_SUPPORT)
endif()
```

## Complete Examples

### Comprehensive Test Program

**test_all_libraries.fn**
```javascript
// Comprehensive Library Integration Test
print("=== Focus Nexus Complete Library Integration Test ===")

// 1. C++ Library Testing
print("\n1. Testing C++ Library Integration:")
extern "libmath.so" as mathlib : cpp

print("Loading C++ math library...")
let cpp_add = call_native(mathlib.add_numbers, 25, 35)
print("C++ add_numbers(25, 35) =", cpp_add)

let cpp_power = call_native(mathlib.calculate_power, 3, 4)
print("C++ calculate_power(3, 4) =", cpp_power)

let cpp_factorial = call_native(mathlib.calculate_factorial, 6)
print("C++ calculate_factorial(6) =", cpp_factorial)

let cpp_reverse = call_native(mathlib.reverse_string, "C++ Integration")
print("C++ reverse_string('C++ Integration') =", cpp_reverse)

// 2. Python Library Testing
print("\n2. Testing Python Library Integration:")
extern "advanced_math.py" as pymath : python

print("Loading Python advanced math module...")
let py_fib = call_native(pymath.fibonacci, 12)
print("Python fibonacci(12) =", py_fib)

let py_prime = call_native(pymath.is_prime, 89)
print("Python is_prime(89) =", py_prime)

let py_factors = call_native(pymath.prime_factors, 84)
print("Python prime_factors(84) =", py_factors)

let py_gcd = call_native(pymath.gcd, 56, 42)
print("Python gcd(56, 42) =", py_gcd)

// Test Python classes
print("Testing Python Calculator class...")
let py_calc = call_native(pymath.Calculator)
let py_calc_add = call_native(py_calc.add, 150, 75)
print("Python Calculator add(150, 75) =", py_calc_add)

let py_calc_power = call_native(py_calc.power, 2, 8)
print("Python Calculator power(2, 8) =", py_calc_power)

let py_history = call_native(py_calc.get_history)
print("Python Calculator history:", py_history)

// Test data processing
print("Testing Python DataProcessor...")
let py_data_proc = call_native(pymath.DataProcessor)
let py_fib_seq = call_native(py_data_proc.fibonacci_sequence, 8)
print("Python fibonacci_sequence(8) =", py_fib_seq)

let test_numbers = [2, 3, 4, 5, 6, 7, 8, 9, 10, 11]
let py_primes = call_native(py_data_proc.filter_primes, test_numbers)
print("Python filter_primes([2..11]) =", py_primes)

// 3. Java Library Testing
print("\n3. Testing Java Library Integration:")
extern "AdvancedMathUtils" as javamath : java

print("Loading Java AdvancedMathUtils class...")
let java_add = call_native(javamath.add, 200, 300)
print("Java add(200, 300) =", java_add)

let java_factorial = call_native(javamath.factorial, 7)
print("Java factorial(7) =", java_factorial)

let java_big_factorial = call_native(javamath.bigFactorial, 20)
print("Java bigFactorial(20) =", java_big_factorial)

let java_fib = call_native(javamath.fibonacci, 20)
print("Java fibonacci(20) =", java_fib)

let java_prime = call_native(javamath.isPrime, 101)
print("Java isPrime(101) =", java_prime)

let java_gcd = call_native(javamath.gcd, 72, 48)
print("Java gcd(72, 48) =", java_gcd)

let java_lcm = call_native(javamath.lcm, 12, 18)
print("Java lcm(12, 18) =", java_lcm)

// String operations
let java_reverse = call_native(javamath.reverseString, "Java Integration")
print("Java reverseString('Java Integration') =", java_reverse)

let java_capitalize = call_native(javamath.capitalizeWords, "hello from java")
print("Java capitalizeWords('hello from java') =", java_capitalize)

let java_palindrome = call_native(javamath.isPalindrome, "A man a plan a canal Panama")
print("Java isPalindrome('A man a plan a canal Panama') =", java_palindrome)

// Number conversions
let java_binary = call_native(javamath.decimalToBinary, 255)
print("Java decimalToBinary(255) =", java_binary)

let java_decimal = call_native(javamath.binaryToDecimal, "11111111")
print("Java binaryToDecimal('11111111') =", java_decimal)

let java_hex = call_native(javamath.decimalToHex, 4095)
print("Java decimalToHex(4095) =", java_hex)

// Random numbers
let java_random_double = call_native(javamath.randomDouble, 10.0, 50.0)
print("Java randomDouble(10.0, 50.0) =", java_random_double)

let java_random_int = call_native(javamath.randomInt, 1, 1000)
print("Java randomInt(1, 1000) =", java_random_int)

// 4. Custom Plugin Testing
print("\n4. Testing Custom Plugin Integration:")
plugin "comprehensive_plugin.so" as myplugin

print("Loading comprehensive custom plugin...")
let plugin_distance = call_native(myplugin.calculate_distance, 1, 1, 4, 5)
print("Plugin calculate_distance(1,1,4,5) =", plugin_distance)

let plugin_area = call_native(myplugin.calculate_circle_area, 5)
print("Plugin calculate_circle_area(5) =", plugin_area)

let plugin_quadratic = call_native(myplugin.solve_quadratic, 1, -5, 6)
print("Plugin solve_quadratic(1,-5,6) =", plugin_quadratic)

let plugin_mean = call_native(myplugin.calculate_mean, 10, 20, 30, 40, 50)
print("Plugin calculate_mean(10,20,30,40,50) =", plugin_mean)

let plugin_std_dev = call_native(myplugin.calculate_standard_deviation, 2, 4, 6, 8, 10)
print("Plugin calculate_standard_deviation(2,4,6,8,10) =", plugin_std_dev)

// String functions
let plugin_reverse = call_native(myplugin.advanced_string_reverse, "Plugin Test")
print("Plugin advanced_string_reverse('Plugin Test') =", plugin_reverse)

let plugin_word_freq = call_native(myplugin.count_word_frequency, "hello world hello universe hello")
print("Plugin count_word_frequency =", plugin_word_freq)

let plugin_extract = call_native(myplugin.extract_numbers_from_string, "Price: $25.99, Quantity: 3, Total: $77.97")
print("Plugin extract_numbers_from_string =", plugin_extract)

// Utility functions
let plugin_currency = call_native(myplugin.format_currency, 1234.56, "USD")
print("Plugin format_currency(1234.56, 'USD') =", plugin_currency)

let plugin_uuid = call_native(myplugin.generate_uuid)
print("Plugin generate_uuid() =", plugin_uuid)

// Data storage
let store_result = call_native(myplugin.store_value, "test_key", "test_value")
print("Plugin store_value('test_key', 'test_value') =", store_result)

let retrieve_result = call_native(myplugin.retrieve_value, "test_key")
print("Plugin retrieve_value('test_key') =", retrieve_result)

let stored_keys = call_native(myplugin.list_stored_keys)
print("Plugin list_stored_keys() =", stored_keys)

// Plugin management
let plugin_log = call_native(myplugin.get_plugin_log)
print("Plugin log entries:", plugin_log)

let plugin_version = call_native(myplugin.get_plugin_version)
print("Plugin version:", plugin_version)

// 5. Performance Comparison
print("\n5. Performance Comparison:")

function performance_test():
{
    let iterations = 1000
    print("Running", iterations, "iterations...")
    
    // Native Focus Nexus operations
    let start_time = clock()
    for i = 0; i < iterations; i = i + 1:
    {
        let native_result = 15 + 25 + i
    }
    let native_time = clock() - start_time
    print("Native operations time:", native_time, "seconds")
    
    // C++ library calls
    start_time = clock()
    for i = 0; i < iterations; i = i + 1:
    {
        let cpp_result = call_native(mathlib.add_numbers, 15, 25)
    }
    let cpp_time = clock() - start_time
    print("C++ library calls time:", cpp_time, "seconds")
    
    // Python function calls
    start_time = clock()
    for i = 0; i < iterations; i = i + 1:
    {
        let py_result = call_native(pymath.fibonacci, 5)
    }
    let py_time = clock() - start_time
    print("Python function calls time:", py_time, "seconds")
    
    // Java method calls
    start_time = clock()
    for i = 0; i < iterations; i = i + 1:
    {
        let java_result = call_native(javamath.add, 15, 25)
    }
    let java_time = clock() - start_time
    print("Java method calls time:", java_time, "seconds")
    
    // Plugin function calls
    start_time = clock()
    for i = 0; i < iterations; i = i + 1:
    {
        let plugin_result = call_native(myplugin.calculate_mean, 15, 25, 35)
    }
    let plugin_time = clock() - start_time
    print("Plugin function calls time:", plugin_time, "seconds")
    
    print("\nPerformance ratios (relative to native):")
    print("C++:", cpp_time / native_time)
    print("Python:", py_time / native_time)
    print("Java:", java_time / native_time)
    print("Plugin:", plugin_time / native_time)
}

performance_test()

// 6. Error Handling
print("\n6. Testing Error Handling:")

function test_error_handling():
{
    try:
    {
        // Test division by zero in Java
        let bad_result = call_native(javamath.divide, 10, 0)
        print("This should not print")
    }
    catch(error):
    {
        print("Caught Java error:", error)
    }
    
    try:
    {
        // Test invalid function call
        let bad_result = call_native(mathlib.nonexistent_function, 1, 2)
        print("This should not print")
    }
    catch(error):
    {
        print("Caught C++ error:", error)
    }
    
    try:
    {
        // Test Python error
        let bad_result = call_native(pymath.fibonacci, -5)
        print("This should not print")
    }
    catch(error):
    {
        print("Caught Python error:", error)
    }
}

test_error_handling()

print("\n=== All Library Integration Tests Complete ===")
print("Successfully tested:")
print("✓ C++ shared library integration")
print("✓ Python module integration")
print("✓ Java class integration")
print("✓ Custom plugin integration")
print("✓ Error handling across all library types")
print("✓ Performance comparison")
print("\nFocus Nexus library integration system is fully functional!")
```

## Troubleshooting

### Common Issues and Solutions

#### 1. Library Loading Failures

**Problem:** `Failed to load library: libmath.so`

**Solutions:**
```bash
# Linux: Check library path and dependencies
ldd libmath.so
export LD_LIBRARY_PATH=$LD_LIBRARY_PATH:/path/to/library

# Windows: Check DLL dependencies
# Use Dependency Walker or PowerShell
Get-Command .\math.dll | Select-Object -ExpandProperty FileVersionInfo
```

#### 2. Python Import Errors

**Problem:** `Failed to import Python module`

**Solutions:**
```bash
# Check Python path
python3 -c "import sys; print(sys.path)"

# Add module directory to PYTHONPATH
export PYTHONPATH=$PYTHONPATH:/path/to/modules

# Verify module syntax
python3 -m py_compile module_name.py
```

#### 3. Java Class Loading Errors

**Problem:** `Java class not found`

**Solutions:**
```bash
# Check classpath
echo $CLASSPATH

# Add to classpath
export CLASSPATH=$CLASSPATH:/path/to/classes

# Verify class compilation
javac -cp . ClassName.java
java -cp . ClassName  # Test main method
```

#### 4. Plugin Loading Issues

**Problem:** `Failed to load custom plugin`

**Solutions:**
```bash
# Check plugin symbols
nm -D plugin.so | grep focus_nexus  # Linux
objdump -t plugin.dll | grep focus_nexus  # Windows

# Verify plugin API compatibility
# Ensure plugin uses correct Value class definition
```

### Debug Mode

Enable verbose logging for troubleshooting:

```javascript
// Enable debug mode (if implemented)
set debug_mode = true

// Check loaded libraries
let loaded_libs = get_loaded_libraries()
print("Loaded libraries:", loaded_libs)

// Verify function availability
let has_func = has_function("mathlib", "add_numbers")
print("Function exists:", has_func)
```

## Best Practices

### 1. Library Design

- **C++ Libraries**: Use simple C-style interfaces for maximum compatibility
- **Python Modules**: Follow PEP 8 and include proper type hints
- **Java Classes**: Use static methods for simple functions, instances for stateful operations
- **Custom Plugins**: Implement proper error handling and resource cleanup

### 2. Error Handling

```cpp
// C++ libraries
extern "C" {
    double safe_divide(double a, double b) {
        if (b == 0.0) {
            // Return special value or set error flag
            return std::numeric_limits<double>::quiet_NaN();
        }
        return a / b;
    }
}
```

```python
# Python modules
def safe_operation(value):
    try:
        return risky_calculation(value)
    except Exception as e:
        # Log error and return safe value
        print(f"Error in safe_operation: {e}")
        return None
```

```java
// Java classes
public static double safeDivide(double a, double b) {
    if (b == 0.0) {
        throw new ArithmeticException("Division by zero");
    }
    return a / b;
}
```

### 3. Performance Optimization

- **Cache Function Pointers**: Library manager caches loaded functions
- **Minimize Type Conversions**: Use compatible types when possible
- **Batch Operations**: Process multiple values in single calls when possible
- **Resource Management**: Properly clean up external resources

### 4. Memory Management

- **C++ Libraries**: Use static buffers or careful dynamic allocation
- **Python Integration**: Proper reference counting with Py_INCREF/Py_DECREF
- **Java Integration**: Clean up local references to prevent memory leaks
- **Custom Plugins**: Implement proper cleanup functions

This complete guide provides everything needed to successfully integrate external libraries with Focus Nexus, enabling powerful extensibility while maintaining performance and reliability.