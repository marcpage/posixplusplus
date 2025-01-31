#include "../inc/PsxLibrary.h"
#include "test.h"

START_TESTS
    typedef double (*double_filter)(double);
#if defined(__linux__)
    static const char * const library = "m.so.6";
#elif defined(__APPLE__)
    static const char * const library = "m";
#endif

    auto z = psx::Library::open(library);
    auto z2 = std::move(z);

    printf("ceil(0.6) = %0.1f\n", z2.load<double_filter>("ceil")(0.6));
    ASSERT(z2.valid());
    ASSERT(!z.valid());

    TEST_TRY
        z2.load<double_filter>("badfunc");
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
