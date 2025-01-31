#include "../inc/PsxFileDescriptor.h"
#include "../inc/PsxFile.h"
#include "test.h"

START_TESTS
    {
        auto fileHandle = psx::File::open("bin/testFileDescriptorFromFileHandle.txt", psx::File::Text, psx::File::ReadWrite);

        fileHandle.write("handle");
        fileHandle.flush();
        ASSERT(psx::FileDescriptor::from(fileHandle).size() == 6);
    }

    auto file = psx::FileDescriptor::open("bin/testFileDescriptor.txt");
    auto file2 = psx::FileDescriptor::open("bin/testFileDescriptorFromFileHandle.txt");
    struct statvfs info;
    
    file2.info(info);
    auto total = info.f_blocks * info.f_frsize;
    auto available = info.f_bfree * info.f_frsize;
    auto used = total - available;
    auto usedPercentage = 100.0 * used / total;
    printf("Used: %0.1f GB of %0.1f GB total %0.1f%%\n", used/1024.0/1024.0/1024.0, total/1024.0/1024.0/1024.0, usedPercentage);

    ASSERT(file2.read(10) == "handle");
    file.resize(1024);
    ASSERT(file.size() == 1024);
    file.resize(2048);
    ASSERT(file.size() == 2048);
    file.resize(512);
    ASSERT(file.size() == 512);
    psx::FileDescriptor::out().write("stdout\n").sync();
    psx::FileDescriptor::err().write("stderr\n").sync();

END_TESTS
