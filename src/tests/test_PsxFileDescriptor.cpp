#include "../inc/PsxFileDescriptor.h"
#include "test.h"

START_TESTS
    auto file = psx::FileDescriptor::open("bin/testFileDescriptor.txt");

    file.resize(1024);
    ASSERT(file.size() == 1024);
    file.resize(2048);
    ASSERT(file.size() == 2048);
    file.resize(512);
    ASSERT(file.size() == 512);

END_TESTS
