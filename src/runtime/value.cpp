#include "value.hpp"
#include "callable.hpp"
#include <sstream>

bool Value::isNil() const {
    return std::holds_alternative<std::monostate>(value);
}

bool Value::isBool() const {
    return std::holds_alternative<bool>(value);
}

bool Value::isNumber() const {
    return std::holds_alternative<double>(value);
}

bool Value::isString() const {
    return std::holds_alternative<std::string>(value);
}

bool Value::isCallable() const {
    return std::holds_alternative<std::shared_ptr<Callable>>(value);
}

bool Value::isList() const {
    return std::holds_alternative<std::shared_ptr<std::vector<Value>>>(value);
}

bool Value::asBool() const {
    return std::get<bool>(value);
}

double Value::asNumber() const {
    return std::get<double>(value);
}

std::string Value::asString() const {
    return std::get<std::string>(value);
}

std::shared_ptr<Callable> Value::asCallable() const {
    return std::get<std::shared_ptr<Callable>>(value);
}

std::shared_ptr<std::vector<Value>> Value::asList() const {
    return std::get<std::shared_ptr<std::vector<Value>>>(value);
}

bool Value::isTruthy() const {
    if (isNil()) return false;
    if (isBool()) return asBool();
    if (isNumber()) return asNumber() != 0.0;
    if (isString()) return !asString().empty();
    return true;
}

std::string Value::toString() const {
    if (isNil()) return "nil";
    if (isBool()) return asBool() ? "true" : "false";
    if (isNumber()) {
        double num = asNumber();
        if (num == static_cast<int>(num)) {
            return std::to_string(static_cast<int>(num));
        }
        return std::to_string(num);
    }
    if (isString()) return asString();
    if (isCallable()) return "<function>";
    if (isList()) {
        std::ostringstream oss;
        oss << "[";
        auto list = asList();
        for (size_t i = 0; i < list->size(); ++i) {
            if (i > 0) oss << ", ";
            oss << (*list)[i].toString();
        }
        oss << "]";
        return oss.str();
    }
    return "<unknown>";
}

std::string Value::getType() const {
    if (isNil()) return "nil";
    if (isBool()) return "boolean";
    if (isNumber()) return "number";
    if (isString()) return "string";
    if (isCallable()) return "function";
    if (isList()) return "list";
    return "unknown";
}

bool Value::operator==(const Value& other) const {
    if (value.index() != other.value.index()) return false;
    
    if (isNil()) return true;
    if (isBool()) return asBool() == other.asBool();
    if (isNumber()) return asNumber() == other.asNumber();
    if (isString()) return asString() == other.asString();
    if (isCallable()) return asCallable() == other.asCallable();
    if (isList()) return asList() == other.asList();
    
    return false;
}

bool Value::operator!=(const Value& other) const {
    return !(*this == other);
}