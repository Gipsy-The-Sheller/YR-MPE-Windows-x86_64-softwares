// wincompat/sysinfo.c
#ifdef _WIN32

#include "sysinfo.h"
#include <windows.h>

int sysinfo(struct sysinfo *info) {
    if (!info) return -1;
    
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    GlobalMemoryStatusEx(&memStatus);
    
    memset(info, 0, sizeof(struct sysinfo));
    info->uptime = GetTickCount() / 1000;
    info->totalram = (unsigned long)memStatus.ullTotalPhys;
    info->freeram = (unsigned long)memStatus.ullAvailPhys;
    info->totalswap = (unsigned long)memStatus.ullTotalPageFile;
    info->freeswap = (unsigned long)memStatus.ullAvailPageFile;
    info->mem_unit = 1;
    
    return 0;
}

#endif // _WIN32