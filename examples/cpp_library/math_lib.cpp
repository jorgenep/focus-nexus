// Example C++ library for Focus Nexus integration
// Compile with: g++ -shared -fPIC -o libmath.so math_lib.cpp -lm

#include <cmath>
#include <cstring>
#include <algorithm>

extern "C" {
    // Basic arithmetic functions
    double add_numbers(double a, double b) {
        return a + b;
    }
    
    double subtract_numbers(double a, double b) {
        return a - b;
    }
    
    double multiply_numbers(double a, double b) {
        return a * b;
    }
    
    double divide_numbers(double a, double b) {
        if (b == 0.0) {
            return 0.0; // Handle division by zero
        }
        return a / b;
    }
    
    // Advanced mathematical functions
    double calculate_power(double base, double exponent) {
        return pow(base, exponent);
    }
    
    double calculate_sqrt(double value) {
        if (value < 0.0) {
            return 0.0; // Handle negative input
        }
        return sqrt(value);
    }
    
    double calculate_sin(double angle) {
        return sin(angle);
    }
    
    double calculate_cos(double angle) {
        return cos(angle);
    }
    
    double calculate_tan(double angle) {
        return tan(angle);
    }
    
    double calculate_log(double value) {
        if (value <= 0.0) {
            return 0.0; // Handle invalid input
        }
        return log(value);
    }
    
    double calculate_log10(double value) {
        if (value <= 0.0) {
            return 0.0; // Handle invalid input
        }
        return log10(value);
    }
    
    // Statistical functions
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
    
    double calculate_fibonacci(double n) {
        if (n < 0 || n != floor(n)) {
            return 0.0; // Handle invalid input
        }
        
        int num = (int)n;
        if (num <= 1) return num;
        
        double a = 0, b = 1, temp;
        for (int i = 2; i <= num; i++) {
            temp = a + b;
            a = b;
            b = temp;
        }
        return b;
    }
    
    // Array processing functions (simplified interface)
    double sum_array(double* arr, int size) {
        double sum = 0.0;
        for (int i = 0; i < size; i++) {
            sum += arr[i];
        }
        return sum;
    }
    
    double average_array(double* arr, int size) {
        if (size == 0) return 0.0;
        return sum_array(arr, size) / size;
    }
    
    double max_array(double* arr, int size) {
        if (size == 0) return 0.0;
        double max_val = arr[0];
        for (int i = 1; i < size; i++) {
            if (arr[i] > max_val) {
                max_val = arr[i];
            }
        }
        return max_val;
    }
    
    double min_array(double* arr, int size) {
        if (size == 0) return 0.0;
        double min_val = arr[0];
        for (int i = 1; i < size; i++) {
            if (arr[i] < min_val) {
                min_val = arr[i];
            }
        }
        return min_val;
    }
    
    // String manipulation functions
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
    
    const char* to_uppercase(const char* input) {
        static char result[1024];
        int len = strlen(input);
        
        if (len >= 1024) {
            strcpy(result, "String too long");
            return result;
        }
        
        for (int i = 0; i < len; i++) {
            result[i] = toupper(input[i]);
        }
        result[len] = '\0';
        
        return result;
    }
    
    const char* to_lowercase(const char* input) {
        static char result[1024];
        int len = strlen(input);
        
        if (len >= 1024) {
            strcpy(result, "String too long");
            return result;
        }
        
        for (int i = 0; i < len; i++) {
            result[i] = tolower(input[i]);
        }
        result[len] = '\0';
        
        return result;
    }
    
    // Utility functions
    double random_range(double min, double max) {
        if (min > max) {
            double temp = min;
            min = max;
            max = temp;
        }
        
        return min + (max - min) * (rand() / (double)RAND_MAX);
    }
    
    double round_to_places(double value, double places) {
        double multiplier = pow(10.0, places);
        return round(value * multiplier) / multiplier;
    }
    
    // Validation functions
    double is_prime(double n) {
        if (n < 2 || n != floor(n)) return 0.0; // false
        
        int num = (int)n;
        for (int i = 2; i <= sqrt(num); i++) {
            if (num % i == 0) return 0.0; // false
        }
        return 1.0; // true
    }
    
    double is_even(double n) {
        if (n != floor(n)) return 0.0; // false for non-integers
        return ((int)n % 2 == 0) ? 1.0 : 0.0;
    }
    
    double is_odd(double n) {
        if (n != floor(n)) return 0.0; // false for non-integers
        return ((int)n % 2 != 0) ? 1.0 : 0.0;
    }
    
    // Advanced function: solve quadratic equation
    // Returns the discriminant, roots stored in static variables
    static double root1, root2;
    
    double solve_quadratic(double a, double b, double c) {
        if (a == 0.0) {
            // Linear equation
            if (b != 0.0) {
                root1 = root2 = -c / b;
                return 0.0; // One solution
            }
            return -1.0; // No solution or infinite solutions
        }
        
        double discriminant = b * b - 4 * a * c;
        
        if (discriminant > 0) {
            root1 = (-b + sqrt(discriminant)) / (2 * a);
            root2 = (-b - sqrt(discriminant)) / (2 * a);
        } else if (discriminant == 0) {
            root1 = root2 = -b / (2 * a);
        } else {
            // Complex roots - return real part
            root1 = root2 = -b / (2 * a);
        }
        
        return discriminant;
    }
    
    double get_root1() {
        return root1;
    }
    
    double get_root2() {
        return root2;
    }
    
    // Library information
    const char* get_library_info() {
        return "Focus Nexus Math Library v1.0 - Comprehensive mathematical functions";
    }
    
    const char* get_library_version() {
        return "1.0.0";
    }
}