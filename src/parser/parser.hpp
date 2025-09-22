#pragma once
#include "ast.hpp"
#include "../lexer/token.hpp"
#include <vector>

class Parser {
private:
    std::vector<Token> tokens;
    int current = 0;

    // Helper methods
    bool match(std::initializer_list<TokenType> types);
    Token advance();
    bool isAtEnd();
    Token peek();
    Token previous();
    bool check(TokenType type);
    Token consume(TokenType type, const std::string& message);
    void synchronize();

    // Expression parsing
    ExprPtr expression();
    ExprPtr assignment();
    ExprPtr logicalOr();
    ExprPtr logicalAnd();
    ExprPtr equality();
    ExprPtr comparison();
    ExprPtr term();
    ExprPtr factor();
    ExprPtr unary();
    ExprPtr call();
    ExprPtr finishCall(ExprPtr callee);
    ExprPtr primary();

    // Statement parsing
    StmtPtr statement();
    StmtPtr printStatement();
    StmtPtr expressionStatement();
    StmtPtr varDeclaration();
    StmtPtr blockStatement();
    StmtPtr ifStatement();
    StmtPtr whileStatement();
    StmtPtr forStatement();
    StmtPtr functionStatement(const std::string& kind);
    StmtPtr returnStatement();
    StmtPtr declaration();

public:
    explicit Parser(std::vector<Token> tokens);
    std::vector<StmtPtr> parse();
};