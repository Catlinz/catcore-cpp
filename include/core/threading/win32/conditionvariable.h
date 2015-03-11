#ifndef CAT_CORE_THREADING_WIN32_CONDITIONVARIABLE_H
#define CAT_CORE_THREADING_WIN32_CONDITIONVARIABLE_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file conditionvariable.h
 * @brief Contains the definition of the ConditionVariable class for threading.
 *
 * @author Catlin Zilinski
 * @date Mar 3, 2015
 */

#include "core/threading/mutex.h"

namespace Cat {

	/**
	 * @class ConditionVariable conditionvariable.h "core/threading/conditionvariable.h"
	 * A mutex that spins in place instead of getting a kernel level
	 * lock (expensive).
	 * @version 1
	 * @author Catlin Zilinski
	 * @since Mar 3, 2015
	 */
	class ConditionVariable {
	  public:
		/**
		 * @brief Create a new ConditionVariable.
		 */
		ConditionVariable();

		/**
		 * @brief Copy constructor.
		 * @param p_src The ConditionVariable to copy.
		 */
		ConditionVariable(const ConditionVariable& p_src);

		/**
		 * @brief Destroy the ConditionVariable.
		 */
		~ConditionVariable();

		/**
		 * @brief Overloaded assignment operator.
		 * @param p_src The ConditionVariable to copy from.
		 * @return A reference to this ConditionVariable.
		 */
		ConditionVariable& operator=(const ConditionVariable& p_src);

		/**
		 * @brief Broadcast that all threads waiting on the CV should wakeup.
		 */
		inline void broadcast() {
			WakeAllConditionVariable(m_pCV);
		}

		/**
		 * @brief Wake up a single thread waiting on the CV.
		 */
		inline void signal() {
			WakeConditionVariable(m_pCV);
		}

		/**
		 * @brief Wait on the condition variable.
		 */
		inline void wait(Mutex& p_lock) {
			SleepConditionVariableCS(m_pCV, p_lock.m_pMutex, INFINITE);
		}

	  private:
		void tryDestroy();
		
		CONDITION_VARIABLE *m_pCV;
		AtomicI32 *m_pRetainCount;
	};
} // namespace Cat

#endif // CAT_CORE_THREADING_WIN32_CONDITIONVARIABLE_H


