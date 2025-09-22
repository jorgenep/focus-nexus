#include "error_handler.hpp"
#include "exceptions.hpp"
#include <iostream>

bool ErrorHandler::hadError = false;
bool ErrorHandler::hadRuntimeError = false;

void ErrorHandler::error(int line, int column, const std::string& message) {
    report(line, column, "", message);
}

void ErrorHandler::error(const Token& token, const std::string& message) {
    if (token.type == TokenType::EOF_TOKEN) {
        report(token.line, token.column, " at end", message);
    } else {
        report(token.line, token.column, " at '" + token.lexeme + "'", message);
    }
}

void ErrorHandler::runtimeError(const RuntimeError& error) {
    std::cerr << "[line " << error.token.line << ", column " << error.token.column << "] Runtime Error: " 
              << error.what() << std::endl;
    hadRuntimeError = true;
}

void ErrorHandler::report(int line, int column, const std::string& where, const std::string& message) {
    std::cerr << "[line " << line << ", column " << column << "] Error" << where << ": " << message << std::endl;
    hadError = true;
}