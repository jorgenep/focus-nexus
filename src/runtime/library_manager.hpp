#pragma once
#include "value.hpp"
#include "../lexer/token.hpp"
#include <string>
#include <unordered_map>
#include <memory>
#include <functional>

#ifdef _WIN32
#include <windows.h>
#else
#include <dlfcn.h>
#endif

// Forward declarations
class Interpreter;

// Base class for library interfaces
class LibraryInterface {
public:
    virtual ~LibraryInterface() = default;
    virtual Value callFunction(const std::string& functionName, const std::vector<Value>& args) = 0;
    virtual bool hasFunction(const std::string& functionName) const = 0;
    virtual std::string getType() const = 0;
};

// C++ Library Interface
class CppLibraryInterface : public LibraryInterface {
private:
#ifdef _WIN32
    HMODULE handle;
#else
    void* handle;
#endif
    std::unordered_map<std::string, void*> functions;

public:
    explicit CppLibraryInterface(const std::string& libraryPath);
    ~CppLibraryInterface() override;
    
    Value callFunction(const std::string& functionName, const std::vector<Value>& args) override;
    bool hasFunction(const std::string& functionName) const override;
    std::string getType() const override { return "cpp"; }
    
    void registerFunction(const std::string& name, void* funcPtr);
};

// Python Library Interface
class PythonLibraryInterface : public LibraryInterface {
private:
    std::string moduleName;
    void* pythonModule; // PyObject* in disguise
    std::unordered_map<std::string, void*> functions;
    static bool pythonInitialized;

public:
    explicit PythonLibraryInterface(const std::string& modulePath);
    ~PythonLibraryInterface() override;
    
    Value callFunction(const std::string& functionName, const std::vector<Value>& args) override;
    bool hasFunction(const std::string& functionName) const override;
    std::string getType() const override { return "python"; }
    
    static void initializePython();
    static void finalizePython();
};

// Java Library Interface (JNI)
class JavaLibraryInterface : public LibraryInterface {
private:
    std::string className;
    void* jvm; // JavaVM* in disguise
    void* env; // JNIEnv* in disguise
    void* javaClass; // jclass in disguise
    std::unordered_map<std::string, void*> methods;
    static bool jvmInitialized;

public:
    explicit JavaLibraryInterface(const std::string& classPath);
    ~JavaLibraryInterface() override;
    
    Value callFunction(const std::string& functionName, const std::vector<Value>& args) override;
    bool hasFunction(const std::string& functionName) const override;
    std::string getType() const override { return "java"; }
    
    static void initializeJVM();
    static void destroyJVM();
};

// Custom Plugin Interface
class CustomPluginInterface : public LibraryInterface {
private:
#ifdef _WIN32
    HMODULE handle;
#else
    void* handle;
#endif
    std::unordered_map<std::string, std::function<Value(const std::vector<Value>&)>> functions;

public:
    explicit CustomPluginInterface(const std::string& pluginPath);
    ~CustomPluginInterface() override;
    
    Value callFunction(const std::string& functionName, const std::vector<Value>& args) override;
    bool hasFunction(const std::string& functionName) const override;
    std::string getType() const override { return "custom"; }
};

// Library Manager
class LibraryManager {
private:
    std::unordered_map<std::string, std::shared_ptr<LibraryInterface>> libraries;
    static std::unique_ptr<LibraryManager> instance;

public:
    static LibraryManager& getInstance();
    
    bool loadLibrary(const std::string& alias, const std::string& path, const std::string& type);
    Value callFunction(const std::string& library, const std::string& function, const std::vector<Value>& args);
    bool hasLibrary(const std::string& alias) const;
    bool hasFunction(const std::string& library, const std::string& function) const;
    
    void unloadLibrary(const std::string& alias);
    void unloadAllLibraries();
    
    std::vector<std::string> getLoadedLibraries() const;
    std::string getLibraryType(const std::string& alias) const;
};

// Plugin API for custom plugins
extern "C" {
    // Function signature for plugin initialization
    typedef void (*PluginInitFunc)(void);
    
    // Function signature for plugin cleanup
    typedef void (*PluginCleanupFunc)(void);
    
    // Function signature for getting plugin info
    typedef const char* (*PluginInfoFunc)(void);
    
    // Function signature for plugin functions
    typedef Value (*PluginFunction)(const std::vector<Value>& args);
    
    // Function signature for registering plugin functions
    typedef void (*RegisterFunctionFunc)(const char* name, PluginFunction func);
}

// Helper macros for plugin development
#define FOCUS_NEXUS_PLUGIN_INIT(func) extern "C" void focus_nexus_plugin_init() { func(); }
#define FOCUS_NEXUS_PLUGIN_CLEANUP(func) extern "C" void focus_nexus_plugin_cleanup() { func(); }
#define FOCUS_NEXUS_PLUGIN_INFO(info) extern "C" const char* focus_nexus_plugin_info() { return info; }
#define FOCUS_NEXUS_EXPORT_FUNCTION(name, func) extern "C" Value focus_nexus_##name(const std::vector<Value>& args) { return func(args); }