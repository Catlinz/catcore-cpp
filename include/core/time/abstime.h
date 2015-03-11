#ifndef CAT_CORE_TIME_ABSTIME_H
#define CAT_CORE_TIME_ABSTIME_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file abstime.h
 * @brief Contains the AbsTime value and static class for POSITIVE time values.
 *
 * @author Catlin Zilinski
 * @date Mar 7, 2015
 */
#include "core/corelib.h"

#if defined (OS_WINDOWS)
#include "core/time/win32/abstime.h"
#elif defined (OS_APPLE)
#include "core/time/osx/abstime.h"
#else
#include "core/time/unix/abstime.h"
#endif
#endif // CAT_CORE_TIME_ABSTIME_H
