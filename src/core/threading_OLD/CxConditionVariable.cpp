#include "core/Cx.h"

#if defined(CX_WINDOWS)
#  include "windows/CxWindowsConditionVariable.cpp"
#else
#  include "posix/CxPOSIXConditionVariable.cpp"
#endif

