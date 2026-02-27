// windows_compat_final.h
#ifndef WINDOWS_COMPAT_FINAL_H
#define WINDOWS_COMPAT_FINAL_H

#ifdef _WIN32

// 避免包含任何 Windows 头文件，使用手动声明
// 手动声明 Windows API 函数
#ifdef __cplusplus
extern "C" {
#endif

// 必要的 Windows 类型定义
typedef void* HANDLE;
typedef unsigned long DWORD;
typedef unsigned long long ULONGLONG;
typedef unsigned long long ULONG64;
typedef long LONG;
typedef unsigned long ULONG;
typedef int BOOL;
typedef unsigned short WORD;
typedef unsigned char BYTE;

// Windows 结构体
typedef struct _FILETIME {
    DWORD dwLowDateTime;
    DWORD dwHighDateTime;
} FILETIME;

typedef struct _SYSTEMTIME {
    WORD wYear;
    WORD wMonth;
    WORD wDayOfWeek;
    WORD wDay;
    WORD wHour;
    WORD wMinute;
    WORD wSecond;
    WORD wMilliseconds;
} SYSTEMTIME;

typedef struct _MEMORYSTATUSEX {
    DWORD dwLength;
    DWORD dwMemoryLoad;
    ULONG64 ullTotalPhys;
    ULONG64 ullAvailPhys;
    ULONG64 ullTotalPageFile;
    ULONG64 ullAvailPageFile;
    ULONG64 ullTotalVirtual;
    ULONG64 ullAvailVirtual;
    ULONG64 ullAvailExtendedVirtual;
} MEMORYSTATUSEX, *LPMEMORYSTATUSEX;

// Windows API 函数声明
void __stdcall GetSystemTimeAsFileTime(FILETIME*);
BOOL __stdcall GlobalMemoryStatusEx(MEMORYSTATUSEX*);
DWORD __stdcall GetTickCount(void);
void __stdcall GetSystemInfo(void*);

#ifdef __cplusplus
}
#endif

// 标准 C 头文件
#include <io.h>
#include <process.h>
#include <direct.h>
#include <stdlib.h>
#include <string.h>
#include <stdio.h>

// POSIX 兼容函数
#define access _access
#define F_OK 0
#define R_OK 4
#define W_OK 2
#define X_OK 1

#define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)

// 自定义 timeval 结构体
struct timeval {
    long tv_sec;
    long tv_usec;
};

struct timezone {
    int tz_minuteswest;
    int tz_dsttime;
};

// 使用 QueryPerformanceCounter 实现高精度计时
static int gettimeofday(struct timeval *tv, struct timezone *tz) {
    static LARGE_INTEGER frequency = {0};
    static LARGE_INTEGER start_count = {0};
    
    if (tv) {
        if (frequency.QuadPart == 0) {
            // 第一次调用，初始化频率
            QueryPerformanceFrequency(&frequency);
            QueryPerformanceCounter(&start_count);
        }
        
        LARGE_INTEGER count;
        QueryPerformanceCounter(&count);
        
        // 计算经过的微秒数
        ULONGLONG elapsed = (count.QuadPart - start_count.QuadPart) * 1000000ULL / frequency.QuadPart;
        
        tv->tv_sec = (long)(elapsed / 1000000ULL);
        tv->tv_usec = (long)(elapsed % 1000000ULL);
    }
    
    if (tz) {
        // 简单实现，返回 UTC
        tz->tz_minuteswest = 0;
        tz->tz_dsttime = 0;
    }
    
    return 0;
}

// sysinfo 结构体
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

// 简化的 sysinfo 实现
static int sysinfo(struct sysinfo *info) {
    if (!info) return -1;
    
    // 使用 Windows API 获取内存信息
    MEMORYSTATUSEX memStatus;
    memStatus.dwLength = sizeof(memStatus);
    
    // 动态加载函数
    HMODULE hKernel = GetModuleHandleA("kernel32.dll");
    if (hKernel) {
        typedef BOOL (WINAPI *GlobalMemoryStatusExFunc)(LPMEMORYSTATUSEX);
        GlobalMemoryStatusExFunc pGlobalMemoryStatusEx = 
            (GlobalMemoryStatusExFunc)GetProcAddress(hKernel, "GlobalMemoryStatusEx");
        
        if (pGlobalMemoryStatusEx && pGlobalMemoryStatusEx(&memStatus)) {
            memset(info, 0, sizeof(struct sysinfo));
            info->uptime = GetTickCount() / 1000;
            info->totalram = (unsigned long)memStatus.ullTotalPhys;
            info->freeram = (unsigned long)memStatus.ullAvailPhys;
            info->totalswap = (unsigned long)memStatus.ullTotalPageFile;
            info->freeswap = (unsigned long)memStatus.ullAvailPageFile;
            info->mem_unit = 1;
            return 0;
        }
    }
    
    return -1;
}

// 线程局部存储
#ifdef _MSC_VER
    #define PLL_THREAD_LOCAL __declspec(thread)
#else
    #define PLL_THREAD_LOCAL __thread
#endif

// 字符串函数
#ifdef _MSC_VER
    #define strdup _strdup
    #define snprintf _snprintf
    #define strcasecmp _stricmp
    #define strncasecmp _strnicmp
#endif

// 缺少的 Windows 类型定义
typedef struct _LARGE_INTEGER {
    union {
        struct {
            DWORD LowPart;
            LONG HighPart;
        };
        struct {
            DWORD LowPart;
            LONG HighPart;
        } u;
        long long QuadPart;
    };
} LARGE_INTEGER;

// 动态加载的性能计数器函数
BOOL WINAPI QueryPerformanceFrequency(LARGE_INTEGER*);
BOOL WINAPI QueryPerformanceCounter(LARGE_INTEGER*);

#endif // _WIN32
#endif // WINDOWS_COMPAT_FINAL_H