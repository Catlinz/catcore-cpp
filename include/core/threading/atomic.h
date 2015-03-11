#ifndef CAT_CORE_THREADING_ATOMIC_H
#define CAT_CORE_THREADING_ATOMIC_H
/**
 * @copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file atomic.h
 * @brief  Contains multiple atomic types.
 *
 * @author Catlin Zilinski
 * @date Mar, 15, 2014
 */

#include "core/corelib.h"

#if defined (OS_WINDOWS)
#include "core/threading/win32/atomic.h"
#elif defined (OS_APPLE)
#include "core/threading/osx/atomic.h"
#else
#include "core/threading/unix/atomic.h"
#endif

#endif // CAT_CORE_THREADING_ATOMIC_H


