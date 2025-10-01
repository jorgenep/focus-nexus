/**
 * Advanced Mathematical Utilities for Focus Nexus
 * This class provides comprehensive mathematical and utility functions
 * 
 * Compile with: javac AdvancedMathUtils.java
 * Create JAR: jar cf advanced-math.jar AdvancedMathUtils.class
 */

import java.util.*;
import java.math.BigInteger;
import java.math.BigDecimal;
import java.math.RoundingMode;

public class AdvancedMathUtils {
    
    // Basic arithmetic operations
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
    
    public static double abs(double value) {
        return Math.abs(value);
    }
    
    public static double ceil(double value) {
        return Math.ceil(value);
    }
    
    public static double floor(double value) {
        return Math.floor(value);
    }
    
    public static double round(double value) {
        return Math.round(value);
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
    
    public static String bigFactorial(int n) {
        if (n < 0) {
            throw new IllegalArgumentException("Factorial cannot be calculated for negative numbers");
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
    
    public static List<Long> fibonacciSequence(int count) {
        List<Long> sequence = new ArrayList<>();
        for (int i = 0; i < count; i++) {
            sequence.add(fibonacci(i));
        }
        return sequence;
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
    
    public static boolean isArmstrongNumber(int n) {
        String str = String.valueOf(n);
        int digits = str.length();
        int sum = 0;
        
        for (char c : str.toCharArray()) {
            int digit = Character.getNumericValue(c);
            sum += Math.pow(digit, digits);
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
    
    public static int countConsonants(String input) {
        if (input == null) {
            return 0;
        }
        
        int count = 0;
        for (char c : input.toCharArray()) {
            if (Character.isLetter(c) && "aeiouAEIOU".indexOf(c) == -1) {
                count++;
            }
        }
        return count;
    }
    
    public static int countWords(String input) {
        if (input == null || input.trim().isEmpty()) {
            return 0;
        }
        return input.trim().split("\\s+").length;
    }
    
    public static String longestWord(String input) {
        if (input == null || input.trim().isEmpty()) {
            return "";
        }
        
        String[] words = input.split("\\s+");
        String longest = "";
        
        for (String word : words) {
            if (word.length() > longest.length()) {
                longest = word;
            }
        }
        
        return longest;
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
    
    public static double[] reverse(double[] array) {
        double[] reversed = new double[array.length];
        for (int i = 0; i < array.length; i++) {
            reversed[i] = array[array.length - 1 - i];
        }
        return reversed;
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
    
    public static String decimalToOctal(int decimal) {
        return Integer.toOctalString(decimal);
    }
    
    public static int octalToDecimal(String octal) {
        try {
            return Integer.parseInt(octal, 8);
        } catch (NumberFormatException e) {
            throw new IllegalArgumentException("Invalid octal string: " + octal);
        }
    }
    
    // Random number utilities
    private static Random random = new Random();
    
    public static double randomDouble() {
        return random.nextDouble();
    }
    
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
    
    public static void setSeed(long seed) {
        random.setSeed(seed);
    }
    
    // Trigonometric functions
    public static double sin(double angle) {
        return Math.sin(angle);
    }
    
    public static double cos(double angle) {
        return Math.cos(angle);
    }
    
    public static double tan(double angle) {
        return Math.tan(angle);
    }
    
    public static double asin(double value) {
        return Math.asin(value);
    }
    
    public static double acos(double value) {
        return Math.acos(value);
    }
    
    public static double atan(double value) {
        return Math.atan(value);
    }
    
    public static double atan2(double y, double x) {
        return Math.atan2(y, x);
    }
    
    // Logarithmic functions
    public static double log(double value) {
        if (value <= 0) {
            throw new IllegalArgumentException("Logarithm input must be positive");
        }
        return Math.log(value);
    }
    
    public static double log10(double value) {
        if (value <= 0) {
            throw new IllegalArgumentException("Logarithm input must be positive");
        }
        return Math.log10(value);
    }
    
    public static double exp(double value) {
        return Math.exp(value);
    }
    
    // Instance variables and methods for stateful operations
    private double value;
    private List<String> operationHistory;
    private double memory;
    
    // Constructors
    public AdvancedMathUtils() {
        this.value = 0.0;
        this.operationHistory = new ArrayList<>();
        this.memory = 0.0;
    }
    
    public AdvancedMathUtils(double initialValue) {
        this.value = initialValue;
        this.operationHistory = new ArrayList<>();
        this.memory = 0.0;
        this.operationHistory.add("Initialized with value: " + initialValue);
    }
    
    // Instance methods
    public double getValue() {
        return value;
    }
    
    public void setValue(double newValue) {
        this.operationHistory.add("Changed value from " + this.value + " to " + newValue);
        this.value = newValue;
    }
    
    public double add(double operand) {
        double oldValue = this.value;
        this.value += operand;
        this.operationHistory.add(oldValue + " + " + operand + " = " + this.value);
        return this.value;
    }
    
    public double subtract(double operand) {
        double oldValue = this.value;
        this.value -= operand;
        this.operationHistory.add(oldValue + " - " + operand + " = " + this.value);
        return this.value;
    }
    
    public double multiply(double operand) {
        double oldValue = this.value;
        this.value *= operand;
        this.operationHistory.add(oldValue + " * " + operand + " = " + this.value);
        return this.value;
    }
    
    public double divide(double operand) {
        if (operand == 0.0) {
            throw new ArithmeticException("Division by zero");
        }
        double oldValue = this.value;
        this.value /= operand;
        this.operationHistory.add(oldValue + " / " + operand + " = " + this.value);
        return this.value;
    }
    
    public double power(double exponent) {
        double oldValue = this.value;
        this.value = Math.pow(this.value, exponent);
        this.operationHistory.add(oldValue + " ^ " + exponent + " = " + this.value);
        return this.value;
    }
    
    public double sqrt() {
        if (this.value < 0) {
            throw new IllegalArgumentException("Cannot calculate square root of negative number");
        }
        double oldValue = this.value;
        this.value = Math.sqrt(this.value);
        this.operationHistory.add("sqrt(" + oldValue + ") = " + this.value);
        return this.value;
    }
    
    public List<String> getHistory() {
        return new ArrayList<>(operationHistory);
    }
    
    public void clearHistory() {
        this.operationHistory.clear();
        this.operationHistory.add("History cleared");
    }
    
    public void reset() {
        this.operationHistory.add("Reset value from " + this.value + " to 0.0");
        this.value = 0.0;
    }
    
    public void storeMemory() {
        this.memory = this.value;
        this.operationHistory.add("Stored " + this.value + " in memory");
    }
    
    public double recallMemory() {
        this.operationHistory.add("Recalled " + this.memory + " from memory");
        return this.memory;
    }
    
    public void clearMemory() {
        this.memory = 0.0;
        this.operationHistory.add("Memory cleared");
    }
    
    // Utility methods
    public static String getLibraryInfo() {
        return "Focus Nexus Advanced Math Utils v2.0 - Java Edition - Comprehensive mathematical utilities";
    }
    
    public static String getVersion() {
        return "2.0.0";
    }
    
    public static List<String> getAvailableMethods() {
        return Arrays.asList(
            // Static methods
            "add", "subtract", "multiply", "divide", "power", "sqrt", "abs", "ceil", "floor", "round",
            "factorial", "bigFactorial", "fibonacci", "fibonacciSequence", "isPrime", "primeFactors",
            "gcd", "lcm", "isPerfectSquare", "isPerfectNumber", "isArmstrongNumber",
            "reverseString", "toUpperCase", "toLowerCase", "capitalizeWords", "isPalindrome",
            "countVowels", "countConsonants", "countWords", "longestWord",
            "sum", "average", "max", "min", "sort", "reverse",
            "decimalToBinary", "binaryToDecimal", "decimalToHex", "hexToDecimal",
            "decimalToOctal", "octalToDecimal", "randomDouble", "randomInt", 
            "generateRandomList", "setSeed", "sin", "cos", "tan", "asin", "acos", 
            "atan", "atan2", "log", "log10", "exp",
            // Instance methods
            "getValue", "setValue", "getHistory", "clearHistory", "reset",
            "storeMemory", "recallMemory", "clearMemory"
        );
    }
    
    public static int getMethodCount() {
        return getAvailableMethods().size();
    }
    
    public static void printMethodList() {
        System.out.println("Available methods:");
        List<String> methods = getAvailableMethods();
        for (int i = 0; i < methods.size(); i++) {
            System.out.printf("%2d. %s%n", i + 1, methods.get(i));
        }
    }
    
    // Self-test method
    public static boolean runSelfTest() {
        try {
            // Test basic arithmetic
            assert add(10, 5) == 15.0 : "Addition test failed";
            assert subtract(10, 5) == 5.0 : "Subtraction test failed";
            assert multiply(10, 5) == 50.0 : "Multiplication test failed";
            assert divide(10, 5) == 2.0 : "Division test failed";
            
            // Test mathematical functions
            assert factorial(5) == 120 : "Factorial test failed";
            assert fibonacci(10) == 55 : "Fibonacci test failed";
            assert isPrime(17) == true : "Prime test failed";
            assert gcd(48, 18) == 6 : "GCD test failed";
            assert lcm(12, 18) == 36 : "LCM test failed";
            
            // Test string functions
            assert reverseString("hello").equals("olleh") : "String reverse test failed";
            assert countVowels("hello world") == 3 : "Vowel count test failed";
            assert capitalizeWords("hello world").equals("Hello World") : "Capitalize test failed";
            
            // Test conversions
            assert decimalToBinary(42).equals("101010") : "Binary conversion test failed";
            assert binaryToDecimal("101010") == 42 : "Binary to decimal test failed";
            assert decimalToHex(255).equals("FF") : "Hex conversion test failed";
            assert hexToDecimal("FF") == 255 : "Hex to decimal test failed";
            
            System.out.println("All self-tests passed!");
            return true;
            
        } catch (AssertionError e) {
            System.err.println("Self-test failed: " + e.getMessage());
            return false;
        } catch (Exception e) {
            System.err.println("Self-test error: " + e.getMessage());
            return false;
        }
    }
    
    // Main method for testing
    public static void main(String[] args) {
        System.out.println("Focus Nexus Advanced Math Utils");
        System.out.println("================================");
        
        // Display library information
        System.out.println("Library: " + getLibraryInfo());
        System.out.println("Version: " + getVersion());
        System.out.println("Method count: " + getMethodCount());
        System.out.println();
        
        // Run self-test
        System.out.println("Running self-test...");
        boolean testResult = runSelfTest();
        System.out.println("Self-test result: " + (testResult ? "PASSED" : "FAILED"));
        System.out.println();
        
        // Demonstrate functionality
        System.out.println("Demonstration:");
        
        // Static method tests
        System.out.println("Static method tests:");
        System.out.println("add(10, 5) = " + add(10, 5));
        System.out.println("factorial(5) = " + factorial(5));
        System.out.println("fibonacci(10) = " + fibonacci(10));
        System.out.println("isPrime(17) = " + isPrime(17));
        System.out.println("gcd(48, 18) = " + gcd(48, 18));
        System.out.println("reverseString('Hello') = " + reverseString("Hello"));
        System.out.println("isPalindrome('racecar') = " + isPalindrome("racecar"));
        System.out.println("decimalToBinary(42) = " + decimalToBinary(42));
        System.out.println();
        
        // Instance method tests
        System.out.println("Instance method tests:");
        AdvancedMathUtils calc = new AdvancedMathUtils(10.0);
        System.out.println("Initial value: " + calc.getValue());
        System.out.println("After add(5): " + calc.add(5));
        System.out.println("After multiply(2): " + calc.multiply(2));
        System.out.println("After sqrt(): " + calc.sqrt());
        
        System.out.println("\nOperation History:");
        for (String entry : calc.getHistory()) {
            System.out.println("  " + entry);
        }
        
        // Array operations
        System.out.println("\nArray operations:");
        double[] testArray = {1.5, 2.7, 3.1, 4.9, 5.2};
        System.out.println("Array: " + Arrays.toString(testArray));
        System.out.println("Sum: " + sum(testArray));
        System.out.println("Average: " + average(testArray));
        System.out.println("Max: " + max(testArray));
        System.out.println("Min: " + min(testArray));
        
        // Random number generation
        System.out.println("\nRandom number generation:");
        setSeed(12345); // Set seed for reproducible results
        System.out.println("Random double (0-1): " + randomDouble());
        System.out.println("Random double (10-50): " + randomDouble(10, 50));
        System.out.println("Random int (1-100): " + randomInt(1, 100));
        
        List<Integer> randomList = generateRandomList(5, 1, 10);
        System.out.println("Random list (5 numbers, 1-10): " + randomList);
        
        System.out.println("\nLibrary ready for Focus Nexus integration!");
    }
}