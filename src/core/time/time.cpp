#include "core/time/time.h"

#if defined (OS_WINDOWS)
#include "win32/time.cpp"
#else
#include "unix/time.cpp"
#endif
