#ifndef CX_CORE_THREADING_POSIX_CXPOSIXSPINLOCK_H
#define CX_CORE_THREADING_POSIX_CXPOSIXSPINLOCK_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxPOSIXSpinlock.h
 * @brief Contains the definition of the POSIX Spinlock for threading.
 *
 * @author Catlin Zilinski
 * @date: June 28, 2015
 */

#include "core/Cx.h"
#include <pthread.h>

namespace cat {

	/**
	 * @class CxSpinlock CxPOSIXSpinlock.h "core/threading/posix/CxPOSIXSpinlock.h"
	 * A mutex that spins in place instead of getting a kernel level lock (expensive).
	 * @version 2
	 * @author Catlin Zilinski
	 * @since Mar 3, 2015
	 */
	class CxSpinlock {
	  public:

		/** @brief Create a new CxSpinlock. */
		CxSpinlock();

		/**
		 * @brief Copy constructor, handles reference counting. 
		 * @param in_src The spinlock to copy.
		 */
		CxSpinlock(const CxSpinlock& in_src);

		/** @brief Destroy the CxSpinlock. */
		~CxSpinlock();

		/**
		 * @brief Overloaded assignment operator to handle reference counting.
		 * @param in_src The spinlock to copy.
		 */
		CxSpinlock& operator=(const CxSpinlock& in_src);

		/**  @brief Lock the mutex. */
		CX_FORCE_INLINE void lock() { pthread_spin_lock(&m_spinlock); }

		/**
		 * @brief Try and lock the CxSpinlock, but don't wait.
		 * @return True if now owns the lock.
		 */
		CX_FORCE_INLINE CxBool trylock() { return (pthread_spin_trylock(&m_spinlock) == 0); }

		/** @brief Unlock the CxSpinlock. */
		CX_FORCE_INLINE void unlock() { pthread_spin_unlock(&m_spinlock); }

	  private:
		pthread_spinlock_t m_spinlock;
		CxI32* mp_refCount;

		void tryDestroy();
				
	};

} // namespace cat

#endif // CX_CORE_THREADING_POSIX_CXPOSIXSPINLOCK_H

