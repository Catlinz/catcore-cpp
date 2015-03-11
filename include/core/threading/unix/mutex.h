#ifndef CAT_CORE_THREADING_UNIX_MUTEX_H
#define CAT_CORE_THREADING_UNIX_MUTEX_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file mutex.h
 * @brief Contains the definition of the Mutex class for threading.
 *
 * @author Catlin Zilinski
 * @date July 22, 2013
 */

#include "core/threading/unix/threaddefs.h"

namespace Cat {

	/**
	 * @class Mutex mutex.h "core/threading/mutext.h"
	 * The Mutex class encapsulates the functionality of a Mutex lock for 
	 * syncronization with multiple threads.
	 * @version 1
	 * @author Catlin Zilinski
	 * @since Mar 3, 2015
	 */
	class Mutex {
	  public:
		/**
		 * @brief Create a new Mutex.
		 */
		Mutex();

		/**
		 * @brief Destroy the Mutex.
		 */
		~Mutex();

		/**
		 * @brief Lock the mutex.
		 */
		inline void lock() {
			pthread_mutex_lock(&m_mutex);
		}

		/**
		 * @brief Try and lock the mutex, but don't wait.
		 */
		inline Boolean tryLock() {
			return (pthread_mutex_trylock(&m_mutex) == 0);
		}

		/**
		 * @brief Unlock the mutex.
		 */
		inline void unlock() {
			pthread_mutex_unlock(&m_mutex);
		}

		friend class ConditionVariable;
	  private:
		pthread_mutex_t	m_mutex;
	};

}

#endif // CAT_CORE_THREADING_UNIX_MUTEX_H


