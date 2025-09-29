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

std::shared_ptr<Callable> createRangeFunction() {
    return std::make_shared<NativeFunction>(
        [](Interpreter& interpreter, std::vector<Value> arguments) -> Value {
            if (arguments.size() < 1 || arguments.size() > 3) {
                throw std::runtime_error("range() takes 1 to 3 arguments");
            }
            
            double start = 0, stop, step = 1;
            
            if (arguments.size() == 1) {
                stop = arguments[0].asNumber();
            } else if (arguments.size() == 2) {
                start = arguments[0].asNumber();
                stop = arguments[1].asNumber();
            } else {
                start = arguments[0].asNumber();
                stop = arguments[1].asNumber();
                step = arguments[2].asNumber();
            }
            
            auto list = std::make_shared<std::vector<Value>>();
            for (double i = start; (step > 0 ? i < stop : i > stop); i += step) {
                list->push_back(Value(i));
            }
            
            return Value(list);
        },
        -1,
        "range"
    );
}

std::shared_ptr<Callable> createMapFunction() {
    return std::make_shared<NativeFunction>(
        [](Interpreter& interpreter, std::vector<Value> arguments) -> Value {
            if (arguments.size() != 2) {
                throw std::runtime_error("map() takes exactly 2 arguments");
            }
            
            if (!arguments[0].isCallable() || !arguments[1].isList()) {
                throw std::runtime_error("map() requires a function and a list");
            }
            
            auto func = arguments[0].asCallable();
            auto list = arguments[1].asList();
            auto result = std::make_shared<std::vector<Value>>();
            
            for (const auto& item : *list) {
                std::vector<Value> args = {item};
                result->push_back(func->call(interpreter, args));
            }
            
            return Value(result);
        },
        2,
        "map"
    );
}

std::shared_ptr<Callable> createFilterFunction() {
    return std::make_shared<NativeFunction>(
        [](Interpreter& interpreter, std::vector<Value> arguments) -> Value {
            if (arguments.size() != 2) {
                throw std::runtime_error("filter() takes exactly 2 arguments");
            }
            
            if (!arguments[0].isCallable() || !arguments[1].isList()) {
                throw std::runtime_error("filter() requires a function and a list");
            }
            
            auto func = arguments[0].asCallable();
            auto list = arguments[1].asList();
            auto result = std::make_shared<std::vector<Value>>();
            
            for (const auto& item : *list) {
                std::vector<Value> args = {item};
                if (func->call(interpreter, args).isTruthy()) {
                    result->push_back(item);
                }
            }
            
            return Value(result);
        },
        2,
        "filter"
    );
}