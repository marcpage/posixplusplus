#pragma once

#include <cxxabi.h> // abi::__cxa_demangle
#include <stdlib.h>
#include <string>
#include <vector>

#if defined(__APPLE__)
#include <execinfo.h> // not available on Linux?
#endif

namespace psx {

class Trace {
public:
    typedef std::string String;
    typedef std::vector<String> StringList;

    static StringList stack(size_t maxDepth=4096);
    static StringList &stack(StringList &list, size_t maxDepth=4096);

    static String demangle(const String &name);
    static std::string demangleLine(const std::string &line);
    static std::string stripPrefix(const std::string &line);
};

inline Trace::StringList Trace::stack(size_t maxDepth) {
    StringList list;

    return stack(list, maxDepth);
}

inline Trace::StringList &Trace::stack(StringList &list, size_t maxDepth) {
    list.clear();

#if defined(__APPLE__)
    constexpr int skipStackFrames = 4;
    void **frames = new void *[maxDepth];
    int count = ::backtrace(frames, maxDepth);
    char **names = ::backtrace_symbols(frames, count);

    for (int i = skipStackFrames; i < count; ++i) {
        list.push_back(demangleLine(stripPrefix(names[i])));
    }
    
    ::free(names);
    delete[] frames;
#else
    void *__unused__[] = {&__unused__, &maxDepth};
#endif
    return list;
}

inline Trace::String Trace::demangle(const String &name) {
    size_t dataSize = 0;
    int status = 0;
    std::string result;

    const char *unmangled = abi::__cxa_demangle(name.c_str(), nullptr, &dataSize, &status);

    if (0 == status) {
        result = unmangled;
    } else {
        result = name;
    }

    ::free(reinterpret_cast<void *>(const_cast<char *>(unmangled)));
    return result;
}

inline Trace::String Trace::demangleLine(const String &line) {
    std::string::size_type start, end = line.rfind('+');

    if ((std::string::npos == end) || (end < 3) || (' ' != line[end - 1])) {
        return line;
    }

    end -= 1;
    start = line.rfind(' ', end - 1);

    if ((std::string::npos == end) || (start < 3) || (end - start < 1)) {
        return line;
    }

    ++start;
    return line.substr(0, start) + demangle(line.substr(start, end - start)) + line.substr(end);
}

inline Trace::String Trace::stripPrefix(const String &line) {
    auto zeroEx = line.find("0x");

    if (std::string::npos != zeroEx) {
        return line.substr(zeroEx + 2);
    }

    return line;
}

}