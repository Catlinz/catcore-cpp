#ifndef CAT_CORE_THREAD_MANAGER_H
#define CAT_CORE_THREAD_MANAGER_H
/**
 * @copyright Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * @file threadmanager.h
 * @brief Contains the definition for the singleton ThreadManager class.
 *
 * Contains the definition for the singleton ThreadManager class used to 
 * manage threads and threaded tasks.
 * The ThreadManager class contains methods for running tasks in parallel, creating ThreadGroups, 
 * etc.
 *
 * @author: Catlin Zilinski
 * @date: June 29, 2013
 */

#include "core/threading/thread.h"
#include "core/threading/asynctaskrunner.h"

#ifndef NUMBER_OF_TASK_RUNNERS
#define NUMBER_OF_TASK_RUNNERS 8
#endif

namespace Cat {

	// Forward class declarations
	class Runnable;

	/**
	 * @class ThreadManager threadmanager.h "core/threading/threadmanager.h"
	 * @brief A singleton class to manage threading.
	 *
	 * The ThreadManager contains the methods to run Runnable objects in a new thread, 
	 * as well as run AsyncTasks on an AsyncTaskRunner with a default of 8 running threads.
	 *
	 * @author Catlin Zilinski
	 * @version 1
	 * @since June 29, 2013
	 */
	class ThreadManager {
		public:
			/**
			 * @brief Creates a new ThreadManager with a AsycnTaskRunner with NUMBER_OF_TASK_RUNNERS runners.
			 */
			ThreadManager();
			/**
			 * @brief Deletes the AsyncTaskRunner associated with the ThreadManager.
			 */
			~ThreadManager();

			/**
			 * @brief Runs a specified Runnable object in a new thread.
			 *
			 * To run an arbitrary function in a new thread, you must first wrap it in 
			 * a RunnableFunc, then pass it to the run(Runnable*) method.
			 *
			 * @param runnable The object that implements the Runnable interface.
			 * @return A Pointer to the ThreadHandle of the new thread if the thread starts successfully.
			 */
			inline ThreadHandle* run(Runnable* runnable);	

			/**
			 * @brief Gets the AsyncTaskRunner associated with this ThreadManager instance.
			 * @return A pointer to the AsyncTaskRunner instance.
			 */
			inline AsyncTaskRunner* getTaskRunner();
			/**
			 * @brief Initializes the ThreadManager singleton instance.
			 */
			static void initializeThreadManagerInstance();

			/**
			 * @brief Destroys the ThreadManager singleton instance.
			 */
			static void destroyThreadManagerInstance();
			
			/**
			 * @brief Returns the singleton ThreadManager object.
			 * @return The ThreadManager instance.
			 */
			static inline ThreadManager* getInstance();


		private:
			AsyncTaskRunner*	runner_;
			
			static ThreadManager*	singleton_instance_;
	};

	inline ThreadManager* ThreadManager::getInstance() {
		#if defined (DEBUG)
		if (!singleton_instance_) {
			DWARN("Getting singleton instance of UNINITIALIZED ThreadManager!");
		}
		#endif

		return ThreadManager::singleton_instance_;
	}

	inline ThreadHandle* ThreadManager::run(Runnable* runnable) {
		return Thread::run(runnable);
	}

	inline AsyncTaskRunner* ThreadManager::getTaskRunner() {
		return runner_;
	}


} // namespace Cat


#endif //CAT_CORE_THREAD_MANAGER_H
