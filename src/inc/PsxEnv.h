#pragma once

#include "PsxErrno.h"
#include <map>
#include <string>
#include <unistd.h>

#if !defined(_GNU_SOURCE)
extern const char **environ;
#endif

namespace psx {

class EnvSetter {
public:
    typedef std::string String;

    String value();
    EnvSetter &set(const String &value);
    operator String();
    bool operator==(const String &compare_value);
    bool operator!=(const String &compare_value);
    EnvSetter &operator=(const String &value);

private:
    friend class Env;
    EnvSetter(const String &key);
    String _key;
};

class Env {
public:
    enum Action { Overwrite, DoNotOverwrite };
    typedef std::string String;
    typedef std::map<String, String> Dictionary;

    const String operator[](const String &key) const;
    EnvSetter operator[](const String &key);

    bool has(const String &name) const;
    String get(const String &name)const;
    Dictionary list() const;

    Env &set(const String &name, const String &value, Action action=Overwrite);
    Env &erase(const String &name);
};

inline EnvSetter::String EnvSetter::value() {
    return Env().get(_key);
}

inline EnvSetter &EnvSetter::set(const String &value) {
    Env().set(_key, value);
    return *this;
}

inline EnvSetter::operator String() {
    return value();
}

inline bool EnvSetter::operator==(const String &compare_value) {
    return value() == compare_value;
}

inline bool EnvSetter::operator!=(const String &compare_value) {
    return value() != compare_value;
}

inline EnvSetter &EnvSetter::operator=(const String &value) {
    return set(value);
}

inline EnvSetter::EnvSetter(const String &key)
    :_key(key) {}

inline const Env::String Env::operator[](const String &key) const {
    return get(key);
}

inline EnvSetter Env::operator[](const String &key) {
    return EnvSetter(key);
}

inline bool Env::has(const String &name) const {
    return nullptr != ::getenv(name.c_str());
}

inline Env::String Env::get(const String &name)const {
    const char *value = ::getenv(name.c_str());

    if (nullptr == value) {
        return "";
    }
    
    return value;
}

inline Env::Dictionary Env::list() const {
    Dictionary env;

    for (int i = 0; NULL != ::environ[i]; ++i) {
        String field = ::environ[i];
        String::size_type equalPos = field.find('=');

        if (String::npos == equalPos) {
            env[field] = ""; // NOTEST
        } else { // NOTEST
            env[field.substr(0, equalPos)] = field.substr(equalPos + 1);
        }
    }
    return env;
}

inline Env &Env::set(const String &name, const String &value, Action action) {
    ErrnoOnNegative(::setenv(name.c_str(), value.c_str(), Overwrite == action ? 1 : 0));
    return *this;
}

inline Env &Env::erase(const String &name) {
    ErrnoOnNegative(::unsetenv(name.c_str()));
    return *this;
}

} // namespace psx
