#include "core/Cx.h"

#if defined(CX_WINDOWS)
#  include "windows/CxWindowsFile.cpp"
#else // POSIX
#  include "posix/CxPOSIXFile.cpp"
#endif
