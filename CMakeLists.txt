cmake_minimum_required(VERSION 3.16)
project(focusNexus)

set(CMAKE_CXX_STANDARD 17)

# Library integration options
option(PYTHON_SUPPORT "Enable Python library integration" OFF)
option(JNI_SUPPORT "Enable Java library integration" OFF)
option(CUSTOM_PLUGIN_SUPPORT "Enable custom plugin support" ON)

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
        src/runtime/library_manager.cpp
        src/runtime/value.cpp
        src/error/error_handler.cpp
        src/utils/file_utils.cpp
        src/utils/string_utils.cpp
)

# Include directories so headers can be found
target_include_directories(focusNexus PRIVATE src)

# Python support
if(PYTHON_SUPPORT)
    find_package(Python3 COMPONENTS Interpreter Development REQUIRED)
    if(Python3_FOUND)
        target_include_directories(focusNexus PRIVATE ${Python3_INCLUDE_DIRS})
        target_link_libraries(focusNexus PRIVATE ${Python3_LIBRARIES})
        target_compile_definitions(focusNexus PRIVATE PYTHON_SUPPORT)
        message(STATUS "Python support enabled")
    else()
        message(WARNING "Python3 not found, disabling Python support")
    endif()
endif()

# Java support
if(JNI_SUPPORT)
    find_package(JNI REQUIRED)
    if(JNI_FOUND)
        target_include_directories(focusNexus PRIVATE ${JNI_INCLUDE_DIRS})
        target_link_libraries(focusNexus PRIVATE ${JNI_LIBRARIES})
        target_compile_definitions(focusNexus PRIVATE JNI_SUPPORT)
        message(STATUS "Java support enabled")
    else()
        message(WARNING "JNI not found, disabling Java support")
    endif()
endif()

# Link system libraries for dynamic loading
if(WIN32)
    target_link_libraries(focusNexus PRIVATE kernel32)
else()
    target_link_libraries(focusNexus PRIVATE dl)
endif()