#pragma once

#include "PsxException.h"
#include "PsxErrno.h"
#include <math.h>
#include <string>
#include <sys/time.h>
#include <time.h>

namespace psx {

class Time {
public:
    enum Span { Seconds, Minutes, Hours, Days, Weeks };
    enum Month {
        Jan,
        January = Jan,
        Feb,
        February = Feb,
        Mar,
        March = Mar,
        Apr,
        April = Apr,
        May,
        Jun,
        June = Jun,
        Jul,
        July = Jul,
        Aug,
        August = Aug,
        Sep,
        September = Sep,
        Oct,
        October = Oct,
        Nov,
        November = Nov,
        Dec,
        December = Dec,
    };
    enum CivilianHour { AM, PM };
    enum Location { GMT, Local };
    enum Epoch {UNIX, DOS, Mac, Cocoa};
    typedef std::string String;

    static Time now();
    static Time epoch(Epoch epc=UNIX);

    Time(clockid_t clock=CLOCK_REALTIME);
    explicit Time(const time_t &time);
    explicit Time(const timeval &time);
    explicit Time(const timespec &time);
    explicit Time(tm &time, Location location=Local);
    explicit Time(const double &time);
    Time(int year, Month month, int day, int hour24=0, int minutes=0, double seconds=0.0, Location location=Local);
    Time(int year, Month month, int day, int hour, CivilianHour ampm, int minutes=0, double seconds=0.0, Location location=Local);

    operator time_t() const;
    operator timeval() const;
    Time &operator+=(double secs);
    Time operator+(double secs) const;
    Time &operator-=(double secs);
    Time operator-(double secs) const;
    double operator-(const Time &other) const;
    bool operator==(const Time &other) const;
    bool operator!=(const Time &other) const;
    bool operator<(const Time &other) const;
    bool operator>(const Time &other) const;
    bool operator<=(const Time &other) const;
    bool operator>=(const Time &other) const;

    int compare(const Time &other) const;
    double seconds_since(const Time &epoch) const;
    Time &add(double value, Span span=Seconds);
    tm &gmt(tm &time) const;
    tm &local(tm &time) const;
    tm &location(tm &time, Location loc) const;
    timeval &value(timeval &tv) const;
    timespec &value(timespec &ts) const;
    String &format(const String &fmt, String &buffer, Location location=Local) const;
    String format(const String &fmt, Location location=Local) const;

private:
    timespec _time;

    static timespec _init(tm &time, double fractionalSeconds, Location location);
    static timespec _init(int year, Month month, int day, int hour24, int minutes, double seconds, Location location);
    static int _ampm_to_hour24(int hour, CivilianHour ampm);
    template<typename S>
    static void clear(S &s);
};

template<typename S>
inline void Time::clear(S &s) {
    ::memset(&s, 0, sizeof(s));
}

inline Time Time::now() {
    return Time();
}

inline Time Time::epoch(Epoch epc) {
    int year = 0;

    switch(epc) {
        case UNIX: // Jan 1 1970 00:00:00 UTC
            return Time(static_cast<time_t>(0)); // faster
        case DOS:
            year = 1980;
            break;
        case Mac:
            year = 1904;
            break;
        case Cocoa:
            year = 2001;
            break;
        default:
            PsxThrow("Unkown Epoch"); // NOTEST
    }
    return Time(year, January, 1, GMT);
}

inline Time::Time(clockid_t clock)
    :_time{0,0} {
    ErrnoOnNegative(::clock_gettime(clock, &_time));
}

inline Time::Time(const time_t &time)
    :_time{time,0} {}

inline Time::Time(const timeval &time)
    :_time{time.tv_sec, time.tv_usec} {
    constexpr auto kNanoSecondsPerMicrosecond = 1000;
    _time.tv_nsec *= kNanoSecondsPerMicrosecond;
}

inline Time::Time(const timespec &time) 
    :_time(time) {}

inline Time::Time(tm &time, Location location)
    :_time(_init(time, 0.0, location)) {}

inline Time::Time(const double &time)
    :_time{0,0} {
    constexpr auto kNanosecondsPerSecond = 1000000000.0;
    const double wholeSeconds = ::floor(time);

    _time.tv_sec = static_cast<time_t>(wholeSeconds);
    _time.tv_nsec = static_cast<long>( (time - wholeSeconds) * kNanosecondsPerSecond );
}

inline Time::Time(int year, Month month, int day, int hour24, int minutes, double seconds, Location location)
    :_time(_init(year, month, day, hour24, minutes, seconds, location)) {}

inline Time::Time(int year, Month month, int day, int hour, CivilianHour ampm, int minutes, double seconds, Location location)
    :_time(_init(year, month, day, _ampm_to_hour24(hour, ampm), minutes, seconds, location)) {}

inline Time::operator time_t() const {
    return _time.tv_sec;
}

inline Time::operator timeval() const {
    struct timeval timeValue;
    clear(timeValue);
    return value(timeValue);
}

inline Time &Time::operator+=(double secs) {
    return add(secs, Seconds);
}

inline Time Time::operator+(double secs) const {
    return Time(*this).add(secs, Seconds);
}

inline Time &Time::operator-=(double secs) {
    return add(-1 * secs, Seconds);
}

inline Time Time::operator-(double secs) const {
    return Time(*this).add(-1 * secs, Seconds);
}

inline double Time::operator-(const Time &other) const {
    return seconds_since(epoch()) - other.seconds_since(epoch());
}

inline bool Time::operator==(const Time &other) const {
    return compare(other) == 0;
}

inline bool Time::operator!=(const Time &other) const {
    return compare(other) != 0;
}

inline bool Time::operator<(const Time &other) const {
    return compare(other) < 0;
}

inline bool Time::operator>(const Time &other) const {
    return compare(other) > 0;
}

inline bool Time::operator<=(const Time &other) const {
    return compare(other) <= 0;
}

inline bool Time::operator>=(const Time &other) const {
    return compare(other) >= 0;
}

inline int Time::compare(const Time &other) const {
    if (_time.tv_sec < other._time.tv_sec) {
        return -1;
    }

    if (_time.tv_sec > other._time.tv_sec) {
        return 1;
    }

    if (_time.tv_nsec < other._time.tv_nsec) {
        return -1;
    }

    if (_time.tv_nsec > other._time.tv_nsec) {
        return 1;
    }

    return 0;
}

inline double Time::seconds_since(const Time &epoch) const {
    constexpr auto kNanosecondsPerSecond = 1000000000.0;
    return double(_time.tv_sec - epoch._time.tv_sec)
            + ( double(_time.tv_nsec - epoch._time.tv_nsec) 
                / kNanosecondsPerSecond );
}

inline Time &Time::add(double value, Span span) {
    switch(span) {
        case Weeks:
            value *= 7.0;
            // fall through
        case Days:
            value *= 24.0;
            // fall through
        case Hours:
            value *= 60.0;
            // fall through
        case Minutes:
            value *= 60.0;
            // fall through
        case Seconds:
            break;
        default:
            PsxThrow("Unknown timespan"); // NOTEST
    }
    return *this = Time(seconds_since(epoch()) + value);
}

inline tm &Time::gmt(tm &time) const {
    clear(time);
    const time_t value = _time.tv_sec;
    PsxThrowIfNull(::gmtime_r(&value, &time));
    return time;
}

inline tm &Time::local(tm &time) const {
    clear(time);
    const time_t value = _time.tv_sec;
    PsxThrowIfNull(::localtime_r(&value, &time));
    return time;
}

inline tm &Time::location(tm &time, Location loc) const {
    switch(loc) {
        case GMT:
            return gmt(time); // time cleared in here
        case Local:
            return local(time); // time cleared in here
        default:
            PsxThrow("Unknown location"); // NOTEST
    }
}

inline timeval &Time::value(timeval &tv) const {
    constexpr auto kNanoSecondsPerMicrosecond = 1000;
    tv.tv_sec = _time.tv_sec;
    tv.tv_usec = static_cast<suseconds_t>(_time.tv_nsec / kNanoSecondsPerMicrosecond);
    return tv;
}

inline timespec &Time::value(timespec &ts) const {
    return ts = _time;
}

inline Time::String &Time::format(const String &fmt, String &buffer, Location loc) const {
    size_t size;
    struct tm timeValue;

    location(timeValue, loc); // clears timeValue
    buffer.assign(fmt.length() * 15, '\0');
#pragma GCC diagnostic push
#pragma GCC diagnostic ignored "-Werror"
    size = ::strftime(const_cast<char *>(buffer.data()), buffer.length(), fmt.c_str(), &timeValue);
#pragma GCC diagnostic pop
    buffer.erase(size);
    PsxAssert(size != 0 || fmt.size() == 0 || fmt == "%p");
    return buffer;
}

inline Time::String Time::format(const String &fmt, Location location) const {
    String buffer;

    return format(fmt, buffer, location);
}

inline timespec Time::_init(tm &time, double fractionalSeconds, Location location) {
    timespec spec;
    clear(spec);
    constexpr auto kNanosecondsPerSecond = 1000000000.0;
    PsxAssert(fractionalSeconds < 1.0);
    PsxAssert(fractionalSeconds >= 0.0);
    PsxAssert( (location == GMT) || (location == Local));
    spec.tv_sec = location == GMT ? ::timegm(&time) : ::mktime(&time);
    PsxAssert(spec.tv_sec != static_cast<time_t>(-1));
    spec.tv_nsec = static_cast<long>(fractionalSeconds * kNanosecondsPerSecond);
    return spec;
}

inline timespec Time::_init(int year, Month month, int day, int hour24, int minutes, double seconds, Location location) {
    struct tm date;
    clear(date);
    const double wholeSeconds = ::floor(seconds);

    date.tm_year = year - 1900;
    date.tm_mon = static_cast<int>(month);
    date.tm_mday = day;
    date.tm_hour = hour24;
    date.tm_min = minutes;
    date.tm_sec = static_cast<int>(wholeSeconds);
    return _init(date, seconds - wholeSeconds, location);
}

inline int Time::_ampm_to_hour24(int hour, CivilianHour ampm) {
    switch(ampm) {
        case AM:
            return hour == 12 ? 0 : hour;
        case PM:
            return hour + 12;
        default:
            PsxThrow("Unknown CivilianHour"); // NOTEST
    }
}

} // namespace psx
