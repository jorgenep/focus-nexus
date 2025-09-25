#include "interpreter.hpp"
#include "runtime/callable.hpp"
#include "runtime/native_functions.hpp"
#include "error/error_handler.hpp"
#include "error/exceptions.hpp"
#include <iostream>
#include <utility>

Interpreter::Interpreter() {
    globals = std::make_shared<Environment>();
    environment = globals;
    
    // Define native functions
    globals->define("print", Value(createPrintFunction()));
    globals->define("input", Value(createInputFunction()));
    globals->define("len", Value(createLenFunction()));
    globals->define("str", Value(createStrFunction()));
    globals->define("num", Value(createNumFunction()));
    globals->define("type", Value(createTypeFunction()));
    globals->define("clock", Value(createClockFunction()));
}

void Interpreter::interpret(const std::vector<StmtPtr>& statements) {
    try {
        for (const auto& statement : statements) {
            execute(*statement);
        }
    } catch (const RuntimeError& error) {
        ErrorHandler::runtimeError(error);
    }
}

void Interpreter::executeBlock(const std::vector<StmtPtr>& statements, 
                              std::shared_ptr<Environment> environment) {
    auto previous = this->environment;
    
    try {
        this->environment = std::move(environment);
        
        for (const auto& statement : statements) {
            execute(*statement);
        }
    } catch (...) {
        this->environment = previous;
        throw;
    }
    
    this->environment = previous;
}

Value Interpreter::evaluate(Expr& expr) {
    return expr.accept(*this);
}

void Interpreter::execute(Stmt& stmt) {
    stmt.accept(*this);
}

Value Interpreter::visitBinaryExpr(BinaryExpr& expr) {
    Value left = evaluate(*expr.left);
    Value right = evaluate(*expr.right);
    
    switch (expr.operator_.type) {
        case TokenType::GREATER:
            checkNumberOperands(expr.operator_, left, right);
            return Value(left.asNumber() > right.asNumber());
        case TokenType::GREATER_EQUAL:
            checkNumberOperands(expr.operator_, left, right);
            return Value(left.asNumber() >= right.asNumber());
        case TokenType::LESS:
            checkNumberOperands(expr.operator_, left, right);
            return Value(left.asNumber() < right.asNumber());
        case TokenType::LESS_EQUAL:
            checkNumberOperands(expr.operator_, left, right);
            return Value(left.asNumber() <= right.asNumber());
        case TokenType::BANG_EQUAL:
            return Value(!isEqual(left, right));
        case TokenType::EQUAL_EQUAL:
            return Value(isEqual(left, right));
        case TokenType::MINUS:
            checkNumberOperands(expr.operator_, left, right);
            return Value(left.asNumber() - right.asNumber());
        case TokenType::PLUS:
            if (left.isNumber() && right.isNumber()) {
                return Value(left.asNumber() + right.asNumber());
            }
            if (left.isString() || right.isString()) {
                return Value(left.toString() + right.toString());
            }
            throw RuntimeError(expr.operator_, "Operands must be two numbers or strings");
        case TokenType::SLASH:
            checkNumberOperands(expr.operator_, left, right);
            if (right.asNumber() == 0) {
                throw RuntimeError(expr.operator_, "Division by zero");
            }
            return Value(left.asNumber() / right.asNumber());
        case TokenType::STAR:
            checkNumberOperands(expr.operator_, left, right);
            return Value(left.asNumber() * right.asNumber());
        case TokenType::AND:
            if (!left.isTruthy()) return left;
            return right;
        case TokenType::OR:
            if (left.isTruthy()) return left;
            return right;
        default:
            break;
    }
    
    return {}; // Unreachable
}

Value Interpreter::visitUnaryExpr(UnaryExpr& expr) {
    Value right = evaluate(*expr.right);
    
    switch (expr.operator_.type) {
        case TokenType::BANG:
            return Value(!right.isTruthy());
        case TokenType::MINUS:
            checkNumberOperand(expr.operator_, right);
            return Value(-right.asNumber());
        default:
            break;
    }
    
    return {}; // Unreachable
}

Value Interpreter::visitLiteralExpr(LiteralExpr& expr) {
    return expr.value;
}

Value Interpreter::visitGroupingExpr(GroupingExpr& expr) {
    return evaluate(*expr.expression);
}

Value Interpreter::visitVariableExpr(VariableExpr& expr) {
    return environment->get(expr.name);
}

Value Interpreter::visitAssignExpr(AssignExpr& expr) {
    Value value = evaluate(*expr.value);
    environment->assign(expr.name, value);
    return value;
}

Value Interpreter::visitCallExpr(CallExpr& expr) {
    Value callee = evaluate(*expr.callee);
    
    std::vector<Value> arguments;
    for (const auto& argument : expr.arguments) {
        arguments.push_back(evaluate(*argument));
    }
    
    if (!callee.isCallable()) {
        throw RuntimeError(expr.paren, "Can only call functions and classes");
    }
    
    auto function = callee.asCallable();
    if (function->arity() != -1 && arguments.size() != function->arity()) {
        throw RuntimeError(expr.paren, "Expected " + std::to_string(function->arity()) + 
                          " arguments but got " + std::to_string(arguments.size()));
    }
    
    return function->call(*this, arguments);
}

Value Interpreter::visitGetExpr(GetExpr& expr) {
    // Not implemented for this basic interpreter
    throw RuntimeError(expr.name, "Property access not implemented");
}

Value Interpreter::visitListExpr(ListExpr& expr) {
    auto list = std::make_shared<std::vector<Value>>();
    
    for (const auto& element : expr.elements) {
        list->push_back(evaluate(*element));
    }
    
    return Value(list);
}

Value Interpreter::visitIndexExpr(IndexExpr& expr) {
    Value object = evaluate(*expr.object);
    Value index = evaluate(*expr.index);
    
    if (!object.isList()) {
        throw RuntimeError(Token(TokenType::LEFT_BRACKET, "[", "", 0, 0), 
                          "Only lists can be indexed");
    }
    
    if (!index.isNumber()) {
        throw RuntimeError(Token(TokenType::LEFT_BRACKET, "[", "", 0, 0), 
                          "List index must be a number");
    }
    
    auto list = object.asList();
    int idx = static_cast<int>(index.asNumber());
    
    if (idx < 0 || idx >= static_cast<int>(list->size())) {
        throw RuntimeError(Token(TokenType::LEFT_BRACKET, "[", "", 0, 0), 
                          "List index out of range");
    }
    
    return (*list)[idx];
}

void Interpreter::visitExpressionStmt(ExpressionStmt& stmt) {
    evaluate(*stmt.expression);
}

void Interpreter::visitPrintStmt(PrintStmt& stmt) {
    Value value = evaluate(*stmt.expression);
    std::cout << stringify(value) << std::endl;
}

void Interpreter::visitVarStmt(VarStmt& stmt) {
    Value value;
    if (stmt.initializer != nullptr) {
        value = evaluate(*stmt.initializer);
    }
    
    environment->define(stmt.name.lexeme, value);
}

void Interpreter::visitBlockStmt(BlockStmt& stmt) {
    executeBlock(stmt.statements, std::make_shared<Environment>(environment));
}

void Interpreter::visitIfStmt(IfStmt& stmt) {
    if (evaluate(*stmt.condition).isTruthy()) {
        execute(*stmt.thenBranch);
    } else if (stmt.elseBranch != nullptr) {
        execute(*stmt.elseBranch);
    }
}

void Interpreter::visitWhileStmt(WhileStmt& stmt) {
    while (evaluate(*stmt.condition).isTruthy()) {
        execute(*stmt.body);
    }
}

void Interpreter::visitForStmt(ForStmt& stmt) {
    auto forEnvironment = std::make_shared<Environment>(environment);
    auto previous = environment;
    
    try {
        environment = forEnvironment;
        
        if (stmt.initializer != nullptr) {
            execute(*stmt.initializer);
        }
        
        while (true) {
            if (stmt.condition != nullptr) {
                if (!evaluate(*stmt.condition).isTruthy()) break;
            }
            
            execute(*stmt.body);
            
            if (stmt.increment != nullptr) {
                evaluate(*stmt.increment);
            }
        }
    } catch (...) {
        environment = previous;
        throw;
    }
    
    environment = previous;
}

void Interpreter::visitFunctionStmt(FunctionStmt& stmt) {
    auto function = std::make_shared<Function>(&stmt, environment);
    environment->define(stmt.name.lexeme, Value(function));
}

void Interpreter::visitReturnStmt(ReturnStmt& stmt) {
    Value value;
    if (stmt.value != nullptr) {
        value = evaluate(*stmt.value);
    }
    
    throw ReturnValue(value);
}

bool Interpreter::isEqual(const Value& a, const Value& b) {
    return a == b;
}

void Interpreter::checkNumberOperand(const Token& operator_, const Value& operand) {
    if (!operand.isNumber()) {
        throw RuntimeError(operator_, "Operand must be a number");
    }
}

void Interpreter::checkNumberOperands(const Token& operator_, const Value& left, const Value& right) {
    if (!left.isNumber() || !right.isNumber()) {
        throw RuntimeError(operator_, "Operands must be numbers");
    }
}

std::string Interpreter::stringify(const Value& value) {
    return value.toString();
}