#pragma once

#include "callable.hpp"
#include <memory>

// Forward declaration
class Interpreter;

// Function declarations for creating native functions
std::shared_ptr<Callable> createPrintFunction();
std::shared_ptr<Callable> createInputFunction();
std::shared_ptr<Callable> createLenFunction();
std::shared_ptr<Callable> createStrFunction();
std::shared_ptr<Callable> createNumFunction();
std::shared_ptr<Callable> createTypeFunction();
std::shared_ptr<Callable> createClockFunction();