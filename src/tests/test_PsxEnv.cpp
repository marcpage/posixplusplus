#include "../inc/PsxEnv.h"
#include "test.h"

using namespace psx;

static bool test_conversion(const string &value) {
    return value == "value";
}

START_TESTS

    auto env = Env();
    env["test"] = "value";
    ASSERT(env.has("test"));
    ASSERT(env["test"] == "value");
    ASSERT(env["test"] != "invalid");
    ASSERT(test_conversion(env["test"]));
    
    auto listing = env.list();
    ASSERT(listing["test"] == "value");

    const auto env2 = Env();
    ASSERT(env2.has("test"));
    ASSERT(env2["test"] == "value");
    ASSERT(env2["test"] != "invalid");

    env.erase("test");
    ASSERT(!env.has("test"));
    ASSERT(env["test"] == "");

    auto new_listing = env.list();
    ASSERT(new_listing["test"] == "");

END_TESTS
