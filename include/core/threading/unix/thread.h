#ifndef CAT_CORE_THREADING_UNIX_THREAD_H
#define CAT_CORE_THREADING_UNIX_THREAD_H
/**
 * @copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file thread.h: 

 * File that contains the core includes / definitions for the 
 * unix threading libraries, including a static Thread class 
 * used for creating and manipulating threads.
 *
 * @author Catlin Zilinski
 * @date Mar 8, 2014
 */

#include "core/threading/unix/runnable.h"

namespace Cat {

	class Runnable;
	class ProcessRunner;
	class TaskRunner;	

	/**
	 * @class Thread thread.h "core/threading/thread.h"
	 * @brief Static class to run and join threads.
	 *
	 * The Thread class is a simple static class that contains methods to 
	 * start, join, and wait for threads.
	 *
	 * @since July 23, 2013
	 * @version 2
	 * @author Catlin Zilinski
	 */
	class Thread {
	  public:
		/**
		 * @brief Runs a specified Runnable object in a new thread.
		 * @param runnable The object that implements the Runnable interface.
		 * @return A Pointer to the ThreadHandle of the Runnable if the thread starts successfully, else NIL.
		 */
		static ThreadHandle* run(Runnable* runnable);

		/**
		 * @brief Runs a specified ProcessRunner in a new thread.
		 * @param manager The ProcessRunner to run.
		 * @return A Pointer to the ThreadHandle of the ProcessRunner 
		 * if the thread starts successfully, else NIL.
		 */
		static ThreadHandle* runProcessRunner(ProcessRunner* runner);

		/**
		 * @brief Runs a specified TaskRunner in a new thread.
		 * @param manager The TaskRunner to run.
		 * @return A Pointer to the ThreadHandle of the TaskRunner 
		 * if the thread starts successfully, else NIL.
		 */
		static ThreadHandle* runTaskRunner(TaskRunner* runner);

		/**
		 * Joins with another thread.
		 * Calling join on another thread will cause the current thread to 
		 * block until the other thread has terminated.
		 * @param threadHandle The ThreadHandle of the thread to join on.
		 */
		static ThreadStatus join(ThreadHandle* threadHandle);

		/**
		 * Gets the currently running threads handle.
		 * @return The ThreadHandle of the currently running thread.
		 */
		inline static ThreadHandle self() {
			return pthread_self();
		}		

	};
	
	VPtr process_runner_func_entry__(VPtr data);
	VPtr task_runner_func_entry__(VPtr data);	
}

#endif // CAT_CORE_THREADING_UNIX_THREAD_H

