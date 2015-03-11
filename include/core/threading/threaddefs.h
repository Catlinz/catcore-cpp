#ifndef CAT_CORE_THREADING_THREAD_DEFS_H
#define CAT_CORE_THREADING_THREAD_DEFS_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file threaddefs.h
 * @brief Load in the proper thread defs.
 *
 * @author Catlin Zilinski
 * @date July 23, 2013
 */
#include "core/corelib.h"

#if defined (OS_WINDOWS)
#include "core/threading/win32/threaddefs.h"
#else //lif defined (OS_APPLE) || defined (OS_UNIX)
#include "core/threading/unix/threaddefs.h"
#endif

#endif // CAT_CORE_THREADING_THREAD_DEFS_H




