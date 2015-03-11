#include "core/corelib.h"

#if defined (OS_WINDOWS)
#include "win32/threaddefs.cpp"
#else // elif defined (OS_APPLE) || defined (OS_UNIX)
#include "unix/threaddefs.cpp"
#endif



