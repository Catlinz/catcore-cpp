#ifndef CX_CORE_THREADING_CXTHREAD_H
#define CX_CORE_THREADING_CXTHREAD_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxThread.h
 * @brief Contains the static Thread class that is responsible for creating new threads.
 *
 * @author Catlin Zilinski
 * @date June 28, 2015
 */

#include "core/Cx.h"

#if defined(CX_WINDOWS)
#  include "windows/CxWindowsThread.h"
#else
#  include "posix/CxPOSIXThread.h"
#endif

#endif // CX_CORE_THREADING_CXTHREAD_H



