#include "lexer.hpp"
#include "../error/error_handler.hpp"

std::unordered_map<std::string, TokenType> TokenUtils::keywords = {
    {"and", TokenType::AND},
    {"class", TokenType::CLASS},
    {"else", TokenType::ELSE},
    {"false", TokenType::FALSE},
    {"for", TokenType::FOR},
    {"function", TokenType::FUNCTION},
    {"if", TokenType::IF},
    {"nil", TokenType::NIL},
    {"or", TokenType::OR},
    {"print", TokenType::PRINT},
    {"return", TokenType::RETURN},
    {"super", TokenType::SUPER},
    {"this", TokenType::THIS},
    {"true", TokenType::TRUE},
    {"var", TokenType::VAR},
    {"while", TokenType::WHILE},
    {"let", TokenType::LET},
    {"break", TokenType::BREAK},
    {"continue", TokenType::CONTINUE}
};

std::string TokenUtils::tokenTypeToString(TokenType type) {
    switch (type) {
        case TokenType::LEFT_PAREN: return "LEFT_PAREN";
        case TokenType::RIGHT_PAREN: return "RIGHT_PAREN";
        case TokenType::LEFT_BRACE: return "LEFT_BRACE";
        case TokenType::RIGHT_BRACE: return "RIGHT_BRACE";
        case TokenType::COMMA: return "COMMA";
        case TokenType::DOT: return "DOT";
        case TokenType::MINUS: return "MINUS";
        case TokenType::PLUS: return "PLUS";
        case TokenType::SEMICOLON: return "SEMICOLON";
        case TokenType::SLASH: return "SLASH";
        case TokenType::STAR: return "STAR";
        case TokenType::BANG: return "BANG";
        case TokenType::BANG_EQUAL: return "BANG_EQUAL";
        case TokenType::EQUAL: return "EQUAL";
        case TokenType::EQUAL_EQUAL: return "EQUAL_EQUAL";
        case TokenType::GREATER: return "GREATER";
        case TokenType::GREATER_EQUAL: return "GREATER_EQUAL";
        case TokenType::LESS: return "LESS";
        case TokenType::LESS_EQUAL: return "LESS_EQUAL";
        case TokenType::IDENTIFIER: return "IDENTIFIER";
        case TokenType::STRING: return "STRING";
        case TokenType::NUMBER: return "NUMBER";
        case TokenType::AND: return "AND";
        case TokenType::CLASS: return "CLASS";
        case TokenType::ELSE: return "ELSE";
        case TokenType::FALSE: return "FALSE";
        case TokenType::FOR: return "FOR";
        case TokenType::FUNCTION: return "FUNCTION";
        case TokenType::IF: return "IF";
        case TokenType::NIL: return "NIL";
        case TokenType::OR: return "OR";
        case TokenType::PRINT: return "PRINT";
        case TokenType::RETURN: return "RETURN";
        case TokenType::SUPER: return "SUPER";
        case TokenType::THIS: return "THIS";
        case TokenType::TRUE: return "TRUE";
        case TokenType::VAR: return "VAR";
        case TokenType::WHILE: return "WHILE";
        case TokenType::EOF_TOKEN: return "EOF";
        default: return "UNKNOWN";
    }
}

TokenType TokenUtils::getKeywordType(const std::string& text) {
    auto it = keywords.find(text);
    if (it != keywords.end()) {
        return it->second;
    }
    return TokenType::IDENTIFIER;
}

Lexer::Lexer(std::string source) : source(std::move(source)) {}

std::vector<Token> Lexer::scanTokens() {
    while (!isAtEnd()) {
        start = current;
        char c = advance();

        switch (c) {
            case '(': addToken(TokenType::LEFT_PAREN); break;
            case ')': addToken(TokenType::RIGHT_PAREN); break;
            case '{': addToken(TokenType::LEFT_BRACE); break;
            case '}': addToken(TokenType::RIGHT_BRACE); break;
            case '[': addToken(TokenType::LEFT_BRACKET); break;
            case ']': addToken(TokenType::RIGHT_BRACKET); break;
            case ',': addToken(TokenType::COMMA); break;
            case '.': addToken(TokenType::DOT); break;
            case '-': addToken(TokenType::MINUS); break;
            case '+': addToken(TokenType::PLUS); break;
            case ';': addToken(TokenType::SEMICOLON); break;
            case '*': addToken(TokenType::STAR); break;
            case ':': addToken(TokenType::COLON); break;
            case '!':
                addToken(match('=') ? TokenType::BANG_EQUAL : TokenType::BANG);
                break;
            case '=':
                addToken(match('=') ? TokenType::EQUAL_EQUAL : TokenType::EQUAL);
                break;
            case '<':
                addToken(match('=') ? TokenType::LESS_EQUAL : TokenType::LESS);
                break;
            case '>':
                addToken(match('=') ? TokenType::GREATER_EQUAL : TokenType::GREATER);
                break;
            case '/':
                if (match('/')) {
                    skipComment();
                } else if (match('*')) {
                    // Multi-line comment
                    while (!isAtEnd() && !(peek() == '*' && peekNext() == '/')) {
                        if (peek() == '\n') {
                            line++;
                            column = 0;
                        }
                        advance();
                    }
                    if (!isAtEnd()) {
                        advance(); // *
                        advance(); // /
                    }
                } else {
                    addToken(TokenType::SLASH);
                }
                break;
            case ' ':
            case '\r':
            case '\t':
                // Ignore whitespace
                break;
            case '\n':
                addToken(TokenType::NEWLINE);
                line++;
                column = 0;
                break;
            case '"': string(); break;
            default:
                if (isDigit(c)) {
                    number();
                } else if (isAlpha(c)) {
                    identifier();
                } else {
                    ErrorHandler::error(line, column, "Unexpected character: " + std::string(1, c));
                }
                break;
        }
    }

    tokens.emplace_back(TokenType::EOF_TOKEN, "", "", line, column);
    return tokens;
}

bool Lexer::isAtEnd() {
    return current >= source.length();
}

char Lexer::advance() {
    column++;
    return source[current++];
}

void Lexer::addToken(TokenType type) {
    addToken(type, "");
}

void Lexer::addToken(TokenType type, const std::string& literal) {
    std::string text = source.substr(start, current - start);
    tokens.emplace_back(type, text, literal, line, column - text.length());
}

bool Lexer::match(char expected) {
    if (isAtEnd()) return false;
    if (source[current] != expected) return false;

    current++;
    column++;
    return true;
}

char Lexer::peek() {
    if (isAtEnd()) return '\0';
    return source[current];
}

char Lexer::peekNext() {
    if (current + 1 >= source.length()) return '\0';
    return source[current + 1];
}

void Lexer::string() {
    std::string value;
    
    while (peek() != '"' && !isAtEnd()) {
        if (peek() == '\n') {
            line++;
            column = 0;
        }
        
        if (peek() == '\\') {
            advance(); // consume backslash
            char escaped = advance();
            switch (escaped) {
                case 'n': value += '\n'; break;
                case 't': value += '\t'; break;
                case 'r': value += '\r'; break;
                case '\\': value += '\\'; break;
                case '"': value += '"'; break;
                default: value += escaped; break;
            }
        } else {
            value += advance();
        }
    }

    if (isAtEnd()) {
        ErrorHandler::error(line, column, "Unterminated string");
        return;
    }

    // The closing "
    advance();
    addToken(TokenType::STRING, value);
}

void Lexer::number() {
    while (isDigit(peek())) advance();

    // Look for a fractional part
    if (peek() == '.' && isDigit(peekNext())) {
        // Consume the "."
        advance();

        while (isDigit(peek())) advance();
    }

    std::string value = source.substr(start, current - start);
    addToken(TokenType::NUMBER, value);
}

void Lexer::identifier() {
    while (isAlphaNumeric(peek())) advance();

    std::string text = source.substr(start, current - start);
    TokenType type = TokenUtils::getKeywordType(text);
    addToken(type);
}

bool Lexer::isDigit(char c) {
    return c >= '0' && c <= '9';
}

bool Lexer::isAlpha(char c) {
    return (c >= 'a' && c <= 'z') ||
           (c >= 'A' && c <= 'Z') ||
           c == '_';
}

bool Lexer::isAlphaNumeric(char c) {
    return isAlpha(c) || isDigit(c);
}

void Lexer::skipComment() {
    while (peek() != '\n' && !isAtEnd()) advance();
}