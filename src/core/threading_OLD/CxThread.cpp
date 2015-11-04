#include "core/Cx.h"

#if defined(CX_WINDOWS)
#  include "windows/CxWindowsThread.cpp"
#else
#  include "posix/CxPOSIXThread.cpp"
#endif





