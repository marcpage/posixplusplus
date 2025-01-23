#include "../include/PsxErrno.h"
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

END_TESTS

