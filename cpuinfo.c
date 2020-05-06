#ifndef __linux__
#include <windows.h>
#else
#include <sys/sysinfo.h>
#endif // __linux__

#include "cpuinfo.h"

unsigned int hardware_concurrency()
{
#ifndef __linux__
    SYSTEM_INFO sysInfo;
    GetSystemInfo(&sysInfo);
    return sysInfo.dwNumberOfProcessors;
#else
    return get_nprocs();
#endif // __linux__
}