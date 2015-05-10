#ifndef CX_CORE_COMMON_CXATOMIC_H
#define CX_CORE_COMMON_CXATOMIC_H

/**
 * @copyright Copyright Catlin Zilinski, 2015.  All rights reserved.
 *
 * @file CxAtomic.h
 * @brief Contains various atomic operations such as increment and decrement.
 *
 * @author Catlin Zilinski
 * @date May 10, 2015
 */

#include "core/Cx.h"

#if defined(CX_WINDOWS)
#  include "windows/CxWindowsAtomic.h"
#elif defined(CX_OSX)
#  include "apple/CxOSXAtomic.h"
#elif defined(CX_GNUC)
#  include "gnuc/CxGNUCAtomic.h"
#endif

#endif // CX_CORE_COMMON_CXATOMIC_H
