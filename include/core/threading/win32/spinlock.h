#ifndef CAT_CORE_THREADING_WIN32_SPINLOCK_H
#define CAT_CORE_THREADING_WIN32_SPINLOCK_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file spinlock.h
 * @brief Contains the definition of the Spinlock class for threading.
 *
 * @author Catlin Zilinski
 * @date Mar 3, 2015
 */

#include "core/threading/atomic.h"

#define CAT_SPINLOCK_TRIES 4000

namespace Cat {

	/**
	 * @class Spinlock spinlock.h "core/threading/spinlock.h"
	 * A mutex that spins in place instead of getting a kernel level
	 * lock (expensive).
	 * @version 1
	 * @author Catlin Zilinski
	 * @since Mar 3, 2015
	 */
	class Spinlock {
	  public:
		/**
		 * @brief Create a new Spinlock.
		 */
		Spinlock();

		/**
		 * @brief Copy constructor.
		 * @param p_src The Spinlock to copy.
		 */
		Spinlock(const Spinlock& p_src);

		/**
		 * @brief Destroy the Spinlock.
		 */
		~Spinlock();

		/**
		 * @brief Overloaded assignment operator.
		 * @param p_src The Spinlock to copy from.
		 * @return A reference to this Spinlock.
		 */
		Spinlock& operator=(const Spinlock& p_src);

		/**
		 * @brief Lock the mutex.
		 */
		inline void lock() {
			EnterCriticalSection(m_pSpinlock);
		}

		/**
		 * @brief Try and lock the Spinlock, but don't wait.
		 * @return True if now owns the lock.
		 */
		inline Boolean tryLock() {
			return TryEnterCriticalSection(m_pSpinlock);
		}

		/**
		 * @brief Unlock the Spinlock.
		 */
		inline void unlock() {
			LeaveCriticalSection(m_pSpinlock);
		}

	  private:
		void tryDestroy();
		
		CRITICAL_SECTION *m_pSpinlock;
		AtomicI32 *m_pRetainCount;
	};
} // namespace Cat

#endif // CAT_CORE_THREADING_WIN32_SPINLOCK_H


