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
		enum FlagsEnum { kSpinlockStatic = 1 << 0, kSpinlockInitialized = 1 << 1 };

		/** @brief Flags to pass to constructor to indicate behaviour. */
		enum StaticEnum { kStatic = 1 << 0 };
		enum InitializeEnum { kInitialize = 1 << 0 };

		
		/** @brief Create an uninitialised CxSpinlock. */
		CX_FORCE_INLINE CxSpinlock() : mp_spinlock(0), m_flags(0) {}

		/** @brief Create and initialise a new spinlock. */
		CX_FORCE_INLINE CxSpinlock(CxSpinlock::InitializeEnum)
			: mp_spinlock(0), m_flags(0) { initialize(); }

		/** 
		 * @brief Create and initialise a new static spinlock. 
		 * A static spinlock will assume that it is never copied (and should not be), 
		 * and as such will destroy itself when the destructor is called. 
		 */
		CX_FORCE_INLINE CxSpinlock(CxSpinlock::StaticEnum)
			: mp_spinlock(0), m_flags(kSpinlockStatic) { initialize(); }

		/** @brief Copy constructor (mainly does debug checking for copying. */
		CX_FORCE_INLINE CxSpinlock(const CxSpinlock &in_src)
			: mp_spinlock(in_src.mp_spinlock), m_flags(in_src.m_flags) {
			CXD_IF_ERR(((m_flags & kSpinlockStatic) != 0), "DO NOT COPY STATIC MUTEXES FFS!");
		}

		/** @brief Destroy the CxSpinlock if it is static. */
		CX_FORCE_INLINE ~CxSpinlock() {
			if ((m_flags & kSpinlockStatic) != 0) { destroy(); }
		}

		/** @brief See copy constructor */
		CxSpinlock & operator=(const CxSpinlock &in_src);

		/** @brief Destroy the spinlock. */
		void destroy();
		
		/** @brief Initialise the spinlock before the first use */
		void initialize();

		/** @brief Lock the spinlock. */
		CX_FORCE_INLINE void lock() { EnterCriticalSection(mp_spinlock); }

		/**
		 * @brief Try and lock the Spinlock, but don't wait.
		 * @return True if now owns the lock.
		 */
		CX_FORCE_INLINE CxBool tryLock() { return TryEnterCriticalSection(mp_spinlock) != 0; }

		/** @brief Unlock the Spinlock. */
		CX_FORCE_INLINE void unlock() { LeaveCriticalSection(mp_spinlock); }

	  private:
		
		CRITICAL_SECTION* mp_spinlock;
		CxI32 m_flags;
	};
} // namespace cat

#endif // CX_CORE_THREADING_WINDOWS_CXWINDOWSSPINLOCK_H


