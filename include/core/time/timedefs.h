#ifndef CAT_CORE_TIME_TIMEDEFS_H
#define CAT_CORE_TIME_TIMEDEFS_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file timedefs.h
 * @brief Common header file to timey wimey stuff.
 *
 * @author Catlin Zilinski
 * @date Mar 7, 2015
 */

#include "core/corelib.h"

#if defined (OS_APPLE)
#include <mach/mach.h>
#include <mach/mach_time.h>
#elif defined(OS_WINDOWS)
#include <somthing>
#else
#include <time.h>
#endif

#define NANO_PER_SEC 1000000000
#define NANO_PER_MILLI 1000000
#define NANO_PER_MICRO 1000
#define MICRO_PER_SEC 1000000
#define MICRO_PER_MILLI 1000
#define MILLI_PER_SEC 1000

#endif // CAT_CORE_TIME_TIMEDEFS
