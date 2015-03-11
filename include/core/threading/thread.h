#ifndef CAT_CORE_THREADING_THREAD_H
#define CAT_CORE_THREADING_THREAD_H
/**
 * Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * thread.h: Contains the static Thread class that is responsible for creating 
 * new threads.
 *
 * Author: Catlin Zilinski
 * Date: Aug 7, 2013
 */

#include "core/corelib.h"

#if defined (OS_WINDOWS)
#include "core/threading/win32/thread.h"
#else //elif defined (OS_APPLE) || defined (OS_UNIX)
#include "core/threading/unix/thread.h"
#endif

#endif // CAT_CORE_THREADING_THREAD_H



