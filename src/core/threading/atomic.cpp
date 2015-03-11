#include "core/corelib.h"

#if defined (OS_WINDOWS)
#include "win32/atomic.cpp"
#elif defined(OS_APPLE)
#include "osx/atomic.cpp"
#else
#include "unix/atomic.cpp"
#endif
