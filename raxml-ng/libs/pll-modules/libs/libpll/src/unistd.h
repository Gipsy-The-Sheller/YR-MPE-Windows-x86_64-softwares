// wincompat/unistd.h
#ifndef WINCOMPAT_UNISTD_H
#define WINCOMPAT_UNISTD_H

#ifdef _WIN32

#include <io.h>
#include <process.h>
#include <direct.h>

#define F_OK 0
#define R_OK 4
#define W_OK 2
#define X_OK 1

#define S_ISDIR(mode) (((mode) & S_IFMT) == S_IFDIR)

#endif // _WIN32
#endif // WINCOMPAT_UNISTD_H