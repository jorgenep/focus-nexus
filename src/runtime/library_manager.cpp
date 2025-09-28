#include "library_manager.hpp"
#include "../error/exceptions.hpp"
#include <iostream>
#include <filesystem>

// Static member initialization
std::unique_ptr<LibraryManager> LibraryManager::instance = nullptr;
bool PythonLibraryInterface::pythonInitialized = false;
bool JavaLibraryInterface::jvmInitialized = false;

// LibraryManager Implementation
LibraryManager& LibraryManager::getInstance() {
    if (!instance) {
        instance = std::make_unique<LibraryManager>();
    }
    return *instance;
}

bool LibraryManager::loadLibrary(const std::string& alias, const std::string& path, const std::string& type) {
    try {
        std::shared_ptr<LibraryInterface> library;
        
        if (type == "cpp") {
            library = std::make_shared<CppLibraryInterface>(path);
        } else if (type == "python") {
            library = std::make_shared<PythonLibraryInterface>(path);
        } else if (type == "java") {
            library = std::make_shared<JavaLibraryInterface>(path);
        } else if (type == "custom") {
            library = std::make_shared<CustomPluginInterface>(path);
        } else {
            std::cerr << "Unknown library type: " << type << std::endl;
            return false;
        }
        
        libraries[alias] = library;
        return true;
    } catch (const std::exception& e) {
        std::cerr << "Failed to load library " << alias << ": " << e.what() << std::endl;
        return false;
    }
}

Value LibraryManager::callFunction(const std::string& library, const std::string& function, const std::vector<Value>& args) {
    auto it = libraries.find(library);
    if (it == libraries.end()) {
        throw RuntimeError(Token(TokenType::IDENTIFIER, library, "", 0, 0), 
                          "Library '" + library + "' not loaded");
    }
    
    return it->second->callFunction(function, args);
}

bool LibraryManager::hasLibrary(const std::string& alias) const {
    return libraries.find(alias) != libraries.end();
}

bool LibraryManager::hasFunction(const std::string& library, const std::string& function) const {
    auto it = libraries.find(library);
    if (it == libraries.end()) return false;
    return it->second->hasFunction(function);
}

void LibraryManager::unloadLibrary(const std::string& alias) {
    libraries.erase(alias);
}

void LibraryManager::unloadAllLibraries() {
    libraries.clear();
}

std::vector<std::string> LibraryManager::getLoadedLibraries() const {
    std::vector<std::string> result;
    for (const auto& pair : libraries) {
        result.push_back(pair.first);
    }
    return result;
}

std::string LibraryManager::getLibraryType(const std::string& alias) const {
    auto it = libraries.find(alias);
    if (it == libraries.end()) return "";
    return it->second->getType();
}

// CppLibraryInterface Implementation
CppLibraryInterface::CppLibraryInterface(const std::string& libraryPath) {
#ifdef _WIN32
    handle = LoadLibraryA(libraryPath.c_str());
    if (!handle) {
        throw std::runtime_error("Failed to load C++ library: " + libraryPath);
    }
#else
    handle = dlopen(libraryPath.c_str(), RTLD_LAZY);
    if (!handle) {
        throw std::runtime_error("Failed to load C++ library: " + std::string(dlerror()));
    }
#endif
}

CppLibraryInterface::~CppLibraryInterface() {
    if (handle) {
#ifdef _WIN32
        FreeLibrary(handle);
#else
        dlclose(handle);
#endif
    }
}

Value CppLibraryInterface::callFunction(const std::string& functionName, const std::vector<Value>& args) {
    auto it = functions.find(functionName);
    void* funcPtr = nullptr;
    
    if (it == functions.end()) {
        // Try to load the function
#ifdef _WIN32
        funcPtr = GetProcAddress(handle, functionName.c_str());
#else
        funcPtr = dlsym(handle, functionName.c_str());
#endif
        
        if (!funcPtr) {
            throw RuntimeError(Token(TokenType::IDENTIFIER, functionName, "", 0, 0),
                              "Function '" + functionName + "' not found in C++ library");
        }
        
        functions[functionName] = funcPtr;
    } else {
        funcPtr = it->second;
    }
    
    // Call the function (simplified - in reality, you'd need proper type marshalling)
    // This is a basic example that assumes functions return double and take double arguments
    typedef double (*SimpleFunc)(double, double, double, double, double);
    auto func = reinterpret_cast<SimpleFunc>(funcPtr);
    
    // Convert arguments to doubles (simplified)
    std::vector<double> doubleArgs;
    for (const auto& arg : args) {
        if (arg.isNumber()) {
            doubleArgs.push_back(arg.asNumber());
        } else {
            doubleArgs.push_back(0.0);
        }
    }
    
    // Pad with zeros if needed
    while (doubleArgs.size() < 5) {
        doubleArgs.push_back(0.0);
    }
    
    double result = func(doubleArgs[0], doubleArgs[1], doubleArgs[2], doubleArgs[3], doubleArgs[4]);
    return Value(result);
}

bool CppLibraryInterface::hasFunction(const std::string& functionName) const {
    if (functions.find(functionName) != functions.end()) {
        return true;
    }
    
    // Try to find the function in the library
#ifdef _WIN32
    void* funcPtr = GetProcAddress(handle, functionName.c_str());
#else
    void* funcPtr = dlsym(handle, functionName.c_str());
#endif
    
    return funcPtr != nullptr;
}

void CppLibraryInterface::registerFunction(const std::string& name, void* funcPtr) {
    functions[name] = funcPtr;
}

// PythonLibraryInterface Implementation (Stub)
PythonLibraryInterface::PythonLibraryInterface(const std::string& modulePath) 
    : moduleName(modulePath), pythonModule(nullptr) {
    if (!pythonInitialized) {
        initializePython();
    }
    
    // In a real implementation, you would:
    // 1. Import the Python module
    // 2. Get references to the functions
    // For now, this is a stub
    std::cout << "Python library interface created for: " << modulePath << std::endl;
}

PythonLibraryInterface::~PythonLibraryInterface() {
    // Cleanup Python objects
}

Value PythonLibraryInterface::callFunction(const std::string& functionName, const std::vector<Value>& args) {
    // Stub implementation
    std::cout << "Calling Python function: " << functionName << " with " << args.size() << " arguments" << std::endl;
    return Value(42.0); // Placeholder
}

bool PythonLibraryInterface::hasFunction(const std::string& functionName) const {
    // Stub implementation
    return true;
}

void PythonLibraryInterface::initializePython() {
    if (!pythonInitialized) {
        // Initialize Python interpreter
        // Py_Initialize();
        pythonInitialized = true;
        std::cout << "Python interpreter initialized" << std::endl;
    }
}

void PythonLibraryInterface::finalizePython() {
    if (pythonInitialized) {
        // Py_Finalize();
        pythonInitialized = false;
        std::cout << "Python interpreter finalized" << std::endl;
    }
}

// JavaLibraryInterface Implementation (Stub)
JavaLibraryInterface::JavaLibraryInterface(const std::string& classPath) 
    : className(classPath), jvm(nullptr), env(nullptr), javaClass(nullptr) {
    if (!jvmInitialized) {
        initializeJVM();
    }
    
    // In a real implementation, you would:
    // 1. Load the Java class
    // 2. Get method IDs
    // For now, this is a stub
    std::cout << "Java library interface created for: " << classPath << std::endl;
}

JavaLibraryInterface::~JavaLibraryInterface() {
    // Cleanup JNI objects
}

Value JavaLibraryInterface::callFunction(const std::string& functionName, const std::vector<Value>& args) {
    // Stub implementation
    std::cout << "Calling Java method: " << functionName << " with " << args.size() << " arguments" << std::endl;
    return Value(84.0); // Placeholder
}

bool JavaLibraryInterface::hasFunction(const std::string& functionName) const {
    // Stub implementation
    return true;
}

void JavaLibraryInterface::initializeJVM() {
    if (!jvmInitialized) {
        // Initialize JVM
        // JNI_CreateJavaVM(&jvm, (void**)&env, &vm_args);
        jvmInitialized = true;
        std::cout << "JVM initialized" << std::endl;
    }
}

void JavaLibraryInterface::destroyJVM() {
    if (jvmInitialized) {
        // Destroy JVM
        jvmInitialized = false;
        std::cout << "JVM destroyed" << std::endl;
    }
}

// CustomPluginInterface Implementation
CustomPluginInterface::CustomPluginInterface(const std::string& pluginPath) {
#ifdef _WIN32
    handle = LoadLibraryA(pluginPath.c_str());
    if (!handle) {
        throw std::runtime_error("Failed to load custom plugin: " + pluginPath);
    }
#else
    handle = dlopen(pluginPath.c_str(), RTLD_LAZY);
    if (!handle) {
        throw std::runtime_error("Failed to load custom plugin: " + std::string(dlerror()));
    }
#endif

    // Call plugin initialization function
#ifdef _WIN32
    auto initFunc = reinterpret_cast<PluginInitFunc>(GetProcAddress(handle, "focus_nexus_plugin_init"));
#else
    auto initFunc = reinterpret_cast<PluginInitFunc>(dlsym(handle, "focus_nexus_plugin_init"));
#endif
    
    if (initFunc) {
        initFunc();
    }
}

CustomPluginInterface::~CustomPluginInterface() {
    if (handle) {
        // Call plugin cleanup function
#ifdef _WIN32
        auto cleanupFunc = reinterpret_cast<PluginCleanupFunc>(GetProcAddress(handle, "focus_nexus_plugin_cleanup"));
#else
        auto cleanupFunc = reinterpret_cast<PluginCleanupFunc>(dlsym(handle, "focus_nexus_plugin_cleanup"));
#endif
        
        if (cleanupFunc) {
            cleanupFunc();
        }

#ifdef _WIN32
        FreeLibrary(handle);
#else
        dlclose(handle);
#endif
    }
}

Value CustomPluginInterface::callFunction(const std::string& functionName, const std::vector<Value>& args) {
    auto it = functions.find(functionName);
    if (it != functions.end()) {
        return it->second(args);
    }
    
    // Try to load the function from the plugin
    std::string symbolName = "focus_nexus_" + functionName;
#ifdef _WIN32
    auto funcPtr = reinterpret_cast<PluginFunction>(GetProcAddress(handle, symbolName.c_str()));
#else
    auto funcPtr = reinterpret_cast<PluginFunction>(dlsym(handle, symbolName.c_str()));
#endif
    
    if (!funcPtr) {
        throw RuntimeError(Token(TokenType::IDENTIFIER, functionName, "", 0, 0),
                          "Function '" + functionName + "' not found in custom plugin");
    }
    
    return funcPtr(args);
}

bool CustomPluginInterface::hasFunction(const std::string& functionName) const {
    if (functions.find(functionName) != functions.end()) {
        return true;
    }
    
    std::string symbolName = "focus_nexus_" + functionName;
#ifdef _WIN32
    void* funcPtr = GetProcAddress(handle, symbolName.c_str());
#else
    void* funcPtr = dlsym(handle, symbolName.c_str());
#endif
    
    return funcPtr != nullptr;
}