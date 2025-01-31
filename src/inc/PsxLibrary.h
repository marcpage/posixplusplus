#pragma once

#include "PsxException.h"
#include <string>
#include <dlfcn.h>

namespace psx {

class Library {
public:
    enum SymbolLoading { LazyLoad, LoadSymbols };
    enum SymbolScoping { GlobalSymbols, LocalSynbols };
    enum UnloadBehavior { UnloadWhenDone, DoNotUnload };
    enum ReallyLoad { LoadLibrary, TestLoadLibrary };
    typedef std::string String;

    static Library open(const String &name, SymbolLoading symbols=LazyLoad, SymbolScoping scope=LocalSynbols, UnloadBehavior unload=UnloadWhenDone, ReallyLoad load=LoadLibrary);

    bool valid() const;
    
    template<typename F>
    F load(const String &name) const;

    Library();
    Library(Library&& other);
    virtual ~Library() throw();

private:
    void *_library;    

    Library(const Library& temp_obj) = delete; 
    Library& operator=(const Library& temp_obj) = delete;
    bool _attempt(const String &name, int flags, String &error);
};

inline Library Library::open(const String &name, SymbolLoading symbols, SymbolScoping scope, UnloadBehavior unload, ReallyLoad load) {
    static const char * const libPrefix = "lib";
#if defined(__linux__)
    static const char * const libSuffix = ".so";
#elif defined(__APPLE__)
    static const char * const libSuffix = ".dylib";
#endif
    Library library;
    String originalError;
    String latestError;
    int flags = (symbols == LazyLoad) ? RTLD_LAZY : RTLD_NOW;

    flags += (scope == GlobalSymbols) ? RTLD_GLOBAL : RTLD_LOCAL;
    flags += (unload == DoNotUnload) ? RTLD_NODELETE : 0;
    flags += (load == TestLoadLibrary) ? RTLD_NOLOAD : 0;
    
    if (library._attempt(name, flags, originalError)) {
        return library; // NOTEST
    }
    
    if (library._attempt(libPrefix + name, flags, latestError)) {
        return library; // NOTEST
    }
    
    if (library._attempt(name + libSuffix, flags, latestError)) {
        return library; // NOTEST
    }

    if (library._attempt(libPrefix + name + libSuffix, flags, latestError)) {
        return library;
    }

    PsxThrow("Failed to load " + name + ": " + originalError);
}

inline bool Library::valid() const {
    return _library != nullptr;
}

template<typename F>
inline F Library::load(const String &name) const {
    PsxAssert(nullptr == ::dlerror());
    F func = reinterpret_cast<F>(::dlsym(_library, name.c_str()));

    if (!func) {
        const char *error = ::dlerror();

        if (error) {
            PsxThrow(name + " not found: " + String(error));
        }
    }

    return func;
}

inline Library::Library()
    :_library(nullptr) {}

inline Library::Library(Library&& other)
    :_library(other._library) {
    if (this != &other) {
        other._library = nullptr;
    }
}

inline Library::~Library() throw() {
    if (_library) {
        ::dlclose(_library);
    }

    _library = nullptr;
}

inline bool Library::_attempt(const String &name, int flags, String &error) {
    _library = ::dlopen(name.c_str(), flags);
    
    if (!_library) {
        error = ::dlerror();
        return false;
    }
    return true;
}

} // namespace psx
