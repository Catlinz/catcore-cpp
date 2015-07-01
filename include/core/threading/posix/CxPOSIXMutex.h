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
		/** @brief Create a new CxMutex. */
		CxMutex();

		/**
		 * @brief Copy constructor, handles reference counting. 
		 * @param in_src The mutex to copy.
		 */
		CxMutex(const CxMutex& in_src);

		/** @brief Destroy the CxMutex. */
		~CxMutex();

		/**
		 * @brief Overloaded assignment operator to handle reference counting.
		 * @param in_src The mutex to copy.
		 */
		CxMutex& operator=(const CxMutex& in_src);

		/** @brief Lock the mutex. */
		CX_FORCE_INLINE void lock() { pthread_mutex_lock(&m_mutex); }

		/** @brief Try and lock the mutex, but don't wait. */
		CX_FORCE_INLINE CxBool tryLock() { return (pthread_mutex_trylock(&m_mutex) == 0); }

		/** @brief Unlock the mutex. */
		CX_FORCE_INLINE void unlock() { pthread_mutex_unlock(&m_mutex); }

		friend class CxConditionVariable;
	  private:
		pthread_mutex_t	m_mutex;
		CxI32* mp_refCount;

		void tryDestroy();
	};

}

#endif // CX_CORE_THREADING_POSIX_CXPOSIXMUTEX_H


