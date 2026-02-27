// windows_compat.h
#ifndef WINDOWS_COMPAT_H
#define WINDOWS_COMPAT_H

#ifdef _WIN32
#include <io.h>
#include <process.h>
#include <direct.h>
#include <windows.h>

// 定义 POSIX 兼容函数
#define access _access
#define F_OK 0
#define R_OK 4
#define W_OK 2
#define X_OK 1

#define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)

// 替换 gettimeofday
#ifdef _MSC_VER
#include <winsock2.h>

struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};

static int gettimeofday(struct timeval *tv, struct timezone *tz) {
    FILETIME ft;
    ULARGE_INTEGER uli;
    static int tzflag = 0;
    
    if (tv) {
        GetSystemTimeAsFileTime(&ft);
        uli.LowPart = ft.dwLowDateTime;
        uli.HighPart = ft.dwHighDateTime;
        uli.QuadPart -= 116444736000000000ULL;
        
        tv->tv_sec = (long)(uli.QuadPart / 10000000L);
        tv->tv_usec = (long)((uli.QuadPart % 10000000L) / 10L);
    }
    
    if (tz) {
        if (!tzflag) {
            _tzset();
            tzflag = 1;
        }
        tz->tz_minuteswest = _timezone / 60;
        tz->tz_dsttime = _daylight;
    }
    
    return 0;
}
#endif

// 定义 Unix 常量
#ifndef S_IRUSR
#define S_IRUSR _S_IREAD
#define S_IWUSR _S_IWRITE
#define S_IXUSR 0
#define S_IRGRP 0
#define S_IWGRP 0
#define S_IXGRP 0
#define S_IROTH 0
#define S_IWOTH 0
#define S_IXOTH 0
#endif

// sys/sysinfo.h 替代
struct sysinfo {
    unsigned long uptime;    /* 系统启动时间（秒） */
    unsigned long loads[3];  /* 1, 5, 15 分钟的平均负载 */
    unsigned long totalram;  /* 总内存大小 */
    unsigned long freeram;   /* 可用内存 */
    unsigned long sharedram; /* 共享内存 */
    unsigned long bufferram; /* 缓冲区内存 */
    unsigned long totalswap; /* 交换区总大小 */
    unsigned long freeswap;  /* 可用交换区 */
    unsigned short procs;    /* 进程数 */
    unsigned long totalhigh; /* 高内存区总大小 */
    unsigned long freehigh;  /* 高内存区可用大小 */
    unsigned int mem_unit;   /* 内存单元大小（字节） */
    char _f[20-2*sizeof(long)-sizeof(int)]; /* 填充 */
};

static int sysinfo(struct sysinfo *info) {
    if (!info) return -1;
    
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    GlobalMemoryStatusEx(&memStatus);
    
    // 获取系统运行时间
    DWORD uptime = GetTickCount();
    
    memset(info, 0, sizeof(struct sysinfo));
    info->uptime = uptime / 1000;  // 转换为秒
    info->totalram = memStatus.ullTotalPhys;
    info->freeram = memStatus.ullAvailPhys;
    info->totalswap = memStatus.ullTotalPageFile;
    info->freeswap = memStatus.ullAvailPageFile;
    info->mem_unit = 1;
    
    return 0;
}

// 线程局部存储宏
#ifdef _MSC_VER
    #define PLL_THREAD_LOCAL __declspec(thread)
#else
    #define PLL_THREAD_LOCAL __thread
#endif

// 其他缺失的函数
#ifdef _MSC_VER
    #define strdup _strdup
    #define snprintf _snprintf
    #define strcasecmp _stricmp
    #define strncasecmp _strnicmp
#endif

#endif // _WIN32
#endif // WINDOWS_COMPAT_H