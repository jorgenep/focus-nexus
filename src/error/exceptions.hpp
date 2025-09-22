#pragma once
#include "../lexer/token.hpp"
#include "../runtime/value.hpp"
#include <stdexcept>
#include <string>

class ParseError : public std::runtime_error {
public:
    explicit ParseError(const std::string& message) : std::runtime_error(message) {}
};

class RuntimeError : public std::runtime_error {
public:
    Token token;
    
    RuntimeError(Token token, const std::string& message)
        : std::runtime_error(message), token(std::move(token)) {}
};

class ReturnValue : public std::runtime_error {
public:
    Value value;
    
    explicit ReturnValue(Value value) : std::runtime_error("return"), value(std::move(value)) {}
};

class BreakException : public std::runtime_error {
public:
    BreakException() : std::runtime_error("break") {}
};

class ContinueException : public std::runtime_error {
public:
    ContinueException() : std::runtime_error("continue") {}
};