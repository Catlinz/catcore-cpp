#ifndef CX_CORE_THREADING_CXMUTEX_H
#define CX_CORE_THREADING_CXMUTEX_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxMutex.h
 * @brief Contains the definition of the CxMutex class for threading.
 *
 * @author Catlin Zilinski
 * @date June 23, 2015
 */


#include "core/Cx.h"

#if defined(CX_WINDOWS)
#  include "windows/CxWindowsMutex.h"
#else
#  include "posix/CxPOSIXMutex.h"
#endif

#endif // CX_CORE_THREADING_CXMUTEX_H


