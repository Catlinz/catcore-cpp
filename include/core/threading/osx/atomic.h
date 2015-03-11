#ifndef CAT_CORE_THREADING_OSX_ATOMIC_H
#define CAT_CORE_THREADING_OSX_ATOMIC_H

/**
 * @copyright Copyright Catlin Zilinski, 2014.  All rights reserved.
 *
 * @file atomic.h
 * @brief Contains various atomic types.
 *
 * @author Catlin Zilinski
 * @date Mar 15, 2014
 */

#include "core/corelib.h"
#include <libkern/OSAtomic.h>

namespace Cat {

	/**
	 * @class AtomicI32 atomic.h "core/threading/atomic.h"
	 * @brief An Atomic Integer type.
	 * 
	 * @author Catlin Zilinski
	 * @version 2
	 * @since Mar 15, 2014
	 */
	class AtomicI32 {
	  public:
		AtomicI32() : m_val(0) {}
		AtomicI32(I32 val) : m_val(val) {}

		/**
		 * @return The value incrememented by 1.
		 */
		inline I32 increment() {
			return OSAtomicIncrement32(&m_val);
		}

		/**
		 * @brief The value decremented by 1.
		 */
		inline I32 decrement() {
			return OSAtomicDecrement32(&m_val);
		}

		/**
		 * @return The stored value.
		 */
		inline I32 val() const { return m_val; }		

	  private:
		I32 m_val;		
	};

} // namespace Cat

#endif // CAT_CORE_THREADING_OSX_ATOMIC_H
