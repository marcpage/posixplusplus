#include "../inc/PsxException.h"
#include "test.h"

using namespace std;

void test_if_not_null() {
    int var;
    int *varPtr = &var;
    PsxThrowIfNull(varPtr);
}

void test_is_null() {
    int *var = nullptr;
    PsxThrowIfNull(var);
}

void test_message() {
    PsxThrow("Test");
}

void test_assert_fail() {
    PsxAssert(1 == 0);
}

void test_assertion() {
    PsxAssert(1 == 1);
}

template <typename T>
void test_self_assign(T &first, const T &second) {
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
        ASSERT_IN_CATCH(string(exception.what()) == "test");
    END_TEST_TRY

    TEST_TRY
        throw psx::Exception("test", "file");
    TEST_CATCH
        ASSERT_IN_CATCH(string(exception.what()) == "test File: file");
    END_TEST_TRY

    TEST_TRY
        throw psx::Exception("test", "file",3);
    TEST_CATCH
        ASSERT_IN_CATCH(string(exception.what()) == "test File: file:3");
    END_TEST_TRY

    TEST_TRY
        auto error = psx::Exception(string("test"), "file",3, "func");
        psx::Exception other(error);
        psx::Exception third(string("what"));
        third = other;
        test_self_assign(third, third);
        throw third;
    TEST_CATCH
        ASSERT_IN_CATCH(string(exception.what()) == "test File: file:3 (func)");
    END_TEST_TRY

    // TODO: test PsxThrowIfNull and PsxAssert in if else clauses

END_TESTS
