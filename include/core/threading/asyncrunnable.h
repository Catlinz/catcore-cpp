#ifndef CAT_CORE_THREADING_ASYNCRUNNABLE_H
#define CAT_CORE_THREADING_ASYNCRUNNABLE_H
/**
 * Copyright Catlin Zilinksi, 2013.  All rights reserved.
 *
 * asyncrunnable.h: Contains the definition for the AsyncFunction.  Basically it's a Runnable 
 * object that can be run by an AsyncTaskRunner.
 *
 * Author: Catlin Zilinski
 * Date: Sept 19, 2013
 */

#include "core/threading/asynctask.h"
#include "core/threading/asyncresult.h"

namespace Cat {

	class AsyncRunnableResult;

	class AsyncRunnable : public AsyncTask {
	  public:
		/**
		 * Create a AsyncRunnable around an Runnable.
		 * @param runnable The Runnable to run.
		 * @param priority The optional priority of the task.
		 * @param destroyOnFinish True if the task should be deleted once it's finished.
		 */
		AsyncRunnable(Runnable* runnable, U32 priority = 0, Boolean destroyOnFinish = false);
		~AsyncRunnable();

		/**
		 * The run() method simple runs the Runnable.
		 * @return The return value of the Runnable's run() method.
		 */
		I32 run();

		/**
		 * Method to ready the AsyncRunnable for deletion.
		 */
		void destroy();

		/**
		 * Method is called after the function has run, and deletes the runnable 
		 * object created if neccessary.
		 */
		void onCompletion();

		/**
		 * A method used to get info about the task/
		 * @return An info string about the task.
		 */
		char* getInfo() const;

		/**
		 * The static method to wrap around a existing Runnable*.  
		 * The AsyncRunnable is deleted once it is finished running.
		 * @param runnable The Runnable to wrap around.
		 * @param priority The optional priority.
		 */
		static AsyncRunnable* createAsyncRunnable(Runnable* runnable, U32 priority = 0);

	  private:
		Runnable*				runnable_;
	};

	/**
	 * A simple AsyncResult class that just allows waiting for completion.
	 */
	class AsyncRunnableResult : public AsyncResult {
	  public:
		/**
		 * Creates an AsyncResult attached to the specified 
		 * AsyncRunnable.
		 * @param object The AsyncRunnable the result belongs to.
		 */
		AsyncRunnableResult(AsyncRunnable* object) 
			: AsyncResult(reinterpret_cast<AsyncTask*>(object)) {}
		
		~AsyncRunnableResult() { setTask(NIL); }
		
		/**
		 * Returns NIL
		 * @return nil pointer
		 */
		VPtr getResult() { return NIL; }

		/**
		 * Sets the task to have completed.
		 */
		void taskCompleted();

	};

} // namespace Cat

#endif // CAT_CORE_THREADING_ASYNCRUNNABLE_H

