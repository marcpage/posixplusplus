#pragma once

#include "PsxException.h"

#include <errno.h>
#include <string.h>

#define ErrnoThrow(name)                                                       \
  throw psx::name##_Errno(#name, __FILE__, __LINE__, __func__)

#define ErrnoCodeThrow(errnoCode, message)                                     \
  psx::Errno::_throw(errnoCode, message, __FILE__, __LINE__, __func__)

#define ErrnoMessageThrow(message)                                             \
  psx::Errno::_throw(errno, message, __FILE__, __LINE__, __func__)

#define ErrnoOnNegative(call)                                                  \
  psx::Errno::_throwOnNegative(call, #call, __FILE__, __LINE__, __func__)

#define ErrnoOnNull(call)                                                      \
  psx::Errno::_throwOnNull(call, #call, __FILE__, __LINE__, __func__)

#define ErrnoOnNullMsg(call, message)                                          \
  psx::Errno::_throwOnNull(call, std::string(#call) + ": " + message, __FILE__, __LINE__, __func__)

#define ErrnoAssert(condition)                                                 \
  if (!(condition)) {                                                          \
    ErrnoCodeThrow(errno, #condition);                                         \
  } else psx::noop()


namespace psx {

class Errno : public psx::Exception {
public:
    Errno(int value, const char *errnoName, const char *file=nullptr, int line=0, const char *function=nullptr) throw();
    Errno(const std::string &message, int value, const char *errnoName, const char *file=nullptr, int line=0, const char *function=nullptr) throw();
    virtual const char *name() const;
    int code() const throw();

    static void _throw(int errnoCode, const std::string &message, const char *file, int line, const char *function);
    static int _throwOnNegative(const int returnCode, const char *call, const char *file, const int line, const char *function);
    
    template <typename T>
    static T* _throwOnNull(T *returnedPtr, const std::string &call, const char *file, const int line, const char *function);

private:
    int _errno;

    template <class S>
    std::string _init(S message, const char* errnoName, int value);
};

#define ErrnoException(errno_name) \
    class errno_name##_Errno : public Errno { \
    public: \
        const char *name() const override {return #errno_name;} \
        explicit errno_name##_Errno(const char *file=nullptr, int line=0, const char *func=nullptr) throw() \
            : Errno(errno_name, #errno_name, file, line, func) {} \
        explicit errno_name##_Errno(const std::string &message, const char *file=nullptr, int line=0, const char *func=nullptr) throw() \
            : Errno(message, errno_name, #errno_name, file, line, func) {} \
    };

ErrnoException(E2BIG);
ErrnoException(EACCES);
ErrnoException(EADDRINUSE);
ErrnoException(EADDRNOTAVAIL);
ErrnoException(EAFNOSUPPORT);
ErrnoException(EAGAIN);
ErrnoException(EALREADY);
ErrnoException(EBADF);
#if defined(EAUTH)
ErrnoException(EAUTH);
#endif
#if defined(EBADARCH)
ErrnoException(EBADARCH);
#endif
#if defined(EBADEXEC)
ErrnoException(EBADEXEC);
#endif
#if defined(EBADMACHO)
ErrnoException(EBADMACHO);
#endif
#if defined(EBADRPC)
ErrnoException(EBADRPC);
#endif
#if defined(EDEVERR)
ErrnoException(EDEVERR);
#endif
#if defined(EFTYPE)
ErrnoException(EFTYPE);
#endif
#if defined(ELAST)
ErrnoException(ELAST);
#endif
#if defined(ENEEDAUTH)
ErrnoException(ENEEDAUTH);
#endif
#if defined(ENOATTR)
ErrnoException(ENOATTR);
#endif
#if defined(ENOPOLICY)
ErrnoException(ENOPOLICY);
#endif
#if defined(EPROCLIM)
ErrnoException(EPROCLIM);
#endif
#if defined(EPROCUNAVAIL)
ErrnoException(EPROCUNAVAIL);
#endif
#if defined(EPROGMISMATCH)
ErrnoException(EPROGMISMATCH);
#endif
#if defined(EPROGUNAVAIL)
ErrnoException(EPROGUNAVAIL);
#endif
#if defined(EPWROFF)
ErrnoException(EPWROFF);
#endif
#if defined(ERPCMISMATCH)
ErrnoException(ERPCMISMATCH);
#endif
#if defined(ESHLIBVERS)
ErrnoException(ESHLIBVERS);
#endif
ErrnoException(EBADMSG);
ErrnoException(EBUSY);
ErrnoException(ECANCELED);
ErrnoException(ECHILD);
ErrnoException(ECONNABORTED);
ErrnoException(ECONNREFUSED);
ErrnoException(ECONNRESET);
ErrnoException(EDEADLK);
ErrnoException(EDESTADDRREQ);
ErrnoException(EDOM);
ErrnoException(EDQUOT);
ErrnoException(EEXIST);
ErrnoException(EFAULT);
ErrnoException(EFBIG);
ErrnoException(EHOSTDOWN);
ErrnoException(EHOSTUNREACH);
ErrnoException(EIDRM);
ErrnoException(EILSEQ);
ErrnoException(EINPROGRESS);
ErrnoException(EINTR);
ErrnoException(EINVAL);
ErrnoException(EIO);
ErrnoException(EISCONN);
ErrnoException(EISDIR);
ErrnoException(ELOOP);
ErrnoException(EMFILE);
ErrnoException(EMLINK);
ErrnoException(EMSGSIZE);
ErrnoException(EMULTIHOP);
ErrnoException(ENAMETOOLONG);
ErrnoException(ENETDOWN);
ErrnoException(ENETRESET);
ErrnoException(ENETUNREACH);
ErrnoException(ENFILE);
ErrnoException(ENOBUFS);
ErrnoException(ENODATA);
ErrnoException(ENODEV);
ErrnoException(ENOENT);
ErrnoException(ENOEXEC);
ErrnoException(ENOLCK);
ErrnoException(ENOLINK);
ErrnoException(ENOMEM);
ErrnoException(ENOMSG);
ErrnoException(ENOPROTOOPT);
ErrnoException(ENOSPC);
ErrnoException(ENOSR);
ErrnoException(ENOSTR);
ErrnoException(ENOSYS);
ErrnoException(ENOTBLK);
ErrnoException(ENOTCONN);
ErrnoException(ENOTDIR);
ErrnoException(ENOTEMPTY);
ErrnoException(ENOTSOCK);
ErrnoException(ENOTSUP);
ErrnoException(ENOTTY);
ErrnoException(ENXIO);
ErrnoException(EOPNOTSUPP);
ErrnoException(EOVERFLOW);
ErrnoException(EPERM);
ErrnoException(EPFNOSUPPORT);
ErrnoException(EPIPE);
ErrnoException(EPROTO);
ErrnoException(EPROTONOSUPPORT);
ErrnoException(EPROTOTYPE);
ErrnoException(ERANGE);
ErrnoException(EREMOTE);
ErrnoException(EROFS);
ErrnoException(ESHUTDOWN);
ErrnoException(ESOCKTNOSUPPORT);
ErrnoException(ESPIPE);
ErrnoException(ESRCH);
ErrnoException(ESTALE);
ErrnoException(ETIME);
ErrnoException(ETIMEDOUT);
ErrnoException(ETOOMANYREFS);
ErrnoException(ETXTBSY);
ErrnoException(EUSERS);
ErrnoException(EWOULDBLOCK);
ErrnoException(EXDEV);
ErrnoException(ENOTRECOVERABLE);
ErrnoException(EOWNERDEAD);

#undef ErrnoException

inline Errno::Errno(int value, const char *errnoName, const char *file, int line, const char *function) throw()
    : psx::Exception(_init("", errnoName, value), file, line, function), _errno(value) {}

inline Errno::Errno(const std::string &message, int value, const char *errnoName, const char *file, int line, const char *function) throw()
    : psx::Exception(_init(message, errnoName, value), file, line, function), _errno(value) {}

inline const char *Errno::name() const {
    return "[Unknown]";
}

inline int Errno::code() const throw() {
    return _errno;
}

inline void Errno::_throw(int errnoCode, const std::string &message, const char *file, int line, const char *function) {
    if (0 != errnoCode) {
        switch (errnoCode) {
#define ErrnoCaseClass(name) case name: throw name##_Errno(message, file, line, function)
            ErrnoCaseClass(E2BIG);
            ErrnoCaseClass(EACCES);
            ErrnoCaseClass(EADDRINUSE);
            ErrnoCaseClass(EADDRNOTAVAIL);
            ErrnoCaseClass(EAFNOSUPPORT);
            ErrnoCaseClass(EAGAIN);
            ErrnoCaseClass(EALREADY);
#if defined(EAUTH)
            ErrnoCaseClass(EAUTH);
#endif
#if defined(EBADARCH)
            ErrnoCaseClass(EBADARCH);
#endif
#if defined(EBADEXEC)
            ErrnoCaseClass(EBADEXEC);
#endif
#if defined(EBADMACHO)
            ErrnoCaseClass(EBADMACHO);
#endif
#if defined(EBADRPC)
            ErrnoCaseClass(EBADRPC);
#endif
#if defined(EDEVERR)
            ErrnoCaseClass(EDEVERR);
#endif
#if defined(EFTYPE)
            ErrnoCaseClass(EFTYPE);
#endif
#if defined(ELAST)
            ErrnoCaseClass(ELAST);
#endif
#if defined(ENEEDAUTH)
            ErrnoCaseClass(ENEEDAUTH);
#endif
#if defined(ENOATTR)
            ErrnoCaseClass(ENOATTR);
#endif
#if defined(EPROCLIM)
            ErrnoCaseClass(EPROCLIM);
#endif
#if defined(EPROCUNAVAIL)
            ErrnoCaseClass(EPROCUNAVAIL);
#endif
#if defined(EPROGMISMATCH)
            ErrnoCaseClass(EPROGMISMATCH);
#endif
#if defined(EPROGUNAVAIL)
            ErrnoCaseClass(EPROGUNAVAIL);
#endif
#if defined(EPWROFF)
            ErrnoCaseClass(EPWROFF);
#endif
#if defined(ERPCMISMATCH)
            ErrnoCaseClass(ERPCMISMATCH);
#endif
#if defined(ESHLIBVERS)
            ErrnoCaseClass(ESHLIBVERS);
#endif
#if defined(ENOPOLICY)
            ErrnoCaseClass(ENOPOLICY);
#endif
#if EOPNOTSUPP != ENOTSUP
            ErrnoCaseClass(EOPNOTSUPP);
#endif
#if EWOULDBLOCK != EAGAIN
            ErrnoCaseClass(EWOULDBLOCK);
#endif
            ErrnoCaseClass(EBADF);
            ErrnoCaseClass(EBADMSG);
            ErrnoCaseClass(EBUSY);
            ErrnoCaseClass(ECANCELED);
            ErrnoCaseClass(ECHILD);
            ErrnoCaseClass(ECONNABORTED);
            ErrnoCaseClass(ECONNREFUSED);
            ErrnoCaseClass(ECONNRESET);
            ErrnoCaseClass(EDEADLK);
            ErrnoCaseClass(EDESTADDRREQ);
            ErrnoCaseClass(EDOM);
            ErrnoCaseClass(EDQUOT);
            ErrnoCaseClass(EEXIST);
            ErrnoCaseClass(EFAULT);
            ErrnoCaseClass(EFBIG);
            ErrnoCaseClass(EHOSTDOWN);
            ErrnoCaseClass(EHOSTUNREACH);
            ErrnoCaseClass(EIDRM);
            ErrnoCaseClass(EILSEQ);
            ErrnoCaseClass(EINPROGRESS);
            ErrnoCaseClass(EINTR);
            ErrnoCaseClass(EINVAL);
            ErrnoCaseClass(EIO);
            ErrnoCaseClass(EISCONN);
            ErrnoCaseClass(EISDIR);
            ErrnoCaseClass(ELOOP);
            ErrnoCaseClass(EMFILE);
            ErrnoCaseClass(EMLINK);
            ErrnoCaseClass(EMSGSIZE);
            ErrnoCaseClass(EMULTIHOP);
            ErrnoCaseClass(ENAMETOOLONG);
            ErrnoCaseClass(ENETDOWN);
            ErrnoCaseClass(ENETRESET);
            ErrnoCaseClass(ENETUNREACH);
            ErrnoCaseClass(ENFILE);
            ErrnoCaseClass(ENOBUFS);
            ErrnoCaseClass(ENODATA);
            ErrnoCaseClass(ENODEV);
            ErrnoCaseClass(ENOENT);
            ErrnoCaseClass(ENOEXEC);
            ErrnoCaseClass(ENOLCK);
            ErrnoCaseClass(ENOLINK);
            ErrnoCaseClass(ENOMEM);
            ErrnoCaseClass(ENOMSG);
            ErrnoCaseClass(ENOPROTOOPT);
            ErrnoCaseClass(ENOSPC);
            ErrnoCaseClass(ENOSR);
            ErrnoCaseClass(ENOSTR);
            ErrnoCaseClass(ENOSYS);
            ErrnoCaseClass(ENOTBLK);
            ErrnoCaseClass(ENOTCONN);
            ErrnoCaseClass(ENOTDIR);
            ErrnoCaseClass(ENOTEMPTY);
            ErrnoCaseClass(ENOTSOCK);
            ErrnoCaseClass(ENOTSUP);
            ErrnoCaseClass(ENOTTY);
            ErrnoCaseClass(ENXIO);
            ErrnoCaseClass(EOVERFLOW);
            ErrnoCaseClass(EOWNERDEAD);
            ErrnoCaseClass(EPERM);
            ErrnoCaseClass(EPFNOSUPPORT);
            ErrnoCaseClass(EPIPE);
            ErrnoCaseClass(EPROTO);
            ErrnoCaseClass(EPROTONOSUPPORT);
            ErrnoCaseClass(EPROTOTYPE);
            ErrnoCaseClass(ERANGE);
            ErrnoCaseClass(EREMOTE);
            ErrnoCaseClass(EROFS);
            ErrnoCaseClass(ESHUTDOWN);
            ErrnoCaseClass(ESOCKTNOSUPPORT);
            ErrnoCaseClass(ESPIPE);
            ErrnoCaseClass(ESRCH);
            ErrnoCaseClass(ESTALE);
            ErrnoCaseClass(ETIME);
            ErrnoCaseClass(ETIMEDOUT);
            ErrnoCaseClass(ETOOMANYREFS);
            ErrnoCaseClass(ETXTBSY);
            ErrnoCaseClass(EUSERS);
            ErrnoCaseClass(EXDEV);
            ErrnoCaseClass(ENOTRECOVERABLE);
#undef ErrnoCaseClass
            default: 
                throw Errno(message, errnoCode, "[Unknown]", file, line);
        }
    }
}

inline int Errno::_throwOnNegative(const int returnCode, const char *call, const char *file, const int line, const char *function) {
    if (returnCode < 0) {
        _throw(errno, call, file, line, function);
    }
    return returnCode;
}

template <typename T>
inline T* Errno::_throwOnNull(T *returnedPtr, const std::string &call, const char *file, const int line, const char *function) {
    if (nullptr == returnedPtr) {
        _throw(errno, call, file, line, function);
    }
    return returnedPtr;
}

template <class S>
inline std::string Errno::_init(S message, const char* errnoName, int value) {
    return std::string("[") + errnoName 
            + " (" + std::to_string(value) + "): " 
            + ::strerror(value) + "]: " 
            + std::string(message);
} // NOTEST

} // namespace psx
