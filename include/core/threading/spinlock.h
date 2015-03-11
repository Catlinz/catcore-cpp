#ifndef CAT_CORE_THREADING_SPINLOCK_H
#define CAT_CORE_THREADING_SPINLOCK_H
/**
 * Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * spinlock.h: Contains the definition of the Spinlock class for threading.
 *
 * Author: Catlin Zilinski
 * Date: July 23, 2013
 */

#include "core/corelib.h"

namespace Cat {

} // namespace Cat

#if defined (OS_WINDOWS)
#include "core/threading/win32/spinlock.h"
#elif defined (OS_APPLE)
#include "core/threading/osx/spinlock.h"
#else
#include "core/threading/unix/spinlock.h"
#endif

#endif // CAT_CORE_THREADING_SPINLOCK_H


