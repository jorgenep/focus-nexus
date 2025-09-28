#include "ast.hpp"

// AST node accept method implementations
Value LambdaExpr::accept(ASTVisitor& visitor) {
    return visitor.visitLambdaExpr(*this);
}

Value TernaryExpr::accept(ASTVisitor& visitor) {
    return visitor.visitTernaryExpr(*this);
}

Value SetExpr::accept(ASTVisitor& visitor) {
    return visitor.visitSetExpr(*this);
}

Value SuperExpr::accept(ASTVisitor& visitor) {
    return visitor.visitSuperExpr(*this);
}

Value ThisExpr::accept(ASTVisitor& visitor) {
    return visitor.visitThisExpr(*this);
}

Value BinaryExpr::accept(ASTVisitor& visitor) {
    return visitor.visitBinaryExpr(*this);
}

Value UnaryExpr::accept(ASTVisitor& visitor) {
    return visitor.visitUnaryExpr(*this);
}

Value LiteralExpr::accept(ASTVisitor& visitor) {
    return visitor.visitLiteralExpr(*this);
}

Value GroupingExpr::accept(ASTVisitor& visitor) {
    return visitor.visitGroupingExpr(*this);
}

Value VariableExpr::accept(ASTVisitor& visitor) {
    return visitor.visitVariableExpr(*this);
}

Value AssignExpr::accept(ASTVisitor& visitor) {
    return visitor.visitAssignExpr(*this);
}

Value CallExpr::accept(ASTVisitor& visitor) {
    return visitor.visitCallExpr(*this);
}

Value GetExpr::accept(ASTVisitor& visitor) {
    return visitor.visitGetExpr(*this);
}

Value ListExpr::accept(ASTVisitor& visitor) {
    return visitor.visitListExpr(*this);
}

Value IndexExpr::accept(ASTVisitor& visitor) {
    return visitor.visitIndexExpr(*this);
}

Value ExternExpr::accept(ASTVisitor& visitor) {
    return visitor.visitExternExpr(*this);
}

Value LoadLibraryExpr::accept(ASTVisitor& visitor) {
    return visitor.visitLoadLibraryExpr(*this);
}

void ClassStmt::accept(ASTVisitor& visitor) {
    visitor.visitClassStmt(*this);
}

void ImportStmt::accept(ASTVisitor& visitor) {
    visitor.visitImportStmt(*this);
}

void TryStmt::accept(ASTVisitor& visitor) {
    visitor.visitTryStmt(*this);
}

void ThrowStmt::accept(ASTVisitor& visitor) {
    visitor.visitThrowStmt(*this);
}

void SwitchStmt::accept(ASTVisitor& visitor) {
    visitor.visitSwitchStmt(*this);
}

void ExternStmt::accept(ASTVisitor& visitor) {
    visitor.visitExternStmt(*this);
}

void PluginStmt::accept(ASTVisitor& visitor) {
    visitor.visitPluginStmt(*this);
}

void ExpressionStmt::accept(ASTVisitor& visitor) {
    visitor.visitExpressionStmt(*this);
}

void PrintStmt::accept(ASTVisitor& visitor) {
    visitor.visitPrintStmt(*this);
}

void VarStmt::accept(ASTVisitor& visitor) {
    visitor.visitVarStmt(*this);
}

void BlockStmt::accept(ASTVisitor& visitor) {
    visitor.visitBlockStmt(*this);
}

void IfStmt::accept(ASTVisitor& visitor) {
    visitor.visitIfStmt(*this);
}

void WhileStmt::accept(ASTVisitor& visitor) {
    visitor.visitWhileStmt(*this);
}

void ForStmt::accept(ASTVisitor& visitor) {
    visitor.visitForStmt(*this);
}

void FunctionStmt::accept(ASTVisitor& visitor) {
    visitor.visitFunctionStmt(*this);
}

void ReturnStmt::accept(ASTVisitor& visitor) {
    visitor.visitReturnStmt(*this);
}