#ifndef CX_CORE_TIME_CXTIME_H
#define CX_CORE_TIME_CXTIME_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxTime.h
 * @brief Contains the CxTime values and static methods.
 *
 * @author Catlin Zilinski
 * @date Sept 29, 2015
 */
#include "core/Cx.h"

#if defined (CX_WINDOWS)
#  include "core/time/windows/CxWindowsTime.h"
#else //elif defined (CX_APPLE) || defined (CX_UNIX)
#  include "core/time/unix/CxUnixTime.h"
#endif // CX_WINDOWS

#endif // CX_CORE_TIME_CXTIME_H
