#ifndef CAT_CORE_THREADING_MUTEX_H
#define CAT_CORE_THREADING_MUTEX_H
/**
 * Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * mutex.h: Contains the definition of the Mutex class for threading.
 *
 * Author: Catlin Zilinski
 * Date: July 22, 2013
 */

#include "core/corelib.h"

#ifdef OS_WINDOWS
#include "core/threading/win32/mutex.h"
#else //elif defined(OS_APPLE) || defined(OS_UNIX)
#include "core/threading/unix/mutex.h"
#endif

#endif // CAT_CORE_THREADING_MUTEX_H


