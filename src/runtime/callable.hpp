#pragma once
#include <functional>

#include "value.hpp"
#include <vector>

class Interpreter;

class Callable {
public:
    virtual ~Callable() = default;
    virtual int arity() = 0;
    virtual Value call(Interpreter& interpreter, std::vector<Value> arguments) = 0;
    virtual std::string toString() = 0;
};

class Function : public Callable {
private:
    class FunctionStmt* declaration;
    std::shared_ptr<class Environment> closure;

public:
    Function(class FunctionStmt* declaration, std::shared_ptr<class Environment> closure);
    
    int arity() override;
    Value call(Interpreter& interpreter, std::vector<Value> arguments) override;
    std::string toString() override;
};

class NativeFunction : public Callable {
private:
    std::function<Value(Interpreter&, std::vector<Value>)> function;
    int arity_;
    std::string name;

public:
    NativeFunction(std::function<Value(Interpreter&, std::vector<Value>)> function, 
                   int arity, std::string name);
    
    int arity() override;
    Value call(Interpreter& interpreter, std::vector<Value> arguments) override;
    std::string toString() override;
};