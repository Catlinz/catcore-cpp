#ifndef CAT_CORE_TIME_TIME_H
#define CAT_CORE_TIME_TIME_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file time.h
 * @brief Contains the Time value and static class.
 *
 * @author Catlin Zilinski
 * @date Mar 7, 2015
 */
#include "core/corelib.h"

#if defined (OS_WINDOWS)
#include "core/time/win32/time.h"
#else //lif defined (OS_APPLE) || defined (OS_UNIX)
#include "core/time/unix/time.h"
#endif
#endif // CAT_CORE_TIME_TIME_H
