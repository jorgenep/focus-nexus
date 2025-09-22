#pragma once
#include <string>
#include <unordered_map>

enum class TokenType {
    // Single-character tokens
    LEFT_PAREN, RIGHT_PAREN, LEFT_BRACE, RIGHT_BRACE,
    LEFT_BRACKET, RIGHT_BRACKET, COMMA, DOT, MINUS, PLUS,
    SEMICOLON, SLASH, STAR, COLON,

    // One or two character tokens
    BANG, BANG_EQUAL,
    EQUAL, EQUAL_EQUAL,
    GREATER, GREATER_EQUAL,
    LESS, LESS_EQUAL,

    // Literals
    IDENTIFIER, STRING, NUMBER,

    // Keywords
    AND, CLASS, ELSE, FALSE, FOR, FUNCTION, IF, NIL, OR,
    PRINT, RETURN, SUPER, THIS, TRUE, VAR, WHILE, LET,
    BREAK, CONTINUE,

    NEWLINE, EOF_TOKEN
};

struct Token {
    TokenType type;
    std::string lexeme;
    std::string literal;
    int line;
    int column;

    Token(TokenType type, std::string lexeme, std::string literal, int line, int column)
        : type(type), lexeme(std::move(lexeme)), literal(std::move(literal)), line(line), column(column) {}
};

class TokenUtils {
public:
    static std::unordered_map<std::string, TokenType> keywords;
    static std::string tokenTypeToString(TokenType type);
    static TokenType getKeywordType(const std::string& text);
};