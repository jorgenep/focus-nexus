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
        src/runtime/library_manager.cpp
        src/runtime/value.cpp
        src/error/error_handler.cpp
        src/utils/file_utils.cpp
        src/utils/string_utils.cpp
)

# Include directories so headers can be found
target_include_directories(focusNexus PRIVATE src)

# Link system libraries for dynamic loading
if(WIN32)
    target_link_libraries(focusNexus PRIVATE kernel32)
else()
    target_link_libraries(focusNexus PRIVATE dl)
endif()

# Optional: Link Python libraries if available
find_package(PkgConfig QUIET)
if(PkgConfig_FOUND)
    pkg_check_modules(PYTHON python3)
    if(PYTHON_FOUND)
        target_include_directories(focusNexus PRIVATE ${PYTHON_INCLUDE_DIRS})
        target_link_libraries(focusNexus PRIVATE ${PYTHON_LIBRARIES})
        target_compile_definitions(focusNexus PRIVATE PYTHON_SUPPORT)
    endif()
endif()

# Optional: Link Java JNI if available
find_package(JNI QUIET)
if(JNI_FOUND)
    target_include_directories(focusNexus PRIVATE ${JNI_INCLUDE_DIRS})
    target_link_libraries(focusNexus PRIVATE ${JNI_LIBRARIES})
    target_compile_definitions(focusNexus PRIVATE JNI_SUPPORT)
endif()