#include "core/Cx.h"

#if defined(CX_WINDOWS)
#  include "windows/CxWindowsBarrier.cpp"
#else // POSIX + APPLE
#  include "posix/CxPOSIXBarrier.cpp"
#endif

