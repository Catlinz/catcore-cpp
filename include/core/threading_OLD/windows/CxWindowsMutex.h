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
		enum FlagsEnum { kMutexStatic = 1 << 0, kMutexInitialized = 1 << 1 };

		/** @brief Flags to pass to constructor to indicate behaviour. */
		enum StaticEnum { kStatic = 1 << 0 };
		enum InitializeEnum { kInitialize = 1 << 0 };
		
	  public:
		/** @brief Create a new CxMutex. */
		CX_FORCE_INLINE CxMutex() : mp_mutex(0), m_flags(0) {}

		/** @brief Create and initialise a new mutex. */
		CX_FORCE_INLINE CxMutex(CxMutex::InitializeEnum)
			: mp_mutex(0), m_flags(0) { initialize(); }
		
		/** 
		 * @brief Create and initialise a new static mutex. 
		 * A static mutex will assume that it is never copied (and should not be), 
		 * and as such will destroy itself when the destructor is called. 
		 */
		CxMutex(CxMutex::StaticEnum)
			: mp_mutex(0), m_flags(kMutexStatic) { initialize(); }

		/** @brief Copy constructor (mainly does debug checking for copying. */
		CxMutex(const CxMutex &in_src)
			: mp_mutex(in_src.mp_mutex), m_flags(in_src.m_flags) {
			CXD_IF_ERR(((m_flags & kMutexStatic) != 0), "DO NOT COPY STATIC MUTEXES FFS!");
		}

		/** @brief Destroy the CxMutex if it is static. */
		CX_FORCE_INLINE ~CxMutex() {
			if ((m_flags & kMutexStatic) != 0) { destroy(); }
		}

		/** @brief See copy constructor */
		CxMutex & operator=(const CxMutex &in_src);

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
		CX_FORCE_INLINE CxBool tryLock() { return TryEnterCriticalSection(mp_mutex) != 0; }

		/** @brief Unlock the mutex. */
		CX_FORCE_INLINE void unlock() { LeaveCriticalSection(mp_mutex); }

		friend class CxConditionVariable;
		
	  private:
		
		CRITICAL_SECTION *mp_mutex;
		CxI32 m_flags;
	};
} // namespace cat

#endif // CX_CORE_THREADING_WINDOWS_CXWINDOWSMUTEX_H


