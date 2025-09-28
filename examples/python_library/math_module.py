"""
Example Python module for Focus Nexus integration
This module provides mathematical and utility functions
"""

import math
import random
import time
from typing import List, Union

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

def gcd(a: int, b: int) -> int:
    """Calculate the Greatest Common Divisor using Euclidean algorithm."""
    while b:
        a, b = b, a % b
    return abs(a)

def lcm(a: int, b: int) -> int:
    """Calculate the Least Common Multiple."""
    return abs(a * b) // gcd(a, b) if a and b else 0

def prime_factors(n: int) -> List[int]:
    """Return a list of prime factors of n."""
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

# Classes for more complex functionality
class Calculator:
    """A simple calculator class with history tracking."""
    
    def __init__(self):
        self.history = []
        self.memory = 0
    
    def add(self, a: float, b: float) -> float:
        """Add two numbers."""
        result = a + b
        self.history.append(f"{a} + {b} = {result}")
        return result
    
    def subtract(self, a: float, b: float) -> float:
        """Subtract two numbers."""
        result = a - b
        self.history.append(f"{a} - {b} = {result}")
        return result
    
    def multiply(self, a: float, b: float) -> float:
        """Multiply two numbers."""
        result = a * b
        self.history.append(f"{a} * {b} = {result}")
        return result
    
    def divide(self, a: float, b: float) -> float:
        """Divide two numbers."""
        if b == 0:
            raise ValueError("Cannot divide by zero")
        result = a / b
        self.history.append(f"{a} / {b} = {result}")
        return result
    
    def power(self, base: float, exponent: float) -> float:
        """Raise base to the power of exponent."""
        result = base ** exponent
        self.history.append(f"{base} ^ {exponent} = {result}")
        return result
    
    def sqrt(self, n: float) -> float:
        """Calculate square root."""
        if n < 0:
            raise ValueError("Cannot calculate square root of negative number")
        result = math.sqrt(n)
        self.history.append(f"sqrt({n}) = {result}")
        return result
    
    def get_history(self) -> List[str]:
        """Get calculation history."""
        return self.history.copy()
    
    def clear_history(self):
        """Clear calculation history."""
        self.history.clear()
    
    def store_memory(self, value: float):
        """Store value in memory."""
        self.memory = value
    
    def recall_memory(self) -> float:
        """Recall value from memory."""
        return self.memory
    
    def clear_memory(self):
        """Clear memory."""
        self.memory = 0

class Statistics:
    """A class for statistical calculations."""
    
    def __init__(self, data: List[float] = None):
        self.data = data or []
    
    def add_data(self, *values: float):
        """Add data points."""
        self.data.extend(values)
    
    def clear_data(self):
        """Clear all data."""
        self.data.clear()
    
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
    
    def get_range(self) -> float:
        """Get the range of the data."""
        if not self.data:
            raise ValueError("No data available")
        return max(self.data) - min(self.data)
    
    def get_summary(self) -> dict:
        """Get a summary of all statistics."""
        if not self.data:
            raise ValueError("No data available")
        
        return {
            'count': len(self.data),
            'mean': self.get_mean(),
            'median': self.get_median(),
            'mode': self.get_mode(),
            'std_dev': self.get_std_dev(),
            'min': min(self.data),
            'max': max(self.data),
            'range': self.get_range()
        }

# Module information
def get_module_info() -> dict:
    """Get information about this module."""
    return {
        'name': 'Focus Nexus Python Math Module',
        'version': '1.0.0',
        'description': 'Comprehensive mathematical and utility functions for Focus Nexus',
        'functions': [
            'fibonacci', 'factorial', 'is_prime', 'gcd', 'lcm',
            'prime_factors', 'is_perfect_number', 'collatz_sequence',
            'digital_root', 'reverse_number', 'is_palindrome_number',
            'mean', 'median', 'mode', 'standard_deviation',
            'reverse_string', 'is_palindrome_string', 'count_vowels',
            'count_consonants', 'capitalize_words', 'remove_duplicates',
            'generate_random_list', 'time_function',
            'binary_to_decimal', 'decimal_to_binary',
            'hex_to_decimal', 'decimal_to_hex'
        ],
        'classes': ['Calculator', 'Statistics']
    }

# Example usage and testing
if __name__ == "__main__":
    print("Focus Nexus Python Math Module")
    print("=" * 40)
    
    # Test some functions
    print(f"Fibonacci(10): {fibonacci(10)}")
    print(f"Factorial(5): {factorial(5)}")
    print(f"Is 17 prime? {is_prime(17)}")
    print(f"GCD(48, 18): {gcd(48, 18)}")
    print(f"Prime factors of 60: {prime_factors(60)}")
    
    # Test Calculator class
    calc = Calculator()
    print(f"Calculator: 10 + 5 = {calc.add(10, 5)}")
    print(f"Calculator: 20 * 3 = {calc.multiply(20, 3)}")
    print(f"Calculator history: {calc.get_history()}")
    
    # Test Statistics class
    stats = Statistics([1, 2, 3, 4, 5, 6, 7, 8, 9, 10])
    print(f"Statistics summary: {stats.get_summary()}")