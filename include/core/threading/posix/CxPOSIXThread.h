#ifndef CX_CORE_THREADING_POSIX_CXPOSIXTHREAD_H
#define CX_CORE_THREADING_POSIX_CXPOSIXTHREAD_H
/**
 * @copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxPOSIXThread.h: 

 * File that contains the core includes / definitions for the 
 * unix threading libraries, including a static Thread class 
 * used for creating and manipulating threads.
 *
 * @author Catlin Zilinski
 * @date July 1, 2015
 */

#include "core/Cx.h"
#include <pthread.h>

namespace cat {

	typedef void * (CxPOSIXThreadEntry)(void *);

	class CxProcessRunner;
	class CxTaskRunner;
	
	/**
	 * @class CxThread CxPOSIXThread.h "core/threading/posix/CxPOSIXThread.h"
	 *
	 * The CxThread class is used as a handle to threads, and has 
	 * static methods to do things like joining on another thread 
	 * and starting certain specific types of threads.
	 *
	 * @since July 23, 2013
	 * @version 3
	 * @author Catlin Zilinski
	 */
	class CxThread {
	  public:

		/** @brief Create a new null thread handle. */
		CxThread();

		/** @brief Create a new Thread handle for the specified thread. */
		CX_FORCE_INLINE CxThread(const pthread_t &in_thread)
			: m_handle(in_thread) {}

		/** @return True if the two threads reference the SAME thread. */
	   CX_FORCE_INLINE CxBool operator==(const CxThread &in_t2) const {
			return pthread_equal(m_handle, in_t2.m_handle) != 0;
		}

		/** @return True if the two threads reference the DIFFERENT threads. */
	   CX_FORCE_INLINE CxBool operator!=(const CxThread &in_t2) const {
			return pthread_equal(m_handle, in_t2.m_handle) == 0;
		}
		
		/**
		 * @brief Runs a specified ProcessRunner in a new thread.
		 * @param in_runner The ProcessRunner to run.
		 * @return 0 if the thread started properly, otherwise, an error code.
		 */
		static CxI32 runProcessRunner(CxProcessRunner* in_runner);

		/**
		 * @brief Runs a specified TaskRunner in a new thread.
		 * @param in_runner The TaskRunner to run.
		 * @return 0 if the thread started properly, otherwise, an error code.
		 */
		static CxI32 runTaskRunner(CxTaskRunner* in_runner);

		/**
		 * @brief Joins with another thread.
		 * Calling join on another thread will cause the current thread to 
		 * block until the other thread has terminated.
		 * @param in_handle The ThreadHandle of the thread to join on.
		 * @return True if succesfull, false if error.
		 */
		static CxBool join(CxThread &in_handle);

		/** @return A handle to the currently running thread. */
	   CX_FORCE_INLINE static CxThread self() { return CxThread(pthread_self()); }

	  private:
		pthread_t m_handle;
	};

	void * cx_process_runner_func_entry__(void *in_data);
	void * cx_task_runner_func_entry__(void *in_data);	
}

#endif // CX_CORE_THREADING_POSIX_CXPOSIXTHREAD_H

