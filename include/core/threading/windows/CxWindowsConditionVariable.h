#ifndef CAT_CORE_THREADING_WINDOWS_CXWINDOWSCONDITIONVARIABLE_H
#define CAT_CORE_THREADING_WINDOWS_CXWINDOWSCONDITIONVARIABLE_H
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
		/** @brief Create a new CxConditionVariable. */
		CxConditionVariable();

		/** @brief Copy constructor, deals with reference counting. */
		CxConditionVariable(const CxConditionVariable &in_src);

		/** @brief Destroy the CxConditionVariable and deal with reference counting. */
		~CxConditionVariable();

		/** @brief Overloaded assignment operator to deal with reference counting. */
		CxConditionVariable& operator=(const CxConditionVariable &in_src);

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
		CxI32 *mp_refCount;

		void tryDestroy();
	};
} // namespace cat

#endif // CAT_CORE_THREADING_WINDOWS_CXWINDOWSCONDITIONVARIABLE_H


