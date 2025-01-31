#pragma once

#include "PsxException.h"
#include "PsxErrno.h"
#include <dirent.h>
#include <stdio.h>
#include <stdlib.h>
#include <string>
#include <sys/param.h>
#include <sys/stat.h>
#include <sys/statvfs.h>
#include <fstab.h>
#include <unistd.h>
#include <string>
#include <vector>

namespace psx {

class Path {
public:
    enum LinkHandling { WorkOnLink, WorkOnLinkTarget };
    enum HavePath { NameOnly, PathAndName };
    enum Depth { RecursiveListing, FlatListing };
    typedef std::string String;
    typedef std::vector<String> StringList;

    static Path working();

    explicit Path(const char *path);
    Path(const String &path="");

    Path operator/(const char *other) const;
    Path operator/(const Path &other) const;
    Path operator+(const char *other) const;
    Path operator+(const Path &other) const;
    bool operator==(const Path &other) const;
    bool operator!=(const Path &other) const;
    String operator[](int index) const;
    int count() const;
    operator String() const;
    const String &get() const;

    struct stat &info(struct stat &info, LinkHandling action=WorkOnLinkTarget) const;
    struct statvfs &info(struct statvfs &info) const;
    bool exists(LinkHandling action=WorkOnLinkTarget) const;
    bool isDirectory(LinkHandling action=WorkOnLinkTarget) const;
    bool isFile(LinkHandling action=WorkOnLinkTarget) const;
    bool isBlockSpecial(LinkHandling action=WorkOnLinkTarget) const;
    bool isCharSpecial(LinkHandling action=WorkOnLinkTarget) const;
    bool isFifo(LinkHandling action=WorkOnLinkTarget) const;
    bool isSocket(LinkHandling action=WorkOnLinkTarget) const;
    bool isLink() const;
    bool isRelative() const;
    bool isAbsolute() const;
    bool isEmpty() const;
    String readLink() const;
    dev_t device(LinkHandling action=WorkOnLinkTarget) const;
    ino_t inode(LinkHandling action=WorkOnLinkTarget) const;
    mode_t permissions(LinkHandling action=WorkOnLinkTarget) const;
    nlink_t links(LinkHandling action=WorkOnLinkTarget) const;
    uid_t userId(LinkHandling action=WorkOnLinkTarget) const;
    gid_t groupId(LinkHandling action=WorkOnLinkTarget) const;
    timespec lastAccess(LinkHandling action=WorkOnLinkTarget) const;
    timespec lastModification(LinkHandling action=WorkOnLinkTarget) const;
    timespec lastStatusChange(LinkHandling action=WorkOnLinkTarget) const;
    timespec created(LinkHandling action=WorkOnLinkTarget) const;
    off_t size(LinkHandling action=WorkOnLinkTarget) const;
    off_t blocks(LinkHandling action=WorkOnLinkTarget) const;
    off_t blockSize(LinkHandling action=WorkOnLinkTarget) const;

    void unlink();
    void rmdir();
    void remove();
    Path &mkdir(unsigned int mode=0777);
    Path &mkdirs(unsigned int mode=0777);
    Path &rename(Path &other);
    const Path &symlink(const char *contents) const;
    
    Path mount() const;
    Path relativeTo(const Path &other) const;
    Path relativeTo(const char *other) const;
    Path absolute(const Path &from=working()) const;
    Path parent() const;
    String name() const;
    String extension() const;
    String basename() const;
    Path canonical() const;

    StringList list(HavePath includePath, Depth recursive=FlatListing) const;
    StringList &list(HavePath includePath, StringList &directoryListing, Depth recursive=FlatListing) const;

private:
    String _path;
    static const char kSeparator = '/';
    
    bool _exists(struct stat &info, LinkHandling action) const;
    bool _elements(int index, std::string::size_type &start, std::string::size_type &end, int &count) const;
};

inline Path Path::working() {
  char buffer[MAXPATHLEN];

  return Path(ErrnoOnNull(::getcwd(buffer, sizeof(buffer))));
}

inline Path::Path(const char *path)
    :_path(path) {}

inline Path::Path(const String &path)
    :_path(path) {}

inline Path Path::operator/(const char *other) const {
    return *this/Path(other);
} // NOTEST, function, yes, closing brace, no

inline Path Path::operator/(const Path &other) const {
    String result = _path;
    const auto separatorString = std::string() + kSeparator;
    auto separator = (isEmpty() || _path == separatorString) ? std::string() : separatorString;

    for (int i = 0; i < other.count(); ++i) {
        result += separator;
        result += other[i];
        separator = kSeparator;
    }

    return result;
}

inline Path Path::operator+(const char *other) const {
    return *this/Path(other);
} // NOTEST, function, yes, closing brace, no

inline Path Path::operator+(const Path &other) const {
    return *this / other;
}

inline bool Path::operator==(const Path &other) const {
    if (isAbsolute() != other.isAbsolute()) {
        return false;
    }
    
    const int elements = count();

    if (elements != other.count()) {
        return false;
    }

    for (int i = 0; i < elements; ++i) {
        if ((*this)[i] != other[i]) {
            return false;
        }
    }

    return true;
}

inline bool Path::operator!=(const Path &other) const {
    return !(*this == other);
}

inline Path::String Path::operator[](int index) const {
    std::string::size_type start, end;
    int count;

    if (!_elements(index, start, end, count)) {
        return String();
    }

    return _path.substr(start, end - start);
}

inline int Path::count() const {
    std::string::size_type start, end;
    int count;

    _elements(_path.size() + 1, start, end, count);
    return count;
}

inline Path::operator String() const {
    return _path;
}

inline const Path::String &Path::get() const {
    return _path;
}

inline bool Path::exists(LinkHandling action) const {
    struct stat info;

    return _exists(info, action);
}

inline bool Path::isDirectory(LinkHandling action) const {
    struct stat info;

    if (_exists(info, action)) {
        return S_ISDIR(info.st_mode) ? true : false;
    }

    return false;
}

inline bool Path::isFile(LinkHandling action) const {
    struct stat info;

    if (_exists(info, action)) {
        return S_ISREG(info.st_mode) ? true : false;
    }

    return false;
}

inline bool Path::isLink() const {
    struct stat info;

    if (_exists(info, WorkOnLink)) {
        return S_ISLNK(info.st_mode) ? true : false;
    }

    return false;
}

inline bool Path::isBlockSpecial(LinkHandling action) const {
    struct stat info;

    if (_exists(info, action)) {
        return S_ISBLK(info.st_mode) ? true : false;
    }

    return false;
}

inline bool Path::isCharSpecial(LinkHandling action) const {
    struct stat info;

    if (_exists(info, action)) {
        return S_ISCHR(info.st_mode) ? true : false;
    }

    return false;
}

inline bool Path::isFifo(LinkHandling action) const {
    struct stat info;

    if (_exists(info, action)) {
        return S_ISFIFO(info.st_mode) ? true : false;
    }

    return false;
}

inline bool Path::isSocket(LinkHandling action) const {
    struct stat info;

    if (_exists(info, action)) {
        return S_ISSOCK(info.st_mode) ? true : false;
    }

    return false;
}

inline bool Path::isRelative() const {
    return !isAbsolute();
}

inline bool Path::isAbsolute() const {
    return _path.length() > 0 && _path[0] == kSeparator;
}

inline bool Path::isEmpty() const {
    return _path.length() == 0;
}

inline Path::String Path::readLink() const {
    ssize_t length;
    String linkPath(PATH_MAX, '\0');

    ErrnoOnNegative(length = ::readlink(_path.c_str(), const_cast<char *>(linkPath.data()), linkPath.length()));
    linkPath.erase(static_cast<std::string::size_type>(length));
    return linkPath;
}

inline dev_t Path::device(LinkHandling action) const {
    struct stat item_info;

    return info(item_info, action).st_dev;
}

inline ino_t Path::inode(LinkHandling action) const {
    struct stat item_info;

    return info(item_info, action).st_ino;
}

inline mode_t Path::permissions(LinkHandling action) const {
    struct stat item_info;

    return info(item_info, action).st_mode;
}

inline nlink_t Path::links(LinkHandling action) const {
    struct stat item_info;

    return info(item_info, action).st_nlink;
}

inline uid_t Path::userId(LinkHandling action) const {
    struct stat item_info;

    return info(item_info, action).st_uid;
}

inline gid_t Path::groupId(LinkHandling action) const {
    struct stat item_info;

    return info(item_info, action).st_gid;
}

inline timespec Path::lastAccess(LinkHandling action) const {
    struct stat item_info;

#if defined(__APPLE__)
    return info(item_info, action).st_atimespec;
#else
    return info(item_info, action).st_atim;
#endif
}

inline timespec Path::lastModification(LinkHandling action) const {
    struct stat item_info;

#if defined(__APPLE__)
    return info(item_info, action).st_mtimespec;
#else
    return info(item_info, action).st_mtim;
#endif
}

inline timespec Path::lastStatusChange(LinkHandling action) const {
    struct stat item_info;

#if defined(__APPLE__)
    return info(item_info, action).st_ctimespec;
#else
    return info(item_info, action).st_ctim;
#endif
}

inline timespec Path::created(LinkHandling action) const {
    struct stat item_info;

#if defined(__APPLE__)
    return info(item_info, action).st_birthtimespec;
#else
    return info(item_info, action).st_mtim;
#endif
}

inline off_t Path::size(LinkHandling action) const {
    struct stat item_info;

    return info(item_info, action).st_size;
}

inline off_t Path::blocks(LinkHandling action) const {
    struct stat item_info;

    return info(item_info, action).st_blocks;
}

inline off_t Path::blockSize(LinkHandling action) const {
    struct stat item_info;

    return info(item_info, action).st_blksize;
}

inline void Path::unlink() {
    ErrnoOnNegative(::unlink(_path.c_str()));
}

inline void Path::rmdir() {
    ErrnoOnNegative(::rmdir(_path.c_str()));
}

inline void Path::remove() {
    if (isDirectory(WorkOnLink)) {
        StringList contents;

        list(PathAndName, contents);

        for (auto i : contents) {
            Path(i).remove();
        }

        rmdir();
    } else {
        unlink();
    }
}

inline Path &Path::mkdir(unsigned int mode) {
    ErrnoOnNegative(::mkdir(_path.c_str(), mode));
    return *this;
}

inline Path &Path::mkdirs(unsigned int mode) {
    if (!isEmpty()) {
        if (!isDirectory()) {
            parent().mkdirs(mode);
            mkdir(mode);
        }
    }

    return *this;
} // NOTEST, function, yes, closing brace, no

inline Path &Path::rename(Path &other) {
    ErrnoOnNegative(::rename(_path.c_str(), other._path.c_str()));
    return other;
}

inline const Path &Path::symlink(const char *contents) const {
    ErrnoOnNegative(::symlink(contents, _path.c_str()));
    return *this;
}

inline Path Path::mount() const {
    auto filesystem = device();
    Path found;

    PsxAssert(::setfsent());

    while (auto fstab = ::getfsent()) {
        auto file = Path(fstab->fs_file);

        if (file.device() == filesystem) {
            found = file;
        }
    }
    ::endfsent();
    return found;
}

inline Path Path::relativeTo(const Path &other) const {
    if (_path == other._path) {
        return Path("");
    }

    if (!isAbsolute()) {
        PsxThrow(String("Path is not absolute: '") + String(*this) + "'");
    }

    if (!other.isAbsolute()) {
        PsxThrow(String("Path is not absolute: '") + String(other) + "'");
    }

    const auto sourceCount = other.count();
    const auto destCount = count();
    const auto maxPossibleCommonCount = std::min(sourceCount, destCount);

    if ( (sourceCount == 0) && (destCount == 0)) {
        return Path("");
    }

    int matchCount = 0;
    Path result("");

    while (matchCount < maxPossibleCommonCount && (*this)[matchCount] == other[matchCount]) {
        matchCount += 1;
    }

    for (int up = 0; up < sourceCount - matchCount; ++up) {
        result = result/"..";
    }

    for (int down = matchCount; down < destCount; ++down) {
        result = result/(*this)[down];
    }

    return result;
}

inline Path Path::relativeTo(const char *other) const {
    return relativeTo(Path(other));
}

inline Path Path::absolute(const Path &from) const {
    if (isAbsolute()) {
        return *this;
    }

    return (from + *this).canonical();
}

inline Path Path::parent() const {
    auto sepPos = _path.rfind(kSeparator);

    if (sepPos == String::npos) {
        return Path("");
    }

    if (sepPos == 0) {
        return Path(String() + kSeparator); // root
    }

    const auto separatorString = std::string() + kSeparator;
    Path result(isAbsolute() ? separatorString : "");
    const auto parentSize = count() - 1;

    for (int i = 0; i < parentSize; ++i) {
        result = result/(*this)[i];
    }

    return result;
}

inline Path::String Path::name() const {
    auto sepPos = _path.rfind(kSeparator);

    if (sepPos == String::npos) {
        return _path;
    }

    return (*this)[count() - 1];
}

inline Path::String Path::extension() const {
    const auto filename = name();
    auto sepPos = filename.rfind('.');

    if (sepPos == String::npos) {
        return "";
    }
    return filename.substr(sepPos);
}

inline Path::String Path::basename() const {
    auto fullname = name();
    auto sepPos = fullname.rfind('.');

    if (sepPos == String::npos) {
        return fullname;
    }
    return fullname.substr(0, sepPos);
}

inline Path Path::canonical() const {
    String buffer(PATH_MAX, '\0');

    ErrnoOnNull(::realpath(String(*this).c_str(), const_cast<char *>(buffer.data())));
    buffer.erase(::strlen(buffer.c_str()));
    return buffer;
}

inline Path::StringList Path::list(HavePath includePath, Depth recursive) const {
    StringList directoryListing;

    return list(includePath, directoryListing, recursive);
}

inline Path::StringList &Path::list(HavePath includePath, StringList &directoryListing, Depth recursive) const {
    DIR *dp;
    StringList directories;

    directoryListing.clear();
    ErrnoOnNull(dp = ::opendir(String(*this).c_str()));

    try {
        bool keepListing = true;

        do {
            try {
                struct dirent *ep;
                ErrnoOnNull(ep = ::readdir(dp));

                if (NULL != ep) {
                    #if defined(__APPLE__)
                    const String name = String(ep->d_name, 0, ep->d_namlen);
                    #else
                    const String name = String(ep->d_name); //, 0, ep->d_namlen);
                    #endif
                    const bool isDirectory = DT_DIR == ep->d_type;

                    if ((name != ".") && (name != "..")) {
                        directoryListing.push_back((includePath == NameOnly ? String() : (String(*this) + kSeparator)) + name + (isDirectory ? (String()+kSeparator) : String()));
                        if ((RecursiveListing == recursive) && isDirectory) {
                            directories.push_back(String(*this) + kSeparator + name);
                        }
                    }
                } else {
                    keepListing = false; // NOTEST
                }
            } catch (const psx::ESRCH_Errno &) {
                keepListing = false; // NOTEST sometimes we get spurious ESRCH No
                                    // such process after listing a directory
            } catch (const psx::EINTR_Errno &) { // NOTEST not covered by tests
                keepListing = false; // NOTEST sometimes we get spurious EINTR when
                                    // there is an empty directory
            } // NOTEST
        } while (keepListing);
    } catch (const psx::ENOENT_Errno &) {
    } catch (const std::exception &) {
        ErrnoOnNegative(::closedir(dp)); // NOTEST
        throw; // NOTEST
    }

    ErrnoOnNegative(::closedir(dp));

    for (auto directory : directories) {
        Path(directory).list(includePath, directoryListing, recursive);
    }

    return directoryListing;
}

inline bool Path::_exists(struct stat &item_info, LinkHandling action) const {
    try {
        info(item_info, action);
    } catch (const psx::ENOENT_Errno &) {
        return false;
    }

    return true;
}

inline struct stat &Path::info(struct stat &f_info, LinkHandling action) const {
    if (WorkOnLink == action) {
        ErrnoOnNegative(::lstat(_path.c_str(), &f_info));
    } else {
        ErrnoOnNegative(::stat(_path.c_str(), &f_info));
    }

    return f_info;
}

inline struct statvfs &Path::info(struct statvfs &fs_info) const {
    ErrnoOnNegative(::statvfs(_path.c_str(), &fs_info));
    return fs_info;
}

inline bool Path::_elements(int index, std::string::size_type &start, std::string::size_type &end, int &count) const {
    const auto separatorString = std::string() + kSeparator;
    count = 0;
    start = 0;

    if (_path.empty() || _path == separatorString) {
        return false;
    }

    while ((end = _path.find(kSeparator, start)) != std::string::npos) {
        if (end > start) { // Ensure non-empty element
            count += 1;

            if (count == index + 1) {
                return true;
            }
        }
        start = end + 1;
    }

    if (start < _path.size()) { // Count the last element
        count += 1;

        if (count == index + 1) {
            return true;
        }
    }

    return false;
}


} // namespace psx
