#include "../inc/PsxAddress.h"
#include "test.h"

using namespace psx;

START_TESTS

    auto apple = IPv4("apple.com", 80);
    auto copy = IPv4(apple.get(), apple.size());

    printf("apple.name() == '%s'\n", apple.name().c_str());
    printf("copy.name() == '%s'\n", copy.name().c_str());
    ASSERT(IPv4().name() == string("0.0.0.0"));
    ASSERT(IPv6().name() == string("::"));


END_TESTS
