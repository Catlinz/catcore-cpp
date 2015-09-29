#ifndef CX_CORE_TIME_CXABSTIME_H
#define CX_CORE_TIME_CXABSTIME_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxAbsTime.h
 * @brief Contains the CxAbsTime values and static methods.
 *
 * @author Catlin Zilinski
 * @date Sept 29, 2015
 */
#include "core/Cx.h"

#if defined (CX_WINDOWS)
#  include "core/time/windows/CxWindowsAbsTime.h"
#elif defined (CX_APPLE)
#  include "core/time/apple/CxAppleAbsTime.h"
#else
#  include "core/time/unix/CxUnixAbsTime.h"
#endif // CX_WINDOWS

#endif // CX_CORE_TIME_CXABSTIME_H
