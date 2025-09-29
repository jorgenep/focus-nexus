#include "callable.hpp"

#include <utility>
#include "../parser/ast.hpp"
#include "../interpreter.hpp"
#include "environment.hpp"
#include "../error/exceptions.hpp"

Function::Function(FunctionStmt* declaration, std::shared_ptr<Environment> closure)
    : declaration(declaration), closure(std::move(closure)) {}

int Function::arity() {
    return declaration->params.size();
}

Value Function::call(Interpreter& interpreter, std::vector<Value> arguments) {
    auto environment = std::make_shared<Environment>(closure);
    
    for (size_t i = 0; i < declaration->params.size(); i++) {
        environment->define(declaration->params[i].lexeme, arguments[i]);
    }
    
    try {
        interpreter.executeBlock(declaration->body, environment);
    } catch (const ReturnValue& returnValue) {
        return returnValue.value;
    }
    
    return {}; // nil
}

std::string Function::toString() {
    return "<fn " + declaration->name.lexeme + ">";
}

NativeFunction::NativeFunction(std::function<Value(Interpreter&, std::vector<Value>)> function, int arity, std::string name) : function(std::move(function)), arity_(arity), name(std::move(name)) {}

int NativeFunction::arity() {
    return arity_;
}

Value NativeFunction::call(Interpreter& interpreter, std::vector<Value> arguments) {
    return function(interpreter, arguments);
}

std::string NativeFunction::toString() {
    return "<native fn " + name + ">";
}

// FocusClass implementation
FocusClass::FocusClass(std::string name, std::shared_ptr<FocusClass> superclass,
                       std::unordered_map<std::string, std::shared_ptr<Function>> methods)
    : name(std::move(name)), superclass(std::move(superclass)), methods(std::move(methods)) {}

int FocusClass::arity() {
    auto initializer = findMethod("init");
    if (initializer == nullptr) return 0;
    return initializer->arity();
}

Value FocusClass::call(Interpreter& interpreter, std::vector<Value> arguments) {
    auto instance = std::make_shared<FocusInstance>(shared_from_this());
    auto initializer = findMethod("init");
    if (initializer != nullptr) {
        auto bound = std::make_shared<BoundMethod>(Value(instance), initializer);
        bound->call(interpreter, arguments);
    }
    return Value(instance);
}

std::string FocusClass::toString() {
    return "<class " + name + ">";
}

std::shared_ptr<Function> FocusClass::findMethod(const std::string& name) {
    auto it = methods.find(name);
    if (it != methods.end()) {
        return it->second;
    }
    
    if (superclass != nullptr) {
        return superclass->findMethod(name);
    }
    
    return nullptr;
}

// FocusInstance implementation
FocusInstance::FocusInstance(std::shared_ptr<FocusClass> klass) : klass(std::move(klass)) {}

Value FocusInstance::get(const Token& name) {
    auto it = fields.find(name.lexeme);
    if (it != fields.end()) {
        return it->second;
    }
    
    auto method = klass->findMethod(name.lexeme);
    if (method != nullptr) {
        return Value(std::make_shared<BoundMethod>(Value(shared_from_this()), method));
    }
    
    throw RuntimeError(name, "Undefined property '" + name.lexeme + "'");
}

void FocusInstance::set(const Token& name, const Value& value) {
    fields[name.lexeme] = value;
}

std::string FocusInstance::toString() {
    return "<" + klass->getName() + " instance>";
}

// BoundMethod implementation
BoundMethod::BoundMethod(Value instance, std::shared_ptr<Function> method)
    : instance(std::move(instance)), method(std::move(method)) {}

int BoundMethod::arity() {
    return method->arity();
}

Value BoundMethod::call(Interpreter& interpreter, std::vector<Value> arguments) {
    auto environment = std::make_shared<Environment>(method->getClosure());
    environment->define("this", instance);
    
    for (size_t i = 0; i < method->declaration->params.size(); i++) {
        environment->define(method->declaration->params[i].lexeme, arguments[i]);
    }
    
    try {
        interpreter.executeBlock(method->declaration->body, environment);
    } catch (const ReturnValue& returnValue) {
        return returnValue.value;
    }
    
    return {};
}

std::string BoundMethod::toString() {
    return "<bound method>";
}

// Lambda implementation
Lambda::Lambda(std::vector<Token> params, std::vector<StmtPtr> body, std::shared_ptr<Environment> closure)
    : params(std::move(params)), body(std::move(body)), closure(std::move(closure)) {}

int Lambda::arity() {
    return params.size();
}

Value Lambda::call(Interpreter& interpreter, std::vector<Value> arguments) {
    auto environment = std::make_shared<Environment>(closure);
    
    for (size_t i = 0; i < params.size(); i++) {
        environment->define(params[i].lexeme, arguments[i]);
    }
    
    try {
        interpreter.executeBlock(body, environment);
    } catch (const ReturnValue& returnValue) {
        return returnValue.value;
    }
    
    return {};
}

std::string Lambda::toString() {
    return "<lambda>";
}