cmake_minimum_required(VERSION 3.16)
project(focusNexus)

set(CMAKE_CXX_STANDARD 17)

# Add all source files with correct paths
add_executable(focusNexus
        src/main.cpp
        src/interpreter.cpp
        src/lexer/lexer.cpp
        src/parser/parser.cpp
        src/parser/ast_printer.cpp
        src/runtime/callable.cpp
        src/runtime/environment.cpp
        src/runtime/native_functions.cpp
        src/runtime/value.cpp
        src/error/error_handler.cpp
        src/utils/file_utils.cpp
        src/utils/string_utils.cpp
)

# Include directories so headers can be found
target_include_directories(focusNexus PRIVATE src)