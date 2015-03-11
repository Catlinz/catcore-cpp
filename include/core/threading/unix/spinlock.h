#ifndef CAT_CORE_THREADING_UNIX_SPINLOCK_H
#define CAT_CORE_THREADING_UNIX_SPINLOCK_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file spinlock.h
 * @brief Contains the definition of the Spinlock for threading.
 *
 * @author Catlin Zilinski
 * @date: July 23, 2013
 */

#include "core/threading/unix/threaddefs.h"

namespace Cat {

	/**
	 * @class Spinlock spinlock.h "core/threading/mutext.h"
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
		 * @brief Destroy the Spinlock.
		 */
		~Spinlock();

		/**
		 * @brief Lock the mutex.
		 */
		inline void lock() {
			pthread_spin_lock(&m_spinlock);
		}

		/**
		 * @brief Try and lock the Spinlock, but don't wait.
		 * @return True if now owns the lock.
		 */
		inline Boolean trylock() {
			return (pthread_spin_trylock(&m_spinlock) == 0);
		}

		/**
		 * @brief Unlock the Spinlock.
		 */
		inline void unlock() {
			pthread_spin_unlock(&m_spinlock);
		}

	  private:
		pthread_spinlock_t	m_spinlock;
	};

} // namespace Cat

#endif // CAT_CORE_THREADING_UNIX_SPINLOCK_H

