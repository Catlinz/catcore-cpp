#ifndef CX_CORE_THREADING_POSIX_CXPOSIXMUTEX_H
#define CX_CORE_THREADING_POSIX_CXPOSIXMUTEX_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxPOSIXMutex.h
 * @brief Contains the definition of the CxMutex class for threading.
 *
 * @author Catlin Zilinski
 * @date June 28, 2015
 */

#include "core/Cx.h"
#include <pthread.h>

namespace cat {

	/**
	 * @class CxMutex CxPOSIXMutex.h "core/threading/posix/CxPOSIXMutex.h"
	 * The Mutex class encapsulates the functionality of a Mutex lock for 
	 * syncronization with multiple threads.
	 * @version 2
	 * @author Catlin Zilinski
	 * @since Mar 3, 2015
	 */
	class CxMutex {
	  public:
		enum FlagsEnum { kMutexStatic = 1 << 0, kMutexInitialized = 1 << 1 };

		/** @brief Flags to pass to constructor to indicate behaviour. */
		enum StaticEnum { kStatic = 1 << 0 };
		enum InitializeEnum { kInitialize = 1 << 0 };
			

		/** @brief Create an uninitialised CxMutex. */
		CX_FORCE_INLINE CxMutex() : m_flags(0) {}

		/** @brief Create and initialise a new mutex. */
		CX_FORCE_INLINE CxMutex(CxMutex::InitializeEnum)
			: m_flags(0) { initialize(); }

		/** 
		 * @brief Create and initialise a new static mutex. 
		 * A static mutex will assume that it is never copied (and should not be), 
		 * and as such will destroy itself when the destructor is called. 
		 */
		CX_FORCE_INLINE CxMutex(CxMutex::StaticEnum)
			: m_flags(kMutexStatic) { initialize(); }

		/** @brief Copy constructor (mainly does debug checking for copying. */
		CX_FORCE_INLINE CxMutex(const CxMutex &in_src)
			: m_mutex(in_src.m_mutex), m_flags(in_src.m_flags) {
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
		CX_FORCE_INLINE void lock() { pthread_mutex_lock(&m_mutex); }

		/** @brief Try and lock the mutex, but don't wait. */
		CX_FORCE_INLINE CxBool tryLock() { return (pthread_mutex_trylock(&m_mutex) == 0); }

		/** @brief Unlock the mutex. */
		CX_FORCE_INLINE void unlock() { pthread_mutex_unlock(&m_mutex); }

		friend class CxConditionVariable;
	  private:
		pthread_mutex_t	m_mutex;
		CxI32 m_flags;
	};

}

#endif // CX_CORE_THREADING_POSIX_CXPOSIXMUTEX_H


