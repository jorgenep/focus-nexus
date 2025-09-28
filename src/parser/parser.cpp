#include "parser.hpp"

#include "../error/error_handler.hpp"
#include "../error/exceptions.hpp"

Parser::Parser(std::vector<Token> tokens) : tokens(std::move(tokens)) {}

std::vector<StmtPtr> Parser::parse() {
    std::vector<StmtPtr> statements;
    
    while (!isAtEnd()) {
        // Skip newlines at the top level
        if (match({TokenType::NEWLINE})) {
            continue;
        }
        
        try {
            if (auto stmt = declaration()) {
                statements.push_back(std::move(stmt));
            }
        } catch ([[maybe_unused]] const ParseError& error) {
            synchronize();
        }
    }
    
    return statements;
}

StmtPtr Parser::declaration() {
    try {
        if (match({TokenType::CLASS})) return classDeclaration();
        if (match({TokenType::EXTERN})) return externDeclaration();
        if (match({TokenType::PLUGIN})) return pluginDeclaration();
        if (match({TokenType::IMPORT})) return importStatement();
        if (match({TokenType::FUNCTION})) return functionStatement("function");
        if (match({TokenType::VAR, TokenType::LET})) return varDeclaration();
        return statement();
    } catch ([[maybe_unused]] const ParseError& error) {
        synchronize();
        return nullptr;
    }
}

StmtPtr Parser::statement() {
    if (match({TokenType::TRY})) return tryStatement();
    if (match({TokenType::THROW})) return throwStatement();
    if (match({TokenType::SWITCH})) return switchStatement();
    if (match({TokenType::IF})) return ifStatement();
    if (match({TokenType::PRINT})) return printStatement();
    if (match({TokenType::RETURN})) return returnStatement();
    if (match({TokenType::WHILE})) return whileStatement();
    if (match({TokenType::FOR})) return forStatement();
    if (match({TokenType::LEFT_BRACE})) return blockStatement();

    return expressionStatement();
}

StmtPtr Parser::printStatement() {
    auto value = expression();
    consume(TokenType::NEWLINE, "Expected newline after value");
    return std::make_unique<PrintStmt>(std::move(value));
}

StmtPtr Parser::returnStatement() {
    Token keyword = previous();
    ExprPtr value = nullptr;

    if (!check(TokenType::NEWLINE)) {
        value = expression();
    }

    consume(TokenType::NEWLINE, "Expected newline after return value");
    return std::make_unique<ReturnStmt>(keyword, std::move(value));
}

StmtPtr Parser::varDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expected variable name");

    ExprPtr initializer = nullptr;
    if (match({TokenType::EQUAL})) {
        initializer = expression();
    }

    consume(TokenType::NEWLINE, "Expected newline after variable declaration");
    return std::make_unique<VarStmt>(name, std::move(initializer));
}

StmtPtr Parser::expressionStatement() {
    auto expr = expression();
    consume(TokenType::NEWLINE, "Expected newline after expression");
    return std::make_unique<ExpressionStmt>(std::move(expr));
}

StmtPtr Parser::functionStatement(const std::string& kind) {
    Token name = consume(TokenType::IDENTIFIER, "Expected " + kind + " name");

    consume(TokenType::LEFT_PAREN, "Expected '(' after " + kind + " name");
    std::vector<Token> parameters;

    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            if (parameters.size() >= 255) {
                ErrorHandler::error(peek().line, peek().column, "Can't have more than 255 parameters");
            }
            parameters.push_back(consume(TokenType::IDENTIFIER, "Expected parameter name"));
        } while (match({TokenType::COMMA}));
    }

    consume(TokenType::RIGHT_PAREN, "Expected ')' after parameters");
    consume(TokenType::COLON, "Expected ':' before " + kind + " body");
    consume(TokenType::NEWLINE, "Expected newline after ':'");

    consume(TokenType::LEFT_BRACE, "Expected '{' before " + kind + " body");
    auto blockStmt = blockStatement();
    auto blockPtr = dynamic_cast<BlockStmt*>(blockStmt.get());
    std::vector<StmtPtr> body = std::move(blockPtr->statements);
    //blockStmt.release(); // Release ownership since we moved the statements

    return std::make_unique<FunctionStmt>(name, std::move(parameters), std::move(body));
}

StmtPtr Parser::ifStatement() {
    auto condition = expression();
    consume(TokenType::COLON, "Expected ':' after if condition");
    consume(TokenType::NEWLINE, "Expected newline after ':'");

    auto thenBranch = statement();
    StmtPtr elseBranch = nullptr;

    if (match({TokenType::ELSE})) {
        consume(TokenType::COLON, "Expected ':' after 'else'");
        consume(TokenType::NEWLINE, "Expected newline after ':'");
        elseBranch = statement();
    }

    return std::make_unique<IfStmt>(std::move(condition), std::move(thenBranch), std::move(elseBranch));
}

StmtPtr Parser::whileStatement() {
    auto condition = expression();
    consume(TokenType::COLON, "Expected ':' after while condition");
    consume(TokenType::NEWLINE, "Expected newline after ':'");

    auto body = statement();
    return std::make_unique<WhileStmt>(std::move(condition), std::move(body));
}

StmtPtr Parser::forStatement() {
    consume(TokenType::IDENTIFIER, "Expected variable name");
    Token variable = previous();
    consume(TokenType::EQUAL, "Expected '=' after for loop variable");
    auto initializer = std::make_unique<VarStmt>(variable, expression());

    consume(TokenType::SEMICOLON, "Expected ';' after for loop initializer");

    ExprPtr condition = nullptr;
    if (!check(TokenType::SEMICOLON)) {
        condition = expression();
    }
    consume(TokenType::SEMICOLON, "Expected ';' after for loop condition");

    ExprPtr increment = nullptr;
    if (!check(TokenType::COLON)) {
        increment = expression();
    }
    consume(TokenType::COLON, "Expected ':' after for clauses");
    consume(TokenType::NEWLINE, "Expected newline after ':'");

    auto body = statement();

    return std::make_unique<ForStmt>(std::move(initializer), std::move(condition),
                                   std::move(increment), std::move(body));
}

StmtPtr Parser::blockStatement() {
    std::vector<StmtPtr> statements;
    
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        if (match({TokenType::NEWLINE})) {
            continue;
        }
        if (auto stmt = declaration()) {
            statements.push_back(std::move(stmt));
        }
    }
    
    consume(TokenType::RIGHT_BRACE, "Expected '}' after block");
    return std::make_unique<BlockStmt>(std::move(statements));
}

StmtPtr Parser::classDeclaration() {
    Token name = consume(TokenType::IDENTIFIER, "Expected class name");
    
    ExprPtr superclass = nullptr;
    if (match({TokenType::EXTENDS})) {
        consume(TokenType::IDENTIFIER, "Expected superclass name");
        superclass = std::make_unique<VariableExpr>(previous());
    }
    
    consume(TokenType::COLON, "Expected ':' before class body");
    consume(TokenType::NEWLINE, "Expected newline after ':'");
    consume(TokenType::LEFT_BRACE, "Expected '{' before class body");
    
    std::vector<StmtPtr> methods;
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        if (match({TokenType::NEWLINE})) continue;
        methods.push_back(functionStatement("method"));
    }
    
    consume(TokenType::RIGHT_BRACE, "Expected '}' after class body");
    return std::make_unique<ClassStmt>(name, std::move(superclass), std::move(methods));
}

StmtPtr Parser::importStatement() {
    Token module = consume(TokenType::IDENTIFIER, "Expected module name");
    Token alias;
    std::vector<Token> items;
    
    if (match({TokenType::AS})) {
        alias = consume(TokenType::IDENTIFIER, "Expected alias name");
    }
    
    consume(TokenType::NEWLINE, "Expected newline after import");
    return std::make_unique<ImportStmt>(module, alias, std::move(items));
}

StmtPtr Parser::tryStatement() {
    consume(TokenType::COLON, "Expected ':' after 'try'");
    consume(TokenType::NEWLINE, "Expected newline after ':'");
    
    auto tryBlock = statement();
    
    Token catchVar;
    StmtPtr catchBlock = nullptr;
    
    if (match({TokenType::CATCH})) {
        if (match({TokenType::LEFT_PAREN})) {
            catchVar = consume(TokenType::IDENTIFIER, "Expected variable name");
            consume(TokenType::RIGHT_PAREN, "Expected ')' after catch variable");
        }
        consume(TokenType::COLON, "Expected ':' after catch");
        consume(TokenType::NEWLINE, "Expected newline after ':'");
        catchBlock = statement();
    }
    
    StmtPtr finallyBlock = nullptr;
    if (match({TokenType::FINALLY})) {
        consume(TokenType::COLON, "Expected ':' after 'finally'");
        consume(TokenType::NEWLINE, "Expected newline after ':'");
        finallyBlock = statement();
    }
    
    return std::make_unique<TryStmt>(std::move(tryBlock), catchVar, std::move(catchBlock), std::move(finallyBlock));
}

StmtPtr Parser::throwStatement() {
    auto value = expression();
    consume(TokenType::NEWLINE, "Expected newline after throw expression");
    return std::make_unique<ThrowStmt>(std::move(value));
}

StmtPtr Parser::switchStatement() {
    auto expr = expression();
    consume(TokenType::COLON, "Expected ':' after switch expression");
    consume(TokenType::NEWLINE, "Expected newline after ':'");
    consume(TokenType::LEFT_BRACE, "Expected '{' before switch body");
    
    std::vector<std::pair<ExprPtr, StmtPtr>> cases;
    StmtPtr defaultCase = nullptr;
    
    while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
        if (match({TokenType::NEWLINE})) continue;
        
        if (match({TokenType::CASE})) {
            auto caseExpr = expression();
            consume(TokenType::COLON, "Expected ':' after case value");
            consume(TokenType::NEWLINE, "Expected newline after ':'");
            auto caseStmt = statement();
            cases.emplace_back(std::move(caseExpr), std::move(caseStmt));
        } else if (match({TokenType::DEFAULT})) {
            consume(TokenType::COLON, "Expected ':' after 'default'");
            consume(TokenType::NEWLINE, "Expected newline after ':'");
            defaultCase = statement();
        } else {
            break;
        }
    }
    
    consume(TokenType::RIGHT_BRACE, "Expected '}' after switch body");
    return std::make_unique<SwitchStmt>(std::move(expr), std::move(cases), std::move(defaultCase));
}

StmtPtr Parser::externDeclaration() {
    Token libraryPath = consume(TokenType::STRING, "Expected library path");
    
    consume(TokenType::AS, "Expected 'as' after library path");
    Token alias = consume(TokenType::IDENTIFIER, "Expected library alias");
    
    std::string libraryType = "cpp"; // default
    if (match({TokenType::COLON})) {
        Token typeToken = consume(TokenType::IDENTIFIER, "Expected library type");
        libraryType = typeToken.lexeme;
    }
    
    std::vector<Token> functions;
    if (match({TokenType::LEFT_BRACE})) {
        while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
            if (match({TokenType::NEWLINE})) continue;
            functions.push_back(consume(TokenType::IDENTIFIER, "Expected function name"));
            if (!check(TokenType::RIGHT_BRACE)) {
                consume(TokenType::COMMA, "Expected ',' between function names");
            }
        }
        consume(TokenType::RIGHT_BRACE, "Expected '}' after function list");
    }
    
    consume(TokenType::NEWLINE, "Expected newline after extern declaration");
    return std::make_unique<ExternStmt>(libraryPath, alias, libraryType, std::move(functions));
}

StmtPtr Parser::pluginDeclaration() {
    Token pluginPath = consume(TokenType::STRING, "Expected plugin path");
    
    consume(TokenType::AS, "Expected 'as' after plugin path");
    Token alias = consume(TokenType::IDENTIFIER, "Expected plugin alias");
    
    std::vector<Token> exports;
    if (match({TokenType::LEFT_BRACE})) {
        while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
            if (match({TokenType::NEWLINE})) continue;
            exports.push_back(consume(TokenType::IDENTIFIER, "Expected export name"));
            if (!check(TokenType::RIGHT_BRACE)) {
                consume(TokenType::COMMA, "Expected ',' between export names");
            }
        }
        consume(TokenType::RIGHT_BRACE, "Expected '}' after export list");
    }
    
    consume(TokenType::NEWLINE, "Expected newline after plugin declaration");
    return std::make_unique<PluginStmt>(pluginPath, alias, std::move(exports));
}

ExprPtr Parser::expression() {
    return ternary();
}

ExprPtr Parser::ternary() {
    auto expr = assignment();
    
    if (match({TokenType::QUESTION})) {
        auto thenExpr = expression();
        consume(TokenType::COLON, "Expected ':' after ternary then expression");
        auto elseExpr = ternary();
        return std::make_unique<TernaryExpr>(std::move(expr), std::move(thenExpr), std::move(elseExpr));
    }
    
    return expr;
}

ExprPtr Parser::assignment() {
    auto expr = logicalOr();
    
    if (match({TokenType::EQUAL})) {
        Token equals = previous();
        auto value = assignment();
        
        if (auto var = dynamic_cast<VariableExpr*>(expr.get())) {
            Token name = var->name;
            //expr.release(); // Release ownership
            return std::make_unique<AssignExpr>(name, std::move(value));
        } else if (auto get = dynamic_cast<GetExpr*>(expr.get())) {
            return std::make_unique<SetExpr>(std::move(get->object), get->name, std::move(value));
        }
        
        ErrorHandler::error(equals.line, equals.column, "Invalid assignment target");
    }
    
    return expr;
}

ExprPtr Parser::logicalOr() {
    auto expr = logicalAnd();
    
    while (match({TokenType::OR})) {
        Token operator_ = previous();
        auto right = logicalAnd();
        expr = std::make_unique<BinaryExpr>(std::move(expr), operator_, std::move(right));
    }
    
    return expr;
}

ExprPtr Parser::logicalAnd() {
    auto expr = equality();
    
    while (match({TokenType::AND})) {
        Token operator_ = previous();
        auto right = equality();
        expr = std::make_unique<BinaryExpr>(std::move(expr), operator_, std::move(right));
    }
    
    return expr;
}

ExprPtr Parser::equality() {
    auto expr = comparison();
    
    while (match({TokenType::BANG_EQUAL, TokenType::EQUAL_EQUAL})) {
        Token operator_ = previous();
        auto right = comparison();
        expr = std::make_unique<BinaryExpr>(std::move(expr), operator_, std::move(right));
    }
    
    return expr;
}

ExprPtr Parser::comparison() {
    auto expr = term();
    
    while (match({TokenType::GREATER, TokenType::GREATER_EQUAL, TokenType::LESS, TokenType::LESS_EQUAL})) {
        Token operator_ = previous();
        auto right = term();
        expr = std::make_unique<BinaryExpr>(std::move(expr), operator_, std::move(right));
    }
    
    return expr;
}

ExprPtr Parser::term() {
    auto expr = factor();
    
    while (match({TokenType::MINUS, TokenType::PLUS})) {
        Token operator_ = previous();
        auto right = factor();
        expr = std::make_unique<BinaryExpr>(std::move(expr), operator_, std::move(right));
    }
    
    return expr;
}

ExprPtr Parser::factor() {
    auto expr = unary();
    
    while (match({TokenType::SLASH, TokenType::STAR})) {
        Token operator_ = previous();
        auto right = unary();
        expr = std::make_unique<BinaryExpr>(std::move(expr), operator_, std::move(right));
    }
    
    return expr;
}

ExprPtr Parser::unary() {
    if (match({TokenType::BANG, TokenType::MINUS})) {
        Token operator_ = previous();
        auto right = unary();
        return std::make_unique<UnaryExpr>(operator_, std::move(right));
    }
    
    return call();
}

ExprPtr Parser::call() {
    auto expr = primary();
    
    while (true) {
        if (match({TokenType::LEFT_PAREN})) {
            expr = finishCall(std::move(expr));
        } else if (match({TokenType::LEFT_BRACKET})) {
            auto index = expression();
            consume(TokenType::RIGHT_BRACKET, "Expected ']' after index");
            expr = std::make_unique<IndexExpr>(std::move(expr), std::move(index));
        } else if (match({TokenType::DOT})) {
            Token name = consume(TokenType::IDENTIFIER, "Expected property name after '.'");
            expr = std::make_unique<GetExpr>(std::move(expr), name);
        } else {
            break;
        }
    }
    
    return expr;
}

ExprPtr Parser::finishCall(ExprPtr callee) {
    std::vector<ExprPtr> arguments;
    
    if (!check(TokenType::RIGHT_PAREN)) {
        do {
            if (arguments.size() >= 255) {
                ErrorHandler::error(peek().line, peek().column, "Can't have more than 255 arguments");
            }
            arguments.push_back(expression());
        } while (match({TokenType::COMMA}));
    }
    
    Token paren = consume(TokenType::RIGHT_PAREN, "Expected ')' after arguments");
    return std::make_unique<CallExpr>(std::move(callee), paren, std::move(arguments));
}

ExprPtr Parser::primary() {
    if (match({TokenType::FALSE})) return std::make_unique<LiteralExpr>(Value(false));
    if (match({TokenType::TRUE})) return std::make_unique<LiteralExpr>(Value(true));
    if (match({TokenType::NIL})) return std::make_unique<LiteralExpr>(Value());
    if (match({TokenType::THIS})) return std::make_unique<ThisExpr>(previous());
    if (match({TokenType::SUPER})) {
        Token keyword = previous();
        consume(TokenType::DOT, "Expected '.' after 'super'");
        Token method = consume(TokenType::IDENTIFIER, "Expected superclass method name");
        return std::make_unique<SuperExpr>(keyword, method);
    }
    
    if (match({TokenType::LAMBDA})) {
        consume(TokenType::LEFT_PAREN, "Expected '(' after 'lambda'");
        std::vector<Token> parameters;
        
        if (!check(TokenType::RIGHT_PAREN)) {
            do {
                parameters.push_back(consume(TokenType::IDENTIFIER, "Expected parameter name"));
            } while (match({TokenType::COMMA}));
        }
        
        consume(TokenType::RIGHT_PAREN, "Expected ')' after parameters");
        consume(TokenType::COLON, "Expected ':' before lambda body");
        consume(TokenType::NEWLINE, "Expected newline after ':'");
        
        std::vector<StmtPtr> body;
        if (match({TokenType::LEFT_BRACE})) {
            while (!check(TokenType::RIGHT_BRACE) && !isAtEnd()) {
                if (match({TokenType::NEWLINE})) continue;
                if (auto stmt = declaration()) {
                    body.push_back(std::move(stmt));
                }
            }
            consume(TokenType::RIGHT_BRACE, "Expected '}' after lambda body");
        } else {
            // Single expression lambda
            auto expr = expression();
            body.push_back(std::make_unique<ReturnStmt>(Token(TokenType::RETURN, "return", "", 0, 0), std::move(expr)));
        }
        
        return std::make_unique<LambdaExpr>(std::move(parameters), std::move(body));
    }
    
    if (match({TokenType::NUMBER})) {
        double value = std::stod(previous().literal);
        return std::make_unique<LiteralExpr>(Value(value));
    }
    
    if (match({TokenType::STRING})) {
        return std::make_unique<LiteralExpr>(Value(previous().literal));
    }
    
    if (match({TokenType::IDENTIFIER})) {
        return std::make_unique<VariableExpr>(previous());
    }
    
    if (match({TokenType::LEFT_PAREN})) {
        auto expr = expression();
        consume(TokenType::RIGHT_PAREN, "Expected ')' after expression");
        return std::make_unique<GroupingExpr>(std::move(expr));
    }
    
    if (match({TokenType::LEFT_BRACKET})) {
        std::vector<ExprPtr> elements;
        
        if (!check(TokenType::RIGHT_BRACKET)) {
            do {
                elements.push_back(expression());
            } while (match({TokenType::COMMA}));
        }
        
        consume(TokenType::RIGHT_BRACKET, "Expected ']' after list elements");
        return std::make_unique<ListExpr>(std::move(elements));
    }
    
    if (match({TokenType::LOAD_LIBRARY})) {
        consume(TokenType::LEFT_PAREN, "Expected '(' after 'load_library'");
        Token libraryPath = consume(TokenType::STRING, "Expected library path");
        consume(TokenType::COMMA, "Expected ',' after library path");
        Token alias = consume(TokenType::IDENTIFIER, "Expected library alias");
        
        std::string libraryType = "cpp";
        if (match({TokenType::COMMA})) {
            Token typeToken = consume(TokenType::STRING, "Expected library type");
            libraryType = typeToken.literal;
        }
        
        consume(TokenType::RIGHT_PAREN, "Expected ')' after load_library arguments");
        return std::make_unique<LoadLibraryExpr>(libraryPath, alias, libraryType);
    }
    
    if (match({TokenType::CALL_NATIVE})) {
        consume(TokenType::LEFT_PAREN, "Expected '(' after 'call_native'");
        Token library = consume(TokenType::IDENTIFIER, "Expected library name");
        consume(TokenType::DOT, "Expected '.' after library name");
        Token function = consume(TokenType::IDENTIFIER, "Expected function name");
        
        std::vector<ExprPtr> arguments;
        if (match({TokenType::COMMA})) {
            do {
                arguments.push_back(expression());
            } while (match({TokenType::COMMA}));
        }
        
        consume(TokenType::RIGHT_PAREN, "Expected ')' after call_native arguments");
        return std::make_unique<ExternExpr>(library, function, std::move(arguments), "native");
    }
    
    throw ParseError("Expected expression at line " + std::to_string(peek().line));
}

bool Parser::match(std::initializer_list<TokenType> types) {
    for (TokenType type : types) { // NOLINT(*-use-anyofallof)
        if (check(type)) {
            advance();
            return true;
        }
    }
    return false;
}

Token Parser::advance() {
    if (!isAtEnd()) current++;
    return previous();
}

bool Parser::isAtEnd() {
    return peek().type == TokenType::EOF_TOKEN;
}

Token Parser::peek() {
    return tokens[current];
}

Token Parser::previous() {
    return tokens[current - 1];
}

bool Parser::check(TokenType type) {
    if (isAtEnd()) return false;
    return peek().type == type;
}

Token Parser::consume(TokenType type, const std::string& message) {
    if (check(type)) return advance();
    
    Token token = peek();
    throw ParseError(message + " at line " + std::to_string(token.line) + ", column " + std::to_string(token.column));
}

void Parser::synchronize() {
    advance();
    
    while (!isAtEnd()) {
        if (previous().type == TokenType::NEWLINE) return;
        
        switch (peek().type) {
            case TokenType::CLASS:
            case TokenType::FUNCTION:
            case TokenType::VAR:
            case TokenType::FOR:
            case TokenType::IF:
            case TokenType::WHILE:
            case TokenType::PRINT:
            case TokenType::RETURN:
                return;
            default:
                break;
        }
        
        advance();
    }
}