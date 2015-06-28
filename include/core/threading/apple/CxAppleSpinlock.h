#ifndef CX_CORE_THREADING_APPLE_CXAPPLESPINLOCK_H
#define CX_CORE_THREADING_APPLE_CXAPPLESPINLOCK_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxAppleSpinlock.h
 * @brief Contains the Apple definition of the Spinlock class for threading.
 *
 * @author Catlin Zilinski
 * @date June 28, 2015
 */

#include "core/Cx.h"
#include <libkern/OSAtomic.h>

namespace cat {

	/**
	 * @class CxSpinlock CxAppleSpinlock.h "core/threading/apple/CxAppleSpinlock.h"
	 * A mutex that spins in place instead of getting a kernel level lock (expensive).
	 * @version 2
	 * @author Catlin Zilinski
	 * @since July 23, 2013
	 */
	class CxSpinlock {
	  public:
		/**  @brief Create a new CxSpinlock. */
		CxSpinlock();

		/**  @brief Lock the mutex. */
		CX_FORCE_INLINE void lock() { OSSpinLockLock(&m_spinlock); }

		/**
		 * @brief Try and lock the CxSpinlock, but don't wait.
		 * @return True if now owns the lock.
		 */
		CX_FORCE_INLINE CxBool tryLock() { return OSSpinLockTry(&m_spinlock); }
		
		/** @brief Unlock the CxSpinlock. */
		CX_FORCE_INLINE void unlock() { OSSpinLockUnlock(&m_spinlock); }

	  private:
		OSSpinLock	m_spinlock;

	};

} // namespace cat

#endif // CX_CORE_THREADING_APPLE_CXAPPLESPINLOCK_H
