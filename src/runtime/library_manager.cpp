#include "library_manager.hpp"
#include "../error/exceptions.hpp"
#include <iostream>
#include <filesystem>

#ifdef PYTHON_SUPPORT
#include <Python.h>
#endif

#ifdef JNI_SUPPORT
#include <jni.h>
#endif

// Static member initialization
std::unique_ptr<LibraryManager> LibraryManager::instance = nullptr;
bool PythonLibraryInterface::pythonInitialized = false;
bool JavaLibraryInterface::jvmInitialized = false;

#ifdef JNI_SUPPORT
JavaVM* JavaLibraryInterface::jvm = nullptr;
JNIEnv* JavaLibraryInterface::env = nullptr;
#endif

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
        DWORD error = GetLastError();
        throw std::runtime_error("Failed to load C++ library: " + libraryPath + " (Error: " + std::to_string(error) + ")");
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
    void* funcPtr = nullptr;
    
    // Try to get cached function pointer
    auto it = functions.find(functionName);
    if (it != functions.end()) {
        funcPtr = it->second;
    } else {
        // Load the function
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
    }
    
    // Handle different function signatures based on argument count and types
    if (args.empty()) {
        // No arguments - double func()
        typedef double (*NoArgFunc)();
        auto func = reinterpret_cast<NoArgFunc>(funcPtr);
        return Value(func());
    } else if (args.size() == 1) {
        if (args[0].isNumber()) {
            // One number argument - double func(double)
            typedef double (*OneNumFunc)(double);
            auto func = reinterpret_cast<OneNumFunc>(funcPtr);
            return Value(func(args[0].asNumber()));
        } else if (args[0].isString()) {
            // One string argument - const char* func(const char*)
            typedef const char* (*OneStrFunc)(const char*);
            auto func = reinterpret_cast<OneStrFunc>(funcPtr);
            const char* result = func(args[0].asString().c_str());
            return Value(std::string(result ? result : ""));
        }
    } else if (args.size() == 2) {
        if (args[0].isNumber() && args[1].isNumber()) {
            // Two number arguments - double func(double, double)
            typedef double (*TwoNumFunc)(double, double);
            auto func = reinterpret_cast<TwoNumFunc>(funcPtr);
            return Value(func(args[0].asNumber(), args[1].asNumber()));
        }
    } else if (args.size() == 3) {
        if (args[0].isNumber() && args[1].isNumber() && args[2].isNumber()) {
            // Three number arguments - double func(double, double, double)
            typedef double (*ThreeNumFunc)(double, double, double);
            auto func = reinterpret_cast<ThreeNumFunc>(funcPtr);
            return Value(func(args[0].asNumber(), args[1].asNumber(), args[2].asNumber()));
        }
    } else if (args.size() == 4) {
        if (args[0].isNumber() && args[1].isNumber() && args[2].isNumber() && args[3].isNumber()) {
            // Four number arguments - double func(double, double, double, double)
            typedef double (*FourNumFunc)(double, double, double, double);
            auto func = reinterpret_cast<FourNumFunc>(funcPtr);
            return Value(func(args[0].asNumber(), args[1].asNumber(), args[2].asNumber(), args[3].asNumber()));
        }
    }
    
    // Fallback: try to call as variadic function with up to 5 arguments
    typedef double (*VariadicFunc)(double, double, double, double, double);
    auto func = reinterpret_cast<VariadicFunc>(funcPtr);
    
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

// PythonLibraryInterface Implementation
PythonLibraryInterface::PythonLibraryInterface(const std::string& modulePath) 
    : moduleName(modulePath), pythonModule(nullptr) {
#ifdef PYTHON_SUPPORT
    if (!pythonInitialized) {
        initializePython();
    }
    
    // Extract module name from path
    std::string modName = modulePath;
    size_t lastSlash = modName.find_last_of("/\\");
    if (lastSlash != std::string::npos) {
        modName = modName.substr(lastSlash + 1);
    }
    size_t lastDot = modName.find_last_of('.');
    if (lastDot != std::string::npos) {
        modName = modName.substr(0, lastDot);
    }
    
    // Import the module
    PyObject* pName = PyUnicode_DecodeFSDefault(modName.c_str());
    pythonModule = PyImport_Import(pName);
    Py_DECREF(pName);
    
    if (!pythonModule) {
        PyErr_Print();
        throw std::runtime_error("Failed to import Python module: " + modulePath);
    }
#else
    throw std::runtime_error("Python support not compiled in");
#endif
}

PythonLibraryInterface::~PythonLibraryInterface() {
#ifdef PYTHON_SUPPORT
    if (pythonModule) {
        Py_DECREF(static_cast<PyObject*>(pythonModule));
    }
#endif
}

Value PythonLibraryInterface::callFunction(const std::string& functionName, const std::vector<Value>& args) {
#ifdef PYTHON_SUPPORT
    if (!pythonModule) {
        throw RuntimeError(Token(TokenType::IDENTIFIER, functionName, "", 0, 0),
                          "Python module not loaded");
    }
    
    PyObject* pModule = static_cast<PyObject*>(pythonModule);
    PyObject* pFunc = PyObject_GetAttrString(pModule, functionName.c_str());
    
    if (!pFunc || !PyCallable_Check(pFunc)) {
        Py_XDECREF(pFunc);
        throw RuntimeError(Token(TokenType::IDENTIFIER, functionName, "", 0, 0),
                          "Function '" + functionName + "' not found or not callable in Python module");
    }
    
    // Convert arguments to Python objects
    PyObject* pArgs = PyTuple_New(args.size());
    for (size_t i = 0; i < args.size(); i++) {
        PyObject* pValue = nullptr;
        
        if (args[i].isNumber()) {
            pValue = PyFloat_FromDouble(args[i].asNumber());
        } else if (args[i].isString()) {
            pValue = PyUnicode_FromString(args[i].asString().c_str());
        } else if (args[i].isBool()) {
            pValue = PyBool_FromLong(args[i].asBool() ? 1 : 0);
        } else {
            pValue = Py_None;
            Py_INCREF(Py_None);
        }
        
        PyTuple_SetItem(pArgs, i, pValue);
    }
    
    // Call the function
    PyObject* pResult = PyObject_CallObject(pFunc, pArgs);
    Py_DECREF(pArgs);
    Py_DECREF(pFunc);
    
    if (!pResult) {
        PyErr_Print();
        throw RuntimeError(Token(TokenType::IDENTIFIER, functionName, "", 0, 0),
                          "Python function call failed");
    }
    
    // Convert result back to Focus Nexus Value
    Value result;
    if (PyFloat_Check(pResult)) {
        result = Value(PyFloat_AsDouble(pResult));
    } else if (PyLong_Check(pResult)) {
        result = Value(static_cast<double>(PyLong_AsLong(pResult)));
    } else if (PyUnicode_Check(pResult)) {
        const char* str = PyUnicode_AsUTF8(pResult);
        result = Value(std::string(str ? str : ""));
    } else if (PyBool_Check(pResult)) {
        result = Value(PyObject_IsTrue(pResult) == 1);
    } else if (pResult == Py_None) {
        result = Value(); // nil
    } else if (PyList_Check(pResult)) {
        // Convert Python list to Focus Nexus list
        auto list = std::make_shared<std::vector<Value>>();
        Py_ssize_t size = PyList_Size(pResult);
        for (Py_ssize_t i = 0; i < size; i++) {
            PyObject* item = PyList_GetItem(pResult, i);
            if (PyFloat_Check(item)) {
                list->push_back(Value(PyFloat_AsDouble(item)));
            } else if (PyLong_Check(item)) {
                list->push_back(Value(static_cast<double>(PyLong_AsLong(item))));
            } else if (PyUnicode_Check(item)) {
                const char* str = PyUnicode_AsUTF8(item);
                list->push_back(Value(std::string(str ? str : "")));
            } else {
                list->push_back(Value()); // nil for unknown types
            }
        }
        result = Value(list);
    } else {
        // Try to convert to string as fallback
        PyObject* pStr = PyObject_Str(pResult);
        if (pStr) {
            const char* str = PyUnicode_AsUTF8(pStr);
            result = Value(std::string(str ? str : ""));
            Py_DECREF(pStr);
        } else {
            result = Value(); // nil
        }
    }
    
    Py_DECREF(pResult);
    return result;
#else
    throw std::runtime_error("Python support not compiled in");
#endif
}

bool PythonLibraryInterface::hasFunction(const std::string& functionName) const {
#ifdef PYTHON_SUPPORT
    if (!pythonModule) return false;
    
    PyObject* pModule = static_cast<PyObject*>(pythonModule);
    PyObject* pFunc = PyObject_GetAttrString(pModule, functionName.c_str());
    
    if (pFunc && PyCallable_Check(pFunc)) {
        Py_DECREF(pFunc);
        return true;
    }
    
    Py_XDECREF(pFunc);
    return false;
#else
    return false;
#endif
}

void PythonLibraryInterface::initializePython() {
#ifdef PYTHON_SUPPORT
    if (!pythonInitialized) {
        Py_Initialize();
        if (!Py_IsInitialized()) {
            throw std::runtime_error("Failed to initialize Python interpreter");
        }
        
        // Add current directory to Python path
        PyRun_SimpleString("import sys");
        PyRun_SimpleString("sys.path.append('.')");
        PyRun_SimpleString("sys.path.append('./examples/python_library')");
        
        pythonInitialized = true;
        std::cout << "Python interpreter initialized" << std::endl;
    }
#endif
}

void PythonLibraryInterface::finalizePython() {
#ifdef PYTHON_SUPPORT
    if (pythonInitialized) {
        Py_Finalize();
        pythonInitialized = false;
        std::cout << "Python interpreter finalized" << std::endl;
    }
#endif
}

// JavaLibraryInterface Implementation
JavaLibraryInterface::JavaLibraryInterface(const std::string& classPath) 
    : className(classPath), javaClass(nullptr) {
#ifdef JNI_SUPPORT
    if (!jvmInitialized) {
        initializeJVM();
    }
    
    // Find the class
    jclass cls = env->FindClass(classPath.c_str());
    if (!cls) {
        env->ExceptionClear();
        throw std::runtime_error("Java class not found: " + classPath);
    }
    
    javaClass = env->NewGlobalRef(cls);
    env->DeleteLocalRef(cls);
    
    if (!javaClass) {
        throw std::runtime_error("Failed to create global reference for Java class: " + classPath);
    }
#else
    throw std::runtime_error("Java support not compiled in");
#endif
}

JavaLibraryInterface::~JavaLibraryInterface() {
#ifdef JNI_SUPPORT
    if (javaClass && env) {
        env->DeleteGlobalRef(static_cast<jobject>(javaClass));
    }
#endif
}

Value JavaLibraryInterface::callFunction(const std::string& functionName, const std::vector<Value>& args) {
#ifdef JNI_SUPPORT
    if (!javaClass || !env) {
        throw RuntimeError(Token(TokenType::IDENTIFIER, functionName, "", 0, 0),
                          "Java class not loaded");
    }
    
    jclass cls = static_cast<jclass>(javaClass);
    
    // Build method signature based on arguments
    std::string signature = "(";
    std::vector<jvalue> jArgs;
    jArgs.reserve(args.size());
    
    for (const auto& arg : args) {
        if (arg.isNumber()) {
            signature += "D"; // double
            jvalue jVal;
            jVal.d = arg.asNumber();
            jArgs.push_back(jVal);
        } else if (arg.isString()) {
            signature += "Ljava/lang/String;";
            jvalue jVal;
            jVal.l = env->NewStringUTF(arg.asString().c_str());
            jArgs.push_back(jVal);
        } else if (arg.isBool()) {
            signature += "Z"; // boolean
            jvalue jVal;
            jVal.z = arg.asBool() ? JNI_TRUE : JNI_FALSE;
            jArgs.push_back(jVal);
        } else {
            signature += "Ljava/lang/Object;";
            jvalue jVal;
            jVal.l = nullptr;
            jArgs.push_back(jVal);
        }
    }
    
    // Try different return types
    std::vector<std::string> returnTypes = {"D", "Ljava/lang/String;", "Z", "J", "I", "V"};
    
    for (const std::string& returnType : returnTypes) {
        std::string fullSignature = signature + ")" + returnType;
        
        jmethodID method = env->GetStaticMethodID(cls, functionName.c_str(), fullSignature.c_str());
        if (method) {
            if (returnType == "D") {
                // double return
                jdouble result = env->CallStaticDoubleMethodA(cls, method, jArgs.data());
                if (env->ExceptionCheck()) {
                    env->ExceptionClear();
                    continue;
                }
                return Value(static_cast<double>(result));
            } else if (returnType == "Ljava/lang/String;") {
                // String return
                jobject result = env->CallStaticObjectMethodA(cls, method, jArgs.data());
                if (env->ExceptionCheck()) {
                    env->ExceptionClear();
                    continue;
                }
                if (result) {
                    const char* str = env->GetStringUTFChars(static_cast<jstring>(result), nullptr);
                    std::string cppStr(str);
                    env->ReleaseStringUTFChars(static_cast<jstring>(result), str);
                    env->DeleteLocalRef(result);
                    return Value(cppStr);
                }
                return Value("");
            } else if (returnType == "Z") {
                // boolean return
                jboolean result = env->CallStaticBooleanMethodA(cls, method, jArgs.data());
                if (env->ExceptionCheck()) {
                    env->ExceptionClear();
                    continue;
                }
                return Value(result == JNI_TRUE);
            } else if (returnType == "J") {
                // long return
                jlong result = env->CallStaticLongMethodA(cls, method, jArgs.data());
                if (env->ExceptionCheck()) {
                    env->ExceptionClear();
                    continue;
                }
                return Value(static_cast<double>(result));
            } else if (returnType == "I") {
                // int return
                jint result = env->CallStaticIntMethodA(cls, method, jArgs.data());
                if (env->ExceptionCheck()) {
                    env->ExceptionClear();
                    continue;
                }
                return Value(static_cast<double>(result));
            } else if (returnType == "V") {
                // void return
                env->CallStaticVoidMethodA(cls, method, jArgs.data());
                if (env->ExceptionCheck()) {
                    env->ExceptionClear();
                    continue;
                }
                return Value(); // nil
            }
        }
    }
    
    // Clean up string arguments
    for (size_t i = 0; i < args.size(); i++) {
        if (args[i].isString() && jArgs[i].l) {
            env->DeleteLocalRef(jArgs[i].l);
        }
    }
    
    throw RuntimeError(Token(TokenType::IDENTIFIER, functionName, "", 0, 0),
                      "Java method '" + functionName + "' not found with compatible signature");
#else
    throw std::runtime_error("Java support not compiled in");
#endif
}

bool JavaLibraryInterface::hasFunction(const std::string& functionName) const {
#ifdef JNI_SUPPORT
    if (!javaClass || !env) return false;
    
    jclass cls = static_cast<jclass>(javaClass);
    
    // Try to find method with common signatures
    std::vector<std::string> signatures = {
        "()D", "()Ljava/lang/String;", "()Z", "()I", "()J", "()V",
        "(D)D", "(Ljava/lang/String;)Ljava/lang/String;", "(I)Z",
        "(DD)D", "(II)I", "(Ljava/lang/String;Ljava/lang/String;)Ljava/lang/String;"
    };
    
    for (const std::string& sig : signatures) {
        jmethodID method = env->GetStaticMethodID(cls, functionName.c_str(), sig.c_str());
        if (method) {
            return true;
        }
        env->ExceptionClear(); // Clear any exceptions from failed lookups
    }
    
    return false;
#else
    return false;
#endif
}

void JavaLibraryInterface::initializeJVM() {
#ifdef JNI_SUPPORT
    if (!jvmInitialized) {
        JavaVMInitArgs vm_args;
        JavaVMOption options[3];
        
        options[0].optionString = const_cast<char*>("-Djava.class.path=.:./examples/java_library");
        options[1].optionString = const_cast<char*>("-Xmx512m");
        options[2].optionString = const_cast<char*>("-Xms256m");
        
        vm_args.version = JNI_VERSION_1_8;
        vm_args.nOptions = 3;
        vm_args.options = options;
        vm_args.ignoreUnrecognized = JNI_FALSE;
        
        jint result = JNI_CreateJavaVM(&jvm, reinterpret_cast<void**>(&env), &vm_args);
        if (result != JNI_OK) {
            throw std::runtime_error("Failed to create Java Virtual Machine");
        }
        
        jvmInitialized = true;
        std::cout << "JVM initialized successfully" << std::endl;
    }
#endif
}

void JavaLibraryInterface::destroyJVM() {
#ifdef JNI_SUPPORT
    if (jvmInitialized && jvm) {
        jvm->DestroyJavaVM();
        jvm = nullptr;
        env = nullptr;
        jvmInitialized = false;
        std::cout << "JVM destroyed" << std::endl;
    }
#endif
}

// CustomPluginInterface Implementation
CustomPluginInterface::CustomPluginInterface(const std::string& pluginPath) {
#ifdef _WIN32
    handle = LoadLibraryA(pluginPath.c_str());
    if (!handle) {
        DWORD error = GetLastError();
        throw std::runtime_error("Failed to load custom plugin: " + pluginPath + " (Error: " + std::to_string(error) + ")");
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
        std::cout << "Custom plugin initialized: " << pluginPath << std::endl;
    }
    
    // Get plugin info
#ifdef _WIN32
    auto infoFunc = reinterpret_cast<PluginInfoFunc>(GetProcAddress(handle, "focus_nexus_plugin_info"));
#else
    auto infoFunc = reinterpret_cast<PluginInfoFunc>(dlsym(handle, "focus_nexus_plugin_info"));
#endif
    
    if (infoFunc) {
        std::cout << "Plugin info: " << infoFunc() << std::endl;
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
    
    // Cache the function for future calls
    functions[functionName] = [funcPtr](const std::vector<Value>& args) -> Value {
        return funcPtr(args);
    };
    
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