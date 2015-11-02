#include "core/Cx.h"

#if defined(CX_WINDOWS)
#  include "windows/CxWindowsSys.cpp"
#else // if POSIX
#  include "posix/CxPOSIXSys.cpp"
#endif
