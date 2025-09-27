#pragma once
#include "../lexer/token.hpp"
#include "../runtime/value.hpp"
#include <memory>
#include <vector>

// Forward declarations
class ASTVisitor;
class Expr;
class Stmt;

using ExprPtr = std::unique_ptr<Expr>;
using StmtPtr = std::unique_ptr<Stmt>;

// Base expression class
class Expr {
public:
    virtual ~Expr() = default;
    virtual Value accept(ASTVisitor& visitor) = 0;
};

// Expression types
class LambdaExpr : public Expr {
public:
    std::vector<Token> params;
    std::vector<StmtPtr> body;

    LambdaExpr(std::vector<Token> params, std::vector<StmtPtr> body)
        : params(std::move(params)), body(std::move(body)) {}

    Value accept(ASTVisitor& visitor) override;
};

class TernaryExpr : public Expr {
public:
    ExprPtr condition;
    ExprPtr thenExpr;
    ExprPtr elseExpr;

    TernaryExpr(ExprPtr condition, ExprPtr thenExpr, ExprPtr elseExpr)
        : condition(std::move(condition)), thenExpr(std::move(thenExpr)), elseExpr(std::move(elseExpr)) {}

    Value accept(ASTVisitor& visitor) override;
};

class SetExpr : public Expr {
public:
    ExprPtr object;
    Token name;
    ExprPtr value;

    SetExpr(ExprPtr object, Token name, ExprPtr value)
        : object(std::move(object)), name(std::move(name)), value(std::move(value)) {}

    Value accept(ASTVisitor& visitor) override;
};

class SuperExpr : public Expr {
public:
    Token keyword;
    Token method;

    SuperExpr(Token keyword, Token method)
        : keyword(std::move(keyword)), method(std::move(method)) {}

    Value accept(ASTVisitor& visitor) override;
};

class ThisExpr : public Expr {
public:
    Token keyword;

    explicit ThisExpr(Token keyword) : keyword(std::move(keyword)) {}

    Value accept(ASTVisitor& visitor) override;
};

class BinaryExpr : public Expr {
public:
    ExprPtr left;
    Token operator_;
    ExprPtr right;

    BinaryExpr(ExprPtr left, Token operator_, ExprPtr right)
        : left(std::move(left)), operator_(std::move(operator_)), right(std::move(right)) {}

    Value accept(ASTVisitor& visitor) override;
};

class UnaryExpr : public Expr {
public:
    Token operator_;
    ExprPtr right;

    UnaryExpr(Token operator_, ExprPtr right)
        : operator_(std::move(operator_)), right(std::move(right)) {}

    Value accept(ASTVisitor& visitor) override;
};

class LiteralExpr : public Expr {
public:
    Value value;

    explicit LiteralExpr(Value value) : value(std::move(value)) {}

    Value accept(ASTVisitor& visitor) override;
};

class GroupingExpr : public Expr {
public:
    ExprPtr expression;

    explicit GroupingExpr(ExprPtr expression) : expression(std::move(expression)) {}

    Value accept(ASTVisitor& visitor) override;
};

class VariableExpr : public Expr {
public:
    Token name;

    explicit VariableExpr(Token name) : name(std::move(name)) {}

    Value accept(ASTVisitor& visitor) override;
};

class AssignExpr : public Expr {
public:
    Token name;
    ExprPtr value;

    AssignExpr(Token name, ExprPtr value)
        : name(std::move(name)), value(std::move(value)) {}

    Value accept(ASTVisitor& visitor) override;
};

class CallExpr : public Expr {
public:
    ExprPtr callee;
    Token paren;
    std::vector<ExprPtr> arguments;

    CallExpr(ExprPtr callee, Token paren, std::vector<ExprPtr> arguments)
        : callee(std::move(callee)), paren(std::move(paren)), arguments(std::move(arguments)) {}

    Value accept(ASTVisitor& visitor) override;
};

class GetExpr : public Expr {
public:
    ExprPtr object;
    Token name;

    GetExpr(ExprPtr object, Token name)
        : object(std::move(object)), name(std::move(name)) {}

    Value accept(ASTVisitor& visitor) override;
};

class ListExpr : public Expr {
public:
    std::vector<ExprPtr> elements;

    explicit ListExpr(std::vector<ExprPtr> elements) : elements(std::move(elements)) {}

    Value accept(ASTVisitor& visitor) override;
};

class IndexExpr : public Expr {
public:
    ExprPtr object;
    ExprPtr index;

    IndexExpr(ExprPtr object, ExprPtr index)
        : object(std::move(object)), index(std::move(index)) {}

    Value accept(ASTVisitor& visitor) override;
};

// Base statement class
class Stmt {
public:
    virtual ~Stmt() = default;
    virtual void accept(ASTVisitor& visitor) = 0;
};

// Statement types
class ClassStmt : public Stmt {
public:
    Token name;
    ExprPtr superclass;
    std::vector<StmtPtr> methods;

    ClassStmt(Token name, ExprPtr superclass, std::vector<StmtPtr> methods)
        : name(std::move(name)), superclass(std::move(superclass)), methods(std::move(methods)) {}

    void accept(ASTVisitor& visitor) override;
};

class ImportStmt : public Stmt {
public:
    Token module;
    Token alias;
    std::vector<Token> items;

    ImportStmt(Token module, Token alias, std::vector<Token> items)
        : module(std::move(module)), alias(std::move(alias)), items(std::move(items)) {}

    void accept(ASTVisitor& visitor) override;
};

class TryStmt : public Stmt {
public:
    StmtPtr tryBlock;
    Token catchVar;
    StmtPtr catchBlock;
    StmtPtr finallyBlock;

    TryStmt(StmtPtr tryBlock, Token catchVar, StmtPtr catchBlock, StmtPtr finallyBlock)
        : tryBlock(std::move(tryBlock)), catchVar(std::move(catchVar)), 
          catchBlock(std::move(catchBlock)), finallyBlock(std::move(finallyBlock)) {}

    void accept(ASTVisitor& visitor) override;
};

class ThrowStmt : public Stmt {
public:
    ExprPtr value;

    explicit ThrowStmt(ExprPtr value) : value(std::move(value)) {}

    void accept(ASTVisitor& visitor) override;
};

class SwitchStmt : public Stmt {
public:
    ExprPtr expr;
    std::vector<std::pair<ExprPtr, StmtPtr>> cases;
    StmtPtr defaultCase;

    SwitchStmt(ExprPtr expr, std::vector<std::pair<ExprPtr, StmtPtr>> cases, StmtPtr defaultCase)
        : expr(std::move(expr)), cases(std::move(cases)), defaultCase(std::move(defaultCase)) {}

    void accept(ASTVisitor& visitor) override;
};

class ExpressionStmt : public Stmt {
public:
    ExprPtr expression;

    explicit ExpressionStmt(ExprPtr expression) : expression(std::move(expression)) {}

    void accept(ASTVisitor& visitor) override;
};

class PrintStmt : public Stmt {
public:
    ExprPtr expression;

    explicit PrintStmt(ExprPtr expression) : expression(std::move(expression)) {}

    void accept(ASTVisitor& visitor) override;
};

class VarStmt : public Stmt {
public:
    Token name;
    ExprPtr initializer;

    VarStmt(Token name, ExprPtr initializer)
        : name(std::move(name)), initializer(std::move(initializer)) {}

    void accept(ASTVisitor& visitor) override;
};

class BlockStmt : public Stmt {
public:
    std::vector<StmtPtr> statements;

    explicit BlockStmt(std::vector<StmtPtr> statements) : statements(std::move(statements)) {}

    void accept(ASTVisitor& visitor) override;
};

class IfStmt : public Stmt {
public:
    ExprPtr condition;
    StmtPtr thenBranch;
    StmtPtr elseBranch;

    IfStmt(ExprPtr condition, StmtPtr thenBranch, StmtPtr elseBranch)
        : condition(std::move(condition)), thenBranch(std::move(thenBranch)), elseBranch(std::move(elseBranch)) {}

    void accept(ASTVisitor& visitor) override;
};

class WhileStmt : public Stmt {
public:
    ExprPtr condition;
    StmtPtr body;

    WhileStmt(ExprPtr condition, StmtPtr body)
        : condition(std::move(condition)), body(std::move(body)) {}

    void accept(ASTVisitor& visitor) override;
};

class ForStmt : public Stmt {
public:
    StmtPtr initializer;
    ExprPtr condition;
    ExprPtr increment;
    StmtPtr body;

    ForStmt(StmtPtr initializer, ExprPtr condition, ExprPtr increment, StmtPtr body)
        : initializer(std::move(initializer)), condition(std::move(condition)),
          increment(std::move(increment)), body(std::move(body)) {}

    void accept(ASTVisitor& visitor) override;
};

class FunctionStmt : public Stmt {
public:
    Token name;
    std::vector<Token> params;
    std::vector<StmtPtr> body;

    FunctionStmt(Token name, std::vector<Token> params, std::vector<StmtPtr> body)
        : name(std::move(name)), params(std::move(params)), body(std::move(body)) {}

    void accept(ASTVisitor& visitor) override;
};

class ReturnStmt : public Stmt {
public:
    Token keyword;
    ExprPtr value;

    ReturnStmt(Token keyword, ExprPtr value)
        : keyword(std::move(keyword)), value(std::move(value)) {}

    void accept(ASTVisitor& visitor) override;
};

// Visitor interface
class ASTVisitor {
public:
    virtual ~ASTVisitor() = default;

    // Expression visitors
    virtual Value visitLambdaExpr(LambdaExpr& expr) = 0;
    virtual Value visitTernaryExpr(TernaryExpr& expr) = 0;
    virtual Value visitSetExpr(SetExpr& expr) = 0;
    virtual Value visitSuperExpr(SuperExpr& expr) = 0;
    virtual Value visitThisExpr(ThisExpr& expr) = 0;
    virtual Value visitBinaryExpr(BinaryExpr& expr) = 0;
    virtual Value visitUnaryExpr(UnaryExpr& expr) = 0;
    virtual Value visitLiteralExpr(LiteralExpr& expr) = 0;
    virtual Value visitGroupingExpr(GroupingExpr& expr) = 0;
    virtual Value visitVariableExpr(VariableExpr& expr) = 0;
    virtual Value visitAssignExpr(AssignExpr& expr) = 0;
    virtual Value visitCallExpr(CallExpr& expr) = 0;
    virtual Value visitGetExpr(GetExpr& expr) = 0;
    virtual Value visitListExpr(ListExpr& expr) = 0;
    virtual Value visitIndexExpr(IndexExpr& expr) = 0;

    // Statement visitors
    virtual void visitClassStmt(ClassStmt& stmt) = 0;
    virtual void visitImportStmt(ImportStmt& stmt) = 0;
    virtual void visitTryStmt(TryStmt& stmt) = 0;
    virtual void visitThrowStmt(ThrowStmt& stmt) = 0;
    virtual void visitSwitchStmt(SwitchStmt& stmt) = 0;
    virtual void visitExpressionStmt(ExpressionStmt& stmt) = 0;
    virtual void visitPrintStmt(PrintStmt& stmt) = 0;
    virtual void visitVarStmt(VarStmt& stmt) = 0;
    virtual void visitBlockStmt(BlockStmt& stmt) = 0;
    virtual void visitIfStmt(IfStmt& stmt) = 0;
    virtual void visitWhileStmt(WhileStmt& stmt) = 0;
    virtual void visitForStmt(ForStmt& stmt) = 0;
    virtual void visitFunctionStmt(FunctionStmt& stmt) = 0;
    virtual void visitReturnStmt(ReturnStmt& stmt) = 0;
};