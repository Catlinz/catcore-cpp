#ifndef CX_CORE_THREADING_CXCONDITIONVARIABLE_H
#define CX_CORE_THREADING_CXCONDITIONVARIABLE_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxConditionVariable.h
 * @brief Contains the definition of the CxConditionVariable class for threading.
 *
 * @author Catlin Zilinski
 * @date: June 28, 2015
 */

#include "core/Cx.h"

#if defined(CX_WINDOWS)
#  include "windows/CxWindowsConditionVariable.h"
#else
#  include "posix/CxPOSIXConditionVariable.h"
#endif

#endif // CX_CORE_THREADING_CXCONDITIONVARIABLE_H


