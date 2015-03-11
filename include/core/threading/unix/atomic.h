#ifndef CAT_CORE_THREADING_ATOMIC_H
#define CAT_CORE_THREADING_ATOMIC_H

/**
 * @copyright Copyright Catlin Zilinski, 2013.  All rights reserved.
 *
 * @file invasivestrongptr.h
 * @brief Contains a reference counting pointer wrapper.
 *
 * The InvasiveStrongPtr class is a strong reference counting safe pointer which assumes 
 * certain methods on the pointer it is storing to increment and decrement the 
 * reference count, which is stored on the actual object.
 *
 * @author Catlin Zilinski
 * @date: Mar 4, 2014
 */

#include <cstdlib>
#include "core/corelib.h"

namespace Cat {

	/**
	 * @class InvasiveStrongPtr invasivestrongptr.h "core/util/invasivestrongptr.h"
	 * @brief A strong reference counting safe pointer.
	 * 
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Mar 4, 2014
	 */
	template<typename T>
	class InvasiveStrongPtr {
	  public:		
