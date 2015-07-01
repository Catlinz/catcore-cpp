#ifndef CX_CORE_THREADING_WINDOWS_CXWINDOWSMUTEX_H
#define CX_CORE_THREADING_WINDOWS_CXWINDOWSMUTEX_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file CxWindowsMutex.h
 * @brief Contains the definition of the Mutex class for threading.
 *
 * @author Catlin Zilinski
 * @date July 1, 2015
 */

#include "core/Cx.h"
#include <Windows.h>

namespace cat {

	/**
	 * @class CxMutex CxWindowsMutex.h "core/threading/windows/CxWindowsMutex.h"
	 * The Mutex class encapsulates the functionality of a Mutex lock for 
	 * syncronization with multiple threads.
	 * @version 2
	 * @author Catlin Zilinski
	 * @since Mar 3, 2015
	 */
	class CxMutex {
	  public:
		/** @brief Create a new CxMutex. */
		CxMutex();

		/** @brief Copy constructor, handles reference counting. */
		CxMutex(const CxMutex& in_src);

		/** @brief Destroy the CxMutex, handles reference counting. */
		~CxMutex();

		/** @brief Overloaded assignment operator, handles reference counting. */
		CxMutex& operator=(const CxMutex& in_src);

		/** @brief Lock the mutex. */
		CX_FORCE_INLINE void lock() { EnterCriticalSection(mp_mutex); }

		/**
		 * @brief Try and lock the mutex, but don't wait.
		 * @return True if now owns the lock.
		 */
		CX_FORCE_INLINE CxBool tryLock() { return TryEnterCriticalSection(mp_mutex); }

		/** @brief Unlock the mutex. */
		CX_FORCE_INLINE void unlock() { LeaveCriticalSection(mp_mutex); }

		friend class CxConditionVariable;
	  private:
		
		CRITICAL_SECTION *mp_mutex;
		CxI32 *mp_refCount;

		void tryDestroy();
	};
} // namespace cat

#endif // CX_CORE_THREADING_WINDOWS_CXWINDOWSMUTEX_H


