#ifndef CX_CORE_THREADING_WINDOWS_CXWINDOWSCONDITIONVARIABLE_H
#define CX_CORE_THREADING_WINDOWS_CXWINDOWSCONDITIONVARIABLE_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file CxWindowsConditionVariable.h
 * @brief Contains the definition of the ConditionVariable class for threading.
 *
 * @author Catlin Zilinski
 * @date July 1, 2015
 */

#include "core/threading/CxMutex.h"

namespace cat {

	/**
	 * @class CxConditionVariable CxWindowsConditionVariable.h "core/threading/windows/CxWindowsConditionVariable.h"
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
		enum FlagsEnum { kConditionVariableStatic = 1 << 0,
							  kConditionVariableInitialized = 1 << 1 };

		/** @brief Flags to pass to constructor to indicate behaviour. */
		enum StaticEnum { kStatic = 1 << 0 };
		enum InitializeEnum { kInitialize = 1 << 0 };
			

		/** @brief Create an uninitialised CxConditionVariable. */
		CX_FORCE_INLINE CxConditionVariable() : mp_cv(0), m_flags(0) {}

		/** @brief Create and initialise a new condition variable. */
		CX_FORCE_INLINE CxConditionVariable(CxConditionVariable::InitializeEnum)
			: mp_cv(0), m_flags(0) { initialize(); }

		/** 
		 * @brief Create and initialise a new static condition variable. 
		 * A static condition variable will assume that it is never copied (and should not be), 
		 * and as such will destroy itself when the destructor is called. 
		 */
		CX_FORCE_INLINE CxConditionVariable(CxConditionVariable::StaticEnum)
			: mp_cv(0), m_flags(kConditionVariableStatic) { initialize(); }

		/** @brief Copy constructor (mainly does debug checking for copying. */
		CX_FORCE_INLINE CxConditionVariable(const CxConditionVariable &in_src)
			: mp_cv(in_src.mp_cv), m_flags(in_src.m_flags) {
			CXD_IF_ERR(((m_flags & kConditionVariableStatic) != 0), "DO NOT COPY STATIC CONDITION VARIABLES FFS!");
		}

		/** @brief Destroy the CxConditionVariable if it is static. */
		CX_FORCE_INLINE ~CxConditionVariable() {
			if ((m_flags & kConditionVariableStatic) != 0) { destroy(); }
		}

		/** @brief See copy constructor */
		CxConditionVariable & operator=(const CxConditionVariable &in_src);

		/** @brief Destroy the condition variable. */
		void destroy();
		
		/** @brief Initialise the condition variable before the first use */
		void initialize();

		/** @brief Broadcast that all threads waiting on the CV should wakeup. */
		CX_FORCE_INLINE void broadcast() { WakeAllConditionVariable(mp_cv); }

		/** @brief Wake up a single thread waiting on the CV. */
		CX_FORCE_INLINE void signal() { WakeConditionVariable(mp_cv); }

		/** @brief Wait on the condition variable with the specified mutex. */
		CX_FORCE_INLINE void wait(CxMutex &in_lock) {
			SleepConditionVariableCS(mp_cv, in_lock.mp_mutex, INFINITE);
		}

	  private:
		CONDITION_VARIABLE *mp_cv;
		CxI32 m_flags;
	};
} // namespace cat

#endif // CX_CORE_THREADING_WINDOWS_CXWINDOWSCONDITIONVARIABLE_H


