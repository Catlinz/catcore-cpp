#ifndef CX_CORE_THREADING_WINDOWS_CXWINDOWSSPINLOCK_H
#define CX_CORE_THREADING_WINDOWS_CXWINDOWSSPINLOCK_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxWindowsSpinlock.h
 * @brief Contains the Windows definition of the Spinlock class for threading.
 *
 * @author Catlin Zilinski
 * @date June 28, 2015
 */

#include "core/Cx.h"
#include <Windows.h>

#define CX_WINDOWS_SPINLOCK_TRIES 4000

namespace cat {

	/**
	 * @class CxWindowsSpinlock CxWindowsSpinlock.h "core/threading/windows/CxWindowsSpinlock.h"
	 * A mutex that spins in place instead of getting a kernel level lock (expensive).
	 * @version 2
	 * @author Catlin Zilinski
	 * @since Mar 3, 2015
	 */
	class CxSpinlock {
	  public:
		/**
		 * @brief Create a new Spinlock.
		 */
		CxSpinlock();

		/**
		 * @brief Copy constructor.
		 * @param in_src The CxSpinlock to copy.
		 */
		CxSpinlock(const CxSpinlock& in_src);

		/**
		 * @brief Destroy the CxSpinlock.
		 */
		~CxSpinlock();

		/**
		 * @brief Overloaded assignment operator.
		 * @param in_src The Spinlock to copy from.
		 * @return A reference to this CxSpinlock.
		 */
		CxSpinlock& operator=(const CxSpinlock& in_src);

		/** @brief Lock the spinlock. */
		CX_FORCE_INLINE void lock() { EnterCriticalSection(mp_spinlock); }

		/**
		 * @brief Try and lock the Spinlock, but don't wait.
		 * @return True if now owns the lock.
		 */
		CX_FORCE_INLINE CxBool tryLock() { return TryEnterCriticalSection(mp_spinlock); }

		/**
		 * @brief Unlock the Spinlock.
		 */
		CX_FORCE_INLINE void unlock() { LeaveCriticalSection(mp_spinlock); }

	  private:
		void tryDestroy();
		
		CRITICAL_SECTION* mp_spinlock;
		CxI32* mp_retainCount;
	};
} // namespace cat

#endif // CX_CORE_THREADING_WINDOWS_CXWINDOWSSPINLOCK_H


