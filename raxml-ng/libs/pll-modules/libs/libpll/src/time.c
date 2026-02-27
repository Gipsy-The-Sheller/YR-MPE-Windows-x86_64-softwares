// wincompat/time.c
#ifdef _WIN32

#include "time.h"
#include <windows.h>

int gettimeofday(struct timeval *tv, struct timezone *tz) {
    if (tv) {
        FILETIME ft;
        ULARGE_INTEGER uli;
        GetSystemTimeAsFileTime(&ft);
        uli.LowPart = ft.dwLowDateTime;
        uli.HighPart = ft.dwHighDateTime;
        uli.QuadPart -= 116444736000000000ULL;
        
        tv->tv_sec = (long)(uli.QuadPart / 10000000L);
        tv->tv_usec = (long)((uli.QuadPart % 10000000L) / 10L);
    }
    
    if (tz) {
        tz->tz_minuteswest = 0;
        tz->tz_dsttime = 0;
    }
    
    return 0;
}

#endif // _WIN32