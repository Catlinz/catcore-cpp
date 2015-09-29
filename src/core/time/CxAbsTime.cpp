#include "core/Cx.h"

#if defined(CX_WINDOWS)
#  include "windows/CxWindowsAbsTime.cpp"
#elif defined(CX_APPLE)
#  include "apple/CxAppleAbsTime.cpp"
#else // elif defined(CX_UNIX)
#  include "unix/CxUnixAbsTime.cpp"
#endif // CX_WINDOWS

