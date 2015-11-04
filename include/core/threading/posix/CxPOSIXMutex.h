#ifndef CX_CORE_THREADING_POSIX_CXPOSIXMUTEX_H
#define CX_CORE_THREADING_POSIX_CXPOSIXMUTEX_H
/**
 * @copyright Copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxMutex.h
 * @brief Contains the definition of the CxMutex class for threading.
 *
 * @author Catlin Zilinski
 * @date Nov 3, 2015
 */

#include "core/Cx.h"
#include <pthread.h>

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
		CX_FORCE_INLINE ~CxMutex() {  destroy(); }

		/** @brief Destroy the mutex. */
		void destroy();
		
		/** @brief Initialise the mutex before the first use */
		void initialize();

		/** @brief Lock the mutex. */
		CX_FORCE_INLINE void lock() { pthread_mutex_lock(mp_mutex); }

		/** @brief Try and lock the mutex, but don't wait. */
		CX_FORCE_INLINE CxBool tryLock() { return (pthread_mutex_trylock(mp_mutex) == 0); }

		/** @brief Unlock the mutex. */
		CX_FORCE_INLINE void unlock() { pthread_mutex_unlock(mp_mutex); }

		friend class CxConditionVariable;
		
	  private:
		pthread_mutex_t *mp_mutex;
	};

}

#endif // CX_CORE_THREADING_POSIX_CXPOSIXMUTEX_H


