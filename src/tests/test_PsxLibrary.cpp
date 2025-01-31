#include "../inc/PsxLibrary.h"
#include "test.h"

START_TESTS

    typedef const char *(*version)();
    typedef pid_t (*get_pid)();

    auto z = psx::Library::open("c");
    auto z2 = std::move(z);

    printf("pid = %d\n", int(z2.load<get_pid>("getpid")()));
    ASSERT(z2.load<get_pid>("getpid")() != 0);
    ASSERT(z2.valid());
    ASSERT(!z.valid());

    TEST_TRY
        z2.load<version>("badfunc");
        FAIL();
    TEST_CATCH_TYPE(psx::Exception)
        ASSERT_IN_CATCH(string(exception.what()).find("badfunc not found:") != string::npos);
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        psx::Library::open("bogus library");
        FAIL();
    TEST_CATCH_TYPE(psx::Exception)
        ASSERT_IN_CATCH(string(exception.what()).find("Failed to load bogus library:") != string::npos);
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

END_TESTS
