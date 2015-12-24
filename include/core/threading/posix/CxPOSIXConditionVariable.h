#ifndef CX_CORE_THREADING_POSIX_CXPOSIXCONDITIONVARIABLE_H
#define CX_CORE_THREADING_POSIX_CXPOSIXCONDITIONVARIABLE_H
/**
 * @copyright copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxConditionVariable.h
 * @brief Contains the definition of the CxConditionVariable class for threading.
 *
 * @author Catlin Zilinski
 * @date Nov 3, 2015
 */

#include "core/threading/CxMutex.h"

namespace cat {

	/**
	 * @class CxConditionVariable CxConditionVariable.h "core/threading/CxConditionVariable.h"
	 *
	 * A synchronisation object that can be waited on by other threads until 
	 * signaled.
	 *
	 * @version 3
	 * @author Catlin Zilinski
	 * @since Mar 3, 2015
	 */
	class CxConditionVariable {
	  public:

		/** @brief Create an uninitialised CxConditionVariable. */
		CX_FORCE_INLINE CxConditionVariable() : mp_cv(0) {}

		/** @brief Create and initialise a new mutex. */
		CX_FORCE_INLINE CxConditionVariable(CxInitFlag) : mp_cv(0) {
			initialize();
		}
		
		/** @brief Move constructor */
		CX_FORCE_INLINE CxConditionVariable(CxConditionVariable &&in_src)
			: mp_cv(in_src.mp_cv) {
			in_src.mp_cv = 0;
		}

		/** @brief DO NOT PASS BY VALUE */
		CxConditionVariable(const CxConditionVariable &);

		/** @brief DO NOT PASS BY VALUE */
		CxConditionVariable & operator=(const CxConditionVariable &);

		/** @brief Move-assign operator. */
		CX_FORCE_INLINE CxConditionVariable & operator=(CxConditionVariable &&in_src) {
		   if (mp_cv != 0) { destroy(); }
			mp_cv = in_src.mp_cv;  in_src.mp_cv = 0;
			return *this;
		}

		/** @brief Destroy the CxConditionVariable. */
		CX_FORCE_INLINE ~CxConditionVariable() {  destroy(); }
		
		/** @brief Initialise the ConditionVariable before the first use */
		void initialize();

		/** @brief Wait on the specified mutex until signalled. */
		CX_FORCE_INLINE void wait(CxMutex &in_mutex) {
			pthread_cond_wait(mp_cv, in_mutex.mp_mutex);
		}

		/** @brief Signal a single thread that is waiting on the condition variable. */
		CX_FORCE_INLINE void signal() { p_thread_cond_signal(mp_cv); }

		/** @brief Signal all threads that are waiting on the condition variable. */
		CX_FORCE_INLINE void broadcast() { pthread_cond_broadcast(mp_cv); }

	  private:
		pthread_cond_t *mp_cv;
	};
}

#endif // CX_CORE_THREADING_POSIX_CXPOSIXCONDITIONVARIABLE_H



