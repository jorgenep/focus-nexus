#include "interpreter.hpp"
#include "runtime/callable.hpp"
#include "runtime/native_functions.hpp"
#include "error/error_handler.hpp"
#include "error/exceptions.hpp"
#include <cmath>
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
    globals->define("range", Value(createRangeFunction()));
    globals->define("map", Value(createMapFunction()));
    globals->define("filter", Value(createFilterFunction()));
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
        case TokenType::PERCENT:
            checkNumberOperands(expr.operator_, left, right);
            if (right.asNumber() == 0) {
                throw RuntimeError(expr.operator_, "Modulo by zero");
            }
            return Value(fmod(left.asNumber(), right.asNumber()));
        case TokenType::STAR_STAR:
            checkNumberOperands(expr.operator_, left, right);
            return Value(pow(left.asNumber(), right.asNumber()));
        case TokenType::LEFT_SHIFT:
            checkNumberOperands(expr.operator_, left, right);
            return Value(static_cast<double>(static_cast<int>(left.asNumber()) << static_cast<int>(right.asNumber())));
        case TokenType::RIGHT_SHIFT:
            checkNumberOperands(expr.operator_, left, right);
            return Value(static_cast<double>(static_cast<int>(left.asNumber()) >> static_cast<int>(right.asNumber())));
        case TokenType::AMPERSAND:
            checkNumberOperands(expr.operator_, left, right);
            return Value(static_cast<double>(static_cast<int>(left.asNumber()) & static_cast<int>(right.asNumber())));
        case TokenType::PIPE:
            checkNumberOperands(expr.operator_, left, right);
            return Value(static_cast<double>(static_cast<int>(left.asNumber()) | static_cast<int>(right.asNumber())));
        case TokenType::CARET:
            checkNumberOperands(expr.operator_, left, right);
            return Value(static_cast<double>(static_cast<int>(left.asNumber()) ^ static_cast<int>(right.asNumber())));
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
        case TokenType::TILDE:
            checkNumberOperand(expr.operator_, right);
            return Value(static_cast<double>(~static_cast<int>(right.asNumber())));
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
    Value object = evaluate(*expr.object);
    
    if (object.isInstance()) {
        return object.asInstance()->get(expr.name);
    }
    
    throw RuntimeError(expr.name, "Only instances have properties");
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

Value Interpreter::visitLambdaExpr(LambdaExpr& expr) {
    return Value(std::make_shared<Lambda>(expr.params, std::move(expr.body), environment));
}

Value Interpreter::visitTernaryExpr(TernaryExpr& expr) {
    Value condition = evaluate(*expr.condition);
    
    if (condition.isTruthy()) {
        return evaluate(*expr.thenExpr);
    } else {
        return evaluate(*expr.elseExpr);
    }
}

Value Interpreter::visitSetExpr(SetExpr& expr) {
    Value object = evaluate(*expr.object);
    
    if (!object.isInstance()) {
        throw RuntimeError(expr.name, "Only instances have fields");
    }
    
    Value value = evaluate(*expr.value);
    object.asInstance()->set(expr.name, value);
    return value;
}

Value Interpreter::visitSuperExpr(SuperExpr& expr) {
    // Simplified super implementation
    throw RuntimeError(expr.keyword, "Super not fully implemented");
}

Value Interpreter::visitThisExpr(ThisExpr& expr) {
    return environment->get(expr.keyword);
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

void Interpreter::visitClassStmt(ClassStmt& stmt) {
    std::shared_ptr<FocusClass> superclass = nullptr;
    if (stmt.superclass != nullptr) {
        Value superclassValue = evaluate(*stmt.superclass);
        if (!superclassValue.isClass()) {
            throw RuntimeError(Token(TokenType::IDENTIFIER, "superclass", "", 0, 0), 
                              "Superclass must be a class");
        }
        superclass = superclassValue.asClass();
    }
    
    environment->define(stmt.name.lexeme, Value());
    
    std::unordered_map<std::string, std::shared_ptr<Function>> methods;
    for (const auto& method : stmt.methods) {
        auto functionStmt = dynamic_cast<FunctionStmt*>(method.get());
        if (functionStmt) {
            auto function = std::make_shared<Function>(functionStmt, environment);
            methods[functionStmt->name.lexeme] = function;
        }
    }
    
    auto klass = std::make_shared<FocusClass>(stmt.name.lexeme, superclass, methods);
    environment->assign(stmt.name, Value(klass));
}

void Interpreter::visitImportStmt(ImportStmt& stmt) {
    // Simplified import - just define the module name
    environment->define(stmt.module.lexeme, Value("imported_module"));
    
    if (!stmt.alias.lexeme.empty()) {
        environment->define(stmt.alias.lexeme, Value("imported_module"));
    }
}

void Interpreter::visitTryStmt(TryStmt& stmt) {
    try {
        execute(*stmt.tryBlock);
    } catch (const RuntimeError& error) {
        if (stmt.catchBlock != nullptr) {
            auto catchEnv = std::make_shared<Environment>(environment);
            if (!stmt.catchVar.lexeme.empty()) {
                catchEnv->define(stmt.catchVar.lexeme, Value(error.what()));
            }
            
            auto previous = environment;
            environment = catchEnv;
            try {
                execute(*stmt.catchBlock);
            } catch (...) {
                environment = previous;
                throw;
            }
            environment = previous;
        }
    }
    
    if (stmt.finallyBlock != nullptr) {
        execute(*stmt.finallyBlock);
    }
}

void Interpreter::visitThrowStmt(ThrowStmt& stmt) {
    Value value = evaluate(*stmt.value);
    throw RuntimeError(Token(TokenType::THROW, "throw", "", 0, 0), value.toString());
}

void Interpreter::visitSwitchStmt(SwitchStmt& stmt) {
    Value switchValue = evaluate(*stmt.expr);
    
    for (const auto& caseStmt : stmt.cases) {
        Value caseValue = evaluate(*caseStmt.first);
        if (isEqual(switchValue, caseValue)) {
            execute(*caseStmt.second);
            return;
        }
    }
    
    if (stmt.defaultCase != nullptr) {
        execute(*stmt.defaultCase);
    }
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