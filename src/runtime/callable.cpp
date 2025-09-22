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