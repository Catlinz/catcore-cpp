#include "core/Cx.h"

#if defined(CX_WINDOWS)
#  include "windows/CxWindowsSpinlock.cpp"
#elif defined(CX_APPLE)
#  include "apple/CxAppleSpinlock.cpp"
#else
#  include "posix/CxPOSIXSpinlock.cpp"
#endif


