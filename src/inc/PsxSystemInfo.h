#pragma once

#include "PsxErrno.h"
#include <sys/utsname.h>
#include <string>

namespace psx {

class SystemInfo {
public:
    typedef std::string String;

    static void all(String &sysname, String &nodename, String &release, String &version, String &machine);
    static String osName();
    static String nodeName();
    static String osRelease();
    static String systemVersion();
    static String architecture();
};

inline void SystemInfo::all(String &sysname, String &nodename, String &release, String &version, String &machine) {
    struct utsname systemInfo;

    ErrnoOnNegative(::uname(&systemInfo));
    sysname = systemInfo.sysname;
    nodename = systemInfo.nodename;
    release = systemInfo.release;
    version = systemInfo.version;
    machine = systemInfo.machine;
}

inline SystemInfo::String SystemInfo::osName() {
    String sysname, nodename, release, version, machine;

    all(sysname, nodename, release, version, machine);
    return sysname;
}

inline SystemInfo::String SystemInfo::nodeName() {
    String sysname, nodename, release, version, machine;

    all(sysname, nodename, release, version, machine);
    return nodename;
}

inline SystemInfo::String SystemInfo::osRelease() {
    String sysname, nodename, release, version, machine;

    all(sysname, nodename, release, version, machine);
    return release;
}

inline SystemInfo::String SystemInfo::systemVersion() {
    String sysname, nodename, release, version, machine;

    all(sysname, nodename, release, version, machine);
    return version;
}

inline SystemInfo::String SystemInfo::architecture() {
    String sysname, nodename, release, version, machine;

    all(sysname, nodename, release, version, machine);
    return machine;
}


} // namespace psx
