#ifndef CAT_CORE_THREADING_CONDITION_VARIABLE_H
#define CAT_CORE_THREADING_CONDITION_VARIABLE_H
/**
 * Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * conditionvariable.h: Contains the definition of the ConditionVariable class for threading.
 *
 * Author: Catlin Zilinski
 * Date: July 22, 2013
 */

#include "core/corelib.h"

#ifdef OS_WINDOWS
#include "core/threading/win32/conditionvariable.h"
#else // elif defined(OS_APPLE) || defined(OS_UNIX)
#include "core/threading/unix/conditionvariable.h"
#endif

#endif // CAT_CORE_THREADING_MUTEX_H


