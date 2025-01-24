#pragma once

#include <exception>
#include <stdio.h>

using namespace std;

inline void noop() {}

#define FAIL() failures += 1; printf("::error file=%s,line=%d,col=1,endColumn=1,title=Test Failure::Test failed\n", __FILE__, __LINE__)
#define FAIL_MESSAGE(msg) failures += 1; printf("::error file=%s,line=%d,col=1,endColumn=1,title=Test Failure::%s\n", __FILE__, __LINE__, string(msg).c_str())
#define FAIL_IN_CATCH() failures += 1; printf("::error file=%s,line=%d,col=1,endColumn=1,title=Test Failure::%s\n", __FILE__, __LINE__, exception.what())
#define ASSERT(condition) \
    if (!(condition)) {                        \
        failures += 1;                         \
        printf("::error file=%s,line=%d,col=1,endColumn=1,title=Test Failure::%s\n", __FILE__, __LINE__, #condition); \
    } else noop()
#define ASSERT_IN_CATCH(condition) \
    if (!(condition)) {                        \
        failures += 1;                         \
        printf("::error file=%s,line=%d,col=1,endColumn=1,title=Test Failure::%s\n%s\n", __FILE__, __LINE__, #condition, exception.what()); \
    } else noop()

#define START_TESTS int main(const int argc, const char *argv[]) {int failures = 0;
#define TEST_TRY try {
#define TEST_CATCH } catch(const exception &exception) {
#define TEST_CATCH_TYPE(type) } catch(const type &exception) {
#define END_TEST_TRY }
#define END_TESTS if ( (argc > 0) && (0 == failures) ) {printf("PASS: %s\n", argv[0]);} return failures;}
