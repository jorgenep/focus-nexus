#pragma once
#include <string>
#include <vector>
#include <memory>
#include <variant>

class Callable;
class FocusClass;
class FocusInstance;

using ValueVariant = std::variant<
    std::monostate,  // nil
    bool,
    double,
    std::string,
    std::shared_ptr<Callable>,
    std::shared_ptr<std::vector<class Value>>,
    std::shared_ptr<FocusClass>,
    std::shared_ptr<FocusInstance>
>;

class Value {
private:
    ValueVariant value;

public:
    // Constructors
    Value() : value(std::monostate{}) {}
    explicit Value(bool b) : value(b) {}
    explicit Value(double d) : value(d) {}
    explicit Value(const std::string& s) : value(s) {}
    explicit Value(std::shared_ptr<Callable> c) : value(c) {}
    explicit Value(std::shared_ptr<std::vector<Value>> l) : value(l) {}
    explicit Value(std::shared_ptr<FocusClass> c) : value(c) {}
    explicit Value(std::shared_ptr<FocusInstance> i) : value(i) {}

    // Type checking
    [[nodiscard]] bool isNil() const;
    [[nodiscard]] bool isBool() const;
    [[nodiscard]] bool isNumber() const;
    [[nodiscard]] bool isString() const;
    [[nodiscard]] bool isCallable() const;
    [[nodiscard]] bool isList() const;
    [[nodiscard]] bool isClass() const;
    [[nodiscard]] bool isInstance() const;

    // Value extraction
    [[nodiscard]] bool asBool() const;
    [[nodiscard]] double asNumber() const;
    [[nodiscard]] std::string asString() const;
    [[nodiscard]] std::shared_ptr<Callable> asCallable() const;
    [[nodiscard]] std::shared_ptr<std::vector<Value>> asList() const;
    [[nodiscard]] std::shared_ptr<FocusClass> asClass() const;
    [[nodiscard]] std::shared_ptr<FocusInstance> asInstance() const;

    // Utility methods
    [[nodiscard]] bool isTruthy() const;
    [[nodiscard]] std::string toString() const;
    [[nodiscard]] std::string getType() const;

    // Operators
    bool operator==(const Value& other) const;
    bool operator!=(const Value& other) const;
};