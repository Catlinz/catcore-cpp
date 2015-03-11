#include "core/corelib.h"

#if defined (OS_WINDOWS)
#include "win32/file.cpp"
#endif

#if defined (OS_APPLE) || defined (OS_UNIX)
#include "unix/file.cpp"
#endif



