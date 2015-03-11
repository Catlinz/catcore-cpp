#include "core/corelib.h"

#if defined (OS_APPLE)
#include "osx/system.cpp"
#endif

#if defined (OS_WINDOWS)
#include "win32/system.cpp"
#endif

#if defined (OS_UNIX)
#include "unix/system.cpp"
#endif
