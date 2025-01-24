#include "../inc/PsxErrno.h"
#include "test.h"

using namespace std;

START_TESTS

    TEST_TRY
        ErrnoThrow(E2BIG);
        FAIL();
    TEST_CATCH_TYPE(psx::err::E2BIG_Errno)
        ASSERT_IN_CATCH(string(exception.what()).find("E2BIG") != string::npos);
        ASSERT_IN_CATCH(string(exception.what()).find(to_string(E2BIG)) != string::npos);
    TEST_CATCH
        FAIL();
    END_TEST_TRY

    const int errors[] = {
        E2BIG,
        EACCES,
        EADDRINUSE,
        EADDRNOTAVAIL,
        EAFNOSUPPORT,
        EAGAIN,
        EALREADY,
        EBADF,
#if defined(EAUTH)
        EAUTH,
#endif
#if defined(EBADARCH)
        EBADARCH,
#endif
#if defined(EBADEXEC)
        EBADEXEC,
#endif
#if defined(EBADMACHO)
        EBADMACHO,
#endif
#if defined(EBADRPC)
        EBADRPC,
#endif
#if defined(EDEVERR)
        EDEVERR,
#endif
#if defined(EFTYPE)
        EFTYPE,
#endif
#if defined(ELAST)
        ELAST,
#endif
#if defined(ENEEDAUTH)
        ENEEDAUTH,
#endif
#if defined(ENOATTR)
        ENOATTR,
#endif
#if defined(ENOPOLICY)
        ENOPOLICY,
#endif
#if defined(EPROCLIM)
        EPROCLIM,
#endif
#if defined(EPROCUNAVAIL)
        EPROCUNAVAIL,
#endif
#if defined(EPROGMISMATCH)
        EPROGMISMATCH,
#endif
#if defined(EPROGUNAVAIL)
        EPROGUNAVAIL,
#endif
#if defined(EPWROFF)
        EPWROFF,
#endif
#if defined(ERPCMISMATCH)
        ERPCMISMATCH,
#endif
#if defined(ESHLIBVERS)
        ESHLIBVERS,
#endif
        EBADMSG,
        EBUSY,
        ECANCELED,
        ECHILD,
        ECONNABORTED,
        ECONNREFUSED,
        ECONNRESET,
        EDEADLK,
        EDESTADDRREQ,
        EDOM,
        EDQUOT,
        EEXIST,
        EFAULT,
        EFBIG,
        EHOSTDOWN,
        EHOSTUNREACH,
        EIDRM,
        EILSEQ,
        EINPROGRESS,
        EINTR,
        EINVAL,
        EIO,
        EISCONN,
        EISDIR,
        ELOOP,
        EMFILE,
        EMLINK,
        EMSGSIZE,
        EMULTIHOP,
        ENAMETOOLONG,
        ENETDOWN,
        ENETRESET,
        ENETUNREACH,
        ENFILE,
        ENOBUFS,
        ENODATA,
        ENODEV,
        ENOENT,
        ENOEXEC,
        ENOLCK,
        ENOLINK,
        ENOMEM,
        ENOMSG,
        ENOPROTOOPT,
        ENOSPC,
        ENOSR,
        ENOSTR,
        ENOSYS,
        ENOTBLK,
        ENOTCONN,
        ENOTDIR,
        ENOTEMPTY,
        ENOTSOCK,
        ENOTSUP,
        ENOTTY,
        ENXIO,
        EOPNOTSUPP,
        EOVERFLOW,
        EPERM,
        EPFNOSUPPORT,
        EPIPE,
        EPROTO,
        EPROTONOSUPPORT,
        EPROTOTYPE,
        ERANGE,
        EREMOTE,
        EROFS,
        ESHUTDOWN,
        ESOCKTNOSUPPORT,
        ESPIPE,
        ESRCH,
        ESTALE,
        ETIME,
        ETIMEDOUT,
        ETOOMANYREFS,
        ETXTBSY,
        EUSERS,
        EWOULDBLOCK,
        EXDEV,
        ENOTRECOVERABLE,
        EOWNERDEAD
    };


    for (unsigned int err = 0; err < sizeof(errors)/sizeof(errors[0]); ++err) {
        TEST_TRY
            ErrnoCodeThrow(errors[err], string("code: ") + to_string(err));
            FAIL_MESSAGE(string("code: ") + to_string(err));
        TEST_CATCH_TYPE(psx::err::Errno)
            ASSERT_IN_CATCH(string(exception.what()).find(string("code: ") + to_string(err)) != string::npos);
            ASSERT_IN_CATCH(string(exception.what()).find(exception.name()) != string::npos);
            ASSERT_IN_CATCH(exception.code() == errors[err]);
        END_TEST_TRY
    }

    TEST_TRY
        ErrnoCodeThrow(-1, string("invalid"));
        FAIL();
    TEST_CATCH_TYPE(psx::err::Errno)
        ASSERT_IN_CATCH(string(exception.what()).find("invalid") != string::npos);
        ASSERT_IN_CATCH(string(exception.what()).find(exception.name()) != string::npos);
        ASSERT_IN_CATCH(exception.code() == -1);
    END_TEST_TRY

    // TODO: throw on negative, null, construct without message
    // TODO: ErrnoMessageThrow, ErrnoAssert

END_TESTS

