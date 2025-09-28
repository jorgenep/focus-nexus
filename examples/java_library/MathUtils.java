/**
 * Example Java class for Focus Nexus integration
 * This class provides mathematical and utility functions
 * 
 * Compile with: javac MathUtils.java
 * Create JAR: jar cf mathutils.jar MathUtils.class
 */

import java.util.*;
import java.math.BigInteger;
import java.math.BigDecimal;
import java.math.RoundingMode;

public class MathUtils {
    
    // Static mathematical functions
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
    
    public static double power(double base, double exponent) {
        return Math.pow(base, exponent);
    }
    
    public static double sqrt(double value) {
        if (value < 0) {
            throw new IllegalArgumentException("Cannot calculate square root of negative number");
        }
        return Math.sqrt(value);
    }
    
    // Advanced mathematical functions
    public static long factorial(int n) {
        if (n < 0) {
            throw new IllegalArgumentException("Factorial cannot be calculated for negative numbers");
        }
        if (n <= 1) {
            return 1;
        }
        
        long result = 1;
        for (int i = 2; i <= n; i++) {
            result *= i;
        }
        return result;
    }
    
    public static BigInteger bigFactorial(int n) {
        if (n < 0) {
            throw new IllegalArgumentException("Factorial cannot be calculated for negative numbers");
        }
        
        BigInteger result = BigInteger.ONE;
        for (int i = 2; i <= n; i++) {
            result = result.multiply(BigInteger.valueOf(i));
        }
        return result;
    }
    
    public static long fibonacci(int n) {
        if (n < 0) {
            throw new IllegalArgumentException("Fibonacci number cannot be negative");
        }
        if (n <= 1) {
            return n;
        }
        
        long a = 0, b = 1, temp;
        for (int i = 2; i <= n; i++) {
            temp = a + b;
            a = b;
            b = temp;
        }
        return b;
    }
    
    public static boolean isPrime(int n) {
        if (n < 2) {
            return false;
        }
        if (n == 2) {
            return true;
        }
        if (n % 2 == 0) {
            return false;
        }
        
        for (int i = 3; i <= Math.sqrt(n); i += 2) {
            if (n % i == 0) {
                return false;
            }
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
        if (a == 0 || b == 0) {
            return 0;
        }
        return Math.abs(a * b) / gcd(a, b);
    }
    
    public static List<Integer> primeFactors(int n) {
        List<Integer> factors = new ArrayList<>();
        
        // Handle factor 2
        while (n % 2 == 0) {
            factors.add(2);
            n /= 2;
        }
        
        // Handle odd factors
        for (int i = 3; i <= Math.sqrt(n); i += 2) {
            while (n % i == 0) {
                factors.add(i);
                n /= i;
            }
        }
        
        // If n is still greater than 2, it's a prime
        if (n > 2) {
            factors.add(n);
        }
        
        return factors;
    }
    
    public static boolean isPerfectSquare(int n) {
        if (n < 0) {
            return false;
        }
        int sqrt = (int) Math.sqrt(n);
        return sqrt * sqrt == n;
    }
    
    public static boolean isPerfectNumber(int n) {
        if (n <= 1) {
            return false;
        }
        
        int sum = 1; // 1 is always a proper divisor
        for (int i = 2; i <= Math.sqrt(n); i++) {
            if (n % i == 0) {
                sum += i;
                if (i != n / i) { // Avoid counting square root twice
                    sum += n / i;
                }
            }
        }
        
        return sum == n;
    }
    
    // String manipulation functions
    public static String reverseString(String input) {
        if (input == null) {
            return null;
        }
        return new StringBuilder(input).reverse().toString();
    }
    
    public static String toUpperCase(String input) {
        if (input == null) {
            return null;
        }
        return input.toUpperCase();
    }
    
    public static String toLowerCase(String input) {
        if (input == null) {
            return null;
        }
        return input.toLowerCase();
    }
    
    public static boolean isPalindrome(String input) {
        if (input == null) {
            return false;
        }
        String cleaned = input.replaceAll("\\s+", "").toLowerCase();
        return cleaned.equals(new StringBuilder(cleaned).reverse().toString());
    }
    
    public static int countVowels(String input) {
        if (input == null) {
            return 0;
        }
        
        int count = 0;
        String vowels = "aeiouAEIOU";
        for (char c : input.toCharArray()) {
            if (vowels.indexOf(c) != -1) {
                count++;
            }
        }
        return count;
    }
    
    public static String capitalizeWords(String input) {
        if (input == null || input.isEmpty()) {
            return input;
        }
        
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
    
    // Array/List utility functions
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
    
    public static double min(double[] array) {
        if (array.length == 0) {
            throw new IllegalArgumentException("Cannot find min of empty array");
        }
        
        double min = array[0];
        for (int i = 1; i < array.length; i++) {
            if (array[i] < min) {
                min = array[i];
            }
        }
        return min;
    }
    
    public static double[] sort(double[] array) {
        double[] sorted = array.clone();
        Arrays.sort(sorted);
        return sorted;
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
    
    // Random number utilities
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
    
    public static List<Integer> generateRandomList(int size, int min, int max) {
        List<Integer> list = new ArrayList<>();
        for (int i = 0; i < size; i++) {
            list.add(randomInt(min, max));
        }
        return list;
    }
    
    // Instance variables and methods
    private double value;
    private List<String> history;
    
    // Constructors
    public MathUtils() {
        this.value = 0.0;
        this.history = new ArrayList<>();
    }
    
    public MathUtils(double initialValue) {
        this.value = initialValue;
        this.history = new ArrayList<>();
        this.history.add("Initialized with value: " + initialValue);
    }
    
    // Instance methods
    public double getValue() {
        return value;
    }
    
    public void setValue(double newValue) {
        this.history.add("Changed value from " + this.value + " to " + newValue);
        this.value = newValue;
    }
    
    public double add(double operand) {
        double oldValue = this.value;
        this.value += operand;
        this.history.add(oldValue + " + " + operand + " = " + this.value);
        return this.value;
    }
    
    public double subtract(double operand) {
        double oldValue = this.value;
        this.value -= operand;
        this.history.add(oldValue + " - " + operand + " = " + this.value);
        return this.value;
    }
    
    public double multiply(double operand) {
        double oldValue = this.value;
        this.value *= operand;
        this.history.add(oldValue + " * " + operand + " = " + this.value);
        return this.value;
    }
    
    public double divide(double operand) {
        if (operand == 0.0) {
            throw new ArithmeticException("Division by zero");
        }
        double oldValue = this.value;
        this.value /= operand;
        this.history.add(oldValue + " / " + operand + " = " + this.value);
        return this.value;
    }
    
    public double power(double exponent) {
        double oldValue = this.value;
        this.value = Math.pow(this.value, exponent);
        this.history.add(oldValue + " ^ " + exponent + " = " + this.value);
        return this.value;
    }
    
    public double sqrt() {
        if (this.value < 0) {
            throw new IllegalArgumentException("Cannot calculate square root of negative number");
        }
        double oldValue = this.value;
        this.value = Math.sqrt(this.value);
        this.history.add("sqrt(" + oldValue + ") = " + this.value);
        return this.value;
    }
    
    public List<String> getHistory() {
        return new ArrayList<>(history);
    }
    
    public void clearHistory() {
        this.history.clear();
        this.history.add("History cleared");
    }
    
    public void reset() {
        this.history.add("Reset value from " + this.value + " to 0.0");
        this.value = 0.0;
    }
    
    // Utility methods
    public static String getLibraryInfo() {
        return "Focus Nexus Java Math Utils v1.0 - Comprehensive mathematical utilities";
    }
    
    public static String getVersion() {
        return "1.0.0";
    }
    
    public static List<String> getAvailableMethods() {
        return Arrays.asList(
            "add", "subtract", "multiply", "divide", "power", "sqrt",
            "factorial", "fibonacci", "isPrime", "gcd", "lcm", "primeFactors",
            "isPerfectSquare", "isPerfectNumber", "reverseString", "isPalindrome",
            "countVowels", "capitalizeWords", "sum", "average", "max", "min",
            "decimalToBinary", "binaryToDecimal", "decimalToHex", "hexToDecimal",
            "randomDouble", "randomInt", "generateRandomList"
        );
    }
    
    // Main method for testing
    public static void main(String[] args) {
        System.out.println("Focus Nexus Java Math Utils");
        System.out.println("============================");
        
        // Test static methods
        System.out.println("Static method tests:");
        System.out.println("add(10, 5) = " + add(10, 5));
        System.out.println("factorial(5) = " + factorial(5));
        System.out.println("fibonacci(10) = " + fibonacci(10));
        System.out.println("isPrime(17) = " + isPrime(17));
        System.out.println("gcd(48, 18) = " + gcd(48, 18));
        System.out.println("reverseString('Hello') = " + reverseString("Hello"));
        System.out.println("isPalindrome('racecar') = " + isPalindrome("racecar"));
        
        // Test instance methods
        System.out.println("\nInstance method tests:");
        MathUtils calc = new MathUtils(10.0);
        System.out.println("Initial value: " + calc.getValue());
        System.out.println("After add(5): " + calc.add(5));
        System.out.println("After multiply(2): " + calc.multiply(2));
        System.out.println("After sqrt(): " + calc.sqrt());
        
        System.out.println("\nHistory:");
        for (String entry : calc.getHistory()) {
            System.out.println("  " + entry);
        }
        
        // Test array operations
        System.out.println("\nArray operations:");
        double[] testArray = {1.5, 2.7, 3.1, 4.9, 5.2};
        System.out.println("Array: " + Arrays.toString(testArray));
        System.out.println("Sum: " + sum(testArray));
        System.out.println("Average: " + average(testArray));
        System.out.println("Max: " + max(testArray));
        System.out.println("Min: " + min(testArray));
        
        // Test number conversions
        System.out.println("\nNumber conversions:");
        System.out.println("42 to binary: " + decimalToBinary(42));
        System.out.println("101010 to decimal: " + binaryToDecimal("101010"));
        System.out.println("255 to hex: " + decimalToHex(255));
        System.out.println("FF to decimal: " + hexToDecimal("FF"));
    }
}