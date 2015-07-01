#ifndef CX_CORE_THREADING_POSIX_CXPOSIXCONDITIONVARIABLE_H
#define CX_CORE_THREADING_POSIX_CXPOSIXCONDITIONVARIABLE_H
/**
 * @copyright copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxPOSIXConditionVariable.h
 * @brief Contains the definition of the CxConditionVariable class for threading.
 *
 * @author Catlin Zilinski
 * @date June 28, 2015
 */

#include "core/threading/CxMutex.h"

namespace cat {

	/**
	 * @class CxConditionVariable CxPOSIXConditionVariable.h "core/threading/posix/CxPOSIXConditionVariable.h"
	 *
	 * A synchronisation object that can be waited on by other threads until 
	 * signalled.
	 *
	 * @version 2
	 * @author Catlin Zilinski
	 * @since Mar 3, 2015
	 */
	class CxConditionVariable {
	  public:
		/** @brief Create a new condition variable. */
		CxConditionVariable();

		/** @brief Copy constructor, handles reference counting. */
		CxConditionVariable(const CxConditionVariable& in_src);

		/** @brief destroy the condition variable. */
		~CxConditionVariable();

		/** @brief Overloaded assignment operator to handle reference counting. */
		CxConditionVariable& operator=(const CxConditionVariable& in_src);

		/** @brief Wait on the specified mutex until signalled. */
		CX_FORCE_INLINE void wait(CxMutex& in_mutex) { pthread_cond_wait(&m_cv, &(in_mutex.m_mutex)); }

		/** @brief Signal a single thread that is waiting on the condition variable. */
		CX_FORCE_INLINE void signal() { p_thread_cond_signal(&m_cv); }

		/** @brief Signal all threads that are waiting on the condition variable. */
		CX_FORCE_INLINE void broadcast() { pthread_cond_broadcast(&m_cv); }

	  private:
		pthread_cond_t m_cv;
		CxI32 *mp_refCount;

		void tryDestroy();
	};
}

#endif // CX_CORE_THREADING_POSIX_CXPOSIXCONDITIONVARIABLE_H



