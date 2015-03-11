#ifndef CAT_CORE_THREADING_RUNNABLE_H
#define CAT_CORE_THREADING_RUNNABLE_H
/**
 * Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * runnable.h: Contains the definition of the IRunnable interface and the 
 * RunnableFunc class.
 *
 * Author: Catlin Zilinski
 * Date: Aug 7, 2013
 */

#include "core/corelib.h"

#ifdef OS_WINDOWS
#include "core/threading/win32/runnable.h"
#else //elif defined(OS_APPLE) || defined(OS_UNIX)
#include "core/threading/unix/runnable.h"
#endif

#endif // CAT_CORE_THREADING_RUNNABLE_H


