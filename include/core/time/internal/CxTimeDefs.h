#ifndef CX_CORE_TIME_INTERNAL_CXTIMEDEFS_H
#define CX_CORE_TIME_INTERNAL_CXTIMEDEFS_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxTimeDefs.h
 * @brief Common header file for time classes.
 *
 * @author Catlin Zilinski
 * @date Sept 29, 2015
 */

#include "core/Cx.h"

#if defined(CX_APPLE)
#  include <mach/mach.h>
#  include <mach/mach_time.h>
#endif // CX_APPLE

#if defined(CX_WINDOWS)
#  include <Windows.h>
#endif // CX_WINDOWS

#include <time.h>

#if defined(CX_APPLE) || defined(CX_UNIX)
#  if !defined (CLOCK_MONOTONIC) && defined(CLOCK_REALTIME)
#    define CLOCK_MONOTONIC CLOCK_REALTIME
#  endif
#endif // CX_APPLE || CX_UNIX

#define CX_NANO_PER_SEC 1000000000
#define CX_NANO_PER_MILLI 1000000
#define CX_NANO_PER_MICRO 1000
#define CX_MICRO_PER_SEC 1000000
#define CX_MICRO_PER_MILLI 1000
#define CX_MILLI_PER_SEC 1000

#define CX_SEC_PER_NANO 1e-9
#define CX_SEC_PER_MILLI 1e-6
#define CX_SEC_PER_MICRO 1e-3

#endif // CX_CORE_TIME_INTERNAL_CXTIMEDEFS_H


