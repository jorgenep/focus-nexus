#pragma once
#include <functional>
#include <unordered_map>

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

class FocusClass : public Callable, public std::enable_shared_from_this<FocusClass> {
private:
    std::string name;
    std::shared_ptr<FocusClass> superclass;
    std::unordered_map<std::string, std::shared_ptr<Function>> methods;

public:
    FocusClass(std::string name, std::shared_ptr<FocusClass> superclass,
               std::unordered_map<std::string, std::shared_ptr<Function>> methods);
    
    int arity() override;
    Value call(Interpreter& interpreter, std::vector<Value> arguments) override;
    std::string toString() override;
    
    std::shared_ptr<Function> findMethod(const std::string& name);
    std::string getName() const { return name; }
};

class FocusInstance : public std::enable_shared_from_this<FocusInstance> {
private:
    std::shared_ptr<FocusClass> klass;
    std::unordered_map<std::string, Value> fields;

public:
    explicit FocusInstance(std::shared_ptr<FocusClass> klass);
    
    Value get(const Token& name);
    void set(const Token& name, const Value& value);
    std::string toString();
};

class BoundMethod : public Callable {
private:
    Value instance;
    std::shared_ptr<Function> method;

public:
    BoundMethod(Value instance, std::shared_ptr<Function> method);
    
    int arity() override;
    Value call(Interpreter& interpreter, std::vector<Value> arguments) override;
    std::string toString() override;
};

class Lambda : public Callable {
private:
    std::vector<Token> params;
    std::vector<StmtPtr> body;
    std::shared_ptr<class Environment> closure;

public:
    Lambda(std::vector<Token> params, std::vector<StmtPtr> body, std::shared_ptr<class Environment> closure);
    
    int arity() override;
    Value call(Interpreter& interpreter, std::vector<Value> arguments) override;
    std::string toString() override;
    
    std::shared_ptr<Environment> getClosure() const { return closure; }
};