#include "core/corelib.h"

#if defined (OS_WINDOWS)
#include "win32/runnable.cpp"
#else //elif defined (OS_APPLE) || defined (OS_UNIX)
#include "unix/runnable.cpp"
#endif



