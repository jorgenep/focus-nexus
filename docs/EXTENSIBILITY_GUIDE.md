# Focus Nexus Extensibility Guide

This guide explains how to extend the Focus Nexus programming language by adding new features, tokens, syntax, and functionality.

## Table of Contents

1. [Architecture Overview](#architecture-overview)
2. [Adding New Tokens](#adding-new-tokens)
3. [Extending the Lexer](#extending-the-lexer)
4. [Creating New AST Nodes](#creating-new-ast-nodes)
5. [Updating the Parser](#updating-the-parser)
6. [Implementing Interpreter Logic](#implementing-interpreter-logic)
7. [Complete Feature Example](#complete-feature-example)
8. [Best Practices](#best-practices)
9. [Testing Extensions](#testing-extensions)

## Architecture Overview

Focus Nexus follows a traditional interpreter architecture:

```
Source Code → Lexer → Tokens → Parser → AST → Interpreter → Execution
```

Each component has specific responsibilities:

- **Lexer**: Converts source code into tokens
- **Parser**: Builds an Abstract Syntax Tree (AST) from tokens
- **Interpreter**: Executes the AST using the visitor pattern

## Adding New Tokens

### Step 1: Define Token Types

Add new token types to `src/lexer/token.hpp`:

```cpp
enum class TokenType {
    // Existing tokens...
    
    // Your new tokens
    ASYNC, AWAIT, YIELD,
    MATCH, WHEN, GUARD,
    DECORATOR, ANNOTATION,
    
    // More tokens...
};
```

### Step 2: Update Token Utilities

Add string representations in `src/lexer/lexer.cpp`:

```cpp
std::string TokenUtils::tokenTypeToString(TokenType type) {
    switch (type) {
        // Existing cases...
        case TokenType::ASYNC: return "ASYNC";
        case TokenType::AWAIT: return "AWAIT";
        case TokenType::YIELD: return "YIELD";
        case TokenType::MATCH: return "MATCH";
        case TokenType::WHEN: return "WHEN";
        case TokenType::GUARD: return "GUARD";
        case TokenType::DECORATOR: return "DECORATOR";
        case TokenType::ANNOTATION: return "ANNOTATION";
        // More cases...
    }
}
```

### Step 3: Add Keywords

If your tokens are keywords, add them to the keywords map:

```cpp
std::unordered_map<std::string, TokenType> TokenUtils::keywords = {
    // Existing keywords...
    {"async", TokenType::ASYNC},
    {"await", TokenType::AWAIT},
    {"yield", TokenType::YIELD},
    {"match", TokenType::MATCH},
    {"when", TokenType::WHEN},
    {"guard", TokenType::GUARD},
};
```

## Extending the Lexer

### Adding Single-Character Tokens

For operators or punctuation, update the lexer's `scanTokens()` method:

```cpp
char c = advance();
switch (c) {
    // Existing cases...
    case '@': addToken(TokenType::AT); break;
    case '#': addToken(TokenType::HASH); break;
    case '$': addToken(TokenType::DOLLAR); break;
    // More cases...
}
```

### Adding Multi-Character Tokens

For operators like `=>`, `??`, or `<=>`:

```cpp
case '=':
    if (match('=')) {
        addToken(TokenType::EQUAL_EQUAL);
    } else if (match('>')) {
        addToken(TokenType::ARROW);  // =>
    } else {
        addToken(TokenType::EQUAL);
    }
    break;

case '?':
    if (match('?')) {
        addToken(TokenType::NULL_COALESCING);  // ??
    } else {
        addToken(TokenType::QUESTION);
    }
    break;
```

### Adding Complex Lexing Logic

For more complex tokens (like string interpolation):

```cpp
void Lexer::stringInterpolation() {
    std::string value;
    std::vector<Token> interpolatedParts;
    
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '$' && peekNext() == '{') {
            // Handle interpolation
            if (!value.empty()) {
                addToken(TokenType::STRING_PART, value);
                value.clear();
            }
            
            advance(); // $
            advance(); // {
            
            // Lex the interpolated expression
            int braceCount = 1;
            std::string expr;
            while (braceCount > 0 && !isAtEnd()) {
                char c = advance();
                if (c == '{') braceCount++;
                else if (c == '}') braceCount--;
                
                if (braceCount > 0) expr += c;
            }
            
            addToken(TokenType::INTERPOLATION_EXPR, expr);
        } else {
            value += advance();
        }
    }
    
    if (!value.empty()) {
        addToken(TokenType::STRING_PART, value);
    }
}
```

## Creating New AST Nodes

### Step 1: Define Expression Nodes

Add new expression types to `src/parser/ast.hpp`:

```cpp
// Async/Await expressions
class AsyncExpr : public Expr {
public:
    ExprPtr expression;
    
    explicit AsyncExpr(ExprPtr expression) : expression(std::move(expression)) {}
    Value accept(ASTVisitor& visitor) override;
};

class AwaitExpr : public Expr {
public:
    ExprPtr expression;
    
    explicit AwaitExpr(ExprPtr expression) : expression(std::move(expression)) {}
    Value accept(ASTVisitor& visitor) override;
};

// Pattern matching expression
class MatchExpr : public Expr {
public:
    ExprPtr value;
    std::vector<std::pair<ExprPtr, ExprPtr>> cases; // pattern -> result
    ExprPtr defaultCase;
    
    MatchExpr(ExprPtr value, std::vector<std::pair<ExprPtr, ExprPtr>> cases, ExprPtr defaultCase)
        : value(std::move(value)), cases(std::move(cases)), defaultCase(std::move(defaultCase)) {}
    
    Value accept(ASTVisitor& visitor) override;
};

// Decorator expression
class DecoratorExpr : public Expr {
public:
    Token decorator;
    ExprPtr target;
    std::vector<ExprPtr> arguments;
    
    DecoratorExpr(Token decorator, ExprPtr target, std::vector<ExprPtr> arguments)
        : decorator(std::move(decorator)), target(std::move(target)), arguments(std::move(arguments)) {}
    
    Value accept(ASTVisitor& visitor) override;
};
```

### Step 2: Define Statement Nodes

```cpp
// Async function statement
class AsyncFunctionStmt : public Stmt {
public:
    Token name;
    std::vector<Token> params;
    std::vector<StmtPtr> body;
    
    AsyncFunctionStmt(Token name, std::vector<Token> params, std::vector<StmtPtr> body)
        : name(std::move(name)), params(std::move(params)), body(std::move(body)) {}
    
    void accept(ASTVisitor& visitor) override;
};

// Yield statement
class YieldStmt : public Stmt {
public:
    Token keyword;
    ExprPtr value;
    
    YieldStmt(Token keyword, ExprPtr value)
        : keyword(std::move(keyword)), value(std::move(value)) {}
    
    void accept(ASTVisitor& visitor) override;
};

// Decorator statement
class DecoratorStmt : public Stmt {
public:
    std::vector<DecoratorExpr> decorators;
    StmtPtr target;
    
    DecoratorStmt(std::vector<DecoratorExpr> decorators, StmtPtr target)
        : decorators(std::move(decorators)), target(std::move(target)) {}
    
    void accept(ASTVisitor& visitor) override;
};
```

### Step 3: Update Visitor Interface

Add visitor methods for your new nodes:

```cpp
class ASTVisitor {
public:
    // Existing methods...
    
    // New expression visitors
    virtual Value visitAsyncExpr(AsyncExpr& expr) = 0;
    virtual Value visitAwaitExpr(AwaitExpr& expr) = 0;
    virtual Value visitMatchExpr(MatchExpr& expr) = 0;
    virtual Value visitDecoratorExpr(DecoratorExpr& expr) = 0;
    
    // New statement visitors
    virtual void visitAsyncFunctionStmt(AsyncFunctionStmt& stmt) = 0;
    virtual void visitYieldStmt(YieldStmt& stmt) = 0;
    virtual void visitDecoratorStmt(DecoratorStmt& stmt) = 0;
};
```

### Step 4: Implement Accept Methods

In `src/parser/ast_printer.cpp`:

```cpp
Value AsyncExpr::accept(ASTVisitor& visitor) {
    return visitor.visitAsyncExpr(*this);
}

Value AwaitExpr::accept(ASTVisitor& visitor) {
    return visitor.visitAwaitExpr(*this);
}

Value MatchExpr::accept(ASTVisitor& visitor) {
    return visitor.visitMatchExpr(*this);
}

void AsyncFunctionStmt::accept(ASTVisitor& visitor) {
    visitor.visitAsyncFunctionStmt(*this);
}

void YieldStmt::accept(ASTVisitor& visitor) {
    visitor.visitYieldStmt(*this);
}
```

## Updating the Parser

### Step 1: Add Parsing Methods

Add parsing methods to `src/parser/parser.hpp`:

```cpp
class Parser {
private:
    // Existing methods...
    
    // New parsing methods
    ExprPtr asyncExpression();
    ExprPtr awaitExpression();
    ExprPtr matchExpression();
    ExprPtr decoratorExpression();
    
    StmtPtr asyncFunctionStatement();
    StmtPtr yieldStatement();
    StmtPtr decoratorStatement();
    
    // Helper methods
    std::vector<std::pair<ExprPtr, ExprPtr>> parseMatchCases();
    ExprPtr parsePattern();
};
```

### Step 2: Implement Parsing Logic

In `src/parser/parser.cpp`:

```cpp
ExprPtr Parser::asyncExpression() {
    consume(TokenType::ASYNC, "Expected 'async'");
    auto expr = expression();
    return std::make_unique<AsyncExpr>(std::move(expr));
}

ExprPtr Parser::awaitExpression() {
    consume(TokenType::AWAIT, "Expected 'await'");
    auto expr = expression();
    return std::make_unique<AwaitExpr>(std::move(expr));
}

ExprPtr Parser::matchExpression() {
    consume(TokenType::MATCH, "Expected 'match'");
    auto value = expression();
    consume(TokenType::COLON, "Expected ':' after match value");
    consume(TokenType::NEWLINE, "Expected newline after ':'");
    consume(TokenType::LEFT_BRACE, "Expected '{' before match cases");
    
    auto cases = parseMatchCases();
    
    ExprPtr defaultCase = nullptr;
    if (match({TokenType::DEFAULT})) {
        consume(TokenType::COLON, "Expected ':' after 'default'");
        consume(TokenType::NEWLINE, "Expected newline after ':'");
        defaultCase = expression();
    }
    
    consume(TokenType::RIGHT_BRACE, "Expected '}' after match cases");
    return std::make_unique<MatchExpr>(std::move(value), std::move(cases), std::move(defaultCase));
}

std::vector<std::pair<ExprPtr, ExprPtr>> Parser::parseMatchCases() {
    std::vector<std::pair<ExprPtr, ExprPtr>> cases;
    
    while (!check(TokenType::RIGHT_BRACE) && !check(TokenType::DEFAULT) && !isAtEnd()) {
        if (match({TokenType::NEWLINE})) continue;
        
        if (match({TokenType::WHEN})) {
            auto pattern = parsePattern();
            consume(TokenType::COLON, "Expected ':' after pattern");
            consume(TokenType::NEWLINE, "Expected newline after ':'");
            auto result = expression();
            cases.emplace_back(std::move(pattern), std::move(result));
        }
    }
    
    return cases;
}

ExprPtr Parser::parsePattern() {
    // Implement pattern parsing logic
    // This could include literal patterns, variable patterns, etc.
    return expression();
}
```

### Step 3: Integrate with Main Parser

Update the main parsing methods to recognize your new syntax:

```cpp
ExprPtr Parser::primary() {
    // Existing cases...
    
    if (match({TokenType::ASYNC})) {
        return asyncExpression();
    }
    
    if (match({TokenType::AWAIT})) {
        return awaitExpression();
    }
    
    if (match({TokenType::MATCH})) {
        return matchExpression();
    }
    
    // More cases...
}

StmtPtr Parser::statement() {
    // Existing cases...
    
    if (match({TokenType::ASYNC})) {
        if (check(TokenType::FUNCTION)) {
            return asyncFunctionStatement();
        } else {
            // Handle async expression statement
            auto expr = asyncExpression();
            consume(TokenType::NEWLINE, "Expected newline after async expression");
            return std::make_unique<ExpressionStmt>(std::move(expr));
        }
    }
    
    if (match({TokenType::YIELD})) {
        return yieldStatement();
    }
    
    if (match({TokenType::AT})) {
        return decoratorStatement();
    }
    
    // More cases...
}
```

## Implementing Interpreter Logic

### Step 1: Add Visitor Methods

In `src/interpreter.hpp`:

```cpp
class Interpreter : public ASTVisitor {
    // Existing methods...
    
    // New expression visitors
    Value visitAsyncExpr(AsyncExpr& expr) override;
    Value visitAwaitExpr(AwaitExpr& expr) override;
    Value visitMatchExpr(MatchExpr& expr) override;
    Value visitDecoratorExpr(DecoratorExpr& expr) override;
    
    // New statement visitors
    void visitAsyncFunctionStmt(AsyncFunctionStmt& stmt) override;
    void visitYieldStmt(YieldStmt& stmt) override;
    void visitDecoratorStmt(DecoratorStmt& stmt) override;
};
```

### Step 2: Implement Execution Logic

In `src/interpreter.cpp`:

```cpp
Value Interpreter::visitAsyncExpr(AsyncExpr& expr) {
    // Create an async task/promise
    auto task = std::make_shared<AsyncTask>();
    task->expression = std::move(expr.expression);
    task->environment = environment;
    
    // Add to async execution queue
    asyncExecutor.schedule(task);
    
    return Value(task); // Return a promise/future-like object
}

Value Interpreter::visitAwaitExpr(AwaitExpr& expr) {
    Value asyncValue = evaluate(*expr.expression);
    
    if (!asyncValue.isAsyncTask()) {
        throw RuntimeError(Token(TokenType::AWAIT, "await", "", 0, 0),
                          "Can only await async expressions");
    }
    
    auto task = asyncValue.asAsyncTask();
    return task->await(); // Block until completion
}

Value Interpreter::visitMatchExpr(MatchExpr& expr) {
    Value matchValue = evaluate(*expr.value);
    
    // Try each case
    for (const auto& case_ : expr.cases) {
        if (patternMatches(case_.first.get(), matchValue)) {
            return evaluate(*case_.second);
        }
    }
    
    // Use default case if no pattern matched
    if (expr.defaultCase) {
        return evaluate(*expr.defaultCase);
    }
    
    throw RuntimeError(Token(TokenType::MATCH, "match", "", 0, 0),
                      "No matching pattern found");
}

void Interpreter::visitAsyncFunctionStmt(AsyncFunctionStmt& stmt) {
    auto function = std::make_shared<AsyncFunction>(&stmt, environment);
    environment->define(stmt.name.lexeme, Value(function));
}

void Interpreter::visitYieldStmt(YieldStmt& stmt) {
    Value value;
    if (stmt.value) {
        value = evaluate(*stmt.value);
    }
    
    // Yield the value (suspend execution)
    throw YieldValue(value);
}
```

### Step 3: Add Supporting Classes

Create supporting classes for your new features:

```cpp
// In src/runtime/async_task.hpp
class AsyncTask {
public:
    ExprPtr expression;
    std::shared_ptr<Environment> environment;
    Value result;
    bool completed = false;
    
    Value await() {
        // Implementation depends on your async model
        while (!completed) {
            // Wait or yield
        }
        return result;
    }
};

// Pattern matching helper
bool Interpreter::patternMatches(Expr* pattern, const Value& value) {
    // Implement pattern matching logic
    if (auto literal = dynamic_cast<LiteralExpr*>(pattern)) {
        return literal->value == value;
    }
    
    if (auto variable = dynamic_cast<VariableExpr*>(pattern)) {
        // Bind variable to value
        environment->define(variable->name.lexeme, value);
        return true;
    }
    
    // More pattern types...
    return false;
}
```

## Complete Feature Example

Let's implement a complete feature: **Range expressions** (`1..10`, `'a'..'z'`)

### Step 1: Add Tokens

```cpp
// In token.hpp
enum class TokenType {
    // ...
    DOT_DOT,        // ..
    DOT_DOT_EQUAL,  // ..=
    // ...
};
```

### Step 2: Update Lexer

```cpp
// In lexer.cpp
case '.':
    if (match('.')) {
        if (match('=')) {
            addToken(TokenType::DOT_DOT_EQUAL);
        } else {
            addToken(TokenType::DOT_DOT);
        }
    } else {
        addToken(TokenType::DOT);
    }
    break;
```

### Step 3: Create AST Node

```cpp
// In ast.hpp
class RangeExpr : public Expr {
public:
    ExprPtr start;
    ExprPtr end;
    bool inclusive;
    
    RangeExpr(ExprPtr start, ExprPtr end, bool inclusive)
        : start(std::move(start)), end(std::move(end)), inclusive(inclusive) {}
    
    Value accept(ASTVisitor& visitor) override;
};
```

### Step 4: Update Parser

```cpp
// In parser.cpp
ExprPtr Parser::comparison() {
    auto expr = term();
    
    while (match({TokenType::DOT_DOT, TokenType::DOT_DOT_EQUAL})) {
        Token operator_ = previous();
        auto right = term();
        bool inclusive = (operator_.type == TokenType::DOT_DOT_EQUAL);
        expr = std::make_unique<RangeExpr>(std::move(expr), std::move(right), inclusive);
    }
    
    return expr;
}
```

### Step 5: Implement Interpreter

```cpp
// In interpreter.cpp
Value Interpreter::visitRangeExpr(RangeExpr& expr) {
    Value start = evaluate(*expr.start);
    Value end = evaluate(*expr.end);
    
    if (start.isNumber() && end.isNumber()) {
        auto range = std::make_shared<NumberRange>(
            start.asNumber(), 
            end.asNumber(), 
            expr.inclusive
        );
        return Value(range);
    }
    
    if (start.isString() && end.isString()) {
        auto range = std::make_shared<CharRange>(
            start.asString()[0], 
            end.asString()[0], 
            expr.inclusive
        );
        return Value(range);
    }
    
    throw RuntimeError(Token(TokenType::DOT_DOT, "..", "", 0, 0),
                      "Range operands must be numbers or single characters");
}
```

### Step 6: Test the Feature

```javascript
// Test range expressions
let numbers = 1..10
print("Numbers:", numbers)  // [1, 2, 3, 4, 5, 6, 7, 8, 9]

let inclusive_numbers = 1..=10
print("Inclusive:", inclusive_numbers)  // [1, 2, 3, 4, 5, 6, 7, 8, 9, 10]

let letters = 'a'..'e'
print("Letters:", letters)  // ['a', 'b', 'c', 'd']

// Use in for loops
for i in 1..5:
{
    print("Index:", i)
}
```

## Best Practices

### 1. Consistent Naming

- Use descriptive names for tokens, AST nodes, and methods
- Follow existing naming conventions
- Use consistent prefixes/suffixes

### 2. Error Handling

- Provide clear error messages
- Include line/column information
- Handle edge cases gracefully

```cpp
if (!condition) {
    throw RuntimeError(token, "Clear description of what went wrong");
}
```

### 3. Documentation

- Document new syntax in grammar files
- Add examples to documentation
- Include usage patterns

### 4. Testing

- Write comprehensive tests for new features
- Test error conditions
- Include integration tests

### 5. Backward Compatibility

- Avoid breaking existing syntax
- Consider deprecation paths for changes
- Maintain semantic versioning

## Testing Extensions

### Unit Tests

Create test files for your new features:

```cpp
// test_range_expressions.cpp
#include "gtest/gtest.h"
#include "../src/interpreter.hpp"

TEST(RangeExpressions, NumericRange) {
    std::string source = "let r = 1..5";
    // Test implementation
}

TEST(RangeExpressions, InclusiveRange) {
    std::string source = "let r = 1..=5";
    // Test implementation
}

TEST(RangeExpressions, CharacterRange) {
    std::string source = "let r = 'a'..'z'";
    // Test implementation
}
```

### Integration Tests

Test your features in complete programs:

```javascript
// test_program.fn
function test_ranges():
{
    let nums = 1..10
    let sum = 0
    
    for n in nums:
    {
        sum = sum + n
    }
    
    print("Sum of 1..9:", sum)
    return sum == 45
}

let result = test_ranges()
print("Test passed:", result)
```

### Performance Tests

Measure the impact of your extensions:

```cpp
// benchmark_ranges.cpp
#include "benchmark/benchmark.h"

static void BM_RangeCreation(benchmark::State& state) {
    for (auto _ : state) {
        // Benchmark range creation
    }
}
BENCHMARK(BM_RangeCreation);
```

This extensibility guide provides a comprehensive framework for adding new features to Focus Nexus. By following these patterns and best practices, you can extend the language while maintaining code quality and consistency.