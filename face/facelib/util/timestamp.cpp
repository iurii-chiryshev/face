#include "timestamp.h"

#include <windows.h>


void currentTimestamp(int64_t &seconds, int32_t &nanos)
{
     FILETIME ft;
     GetSystemTimeAsFileTime(&ft);
     UINT64 ticks = (((UINT64)ft.dwHighDateTime) << 32) | ft.dwLowDateTime;
     seconds = (int64_t) ((ticks / 10000000) - 11644473600LL);
     nanos = (int32_t) ((ticks % 10000000) * 100);
}
