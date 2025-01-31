#include "../inc/PsxException.h"
#include "test.h"

static void test_if_not_null() {
    int var;
    int *varPtr = &var;
    if (false) {}
    else PsxThrowIfNull(varPtr);
}

static void test_is_null() {
    int *var = nullptr;
    if (false) {}
    else PsxThrowIfNull(var);
}

static void test_message() {
    PsxThrow("Test");
}

static void test_assert_fail() {
    if (false) {}
    else PsxAssert(1 == 0);
}

static void test_assertion() {
    if (false) {}
    else PsxAssert(1 == 1);
}

template <typename T>
static void test_self_assign(T &first, const T &second) {
    first = second;
}

START_TESTS
    TEST_TRY
        test_message();
        FAIL();
    TEST_CATCH
        ASSERT_IN_CATCH(string(exception.what()).find("Test") != string::npos);
        ASSERT_IN_CATCH(string(exception.what()).find("test_message") != string::npos);
    END_TEST_TRY

    TEST_TRY
        test_is_null();
        FAIL();
    TEST_CATCH
        ASSERT_IN_CATCH(string(exception.what()).find("var") != string::npos);
        ASSERT_IN_CATCH(string(exception.what()).find("test_is_null") != string::npos);
    END_TEST_TRY

    TEST_TRY
        test_assert_fail();
        FAIL();
    TEST_CATCH
        ASSERT_IN_CATCH(string(exception.what()).find("1 == 0") != string::npos);
        ASSERT_IN_CATCH(string(exception.what()).find("test_assert_fail") != string::npos);
    END_TEST_TRY

    TEST_TRY
        test_if_not_null();
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        test_assertion();
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        throw psx::Exception("test");
    TEST_CATCH
        ASSERT_IN_CATCH(string(exception.what()).find("test") != std::string::npos);
    END_TEST_TRY

    TEST_TRY
        throw psx::Exception("test", "file");
    TEST_CATCH
        ASSERT_IN_CATCH(string(exception.what()).find("test File: file") != std::string::npos);
    END_TEST_TRY

    TEST_TRY
        throw psx::Exception("test", "file",3);
    TEST_CATCH
        ASSERT_IN_CATCH(string(exception.what()).find("test File: file:3") != std::string::npos);
    END_TEST_TRY

    TEST_TRY
        auto error = psx::Exception(string("test"), "file",3, "func");
        psx::Exception other(error);
        psx::Exception third(string("what"));
        third = other;
        test_self_assign(third, third);
        throw third;
    TEST_CATCH
        ASSERT_IN_CATCH(string(exception.what()).find("test File: file:3 (func)") != std::string::npos);
    END_TEST_TRY

END_TESTS
