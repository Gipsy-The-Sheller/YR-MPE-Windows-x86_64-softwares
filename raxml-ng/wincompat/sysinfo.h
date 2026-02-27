// wincompat/sysinfo.h
#ifndef WINCOMPAT_SYSINFO_H
#define WINCOMPAT_SYSINFO_H

#ifdef _WIN32

struct sysinfo {
    unsigned long uptime;
    unsigned long loads[3];
    unsigned long totalram;
    unsigned long freeram;
    unsigned long sharedram;
    unsigned long bufferram;
    unsigned long totalswap;
    unsigned long freeswap;
    unsigned short procs;
    unsigned long totalhigh;
    unsigned long freehigh;
    unsigned int mem_unit;
    char _f[20-2*sizeof(long)-sizeof(int)];
};

int sysinfo(struct sysinfo *info);

#endif // _WIN32
#endif // WINCOMPAT_SYSINFO_H