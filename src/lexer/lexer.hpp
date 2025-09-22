#pragma once
#include "token.hpp"
#include <vector>
#include <string>

class Lexer {
private:
    std::string source;
    std::vector<Token> tokens;
    int start = 0;
    int current = 0;
    int line = 1;
    int column = 1;

    bool isAtEnd();
    char advance();
    void addToken(TokenType type);
    void addToken(TokenType type, const std::string& literal);
    bool match(char expected);
    char peek();
    char peekNext();
    void string();
    void number();
    void identifier();
    static bool isDigit(char c);
    static bool isAlpha(char c);
    static bool isAlphaNumeric(char c);
    void skipWhitespace();
    void skipComment();

public:
    explicit Lexer(std::string source);
    std::vector<Token> scanTokens();
};