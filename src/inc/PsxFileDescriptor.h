#pragma once

#include "PsxErrno.h"
#include <fcntl.h> // open/close
#include <string>
#include <sys/stat.h> // fstat
#include <unistd.h>   // ftruncate/fsyncs
#include <sys/statvfs.h> // fstatvfs

namespace psx {

class FileDescriptor {
public:
    typedef std::string String;
    typedef int (*CloseFunc)(int);

    static FileDescriptor open(const std::string &path, int oflag=O_RDWR|O_CREAT, mode_t mode=0777);
    static FileDescriptor reference(int desc);
    static FileDescriptor own(int desc, CloseFunc closeFunc=::close);
    static FileDescriptor from(FILE *file);
    static FileDescriptor err();
    static FileDescriptor out();
    static FileDescriptor in();
    
    FileDescriptor();
    FileDescriptor(FileDescriptor&& other);
    virtual ~FileDescriptor() throw();

    operator int() const;
    operator int();
    int get() const;
    int get();

    struct stat &info(struct stat &buffer) const;
    struct statvfs &info(struct statvfs &buffer) const;
    off_t size() const;
    FileDescriptor &resize(off_t newSize);
    FileDescriptor &sync();
    String &read(String &buffer, size_t bytes);
    String read(size_t bytes);
    FileDescriptor &write(const String &data);

    FileDescriptor(const FileDescriptor& temp_obj) = delete; 
    FileDescriptor& operator=(const FileDescriptor& temp_obj) = delete; 

private:
    int _desc;
    CloseFunc _close;
};

inline FileDescriptor FileDescriptor::open(const std::string &path, int oflag, mode_t mode) {
    return own(ErrnoOnNegative(::open(path.c_str(), oflag, mode)), ::close);
}

inline FileDescriptor FileDescriptor::reference(int desc) {
    return own(desc, nullptr);
}

inline FileDescriptor FileDescriptor::own(int desc, CloseFunc closeFunc) {
    FileDescriptor descriptor;

    descriptor._desc = desc;
    descriptor._close = closeFunc;
    return descriptor;
}

inline FileDescriptor FileDescriptor::from(FILE *file) {
    return reference(ErrnoOnNegative(fileno(file)));
}

inline FileDescriptor FileDescriptor::err() {
    return reference(STDERR_FILENO);
}

inline FileDescriptor FileDescriptor::out() {
    return reference(STDOUT_FILENO);
}

inline FileDescriptor FileDescriptor::in() { // NOTEST
    return reference(STDIN_FILENO); // NOTEST
}

inline FileDescriptor::FileDescriptor()
    :_desc(0), _close(nullptr) {}

inline FileDescriptor::FileDescriptor(FileDescriptor&& other)
    :_desc(other._desc), _close(other._close) {
    if (this != &other) {
        other._desc = 0;
        other._close = nullptr;
    }
}

inline FileDescriptor::~FileDescriptor() throw() {
    if (_close) {
        _close(_desc);
    }
    _desc = 0;
    _close = nullptr;
}

inline FileDescriptor::operator int() const {
    return _desc;
}

inline FileDescriptor::operator int() {
    return _desc;
}

inline int FileDescriptor::get() const {
    return _desc;
}

inline int FileDescriptor::get() {
    return _desc;
}

inline struct stat &FileDescriptor::info(struct stat &buffer) const {
    ErrnoOnNegative(::fstat(_desc, &buffer));
    return buffer;
}

inline struct statvfs &FileDescriptor::info(struct statvfs &buffer) const {
    ErrnoOnNegative(::fstatvfs(_desc, &buffer));
    return buffer;
}

inline off_t FileDescriptor::size() const {
  struct stat buffer;

    return info(buffer).st_size;
}

inline FileDescriptor &FileDescriptor::resize(off_t newSize) {
    ErrnoOnNegative(::ftruncate(_desc, newSize));
    return *this;
}

inline FileDescriptor &FileDescriptor::sync() {
    ErrnoOnNegative(::fsync(_desc));
    return *this;
}

inline FileDescriptor::String &FileDescriptor::read(String &buffer, size_t bytes) {
    buffer.assign(bytes, '\0');
    
    auto count = ErrnoOnNegative(::read(_desc, const_cast<char*>(buffer.data()), bytes));
    buffer.erase(static_cast<size_t>(count));
    return buffer;
}

inline FileDescriptor::String FileDescriptor::read(size_t bytes) {
    String buffer;
    return read(buffer, bytes);
}

inline FileDescriptor &FileDescriptor::write(const String &data) {
    PsxAssert(ErrnoOnNegative(::write(_desc, data.data(), data.size())) == static_cast<int>(data.size()));
    return *this;
}

} // namespace psx
