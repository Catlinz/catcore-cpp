#ifndef CX_CORE_THREADING_CXSPINLOCK_H
#define CX_CORE_THREADING_CXSPINLOCK_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxSpinLock.h 
 * @brief Contains the definition of the Spinlock class for threading.
 *
 * @author Catlin Zilinski
 * @date June 28, 2015
 */

#include "core/Cx.h"

#if defined(CX_WINDOWS)
#  include "windows/CxWindowsSpinlock.h"
#elif defined(CX_APPLE)
#  include "apple/CxAppleSpinlock.h"
#else
#  include "posix/CxPOSIXSpinlock.h"
#endif

#endif // CX_CORE_THREADING_CXSPINLOCK_H


