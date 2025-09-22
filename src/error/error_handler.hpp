#pragma once
#include "../lexer/token.hpp"
#include <string>

class ErrorHandler {
private:
    static bool hadError;
    static bool hadRuntimeError;

public:
    static void error(int line, int column, const std::string& message);
    static void error(const Token& token, const std::string& message);
    static void runtimeError(const class RuntimeError& error);
    static void report(int line, int column, const std::string& where, const std::string& message);
    
    static bool getHadError() { return hadError; }
    static bool getHadRuntimeError() { return hadRuntimeError; }
    static void reset() { hadError = false; hadRuntimeError = false; }
};