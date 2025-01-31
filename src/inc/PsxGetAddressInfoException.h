#pragma once

#include "PsxException.h"
#include <netdb.h>

namespace psx {

#define GAIMessageThrow(condition) psx::GetAddressInfoException::_throw(condition, #condition, __FILE__, __LINE__, __func__)

class GetAddressInfoException : public Exception {
public:
    GetAddressInfoException(int value, const char *gaiName, const char *file=nullptr, int line=0, const char *function=nullptr) throw();
    GetAddressInfoException(const std::string &message, int value, const char *gaiName, const char *file=nullptr, int line=0, const char *function=nullptr) throw();
    virtual const char *name() const;
    int code() const throw();

    static void _throw(int gaiCode, const std::string &message, const char *file, int line, const char *function);

private:
    int _value;

    template <class S>
    std::string _init(S message, const char* gaiName, int value);
};

#define GAIException(GAI_name) \
    class GAI_name##_Exception : public GetAddressInfoException { \
    public: \
        const char *name() const override {return #GAI_name;} \
        explicit GAI_name##_Exception(const char *file=nullptr, int line=0, const char *func=nullptr) throw() \
            : GetAddressInfoException(GAI_name, #GAI_name, file, line, func) {} \
        explicit GAI_name##_Exception(const std::string &message, const char *file=nullptr, int line=0, const char *func=nullptr) throw() \
            : GetAddressInfoException(message, GAI_name, #GAI_name, file, line, func) {} \
    };

GAIException(EAI_AGAIN);
GAIException(EAI_BADFLAGS);
#if defined(EAI_BADHINTS)
GAIException(EAI_BADHINTS);
#endif
#if defined(EAI_PROTOCOL)
GAIException(EAI_PROTOCOL);
#endif
GAIException(EAI_FAIL);
GAIException(EAI_FAMILY);
GAIException(EAI_MEMORY);
GAIException(EAI_NONAME);
GAIException(EAI_OVERFLOW);
GAIException(EAI_SERVICE);
GAIException(EAI_SOCKTYPE);
GAIException(EAI_SYSTEM);

#undef GAIException

inline GetAddressInfoException::GetAddressInfoException(int value, const char *gaiName, const char *file, int line, const char *function) throw()
    : psx::Exception(_init("", gaiName, value), file, line, function), _value(value) {}

inline GetAddressInfoException::GetAddressInfoException(const std::string &message, int value, const char *gaiName, const char *file, int line, const char *function) throw()
    : psx::Exception(_init(message, gaiName, value), file, line, function), _value(value) {}

inline const char *GetAddressInfoException::name() const {
    return "[Unknown]";
}

inline int GetAddressInfoException::code() const throw() {
    return _value;
}

inline void GetAddressInfoException::_throw(int gaiCode, const std::string &message, const char *file, int line, const char *function) {
    if (0 != gaiCode) {
        switch (gaiCode) {
#define GAICaseClass(name) case name: throw name##_Exception(message, file, line, function)
      GAICaseClass(EAI_AGAIN);
      GAICaseClass(EAI_BADFLAGS);
#if defined(EAI_BADHINTS)
      GAICaseClass(EAI_BADHINTS);
#endif
#if defined(EAI_PROTOCOL)
      GAICaseClass(EAI_PROTOCOL);
#endif
      GAICaseClass(EAI_FAIL);
      GAICaseClass(EAI_FAMILY);
      GAICaseClass(EAI_MEMORY);
      GAICaseClass(EAI_NONAME);
      GAICaseClass(EAI_OVERFLOW);
      GAICaseClass(EAI_SERVICE);
      GAICaseClass(EAI_SOCKTYPE);
      GAICaseClass(EAI_SYSTEM);
#undef GAICaseClass
            default: 
                throw GetAddressInfoException(message, gaiCode, "[Unknown]", file, line);
        }
    }
}

template <class S>
inline std::string GetAddressInfoException::_init(S message, const char* gaiName, int value) {
    return std::string("[") + gaiName 
            + " (" + std::to_string(value) + "): " 
            + ::strerror(value) + "]: " 
            + std::string(message);
}

} // namespace psx
