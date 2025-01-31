#include "../inc/PsxTime.h"
#include "test.h"

START_TESTS

    auto t1 = psx::Time(2018, psx::Time::Feb, 5, 14, 30, 15);
    psx::Time copy1(t1);

    ASSERT( t1 == copy1 );
    ASSERT(t1.format("%A") == "Monday");
    ASSERT(t1.format("%a") == "Mon");
    ASSERT(t1.format("%B") == "February");
    ASSERT(t1.format("%b") == "Feb");
    ASSERT(t1.format("%C") == "20");
    ASSERT(t1.format("%D") == "02/05/18");
    ASSERT(t1.format("%d") == "05");
    ASSERT(t1.format("%F") == "2018-02-05");
    ASSERT(t1.format("%M") == "30");
    ASSERT(t1.format("%m") == "02");
    ASSERT(t1.format("%p") == "PM");
    ASSERT(t1.format("%S") == "15");

    ASSERT(t1.format("%A", psx::Time::GMT) == "Monday");
    ASSERT(t1.format("%a", psx::Time::GMT) == "Mon");
    ASSERT(t1.format("%B", psx::Time::GMT) == "February");
    ASSERT(t1.format("%b", psx::Time::GMT) == "Feb");
    ASSERT(t1.format("%C", psx::Time::GMT) == "20");
    ASSERT(t1.format("%D", psx::Time::GMT) == "02/05/18");
    ASSERT(t1.format("%d", psx::Time::GMT) == "05");
    ASSERT(t1.format("%F", psx::Time::GMT) == "2018-02-05");
    ASSERT(t1.format("%M", psx::Time::GMT) == "30");
    ASSERT(t1.format("%m", psx::Time::GMT) == "02");
    ASSERT(t1.format("%p", psx::Time::GMT) == "PM");
    ASSERT(t1.format("%S", psx::Time::GMT) == "15");

    time_t now;
    time(&now);
    ASSERT(static_cast<time_t>(psx::Time::now()) == now);
    time(&now);
    ASSERT(psx::Time::now() - psx::Time(now) < 1.0);

    struct timeval timeValue;
    ErrnoOnNegative(::gettimeofday(&timeValue, nullptr));
    ASSERT(static_cast<timeval>(psx::Time::now()).tv_sec == timeValue.tv_sec);
    ErrnoOnNegative(::gettimeofday(&timeValue, nullptr));
    ASSERT(psx::Time::now() - psx::Time(timeValue) < 1.0);

    timespec timespecValue, timespecValue2;
    ErrnoOnNegative(::clock_gettime(CLOCK_REALTIME, &timespecValue));
    ASSERT(psx::Time::now() - psx::Time(timespecValue) < 1.0);
    ASSERT(psx::Time(timespecValue).value(timespecValue2).tv_nsec == timespecValue.tv_nsec);

    struct tm date;
    date.tm_year = 120;
    date.tm_mon = 5;
    date.tm_mday = 28;
    date.tm_hour = 20;
    date.tm_min = 57;
    date.tm_sec = 54;
    //const char *dateFormat = "%Y/%m/%d %H:%M:%S";

    ASSERT(psx::Time(date, psx::Time::GMT) 
            == psx::Time(2020, psx::Time::June, 28, 20, 57, 54, psx::Time::GMT));
    ASSERT(psx::Time(date, psx::Time::GMT)
            == psx::Time(2020, psx::Time::June, 28, 8, psx::Time::PM, 57, 54, psx::Time::GMT));

    ASSERT(psx::Time() - ((psx::Time() + 5.0) - 5.0) < 1.0);
    ASSERT(psx::Time() - ((psx::Time() + 5.0) - 5.0) > -1.0);

    ASSERT(psx::Time() - (psx::Time() -= 5.0) > 4.9);
    ASSERT((psx::Time() += 5.0) - psx::Time() > 4.9);

    ASSERT(psx::Time(2020, psx::Time::June, 28, 20, 57, 54, psx::Time::GMT) 
            == psx::Time(2020, psx::Time::June, 28, 8, psx::Time::PM, 57, 54, psx::Time::GMT));

    ASSERT(psx::Time(2020, psx::Time::June, 28, 8, 57, 54, psx::Time::GMT) 
            == psx::Time(2020, psx::Time::June, 28, 8, psx::Time::AM, 57, 54, psx::Time::GMT));

    ASSERT((psx::Time() + 0.001) != psx::Time());
    ASSERT((psx::Time() += 1.0) > psx::Time());
    ASSERT((psx::Time() -= 1.0) < psx::Time());
    ASSERT((psx::Time() += 1.0) >= psx::Time());
    ASSERT((psx::Time() -= 1.0) <= psx::Time());

    ASSERT((psx::Time() += -0.1) <= psx::Time());
    ASSERT((psx::Time() -= 0.1) <= psx::Time());

    ASSERT((psx::Time() += 0.1) >= psx::Time());
    ASSERT((psx::Time() -= -0.1) >= psx::Time());

    ASSERT(fabs(psx::Time().add(1.0, psx::Time::Seconds) 
                - (psx::Time() + 1.0)) 
                < 1.0);
    ASSERT(fabs(psx::Time().add(1.0, psx::Time::Minutes) 
                - (psx::Time() + 60.0)) 
                < 1.0);
    ASSERT(fabs(psx::Time().add(1.0, psx::Time::Hours) 
                - (psx::Time() + 3600.0)) 
                < 1.0);
    ASSERT(fabs(psx::Time().add(1.0, psx::Time::Days) 
                - (psx::Time() + 24.0 * 3600.0)) 
                < 1.0);
    ASSERT(fabs(psx::Time().add(1.0, psx::Time::Weeks) 
                - (psx::Time() + 7.0 * 24.0 * 3600.0)) 
                < 1.0);

    ASSERT(psx::Time::epoch(psx::Time::DOS) 
            == psx::Time(1980, psx::Time::Jan, 1, psx::Time::GMT));
    ASSERT(psx::Time::epoch(psx::Time::Mac) 
            == psx::Time(1904, psx::Time::Jan, 1, psx::Time::GMT));
    ASSERT(psx::Time::epoch(psx::Time::Cocoa) 
            == psx::Time(2001, psx::Time::Jan, 1, psx::Time::GMT));

    psx::Time t2, t3, t4;

    ASSERT(t1 < t2);
    ASSERT(t2 <= t3);
    ASSERT(t3 <= t4);

    ASSERT(t2 > t1);
    ASSERT(t3 >= t2);
    ASSERT(t4 >= t3);
    
END_TESTS

