#ifndef CX_CORE_THREADING_WINDOWS_CXWINDOWSTHREAD_H
#define CX_CORE_THREADING_WINDOWS_CXWINDOWSTHREAD_H
/**
 * @copyright Catlin Zilinksi, 2015.  All rights reserved.
 *
 * @file CxWindowsThread.h: 

 * File that contains the core includes / definitions for the 
 * Windows threading libraries, including a static Thread class 
 * used for creating and manipulating threads.
 *
 * @author Catlin Zilinski
 * @date July 1, 2015
 */

#include "core/Cx.h"
#include <Windows.h>

namespace cat {

	typedef DWORD (CxWindowsThreadEntry)(void *);

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
		CX_FORCE_INLINE CxThread() : m_id(0) {}

		/** @brief Create a new Thread handle for the specified thread id. */
		CX_FORCE_INLINE CxThread(DWORD in_threadId) : m_id(in_threadId) {}

		/** @brief Create a new Thread handle for the specified thread handle. */
		CX_FORCE_INLINE CxThread(HANDLE in_thread) {
			m_id = GetThreadId(in_thread);
		}

		/** @return True if the two threads reference the SAME thread. */
	   CX_FORCE_INLINE CxBool operator==(const CxThread &in_t2) const {
			return m_id == in_t2.m_id;
		}

		/** @return True if the two threads reference the DIFFERENT threads. */
	   CX_FORCE_INLINE CxBool operator!=(const CxThread &in_t2) const {
			return m_id != in_t2.m_id;
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
		CX_FORCE_INLINE static CxThread self() { return CxThread(GetCurrentThreadId()); }
		
	  private:
		DWORD m_id;
	};

   DWORD cx_process_runner_func_entry__(void *in_data);
   DWORD cx_task_runner_func_entry__(void *in_data);	
}

#endif // CX_CORE_THREADING_WINDOWS_CXWINDOWSTHREAD_H

