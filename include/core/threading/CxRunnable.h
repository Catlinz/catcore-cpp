#ifndef CX_CORE_THREADING_CXRUNNABLE_H
#define CX_CORE_THREADING_CXRUNNABLE_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxRunnable.h
 * @brief Contains the definition of the CxRunnable interface and the CxRunnableFunc class.
 *
 * @author Catlin Zilinski
 * @date June 28, 2015
 */

#include "core/Cx.h"

#if defined(CX_WINDOWS)
#  include "windows/CxWindowsRunnable.h"
#else
#  include "posix/CxPOSIXRunnable.h"
#endif

#endif // CX_CORE_THREADING_CXRUNNABLE_H


