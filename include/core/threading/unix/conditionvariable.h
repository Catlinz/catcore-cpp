#ifndef CAT_CORE_THREADING_UNIX_CONDITION_VARIABLE_H
#define CAT_CORE_THREADING_UNIX_CONDITION_VARIABLE_H
/**
 * Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * conditionvariable.h: Contains the definition of the ConditionVariable class for threading.
 *
 * Author: Catlin Zilinski
 * Date: July 22, 2013
 */

#include "core/threading/mutex.h"


namespace Cat {

	/**
	 * The ConditionVariable class contains the methods to lock/unlock the ConditionVariable for testing, 
	 * and the method to wait/signal the ConditionVariable.
	 */
	class ConditionVariable {
		public:
			ConditionVariable();
			~ConditionVariable();

			inline void wait(Mutex& p_lock) {
				pthread_cond_wait(&m_cv, &(p_lock.m_mutex));
			}
			  
			inline void signal() {
				p_thread_cond_signal(&m_cv);
			}
			
			inline void broadcast() {
				pthread_cond_broadcast(&m_cv);
			}

		private:
			pthread_cond_t		m_cv;
	};
}

#endif // CAT_CORE_THREADING_UNIX_CONDITION_VARIABLE_H



