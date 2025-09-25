#include "callable.hpp"
#include "../interpreter.hpp"
#include <iostream>
#include <chrono>

std::shared_ptr<Callable> createPrintFunction() {
    return std::make_shared<NativeFunction>(
        [](Interpreter& interpreter, std::vector<Value> arguments) -> Value {
            for (size_t i = 0; i < arguments.size(); ++i) {
                if (i > 0) std::cout << " ";
                std::cout << arguments[i].toString();
            }
            std::cout << std::endl;
            return {}; // nil
        },
        -1, // variadic
        "print"
    );
}

std::shared_ptr<Callable> createInputFunction() {
    return std::make_shared<NativeFunction>(
        [](Interpreter& interpreter, std::vector<Value> arguments) -> Value {
            if (!arguments.empty()) {
                std::cout << arguments[0].toString();
            }
            std::string input;
            std::getline(std::cin, input);
            return Value(input);
        },
        -1, // 0 or 1 argument
        "input"
    );
}

std::shared_ptr<Callable> createLenFunction() {
    return std::make_shared<NativeFunction>(
        [](Interpreter& interpreter, std::vector<Value> arguments) -> Value {
            if (arguments.size() != 1) {
                throw std::runtime_error("len() takes exactly one argument");
            }
            
            const Value& arg = arguments[0];
            if (arg.isString()) {
                return Value(static_cast<double>(arg.asString().length()));
            } else if (arg.isList()) {
                return Value(static_cast<double>(arg.asList()->size()));
            } else {
                throw std::runtime_error("Object of type '" + arg.getType() + "' has no len()");
            }
        },
        1,
        "len"
    );
}

std::shared_ptr<Callable> createStrFunction() {
    return std::make_shared<NativeFunction>(
        [](Interpreter& interpreter, std::vector<Value> arguments) -> Value {
            if (arguments.size() != 1) {
                throw std::runtime_error("str() takes exactly one argument");
            }
            return Value(arguments[0].toString());
        },
        1,
        "str"
    );
}

std::shared_ptr<Callable> createNumFunction() {
    return std::make_shared<NativeFunction>(
        [](Interpreter& interpreter, std::vector<Value> arguments) -> Value {
            if (arguments.size() != 1) {
                throw std::runtime_error("num() takes exactly one argument");
            }
            
            Value arg = arguments[0];
            if (arg.isNumber()) {
                return arg;
            } else if (arg.isString()) {
                try {
                    return Value(std::stod(arg.asString()));
                } catch (const std::exception&) {
                    throw std::runtime_error("Cannot convert '" + arg.asString() + "' to number");
                }
            } else {
                throw std::runtime_error("Cannot convert " + arg.getType() + " to number");
            }
        },
        1,
        "num"
    );
}

std::shared_ptr<Callable> createTypeFunction() {
    return std::make_shared<NativeFunction>(
        [](Interpreter& interpreter, std::vector<Value> arguments) -> Value {
            if (arguments.size() != 1) {
                throw std::runtime_error("type() takes exactly one argument");
            }
            return Value(arguments[0].getType());
        },
        1,
        "type"
    );
}

std::shared_ptr<Callable> createClockFunction() {
    return std::make_shared<NativeFunction>(
        [](Interpreter& interpreter, const std::vector<Value>& arguments) -> Value {
            auto now = std::chrono::high_resolution_clock::now();
            auto duration = now.time_since_epoch();
            auto millis = std::chrono::duration_cast<std::chrono::milliseconds>(duration).count();
            return Value(static_cast<double>(millis) / 1000.0);
        },
        0,
        "clock"
    );
}