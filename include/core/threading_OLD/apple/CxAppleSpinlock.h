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
		enum FlagsEnum { kSpinlockStatic = 1 << 0, kSpinlockInitialized = 1 << 1 };

		/** @brief Flags to pass to constructor to indicate behaviour. */
		enum StaticEnum { kStatic = 1 << 0 };
		enum InitializeEnum { kInitialize = 1 << 0 };

		/** @brief Create a CxSpinlock. */
		CX_FORCE_INLINE CxSpinlock() { m_spinlock = OS_SPINLOCK_INIT; }

		/** @brief Create and initialise a new spinlock. */
		CX_FORCE_INLINE CxSpinlock(CxSpinlock::InitializeEnum) { m_spinlock = OS_SPINLOCK_INIT; }

		/** 
		 * @brief Create and initialise a new static spinlock. 
		 * A static spinlock will assume that it is never copied (and should not be), 
		 * and as such will destroy itself when the destructor is called. 
		 */
		CX_FORCE_INLINE CxSpinlock(CxSpinlock::StaticEnum) { m_spinlock = OS_SPINLOCK_INIT; }

		/** @brief Copy constructor (mainly does debug checking for copying. */
		CX_FORCE_INLINE CxSpinlock(const CxSpinlock &in_src) : m_spinlock(in_src.m_spinlock) {}

		/** @brief noop */
		CX_FORCE_INLINE ~CxSpinlock() {}

		/** @brief See copy constructor */
		CX_FORCE_INLINE CxSpinlock & operator=(const CxSpinlock &in_src) {
			m_spinlock = in_src.m_spinlock;
		}

		/** @brief Destroy the spinlock. */
		CX_FORCE_INLINE void destroy() {}
		
		/** @brief Initialise the spinlock before the first use */
		CX_FORCE_INLINE void initialize() {}

		/**  @brief Lock the spinlock. */
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
