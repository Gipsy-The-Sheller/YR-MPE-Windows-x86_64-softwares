@echo off
setlocal enabledelayedexpansion

echo 修复头文件包含顺序...

:: 修复 pll.h
set PLL_H="%~dp0libs\pll-modules\libs\libpll\src\pll.h"
if exist %PLL_H% (
    echo 修复 %PLL_H%
    powershell -Command "(Get-Content %PLL_H%) | ForEach-Object { $_ -replace '#ifdef HAVE_SYS_SYSINFO_H', '#ifdef _WIN32\n#include \"windows_compat.h\"\n#else\n#ifdef HAVE_SYS_SYSINFO_H' } | Set-Content temp.txt"
    powershell -Command "(Get-Content temp.txt) | ForEach-Object { $_ -replace '#endif', '#endif\n#endif' } | Set-Content %PLL_H%"
    del temp.txt
)

:: 修复 common.h
set COMMON_H="%~dp0src\common.h"
if exist %COMMON_H% (
    echo 修复 %COMMON_H%
    (
        echo #ifdef _WIN32
        echo #include \"windows_compat.h\"
        echo #else
        echo #include ^<unistd.h^>
        echo #include ^<sys/time.h^>
        echo #endif
    ) > temp.txt
    type %COMMON_H% | findstr /v "#include.*unistd.h" | findstr /v "#include.*sys/time.h" >> temp.txt
    move /y temp.txt %COMMON_H%
)

echo 修复完成
endlocal