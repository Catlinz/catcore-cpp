#ifndef CX_CORE_CX_H
#define CX_CORE_CX_H

/**
 * @copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file Cx.h
 * @brief Standard header to include.
 *
 * @author Catlin Zilinski
 * @date May 7, 2015
 */

/* These are used a lot, so just always include them */
#include <stdlib.h>
#include <string.h>

#include "core/CxTypes.h"
#include "core/CxCoreFunc.h"

#include "core/CxError.h"
#include "core/CxDebug.h"

namespace cat {

	/** @brief Enum flags for constructors + methods of objects. */
	enum CxInitFlag { kCxInit };
	enum CxNoLock { kCxNoLock };
	enum CxLock { kCxLock };
	

	/** @brief An enum that defines how data is copied (if at all) */
	enum CxCopy {
		kCxNoCopy = 0,
		kCxCopy = 1 << 0,
		kCxCopyOnRead = 1 << 1,
		kCxCopyOnWrite = 1 << 2
	};
	

} // namespace cat

#endif // CAT_CORE_CORELIB_H

