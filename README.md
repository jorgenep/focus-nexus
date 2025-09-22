# Focus Nexus - A Complete Interpreted Programming Language

Focus Nexus is a fully functional interpreted programming language implemented in C++. It features a complete lexer, parser, AST, and interpreter with support for variables, functions, control flow, and built-in operations.

## Features

- **Data Types**: Numbers (double), strings, booleans, nil, functions, lists
- **Variables**: Dynamic typing with `let` and `var` declarations
- **Operators**: Arithmetic (+, -, *, /), comparison (==, !=, <, >, <=, >=), logical (and, or, !)
- **Control Flow**: if/else statements, while loops, for loops
- **Functions**: User-defined functions with parameters, return values, and closures
- **Built-ins**: print(), input(), len(), str(), num(), type(), clock()
- **Lists**: Dynamic arrays with indexing support
- **Scoping**: Proper lexical scoping with block scope
- **Error Handling**: Comprehensive error reporting with line/column information

## Building

### Prerequisites
- C++17 compatible compiler (GCC 7+, Clang 5+, MSVC 2017+)
- CMake 3.10 or higher

### Build Instructions

```bash
# Clone or extract the project
cd FocusNexus

# Make build script executable (Linux/Mac)
chmod +x build.sh

# Build the project
./build.sh

# Or manually:
mkdir build
cd build
cmake ..
make
```

## Usage

### Running Files
```bash
# Run a MyLanguage program
./build/focusNexus examples/hello_world.fn

# Run the Fibonacci example
./build/focusNexus examples/fibonacci.fn
```

### Interactive Mode (REPL)
```bash
# Start interactive interpreter
./build/focusNexus

# Example session:
> let x = 10
> let y = 20
> print(x + y)
30
> exit
```

## Language Syntax

### Variables and Data Types
```javascript
// Variable declarations
let name = "Alice"
let age = 25
let pi = 3.14159
let is_student = true
let nothing = nil

// Lists
let numbers = [1, 2, 3, 4, 5]
print(numbers[0])  // 1
print(len(numbers))  // 5
```

### Functions
```javascript
// Function definition
function greet(name):
{
    print("Hello, " + name + "!")
    return "greeting complete"
}

// Function call
let result = greet("World")

// Recursive function
function factorial(n):
{
    if n <= 1:
    {
        return 1
    }
    return n * factorial(n - 1)
}
```

### Control Flow
```javascript
// If statements
if age >= 18:
{
    print("Adult")
}
else:
{
    print("Minor")
}

// While loops
let i = 0
while i < 5:
{
    print(i)
    i = i + 1
}

// For loops
for j = 0; j < 10; j = j + 1:
{
    print("j =", j)
}
```

### Built-in Functions
```javascript
// Output
print("Hello", "World", 123)

// Input
let name = input("Enter your name: ")

// Type conversion
let num = num("42")        // String to number
let text = str(123)        // Number to string

// Utility functions
print(len("Hello"))        // String length: 5
print(type(42))           // Type checking: "number"
print(clock())            // Current time in seconds
```

## Example Programs

The `tests/examples/` directory contains comprehensive example programs:

- **hello_world.fn** - Basic syntax and output
- **calculator.fn** - Function definitions and arithmetic
- **variables.fn** - Variable scoping and data types
- **functions.fn** - Function features including recursion and closures
- **loops.fn** - While and for loop examples
- **conditionals.fn** - If/else statements and boolean logic
- **fibonacci.fn** - Recursive Fibonacci implementation

## Architecture

The interpreter consists of several well-organized components:

### Core Components
- **Lexer** (`src/lexer/`) - Tokenizes source code into tokens
- **Parser** (`src/parser/`) - Builds Abstract Syntax Tree from tokens
- **Interpreter** (`src/interpreter.cpp`) - Executes the AST using the visitor pattern
- **Runtime** (`src/runtime/`) - Value system, environment, and callable functions

### Supporting Systems
- **Error Handling** (`src/error/`) - Comprehensive error reporting
- **Utilities** (`src/utils/`) - String and file manipulation helpers
- **Native Functions** - Built-in function implementations

## Language Grammar

```
program        → declaration* EOF ;

declaration    → funDecl
               | varDecl
               | statement ;

funDecl        → "function" IDENTIFIER "(" parameters? ")" ":" NEWLINE "{" block "}" ;
varDecl        → ( "var" | "let" ) IDENTIFIER ( "=" expression )? NEWLINE ;

statement      → exprStmt
               | forStmt
               | ifStmt
               | printStmt
               | returnStmt
               | whileStmt
               | block ;

exprStmt       → expression NEWLINE ;
forStmt        → "for" IDENTIFIER "=" expression ";" expression ";" expression ":" NEWLINE statement ;
ifStmt         → "if" expression ":" NEWLINE statement ( "else" ":" NEWLINE statement )? ;
printStmt      → "print" expression NEWLINE ;
returnStmt     → "return" expression? NEWLINE ;
whileStmt      → "while" expression ":" NEWLINE statement ;
block          → "{" declaration* "}" ;

expression     → assignment ;
assignment     → IDENTIFIER "=" assignment | logic_or ;
logic_or       → logic_and ( "or" logic_and )* ;
logic_and      → equality ( "and" equality )* ;
equality       → comparison ( ( "!=" | "==" ) comparison )* ;
comparison     → term ( ( ">" | ">=" | "<" | "<=" ) term )* ;
term           → factor ( ( "-" | "+" ) factor )* ;
factor         → unary ( ( "/" | "*" ) unary )* ;
unary          → ( "!" | "-" ) unary | call ;
call           → primary ( "(" arguments? ")" | "[" expression "]" )* ;
primary        → "true" | "false" | "nil" | NUMBER | STRING | IDENTIFIER
               | "(" expression ")" | "[" arguments? "]" ;
```

## Error Handling

The language provides comprehensive error handling:

- **Lexical Errors**: Invalid characters, unterminated strings
- **Parse Errors**: Syntax errors with line/column information
- **Runtime Errors**: Type errors, undefined variables, division by zero
- **Function Errors**: Wrong argument count, undefined functions

## Extending the Language

To add new features:

1. **New Tokens**: Add to `TokenType` enum in `token.hpp`
2. **Lexer**: Update `lexer.cpp` to recognize new syntax
3. **AST Nodes**: Add new node types in `ast.hpp`
4. **Parser**: Update parsing rules in `parser.cpp`
5. **Interpreter**: Implement evaluation in `interpreter.cpp`

## Testing

Run the example programs to test functionality:

```bash
# Test all examples
for file in tests/examples/*.fn; do
    echo "Testing $file"
    ./build/focusNexus "$file"
    echo "---"
done
```

## License

This project is provided for educational purposes. Feel free to use, modify, and distribute.

## Contributing

Contributions are welcome! Please ensure:
- Code follows the existing style
- New features include tests
- Documentation is updated
- All examples continue to work

## Troubleshooting

### Build Issues
- Ensure C++17 support is available
- Check CMake version (3.10+)
- Verify all source files are present

### Runtime Issues
- Check file paths are correct
- Ensure proper syntax (colons after control structures)
- Verify function definitions before calls

For more help, check the example programs or create an issue.