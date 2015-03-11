#ifndef CAT_CORE_THREADING_WIN32_ATOMIC_H
#define CAT_CORE_THREADING_WIN32_ATOMIC_H

/**
 * @copyright Copyright Catlin Zilinski, 2014.  All rights reserved.
 *
 * @file atomic.h
 * @brief Contains various atomic types.
 *
 * @author Catlin Zilinski
 * @date Mar 3, 2015
 */

#include "core/corelib.h"
namespace Cat {

	/**
	 * @class AtomicI32 atomic.h "core/threading/atomic.h"
	 * @brief An Atomic Integer type.
	 * 
	 * @author Catlin Zilinski
	 * @version 1
	 * @since Mar 3, 2015
	 */
	class AtomicI32 {
	  public:
		AtomicI32() : m_val(0) {}
		AtomicI32(I32 val) : m_val(val) {}

		/**
		 * @return The value incrememented by 1.
		 */
		inline I32 increment() {
			return InterlockedIncrement(&m_val);
		}

		/**
		 * @brief The value decremented by 1.
		 */
		inline I32 decrement() {
			return InterlockedDecrement(&m_val);
		}

		/**
		 * @return The stored value.
		 */
		inline I32 val() const { return m_val; }		

	  private:
		I32 m_val;		
	};

	namespace Math {
	} // namespace Math

} // namespace Cat

#endif // CAT_CORE_THREADING_WIN32_ATOMIC_H
