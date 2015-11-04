#ifndef CX_CORE_THREADING_WINDOWS_CXWINDOWSSPINLOCK_H
#define CX_CORE_THREADING_WINDOWS_CXWINDOWSSPINLOCK_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxSpinlock.h
 * @brief Contains the  definition of the Spinlock class for threading.
 *
 * @author Catlin Zilinski
 * @date Nov 3, 2015
 */

#include "core/Cx.h"
#include <Windows.h>

#define CX_WINDOWS_SPINLOCK_TRIES 4000

namespace cat {

	/**
	 * @class CxSpinlock CxSpinlock.h "core/threading/CxSpinlock.h"
	 * A mutex that spins in place instead of getting a kernel level lock (expensive).
	 * @version 3
	 * @author Catlin Zilinski
	 * @since Mar 3, 2015
	 */
	class CxSpinlock {
	  public:

		/** @brief Create an uninitialised CxSpinlock. */
		CX_FORCE_INLINE CxSpinlock() : mp_spin(0) {}

		/** @brief Create and initialise a new mutex. */
		CX_FORCE_INLINE CxSpinlock(CxInitFlag) : mp_spin(0) {
			initialize();
		}

		/** @brief Move constructor */
		CX_FORCE_INLINE CxSpinlock(CxSpinlock &&in_src)
			: mp_spin(in_src.mp_spin) {
			in_src.mp_spin = 0;
		}

		/** @brief DO NOT PASS BY VALUE */
		CxSpinlock(const CxSpinlock &);

		/** @brief DO NOT PASS BY VALUE */
		CxSpinlock & operator=(const CxSpinlock &);

		/** @brief Move-assign operator. */
		CX_FORCE_INLINE CxSpinlock & operator=(CxSpinlock &&in_src) {
		   if (mp_spin != 0) { destroy(); }
			mp_spin = in_src.mp_spin;  in_src.mp_spin = 0;
			return *this;
		}

		/** @brief Destroy the CxSpinlock. */
		CX_FORCE_INLINE ~CxSpinlock() { destroy(); }

		/** @brief Destroy the spinlock. */
		void destroy();
		
		/** @brief Initialise the spinlock before the first use */
		void initialize();

		/** @brief Lock the spinlock. */
		CX_FORCE_INLINE void lock() { EnterCriticalSection(mp_spin); }

		/**
		 * @brief Try and lock the Spinlock, but don't wait.
		 * @return True if now owns the lock.
		 */
		CX_FORCE_INLINE CxBool tryLock() { return TryEnterCriticalSection(mp_spin); }

		/** @brief Unlock the Spinlock. */
		CX_FORCE_INLINE void unlock() { LeaveCriticalSection(mp_spin); }

	  private:
		
		CRITICAL_SECTION* mp_spin;
	};
} // namespace cat

#endif // CX_CORE_THREADING_WINDOWS_CXWINDOWSSPINLOCK_H


