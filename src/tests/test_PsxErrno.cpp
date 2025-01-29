#include "../inc/PsxErrno.h"
#include "test.h"

START_TESTS

    TEST_TRY
        ErrnoThrow(E2BIG);
        FAIL();
    TEST_CATCH_TYPE(psx::E2BIG_Errno)
        ASSERT_IN_CATCH(string(exception.what()).find("E2BIG") != string::npos);
        ASSERT_IN_CATCH(string(exception.what()).find(to_string(E2BIG)) != string::npos);
    TEST_CATCH
        FAIL_IN_CATCH();
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
        TEST_CATCH_TYPE(psx::Errno)
            ASSERT_IN_CATCH(string(exception.what()).find(string("code: ") + to_string(err)) != string::npos);
            ASSERT_IN_CATCH(string(exception.what()).find(exception.name()) != string::npos);
            ASSERT_IN_CATCH(exception.code() == errors[err]);
        END_TEST_TRY
    }

    TEST_TRY
        ErrnoCodeThrow(-1, string("invalid"));
        FAIL();
    TEST_CATCH_TYPE(psx::Errno)
        ASSERT_IN_CATCH(string(exception.what()).find("invalid") != string::npos);
        ASSERT_IN_CATCH(string(exception.what()).find(exception.name()) != string::npos);
        ASSERT_IN_CATCH(exception.code() == -1);
    END_TEST_TRY

    TEST_TRY
        errno = ENOENT;
        ErrnoMessageThrow("Bad mojo");
        FAIL();
    TEST_CATCH_TYPE(psx::ENOENT_Errno)
        ASSERT_IN_CATCH(string(exception.what()).find("Bad mojo") != string::npos);
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        errno = 0;
        ErrnoMessageThrow("No error");
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        errno = ENOENT;
        ErrnoOnNegative(-1);
        FAIL();
    TEST_CATCH_TYPE(psx::ENOENT_Errno)
        ASSERT_IN_CATCH(string(exception.what()).find("-1") != string::npos);
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        errno = 0;
        ErrnoOnNegative(0);
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        errno = 0;
        ErrnoOnNegative(-1);
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        errno = ENOENT;
        int *value = nullptr;
        ErrnoOnNull(value);
        FAIL();
    TEST_CATCH_TYPE(psx::ENOENT_Errno)
        ASSERT_IN_CATCH(string(exception.what()).find("value") != string::npos);
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        errno = 0;
        auto value = 5;
        ErrnoOnNull(&value);
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        errno = 0;
        int *value = nullptr;
        ErrnoOnNull(value);
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        errno = ENOENT;
        if (false) {}
        else ErrnoAssert(0 == 1);
        FAIL();
    TEST_CATCH_TYPE(psx::ENOENT_Errno)
        ASSERT_IN_CATCH(string(exception.what()).find("0 == 1") != string::npos);
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        errno = 0;
        ErrnoAssert(true);
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        errno = 0;
        ErrnoAssert(false);
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

END_TESTS

