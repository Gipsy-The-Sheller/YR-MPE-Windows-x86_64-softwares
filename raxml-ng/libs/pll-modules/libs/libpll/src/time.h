// wincompat/time.h
#ifndef WINCOMPAT_TIME_H
#define WINCOMPAT_TIME_H

#ifdef _WIN32

#include <winsock2.h>  // 只包含 winsock2，不包含 winsock

struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};

int gettimeofday(struct timeval *tv, struct timezone *tz);

#else

#include_next <time.h>

#endif // _WIN32
#endif // WINCOMPAT_TIME_H