#include "core/Cx.h"

#if defined(CX_WINDOWS)
#  include "windows/CxWindowsTime.cpp"
#else // elif defined(CX_UNIX) || defined(CX_APPLE)
#  include "unix/CxUnixTime.cpp"
#endif // CX_WINDOWS
