#include "core/Cx.h"

#if defined(CX_WINDOWS)
#  include "windows/CxWindowsMutex.cpp"
#else
#  include "posix/CxPOSIXMutex.cpp"
#endif

