#pragma once

#include "PsxErrno.h"
#include <stdio.h>

namespace psx {

class File {
public:
    static constexpr auto RestOfFile = static_cast<size_t>(-1);
    enum Method { Binary, Text };
    enum Protection { ReadOnly, ReadWrite, TryWrite };
    enum Relative { FromHere, FromStart, FromEnd };
    typedef int (*CloseFunc)(FILE*);

    static File open(const char *path, Method method=Text, Protection protection=ReadOnly);
    static File open(const std::string &path, Method method=Text, Protection protection=ReadOnly);
    static File execute(const std::string &command);
    static File reference(FILE *file, bool readOnly=false);
    static File own(FILE *file, CloseFunc closeFunc=::fclose, bool readOnly=false);
    static File err();
    static File out();
    static File in();

    File();
    File(File&& other);
    virtual ~File();

    operator FILE *() const;
    operator FILE *();
    FILE *get() const;
    FILE *get();

    off_t size() const;
    off_t location() const;
    bool writeable() const;
    bool endOfFile() const;

    const File &moveto(off_t offset, Relative relative=FromStart) const;
    const File &move(off_t offset, Relative relative=FromHere) const;
    const File &read(void *buffer, size_t bufferSize, off_t offset = 0, Relative relative = FromHere) const;
    std::string &read(std::string &buffer, size_t bytesToRead=RestOfFile, off_t offset=0, Relative relative=FromHere) const;
    std::string read(size_t bytesToRead=RestOfFile, off_t offset=0, Relative relative=FromHere) const;
    std::string &readLine(std::string &buffer, off_t offset=0, Relative relative=FromHere, size_t bufferSize=4096) const;
    std::string readLine(off_t offset=0, Relative relative=FromHere, size_t bufferSize=4096) const;
    std::string &readToEnd(std::string &buffer, const size_t blocks=4096) const;
    std::string readToEnd(const size_t blocks=4096) const;

    File &write(const void *buffer, size_t bufferSize, off_t offset=0, Relative relative=FromHere);
    File &write(const std::string &buffer, off_t offset=0, Relative relative=FromHere);
    File &flush();

    File(const File& temp_obj) = delete; 
    File& operator=(const File& temp_obj) = delete; 

private:
    FILE *_file;
    bool _readOnly;
    CloseFunc _close;

    File(FILE *file, CloseFunc closeFunc, bool readOnly);
    size_t _readCore(void *buffer, size_t bufferSize, bool exactSize=true) const;
    void _goto(off_t offset, Relative relative) const;
    void _locationAndSize(off_t &location, off_t &size) const;

    static int _whence(Relative relative);
    static FILE *_open(const char *path, Method method, Protection protection, bool &readOnly);    
};


inline File::File()
    :_file(nullptr), _readOnly(true), _close(nullptr) {}

inline File::File(File&& other)
    :_file(other._file), _readOnly(other._readOnly), _close(other._close) {
    if (this != &other) {
        other._file = nullptr;
    }
}

inline File::~File() {
    if (_close && _file) {
        _close(_file);
    }
    _file = nullptr;
    _close = nullptr;
    _readOnly = true;
}

inline File::operator FILE *() const {
    return get();
}

inline File::operator FILE *() {
    return get();
}

inline FILE *File::get() const {
    return _file;
}

inline FILE *File::get() {
    return _file;
}

inline off_t File::size() const {
    off_t currentPos;
    off_t end;

    _locationAndSize(currentPos, end);
    return end;
}

inline off_t File::location() const {
    return ErrnoOnNegative(::ftello(_file));
}

inline bool File::writeable() const {
    return !_readOnly;
}

inline bool File::endOfFile() const {
    return ::feof(_file) != 0;
}

inline const File &File::moveto(off_t offset, Relative relative) const {
    ErrnoOnNegative(::fseeko(_file, offset, _whence(relative)));
    return *this;
}

inline const File &File::move(off_t offset, Relative relative) const {
    return moveto(offset, relative);
}

inline const File &File::read(void *buffer, size_t bufferSize, off_t offset, Relative relative) const {
    off_t currentPos, current_size;

    _goto(offset, relative);
    _locationAndSize(currentPos, current_size);
    PsxAssert(currentPos + static_cast<off_t>(bufferSize) <= current_size);
    _readCore(buffer, bufferSize);
    return *this;
}

inline std::string &File::read(std::string &buffer, size_t bytesToRead, off_t offset, Relative relative) const {
    off_t currentPos, current_size;

    _goto(offset, relative);
    _locationAndSize(currentPos, current_size);

    if (RestOfFile == bytesToRead) {
        bytesToRead = static_cast<size_t>(current_size - currentPos);
    }

    PsxAssert(currentPos + static_cast<off_t>(bytesToRead) <= current_size);
    buffer.assign(bytesToRead, '\0');
    _readCore(const_cast<char *>(buffer.data()), bytesToRead);
    return buffer;
}

inline std::string File::read(size_t bytesToRead, off_t offset, Relative relative) const {
    std::string buffer;

    return read(buffer, bytesToRead, offset, relative);
}

inline std::string &File::readLine(std::string &buffer, off_t offset, Relative relative, size_t bufferSize) const {
    std::string partial;
    std::string::size_type lf, cr, eol;
    off_t left;
    bool foundCR = false, foundLF = false;
    off_t currentPos, current_size;
    PsxAssert(bufferSize > 0);

    buffer.clear();
    _goto(offset, relative);
    _locationAndSize(currentPos, current_size);
    left = current_size - currentPos;

    while (!foundCR && !foundLF && (left > 0)) {
        bufferSize = bufferSize > static_cast<size_t>(left)
                        ? static_cast<size_t>(left)
                        : bufferSize;
        read(partial, bufferSize, 0, FromHere);
        left -= static_cast<off_t>(partial.size());
        cr = partial.find('\r');
        foundCR = (cr != std::string::npos);

        if (foundCR && (cr == partial.size() - 1) && (left > 0)) {
            char character;

            read(&character, 1, 0, FromHere);
            partial.append(1, character);
            left -= 1;
        }

        lf = partial.find('\n');
        foundLF = (lf != std::string::npos);
        eol = std::string::npos;

        if (foundCR && foundLF) {
        eol = ((cr + 1 == lf) || (lf < cr)) ? lf : cr;
        } else if (foundLF || foundCR) {
        eol = foundLF ? lf : cr;
        }

        if (eol != std::string::npos) {
            const off_t rewindTo = static_cast<off_t>(partial.size() - eol - 1);

            _goto(-1 * rewindTo, FromHere);
            partial.erase(eol + 1);
        }

        buffer.append(partial);
    }

  return buffer;
}

inline std::string File::readLine(off_t offset, Relative relative, size_t bufferSize) const {
    std::string buffer;

    return readLine(buffer, offset, relative, bufferSize);
}

inline std::string &File::readToEnd(std::string &buffer, const size_t blocks) const {
    buffer.clear();

    while (!endOfFile()) {
        std::string::size_type offset = buffer.size();
        size_t amount;

        buffer.append(blocks, '\0');
        amount = _readCore(&const_cast<char *>(buffer.data())[offset], blocks, false);

        if (static_cast<size_t>(amount) != blocks) {
            buffer.erase(offset + amount);
            ErrnoCodeThrow(::ferror(_file), "buffer error");
        }
    }

    return buffer;
}

inline std::string File::readToEnd(const size_t blocks) const {
    std::string buffer;

    return readToEnd(buffer, blocks);
}

inline File &File::write(const void *buffer, size_t bufferSize, off_t offset, Relative relative) {
    PsxAssert(!_readOnly);
    _goto(offset, relative);
    auto amount = ::fwrite(reinterpret_cast<const char *>(buffer), 1, bufferSize, _file);
    PsxAssert(::ferror(_file) == 0);
    PsxAssert(amount == bufferSize);
    return *this;
}

inline File &File::write(const std::string &buffer, off_t offset, Relative relative) {
    write(buffer.data(), buffer.size(), offset, relative);
    return *this;
}

inline File &File::flush() {
    ErrnoOnNegative(::fflush(_file));
    return *this;
}

inline File File::open(const char *path, Method method, Protection protection) {
    auto readOnly = false;
    auto file = _open(path, method, protection, readOnly);

    return own(file, ::fclose, readOnly);
}

inline File File::open(const std::string &path, Method method, Protection protection) {
    bool readOnly = false;
    auto file = _open(path.c_str(), method, protection, readOnly);

    return own(file, ::fclose, readOnly);
}

inline File File::execute(const std::string &command) {
    const auto readOnly = true;
    return own(ErrnoOnNullMsg(::popen(command.c_str(), "r"), command), ::pclose, readOnly);
}

inline File File::reference(FILE *file, bool readOnly) {
    return File(file, nullptr, readOnly);
}

inline File File::own(FILE *file, CloseFunc closeFunc, bool readOnly) {
    return File(file, closeFunc, readOnly);
}

inline File File::err() {
    return reference(stderr, false);
}

inline File File::out() {
    return reference(stdout, false);
}

inline File File::in() {
    return reference(stdin, true);
}

inline File::File(FILE *file, CloseFunc closeFunc, bool readOnly)
    :_file(file), _readOnly(readOnly), _close(closeFunc) {}

inline size_t File::_readCore(void *buffer, size_t bufferSize, bool exactSize) const {
    int fileError;
    auto amount = ::fread(reinterpret_cast<char *>(buffer), 1, bufferSize, _file);

    PsxAssert(((fileError = ::ferror(_file)) == 0) || (fileError == EOF));
    PsxAssert(!exactSize || amount == bufferSize);
    return amount;
}

inline void File::_goto(off_t offset, Relative relative) const {
    if ((0 != offset) || (relative != FromHere)) {
        moveto(offset, relative);
    }
}

inline void File::_locationAndSize(off_t &currentPos, off_t &current_size) const {
    currentPos = location();

    moveto(0, FromEnd);
    current_size = location();
    moveto(currentPos, FromStart);
}

inline int File::_whence(File::Relative relative) {
    switch(relative) {
        case FromHere: return SEEK_CUR;
        case FromStart: return SEEK_SET;
        case FromEnd: return SEEK_END;
        default: PsxThrow(std::string("Invalid relative: " + std::to_string(static_cast<int>(relative)))); // NOTEST
    }
}

inline FILE *File::_open(const char *path, File::Method method, File::Protection protection, bool &readOnly) {
    FILE *opened = nullptr;
    const bool tryWritable = ((ReadWrite == protection) | (TryWrite == protection));

    if (tryWritable) {
        opened = ::fopen(path, Binary == method ? "r+b" : "r+");
    }

    if ((nullptr == opened) && tryWritable) {
        opened = ::fopen(path, Binary == method ? "w+b" : "w+");
    }

    if (ReadWrite == protection) {
        ErrnoOnNullMsg(opened, path); // tried to open it as write, but could not
    }

    if (nullptr != opened) {
        readOnly = false;
        return opened;
    }

    ErrnoOnNullMsg(opened = ::fopen(path, Binary == method ? "rb" : "r"), path);
    readOnly = true;
    return opened;
}
    


} // namespace psx