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

#define CX_NANO_PER_SEC 1000000000
#define CX_NANO_PER_MILLI 1000000
#define CX_NANO_PER_MICRO 1000
#define CX_MICRO_PER_SEC 1000000
#define CX_MICRO_PER_MILLI 1000
#define CX_MILLI_PER_SEC 1000

#if defined (CX_WINDOWS)
#include "core/time/windows/CxWindowsTime.h"
#else //lif defined (CX_APPLE) || defined (CX_UNIX)
#include "core/time/unix/CxUnixTime.h"
#endif
#endif // CX_CORE_TIME_CXTIME_H
