#ifndef CAT_CORE_THREADING_WIN32_MUTEX_H
#define CAT_CORE_THREADING_WIN32_MUTEX_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file mutex.h
 * @brief Contains the definition of the Mutex class for threading.
 *
 * @author Catlin Zilinski
 * @date Mar 3, 2015
 */

#include "core/threading/atomic.h"

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
		 * @brief Copy constructor.
		 * @param p_src The Mutex to copy.
		 */
		Mutex(const Mutex& p_src);

		/**
		 * @brief Destroy the Mutex.
		 */
		~Mutex();

		/**
		 * @brief Overloaded assignment operator.
		 * @param p_src The Mutex to copy from.
		 * @return A reference to this mutex.
		 */
		Mutex& operator=(const Mutex& p_src);

		/**
		 * @brief Lock the mutex.
		 */
		inline void lock() {
			EnterCriticalSection(m_pMutex);
		}

		/**
		 * @brief Try and lock the mutex, but don't wait.
		 * @return True if now owns the lock.
		 */
		inline Boolean tryLock() {
			return TryEnterCriticalSection(m_pMutex);
		}

		/**
		 * @brief Unlock the mutex.
		 */
		inline void unlock() {
			LeaveCriticalSection(m_pMutex);
		}

		friend class ConditionVariable;
	  private:
		void tryDestroy();
		
		CRITICAL_SECTION *m_pMutex;
		AtomicI32 *m_pRetainCount;
	};
} // namespace Cat

#endif // CAT_CORE_THREADING_WIN32_MUTEX_H


