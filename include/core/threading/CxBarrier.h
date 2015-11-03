#ifndef CX_CORE_THREADING_CXBARRIER_H
#define CX_CORE_THREADING_CXBARRIER_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxBarrier.h 
 * @brief Contains the definition of the Barrier class for threading.
 *
 * @author Catlin Zilinski
 * @date Nov 3, 2015
 */

#include "core/Cx.h"

#if defined(CX_WINDOWS)
#  include "windows/CxWindowsBarrier.h"
#else // POSIX + APPLE
#  include "posix/CxPOSIXBarrier.h"
#endif

#endif // CX_CORE_THREADING_CXBARRIER_H


