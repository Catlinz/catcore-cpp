#ifndef CX_CORE_THREADING_WINDOWS_CXWINDOWSMUTEX_H
#define CX_CORE_THREADING_WINDOWS_CXWINDOWSMUTEX_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file CxMutex.h
 * @brief Contains the definition of the Mutex class for threading.
 *
 * @author Catlin Zilinski
 * @date Nov 3, 2015
 */

#include "core/Cx.h"
#include <Windows.h>

namespace cat {

	/**
	 * @class CxMutex CxMutex.h "core/threading/CxMutex.h"
	 * The Mutex class encapsulates the functionality of a Mutex lock for 
	 * syncronization with multiple threads.
	 * @version 3
	 * @author Catlin Zilinski
	 * @since Mar 3, 2015
	 */
	class CxMutex {
		
	  public:
		/** @brief Create an uninitialised CxMutex. */
		CX_FORCE_INLINE CxMutex() : mp_mutex(0) {}

		/** @brief Create and initialise a new mutex. */
		CX_FORCE_INLINE CxMutex(CxInitFlag) : mp_mutex(0) {
			initialize();
		}

		/** @brief Move constructor */
		CX_FORCE_INLINE CxMutex(CxMutex &&in_src)
			: mp_mutex(in_src.mp_mutex) {
			in_src.mp_mutex = 0;
		}

		/** @brief DO NOT PASS BY VALUE */
		CxMutex(const CxMutex &);

		/** @brief DO NOT PASS BY VALUE */
		CxMutex & operator=(const CxMutex &);

		/** @brief Move-assign operator. */
		CX_FORCE_INLINE CxMutex & operator=(CxMutex &&in_src) {
		   if (mp_mutex != 0) { destroy(); }
			mp_mutex = in_src.mp_mutex;  in_src.mp_mutex = 0;
			return *this;
		}

		/** @brief Destroy the CxMutex. */
		CX_FORCE_INLINE ~CxMutex() { destroy(); }

		/** @brief Destroy the mutex. */
		void destroy();
		
		/** @brief Initialise the mutex before the first use */
		void initialize();

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
	};
} // namespace cat

#endif // CX_CORE_THREADING_WINDOWS_CXWINDOWSMUTEX_H


