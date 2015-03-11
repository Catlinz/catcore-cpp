#include "core/time/abstime.h"

#if defined (OS_APPLE)
#include "osx/abstime.cpp"
#elif defined (OS_WINDOWS)
#include "win32/abstime.cpp"
#else
#include "unix/abstime.cpp"
#endif

