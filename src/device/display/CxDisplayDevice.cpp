#include "core/Cx.h"

#if defined(CX_WINDOWS)
#  include "windows/CxWindowsDisplayDevice.cpp"
#elif defined(CX_APPLE)
#  include "apple/CxAppleDisplayDevice.cpp"
#else
#  include "x11/CxX11DisplayDevice.cpp"
#endif
