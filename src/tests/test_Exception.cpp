#include "../include/Exception.h"
#include "test.h"

using namespace std;

void test_it_not_null() {
    int var;
    int *varPtr = &var;
    MsgThrowIfNull(varPtr);
}

void test_is_null() {
    int *var = nullptr;
    MsgThrowIfNull(var);
}

void test_message() {
    MsgThrow("Test");
}

void test_assert_fail() {
    MsgAssert(1 == 0);
}

void test_assertion() {
    MsgAssert(1 == 1);
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
        test_it_not_null();
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

    TEST_TRY
        test_assertion();
    TEST_CATCH
        FAIL_IN_CATCH();
    END_TEST_TRY

END_TESTS
