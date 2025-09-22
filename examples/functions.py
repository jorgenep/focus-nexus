# Function definition and calls
def greet(name):
    print("Hello", name)
    return "Greeting complete"

def add(a, b):
    result = a + b
    return result

def factorial(n):
    if n <= 1:
        return 1
    else:
        return n * factorial(n - 1)

# Function calls
greet("World")
result = add(5, 3)
print("5 + 3 =", result)

# Recursive function
fact = factorial(5)
print("5! =", fact)

# Function with no return value
def say_hello():
    print("Hello from function!")

say_hello()