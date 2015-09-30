#include "core/Cx.h"

#if defined(CX_WINDOWS)
#  include "windows/CxWindowsTime.cpp"
#elif defined(CX_APPLE)
#  include "apple/CxAppleTime.cpp"
#else // CX_UNIX
#  include "unix/CxUnixTime.cpp"
#endif // CX_WINDOWS

