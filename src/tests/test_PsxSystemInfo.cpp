#include "../inc/PsxSystemInfo.h"
#include "test.h"

START_TESTS

    ASSERT(psx::SystemInfo::osName() != "");
    ASSERT(psx::SystemInfo::architecture() != "");
    ASSERT(psx::SystemInfo::nodeName() != "");
    ASSERT(psx::SystemInfo::osRelease() != "");
    ASSERT(psx::SystemInfo::systemVersion() != "");
    
END_TESTS
