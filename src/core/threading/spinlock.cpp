#include "core/corelib.h"

#if defined (OS_WINDOWS)
#include "win32/spinlock.cpp"
#elif defined(OS_APPLE)
#include "osx/spinlock.cpp"
#else
#include "unix/spinlock.cpp"
#endif

