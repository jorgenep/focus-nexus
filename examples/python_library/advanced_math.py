"""
Advanced Mathematical Functions for Focus Nexus
This module provides comprehensive mathematical and utility functions
"""

import math
import random
import time
from typing import List, Union, Dict, Any

def fibonacci(n: int) -> int:
    """Calculate the nth Fibonacci number using iterative approach."""
    if n < 0:
        raise ValueError("Fibonacci number cannot be negative")
    if n <= 1:
        return n
    
    a, b = 0, 1
    for _ in range(2, n + 1):
        a, b = b, a + b
    return b

def factorial(n: int) -> int:
    """Calculate factorial of n."""
    if n < 0:
        raise ValueError("Factorial cannot be calculated for negative numbers")
    if n <= 1:
        return 1
    
    result = 1
    for i in range(2, n + 1):
        result *= i
    return result

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
    """Return a list of prime factors of n."""
    if n <= 1:
        return []
    
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
    """Calculate the Greatest Common Divisor using Euclidean algorithm."""
    while b:
        a, b = b, a % b
    return abs(a)

def lcm(a: int, b: int) -> int:
    """Calculate the Least Common Multiple."""
    return abs(a * b) // gcd(a, b) if a and b else 0

def is_perfect_number(n: int) -> bool:
    """Check if a number is perfect (sum of proper divisors equals the number)."""
    if n <= 1:
        return False
    
    divisor_sum = 1  # 1 is always a proper divisor
    for i in range(2, int(math.sqrt(n)) + 1):
        if n % i == 0:
            divisor_sum += i
            if i != n // i:  # Avoid counting the square root twice
                divisor_sum += n // i
    
    return divisor_sum == n

def collatz_sequence(n: int) -> List[int]:
    """Generate the Collatz sequence starting from n."""
    if n <= 0:
        raise ValueError("Starting number must be positive")
    
    sequence = [n]
    while n != 1:
        if n % 2 == 0:
            n = n // 2
        else:
            n = 3 * n + 1
        sequence.append(n)
    
    return sequence

def digital_root(n: int) -> int:
    """Calculate the digital root of a number."""
    while n >= 10:
        n = sum(int(digit) for digit in str(n))
    return n

def reverse_number(n: int) -> int:
    """Reverse the digits of a number."""
    return int(str(abs(n))[::-1]) * (1 if n >= 0 else -1)

def is_palindrome_number(n: int) -> bool:
    """Check if a number is a palindrome."""
    return str(n) == str(n)[::-1]

# Statistical functions
def mean(numbers: List[float]) -> float:
    """Calculate the arithmetic mean of a list of numbers."""
    if not numbers:
        raise ValueError("Cannot calculate mean of empty list")
    return sum(numbers) / len(numbers)

def median(numbers: List[float]) -> float:
    """Calculate the median of a list of numbers."""
    if not numbers:
        raise ValueError("Cannot calculate median of empty list")
    
    sorted_numbers = sorted(numbers)
    n = len(sorted_numbers)
    
    if n % 2 == 0:
        return (sorted_numbers[n // 2 - 1] + sorted_numbers[n // 2]) / 2
    else:
        return sorted_numbers[n // 2]

def mode(numbers: List[Union[int, float]]) -> Union[int, float]:
    """Find the mode (most frequent value) in a list of numbers."""
    if not numbers:
        raise ValueError("Cannot calculate mode of empty list")
    
    frequency = {}
    for num in numbers:
        frequency[num] = frequency.get(num, 0) + 1
    
    max_frequency = max(frequency.values())
    modes = [num for num, freq in frequency.items() if freq == max_frequency]
    
    return modes[0] if len(modes) == 1 else modes

def standard_deviation(numbers: List[float]) -> float:
    """Calculate the standard deviation of a list of numbers."""
    if len(numbers) < 2:
        raise ValueError("Need at least 2 numbers to calculate standard deviation")
    
    avg = mean(numbers)
    variance = sum((x - avg) ** 2 for x in numbers) / (len(numbers) - 1)
    return math.sqrt(variance)

def variance(numbers: List[float]) -> float:
    """Calculate the variance of a list of numbers."""
    if len(numbers) < 2:
        raise ValueError("Need at least 2 numbers to calculate variance")
    
    avg = mean(numbers)
    return sum((x - avg) ** 2 for x in numbers) / (len(numbers) - 1)

def percentile(numbers: List[float], p: float) -> float:
    """Calculate the p-th percentile of a list of numbers."""
    if not numbers:
        raise ValueError("Cannot calculate percentile of empty list")
    if not 0 <= p <= 100:
        raise ValueError("Percentile must be between 0 and 100")
    
    sorted_numbers = sorted(numbers)
    n = len(sorted_numbers)
    
    if p == 100:
        return sorted_numbers[-1]
    
    index = (p / 100) * (n - 1)
    lower = int(index)
    upper = lower + 1
    
    if upper >= n:
        return sorted_numbers[lower]
    
    weight = index - lower
    return sorted_numbers[lower] * (1 - weight) + sorted_numbers[upper] * weight

# String manipulation functions
def reverse_string(s: str) -> str:
    """Reverse a string."""
    return s[::-1]

def is_palindrome_string(s: str) -> bool:
    """Check if a string is a palindrome (ignoring case and spaces)."""
    cleaned = ''.join(s.lower().split())
    return cleaned == cleaned[::-1]

def count_vowels(s: str) -> int:
    """Count the number of vowels in a string."""
    vowels = 'aeiouAEIOU'
    return sum(1 for char in s if char in vowels)

def count_consonants(s: str) -> int:
    """Count the number of consonants in a string."""
    consonants = 'bcdfghjklmnpqrstvwxyzBCDFGHJKLMNPQRSTVWXYZ'
    return sum(1 for char in s if char in consonants)

def capitalize_words(s: str) -> str:
    """Capitalize the first letter of each word."""
    return ' '.join(word.capitalize() for word in s.split())

def remove_duplicates(s: str) -> str:
    """Remove duplicate characters from a string while preserving order."""
    seen = set()
    result = []
    for char in s:
        if char not in seen:
            seen.add(char)
            result.append(char)
    return ''.join(result)

def word_frequency(text: str) -> Dict[str, int]:
    """Count word frequency in text."""
    words = text.lower().split()
    frequency = {}
    for word in words:
        # Remove punctuation
        clean_word = ''.join(c for c in word if c.isalnum())
        if clean_word:
            frequency[clean_word] = frequency.get(clean_word, 0) + 1
    return frequency

def longest_word(text: str) -> str:
    """Find the longest word in text."""
    words = text.split()
    return max(words, key=len) if words else ""

# Utility functions
def generate_random_list(size: int, min_val: int = 1, max_val: int = 100) -> List[int]:
    """Generate a list of random integers."""
    return [random.randint(min_val, max_val) for _ in range(size)]

def time_function(func, *args, **kwargs):
    """Time the execution of a function."""
    start_time = time.time()
    result = func(*args, **kwargs)
    end_time = time.time()
    return result, end_time - start_time

def binary_to_decimal(binary_str: str) -> int:
    """Convert binary string to decimal."""
    try:
        return int(binary_str, 2)
    except ValueError:
        raise ValueError("Invalid binary string")

def decimal_to_binary(decimal: int) -> str:
    """Convert decimal to binary string."""
    return bin(decimal)[2:]  # Remove '0b' prefix

def hex_to_decimal(hex_str: str) -> int:
    """Convert hexadecimal string to decimal."""
    try:
        return int(hex_str, 16)
    except ValueError:
        raise ValueError("Invalid hexadecimal string")

def decimal_to_hex(decimal: int) -> str:
    """Convert decimal to hexadecimal string."""
    return hex(decimal)[2:]  # Remove '0x' prefix

def is_armstrong_number(n: int) -> bool:
    """Check if a number is an Armstrong number."""
    digits = [int(d) for d in str(n)]
    power = len(digits)
    return sum(d ** power for d in digits) == n

def sum_of_digits(n: int) -> int:
    """Calculate the sum of digits in a number."""
    return sum(int(digit) for digit in str(abs(n)))

def product_of_digits(n: int) -> int:
    """Calculate the product of digits in a number."""
    result = 1
    for digit in str(abs(n)):
        result *= int(digit)
    return result

# Classes for complex functionality
class Calculator:
    """A comprehensive calculator class with history tracking."""
    
    def __init__(self):
        self.history = []
        self.memory = 0
        self.last_result = 0
    
    def add(self, a: float, b: float) -> float:
        """Add two numbers."""
        result = a + b
        self.last_result = result
        self.history.append(f"{a} + {b} = {result}")
        return result
    
    def subtract(self, a: float, b: float) -> float:
        """Subtract two numbers."""
        result = a - b
        self.last_result = result
        self.history.append(f"{a} - {b} = {result}")
        return result
    
    def multiply(self, a: float, b: float) -> float:
        """Multiply two numbers."""
        result = a * b
        self.last_result = result
        self.history.append(f"{a} * {b} = {result}")
        return result
    
    def divide(self, a: float, b: float) -> float:
        """Divide two numbers."""
        if b == 0:
            raise ValueError("Cannot divide by zero")
        result = a / b
        self.last_result = result
        self.history.append(f"{a} / {b} = {result}")
        return result
    
    def power(self, base: float, exponent: float) -> float:
        """Raise base to the power of exponent."""
        result = base ** exponent
        self.last_result = result
        self.history.append(f"{base} ^ {exponent} = {result}")
        return result
    
    def sqrt(self, n: float) -> float:
        """Calculate square root."""
        if n < 0:
            raise ValueError("Cannot calculate square root of negative number")
        result = math.sqrt(n)
        self.last_result = result
        self.history.append(f"sqrt({n}) = {result}")
        return result
    
    def factorial(self, n: int) -> int:
        """Calculate factorial."""
        if n < 0:
            raise ValueError("Cannot calculate factorial of negative number")
        result = factorial(n)  # Use module function
        self.last_result = result
        self.history.append(f"{n}! = {result}")
        return result
    
    def get_history(self) -> List[str]:
        """Get calculation history."""
        return self.history.copy()
    
    def clear_history(self):
        """Clear calculation history."""
        self.history.clear()
    
    def get_last_result(self) -> float:
        """Get the last calculation result."""
        return self.last_result
    
    def store_memory(self, value: float):
        """Store value in memory."""
        self.memory = value
        self.history.append(f"Stored in memory: {value}")
    
    def recall_memory(self) -> float:
        """Recall value from memory."""
        self.history.append(f"Recalled from memory: {self.memory}")
        return self.memory
    
    def clear_memory(self):
        """Clear memory."""
        self.memory = 0
        self.history.append("Memory cleared")

class Statistics:
    """A class for statistical calculations with data management."""
    
    def __init__(self, data: List[float] = None):
        self.data = data or []
    
    def add_data(self, *values: float):
        """Add data points."""
        self.data.extend(values)
    
    def clear_data(self):
        """Clear all data."""
        self.data.clear()
    
    def get_data(self) -> List[float]:
        """Get a copy of the data."""
        return self.data.copy()
    
    def get_count(self) -> int:
        """Get the number of data points."""
        return len(self.data)
    
    def get_mean(self) -> float:
        """Get the mean of the data."""
        return mean(self.data)
    
    def get_median(self) -> float:
        """Get the median of the data."""
        return median(self.data)
    
    def get_mode(self):
        """Get the mode of the data."""
        return mode(self.data)
    
    def get_std_dev(self) -> float:
        """Get the standard deviation of the data."""
        return standard_deviation(self.data)
    
    def get_variance(self) -> float:
        """Get the variance of the data."""
        return variance(self.data)
    
    def get_range(self) -> float:
        """Get the range of the data."""
        if not self.data:
            raise ValueError("No data available")
        return max(self.data) - min(self.data)
    
    def get_percentile(self, p: float) -> float:
        """Get the p-th percentile of the data."""
        return percentile(self.data, p)
    
    def get_summary(self) -> Dict[str, Any]:
        """Get a comprehensive summary of all statistics."""
        if not self.data:
            raise ValueError("No data available")
        
        return {
            'count': len(self.data),
            'sum': sum(self.data),
            'mean': self.get_mean(),
            'median': self.get_median(),
            'mode': self.get_mode(),
            'std_dev': self.get_std_dev(),
            'variance': self.get_variance(),
            'min': min(self.data),
            'max': max(self.data),
            'range': self.get_range(),
            'q1': self.get_percentile(25),
            'q3': self.get_percentile(75)
        }

class DataProcessor:
    """Data processing utilities."""
    
    @staticmethod
    def filter_primes(numbers: List[int]) -> List[int]:
        """Filter prime numbers from a list."""
        return [n for n in numbers if is_prime(n)]
    
    @staticmethod
    def filter_evens(numbers: List[int]) -> List[int]:
        """Filter even numbers from a list."""
        return [n for n in numbers if n % 2 == 0]
    
    @staticmethod
    def filter_odds(numbers: List[int]) -> List[int]:
        """Filter odd numbers from a list."""
        return [n for n in numbers if n % 2 != 0]
    
    @staticmethod
    def factorial_list(numbers: List[int]) -> List[int]:
        """Calculate factorial for each number in list (limited to n <= 20)."""
        return [factorial(n) for n in numbers if 0 <= n <= 20]
    
    @staticmethod
    def fibonacci_sequence(count: int) -> List[int]:
        """Generate Fibonacci sequence with specified count."""
        return [fibonacci(i) for i in range(count)]
    
    @staticmethod
    def prime_sequence(count: int) -> List[int]:
        """Generate first 'count' prime numbers."""
        primes = []
        candidate = 2
        while len(primes) < count:
            if is_prime(candidate):
                primes.append(candidate)
            candidate += 1
        return primes
    
    @staticmethod
    def perfect_numbers(limit: int) -> List[int]:
        """Find perfect numbers up to limit."""
        return [n for n in range(1, limit + 1) if is_perfect_number(n)]
    
    @staticmethod
    def armstrong_numbers(limit: int) -> List[int]:
        """Find Armstrong numbers up to limit."""
        return [n for n in range(1, limit + 1) if is_armstrong_number(n)]
    
    @staticmethod
    def sort_ascending(numbers: List[float]) -> List[float]:
        """Sort numbers in ascending order."""
        return sorted(numbers)
    
    @staticmethod
    def sort_descending(numbers: List[float]) -> List[float]:
        """Sort numbers in descending order."""
        return sorted(numbers, reverse=True)
    
    @staticmethod
    def remove_outliers(numbers: List[float], threshold: float = 2.0) -> List[float]:
        """Remove outliers using standard deviation method."""
        if len(numbers) < 3:
            return numbers
        
        avg = mean(numbers)
        std_dev = standard_deviation(numbers)
        
        return [n for n in numbers if abs(n - avg) <= threshold * std_dev]

class NumberTheory:
    """Number theory utilities."""
    
    @staticmethod
    def sieve_of_eratosthenes(limit: int) -> List[int]:
        """Generate primes up to limit using Sieve of Eratosthenes."""
        if limit < 2:
            return []
        
        sieve = [True] * (limit + 1)
        sieve[0] = sieve[1] = False
        
        for i in range(2, int(math.sqrt(limit)) + 1):
            if sieve[i]:
                for j in range(i * i, limit + 1, i):
                    sieve[j] = False
        
        return [i for i in range(2, limit + 1) if sieve[i]]
    
    @staticmethod
    def euler_totient(n: int) -> int:
        """Calculate Euler's totient function."""
        if n <= 0:
            raise ValueError("Input must be positive")
        
        result = n
        p = 2
        while p * p <= n:
            if n % p == 0:
                while n % p == 0:
                    n //= p
                result -= result // p
            p += 1
        
        if n > 1:
            result -= result // n
        
        return result
    
    @staticmethod
    def chinese_remainder_theorem(remainders: List[int], moduli: List[int]) -> int:
        """Solve system of congruences using Chinese Remainder Theorem."""
        if len(remainders) != len(moduli):
            raise ValueError("Remainders and moduli lists must have same length")
        
        total = 0
        prod = 1
        for m in moduli:
            prod *= m
        
        for r, m in zip(remainders, moduli):
            p = prod // m
            total += r * p * pow(p, -1, m)
        
        return total % prod

# Module information and utilities
def get_module_info() -> Dict[str, Any]:
    """Get comprehensive information about this module."""
    return {
        'name': 'Focus Nexus Advanced Math Module',
        'version': '2.0.0',
        'description': 'Comprehensive mathematical and utility functions for Focus Nexus',
        'author': 'Focus Nexus Team',
        'functions': [
            'fibonacci', 'factorial', 'is_prime', 'prime_factors', 'gcd', 'lcm',
            'is_perfect_number', 'collatz_sequence', 'digital_root', 'reverse_number',
            'is_palindrome_number', 'mean', 'median', 'mode', 'standard_deviation',
            'variance', 'percentile', 'reverse_string', 'is_palindrome_string',
            'count_vowels', 'count_consonants', 'capitalize_words', 'remove_duplicates',
            'word_frequency', 'longest_word', 'generate_random_list', 'time_function',
            'binary_to_decimal', 'decimal_to_binary', 'hex_to_decimal', 'decimal_to_hex',
            'is_armstrong_number', 'sum_of_digits', 'product_of_digits'
        ],
        'classes': ['Calculator', 'Statistics', 'DataProcessor', 'NumberTheory'],
        'dependencies': ['math', 'random', 'time', 'typing']
    }

def run_self_test() -> bool:
    """Run self-test to verify module functionality."""
    try:
        # Test basic functions
        assert fibonacci(10) == 55
        assert factorial(5) == 120
        assert is_prime(17) == True
        assert gcd(48, 18) == 6
        assert lcm(12, 18) == 36
        
        # Test string functions
        assert reverse_string("hello") == "olleh"
        assert count_vowels("hello world") == 3
        assert capitalize_words("hello world") == "Hello World"
        
        # Test classes
        calc = Calculator()
        assert calc.add(10, 5) == 15
        assert calc.multiply(3, 4) == 12
        
        stats = Statistics([1, 2, 3, 4, 5])
        assert stats.get_mean() == 3.0
        assert stats.get_median() == 3.0
        
        print("All self-tests passed!")
        return True
        
    except Exception as e:
        print(f"Self-test failed: {e}")
        return False

# Example usage and testing
if __name__ == "__main__":
    print("Focus Nexus Advanced Math Module")
    print("=" * 40)
    
    # Run self-test
    run_self_test()
    
    # Display module info
    info = get_module_info()
    print(f"\nModule: {info['name']}")
    print(f"Version: {info['version']}")
    print(f"Description: {info['description']}")
    print(f"Functions: {len(info['functions'])}")
    print(f"Classes: {len(info['classes'])}")