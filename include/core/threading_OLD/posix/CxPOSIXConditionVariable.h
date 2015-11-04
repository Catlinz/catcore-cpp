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
		enum FlagsEnum { kConditionVariableStatic = 1 << 0, kConditionVariableInitialized = 1 << 1 };

		/** @brief Flags to pass to constructor to indicate behaviour. */
		enum StaticEnum { kStatic = 1 << 0 };
		enum InitializeEnum { kInitialize = 1 << 0 };
			

		/** @brief Create an uninitialised CxConditionVariable. */
		CX_FORCE_INLINE CxConditionVariable() : m_flags(0) {}

		/** @brief Create and initialise a new ConditionVariable. */
		CX_FORCE_INLINE CxConditionVariable(CxConditionVariable::InitializeEnum)
			: m_flags(0) { initialize(); }

		/** 
		 * @brief Create and initialise a new static ConditionVariable. 
		 * A static ConditionVariable will assume that it is never copied (and should not be), 
		 * and as such will destroy itself when the destructor is called. 
		 */
		CX_FORCE_INLINE CxConditionVariable(CxConditionVariable::StaticEnum)
			: m_flags(kConditionVariableStatic) { initialize(); }

		/** @brief Copy constructor (mainly does debug checking for copying. */
		CX_FORCE_INLINE CxConditionVariable(const CxConditionVariable &in_src)
			: m_cv(in_src.m_cv), m_flags(in_src.m_flags) {
			CXD_IF_ERR(((m_flags & kConditionVariableStatic) != 0), "DO NOT COPY STATIC CONDITION VARIABLES FFS!");
		}

		/** @brief Destroy the CxConditionVariable if it is static. */
		CX_FORCE_INLINE ~CxConditionVariable() {
			if ((m_flags & kConditionVariableStatic) != 0) { destroy(); }
		}

		/** @brief See copy constructor */
		CxConditionVariable & operator=(const CxConditionVariable &in_src);

		/** @brief Destroy the ConditionVariable. */
		void destroy();
		
		/** @brief Initialise the ConditionVariable before the first use */
		void initialize();

		/** @brief Wait on the specified mutex until signalled. */
		CX_FORCE_INLINE void wait(CxMutex& in_mutex) { pthread_cond_wait(&m_cv, &(in_mutex.m_mutex)); }

		/** @brief Signal a single thread that is waiting on the condition variable. */
		CX_FORCE_INLINE void signal() { p_thread_cond_signal(&m_cv); }

		/** @brief Signal all threads that are waiting on the condition variable. */
		CX_FORCE_INLINE void broadcast() { pthread_cond_broadcast(&m_cv); }

	  private:
		pthread_cond_t m_cv;
		CxI32 m_flags;
	};
}

#endif // CX_CORE_THREADING_POSIX_CXPOSIXCONDITIONVARIABLE_H



