#pragma once

#include "PsxTrace.h"
#include <exception>
#include <string>
#include <memory>

#define PsxThrow(message)                                                    \
    throw psx::Exception((message), __FILE__, __LINE__, __func__)

#define PsxThrowIfNull(variable)                                             \
    if (nullptr == (variable)) {                                             \
        PsxThrow(std::string(#variable).append(" == nullptr"));              \
    } else psx::noop()

#define ExceptionOnNull(call)                                                \
  psx::Errno::_throwOnNull(call, #call, __FILE__, __LINE__, __func__)

#define PsxAssert(condition)                                                 \
    if (!(condition)) {                                                      \
        PsxThrow(#condition);                                                \
    } else psx::noop()

namespace psx {

class Exception : public std::exception {
public:
    explicit Exception(const char *message, const char *file=nullptr, int line=0, const char *function=nullptr) throw();
    explicit Exception(const std::string &message, const char *file=nullptr, int line=0, const char *function=nullptr) throw();
    Exception(const Exception &other);
    Exception &operator=(const Exception &other);
    virtual const char *what() const throw();

    template <typename T>
    static T* _throwOnNull(T *returnedPtr, const std::string &call, const char *file, const int line, const char *function);

private:
    using Str = std::string;
    using StrPtr = std::unique_ptr<Str>;
    StrPtr _message;

    template <class S>
    StrPtr _init(S message, const char *file, int line, const char *function);
};

inline Exception::Exception(const char *message, const char *file, int line, const char *function) throw()
    :exception(),_message(_init(message, file, line, function)) {}

inline Exception::Exception(const std::string &message, const char *file, int line, const char *function) throw()
    :exception(), _message(_init(message, file, line, function)) {}

inline Exception::Exception(const Exception &other)
    :exception(), _message(nullptr) {
    if (nullptr == other._message) {
        return; // NOTEST
    }
    
    try {
        _message = StrPtr(std::make_unique<Str>(*other._message));
    } catch (const std::exception &) {
        _message = nullptr; // NOTEST
    } // NOTEST
}

inline Exception &Exception::operator=(const Exception &other) {
    if (this == &other) {
        return *this;
    }

    if (nullptr == other._message) {
        _message = nullptr; // NOTEST
        return *this; // NOTEST
    }

    _message = std::make_unique<Str>(*other._message);
    return *this;
}

inline const char *Exception::what() const throw() {
    if (nullptr != _message) {
        return _message->c_str();
    }
    return "Unable to allocate message string"; // NOTEST
}

template <typename T>
inline T* Exception::_throwOnNull(T *returnedPtr, const std::string &call, const char *file, const int line, const char *function) {
    if (nullptr == returnedPtr) {
        throw psx::Exception(call + " == nullptr", file, line, function);
    }
    return returnedPtr;
}

template <class S>
inline Exception::StrPtr Exception::_init(S message, const char *file, int line, const char *function) {
    try {
        StrPtr messagePtr = std::make_unique<Str>(message);
        auto trace = psx::Trace::stack();
        std::string stack;

        for (auto i : trace) {
            stack.append("\n" + i);
        }

        if (nullptr == file) {
            messagePtr->append(stack);
            return messagePtr;
        }

        messagePtr->append(" File: ").append(file);

        if (0 == line) {
            messagePtr->append(stack);
            return messagePtr;
        }

        messagePtr->append(":").append(std::to_string(line));

        if (nullptr == function) {
            messagePtr->append(stack);
            return messagePtr;
        }

        messagePtr->append(" (").append(function).append(")");
        messagePtr->append(stack);
        return messagePtr;
    } catch(const std::exception &) {}

    return StrPtr(); // NOTEST
}

inline void noop() {}

} // namespace psx
