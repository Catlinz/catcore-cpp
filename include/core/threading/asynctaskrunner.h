#ifndef CAT_CORE_THREADING_ASYNCTASKRUNNER_H
#define CAT_CORE_THREADING_ASYNCTASKRUNNER_H
/**
 * @copyright Catlin Zilinksi, 2014.  All rights reserved.
 *
 * @file asynctaskrunner.h: 
 * @brief Contains the definition for the AsyncTaskRunner class.
 *
 * @author Catlin Zilinski
 * @date July 22, 2013
 */

#include "core/corelib.h"
#include "core/threading/conditionvariable.h"
#include "core/threading/runnable.h"

namespace Cat {

	class AsyncTaskRunnerThread;
	class AsyncTask;
	class AsyncTaskQueuedItem;
	class AsyncResult;

	enum AsyncTaskRunnerState { RUNNER_STARTED, RUNNER_STOPPING, RUNNER_STOPPED };

	/**
	 * The AsyncTaskRunner class is used to run tasks in parallel.  
	 * The AsyncTaskRunner contains a set amount of threads that are constantly 
	 * running (or waiting) on which tasks are run.  The number of threads should be equal to 
	 * the number of processor cores to ensure maximum parallellism.  
	 */
	class AsyncTaskRunner {
	  public:
		/**
		 * Initializes a AsyncTaskRunner with a default number of threads.
		 */
		AsyncTaskRunner();
		/**
		 * Initializes a AsyncTaskRunner with the specified number of threads.
		 * @param number_of_threads The number of threads the runner has.
		 */
		AsyncTaskRunner(U32 number_of_threads);
		/**
		 * The destructor waits for the threads to all terminate.
		 */
		~AsyncTaskRunner();

		/**
		 * The method to stop the AsyncTaskRunner. 
		 * After this method is called, no other task can be run, and the remaining
		 * tasks are run to completion.
		 */
		void stop();

		/**
		 * Add an AsyncTask to the queue to run.
		 * @param task The AsyncTask* to add to the queue.
		 */
		AsyncResult* run(AsyncTask* task);

		/**
		 * Get the number of threads in the AsyncTaskRunner.
		 * @return The number of threads in the AsyncTaskRunner.
		 */
		inline U32 getNumberOfThreads() const;


		friend class AsyncTaskRunnerThread;

	  private:
		ConditionVariable*	sync_controller_;	/**< The condition variable for threads to wait on */
			
		AsyncTaskQueuedItem*	first_;			/**< The first queued task */
		AsyncTaskQueuedItem*	last_;			/**< The last queued task */

		AsyncTaskRunnerThread**	runners_;	/**< The array of threads */
		AsyncTaskRunnerState		state_;		/**< The current state of the AsyncTaskRunner */

		U32 	number_of_threads_;	/**< The number of threads in the array */



		void initAsyncTaskRunner(U32 number_of_threads);

	};

	inline U32 AsyncTaskRunner::getNumberOfThreads() const { return number_of_threads_; }

	/**
	 * The AsyncTaskRunnerThread encompases a thread of the AsyncTaskRunner that is able 
	 * to accept and wait for tasks.
	 */
	class AsyncTaskRunnerThread : public Runnable {
	  public:
		/**
		 * Creates a new AsyncTaskRunnerThread in a waiting state.
		 * @param runner The AsyncTaskRunner that created the thread.
		 */
		AsyncTaskRunnerThread(AsyncTaskRunner* runner, U32 id) : runner_(runner), id_(id) {}

		~AsyncTaskRunnerThread();

		/**
		 * The method to get a task from the AsyncTaskRunner's queue to run on 
		 * this thread.
		 * @return A AsyncTask to run, or NIL if the queue is empty.
		 * */
		AsyncTask* getTaskToRun();

		/**
		 * The method that runs the Thread loop.
		 */
		I32 run();

	  private:
		AsyncTaskRunner* 		runner_;	/**< The ower of this thread */
		U32 						id_;		/**< A numeric identifier of the thread runner */
			

	};

	/**
	 * A simple class to hold a queued task to be run on a thread.
	 */
	class AsyncTaskQueuedItem {
	  public:
		AsyncTaskQueuedItem(AsyncTask* ptask, AsyncTaskQueuedItem* pprev = NIL);
		~AsyncTaskQueuedItem();

		AsyncTask*				task;
		AsyncTaskQueuedItem* next;
		AsyncTaskQueuedItem* prev;
			
	};

} // namespace Cat

#endif // CAT_CORE_THREADING_ASYNCTASKRUNNER_H


