#ifndef CAT_CORE_THREADING_OSX_SPINLOCK_H
#define CAT_CORE_THREADING_OSX_SPINLOCK_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file spinlock.h
 * @brief Contains the definition of the Spinlock class for threading.
 *
 * @author Catlin Zilinski
 * @date Jul 23, 2013
 */

#include <libkern/OSAtomic.h>
#include "core/corelib.h"

namespace Cat {

	/**
	 * @class Spinlock spinlock.h "core/threading/mutext.h"
	 * A mutex that spins in place instead of getting a kernel level
	 * lock (expensive).
	 * @version 1
	 * @author Catlin Zilinski
	 * @since July 23, 2013
	 */
	class Spinlock {
	  public:
		/**
		 * @brief Create a new Spinlock.
		 */
		Spinlock();
		/**
		 * @brief Destroy the Spinlock.
		 */
		~Spinlock();

		/**
		 * @brief Lock the mutex.
		 */
		inline void lock() {
			OSSpinLockLock(&m_spinlock);
		}

		/**
		 * @brief Try and lock the Spinlock, but don't wait.
		 * @return True if now owns the lock.
		 */
		inline Boolean tryLock() {
			return OSSpinLockTry(&m_spinlock);
		}
		
		/**
		 * @brief Unlock the Spinlock.
		 */
		inline void unlock() {
			OSSpinLockUnlock(&m_spinlock);
		}
	  private:
		OSSpinLock	m_spinlock;

	};

} // namespace Cat

#endif // CAT_CORE_THREADING_OSX_SPINLOCK_H
