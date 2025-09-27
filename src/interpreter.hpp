#pragma once

#include "parser/ast.hpp"
#include "runtime/environment.hpp"
#include "runtime/value.hpp"
#include <memory>

class Interpreter : public ASTVisitor {
private:
    std::shared_ptr<Environment> globals;
    std::shared_ptr<Environment> environment;

public:
    Interpreter();
    
    void interpret(const std::vector<StmtPtr>& statements);
    void executeBlock(const std::vector<StmtPtr>& statements, std::shared_ptr<Environment> environment);
    
    // Expression visitors
    Value visitLambdaExpr(LambdaExpr& expr) override;
    Value visitTernaryExpr(TernaryExpr& expr) override;
    Value visitSetExpr(SetExpr& expr) override;
    Value visitSuperExpr(SuperExpr& expr) override;
    Value visitThisExpr(ThisExpr& expr) override;
    Value visitBinaryExpr(BinaryExpr& expr) override;
    Value visitUnaryExpr(UnaryExpr& expr) override;
    Value visitLiteralExpr(LiteralExpr& expr) override;
    Value visitGroupingExpr(GroupingExpr& expr) override;
    Value visitVariableExpr(VariableExpr& expr) override;
    Value visitAssignExpr(AssignExpr& expr) override;
    Value visitCallExpr(CallExpr& expr) override;
    Value visitGetExpr(GetExpr& expr) override;
    Value visitListExpr(ListExpr& expr) override;
    Value visitIndexExpr(IndexExpr& expr) override;
    
    // Statement visitors
    void visitClassStmt(ClassStmt& stmt) override;
    void visitImportStmt(ImportStmt& stmt) override;
    void visitTryStmt(TryStmt& stmt) override;
    void visitThrowStmt(ThrowStmt& stmt) override;
    void visitSwitchStmt(SwitchStmt& stmt) override;
    void visitExpressionStmt(ExpressionStmt& stmt) override;
    void visitPrintStmt(PrintStmt& stmt) override;
    void visitVarStmt(VarStmt& stmt) override;
    void visitBlockStmt(BlockStmt& stmt) override;
    void visitIfStmt(IfStmt& stmt) override;
    void visitWhileStmt(WhileStmt& stmt) override;
    void visitForStmt(ForStmt& stmt) override;
    void visitFunctionStmt(FunctionStmt& stmt) override;
    void visitReturnStmt(ReturnStmt& stmt) override;

private:
    Value evaluate(Expr& expr);
    void execute(Stmt& stmt);
    static bool isEqual(const Value& a, const Value& b);
    static void checkNumberOperand(const Token& operator_, const Value& operand);
    static void checkNumberOperands(const Token& operator_, const Value& left, const Value& right);
    static std::string stringify(const Value& value);
};