#include "core/corelib.h"

#if defined (OS_WINDOWS)
#include "win32/conditionvariable.cpp"
#else //elif defined (OS_APPLE) || defined (OS_UNIX)
#include "unix/conditionvariable.cpp"
#endif



